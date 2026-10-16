## Findings (from the supplied diagnostics + `analysis.json`)

Three real defects, all in the **summariser**, none in the DUT (every run has `exit_code 0`, PASS marker, no FATAL) — R1–R8 each fail exactly 3 conditions:

1. **`no unknown csv region` → detail `other`** (all 8 runs). `other` is a *legal* region for `wcount` / `rblock_begin` / `rblock_end` only (event_totals in `analysis.json`: `wcount 70`, `rblock_begin 4`, `rblock_end 4`); the blanket `reg not in KNOWN_REGIONS` check wrongly flags it.
2. **`{vsrc,asrc}: var count == 64/32`**. `VAR_COUNT = {"pre": 64, "post": 32}` is the **two-flow total**. Real per-flow counts (from `analysis.json` `csv.var_counts`): pre `vsrc 32 / asrc 32` (=64), post `vsrc 16 / asrc 16` (=32). The per-flow check must be `VAR_COUNT[prefix] // 2`, plus a new **total** check that must stay 64/32.
3. **`ORIGINAL_MODEL_NA` narrative** (`analysis.json` key `original_model_na`, `NOTES[5]`, md section "counts the original modelling recorded as NA") asserts an unverified "original model marked NA" history. These are just different observables from earlier summaries — remove the claim, keep the per-placement observation section (`observed_placement_metrics` / `## observed placement metrics`).

`NATURAL-JSON.var == VAR_COUNT[prefix]` (two-flow total 64/32) is **correct and unchanged**. Strict failure path (`sys.exit(0 if overall_ok else 1)`) and all positive/negative boundaries are untouched.

Post-fix expectation: `unknown_regions == []`, per-flow var 32/16, totals 64/32 ⇒ **8/8 PASS, exit 0, conditions_total 902 (8×112 + 6), conditions_failed 0**.

