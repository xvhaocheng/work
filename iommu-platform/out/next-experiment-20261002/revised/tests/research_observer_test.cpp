// Regression test for the passive research observer (E2 lifecycle + natural cost).
// Build/run with Verilator 4.038. Simulation only; no board claim, no formal claim.
// T_CYCLES is overridden to 64 by the runner; the production default is 5000000.
#include "Vdma_research_observer.h"
#include "verilated.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

static int checks = 0;
static void check(bool c, const char *m) {
    if (!c) throw std::runtime_error(m);
    checks++;
}

// Local observer index space (legacy index = local + 512).
enum : unsigned {
    R_MAGIC0 = 0x0000, R_MAGIC1 = 0x0001, R_STATUS = 0x0002, R_ERROR = 0x0003,
    R_MODE = 0x0004, R_CMD = 0x0005, R_PHASE = 0x0006, R_ASTART = 0x0007,
    R_AELAPSED = 0x0008, R_EPOCH = 0x0009, R_EVCOUNT = 0x000A, R_INVAL = 0x000B,
    R_E2OVF = 0x000C, R_E2ERR = 0x000D, R_UPDCNT = 0x000E, R_QUIETTO = 0x000F,
    R_T0LO = 0x0010, R_T0HI = 0x0011, R_T1LO = 0x0012, R_T1HI = 0x0013,
    R_FP0TIME = 0x0014, R_FP0META = 0x0015, R_FP0LO = 0x0016, R_FP0HI = 0x0017,
    R_FP1TIME = 0x0018, R_FP1META = 0x0019, R_FP1LO = 0x001A, R_FP1HI = 0x001B,
    R_TVALID = 0x001C, R_FPDONE = 0x001D, R_DEADLINE = 0x001E, R_PROBESTART = 0x001F,
    R_FREEZETS = 0x0020, R_SNAPVALID = 0x0021,
    R_PLRUW = 0x0022, R_PLRUD = 0x0023, R_PLRUF = 0x0024,
    R_AISSUE = 0x0025, R_ADONE = 0x0026, R_AFIRST = 0x0027, R_ALAST = 0x0028,
    R_SRC = 0x0030, R_DST = 0x0031, R_IOVAV = 0x0032,
    R_SNAP_WARM = 0x0100, R_EVT = 0x0200, R_SAMPLE = 0x0800,
    R_COSTMAGIC = 0x0C00, R_COSTSTAT = 0x0C01, R_COSTERR = 0x0C02, R_CYCLES = 0x0C03,
    R_VMAIN = 0x0C04, R_AMAIN = 0x0C05, R_VCOMP = 0x0C06, R_ACOMP = 0x0C07,
    R_VSEL = 0x0C08, R_ASEL = 0x0C09, R_VHIT = 0x0C0A, R_VMISS = 0x0C0B,
    R_AHIT = 0x0C0C, R_AMISS = 0x0C0D, R_OWNERR = 0x0C0E,
    R_AISSUEC = 0x0C0F, R_ADONEC = 0x0C10, R_VISSUEC = 0x0C11, R_VDONEC = 0x0C12,
    R_ADONEALL = 0x0C13, R_ELIGIBLE = 0x0C14, R_SAMPCOUNT = 0x0C15, R_SAMPOVF = 0x0C16,
    R_CARRYIN = 0x0C17, R_CARRYOUT = 0x0C18, R_WINCYCLES = 0x0C19,
    R_WVMAIN = 0x0C1A, R_WAMAIN = 0x0C1B, R_WVCOMP = 0x0C1C, R_WACOMP = 0x0C1D,
    R_WVSEL = 0x0C1E, R_WASEL = 0x0C1F, R_WVHIT = 0x0C20, R_WVMISS = 0x0C21,
    R_WAHIT = 0x0C22, R_WAMISS = 0x0C23,
    R_CURAMAIN = 0x0C24, R_CURAREQ = 0x0C25, R_CURAMISS = 0x0C26,
    R_CURACOMP = 0x0C27, R_CURADUR = 0x0C28, R_CURAFLAGS = 0x0C29,
    R_CURVMAIN = 0x0C2A, R_CURVCOMP = 0x0C2B, R_LANEACT = 0x0C2C, R_LANEOWN = 0x0C2D
};

static const uint64_t T_TEST = 64;   // must match -GT_CYCLES=64 given by the runner

struct Entry { uint64_t lo, hi; };

static Entry mk_entry(uint64_t vpn, uint16_t gscid, uint32_t pscid, bool valid,
                      bool s1g, bool s1_2m, bool s2g, bool s2_2m, bool global) {
    Entry e;
    e.lo  = (vpn & 0x1FFFFFFFULL);
    e.lo |= ((uint64_t)(pscid & 0xFFFFFULL)) << 29;
    e.lo |= 1ULL << 49;                  // en1 (stage 1 enabled)
    e.lo |= 0ULL << 50;                  // en2 (stage 2 disabled)
    e.lo |= ((uint64_t)valid) << 51;
    e.lo |= ((uint64_t)s1g)   << 52;
    e.lo |= ((uint64_t)s1_2m) << 53;
    e.lo |= ((uint64_t)s2g)   << 54;
    e.lo |= ((uint64_t)s2_2m) << 55;
    e.lo |= ((uint64_t)global)<< 56;
    e.hi  = (uint64_t)gscid;             // raw gscid lives at bits [79:64]
    return e;
}

