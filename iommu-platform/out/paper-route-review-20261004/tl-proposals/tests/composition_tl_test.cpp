// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vcomposition_tl_top.h"
#include "verilated.h"

double sc_time_stamp() { return 0; }

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <map>
#include <string>
#include <vector>

static void tb_fail(const char* msg, const char* file, int line) {
    std::fprintf(stderr, "composition_tl_test: FAIL: %s (%s:%d)\n",
                 msg, file, line);
    std::abort();
}

#define FAIL_AT(msg) tb_fail((msg), __FILE__, __LINE__)
#define CHECK_AT(cond, msg) do { if (!(cond)) tb_fail((msg), __FILE__, __LINE__); } while (0)

struct Resp {
    uint64_t spaddr     = 0;
    uint8_t  bank       = 0;
    bool     error      = false;
    bool     is_mrif    = false;
    uint16_t fault_code = 0;
};

struct Fill {
    uint32_t did  = 0;
    uint16_t mask = 0;
};

struct AxiReadState {
    bool     pending = false;
    uint64_t addr    = 0;
    uint8_t  len     = 0;
    uint8_t  size    = 0;
    uint8_t  id      = 0;
    int      beat    = 0;
};

class Tb {
public:
    Vcomposition_tl_top* top;
    std::map<uint64_t, uint64_t> mem;

    AxiReadState ddtw_rd;
    AxiReadState ptw_rd;

    bool hold_ptw_r = false;

    bool     req_fire      = false;
    bool     req_fire_fast = false;
    bool     req_busy_s    = false;
    bool     d_ar_fire_last = false;
    bool     p_ar_fire_last = false;
    bool     resp_valid_s  = false;
    uint64_t resp_spaddr_s = 0;
    uint8_t  resp_bank_s   = 0;
    bool     resp_error_s  = false;
    bool     quiet_s       = false;
    bool     saw_nonblock_fire = false;

    std::deque<Resp> resp_q;
    std::vector<Fill> fills;

    bool latched_req_fire = false;

    Tb() : top(new Vcomposition_tl_top) {}
    ~Tb() {
        top->final();
        delete top;
    }

    // -- memory ----------------------------------------------------------
    void write64(uint64_t addr, uint64_t data) { mem[addr] = data; }

    uint64_t read64(uint64_t addr) {
        auto it = mem.find(addr);
        if (it == mem.end()) {
            std::fprintf(stderr,
                         "composition_tl_test: unmapped read @0x%016lx\n",
                         (unsigned long)addr);
            std::abort();
        }
        return it->second;
    }

    // -- drive R channels from the current model state -------------------
    void drive_ddtw_r() {
        if (ddtw_rd.pending) {
            top->ddtw_r_valid = 1;
            top->ddtw_r_data  = read64(ddtw_rd.addr +
                                       (uint64_t)ddtw_rd.beat *
                                           (1ULL << ddtw_rd.size));
            top->ddtw_r_id    = ddtw_rd.id;
            top->ddtw_r_resp  = 0;
            top->ddtw_r_last  = (ddtw_rd.beat == ddtw_rd.len);
        } else {
            top->ddtw_r_valid = 0;
            top->ddtw_r_data  = 0;
            top->ddtw_r_id    = 0;
            top->ddtw_r_resp  = 0;
            top->ddtw_r_last  = 0;
        }
    }

    void drive_ptw_r() {
        if (ptw_rd.pending && !hold_ptw_r) {
            top->ptw_r_valid = 1;
            top->ptw_r_data  = read64(ptw_rd.addr +
                                      (uint64_t)ptw_rd.beat *
                                          (1ULL << ptw_rd.size));
            top->ptw_r_id    = ptw_rd.id;
            top->ptw_r_resp  = 0;
            top->ptw_r_last  = (ptw_rd.beat == ptw_rd.len);
        } else {
            top->ptw_r_valid = 0;
            top->ptw_r_data  = 0;
            top->ptw_r_id    = 0;
            top->ptw_r_resp  = 0;
            top->ptw_r_last  = 0;
        }
    }

