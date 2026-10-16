## Summary

- The `COMPLETE` release path is: `rv_iommu_trans_ctl_wrap` `COMPLETE` → `axi_comp_req` → internal `axi_demux` with `SpillAr=1` → `comp_axi_req_o` → banked wrapper `bank_comp_req` → completion admission gate.
- The internal TC demux AR path has **two parallel spill instances** (`i_ar_spill_reg` for the AR header and `i_ar_sel_spill_reg` for the select). Each is a `spill_register_flushable` with A/B registers, so the pair can hold **two AR transactions**, each with header + select. It is not four independent AR slots.
- `COMPLETE` can therefore handshake with the internal demux and return to `IDLE` while the external completion AR is still not ready, provided the internal AR spill has room.
- The earlier “completed READ holds TC `COMPLETE`, preventing its own AW” hypothesis is not proven by the supplied source or logs. If the internal AR spill is full and downstream remains unready, `COMPLETE` can stall, but the supplied logs do not show that as the cause.
- The supplied `awgap4096-pn-hot.log` is the original failure: it fails at `nonblock_actual_dma_test.cpp:2233`, with `W_cycles=285765` and `AW-FIXTURE-TERMINAL-JSON {"stall_cycles":2628348}`. It does **not** print `E2-TERMINAL-JSON`, so `grants_future=0` is task context, not visible in that excerpt.
- The bounded fix below only changes the future-grant acceptance condition. It preserves all byte, fault, drain, conservation, metadata, and V/A completion checks.

---

## 1. Evidence trace: COMPLETE → internal AR spill → bank_comp_req → admission

### 1.1 TC COMPLETE and completion AR issue

`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`

- State enum includes `COMPLETE`: lines 96–102.
- `in_flight_o = (state_q != IDLE)`: line 105.
- Completion AR request fields are assembled from the original `ar_data` and translated `spaddr`:
  - `axi_comp_req.ar.id = ar_data.id`: line 346.
  - `axi_comp_req.ar.addr = addr_t'(trans_resp_data_q.spaddr)`: line 347.
  - `len/size/burst/lock/cache/prot/qos/region/user` from `ar_data`: lines 348–356.
  - `axi_comp_req.ar_valid = complete_read`: line 357.
- `COMPLETE` issues the AR and returns to `IDLE` only when the downstream demux slave-side AR handshake is accepted:
  - `complete_read = is_read`: line 718.
  - `if ((axi_comp_resp.ar_ready && is_read) || ...)`: lines 721–722.
  - `ar_ready = is_read; state_n = IDLE;`: lines 724–725.

So the TC leaves `COMPLETE` on `axi_comp_resp.ar_ready`, which is the internal demux slave-side ready, not necessarily the external completion AR ready.

### 1.2 The internal TC demux has `SpillAr=1`

With `MSITrans = MSI_BT_ONLY` (`tests/nonblock_banked_top.sv:374`), the active branch in the TC wrapper is `gen_mrif_support_disabled`:

`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`

- `gen_mrif_support_disabled`: lines 522–579.
- Internal `axi_demux`:
  - `NoMstPorts = 2`: line 551.
  - `AxiLookBits = RVIOMMUCfg.AxiIdWidth`: line 552.
  - `SpillAw = 1`, `SpillW = 1`, `SpillB = 1`, `SpillAr = 1`, `SpillR = 1`: lines 553–557.
  - `slv_req_i = axi_comp_req`, `slv_resp_o = axi_comp_resp`: lines 564–565.
  - `mst_reqs_o = {comp_axi_req_o, axi_err_req}`: line 567.
  - `mst_resps_i = {comp_axi_resp_i, axi_err_resp}`: line 568.

This is the demux that sees the TC’s `axi_comp_req` and can absorb ARs into spill before they reach `comp_axi_req_o`.

### 1.3 AR spill capacity: TWO parallel header+select entries, not four AR slots

`cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux.sv`

- Header AR spill:
  - `i_ar_spill_reg` for `ar_chan_t`, `Bypass(~SpillAr)`: lines 132–144.
