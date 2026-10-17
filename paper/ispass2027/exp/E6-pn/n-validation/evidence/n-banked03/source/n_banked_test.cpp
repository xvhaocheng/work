// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Stimulus and checker for the banked transaction-controller wrapper.
//
//   * n_banked_top is a purely-structural scalar-I/O wrapper (no clock,
//     no event controls, no initial stimuli).
//   * Two behavioural models live only here:
//       - a translation engine  (captures trans_req, later drives trans_resp)
//       - a completion slave + scoreboard (accepts comp AW/AR/W, drives B/R)
//   * Every handshake / observed transfer is recorded *pre-edge* inside
//     service().  The tests assert on those recorded logs, never on DUT outputs
//     sampled after the rising edge (where a one-cycle pulse could be gone).
//   * The completion scoreboard is keyed on a per-IOVA translation dictionary,
//     so a translation that is still pending can never change an expectation.

#include "Vn_banked_top.h"
#include "verilated.h"

#include <cstdio>
#include <cstdint>
#include <deque>
#include <map>
#include <vector>

// ============================================================================
// Time
// ============================================================================
static long g_cycle = 0;

// Verilator 4.038 links an external reference to sc_time_stamp(); provide it.
// The declaration (and hence its language linkage) comes from verilated.h.
double sc_time_stamp() { return static_cast<double>(g_cycle); }

// ============================================================================
// Check helpers
// ============================================================================
static int g_fail = 0; static unsigned g_checks=0; static FILE* g_trace=nullptr;

static void expect(bool cond, const char *msg) { g_checks++;
    if (!cond) { std::printf("[cycle %ld] FAIL: %s\n", g_cycle, msg); ++g_fail; }
}
static void expect_eq(uint64_t got, uint64_t exp, const char *msg) { g_checks++;
    if (got != exp) {
        std::printf("[cycle %ld] FAIL: %s (got 0x%llx expected 0x%llx)\n",
                    g_cycle, msg,
                    (unsigned long long)got, (unsigned long long)exp);
        ++g_fail;
    }
}

// ============================================================================
// Recorded transfer logs (all filled pre-edge in service())
// ============================================================================
struct WBeat { uint64_t data; uint64_t strb; bool last; };

struct UpAwRec  { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
struct UpArRec  { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
struct UpWRec   { long cyc; uint32_t id; uint64_t data; uint64_t strb; bool last; };
struct UpBRec   { long cyc; uint32_t id; uint32_t resp; };
struct UpRRec   { long cyc; uint32_t id; uint64_t data; bool last; uint32_t resp; };
struct CmpAwRec { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
struct CmpArRec { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
struct CmpWRec  { long cyc; uint64_t data; uint64_t strb; bool last; };
struct CmpAwSmp { long cyc; bool valid; uint32_t id; uint64_t addr; uint32_t len; };

static std::vector<UpAwRec>     L_up_aw;
static std::vector<UpArRec>     L_up_ar;
static std::vector<UpWRec>      L_up_w;
static std::vector<UpBRec>      L_up_b;
static std::vector<UpRRec>      L_up_r;
static std::vector<CmpAwRec>    L_cmp_aw;
static std::vector<CmpArRec>    L_cmp_ar;
static std::vector<CmpWRec>     L_cmp_w;
static std::vector<CmpAwSmp>    L_cmp_aw_smp;

// First-error localiser: a completion AW handshake that appears before ANY
// upstream AW has been accepted cannot be the forwarded header of a real
// write.  Reported once per test; it never replaces or relaxes a check.
static bool g_cmp_aw_pre_aw_reported = false;

static long g_up_aw_stall = 0;   // cycles with up.aw_valid && !tr_aw_ready

// ============================================================================
// Per-IOVA translation dictionary + expected transactions
// ============================================================================
struct XlEntry { uint64_t paddr; bool error; int delay; uint32_t did; };
static std::map<uint64_t, XlEntry> g_xlate;

struct ExpWr {
    uint32_t id;
    uint64_t paddr;
    std::vector<WBeat> beats;
    int aw_hits = 0;   // times the completion AW was seen
    int b_hits  = 0;   // times a B completed the write
};
static std::vector<ExpWr> g_exp_wr;

struct ExpRd {
    uint32_t id;
    uint64_t paddr;
    uint32_t len;
    int ar_hits = 0;
};
static std::vector<ExpRd> g_exp_rd;

// ============================================================================
// Upstream stimulus (held until accepted)
// ============================================================================
struct Up {
    bool     aw_valid = false;
    uint32_t aw_id = 0; uint64_t aw_addr = 0;
    uint32_t aw_len = 0, aw_size = 3, aw_burst = 1, aw_prot = 0, aw_user = 0;
    uint32_t aw_did = 0; bool aw_ssid = false; uint32_t aw_subid = 0;
    bool     aw_atst = false, aw_mvalid = false;

    bool     ar_valid = false;
    uint32_t ar_id = 0; uint64_t ar_addr = 0;
    uint32_t ar_len = 0, ar_size = 3, ar_burst = 1, ar_prot = 0, ar_user = 0;
    uint32_t ar_did = 0; bool ar_ssid = false; uint32_t ar_subid = 0;
    bool     ar_atst = false, ar_mvalid = false;

    std::deque<WBeat> wq;                       // W beats waiting to be accepted
    std::deque<std::pair<uint32_t,int> > w_attrib;  // accepted AW id -> beats left

