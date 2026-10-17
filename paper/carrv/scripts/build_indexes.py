#!/usr/bin/env python3
"""Build deterministic, source-bound claim/TODO indexes; never run experiments.

Only reads the manuscript, paper data, and explicitly catalogued source paths.
Only writes paper/carrv/claims.md and paper/carrv/todo.md. It checks coverage,
source existence/ranges/selectors, numeric macro values, plot provenance, and
table cells. This is an index/consistency check, not scientific validation.
"""
from __future__ import annotations

import csv
import hashlib
import json
from pathlib import Path
import re

PAPER = Path(__file__).resolve().parents[1]
ROOT = PAPER.parents[1]
CONFIG = PAPER / "data/claim_sources.json"
INV = "iommu-platform/out/invalidation-interference-20261017/analysis/results.csv"
PLRU = "iommu-platform/out/plru-ablation-20261017/analysis/comparison.csv"
ML = "iommu-platform/out/dead-translation-20261017/analysis/final/results.csv"


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def records(path: str) -> list[dict]:
    with (ROOT / path).open(newline="") as stream:
        return [{**row, "_line": line} for line, row in enumerate(csv.DictReader(stream), 2)]


def esc(text: object) -> str:
    return str(text).replace("|", "&#124;").replace("\n", "<br>")


def link(path: str, line: int | str = 1) -> str:
    first = str(line).split("-")[0]
    return f"[{path}:{line}]({(ROOT / path).as_posix()}:{first})"


def selected(rows: list[dict], selector: dict) -> list[dict]:
    return [r for r in rows if all(str(r.get(k)) in [str(x) for x in (v if isinstance(v, list) else [v])]
                                  for k, v in selector.items())]


def reference(ref: dict) -> str:
    path = ROOT / ref["path"]
    if not path.is_file():
        raise ValueError(f"Missing original evidence: {path}")
    if "selector" in ref:
        rows = records(ref["path"])
        match = selected(rows, ref["selector"])
        if not match:
            raise ValueError(f"Empty source selector: {ref}")
        if any(c not in match[0] for c in ref["columns"]):
            raise ValueError(f"Unknown source column: {ref}")
        lines = ",".join(str(r["_line"]) for r in match)
        return (link(ref["path"], match[0]["_line"]) + f"; CSV lines {lines}; "
                + "selector `" + json.dumps(ref["selector"], ensure_ascii=False, sort_keys=True)
                + "`; columns `" + ",".join(ref["columns"]) + "`")
    limits = [int(n) for n in ref.get("lines", "1").split("-")]
    # Physical source lines: form-feeds in extracted PDF text are not newlines.
    count = len(path.read_text().split("\n"))
    if min(limits) < 1 or max(limits) > count:
        raise ValueError(f"Invalid original line range ({count} lines): {ref}")
    return link(ref["path"], ref.get("lines", "1"))


def balanced(text: str, brace: int) -> tuple[str, int]:
    depth = 1
    pos = brace + 1
    while pos < len(text):
        if text[pos] == "{" and (pos == 0 or text[pos - 1] != "\\"):
            depth += 1
        elif text[pos] == "}" and text[pos - 1] != "\\":
            depth -= 1
            if depth == 0:
                return text[brace + 1:pos], pos + 1
        pos += 1
    raise ValueError("Unbalanced manuscript command")


def expanded(text: str, macros: dict) -> str:
    for name in sorted(macros, key=len, reverse=True):
        text = re.sub(r"\\" + re.escape(name) + r"\b(?:\{\})?", lambda _: macros[name], text)
    return text


def display(text: str) -> str:
    text = re.sub(r"\\claim\{[^}]+\}", "", text).strip()
    text = re.sub(r"\\cite\{([^}]+)\}", r"[\1]", text)
    text = re.sub(r"\\ref\{([^}]+)\}", r"[\1]", text)
    for command in ("caption", "Description", "texttt", "textsf", "mathrm", "textit", "textbf"):
        prefix = "\\" + command
        while prefix + "{" in text:
            begin = text.index(prefix + "{")
            value, end = balanced(text, begin + len(prefix))
            text = text[:begin] + value + text[end:]
    return text.replace("\\%", "%").replace("\\,", " ").replace("~", " ").replace("{}", "").strip()


