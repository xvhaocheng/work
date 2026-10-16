# 2. Edits to `tests/nonblock_actual_dma_test.cpp`

All edits are given as **explicit replacements** of the original line ranges in the context you supplied.

### R1 — replace lines **34–35** with

```cpp
#ifdef ACTUAL_DMA_SYSTEM
#include "Vnonblock_actual_dma_system_top.h"
using TopT = Vnonblock_actual_dma_system_top;
#else
#include "Vnonblock_actual_dma_top.h"
using TopT = Vnonblock_actual_dma_top;
#endif
#include "verilated.h"
```

### R2 — replace line **116**

```cpp
    Vnonblock_actual_dma_top* top;
```
with
```cpp
    TopT* top;
```

### R3 — replace line **121**

```cpp
    enum Port { P_COMP = 0, P_DDTW = 1, P_PTW = 2 };
```
with
```cpp
    enum Port { P_COMP = 0, P_DDTW = 1, P_PTW = 2, P_DDR = 3 };
```

### R4 — insert **after line 155** (`long v_watchdog_cycles = 200000L;`)

```cpp
#ifdef ACTUAL_DMA_SYSTEM
    // Synthetic-CPU independent progress baseline (refreshed only by the CPU
    // lifecycle counters, never by DMA handshakes).
    long     cpu_progress_cycle  = 0;
    long     cpu_watchdog_cycles = 200000L;
    uint32_t last_cpu_issued     = 0;
    uint32_t last_cpu_completed  = 0;
#endif
```

### R5 — replace line **182**

```cpp
    Tb() : top(new Vnonblock_actual_dma_top) {}
```
with
```cpp
    Tb() : top(new TopT) {}
```

### R6 — wrap lines **264–311** (the existing `drive_comp()` + `drive_mem_r()`) in `#ifndef ACTUAL_DMA_SYSTEM`, and append a system-mode driver

Replace lines **264–311** with:

```cpp
#ifndef ACTUAL_DMA_SYSTEM
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
        default: break;
        }
    }
#else
    // ---- SYSTEM mode: ONE shared DDR port (comp + DS merged by the xbar) ----
    // The plain DDR model latency applies uniformly to every accepted DDR read;
    // the real 50/50 DS delayer adds latency on the DS path in RTL only.
    void drive_ddr() {
        top->ddr_aw_ready = 1;
        top->ddr_w_ready  = 1;
        top->ddr_ar_ready = (mem_q.size() < 16) ? 1 : 0;
        if (!cb_q.empty()) {
            top->ddr_b_valid = 1;
            top->ddr_b_id    = (uint8_t)cb_q.front();
            top->ddr_b_resp  = 0;
        } else {
            top->ddr_b_valid = 0;
            top->ddr_b_id    = 0;
            top->ddr_b_resp  = 0;
        }

        top->ddr_r_valid = 0;
        if (mem_q.empty()) return;
        MemRead& h = mem_q.front();
        if (h.latency > 0) { h.latency--; return; }

        top->ddr_r_valid = 1;
        top->ddr_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
        top->ddr_r_id    = (uint8_t)h.id;   // 6-bit shared-DDR ID echoed verbatim
        top->ddr_r_resp  = 0;
        top->ddr_r_last  = (h.beat == h.len) ? 1 : 0;
    }
#endif
```

### R7 — add system-mode counters

Insert **after line 169** (`uint64_t n_ptw_active = 0, n_iotlb_lookup = 0, n_iotlb_hit = 0;`):

```cpp
#ifdef ACTUAL_DMA_SYSTEM
    uint64_t n_ddr_ar = 0, n_ddr_aw = 0, n_ddr_b = 0, n_ddr_r = 0;
    uint64_t n_slv_ar[4] = {0, 0, 0, 0};   // by top 2 bits of the 6-bit DDR ID
    uint64_t n_walk_ddtw = 0, n_walk_ptw = 0;
#endif
```

### R8 — replace the whole `tick()` body, lines **316–475**

