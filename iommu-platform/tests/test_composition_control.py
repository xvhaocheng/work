#!/usr/bin/env python3
# SPDX-License-Identifier: Solderpad-0.51
"""Build and run the composition-control Verilator test.

Builds the default model and a generation-saturation model compiled with
+define+CVC_TEST_GEN_FULL, then runs tests/composition_control_test.cpp against
each.  Exit status is non-zero on the first failing model.

This follows the existing Verilator-compatibility style used by
tests/test_nonblock_observer.py and does not claim that the tests have passed.
"""
from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
SRC = os.path.join(ROOT, "cva6", "corev_apu", "fpga", "src")
DUT = os.path.join(SRC, "dma_composition_control.sv")
TOP = os.path.join(HERE, "composition_control_top.sv")
CPP = os.path.join(HERE, "composition_control_test.cpp")
TOPNAME = "composition_control_top"


def build_and_run(verilator: str, gen_full: bool, build_root: str) -> int:
    tag = "gen_full" if gen_full else "default"
    obj = os.path.join(build_root, "obj_" + tag)
    os.makedirs(obj, exist_ok=True)
    cmd = [
        verilator,
        "--cc", "--exe", "--build",
        "-O2", "-Wno-fatal",
        "--top-module", TOPNAME,
        "-Mdir", obj,
        TOP, DUT, CPP,
    ]
    if gen_full:
        cmd.append("+define+CVC_TEST_GEN_FULL")
    print(f"[build] {tag}: {' '.join(cmd)}", flush=True)
    subprocess.run(cmd, check=True, cwd=ROOT)
    exe = os.path.join(obj, "V" + TOPNAME)
    print(f"[run] {exe}", flush=True)
    return subprocess.run([exe], cwd=obj).returncode


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--verilator", default=shutil.which("verilator") or "verilator")
    ap.add_argument(
        "--build-root",
        default=os.path.join(ROOT, "out", "build_composition"),
    )
    args = ap.parse_args()
    args.build_root = os.path.abspath(args.build_root)

    rc = 0
    for gen_full in (False, True):
        try:
            run_rc = build_and_run(args.verilator, gen_full, args.build_root)
        except subprocess.CalledProcessError as exc:
            print(f"build failed for gen_full={gen_full}: {exc}", file=sys.stderr)
            return 2
        if run_rc != 0 and rc == 0:
            rc = run_rc
    return rc


if __name__ == "__main__":
    sys.exit(main())
