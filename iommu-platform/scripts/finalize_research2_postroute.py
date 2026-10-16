#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''finalize_research2_postroute.py -- finalize the role 2 post-route optimization.

Reads the fixed input.json and execution.json in the optimization directory,
validates every input path and SHA256, the base method manifest, the actual
source lock, the Tcl, the tool execution exit code, and the optimized outputs.
Parses the optimized timing report with the package_research2 timing helpers
and requires every routed-timing gate to pass.  Only then writes the additive
manifest.json in the same optimization directory.  It never rewrites the base
manifest, never overwrites base artifacts and never claims board validation.

The script is host-side only, uses the Python standard library plus the
selected scripts directory package_research2 module, and fails closed.
'''
from __future__ import annotations

import argparse
import json
import pathlib
import sys
from typing import Any, Dict, Optional, Sequence

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import package_research2 as pkg

POSTROUTE_DIR_REL = 'out/two-bit-20261003/method-direct-arm-postroute'
POSTROUTE_MANIFEST_REL = POSTROUTE_DIR_REL + '/manifest.json'
POSTROUTE_INPUT_REL = POSTROUTE_DIR_REL + '/input.json'
POSTROUTE_EXECUTION_REL = POSTROUTE_DIR_REL + '/execution.json'
POSTROUTE_SCHEMA = 'research2-postroute-optimization-v1'
POSTROUTE_ACTION = 'postroute-optimization'
EXPECTED_METHOD_PLATFORM_COMMIT = pkg.EXPECTED_METHOD_PLATFORM_COMMIT
EXPECTED_METHOD_CVA6_COMMIT = pkg.EXPECTED_METHOD_CVA6_COMMIT
EXPECTED_METHOD_LOCK_SHA256 = pkg.EXPECTED_METHOD_LOCK_SHA256
EXPECTED_SESSION_ID = 46536
EXPECTED_ROLE = 2

INPUT_KEYS = ('base_manifest', 'input_checkpoint', 'tcl', 'base_timing')
OUTPUT_KEYS = ('optimized_dcp', 'bit', 'mcs', 'utilization', 'timing')


def _require_dict(data, label, rel):
    if not isinstance(data, dict):
        raise pkg.PackageError('%s (%s): expected a JSON object' % (label, rel))
    return data


def _require_sha(item, key, label, rel):
    value = item.get('sha256')
    if not isinstance(value, str) or not pkg.HEX64_RE.fullmatch(value):
        raise pkg.PackageError('%s (%s): %s.sha256 is missing or not a sha256' % (label, rel, key))
    return value.lower()


def _require_bytes(item, key, label, rel):
    value = item.get('bytes')
    if not isinstance(value, int) or isinstance(value, bool) or value < 0:
        raise pkg.PackageError('%s (%s): %s.bytes is missing or not a non-negative integer' % (label, rel, key))
    return value


def load_input(repo, optimization_dir):
    input_path = optimization_dir / 'input.json'
    if not input_path.is_file():
        raise pkg.PackageError('post-route input is missing: %s' % input_path)
    data = _require_dict(pkg.read_json(input_path, 'post-route input'),
                         'post-route input', str(input_path))
    entries = {}
    for key in INPUT_KEYS:
        item = data.get(key)
        if not isinstance(item, dict):
            raise pkg.PackageError('%s: %s is missing or not an object' % (input_path, key))
        rel = item.get('path')
        if not isinstance(rel, str) or not rel:
            raise pkg.PackageError('%s: %s.path is missing' % (input_path, key))
        declared_sha = _require_sha(item, key, 'post-route input', str(input_path))
        declared_bytes = _require_bytes(item, key, 'post-route input', str(input_path))
        path = pkg.require_file(repo, rel, 'post-route input %s' % key)
        actual_sha = pkg.sha256_file(path)
        if actual_sha != declared_sha:
            raise pkg.PackageError('%s: %s sha256 mismatch: %s != %s' %
                                   (input_path, key, actual_sha, declared_sha))
        actual_bytes = path.stat().st_size
        if actual_bytes != declared_bytes:
            raise pkg.PackageError('%s: %s bytes mismatch: %s != %s' %
                                   (input_path, key, actual_bytes, declared_bytes))
        entries[key] = {'path': rel, 'resolved': path,
                        'sha256': actual_sha, 'bytes': actual_bytes}
    return {'input_path': input_path, 'data': data, 'entries': entries}


def load_execution(optimization_dir, expected_session_id=EXPECTED_SESSION_ID):
    execution_path = optimization_dir / 'execution.json'
    if not execution_path.is_file():
        raise pkg.PackageError('post-route execution.json is missing: %s' % execution_path)
    data = _require_dict(pkg.read_json(execution_path, 'post-route execution'),
                         'post-route execution', str(execution_path))
    session_id = data.get('session_id')
    if session_id != expected_session_id:
        raise pkg.PackageError('%s: session_id is %r, want %s' %
                               (execution_path, session_id, expected_session_id))
    exit_code = data.get('exit_code')
    if not isinstance(exit_code, int) or isinstance(exit_code, bool):
        raise pkg.PackageError('%s: exit_code is missing or not an integer' % execution_path)
    if exit_code != 0:
        raise pkg.PackageError('%s: exit_code is %s, want 0' % (execution_path, exit_code))
    return {'execution_path': execution_path, 'data': data,
            'session_id': session_id, 'exit_code': exit_code}


def load_base_method_manifest(repo, input_data, expected_platform_commit=EXPECTED_METHOD_PLATFORM_COMMIT):
    entry = input_data['entries']['base_manifest']
    rel = entry['path']
    path = entry['resolved']
    data = _require_dict(pkg.read_json(path, 'post-route base method manifest'),
                         'post-route base method manifest', rel)
    if data.get('status') != 'passed':
        raise pkg.PackageError('%s: base method manifest status is %r, want passed' %
                               (rel, data.get('status')))
    if data.get('platform_commit') != expected_platform_commit:
        raise pkg.PackageError('%s: base method manifest platform_commit is %r, want %s' %
                               (rel, data.get('platform_commit'), expected_platform_commit))
    profile = data.get('profile')
    if not isinstance(profile, dict) or profile.get('research_role') != EXPECTED_ROLE:
        raise pkg.PackageError('%s: base method manifest profile.research_role must be %s' %
                               (rel, EXPECTED_ROLE))
    lock_sha = data.get('sources_lock_sha256')
    if not isinstance(lock_sha, str) or not pkg.HEX64_RE.fullmatch(lock_sha):
        raise pkg.PackageError('%s: base method manifest sources_lock_sha256 is missing or invalid' % rel)
    return {'path': rel, 'resolved': path, 'sha256': entry['sha256'],
            'bytes': entry['bytes'], 'data': data, 'profile': profile,
            'sources_lock_sha256': lock_sha.lower()}


def validate_method_source_lock(repo, base_manifest,
                                expected_lock_sha256=EXPECTED_METHOD_LOCK_SHA256,
                                expected_cva6_commit=EXPECTED_METHOD_CVA6_COMMIT):
    rel = pkg.SOURCES_LOCK_REL
    path = pkg.resolve_rel(repo, rel)
    if not path.is_file():
        raise pkg.PackageError('method source lock is missing: %s' % rel)
    actual_sha = pkg.sha256_file(path)
    if actual_sha != expected_lock_sha256:
        raise pkg.PackageError('method source lock %s hash is %s, want %s' %
                               (rel, actual_sha, expected_lock_sha256))
    if base_manifest['sources_lock_sha256'] != actual_sha:
        raise pkg.PackageError('base method manifest sources_lock_sha256 %s does not match actual source lock %s' %
                               (base_manifest['sources_lock_sha256'], actual_sha))
    lock = _require_dict(pkg.read_json(path, 'method source lock'),
                         'method source lock', rel)
    components = lock.get('components')
    if not isinstance(components, list):
        raise pkg.PackageError('method source lock must have a components list')
    cva6 = None
    for component in components:
        if isinstance(component, dict) and component.get('path') == pkg.CVA6_LOCK_PATH:
            cva6 = component
            break
    if cva6 is None:
        raise pkg.PackageError('method source lock does not contain %s' % pkg.CVA6_LOCK_PATH)
    if cva6.get('commit') != expected_cva6_commit:
        raise pkg.PackageError('method source lock cva6 commit is %r, want %s' %
                               (cva6.get('commit'), expected_cva6_commit))
    return {'path': rel, 'sha256': actual_sha, 'cva6_commit': cva6.get('commit')}


def parse_base_timing(input_data):
    entry = input_data['entries']['base_timing']
    path = entry['resolved']
    text = path.read_text(encoding='utf-8', errors='replace')
    summary = pkg.parse_design_timing_summary(text, 'base method routed timing', entry['path'])
    summary['loops'] = pkg.parse_combinational_loops(text, 'base method routed timing', entry['path'])
    return {'path': entry['path'], 'sha256': entry['sha256'], 'bytes': entry['bytes'],
            'design_timing_summary': summary, 'accepted': False,
            'note': ('original base method timing is negative and is preserved as '
                     'provenance only; it does not block delivery when the '
                     'authenticated post-route optimization passes all gates')}


def require_outputs(repo, optimization_dir):
    repo_resolved = repo.resolve()
    result = {}
    for key, filename in (('optimized_dcp', 'optimized.dcp'),
                          ('bit', 'ariane_xilinx.bit'),
                          ('mcs', 'ariane_xilinx.mcs'),
                          ('utilization', 'utilization.rpt'),
                          ('timing', 'timing.rpt')):
        path = optimization_dir / filename
        if not path.is_file():
            raise pkg.PackageError('post-route output is missing: %s' % path)
        if path.stat().st_size <= 0:
            raise pkg.PackageError('post-route output is empty: %s' % path)
        resolved = path.resolve()
        if not pkg.is_relative_to(resolved, repo_resolved):
            raise pkg.PackageError('post-route output escapes the repository: %s' % path)
        rel = resolved.relative_to(repo_resolved).as_posix()
        result[key] = {'path': rel, 'resolved': resolved,
                       'sha256': pkg.sha256_file(resolved),
                       'bytes': resolved.stat().st_size}
    return result


def parse_optimized_timing(outputs):
    entry = outputs['timing']
    text = entry['resolved'].read_text(encoding='utf-8', errors='replace')
    summary = pkg.parse_design_timing_summary(text, 'role 2 post-route optimized timing', entry['path'])
    summary['loops'] = pkg.parse_combinational_loops(text, 'role 2 post-route optimized timing', entry['path'])
    pkg.validate_routed_timing(summary, 'role 2 post-route optimized timing', entry['path'])
    return {'path': entry['path'], 'sha256': entry['sha256'], 'bytes': entry['bytes'],
            'design_timing_summary': summary, 'accepted': True}


def finalize(repo_root,
             optimization_dir_rel=POSTROUTE_DIR_REL,
             expected_session_id=EXPECTED_SESSION_ID,
             expected_platform_commit=EXPECTED_METHOD_PLATFORM_COMMIT,
             expected_lock_sha256=EXPECTED_METHOD_LOCK_SHA256,
             expected_cva6_commit=EXPECTED_METHOD_CVA6_COMMIT):
    repo = pathlib.Path(repo_root).resolve()
    if not repo.is_dir():
        raise pkg.PackageError('repo root is not a directory: %s' % repo)
    optimization_dir = pkg.resolve_rel(repo, optimization_dir_rel)
    if not optimization_dir.is_dir():
        raise pkg.PackageError('post-route optimization directory is missing: %s' % optimization_dir_rel)
    manifest_path = optimization_dir / 'manifest.json'
    if manifest_path.exists():
        raise pkg.PackageError('post-route manifest already exists, refusing to overwrite: %s' % manifest_path)
    input_data = load_input(repo, optimization_dir)
    execution = load_execution(optimization_dir, expected_session_id=expected_session_id)
    base_manifest = load_base_method_manifest(
        repo, input_data, expected_platform_commit=expected_platform_commit)
    lock = validate_method_source_lock(
        repo, base_manifest,
        expected_lock_sha256=expected_lock_sha256,
        expected_cva6_commit=expected_cva6_commit)
    tcl_entry = input_data['entries']['tcl']
    base_timing = parse_base_timing(input_data)
    outputs = require_outputs(repo, optimization_dir)
    optimized_timing = parse_optimized_timing(outputs)
    manifest = {
        'schema': POSTROUTE_SCHEMA,
        'action': POSTROUTE_ACTION,
        'status': 'passed',
        'generated_at': pkg.utc_now(),
        'optimization_dir': optimization_dir_rel,
        'tool': {
            'name': 'vivado',
            'session_id': execution['session_id'],
            'exit_code': execution['exit_code'],
            'execution_path': POSTROUTE_EXECUTION_REL,
            'execution_sha256': pkg.sha256_file(execution['execution_path']),
        },
        'input': {
            'path': POSTROUTE_INPUT_REL,
            'sha256': pkg.sha256_file(input_data['input_path']),
            'bytes': input_data['input_path'].stat().st_size,
        },
        'base': {
            'method_manifest': {
                'path': base_manifest['path'],
                'sha256': base_manifest['sha256'],
                'bytes': base_manifest['bytes'],
            },
            'platform_commit': base_manifest['data']['platform_commit'],
            'profile': base_manifest['profile'],
            'sources_lock': {
                'path': lock['path'],
                'sha256': lock['sha256'],
                'cva6_commit': lock['cva6_commit'],
            },
            'checkpoint': {
                'path': input_data['entries']['input_checkpoint']['path'],
                'sha256': input_data['entries']['input_checkpoint']['sha256'],
                'bytes': input_data['entries']['input_checkpoint']['bytes'],
            },
            'base_timing': base_timing,
        },
        'tcl': {
            'path': tcl_entry['path'],
            'sha256': tcl_entry['sha256'],
            'bytes': tcl_entry['bytes'],
        },
        'outputs': {
            'optimized_dcp': {
                'path': outputs['optimized_dcp']['path'],
                'sha256': outputs['optimized_dcp']['sha256'],
                'bytes': outputs['optimized_dcp']['bytes'],
            },
            'bit': {
                'path': outputs['bit']['path'],
                'sha256': outputs['bit']['sha256'],
                'bytes': outputs['bit']['bytes'],
            },
            'mcs': {
                'path': outputs['mcs']['path'],
                'sha256': outputs['mcs']['sha256'],
                'bytes': outputs['mcs']['bytes'],
            },
            'utilization': {
                'path': outputs['utilization']['path'],
                'sha256': outputs['utilization']['sha256'],
                'bytes': outputs['utilization']['bytes'],
            },
            'timing': optimized_timing,
        },
        'note': ('host-side post-route optimization only; no board validation is '
                 'performed or claimed; the base method timing is negative and is '
                 'preserved as provenance only; the optimized timing must pass every '
                 'routed-timing gate before this manifest is written; no automatic '
                 're-run or auto-completion is performed.'),
    }
    pkg.write_json(manifest_path, manifest)
    return {
        'ok': True,
        'manifest': str(manifest_path),
        'manifest_sha256': pkg.sha256_file(manifest_path),
        'session_id': execution['session_id'],
        'exit_code': execution['exit_code'],
        'optimized_timing': optimized_timing['design_timing_summary'],
    }


def build_parser():
    parser = argparse.ArgumentParser(
        prog='finalize_research2_postroute.py',
        description='Finalize the role 2 post-route optimization manifest (host-side only).')
    parser.add_argument('--repo-root', default=str(pathlib.Path(__file__).resolve().parents[1]),
                        help='repository root (default: the parent of scripts/)')
    parser.add_argument('--optimization-dir', default=POSTROUTE_DIR_REL,
                        help='optimization directory relative to the repository root')
    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = build_parser().parse_args(argv)
    try:
        summary = finalize(args.repo_root, optimization_dir_rel=args.optimization_dir)
    except pkg.PackageError as exc:
        sys.stderr.write('finalize_research2_postroute: error: %s%s' % (exc, pkg.NL))
        return 2
    sys.stdout.write(json.dumps(summary, indent=2, ensure_ascii=False, sort_keys=True) + pkg.NL)
    return 0


if __name__ == '__main__':
    sys.exit(main())