```cpp
    void tick() {
        top->nonblock_enable_i  = nonblock_enable  ? 1 : 0;
        top->policy_partition_i = policy_partition ? 1 : 0;

        top->cfg_sel   = cfg_sel   ? 1 : 0;
        top->cfg_valid = cfg_valid ? 1 : 0;
        top->cfg_write = cfg_write ? 1 : 0;
        top->cfg_addr  = cfg_addr;
        top->cfg_wdata = cfg_wdata;
        top->cfg_wstrb = cfg_wstrb;

        top->clk_i = 0;
#ifdef ACTUAL_DMA_SYSTEM
        drive_ddr();
#else
        drive_comp();
        drive_mem_r();
        top->ddtw_ar_ready = 1;
        top->ptw_ar_ready  = 1;
#endif
        top->eval();

        // ================= pre-edge snapshot ==============================
        bool any_fire = false;

#ifdef ACTUAL_DMA_SYSTEM
        const bool     f_ddr_ar   = top->ddr_ar_valid && top->ddr_ar_ready;
        const bool     f_ddr_aw   = top->ddr_aw_valid && top->ddr_aw_ready;
        const bool     f_ddr_w    = top->ddr_w_valid  && top->ddr_w_ready;
        const bool     f_ddr_b    = top->ddr_b_valid  && top->ddr_b_ready;
        const bool     f_ddr_r    = top->ddr_r_valid  && top->ddr_r_ready;
        const uint64_t s_ddr_ar_a = top->ddr_ar_addr;
        const uint32_t s_ddr_ar_l = (uint32_t)top->ddr_ar_len;
        const uint32_t s_ddr_ar_i = (uint32_t)top->ddr_ar_id;
        const uint64_t s_ddr_aw_a = top->ddr_aw_addr;
        const uint32_t s_ddr_aw_l = (uint32_t)top->ddr_aw_len;
        const uint32_t s_ddr_aw_i = (uint32_t)top->ddr_aw_id;
        const uint64_t s_ddr_w_d  = top->ddr_w_data;
        const uint8_t  s_ddr_w_s  = (uint8_t)top->ddr_w_strb;
        const bool     s_ddr_w_l  = top->ddr_w_last;
        any_fire = f_ddr_ar || f_ddr_aw || f_ddr_w || f_ddr_b || f_ddr_r;
#else
        const bool     s_comp_ar_v = top->comp_ar_valid;
        const bool     s_comp_ar_r = top->comp_ar_ready;
        const uint32_t s_comp_ar_i = top->comp_ar_id;
        const uint64_t s_comp_ar_a = top->comp_ar_addr;
        const uint32_t s_comp_ar_l = (uint32_t)top->comp_ar_len;

        const bool     s_ddtw_ar_v = top->ddtw_ar_valid;
        const bool     s_ddtw_ar_r = top->ddtw_ar_ready;
        const uint64_t s_ddtw_ar_a = top->ddtw_ar_addr;
        const uint32_t s_ddtw_ar_l = (uint32_t)top->ddtw_ar_len;
        const uint32_t s_ddtw_ar_i = (uint32_t)top->ddtw_ar_id;

        const bool     s_ptw_ar_v = top->ptw_ar_valid;
        const bool     s_ptw_ar_r = top->ptw_ar_ready;
        const uint64_t s_ptw_ar_a = top->ptw_ar_addr;
        const uint32_t s_ptw_ar_l = (uint32_t)top->ptw_ar_len;
        const uint32_t s_ptw_ar_i = (uint32_t)top->ptw_ar_id;

        const bool     s_comp_aw_v = top->comp_aw_valid;
        const bool     s_comp_aw_r = top->comp_aw_ready;
        const uint32_t s_comp_aw_i = top->comp_aw_id;
        const uint64_t s_comp_aw_a = top->comp_aw_addr;
        const uint32_t s_comp_aw_l = (uint32_t)top->comp_aw_len;

        const bool     s_comp_w_v = top->comp_w_valid;
        const bool     s_comp_w_r = top->comp_w_ready;
        const uint64_t s_comp_w_d = top->comp_w_data;
        const uint8_t  s_comp_w_s = (uint8_t)top->comp_w_strb;
        const bool     s_comp_w_l = top->comp_w_last;

        const bool     s_comp_b_v = top->comp_b_valid;
        const bool     s_comp_b_r = top->comp_b_ready;

        const bool     s_comp_r_v = top->comp_r_valid;
        const bool     s_comp_r_r = top->comp_r_ready;
        const bool     s_ddtw_r_v = top->ddtw_r_valid;
        const bool     s_ddtw_r_r = top->ddtw_r_ready;
        const bool     s_ptw_r_v  = top->ptw_r_valid;
        const bool     s_ptw_r_r  = top->ptw_r_ready;

        const bool f_comp_ar = s_comp_ar_v && s_comp_ar_r;
        const bool f_ddtw_ar = s_ddtw_ar_v && s_ddtw_ar_r;
        const bool f_ptw_ar  = s_ptw_ar_v  && s_ptw_ar_r;
        const bool f_comp_aw = s_comp_aw_v && s_comp_aw_r;
        const bool f_comp_w  = s_comp_w_v  && s_comp_w_r;
        const bool f_comp_b  = s_comp_b_v  && s_comp_b_r;
        const bool f_comp_r  = s_comp_r_v  && s_comp_r_r;
        const bool f_ddtw_r  = s_ddtw_r_v  && s_ddtw_r_r;
        const bool f_ptw_r   = s_ptw_r_v   && s_ptw_r_r;

        any_fire = f_comp_ar || f_ddtw_ar || f_ptw_ar || f_comp_aw || f_comp_w ||
                   f_comp_b  || f_comp_r  || f_ddtw_r  || f_ptw_r;
#endif

        if (any_fire) last_progress_cycle = g_cycle;

        // V (DMA0 = bank0) independent completion progress.
        if (top->dbg_dma_done & 1) v_progress_cycle = g_cycle;

        if (top->dbg_fast_resp_valid && top->dbg_fast_resp_ready) {
            ++n_fast;
            ++n_fast_bank[top->dbg_fast_resp_bank & 1];
        }
        if (top->dbg_tl_req_valid && top->dbg_tl_req_ready) {
            ++n_tl_accept;
            ++n_req_bank[top->dbg_tl_req_bank & 1];
        }
        if (top->dbg_tl_resp_valid && top->dbg_tl_resp_ready) {
            ++n_tl_resp;
            ++n_resp_bank[top->dbg_tl_resp_bank & 1];
        }
        if (top->dbg_tl_ptw_active) ++n_ptw_active;
        if (top->dbg_tl_iotlb_lookup) {
            ++n_iotlb_lookup;
            if (top->dbg_tl_iotlb_hit) ++n_iotlb_hit;
        }

#ifdef ACTUAL_DMA_SYSTEM
        // Underlying walker counters: the real ds_if merge hides the external
        // dd/ptw ports, so walk evidence comes from the harness taps.
        if (top->dbg_ddtw_ar_fire) ++n_walk_ddtw;
        if (top->dbg_ptw_ar_fire)  ++n_walk_ptw;
        // Synthetic-CPU independent progress.
        if (top->dbg_cpu_issued != last_cpu_issued ||
            top->dbg_cpu_completed != last_cpu_completed) {
            cpu_progress_cycle = g_cycle;
            last_cpu_issued    = top->dbg_cpu_issued;
            last_cpu_completed = top->dbg_cpu_completed;
        }
#endif

        // ================= rising edge ====================================
        top->clk_i = 1;
        top->eval();

#ifdef ACTUAL_DMA_SYSTEM
        if (f_ddr_ar) {
            ++n_ddr_ar;
            ++n_slv_ar[(s_ddr_ar_i >> 4) & 3];
            MemRead r; r.port = P_DDR; r.addr = s_ddr_ar_a;
            r.len = (uint8_t)s_ddr_ar_l; r.id = (uint8_t)s_ddr_ar_i;
            r.beat = 0; r.latency = (int)ptw_latency;
            mem_q.push_back(r);
        }
        if (f_ddr_aw) {
            ++n_ddr_aw;
            CawEnt c; c.id = s_ddr_aw_i; c.addr = s_ddr_aw_a;
            c.len = (uint8_t)s_ddr_aw_l; c.got = 0;
            caw_q.push_back(c);
        }
        if (f_ddr_w) {
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            CawEnt& c = caw_q.front();
            CHECK(c.got < (uint32_t)c.len + 1, "too many completion W beats");
            write_beat(c.addr + (uint64_t)c.got * 8, s_ddr_w_d, s_ddr_w_s);
            c.got++;
            if (s_ddr_w_l) {
                CHECK(c.got == (uint32_t)c.len + 1, "completion W burst length mismatch");
                cb_q.push_back(c.id);
                caw_q.pop_front();
            }
        }
        if (f_ddr_b) {
            ++n_ddr_b;
            CHECK(!cb_q.empty(), "completion B handshake with empty cb_q");
            cb_q.pop_front();
        }
        if (f_ddr_r) {
            ++n_ddr_r;
            MemRead& h = mem_q.front();
            CHECK(h.port == P_DDR, "R handshake on an unexpected port");
            if (h.beat == h.len) mem_q.pop_front();
            else                 h.beat++;
        }
#else
        if (f_comp_ar) {
            ++n_comp_ar;
            MemRead r; r.port = P_COMP; r.addr = s_comp_ar_a;
            r.len = (uint8_t)s_comp_ar_l; r.id = (uint8_t)s_comp_ar_i;
            r.beat = 0; r.latency = 1;
            mem_q.push_back(r);
        }
        if (f_ddtw_ar) {
            ++n_ddtw_ar;
            MemRead r; r.port = P_DDTW; r.addr = s_ddtw_ar_a;
            r.len = (uint8_t)s_ddtw_ar_l; r.id = (uint8_t)s_ddtw_ar_i;
            r.beat = 0; r.latency = 1;
            mem_q.push_back(r);
        }
        if (f_ptw_ar) {
            ++n_ptw_ar;
            MemRead r; r.port = P_PTW; r.addr = s_ptw_ar_a;
            r.len = (uint8_t)s_ptw_ar_l; r.id = (uint8_t)s_ptw_ar_i;
            r.beat = 0; r.latency = (int)ptw_latency;
            mem_q.push_back(r);
        }
        if (f_comp_aw) {
            CawEnt c; c.id = s_comp_aw_i; c.addr = s_comp_aw_a;
            c.len = (uint8_t)s_comp_aw_l; c.got = 0;
            caw_q.push_back(c);
        }
        if (f_comp_w) {
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            CawEnt& c = caw_q.front();
            CHECK(c.got < (uint32_t)c.len + 1, "too many completion W beats");
            write_beat(c.addr + (uint64_t)c.got * 8, s_comp_w_d, s_comp_w_s);
            c.got++;
            if (s_comp_w_l) {
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
#endif

        top->clk_i = 0;
        top->eval();
        ++g_cycle;
    }
```