def numeric_sources(macros: dict, inv: list[dict], ml: list[dict]) -> list[list[str]]:
    oi = {r["case_id"]: r for r in inv}
    ni = {r["case_id"]: r for r in ml}
    out = {}

    def old(mode, pages, policy="full-api", domain="same"):
        return oi[f"{domain}-b{pages}-{mode}-{policy}"]

    def new(mode, pages):
        return ni[f"same-b{pages}-{mode}"]

    def cite(path, row, cols):
        return link(path, row["_line"]) + f"; `case_id={row['case_id']}`; " + ", ".join(f"`{c}={row[c]}`" for c in cols)

    def put(name, value, sources, operation="Exact source cell"):
        if str(value) != macros[name]:
            raise ValueError(f"Macro {name}: manuscript={macros[name]}, recomputed={value}")
        out[name] = [str(value), "<br>".join(sources), operation]

    slow = [100 * (int(old("m1", p)["B_complete_steps"]) / int(old("m0", p)["B_complete_steps"]) - 1) for p in (4, 8)]
    penalty = [100 * (int(old("m3", p)["A_complete_with_final_flush_steps"]) / int(old("m1", p)["A_complete_with_final_flush_steps"]) - 1) for p in (4, 8)]
    for name, value, modes, col, operation in [
        ("BSlowMin", min(slow), ("m0", "m1"), "B_complete_steps", "min_p 100*(M1.B/M0.B-1), p=4,8; one decimal"),
        ("BSlowMax", max(slow), ("m0", "m1"), "B_complete_steps", "max_p 100*(M1.B/M0.B-1), p=4,8; one decimal"),
        ("APenaltyMin", min(penalty), ("m1", "m3"), "A_complete_with_final_flush_steps", "min_p 100*(M3.A/M1.A-1), p=4,8; one decimal"),
        ("APenaltyMax", max(penalty), ("m1", "m3"), "A_complete_with_final_flush_steps", "max_p 100*(M3.A/M1.A-1), p=4,8; one decimal")]:
        put(name, f"{value:.1f}", [cite(INV, old(m, p), [col]) for p in (4, 8) for m in modes], operation)
    for prefix, mode, col in [("BaseWalk", "m0", "B_walks"), ("WideWalk", "m1", "B_walks"), ("FineWalk", "m3", "B_walks"),
                               ("WideTime", "m1", "B_complete_steps"), ("BaseTime", "m0", "B_complete_steps"), ("FineTime", "m3", "B_complete_steps")]:
        for p, suffix in ((4, "Four"), (8, "Eight")):
            put(prefix + suffix, old(mode, p)[col], [cite(INV, old(mode, p), [col])])
    for name, mode, p, col in [("WideA", "m1", 4, "A_complete_with_final_flush_steps"),
                              ("FineAFour", "m3", 4, "A_complete_with_final_flush_steps"),
                              ("FineAEight", "m3", 8, "A_complete_with_final_flush_steps"), ("AWalks", "m1", 4, "A_walks")]:
        put(name, old(mode, p)[col], [cite(INV, old(mode, p), [col])])
    for p, suffix in ((4, "Four"), (8, "Eight")):
        for prefix, col in (("UnmapWalk", "B_walks"), ("UnmapTime", "B_complete_steps")):
            row = old("m1", p, "unmap-only")
            put(prefix + suffix, row[col], [cite(INV, row, [col])])
    for mode, prefix in (("m4-16", "BatchSixteen"), ("m4-64", "BatchSixtyFour")):
        for suffix, col in (("Inv", "cq_iotinval"), ("Fence", "cq_iofence")):
            row = old(mode, 4)
            put(prefix + suffix, row[col], [cite(INV, row, [col])])
        for p, suffix in ((4, "Four"), (8, "Eight")):
            row, base = old(mode, p), old("m0", p)
            val = 100 * (int(row["B_complete_steps"]) / int(base["B_complete_steps"]) - 1)
            put(prefix + suffix, f"{val:.1f}", [cite(INV, r, ["B_complete_steps"]) for r in (row, base)], "100*(M4.B/M0.B-1); one decimal")
    for name, policy in (("MainCases", "full-api"), ("ControlCases", "unmap-only")):
        match = [r for r in inv if r["map_policy"] == policy]
        put(name, len(match), [f"{link(INV)}; `map_policy={policy}`; physical CSV lines " + ",".join(str(r["_line"]) for r in match)], "Count unique recorded case_id rows; not repeated trials")
    for p, suffix in ((4, "Four"), (8, "Eight")):
        strict, base, fq = (new(mode, p) for mode in ("ml-strict", "m0", "ml-fq"))
        for prefix, col in (("MLStrictB", "T_B"), ("MLStrictA", "A_final_flush_end")):
            put(prefix + suffix, strict[col], [cite(ML, strict, [col])])
        put("MLStrictSlow" + suffix, f"{100 * (int(strict['T_B']) / int(base['T_B']) - 1):.3f}",
            [cite(ML, r, ["T_B"]) for r in (strict, base)], "100*(ML-strict.B/M0.B-1); three decimals")
        dead = 100 * int(fq["dead_slot_cycles"]) / (16 * int(fq["dead_sample_cycles"]))
        put("MLDead" + suffix, f"{dead:.3f}", [cite(ML, fq, ["dead_slot_cycles", "dead_sample_cycles", "dead_average_slot_fraction"])], "100*dead_slot_cycles/(16*dead_sample_cycles); three decimals")
    fq = new("ml-fq", 4)
    for name, col in (("MLFQPeak", "fq_peak"), ("MLFQCapacity", "fq_capacity"), ("MLFQDrain", "fq_drain_end"), ("MLFQTimeout", "fq_timeout_cycles")):
        put(name, fq[col], [cite(ML, fq, [col])])
    broad, fine = ni["concurrent-different-b8-m1"], ni["concurrent-different-b8-m2"]
    for name, col in (("DiagADelta", "A_final_flush_end"), ("DiagFlushDelta", "A_wall_flush"), ("DiagFetchDelta", "A_flush_cq_fetch"),
                      ("DiagPTWDelta", "A_wall_copy_pte"), ("DiagOtherDelta", "A_wall_copy_other"), ("DiagHostDelta", "A_wall_host")):
        put(name, int(fine[col]) - int(broad[col]), [cite(ML, r, [col]) for r in (fine, broad)], "Fine minus broad; mutually exclusive occupancy where applicable, not recoverable causal costs")
    if set(out) != set(macros):
        raise ValueError(f"Unbound numeric macros: {set(macros) - set(out)}")
    return [[name] + out[name] for name in macros]