    // -- one full clock cycle -------------------------------------------
    void tick() {
        top->ddtw_ar_ready = ddtw_rd.pending ? 0 : 1;
        top->ptw_ar_ready  = ptw_rd.pending  ? 0 : 1;
        drive_ddtw_r();
        drive_ptw_r();

        top->eval();

        const bool     d_ar_fire = top->ddtw_ar_valid && top->ddtw_ar_ready;
        const uint64_t d_ar_addr = top->ddtw_ar_addr;
        const uint8_t  d_ar_len  = top->ddtw_ar_len;
        const uint8_t  d_ar_size = top->ddtw_ar_size;
        const uint8_t  d_ar_id   = top->ddtw_ar_id;
        const bool     d_r_fire  = ddtw_rd.pending &&
                                   top->ddtw_r_valid && top->ddtw_r_ready;

        const bool     p_ar_fire = top->ptw_ar_valid && top->ptw_ar_ready;
        const uint64_t p_ar_addr = top->ptw_ar_addr;
        const uint8_t  p_ar_len  = top->ptw_ar_len;
        const uint8_t  p_ar_size = top->ptw_ar_size;
        const uint8_t  p_ar_id   = top->ptw_ar_id;
        const bool     p_r_fire  = ptw_rd.pending &&
                                   top->ptw_r_valid && top->ptw_r_ready;

        req_fire      = top->trans_req_valid_i && top->trans_req_ready_o;
        req_fire_fast = req_fire && top->nonblock_fire_o;
        req_busy_s    = top->nonblock_busy_o;
        quiet_s       = top->policy_quiet_o;
        if (top->nonblock_fire_o) saw_nonblock_fire = true;

        if (top->research_update_o) {
            fills.push_back(Fill{top->research_did_o,
                                 top->research_replace_mask_o});
        }

        resp_valid_s  = top->trans_resp_valid_o;
        resp_spaddr_s = top->resp_spaddr;
        resp_bank_s   = top->resp_bank;
        resp_error_s  = top->resp_error;

        const bool resp_fire = top->trans_resp_valid_o &&
                               top->trans_resp_ready_i;
        Resp resp_snap;
        if (resp_fire) {
            resp_snap.spaddr     = top->resp_spaddr;
            resp_snap.bank       = top->resp_bank;
            resp_snap.error      = top->resp_error;
            resp_snap.is_mrif    = top->resp_is_mrif;
            resp_snap.fault_code = top->resp_fault_code & 0xFFF;
        }

        top->clk_i = 1;
        top->eval();

        d_ar_fire_last = d_ar_fire;
        p_ar_fire_last = p_ar_fire;

        if (d_ar_fire) {
            if (ddtw_rd.pending)
                FAIL_AT("DDTW AR fired while a read was already pending");
            ddtw_rd.pending = true;
            ddtw_rd.addr    = d_ar_addr;
            ddtw_rd.len     = d_ar_len;
            ddtw_rd.size    = d_ar_size;
            ddtw_rd.id      = d_ar_id;
            ddtw_rd.beat    = 0;
        }
        if (d_r_fire) {
            if (ddtw_rd.beat == ddtw_rd.len) ddtw_rd.pending = false;
            else                             ddtw_rd.beat++;
        }

        if (p_ar_fire) {
            if (ptw_rd.pending)
                FAIL_AT("PTW AR fired while a read was already pending");
            ptw_rd.pending = true;
            ptw_rd.addr    = p_ar_addr;
            ptw_rd.len     = p_ar_len;
            ptw_rd.size    = p_ar_size;
            ptw_rd.id      = p_ar_id;
            ptw_rd.beat    = 0;
        }
        if (p_r_fire) {
            if (ptw_rd.beat == ptw_rd.len) ptw_rd.pending = false;
            else                           ptw_rd.beat++;
        }

        if (resp_fire) resp_q.push_back(resp_snap);

        top->clk_i = 0;
        top->eval();
    }

