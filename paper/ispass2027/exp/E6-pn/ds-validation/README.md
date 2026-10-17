# PN DS transport quiet — finite component batch

Accepted implementation scope requested by Root: passive DS protection only. Component **`9713cd0cee4ba23fa37041d9bf48e2d58ece74af`**, directly after **`5b7dfbc70e458244a8e5682e5ae3f8571320254c`**, changes only `Bender.yml`, `rtl/ext_interfaces/rv_iommu_ds_if.sv`, and new `rtl/ext_interfaces/rv_iommu_ds_debt.sv`. No top, TC, vendor, SW, MSIPTW/PTW, constraints, FPGA build, or board operation. Execution was Codex-only. Native runtime model/context metadata was not available; no manual model/window/cache change was made.

## Interface and semantics

`rv_iommu_ds_if.PnEnable` is appended, default **0**. Both new outputs are read-only:

- `pn_buffers_quiet_o`: existing W-owner FIFO empty; all five spill output VALID flags, current local arbiter/mux work, all nine producer AR/AW/W offers (including unused channels), external AR/AW/W offers and R/B offers absent. READY and stale payload do not mean work. Non-bypass spill output VALID already covers both resident entries; no shadow inventory is added.
- `pn_transport_quiet_o`: buffers quiet **and** read/write debts both zero **and** sticky bad clear. It is **not** complete IOMMU/domain quiet. Producer lifecycle state, TC/vendor/SW state and caller private DMA work remain later batches.

Only the enabled generate instantiates `rv_iommu_ds_debt`. It has exactly 8 read-state bits, 8 write-state bits and 1 shared bad bit, reset asynchronously by existing `rst_ni` only. The external DS boundary events are:

| Debt | Increment | Decrement |
|---|---|---|
| read | `ds_req_o.ar_valid && ds_resp_i.ar_ready` | `ds_resp_i.r_valid && ds_req_o.r_ready && ds_resp_i.r.last` |
| write | `ds_req_o.aw_valid && ds_resp_i.aw_ready` | `ds_resp_i.b_valid && ds_req_o.b_ready` |

For each counter, events `00`/`11` hold, including net `11` at 0/255. Increment at 255 holds saturated and sets bad; decrement at zero holds zero and sets bad. Bad never clears by drain, ARM, STOP or policy clear (none of those are ports). Once bad, transport quiet remains false until reset. Counter range is not a credit/capacity claim. No tracker output affects any original READY, VALID, payload, arbiter choice or FIFO control. Default-off outputs are conservative zero and the tracker instance/state is absent from actual elaboration.

## Actual finite evidence

Final immutable execution is [`evidence/ds04/manifest.json`](evidence/ds04/manifest.json); the source snapshot/commands and original paths remain exactly recorded, even though archived evidence is copied below this directory. Actual candidate inputs were dirty bytes on parent `5b7dfbc`; [`summary.json`](summary.json) binds their exact SHA/bytes to the subsequently created component commit `9713cd0`. No execution identity is rewritten to the later commit.

`ds_top.sv` instantiates actual candidate DS with `PnEnable=1`, actual candidate DS default-off, and raw original DS from `5b7dfbc` (only its module declaration renamed). It compares the complete external request and all nine complete producer response structs at every evaluation, including invalid-channel payload. The frozen vendor FIFO/spill/arbitration modules are actual baseline dependencies. No formula replaces DS or MSIPTW.

Final run: **20 named scenarios, 9,612 clocks, 33,729 evaluations, 241,912 C++ assertions**. Observed external handshakes: AR740, AW792, W1588, RLAST740, B793; B includes one deliberate unsolicited-response underflow. Scenarios include independent unit read/write boundaries, net simultaneous events at 0/255, reset-only sticky errors, all current producer offers, W-before-AW, owner ordering/nonlast/WLAST, multi-owner W spill backpressure, AR held before external acceptance, R/B spill held after external debt retirement, bounded concurrent legal traffic, and explicit 256-transaction read/write overflow negative tests. Exact old-path comparisons continue after bad.

