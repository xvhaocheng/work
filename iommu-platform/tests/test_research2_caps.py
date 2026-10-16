#!/usr/bin/env python3
"""Build the research observer capability/register test for roles 1, 2, 0.
Reuses the existing dma_research_observer top directly. Simulation only.
Role 2 policy behaviour is covered separately by test_idma_research_policy.py.
"""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
RTL = ROOT / "cva6" / "corev_apu" / "fpga" / "src" / "dma_research_observer.sv"
TIMING = ROOT / "cva6" / "corev_apu" / "fpga" / "src" / "dma_research_e2_timing.sv"
TB = ROOT / "tests" / "research2_caps_test.cpp"

for role in (1, 2, 0):
    out = ROOT / "out" / "research2-caps-20261003" / ("role" + str(role))
    out.mkdir(parents=True, exist_ok=True)
    args = [
        "verilator", "--cc", "--exe", "--build", "-Wno-fatal",
        "--top-module", "dma_research_observer", "--Mdir", str(out),
        "-GResearchRole=" + str(role), "-GT_CYCLES=64",
        str(RTL), str(TIMING), str(TB),
    ]
    with (out / "build.log").open("w") as log:
        result = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if result.returncode != 0:
        tail = (out / "build.log").read_text()[-4000:]
        print("BUILD FAILED role=%d tail:\n%s" % (role, tail))
        sys.exit(1)
    run = subprocess.run([str(out / "Vdma_research_observer"), str(role)],
                         capture_output=True, text=True)
    if run.returncode != 0:
        print("RUN FAILED role=%d:\n%s\n%s" % (role, run.stdout, run.stderr))
        sys.exit(1)
    print(run.stdout.strip())

print("PASS research2 caps builds and runs (simulation only, no board claim)")
