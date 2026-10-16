# P4M-1 read-only interface mapping (no implementation)

Scope note: everything below is derived only from the supplied files. No file was written, no test/build was run. "Unused"/"free" claims are static reads of decodes, not silicon proof.

## 0. Fixed roles and workload anchors

- V0/A1, optional A, chunk 4 KiB, V 4096 jobs, warmup 256 are already the CLI defaults: `bench/idma_experiment.c:354-360` (`victim_pages=1`, `aggressor_pages=8`, `chunk_bytes=4096`, `victim_transfers=4096`, `warmup_transfers=256`).
- Batch constrains chunk and count: `bench/idma_experiment.c:144-148` requires `chunk_bytes==4096` and `victim_transfers <= IDMA_BATCH_MAX_TRANSFERS`; kernel repeats the check at `linux/drivers/misc/idma.c:1833-1835`; `IDMA_BATCH_MAX_TRANSFERS (1ULL<<20)` at `linux/include/uapi/linux/idma.h:18`.
- Roles/config come from the driver, not the frontend: `idma_run_session` sets `e.victim_did = victim->did` and resolves the aggressor at `linux/drivers/misc/idma.c:1830-1857`; in this SoC A is DMA1 when `aggressor_id==1` (`idma.c:1845-1856`, `ariane_peripherals_xilinx.sv:1096-1097` `DEVICE_ID 10+i`).
- `OBSERVER_PORT (i==0)`, `LAUNCH_PORT (InclIOMMU && i<2)`, `BATCH_PORT 1'b1` on all 4 DMA instances: `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1092-1094`. So observer/config window only exists on DMA0; A's batch register bank exists on DMA1 but its observer config bus is not routed (see §1.4).

---

## 1. Hardware batch frontend (idma_reg64_frontend.sv)

### 1.1 State machine, offer, completion, STOP

File: `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv`

- States `IDLE/COUNT/WAIT/HOLD` at `:64-70`.
- Batch state/counters at `:94-98`; `batch_active_o` at `:116`.
- Offer/completion taps at `:124-125`:
  - `batch_task_issue = batch_active_o && state_q == COUNT && ready_i`
  - `batch_task_done  = batch_active_o && state_q == WAIT  && trans_complete_i`
- Offer is one cycle of `valid_o` in COUNT, held until `ready_i`, with address generation:
  `:455-471` — `valid_o = 1'b1` in COUNT; `if (ready_i) state_n = WAIT; transf_offset_n += 13'h1000`, and ring wraparound `:469-470` (`transf_offset_n == (batch_pages_q<<12) ? 0`). Comment at `:457-458` states VALID stays asserted regardless of later cancel/hold/go until the backend is ready.
- Completion is `WAIT` + `trans_complete_i` (`:474-496`).
- STOP semantics:
  - STOP write recognized only while active and full-strobe: `:147-150` (`full_addr==8'h88 && wdata==2 => batch_stop_q <= 1`). Note it is inside the `batch_access && write && !launch_busy` guard.
  - STOP is latched and only ends the batch at the *next* completed task: `:140-143` (`batch_task_done` sets `batch_stopped_q<=1` when `batch_stop_q`) and `:484-490` (`... || (batch_enable_q && batch_stop_q)` retires and returns to IDLE).
  - STOP cannot withdraw an offered task: the offer is not conditioned on `batch_stop_q`; the comment at `:128` says "STOP drains the currently offered task."
- Batch config is immutable while active: `locked_write` at `:117-118`; writes to the batch bank while `state_q != IDLE` set sticky `batch_error_q` at `:151`; readback status assembles `error/stopped/active/enable` at `:381-383`.
- Config legality gate: `batch_cfg_ok` at `:119-123` (target 1..2^32, pages 1..4096, `num_bytes==4096`, `ttype==DEFAULT`, src/dst 4 KiB-aligned). Start path (`next_id.re`) refuses when `batch_enable_q && !batch_cfg_ok` (`:435-437`), and refuses to start if backend not ready (`:447-450`).

### 1.2 Where a gap can be inserted, and the exact constraints

