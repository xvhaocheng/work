#!/usr/bin/env python3
import argparse, csv, hashlib, json, re, sys
from collections import Counter, defaultdict
from pathlib import Path

ACCESS_DATE = "2026-10-06"
TOPICS = ["foundations", "gpu", "multigpu", "io", "accelerators", "lifecycle"]
FULLTEXT_STATUS = "fulltext_worker_reviewed"


def load_json(p):
    with p.open(encoding="utf-8") as f:
        return json.load(f)


def sha256_file(p):
    h = hashlib.sha256()
    with p.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def norm_doi(v):
    if not v:
        return v
    s = str(v).strip()
    for pre in ("https://doi.org/", "http://doi.org/"):
        if s.lower().startswith(pre):
            return s[len(pre):]
    return s


def bib_escape(s):
    if s is None:
        return ""
    out = []
    for ch in str(s):
        if ch == "\\":
            out.append(r"\textbackslash{}")
        elif ch in "&%_#${}":
            out.append("\\" + ch)
        else:
            out.append(ch)
    return "".join(out)


def author_list(v):
    if isinstance(v, list):
        return [str(x) for x in v if x is not None]
    if v:
        return [str(v)]
    return []


def rel(root, p):
    try:
        return p.resolve().relative_to(root).as_posix()
    except Exception:
        return p.as_posix()


