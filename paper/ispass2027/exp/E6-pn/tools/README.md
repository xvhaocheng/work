# Pure host S15 gate

`s15_gate.py` reads an already saved S15 campaign. It has no UART/JTAG/SD,
collector, control-helper, build or DS50 execution entry. It does not create a
campaign, supply missing points, rewrite the source status or modify input files.
No real E6-PN campaign has been analyzed by A3.

```text
python3 s15_gate.py --run SAVED_S15 --source-root ROOT_WORKTREE \
  --fpga-manifest ROOT_LOCKED_PASSED_NEW_DS0_BUILD_MANIFEST \
  --source-manifest ROOT_LOCKED_ACTUAL_SOURCE_MANIFEST \
  --expected-cva6 FULL_ROOT_APPROVED_CVA6_COMMIT \
  --expected-iommu FULL_ROOT_APPROVED_IOMMU_COMMIT \
  --output NEW_E6_LOCAL_GATE_DIRECTORY
```

All paths read must be inside the explicit source root or saved run. Historical
`/home/xvhaocheng/work` and `work-root-a` references are explicitly rebased onto
that root without changing original bytes. Output must be a new E6-local directory
outside the original run. BIT/ELF bytes are not rehashed. Root supplies the actual
new DS0 manifest and full source refs when they exist; there is no candidate
commit whitelist or fabricated passed package in this tool.

