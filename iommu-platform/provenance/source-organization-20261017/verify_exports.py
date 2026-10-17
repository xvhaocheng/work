#!/usr/bin/env python3
"""Read-only Git/archive/export identity audit; no builds or hardware actions."""
import datetime
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import stat
import subprocess
import sys
import tarfile
import tempfile

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
PLATFORM = HERE.parents[1]
REGISTRY = HERE / 'source-profiles.json'
EXPORTER = HERE / 'export_sources.py'
STAGER = PLATFORM / 'scripts/platform.py'
REPORT = HERE / 'source-export-verification.json'
TREE_CACHE = {}
ARCHIVE_CACHE = {}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def canonical(value):
    return json.dumps(value, sort_keys=True, separators=(',', ':'), ensure_ascii=True).encode()


def git(repo, *args, data=None, env=None):
    return subprocess.check_output(['git', '-C', str(repo), *args], input=data, env=env)


def component_repo(relative):
    parts = PurePosixPath(relative)
    if parts.is_absolute() or '..' in parts.parts or str(parts) != relative:
        raise ValueError('Noncanonical component path: ' + relative)
    repo = PLATFORM / relative
    top = Path(git(repo, 'rev-parse', '--show-toplevel').decode().strip()).resolve()
    if top != repo.resolve():
        raise ValueError('Component is not its own repository: ' + relative)
    return repo


def selected_tree(relative, commit):
    key = relative, commit
    if key in TREE_CACHE:
        return TREE_CACHE[key]
    if not re.fullmatch('[0-9a-f]{40}', commit):
        raise ValueError('Commit is not full immutable SHA: ' + relative)
    repo = component_repo(relative)
    if git(repo, 'cat-file', '-t', commit).decode().strip() != 'commit':
        raise ValueError('Selected object is not a commit: ' + relative)
    tree = git(repo, 'rev-parse', commit + '^{tree}').decode().strip()
    entries = {}
    for raw in git(repo, 'ls-tree', '-r', '-t', '-z', commit).split(b'\0'):
        if not raw:
            continue
        header, path = raw.split(b'\t', 1)
        mode, kind, oid = header.decode().split()
        name = os.fsdecode(path)
        if PurePosixPath(name).is_absolute() or '..' in PurePosixPath(name).parts:
            raise ValueError('Unsafe tracked path: ' + name)
        entries[name] = dict(mode=mode, kind=kind, oid=oid)
    result = dict(path=relative, commit=commit, tree=tree, entries=entries)
    TREE_CACHE[key] = result
    return result


def independent_closure(profile):
    explicit = {item['path']: item['commit'] for item in profile['components']}
    if len(explicit) != len(profile['components']):
        raise ValueError('Duplicate explicit component: ' + profile['id'])
    queue = list(explicit.items())
    resolved = {}
    edges = []
    while queue:
        relative, commit = queue.pop(0)
        if relative in resolved:
            if resolved[relative]['commit'] != commit:
                raise ValueError('Conflicting closure pin: ' + relative)
            continue
        if len(resolved) >= 512:
            raise ValueError('Unbounded profile closure')
        selected = selected_tree(relative, commit)
        resolved[relative] = {name: selected[name] for name in ['path', 'commit', 'tree']}
        for name, entry in selected['entries'].items():
            if entry['mode'] != '160000':
                continue
            child = relative + '/' + name
            if child in explicit and explicit[child] != entry['oid']:
                raise ValueError('Explicit child disagrees with parent gitlink: ' + child)
            edges.append(dict(parent=relative, parent_commit=commit, parent_tree=selected['tree'],
                              gitlink_path=name, child=child, child_commit=entry['oid'],
                              explicit_child=child in explicit, pin_matches=True))
            queue.append((child, entry['oid']))
    components = sorted(resolved.values(), key=lambda x: (len(PurePosixPath(x['path']).parts), x['path']))
    for edge in edges:
        if resolved[edge['child']]['commit'] != edge['child_commit']:
            raise ValueError('Missing or mismatched transitive child')
    return components, edges