def resolve(root, value, default):
    if value:
        p = Path(value)
        if p.is_absolute() and p.exists():
            return p
        cand = root / p
        if cand.exists():
            return cand
        if (root / p.name).exists():
            return root / p.name
        return cand
    return root / default


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", default=None)
    args = ap.parse_args()
    root = (Path(args.root) if args.root else Path(__file__).resolve().parent).resolve()
    errors, hashes = [], {}

    def read_source(rel_path):
        p = root / rel_path
        if not p.exists():
            errors.append(f"missing source: {rel_path}")
            return None
        hashes[rel_path] = sha256_file(p)
        return load_json(p)

    seed = read_source("catalog-seed.json") or []
    metadata = read_source("records/metadata-only.json") or []
    overrides = read_source("records/root-overrides.json") or {}
    families = read_source("records/work-families.json") or {}
    spot_raw = read_source("records/root-spotchecks.json") or []
    assert isinstance(seed, list) and isinstance(metadata, list)
    assert isinstance(overrides, dict) and isinstance(families, dict)
    if isinstance(spot_raw, list):
        spot_ids = set(spot_raw)
    elif isinstance(spot_raw, dict):
        spot_ids = set(spot_raw.get("ids") or spot_raw.keys())
    else:
        spot_ids = set()

    downloads = []
    for p in sorted((root / "records").glob("download-batch*.json")):
        relp = rel(root, p)
        hashes[relp] = sha256_file(p)
        data = load_json(p)
        assert isinstance(data, list)
        for rec in data:
            downloads.append((relp, rec))

    fulltext = {}
    for rec in seed:
        assert isinstance(rec, dict) and rec.get("id")
        fulltext[rec["id"]] = rec
    for _src, rec in downloads:
        if rec.get("status") != "downloaded_extracted":
            continue
        vid = rec.get("id")
        if not vid:
            continue
        pdf = resolve(root, rec.get("pdf"), f"papers/{vid}.pdf")
        txt = resolve(root, rec.get("text"), f"papers/{vid}.txt")
        if pdf.exists() and txt.exists():
            fulltext[vid] = rec

    worker_cards, worker_json, worker_reports = {}, {}, {}
    for jp in sorted((root / "workers").glob("G[1-8]_*-papers.json")):
        relp = rel(root, jp)
        hashes[relp] = sha256_file(jp)
        data = load_json(jp)
        assert isinstance(data, dict) and isinstance(data.get("papers"), list)
        md = jp.with_name(jp.name.replace("-papers.json", ".md"))
        md_rel = rel(root, md)
        if md.exists():
            hashes[md_rel] = sha256_file(md)
        for card in data["papers"]:
            vid = card.get("id")
            assert vid
            if vid in worker_cards:
                errors.append(f"duplicate worker card: {vid}")
            worker_cards[vid] = card
            worker_json[vid] = relp
            worker_reports[vid] = md_rel if md.exists() else None
        if not md.exists():
            errors.append(f"missing worker report: {md_rel}")

    papers_dir = root / "papers"
    paper_pdfs = sorted(papers_dir.glob("*.pdf")) if papers_dir.exists() else []
    for p in paper_pdfs:
        pid = p.stem
        relp = rel(root, p)
        hashes[relp] = sha256_file(p)
        txt = papers_dir / f"{pid}.txt"
        if not txt.exists():
            errors.append(f"pdf without text: {relp}")
        else:
            hashes[rel(txt)] = sha256_file(txt)
        if pid not in worker_cards:
            errors.append(f"pdf without worker evidence card: {relp}")
        if pid not in fulltext:
            errors.append(f"pdf without fulltext record: {relp}")

    for vid, rec in fulltext.items():
        pdf = resolve(root, rec.get("pdf"), f"papers/{vid}.pdf")
        txt = resolve(root, rec.get("text"), f"papers/{vid}.txt")
        if not pdf.exists():
            errors.append(f"fulltext pdf missing: {vid} {pdf}")
        else:
            hashes[rel(root, pdf)] = sha256_file(pdf)
        if not txt.exists():
            errors.append(f"fulltext text missing: {vid} {txt}")
        else:
            hashes[rel(root, txt)] = sha256_file(txt)
        if vid not in worker_cards:
            errors.append(f"fulltext without worker card: {vid}")
    for vid in worker_cards:
        if vid not in fulltext:
            errors.append(f"worker card without fulltext: {vid}")

    family_of, versions_of = {}, defaultdict(list)
    for canon, vids in families.items():
        assert isinstance(vids, list) and vids
        versions_of[canon] = list(vids)
        for vid in vids:
            if vid in family_of:
                errors.append(f"version {vid} in multiple families")
            family_of[vid] = canon
    for vid in fulltext:
        if vid not in family_of:
            family_of[vid] = vid
            versions_of[vid].append(vid)

    meta_by_id = {}
    for rec in metadata:
        vid = rec.get("id")
        assert vid
        if vid in meta_by_id:
            errors.append(f"duplicate metadata-only id: {vid}")
        meta_by_id[vid] = rec
        if vid in fulltext:
            errors.append(f"metadata-only also fulltext: {vid}")
        if vid in family_of:
            canon = family_of[vid]
            if vid not in versions_of[canon]:
                versions_of[canon].append(vid)
        else:
            family_of[vid] = vid
            versions_of[vid].append(vid)

    seen = Counter()
    for canon, vids in versions_of.items():
        for vid in vids:
            seen[vid] += 1
            if vid not in fulltext and vid not in meta_by_id:
                errors.append(f"unknown version id: {canon}/{vid}")
    for vid in fulltext:
        if seen[vid] != 1:
            errors.append(f"fulltext version not in exactly one work: {vid}")
    for vid in meta_by_id:
        if seen[vid] != 1:
            errors.append(f"metadata-only not in exactly one work: {vid}")

    def build_version(vid):
        in_full = vid in fulltext
        base = {}
        if in_full:
            base.update({k: v for k, v in fulltext[vid].items() if k != "status"})
        if vid in meta_by_id:
            base.update(meta_by_id[vid])
        card = worker_cards.get(vid)
        if card:
            for k, v in card.items():
                if v is not None and v != "" and v != []:
                    base[k] = v
        override = overrides.get(vid)
        if override:
            base.update(override)
        base["doi"] = norm_doi(base.get("doi"))
        base["relevance"] = base.get("relevance") or "uncertain"
        if in_full:
            base["reading_status"] = FULLTEXT_STATUS
        else:
            base.setdefault("reading_status", None)
        pdf = resolve(root, base.get("pdf") or fulltext.get(vid, {}).get("pdf"), f"papers/{vid}.pdf") if in_full else None
        txt = resolve(root, base.get("text") or fulltext.get(vid, {}).get("text"), f"papers/{vid}.txt") if in_full else None
        v = {
            "id": vid,
            "fulltext": in_full,
            "pdf": rel(root, pdf) if pdf else None,
            "text": rel(root, txt) if txt else None,
            "pdf_sha256": hashes.get(rel(root, pdf)) if pdf else None,
            "text_sha256": hashes.get(rel(root, txt)) if txt else None,
            "worker_report": worker_reports.get(vid),
            "evidence_card": worker_json.get(vid),
            "worker_evidence": card.get("evidence") if card else None,
            "worker_card": card,
            "root_override": override,
            "input_record": fulltext.get(vid) or meta_by_id.get(vid),
            "root_spotchecked": vid in spot_ids,
        }
        for k in ("title", "authors", "year", "venue", "doi", "source_url",
                  "publication_status", "relevance", "reading_status", "topic",
                  "fulltext_available", "metadata_source", "metadata_note",
                  "notes", "version_note"):
            v[k] = base.get(k)
        v["fulltext_available"] = True if in_full else bool(base.get("fulltext_available", False))
        return v

    works = []
    for canon, vids in versions_of.items():
        vmetas = [build_version(vid) for vid in vids]
        if not vmetas:
            continue
        pref = vmetas[0]
        work = {"id": canon, "preferred_version_id": pref["id"], "versions": vmetas,
                "version_count": len(vmetas)}
        for k in ("title", "authors", "year", "venue", "doi", "source_url",
                  "publication_status", "relevance", "reading_status", "topic",
                  "fulltext_available", "metadata_source", "metadata_note",
                  "notes", "version_note"):
            work[k] = pref.get(k)
        if canon not in {v["id"] for v in vmetas} and canon in overrides:
            work.update(overrides[canon])
        work["fulltext_available"] = any(v["fulltext"] for v in vmetas)
        work["root_spotchecked"] = canon in spot_ids or any(v.get("root_spotchecked") for v in vmetas)
        works.append(work)

    works.sort(key=lambda w: w["id"])
    if len({w["id"] for w in works}) != len(works):
        errors.append("duplicate work id")

    def count_field(items, field):
        return Counter((x.get(field) if x.get(field) is not None else "null") for x in items)

    stats = {
        "unique_work_count": len(works),
        "fulltext_work_count": sum(1 for w in works if w["fulltext_available"]),
        "fulltext_version_count": sum(1 for w in works for v in w["versions"] if v["fulltext"]),
        "metadata_only_count": sum(1 for w in works if not w["fulltext_available"]),
        "topics": dict(count_field(works, "topic")),
        "publication_status": dict(count_field(works, "publication_status")),
        "reading_status": dict(count_field(works, "reading_status")),
        "reading_status_versions": dict(Counter((v.get("reading_status") or "null") for w in works for v in w["versions"])),
        "root_spotchecked_works": sum(1 for w in works if w.get("root_spotchecked")),
    }

    (root / "catalog.json").write_text(json.dumps(works, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    (root / "statistics.json").write_text(json.dumps(stats, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")

    csv_fields = ["id", "title", "authors", "year", "venue", "topic", "relevance",
                  "publication_status", "reading_status", "doi", "source_url",
                  "pdf", "text", "worker_report", "evidence_card",
                  "fulltext_available", "version_count", "version_ids",
                  "metadata_source", "metadata_note", "notes", "version_note"]
    with (root / "catalog.csv").open("w", encoding="utf-8-sig", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=csv_fields)
        wr.writeheader()
        for work in works:
            pref = work["versions"][0]
            row = {k: work.get(k) for k in csv_fields}
            row["authors"] = "; ".join(author_list(work.get("authors")))
            row["pdf"] = pref.get("pdf") or ""
            row["text"] = pref.get("text") or ""
            row["worker_report"] = pref.get("worker_report") or ""
            row["evidence_card"] = pref.get("evidence_card") or ""
            row["version_ids"] = "; ".join(v["id"] for v in work["versions"])
            row["fulltext_available"] = "true" if work["fulltext_available"] else "false"
            row["year"] = work.get("year") if work.get("year") is not None else ""
            for k in csv_fields:
                if row.get(k) is None:
                    row[k] = ""
            wr.writerow(row)

    bib_lines = []
    for work in works:
        key = re.sub(r"[^A-Za-z0-9_:-]", "", work["id"]) or "ref"
        fields = []
        authors = author_list(work.get("authors"))
        if authors:
            fields.append(("author", " and ".join(authors)))
        if work.get("title"):
            fields.append(("title", work["title"]))
        if work.get("year") is not None:
            fields.append(("year", str(work["year"])))
        if work.get("venue"):
            fields.append(("howpublished", work["venue"]))
        if work.get("doi"):
            fields.append(("doi", work["doi"]))
        if work.get("source_url"):
            fields.append(("url", work["source_url"]))
        note = []
        if work.get("publication_status") in ("preprint", "accepted_upcoming"):
            note.append(work["publication_status"])
        note.append(f"访问日期：{ACCESS_DATE}")
        fields.append(("note", "；".join(note)))
        bib_lines.append(f"@misc{{{key},")
        for k, val in fields:
            if val is None or val == "":
                continue
            if k in ("doi", "url"):
                bib_lines.append(f"  {k} = {{{val}}},")
            else:
                bib_lines.append(f"  {k} = {{{bib_escape(val)}}},")
        bib_lines.append("}\n")
    (root / "references.bib").write_text("\n".join(bib_lines), encoding="utf-8")

    def is_null(v):
        return v is None or v == "" or v == []

    nulls = []
    for work in works:
        for f in ("title", "authors", "year", "venue", "doi", "topic",
                  "publication_status", "relevance", "metadata_source"):
            if is_null(work.get(f)):
                nulls.append(f"{work['id']}.{f}")
        for v in work["versions"]:
            for f in ("title", "authors", "year", "venue", "doi", "publication_status",
                      "reading_status", "relevance", "metadata_source"):
                if is_null(v.get(f)):
                    nulls.append(f"{work['id']}/{v['id']}.{f}")

    lines = [
        "# 共享地址翻译文献目录",
        "",
        f"截至 {ACCESS_DATE}。本目录由离线脚本生成，不联网。阅读层次：`{FULLTEXT_STATUS}` 表示模型已读本地全文并留证据卡；`root_spotchecked` 表示 root 对关键点另做核查；`metadata_only`/`abstract_or_indexed_excerpt` 仅题录或摘要层面。全文证据卡不是复现实验，也不声称逐字审读。",
        "",
        f"- 独立工作：{stats['unique_work_count']}",
        f"- 全文工作：{stats['fulltext_work_count']}",
        f"- 全文版本：{stats['fulltext_version_count']}",
        f"- 仅题录工作：{stats['metadata_only_count']}",
        "",
    ]
    grouped = defaultdict(list)
    for work in works:
        grouped[work.get("topic") or "unknown"].append(work)
    for topic in TOPICS + (["unknown"] if grouped.get("unknown") else []):
        items = grouped.get(topic) or []
        if not items:
            continue
        lines.append(f"## {topic}")
        lines.append("")
        lines.append("| 年份/状态 | 题目 | PDF | 证据卡 | 相关性 |")
        lines.append("|---|---|---|---|---|")
        for work in sorted(items, key=lambda x: (x.get("year") is None, x.get("year") or 0, (x.get("title") or "").lower())):
            anchor = f'<a id="{work["id"]}"></a>'
            pdfs = [f'[{v["id"]}]({v["pdf"]})' for v in work["versions"] if v.get("pdf")]
            cards = [f'[{v["id"]}]({v["evidence_card"]})' for v in work["versions"] if v.get("evidence_card")]
            year = work.get("year") if work.get("year") is not None else "未知"
            status = work.get("publication_status") or "未知"
            lines.append(f'| {year} / {status} | {anchor}{work.get("title") or ""} | {"<br>".join(pdfs) or "—"} | {"<br>".join(cards) or "—"} | {work.get("relevance") or "uncertain"} |')
        lines.append("")
        for work in sorted(items, key=lambda x: (x.get("year") is None, x.get("year") or 0, (x.get("title") or "").lower())):
            lines.append(f"- **{work['id']}** {work.get('title') or ''} — {', '.join(author_list(work.get('authors'))) or '作者未知'}；{work.get('venue') or 'venue未知'}；{work.get('doi') or 'doi未知'}；source={work.get('source_url') or '未知'}；topic={work.get('topic') or '未知'}；status={work.get('publication_status') or '未知'}；relevance={work.get('relevance') or 'uncertain'}；reading={work.get('reading_status') or '未知'}；fulltext={work.get('fulltext_available')}；root_spotchecked={work.get('root_spotchecked')}；metadata_source={work.get('metadata_source') or '未知'}")
            for f in ("metadata_note", "notes", "version_note"):
                if work.get(f):
                    lines.append(f"  - {f}: {work[f]}")
            for v in work["versions"]:
                links = []
                if v.get("pdf"):
                    links.append(f'PDF [{v["pdf"]}]({v["pdf"]})')
                if v.get("text"):
                    links.append(f'text [{v["text"]}]({v["text"]})')
                if v.get("worker_report"):
                    links.append(f'报告 [{v["worker_report"]}]({v["worker_report"]})')
                if v.get("evidence_card"):
                    links.append(f'证据卡 [{v["evidence_card"]}]({v["evidence_card"]})')
                lines.append(f"  - 版本 {v['id']}：{'; '.join(links) or '无本地文件'}；reading={v.get('reading_status') or '未知'}；status={v.get('publication_status') or '未知'}；year={v.get('year') if v.get('year') is not None else '未知'}；venue={v.get('venue') or '未知'}；doi={v.get('doi') or '未知'}；source={v.get('source_url') or '未知'}；root_spotchecked={v.get('root_spotchecked')}")
                for f in ("metadata_note", "notes", "version_note"):
                    if v.get(f):
                        lines.append(f"    - {f}: {v[f]}")
        lines.append("")
    lines.append("## 待核 null 字段")
    lines.append("")
    lines.extend(f"- {x}" for x in nulls)
    lines.append("")
    (root / "INDEX.md").write_text("\n".join(lines), encoding="utf-8")

    checks = {
        "errors": errors,
        "all_papers_pdf_have_text_and_card": not any("pdf without" in e for e in errors),
        "every_fulltext_has_worker_card": all(v in worker_cards for v in fulltext),
        "every_fulltext_version_in_unique_work": all(seen[v] == 1 for v in fulltext),
        "missing_local_files": [e for e in errors if "missing" in e or "pdf" in e or "text" in e],
        "null_fields": nulls,
        "file_sha256": hashes,
        "external_sources_online_tested": False,
    }
    (root / "catalog-checks.json").write_text(json.dumps(checks, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")

    if errors:
        print("catalog failed:", file=sys.stderr)
        for e in errors:
            print(" -", e, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