```diff
--- a/out/fragment-placement-20261008/placement_summary.py
+++ b/out/fragment-placement-20261008/placement_summary.py
@@ -30,6 +30,7 @@
 Scope notes kept deliberately narrow:
   * CSV "var" is the IOMMU ingress request stream (flowID 0 for v, 4 for a);
-    the iDMA logical parent count (32) comes from POSTTL-STATS only.
+    32 rows per flow pre / 16 per flow post (64 / 32 across both flows).  The
+    iDMA logical parent count (32) comes from POSTTL-STATS only.
   * rfirst/rlast are physical R burst endpoint marks and are never counted
     as beats.  Per-flow physical W beats = pw + pwlast = 4096.
@@ -67,8 +67,10 @@
 FLOW_ID = {"v": 0, "a": 4}            # IOMMU ingress flowID seen in CSV "var"
 PAR_ID = {"v": 32, "a": 36}           # physical source AR id seen in CSV "par"
 
 # CSV "var" == IOMMU ingress requests (a quantity distinct from the iDMA
-# parent count).  pre 64 / post 32; never folded into one constant.
-VAR_COUNT = {"pre": 64, "post": 32}
+# parent count).  pre 64 / post 32 is the total across BOTH flows (v + a);
+# the per-flow expectation is half of that.  The two are never folded into
+# one constant.
+VAR_COUNT = {"pre": 64, "post": 32}                        # two-flow total
+VAR_COUNT_PER_FLOW = {k: v // 2 for k, v in VAR_COUNT.items()}
 
 EXPECT = {
     "parents": 32,
@@ -106,13 +106,6 @@
              "n_ptw_ar": 102, "n_ddtw_ar": 2, "n_fast": 0},
 }
 
-# The three counts the original modelling accepted as NA (no single accepted
-# constant), including the IOMMU-ingress VAR difference.  Checked per variant.
-ORIGINAL_MODEL_NA = [
-    "CSV var count (IOMMU ingress requests): pre 64 / post 32",
-    "n_tl_accept (post-TL accepted requests): pre 98 / post 66",
-    "n_iotlb_hit: pre 64 / post 32",
-]
-
 STARTUP_COND = "startup order matches natural-first (v issues first for first=v, mirrored for first=a)"
 
 FORBIDDEN_OPT_SUBSTR = ("cpu", "fixture", "priority", "prefer", "force")
@@ -122,7 +122,10 @@
     "wcount", "vaw", "var", "rblock_begin", "rblock_end",
     "paw", "par", "rfirst", "rlast", "vw", "pw", "vwlast", "pwlast",
 }
 KNOWN_REGIONS = ("vsrc", "asrc", "vdst", "adst")
+# "other" is a legitimate region only for the three bookkeeping events that
+# carry no placement side; an "other" region on any other event (including
+# source/destination events) is still reported as an unknown region.
+OTHER_REGION_EVENTS = ("wcount", "rblock_begin", "rblock_end")
 PW_EVENTS = ("pw", "pwlast")
 VW_EVENTS = ("vw", "vwlast")
 
@@ -133,15 +133,16 @@
     "cpu/fixture/priority/prefer/force option may be present.",
     "all 8 runs of this round are fresh (result.json.reused is false); the "
     "../parent-capacity-20261008 files are reference-only and are not counted as runs.",
-    "POSTTL-STATS parents=32 is the iDMA logical parent count.  The CSV 'var' event is the "
-    "IOMMU ingress stream (flowID 0 for v, 4 for a): pre 64 / post 32.  The two must not be "
-    "confused, and the parent count is never read from the CSV var count.",
+    "POSTTL-STATS parents=32 is the iDMA logical parent count.  The CSV 'var' event is the "
+    "IOMMU ingress stream (flowID 0 for v, 4 for a): 64 requests in total across both flows "
+    "pre / 32 post, i.e. 32 per flow pre / 16 per flow post.  The parent count and the CSV "
+    "var count are different quantities and are never read from one another.",
     "PLACEMENT-METRICS-JSON is produced by the newly added C++ observation.  TL accept "
     "98/66, IOTLB hit 64/32, miss 34, PTW 102, DDTW 2, fast 0 are the currently observed "
     "values and are checked per placement, not asserted as pre-existing theory.",
-    "The three counts the original modelling recorded as NA (CSV var, n_tl_accept, "
-    "n_iotlb_hit) legitimately differ between pre and post and are therefore checked per "
-    "variant instead of against one constant.",
+    "CSV region 'other' is accepted only for wcount / rblock_begin / rblock_end, the three "
+    "bookkeeping events that carry no placement side; an 'other' region on any other event "
+    "is still a failure.",
     "per-flow latency uses PLACEMENT-METRICS-JSON.flow[*].done_cycle.  The last DDR W cycle "
@@ -359,9 +359,10 @@
                 if ev not in KNOWN_EVENTS:
                     unknown_events.add(ev)
                     continue
-                if reg not in KNOWN_REGIONS:
+                if reg not in KNOWN_REGIONS and not (
+                        reg == "other" and ev in OTHER_REGION_EVENTS):
                     unknown_regions.add(reg)
                 if ev == "var":
                     out["var"].setdefault(reg, []).append((rid, addr, length, cyc))
                 elif ev == "par":
                     out["par"].setdefault(reg, []).append((rid, addr, length, cyc))
@@ -596,9 +596,10 @@
 
         var_rows = csvd["var"].get(src, [])
         add(f"{src}: var records present", bool(var_rows), len(var_rows))
-        add(f"{src}: var count == {VAR_COUNT[prefix]} (IOMMU ingress: pre 64 / post 32)",
-            len(var_rows) == VAR_COUNT[prefix], len(var_rows))
+        add(f"{src}: var count == {VAR_COUNT_PER_FLOW[prefix]} per flow "
+            f"(IOMMU ingress: {VAR_COUNT[prefix]} total across both flows)",
+            len(var_rows) == VAR_COUNT_PER_FLOW[prefix], len(var_rows))
         add(f"{src}: var flowID == {FLOW_ID[flow]}",
             bool(var_rows) and all(v[0] == FLOW_ID[flow] for v in var_rows),
             sorted({v[0] for v in var_rows}))
 
@@ -634,7 +634,12 @@
         w = csvd["w_count"].get(dst, 0)
         add(f"{dst}: physical W beats (pw + pwlast) == {EXPECT['w_beats_per_flow']}",
             w == EXPECT["w_beats_per_flow"], w)
 
+    var_total = sum(len(rows) for rows in csvd["var"].values())
+    add(f"csv var total == {VAR_COUNT[prefix]} (IOMMU ingress, both flows)",
+        var_total == VAR_COUNT[prefix],
+        json.dumps({r: len(v) for r, v in csvd["var"].items()}, sort_keys=True))
+
     vaw_total = sum(csvd["vaw_count"].values())
     add(f"csv vaw total == {EXPECT['vaw_total']} (AW 34)",
         vaw_total == EXPECT["vaw_total"], json.dumps(csvd["vaw_count"], sort_keys=True))
@@ -726,5 +726,5 @@
 # --------------------------------------------------------------------------- #
 def write_md(path, *, runs, equivalence, latency_rows, latency_deltas,
-             conditions_summary, original_model_na, notes, claim_limits,
+             conditions_summary, notes, claim_limits,
              not_evaluated):
     L = []
@@ -784,12 +784,6 @@
            f"{pm.get('n_fast', '-')} |")
     ap("")
 
-    ap("## counts the original modelling recorded as NA (including the IOMMU-ingress VAR)")
-    ap("")
-    for item in original_model_na:
-        ap(f"- {item}")
-    ap("")
-
     ap("## issue -> done latency per flow (PLACEMENT-METRICS-JSON)")
     ap("")
     ap("| run | natural-first | v issue | v done | v issue->done | a issue | a done | "
@@ -852,9 +852,9 @@
     ap("## endpoints / per-flow physical accounting")
     ap("")
     ap("| run | flow | par id | par count | par len | par base | 32 KiB tile | rfirst | "
-       "rlast | dst region | W beats (pw+pwlast) | var (IOMMU ingress) |")
+       "rlast | dst region | W beats (pw+pwlast) | var (IOMMU ingress, per flow) |")
     ap("|---|---|---|---|---|---|---|---|---|---|---|---|")
     for stem, r in runs.items():
         eps = r.get("endpoints") if isinstance(r.get("endpoints"), dict) else {}
@@ -1105,9 +1105,8 @@
         "equivalence": equivalence,
         "conditions": aggregate_conditions,
         "summary": summary,
         "observed_placement_metrics": PLACEMENT_METRICS_OBSERVED,
-        "original_model_na": ORIGINAL_MODEL_NA,
         "notes": NOTES,
         "claim_limits": CLAIM_LIMITS,
         "not_evaluated": NOT_EVALUATED,
@@ -1120,10 +1120,9 @@
         runs=runs,
         equivalence=equivalence,
         latency_rows=latency_rows,
         latency_deltas=latency_deltas,
         conditions_summary=summary,
-        original_model_na=ORIGINAL_MODEL_NA,
         notes=NOTES,
         claim_limits=CLAIM_LIMITS,
         not_evaluated=NOT_EVALUATED,
     )
```

