// Directed + randomized reference-account regression for dma_research_policy.sv
// Build with Verilator (4.038 is the reference toolchain of this repo).
// Simulation only. No board claim, no formal claim.
//
// argv[1] variant selector:
//   "full"  -> Enabled=1, CounterWidth=64 (default build)
//   "small" -> Enabled=1, CounterWidth=8  (deliberate statistics wrap)
//   "off"   -> Enabled=0                  (feature absent, magic readable)
//
// The reference model below mirrors the corrected contract; it does NOT copy
// the previous (buggy) behaviour.  rd() never advances the clock, so register
// reads never perturb billing.
#include "Vdma_research_policy.h"
#include "verilated.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

static int checks = 0;
static void check(bool c, const char *m) {
    if (!c) throw std::runtime_error(m);
    checks++;
}

// ---------------------------------------------------------------------------
// register offsets (in-bank) and full E-bank index helper
// ---------------------------------------------------------------------------
enum {
    R_MAGIC = 0x00, R_VERSION = 0x01, R_ERROR = 0x02, R_STATUS = 0x03,
    R_COMMAND = 0x04, R_MODE = 0x05, R_WINDOW = 0x06, R_TARGET = 0x07,
    R_DEADBAND = 0x08, R_INITBUD = 0x09, R_MINBUD = 0x0A, R_MAXBUD = 0x0B,
    R_STEP = 0x0C, R_CAP = 0x0D, R_WHIT = 0x0E, R_WMISS = 0x0F, R_WPTW = 0x10
};
enum {
    S_WIN = 0x20, S_VDONE, S_VBYTES, S_SHORTW, S_SHORTB, S_PERMCLOSE, S_CHARGE,
    S_REPLEN, S_AISSUE, S_ADONE, S_CHARGECYC, S_HIT, S_MISS, S_PTW, S_AOWN,
    S_SEL, S_LUTA, S_LUTAMISS, S_ACTUALCYC, S_WINALL, S_WINPART, S_FBUP,
    S_FBDN, S_GAPL, S_STALE, S_ARMEDCYC,
    S_BALANCE = 0x3A, S_BMIN = 0x3B, S_BMAX = 0x3C, S_BFINAL = 0x3D,
    S_NODONE = 0x3E, S_HIST = 0x40
};
enum {
    E_BADADDR = 0, E_BADCMD = 1, E_UNSUP = 2, E_CFGLOCK = 3, E_CNTOVF = 4,
    E_BALOVF = 5, E_CLEARILL = 6, E_FRZILL = 7, E_ARMSTATE = 8, E_OBS = 9,
    E_BADCFG = 10, E_SUPDROP = 11
};

static const uint32_t DID_A   = 11;      // 24'd11 -> A owner (0x0B)
static const uint32_t DID_NOT = 0x10;    // 16 -> must NOT be treated as A
static const uint32_t DID_B   = 0x22;
static const uint32_t WINDOW_MAX_DEF = 16777215u;

// full peripheral E-bank index for an in-bank offset
static inline uint32_t E(uint32_t off) { return 0xE00u + off; }

// ---------------------------------------------------------------------------
// stimulus bundle
// ---------------------------------------------------------------------------
struct In {
    bool     quiet = false;
    bool     supported = true;
    bool     v_active = false, a_issue = false, a_done = false, v_done = false;
    uint32_t ctl_state = 0, ctl_did = DID_A;
    bool     select_fire = false; uint32_t select_did = DID_A;
    bool     lookup = false, lookup_hit = false; uint32_t lookup_did = DID_A;
    bool     ptw_r_fire = false, response_error = false;
};

struct Obs { bool en, allow, abort; };

struct Bench {
    Vdma_research_policy d;
    In cur;

    Bench() { reset(); }

    void apply(const In &in) {
        cur = in;
        d.quiet_i        = in.quiet;
        d.supported_i    = in.supported;
        d.v_active_i     = in.v_active;
        d.a_issue_i      = in.a_issue;
        d.a_done_i       = in.a_done;
        d.v_done_i       = in.v_done;
        d.ctl_state_i    = in.ctl_state;
        d.ctl_did_i      = in.ctl_did;
        d.select_fire_i  = in.select_fire;
        d.select_did_i   = in.select_did;
        d.lookup_i       = in.lookup;
        d.lookup_hit_i   = in.lookup_hit;
        d.lookup_did_i   = in.lookup_did;
        d.ptw_r_fire_i   = in.ptw_r_fire;
        d.response_error_i = in.response_error;
    }

    void edge() { d.clk_i = 1; d.eval(); d.clk_i = 0; d.eval(); }

    void reset() {
        d.clk_i = 0;
        d.cfg_valid_i = 0; d.cfg_write_i = 0; d.index_i = 0; d.wdata_i = 0;
        apply(In());
        d.rst_ni = 0; edge(); edge();
        d.rst_ni = 1; edge();
    }

    // drive inputs, sample registered-state-driven outputs before the edge
    Obs drive(const In &in) {
        apply(in);
        d.clk_i = 0; d.eval();
        Obs o{ (bool)d.policy_enable_o, (bool)d.policy_allow_o, (bool)d.policy_abort_o };
        return o;
    }

    // register write at an in-bank offset (full E-bank index is generated)
    //
    // Verilator (4.038 reference toolchain) has no delta cycles: a changed
    // combinational input must be evaluated at the LOW clock before the
    // posedge, otherwise the clocked block samples the pre-change values of
    // wr_any / wr_ok / new_err and the write never lands.
    void wr(uint32_t off, uint64_t data) {
        d.cfg_valid_i = 1; d.cfg_write_i = 1;
        d.index_i = (uint16_t)E(off); d.wdata_i = data;
        d.clk_i = 0; d.eval();            // settle changed combinational inputs
        edge();
        d.cfg_valid_i = 0; d.cfg_write_i = 0; d.wdata_i = 0; d.eval();
    }

    // raw-index write (used for out-of-bank bad-address checks)
    void wr_raw(uint32_t idx, uint64_t data) {
        d.cfg_valid_i = 1; d.cfg_write_i = 1;
        d.index_i = (uint16_t)idx; d.wdata_i = data;
        d.clk_i = 0; d.eval();            // settle changed combinational inputs
        edge();
        d.cfg_valid_i = 0; d.cfg_write_i = 0; d.wdata_i = 0; d.eval();
    }

    // reads are purely combinational - no clock edge, no billing side effect
    uint64_t rd(uint32_t off) {
        d.cfg_valid_i = 1; d.cfg_write_i = 0;
        d.index_i = (uint16_t)E(off); d.eval();
        uint64_t v = (uint64_t)d.rdata_o;
        d.cfg_valid_i = 0; d.eval();
        return v;
    }

    uint64_t rd_raw(uint32_t idx) {
        d.cfg_valid_i = 1; d.cfg_write_i = 0;
        d.index_i = (uint16_t)idx; d.eval();
        uint64_t v = (uint64_t)d.rdata_o;
        d.cfg_valid_i = 0; d.eval();
        return v;
    }
};

// ---------------------------------------------------------------------------
// cycle-accurate software reference of the corrected contract
// ---------------------------------------------------------------------------
struct RefObs { bool en, allow, abort; };

struct Ref {
    bool enabled = true;
    int  cw = 64;
    uint32_t did_a = DID_A;

