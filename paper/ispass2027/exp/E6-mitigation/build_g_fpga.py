#!/usr/bin/env python3
"""Build bounded E6 G/OBS4/Depth4 DS0 using the preserved E5 50MHz flow.

Only the independent export sets IOMMUGapEnable=1. Constraints and synthesis/
implementation directives are unchanged. No board operation or retry exists.
"""
import argparse
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import re

HERE = Path(__file__).resolve().parent
WORK = HERE.parents[3]
PLATFORM = Path('/home/xvhaocheng/work/iommu-platform')
BASE_SOURCE = PLATFORM / 'out/ispass2027-20261017-source01/source-manifest.json'
BASE_CVA6 = '6c5c666d45979d3a03fb98cff89ba7ad095f6125'
CVA6 = 'cfb9a0a8bb70b32be6036bc112a5189d6e2aa0ab'
PLATFORM_COMMIT = 'd5cc44834a89d8e1633a028ad82d6e1f26b38831'
LINUX_REFERENCE = '65b427946152e3aca27babab0a437821729805d4'
ALLOWED = ['corev_apu/fpga/src/ariane_peripherals_xilinx.sv',
           'corev_apu/fpga/src/ariane_xilinx.sv',
           'corev_apu/fpga/src/dma_observer.sv',
           'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv',
           'corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv']
