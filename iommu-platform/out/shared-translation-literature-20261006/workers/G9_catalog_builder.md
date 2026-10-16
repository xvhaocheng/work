下面脚本保存为语料库根目录 `build_catalog.py` 即可。它只读输入、只写六个输出文件，不做网络访问，不修改源文件。缺失全文 worker 证据卡、全文本地 PDF/TXT、被引用的 worker 报告 md 等会写入 `catalog-checks.json` 并以非零退出；但会先把能生成的目录写出来，避免“静默失败”。以下代码中所有统计数都运行时计算，不硬编码 59/60/13。

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
build_catalog.py

离线目录生成器。只使用 Python 标准库。

用法：
    python3 build_catalog.py
    python3 build_catalog.py --root /path/to/corpus-root

默认 root 为 build_catalog.py 所在目录。
输出：
    catalog.json
    catalog.csv
    references.bib
    INDEX.md
    statistics.json
    catalog-checks.json

设计约束：
- 不联网，不补 DOI，不猜 year/venue。
- 全文阅读状态统一为 fulltext_worker_reviewed；root 重点核查用 root_spotchecked 单独字段。
- 仅题录沿用输入 reading_status。
- 全文文件缺失、全文证据卡缺失、被引用本地链接缺失，均报告为 fatal，最终退出码非 0。
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import sys
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path

TOPIC_ORDER = ["foundations", "gpu", "multigpu", "io", "accelerators", "lifecycle"]
TOPIC_LABELS = {
    "foundations": "foundations（基础）",
    "gpu": "gpu",
    "multigpu": "multigpu",
    "io": "io",
    "accelerators": "accelerators",
    "lifecycle": "lifecycle",
}
DEFAULT_ACCESS_DATE = "2026-10-06"

CSV_FIELDS = [
    "work_id",
    "canonical_work_id",
    "version_id",
    "is_preferred",
    "title",
    "authors",
    "year",
    "venue",
    "topic",
    "relevance",
    "publication_status",
    "reading_status",
    "fulltext_available",
    "doi",
    "doi_valid",
    "source_url",
    "pdf",
    "text",
    "pdf_sha256",
    "text_sha256",
    "worker_report",
    "root_spotchecked",
    "metadata_note",
    "notes",
    "version_note",
    "metadata_source",
]

DOI_PREFIX_RE = re.compile(r"^(?:https?://(?:dx\.)?doi\.org/|doi:\s*)", re.I)
DOI_RE = re.compile(r"^10\.\d{4,9}/\S+$")

BIB_ESCAPE_MAP = {
    "\\": r"\textbackslash{}",
    "&": r"\&",
    "%": r"\%",
    "_": r"\_",
    "#": r"\#",
    "$": r"\$",
    "{": r"\{",
    "}": r"\}",
    "~": r"\textasciitilde{}",
    "^": r"\textasciicircum{}",
}


def add_issue(issues, level, code, message, ident=None):
    issues.append({
        "level": level,
        "code": code,
        "message": message,
        "id": ident,
    })


def is_missing(v):
    return v is None or v == "" or v == []


def norm_empty(v):
    return None if is_missing(v) else v


def derive_access_date(root: Path) -> str:
    m = re.search(r"(\d{4})(\d{2})(\d{2})$", root.name)
    if m:
        return f"{m.group(1)}-{m.group(2)}-{m.group(3)}"
    return DEFAULT_ACCESS_DATE


def load_json_file(path: Path, issues, default, required=False):
    if not path.is_file():
        if required:
            add_issue(issues, "fatal", "missing_required_file", f"缺少必需文件：{path}", None)
        return default
    try:
        with path.open("r", encoding="utf-8") as f:
            return json.load(f)
    except Exception as e:
        add_issue(issues, "fatal", "json_load_error", f"读取 JSON 失败：{path}: {e}", None)
        return default


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def clean_doi(doi):
    if is_missing(doi):
        return None, False, None
    raw = str(doi).strip()
    cleaned = DOI_PREFIX_RE.sub("", raw).strip()
    valid = bool(DOI_RE.match(cleaned))
    return cleaned, valid, raw


def missing_metadata_fields(obj):
    missing = []
    for field in ("year", "venue", "doi", "authors"):
        if is_missing(obj.get(field)):
            missing.append(field)
    return missing


def anchor(prefix, ident):
    safe = re.sub(r"[^A-Za-z0-9_.-]+", "-", str(ident)).strip("-")
    if not safe:
        safe = "item"
    return f"{prefix}-{safe}"


def md_cell(s):
    if s is None:
        return "null"
    return str(s).replace("|", "\\|").replace("\n", " ").strip()


def md_link(path, text):
    if not path:
        return ""
    return f"[{text}]({path})"


def bib_escape(s):
    if s is None:
        return ""
    return "".join(BIB_ESCAPE_MAP.get(ch, ch) for ch in str(s))


def bib_key(s):
    key = re.sub(r"[^A-Za-z0-9:_-]+", "-", str(s)).strip("-")
    return key or "ref"


def pick_meta(card, override, field, default=None):
    if field in override:
        return override.get(field)
    if card is not None and field in card:
        return card.get(field)
    return default


def pick_rec_override(rec, override, field, default=None):
    if field in override:
        return override.get(field)
    return rec.get(field, default)