    uint32_t mode = 0, window = 16384, target = 51540, deadband = 4096,
             initbud = 8000, minbud = 1, maxbud = 32768, step = 512, cap = 65536,
             whit = 1, wmiss = 100, wptw = 100;

    bool armed = false, frozen = false, aborted = false;
    uint64_t err = 0;
    int64_t  balance = 8000;
    uint64_t b = 8000, bmin = 8000, bmax = 8000;
    uint64_t prev_main = 0, win_main = 0;
    uint64_t win_cnt = 0, win_vdone = 0;
    bool     win_allact = false;
    uint64_t nodone_run = 0, nodone_max = 0;
    uint64_t m6_cd = 0;
    uint64_t stat[26];
    uint64_t hist[32];

    enum { ST_WIN = 0, ST_VDONE, ST_VBYTES, ST_SHORTW, ST_SHORTB, ST_PERMCLOSE,
           ST_CHARGE, ST_REPLEN, ST_AISSUE, ST_ADONE, ST_CHARGECYC, ST_HIT,
           ST_MISS, ST_PTW, ST_AOWN, ST_SEL, ST_LUTA, ST_LUTAMISS,
           ST_ACTUALCYC, ST_WINALL, ST_WINPART, ST_FBUP, ST_FBDN, ST_GAPL,
           ST_STALE, ST_ARMEDCYC };

    Ref() { std::memset(stat, 0, sizeof(stat)); std::memset(hist, 0, sizeof(hist)); }

    uint64_t mask() const {
        return (cw >= 64) ? ~0ull : ((1ull << cw) - 1ull);
    }

    bool config_bad() const {
        if (mode > 7)                         return true;
        if (window == 0)                      return true;
        if (window > WINDOW_MAX_DEF)          return true;
        if (minbud > maxbud)                  return true;
        if (initbud < minbud || initbud > maxbud) return true;
        if (step == 0)                        return true;
        if (cap < maxbud)                     return true;
        if (mode != 0 && mode != 6 && minbud == 0) return true;
        if (mode == 4 && ((uint64_t)whit + wmiss + wptw) == 0) return true;
        return false;
    }

    void do_reset() {
        armed = frozen = aborted = false;
        err = 0;
        balance = (int64_t)initbud;
        b = bmin = bmax = initbud;
        prev_main = win_main = 0;
        win_cnt = win_vdone = 0;
        win_allact = false;
        nodone_run = nodone_max = 0;
        m6_cd = 0;
        std::memset(stat, 0, sizeof(stat));
        std::memset(hist, 0, sizeof(hist));
    }

    void arm_now() {
        armed = true; frozen = false; aborted = false;
        balance = (int64_t)initbud;   // no clamping
        b = bmin = bmax = initbud;
        prev_main = win_main = 0;
        win_cnt = win_vdone = 0;
        win_allact = true;
        m6_cd = 0;
        nodone_run = 0;
    }

    void set_cfg(uint32_t off, uint32_t v) {
        switch (off) {
            case R_MODE:     mode = v; break;
            case R_WINDOW:   window = v; break;
            case R_TARGET:   target = v; break;
            case R_DEADBAND: deadband = v; break;
            case R_INITBUD:  initbud = v; break;
            case R_MINBUD:   minbud = v; break;
            case R_MAXBUD:   maxbud = v; break;
            case R_STEP:     step = v; break;
            case R_CAP:      cap = v; break;
            case R_WHIT:     whit = v; break;
            case R_WMISS:    wmiss = v; break;
            case R_WPTW:     wptw = v; break;
            default: break;
        }
    }

    // full-index write decode; returns the error bits raised by the write
    uint64_t decode_write(uint32_t idx, uint64_t data, bool quiet,
                          bool &f_arm, bool &f_abort, bool &f_freeze,
                          bool &f_clear, bool &f_cfg) {
        uint64_t ne = 0;
        f_arm = f_abort = f_freeze = f_clear = f_cfg = false;

        bool bank_ok = ((idx >> 8) & 0x1Fu) == 0x0Eu;
        uint32_t off = idx & 0xFFu;
        bool ok = bank_ok && (off >= R_COMMAND) && (off <= R_WPTW)
                  && ((data >> 32) == 0);
        if (!ok) { ne |= (1ull << E_BADADDR); return ne; }

        if (off == R_COMMAND) {
            uint32_t cmd = (uint32_t)data;
            if (cmd == 1) {                                   // ARM
                if (!enabled || !in_supported)      ne |= (1ull << E_UNSUP);
                else if (armed || frozen || !quiet) ne |= (1ull << E_ARMSTATE);
                else if (config_bad())              ne |= (1ull << E_BADCFG);
                else                                f_arm = true;
            } else if (cmd == 2) {                            // ABORT
                if (!armed || frozen)               ne |= (1ull << E_ARMSTATE);
                else if (!enabled || !in_supported) ne |= (1ull << E_UNSUP);
                else                                f_abort = true;
            } else if (cmd == 3) {                            // FREEZE
                if (!armed || frozen)               ne |= (1ull << E_ARMSTATE);
                else if (!quiet)                    ne |= (1ull << E_FRZILL);
                else                                f_freeze = true;
            } else if (cmd == 4) {                            // CLEAR
                if (!quiet || armed)                ne |= (1ull << E_CLEARILL);
                else                                f_clear = true;
            } else {
                ne |= (1ull << E_BADCMD);
            }
        } else {
            f_cfg = true;
            if (armed) ne |= (1ull << E_CFGLOCK);
        }
        return ne;
    }

    // in_supported mirrors the DUT supported_i for the current cycle
    bool in_supported = true;

