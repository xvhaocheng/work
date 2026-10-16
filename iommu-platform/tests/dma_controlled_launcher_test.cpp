#include "Vdma_controlled_launcher_top.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
#include <cstdint>

static constexpr uint64_t MAGIC = 0x49444d414c4e4301ULL;

struct Dut {
    Vdma_controlled_launcher_top *d;

    Dut() {
        d = new Vdma_controlled_launcher_top;
        reset();
    }
    ~Dut() { delete d; }

    void tick() {
        d->clk_i = 0; d->eval();
        d->clk_i = 1; d->eval();
        d->clk_i = 0; d->eval();
    }
    void reset() {
        d->rst_ni = 0;
        d->cfg_valid_i = 0; d->cfg_write_i = 0;
        d->cfg_addr_i = 0; d->cfg_wdata_i = 0; d->cfg_wstrb_i = 0xff;
        d->frontend_held_i = 0; d->frontend_active_i = 0;
        d->frontend_cancelled_i = 0; d->frontend_error_i = 0;
        d->obs_issue_i = 0; d->obs_done_i = 0;
        for (int i = 0; i < 8; i++) {
            d->obs_issue_id_i_flat[i] = 0;
            d->obs_done_id_i_flat[i] = 0;
        }
        d->selected_i = 0; d->select_write_i = 0;
        d->select_id_i = 0; d->select_did_i = 0; d->select_iova_i = 0;
        tick();
        d->rst_ni = 1;
        tick();
    }
    void write(uint8_t addr, uint64_t data, uint8_t strb = 0xff) {
        d->cfg_valid_i = 1; d->cfg_write_i = 1;
        d->cfg_addr_i = addr; d->cfg_wdata_i = data; d->cfg_wstrb_i = strb;
        tick();
        d->cfg_valid_i = 0; d->cfg_write_i = 0;
        d->eval();
    }
    void write_no_tick(uint8_t addr, uint64_t data, uint8_t strb = 0xff) {
        d->cfg_valid_i = 1; d->cfg_write_i = 1;
        d->cfg_addr_i = addr; d->cfg_wdata_i = data; d->cfg_wstrb_i = strb;
        d->eval();
    }
    void end_write() {
        d->cfg_valid_i = 0; d->cfg_write_i = 0;
        d->eval();
    }
    uint64_t read(uint8_t addr) {
        d->cfg_valid_i = 0; d->cfg_write_i = 0;
        d->cfg_addr_i = addr;
        d->eval();
        return d->cfg_rdata_o;
    }
    uint64_t read_index(uint8_t idx) {
        write(0xc0, idx);
        return read(0xc8);
    }
    void set_issue_id(int port, uint64_t id) {
        d->obs_issue_id_i_flat[port*2] = (uint32_t)id;
        d->obs_issue_id_i_flat[port*2+1] = (uint32_t)(id >> 32);
    }
    void set_done_id(int port, uint64_t id) {
        d->obs_done_id_i_flat[port*2] = (uint32_t)id;
        d->obs_done_id_i_flat[port*2+1] = (uint32_t)(id >> 32);
    }
    void issue(int port, uint64_t id) {
        d->obs_issue_i = (1u << port);
        set_issue_id(port, id);
    }
    void done(int port, uint64_t id) {
        d->obs_done_i = (1u << port);
        set_done_id(port, id);
    }
    void clear_events() {
        d->obs_issue_i = 0; d->obs_done_i = 0;
        for (int i = 0; i < 8; i++) {
            d->obs_issue_id_i_flat[i] = 0;
            d->obs_done_id_i_flat[i] = 0;
        }
    }
    void set_held(int port, bool v) {
        if (v) d->frontend_held_i = d->frontend_held_i | (1u << port);
        else d->frontend_held_i = d->frontend_held_i & ~(1u << port);
    }
    void set_active(int port, bool v) {
        if (v) d->frontend_active_i = d->frontend_active_i | (1u << port);
        else d->frontend_active_i = d->frontend_active_i & ~(1u << port);
    }
    uint64_t status() { return read(0xb8); }
    bool error_any() { return (status() >> 32) != 0; }
};

static void test_magic(Dut &d) {
    assert(d.read(0xb0) == MAGIC);
}

