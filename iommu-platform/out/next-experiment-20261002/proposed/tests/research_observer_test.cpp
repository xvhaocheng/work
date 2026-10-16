// Research observer contract regression: E2 and cost modes.
// Independently driven clock and event schedule; no board or formal claim.
#include "Vdma_research_observer.h"
#include "verilated.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

static unsigned cases;
static void check(bool condition, const char *message) {
    if (!condition) throw std::runtime_error(message);
}

static uint64_t entry_lo(uint32_t vpn, uint32_t pscid, bool en1, bool en2, bool valid,
                         bool s1is1g, bool s1is2m, bool s2is1g, bool s2is2m, bool s1global) {
    uint64_t lo = 0;
    lo |= (uint64_t)(vpn & 0x1FFFFFFF);
    lo |= (uint64_t)(pscid & 0xFFFFF) << 29;
    lo |= (uint64_t)en1 << 49;
    lo |= (uint64_t)en2 << 50;
    lo |= (uint64_t)valid << 51;
    lo |= (uint64_t)s1is1g << 52;
    lo |= (uint64_t)s1is2m << 53;
    lo |= (uint64_t)s2is1g << 54;
    lo |= (uint64_t)s2is2m << 55;
    lo |= (uint64_t)s1global << 56;
    return lo;
}
static uint64_t entry_hi(uint16_t gscid) { return gscid; }

