#!/usr/bin/env python3
# SPDX-License-Identifier: Solderpad-0.51
"""Build and run the composition-observer linkage Verilator test.

Verilator 4.038, -Wno-fatal, single model built into
out/composition-trim-20261005/integration.  The full build log is written to
<build-root>/build.log and only a failing build is tailed.  No driver, board or
AXI-conservation claim is made: the test drives the DUT only through the
observer register interface and the real ARM/OPEN/CLOSE plumbing.
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

OBSERVER = os.path.join(SRC, "dma_observer.sv")
PATH_OBSERVER = os.path.join(SRC, "dma_path_observer.sv")
COMPOSITION = os.path.join(SRC, "dma_composition_control.sv")
TOP = os.path.join(HERE, "composition_observer_top.sv")
CPP = os.path.join(HERE, "composition_observer_test.cpp")
TOPNAME = "composition_observer_top"
DEFAULT_BUILD_ROOT = os.path.join(ROOT, "out", "composition-trim-20261005", "integration")


def main() -> int:
    parser = argparse.ArgumentParser(description="Composition observer test")
    parser.add_argument(
        "--verilator",
        default=shutil.which("verilator") or "verilator",
    )
    parser.add_argument("--build-root", default=DEFAULT_BUILD_ROOT)
    args = parser.parse_args()

    build_root = os.path.abspath(args.build_root)
    obj = os.path.join(build_root, "obj")
    os.makedirs(obj, exist_ok=True)
    log_path = os.path.join(build_root, "build.log")

    cmd = [
        args.verilator, "--cc", "--exe", "--build",
        "-O2", "-Wno-fatal",
        "--top-module", TOPNAME,
        "-Mdir", obj,
        TOP, OBSERVER, PATH_OBSERVER, COMPOSITION, CPP,
    ]
    print(f"[build] {' '.join(cmd)}", flush=True)
    with open(log_path, "w") as log:
        log.write(" ".join(cmd) + "\n")
        log.flush()
        rc = subprocess.run(
            cmd, stdout=log, stderr=subprocess.STDOUT, cwd=ROOT
        ).returncode
    if rc != 0:
        print(f"BUILD FAILED (rc={rc}); tail of {log_path}:", file=sys.stderr)
        with open(log_path) as log:
            for line in log.readlines()[-40:]:
                sys.stderr.write(line)
        return 2

    exe = os.path.join(obj, "V" + TOPNAME)
    print(f"[run] {exe}", flush=True)
    return subprocess.run([exe], cwd=obj).returncode


if __name__ == "__main__":
    sys.exit(main())
