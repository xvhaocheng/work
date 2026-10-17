#!/usr/bin/env python3
"""Organize only the frozen Linux delta with an isolated Git index."""
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tarfile
from datetime import datetime, timezone

ROOT = Path('/home/xvhaocheng/work/iommu-platform')
REPO = ROOT / 'linux'
OUT = ROOT / 'provenance/source-organization-20261017/linux'
TEMP = ROOT / 'out/source-organization-20261017/worktrees/linux'
HEAD = 'c0f5e13ea81fa30d78f3bb1d6389e2397321e040'
UPSTREAM = 'd371fb0f95d5bae20af8135f976ffd8a06bfaa23'
PREFIX = 'codex/organize-20261017-linux-'
C = 'drivers/iommu/riscv/iommu.c'
H = 'drivers/iommu/riscv/iommu.h'
IDMA = 'drivers/misc/idma.c'
RH = 'include/linux/riscv_iommu_research.h'

def run(*args, cwd=REPO, data=None, env=None):
    return subprocess.run(args, cwd=cwd, input=data, stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE, check=True, env=env).stdout

def git(*args, data=None, env=None):
    return run('git', *args, data=data, env=env)

def sha(data):
    return hashlib.sha256(data).hexdigest()

def save(name, obj):
    (OUT / name).write_text(json.dumps(obj, ensure_ascii=False, indent=2) + '\n')

def old(path, ref=HEAD):
    return git('show', f'{ref}:{path}').decode()

def replace(text, before, after, count=1):
    assert text.count(before) == count, (before, text.count(before), count)
    return text.replace(before, after)

def span(text, start, end):
    assert text.count(start) == 1, start
    a = text.index(start)
    b = text.index(end, a+len(start))
    return text[a:b]

def function(text, name):
    pattern = r'^[^\n;]*\b' + re.escape(name) + r'\([^;]*?\n\{.*?^\}\n'
    matches = list(re.finditer(pattern, text, re.M | re.S))
    assert len(matches) == 1, (name, len(matches))
    return matches[0].group()

def freeze():
    OUT.mkdir(parents=True, exist_ok=True)
    TEMP.mkdir(parents=True, exist_ok=True)
    assert git('rev-parse', 'HEAD').decode().strip() == HEAD
    tracked = git('diff', '--name-only', 'HEAD').decode().splitlines()
    expected_tracked = {C, H, IDMA, 'drivers/iommu/riscv/io_pgtable.c',
                        'drivers/misc/idma_research.h', 'drivers/net/ethernet/Kconfig',
                        'drivers/net/ethernet/Makefile', 'include/uapi/linux/idma.h'}
    assert set(tracked) == expected_tracked
    untracked = git('ls-files', '--others', '--exclude-standard', '--',
                    'drivers/iommu/riscv', 'drivers/misc', 'drivers/net/ethernet/lowrisc',
                    'include/linux', 'include/uapi/linux').decode().splitlines()
    assert set(untracked) == {'drivers/misc/idma_sva_task.h', RH, 'include/uapi/linux/idma_sva.h',
                              'drivers/net/ethernet/lowrisc/Kconfig', 'drivers/net/ethernet/lowrisc/Makefile',
                              'drivers/net/ethernet/lowrisc/lowrisc_ethernet.c'}
    assert not git('ls-files', '--others', '--ignored', '--exclude-standard', '--',
                   'drivers/net/ethernet/lowrisc', 'drivers/misc/idma_sva_task.h', RH,
                   'include/uapi/linux/idma_sva.h').strip()
    frozen = {'timestamp_utc': datetime.now(timezone.utc).isoformat(), 'repo': str(REPO), 'head': HEAD,
              'head_tree': git('rev-parse', HEAD+'^{tree}').decode().strip(),
              'symbolic_head': git('symbolic-ref', 'HEAD').decode().strip(),
              'refs': git('show-ref').decode(), 'remotes': git('remote', '-v').decode(),
              'status': git('status', '--porcelain=v2', '--untracked-files=all').decode(),
              'index_sha256': sha((Path(git('rev-parse', '--absolute-git-dir').decode().strip())/'index').read_bytes()),
              'tracked_delta_paths': tracked, 'untracked_source_paths': untracked, 'files': {}}
    with tarfile.open(OUT / 'frozen-delta.tar.gz', 'w:gz') as tar:
        for path in sorted(tracked+untracked):
            source = REPO/path
            data = source.read_bytes()
            index = git('ls-files', '-s', '--', path).decode().strip()
            mode = index.split()[0] if index else ('100755' if os.access(source, os.X_OK) else '100644')
            assert mode in {'100644', '100755'}
            frozen['files'][path] = {'sha256': sha(data), 'bytes': len(data), 'mode': mode,
                                      'blob': git('hash-object', '--stdin', data=data).decode().strip(),
                                      'was_untracked': path in untracked}
            tar.add(source, arcname=path, recursive=False)
    save('frozen-input.json', frozen)
    (OUT/'original-working.patch').write_bytes(git('diff', '--binary', 'HEAD'))
    (OUT/'original-staged.patch').write_bytes(git('diff', '--cached', '--binary'))
    # Read-only branch query was captured during bounded scoping; no fetch.
    (OUT/'upstream-ls-remote.txt').write_text('record_written_utc='+frozen['timestamp_utc']+'\nquery_timestamp_utc=not_recorded; read-only query was performed earlier in this task\n'+
        '5d0c230f1de8c7515b6567d9afba1f196fb4e2f4\trefs/heads/master\n'+
        UPSTREAM+'\trefs/heads/wip/riscv-iommu\n')
    git('merge-base', '--is-ancestor', UPSTREAM, HEAD)
    print(json.dumps({'head': HEAD, 'frozen_delta_files': len(frozen['files']), 'ordinary_plus_existing_research_software_base': True}, indent=2))