struct Bench {
    Vdma_research_observer d;

    Bench() { reset(); }

    void tick(unsigned n = 1) {
        while (n--) {
            d.clk_i = 0; d.eval();
            d.clk_i = 1; d.eval();
            d.clk_i = 0; d.eval();
        }
    }

    void reset() {
        d.cfg_valid_i = 0; d.cfg_write_i = 0; d.index_i = 0; d.wdata_i = 0;
        d.quiet_i = 1; d.supported_i = 1;
        d.update_i = 0; d.invalidate_i = 0; d.replace_mask_i = 0; d.update_did_i = 0;
        d.lookup_i = 0; d.lookup_hit_i = 0; d.lookup_did_i = 0;
        d.lookup_iova_i = 0; d.lookup_pscid_i = 0; d.lookup_gscid_i = 0;
        d.lookup_stage1_i = 1; d.lookup_stage2_i = 0;
        d.ctl_state_i = 0; d.ctl_did_i = 0;
        d.select_fire_i = 0; d.select_did_i = 0; d.response_error_i = 0;
        d.issue_i = 0; d.done_i = 0;
        d.issue_id_i[0] = 0; d.issue_id_i[1] = 0;
        d.done_id_i[0] = 0; d.done_id_i[1] = 0;
        d.p4m_arm_i = 0; d.p4m_open_i = 0; d.p4m_close_i = 0;
        d.plru_i = 0;
        for (int i = 0; i < 16; i++)
            for (int w = 0; w < 4; w++) { d.entries_i[i][w] = 0; d.entries_next_i[i][w] = 0; }
        d.rst_ni = 0; tick(2); d.rst_ni = 1; tick(1);
    }

    void write(unsigned idx, uint64_t v) {
        d.cfg_valid_i = 1; d.cfg_write_i = 1; d.index_i = idx; d.wdata_i = v;
        tick(1);
        d.cfg_valid_i = 0; d.cfg_write_i = 0; d.eval();
    }

    uint64_t read(unsigned idx) { d.index_i = idx; d.eval(); return d.rdata_o; }

    void set_entry(int i, const Entry &e) {
        d.entries_i[i][0] = (uint32_t)(e.lo & 0xFFFFFFFFu);
        d.entries_i[i][1] = (uint32_t)(e.lo >> 32);
        d.entries_i[i][2] = (uint32_t)(e.hi & 0xFFFFFFFFu);
        d.entries_i[i][3] = (uint32_t)(e.hi >> 32);
    }
    void set_next_entry(int i, const Entry &e) {
        d.entries_next_i[i][0] = (uint32_t)(e.lo & 0xFFFFFFFFu);
        d.entries_next_i[i][1] = (uint32_t)(e.lo >> 32);
        d.entries_next_i[i][2] = (uint32_t)(e.hi & 0xFFFFFFFFu);
        d.entries_next_i[i][3] = (uint32_t)(e.hi >> 32);
    }
    void mirror() {
        for (int i = 0; i < 16; i++)
            for (int w = 0; w < 4; w++) d.entries_next_i[i][w] = d.entries_i[i][w];
    }
    void clear_entry(int i) { set_entry(i, Entry{0, 0}); }

    void mode(uint64_t v)              { write(R_MODE, v); }
    void cmd(uint64_t v)               { write(R_CMD, v); }
    void arm_warm()                    { cmd(1); }
    void arm_a()                       { cmd(2); }
    void probe_cmd()                   { cmd(3); }
    void freeze_cmd()                  { cmd(4); }
    void abort_cmd()                   { cmd(5); }
    void clear_cmd()                   { cmd(6); }
    void p4m_arm()  { d.p4m_arm_i = 1;  tick(1); d.p4m_arm_i = 0; }
    void p4m_open() { d.p4m_open_i = 1; tick(1); d.p4m_open_i = 0; }
    void p4m_close(){ d.p4m_close_i = 1; tick(1); d.p4m_close_i = 0; }

    void lookup(uint64_t iova, uint32_t pscid, uint16_t gscid, bool hit, uint32_t did,
                bool s1 = true, bool s2 = false) {
        d.lookup_i = 1; d.lookup_iova_i = iova; d.lookup_pscid_i = pscid;
        d.lookup_gscid_i = gscid; d.lookup_hit_i = hit; d.lookup_stage1_i = s1;
        d.lookup_stage2_i = s2; d.lookup_did_i = did;
        tick(1);
        d.lookup_i = 0; d.eval();
    }
    void issue(int lane, uint64_t id) { d.issue_i = (1u << lane); d.issue_id_i[lane] = id; tick(1); d.issue_i = 0; }
    void done(int lane, uint64_t id)  { d.done_i  = (1u << lane); d.done_id_i[lane]  = id; tick(1); d.done_i  = 0; }
    void close_with_done(int lane, uint64_t id) {
        d.done_i = (1u << lane); d.done_id_i[lane] = id; d.p4m_close_i = 1;
        tick(1);
        d.done_i = 0; d.p4m_close_i = 0; d.eval();
    }
    void update_event(unsigned slot, const Entry &old_e, const Entry &new_e, uint32_t did) {
        set_entry((int)slot, old_e);
        set_next_entry((int)slot, new_e);
        d.update_i = 1; d.replace_mask_i = (uint16_t)(1u << slot); d.update_did_i = did;
        tick(1);
        d.update_i = 0; d.replace_mask_i = 0; d.eval();
    }
};

