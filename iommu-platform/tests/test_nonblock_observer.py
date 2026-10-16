#!/usr/bin/env python3
# SPDX-License-Identifier: Solderpad-0.51
"""Build and run the non-block observer Verilator test.

Builds the Mode 2 model (default) and, unless --no-mode1 is given, a Mode 1
model compiled with +define+NB_MODE1, then compiles and runs
tests/nonblock_observer_test.cpp against each. Exit status is non-zero on the
first failing model.

Verilator 4.038 rejects default values on module inputs.  The production RTL
keeps one such default on dma_nonblock_observer's diag_events port for backward
compatibility.  The observer top always drives that port explicitly, so this
harness compiles a temporary build copy with only that initializer removed.
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
DUT = os.path.join(SRC, "dma_nonblock_observer.sv")
TOP = os.path.join(HERE, "nonblock_observer_top.sv")
CPP = os.path.join(HERE, "nonblock_observer_test.cpp")
TOPNAME = "nonblock_observer_top"

# Verilator 4.038 error:
#   Unsupported: Default value on module input: 'diag_events'
# Remove only the initializer in a temporary build copy.  The top explicitly
# drives this port, so the default has no simulation effect.
OBSERVER_DIAG_DEFAULT = b"input  logic [15:0] diag_events = 16'd0"
OBSERVER_DIAG_NO_DEFAULT = b"input  logic [15:0] diag_events"
TOP_DIAG_CONNECTION = b".diag_events(diag_events)"


def prepare_observer_source(build_root: str) -> str:
    """Return a Verilator-4.038-compatible copy of the observer source.

    Only the unsupported default initializer is removed.  Every other byte,
    port, and logic expression remains identical.
    """
    os.makedirs(build_root, exist_ok=True)

    with open(DUT, "rb") as f:
        dut_bytes = f.read()
    with open(TOP, "rb") as f:
        top_bytes = f.read()

    default_count = dut_bytes.count(OBSERVER_DIAG_DEFAULT)
    if default_count != 1:
        raise RuntimeError(
            f"expected exactly one {OBSERVER_DIAG_DEFAULT.decode('ascii')!r} "
            f"in {DUT}, found {default_count}"
        )

    connection_count = top_bytes.count(TOP_DIAG_CONNECTION)
    if connection_count != 1:
        raise RuntimeError(
            f"expected exactly one explicit "
            f"{TOP_DIAG_CONNECTION.decode('ascii')!r} connection in {TOP}, "
            f"found {connection_count}"
        )

    compat_bytes = dut_bytes.replace(
        OBSERVER_DIAG_DEFAULT, OBSERVER_DIAG_NO_DEFAULT, 1
    )
    if compat_bytes == dut_bytes:
        raise RuntimeError("observer compatibility transform did not change the source")
    if OBSERVER_DIAG_DEFAULT in compat_bytes:
        raise RuntimeError(
            "observer compatibility transform left a default initializer"
        )

    compat_dut = os.path.join(build_root, "dma_nonblock_observer.compat.sv")
    with open(compat_dut, "wb") as f:
        f.write(compat_bytes)

    print(
        "[compat] Verilator 4.038 does not support default values on module inputs",
        flush=True,
    )
    print(f"[compat] original source: {DUT}", flush=True)
    print(
        f"[compat] transform: removed exactly one "
        f"{OBSERVER_DIAG_DEFAULT.decode('ascii')!r}",
        flush=True,
    )
    print(
        f"[compat] top explicitly drives {TOP_DIAG_CONNECTION.decode('ascii')}: {TOP}",
        flush=True,
    )
    print(f"[compat] temporary source: {compat_dut}", flush=True)

    return compat_dut


def build_and_run(verilator: str, mode: int, build_root: str, dut_source: str) -> int:
    obj = os.path.join(build_root, f"obj_mode{mode}")
    os.makedirs(obj, exist_ok=True)
    cmd = [
        verilator,
        "--cc", "--exe", "--build",
        "-O2", "-Wno-fatal",
        "--top-module", TOPNAME,
        "-Mdir", obj,
        TOP, dut_source, CPP,
    ]
    if mode == 1:
        cmd.append("+define+NB_MODE1")
    print(f"[build] mode={mode}: {' '.join(cmd)}", flush=True)
    subprocess.run(cmd, check=True, cwd=ROOT)
    exe = os.path.join(obj, "V" + TOPNAME)
    print(f"[run] {exe}", flush=True)
    return subprocess.run([exe], cwd=obj).returncode


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--verilator", default=shutil.which("verilator") or "verilator")
    ap.add_argument("--no-mode1", action="store_true")
    ap.add_argument("--build-root",
                    default=os.path.join(ROOT, "out", "build_nonblock"))
    args = ap.parse_args()
    args.build_root = os.path.abspath(args.build_root)

    observer_dut = prepare_observer_source(args.build_root)

    modes = [2] if args.no_mode1 else [2, 1]
    rc = 0
    for mode in modes:
        try:
            run_rc = build_and_run(
                args.verilator, mode, args.build_root, observer_dut
            )
        except subprocess.CalledProcessError as exc:
            print(f"build failed for mode {mode}: {exc}", file=sys.stderr)
            return 2
        if run_rc != 0 and rc == 0:
            rc = run_rc
    return rc


if __name__ == "__main__":
    sys.exit(main())