The original software manifest bytes are fixed by the accepted source map:
Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`, release
`6.5.0-rc2-ispass2027-pathv3`. `f91112f6...` is the SHA256 of
**CLI source** `bench/idma_experiment.c`, selected from platform `a2e847...`;
it is not an ELF hash. The original ELF/payload remain associated by that
unchanged passed software build manifest. The new FPGA source/bit identity must
agree with the explicitly locked manifests and actual load receipt.

## Saved input contract

Use the existing campaign manifest/records/raw-command pattern. Future real
S15 collection must save its actual entry source and the following fields; this
batch does not provide or invoke that collector:

| Manifest field | Required value or evidence |
| --- | --- |
| `phase`, `status`, `recorded_cases`, `ds` | `e6-pn-s15`, `passed`, `15`, `0/0` |
| `planned_cases` | `[[0,"Solo"],[0,"A1"],[0,"A8"],...,[4,"Solo"],[4,"A1"],[4,"A8"]]`, fixed order |
| `control_actions` | Explicit `[]`: no extra MODE/GAP/flush/reset configuration actions |
| `script_source`, `script_sha256` | Saved actual as-executed S15 entry file; its exact bytes must match the recorded SHA |
| `kernel_release`, `bit_sha256` | Original release; unique `.bit` hash from actual new DS0 build manifest |
| `load_manifest`, `load_manifest_sha256` | Original verified-load receipt bytes, actual boot/bit/release and existing `program_file_sha256_verified` result |
| `software_manifest` | Exact original passed software build manifest bytes |
| `fpga_manifest`, `fpga_manifest_sha256` | Same file explicitly supplied by Root and its original byte hash |
| `source_manifest`, `source_manifest_sha256` | Same source file explicitly supplied by Root; hash also matches build manifest |
| `pre_readback`, `post_readback` | Two distinct saved raw command JSON files for actual configuration-readonly observations |

The existing FPGA build manifest fields are `status=passed`, `action=fpga-ds0`,
`variant=ds0`, `selected_cva6`, `selected_iommu`, `source_manifest`, its SHA, and
`artifacts` mapping the actual `.bit` path to its already recorded SHA. Source
manifest fields are `selected_cva6`, `iommu`, `linux`, and actual recorded
`reset_mode=0`, `v_gap_reset_cycles=0`, `pn_cap`, `gap_cap` (nonzero uint64 values).
The last four describe the actual built source/reset contract; do not populate
them from this README, a source comment or candidate constants before real source
and reset evidence exist. A1's builder will retain the actual values. Hardware
pre/post readbacks must independently match them. Status bits194/199 are not
interpreted or invented by this batch.

`control_actions=[]` does **not** mean zero writes on the bus. A configuration-read
of the PN bank writes INDEX to select words and then reads DATA; those selector
writes are allowed and are not MODE/GAP/cache/reset configuration actions. The
unchanged old CLI's own ARM/mapping/batch MMIO writes also are not extra control
actions. Readbacks are stored separately and have this exact saved command:

```text
printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\([0-9]*\).*/\1/p' | tail -n 1); printf "PRE_SEQ=%s\n" "${pre_seq:-0}"; pn-control read
```

`pn-control` is a future independent helper, not implemented here. Its retained
raw object has `command`, `exit_code=0`, `stdout`. Stdout has one KERNEL, BOOT and
PRE_SEQ line, and exactly one JSON object starting with
`"schema":"idma.pn_control.v1"`, containing `success=true`, `errno=0`,
`operation="read"`, `read_only=true`, `cap`, `gap_cap`, `mode=0`, `v_gap_cycles=0`.
Here `read_only` means configuration-readonly as defined above. Cap values are
actual readbacks, not mocked fixed constants. Pre watermark must equal first
trial sequence minus1; post watermark equals the final trial sequence. Both must
belong to the loaded boot and original kernel.

`records.json` contains exactly15 original parsed records with case1..15,
block0..4, condition, ds, raw command path, actual CLI/observer/PATH, boot and
sequence. Each trial raw is a unique run-local file. The complete original
wrapper must exactly match `command(condition)` in the script, including original
CLI options, CASE_EXIT, complete dmesg and taint boundaries. Only copied derived
metrics are ignored and recomputed; cached CLI/observer/PATH must match raw.
All run-local `command*.json` must be exactly the15 trials plus the2 readbacks;
extra failed/timeout attempts reject the gate and remain untouched. Resumed,
discarded, replaced or partial campaign fields reject. This is validation of the
saved evidence and its command inventory, not recovery or deletion detection
outside the recorded campaign.

## Parser and reference scope

The original as-dispatched parser is hash-locked from the approved source map.
Only its `parse` function is AST-compiled: no imports/main/Console are executed.
Exactly one legacy assertion (`shared==Vprivate+Aprivate`) is replaced by
same-window PATH0 `full+carry` equalities for AR/AW. All other assertions,
private stock/flow and16-stage checks remain. This handles the proven mux spill
boundary; it does not equate first offer with private acceptance. Original parser
hash/as-executed baseline identity and the exact adaptation are retained apart
from the actual S15 entry's independently verified hash. Shared collector files
are unchanged. PathEnable is mandatory; zero-full means/max remain N/A.

The source map itself is byte-locked. Its30 historical DS0 raw commands and
record membership are reread, hashes/commands/CLI/observer/PATH/boot/seq are checked
using the unmodified original parser, and the three n10 raw medians must equal
56.53868/107.09009/118.22441ms. No DS50/G1/reference revision is silently selected.
New values use actual raw hardware cycles/50000; each condition has five actual
trials. The inclusive threshold is checked exactly using Decimal:
`abs(new5_median-reference10_median)<=reference10_median*0.005`.
This is a cross-boot descriptive admission rule, not statistical trial pairing.

## Output and finite evidence

Only after complete identity/raw validation does the tool create `gate.json` and
`report.md`. Complete real numerical failure creates a failed gate with
`eligible_for_DS50=false`; missing/invalid evidence creates no output directory.
The original run is unchanged in both cases. A real passed gate returns exit0;
it still performs no hardware, build or automatic DS50 action. There is no paper
CSV/export entry.

Every public input recursively refuses explicit synthetic/fixture flags in
manifests, cached/raw objects and decoded trial/control JSON. There is no public
fixture override. Private finite tests force every product synthetic,
source_board_cases0 and eligible_for_DS50=false, even when all numeric cells pass.

[Final51-check receipt](s15-host-final03/validation.json) and
[actual execution output](s15-host-final03/execution.json) bind delivered source
hashes. Coverage includes exact15/30 references/n5/threshold, original immutable
bytes, valid closing spill/private−1, tampered shared/PATH, actual source/load/
software/collector bytes, old CLI ABI/roles/pages/bytes/data, missing/extra/reordered/
duplicate/gapped/cross-boot trials, MODE action/missing ledger/resume, cap/mode/gap
and pre/post watermark mismatches, recursive metadata and decoded raw markers,
fresh raw-derived metrics, source-byte changes during validation and no UART
import. Test inputs are constructed explicit synthetic protocols; temporary
campaigns are deleted afterward. Prior45/51-check receipts and one intermediate
failed test expectation are retained with their own source identity/limits;
they do not qualify the final code. The failure was a test expecting decoded-raw
refusal while the same marker was correctly rejected earlier in cached records.
