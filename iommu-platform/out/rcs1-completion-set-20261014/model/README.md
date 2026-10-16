# Four-condition RCS1 model handoff

Codex authored and executed this new conditional engine. Root authorized its
entry in `../root/entry-gate.json`; no DeepSeek, Claude, Astra, old experiment,
trace, validator, benchmark, build or production change was used here.

All32 fixed policy episodes completed and passed the engine's actual success
checks. RCS1 does not outperform the strongest frozen ordinary control in
these whole-episode results. Root owns the final hypothesis/novelty decision.

| Condition | RCS1 max / sum ticks | Best ordinary max / sum ticks | Concrete ordinary |
|---|---:|---:|---|
| C1 | 921 / 3491 | 920 / 3478 | SJF |
| C2 | 923 / 3484 | 923 / 3484 | All programs exactly equal |
| C3 | 921 / 3491 | 920 / 3478 | Oldest and SJF |
| C4 | 907 / 3411 | 900 / 3408 | SJF |

Ticks are conditional completion timestamps from common-ready0, including all
16 real prefix copies, publication, remaining payload/B, each final consumer
receipt and owner release. They exclude unknown physical software supply cost;
they are not measured FPGA/application or CPU-inclusive gains. See
[limits](limitations.md).

Every successful row transfers51 real64B copies:3264 payload bytes read and3264
written,51 payload AR/AW/B and408 R/W beats. Each row checks every read value,
51 copy outputs,408 final destination beats,4 task consumer receipts/releases,
one producer publication receipt, held owner/epoch/permission identities,
native AW order, W-from-actual-R order, original512-beat capacity, and final
quiet. All responses are OKAY, error_count0, zero error injection. Error/drain
and generation-collision fallback are not experimentally validated here;
actual stale_snapshot_fallbacks and response-port collisions are0 in all rows.

All programs independently start with invalid cache entries and native15-bit
tree zero. PLRU, pin, oldest, SJF, minrefs, fractionInsert and RCS1 naturally
produce the intended16-way q snapshot with native tree all0. RCS1 evicts p3,
SJF evicts b, PLRU/minrefs/fractionInsert evict a. B4's fixed cold fraction
admission independently leaves no resident entries and walks all102 lookups;
no prefix or cache state from another rule is assigned to it. The stable q
snapshot/action records are in each per-case result and raw event file.

The model run had one concrete implementation failure: first C1/PLRU stopped
at tick0 before any PTE/data service because the log helper's parameter `kind`
conflicted with a service operation's `kind` field. The parameter was renamed;
the original raw events, traceback and first ledger are preserved. Only this
failed case was retried, followed by the31 still-unexecuted cases. There are32
completed episodes plus this one failed invocation, not a hidden sweep. No
work, policy, bytes, clock or service parameter was changed after execution.

- [Frozen input](frozen-input.json), SHA256
  `5945288d97bb3fd61b72db67d6cc4feb702b40e656c2190db1d499793b650a2e`.
- [Execution ledger](execution-ledger.json) records32 passed entries and the
  failed attempt, per-attempt code hashes, gate/input hashes and UTC times.
- [Original first ledger](execution-ledger-before-recovery1.json),
  [failed raw events](C1-PLRU.events.jsonl),
  [failure traceback](C1-PLRU.failure.txt),
  [successful targeted recovery events](C1-PLRU.retry2.events.jsonl).
- [Complete32-row JSON summary](results-summary.json) and
  [CSV completion/counter table](results-table.csv).
- Per-case `C{1..4}-{policy}.result.json` contains role+ID task completions,
  actual bytes/PTE/AXI counters, all cold prefix receipts, full q decision and
  all policy snapshots/actions. Its `events` member points to the complete
  raw JSONL and supplies its count and SHA256.
- [Engine](conditional_engine.py): native PLRU at line48; fixed work/blocks at
  line92; task/remaining-use ledger at line174; policy programs at line279;
  original job certificate/native AW association at line359; actual last-use
  retirement at line396; stable result/response arbitration at line409;
  single-port RR/active-PTE window at line541; paid choice/generation fallback
  at line596; native AR/AW receipts/W ordering at line648; dependent PTE/shared
  FIFO and actual data at line696; completion/quiet checks at line764.

One metadata clarification happened before any model run: root finalized the
native shared single lookup/response ports, RR and active-PTE fast-hit window.
[Preexecution correction](preexecution-correction.json) and the original
`frozen-input-before-port-clarification.json` preserve that input history.
No model result existed when this clarification was applied.

Physical area, Fmax, measured CPU/MMIO/registration time, actual software poll
count, application and GPU/AI gain all remain null. The sourced possible
software supply route accounts255 MMIO operations / nominal2040 register
bytes separately; it is not charged as actual bus or CPU time by this FIFO.
