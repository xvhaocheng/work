#!/usr/bin/env python3
"""Compile and run the research observer contract test with Verilator 4.038.

Two variants are built from the same sources:
  rtl        : CounterWidth=64 (default), T_CYCLES=64
  rtl-small  : CounterWidth=8 plus T_CYCLES=64, for counter overflow evidence

The production default of T_CYCLES is 5000000 cycles; the override below is a
simulation-only convenience and is not a change of the experiment contract.
No claim is made that these commands were executed by the authoring agent.
"""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
RTL = ROOT / "cva6" / "corev_apu" / "fpga" / "src" / "dma_research_observer.sv"
TIMING = ROOT / "cva6" / "corev_apu" / "fpga" / "src" / "dma_research_e2_timing.sv"
TB = ROOT / "tests" / "research_observer_test.cpp"

VARIANTS = (("rtl", []), ("rtl-small", ["-GCounterWidth=8"]))

for name, extra in VARIANTS:
    out = ROOT / "out" / "research-observer-fullstats-20261002" / name
    out.mkdir(parents=True, exist_ok=True)
    args = [
        "verilator", "--cc", "--exe", "--build", "-Wall", "-Wno-fatal",
        "--top-module", "dma_research_observer", "--Mdir", str(out),
        "-GT_CYCLES=64", str(RTL), str(TIMING), str(TB),
    ] + extra
    with (out / "build.log").open("w") as log:
        result = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
        if result.returncode != 0:
            tail = (out / "build.log").read_text()[-4000:]
            print("BUILD FAILED (%s) tail:\n%s" % (name, tail))
            sys.exit(1)
    argv = [str(out / "Vdma_research_observer")]
    if name == "rtl-small":
        argv.append("small")
    run = subprocess.run(argv, capture_output=True, text=True)
    if run.returncode != 0:
        print("RUN FAILED (%s):\n%s\n%s" % (name, run.stdout, run.stderr))
        sys.exit(1)
    print(run.stdout.strip())

print("PASS research observer builds (simulation only, no board claim)")
