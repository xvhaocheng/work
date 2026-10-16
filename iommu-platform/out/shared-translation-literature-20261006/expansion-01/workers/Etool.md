## 0. 结论摘要

- **A**：给出一个自包含、仅用标准库的下载工具（`fetch_public_pdfs.py`），CLI 为 `--root / --input / --output`，4 并发、单次请求 timeout=45、无重试、无 URL 探测、无凭据；PDF 头校验、`pdftotext -layout` 抽取去 NUL；已有同名 PDF+TXT 时跳过网络并标记 `reused_existing`；失败只写 `status: unavailable` + `error`；最后一次性写出与 `build_catalog.py` 兼容的记录 JSON 列表。
- **B**：`build_catalog.py:131` 的 worker glob 从 `G[1-8]_*-papers.json` 改为 `G[0-9]*_*-papers.json`，单行替换，语义不变（下方给最小 unified diff）。
- 两者均未执行、未联网；`build_catalog.py` 的约束依据是我对给定源码行的静态阅读（引用行号见各处）。

---

## A) 下载工具代码（fenced python）

```python
#!/usr/bin/env python3
"""Bounded public-PDF fetcher for the shared-translation literature catalog.

Standard-library only. Contract:

* one HTTP(S) GET per item, socket timeout 45 s, no retries, no URL probing,
  no credentials / cookies / secrets of any kind;
* a payload is written as a PDF only when its first four bytes are ``%PDF``;
* text is extracted with ``pdftotext -layout`` and NUL bytes are stripped;
* a new ``papers/<id>.pdf`` is never left behind without ``papers/<id>.txt``;
* one record file (JSON list) is written at the very end, shaped for
  ``build_catalog.py`` (``records/download-batch*.json``);
* pre-existing ``papers/<id>.pdf`` + ``papers/<id>.txt`` pairs are reused with
  no network access and flagged ``reused_existing``;
* ``local_pdf`` / ``local_text`` inputs are copied in as local full text and
  keep the caller-supplied ``origin``.

Usage:

    python3 fetch_public_pdfs.py --root <library> --input batch.json \
        --output records/download-batch7.json
"""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import re
import shutil
import subprocess
import sys
import urllib.error
import urllib.parse
import urllib.request
from pathlib import Path

PDF_MAGIC = b"%PDF"
HTTP_TIMEOUT = 45          # seconds, per item, exactly one request
MAX_WORKERS = 4            # hard cap on concurrent downloads
PDFTOTEXT_TIMEOUT = 300    # seconds, local subprocess (not a network retry)
USER_AGENT = "shared-translation-literature-fetch/1.0 (stdlib urllib; offline catalog)"
SAFE_ID = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._-]*$")


# --------------------------------------------------------------------------- #
# small helpers
# --------------------------------------------------------------------------- #
def sha256_file(path):
    h = hashlib.sha256()
    with Path(path).open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def has_pdf_header(path):
    try:
        with Path(path).open("rb") as f:
            return f.read(4) == PDF_MAGIC
    except OSError:
        return False


def clean_nul_bytes(path):
    """Drop NUL bytes from a text file in place; return character count."""
    raw = Path(path).read_bytes()
    if b"\x00" in raw:
        raw = raw.replace(b"\x00", b"")
        Path(path).write_bytes(raw)
    return len(raw.decode("utf-8", errors="replace"))


def run_pdftotext(pdf_input, txt_output):
    """`pdftotext -layout`, strip NUL, atomically place the result.

    Returns the character count of the NUL-free text. Raises RuntimeError with
    a short message on failure; the caller decides whether to keep the PDF.
    """
    tmp = Path(txt_output).with_name(Path(txt_output).name + ".raw")
    try:
        subprocess.run(
            ["pdftotext", "-layout", str(pdf_input), str(tmp)],
            check=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.PIPE,
            timeout=PDFTOTEXT_TIMEOUT,
        )
    except FileNotFoundError:
        raise RuntimeError("pdftotext not found on PATH")
    except subprocess.CalledProcessError as exc:
        detail = (exc.stderr or b"").decode("utf-8", "replace").strip()
        raise RuntimeError(f"pdftotext failed: {detail or exc.returncode}")
    except subprocess.TimeoutExpired:
        raise RuntimeError("pdftotext timeout")

    chars = clean_nul_bytes(tmp)
    if chars == 0:
        tmp.unlink(missing_ok=True)
        raise RuntimeError("pdftotext produced empty text")
    tmp.replace(txt_output)
    return chars


def relative_or_absolute(root, path):
    try:
        return Path(path).resolve().relative_to(root).as_posix()
    except Exception:
        return Path(path).as_posix()


def finish_ok(rec, item, pdf_path, txt_path, chars, origin_default, reused):
    rec["origin"] = item.get("origin") or origin_default
    rec["status"] = "downloaded_extracted"
    rec["reused_existing"] = bool(reused)
    rec["text_chars"] = chars
    rec["pdf_sha256"] = sha256_file(pdf_path)
    rec["text_sha256"] = sha256_file(txt_path)
    return rec


# --------------------------------------------------------------------------- #
# per-item work (never raises for expected failures)
# --------------------------------------------------------------------------- #
def handle_download(item, pdf_path, txt_path, rec):
    url = item.get("source_url")
    if not isinstance(url, str) or not url.strip():
        rec["status"] = "unavailable"
        rec["error"] = "no source_url and no reusable local full text"
        return rec
    url = url.strip()

    if urllib.parse.urlsplit(url).scheme.lower() not in ("http", "https"):
        rec["status"] = "unavailable"
        rec["error"] = f"refusing non-http(s) source_url: {url}"
        return rec

    # Exactly one request: no retry loop, no alternate URL probing, no auth.
    request = urllib.request.Request(
        url, headers={"User-Agent": USER_AGENT, "Accept": "application/pdf,*/*"}
    )
    try:
        with urllib.request.urlopen(request, timeout=HTTP_TIMEOUT) as response:
            data = response.read()
    except urllib.error.HTTPError as exc:
        rec["status"] = "unavailable"
        rec["error"] = f"HTTP {exc.code} for {url}"
        return rec
    except urllib.error.URLError as exc:
        rec["status"] = "unavailable"
        rec["error"] = f"network error for {url}: {exc.reason}"
        return rec
    except OSError as exc:
        rec["status"] = "unavailable"
        rec["error"] = f"network error for {url}: {exc!r}"
        return rec

    if data[:4] != PDF_MAGIC:
        rec["status"] = "unavailable"
        rec["error"] = (
            f"response for {url} is not a PDF (first bytes={data[:4]!r}, "
            f"{len(data)} bytes); nothing saved"
        )
        return rec

    part_pdf = pdf_path.with_name(pdf_path.name + ".part")
    part_txt = txt_path.with_name(txt_path.name + ".part")
    part_pdf.write_bytes(data)
    try:
        chars = run_pdftotext(part_pdf, part_txt)
    except RuntimeError as exc:
        # never leave a new PDF (or half state) behind
        part_pdf.unlink(missing_ok=True)
        part_txt.unlink(missing_ok=True)
        rec["status"] = "unavailable"
        rec["error"] = str(exc)
        return rec

    # text first, then pdf: a crash in between leaves no new PDF without text
    part_txt.replace(txt_path)
    part_pdf.replace(pdf_path)
    return finish_ok(
        rec, item, pdf_path, txt_path, chars,
        origin_default="public_http_download", reused=False,
    )


def handle_local_copy(item, root, vid, pdf_path, txt_path, rec):
    def resolve_local(value):
        if not value:
            return None
        p = Path(str(value))
        if p.is_absolute():
            return p
        cand = root / p
        if cand.exists():
            return cand
        if p.exists():
            return p
        return cand

    src_pdf = resolve_local(item.get("local_pdf"))
    src_txt = resolve_local(item.get("local_text"))
    notes = []

    if not pdf_path.exists():
        if not src_pdf or not src_pdf.is_file():
            rec["status"] = "unavailable"
            rec["error"] = f"local_pdf not found: {item.get('local_pdf')!r}"
            return rec
        if not has_pdf_header(src_pdf):
            rec["status"] = "unavailable"
            rec["error"] = f"local_pdf header is not %PDF: {src_pdf}"
            return rec
        shutil.copyfile(src_pdf, pdf_path)
        notes.append(f"copied pdf from {src_pdf}")
    elif not has_pdf_header(pdf_path):
        rec["status"] = "unavailable"
        rec["error"] = f"existing papers/{vid}.pdf header is not %PDF"
        return rec

    if not txt_path.exists():
        if src_txt and src_txt.is_file():
            shutil.copyfile(src_txt, txt_path)
            notes.append(f"copied text from {src_txt}")
        else:
            try:
                run_pdftotext(pdf_path, txt_path)
            except RuntimeError as exc:
                pdf_path.unlink(missing_ok=True)
                rec["status"] = "unavailable"
                rec["error"] = str(exc)
                return rec

    chars = clean_nul_bytes(txt_path)
    if chars == 0:
        rec["status"] = "unavailable"
        rec["error"] = f"text for {vid} is empty"
        return rec
    if notes:
        rec["local_copy_notes"] = notes
    if item.get("local_pdf") or item.get("local_text"):
        rec["local_source"] = {
            "pdf": str(item.get("local_pdf")) if item.get("local_pdf") else None,
            "text": str(item.get("local_text")) if item.get("local_text") else None,
        }
    # origin: caller-supplied origin wins, otherwise "local_copy"
    return finish_ok(
        rec, item, pdf_path, txt_path, chars,
        origin_default="local_copy", reused=False,
    )


def fetch_one(item, root, papers_dir):
    item = item if isinstance(item, dict) else {}
    vid = str(item.get("id") or "").strip()
    rec = {
        "id": vid or None,
        "topic": item.get("topic"),
        "title_hint": item.get("title_hint"),
        "venue_hint": item.get("venue_hint"),
        "source_url": item.get("source_url"),
    }
    if not vid or not SAFE_ID.match(vid):
        rec["status"] = "unavailable"
        rec["error"] = f"invalid or missing id: {vid!r}"
        return rec

    pdf_path = papers_dir / f"{vid}.pdf"
    txt_path = papers_dir / f"{vid}.txt"
    rec["pdf"] = relative_or_absolute(root, pdf_path)
    rec["text"] = relative_or_absolute(root, txt_path)

    try:
        # (1) both artefacts already local -> reuse, zero network
        if pdf_path.exists() and txt_path.exists():
            if not has_pdf_header(pdf_path):
                rec["status"] = "unavailable"
                rec["error"] = f"existing papers/{vid}.pdf header is not %PDF"
                return rec
            chars = clean_nul_bytes(txt_path)
            if chars == 0:
                rec["status"] = "unavailable"
                rec["error"] = f"existing papers/{vid}.txt is empty"
                return rec
            return finish_ok(
                rec, item, pdf_path, txt_path, chars,
                origin_default="reused_existing", reused=True,
            )

        # (2) caller-provided local full text -> copy in, zero network
        if item.get("local_pdf") or item.get("local_text"):
            return handle_local_copy(item, root, vid, pdf_path, txt_path, rec)

        # (3) single public HTTP(S) fetch
        return handle_download(item, pdf_path, txt_path, rec)
    except Exception as exc:  # last-resort guard: one bad item must not kill the batch
        rec["status"] = "unavailable"
        rec["error"] = f"unhandled error: {exc!r}"
        return rec


# --------------------------------------------------------------------------- #
# CLI
# --------------------------------------------------------------------------- #
def compact_line(rec):
    parts = [f"[{rec.get('status') or '?'}]", str(rec.get("id") or "?")]
    if rec.get("text_chars") is not None:
        parts.append(f"chars={rec['text_chars']}")
    if rec.get("reused_existing"):
        parts.append("reused_existing")
    if rec.get("error"):
        parts.append(f"error={rec['error']}")
    return " ".join(parts)


def main(argv=None):
    parser = argparse.ArgumentParser(
        description="Fetch public PDFs into papers/<id>.pdf|.txt and write one records JSON."
    )
    parser.add_argument("--root", required=True, help="文献库 root directory")
    parser.add_argument("--input", required=True, help="JSON list of items")
    parser.add_argument(
        "--output",
        required=True,
        help="record file path; absolute, or relative to --root",
    )
    args = parser.parse_args(argv)

    root = Path(args.root).expanduser().resolve()
    if not root.is_dir():
        print(f"error: --root is not a directory: {root}", file=sys.stderr)
        return 2

    input_path = Path(args.input).expanduser()
    if not input_path.is_absolute():
        input_path = root / input_path
    if not input_path.is_file():
        print(f"error: --input not found: {input_path}", file=sys.stderr)
        return 2
    try:
        items = json.loads(input_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        print(f"error: cannot read --input: {exc}", file=sys.stderr)
        return 2
    if not isinstance(items, list):
        print("error: --input must be a JSON list", file=sys.stderr)
        return 2

    # refuse duplicate ids: two workers would race on the same papers/<id>.*
    seen_ids = {}
    for idx, item in enumerate(items):
        if isinstance(item, dict) and item.get("id"):
            vid = str(item["id"])
            if vid in seen_ids:
                print(
                    f"error: duplicate id {vid!r} at input indexes "
                    f"{seen_ids[vid]} and {idx}",
                    file=sys.stderr,
                )
                return 2
            seen_ids[vid] = idx

    papers_dir = root / "papers"
    papers_dir.mkdir(parents=True, exist_ok=True)

    output_path = Path(args.output).expanduser()
    if not output_path.is_absolute():
        output_path = root / output_path

    records = [None] * len(items)
    with concurrent.futures.ThreadPoolExecutor(max_workers=MAX_WORKERS) as pool:
        futures = {
            pool.submit(fetch_one, item, root, papers_dir): idx
            for idx, item in enumerate(items)
        }
        for future in concurrent.futures.as_completed(futures):
            idx = futures[future]
            try:
                rec = future.result()
            except Exception as exc:  # defensive, should not happen
                item = items[idx] if isinstance(items[idx], dict) else {}
                rec = {
                    "id": item.get("id"),
                    "topic": item.get("topic"),
                    "title_hint": item.get("title_hint"),
                    "venue_hint": item.get("venue_hint"),
                    "source_url": item.get("source_url"),
                    "status": "unavailable",
                    "error": f"unhandled error: {exc!r}",
                }
            records[idx] = rec
            print(compact_line(rec), flush=True)  # compact one-line per item

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(  # single write at the end
        json.dumps(records, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )
    ok = sum(1 for r in records if r.get("status") == "downloaded_extracted")
    print(
        f"wrote {relative_or_absolute(root, output_path)}: "
        f"{ok}/{len(records)} downloaded_extracted"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
```