## Where each change lands

| # | Location (new line) | Change |
|---|---|---|
| 1 | 30–35 docstring | scope note now states per-flow 32/16 and two-flow 64/32 |
| 2 | 70–76 | `VAR_COUNT` documented as two-flow total; new `VAR_COUNT_PER_FLOW = value // 2` |
| 3 | 108–115 | whole `ORIGINAL_MODEL_NA` list **deleted** |
| 4 | 125–129 | `OTHER_REGION_EVENTS = ("wcount","rblock_begin","rblock_end")` added next to `KNOWN_REGIONS` |
| 5 | 136–144 | `NOTES` var wording fixed; NA note replaced by the precise `other`-region rule; per-placement observation note kept |
| 6 | 362–364 | region gate: `other` tolerated **only** for `OTHER_REGION_EVENTS`; everything else still unknown |
| 7 | 599–601 | per-flow var check → `VAR_COUNT_PER_FLOW[prefix]` (32 pre / 16 post) |
| 8 | 638–641 | **new aggregate check** `csv var total == VAR_COUNT[prefix]` (64 / 32 across both flows) |
| 9 | 727–729 | `write_md` signature: `original_model_na` parameter removed (call site updated ⇒ interface still consistent) |
| 10 | 787–791 | md section "counts the original modelling recorded as NA …" removed |
| 11 | 855 | md column renamed `var (IOMMU ingress, per flow)` |
| 12 | 1109 | `report["original_model_na"]` key removed |
| 13 | 1125 | `write_md(... original_model_na=...)` kwarg removed |

