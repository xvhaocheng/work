#!/usr/bin/env python3
"""Build the committed G CAPS driver and native nofault-load object, no kernel image."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

HERE = Path(__file__).resolve().parent
COMMIT = '0bfd2e9a196c4296487ff0f82d0894e59f7767f5'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--linux', required=True, type=Path)
    parser.add_argument('--config', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    parser.add_argument('--nofault-reference', type=Path, help='Reuse an unchanged, passed mm/maccess.o evidence directory')
    args = parser.parse_args()
    source, output = args.linux.resolve(), args.out.resolve()
    assert output.is_relative_to((HERE.parent / 'artifacts').resolve()) and not output.exists()
    assert subprocess.check_output(['git', '-C', str(source), 'rev-parse', 'HEAD'], text=True).strip() == COMMIT
    assert subprocess.check_output(['git', '-C', str(source), 'diff', '--binary', COMMIT]) == b''
    output.mkdir(parents=True)
    kernel = output / 'kernel'
    kernel.mkdir()
    shutil.copyfile(args.config, kernel / '.config')
    shutil.copyfile(args.config, output / 'input.config')
    shutil.copyfile(__file__, output / 'build_g_object.as-executed.py')
    files = ('drivers/misc/idma.c', 'drivers/misc/idma-g.h', 'include/uapi/linux/idma_g.h',
             'mm/maccess.c', 'arch/riscv/include/asm/uaccess.h', 'arch/riscv/include/asm/mmio.h',
             'arch/riscv/kernel/traps.c', 'arch/riscv/mm/extable.c')
    manifest = {'schema': 'ispass2027-e6-g-object-build-v1', 'start': time.time(), 'status': 'running',
                'linux_source': str(source), 'linux_commit': COMMIT,
                'input_config': str(args.config.resolve()), 'input_config_sha256': sha(args.config),
                'inputs': {name: sha(source / name) for name in files},
                'scope': 'RV64 idma.o and mm/maccess.o only; no Image/rootfs/payload or hardware', 'commands': []}
    path = output / 'manifest.json'

    def save():
        path.write_text(json.dumps(manifest, indent=2) + '\n')

    common = ['make', '-C', str(source), 'ARCH=riscv', 'O=' + str(kernel), 'CROSS_COMPILE=riscv64-linux-gnu-']
    commands = [(common + ['olddefconfig'], 'config', 120),
                (common + ['-j4', 'drivers/misc/idma.o', 'mm/maccess.o'], 'compile', 300),
                (['riscv64-linux-gnu-objdump', '-dr', str(kernel / 'drivers/misc/idma.o')], 'idma-disassembly', 30),
                (['riscv64-linux-gnu-objdump', '-dr', str(kernel / 'mm/maccess.o')], 'maccess-disassembly', 30),
                (['riscv64-linux-gnu-readelf', '-SWr', str(kernel / 'mm/maccess.o')], 'maccess-sections-relocations', 30)]
    objects = ('drivers/misc/idma.o', 'mm/maccess.o')
    if args.nofault_reference:
        reference = args.nofault_reference.resolve()
        receipt = json.loads((reference / 'manifest.json').read_text())
        assert receipt['status'] == 'passed'
        for name in files[3:]:
            assert receipt['inputs'][name] == sha(source / name), 'nofault source changed: ' + name
        assert sha(reference / 'kernel/mm/maccess.o') == receipt['object_sha256']['mm/maccess.o']
        manifest['unchanged_nofault_reference'] = {'manifest': str(reference / 'manifest.json'),
                'manifest_sha256': sha(reference / 'manifest.json'),
                'object_sha256': receipt['object_sha256']['mm/maccess.o']}
        commands[1] = (common + ['-j4', 'drivers/misc/idma.o'], 'compile', 300)
        commands = commands[:3]
        objects = ('drivers/misc/idma.o',)
    try:
        for argv, label, timeout in commands:
            record = {'argv': argv, 'start': time.time(), 'log': label + '.log', 'timeout_s': timeout}
            manifest['commands'].append(record)
            save()
            with (output / record['log']).open('w') as log:
                result = subprocess.run(argv, cwd=output, stdout=log, stderr=subprocess.STDOUT, timeout=timeout)
            record.update(end=time.time(), returncode=result.returncode)
            save()
            if result.returncode:
                raise RuntimeError(label + ' failed')
        manifest.update(status='passed', object_sha256={name: sha(kernel / name) for name in objects},
                        effective_config_sha256=sha(kernel / '.config'))
    except Exception as error:
        manifest.update(status='failed', error=repr(error))
        raise
    finally:
        manifest['end'] = time.time()
        save()


if __name__ == '__main__':
    main()