### A1. CLI / 行为对照（逐条对齐需求）

| 需求 | 实现位置 |
|---|---|
| 仅标准库、无新依赖 | import 段只用 `argparse/concurrent.futures/hashlib/json/re/shutil/subprocess/sys/urllib/Path` |
| `--root` / `--input` / `--output`（相对 root 或绝对） | `main()` 中 `input_path`、`output_path` 的相对化分支 |
| 每项 `id/topic/source_url/title_hint/venue_hint` | `rec` 初始化字典 |
| `local_pdf` / `local_text` 复用 | 分支 (2) `handle_local_copy` |
| 只 http/https | `urllib.parse.urlsplit(url).scheme.lower() not in ("http","https")` 直接判失败 |
| 最多 4 并发 | `ThreadPoolExecutor(max_workers=MAX_WORKERS)`，`MAX_WORKERS = 4` |
| 每项一次请求、timeout 45、无重试、不探测其他 URL | 单个 `urlopen(..., timeout=45)`，无循环、无候选 URL 列表 |
| 无 secret/auth | 只发 `User-Agent` 与 `Accept`，无 Cookie/Auth 头，无 netrc |
| PDF 头 `%PDF` 检查 | `data[:4] != PDF_MAGIC`（新下载）、`has_pdf_header()`（本地/既有文件） |
| 保存 `papers/<id>.pdf`、`pdftotext -layout` 到 `papers/<id>.txt`、去 NUL | `handle_download` + `run_pdftotext` + `clean_nul_bytes` |
| hashes / text_chars / source_url / status `downloaded_extracted` | `finish_ok()`，字段名 `pdf_sha256`/`text_sha256`/`text_chars`/`source_url`/`status` |
| 同名 PDF+TXT 已存在 → 跳网、核头、记 `reused_existing` | 分支 (1) |
| 失败 `status: unavailable` + `error` | 所有失败路径 |
| 不把非 PDF 存成 PDF | 头校验失败直接 return，不写 `pdf_path` |
| 不留无 TXT 的新 PDF | 先 `part_txt.replace(txt_path)` 再 `part_pdf.replace(pdf_path)`；抽取失败时 `part_pdf.unlink()` |
| localcopy 保留 `origin` | `finish_ok` 中 `item.get("origin") or origin_default`（`local_copy` 为默认） |
| 打印紧凑每项状态 + 最后一次性写 JSON | `compact_line()` 逐项打印；`write_text` 只在循环结束后调用一次 |
| 不递归扫仓库 | 全程只触碰 `root/"papers"` 与 output 路径，无 `rglob`/`glob` |