    // -- reset -----------------------------------------------------------
    void reset(bool nonblock_enable, bool policy_partition,
               uint32_t policy_did = 10) {
        top->clk_i = 0;
        top->rst_ni = 0;
        top->nonblock_enable_i = nonblock_enable ? 1 : 0;
        top->policy_partition_i = policy_partition ? 1 : 0;
        top->policy_clear_i = 0;
        top->policy_did_i = policy_did & 0xFFFFFF;
        top->trans_req_valid_i = 0;
        top->trans_resp_ready_i = 1;

        top->req_did = 0;       top->req_iova = 0;
        top->req_ttype = 0;     top->req_pid = 0;
        top->req_pid_valid = 0; top->req_priv = 0;
        top->req_is_debug = 0;  top->req_bank = 0;

        top->inval_vma = 0;  top->inval_gvma = 0;
        top->inval_av = 0;   top->inval_gv = 0;
        top->inval_pscv = 0; top->inval_vpn = 0;
        top->inval_gscid = 0; top->inval_pscid = 0;
        top->iodir_inval_ddtc = 0; top->iodir_inval_pdtc = 0;
        top->iodir_dv = 0;   top->iodir_did = 0; top->iodir_pid = 0;

        top->ddtw_r_valid = 0; top->ddtw_r_data = 0; top->ddtw_r_id = 0;
        top->ddtw_r_resp = 0;  top->ddtw_r_last = 0;
        top->ptw_r_valid = 0;  top->ptw_r_data = 0;  top->ptw_r_id = 0;
        top->ptw_r_resp = 0;   top->ptw_r_last = 0;

        ddtw_rd = AxiReadState();
        ptw_rd  = AxiReadState();
        hold_ptw_r = false;

        req_fire = false; req_fire_fast = false; req_busy_s = false;
        d_ar_fire_last = false; p_ar_fire_last = false;
        resp_valid_s = false;
        quiet_s = false;
        saw_nonblock_fire = false;
        resp_q.clear();
        fills.clear();
        latched_req_fire = false;

        for (int i = 0; i < 8; ++i) tick();
        top->rst_ni = 1;
        tick();

        fills.clear();
        saw_nonblock_fire = false;
    }

    // -- request helpers -------------------------------------------------
    void set_req(uint32_t did, uint64_t iova, uint8_t ttype, uint8_t bank,
                 bool valid) {
        top->req_did       = did & 0xFFFFFF;
        top->req_iova      = iova;
        top->req_ttype     = ttype & 0x3F;
        top->req_pid       = 0;
        top->req_pid_valid = 0;
        top->req_priv      = 0;
        top->req_is_debug  = 0;
        top->req_bank      = bank ? 1 : 0;
        top->trans_req_valid_i = valid ? 1 : 0;
    }

    bool wait_accept(int maxc = 40000) {
        if (latched_req_fire) {
            latched_req_fire = false;
            return true;
        }
        for (int i = 0; i < maxc; ++i) {
            tick();
            if (req_fire) return true;
        }
        return false;
    }

    Resp pop_resp_track_ptw(bool& saw_ptw, int maxc = 200000) {
        for (int i = 0; i < maxc; ++i) {
            if (!resp_q.empty()) {
                Resp r = resp_q.front();
                resp_q.pop_front();
                return r;
            }
            tick();
            if (req_fire) latched_req_fire = true;
            if (p_ar_fire_last) saw_ptw = true;
        }
        FAIL_AT("timeout waiting for a translation response");
        return Resp();
    }

    Resp pop_resp(int maxc = 200000) {
        bool saw_ptw = false;
        return pop_resp_track_ptw(saw_ptw, maxc);
    }

