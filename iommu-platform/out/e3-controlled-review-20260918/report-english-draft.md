<!-- docs/e3-controlled-results-review.md -->
# E3 v6 controlled run — server-side results review (draft)

**Status: draft, not yet re-verified.** The tables below were derived from
`all-original-pairs.json` and cross-checked against `E3_CONTROLLED_RESULTS.md`
and `codex-decisions.md`. The first reviewer run (`review-draft.json`) is
**invalid as a verdict** (see §6) and the corrected reviewer must be re-run
before any of these numbers is quoted as a verified reviewer output. No RTL,
driver or CLI file was changed; no board run, recompile or re-flash was made.

## 1. Evidence and integrity

| Input | Expected relation | Checked by |
|---|---|---|
| `all-original-pairs.json` | 11 `idma.e3_controlled.v1` pair documents | reviewer pair checks |
| `console.raw.log` | 22 unique sequences, 418 unique observation rows, cumulative dmesg (short dump seq 1–6, main dump seq 1–22) | row-count + sequence checks |
| `console.readable.log` | exactly `console.raw.log` with its single NUL replaced by `[NUL]` | relation check (`nul_bytes_in_raw = 1`) |
| `run.json` | secondary summary; counters re-derived by the script | secondary-summary comparison (now propagated) |

All inputs are opened read-only; the reviewer writes exactly one new file
outside the run folder. The short dump duplicates seq 1–6 of the main dump, so
**114 identical duplicate rows are expected and deduplicated**, leaving 418.

## 2. Execution facts (script-derived)

- 11 pairs / 22 rounds; every round has 2 entries per pair; sequences 1–22 with
  the pair order 1,3,5,…,21.
- 14 triggered rounds (all cold), 8 nontrigger rounds (solo, both hot pairs,
  and their D0/D128 companions). No V IOTLB miss and no V S1 PTW in any formal
  round; A shows 2 IOTLB misses / 2 S1 PTW per cold round and 0 in hot rounds.
- `data_errors = 0`, `quarantined = 0`, `s2_ptw = 0`, taint 0.

## 3. Per-pair round readings ("first / second" is order within the pair, not a ratio)

| Pair (phase/case) | A prep | D order | Trigger | V hw cycles | A remaining `e41−e4` | V remaining `e70−e4` | V queue `e70−e69` | V own `e73−e71` |
|---|---|---|---|---|---|---|---|---|
| 1 (short/1) | none cold | 0 / 128 | 0 / 0 | 99 / 99 | – | – | – | – |
| 2 (short/2) | 1 hot | 0 / 128 | 0 / 0 | 115 / 115 | – | – | – | – |
| 3 (short/3) | 1 cold | 0 / 0 | 1 / 1 | 545 / 545 | 273 / 271 | 282 / 280 | 416 / 413 | 4 / 4 |
| 4 (main/1) | 1 cold | 0 / 0 | 1 / 1 | 551 / 543 | 274 / 272 | 283 / 281 | 417 / 412 | 4 / 4 |
| 5 (main/2) | 1 cold | 0 / 128 | 1 / 1 | 542 / 672 | 272 / 399 | 281 / 408 | 412 / 541 | 4 / 4 |
| 6 (main/3) | 1 hot | 0 / 128 | 0 / 0 | 114 / 116 | – | – | – | – |
| 7 (main/4) | 1 cold | 128 / 0 | 1 / 1 | 680 / 546 | 400 / 271 | 409 / 280 | 551 / 415 | 4 / 4 |
| 8 (main/5) | 1 cold | 0 / 0 | 1 / 1 | 542 / 543 | 271 / 271 | 280 / 280 | 411 / 411 | 4 / 4 |
| 9 (main/6) | 1 cold | 0 / 128 | 1 / 1 | 543 / 672 | 272 / 401 | 281 / 410 | 412 / 541 | 4 / 4 |
| 10 (main/7) | 1 hot | 128 / 0 | 0 / 0 | 124 / 117 | – | – | – | – |
| 11 (main/8) | 1 cold | 128 / 0 | 1 / 1 | 672 / 546 | 401 / 271 | 410 / 280 | 541 / 415 | 4 / 4 |

Nontrigger controls keep `identity`, `milestones` and readings at `null` with an
explicit `no_trigger` reason; no zero-time substitute is written.