### R9 — in `reset()`, after line **569** (`v_progress_cycle = g_cycle;`)

```cpp
#ifdef ACTUAL_DMA_SYSTEM
        n_ddr_ar = n_ddr_aw = n_ddr_b = n_ddr_r = 0;
        n_slv_ar[0] = n_slv_ar[1] = n_slv_ar[2] = n_slv_ar[3] = 0;
        n_walk_ddtw = n_walk_ptw = 0;
        cpu_progress_cycle = g_cycle;
        last_cpu_issued    = 0;
        last_cpu_completed = 0;
#endif
```

### R10 — replace the `check_quiet()` quiet predicate, lines **586–588**

```cpp
            if (top->dbg_in_flight == 0 && top->dbg_dma_job_quiet == 3) {
                quiet = true;
                break;
            }
```
with
```cpp
#ifdef ACTUAL_DMA_SYSTEM
            const bool cpu_q = top->dbg_cpu_quiet;
#else
            const bool cpu_q = true;
#endif
            if (top->dbg_in_flight == 0 && top->dbg_dma_job_quiet == 3 && cpu_q) {
                quiet = true;
                break;
            }
```

### R11 — in `dump_stall()`, insert **after line 668** (end of the DMA1 fprintf)

```cpp
#ifdef ACTUAL_DMA_SYSTEM
        std::fprintf(stderr,
            "  SYS cpu issued=%u completed=%u quiet=%d active=%d\n",
            (unsigned)top->dbg_cpu_issued, (unsigned)top->dbg_cpu_completed,
            (int)top->dbg_cpu_quiet, (int)top->dbg_cpu_active);
        std::fprintf(stderr,
            "  SYS ddr ar_v=%d ar_r=%d r_v=%d r_r=%d aw_v=%d aw_r=%d b_v=%d b_r=%d "
            "ds ar_v=%d ar_r=%d r_v=%d r_r=%d comp ar_f=%d aw_f=%d w_f=%d "
            "ddtw_f=%d ptw_f=%d\n",
            (int)top->dbg_ddr_ar_valid, (int)top->dbg_ddr_ar_ready,
            (int)top->dbg_ddr_r_valid,  (int)top->dbg_ddr_r_ready,
            (int)top->dbg_ddr_aw_valid, (int)top->dbg_ddr_aw_ready,
            (int)top->dbg_ddr_b_valid,  (int)top->dbg_ddr_b_ready,
            (int)top->dbg_ds_ar_valid,  (int)top->dbg_ds_ar_ready,
            (int)top->dbg_ds_r_valid,   (int)top->dbg_ds_r_ready,
            (int)top->dbg_comp_ar_fire, (int)top->dbg_comp_aw_fire,
            (int)top->dbg_comp_w_fire,  (int)top->dbg_ddtw_ar_fire,
            (int)top->dbg_ptw_ar_fire);
        std::fprintf(stderr,
            "  SYS ddr_ar=%lu ddr_aw=%lu ddr_r=%lu  slv_ar cpu=%lu dbg=%lu ds=%lu comp=%lu\n",
            (unsigned long)n_ddr_ar, (unsigned long)n_ddr_aw, (unsigned long)n_ddr_r,
            (unsigned long)n_slv_ar[0], (unsigned long)n_slv_ar[1],
            (unsigned long)n_slv_ar[2], (unsigned long)n_slv_ar[3]);
        if (!mem_q.empty()) {
            const MemRead& h = mem_q.front();
            std::fprintf(stderr,
                "  SYS ddrR.head slv=%u id=0x%02x addr=0x%016lx len=%u beat=%u lat=%d\n",
                (unsigned)((h.id >> 4) & 3), (unsigned)h.id,
                (unsigned long)h.addr, (unsigned)h.len, h.beat, h.latency);
        }
#endif
```