- Select spill:
  - `i_ar_sel_spill_reg` for `select_t`, `Bypass(~SpillAr)`: lines 145–157.
- Slave-side AR handshake combines both:
  - `slv_resp_o.ar_ready = slv_ar_ready_chan & slv_ar_ready_sel;`: line 159.
  - `slv_req_cut.ar_valid = slv_ar_valid_chan & slv_ar_valid_sel;`: line 160.
- Since `SpillAr=1`, `Bypass=0`.

`cva6/corev_apu/riscv-iommu/vendor/common_cells/src/spill_register.sv`

- Wrapper passes through to `spill_register_flushable`: lines 31–44.

`cva6/corev_apu/riscv-iommu/vendor/common_cells/src/spill_register_flushable.sv`

- A register: lines 38–55.
- B register: lines 57–74.
- `ready_o = !a_full_q || !b_full_q`: line 90.
- `valid_o = a_full_q | b_full_q`: line 93.
- `data_o = b_full_q ? b_data_q : a_data_q`: line 96.

Therefore each of the two parallel spills can hold two entries, but they are paired by the same AR handshake. The AR path can hold **two AR transactions total**, each occupying one header entry and one select entry. Raw storage cells are 2 header + 2 select, but not four independent ARs.

### 1.4 Selected vs queued read semantics

`cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv`

- Internal AR control:
  - `lock_ar_valid_q` holds an arbitration decision: lines 311–319.
  - AR can start only when `!ar_id_cnt_full`: lines 322–338.
  - `ar_push` pushes the AR ID counter: line 331, and counter block lines 355–373.
- Master-side AR ready:
  - `assign ar_ready = ar_valid & mst_resps_i[slv_ar_select_i].ar_ready;`: line 410.
- Master-side AR request:
  - `mst_reqs_o[i].ar = slv_req_i.ar;`: line 442.
  - `mst_reqs_o[i].ar_valid = 1'b1` when `ar_valid && slv_ar_select_i == i`: lines 444–445.

Semantics:

- **Queued read**: accepted into `i_ar_spill_reg`/`i_ar_sel_spill_reg`, not yet presented to the chosen master.
- **Selected read**: `axi_demux_simple` has asserted `ar_valid` for a particular master and, when the master is ready, completes the master-side AR handshake.
- The TC only sees the slave-side handshake. It can leave `COMPLETE` while the AR is queued in spill.

### 1.5 `bank_comp_req` and completion admission

`tests/nonblock_completion_banked_ctl.sv`

- Per-bank completion arrays:
  - `axi_comp_req_t [1:0] bank_comp_req;`
  - `axi_comp_resp_t [1:0] bank_comp_resp;`
  - lines 226–227.
- `i_bank` connections:
  - `.comp_axi_resp_i(bank_comp_resp[b])`
  - `.comp_axi_req_o(bank_comp_req[b])`
  - lines 352–353.
- Admission gate when `CompletionGateEnable`:
  - `admitted_comp_req[b].ar_valid = bank_comp_req[b].ar_valid && completion_ar_admit_i[b];`
  - `bank_comp_resp[b].ar_ready = admitted_comp_resp[b].ar_ready && completion_ar_admit_i[b];`
  - lines 425–429.

`tests/nonblock_actual_dma_top.sv`

- `late_comp_ar_admit` is wired from the late pool: lines 584, 823–826.
- `CompletionGateEnable(SharedPoolMode >= 4)`: line 700.

So for late-pool modes, the external completion admission can hold `bank_comp_resp[b].ar_ready` low. But the TC’s internal demux can still accept the AR into its two-entry spill, releasing TC `COMPLETE`.

### 1.6 Original response fields/tags preserved

- TC completion AR assembly preserves original tags:
  - `id`: `rv_iommu_trans_ctl_wrap.sv:346`.
  - `addr`: translated `spaddr`: line 347.
  - `len/size/burst/lock/cache/prot/qos/region/user`: lines 348–356.
- Internal demux copies the whole AR header:
  - `mst_reqs_o[i].ar = slv_req_i.ar;`: `axi_demux_simple.sv:442`.
  - select is carried separately through `i_ar_sel_spill_reg` and used for routing: `axi_demux.sv:145–157`, `axi_demux_simple.sv:444–445`.
