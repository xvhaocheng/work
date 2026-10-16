Below are the three complete new test files, the proposed Chinese documentation append, the acceptance commands, the coverage/limitations, and the source evidence.

Nothing here has been executed. All outputs shown are **templates/expectations**, not measured results. The test targets the *described post-change* interface; it will not compile against the production tree as received (that is expected and stated below).

---

## 1. Assumptions (must hold for the test to be valid)

A1. `fifo_v3` gets `parameter bit FORCE_FPGA_RAM = 1'b0` appended **last** in the parameter list (after `ADDR_DEPTH`), and the three global `ariane_pkg::FPGA_EN` tests at `fifo_v3.sv:73, :87, :152` are replaced by a single `localparam UseFpgaRam = ariane_pkg::FPGA_EN || FORCE_FPGA_RAM`; `AsyncDpRam` branch otherwise reused unchanged (`fifo_v3.sv:152-164`).
A2. `stream_fifo` gets the same parameter appended last (after `ADDR_DEPTH`) and forwards it to `fifo_v3` (`stream_fifo.sv:47-52`).
A3. `idma_stream_fifo` opts the *depth>2* branch in **only** for `Depth == 512 && $bits(type_t) == 8`; it needs no new user-visible parameter for this test (the rule is internal). If Codex instead adds a parameter there with default 0, the DUT would silently stay on the register branch — see limitation L1.
A4. `ariane_pkg::FPGA_EN` remains `1'b0` in production (the stub package in the test top hard-codes 0).
A5. Verilator 4.038, `--cc --exe --build`, no `--timing`, `-Wno-fatal +define+ASSERTS_OFF` (per repo precedent, `tests/test_nonblock_coupled.py:132-133`). Verilator predefines `VERILATOR`, so `ifndef VERILATOR` guards (`fifo_v3.sv:176`, `guard.svh:16`, `assertions.svh:28`) and `IDMA_NONSYNTH_BLOCK` (`guard.svh:12-27`) are inert.
A6. All FIFO signals used here are ≤ 32 bit, so Verilator's `CData` is the C++ member type (the code uses `decltype` so it stays correct if that ever changes).
A7. `rst_ni` is asynchronous active-low; `flush_i` is synchronous and clears pointers/count only (`fifo_v3.sv:134-150`). Storage contents are **not** reset on flush; only the register-array branch clears `mem_q` on reset (`fifo_v3.sv:166-172`) while `AsyncDpRam` never clears (`AsyncDpRam.sv:39-50`). Hence data is compared only while `valid_o` is asserted.
A8. The stream-level accept rule used by the oracle is the RTL's own: `push = valid_i & ~full` / `pop = ready_i & ~empty`, with `full`/`empty` judged from the **start-of-cycle** state (`stream_fifo.sv:42-45`, `fifo_v3.sv:62-63, 86, 107`). Therefore holding `valid_i` high while `ready_o` is low is legal and is covered; the disabled `ASSERT_NEVER(CheckFullPush)` (`idma_stream_fifo.sv:123`) is not relied upon.
A9. `usage_o` is `status_cnt_q[ADDR_DEPTH-1:0]` (`fifo_v3.sv:56`), i.e. 0 at full for depth 512 (512 & 0x1FF) and `3` at full for depth 3.

---

## 2. `tests/nonblock_fifo_ram_top.sv`

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_fifo_ram_top.sv
//
// Test-only harness for the production FIFO FPGA-RAM storage equivalence test.
//
// This file contains NO production logic.  It only instantiates the real
// production modules that are under test:
//
//   DUT    idma_stream_fifo  Depth = 512, type_t = logic [7:0]
//          -> the (to be implemented) depth/type rule must make this instance
//             use the FPGA RAM storage branch (AsyncDpRam).
//   REF    stream_fifo       DEPTH = 512, T = logic [7:0], FORCE_FPGA_RAM = 0
//          -> historical register/distributed-RAM storage branch.
//   AB     stream_fifo       DEPTH = 512, T = logic [7:0], FORCE_FPGA_RAM = 1
//          -> same module as REF, ONLY the storage flag differs: direct A/B
//             anchor for the flag itself.
//   LANE3  idma_stream_fifo  Depth = 3,  type_t = logic [7:0]
//          -> lane guard: the opt-in rule is depth+type based, so a depth-3
//             lane must NOT be switched to RAM storage.
//
// ariane_pkg is TEST-ONLY here.  The production tree keeps
// ariane_pkg::FPGA_EN == 0, but pulling in the full CVA6 package would drag
// in the whole SoC configuration; this stub makes the value explicit and
// keeps the FORCE_FPGA_RAM = 0 reference a genuine non-RAM reference.
package ariane_pkg;
    localparam bit FPGA_EN = 1'b0;
endpackage : ariane_pkg