### R12 — in the V-completion loop of `run_scenario()`, insert after line **818** (`}` closing the V-progress check)

```cpp
#ifdef ACTUAL_DMA_SYSTEM
                if ((top->dbg_dma_batch_active & 1) &&
                    (g_cycle - cpu_progress_cycle > cpu_watchdog_cycles)) {
                    dump_stall("scenario CPU progress");
                    throw TestFailure("watchdog: synthetic CPU made no progress");
                }
#endif
```

### R13 — replace the mechanism-guard block, lines **872–881**

```cpp
        // ---- 7. mechanism guards ---------------------------------------
        CHECK(n_comp_ar > 0, "no completion reads observed");
        CHECK(n_r_beats + n_comp_ar > 0, "no R traffic observed");
        CHECK(n_tl_accept > 0, "no translation accepted by the TL");
        CHECK(n_ddtw_ar + n_ptw_ar > 0, "no walker reads observed");
        // With nonblocking disabled the fast path must never fire.
        if (!enable) {
            CHECK(n_fast == 0 && n_fast_bank[0] == 0 && n_fast_bank[1] == 0,
                  "nonblocking fast path fired while nonblocking is disabled");
        }
```
with
```cpp
        // ---- 7. mechanism guards ---------------------------------------
#ifdef ACTUAL_DMA_SYSTEM
        CHECK(n_ddr_ar > 0, "no shared-DDR reads observed");
        CHECK(n_ddr_aw > 0, "no shared-DDR writes observed");
        CHECK(n_ddr_r  > 0, "no shared-DDR R beats observed");
        CHECK(n_tl_accept > 0, "no translation accepted by the TL");
        // Walker evidence comes from the underlying harness taps, not from the
        // (merged-away) external dd/ptw ports.
        CHECK((n_walk_ddtw + n_walk_ptw) > 0, "no walker reads observed");
        // Synthetic CPU must actually run and drain (no false PASS by skipping
        // the new branch).
        CHECK(top->dbg_cpu_completed > 0, "synthetic CPU completed no reads");
        CHECK(top->dbg_cpu_quiet, "synthetic CPU not quiet at terminal");
        // The idle debug port must never produce shared-DDR traffic.
        CHECK(n_slv_ar[1] == 0, "idle debug port produced shared-DDR reads");
        if (!enable) {
            CHECK(n_fast == 0 && n_fast_bank[0] == 0 && n_fast_bank[1] == 0,
                  "nonblocking fast path fired while nonblocking is disabled");
        }
        std::printf("SYSTEM walk ddtw=%lu ptw=%lu ddr_ar=%lu ddr_aw=%lu ddr_r=%lu "
                    "slv_ar cpu=%lu ds=%lu comp=%lu cpu_issued=%u cpu_completed=%u\n",
                    (unsigned long)n_walk_ddtw, (unsigned long)n_walk_ptw,
                    (unsigned long)n_ddr_ar, (unsigned long)n_ddr_aw,
                    (unsigned long)n_ddr_r, (unsigned long)n_slv_ar[0],
                    (unsigned long)n_slv_ar[2], (unsigned long)n_slv_ar[3],
                    (unsigned)top->dbg_cpu_issued, (unsigned)top->dbg_cpu_completed);
#else
        CHECK(n_comp_ar > 0, "no completion reads observed");
        CHECK(n_r_beats + n_comp_ar > 0, "no R traffic observed");
        CHECK(n_tl_accept > 0, "no translation accepted by the TL");
        CHECK(n_ddtw_ar + n_ptw_ar > 0, "no walker reads observed");
        if (!enable) {
            CHECK(n_fast == 0 && n_fast_bank[0] == 0 && n_fast_bank[1] == 0,
                  "nonblocking fast path fired while nonblocking is disabled");
        }
#endif
```

