#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the composition-IOTLB allocation test.

The reference IOTLB is obtained *at run time* from a pre-existing revision,
exactly as tests/test_research_iotlb.py does:

    git -C cva6/corev_apu/riscv-iommu show \
        137f5e5fb1f2cc65a1346359226b16247c3f8445:rtl/translation_logic/rv_iommu_iotlb.sv

The extracted copy is written under out/composition-iotlb/rtl/ and *only* its
module declaration identifier is renamed (rv_iommu_iotlb ->
rv_iommu_iotlb_reference).  The checked-in RTL is never written to.

tests/composition_iotlb_top.sv instantiates the current rv_iommu_iotlb with
PolicyEnable=1 (under test), PolicyEnable=0 (in-revision baseline) and the
extracted pre-change reference, all with the same configuration
(NumIotlbEntries = 16) and the same inputs.  The C++ testbench compares
lu_hit_o/lu_content_o on every evaluation and checks the passive research tap.

This is an actual-RTL test; no Python model of the replacement policy is used.
It proves functional equivalence of the functional outputs within the exercised
sequences and does not model FPGA physical timing.
"""

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
IOMMU = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
OUT = ROOT / "out" / "composition-iotlb"

# Frozen H1 history probe (2026-10-06): independent output tree.  The default
# tree above is never written by the probe branch and vice versa.
HISTORY_PROBE_OUT = ROOT / "out" / "b1-causal-20261006" / "iotlb-probe"

REF_SHA = "137f5e5fb1f2cc65a1346359226b16247c3f8445"
REF_REL = "rtl/translation_logic/rv_iommu_iotlb.sv"
REF_MODULE = "rv_iommu_iotlb_reference"

TOP_MODULE = "composition_iotlb_top"


def tail(path: Path, lines: int = 200) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def extract_reference(out: Path) -> Path:
    """git-show the prior revision and rename only the module declaration.

    The extracted module is required for elaboration in both branches because
    composition_iotlb_top always instantiates rv_iommu_iotlb_reference; the
    history-probe branch simply ignores its outputs.
    """
    rtl_dir = out / "rtl"
    rtl_dir.mkdir(parents=True, exist_ok=True)
    ref_file = rtl_dir / "rv_iommu_iotlb_reference.sv"

    shown = subprocess.run(
        ["git", "-C", str(IOMMU), "show", "{}:{}".format(REF_SHA, REF_REL)],
        capture_output=True, text=True)
    if shown.returncode != 0:
        sys.stderr.write(shown.stderr)
        sys.exit("git show failed for {}:{} (is the history available?)".format(REF_SHA, REF_REL))

    src = shown.stdout
    patched, count = re.subn(
        r"(?m)^([ \t]*module[ \t]+)rv_iommu_iotlb\b",
        lambda m: m.group(1) + REF_MODULE,
        src, count=1)
    if count != 1:
        sys.exit("could not locate the rv_iommu_iotlb module declaration in the extracted reference")
    if patched.count(REF_MODULE) != 1:
        sys.exit("unexpected number of {} occurrences after renaming".format(REF_MODULE))

    ref_file.write_text(patched)
    return ref_file


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p not in seen:
            seen.add(p)
            out.append(p)
    return out


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description="composition IOTLB allocation test")
    ap.add_argument("--history-probe", action="store_true",
                    help="run the frozen H1 history probe instead of the default suite")
    ap.add_argument("--probe-out", default=None,
                    help="probe output dir (default: out/b1-causal-20261006/iotlb-probe)")
    cli = ap.parse_args(argv)

    if cli.probe_out and not cli.history_probe:
        ap.error("--probe-out requires --history-probe")

    if cli.history_probe:
        # Relative --probe-out is resolved against cwd so Verilator --Mdir and
        # the exe's file paths are all absolute.
        out = (Path(cli.probe_out) if cli.probe_out else HISTORY_PROBE_OUT).resolve()
    else:
        out = OUT

    verilator_extra = ["-GExplicitTouch=1"] if cli.history_probe else []
    run_extra = ["--history-probe", "--probe-out={}".format(out)] if cli.history_probe else []

    ref_file = extract_reference(out)
    cfg_pkg = IOMMU / "packages" / "rv_iommu_cfg_pkg.sv"

    sources = dedup([
        IOMMU / "packages" / "rv_iommu_reg_pkg.sv",   # must precede rv_iommu_pkg
        cfg_pkg,
        IOMMU / "packages" / "rv_iommu_pkg.sv",
        IOMMU / "rtl" / "translation_logic" / "rv_iommu_iotlb.sv",
        ref_file,
        ROOT / "tests" / "composition_iotlb_top.sv",
    ])
    tb = ROOT / "tests" / "composition_iotlb_test.cpp"

    for path in sources + [tb]:
        if not path.exists():
            sys.exit("required file missing: {}".format(path))

    obj_dir = out / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = ["verilator", "--cc", "--exe", "--build",
            "-Wall", "-Wno-fatal",
            "--top-module", TOP_MODULE,
            "--Mdir", str(obj_dir)]
    args += verilator_extra
    args += [str(p) for p in sources]
    args += [str(tb)]

    build_log = out / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        # The build log is only inspected on failure; never dumped otherwise.
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP_MODULE)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.exit("verilator did not produce {}".format(exe))

    run = subprocess.run([str(exe)] + run_extra)
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
