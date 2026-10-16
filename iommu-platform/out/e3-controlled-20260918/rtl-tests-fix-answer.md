Below are complete proposed test-only fixes. No production RTL is changed.

## Handshake / behavior summary

- `idma_reg64_frontend.sv:270-272` requires a real `burst_req_t`; the pair top was leaving it as `logic`, which is why `length` failed. The fix uses `IDMA_TYPEDEF_FULL_REQ_T` exactly like `tests/idma_frontend_launch_top.sv:29`.
- A frontend private `AxID` is `0` in the production top; the pair top had `4'd1`. Fixed to `4'd0`. The E3 select tap remains ID `4`, DID `11` (`dma_controlled_launcher.sv:60-61`).
- Controlled reserve is a CPU read of `0x28` while `launch_hold_i=1` (`idma_reg64_frontend.sv:196-197`, `416-432`). The CPU helper now:
  1. drives request,
  2. evaluates,
  3. waits for `ready`,
  4. checks `error`,
  5. applies exactly one `posedge`,
  6. deasserts `valid`,
  7. evaluates.
- Backend accept is exactly one `ready` pulse while `valid_o` is high. The helper drives `backend_ready_i`, evaluates, waits for `valid_o`, then applies exactly one accepting edge and drops `ready`.
- All `ready`/`valid` waits are timeout-bounded.
- `hold_mask` is no longer expected to clear on issue. Tests assert ownership only through terminal/freeze behavior, not early `frontend_held_o==0`.
- Normal completion now checks upper-32 errors, issued/done/cancel masks, counts, per-lane IDs, and per-lane timestamps.
- Finite observer test removes the erroneous `1<<8` victim-port-1 bit from all commands, uses V port `0` and A port `1`, drives `batch_active_i[1]` initially and then drops it while V is active, and checks V done/poll freeze.

---

## `tests/idma_controlled_pair_top.sv`

