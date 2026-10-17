#!/usr/bin/env python3
"""Run actual leaf/map/walk functions with bounded host page-allocation stubs."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess


def sha(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024*1024), b''):
            digest.update(block)
    return digest.hexdigest()


def function(source, name):
    matches = re.findall(r'^[\w *]+\b' + name + r'\(.*?^\}', source, re.M | re.S)
    assert len(matches) == 1, (name, len(matches))
    return matches[0]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--linux', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=False)
    build = args.output / 'build'
    include = build / 'include/linux'
    include.mkdir(parents=True)
    (include / 'types.h').write_text('#include <stdint.h>\ntypedef uint64_t u64;\ntypedef uint32_t u32;\ntypedef uint64_t phys_addr_t;\n')
    (include / 'errno.h').write_text('#include <asm-generic/errno.h>\n')
    (include / 'kconfig.h').write_text('#define IS_REACHABLE(x) TEST_REACHABLE\n')
    (include / 'string.h').write_text('#include <string.h>\n')
    header = args.linux / 'include/linux/riscv_iommu_leaf.h'
    (include / header.name).write_bytes(header.read_bytes())
    pgtable_path = args.linux / 'drivers/iommu/riscv/io_pgtable.c'
    pgtable = pgtable_path.read_text()
    domain_path = args.linux / 'drivers/iommu/riscv/iommu.c'
    domain = domain_path.read_text()
    names = ['get_page_size', 'riscv_iommu_pt_walk_free', 'riscv_iommu_pt_walk_alloc',
             'riscv_iommu_pt_walk_fetch', 'riscv_iommu_map_pages', 'riscv_iommu_unmap_pages',
             'riscv_iommu_iova_to_phys', 'riscv_iommu_pgtable_query_leaf']
    functions = [function(pgtable, name) for name in names]
    functions += [function(domain, 'riscv_iommu_query_leaf')]
    internal_header = args.linux / 'drivers/iommu/riscv/iommu.h'
    constants = '\n'.join(re.findall(r'^#define IOMMU_PAGE_SIZE_.*$', internal_header.read_text(), re.M))
    harness = Path(__file__).with_name('leaf_host.c')
    unit = build / 'leaf_host.c'
    unit.write_text(harness.read_text().replace('/* CONSTANTS */', constants).replace(
        '/* FUNCTIONS */', '\n\n'.join(functions)))
    disabled = build / 'disabled.c'
    disabled.write_text('''#include <assert.h>
#include <string.h>
#include <linux/riscv_iommu_leaf.h>
int main(void) {
    struct riscv_iommu_leaf_info out, zero = {};
    memset(&out, 0xa5, sizeof(out));
    assert(riscv_iommu_query_leaf((void *)1, 4096, &out) == -EOPNOTSUPP);
    assert(!memcmp(&out, &zero, sizeof(out)));
    assert(riscv_iommu_query_leaf((void *)1, 4096, 0) == -EINVAL);
    return 0;
}
''')
    report = {'schema': 'ispass2027.e4_leaf.host.v1',
              'scope': 'Extracted unmodified production function bodies; host allocation/PTE/MM stubs, no kernel/RTL/board execution',
              'sources': {str(p.resolve()): sha(p) for p in (pgtable_path, domain_path, header, internal_header, harness)},
              'extracted_functions': names + ['riscv_iommu_query_leaf'],
              'commands': []}
    common = ['cc', '-std=gnu11', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
              '-Wno-unused-parameter', '-Wno-sign-compare', '-fsanitize=address,undefined',
              '-fno-omit-frame-pointer', '-fno-pie', '-no-pie', '-I', str(include.parent.resolve())]
    try:
        for label, source, reachable in [('leaf', unit, 1), ('disabled', disabled, 0)]:
            binary = build / label
            for stage, command in [('compile', common + [f'-DTEST_REACHABLE={reachable}', str(source.resolve()), '-o', str(binary.resolve())]),
                                   ('run', [str(binary.resolve())])]:
                log = args.output / (label + '-' + stage + '.log')
                with log.open('w') as output:
                    result = subprocess.run(command, stdout=output, stderr=subprocess.STDOUT, text=True, timeout=60)
                report['commands'].append({'argv': command, 'exit_code': result.returncode,
                                           'log': str(log.resolve()), 'log_sha256': sha(log)})
                if result.returncode:
                    raise RuntimeError(f'{label} {stage} failed: {log}')
                if stage == 'run':
                    result.stdout = log.read_text()
                    print(result.stdout, end='')
                    if label == 'leaf':
                        report['baseline_table_retention'] = re.search(r'^BASELINE_ALTERNATION[^\n]+', result.stdout, re.M)[0]
                        report['result'] = re.search(r'^PASS [^\n]+', result.stdout, re.M)[0]
        report['status'] = 'passed'
    except BaseException as error:
        report.update(status='failed', error=type(error).__name__ + ': ' + str(error))
        raise
    finally:
        (args.output / 'validation.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
