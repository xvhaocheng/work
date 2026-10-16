// Directed integration test for production dma_research_policy with the real
// idma_reg64_frontend ResearchPort path. Simulation only; no board claim.
#include "Vidma_research_policy_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

static void ck(bool b, const char *s) {
    if (!b) throw std::runtime_error(s);
}

static constexpr uint16_t P_MAGIC    = 0xE00;
static constexpr uint16_t P_VERSION  = 0xE01;
static constexpr uint16_t P_ERROR    = 0xE02;
static constexpr uint16_t P_STATUS   = 0xE03;
static constexpr uint16_t P_COMMAND  = 0xE04;
static constexpr uint16_t P_MODE     = 0xE05;
static constexpr uint16_t P_WINDOW   = 0xE06;
static constexpr uint16_t P_TARGET   = 0xE07;
static constexpr uint16_t P_DEADBAND = 0xE08;
static constexpr uint16_t P_INITBUD  = 0xE09;
static constexpr uint16_t P_MINBUD   = 0xE0A;
static constexpr uint16_t P_MAXBUD   = 0xE0B;
static constexpr uint16_t P_STEP     = 0xE0C;
static constexpr uint16_t P_CAP      = 0xE0D;
static constexpr uint16_t P_WHIT     = 0xE0E;
static constexpr uint16_t P_WMISS    = 0xE0F;
static constexpr uint16_t P_WPTW     = 0xE10;
static constexpr uint16_t P_ST_WIN     = 0xE20;
static constexpr uint16_t P_ST_VDONE   = 0xE21;
static constexpr uint16_t P_ST_VBYTES  = 0xE22;
static constexpr uint16_t P_ST_SHORTW  = 0xE23;
static constexpr uint16_t P_ST_SHORTB  = 0xE24;
static constexpr uint16_t P_ST_PERMCLOSE = 0xE25;
static constexpr uint16_t P_ST_CHARGE  = 0xE26;
static constexpr uint16_t P_ST_REPLEN  = 0xE27;
static constexpr uint16_t P_ST_AISSUE  = 0xE28;
static constexpr uint16_t P_ST_ADONE   = 0xE29;
static constexpr uint16_t P_ST_CHARGECYC = 0xE2A;
static constexpr uint16_t P_ST_HIT     = 0xE2B;
static constexpr uint16_t P_ST_MISS    = 0xE2C;
static constexpr uint16_t P_ST_PTW     = 0xE2D;
static constexpr uint16_t P_ST_AOWN    = 0xE2E;
static constexpr uint16_t P_ST_SEL     = 0xE2F;
static constexpr uint16_t P_ST_LUTA    = 0xE30;
static constexpr uint16_t P_ST_LUTAMISS= 0xE31;
static constexpr uint16_t P_ST_ACTUALCYC = 0xE32;
static constexpr uint16_t P_ST_WINALL  = 0xE33;
static constexpr uint16_t P_ST_WINPART = 0xE34;
static constexpr uint16_t P_ST_FBUP    = 0xE35;
static constexpr uint16_t P_ST_FBDN    = 0xE36;
static constexpr uint16_t P_ST_GAPL    = 0xE37;
static constexpr uint16_t P_ST_STALE   = 0xE38;
static constexpr uint16_t P_ST_ARMEDCYC= 0xE39;
static constexpr uint16_t P_BALANCE    = 0xE3A;
static constexpr uint16_t P_BMIN       = 0xE3B;
static constexpr uint16_t P_BMAX       = 0xE3C;
static constexpr uint16_t P_BFINAL     = 0xE3D;
static constexpr uint16_t P_NODONE     = 0xE3E;