RUN_TCL = 'corev_apu/fpga/scripts/run.tcl'
GENERIC_MARKER = 'set_property top ${project}_xilinx [current_fileset]'
SYNTH_MARKER = 'open_run synth_1'
GENERIC_OVERLAY = '''
# E6 G export only. All source defaults, constraints and directives are unchanged.
set_property generic {IOMMUGapEnable=1} [current_fileset]
set e6_g_generic [get_property generic [current_fileset]]
if {$e6_g_generic ne "IOMMUGapEnable=1"} { error "G top generic absent: $e6_g_generic" }
set e6_g_receipt [open "e6-g-top-generic.json" w]
puts $e6_g_receipt { {"IOMMUGapEnable":1,"evidence":"Vivado current_fileset generic before synthesis"} }
close $e6_g_receipt
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', required=True, type=Path)
    parser.add_argument('--check-source', action='store_true', help='Freeze/check source receipt only; no export or build')
    args = parser.parse_args()
    output = args.out.resolve()
    if not output.is_relative_to((HERE / 'artifacts').resolve()) or output.exists():
        parser.error('--out must name a new directory inside this E6 worktree artifacts/')
    preserved = HERE / 'source/build_baseline.as-reused.py'
    spec = importlib.util.spec_from_file_location('e6_g_preserved_builder', preserved)
    base = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(base)
    base.WORK, base.PLATFORM, base.HERE = WORK, PLATFORM, HERE / 'build'
    original_phase, original_save, original_archive = base.phase_manifest, base.save, base.archive
    helper_inputs = {}
    for name in ('scripts/platform.py', 'provenance/source-organization-20261017/export_sources.py'):
        helper = PLATFORM / name
        assert helper.read_bytes() == base.show(PLATFORM, PLATFORM_COMMIT, name)
        helper_inputs[name] = base.sha(helper)
    board_files = base.git(PLATFORM, 'ls-tree', '-r', '--name-only', PLATFORM_COMMIT, 'configs/boards').decode().splitlines()
    for name in board_files:
        assert (PLATFORM / name).read_bytes() == base.show(PLATFORM, PLATFORM_COMMIT, name)
        helper_inputs[name] = base.sha(PLATFORM / name)
    baseline, _ = base.validate_source(BASE_SOURCE)
    for repo, commit in ((PLATFORM, PLATFORM_COMMIT), (PLATFORM / 'cva6', CVA6), (PLATFORM / 'linux', LINUX_REFERENCE)):
        assert re.fullmatch('[0-9a-f]{40}', commit)
        assert base.git(repo, 'rev-parse', commit + '^{commit}').decode().strip() == commit
    changed = base.git(PLATFORM / 'cva6', 'diff', '--name-only', BASE_CVA6, CVA6).decode().splitlines()
    assert changed == ALLOWED, changed
    assert base.git(PLATFORM / 'cva6', 'rev-parse', CVA6 + ':corev_apu/riscv-iommu').decode().strip() == base.IOMMU
    lock = json.loads(base.show(PLATFORM, PLATFORM_COMMIT, 'sources.lock.json'))
    locks = {entry['path']: entry['commit'] for entry in lock['components']}
    for name, commit in (('cva6', CVA6), ('linux', LINUX_REFERENCE)):
        assert locks[name] == commit
        assert base.git(PLATFORM, 'rev-parse', PLATFORM_COMMIT + ':' + name).decode().strip() == commit
    exporter = base.load_module(PLATFORM / 'provenance/source-organization-20261017/export_sources.py', 'e6_g_resolve')
    components = exporter.resolve({'components': [{'path': 'cva6', 'commit': CVA6},
                                                  {'path': 'linux', 'commit': LINUX_REFERENCE},
                                                  {'path': 'opensbi', 'commit': base.SBI}]})
    assert {entry['path']: entry['commit'] for entry in components} == locks
    before = base.show(PLATFORM / 'cva6', CVA6, RUN_TCL)
    assert before.decode().count(GENERIC_MARKER) == before.decode().count(SYNTH_MARKER) == 1
    after = before.decode().replace(GENERIC_MARKER, GENERIC_MARKER + '\n' + GENERIC_OVERLAY)
    after = after.replace(SYNTH_MARKER, SYNTH_MARKER + '\nsource scripts/e6_g_verify_synth.tcl').encode()
    synth_verifier = (HERE / 'verify_g_synth.tcl').read_bytes()
    sha = lambda data: hashlib.sha256(data).hexdigest()
    selected = copy.deepcopy(baseline)
    selected.update(schema='ispass2027-e6-g-fpga-source-v1', selected_cva6=CVA6,
                    selected_platform=PLATFORM_COMMIT, linux=LINUX_REFERENCE, components=components,
                    source_ref='refs/heads/ispass/e6-mitigation', baseline_source_manifest=str(BASE_SOURCE),
                    baseline_source_sha256=base.sha(BASE_SOURCE), e5_base_cva6=BASE_CVA6, changed_files=changed,
                    scope='Bounded G gap plus fixed whole/window observation on E5 OBS4/Depth4; DS0',
                    exclusions=['BTC3', 'OBS5', 'M', 'P/N/PN', 'service scheduling changes', 'DS delay'],
                    linux_reference_only=True, g_linux_present=False,
                    batch_version=2, batch_magic='0x49444d4142544302', observer_version=4,
                    observer_magic='0x49444d414f425304', PathDepth=4,
                    histogram={'stages': 16, 'bins': 32, 'counter_bits': 32, 'scheme': 1},
                    gap_capability='0x49444d4147415001', g_capability='0x49444d4150344d02', g_bank=[768, 799],
                    top_generic={'IOMMUGapEnable': 1}, source_defaults={'IOMMUGapEnable': 0},
                    export_overlay={'path': RUN_TCL, 'before_sha256': sha(before), 'after_sha256': sha(after),
                                    'synth_verifier_path': 'corev_apu/fpga/scripts/e6_g_verify_synth.tcl',
                                    'synth_verifier_sha256': sha(synth_verifier),
                                    'reason': 'Explicit G generic plus synthesized structural evidence; constraints/directives unchanged'},
                    execution_helper_inputs=helper_inputs,
                    finite_evidence={str(path.relative_to(HERE)): base.sha(path) for path in
                                     (HERE / 'frontend-validation/component.json', HERE / 'observer-validation/component.json')})
    for name in ('repositories', 'profiles', 'measurement_overlay_files'):
        selected.pop(name, None)
    source = base.HERE / 'g-fpga-source.json'
    if source.exists():
        assert json.loads(source.read_text()) == selected, 'frozen G source receipt differs'
    else:
        base.save(source, selected)
    if args.check_source:
        print(json.dumps({'status': 'source_checked_no_build', 'source_manifest': str(source),
                          'sha256': base.sha(source), 'platform': PLATFORM_COMMIT, 'cva6': CVA6,
                          'linux_reference_only': LINUX_REFERENCE, 'future_out': str(output)}))
        return 0
    verifier = (HERE / 'source/verify_routed.as-reused.tcl').read_bytes()
    target = base.HERE / 'verify_routed.tcl'
    if target.exists():
        assert target.read_bytes() == verifier, 'frozen routed verifier differs'
    else:
        target.write_bytes(verifier)

    def validate(path):
        value = json.loads(Path(path).read_text())
        assert value == selected, 'G source identity mismatch'
        return value, {entry['path']: entry['commit'] for entry in value['components']}

    def archive(repo, commit, dest):
        original_archive(repo, commit, dest)
        if repo == PLATFORM / 'cva6':
            assert commit == CVA6
            tcl = dest / RUN_TCL
            assert tcl.read_bytes() == before
            tcl.write_bytes(after)
            (dest / selected['export_overlay']['synth_verifier_path']).write_bytes(synth_verifier)

    def phase(out, source_path, action):
        manifest = original_phase(out, source_path, action)
        manifest.update(schema='ispass2027-e6-g-build-v1', script_sha256=base.sha(__file__),
                        reused_baseline_builder_sha256=base.sha(preserved), change_scope=selected['scope'],
                        linux_reference_only=LINUX_REFERENCE, g_linux_present=False, batch_version=2,
                        observer_version=4, PathDepth=4, top_generic=selected['top_generic'],
                        finite_evidence=selected['finite_evidence'], export_overlay=selected['export_overlay'],
                        timing_policy='unchanged E5 50MHz constraints/strategy; WNS>=0, setup/hold/pulse failing=0, routing errors=0; terminal failure')
        base.save(out / 'build-manifest.json', manifest)
        return manifest

    def save(path, value):
        if Path(path).name == 'build-manifest.json' and 'parameters' in value:
            value['parameters'].update(observer_version=4, observer_magic=selected['observer_magic'],
                                       batch_version=2, batch_magic=selected['batch_magic'], PathDepth=4,
                                       histogram_stages=16, histogram_bins=32, histogram_counter_bits=32,
                                       histogram_scheme=1, top_generic=selected['top_generic'])
            fpga = output / 'src/cva6/corev_apu/fpga'
            actual = fpga / 'e6-g-top-generic.json'
            synth = fpga / 'e6-g-synth-evidence.json'
            inventory = fpga / 'e6-g-synth-registers.tsv'
            if actual.exists():
                setting = json.loads(actual.read_text())
                assert setting['IOMMUGapEnable'] == 1
                value['actual_vivado_top_generic'] = {'path': str(actual), 'sha256': base.sha(actual), **setting}
            if synth.exists():
                evidence = json.loads(synth.read_text())
                assert evidence['passed'] is True
                value['synthesized_configuration'] = {'path': str(synth), 'sha256': base.sha(synth), **evidence}
            if inventory.exists():
                value['synthesized_register_inventory'] = {'path': str(inventory), 'sha256': base.sha(inventory)}
            if 'artifacts' in value:
                utilization = fpga / 'reports/ariane.utilization.rpt'
                if utilization.exists():
                    value['artifacts'][str(utilization)] = base.sha(utilization)
                    value['hierarchical_utilization_report'] = str(utilization)
            if value.get('status') == 'passed':
                assert actual.exists() and synth.exists() and inventory.exists(), 'successful build lacks actual G/synthesis evidence'
        return original_save(path, value)

    base.validate_source, base.archive, base.phase_manifest, base.save = validate, archive, phase, save
    base.fpga(output, source, 'ds0')
    return 0 if json.loads((output / 'build-manifest.json').read_text())['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
