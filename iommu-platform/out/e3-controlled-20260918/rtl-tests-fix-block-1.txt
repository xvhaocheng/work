#include "Vidma_controlled_pair_top.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <cstdlib>

static constexpr int TIMEOUT = 2000;

static void fail(const char *msg) {
    std::fprintf(stderr, "FAIL: %s\n", msg);
    std::abort();
}

struct Tb {
    Vidma_controlled_pair_top *d;

    Tb() {
        d = new Vidma_controlled_pair_top;
        reset();
    }

    ~Tb() { delete d; }

    void tick() {
        d->clk_i = 0; d->eval();
        d->clk_i = 1; d->eval();
        d->clk_i = 0; d->eval();
    }

    void reset() {
        d->rst_ni = 0;
        d->v_cpu_valid_i = 0; d->v_cpu_write_i = 0; d->v_cpu_addr_i = 0;
        d->v_cpu_wdata_i = 0; d->v_cpu_wstrb_i = 0xff;
        d->a_cpu_valid_i = 0; d->a_cpu_write_i = 0; d->a_cpu_addr_i = 0;
        d->a_cpu_wdata_i = 0; d->a_cpu_wstrb_i = 0xff;
        d->v_backend_ready_i = 0; d->v_backend_idle_i = 1; d->v_trans_complete_i = 0;
        d->a_backend_ready_i = 0; d->a_backend_idle_i = 1; d->a_trans_complete_i = 0;
        d->selected_i = 0; d->select_write_i = 0;
        d->select_id_i = 0; d->select_did_i = 0; d->select_iova_i = 0;
        tick();
        d->rst_ni = 1;
        tick();
    }

    bool cpu_ready(bool is_a) const {
        return is_a ? d->a_cpu_ready_o : d->v_cpu_ready_o;
    }

    bool cpu_error(bool is_a) const {
        return is_a ? d->a_cpu_error_o : d->v_cpu_error_o;
    }

    uint64_t cpu_rdata(bool is_a) const {
        return is_a ? d->a_cpu_rdata_o : d->v_cpu_rdata_o;
    }

    void wait_cpu_ready(bool is_a) {
        int guard = 0;
        while (!cpu_ready(is_a)) {
            if (++guard > TIMEOUT) fail("CPU ready timeout");
            tick();
        }
    }

    // Drive -> eval -> wait ready -> check error -> exactly one accept posedge
    // -> deassert valid -> eval.
    void cpu_write(bool is_a, uint32_t addr, uint64_t data, uint8_t strb = 0xff) {
        if (is_a) {
            d->a_cpu_valid_i = 1; d->a_cpu_write_i = 1;
            d->a_cpu_addr_i = addr; d->a_cpu_wdata_i = data; d->a_cpu_wstrb_i = strb;
        } else {
            d->v_cpu_valid_i = 1; d->v_cpu_write_i = 1;
            d->v_cpu_addr_i = addr; d->v_cpu_wdata_i = data; d->v_cpu_wstrb_i = strb;
        }
        d->eval();
        wait_cpu_ready(is_a);
        if (cpu_error(is_a)) fail("CPU write error");
        tick();
        if (is_a) {
            d->a_cpu_valid_i = 0; d->a_cpu_write_i = 0;
        } else {
            d->v_cpu_valid_i = 0; d->v_cpu_write_i = 0;
        }
        d->eval();
    }

    uint64_t cpu_read(bool is_a, uint32_t addr) {
        if (is_a) {
            d->a_cpu_valid_i = 1; d->a_cpu_write_i = 0; d->a_cpu_addr_i = addr;
        } else {
            d->v_cpu_valid_i = 1; d->v_cpu_write_i = 0; d->v_cpu_addr_i = addr;
        }
        d->eval();
        wait_cpu_ready(is_a);
        if (cpu_error(is_a)) fail("CPU read error");
        uint64_t v = cpu_rdata(is_a);
        tick();
        if (is_a) {
            d->a_cpu_valid_i = 0;
        } else {
            d->v_cpu_valid_i = 0;
        }
        d->eval();
        return v;
    }

    void launcher_write(uint8_t addr, uint64_t data) {
        cpu_write(false, addr, data);
    }

    uint64_t launcher_read(uint8_t addr) {
        return cpu_read(false, addr);
    }

