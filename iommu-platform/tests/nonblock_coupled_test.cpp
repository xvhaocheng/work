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
// Every W beat a DMA puts on the wire is popped from a small FIFO that is
// filled ONLY by source R beats that actually came back through the DUT.
// The completion port, the DDTW port and the PTW port are served by ONE
// shared memory through a single ordered (head-of-line) R-return queue.
//
// Model correspondence (this revision)
// ------------------------------------
//   * board RAWCouplingAvail = 0: AW is asserted INDEPENDENTLY, immediately on
//     job/half creation, with NO dependency on cfifo occupancy and NO
//     dependency on the read response.  AR and AW are therefore concurrently
//     legal.  AR priority is V-first; AW priority is A-first (A ahead of V),
//     deliberately forcing the adversarial order.
//   * the memory model keeps comp_r_valid / ddtw_r_valid / ptw_r_valid STABLE
//     for the head beat regardless of downstream FIFO room.  Upstream R
//     acceptance (tr_r_ready) is throttled by the bank of the RETURNED tr_r_id
//     and the coupling-FIFO room (Tb::couple_depth, 3 by default), evaluated after settling clk-low.
//   * completion B is accepted exactly on sampled comp_b_valid && comp_b_ready
//     and popped from cb_q.
//   * all handshake payloads are snapshotted BEFORE the rising edge.
//   * a 4096-cycle last-progress watchdog dumps detailed state on a stall.
//
// Overlap mode (opt-in, --overlap)
// --------------------------------
// A second stimulus mode relaxes the strict serialization of the two 2 KiB
// halves of one 4 KiB copy: the two ARs and the two AWs of one job may be in
// flight concurrently on the upstream translation-request port.  This is the
// exact R->W decoupling the real iDMA backend exposes when NumAxInFlight > 1
// and RAWCouplingAvail = 0 (see cva6/corev_apu/iDMA/src/idma_backend.sv:472-473,
// :554-557, :590-608, :612-625, :837-858).  The dependencies kept by the
// harness in this mode are:
//   * W data only from R beats that THIS same DMA actually handshook;
//   * the same per-DMA R->W coupling FIFO (Tb::couple_depth) shared by both halves;
//   * W beats strictly matched to the currently-issued AW;
//   * both AWs of a job emitted in AW-issue order;
//   * both B responses before the job is reported done / the next job issued;
//   * the ordered, fully shared memory R-return queue (completion + DDTW +
//     PTW) is unchanged; PTW is NOT priority-hoisted on a stalled data head.
// The default mode is byte-for-byte identical to the pre-existing serial run,
// so the existing PASS logs stay valid.
//
// Coupling-FIFO depth (opt-in, +couple-depth=3|512)
// ------------------------------------------------
// The per-DMA read->write coupling FIFO models the board RAWCouplingAvail = 0
// buffer.  Depth 3 is the historical v1 default and is what every existing log
// in this repository was produced with, so it is never changed implicitly.
// Depth 512 is the v2 "full 4 KiB job" configuration: both 2 KiB halves of one
// 4 KiB copy (2 x 256 R beats) fit in the lane buffer, so a returned R beat is
// not held off just because the other half still has W beats to drain.  The
// depth is chosen solely by this C++ flag, forwarded by
// tests/test_nonblock_coupled.py --couple-depth; the RTL, the Verilator source
// list and the obj directory are unchanged.  The SAME value feeds both the
// upstream tr_r_ready back-pressure (settle_upstream_r_ready) and the
// coupling-FIFO overflow CHECK (the f_up_r update), so a run can never
// back-pressure at one depth and check another.
//
// This is a test harness model.  It is NOT DDR and NOT the real iDMA backend:
// it has no XBAR, no rv_iommu_ds_if arbitration, no board axi_delayer and no
// ID remap.  It only reproduces the ordering/back-pressure shape of the board.
//
// Exit status: 0 on PASS, non-zero on FAIL or bounded STALL.

#include "Vnonblock_banked_top.h"
#include "verilated.h"

double sc_time_stamp() { return 0; }

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// identifiers / addresses / sizes
// ---------------------------------------------------------------------------
static const uint32_t V_ID      = 0;   // bank 0
static const uint32_t A_ID      = 4;   // bank 1
static const uint32_t V_DID     = 10;
static const uint32_t A_DID     = 11;
static const uint32_t BANK_BIT  = 2;   // BankIdBit == 2  (harness)

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
// The per-DMA read->write coupling-FIFO depth is no longer a file constant:
// it is Tb::couple_depth (default 3), set from +couple-depth=3|512 in main().
static const uint32_t WARM_ROUNDS  = 16;   // not scaled with the job count

// Job count per DMA.  64 is the historical default (every existing log); 4096
// is the bounded long continuous-work run.  Set from +job-count=64|4096.
static const uint32_t JOB_COUNT_DEFAULT = 64;

// Last-progress watchdog: 4096 consecutive cycles with no upstream / completion
// handshake is treated as a real stall.
static const long WATCHDOG_CYCLES = 4096L;
// Largest accepted +v-gap-cycles, strictly inside the watchdog window so a
// planned V inter-job gap is never mistaken for a stall.
static const uint32_t V_GAP_MAX = 4000;
static_assert(V_GAP_MAX < (uint32_t)WATCHDOG_CYCLES,
              "planned V gap must stay inside the watchdog window");

// Ultimate backstop only; the watchdog normally fires long before this.  Now
// DERIVED from the job count and the V gap: keep the historical per-job
// allowance (4_000_000 / 64 == 62_500 cycles per job), add the total planned V
// gap, and add a fixed warm-up + drain margin.  Finite for both job counts.
static const long SCENARIO_BUDGET_PER_JOB = 62500L;
static const long SCENARIO_BUDGET_WARM    = 400000L;
static const long WARM_BUDGET             = 200000L;

// Per-scenario backstop derived from the two run knobs (upper bound only; the
// per-job watchdog is what detects a real stall).
static long scenario_budget(uint32_t job_count, uint32_t v_gap_cycles) {
    return SCENARIO_BUDGET_PER_JOB * (long)job_count
         + (long)v_gap_cycles * (long)job_count
         + SCENARIO_BUDGET_WARM;
}

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

