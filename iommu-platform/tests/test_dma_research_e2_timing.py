#!/usr/bin/env python3
"""Compile and run the E2 timing directed test with Verilator 4.038.

Two variants:
  rtl        : CounterWidth=64
  rtl-small  : CounterWidth=8 for counter-overflow evidence
Simulation only; no board claim and no execution claim.
"""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
RTL = ROOT / "cva6" / "corev_apu" / "fpga" / "src" / "dma_research_e2_timing.sv"
TB = ROOT / "tests" / "dma_research_e2_timing_test.cpp"

VARIANTS = (("rtl", []), ("rtl-small", ["-GCounterWidth=8"]))

for name, extra in VARIANTS:
    out = ROOT / "out" / "dma-research-e2-timing-20261003" / name
    out.mkdir(parents=True, exist_ok=True)
    args = [
        "verilator", "--cc", "--exe", "--build", "-Wall", "-Wno-fatal",
        "--top-module", "dma_research_e2_timing", "--Mdir", str(out),
        str(RTL), str(TB),
    ] + extra
    with (out / "build.log").open("w") as log:
        result = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
        if result.returncode != 0:
            tail = (out / "build.log").read_text()[-4000:]
            print("BUILD FAILED (%s) tail:\n%s" % (name, tail))
            sys.exit(1)
    argv = [str(out / "Vdma_research_e2_timing")]
    if name == "rtl-small":
        argv.append("small")
    run = subprocess.run(argv, capture_output=True, text=True)
    if run.returncode != 0:
        print("RUN FAILED (%s):\n%s\n%s" % (name, run.stdout, run.stderr))
        sys.exit(1)
    print(run.stdout.strip())

print("PASS E2 timing builds (simulation only, no board claim)")
