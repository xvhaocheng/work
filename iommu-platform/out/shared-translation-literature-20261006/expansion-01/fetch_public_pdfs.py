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