static Bench *g_bench = nullptr;

// ---------------------------------------------------------------- scenarios
static void t_magic()
{
    Bench &b = *g_bench;
    check(b.read(R_MAGIC0) == 0x49444d4152455331ULL, "magic0");
    check(b.read(R_MAGIC1) == 0x49444d414f425308ULL, "magic1");
    check(b.read(R_COSTMAGIC) == 0x49444d4152454331ULL, "cost magic");
}

static void t_config_and_commands_invalid()
{
    Bench &b = *g_bench;
    b.reset();
    // mode value 3 (E2|COST) must be rejected and not change the mode
    b.mode(3);
    check((b.read(R_MODE) & 3) == 0, "mode 3 rejected");
    check(((b.read(R_E2ERR) >> 0) & 1) == 1, "mode 3 error bit0");
    check(((b.read(R_COSTERR) >> 6) & 1) == 1, "mode 3 cost err bit6");
    // reserved index write
    b.reset();
    b.write(0x0080, 0xFFFF);
    check(((b.read(R_E2ERR) >> 0) & 1) == 1, "reserved write error");
    // command 0 and command 7 are reserved
    b.reset();
    b.mode(1);
    b.cmd(0);
    check(((b.read(R_E2ERR) >> 0) & 1) == 1, "cmd 0 rejected");
    b.cmd(7);
    check(((b.read(R_E2ERR) >> 0) & 1) == 1, "cmd 7 rejected");
    // ARM_A in IDLE is invalid
    b.reset();
    b.mode(1);
    b.arm_a();
    check((b.read(R_PHASE) & 7) == 0, "ARM_A in IDLE rejected");
    check(((b.read(R_E2ERR) >> 0) & 1) == 1, "ARM_A in IDLE error");
    // FREEZE is only legal in PROBE
    b.reset();
    b.mode(1);
    b.freeze_cmd();
    check(((b.read(R_E2ERR) >> 0) & 1) == 1, "FREEZE in IDLE error");
    // misaligned / identical IOVA configuration
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x0000000010000001ULL);
    check(((b.read(R_E2ERR) >> 11) & 1) == 1, "misaligned IOVA rejected");
    b.write(R_SRC, 0x0000000010000000ULL);
    b.write(R_DST, 0x0000000010000000ULL);
    check(((b.read(R_E2ERR) >> 11) & 1) == 1, "identical IOVA rejected");
    check((b.read(R_IOVAV) & 3) != 3, "IOVA config not complete");
}

static void t_e2_capture()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x12345000ULL);
    b.write(R_DST, 0x23456000ULL);
    check((b.read(R_IOVAV) & 3) == 3, "IOVA config valid");
    uint64_t vpn_s = 0x12345000ULL >> 12;
    uint64_t vpn_d = 0x23456000ULL >> 12;
    Entry src = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
    Entry dst = mk_entry(vpn_d, 0x2345, 0x2, true, false, false, false, false, false);
    b.set_entry(0, src); b.set_entry(1, dst); b.mirror();

    b.arm_warm();
    check((b.read(R_PHASE) & 7) == 1, "ARM_WARM accepted");

    // repeated source lookups must never create the destination target
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 10);
    check((b.read(R_TVALID) & 1) == 1, "target0 captured");
    check((b.read(R_TVALID) & 2) == 0, "repeated source did not fill target1");

    b.lookup(0x23456000ULL, 0x2, 0x2345, true, 10);
    check((b.read(R_TVALID) & 3) == 3, "both targets captured");

    // nonzero gscid must appear in the raw128 key at bits [79:64] == hi[15:0]
    check(b.read(R_T0HI) == 0x0000000000001234ULL, "target0 gscid raw128 position");
    check(b.read(R_T1HI) == 0x0000000000002345ULL, "target1 gscid raw128 position");
    check(b.read(R_T0LO) == src.lo, "target0 key equals actual entry");
    check(b.read(R_T1LO) == dst.lo, "target1 key equals actual entry");

    // superpage / global entries are not ordinary targets
    Entry bad = dst; bad.lo |= (1ULL << 56);          // S1 global
    b.set_entry(1, bad); b.mirror();
    b.arm_a();
    check((b.read(R_PHASE) & 7) == 1, "ARM_A refused with global target");
    check(((b.read(R_E2ERR) >> 6) & 1) == 1, "target geometry error");
    b.set_entry(1, dst); b.mirror();

    // unknown owner / fault guards
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 7);
    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "unknown owner rejected");
    b.d.response_error_i = 1;
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 10);
    b.d.response_error_i = 0;
    check(((b.read(R_E2ERR) >> 13) & 1) == 1, "response fault flagged");

    // unsupported configuration
    b.reset();
    b.mode(1);
    b.d.supported_i = 0;
    b.cmd(1);
    check((b.read(R_PHASE) & 7) == 0, "ARM_WARM refused when unsupported");
    check(((b.read(R_E2ERR) >> 9) & 1) == 1, "unsupported flagged");
    b.d.supported_i = 1;
}

