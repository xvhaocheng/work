#!/usr/bin/env python3
"""Compile the committed initial ABI5 geometry/OBS5 driver in a separate kernel O directory only."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

HERE = Path(__file__).resolve().parent
COMMIT = '682a2caf0a7fdf69749ebdac146df60df55dcbe6'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--linux', type=Path, required=True)
    p.add_argument('--config', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    out, source = a.out.resolve(), a.linux.resolve()
    if not out.is_relative_to((HERE / 'artifacts').resolve()) or out.exists():
        p.error('--out must be a new directory under this E3 worktree artifacts/')
    actual = subprocess.check_output(['git', '-C', str(source), 'rev-parse', 'HEAD'], text=True).strip()
    assert actual == COMMIT
    assert subprocess.check_output(['git', '-C', str(source), 'diff', '--binary', COMMIT]) == b''
    out.mkdir(parents=True)
    kernel = out / 'kernel'
    kernel.mkdir()
    shutil.copyfile(a.config, kernel / '.config')
    shutil.copyfile(a.config, out / 'input.config')
    shutil.copyfile(__file__, out / 'build_driver_object.as-executed.py')
    manifest = {'schema': 'ispass2027-e3-initial-driver-object-build-v1', 'status': 'running',
                'start': time.time(), 'linux_commit': COMMIT, 'linux_source': str(source),
                'driver_sha256': sha(source / 'drivers/misc/idma.c'),
                'critical_source_hashes': {n: sha(source/n) for n in ['drivers/misc/idma.c', 'drivers/misc/idma-workload.h', 'include/uapi/linux/idma_workload.h']},
                'input_config': str(a.config.resolve()), 'input_config_sha256': sha(a.config),
                'scope': 'Cross-compile drivers/misc/idma.o only; no Image/rootfs/payload or hardware',
                'commands': []}
    path = out / 'manifest.json'

    def save():
        path.write_text(json.dumps(manifest, indent=2) + '\n')

    common = ['make', '-C', str(source), 'ARCH=riscv', 'O=' + str(kernel), 'CROSS_COMPILE=riscv64-linux-gnu-']
    try:
        for argv, name, timeout in [(common + ['olddefconfig'], 'config', 120),
                                    (common + ['-j4', 'drivers/misc/idma.o'], 'compile', 300)]:
            record = {'argv': argv, 'start': time.time(), 'log': name + '.log', 'timeout_s': timeout}
            manifest['commands'].append(record)
            save()
            with (out / record['log']).open('w') as log:
                result = subprocess.run(argv, cwd=out, stdout=log, stderr=subprocess.STDOUT, timeout=timeout)
            record.update(end=time.time(), returncode=result.returncode)
            save()
            if result.returncode: raise RuntimeError(name + ' failed')
        manifest.update(status='passed', end=time.time(), object_sha256=sha(kernel / 'drivers/misc/idma.o'),
                        object_bytes=(kernel / 'drivers/misc/idma.o').stat().st_size,
                        effective_config_sha256=sha(kernel / '.config'))
    except Exception as error:
        manifest.update(status='failed', end=time.time(), error=str(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
