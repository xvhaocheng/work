# P4M batch-start control protocol (draft)

**Status: protocol draft. This round freezes only the runtime protocol and the
log/JSON contract. The software build is pending and this runner has not been
run on a board; there is no new build or board evidence here. This is not a
performance conclusion.**

## Scope

- Exactly twelve `idma_experiment` runs are allowed: four short runs first, then
eight comparison runs, and the comparison block runs only after every short run
passed. No retry, no extra benchmark, no FPGA/SD write, no `dmesg` clear.
- short (64 transfers, warmup 8, 4096-byte chunk, `platform-timeout 120`,
  `--timeout-ms 60000`):
  1. Solo `--p4m --aggressor none --a-gap-cycles 0`
  2. A1 gap0 `--p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0`
  3. A8 gap0 `--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0`
  4. A8 gap1000 `--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000`
     (engineering check only)
- compare (4096 transfers, warmup 256, 4096-byte chunk), frozen order
  `B8 P8g0 P8g0 B8 B8 P8g0 P8g0 B8`:
  - B8   = `--batch --aggressor 1 --aggressor-pages 8` (legacy batch, no P4M bank)
  - P8g0 = `--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0`
- Default output directory `/tmp/p4m-start-$$`; a user-supplied directory must not
  already exist.

## Startup record contract (software implementation)

Every batch attempt emits, after the window ends, after drain and after
validation, exactly one line:

```
idma_batch_start version=1 seq=<observer sequence> method=irq_pair v_did=10 a_did=11|4294967295 attempted=1 has_a=.. a_triggered=.. v_triggered=1 a_id=.. v_id=.. a_begin_ns=.. a_end_ns=.. v_begin_ns=.. v_end_ns=.. irq_span_ns=.. a_to_v_ns=.. error=0
```

- `method=irq_pair` is only used in the protected "A first, then V, plus the
  software timestamp reads" interval; the ordinary non-batch path does not claim
  `irq_pair`.
- `attempted=1`: the trigger interval was entered.
- `a_triggered`: a read of the A-side NEXT_ID register was attempted. A zero
  readback still counts as an attempt; the field only means something when
  `has_a=1`.
- `a_id` and `v_id` are mandatory. Solo reports `a_id=0` (no A trigger) and a
  positive `v_id`. Pair reports a positive `a_id` and a positive `v_id`.
- solo (`--aggressor none`): `has_a=0`, `a_triggered=0`, `a_did=4294967295`,
  `a_id=0`, `a_begin_ns=0`, `a_end_ns=0`, `a_to_v_ns=0`; no A-field is faked.
- `error` is the session's final error number; 0 means this line's attempt had
  no error.
- The four A/V timestamps are software trigger-read brackets:
  `a_begin_ns <= a_end_ns <= v_begin_ns <= v_end_ns` and
  `a_to_v_ns = v_begin_ns - a_begin_ns`. They are NOT hardware issue instants and
  are not an interrupt-latency measurement.
- `irq_span_ns = v_end_ns - (has_a ? a_begin_ns : v_begin_ns)`, and it is a
  lower bound for the protected interval. It is only required to be positive; it
  is not presented as hardware-timing evidence.
- The runner tolerates unknown additional fields.

## Batch window record contract

For the same seq, exactly one `idma_batch_obs ... part=time ...` line carries the
run's own hardware window:

```
idma_batch_obs seq=<seq> part=time valid=.. status=0x.. v_did=10 a_did=.. cycles=.. h=.. gap=.. hmax=.. gapmax=.. issued=.. done=.. batch_polled=.. done_to_read=..
```

The runner requires `valid=1`, `status=0x4`, `issued=done=<transfers>` and
`batch_polled=1` from this record, for both P4M and legacy batch.

## CLI JSON contract

The frozen CLI JSON is nested. The runner reads values from their real locations
and does not assume any top-level hardware-window field:

- top level: `success`.
- `result`: `status`, `hpm_configured`, `measurement_started`,
  `measurement_complete`, `victim_did`, `aggressor_did`,
  `victim.{submitted,completed,data_errors,quarantined}` and (for a pair)
  `aggressor.{data_errors,quarantined}`.
- `p4m` (only for `--p4m`): `sequence`, `observer_status`, `v_issued`, `v_done`.
- There is NO top-level `batch_polled`, `status`, `v_issued`, `v_done` or
  `time_valid`.

## What the runner checks

- Each case snapshots `dmesg` before and after the CLI call. Exactly one NEW
  `idma_batch_start` line must appear AND its `seq` must be strictly greater than
  the maximum `seq` seen before the run; a counted-but-reused `seq` is refused.
- Exactly one new `idma_batch_obs ... part=time ... seq=<that seq>` record must
  exist in the after snapshot.
- The startup record has prefix `idma_batch_start version=1 seq=`,
  `method=irq_pair`, `attempted=1`, `v_triggered=1`, `v_did=10`, `error=0`,
  integer fields, `a_id`/`v_id` present, ordered timestamps and the `irq_span_ns`
  identity above. Solo and pair differences follow the table above.
- The CLI JSON must report `success=true` and, from the nested objects,
  `result.status=0`, `hpm_configured=1`, `measurement_started=1`,
  `measurement_complete=1`, `victim_did=10`,
  `victim.submitted=victim.completed=<transfers>`, `victim.data_errors=0`,
  `victim.quarantined=0`. Pair trials also require `aggressor_did=11`,
  `aggressor.data_errors=0` and `aggressor.quarantined=0`. `--p4m` trials also
  require `p4m.observer_status=4`, `p4m.v_issued=p4m.v_done=<transfers>` and
  `p4m.sequence=<startup seq>`.
- A opening id (opening in-flight ID) and A progress are recorded but never
  judged: a different opening id and zero A progress are not failures. The raw
  JSON is kept unchanged and printed to UART.
- Other gates: `uname -r` must not be the known old revision `00088` (the new
  revision hash is not hardcoded; `uname -r`/`uname -a` are recorded); required
  commands and a working `uname` are checked before any DMA; `path_observe=Y` is
  required before any `--p4m` case; `observe` is recorded but not required to be
  Y; taint must be 0 at preflight, per case and at the end.

## Evidence and manual selection

- Each case keeps: raw JSON, raw stderr, `<case>.dmesg.before`,
  `<case>.dmesg.raw`, the verified `<case>.startup` and the matching
  `<case>.batch_time`; plus `argv.txt`, `input.txt`, `matrix.txt`, `params.txt`,
  `path_observe.txt` and the closing `dmesg.raw`/`taint.txt`.
- stdout prints the raw CLI JSON (visible on UART); control lines and dmesg go to
  stderr and are captured on the host side. Every sample is retained on failure;
  nothing is cherry-picked.

## Non-claims

- This is not a formal P4M conclusion: the new software needs its own gap0
  baseline; the legacy batch A window is still unknown; the A/V start interval is
  an observation inside the software trigger brackets and is not directly the
  hardware issue interval.
- gap500 stays in the old pilot-21 records; this round does not repeat it and
  draws no conclusion from it.
- No new FPGA compile, no RTL timing signoff, no performance gain/regression
  verdict.
