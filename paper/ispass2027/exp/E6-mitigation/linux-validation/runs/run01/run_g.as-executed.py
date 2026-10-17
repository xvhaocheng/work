#!/usr/bin/env python3
"""Compile/run actual G helper and extracted CAPS functions in a bounded host fixture."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

HERE = Path(__file__).resolve().parent
COMMIT = 'b2586bfc08cbe2c9ae300308c4ba34c95f3c739c'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def extract(text, name):
    start = text.index('static ', text.rfind('\n}', 0, text.index(name)))
    # Locate the declaration directly, without matching a preceding call.
    start = text.rfind('\nstatic ', 0, text.index(name)) + 1
    opening = text.index('{', text.index(name))
    depth = 1
    end = opening + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[start:end] + '\n'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--linux', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    args = parser.parse_args()
    source, out = args.linux.resolve(), args.out.resolve()
    assert out.is_relative_to((HERE / 'runs').resolve()) and not out.exists()
    assert subprocess.check_output(['git', '-C', str(source), 'rev-parse', 'HEAD'], text=True).strip() == COMMIT
    assert subprocess.check_output(['git', '-C', str(source), 'diff', '--binary', COMMIT]) == b''
    out.mkdir(parents=True)
    for name in ('drivers/misc/idma.c', 'drivers/misc/idma-g.h', 'include/uapi/linux/idma.h', 'include/uapi/linux/idma_g.h'):
        target = out / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source / name, target)
    text = (out / 'drivers/misc/idma.c').read_text()
    assert 'case IDMA_IOC_RUN_G:' not in text
    (out / 'actual-g-functions.c').write_text('\n'.join(extract(text, name) for name in
                                            ('idma_g_read_gap_cap(', 'idma_caps_g(')))
    shutil.copyfile(HERE / 'test_g.c', out / 'test_g.c')
    shutil.copyfile(__file__, out / 'run_g.as-executed.py')
    shim = out / 'shim/linux'
    shim.mkdir(parents=True)
    (shim / 'types.h').write_text('#include_next <linux/types.h>\n#include <stdbool.h>\ntypedef __u64 u64; typedef __u32 u32;\n')
    (shim / 'errno.h').write_text('#include <asm-generic/errno.h>\n')
    (shim / 'string.h').write_text('#include <string.h>\nstatic inline void *memchr_inv(const void *p, int c, size_t n) { const unsigned char *s=p; while(n--) {if(*s!=(unsigned char)c) return (void *)s; s++;} return NULL;}\n')
    command = ['gcc', '-std=gnu11', '-O2', '-Wall', '-Wextra', '-Werror', '-fno-pie', '-no-pie',
               '-fsanitize=address,undefined', '-I', str(out / 'shim'), '-I', str(out / 'include/uapi'),
               '-I', str(out / 'include'), '-I', str(out / 'drivers/misc'), '-I', str(out),
               str(out / 'test_g.c'), '-o', str(out / 'fixture')]
    manifest = {'schema': 'ispass2027-e6-g-linux-fixture-v1', 'start': time.time(), 'status': 'running',
                'linux_commit': COMMIT, 'fixture_only': True, 'RUN_G_implemented': False,
                'command': command, 'input_sha256': {str(path.relative_to(out)): digest(path) for path in out.rglob('*') if path.is_file()}}
    path = out / 'manifest.json'
    try:
        path.write_text(json.dumps(manifest, indent=2) + '\n')
        with (out / 'compile.log').open('w') as log:
            subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=120, check=True)
        with (out / 'fixture.log').open('w') as log:
            subprocess.run([str(out / 'fixture')], stdout=log, stderr=subprocess.STDOUT, timeout=30, check=True)
        manifest.update(status='passed', fixture_log_sha256=digest(out / 'fixture.log'))
        print((out / 'fixture.log').read_text().strip())
    except Exception as error:
        manifest.update(status='failed', error=repr(error))
        raise
    finally:
        manifest['end'] = time.time()
        path.write_text(json.dumps(manifest, indent=2) + '\n')


if __name__ == '__main__':
    main()
