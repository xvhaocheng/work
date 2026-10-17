#!/usr/bin/env python3
"""Build the approved P0 DS0 frontend change with the existing FPGA procedure.

Uses a preserved baseline builder as an execution library with a P0-specific
source validator/manifest. No baseline pins or production checkout are edited.
No board control is present. Timing failure is terminal; no constraint changes.
"""
import argparse
import copy
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess

HERE = Path(__file__).resolve().parent
PLATFORM = Path('/home/xvhaocheng/work/iommu-platform')
BASE_CVA6 = '8aac3fcd345af7686a55af1867783fd6b91a0069'
P0_CVA6 = '8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8'
FRONTEND = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'
BASE_SOURCE = PLATFORM / 'out/ispass2027-20261017-source01/source-manifest.json'


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--out', required=True, type=Path)
    p.add_argument('--platform-commit', required=True)
    a = p.parse_args()
    preserved = HERE / 'source/build_baseline.as-reused.py'
    spec = importlib.util.spec_from_file_location('p0_reused_baseline_builder', preserved)
    base = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(base)
    base.WORK = PLATFORM.parent
    base.PLATFORM = PLATFORM
    base.HERE = HERE / 'build'
    base.HERE.mkdir(exist_ok=True)
    # This is the existing routed-boundary check, not a new timing policy.
    target = base.HERE / 'verify_routed.tcl'
    if not target.exists():
        shutil.copyfile(PLATFORM.parent / 'paper/ispass2027/board/verify_routed.tcl', target)
    original_phase = base.phase_manifest
    baseline, _ = base.validate_source(BASE_SOURCE)
    changed = base.git(PLATFORM / 'cva6', 'diff', '--name-only', BASE_CVA6, P0_CVA6).decode().splitlines()
    assert changed == [FRONTEND], changed
    assert base.git(PLATFORM / 'cva6', 'rev-parse', P0_CVA6 + '^').decode().strip() == BASE_CVA6
    assert base.git(PLATFORM / 'cva6', 'rev-parse', P0_CVA6 + ':corev_apu/riscv-iommu').decode().strip() == base.IOMMU
    lock = json.loads(base.show(PLATFORM, a.platform_commit, 'sources.lock.json'))
    locks = {x['path']: x['commit'] for x in lock['components']}
    assert locks['cva6'] == P0_CVA6 and locks['linux'] == base.LINUX
    assert base.git(PLATFORM, 'rev-parse', a.platform_commit + ':cva6').decode().strip() == P0_CVA6
    selected = copy.deepcopy(baseline)
    selected.update(schema='ispass2027-p0-fpga-source-v1', selected_cva6=P0_CVA6,
                    selected_platform=a.platform_commit, source_ref='refs/heads/ispass/a2-workloads',
                    baseline_source_manifest=str(BASE_SOURCE), baseline_source_sha256=base.sha(BASE_SOURCE),
                    changed_files=changed,
                    scope='P0 only; DS0, V64/1024/4096 and A4096 packed ring; frontend BTC2',
                    exclusions=['16/64KiB descriptors', '2D', 'P/N/PN', 'service scheduling changes'])
    selected.pop('repositories', None)
    selected.pop('profiles', None)
    selected.pop('measurement_overlay_files', None)
    for c in selected['components']:
        if c['path'] == 'cva6':
            c.update(commit=P0_CVA6, tree=base.git(PLATFORM / 'cva6', 'rev-parse', P0_CVA6 + '^{tree}').decode().strip())
    source = base.HERE / 'p0-fpga-source.json'
    if source.exists():
        assert json.loads(source.read_text()) == selected, 'P0 source manifest already differs'
    else:
        base.save(source, selected)

    def p0_validate(path):
        s = json.loads(Path(path).read_text())
        assert s == selected, 'P0 source identity mismatch'
        return s, {x['path']: x['commit'] for x in s['components']}

    def p0_phase(out, manifest_source, action):
        m = original_phase(out, manifest_source, action)
        m.update(schema='ispass2027-p0-build-v1', script_sha256=base.sha(__file__),
                 reused_baseline_builder_sha256=base.sha(preserved),
                 change_scope='frontend only: BTC2 magic, 64/1024/4096 lengths, batch offset+=length',
                 baseline_cva6=BASE_CVA6, approved_frontend_checks='79 finite scenarios',
                 timing_policy='unchanged 50MHz constraints; WNS>=0, setup/hold/pulse failing=0, routing errors=0')
        base.save(out / 'build-manifest.json', m)
        return m

    base.validate_source = p0_validate
    base.phase_manifest = p0_phase
    base.fpga(a.out.resolve(), source, 'ds0')
    result = json.loads((a.out / 'build-manifest.json').read_text())
    return 0 if result['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
