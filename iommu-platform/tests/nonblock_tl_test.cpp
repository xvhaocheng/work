// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vnonblock_tl_top.h"
#include "verilated.h"

double sc_time_stamp() { return 0; }

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <map>
#include <vector>

// ---------------------------------------------------------------------------
// Failure helper: always print a concrete reason before aborting.
// ---------------------------------------------------------------------------
#define FAIL_AT(msg)                                                          \
    do {                                                                      \
        std::fprintf(stderr, "nonblock_tl_test: FAIL: %s (%s:%d)\n", (msg),   \
                     __FILE__, __LINE__);                                     \
        std::abort();                                                         \
    } while (0)

#define CHECK_AT(cond, msg)                                                   \
    do {                                                                      \
        if (!(cond)) FAIL_AT(msg);                                            \
    } while (0)

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------
struct Resp {
    uint64_t spaddr     = 0;
    uint8_t  bank       = 0;
    bool     error      = false;
    bool     is_mrif    = false;
    uint16_t fault_code = 0;
};

// Minimal AXI read responder / memory model.
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
    Vnonblock_tl_top* top;
    std::map<uint64_t, uint64_t> mem;

    AxiReadState ddtw_rd;
    AxiReadState ptw_rd;

    bool hold_ptw_r = false;

    // Pre-edge samples of the previous tick().
    bool     req_fire      = false;
    bool     req_fire_fast = false;
    bool     req_busy_s    = false;
    bool     d_ar_fire_last = false;
    bool     p_ar_fire_last = false;
    bool     resp_valid_s  = false;
    uint64_t resp_spaddr_s = 0;
    uint8_t  resp_bank_s   = 0;
    bool     resp_error_s  = false;

    std::deque<Resp> resp_q;

    // A request handshake observed while a response is being awaited, e.g.
    // release_a() -> pop_resp(): latch it so the following wait_accept() does
    // not miss it.
    bool latched_req_fire = false;

    Tb() : top(new Vnonblock_tl_top) {}
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
                         "nonblock_tl_test: unmapped read @0x%016lx\n",
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
    // Sample every handshake/header BEFORE the posedge, then advance the
    // memory models after the edge from that pre-edge snapshot.
    void tick() {
        // 1) Drive TB-controlled inputs from the current model state.
        top->ddtw_ar_ready = ddtw_rd.pending ? 0 : 1;
        top->ptw_ar_ready  = ptw_rd.pending  ? 0 : 1;
        drive_ddtw_r();
        drive_ptw_r();

        // 2) Settle combinational logic.
        top->eval();

        // 3) Sample handshakes/headers BEFORE the edge.
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

        // 4) Clock edge.
        top->clk_i = 1;
        top->eval();

        // 5) Advance the models from the pre-edge snapshot.
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

        // 6) Falling edge.
        top->clk_i = 0;
        top->eval();
    }

    // -- reset -----------------------------------------------------------
    void reset(bool nonblock_enable) {
        top->clk_i = 0;
        top->rst_ni = 0;
        top->nonblock_enable_i = nonblock_enable ? 1 : 0;
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
        resp_q.clear();
        latched_req_fire = false;

        for (int i = 0; i < 8; ++i) tick();
        top->rst_ni = 1;
        tick();
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

    // Settle inputs, take exactly one accept, then deassert valid.
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

    // Wait for a translation response while recording any PTW AR refill that
    // fires after the request has been accepted (the accept always precedes
    // the walk), so a caller can require a real refill before the response.
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
    void start_cold_a_and_hold() {
        set_req(11, 0x20000000ULL, 0x2, 1, true);
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
        // Word order = rv_iommu_ddtw LEAF state (tc, iohgatp, ta, fsc).
        write64(0x10280, 0x0000000000000001ULL); // tc: v=1 only
        write64(0x10288, 0x0000000000000000ULL); // iohgatp: Bare
        write64(0x10290, 0x000000000000A000ULL); // ta: pscid=10 (<<12)
        write64(0x10298, 0x8000000000000020ULL); // fsc: Sv39, ppn=0x20

        write64(0x102C0, 0x0000000000000001ULL); // tc: v=1 only
        write64(0x102C8, 0x0000000000000000ULL); // iohgatp: Bare
        write64(0x102D0, 0x000000000000B000ULL); // ta: pscid=11
        write64(0x102D8, 0x8000000000000030ULL); // fsc: Sv39, ppn=0x30

        // V page tables: root 0x20000, L1V 0x40000, L0V 0x50000.
        // IOVA 0x10000000/0x10001000/0x10002000 -> VPN[1]=0x80 -> entry 0x40400.
        write64(0x20000, 0x0000000000010001ULL); // root V -> L1V (ppn=0x40)
        write64(0x40400, 0x0000000000014001ULL); // L1V -> L0V (ppn=0x50)
        write64(0x50000, 0x00000000200000DFULL); // Vsrc -> phys 0x80000000
        write64(0x50008, 0x00000000200004DFULL); // Vdst -> phys 0x80001000
        write64(0x50010, 0x00000000200000D3ULL); // Vro  -> r=1, w=0, x=0
        write64(0x50018, 0x00000000200008DFULL); // Vcold -> phys 0x80002000 (r/w)

        // A page tables: root 0x30000, L1A 0x60000, L0A 0x70000.
        // IOVA 0x20000000 -> VPN[1]=0x100 -> entry 0x60800.
        write64(0x30000, 0x0000000000018001ULL); // root A -> L1A (ppn=0x60)
        write64(0x60800, 0x000000000001C001ULL); // L1A -> L0A (ppn=0x70)
        write64(0x70000, 0x00000000240000DFULL); // Asrc -> phys 0x90000000
        write64(0x70008, 0x00000000240004DFULL); // Adst -> phys 0x90001000
    }

    // -- warm a leaf through the normal path -----------------------------
    void warm_leaf(uint64_t iova, uint64_t expect_spaddr) {
        set_req(10, iova, 0x2 /*UNTRANSLATED_R*/, 0, true);
        if (!wait_accept()) FAIL_AT("warm_leaf: request was not accepted");
        set_req(10, 0, 0, 0, false);
        Resp r = pop_resp();
        CHECK_AT(!r.error, "warm_leaf: unexpected error response");
        CHECK_AT(r.bank == 0, "warm_leaf: response bank mismatch");
        CHECK_AT(r.spaddr == expect_spaddr,
                 "warm_leaf: unexpected translated address");
    }

    void warm_v() {
        warm_leaf(0x10000000ULL, 0x80000000ULL); // Vsrc (r/w)
        warm_leaf(0x10001000ULL, 0x80001000ULL); // Vdst (r/w)
        warm_leaf(0x10002000ULL, 0x80000000ULL); // Vro  (read-only)
    }

    // -- cases -----------------------------------------------------------
    void case_fast_hit_nonblock_on() {
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("fast-on: V probe was not accepted");
        const bool was_fast = req_fire_fast;
        set_req(10, 0, 0, 0, false);
        CHECK_AT(was_fast, "fast-on: V probe was not accepted by the fast path");

        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "fast-on: fast V response mismatch");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error && ra.spaddr == 0x90000000ULL,
                 "fast-on: A response mismatch after the fast hit");
    }

    void case_blocked_nonblock_off() {
        reset(false);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        bool any = false, fast = false;
        for (int i = 0; i < 200; ++i) {
            tick();
            if (req_fire) { any = true; if (req_fire_fast) fast = true; }
        }
        CHECK_AT(!any, "fast-off: V accepted while A was still in flight");
        CHECK_AT(!fast, "fast-off: fast path fired with nonblock disabled");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error && ra.spaddr == 0x90000000ULL,
                 "fast-off: A response mismatch");

        if (!wait_accept()) FAIL_AT("fast-off: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "fast-off: V response mismatch after fallback");
    }

    void case_invalidation_during_dc_leaf() {
        // ---- invalidation while the fast FSM is in DC ----
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        tick(); // IDLE -> DC (start), no accept yet
        CHECK_AT(!req_fire, "inval/DC: accepted on entry to DC");
        top->iodir_dv      = 1;
        top->iodir_did     = 10;
        top->iodir_inval_ddtc = 1;
        tick(); // DC sees invalidate -> REJECT
        CHECK_AT(!req_fire, "inval/DC: fast accept during DC invalidation");
        top->iodir_inval_ddtc = 0;
        top->iodir_dv         = 0;
        top->iodir_did        = 0;

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error, "inval/DC: A response mismatch");
        if (!wait_accept()) FAIL_AT("inval/DC: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "inval/DC: V response mismatch after fallback");

        // ---- invalidation while the fast FSM is in LEAF ----
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        tick(); // IDLE -> DC
        CHECK_AT(!req_fire, "inval/LEAF: accepted on entry to DC");
        tick(); // DC -> LEAF (no invalidate yet)
        CHECK_AT(!req_fire, "inval/LEAF: accepted while DC");
        top->iodir_dv         = 1;
        top->iodir_did        = 10;
        top->iodir_inval_ddtc = 1;
        tick(); // LEAF sees invalidate -> REJECT
        CHECK_AT(!req_fire, "inval/LEAF: fast accept during LEAF invalidation");
        top->iodir_inval_ddtc = 0;
        top->iodir_dv         = 0;
        top->iodir_did        = 0;

        Resp ra2 = release_a();
        CHECK_AT(ra2.bank == 1 && !ra2.error, "inval/LEAF: A response mismatch");
        if (!wait_accept()) FAIL_AT("inval/LEAF: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        Resp rv2 = pop_resp();
        CHECK_AT(rv2.bank == 0 && !rv2.error && rv2.spaddr == 0x80000000ULL,
                 "inval/LEAF: V response mismatch after fallback");
    }

    void case_permission_write_denied() {
        reset(true);
        warm_v(); // Vro is warmed by a read, so its leaf caches w=0
        start_cold_a_and_hold();

        set_req(10, 0x10002000ULL, 0x3 /*UNTRANSLATED_W*/, 0, true);
        bool any = false, fast = false;
        for (int i = 0; i < 60; ++i) {
            tick();
            if (req_fire) { any = true; if (req_fire_fast) fast = true; }
        }
        CHECK_AT(!fast, "write-denied: read-only leaf write taken by fast path");
        CHECK_AT(!any,  "write-denied: read-only leaf write accepted pre-fallback");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error, "write-denied: A response mismatch");

        if (!wait_accept()) FAIL_AT("write-denied: write never accepted after A");
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0, "write-denied: fault response bank mismatch");
        CHECK_AT(rv.error, "write-denied: expected an error response");
        CHECK_AT(rv.fault_code == 15,
                 "write-denied: expected STORE_PAGE_FAULT (15)");
    }

    void case_ddtc_miss_fallthrough() {
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        // Evict V's device context so the fast DC lookup misses.
        top->iodir_dv         = 1;
        top->iodir_did        = 10;
        top->iodir_inval_ddtc = 1;
        tick();
        tick();
        top->iodir_inval_ddtc = 0;
        top->iodir_dv         = 0;
        top->iodir_did        = 0;

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        bool any = false, fast = false;
        for (int i = 0; i < 60; ++i) {
            tick();
            if (req_fire) { any = true; if (req_fire_fast) fast = true; }
        }
        CHECK_AT(!fast, "ddtc-miss: fast path accepted despite a DDTC miss");
        CHECK_AT(!any,  "ddtc-miss: request accepted before fallback");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error, "ddtc-miss: A response mismatch");

        if (!wait_accept()) FAIL_AT("ddtc-miss: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "ddtc-miss: V response mismatch after normal-path DDTW");
    }

    void case_fast_write_vdst_while_a_held() {
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10001000ULL, 0x3 /*UNTRANSLATED_W*/, 0, true);
        if (!wait_accept()) FAIL_AT("fast-wr: Vdst write was not accepted");
        CHECK_AT(req_fire_fast, "fast-wr: Vdst write was not accepted by fast path");
        set_req(10, 0, 0, 0, false);

        Resp rv = pop_resp();
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80001000ULL,
                 "fast-wr: fast write response mismatch");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error && ra.spaddr == 0x90000000ULL,
                 "fast-wr: A response mismatch after fast write hit");
        CHECK_AT(resp_q.empty(), "fast-wr: unexpected duplicate response");
    }

    void case_invalidation_vma_during_leaf() {
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        tick(); // IDLE -> DC
        CHECK_AT(!req_fire, "inval/VMA: accepted on entry to DC");
        tick(); // DC -> LEAF
        CHECK_AT(!req_fire, "inval/VMA: accepted while DC");
        top->inval_vma = 1;
        top->inval_gv  = 0; // S1-only entry: gv=0,av=0,pscv=0 invalidates host S1 entries
        tick(); // LEAF sees VMA invalidation -> REJECT
        CHECK_AT(!req_fire, "inval/VMA: fast accept during LEAF VMA invalidation");
        CHECK_AT(!req_fire_fast, "inval/VMA: fast path fired during LEAF VMA invalidation");
        top->inval_vma = 0;
        top->inval_gv  = 0;

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error, "inval/VMA: A response mismatch");

        if (!wait_accept()) FAIL_AT("inval/VMA: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        bool saw_ptw = false;
        Resp rv = pop_resp_track_ptw(saw_ptw);
        CHECK_AT(saw_ptw, "inval/VMA: normal path did not perform a PTW refill");
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                 "inval/VMA: V response mismatch after refill");
        CHECK_AT(resp_q.empty(), "inval/VMA: unexpected duplicate response");
    }

    void case_unwarmed_v_fallthrough() {
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        set_req(10, 0x10003000ULL, 0x2, 0, true);
        bool any = false, fast = false;
        for (int i = 0; i < 60; ++i) {
            tick();
            if (req_fire) { any = true; if (req_fire_fast) fast = true; }
        }
        CHECK_AT(!fast, "unwarmed: fast path accepted a cold IOTLB entry");
        CHECK_AT(!any, "unwarmed: V accepted before A finished");

        Resp ra = release_a();
        CHECK_AT(ra.bank == 1 && !ra.error, "unwarmed: A response mismatch");

        if (!wait_accept()) FAIL_AT("unwarmed: V never accepted after fallback");
        set_req(10, 0, 0, 0, false);
        bool saw_ptw = false;
        Resp rv = pop_resp_track_ptw(saw_ptw);
        CHECK_AT(saw_ptw, "unwarmed: normal path did not perform a PTW refill");
        CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80002000ULL,
                 "unwarmed: V response mismatch after normal-path PTW");
        CHECK_AT(resp_q.empty(), "unwarmed: unexpected duplicate response");
    }

    void case_response_backpressure() {
        reset(true);
        warm_v();
        start_cold_a_and_hold();

        top->trans_resp_ready_i = 0;

        set_req(10, 0x10000000ULL, 0x2, 0, true);
        if (!wait_accept()) FAIL_AT("backpressure: fast probe was not accepted");
        CHECK_AT(req_fire_fast, "backpressure: accept was not a fast-path fire");
        CHECK_AT(req_busy_s, "backpressure: nonblock_busy_o should be high");
        set_req(10, 0, 0, 0, false);

        // Let A finish while the fast response is held at the stalled sink.
        release_a_raw();

        bool have = false;
        Resp held;
        int held_cycles = 0;
        for (int i = 0; i < 800; ++i) {
            tick();
            if (resp_valid_s) {
                if (!have) {
                    have = true;
                    held.spaddr = resp_spaddr_s;
                    held.bank   = resp_bank_s;
                    held.error  = resp_error_s;
                } else {
                    CHECK_AT(resp_spaddr_s == held.spaddr &&
                             resp_bank_s == held.bank,
                             "backpressure: response changed while stalled");
                }
                held_cycles++;
            }
        }
        CHECK_AT(have, "backpressure: no response presented");
        CHECK_AT(held_cycles > 100, "backpressure: response was not actually held");

        // Release the sink and drain exactly the two expected responses.
        top->trans_resp_ready_i = 1;
        std::vector<Resp> got;
        for (int i = 0; i < 20000 && got.size() < 2; ++i) {
            tick();
            while (!resp_q.empty()) {
                got.push_back(resp_q.front());
                resp_q.pop_front();
            }
        }
        CHECK_AT(got.size() == 2, "backpressure: expected exactly two responses");

        int fast_cnt = 0, slow_cnt = 0;
        for (size_t i = 0; i < got.size(); ++i) {
            if (got[i].bank == 0 && got[i].spaddr == 0x80000000ULL &&
                !got[i].error)
                fast_cnt++;
            if (got[i].bank == 1 && got[i].spaddr == 0x90000000ULL &&
                !got[i].error)
                slow_cnt++;
        }
        CHECK_AT(fast_cnt == 1, "backpressure: fast response count != 1");
        CHECK_AT(slow_cnt == 1, "backpressure: A response count != 1");
        for (int i = 0; i < 20; ++i) tick();
        CHECK_AT(resp_q.empty(), "backpressure: response queue not empty after drain");
        CHECK_AT(!top->nonblock_busy_o, "backpressure: nonblock_busy_o still high after drain");
    }
};

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Tb tb;
    tb.setup_memory();

    tb.case_fast_hit_nonblock_on();
    tb.case_blocked_nonblock_off();
    tb.case_invalidation_during_dc_leaf();
    tb.case_permission_write_denied();
    tb.case_ddtc_miss_fallthrough();
    tb.case_fast_write_vdst_while_a_held();
    tb.case_invalidation_vma_during_leaf();
    tb.case_unwarmed_v_fallthrough();
    tb.case_response_backpressure();

    std::printf("nonblock_tl_test: PASS\n");
    return 0;
}