    bool     b_ready = true;
    bool     r_ready = true;
};
static Up up;

// v5 full-buffer admission proxy.  Existing tests leave this at 0 so the
// legacy read guard is unchanged.
static uint32_t g_fullbuffer_jobs = 0;

// ============================================================================
// Behavioural models
// ============================================================================
struct BBeat { uint32_t id; uint32_t resp; };
struct RBeat { uint32_t id; uint64_t data; bool last; uint32_t resp = 0; };

struct CawEnt {
    uint32_t id = 0;
    uint64_t addr = 0;
    uint32_t len = 0;
    std::vector<WBeat> exp;
    bool     have_exp = false;
    size_t   got = 0;
    ExpWr   *exp_ptr = nullptr;
};

struct Sim {
    // translation engine (one outstanding translation per bank by contract)
    struct Xl {
        bool     busy  = false;
        int      delay = 0;
        bool     error = false;
        uint64_t paddr = 0;
        uint64_t iova  = 0;
        uint32_t did   = 0;
    } xl[2];
    bool xl_accept   = true;                 // drives trans_req_ready
    bool xl_hold[2]  = {false, false};       // do not emit this bank's response yet

    bool     xl_resp_valid = false;
    int      xl_resp_bank  = 0;
    bool     xl_resp_error = false;
    uint64_t xl_resp_paddr = 0;

    // completion slave + scoreboard
    std::deque<CawEnt> caw_q;                // accepted comp AW awaiting their W
    std::deque<CmpWRec> cw_q;                // comp W beats captured independently of AW
    std::deque<BBeat>  b_q;
    std::deque<RBeat>  r_q;

    bool     aw_ready = true, ar_ready = true, w_ready = true;

