# PN passive SW/program quiet finite delivery

Actual component **0dbd526040326d3076a3689e469041429c7a530f** on independent
`ispass/e6-pn-sw-quiet`, parent **5b7dfbc70e458244a8e5682e5ae3f8571320254c**,
worktree `/home/xvhaocheng/work-e6-pn-sw-quiet-iommu`. Exactly ten source files
append defaultoff PnEnable/read-only pn_idle and necessary child connections.
No top/Bender/DS/TC/TL/frontend/observer/peripheral/Linux/constraint changes.
No Vivado or board operation occurred.

[metadata/component.json](metadata/component.json) locks the actual component,
source copies, minimal incremental bundle/patch, two required final runs and
local ELF identities. Only zero-day upstream exists: the component was
committed locally and **not pushed upstream**. The verified 5150-byte
[component bundle](metadata/iommu-sw-quiet.bundle) requires the already accepted
5b7 parent and contains the independent task branch. Root can merge the actual
commit from the shared Git DB, or restore the bundle; top integration remains
Root's separate task.

## Actual finite results

| Actual run | Scope | Cases | Cycles | Pn0/Pn1 raw whole-output comparisons |
| --- | --- | --- | --- | --- |
| [run06-target/manifest.json](runs/run06-target/manifest.json) | Complete program/SW, target BOTH/MSI_BT_MRIF/PC/debug/ATS0, original8/32/8/8, DS-ID4/program-ID6/user1 | 15 passed | 617 | 2498 |
| [run07-omitted/manifest.json](runs/run07-omitted/manifest.json) | Separate complete program/SW WSI_ONLY/debug0 smoke; other fields unchanged | 12 passed | 395 | 1604 |

Main C++ runtime checks:644438; omitted:413843. Each comparison evaluates all
original output bits against independent original85b gold, including invalid
payload cycles. Each sample additionally stores complete zero-extended old
output vectors in raw JSON, so the independent auditor compares actual bits,
not a hash or equality flag alone. Final source deletion-proof recovers the
original bytes of **all ten** modules after removing only explicit tap
additions: [functional-source-check01.json](functional-source-check01.json).

The main executable contains three independently stateful real hierarchies:
enabled candidate, defaultoff candidate and original85b gold. It uses the real
program bridge/five AXI buffers, complete SW wrapper, original register
map/fields/HPM/WSI and real CQ/fault-spill/arbiter/FIFO/FQ/PQ/MSI/debug modules.
Only gold module namespace and modified-child binding names change. The AXI
master, four DS memory responders, debug responder and fault sources are
bounded test environment, not substitutes for the SW state machines. Inputs
come through public ports/real program AXI CSR; no internal state is forced.
The original nine files plus regmap were independently verified byte-equal
between85b and5b7 before modifications.

Actual cases cover program AW-only, W-before-AW, held program B/R, 64bit/two
32bit CSR halves, original nonzero HPM state, CQ head==tail during DECODE,
CQ delayed B then stale B_RESP substate/WSI sticky IRQ, real fault spills/FIFO
and12 FQ records, unread memory ring/IRQ idle, CQ/FQ ERROR and original CSR
recovery, real GO/held debug/ONGOING/response, masked MSI pending across route
disable, sticky edged IRQ, and MSI B error→real fault arbiter→second real FQ
record. Both low/high DDTP deferred32bit writes return program B first and
stay nonquiet through the original pending commit edge.

The accepted ten-file implementation includes the [DDTP pending correction](ddtp-addendum.md)
and [known next-edge MSI source correction](irq-addendum.md). SW and program
idle are **separate domain outputs**, not frontend/system allquiet. The old
`in_flight` and register write protections are unchanged. Defaultoff outputs0;
enabled outputs0 in reset. Feature-absent MSI/debug child idle is the AND
identity1; final whole SW defaultoff remains0.

[raw-audit01.json](raw-audit01.json) independently passes19704 checks over both
final raw logs and before/after IRQ window: complete pre/post/final samples,
all old output bits, actual AR/R/AW/W/B sequence/ID/length/WLAST/debt, exact
12-record FQ and MSI-error chain counts, tested-source/Git-gold binding and
known-next-edge prediction. It finds the original cycle50 gap closing idle1→0
while all old outputs remain byte-identical. This is offline evidence replay,
with **0 new RTL runs and0 board points**.