Actual unmodified `rv_iommu_msiptw` uses `MSI_BT_MRIF`, issues ID4/ARLEN1/two 64-bit beats, and fills the IOTLB on valid first Basic-Translate PTE. After that first beat, the real BT and MRIF FSM taps are idle and current offers/spills become empty. RLAST is deliberately delayed **24 clocks**; read debt stays1 and transport quiet0 throughout that interval (raw trace has 27 observed idle/no-VALID gap cycle numbers including settling edges). External RLAST acceptance reduces debt to0, but real DS response spill still keeps quiet0; quiet becomes true after the tail reaches the always-ready real walker. Its production source is unchanged, and internal state reads exist only in this test wrapper. This proves the specific tail gap; it does not claim all MSI/MRIF functionality, full-system quiet or real DMA certification.

[`check_evidence.py`](check_evidence.py) verifies all archived input/output hashes, exact execution/commit binding, exact namespace-only gold transformation, unchanged original DS dataflow source, unchanged MSIPTW, actual elaboration with enabled/default-off instances, and independently replays both debt trackers from raw clock/handshake events. It also reconstructs buffer/transport quiet and checks the real BT first PTE, tail gap and held tail response. **168,906 checks pass**, board cases0. `elaboration.xml.gz` proves default-off has zero debt instances and enabled module state is 8+8+1. This is elaboration evidence, not physical area/timing.

## Preserved attempts and corrections

All original manifests, source snapshots, logs and traces are preserved without overwrite:

1. `ds01`: build failed because test wrapper used incorrect AXI typedef channel names and omitted an enum config literal. The compiler diagnostics are retained. Production RTL unchanged.
2. `ds02`: random source initially admitted work without a finite outstanding bound, accumulating more than255 reads and failing its fixed drain budget. New sticky bad correctly set; all candidate/default-off vs original comparisons passed up to the testbench failure. Test admission was bounded, and distinct explicit read/write overflow negative scenarios were added rather than hiding overflow. Production RTL unchanged.
3. `ds03`: first passed corrected finite run, 241,905 checks.
4. `ds04`: same production bytes; expanded raw event/data fields, actual XML specialization and explicit real tail response-spill edge assertion. Final241,912 checks.

The first bundle command using only a SHA range yielded “Refusing to create empty bundle”; [`../component-source/ds01/bundle-attempt01.txt`](../component-source/ds01/bundle-attempt01.txt) records the packaging correction to a named branch ref. Git history/source was unchanged. The final incremental bundle verifies with prerequisite `5b7dfbc` and retains actual component history; the full patch and three source files accompany it.

## Recheck and reproduce

Read-only evidence replay (no simulation):

```sh
python3 /home/xvhaocheng/work-e6-pn/paper/ispass2027/exp/E6-pn/ds-validation/check_evidence.py --iommu-root /home/xvhaocheng/work-e6-pn-iommu
```

Only if a new finite run is needed, choose a **new nonexistent** output directory. The runner records source bytes and commands before execution and refuses an old output; build budget180s, each run/XML budget30s, jobs2:

```sh
python3 /home/xvhaocheng/work-e6-pn/paper/ispass2027/exp/E6-pn/ds-validation/run_ds.py \
  --iommu-root /home/xvhaocheng/work-e6-pn-iommu \
  --deps-root /home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6/corev_apu/riscv-iommu \
  --verilator /home/xvhaocheng/work/iommu-platform/out/stage117-real-cache-handoff-20261013/implementation/toolchain/verilator-5.040/bin/verilator \
  --out /home/xvhaocheng/work-e6-pn/paper/ispass2027/exp/E6-pn/artifacts/ds-NEW
```

Incremental component restoration: verify/fetch `../component-source/ds01/iommu.bundle` with prerequisite5b7, then integrate the exact9713 commit into Root's component branch. This batch does not update parent gitlinks or CVA source lists. Those remain part of later accepted integration. No full build is authorized by these finite results.