struct Bench {
    Vidma_research_policy_top d;
    uint64_t cycle = 0;
    bool valid = false, write = false;
    uint8_t addr = 0;
    uint64_t wdata = 0, wstrb = 0xff;
    bool ready = true;
    bool due_valid = false;
    uint64_t due = 0;
    bool backend_idle_override = false;
    bool backend_idle_value = false;
    bool p_cfg_valid = false, p_cfg_write = false;
    uint16_t p_index = 0;
    uint64_t p_wdata = 0;
    bool p_quiet = true, p_supported = true;
    bool p_v_active = false, p_v_done = false;
    uint8_t p_ctl_state = 0;
    uint32_t p_ctl_did = 0;
    bool p_select_fire = false;
    uint32_t p_select_did = 0;
    bool p_lookup = false, p_lookup_hit = false;
    uint32_t p_lookup_did = 0;
    bool p_ptw_r_fire = false, p_response_error = false;
    bool batch_reverse = false;
    unsigned completions = 0;
    uint64_t last_done_id = 0;
    bool have_done = false;
    std::vector<uint64_t> srcs;
    std::vector<uint64_t> dsts;

    void eval() {
        d.valid_i = valid;
        d.write_i = write;
        d.addr_i = addr;
        d.wdata_i = wdata;
        d.wstrb_i = wstrb;
        d.ready_i = ready;
        d.complete_i = due_valid && (cycle == due);
        d.backend_idle_i = backend_idle_override ? backend_idle_value : !due_valid;
        d.batch_reverse_i = batch_reverse;
        d.p_cfg_valid_i = p_cfg_valid;
        d.p_cfg_write_i = p_cfg_write;
        d.p_index_i = p_index;
        d.p_wdata_i = p_wdata;
        d.p_quiet_i = p_quiet;
        d.p_supported_i = p_supported;
        d.p_v_active_i = p_v_active;
        d.p_v_done_i = p_v_done;
        d.p_ctl_state_i = p_ctl_state;
        d.p_ctl_did_i = p_ctl_did;
        d.p_select_fire_i = p_select_fire;
        d.p_select_did_i = p_select_did;
        d.p_lookup_i = p_lookup;
        d.p_lookup_hit_i = p_lookup_hit;
        d.p_lookup_did_i = p_lookup_did;
        d.p_ptw_r_fire_i = p_ptw_r_fire;
        d.p_response_error_i = p_response_error;
        d.eval();
    }

    void tick() {
        eval();
        if (d.rst_ni) {
            if (d.request_o && d.ready_i) {
                ck(!due_valid, "one outstanding task");
                srcs.push_back(d.src_o);
                dsts.push_back(d.dst_o);
                due_valid = true;
                due = cycle + 3;
            }
        }
        if (d.rst_ni && d.complete_i) {
            ck(d.done_o, "batch done on completion");
            completions++;
            last_done_id = d.done_id_o;
            have_done = true;
            due_valid = false;
        }
        // Keep pre-edge stimulus stable through the active clock edge.
        d.clk_i = 1; d.eval();
        cycle++;
        d.clk_i = 0; eval();
    }

    void reset() {
        d.rst_ni = 0;
        valid = false; write = false; addr = 0; wdata = 0; wstrb = 0xff;
        ready = true; due_valid = false; due = 0;
        backend_idle_override = false; backend_idle_value = false;
        p_cfg_valid = false; p_cfg_write = false; p_index = 0; p_wdata = 0;
        p_quiet = true; p_supported = true; p_v_active = false; p_v_done = false;
        p_ctl_state = 0; p_ctl_did = 0;
        p_select_fire = false; p_select_did = 0;
        p_lookup = false; p_lookup_hit = false; p_lookup_did = 0;
        p_ptw_r_fire = false; p_response_error = false;
        batch_reverse = false;
        completions = 0; last_done_id = 0; have_done = false; srcs.clear(); dsts.clear();
        for (int i = 0; i < 3; i++) { eval(); d.clk_i = 1; eval(); d.clk_i = 0; eval(); }
        d.rst_ni = 1; eval(); d.clk_i = 1; eval(); d.clk_i = 0; eval();
    }

    Bench() { reset(); }

    uint64_t access(unsigned a, bool wr = false, uint64_t val = 0, uint8_t strb = 0xff) {
        valid = true; write = wr; addr = (uint8_t)a; wdata = val; wstrb = strb;
        eval();
        ck(d.ready_o, "frontend MMIO ready");
        uint64_t r = d.rdata_o;
        tick();
        valid = false; wstrb = 0xff; eval();
        return r;
    }