    RefObs cycle(const In &in, bool do_wr, uint32_t widx, uint64_t wdata) {
        in_supported = in.supported;

        // ---- events / cost -------------------------------------------------
        bool ev_sel_a  = in.select_fire && in.select_did == did_a;
        bool ev_lut_a  = in.lookup && in.lookup_did == did_a;
        bool ev_hit_a  = ev_lut_a && in.lookup_hit;
        bool ev_miss_a = ev_lut_a && !in.lookup_hit;
        bool ev_ptw_a  = in.ptw_r_fire && in.ctl_did == did_a;
        bool ev_act_a  = (in.ctl_did == did_a) && (in.ctl_state == 1 || in.ctl_state == 2);
        bool ev_aown   = (in.ctl_did == did_a);

        uint64_t cost = 0;
        switch (mode) {
            case 0: cost = 0; break;
            case 1: cost = in.a_issue ? 4096 : 0; break;
            case 2: cost = ev_sel_a ? 1 : 0; break;
            case 3: cost = ev_miss_a ? 1 : 0; break;
            case 4: cost = (ev_hit_a ? whit : 0) + (ev_miss_a ? wmiss : 0)
                         + (ev_ptw_a ? wptw : 0); break;
            case 5: cost = ev_act_a ? 1 : 0; break;
            default: cost = 0; break;
        }

        // ---- window --------------------------------------------------------
        bool win_run  = armed && !frozen;
        bool win_last = win_run && window != 0 && win_cnt == (uint64_t)window - 1;
        bool act_all  = win_allact && in.v_active;
        uint64_t vdone_now = win_vdone + (in.v_done ? 1 : 0);
        bool win_complete = win_last && act_all;
        bool stale = win_last && mode == 7;
        uint64_t charge_now = (win_run ? cost : 0) + (stale ? prev_main : 0);

        // refill at every complete window end, independent of V activity
        bool refill_en = win_last && mode != 0 && mode != 6;
        uint64_t refill_now = refill_en ? b : 0;

        // ---- balance (66-bit intermediate; mid-charge underflow detected) --
        __int128 balc = (__int128)balance - (__int128)charge_now;
        bool balc_ovf = (balc > (__int128)INT64_MAX) || (balc < (__int128)INT64_MIN);
        __int128 balr = balc + (__int128)refill_now;
        if (refill_en && balr > (__int128)cap) balr = (__int128)cap;
        bool balr_ovf = (balr > (__int128)INT64_MAX) || (balr < (__int128)INT64_MIN);
        bool bal_ovf = balc_ovf || balr_ovf;
        int64_t bal_next = bal_ovf ? (balr > 0 ? INT64_MAX : INT64_MIN) : (int64_t)balr;

        // ---- feedback (direction unchanged) --------------------------------
        __int128 obs = (__int128)(vdone_now * 4096ull);
        __int128 tgt = (__int128)target;
        __int128 db  = (__int128)deadband;
        bool fb_inc = (mode == 6) ? (obs + db < tgt) : (obs > tgt + db);
        bool fb_dec = (mode == 6) ? (obs > tgt + db) : (obs + db < tgt);

        int64_t up = (int64_t)b + (int64_t)step;
        uint64_t b_inc_val = (up > (int64_t)maxbud) ? maxbud : (uint64_t)up;
        int64_t dn = (int64_t)b - (int64_t)step;
        uint64_t b_dec_val = (dn < (int64_t)minbud) ? minbud : (uint64_t)dn;
        bool apply_fb = win_complete && mode != 0 && (fb_inc || fb_dec);
        uint64_t b_next = apply_fb ? (fb_inc ? b_inc_val : b_dec_val) : b;

        // ---- mode6 countdown: every A done loads the gap -------------------
        uint64_t cd_dec = (win_run && m6_cd != 0) ? m6_cd - 1 : m6_cd;
        uint64_t cd_next = cd_dec;
        bool gap_load = false;
        if (win_run && mode == 6 && in.a_done) { cd_next = b; gap_load = true; }

        // ---- write decode ---------------------------------------------------
        bool f_arm = false, f_abort = false, f_freeze = false, f_clear = false,
             f_cfg = false;
        uint64_t ne = do_wr ? decode_write(widx, wdata, in.quiet,
                                           f_arm, f_abort, f_freeze, f_clear, f_cfg)
                            : 0;

        // ---- allow / abort ---------------------------------------------------
        bool enable_core = enabled && armed && !frozen;
        bool allow_core = (mode == 0) ? true
                        : (mode == 6) ? (cd_next == 0)
                                      : (bal_next >= 0);
        bool abort_eff = aborted || f_abort;

        bool obs_err_now  = armed && !frozen && in.response_error;
        bool sup_drop_now = armed && !in.supported;
        bool err_any_reg  = (err != 0);
        bool perm_closed  = enable_core && (abort_eff || err_any_reg || !allow_core);

        // ---- statistics increments -----------------------------------------
        uint64_t inc[26] = {0};
        uint64_t hinc[32] = {0};
        inc[ST_ARMEDCYC] = win_run ? 1 : 0;
        inc[ST_PERMCLOSE]= perm_closed ? 1 : 0;
        inc[ST_CHARGE]   = charge_now;
        inc[ST_CHARGECYC]= (charge_now != 0) ? 1 : 0;
        inc[ST_REPLEN]   = refill_now;
        inc[ST_VDONE]    = (win_run && in.v_done)  ? 1 : 0;
        inc[ST_VBYTES]   = (win_run && in.v_done)  ? 4096 : 0;
        inc[ST_AISSUE]   = (win_run && in.a_issue) ? 1 : 0;
        inc[ST_ADONE]    = (win_run && in.a_done)  ? 1 : 0;
        inc[ST_HIT]      = (win_run && ev_hit_a)   ? 1 : 0;
        inc[ST_MISS]     = (win_run && ev_miss_a)  ? 1 : 0;
        inc[ST_PTW]      = (win_run && ev_ptw_a)   ? 1 : 0;
        inc[ST_AOWN]     = (win_run && ev_aown)    ? 1 : 0;
        inc[ST_SEL]      = (win_run && ev_sel_a)   ? 1 : 0;
        inc[ST_LUTA]     = (win_run && ev_lut_a)   ? 1 : 0;
        inc[ST_LUTAMISS] = (win_run && ev_miss_a)  ? 1 : 0;
        inc[ST_ACTUALCYC]= (win_run && ev_act_a)   ? 1 : 0;
        inc[ST_WIN]      = win_complete ? 1 : 0;
        inc[ST_WINALL]   = win_last ? 1 : 0;
        inc[ST_WINPART]  = (win_last && !act_all) ? 1 : 0;
        inc[ST_GAPL]     = gap_load ? 1 : 0;
        inc[ST_STALE]    = stale ? 1 : 0;
        bool shortfall = win_complete && (obs < tgt);
        inc[ST_SHORTW]   = shortfall ? 1 : 0;
        inc[ST_SHORTB]   = shortfall ? (uint64_t)(tgt - obs) : 0;
        inc[ST_FBUP]     = (win_complete && mode != 0 && fb_inc) ? 1 : 0;
        inc[ST_FBDN]     = (win_complete && mode != 0 && fb_dec) ? 1 : 0;

        // counter overflow: compare the full sum against the counter mask so
        // an increment larger than the counter range is still reported
        bool cnt_ovf = false;
        uint64_t m = mask();
        uint64_t nstat[26];
        for (int i = 0; i < 26; i++) {
            unsigned __int128 s = (unsigned __int128)stat[i] + inc[i];
            if (s > (unsigned __int128)m) cnt_ovf = true;
            nstat[i] = (uint64_t)s & m;
        }
        uint64_t nhist[32];
        for (int i = 0; i < 32; i++) nhist[i] = hist[i];
        if (win_complete) {
            int hb = (vdone_now >= 31) ? 31 : (int)vdone_now;
            unsigned __int128 s = (unsigned __int128)hist[hb] + 1;
            if (s > (unsigned __int128)m) cnt_ovf = true;
            nhist[hb] = (uint64_t)s & m;
        }

        bool err_any_now = (ne != 0) || obs_err_now || sup_drop_now || cnt_ovf || bal_ovf;
        bool block_all = err_any_reg || err_any_now;

        RefObs o;
        o.en    = enable_core;
        o.allow = enable_core && !abort_eff && !block_all && allow_core;
        o.abort = enable_core && (abort_eff || block_all);

        // ---- commit ---------------------------------------------------------
        if (f_clear) {
            std::memset(stat, 0, sizeof(stat));
            std::memset(hist, 0, sizeof(hist));
            err = 0;
            armed = frozen = aborted = false;
            balance = (int64_t)initbud;
            b = bmin = bmax = initbud;
            prev_main = win_main = 0;
            win_cnt = win_vdone = 0;
            win_allact = false;
            m6_cd = 0;
            nodone_run = nodone_max = 0;
            return o;
        }

        for (int i = 0; i < 26; i++) stat[i] = nstat[i];
        for (int i = 0; i < 32; i++) hist[i] = nhist[i];
        err |= ne;
        if (obs_err_now)  err |= (1ull << E_OBS);
        if (sup_drop_now) err |= (1ull << E_SUPDROP);
        if (cnt_ovf)      err |= (1ull << E_CNTOVF);
        if (bal_ovf)      err |= (1ull << E_BALOVF);

        if (f_cfg && !armed) set_cfg(widx & 0xFFu, (uint32_t)wdata);

        // datapath (never concurrent with ARM/CLEAR)
        if (win_run && !f_arm && !f_clear) {
            balance = bal_next;
            if (win_last) { win_cnt = 0; win_allact = true; win_vdone = 0; }
            else          { win_cnt++; win_allact = act_all; win_vdone = vdone_now; }
            if (win_last) { prev_main = win_main + (mode == 7 && ev_act_a ? 1 : 0);
                            win_main = 0; }
            else          { win_main += (mode == 7 && ev_act_a ? 1 : 0); }
            if (apply_fb) {
                b = b_next;
                if (b < bmin) bmin = b;
                if (b > bmax) bmax = b;
            }
            m6_cd = cd_next;
        }

        if (f_arm) arm_now();
        if (f_abort) aborted = true;
        // FREEZE retains aborted_q: diagnostics survive until CLEAR
        if (f_freeze) { armed = false; frozen = true; }

        {
            bool act_now = win_run && in.v_active && !in.v_done;
            nodone_run = act_now ? nodone_run + 1 : 0;
            if (nodone_run > nodone_max) nodone_max = nodone_run;
        }
        return o;
    }
};