The only point where "one full completed copy" is known is `WAIT` + `trans_complete_i` (`:476`). The current non-terminal path goes straight to `COUNT` (`:492-494`). So the gap must gate the `WAIT -> COUNT` edge (or sit in a new state between them). Consequences that match the stated requirements:

- "Gap only between full completed batch copy tasks": the trigger is `trans_complete_i` in WAIT, so a gap cannot be inserted after `ready_i` (which would stall the *next offer* mid-flight).
- "gap0 no extra bubble": adding an unconditional state costs one cycle even at gap 0, so the `WAIT -> COUNT` transition must remain direct when `gap==0` (or the gap state must exit combinationally). This is the single most important RTL hazard for the P4M-2 gap0 self-control.
- "No first-copy/warmup gap": the first copy is `IDLE -> COUNT` (`:434-451`), which is untouched; warmup runs in ordinary (non-batch) mode (`idma.c:1914-1935`), so gating the gap on `batch_enable_q` naturally excludes it. A simple equivalent rule is "gap only after `batch_completed_q != 0`".
- "STOP gap must drain/cancel safe": `:484-490` must be evaluated with STOP priority over gap insertion, so a batch stopped while parked in a gap exits immediately to IDLE without a new offer; STOP already terminates on the next `batch_task_done`.
- "Pending valid cannot withdraw": in a gap state `valid_o` must stay 0 (top-of-always_comb assignment `:327`), so nothing is pending to withdraw; once COUNT is entered (`:459`) the offer is held until `ready_i`.

Note the observer already treats a long REFILL/HARDWARE residency as legal (no error), so extra cycles from a gap will not trip observer validation — but they will inflate the REFILL cycle/max-segment counters (see §2.4).

### 1.3 Register decode, 8-bit routing, and where a 32-bit gap can live

- `full_addr = 8'(dma_ctrl_req_i.addr)` at `:111`.
- `launcher_access = LaunchPort && ObserverPort && valid && full_addr >= 8'hb0` at `:114` (LauncherBase `:62`).
- `batch_access = BatchPort && valid && full_addr[7] && !launcher_access` at `:115` → batch bank is `0x80..0xaf`.
- `observer_access` is an exact decode of `0x58/0x60/0x68/0x70/0x78` at `:174-177`.
- Forwarding to the external config bus: `:188-192` (`obs_cfg_valid_o = observer_access || launcher_access; obs_cfg_addr_o = full_addr;`). The observer module only ever receives `!obs_cfg_addr[7]` (see §1.4), and the launcher receives the full 8-bit address.
- Batch write decode `:147-170` handles only `0x88/0x90/0x98`; anything else in the bank is `default: batch_error_q <= 1` (`:168`). Batch read decode handles only `0x80/0x88/0x90/0x98/0xa0/0xa8`, else `error` (`:371-390`).

Used batch offsets today (frontend `:380-387`; driver `idma.c:52-57`): `0x80 magic, 0x88 control, 0x90 target, 0x98 pages, 0xa0 completed, 0xa8 issued`. **The whole `0x80..0xaf` window is 48 bytes = 6 aligned slots and all six are used.** That is a hard finding: there is no free 8-byte-aligned address inside the existing batch bank.

Candidate carriers for a 32-bit gap (Codex decision, see §5):

1. A half-slot inside the batch bank (`0x84/0x8c/0x94/0x9c/0xa4/0xac`) if the register path here supports sub-8-byte-aligned access. Alignment behaviour of `axi_to_reg` is **not** in the supplied files → unverified dependency.
2. A low-bank special decode (like `observer_access`) at a currently unmapped low address. `idma_reg64_frontend_reg_top` contents are not supplied; `0x00/08/10/18/20/28/30/50` are known, `0x38/0x40/0x48` are unverified.
3. Carve out of the launcher region (`0xb0..0xff`; `0xd0/0xd8/0xe0/0xe8/0xf0/0xf8` are unused per `idma.c:65-68`). This changes `launcher_access` (`:114`) and the observer `cfg_valid` gate at `ariane_peripherals_xilinx.sv:938`, so it touches the LaunchPort/E3 path.
4. Carry the gap in the upper 32 bits of the existing control register write (`:153`: `if (wdata <= 1)`). This conflicts with the STOP encoding (`wdata==2`) and the `<=1` check, and would need a new decode.