## Preserved execution history

- `preparation-attempt01.json`: insertion preflight matched license text before
  module; failed before any source write.
- `runs/run01`: failed compile, missing original deprecated fifo_v2 dependency;
  dependency explicitly added, no replacement FIFO or production change.
- `runs/run02`: passed initial empty complete-hierarchy smoke only. It does not
  establish the required workload domain.
- `runs/run03`: failed extra WSI assertion after original-output gold agreed;
  original WSI vector loop overwrites coincident default vector0. Harness now
  configures separate0/1/2 vectors through real CSR; WSI was not modified. Its
  early case-pass marker precedes a later assertion; whole run remains failed.
- `runs/run04`:14 cases passed, final chain-count check failed because ending on
  instantaneous pre-IRQ idle was too early; no failure erased.
- `runs/run05-before-irq-fix`: actual original ten-file diagnostic completes
  MSI B1/FQ B2 and preserves the real one-cycle known-source quiet defect.
  Root accepted the passive prediction before final required runs06/07.

Every run snapshots its runner/SV/C++/all production and dependency bytes,
commands, tool version, source HEAD+dirty diff and raw logs. Execution HEAD
was5b7 plus the recorded dirty source; final component commit was created
**after testing**, and all ten committed blobs are byte-identical to both
final executed snapshots. Branch name/current HEAD is not substituted for
execution identity. Existing Verilator5.040 is reused; compiler/tool file
identities are in [metadata/toolchain.json](metadata/toolchain.json). All native
ELFs and obj trees remain local/ignored, with hashes in component.json.

## Limits and finite reproduction

Target ATS0 and its original PQ tie-offs remain. **Active PQ legal retirement
is not covered**: original PQ AWLEN3 contradicts its second-beat WLAST. Original
PQ repeated IRQ assignments are also untouched. No strict responder was
relaxed to pretend that path passed. Active ATS1, separate APB PREADY-wait
standalone, two-entry buffer capacity saturation, arbitrary reset interruption,
CIP/PMIP isolated MSI sources, actual TC/TL/DS integration and complete global
MODE eligibility are not claimed. Program R/B backpressure and DS delayed B
are real observed stimuli; DS R/B readiness backpressure is not artificially
forced or claimed. No physical mapping, fit/timing, PN/S15 or paper n.

The already accepted successful DUTs need no routine repeat. To inspect without
rerunning RTL, choose new receipt names:

```bash
python3 /home/xvhaocheng/work-e6-pn-sw-quiet/paper/ispass2027/exp/E6-pn/sw-quiet/audit_events.py \
  --root /home/xvhaocheng/work-e6-pn-sw-quiet/paper/ispass2027/exp/E6-pn/sw-quiet \
  --out /tmp/sw-quiet-raw-audit-new.json
python3 /home/xvhaocheng/work-e6-pn-sw-quiet/paper/ispass2027/exp/E6-pn/sw-quiet/verify_functional_source.py \
  --iommu-root /home/xvhaocheng/work-e6-pn-sw-quiet-iommu \
  --source-map /home/xvhaocheng/work-e6-pn-sw-quiet/paper/ispass2027/exp/E6-pn/sw-quiet/source-map-v2.json \
  --out /tmp/sw-quiet-functional-source-new.json
```

For a necessary new bounded finite run, use `run_sw_quiet.py` with
`--iommu-root --deps-root --cva6-root --verilator --out` exactly as recorded in
final manifest.command; out must be new. `--omit-features` chooses only the
explicit static smoke, and `--case msi-tail-probe` is the focused actual chain.
It includes the fixed source snapshots; no SD, JTAG, UART or FPGA command is
present. Hard bounds:compile900s/run60s/per-case20000/total200000cycles. Failures
and timeouts retain their new run directory and never retry automatically.

Original [nine-file proposal](plan.md) and original source-map/administrative
source verification stay unchanged. The accepted extension is recorded in
source-map-v2/addenda and the final component receipt. This report supplies
only the executed finite scope for Root's actual code/evidence review.
