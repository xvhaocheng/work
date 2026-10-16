#!/usr/bin/env python3
"""Compile and run the fullstats cost-group helper contract test with Verilator 4.038.

Two variants are built from the same sources:
  rtl        : StatWidth=64 (default)
  rtl-small  : StatWidth=8 for narrow overflow evidence

No claim is made that these commands were executed by the authoring agent.
"""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
RTL_SOURCE = ROOT / "cva6/corev_apu/fpga/src/dma_research_observer.sv"
HELPER_MARKER = "module dma_research_cost_groups #(\n"
TB = ROOT / "tests" / "research_cost_groups_test.cpp"

VARIANTS = (("rtl", []), ("rtl-small", ["-GStatWidth=8"]))

for name, extra in VARIANTS:
    out = ROOT / "out" / "research-cost-groups-20261002" / name
    out.mkdir(parents=True, exist_ok=True)
    # Extract the actual production helper each time; never test a stale copy.
    source = RTL_SOURCE.read_text()
    assert source.count(HELPER_MARKER) == 1
    rtl = out / "dma_research_cost_groups.sv"
    rtl.write_text(source[source.index(HELPER_MARKER):])
    args = [
        "verilator", "--cc", "--exe", "--build", "-Wall", "-Wno-fatal",
        "--top-module", "dma_research_cost_groups", "--Mdir", str(out),
        str(rtl), str(TB),
    ] + extra
    with (out / "build.log").open("w") as log:
        result = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
        if result.returncode != 0:
            tail = (out / "build.log").read_text()[-4000:]
            print("BUILD FAILED (%s) tail:\n%s" % (name, tail))
            sys.exit(1)
    argv = [str(out / "Vdma_research_cost_groups")]
    if name == "rtl-small":
        argv.append("small")
    run = subprocess.run(argv, capture_output=True, text=True)
    if run.returncode != 0:
        print("RUN FAILED (%s):\n%s\n%s" % (name, run.stdout, run.stderr))
        sys.exit(1)
    print(run.stdout.strip())

print("PASS fullstats cost-group builds (simulation only, no board claim)")
