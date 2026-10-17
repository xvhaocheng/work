#!/usr/bin/env python3
"""Verify static Linux delta reconstruction without builds or full checkouts."""
import json
import os
from pathlib import Path
import subprocess
import sys
from datetime import datetime, timezone

sys.dont_write_bytecode = True
from organize import OUT, TEMP, REPO, HEAD, UPSTREAM, PREFIX, C, H, IDMA, RH, git, run, sha, save, old, function, sources

frozen = json.loads((OUT/'frozen-input.json').read_text())
steps = json.loads((OUT/'steps.json').read_text())
by_id = {s['feature_id']: s for s in steps}
end = steps[-1]['commit']
src = sources()

def new_index(name):
    path = TEMP/name
    assert not path.exists(), 'Preserve previous verification evidence'
    return dict(os.environ, GIT_INDEX_FILE=str(path))

def assert_original_unchanged():
    assert git('rev-parse', 'HEAD').decode().strip() == frozen['head']
    assert git('symbolic-ref', 'HEAD').decode().strip() == frozen['symbolic_head']
    assert git('status', '--porcelain=v2', '--untracked-files=all').decode() == frozen['status']
    index = Path(git('rev-parse', '--absolute-git-dir').decode().strip())/'index'
    assert sha(index.read_bytes()) == frozen['index_sha256']
    assert git('remote', '-v').decode() == frozen['remotes']
    oldrefs = {l.split()[1]: l.split()[0] for l in frozen['refs'].splitlines()}
    nowrefs = {l.split()[1]: l.split()[0] for l in git('show-ref').decode().splitlines()}
    for ref, oid in oldrefs.items(): assert nowrefs[ref] == oid
    assert all(r.startswith('refs/heads/'+PREFIX) for r in nowrefs.keys()-oldrefs.keys())
    for path, info in frozen['files'].items():
        assert sha((REPO/path).read_bytes()) == info['sha256'], path

assert_original_unchanged()
env = new_index('expected-frozen.index')
git('read-tree', HEAD, env=env)
for path, info in frozen['files'].items():
    data = src[path].encode()
    assert sha(data) == info['sha256']
    assert sha(git('show', f'{end}:{path}')) == info['sha256'], path
    assert git('hash-object', '--stdin', data=data).decode().strip() == info['blob']
    git('update-index', '--add', '--cacheinfo', f"{info['mode']},{info['blob']},{path}", env=env)
expected_tree = git('write-tree', env=env).decode().strip()
assert expected_tree == steps[-1]['tree']
assert set(git('diff', '--name-only', HEAD, end).decode().splitlines()) == set(src)

# Check the source boundaries which justified the mixed-hunk split.
lf3 = old(C, by_id['LF3']['commit'])
assert not any(token in lf3 for token in ['research_cq_owned', 'cq_posted', 'research_epoch', 'research_marker'])
lcq = old(C, by_id['LCQ']['commit'])
assert 'riscv_iommu_research_barrier' in lcq
assert not any(token in lcq for token in ['RISCV_IOMMU_DOMAIN_PSCID_MIN', 'pgd_borrowed', 'riscv_iommu_domain_mutex', 'borrowed_mm'])
lcqh = old(H, by_id['LCQ']['commit'])
assert 'research_cq_owned' in lcqh and 'pgd_borrowed' not in lcqh
assert 'riscv_iommu_research_domain_alloc' not in old(RH, by_id['LCQ']['commit'])
for name in ['riscv_iommu_post_sync', 'riscv_iommu_cmdq_process', 'riscv_iommu_remove']:
    assert function(lcq, name) == function(src[C], name), name
sva_idma = old(IDMA, by_id['LSVA']['commit'])
assert '#include "idma_sva_task.h"' in sva_idma
assert 'IDMA_COMPOSITION_' not in sva_idma and 'idma_composition_' not in sva_idma
assert old('include/uapi/linux/idma.h', by_id['LCOMP_ABI']['commit']) == src['include/uapi/linux/idma.h']

