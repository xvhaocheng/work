#include "Vdma_observer.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>

struct B {
    Vdma_observer d;

    void tick() {
        d.clk_i = 0; d.eval();
        d.clk_i = 1; d.eval();
        d.clk_i = 0; d.eval();
    }

    B() {
        d.rst_ni = 0;
        d.cfg_valid_i = 0; d.cfg_write_i = 0;
        d.cfg_addr_i = 0; d.cfg_wdata_i = 0; d.cfg_wstrb_i = 0;

        d.issue_i = 0; d.done_i = 0; d.poll_i = 0;
        d.batch_active_i = 0;

        for (int i = 0; i < 4; i++) {
            d.issue_id_i[i] = 0;
            d.done_id_i[i] = 0;
            d.poll_id_i[i] = 0;
        }

        d.ar_valid_i = 0; d.aw_valid_i = 0;
        d.ar_ready_i = 0; d.aw_ready_i = 0;
        d.rlast_fire_i = 0; d.b_fire_i = 0; d.bad_id_i = 0;
        d.shared_ar_valid_i = 0; d.shared_aw_valid_i = 0;
        d.shared_ar_ready_i = 0; d.shared_aw_ready_i = 0;

        for (int i = 0; i < 2; i++) {
            d.tr_id_i[i] = 0;
            d.out_id_i[i] = 0;
            d.return_id_i[i] = 0;
        }

        d.out_valid_i = 0; d.out_ready_i = 0; d.return_i = 0;
        d.e3_data_i = 0; d.e3_error_i = 0;

        tick();
        d.rst_ni = 1;
        tick();
    }

    void write(unsigned a, unsigned long long v) {
        d.cfg_valid_i = d.cfg_write_i = 1;
        d.cfg_wstrb_i = 0xff;
        d.cfg_addr_i = a;
        d.cfg_wdata_i = v;
        tick();
        d.cfg_valid_i = d.cfg_write_i = 0;
        d.eval();
    }

    unsigned long long read(unsigned a) {
        d.cfg_addr_i = a;
        d.eval();
        return d.cfg_rdata_o;
    }
};

// Victim port is V=0.  A is aggressor port 1.  Keep bit 8 clear.
static void arm_common(B &b, bool finite, unsigned target = 1) {
    b.write(0x68, target);
    unsigned long long cmd = 1 | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27);
    if (finite) cmd |= (1ULL << 29);
    b.write(0x60, cmd);
}

#ifdef CONTROLLED
static void test_controlled_finite_allows_a_before_v() {
    B b;
    assert(b.read(0x58) == 0x49444d414f425306ULL);
    arm_common(b, true, 1);
    assert(!(b.read(0x60) >> 32));

    // A batch is active initially.
    b.d.batch_active_i = 1u << 1;
    b.d.eval();

    // V issue on port 0.
    b.d.issue_i = 1u << 0;
    b.d.issue_id_i[0] = 1;
    b.tick();
    b.d.issue_i = 0;
    b.d.eval();
    assert(!(b.read(0x60) >> 32));

    // A finishes while V is still active: A batch_active drops to 0.
    b.d.batch_active_i = 0;
    b.d.eval();
    assert(!(b.read(0x60) >> 32)); // controlled finite must not flag bit5
    assert(!(b.read(0x60) & (1ULL << 37)));

    // Full V done/poll freeze.
    b.d.done_i = 1u << 0;
    b.d.done_id_i[0] = 1;
    b.tick();
    b.d.done_i = 0;
    b.d.eval();

    b.d.poll_i = 1u << 0;
    b.d.poll_id_i[0] = 1;
    b.tick();
    b.d.poll_i = 0;
    b.d.eval();

    unsigned long long status = b.read(0x60);
    assert((status >> 32) == 0);
    assert(status & (1ULL << 2)); // FROZEN
    assert(!(status & (1ULL << 37)));
}

static void test_controlled_rejects_finite_without_e3() {
    B b;
    b.write(0x68, 1);
    unsigned long long cmd = 1 | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1ULL << 29);
    b.write(0x60, cmd);
    assert((b.read(0x60) >> 32) != 0);
}

static void test_controlled_rejects_finite_target_gt1() {
    B b;
    b.write(0x68, 2);
    unsigned long long cmd = 1 | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27) | (1ULL << 29);
    b.write(0x60, cmd);
    assert((b.read(0x60) >> 32) != 0);
}
#endif

#ifndef CONTROLLED
static void test_legacy_finite_flag_rejected_and_continuous_a_error() {
    B b;
    assert(b.read(0x58) == 0x49444d414f425305ULL);
    b.write(0x68, 1);
    unsigned long long bad = 1 | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27) | (1ULL << 29);
    b.write(0x60, bad);
    assert((b.read(0x60) >> 32) != 0);

    // Same A-before-V without finite must set batch-active error bit5
    // only when A becomes inactive while V is active.
    B c;
    arm_common(c, false, 1);
    assert(!(c.read(0x60) >> 32));

    c.d.batch_active_i = 1u << 1;
    c.d.eval();

    c.d.issue_i = 1u << 0;
    c.d.issue_id_i[0] = 1;
    c.tick();
    c.d.issue_i = 0;
    c.d.eval();
    assert(!(c.read(0x60) >> 32)); // still no error while A active

    c.d.batch_active_i = 0;
    c.d.eval();
    c.tick();

    unsigned long long status = c.read(0x60);
    assert(status & (1ULL << 37)); // error bit 5
}
#endif

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
#ifdef CONTROLLED
    test_controlled_finite_allows_a_before_v();
    test_controlled_rejects_finite_without_e3();
    test_controlled_rejects_finite_target_gt1();
    std::puts("PASS controlled v6 observer finite/legacy checks");
#else
    test_legacy_finite_flag_rejected_and_continuous_a_error();
    std::puts("PASS legacy v5 observer continuous A rejection");
#endif
    return 0;
}
