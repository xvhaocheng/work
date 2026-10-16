#!/usr/bin/env python3
"""Compile and run the P4M gap frontend test against production RTL and a
renamed pinned prechange golden module generated with git show."""
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
cva6 = root / "cva6"
out = root / "out/p4m-gap-20260920/frontend-gap"
out.mkdir(parents=True, exist_ok=True)

pin = "2b04936d2b4fe970bba4c97ab99f0a46a00eec06"
base = "corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv"
old = subprocess.check_output(["git", "-C", str(cva6), "show", f"{pin}:{base}"], text=True)
(out / "before.sv").write_text(
    old.replace("module idma_reg64_frontend #(", "module idma_reg64_frontend_before #(")
       .replace("endmodule : idma_reg64_frontend", "endmodule : idma_reg64_frontend_before")
)

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
    cva6 / base,
    out / "before.sv",
    root / "tests/idma_p4m_gap_top.sv",
    root / "tests/idma_p4m_gap_test.cpp",
]

cmd = [
    "verilator", "--cc", "--exe", "--build", "-Wno-fatal",
    "--top-module", "idma_p4m_gap_top",
    "--Mdir", str(out),
    "-I" + str(cva6 / "vendor/pulp-platform/common_cells/include"),
    "-I" + str(cva6 / "corev_apu/iDMA/src/include"),
] + list(map(str, files))

log_path = out / "build.log"
with log_path.open("w") as log:
    res = subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT)
if res.returncode != 0:
    print(log_path.read_text())
    raise SystemExit(res.returncode)

res = subprocess.run([str(out / "Vidma_p4m_gap_top")])
if res.returncode != 0:
    raise SystemExit(res.returncode)
print("PASS P4M gap build and run")