    uint64_t p_access(uint16_t idx, bool wr = false, uint64_t val = 0) {
        p_cfg_valid = true; p_cfg_write = wr; p_index = idx; p_wdata = val;
        eval();
        uint64_t r = d.p_rdata_o;
        tick();
        p_cfg_valid = false; p_cfg_write = false; eval();
        return r;
    }

    // Sample the combinational read mux without silently advancing the test clock.
    uint64_t p_read(uint16_t idx) {
        p_index = idx; p_cfg_valid = true; p_cfg_write = false; eval();
        uint64_t value = d.p_rdata_o;
        p_cfg_valid = false; eval();
        return value;
    }
    void p_write(uint16_t idx, uint64_t val) { (void)p_access(idx, true, val); }

    void p_configure_mode(uint32_t mode, uint32_t window, uint32_t target, uint32_t deadband,
                          uint32_t init, uint32_t minb, uint32_t maxb, uint32_t step, uint32_t cap,
                          uint32_t whit = 1, uint32_t wmiss = 100, uint32_t wptw = 100) {
        p_write(P_MODE, mode);
        p_write(P_WINDOW, window);
        p_write(P_TARGET, target);
        p_write(P_DEADBAND, deadband);
        p_write(P_INITBUD, init);
        p_write(P_MINBUD, minb);
        p_write(P_MAXBUD, maxb);
        p_write(P_STEP, step);
        p_write(P_CAP, cap);
        p_write(P_WHIT, whit);
        p_write(P_WMISS, wmiss);
        p_write(P_WPTW, wptw);
    }

    void arm_policy() {
        p_write(P_COMMAND, 1);
        ck((p_read(P_STATUS) & 1) == 1, "policy armed");
    }

    void configure_frontend(uint32_t target, unsigned pages, bool reverse = false, uint32_t gap = 0) {
        batch_reverse = reverse;
        access(0x00, true, 0x10000);
        access(0x08, true, 0x100000);
        access(0x10, true, 4096);
        access(0x18, true, 0);
        if (gap) access(0xD0, true, gap);
        access(0x88, true, 0);
        access(0x90, true, target);
        access(0x98, true, pages);
        access(0x88, true, 1);
    }

    void start_frontend() {
        uint64_t id = access(0x28);
        ck(id == 2, "one functional ID per batch");
    }

    void run_until(unsigned n, unsigned limit = 200000) {
        for (unsigned i = 0; i < limit && completions < n; i++) tick();
        ck(completions == n, "bounded completion count");
    }
};

static void t_first_admit() {
    Bench b;
    b.p_configure_mode(0, 64, 51540, 4096, 8000, 1, 32768, 512, 65536);
    b.arm_policy();
    b.configure_frontend(2, 1);
    b.backend_idle_override = true;
    b.backend_idle_value = false;
    b.start_frontend();
    for (int i = 0; i < 5; i++) { b.tick(); ck(!b.d.request_o, "no VALID in ADMIT before backend idle"); }
    ck(b.d.policy_enable_o, "policy enable armed");
    b.backend_idle_override = false;
    b.tick();
    ck(b.d.request_o, "VALID after backend idle and allow");
}

static void t_bytes_negative_refill() {
    Bench b;
    b.p_configure_mode(1, 64, 51540, 4096, 8192, 1, 32768, 512, 65536);
    b.arm_policy();
    b.configure_frontend(4, 1);
    b.start_frontend();
    b.run_until(3);
    for (int i = 0; i < 5; i++) { b.tick(); ck(!b.d.request_o, "bytes negative blocks next"); }
    ck(b.p_read(P_BALANCE) == 0xFFFFFFFFFFFFF000ULL, "balance is -4096");
    unsigned guard = 0;
    while (b.p_read(P_BALANCE) == 0xFFFFFFFFFFFFF000ULL && guard++ < 200) b.tick();
    ck(b.p_read(P_BALANCE) == 4096, "V-inactive window refill recovered balance");
    ck(b.p_read(P_ST_REPLEN) == 8192, "replenish counted");
    for (int i = 0; i < 10 && !b.d.request_o; i++) b.tick();
    ck(b.d.request_o, "request after refill");
}