static void cmp_obs(const Obs &a, const RefObs &b, const char *where) {
    if (a.en != b.en || a.allow != b.allow || a.abort != b.abort) {
        char msg[256];
        std::snprintf(msg, sizeof(msg),
                      "%s: rtl en=%d allow=%d abort=%d ; ref en=%d allow=%d abort=%d",
                      where, a.en, a.allow, a.abort, b.en, b.allow, b.abort);
        throw std::runtime_error(msg);
    }
    checks++;
}

// ---------------------------------------------------------------------------
// directed helpers
// ---------------------------------------------------------------------------
// In{} defaults to quiet=false and `b.cur.quiet = true` only mutates the
// software copy - it never drives d.quiet_i.  Build every quiet (fully
// drained) stimulus through this helper instead.
static In quiet_in() { In in; in.quiet = true; return in; }

static void cfg(Ref &r, Bench &b, uint32_t off, uint32_t v) {
    b.wr(off, v);
    r.set_cfg(off, v);
}

static void cmd(Bench &b, Ref &r, uint32_t c) {
    b.wr(R_COMMAND, c);
    bool a, ab, f, cl, cg;
    uint64_t ne = r.decode_write(E(R_COMMAND), c, b.cur.quiet, a, ab, f, cl, cg);
    if (cl) {
        r.do_reset();
    } else {
        r.err |= ne;
        if (a)  r.arm_now();
        if (ab) r.aborted = true;
        if (f)  { r.armed = false; r.frozen = true; }
    }
}

// cleanup: FREEZE (quiet) then CLEAR leaves the block idle with no error
static void cleanup(Bench &b) {
    b.apply(quiet_in());
    b.wr(R_COMMAND, 3);
    b.wr(R_COMMAND, 4);
}

// ---------------------------------------------------------------------------
// tests
// ---------------------------------------------------------------------------
static void t_reset_defaults(Bench &b) {
    b.reset();
    check(b.rd(R_MAGIC) == 0x49444D414D435431ull, "magic IDMAMCT1 at 0xE00");
    check(b.rd(R_VERSION) == 1, "version 1");
    check(b.rd(R_ERROR) == 0, "reset error zero");
    check(b.rd(R_STATUS) == 0, "reset status zero");
    check(b.rd(R_MODE) == 0, "default mode 0");
    check(b.rd(R_WINDOW) == 16384, "default window 16384");
    check(b.rd(R_TARGET) == 51540, "default target 51540");
    check(b.rd(R_DEADBAND) == 4096, "default deadband 4096");
    check(b.rd(R_INITBUD) == 8000, "default initial budget 8000");
    check(b.rd(R_MINBUD) == 1, "default min 1");
    check(b.rd(R_MAXBUD) == 32768, "default max 32768");
    check(b.rd(R_STEP) == 512, "default step 512");
    check(b.rd(R_CAP) == 65536, "default cap 65536");
    check(b.rd(R_WHIT) == 1 && b.rd(R_WMISS) == 100 && b.rd(R_WPTW) == 100,
          "default weights 1/100/100");
    for (uint32_t i = S_WIN; i <= S_ARMEDCYC; i++)
        check(b.rd(i) == 0, "reset statistic zero");
    // reads outside the bank return 0 and never raise an error
    check(b.rd_raw(0x0000) == 0, "read outside E bank returns 0");
    check(b.rd_raw(0xE60) == 0, "read E60 returns 0");
    check(b.rd(R_ERROR) == 0, "reads never set sticky errors");
}

static void t_bad_writes_and_sticky(Bench &b) {
    b.reset();

    b.wr_raw(0x60, 0);                               // outside E bank
    check((b.rd(R_ERROR) >> E_BADADDR) & 1, "bad address sticky");
    b.wr(R_MODE, 0x100000000ull);                    // high bits nonzero
    check((b.rd(R_ERROR) >> E_BADADDR) & 1, "high bits sticky");
    b.wr(R_COMMAND, 99);                             // unknown command
    check((b.rd(R_ERROR) >> E_BADCMD) & 1, "bad command sticky");
    b.wr(R_COMMAND, 4);                              // CLEAR while not quiet
    check((b.rd(R_ERROR) >> E_CLEARILL) & 1, "illegal clear sticky");
    check((b.rd(R_ERROR) >> E_BADCMD) & 1, "illegal clear kept earlier bits");

    // sticky error blocks admission and asserts abort while disabled/idle
    // (enable_core stays 0 when not armed) then a legal CLEAR recovers
    b.apply(quiet_in());
    b.wr(R_COMMAND, 4);
    check(b.rd(R_ERROR) == 0, "legal CLEAR clears sticky errors");
    b.apply(In());
}

static void t_arm_predicates(Bench &b) {
    b.reset();
    In in; in.quiet = false; in.supported = false;
    b.apply(in);
    b.wr(R_COMMAND, 1);                              // ARM not quiet, unsupported
    check((b.rd(R_ERROR) >> E_UNSUP) & 1, "unsupported ARM refused");
    check(b.rd(R_STATUS) == 0, "still not armed");
    b.apply(quiet_in());
    b.wr(R_COMMAND, 4);
    check(b.rd(R_ERROR) == 0, "recovered");
}