def archive_expectation(relative, commit):
    key = relative, commit
    if key in ARCHIVE_CACHE:
        return ARCHIVE_CACHE[key]
    selected = selected_tree(relative, commit)
    repo = component_repo(relative)
    # Git 2.34 has no check-attr --source. An isolated temporary index selects
    # this commit's .gitattributes without changing the component's own index.
    with tempfile.TemporaryDirectory(prefix='source-export-attrs-') as temp:
        env = dict(os.environ, GIT_INDEX_FILE=str(Path(temp) / 'index'))
        git(repo, 'read-tree', commit, env=env)
        names = sorted(selected['entries'])
        data = b''.join(os.fsencode(name) + b'\0' for name in names)
        raw = git(repo, 'check-attr', '--cached', '-z', '--stdin', 'export-ignore', 'export-subst', data=data, env=env).split(b'\0')
    attrs = {}
    for n in range(0, len(raw) - 1, 3):
        name, attribute, value = map(os.fsdecode, raw[n:n+3])
        attrs.setdefault(name, {})[attribute] = value
    source_entries = {name: item for name, item in selected['entries'].items() if item['kind'] == 'blob'}
    expected = {}
    substitutions = []
    proc = subprocess.Popen(['git', '-C', str(repo), 'archive', commit], stdout=subprocess.PIPE)
    try:
        with tarfile.open(fileobj=proc.stdout, mode='r|') as archive:
            for member in archive:
                name = member.name.rstrip('/')
                if member.isdir():
                    continue
                if not (member.isfile() or member.issym()):
                    raise ValueError('Unexpected archive member kind: ' + name)
                if name not in source_entries or name in expected:
                    raise ValueError('Archive ordinary path not uniquely in selected tree: ' + name)
                entry = source_entries[name]
                payload = os.fsencode(member.linkname) if member.issym() else archive.extractfile(member).read()
                blob = hashlib.sha1(b'blob ' + str(len(payload)).encode() + b'\0' + payload).hexdigest()
                transformed = blob != entry['oid']
                if transformed:
                    if attrs.get(name, {}).get('export-subst') != 'set' or member.issym():
                        raise ValueError('Unexplained archive/blob mismatch: ' + relative + '/' + name)
                    original = git(repo, 'cat-file', 'blob', entry['oid'])
                    substitutions.append(dict(path=name, git_blob=entry['oid'], original_sha256=digest(original),
                                              archive_sha256=digest(payload), reason='committed export-subst attribute'))
                if (entry['mode'] == '120000') != member.issym():
                    raise ValueError('Archive/tree kind mismatch: ' + name)
                if member.isfile() and bool(member.mode & 0o111) != (entry['mode'] == '100755'):
                    raise ValueError('Archive/tree executable mismatch: ' + name)
                expected[name] = dict(kind='symlink' if member.issym() else 'file', tree_mode=entry['mode'],
                                      git_blob=entry['oid'], sha256=digest(payload), size=len(payload),
                                      archive_mode=oct(member.mode), filtered_mode=member.mode & 0o755,
                                      executable_bits=member.mode & 0o111, archive_substituted=transformed,
                                      link_target=member.linkname if member.issym() else None)
    finally:
        proc.stdout.close()
        returncode = proc.wait()
    if returncode:
        raise RuntimeError('git archive failed: ' + relative)
    ignored = []
    for name in sorted(set(source_entries) - set(expected)):
        candidates = [name] + [str(parent) for parent in PurePosixPath(name).parents if str(parent) != '.']
        reasons = [candidate for candidate in candidates if attrs.get(candidate, {}).get('export-ignore') == 'set']
        if not reasons:
            raise ValueError('Selected blob omitted without export-ignore: ' + relative + '/' + name)
        ignored.append(dict(path=name, git_blob=source_entries[name]['oid'], export_ignore_at=reasons))
    attr_inputs = []
    info = Path(git(repo, 'rev-parse', '--git-path', 'info/attributes').decode().strip())
    if not info.is_absolute():
        info = repo / info
    if info.is_file():
        attr_inputs.append(dict(kind='local-info-attributes', path=str(info), sha256=digest(info.read_bytes()), size=info.stat().st_size))
    external = subprocess.run(['git', '-C', str(repo), 'config', '--get', 'core.attributesFile'], capture_output=True)
    if external.returncode == 0:
        attrpath = Path(external.stdout.decode().strip()).expanduser()
        if not attrpath.is_absolute():
            attrpath = repo / attrpath
        if attrpath.is_file():
            attr_inputs.append(dict(kind='configured-core-attributesFile', path=str(attrpath), sha256=digest(attrpath.read_bytes()), size=attrpath.stat().st_size))
    result = dict(expected=expected, tree_blob_count=len(source_entries), ignored=ignored,
                  export_substitutions=substitutions, external_attribute_inputs=attr_inputs)
    ARCHIVE_CACHE[key] = result
    return result