module nonblock_fifo_ram_top (
    input  logic        clk_i,
    input  logic        rst_ni,        // async, active low: resets ALL FIFOs
    input  logic        testmode_i,    // unused by the FIFO RTL, kept for fidelity

    // ---- shared 512 deep, 8 bit stream for DUT / REF / AB ---------------
    input  logic        flush_i,       // synchronous flush for DUT / REF / AB
    input  logic [7:0]  data_i,
    input  logic        valid_i,

    // DUT (production idma_stream_fifo, Depth 512)
    input  logic        dut_ready_i,   // downstream pop side
    output logic [7:0]  dut_data_o,
    output logic        dut_valid_o,
    output logic        dut_ready_o,   // upstream "not full"
    output logic [8:0]  dut_usage_o,

    // REF (production stream_fifo, Depth 512, FORCE_FPGA_RAM = 0)
    input  logic        ref_ready_i,
    output logic [7:0]  ref_data_o,
    output logic        ref_valid_o,
    output logic        ref_ready_o,
    output logic [8:0]  ref_usage_o,

    // AB (production stream_fifo, Depth 512, FORCE_FPGA_RAM = 1)
    input  logic        ab_ready_i,
    output logic [7:0]  ab_data_o,
    output logic        ab_valid_o,
    output logic        ab_ready_o,
    output logic [8:0]  ab_usage_o,

    // ---- independent depth-3 lane guard --------------------------------
    input  logic        lane3_flush_i,
    input  logic [7:0]  lane3_data_i,
    input  logic        lane3_valid_i,
    input  logic        lane3_ready_i,
    output logic [7:0]  lane3_data_o,
    output logic        lane3_valid_o,
    output logic        lane3_ready_o,
    output logic [1:0]  lane3_usage_o
);

    // -----------------------------------------------------------------
    // DUT: production iDMA stream FIFO, 512 x 8 bit.
    // -----------------------------------------------------------------
    idma_stream_fifo #(
        .Depth  ( 32'd512     ),
        .type_t ( logic [7:0] )
    ) i_dut_stream_fifo (
        .clk_i      ( clk_i       ),
        .rst_ni     ( rst_ni      ),
        .flush_i    ( flush_i     ),
        .testmode_i ( testmode_i  ),
        .usage_o    ( dut_usage_o ),
        .data_i     ( data_i      ),
        .valid_i    ( valid_i     ),
        .ready_o    ( dut_ready_o ),
        .data_o     ( dut_data_o  ),
        .valid_o    ( dut_valid_o ),
        .ready_i    ( dut_ready_i )
    );

    // -----------------------------------------------------------------
    // REF: exactly the same production FIFO module, RAM storage OFF.
    // -----------------------------------------------------------------
    stream_fifo #(
        .DEPTH          ( 32'd512     ),
        .T              ( logic [7:0] ),
        .FORCE_FPGA_RAM ( 1'b0        )
    ) i_ref_stream_fifo (
        .clk_i      ( clk_i       ),
        .rst_ni     ( rst_ni      ),
        .flush_i    ( flush_i     ),
        .testmode_i ( testmode_i  ),
        .usage_o    ( ref_usage_o ),
        .data_i     ( data_i      ),
        .valid_i    ( valid_i     ),
        .ready_o    ( ref_ready_o ),
        .data_o     ( ref_data_o  ),
        .valid_o    ( ref_valid_o ),
        .ready_i    ( ref_ready_i )
    );

    // -----------------------------------------------------------------
    // AB: direct flag A/B anchor - identical module, RAM storage ON.
    // -----------------------------------------------------------------
    stream_fifo #(
        .DEPTH          ( 32'd512     ),
        .T              ( logic [7:0] ),
        .FORCE_FPGA_RAM ( 1'b1        )
    ) i_ab_stream_fifo (
        .clk_i      ( clk_i      ),
        .rst_ni     ( rst_ni     ),
        .flush_i    ( flush_i    ),
        .testmode_i ( testmode_i ),
        .usage_o    ( ab_usage_o ),
        .data_i     ( data_i     ),
        .valid_i    ( valid_i    ),
        .ready_o    ( ab_ready_o ),
        .data_o     ( ab_data_o  ),
        .valid_o    ( ab_valid_o ),
        .ready_i    ( ab_ready_i )
    );

    // -----------------------------------------------------------------
    // LANE3: depth-3 lane guard (must stay on the non-RAM branch).
    // -----------------------------------------------------------------
    idma_stream_fifo #(
        .Depth  ( 32'd3       ),
        .type_t ( logic [7:0] )
    ) i_lane3_stream_fifo (
        .clk_i      ( clk_i         ),
        .rst_ni     ( rst_ni        ),
        .flush_i    ( lane3_flush_i ),
        .testmode_i ( testmode_i    ),
        .usage_o    ( lane3_usage_o ),
        .data_i     ( lane3_data_i  ),
        .valid_i    ( lane3_valid_i ),
        .ready_o    ( lane3_ready_o ),
        .data_o     ( lane3_data_o  ),
        .valid_o    ( lane3_valid_o ),
        .ready_i    ( lane3_ready_i )
    );

endmodule : nonblock_fifo_ram_top
```

---

## 3. `tests/nonblock_fifo_ram_test.cpp`

```cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_fifo_ram_test.cpp
//
// Production FIFO storage equivalence test.
//
// What is compared (see tests/nonblock_fifo_ram_top.sv)
// ----------------------------------------------------
//   dut   : production idma_stream_fifo, Depth 512, type_t = logic [7:0]
//           (must internally select the FPGA RAM storage branch)
//   ref   : production stream_fifo, DEPTH 512, T = logic [7:0],
//           FORCE_FPGA_RAM = 0  (historical storage)
//   ab    : production stream_fifo, DEPTH 512, T = logic [7:0],
//           FORCE_FPGA_RAM = 1  (same module, flag only)
//   lane3 : production idma_stream_fifo, Depth 3, type_t = logic [7:0]
//           (must NOT be opted in by the depth/type rule)
//
// Three independent checkers run every cycle
// ------------------------------------------
//   1) DUT vs REF and AB vs REF: valid_/ready_/usage_ must be identical, and
//      data_o must be identical whenever valid_o is asserted.
//   2) an independent std::deque software scoreboard per FIFO.  The FIFO's
//      full/empty/usage flags and the head data are compared against the
//      queue, and the queue is advanced with the same accept rule the RTL
//      uses:  push only when not full, pop only when not empty, both judged
//      from the state at the START of the cycle (fifo_v3.sv:62-63, :86, :107,
//      stream_fifo.sv:42-45).
//   3) an AXI-style stability rule: while valid_o is asserted and the consumer
//      has not accepted, valid_o must stay high and data_o must not change.
//
// data_o is compared ONLY while valid_o is asserted.  The register array is
// reset to 0 (fifo_v3.sv:166-172) while AsyncDpRam never clears its contents
// (AsyncDpRam.sv:39-50), so the *invalid* head differs by construction and is
// not observable through the stream interface.  The number of such
// (uninteresting) invalid-data differences is counted and printed.
//
// Handshakes and payloads are sampled at clk low, before the rising edge
// (same discipline as tests/nonblock_coupled_test.cpp:566-689).
//
// Environment: Verilator 4.038, --cc --exe --build --top-module
// nonblock_fifo_ram_top, +define+ASSERTS_OFF, -Wno-fatal, no --timing.
//
// Exit status: 0 when every scenario passed, 1 on any scenario failure,
// 2 on a bad command line / no scenario selected.

#include "Vnonblock_fifo_ram_top.h"
#include "verilated.h"

double sc_time_stamp() { return 0.0; }

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

// ---------------------------------------------------------------------------
// signal type of the Verilated model (CData for all signals used here; the
// decltype keeps this correct if a Verilator version changes the member type)
// ---------------------------------------------------------------------------
typedef std::remove_reference<
            decltype(((Vnonblock_fifo_ram_top*)0)->dut_data_o)
        >::type SigT;

// ---------------------------------------------------------------------------
// bookkeeping / failure reporting
// ---------------------------------------------------------------------------
static int      g_n_checks           = 0;
static long     g_n_cycles           = 0;
static uint64_t g_n_invalid_data_diff = 0;   // informational only
static uint64_t g_n_stall_cycles      = 0;   // informational only

struct TestFailure : std::runtime_error {
    explicit TestFailure(const std::string& m) : std::runtime_error(m) {}
};

static void fail(const std::string& msg, const char* file, int line) {
    char buf[1024];
    std::snprintf(buf, sizeof(buf), "%s (%s:%d)", msg.c_str(), file, line);
    throw TestFailure(buf);
}

#define CHECK(cond, msg)                                                        \
    do {                                                                        \
        ++g_n_checks;                                                           \
        if (!(cond)) fail((msg), __FILE__, __LINE__);                           \
    } while (0)

#define CHECKF(cond, ...)                                                       \
    do {                                                                        \
        ++g_n_checks;                                                           \
        if (!(cond)) {                                                          \
            char b_[1024];                                                      \
            std::snprintf(b_, sizeof(b_), __VA_ARGS__);                         \
            fail(std::string(b_), __FILE__, __LINE__);                          \
        }                                                                       \
    } while (0)

// deterministic, non trivial 8 bit pattern; the index only advances when the
// beat was actually accepted, so the driven stream is exactly the accepted
// stream (as a real producer would behave)
static uint8_t pat(uint32_t i) {
    const uint32_t x = i * 2654435761u;
    return (uint8_t)((x >> 13) ^ (i & 0xFFu));
}

// ---------------------------------------------------------------------------
// one FIFO channel: DUT interface + independent software scoreboard
// ---------------------------------------------------------------------------
struct Chan {
    std::string name;
    unsigned    depth;
    unsigned    addr_bits;

    SigT* p_data_i;
    SigT* p_valid_i;
    SigT* p_ready_i;      // downstream pop side
    SigT* p_flush_i;
    SigT* p_data_o;
    SigT* p_valid_o;
    SigT* p_ready_o;      // upstream "not full"
    SigT* p_usage_o;

    std::deque<uint8_t> q;          // ground truth

    // start-of-cycle (pre rising edge) snapshot
    bool     s_valid_i;
    uint8_t  s_data_i;
    bool     s_ready_i;
    bool     s_flush;
    bool     s_valid_o;
    bool     s_ready_o;
    uint8_t  s_data_o;
    uint32_t s_usage_o;

    // previous cycle, for the output stability rule
    bool     h_have;
    bool     h_valid_o;
    uint8_t  h_data_o;
    bool     h_ready_i;

    uint64_t n_push;
    uint64_t n_pop;
    uint64_t n_flush;
    uint64_t n_stall;

    Chan()
        : depth(0), addr_bits(0),
          p_data_i(0), p_valid_i(0), p_ready_i(0), p_flush_i(0),
          p_data_o(0), p_valid_o(0), p_ready_o(0), p_usage_o(0),
          s_valid_i(false), s_data_i(0), s_ready_i(false), s_flush(false),
          s_valid_o(false), s_ready_o(false), s_data_o(0), s_usage_o(0),
          h_have(false), h_valid_o(false), h_data_o(0), h_ready_i(false),
          n_push(0), n_pop(0), n_flush(0), n_stall(0) {}

    void init(const char* n, unsigned d, unsigned ab,
              SigT* data_i, SigT* valid_i, SigT* ready_i, SigT* flush_i,
              SigT* data_o, SigT* valid_o, SigT* ready_o, SigT* usage_o) {
        name.assign(n);
        depth     = d;
        addr_bits = ab;
        p_data_i  = data_i;  p_valid_i = valid_i;  p_ready_i = ready_i;
        p_flush_i = flush_i;
        p_data_o  = data_o;  p_valid_o = valid_o;  p_ready_o = ready_o;
        p_usage_o = usage_o;
        clear_all();
    }

    void clear_all() {
        q.clear();
        s_valid_i = s_ready_i = s_flush = s_valid_o = s_ready_o = false;
        s_data_i = s_data_o = 0;
        s_usage_o = 0;
        h_have = h_valid_o = h_ready_i = false;
        h_data_o = 0;
        n_push = n_pop = n_flush = n_stall = 0;
    }

    void drive(bool valid, uint8_t data, bool out_ready) {
        *p_valid_i = valid ? 1 : 0;
        *p_data_i  = (SigT)data;
        *p_ready_i = out_ready ? 1 : 0;
    }

    void set_flush(bool f) { *p_flush_i = f ? 1 : 0; }

    void snapshot() {
        s_valid_i = (*p_valid_i != 0);
        s_data_i  = (uint8_t)(*p_data_i & 0xFFu);
        s_ready_i = (*p_ready_i != 0);
        s_flush   = (*p_flush_i != 0);
        s_valid_o = (*p_valid_o != 0);
        s_ready_o = (*p_ready_o != 0);
        s_data_o  = (uint8_t)(*p_data_o & 0xFFu);
        s_usage_o = (uint32_t)(*p_usage_o);
    }

    // called after the rising edge, using only the pre-edge snapshot
    void update_and_check() {
        const bool     full_pre  = (q.size() >= (size_t)depth);
        const bool     empty_pre = q.empty();
        const uint32_t mask      = (1u << addr_bits) - 1u;
        const uint32_t usage_pre = (uint32_t)((uint64_t)q.size() & (uint64_t)mask);

        CHECKF(s_valid_o == !empty_pre,
               "%s: valid_o=%d but scoreboard empty=%d",
               name.c_str(), (int)s_valid_o, (int)empty_pre);
        CHECKF(s_ready_o == !full_pre,
               "%s: ready_o=%d but scoreboard full=%d",
               name.c_str(), (int)s_ready_o, (int)full_pre);
        CHECKF(s_usage_o == usage_pre,
               "%s: usage_o=%u scoreboard=%u",
               name.c_str(), (unsigned)s_usage_o, (unsigned)usage_pre);

        // head data must match whenever the output is valid
        if (s_valid_o) {
            CHECKF(!q.empty(), "%s: valid_o with empty scoreboard", name.c_str());
            CHECKF(s_data_o == q.front(),
                   "%s: data_o=0x%02x scoreboard head=0x%02x",
                   name.c_str(), (unsigned)s_data_o, (unsigned)q.front());
        }

        // AXI-style stability while the consumer does not accept
        if (h_have && h_valid_o && !h_ready_i) {
            ++n_stall;
            ++g_n_stall_cycles;
            CHECKF(s_valid_o, "%s: valid_o dropped before handshake", name.c_str());
            CHECKF(s_data_o == h_data_o,
                   "%s: data_o changed while stalled (0x%02x -> 0x%02x)",
                   name.c_str(), (unsigned)h_data_o, (unsigned)s_data_o);
        }

        h_have    = true;
        h_valid_o = s_valid_o;
        h_data_o  = s_data_o;
        h_ready_i = s_ready_i;

        // RTL accept rule, judged from the start-of-cycle state
        const bool pop_fire  = s_valid_o && s_ready_i;
        const bool push_fire = s_valid_i && !full_pre;

        if (push_fire) { q.push_back(s_data_i); ++n_push; }
        if (pop_fire) {
            CHECKF(!q.empty(), "%s: pop with empty scoreboard", name.c_str());
            q.pop_front();
            ++n_pop;
        }

        // synchronous flush wins over everything (fifo_v3.sv:140-148)
        if (s_flush) {
            q.clear();
            ++n_flush;
            h_have = false;
        }
    }
};

// ---------------------------------------------------------------------------
// test bench
// ---------------------------------------------------------------------------
struct Tb {
    Vnonblock_fifo_ram_top* top;
    Chan dut, ref, ab, l3;
    long cycle;
    bool flush_req;
    bool lane3_flush_req;

    Tb() : top(0), cycle(0), flush_req(false), lane3_flush_req(false) {
        top = new Vnonblock_fifo_ram_top;

        top->clk_i         = 0;
        top->rst_ni        = 0;
        top->testmode_i    = 0;
        top->flush_i       = 0;
        top->lane3_flush_i = 0;

        dut.init("dut", 512, 9,
                 &top->dut_data_i,  &top->dut_valid_i,  &top->dut_ready_i,
                 &top->flush_i,
                 &top->dut_data_o,  &top->dut_valid_o,  &top->dut_ready_o,
                 &top->dut_usage_o);

        ref.init("ref", 512, 9,
                 &top->ref_data_i,  &top->ref_valid_i,  &top->ref_ready_i,
                 &top->flush_i,
                 &top->ref_data_o,  &top->ref_valid_o,  &top->ref_ready_o,
                 &top->ref_usage_o);

        ab.init("ab", 512, 9,
                &top->ab_data_i,  &top->ab_valid_i,  &top->ab_ready_i,
                &top->flush_i,
                &top->ab_data_o,  &top->ab_valid_o,  &top->ab_ready_o,
                &top->ab_usage_o);

        l3.init("lane3", 3, 2,
                &top->lane3_data_i,  &top->lane3_valid_i,  &top->lane3_ready_i,
                &top->lane3_flush_i,
                &top->lane3_data_o,  &top->lane3_valid_o,  &top->lane3_ready_o,
                &top->lane3_usage_o);

        dut.drive(false, 0, false);
        ref.drive(false, 0, false);
        ab.drive(false, 0, false);
        l3.drive(false, 0, false);

        top->eval();
    }

    ~Tb() {
        top->final();
        delete top;
    }

    // ---- driving helpers -------------------------------------------------
    void drive_big(bool valid, uint8_t data, bool out_ready) {
        dut.drive(valid, data, out_ready);
        ref.drive(valid, data, out_ready);
        ab.drive(valid, data, out_ready);
    }

    // push n beats with the output blocked; `occ` is the model occupancy at
    // the start of the current cycle and is updated with the RTL rule
    void push_n(int n, uint32_t& idx, int& occ) {
        for (int i = 0; i < n; ++i) {
            const bool will_push = (occ < 512);
            drive_big(true, pat(idx), false);
            if (will_push) { occ += 1; idx += 1; }
            tick();
        }
    }

    // pop n beats with the input idle
    void pop_n(int n, int& occ) {
        for (int i = 0; i < n; ++i) {
            const bool will_pop = (occ > 0);
            drive_big(false, 0, true);
            if (will_pop) { occ -= 1; }
            tick();
        }
    }

    // ---- cycle -----------------------------------------------------------
    void tick() {
        dut.set_flush(flush_req);
        ref.set_flush(flush_req);
        ab.set_flush(flush_req);
        l3.set_flush(lane3_flush_req);

        top->clk_i = 0;
        top->eval();                       // settle combinationals at clk low

        dut.snapshot(); ref.snapshot(); ab.snapshot(); l3.snapshot();

        // cross implementation equivalence
        check_equiv(dut, ref, "dut", "ref");
        check_equiv(ab,  ref, "ab",  "ref");
        check_equiv(dut, ab,  "dut", "ab");

        // informational: storage differs on INVALID entries by construction
        if (!dut.s_valid_o && !ref.s_valid_o && dut.s_data_o != ref.s_data_o)
            ++g_n_invalid_data_diff;

        top->clk_i = 1;
        top->eval();                       // rising edge: state updates

        dut.update_and_check();
        ref.update_and_check();
        ab.update_and_check();
        l3.update_and_check();

        top->clk_i = 0;
        top->eval();

        ++cycle;
        ++g_n_cycles;
    }

    void check_equiv(const Chan& a, const Chan& b, const char* an, const char* bn) {
        CHECKF(a.s_valid_o == b.s_valid_o,
               "%s.valid_o=%d != %s.valid_o=%d (cycle %ld)",
               an, (int)a.s_valid_o, bn, (int)b.s_valid_o, cycle);
        CHECKF(a.s_ready_o == b.s_ready_o,
               "%s.ready_o=%d != %s.ready_o=%d (cycle %ld)",
               an, (int)a.s_ready_o, bn, (int)b.s_ready_o, cycle);
        CHECKF(a.s_usage_o == b.s_usage_o,
               "%s.usage_o=%u != %s.usage_o=%u (cycle %ld)",
               an, (unsigned)a.s_usage_o, bn, (unsigned)b.s_usage_o, cycle);
        if (a.s_valid_o && b.s_valid_o) {
            CHECKF(a.s_data_o == b.s_data_o,
                   "%s.data_o=0x%02x != %s.data_o=0x%02x (cycle %ld)",
                   an, (unsigned)a.s_data_o, bn, (unsigned)b.s_data_o, cycle);
        }
    }

    // ---- reset -----------------------------------------------------------
    void reset_all() {
        top->clk_i  = 0;
        top->rst_ni = 0;
        top->flush_i = 0;
        top->lane3_flush_i = 0;
        flush_req = false;
        lane3_flush_req = false;

        dut.drive(false, 0, false);
        ref.drive(false, 0, false);
        ab.drive(false, 0, false);
        l3.drive(false, 0, false);

        top->eval();                       // asynchronous reset takes effect

        dut.clear_all();
        ref.clear_all();
        ab.clear_all();
        l3.clear_all();

        for (int i = 0; i < 3; ++i) tick();

        top->rst_ni = 1;
        top->eval();

        for (int i = 0; i < 3; ++i) tick();
    }

    // =====================================================================
    // scenarios
    // =====================================================================

    // 1) reset / idle
    void sc_reset_idle() {
        reset_all();
        for (int i = 0; i < 16; ++i) {
            drive_big(false, 0, (i & 1) != 0);
            tick();
        }
        CHECK(dut.q.empty() && ref.q.empty() && ab.q.empty(),
              "reset-idle: big FIFOs not empty");
        CHECK(!dut.s_valid_o && dut.s_ready_o && dut.s_usage_o == 0,
              "reset-idle: dut flags wrong");
        CHECK(!ref.s_valid_o && ref.s_ready_o && ref.s_usage_o == 0,
              "reset-idle: ref flags wrong");
        CHECK(l3.q.empty() && !l3.s_valid_o && l3.s_ready_o && l3.s_usage_o == 0,
              "reset-idle: lane3 flags wrong");
    }

    // 2) deterministic fill of 512 + one refused push / full stall
    void sc_fill512_full_stall() {
        reset_all();
        uint32_t idx = 0;
        int occ = 0;

        push_n(512, idx, occ);
        CHECKF(dut.q.size() == 512, "fill512: dut occupancy %zu != 512", dut.q.size());
        CHECKF(ref.q.size() == 512, "fill512: ref occupancy %zu != 512", ref.q.size());
        CHECK(dut.n_push == 512, "fill512: push count != 512");
        CHECK(!dut.s_ready_o, "fill512: ready_o must be low when full");
        CHECKF(dut.s_usage_o == 0,
               "fill512: usage_o=%u (9 bit truncation of 512 is 0)", (unsigned)dut.s_usage_o);
        CHECKF(dut.s_data_o == pat(0),
               "fill512: head 0x%02x != 0x%02x", (unsigned)dut.s_data_o, (unsigned)pat(0));

        // refused push: hold valid high while full for several cycles
        const uint8_t refused = pat(9000u);
        for (int k = 0; k < 4; ++k) {
            drive_big(true, refused, false);
            tick();
            CHECK(!dut.s_ready_o, "full-stall: ready_o must stay low while full");
            CHECKF(dut.q.size() == 512, "full-stall: occupancy changed to %zu", dut.q.size());
        }
        CHECKF(dut.q.back() == pat(511), "full-stall: tail changed");
        CHECKF(dut.s_data_o == pat(0), "full-stall: head changed");

        // free one slot with the same valid still asserted: the RTL uses the
        // PRESENT-state full flag, so this cycle refuses the push
        drive_big(true, refused, true);
        tick();
        CHECKF(dut.q.size() == 511, "refused-push cycle: occupancy %zu != 511", dut.q.size());
        CHECKF(dut.q.back() == pat(511), "refused-push cycle: beat accepted too early");

        // next cycle the same beat is accepted (and one is popped)
        drive_big(true, refused, true);
        tick();
        CHECKF(dut.q.size() == 511, "accept cycle: occupancy %zu != 511", dut.q.size());
        CHECKF(dut.q.back() == refused, "accept cycle: beat not accepted");
    }

    // 3) drain of 512
    void sc_drain512() {
        reset_all();
        uint32_t idx = 0;
        int occ = 0;
        push_n(512, idx, occ);
        CHECK(dut.q.size() == 512, "drain512: preload failed");

        const uint64_t pops0 = dut.n_pop;
        pop_n(512, occ);
        CHECKF(dut.q.empty(), "drain512: not empty (%zu)", dut.q.size());
        CHECKF(dut.n_pop - pops0 == 512,
               "drain512: popped %llu != 512",
               (unsigned long long)(dut.n_pop - pops0));
        CHECK(!dut.s_valid_o && dut.s_ready_o && dut.s_usage_o == 0,
              "drain512: flags wrong when empty");

        // extra ready cycles on an empty FIFO must not pop
        const uint64_t pops1 = dut.n_pop;
        for (int i = 0; i < 8; ++i) { drive_big(false, 0, true); tick(); }
        CHECK(dut.n_pop == pops1, "drain512: pop while empty");
    }

    // 4) several wraps plus phase shifted partial bursts
    void sc_wrap() {
        reset_all();
        uint32_t idx = 0;
        int occ = 0;

        for (int w = 0; w < 3; ++w) {
            push_n(512, idx, occ);
            CHECKF(dut.q.size() == 512, "wrap %d: occupancy %zu != 512", w, dut.q.size());
            CHECK(!dut.s_ready_o, "wrap: ready_o must be low when full");
            pop_n(512, occ);
            CHECKF(dut.q.empty(), "wrap %d: not empty (%zu)", w, dut.q.size());
            CHECK(dut.s_ready_o, "wrap: ready_o must be high when empty");
        }

        static const int p_push[6] = { 300, 211,   1, 511, 250, 512 };
        static const int p_pop [6] = { 299,   1,   1,   1, 512,   0 };

        for (int p = 0; p < 6; ++p) {
            push_n(p_push[p], idx, occ);
            CHECKF(dut.q.size() == (size_t)occ,
                   "phase %d push: dut %zu model %d", p, dut.q.size(), occ);
            pop_n(p_pop[p], occ);
            CHECKF(dut.q.size() == (size_t)occ,
                   "phase %d pop: dut %zu model %d", p, dut.q.size(), occ);
        }

        pop_n(occ, occ);
        CHECK(dut.q.empty(), "wrap: final drain failed");
        CHECK(occ == 0, "wrap: model occupancy not zero");
    }

    // 5) simultaneous push and pop over a long run
    void sc_simultaneous() {
        reset_all();
        uint32_t idx = 0;
        int occ = 0;
        push_n(256, idx, occ);
        CHECKF(dut.q.size() == 256, "simultaneous: preload %zu != 256", dut.q.size());

        const uint64_t pushes0 = dut.n_push;
        const uint64_t pops0   = dut.n_pop;

        for (int i = 0; i < 1000; ++i) {
            const bool will_push = (occ < 512);
            const bool will_pop  = (occ > 0);
            drive_big(true, pat(idx), true);
            if (will_push) { occ += 1; idx += 1; }
            if (will_pop)  { occ -= 1; }
            tick();
            CHECKF(dut.q.size() == (size_t)occ,
                   "simultaneous cycle %d: dut %zu model %d", i, dut.q.size(), occ);
        }

        CHECKF(dut.q.size() == 256, "simultaneous: occupancy %zu != 256", dut.q.size());
        CHECKF(dut.n_push - pushes0 == 1000,
               "simultaneous: pushes %llu != 1000",
               (unsigned long long)(dut.n_push - pushes0));
        CHECKF(dut.n_pop - pops0 == 1000,
               "simultaneous: pops %llu != 1000",
               (unsigned long long)(dut.n_pop - pops0));

        pop_n(256, occ);
        CHECK(dut.q.empty(), "simultaneous: drain failed");
    }

    // 6) sustained output backpressure with a stable payload
    void sc_backpressure() {
        reset_all();
        uint32_t idx = 0;
        int occ = 0;
        push_n(300, idx, occ);
        CHECKF(dut.q.size() == 300, "backpressure: preload %zu != 300", dut.q.size());

        const uint64_t stalls0 = dut.n_stall;
        for (int i = 0; i < 300; ++i) { drive_big(false, 0, false); tick(); }
        CHECKF(dut.n_stall - stalls0 == 300,
               "backpressure: stalled cycles %llu != 300",
               (unsigned long long)(dut.n_stall - stalls0));
        CHECKF(dut.q.size() == 300, "backpressure: occupancy %zu != 300", dut.q.size());
        CHECK(dut.s_valid_o, "backpressure: valid_o dropped");
        CHECKF(dut.s_data_o == pat(0),
               "backpressure: head 0x%02x != 0x%02x",
               (unsigned)s_data_o_dummy(dut), (unsigned)pat(0));

        // input traffic while the output stays blocked
        push_n(100, idx, occ);
        CHECKF(dut.q.size() == 400, "backpressure: occupancy %zu != 400", dut.q.size());
        for (int i = 0; i < 50; ++i) { drive_big(false, 0, false); tick(); }
        CHECK(dut.s_data_o == pat(0), "backpressure: head changed after more pushes");

        // release and drain
        pop_n(400, occ);
        CHECKF(dut.q.empty(), "backpressure: not empty after drain (%zu)", dut.q.size());
    }

    // 7) flush while occupied, then refill and drain
    void sc_flush() {
        reset_all();
        uint32_t idx = 0;
        int occ = 0;
        push_n(200, idx, occ);
        CHECKF(dut.q.size() == 200, "flush: preload %zu != 200", dut.q.size());

        flush_req = true;
        drive_big(false, 0, false);
        tick();
        drive_big(false, 0, false);
        tick();
        flush_req = false;
        CHECKF(dut.q.empty(), "flush: occupancy not cleared (%zu)", dut.q.size());
        CHECK(dut.n_flush == 2, "flush: flush count != 2");

        drive_big(false, 0, true);
        tick();
        CHECK(!dut.s_valid_o && dut.s_ready_o && dut.s_usage_o == 0,
              "flush: flags wrong after flush");

        // the storage must stay usable afterwards
        occ = 0; idx = 0;
        push_n(512, idx, occ);
        CHECKF(dut.q.size() == 512, "flush-refill: occupancy %zu != 512", dut.q.size());
        pop_n(512, occ);
        CHECKF(dut.q.empty(), "flush-refill: not empty (%zu)", dut.q.size());
    }

    // 8) asynchronous reset while occupied, partial refill, full refill/drain
    void sc_reset_occupied() {
        reset_all();
        uint32_t idx = 0;
        int occ = 0;
        push_n(300, idx, occ);
        pop_n(100, occ);
        CHECKF(dut.q.size() == 200, "reset: preload %zu != 200", dut.q.size());

        // asynchronous reset while occupied: production state collapses in the
        // same simulation time, the scoreboard is re-initialised to match
        top->clk_i = 0;
        top->rst_ni = 0;
        top->eval();

        dut.clear_all();
        ref.clear_all();
        ab.clear_all();
        l3.clear_all();

        drive_big(false, 0, false);
        for (int i = 0; i < 4; ++i) tick();
        top->rst_ni = 1;
        top->eval();
        for (int i = 0; i < 4; ++i) tick();

        CHECK(!dut.s_valid_o && dut.s_ready_o && dut.s_usage_o == 0,
              "reset: flags wrong after reset");

        // partial refill of only 5 entries: the storage still holds the
        // pre-reset data at every other address.  Only the 5 new entries may
        // ever be observed (the comparison is gated by valid_o); this is the
        // case that would fail if data_o were compared unconditionally.
        idx = 0; occ = 0;
        push_n(5, idx, occ);
        CHECKF(dut.s_valid_o, "reset: no valid output after 5 pushes");
        CHECKF(dut.s_data_o == pat(0),
               "reset: first entry 0x%02x != 0x%02x",
               (unsigned)dut.s_data_o, (unsigned)pat(0));
        pop_n(5, occ);
        CHECK(!dut.s_valid_o, "reset: still valid after draining 5 entries");

        // full refill and drain
        push_n(512, idx, occ);
        CHECKF(dut.q.size() == 512, "reset-refill: occupancy %zu != 512", dut.q.size());
        pop_n(512, occ);
        CHECKF(dut.q.empty(), "reset-refill: not empty (%zu)", dut.q.size());
    }

    // 9) depth-3 lane guard (must stay on the non-RAM branch)
    void sc_lane3_guard() {
        reset_all();

        for (int i = 0; i < 3; ++i) {
            l3.drive(true, pat(100u + (uint32_t)i), false);
            tick();
        }
        CHECKF(l3.q.size() == 3, "lane3: occupancy %zu != 3", l3.q.size());
        CHECKF(l3.s_usage_o == 3, "lane3: usage_o=%u != 3", (unsigned)l3.s_usage_o);
        CHECK(!l3.s_ready_o, "lane3: ready_o must be low when full");

        // refused push while full
        for (int k = 0; k < 3; ++k) {
            l3.drive(true, pat(200u), false);
            tick();
            CHECKF(l3.q.size() == 3, "lane3: occupancy changed to %zu", l3.q.size());
        }

        // pop one, then push+pop in the same cycle
        l3.drive(false, 0, true);
        tick();
        CHECKF(l3.q.size() == 2, "lane3: occupancy %zu != 2", l3.q.size());
        l3.drive(true, pat(300u), true);
        tick();
        CHECKF(l3.q.size() == 2, "lane3: occupancy %zu != 2 after push+pop", l3.q.size());

        // flush
        lane3_flush_req = true;
        l3.drive(false, 0, false);
        tick();
        lane3_flush_req = false;
        CHECKF(l3.q.empty(), "lane3: flush failed (%zu)", l3.q.size());
        l3.drive(false, 0, false);
        tick();
        CHECK(l3.s_usage_o == 0 && !l3.s_valid_o && l3.s_ready_o,
              "lane3: flags wrong after flush");

        // still usable
        for (int i = 0; i < 3; ++i) {
            l3.drive(true, pat(400u + (uint32_t)i), false);
            tick();
        }
        for (int i = 0; i < 3; ++i) { l3.drive(false, 0, true); tick(); }
        CHECKF(l3.q.empty(), "lane3: drain failed (%zu)", l3.q.size());
    }

    // small helper only used to keep the CHECKF argument list readable
    static uint32_t s_data_o_dummy(const Chan& c) { return (uint32_t)c.s_data_o; }
};

// ---------------------------------------------------------------------------
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    std::string only;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        const std::string key = "+scenario=";
        if (a.size() >= key.size() && a.compare(0, key.size(), key) == 0)
            only = a.substr(key.size());
    }

    const struct Scenario {
        const char* name;
        void (Tb::*fn)();
    } plan[] = {
        { "reset-idle",                  &Tb::sc_reset_idle         },
        { "fill512-and-full-stall",      &Tb::sc_fill512_full_stall },
        { "drain512",                    &Tb::sc_drain512           },
        { "wrap-multiple-times",         &Tb::sc_wrap               },
        { "simultaneous-push-pop",       &Tb::sc_simultaneous       },
        { "output-backpressure-stable",  &Tb::sc_backpressure       },
        { "flush-while-occupied-refill", &Tb::sc_flush              },
        { "reset-while-occupied-refill", &Tb::sc_reset_occupied     },
        { "depth3-lane-guard",           &Tb::sc_lane3_guard        },
    };
    const unsigned n_plan = (unsigned)(sizeof(plan) / sizeof(plan[0]));

    std::printf("nonblock_fifo_ram_test: start (%u scenarios in plan, "
                "filter='%s')\n", n_plan, only.empty() ? "(none)" : only.c_str());

    int ran = 0;
    int failed = 0;

    for (unsigned i = 0; i < n_plan; ++i) {
        if (!only.empty() && only != plan[i].name) continue;
        ++ran;
        try {
            Tb tb;
            (tb.*(plan[i].fn))();
            std::printf("nonblock_fifo_ram_test: scenario %-30s PASS "
                        "(cycles=%ld, occ dut=%zu ref=%zu ab=%zu lane3=%zu)\n",
                        plan[i].name, tb.cycle,
                        tb.dut.q.size(), tb.ref.q.size(),
                        tb.ab.q.size(),  tb.l3.q.size());
            std::fflush(stdout);
        } catch (const std::exception& e) {
            ++failed;
            std::fprintf(stderr, "nonblock_fifo_ram_test: scenario %-30s FAIL: %s\n",
                         plan[i].name, e.what());
            std::fflush(stderr);
        }
    }

    if (ran == 0) {
        std::fprintf(stderr, "nonblock_fifo_ram_test: no scenario matched '%s'\n",
                     only.c_str());
        return 2;
    }

    std::printf("nonblock_fifo_ram_test: %d/%d scenarios passed, %d checks, "
                "%ld cycles, invalid-storage-data-diffs=%llu, "
                "stalled-output-cycles=%llu\n",
                ran - failed, ran, g_n_checks, g_n_cycles,
                (unsigned long long)g_n_invalid_data_diff,
                (unsigned long long)g_n_stall_cycles);

    if (failed) {
        std::printf("nonblock_fifo_ram_test: FAIL (%d/%d scenarios failed)\n",
                    failed, ran);
        return 1;
    }

    std::printf("nonblock_fifo_ram_test: PASS (%d/%d scenarios)\n", ran, ran);
    return 0;
}
```

### 3.1 Small cleanup note for the C++

Replace the helper call in `sc_backpressure` (`(unsigned)s_data_o_dummy(dut)`) with a plain local variable to avoid the extra helper — or keep the helper, it is harmless; I left it because it keeps the `CHECKF` argument list identical to the other checks. Codex may delete `s_data_o_dummy` and write:

```cpp
        CHECKF(dut.s_data_o == pat(0), "backpressure: head 0x%02x != 0x%02x",
               (unsigned)dut.s_data_o, (unsigned)pat(0));
