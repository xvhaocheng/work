// SPDX-License-Identifier: Solderpad-0.51
//
// Verilator 4.038 harness for dma_composition_control.
// Manual clock, no SV timing.  The test checks the default and the
// +define+CVC_TEST_GEN_FULL generation-saturation model.
//
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "Vcomposition_control_top.h"
#include "verilated.h"

static Vcomposition_control_top *dut;
static int g_fail = 0;

static void check(const char *what, uint64_t got, uint64_t want) {
    if (got != want) {
        std::fprintf(stderr,
                     "FAIL %s: got %llu (0x%llx) want %llu (0x%llx)\n",
                     what,
                     (unsigned long long)got, (unsigned long long)got,
                     (unsigned long long)want, (unsigned long long)want);
        g_fail++;
    }
}

static void tick() {
    dut->clk_i = 0; dut->eval();
    dut->clk_i = 1; dut->eval();
    dut->clk_i = 0; dut->eval();
}

static void settle() {
    dut->clk_i = 0; dut->eval();
}

static void idle() {
    dut->cfg_write_i = 0;
    dut->index_i = 0;
    dut->wdata_i = 0;
    dut->quiet_i = 0;
    dut->supported_i = 1;
    dut->arm_i = 0;
    dut->open_i = 0;
    dut->close_i = 0;
    dut->lookup_i = 0;
    dut->lookup_hit_i = 0;
    dut->normal_i = 0;
    dut->stage1_i = 0;
    dut->stage2_i = 0;
    dut->lookup_did_i = 0;
    dut->lookup_pscid_i = 0;
    dut->accept_i = 0;
    dut->fast_i = 0;
    dut->fast_did_i = 0;
    dut->gap_active_i = 0;
    dut->update_i = 0;
    dut->invalidate_i = 0;
    dut->update_did_i = 0;
    dut->replace_mask_i = 0;
    dut->plru_i = 0;
}

static void reset_dut() {
    dut->rst_ni = 0;
    idle();
    settle();
    tick();
    dut->rst_ni = 1;
    idle();
    settle();
}

static uint64_t rd(int idx) {
    dut->cfg_write_i = 0;
    dut->index_i = (uint8_t)(idx & 0x7f);
    settle();
    return (uint64_t)dut->rdata_o;
}

static bool wr_cfg(uint64_t data, bool quiet, bool supported) {
    idle();
    dut->cfg_write_i = 1;
    dut->index_i = 2;
    dut->wdata_i = data;
    dut->quiet_i = quiet ? 1 : 0;
    dut->supported_i = supported ? 1 : 0;
    settle();
    bool clear = dut->clear_o != 0;
    tick();
    idle();
    settle();
    return clear;
}

static void set_entry(VlWide<64> &flat, int idx, bool valid, uint32_t pscid) {
    int base = idx * 4;
    uint64_t low = 0;
    if (valid) low |= (1ULL << 51);
    low |= ((uint64_t)(pscid & 0xFFFFF)) << 29;
    flat[base]     = (IData)(low & 0xffffffffULL);
    flat[base + 1] = (IData)(low >> 32);
    flat[base + 2] = 0;
    flat[base + 3] = 0;
}