static void t_config_lock(Bench &b) {
    b.reset();
    In in; in.quiet = true;
    b.apply(in);
    b.wr(R_MODE, 0);
    b.wr(R_COMMAND, 1);                              // ARM
    check(b.rd(R_STATUS) & 1, "armed");
    b.wr(R_WINDOW, 7);                               // locked
    check(b.rd(R_WINDOW) == 16384, "window unchanged while armed");
    check((b.rd(R_ERROR) >> E_CFGLOCK) & 1, "config lock sticky");
    // recovery: FREEZE leaves armed=0/frozen=1, then CLEAR is legal
    b.apply(quiet_in());
    b.wr(R_COMMAND, 3);
    check(b.rd(R_STATUS) == 2, "successful FREEZE status 2 (frozen, not aborted)");
    b.wr(R_COMMAND, 4);
    check(b.rd(R_STATUS) == 0, "cleared after freeze");
    check(b.rd(R_ERROR) == 0, "CLEAR clears config-lock");
}

static void t_first_allow_and_mode0(Bench &b) {
    b.reset();
    In in; in.quiet = true; in.supported = true;
    b.apply(in);
    b.wr(R_MODE, 0);
    b.wr(R_COMMAND, 1);
    in = In(); in.quiet = true; in.v_active = true;
    Obs o = b.drive(in);
    check(o.en && o.allow && !o.abort, "mode0 first ADMIT allowed");
    for (int i = 0; i < 100; i++) { o = b.drive(in); b.edge(); }
    check(o.allow, "mode0 always allow");
    check(b.rd(S_CHARGE) == 0, "mode0 never charges");
    cleanup(b);
}

static void t_mode1_charge_and_debt(Bench &b, Ref &r, bool narrow) {
    b.reset(); r.do_reset(); r.cw = narrow ? 8 : 64;
    In idle; idle.quiet = true;
    b.apply(idle);
    cfg(r, b, R_MODE, 1);
    cfg(r, b, R_WINDOW, 64);
    cfg(r, b, R_INITBUD, 4);          // tiny budget -> debt after 2 issues
    cfg(r, b, R_MINBUD, 1);
    cfg(r, b, R_MAXBUD, 32768);
    cfg(r, b, R_STEP, 512);
    cfg(r, b, R_CAP, 65536);
    cfg(r, b, R_TARGET, 0);
    cfg(r, b, R_DEADBAND, 0);
    cmd(b, r, 1);                     // ARM

    // no clamping: exactly the programmed initial budget
    check((int64_t)b.rd(S_BALANCE) == 4, "initial balance not clamped");

    In in; in.quiet = true;
    Obs o = b.drive(in);
    check(o.en && o.allow, "first admit allowed with positive budget");
    b.edge();

    in.a_issue = true;
    o = b.drive(in);
    check(o.en && !o.allow, "debt blocks further admission");
    b.edge();
    in.a_issue = false;

    // refill happens at every window end now, but debt is never forgiven
    for (int i = 0; i < 70; i++) { b.drive(in); b.edge(); }
    o = b.drive(in);
    int64_t bal = (int64_t)b.rd(S_BALANCE);
    check(b.rd(S_CHARGE) == 4096, "single 4096 charge");
    check(b.rd(S_REPLEN) > 0, "refill happened at window end");
    check(bal < 0, "debt preserved below any refill");
    cleanup(b);
}

static void t_window_feedback(Bench &b) {
    b.reset();
    In idle; idle.quiet = true; b.apply(idle);
    b.wr(R_MODE, 1);
    b.wr(R_WINDOW, 8);
    b.wr(R_INITBUD, 8000);
    b.wr(R_MINBUD, 1);
    b.wr(R_MAXBUD, 32768);
    b.wr(R_STEP, 512);
    b.wr(R_CAP, 65536);
    b.wr(R_TARGET, 8192);
    b.wr(R_DEADBAND, 0);
    b.wr(R_COMMAND, 1);

    // one fully V-active window with zero V done -> b up, bucket 0
    In in; in.quiet = true; in.v_active = true;
    for (int i = 0; i < 8; i++) { b.drive(in); b.edge(); }
    check(b.rd(S_WIN) == 1, "one complete active window");
    check(b.rd(S_HIST + 0) == 1, "histogram bucket 0");
    check(b.rd(S_FBUP) == 1 && b.rd(S_FBDN) == 0, "observed below target -> b up");
    check(b.rd(S_BFINAL) == 8000 + 512, "b stepped up by 512");
    check(b.rd(S_SHORTW) == 1 && b.rd(S_SHORTB) == 8192, "shortfall recorded");

    // one fully V-active window with 8 V done (32768 B > target) -> b down
    In busy = in; busy.v_done = true;
    for (int i = 0; i < 8; i++) { b.drive(busy); b.edge(); }
    check(b.rd(S_WIN) == 2, "two complete active windows");
    check(b.rd(S_HIST + 8) == 1, "histogram bucket 8");
    check(b.rd(S_FBUP) == 1 && b.rd(S_FBDN) == 1, "b stepped down once");
    check(b.rd(S_REPLEN) == 8000 + (8000 + 512), "two refills from the pre-window b");

    // partial window (one idle cycle): no feedback, but refill still happens
    uint64_t before   = b.rd(S_BFINAL);
    uint64_t replen0  = b.rd(S_REPLEN);
    In gap = in; gap.v_active = false;
    b.drive(gap); b.edge();
    for (int i = 1; i < 8; i++) { b.drive(in); b.edge(); }
    check(b.rd(S_WINPART) == 1, "partial window counted separately");
    check(b.rd(S_BFINAL) == before, "partial window does not update b");
    check(b.rd(S_REPLEN) == replen0 + before, "partial window still refills");
    cleanup(b);
}

// E25 permission_closed_cycles: registered drift only, no a_wait claim
static void t_permission_closed(Bench &b) {
    b.reset();
    In idle; idle.quiet = true; b.apply(idle);
    b.wr(R_MODE, 1);
    b.wr(R_WINDOW, 64);
    b.wr(R_INITBUD, 4);
    b.wr(R_COMMAND, 1);
    In in; in.quiet = true; in.a_issue = true;
    b.drive(in); b.edge();              // one 4096 charge -> balance negative
    in.a_issue = false;
    uint64_t closed0 = b.rd(S_PERMCLOSE);
    for (int i = 0; i < 10; i++) { b.drive(in); b.edge(); }
    uint64_t closed1 = b.rd(S_PERMCLOSE);
    check(closed1 > closed0, "permission_closed_cycles counts denied cycles");
    cleanup(b);
}

static void t_mode6_gap(Bench &b) {
    b.reset();
    In idle; idle.quiet = true; b.apply(idle);
    b.wr(R_MODE, 6);
    b.wr(R_WINDOW, 64);
    b.wr(R_INITBUD, 3);
    b.wr(R_MINBUD, 0);
    b.wr(R_MAXBUD, 32768);
    b.wr(R_COMMAND, 1);

    In in; in.quiet = true;
    Obs o = b.drive(in);
    check(o.en && o.allow, "mode6 first admit allowed (countdown 0)");
    b.edge();

    // first A done already loads the gap (no skipped first done)
    in.a_done = true;
    o = b.drive(in);
    check(!o.allow, "first A done loads the gap in the same cycle decision");
    b.edge();
    in.a_done = false;
    check(b.rd(S_GAPL) == 1, "exactly one gap load after the first A done");

    o = b.drive(in); check(!o.allow, "countdown 3 -> blocked"); b.edge();
    o = b.drive(in); check(!o.allow, "countdown 2 -> blocked"); b.edge();
    o = b.drive(in); check(!o.allow, "countdown 1 -> blocked"); b.edge();
    o = b.drive(in); check(o.allow, "countdown exhausted -> allow");
    check(b.rd(S_GAPL) == 1, "still exactly one gap load");
    cleanup(b);
}

