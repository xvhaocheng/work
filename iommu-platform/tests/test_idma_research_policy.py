#!/usr/bin/env python3
"""Compile and run the dma_research_policy integration test with Verilator 4.038.
Simulation only; no board claim. Build log is printed only on failure.
"""
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
cva6 = root / "cva6"
out = root / "out/idma-research-policy-20261003"
out.mkdir(parents=True, exist_ok=True)

prim = cva6 / "corev_apu/register_interface/vendor/lowrisc_opentitan/src"
files = [
    cva6 / "vendor/pulp-platform/common_cells/include/common_cells/registers.svh",
    cva6 / "vendor/pulp-platform/axi/src/axi_pkg.sv",
    cva6 / "corev_apu/iDMA/src/idma_pkg.sv",
    cva6 / "corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv",
    prim / "prim_subreg_arb.sv",
    prim / "prim_subreg.sv",
    prim / "prim_subreg_ext.sv",
    cva6 / "corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv",
    cva6 / "corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv",
    cva6 / "corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv",
    cva6 / "corev_apu/fpga/src/dma_research_policy.sv",
    root / "tests/idma_research_policy_top.sv",
    root / "tests/idma_research_policy_test.cpp",
]

cmd = [
    "verilator", "--cc", "--exe", "--build", "-Wno-fatal",
    "--top-module", "idma_research_policy_top",
    "--Mdir", str(out),
    "-I" + str(cva6 / "vendor/pulp-platform/common_cells/include"),
    "-I" + str(cva6 / "corev_apu/iDMA/src/include"),
    "-I" + str(cva6 / "corev_apu/fpga/src"),
] + list(map(str, files))

log_path = out / "build.log"
with log_path.open("w") as log:
    res = subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT)
if res.returncode != 0:
    print(log_path.read_text())
    raise SystemExit(res.returncode)

res = subprocess.run([str(out / "Vidma_research_policy_top")])
if res.returncode != 0:
    raise SystemExit(res.returncode)
print("PASS dma_research_policy integration build and run")