Any new bank offset must also be added to the `batch_access` write case with full-strobe enforcement (`:148`), and a new sticky error otherwise (`:168`).

### 1.4 SoC-level routing that constrains the gap register

`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:934-941`:

- observer gets `cfg_valid_i(obs_cfg_valid[0] && !obs_cfg_addr[0][7])`, `cfg_addr_i(obs_cfg_addr[0][6:0])`;
- launcher gets `cfg_valid_i(obs_cfg_valid[0] && obs_cfg_addr[0][7])`, `cfg_addr_i(obs_cfg_addr[0])` (full 8 bits);
- read return mux `obs_cfg_rdata = obs_cfg_addr[0][7] ? launch_cfg_rdata : legacy_cfg_rdata` at `:962`;
- all four DMA instances share one `obs_cfg_rdata` read bus (`.obs_cfg_rdata_i(obs_cfg_rdata)` at `:1105`).

Consequence: a gap register placed at `>=0x80` is invisible to the observer module, and a gap register placed in the launcher window requires the launcher routing gate to change. `dma_core_wrap.sv:130-140`/`:152` and `:398-433` purely forward ports and don't constrain the offset choice.

---

## 2. Observer: batch V window equations and available inputs

File: `cva6/corev_apu/fpga/src/dma_observer.sv`

### 2.1 Window open / close

- Phase enum `:38`; `active = HARDWARE || POLL_WAIT || REFILL` `:83`.
- Open equation `:167-175`:
  ```
  if (phase_q == ARMED && issue_i[victim_q]) begin
      phase_n = HARDWARE;
      expected_n = issue_id_i[victim_q];
      count_n[7] = 1;                       // first issue already counted
      segment_n  = 0;
      count_n[30] = (reads_n[victim_q]   << 16) | writes_n[victim_q];   // V opening stock
      count_n[31] = has_a_q ? ((reads_n[aggressor_q]<<16)|writes_n[aggressor_q]) : 0; // A opening stock
      if (issue_id_i[victim_q] == 0) error_n[1] = 1'b1;
  end
  ```
  Additional gate on the open cycle `:180-181`: for `!finite_q && batch_q`, A must already be batch-active (`!batch_active_i[aggressor_q]` → `error_n[5]`).
- Close equation (batch) `:210-226`:
  ```
  HARDWARE: bump(1,1); if (issue_i[victim_q]) error_n[1]=1;
            if (done_i[victim_q]) begin
               if (done_id_i[victim_q] != expected_q) error_n[1]=1;
               bump(8,1);
               if (segment_n > count_q[4]) count_n[4]=segment_n; segment_n=0;
               if (batch_q) begin
                  if (count_n[8] == target_q) begin
                      phase_n = BATCH_READ;
                      count_n[32] = V closing stock; count_n[33] = A closing stock;
                  end else phase_n = REFILL;
               end else phase_n = POLL_WAIT;
            end
  ```
  So the hardware V window is exactly `[first V issue] → [target-th V done]`, and both closing stocks are sampled *in the done cycle* using `reads_n/writes_n`.
- Freeze/drain `:182-189` (`BATCH_READ` counts `34`, forbids `issue_i/done_i`, freezes on `poll_id_i == expected_q` → `FROZEN`), and non-batch close `POLL_WAIT:230-238` (freeze on polled target with `count_n[9]==target_q`).
- REFILL `:241-252`: `done_i[victim_q]` is an error here; next `issue_i[victim_q]` re-enters HARDWARE and re-samples `expected_n`.

### 2.2 Exact same-edge semantics (important for tests)