```

---

## 4. `tests/test_nonblock_fifo_ram.py`

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the production FIFO FPGA-RAM storage equivalence test.

Builds tests/nonblock_fifo_ram_test.cpp against the real production RTL:

    cva6/vendor/pulp-platform/common_cells/src/fifo_v3.sv
    cva6/vendor/pulp-platform/common_cells/src/stream_fifo.sv
    cva6/vendor/pulp-platform/fpga-support/rtl/AsyncDpRam.sv
    cva6/corev_apu/iDMA/src/idma_stream_fifo.sv
    tests/nonblock_fifo_ram_top.sv          (test-only harness + ariane_pkg stub)

Usage:
    python3 tests/test_nonblock_fifo_ram.py
    python3 tests/test_nonblock_fifo_ram.py --strict-branch
    python3 tests/test_nonblock_fifo_ram.py --scenario drain512

The RTL sources are used in place: nothing is copied or duplicated.  The
test top declares a TEST-ONLY ariane_pkg with FPGA_EN = 1'b0 so the test is
independent of the whole CVA6 configuration and so the FORCE_FPGA_RAM = 0
reference really is the non-RAM reference.

This test targets the post-change production interface:
    fifo_v3     gains `parameter bit FORCE_FPGA_RAM = 1'b0` (appended LAST)
    stream_fifo gains the same parameter and forwards it
    idma_stream_fifo opts the depth>2 branch in only for
        Depth == 512 && $bits(type_t) == 8
