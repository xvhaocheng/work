#!/usr/bin/env python3
"""Check real generated registers/frontend against the pinned pre-observation code."""
from pathlib import Path
import subprocess
root = Path(__file__).resolve().parents[1]
cva6 = root/'cva6'
out = root/'out/e4-batch-20260916/frontend-batch'
out.mkdir(parents=True, exist_ok=True)
base = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'
old = subprocess.check_output(['git', '-C', str(cva6), 'show',
    '7f086b73f19d8e4d48ed74a671e8315f827d2ce0:'+base], text=True)
(out/'before.sv').write_text(old.replace('module idma_reg64_frontend #(', 'module idma_reg64_frontend_before #(').replace('endmodule : idma_reg64_frontend', 'endmodule : idma_reg64_frontend_before'))
prim = cva6/'corev_apu/register_interface/vendor/lowrisc_opentitan/src'
files = [cva6/'vendor/pulp-platform/common_cells/include/common_cells/registers.svh',
 cva6/'vendor/pulp-platform/axi/src/axi_pkg.sv', cva6/'corev_apu/iDMA/src/idma_pkg.sv',
 cva6/'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
 prim/'prim_subreg_arb.sv', prim/'prim_subreg.sv', prim/'prim_subreg_ext.sv',
 cva6/'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
 cva6/'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv', cva6/base,
 out/'before.sv', root/'tests/idma_frontend_observer_top.sv', root/'tests/idma_frontend_batch_test.cpp']
cmd=['verilator','--cc','--exe','--build','-Wno-fatal','--top-module','idma_frontend_observer_top',
 '--Mdir',str(out),'-I'+str(cva6/'vendor/pulp-platform/common_cells/include'),
 '-I'+str(cva6/'corev_apu/iDMA/src/include')]+list(map(str,files))
with (out/'build.log').open('w') as log: subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,check=True)
subprocess.run([str(out/'Vidma_frontend_observer_top')],check=True)