### A2. 与 `build_catalog.py` 的兼容性说明（含行号依据）

- 记录是 **JSON list**：`assert isinstance(data, list)`（`build_catalog.py:111`），文件路径需匹配 `records/download-batch*.json`（`build_catalog.py:107`）才能被收进来。
- 只有 `status == "downloaded_extracted"` 的记录进入 `fulltext`（`build_catalog.py:120-128`），并在 `pdf.exists() and txt.exists()` 时生效（`:127`）。本工具成功路径统一写 `downloaded_extracted`，因此复用项也能被目录收录。
- 工具写入的 `"pdf": "papers/<id>.pdf"`、`"text": "papers/<id>.txt"` 可被 `resolve()` 解析（`build_catalog.py:63-74`, `:125-126`）。
- 目录里 `source_url` 会进入 `references.bib` 的 `url` 字段（`build_catalog.py:353-354`）。
- 记录里多出的字段（`title_hint/venue_hint/origin/text_chars/reused_existing/pdf_sha256/text_sha256/local_source/...`）会整体进入 `base`（`build_catalog.py:231`），只会被指定键取用（`:265-269`），不会污染已有元数据；`build_catalog.py` 自己重算 hash（`:156-177`），所以记录内的 hash 仅作可读记录。
- **注意**：`build_catalog.py:162-165` 要求 `papers/*.pdf` 必须有同名 worker 证据卡与 fulltext 记录，否则报错。新抓取的条目在补 worker 卡之前会让 `build_catalog.py` 返回 1。

