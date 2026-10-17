# E3-lite: finite-validated passive timestamp storage candidate

**Source and finite real-RTL checks passed; no Vivado invocation, RAM
inference result, fit/timing claim or board qualification.** PN's complete
build has priority. The original full-E3 failure remains stopped and intact;
this is a new source candidate, not a route retry.

Root-approved scope is one actual CVA6 file:
`corev_apu/fpga/src/dma_path_observer.sv`. Depth64 combines timestamp and born
flag in a 64-word memory (33 bits with the production 32-bit timestamp),
with an independent posedge write and asynchronous head read. Write enable
is exactly `rst_ni && !clear_i && run_i && !bad && enter_i`. The original
Depth4 writes remain in their original reset/clear/run process under the
constant legacy branch. Queue metadata, statistics, histograms, sticky error
semantics, BTC3, OBS5/INFO2, all interfaces, top generics and constraints
retain their previous behavior. Sticky errors do not newly gate legal writes.

## Actual source and recovery

| Repository | Parent | New local commit, branch `ispass/e3-lite` |
| --- | --- | --- |
| CVA6 | `1d587125eee690e623e6ffae13e70f9bbe989adc` | `0d0ff6b3794853c57ed241ca5303e434995e8167` |
| platform | `dfebcc0425bab25567e8514d156650a608530be1` | `17b11c57ff5042b1d3e63361e88df25fc7fea647` |

Actual worktrees are `/home/xvhaocheng/work-e3-lite-cva6` and
`/home/xvhaocheng/work-e3-lite-platform`; neither changes the old branches or
build inputs. The platform change only updates its CVA6 gitlink and lock.
Linux `da6b69fc007ad5e671dc288dd8038d02e3ebe43d` remains **reference_only**;
this package builds no kernel or payload. The unchanged IOMMU child is
`85b157ebe71a623e326e51fff7b7126fe77e8eb9`.

Candidate source SHA256:
`bd5963cd38565c9be78073f81b62488027f0596cd5e0f6983135c1afdc57e12b`.
The source snapshot, exact component topology, changed files, minimal
prerequisite-bound bundles, patches and original bundle verification logs
are in [metadata/components.json](metadata/components.json). CVA6 only has
the upstream zero-day remote and platform has none: these are actual local
component commits with recovery bundles, not claimed component remote pushes.
No new remote or upstream head was created. The top-level task branch is
independent from `main=8f4a2459fa4005313b98ca3e2cb66dfbb9ccda58`.
The narrow attributes for raw format-patch/bundle logs preserve their original
Git-generated whitespace; production source and test code use normal checks.

All three executed run snapshots match the final committed candidate bytes.
Execution itself was at parent1d587 plus the recorded one-file dirty diff;
the final component commit was created afterward. The manifests distinguish
these identities. The [initial plan](plans/e3-lite-readonly-design.md) and
[source map](plans/e3-lite-source-map.md) were first committed separately as
top-level `6ac77f5` before any candidate change or new worktree creation.

## Actual finite evidence

[run02](runs/run02/manifest.json) covers Depth64/HistEnable1;
[run03](runs/run03/manifest.json) covers Depth64/HistEnable0 and
Depth4/HistEnable0/1. Each configuration ran three independent initial-memory
seeds (1,17,271828), using **actual candidate and exact1d587 RTL modules
renamed only with a gold suffix**, under Verilator4.038. Each pair initially
has a different value in every stale memory word. Before and after every
driven clock the harness compares:

- All eight statistics, 32 histogram bins, error/overflow, head/tail/count
  for production-width and narrow-counter test instances.
- All 16 actual PATH stage statistics and bins, reduction errors, timer/run
  state and selected readout. PATH's timer is narrowed to8 only for finite
  wrap testing; production-width direct stage timestamps cover all32 bits.
- Every candidate and gold memory word against the exact write-enable,
  tail address, timestamp and active flag, including disabled/error edges.

There are **144 scenario executions / 157,872 cycles / 315,744 paired
before/after comparisons**, across four configurations and three seeds.
The [independent raw-log/source audit](finite-audit01.json) passed137 checks,
including an arithmetic reconstruction that would catch reading the newly
written full-queue head instead of the old head. All three initial-memory
seeds produce the same observable digest within each configuration.

The 12 directed groups cover empty enter+leave; full simultaneous pop+push;
multiple pointer wraps; born/carry and open/close boundaries; run0; queue
overflow/unmatched return followed by legal writes with sticky errors;
reset/clear priority with stale words; all timestamp bit boundaries and
wrap; real narrow histogram/counter/sum/area overflow followed by legal
writes; actual PATH abort/close/timer behavior; and12,000 randomized cycles.
The production RTL is identical for narrow test instances apart from their
explicit width parameters. This is finite two-state simulation, not formal
equivalence or a full backend/AXI system test.

[run01](runs/run01/manifest.json) is the preserved failed test-wrapper
compilation: the instance name `small` was an SV strength keyword. Only the
test wrapper name was corrected. Its candidate production source bytes are
identical to run02/run03; no failed test was overwritten or concealed.

To replay into a **new output**, with no Vivado or board access:

```sh
python3 /home/xvhaocheng/work-e3-lite/paper/ispass2027/exp/E3-lite/verify_storage.py \
  --cva6 /home/xvhaocheng/work-e3-lite-cva6 \
  --output /home/xvhaocheng/work-e3-lite/paper/ispass2027/exp/E3-lite/runs/root-new01
python3 /home/xvhaocheng/work-e3-lite/paper/ispass2027/exp/E3-lite/audit_storage.py \
  --runs /home/xvhaocheng/work-e3-lite/paper/ispass2027/exp/E3-lite/runs/root-new01 \
  --output /home/xvhaocheng/work-e3-lite/paper/ispass2027/exp/E3-lite/root-audit-new01.json
```

The runner snapshots all compiled RTL/harness/runner bytes and fails on
existing output paths. Native generated C++ objects and model executables
remain local under ignored `runs/*/obj-*`; their actual compile commands,
logs and per-configuration binary SHAs are in each original manifest.

## Remaining boundary

The source carries `ram_style="distributed"`, but inference is untested.
The existing arrays were already unreset; the candidate isolates their write
process to permit a different synthesis mapping. No saving of the33,792
logical storage bits is asserted. Physical read-during-write behavior and
actual RAM primitives require later synthesis/netlist validation. All
histograms remain and their routing can still dominate. No fit or50MHz
timing improvement follows from this finite pass.

After Root's source acceptance, PN must complete its build first. Only a
separately authorized inference/full implementation may follow. It must keep
the original constraints/strategy and all setup/hold/pulse/routing/DRC gates,
bind the new source/platform/BIT tuple honestly, and stop on failure. This
package provides neither a BIT nor permission to load one.