struct Bench {
    Vdma_research_observer d;
    void tick(unsigned n = 1) {
        while (n--) {
            d.clk_i = 0; d.eval();
            d.clk_i = 1; d.eval();
            d.clk_i = 0; d.eval();
        }
    }
    void reset() {
        d.cfg_valid_i = d.cfg_write_i = 0;
        d.index_i = 0;
        d.wdata_i = 0;
        d.quiet_i = 1;
        d.update_i = d.invalidate_i = 0;
        d.replace_mask_i = 0;
        d.update_did_i = 0;
        d.lookup_i = d.lookup_hit_i = 0;
        d.lookup_did_i = 0;
        d.lookup_iova_i = 0;
        d.lookup_pscid_i = 0;
        d.lookup_gscid_i = 0;
        d.lookup_stage1_i = 0;
        d.lookup_stage2_i = 0;
        d.ctl_state_i = 0;
        d.ctl_did_i = 0;
        d.select_fire_i = 0;
        d.select_did_i = 0;
        d.response_error_i = 0;
        d.issue_i = 0;
        d.done_i = 0;
        d.issue_id_i[0] = d.issue_id_i[1] = 0;
        d.done_id_i[0] = d.done_id_i[1] = 0;
        d.p4m_arm_i = d.p4m_open_i = d.p4m_close_i = 0;
        d.plru_i = 0;
        for (int i = 0; i < 16; i++) {
            for (int w = 0; w < 4; w++) {
                d.entries_i[i][w] = 0;
                d.entries_next_i[i][w] = 0;
            }
        }
        d.rst_ni = 0; tick(2); d.rst_ni = 1; tick();
    }
    Bench() { reset(); }
    void write(unsigned idx, uint64_t v) {
        d.cfg_valid_i = d.cfg_write_i = 1;
        d.index_i = idx;
        d.wdata_i = v;
        tick();
        d.cfg_valid_i = d.cfg_write_i = 0;
        d.eval();
    }
    uint64_t read(unsigned idx) {
        d.index_i = idx;
        d.eval();
        return d.rdata_o;
    }
    void set_entry(int idx, uint64_t lo, uint64_t hi) {
        d.entries_i[idx][0] = lo & 0xFFFFFFFF;
        d.entries_i[idx][1] = lo >> 32;
        d.entries_i[idx][2] = hi & 0xFFFFFFFF;
        d.entries_i[idx][3] = hi >> 32;
    }
    void set_entry_next(int idx, uint64_t lo, uint64_t hi) {
        d.entries_next_i[idx][0] = lo & 0xFFFFFFFF;
        d.entries_next_i[idx][1] = lo >> 32;
        d.entries_next_i[idx][2] = hi & 0xFFFFFFFF;
        d.entries_next_i[idx][3] = hi >> 32;
    }
    void copy_entries_to_next() {
        for (int i = 0; i < 16; i++)
            for (int w = 0; w < 4; w++)
                d.entries_next_i[i][w] = d.entries_i[i][w];
    }
    void arm_warm() { write(0x0005, 1); }
    void arm_a() { write(0x0005, 2); }
    void probe() { write(0x0005, 3); }
    void freeze() { write(0x0005, 4); }
    void abort() { write(0x0005, 5); }
    void clear() { write(0x0005, 6); }
    void mode(bool cost, bool enable = true) { write(0x0004, (enable ? 1ULL : 0) | (cost ? 2ULL : 0)); }
    void p4m_arm() { d.p4m_arm_i = 1; tick(); d.p4m_arm_i = 0; }
    void p4m_open() { d.p4m_open_i = 1; tick(); d.p4m_open_i = 0; }
    void p4m_close() { d.p4m_close_i = 1; tick(); d.p4m_close_i = 0; }
    void lookup(uint64_t iova, uint32_t pscid, uint16_t gscid, bool hit,
                bool s1 = true, bool s2 = false, uint32_t did = 0) {
        d.lookup_i = 1;
        d.lookup_iova_i = iova;
        d.lookup_pscid_i = pscid;
        d.lookup_gscid_i = gscid;
        d.lookup_hit_i = hit;
        d.lookup_stage1_i = s1;
        d.lookup_stage2_i = s2;
        d.lookup_did_i = did;
        tick();
        d.lookup_i = 0;
    }
    void issue(int lane, uint64_t id) { d.issue_i = 1 << lane; d.issue_id_i[lane] = id; tick(); d.issue_i = 0; }
    void done(int lane, uint64_t id) { d.done_i = 1 << lane; d.done_id_i[lane] = id; tick(); d.done_i = 0; }
    void set_ctl_state(int st) { d.ctl_state_i = st; }
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    bool small = (argc > 1 && std::string(argv[1]) == "small");
    try {
        Bench b;
        check(b.read(0x0000) == 0x49444d4152455331ULL, "research magic 0");
        check(b.read(0x0001) == 0x49444d414f425308ULL, "research magic 1");
        cases++;

        if (!small) {
            // E2 positive chain: warm captures two target keys, A phase logs one update,
            // deadline snapshot, PROBE records first lookups, FREEZE freezes session.
            b.reset();
            b.mode(false, true);
            uint64_t lo0 = entry_lo(0x12345, 0x1, true, false, true, false, false, false, false, false);
            uint64_t hi0 = entry_hi(0x1);
            b.set_entry(0, lo0, hi0);
            b.copy_entries_to_next();
            b.arm_warm();
            b.lookup(0x12345000ULL, 0x1, 0x1, true, true, false, 0);
            check((b.read(0x0002) & (1 << 16)) != 0, "target0 valid");
            uint64_t lo1 = entry_lo(0x23456, 0x2, true, false, true, false, false, false, false, false);
            uint64_t hi1 = entry_hi(0x2);
            b.set_entry(1, lo1, hi1);
            b.copy_entries_to_next();
            b.lookup(0x23456000ULL, 0x2, 0x2, true, true, false, 0);
            check((b.read(0x0002) & (1 << 17)) != 0, "target1 valid");
            b.arm_a();
            b.set_entry(2, 0x111, 0x1);
            b.set_entry_next(2, 0x222, 0x2);
            b.d.update_i = 1;
            b.d.replace_mask_i = (1 << 2);
            b.d.update_did_i = 0x10;
            b.tick();
            b.d.update_i = 0;
            b.copy_entries_to_next();
            b.tick(64);
            check((b.read(0x0002) & (1 << 19)) != 0, "deadline snapshot valid");
            b.probe();
            b.lookup(0x12345000ULL, 0x1, 0x1, true, true, false, 0);
            b.lookup(0x23456000ULL, 0x2, 0x2, true, true, false, 0);
            b.freeze();
            check(b.read(0x0008) == 4, "E2 frozen");
            check(b.read(0x000A) == 1, "one event logged");
            cases++;

            // Negative geometry: superpage must not be captured as target.
            b.reset();
            b.mode(false, true);
            b.arm_warm();
            uint64_t lo_bad = entry_lo(0x33333, 0x1, true, false, true, true, false, false, false, false);
            b.set_entry(0, lo_bad, 0x1);
            b.copy_entries_to_next();
            b.lookup(0x33333000ULL, 0x1, 0x1, true, true, false, 0);
            check((b.read(0x0002) & (1 << 16)) == 0, "unsupported geometry not captured");
            cases++;

            // Invalidation must set E2 error and count.
            b.reset();
            b.mode(false, true);
            b.arm_warm();
            b.lookup(0x12345000ULL, 0x1, 0x1, true, true, false, 0);
            b.lookup(0x23456000ULL, 0x2, 0x2, true, true, false, 0);
            b.arm_a();
            b.d.invalidate_i = 1;
            b.tick();
            b.d.invalidate_i = 0;
            check((b.read(0x0003) & (1 << 2)) != 0, "invalidation error");
            check(b.read(0x000B) == 1, "invalidation count");
            cases++;

            // Same-cycle multi-slot conflict must set error.
            b.reset();
            b.mode(false, true);
            b.arm_warm();
            b.lookup(0x12345000ULL, 0x1, 0x1, true, true, false, 0);
            b.lookup(0x23456000ULL, 0x2, 0x2, true, true, false, 0);
            b.arm_a();
            b.set_entry(3, 0x1, 0x1); b.set_entry_next(3, 0x2, 0x2);
            b.set_entry(4, 0x1, 0x1); b.set_entry_next(4, 0x2, 0x2);
            b.d.update_i = 1;
            b.d.replace_mask_i = (1 << 3) | (1 << 4);
            b.tick();
            b.d.update_i = 0;
            check((b.read(0x0003) & (1 << 1)) != 0, "multi-slot conflict error");
            cases++;

            // Quiet timeout at deadline.
            b.reset();
            b.mode(false, true);
            b.arm_warm();
            b.lookup(0x12345000ULL, 0x1, 0x1, true, true, false, 0);
            b.lookup(0x23456000ULL, 0x2, 0x2, true, true, false, 0);
            b.arm_a();
            b.d.quiet_i = 0;
            b.tick(65);
            b.d.quiet_i = 1;
            check((b.read(0x0003) & (1 << 4)) != 0, "quiet timeout error");
            cases++;

            // Cost mode: V main billing and COMPLETE separation.
            b.reset();
            b.mode(true, true);
            b.p4m_arm();
            b.p4m_open();
            b.set_ctl_state(1);
            b.issue(0, 0x100);
            b.tick(5);
            b.done(0, 0x100);
            b.set_ctl_state(4);
            b.tick(1);
            b.p4m_close();
            check(b.read(0x0C04) == 5, "V main cycles");
            check(b.read(0x0C06) == 1, "V complete cycles");
            cases++;

            // Cost carry-in and carry-out for an A copy crossing the window.
            b.reset();
            b.mode(true, true);
            b.p4m_arm();
            b.issue(1, 0x200);
            b.p4m_open();
            check(b.read(0x0C1C) == 1, "carry in");
            b.p4m_close();
            check(b.read(0x0C1D) == 1, "carry out");
            cases++;

            // Deterministic every-16th A-copy sampling.
            b.reset();
            b.mode(true, true);
            b.p4m_arm();
            b.p4m_open();
            for (int i = 1; i <= 16; i++) {
                b.set_ctl_state(1);
                b.issue(1, 0x300 + i);
                b.tick(1);
                b.done(1, 0x300 + i);
                b.set_ctl_state(4);
                b.tick(1);
            }
            b.p4m_close();
            check(b.read(0x0C19) == 1, "one sample at 16th");
            check(b.read(0x0800) == 0x310, "sample done id");
            cases++;
        } else {
            // Small-counter overflow evidence.
            b.reset();
            b.mode(true, true);
            b.p4m_arm();
            b.p4m_open();
            b.set_ctl_state(1);
            b.issue(0, 0x400);
            b.tick(300);
            b.done(0, 0x400);
            b.p4m_close();
            check(((b.read(0x0C02) >> 3) & 1) != 0, "counter overflow sticky");
            cases++;
        }

        std::cout << "PASS " << cases << " research observer scenarios (no board claim)\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL after " << cases << ": " << e.what() << "\n";
        return 1;
    }
}
