#!/usr/bin/env python3
"""Run the real transaction controller and dependencies with a modeled translation engine."""
import hashlib
import json
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
out = root / 'out/e3-followup-20260918/controller-test'
out.mkdir(parents=True, exist_ok=True)
iommu = root / 'cva6/corev_apu/riscv-iommu'
common = root / 'cva6/vendor/pulp-platform/common_cells'
axi = root / 'cva6/vendor/pulp-platform/axi'
sources = [
    common / 'src/cf_math_pkg.sv',
    root / 'cva6/core/include/cv64a6_imafdc_sv39_config_pkg.sv',
    root / 'cva6/core/include/riscv_pkg.sv',
    root / 'cva6/vendor/pulp-platform/fpnew/src/fpnew_pkg.sv',
    axi / 'src/axi_pkg.sv', root / 'cva6/core/include/ariane_dm_pkg.sv',
    root / 'cva6/core/include/ariane_pkg.sv',
    iommu / 'packages/rv_iommu_cfg_pkg.sv', iommu / 'packages/rv_iommu_pkg.sv',
    iommu / 'rtl/transaction_controller/rv_iommu_e3_queue.sv',
    iommu / 'rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv',
    iommu / 'rtl/transaction_controller/rv_iommu_axi_bc.sv',
    iommu / 'rtl/translation_logic/rv_iommu_e3_delay.sv',
    root / 'tests/e3_queue_controller_top.sv', root / 'tests/e3_queue_controller_test.cpp',
]
result = {'scope': 'Real controller, FIFOs, demux, queue and delay; modeled translation engine; no board validation',
          'sources_sha256': {str(p.relative_to(root)): hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},
          'variants': []}
for enabled in (1, 0):
    dest = out / f'enabled-{enabled}'
    dest.mkdir(exist_ok=True)
    command = ['verilator', '--cc', '--exe', '--build', '-j', '4', '-Wno-fatal',
               '--top-module', 'e3_queue_controller_top', '--Mdir', str(dest / 'obj'),
               f'-GE3Enable={enabled}', '-y', str(common / 'src'), '-y', str(axi / 'src'),
               '+incdir+' + str(common / 'include'), '+incdir+' + str(axi / 'include'),
               '+incdir+' + str(iommu / 'include'), *map(str, sources)]
    record = {'E3Enable': enabled, 'command': command}
    with (dest / 'build.log').open('w') as log:
        record['build_exit_code'] = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT).returncode
    if record['build_exit_code'] == 0:
        with (dest / 'test.log').open('w') as log:
            record['test_exit_code'] = subprocess.run([dest / 'obj/Ve3_queue_controller_top'], stdout=log,
                                                      stderr=subprocess.STDOUT).returncode
    result['variants'].append(record)
    (out / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
    exit_code = record.get('test_exit_code', record['build_exit_code'])
    print(json.dumps({k: v for k, v in record.items() if k != 'command'}), flush=True)
    if exit_code:
        raise SystemExit(exit_code)
print('PASS real E3 controller integration: enabled and disabled variants')