def build_work(canonical, versions, is_family):
    for i, v in enumerate(versions):
        v["canonical_work_id"] = canonical
        v["is_preferred"] = (i == 0)

    pref = versions[0]

    def first_val(field, default=None):
        for v in versions:
            val = v.get(field)
            if not is_missing(val):
                return val
        return default

    title = first_val("title", canonical)
    authors = first_val("authors", [])
    if not isinstance(authors, list):
        authors = []
    year = first_val("year")
    venue = first_val("venue")
    doi = first_val("doi")
    doi_valid = False
    if doi:
        _, doi_valid, _ = clean_doi(doi)
    publication_status = first_val("publication_status", "uncertain")
    relevance = first_val("relevance", "uncertain")
    topic = pref.get("topic") or first_val("topic", "other")
    topics = sorted({v.get("topic") for v in versions if not is_missing(v.get("topic"))})
    fulltext_available = any(v.get("fulltext_available") for v in versions)
    reading_status = "fulltext_worker_reviewed" if fulltext_available else first_val("reading_status", "metadata_only")
    root_spotchecked = any(v.get("root_spotchecked") for v in versions)
    source_url = first_val("source_url")
    metadata_source = first_val("metadata_source")
    metadata_notes = [v.get("metadata_note") for v in versions if not is_missing(v.get("metadata_note"))]
    notes = [v.get("notes") for v in versions if not is_missing(v.get("notes"))]
    version_notes = [v.get("version_note") for v in versions if not is_missing(v.get("version_note"))]

    work = {
        "id": canonical,
        "canonical_work_id": canonical,
        "title": title,
        "authors": authors,
        "year": year,
        "venue": venue,
        "doi": doi,
        "doi_valid": doi_valid,
        "publication_status": publication_status,
        "relevance": relevance,
        "topic": topic,
        "topics": topics,
        "fulltext_available": fulltext_available,
        "reading_status": reading_status,
        "root_spotchecked": root_spotchecked,
        "source_url": source_url,
        "metadata_source": metadata_source,
        "metadata_notes": metadata_notes,
        "notes": notes,
        "version_notes": version_notes,
        "version_count": len(versions),
        "preferred_version_id": pref["id"],
        "versions": versions,
        "is_work_family": is_family,
        "version_comparison_note": "同一工作的多个版本各自独立保存与审读；未做逐字比对。",
        "unknown_fields": [],
    }
    work["unknown_fields"] = missing_metadata_fields(work)
    return work


def write_json(path: Path, obj):
    with path.open("w", encoding="utf-8") as f:
        json.dump(obj, f, ensure_ascii=False, indent=2)
        f.write("\n")


def write_csv(path: Path, rows):
    with path.open("w", encoding="utf-8-sig", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=CSV_FIELDS)
        writer.writeheader()
        for row in rows:
            writer.writerow({k: row.get(k, "") for k in CSV_FIELDS})


def write_bib(path: Path, works, access_date):
    entries = []
    seen = {}
    for w in works:
        key_base = bib_key(w["id"])
        n = seen.get(key_base, 0) + 1
        seen[key_base] = n
        key = key_base if n == 1 else f"{key_base}-{n}"

        fields = []
        if w.get("authors"):
            fields.append(f"  author = {{{bib_escape(' and '.join(w['authors']))}}}")
        if w.get("title"):
            fields.append(f"  title = {{{{{bib_escape(w['title'])}}}}}")
        if w.get("year") is not None:
            fields.append(f"  year = {{{w['year']}}}")
        if w.get("venue"):
            fields.append(f"  howpublished = {{{bib_escape(w['venue'])}}}")
        if w.get("doi"):
            fields.append(f"  doi = {{{w['doi']}}}")
        if w.get("source_url"):
            fields.append(f"  url = {{{w['source_url']}}}")

        note_parts = []
        status = w.get("publication_status") or "uncertain"
        if status == "accepted_upcoming":
            note_parts.append("待开会（accepted_upcoming），不得视为已正式出版")
        elif status == "preprint":
            note_parts.append("预印本（preprint），不得视为已正式出版")
        elif status == "published":
            note_parts.append("出版状态：published")
        elif status == "uncertain":
            note_parts.append("出版状态未定（uncertain）")
        else:
            note_parts.append(f"出版状态：{status}")
        if w.get("version_count", 1) > 1:
            note_parts.append(f"包含 {w['version_count']} 个版本；首选版本 {w.get('preferred_version_id')}；未做逐字比对")
        if w.get("reading_status"):
            note_parts.append(f"阅读状态：{w['reading_status']}")
        note_parts.append(f"访问日期：{access_date}")
        fields.append(f"  note = {{{bib_escape('；'.join(note_parts))}}}")

        entries.append("@misc{" + key + ",\n" + ",\n".join(fields) + "\n}")

    path.write_text("\n\n".join(entries) + "\n", encoding="utf-8")


def work_pdf_cell(work):
    pref = work["versions"][0]
    if pref.get("fulltext_available") and pref.get("pdf"):
        return md_link(pref["pdf"], "PDF")
    if pref.get("source_url"):
        return md_link(pref["source_url"], "来源")
    return "—"


def work_evidence_cell(work):
    pref = work["versions"][0]
    if pref.get("fulltext_available"):
        if pref.get("worker_report"):
            return md_link(pref["worker_report"], "证据卡")
        return "证据卡缺失"
    return f"题录深度：{md_cell(pref.get('reading_status'))}"


