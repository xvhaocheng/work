# rv_iommu_ptw leaf-boundary regression — proposed new files

Three new files only (no production RTL, no existing tests touched):

- `tests/iommu_ptw_leaf_tb.sv` — thin wrapper around the **real** `rv_iommu_ptw`, exposing scalar config/status pins and a read-only AXI AR/R port. No hierarchy force, no FSM mirroring.
- `tests/iommu_ptw_leaf_tb.cpp` — Verilator 4.x C++ driver: clock, page-table memory model (real AXI reads), scenario map, idle-drain and late-error observation.
- `tests/test_iommu_ptw_leaf.py` — runner (`--ptw-source`, `--out`, `--scenarios`, `--build-only`) following the existing `test_iommu_ds_if_write.py` style.

## What the regression pins down

The confirmed bug (old `rv_iommu_ptw.before.sv`):

- STAGE_1 leaf: `if (main_lvl_q==LVL2) … if (main_lvl_q==LVL1) … else <ERROR>` — so **LVL3 (4 KiB) and LVL2 (2 MiB) legal leaves set `state_n=ERROR`** (line 430‑433) while `update_o.update` had already been set in the “translation completed” block (line 476‑481). Result: `error_o` and `update_o.update` together.
- STAGE_2_INTERMED leaf: same `else` shape (line 465‑468), so a **4 KiB G‑stage leaf** faults.

Fix in production (present in the current file): second `if` → `else if`, final `else` → `else if (main_lvl_q != LVL3)`; same for STAGE_2_INTERMED.

The test therefore insists on:
1. driving **real AXI AR/R** page-table fetches (no internal poke),
2. legal leaves produce **no `error_o`/`imp_err_o`**, correct `content_*.ppn` / `is_1G` / `is_2M`, exactly one `update_o.update`,
3. the walker **returns to `active_o==0`** and is watched for `DRAIN_CYCLES` afterwards to catch late/ERROR-state errors,
4. illegal `V=0` and store with `D=0` still fault and never update.

### Scenarios

| id | name | config | expectation (new) | old result |
|----|------|--------|-------------------|------------|
| 1 | `s1_4k_read` | S1 only, LVL3 4 KiB, load | no err, `ppn=0x12345`, not 2M/1G, 1 update | **FAIL** (err+update) |
| 2 | `s1_4k_write` | S1 only, LVL3 4 KiB, store | no err, 1 update | **FAIL** |
| 3 | `s1_2m_read` | S1 only, LVL2 2 MiB, load | no err, `is_2M=1`, `ppn=0x20000` | **FAIL** |
| 4 | `s1_1g_read` | S1 only, LVL1 1 GiB, load | no err, `is_1G=1`, `ppn=0x40000` | PASS (control) |
| 5 | `s1_invalid_v` | S1 only, LVL3 leaf `V=0` | error, no update | PASS |
| 6 | `s1_write_d0` | S1 only, LVL3 leaf `V\|R\|W\|A`, store, no `D` | error, no update | PASS |
| 7 | `two_stage_4k` | S1 + G-stage, all levels 4 KiB | no err, `1S.ppn=0x203`, `2S.ppn=0x1003` | **FAIL** (STAGE_2_INTERMED) |

Scenario 1 is the “legal 4K counterexample” used with `--scenarios 1`.

---

## 1. `tests/iommu_ptw_leaf_tb.sv`

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Test-only wrapper around the UNMODIFIED production rv_iommu_ptw.
//
// It instantiates the real PTW, exposes its scalar configuration/status pins
// and a read-only AXI (AR/R) page-table port, and lets the C++ driver own the
// page-table memory model.  No hierarchical signal is forced and no internal
// FSM branch is mirrored here.