- `reads_n/writes_n` are computed first (`:139-165`) and already include the current cycle's `{ar_valid&&ar_ready, rlast_fire}` / `{aw_valid&&aw_ready, b_fire}`; simultaneous accept+return is a net zero (`:151` comment).
- On the **open** cycle (`phase_q==ARMED`), `active` is false (`:83`), so the request-side counters (`14..25`), shared counters (`26..29`), and `count[0..3]` are **not** bumped that cycle; only `count[7]=1` and the two opening stocks are written. The first V request's own AXI events are still reflected in the opening stock because `reads_n/writes_n` already applied them.
- On the **close** cycle (`phase_q==HARDWARE`, target-th done), the `active` block (`:190-208`) runs *before* the `case` (`:209-254`), so that cycle's `count[0], count[1], count[8], role counters` are all inside the window and only the stocks are sampled after `bump(8,1)`.
- `store` side: counters `count[30..33]` are opening/closing stocks; `count[7]=issued`, `count[8]=done`, `count[9]=polled`; `count[10..13]` = `{a_pending,v_pending}` histogram (`:194`); `count[14..19]` = V AXI, `count[20..25]` = A AXI (`:195-204`); `count[26..29]` shared; `count[0..3]` cycles per phase; `count[4..6]` max segments; `count[34]` done-to-read (`:182-189`). `Count = 35` at `:37`.

### 2.3 Per-lane IDs and inputs available to track A inflight before the window

- Per-lane event inputs already exist: `issue_i/done_i/poll_i` (`:18`), `issue_id_i/done_id_i/poll_id_i` (`:20-22`), plus AXI level/ready and `rlast_fire_i/b_fire_i/bad_id_i` (`:23-24`) and `batch_active_i` (`:19`).
- Frontend provides 1-based position IDs in batch mode: `obs_issue_id_o = batch_issued_q+1`, `obs_done_id_o = batch_completed_q+1` (`idma_reg64_frontend.sv:236-244`), monotone within a batch because both counters reset on batch start (`:136-137`).
- The observer currently counts **only V** issue/done (`count[7]`, `count[8]` in `HARDWARE`). A's `issue_i/done_i` are wired but never counted per lane. A's inflight *before* the window is only readable as the opening stock `count[31]` captured at the V open cycle — there is no counter for "A done inside window but issued before open", and no counter for A dones at all. This is the gap for the "born-before-open done counts" requirement (§4).
- Free-running per-port stock (`reads_q/writes_q`) is intentionally never reset on ARM (`:138` comment), so `count[31]` at open is the correct "A requests outstanding at V issue".

### 2.4 Index space and ARM bits

- `index_q` is 9 bits (`:47`), so selector max 511.
- Read decode `:97-116`: `0..34` counters; `64..191` path (`PathEnable`); `192..447` E3 (`E3Enable`); **`448..511` currently decode to 0** (no branch).
- Write gate `:259-265`: index accepted only for `<Count`, or `PathEnable && [64,192)`, or `E3Enable && [192,448)`. So `448..511` is *writable nowhere* today; using it requires extending this gate with a new enable term (and extending `Count` or adding a fourth bank). `e3_index_o = 8'(index_q - 192)` (`:57`) is 8-bit and only valid for E3.
- Target register `7'h68` `:260-261`; ARM is a write to `7'h60` `:266-300`. Arm word bit usage (from the accept expression `:275-285`): used bits are `0, 8, 9, 16, 17, 24, 25, 26, 27, 28`, plus `29` when `ControlledEnable && E3Enable`. The `(cfg_wdata_i & ~MASK)==0` test means **any new arm bit requires extending the mask constant**; free bits are `1..7, 10..15, 18..23, 30, 31`. Cross-constraints to remember: bit27 (E3) requires bit26 (path); bit28 (delay128) requires bit27; bit29 (finite) requires `ControlledEnable && E3Enable && bit27 && bit26 && bit25` with `target==1`, port0, and `(!bit24 || aggressor_port==1)`.
- Control-status read `:100-108`: `[0]=ARMED`, `[1]=busy`, `[2]=FROZEN`, `[63:32]=error_q`, `[38]=path error`, `[39]=E3 error`; bits `3..31` free.
- Magic (interface version) is `:34-36`; P4M-1 adds a new variant only if an existing bit/version contract cannot express it. `IDMA_OBS_MAGIC_V6 = …5306` (`idma.c:48`), and the driver gates capabilities on exact magic (see §3).

---

## 3. Linux batch lifecycle and ABI extension route

File: `linux/drivers/misc/idma.c`

### 3.1 Batch register plumbing

