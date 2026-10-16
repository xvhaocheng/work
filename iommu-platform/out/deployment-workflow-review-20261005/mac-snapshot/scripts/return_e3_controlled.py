#!/usr/bin/env python3
"""Return fixed E3 controlled evidence to a new Dell directory, never overwrite."""
import argparse
import base64
import json
import shlex
import subprocess
from pathlib import Path
from fpga import ROOT

RUN = "20260918-164635-run-rk48bhte"
BASE = "/home/xvhaocheng/work/iommu-platform/out"
DEST = "mac-e3-controlled-" + RUN
EXP = ROOT / "results" / "experiments" / RUN
UART = ROOT / "results" / "serial" / "20260918-164542-console-libjqis3" / "console.raw.log"
SOURCES = {
    "run.json": EXP / "run.json",
    "short-assessment.json": EXP / "short-assessment.json",
    "main-assessment.json": EXP / "main-assessment.json",
    "short-records.json": EXP / "short-records.json",
    "main-records.json": EXP / "main-records.json",
    "short-summary.json": EXP / "short-summary.json",
    "main-summary.json": EXP / "main-summary.json",
    "host-validation.json": EXP / "host-validation.json",
    "E3_CONTROLLED_RESULTS.md": ROOT / "E3_CONTROLLED_RESULTS.md",
    "console.raw.log": UART,
    "all-original-pairs.json": EXP / "all-original-pairs.json",
    "delivery.json": ROOT / "artifacts/software/e3-controlled-20260918/delivery/delivery.json",
    "e3_controlled_fields.py": ROOT / "scripts/e3_controlled_fields.py",
    "collect_e3_controlled.py": ROOT / "scripts/collect_e3_controlled.py",
    "test_e3_controlled_fields.py": ROOT / "tests/test_e3_controlled_fields.py",
    "test_collect_e3_controlled.py": ROOT / "scripts/test_collect_e3_controlled.py",
}
ALLOWED = tuple(SOURCES)
MAX_BYTES = 8 * 1024 * 1024
RECEIVER = r'''
import base64, json, pathlib, sys
allowed = set(json.loads(sys.argv[3]))
raw = sys.stdin.buffer.read(8388609)
if len(raw) > 8388608:
    raise RuntimeError("payload too large")
files = json.loads(raw)
base = pathlib.Path(sys.argv[1])
if base.resolve() != base or not base.is_dir():
    raise RuntimeError("Unexpected base directory")
if set(files) != allowed:
    raise RuntimeError("Unexpected file set")
for name in files:
    if pathlib.Path(name).name != name:
        raise RuntimeError("Unexpected file path")
dest = base / sys.argv[2]
dest.mkdir(exist_ok=False)
counts = {}
for name, encoded in files.items():
    if name not in allowed or not isinstance(encoded, str):
        raise RuntimeError("Unexpected file entry")
    content = base64.b64decode(encoded, validate=True)
    target = dest / name
    with target.open("xb") as handle:
        handle.write(content)
    if target.read_bytes() != content:
        raise RuntimeError("Readback mismatch: " + name)
    counts[name] = len(content)
print(json.dumps({"directory": str(dest), "files": counts, "status": "written"}))
'''


def load_sources():
    files = {}
    counts = {}
    for name, path in SOURCES.items():
        data = path.read_bytes()
        files[name] = base64.b64encode(data).decode("ascii")
        counts[name] = len(data)
    return files, counts


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--upload", action="store_true")
    args = parser.parse_args()
    files, counts = load_sources()
    destination = BASE + "/" + DEST
    if not args.upload:
        print(json.dumps({"destination": destination, "bytes": sum(counts.values()), "files": counts}))
        return
    wire = json.dumps(files, separators=(",", ":"))
    if len(wire.encode()) > MAX_BYTES:
        raise RuntimeError("Return payload exceeds 8 MiB")
    command = shlex.join(["python3", "-c", RECEIVER, BASE, DEST, json.dumps(ALLOWED)])
    result = subprocess.run(
        ["ssh", "-o", "BatchMode=yes", "-o", "ConnectTimeout=15", "dell", command],
        input=wire, text=True, capture_output=True, check=True, timeout=120)
    remote = json.loads(result.stdout)
    if remote.get("status") != "written":
        raise RuntimeError("Unexpected remote status")
    if remote.get("directory") != destination:
        raise RuntimeError("Unexpected remote directory")
    if remote.get("files") != counts:
        raise RuntimeError("Unexpected remote file counts")
    receipt = {"destination": destination, "remote": remote, "run": RUN}
    (EXP / "upload-receipt.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps(receipt, indent=2))


if __name__ == "__main__":
    main()