    // -- A-hang helpers --------------------------------------------------
    void start_cold_a_and_hold(uint64_t iova, uint8_t bank) {
        set_req(11, iova, 0x2, bank, true);
        for (int i = 0; i < 20000; ++i) {
            tick();
            if (p_ar_fire_last) {
                hold_ptw_r = true;
                set_req(11, 0, 0, 0, false);
                return;
            }
        }
        FAIL_AT("start_cold_a_and_hold: no A PTW AR before timeout");
    }

    void release_a_raw() { hold_ptw_r = false; }

    Resp release_a() {
        hold_ptw_r = false;
        return pop_resp();
    }

    // -- memory image ----------------------------------------------------
    void setup_memory() {
        // DDT is 1-level: leaf stride 64 bytes at base 0x10000.
        // DID10 -> 0x10280, DID11 -> 0x102C0.
        write64(0x10280, 0x0000000000000001ULL); // tc: v=1 only
        write64(0x10288, 0x0000000000000000ULL); // iohgatp: Bare
        write64(0x10290, 0x000000000000A000ULL); // ta: pscid=10 (<<12)
        write64(0x10298, 0x8000000000000020ULL); // fsc: Sv39, ppn=0x20

        write64(0x102C0, 0x0000000000000001ULL); // tc: v=1 only
        write64(0x102C8, 0x0000000000000000ULL); // iohgatp: Bare
        write64(0x102D0, 0x000000000000B000ULL); // ta: pscid=11
        write64(0x102D8, 0x8000000000000030ULL); // fsc: Sv39, ppn=0x30

        // V page tables: root 0x20000, L1V 0x40000, L0V 0x50000.
        write64(0x20000, 0x0000000000010001ULL); // root V -> L1V (ppn=0x40)
        write64(0x40400, 0x0000000000014001ULL); // L1V -> L0V (ppn=0x50)
        write64(0x50000, 0x00000000200000DFULL); // V0 -> phys 0x80000000
        write64(0x50008, 0x00000000200004DFULL); // V1 -> phys 0x80001000
        write64(0x50010, 0x00000000200000D3ULL); // Vro -> r=1, w=0, x=0
        write64(0x50018, 0x00000000200008DFULL); // Vcold -> phys 0x80002000

        // A page tables: root 0x30000, L1A 0x60000, L0A 0x70000.
        write64(0x30000, 0x0000000000018001ULL); // root A -> L1A (ppn=0x60)
        write64(0x60800, 0x000000000001C001ULL); // L1A -> L0A (ppn=0x70)
        // 16 A pages: IOVA 0x20000000 + n*0x1000 -> phys 0x90000000 + n*0x1000
        for (int n = 0; n < 16; ++n) {
            const uint64_t ppn = 0x90000ULL + (uint64_t)n;
            const uint64_t pte = (ppn << 10) | 0xDFULL;
            write64(0x70000 + (uint64_t)n * 8, pte);
        }
    }

    // -- fillmask checker ------------------------------------------------
    void check_all_fills(bool partition_on) {
        if (!partition_on) return;
        for (size_t i = 0; i < fills.size(); ++i) {
            const uint16_t m = fills[i].mask;
            CHECK_AT(__builtin_popcount((unsigned)m) == 1,
                     "fill mask is not one-hot");
            if (fills[i].did == 10) {
                CHECK_AT((m & ~((uint16_t)0x0003)) == 0,
                         "V (DID10) fill outside protected ways");
            } else if (fills[i].did == 11) {
                CHECK_AT((m & ((uint16_t)0x0003)) == 0,
                         "A (DID11) fill in protected ways");
            } else {
                FAIL_AT("unexpected IOTLB fill owner DID");
            }
        }
    }