static void t_mode7_stale(Bench &b) {
    b.reset();
    In idle; idle.quiet = true; b.apply(idle);
    b.wr(R_MODE, 7);
    b.wr(R_WINDOW, 8);
    b.wr(R_COMMAND, 1);

    // window 0: 8 A REQ_TRANS cycles -> win_main = 8
    In in; in.quiet = true; in.ctl_did = DID_A; in.ctl_state = 1;
    for (int i = 0; i < 8; i++) { b.drive(in); b.edge(); }
    check(b.rd(S_CHARGE) == 0, "first window has no stale charge");

    // window 1: A-owner but idle (not REQ_TRANS/AWAIT_RESP) -> stale = 8
    In own = in; own.ctl_state = 0;
    for (int i = 0; i < 8; i++) { b.drive(own); b.edge(); }
    check(b.rd(S_STALE) == 1, "one stale charge");
    check(b.rd(S_CHARGE) == 8, "stale uses actual cycles only");
    check(b.rd(S_AOWN) == 16, "A-owner cycles counted independently");

    // window 2: window 1 main was 0 -> stale 0
    for (int i = 0; i < 8; i++) { b.drive(own); b.edge(); }
    check(b.rd(S_STALE) == 2, "second stale charge");
    check(b.rd(S_CHARGE) == 8, "window 1 had zero actual cycles -> stale 0");
    cleanup(b);
}

static void t_did_ownership(Bench &b) {
    b.reset();
    In in; in.quiet = true; b.apply(in);
    b.wr(R_MODE, 0);
    b.wr(R_COMMAND, 1);

    // A11 (0x0B) is A
    in = In(); in.quiet = true;
    in.select_fire = true; in.select_did = DID_A;
    b.drive(in); b.edge();
    check(b.rd(S_SEL) == 1, "DID 11 select counted as A");

    // A16 (0x10) is NOT A
    in.select_did = DID_NOT;
    b.drive(in); b.edge();
    check(b.rd(S_SEL) == 1, "DID 16 select not counted");

    // actual state cycles: REQ_TRANS + AWAIT_RESP, COMPLETE excluded
    in = In(); in.quiet = true; in.ctl_did = DID_A; in.ctl_state = 1;
    b.drive(in); b.edge();
    in.ctl_state = 2;
    b.drive(in); b.edge();
    in.ctl_state = 4;               // COMPLETE must not be charged
    b.drive(in); b.edge();
    check(b.rd(S_ACTUALCYC) == 2, "REQ_TRANS+AWAIT_RESP counted, COMPLETE not");

    // DID 16 actual cycle not counted
    in.ctl_did = DID_NOT; in.ctl_state = 1;
    b.drive(in); b.edge();
    check(b.rd(S_ACTUALCYC) == 2, "DID 16 actual cycle not counted");

    // A-owner counter still counts both, ownership is ctl_did only
    check(b.rd(S_AOWN) == 1 + 3 + 1, "A-owner cycles count by DID only");
    cleanup(b);
}

static void t_abort_keeps_charging(Bench &b) {
    b.reset();
    In in; in.quiet = true; b.apply(in);
    b.wr(R_MODE, 1);
    b.wr(R_WINDOW, 32);
    b.wr(R_COMMAND, 1);
    b.wr(R_COMMAND, 2);                            // ABORT
    Obs o = b.drive(in);
    check(o.en && !o.allow && o.abort, "abort stops new admission");
    b.edge();
    in.a_issue = true;
    b.drive(in);
    b.edge();
    in.a_issue = false;
    check(b.rd(S_CHARGE) == 4096, "in-flight charging continues after ABORT");
    cleanup(b);
}

// do_abort must block admission in the very cycle it is decoded
static void t_abort_same_cycle(Bench &b) {
    b.reset();
    In in; in.quiet = true; b.apply(in);
    b.wr(R_MODE, 0);
    b.wr(R_COMMAND, 1);
    in = In(); in.quiet = true; in.v_active = true;
    Obs o = b.drive(in);
    check(o.en && o.allow && !o.abort, "armed and allowed");

    b.apply(in);
    b.d.cfg_valid_i = 1; b.d.cfg_write_i = 1;
    b.d.index_i = (uint16_t)E(R_COMMAND); b.d.wdata_i = 2;
    b.d.clk_i = 0; b.d.eval();
    Obs o2{ (bool)b.d.policy_enable_o, (bool)b.d.policy_allow_o,
            (bool)b.d.policy_abort_o };
    check(o2.en && !o2.allow && o2.abort, "ABORT blocks admission same cycle");
    b.d.clk_i = 1; b.d.eval(); b.d.clk_i = 0; b.d.eval();
    b.d.cfg_valid_i = 0; b.d.cfg_write_i = 0; b.d.wdata_i = 0;
    cleanup(b);
}

// FREEZE / CLEAR / next ARM across several runs
static void t_freeze_clear_rearm(Bench &b) {
    b.reset();
    for (int run = 0; run < 3; run++) {
        In in; in.quiet = true; b.apply(in);
        b.wr(R_MODE, 0);
        b.wr(R_COMMAND, 1);
        check(b.rd(R_STATUS) == 1, "armed");
        in.v_active = true;
        for (int i = 0; i < 4; i++) { b.drive(in); b.edge(); }
        b.wr(R_COMMAND, 3);                       // FREEZE (quiet)
        check(b.rd(R_STATUS) == 2, "successful FREEZE status 2");
        b.wr(R_COMMAND, 4);                       // legal CLEAR now
        check(b.rd(R_STATUS) == 0, "CLEAR accepted after FREEZE");
        check(b.rd(R_ERROR) == 0, "no error after freeze/clear");
    }
}

// ABORT that is still latched must survive FREEZE: status stays 6 and the
// diagnostics are retained until a legal CLEAR.
static void t_abort_survives_freeze(Bench &b) {
    b.reset();
    b.apply(quiet_in());
    b.wr(R_MODE, 1);
    b.wr(R_COMMAND, 1);                          // ARM
    check(b.rd(R_STATUS) == 1, "armed (status 1)");
    b.wr(R_COMMAND, 2);                          // ABORT
    check(b.rd(R_STATUS) == 5, "abort latched while still armed (status 5)");

    b.wr(R_COMMAND, 3);                          // FREEZE after ABORT
    check(b.rd(R_STATUS) == 6, "aborted FREEZE status 6");
    check(b.rd(R_ERROR) == 0, "ABORT then FREEZE raise no error bits");
    check((b.rd(R_STATUS) >> 2) & 1, "aborted flag retained across FREEZE");
    check((b.rd(R_STATUS) >> 1) & 1, "frozen flag set by FREEZE");
    check((b.rd(R_STATUS) & 1) == 0, "armed cleared by FREEZE");

    // capture: diagnostics raised while frozen are kept, and only CLEAR drops
    // them together with the frozen/aborted state
    b.wr(R_COMMAND, 3);                          // second FREEZE is illegal
    check((b.rd(R_ERROR) >> E_ARMSTATE) & 1, "diagnostic latched while frozen");
    check(b.rd(R_STATUS) == 6, "illegal FREEZE leaves status 6 unchanged");

    b.wr(R_COMMAND, 4);                          // CLEAR
    check(b.rd(R_STATUS) == 0, "CLEAR drops frozen and aborted");
    check(b.rd(R_ERROR) == 0, "CLEAR drops all diagnostics");
}