- Register offsets: `:52-58` (magic `0x80`, control `0x88`, target `0x90`, pages `0x98`, completed `0xa0`, issued `0xa8`); `IDMA_BATCH_MAGIC` `:58`.
- `idma_batch_configure` `:580-595`: magic check → busy check (`ctrl & 2`) → write control 0 → target → pages → control 1 → readback `==1`. Uses `s->size / PAGE_SIZE` for pages.
- `idma_batch_stop` `:597-601`: writes `2` only if `batch_configured && batch_started`.
- `idma_batch_collect` `:603-628`: reads control; if still active (`&2`) quarantines with `-EIO`; reads `issued/completed` into `stats`; requires `issued == completed`; returns `-EIO` on any `status>>32` (sticky error).
- `idma_batch_disable` `:630-634`: writes 0 if configured and not retained.
- `idma_batch_window` `:636-684`: optional A `idma_submit` (`:650-655`), V `idma_submit` (`:656-659`), set V deadline, then **poll V only** (`:670`) until done, capturing `last_complete_ns`. No per-job CPU supply: one batch descriptor per lane.

### 3.2 Run flow (`idma_run_session`, `:1811-2081`)

Order of operations on the measured round (`:1890-2032`):

1. `local_flush_icache_all()` before warmup (`:1908`).
2. A warmup submit if `!batch || warmup_transfers` (`:1914-1918`), then software warmup window `idma_run_window(v, a, warmup, …, false)` (`:1919`), offsets/last-complete reset to 0 (`:1933-1937`).
3. Batch: drain A (`:1940-1944`), `idma_batch_configure(v, victim_transfers)` and `idma_batch_configure(a, IDMA_EXPERIMENT_MAX_TRANSFERS)` (`:1955-1959`).
4. `in_flight_at_start` recorded (`:1961-1962`), observer arm (`:1965`) **after** batch configure, then HPM+active snapshots, `start = ktime_get_ns()` (`:1971-1974`).
5. `idma_batch_window` (`:1975-1976`), `victim_elapsed_ns = end - start` (`:1977`), `in_flight_at_end` (`:1979-1980`).
6. STOP A after V is observed (`:1981-1986`), then post-window snapshot and deltas (`:1988-1994`).
7. `out_drain`: STOP V and A again (`:1996-1999`), `idma_drain` both (`:2000-2009`), `idma_batch_collect` both, require `victim.completed == victim_transfers` (`:2010-2019`).
8. Verify only after both drained (`:2021-2031`), `out_cleanup` calls `idma_observation_finish` (`:2034`) and `idma_batch_disable` (`:2037-2040`).

Observer lifecycle helpers: `idma_observation_prepare` `:755-786` (magic/version gate, refuses busy, writes `4` to clear), `idma_observation_arm` `:788-812` (builds arm word, writes target then control, readback `==1`), `idma_observation_finish` `:815-918` (aborts if still busy, reads 34 words, runs conservation checks, logs).

Validation/limits: `idma_validate` `:706-723`; caps advertised at `:2142-2154` (`CAP_SW_REFILL|CAP_HW_BATCH|…`); ioctl dispatch `:2156-2175` (`IDMA_IOC_RUN_BATCH` `:2169-2170`).

### 3.3 ABI extension route (old ioctls untouched)

- Old surface is frozen: `struct idma_experiment` is versioned by `abi_version`/`struct_size` and rejects nonzero `flags`/`reserved` (`idma.c:710-713`; `idma.h:52-79`). `IDMA_IOC_RUN_EXPERIMENT`/`IDMA_IOC_RUN_BATCH` = `0x07`/`0x08` (`idma.h:83,89`).
- Precedent for extending without touching old ABIs: `idma_e3_pair` embeds `struct idma_experiment` and gets its own `_IOWR('i', 0x09, …)` (`idma.h:96-108`); `idma_e3_controlled` does the same at `0x0c` (`idma.h:170-178`).
- So the smallest route is a new ioctl number (next free `0x0d`) + new struct embedding `struct idma_experiment` plus gap fields, a new CAP bit (free bits exist; current capabilities are bits 0..6 at `idma.h:11-17`, `idma.c:2145-2148`), and a new dispatch case at `idma.c:2156-2175`. Existing 0x07/0x08 paths must remain byte-identical.
- Gap programming must happen while the batch engine is IDLE and before enabling, because writes while `state_q != IDLE` set sticky `batch_error_q` (`idma_reg64_frontend.sv:151`) and the driver treats `status>>32` as fatal (`idma.c:627`). It must also be covered by a new legality gate alongside `batch_cfg_ok` (`idma_reg64_frontend.sv:119-123`).

