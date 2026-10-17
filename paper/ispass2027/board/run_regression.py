#!/usr/bin/env python3
"""Run historical short regression fixtures against the isolated build sources."""
import argparse
import datetime as dt
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

WORK = Path(__file__).resolve().parents[3]
PLATFORM = WORK / "iommu-platform"
HISTORY = "a2e847539bb638662f98dc920de0e8fd63d046e3"
BEFORE = "7f086b73f19d8e4d48ed74a671e8315f827d2ce0"
FRONTEND = "corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv"
SCRIPTS = ["test_idma_frontend_observer.py", "test_idma_frontend_batch.py",
           "test_idma_batch_pair.py", "test_dma_observer.py", "test_dma_path.py",
           "experiment_cli_test.py", "test_idma_scheduler.py"]


def git(repo, *args):
    return subprocess.check_output(["git", "-C", str(repo), *args])


def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--out", required=True, type=Path)
    ap.add_argument("--cva6", required=True, type=Path)
    ap.add_argument("--linux", required=True, type=Path)
    args = ap.parse_args()
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    (out / "tests").mkdir()
    (out / "bench").mkdir()
    (out / "cva6").symlink_to(args.cva6.resolve(), target_is_directory=True)
    (out / "linux").symlink_to(args.linux.resolve(), target_is_directory=True)
    names = git(PLATFORM, "ls-tree", "-r", "--name-only", HISTORY, "tests").decode().splitlines()
    # Freeze the small historical test directory; tests build only under this new root.
    inputs = {}
    for name in names:
        p = out / name
        p.parent.mkdir(parents=True, exist_ok=True)
        raw = git(PLATFORM, "show", HISTORY + ":" + name)
        p.write_bytes(raw)
        inputs[name] = {"original_sha256": sha(p), "adapted": False}
    fixture = out / "tests/pre-observation-idma-reg64.sv"
    fixture.write_bytes(git(PLATFORM / "cva6", "show", BEFORE + ":" + FRONTEND))
    for name in SCRIPTS:
        p = out / "tests" / name
        text = p.read_text()
        if "old = subprocess.check_output" in text:
            assert BEFORE in text
            text, count = re.subn(r"old = subprocess\.check_output\(\['git'.*?text=True\)",
                                 "old = (root/'tests/pre-observation-idma-reg64.sv').read_text()",
                                 text, flags=re.S)
            assert count == 1
            p.write_text(text)
            inputs["tests/" + name].update(adapted=True,
                adaptation="Read exact pre-observation fixture instead of git lookup in archive-only source tree")
    cli = out / "bench/idma_experiment.c"
    cli.write_bytes(git(PLATFORM, "show", HISTORY + ":bench/idma_experiment.c"))
    manifest = {"schema": "ispass2027-short-regression-v1", "start": dt.datetime.now(dt.timezone.utc).isoformat(),
                "history_commit": HISTORY, "before_frontend_commit": BEFORE,
                "cva6_source": str(args.cva6.resolve()), "linux_source": str(args.linux.resolve()),
                "inputs": {n: dict(v, actual_sha256=sha(out / n)) for n, v in inputs.items()},
                "before_fixture_sha256": sha(fixture), "cli_sha256": sha(cli),
                "subject_sha256": {n: sha(args.cva6 / n) for n in
                    [FRONTEND, "corev_apu/fpga/src/dma_observer.sv", "corev_apu/fpga/src/dma_path_observer.sv",
                     "corev_apu/fpga/src/ariane_peripherals_xilinx.sv", "vendor/pulp-platform/common_cells/src/stream_delay.sv"]},
                "driver_sha256": sha(args.linux / "drivers/misc/idma.c"),
                "commands": [], "status": "running", "dedicated_delayer_regression": "not present in selected historical test suite; actual two-profile FPGA elaboration and routed boundary checks recorded separately",
                "scope": "Short RTL event and host MMIO/CLI regression only; no long system simulation or board action"}
    path = out / "regression-manifest.json"
    def save():
        path.write_text(json.dumps(manifest, indent=2) + "\n")
    save()
    for name in SCRIPTS:
        command = [sys.executable, str(out / "tests" / name)]
        log = out / (name + ".log")
        row = {"argv": command, "cwd": str(out), "start": dt.datetime.now(dt.timezone.utc).isoformat(), "log": str(log)}
        manifest["commands"].append(row)
        save()
        with log.open("wb") as f:
            proc = subprocess.run(command, cwd=out, stdout=f, stderr=subprocess.STDOUT)
        row.update(exit_code=proc.returncode, end=dt.datetime.now(dt.timezone.utc).isoformat(), log_sha256=sha(log))
        save()
        print(name, "exit=" + str(proc.returncode), flush=True)
        if proc.returncode:
            manifest.update(status="failed", end=row["end"])
            save()
            raise SystemExit(proc.returncode)
    manifest.update(status="passed", end=dt.datetime.now(dt.timezone.utc).isoformat())
    save()


if __name__ == "__main__":
    main()