    uint64_t launcher_index(uint8_t idx) {
        launcher_write(0xc0, idx);
        return launcher_read(0xc8);
    }

    void configure(bool is_a, uint64_t src, uint64_t dst) {
        cpu_write(is_a, 0x00, src);
        cpu_write(is_a, 0x08, dst);
        cpu_write(is_a, 0x10, 64);
        cpu_write(is_a, 0x18, 0);
    }

    uint64_t reserve(bool is_a) {
        return cpu_read(is_a, 0x28);
    }

    // Set backend ready, wait for valid, then exactly one accepting edge.
    void backend_accept(bool is_a) {
        if (is_a) {
            d->a_backend_ready_i = 1;
            d->eval();
            int guard = 0;
            while (!d->a_valid_o) {
                if (++guard > TIMEOUT) fail("A valid timeout");
                tick();
            }
            tick();
            d->a_backend_ready_i = 0;
            d->eval();
        } else {
            d->v_backend_ready_i = 1;
            d->eval();
            int guard = 0;
            while (!d->v_valid_o) {
                if (++guard > TIMEOUT) fail("V valid timeout");
                tick();
            }
            tick();
            d->v_backend_ready_i = 0;
            d->eval();
        }
    }

    void complete(bool is_a) {
        if (is_a) {
            d->a_trans_complete_i = 1;
            tick();
            d->a_trans_complete_i = 0;
            d->eval();
        } else {
            d->v_trans_complete_i = 1;
            tick();
            d->v_trans_complete_i = 0;
            d->eval();
        }
    }

    void select_a_read(uint8_t id, uint32_t did, uint64_t iova = 0) {
        d->selected_i = 1;
        d->select_write_i = 0;
        d->select_id_i = id;
        d->select_did_i = did;
        d->select_iova_i = iova;
        d->eval();
        tick();
        d->selected_i = 0;
        d->eval();
    }
};

static void test_basic_pair_flow() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);

    assert(t.launcher_read(0xb0) == 0x49444d414c4e4301ULL);

    t.launcher_write(0xb8, 1 | 0x100);
    uint64_t v_id = t.reserve(false);
    uint64_t a_id = t.reserve(true);
    // Independent ID generators may both be 2.  Do not assume global uniqueness.
    assert(v_id != 0 && a_id != 0);
    assert(t.d->frontend_held_o == 3);

    t.launcher_write(0xb8, 2); // GO -> A first
    assert(t.d->launch_go_o == 2);
    t.tick();
    assert(t.d->launch_go_o == 0);

    t.backend_accept(true);
    assert((t.launcher_read(0xb8) & 4) == 0); // not frozen yet

    // Valid A read select only after actual A issue.
    t.select_a_read(4, 11, 0x3000);
    assert(t.d->launch_go_o == 1); // V released
    t.tick();
    assert(t.d->launch_go_o == 0);

    t.backend_accept(false);

    // V done before A.
    t.complete(false);
    t.complete(true);
    t.tick();

    uint64_t status = t.launcher_read(0xb8);
    assert((status & 4) == 4);       // FROZEN
    assert((status >> 32) == 0);     // no errors

    assert(t.launcher_index(1) == 0);   // error_q
    assert(t.launcher_index(15) == 3);  // issued_mask
    assert(t.launcher_index(16) == 3);  // done_mask
    assert(t.launcher_index(17) == 0);  // cancelled_mask
    assert(t.launcher_index(24) == 1);  // issue_count V
    assert(t.launcher_index(25) == 1);  // issue_count A
    assert(t.launcher_index(26) == 1);  // done_count V
    assert(t.launcher_index(27) == 1);  // done_count A
    assert(t.launcher_index(11) == v_id);
    assert(t.launcher_index(12) == a_id);
    assert(t.launcher_index(13) == v_id);
    assert(t.launcher_index(14) == a_id);
    assert(t.launcher_index(20) == 4);
    assert(t.launcher_index(21) == 11);
    assert(t.launcher_index(4) > 0);    // go_a_ts
    assert(t.launcher_index(6) > 0);    // go_v_ts
    assert(t.launcher_index(7) > 0);    // issue_ts V
    assert(t.launcher_index(8) > 0);    // issue_ts A
    assert(t.launcher_index(9) > 0);    // done_ts V
    assert(t.launcher_index(10) > 0);   // done_ts A
}