- Banked controller outbound conversion preserves AR fields field-by-field:
  - `comp_axi_req_o.ar.id = mux_mst_req.ar.id[...]`: `nonblock_completion_banked_ctl.sv:501`.
  - `addr/len/size/burst/lock/cache/prot/qos/region/user`: lines 502–511.
- B/R return path reconstructs the bank bit from the original ID bit:
  - `comp_axi_resp_i.b.id[BankIdBit]`: line 529.
  - `comp_axi_resp_i.r.id[BankIdBit]`: line 534.
- Test-only late pool checks the physical AR fields against the captured metadata:
  - captures `completion_ar_id_i/addr/len/size` on grant: `nonblock_completion_pool_late.sv:382–385`.
  - checks bank bits and metadata match: lines 386–389.
  - while armed, requires identical `id/addr/len/size`: lines 391–395.

### 1.7 Corrections to earlier assumptions

- **Earlier AW-reversal hypothesis**: The claim that a completed READ holds bank TC `COMPLETE` and prevents its own needed AW is not proven. `COMPLETE` can release into the internal AR spill (`SpillAr=1`), with up to two queued AR transactions. The source does not prove the reverse either: if both spill entries are occupied and downstream remains unready, `COMPLETE` can stall. The supplied logs do not show internal spill state, so neither direction is proven by log.
- **“No read parking” assumption**: The input bank demux has `SpillAr=0` (`nonblock_completion_banked_ctl.sv:302`), so it does not park reads in an AR spill. But the wrapper’s read-admission logic can delay selecting new non-owner reads while a W owner is unfilled:
  - `restrict_reads`, `read_owner`, `bank_read_admit[0/1]`: `nonblock_completion_banked_ctl.sv:251–273`.
  - This holds off `read_admit_i`, not AXI `VALID`.
  - The TC internal demux is separate and does park completed translation ARs with `SpillAr=1`. So a blanket “no read parking” statement is incorrect.
- **Input demux same-bank W locking matters**: The input `axi_demux` has `SpillAw=0`, `SpillW=0`, `SpillB=0`, `SpillAr=0`, `SpillR=0` (`nonblock_completion_banked_ctl.sv:299–303`). In `axi_demux_simple`, a new AW is allowed only when `(w_open == 0) || (w_select == slv_aw_select_i)` (`axi_demux_simple.sv:168–176`). W routing follows `w_select`/`w_open` (`axi_demux_simple.sv:232–253`, `432–435`). Thus a new AW for the other bank waits while a same-bank W burst is open. This is relevant to AW scheduling and the fixture stall, but it is a separate mechanism from the TC internal AR spill.

### 1.8 What the actual logs do and do not prove

`out/write-order-boundary-20261007/awgap4096-pn-hot.log`

- Line 1: `SCENARIO-FAIL pn: E2 future-stream grant count inconsistent with mode (tests/nonblock_actual_dma_test.cpp:2233)`.
- Line 12: `W_cycles=285765`, window `v_completed=64`, `a_completed=64`, `ddr_aw=257`, `ddr_w=65792`, `ddr_b=257`, `comp_aw_fire=257`, `comp_w_fire=65792`.
- Line 18: `AW-FIXTURE-TERMINAL-JSON {"stall_cycles":2628348}`.
- No `E2-TERMINAL-JSON`, no `PASS`.

Proves:
- The original failure is exactly at the future-stream grant acceptance check.
- All earlier checks in `run_scenario()` reached line 2233; the run was not blocked earlier by fault, drain, byte, or metadata checks.
- The AW fixture did actually stall (`2628348` cycles).
- The run had many AW/W/B/R completions, so this is not a global no-progress deadlock.

Does not prove:
- The `grants_future` value: `E2-TERMINAL-JSON` is printed after the failing check, so the supplied excerpt does not show `grants_future=0`. That value is task context.
- The internal AR spill was occupied, or that spill capacity was the cause of the acceptance mismatch.
- That the earlier AW-reversal hypothesis occurred.
- That the patched run passes; I cannot execute tests.