def sources():
    with tarfile.open(OUT/'frozen-delta.tar.gz') as tar:
        return {m.name: tar.extractfile(m).read().decode() for m in tar.getmembers()}

def create():
    frozen = json.loads((OUT/'frozen-input.json').read_text())
    source = sources()
    index = TEMP/'organization.index'
    previous_steps = json.loads((OUT/'steps.json').read_text()) if (OUT/'steps.json').exists() else []
    if index.exists():
        index = TEMP/'organization-recovery-1.index'
        assert not index.exists(), 'Do not replace previous construction evidence'
    env = dict(os.environ, GIT_INDEX_FILE=str(index))
    git('read-tree', HEAD, env=env)
    steps = []
    parent = HEAD
    staged = {}

    def put(path, text):
        data = text.encode()
        blob = git('hash-object', '-w', '--stdin', data=data).decode().strip()
        mode = frozen['files'][path]['mode']
        git('update-index', '--add', '--cacheinfo', f'{mode},{blob},{path}', env=env)
        target = TEMP/'sources'/path
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)
        staged[path] = text

    def commit(feature, name, role, title, semantic):
        nonlocal parent
        tree = git('write-tree', env=env).decode().strip()
        if len(steps) < len(previous_steps):
            existing = previous_steps[len(steps)]
            assert existing['feature_id'] == feature and existing['tree'] == tree
            assert existing['parent'] == parent
            steps.append(existing)
            parent = existing['commit']
            return
        oid = git('commit-tree', tree, '-p', parent, '-m', 'organize: '+title).decode().strip()
        ref = 'refs/heads/'+PREFIX+name
        git('update-ref', ref, oid, '0'*40)
        entry = {'feature_id': feature, 'name': name, 'role': role, 'description': title,
                 'commit': oid, 'tree': tree, 'ref': ref, 'parent': parent,
                 'stack_prerequisites': [steps[-1]['feature_id']] if steps else ['L0'],
                 'semantic_requires': semantic,
                 'changed_paths': git('diff-tree', '--no-commit-id', '--name-only', '-r', oid).decode().splitlines(),
                 'validation': 'static reconstruction only; not built, tested or run',
                 'independence_note': '功能前提与Git补丁堆叠分别列出；没有验证独立挑选、重排后的接口、编译或运行。'}
        steps.append(entry)
        save('steps.json', steps)
        parent = oid

    put('drivers/iommu/riscv/io_pgtable.c', source['drivers/iommu/riscv/io_pgtable.c'])
    commit('LF1', 'fix-pgtable-free', 'ordinary page-table release fix on existing software base',
           'free every owned child page-table page including lowest-level tables', ['L0'])
    c = replace(old(C), '\tINIT_LIST_HEAD(&ep->regions);\n\tINIT_LIST_HEAD(&ep->regions);\n',
                '\tINIT_LIST_HEAD(&ep->regions);\n\tINIT_LIST_HEAD(&ep->domains);\n')
    put(C, c)
    commit('LF2', 'fix-endpoint-list', 'ordinary endpoint list initialization fix',
           'initialize the endpoint domain-list node', ['L0'])
    post = function(c, 'riscv_iommu_post_sync')
    mismatch = span(post, '\n\tif (tail != last) {\n', '\n\tnext = (last + 1)')
    post_fixed = replace(post, mismatch, '\n\tif (tail != last) {\n\t\tspin_unlock_irqrestore(&iommu->cq_lock, flags);\n\t\treturn false;\n\t}\n')
    c = replace(c, post, post_fixed)
    put(C, c)
    commit('LF3', 'fix-cq-tail', 'conservative CQ consistency correction candidate; outside minimal adaptation baseline; unvalidated',
           'conservatively reject a mismatched CQ hardware tail before posting', ['L0'])
    # LCQ: retain the final exclusive CQ path, without borrowed-domain changes.
    c = replace(c, '#include <linux/dma-map-ops.h>\n', '#include <linux/dma-map-ops.h>\n#include <linux/riscv_iommu_research.h>\n')
    c = replace(c, 'static const struct iommu_ops riscv_iommu_ops;\n',
                'static const struct iommu_ops riscv_iommu_ops;\n\nstatic bool research;\nmodule_param(research, bool, 0400);\nMODULE_PARM_DESC(research, "Enable restricted synchronous DID10/11 Sv39 research API");\n')
    for name in ['riscv_iommu_post_sync', 'riscv_iommu_cmdq_process', 'riscv_iommu_remove', 'riscv_iommu_init']:
        new = function(source[C], name)
        if name == 'riscv_iommu_init':
            new = replace(new, '\t/* Upper-half PSCIDs are private domains, so real PASIDs use 19 bits. */\n', '')
            new = replace(new, '\t\tiommu->iommu.max_pasids = RISCV_IOMMU_DOMAIN_PSCID_MIN;\n', '\t\tiommu->iommu.max_pasids = 1u << 20;\n')
        c = replace(c, function(c, name), new)
    support = span(source[C], '/* All callers use the opt-in static single-mm current-platform contract. */\n',
                   'struct iommu_domain *riscv_iommu_research_domain_alloc(')
    barrier = span(source[C], '#define RISCV_IOMMU_RESEARCH_CQ_ERRORS ',
                   '/*\n * Validate one acquire-time target')
    c = replace(c, '/*\n * Validate one acquire-time target', support+barrier+'/*\n * Validate one acquire-time target')
    put(C, c)
    h = old(H)
    old_fields = span(h, '\tatomic64_t cq_error_seq;\n', '};\n\nstruct riscv_iommu_domain')
    new_fields = span(source[H], '\tatomic64_t cq_error_seq;\n', '};\n\nstruct riscv_iommu_domain')
    put(H, replace(h, old_fields, new_fields))
    header = source[RH]
    header = replace(header, span(header, '/*\n * Restricted current-platform experiment;', '/*\n * Caller already owns DID10/11'), '')
    header = replace(header, span(header, 'static inline struct iommu_domain *\n', 'static inline int\nriscv_iommu_research_barrier'), '')
    put(RH, header)
    commit('LCQ', 'research-cq', 'research exclusive CQ ownership, completion certificate and quarantine API',
           'isolate restricted CQ marker certificates and exclusive error history', ['LF2', 'LF3'])
    # LDOM: namespace and lifecycle changes are research prerequisites, not stock fixes.
    for path in [C, H, RH]: put(path, source[path])
    commit('LDOM', 'research-domains', 'research private-domain identifiers, borrowed root and lifetime protocol',
           'add restricted borrowed-root domains and certified lifetime handling', ['LCQ', 'LF1', 'LF2'])
    # LSVA: fixed synchronous task, independent of the composition ioctl.
    idma = replace(old(IDMA), '// SPDX-License-Identifier: GPL-2.0\n', '// SPDX-License-Identifier: GPL-2.0\n#include <linux/capability.h>\n')
    idma = replace(idma, '#include "idma_research.h"\n', '#include "idma_research.h"\n#include "idma_sva_task.h"\n')
    idma = replace(idma, '\tswitch (cmd) {\n\tcase IDMA_IOC_RUN_RESEARCH:',
                   '\tswitch (cmd) {\n\tcase IDMA_IOC_SVA_CAPS:\n\t\treturn idma_sva_caps_ioctl(idma, (void __user *)arg);\n\tcase IDMA_IOC_SVA_RUN:\n\t\treturn idma_sva_run_ioctl(idma, (void __user *)arg);\n\tcase IDMA_IOC_RUN_RESEARCH:')
    idma = replace(idma, function(idma, 'idma_remove'), function(source[IDMA], 'idma_remove'))
    for path in ['drivers/misc/idma_sva_task.h', 'include/uapi/linux/idma_sva.h']: put(path, source[path])
    put(IDMA, idma)
    commit('LSVA', 'research-sva-task', 'private synchronous fixed-task SVA/shadow/IOVA research ABI; not generic SVA',
           'add privileged fixed synchronous DMA task and SVA UAPI', ['LDOM', 'LCQ'])
    put('include/uapi/linux/idma.h', source['include/uapi/linux/idma.h'])
    commit('LCOMP_ABI', 'composition-uapi', 'research composition ioctl UAPI declarations',
           'declare the versioned S N P PN composition ioctl', ['L0'])
    for path in [IDMA, 'drivers/misc/idma_research.h']: put(path, source[path])
    commit('LCOMP', 'composition-ioctl', 'research composition control, capture, restoration and optional stage logging',
           'add composition configuration capture restoration and session integration', ['LCOMP_ABI', 'L0'])
    for path in ['drivers/net/ethernet/Kconfig', 'drivers/net/ethernet/Makefile',
                 'drivers/net/ethernet/lowrisc/Kconfig', 'drivers/net/ethernet/lowrisc/Makefile',
                 'drivers/net/ethernet/lowrisc/lowrisc_ethernet.c']:
        put(path, source[path])
    commit('LNET', 'research-current', 'independent LowRISC network integration; exact frozen Linux endpoint',
           'preserve LowRISC Ethernet driver and build integration', ['L0'])
    assert set(staged) == set(source)
    for path in source: assert staged[path] == source[path], path
    print(json.dumps({'endpoint': parent, 'tree': steps[-1]['tree'], 'new_commits': len(steps)}, indent=2))

if __name__ == '__main__':
    {'freeze': freeze, 'create': create}[sys.argv[1]]()