### A3. 已知限制 / 未决点（需 Codex 决定）

1. **`reused_existing` 的记录形式**：我按“`status` 保持 `downloaded_extracted`（否则 `build_catalog.py:120` 会跳过）+ 增加布尔 `reused_existing: true` + `origin` 默认 `reused_existing`”实现。若要求 `status` 本身取值为 `reused_existing`，需同步改 `build_catalog.py:120`，否则复用项不会进入 fulltext。
2. **重定向**：`urlopen` 默认跟随最多 10 次重定向，严格“每项一次 TCP 请求”不成立。如需硬约束，应安装禁重定向的 `OpenerDirector`。
3. **`text_chars` 定义**：取“去 NUL 后 UTF-8 解码的字符数”。给定样例 `download-batch6.json:12` 的 `138049` 无法区分字符数/字节数，需要确认口径。
4. **既有 `papers/<id>.pdf` 无 `.txt`**：未做特殊处理（本工具不会对孤立 PDF 直接本地抽取）；若走网络路径且抽取成功，会以新文件覆盖该孤立 PDF，失败则不动它。这是有意的保守行为，如需“先本地抽取”需显式指派。
5. 不处理大小上限、Content-Type、robots/ToS、断点续传、跨条目去重（重复 id 直接报错退出）。
6. `pdftotext` 必须在 PATH 上；`papers/` 若不存在会被创建。

