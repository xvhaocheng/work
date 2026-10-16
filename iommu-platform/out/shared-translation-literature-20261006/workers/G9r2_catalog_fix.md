## Findings

**Bug (line 161).** `rel()` is defined as `rel(root, p)` (line 56) but line 161 calls it with one argument:

```python
hashes[rel(txt)] = sha256_file(txt)
```

`sha256_file(p)` itself is fine (single-arg by design, line 16), so the `TypeError` is purely the missing `root`.

**Arity audit of every `rel(` / `resolve(` / `sha256_file(` call** (full file, 463 lines):

| Line | Call | Verdict |
|---|---|---|
| 58 | `p.resolve().relative_to(root)` | definition |
| 108 | `rel(root, p)` | ok |
| 132 | `rel(root, jp)` | ok |
| 137 | `rel(root, md)` | ok |
| 155 | `rel(root, p)` | ok |
| **161** | `rel(txt)` | **broken — the reported failure** |
| 173 | `rel(root, pdf)` | ok |
| 177 | `rel(root, txt)` | ok |
| 253, 254, 255, 256 | `rel(root, pdf)` / `rel(root, txt)` | ok |

`resolve(root, …)` is called with 3 args at lines 125, 126, 168, 169, 248, 249 — all correct. `sha256_file(p)` is 1-arg everywhere (89, 109, 133, 139, 156, 161, 173, 177). **Line 161 is the only missing-argument call in the file.**

**Output changes requested.** Only the two per-topic tables at lines 406–414 are touched:
- 证据卡 column source becomes `worker_report` (the `.md`) instead of `evidence_card` (the JSON) — line 411.
- Title cell gets `source_url` as Markdown link, falling back to the raw title — line 414.
- Anchor `<a id="…"></a>` untouched (line 409).
- The per-version detail block (line 429–430) still emits `证据卡 [workers/G7-…-papers.json](…)`, so the JSON remains in the version details. CSV columns `worker_report`/`evidence_card` (lines 316, 328–329) untouched.

## Unified diff

```diff
--- a/iommu-platform/out/shared-translation-literature-20261006/workers/G9r1-proposed-build_catalog.py
+++ b/iommu-platform/out/shared-translation-literature-20261006/workers/G9r1-proposed-build_catalog.py
@@ -158,7 +158,7 @@
         if not txt.exists():
             errors.append(f"pdf without text: {relp}")
         else:
-            hashes[rel(txt)] = sha256_file(txt)
+            hashes[rel(root, txt)] = sha256_file(txt)
         if pid not in worker_cards:
             errors.append(f"pdf without worker evidence card: {relp}")
         if pid not in fulltext:
@@ -408,7 +408,10 @@
         for work in sorted(items, key=lambda x: (x.get("year") is None, x.get("year") or 0, (x.get("title") or "").lower())):
             anchor = f'<a id="{work["id"]}"></a>'
             pdfs = [f'[{v["id"]}]({v["pdf"]})' for v in work["versions"] if v.get("pdf")]
-            cards = [f'[{v["id"]}]({v["evidence_card"]})' for v in work["versions"] if v.get("evidence_card")]
+            cards = [f'[{v["id"]}]({v["worker_report"]})' for v in work["versions"] if v.get("worker_report")]
             year = work.get("year") if work.get("year") is not None else "未知"
             status = work.get("publication_status") or "未知"
-            lines.append(f'| {year} / {status} | {anchor}{work.get("title") or ""} | {"<br>".join(pdfs) or "—"} | {"<br>".join(cards) or "—"} | {work.get("relevance") or "uncertain"} |')
+            title = work.get("title") or ""
+            src = work.get("source_url")
+            title_md = f"[{title}]({src})" if src else title
+            lines.append(f'| {year} / {status} | {anchor}{title_md} | {"<br>".join(pdfs) or "—"} | {"<br>".join(cards) or "—"} | {work.get("relevance") or "uncertain"} |')
```

## Notes / assumptions

- 2 hunks, ~10 changed lines, no new functions, no layout change, no tests added.
- `worker_reports[vid]` is `None` when the `.md` is absent (line 147), so those rows fall back to `—` exactly like today's missing-card case; the existing `missing worker report` error (line 149) still fires.
- If a work has a `source_url` but an empty/absent title, the cell renders `[](url)`. Left as-is to avoid scope creep; if undesirable, use `title_md = f"[{title or src}]({src})"`.
- Titles containing `|` would still break the table cell — pre-existing behavior, unchanged.

## Acceptance checks (to be run by the parent)

1. `python3 -c "import ast,sys;ast.parse(open('<path>').read())"` — syntax OK.
2. `grep -n "rel(" <path>` — every `rel(` occurrence except the definition (line 56/58) and the `relp = rel(root, p)` patterns now takes `root` as first arg.
3. Re-run the script; it should reach the writer stage instead of raising at line 161.
4. In generated `INDEX.md`: inside any `## <topic>` table, the 证据卡 column links end in `.md` (`workers/G[1-8]_*-papers.md`); rows whose work has no worker report show `—`.
5. Rows whose work has `source_url` render `[<title>](<url>)`; rows without it render the bare title.
6. `<a id="<work-id>"></a>` strings in tables unchanged.
7. Below each topic, the `- 版本 <id>：…` bullet still contains `证据卡 [workers/G*-papers.json](…)` (JSON retained).
8. `catalog.csv` still has both `worker_report` and `evidence_card` columns populated with the `.md` and `.json` paths respectively.