Until that change is applied the build fails with a clear Verilator
"parameter not found" error; that is expected.

Verilator 4.038 is the target version; --timing is deliberately NOT used.
"""

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT  = ROOT / "out" / "nonblock-fifo-ram"

TOP = "nonblock_fifo_ram_top"
CPP = "tests/nonblock_fifo_ram_test.cpp"

REQUIRED_SOURCES = [
    "cva6/vendor/pulp-platform/common_cells/src/fifo_v3.sv",
    "cva6/vendor/pulp-platform/common_cells/src/stream_fifo.sv",
    "cva6/vendor/pulp-platform/fpga-support/rtl/AsyncDpRam.sv",
    "cva6/corev_apu/iDMA/src/idma_stream_fifo.sv",
    "tests/nonblock_fifo_ram_top.sv",
]

# Needed only if the tool happens to elaborate the Depth == 2 spill branch of
# idma_stream_fifo (it must not for Depth 512, but some Verilator versions
# still resolve cells in unselected generate branches).
OPTIONAL_SOURCES = [
    "cva6/vendor/pulp-platform/common_cells/src/spill_register_flushable.sv",
    "cva6/vendor/pulp-platform/common_cells/src/spill_register.sv",
]

INCDIRS = [
    "cva6/vendor/pulp-platform/common_cells/include",
    "cva6/corev_apu/iDMA/src/include",
]

# Verilator flattens hierarchical names with __DOT__ separators.  These
# patterns are only used to REPORT which storage branch each instance got;
# they are not part of the pass/fail criteria unless --strict-branch is given.
BRANCH_PATTERNS = {
    "dut_ram":    r"i_dut_stream_fifo[A-Za-z0-9_]{0,80}gen_fpga_queue",
    "dut_asic":   r"i_dut_stream_fifo[A-Za-z0-9_]{0,80}gen_asic_queue",
    "ab_ram":     r"i_ab_stream_fifo[A-Za-z0-9_]{0,80}gen_fpga_queue",
    "ref_asic":   r"i_ref_stream_fifo[A-Za-z0-9_]{0,80}gen_asic_queue",
    "lane3_asic": r"i_lane3_stream_fifo[A-Za-z0-9_]{0,80}gen_asic_queue",
}


def require(path: Path) -> Path:
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def tail(path: Path, lines: int = 200) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def verilator_version() -> str:
    try:
        out = subprocess.run(["verilator", "--version"],
                             stdout=subprocess.PIPE,
                             stderr=subprocess.STDOUT)
    except FileNotFoundError:
        return ""
    return out.stdout.decode(errors="replace").strip()


def scan_branch_evidence(obj_dir: Path) -> dict:
    found = {}
    for p in sorted(obj_dir.rglob("*")):
        if p.suffix not in (".h", ".hpp", ".cpp"):
            continue
        try:
            text = p.read_text(errors="replace")
        except OSError:
            continue
        for key, rx in BRANCH_PATTERNS.items():
            if key in found:
                continue
            if re.search(rx, text, re.DOTALL):
                found[key] = p.name
    return found


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--scenario", default=None,
                    help="run only the named scenario "
                         "(forwarded to the C++ harness as +scenario=)")
    ap.add_argument("--strict-branch", action="store_true",
                    help="also FAIL if the generated model does not show the "
                         "DUT on the FPGA RAM branch (gen_fpga_queue).  Off by "
                         "default: the pass/fail criterion of this test is "
                         "behavioural equivalence, the structural scan is "
                         "reported as evidence only because Verilator's "
                         "flattened naming is version dependent.")
    opts = ap.parse_args()

    ver = verilator_version()
    if not ver:
        sys.stderr.write("verilator not found on PATH\n")
        return 1
    print("test_nonblock_fifo_ram: {}".format(ver))

    sources = [require(ROOT / p) for p in REQUIRED_SOURCES]
    for p in OPTIONAL_SOURCES:
        cand = ROOT / p
        if cand.exists():
            sources.append(cand)
        else:
            print("test_nonblock_fifo_ram: note: optional source absent: {}".format(p))

    cpp = require(ROOT / CPP)

    OUT.mkdir(parents=True, exist_ok=True)
    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
        "--top-module", TOP,
        "--Mdir", str(obj_dir),
    ]
    for inc in INCDIRS:
        args.append("-I{}".format(ROOT / inc))
    args += [str(p) for p in sources]
    args += [str(cpp)]

    print("test_nonblock_fifo_ram: build: {}".format(" ".join(args)))

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)

    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        sys.stderr.write("test_nonblock_fifo_ram: build FAILED\n")
        return 1

    exe = obj_dir / "V{}".format(TOP)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.stderr.write("test_nonblock_fifo_ram: verilator did not produce {}\n".format(exe))
        return 1

    evidence = scan_branch_evidence(obj_dir)
    for key in sorted(BRANCH_PATTERNS):
        print("test_nonblock_fifo_ram: branch-evidence {:10s} {}".format(
            key, evidence.get(key, "-")))

    if opts.strict_branch and "dut_ram" not in evidence:
        sys.stderr.write(
            "test_nonblock_fifo_ram: --strict-branch: the DUT instance does not "
            "appear to be on the FPGA RAM storage branch "
            "(gen_fpga_queue not found in the generated model)\n")
        return 2

    run_args = [str(exe)]
    if opts.scenario:
        run_args.append("+scenario={}".format(opts.scenario))

    print("test_nonblock_fifo_ram: run: {}".format(" ".join(run_args)))
    run = subprocess.run(run_args)
    if run.returncode != 0:
        print("test_nonblock_fifo_ram: FAIL (rc={})".format(run.returncode))
        return run.returncode

    print("test_nonblock_fifo_ram: PASS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
```

---

## 5. Proposed Chinese documentation append

Append inside `docs/research-n-prototype-spec-20261004.md`, in the `## v2更新` section under `### 测试侧状态` (after the existing "尚未执行" bullet list). Nothing below claims a result.

```markdown
- 新增 FIFO 存储等价测试（与 iDMA 数据通路无关，只针对生产 FIFO 的存储实现切换）：`tests/nonblock_fifo_ram_top.sv` 用测试专用 `ariane_pkg::FPGA_EN=0`，实例化生产 `idma_stream_fifo`（Depth 512、8 bit）为 DUT、同一个生产 `stream_fifo`（Depth 512、8 bit、`FORCE_FPGA_RAM=0`）为参考、再加一个 `FORCE_FPGA_RAM=1` 的 A/B 锚点和一个 Depth 3 车道守门；`tests/nonblock_fifo_ram_test.cpp` 在低电平时采样并用独立软件 deque 记分板（推入只在非满、弹出只在非空，均按本拍起始状态判定）核对 valid/ready/usage/数据顺序，同时逐拍比对 DUT 与参考、并施加持续出向反压验证负载稳定；场景包括复位空闲、512 次确定性填满与一次被拒推入/满停滞、512 排空、多次环绕与错位分段、同拍推入弹出、占用中 flush 与占用中复位后重填排空；`tests/test_nonblock_fifo_ram.py` 负责 Verilator 4.038 构建与运行。数据仅在 `valid_o` 有效时比对：复位/flush 后无效存储条目允许不同（寄存器阵列复位清零，AsyncDpRam 不清零），该差异不通过流接口可观测。该测试只证明流接口行为等价，不证明 DUT 内部确实例化了 AsyncDpRam（runner 默认只打印结构性证据，`--strict-branch` 才作为失败条件），也不构成任何通用 DMA/AXI 进度或上板结论。命令与预期输出（尚未运行）见下。
  - `python3 tests/test_nonblock_fifo_ram.py`
  - `python3 tests/test_nonblock_fifo_ram.py --strict-branch`
```

---

## 6. Acceptance commands and expected (not measured) output

```bash
cd <repo root>
python3 tests/test_nonblock_fifo_ram.py
python3 tests/test_nonblock_fifo_ram.py --strict-branch
python3 tests/test_nonblock_fifo_ram.py --scenario fill512-and-full-stall
```

Expected shape (template only — **not** a measured result):

```
test_nonblock_fifo_ram: Verilator 4.038 ...
test_nonblock_fifo_ram: build: verilator --cc --exe --build -Wno-fatal +define+ASSERTS_OFF --top-module nonblock_fifo_ram_top --Mdir .../out/nonblock-fifo-ram/obj -I.../common_cells/include -I.../iDMA/src/include .../fifo_v3.sv .../stream_fifo.sv .../AsyncDpRam.sv .../idma_stream_fifo.sv .../tests/nonblock_fifo_ram_top.sv .../tests/nonblock_fifo_ram_test.cpp
test_nonblock_fifo_ram: branch-evidence ab_ram     Vnonblock_fifo_ram_top___024root.h
test_nonblock_fifo_ram: branch-evidence dut_ram    Vnonblock_fifo_ram_top___024root.h
...
nonblock_fifo_ram_test: scenario reset-idle                       PASS (cycles=..., occ dut=0 ref=0 ab=0 lane3=0)
nonblock_fifo_ram_test: scenario fill512-and-full-stall           PASS (...)
nonblock_fifo_ram_test: scenario drain512                         PASS (...)
nonblock_fifo_ram_test: scenario wrap-multiple-times              PASS (...)
nonblock_fifo_ram_test: scenario simultaneous-push-pop            PASS (...)
nonblock_fifo_ram_test: scenario output-backpressure-stable       PASS (...)
nonblock_fifo_ram_test: scenario flush-while-occupied-refill      PASS (...)
nonblock_fifo_ram_test: scenario reset-while-occupied-refill       PASS (...)
nonblock_fifo_ram_test: scenario depth3-lane-guard                PASS (...)
nonblock_fifo_ram_test: 9/9 scenarios passed, <N> checks, <M> cycles, invalid-storage-data-diffs=<K>, stalled-output-cycles=<S>
test_nonblock_fifo_ram: PASS
```

Exit codes: `0` = all 9 scenarios passed; `1` = at least one scenario failed or the build failed; `2` = no scenario matched / `--strict-branch` structural failure.

---

## 7. Coverage and limitations

**Covered**
- READ-side: `valid_o`, `data_o` head value, `ready_o`, `usage_o` against an independent `std::deque` and against the two other storage implementations, every cycle.
- `usage_o` 9-bit truncation at full (512 → 0) and the depth-3 width (`2` bits → full reads `3`).
- Full behaviour: 512 deterministic pushes with the output blocked, an extra refused push held for several cycles, and the "full at cycle start ⇒ push refused even though a pop frees a slot this cycle" corner (`fifo_v3.sv:62, :86, :107`).
- Drain order of all 512 entries, plus no-pop on an empty FIFO.
- Write/read pointer wrap: 3 complete wraps plus 6 phase-shifted partial fill/drain phases that move both pointers off the wrap boundary.
- Simultaneous push and pop for 1000 consecutive cycles with occupancy held at 256 and equal push/pop counts.
- Sustained output backpressure: 300 cycles of `ready_i=0` with the payload held, enforced by an AXI-style stability rule, plus more pushes while the output stays blocked.
- Synchronous flush while occupied (2 cycles), then flag/usage state and a full refill+drain.
- Asynchronous reset while occupied, then a 5-entry partial refill (the case where the register array is 0 and AsyncDpRam still holds pre-reset data — this is exactly why data is compared only when `valid_o`), then a full refill+drain.
- Depth-3 lane guard: fill/refuse/push+pop/flush/refill for a FIFO that must not be opted in.
- Direct flag A/B: `stream_fifo` FORCE=0 vs FORCE=1 through the same stimulus.

**Not covered / limitations**
- **L1** The test cannot see which internal storage branch the *DUT* selected; it verifies stream behaviour only. If Codex implements the `idma_stream_fifo` opt-in as a parameter defaulting to 0 instead of the described `Depth == 512 && $bits(type_t) == 8` local rule, the DUT would silently stay on the register branch and this test would still pass. The runner prints a structural scan of the flattened model (`gen_fpga_queue` / `gen_asic_queue`) and `--strict-branch` turns it into a failure condition, but Verilator's flattened naming is version dependent; confirm the branch selection by review/lint as the primary evidence.
- **L2** No synthesis, no timing, no area/LUT/BRAM check. This says nothing about the v2 placement failure; it only shows the storage change is functionally transparent.
- **L3** Only `Depth == 512`, 8-bit, `FALL_THROUGH = 0` and the depth-3 lane are exercised. The `Depth == 2` spill-register branch (`idma_stream_fifo.sv:52-86`), other depths and other types are not covered.
- **L4** Verilator 4.038 without `--assert` does not check the RTL SVA (`fifo_v3.sv:181-187`, `idma_stream_fifo.sv:83-85, :123-125`, `AsyncDpRam.sv:57-59`); the equivalent stream-level rules are re-implemented in C++. `+define+ASSERTS_OFF` is passed as required, but it does not influence `common_cells/assertions.svh`, which already disables `INC_ASSERT` under `VERILATOR` (`assertions.svh:28-34`).
- **L5** No DDR/DMA/AXI/IOMMU involvement and no claim about iDMA end-to-end progress, throughput, deadlock freedom, or any board behaviour.
- **L6** The test intentionally drives `valid_i` high while `ready_o` is low (the refused-push case). This is legal at the stream level (`stream_fifo.sv:42`) and the corresponding RTL assertion is inert/disabled; do not re-enable `ASSERTS` on top of this harness without re-checking that construct.
- **L7** The harness does not build against the production tree as received — the new parameter and the internal opt-in rule must exist first.

---

## 8. Source evidence (`file:line`)

- `fifo_v3.sv:13-19` parameter list (new `FORCE_FPGA_RAM` must be appended **after** `ADDR_DEPTH`); `fifo_v3.sv:56` `usage_o = status_cnt_q[ADDR_DEPTH-1:0]`; `:62-63` full/empty definition from the start-of-cycle counter; `:73`, `:87`, `:152` the three `ariane_pkg::FPGA_EN` tests to be replaced; `:86` `push_i && ~full_o`; `:107` `pop_i && ~empty_o`; `:134-150` async reset / synchronous flush (flush clears pointers only); `:152-164` `AsyncDpRam` instantiation; `:166-172` register array cleared on reset; `:176-188` SVA guarded by `ifndef VERILATOR`.
- `stream_fifo.sv:13-22` parameters (new parameter appended last), `:42-45` `push/pop/ready_o/valid_o`, `:47-52` `fifo_v3` instantiation to forward the new parameter.
- `AsyncDpRam.sv:39-50` memory array with `'{default:0}` init, write on `Clk_CI`, asynchronous read (no reset of contents); `:57-59` unguarded `assert property` (covered by `-Wno-fatal`).
- `idma_stream_fifo.sv:11-19` parameters; `:40-46` depth<2 guard inside `IDMA_NONSYNTH_BLOCK`; `:52-86` the Depth 2 spill branch (`spill_register_flushable`, `:64-77`) that must not be elaborated for depth 512; `:93` `if (Depth > 32'd2)`; `:105-120` `stream_fifo` instantiation where the Depth512/8-bit opt-in belongs; `:123-125` `CheckFullPush`/`CheckEmptyPop` asserts.
- `idma/guard.svh:12-27` `IDMA_NONSYNTH_BLOCK` excludes the block when `VERILATOR` is defined; `common_cells/assertions.svh:28-34` `INC_ASSERT` stays undefined under `VERILATOR`; `:105-111` `ASSERT_NEVER`.
- `tests/test_nonblock_coupled.py:127-146` Verilator invocation precedent (`--cc --exe --build -Wno-fatal +define+ASSERTS_OFF --top-module --Mdir -G...`), `:148-170` build-log capture / exe name / return-code propagation.
- `tests/nonblock_coupled_test.cpp:561-689` clk-low drive → eval → pre-edge snapshot → rising edge → model-update discipline; `:72-75` `verilated.h` + `sc_time_stamp`.
- `docs/research-n-prototype-spec-20261004.md:117-146` v2 context (512-beat/lane buffer, 4 KiB job shape, `+couple-depth=3|512`, "尚未执行" status) — the proposed Chinese paragraph is written to be appended in this section.

---

## 9. Unresolved items for Codex

1. Confirm where the `Depth > 2` opt-in expression lives (localparam inside `idma_stream_fifo` vs a new parameter). The test assumes an internal rule and passes no new parameter to `idma_stream_fifo`; if a parameter is used, add `.FORCE_FPGA_RAM(1'b1)` (or the chosen name) to `i_dut_stream_fifo` in `tests/nonblock_fifo_ram_top.sv` and, if it is a new *interface* decision, treat that as an architecture change (not this test's call).
2. Confirm the exact `spill_register_flushable.sv` / `spill_register.sv` paths, or confirm Verilator 4.038 does not require them for the unelaborated Depth 2 branch; the runner tolerates their absence with a printed note.
3. Confirm that `-Wno-fatal` suffices for the unguarded `assert property` in `AsyncDpRam.sv:57-59` on the exact Verilator 4.038 build in use.
4. Decide whether `--strict-branch` should become the default once the flattened naming has been observed once.