def table(headers: list[str], rows: list[list]) -> str:
    return "| " + " | ".join(headers) + " |\n| " + " | ".join("---" for _ in headers) + " |\n" + "\n".join("| " + " | ".join(esc(c) for c in row) + " |" for row in rows) + "\n"


def main() -> None:
    cfg = json.loads(CONFIG.read_text())
    tex = (PAPER / "main.tex").read_text()
    lines = tex.split("\n")
    macros = dict(re.findall(r"\\newcommand\{\\(\w+)\}\{([^{}]*)\}", (PAPER / "data/numbers.tex").read_text()))
    if macros != json.loads((PAPER / "data/numbers.json").read_text()):
        raise ValueError("numbers.tex and numbers.json differ")
    bundles = cfg["bundles"]
    rendered = {key: "<br>".join(reference(r) for r in bundle["references"]) for key, bundle in bundles.items()}

    def evidence(binding):
        keys = binding["bundles"]
        if not keys:
            return "Writing-process declaration / author action", binding["note"]
        source = "<br>".join(rendered[key] for key in keys)
        notes = [binding.get("note", "")] + [bundles[k].get("derivation", "") for k in keys] + [bundles[k]["uncertainty"] for k in keys]
        return source, " ".join(dict.fromkeys(n for n in notes if n))

    claim_rows, desc_rows, other_rows, todos = [], [], [], []
    ids = []
    section, subsection = "Front matter", ""
    for line_num, line in enumerate(lines, 1):
        if "\\begin{abstract}" in line:
            section, subsection = "Abstract", ""
        elif "\\end{abstract}" in line:
            section, subsection = "Front matter", ""
        match = re.search(r"\\section\{([^}]+)\}", line)
        if match:
            section, subsection = match[1], ""
        match = re.search(r"\\subsection\{([^}]+)\}", line)
        if match:
            subsection = match[1]
        where = section + (" / " + subsection if subsection else "")
        for match in re.finditer(r"\\todo\{", line):
            if line.lstrip().startswith("\\newcommand"):
                continue
            value, _ = balanced(line, match.end() - 1)
            category = "等待并行实验" if value.startswith(("等待", "Waiting")) else "待核文献" if value.startswith("待核文献") else "需要作者决定"
            todos.append([category, link("paper/carrv/main.tex", line_num), value])
        found = re.findall(r"\\claim\{([^}]+)\}", line)
        for cid in found:
            if cid not in cfg["claims"]:
                raise ValueError(f"Claim has no source binding: {cid} at main.tex:{line_num}")
            ids.append(cid)
            binding = cfg["claims"][cid]
            source, caveat = evidence(binding)
            statement = expanded(display(line), macros)
            if any(re.search(r"\\" + re.escape(k) + r"\b", statement) for k in macros):
                raise ValueError(f"Unexpanded numeric macro: {cid}")
            claim_rows.append([cid, where, link("paper/carrv/main.tex", line_num), statement, source, caveat])
        if "\\Description{" in line:
            value, _ = balanced(line, line.index("\\Description{") + len("\\Description"))
            # Its following caption supplies the authoritative figure binding.
            future = "\n".join(lines[line_num:line_num + 5])
            cid = re.search(r"\\claim\{(F\d+)\}", future)
            if not cid:
                raise ValueError(f"Unbound figure description at {line_num}")
            source, caveat = evidence(cfg["claims"][cid[1]])
            desc_rows.append([cid[1] + "-description", where, link("paper/carrv/main.tex", line_num), expanded(display(value), macros), source, caveat])
        for prefix, binding in cfg["unmarked"].items():
            if line.strip().startswith(prefix):
                source, caveat = evidence(binding)
                other_rows.append(["U" + str(len(other_rows) + 1).zfill(3), where, link("paper/carrv/main.tex", line_num), expanded(display(line), macros), source, caveat])
    if len(ids) != len(set(ids)):
        raise ValueError("Duplicate claim IDs")
    unmapped_text = [(i, t) for i, t in enumerate(lines, 1) if t.strip() and i > 20 and not t.strip().startswith("\\")
                     and "\\claim{" not in t and not any(t.strip().startswith(p) for p in cfg["unmarked"])]
    if unmapped_text:
        raise ValueError(f"Unindexed prose lines: {unmapped_text}")

    inv, plru, ml = records(INV), records(PLRU), records(ML)
    numeric = numeric_sources(macros, inv, ml)
    macro_lines = {name: i for i, line in enumerate((PAPER / "data/numbers.tex").read_text().split("\n"), 1)
                   for name in re.findall(r"\\newcommand\{\\(\w+)\}", line)}
    numeric = [[r[0], link("paper/carrv/data/numbers.tex", macro_lines[r[0]])] + r[1:] for r in numeric]
    parts = ["# Manuscript claim and numerical source index\n",
             "Generated by `scripts/build_indexes.py` from the current manuscript, numeric macros, explicit source catalog, and existing CSVs. This is a navigable provenance/consistency index, not a new experiment or independent scientific validation. All numeric macros are expanded below. Original source paths and physical source lines are given; CSV selectors and columns disambiguate every plotted/table value. Statements may combine several bounded facts, with their joint sources and limits in the same row.\n",
             f"Indexed manuscript SHA256: `{digest(PAPER / 'main.tex')}`. Source catalog SHA256: `{digest(CONFIG)}`. {len(ids)} unique marked statements, {len(desc_rows)} image descriptions, {len(other_rows)} additional prose declarations, {len(macros)} numeric macros. No pending IOFENCE traffic results were read.\n",
             "## Every marked factual/numerical statement\n",
             table(["ID", "Section", "Manuscript location", "Macro-expanded statement", "Supporting original evidence", "Derivation / uncertainty / limits"], claim_rows),
             "## Figure descriptions and unmarked prose declarations\n",
             table(["ID", "Section", "Manuscript location", "Macro-expanded text", "Supporting original evidence", "Limits"], desc_rows + other_rows),
             "## Every numeric macro and its original cells\n",
             table(["Macro", "Definition location", "Expanded value", "Original CSV rows / columns / values", "Calculation"], numeric)]

    # Every figure point, with both exact original input and the actual paper-derived value.
    figure_sets = [
        ("invalidation-derived.csv", INV, ["case_id", "neighbor_plot_role", "initiator_plot_role", "B_complete_steps", "baseline_case_id", "B_baseline_complete_steps", "B_completion_increase_pct", "A_complete_with_final_flush_steps", "A_broad_reference_case_id", "A_broad_complete_steps", "A_completion_over_broad"], "F002/F003 (neighbor) and F004/F005 (initiator): domain/footprint panels, five full-api bars and unmap-only control diamonds; M0 is denominator only. B percent=100*(B/M0.B−1); A ratio=A.final/M1.A.final. All 28 source rows retained, including four normalization references.", "source_csv_line"),
        ("plru-derived.csv", PLRU, ["variant", "mode", "ring", "walks_total", "walks_A", "walks_V", "T_return", "baseline_T_return", "return_reduction_pct"], "F006 panel(a): mode=p walks for R1/R8/R16, after exact P/PN count equality. Panel(b): P/PN complete T_return/1000 at R8/R16. V2 annotations=100*(V0.T_return−V2.T_return)/V0.T_return at R8. All 18 source rows retained, including R1 complete times omitted from panel(b).", "source_line"),
        ("mainline-derived.csv", ML, ["case_id", "plot_role", "T_B", "baseline_case_id", "B_baseline_steps", "B_increase_over_M0_pct", "dead_slot_cycles", "dead_sample_cycles", "dead_peak", "dead_mean_slot_pct", "dead_peak_slot_pct", "oracle_benefit_over_FQ_pct"], "F007 panel(a): group=matrix, domain=same, mode=ml-strict/ml-fq/oracle, b_pages=4/8, B percent=100*(T_B/M0.T_B−1). Panel(b): same rows, mean=100*dead_slot_cycles/(16*dead_sample_cycles), peak=100*dead_peak/16 (green markers). Different-domain metrics checked exactly before coalescing; all 28 source rows retained, including baseline and diagnosis rows not plotted.", "source_csv_line")]
    for filename, original, cols, recipe, line_col in figure_sets:
        path = PAPER / "data" / filename
        data = records(path.relative_to(ROOT).as_posix())
        raw = {str(r["_line"]): r for r in records(original)}
        if len(data) != len(raw) or {r[line_col] for r in data} != set(raw):
            raise ValueError(f"Incomplete/duplicate source-row coverage in {filename}")
        for row in data:
            line = row[line_col]
            if line not in raw:
                raise ValueError(f"Unknown source row in {filename}: {line}")
            expected_hash = digest(ROOT / original)
            got_hash = row.get("source_csv_sha256", row.get("source_sha256"))
            if got_hash != expected_hash:
                raise ValueError(f"Stale source hash in {filename}")
            for field, value in raw[line].items():
                if field != "_line" and row.get(field) != value:
                    raise ValueError(f"Derived row changed original field {filename}:{line}:{field}")
        figure_rows = [[link(original, r[line_col]), link(path.relative_to(ROOT).as_posix(), r["_line"])] + [r.get(c, "") for c in cols] for r in data]
        parts += [f"## Figure data: {filename}\n", recipe + "\n", table(["Original physical line", "Paper derived line"] + cols, figure_rows)]
    parts += ["## Platform diagram facts (F001)\n", "The parent platform's CVA6/four-DMA context is sourced from `iommu-platform/README.md:3-5`; the exercised two DMA instances are `runner/invalidation_dma_top.sv:132-140`. The visible diagram labels A64/B256 come from every old CSV row's `A_jobs=64,B_jobs=256`; `copy_bytes=4096` is the associated source workload, not a visible diagram label. CPU/Linux are contextual, not executed. CQ/translation/memory connections are the exercised harness sources listed in F001.\n"]

    oi = {r["case_id"]: r for r in inv}
    table_rows = []
    text = (PAPER / "data/commands-table.tex").read_text()
    for label, mode, domain in [("M0", "m0", "same"), ("M1", "m1", "same"), ("M2, different", "m2", "different"), ("M3", "m3", "same"), ("M4, $N=16$", "m4-16", "same"), ("M4, $N=64$", "m4-64", "same")]:
        rows = [oi[f"{domain}-b{p}-{mode}-full-api"] for p in (4, 8)]
        q, w = rows
        exact = f'{label} & {q["cq_iotinval"]} / {q["cq_iofence"]} & {q["B_walks"]} / {w["B_walks"]} & {int(q["A_complete_with_final_flush_steps"]):,} / {int(w["A_complete_with_final_flush_steps"]):,}'
        if exact not in text:
            raise ValueError(f"Table row differs from source: {label}")
        table_rows.append([label, domain, "<br>".join(link(INV, r["_line"]) + f"; `case_id={r['case_id']}`" for r in rows), q["cq_iotinval"] + "/" + q["cq_iofence"], q["B_walks"] + "/" + w["B_walks"], q["A_complete_with_final_flush_steps"] + "/" + w["A_complete_with_final_flush_steps"]])
    parts += ["## Every table data row (T001/T002)\n", "Generated by `scripts/prepare_data.py` into `data/commands-table.tex`. Original columns are `cq_iotinval,cq_iofence,B_walks,A_complete_with_final_flush_steps`; values are exact integers and the final two cells list B4/B8. Commands are identical across the two footprints.\n", table(["Rendered row", "Domain", "Original row selectors", "IOTINVAL / IOFENCE", "B walks 4/8", "A final time 4/8"], table_rows)]

    bib = (PAPER / "references.bib").read_text()
    bib_rows = []
    cited = {key for group in re.findall(r"\\cite\{([^}]+)\}", tex) for key in group.split(",")}
    for match in re.finditer(r"@\w+\{([^,]+),", bib):
        key = match[1]
        content, end = balanced(bib, match.end() - len(key) - 2)
        if key not in cfg["bibliography"]:
            raise ValueError(f"Unbound bibliography entry: {key}")
        source = cfg["bibliography"][key]
        fields = []
        for field in re.finditer(r"(\w+)\s*=\s*\{", content):
            value, _ = balanced(content, field.end() - 1)
            fields.append(f"{field[1]}: {value}")
        line = bib[:match.start()].count("\n") + 1
        bib_rows.append([key, link("paper/carrv/references.bib", line), "<br>".join(fields), "<br>".join(reference(r) for r in source["sources"]), source["note"]])
    if cited - set(cfg["bibliography"]):
        raise ValueError(f"Cited but unbound references: {cited - set(cfg['bibliography'])}")
    parts += ["## Every bibliography record and checked metadata source\n", "Scholarly metadata verification is distinct from full technical reading. The five original scholarly records remain version-bound; specification and Linux are descriptive primary-source references. Intel is the separately authorized completed prior-source passage, not pending experimental output.\n", table(["BibTeX key", "Draft record", "All rendered metadata fields", "Checked original/metadata authority", "Reading and identity limits"], bib_rows),
              "## Index checks and limits\n", f"Every one of {len(ids)} current claim IDs has an explicit nonempty source binding; all source paths/ranges/selectors and columns resolve. All {len(macros)} macro values were recomputed from source CSV cells; all original fields in 28 invalidation, 18 PLRU and 28 mainline/diagnosis derived rows match their recorded source lines/hashes. All six table rows match the CSV; every cited bibliography key has an evidence binding. The claim index retains inference/recommendation limits and does not certify complete evidence sufficiency or publication readiness.\n"]

    todo_parts = ["# Open items and author decisions\n", "Generated from every current `\\todo{...}` in main.tex, plus explicitly catalogued scope limits. An empty category means no active item, not permission to launch work. No experiment is launched or awaited by this index.\n"]
    for category in ("等待并行实验", "待核文献", "需要作者决定"):
        category_rows = [[location, value, "Explicit manuscript TODO"] for cat, location, value in todos if cat == category]
        category_rows += [[s["source"], s["text"], "Carried source/author boundary"] for s in cfg.get("todo_supplements", []) if s["category"] == category]
        todo_parts += ["## " + category + "\n"]
        if category_rows:
            todo_parts.append(table(["Location / source", "Item", "Status"], category_rows))
        else:
            todo_parts.append("No active waiting item is recorded. The IOFENCE global-neighbor-wait question is unmeasured and outside established findings; a failed prior-work gate did not create a pending traffic experiment.\n" if category == "等待并行实验" else "No item in this category.\n")
    todo_parts += ["## Coverage\n", f"All {len(todos)} literal manuscript TODO invocations are included exactly once; the macro definition is excluded. Source SHA256: `{digest(PAPER / 'main.tex')}`.\n"]
    (PAPER / "claims.md").write_text("\n".join(parts))
    (PAPER / "todo.md").write_text("\n".join(todo_parts))
    print(f"Indexed {len(ids)} marked claims, {len(desc_rows)} descriptions, {len(other_rows)} other prose, {len(macros)} macros, {len(bib_rows)} bibliography entries, {len(todos)} TODOs; source/table/derived CSV checks passed.")


if __name__ == "__main__":
    main()