    // -- warm a leaf through the normal path -----------------------------
    void warm_leaf(uint32_t did, uint64_t iova, uint64_t expect_spaddr,
                   uint8_t bank) {
        set_req(did, iova, 0x2, bank, true);
        if (!wait_accept()) FAIL_AT("warm_leaf: request was not accepted");
        set_req(did, 0, 0, 0, false);
        bool saw_ptw = false;
        Resp r = pop_resp_track_ptw(saw_ptw);
        CHECK_AT(!r.error, "warm_leaf: unexpected error response");
        CHECK_AT(r.bank == bank, "warm_leaf: response bank mismatch");
        CHECK_AT(r.spaddr == expect_spaddr,
                 "warm_leaf: unexpected translated address");
    }

    // -- cases -----------------------------------------------------------
    void case_mode_gating(const char* name, bool n, bool p) {
        reset(n, p, 10);

        // Warm V page 0 via the normal path.
        warm_leaf(10, 0x10000000ULL, 0x80000000ULL, 0);

        // Second V probe.
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("mode: second V probe not accepted");
        const bool was_fast = req_fire_fast;
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "mode: V response mismatch");

        if (n) {
            CHECK_AT(was_fast,
                     "mode: expected fast path fire with N enabled");
            CHECK_AT(saw_nonblock_fire,
                     "mode: nonblock_fire_o never asserted with N enabled");
        } else {
            CHECK_AT(!was_fast,
                     "mode: fast path fired with N disabled");
            CHECK_AT(!saw_nonblock_fire,
                     "mode: nonblock_fire_o asserted with N disabled");
        }

        // Cold A page 0.
        warm_leaf(11, 0x20000000ULL, 0x90000000ULL, 1);