    bool     b_valid = false; uint32_t b_id = 0; uint32_t b_resp = 0;
    bool     r_valid = false; uint32_t r_id = 0; uint64_t r_data = 0; bool r_last = false;
    uint32_t r_resp = 0;
};
static Sim sim;
static Vn_banked_top *dut = nullptr;

// ============================================================================
// Small helpers
// ============================================================================
static WBeat beat(uint64_t tag, bool last) {
    WBeat b; b.data = tag; b.strb = 0xFF; b.last = last; return b;
}

static ExpWr *claim_exp_wr(uint32_t id, uint64_t paddr) {
    for (auto &e : g_exp_wr)
        if (e.paddr == paddr && e.id == id && e.aw_hits == 0) { e.aw_hits++; return &e; }
    return nullptr;
}
static ExpRd *claim_exp_rd(uint32_t id, uint64_t paddr) {
    for (auto &e : g_exp_rd)
        if (e.paddr == paddr && e.id == id && e.ar_hits == 0) { e.ar_hits++; return &e; }
    return nullptr;
}
static bool saw_b(uint32_t id) {
    for (auto &x : L_up_b) if (x.id == id) return true;
    return false;
}
static bool saw_caw(uint32_t id) {
    for (auto &x : L_cmp_aw) if (x.id == id) return true;
    return false;
}

static void add_xlate(uint64_t iova, uint64_t paddr, bool error, uint32_t did) {
    XlEntry e; e.paddr = paddr; e.error = error; e.delay = 0; e.did = did;
    g_xlate[iova] = e;
}

static void expect_write(uint32_t id, uint64_t iova, uint64_t paddr,
                         bool error, uint32_t did,
                         const std::vector<WBeat> &beats) {
    add_xlate(iova, paddr, error, did);
    ExpWr e; e.id = id; e.paddr = paddr; e.beats = beats;
    g_exp_wr.push_back(e);
}
static void expect_read(uint32_t id, uint64_t iova, uint64_t paddr,
                        uint32_t len, bool error, uint32_t did) {
    add_xlate(iova, paddr, error, did);
    ExpRd e; e.id = id; e.paddr = paddr; e.len = len;
    g_exp_rd.push_back(e);
}

static void drive_write(uint32_t id, uint64_t iova, uint32_t len, uint32_t did,
                        const std::vector<WBeat> &beats) {
    up.aw_valid = true; up.aw_id = id; up.aw_addr = iova; up.aw_len = len;
    up.aw_size = 3; up.aw_burst = 1; up.aw_prot = 0; up.aw_did = did;
    up.wq.clear();
    for (const auto &b : beats) up.wq.push_back(b);
}
static void drive_read(uint32_t id, uint64_t iova, uint32_t len, uint32_t did) {
    up.ar_valid = true; up.ar_id = id; up.ar_addr = iova; up.ar_len = len;
    up.ar_size = 3; up.ar_burst = 1; up.ar_prot = 0; up.ar_did = did;
}

// ============================================================================
// Drive every DUT input from the current stimulus + model state
// ============================================================================
static void drive_upstream() {
    dut->tr_aw_valid = up.aw_valid ? 1 : 0;
    dut->tr_aw_id    = up.aw_id;
    dut->tr_aw_addr  = up.aw_addr;
    dut->tr_aw_len   = up.aw_len;
    dut->tr_aw_size  = up.aw_size;
    dut->tr_aw_burst = up.aw_burst;
    dut->tr_aw_prot  = up.aw_prot;
    dut->tr_aw_user  = up.aw_user;
    dut->tr_aw_did   = up.aw_did;
    dut->tr_aw_ssid_valid = up.aw_ssid ? 1 : 0;
    dut->tr_aw_subid = up.aw_subid;
    dut->tr_aw_mmu_atst  = up.aw_atst   ? 1 : 0;
    dut->tr_aw_mmu_valid = up.aw_mvalid ? 1 : 0;

    dut->tr_ar_valid = up.ar_valid ? 1 : 0;
    dut->tr_ar_id    = up.ar_id;
    dut->tr_ar_addr  = up.ar_addr;
    dut->tr_ar_len   = up.ar_len;
    dut->tr_ar_size  = up.ar_size;
    dut->tr_ar_burst = up.ar_burst;
    dut->tr_ar_prot  = up.ar_prot;
    dut->tr_ar_user  = up.ar_user;
    dut->tr_ar_did   = up.ar_did;
    dut->tr_ar_ssid_valid = up.ar_ssid ? 1 : 0;
    dut->tr_ar_subid = up.ar_subid;
    dut->tr_ar_mmu_atst  = up.ar_atst   ? 1 : 0;
    dut->tr_ar_mmu_valid = up.ar_mvalid ? 1 : 0;

    if (!up.wq.empty()) {
        dut->tr_w_valid = 1;
        dut->tr_w_data  = up.wq.front().data;
        dut->tr_w_strb  = up.wq.front().strb;
        dut->tr_w_last  = up.wq.front().last ? 1 : 0;
    } else {
        dut->tr_w_valid = 0;
        dut->tr_w_data  = 0;
        dut->tr_w_strb  = 0;
        dut->tr_w_last  = 0;
    }
    dut->tr_w_user = 0;

    dut->tr_b_ready = up.b_ready ? 1 : 0;
    dut->tr_r_ready = up.r_ready ? 1 : 0;

    dut->fullbuffer_jobs_i = (uint8_t)(g_fullbuffer_jobs & 0x3u);

    // model driven
    dut->trans_req_ready   = sim.xl_accept ? 1 : 0;
    dut->trans_resp_valid  = sim.xl_resp_valid ? 1 : 0;
    dut->trans_resp_bank   = sim.xl_resp_bank;
    dut->trans_resp_error  = sim.xl_resp_error ? 1 : 0;
    dut->trans_resp_spaddr = sim.xl_resp_paddr;

    dut->comp_aw_ready = sim.aw_ready ? 1 : 0;
    dut->comp_ar_ready = sim.ar_ready ? 1 : 0;
    dut->comp_w_ready  = sim.w_ready  ? 1 : 0;
    dut->comp_b_valid  = sim.b_valid  ? 1 : 0;
    dut->comp_b_id     = sim.b_id;
    dut->comp_b_resp   = sim.b_resp;
    dut->comp_r_valid  = sim.r_valid  ? 1 : 0;
    dut->comp_r_id     = sim.r_id;
    dut->comp_r_data   = sim.r_data;
    dut->comp_r_resp   = sim.r_resp;
    dut->comp_r_last   = sim.r_last ? 1 : 0;
}

// ============================================================================
// Observe DUT outputs (pre-edge) and service both behavioural models.
// Order: retire handshook model outputs -> capture DUT transactions ->
//        start new model outputs -> advance timers.
// ============================================================================
static void service() {
    expect(!dut->mrif_unexpected_o,"restricted bank emitted MRIF request/fault");
    std::fprintf(g_trace,"%ld,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%016llx,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
       g_cycle,dut->rst_ni,dut->fullbuffer_jobs_i,dut->tr_aw_valid,dut->tr_aw_ready,dut->tr_aw_id,dut->tr_w_valid,dut->tr_w_ready,dut->tr_w_last,dut->tr_ar_valid,dut->tr_ar_ready,dut->tr_ar_id,
       dut->trans_req_valid,dut->trans_req_ready,dut->trans_req_bank,(unsigned long long)dut->trans_req_iova,dut->trans_req_did,dut->trans_resp_valid,dut->trans_resp_ready,dut->trans_resp_bank,dut->trans_resp_error,
       dut->comp_aw_valid,dut->comp_aw_ready,dut->comp_w_valid,dut->comp_w_ready,dut->comp_w_last,dut->comp_ar_valid,dut->comp_ar_ready,dut->tr_b_valid,dut->tr_b_ready,dut->tr_b_resp,dut->tr_r_valid,dut->tr_r_ready);

    // (0) retire upstream stimulus on handshake + record every transfer
    if (up.aw_valid && dut->tr_aw_ready) {
        L_up_aw.push_back({g_cycle, up.aw_id, up.aw_addr, up.aw_len});
        up.w_attrib.push_back(std::make_pair(up.aw_id, (int)up.aw_len + 1));
        up.aw_valid = false;
    } else if (up.aw_valid) {
        ++g_up_aw_stall;
    }
    if (up.ar_valid && dut->tr_ar_ready) {
        L_up_ar.push_back({g_cycle, up.ar_id, up.ar_addr, up.ar_len});
        up.ar_valid = false;
    }
    if (!up.wq.empty() && dut->tr_w_ready) {
        uint32_t wid = up.w_attrib.empty() ? 0u : up.w_attrib.front().first;
        L_up_w.push_back({g_cycle, wid, up.wq.front().data, up.wq.front().strb, up.wq.front().last});
        if (!up.w_attrib.empty()) {
            if (--up.w_attrib.front().second <= 0) up.w_attrib.pop_front();
        }
        up.wq.pop_front();
    }
    if (dut->tr_b_valid && up.b_ready)
        L_up_b.push_back({g_cycle, dut->tr_b_id, (uint32_t)dut->tr_b_resp});
    if (dut->tr_r_valid && up.r_ready)
        L_up_r.push_back({g_cycle, dut->tr_r_id, dut->tr_r_data,
                          (bool)dut->tr_r_last, (uint32_t)dut->tr_r_resp});

    // sample the completion AW so stability while stalled can be proven
    L_cmp_aw_smp.push_back({g_cycle, dut->comp_aw_valid != 0,
                            dut->comp_aw_id, dut->comp_aw_addr, dut->comp_aw_len});

    // (1) retire model outputs that handshook this cycle
    if (sim.xl_resp_valid && dut->trans_resp_ready) {
        sim.xl[sim.xl_resp_bank].busy = false;
        sim.xl_resp_valid = false;
    }
    if (sim.b_valid && dut->comp_b_ready) { sim.b_q.pop_front(); sim.b_valid = false; }
    if (sim.r_valid && dut->comp_r_ready) { sim.r_q.pop_front(); sim.r_valid = false; }

    // (2) capture DUT-initiated transactions
    if (dut->trans_req_valid && dut->trans_req_ready) {
        int b = dut->trans_req_bank ? 1 : 0;
        uint64_t iova = dut->trans_req_iova;
        std::map<uint64_t, XlEntry>::iterator it = g_xlate.find(iova);
        expect(it != g_xlate.end(), "translation request for IOVA with no expected translation");
        if (it == g_xlate.end()) {
            sim.xl[b].error = true; sim.xl[b].paddr = 0; sim.xl[b].delay = 0;
        } else {
            sim.xl[b].error = it->second.error;
            sim.xl[b].paddr = it->second.paddr;
            sim.xl[b].delay = it->second.delay;
            expect_eq(dut->trans_req_did, it->second.did, "translation request DID mismatch");
        }
        sim.xl[b].busy = true;
        sim.xl[b].iova = iova;
        sim.xl[b].did  = dut->trans_req_did;
    }

    if (dut->comp_aw_valid && dut->comp_aw_ready) {
        L_cmp_aw.push_back({g_cycle, dut->comp_aw_id, dut->comp_aw_addr, dut->comp_aw_len});
        if (L_up_aw.empty() && !g_cmp_aw_pre_aw_reported) {
            g_cmp_aw_pre_aw_reported = true;
            std::printf("[cycle %ld] DIAG: completion AW handshake before any "
                        "upstream AW was accepted (id=0x%x addr=0x%llx len=%u "
                        "size=%u burst=%u) -> non-transactional valid?\n",
                        g_cycle, (unsigned)dut->comp_aw_id,
                        (unsigned long long)dut->comp_aw_addr,
                        (unsigned)dut->comp_aw_len, (unsigned)dut->comp_aw_size,
                        (unsigned)dut->comp_aw_burst);
        }
        CawEnt c;
        c.id = dut->comp_aw_id; c.addr = dut->comp_aw_addr; c.len = dut->comp_aw_len;
        ExpWr *e = claim_exp_wr(dut->comp_aw_id, dut->comp_aw_addr);
        if (e) {
            c.exp = e->beats; c.have_exp = true; c.exp_ptr = e;
            expect_eq((uint64_t)e->beats.size(), (uint64_t)dut->comp_aw_len + 1,
                      "comp AW len != expected beat count");
        } else {
            expect(false, "comp AW with unexpected id/addr (duplicate or wrong translation?)");
        }
        sim.caw_q.push_back(c);
    }

    if (dut->comp_ar_valid && dut->comp_ar_ready) {
        L_cmp_ar.push_back({g_cycle, dut->comp_ar_id, dut->comp_ar_addr, dut->comp_ar_len});
        ExpRd *e = claim_exp_rd(dut->comp_ar_id, dut->comp_ar_addr);
        if (e) expect_eq((uint64_t)e->len, (uint64_t)dut->comp_ar_len, "comp AR len mismatch");
        else   expect(false, "comp AR with unexpected id/addr");
        for (uint32_t i = 0; i <= (uint32_t)dut->comp_ar_len; ++i) {
            RBeat rb;
            rb.id = dut->comp_ar_id;
            rb.data = 0xA5A5A5A5A5A5A5A5ull + i;
            rb.last = (i == (uint32_t)dut->comp_ar_len);
            sim.r_q.push_back(rb);
        }
    }

    if (dut->comp_w_valid && dut->comp_w_ready) {
        L_cmp_w.push_back({g_cycle, dut->comp_w_data, dut->comp_w_strb, (bool)dut->comp_w_last});
        sim.cw_q.push_back({g_cycle, dut->comp_w_data, (uint64_t)dut->comp_w_strb, (bool)dut->comp_w_last});
    }
    while (!sim.caw_q.empty() && !sim.cw_q.empty()) {
        CawEnt &c = sim.caw_q.front();
        const CmpWRec w = sim.cw_q.front();
        if (!c.have_exp) {
            expect(false, "comp AW without expected beats");
            sim.caw_q.pop_front();
            continue;
        }
        if (c.got >= c.exp.size()) {
            expect(false, "comp AW has more W beats than expected");
            sim.caw_q.pop_front();
            continue;
        }
        const WBeat &e = c.exp[c.got];
        if (w.data != e.data || w.strb != e.strb || w.last != e.last) {
            expect(false, "comp W beat mismatch (data/strb/last) for oldest outstanding comp AW");
            sim.cw_q.pop_front();
            continue;
        }
        c.got++;
        sim.cw_q.pop_front();
        if (w.last) {
            expect_eq((uint64_t)c.got, (uint64_t)c.len + 1, "comp W burst length != len+1");
            BBeat bb; bb.id = c.id; bb.resp = 0;
            sim.b_q.push_back(bb);
            if (c.exp_ptr) c.exp_ptr->b_hits++;
            sim.caw_q.pop_front();
        }
    }

    // (3) start new model outputs
    if (!sim.xl_resp_valid) {
        for (int b = 0; b < 2; b++) {
            if (sim.xl[b].busy && sim.xl[b].delay == 0 && !sim.xl_hold[b]) {
                sim.xl_resp_valid = true;
                sim.xl_resp_bank  = b;
                sim.xl_resp_error = sim.xl[b].error;
                sim.xl_resp_paddr = sim.xl[b].paddr;
                break;
            }
        }
    }
    if (!sim.b_valid && !sim.b_q.empty()) {
        sim.b_valid = true; sim.b_id = sim.b_q.front().id; sim.b_resp = sim.b_q.front().resp;
    }
    if (!sim.r_valid && !sim.r_q.empty()) {
        sim.r_valid = true;
        sim.r_id    = sim.r_q.front().id;
        sim.r_data  = sim.r_q.front().data;
        sim.r_resp  = sim.r_q.front().resp;
        sim.r_last  = sim.r_q.front().last;
    }

    // (4) advance translation timers
    for (int b = 0; b < 2; b++) {
        if (sim.xl[b].busy && sim.xl[b].delay > 0 &&
            !(sim.xl_resp_valid && sim.xl_resp_bank == b)) {
            sim.xl[b].delay--;
        }
    }
}

// ============================================================================
// One full cycle: apply inputs (clk low), settle, service, rising edge.
// ============================================================================
static void sim_cycle() {
    drive_upstream();
    dut->clk_i = 0; dut->eval();      // pre-edge capture point
    service();                         // observe outputs / update models
    dut->clk_i = 1; dut->eval();       // rising edge: DUT registers latch
    ++g_cycle;
}

static void run_cycles(int n) { for (int i = 0; i < n; i++) sim_cycle(); }

// ============================================================================
// Reset
// ============================================================================
static void reset_all() {
    dut->rst_ni = 0;
    up  = Up();
    sim = Sim();
    g_fullbuffer_jobs = 0;
    g_xlate.clear();
    g_exp_wr.clear();
    g_exp_rd.clear();
    L_up_aw.clear();  L_up_ar.clear();  L_up_w.clear();
    L_up_b.clear();   L_up_r.clear();
    L_cmp_aw.clear(); L_cmp_ar.clear(); L_cmp_w.clear();
    L_cmp_aw_smp.clear();
    g_up_aw_stall = 0;
    g_cmp_aw_pre_aw_reported = false;

    for (int i = 0; i < 5; i++) {
        drive_upstream();
        dut->clk_i = 0; dut->eval();
        dut->clk_i = 1; dut->eval();
        ++g_cycle;
    }
    dut->rst_ni = 1;
    for (int i = 0; i < 2; i++) {
        drive_upstream();
        dut->clk_i = 0; dut->eval();
        dut->clk_i = 1; dut->eval();
        ++g_cycle;
    }
}

// ============================================================================
// Tests
// ============================================================================

// Bank-0 write, len=1 (two beats): end-to-end, ID/addr preservation, quiet.
static void test_write_bank0_multi() {
    std::printf("== write, bank 0, len=1 (2 beats) ==\n");
    reset_all();

    std::vector<WBeat> beats;
    beats.push_back(beat(0x1111222233334444ull, false));
    beats.push_back(beat(0xAAAABBBBCCCCDDDDull, true));

    expect_write(0x0, 0x1000, 0x80000000ull, false, 0x5, beats);
    drive_write(0x0, 0x1000, 1, 0x5, beats);
    run_cycles(140);

    expect_eq(L_cmp_aw.size(), 1, "comp AW count");
    if (!L_cmp_aw.empty()) {
        expect_eq(L_cmp_aw[0].id,   0x0,           "comp AW id not preserved");
        expect_eq(L_cmp_aw[0].addr, 0x80000000ull, "comp AW addr != translated paddr");
        expect_eq(L_cmp_aw[0].len,  1,             "comp AW len not preserved");
    }
    expect_eq(L_cmp_w.size(), 2, "comp W beat count");
    expect_eq(L_up_b.size(),  1, "upstream B count");
    if (!L_up_b.empty()) {
        expect_eq(L_up_b[0].id,   0x0, "upstream B id not restored");
        expect_eq(L_up_b[0].resp, 0x0, "upstream B resp != OKAY");
    }
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "write did not complete exactly once");
    expect(!dut->in_flight_o, "in_flight_o stuck after write");
}

// Two banks with distinct IDs: both responses restored onto one upstream port.
static void test_two_banks_distinct_ids() {
    std::printf("== two banks, distinct IDs restored upstream ==\n");
    reset_all();

    std::vector<WBeat> b0; b0.push_back(beat(0xB0B0B0B0B0B0B0B0ull, true));
    std::vector<WBeat> b1; b1.push_back(beat(0xB1B1B1B1B1B1B1B1ull, true));
    expect_write(0x1, 0x1000, 0x40000000ull, false, 0x1, b0);
    expect_write(0x4, 0x2000, 0x40001000ull, false, 0x4, b1);

    drive_write(0x1, 0x1000, 0, 0x1, b0);
    run_cycles(20);
    drive_write(0x4, 0x2000, 0, 0x4, b1);
    run_cycles(140);

    expect_eq(L_cmp_aw.size(), 2, "two comp AWs");
    expect(saw_caw(0x1), "comp AW for bank 0 id 0x1 missing");
    expect(saw_caw(0x4), "comp AW for bank 1 id 0x4 missing");
    expect_eq(L_up_b.size(), 2, "two upstream Bs");
    expect(saw_b(0x1), "upstream B for id 0x1 missing");
    expect(saw_b(0x4), "upstream B for id 0x4 missing");
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "write 0x1 completed once");
    expect_eq((uint64_t)g_exp_wr[1].b_hits, 1, "write 0x4 completed once");
    expect(!dut->in_flight_o, "in_flight_o stuck after two-bank writes");
}

// Error write to bank 1: absorbed by the bank error slave, never forwarded.
static void test_error_write_bank1() {
    std::printf("== error write, bank 1 ==\n");
    reset_all();

    std::vector<WBeat> beats; beats.push_back(beat(0xE1E1E1E1E1E1E1E1ull, true));
    expect_write(0x4, 0x2000, 0xDEADBEE0ull, true, 0x7, beats);   // translation error
    drive_write(0x4, 0x2000, 0, 0x7, beats);
    run_cycles(100);

    expect_eq(L_cmp_aw.size(), 0, "error write must not reach the completion AW");
    expect_eq(L_cmp_w.size(),  0, "error write must not reach the completion W");
    expect_eq(L_up_b.size(),   1, "error write must produce exactly one upstream B");
    if (!L_up_b.empty()) {
        expect_eq(L_up_b[0].id,   0x4, "error B id not restored");
        expect_eq(L_up_b[0].resp, 2,   "error B resp != SLVERR");
    }
    expect(!dut->in_flight_o, "in_flight_o stuck after error write");
}

// Different-bank bypass: bank 0's translation response is held; bank 1
// completes first, then bank 0 drains.
static void test_two_bank_reorder() {
    std::printf("== two banks, response reordered ==\n");
    reset_all();
    sim.xl_hold[0] = true;

    std::vector<WBeat> b0; b0.push_back(beat(0xA0A0A0A0A0A0A0A0ull, true));
    std::vector<WBeat> b1; b1.push_back(beat(0xA1A1A1A1A1A1A1A1ull, true));
    expect_write(0x0, 0x1000, 0x50000000ull, false, 0x0, b0);
    expect_write(0x4, 0x2000, 0x50001000ull, false, 0x4, b1);

    drive_write(0x0, 0x1000, 0, 0x0, b0);
    run_cycles(20);
    drive_write(0x4, 0x2000, 0, 0x4, b1);
    run_cycles(80);

    expect_eq(L_up_b.size(), 1, "bank 1 must complete while bank 0 is held");
    if (!L_up_b.empty()) expect_eq(L_up_b[0].id, 0x4, "first B must be bank 1's (id 0x4)");

    sim.xl_hold[0] = false;
    run_cycles(120);

    expect_eq(L_up_b.size(), 2, "two Bs after release");
    if (L_up_b.size() == 2) {
        expect_eq(L_up_b[0].id, 0x4, "bank 1 completed first");
        expect_eq(L_up_b[1].id, 0x0, "bank 0 completed second");
    }
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "bank 0 write completed once");
    expect_eq((uint64_t)g_exp_wr[1].b_hits, 1, "bank 1 write completed once");
    expect(!dut->in_flight_o, "in_flight_o stuck after two-bank reorder");
}

// Same-ID ordering + per-bank slot capacity blocked then released.
static void test_same_id_order_and_capacity() {
    std::printf("== same-ID order + 4 per-bank slots / global8 capacity ==\n");
    reset_all(); sim.xl_hold[0]=true;
    std::vector<std::vector<WBeat>> ws(5);
    for(unsigned i=0;i<5;i++) {
        ws[i].push_back(beat(0xd000000000000000ULL+i,true));
        expect_write(0,0x1000+0x1000*i,0x60000000+0x1000*i,false,0,ws[i]);
    }
    g_up_aw_stall=0;
    for(unsigned i=0;i<5;i++) {drive_write(0,0x1000+0x1000*i,0,0,ws[i]);run_cycles(10);}
    run_cycles(60);
    expect(g_up_aw_stall>0,"same-bank four-slot capacity never backpressured fifth AW");
    expect_eq(L_up_aw.size(),4,"held translations must accept exactly four AW slots");
    sim.xl_hold[0]=false;run_cycles(400);
    expect_eq(L_up_aw.size(),5,"all five AWs eventually accepted");
    expect_eq(L_cmp_aw.size(),5,"five completion AWs");expect_eq(L_up_b.size(),5,"five B responses");
    for(unsigned i=0;i<5;i++) {
        expect_eq(g_exp_wr[i].b_hits,1,"each write completed once");
        if(L_cmp_aw.size()==5)expect_eq(L_cmp_aw[i].addr,0x60000000+0x1000*i,"same-ID completion order");
    }
    expect(!dut->in_flight_o,"in_flight stuck after same-ID writes");
}

// Output pressure: AW stable while stalled, then fully drained after release.
static void test_output_backpressure_recovery() {
    std::printf("== completion backpressure + drain ==\n");
    reset_all();
    sim.aw_ready = false;

    std::vector<WBeat> beats;
    beats.push_back(beat(0xC0C0C0C0C0C0C0C0ull, false));
    beats.push_back(beat(0xC1C1C1C1C1C1C1C1ull, true));
    expect_write(0x0, 0x1000, 0x70000000ull, false, 0x0, beats);
    drive_write(0x0, 0x1000, 1, 0x0, beats);

    run_cycles(40);

    int seen = 0, stable = 0;
    uint64_t addr0 = 0;
    for (size_t i = 0; i < L_cmp_aw_smp.size(); ++i) {
        if (!L_cmp_aw_smp[i].valid) continue;
        ++seen;
        if (seen == 1) addr0 = L_cmp_aw_smp[i].addr;
        else if (L_cmp_aw_smp[i].addr == addr0) ++stable;
        else expect(false, "comp AW payload changed while stalled");
    }
    expect(seen > 0, "comp AW never asserted while comp_aw_ready was low");
    expect(stable >= 3, "comp AW payload not stable across >=3 stalled cycles");
    expect_eq(L_cmp_aw.size(), 0, "comp AW handshook while comp_aw_ready was low");

    sim.aw_ready = true;
    run_cycles(160);

    expect_eq(L_cmp_aw.size(), 1, "comp AW did not handshake after release");
    expect_eq(L_cmp_w.size(),  2, "comp W beats after release");
    expect_eq(L_up_b.size(),   1, "upstream B after release");
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "write completed exactly once");
    expect(L_cmp_aw_smp.size() > 0, "no completion AW samples recorded");
    expect(!dut->in_flight_o, "in_flight_o stuck after drain");
}