static void t_actual_owner() {
    Bench b;
    b.p_configure_mode(5, 1000, 51540, 4096, 100000, 1, 1000000, 512, 1000000);
    b.arm_policy();
    uint64_t c0 = b.p_read(P_ST_CHARGE);
    uint64_t a0 = b.p_read(P_ST_ACTUALCYC);
    uint64_t o0 = b.p_read(P_ST_AOWN);
    b.p_ctl_state = 1; b.p_ctl_did = 11;
    for (int i = 0; i < 7; i++) b.tick();
    b.p_ctl_state = 2;
    for (int i = 0; i < 5; i++) b.tick();
    uint64_t c1 = b.p_read(P_ST_CHARGE);
    uint64_t a1 = b.p_read(P_ST_ACTUALCYC);
    uint64_t o1 = b.p_read(P_ST_AOWN);
    ck(c1 - c0 == 12, "owner11 state1/2 charged");
    ck(a1 - a0 == 12, "owner11 actual cycles");
    ck(o1 - o0 == 12, "owner11 owner cycles");
    b.p_ctl_state = 1; b.p_ctl_did = 16;
    for (int i = 0; i < 9; i++) b.tick();
    ck(b.p_read(P_ST_CHARGE) == c1, "owner16 not charged");
    ck(b.p_read(P_ST_ACTUALCYC) == a1, "owner16 not actual");
    b.p_ctl_state = 0; b.p_ctl_did = 0;
}

static void t_backpressure_abort_count() {
    Bench b;
    b.p_configure_mode(0, 1000, 51540, 4096, 8000, 1, 32768, 512, 65536);
    b.arm_policy();
    b.configure_frontend(2, 1);
    b.start_frontend();
    for (int i = 0; i < 20 && !b.d.request_o; i++) b.tick();
    ck(b.d.request_o, "request offered");
    b.ready = false;
    uint64_t src0 = b.d.src_o, dst0 = b.d.dst_o;
    b.p_write(P_COMMAND, 2);
    ck(b.d.policy_abort_o, "policy abort active");
    for (int i = 0; i < 8; i++) {
        b.eval();
        ck(b.d.request_o, "abort cannot withdraw stalled VALID");
        ck(b.d.src_o == src0 && b.d.dst_o == dst0, "stalled payload stable");
        b.tick();
    }
    b.ready = true;
    b.tick();
    b.run_until(1);
    ck(b.completions == 1, "committed aborted copy completed once");
    for (int i = 0; i < 5; i++) { b.tick(); ck(!b.d.request_o, "abort retires after WAIT completion"); }
}

static void t_abort_in_admit_no_done() {
    Bench b;
    b.p_configure_mode(0, 1000, 51540, 4096, 8000, 1, 32768, 512, 65536);
    b.arm_policy();
    b.configure_frontend(2, 1);
    b.backend_idle_override = true;
    b.backend_idle_value = false;
    b.start_frontend();
    b.tick();
    ck(!b.d.request_o, "ADMIT no VALID");
    b.p_write(P_COMMAND, 2);
    b.tick(); b.tick();
    ck(!b.d.batch_active_o, "ABORT in ADMIT retires");
    ck(b.completions == 0, "no fake done");
}

static void t_mode6_gap() {
    Bench b;
    b.p_configure_mode(6, 1000, 51540, 4096, 5, 0, 4000, 100, 4000);
    b.arm_policy();
    b.configure_frontend(2, 1);
    b.start_frontend();
    b.run_until(1);
    ck(b.p_read(P_ST_GAPL) == 1, "mode6 first completion loads gap");
    int blocked = 0;
    for (int i = 0; i < 8; i++) { b.tick(); if (!b.d.request_o) blocked++; }
    ck(blocked > 0, "mode6 gap blocks next copy");
    b.run_until(2);
    ck(b.p_read(P_ST_GAPL) >= 2, "mode6 second completion loads gap again");
}

