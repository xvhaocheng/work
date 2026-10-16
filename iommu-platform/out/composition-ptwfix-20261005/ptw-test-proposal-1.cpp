// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Verilator 4.x driver for tests/iommu_ptw_leaf_tb.sv.
//
// Drives the real, unmodified rv_iommu_ptw through a real read-only AXI AR/R
// page-table port.  The page-table memory model lives in this file; the SV
// wrapper only exposes scalar PTW pins.  No hierarchical force into the PTW
// and no re-implementation of its internal branches.
//
// One process run handles one scenario selected with +scenario=<id>; with no
// plusarg it runs all defined scenarios and returns nonzero on any mismatch.

#include "Viommu_ptw_leaf_tb.h"
#include "verilated.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

static vluint64_t sim_time = 0;
double sc_time_stamp() { return static_cast<double>(sim_time); }

// ---------------------------------------------------------------- PTE coding
// RISC-V Sv39 `pte_t` layout used by rv_iommu::pte_t:
//   [63:54] reserved, [53:10] ppn, [9:8] rsw, [7] d, [6] a, [5] g,
//   [4] u, [3] x, [2] w, [1] r, [0] v
enum {
    PTE_V = 1u << 0,
    PTE_R = 1u << 1,
    PTE_W = 1u << 2,
    PTE_X = 1u << 3,
    PTE_U = 1u << 4,
    PTE_G = 1u << 5,
    PTE_A = 1u << 6,
    PTE_D = 1u << 7,
};

static uint64_t make_pte(uint64_t ppn, uint64_t flags) {
    return ((ppn & 0xFFFFFFFFFFFULL) << 10) | (flags & 0x3FFULL);
}

// --------------------------------------------------------------- constants
static const int      MAX_CYCLES   = 1000;
static const int      DRAIN_CYCLES = 16;
static const int      RESET_CYCLES = 8;
static const uint64_t IOVA         = 0x0000000000201234ULL;  // vpn2=0 vpn1=1 vpn0=1 off=0x234

static uint64_t tbl_entry(uint64_t table_ppn, unsigned shift) {
    return (table_ppn << 12) | (((IOVA >> shift) & 0x1FF) << 3);
}

// --------------------------------------------------------------- data types
struct MemModel {
    std::map<uint64_t, uint64_t> ptes;
    void put(uint64_t addr, uint64_t pte) { ptes[addr & ~0x7ULL] = pte; }
    uint64_t get(uint64_t addr) const {
        std::map<uint64_t, uint64_t>::const_iterator it = ptes.find(addr & ~0x7ULL);
        return (it == ptes.end()) ? 0ULL : it->second;
    }
};

struct Scenario {
    int         id = 0;
    std::string name;
    bool        en_1S = true;
    bool        en_2S = false;
    bool        is_store = false;
    bool        expect_error = false;
    uint64_t    iosatp_ppn = 0;
    uint64_t    iohgatp_ppn = 0;
    bool        chk_1S_1G = false;
    bool        chk_1S_2M = false;
    uint64_t    exp_1S_ppn = 0;
    bool        chk_2S = false;
    bool        chk_2S_1G = false;
    bool        chk_2S_2M = false;
    uint64_t    exp_2S_ppn = 0;
    MemModel    mem;
};

struct Observed {
    int      updates = 0;
    int      errors = 0;
    int      imp_errors = 0;
    int      error_2s = 0;
    int      error_2s_int = 0;
    int      ar_count = 0;
    bool     saw_active = false;
    bool     idle_reached = false;
    int      idle_streak = 0;
    uint64_t u1_ppn = 0;
    bool     u1_1g = false, u1_2m = false;
    uint64_t u2_ppn = 0;
    bool     u2_1g = false, u2_2m = false;
    uint16_t cause = 0;
};

