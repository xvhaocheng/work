#!/usr/bin/env python3
"""Finite production BTC3 frontend + frozen BTC1/BTC2 cycle comparisons.

Frontend MMIO/length/addresses/handshakes only; synthetic completions. No AXI,
data memory, IOMMU, observer queue, timing, or FPGA measurement qualification.
"""
import argparse
from collections import Counter
import datetime
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

HERE = Path(__file__).resolve().parent
FRONTEND = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'
BASES = {'gold': '8aeb9ac9b3aea82a975cc789cb47633fb29bf5c8',
         'gold1': '8aac3fcd345af7686a55af1867783fd6b91a0069'}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def audit_events(path, cases):
    """Recompute every accepted address from closed-form geometry and raw DUT outputs."""
    by_name = {c['name']: c for c in cases}
    assert len(by_name) == len(cases), 'duplicate scenario names'
    counts, offsets = Counter(), {name: set() for name in by_name}
    for line in path.open():
        e = json.loads(line)
        c = by_name[e['case']]
        i, n = counts[e['case']], c['length']
        assert e['index'] == i
        if not c['batch']:
            off = 0
        elif c['geometry'] == 0:
            off = i * n % (c['pages'] * 4096)
        elif c['geometry'] == 1:
            slots = 4096 // n
            off = ((i // slots) % c['pages']) * 4096 + (i % slots) * n
            assert off % 4096 + n <= 4096
        else:
            k = i % (c['matrix'] * c['columns'])
            op, row = divmod(k, c['rows'])
            group, column = divmod(op, c['columns'])
            off = (group * c['rows'] + row) * c['pitch'] + column * n
        # One directed test uses the highest nonoverflowing page for both bases.
        src = dst = 2**64 - 8192 if e['case'] == 'highest_nonoverflow_page' else None
        if src is None:
            src, dst = 0x10000, 0x100000
        assert (e['src'], e['dst'], e['length']) == (src + off, dst + off, n)
        if c['batch']:
            assert 0 <= off and off + n <= c['pages'] * 4096
        counts[e['case']] += 1
        offsets[e['case']].add(off)
    for name, c in by_name.items():
        assert counts[name] == c['accepted'] == c['completed']
        assert len(offsets[name]) == c['distinct_offsets']
    return {'status': 'passed', 'scope': 'all raw accepted frontend COPY addresses',
            'cases': len(cases), 'accepted_descriptors': sum(counts.values()),
            'events_sha256': sha(path), 'counts': dict(counts)}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cva6', required=True, type=Path)
    p.add_argument('--deps', type=Path, default=HERE / 'artifacts/p0-ds0-build01/src/cva6')
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args()
    a.cva6, a.deps, a.output = a.cva6.resolve(), a.deps.resolve(), a.output.resolve()
    a.output.mkdir(parents=True, exist_ok=False)
    snapshots = a.output / 'source'
    snapshots.mkdir()
    actual = snapshots / 'idma_reg64_frontend.sv'
    shutil.copyfile(a.cva6 / FRONTEND, actual)
    golds = []
    for label, commit in BASES.items():
        source = subprocess.check_output(['git', '-C', str(a.cva6), 'show', commit + ':' + FRONTEND], text=True)
        out = snapshots / (label + '.sv')
        out.write_text(source.replace('module idma_reg64_frontend #(', f'module idma_reg64_frontend_{label} #(')
                       .replace('endmodule : idma_reg64_frontend', f'endmodule : idma_reg64_frontend_{label}'))
        golds.append(out)
    top, cpp = snapshots / 'full_frontend_top.sv', snapshots / 'full_frontend_test.cpp'
    shutil.copyfile(HERE / 'source/full_frontend_top.sv', top)
    shutil.copyfile(HERE / 'source/full_frontend_test.cpp', cpp)
    shutil.copyfile(Path(__file__), snapshots / Path(__file__).name)
    prim = a.deps / 'corev_apu/register_interface/vendor/lowrisc_opentitan/src'
    files = [a.deps / 'vendor/pulp-platform/common_cells/include/common_cells/registers.svh',
             a.deps / 'vendor/pulp-platform/axi/src/axi_pkg.sv', a.deps / 'corev_apu/iDMA/src/idma_pkg.sv',
             a.deps / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
             prim / 'prim_subreg_arb.sv', prim / 'prim_subreg.sv', prim / 'prim_subreg_ext.sv',
             a.deps / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
             a.deps / 'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv', actual, *golds, top, cpp]
    cmd = ['verilator', '--cc', '--exe', '--build', '-Wno-fatal', '--top-module', 'full_frontend_top',
           '--Mdir', str(a.output / 'obj'), '-I' + str(a.deps / 'vendor/pulp-platform/common_cells/include'),
           '-I' + str(a.deps / 'corev_apu/iDMA/src/include'), *map(str, files)]
    report = {'status': 'running', 'fixture_only': True, 'evidence_level': 'finite_frontend_rtl',
              'scope': __doc__, 'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'cva6_worktree': str(a.cva6), 'cva6_parent': subprocess.check_output(
                  ['git', '-C', str(a.cva6), 'rev-parse', 'HEAD'], text=True).strip(),
              'golden_commits': BASES, 'inputs': {str(x): sha(x) for x in files}, 'commands': []}
    # This macro header is textually included by the wrapper/frontend, rather
    # than passed as a separate compilation unit. Keep its identity as well.
    included = a.deps / 'corev_apu/iDMA/src/include/idma/typedef.svh'
    report['included_headers'] = {str(included): sha(included)}
    (a.output / 'frontend.patch').write_bytes(subprocess.check_output(
        ['git', '-C', str(a.cva6), 'diff', '--', FRONTEND]))
    def save():
        (a.output / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    save()
    try:
        with (a.output / 'build.log').open('w') as f:
            r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, timeout=240)
        report['commands'].append({'argv': cmd, 'exit_code': r.returncode})
        if r.returncode:
            raise RuntimeError('Verilator compile failed; see build.log')
        cmd = [str(a.output / 'obj/Vfull_frontend_top'), str(a.output / 'events.jsonl')]
        r = subprocess.run(cmd, text=True, capture_output=True, timeout=120)
        report['commands'].append({'argv': cmd, 'exit_code': r.returncode})
        (a.output / 'test.log').write_text(r.stdout + r.stderr)
        cases = [json.loads(line[5:]) for line in r.stdout.splitlines() if line.startswith('CASE ')]
        report['cases'] = cases
        if r.returncode:
            raise RuntimeError('finite frontend test failed; see test.log')
        audit = audit_events(a.output / 'events.jsonl', cases)
        (a.output / 'event-audit.json').write_text(json.dumps(audit, indent=2) + '\n')
        report.update(status='passed', case_count=len(cases), accepted_descriptors=audit['accepted_descriptors'],
                      event_audit={'path': str(a.output / 'event-audit.json'), 'sha256': sha(a.output / 'event-audit.json')},
                      events={'path': str(a.output / 'events.jsonl'), 'sha256': sha(a.output / 'events.jsonl')})
    except Exception as exc:
        report.update(status='failed', error=str(exc))
    report['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
    save()
    print(json.dumps({'status': report['status'], 'report': str(a.output / 'report.json'),
                      'case_count': report.get('case_count'), 'error': report.get('error')}))
    return 0 if report['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