---

## 4. CLI / rootfs / test reuse and the tests the requirement names

### 4.1 Existing entry points to reuse

- CLI: `bench/idma_experiment.c` — option table `:63-66`, boolean flags `:95-99`, decimal limits `:117-121`, field assignment `:122-130`, positivity checks `:132-137`, batch checks `:144-148`. A `--gap` option is a new entry in `names[]`, a `case` in the switch, and a new positive/limit check; nothing else structurally changes.
- JSON/semantics: `print_stats` `:220-251`, `report` `:253-344` (batch semantics text at `:322-332`). New fields land here if a new ioctl is used.
- Rootfs packaging: `tests/test_rootfs.py:12-16` loads `scripts/rootfs.py` (not supplied) and asserts over `rootfs.EXECUTABLES`, `rootfs.LEGACY_NAMES`, `rootfs.repack`, `rootfs.write_entry`, `rootfs.read_newc` (`:51-133`). If P4M-1 adds no new userspace binary, this file needs no change.
- Frontend unit harness: `tests/idma_frontend_batch_test.cpp` — `Bench::configure` writes `0x88/0x90/0x98` and triggers `0x28` (`:31`), `drain()` polls `batch_active_o` (`:32`), stall experiment `:38-39`, STOP experiment `:44-46`, STOP-under-stall experiment `:52-54`, consecutive batches `:55-58`. Test entry `tests/test_idma_frontend_batch.py` builds Verilator with a `git show 7f086b73…:idma_reg64_frontend.sv` "before" module (`:10-12`, `:20`) — this is the natural gap0 legacy regression.
- Differential observer/legacy harness: `tests/idma_frontend_observer_top.sv:31-50` instantiates the current frontend next to `idma_reg64_frontend_before` and `:59-61` asserts `unchanged_o` for the legacy path; consumed by `tests/idma_frontend_observer_test.cpp` (`:9`, `:17-25`).
- Host model of the driver: `tests/idma_scheduler_harness.c` with batch emulation at `:189-197`, MMIO decode at `:222-247`/`:249-373`, batch case at `:653-684`. The extractor `tests/test_idma_scheduler.py:17-25` lists production functions by name, and `:26-48` lists case names — **any new driver function or new case must be added to `FUNCTIONS`/`CASES` or the tests silently skip it.**

### 4.2 Meaningful tests for the named P4M-1 properties

- Window count conservation: already implemented for existing counters in `idma_observation_finish` — `idma.c:838-839` (`count0 == count1+count2+count3 == count10+count11+count12+count13`) and `idma.c:841-849` (`opening + accepted == returns + closing`, per V and A lane, reading indices `30+i`/`32+i`/`14+6i`/`16+6i`/`15+6i`/`17+6i`). A new P4M-1 A-side counter must be folded into this same identity set or a parallel identity block.
- Born-before-open done counts: no existing counter covers A events at all (see §2.3). The minimal test needs (a) a new A-issue/A-done counter pair that runs during `active`, and (b) a test where A is submitted before V's first issue, so some A dones fall inside the window and some dones before it; assert `A_issued_window + A_issued_pre = A_done_window + A_done_pre + A_inflight_at_end_of_window` style identities. The harness can force this deterministically with `hw[1].latency` (`idma_scheduler_harness.c:752`, `:774`, `:831`) — `victim_finishes_first` at `:773-783` already builds the "A still in flight when V ends" scenario.
- Events at open/close edges: already enforced by the observer and asserted by the harness: second V issue inside HARDWARE is an error (`dma_observer.sv:212`), done inside REFILL is an error (`:243`), issue/done inside BATCH_READ is an error (`:184`), and "all V transactions wholly inside the window" is checked at `idma.c:867-870`. A gap must not introduce any of these (gap lives in REFILL, §1.2).
- Sustained nonzero gap: a new frontend test alongside `configure()` in `idma_frontend_batch_test.cpp:31`, asserting that after the first completion the next offer is delayed by exactly the programmed cycles (count cycles with `request_o==0` between `done` and next `issue`), and that the first copy is not delayed.
- gap0 legacy regression: reuse `tests/idma_frontend_observer_top.sv` (`:44-50`,`:59-61`) and the `before` module from `tests/test_idma_frontend_batch.py:10-12`; with gap 0 the new frontend must be bit-identical to `idma_reg64_frontend_before`, and `tests/idma_frontend_batch_test.cpp` cases `:34-59` must still pass unchanged.
- Stop/error: extend the existing STOP cases (`idma_frontend_batch_test.cpp:44-46`, `:52-54`) with STOP issued while the frontend is parked in a gap, and assert no extra task is offered and status bit1 (`batch_stopped_q`) set; driver-side stop/error paths already exist in `idma_scheduler_harness.c:653-684` (`batch_timeout`, `batch_interrupt`, `batch_zero_id`, `batch_data_error`).
- Selective 128 OFF: `dma_observer.sv:275-282` gates bit28 after bit27; the P4M-1 driver path must not set `BIT_ULL(28)` (`idma.c:791-796`) and the new arm word must remain legal without it.