// --------------------------------------------------------------- scenario map
static bool build_scenario(int id, Scenario& sc) {
    sc.id = id;
    switch (id) {
        case 1: {
            sc.name = "s1_4k_read";
            sc.en_1S = true; sc.en_2S = false; sc.is_store = false;
            sc.iosatp_ppn = 0x400;
            sc.exp_1S_ppn = 0x12345;
            sc.mem.put(tbl_entry(0x400, 30), make_pte(0x401, PTE_V));
            sc.mem.put(tbl_entry(0x401, 21), make_pte(0x402, PTE_V));
            sc.mem.put(tbl_entry(0x402, 12), make_pte(0x12345, PTE_V|PTE_R|PTE_W|PTE_A|PTE_D));
            return true;
        }
        case 2: {
            sc.name = "s1_4k_write";
            sc.en_1S = true; sc.en_2S = false; sc.is_store = true;
            sc.iosatp_ppn = 0x400;
            sc.exp_1S_ppn = 0x12345;
            sc.mem.put(tbl_entry(0x400, 30), make_pte(0x401, PTE_V));
            sc.mem.put(tbl_entry(0x401, 21), make_pte(0x402, PTE_V));
            sc.mem.put(tbl_entry(0x402, 12), make_pte(0x12345, PTE_V|PTE_R|PTE_W|PTE_A|PTE_D));
            return true;
        }
        case 3: {
            sc.name = "s1_2m_read";
            sc.en_1S = true; sc.en_2S = false; sc.is_store = false;
            sc.chk_1S_2M = true;
            sc.iosatp_ppn = 0x400;
            sc.exp_1S_ppn = 0x20000;
            sc.mem.put(tbl_entry(0x400, 30), make_pte(0x401, PTE_V));
            sc.mem.put(tbl_entry(0x401, 21), make_pte(0x20000, PTE_V|PTE_R|PTE_W|PTE_A|PTE_D));
            return true;
        }
        case 4: {
            sc.name = "s1_1g_read";
            sc.en_1S = true; sc.en_2S = false; sc.is_store = false;
            sc.chk_1S_1G = true;
            sc.iosatp_ppn = 0x400;
            sc.exp_1S_ppn = 0x40000;
            sc.mem.put(tbl_entry(0x400, 30), make_pte(0x40000, PTE_V|PTE_R|PTE_W|PTE_A|PTE_D));
            return true;
        }
        case 5: {
            sc.name = "s1_invalid_v";
            sc.en_1S = true; sc.en_2S = false; sc.is_store = false;
            sc.expect_error = true;
            sc.iosatp_ppn = 0x400;
            sc.mem.put(tbl_entry(0x400, 30), make_pte(0x401, PTE_V));
            sc.mem.put(tbl_entry(0x401, 21), make_pte(0x402, PTE_V));
            sc.mem.put(tbl_entry(0x402, 12), make_pte(0x12345, PTE_R|PTE_W|PTE_A|PTE_D)); // V=0
            return true;
        }
        case 6: {
            sc.name = "s1_write_d0";
            sc.en_1S = true; sc.en_2S = false; sc.is_store = true;
            sc.expect_error = true;
            sc.iosatp_ppn = 0x400;
            sc.mem.put(tbl_entry(0x400, 30), make_pte(0x401, PTE_V));
            sc.mem.put(tbl_entry(0x401, 21), make_pte(0x402, PTE_V));
            sc.mem.put(tbl_entry(0x402, 12), make_pte(0x12345, PTE_V|PTE_R|PTE_W|PTE_A)); // D=0
            return true;
        }
        case 7: {
            // Full two-stage Sv39/Sv39x4 walk, all levels 4 KiB.
            //
            // Guest memory map (all within the same 2 MiB G-region
            // 0x20_0000 - 0x3f_ffff, so the G-stage L1/L2 tables are shared):
            //   S1 root table GPA 0x200000 -> PA 0x1000000
            //   S1 L2   table GPA 0x201000 -> PA 0x1001000
            //   S1 L3   table GPA 0x202000 -> PA 0x1002000
            //   final GPA       0x203234   -> PA 0x1003234
            // with iova = 0x201234 (vpn2=0 vpn1=1 vpn0=1 off=0x234).
            //
            // G-stage tables:
            //   iohgatp_ppn=0x100 -> G root @ PA 0x100000
            //   G L2 L1 entry[0]  -> ppn 0x101
            //   G L3 L2 entry[1]  -> ppn 0x102
            //   G L3 leaves idx 0..3 -> ppn 0x1000..0x1003
            sc.name = "two_stage_4k";
            sc.en_1S = true; sc.en_2S = true; sc.is_store = false;
            sc.iosatp_ppn = 0x200;
            sc.iohgatp_ppn = 0x100;
            sc.chk_2S = true; sc.chk_2S_1G = false; sc.chk_2S_2M = false;
            sc.exp_1S_ppn = 0x203;
            sc.exp_2S_ppn = 0x1003;

            // G-stage tables (host physical)
            sc.mem.put(0x100000, make_pte(0x101, PTE_V));                        // root -> L2
            sc.mem.put(0x101008, make_pte(0x102, PTE_V));                        // L2  -> L3
            sc.mem.put(0x102000, make_pte(0x1000, PTE_V|PTE_R|PTE_W|PTE_U|PTE_A|PTE_D));
            sc.mem.put(0x102008, make_pte(0x1001, PTE_V|PTE_R|PTE_W|PTE_U|PTE_A|PTE_D));
            sc.mem.put(0x102010, make_pte(0x1002, PTE_V|PTE_R|PTE_W|PTE_U|PTE_A|PTE_D));
            sc.mem.put(0x102018, make_pte(0x1003, PTE_V|PTE_R|PTE_W|PTE_U|PTE_A|PTE_D));

            // S1 tables (guest physical, reached through the G-stage map)
            sc.mem.put(0x1000000, make_pte(0x201, PTE_V));                       // root -> L2
            sc.mem.put(0x1001008, make_pte(0x202, PTE_V));                       // L2  -> L3
            sc.mem.put(0x1002008, make_pte(0x203, PTE_V|PTE_R|PTE_W|PTE_A|PTE_D)); // S1 4K leaf
            return true;
        }
    }
    return false;
}