static void t_e2_events_and_deadline()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x12345000ULL);
    b.write(R_DST, 0x23456000ULL);
    uint64_t vpn_s = 0x12345000ULL >> 12;
    uint64_t vpn_d = 0x23456000ULL >> 12;
    Entry src = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
    Entry dst = mk_entry(vpn_d, 0x1234, 0x1, true, false, false, false, false, false);
    b.set_entry(0, src); b.set_entry(1, dst); b.mirror();
    b.arm_warm();
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x23456000ULL, 0x1, 0x1234, true, 10);
    b.arm_a();
    check((b.read(R_PHASE) & 7) == 2, "ARM_A accepted");
    check(((b.read(R_SNAPVALID) >> 0) & 1) == 1, "warm snapshot valid");

    // committed identical update (old == new) is still an event for the slot
    b.update_event(2, src, src, 11);
    check(b.read(R_EVCOUNT) == 1, "identical update logged");
    check(((b.read(R_EVT + 1) >> 49) & 1) == 1, "meta old==new flag");
    check(((b.read(R_EVT + 1) >> 51) & 1) == 0, "meta slot mismatch clear");

    // target eviction is recorded in the event meta
    uint64_t ev_before = b.read(R_EVCOUNT);
    Entry other = mk_entry(0x777, 0x9, 0x9, true, false, false, false, false, false);
    b.update_event(0, src, other, 11);
    check(b.read(R_EVCOUNT) == ev_before + 1, "eviction logged");
    check(((b.read(R_EVT + ev_before * 6 + 1) >> 52) & 1) == 1, "evicted target0 flag");
    b.lookup(0x12345000ULL, 0x1, 0x1234, false, 11);

    // deadline is exactly T after ASTART; EPOCH is sampled pre-edge, so
    // after the transition EPOCH == DEADLINE + 1 while A_ELAPSED == T.
    uint64_t elapsed = b.read(R_AELAPSED);
    check(elapsed < T_TEST, "not at deadline yet");
    b.tick((unsigned)(T_TEST - elapsed - 1));
    check((b.read(R_PHASE) & 7) == 2, "still ARM_A before T");
    b.tick(1);
    check((b.read(R_PHASE) & 7) == 6, "READY at exactly T");
    check(b.read(R_AELAPSED) == T_TEST, "A elapsed == T");
    check(b.read(R_DEADLINE) - b.read(R_ASTART) == T_TEST, "deadline - astart == T");
    check(b.read(R_EPOCH) == b.read(R_DEADLINE) + 1, "EPOCH is pre-edge deadline + 1 after transition");
    check(((b.read(R_SNAPVALID) >> 1) & 1) == 1, "deadline snapshot valid");
    check(b.read(R_ERROR) == 0, "no errors on positive E2 path");

    // any translation activity in READY is invalid
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 10);
    check(((b.read(R_E2ERR) >> 5) & 1) == 1, "READY activity flagged");
}

static void t_e2_update_faults()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x12345000ULL);
    b.write(R_DST, 0x23456000ULL);
    uint64_t vpn_s = 0x12345000ULL >> 12;
    uint64_t vpn_d = 0x23456000ULL >> 12;
    Entry src = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
    Entry dst = mk_entry(vpn_d, 0x1234, 0x1, true, false, false, false, false, false);
    b.set_entry(0, src); b.set_entry(1, dst); b.mirror();
    b.arm_warm();
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x23456000ULL, 0x1, 0x1234, true, 10);
    b.arm_a();
    check((b.read(R_PHASE) & 7) == 2, "ARM_A accepted");
    check(((b.read(R_SNAPVALID) >> 0) & 1) == 1, "warm snapshot valid");

    // change outside the committed slot
    { Entry a; b.set_entry(3, src); Entry c = dst; b.set_next_entry(3, c);
      b.d.update_i = 1; b.d.replace_mask_i = (1 << 2); b.d.update_did_i = 11;
      b.tick(1); b.d.update_i = 0; b.d.replace_mask_i = 0; b.d.eval(); }
    check(((b.read(R_E2ERR) >> 8) & 1) == 1, "change outside slot flagged");

    b.mirror();
    // more than one changed slot
    { b.set_entry(4, src); b.set_next_entry(4, dst);
      b.set_entry(5, src); b.set_next_entry(5, dst);
      b.d.update_i = 1; b.d.replace_mask_i = (1 << 4); b.d.update_did_i = 11;
      b.tick(1); b.d.update_i = 0; b.d.replace_mask_i = 0; b.d.eval(); }
    check(((b.read(R_E2ERR) >> 1) & 1) == 1, "multi change flagged");

}