def render_topic_section(lines, topic, topic_works):
    if not topic_works:
        return
    topic_works = sorted(
        topic_works,
        key=lambda w: (
            w["year"] is None,
            w["year"] or 0,
            (w["title"] or "").lower(),
            w["id"],
        ),
    )
    lines.append(f"\n## {TOPIC_LABELS.get(topic, topic)}\n")
    lines.append("| 年份 / 出版状态 | 题目 | PDF/来源 | 证据卡/深度 | 相关性 |")
    lines.append("|---|---|---|---|---|")
    for w in topic_works:
        work_anchor = anchor("work", w["id"])
        year_status = f"{w['year'] if w['year'] is not None else 'null'} / {w['publication_status']}"
        title_cell = f'<a id="{work_anchor}"></a>{md_cell(w["title"])}'
        if w["version_count"] > 1:
            title_cell += f"（{w['version_count']}版）"
        lines.append(
            "| "
            + " | ".join([
                md_cell(year_status),
                title_cell,
                work_pdf_cell(w),
                work_evidence_cell(w),
                md_cell(w["relevance"]),
            ])
            + " |"
        )

    detail_works = [
        w for w in topic_works
        if w["version_count"] > 1 or w["version_notes"] or w["metadata_notes"] or w["notes"]
    ]
    if not detail_works:
        return

    lines.append("\n### 版本与说明\n")
    for w in detail_works:
        summary = f'{md_cell(w["title"])}（{w["version_count"]} 个版本）'
        lines.append(f"<details><summary>{summary}</summary>\n")
        for v in w["versions"]:
            v_anchor = anchor("version", v["id"])
            pref_mark = "（首选）" if v["is_preferred"] else ""
            year_status = f"{v['year'] if v['year'] is not None else 'null'} / {v['publication_status']}"
            parts = [f'<a id="{v_anchor}"></a>**{md_cell(v["id"])}**{pref_mark}：{md_cell(year_status)}']
            if v.get("fulltext_available"):
                if v.get("pdf"):
                    parts.append(f'PDF：{md_link(v["pdf"], v["pdf"])}')
                if v.get("worker_report"):
                    parts.append(f'证据卡：{md_link(v["worker_report"], "worker报告")}')
            else:
                if v.get("source_url"):
                    parts.append(f'来源：{md_link(v["source_url"], "题录")}')
                parts.append(f'深度：{md_cell(v.get("reading_status"))}')
            lines.append("- " + "；".join(parts))

        for note in w["version_notes"] + w["metadata_notes"] + w["notes"]:
            lines.append(f"- 说明：{md_cell(note)}")
        if w["version_count"] > 1:
            lines.append("- 未对以上版本做逐字比对；版本差异以各自版本为准。")
        lines.append("\n</details>\n")


