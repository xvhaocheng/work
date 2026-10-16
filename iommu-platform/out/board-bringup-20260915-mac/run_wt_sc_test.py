#!/usr/bin/env python3
"""Test the actual production WT AXI adapter, with independent AW/W/B delays."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--root', type=Path, required=True)
p.add_argument('--output', type=Path, required=True)
a = p.parse_args()
root = a.root.resolve()
out = a.output.resolve()
out.mkdir(parents=True, exist_ok=False)
tests = Path(__file__).resolve().parent
cva = root / 'cva6'
common = cva / 'vendor/pulp-platform/common_cells'
sources = [
    common/'src/cf_math_pkg.sv',
    cva/'core/include/cv64a6_imafdc_sv39_config_pkg.sv',
    cva/'core/include/riscv_pkg.sv',
    cva/'vendor/pulp-platform/fpnew/src/fpnew_pkg.sv',
    cva/'vendor/pulp-platform/axi/src/axi_pkg.sv',
    cva/'core/include/ariane_dm_pkg.sv',
    cva/'core/include/ariane_pkg.sv', cva/'core/include/ariane_axi_pkg.sv',
    cva/'core/include/wt_cache_pkg.sv', common/'src/fifo_v3.sv',
    common/'src/lzc.sv', common/'src/rr_arb_tree.sv',
    cva/'core/axi_shim.sv', cva/'core/cache_subsystem/wt_axi_adapter.sv',
    tests/'wt_sc_test_top.sv', tests/'wt_sc_test.cpp']
cmd = ['verilator', '--cc', '--exe', '--build', '-j', '4', '-Wno-fatal',
       '-DWT_DCACHE', '+incdir+'+str(common/'include'),
       '--top-module', 'wt_sc_test_top', '--Mdir', str(out/'obj')] + list(map(str,sources))
result = {'command': cmd, 'sources_sha256': {str(s):hashlib.sha256(s.read_bytes()).hexdigest() for s in sources}}
with (out/'build.log').open('w') as log:
    result['build_exit_code'] = subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT).returncode
if result['build_exit_code'] == 0:
    with (out/'test.log').open('w') as log:
        result['test_exit_code'] = subprocess.run([out/'obj/Vwt_sc_test_top'], stdout=log, stderr=subprocess.STDOUT).returncode
(out/'result.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:v for k,v in result.items() if k.endswith('exit_code')}))
raise SystemExit(result.get('test_exit_code',result['build_exit_code']))