`out/write-order-boundary-20261007/awgap-pn-hot.log`

- Lines 17–19: `AW-FIXTURE-TERMINAL-JSON {"stall_cycles":39804}`, `E2-TERMINAL-JSON ... "grants_future":638 ... "received_beats":328704 ... "replayed_beats":328704 ... "meta_accepted":1284 ... "meta_early":642 ... "meta_highwater":4 ... "meta_queued":0`, `PASS`.
- Shows nonzero fixture stall does not by itself force `grants_future=0`. The delayed-AW case is the one where the task context says `future=0`.

---

## 2. Smallest complete C++ acceptance diff

File: `tests/nonblock_actual_dma_test.cpp`

```diff
--- a/tests/nonblock_actual_dma_test.cpp
+++ b/tests/nonblock_actual_dma_test.cpp
@@ -2186,10 +2186,11 @@
 #endif
 
-        std::printf("AW-FIXTURE-TERMINAL-JSON {\"stall_cycles\":%u}\n",
-                    (unsigned)top->dbg_e2_aw_fixture_stall);
+        const uint32_t e2_aw_fixture_stall = (uint32_t)top->dbg_e2_aw_fixture_stall;
+        std::printf("AW-FIXTURE-TERMINAL-JSON {\"stall_cycles\":%u}\n",
+                    (unsigned)e2_aw_fixture_stall);
 
         const uint32_t e2_grant_future = (uint32_t)top->dbg_e2_grant_future;
         const uint32_t e2_meta_accepted  = (uint32_t)top->dbg_e2_meta_accepted;
@@ -2230,8 +2231,15 @@
-        CHECK(e2_mode >= 6 ? e2_grant_future > 0 : e2_grant_future == 0,
-              "E2 future-stream grant count inconsistent with mode");
+        if (e2_mode >= 6) {
+            // Legal delayed-AW fixture: the fixture may remove the future
+            // grant opportunity by backpressuring the second AW.  Require a
+            // future grant only when the fixture did not actually stall.
+            CHECK(e2_aw_fixture_stall > 0 || e2_grant_future > 0,
+                  "E2 future-stream grant count inconsistent with mode");
+        } else {
+            CHECK(e2_grant_future == 0,
+                  "E2 future-stream grant count inconsistent with mode");
+        }
```

### Exact acceptance semantics after patch

- `e2_mode >= 6` and `dbg_e2_aw_fixture_stall == 0`: still require `e2_grant_future > 0`. Original unperturbed failure retained.
- `e2_mode >= 6` and `dbg_e2_aw_fixture_stall > 0`: permit `e2_grant_future == 0`; also permit `> 0`.
- `e2_mode < 6`: still require `e2_grant_future == 0`.
- No other checks are touched:
  - `dbg_e2_fault == 0`: preserved at `nonblock_actual_dma_test.cpp:630–631`.
  - metadata queue bound: preserved at line 641.
  - drain/quiet: preserved at `check_quiet()` lines 1312–1348 and call at 2008.
  - byte verification: preserved at lines 2011–2041.
  - E2 occupancy/reserved/busy/received/replayed and metadata bounds: preserved at lines 2198–2231.
  - V/A completion and window checks: preserved in `report_window()` and terminal checks.

### Tests to validate the fix

1. **Original delayed-AW failure case**
   - Use the same failing configuration that produced `out/write-order-boundary-20261007/awgap4096-pn-hot.log` (`InterleaveAwEnable=1`, delay 4096, `+mode=pn`, `+v-gap=0`, `+transfers=64`, `+warmup=256`, `+ptw-latency=31`, `+a-pages=8`, `+a-target=1048576`, `+read-admit=buffer`, `+read-budget=256`, `+read-period=256`).
   - Expected after patch: no `SCENARIO-FAIL` at line 2233; `AW-FIXTURE-TERMINAL-JSON` shows `stall_cycles > 0`; `E2-TERMINAL-JSON` prints with `grants_future=0`; exit 0 and `PASS` if no other check fails.
   - I cannot claim this passes; it must be run.