// ------------------------------------------------------------------- driver
static void tick_plain(Viommu_ptw_leaf_tb* top) {
    top->clk_i = 0; ++sim_time; top->eval();
    top->clk_i = 1; ++sim_time; top->eval();
}

static void drive_cycle(Viommu_ptw_leaf_tb* top, const MemModel& mem,
                        bool& r_active, uint64_t& r_data, Observed& obs) {
    // Low phase: present the registered read response, then sample outputs.
    top->clk_i    = 0;
    top->ar_ready_i = 1;
    top->r_valid_i  = r_active ? 1 : 0;
    top->r_data_i   = r_data;
    top->r_resp_i   = 0; // RESP_OKAY
    ++sim_time;
    top->eval();

    // -------- observe current-cycle combinational outputs --------
    if (top->active_o) {
        obs.saw_active = true;
        obs.idle_streak = 0;
    } else if (obs.saw_active) {
        obs.idle_streak++;
    }
    if (top->error_o)   { obs.errors++;   obs.cause = (uint16_t)top->cause_code_o; }
    if (top->imp_err_o) { obs.imp_errors++; }
    if (top->error_2S_o)     { obs.error_2s++; }
    if (top->error_2S_int_o) { obs.error_2s_int++; }
    if (top->update_valid_o) {
        obs.updates++;
        obs.u1_ppn = top->update_1S_ppn_o;
        obs.u1_1g  = (top->update_1S_is_1G_o != 0);
        obs.u1_2m  = (top->update_1S_is_2M_o != 0);
        obs.u2_ppn = top->update_2S_ppn_o;
        obs.u2_1g  = (top->update_2S_is_1G_o != 0);
        obs.u2_2m  = (top->update_2S_is_2M_o != 0);
    }

    // -------- capture AR / R handshakes and queue next response --------
    bool ar_fire = (top->ar_valid_o != 0) && (top->ar_ready_i != 0);
    bool r_fire  = r_active && (top->r_ready_o != 0);
    if (ar_fire) obs.ar_count++;

    bool     next_active = r_active;
    uint64_t next_data   = r_data;
    if (ar_fire) { next_active = true; next_data = mem.get(top->ar_addr_o); }
    if (r_fire)  { next_active = false; }

    // High phase (posedge).
    top->clk_i = 1;
    ++sim_time;
    top->eval();

    r_active = next_active;
    r_data   = next_data;
}

static bool run_scenario(Viommu_ptw_leaf_tb* top, const Scenario& sc, Observed& obs) {
    // Reset.
    top->rst_ni       = 0;
    top->init_ptw_i   = 0;
    top->en_1S_i      = sc.en_1S ? 1 : 0;
    top->en_2S_i      = sc.en_2S ? 1 : 0;
    top->is_store_i   = sc.is_store ? 1 : 0;
    top->is_rx_i      = 0;
    top->iosatp_ppn_i = (uint64_t)sc.iosatp_ppn;
    top->iohgatp_ppn_i= (uint64_t)sc.iohgatp_ppn;
    top->iova_i       = IOVA;
    top->pscid_i      = 0;
    top->gscid_i      = 0;
    top->imp_init_i   = 0;
    top->pdt_gppn_i   = 0;
    top->ar_ready_i   = 1;
    top->r_valid_i    = 0;
    top->r_data_i     = 0;
    top->r_resp_i     = 0;
    top->clk_i        = 0;
    ++sim_time;
    top->eval();

    for (int i = 0; i < RESET_CYCLES; ++i) { tick_plain(top); }
    top->rst_ni = 1;
    top->clk_i  = 0;
    ++sim_time;
    top->eval();

    // Kick the walk.
    bool     r_active = false;
    uint64_t r_data   = 0;
    top->init_ptw_i = 1;

    for (int i = 0; i < MAX_CYCLES; ++i) {
        drive_cycle(top, sc.mem, r_active, r_data, obs);
        if (i == 1) top->init_ptw_i = 0;
        if (obs.saw_active && obs.idle_streak >= DRAIN_CYCLES) {
            obs.idle_reached = true;
            break;
        }
    }
    return obs.idle_reached;
}

