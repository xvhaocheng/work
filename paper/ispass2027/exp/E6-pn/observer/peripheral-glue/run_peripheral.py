#!/usr/bin/env python3
"""Bounded real-peripheral compile/run; reads selected source in place, no archive copy."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

HERE = Path(__file__).resolve().parent


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--cva6', type=Path, required=True)
    ap.add_argument('--iommu', type=Path, required=True)
    ap.add_argument('--verilator', type=Path, required=True)
    ap.add_argument('--output', type=Path, required=True)
    ap.add_argument('--baseline-deps', type=Path,
                    default=Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6'))
    ap.add_argument('--elaborate-only', action='store_true')
    ap.add_argument('--allow-dirty-elaboration', action='store_true')
    ap.add_argument('--jobs', type=int, default=4)
    ap.add_argument('--reuse-model', type=Path, help='Previous result.json with the identical compiled RTL/config; rebuild only C++')
    args = ap.parse_args()
    c, i, out = args.cva6.resolve(), args.iommu.resolve(), args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    report = {'schema': 'ispass2027.e6_pn.peripheral_finite.v1', 'status': 'preparing',
              'fixture_only': True, 'synthetic': True, 'source_board_cases': 0,
              'hardware_access': False, 'full_build': False, 'commands': [],
              'scope': 'actual peripheral four DMA/AXI adapters/IOMMU; board I/O disabled; host AXI memory'}

    def save():
        (out / 'result.json').write_text(json.dumps(report, indent=2) + '\n')

    def git(root, *argv):
        return subprocess.check_output(['git', '-C', str(root), *argv], text=True).strip()

    def command(argv, name, timeout):
        item = {'argv': [str(x) for x in argv], 'log': str(out / (name + '.log'))}
        report['commands'].append(item)
        save()
        with Path(item['log']).open('w') as log:
            proc = subprocess.run(item['argv'], stdout=log, stderr=subprocess.STDOUT, timeout=timeout)
        item['exit_code'] = proc.returncode
        save()
        if proc.returncode:
            raise RuntimeError(name + ' failed; original local log retained')

    started = time.monotonic()
    try:
        def cv(relative):
            actual = c / relative
            # Unchanged gitlink dependencies may be uninitialized in the task
            # tree. Reuse the existing frozen baseline, never a current HEAD.
            return actual if actual.exists() else args.baseline_deps.resolve() / relative

        report['sources'] = {}
        for name, root in [('cva6', c), ('iommu', i)]:
            dirty = git(root, 'diff')
            report['sources'][name] = {'root': str(root), 'commit': git(root, 'rev-parse', 'HEAD'), 'diff': dirty}
            if dirty and not (args.elaborate_only and args.allow_dirty_elaboration):
                raise ValueError('functional run requires frozen clean ' + name + ' production source')
        if args.allow_dirty_elaboration and not args.elaborate_only:
            raise ValueError('dirty allowance is only an explicit preliminary elaboration')
        top = i / 'rtl/rv_iommu_top.sv'
        text = top.read_text()
        for token in ['pn_fullbuffer_jobs_i', 'pn_quiet_o', 'pn_n_fire_o', 'pn_history_isolate_i']:
            if token not in text:
                raise ValueError('selected actual IOMMU top lacks ' + token)
        # Package ordering is the real CVA Makefile ordering, reduced to this cone.
        packages = [cv(x) for x in [
            'core/include/cv64a6_imafdc_sv39_config_pkg.sv', 'core/include/riscv_pkg.sv',
            'core/include/ariane_dm_pkg.sv', 'core/include/ariane_pkg.sv',
            'vendor/pulp-platform/axi/src/axi_pkg.sv', 'core/include/ariane_axi_pkg.sv',
            'vendor/pulp-platform/common_cells/src/cf_math_pkg.sv',
            'core/include/axi_intf.sv', 'corev_apu/register_interface/src/reg_intf.sv']]
        packages += [i / 'packages/rv_iommu_cfg_pkg.sv', c / 'corev_apu/tb/ariane_soc_pkg.sv',
                     c / 'corev_apu/tb/ariane_axi_soc_pkg.sv', c / 'corev_apu/iDMA/src/idma_pkg.sv',
                     c / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
                     c / 'corev_apu/iDMA/src/frontends/register_64bit_2d/idma_reg64_2d_frontend_reg_pkg.sv',
                     i / 'packages/rv_iommu_pkg.sv', i / 'packages/rv_iommu_field_pkg.sv',
                     i / 'packages/rv_iommu_reg_pkg.sv', i / 'rtl/ext_interfaces/axi_iommu_intf.sv']
        packages.insert(-1, i / 'packages/rv_iommu_dti_ats_pkg.sv')
        pool = set()
        for root in [cv(x) for x in ['vendor/pulp-platform/common_cells/src', 'vendor/pulp-platform/axi/src',
                     'corev_apu/iDMA/src', 'corev_apu/register_interface/src',
                     'corev_apu/register_interface/vendor/lowrisc_opentitan/src',
                     'corev_apu/rv_plic/rtl', 'corev_apu/fpga/src/axi_slice/src',
                     'corev_apu/fpga/src/axi2apb/src']] + [i / 'rtl', i / 'vendor/modified']:
            pool.update(root.rglob('*.sv'))
        pool.update([c / 'corev_apu/fpga/src' / n for n in
                     ['dma_observer.sv', 'dma_path_observer.sv',
                      'dma_pn_control.sv', 'dma_pn_observer.sv', 'ariane_peripherals_xilinx.sv']])
        module_files = {}
        contents = {}
        for file in sorted(pool):
            if file.parent.name == 'deprecated' and file.name != 'fifo_v2.sv':
                continue
            if str(file.parent).endswith('/rv_plic/rtl') and file.name not in {
                    'rv_plic_target.sv', 'rv_plic_gateway.sv', 'plic_regmap.sv', 'plic_top.sv'}:
                continue
            data = re.sub(r'//[^\n]*|/\*.*?\*/', '', file.read_text(), flags=re.S)
            contents[file] = data
            for name in re.findall(r'^\s*module\s+(\w+)', data, re.M):
                if name in module_files:
                    raise ValueError('ambiguous source module ' + name)
                module_files[name] = file
        # Resolve only actual module dependencies. Disabled board IP may remain
        # absent; every DMA/IOMMU module is the real selected production file.
        required = {c / 'corev_apu/fpga/src/ariane_peripherals_xilinx.sv', top}
        queue = list(required)
        while queue:
            data = contents[queue.pop()]
            for name, file in module_files.items():
                if file not in required and re.search(r'\b' + re.escape(name) + r'\s*(?:#\s*\(|\w+\s*\()', data):
                    required.add(file)
                    queue.append(file)
        def source_order(path):
            name = str(path)
            rank = 0 if '/common_cells/src/' in name else 1 if '/axi/src/' in name else 2
            return rank, name
        sources = list(dict.fromkeys(packages + sorted(required, key=source_order) + [HERE / 'peripheral_top.sv']))
        includes = [cv(x) for x in ['vendor/pulp-platform/common_cells/include', 'vendor/pulp-platform/axi/include',
                    'corev_apu/register_interface/include', 'corev_apu/iDMA/src/include',
                    'core/include', 'corev_apu/rv_plic/rtl',
                    'corev_apu/register_interface/vendor/lowrisc_opentitan/src',
                    'corev_apu/fpga/src/axi2apb/include']] + [i / 'include']
        files = sources + [HERE / 'peripheral_test.cpp', HERE / 'config.vlt', Path(__file__).resolve()]
        report['input_files'] = [{'path': str(p), 'sha256': hashlib.sha256(p.read_bytes()).hexdigest()} for p in files]
        report['verilator'] = subprocess.check_output([str(args.verilator), '--version'], text=True).strip()
        common = ['--assert', '--inline-mult', '1', '-Wno-fatal', '-Werror-LATCH', '+define+ASSERTS_OFF',
                  '--top-module', 'pn_peripheral_top', *['-I' + str(x) for x in includes],
                  str(HERE / 'config.vlt'), *map(str, sources)]
        if args.elaborate_only:
            command([args.verilator, '--lint-only', *common], 'elaborate', 180)
            report['status'] = 'passed_elaboration_only'
        else:
            model = out / 'obj'
            if args.reuse_model:
                previous = json.loads(args.reuse_model.read_text())
                for name in ['cva6', 'iommu']:
                    if previous['sources'][name] != report['sources'][name]:
                        raise ValueError('reuse model production identity differs')
                current_inputs = {x['path']: x['sha256'] for x in report['input_files']}
                for item in previous['input_files']:
                    if Path(item['path']).name not in ['peripheral_test.cpp', 'run_peripheral.py']:
                        if current_inputs.get(item['path']) != item['sha256']:
                            raise ValueError('reuse model RTL/config differs: ' + item['path'])
                model = args.reuse_model.resolve().parent / 'obj'
                report['reused_model'] = str(args.reuse_model.resolve())
                command(['make', '-C', model, '-f', 'Vpn_peripheral_top.mk', '-j', args.jobs], 'rebuild_test', 120)
            else:
                command([args.verilator, '--cc', '--exe', '--build', '-j', args.jobs,
                         '--Mdir', model, '-CFLAGS', '-std=c++17 -O1', *common,
                         HERE / 'peripheral_test.cpp'], 'build', 600)
            command([model / 'Vpn_peripheral_top'], 'run', 90)
            report['status'] = 'passed_targeted_peripheral_finite'
    except Exception as error:
        report.update(status='failed', error=str(error))
    report['elapsed_s'] = time.monotonic() - started
    save()
    print(json.dumps({'status': report['status'], 'output': str(out), 'error': report.get('error')}))
    return 0 if report['status'].startswith('passed_') else 1


if __name__ == '__main__':
    raise SystemExit(main())