def write_index(path: Path, works, stats, access_date):
    lines = []
    lines.append("# 共享地址翻译文献目录\n")
    lines.append(
        f"截至 {access_date}，本目录包含 {stats['unique_work_count']} 项工作，"
        f"其中 {stats['fulltext_work_count']} 项有全文、{stats['fulltext_version_count']} 个全文版本，"
        f"另有 {stats['metadata_only_count']} 条仅题录。"
    )
    lines.append(
        "全文阅读状态为 `fulltext_worker_reviewed`：证据来自模型阅读全文证据卡，"
        "并非复现实验；root 重点核查另见 `root_spotchecked` 字段。"
    )
    lines.append(
        "未对同一工作的多个版本做逐字比对；版本差异以各自版本为准，"
        "不能据此声称逐字一致。"
    )

    for topic in TOPIC_ORDER:
        render_topic_section(lines, topic, [w for w in works if w["topic"] == topic])

    other_works = [w for w in works if w["topic"] not in TOPIC_ORDER]
    render_topic_section(lines, "other", other_works)

    lines.append("\n## 仍待核的 null 元数据\n")
    lines.append("以下字段在目录生成时仍为 null/未知；未根据文件名、hint 或联网补全。\n")

    null_work_items = []
    for w in works:
        missing = set(w.get("unknown_fields", []))
        for v in w["versions"]:
            missing.update(v.get("unknown_fields", []))
        if missing:
            null_work_items.append((w, sorted(missing)))

    if not null_work_items:
        lines.append("- 无。\n")
    else:
        for w, missing in null_work_items:
            work_anchor = anchor("work", w["id"])
            lines.append(
                f'- <a id="null-{work_anchor}"></a>[{md_cell(w["title"])}](#{work_anchor})：'
                f'{", ".join(missing)} 为 null/未知'
            )

    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description="Generate offline catalog for shared translation literature.")
    parser.add_argument("--root", default=None, help="Corpus root. Defaults to the directory containing build_catalog.py.")
    args = parser.parse_args()

    root = Path(args.root).resolve() if args.root else Path(__file__).resolve().parent
    if not root.is_dir():
        print(f"ERROR: root is not a directory: {root}", file=sys.stderr)
        return 2

    access_date = derive_access_date(root)
    issues = []

    # ------------------------------------------------------------------
    # 1. catalog-seed.json
    # ------------------------------------------------------------------
    seed_raw = load_json_file(root / "catalog-seed.json", issues, [], required=True)
    if not isinstance(seed_raw, list):
        add_issue(issues, "fatal", "invalid_catalog_seed", "catalog-seed.json 必须是 list", None)
        seed_raw = []

    seed_by_id = {}
    for rec in seed_raw:
        if not isinstance(rec, dict):
            add_issue(issues, "warning", "invalid_seed_record", "catalog-seed.json 中存在非对象记录", None)
            continue
        rid = rec.get("id")
        if is_missing(rid):
            add_issue(issues, "warning", "seed_missing_id", "catalog-seed.json 记录缺少 id", None)
            continue
        rid = str(rid)
        if rid in seed_by_id:
            add_issue(issues, "warning", "duplicate_seed_id", f"catalog-seed.json 重复 id：{rid}", rid)
        seed_by_id[rid] = rec

    # ------------------------------------------------------------------
    # 2. records/download-batch*.json
    # ------------------------------------------------------------------
    download_files = sorted((root / "records").glob("download-batch*.json"))
    download_attempts = []
    for f in download_files:
        data = load_json_file(f, issues, [], required=False)
        if not isinstance(data, list):
            add_issue(issues, "warning", "invalid_download_batch", f"{f} 不是 list", None)
            continue
        for idx, rec in enumerate(data):
            if not isinstance(rec, dict):
                continue
            rec = dict(rec)
            rec["_download_file"] = str(f.relative_to(root))
            rec["_download_index"] = idx
            download_attempts.append(rec)

    success_by_id = {}
    download_missing_files = []
    for rec in download_attempts:
        rid = rec.get("id")
        if is_missing(rid):
            continue
        rid = str(rid)
        if rec.get("status") != "downloaded_extracted":
            continue
        pdf = rec.get("pdf")
        text = rec.get("text")
        if is_missing(pdf) or is_missing(text):
            add_issue(issues, "warning", "download_success_missing_paths", f"{rid}: downloaded_extracted 但缺少 pdf/text 字段", rid)
            continue
        pdf_path = root / str(pdf)
        text_path = root / str(text)
        if not pdf_path.is_file() or not text_path.is_file():
            download_missing_files.append({
                "id": rid,
                "pdf": str(pdf),
                "text": str(text),
                "download_file": rec.get("_download_file"),
            })
            continue
        # 后出现的成功记录覆盖先前的成功记录；失败尝试已在上面跳过。
        success_by_id[rid] = rec

    for item in download_missing_files:
        add_issue(
            issues,
            "warning",
            "download_success_files_missing",
            f"{item['id']}: downloaded_extracted 但本地文件不存在（未纳入全文集）",
            item["id"],
        )

    # ------------------------------------------------------------------
    # 3. 合并全文集合：seed + 成功下载
    # ------------------------------------------------------------------
    fulltext_selected = {}
    for rid, rec in seed_by_id.items():
        fulltext_selected[rid] = {
            "base": dict(rec),
            "seed_record": rec,
            "download_record": None,
            "source_kind": "catalog-seed",
        }
    for rid, rec in success_by_id.items():
        if rid in fulltext_selected:
            base = dict(fulltext_selected[rid]["base"])
            base.update(rec)
            fulltext_selected[rid] = {
                "base": base,
                "seed_record": fulltext_selected[rid]["seed_record"],
                "download_record": rec,
                "source_kind": "catalog-seed+download",
            }
        else:
            fulltext_selected[rid] = {
                "base": dict(rec),
                "seed_record": None,
                "download_record": rec,
                "source_kind": "download",
            }

    # ------------------------------------------------------------------
    # 4. records/metadata-only.json
    # ------------------------------------------------------------------
    metadata_only_raw = load_json_file(root / "records" / "metadata-only.json", issues, [], required=True)
    if not isinstance(metadata_only_raw, list):
        add_issue(issues, "fatal", "invalid_metadata_only", "records/metadata-only.json 必须是 list", None)
        metadata_only_raw = []

    metadata_only_by_id = {}
    for rec in metadata_only_raw:
        if not isinstance(rec, dict):
            add_issue(issues, "warning", "invalid_metadata_only_record", "metadata-only.json 中存在非对象记录", None)
            continue
        rid = rec.get("id")
        if is_missing(rid):
            add_issue(issues, "warning", "metadata_only_missing_id", "metadata-only.json 记录缺少 id", None)
            continue
        rid = str(rid)
        if rid in metadata_only_by_id:
            add_issue(issues, "warning", "duplicate_metadata_only_id", f"metadata-only.json 重复 id：{rid}", rid)
        metadata_only_by_id[rid] = rec

    # ------------------------------------------------------------------
    # 5. root-overrides.json / root-spotchecks.json / work-families.json
    # ------------------------------------------------------------------
    root_overrides = load_json_file(root / "records" / "root-overrides.json", issues, {}, required=False)
    if not isinstance(root_overrides, dict):
        add_issue(issues, "fatal", "invalid_root_overrides", "records/root-overrides.json 必须是 dict", None)
        root_overrides = {}
    root_overrides = {str(k): v for k, v in root_overrides.items() if isinstance(v, dict)}

    spot_raw = load_json_file(root / "records" / "root-spotchecks.json", issues, [], required=False)
    root_spotchecks = set()
    if isinstance(spot_raw, dict):
        root_spotchecks.update(str(k) for k in spot_raw.keys())
    elif isinstance(spot_raw, list):
        for item in spot_raw:
            if isinstance(item, str):
                root_spotchecks.add(item)
            elif isinstance(item, dict) and "id" in item:
                root_spotchecks.add(str(item["id"]))
    else:
        add_issue(issues, "warning", "invalid_root_spotchecks", "root-spotchecks.json 既不是 list 也不是 dict", None)

    work_families_raw = load_json_file(root / "records" / "work-families.json", issues, {}, required=False)
    if not isinstance(work_families_raw, dict):
        add_issue(issues, "fatal", "invalid_work_families", "records/work-families.json 必须是 dict", None)
        work_families_raw = {}

    work_families = {}
    for canonical, vlist in work_families_raw.items():
        canonical = str(canonical)
        if not isinstance(vlist, list):
            add_issue(issues, "fatal", "invalid_work_family_versions", f"{canonical} 的版本列表不是 list", canonical)
            continue
        versions = [str(x) for x in vlist if isinstance(x, (str, int))]
        if len(versions) != len(vlist):
            add_issue(issues, "warning", "work_family_non_string_version", f"{canonical} 版本列表包含非字符串条目", canonical)
        if len(versions) != len(set(versions)):
            add_issue(issues, "fatal", "duplicate_version_in_work_family", f"{canonical} 版本列表有重复", canonical)
        work_families[canonical] = versions

    # ------------------------------------------------------------------
    # 6. workers/G[1-8]_*-papers.json
    # ------------------------------------------------------------------
    cards = {}
    card_reports = {}
    worker_files = sorted((root / "workers").glob("G[1-8]_*-papers.json"))
    if not worker_files:
        add_issue(issues, "warning", "no_worker_files", "未找到 workers/G[1-8]_*-papers.json", None)

    for wf in worker_files:
        stem = wf.name
        if stem.endswith("-papers.json"):
            stem = stem[:-len("-papers.json")]
        report_rel = f"workers/{stem}.md"
        data = load_json_file(wf, issues, {}, required=False)
        papers = data.get("papers", []) if isinstance(data, dict) else []
        if not isinstance(papers, list):
            add_issue(issues, "warning", "invalid_worker_papers", f"{wf} 的 papers 不是 list", None)
            continue
        for paper in papers:
            if not isinstance(paper, dict):
                continue
            pid = paper.get("id")
            if is_missing(pid):
                add_issue(issues, "warning", "worker_card_missing_id", f"{wf} 中存在缺少 id 的证据卡", None)
                continue
            pid = str(pid)
            if pid in cards:
                add_issue(issues, "warning", "duplicate_worker_evidence_card", f"id {pid} 有重复 worker 证据卡", pid)
            cards[pid] = paper
            card_reports[pid] = report_rel
            if not (root / report_rel).is_file():
                add_issue(issues, "fatal", "missing_worker_report_md", f"worker报告不存在：{report_rel}", pid)

    # ------------------------------------------------------------------
    # 7. 构造全文版本
    # ------------------------------------------------------------------
    fulltext_versions = []
    for rid in sorted(fulltext_selected.keys()):
        sel = fulltext_selected[rid]
        base = sel["base"]
        card = cards.get(rid)
        override = root_overrides.get(rid, {})

        if card is None:
            add_issue(issues, "fatal", "missing_worker_evidence_card", f"全文版本缺少 worker 证据卡：{rid}", rid)

        title = pick_meta(card, override, "title")
        if is_missing(title):
            title = base.get("title_hint") or base.get("title") or rid

        authors = pick_meta(card, override, "authors", [])
        if not isinstance(authors, list):
            authors = [] if is_missing(authors) else [str(authors)]

        year = norm_empty(pick_meta(card, override, "year"))
        venue = norm_empty(pick_meta(card, override, "venue"))
        doi_raw = norm_empty(pick_meta(card, override, "doi"))
        doi, doi_valid, doi_original = clean_doi(doi_raw)

        publication_status = pick_meta(card, override, "publication_status")
        if is_missing(publication_status):
            publication_status = "uncertain"

        relevance = pick_meta(card, override, "relevance")
        if is_missing(relevance):
            relevance = "uncertain"

        topic = pick_meta(card, override, "topic")
        if is_missing(topic):
            topic = base.get("topic") or "other"

        reading_status = "fulltext_worker_reviewed"
        root_spotchecked = rid in root_spotchecks

        pdf = norm_empty(base.get("pdf") or (card.get("pdf") if card else None))
        text = norm_empty(base.get("text") or (card.get("text") if card else None))

        pdf_exists = bool(pdf and (root / str(pdf)).is_file())
        text_exists = bool(text and (root / str(text)).is_file())
        if not pdf_exists:
            add_issue(issues, "fatal", "missing_fulltext_pdf", f"全文PDF缺失：{rid} -> {pdf}", rid)
        if not text_exists:
            add_issue(issues, "fatal", "missing_fulltext_text", f"全文文本缺失：{rid} -> {text}", rid)

        pdf_sha = sha256_file(root / str(pdf)) if pdf_exists else None
        text_sha = sha256_file(root / str(text)) if text_exists else None

        pdf_sha_provided = base.get("pdf_sha256")
        if is_missing(pdf_sha_provided) and card:
            pdf_sha_provided = card.get("pdf_sha256")
        text_sha_provided = base.get("text_sha256")
        if is_missing(text_sha_provided) and card:
            text_sha_provided = card.get("text_sha256")

        if not is_missing(pdf_sha_provided) and pdf_sha and str(pdf_sha_provided) != pdf_sha:
            add_issue(issues, "warning", "pdf_sha256_mismatch", f"{rid}: pdf_sha256 与本地文件不一致", rid)
        if not is_missing(text_sha_provided) and text_sha and str(text_sha_provided) != text_sha:
            add_issue(issues, "warning", "text_sha256_mismatch", f"{rid}: text_sha256 与本地文件不一致", rid)

        metadata_note = override.get("metadata_note", base.get("metadata_note"))
        if is_missing(metadata_note):
            metadata_note = None
        notes = base.get("notes")
        version_note = override.get("version_note", base.get("version_note"))
        if is_missing(version_note):
            version_note = None
        metadata_source = override.get("metadata_source", base.get("metadata_source"))
        if is_missing(metadata_source):
            metadata_source = None

        source_url = base.get("source_url") or (card.get("source_url") if card else None)
        worker_report = card_reports.get(rid)

        version_obj = {
            "id": rid,
            "canonical_work_id": None,
            "is_preferred": False,
            "title": title,
            "authors": authors,
            "year": year,
            "venue": venue,
            "doi": doi,
            "doi_valid": doi_valid,
            "doi_original": doi_original,
            "publication_status": publication_status,
            "relevance": relevance,
            "topic": topic,
            "source_url": source_url,
            "final_url": base.get("final_url"),
            "origin": base.get("origin"),
            "source_record": base.get("source_record"),
            "retrieved": base.get("retrieved"),
            "fulltext_available": True,
            "reading_status": reading_status,
            "root_spotchecked": root_spotchecked,
            "pdf": pdf,
            "text": text,
            "pdf_exists": pdf_exists,
            "text_exists": text_exists,
            "pdf_sha256": pdf_sha,
            "pdf_sha256_provided": pdf_sha_provided,
            "text_sha256": text_sha,
            "text_sha256_provided": text_sha_provided,
            "text_chars": base.get("text_chars"),
            "bytes": base.get("bytes"),
            "worker_report": worker_report,
            "evidence_card": card,
            "metadata_note": metadata_note,
            "notes": notes,
            "version_note": version_note,
            "metadata_source": metadata_source,
            "title_hint": base.get("title_hint"),
            "venue_hint": base.get("venue_hint"),
            "raw_seed_record": sel.get("seed_record"),
            "raw_download_record": sel.get("download_record"),
            "raw_record": base,
            "root_override": override,
            "source_kind": sel.get("source_kind"),
            "unknown_fields": [],
        }
        version_obj["unknown_fields"] = missing_metadata_fields(version_obj)
        fulltext_versions.append(version_obj)

    # ------------------------------------------------------------------
    # 8. 构造仅题录版本
    # ------------------------------------------------------------------
    metadata_only_versions = []
    for rid in sorted(metadata_only_by_id.keys()):
        rec = metadata_only_by_id[rid]
        override = root_overrides.get(rid, {})

        title = pick_rec_override(rec, override, "title")
        if is_missing(title):
            title = rid

        authors = pick_rec_override(rec, override, "authors", [])
        if not isinstance(authors, list):
            authors = [] if is_missing(authors) else [str(authors)]

        year = norm_empty(pick_rec_override(rec, override, "year"))
        venue = norm_empty(pick_rec_override(rec, override, "venue"))
        doi_raw = norm_empty(pick_rec_override(rec, override, "doi"))
        doi, doi_valid, doi_original = clean_doi(doi_raw)

        publication_status = pick_rec_override(rec, override, "publication_status")
        if is_missing(publication_status):
            publication_status = "uncertain"

        relevance = pick_rec_override(rec, override, "relevance")
        if is_missing(relevance):
            relevance = "uncertain"

        topic = pick_rec_override(rec, override, "topic")
        if is_missing(topic):
            topic = "other"

        reading_status = pick_rec_override(rec, override, "reading_status")
        if is_missing(reading_status):
            reading_status = "metadata_only"

        root_spotchecked = rid in root_spotchecks

        metadata_note = override.get("metadata_note", rec.get("metadata_note"))
        if is_missing(metadata_note):
            metadata_note = None
        notes = rec.get("notes")
        version_note = override.get("version_note", rec.get("version_note"))
        if is_missing(version_note):
            version_note = None
        metadata_source = override.get("metadata_source", rec.get("metadata_source"))
        if is_missing(metadata_source):
            metadata_source = None

        source_url = pick_rec_override(rec, override, "source_url", rec.get("source_url"))

        version_obj = {
            "id": rid,
            "canonical_work_id": None,
            "is_preferred": False,
            "title": title,
            "authors": authors,
            "year": year,
            "venue": venue,
            "doi": doi,
            "doi_valid": doi_valid,
            "doi_original": doi_original,
            "publication_status": publication_status,
            "relevance": relevance,
            "topic": topic,
            "source_url": source_url,
            "final_url": rec.get("final_url"),
            "origin": rec.get("origin"),
            "source_record": rec.get("source_record"),
            "retrieved": rec.get("retrieved"),
            "fulltext_available": False,
            "reading_status": reading_status,
            "root_spotchecked": root_spotchecked,
            "pdf": None,
            "text": None,
            "pdf_exists": False,
            "text_exists": False,
            "pdf_sha256": None,
            "pdf_sha256_provided": None,
            "text_sha256": None,
            "text_sha256_provided": None,
            "text_chars": rec.get("text_chars"),
            "bytes": rec.get("bytes"),
            "worker_report": None,
            "evidence_card": None,
            "metadata_note": metadata_note,
            "notes": notes,
            "version_note": version_note,
            "metadata_source": metadata_source,
            "title_hint": rec.get("title_hint"),
            "venue_hint": rec.get("venue_hint"),
            "raw_seed_record": None,
            "raw_download_record": None,
            "raw_record": rec,
            "root_override": override,
            "source_kind": "metadata-only",
            "unknown_fields": [],
        }
        version_obj["unknown_fields"] = missing_metadata_fields(version_obj)
        metadata_only_versions.append(version_obj)

    all_versions = fulltext_versions + metadata_only_versions

    # DOI 校验记录
    invalid_dois = []
    for v in all_versions:
        if v.get("doi_original") is not None and not v.get("doi_valid"):
            invalid_dois.append({
                "id": v["id"],
                "doi_original": v.get("doi_original"),
                "doi_cleaned": v.get("doi"),
            })
            add_issue(issues, "warning", "invalid_doi_format", f"{v['id']}: DOI 格式校验失败：{v.get('doi_original')}", v["id"])

    # 版本 id 唯一性
    version_id_counts = Counter(v["id"] for v in all_versions)
    duplicate_version_ids = sorted([vid for vid, c in version_id_counts.items() if c > 1])
    for vid in duplicate_version_ids:
        add_issue(issues, "fatal", "duplicate_version_id", f"版本 id 重复：{vid}（{version_id_counts[vid]} 次）", vid)

    versions_by_id = {v["id"]: v for v in all_versions}

    # override/spotcheck 未知 id 警告
    for oid in root_overrides:
        if oid not in versions_by_id:
            add_issue(issues, "warning", "root_override_unknown_id", f"root-overrides.json 包含未在全文/题录中出现的 id：{oid}", oid)
    for sid in root_spotchecks:
        if sid not in versions_by_id:
            add_issue(issues, "warning", "root_spotcheck_unknown_id", f"root-spotchecks.json 包含未出现的 id：{sid}", sid)

    # ------------------------------------------------------------------
    # 9. 按 work-families 归并工作
    # ------------------------------------------------------------------
    works = []
    assigned_version_ids = set()
    canonical_ids = set()

    for canonical in sorted(work_families.keys()):
        if canonical in canonical_ids:
            add_issue(issues, "fatal", "duplicate_canonical_work_id", f"canonical_work_id 重复：{canonical}", canonical)
        canonical_ids.add(canonical)

        versions = []
        for vid in work_families[canonical]:
            v = versions_by_id.get(vid)
            if v is None:
                add_issue(issues, "fatal", "work_family_unknown_version", f"work-families 引用未知版本：{canonical} -> {vid}", vid)
                continue
            if vid in assigned_version_ids:
                add_issue(issues, "fatal", "version_assigned_to_multiple_works", f"版本 {vid} 被多个工作引用", vid)
            assigned_version_ids.add(vid)
            versions.append(v)

        if not versions:
            add_issue(issues, "fatal", "empty_work_family", f"work-families 中 {canonical} 没有可用版本", canonical)
            continue

        works.append(build_work(canonical, versions, True))

    for vid in sorted(versions_by_id.keys()):
        if vid in assigned_version_ids:
            continue
        if vid in canonical_ids:
            add_issue(issues, "fatal", "version_id_collides_with_canonical_work", f"版本 id 与工作 canonical id 冲突：{vid}", vid)
        works.append(build_work(vid, [versions_by_id[vid]], False))

    topic_index = {t: i for i, t in enumerate(TOPIC_ORDER)}

    def work_sort_key(w):
        return (
            topic_index.get(w["topic"], len(TOPIC_ORDER)),
            1 if w["year"] is None else 0,
            w["year"] if w["year"] is not None else 0,
            (w["title"] or "").lower(),
            w["id"],
        )

    works.sort(key=work_sort_key)

    # ------------------------------------------------------------------
    # 10. 统计与机器检查
    # ------------------------------------------------------------------
    unique_work_count = len(works)
    fulltext_work_count = sum(1 for w in works if w["fulltext_available"])
    fulltext_version_count = len(fulltext_versions)
    metadata_only_count = len(metadata_only_versions)

    fulltext_ids = {v["id"] for v in fulltext_versions}
    metadata_only_ids = {v["id"] for v in metadata_only_versions}
    work_version_ids = {v["id"] for v in all_versions}

    missing_from_works = sorted(fulltext_ids - work_version_ids)
    extra_in_works = sorted(work_version_ids - fulltext_ids - metadata_only_ids)
    coverage_pass = not missing_from_works and not extra_in_works and len(fulltext_ids) == fulltext_version_count

    missing_evidence = sorted([v["id"] for v in fulltext_versions if v["id"] not in cards])

    missing_links = []
    for v in all_versions:
        if v.get("pdf") and not (root / str(v["pdf"])).is_file():
            missing_links.append({"id": v["id"], "kind": "pdf", "path": v["pdf"]})
        if v.get("text") and not (root / str(v["text"])).is_file():
            missing_links.append({"id": v["id"], "kind": "text", "path": v["text"]})
        if v.get("worker_report") and not (root / str(v["worker_report"])).is_file():
            missing_links.append({"id": v["id"], "kind": "worker_report", "path": v["worker_report"]})

    hash_mismatches = []
    for v in fulltext_versions:
        if v.get("pdf_sha256_provided") and v.get("pdf_sha256") and str(v["pdf_sha256_provided"]) != v["pdf_sha256"]:
            hash_mismatches.append({
                "id": v["id"],
                "kind": "pdf",
                "provided": v["pdf_sha256_provided"],
                "actual": v["pdf_sha256"],
            })
        if v.get("text_sha256_provided") and v.get("text_sha256") and str(v["text_sha256_provided"]) != v["text_sha256"]:
            hash_mismatches.append({
                "id": v["id"],
                "kind": "text",
                "provided": v["text_sha256_provided"],
                "actual": v["text_sha256"],
            })

    null_items = []
    for w in works:
        missing = set(w.get("unknown_fields", []))
        for v in w["versions"]:
            missing.update(v.get("unknown_fields", []))
        if missing:
            null_items.append({
                "work_id": w["id"],
                "missing": sorted(missing),
                "work_values": {f: w.get(f) for f in sorted(missing)},
                "version_values": {v["id"]: {f: v.get(f) for f in sorted(missing)} for v in w["versions"]},
            })

    family_issues = []
    for canonical, vids in work_families.items():
        for vid in vids:
            if vid not in versions_by_id:
                family_issues.append({
                    "work_id": canonical,
                    "version_id": vid,
                    "problem": "unknown_version",
                })

    if not coverage_pass:
        add_issue(issues, "fatal", "fulltext_coverage_mismatch", "全文集合没有被各 work 版本 exactly 覆盖", None)
    if missing_links:
        add_issue(issues, "fatal", "missing_local_links", "存在被引用的本地 pdf/text/worker_report 链接缺失", None)

    checks = {
        "unique_version_ids": {
            "pass": not duplicate_version_ids,
            "duplicates": duplicate_version_ids,
        },
        "fulltext_set_covered_exactly": {
            "pass": coverage_pass,
            "fulltext_ids": sorted(fulltext_ids),
            "work_version_ids": sorted(work_version_ids),
            "missing_from_works": missing_from_works,
            "extra_in_works": extra_in_works,
            "note": "metadata-only id 不属于全文集合，但计入 work_version_ids。",
        },
        "worker_evidence_cards_exist": {
            "pass": not missing_evidence,
            "missing": missing_evidence,
        },
        "links_exist": {
            "pass": not missing_links,
            "missing": missing_links,
            "note": "仅检查本地 pdf/text/worker_report 相对路径；source_url 等外链不联网检查。",
        },
        "null_fields": {
            "pass": True,
            "note": "以下 null/未知字段已明示，未根据文件名、hint 或联网补全。",
            "items": null_items,
        },
        "doi_validation": {
            "invalid": invalid_dois,
            "note": "仅做格式校验，不联网补 DOI。",
        },
        "hash_mismatches": {
            "pass": not hash_mismatches,
            "items": hash_mismatches,
        },
        "work_family_consistency": {
            "pass": not family_issues,
            "issues": family_issues,
        },
    }

    fatal = any(i["level"] == "fatal" for i in issues)

    stats = {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "access_date": access_date,
        "root": str(root),
        "unique_work_count": unique_work_count,
        "fulltext_work_count": fulltext_work_count,
        "fulltext_version_count": fulltext_version_count,
        "metadata_only_count": metadata_only_count,
        "version_count_total": len(all_versions),
        "topics": dict(Counter(w["topic"] for w in works)),
        "publication_status": dict(Counter(w["publication_status"] for w in works)),
        "reading_status": dict(Counter(w["reading_status"] for w in works)),
        "publication_status_by_version": dict(Counter(v["publication_status"] for v in all_versions)),
        "reading_status_by_version": dict(Counter(v["reading_status"] for v in all_versions)),
        "root_spotchecked_work_count": sum(1 for w in works if w["root_spotchecked"]),
        "root_spotchecked_version_count": sum(1 for v in all_versions if v["root_spotchecked"]),
    }

    catalog_checks = {
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "access_date": access_date,
        "root": str(root),
        "fatal": fatal,
        "issues": issues,
        "checks": checks,
        "counts": stats,
    }

    # ------------------------------------------------------------------
    # 11. 写出
    # ------------------------------------------------------------------
    csv_rows = []
    for w in works:
        for v in w["versions"]:
            csv_rows.append({
                "work_id": w["id"],
                "canonical_work_id": v["canonical_work_id"],
                "version_id": v["id"],
                "is_preferred": v["is_preferred"],
                "title": v["title"],
                "authors": "; ".join(v["authors"] or []),
                "year": v["year"] if v["year"] is not None else "",
                "venue": v["venue"] if v["venue"] is not None else "",
                "topic": v["topic"],
                "relevance": v["relevance"],
                "publication_status": v["publication_status"],
                "reading_status": v["reading_status"],
                "fulltext_available": v["fulltext_available"],
                "doi": v["doi"] or "",
                "doi_valid": v["doi_valid"],
                "source_url": v["source_url"] or "",
                "pdf": v["pdf"] or "",
                "text": v["text"] or "",
                "pdf_sha256": v["pdf_sha256"] or "",
                "text_sha256": v["text_sha256"] or "",
                "worker_report": v["worker_report"] or "",
                "root_spotchecked": v["root_spotchecked"],
                "metadata_note": v["metadata_note"] or "",
                "notes": v["notes"] or "",
                "version_note": v["version_note"] or "",
                "metadata_source": v["metadata_source"] or "",
            })

    write_json(root / "catalog.json", works)
    write_json(root / "statistics.json", stats)
    write_json(root / "catalog-checks.json", catalog_checks)
    write_csv(root / "catalog.csv", csv_rows)
    write_bib(root / "references.bib", works, access_date)
    write_index(root / "INDEX.md", works, stats, access_date)

    if fatal:
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
```

必要验收建议：

1. 在语料库根目录运行 `python3 build_catalog.py --root <root>`，检查退出码：无 fatal 问题应为 0；若有全文证据卡缺失、全文 PDF/TXT 缺失、worker 报告 md 缺失、覆盖不一致，应为 2，且 `catalog-checks.json.fatal == true`。
2. 检查六个输出文件生成：`catalog.json`、`catalog.csv`、`references.bib`、`INDEX.md`、`statistics.json`、`catalog-checks.json`。
3. 检查 `catalog.csv` 以 UTF-8 BOM 开头，字段包含 `title/year/venue/topic/relevance/publication_status/doi/source_url/pdf/worker_report` 等筛选所需列。
4. 检查 `INDEX.md` 中文简介中的数字来自实际计算，不是硬编码；`catalog-checks.json.checks.unique_version_ids/fulltext_set_covered_exactly/worker_evidence_cards_exist/links_exist` 应按实际数据给出 pass 与缺失列表。
5. 抽查 `star2024` 相关工作：应合并为 1 项工作、保留多个版本、每个版本有各自 PDF/TXT/metadata/worker 报告链接，并且不能声称两版逐字一致。