---

## 5. Smallest coherent file batches, interfaces, and Codex decisions

### 5.1 Proposed minimal file batches (interface-only statements, no code)

Batch A — frontend gap (HW core), smallest:
- `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv`: new gap storage + new `WAIT→COUNT` gating (bypass at gap 0) + new decode case + folding into `batch_cfg_ok` and, if needed, `locked_write`.
- Tests: `tests/idma_frontend_batch_test.cpp`, `tests/test_idma_frontend_batch.py`, `tests/idma_frontend_observer_top.sv`/`tests/idma_frontend_observer_test.cpp` (gap0 differential).
- Interfaces touched: `batch_access` decode, batch read mux (`:379-389`), `state_n` in WAIT (`:484-494`), `batch_cfg_ok` (`:119-123`). No new ports expected; if a new port is unavoidable, batch B is `dma_core_wrap.sv` (`:130-164`, `:398-433`).

Batch B — observer window additions:
- `cva6/corev_apu/fpga/src/dma_observer.sv`: new counters (either extend `Count` into `35..63`, which stays inside the `path` boundary at `64`, or use the free `448..511` bank with a new enable term in the `7'h70` gate), new arm bits in the `7'h60` mask, and possibly a new magic version (`:34-36`).
- Only if routing changes: `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:934-941` (observer cfg gate / launcher gate) and `:962`.
- Tests: extend `idma_observation_finish`-style conservation in the host model and add cases to `tests/idma_scheduler_harness.c` + `tests/test_idma_scheduler.py` `CASES`/`FUNCTIONS`.

Batch C — Linux ABI + batch lifecycle:
- `linux/include/uapi/linux/idma.h`: new ioctl number, new struct embedding `struct idma_experiment`, new CAP bit; old `0x07/0x08` and their structs unchanged.
- `linux/drivers/misc/idma.c`: new run function reusing `idma_batch_configure/window/stop/collect/drain`, new dispatch case in `idma_ioctl` (`:2156-2175`), new caps bit (`:2145-2148`). Old functions untouched.
- Tests: `tests/idma_scheduler_harness.c` new cases (`batch_gap_*`), `tests/test_idma_scheduler.py` `FUNCTIONS` (`:17-25`) and `CASES` (`:26-47`).

Batch D — CLI + optional rootfs:
- `bench/idma_experiment.c`: `--gap` option and semantics text. `scripts/rootfs.py`/`tests/test_rootfs.py` only change if a new binary is added; otherwise reuse the existing `idma_experiment` entry point.

### 5.2 Ambiguities requiring Codex decision (no decision implied here)

