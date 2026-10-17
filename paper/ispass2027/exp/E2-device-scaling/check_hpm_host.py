#!/usr/bin/env python3
"""Compile unchanged production HPM function bodies against controlled host MMIO."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess


def stripped(text):
    return re.sub(r'^\s*#include[^\n]*', '', text, flags=re.M)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--linux', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=False)
    build = args.output / 'build'
    build.mkdir()
    source = args.linux / 'drivers/iommu/riscv/iommu-hpm.c'
    header = args.linux / 'include/linux/riscv_iommu_hpm.h'
    bits = args.linux / 'drivers/iommu/riscv/iommu-bits.h'
    (build / 'production_hpm.c').write_text(stripped(source.read_text()))
    (build / 'production_hpm_api.h').write_text(stripped(header.read_text()))
    definitions = []
    for line in bits.read_text().splitlines():
        if re.match(r'#define\s+RISCV_IOMMU_', line) and (
            'HPM' in line or 'IOCOUNT' in line or
            re.match(r'#define\s+RISCV_IOMMU_REG_CAP\s', line) or
            re.match(r'#define\s+RISCV_IOMMU_EVENT_(TLB_MISS|S1_PTW|S2_PTW)\s', line)):
            if line.endswith('\\'):
                raise ValueError('continued register definition')
            definitions.append(line)
    (build / 'production_hpm_bits.h').write_text('\n'.join(definitions) + '\n')
    harness = Path(__file__).with_name('hpm_session_host.c')
    binary = build / 'hpm-session-host'
    disabled = build / 'disabled.c'
    disabled.write_text(r'''
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
typedef uint64_t u64;
#define IS_REACHABLE(option) 0
#include "production_hpm_api.h"
int main(void) {
    struct riscv_iommu_hpm_session *session = (void *)1;
    struct riscv_iommu_hpm_multi_sample multi, zero = {};
    struct riscv_iommu_hpm_sample pair, pair_zero = {};
    assert(sizeof(pair) == 56);
    assert(!riscv_iommu_hpm_supported(NULL));
    assert(riscv_iommu_hpm_acquire(NULL, NULL, &session) == -EOPNOTSUPP && !session);
    session = (void *)1;
    assert(riscv_iommu_hpm_acquire_multi(NULL, 1, &session) == -EOPNOTSUPP && !session);
    memset(&multi, 0xff, sizeof(multi));
    riscv_iommu_hpm_read_multi(NULL, &multi);
    assert(!memcmp(&multi, &zero, sizeof(multi)));
    memset(&pair, 0xff, sizeof(pair));
    riscv_iommu_hpm_read(NULL, &pair);
    assert(!memcmp(&pair, &pair_zero, sizeof(pair)));
    riscv_iommu_hpm_release(NULL);
    puts("PASS CONFIG-disabled old/new header stubs and unchanged legacy sample size");
    return 0;
}
''')
    compiler = [os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O1', '-g',
                '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined',
                '-fno-omit-frame-pointer', '-I', str(build.resolve())]
    disabled_binary = build / 'hpm-disabled-host'
    commands = [('compile.log', compiler + [str(harness.resolve()), '-o', str(binary.resolve())]),
                ('run.log', [str(binary.resolve())]),
                ('disabled-compile.log', compiler + ['-Wno-unused-parameter', str(disabled.resolve()),
                                                    '-o', str(disabled_binary.resolve())]),
                ('disabled-run.log', [str(disabled_binary.resolve())])]
    receipts = []
    for name, command in commands:
        result = subprocess.run(command, capture_output=True, text=True)
        log = args.output / name
        log.write_text(result.stdout + result.stderr)
        receipts.append({'argv': command, 'exit_code': result.returncode,
                         'log': str(log.resolve()),
                         'log_sha256': hashlib.sha256(log.read_bytes()).hexdigest()})
        if result.returncode:
            (args.output / 'validation.json').write_text(json.dumps(
                {'passed': False, 'commands': receipts}, indent=2) + '\n')
            raise SystemExit(f'HPM check failed; see {log}')
    (args.output / 'validation.json').write_text(json.dumps({
        'schema': 'ispass2027.e2_hpm.host_validation.v1', 'passed': True,
        'scope': 'Production C control logic with stub MMIO/locks/tasks; not RTL event or board calibration',
        'linux_root': str(args.linux.resolve()),
        'sources': {str(p.relative_to(args.linux)): hashlib.sha256(p.read_bytes()).hexdigest()
                    for p in [source, header, bits]},
        'harness_sha256': hashlib.sha256(harness.read_bytes()).hexdigest(),
        'commands': receipts}, indent=2) + '\n')
    print((args.output / 'run.log').read_text(), end='')
    print((args.output / 'disabled-run.log').read_text(), end='')


if __name__ == '__main__':
    main()