static void test_reset_and_config() {
    // clear_o must remain low while reset is asserted.
    dut->rst_ni = 0;
    idle();
    dut->cfg_write_i = 1;
    dut->index_i = 2;
    dut->wdata_i = ((uint64_t)10 << 8) | 0;
    dut->quiet_i = 1;
    dut->supported_i = 1;
    settle();
    check("reset clear gated", dut->clear_o ? 1 : 0, 0);
    tick();
    dut->rst_ni = 1;
    idle();
    settle();

    reset_dut();
    check("magic", rd(0), 0x49444D4143504E01ULL);
    check("abi", rd(1), 1);
    check("reset mode", rd(2) & 3, 0);
    check("reset did", (rd(2) >> 8) & 0xFFFFFF, 10);
    check("reset gen", rd(3), 0);
    check("reset status", rd(4), 0);
    check("reset flags", rd(6), 0);
    check("reset seq", rd(7), 0);
    check("reset cycles", rd(8), 0);
    check("reset err", rd(9), 0);
    check("reset mask", rd(34), 3);
    check("mode_o reset", dut->mode_o, 0);
    check("did_o reset", dut->protected_did_o, 10);

    for (int m = 0; m < 4; m++) {
        int did = (m & 1) ? 11 : 10;
        uint64_t data = ((uint64_t)did << 8) | (uint64_t)m;
        uint64_t gen_before = rd(3);
        bool clear = wr_cfg(data, true, true);
        check("cfg clear", clear ? 1 : 0, 1);
        check("cfg status", rd(4), 0);
        check("cfg mode", rd(2) & 3, (uint64_t)m);
        check("cfg did", (rd(2) >> 8) & 0xFFFFFF, (uint64_t)did);
        check("cfg gen", rd(3), gen_before + 1);
        check("cfg mode_o", dut->mode_o, (uint64_t)m);
        check("cfg did_o", dut->protected_did_o, (uint64_t)did);

        gen_before = rd(3);
        clear = wr_cfg(data, true, true);
        check("same cfg clear", clear ? 1 : 0, 1);
        check("same cfg gen", rd(3), gen_before + 1);
    }

    uint64_t mode_before = rd(2);
    uint64_t gen_before = rd(3);
    bool clear = wr_cfg(((uint64_t)10 << 8) | 0, false, true);
    check("busy clear", clear ? 1 : 0, 0);
    check("busy status", rd(4), 1);
    check("busy mode unchanged", rd(2), mode_before);
    check("busy gen unchanged", rd(3), gen_before);

    clear = wr_cfg(((uint64_t)10 << 8) | 0, true, false);
    check("unsupported clear", clear ? 1 : 0, 0);
    check("unsupported status", rd(4), 2);
    check("unsupported mode unchanged", rd(2), mode_before);
    check("unsupported gen unchanged", rd(3), gen_before);

    clear = wr_cfg(((uint64_t)10 << 8) | 0 | (1ULL << 32), true, true);
    check("bad bits clear", clear ? 1 : 0, 0);
    check("bad bits status", rd(4), 2);

    clear = wr_cfg(((uint64_t)12 << 8) | 0, true, true);
    check("bad did clear", clear ? 1 : 0, 0);
    check("bad did status", rd(4), 2);

    mode_before = rd(2);
    gen_before = rd(3);
    idle();
    dut->cfg_write_i = 1;
    dut->index_i = 2;
    dut->wdata_i = ((uint64_t)11 << 8) | 2;
    dut->quiet_i = 1;
    dut->supported_i = 1;
    dut->arm_i = 1;
    settle();
    clear = dut->clear_o != 0;
    tick();
    idle();
    settle();
    check("cfg+arm clear", clear ? 1 : 0, 0);
    check("cfg+arm status", rd(4), 1);
    check("cfg+arm mode unchanged", rd(2), mode_before);
    check("cfg+arm gen unchanged", rd(3), gen_before);
    check("cfg+arm armed", (rd(6) >> 0) & 1, 1);
}

