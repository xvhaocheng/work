#!/usr/bin/env python3
"""Create deterministic, verified gzip copies while preserving local raw traces."""
from __future__ import annotations

import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import sys

sys.dont_write_bytecode = True
from identity import sha256, utc

TASK = Path(__file__).resolve().parent.parent
TRACE_NAMES = ("events.csv", "events.jsonl", "dead-trace.csv")


def write_json(path: Path, value: dict) -> None:
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(json.dumps(value, indent=2, ensure_ascii=False) + "\n")
    os.replace(temporary, path)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--campaign", type=Path, action="append", required=True)
    parser.add_argument("--out", type=Path, required=True, help="New compression receipt within task")
    args = parser.parse_args()
    output = args.out.resolve()
    if not output.is_relative_to(TASK) or output == TASK or output.exists():
        parser.error("receipt must be new and within this task")
    inputs = []
    seen = set()
    campaigns = []
    for value in args.campaign:
        path = (value / "campaign.json" if value.is_dir() else value).resolve(strict=True)
        if not path.is_relative_to(TASK / "runs"):
            parser.error("campaign must be inside this task/runs")
        campaign = json.loads(path.read_text())
        campaigns.append(dict(path=str(path), sha256=sha256(path), complete=campaign.get("complete")))
        for case in campaign.get("cases", []):
            directory = Path(case["output"]).resolve(strict=True)
            if not directory.is_relative_to(TASK / "runs"):
                parser.error("case directory escapes task/runs")
            for name in TRACE_NAMES:
                original = directory / name
                if original.is_file():
                    if original in seen or original.with_suffix(original.suffix + ".gz").exists():
                        parser.error("duplicate trace or compressed output already exists: " + str(original))
                    seen.add(original)
                    inputs.append(original)
    output.parent.mkdir(parents=True, exist_ok=True)
    receipt = dict(schema="dead-translation-lossless-gzip-v1", state="running", started_utc=utc(),
                   originals_preserved=True, published_form=".gz only; raw paths in original manifests can be restored by gzip decompression",
                   campaigns=campaigns, files=[])
    write_json(output, receipt)
    for original in inputs:
        original_hash = sha256(original)
        original_size = original.stat().st_size
        target = original.with_suffix(original.suffix + ".gz")
        temporary = target.with_suffix(target.suffix + ".tmp")
        with original.open("rb") as source, temporary.open("xb") as destination:
            with gzip.GzipFile(filename="", mode="wb", fileobj=destination, compresslevel=9, mtime=0) as zipped:
                for block in iter(lambda: source.read(1024 * 1024), b""):
                    zipped.write(block)
        restored_hash = hashlib.sha256()
        restored_size = 0
        with gzip.open(temporary, "rb") as stream:
            for block in iter(lambda: stream.read(1024 * 1024), b""):
                restored_hash.update(block)
                restored_size += len(block)
        if (restored_hash.hexdigest() != original_hash or restored_size != original_size
                or sha256(original) != original_hash):
            receipt.update(state="failed", failure="raw or round-trip identity changed: " + str(original), finished_utc=utc())
            write_json(output, receipt)
            raise RuntimeError(receipt["failure"])
        os.rename(temporary, target)
        receipt["files"].append(dict(original=str(original), original_sha256=original_hash, original_bytes=original_size,
                                      gzip=str(target), gzip_sha256=sha256(target), gzip_bytes=target.stat().st_size,
                                      roundtrip_sha256=restored_hash.hexdigest(), roundtrip_bytes=restored_size,
                                      roundtrip_verified=True))
        write_json(output, receipt)
    receipt.update(state="passed", finished_utc=utc(), files_compressed=len(inputs),
                   original_bytes=sum(row["original_bytes"] for row in receipt["files"]),
                   gzip_bytes=sum(row["gzip_bytes"] for row in receipt["files"]))
    write_json(output, receipt)
    print(json.dumps({key: receipt[key] for key in ("state", "files_compressed", "original_bytes", "gzip_bytes")}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