`include "axi/typedef.svh"

module iommu_ptw_leaf_tb
  import rv_iommu::*;
  import rv_iommu_cfg::*;
  import axi_pkg::*;
(
    input  logic        clk_i,
    input  logic        rst_ni,

    // PTW configuration / request (driven by C++)
    input  logic        init_ptw_i,
    input  logic        en_1S_i,
    input  logic        en_2S_i,
    input  logic        is_store_i,
    input  logic        is_rx_i,
    input  logic [43:0] iosatp_ppn_i,
    input  logic [43:0] iohgatp_ppn_i,
    input  logic [63:0] iova_i,
    input  logic [19:0] pscid_i,
    input  logic [15:0] gscid_i,
    input  logic        imp_init_i,
    input  logic [43:0] pdt_gppn_i,

    // Read-only AXI memory port (AR / R)
    input  logic        ar_ready_i,
    input  logic        r_valid_i,
    input  logic [63:0] r_data_i,
    input  logic [1:0]  r_resp_i,
    output logic        ar_valid_o,
    output logic [63:0] ar_addr_o,
    output logic [3:0]  ar_id_o,
    output logic [7:0]  ar_len_o,
    output logic [2:0]  ar_size_o,
    output logic [1:0]  ar_burst_o,
    output logic        r_ready_o,

    // Status
    output logic        active_o,
    output logic        error_o,
    output logic        error_2S_o,
    output logic        error_2S_int_o,
    output logic [40:0] bad_gpaddr_o,
    output logic [11:0] cause_code_o,
    output logic        imp_done_o,
    output logic        imp_err_o,
    output logic        gpaddr_is_msi_o,
    output logic [1:0]  e3_level_o,

    // IOTLB update content (flattened for the C++ driver)
    output logic        update_valid_o,
    output logic [28:0] update_vpn_o,
    output logic        update_1S_v_o,
    output logic        update_1S_r_o,
    output logic        update_1S_w_o,
    output logic        update_1S_x_o,
    output logic        update_1S_u_o,
    output logic        update_1S_g_o,
    output logic        update_1S_is_1G_o,
    output logic        update_1S_is_2M_o,
    output logic [43:0] update_1S_ppn_o,
    output logic        update_2S_v_o,
    output logic        update_2S_r_o,
    output logic        update_2S_w_o,
    output logic        update_2S_x_o,
    output logic        update_2S_u_o,
    output logic        update_2S_g_o,
    output logic        update_2S_is_1G_o,
    output logic        update_2S_is_2M_o,
    output logic [43:0] update_2S_ppn_o
);

    typedef logic [63:0] addr_t;
    typedef logic [55:0] paddr_t;

    `AXI_TYPEDEF_AW_CHAN_T(axi_aw_chan_t, logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_W_CHAN_T (axi_w_chan_t,  logic [63:0], logic [7:0], logic [0:0])
    `AXI_TYPEDEF_B_CHAN_T (axi_b_chan_t,  logic [3:0],  logic [0:0])
    `AXI_TYPEDEF_AR_CHAN_T(axi_ar_chan_t, logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_R_CHAN_T (axi_r_chan_t,  logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_REQ_T    (axi_req_t,     axi_aw_chan_t, axi_w_chan_t, axi_ar_chan_t)
    `AXI_TYPEDEF_RESP_T   (axi_resp_t,    axi_b_chan_t,  axi_r_chan_t)

    localparam rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::rv_iommu_cfg_t'{
        NumOutstandingTrans : 1,
        WFifoDepth          : 1,
        MrifFifoDepth       : 1,
        FaultFifoDepth      : 1,
        NumIotlbEntries     : 16,
        NumDdtcEntries      : 8,
        NumPdtcEntries      : 1,
        NumMrifcEntries     : 1,
        InclPC              : 1'b0,
        InclAxiBC           : 1'b0,
        InclDbg             : 1'b0,
        InclATS             : 1'b0,
        MSITrans            : rv_iommu_cfg::MSI_DISABLED,
        IGS                 : rv_iommu_cfg::MSI_ONLY,
        NumIntVec           : 1,
        NumHpmCounters      : 0,
        PAddrWidth          : 56,
        AxiAddrWidth        : 64,
        AxiDataWidth        : 64,
        AxiIdWidth          : 4,
        AxiProgIdWidth      : 4,
        AxiUserWidth        : 1,
        AxiProIdWidth       : 1,
        AxiDevIdWidth       : 1,
        AxisDataWidth       : 64,
        AxisDestWidth       : 1,
        AxisUserWidth       : 1,
        AxisStrbWidth       : 8,
        AxisKeepWidth       : 8,
        AxisIdWidth         : 4,
        Freq                : 1
    };

    axi_req_t  ptw_req;
    axi_resp_t ptw_resp;
    iotlb_up_t ptw_update;

    // Read-only memory: only AR/R are used.  '0 the whole struct first so that
    // fields not driven here are deterministic.
    always_comb begin
        ptw_resp              = '0;
        ptw_resp.ar_ready     = ar_ready_i;
        ptw_resp.r_valid      = r_valid_i;
        ptw_resp.r.data       = r_data_i;
        ptw_resp.r.resp       = r_resp_i;
        ptw_resp.r.last       = 1'b1;
        ptw_resp.r.id         = '0;
    end

    assign ar_valid_o = ptw_req.ar_valid;
    assign ar_addr_o  = ptw_req.ar.addr;
    assign ar_id_o    = ptw_req.ar.id;
    assign ar_len_o   = ptw_req.ar.len;
    assign ar_size_o  = ptw_req.ar.size;
    assign ar_burst_o = ptw_req.ar.burst;
    assign r_ready_o  = ptw_req.r_ready;

    assign update_valid_o    = ptw_update.update;
    assign update_vpn_o      = ptw_update.vpn;
    assign update_1S_v_o     = ptw_update.content.content_1S.v;
    assign update_1S_r_o     = ptw_update.content.content_1S.r;
    assign update_1S_w_o     = ptw_update.content.content_1S.w;
    assign update_1S_x_o     = ptw_update.content.content_1S.x;
    assign update_1S_u_o     = ptw_update.content.content_1S.u;
    assign update_1S_g_o     = ptw_update.content.content_1S.g;
    assign update_1S_is_1G_o = ptw_update.content.content_1S.is_1G;
    assign update_1S_is_2M_o = ptw_update.content.content_1S.is_2M;
    assign update_1S_ppn_o   = ptw_update.content.content_1S.ppn;
    assign update_2S_v_o     = ptw_update.content.content_2S.v;
    assign update_2S_r_o     = ptw_update.content.content_2S.r;
    assign update_2S_w_o     = ptw_update.content.content_2S.w;
    assign update_2S_x_o     = ptw_update.content.content_2S.x;
    assign update_2S_u_o     = ptw_update.content.content_2S.u;
    assign update_2S_g_o     = ptw_update.content.content_2S.g;
    assign update_2S_is_1G_o = ptw_update.content.content_2S.is_1G;
    assign update_2S_is_2M_o = ptw_update.content.content_2S.is_2M;
    assign update_2S_ppn_o   = ptw_update.content.content_2S.ppn;

    rv_iommu_ptw #(
        .RVIOMMUCfg (RVIOMMUCfg),
        .addr_t     (addr_t),
        .paddr_t    (paddr_t),
        .axi_req_t  (axi_req_t),
        .axi_resp_t (axi_resp_t)
    ) i_ptw (
        .clk_i,
        .rst_ni,
        .init_ptw_i,
        .active_o,
        .error_o,
        .error_2S_o,
        .error_2S_int_o,
        .bad_gpaddr_o,
        .cause_code_o,
        .en_1S_i,
        .en_2S_i,
        .is_store_i,
        .is_rx_i,
        .iosatp_ppn_i,
        .iohgatp_ppn_i,
        .iova_i            (addr_t'(iova_i)),
        .pscid_i,
        .gscid_i,
        .update_o          (ptw_update),
        .msi_en_i          (1'b0),
        .msi_addr_mask_i   ('0),
        .msi_addr_pattern_i('0),
        .gpaddr_is_msi_o,
        .msi_1S_content_o  (),
        .imp_init_i,
        .pdt_gppn_i,
        .imp_done_o,
        .imp_err_o,
        .mem_resp_i        (ptw_resp),
        .mem_req_o         (ptw_req),
        .e3_level_o
    );

endmodule
```

---

## 2. `tests/iommu_ptw_leaf_tb.cpp`

```cpp
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
```

---

## 3. `tests/test_iommu_ptw_leaf.py`

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""
Verilate and run the bounded rv_iommu_ptw leaf-boundary regression.

Builds the UNMODIFIED production
cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv together with
tests/iommu_ptw_leaf_tb.sv and tests/iommu_ptw_leaf_tb.cpp using the Verilator
4.x --cc --exe --build flow with an explicit C++ clock driver.  No --binary and
no --timing; no `timescale is added (avoids TIMESCALEMOD).

The DUT is the real PTW; the page-table memory is a real read-only AXI AR/R port
driven from C++.  No FSM signal is forced and no internal branch is mirrored.

Usage
-----
    # production RTL (after the leaf fix) -- all scenarios must PASS
    python3 tests/test_iommu_ptw_leaf.py

    # saved "before" baseline copy, isolated output directory
    python3 tests/test_iommu_ptw_leaf.py \\
        --ptw-source out/composition-ptwfix-20261005/rv_iommu_ptw.before.sv \\
        --out out/iommu-ptw-leaf-before

    # only the legal 4 KiB read counterexample (scenario 1)
    python3 tests/test_iommu_ptw_leaf.py --scenarios 1

Exit status is nonzero if the build fails or if any scenario reports a mismatch.
"""

import argparse
import os
import subprocess
import sys
from pathlib import Path

ROOT   = Path(__file__).resolve().parents[1]
IOMMU  = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
VENDOR = IOMMU / "vendor"

DEFAULT_PTW = IOMMU / "rtl" / "translation_logic" / "rv_iommu_ptw.sv"
DEFAULT_OUT = ROOT / "out" / "iommu-ptw-leaf"

TB     = ROOT / "tests" / "iommu_ptw_leaf_tb.sv"
TB_CPP = ROOT / "tests" / "iommu_ptw_leaf_tb.cpp"
TOP    = "iommu_ptw_leaf_tb"

COMMON = VENDOR / "common_cells" / "src"
AXI    = VENDOR / "axi" / "src"
PKG    = IOMMU / "packages"

# Exactly the modules/packages needed by rv_iommu_ptw.sv and its wrapper.
SOURCES = [
    COMMON / "cf_math_pkg.sv",
    AXI    / "axi_pkg.sv",
    PKG    / "rv_iommu_cfg_pkg.sv",
    PKG    / "rv_iommu_reg_pkg.sv",
    PKG    / "rv_iommu_pkg.sv",
]

INCDIRS = [
    VENDOR / "axi" / "include",
    VENDOR / "common_cells" / "include",
]

DEFINES = ["+define+ASSERTS_OFF"]

ALL_SCENARIOS = ["1", "2", "3", "4", "5", "6", "7"]


def require(path: Path) -> Path:
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def tail(path: Path, lines: int = 240) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def find_exe(obj_dir: Path, top: str):
    cand = obj_dir / ("V" + top)
    if cand.is_file() and os.access(str(cand), os.X_OK):
        return cand
    return None


def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    ap.add_argument("--ptw-source", type=Path, default=DEFAULT_PTW,
                    help="rv_iommu_ptw.sv under test (default: production RTL); "
                         "pass the saved before.sv to reproduce the baseline")
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT,
                    help="output directory (obj dir + logs); use distinct "
                         "directories to isolate old vs new compiles")
    ap.add_argument("--scenarios", default=",".join(ALL_SCENARIOS),
                    help="comma-separated scenario list (default all)")
    ap.add_argument("--build-only", action="store_true",
                    help="compile the model but do not run any scenario")
    opts = ap.parse_args()

    ptw_source = require(opts.ptw_source.resolve())
    tb         = require(TB)
    tb_cpp     = require(TB_CPP)
    out_dir    = opts.out.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)
    obj_dir    = out_dir / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    scenarios = [s.strip() for s in opts.scenarios.split(",") if s.strip()]
    if not scenarios:
        sys.stderr.write("empty scenario list\n")
        return 1
    bad = [s for s in scenarios if s not in ALL_SCENARIOS]
    if bad:
        sys.stderr.write("unknown scenario(s): {}\n".format(",".join(bad)))
        return 1

    for inc in INCDIRS:
        if not inc.exists():
            sys.exit("required include directory missing: {}".format(inc))

    sources = [require(p) for p in SOURCES] + [ptw_source, tb]

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wall",
        "-Wno-fatal",
        "--top-module", TOP,
        "--Mdir", str(obj_dir),
    ] + DEFINES
    for inc in INCDIRS:
        args += ["-I{}".format(inc)]
    args += [str(p) for p in sources]
    args += [str(tb_cpp)]

    build_log = out_dir / "build.log"
    print("test_iommu_ptw_leaf: ptw_source={}".format(ptw_source))
    print("test_iommu_ptw_leaf: out={}".format(out_dir))
    print("test_iommu_ptw_leaf: build cmd: {} ...".format(" ".join(args[:8])))

    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator build FAILED (rc={})\n".format(build.returncode))
        return 1

    exe = find_exe(obj_dir, TOP)
    if exe is None:
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce an executable for {}\n".format(TOP))
        return 1
    print("test_iommu_ptw_leaf: exe={}".format(exe))

    if opts.build_only:
        return 0

    results = []
    for sc in scenarios:
        run_log  = out_dir / "run-scenario-{}.log".format(sc)
        run_args = [str(exe), "+scenario={}".format(sc)]
        print("test_iommu_ptw_leaf: running {}".format(" ".join(run_args)))
        with run_log.open("w") as log:
            run = subprocess.run(run_args, stdout=log, stderr=subprocess.STDOUT)
        sys.stdout.write(tail(run_log, 40))
        results.append((sc, run.returncode))
        if run.returncode != 0:
            print("test_iommu_ptw_leaf: scenario {} FAILED (rc={})".format(sc, run.returncode))

    rc = 0
    print("test_iommu_ptw_leaf: summary")
    for sc, r in results:
        status = "PASS" if r == 0 else "FAIL(rc={})".format(r)
        print("  scenario {} -> {}".format(sc, status))
        if r != 0:
            rc = 1
    return rc


if __name__ == "__main__":
    sys.exit(main())
```

---

## Acceptance commands (relative to platform root)

```bash
# 1) production (already-fixed) RTL: every scenario must PASS
python3 tests/test_iommu_ptw_leaf.py

# 2) baseline reproduction with the saved "before" source in an isolated dir:
#    scenarios 1,2,3 and 7 must FAIL; 4,5,6 must PASS
python3 tests/test_iommu_ptw_leaf.py \
    --ptw-source out/composition-ptwfix-20261005/rv_iommu_ptw.before.sv \
    --out out/iommu-ptw-leaf-before

# 3) the legal 4 KiB read counterexample alone:
#    production -> PASS, baseline -> FAIL
python3 tests/test_iommu_ptw_leaf.py --scenarios 1
python3 tests/test_iommu_ptw_leaf.py --scenarios 1 \
    --ptw-source out/composition-ptwfix-20261005/rv_iommu_ptw.before.sv \
    --out out/iommu-ptw-leaf-before
```

Expected:

- Production: `scenario 1..7 -> PASS`.
- Baseline: 1, 2, 3, 7 `-> FAIL`; 4, 5, 6 `-> PASS`.
  - 1/2/3 fail because the walker enters ERROR while `update_o.update` is high (the old `else` bug). The checker sees `error_o` in a legal scenario.
  - 7 fails first at the G‑stage 4 KiB leaf (STAGE_2_INTERMED `else` bug) — an error with no update.
  - 4 is the LVL1 control (old code already correct).

---

## Assumptions / notes for Codex

1. **Paths** are relative to the platform root; `cva6/corev_apu/riscv-iommu/...` vendor trees are assumed to be present exactly as used by `tests/test_composition_tl.py` (`vendor/common_cells/src`, `vendor/axi/src`, `vendor/axi/include`, `vendor/common_cells/include`).
2. The old G-stage root address formation `{iohgatp_ppn[PPNW-1:2], gpa[GPLEN39-1:30], 3'b0}` drops `iohgatp_ppn[1:0]`; the test therefore uses `iohgatp_ppn = 0x100` (low two bits zero) so both the truncated and the natural address agree. This is **not** part of the assigned fix; it is simply a test construction choice.
3. The page-table memory is a real AXI AR/R slave driven from C++ (`ar_ready` always high, one registered `r_valid` per accepted AR). It mirrors only the *page-table format*, not the DUT FSM.
4. Termination rule: watch `active_o` go high, require `active_o == 0` for `DRAIN_CYCLES=16` consecutive cycles; `error_o`/`imp_err_o`/`update_o` are sampled in the low phase of every cycle, so a one-cycle ERROR that coincides with `update` is observed.
5. Config: `PAddrWidth=56`, `AxiAddrWidth=64`, `AxiDataWidth=64`, `MSITrans=MSI_DISABLED`, `AxiIdWidth=4`, mirroring `tests/composition_tl_top.sv`.
6. Scenario 7’s complete map is documented inline in `build_scenario`. 15 AR beats are expected (three 3-level G-stage walks + three S1 fetches + one final G-stage walk).
7. Not executed here — the above is a proposal; Codex should run the acceptance commands and confirm. If `-Wall` produces fatal errors specific to this Verilator build, keep `-Wno-fatal` (already present) or add the minimal matching `-Wno-…` used elsewhere; do not switch to `--binary`/`--timing` or add `` `timescale``.
8. No production RTL or pre-existing test was modified; the three files above are the only additions.