// W presented before AW: the model only commits beats the DUT is ready for.
static void test_w_before_aw() {
    std::printf("== W presented before AW ==\n");
    reset_all();

    std::vector<WBeat> beats; beats.push_back(beat(0xE0E0E0E0E0E0E0E0ull, true));
    expect_write(0x0, 0x1000, 0x90000000ull, false, 0x0, beats);

    up.wq.push_back(beats[0]);
    up.aw_valid = false;
    run_cycles(5);                 // W offered first; nothing may be forced

    up.aw_valid = true; up.aw_id = 0x0; up.aw_addr = 0x1000; up.aw_len = 0;
    up.aw_size = 3; up.aw_burst = 1; up.aw_prot = 0; up.aw_did = 0x0;
    run_cycles(140);

    expect_eq(L_up_aw.size(),  1, "AW eventually accepted");
    expect_eq(L_up_w.size(),   1, "W beat accepted exactly once");
    expect_eq(L_cmp_aw.size(), 1, "comp AW");
    expect_eq(L_up_b.size(),   1, "upstream B");
    expect_eq((uint64_t)g_exp_wr[0].b_hits, 1, "write completed once");
    expect(!dut->in_flight_o, "in_flight_o stuck after W-before-AW");
}

// Single-beat read on bank 0.
static void test_read_bank0_single() {
    std::printf("== read, bank 0, len=0 ==\n");
    reset_all();

    expect_read(0x0, 0x4000, 0xCAFEF000ull, 0, false, 0x9);
    drive_read(0x0, 0x4000, 0, 0x9);
    run_cycles(100);

    expect_eq(L_cmp_ar.size(), 1, "comp AR count");
    if (!L_cmp_ar.empty()) {
        expect_eq(L_cmp_ar[0].id,   0x0,           "comp AR id not preserved");
        expect_eq(L_cmp_ar[0].addr, 0xCAFEF000ull, "comp AR addr != translated paddr");
        expect_eq(L_cmp_ar[0].len,  0,             "comp AR len");
    }
    expect_eq(L_up_r.size(), 1, "upstream R beat count");
    if (!L_up_r.empty()) {
        expect_eq(L_up_r[0].id,   0x0, "upstream R id not restored");
        expect(L_up_r[0].last,         "upstream R last");
        expect_eq(L_up_r[0].resp, 0x0, "upstream R resp != OKAY");
    }
    expect(!dut->in_flight_o, "in_flight_o stuck after read");
}