`main()` needs no modification: both builds accept the same `+key=value` plusargs. (The `--system` switch is a **build/runner** concern.)

---

# 3. Edits to `tests/test_nonblock_actual_dma.py`

### S1 — add `system_sources()` after `production_fifo_sources()` (insert after line **166**)

```python
def system_sources():
    """Extra production sources required by the SYSTEM-mode top:
    rv_iommu_ds_if + axi_delayer/stream_delay + axi_xbar family
    (axi_xbar -> axi_xbar_unmuxed -> axi_multicut -> axi_cut, addr_decode)
    and the stream_mux used by rv_iommu_ds_if.  axi_atop_filter /
    onehot_to_bin are already appended by idma_sources()."""
    axi = IOMMU / "vendor" / "axi" / "src"
    cc  = IOMMU / "vendor" / "common_cells" / "src"
    return [
        cc  / "stream_delay.sv",
        cc  / "addr_decode.sv",
        cc  / "stream_mux.sv",
        axi / "axi_cut.sv",
        axi / "axi_multicut.sv",
        axi / "axi_delayer.sv",
        axi / "axi_xbar_unmuxed.sv",
        axi / "axi_xbar.sv",
        IOMMU / "rtl" / "ext_interfaces" / "rv_iommu_ds_if.sv",
        ROOT / "tests" / "nonblock_actual_dma_system_top.sv",
    ]
```