2. **Existing passing delayed-AW case**
   - `out/write-order-boundary-20261007/awgap-pn-hot.log`: `stall_cycles=39804`, `grants_future=638`.
   - Expected: still `PASS`; the check accepts because `stall_cycles > 0` or `grants_future > 0`.

3. **Unperturbed fixture**
   - `InterleaveAwEnable=0` or equivalent so `dbg_e2_aw_fixture_stall == 0`, `e2_mode >= 6`, `grants_future == 0`.
   - Expected: still fails at the same check. This retains the original acceptance failure.

4. **Modes < 6**
   - Any `e2_mode < 6` with `grants_future != 0` must still fail; `grants_future == 0` must pass this check.

5. **Full invariant set**
   - Confirm `fault==0`, byte-for-byte copies, quiet/drain, `received == replayed`, metadata bounds, occupancy/reserved/busy zero, and V/A completion counts remain unchanged.

---

## 3. Optional evidence taps, no production RTL edits

Existing taps are likely sufficient to distinguish TC stalled from AR queued:

`tests/nonblock_actual_dma_top.sv`

- `dbg_e2_aw_fixture_stall`: output line 204; assigned 0 for modes 0 and <4 at lines 496 and 504; late pool connection at line 590.
- `dbg_e2_tc_read_wait`: lines 994–996. This taps `bank_comp_req[b].ar_valid && !bank_comp_resp[b].ar_ready`.
- `dbg_e2_tc_state`: lines 999–1000. This taps `i_bank.state_q` for each bank.

Interpretation:
- `tc_state == COMPLETE` and `tc_read_wait == 1`: TC is still waiting for its internal demux slave-side AR ready. This can happen if the internal AR spill is full or the internal demux AR ID counter is full.
- `tc_state == IDLE` and `tc_read_wait == 1`: TC has released `COMPLETE`; the AR is already past the internal demux and is waiting for `bank_comp_resp.ar_ready`/completion admission. This is the spill-absorbed case.

If more direct observation is needed, add **test-only** hierarchical taps in `tests/nonblock_actual_dma_top.sv` only, e.g.:

```systemverilog
// Optional test-only taps; do not edit production RTL.
output logic [1:0] dbg_e2_tc_comp_ar_spill_full,
output logic [1:0] dbg_e2_tc_comp_ar_spill_valid,

for (genvar b = 0; b < 2; b++) begin : gen_comp_ar_spill_tap
    assign dbg_e2_tc_comp_ar_spill_full[b] =
        i_sys.gen_banked.i_banked.gen_bank[b].i_bank
            .gen_mrif_support_disabled.i_iommu_axi_demux_no_mrif
            .i_ar_spill_reg.a_full_q |
        i_sys.gen_banked.i_banked.gen_bank[b].i_bank
            .gen_mrif_support_disabled.i_iommu_axi_demux_no_mrif
            .i_ar_spill_reg.b_full_q;
    assign dbg_e2_tc_comp_ar_spill_valid[b] =
        i_sys.gen_banked.i_banked.gen_bank[b].i_bank
            .gen_mrif_support_disabled.i_iommu_axi_demux_no_mrif
            .i_ar_spill_reg.valid_o;
end
```

The exact hierarchical path depends on generate-block elaboration; use only in the test wrapper, never in production RTL.

---

## Limitations / not proven

- I cannot execute tests or edit files. The patch is a proposed diff only.
- The supplied `awgap4096-pn-hot.log` is the original failure, not a PASS. It does not print `grants_future`; the `future=0` value is task context.
- The source explains how the TC can release into the internal AR spill, but the supplied logs do not prove that spill occupancy caused the delayed-AW result.
- No deadlock is confirmed. The logs show forward progress; the failure is an acceptance-check mismatch after the main scenario checks.
- The earlier AW-reversal hypothesis is not proven. The source shows a finite two-entry internal AR spill; if it fills, `COMPLETE` can stall, but no supplied log demonstrates that case.
- The two parallel header/select spills share handshakes in the supplied code, but the supplied source does not include an assertion proving they cannot skew under unusual reset/flush conditions.
- No Q2 novelty claim and no new deadlock claim are made.
