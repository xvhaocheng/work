#!/usr/bin/env python3
"""Add read-only provenance sidecars for the preserved invalidation campaign.

This script hashes and reads existing evidence and Git objects. It does not import
the old build/runner programs, build, execute a simulator, or edit original data.
"""
import csv
import hashlib
import json
import subprocess
from collections import Counter
from pathlib import Path

WORK = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
TASK = WORK / 'iommu-platform/out/invalidation-interference-20261017'
HASHES = {}


def relative(path):
    return str(Path(path).resolve().relative_to(WORK))


def digest(path):
    path = Path(path)
    if path not in HASHES:
        HASHES[path] = hashlib.sha256(path.read_bytes()).hexdigest()
    return HASHES[path]


def read(path):
    return json.loads(Path(path).read_text())


def ref(path, pointer=None, line=None):
    result = {'path': relative(path), 'sha256': digest(path)}
    if pointer is not None:
        result['json_pointer'] = pointer
    if line is not None:
        result['line'] = line
    return result


def at(name, pointer=None):
    return ref(TASK / name, pointer)


def git(repo, *args):
    return subprocess.check_output(['git', '-C', str(WORK / repo), *args])


def source_repository(logical):
    for prefix in ['cva6/corev_apu/riscv-iommu/',
                   'cva6/corev_apu/register_interface/',
                   'cva6/corev_apu/fpga/src/axi_slice/', 'cva6/']:
        if logical.startswith(prefix):
            return 'iommu-platform/' + prefix.rstrip('/'), logical[len(prefix):]
    raise ValueError(logical)