### S2 — add the `--system` argument, after line **199**

```python
    ap.add_argument("--system", action="store_true",
                    help="build the shared-DDR system-integration top "
                         "(tests/nonblock_actual_dma_system_top.sv) with the "
                         "production rv_iommu_ds_if / axi_delayer(50/50) / "
                         "axi_xbar; compiles with -DACTUAL_DMA_SYSTEM and uses "
                         "the separate object directory out/nonblock-actual-dma/obj-system")
```

### S3 — replace lines **202–221** (source/objdir/args setup) with

```python
    sibling = load_sibling()
    tb = ROOT / "tests" / "nonblock_actual_dma_test.cpp"

    if opts.system:
        top_name = "nonblock_actual_dma_system_top"
        obj_dir  = OUT / "obj-system"
        defines  = ["-DACTUAL_DMA_SYSTEM"]
        extra    = [require(p) for p in system_sources()]
    else:
        top_name = TOP
        obj_dir  = OUT / "obj"
        defines  = []
        extra    = [require(ROOT / "tests" / "nonblock_actual_dma_top.sv")]

    sources = dedup(
        [require(ARIANE_PKG_SRC)]
        + [require(p) for p in sibling.SOURCES
           if p not in REPLACED_BY_PRODUCTION]
        + [require(p) for p in idma_sources()]
        + [require(p) for p in production_fifo_sources()]
        + extra
    )
    incdirs = dedup(list(sibling.INCDIRS) + idma_incdirs())

    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
    ] + defines + [
        "--top-module", top_name,
        "--Mdir", str(obj_dir),
    ]
```