static bool check_scenario(const Scenario& sc, const Observed& obs, std::string& msg) {
    char buf[512];
    if (!obs.idle_reached) {
        std::snprintf(buf, sizeof(buf),
                      "walker never returned to IDLE (AR reads=%d)", obs.ar_count);
        msg = buf; return false;
    }

    if (sc.expect_error) {
        if (obs.errors == 0) {
            std::snprintf(buf, sizeof(buf),
                          "expected a fault but error_o never asserted (AR reads=%d)",
                          obs.ar_count);
            msg = buf; return false;
        }
        if (obs.updates != 0) {
            std::snprintf(buf, sizeof(buf),
                          "illegal translation produced %d IOTLB update(s)", obs.updates);
            msg = buf; return false;
        }
        return true;
    }

    // Legal translation.
    if (obs.errors != 0) {
        std::snprintf(buf, sizeof(buf),
                      "unexpected error_o (count=%d cause=0x%03x) -- a legal page "
                      "must not fault (old bug: ERROR + update together)",
                      obs.errors, (unsigned)obs.cause);
        msg = buf; return false;
    }
    if (obs.imp_errors != 0) {
        std::snprintf(buf, sizeof(buf), "unexpected imp_err_o (count=%d)", obs.imp_errors);
        msg = buf; return false;
    }
    if (obs.updates != 1) {
        std::snprintf(buf, sizeof(buf), "expected exactly 1 IOTLB update, got %d", obs.updates);
        msg = buf; return false;
    }

    if (sc.chk_1S_1G && !obs.u1_1g) { msg = "update content_1S.is_1G not set"; return false; }
    if (sc.chk_1S_2M && !obs.u1_2m) { msg = "update content_1S.is_2M not set"; return false; }
    if (!sc.chk_1S_1G && !sc.chk_1S_2M && (obs.u1_1g || obs.u1_2m)) {
        msg = "update content_1S page size is not 4 KiB"; return false;
    }
    if (obs.u1_ppn != sc.exp_1S_ppn) {
        std::snprintf(buf, sizeof(buf),
                      "content_1S.ppn=0x%llx expected 0x%llx",
                      (unsigned long long)obs.u1_ppn, (unsigned long long)sc.exp_1S_ppn);
        msg = buf; return false;
    }

    if (sc.chk_2S) {
        if (sc.chk_2S_1G && !obs.u2_1g) { msg = "update content_2S.is_1G not set"; return false; }
        if (sc.chk_2S_2M && !obs.u2_2m) { msg = "update content_2S.is_2M not set"; return false; }
        if (!sc.chk_2S_1G && !sc.chk_2S_2M && (obs.u2_1g || obs.u2_2m)) {
            msg = "update content_2S page size is not 4 KiB"; return false;
        }
        if (obs.u2_ppn != sc.exp_2S_ppn) {
            std::snprintf(buf, sizeof(buf),
                          "content_2S.ppn=0x%llx expected 0x%llx",
                          (unsigned long long)obs.u2_ppn, (unsigned long long)sc.exp_2S_ppn);
            msg = buf; return false;
        }
    }
    return true;
}

static const int NUM_SCENARIOS = 7;

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    int want = 0;
    for (int i = 1; i < argc; ++i) {
        if (std::strncmp(argv[i], "+scenario=", 10) == 0) {
            want = std::atoi(argv[i] + 10);
        }
    }

    Viommu_ptw_leaf_tb* top = new Viommu_ptw_leaf_tb;

    int rc    = 0;
    int first = (want == 0) ? 1 : want;
    int last  = (want == 0) ? NUM_SCENARIOS : want;

    for (int id = first; id <= last; ++id) {
        Scenario sc;
        if (!build_scenario(id, sc)) {
            std::fprintf(stderr, "ptw_leaf_tb: unknown scenario %d\n", id);
            rc = 1;
            continue;
        }
        Observed obs;
        bool ran = run_scenario(top, sc, obs);
        std::string msg;
        bool ok = ran && check_scenario(sc, obs, msg);
        std::printf("scenario %d (%s): %s [updates=%d errors=%d imp_err=%d AR=%d]\n",
                    id, sc.name.c_str(), ok ? "PASS" : "FAIL",
                    obs.updates, obs.errors, obs.imp_errors, obs.ar_count);
        if (!ok) {
            if (ran) std::printf("    reason: %s\n", msg.c_str());
            rc = 1;
        }
    }

    top->final();
    delete top;
    return rc;
}