static void t_e2_probe_and_freeze()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x12345000ULL);
    b.write(R_DST, 0x23456000ULL);
    uint64_t vpn_s = 0x12345000ULL >> 12;
    uint64_t vpn_d = 0x23456000ULL >> 12;
    Entry src = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
    Entry dst = mk_entry(vpn_d, 0x1234, 0x1, true, false, false, false, false, false);
    b.set_entry(0, src); b.set_entry(1, dst); b.mirror();
    b.arm_warm();
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x23456000ULL, 0x1, 0x1234, true, 10);
    b.arm_a();
    b.tick((unsigned)T_TEST);
    check((b.read(R_PHASE) & 7) == 6, "READY before PROBE");

    b.probe_cmd();
    check((b.read(R_PHASE) & 7) == 3, "PROBE accepted");
    check(b.read(R_PROBESTART) != 0, "probe start recorded");
    check(((b.read(R_FPDONE)) & 3) == 0, "no first lookup yet");

    // first probe of target0: both targets still live in the snapshot
    b.lookup(0x12345000ULL, 0x1, 0x1234, true, 10);
    check((b.read(R_FPDONE) & 1) == 1, "fp0 done");
    check(((b.read(R_FP0META) >> 32) & 1) == 1, "fp0 hit");
    check((b.read(R_FP0META) & 0xFFFF) == 0x1, "fp0 self live bitmap");
    check(((b.read(R_FP0META) >> 16) & 0xFFFF) == 0x2, "fp0 other live bitmap (target1 still live)");

    // the probe of target0 causes an eviction of target1 before its first lookup
    Entry other = mk_entry(0x777, 0x9, 0x9, true, false, false, false, false, false);
    b.set_entry(1, other); b.mirror();
    b.lookup(0x23456000ULL, 0x1, 0x1234, false, 10);
    check((b.read(R_FPDONE) & 2) == 2, "fp1 done");
    check(((b.read(R_FP1META) >> 32) & 1) == 0, "fp1 miss observed");
    check((b.read(R_FP1META) & 0xFFFF) == 0, "fp1 self live bitmap empty (probe-induced eviction)");
    check(b.read(R_FP1TIME) > b.read(R_FP0TIME), "fp timestamps monotonic");

    b.freeze_cmd();
    check((b.read(R_PHASE) & 7) == 4, "FREEZE accepted");
    check(((b.read(R_SNAPVALID) >> 2) & 1) == 1, "freeze snapshot valid");
    check(b.read(R_ERROR) == 0, "no errors on positive E2 probe path");

    // CLEAR is legal in FREEZE and does not destroy frozen evidence
    uint64_t frozen_epoch = b.read(R_EPOCH);
    b.clear_cmd();
    check((b.read(R_PHASE) & 7) == 0, "CLEAR returns to IDLE");
    check(b.read(R_EPOCH) == frozen_epoch, "CLEAR keeps epoch evidence");
    check(((b.read(R_SNAPVALID) >> 2) & 1) == 1, "CLEAR keeps snapshots readable");
    // and not while active
    b.reset();
    b.mode(1);
    uint64_t lo = 0, hi = 0;
    (void)lo; (void)hi;
    b.write(R_SRC, 0x12345000ULL);
    b.write(R_DST, 0x23456000ULL);
    b.set_entry(0, src); b.set_entry(1, dst); b.mirror();
    b.arm_warm();
    b.clear_cmd();
    check((b.read(R_PHASE) & 7) == 1, "CLEAR refused while active");
    check(((b.read(R_E2ERR) >> 0) & 1) == 1, "CLEAR while active flagged");
    b.abort_cmd();
    check((b.read(R_PHASE) & 7) == 5, "ABORT accepted");
    b.clear_cmd();
    check((b.read(R_PHASE) & 7) == 0, "CLEAR after ABORT accepted");
}

static void t_e2_context_guards()
{
    Bench &b = *g_bench;
    // Different context between the two V keys is rejected at ARM_A.
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x10000000ULL);
    b.write(R_DST, 0x20000000ULL);
    uint64_t vpn_s = 0x10000000ULL >> 12;
    uint64_t vpn_d = 0x20000000ULL >> 12;
    Entry srcA = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
    Entry dstB = mk_entry(vpn_d, 0x2345, 0x2, true, false, false, false, false, false);
    b.set_entry(0, srcA); b.set_entry(1, dstB); b.mirror();
    b.arm_warm();
    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x20000000ULL, 0x2, 0x2345, true, 10);
    check((b.read(R_TVALID) & 3) == 3, "both targets captured with different ctx");
    b.arm_a();
    check((b.read(R_PHASE) & 7) == 1, "ARM_A refused mixed ctx");
    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "mixed ctx bit12");

    // Context switch after warm on the same page.
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x10000000ULL);
    b.write(R_DST, 0x20000000ULL);
    b.set_entry(0, srcA); b.set_entry(1, dstB); b.mirror();
    b.arm_warm();
    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x10000000ULL, 0x2, 0x1234, true, 10);
    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "warm context switch bit12");

    // Cold first lookup then changed PSCID before fill/hit.
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x10000000ULL);
    b.write(R_DST, 0x20000000ULL);
    b.set_entry(1, dstB); b.mirror();
    b.arm_warm();
    b.lookup(0x10000000ULL, 0x1, 0x1234, false, 10);
    check((b.read(R_TVALID) & 1) == 0, "cold source not captured");
    b.lookup(0x10000000ULL, 0x2, 0x1234, true, 10);
    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "cold context switch bit12");

    // Nonzero GSCID mismatch.
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x10000000ULL);
    b.write(R_DST, 0x20000000ULL);
    b.set_entry(0, srcA); b.set_entry(1, dstB); b.mirror();
    b.arm_warm();
    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x10000000ULL, 0x1, 0x5678, true, 10);
    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "gscid context switch bit12");

    // PROBE context validation and hit/presence validation.
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x10000000ULL);
    b.write(R_DST, 0x20000000ULL);
    Entry srcC = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
    Entry dstC = mk_entry(vpn_d, 0x1234, 0x1, true, false, false, false, false, false);
    b.set_entry(0, srcC); b.set_entry(1, dstC); b.mirror();
    b.arm_warm();
    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x20000000ULL, 0x1, 0x1234, true, 10);
    b.arm_a();
    b.tick((unsigned)T_TEST);
    b.probe_cmd();
    b.lookup(0x10000000ULL, 0x2, 0x1234, false, 10);
    check(((b.read(R_E2ERR) >> 12) & 1) == 1, "probe context mismatch bit12");
    b.clear_entry(0); b.mirror();
    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
    check(((b.read(R_E2ERR) >> 5) & 1) == 1, "probe hit/presence mismatch");
}