1. **Gap register location.** There is no free 8-byte slot in `0x80..0xaf`; candidates are a half-slot (`0x84…0xac`, alignment capability unverified), a low-bank special decode (`0x38/0x40/0x48`, reg_top mapping unverified), a launcher-window carve-out (`0xd0…0xf8`, changes E3 routing), or a control-word field. Codex must pick, since items 2/3 depend on it.
2. **Gap granularity and unit.** Single scalar for V only, vs per-lane, vs shared A/V; and cycles vs ns vs "N–1 clones" (the doc says the sweep/repetition points are not to be chosen now, `docs/e3-next-method-plan.md:62`).
3. **gap0 bypass style.** Combinational `WAIT→COUNT` when gap 0, vs a state that is exited in the same cycle. This determines whether P4M-2's "gap0 equals the continuous baseline" can ever hold.
4. **A-window accounting semantics.** Whether "A copies completed in the V window" is a new counter derived from `done_i[aggressor_q]` during `active`, or reconstructed from `count[31]` + AXI counters. "Born-before-open done" has no existing storage; Codex must choose whether this is a new counter or a derived quantity.
5. **Observer index bank.** Extend `Count` into `35..63` (read gate `:111`, arm/reset loops `:299`, `:324`) vs use `448..511` (needs a new enable term in `:259-265` and a new read branch in `:111-113`). Also whether a new observer magic version is required, which would force `idma_observation_prepare` gates (`idma.c:767-780`) to change.
6. **Arm bit budget.** Free arm bits are only `1..7, 10..15, 18..23, 30, 31`; if more than one new enable is needed, the mask constant `:275` and the driver arm builder `:791-796` must both change in lockstep.
7. **Where V's window ends relative to STOP.** The observer freezes only on the target-th V done; if STOP is used to cut a batch, the observer must be aborted (`idma.c:827-830`), so any new counter must be defined for the aborted path too.
8. **Sub-8-byte MMIO alignment.** Whether `axi_to_reg` in this SoC supports 4-byte-aligned register accesses (needed only if option 1 in gap location is chosen). Not determinable from supplied files.
9. **CLI/semantics exposure.** Whether the gap is a new ioctl or an added field gated by `flags`; the latter is blocked by `idma.c:712` rejecting nonzero flags, so a new ioctl is implied but not decided here.
10. **Rootfs/test entry reuse.** Whether P4M-1 should add a new userspace binary or reuse `idma_experiment` with `--gap`; the latter avoids changing `scripts/rootfs.py` (not supplied) and `tests/test_rootfs.py`.

### 5.3 Assumptions stated explicitly

- Assumed `1` cycle per state at `clk_i`, and that the observer's `issue_i[victim_q]` corresponds to the frontend's `obs_issue_o` (batch issue at `ready_i`, not at `valid_o`).
- Assumed gap applies to `batch_enable_q` only (warmup is non-batch, so no warmup gap by construction).
- Assumed "A window count" means completed *jobs* (1 per 4 KiB chunk), matching the plan's `A_completed_jobs_window` (`docs/e3-next-method-plan.md:25-27`), not AXI bytes.
- Assumed no IOMMU translation-core change is needed: all P4M-1 changes sit in frontend/observer/driver/CLI per `docs/e3-next-method-plan.md:49`.
- Unverified from supplied files: `scripts/rootfs.py` contents, `idma_reg64_frontend_reg_top` register map, `axi_to_reg` alignment behavior, and the actual arm/refresh timing budgets.

### 5.4 Suggested acceptance checks (short, for Codex to run)

1. `tests/test_idma_frontend_batch.py` (with gap 0) remains green and `tests/idma_frontend_observer_test.cpp` still passes the `unchanged_o` differential against `7f086b73…`.
2. A new frontend case proves first-copy immedate offer with gap N, gap applied exactly once per completed copy thereafter, and STOP-in-gap offers no additional task.
3. Host model: new `batch_gap_*` case(s) in `tests/idma_scheduler_harness.c` + `tests/test_idma_scheduler.py` show `issued == completed == target` and conservation of the new A counters, including a case where A is submitted before V.
4. Observer: a batch with nonzero gap still passes `idma_observation_finish` conservation (`idma.c:835-849`) and the "all V transactions inside the window" check (`idma.c:867-870`); REFILL-cycle counters grow monotonically with gap and no error bit is set.
5. Old `IDMA_IOC_RUN_EXPERIMENT`/`IDMA_IOC_RUN_BATCH` byte-for-byte behaviour unchanged (existing `CASES` set), with selective 128 OFF in the new P4M-1 path.