def all_export_entries(root):
    entries = {}
    for base, dirs, files in os.walk(root, followlinks=False):
        for name in list(dirs):
            path = Path(base) / name
            if path.is_symlink():
                entries[str(path.relative_to(root))] = path
                dirs.remove(name)
        for name in files:
            path = Path(base) / name
            entries[str(path.relative_to(root))] = path
    return entries


def verify_export(root, profiles, input_hashes):
    metadata_path = root / 'source-export.json'
    metadata = json.loads(metadata_path.read_text())
    components, edges = independent_closure(profiles[metadata['profile']])
    if metadata['components'] != components:
        raise ValueError('Export metadata differs from independent profile closure: ' + str(root))
    if metadata['registry_sha256'] != input_hashes['registry'] or metadata['export_script_sha256'] != input_hashes['exporter']:
        raise ValueError('Export metadata uses a different registry/script identity')
    if metadata.get('source_only') is not True or metadata.get('built_or_run') is not False:
        raise ValueError('Export metadata scope marker mismatch')
    actual = all_export_entries(root)
    checked = set()
    rows = []
    total = dict(regular_files=0, symlinks=0, executable_regular_files=0, payload_bytes=0, export_ignored_tree_blobs=0, export_substituted_files=0)
    for component in components:
        expectation = archive_expectation(component['path'], component['commit'])
        checks = []
        counts = dict(regular_files=0, symlinks=0, executable_regular_files=0, payload_bytes=0)
        for name, wanted in sorted(expectation['expected'].items()):
            relative = component['path'] + '/' + name
            if relative in checked or relative not in actual:
                raise ValueError('Missing/duplicate exported source entry: ' + relative)
            checked.add(relative)
            path = actual[relative]
            mode = path.lstat().st_mode
            if wanted['kind'] == 'symlink':
                if not stat.S_ISLNK(mode):
                    raise ValueError('Exported symlink changed kind: ' + relative)
                payload = os.fsencode(os.readlink(path))
                counts['symlinks'] += 1
            else:
                if not stat.S_ISREG(mode):
                    raise ValueError('Exported regular file changed kind: ' + relative)
                payload = path.read_bytes()
                if mode & 0o777 != wanted['filtered_mode']:
                    raise ValueError('Exported regular permissions differ from archive tar_filter: ' + relative)
                if mode & 0o111 != wanted['executable_bits']:
                    raise ValueError('Exported executable mode mismatch: ' + relative)
                counts['regular_files'] += 1
                counts['executable_regular_files'] += bool(mode & 0o111)
            if len(payload) != wanted['size'] or digest(payload) != wanted['sha256']:
                raise ValueError('Exported bytes/target mismatch: ' + relative)
            counts['payload_bytes'] += len(payload)
            checks.append(dict(path=relative, kind=wanted['kind'], tree_mode=wanted['tree_mode'],
                               git_blob=wanted['git_blob'], sha256=wanted['sha256'],
                               posix_mode=oct(mode & 0o777), archive_substituted=wanted['archive_substituted']))
        for key in counts:
            total[key] += counts[key]
        total['export_ignored_tree_blobs'] += len(expectation['ignored'])
        total['export_substituted_files'] += len(expectation['export_substitutions'])
        rows.append(dict(**component, **counts, selected_tree_blob_count=expectation['tree_blob_count'],
                         all_exported_bytes_symlinks_and_modes_match=True,
                         checked_entries_sha256=digest(canonical(checks)), export_ignored=expectation['ignored'],
                         export_substitutions=expectation['export_substitutions'],
                         external_attribute_inputs=expectation['external_attribute_inputs']))
    extras = sorted(set(actual) - checked - {'source-export.json'})
    if extras:
        raise ValueError('Unaccounted exported ordinary entries: ' + ', '.join(extras))
    return dict(directory=str(root), profile=metadata['profile'], metadata_sha256=digest(metadata_path.read_bytes()),
                components=len(components), gitlink_edges=len(edges), ordinary_entries_checked=len(checked),
                totals=total, component_checks=rows, missing_entries=[], extra_entries=[],
                metadata_matches_registry_and_exporter=True, status='passed')


