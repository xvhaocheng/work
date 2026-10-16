# Findings

The failing check is the **PASS-branch QW50 assertion** at `audit75.py:558`:

```python
assert qw50 and all(t == 256 for t in qw50), (n, qw50)   # run11 -> [256,255]
```

This is wrong for a cross-page A job. `q_write_splitter` prints `total_q` at `q_write_splitter.sv:152`, and `total_q` is loaded from **each accepted raw parent AW** (`s_req_i.aw.len+1`, `q_write_splitter.sv:85-95`). It is the *current/terminal raw parent*, not the whole-job beat count. For the A instance the job is pre-split by the `+a-dst-offset=4088` page boundary into a first raw parent of `F=(4096-4088)/8 = 1` beat, then a terminal raw parent of `256-1 = 255`. The V instance is never split (`dst_offset=0`), so it stays `256`. So `run11`’s `[256,255]` is the DUT behaving correctly; the audit’s expectation was wrong. `run12` (same `a-dst-offset=4088`, `first_a_page_beats=1`, `a_first_raw_parent_bytes_expected=8`, `a_cross_page=1`) is the same geometry.

The NATURAL geometry fields confirm this (`run11-result.json:25,30,32,33`): `a_dst_offset=4088`, `a_cross_page=1`, `first_a_page_beats=1`, `a_first_raw_parent_bytes_expected=8`.

# Scope of the fix (why this is correct)
- Derive the expected **terminal A parent** from the *real* argv `+a-dst-offset`, not from a range: `offset==0` (or job fits in one page) ⇒ `256`; page-split ⇒ `256 - (4096-offset)/8`. Batch `4088` ⇒ `255`.
- V is always `256` (un-split).
- Assert exact two-element list in `q_write_splitter` `final` order `[V, A]` (run11 log order is `[256,255]`, i.e. V prints first), and cross-check the NATURAL geometry fields for agreement.
- Guards are kept/strengthened, never relaxed or deleted.
- Unrelated guards (two CSVs each `target_W == {vdst:256, adst:256}`, `logical bytes == 2048`, `sr50 returned_v/returned_a == 256`) are untouched.

# Focused diff (`out/shared-read-short-prefix-20261009/audit75.py`)

