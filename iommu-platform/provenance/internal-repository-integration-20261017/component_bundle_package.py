#!/usr/bin/env python3
"""Package/verify component heads using excluded upstream prerequisites only.

This tool never builds, tests, checks out sources, changes source refs, edits
locks or contacts remotes. Restore operates only on an explicitly named target.
"""
import argparse
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
BUNDLES = HERE / 'bundles'
PLATFORM = HERE.parent.parent
MANIFEST = HERE / 'component-bundles.json'
RECEIPT = HERE / 'component-bundle-verification.json'
CONFIG = [
    ('iommu', 'cva6/corev_apu/riscv-iommu', 'c80bba7ae74b54b50eb1028e6efd4292661ca75b'),
    ('cva6', 'cva6', '1f992d808dc0abe599329dedf73842789f8013b8'),
    ('linux', 'linux', 'd371fb0f95d5bae20af8135f976ffd8a06bfaa23'),
    ('opensbi', 'opensbi', '48f91ee9c960f048c4a7d1da4447d31e04931e38'),
]

def git(repo, *args, data=None):
    p = subprocess.run(['git', '--no-replace-objects', *args], cwd=repo,
                       input=data, capture_output=True, check=True)
    return p.stdout

def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        while data := f.read(1024*1024): h.update(data)
    return h.hexdigest()

def save(path, value):
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2)+'\n')

def snapshot(repo):
    status = git(repo, 'status', '--porcelain=v2').decode()
    assert not status, (repo, status)
    idx = Path(git(repo, 'rev-parse', '--git-path', 'index').decode().strip())
    if not idx.is_absolute(): idx = repo/idx
    heads = {}
    for line in git(repo, 'for-each-ref', '--format=%(refname) %(objectname)', 'refs/heads').decode().splitlines():
        name, commit = line.split()
        heads[name] = {'commit': commit, 'tree': git(repo, 'rev-parse', commit+'^{tree}').decode().strip()}
    return {'head': git(repo, 'rev-parse', 'HEAD').decode().strip(),
            'head_tree': git(repo, 'rev-parse', 'HEAD^{tree}').decode().strip(),
            'symbolic_head': git(repo, 'symbolic-ref', 'HEAD').decode().strip(),
            'heads': heads, 'all_refs': git(repo, 'show-ref').decode(),
            'remotes': git(repo, 'remote', '-v').decode(),
            'index_sha256': digest(idx), 'status': status}

def object_inventory(repo, base):
    objects = {}
    for line in git(repo, 'rev-list', '--objects', '--branches', '^'+base).decode().splitlines():
        oid, _, path = line.partition(' ')
        objects[oid] = {'object': oid, 'path_hint': path}
    reply = git(repo, 'cat-file', '--batch-check=%(objectname) %(objecttype) %(objectsize)',
                data=('\n'.join(objects)+'\n').encode()).decode().splitlines()
    for line in reply:
        oid, kind, size = line.split()
        objects[oid].update(type=kind, bytes=int(size))
    return list(objects.values())