### A4. 建议验收检查（我未执行）

1. 单条目公开 PDF fixture：`python3 fetch_public_pdfs.py --root LIB --input b.json --output records/download-batch7.json` → `papers/<id>.pdf` 前 4 字节为 `%PDF`；`papers/<id>.txt` 非空且 `grep -c $'\0'` 为 0；输出 JSON 为 list、`status == downloaded_extracted`；`sha256sum papers/<id>.pdf` 与记录 `pdf_sha256` 一致。
2. 第二次运行：把 `source_url` 换成不可达主机，仍应成功且 `reused_existing == true`（证明复用分支未联网）。
3. 指向一个 HTML 页面：`status == unavailable`，且 `papers/<id>.pdf` 不存在（非 PDF 不落盘）。
4. 把 `pdftotext` 移出 PATH：`status == unavailable`，且不留下新 `papers/<id>.pdf`。
5. `local_pdf`/`local_text` 条目：文件被复制到 `papers/<id>.*`，记录的 `origin` 等于输入 `origin`（无输入时为 `local_copy`），`local_source` 有源路径。
6. 并发上限只能靠代码审阅确认（`MAX_WORKERS = 4`），不适合离线测试。

---

## B) `build_catalog.py` 最小 unified diff

### B1. 补丁

```diff
--- a/iommu-platform/out/shared-translation-literature-20261006/build_catalog.py
+++ b/iommu-platform/out/shared-translation-literature-20261006/build_catalog.py
@@ -128,7 +128,7 @@
             fulltext[vid] = rec
 
     worker_cards, worker_json, worker_reports = {}, {}, {}
-    for jp in sorted((root / "workers").glob("G[1-8]_*-papers.json")):
+    for jp in sorted((root / "workers").glob("G[0-9]*_*-papers.json")):
         relp = rel(root, jp)
         hashes[relp] = sha256_file(jp)
         data = load_json(jp)
```