def main():
    before_path = TASK / 'production-before.json'
    before = read(before_path)
    after = read(TASK / 'production-after.json')
    assert after['before_sha256'] == digest(before_path)
    closure_path = TASK / 'frozen/source-manifest.json'
    closure = read(closure_path)
    builds = {}
    verification = {'method': 'static original-file hashes, JSON relationships and Git object reads only',
                    'simulator_runs_performed': 0, 'builds_performed': 0,
                    'frozen_files_verified': 0, 'frozen_git_blobs_verified': 0,
                    'build_input_hashes_verified': 0, 'build_include_hashes_verified': 0,
                    'runtime_artifact_hashes_verified': 0,
                    'historical_path_drift': [], 'gitlinks': []}

    # This also checks that the final closure is exported from the recorded
    # individual commits, rather than from the production checkout.
    for row in closure['files']:
        frozen = TASK / 'frozen' / row['logical_path']
        assert digest(frozen) == row['sha256'], frozen
        repo, name = source_repository(row['logical_path'])
        data = git(repo, 'show', row['commit'] + ':' + name)
        assert hashlib.sha256(data).hexdigest() == row['sha256'], name
        assert git(repo, 'rev-parse', row['commit'] + ':' + name).decode().strip() == row['git_blob']
        verification['frozen_files_verified'] += 1
        verification['frozen_git_blobs_verified'] += 1

    parent_commit = closure['cva6']
    for child, key in [('corev_apu/riscv-iommu', 'iommu'),
                       ('corev_apu/register_interface', 'register_interface'),
                       ('corev_apu/fpga/src/axi_slice', 'axi_slice')]:
        entry = git('iommu-platform/cva6', 'ls-tree', parent_commit, '--', child).decode().strip()
        assert entry.split()[0:3] == ['160000', 'commit', closure[key]], entry
        verification['gitlinks'].append({'repository': 'iommu-platform/cva6',
                                         'commit': parent_commit, 'git_path': child,
                                         'mode': '160000', 'child_commit': closure[key]})

    checkout_repositories = []
    for i, row in enumerate(before['repositories']):
        actual = after['repositories'][i]['actual']
        assert all(actual[key] == row[key] for key in ['path', 'head', 'branch', 'status', 'index_sha256'])
        assert after['repositories'][i]['differences'] == []
        checkout_repositories.append({
            'repository': row['path'], 'checkout_branch': row['branch'],
            'checkout_commit': row['head'], 'dirty': bool(row['status']),
            'status': row['status'], 'index_sha256': row['index_sha256'],
            'captured_utc': before['captured_utc'],
            'role': ('read-only source comparison, not simulation execution dependency'
                     if row['path'].endswith('/linux') else
                     'not a simulation execution dependency' if row['path'].endswith('/opensbi') else
                     'production checkout; not the exported compile-source identity'),
            'field_evidence': {key: [ref(before_path, f'/repositories/{i}/{original}')]
                               for key, original in [('repository', 'path'), ('checkout_branch', 'branch'),
                                                     ('checkout_commit', 'head'), ('dirty', 'status'),
                                                     ('index_sha256', 'index_sha256')]},
            'unchanged_endpoint_evidence': at('production-after.json', f'/repositories/{i}')})
    checkout_repositories.insert(0, {
        'repository': '.', 'checkout_branch': None, 'checkout_commit': None, 'dirty': None,
        'role': 'parent work repository; no contemporaneous checkout recorded in this experiment',
        'known_unknowns': ['production-before records platform and four children, not the work repository. '
                           'A later work HEAD/branch cannot establish its execution-time identity.'],
        'absence_evidence': ref(before_path, '/repositories')})
    exported = []
    checkout_map = {row['repository']: row for row in checkout_repositories}
    for key, repo in [('cva6', 'iommu-platform/cva6'),
                      ('iommu', 'iommu-platform/cva6/corev_apu/riscv-iommu'),
                      ('register_interface', 'iommu-platform/cva6/corev_apu/register_interface'),
                      ('axi_slice', 'iommu-platform/cva6/corev_apu/fpga/src/axi_slice')]:
        snapshot = checkout_map.get(repo)
        exported.append({
            'repository': repo, 'selected_ref': closure[key], 'selected_commit': closure[key],
            'selection_method': 'Git object export by literal full commit SHA; not a branch checkout',
            'source_class': ('platform adaptation plus necessary functional fixes; no partition/bypass/DS-first-F4'
                             if key in ['cva6', 'iommu'] else 'unchanged dependency pinned by the selected CVA6 gitlink'),
            'checkout_branch': snapshot['checkout_branch'] if snapshot else None,
            'checkout_commit': snapshot['checkout_commit'] if snapshot else None,
            'dirty': snapshot['dirty'] if snapshot else None,
            'known_unknowns': ([] if snapshot else ['No contemporaneous checkout branch/HEAD/status captured '
                                                    'for this vendor repository; the export commit is known.']),
            'field_evidence': {
                'selected_ref': [ref(closure_path, '/' + key),
                                 at('runner/build03-builder.py.snapshot', None)],
                'selected_commit': [ref(closure_path, '/' + key)],
                'source_class': ([at('sources/baseline/manifest.json', '/components')]
                                 if key in ['cva6', 'iommu'] else
                                 [verification['gitlinks'][1 if key == 'register_interface' else 2]]),
                'per_file_sha256_and_git_blob': [ref(closure_path, '/files')]}})
    context = {
        'production_snapshots': {'before': ref(before_path), 'after': at('production-after.json')},
        'checkout_repositories': checkout_repositories, 'selected_source_repositories': exported,
        'fixture_repository': {
            'repository': 'iommu-platform', 'selected_ref': None, 'selected_commit': None,
            'source_class': 'isolated handwritten test fixture with per-build snapshots',
            'known_unknowns': ['The uncommitted runner/top/stub are fixed by build snapshot hashes; '
                               'the production platform commit is not their source commit.'],
            'field_evidence': {'snapshots': [at('build/build03/build-manifest.json', '/handwritten_inputs')]}},
        'capture_limit': 'Before/after snapshots agree; there is no per-case instantaneous checkout capture. '
                         'Source selection is independently fixed by the frozen closure and build manifest.'}

    for path in sorted((TASK / 'build').glob('*/build-manifest.json')):
        name = path.parent.name
        raw = read(path)
        item = {
            'build_id': name, 'manifest': ref(path), 'evidence_layer': 'lint' if raw['lint_only'] else 'build',
            'state': raw['state'], 'exit_code': raw['exit_code'], 'lint_only': raw['lint_only'],
            'started_unix': raw['started_unix'], 'ended_unix': raw['ended_unix'],
            'source_count': len(raw['sources']), 'closure_file_count': len(raw['includes']),
            'closure_roles': dict(Counter(x['role'] for x in raw['includes'])),
            'baseline_refs': raw['baseline_refs'], 'parameters': raw['parameters'],
            'sources': ref(path, '/sources'), 'includes': ref(path, '/includes'),
            'argv': ref(path, '/argv'), 'versions': ref(path, '/versions'),
            'environment': ref(path, '/environment'),
            'field_evidence': {key: [ref(path, '/' + key)] for key in
                               ['state', 'exit_code', 'lint_only', 'baseline_refs', 'parameters',
                                'started_unix', 'ended_unix']},
            'handwritten_inputs': [], 'known_unknowns': []}
        if 'frozen_source_manifest' in raw:
            assert raw['frozen_source_manifest']['sha256'] == digest(closure_path)
            item['frozen_source_manifest'] = ref(closure_path)
            item['field_evidence']['frozen_source_manifest'] = [ref(path, '/frozen_source_manifest')]
        else:
            item['known_unknowns'].append('No final frozen-source-manifest hash captured for this early lint; '
                                          'its own includes list records its historical closure.')
        # Do not mistake the later final runner files for earlier compiler inputs.
        for row in raw.get('handwritten_inputs', []):
            snap = Path(row['snapshot'])
            assert digest(snap) == row['sha256'], snap
            item['handwritten_inputs'].append({**row, 'snapshot': relative(snap),
                                               'original': relative(row['original']),
                                               'evidence': ref(path, '/handwritten_inputs')})
        for index, row in enumerate(raw['sources']):
            source = Path(row['path'])
            assert row['path'] in raw['argv'], source
            if digest(source) == row['sha256']:
                verification['build_input_hashes_verified'] += 1
            else:
                alternatives = []
                for other in sorted((TASK / 'build').glob('build*/inputs/' + source.name)):
                    if digest(other) == row['sha256']:
                        alternatives.append(ref(other))
                drift = {'build_id': name, 'original_path': relative(source),
                         'recorded_sha256': row['sha256'], 'current_sha256': digest(source),
                         'evidence': ref(path, f'/sources/{index}'), 'retained_matching_snapshots': alternatives}
                verification['historical_path_drift'].append(drift)
                item['known_unknowns'].append('Original mutable source path drifted: ' + relative(source) +
                                               ('; matching later snapshot retained.' if alternatives else
                                                '; no matching handwritten snapshot found in retained build inputs.'))
        for row in raw['includes']:
            assert digest(TASK / 'frozen' / row['logical_path']) == row['sha256']
            verification['build_include_hashes_verified'] += 1
        if raw.get('cpp'):
            cpp = raw['cpp']
            assert digest(cpp['path']) == cpp['sha256']
            assert cpp['path'] in raw['argv']
            item['cpp_snapshot'] = {'path': relative(cpp['path']), 'sha256': cpp['sha256'],
                                    'evidence': ref(path, '/cpp')}
        if raw.get('executable'):
            exe = raw['executable']
            assert digest(exe['path']) == exe['sha256']
            item['binary'] = {'path': relative(exe['path']), 'sha256': exe['sha256'],
                              'evidence': ref(path, '/executable'),
                              'retention': 'local generated artifact; hash retained even if binary is excluded from source archive'}
        if name == 'build03':
            snap = TASK / 'runner/build03-builder.py.snapshot'
            assert digest(snap) == raw['build_driver']['sha256']
            item['build_driver_snapshot'] = ref(snap)
            item['field_evidence']['build_driver_snapshot'] = [ref(path, '/build_driver')]
        elif 'build_driver' in raw:
            assert digest(raw['build_driver']['path']) == raw['build_driver']['sha256']
            item['build_driver_snapshot'] = ref(raw['build_driver']['path'])
        else:
            item['known_unknowns'].append('Exact historical build-driver bytes were not captured in this manifest; '
                                          'do not substitute the final runner/build.py.')
        if 'driver_argv' in raw:
            item['driver_argv'] = raw['driver_argv']
            item['field_evidence']['driver_argv'] = [ref(path, '/driver_argv')]
        else:
            item['known_unknowns'].append('Wrapper driver invocation was not recorded; compiler argv is recorded.')
        if 'source_mode' in raw:
            item['source_mode'] = raw['source_mode']
            item['field_evidence']['source_mode'] = [ref(path, '/source_mode')]
        builds[name] = item

    records, preparation, campaigns = [], [], {}
    for name in ['matrix01', 'sensitivity01', 'smoke01', 'smoke02']:
        path = TASK / 'runs' / name / 'campaign.json'
        raw = read(path)
        build_name = Path(raw['build_manifest']).parent.name
        build = builds[build_name]
        assert raw['build_manifest_sha256'] == build['manifest']['sha256']
        assert raw['binary_sha256'] == build['binary']['sha256']
        assert raw['runner_sha256'] == build['cpp_snapshot']['sha256']
        assert digest(TASK / 'runner/execute.py') == raw['execute_sha256']
        campaigns[name] = {'campaign_id': name, 'manifest': ref(path), 'stage': raw['stage'],
                           'build_id': build_name, 'case_count': len(raw['cases']),
                           'complete': raw['complete'], 'ddr': raw['ddr'],
                           'native_cpu_time_measured': raw['native_cpu_time_measured'],
                           'binary': ref(path, '/binary'), 'binary_sha256': raw['binary_sha256'],
                           'build_link_evidence': [ref(path, '/build_manifest'), ref(path, '/build_manifest_sha256')],
                           'cpp_link_evidence': ref(path, '/runner_sha256'),
                           'execute_sha256': raw['execute_sha256'], 'execute': ref(TASK / 'runner/execute.py'),
                           'field_evidence': {key: [ref(path, '/' + key)] for key in
                                              ['stage', 'complete', 'ddr', 'native_cpu_time_measured']}}
        for i, case in enumerate(raw['cases']):
            base = f'/cases/{i}'
            assert case['command'][0] == raw['binary']
            assert case['command'][case['command'].index('--case-id') + 1] == case['config']['case_id']
            for flag, key in [('--domain', 'domain'), ('--b-pages', 'b_pages'),
                              ('--mode', 'mode'), ('--map-policy', 'map_policy')]:
                assert case['command'][case['command'].index(flag) + 1] == str(case['config'][key])
            summary_path = Path(case['output']) / 'summary.json'
            assert digest(summary_path) == case['summary_sha256']
            summary = read(summary_path)
            assert summary['config']['case_id'] == case['config']['case_id']
            assert all(summary['config'][key] == value for key, value in case['config'].items())
            for artifact_name, identity in case['artifacts'].items():
                artifact = Path(case['output']) / artifact_name
                assert digest(artifact) == identity['sha256'], artifact
                assert artifact.stat().st_size == identity['bytes'], artifact
                verification['runtime_artifact_hashes_verified'] += 1
            measurement = name in ['matrix01', 'sensitivity01']
            assert ('--smoke' in case['command']) == (not measurement)
            assert summary['config']['smoke'] == (not measurement)
            item = {
                'experiment_id': 'invalidation-interference-20261017/' + name + '/' + case['config']['case_id'],
                'campaign_id': name, 'case_id': case['config']['case_id'],
                'evidence_layer': 'RTL simulation with behavioral DDR',
                'execution_status': 'passed' if case['passed'] else 'failed',
                'is_measurement': measurement,
                'repositories': {'context_id': 'production-plus-frozen-export'},
                'build_inputs': {'build_id': build_name, 'source_closure_id': 'fixed-export-closure',
                                 'binary_sha256': raw['binary_sha256'],
                                 'evidence': [ref(path, '/binary_sha256'), ref(path, '/build_manifest_sha256'),
                                              ref(path, base + '/command/0')]},
                'field_evidence': {'case_id': [ref(path, base + '/config/case_id')],
                                   'execution_status': [ref(path, base + '/passed'), ref(path, base + '/returncode')],
                                   'is_measurement': [ref(path, '/stage'), ref(summary_path, '/config/smoke')],
                                   'evidence_layer': [ref(summary_path, '/limitations')]},
                'run_identity': {
                    'config': case['config'], 'argv': case['command'],
                    'started_utc': case['started_utc'], 'finished_utc': case['finished_utc'],
                    'returncode': case['returncode'], 'passed': case['passed'], 'failure': case['failure'],
                    'summary': ref(summary_path), 'observed_config': summary['config'],
                    'artifact_hashes': ref(path, base + '/artifacts'),
                    'field_evidence': {key: [ref(path, base + '/' + original)] for key, original in
                                       [('config', 'config'), ('argv', 'command'), ('started_utc', 'started_utc'),
                                        ('finished_utc', 'finished_utc'), ('returncode', 'returncode'),
                                        ('passed', 'passed'), ('failure', 'failure')]},
                    'observed_config_evidence': ref(summary_path, '/config')},
                'known_unknowns': ['No per-case host environment capture; build environment is recorded. '
                                   'This is not native Linux/CPU/FPGA timing.']}
            if measurement:
                assert case['passed'] and case['returncode'] == 0 and summary['checks']['passed']
                records.append(item)
            else:
                item['preparation_kind'] = 'smoke simulation'
                preparation.append(item)

    for name, build in builds.items():
        linked = [key for key, value in campaigns.items() if value['build_id'] == name]
        item = {
            'experiment_id': 'invalidation-interference-20261017/' + name,
            'preparation_kind': 'lint' if build['lint_only'] else 'compilation',
            'evidence_layer': build['evidence_layer'], 'execution_status': build['state'],
            'is_measurement': False, 'repositories': {'context_id': 'production-plus-frozen-export'},
            'build_inputs': {'build_id': name}, 'linked_campaigns': linked,
            'known_unknowns': build['known_unknowns'], 'evidence': build['manifest']}
        item['repositories']['selected_source_evidence'] = ref(TASK / 'build' / name / 'build-manifest.json', '/baseline_refs')
        item['repositories']['selection_scope'] = ('Only this build baseline_refs and source list define its selected '
                                                    'components; the shared context also describes the final export.')
        if name in ['lint01', 'lint02']:
            log = TASK / 'build' / name / 'build.log'
            lines = log.read_text().splitlines()
            item['failure_evidence'] = [ref(log, line=i + 1) for i, value in enumerate(lines) if '%Error' in value]
        if name == 'build04':
            item['execution_status'] = 'passed_build_only_no_simulation'
            item['no_run_evidence'] = [ref(TASK / 'runner/README.md', line=12)]
            item['scope_note'] = ('Later frozen-only rebuild; different output hash; no campaign uses it. '
                                  'The 28 measured cases remain tied to build03.')
        elif not linked and not build['lint_only']:
            item['scope_note'] = 'No runtime campaign for this binary appears in the four preserved campaign manifests.'
        preparation.append(item)

    linux_path = TASK / 'sources/linux/manifest.json'
    linux = read(linux_path)
    comparisons = []
    for i, version in enumerate(linux['versions']):
        local = version['id'] == 'C-platform'
        comparisons.append({
            'comparison_id': version['id'], 'evidence_layer': 'read-only source review',
            'execution_status': 'source_checked_not_built_or_run', 'is_measurement': False,
            'repository': version['repository'], 'selected_ref': version['commit'],
            'queried_ref': version['branch'],
            'selected_commit': version['commit'],
            'checkout_branch': version['branch'] if local else None,
            'checkout_commit': version['commit'] if local else None,
            'runtime_dependency': False,
            'known_unknowns': ([] if local else ['Remote ref query and fixed file downloads; no local checkout was performed.']),
            'source_files': ref(linux_path, f'/versions/{i}/files'),
            'field_evidence': {key: [ref(linux_path, f'/versions/{i}/{original}')] for key, original in
                               [('repository', 'repository'), ('selected_ref', 'commit'), ('selected_commit', 'commit'),
                                ('queried_ref', 'branch')]},
            'scope_evidence': ref(linux_path, '/scope')})

    assert len(records) == 28 and Counter(x['campaign_id'] for x in records) == {'matrix01': 24, 'sensitivity01': 4}
    assert len(preparation) == 9
    archive_path = TASK / 'publication/receipt.json'
    archive = read(archive_path)
    assert archive['platform_before'] == before['repositories'][0]['head']
    output = {
        'schema': 'historical-invalidation-identities-v1',
        'scope': 'Historical identity backfill from preserved bytes; no experiment rerun or retrospective branch assignment.',
        'provenance': {'ars_workflow': 'experiment-agent/validate',
                              'verification_status': 'ANALYZED',
                              'scope': 'provenance only; no statistical reanalysis or reproducibility rerun'},
        'evidence_policy': {
            'path_base': 'work repository root',
            'file_reference': 'path + full SHA256 + optional RFC6901 json_pointer or one-based line',
            'field_evidence': 'Original field values use field_evidence, inherited context/build/campaign ids resolve here.',
            'selected_ref': 'Literal SHA for hardware Git export; source-profile names/branch aliases are not execution identities.',
            'unknown': 'null plus a reason; no inference from current branch tips.',
            'original_bytes': 'All original manifests, logs, snapshots and README files remain unchanged.'},
        'repository_contexts': {'production-plus-frozen-export': context},
        'source_closures': {'fixed-export-closure': {
            'manifest': ref(closure_path), 'files': ref(closure_path, '/files'),
            'source_order': ref(closure_path, '/source_order'), 'include_roots': ref(closure_path, '/include_roots'),
            'roles': dict(Counter(x['role'] for x in closure['files'])),
            'selection_evidence': [ref(TASK / 'runner/build03-builder.py.snapshot', line=12),
                                   ref(TASK / 'runner/build03-builder.py.snapshot', line=95)],
            'gitlinks': verification['gitlinks'],
            'scope_note': 'Final 94-source +10-include +10-license closure; earlier lints use their own includes lists.'}},
        'builds': builds, 'campaigns': campaigns, 'records': records,
        'preparation_entries': preparation, 'read_only_source_comparisons': comparisons,
        'archive': {
            'platform_results_commit': archive['platform_results_commit'],
            'platform_results_tree': archive['platform_results_tree'],
            'platform_archive_ref': archive['ref'],
            'work_execution_commit': None, 'work_archive_commit': None,
            'note': 'Publication is later result archival, not the hardware source commit. '
                    'This receipt does not capture the public work repository commit.',
            'field_evidence': {key: [ref(archive_path, '/' + key)] for key in
                               ['platform_results_commit', 'platform_results_tree']},
            'receipt': ref(archive_path)},
        'known_unknowns': [
            'No contemporaneous work repository branch/commit/dirty snapshot.',
            'Vendor register_interface/axi_slice selected commits are known, their checkout branches/HEAD/status are not captured.',
            'Production endpoint captures are not per-case checkout captures; exported frozen bytes establish actual compiled RTL.',
            'Early lint/build wrapper-driver versions are unrecorded; lint01 mutable handwritten top has no matching retained build snapshot.'],
        'verification': verification,
        'generator': ref(Path(__file__).resolve())}
    destination = HERE / 'invalidation-identities.json'
    destination.write_text(json.dumps(output, ensure_ascii=False, indent=2) + '\n')
    csv_path = HERE / 'invalidation-identities.csv'
    fields = ['experiment_id', 'is_measurement', 'evidence_layer', 'execution_status', 'campaign_id',
              'build_id', 'checkout_branch_work', 'checkout_commit_work',
              'checkout_branch_platform', 'checkout_commit_platform', 'checkout_branch_cva6',
              'checkout_commit_cva6', 'selected_ref_cva6', 'selected_commit_cva6', 'checkout_branch_iommu',
              'checkout_commit_iommu', 'selected_ref_iommu', 'selected_commit_iommu',
              'checkout_branch_register_interface', 'selected_commit_register_interface',
              'checkout_branch_axi_slice', 'selected_commit_axi_slice', 'source_manifest',
              'source_manifest_sha256', 'binary_sha256', 'case_evidence', 'unknowns']
    with csv_path.open('w', newline='') as stream:
        writer = csv.DictWriter(stream, fields, lineterminator="\n")
        writer.writeheader()
        for row in records + preparation:
            build = builds[row['build_inputs']['build_id']]
            platform = checkout_map['iommu-platform']
            cva6 = checkout_map['iommu-platform/cva6']
            iommu = checkout_map['iommu-platform/cva6/corev_apu/riscv-iommu']
            evidence = row.get('run_identity', {}).get('summary', row.get('evidence', {}))
            writer.writerow({
                'experiment_id': row['experiment_id'], 'is_measurement': row['is_measurement'],
                'evidence_layer': row['evidence_layer'], 'execution_status': row['execution_status'],
                'campaign_id': row.get('campaign_id', ''), 'build_id': build['build_id'],
                'checkout_branch_work': 'null', 'checkout_commit_work': 'null',
                'checkout_branch_platform': platform['checkout_branch'], 'checkout_commit_platform': platform['checkout_commit'],
                'checkout_branch_cva6': cva6['checkout_branch'], 'checkout_commit_cva6': cva6['checkout_commit'],
                'selected_ref_cva6': build['baseline_refs']['cva6'], 'selected_commit_cva6': build['baseline_refs']['cva6'],
                'checkout_branch_iommu': iommu['checkout_branch'], 'checkout_commit_iommu': iommu['checkout_commit'],
                'selected_ref_iommu': build['baseline_refs']['iommu'], 'selected_commit_iommu': build['baseline_refs']['iommu'],
                'checkout_branch_register_interface': 'null',
                'selected_commit_register_interface': build['baseline_refs']['register_interface'],
                'checkout_branch_axi_slice': 'null',
                'selected_commit_axi_slice': build['baseline_refs'].get('axi_slice', 'null'),
                'source_manifest': relative(closure_path) if 'frozen_source_manifest' in build else build['includes']['path'] + '#/includes',
                'source_manifest_sha256': build.get('frozen_source_manifest', build['manifest'])['sha256'],
                'binary_sha256': build.get('binary', {}).get('sha256', ''),
                'case_evidence': evidence.get('path', '') + ('#' + evidence['json_pointer'] if 'json_pointer' in evidence else ''),
                'unknowns': ' | '.join(output['known_unknowns'][:3] + row['known_unknowns'])})
    print(json.dumps({'records': len(records), 'preparation_entries': len(preparation),
                      'linux_read_only': len(comparisons), 'verification': verification,
                      'files': [relative(destination), relative(csv_path)]}, ensure_ascii=False, indent=2))


if __name__ == '__main__':
    main()
