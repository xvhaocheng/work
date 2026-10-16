#!/usr/bin/env python3
"""Compile and run the dma_research_policy directed/reference-account test.

Production layout (all paths relative to the repository root):
  RTL       cva6/core_v_apu/fpga/src/dma_research_policy.sv
  testbench tests/research_policy_test.cpp
  build     out/tests/<variant>/           (generated; not checked in)

Three variants are built from the same sources:
  rtl       : Enabled=1, CounterWidth=64  (production width)
  rtl-small : Enabled=1, CounterWidth=8   (deliberate statistics wrap/overflow)
  rtl-off   : Enabled=0                   (feature absent, magic still readable)

Build logs stay in out/tests/<variant>/build.log and are only echoed when the
build fails.  Compilation is capped at MAX_JOBS parallel jobs.

No claim is made that these commands were executed by the authoring agent.
"""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent
RTL  = ROOT / "cva6" / "core_v_apu" / "fpga" / "src" / "dma_research_policy.sv"
TB   = ROOT / "tests" / "research_policy_test.cpp"
OUT  = ROOT / "out" / "tests"

MAX_JOBS = 12

VARIANTS = (
    ("rtl",       "full",  []),
    ("rtl-small", "small", ["-GCounterWidth=8"]),
    ("rtl-off",   "off",   ["-GEnabled=0"]),
)


def die(msg):
    print(msg)
    sys.exit(1)


if not RTL.is_file():
    die("missing RTL: %s" % RTL)
if not TB.is_file():
    die("missing testbench: %s" % TB)

for name, mode, extra in VARIANTS:
    out = OUT / name
    out.mkdir(parents=True, exist_ok=True)
    log = out / "build.log"
    args = [
        "verilator", "--cc", "--exe", "--build",
        "-Wall", "-Wno-fatal",
        "-j", str(MAX_JOBS),
        "--top-module", "dma_research_policy",
        "--Mdir", str(out),
        str(RTL), str(TB),
    ] + extra
    build = subprocess.run(args, stdout=subprocess.PIPE,
                           stderr=subprocess.STDOUT, text=True)
    log.write_text(build.stdout or "")
    if build.returncode != 0:
        die("BUILD FAILED (%s) tail:\n%s" % (name, (build.stdout or "")[-4000:]))

    run = subprocess.run([str(out / "Vdma_research_policy"), mode],
                         capture_output=True, text=True)
    if run.returncode != 0:
        die("RUN FAILED (%s):\n%s\n%s" % (name, run.stdout, run.stderr))
    print(run.stdout.strip())

print("PASS dma_research_policy builds (simulation only, no board claim)")