// sticky failure diagnostics also survive FREEZE and are only dropped by CLEAR
static void t_error_survives_freeze(Bench &b) {
    b.reset();
    In in; in.quiet = true; in.supported = true;
    b.apply(in);
    b.wr(R_MODE, 0);
    b.wr(R_COMMAND, 1);                          // ARM
    check(b.rd(R_STATUS) == 1, "armed (status 1)");

    in.supported = false;
    Obs o = b.drive(in);
    check(!o.allow && o.abort, "supported drop blocks and aborts");
    b.edge();
    check((b.rd(R_ERROR) >> E_SUPDROP) & 1, "E_SUPDROP sticky");
    check(b.rd(R_STATUS) == 1, "protocol error does not set the abort bit");

    in.supported = true;
    b.apply(in);
    b.wr(R_COMMAND, 3);                          // FREEZE (quiet)
    check(b.rd(R_STATUS) == 2, "FREEZE of an error-only run reads status 2");
    check((b.rd(R_ERROR) >> E_SUPDROP) & 1, "error register survives FREEZE");

    b.wr(R_COMMAND, 4);                          // CLEAR
    check(b.rd(R_STATUS) == 0, "CLEAR drops frozen state");
    check(b.rd(R_ERROR) == 0, "CLEAR drops sticky errors");
}

// refill on a window that is never V-active (prevents A dead-lock)
static void t_refill_inactive_window(Bench &b) {
    b.reset();
    In idle; idle.quiet = true; b.apply(idle);
    b.wr(R_MODE, 1);
    b.wr(R_WINDOW, 8);
    b.wr(R_COMMAND, 1);

    In off; off.quiet = true; off.v_active = false;
    for (int i = 0; i < 8; i++) { b.drive(off); b.edge(); }
    check(b.rd(S_REPLEN) == 8000, "refill happens on an inactive window");
    check(b.rd(S_WIN) == 0, "no complete active window");
    check(b.rd(S_WINPART) == 1, "partial window boundary counted");
    check(b.rd(S_FBUP) == 0 && b.rd(S_FBDN) == 0, "no feedback when V inactive");
    check(b.rd(S_HIST + 0) == 0, "no histogram bucket when V inactive");
    check((int64_t)b.rd(S_BALANCE) == 8000 + 8000, "balance replenished by old b");
    cleanup(b);
}

// cfg_bad coverage: init/step/cap/weights plus no-clamp behaviour
static void t_cfg_bad_and_no_clamp(Bench &b) {
    b.reset();
    In in; in.quiet = true; b.apply(in);

    // init above max -> rejected
    b.wr(R_MODE, 1);
    b.wr(R_MAXBUD, 100);
    b.wr(R_INITBUD, 100000);
    b.wr(R_COMMAND, 1);
    check((b.rd(R_ERROR) >> E_BADCFG) & 1, "init above max rejected");
    check((b.rd(R_STATUS) & 1) == 0, "not armed on bad config");
    b.wr(R_COMMAND, 4);                            // clear sticky

    // step 0 -> rejected
    b.wr(R_INITBUD, 50);
    b.wr(R_STEP, 0);
    b.wr(R_COMMAND, 1);
    check((b.rd(R_ERROR) >> E_BADCFG) & 1, "step 0 rejected");
    b.wr(R_COMMAND, 4);

    // cap below max -> rejected
    b.wr(R_STEP, 1);
    b.wr(R_CAP, 1000);
    b.wr(R_MAXBUD, 32768);
    b.wr(R_COMMAND, 1);
    check((b.rd(R_ERROR) >> E_BADCFG) & 1, "cap below max rejected");
    b.wr(R_COMMAND, 4);

    // mode1 with min=0 -> rejected; mode6 with min=0 -> accepted
    b.wr(R_CAP, 65536); b.wr(R_MAXBUD, 32768); b.wr(R_MINBUD, 0);
    b.wr(R_MODE, 1);
    b.wr(R_COMMAND, 1);
    check((b.rd(R_ERROR) >> E_BADCFG) & 1, "billing mode min 0 rejected");
    b.wr(R_COMMAND, 4);
    b.wr(R_MODE, 6);
    b.wr(R_COMMAND, 1);
    check(b.rd(R_STATUS) & 1, "mode6 min 0 accepted");
    check((int64_t)b.rd(S_BALANCE) == 8000, "mode6 balance equals programmed init");
    cleanup(b);
}

// supported_i deasserted while armed is a sticky protocol error
static void t_supported_drop(Bench &b) {
    b.reset();
    In in; in.quiet = true; in.supported = true; b.apply(in);
    b.wr(R_MODE, 0);
    b.wr(R_COMMAND, 1);
    check(b.rd(R_STATUS) & 1, "armed");
    in.supported = false;
    Obs o = b.drive(in);
    check(!o.allow && o.abort, "supported drop blocks and aborts");
    b.edge();
    check((b.rd(R_ERROR) >> E_SUPDROP) & 1, "E_SUPDROP sticky");
    in.supported = true;
    o = b.drive(in);
    check(!o.allow && o.abort, "restoring supported does not clear sticky error");
    cleanup(b);
}

static void t_narrow_overflow(Bench &b) {
    b.reset();
    In in; in.quiet = true; b.apply(in);
    b.wr(R_MODE, 0);
    b.wr(R_WINDOW, 1);
    b.wr(R_COMMAND, 1);

    for (int i = 0; i < 300; i++) { b.drive(in); b.edge(); }  // > 256 armed cycles
    check((b.rd(R_ERROR) >> E_CNTOVF) & 1, "CounterWidth overflow sticky");
    check(b.rd(S_ARMEDCYC) == (300 & 0xFF), "statistic wraps at CounterWidth");
    Obs o = b.drive(in);
    check(!o.allow, "fatal overflow blocks allow even in mode 0");
    check(o.abort, "fatal overflow raises abort");

    cleanup(b);
    check(b.rd(R_ERROR) == 0, "CLEAR clears counter overflow");
    check(b.rd(S_ARMEDCYC) == 0, "CLEAR zeroes statistics");
}

static void t_enabled0(Bench &b) {
    b.reset();
    check(b.rd(R_MAGIC) == 0x49444D414D435431ull, "Enabled=0 magic still readable");
    check(b.rd(R_VERSION) == 1, "Enabled=0 version still readable");
    In in; in.quiet = true; in.supported = true;
    b.apply(in);
    b.wr(R_COMMAND, 1);
    check((b.rd(R_ERROR) >> E_UNSUP) & 1, "ARM refused as unsupported");
    check(b.rd(R_STATUS) == 0, "never armed");
    Obs o = b.drive(in);
    check(!o.en && !o.allow && !o.abort, "feature absent: outputs inactive");
}