        if (p) {
            check_all_fills(true);
        }
    }

    void case_pn_allocation_with_concurrent_v_probe() {
        reset(true, true, 10);

        // Warm two V pages (DID10) -> protected ways.
        warm_leaf(10, 0x10000000ULL, 0x80000000ULL, 0);
        warm_leaf(10, 0x10001000ULL, 0x80001000ULL, 0);
        check_all_fills(true);

        // Start cold A page 0 and hold it in PT.
        start_cold_a_and_hold(0x20000000ULL, 1);

        // Concurrent V fast probe while A is in PT.
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("concurrent: V probe not accepted");
        CHECK_AT(req_fire_fast,
                 "concurrent: V probe not accepted by fast path");
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "concurrent: V fast response mismatch");

        // Release A and let it fill.
        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error && ra.spaddr == 0x90000000ULL,
                 "concurrent: A response mismatch");
        check_all_fills(true);

        // Fill the remaining A pages to exercise all 14 non-protected ways.
        for (int n = 1; n < 15; ++n) {
            const uint64_t iova = 0x20000000ULL + (uint64_t)n * 0x1000ULL;
            const uint64_t phys = 0x90000000ULL + (uint64_t)n * 0x1000ULL;
            warm_leaf(11, iova, phys, 1);
        }
        check_all_fills(true);

        // V protected entries must still be present and fast-hittable.
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("post-A: V probe not accepted");
        CHECK_AT(req_fire_fast,
                 "post-A: V probe did not fast hit; protected entry evicted");
        set_req(10, 0, 0, 0, false);
        Resp rv_post = pop_resp();
        CHECK_AT(rv_post.bank == 0 && !rv_post.error &&
                 rv_post.spaddr == 0x80000000ULL,
                 "post-A: V fast response mismatch");
        check_all_fills(true);
    }

    void case_policy_quiet() {
        reset(true, true, 10);

        set_req(0, 0, 0, 0, false);
        for (int i = 0; i < 50; ++i) tick();
        CHECK_AT(quiet_s, "quiet: not true in idle");

        // (a) Incoming valid in the same cycle.
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        tick();
        CHECK_AT(!quiet_s,
                 "quiet: true with incoming valid in the same cycle");
        bool accepted = req_fire;
        if (!accepted) accepted = wait_accept();
        if (!accepted) FAIL_AT("quiet: cold V not accepted");
        set_req(10, 0, 0, 0, false);
        Resp r0 = pop_resp();
        CHECK_AT(r0.bank == 0 && !r0.error && r0.spaddr == 0x80000000ULL,
                 "quiet: cold V response mismatch");
        for (int i = 0; i < 50; ++i) tick();
        CHECK_AT(quiet_s, "quiet: not true after cold V drain");

        // (b) Slow walk.
        start_cold_a_and_hold(0x20000000ULL, 1);
        CHECK_AT(!quiet_s, "quiet: true during slow A walk");
        release_a_raw();
        Resp ra = pop_resp();
        CHECK_AT(ra.bank == 1 && !ra.error && ra.spaddr == 0x90000000ULL,
                 "quiet: A response mismatch");
        for (int i = 0; i < 50; ++i) tick();
        CHECK_AT(quiet_s, "quiet: not true after A drain");

        // (c) Fast response / backpressure.
        top->trans_resp_ready_i = 0;
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("quiet: fast V probe not accepted");
        CHECK_AT(req_fire_fast, "quiet: V probe did not fast fire");
        CHECK_AT(!quiet_s, "quiet: true while fast response pending");
        set_req(10, 0, 0, 0, false);
        for (int i = 0; i < 100; ++i) tick();
        CHECK_AT(!quiet_s,
                 "quiet: true while fast response backpressured");
        top->trans_resp_ready_i = 1;
        Resp rf = pop_resp();
        CHECK_AT(rf.bank == 0 && !rf.error && rf.spaddr == 0x80000000ULL,
                 "quiet: fast response mismatch");
        for (int i = 0; i < 50; ++i) tick();
        CHECK_AT(quiet_s, "quiet: not true after fast drain");
    }

    void case_idle_clear_rewarm() {
        reset(true, true, 10);

        warm_leaf(10, 0x10000000ULL, 0x80000000ULL, 0);

        // Pre-clear fast hit.
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("clear: pre-clear probe not accepted");
        CHECK_AT(req_fire_fast, "clear: pre-clear probe not fast");
        set_req(10, 0, 0, 0, false);
        Resp r1 = pop_resp();
        CHECK_AT(r1.bank == 0 && !r1.error && r1.spaddr == 0x80000000ULL,
                 "clear: pre-clear response mismatch");

        for (int i = 0; i < 50; ++i) tick();
        CHECK_AT(quiet_s, "clear: not quiet before clear");

        // Idle clear.
        top->policy_clear_i = 1;
        tick();
        top->policy_clear_i = 0;
        tick();

        // Post-clear: V must miss and re-warm.
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("clear: post-clear V not accepted");
        CHECK_AT(!req_fire_fast,
                 "clear: post-clear V fast hit; IOTLB not cleared");
        set_req(10, 0, 0, 0, false);
        bool saw_ptw = false;
        Resp r2 = pop_resp_track_ptw(saw_ptw);
        CHECK_AT(saw_ptw, "clear: post-clear V did not walk");
        CHECK_AT(r2.bank == 0 && !r2.error && r2.spaddr == 0x80000000ULL,
                 "clear: post-clear V response mismatch");

        // Re-warmed: fast hit works again.
        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("clear: post-rewarm V not accepted");
        CHECK_AT(req_fire_fast, "clear: post-rewarm V not fast");
        set_req(10, 0, 0, 0, false);
        Resp r3 = pop_resp();
        CHECK_AT(r3.bank == 0 && !r3.error && r3.spaddr == 0x80000000ULL,
                 "clear: post-rewarm response mismatch");
    }
};

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Tb tb;
    tb.setup_memory();

    tb.case_mode_gating("S",  false, false);
    tb.case_mode_gating("N",  true,  false);
    tb.case_mode_gating("P",  false, true);
    tb.case_mode_gating("PN", true,  true);

    tb.case_pn_allocation_with_concurrent_v_probe();
    tb.case_policy_quiet();
    tb.case_idle_clear_rewarm();

    std::printf("composition_tl_test: PASS\n");
    return 0;
}