static void t_mode0_fixed_gap() {
    Bench b;
    b.p_configure_mode(0, 1000, 51540, 4096, 8000, 1, 32768, 512, 65536);
    b.arm_policy();
    b.configure_frontend(3, 1, false, 3);
    b.start_frontend();
    b.run_until(3);
    ck(b.completions == 3, "mode0 fixed gap completes");
    ck(b.p_read(P_ST_AISSUE) == 3, "A issue count");
    ck(b.p_read(P_ST_ADONE) == 3, "A done count");
}

static void t_partial_v_no_adapt() {
    Bench b;
    b.p_configure_mode(1, 4, 1, 0, 8192, 1, 100000, 512, 200000);
    b.arm_policy();
    uint64_t b0 = b.p_read(P_BFINAL);
    for (int i = 0; i < 4; i++) {
        b.p_v_active = (i == 0);
        b.tick();
    }
    b.p_v_active = false;
    uint64_t b1 = b.p_read(P_BFINAL);
    ck(b1 == b0, "partial V window does not change b");
    ck(b.p_read(P_ST_FBUP) == 0 && b.p_read(P_ST_FBDN) == 0, "no feedback on partial");
    ck(b.p_read(P_ST_WINPART) >= 1, "partial window counted");
    ck(b.p_read(P_ST_WIN) == 0, "no complete window");
}

static void t_count_actual_drain() {
    int modes[2] = {2, 5};
    for (int mi = 0; mi < 2; mi++) {
        int mode = modes[mi];
        Bench b;
        b.p_configure_mode((uint32_t)mode, 1000, 51540, 4096, 100000, 1, 1000000, 1, 1000000);
        b.arm_policy();
        b.configure_frontend(3, 1);
        b.start_frontend();
        b.run_until(3);
        ck(b.p_read(P_ST_AISSUE) == 3, "A issue drained");
        ck(b.p_read(P_ST_ADONE) == 3, "A done drained");
        ck(b.p_read(P_ST_AISSUE) == b.p_read(P_ST_ADONE), "count/actual equal work drain");
        ck(!b.d.request_o, "no pending request after drain");
    }
}

static void t_freeze_clear_new_run() {
    Bench b;
    b.p_configure_mode(0, 1000, 51540, 4096, 8000, 1, 32768, 512, 65536);
    b.arm_policy();
    ck(((b.p_read(P_STATUS) >> 0) & 1) == 1, "armed");
    b.p_write(P_COMMAND, 2);
    ck(((b.p_read(P_STATUS) >> 2) & 1) == 1, "abort bit set");
    b.p_write(P_COMMAND, 3);
    ck(((b.p_read(P_STATUS) >> 1) & 1) == 1, "frozen after FREEZE");
    ck(((b.p_read(P_STATUS) >> 2) & 1) == 1, "FREEZE retains abort bit");
    b.p_write(P_COMMAND, 4);
    ck(((b.p_read(P_STATUS) >> 2) & 1) == 0, "CLEAR removes abort bit");
    ck(((b.p_read(P_STATUS) >> 1) & 1) == 0, "CLEAR removes frozen");
    b.arm_policy();
    ck(((b.p_read(P_STATUS) >> 0) & 1) == 1, "re-armed for new run");
    b.configure_frontend(1, 1);
    b.start_frontend();
    b.run_until(1);
    ck(b.completions == 1, "new run completes");
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    try {
        t_first_admit();
        t_bytes_negative_refill();
        t_actual_owner();
        t_backpressure_abort_count();
        t_abort_in_admit_no_done();
        t_mode6_gap();
        t_mode0_fixed_gap();
        t_partial_v_no_adapt();
        t_count_actual_drain();
        t_freeze_clear_new_run();
        std::cout << "PASS idma_research_policy integration scenarios" << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL " << e.what() << std::endl;
        return 1;
    }
}