// Multi-beat read on bank 1: the read sender must honour len+1 and WLAST-like
// R-LAST placement.
static void test_read_bank1_multi() {
    std::printf("== read, bank 1, len=1 ==\n");
    reset_all();

    expect_read(0x4, 0x4000, 0xC0FFEE00ull, 1, false, 0x4);
    drive_read(0x4, 0x4000, 1, 0x4);
    run_cycles(160);

    expect_eq(L_cmp_ar.size(), 1, "comp AR count");
    if (!L_cmp_ar.empty()) {
        expect_eq(L_cmp_ar[0].id,  0x4, "comp AR id not preserved");
        expect_eq(L_cmp_ar[0].len, 1,   "comp AR len mismatch");
    }
    expect_eq(L_up_r.size(), 2, "two upstream R beats");
    if (L_up_r.size() == 2) {
        expect(!L_up_r[0].last,     "first R must not be LAST");
        expect(L_up_r[1].last,      "second R must be LAST");
        expect_eq(L_up_r[0].id, 0x4, "R id not restored");
    }
    expect(!dut->in_flight_o, "in_flight_o stuck after multi-beat read");
}

// Error read on bank 0: absorbed by the bank error slave, never forwarded,
// and the upstream still gets an error R.
static void test_error_read_bank0() {
    std::printf("== error read, bank 0 ==\n");
    reset_all();

    expect_read(0x0, 0x4000, 0x0ull, 0, true, 0x9);   // translation error
    drive_read(0x0, 0x4000, 0, 0x9);
    run_cycles(100);

    expect_eq(L_cmp_ar.size(), 0, "error read must not reach the completion AR");
    expect_eq(L_up_r.size(),   1, "error read must produce one upstream R");
    if (!L_up_r.empty()) {
        expect_eq(L_up_r[0].id, 0x0, "error R id not restored");
        expect(L_up_r[0].resp != 0,  "error R resp must signal an error (not OKAY)");
    }
    expect(!dut->in_flight_o, "in_flight_o stuck after error read");
}