### S4 — replace line **245** exe path

```python
    exe = obj_dir / "V{}".format(TOP)
```
with
```python
    exe = obj_dir / "V{}".format(top_name)
```

### S5 — replace the run-args/print block, lines **251–264**

```python
    run_args = [
        str(exe),
        "+mode={}".format(opts.mode),
        "+a-pages={}".format(opts.a_pages),
        "+v-gap={}".format(opts.v_gap),
        "+transfers={}".format(opts.transfers),
        "+warmup={}".format(opts.warmup),
        "+ptw-latency={}".format(opts.ptw_latency),
        "+a-target={}".format(opts.a_target),
    ]
    if opts.system:
        run_args += ["+system=1"]
    print("test_nonblock_actual_dma: system={} top={} mode={} a_pages={} v_gap={} "
          "transfers={} warmup={} ptw_latency={} a_target={}".format(
              int(opts.system), top_name, opts.mode, opts.a_pages, opts.v_gap,
              opts.transfers, opts.warmup, opts.ptw_latency, opts.a_target))
    run = subprocess.run(run_args)
    return run.returncode
```

The `+system=1` plusarg is informational only (compile-time `-DACTUAL_DMA_SYSTEM` is the real switch); Verilator's `commandArgs` ignores unknown plusargs, so the C++ needs no handler.

---

# 4. Source additions / dependencies (no duplicate definitions)

Added only in `--system` builds:

| File | Why | Already present? |
|---|---|---|
| `.../common_cells/src/stream_delay.sv` | `axi_delayer` | no |
| `.../common_cells/src/addr_decode.sv` | `axi_xbar_unmuxed` | no |
| `.../common_cells/src/stream_mux.sv` | `rv_iommu_ds_if` W channel | no |
| `.../axi/src/axi_cut.sv` | `axi_multicut` | no |
| `.../axi/src/axi_multicut.sv` | `axi_xbar_unmuxed` `PipelineStages` | no |
| `.../axi/src/axi_delayer.sv` | DS delay (50/50) | no |
| `.../axi/src/axi_xbar_unmuxed.sv` | `axi_xbar` | no |
| `.../axi/src/axi_xbar.sv` | system fabric | no |
| `.../riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv` | real DS merge | no |
| `tests/nonblock_actual_dma_system_top.sv` | new top | new |