static void test_window_events() {
    reset_dut();
    bool clear = wr_cfg(((uint64_t)10 << 8) | 2, true, true);
    check("win cfg", clear ? 1 : 0, 1);
    uint64_t win_cfg = rd(2);
    uint64_t win_gen = rd(3);

    idle(); dut->arm_i = 1; tick(); idle(); settle();
    check("win armed", (rd(6) >> 0) & 1, 1);
    check("win seq", rd(7), 1);
    check("win cfg latch", rd(10), win_cfg);
    check("win gen latch", rd(11), win_gen);

    uint64_t mode_before = rd(2);
    uint64_t gen_before = rd(3);
    clear = wr_cfg(((uint64_t)10 << 8) | 0, true, true);
    check("armed cfg clear", clear ? 1 : 0, 0);
    check("armed cfg status", rd(4), 1);
    check("armed cfg mode unchanged", rd(2), mode_before);
    check("armed cfg gen unchanged", rd(3), gen_before);

    for (int i = 0; i < 16; i++) {
        set_entry(dut->entries_i, i, i < 4, 0x100 + i);
        set_entry(dut->entries_next_i, i, false, 0);
    }
    dut->plru_i = 0x12345678;
    idle(); dut->open_i = 1; tick(); idle(); settle();
    check("win active", (rd(6) >> 1) & 1, 1);
    check("open cycles", rd(8), 0);
    check("open plru", rd(36), 0x12345678);
    for (int i = 0; i < 4; i++) {
        check("open entry low", rd(64 + 2 * i), (1ULL << 51) | ((uint64_t)(0x100 + i) << 29));
        check("open entry high", rd(64 + 2 * i + 1), 0);
    }

    idle(); tick(); idle(); settle();
    check("active cycle", rd(8), 1);

    // Three normal V(DID10) lookups: hit, miss, hit.  The first two are
    // consecutive cycles; the second uses a different PSCID that must not
    // overwrite the first-PSCID latch.
    idle();
    dut->lookup_i = 1; dut->lookup_did_i = 10; dut->normal_i = 1;
    dut->stage1_i = 1; dut->stage2_i = 0; dut->lookup_hit_i = 1;
    dut->lookup_pscid_i = 0x123;
    tick();
    dut->lookup_hit_i = 0;
    dut->lookup_pscid_i = 0x999;
    tick();
    idle(); settle();
    check("lookup_v after 2", rd(12), 2);
    check("hit_v after 2", rd(13), 1);
    check("miss_v after 2", rd(14), 1);
    check("pscid_v first", rd(38), 0x123);

    idle(); dut->lookup_i = 1; dut->lookup_did_i = 11; dut->normal_i = 1;
    dut->stage1_i = 1; dut->stage2_i = 0; dut->lookup_hit_i = 0;
    dut->lookup_pscid_i = 0x456;
    tick(); idle(); settle();
    check("lookup_a", rd(15), 1);
    check("hit_a", rd(16), 0);
    check("miss_a", rd(17), 1);
    check("pscid_a", rd(39), 0x456);

    idle(); dut->lookup_i = 1; dut->lookup_did_i = 10; dut->normal_i = 1;
    dut->stage1_i = 1; dut->stage2_i = 0; dut->lookup_hit_i = 1;
    dut->lookup_pscid_i = 0x777;
    tick(); idle(); settle();
    check("lookup_v after 3", rd(12), 3);
    check("hit_v after 3", rd(13), 2);
    check("miss_v after 3", rd(14), 1);
    check("pscid_v unchanged", rd(38), 0x123);

    // Other DIDs are counted independently for each lookup.
    idle(); dut->lookup_i = 1; dut->lookup_did_i = 12; dut->normal_i = 1;
    dut->stage1_i = 1; dut->stage2_i = 0; dut->lookup_hit_i = 1;
    tick(); idle(); settle();
    check("lookup_o first", rd(18), 1);
    check("hit_o first", rd(19), 1);
    check("miss_o first", rd(20), 0);

    idle(); dut->lookup_i = 1; dut->lookup_did_i = 13; dut->normal_i = 1;
    dut->stage1_i = 1; dut->stage2_i = 0; dut->lookup_hit_i = 0;
    tick(); idle(); settle();
    check("lookup_o second", rd(18), 2);
    check("hit_o second", rd(19), 1);
    check("miss_o second", rd(20), 1);

    idle(); dut->accept_i = 1; dut->fast_i = 1; dut->fast_did_i = 10;
    tick(); idle(); settle();
    check("fast_v", rd(21), 1);
    check("fast_n", rd(33), 1);
    check("err fast_n", (rd(9) >> 2) & 1, 1);
    idle(); dut->accept_i = 1; dut->fast_i = 1; dut->fast_did_i = 11;
    tick(); idle(); settle();
    check("fast_a", rd(22), 1);
    check("fast_n2", rd(33), 2);
    idle(); dut->accept_i = 1; dut->fast_i = 1; dut->fast_did_i = 12;
    tick(); idle(); settle();
    check("fast_o", rd(23), 1);
    check("fast_n3", rd(33), 3);

    idle(); dut->fast_i = 1; dut->accept_i = 0; dut->fast_did_i = 10;
    tick(); idle(); settle();
    check("err bad_event", (rd(9) >> 0) & 1, 1);

    idle(); dut->gap_active_i = 1; tick(); idle(); settle();
    check("gap_v", rd(24), 1);
    idle(); dut->gap_active_i = 2; tick(); idle(); settle();
    check("gap_a", rd(25), 1);

    set_entry(dut->entries_i, 0, true, 1);
    set_entry(dut->entries_next_i, 0, true, 2);
    idle(); dut->update_i = 1; dut->replace_mask_i = 1 << 0; dut->update_did_i = 10;
    tick(); idle(); settle();
    check("fill_v", rd(26), 1);
    check("repl_pscid", rd(29), 1);
    check("part_viol zero", rd(30), 0);

    set_entry(dut->entries_i, 1, true, 1);
    set_entry(dut->entries_next_i, 1, true, 2);
    idle(); dut->update_i = 1; dut->replace_mask_i = 1 << 1; dut->update_did_i = 12;
    tick(); idle(); settle();
    check("fill_o", rd(28), 1);
    check("part_viol one", rd(30), 1);
    check("err part", (rd(9) >> 4) & 1, 1);

    idle(); dut->update_i = 1; dut->replace_mask_i = 3; dut->update_did_i = 10;
    tick(); idle(); settle();
    check("err bad_geom", (rd(9) >> 3) & 1, 1);

    idle(); dut->update_i = 1; dut->replace_mask_i = 0; dut->update_did_i = 10;
    tick(); idle(); settle();
    check("err bad_geom zero", (rd(9) >> 3) & 1, 1);

    idle(); dut->invalidate_i = 1; tick(); idle(); settle();
    check("inval", rd(31), 1);

    set_entry(dut->entries_i, 2, true, 1);
    set_entry(dut->entries_next_i, 2, true, 3);
    dut->plru_i = 0xCAFEBABE;
    idle();
    dut->close_i = 1;
    dut->update_i = 1;
    dut->replace_mask_i = 1 << 2;
    dut->update_did_i = 11;
    tick();
    idle(); settle();
    check("frozen", (rd(6) >> 2) & 1, 1);
    check("close plru", rd(37), 0xCAFEBABE);
    check("close entry2 low", rd(96 + 2 * 2), (1ULL << 51) | (3ULL << 29));
    check("close entry2 high", rd(96 + 2 * 2 + 1), 0);
    check("fill_a", rd(27), 1);
    check("repl_pscid2", rd(29), 2);
    check("close cycles", (rd(8) > 0) ? 1 : 0, 1);

    check("conserve v", rd(13) + rd(14), rd(12));
    check("conserve a", rd(16) + rd(17), rd(15));
    check("conserve o", rd(19) + rd(20), rd(18));

    uint64_t a[40];
    for (int i = 0; i < 40; i++) a[i] = rd(i);
    idle(); tick(); idle(); settle();
    for (int i = 0; i < 40; i++) check("frozen stable", rd(i), a[i]);

    uint64_t f_cycles = rd(8);
    uint64_t f_cfg = rd(10);
    uint64_t f_gen = rd(11);
    uint64_t f_flags = rd(6);
    clear = wr_cfg(((uint64_t)10 << 8) | 0, true, true);
    check("postclose clear", clear ? 1 : 0, 1);
    check("postclose flags", rd(6), f_flags);
    check("postclose cycles", rd(8), f_cycles);
    check("postclose win cfg", rd(10), f_cfg);
    check("postclose win gen", rd(11), f_gen);
    check("postclose mode S", rd(2) & 3, 0);

    idle(); dut->arm_i = 1; tick(); idle(); settle();
    check("rearm seq", rd(7), 2);
    check("rearm cycles", rd(8), 0);
    check("rearm win cfg", rd(10), rd(2));
    check("rearm win gen", rd(11), rd(3));
    check("rearm err clear", rd(9), 0);

    for (int i = 0; i < 16; i++) set_entry(dut->entries_i, i, false, 0);
    idle(); dut->open_i = 1; tick(); idle(); settle();
    check("open2 cycles", rd(8), 0);
    idle(); tick(); idle(); settle();
    check("active2 cycles", rd(8), 1);
    idle(); dut->close_i = 1; tick(); idle(); settle();
    check("close2 cycles", rd(8), 2);
}