### B2. 修改后的完整代码块（其余 466 行逐字不变）

```python
    worker_cards, worker_json, worker_reports = {}, {}, {}
    for jp in sorted((root / "workers").glob("G[0-9]*_*-papers.json")):
        relp = rel(root, jp)
        hashes[relp] = sha256_file(jp)
        data = load_json(jp)
        assert isinstance(data, dict) and isinstance(data.get("papers"), list)
        md = jp.with_name(jp.name.replace("-papers.json", ".md"))
```

未重写脚本，未输出全文 466 行（这是“最小 diff + 不重写脚本”的直接结果）；其余行保持原样，唯一改动为第 131 行。

### B3. 可选更严格变体（若担心 `G1abc_...` 之类文件名）

```diff
-    for jp in sorted((root / "workers").glob("G[1-8]_*-papers.json")):
+    for jp in sorted((root / "workers").glob("G*_*-papers.json")):
+        if not re.match(r"^G\d+_.*-papers\.json$", jp.name):
+            continue
```

`re` 已在 `build_catalog.py:2` 导入，无需新增 import。

### B4. 影响与限制

- 语义保持：仍然只收集 `workers/G<数字>_*-papers.json`（下划线分隔），`.md` 同名报告查找逻辑不变（`:136-139`），缺报告仍报 `missing worker report`（`:149`），重复卡片仍报错（`:143-144`）。
- 影响面：`[1-8]` → `[0-9]*`，新增覆盖 G0、G9、G10、G11…；其余数字组行为一致。
- 未触及：主题列表 `TOPICS`（`:7`）、`FULLTEXT_STATUS`（`:8`）、任何已有元数据、CSV/Bib/INDEX 渲染逻辑、download-batch 收集（`:107`）。
- 需自行确认仓库中是否还有**其他手工维护的 G 组清单**（文档、manifest、`records/` 下别的列表），该补丁不会同步它们；建议在仓库内检索 `G1_`、`G8_`、`workers/G` 以确认没有第二处硬编码。
- 该补丁未执行、未运行 `build_catalog.py`；G10/G11 的 `.md` 报告与 `records/work-families.json` 归属仍需各自到位，否则 `:148-149` 与 `:218-219` 会继续报错。