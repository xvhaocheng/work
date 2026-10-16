# Bounded implementation draft — read→write coupled nonblocking test, runner, and focused analyzer/runner guards

## 0. Scope, boundaries and what I did **not** do

- **No RTL behaviour change.** The only change proposed to a `.sv` file is a *test-only* debug-port addition to `tests/nonblock_banked_top.sv` (focused diff, §4). `rv_iommu_banked_ctl`, `rv_iommu_tl_wrap`, all `rtl/**`, `idma_backend`, `idma_axi_transport_layer` and `dma_core_wrap` are untouched.
- **I did not execute anything.** No `verilator`, no `python3`, no board. Nothing below is claimed to pass. Every "should" is an acceptance intent for Codex to run.
- **I did not re-run, re-score or delete any existing raw data.** The two script patches are *additive guards*; failed raw counters stay on disk and are simply marked unqualified (the existing `problems`/`qualified_reasons` machinery already does that).
- The model in `tests/nonblock_coupled_test.cpp` is a **test harness**. It is **not DDR** and **not the real iDMA backend**: it has no XBAR, no `rv_iommu_ds_if` arbitration, no real ID remap and no board `axi_delayer`. I state this in the file header and in §7.
- **Harness config decision (documented, not silently changed).** `tests/nonblock_banked_top.sv:319-320` uses `NumOutstandingTrans=4`, `WFifoDepth=8` → per-bank OT=2, W=4. The board transfer FIFO is 32 split 16/bank. I **left the harness cfg unchanged** because (a) changing `test_cfg()` alters the already-passing `tests/nonblock_banked_test.cpp` config, and (b) the coupled test exercises the read→write dependency in a *per-DMA 3-beat* coupling FIFO that lives in the C++ model, not in the DUT. If Codex wants board-faithful per-bank depths, the one-line change is `c.NumOutstandingTrans = 8; c.WFifoDepth = 32;` at `tests/nonblock_banked_top.sv:319-320`; that is a separate, revertible decision.

---

## 1. `tests/nonblock_coupled_test.cpp` (new, complete)

```cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_coupled_test.cpp
//
// Read->write COUPLED copy test around the real
//     rv_iommu_banked_ctl  <->  rv_iommu_tl_wrap
// harness (tests/nonblock_banked_top.sv).
//
// Why this file exists
// --------------------
// tests/nonblock_banked_test.cpp pre-generates every W beat from an expected
// payload and serves the completion / DDTW / PTW ports from independent
// memories.  It therefore never reproduces the read->write dependency of a
// real copy engine.  This test does:
//
//   * every W beat a DMA puts on the wire is popped from a small FIFO that is
//     filled ONLY by source R beats that actually came back through the DUT
//     -- nothing is synthesised from the expected payload;
//   * the completion port, the DDTW port and the PTW port are served by ONE
//     shared memory through a single ordered (head-of-line) R-return queue;
//   * each DMA has a limited 3-beat read->write coupling FIFO, so the DUT's
//     R-ready is throttled by real downstream room, not by a pre-filled queue.
//
// This is a test harness model.  It is NOT DDR and NOT the real iDMA backend:
// it has no XBAR, no rv_iommu_ds_if arbitration, no board axi_delayer and no ID
// remap.  It only reproduces the ordering/back-pressure shape of the board.
//
// Exit status: 0 on PASS, non-zero on FAIL or bounded STALL.  A stall prints
// the last AR/AW/W/R and the observed TL state instead of aborting silently.

#include "Vnonblock_banked_top.h"
#include "verilated.h"

double sc_time_stamp() { return 0; }

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <map>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// identifiers / addresses / sizes
// ---------------------------------------------------------------------------
static const uint32_t V_ID      = 0;   // bank 0
static const uint32_t A_ID      = 4;   // bank 1
static const uint32_t V_DID     = 10;
static const uint32_t A_DID     = 11;
static const uint32_t BANK_BIT  = 2;   // BankIdBit == 2  (harness :578)

static const uint64_t V_SRC_IOVA = 0x10000000ULL;
static const uint64_t V_DST_IOVA = 0x10001000ULL;
static const uint64_t V_SRC_PA   = 0x80000000ULL;
static const uint64_t V_DST_PA   = 0x80001000ULL;

static const uint64_t A_IOVA_BASE   = 0x20000000ULL;
static const uint64_t A_SRC_PA_BASE = 0x90000000ULL;  // A source pages
static const uint64_t A_DST_PA_BASE = 0x90100000ULL;  // A destination pages

static const uint32_t PAGE_BEATS   = 512;   // 4 KiB / 8 B
static const uint32_t BURST_BEATS  = 256;   // 2 KiB / 8 B
static const uint32_t BURST_LEN    = 255;   // -> 256 beats
static const uint32_t COUPLE_DEPTH = 3;     // per-DMA read->write FIFO
static const uint32_t WARM_ROUNDS  = 16;
static const uint32_t JOB_COUNT    = 64;

// Scenario-level hard cycle cap.  A copy is ~2 k cycles per job; 64 jobs plus
// 16 warm rounds is far below this, so exceeding it is a real stall.
static const long SCENARIO_BUDGET = 4000000L;

// ---------------------------------------------------------------------------
// deterministic, injective payload words
// ---------------------------------------------------------------------------
static uint64_t make_test_word(uint64_t word_index) {
    return (word_index * 0x9E3779B97F4A7C15ULL) ^ 0xA5C3E10D2B4F9678ULL;
}
// page 0 .. 16, word 0 .. 511 -> unique 64-bit pattern
static uint64_t page_word(uint32_t page, uint32_t i) {
    return make_test_word(((uint64_t)page << 9) | (uint64_t)i);
}
// pre-fill of a destination page: every byte differs from the copied data
static uint64_t page_old(uint32_t page, uint32_t i) {
    return 0xD000000000000000ULL ^ ((uint64_t)(page + 1) << 40) ^ (uint64_t)i;
}

static void fail(const char* msg, const char* file, int line) {
    std::fprintf(stderr, "nonblock_coupled_test: FAIL: %s (%s:%d)\n",
                 msg, file, line);
    std::exit(1);
}
#define CHECK(cond, msg) do { if (!(cond)) fail((msg), __FILE__, __LINE__); } while (0)

struct Beat {
    uint64_t data = 0;
    uint8_t  strb = 0;
    bool     last = false;
    uint32_t id   = 0;
};

// ---------------------------------------------------------------------------
class Tb {
public:
    Vnonblock_banked_top* top;

    // ---------------- shared memory -------------------------------------
    std::map<uint64_t, uint8_t> mem;

    enum Port { P_COMP = 0, P_DDTW = 1, P_PTW = 2 };
    struct MemRead {
        Port     port    = P_COMP;
        uint64_t addr    = 0;
        uint8_t  len     = 0;
        uint8_t  id      = 0;
        uint32_t beat    = 0;
        int      latency = 1;
    };
    // SINGLE ordered R-return queue for completion + DDTW + PTW (HOL).
    std::deque<MemRead> mem_q;

    struct CawEnt { uint32_t id = 0; uint64_t addr = 0; uint8_t len = 0; uint32_t got = 0; };
    std::deque<CawEnt>   caw_q;     // completion AWs being written by the memory model
    std::deque<uint32_t> cb_q;      // completion B ids ready to return
    struct ExpAw { uint32_t id = 0; uint64_t addr = 0; uint8_t len = 0; };
    std::deque<ExpAw>    exp_aw_q;  // AWs the test itself issued upstream

    // ---------------- upstream DMA model --------------------------------
    struct Dma {
        bool     used     = false;
        uint32_t id       = 0;
        uint32_t did      = 0;
        uint64_t src_iova = 0, dst_iova = 0;
        uint64_t src_pa   = 0, dst_pa   = 0;
        uint32_t src_page = 0;

        int      half = 0;          // 0 or 1; 2 -> job done
        bool     job_done = true;

        bool     ar_want = false, ar_done = false;
        bool     aw_want = false, aw_done = false;
        uint32_t rx      = 0;       // source R beats consumed this half
        bool     r_last  = false;
        uint32_t wx      = 0;       // W beats emitted this half
        bool     b_done  = false;

        std::deque<Beat> cfifo;     // read->write coupling FIFO (<= COUPLE_DEPTH)
    };
    Dma dma[2];                     // [0]=V (bank0), [1]=A (bank1)

    std::deque<int> aw_order;       // accepted-AW order == DUT demux W order
    int w_owner = -1;

    int cur_ar_d = -1, cur_aw_d = -1;

    // ---------------- tracing -------------------------------------------
    struct RecAr { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
    struct RecAw { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
    struct RecW  { long cyc; uint32_t id; uint64_t data; uint8_t strb; bool last; };
    struct RecR  { long cyc; uint32_t id; uint64_t data; bool last; uint32_t resp; };
    struct RecB  { long cyc; uint32_t id; uint32_t resp; };

    std::vector<RecAr> up_ar, cmp_ar;
    std::vector<RecAw> up_aw, cmp_aw;
    std::vector<RecW>  up_w,  cmp_w;
    std::vector<RecR>  up_r;
    std::vector<RecB>  up_b;

    bool nonblock_enable = true;
    long g_cycle         = 0;
    long scenario_start  = 0;
    uint32_t ptw_latency = 1;
    uint32_t a_pages_    = 0;

    // stability / liveness observation
    bool     last_r_valid = false;
    uint64_t last_r_data  = 0;
    uint32_t last_r_id    = 0;
    bool     last_r_last  = false;

    uint64_t n_comp_ar = 0, n_ddtw_ar = 0, n_ptw_ar = 0;
    uint64_t n_tl_accept = 0, n_tl_resp = 0;
    uint64_t n_ptw_active = 0, n_iotlb_lookup = 0, n_iotlb_hit = 0;
    uint64_t n_r_beats = 0, n_b_up = 0;

    // warm-up plumbing
    bool     warm_active = false;
    bool     warm_sent   = false;
    uint32_t warm_id     = 0;
    uint32_t warm_did    = 0;
    uint64_t warm_iova   = 0;

    Tb() : top(new Vnonblock_banked_top) {}
    ~Tb() { top->final(); delete top; }

    // ---------------------------------------------------------------------
    // memory
    // ---------------------------------------------------------------------
    void write64(uint64_t addr, uint64_t data) {
        for (int i = 0; i < 8; ++i)
            mem[addr + i] = (uint8_t)((data >> (8 * i)) & 0xFF);
    }
    uint64_t read64(uint64_t addr) {
        uint64_t data = 0;
        for (int i = 0; i < 8; ++i) {
            auto it = mem.find(addr + i);
            if (it == mem.end())
                fail("unmapped read", __FILE__, __LINE__);
            data |= (uint64_t)it->second << (8 * i);
        }
        return data;
    }
    void write_beat(uint64_t addr, uint64_t data, uint8_t strb) {
        for (int i = 0; i < 8; ++i)
            if (strb & (1 << i))
                mem[addr + i] = (uint8_t)((data >> (8 * i)) & 0xFF);
    }

    // ---------------------------------------------------------------------
    // devices / page tables
    // ---------------------------------------------------------------------
    void setup_memory(uint32_t a_pages) {
        mem.clear();
        a_pages_ = a_pages;

        // Extended 1-level DDT at 0x10000, stride 64.
        // DID10 -> 0x10280, DID11 -> 0x102C0.
        write64(0x10280, 0x0000000000000001ULL);
        write64(0x10288, 0x0000000000000000ULL);
        write64(0x10290, 0x000000000000A000ULL);
        write64(0x10298, 0x8000000000000020ULL); // Sv39, V root 0x20000
        write64(0x102A0, 0x0ULL);
        write64(0x102A8, 0x0ULL);
        write64(0x102B0, 0x0ULL);
        write64(0x102B8, 0x0ULL);

        write64(0x102C0, 0x0000000000000001ULL);
        write64(0x102C8, 0x0000000000000000ULL);
        write64(0x102D0, 0x000000000000B000ULL);
        write64(0x102D8, 0x8000000000000030ULL); // Sv39, A root 0x30000
        write64(0x102E0, 0x0ULL);
        write64(0x102E8, 0x0ULL);
        write64(0x102F0, 0x0ULL);
        write64(0x102F8, 0x0ULL);

        // V page tables: root 0x20000, L1V 0x40000, L0V 0x50000.
        write64(0x20000, 0x0000000000010001ULL);
        write64(0x40400, 0x0000000000014001ULL);
        write64(0x50000, 0x00000000200000DFULL); // Vsrc -> 0x80000000
        write64(0x50008, 0x00000000200004DFULL); // Vdst -> 0x80001000

        // A page tables: root 0x30000, L1A 0x60000, L0A 0x70000.
        write64(0x30000, 0x0000000000018001ULL);
        write64(0x60800, 0x000000000001C001ULL);
        for (uint32_t n = 0; n < 2 * a_pages; ++n) {
            uint64_t pa = (n < a_pages)
                        ? (A_SRC_PA_BASE + (uint64_t)n * 0x1000)
                        : (A_DST_PA_BASE + (uint64_t)(n - a_pages) * 0x1000);
            write64(0x70000 + (uint64_t)n * 8, ((pa >> 12) << 10) | 0xDFULL);
        }

        // V source content + unique destination pre-fill.
        for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
            write64(V_SRC_PA + (uint64_t)i * 8, page_word(0, i));
            write64(V_DST_PA + (uint64_t)i * 8, page_old(0, i));
        }
        // A source content + unique destination pre-fill.
        for (uint32_t p = 0; p < a_pages; ++p) {
            for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
                write64(A_SRC_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                        page_word(1 + p, i));
                write64(A_DST_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                        page_old(1 + p, i));
            }
        }
    }

    // ---------------------------------------------------------------------
    // upstream presentation
    // ---------------------------------------------------------------------
    void drive_upstream() {
        top->tr_aw_valid = 0;
        top->tr_ar_valid = 0;
        top->tr_w_valid  = 0;
        top->tr_b_ready  = 1;
        top->tr_r_ready  = 1;
        cur_ar_d = -1;
        cur_aw_d = -1;

        // ---- AR : V first, then A, then the warm-up probe ----
        int ar_d = -1;
        for (int d = 0; d < 2; ++d)
            if (dma[d].used && !dma[d].job_done && dma[d].ar_want) { ar_d = d; break; }
        if (ar_d >= 0) {
            Dma& m = dma[ar_d];
            top->tr_ar_valid = 1;
            top->tr_ar_id    = m.id;
            top->tr_ar_addr  = m.src_iova + (uint64_t)m.half * 0x800;
            top->tr_ar_len   = (uint8_t)BURST_LEN;
            top->tr_ar_size  = 3;
            top->tr_ar_burst = 1;
            top->tr_ar_prot  = 0;
            top->tr_ar_user  = 0;
            top->tr_ar_did   = m.did;
            top->tr_ar_ssid_valid = 0;
            top->tr_ar_subid = 0;
            top->tr_ar_mmu_atst  = 0;
            top->tr_ar_mmu_valid = 0;
            cur_ar_d = ar_d;
        } else if (warm_active && !warm_sent) {
            top->tr_ar_valid = 1;
            top->tr_ar_id    = warm_id;
            top->tr_ar_addr  = warm_iova;
            top->tr_ar_len   = 0;
            top->tr_ar_size  = 3;
            top->tr_ar_burst = 1;
            top->tr_ar_prot  = 0;
            top->tr_ar_user  = 0;
            top->tr_ar_did   = warm_did;
            top->tr_ar_ssid_valid = 0;
            top->tr_ar_subid = 0;
            top->tr_ar_mmu_atst  = 0;
            top->tr_ar_mmu_valid = 0;
        }

        // ---- AW : A ahead of V, gated on real coupling data ----
        int aw_d = -1;
        for (int d = 1; d >= 0; --d) {
            if (!dma[d].used || dma[d].job_done) continue;
            if (!dma[d].aw_want || dma[d].aw_done) continue;
            if (dma[d].cfifo.empty()) continue;   // no data -> no AW yet
            aw_d = d;
            break;
        }
        if (aw_d >= 0) {
            Dma& m = dma[aw_d];
            top->tr_aw_valid = 1;
            top->tr_aw_id    = m.id;
            top->tr_aw_addr  = m.dst_iova + (uint64_t)m.half * 0x800;
            top->tr_aw_len   = (uint8_t)BURST_LEN;
            top->tr_aw_size  = 3;
            top->tr_aw_burst = 1;
            top->tr_aw_prot  = 0;
            top->tr_aw_user  = 0;
            top->tr_aw_did   = m.did;
            top->tr_aw_ssid_valid = 0;
            top->tr_aw_subid = 0;
            top->tr_aw_mmu_atst  = 0;
            top->tr_aw_mmu_valid = 0;
            cur_aw_d = aw_d;
        }

        // ---- W : only the current burst owner, popped from its cfifo ----
        if (w_owner >= 0 && !dma[w_owner].cfifo.empty()) {
            const Beat& b = dma[w_owner].cfifo.front();
            top->tr_w_valid = 1;
            top->tr_w_data  = b.data;
            top->tr_w_strb  = b.strb;
            top->tr_w_last  = b.last ? 1 : 0;
            top->tr_w_user  = 0;
        }
    }

    void drive_comp() {
        top->comp_aw_ready = 1;
        top->comp_ar_ready = (mem_q.size() < 16) ? 1 : 0;
        top->comp_w_ready  = 1;
        if (!cb_q.empty()) {
            top->comp_b_valid = 1;
            top->comp_b_id    = cb_q.front();
            top->comp_b_resp  = 0;
        } else {
            top->comp_b_valid = 0;
            top->comp_b_id    = 0;
            top->comp_b_resp  = 0;
        }
    }

    // ---------------------------------------------------------------------
    // shared memory R-return drive (ordered, head-of-line)
    // ---------------------------------------------------------------------
    void drive_mem_r() {
        top->comp_r_valid = 0;
        top->ddtw_r_valid = 0;
        top->ptw_r_valid  = 0;

        if (mem_q.empty()) return;
        MemRead& h = mem_q.front();
        if (h.latency > 0) { h.latency--; return; }

        switch (h.port) {
        case P_COMP: {
            uint32_t bank = (h.id >> BANK_BIT) & 1u;
            if (dma[bank].cfifo.size() >= COUPLE_DEPTH) return; // honour real FIFO room
            top->comp_r_valid = 1;
            top->comp_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
            top->comp_r_id    = h.id;
            top->comp_r_resp  = 0;
            top->comp_r_last  = (h.beat == h.len) ? 1 : 0;
            break;
        }
        case P_DDTW:
            top->ddtw_r_valid = 1;
            top->ddtw_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
            top->ddtw_r_id    = h.id;
            top->ddtw_r_resp  = 0;
            top->ddtw_r_last  = (h.beat == h.len) ? 1 : 0;
            break;
        case P_PTW:
            top->ptw_r_valid = 1;
            top->ptw_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
            top->ptw_r_id    = h.id;
            top->ptw_r_resp  = 0;
            top->ptw_r_last  = (h.beat == h.len) ? 1 : 0;
            break;
        }
    }

    // ---------------------------------------------------------------------
    // one cycle
    // ---------------------------------------------------------------------
    void tick() {
        top->nonblock_enable_i = nonblock_enable ? 1 : 0;

        for (int d = 0; d < 2; ++d) dma_progress(d);
        drive_upstream();
        drive_comp();
        drive_mem_r();
        top->ddtw_ar_ready = 1;
        top->ptw_ar_ready  = 1;

        top->eval();

        bool f_up_ar = top->tr_ar_valid && top->tr_ar_ready;
        bool f_up_aw = top->tr_aw_valid && top->tr_aw_ready;
        bool f_up_w  = top->tr_w_valid  && top->tr_w_ready;
        bool f_up_b  = top->tr_b_valid  && top->tr_b_ready;
        bool f_up_r  = top->tr_r_valid  && top->tr_r_ready;

        bool f_comp_ar = top->comp_ar_valid && top->comp_ar_ready;
        bool f_ddtw_ar = top->ddtw_ar_valid && top->ddtw_ar_ready;
        bool f_ptw_ar  = top->ptw_ar_valid  && top->ptw_ar_ready;
        bool f_comp_aw = top->comp_aw_valid && top->comp_aw_ready;
        bool f_comp_w  = top->comp_w_valid  && top->comp_w_ready;
        bool f_comp_r  = top->comp_r_valid  && top->comp_r_ready;
        bool f_ddtw_r  = top->ddtw_r_valid  && top->ddtw_r_ready;
        bool f_ptw_r   = top->ptw_r_valid   && top->ptw_r_ready;

        // ---- TL observation counters -----------------------------------
        if (top->dbg_tl_req_valid && top->dbg_tl_req_ready) ++n_tl_accept;
        if (top->dbg_tl_resp_valid && top->dbg_tl_resp_ready) ++n_tl_resp;
        if (top->dbg_tl_ptw_active) ++n_ptw_active;
        if (top->dbg_tl_iotlb_lookup) {
            ++n_iotlb_lookup;
            if (top->dbg_tl_iotlb_hit) ++n_iotlb_hit;
        }

        // ---- R stability (AXI: valid beat must not change until handshake)
        if (last_r_valid) {
            CHECK(top->tr_r_valid, "R valid dropped before handshake");
            CHECK(top->tr_r_data == last_r_data && top->tr_r_id == last_r_id &&
                  top->tr_r_last == last_r_last, "R beat changed while valid");
        }
        last_r_valid = top->tr_r_valid && !f_up_r;
        last_r_data  = top->tr_r_data;
        last_r_id    = top->tr_r_id;
        last_r_last  = top->tr_r_last;

        top->clk_i = 1;
        top->eval();

        // ---- posedge state updates -------------------------------------
        if (f_up_ar) {
            up_ar.push_back({g_cycle, top->tr_ar_id, top->tr_ar_addr, top->tr_ar_len});
            if (cur_ar_d >= 0) {
                dma[cur_ar_d].ar_want = false;
                dma[cur_ar_d].ar_done = true;
            } else if (warm_active) {
                warm_sent = true;
            }
        }
        if (f_up_aw) {
            up_aw.push_back({g_cycle, top->tr_aw_id, top->tr_aw_addr, top->tr_aw_len});
            ExpAw e; e.id = top->tr_aw_id; e.addr = top->tr_aw_addr; e.len = top->tr_aw_len;
            exp_aw_q.push_back(e);
            if (cur_aw_d >= 0) {
                dma[cur_aw_d].aw_want = false;
                dma[cur_aw_d].aw_done = true;
                aw_order.push_back(cur_aw_d);
                if (w_owner < 0) w_owner = aw_order.front();
            }
        }
        if (f_up_w) {
            CHECK(w_owner >= 0 && !dma[w_owner].cfifo.empty(), "W fire without owner data");
            Dma& m = dma[w_owner];
            Beat b = m.cfifo.front();
            m.cfifo.pop_front();
            m.wx++;
            up_w.push_back({g_cycle, m.id, b.data, b.strb, b.last});
            if (b.last) {
                CHECK(m.wx == BURST_BEATS, "W burst length != BURST_BEATS");
                CHECK(!aw_order.empty() && aw_order.front() == w_owner,
                      "W owner is not at the AW head");
                aw_order.pop_front();
                w_owner = aw_order.empty() ? -1 : aw_order.front();
            }
        }
        if (f_up_b) {
            uint32_t bank = (top->tr_b_id >> BANK_BIT) & 1u;
            ++n_b_up;
            dma[bank].b_done = true;
            up_b.push_back({g_cycle, top->tr_b_id, (uint32_t)top->tr_b_resp});
        }
        if (f_up_r) {
            CHECK(top->tr_r_resp == 0, "upstream R resp not OKAY");
            uint32_t bank = (top->tr_r_id >> BANK_BIT) & 1u;
            Dma& m = dma[bank];
            if (m.used && !m.job_done) {
                Beat b; b.data = top->tr_r_data; b.strb = 0xFF;
                b.last = top->tr_r_last; b.id = top->tr_r_id;
                m.cfifo.push_back(b);
                CHECK(m.cfifo.size() <= COUPLE_DEPTH, "coupling FIFO overflowed");
                m.rx++;
                if (b.last) m.r_last = true;
            }
            up_r.push_back({g_cycle, top->tr_r_id, top->tr_r_data,
                            (bool)top->tr_r_last, (uint32_t)top->tr_r_resp});
            ++n_r_beats;
        }

        if (f_comp_ar) {
            cmp_ar.push_back({g_cycle, top->comp_ar_id, top->comp_ar_addr,
                              top->comp_ar_len});
            ++n_comp_ar;
            MemRead r; r.port = P_COMP; r.addr = top->comp_ar_addr;
            r.len = (uint8_t)top->comp_ar_len; r.id = (uint8_t)top->comp_ar_id;
            r.beat = 0; r.latency = 1;
            mem_q.push_back(r);
        }
        if (f_ddtw_ar) {
            ++n_ddtw_ar;
            MemRead r; r.port = P_DDTW; r.addr = top->ddtw_ar_addr;
            r.len = (uint8_t)top->ddtw_ar_len; r.id = (uint8_t)top->ddtw_ar_id;
            r.beat = 0; r.latency = 1;
            mem_q.push_back(r);
        }
        if (f_ptw_ar) {
            ++n_ptw_ar;
            MemRead r; r.port = P_PTW; r.addr = top->ptw_ar_addr;
            r.len = (uint8_t)top->ptw_ar_len; r.id = (uint8_t)top->ptw_ar_id;
            r.beat = 0; r.latency = (int)ptw_latency;
            mem_q.push_back(r);
        }
        if (f_comp_aw) {
            cmp_aw.push_back({g_cycle, top->comp_aw_id, top->comp_aw_addr,
                              top->comp_aw_len});
            CHECK(!exp_aw_q.empty(), "completion AW without a matching upstream AW");
            ExpAw e = exp_aw_q.front();
            CHECK(e.id == top->comp_aw_id && e.addr == top->comp_aw_addr &&
                  e.len == top->comp_aw_len, "completion AW does not match upstream AW");
            exp_aw_q.pop_front();
            CawEnt c; c.id = top->comp_aw_id; c.addr = top->comp_aw_addr;
            c.len = (uint8_t)top->comp_aw_len; c.got = 0;
            caw_q.push_back(c);
        }
        if (f_comp_w) {
            cmp_w.push_back({g_cycle, 0, top->comp_w_data,
                             (uint8_t)top->comp_w_strb, (bool)top->comp_w_last});
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            CawEnt& c = caw_q.front();
            CHECK(c.got < (uint32_t)c.len + 1, "too many completion W beats");
            write_beat(c.addr + (uint64_t)c.got * 8, top->comp_w_data,
                       (uint8_t)top->comp_w_strb);
            c.got++;
            if (top->comp_w_last) {
                CHECK(c.got == (uint32_t)c.len + 1, "completion W burst length mismatch");
                cb_q.push_back(c.id);
                caw_q.pop_front();
            }
        }
        if (f_comp_r || f_ddtw_r || f_ptw_r) {
            MemRead& h = mem_q.front();
            CHECK(h.port == (f_comp_r ? P_COMP : (f_ddtw_r ? P_DDTW : P_PTW)),
                  "R handshake on an unexpected port");
            if (h.beat == h.len) mem_q.pop_front();
            else                 h.beat++;
        }

        top->clk_i = 0;
        top->eval();
        ++g_cycle;
    }

    // ---------------------------------------------------------------------
    // DMA book-keeping
    // ---------------------------------------------------------------------
    void dma_progress(int d) {
        Dma& m = dma[d];
        if (!m.used || m.job_done) return;
        if (m.half >= 2) { m.job_done = true; return; }

        if (!m.ar_want && !m.ar_done) m.ar_want = true;
        if (m.ar_done && !m.aw_done && !m.aw_want && !m.cfifo.empty()) m.aw_want = true;

        if (m.ar_done && m.r_last && m.rx == BURST_BEATS &&
            m.aw_done && m.wx == BURST_BEATS && m.b_done) {
            m.half++;
            if (m.half >= 2) { m.job_done = true; return; }
            m.ar_want = false; m.ar_done = false;
            m.aw_want = false; m.aw_done = false;
            m.rx = 0; m.r_last = false;
            m.wx = 0; m.b_done = false;
        }
    }

    void start_job(int d, uint32_t idx) {
        Dma& m = dma[d];
        CHECK(m.cfifo.empty(), "start_job with a non-empty coupling FIFO");
        m.used = true;
        m.half = 0;
        m.job_done = false;
        m.ar_want = false; m.ar_done = false;
        m.aw_want = false; m.aw_done = false;
        m.rx = 0; m.r_last = false;
        m.wx = 0; m.b_done = false;
        if (d == 0) {
            m.id = V_ID; m.did = V_DID;
            m.src_iova = V_SRC_IOVA; m.dst_iova = V_DST_IOVA;
            m.src_pa = V_SRC_PA; m.dst_pa = V_DST_PA;
            m.src_page = 0;
        } else {
            uint32_t p = idx % a_pages_;
            m.id = A_ID; m.did = A_DID;
            m.src_iova = A_IOVA_BASE + (uint64_t)p * 0x1000;
            m.dst_iova = A_IOVA_BASE + (uint64_t)(a_pages_ + p) * 0x1000;
            m.src_pa = A_SRC_PA_BASE + (uint64_t)p * 0x1000;
            m.dst_pa = A_DST_PA_BASE + (uint64_t)p * 0x1000;
            m.src_page = 1 + p;
        }
    }

    // ---------------------------------------------------------------------
    // warm-up: one single-beat read per page to populate DDTC/IOTLB
    // ---------------------------------------------------------------------
    void warm_page(uint32_t id, uint32_t did, uint64_t iova) {
        warm_active = true; warm_sent = false;
        warm_id = id; warm_did = did; warm_iova = iova;
        size_t base_r = up_r.size();
        for (int i = 0; i < 200000; ++i) {
            tick();
            if (up_r.size() > base_r) { warm_active = false; return; }
        }
        warm_active = false;
        dump_stall("warm");
        std::exit(1);
    }

    void warm_round(uint32_t a_pages) {
        warm_page(V_ID, V_DID, V_SRC_IOVA);
        warm_page(V_ID, V_DID, V_DST_IOVA);
        for (uint32_t p = 0; p < a_pages; ++p) {
            warm_page(A_ID, A_DID, A_IOVA_BASE + (uint64_t)p * 0x1000);
            warm_page(A_ID, A_DID, A_IOVA_BASE + (uint64_t)(a_pages + p) * 0x1000);
        }
    }

    // ---------------------------------------------------------------------
    // stall dump
    // ---------------------------------------------------------------------
    void dump_stall(const std::string& where) {
        std::fprintf(stderr,
            "nonblock_coupled_test: STALL at %s cycle=%ld scenario_cycles=%ld\n",
            where.c_str(), g_cycle, g_cycle - scenario_start);
        std::fprintf(stderr,
            "  last up_ar=%s last up_aw=%s last up_w=%s last up_r=%s last up_b=%s\n",
            up_ar.empty() ? "(none)" : "yes", up_aw.empty() ? "(none)" : "yes",
            up_w.empty()  ? "(none)" : "yes", up_r.empty()  ? "(none)" : "yes",
            up_b.empty()  ? "(none)" : "yes");
        if (!up_ar.empty()) {
            const RecAr& r = up_ar.back();
            std::fprintf(stderr, "  AR id=%u addr=0x%016lx len=%u\n",
                         r.id, (unsigned long)r.addr, r.len);
        }
        if (!up_aw.empty()) {
            const RecAw& r = up_aw.back();
            std::fprintf(stderr, "  AW id=%u addr=0x%016lx len=%u\n",
                         r.id, (unsigned long)r.addr, r.len);
        }
        if (!up_w.empty()) {
            const RecW& r = up_w.back();
            std::fprintf(stderr, "  W  data=0x%016lx strb=0x%02x last=%d\n",
                         (unsigned long)r.data, r.strb, (int)r.last);
        }
        if (!up_r.empty()) {
            const RecR& r = up_r.back();
            std::fprintf(stderr, "  R  data=0x%016lx last=%d resp=%u\n",
                         (unsigned long)r.data, (int)r.last, r.resp);
        }
        std::fprintf(stderr,
            "  TL req_v=%d req_r=%d req_bank=%d resp_v=%d resp_r=%d resp_bank=%d "
            "ptw_active=%d ptw_r_fire=%d iotlb_look=%d iotlb_hit=%d\n",
            (int)top->dbg_tl_req_valid, (int)top->dbg_tl_req_ready,
            (int)top->dbg_tl_req_bank,  (int)top->dbg_tl_resp_valid,
            (int)top->dbg_tl_resp_ready,(int)top->dbg_tl_resp_bank,
            (int)top->dbg_tl_ptw_active,(int)top->dbg_tl_ptw_r_fire,
            (int)top->dbg_tl_iotlb_lookup,(int)top->dbg_tl_iotlb_hit);
        std::fprintf(stderr,
            "  mem_q=%zu caw_q=%zu cb_q=%zu exp_aw_q=%zu aw_order=%zu w_owner=%d "
            "cfifoV=%zu cfifoA=%zu in_flight=%d nonblock_busy=%d nonblock_fire=%d\n",
            mem_q.size(), caw_q.size(), cb_q.size(), exp_aw_q.size(),
            aw_order.size(), w_owner,
            dma[0].cfifo.size(), dma[1].cfifo.size(),
            (int)top->in_flight_o, (int)top->nonblock_busy_o,
            (int)top->nonblock_fire_o);
    }

    // ---------------------------------------------------------------------
    // reset / quiet
    // ---------------------------------------------------------------------
    void reset(bool enable) {
        nonblock_enable = enable;
        top->clk_i  = 0;
        top->rst_ni = 0;

        dma[0] = Dma(); dma[1] = Dma();
        aw_order.clear(); w_owner = -1;
        mem_q.clear(); caw_q.clear(); cb_q.clear(); exp_aw_q.clear();
        up_ar.clear(); up_aw.clear(); up_w.clear(); up_r.clear(); up_b.clear();
        cmp_ar.clear(); cmp_aw.clear(); cmp_w.clear();
        warm_active = false; warm_sent = false;
        last_r_valid = false;
        n_comp_ar = n_ddtw_ar = n_ptw_ar = 0;
        n_tl_accept = n_tl_resp = 0;
        n_ptw_active = n_iotlb_lookup = n_iotlb_hit = 0;
        n_r_beats = n_b_up = 0;

        for (int i = 0; i < 8; ++i) tick();
        top->rst_ni = 1;
        for (int i = 0; i < 4; ++i) tick();
    }

    void check_quiet(const char* tag) {
        for (int i = 0; i < 40; ++i) tick();
        CHECK(!top->in_flight_o, "in_flight_o not low after drain");
        CHECK(!top->nonblock_busy_o, "nonblock_busy_o not low after drain");
        CHECK(mem_q.empty() && caw_q.empty() && cb_q.empty() && exp_aw_q.empty(),
              "memory/AW queues not empty after drain");
        CHECK(dma[0].cfifo.empty() && dma[1].cfifo.empty(),
              "coupling FIFOs not empty after drain");
        (void)tag;
    }

    // ---------------------------------------------------------------------
    // one scenario
    // ---------------------------------------------------------------------
    void run_scenario(const char* name, bool enable, uint32_t a_pages,
                      uint32_t ptw_lat) {
        std::printf("== coupled scenario %s (runtime enable=%d, a_pages=%u, "
                    "ptw_latency=%u) ==\n", name, (int)enable, a_pages, ptw_lat);
        setup_memory(a_pages);
        reset(enable);
        ptw_latency    = ptw_lat;
        scenario_start = g_cycle;

        for (uint32_t r = 0; r < WARM_ROUNDS; ++r) warm_round(a_pages);

        uint32_t v_next = 0, a_next = 0;
        start_job(0, v_next++);
        if (a_pages) start_job(1, a_next++);

        while (v_next < JOB_COUNT || (a_pages && a_next < JOB_COUNT)) {
            tick();
            if (dma[0].job_done && v_next < JOB_COUNT) start_job(0, v_next++);
            if (a_pages && dma[1].job_done && a_next < JOB_COUNT) start_job(1, a_next++);
            if (g_cycle - scenario_start > SCENARIO_BUDGET) {
                dump_stall(std::string("scenario ") + name);
                std::exit(1);
            }
        }
        while (!dma[0].job_done || (a_pages && !dma[1].job_done)) {
            tick();
            if (g_cycle - scenario_start > SCENARIO_BUDGET) {
                dump_stall(std::string("scenario-drain ") + name);
                std::exit(1);
            }
        }

        check_quiet(name);

        // ---- exact byte verification -----------------------------------
        for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
            CHECK(read64(V_SRC_PA + (uint64_t)i * 8) == page_word(0, i),
                  "V source page was modified");
            CHECK(read64(V_DST_PA + (uint64_t)i * 8) == page_word(0, i),
                  "V destination bytes do not match the copied source");
        }
        for (uint32_t p = 0; p < a_pages; ++p) {
            for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
                CHECK(read64(A_SRC_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8)
                      == page_word(1 + p, i),
                      "A source page was modified");
                CHECK(read64(A_DST_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8)
                      == page_word(1 + p, i),
                      "A destination bytes do not match the copied source");
            }
        }

        // ---- mechanism guards (a run in which nothing moved is not a pass)
        CHECK(n_comp_ar > 0, "no completion reads observed");
        CHECK(n_r_beats > 0, "no upstream R beats observed");
        CHECK(n_b_up > 0, "no upstream B responses observed");
        CHECK(n_tl_accept > 0, "no translation accepted by the TL");
        CHECK(n_tl_resp > 0, "no translation response returned by the TL");
        CHECK(n_ddtw_ar + n_ptw_ar > 0, "no walker reads observed");

        std::printf("COUPLED-SUMMARY %s cycles=%ld comp_ar=%lu ddtw_ar=%lu ptw_ar=%lu "
                    "tl_accept=%lu tl_resp=%lu iotlb_look=%lu iotlb_hit=%lu "
                    "ptw_active=%lu r_beats=%lu b_up=%lu\n",
                    name, g_cycle - scenario_start,
                    (unsigned long)n_comp_ar, (unsigned long)n_ddtw_ar,
                    (unsigned long)n_ptw_ar, (unsigned long)n_tl_accept,
                    (unsigned long)n_tl_resp, (unsigned long)n_iotlb_lookup,
                    (unsigned long)n_iotlb_hit, (unsigned long)n_ptw_active,
                    (unsigned long)n_r_beats, (unsigned long)n_b_up);
    }
};

// ---------------------------------------------------------------------------
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Tb tb;

    // (runtime enable, aggressor pages, ptw latency)
    const struct { const char* name; bool en; uint32_t a; uint32_t lat; } plan[] = {
        { "mode1-solo-l1",  false, 0,  1 },
        { "mode1-solo-l31", false, 0, 31 },
        { "mode1-a1-l1",    false, 1,  1 },
        { "mode1-a1-l31",   false, 1, 31 },
        { "mode1-a8-l1",    false, 8,  1 },
        { "mode1-a8-l31",   false, 8, 31 },
        { "mode2-solo-l1",  true,  0,  1 },
        { "mode2-solo-l31", true,  0, 31 },
        { "mode2-a1-l1",    true,  1,  1 },
        { "mode2-a1-l31",   true,  1, 31 },
        { "mode2-a8-l1",    true,  8,  1 },
        { "mode2-a8-l31",   true,  8, 31 },
    };

    for (const auto& s : plan)
        tb.run_scenario(s.name, s.en, s.a, s.lat);

    std::printf("nonblock_coupled_test: PASS\n");
    return 0;
}
```