```systemverilog
`include "common_cells/registers.svh"
`include "idma/typedef.svh"

module idma_controlled_pair_top (
    input  logic clk_i, rst_ni,
    // CPU config for V frontend (port 0)
    input  logic v_cpu_valid_i, v_cpu_write_i,
    input  logic [31:0] v_cpu_addr_i,
    input  logic [63:0] v_cpu_wdata_i,
    input  logic [7:0] v_cpu_wstrb_i,
    output logic v_cpu_ready_o,
    output logic [63:0] v_cpu_rdata_o,
    output logic v_cpu_error_o,
    // CPU config for A frontend (port 1)
    input  logic a_cpu_valid_i, a_cpu_write_i,
    input  logic [31:0] a_cpu_addr_i,
    input  logic [63:0] a_cpu_wdata_i,
    input  logic [7:0] a_cpu_wstrb_i,
    output logic a_cpu_ready_o,
    output logic [63:0] a_cpu_rdata_o,
    output logic a_cpu_error_o,
    // Backend controls/outputs for V
    input  logic v_backend_ready_i,
    input  logic v_backend_idle_i,
    input  logic v_trans_complete_i,
    output logic v_valid_o,
    output logic [63:0] v_burst_src_addr_o,
    output logic [63:0] v_burst_dst_addr_o,
    output logic [63:0] v_burst_length_o,
    output logic [63:0] v_issue_id_o,
    output logic [63:0] v_done_id_o,
    output logic v_obs_issue_o,
    output logic v_obs_done_o,
    // Backend controls/outputs for A
    input  logic a_backend_ready_i,
    input  logic a_backend_idle_i,
    input  logic a_trans_complete_i,
    output logic a_valid_o,
    output logic [63:0] a_burst_src_addr_o,
    output logic [63:0] a_burst_dst_addr_o,
    output logic [63:0] a_burst_length_o,
    output logic [63:0] a_issue_id_o,
    output logic [63:0] a_done_id_o,
    output logic a_obs_issue_o,
    output logic a_obs_done_o,
    // Launcher status/select
    input  logic selected_i,
    input  logic select_write_i,
    input  logic [7:0] select_id_i,
    input  logic [23:0] select_did_i,
    input  logic [63:0] select_iova_i,
    output logic [3:0] launch_hold_o,
    output logic [3:0] launch_go_o,
    output logic [3:0] launch_cancel_o,
    output logic [3:0] frontend_held_o,
    output logic [3:0] frontend_active_o,
    output logic [3:0] frontend_cancelled_o,
    output logic [3:0] frontend_error_o,
    output logic [63:0] launcher_rdata_o,
    output logic launcher_valid_o,
    output logic launcher_write_o,
    output logic [7:0] launcher_addr_o
);
    typedef struct packed {
        logic        valid;
        logic        write;
        logic [31:0] addr;
        logic [63:0] wdata;
        logic [7:0]  wstrb;
    } dma_req_t;

    typedef struct packed {
        logic        ready;
        logic [63:0] rdata;
        logic        error;
    } dma_rsp_t;

    typedef logic [63:0] addr_t;
    typedef logic [3:0]  id_t;
    `IDMA_TYPEDEF_FULL_REQ_T(burst_t, id_t, addr_t, addr_t)

    dma_req_t v_req, a_req;
    dma_rsp_t v_rsp, a_rsp;
    burst_t v_burst_req, a_burst_req;

    logic v_obs_cfg_valid, v_obs_cfg_write;
    logic [7:0] v_obs_cfg_addr;
    logic [63:0] v_obs_cfg_wdata, v_obs_cfg_rdata;
    logic [7:0] v_obs_cfg_wstrb;
    logic a_obs_cfg_valid, a_obs_cfg_write;
    logic [7:0] a_obs_cfg_addr;
    logic [63:0] a_obs_cfg_wdata, a_obs_cfg_rdata;
    logic [7:0] a_obs_cfg_wstrb;

    logic v_launch_held, v_launch_active, v_launch_cancelled, v_launch_error;
    logic a_launch_held, a_launch_active, a_launch_cancelled, a_launch_error;
    logic [3:0] fe_held, fe_active, fe_cancelled, fe_error;
    logic [3:0] launcher_hold, launcher_go, launcher_cancel;
    logic [63:0] launcher_issue_id [4];
    logic [63:0] launcher_done_id [4];

    assign v_req.valid = v_cpu_valid_i;
    assign v_req.write = v_cpu_write_i;
    assign v_req.addr = v_cpu_addr_i;
    assign v_req.wdata = v_cpu_wdata_i;
    assign v_req.wstrb = v_cpu_wstrb_i;
    assign v_cpu_ready_o = v_rsp.ready;
    assign v_cpu_rdata_o = v_rsp.rdata;
    assign v_cpu_error_o = v_rsp.error;

    assign a_req.valid = a_cpu_valid_i;
    assign a_req.write = a_cpu_write_i;
    assign a_req.addr = a_cpu_addr_i;
    assign a_req.wdata = a_cpu_wdata_i;
    assign a_req.wstrb = a_cpu_wstrb_i;
    assign a_cpu_ready_o = a_rsp.ready;
    assign a_cpu_rdata_o = a_rsp.rdata;
    assign a_cpu_error_o = a_rsp.error;

    assign v_obs_cfg_rdata = v_obs_cfg_addr[7] ? launcher_rdata_o : 64'h0;
    assign a_obs_cfg_rdata = 64'h0;

    idma_reg64_frontend #(
        .dma_regs_req_t(dma_req_t),
        .dma_regs_rsp_t(dma_rsp_t),
        .burst_req_t(burst_t),
        .ObserverPort(1'b1),
        .BatchPort(1'b0),
        .LaunchPort(1'b1),
        .DEVICE_ID(24'd10),
        .PROCESS_ID(20'd20),
        .AxiIdWidth(4),
        .AxID(4'd0)
    ) i_v_frontend (
        .clk_i, .rst_ni,
        .obs_issue_o(v_obs_issue_o), .obs_done_o(v_obs_done_o), .obs_poll_o(),
        .batch_active_o(),
        .obs_issue_id_o(v_issue_id_o), .obs_done_id_o(v_done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(v_obs_cfg_valid), .obs_cfg_write_o(v_obs_cfg_write),
        .obs_cfg_addr_o(v_obs_cfg_addr), .obs_cfg_wdata_o(v_obs_cfg_wdata),
        .obs_cfg_wstrb_o(v_obs_cfg_wstrb), .obs_cfg_rdata_i(v_obs_cfg_rdata),
        .dma_ctrl_req_i(v_req), .dma_ctrl_rsp_o(v_rsp),
        .burst_req_o(v_burst_req), .valid_o(v_valid_o),
        .ready_i(v_backend_ready_i), .backend_idle_i(v_backend_idle_i),
        .trans_complete_i(v_trans_complete_i),
        .ttype_o(), .wdata_o(), .rdata_i(64'h0), .rdata_valid_i(1'b0),
        .launch_hold_i(launcher_hold[0]), .launch_go_i(launcher_go[0]),
        .launch_cancel_i(launcher_cancel[0]),
        .launch_held_o(v_launch_held), .launch_active_o(v_launch_active),
        .launch_cancelled_o(v_launch_cancelled), .launch_error_o(v_launch_error)
    );

    idma_reg64_frontend #(
        .dma_regs_req_t(dma_req_t),
        .dma_regs_rsp_t(dma_rsp_t),
        .burst_req_t(burst_t),
        .ObserverPort(1'b0),
        .BatchPort(1'b0),
        .LaunchPort(1'b1),
        .DEVICE_ID(24'd11),
        .PROCESS_ID(20'd21),
        .AxiIdWidth(4),
        .AxID(4'd0)
    ) i_a_frontend (
        .clk_i, .rst_ni,
        .obs_issue_o(a_obs_issue_o), .obs_done_o(a_obs_done_o), .obs_poll_o(),
        .batch_active_o(),
        .obs_issue_id_o(a_issue_id_o), .obs_done_id_o(a_done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(a_obs_cfg_valid), .obs_cfg_write_o(a_obs_cfg_write),
        .obs_cfg_addr_o(a_obs_cfg_addr), .obs_cfg_wdata_o(a_obs_cfg_wdata),
        .obs_cfg_wstrb_o(a_obs_cfg_wstrb), .obs_cfg_rdata_i(a_obs_cfg_rdata),
        .dma_ctrl_req_i(a_req), .dma_ctrl_rsp_o(a_rsp),
        .burst_req_o(a_burst_req), .valid_o(a_valid_o),
        .ready_i(a_backend_ready_i), .backend_idle_i(a_backend_idle_i),
        .trans_complete_i(a_trans_complete_i),
        .ttype_o(), .wdata_o(), .rdata_i(64'h0), .rdata_valid_i(1'b0),
        .launch_hold_i(launcher_hold[1]), .launch_go_i(launcher_go[1]),
        .launch_cancel_i(launcher_cancel[1]),
        .launch_held_o(a_launch_held), .launch_active_o(a_launch_active),
        .launch_cancelled_o(a_launch_cancelled), .launch_error_o(a_launch_error)
    );

    assign fe_held = {2'b0, a_launch_held, v_launch_held};
    assign fe_active = {2'b0, a_launch_active, v_launch_active};
    assign fe_cancelled = {2'b0, a_launch_cancelled, v_launch_cancelled};
    assign fe_error = {2'b0, a_launch_error, v_launch_error};
    assign frontend_held_o = fe_held;
    assign frontend_active_o = fe_active;
    assign frontend_cancelled_o = fe_cancelled;
    assign frontend_error_o = fe_error;

    assign launcher_issue_id[0] = v_issue_id_o;
    assign launcher_issue_id[1] = a_issue_id_o;
    assign launcher_issue_id[2] = '0;
    assign launcher_issue_id[3] = '0;
    assign launcher_done_id[0] = v_done_id_o;
    assign launcher_done_id[1] = a_done_id_o;
    assign launcher_done_id[2] = '0;
    assign launcher_done_id[3] = '0;

    assign launcher_valid_o = v_obs_cfg_valid && v_obs_cfg_addr[7];
    assign launcher_write_o = v_obs_cfg_write;
    assign launcher_addr_o = v_obs_cfg_addr;

    dma_controlled_launcher #(.NumPorts(4)) i_launcher (
        .clk_i, .rst_ni,
        .cfg_valid_i(v_obs_cfg_valid && v_obs_cfg_addr[7]),
        .cfg_write_i(v_obs_cfg_write),
        .cfg_addr_i(v_obs_cfg_addr),
        .cfg_wdata_i(v_obs_cfg_wdata),
        .cfg_wstrb_i(v_obs_cfg_wstrb),
        .cfg_rdata_o(launcher_rdata_o),
        .frontend_held_i(fe_held),
        .frontend_active_i(fe_active),
        .frontend_cancelled_i(fe_cancelled),
        .frontend_error_i(fe_error),
        .obs_issue_i({2'b0, a_obs_issue_o, v_obs_issue_o}),
        .obs_done_i({2'b0, a_obs_done_o, v_obs_done_o}),
        .obs_issue_id_i(launcher_issue_id),
        .obs_done_id_i(launcher_done_id),
        .selected_i(selected_i),
        .select_write_i(select_write_i),
        .select_id_i(select_id_i),
        .select_did_i(select_did_i),
        .select_iova_i(select_iova_i),
        .launch_hold_o(launcher_hold),
        .launch_go_o(launcher_go),
        .launch_cancel_o(launcher_cancel)
    );

    assign launch_hold_o = launcher_hold;
    assign launch_go_o = launcher_go;
    assign launch_cancel_o = launcher_cancel;

    assign v_burst_src_addr_o = v_burst_req.src_addr;
    assign v_burst_dst_addr_o = v_burst_req.dst_addr;
    assign v_burst_length_o = v_burst_req.length;
    assign a_burst_src_addr_o = a_burst_req.src_addr;
    assign a_burst_dst_addr_o = a_burst_req.dst_addr;
    assign a_burst_length_o = a_burst_req.length;
endmodule
```

---

## `tests/idma_controlled_pair_test.cpp`

```cpp
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
```

---

## `tests/dma_observer_finite_test.cpp`

```cpp
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
```

---

## Notes for Codex

- No runner changes are proposed. `tests/test_idma_controlled_pair.py` already supplies `-I cva6/corev_apu/iDMA/src/include`, so the new `` `include "idma/typedef.svh" `` resolves.
- `tests/test_dma_observer_finite.py` already passes `-D CONTROLLED` / `-D LEGACY` through `-CFLAGS`; that is left unchanged.
- I did not edit production RTL.
- I could not execute the tests in this environment, so the acceptance checks below remain for Codex to run.

## Suggested acceptance checks

1. Pair build:
   - `python3 tests/test_idma_controlled_pair.py`
   - expected: no Verilator `UNSUPPORTED` on `burst_req_o.length`, executable prints `PASS idma_controlled_pair`.
2. Finite observer:
   - `python3 tests/test_dma_observer_finite.py`
   - expected controlled: `PASS controlled v6 observer finite/legacy checks`.
   - expected legacy: `PASS legacy v5 observer continuous A rejection`.
3. Confirm no production RTL file is modified by the patch.