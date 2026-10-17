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

`pn-control` is the independent configuration-readonly helper described below. Its retained
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

## Independent PN read helper

`pn_control.c` provides exactly two operations: `pn-control capabilities` and
`pn-control read`. Both take a complete8-word snapshot at fixed DMA0 physical
base `0x50000000`; they differ only in the JSON `operation` field. There is no
MODE/GAP/configuration write operation, address override, alternate capability
profile or fallback path. Invalid/extra arguments fail before opening the device.
This batch compiles the helper and tests fake MMIO; it does not install or run it
on the board, modify the original CLI/kernel/rootfs or collect any board point.

Production uses `/dev/mem` with `O_RDWR|O_SYNC|O_CLOEXEC` and one page mapped by
`mmap64(MAP_SHARED)`. All register accesses are aligned volatile64-bit accesses
with `fence iorw,iorw` before and after. The IO interface has only `load64` and
`select_index`; the latter can write only INDEX at `+0x70`, with indices192..199.
Each successful INDEX store is followed by a64-bit INDEX readback equality check
before DATA at `+0x78` is read. A complete scan leaves INDEX199 selected. These
selector writes are configuration-readonly as defined above; no DATA/MODE/GAP
store exists. Saved raw words are decimal uint64 values, not split32-bit reads.

The accepted constants are PN_CAP `0x49444d41504e0001` at word192 and GAP_CAP
`0x49444d4147415001` at `+0xb8`. Word193 must be mode0..3. The direct gap read at
`+0xb0` must have high32 bits zero; low32 bits are `v_gap_cycles`. Words194/199,
including arbitrary high bits, are saved unchanged and have no interpreted
status/validity/overflow fields in this batch. Words195..198 also remain raw;
this read-only batch does not qualify Aown/union duration or N-accept metrics.

Root must own the board execution lock and serialize the helper after the old
CLI has exited and its actual STOP/drain has finished. Never interleave a PN
INDEX/DATA scan with original CLI/kernel PATH reads. OBS_CONTROL at `+0x60` is
read before and after the scan; lowbits0/1 (ARMED/busy) reject the read. This is
supplementary conflict detection, not an atomic kernel mutex, global quiet proof
or protection against a concurrent writer. Neither the8 bank words nor their
relation to direct gap/status reads is an atomic snapshot. Root serialization is
the execution contract; this helper does not acquire or impersonate it.

Every normal failure emits `success=false`, a positive Linux `errno` and a fixed
`stage`, then exits nonzero. It preserves all values acquired before failure and
uses JSON null for unread values. Capability mismatch is `ENODEV`; invalid mode
or highgap is `EPROTO`; INDEX readback mismatch or synchronous MMIO SIGBUS/SIGSEGV
is `EIO`; armed/busy is `EBUSY`. Open/map/signal setup/cleanup errors retain their
actual errno. The first error survives later cleanup errors. The synchronous
fault handler only jumps out of the guarded volatile access; JSON is emitted
after normal cleanup. External fatal signals and stdout failure cannot guarantee
a complete saved JSON; a nonzero/absent result must not qualify a readback.

The native fake harness includes the actual `pn_snapshot`, argument parser and
JSON emitter. It cannot open `/dev/mem`; every output is explicitly fixture-only,
synthetic and source_board_cases0. There is no production command-line fixture
switch. The public S15 gate rejects the actual emitted fake JSON; a private
fixture protocol check verifies the same fields without removing its tags.

```text
python3 check_pn_control.py --software-manifest ORIGINAL_SOFTWARE_BUILD_JSON \
  --output NEW_TOOLS_ARTIFACTS_DIRECTORY
```

The compiler name and `-O2 -static -Wall -Wextra` profile come from the unchanged
original software manifest (SHA `d9f3df411c28628293d60e9a8689535b2119955cd3975bbfe29b8cdc576c7951`).
Its CLI-only UAPI `-I` is unnecessary for this standalone libc helper and is
omitted. The installed compiler is RV64 GCC11.4.0, default `rv64imafdc/lp64d`;
no compiler/tool installation or upgrade is performed. Actual argv, compiler
bytes/version/defaults, source hashes, ELF SHA and readelf/objdump output are
retained in the [137-check receipt](pn-control-host01/validation.json).
The compiled production ELF stays local and ignored at
`tools/artifacts/pn-control-host01/pn-control`; it is not committed.

The58 labeled C scenarios cover four modes; gap0/1000/4000/u32max; raw194/199;
capability/mode/highgap refusal; both OBS guards; all8 INDEX readback mismatches;
errors at all28 read/store positions with immediate stop and retained/null data;
unknown/write/extra arguments and first-error retention. Native O2 and non-PIE
ASan+UBSan runs agree. The actual RV64 static ELF has64-bit `ld` at the MMIO read
and `sd` at INDEX offset112, both bracketed by instruction `0ff0000f` (full I/O
fence). Stack/RAM stores in those functions are not MMIO writes. Production
open/mmap/signal-handler execution is source-reviewed and cross-compiled only;
neither an emulator nor hardware was used. This evidence does not establish
board success or permit any fixture to enter S15/DS50/paper results.