struct TestFailure : std::runtime_error {
    explicit TestFailure(const std::string& m) : std::runtime_error(m) {}
};
static void fail(const char* msg, const char* file, int line) {
    char buf[512];
    std::snprintf(buf, sizeof(buf), "%s (%s:%d)", msg, file, line);
    throw TestFailure(buf);
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
    // Expected completion AW.  The DUT translates the upstream write IOVA to a
    // physical destination, so the upstream wire address (an IOVA) can never be
    // compared against the completion wire address (a PA).  Keep both:
    //   addr = upstream IOVA as observed (trace / back-reference only)
    //   pa   = expected translated physical destination (the real match key)
    struct ExpAw { uint32_t id = 0; uint64_t addr = 0; uint64_t pa = 0; uint8_t len = 0; };
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

        // Per-half state.  Index 0/1 = half 0 / half 1 of the job.
        // Serial mode touches only index `half`; overlap mode touches both.
        bool     ar_want[2] = {false, false};
        bool     ar_done[2] = {false, false};
        bool     aw_want[2] = {false, false};
        bool     aw_done[2] = {false, false};
        uint32_t rx     [2] = {0, 0};   // source R beats this half
        bool     r_last [2] = {false, false};
        uint32_t wx     [2] = {0, 0};   // W beats emitted this half
        bool     b_done [2] = {false, false};

        std::deque<Beat> cfifo;     // read->write coupling FIFO (<= Tb::couple_depth)
                                    // shared across both halves of one DMA
    };
    Dma dma[2];                     // [0]=V (bank0), [1]=A (bank1)

    std::deque<int> aw_order;       // accepted-AW order == DUT demux W order
    int w_owner = -1;

    int cur_ar_d = -1, cur_aw_d = -1;

    // locked VALID ownership (held until handshake)
    int ar_lock = -1;               // -1 none, 0/1 dma, 2 warm
    int aw_lock = -1;               // -1 none, 0/1 dma

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
    // Per-DMA read->write coupling-FIFO depth.  3 = the v1 default and the
    // value every existing log was produced with; 512 = the v2 full 4 KiB job
    // buffered per lane.  Set from +couple-depth=3|512 in main().
    uint32_t couple_depth = 3;
    long g_cycle         = 0;
    long scenario_start  = 0;
    uint32_t ptw_latency = 1;
    uint32_t a_pages_    = 0;
    // Job count per DMA (V and A each issue this many 4 KiB jobs).  64 keeps the
    // historical 12-row run; 4096 is the bounded long run.  Set from
    // +job-count=64|4096 in main(); every V/A job loop uses this value.
    uint32_t job_count = JOB_COUNT_DEFAULT;

    // Upstream request-priority order for the two DMA clients (see +order= in
    // main()): 0 = V (dma[0]) first, 1 = A (dma[1]) first.
    // Defaults select the matching-source order (both V-first).
    int order_ar_prefer = 0;
    int order_aw_prefer = 0;

    // +order=rr: independent AR/AW round-robin cursors.  They advance only on
    // a real DMA AR/AW handshake (cur_*_d >= 0); a warm-up AR (owner 2) must
    // never perturb them.
    bool rr_order = false;
    int  rr_ar_next = 0;            // next preferred DMA for AR (0=V, 1=A)
    int  rr_aw_next = 0;            // next preferred DMA for AW (0=V, 1=A)

    // Overlap mode: honor both halves of one 4 KiB job concurrently.
    bool overlap_mode = false;

    // v5 opt-in proxy: drive fullbuffer_jobs_i = 3u for the bounded 4 KiB
    // candidate.  Default false -> 0u (legacy guard).  With +couple-depth=512
    // the harness runs all 12 existing scenarios; each scenario has one 4 KiB
    // job per DMA, and the A-enabled scenarios therefore carry two jobs on the
    // two separate 512-deep coupling FIFOs.
    bool fullbuffer_permit = false;

    // +v-gap-cycles=0|1000|4000: number of complete ticks the V victim waits
    // after finishing one 4 KiB job before the next V job is started.  0 is the
    // historical back-to-back issue.  A is never gated by this value: A's own
    // start condition is unchanged and tick() keeps running during the wait.
    // The wait is purely temporal; no IOVA/PA or page distance is changed.
    uint32_t v_gap_cycles = 0;

    // --continuous-aggressor: opt-in.  When set (and the scenario has an A
    // aggressor, i.e. a_pages > 0) the A DMA launches each next 4 KiB job as
    // soon as its previous job completes, cycling the existing a_pages
    // source/destination pages, and keeps doing so until the V victim has
    // actually completed its FINAL job (v_next == job_count &&
    // dma[0].job_done).  At that point no new A job is launched and the
    // already-active A job is drained.  Default false -> byte-for-byte
    // identical to the historical fixed job_count run.
    bool continuous_aggressor = false;

    // +policy=0|1: forwarded verbatim to the wrapper input policy_partition_i.
    // Only observable in a build elaborated with PolicyEnable=1 (the Python
    // --policy build); the default build forces the full replacement mask.
    bool policy_partition = false;

    // Per-DMA selectors for the half whose AR / AW is currently on the wire,
    // and per-DMA pointers to the half of the next returning R / B / W beat.
    // They are all no-ops in serial mode (they stay 0 or 1).
    int ar_half_sel     = -1;
    int aw_half_sel     = -1;
    int r_half_cur[2]   = {0, 0};
    int b_half_cur[2]   = {0, 0};
    int w_half_cur[2]   = {0, 0};

    // liveness observation
    long last_progress_cycle = 0;

    // stability observation
    bool     last_r_valid = false;
    uint64_t last_r_data  = 0;
    uint32_t last_r_id    = 0;
    bool     last_r_last  = false;

    uint64_t n_comp_ar = 0, n_ddtw_ar = 0, n_ptw_ar = 0;
    uint64_t n_tl_accept = 0, n_tl_resp = 0;
    uint64_t n_ptw_active = 0, n_iotlb_lookup = 0, n_iotlb_hit = 0;
    uint64_t n_r_beats = 0, n_b_up = 0;

    // Observed nonblock activity.  Counted only from the outputs the harness
    // already exposes (nonblock_banked_top.sv:150-151 nonblock_fire_o /
    // nonblock_busy_o); the top exposes no candidate/probe counter.
    uint64_t n_nonblock_fire = 0, n_nonblock_busy = 0;

    // --continuous-aggressor accounting.  a_issued counts every 4 KiB A job
    // launched; a_completed counts every A job observed finishing (A jobs are
    // serial, so this is an edge count).  Both stay at the plain job_count for
    // the default non-continuous runs.
    uint64_t a_issued = 0, a_completed = 0;
    bool     a_done_prev = false;

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
        top->tr_r_ready  = 1;          // optimistic; corrected by settle_upstream_r_ready()
        cur_ar_d = -1;
        cur_aw_d = -1;

        // ---- AR : priority order selectable (see order_ar_prefer), then the
        //          warm-up probe ---------------------------------------------
        // Once the choice is made it is LOCKED until the handshake, so a second
        // DMA becoming eligible cannot flip VALID ownership under a stalled beat.
        if (ar_lock < 0) {
            int ar_d = -1, ar_h = -1;
            // rr_order starts at rr_ar_next; fixed modes start at
            // order_ar_prefer.  The scan is always per-DMA ascending half.
            for (int k = 0; k < 2 && ar_d < 0; ++k) {
                int d = rr_order ? ((rr_ar_next + k) & 1)
                                 : ((order_ar_prefer + k) & 1);
                if (!dma[d].used || dma[d].job_done) continue;
                for (int h = 0; h < 2; ++h) {
                    if (dma[d].ar_want[h] && !dma[d].ar_done[h]) {
                        ar_d = d; ar_h = h; break;
                    }
                }
            }
            if (ar_d < 0 && warm_active && !warm_sent) { ar_d = 2; ar_h = 0; }
            ar_lock     = ar_d;
            ar_half_sel = ar_h;
        }
        if (ar_lock == 2) {
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
        } else if (ar_lock >= 0) {
            Dma& m = dma[ar_lock];
            top->tr_ar_valid = 1;
            top->tr_ar_id    = m.id;
            top->tr_ar_addr  = m.src_iova + (uint64_t)ar_half_sel * 0x800;
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
            cur_ar_d = ar_lock;
        }

        // ---- AW : priority order selectable (see order_aw_prefer); asserted
        //           independently of cfifo occupancy and read data ----------
        if (aw_lock < 0) {
            int aw_d = -1, aw_h = -1;
            // rr_order starts at rr_aw_next; fixed modes start at
            // order_aw_prefer.  The scan is always per-DMA ascending half.
            for (int k = 0; k < 2 && aw_d < 0; ++k) {
                int d = rr_order ? ((rr_aw_next + k) & 1)
                                 : ((order_aw_prefer + k) & 1);
                if (!dma[d].used || dma[d].job_done) continue;
                for (int h = 0; h < 2; ++h) {
                    if (dma[d].aw_want[h] && !dma[d].aw_done[h]) {
                        aw_d = d; aw_h = h; break;
                    }
                }
            }
            aw_lock     = aw_d;
            aw_half_sel = aw_h;
        }
        if (aw_lock >= 0) {
            Dma& m = dma[aw_lock];
            top->tr_aw_valid = 1;
            top->tr_aw_id    = m.id;
            top->tr_aw_addr  = m.dst_iova + (uint64_t)aw_half_sel * 0x800;
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
            cur_aw_d = aw_lock;
        }

        // ---- W : only the current burst owner, popped from its cfifo ------
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
    //   The producer keeps *_r_valid STABLE for the head beat regardless of
    //   downstream FIFO room.  R is NOT suppressed on cfifo fullness; that
    //   back-pressure is applied on the upstream tr_r_ready instead.
    // ---------------------------------------------------------------------
    void drive_mem_r() {
        top->comp_r_valid = 0;
        top->ddtw_r_valid = 0;
        top->ptw_r_valid  = 0;

        if (mem_q.empty()) return;
        MemRead& h = mem_q.front();
        if (h.latency > 0) { h.latency--; return; }

        switch (h.port) {
        case P_COMP:
            top->comp_r_valid = 1;
            top->comp_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
            top->comp_r_id    = h.id;
            top->comp_r_resp  = 0;
            top->comp_r_last  = (h.beat == h.len) ? 1 : 0;
            break;
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
    // upstream R acceptance: throttle tr_r_ready from the RETURNED id and
    // the coupling-FIFO room (Tb::couple_depth).  Called after the first clk-low eval;
    // re-evaluates so the combinational chain settles before sampling.
    // ---------------------------------------------------------------------
    void settle_upstream_r_ready() {
        if (top->tr_r_valid) {
            uint32_t bank = (top->tr_r_id >> BANK_BIT) & 1u;
            top->tr_r_ready = (dma[bank].cfifo.size() < couple_depth) ? 1 : 0;
        } else {
            top->tr_r_ready = 1;
        }
        top->eval();
    }

    // ---------------------------------------------------------------------
    // one cycle
    // ---------------------------------------------------------------------
    void tick() {
        top->nonblock_enable_i = nonblock_enable ? 1 : 0;
        top->fullbuffer_jobs_i = fullbuffer_permit ? 3u : 0u;
        top->policy_partition_i = policy_partition ? 1 : 0;

        for (int d = 0; d < 2; ++d) dma_progress(d);

        // ---- drive all combinational inputs at clk low -----------------
        top->clk_i = 0;
        drive_upstream();
        drive_comp();
        drive_mem_r();
        top->ddtw_ar_ready = 1;
        top->ptw_ar_ready  = 1;

        top->eval();                   // settle #1
        settle_upstream_r_ready();     // adjust tr_r_ready + settle #2

        // ---- snapshot ALL handshake payloads before the rising edge -----
        const bool     s_tr_ar_valid = top->tr_ar_valid;
        const bool     s_tr_ar_ready = top->tr_ar_ready;
        const uint32_t s_tr_ar_id    = top->tr_ar_id;
        const uint64_t s_tr_ar_addr  = top->tr_ar_addr;
        const uint32_t s_tr_ar_len   = (uint32_t)top->tr_ar_len;

        const bool     s_tr_aw_valid = top->tr_aw_valid;
        const bool     s_tr_aw_ready = top->tr_aw_ready;
        const uint32_t s_tr_aw_id    = top->tr_aw_id;
        const uint64_t s_tr_aw_addr  = top->tr_aw_addr;
        const uint32_t s_tr_aw_len   = (uint32_t)top->tr_aw_len;

        const bool     s_tr_w_valid  = top->tr_w_valid;
        const bool     s_tr_w_ready  = top->tr_w_ready;

        const bool     s_tr_b_valid  = top->tr_b_valid;
        const bool     s_tr_b_ready  = top->tr_b_ready;
        const uint32_t s_tr_b_id     = top->tr_b_id;
        const uint32_t s_tr_b_resp   = (uint32_t)top->tr_b_resp;

        const bool     s_tr_r_valid  = top->tr_r_valid;
        const bool     s_tr_r_ready  = top->tr_r_ready;
        const uint32_t s_tr_r_id     = top->tr_r_id;
        const uint64_t s_tr_r_data   = top->tr_r_data;
        const bool     s_tr_r_last   = top->tr_r_last;
        const uint32_t s_tr_r_resp   = (uint32_t)top->tr_r_resp;

        const bool     s_comp_ar_valid = top->comp_ar_valid;
        const bool     s_comp_ar_ready = top->comp_ar_ready;
        const uint32_t s_comp_ar_id    = top->comp_ar_id;
        const uint64_t s_comp_ar_addr  = top->comp_ar_addr;
        const uint32_t s_comp_ar_len   = (uint32_t)top->comp_ar_len;

        const bool     s_ddtw_ar_valid = top->ddtw_ar_valid;
        const bool     s_ddtw_ar_ready = top->ddtw_ar_ready;
        const uint64_t s_ddtw_ar_addr  = top->ddtw_ar_addr;
        const uint32_t s_ddtw_ar_len   = (uint32_t)top->ddtw_ar_len;
        const uint32_t s_ddtw_ar_id    = top->ddtw_ar_id;

        const bool     s_ptw_ar_valid = top->ptw_ar_valid;
        const bool     s_ptw_ar_ready = top->ptw_ar_ready;
        const uint64_t s_ptw_ar_addr  = top->ptw_ar_addr;
        const uint32_t s_ptw_ar_len   = (uint32_t)top->ptw_ar_len;
        const uint32_t s_ptw_ar_id    = top->ptw_ar_id;

        const bool     s_comp_aw_valid = top->comp_aw_valid;
        const bool     s_comp_aw_ready = top->comp_aw_ready;
        const uint32_t s_comp_aw_id    = top->comp_aw_id;
        const uint64_t s_comp_aw_addr  = top->comp_aw_addr;
        const uint32_t s_comp_aw_len   = (uint32_t)top->comp_aw_len;

        const bool     s_comp_w_valid = top->comp_w_valid;
        const bool     s_comp_w_ready = top->comp_w_ready;
        const uint64_t s_comp_w_data  = top->comp_w_data;
        const uint8_t  s_comp_w_strb  = (uint8_t)top->comp_w_strb;
        const bool     s_comp_w_last  = top->comp_w_last;

        const bool     s_comp_b_valid = top->comp_b_valid;
        const bool     s_comp_b_ready = top->comp_b_ready;

        const bool     s_comp_r_valid = top->comp_r_valid;
        const bool     s_comp_r_ready = top->comp_r_ready;
        const bool     s_ddtw_r_valid = top->ddtw_r_valid;
        const bool     s_ddtw_r_ready = top->ddtw_r_ready;
        const bool     s_ptw_r_valid  = top->ptw_r_valid;
        const bool     s_ptw_r_ready  = top->ptw_r_ready;

        // ---- fire signals (from settled snapshots) ----------------------
        const bool f_up_ar   = s_tr_ar_valid && s_tr_ar_ready;
        const bool f_up_aw   = s_tr_aw_valid && s_tr_aw_ready;
        const bool f_up_w    = s_tr_w_valid  && s_tr_w_ready;
        const bool f_up_b    = s_tr_b_valid  && s_tr_b_ready;
        const bool f_up_r    = s_tr_r_valid  && s_tr_r_ready;
        const bool f_comp_ar = s_comp_ar_valid && s_comp_ar_ready;
        const bool f_ddtw_ar = s_ddtw_ar_valid && s_ddtw_ar_ready;
        const bool f_ptw_ar  = s_ptw_ar_valid  && s_ptw_ar_ready;
        const bool f_comp_aw = s_comp_aw_valid && s_comp_aw_ready;
        const bool f_comp_w  = s_comp_w_valid  && s_comp_w_ready;
        const bool f_comp_b  = s_comp_b_valid  && s_comp_b_ready;
        const bool f_comp_r  = s_comp_r_valid  && s_comp_r_ready;
        const bool f_ddtw_r  = s_ddtw_r_valid  && s_ddtw_r_ready;
        const bool f_ptw_r   = s_ptw_r_valid   && s_ptw_r_ready;

        if (f_up_ar || f_up_aw || f_up_w || f_up_b || f_up_r ||
            f_comp_ar || f_ddtw_ar || f_ptw_ar || f_comp_aw || f_comp_w ||
            f_comp_b || f_comp_r || f_ddtw_r || f_ptw_r)
            last_progress_cycle = g_cycle;

        // ---- TL observation counters -----------------------------------
        if (top->dbg_tl_req_valid && top->dbg_tl_req_ready) ++n_tl_accept;
        if (top->dbg_tl_resp_valid && top->dbg_tl_resp_ready) ++n_tl_resp;
        if (top->dbg_tl_ptw_active) ++n_ptw_active;
        if (top->dbg_tl_iotlb_lookup) {
            ++n_iotlb_lookup;
            if (top->dbg_tl_iotlb_hit) ++n_iotlb_hit;
        }

        // ---- observed nonblock activity (only the exposed top outputs) --
        if (top->nonblock_fire_o) ++n_nonblock_fire;
        if (top->nonblock_busy_o) ++n_nonblock_busy;

        // ---- R stability (AXI: valid beat must not change until handshake)
        if (last_r_valid) {
            CHECK(s_tr_r_valid, "R valid dropped before handshake");
            CHECK(s_tr_r_data == last_r_data && s_tr_r_id == last_r_id &&
                  s_tr_r_last == last_r_last, "R beat changed while valid");
        }
        last_r_valid = s_tr_r_valid && !f_up_r;
        last_r_data  = s_tr_r_data;
        last_r_id    = s_tr_r_id;
        last_r_last  = s_tr_r_last;

        // ---- rising edge -----------------------------------------------
        top->clk_i = 1;
        top->eval();

        // ---- state updates from snapshots (NOT post-edge outputs) -------
        if (f_up_ar) {
            up_ar.push_back({g_cycle, s_tr_ar_id, s_tr_ar_addr, s_tr_ar_len});
            if (cur_ar_d >= 0) {
                Dma& m = dma[cur_ar_d];
                m.ar_want[ar_half_sel] = false;
                m.ar_done[ar_half_sel] = true;
                // Advance AR RR only for a real DMA beat.  A warm-up AR has
                // cur_ar_d == -1 and must leave both RR cursors untouched.
                if (rr_order) rr_ar_next = 1 - cur_ar_d;
            } else if (ar_lock == 2) {
                warm_sent = true;
            }
            ar_lock = -1;
        }
        if (f_up_aw) {
            up_aw.push_back({g_cycle, s_tr_aw_id, s_tr_aw_addr, s_tr_aw_len});
            ExpAw e;
            e.id   = s_tr_aw_id;
            e.addr = s_tr_aw_addr;              // retain the upstream IOVA for trace
            e.len  = (uint8_t)s_tr_aw_len;
            if (cur_aw_d >= 0) {
                // Completion AW will carry the translated physical destination,
                // not the IOVA.  Snapshot it from the issuing DMA's current half.
                Dma& m = dma[cur_aw_d];
                e.pa = m.dst_pa + (uint64_t)aw_half_sel * 0x800;
                m.aw_want[aw_half_sel] = false;
                m.aw_done[aw_half_sel] = true;
                aw_order.push_back(cur_aw_d);
                if (w_owner < 0) w_owner = aw_order.front();
                if (rr_order) rr_aw_next = 1 - cur_aw_d;
            } else {
                // No local model (a DMA AW always has cur_aw_d >= 0); fall back
                // to the wire address so the expected queue stays well-formed.
                e.pa = s_tr_aw_addr;
            }
            exp_aw_q.push_back(e);
            aw_lock = -1;
        }
        if (f_up_w) {
            CHECK(w_owner >= 0 && !dma[w_owner].cfifo.empty(), "W fire without owner data");
            Dma& m = dma[w_owner];
            Beat b = m.cfifo.front();
            m.cfifo.pop_front();
            int h = w_half_cur[w_owner];
            CHECK(h < 2, "W half pointer overflow");
            m.wx[h]++;
            up_w.push_back({g_cycle, m.id, b.data, b.strb, b.last});
            if (b.last) {
                CHECK(m.wx[h] == BURST_BEATS, "W burst length != BURST_BEATS");
                CHECK(!aw_order.empty() && aw_order.front() == w_owner,
                      "W owner is not at the AW head");
                aw_order.pop_front();
                w_half_cur[w_owner] = h + 1;
                w_owner = aw_order.empty() ? -1 : aw_order.front();
            }
        }
        if (f_up_b) {
            uint32_t bank = (s_tr_b_id >> BANK_BIT) & 1u;
            ++n_b_up;
            int h = b_half_cur[bank];
            CHECK(h < 2, "B half pointer overflow");
            dma[bank].b_done[h] = true;
            b_half_cur[bank] = h + 1;
            up_b.push_back({g_cycle, s_tr_b_id, s_tr_b_resp});
        }
        if (f_up_r) {
            CHECK(s_tr_r_resp == 0, "upstream R resp not OKAY");
            uint32_t bank = (s_tr_r_id >> BANK_BIT) & 1u;
            Dma& m = dma[bank];
            if (m.used && !m.job_done) {
                Beat b; b.data = s_tr_r_data; b.strb = 0xFF;
                b.last = s_tr_r_last; b.id = s_tr_r_id;
                m.cfifo.push_back(b);
                CHECK(m.cfifo.size() <= couple_depth, "coupling FIFO overflowed");
                int h = r_half_cur[bank];
                CHECK(h < 2, "R half pointer overflow");
                m.rx[h]++;
                if (b.last) { m.r_last[h] = true; r_half_cur[bank] = h + 1; }
            }
            up_r.push_back({g_cycle, s_tr_r_id, s_tr_r_data,
                            (bool)s_tr_r_last, s_tr_r_resp});
            ++n_r_beats;
        }

        if (f_comp_ar) {
            cmp_ar.push_back({g_cycle, s_comp_ar_id, s_comp_ar_addr, s_comp_ar_len});
            ++n_comp_ar;
            MemRead r; r.port = P_COMP; r.addr = s_comp_ar_addr;
            r.len = (uint8_t)s_comp_ar_len; r.id = (uint8_t)s_comp_ar_id;
            r.beat = 0; r.latency = 1;
            mem_q.push_back(r);
        }
        if (f_ddtw_ar) {
            ++n_ddtw_ar;
            MemRead r; r.port = P_DDTW; r.addr = s_ddtw_ar_addr;
            r.len = (uint8_t)s_ddtw_ar_len; r.id = (uint8_t)s_ddtw_ar_id;
            r.beat = 0; r.latency = 1;
            mem_q.push_back(r);
        }
        if (f_ptw_ar) {
            ++n_ptw_ar;
            MemRead r; r.port = P_PTW; r.addr = s_ptw_ar_addr;
            r.len = (uint8_t)s_ptw_ar_len; r.id = (uint8_t)s_ptw_ar_id;
            r.beat = 0; r.latency = (int)ptw_latency;
            mem_q.push_back(r);
        }
        if (f_comp_aw) {
            cmp_aw.push_back({g_cycle, s_comp_aw_id, s_comp_aw_addr, s_comp_aw_len});
            CHECK(!exp_aw_q.empty(), "completion AW without a matching upstream AW");
            ExpAw e = exp_aw_q.front();
            // Match on the expected translated PHYSICAL destination (e.pa); the
            // upstream IOVA (e.addr) is retained for trace only and is NOT what
            // the completion master puts on the wire.
            CHECK(e.id == s_comp_aw_id && e.pa == s_comp_aw_addr &&
                  e.len == (uint8_t)s_comp_aw_len, "completion AW does not match upstream AW");
            exp_aw_q.pop_front();
            CawEnt c; c.id = s_comp_aw_id; c.addr = s_comp_aw_addr;
            c.len = (uint8_t)s_comp_aw_len; c.got = 0;
            caw_q.push_back(c);
        }
        if (f_comp_w) {
            cmp_w.push_back({g_cycle, 0, s_comp_w_data, s_comp_w_strb, s_comp_w_last});
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            CawEnt& c = caw_q.front();
            CHECK(c.got < (uint32_t)c.len + 1, "too many completion W beats");
            write_beat(c.addr + (uint64_t)c.got * 8, s_comp_w_data, s_comp_w_strb);
            c.got++;
            if (s_comp_w_last) {
                CHECK(c.got == (uint32_t)c.len + 1, "completion W burst length mismatch");
                cb_q.push_back(c.id);
                caw_q.pop_front();
            }
        }
        if (f_comp_b) {
            CHECK(!cb_q.empty(), "completion B handshake with empty cb_q");
            cb_q.pop_front();
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
    //   board RAWCouplingAvail = 0: AW is requested at the same time as AR,
    //   as soon as the half starts.  It is NOT gated on source data and NOT
    //   on the read response.
    // ---------------------------------------------------------------------
    void dma_progress(int d) {
        Dma& m = dma[d];
        if (!m.used || m.job_done) return;

        if (overlap_mode) {
            // Both halves of the job are eligible at once.  AR/AW are issued
            // in ascending half order; R/B/W are matched to the half pointer.
            for (int h = 0; h < 2; ++h) {
                if (!m.ar_want[h] && !m.ar_done[h]) m.ar_want[h] = true;
                if (!m.aw_want[h] && !m.aw_done[h]) m.aw_want[h] = true;
            }
            const bool h0_ok = m.ar_done[0] && m.r_last[0] && m.rx[0] == BURST_BEATS &&
                               m.aw_done[0] && m.wx[0] == BURST_BEATS && m.b_done[0];
            const bool h1_ok = m.ar_done[1] && m.r_last[1] && m.rx[1] == BURST_BEATS &&
                               m.aw_done[1] && m.wx[1] == BURST_BEATS && m.b_done[1];
            if (h0_ok && h1_ok) { m.job_done = true; return; }
        } else {
            int h = m.half;
            if (h >= 2) { m.job_done = true; return; }
            if (m.ar_done[h] && m.r_last[h] && m.rx[h] == BURST_BEATS &&
                m.aw_done[h] && m.wx[h] == BURST_BEATS && m.b_done[h]) {
                m.half = h + 1;
                if (m.half >= 2) { m.job_done = true; return; }
                h = m.half;
            }
            if (!m.ar_want[h] && !m.ar_done[h]) m.ar_want[h] = true;
            if (!m.aw_want[h] && !m.aw_done[h]) m.aw_want[h] = true;
        }
    }

    void start_job(int d, uint32_t idx) {
        Dma& m = dma[d];
        CHECK(m.cfifo.empty(), "start_job with a non-empty coupling FIFO");
        m.used = true;
        m.half = 0;
        m.job_done = false;
        for (int h = 0; h < 2; ++h) {
            m.ar_want[h] = false; m.ar_done[h] = false;
            m.aw_want[h] = false; m.aw_done[h] = false;
            m.rx[h]      = 0;     m.r_last[h]  = false;
            m.wx[h]      = 0;     m.b_done[h]  = false;
        }
        r_half_cur[d] = 0;
        b_half_cur[d] = 0;
        w_half_cur[d] = 0;
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
        last_progress_cycle = g_cycle;
        for (int i = 0; i < WARM_BUDGET; ++i) {
            tick();
            if (up_r.size() > base_r) { warm_active = false; return; }
            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) break;
            if (i == WARM_BUDGET - 1) break;
        }
        warm_active = false;
        dump_stall("warm");
        throw TestFailure("warm-up read did not return");
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
            "nonblock_coupled_test: STALL at %s cycle=%ld scenario_cycles=%ld "
            "since_progress=%ld\n",
            where.c_str(), g_cycle, g_cycle - scenario_start,
            g_cycle - last_progress_cycle);

        // shared ordered R-return queue head
        if (!mem_q.empty()) {
            const MemRead& h = mem_q.front();
            std::fprintf(stderr,
                "  sharedR.head port=%d id=%u addr=0x%016lx len=%u beat=%u latency=%d\n",
                (int)h.port, (unsigned)h.id, (unsigned long)h.addr,
                (unsigned)h.len, h.beat, h.latency);
        } else {
            std::fprintf(stderr, "  sharedR.head (empty)\n");
        }

        // per-DMA state
        for (int d = 0; d < 2; ++d) {
            const Dma& m = dma[d];
            std::fprintf(stderr,
                "  DMA%d used=%d done=%d half=%d "
                "ar_want[0/1]=%d/%d ar_done[0/1]=%d/%d "
                "aw_want[0/1]=%d/%d aw_done[0/1]=%d/%d "
                "rx[0/1]=%u/%u wx[0/1]=%u/%u "
                "r_last[0/1]=%d/%d b_done[0/1]=%d/%d cfifo=%zu "
                "half_cur R/B/W=%d/%d/%d\n",
                d, (int)m.used, (int)m.job_done, m.half,
                (int)m.ar_want[0], (int)m.ar_want[1],
                (int)m.ar_done[0], (int)m.ar_done[1],
                (int)m.aw_want[0], (int)m.aw_want[1],
                (int)m.aw_done[0], (int)m.aw_done[1],
                m.rx[0], m.rx[1], m.wx[0], m.wx[1],
                (int)m.r_last[0], (int)m.r_last[1],
                (int)m.b_done[0], (int)m.b_done[1],
                m.cfifo.size(),
                r_half_cur[d], b_half_cur[d], w_half_cur[d]);
        }

        // pending completion AWs
        if (!caw_q.empty()) {
            std::fprintf(stderr, "  caw_q.head id=%u addr=0x%016lx len=%u got=%u\n",
                         (unsigned)caw_q.front().id, (unsigned long)caw_q.front().addr,
                         (unsigned)caw_q.front().len, caw_q.front().got);
        } else {
            std::fprintf(stderr, "  caw_q.head (empty)\n");
        }
        if (!exp_aw_q.empty()) {
            std::fprintf(stderr, "  exp_aw_q.head id=%u iova=0x%016lx pa=0x%016lx len=%u\n",
                         (unsigned)exp_aw_q.front().id,
                         (unsigned long)exp_aw_q.front().addr,
                         (unsigned long)exp_aw_q.front().pa,
                         (unsigned)exp_aw_q.front().len);
        } else {
            std::fprintf(stderr, "  exp_aw_q.head (empty)\n");
        }

        // last observed upstream traffic
        std::fprintf(stderr,
            "  last up_ar=%s up_aw=%s up_w=%s up_r=%s up_b=%s\n",
            up_ar.empty() ? "(none)" : "yes", up_aw.empty() ? "(none)" : "yes",
            up_w.empty()  ? "(none)" : "yes", up_r.empty()  ? "(none)" : "yes",
            up_b.empty()  ? "(none)" : "yes");
        if (!up_ar.empty()) {
            const RecAr& r = up_ar.back();
            std::fprintf(stderr, "  AR id=%u addr=0x%016lx len=%u\n",
                         (unsigned)r.id, (unsigned long)r.addr, (unsigned)r.len);
        }
        if (!up_aw.empty()) {
            const RecAw& r = up_aw.back();
            std::fprintf(stderr, "  AW id=%u addr=0x%016lx len=%u\n",
                         (unsigned)r.id, (unsigned long)r.addr, (unsigned)r.len);
        }
        if (!up_w.empty()) {
            const RecW& r = up_w.back();
            std::fprintf(stderr, "  W  data=0x%016lx strb=0x%02x last=%d\n",
                         (unsigned long)r.data, (unsigned)r.strb, (int)r.last);
        }
        if (!up_r.empty()) {
            const RecR& r = up_r.back();
            std::fprintf(stderr, "  R  data=0x%016lx last=%d resp=%u\n",
                         (unsigned long)r.data, (int)r.last, (unsigned)r.resp);
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
            "in_flight=%d nonblock_busy=%d nonblock_fire=%d\n",
            mem_q.size(), caw_q.size(), cb_q.size(), exp_aw_q.size(),
            aw_order.size(), w_owner,
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
        top->fullbuffer_jobs_i = fullbuffer_permit ? 3u : 0u;

        dma[0] = Dma(); dma[1] = Dma();
        aw_order.clear(); w_owner = -1;
        ar_half_sel = -1; aw_half_sel = -1;
        for (int d = 0; d < 2; ++d) {
            r_half_cur[d] = 0;
            b_half_cur[d] = 0;
            w_half_cur[d] = 0;
        }
        ar_lock = -1; aw_lock = -1; cur_ar_d = -1; cur_aw_d = -1;
        rr_ar_next = 0; rr_aw_next = 0;
        mem_q.clear(); caw_q.clear(); cb_q.clear(); exp_aw_q.clear();
        up_ar.clear(); up_aw.clear(); up_w.clear(); up_r.clear(); up_b.clear();
        cmp_ar.clear(); cmp_aw.clear(); cmp_w.clear();
        warm_active = false; warm_sent = false;
        last_r_valid = false;
        n_comp_ar = n_ddtw_ar = n_ptw_ar = 0;
        n_tl_accept = n_tl_resp = 0;
        n_ptw_active = n_iotlb_lookup = n_iotlb_hit = 0;
        n_r_beats = n_b_up = 0;
        n_nonblock_fire = n_nonblock_busy = 0;
        a_issued = a_completed = 0;
        a_done_prev = false;

        for (int i = 0; i < 8; ++i) tick();
        top->rst_ni = 1;
        for (int i = 0; i < 4; ++i) tick();
        last_progress_cycle = g_cycle;
    }

    void check_quiet(const char* tag) {
        for (int i = 0; i < 40; ++i) {
            tick();
            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                dump_stall(std::string("quiet ") + tag);
                throw TestFailure("watchdog during drain-to-quiet");
            }
        }
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
                    "ptw_latency=%u, jobs=%u, v_gap_cycles=%u, "
                    "policy_partition=%d) ==\n",
                    name, (int)enable, a_pages, ptw_lat, job_count,
                    v_gap_cycles, (int)policy_partition);
        setup_memory(a_pages);
        reset(enable);
        ptw_latency    = ptw_lat;
        scenario_start = g_cycle;
        last_progress_cycle = g_cycle;

        // Baseline for the (scenario baseline, scenario end] nonblock window:
        // scenario_start is the same reference COUPLED-SUMMARY already uses for
        // its cycle count, so the delta covers the warm-up + job window.
        const uint64_t base_nonblock_fire = n_nonblock_fire;
        const uint64_t base_nonblock_busy = n_nonblock_busy;

        for (uint32_t r = 0; r < WARM_ROUNDS; ++r) warm_round(a_pages);

        // Bounded per-scenario budget; scales with the job count and V gap.
        const long budget = scenario_budget(job_count, v_gap_cycles);

        uint32_t v_next = 0, a_next = 0;
        // Optional V-only inter-job gap.  After a V 4 KiB job completes, the
        // next V job is only started once v_gap_cycles complete ticks have
        // elapsed.  A is never gated here: the A start below is unchanged and
        // tick() keeps running, so A keeps progressing while V is idle.  The
        // gap is purely temporal and never touches any IOVA/PA or page stride.
        long v_ready_at = -1;
        // --continuous-aggressor is only meaningful with an A aggressor
        // present; solo scenarios keep the exact historical behaviour.
        const bool continuous = continuous_aggressor && a_pages > 0;
        start_job(0, v_next++);
        if (a_pages) { start_job(1, a_next++); ++a_issued; }

        // Exit condition.  Default: stop launching once both DMAs have issued
        // their fixed job_count jobs (the drain loop finishes the in-flight
        // job).  Continuous-aggressor: A is unbounded, so the loop can only end
        // after V's FINAL job has actually completed
        // (v_next == job_count && dma[0].job_done) AND the last launched A job
        // has drained.
        while (continuous
                 ? !((v_next >= job_count && dma[0].job_done) &&
                     !(a_pages && !dma[1].job_done))
                 : (v_next < job_count || (a_pages && a_next < job_count))) {
            tick();
            // A-job completion edge (A jobs are strictly serial: one active at
            // a time), counted in BOTH loops so the drain of the last A job is
            // observed in the default runs as well.
            if (a_pages) {
                if (!a_done_prev && dma[1].job_done) ++a_completed;
                a_done_prev = dma[1].job_done;
            }
            // A pending planned V gap is a deliberate idle: widen the progress
            // window by that bounded gap so a planned gap is never reported as
            // a stall, while a real stall is still caught.
            long idle_limit = WATCHDOG_CYCLES;
            if (v_ready_at >= 0) idle_limit += (long)v_gap_cycles;
            if (g_cycle - last_progress_cycle > idle_limit) {
                dump_stall(std::string("scenario ") + name);
                throw TestFailure("watchdog: no handshake progress");
            }
            if (dma[0].job_done && v_next < job_count) {
                if (v_gap_cycles == 0) {
                    start_job(0, v_next++);
                    v_ready_at = -1;
                } else if (v_ready_at < 0) {
                    v_ready_at = g_cycle + (long)v_gap_cycles;
                } else if (g_cycle >= v_ready_at) {
                    start_job(0, v_next++);
                    v_ready_at = -1;
                }
            }
            if (a_pages && dma[1].job_done) {
                if (continuous) {
                    // Stop launching A the moment V's final job is done; the
                    // in-flight A job is drained by the loop exit condition.
                    const bool v_final_done =
                        (v_next >= job_count) && dma[0].job_done;
                    if (!v_final_done) { start_job(1, a_next++); ++a_issued; }
                } else if (a_next < job_count) {
                    start_job(1, a_next++);
                    ++a_issued;
                }
            }
            if (g_cycle - scenario_start > budget) {
                dump_stall(std::string("scenario ") + name);
                throw TestFailure("scenario budget exceeded");
            }
        }
        while (!dma[0].job_done || (a_pages && !dma[1].job_done)) {
            tick();
            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                dump_stall(std::string("scenario-drain ") + name);
                throw TestFailure("watchdog: no handshake progress during drain");
            }
            if (g_cycle - scenario_start > budget) {
                dump_stall(std::string("scenario-drain ") + name);
                throw TestFailure("scenario drain budget exceeded");
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

        // ---- OBSERVED nonblock activity in (scenario baseline, end] -----
        const uint64_t d_nonblock_fire = n_nonblock_fire - base_nonblock_fire;
        const uint64_t d_nonblock_busy = n_nonblock_busy - base_nonblock_busy;
        // Non-synthetic guard, disabled scenarios only: with the runtime enable
        // low the TL must not run the nonblock path at all.  No scenario is
        // required to show a nonblock event.
        if (!enable) {
            CHECK(d_nonblock_fire == 0,
                  "nonblock activity observed while the runtime enable was 0");
        }
        std::printf("COUPLED-SUMMARY %s couple_depth=%u jobs=%u cycles=%ld comp_ar=%lu "
                    "ddtw_ar=%lu ptw_ar=%lu tl_accept=%lu tl_resp=%lu "
                    "iotlb_look=%lu iotlb_hit=%lu ptw_active=%lu r_beats=%lu b_up=%lu "
                    "nonblock_fire=%lu nonblock_busy=%lu "
                    "v_gap=%u policy=%d\n",
                    name, couple_depth, job_count, g_cycle - scenario_start,
                    (unsigned long)n_comp_ar, (unsigned long)n_ddtw_ar,
                    (unsigned long)n_ptw_ar, (unsigned long)n_tl_accept,
                    (unsigned long)n_tl_resp, (unsigned long)n_iotlb_lookup,
                    (unsigned long)n_iotlb_hit, (unsigned long)n_ptw_active,
                    (unsigned long)n_r_beats, (unsigned long)n_b_up,
                    (unsigned long)d_nonblock_fire,
                    (unsigned long)d_nonblock_busy,
                    v_gap_cycles, (int)policy_partition);
        // Continuous-aggressor accounting (additive; only printed for the
        // opt-in flag so the default 12-row output is unchanged).
        if (continuous_aggressor)
            std::printf("COUPLED-AGGRESSOR %s continuous=%d a_issued=%lu "
                        "a_completed=%lu v_jobs=%u\n",
                        name, (int)continuous, (unsigned long)a_issued,
                        (unsigned long)a_completed, job_count);
    }
};

// ---------------------------------------------------------------------------
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    // (runtime enable, aggressor pages, ptw latency, opt-in)
    // The first 12 rows are the historical default plan; the four a16 rows run
    // only when named with +scenario=<name>.
    const struct { const char* name; bool en; uint32_t a; uint32_t lat; bool opt_in; } plan[] = {
        { "mode1-solo-l1",  false, 0,  1,  false },
        { "mode1-solo-l31", false, 0,  31, false },
        { "mode1-a1-l1",    false, 1,  1,  false },
        { "mode1-a1-l31",   false, 1,  31, false },
        { "mode1-a8-l1",    false, 8,  1,  false },
        { "mode1-a8-l31",   false, 8,  31, false },
        { "mode1-a16-l1",   false, 16, 1,  true  },
        { "mode1-a16-l31",  false, 16, 31, true  },
        { "mode2-solo-l1",  true,  0,  1,  false },
        { "mode2-solo-l31", true,  0,  31, false },
        { "mode2-a1-l1",    true,  1,  1,  false },
        { "mode2-a1-l31",   true,  1,  31, false },
        { "mode2-a8-l1",    true,  8,  1,  false },
        { "mode2-a8-l31",   true,  8,  31, false },
        { "mode2-a16-l1",   true,  16, 1,  true  },
        { "mode2-a16-l31",  true,  16, 31, true  },
    };

    // optional single-scenario selection: +scenario=<name>
    // optional priority-order selection: +order=vv|aa|av|rr  (default vv)
    // optional overlap mode:              --overlap
    // optional coupling depth:            +couple-depth=3|512 (default 3)
    //
    //   vv : AR prefers V, AW prefers V   (matching-source order, default)
    //   aa : AR prefers A, AW prefers A
    //   av : AR prefers V, AW prefers A   (legacy adversarial order)
    //   rr : AR/AW round-robin independently; after each real DMA AR/AW
    //        handshake the preference flips to the other DMA.  A warm-up AR
    //        (owner 2) never changes either RR cursor.  This interleaves the
    //        two halves of both 4 KiB jobs on the shared upstream port as a
    //        legal AXI stream, e.g. V.h0 -> A.h0 -> V.h1 -> A.h1.
    // optional V inter-job gap:           +v-gap-cycles=0|1000|4000 (default 0)
    //   The V victim waits this many complete ticks between finishing one
    //   4 KiB job and starting the next one.  The wait is counted by tick(),
    //   so A keeps progressing; A is never gated by this value and no IOVA/PA
    //   or page distance is changed.
    // optional IOTLB partition policy:    +policy=0|1 (default 0)
    //   Forwarded to the wrapper input policy_partition_i.  It is only
    //   observable in a wrapper elaborated with -GPolicyEnable=1 (python
    //   --policy), which is also the only reason the obj directory is split.
    std::string only;
    std::string order = "vv";
    std::string couple_depth_str = "3";
    std::string v_gap_str = "0";
    std::string policy_str = "0";
    std::string job_count_str = "64";
    bool overlap = false;
    bool fullbuffer_permit = false;
    bool continuous_aggressor = false;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        const std::string skey = "+scenario=";
        if (a.rfind(skey, 0) == 0) only = a.substr(skey.size());
        const std::string okey = "+order=";
        if (a.rfind(okey, 0) == 0) order = a.substr(okey.size());
        const std::string ckey = "+couple-depth=";
        if (a.rfind(ckey, 0) == 0) couple_depth_str = a.substr(ckey.size());
        const std::string gkey = "+v-gap-cycles=";
        if (a.rfind(gkey, 0) == 0) v_gap_str = a.substr(gkey.size());
        const std::string pkey = "+policy=";
        if (a.rfind(pkey, 0) == 0) policy_str = a.substr(pkey.size());
        const std::string jkey = "+job-count=";
        if (a.rfind(jkey, 0) == 0) job_count_str = a.substr(jkey.size());
        if (a == "--overlap") overlap = true;
        if (a == "--fullbuffer-permit") fullbuffer_permit = true;
        if (a == "--continuous-aggressor") continuous_aggressor = true;
    }
    if (order != "vv" && order != "aa" && order != "av" && order != "rr") {
        std::fprintf(stderr, "nonblock_coupled_test: unknown +order='%s' "
                             "(expected vv|aa|av|rr)\n", order.c_str());
        return 2;
    }
    // Strict depth validation: exactly the two known values.  A free-form
    // integer would let a typo silently become the coupling-FIFO bound, and the
    // overflow CHECK would then happily agree with the typo.
    uint32_t couple_depth = 0;
    if (couple_depth_str == "3")        couple_depth = 3;
    else if (couple_depth_str == "512") couple_depth = 512;
    else {
        std::fprintf(stderr, "nonblock_coupled_test: invalid +couple-depth='%s' "
                             "(expected exactly 3 or 512)\n", couple_depth_str.c_str());
        return 2;
    }
    // Strict gap/knob validation, same style as the depth check: exactly the
    // known values, never a free-form integer.
    uint32_t v_gap_cycles = 0;
    if (v_gap_str == "0")         v_gap_cycles = 0;
    else if (v_gap_str == "1000") v_gap_cycles = 1000;
    else if (v_gap_str == "4000") v_gap_cycles = 4000;
    else {
        std::fprintf(stderr, "nonblock_coupled_test: invalid +v-gap-cycles='%s' "
                             "(expected exactly 0, 1000 or 4000)\n", v_gap_str.c_str());
        return 2;
    }
    bool policy_partition = false;
    if (policy_str == "0")      policy_partition = false;
    else if (policy_str == "1") policy_partition = true;
    else {
        std::fprintf(stderr, "nonblock_coupled_test: invalid +policy='%s' "
                             "(expected exactly 0 or 1)\n", policy_str.c_str());
        return 2;
    }
    // Strict job-count validation: exactly the two known values (same style as
    // the depth/gap/policy knobs).
    uint32_t job_count = 0;
    if (job_count_str == "64")        job_count = 64;
    else if (job_count_str == "4096") job_count = 4096;
    else {
        std::fprintf(stderr, "nonblock_coupled_test: invalid +job-count='%s' "
                             "(expected exactly 64 or 4096)\n",
                             job_count_str.c_str());
        return 2;
    }
    int order_ar = 0, order_aw = 0;   // 0 = V first, 1 = A first
    bool order_rr = false;
    if (order == "aa")      { order_ar = 1; order_aw = 1; }
    else if (order == "av") { order_ar = 0; order_aw = 1; }  // legacy adversarial
    else if (order == "rr") { order_rr = true; }
    // "vv" (default) leaves both at V-first.
    if (order_rr) {
        std::printf("nonblock_coupled_test: +order=rr "
                    "(AR round-robin, AW round-robin)\n");
    } else {
        std::printf("nonblock_coupled_test: +order=%s (AR %s-first, AW %s-first)\n",
                    order.c_str(), order_ar ? "A" : "V", order_aw ? "A" : "V");
    }
    std::printf("nonblock_coupled_test: overlap_mode=%d\n", (int)overlap);
    std::printf("nonblock_coupled_test: couple_depth=%u\n", couple_depth);
    if (fullbuffer_permit && couple_depth != 512) {
        std::fprintf(stderr, "nonblock_coupled_test: --fullbuffer-permit "
                             "requires +couple-depth=512\n");
        return 2;
    }
    std::printf("nonblock_coupled_test: fullbuffer_permit=%d\n",
                (int)fullbuffer_permit);
    std::printf("nonblock_coupled_test: continuous_aggressor=%d\n",
                (int)continuous_aggressor);
    std::printf("nonblock_coupled_test: v_gap_cycles=%u\n", v_gap_cycles);
    std::printf("nonblock_coupled_test: job_count=%u\n", job_count);
    std::printf("nonblock_coupled_test: policy_partition=%d "
                "(wrapper PolicyEnable is an elaboration flag; see --policy)\n",
                (int)policy_partition);

    int ran = 0, failed = 0;
    for (const auto& s : plan) {
        if (!only.empty()) {
            // Explicit +scenario selects exactly that row (a16 rows included).
            if (only != s.name) continue;
        } else if (s.opt_in) {
            // No +scenario: the historical 12 rows run; the a16 rows are opt-in.
            continue;
        }
        ++ran;
        try {
            Tb tb;
            tb.order_ar_prefer = order_ar;
            tb.order_aw_prefer = order_aw;
            tb.rr_order        = order_rr;
            tb.overlap_mode    = overlap;
            tb.couple_depth    = couple_depth;
            tb.fullbuffer_permit = fullbuffer_permit;
            tb.v_gap_cycles      = v_gap_cycles;
            tb.policy_partition  = policy_partition;
            tb.job_count         = job_count;
            tb.continuous_aggressor = continuous_aggressor;
            tb.run_scenario(s.name, s.en, s.a, s.lat);
        } catch (const std::exception& e) {
            ++failed;
            std::fprintf(stderr, "nonblock_coupled_test: SCENARIO-FAIL %s: %s\n",
                         s.name, e.what());
        }
    }

    if (ran == 0) {
        std::fprintf(stderr, "nonblock_coupled_test: no scenario matched '%s'\n",
                     only.c_str());
        return 2;
    }
    if (failed) {
        std::printf("nonblock_coupled_test: %d/%d scenarios FAILED\n", failed, ran);
        return 1;
    }
    std::printf("nonblock_coupled_test: PASS (%d scenarios)\n", ran);
    return 0;
}