`axi_err_slv`, `axi_demux`, `axi_demux_simple`, `axi_mux`, `axi_id_prepend`, `fifo_v3`, `stream_arbiter`, `rr_arb_tree`, `spill_register`, `axi_atop_filter`, `onehot_to_bin` are already in the existing list — **not** duplicated.

---

# 5. Assumptions, limitations, unresolved

**Assumptions**
1. The real `rv_iommu_ddtw` emits `ARID = 1` and the real `rv_iommu_ptw` emits `ARID = 0`, matching `rv_iommu_ds_if.sv:228-235` (`0 = PTW`, `1 = DDTW`). The wrapper does **not** rewrite IDs.
2. `nonblock_actual_dma_top.comp_*` / `ddtw_*` / `ptw_*` are the only external ports to absorb; `cfg_*`, the 10 TL taps and the 9 DMA taps are unchanged.
3. `--ptw-latency` is interpreted as the shared-DDR model read latency applied uniformly in system mode; the real DS delayer adds latency in RTL. **Flagged as an interpretation** — if the intended meaning is "DS reads only get this latency", change the `r.latency` assignment in `R8` to a per-source branch on `(id >> 4) == 2`.
4. `AXI_USER_WIDTH = 1`, `AxiIdWidthSlvPorts = 4`, master ID = 6. Matches the board (`ariane_peripherals_xilinx.sv:867`, `ariane_xilinx.sv:161`).
5. All test addresses < 4 GiB, so `addr_map[0]` (0..0x1_0000_0000 → DDR) covers everything; `mst1` is never hit.

**Limitations to be reported with every system-mode result** (also written into the SV header):
- no real CPU/caches/`axi_adapter`/dwidth-converters; the CPU master is a bounded read injector;
- only 2 of the 11 board master ports — no PLIC/CLINT/ROM/UART/SPI/GPIO/Ethernet/timer, no `dma_cfg axi_demux`;
- no MIG/DDR controller/clock-domain converter/512→64 width converter — DDR is a behavioural model;
- `cfg_*` still goes **directly** to the two `dma_core_wrap` slaves (bypasses the CPU crossbar and the production `dma_cfg axi_demux`);
- the xbar is fully connected (`Connectivity = '1`) whereas the board relies on the address rules.

**Unresolved / for Codex**
- Cannot compile. The `unique case` duplicate-`3:` label in `rv_iommu_ds_if.sv:158-159` is pre-existing and expects `-Wno-fatal`.
- Whether `n_walk_ptw > 0` in the `--warmup 256` cases: I kept the *combined* walker guard hard (as the original) and print `n_walk_ddtw`/`n_walk_ptw` individually, so a false FAIL is not introduced.
- Whether `axi_err_slv`'s parameter is `ATOPs` (typo for `ATOPs`) — I used the spelling seen in `ariane_peripherals_xilinx.sv:1325-1334` (`ATOPs` not passed there); the real module parameter is `ATOPs`. **Verify the exact casing** against `axi_err_slv.sv` before compiling.

**Suggested acceptance checks**
```
# S64 (must pass, both modes)
python3 tests/test_nonblock_actual_dma.py                          --mode s --transfers 64   --ptw-latency 31
python3 tests/test_nonblock_actual_dma.py --system                 --mode s --transfers 64   --ptw-latency 31
# N64
python3 tests/test_nonblock_actual_dma.py --system                 --mode n --transfers 64   --ptw-latency 31
# N4096 with the board DS delay in the loop
python3 tests/test_nonblock_actual_dma.py --system                 --mode n --transfers 4096 --ptw-latency 31 --warmup 256 --v-gap 1000
```
Checks: object dir `out/nonblock-actual-dma/obj-system`; log contains `SYSTEM walk ddtw=>0 ptw=>0`; `ddr_ar>0 && ddr_aw>0 && n_slv_ar[1]==0`; `cpu_completed>0`; terminal `PASS`; and on stall, the `SYS ddrR.head slv=… id=0x…` line names the blocking source.