// Small capability/register test for the research observer role/features and
// reverse-order F10 register. Simulation only; no board claim.
#include "Vdma_research_observer.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

static int checks = 0;
static void check(bool c, const char *m) {
    if (!c) throw std::runtime_error(m);
    checks++;
}

enum : unsigned {
    R_MAGIC0 = 0x0000, R_MAGIC1 = 0x0001, R_STATUS = 0x0002, R_ERROR = 0x0003,
    R_MODE = 0x0004, R_CMD = 0x0005, R_PHASE = 0x0006,
    R_SRC = 0x0030, R_DST = 0x0031, R_IOVAV = 0x0032,
    R_E2ERR = 0x000D,
    R_F00 = 0x0F00, R_F01 = 0x0F01, R_F02 = 0x0F02, R_F03 = 0x0F03, R_F10 = 0x0F10
};

struct Entry { uint64_t lo, hi; };
static Entry mk_entry(uint64_t vpn, uint16_t gscid, uint32_t pscid, bool valid) {
    Entry e;
    e.lo  = (vpn & 0x1FFFFFFFULL);
    e.lo |= ((uint64_t)(pscid & 0xFFFFFULL)) << 29;
    e.lo |= 1ULL << 49;
    e.lo |= 0ULL << 50;
    e.lo |= ((uint64_t)valid) << 51;
    e.hi  = (uint64_t)gscid;
    return e;
}

struct Bench {
    Vdma_research_observer d;
    Bench() { reset(); }
    void tick(unsigned n = 1) {
        while (n--) { d.clk_i = 0; d.eval(); d.clk_i = 1; d.eval(); d.clk_i = 0; d.eval(); }
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
    void mirror() {
        for (int i = 0; i < 16; i++)
            for (int w = 0; w < 4; w++) d.entries_next_i[i][w] = d.entries_i[i][w];
    }
    void mode(uint64_t v) { write(R_MODE, v); }
    void cmd(uint64_t v) { write(R_CMD, v); }
    void arm_warm() { cmd(1); }
    void arm_a() { cmd(2); }
    void abort_cmd() { cmd(5); }
    void clear_cmd() { cmd(6); }
    void p4m_arm() { d.p4m_arm_i = 1; tick(1); d.p4m_arm_i = 0; }
    void p4m_open() { d.p4m_open_i = 1; tick(1); d.p4m_open_i = 0; }
    void p4m_close() { d.p4m_close_i = 1; tick(1); d.p4m_close_i = 0; }
    void lookup(uint64_t iova, uint32_t pscid, uint16_t gscid, bool hit, uint32_t did) {
        d.lookup_i = 1; d.lookup_iova_i = iova; d.lookup_pscid_i = pscid;
        d.lookup_gscid_i = gscid; d.lookup_hit_i = hit;
        d.lookup_stage1_i = 1; d.lookup_stage2_i = 0; d.lookup_did_i = did;
        tick(1);
        d.lookup_i = 0; d.eval();
    }
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    if (argc < 2) { std::cerr << "usage: research2_caps_test role" << std::endl; return 2; }
    int role = std::stoi(argv[1]);
    try {
        Bench b;
        uint64_t f02 = b.read(R_F02);
        check(f02 == (uint64_t)role, "F02 role");
        if (role == 0) {
            check(b.read(R_F00) == 0, "role0 F00 absent");
            check(b.read(R_F03) == 0, "role0 F03 absent");
        } else {
            check(b.read(R_F00) == 0x49444d4132434150ULL, "F00 magic");
            check(b.read(R_F01) == 1, "F01 version");
            uint64_t feat = b.read(R_F03);
            if (role == 2) {
                check(feat == 15, "role2 features");
                check((feat & 8) != 0, "role2 method control bit");
            } else {
                check(feat == 7, "role1 features");
                check((feat & 8) == 0, "role1 no method control bit");
            }
            b.write(R_F10, 1);
            check(b.read(R_F10) == 1, "F10 legal write readback");
            check(b.read(R_ERROR) == 0, "F10 legal write no error");
            b.write(R_F10, 2);
            check(b.read(R_F10) == 1, "F10 illegal bit rejected");
            check(((b.read(R_E2ERR) >> 0) & 1) == 1, "F10 illegal bit error");
            b.clear_cmd();
            check(b.read(R_F10) == 0, "CLEAR removes F10");
            b.write(R_F10, 1);
            check(b.read(R_F10) == 1, "F10 re-enabled");
            check(b.d.reverse_order_o == 0, "reverse quiet in IDLE");
            b.mode(2);
            b.d.ctl_state_i = 0; b.d.ctl_did_i = 0;
            b.p4m_arm();
            check(b.d.reverse_order_o == 1, "reverse active in COST");
            b.p4m_open();
            b.p4m_close();
            check(b.d.reverse_order_o == 0, "reverse quiet after COST close");
            b.clear_cmd();
            b.write(R_F10, 1);
            b.mode(1);
            b.write(R_SRC, 0x10000000ULL);
            b.write(R_DST, 0x20000000ULL);
            Entry src = mk_entry(0x10000000ULL >> 12, 0x1234, 0x1, true);
            Entry dst = mk_entry(0x20000000ULL >> 12, 0x1234, 0x1, true);
            b.set_entry(0, src); b.set_entry(1, dst); b.mirror();
            b.arm_warm();
            b.lookup(0x10000000ULL, 0x1, 0x1234, true, 10);
            b.lookup(0x20000000ULL, 0x1, 0x1234, true, 10);
            b.arm_a();
            check((b.read(R_PHASE) & 7) == 2, "E2 ARM_A for reverse");
            check(b.d.reverse_order_o == 1, "reverse active in E2 ARM_A");
            b.abort_cmd();
            check((b.read(R_PHASE) & 7) == 5, "E2 ABORT");
            check(b.d.reverse_order_o == 0, "reverse quiet in ABORT");
            check(b.read(R_F10) == 1, "F10 retained after ABORT");
            b.clear_cmd();
            check(b.read(R_F10) == 0, "CLEAR removes F10 after ABORT");
            b.write(R_F10, 1);
            check(b.read(R_F10) == 1, "F10 set before active write test");
            b.mode(1);
            b.write(R_SRC, 0x10000000ULL);
            b.write(R_DST, 0x20000000ULL);
            b.set_entry(0, src); b.set_entry(1, dst); b.mirror();
            b.arm_warm();
            check((b.read(R_PHASE) & 7) == 1, "ARM_WARM for active F10 write");
            b.write(R_F10, 1);
            check(b.read(R_F10) == 1, "F10 active write rejected, value retained");
            check(((b.read(R_E2ERR) >> 0) & 1) == 1, "F10 active write error");
            b.abort_cmd(); b.clear_cmd();
        }
        std::cout << "PASS " << checks << " research2 caps checks role=" << role << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL role=" << role << " after " << checks << " checks: " << e.what() << std::endl;
        return 1;
    }
}