---

## 2. `tests/test_nonblock_coupled.py` (new, complete — reuses sibling `SOURCES`/`INCDIRS`, no copied RTL)

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the read->write coupled banked nonblocking test.

Builds tests/nonblock_coupled_test.cpp against exactly the same RTL source
list and include directories as tests/test_nonblock_banked.py.  No RTL file is
copied or duplicated: the source list is imported from the sibling runner.
"""

import importlib.util
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT  = ROOT / "out" / "nonblock-coupled"

TOP = "nonblock_banked_top"


def load_sibling():
    path = Path(__file__).resolve().parent / "test_nonblock_banked.py"
    if not path.is_file():
        sys.exit("sibling runner missing: {}".format(path))
    spec = importlib.util.spec_from_file_location("nonblock_banked_runner", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def tail(path: Path, lines: int = 200) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def require(path: Path) -> Path:
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p is None or p in seen:
            continue
        seen.add(p)
        out.append(p)
    return out


def main() -> int:
    sibling = load_sibling()
    tb = ROOT / "tests" / "nonblock_coupled_test.cpp"
    sources = dedup([require(p) for p in sibling.SOURCES])

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
        "--top-module",
        TOP,
        "--Mdir",
        str(obj_dir),
    ]
    for inc in sibling.INCDIRS:
        args += ["-I{}".format(inc)]
    args += [str(p) for p in sources]
    args += [str(tb)]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce {}\n".format(exe))
        return 1

    run = subprocess.run([str(exe)])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
```

---

## 3. Focused unified diff — `tests/nonblock_banked_top.sv` (test-only debug ports only)

Hunk 1 — add observation ports after `in_flight_o`:

```diff
--- a/tests/nonblock_banked_top.sv
+++ b/tests/nonblock_banked_top.sv
@@ -146,7 +146,19 @@
     input  logic        nonblock_enable_i,
     output logic        nonblock_fire_o,
     output logic        nonblock_busy_o,
-    output logic        in_flight_o
+    output logic        in_flight_o,
+
+    // ---------------- test-only observation (no DUT behaviour change) -------
+    // These ports only tap existing internal wires (trans_req_* / trans_resp_*
+    // of the joint harness and the rv_iommu_tl_wrap e3_tl observer).  They do
+    // not drive anything inside either DUT.
+    output logic        dbg_tl_req_valid,
+    output logic        dbg_tl_req_ready,
+    output logic        dbg_tl_req_bank,
+    output logic        dbg_tl_resp_valid,
+    output logic        dbg_tl_resp_ready,
+    output logic        dbg_tl_resp_bank,
+    output logic        dbg_tl_ptw_active,
+    output logic        dbg_tl_ptw_r_fire,
+    output logic        dbg_tl_iotlb_lookup,
+    output logic        dbg_tl_iotlb_hit
 );
```

Hunk 2 — declare the TL observer and the assign taps (placed next to the
existing invalidation wires, after the `trans_*` wire declarations at
`:360-365`):

```diff
@@ -544,6 +556,25 @@
     rv_iommu::iotlb_inval_t iotinval_i;
     rv_iommu::xdtc_inval_t  iodirinval_i;
+    rv_iommu::e3_tl_t       tl_e3;
     always_comb begin
         iotinval_i   = '0;
         iodirinval_i = '0;
     end
+
+    // Observation-only taps (see the port list).  Field names come from
+    // rv_iommu_tl_wrap.sv:689-703; trans_req_* / trans_resp_* are the harness
+    // wires declared above at :360-365.
+    assign dbg_tl_req_valid    = trans_req_valid;
+    assign dbg_tl_req_ready    = trans_req_ready;
+    assign dbg_tl_req_bank     = trans_req_data.bank;
+    assign dbg_tl_resp_valid   = trans_resp_valid;
+    assign dbg_tl_resp_ready   = trans_resp_ready;
+    assign dbg_tl_resp_bank    = trans_resp_data.bank;
+    assign dbg_tl_ptw_active   = tl_e3.ptw_active;
+    assign dbg_tl_ptw_r_fire   = tl_e3.ptw_r_fire;
+    assign dbg_tl_iotlb_lookup = tl_e3.lookup;
+    assign dbg_tl_iotlb_hit    = tl_e3.hit;
```

Hunk 3 — connect the TL observer (line 686, `.e3_o ()`):

```diff
@@ -683,7 +714,7 @@
         .e3_delay128_i          (1'b0),
-        .e3_o                   (),
+        .e3_o                   (tl_e3),
         .research_o             (),
```

**No other harness line changes.** In particular `test_cfg()` (`:316-351`),
`BankIdBit` (`:578`), the DUT instantiations and all tie-offs are untouched.

---

## 4. Focused unified patch — `scripts/mac_nonblock_pilot.py`

### 4.1 New helper (insert immediately before `validate_helper_window`, i.e. before line 423)

```diff
--- a/scripts/mac_nonblock_pilot.py
+++ b/scripts/mac_nonblock_pilot.py
@@ -420,6 +420,29 @@
     }
 
 
+def helper_mechanism_expectations(record: Any) -> Tuple[bool, bool]:
+    '''Which mechanism counters the helper window must show as non-zero.
+
+    A normal P4M case that ran victim transfers moved V, so the helper must
+    have counted at least one V accept; a case whose window reports has_a must
+    likewise show at least one A accept.  When the record cannot be read the V
+    guard stays armed, so an all-zero window can never be reported as a pass.
+    The raw window is never rewritten; the guard only adds problems.
+    '''
+    expect_v = True
+    expect_a = False
+    if isinstance(record, dict):
+        inp = record.get('input')
+        if isinstance(inp, dict):
+            transfers = inp.get('victim_transfers')
+            expect_v = (isinstance(transfers, int)
+                        and not isinstance(transfers, bool)
+                        and transfers > 0)
+        p4m = record.get('p4m')
+        if isinstance(p4m, dict):
+            expect_a = bool(p4m.get('has_a'))
+    return expect_v, expect_a
+
+
 def validate_helper_window(window: Any, label: str, expected_cycles: Any,
                            previous_seq: Optional[int]) -> List[str]:
```

### 4.2 `validate_helper_window` — add the two guard parameters and the liveness checks

```diff
@@ -423,8 +446,10 @@
-def validate_helper_window(window: Any, label: str, expected_cycles: Any,
-                           previous_seq: Optional[int]) -> List[str]:
+def validate_helper_window(window: Any, label: str, expected_cycles: Any,
+                           previous_seq: Optional[int],
+                           expect_v_movement: bool = True,
+                           expect_a: bool = False) -> List[str]:
     '''Validate the static-ELF helper window (ABI idma-nonblock-window-v1).
 
     The full 16-word window is kept verbatim elsewhere; nothing here is
-    reinterpreted as an HPM counter.
+    reinterpreted as an HPM counter.  The mechanism liveness guards below are
+    additive: an all-zero window satisfies window[14] == fast + normal
+    trivially, so without them a run in which nothing was counted at all would
+    be accepted as a pass.  The raw words are never modified.
     '''
@@ -452,6 +477,31 @@
     fast = sum(window[i] for i in READER_FAST_WORDS)
     normal = sum(window[i] for i in READER_NORMAL_WORDS)
     if window[14] != fast + normal:
         problems.append('helper accepted ' + repr(window[14]) + ' != fast '
                         + repr(fast) + ' + normal ' + repr(normal))
+
+    # --- mechanism liveness guards (banked labels only; see call site) ------
+    # V fast words: 6=Vread, 7=Vwrite; V normal word: 11
+    # A fast words: 8=Aread, 9=Awrite; A normal word: 12
+    if expect_v_movement or expect_a:
+        if window[14] <= 0:
+            problems.append('helper window[14] accepted is not > 0 although '
+                            'the case ran V/A transfers')
+    if expect_v_movement:
+        v_counts = (window[6], window[7], window[11])
+        if sum(v_counts) <= 0:
+            problems.append('helper V fast+normal counts ' + repr(v_counts)
+                            + ' are not sum > 0 although the case ran V transfers')
+    if expect_a:
+        a_counts = (window[8], window[9], window[12])
+        if sum(a_counts) <= 0:
+            problems.append('helper A fast+normal counts ' + repr(a_counts)
+                            + ' are not sum > 0 although an A aggressor ran')
     if previous_seq is not None and window[4] <= previous_seq:
         problems.append('helper window[4] sequence ' + repr(window[4])
                         + ' did not increase past ' + repr(previous_seq))
```

### 4.3 Call site (`_run_helper_case`, lines 1293-1298)

```diff
@@ -1293,9 +1353,12 @@
         if window is None:
             problems.append('helper window is missing')
         else:
+            expect_v, expect_a = helper_mechanism_expectations(record)
             window_problems = validate_helper_window(
-                window, self.args.label, expected_cycles, self.last_helper_seq)
+                window, self.args.label, expected_cycles, self.last_helper_seq,
+                expect_v_movement=expect_v, expect_a=expect_a)
             problems.extend(window_problems)
             if not window_problems:
                 self.last_helper_seq = window[4]
```

### 4.4 Raw failed counters are marked unqualified, never deleted

```diff
@@ -1301,6 +1364,10 @@
                 'note': 'full frozen 16-word window saved verbatim; not HPM',
             })
+            # Raw failed counters remain on disk in helper-window.json; they are
+            # only marked unqualified through the problems/status path above.
+            # No sample is re-run, re-scored or deleted.
         return problems, window
```

`--case-timeout`, the framed UART interface (`run_shell`, `read_line`,
`drain_input`), `build_case_plan()` and `build_case_argv()` are **unchanged**.

---

## 5. Focused unified patch — `scripts/analyze_nonblock_pilot.py`

### 5.1 `validate_helper_words` — add the guard parameters and checks

```diff
--- a/scripts/analyze_nonblock_pilot.py
+++ b/scripts/analyze_nonblock_pilot.py
@@ -323,8 +323,10 @@
-def validate_helper_words(label: str, words: Any,
-                          expected_cycles: Optional[int]) -> List[str]:
+def validate_helper_words(label: str, words: Any,
+                          expected_cycles: Optional[int],
+                          expect_v_movement: bool = True,
+                          expect_a: bool = False) -> List[str]:
     problems: List[str] = []
@@ -356,6 +358,29 @@
     fast = sum(words[i] for i in HELPER_FAST_SLICE)
     normal = sum(words[i] for i in HELPER_NORMAL_SLICE)
     if words[HELPER_ACCEPT_INDEX] != fast + normal:
         problems.append('helper 计数守恒失败: words[14]=%r != Σfast %r + Σnormal %r'
                         % (words[HELPER_ACCEPT_INDEX], fast, normal))
+
+    # --- 机制活跃性 guard（banked label；只对已有 words 追加问题，不改写） ----
+    # V: fast 6=Vread, 7=Vwrite; normal 11=V
+    # A: fast 8=Aread, 9=Awrite; normal 12=A
+    if expect_v_movement or expect_a:
+        if words[HELPER_ACCEPT_INDEX] <= 0:
+            problems.append('helper words[14] accepted 不是 > 0，但该用例跑了 '
+                            'V/A 传输（全 0 机制窗口不得判为通过）')
+    if expect_v_movement:
+        v_counts = (words[6], words[7], words[11])
+        if sum(v_counts) <= 0:
+            problems.append('helper V fast+normal 计数 %r 之和不是 > 0，但该用例'
+                            '跑了 V 传输' % (v_counts,))
+    if expect_a:
+        a_counts = (words[8], words[9], words[12])
+        if sum(a_counts) <= 0:
+            problems.append('helper A fast+normal 计数 %r 之和不是 > 0，但该用例'
+                            '跑了 A aggressor' % (a_counts,))
 
     if expected_mode == 1:
         fast_words = [words[i] for i in HELPER_FAST_SLICE]
```

### 5.2 Call site (`analyze_case`, lines 756-757) — pass `case.has_a` and the transfer count

```diff
@@ -753,8 +778,12 @@
                 if helper_entry['normalcounts'] is not None and \
                         helper_entry['normalcounts'] != [w[i] for i in HELPER_NORMAL_SLICE]:
                     helper_entry['problems'].append(
                         'normalcounts 与 words[11..13] 不一致')
+                expect_v = int_or_none(spec.get('transfers')) not in (None, 0)
+                expect_a = bool(entry.get('p4m_has_a'))
                 helper_entry['problems'].extend(validate_helper_words(
-                    label, w, entry['v_window_cycles']))
+                    label, w, entry['v_window_cycles'],
+                    expect_v_movement=expect_v, expect_a=expect_a))
+                helper_entry['mechanism_expect_v'] = expect_v
+                helper_entry['mechanism_expect_a'] = expect_a
```

`entry['p4m_has_a']` is already populated from `record.p4m.has_a` at
`analyze_nonblock_pilot.py:538`, so the guard has an explicit
`case.has_a` input and never guesses.

### 5.3 Why this makes the failed counters *unqualified* without deleting them

The existing pipeline already turns every `helper_entry['problems']` into
`an.q(label, case_id, 'helper_window', problem)`
(`analyze_nonblock_pilot.py:778-779`), and `finalize_label_qualification`
merges those into each case's `qualified_reasons` / `qualified`
(`:834-860`). So a helper window that violates the new liveness guard
automatically becomes `qualified: false`, which in turn blocks
`compute_effects` from publishing (`:1234-1237`). The raw words stay untouched
in the analysis JSON (`cases[*].helper.words`) and in every `helper.json` /
`helper-window.json`. No file is rewritten, no run is re-executed.

Conservation, `words[5] == raw_window[1]`, the `(open, close]` semantics, the
`words[4]` sequence ordering and the Mode-1 fast-must-be-zero check are all
**unchanged**.

---

## 6. Navigable summary

| Deliverable | File | Status |
|---|---|---|
| Coupled read→write test | `tests/nonblock_coupled_test.cpp` | complete, new |
| Runner | `tests/test_nonblock_coupled.py` | complete, new; reuses sibling `SOURCES`/`INCDIRS` |
| Harness observation ports | `tests/nonblock_banked_top.sv` | focused diff, test-only |
| Runner liveness guard | `scripts/mac_nonblock_pilot.py` | focused diff |
| Analyzer liveness guard + `case.has_a` input | `scripts/analyze_nonblock_pilot.py` | focused diff |

Coupled test coverage:

- runtime **Mode1** (`nonblock_enable_i=0`) and **Mode2** (`=1`);
- **Solo** (V only), **A1** (8× fewer aggressor pages: 1 src + 1 dst), **A8**
  (8 src + 8 dst = 16 alternating pages);
- 16 warm rounds + 64 jobs per scenario;
- V pinned to bank0/`V_ID=0`, A pinned to bank1/`A_ID=4`;
- each 4 KiB transfer issued as **two 256-beat bursts**;
- **W comes only from received R beats** (the coupling FIFO, depth 3);
- completion + DDTW + PTW share **one ordered R-return queue**, the DUT's
  R-ready being gated by actual FIFO capacity;
- deterministic issue order: **V source AR first; A's destination AW has
  priority over V's**; W bursts are attributed in upstream-AW order, which
  matches the banked demux W order;
- PTW latency is varied **(1 / 31)** across the 12 sub-scenarios;
- AXI **stable-valid** of `tr_r` is asserted every cycle, and a bounded stall
  dumps last AR/AW/W/R plus the TL observation ports and returns **non-zero**
  (not a silent abort).

---

## 7. Assumptions, limitations, unresolved issues

1. **Test model ≠ hardware.** The C++ model is not DDR and not the real iDMA
   backend. It does not model the XBAR, `rv_iommu_ds_if` arbitration, the board
   `axi_delayer`, or real ID remap.
2. **Memory HOL and the W burst lock.** The request explicitly asks for a
   head-of-line R-return queue and per-DMA 3-beat coupling. In this harness a
   combination of strict HOL, a 3-beat coupling FIFO and an AXI-correct
   non-interleaving W burst can in principle form a loop
   (DMA-A cfifo full → its completion read blocks the HOL head → DMA-B's read
   (needed to refill its stalled W burst) is behind it). The test **detects
   this as a bounded stall and returns non-zero with a dump** rather than
   hanging; it never silently passes. I did not weaken HOL. If Codex sees the
   dump, that loop is the first thing to inspect — it is a harness-level
   property, not an `rtl/**` finding.
3. **Harness cfg is not changed.** `tests/nonblock_banked_top.sv:319-320` keeps
   `NumOutstandingTrans=4`, `WFifoDepth=8` (per bank 2/4) instead of the board's
   32 (per bank 16). Rationale in §0. Changing it is a one-line, revertible
   decision but would perturb the already-passing
   `tests/nonblock_banked_test.cpp` config.
4. **`rv_iommu_nonblock_hit.sv` is not in the supplied evidence.** The Mode-1
   assumption "`nonblock_enable_i=0 ⇒ n_intercept/n_accept/n_busy=0`" is used
   only via the existing `case_nonblock_disabled_blocks`. The coupled test does
   not depend on it.
5. **The debug-port diff is required** for `tests/nonblock_coupled_test.cpp` to
   compile. If Codex prefers not to touch the harness, the TL dump lines in
   `dump_stall` are the only ones that reference `top->dbg_tl_*`; they can be
   detached with a small local edit.
6. **`A AW ahead of V AW` is a priority, not a guarantee.** AW presentation is
   gated on real coupling data, so A's AW can only be accepted once A's source
   R data is in A's coupling FIFO. The priority only decides ties.
7. **The runner/analyzer guards only add problems.** They never re-run, delete
   or rewrite a sample. The framed UART interface, `build_case_plan()` and
   `build_case_argv()` are untouched.

---

## 8. Suggested acceptance checks (Codex executes; I claim none of these as run)

1. `python3 tests/test_nonblock_banked.py` → still prints `nonblock_banked_test: PASS`
   after the harness debug-port diff.
2. `python3 tests/test_nonblock_coupled.py` → prints 12 `COUPLED-SUMMARY` lines
   and ends with `nonblock_coupled_test: PASS`; exit code 0.
3. Self-checks that *must* fail: set `SCENARIO_BUDGET = 1000` → a
   `nonblock_coupled_test: STALL …` dump (last AR/AW/W/R + TL line + queue
   depths) and exit 1; comment out the `page_word` comparison for one page →
   exit 1 on "…destination bytes do not match…".
4. On a helper window with `window[14] == 0` (the supplied frame at
   `out/n-prototype-20261004`): the patched
   `mac_nonblock_pilot.validate_helper_window` must return ≥1 problem, while
   the pre-patch function returned 0 — verifiable offline with one `python3 -c`
   call, no hardware.
5. Re-run `scripts/analyze_nonblock_pilot.py` on the existing three runner
   output directories and confirm: the JSON is written, the affected cases get
   `qualified: false` with a `helper_window` reason, and **no** existing
   `helper.json` / `helper-window.json` / `record.json` is modified.