static void t_cost_owner_exclusive()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(2);
    b.d.ctl_state_i = 0;             // Idle until an actual owner is selected.
    b.p4m_arm();
    b.p4m_open();
    // both lanes active at the same time
    b.issue(0, 0x10);
    b.issue(1, 0x20);
    check((b.read(R_LANEACT) & 3) == 3, "both lanes active");
    check(b.read(R_LANEOWN) == ((uint64_t)0x0BULL << 24 | 0x0AULL), "lane owners hardwired");

    b.d.ctl_state_i = 1;
    b.d.ctl_did_i = 10;
    b.tick(3);
    check(b.read(R_VMAIN) == 3, "V owner billed only to V");
    check(b.read(R_AMAIN) == 0, "A not billed while V owns");
    b.d.ctl_did_i = 11;
    b.tick(3);
    check(b.read(R_VMAIN) == 3, "V frozen while A owns");
    check(b.read(R_AMAIN) == 3, "A owner billed only to A");
    // unknown owner
    b.d.ctl_did_i = 7;
    b.tick(1);
    check(((b.read(R_COSTERR) >> 1) & 1) == 1, "unknown owner flagged");
    check(b.read(R_OWNERR) == 1, "unknown owner counted");

    // COMPLETE is separate and follows the same owner
    b.d.ctl_did_i = 10;
    b.d.ctl_state_i = 4;
    b.tick(2);
    check(b.read(R_VCOMP) == 2, "V complete cycles");
    check(b.read(R_ACOMP) == 0, "A complete not billed");
    b.d.ctl_state_i = 0;

    // per-copy first miss is attributed by lookup_did
    b.d.lookup_i = 1; b.d.lookup_iova_i = 0x1000; b.d.lookup_hit_i = 0;
    b.d.lookup_did_i = 10; b.d.lookup_stage1_i = 1; b.d.lookup_stage2_i = 0;
    b.d.select_fire_i = 1; b.d.select_did_i = 10;
    b.tick(1);
    b.d.lookup_i = 0; b.d.select_fire_i = 0; b.d.eval();
    check(b.read(R_VHIT) == 0 && b.read(R_VMISS) == 1, "V first miss");
    check(b.read(R_AHIT) == 0 && b.read(R_AMISS) == 0, "A first miss untouched");
    check(b.read(R_VSEL) == 1 && b.read(R_ASEL) == 0, "V select by select_did");
    check(b.read(R_CURAREQ) == 0, "per-copy requests are not REQ_TRANS cycles");
    b.p4m_close();
    // drain activity after close must not change frozen totals
    uint64_t a_main = b.read(R_AMAIN);
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.tick(5);
    check(b.read(R_AMAIN) == a_main, "totals frozen after close");
    b.d.ctl_state_i = 0;
    check(((b.read(R_COSTSTAT) >> 0) & 1) == 0, "cost session ended at close");
}

static void t_cost_window_edges()
{
    Bench &b = *g_bench;
    // carry-in / carry-out across the window
    b.reset();
    b.mode(2);
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.p4m_arm();
    b.issue(1, 0x200);
    check(((b.read(R_CURAFLAGS) >> 2) & 1) == 0, "pre-open issue not contained");
    b.p4m_open();
    check(b.read(R_CARRYIN) == 1, "carry-in from pre-open issue");
    b.p4m_close();
    check(b.read(R_CARRYOUT) == 1, "carry-out from still active copy");
    check(b.read(R_ELIGIBLE) == 0, "carry-in copy is not eligible");

    // opening edge excluded, closing edge included, done on closing edge eligible
    b.reset();
    b.mode(2);
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.p4m_arm();
    b.p4m_open();
    check(b.read(R_WAMAIN) == 0, "opening edge excluded");
    check(b.read(R_WINCYCLES) == 0, "opening edge not in window cycles");
    b.issue(1, 0x300);
    b.tick(3);
    check(b.read(R_WAMAIN) == 4, "window main cycles after open");
    check(b.read(R_WINCYCLES) == 4, "window cycles counted");
    b.close_with_done(1, 0x300);
    check(b.read(R_WAMAIN) == 5, "closing edge included");
    check(b.read(R_WINCYCLES) == 5, "closing edge in window cycles");
    check(b.read(R_CARRYOUT) == 0, "carry-out uses lane state after closing done");
    check(b.read(R_ELIGIBLE) == 1, "done on closing edge is eligible");
    check(b.read(R_SAMPCOUNT) == 0, "only the 16th eligible copy is sampled");
    b.d.ctl_state_i = 0;
}