// ============================================================================
// v5 full-buffer READ admission:
//   * bank 1 has an accepted A AW with no W beats, so w_unfilled_q > 0 and
//     bank 1 is the W owner;
//   * a queued bank-0 READ with DID10 (the bank-0 FullBufferDID) is admitted
//     only when fullbuffer_jobs_i[0] is set;
//   * a queued bank-0 READ with DID12 remains blocked even with permit bit 0
//     set, because its DID does not match the bank's FullBufferDID.
//
// The A write is intentionally left unfinished: the bank-1 translation
// response is held, so w_unfilled_q remains > 0.  The test is bounded and
// never checks the global quiet condition for this unfinished write.  No
// upstream AW reorder is attempted: exactly one legal AW is accepted.
// ============================================================================
static void test_fullbuffer_read_admission() {
    std::printf("== v5 full-buffer READ admission vs W-owner guard ==\n");
    reset_all();

    // Hold bank-1 translation response so the A write stays in flight.
    sim.xl_hold[1] = true;

    // A (bank 1) AW accepted, no W beats: w_unfilled_q becomes 1 and the
    // current W owner is bank 1.
    add_xlate(0x2000, 0x50000000ull, false, 0xB);
    std::vector<WBeat> no_w;
    drive_write(0x4, 0x2000, 0, 0xB, no_w);   // AXI id bit 2 = 1 -> bank 1
    run_cycles(20);
    expect_eq(L_up_aw.size(), 1, "A AW was not accepted");
    expect(sim.xl[1].busy, "bank-1 A translation was not selected / held");

    // Expected DID10 and DID12 reads (both bank 0: AXI id bit 2 == 0).
    expect_read(0x0, 0x4000, 0xCAFE1000ull, 0, false, 0xA);
    expect_read(0x0, 0x5000, 0xCAFE2000ull, 0, false, 0xC);

    // --- permit bit 0 clear: the DID10 read is buffered upstream but must not
    //     be selected by the bank-0 controller.
    g_fullbuffer_jobs = 0u;
    drive_read(0x0, 0x4000, 0, 0xA);
    run_cycles(40);
    expect_eq(L_up_ar.size(), 1, "DID10 AR was not buffered upstream");
    expect(!sim.xl[0].busy, "DID10 read was selected with fullbuffer permit=0");
    expect_eq(L_cmp_ar.size(), 0, "DID10 reached completion with permit=0");

    // --- permit bit 0 set: the queued qualified DID10 read is now selected
    //     even though bank 1 is the unfilled W owner.
    g_fullbuffer_jobs = 1u;
    for (int i = 0; i < 160 && L_cmp_ar.size() == 0; ++i) sim_cycle();
    expect_eq(L_cmp_ar.size(), 1, "DID10 completion AR count");
    if (!L_cmp_ar.empty()) {
        expect_eq(L_cmp_ar[0].addr, 0xCAFE1000ull, "DID10 comp AR addr");
    }
    for (int i = 0; i < 80; ++i) sim_cycle();
    expect_eq(L_up_r.size(), 1, "DID10 upstream R count");

    // --- same bank, non-matching DID12: permit bit 0 is still set, but the
    //     original W-owner guard must keep it unselected.
    g_fullbuffer_jobs = 1u;
    drive_read(0x0, 0x5000, 0, 0xC);
    run_cycles(80);
    expect_eq(L_up_ar.size(), 2, "DID12 AR was not buffered upstream");
    expect_eq(L_cmp_ar.size(), 1, "DID12 read was selected despite non-matching FullBufferDID");

    // Keep the A write unfinished; this test is bounded and never checks quiet.
}