patchdir = OUT/'patches'
patchdir.mkdir(exist_ok=True)
patches = git('format-patch', '--no-signature', '--full-index', '--binary', '-o', str(patchdir), f'{HEAD}..{end}').decode().splitlines()
assert len(patches) == len(steps) == 9
env = new_index('patch-reconstruction.index')
git('read-tree', HEAD, env=env)
patch_results = []
for step, patch in zip(steps, patches):
    data = Path(patch).read_bytes()
    git('apply', '--cached', '--check', data=data, env=env)
    git('apply', '--cached', data=data, env=env)
    tree = git('write-tree', env=env).decode().strip()
    assert tree == step['tree'], step['feature_id']
    step['patch'] = str(Path(patch).relative_to(OUT))
    step['patch_sha256'] = sha(data)
    patch_results.append({'feature_id': step['feature_id'], 'commit': step['commit'], 'tree': tree, 'matches': True})
save('steps.json', steps)
(OUT/'series').write_text('\n'.join(s['patch'] for s in steps)+'\n')

bundle = OUT/'linux-delta-series.bundle'
assert not bundle.exists()
git('bundle', 'create', str(bundle), *[s['ref'] for s in steps], '^'+HEAD)
check = subprocess.run(['git', 'bundle', 'verify', str(bundle)], cwd=REPO, capture_output=True, check=True)
(OUT/'bundle-verification.txt').write_bytes(check.stdout+check.stderr)
# Reuse the prerequisite object store read-only while thickening the incremental
# bundle, then remove the alternate before checking recovered new objects.
# This avoids copying the complete Linux history or checking out the kernel.
recovery = TEMP/'bundle-recovery.git'
assert not recovery.exists()
run('git', 'init', '--bare', str(recovery), cwd=TEMP)
objectdir = Path(git('rev-parse', '--git-path', 'objects').decode().strip())
if not objectdir.is_absolute(): objectdir = REPO/objectdir
alternate = recovery/'objects/info/alternates'
alternate.write_text(str(objectdir.resolve())+'\n')
run('git', '-C', str(recovery), 'bundle', 'unbundle', str(bundle), cwd=TEMP)
alternate.unlink()
recovered = []
for step in steps:
    commit_text = run('git', '-C', str(recovery), 'cat-file', 'commit', step['commit'], cwd=TEMP)
    assert sha(commit_text) == sha(git('cat-file', 'commit', step['commit']))
    assert commit_text.splitlines()[0].decode() == 'tree '+step['tree']
    run('git', '-C', str(recovery), 'cat-file', '-e', step['tree'], cwd=TEMP)
    recovered.append({'commit': step['commit'], 'tree': step['tree'], 'available_after_removing_alternate': True})
for path in src:
    blob = frozen['files'][path]['blob']
    assert sha(run('git', '-C', str(recovery), 'cat-file', 'blob', blob, cwd=TEMP)) == frozen['files'][path]['sha256']
assert not alternate.exists()

history_refs = [UPSTREAM, '4c2d88b85dd2', '05b08d813c10', 'eda7d89fb4bf', '5fe2b7ce2a53',
    '0cb98744633b', '48185ad5dc62', 'fb443888cacb', '2ef3f4b82afd', 'affbfbf6adda',
    '031e91dbadc1', '53d3113dee74', '3b47ab1645e4', '853e60b234c3', 'b7979a1a06ac',
    '45bf76b15e36', '94a519eebf07', '1b268b89200c', 'd2cd32d78918', '2de370cd344e',
    '9f8da9e7a6a0', 'efd9461d6459', 'a8d28cd44f35', '9ad05d00cca7', '5853e425caf8', HEAD,
    '8e3a7406fadb', '77805904b3c0']
