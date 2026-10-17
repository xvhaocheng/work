#!/usr/bin/env python3
"""Native sanitizers and RV64 layout for exact ABI5 geometry helper functions.

Kernel bitmap primitives are replaced by host bit operations; production
geometry/offset/prefix functions are compiled unchanged. No DMA/board evidence.
"""
import argparse
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess

HERE = Path(__file__).resolve().parent


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--linux', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    out, linux = a.output.resolve(), a.linux.resolve()
    out.mkdir(parents=True, exist_ok=False)
    src = out / 'source'
    (src / 'uapi/linux').mkdir(parents=True)
    for name in ('idma.h', 'idma_workload.h'):
        shutil.copyfile(linux / 'include/uapi/linux' / name, src / 'uapi/linux' / name)
    shutil.copyfile(linux / 'drivers/misc/idma-workload.h', src / 'idma-workload.h')
    shutil.copyfile(HERE / 'source/workload_helpers_test.c', src / 'workload_helpers_test.c')
    shutil.copyfile(HERE / 'model_full_coverage.py', src / 'model_full_coverage.py')
    shutil.copyfile(Path(__file__), src / Path(__file__).name)
    layout = (HERE / 'source/check_workload_draft_layout.c').read_text().replace(
        '#include "proposed_idma_workload.h"', '#include <linux/idma_workload.h>')
    (src / 'layout.c').write_text(layout)
    spec = importlib.util.spec_from_file_location('workload_model', src / 'model_full_coverage.py')
    model = importlib.util.module_from_spec(spec);spec.loader.exec_module(model)
    planned = json.loads((HERE / 'full-planned-coverage.json').read_text())
    cases = []
    for row in planned['rows']:
        cases.append({'kind': 'planned_configuration', 'g': row['geometry'],
                      'n': row['planned_V_measurement']['completed_descriptors'],
                      'w': row['planned_V_warmup']['completed_descriptors'], 'valid': True})
    for g in [model.geometry(s, n) for s in ('p1_minimal', 'nic', 'storage', 'tile') for n in model.SIZES] + [model.geometry('nic', 1536)]:
        ring, rows = model.period(g), g['rows_per_operation']
        for n in sorted({0, 1, rows-1, rows, rows+1, ring-1, ring, ring+1, 2**32, 2**64-1}):
            cases.append({'kind': 'actual_A_prefix_boundary', 'g': g, 'n': n, 'w': max(0, rows-1), 'valid': True})
    # Individual enum, width, reserved, and cross-field constraints.
    keys = {'row_bytes': [0, 1, 63, 65, 128, 1500, 2048, 8192, 65537, 2**32-1],
            'mapped_pages': [0, 3, 6, 257, 4096, 2**32-1],
            'rows_per_operation': [0, 2, 3, 65, 2**32-1],
            'pitch_bytes': [1, 8192, 2**32-1], 'matrix_rows': [1, 32, 65, 2**32-1],
            'columns': [1, 15, 17, 2**32-1]}
    for shape, size in [('storage', 1024), ('nic', 1536), ('tile', 65536)]:
        for key, values in keys.items():
            for value in values:
                g = copy.deepcopy(model.geometry(shape, size));g[key] = value
                cases.append({'kind': 'invalid_geometry', 'g': g, 'n': 1, 'w': 0, 'valid': False})
        for key in ('reserved0', 'reserved'):
            cases.append({'kind': 'reserved_rejection', 'g': model.geometry(shape, size),
                          'n': 1, 'w': 0, 'valid': False, key: 1})
    cases += [{'kind': 'span_too_short', 'g': model.geometry('p1_minimal', 65536) | {'mapped_pages': 8}, 'n': 1, 'w': 0, 'valid': False}]
    lines = []
    kinds = {'linear': 0, 'page_packed': 1, 'tile_rows': 2}
    for i, case in enumerate(cases):
        g = case['g']
        fields = [i, kinds[g['geometry']], g['row_bytes'], g['rows_per_operation'], g['pitch_bytes'],
                  g['mapped_pages'], g['matrix_rows'], g['columns'], case.get('reserved0', 0),
                  case.get('reserved', 0), case['n'], case['w']]
        lines.append(' '.join(map(str, fields)) + '\n')
    (out / 'cases.txt').write_text(''.join(lines))
    manifest = {'status': 'running', 'fixture_only': True, 'evidence_level': 'finite_native_functions', 'scope': __doc__,
                'source': {str(x.relative_to(out)): sha(x) for x in src.rglob('*') if x.is_file()},
                'cases': len(cases), 'commands': []}
    path = out / 'manifest.json'
    def save():
        path.write_text(json.dumps(manifest, indent=2) + '\n')
    def run(argv, label, input=None):
        r = subprocess.run(list(map(str, argv)), input=input, text=True, capture_output=True, timeout=120)
        (out / (label + '.log')).write_text(r.stdout + r.stderr)
        manifest['commands'].append({'argv': list(map(str, argv)), 'exit_code': r.returncode, 'log': label + '.log'});save()
        r.check_returncode()
        return r.stdout
    save()
    try:
        common = ['-Wall', '-Wextra', '-Werror', '-I', src / 'uapi']
        run(['gcc', '-std=gnu11', '-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
             '-fno-pie', '-no-pie', *common, '-I', src, src / 'workload_helpers_test.c', '-o', out / 'helpers'], 'native-build')
        raw = run([out / 'helpers'], 'native-run', ''.join(lines))
        results = [json.loads(line) for line in raw.splitlines()]
        assert len(results) == len(cases)
        def blocks(g, n):
            touched = set()
            for k in range(min(n, model.period(g))):
                off = model.offset(g, k)
                touched.update(range(off // 64, (off + g['row_bytes']) // 64))
            return touched
        def decode(text):
            result = set()
            for word in range(256):
                v = int(text[word*16:(word+1)*16], 16)
                result.update(word*64+bit for bit in range(64) if v & (1 << bit))
            return result
        for i, (case, result) in enumerate(zip(cases, results)):
            assert result['index'] == i
            assert (result['validation'] == 0) == case['valid'], (i, case, result)
            if not case['valid']:
                continue
            g = case['g'];mb, wb = blocks(g, case['n']), blocks(g, case['w'])
            assert decode(result.pop('measured_bitmap')) == mb
            assert decode(result.pop('warmup_bitmap')) == wb
            assert result['ring'] == model.period(g)
            assert result['measured_bytes'] == len(mb)*64 and result['warmup_bytes'] == len(wb)*64
            assert result['union_bytes'] == len(mb | wb)*64
            assert result['measured_pages'] == len({b//64 for b in mb})
            assert result['warmup_pages'] == len({b//64 for b in wb})
            assert all(off == model.offset(g, k) for k, off in result['offset_checks'])
        run(['gcc', '-std=gnu11', *common, src / 'layout.c', '-o', out / 'layout-native'], 'layout-native-build')
        run([out / 'layout-native'], 'layout-native-run')
        run(['riscv64-linux-gnu-gcc', '-std=gnu11', *common, '-c', src / 'layout.c', '-o', out / 'layout-rv64.o'], 'layout-rv64-build')
        manifest.update(status='passed', valid_cases=sum(c['valid'] for c in cases),
                        invalid_cases=sum(not c['valid'] for c in cases),
                        raw_output_sha256=sha(out / 'native-run.log'), checked_results=results,
                        formal_caps_bytes=64, formal_run_bytes=544,
                        caps_ioctl='0x8040690f', run_ioctl='0xc2206910')
    except Exception as exc:
        manifest.update(status='failed', error=str(exc))
    save()
    print(json.dumps({'status': manifest['status'], 'manifest': str(path), 'cases': len(cases), 'error': manifest.get('error')}))
    return 0 if manifest['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
