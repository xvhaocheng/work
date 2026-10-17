#!/usr/bin/env python3
"""Build the accepted E5 DS0 passive histogram using the preserved FPGA flow.

No FPGA loading, UART, JTAG or other board operation is present. Failure is
terminal. The generic builder is unchanged; this wrapper binds E5 identities
and corrects its baseline-specific parameter metadata to the actual v4 RTL.
"""
import argparse
import copy
import importlib.util
import json
from pathlib import Path
import re

HERE = Path(__file__).resolve().parent
PLATFORM = Path('/home/xvhaocheng/work/iommu-platform')
WORK = HERE.parents[3]
BASE_SOURCE = PLATFORM / 'out/ispass2027-20261017-source01/source-manifest.json'
BASE_CVA6 = '8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8'
CVA6 = '6c5c666d45979d3a03fb98cff89ba7ad095f6125'
LINUX = 'acece3a3ab7d72ba1f936c1984b82a4a55f28d46'
PLATFORM_COMMIT = '2f19f6c2de5530760f521bf771b6217066bc54d5'
FRONTEND = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'
ALLOWED = ['corev_apu/fpga/src/ariane_peripherals_xilinx.sv',
           'corev_apu/fpga/src/dma_observer.sv', 'corev_apu/fpga/src/dma_path_observer.sv']


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--out', required=True, type=Path)
    a = p.parse_args()
    preserved = HERE / 'source/build_baseline.as-reused.py'
    spec = importlib.util.spec_from_file_location('e5_reused_baseline_builder', preserved)
    base = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(base)
    base.WORK, base.PLATFORM, base.HERE = WORK, PLATFORM, HERE / 'build'
    original_phase, original_save = base.phase_manifest, base.save
    baseline, _ = base.validate_source(BASE_SOURCE)
    # All chosen refs are full immutable object IDs; never accept a short ref.
    for repo, commit in [(PLATFORM, PLATFORM_COMMIT), (PLATFORM / 'cva6', CVA6),
                         (PLATFORM / 'linux', LINUX)]:
        assert re.fullmatch('[0-9a-f]{40}', commit)
        assert base.git(repo, 'rev-parse', commit + '^{commit}').decode().strip() == commit
    changed = base.git(PLATFORM / 'cva6', 'diff', '--name-only', BASE_CVA6, CVA6).decode().splitlines()
    assert changed == ALLOWED, changed
    assert base.show(PLATFORM / 'cva6', BASE_CVA6, FRONTEND) == base.show(PLATFORM / 'cva6', CVA6, FRONTEND)
    assert base.git(PLATFORM / 'cva6', 'rev-parse', CVA6 + ':corev_apu/riscv-iommu').decode().strip() == base.IOMMU
    lock = json.loads(base.show(PLATFORM, PLATFORM_COMMIT, 'sources.lock.json'))
    locks = {x['path']: x['commit'] for x in lock['components']}
    assert locks['cva6'] == CVA6 and locks['linux'] == LINUX
    assert base.git(PLATFORM, 'rev-parse', PLATFORM_COMMIT + ':cva6').decode().strip() == CVA6
    assert base.git(PLATFORM, 'rev-parse', PLATFORM_COMMIT + ':linux').decode().strip() == LINUX
    exporter = base.load_module(PLATFORM / 'provenance/source-organization-20261017/export_sources.py', 'e5_resolve')
    components = exporter.resolve({'components': [{'path': 'cva6', 'commit': CVA6},
                                                 {'path': 'linux', 'commit': LINUX},
                                                 {'path': 'opensbi', 'commit': base.SBI}]})
    assert {x['path']: x['commit'] for x in components} == locks
    selected = copy.deepcopy(baseline)
    selected.update(schema='ispass2027-e5-fpga-source-v1', selected_cva6=CVA6,
                    selected_platform=PLATFORM_COMMIT, linux=LINUX, components=components,
                    source_ref='refs/heads/ispass/e5-latency',
                    baseline_source_manifest=str(BASE_SOURCE), baseline_source_sha256=base.sha(BASE_SOURCE),
                    e5_base_cva6=BASE_CVA6, changed_files=changed,
                    scope='Accepted P0/multi plus passive E5 histogram; DS0; PathDepth4; no full-E3 extension',
                    linux_reference_only=True, observer_version=4, PathDepth=4,
                    histogram={'scheme': 1, 'stages': 16, 'bins': 32, 'counter_bits': 32,
                               'sample_population': 'full-window PATH samples only',
                               'overflow': 'saturating bin plus sticky stage mask and PATH error'})
    for field in ('repositories', 'profiles', 'measurement_overlay_files'):
        selected.pop(field, None)
    source = base.HERE / 'e5-fpga-source.json'
    assert not source.exists(), 'E5 source receipt exists; do not launch duplicate builds'
    base.save(source, selected)

    def e5_validate(path):
        s = json.loads(Path(path).read_text())
        assert s == selected, 'E5 source identity mismatch'
        return s, {x['path']: x['commit'] for x in s['components']}

    def e5_phase(out, manifest_source, action):
        m = original_phase(out, manifest_source, action)
        m.update(schema='ispass2027-e5-build-v1', script_sha256=base.sha(__file__),
                 reused_baseline_builder_sha256=base.sha(preserved),
                 change_scope='passive histogram only on accepted P0/multi RTL',
                 e5_base_cva6=BASE_CVA6, observer_version=4, PathDepth=4,
                 histogram=selected['histogram'], p0_frontend_unchanged=True,
                 finite_rtl_manifest=str(HERE / 'rtl-validation/runs/run03/manifest.json'),
                 finite_rtl_manifest_sha256=base.sha(HERE / 'rtl-validation/runs/run03/manifest.json'),
                 timing_policy='unchanged 50MHz constraints; WNS>=0, setup/hold/pulse failing=0, routing errors=0')
        base.save(out / 'build-manifest.json', m)
        return m

    def e5_save(path, value):
        # The reused generic flow labels all variants v3; replace only that
        # experiment-specific metadata before any execution or source receipt.
        if Path(path).name == 'build-manifest.json' and 'parameters' in value:
            value['parameters'].update(observer_version=4, PathDepth=4,
                                       histogram_stages=16, histogram_bins=32, histogram_counter_bits=32,
                                       histogram_scheme=1)
        return original_save(path, value)

    base.validate_source, base.phase_manifest, base.save = e5_validate, e5_phase, e5_save
    base.fpga(a.out.resolve(), source, 'ds0')
    result = json.loads((a.out / 'build-manifest.json').read_text())
    return 0 if result['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