history = []
for ref in history_refs:
    oid = git('rev-parse', ref).decode().strip()
    metadata = git('show', '-s', '--format=%H%n%P%n%T%n%aI%n%cI%n%an <%ae>%n%s', oid).decode().splitlines()
    ancestor = subprocess.run(['git', 'merge-base', '--is-ancestor', oid, HEAD], cwd=REPO, capture_output=True).returncode
    assert ancestor in {0, 1}
    item = {'commit': oid, 'parents': metadata[1].split(), 'tree': metadata[2], 'author_date': metadata[3],
            'committer_date': metadata[4], 'author': metadata[5], 'subject': metadata[6],
            'ancestor_of_original_head': ancestor == 0,
            'changed_paths': git('diff-tree', '--no-commit-id', '--name-only', '-r', oid).decode().splitlines(),
            'reuse': 'existing commit retained unchanged; metadata and paths indexed, no old experiment rerun'}
    if oid.startswith('8e3a7406'):
        item['scope_note'] = 'Separate IOVA-offset contribution; not in original HEAD ancestry. Equivalent production behavior comes from 3b47ab1645e4, which also contains a CVA6 DMA cache workaround.'
    if oid.startswith('77805904'):
        item['scope_note'] = 'Separate HPM macro contribution; not in original HEAD ancestry and not applied to the exact frozen endpoint.'
    history.append(item)
save('existing-history-index.json', history)

def evidence(feature, path, needle, context=5):
    entry = by_id[feature]
    lines = old(path, entry['commit']).splitlines()
    matches = [i for i, line in enumerate(lines) if needle in line]
    assert matches, (feature, path, needle)
    a, b = max(0, matches[0]-context), min(len(lines), matches[0]+context+1)
    return {'feature_id': feature, 'commit': entry['commit'], 'path': path,
            'line_start': a+1, 'line_end': b, 'exact_excerpt': '\n'.join(lines[a:b])}
evidence_rows = [
    evidence('LF1', 'drivers/iommu/riscv/io_pgtable.c', 'A child table is owned', 9),
    evidence('LF2', C, 'INIT_LIST_HEAD(&ep->domains)'),
    evidence('LF3', C, 'if (tail != last)', 6),
    evidence('LCQ', C, 'if (iommu->research_cq_owned', 8),
    evidence('LCQ', C, 'static irqreturn_t riscv_iommu_cmdq_process', 28),
    evidence('LCQ', C, 'iommu->research_cq_owned = true', 6),
    evidence('LDOM', C, 'RISCV_IOMMU_DOMAIN_PSCID_MIN', 9),
    evidence('LDOM', C, 'reclaim_id =', 14),
    evidence('LDOM', C, 'domain->pgd_root = mm->pgd', 11),
    evidence('LSVA', 'drivers/misc/idma_sva_task.h', 'static void __noreturn idma_sva_retain', 13),
    evidence('LSVA', 'include/uapi/linux/idma_sva.h', 'Private, synchronous', 5),
    evidence('LCOMP_ABI', 'include/uapi/linux/idma.h', 'Same-BIT composition', 18),
    evidence('LCOMP', IDMA, 'static int idma_composition_configure', 27),
    evidence('LCOMP', IDMA, 'module_param_named(composition_trace', 9),
    evidence('LNET', 'drivers/net/ethernet/Kconfig', 'lowrisc/Kconfig'),
    evidence('LNET', 'drivers/net/ethernet/lowrisc/lowrisc_ethernet.c', 'Copyright (C) 2016', 8),
]
save('critical-source-evidence.json', evidence_rows)
for step in steps:
    step['source_evidence_rows'] = [i for i, row in enumerate(evidence_rows) if row['feature_id'] == step['feature_id']]