static void test_go_then_immediate_abort() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);
    t.launcher_write(0xb8, 1 | 0x100);
    t.reserve(false);
    t.reserve(true);
    t.launcher_write(0xb8, 2);
    assert(t.d->launch_go_o == 2);

    // Abort before frontend samples registered GO.
    t.d->v_cpu_valid_i = 1; t.d->v_cpu_write_i = 1;
    t.d->v_cpu_addr_i = 0xb8; t.d->v_cpu_wdata_i = 4; t.d->v_cpu_wstrb_i = 0xff;
    t.d->eval();
    assert(t.d->launch_go_o == 0);
    assert(t.d->launch_cancel_o == 3);
    t.tick();
    t.d->v_cpu_valid_i = 0; t.d->v_cpu_write_i = 0;
    t.d->eval();
    t.tick();

    assert(t.d->v_valid_o == 0 && t.d->a_valid_o == 0);

    uint64_t status = t.launcher_read(0xb8);
    assert((status & 8) == 8);       // aborted
    assert((status & 4) == 4);       // FROZEN
    assert((status >> 32) == 0);     // no errors
    assert(t.launcher_index(15) == 0); // issued_mask
    assert(t.launcher_index(16) == 0); // done_mask
    assert(t.launcher_index(17) == 3); // both cancelled
}

static void test_count_backpressure_abort() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.launcher_write(0xb8, 1); // PREP V only
    uint64_t v_id = t.reserve(false);
    assert(v_id != 0);

    t.launcher_write(0xb8, 2); // GO
    assert(t.d->launch_go_o == 1);
    t.tick();
    assert(t.d->launch_go_o == 0);

    // Backend not ready: VALID must persist.
    t.d->v_backend_ready_i = 0;
    assert(t.d->v_valid_o == 1);

    t.launcher_write(0xb8, 4); // ABORT while COUNT, ready 0
    assert((t.d->launch_cancel_o & 1) == 0); // no cancel
    t.tick();
    assert(t.d->v_valid_o == 1); // still offered

    // Actual issue/drain, no cancel.
    t.backend_accept(false);
    t.complete(false);
    t.tick();

    uint64_t status = t.launcher_read(0xb8);
    assert((status & 8) == 8);       // aborted
    assert((status & 4) == 4);       // FROZEN
    assert((status >> 32) == 0);     // no errors
    assert(t.launcher_index(15) == 1);
    assert(t.launcher_index(16) == 1);
    assert(t.launcher_index(17) == 0);
    assert(t.launcher_index(11) == v_id);
    assert(t.launcher_index(13) == v_id);
    assert(t.launcher_index(24) == 1);
    assert(t.launcher_index(26) == 1);
}

static void test_partial_reserve_abort() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);
    t.launcher_write(0xb8, 1 | 0x100);
    t.reserve(false);
    assert(t.d->frontend_held_o == 1);

    t.launcher_write(0xb8, 4);
    t.tick();

    uint64_t status = t.launcher_read(0xb8);
    assert((status & 8) == 8);
    assert((status & 4) == 4);
    assert((status >> 32) == 0);
    assert(t.launcher_index(15) == 0);
    assert(t.launcher_index(16) == 0);
    assert(t.launcher_index(17) == 1); // only V cancelled
}

static void test_new_prep_after_aborted_terminal() {
    Tb t;
    t.launcher_write(0xb8, 4); // ABORT from OFF
    t.tick();
    uint64_t status = t.launcher_read(0xb8);
    assert((status & 8) == 8);
    assert((status & 4) == 4);

    t.launcher_write(0xb8, 1); // PREP
    status = t.launcher_read(0xb8);
    assert((status & 1) == 1); // PREP
    assert((status & 8) == 0); // aborted cleared
    assert((status >> 32) == 0);
}