// ============================================================================
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    expect(argc==2,"need trace path"); if(argc!=2)return 2;
    g_trace=std::fopen(argv[1],"w"); if(!g_trace)return 2;
    std::fprintf(g_trace,"cycle,rst,fullbuffer_test_interface,awvalid,awready,awid,wvalid,wready,wlast,arvalid,arready,arid,tlvalid,tlready,tlbank,iova,did,respvalid,respready,respbank,resperror,cawvalid,cawready,cwvalid,cwready,cwlast,carvalid,carready,bvalid,bready,bresp,rvalid,rready\n");
    dut = new Vn_banked_top;

    test_write_bank0_multi();
    test_two_banks_distinct_ids();
    test_error_write_bank1();
    test_two_bank_reorder();
    test_same_id_order_and_capacity();
    test_output_backpressure_recovery();
    test_w_before_aw();
    test_read_bank0_single();
    test_read_bank1_multi();
    test_error_read_bank0();
    test_fullbuffer_read_admission();

    dut->final();
    delete dut;

    if (g_fail) {
        std::printf("FAILED: %d check(s)\n", g_fail);
        return 1;
    }
    std::fclose(g_trace); std::printf("PASS n_banked cases=11 checks=%u cycles=%ld; real banked controllers/AXI; synthetic TL responses and fullbuffer test-interface; no actual DMA certificate/backend/board\n",g_checks,g_cycle);
    return 0;
}