```diff
@@ -304,6 +304,27 @@ def parse_qw50_totals(text):
 def parse_qw50_totals(text):
     return [int(x) for x in QW50_RE.findall(text)]
 
 
+def qw50_expected_totals(a_dst_offset, beats=256, page_bytes=4096, beat_bytes=8):
+    """Expected QW50 `total` per q_write_splitter, in final-$display order [V, A].
+
+    QW50 `total` is q_write_splitter total_q = the *terminal raw parent AW*
+    length, not the whole-job beat count (q_write_splitter.sv:152 prints
+    total_q, and total_q is loaded from each accepted raw parent AW at
+    q_write_splitter.sv:85-95).  V is never page-split -> 256.  A is pre-split
+    at the +a-dst-offset 4096-byte page boundary: first raw parent
+    F=(page_bytes-a_dst_offset)/8 beats, then a terminal parent of beats-F.
+    If the job does not reach the page boundary A is one whole-job parent -> 256.
+    This batch a-dst-offset=4088 -> F=1 -> [256, 255].
+    Returns (expected totals, F); F==0 means no cross-page split.
+    """
+    if a_dst_offset == 0 or a_dst_offset + beats * beat_bytes <= page_bytes:
+        return [beats, beats], 0
+    rem = page_bytes - a_dst_offset
+    assert rem % beat_bytes == 0, (a_dst_offset, rem)
+    f = rem // beat_bytes
+    assert 0 < f < beats, (a_dst_offset, f)
+    return [beats, beats - f], f
+
+
 def parse_natural_json_text(text):
     m = NATURAL_JSON_RE.search(text)
     if not m:
@@ -555,7 +576,24 @@
             csvs["source_par_bursts"],
         )
         assert sr50["returned_v"] == 256 and sr50["returned_a"] == 256, (n, sr50)
-        assert qw50 and all(t == 256 for t in qw50), (n, qw50)
+        # QW50 total is the terminal raw parent AW length, not the whole job.
+        # Derive the expected last-A-parent length from the real +a-dst-offset
+        # geometry (offset0 -> 256; page-split F=(4096-offset)/8 -> 256-F) and
+        # cross-check the NATURAL geometry fields.
+        a_dst_offset = parse_int(knobs(a).get("a-dst-offset", "0"))
+        want_qw50, qw_f = qw50_expected_totals(a_dst_offset)
+        assert len(qw50) == 2, (n, qw50)
+        assert qw50 == want_qw50, (n, qw50, want_qw50, a_dst_offset)
+        assert natural["a_dst_offset"] == a_dst_offset, (
+            n,
+            natural["a_dst_offset"],
+            a_dst_offset,
+        )
+        if qw_f:
+            assert natural["a_cross_page"] == 1, (n, natural)
+            assert natural["first_a_page_beats"] == qw_f, (n, natural["first_a_page_beats"], qw_f)
+            assert natural["a_first_raw_parent_bytes_expected"] == qw_f * 8, (n, natural)
+        else:
+            assert natural.get("a_cross_page", 0) == 0, (n, natural)
 
         assert "route not bound" not in log_text, n
         assert "SCENARIO-FAIL" not in log_text, n
@@ -600,7 +648,12 @@
         assert "route not bound" in log_text, n
         assert sr50 is not None, (n, "missing SR50 in expected fail")
         assert sr50["returned_v"] == 256 and sr50["returned_a"] == 256, (n, sr50)
-        assert qw50 and all(t == 256 for t in qw50), (n, qw50)
+        # run17/run21 are a-dst-offset==0 (no page split): both terminal raw
+        # parents are the whole-job 256.  Tightens the old all(t == 256) form
+        # to an exact two-element list (still not a range; guard not removed).
+        fail_offset = parse_int(knobs(a).get("a-dst-offset", "0"))
+        want_qw50, _ = qw50_expected_totals(fail_offset)
+        assert qw50 == want_qw50 == [256, 256], (n, qw50, want_qw50, fail_offset)
         if nat_log is not None:
             assert nat_log.get("completed") is False and nat_log.get("verified") is False, (n, nat_log)
```

# Assumptions / unresolved
- **Instance order**: I use the `final $display` order `[V=256, A=256-F]`, evidenced by `run11` producing `[256,255]` (V un-split prints first). If the top level is not guaranteed to emit V before A, replace the positional `qw50 == want_qw50` with an order-independent exact multiset `sorted(qw50) == sorted(want_qw50)` (still exact, still not a range) — but that would no longer catch a V/A swap. `QW66-OBS instance=%m` (`q_write_splitter.sv:153`) is available if you prefer explicit per-instance attribution.
- I could not see `multi_system_top.sv`, so the V/A naming/order is inferred from `run11`, not from the instantiation.
- `run17/run21` fail-branch expectation `[256,256]` is taken as given by the request; the old `all(t==256)` never actually executed (the script aborted at `run11` first, per `audit75-parent-scope-failure.log:2-4`), so this is an assumption to confirm on the next run.
- The `else`-branch uses `natural.get("a_cross_page", 0)` for the no-split case only, to avoid a KeyError on offset-0 runs whose NATURAL may omit split-only fields; split-case fields are asserted directly.

# Suggested acceptance checks (parent to execute; I ran nothing)
1. `python3 audit75.py` completes with no assertion; `run11`/`run12` PASS with `qw50_totals == [256, 255]`; V total `256`; `natural.first_a_page_beats == 1`, `a_first_raw_parent_bytes_expected == 8`, `a_cross_page == 1` verified.
2. `run17`/`run21` still `EXPECTED_FAIL_ROUTE_NOT_BOUND` with `qw50 == [256, 256]`.
3. Unchanged: both CSVs `target_W == {"vdst": 256, "adst": 256}`, share bytes `2048`×4, `sr50.returned_v == returned_a == 256`.
4. Negative control: temporarily flip one QW50 value to ensure the new assertion still fires (guard not weakened).

No tests were run by me; the above diff is a proposal only.