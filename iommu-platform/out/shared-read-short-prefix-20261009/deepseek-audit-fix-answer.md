以下是仅 `audit75.py` 的 focused unified diff 提案。未执行、未修改文件；不能声称已通过。

```diff
--- a/out/shared-read-short-prefix-20261009/audit75.py
+++ b/out/shared-read-short-prefix-20261009/audit75.py
@@ -48,6 +48,11 @@
 NEW_BUILDS = {"build02-short0", "build02-short1"}
 
 IGNORED_ARGV_KNOBS = {"natural-trace", "binary"}
+
+# Actual CSV source trace has par/rfirst/rlast boundary rows only, no per-beat
+# pr rows.  The physical AXI ID owner bit (ID bit 2 in this fixture: id 33=V,
+# id 37=A) distinguishes V and A even though both are in the vsrc region.
+OWNER_BIT = 2
+SRC_PA_BASE = 0x80000000
 
 
 def h(p):
@@ -111,6 +116,105 @@
 def parse_int(x):
     s = str(x).strip()
     if s.startswith(("0x", "0X")):
         return int(s, 16)
     try:
         return int(s, 0)
     except ValueError:
         return int(s, 16)
 
 
+def owner_is_a(id_value):
+    return bool((int(id_value) >> OWNER_BIT) & 1)
+
+
+def _row_id(x):
+    return parse_int(x["id"])
+
+
+def _row_addr(x):
+    return parse_int(x["addr"])
+
+
+def _row_len_plus1(x):
+    return parse_int(x["len"]) + 1
+
+
+def _row_cycle(x):
+    return parse_int(x.get("cycle", "0")) if "cycle" in x else 0
+
+
+def _group_rows_by_id(rows):
+    out = {}
+    for x in rows:
+        out.setdefault(_row_id(x), []).append(x)
+    return out
+
+
+def match_source_bursts(par_rows, rfirst_rows, rlast_rows):
+    """Match accepted source PAR bursts to real rfirst/rlast completions.
+
+    The actual CSV has no per-beat pr/prlast rows.  It has one PAR row at
+    request accept time and one rfirst/rlast pair per accepted source burst.
+    Match by physical owner ID, address and length, preserving per-ID order;
+    rlast must be the terminal address of the same burst.
+    """
+    par_by_id = _group_rows_by_id(par_rows)
+    rf_by_id = _group_rows_by_id(rfirst_rows)
+    rl_by_id = _group_rows_by_id(rlast_rows)
+    problems = []
+    matched = []
+
+    all_ids = sorted(set(par_by_id) | set(rf_by_id) | set(rl_by_id))
+    for idv in all_ids:
+        pars = sorted(par_by_id.get(idv, []), key=_row_cycle)
+        rfs = sorted(rf_by_id.get(idv, []), key=_row_cycle)
+        rls = sorted(rl_by_id.get(idv, []), key=_row_cycle)
+        if len(pars) != len(rfs):
+            problems.append(
+                {
+                    "id": idv,
+                    "kind": "par_rfirst_count",
+                    "par": len(pars),
+                    "rfirst": len(rfs),
+                }
+            )
+        if len(pars) != len(rls):
+            problems.append(
+                {
+                    "id": idv,
+                    "kind": "par_rlast_count",
+                    "par": len(pars),
+                    "rlast": len(rls),
+                }
+            )
+        for i, p in enumerate(pars):
+            if i >= len(rfs) or i >= len(rls):
+                break
+            rf = rfs[i]
+            rl = rls[i]
+            if _row_addr(rf) != _row_addr(p) or parse_int(rf["len"]) != parse_int(p["len"]):
+                problems.append(
+                    {
+                        "id": idv,
+                        "kind": "rfirst_mismatch",
+                        "par": {"addr": _row_addr(p), "len": parse_int(p["len"])},
+                        "rfirst": {"addr": _row_addr(rf), "len": parse_int(rf["len"])},
+                    }
+                )
+            expected_rlast_addr = _row_addr(p) + parse_int(p["len"]) * 8
+            if _row_addr(rl) != expected_rlast_addr or parse_int(rl["len"]) != parse_int(p["len"]):
+                problems.append(
+                    {
+                        "id": idv,
+                        "kind": "rlast_mismatch",
+                        "par": {"addr": _row_addr(p), "len": parse_int(p["len"])},
+                        "rlast": {"addr": _row_addr(rl), "len": parse_int(rl["len"])},
+                        "expected_rlast_addr": expected_rlast_addr,
+                    }
+                )
+            matched.append(
+                {
+                    "id": idv,
+                    "owner": "A" if owner_is_a(idv) else "V",
+                    "addr": _row_addr(p),
+                    "len": parse_int(p["len"]),
+                    "len_plus1": _row_len_plus1(p),
+                    "rfirst_addr": _row_addr(rf),
+                    "rlast_addr": _row_addr(rl),
+                }
+            )
+    return {"ok": not problems, "problems": problems, "matched": matched}
+
+
 SHORT75_RE = re.compile(
@@ -199,6 +303,15 @@
 def parse_natural_json_text(text):
     m = NATURAL_JSON_RE.search(text)
     if not m:
         return None
     return json.loads(m.group(1))
 
 
+LOG_JSON_RE = re.compile(r"^\s*([A-Za-z0-9_.-]+-JSON)\s+(\{.*\})\s*$", re.M)
+
+
+def parse_log_json_objects(text):
+    out = {}
+    for m in LOG_JSON_RE.finditer(text):
+        out[m.group(1)] = json.loads(m.group(2))
+    return out
+
+
 def csv_summary(rows):
@@ -206,28 +319,31 @@
 def csv_summary(rows):
     def ev(x):
         return x.get("event", "")
 
     def reg(x):
         return x.get("region", "")
 
     targets = {
         r: sum(1 for x in rows if ev(x) in ("pw", "pwlast") and reg(x) == r)
         for r in ("vdst", "adst")
     }
     par_all = [x for x in rows if ev(x) == "par"]
-    par_src = [x for x in par_all if reg(x) in ("vsrc", "asrc")]
+    par_src = [x for x in par_all if reg(x) == "vsrc"]
     par_len_plus1 = sum(parse_int(x["len"]) + 1 for x in par_src)
     source_par_bursts = len(par_src)
     all_par_bursts = len(par_all)
-    pr_rows = [x for x in rows if ev(x) == "pr"]
-    prlast_rows = [x for x in rows if ev(x) == "prlast"]
-    pr_beats = len(pr_rows) + len(prlast_rows)
+    rfirst_src = [x for x in rows if ev(x) == "rfirst" and reg(x) == "vsrc"]
+    rlast_src = [x for x in rows if ev(x) == "rlast" and reg(x) == "vsrc"]
+    source_burst_match = match_source_bursts(par_src, rfirst_src, rlast_src)
     zero_len_par = sum(1 for x in par_all if parse_int(x["len"]) + 1 == 0)
     return {
         "target_W": targets,
         "par_len_plus1": par_len_plus1,
         "source_par_bursts": source_par_bursts,
         "all_par_bursts": all_par_bursts,
-        "pr_beats": pr_beats,
+        "rfirst_bursts": len(rfirst_src),
+        "rlast_bursts": len(rlast_src),
+        "source_burst_match": source_burst_match,
         "zero_len_par": zero_len_par,
     }
@@ -236,33 +352,61 @@
-def short_prefix_csv_checks(rows, J):
-    par = [x for x in rows if x.get("event") == "par" and x.get("region") == "asrc"]
+def short_prefix_csv_checks(rows, obs):
+    J = obs["J"]
+    E = obs.get("E")
+    par = [x for x in rows if x.get("event") == "par" and x.get("region") == "vsrc"]
     if not par:
-        return {"ok": False, "reason": "no asrc par rows"}
-
-    def addr(x):
-        return parse_int(x["addr"])
-
-    def cyc(x):
-        return parse_int(x.get("cycle", "0")) if "cycle" in x else 0
-
-    par_sorted = sorted(par, key=cyc)
-    start = min(addr(x) for x in par)
-    first = par_sorted[0]
-    first_len_plus1 = parse_int(first["len"]) + 1
-    first_addr = addr(first)
-    fallback_offset_present = any(addr(x) == start + J * 8 for x in par)
-    zero_len = any(parse_int(x["len"]) + 1 == 0 for x in par)
-    ok = (
-        first_len_plus1 == J
-        and first_addr == start
-        and fallback_offset_present
-        and not zero_len
-    )
+        return {"ok": False, "reason": "no vsrc par rows"}
+
+    v_par = [x for x in par if not owner_is_a(_row_id(x))]
+    a_par = [x for x in par if owner_is_a(_row_id(x))]
+    if not v_par:
+        return {"ok": False, "reason": "no V-owner par rows"}
+    if not a_par:
+        return {"ok": False, "reason": "no A-owner par rows"}
+
+    v_first = min(v_par, key=_row_cycle)
+    base = _row_addr(v_first)
+    base_is_fixture = base == SRC_PA_BASE
+
+    a_first = min(a_par, key=_row_cycle)
+    first_addr = _row_addr(a_first)
+    first_len_plus1 = _row_len_plus1(a_first)
+    first_uses_a_owner = owner_is_a(_row_id(a_first))
+
+    fallback_addr = base + J * 8
+    fallback_rows = [x for x in a_par if _row_addr(x) == fallback_addr]
+    fallback_offset_present = bool(fallback_rows)
+    fallback_len_plus1 = _row_len_plus1(fallback_rows[0]) if fallback_rows else None
+
+    zero_len = any(_row_len_plus1(x) == 0 for x in par)
+    ok = (
+        base_is_fixture
+        and first_uses_a_owner
+        and first_len_plus1 == J
+        and first_addr == base
+        and fallback_offset_present
+        and not zero_len
+    )
     return {
         "ok": ok,
-        "source_start": start,
+        "base": base,
+        "base_is_fixture": base_is_fixture,
+        "owner_bit": OWNER_BIT,
+        "v_owner_id": _row_id(v_first),
+        "a_owner_id": _row_id(a_first),
+        "source_start": base,
         "first_addr": first_addr,
         "first_len_plus1": first_len_plus1,
+        "E": E,
+        "fallback_addr": fallback_addr,
+        "fallback_len_plus1": fallback_len_plus1,
         "fallback_offset_present": fallback_offset_present,
         "zero_len": zero_len,
-        "asrc_par_count": len(par),
+        "vsrc_par_count": len(par),
+        "v_par_count": len(v_par),
+        "a_par_count": len(a_par),
     }
@@ -271,13 +425,24 @@
+def argv_mdir_obj(argv):
+    for i, x in enumerate(argv):
+        if x == "--Mdir" and i + 1 < len(argv):
+            return Path(argv[i + 1]).name
+    return None
+
+
 def build_from_result_or_argv(z, argv):
     if "build" in z:
         return z["build"]
-    p = Path(argv[0]).parent.name
-    if p == "obj-short0":
+    obj = argv_mdir_obj(argv)
+    if obj is None and argv and Path(argv[0]).name != "verilator":
+        obj = Path(argv[0]).parent.name
+    if obj == "obj-short0":
         return "build-short0"
-    if p == "obj-short1":
+    if obj == "obj-short1":
         return "build-short1"
-    raise AssertionError(f"cannot infer build from argv[0]={argv[0]!r}")
+    raise AssertionError(f"cannot infer build from argv={argv!r} (obj={obj!r})")
@@ -316,6 +486,7 @@
     z = j(rp)
     a = j(ap)
     log_text = lp.read_text() if lp.exists() else ""
     obs = parse_short75_log(lp) or parse_short75_result(z)
     sr50 = parse_sr50_text(log_text)
     qw50 = parse_qw50_totals(log_text)
     nat_log = parse_natural_json_text(log_text)
+    log_json = parse_log_json_objects(log_text)
     rows = read_csv(cp) if cp.exists() else []
     csvs = csv_summary(rows) if rows else None
     build = build_from_result_or_argv(z, a)
@@ -326,6 +497,13 @@
     short_prefix = int(z.get("short_prefix", 0))
     exit_code = z.get("exit_code")
 
+    if "metrics" in z:
+        _m = z["metrics"]
+        for key, val in log_json.items():
+            assert key in _m, (n, key, sorted(_m))
+            assert norm_metrics(_m[key]) == val, (n, key, _m[key], val)
+
     base = {
@@ -340,6 +518,7 @@
         "observer": obs,
         "sr50": sr50,
         "qw50_totals": qw50,
+        "log_json_keys": sorted(log_json),
     }
@@ -360,12 +539,27 @@
         assert csvs["target_W"] == {"vdst": 256, "adst": 256}, (n, csvs["target_W"])
         assert csvs["zero_len_par"] == 0, (n, csvs["zero_len_par"])
+        assert csvs["source_burst_match"]["ok"], (n, csvs["source_burst_match"])
+        assert csvs["rfirst_bursts"] == csvs["source_par_bursts"] == csvs["rlast_bursts"], (
+            n,
+            csvs["source_par_bursts"],
+            csvs["rfirst_bursts"],
+            csvs["rlast_bursts"],
+        )
 
         actual_r = natural["v_src_r_beats"] + natural["a_src_r_beats"]
-        assert actual_r == csvs["par_len_plus1"] == csvs["pr_beats"], (
+        nat_pr_beats = natural.get("nat_pr_beats")
+        if nat_pr_beats is not None:
+            assert nat_pr_beats == actual_r, (n, nat_pr_beats, actual_r)
+        assert actual_r == csvs["par_len_plus1"], (
             n,
             actual_r,
             csvs["par_len_plus1"],
-            csvs["pr_beats"],
+            csvs["source_burst_match"],
         )
@@ -393,7 +597,7 @@
         assert_short_observer(n, short_prefix, obs)
         short_ok = None
         if short_prefix == 1:
-            short_ok = short_prefix_csv_checks(rows, obs["J"])
+            short_ok = short_prefix_csv_checks(rows, obs)
             assert short_ok["ok"], (n, obs, short_ok)
@@ -435,6 +639,7 @@
         if nat_log is not None:
             assert nat_log.get("completed") is False and nat_log.get("verified") is False, (n, nat_log)
         if csvs is not None and sr50 is not None:
+            assert csvs["target_W"] == {"vdst": 256, "adst": 256}, (n, csvs["target_W"])
             assert sr50["children"] == csvs["source_par_bursts"], (
                 n,
                 sr50["children"],
@@ -464,7 +669,7 @@
     co = z["metrics"]["SOURCE-SPONSOR-HANDOVER-JSON"]
     assert rep["decided"] == 1 and rep["share"] == 0, (new, rep)
-    assert co["actual_source_r_beats"] == 512, (new, co)
+    assert co["actual_source_r_beats"] == co["derived_expected_source_r_beats"] == 512, (new, co)
     repair_pairs.append(
@@ -510,6 +715,17 @@
 if not stage74_run08.exists():
     raise AssertionError(f"missing stage74 control {stage74_run08}")
 
+stage74_run08_csv = STAGE74 / "run08.csv"
+if not stage74_run08_csv.exists():
+    raise AssertionError(f"missing stage74 control CSV {stage74_run08_csv}")
+run29_csv = D / "run29.csv"
+assert run29_csv.exists(), f"missing run29 CSV {run29_csv}"
+assert h(run29_csv) == h(stage74_run08_csv), (
+    "run29 CSV not byte-identical to stage74 run08 CSV",
+    h(run29_csv),
+    h(stage74_run08_csv),
+)
+
 r29 = j(D / "run29-result.json")
 r74 = j(stage74_run08)
@@ -532,6 +748,9 @@
     "run29_build": r29.get("build"),
     "run29_prior_stage74": r29.get("prior_stage74"),
     "stage74_control": "run08",
+    "csv_byte_exact": True,
+    "run29_csv_sha256": h(run29_csv),
+    "stage74_run08_csv_sha256": h(stage74_run08_csv),
     "only_added_source_sponsor_fields": sorted(k.rsplit("/", 1)[1] for k in allowed_paths),
     "other_metric_diffs": bad_diffs,
@@ -565,7 +784,9 @@
     want_short = 1 if build.endswith("short1") else 0
     assert f"-GShortPrefix={want_short}" in ba, (build, ba)
-    assert Path(ba[0]).parent.name in ("obj-short0", "obj-short1"), (build, ba[0])
+    assert ba and ba[0] == "verilator", (build, ba[:1])
+    build_obj = argv_mdir_obj(ba)
+    assert build_obj in ("obj-short0", "obj-short1"), (build, ba)
 
     snap = D / snapname
@@ -580,13 +799,26 @@
-    files_present = all((snap / f).exists() for f in S + [EXE])
-    file_hash_mismatches = []
-    if files_present:
-        for f in S + [EXE]:
-            actual = h(snap / f)
-            if actual != manifest[f]:
-                file_hash_mismatches.append({"file": f, "actual": actual, "manifest": manifest[f]})
-    assert not file_hash_mismatches, (snapname, file_hash_mismatches)
+    snapshot_file_problems = []
+    for f in S + [EXE]:
+        fp = snap / f
+        if not fp.exists():
+            snapshot_file_problems.append({"file": f, "problem": "missing"})
+            continue
+        if fp.stat().st_size == 0:
+            snapshot_file_problems.append({"file": f, "problem": "empty"})
+            continue
+        actual = h(fp)
+        if actual != manifest[f]:
+            snapshot_file_problems.append(
+                {"file": f, "problem": "hash_mismatch", "actual": actual, "manifest": manifest[f]}
+            )
+    assert not snapshot_file_problems, (snapname, snapshot_file_problems)
+    files_present = True
+    file_hash_mismatches = [
+        p for p in snapshot_file_problems if p.get("problem") == "hash_mismatch"
+    ]
@@ -641,6 +873,16 @@
 assert isinstance(cli, list) and len(cli) == 7, len(cli)
 for e in cli:
     assert e.get("exit_code") == 2, e
     assert e.get("expect") == 2, e
+for idx, e in enumerate(cli, 1):
+    lp = D / f"cli{idx:02d}.log"
+    assert lp.exists(), (idx, lp)
+    text = lp.read_text()
+    assert "nonblock_actual_dma_test:" in text, (idx, text)
+    assert "+cutover-after-rv" in text, (idx, text)
+    assert "PREP4-DECL" not in text, (idx, "CLI failure log contains DUT init evidence")
+    assert "SHORT75-OBS" not in text, (idx, "CLI failure log contains DUT init evidence")
+    assert "nonblock_actual_dma_test: mode=" not in text, (idx, "CLI failure log contains DUT init evidence")
 cli_invalid = {
@@ -686,6 +928,12 @@
         "impossibility claim."
     ),
+    "csv_beat_count_basis": (
+        "CSV contains par/rfirst/rlast boundary rows only, not per-beat pr rows. "
+        "R beat totals come from DUT/harness per-R handshake counters; CSV is used "
+        "to verify accepted source bursts have real rfirst/rlast completions and "
+        "that accepted PAR length sums match those counters."
+    ),
     "authored_by": "DeepSeek (proposal)",
@@ -763,10 +1011,20 @@
     "- SHORT75-OBS was parsed per run. J is the initial observation; terminal "
     "cutover_vcut may differ and is not conflated with J.",
-    "- For short-prefix runs, the first actual A-source AR len+1 equals J and "
-    "address source start; normal fallback first offset equals J; no zero-length AR.",
-    "- Timeout fallback may continue to N rather than stopping at E; source/actualAR "
-    "were used to distinguish this.",
+    "- CSV source trace has par/rfirst/rlast rows only; it has no per-beat pr rows. "
+    "The script matches each accepted source PAR burst to a real rfirst/rlast by "
+    "physical owner ID, address and length, preserving per-ID order. R beat totals "
+    "are checked against DUT/harness per-R handshake counters, not inferred from "
+    "per-beat CSV rows.",
+    "- A-source bursts are identified by the AXI ID owner bit (ID bit 2 in this "
+    "fixture: id 33=V, id 37=A), not by an asrc CSV region. The immutable source "
+    "PA base is taken from the first V rfirst/par and asserted equal to 0x80000000. "
+    "For short-prefix runs, first A addr==base and len+1==J; the next fallback A "
+    "burst starts at base+J*8. The fallback length may be E-J or N-J depending on "
+    "timeout and is recorded, not over-claimed.",
+    "- Shared suffix A bursts also carry the A owner ID and are matched as A. The "
+    "initial repair burst is not counted as the shared suffix fallback.",
     "- opportunities=0 is an observation limited to this fixture, not a theoretical "
     "impossibility proof.",
```

