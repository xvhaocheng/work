#!/usr/bin/env python3
"""Finite source/rootfs/refusal checks; never execute Image or board commands."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cli-run', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args()
    out = a.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    spec = importlib.util.spec_from_file_location('g_sw', HERE.parent/'build_g_software.py')
    sw = importlib.util.module_from_spec(spec); spec.loader.exec_module(sw)
    base = sw.library()
    common = sw.frozen(base)
    passed = []
    passed.append('actual platform/linux/cva6/opensbi gitlinks and lock match frozen pins')
    passed.append('archiver/rootfs helper bytes match frozen platform')
    passed.append('actual common manifest/rootfs/config/DTS match passed full-E3 receipt')
    (out/'bin').mkdir()
    shutil.copyfile(a.cli_run/'idma_g', out/'bin/idma_g')
    report = {}
    sw.rootfs(base, out, report)
    module = base.load_module(sw.HERE/'source/rootfs.as-reused.py', 'checked_rootfs')
    original = list(module.read_newc((sw.BASE_SW/'rootfs.cpio').read_bytes()))
    packed = list(module.read_newc((out/'rootfs.cpio').read_bytes()))
    assert len(packed) == len(original)+1
    assert (out/'rootfs.cpio').stat().st_size % 512 == 0
    for old, new in zip(original, packed):
        name, fields, body = old
        assert name == new[0] and body == new[2]
        expected = list(fields); expected[5] = 0
        assert expected == new[1]
    assert packed[-1][0] == 'usr/bin/idma_g' and packed[-1][2] == (a.cli_run/'idma_g').read_bytes()
    assert packed[-1][1][1] == 0o100755
    passed.append('all original rootfs names/body/metadata preserved (mtime deterministically zero); one executable added')
    copied = dict((n, b) for n, _, b in packed)
    for name in ['idma_experiment', 'idma_p0', 'idma_scaling', 'idma_pages', 'idma_workload',
                 'platform-timeout', 'platform-sd-update']:
        assert hashlib.sha256(copied['usr/bin/'+name]).hexdigest() == common['artifacts']['bin/'+name]
    passed.append('all seven common static binaries exactly equal original passed artifacts')
    real_base = sw.BASE_SW
    try:
        fake = out/'fake-common'; fake.mkdir()
        sw.BASE_SW = fake
        for kind in ['existing-g', 'duplicate-path']:
            data = bytearray()
            entries = [('usr/bin/idma_g', [1, 0o100755, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0], b'x')]
            if kind == 'duplicate-path':
                entries = [('etc/x', entries[0][1], b'x'), ('./etc/x', entries[0][1], b'y')]
            for entry in entries: module.write_entry(data, *entry)
            module.write_entry(data, 'TRAILER!!!', [0]*13, b'')
            (fake/'rootfs.cpio').write_bytes(data)
            try: sw.rootfs(base, out, {})
            except AssertionError: passed.append('reject '+kind)
            else: raise AssertionError('accepted '+kind)
    finally:
        sw.BASE_SW = real_base
    # Wrong output/jobs are rejected before output creation or any tool build.
    commands = []
    forbidden = out/'must-not-exist'
    for args in [['prepare', '--out', str(forbidden), '--jobs', '0'],
                 ['prepare', '--out', str(out)]]:
        r = subprocess.run([sys.executable, sw.__file__, *args], text=True, capture_output=True, timeout=30)
        assert r.returncode != 0 and not forbidden.exists()
        commands.append({'argv': args, 'exit_code': r.returncode, 'stdout': r.stdout, 'stderr': r.stderr})
    passed.append('invalid jobs/out refused; no build command or output')
    # Actual finalize refusal path must stop before any command/source archive.
    guard = sw.HERE/'artifacts/g-software-entry-refusal01'
    guard.mkdir(parents=True, exist_ok=False)
    (guard/'build-manifest.json').write_text('{"status":"failed"}\n')
    r = subprocess.run([sys.executable, sw.__file__, 'finalize', '--out', str(guard)],
                       text=True, capture_output=True, timeout=30)
    assert r.returncode != 0 and not (guard/'src').exists() and not (guard/'linux-build').exists()
    commands.append({'argv': ['finalize', '--out', str(guard)], 'exit_code': r.returncode,
                     'stdout': r.stdout, 'stderr': r.stderr})
    passed.append('failed package cannot finalize/retry; no source export/Image command')
    report.update(status='passed', scope='source pins, real-rootfs repack and refusal paths only; no Image/board',
                  checks=passed, check_count=len(passed), commands=commands,
                  source_sha256=base.sha(sw.__file__), runner_sha256=base.sha(__file__))
    (out/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'status': 'passed', 'checks': len(passed), 'report': str(out/'report.json')}))


if __name__ == '__main__':
    main()