def check_objects_local(repo, objects):
    # Every expected object must remain readable after removing alternates.
    # Hash the raw Git object header + payload, not a formatted textual view.
    proc = subprocess.Popen(['git', '--no-replace-objects', 'cat-file', '--batch'],
                            cwd=repo, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    proc.stdin.write(('\n'.join(o['object'] for o in objects)+'\n').encode())
    proc.stdin.close()
    verified = []
    for item in objects:
        header = proc.stdout.readline().decode().strip().split()
        assert len(header) == 3 and header[0] == item['object'], header
        oid, kind, size_text = header
        size = int(size_text)
        assert kind == item['type'] and size == item['bytes']
        h = hashlib.sha1(f'{kind} {size}\0'.encode())
        payload = hashlib.sha256()
        remaining = size
        while remaining:
            block = proc.stdout.read(min(1024*1024, remaining))
            assert block, (oid, remaining)
            remaining -= len(block)
            h.update(block)
            payload.update(block)
        assert proc.stdout.read(1) == b'\n'
        assert h.hexdigest() == oid, oid
        verified.append(dict(item, git_object_id_verified=True, payload_sha256=payload.hexdigest()))
    assert proc.stdout.read() == b''
    error = proc.stderr.read()
    assert proc.wait() == 0, error.decode()
    return verified

def verify_one(entry, source_root, *, write_inventory=False):
    repo = source_root/entry['path']
    assert snapshot(repo) == entry['source_state'], entry['component']
    bundle = HERE/entry['bundle']
    assert bundle.stat().st_size == entry['bytes'] and digest(bundle) == entry['sha256']
    actual_heads = dict(line.split()[::-1] for line in git(repo, 'bundle', 'list-heads', str(bundle)).decode().splitlines())
    assert actual_heads == entry['bundle_header_heads']
    inventory = json.loads((HERE/entry['object_inventory']).read_text())
    assert digest(HERE/entry['object_inventory']) == entry['object_inventory_sha256']
    assert len(inventory) == entry['new_object_count']
    source_objects = object_inventory(repo, entry['excluded_base'])
    assert {o['object']: (o['type'], o['bytes']) for o in source_objects} == {o['object']: (o['type'], o['bytes']) for o in inventory}
    p = subprocess.run(['git', 'bundle', 'verify', str(bundle)], cwd=repo, capture_output=True, check=True)
    verify_output = (p.stdout+p.stderr).decode()
    # The bundle may be thin against the explicit upstream prerequisite. Only
    # use the source object store while index-pack fixes those deltas.
    with tempfile.TemporaryDirectory(prefix='bare-'+entry['component']+'-', dir=BUNDLES) as scratch:
        bare = Path(scratch)/'recovery.git'
        subprocess.run(['git', 'init', '--bare', str(bare)], capture_output=True, check=True)
        object_dir = Path(git(repo, 'rev-parse', '--git-path', 'objects').decode().strip())
        if not object_dir.is_absolute(): object_dir = repo/object_dir
        alternate = bare/'objects/info/alternates'
        alternate.write_text(str(object_dir.resolve())+'\n')
        unbundle = git(bare, 'bundle', 'unbundle', str(bundle)).decode()
        for ref, info in entry['heads'].items():
            git(bare, 'update-ref', ref, info['commit'], '0'*40)
        recovered_heads = {}
        for line in git(bare, 'for-each-ref', '--format=%(refname) %(objectname) %(tree)', 'refs/heads').decode().splitlines():
            ref, oid, tree = line.split()
            recovered_heads[ref] = {'commit': oid, 'tree': tree}
        assert recovered_heads == entry['heads']
        alternate.unlink()
        checked = check_objects_local(bare, inventory)
        # Read all newly packed head commits directly from the local recovery
        # store, after deleting the alternate; base-only heads remain explicit
        # prerequisites and are restored from manifest when that base exists.
        for ref, info in entry['heads'].items():
            if info['commit'] == entry['excluded_base']: continue
            body = git(bare, 'cat-file', 'commit', info['commit'])
            assert body.splitlines()[0].decode() == 'tree '+info['tree'], ref
    assert snapshot(repo) == entry['source_state'], entry['component']
    if write_inventory:
        save(HERE/entry['object_verification'], checked)
    return {'component': entry['component'], 'status': 'ALL_INCREMENTAL_OBJECT_IDENTITIES_VERIFIED',
            'bundle': entry['bundle'], 'bytes': entry['bytes'], 'sha256': entry['sha256'],
            'heads_preserved': len(entry['heads']), 'new_objects_locally_read_and_rehashed': len(checked),
            'all_head_names_commits_and_trees_actually_restored_with_prerequisite': True,
            'alternate_removed_before_identity_verification': True,
            'source_repo_state_unchanged': True, 'bundle_verify_output': verify_output,
            'unbundle_head_output': unbundle, 'temporary_bare_removed': True}

def create(source_root):
    assert not MANIFEST.exists(), 'Do not overwrite a previously frozen package'
    BUNDLES.mkdir(parents=True, exist_ok=True)
    integration = json.loads((HERE/'component-integration.json').read_text())
    active = {x['component']: x for x in integration['components']}
    entries = []
    for component, path, base in CONFIG:
        repo = source_root/path
        state = snapshot(repo)
        assert state['head'] == active[component]['active_commit']
        assert state['head_tree'] == active[component]['tree']
        for ref, info in state['heads'].items():
            git(repo, 'merge-base', '--is-ancestor', base, info['commit'])
        inventory = object_inventory(repo, base)
        inventory_path = BUNDLES/(component+'-objects.json')
        save(inventory_path, inventory)
        bundle = BUNDLES/(component+'-all-heads.bundle')
        assert not bundle.exists()
        git(repo, 'bundle', 'create', str(bundle), *state['heads'], '^'+base)
        if bundle.stat().st_size > 49*1024*1024:
            raise RuntimeError(f'{component} bundle exceeds 49 MiB: {bundle.stat().st_size} bytes; report before any splitting/sync')
        header = dict(line.split()[::-1] for line in git(repo, 'bundle', 'list-heads', str(bundle)).decode().splitlines())
        omitted = {ref: info for ref, info in state['heads'].items() if ref not in header}
        assert all(info['commit'] == base for info in omitted.values()), omitted
        assert all(state['heads'][ref]['commit'] == oid for ref, oid in header.items())
        prereq = []
        with bundle.open('rb') as f:
            version = f.readline().decode().strip()
            while line := f.readline():
                if line == b'\n': break
                if line.startswith(b'-'):
                    prereq.append(line[1:].decode().strip().split(' ', 1)[0])
        assert prereq == [base], prereq
        entry = {'component': component, 'path': path, 'excluded_base': base,
                 'base_tree': git(repo, 'rev-parse', base+'^{tree}').decode().strip(),
                 'prerequisite_commits': prereq, 'base_is_ancestor_of_every_head': True,
                 'heads': state['heads'], 'head_count': len(state['heads']),
                 'bundle': str(bundle.relative_to(HERE)), 'bundle_version': version,
                 'bytes': bundle.stat().st_size, 'sha256': digest(bundle),
                 'bundle_header_heads': header, 'prerequisite_only_heads': omitted,
                 'object_inventory': str(inventory_path.relative_to(HERE)),
                 'object_inventory_sha256': digest(inventory_path),
                 'object_verification': 'bundles/'+component+'-object-verification.json',
                 'new_object_count': len(inventory), 'new_object_uncompressed_bytes': sum(x['bytes'] for x in inventory),
                 'source_state': state}
        entries.append(entry)
        assert snapshot(repo) == state
    package = {'schema': 'integrated-component-all-head-bundles-v1',
               'created_utc': datetime.now(timezone.utc).isoformat(),
               'scope': 'all existing local heads in four component repositories, excluding fixed upstream ancestors',
               'component_integration_sha256': digest(HERE/'component-integration.json'),
               'components': entries,
               'prerequisites_note': 'Each complete excluded upstream repository/history is required for full recovery. Submodule repositories remain separate dependencies.',
               'base_only_heads_note': 'Standard Git bundles omit heads equal to the excluded base; their exact names/commit/tree are preserved in prerequisite_only_heads and restore recreates them from the available base.',
               'no_builds_tests_experiments_remote_changes_or_pushes': True,
               'no_full_kernel_file_hash_or_checkout': True}
    save(MANIFEST, package)
    verify(source_root, write_inventory=True)

def verify(source_root, write_inventory=False):
    package = json.loads(MANIFEST.read_text())
    results = [verify_one(e, source_root, write_inventory=write_inventory) for e in package['components']]
    receipt = {'schema': 'component-bundle-verification-v1', 'verified_utc': datetime.now(timezone.utc).isoformat(),
               'manifest_sha256': digest(MANIFEST), 'results': results,
               'total_heads': sum(r['heads_preserved'] for r in results),
               'total_new_objects': sum(r['new_objects_locally_read_and_rehashed'] for r in results),
               'no_builds_tests_experiments_or_source_changes': True,
               'verification_scope': 'Git metadata and incremental object identities; excluded base history is a prerequisite, not recopied or claimed packed'}
    if write_inventory: save(RECEIPT, receipt)
    print(json.dumps({'status': 'VERIFIED', 'heads': receipt['total_heads'], 'new_objects': receipt['total_new_objects'],
                      'bundles': [{'component': r['component'], 'bytes': r['bytes']} for r in results]}, indent=2))

def restore(component, target):
    package = json.loads(MANIFEST.read_text())
    entry = next(e for e in package['components'] if e['component'] == component)
    target = target.resolve()
    git(target, 'cat-file', '-e', entry['excluded_base']+'^{commit}')
    bundle = HERE/entry['bundle']
    assert digest(bundle) == entry['sha256']
    for ref, info in entry['heads'].items():
        existing = subprocess.run(['git', 'show-ref', '--verify', '--hash', ref], cwd=target, capture_output=True)
        if existing.returncode == 0:
            assert existing.stdout.decode().strip() == info['commit'], f'Existing ref differs; refusing overwrite: {ref}'
        else:
            assert existing.returncode == 128 or existing.returncode == 1
    git(target, 'bundle', 'verify', str(bundle))
    git(target, 'bundle', 'unbundle', str(bundle))
    for ref, info in entry['heads'].items():
        assert git(target, 'rev-parse', info['commit']+'^{tree}').decode().strip() == info['tree']
        current = subprocess.run(['git', 'show-ref', '--verify', '--hash', ref], cwd=target, capture_output=True)
        if current.returncode:
            git(target, 'update-ref', ref, info['commit'], '0'*40)
    print(json.dumps({'component': component, 'restored_heads': len(entry['heads']), 'checked_out_sources': False}, indent=2))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode', choices=['create', 'verify', 'restore'])
    parser.add_argument('--source-root', type=Path, default=PLATFORM)
    parser.add_argument('--component', choices=[x[0] for x in CONFIG])
    parser.add_argument('--target', type=Path)
    parser.add_argument('--write-receipt', action='store_true', help='Save a fresh verification receipt and per-object results')
    args = parser.parse_args()
    if args.mode == 'create': create(args.source_root)
    elif args.mode == 'verify': verify(args.source_root, write_inventory=args.write_receipt)
    else:
        assert args.component and args.target, 'restore requires --component and --target'
        restore(args.component, args.target)