def main():
    tracked_inputs = {'registry':REGISTRY, 'exporter':EXPORTER, 'platform_archive_stager':STAGER,
                      'verification_script':Path(__file__).resolve()}
    inputs = {name:digest(path.read_bytes()) for name,path in tracked_inputs.items()}
    registry = json.loads(REGISTRY.read_text())
    profiles = {item['id']:item for item in registry['profiles']}
    if len(profiles) != 5:
        raise ValueError('Expected five source profiles')
    closures = []
    for profile in profiles.values():
        components, edges = independent_closure(profile)
        actual = json.loads(subprocess.check_output([sys.executable,str(EXPORTER),'--profile',profile['id'],'--check-only']))
        if actual['components'] != components:
            raise ValueError('Exporter resolver differs from independent closure: ' + profile['id'])
        closures.append(dict(profile=profile['id'], explicit_components=profile['components'],
                             resolved_components=components, gitlink_edges=edges,
                             independent_closure_matches_exporter_check_only=True,
                             all_objects_exist_and_all_child_pins_match=True,
                             built_or_run=profile['built_or_run'], historical_result_transfer=profile['historical_result_transfer']))
    exports = [verify_export(PLATFORM/'out/source-organization-20261017'/name, profiles, inputs)
               for name in ['export-upstream','export-corrected']]
    if inputs != {name:digest(path.read_bytes()) for name,path in tracked_inputs.items()}:
        raise ValueError('Audit input changed during verification')
    report = dict(schema='source-export-verification-v1',timestamp=datetime.datetime.now(datetime.timezone.utc).isoformat(),
                  status='passed',scope='Read-only source/archive/profile audit; no build, tests, simulation or experiments.',
                  inputs_sha256=inputs,profiles_checked=len(closures),profile_closures=closures,exports=exports,
                  totals=dict(exports=2, ordinary_entries_checked=sum(x['ordinary_entries_checked'] for x in exports),
                              regular_files=sum(x['totals']['regular_files'] for x in exports),
                              symlinks=sum(x['totals']['symlinks'] for x in exports),
                              executable_regular_files=sum(x['totals']['executable_regular_files'] for x in exports),
                              gitlink_edges_checked_across_profiles=sum(len(x['gitlink_edges']) for x in closures)),
                  method=['Independent recursive ls-tree closure and explicit-parent/child pin validation for all five profiles.',
                          'Compared current exporter --check-only output to independently computed closure.',
                          'Selected-commit archive payloads checked against Git blob identities; export-ignore exclusions justified by committed/index attributes.',
                          'Every exported regular byte sequence, symlink target and executable/full regular permission mode checked; only source-export.json allowed as a non-Git ordinary file.',
                          'Temporary alternate indexes only; original repositories, component deliverables, exports, registry and exporter left untouched.'],
                  limitations=['Archive attributes intentionally omit export-ignore files and may expand export-subst; archive export is not a byte-complete Git checkout.',
                               'Regular permissions follow tar_filter(member.mode & 0o755); symlink execution bits are not Git identities. Directory permissions, ownership and timestamps are outside this audit.',
                               'Profile semantic_features are descriptive labels/deltas; recursive Git source closure is verified, not arbitrary feature-combination buildability.',
                               'Only the two existing upstream/corrected export directories are audited bytewise; the other three profiles receive closure/pin checks only.',
                               'No HDL compilation, runtime behavior, synthesis, timing, historical result transfer or performance has been verified.'])
    REPORT.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(status=report['status'],profiles=5,**report['totals']),indent=2))


if __name__ == '__main__':
    main()