static void t_cost_window_open_issue()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(2);
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.p4m_arm();
    // Issue lane 1 on the same edge as p4m_open: contained, not carry-in.
    b.d.p4m_open_i = 1; b.d.issue_i = (1u << 1); b.d.issue_id_i[1] = 0x500;
    b.tick(1);
    b.d.p4m_open_i = 0; b.d.issue_i = 0; b.d.eval();
    check((b.read(R_LANEACT) & 2) == 2, "lane active at open edge");
    check(((b.read(R_CURAFLAGS) >> 2) & 1) == 1, "opening-edge issue is contained");
    check(((b.read(R_CURAFLAGS) >> 0) & 1) == 0, "opening-edge issue not before open");
    check(b.read(R_CARRYIN) == 0, "issue at open is not carry-in");
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.tick(1);
    b.d.ctl_state_i = 0;
    b.done(1, 0x500);
    for (int i = 1; i < 16; i++) {
        b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
        b.issue(1, 0x500 + (uint64_t)i);
        b.tick(1);
        b.d.ctl_state_i = 0;
        if (i == 15) b.close_with_done(1, 0x500 + (uint64_t)i);
        else         b.done(1, 0x500 + (uint64_t)i);
    }
    check(b.read(R_ELIGIBLE) == 16, "16 contained copies starting at open");
    check(b.read(R_SAMPCOUNT) == 1, "sample at 16th contained copy");
    check(b.read(R_SAMPLE + 0) == 0x50F, "sample is 16th done id");
    check(((b.read(R_SAMPLE + 3) >> 2) & 1) == 1, "sample contained flag");
    check(b.read(R_CARRYOUT) == 0, "closing done not carried out");
    check(((b.read(R_COSTSTAT) >> 2) & 1) == 1, "window closed on closing done");
    b.d.ctl_state_i = 0;
}

static void t_cost_sampling()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(2);
    b.p4m_arm();
    // copy completed inside the window but issued before it: not eligible
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.issue(1, 0x400);
    b.p4m_open();
    b.d.ctl_state_i = 0;
    b.done(1, 0x400);
    check(b.read(R_ELIGIBLE) == 0, "carry-in completion not eligible");
    check(b.read(R_ADONEALL) == 1, "carry-in completion still counted in A seq");
    // 16 fully contained completed copies
    for (int i = 1; i <= 16; i++) {
        b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
        b.issue(1, 0x400 + (uint64_t)i);
        b.tick(1);
        b.d.ctl_state_i = 0;
        b.done(1, 0x400 + (uint64_t)i);
    }
    b.p4m_close();
    check(b.read(R_ELIGIBLE) == 16, "16 eligible copies");
    check(b.read(R_ADONEALL) == 17, "all A completions counted");
    check(b.read(R_SAMPCOUNT) == 1, "one sample at the 16th eligible copy");
    check(b.read(R_SAMPLE + 0) == 0x410, "sample carries the 16th eligible done id");
    check(((b.read(R_SAMPLE + 3) >> 2) & 1) == 1, "sample contained flag");
    check(((b.read(R_SAMPLE + 3) >> 32) & 0xFFFFFFFF) != 0, "sample duration nonzero");
}

static void t_overflow_small()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(2);
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 10;
    b.p4m_arm();          // counters reset and started; this cycle is not counted
    b.p4m_open();         // this cycle is counted (owner V, valid)
    b.tick(300);          // 301 increments total: wraps at 2^CounterWidth
    check(((b.read(R_COSTERR) >> 2) & 1) == 1, "counter overflow sticky");
    check(b.read(R_VMAIN) == (301ULL & 0xFFULL), "8-bit counter wrapped");
    b.d.ctl_state_i = 0;
    b.p4m_close();
}

static void t_clear_before_mode()
{
    Bench &b = *g_bench;
    b.reset();
    check((b.read(R_MODE) & 3) == 0, "power-on mode none");
    check((b.read(R_PHASE) & 7) == 0, "power-on phase idle");
    b.clear_cmd();
    check((b.read(R_PHASE) & 7) == 0, "CLEAR before MODE keeps IDLE");
    check(b.read(R_ERROR) == 0, "CLEAR before MODE no error");
    b.mode(1);
    check((b.read(R_MODE) & 3) == 1, "MODE_E2 accepted after CLEAR");
    b.clear_cmd();
    b.mode(2);
    check((b.read(R_MODE) & 3) == 2, "MODE_COST accepted after CLEAR");
    b.clear_cmd();
}

static void t_cost_clear_mode_switch()
{
    Bench &b = *g_bench;
    b.reset();
    // COST session
    b.mode(2);
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.p4m_arm();
    b.p4m_open();
    b.tick(3);
    b.p4m_close();
    check((b.read(R_COSTSTAT) & 1) == 0, "COST session ended");
    b.clear_cmd();
    check(b.read(R_ERROR) == 0, "CLEAR after COST no error");
    // Switch to E2
    b.mode(1);
    check((b.read(R_MODE) & 3) == 1, "MODE_E2 after CLEAR");
    b.write(R_SRC, 0x10000000ULL);
    b.write(R_DST, 0x20000000ULL);
    uint64_t vpn_s = 0x10000000ULL >> 12;
    uint64_t vpn_d = 0x20000000ULL >> 12;
    Entry src = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
    Entry dst = mk_entry(vpn_d, 0x1234, 0x1, true, false, false, false, false, false);
    b.set_entry(0, src); b.set_entry(1, dst); b.mirror();
    b.arm_warm();
    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x20000000ULL, 0x1, 0x1234, true, 10);
    b.arm_a();
    b.tick((unsigned)T_TEST);
    b.probe_cmd();
    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x20000000ULL, 0x1, 0x1234, true, 10);
    b.freeze_cmd();
    b.clear_cmd();
    check(b.read(R_ERROR) == 0, "CLEAR after E2 no error");
    b.mode(2);
    check((b.read(R_MODE) & 3) == 2, "MODE_COST after E2 CLEAR");
}

