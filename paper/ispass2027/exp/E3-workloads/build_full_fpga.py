#!/usr/bin/env python3
"""Build accepted BTC3 + OBS5 DS0 through the preserved 50MHz FPGA flow.

All source defaults remain depth4. The exported E3 build alone sets the Vivado
top generic to64 before synthesis, records the actual setting, and keeps every
existing timing/routing failure condition. No board operation is present.
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
BASE_CVA6 = '8aac3fcd345af7686a55af1867783fd6b91a0069'
CVA6 = '1d587125eee690e623e6ffae13e70f9bbe989adc'
LINUX_REFERENCE = 'da6b69fc007ad5e671dc288dd8038d02e3ebe43d'
PLATFORM_COMMIT = 'dfebcc0425bab25567e8514d156650a608530be1'
ALLOWED = ['corev_apu/fpga/src/ariane_peripherals_xilinx.sv',
           'corev_apu/fpga/src/ariane_xilinx.sv',
           'corev_apu/fpga/src/dma_observer.sv',
           'corev_apu/fpga/src/dma_path_observer.sv',
           'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv']
RUN_TCL = 'corev_apu/fpga/scripts/run.tcl'
GENERIC_MARKER = 'set_property top ${project}_xilinx [current_fileset]'
GENERIC_OVERLAY = '''
# Full E3 only. Source/module defaults and all constraints remain unchanged.
set_property generic {DMAPathQueueDepth=64} [current_fileset]
set e3_top_generic [get_property generic [current_fileset]]
if {$e3_top_generic ne "DMAPathQueueDepth=64"} { error "full E3 top generic is not64: $e3_top_generic" }
set e3_top_receipt [open "full-e3-top-generic.json" w]
puts $e3_top_receipt { {"DMAPathQueueDepth":64,"evidence":"Vivado current_fileset generic before synthesis"} }
close $e3_top_receipt
'''


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--out', required=True, type=Path)
    p.add_argument('--check-source', action='store_true', help='Validate/freeze source receipt only; no export or build')
    a = p.parse_args()
    output = a.out.resolve()
    if not output.is_relative_to((HERE / 'artifacts').resolve()) or output.exists():
        p.error('--out must be a new directory inside this worktree E3 artifacts/')
    preserved = HERE / 'source/build_baseline.as-reused.py'
    spec = importlib.util.spec_from_file_location('full_e3_preserved_builder', preserved)
    base = importlib.util.module_from_spec(spec);spec.loader.exec_module(base)
    base.WORK, base.PLATFORM, base.HERE = WORK, PLATFORM, HERE / 'build/full-e3'
    base.HERE.mkdir(parents=True, exist_ok=True)
    verifier = (HERE / 'verify_routed_full.tcl').read_bytes()
    target = base.HERE / 'verify_routed.tcl'
    if target.exists():
        assert target.read_bytes() == verifier, 'frozen full-E3 routed verifier differs'
    else:
        target.write_bytes(verifier)
    original_phase, original_save, original_archive = base.phase_manifest, base.save, base.archive
    helper_inputs = {}
    for name in ('scripts/platform.py', 'provenance/source-organization-20261017/export_sources.py'):
        actual_helper = PLATFORM / name
        assert actual_helper.read_bytes() == base.show(PLATFORM, PLATFORM_COMMIT, name)
        helper_inputs[name] = base.sha(actual_helper)
    baseline, _ = base.validate_source(BASE_SOURCE)
    for repo, commit in ((PLATFORM, PLATFORM_COMMIT), (PLATFORM / 'cva6', CVA6),
                         (PLATFORM / 'linux', LINUX_REFERENCE)):
        assert re.fullmatch('[0-9a-f]{40}', commit)
        assert base.git(repo, 'rev-parse', commit + '^{commit}').decode().strip() == commit
    changed = base.git(PLATFORM / 'cva6', 'diff', '--name-only', BASE_CVA6, CVA6).decode().splitlines()
    assert changed == ALLOWED, changed
    assert base.git(PLATFORM / 'cva6', 'rev-parse', CVA6 + ':corev_apu/riscv-iommu').decode().strip() == base.IOMMU
    lock = json.loads(base.show(PLATFORM, PLATFORM_COMMIT, 'sources.lock.json'))
    locks = {c['path']: c['commit'] for c in lock['components']}
    assert locks['cva6'] == CVA6 and locks['linux'] == LINUX_REFERENCE
    for path, commit in (('cva6', CVA6), ('linux', LINUX_REFERENCE)):
        assert base.git(PLATFORM, 'rev-parse', PLATFORM_COMMIT + ':' + path).decode().strip() == commit
    exporter = base.load_module(PLATFORM / 'provenance/source-organization-20261017/export_sources.py', 'full_e3_resolve')
    components = exporter.resolve({'components': [{'path': 'cva6', 'commit': CVA6},
                                                  {'path': 'linux', 'commit': LINUX_REFERENCE},
                                                  {'path': 'opensbi', 'commit': base.SBI}]})
    assert {c['path']: c['commit'] for c in components} == locks
    before = base.show(PLATFORM / 'cva6', CVA6, RUN_TCL)
    assert before.decode().count(GENERIC_MARKER) == 1
    after = before.decode().replace(GENERIC_MARKER, GENERIC_MARKER + '\n' + GENERIC_OVERLAY).encode()
    sha = lambda data: hashlib.sha256(data).hexdigest()
    selected = copy.deepcopy(baseline)
    selected.update(schema='ispass2027-full-e3-fpga-source-v1', selected_cva6=CVA6,
                    selected_platform=PLATFORM_COMMIT, linux=LINUX_REFERENCE, components=components,
                    source_ref='refs/heads/ispass/a2-workloads', baseline_source_manifest=str(BASE_SOURCE),
                    baseline_source_sha256=base.sha(BASE_SOURCE), changed_files=changed,
                    scope='Bounded hardware-fed linear/page-packed/tiled-row COPY; single COPY; DS0; no new research mechanism',
                    exclusions=['P/N/PN', 'service scheduling changes', 'overlapping COPY jobs', 'ABI5 software not yet implemented'],
                    linux_reference_only=True, abi5_software_present=False,
                    batch_version=3, batch_magic='0x49444d4142544303', observer_version=5,
                    observer_magic='0x49444d414f425305', PathQueueDepth=64, hist_info_format=2,
                    top_generic={'DMAPathQueueDepth': 64}, source_defaults={'DMAPathQueueDepth': 4},
                    export_overlay={'path': RUN_TCL, 'before_sha256': sha(before), 'after_sha256': sha(after),
                                    'reason': 'Explicit full-E3 top generic; no constraint or implementation-strategy change'},
                    execution_helper_inputs=helper_inputs,
                    finite_evidence={str(p.relative_to(HERE)): base.sha(p) for p in
                                     [HERE / 'test-results/full-frontend03-report.json',
                                      HERE / 'test-results/path64-qualification03.json']})
    for name in ('repositories', 'profiles', 'measurement_overlay_files'):
        selected.pop(name, None)
    source = base.HERE / 'full-fpga-source.json'
    if source.exists():
        assert json.loads(source.read_text()) == selected, 'frozen full-E3 source receipt differs'
    else:
        base.save(source, selected)
    if a.check_source:
        print(json.dumps({'status': 'source_checked_no_build', 'source_manifest': str(source),
                          'sha256': base.sha(source), 'platform': PLATFORM_COMMIT, 'cva6': CVA6,
                          'linux_reference_only': LINUX_REFERENCE, 'future_out': str(output)}))
        return 0

    def validate(path):
        value = json.loads(Path(path).read_text())
        assert value == selected, 'full-E3 source identity mismatch'
        return value, {c['path']: c['commit'] for c in value['components']}

    def archive(repo, commit, dest):
        original_archive(repo, commit, dest)
        if repo == PLATFORM / 'cva6':
            assert commit == CVA6
            tcl = dest / RUN_TCL
            assert tcl.read_bytes() == before
            tcl.write_bytes(after)

    def phase(out, source_path, action):
        manifest = original_phase(out, source_path, action)
        manifest.update(schema='ispass2027-full-e3-build-v1', script_sha256=base.sha(__file__),
                        reused_baseline_builder_sha256=base.sha(preserved),
                        change_scope=selected['scope'], linux_reference_only=LINUX_REFERENCE,
                        abi5_software_present=False, batch_version=3, observer_version=5,
                        PathQueueDepth=64, hist_info_format=2, top_generic={'DMAPathQueueDepth': 64},
                        finite_evidence=selected['finite_evidence'], export_overlay=selected['export_overlay'],
                        timing_policy='unchanged50MHz; WNS>=0, setup/hold/pulse failing=0, routing errors=0; terminal failure')
        base.save(out / 'build-manifest.json', manifest)
        return manifest

    def save(path, value):
        if Path(path).name == 'build-manifest.json' and 'parameters' in value:
            value['parameters'].update(observer_version=5, observer_magic=selected['observer_magic'],
                                       batch_version=3, batch_magic=selected['batch_magic'], PathQueueDepth=64,
                                       hist_info_format=2, histogram_stages=16, histogram_bins=32,
                                       histogram_counter_bits=32, histogram_scheme=1,
                                       top_generic={'DMAPathQueueDepth': 64})
            actual = output / 'src/cva6/corev_apu/fpga/full-e3-top-generic.json'
            if actual.exists():
                setting = json.loads(actual.read_text())
                assert setting['DMAPathQueueDepth'] == 64
                value['actual_vivado_top_generic'] = {'path': str(actual), 'sha256': base.sha(actual), **setting}
            if 'artifacts' in value:
                utilization = output / 'src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt'
                if utilization.exists():
                    value['artifacts'][str(utilization)] = base.sha(utilization)
                    value['hierarchical_utilization_report'] = str(utilization)
            if value.get('status') == 'passed':
                assert actual.exists(), 'successful build lacks actual pre-synthesis top-generic receipt'
        return original_save(path, value)

    base.validate_source, base.archive, base.phase_manifest, base.save = validate, archive, phase, save
    base.fpga(output, source, 'ds0')
    result = json.loads((output / 'build-manifest.json').read_text())
    return 0 if result['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