static void test_v_release_only_valid_a_select() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);
    t.launcher_write(0xb8, 1 | 0x100);
    t.reserve(false);
    t.reserve(true);

    t.launcher_write(0xb8, 2); // GO A first
    assert(t.d->launch_go_o == 2);
    t.tick();
    assert(t.d->launch_go_o == 0);
    t.backend_accept(true); // A issued

    // Invalid selects.
    t.d->selected_i = 1; t.d->select_write_i = 1;
    t.d->select_id_i = 4; t.d->select_did_i = 11;
    t.d->eval();
    t.tick();
    assert(t.d->launch_go_o == 0);

    t.d->select_write_i = 0; t.d->select_id_i = 5;
    t.d->eval();
    t.tick();
    assert(t.d->launch_go_o == 0);

    t.d->select_id_i = 4; t.d->select_did_i = 12;
    t.d->eval();
    t.tick();
    assert(t.d->launch_go_o == 0);

    // Valid read select.
    t.d->select_did_i = 11;
    t.d->eval();
    t.tick();
    assert(t.d->launch_go_o == 1);

    t.d->selected_i = 0; t.d->eval();
    assert((t.launcher_read(0xb8) >> 32) == 0);
}

static void test_both_done_late_a() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);
    t.launcher_write(0xb8, 1 | 0x100);
    t.reserve(false);
    t.reserve(true);

    t.launcher_write(0xb8, 2);
    assert(t.d->launch_go_o == 2);
    t.tick();
    assert(t.d->launch_go_o == 0);
    t.backend_accept(true);
    t.select_a_read(4, 11);
    assert(t.d->launch_go_o == 1);
    t.tick();
    assert(t.d->launch_go_o == 0);
    t.backend_accept(false);

    // V done first.
    t.complete(false);
    uint64_t status = t.launcher_read(0xb8);
    assert((status & 4) == 0); // not FROZEN yet
    assert((status >> 32) == 0);
    assert(t.launcher_index(16) == 1); // only V done

    // A done late.
    t.complete(true);
    t.tick();
    status = t.launcher_read(0xb8);
    assert((status & 4) == 4); // FROZEN
    assert((status >> 32) == 0);
    assert(t.launcher_index(16) == 3);
}

static void test_per_lane_ids_after_cancel_and_normal() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);

    // Two V-only reserve+cancel cycles advance V's private ID generator only.
    t.launcher_write(0xb8, 1); // PREP V only
    uint64_t v1 = t.reserve(false);
    assert(v1 != 0);
    t.launcher_write(0xb8, 4); // ABORT cancels V in HOLD
    t.tick();
    assert((t.launcher_read(0xb8) & 4) == 4); // FROZEN

    t.launcher_write(0xb8, 1); // PREP V only
    uint64_t v2 = t.reserve(false);
    assert(v2 != 0 && v2 != v1);
    t.launcher_write(0xb8, 4); // ABORT
    t.tick();
    assert((t.launcher_read(0xb8) & 4) == 4);

    // Now both lanes.  A's private generator is still at its initial value.
    t.launcher_write(0xb8, 1 | 0x100);
    uint64_t v3 = t.reserve(false);
    uint64_t a1 = t.reserve(true);
    assert(v3 != 0 && a1 != 0);
    assert(v3 > v2); // V advanced by prior cancels
    assert(a1 == 2); // A untouched, initial ID

    t.launcher_write(0xb8, 2); // GO A first
    assert(t.d->launch_go_o == 2);
    t.tick();
    assert(t.d->launch_go_o == 0);
    t.backend_accept(true);
    t.select_a_read(4, 11, 0x3000);
    assert(t.d->launch_go_o == 1);
    t.tick();
    assert(t.d->launch_go_o == 0);
    t.backend_accept(false);

    t.complete(true);
    t.complete(false);
    t.tick();

    uint64_t status = t.launcher_read(0xb8);
    assert((status & 4) == 4);
    assert((status >> 32) == 0);
    assert(t.launcher_index(11) == v3);
    assert(t.launcher_index(12) == a1);
    assert(t.launcher_index(13) == v3);
    assert(t.launcher_index(14) == a1);
    assert(t.launcher_index(16) == 3);
    assert(t.launcher_index(17) == 0);
    assert(t.launcher_index(24) == 1);
    assert(t.launcher_index(25) == 1);
    assert(t.launcher_index(26) == 1);
    assert(t.launcher_index(27) == 1);
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    test_basic_pair_flow();
    test_go_then_immediate_abort();
    test_count_backpressure_abort();
    test_partial_reserve_abort();
    test_new_prep_after_aborted_terminal();
    test_v_release_only_valid_a_select();
    test_both_done_late_a();
    test_per_lane_ids_after_cancel_and_normal();
    std::puts("PASS idma_controlled_pair");
    return 0;
}