// ---------------------------------------------------------------------------
// randomized reference-account test
// ---------------------------------------------------------------------------
static void run_reference(const char *tag, uint32_t seed, int cycles,
                          bool narrow, int mode) {
    Bench b;
    Ref   r;
    r.cw = narrow ? 8 : 64;

    In idle; idle.quiet = true;
    b.apply(idle);
    cfg(r, b, R_MODE, (uint32_t)mode);
    cfg(r, b, R_WINDOW, narrow ? 1 : 23);
    cfg(r, b, R_TARGET, 12000);
    cfg(r, b, R_DEADBAND, 4096);
    cfg(r, b, R_INITBUD, 5000);
    cfg(r, b, R_MINBUD, (mode == 6) ? 0 : 1);
    cfg(r, b, R_MAXBUD, 32768);
    cfg(r, b, R_STEP, narrow ? 3 : 1024);
    cfg(r, b, R_CAP, 65536);
    cfg(r, b, R_WHIT, 1);
    cfg(r, b, R_WMISS, 100);
    cfg(r, b, R_WPTW, 100);
    cmd(b, r, 1);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> bite(0, 99);
    In in;

    for (int c = 0; c < cycles; c++) {
        in = In();
        in.quiet     = (bite(rng) < 30);
        in.supported = true;
        in.v_active  = bite(rng) < 80;
        in.a_issue   = bite(rng) < 15;
        in.a_done    = bite(rng) < 10;
        in.v_done    = bite(rng) < 40;
        in.ctl_did   = (bite(rng) < 70) ? DID_A : (uint32_t)(0x11 + (c % 3));
        in.ctl_state = (uint32_t)((bite(rng) < 50) ? 1 : (bite(rng) < 60 ? 2 : 4));
        in.select_fire = bite(rng) < 20;
        in.select_did  = (bite(rng) < 70) ? DID_A : DID_B;
        in.lookup      = bite(rng) < 25;
        in.lookup_hit  = bite(rng) < 50;
        in.lookup_did  = (bite(rng) < 70) ? DID_A : DID_B;
        in.ptw_r_fire  = bite(rng) < 12;
        in.response_error = bite(rng) < 2;

        bool do_wr = false;
        uint32_t widx = 0;
        uint64_t wdata = 0;
        int roll = bite(rng);
        if (roll < 3) {                       // config write (mostly locked)
            do_wr = true; widx = E(R_STEP); wdata = (uint64_t)(1 + (rng() % 1000));
        } else if (roll < 4) {                // bad address (outside bank E)
            do_wr = true; widx = 0x77; wdata = 0;
        } else if (roll == 5) {               // CLEAR
            do_wr = true; widx = E(R_COMMAND); wdata = 4;
        } else if (roll == 6) {               // ABORT
            do_wr = true; widx = E(R_COMMAND); wdata = 2;
        } else if (roll == 7) {               // FREEZE
            do_wr = true; widx = E(R_COMMAND); wdata = 3;
        }

        b.apply(in);
        b.d.cfg_valid_i = do_wr ? 1 : 0;
        b.d.cfg_write_i = do_wr ? 1 : 0;
        b.d.index_i     = (uint16_t)widx;
        b.d.wdata_i     = wdata;
        b.d.clk_i = 0; b.d.eval();

        Obs rtl{ (bool)b.d.policy_enable_o, (bool)b.d.policy_allow_o,
                 (bool)b.d.policy_abort_o };
        RefObs ref = r.cycle(in, do_wr, widx, wdata);

        if (rtl.en != ref.en || rtl.allow != ref.allow || rtl.abort != ref.abort) {
            char msg[256];
            std::snprintf(msg, sizeof(msg),
                "%s cycle %d en/allow/abort rtl=%d/%d/%d ref=%d/%d/%d",
                tag, c, rtl.en, rtl.allow, rtl.abort, ref.en, ref.allow, ref.abort);
            throw std::runtime_error(msg);
        }
        checks++;

        b.d.clk_i = 1; b.d.eval(); b.d.clk_i = 0; b.d.eval();
        b.d.cfg_valid_i = 0; b.d.cfg_write_i = 0; b.d.wdata_i = 0;
        b.apply(in);
    }

    check(b.rd(R_ERROR) == r.err, "reference error register");
    check(b.rd(S_CHARGE) == (r.stat[Ref::ST_CHARGE] & r.mask()), "reference total charge");
    check(b.rd(S_REPLEN) == (r.stat[Ref::ST_REPLEN] & r.mask()), "reference total replenish");
    check(b.rd(S_WIN) == (r.stat[Ref::ST_WIN] & r.mask()), "reference complete windows");
    check(b.rd(S_VDONE) == (r.stat[Ref::ST_VDONE] & r.mask()), "reference V done events");
    check(b.rd(S_FBUP) == (r.stat[Ref::ST_FBUP] & r.mask()), "reference b-up count");
    check(b.rd(S_FBDN) == (r.stat[Ref::ST_FBDN] & r.mask()), "reference b-down count");
    check(b.rd(S_PERMCLOSE) == (r.stat[Ref::ST_PERMCLOSE] & r.mask()),
          "reference permission-closed cycles");
    check((int64_t)b.rd(S_BALANCE) == r.balance, "reference final balance");
    check(b.rd(S_BFINAL) == r.b, "reference final b");
    check(b.rd(S_BMIN) == r.bmin, "reference b min");
    check(b.rd(S_BMAX) == r.bmax, "reference b max");
    check(b.rd(S_NODONE) == r.nodone_max, "reference longest no-done run");
    check(b.rd(R_STATUS) == ((r.armed ? 1ull : 0ull) | (r.frozen ? 2ull : 0ull) |
                             (r.aborted ? 4ull : 0ull)), "reference status word");
    for (int i = 0; i < 32; i++)
        check(b.rd(S_HIST + i) == (r.hist[i] & r.mask()), "reference histogram bucket");
}

// ---------------------------------------------------------------------------
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    std::string mode = (argc > 1) ? std::string(argv[1]) : std::string("full");

    try {
        if (mode == "off") {
            Bench b;
            t_enabled0(b);
        } else if (mode == "small") {
            Bench b;
            t_narrow_overflow(b);
            run_reference("small-ref-m0", 0xC0FFEE01u, 260, true, 0);
            run_reference("small-ref-m3", 0xC0FFEE02u, 260, true, 3);
        } else {
            Bench b;
            Ref   r;
            t_reset_defaults(b);
            t_bad_writes_and_sticky(b);
            t_arm_predicates(b);
            t_config_lock(b);
            t_first_allow_and_mode0(b);
            t_mode1_charge_and_debt(b, r, false);
            t_window_feedback(b);
            t_permission_closed(b);
            t_mode6_gap(b);
            t_mode7_stale(b);
            t_did_ownership(b);
            t_abort_keeps_charging(b);
            t_abort_same_cycle(b);
            t_freeze_clear_rearm(b);
            t_abort_survives_freeze(b);
            t_error_survives_freeze(b);
            t_refill_inactive_window(b);
            t_cfg_bad_and_no_clamp(b);
            t_supported_drop(b);

            for (int m = 0; m <= 7; m++) {
                char nm[32];
                std::snprintf(nm, sizeof(nm), "ref-mode%d", m);
                run_reference(nm, 0x12345678u + (uint32_t)m, 400, false, m);
            }
        }
        std::cout << "PASS " << checks
                  << " dma_research_policy checks (simulation only)" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL after " << checks << " checks: " << e.what() << std::endl;
        return 1;
    }
}