static void test_solo_prep_go_done(Dut &d) {
    d.write(0xb8, 1); // PREP solo
    assert((d.status() & 1) == 1); // PREP
    assert(d.d->launch_hold_o == 1);
    d.set_held(0, true);
    d.write(0xb8, 2); // GO
    assert(d.d->launch_go_o == 1);
    d.tick();
    assert(d.d->launch_go_o == 0);
    d.set_held(0, false);
    d.set_active(0, true);
    d.issue(0, 91);
    d.tick();
    d.clear_events();
    d.done(0, 91);
    d.tick();
    d.clear_events();
    d.set_active(0, false);
    d.tick();
    assert((d.status() & 4) == 4); // FROZEN
    assert((d.status() & 8) == 0); // not aborted
    assert(d.read_index(24) == 1); // V issue count
    assert(d.read_index(26) == 1); // V done count
    assert(d.read_index(11) == 91);
    assert(d.read_index(13) == 91);
}

static void test_non_solo_exact_goA_then_goV(Dut &d) {
    d.write(0xb8, 1 | 0x100); // PREP with A
    assert(d.read_index(19) == 3); // expected mask V|A
    assert(d.d->launch_hold_o == 3);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2); // GO
    assert(d.d->launch_go_o == 2); // A first only
    d.tick();
    assert(d.d->launch_go_o == 0);
    d.set_held(1, false);
    d.set_active(1, true);
    d.issue(1, 2);
    d.tick();
    d.clear_events();
    // Valid A read select.
    d.d->selected_i = 1; d.d->select_write_i = 0;
    d.d->select_id_i = 4; d.d->select_did_i = 11; d.d->select_iova_i = 0x1000;
    d.tick();
    assert(d.d->launch_go_o == 1); // V go on following cycle
    d.d->selected_i = 0;
    d.tick();
    assert(d.d->launch_go_o == 0);
}

static void test_v_never_go_on_bad_a_select(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2);
    d.tick();
    d.set_held(1, false);
    d.set_active(1, true);
    d.issue(1, 2);
    d.tick();
    d.clear_events();
    // A write must not release V.
    d.d->selected_i = 1; d.d->select_write_i = 1;
    d.d->select_id_i = 4; d.d->select_did_i = 11;
    d.tick();
    assert(d.d->launch_go_o == 0);
    // Wrong ID.
    d.d->select_write_i = 0; d.d->select_id_i = 5; d.d->select_did_i = 11;
    d.tick();
    assert(d.d->launch_go_o == 0);
    // Wrong DID.
    d.d->select_id_i = 4; d.d->select_did_i = 12;
    d.tick();
    assert(d.d->launch_go_o == 0);
    d.d->selected_i = 0;
}

static void test_duplicate_and_bad_writes(Dut &d) {
    d.write(0xb8, 1);
    d.write(0xb8, 1); // second PREP
    assert(d.error_any());
    d.write(0x60, 4); // unsupported forwarded write
    assert(d.error_any());
    d.write(0xb8, 2, 0x00); // partial strobe
    assert(d.error_any());
    d.write(0xb0, 0); // write magic
    assert(d.error_any());
    d.write(0xc8, 0); // write data
    assert(d.error_any());
}

static void test_abort_before_reserve(Dut &d) {
    d.write(0xb8, 4); // ABORT in OFF
    d.tick();
    uint64_t s = d.status();
    assert((s & 8) == 8); // aborted bit
    assert((s & 4) == 4); // frozen
    assert((s & 1) == 0); // not PREP
}

static void test_go_then_immediate_abort(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2); // GO, registers go pulse
    assert(d.d->launch_go_o == 2);
    // Same edge MMIO ABORT while registered go is high.
    d.write_no_tick(0xb8, 4, 0xff);
    assert(d.d->launch_go_o == 0);
    assert((d.d->launch_cancel_o & 0x3) == 0x3);
    d.tick();
    d.end_write();
    // Frontend acknowledges cancel for both held ports.
    d.d->frontend_cancelled_i = 0x3;
    d.tick();
    d.d->frontend_cancelled_i = 0;
    d.set_held(0, false);
    d.set_held(1, false);
    d.tick();
    assert((d.status() & 8) == 8);
    assert(d.read_index(17) == 3);
    assert(d.read_index(15) == 0);
}

