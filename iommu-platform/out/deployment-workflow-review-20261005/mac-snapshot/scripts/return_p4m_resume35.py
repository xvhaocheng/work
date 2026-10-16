#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Return the complete P4M formal60 resume35 evidence set to Dell.

The upload never overwrites an existing result directory: the remote side must
create a new directory (``mkdir`` fails otherwise) and every file is read back
and compared byte for byte before the receipt is written.
"""

import argparse
import base64
import json
import shlex
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RUN = "20260920-231600-run-7_ie485l"
EXP = ROOT / "results" / "experiments" / RUN
SER = EXP / "serial"
BASE = "/home/xvhaocheng/work/iommu-platform/out"
DEST = "mac-p4m-formal60-resume35-" + RUN
MAX_BYTES = 32 * 1024 * 1024

RECEIVER = r'''
import base64, json, pathlib, sys
allowed = set(json.loads(sys.argv[3]))
raw = sys.stdin.buffer.read(33554433)
if len(raw) > 33554432:
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
    content = base64.b64decode(encoded, validate=True)
    target = dest / name
    with target.open("xb") as handle:
        handle.write(content)
    if target.read_bytes() != content:
        raise RuntimeError("Readback mismatch: " + name)
    counts[name] = len(content)
print(json.dumps({"directory": str(dest), "files": counts, "status": "written"}))
'''


def collect_sources():
    sources = {
        "run.json": EXP / "run.json",
        "PLAN.json": EXP / "PLAN.json",
        "board-evidence.tgz": EXP / "board-evidence.tgz",
        "FAILURE_HANDOFF-FIRST-SEGMENT.md": EXP / "FAILURE_HANDOFF-FIRST-SEGMENT.md",
        "RESULTS.md": EXP / "analysis" / "RESULTS.md",
        "summary.json": EXP / "analysis" / "summary.json",
        "records.json": EXP / "analysis" / "records.json",
        "captures.json": EXP / "analysis" / "captures.json",
        "trial_summary.json": EXP / "analysis" / "trial_summary.json",
        "resume35-RESULTS.md": EXP / "analysis-resume35" / "RESULTS.md",
        "resume35-summary.json": EXP / "analysis-resume35" / "summary.json",
        "resume35-records.json": EXP / "analysis-resume35" / "records.json",
        "resume35-captures.json": EXP / "analysis-resume35" / "captures.json",
        "resume35-trial_summary.json": EXP / "analysis-resume35" / "trial_summary.json",
        "merged60-trial_summary.json": EXP / "analysis-merged" / "merged-trial-summary.json",
        "console.raw.log": SER / "console.raw.log",
        "driver-report.json": SER / "driver-report.json",
        "preflight-report.json": SER / "preflight-report.json",
        "preflight-console.raw.log": SER / "preflight-console.raw.log",
        "upload-test-report.json": SER / "upload-test2-report.json",
        "upload-test-console.raw.log": SER / "upload-test2.raw.log",
        "extract-report.json": EXP / "evidence-extract" / "extract-report.json",
        "p4m_resume35_session.py": ROOT / "scripts" / "p4m_resume35_session.py",
        "p4m_resume35_tools.py": ROOT / "scripts" / "p4m_resume35_tools.py",
        "collect_p4m_formal60.py": ROOT / "scripts" / "collect_p4m_formal60.py",
        "platform-p4m-formal60": ROOT / "artifacts" / "software"
                                / "p4m-formal60-recovery-20260920" / "delivery"
                                / "platform-p4m-formal60",
        "delivery.json": ROOT / "artifacts" / "software"
                         / "p4m-formal60-recovery-20260920" / "delivery" / "delivery.json",
        "remaining-plan.json": ROOT / "artifacts" / "software"
                               / "p4m-formal60-recovery-20260920" / "delivery"
                               / "remaining-plan.json",
        "P4M_RESUME35_REPORT.md": ROOT / "P4M_RESUME35_REPORT.md",
    }
    missing = [name for name, path in sources.items() if not path.is_file()]
    if missing:
        raise RuntimeError("missing evidence files: %s" % ", ".join(sorted(missing)))
    return sources


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--upload", action="store_true")
    args = parser.parse_args()

    sources = collect_sources()
    files, counts = {}, {}
    for name, path in sources.items():
        data = path.read_bytes()
        files[name] = base64.b64encode(data).decode("ascii")
        counts[name] = len(data)
    destination = BASE + "/" + DEST
    if not args.upload:
        print(json.dumps({"destination": destination,
                          "bytes": sum(counts.values()), "files": counts},
                         indent=2, ensure_ascii=False))
        return 0
    wire = json.dumps(files, separators=(",", ":"))
    if len(wire.encode()) > MAX_BYTES:
        raise RuntimeError("return payload exceeds 32 MiB")
    command = shlex.join(["python3", "-c", RECEIVER, BASE, DEST, json.dumps(sorted(counts))])
    result = subprocess.run(
        ["ssh", "-o", "BatchMode=yes", "-o", "ConnectTimeout=15", "dell", command],
        input=wire, text=True, capture_output=True, check=True, timeout=900)
    remote = json.loads(result.stdout)
    if remote.get("status") != "written":
        raise RuntimeError("Unexpected remote status")
    if remote.get("directory") != destination:
        raise RuntimeError("Unexpected remote directory")
    if remote.get("files") != counts:
        raise RuntimeError("Unexpected remote file counts")
    receipt = {"destination": destination, "remote": remote, "run": RUN}
    (EXP / "upload-receipt.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(json.dumps(receipt, indent=2, ensure_ascii=False))
    return 0


if __name__ == "__main__":
    sys.exit(main())
