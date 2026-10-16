```diff
--- a/out/posttl-independent-20261008/analyze.py
+++ b/out/posttl-independent-20261008/analyze.py
@@ -132,30 +132,14 @@
 def discover(directory: Path, runs):
     """-> {run: {'log':Path|None, 'csv':Path|None, 'command':.., 'result':..}}"""
-    by_longest = sorted(runs, key=len, reverse=True)
-    found = {r: {k: None for k in KIND_PATTERNS} for r in runs}
-    unknown = []
-    for p in sorted(directory.iterdir()):
-        if not p.is_file():
-            continue
-        hit = None
-        for r in by_longest:
-            if p.name == r:
-                hit = (r, "")
-                break
-            if p.name.startswith(r):
-                hit = (r, p.name[len(r):])
-                break
-        if hit is None:
-            unknown.append(p.name)
-            continue
-        run, rest = hit
-        placed = False
-        for kind, suffixes in KIND_PATTERNS.items():
-            if rest in suffixes:
-                if found[run][kind] is None:
-                    found[run][kind] = p
-                placed = True
-                break
-        if not placed:
-            unknown.append(p.name)
-    return found, unknown
+    found = {}
+    for r in runs:
+        found[r] = {
+            "log": directory / f"{r}.log",
+            "csv": directory / f"{r}.csv",
+            "command": directory / f"{r}-command.json",
+            "result": directory / f"{r}-result.json",
+        }
+    return found, []
@@ -233,6 +217,15 @@
 def read_text(path):
     if path is None:
         return None
     return path.read_text(encoding="utf-8", errors="replace")
 
 
+def read_json(path):
+    if path is None or not path.exists():
+        return None
+    try:
+        return json.loads(path.read_text(encoding="utf-8"))
+    except json.JSONDecodeError as exc:
+        raise ParseFailure(f"{path}: invalid JSON: {exc}") from exc
+
+
 # ======================================================================== #
@@ -272,6 +265,25 @@
 def cover_ok(sub, base):
     cur = base
     for r in sub:
         if r["addr"] != cur:
             return False, (f"expected 0x{cur:08x} got 0x{r['addr']:08x} "
                            f"at line {r['line']}")
         cur += (r["len"] + 1) * BEAT_BYTES
     return True, f"covered [0x{base:08x}, 0x{cur:08x})"
 
 
+def cover_beats_ok(sub, base):
+    """pw/pwlast rows are one 8-byte data beat each; len is the whole AW length."""
+    cur = base
+    for r in sub:
+        if r["addr"] != cur:
+            return False, (f"expected 0x{cur:08x} got 0x{r['addr']:08x} "
+                           f"at line {r['line']}")
+        cur += BEAT_BYTES
+    return True, f"covered {len(sub)} beats [0x{base:08x}, 0x{cur:08x})"
+
+
 def no_cross(sub, base, gran):
     for r in sub:
         sz = (r["len"] + 1) * BEAT_BYTES
@@ -282,6 +294,16 @@
     return True, ""
 
 
+def no_cross_beat(sub):
+    """A W data beat is fixed 8 B; page decision uses absolute addr//4096."""
+    for r in sub:
+        if (r["addr"] // PAGE_BYTES) != ((r["addr"] + BEAT_BYTES - 1) // PAGE_BYTES):
+            return False, (f"line {r['line']} 0x{r['addr']:x}+8 crosses a "
+                           f"{PAGE_BYTES}-byte page")
+    return True, ""
+
+
 # ======================================================================== #
@@ -306,11 +328,9 @@
-    log_text = read_text(artefacts.get("log"))
-    cmd_text = read_text(artefacts.get("command"))
-    res_text = read_text(artefacts.get("result"))
-    if cmd_text:
-        out["command"] = [l for l in cmd_text.strip().splitlines() if l][:3]
+    log_text = read_text(artefacts.get("log"))
+    cmd_json = read_json(artefacts.get("command"))
+    res_json = read_json(artefacts.get("result"))
+    if cmd_json is not None:
+        out["command"] = cmd_json
@@ -319,23 +339,63 @@
-    # ---------------- exit 0 / PASS --------------------------------
+    # ---------------- log marker (diagnostic only) -----------------
     if log_text is not None:
         C("log PASS marker (and no SCENARIO-FAIL)",
           "nonblock_actual_dma_test: PASS" in log_text and "SCENARIO-FAIL" not in log_text,
           "PASS present" if "nonblock_actual_dma_test: PASS" in log_text else "PASS absent")
         log = parse_log(log_text)
     else:
         log = {"pass": False, "fail": True, "children": [], "stats": None,
                "natural_json": None, "natural_summary": None}
 
-    if res_text is not None:
-        m = re.search(r"(?:exit|rc|status)\s*[=:]\s*(-?\d+)", res_text)
-        if m:
-            C("result exit code == 0", int(m.group(1)) == 0, f"exit={m.group(1)}")
-        else:
-            ok = ("PASS" in res_text) or (res_text.strip() in ("0", ""))
-            C("result exit code == 0", ok, f"result text {res_text.strip()[:80]!r}")
-    else:
-        C("result exit code == 0",
-          log.get("pass", False),
-          "no result/exit file discovered; inferred from log PASS marker")
+    # ---------------- command JSON (real array) --------------------
+    if cmd_json is None:
+        C("command JSON parse", False, "missing or unparseable command JSON")
+    else:
+        C("command JSON parse", True, str(artefacts.get("command")))
+        exe = cmd_json[0] if cmd_json else None
+        out["binary"] = exe
+
+        def flag_value(flag):
+            for arg in cmd_json:
+                if isinstance(arg, str) and arg.startswith(flag + "="):
+                    return arg.split("=", 1)[1]
+            return None
+
+        def has_flag(flag):
+            return any(isinstance(arg, str) and (arg == flag or arg.startswith(flag + "="))
+                       for arg in cmd_json)
+
+        C("command binary present", isinstance(exe, str) and bool(exe), str(exe))
+        C("command +natural=1", flag_value("+natural") == "1",
+          f"natural={flag_value('+natural')!r}")
+        C("command +natural-bytes=32768", flag_value("+natural-bytes") == "32768",
+          f"natural-bytes={flag_value('+natural-bytes')!r}")
+        C("command +mode=s", flag_value("+mode") == "s",
+          f"mode={flag_value('+mode')!r}")
+        want_first = "v" if run.startswith("fixed-v") or run.startswith("dynamic-v") else "a"
+        C("command +natural-first matches run", flag_value("+natural-first") == want_first,
+          f"natural-first={flag_value('+natural-first')!r} want={want_first}")
+        want_lat = "100" if "lat100" in run else "31"
+        C("command +ptw-latency matches run", flag_value("+ptw-latency") == want_lat,
+          f"ptw-latency={flag_value('+ptw-latency')!r} want={want_lat}")
+        want_admit = "buffer" if is_dynamic else "none"
+        C("command +read-admit matches policy", flag_value("+read-admit") == want_admit,
+          f"read-admit={flag_value('+read-admit')!r} want={want_admit}")
+        C("command disables CPU/priority/fixtures",
+          not has_flag("+cpu-writes") and not has_flag("+ds-priority") and
+          not has_flag("+write-tail-pause") and not has_flag("+read-latency-model") and
+          not has_flag("+event-trace") and not has_flag("+owner-only") and
+          not has_flag("+v-cold-dst"),
+          "forbidden optional knobs present")
+
+    # ---------------- result exit code from real JSON --------------
+    if res_json is None:
+        C("result JSON parse", False, "missing or unparseable result JSON")
+        C("result exit_code == 0", False, "no result JSON")
+    else:
+        C("result JSON parse", True, str(artefacts.get("result")))
+        C("result exit_code == 0", res_json.get("exit_code") == 0,
+          f"exit_code={res_json.get('exit_code')!r}")
@@ -377,15 +437,14 @@
-    for region, flow in (("vsrc", 0), ("asrc", 1)):
+    for region in ("vsrc", "asrc"):
         sub = rows_of(rows, VIRT_EVENTS, region)
         if not sub:
             C(f"{region}: virtual rows present", False, "none")
             continue
-        lo_i, hi_i = VIRT_BASE[region], VIRT_BASE[region] + VIRT_SIZE[region]
-        lo_p, hi_p = PHYS_BASE[region], PHYS_BASE[region] + PHYS_SIZE[region]
-        ok = all(in_window(r["addr"], lo_i, VIRT_SIZE[region]) or
-                 in_window(r["addr"], lo_p, PHYS_SIZE[region]) for r in sub)
-        C(f"{region}: virtual addr within IOVA 0x{lo_i:08x} "
-          f"or PA 0x{lo_p:08x}", ok,
-          f"first=0x{sub[0]['addr']:08x} last=0x{sub[-1]['addr']:08x} "
-          f"(task text: 'virtualsrc 0x80000000/0x90000000')")
+        lo_i = VIRT_BASE[region]
+        ok = all(in_window(r["addr"], lo_i, VIRT_SIZE[region]) for r in sub)
+        C(f"{region}: virtual addr within IOVA 0x{lo_i:08x} (strict)",
+          ok,
+          f"first=0x{sub[0]['addr']:08x} last=0x{sub[-1]['addr']:08x}")
         out[f"{region}_first_virtual_addr"] = sub[0]["addr"]
@@ -420,8 +479,9 @@
-        ids = {r["id"] for r in sub}
-        C(f"{region}: par DDR id == 32", ids == {32}, f"ids={sorted(ids)}")
+        want_id = 32 if region == "vsrc" else 36
+        ids = {r["id"] for r in sub}
+        C(f"{region}: par DDR id == {want_id}", ids == {want_id}, f"ids={sorted(ids)}")
@@ -431,14 +491,20 @@
         sub = sorted(rows_of(rows, {"pw", "pwlast"}, region),
                      key=lambda r: (r["cycle"], r["line"]))
         out[f"{region}_pw_beats"] = len(sub)
         C(f"{region}: pw data beats == {JOB_BEATS}", len(sub) == JOB_BEATS,
           f"{len(sub)}")
         C(f"{region}: first pw addr == 0x{first:08x}",
           bool(sub) and sub[0]["addr"] == first,
           f"first={hex(sub[0]['addr']) if sub else None}")
-        ok, det = cover_ok(sub, first)
-        C(f"{region}: pw contiguous cover of destination", ok, det)
-        ok_4, det_4 = no_cross(sub, first, PAGE_BYTES)
-        C(f"{region}: pw beat does not cross a 4-KiB page", ok_4, det_4)
-        ids = {r["id"] for r in sub}
-        C(f"{region}: pw DDR id == 36", ids == {36}, f"ids={sorted(ids)}")
+        ok, det = cover_beats_ok(sub, first)
+        C(f"{region}: pw contiguous 1-beat cover of destination", ok, det)
+        ok_4, det_4 = no_cross_beat(sub)
+        C(f"{region}: pw beat does not cross a 4-KiB page (absolute)", ok_4, det_4)
+        want_id = 32 if region == "vdst" else 36
+        ids = {r["id"] for r in sub}
+        C(f"{region}: pw DDR id == {want_id}", ids == {want_id}, f"ids={sorted(ids)}")
         C(f"{region}: pwlast present", any(r["event"] == "pwlast" for r in sub), "")
+        aw = sorted(rows_of(rows, {"paw"}, region),
+                    key=lambda r: (r["cycle"], r["line"]))
+        C(f"{region}: paw DDR id == {want_id}",
+          {r["id"] for r in aw} == {want_id},
+          f"ids={sorted({r['id'] for r in aw})}")
@@ -457,8 +523,9 @@
-        ids = {r["id"] for r in rl}
-        C(f"{region}: R DDR id == 32", ids == {32}, f"ids={sorted(ids)}")
+        want_id = 32 if region == "vsrc" else 36
+        ids = {r["id"] for r in rl}
+        C(f"{region}: R DDR id == {want_id}", ids == {want_id}, f"ids={sorted(ids)}")
@@ -461,9 +528,15 @@
     for region in SRC_REGIONS:
-        sub = rows_of(rows, {"var"}, region)
+        sub = sorted(rows_of(rows, {"var"}, region),
+                     key=lambda r: (r["cycle"], r["line"]))
         C(f"{region}: VAR count == {PARENTS_PER_FLOW}",
           len(sub) == PARENTS_PER_FLOW, f"{len(sub)}")
         C(f"{region}: every VAR is {PARENT_BYTES} B",
           all((r["len"] + 1) * BEAT_BYTES == PARENT_BYTES for r in sub),
           f"lengths={sorted({(r['len'] + 1) * BEAT_BYTES for r in sub})}")
         C(f"{region}: VAR bytes total == {JOB_BYTES}",
           len(sub) * PARENT_BYTES == JOB_BYTES, "")
+        base = VIRT_BASE[region]
+        ok, det = cover_ok(sub, base)
+        C(f"{region}: VAR contiguous 2-KiB cover of IOVA", ok, det)
@@ -488,50 +561,62 @@
         groups = {}
         for k in kids:
             groups.setdefault((k["owner"], k["parent"]), []).append(k)
         C(f"POSTTL-CHILD parents total == {EXP_PARENTS_TOTAL}",
           len(groups) == EXP_PARENTS_TOTAL, f"{len(groups)}")
         per_owner = {}
         for (o, p) in groups:
             per_owner.setdefault(o, set()).add(p)
         C("POSTTL-CHILD owners == {0,1}", set(per_owner) == set(OWNERS),
           f"{sorted(per_owner)}")
         for o in sorted(per_owner):
             C(f"POSTTL-CHILD owner {o}: parents 0..{PARENTS_PER_FLOW - 1}",
               per_owner[o] == set(range(PARENTS_PER_FLOW)),
               f"{sorted(per_owner[o])}")
+        bad_parent = []
+        for k in kids:
+            if k["parent"] != k["offset"] // PARENT_BYTES:
+                bad_parent.append((k["owner"], k["parent"], k["offset"]))
+        C(f"POSTTL-CHILD parent == offset//{PARENT_BYTES}",
+          not bad_parent, f"bad={bad_parent[:4]}")
         bad = []
         for (o, p), ks in groups.items():
             s = sum(k["beats"] for k in ks)
             if s != EXP_CHILD_SUM:
                 bad.append(f"owner={o} parent={p} sum={s}")
         C(f"POSTTL-CHILD children sum == {EXP_CHILD_SUM} beats per parent",
           not bad, "; ".join(bad))
         if exp_policy == 0:
             C(f"POSTTL-CHILD fixed policy: every child == {FIXED_CHILD_BEATS} beats",
               all(k["beats"] == FIXED_CHILD_BEATS for k in kids),
               f"beats={sorted({k['beats'] for k in kids})}")
         else:
             C("POSTTL-CHILD dynamic policy: child beats in {128,256}",
               all(k["beats"] in DYN_CHILD_BEATS for k in kids),
               f"beats={sorted({k['beats'] for k in kids})}")
 
-        # ---- cross-check against the CSV physical-AR boundaries ----
-        for o, base in ((0, PHYS_BASE["vsrc"]), (1, PHYS_BASE["asrc"])):
-            par = sorted(rows_of(rows, {"par"}, "vsrc" if o == 0 else "asrc"),
-                         key=lambda r: (r["cycle"], r["line"]))
-            seq = [(r["addr"] - base, r["len"] + 1) for r in par]
-            child = sorted([(k["offset"], k["beats"]) for k in kids if k["owner"] == o],
-                           key=lambda t: (t[0], -t[1]))
-            seq_s = sorted(seq, key=lambda t: (t[0], -t[1]))
-            C(f"owner {o}: POSTTL-CHILD (offset,beats) == CSV par "
-              f"(addr-src, len+1) multiset",
-              child == seq_s,
-              f"child={child[:4]}... par={seq_s[:4]}..."
-              if child != seq_s else f"{len(child)} entries match")
+        # ---- ordered cross-check against CSV physical-AR sequence ----
+        for o, region, base in ((0, "vsrc", PHYS_BASE["vsrc"]),
+                                (1, "asrc", PHYS_BASE["asrc"])):
+            par = sorted(rows_of(rows, {"par"}, region),
+                         key=lambda r: (r["cycle"], r["line"]))
+            seq = [(r["addr"] - base, r["len"] + 1) for r in par]
+            child = [(k["offset"], k["beats"]) for k in kids if k["owner"] == o]
+            C(f"owner {o}: POSTTL-CHILD (offset,beats) == CSV par sequence",
+              child == seq,
+              f"child={child[:4]}... par={seq[:4]}..."
+              if child != seq else f"{len(child)} entries match")
         st = log.get("stats")
         if st is None:
             C("POSTTL-STATS line present", False, "absent")
         else:
             C("POSTTL-STATS policy matches run name",
               st["policy"] == exp_policy, f"{st}")
+            C("POSTTL-STATS parents == 32",
+              st["parents"] == EXP_PARENTS_TOTAL, f"{st}")
+            csv_par_total = len(rows_of(rows, {"par"}))
+            C("POSTTL-STATS children == CSV par total",
+              st["children"] == csv_par_total,
+              f"stats={st['children']} csv={csv_par_total}")
@@ -557,6 +642,13 @@
     out["first_var_overall"] = fva
     out["last_w_overall"] = lwa
     out["window_lastW_minus_firstVAR"] = (lwa - fva) if (fva is not None and lwa is not None) else None
+    out["region_window"] = {}
+    for src, dst in (("vsrc", "vdst"), ("asrc", "adst")):
+        fv = out["first_var"].get(src)
+        lw = out["last_w"].get(dst)
+        if fv is not None and lw is not None:
+            out["region_window"][f"{src}->{dst}"] = lw - fv
     return out
@@ -575,30 +667,61 @@
 def cross_run(runs_res, rep):
-    def pick(pred):
-        return {r: v for r, v in runs_res.items() if pred(r)}
-
-    fixed = pick(lambda r: r.startswith("fixed"))
-    dyn = pick(lambda r: r.startswith("dynamic"))
-    out = {}
-
-    def med(d, key):
-        vals = [v.get(key) for v in d.values() if v.get(key) is not None]
-        return (sum(vals) / len(vals)) if vals else None
-
-    out["fixed_src_ar"] = med(fixed, "src_ar_total")
-    out["dynamic_src_ar"] = med(dyn, "src_ar_total")
-    if out["fixed_src_ar"] and out["dynamic_src_ar"]:
-        out["src_ar_reduction_pct"] = 100.0 * (
-            out["fixed_src_ar"] - out["dynamic_src_ar"]) / out["fixed_src_ar"]
-    out["fixed_lastW"] = med(fixed, "last_w_overall")
-    out["dynamic_lastW"] = med(dyn, "last_w_overall")
-    out["fixed_window"] = med(fixed, "window_lastW_minus_firstVAR")
-    out["dynamic_window"] = med(dyn, "window_lastW_minus_firstVAR")
-    if out["fixed_lastW"] and out["dynamic_lastW"]:
-        # raw endpoint difference (different reset endpoints -> must be annotated)
-        out["endpoint_delta_raw_pct"] = 100.0 * (
-            out["dynamic_lastW"] - out["fixed_lastW"]) / out["fixed_lastW"]
-    if out["fixed_window"] and out["dynamic_window"]:
-        # normalised to the same start (first VAR) -> comparable observation window
-        out["endpoint_delta_same_start_pct"] = 100.0 * (
-            out["dynamic_window"] - out["fixed_window"]) / out["fixed_window"]
-    return out
+    out = {}
+
+    def diff(a, b):
+        if a is None or b is None:
+            return None
+        return b - a
+
+    def mean(vals):
+        vals = [v for v in vals if v is not None]
+        return (sum(vals) / len(vals)) if vals else None
+
+    pairs = [
+        ("v31", "fixed-v-32768", "dynamic-v-32768"),
+        ("a31", "fixed-a-32768", "dynamic-a-32768"),
+        ("v100", "fixed-v-32768-lat100", "dynamic-v-32768-lat100"),
+    ]
+    for name, fr, dr in pairs:
+        f = runs_res.get(fr, {})
+        d = runs_res.get(dr, {})
+        p = {"fixed_run": fr, "dynamic_run": dr}
+        p["fixed_src_ar"] = f.get("src_ar_total")
+        p["dynamic_src_ar"] = d.get("src_ar_total")
+        p["src_ar_diff"] = diff(p["fixed_src_ar"], p["dynamic_src_ar"])
+        p["fixed_lastW"] = f.get("last_w_overall")
+        p["dynamic_lastW"] = d.get("last_w_overall")
+        p["lastW_diff"] = diff(p["fixed_lastW"], p["dynamic_lastW"])
+        p["fixed_window"] = f.get("window_lastW_minus_firstVAR")
+        p["dynamic_window"] = d.get("window_lastW_minus_firstVAR")
+        p["window_diff"] = diff(p["fixed_window"], p["dynamic_window"])
+        for label, reg in (("V", "vdst"), ("A", "adst")):
+            fv = f.get("last_w", {}).get(reg)
+            dv = d.get("last_w", {}).get(reg)
+            p[f"{label}_fixed_lastW"] = fv
+            p[f"{label}_dynamic_lastW"] = dv
+            p[f"{label}_lastW_diff"] = diff(fv, dv)
+        f_mean = mean([f.get("last_w", {}).get("vdst"),
+                       f.get("last_w", {}).get("adst")])
+        d_mean = mean([d.get("last_w", {}).get("vdst"),
+                       d.get("last_w", {}).get("adst")])
+        p["fixed_mean_Vend_Aend"] = f_mean
+        p["dynamic_mean_Vend_Aend"] = d_mean
+        p["mean_Vend_Aend_diff"] = diff(f_mean, d_mean)
+        out[name] = p
+    return out
@@ -619,32 +742,34 @@
 def conclusion_draft(cross, runs_res):
-    fa = cross.get("fixed_src_ar")
-    da = cross.get("dynamic_src_ar")
-    r = cross.get("src_ar_reduction_pct")
-    end_raw = cross.get("endpoint_delta_raw_pct")
-    end_norm = cross.get("endpoint_delta_same_start_pct")
-    return (
-        "CONSERVATIVE DRAFT (adjudication belongs to Codex; no claim of priority "
-        "or of a steady-state speed-up is made here):\n"
-        f"- Under this deterministic, two-real-DMA, single-job-per-flow geometry "
-        f"the physical source-AR count moved from {fa} (fixed 128-beat children) "
-        f"to {da} (longest contiguous prepared prefix)"
-        + (f", i.e. {r:.2f}% fewer source requests.\n" if r is not None else ".\n")
-        + (f"- The last-destination-write endpoint moved by {end_raw:.2f}% "
-           f"relative to each run's own reset endpoint, and by "
-           f"{end_norm:.2f}% once both runs are normalised to the same start "
-           f"(first virtual AR).  These two numbers are NOT the same and neither "
-           f"is a NEXT->DONE latency or a steady-state throughput ratio.\n"
-           if (end_raw is not None and end_norm is not None) else
-           "- Endpoint deltas could not be computed from the supplied artefacts.\n")
-        + "- The first-VAR -> last-W delta is an observation window only: it "
-          "spans from the first source read request to the final destination data "
-          "beat and must not be read as 'NEXT to DONE'.\n"
-          "- R wait evidence (rlast latency, rblock durations) is reported as "
-          "observed; this script deliberately does not attribute queueing or "
-          "stalls to a single root cause.\n"
-          "- The fixed policy's contribution (128-beat children, 64 source ARs) "
-          "is already covered by the plain per-channel 'complete read/write "
-          "burst' rule (arm-dma350-2023.txt 1013-1019, 4624-4637); whether the "
-          "dynamic boundary adds anything beyond that ordinary rule cannot be "
-          "decided from these counters alone."
-    )
+    lines = [
+        "CONSERVATIVE DRAFT (adjudication belongs to Codex; no priority or "
+        "steady-state speed-up claim):",
+    ]
+    for name in ("v31", "a31", "v100"):
+        p = cross.get(name, {})
+        lines.append(
+            f"- {name}: source AR {p.get('fixed_src_ar')} (fixed) vs "
+            f"{p.get('dynamic_src_ar')} (dynamic); lastW diff "
+            f"{p.get('lastW_diff')}; observation-window diff "
+            f"{p.get('window_diff')}; V lastW diff {p.get('V_lastW_diff')}; "
+            f"A lastW diff {p.get('A_lastW_diff')}; "
+            f"mean(Vend,Aend) diff {p.get('mean_Vend_Aend_diff')}."
+        )
+    lines.extend([
+        "- The first-VAR -> last-W delta is an observation window only, not a "
+        "NEXT->DONE latency or steady-state throughput figure.",
+        "- DMA350 whole-read/whole-write caching is a related strong ordinary "
+        "method, but it is not the same as this postTL owner schedule.  The "
+        "existing 4/6 correct slices plus an unproven independent contribution "
+        "cannot be equated to all prior work.",
+        "- The reported small FIFO (48 B) is not the total state area.",
+        "- AXI attribute flags are preserved by the SystemVerilog struct; the "
+        "harness reports errors/faults as fatal only, not as complete handling.",
+        "- No number above is a board timing or area claim.",
+    ])
+    return "\n".join(lines)
@@ -697,1 +822,1 @@
-    md.append(f"
+    md.append(f"""
@@ -710,2 +835,2 @@
-    ")
+    """)
     (directory / "analysis.md").write_text("\n".join(md), encoding="utf-8")
```

Not executed. The supplied `content_with_line_numbers` stopped at line 697, so the closing-quote hunk above assumes the original multi-line string ends with `    ")` immediately before the `analysis.md` write. If the real tail differs, adjust that one hunk; the rest of the patch is independent of the omitted tail.