static void test_abnormal_lookup() {
    reset_dut();
    wr_cfg(((uint64_t)10 << 8) | 0, true, true);
    idle(); dut->arm_i = 1; tick(); idle(); settle();
    idle(); dut->open_i = 1; tick(); idle(); settle();
    idle(); dut->lookup_i = 1; dut->lookup_did_i = 12; dut->normal_i = 0;
    dut->stage1_i = 1; dut->stage2_i = 0;
    tick(); idle(); settle();
    check("first_bad", rd(32), 1);
    check("err bad stage", (rd(9) >> 3) & 1, 1);
    check("other lookup counted", rd(18), 1);
    check("other hit zero", rd(19), 0);
    check("other miss counted", rd(20), 1);
    idle(); dut->close_i = 1; tick(); idle(); settle();
}

static int test_gen_full() {
    reset_dut();
    check("gen full", rd(3), 0xFFFFFFFFFFFFFFFFULL);
    uint64_t mode_before = rd(2);
    bool clear = wr_cfg(((uint64_t)10 << 8) | 0, true, true);
    check("gen full no clear", clear ? 1 : 0, 0);
    check("gen full status", rd(4), 3);
    check("gen full unchanged", rd(3), 0xFFFFFFFFFFFFFFFFULL);
    check("gen full mode unchanged", rd(2), mode_before);
    return g_fail ? 1 : 0;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vcomposition_control_top;
    reset_dut();

    if (rd(3) == 0xFFFFFFFFFFFFFFFFULL) {
        int rc = test_gen_full();
        delete dut;
        return rc;
    }

    test_reset_and_config();
    test_window_events();
    test_abnormal_lookup();

    delete dut;
    if (g_fail) {
        std::fprintf(stderr, "%d check(s) failed\n", g_fail);
        return 1;
    }

    std::printf("PASS\n");
    return 0;
}