static void test_count_backpressure_abort_drains(Dut &d) {
    d.write(0xb8, 1); // PREP solo
    d.set_held(0, true);
    d.write(0xb8, 2); // GO
    d.tick();
    // Frontend left HOLD with COUNT backpressure.
    d.set_held(0, false);
    d.set_active(0, true);
    d.tick();
    // ABORT while active COUNT.
    d.write(0xb8, 4);
    assert((d.d->launch_cancel_o & 1) == 0);
    d.tick();
    d.tick();
    assert((d.status() & 4) == 0); // not frozen
    // Actual issue and done drain.
    d.issue(0, 103);
    d.tick();
    d.clear_events();
    d.done(0, 103);
    d.tick();
    d.clear_events();
    d.set_active(0, false);
    d.tick();
    assert((d.status() & 8) == 8);
    assert((d.status() & 4) == 4);
    assert(d.read_index(15) == 1);
    assert(d.read_index(16) == 1);
    assert(d.read_index(17) == 0);
}

static void test_partial_reserve_abort(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true); // only V reserves
    d.write(0xb8, 4); // ABORT while only V held
    assert((d.d->launch_cancel_o & 1) == 1);
    assert((d.d->launch_cancel_o & 2) == 0);
    d.tick();
    d.d->frontend_cancelled_i = 1;
    d.tick();
    d.d->frontend_cancelled_i = 0;
    d.set_held(0, false);
    d.tick();
    assert((d.status() & 8) == 8);
    assert(d.read_index(17) == 1);
}

static void test_new_prep_after_aborted_terminal(Dut &d) {
    d.write(0xb8, 4); // abort in OFF -> frozen aborted
    d.tick();
    assert((d.status() & 8) == 8);
    d.write(0xb8, 1); // new PREP in FROZEN
    assert((d.status() & 1) == 1);
    assert((d.status() & 8) == 0);
    assert(d.read_index(1) == 0);
}

static void test_both_done_and_late_a(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2);
    d.tick();
    d.set_held(1, false);
    d.set_active(1, true);
    d.issue(1, 2);
    d.tick();
    d.clear_events();
    d.d->selected_i = 1; d.d->select_write_i = 0;
    d.d->select_id_i = 4; d.d->select_did_i = 11;
    d.tick();
    d.d->selected_i = 0;
    d.tick();
    // V issue and done first.
    d.set_held(0, false);
    d.set_active(0, true);
    d.issue(0, 3);
    d.tick();
    d.clear_events();
    d.done(0, 3);
    d.tick();
    d.clear_events();
    d.set_active(0, false);
    d.tick();
    assert((d.status() & 4) == 0); // not frozen, A still pending
    // A done late.
    d.done(1, 2);
    d.tick();
    d.clear_events();
    d.set_active(1, false);
    d.tick();
    assert((d.status() & 4) == 4);
}

static void test_abort_select_race(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2);
    d.tick();
    d.set_held(1, false);
    d.set_active(1, true);
    d.issue(1, 2);
    // ABORT and valid A select same cycle. ABORT must win.
    d.d->selected_i = 1; d.d->select_write_i = 0;
    d.d->select_id_i = 4; d.d->select_did_i = 11;
    d.write_no_tick(0xb8, 4, 0xff);
    assert(d.d->launch_go_o == 0);
    d.tick();
    d.end_write();
    d.d->selected_i = 0;
    d.clear_events();
    // A issue must still drain.
    d.done(1, 2);
    d.tick();
    d.clear_events();
    d.set_active(1, false);
    d.tick();
    assert((d.status() & 8) == 8);
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    test_magic(*(new Dut));
    test_solo_prep_go_done(*(new Dut));
    test_non_solo_exact_goA_then_goV(*(new Dut));
    test_v_never_go_on_bad_a_select(*(new Dut));
    test_duplicate_and_bad_writes(*(new Dut));
    test_abort_before_reserve(*(new Dut));
    test_go_then_immediate_abort(*(new Dut));
    test_count_backpressure_abort_drains(*(new Dut));
    test_partial_reserve_abort(*(new Dut));
    test_new_prep_after_aborted_terminal(*(new Dut));
    test_both_done_and_late_a(*(new Dut));
    test_abort_select_race(*(new Dut));
    std::puts("PASS dma_controlled_launcher");
    return 0;
}