修订理由摘要：

- 初稿失败在 `csvs["pr_beats"]`：真实 CSV 没有 `pr/prlast` 逐拍行，只有 `par/rfirst/rlast`。已删除 `pr_beats` 断言，改为按 owner ID + address + length、同 ID 内顺序匹配 `par ↔ rfirst ↔ rlast`，并检查每个 accepted source burst 都有真实完成。
- 用 AXI ID owner bit（bit 2）区分 V/A，避免把 A shared PA repair 误当 `asrc`；样例 id 33=V、id 37=A 与该规则一致。
- A first AR：从实际 V first row 取 base，assert `base == 0x80000000`；A first `addr == base`、`len+1 == J`；fallback `addr == base + J*8`。fallback 长度只记录，不硬编码 `E-J`/`N-J`。
- build：`argv[0]` 实际是 `verilator`，obj 名改为从 `--Mdir` 下一项取；DUT argv 仍可用 exe parent。
- snapshot：四个 snapshot 的 9 个文件必须存在、非空、逐文件 hash 匹配 manifest，missing/empty/hash mismatch 都 fail。
- log JSON：新增 `parse_log_json_objects`，有 `metrics` 的 run 要求 log 中所有 `*-JSON` 与 `result.metrics` 吻合；run17/21 无 metrics 时仍允许从 log 解析 NATURAL。
- 旧失败 run17/21：保留非 full-PASS 语义，增加旧 CSV W 各 256 检查。
- repair 22/23：加强为 `actual_source_r_beats == derived_expected_source_r_beats == 512`。
- stage74 run08 vs run29：增加 CSV 逐字 hash 相等检查；原四 SOURCE 字段 metrics diff 白名单保留。
- CLI7：检查实际 exit 2 错误日志，且日志中无 `PREP4-DECL`/`SHORT75-OBS`/`mode=` 等 DUT 初始化证据；不自动假设后续扩展参数。
- markdown 输出：明确 CSV 只有 boundary rows，不写存在 pr 逐拍 CSV；拍计数口径归 DUT/harness per-R handshake counter。

未解决/需 Codex 确认：

- `natural.get("nat_pr_beats")` 是防御性检查；若真实 metric 名称或层级不同，需要按实际 `result.metrics` 调整。
- fallback 长度 `E-J` 还是 `N-J` 未硬断言，因为脚本内没有可靠 `N` 来源；当前只记录 `E` 与 `fallback_len_plus1`。
- 没有硬编码 `33/37` ID，只使用 owner bit 规则；若该 fixture 固定必须断言精确 ID，可再加 fixture-ID assert。
- `rlast` 地址按 `par_addr + len*8` 校验；若 RTL 终端地址语义不是该公式，需要修正。
- 未执行脚本；以上只是 focused diff 提案。