## 4. Raw e3_words deltas on the 14 triggered (cold) rounds

Recorded as facts by `word_deltas`, with **no expected value used as a gate**:

- `e70 − e41 = 9` on all 14 triggered rounds.
- `e42 − e41 = 2` on all 14.
- `e73 − e71 = 4` on all 14 (V's own translation service is unchanged).
- `e41 − e4` is 271–274 on the D0 rounds and 399–401 on the D128 rounds.

The constant 9 and 2 are *observed regularities*. They are reported, never
asserted as an eligibility or pass condition, and no tolerance is defined.

## 5. Baselines and D128−D0 deltas

D0/D0 baselines (second round − first round): pair 4 `V = −8`, A remaining
`−2`, V remaining `−2`, queue `−5`; pair 8 `V = +1`, `0 / 0 / 0`; pair 1
(nontrigger) `0`; pair 3 (cold, both rounds) `0 / 0 / 0 / 0`.

Cold complementary pairs, computed as **D128 − D0** (not "second − first"):

| Pair (main case) | ΔA remaining | ΔV remaining | ΔV queue | ΔV hw cycles |
|---|---|---|---|---|
| 5 (case 2) | +127 | +127 | +129 | +130 |
| 7 (case 4) | +129 | +129 | +136 | +134 |
| 9 (case 6) | +129 | +129 | +129 | +129 |
| 11 (case 8) | +130 | +130 | +126 | +126 |

So ΔV remaining is {127, 129, 129, 130}, identical to ΔA remaining; ΔV queue is
{129, 136, 129, 126}. **This corrects an earlier draft statement that put V
remaining at 126–136** — 126 belongs to the queue/total column, not to V
remaining.

Hot negative controls: pair 6 `+2` cycles, pair 10 `+7` cycles, both with **no
trigger**; they are controls without an actual 128-cycle exposure and are not
equal-exposure comparisons.

## 6. Prefix / identity / insertion comparability, and finite N

For the 6 formal cold pairs: A is a read with sequence 1, ID 4, DID 11; V is a
read with sequence 1, ID 0, DID 10 and `first_hit = 1`; both share IOVA
274875809792, `s1_level = 0`, `pte_pa = 2193147896`, and the same prefix vector
(words 78–93). Each triggered round holds exactly **one** intervening request:
a V **write**, sequence 1, IOVA 274875813888, `hit = 1`, untruncated — a
different request from the V read despite the shared sequence number. A
comp/appear/return events are present (event bitmap 127).

Pre-trigger elapsed times are reported, not gated: `e4 − e69` and `e4 − e39`
differ across rounds in three of the four cold complementary pairs; **main case
6 is the only one of those four with equal recorded pre-trigger elapsed times**
(`131/131` and `135/135`) and is the strongest timing-matched example. The other
three are kept, not discarded, because the original experiment's eligibility
requirements (E3_CONTROLLED §11) do not require identical pre-trigger times.

Limits: N = 4 complementary pairs, one boot, finite 64 B diagnosis. No
significance test, no invented tolerance, no total-time attribution of the
whole residual difference.

## 7. Mechanism statement (scoped) and its RTL basis

> In this implementation, an extended A S1 PTW response propagates to an
> already waiting V read that then first-hits: the A miss and the V hit are
> serialised behind one shared controller/AR queue.

Supporting RTL, as reviewed by Codex: `rv_iommu_e3_delay.sv` lines 20–60 (one
buffered response on the PTW-only path, one capture per ARM, the 128 counter
only on the chosen response); `rv_iommu_tl_wrap.sv` lines 590–616 (DID 11 S1
miss, no S2/DDT/PDT/MSI, V waiting); `rv_iommu_trans_ctl_wrap.sv` lines 140–157
(AR FIFO head advances only on `ar_ready`) and 616–716 (IDLE select, AWAIT_RESP
holds, COMPLETE waits for output-address accept). FPGA observer `dma_e3_observer.sv`
lines 246–251 record the translation response `e41`/`e73` (not the data return,
which is `e43`/`e75`), lines 254–273 the trigger + V-pending + prefix, lines
175–185 the data return. The constant 9 and the output-accept+2 support
serial-service waiting; they do **not** prove that every downstream stage was
ready throughout.

Precise wording to use: *this implementation's A-miss-induced V-hit blocking at
the shared serial controller/AR queue*. Not: "the AXI specification forces it".

## 8. What is not claimed

Not full E3 matrix completion, not universal head-of-line blocking, not an
explanation of all old A8 slowdown, not a mitigation-performance result, not
statistical generalisation, not novelty, no p-value, no tolerance. E1 evidence
is untouched. E2 is not proved and is an optional evidence-driven branch. E4 is
only partly done (no claim that all data/software effects are excluded).

## 9. Reviewer defects found and fixed (why `review-draft.json` is not a verdict)

1. `OBS_RE` used `$` without `re.MULTILINE` over the whole multi-line console,
   so it could only match at end-of-file → **0 UART rows** against 418 expected.
   Fixed with `re.MULTILINE` and an explicit `[ \t]+` separator; fragmented
   realtime lines outside the dmesg dumps stay ignored (counted, not used).
2. Errors raised by `check_round`, `check_round_uart` and `expect_matrix_entry`
   usually lack a `pair N ` prefix and were filtered out when building
   `execution_ok`, `pair.errors` and `findings.structural_errors` → a
   **false pass**. All errors raised while a pair is checked are now prefixed
   exactly once and propagated without any string matching.
3. `check_round_uart` only compared `cycles` and `e3_words[233]` against the
   UART. A full-record comparison is now required: every
   `idma.e3_controlled.v1` JSON document printed to the console is parsed,
   identical duplicates removed, and each is compared field-by-field (all 256
   `e3_words`, the 32 launcher words, prep stats, per-round stats and mapping,
   uint64 preserved) against the corresponding original pair document, in
   order and count. Missing, extra or conflicting documents fail.
4. `observed_version_and_magic.ok` was hard-coded `True`; it now reflects the
   actual version/magic findings.
5. The secondary `run.json` comparison could disagree without affecting the
   exit code; it is now propagated as a structural error (the compared counters
   are also computed independently by the script).
6. `pair_eligibility` did not expose per-round event/hit/truncation facts and
   used pre-trigger time equality inside the causal label chain. It now reports
   per-round event bitmaps, first-hit values, insertion count/truncation and
   selection order, plus identity/PTE/prefix/insertion-order/multiplicity match
   flags; pre-trigger equality remains a **diagnostic only**.
7. `word_deltas` (`e41−e4`, `e70−e41`, `e42−e41`, `e73−e71`, `e4−e69`,
   `e4−e39`, `e70−e4`) are now produced by the script per triggered round.

No existing round check was found semantically wrong: `e3_words[233]` vs the V
write stage-3 sum, `e3_words[17]+[21] = 1`, `e3_words[18]+[22] = 1` and the
launcher/observer checks are all retained unchanged.

## 10. Acceptance (to be executed, not yet run here)

```
python3 out/e3-controlled-review-20260918/server_review_v6.py \
    <run_folder> <new_out.json> \
    --console <run_folder>/console.raw.log \
    --readable-console out/e3-controlled-review-20260918/console.readable.log
```

Expected: exit 0, `findings.structural_errors == []`, `totals.unique_uart_rows
== 418`, `totals.duplicate_rows_ignored == 114`, `totals.pairs == 11`,
`totals.rounds == 22`, `checks.console_cli_json_match.ok == true` with 11
deduplicated documents, `checks.pair_documents.ok == true`,
`checks.observed_version_and_magic.ok == true`, and every pair document
matching the console CLI JSON exactly. Then run the narrow mutation check
(§11) on temporary copies only.

## 11. Mutation checks

`scripts/mutate_e3_controlled_review.py` copies the run folder to a temp
directory and requires a non-zero exit with at least one structural error for
each of: an E3 magic word, an E3 value word (`e3_words[17]`), a launcher word,
a UART `cycles=` field, the pair `input` metadata, and one deleted UART row —
while the unmutated control must exit 0 with zero structural errors. It hashes
the real inputs before and after and refuses to report success if they changed.

## 12. Reproduce

```
python3 scripts/collect_e3_controlled.py results/experiments/20260918-164635-run-rk48bhte --phase short
# then --phase main
```

Raw data stays read-only; no RTL/driver/CLI change, no recompile, no board run
in this task. Timing closure, CDC and external-IO constraints remain
unreviewed and are not replaced by a successful board execution.