static void t_cost_fault_abort()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(2);
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.p4m_arm();
    b.p4m_open();
    b.tick(2);
    // response error
    b.d.response_error_i = 1;
    b.tick(1);
    b.d.response_error_i = 0;
    check(((b.read(R_COSTERR) >> 7) & 1) == 1, "COST response error flagged");
    check((b.read(R_COSTSTAT) & 1) == 1, "COST still active after fault");
    check(((b.read(R_COSTSTAT) >> 2) & 1) == 0, "window not closed after fault");
    // unsupported
    b.d.supported_i = 0;
    b.tick(1);
    b.d.supported_i = 1;
    check(((b.read(R_COSTERR) >> 9) & 1) == 1, "COST unsupported flagged");
    check((b.read(R_COSTSTAT) & 1) == 1, "COST still active after unsupported");
    // ABORT
    b.abort_cmd();
    check((b.read(R_COSTSTAT) & 1) == 0, "COST aborted");
    check(((b.read(R_COSTERR) >> 8) & 1) == 1, "COST abort flag");
    check(((b.read(R_COSTSTAT) >> 2) & 1) == 0, "window not closed after ABORT");
    // cleanup benign
    b.clear_cmd();
    check(b.read(R_ERROR) == 0, "CLEAR after ABORT clears errors");
    check((b.read(R_COSTSTAT) & 1) == 0, "COST inactive after CLEAR");
}

static void t_arm_while_active()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(2);
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.p4m_arm();
    b.p4m_open();
    b.tick(3);
    b.d.ctl_state_i = 0;
    uint64_t main_before = b.read(R_AMAIN);
    uint64_t cycles_before = b.read(R_CYCLES);
    b.p4m_arm();
    check(((b.read(R_COSTERR) >> 6) & 1) == 1, "ARM while active flagged");
    check(b.read(R_AMAIN) >= main_before, "ARM while active preserves main count");
    check(b.read(R_CYCLES) >= cycles_before, "ARM while active preserves cycles");
    check((b.read(R_COSTSTAT) & 1) == 1, "still active");
    b.p4m_close();
}

static void t_mode_none_p4m_arm()
{
    Bench &b = *g_bench;
    b.reset();
    check((b.read(R_MODE) & 3) == 0, "mode none");
    b.p4m_arm();
    check(b.read(R_ERROR) == 0, "p4m_arm in MODE_NONE ignored (no error)");
    check((b.read(R_COSTSTAT) & 1) == 0, "no COST session in MODE_NONE");
}

static void t_event_overflow()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(1);
    b.write(R_SRC, 0x10000000ULL);
    b.write(R_DST, 0x20000000ULL);
    uint64_t vpn_s = 0x10000000ULL >> 12;
    uint64_t vpn_d = 0x20000000ULL >> 12;
    Entry src = mk_entry(vpn_s, 0x1234, 0x1, true, false, false, false, false, false);
    Entry dst = mk_entry(vpn_d, 0x1234, 0x1, true, false, false, false, false, false);
    b.set_entry(0, src); b.set_entry(1, dst); b.mirror();
    b.arm_warm();
    b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
    b.lookup(0x20000000ULL, 0x1, 0x1234, true, 10);
    // 257 identical updates on slot 0 during ARM_WARM
    for (int i = 0; i < 257; i++) {
        b.update_event(0, src, src, 10);
    }
    check(b.read(R_EVCOUNT) == 256, "event count saturates at EventDepth");
    check(((b.read(R_E2OVF) >> 0) & 1) == 1, "event overflow sticky");
    check(((b.read(R_E2ERR) >> 3) & 1) == 1, "event overflow error bit");
}

static void t_sample_overflow()
{
    Bench &b = *g_bench;
    b.reset();
    b.mode(2);
    b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
    b.p4m_arm();
    b.p4m_open();
    // 257 samples * 16 = 4112 eligible completed copies
    for (int i = 0; i < 4112; i++) {
        b.d.ctl_state_i = 1; b.d.ctl_did_i = 11;
        b.issue(1, (uint64_t)i);
        b.tick(1);
        b.d.ctl_state_i = 0;
        b.done(1, (uint64_t)i);
    }
    b.p4m_close();
    check(b.read(R_SAMPCOUNT) == 256, "sample count saturates at SampleDepth");
    check(((b.read(R_SAMPOVF) >> 0) & 1) == 1, "sample overflow sticky");
    check(((b.read(R_COSTERR) >> 3) & 1) == 1, "sample overflow error bit");
}

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);
    bool small = (argc > 1 && std::string(argv[1]) == "small");
    Bench b;
    g_bench = &b;
    try {
        if (small) {
            t_overflow_small();
        } else {
            t_magic();
            t_config_and_commands_invalid();
            t_e2_capture();
            t_e2_context_guards();
            t_e2_events_and_deadline();
            t_e2_update_faults();
            t_e2_probe_and_freeze();
            t_cost_owner_exclusive();
            t_cost_window_edges();
            t_cost_window_open_issue();
            t_cost_sampling();
            t_clear_before_mode();
            t_cost_clear_mode_switch();
            t_cost_fault_abort();
            t_arm_while_active();
            t_mode_none_p4m_arm();
            t_event_overflow();
            t_sample_overflow();
        }
        std::cout << "PASS " << checks << " research observer checks (simulation only)" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL after " << checks << " checks: " << e.what() << std::endl;
        return 1;
    }
}