Unchanged on purpose: `NATURAL-JSON var == VAR_COUNT[prefix]` (line ~500, two-flow total), `PLACEMENT_METRICS_OBSERVED`, the byte-equality cross-checks, the `not csvd["unknown_regions"]` condition itself, and `sys.exit(0 if overall_ok else 1)`.

## Acceptance checks (no DUT re-run needed)

1. `python3 out/fragment-placement-20261008/placement_summary.py; echo $?` → prints `overall_ok: true`, `passed: 8`, `non_pass: 0`, `conditions_failed: 0`, `conditions_total: 902`, exit `0`.
2. `python3 -c "import json;d=json.load(open('out/fragment-placement-20261008/analysis.json'));print('original_model_na' in d);print(sorted({r['csv']['var_counts'] for r in d['runs'].values()}))"` → `False`, `[{'asrc': 16, 'vsrc': 16}, {'asrc': 32, 'vsrc': 32}]`.
3. Each run's conditions contain `"no unknown csv region"` ok `true`, `"<src>: var count == 32 per flow …"` / `== 16 per flow …` ok `true`, and a new `"csv var total == 64 (IOMMU ingress, both flows)"` / `== 32 …` ok `true`.
4. Negative boundary preserved: a synthetic row `var,other,0,0x0,1,0` must still yield `no unknown csv region` = false (only `wcount`/`rblock_begin`/`rblock_end` with `other` are excused).
5. `grep -n "ORIGINAL_MODEL_NA\|original_model_na\|original modelling" out/fragment-placement-20261008/placement_summary.py` → no hits; `grep -n "counts the original modelling" analysis.md` → no hits; `## observed placement metrics` still present in `analysis.md`.

## Assumptions / notes

- "`other` region is only legal for those three events" was verified against `analysis.json`: `unknown_events == []`, `unknown_regions == ["other"]`, and the four `other`-bearing event totals (`70+4+4`) are exactly the bookkeeping events — no source/destination event appears with `other`. The gate is per `(event, region)`, so an `other` on `var`/`par`/`vaw`/`paw`/`vw`/`pw`/`rfirst`/`rlast` still fails.
- `VAR_COUNT` is left as the two-flow total so `NATURAL-JSON var` (64/32) and the new total check stay correct; only the per-flow expectation is halved.
- The `csv var total` check sums the whole `csvd["var"]` mapping; for these CSVs that is exactly `vsrc` + `asrc`.
- `analysis.json` currently on disk is the stale, failing artefact — re-running the script overwrites it (and `analysis.md`); nothing else in the directory is written.
- The `~7287-cycle` figure in `CLAIM_LIMITS` is untouched (not part of the assigned fix), but it is a text constant from the earlier summary — flagging it for Codex review rather than editing.