save('steps.json', steps)
save('manifest.json', {
    'schema': 'linux-delta-source-organization-v1', 'created_utc': datetime.now(timezone.utc).isoformat(),
    'scope': 'reuse existing Linux history and organize only the frozen 8 tracked plus 6 untracked source delta',
    'upstream_provenance': {'url': 'https://github.com/malejo97/linux.git', 'branch': 'wip/riscv-iommu',
        'commit': UPSTREAM, 'ancestor_of_original_head': True, 'evidence': 'upstream-ls-remote.txt',
        'not_linux_mainline_claim': True},
    'original': {'head': HEAD, 'tree': frozen['head_tree'], 'symbolic_head': frozen['symbolic_head'],
        'capture': 'frozen-input.json', 'scope_note': 'This existing software base already contains HPM, DMA experiments, batch/path/E3/P4M/research2. It is not a stock Linux kernel or a minimal ordinary DMA software profile.'},
    'anchors': {'existing_software': HEAD, 'ordinary_fix_candidates': by_id['LF3']['commit'],
        'restricted_cq': by_id['LCQ']['commit'], 'restricted_domains': by_id['LDOM']['commit'],
        'sva_fixed_task': by_id['LSVA']['commit'], 'composition': by_id['LCOMP']['commit'], 'research_current': end},
    'steps': steps, 'linear_patch_order_required': True, 'independent_cherry_picking_verified': False,
    'existing_history_index': 'existing-history-index.json',
    'hpm_macro_fix': '77805904b3c0 is retained as a separate preexisting contribution ref; it is not in original HEAD ancestry and is intentionally absent from the exact frozen endpoint.',
    'cq_tail_candidate': 'LF3 extracts only existing tail mismatch rejection behavior. It is a conservative consistency correction candidate outside the minimal RTL adaptation baseline, without build or execution validation.',
    'research_boundaries': 'CQ ownership/overflow/epoch/poison/IRQ direct-CSR changes and marker certificate belong to LCQ. Identifier partition, borrowed root, domain transitions and lifetime reclamation belong to LDOM, including changes affecting ordinary paths. Neither is called necessary ordinary integration.',
    'recovery': {'bundle': bundle.name, 'prerequisite_commit': HEAD, 'frozen_delta': 'frozen-delta.tar.gz',
        'series': 'series', 'frozen_tree': expected_tree, 'unchanged_files_identified_by': frozen['head_tree'],
        'bundle_recovery_note': 'Fresh bare object directory temporarily read original prerequisite objects while unbundling; alternates removed before all nine new commits, their tree objects and all 14 delta blobs were checked locally. Full Linux history was not copied or checked out.'},
    'verification': 'reconstruction-verification.json', 'critical_source': 'critical-source-evidence.json',
    'limits': ['Static source reconstruction only; no build, host test, simulation, experiment or board run.',
               'New intermediate interfaces and concurrency behavior were not validated by execution.',
               'No new stock-software/native performance baseline is claimed; original RTL baselines use Linux only as an explicitly fixed software dependency.',
               'Historic experiments remain bound to original code identities and overlays; no new SHA is backdated as their source.',
               'LowRISC provenance is limited to retained source header attribution and this platform integration; no new external source comparison was performed.'],
})
assert_original_unchanged()
save('reconstruction-verification.json', {
    'timestamp_utc': datetime.now(timezone.utc).isoformat(), 'status': 'STATIC_RECONSTRUCTION_VERIFIED',
    'tests_experiments_or_builds_run': [], 'build_verified': False,
    'original_head_tree': frozen['head_tree'], 'endpoint_commit': end, 'frozen_tree': expected_tree,
    'delta_files_verified': len(src), 'untracked_files_included': frozen['untracked_source_paths'],
    'whole_tree_verified_by_base_tree_plus_exact_delta': True,
    'original_head_existing_refs_index_status_delta_files_and_remotes_unchanged': True,
    'patch_steps': patch_results, 'bundle_sha256': sha(bundle.read_bytes()),
    'bundle_prerequisite': HEAD, 'bundle_recovered_new_objects_after_removing_alternate': recovered,
    'bundle_recovered_all_delta_blobs_locally': True,
    'full_kernel_history_copied_or_checked_out': False,
    'source_boundaries_checked': ['LF3 has no CQ research fields', 'LCQ has no borrowed root/private-ID/domain mutex',
        'LCQ header has no domain allocation/release interface', 'LCQ post/IRQ/remove functions equal frozen final sources',
        'LSVA driver stage has no composition controls', 'LCOMP ABI matches frozen final header'],
})
print(json.dumps({'endpoint': end, 'tree': expected_tree, 'patches_verified': len(steps),
                  'delta_files_verified': len(src), 'bundle_exact_new_objects_verified': True}, indent=2))
