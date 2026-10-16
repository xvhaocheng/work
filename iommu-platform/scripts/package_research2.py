#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
package_research2.py -- assemble the two-BIT research2 delivery tree.

Host-side packaging helper for the shared-Linux two-BIT research2 run.  It is
standalone (Python 3 standard library only), read-only on the repository and
fails closed: every value written into DELIVERY.json or acceptance.json is read
from a real manifest, a real artifact or a freshly regenerated plan, so no
manifest is fabricated and no build log is inspected.

Steps
-----
1. Requires out/manifests/software-migration-4dma-research-e2-v2.json and both
   out/manifests/fpga-<profile>.json with status == passed.
2. Requires three exact role builds:
   - software: platform 481bd47fd9d62de212ed7eb4619a0554c05bc12e, saved lock
     out/two-bit-20261003/software-sources.lock.json, cva6
     6f968ec980e663046b89383b00d3dc85ce3e176f;
   - E2 role1: platform 10fd189ee705c2b14983c513c295e381fc997e2d, saved lock
     out/two-bit-20261003/e2-sources.lock.json, cva6
     a0497a8dac660f8feb19d0f486d4862777276ed3;
   - method role2: platform 7430772309dbed9453b505eb97f43c2a3f54f141, current
     lock sources.lock.json, cva6
     a1b08934391833aed21e6ad63b545634040808ac.
   Verifies that the three locks are identical except for the cva6 commit and
   that each cva6 commit is the exact expected value.  An old method manifest
   at platform 10fd189ee705c2b14983c513c295e381fc997e2d is refused.
3. Requires the two FPGA manifests to declare roles 1 and 2 and verifies every
   artifact each manifest lists (existence and sha256, relative paths).
4. Requires the fixed inputs: both profiles ariane_xilinx.bit and
   ariane_xilinx.mcs (non-empty; the .mcs files stay on the build host), the
   OpenSBI payload, the board CLI and kernel.release ==
   6.5.0-rc2-00093-gc0f5e13ea81f.
5. Regenerates the frozen plans with the real build_plan (64 e2 cases, 462
   method cases) and writes them byte-exactly; no calibration selection is
   applied here, that belongs to the board phase.
6. Copies the delivery members into <out>/delivery, writes DELIVERY.json with
   schema research2-delivery-v1 (shaped for scripts/mac_research2.py) and a
   complete SHA256SUMS.
7. Builds <out>/research-two-bit-20261003.tar.gz from the delivery members only
   and re-opens it to prove the members are regular files with the expected
   paths and the original hashes.
8. Writes <out>/acceptance.json outside the delivery tree with the actual hashes
   and counts and an explicit no-board-validation statement.

Only one shell command is ever run: git -C <repo> rev-parse HEAD.
'''
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import math
import pathlib
import re
import shutil
import subprocess
import sys
import tarfile
import time
from typing import Any, Dict, List, Optional, Sequence, Tuple


REPO_ROOT_DEFAULT = pathlib.Path(__file__).resolve().parents[1]

SOFTWARE_MANIFEST_REL = 'out/manifests/software-migration-4dma-research-e2-v2.json'
FPGA_MANIFEST_REL_FMT = 'out/manifests/fpga-{profile}.json'

ROLE_PROFILE = {
    1: 'migration-4dma-research-e2-v2',
    2: 'migration-4dma-research-method-v2',
}

EXPECTED_SOFTWARE_PLATFORM_COMMIT = '481bd47fd9d62de212ed7eb4619a0554c05bc12e'
EXPECTED_E2_PLATFORM_COMMIT = '10fd189ee705c2b14983c513c295e381fc997e2d'
EXPECTED_METHOD_PLATFORM_COMMIT = '7430772309dbed9453b505eb97f43c2a3f54f141'
EXPECTED_KERNEL_RELEASE = '6.5.0-rc2-00093-gc0f5e13ea81f'
SOURCES_LOCK_REL = 'sources.lock.json'
SOFTWARE_SOURCES_LOCK_REL = 'out/two-bit-20261003/software-sources.lock.json'
E2_SOURCES_LOCK_REL = 'out/two-bit-20261003/e2-sources.lock.json'
EXPECTED_SOFTWARE_LOCK_SHA256 = 'b9519530a4f5df872c82a4c994c496cd9667736c47668bfc26d9e09ffc0d6f0d'
EXPECTED_E2_LOCK_SHA256 = '1c87639502d4b2b9296e0dd7e3dee0771d6a8e4c8cec68d05e68a082bea3ae55'
EXPECTED_SOFTWARE_CVA6_COMMIT = '6f968ec980e663046b89383b00d3dc85ce3e176f'
EXPECTED_E2_CVA6_COMMIT = 'a0497a8dac660f8feb19d0f486d4862777276ed3'
EXPECTED_METHOD_CVA6_COMMIT = 'a1b08934391833aed21e6ad63b545634040808ac'
CVA6_LOCK_PATH = 'cva6'
POSTROUTE_DIR_REL = 'out/two-bit-20261003/method-direct-arm-postroute'
POSTROUTE_MANIFEST_REL = POSTROUTE_DIR_REL + '/manifest.json'
POSTROUTE_INPUT_REL = POSTROUTE_DIR_REL + '/input.json'
POSTROUTE_EXECUTION_REL = POSTROUTE_DIR_REL + '/execution.json'
POSTROUTE_SCHEMA = 'research2-postroute-optimization-v1'
POSTROUTE_ACTION = 'postroute-optimization'
EXPECTED_POSTROUTE_BASE_MANIFEST_SHA256 = '66d0549eca04754cf0c6c61f5c1765e43e5d86785d16bae9c09086cb1dc9ea30'
EXPECTED_POSTROUTE_CHECKPOINT_SHA256 = 'd6f425b03ba2a211858645ca15c56daf2467625667acc1b717a92534868d0e67'
EXPECTED_POSTROUTE_TCL_SHA256 = 'd98726671f6a788998d4e93a639325009dd78bc83a2460307970c3e5cb230d9f'
EXPECTED_POSTROUTE_BASE_TIMING_SHA256 = '305fde2d4bb746c053d41f36f6f840769b15fadaf847f9a781d035d6a4655f1f'
EXPECTED_POSTROUTE_SESSION_ID = 46536
EXPECTED_METHOD_LOCK_SHA256 = 'bdc7c776bba2434964b62fc5535db8af29ca440e791a0618fa0aaabc6873694a'

POSTROUTE_OUTPUT_FILENAMES = {
    'optimized_dcp': 'optimized.dcp',
    'bit': 'ariane_xilinx.bit',
    'mcs': 'ariane_xilinx.mcs',
    'utilization': 'utilization.rpt',
    'timing': 'timing.rpt',
}

EXPECTED_CVA6_COMMITS = {
    'software': EXPECTED_SOFTWARE_CVA6_COMMIT,
    'e2': EXPECTED_E2_CVA6_COMMIT,
    'method': EXPECTED_METHOD_CVA6_COMMIT,
}
COMPATIBILITY_NOTE = ('the three source locks differ only in the cva6 commit: software original '
                      '6f968ec980e663046b89383b00d3dc85ce3e176f; E2 role1 '
                      '10fd189ee705c2b14983c513c295e381fc997e2d / '
                      'a0497a8dac660f8feb19d0f486d4862777276ed3, a Vivado input net declaration '
                      'fix; method role2 7430772309dbed9453b505eb97f43c2a3f54f141 / '
                      'a1b08934391833aed21e6ad63b545634040808ac, which changes the ARM command '
                      'decode to pulse the research arm directly only in the accepted ARM command '
                      'branch, zero added latency and no behavior change; normal billing rules are '
                      'unchanged.  The earlier method builds at platform '
                      '10fd189ee705c2b14983c513c295e381fc997e2d and '
                      '9a172d2a24842b316e6a7975a24fe69848ff44fe remain historic rejected/negative '
                      'timing evidence and are not used for delivery.  E2 is role1 and excludes the '
                      'policy block, so its BIT and manifest are reused unchanged')
EXPECTED_SOFTWARE_ARTIFACTS = {
    'out/rootfs.cpio': '312eb8e11665a5221d26fb3f93f9079592b01815bc638092a8a6bb49ea42ea14',
    'out/linux/arch/riscv/boot/Image': '43962488f887ba44fcf953cad75a1e10cf64811fe7481aa4cb2454e73bafdd6c',
    'out/linux/platform.dtb': '0439c6c944dbb83af0ee3c3f637c0e952d8a75bfedda9cbc3e06c31fe7542f4a',
    'out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin': 'af9f703b6d00420598282a983201361983b17ed731bc245284b67689dcd9e748',
}

PAYLOAD_REL = 'out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin'
CLI_REL = 'out/bench/idma_research'
KERNEL_RELEASE_REL = 'out/linux/include/config/kernel.release'
BIT_REL_FMT = 'out/{profile}/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit'
MCS_REL_FMT = 'out/{profile}/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.mcs'
TIMING_REPORT_REL_FMT = 'out/{profile}/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt'
TIMING_REPORT_DELIVERY_REL_FMT = 'provenance/timing/role{role}.timing_summary_routed.rpt'

BOARD_CLI_PATH = '/usr/bin/idma_research'

DELIVERY_SCHEMA = 'research2-delivery-v1'
DELIVERY_DIRNAME = 'delivery'
ARCHIVE_FILENAME = 'research-two-bit-20261003.tar.gz'
ACCEPTANCE_FILENAME = 'acceptance.json'
ACCEPTANCE_SCHEMA = 'research2-package-acceptance-v1'
SHA256SUMS_FILENAME = 'SHA256SUMS'

PLAN_MODULE_REL = 'scripts/research2_plan.py'
PLAN_COUNTS = (('e2', 64), ('method', 462))

REQUIRED_SCRIPTS = (
    'scripts/mac_research2.py',
    'scripts/research2_plan.py',
    'scripts/research2_results.py',
    'scripts/research_results.py',
)
REQUIRED_DOCS = (
    'docs/research-two-bit-v2.md',
    'docs/mac-research2.md',
    'docs/research-two-bit-mac-20261003.md',
)
OPTIONAL_DOCS = (
    'docs/two-bit-build-acceptance-20261003.md',
)
PROVENANCE_CONFIGS = (
    'configs/migration-4dma-research-e2-v2.json',
    'configs/migration-4dma-research-method-v2.json',
)

NL = chr(10)
HEX64_RE = re.compile(r'[0-9a-fA-F]{64}')
GIT_SHA_RE = re.compile(r'[0-9a-f]{40}')


class PackageError(Exception):
    '''A condition that must stop the packaging run.'''


# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------

def utc_now() -> str:
    return time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: pathlib.Path) -> str:
    digest = hashlib.sha256()
    with open(str(path), 'rb') as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b''):
            digest.update(chunk)
    return digest.hexdigest()


def is_relative_to(path: pathlib.Path, root: pathlib.Path) -> bool:
    try:
        path.relative_to(root)
        return True
    except ValueError:
        return False


def safe_rel(rel: str) -> pathlib.PurePosixPath:
    if not isinstance(rel, str) or not rel:
        raise PackageError('empty manifest path')
    if chr(0) in rel or chr(92) in rel:
        raise PackageError('rejected manifest path %r' % (rel,))
    pure = pathlib.PurePosixPath(rel)
    if pure.is_absolute():
        raise PackageError('absolute manifest path rejected: %s' % rel)
    parts = pure.parts
    if not parts:
        raise PackageError('empty manifest path')
    for part in parts:
        if part in ('', '.', '..'):
            raise PackageError('manifest path escape rejected: %s' % rel)
    return pure


def resolve_rel(root: pathlib.Path, rel: str) -> pathlib.Path:
    root = pathlib.Path(root).resolve()
    pure = safe_rel(rel)
    candidate = root.joinpath(*pure.parts)
    resolved = candidate.resolve()
    if not is_relative_to(resolved, root):
        raise PackageError('manifest path escapes the repository: %s' % rel)
    return resolved


def require_file(root: pathlib.Path, rel: str, label: str) -> pathlib.Path:
    path = resolve_rel(root, rel)
    if not path.is_file():
        raise PackageError('%s is missing: %s' % (label, rel))
    if path.stat().st_size <= 0:
        raise PackageError('%s is empty: %s' % (label, rel))
    return path


# ---------------------------------------------------------------------------
# Routed timing acceptance
# ---------------------------------------------------------------------------

TIMING_REQUIRED_NONNEGATIVE = ('wns', 'whs', 'wpws')
TIMING_REQUIRED_ZERO_FLOAT = ('tns', 'ths', 'tpws')
TIMING_REQUIRED_ZERO_INT = ('tns_fail', 'ths_fail', 'tpws_fail', 'loops')


def _timing_float(token, name, label, rel):
    text = str(token).strip()
    if not text:
        raise PackageError('%s (%s): empty %s value' % (label, rel, name))
    if text.lower() in ('inf', '-inf', 'nan', 'n/a', 'na', '--', '-', 'x'):
        raise PackageError('%s (%s): non-finite %s value %r' % (label, rel, name, token))
    try:
        value = float(text)
    except ValueError:
        raise PackageError('%s (%s): %s value %r is not numeric' % (label, rel, name, token))
    if not math.isfinite(value):
        raise PackageError('%s (%s): non-finite %s value %r' % (label, rel, name, token))
    return value


def _timing_int(token, name, label, rel):
    text = str(token).strip()
    if not text:
        raise PackageError('%s (%s): empty %s value' % (label, rel, name))
    if not re.fullmatch(r'-?\d+', text):
        raise PackageError('%s (%s): %s value %r is not an integer' % (label, rel, name, token))
    return int(text)


def parse_design_timing_summary(text, label, rel):
    lines = text.splitlines()
    start = None
    for index, line in enumerate(lines):
        if 'design timing summary' in line.lower():
            start = index
            break
    if start is None:
        raise PackageError('%s (%s): no Design Timing Summary section' % (label, rel))

    header_index = None
    for index in range(start + 1, min(len(lines), start + 60)):
        compact = ' '.join(lines[index].split())
        if all(key in compact for key in ('WNS', 'TNS', 'TNS Failing Endpoints',
                                          'WHS', 'THS', 'THS Failing Endpoints',
                                          'WPWS', 'TPWS', 'TPWS Failing Endpoints')):
            header_index = index
            break
    if header_index is None:
        raise PackageError('%s (%s): no Design Timing Summary header with the expected columns' %
                           (label, rel))

    numeric_re = re.compile(
        r'^\s*(-?\d+(?:\.\d+)?)\s+(-?\d+(?:\.\d+)?)\s+(\d+)\s+(\d+)\s+'
        r'(-?\d+(?:\.\d+)?)\s+(-?\d+(?:\.\d+)?)\s+(\d+)\s+(\d+)\s+'
        r'(-?\d+(?:\.\d+)?)\s+(-?\d+(?:\.\d+)?)\s+(\d+)\s+(\d+)\s*$')
    match = None
    for index in range(header_index + 1, len(lines)):
        candidate = lines[index]
        stripped = candidate.strip()
        if not stripped:
            continue
        if all(ch in '-= ' for ch in stripped):
            continue
        match = numeric_re.match(candidate)
        if match is None:
            raise PackageError('%s (%s): malformed Design Timing Summary row after the header: %r' %
                               (label, rel, candidate))
        break
    if match is None:
        raise PackageError('%s (%s): no numeric Design Timing Summary row after the header' %
                           (label, rel))

    groups = match.groups()
    return {
        'wns': _timing_float(groups[0], 'WNS', label, rel),
        'tns': _timing_float(groups[1], 'TNS', label, rel),
        'tns_fail': _timing_int(groups[2], 'TNS Failing Endpoints', label, rel),
        'whs': _timing_float(groups[4], 'WHS', label, rel),
        'ths': _timing_float(groups[5], 'THS', label, rel),
        'ths_fail': _timing_int(groups[6], 'THS Failing Endpoints', label, rel),
        'wpws': _timing_float(groups[8], 'WPWS', label, rel),
        'tpws': _timing_float(groups[9], 'TPWS', label, rel),
        'tpws_fail': _timing_int(groups[10], 'TPWS Failing Endpoints', label, rel),
    }


def parse_combinational_loops(text, label, rel):
    matches = re.findall(r'checking\s+loops\s*\(\s*(\d+)\s*\)', text,
                         re.IGNORECASE)
    if not matches:
        raise PackageError('%s (%s): no explicit combinational loops count found' %
                           (label, rel))
    counts = sorted({int(value) for value in matches})
    if len(counts) != 1:
        raise PackageError('%s (%s): contradictory combinational loops counts: %s' %
                           (label, rel, counts))
    return counts[0]


def validate_routed_timing(summary, label, rel):
    for key in TIMING_REQUIRED_NONNEGATIVE:
        value = summary.get(key)
        if not isinstance(value, float) or not math.isfinite(value):
            raise PackageError('%s (%s): %s is missing or non-finite: %r' % (label, rel, key, value))
        if value < 0:
            raise PackageError('%s (%s): %s is negative: %s' % (label, rel, key, value))
    for key in TIMING_REQUIRED_ZERO_FLOAT:
        value = summary.get(key)
        if not isinstance(value, float) or not math.isfinite(value):
            raise PackageError('%s (%s): %s is missing or non-finite: %r' % (label, rel, key, value))
        if value != 0:
            raise PackageError('%s (%s): %s is not zero: %s' % (label, rel, key, value))
    for key in TIMING_REQUIRED_ZERO_INT:
        value = summary.get(key)
        if not isinstance(value, int) or isinstance(value, bool):
            raise PackageError('%s (%s): %s is missing or not an integer: %r' % (label, rel, key, value))
        if value != 0:
            raise PackageError('%s (%s): %s is not zero: %s' % (label, rel, key, value))


def load_routed_timing_acceptance(repo, profile, label):
    rel = TIMING_REPORT_REL_FMT.format(profile=profile)
    path = require_file(repo, rel, '%s routed timing report' % label)
    text = path.read_text(encoding='utf-8', errors='replace')
    summary = parse_design_timing_summary(text, label, rel)
    summary['loops'] = parse_combinational_loops(text, label, rel)
    validate_routed_timing(summary, label, rel)
    return {
        'source_path': rel,
        'sha256': sha256_file(path),
        'design_timing_summary': summary,
    }


def delivery_path(delivery: pathlib.Path, rel: str) -> pathlib.Path:
    return delivery.joinpath(*pathlib.PurePosixPath(rel).parts)


def read_json(path: pathlib.Path, label: str) -> Any:
    try:
        with open(str(path), 'r', encoding='utf-8') as handle:
            return json.load(handle)
    except OSError as exc:
        raise PackageError('%s: cannot read %s: %s' % (label, path, exc))
    except ValueError as exc:
        raise PackageError('%s: invalid JSON in %s: %s' % (label, path, exc))


def write_json(path: pathlib.Path, obj: Any) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    text = json.dumps(obj, indent=2, ensure_ascii=False, sort_keys=True) + NL
    path.write_bytes(text.encode('utf-8'))


def import_module_from_path(path: pathlib.Path, name: str):
    spec = importlib.util.spec_from_file_location(name, str(path))
    if spec is None or spec.loader is None:
        raise PackageError('cannot build an import spec for %s' % path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    try:
        spec.loader.exec_module(module)
    except PackageError:
        raise
    except Exception as exc:
        raise PackageError('cannot import %s: %r' % (path, exc))
    return module


def git_head(repo: pathlib.Path) -> str:
    try:
        proc = subprocess.run(
            ['git', '-C', str(repo), 'rev-parse', 'HEAD'],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
    except OSError as exc:
        raise PackageError('cannot run git rev-parse: %r' % (exc,))
    if proc.returncode != 0:
        raise PackageError('git rev-parse HEAD failed: %s' %
                           proc.stderr.decode('utf-8', 'replace').strip())
    text = proc.stdout.decode('utf-8', 'replace').strip()
    if not GIT_SHA_RE.fullmatch(text):
        raise PackageError('git rev-parse HEAD returned %r' % (text,))
    return text


# ---------------------------------------------------------------------------
# Manifest handling
# ---------------------------------------------------------------------------

def validate_manifest(data: Any, label: str, rel: str,
                      expected_role: Optional[int],
                      expected_platform_commit: str) -> int:
    if not isinstance(data, dict):
        raise PackageError('%s (%s): manifest is not a JSON object' % (label, rel))
    status = data.get('status')
    if status != 'passed':
        raise PackageError('%s (%s): status is %r, want passed' % (label, rel, status))
    commit = data.get('platform_commit')
    if commit != expected_platform_commit:
        raise PackageError('%s (%s): platform_commit is %r, want %s' %
                           (label, rel, commit, expected_platform_commit))
    lock = data.get('sources_lock_sha256')
    if not isinstance(lock, str) or not HEX64_RE.fullmatch(lock):
        raise PackageError('%s (%s): sources_lock_sha256 is missing or not a sha256' %
                           (label, rel))
    profile = data.get('profile')
    if not isinstance(profile, dict):
        raise PackageError('%s (%s): profile must be an object' % (label, rel))
    role = profile.get('research_role')
    if role not in (1, 2):
        raise PackageError('%s (%s): profile.research_role must be 1 or 2, got %r' %
                           (label, rel, role))
    if expected_role is not None and role != expected_role:
        raise PackageError('%s (%s): profile.research_role is %s, want %s' %
                           (label, rel, role, expected_role))
    return int(role)


def load_manifest(repo: pathlib.Path, rel: str, label: str,
                  expected_role: Optional[int] = None,
                  expected_platform_commit: Optional[str] = None) -> Dict[str, Any]:
    path = resolve_rel(repo, rel)
    if not path.is_file():
        raise PackageError('%s is missing: %s' % (label, rel))
    data = read_json(path, label)
    if expected_platform_commit is None:
        raise PackageError('%s (%s): missing expected platform commit' % (label, rel))
    role = validate_manifest(data, label, rel, expected_role,
                             expected_platform_commit)
    return {'label': label, 'rel': rel, 'path': path, 'data': data, 'role': role,
            'sha256': sha256_file(path)}


def validate_lock_triple(lock_paths: Dict[str, pathlib.Path],
                         expected_cva6: Dict[str, str]) -> Dict[str, Any]:
    labels = ['software', 'e2', 'method']
    locks: Dict[str, Any] = {}
    for label in labels:
        data = read_json(lock_paths[label], '%s sources lock' % label)
        if not isinstance(data, dict):
            raise PackageError('%s sources lock must be a JSON object' % label)
        locks[label] = data

    reference_label = labels[0]
    reference_top = dict(locks[reference_label])
    reference_components = reference_top.pop('components', None)
    if not isinstance(reference_components, list):
        raise PackageError('sources locks must have a components list')

    for label in labels[1:]:
        top = dict(locks[label])
        components = top.pop('components', None)
        if top != reference_top:
            raise PackageError('%s sources lock differs outside components' % label)
        if not isinstance(components, list):
            raise PackageError('%s sources lock must have a components list' % label)
        if len(components) != len(reference_components):
            raise PackageError('sources locks have different component counts')

    seen = set()
    for index, reference_component in enumerate(reference_components):
        if not isinstance(reference_component, dict):
            raise PackageError('sources lock component is not an object')
        path = reference_component.get('path')
        if not isinstance(path, str) or not path:
            raise PackageError('sources lock component has no path')
        if path in seen:
            raise PackageError('sources lock has a duplicate component: %s' % path)
        seen.add(path)
        components = [reference_component]
        for label in labels[1:]:
            component = locks[label]['components'][index]
            if not isinstance(component, dict):
                raise PackageError('sources lock component is not an object')
            if component.get('path') != path:
                raise PackageError('sources lock component order differs at %s' % path)
            components.append(component)
        keys = set()
        for component in components:
            keys.update(component.keys())
        for key in sorted(keys):
            if key == 'commit' and path == CVA6_LOCK_PATH:
                continue
            first = components[0].get(key)
            for component in components[1:]:
                if component.get(key) != first:
                    raise PackageError('sources lock component %s differs in %s' %
                                       (path, key))
        if path == CVA6_LOCK_PATH:
            for label, component in zip(labels, components):
                actual = component.get('commit')
                expected = expected_cva6[label]
                if actual != expected:
                    raise PackageError('%s lock cva6 commit is %r, want %s' %
                                       (label, actual, expected))
    if CVA6_LOCK_PATH not in seen:
        raise PackageError('sources locks do not contain %s' % CVA6_LOCK_PATH)
    return locks


def verify_manifest_artifacts(repo: pathlib.Path, entry: Dict[str, Any],
                              verified: Dict[str, str]) -> None:
    listed = entry['data'].get('artifacts')
    if not isinstance(listed, dict) or not listed:
        raise PackageError('%s (%s): artifact map is missing or empty' %
                           (entry['label'], entry['rel']))
    for rel in sorted(listed):
        expected = listed[rel]
        if not isinstance(rel, str) or not rel:
            raise PackageError('%s (%s): invalid artifact path %r' %
                               (entry['label'], entry['rel'], rel))
        if not isinstance(expected, str) or not HEX64_RE.fullmatch(expected):
            raise PackageError('%s (%s): artifact %s has no sha256' %
                               (entry['label'], entry['rel'], rel))
        path = resolve_rel(repo, rel)
        if not path.is_file():
            raise PackageError('%s (%s): artifact is missing: %s' %
                               (entry['label'], entry['rel'], rel))
        actual = sha256_file(path)
        if actual != expected.lower():
            raise PackageError('%s (%s): artifact %s sha256 mismatch: %s != %s' %
                               (entry['label'], entry['rel'], rel, actual,
                                expected.lower()))
        verified[rel] = actual


def _postroute_item(item, key, rel):
    if not isinstance(item, dict):
        raise PackageError('%s: %s is missing or not an object' % (rel, key))
    path = item.get('path')
    if not isinstance(path, str) or not path:
        raise PackageError('%s: %s.path is missing' % (rel, key))
    sha = item.get('sha256')
    if not isinstance(sha, str) or not HEX64_RE.fullmatch(sha):
        raise PackageError('%s: %s.sha256 is missing or invalid' % (rel, key))
    size = item.get('bytes')
    if not isinstance(size, int) or isinstance(size, bool) or size < 0:
        raise PackageError('%s: %s.bytes is missing or invalid' % (rel, key))
    return path, sha.lower(), size


def _verify_actual_file(repo, rel_path, declared_sha, declared_bytes, label):
    path = require_file(repo, rel_path, label)
    actual_sha = sha256_file(path)
    actual_bytes = path.stat().st_size
    if actual_sha != declared_sha:
        raise PackageError('%s (%s): sha256 mismatch: %s != %s' % (label, rel_path, actual_sha, declared_sha))
    if actual_bytes != declared_bytes:
        raise PackageError('%s (%s): bytes mismatch: %s != %s' % (label, rel_path, actual_bytes, declared_bytes))
    return path, actual_sha, actual_bytes


def postroute_output_record(item):
    return {
        'source_path': item['source_path'],
        'sha256': item['sha256'],
        'bytes': item['bytes'],
    }

def load_postroute_manifest(repo: pathlib.Path) -> Dict[str, Any]:
    rel = POSTROUTE_MANIFEST_REL
    path = resolve_rel(repo, rel)
    if not path.is_file():
        raise PackageError('role 2 post-route optimization manifest is missing: %s' % rel)
    data = read_json(path, 'role 2 post-route optimization manifest')
    if not isinstance(data, dict):
        raise PackageError('%s: post-route manifest is not a JSON object' % rel)
    if data.get('schema') != POSTROUTE_SCHEMA:
        raise PackageError('%s: schema is %r, want %s' % (rel, data.get('schema'), POSTROUTE_SCHEMA))
    if data.get('action') != POSTROUTE_ACTION:
        raise PackageError('%s: action is %r, want %s' % (rel, data.get('action'), POSTROUTE_ACTION))
    if data.get('status') != 'passed':
        raise PackageError('%s: status is %r, want passed' % (rel, data.get('status')))

    base = data.get('base')
    if not isinstance(base, dict):
        raise PackageError('%s: base provenance is missing' % rel)

    method_manifest = base.get('method_manifest')
    mm_path, mm_sha, mm_bytes = _postroute_item(method_manifest, 'base.method_manifest', rel)
    expected_mm_rel = FPGA_MANIFEST_REL_FMT.format(profile=ROLE_PROFILE[2])
    if mm_path != expected_mm_rel:
        raise PackageError('%s: base.method_manifest.path is %r, want %s' % (rel, mm_path, expected_mm_rel))
    if mm_sha != EXPECTED_POSTROUTE_BASE_MANIFEST_SHA256:
        raise PackageError('%s: base.method_manifest.sha256 is %r, want %s' % (rel, mm_sha, EXPECTED_POSTROUTE_BASE_MANIFEST_SHA256))
    mm_file, actual_mm_sha, actual_mm_bytes = _verify_actual_file(
        repo, mm_path, mm_sha, mm_bytes, 'base method manifest')
    mm_data = read_json(mm_file, 'base method manifest')
    if not isinstance(mm_data, dict):
        raise PackageError('%s: base method manifest is not a JSON object' % mm_path)
    if mm_data.get('status') != 'passed':
        raise PackageError('%s: base method manifest status is %r, want passed' % (mm_path, mm_data.get('status')))
    if mm_data.get('platform_commit') != EXPECTED_METHOD_PLATFORM_COMMIT:
        raise PackageError('%s: base method manifest platform_commit is %r, want %s' % (mm_path, mm_data.get('platform_commit'), EXPECTED_METHOD_PLATFORM_COMMIT))
    mm_profile = mm_data.get('profile')
    if not isinstance(mm_profile, dict) or mm_profile.get('research_role') != 2:
        raise PackageError('%s: base method manifest profile.research_role must be 2' % mm_path)
    mm_lock_sha = mm_data.get('sources_lock_sha256')
    if not isinstance(mm_lock_sha, str) or not HEX64_RE.fullmatch(mm_lock_sha):
        raise PackageError('%s: base method manifest sources_lock_sha256 is missing or invalid' % mm_path)
    mm_lock_sha = mm_lock_sha.lower()

    lock = base.get('sources_lock')
    if not isinstance(lock, dict):
        raise PackageError('%s: base.sources_lock is missing' % rel)
    lock_path = lock.get('path')
    if not isinstance(lock_path, str) or lock_path != SOURCES_LOCK_REL:
        raise PackageError('%s: base.sources_lock.path is %r, want %s' % (rel, lock_path, SOURCES_LOCK_REL))
    lock_sha_decl = lock.get('sha256')
    if not isinstance(lock_sha_decl, str) or not HEX64_RE.fullmatch(lock_sha_decl):
        raise PackageError('%s: base.sources_lock.sha256 is missing or invalid' % rel)
    lock_sha_decl = lock_sha_decl.lower()
    if lock_sha_decl != EXPECTED_METHOD_LOCK_SHA256:
        raise PackageError('%s: base.sources_lock.sha256 is %r, want %s' % (rel, lock_sha_decl, EXPECTED_METHOD_LOCK_SHA256))
    lock_file = require_file(repo, lock_path, 'method source lock')
    actual_lock_sha = sha256_file(lock_file)
    if actual_lock_sha != lock_sha_decl:
        raise PackageError('%s: base.sources_lock.sha256 is %s, actual %s is %s' % (rel, lock_sha_decl, lock_path, actual_lock_sha))
    if mm_lock_sha != actual_lock_sha:
        raise PackageError('%s: base method manifest sources_lock_sha256 %s does not match actual source lock %s' % (mm_path, mm_lock_sha, actual_lock_sha))
    lock_data = read_json(lock_file, 'method source lock')
    if not isinstance(lock_data, dict):
        raise PackageError('%s: method source lock is not a JSON object' % lock_path)
    components = lock_data.get('components')
    if not isinstance(components, list):
        raise PackageError('%s: method source lock must have a components list' % lock_path)
    cva6 = None
    for component in components:
        if isinstance(component, dict) and component.get('path') == CVA6_LOCK_PATH:
            cva6 = component
            break
    if cva6 is None:
        raise PackageError('%s: method source lock does not contain %s' % (lock_path, CVA6_LOCK_PATH))
    if cva6.get('commit') != lock.get('cva6_commit'):
        raise PackageError('%s: base.sources_lock.cva6_commit is %r, actual %s is %r' % (rel, lock.get('cva6_commit'), lock_path, cva6.get('commit')))
    if cva6.get('commit') != EXPECTED_METHOD_CVA6_COMMIT:
        raise PackageError('%s: method source lock cva6 commit is %r, want %s' % (lock_path, cva6.get('commit'), EXPECTED_METHOD_CVA6_COMMIT))

    if base.get('platform_commit') != EXPECTED_METHOD_PLATFORM_COMMIT:
        raise PackageError('%s: base.platform_commit is %r, want %s' % (rel, base.get('platform_commit'), EXPECTED_METHOD_PLATFORM_COMMIT))
    profile = base.get('profile')
    if not isinstance(profile, dict) or profile.get('research_role') != 2:
        raise PackageError('%s: base.profile.research_role must be 2' % rel)

    checkpoint = base.get('checkpoint')
    cp_path, cp_sha, cp_bytes = _postroute_item(checkpoint, 'base.checkpoint', rel)
    if cp_sha != EXPECTED_POSTROUTE_CHECKPOINT_SHA256:
        raise PackageError('%s: base.checkpoint.sha256 is %r, want %s' % (rel, cp_sha, EXPECTED_POSTROUTE_CHECKPOINT_SHA256))
    cp_file, actual_cp_sha, actual_cp_bytes = _verify_actual_file(
        repo, cp_path, cp_sha, cp_bytes, 'base checkpoint')

    base_timing = base.get('base_timing')
    bt_path, bt_sha, bt_bytes = _postroute_item(base_timing, 'base.base_timing', rel)
    if bt_sha != EXPECTED_POSTROUTE_BASE_TIMING_SHA256:
        raise PackageError('%s: base.base_timing.sha256 is %r, want %s' % (rel, bt_sha, EXPECTED_POSTROUTE_BASE_TIMING_SHA256))
    if base_timing.get('accepted') is not False:
        raise PackageError('%s: base.base_timing.accepted must be false' % rel)
    bt_file, actual_bt_sha, actual_bt_bytes = _verify_actual_file(
        repo, bt_path, bt_sha, bt_bytes, 'base timing')
    bt_text = bt_file.read_text(encoding='utf-8', errors='replace')
    bt_summary = parse_design_timing_summary(bt_text, 'base method routed timing', bt_path)
    bt_summary['loops'] = parse_combinational_loops(bt_text, 'base method routed timing', bt_path)
    declared_bt_summary = base_timing.get('design_timing_summary')
    if declared_bt_summary != bt_summary:
        raise PackageError('%s: base.base_timing.design_timing_summary does not match the actual timing report' % rel)

    tcl = data.get('tcl')
    tcl_path, tcl_sha, tcl_bytes = _postroute_item(tcl, 'tcl', rel)
    if tcl_sha != EXPECTED_POSTROUTE_TCL_SHA256:
        raise PackageError('%s: tcl.sha256 is %r, want %s' % (rel, tcl_sha, EXPECTED_POSTROUTE_TCL_SHA256))
    tcl_file, actual_tcl_sha, actual_tcl_bytes = _verify_actual_file(
        repo, tcl_path, tcl_sha, tcl_bytes, 'post-route tcl')

    tool = data.get('tool')
    if not isinstance(tool, dict):
        raise PackageError('%s: tool is missing' % rel)
    if tool.get('session_id') != EXPECTED_POSTROUTE_SESSION_ID:
        raise PackageError('%s: tool.session_id is %r, want %s' % (rel, tool.get('session_id'), EXPECTED_POSTROUTE_SESSION_ID))
    if tool.get('exit_code') != 0:
        raise PackageError('%s: tool.exit_code is %r, want 0' % (rel, tool.get('exit_code')))
    exec_path = tool.get('execution_path')
    if not isinstance(exec_path, str) or exec_path != POSTROUTE_EXECUTION_REL:
        raise PackageError('%s: tool.execution_path is %r, want %s' % (rel, exec_path, POSTROUTE_EXECUTION_REL))
    exec_sha = tool.get('execution_sha256')
    if not isinstance(exec_sha, str) or not HEX64_RE.fullmatch(exec_sha):
        raise PackageError('%s: tool.execution_sha256 is missing or invalid' % rel)
    exec_sha = exec_sha.lower()
    exec_file = require_file(repo, exec_path, 'post-route execution')
    actual_exec_sha = sha256_file(exec_file)
    if actual_exec_sha != exec_sha:
        raise PackageError('%s: tool.execution_sha256 is %s, actual %s is %s' % (rel, exec_sha, exec_path, actual_exec_sha))
    exec_data = read_json(exec_file, 'post-route execution')
    if not isinstance(exec_data, dict):
        raise PackageError('%s: post-route execution is not a JSON object' % exec_path)
    if exec_data.get('session_id') != tool.get('session_id'):
        raise PackageError('%s: execution.json session_id is %r, manifest tool.session_id is %r' % (exec_path, exec_data.get('session_id'), tool.get('session_id')))
    if exec_data.get('exit_code') != tool.get('exit_code'):
        raise PackageError('%s: execution.json exit_code is %r, manifest tool.exit_code is %r' % (exec_path, exec_data.get('exit_code'), tool.get('exit_code')))

    input_entry = data.get('input')
    input_path, input_sha, input_bytes = _postroute_item(input_entry, 'input', rel)
    if input_path != POSTROUTE_INPUT_REL:
        raise PackageError('%s: input.path is %r, want %s' % (rel, input_path, POSTROUTE_INPUT_REL))
    input_file, actual_input_sha, actual_input_bytes = _verify_actual_file(
        repo, input_path, input_sha, input_bytes, 'post-route input')
    input_data = read_json(input_file, 'post-route input')
    if not isinstance(input_data, dict):
        raise PackageError('%s: post-route input is not a JSON object' % input_path)
    expected_input = {
        'base_manifest': (mm_path, mm_sha, mm_bytes),
        'input_checkpoint': (cp_path, cp_sha, cp_bytes),
        'tcl': (tcl_path, tcl_sha, tcl_bytes),
        'base_timing': (bt_path, bt_sha, bt_bytes),
    }
    for key, (want_path, want_sha, want_bytes) in expected_input.items():
        item = input_data.get(key)
        if not isinstance(item, dict):
            raise PackageError('%s: input.json %s is missing or not an object' % (input_path, key))
        got_path = item.get('path')
        got_sha = item.get('sha256')
        got_bytes = item.get('bytes')
        if got_path != want_path:
            raise PackageError('%s: input.json %s.path is %r, want %s' % (input_path, key, got_path, want_path))
        if not isinstance(got_sha, str) or got_sha.lower() != want_sha:
            raise PackageError('%s: input.json %s.sha256 is %r, want %s' % (input_path, key, got_sha, want_sha))
        if got_bytes != want_bytes:
            raise PackageError('%s: input.json %s.bytes is %r, want %s' % (input_path, key, got_bytes, want_bytes))

    outputs = data.get('outputs')
    if not isinstance(outputs, dict):
        raise PackageError('%s: outputs are missing' % rel)
    required = ('optimized_dcp', 'bit', 'mcs', 'utilization', 'timing')
    verified: Dict[str, Any] = {}
    for key in required:
        item = outputs.get(key)
        output_path, output_sha, output_bytes = _postroute_item(item, 'outputs.%s' % key, rel)
        expected_filename = POSTROUTE_OUTPUT_FILENAMES[key]
        pure = pathlib.PurePosixPath(output_path)
        if pure.name != expected_filename:
            raise PackageError('%s: outputs.%s.path basename is %r, want %s' % (rel, key, pure.name, expected_filename))
        if not output_path.startswith(POSTROUTE_DIR_REL + '/'):
            raise PackageError('%s: outputs.%s.path is %r, want under %s' % (rel, key, output_path, POSTROUTE_DIR_REL))
        output_file = require_file(repo, output_path, '%s outputs.%s' % (rel, key))
        actual_sha = sha256_file(output_file)
        actual_bytes = output_file.stat().st_size
        if actual_sha != output_sha:
            raise PackageError('%s: outputs.%s sha256 mismatch: %s != %s' % (rel, key, actual_sha, output_sha))
        if actual_bytes != output_bytes:
            raise PackageError('%s: outputs.%s bytes mismatch: %s != %s' % (rel, key, actual_bytes, output_bytes))
        verified[key] = {
            'source_path': output_path,
            'resolved': output_file,
            'sha256': actual_sha,
            'bytes': actual_bytes,
        }

    timing_rel = verified['timing']['source_path']
    timing_text = verified['timing']['resolved'].read_text(encoding='utf-8', errors='replace')
    timing_summary = parse_design_timing_summary(timing_text, 'role 2 post-route optimized timing', timing_rel)
    timing_summary['loops'] = parse_combinational_loops(timing_text, 'role 2 post-route optimized timing', timing_rel)
    validate_routed_timing(timing_summary, 'role 2 post-route optimized timing', timing_rel)
    declared_summary = outputs['timing'].get('design_timing_summary')
    if declared_summary != timing_summary:
        raise PackageError('%s: outputs.timing.design_timing_summary does not match the actual timing report' % rel)
    if outputs['timing'].get('accepted') is not True:
        raise PackageError('%s: outputs.timing.accepted must be true' % rel)

    return {
        'label': 'role 2 post-route optimization manifest',
        'rel': rel,
        'path': path,
        'data': data,
        'sha256': sha256_file(path),
        'outputs': verified,
        'base_timing_summary': bt_summary,
        'timing_summary': timing_summary,
        'input': {'source_path': input_path, 'resolved': input_file,
                  'sha256': actual_input_sha, 'bytes': actual_input_bytes},
        'execution': {'source_path': exec_path, 'resolved': exec_file,
                      'sha256': actual_exec_sha},
        'base_method_manifest': {'source_path': mm_path, 'resolved': mm_file,
                                 'sha256': actual_mm_sha, 'bytes': actual_mm_bytes},
        'sources_lock': {'source_path': lock_path, 'resolved': lock_file,
                         'sha256': actual_lock_sha, 'cva6_commit': cva6.get('commit')},
    }


# ---------------------------------------------------------------------------
# Archive verification
# ---------------------------------------------------------------------------

def verify_archive(archive: pathlib.Path, delivery: pathlib.Path,
                   member_rels: Sequence[str], hashes: Dict[str, str],
                   sums_sha: str) -> None:
    expected = set()
    for rel in member_rels:
        expected.add(DELIVERY_DIRNAME + '/' + rel)
    seen = set()
    with tarfile.open(str(archive), 'r:gz') as tar:
        for info in tar.getmembers():
            if not info.isfile():
                raise PackageError('archive member is not a regular file: %s' % info.name)
            if info.name in seen:
                raise PackageError('archive has a duplicate member: %s' % info.name)
            if info.name not in expected:
                raise PackageError('archive has an unexpected member: %s' % info.name)
            seen.add(info.name)
            rel = info.name[len(DELIVERY_DIRNAME) + 1:]
            handle = tar.extractfile(info)
            if handle is None:
                raise PackageError('archive member has no data: %s' % info.name)
            digest = hashlib.sha256()
            with handle:
                for chunk in iter(lambda: handle.read(1024 * 1024), b''):
                    digest.update(chunk)
            actual = digest.hexdigest()
            want = sums_sha if rel == SHA256SUMS_FILENAME else hashes[rel]
            if actual != want:
                raise PackageError('archive member sha256 mismatch: %s' % info.name)
            if sha256_file(delivery_path(delivery, rel)) != actual:
                raise PackageError('archive member differs from the original file: %s' %
                                   info.name)
            if rel != SHA256SUMS_FILENAME and hashes.get(rel) != actual:
                raise PackageError('archive member disagrees with SHA256SUMS: %s' % rel)
    missing = expected - seen
    if missing:
        raise PackageError('archive is missing members: %s' % ', '.join(sorted(missing)))


# ---------------------------------------------------------------------------
# Packaging run
# ---------------------------------------------------------------------------

def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog='package_research2.py',
        description='Assemble the two-BIT research2 delivery tree (host-side only).')
    parser.add_argument('--out', required=True,
                        help='new output directory; it must not exist yet')
    parser.add_argument('--repo-root', default=str(REPO_ROOT_DEFAULT),
                        help='repository root (default: the parent of scripts/)')
    return parser


def run(argv: Optional[Sequence[str]] = None) -> Dict[str, Any]:
    args = build_parser().parse_args(argv)
    repo = pathlib.Path(args.repo_root).resolve()
    if not repo.is_dir():
        raise PackageError('--repo-root is not a directory: %s' % repo)
    out = pathlib.Path(args.out)
    if out.exists():
        raise PackageError('--out already exists, refusing to reuse it: %s' % out)

    # 1. manifests --------------------------------------------------------
    software = load_manifest(repo, SOFTWARE_MANIFEST_REL, 'software manifest',
                             expected_platform_commit=EXPECTED_SOFTWARE_PLATFORM_COMMIT)
    e2 = load_manifest(repo, FPGA_MANIFEST_REL_FMT.format(profile=ROLE_PROFILE[1]),
                       'fpga manifest role 1 (E2)',
                       expected_role=1,
                       expected_platform_commit=EXPECTED_E2_PLATFORM_COMMIT)
    method = load_manifest(repo, FPGA_MANIFEST_REL_FMT.format(profile=ROLE_PROFILE[2]),
                           'fpga manifest role 2 (method)',
                           expected_role=2,
                           expected_platform_commit=EXPECTED_METHOD_PLATFORM_COMMIT)
    manifests = [software, e2, method]

    # 2. sources locks ----------------------------------------------------
    lock_paths = {
        'software': resolve_rel(repo, SOFTWARE_SOURCES_LOCK_REL),
        'e2': resolve_rel(repo, E2_SOURCES_LOCK_REL),
        'method': resolve_rel(repo, SOURCES_LOCK_REL),
    }
    lock_sources = {
        'software': SOFTWARE_SOURCES_LOCK_REL,
        'e2': E2_SOURCES_LOCK_REL,
        'method': SOURCES_LOCK_REL,
    }
    for label, path in lock_paths.items():
        if not path.is_file():
            raise PackageError('%s sources lock is missing: %s' %
                               (label, lock_sources[label]))
    lock_sha = {label: sha256_file(path) for label, path in lock_paths.items()}
    if lock_sha['software'] != EXPECTED_SOFTWARE_LOCK_SHA256:
        raise PackageError('software sources lock %s hash is %s, want %s' %
                           (SOFTWARE_SOURCES_LOCK_REL, lock_sha['software'],
                            EXPECTED_SOFTWARE_LOCK_SHA256))
    if lock_sha['e2'] != EXPECTED_E2_LOCK_SHA256:
        raise PackageError('E2 sources lock %s hash is %s, want %s' %
                           (E2_SOURCES_LOCK_REL, lock_sha['e2'],
                            EXPECTED_E2_LOCK_SHA256))
    if len(set(lock_sha.values())) != len(lock_sha):
        raise PackageError('software, E2 and method sources locks are identical; '
                           'each role build must carry its own cva6 commit')
    declared = {
        'software': software['data']['sources_lock_sha256'].lower(),
        'e2': e2['data']['sources_lock_sha256'].lower(),
        'method': method['data']['sources_lock_sha256'].lower(),
    }
    for label, entry in (('software', software), ('e2', e2), ('method', method)):
        if declared[label] != lock_sha[label]:
            raise PackageError('%s (%s): sources_lock_sha256 %s does not match the '
                               '%s lock %s hash %s' %
                               (entry['label'], entry['rel'], declared[label],
                                label, lock_sources[label], lock_sha[label]))
    validate_lock_triple(lock_paths, EXPECTED_CVA6_COMMITS)
    lock_delivery_rel = {
        label: 'provenance/' + pathlib.PurePosixPath(lock_sources[label]).name
        for label in lock_paths
    }

    # 3. artifacts listed by every manifest -------------------------------
    verified_artifacts: Dict[str, str] = {}
    for entry in manifests:
        verify_manifest_artifacts(repo, entry, verified_artifacts)
    software_artifacts = software['data'].get('artifacts')
    if software_artifacts != EXPECTED_SOFTWARE_ARTIFACTS:
        raise PackageError('software manifest artifacts differ from the original '
                           'software artifact set')

    # 4. fixed inputs -----------------------------------------------------
    bits = {}
    mcs = {}
    for role in (1,):
        profile = ROLE_PROFILE[role]
        bit_rel = BIT_REL_FMT.format(profile=profile)
        bits[role] = (bit_rel, require_file(repo, bit_rel, 'role %d bit' % role))
        mcs_rel = MCS_REL_FMT.format(profile=profile)
        mcs[role] = (mcs_rel, require_file(repo, mcs_rel, 'role %d mcs' % role))
    postroute = load_postroute_manifest(repo)
    bits[2] = (postroute['outputs']['bit']['source_path'], postroute['outputs']['bit']['resolved'])
    mcs[2] = (postroute['outputs']['mcs']['source_path'], postroute['outputs']['mcs']['resolved'])
    payload_path = require_file(repo, PAYLOAD_REL, 'opensbi payload')
    cli_path = require_file(repo, CLI_REL, 'board cli')
    kernel_path = require_file(repo, KERNEL_RELEASE_REL, 'kernel release')
    kernel_release = kernel_path.read_bytes().decode('utf-8', 'replace').strip()
    if kernel_release != EXPECTED_KERNEL_RELEASE:
        raise PackageError('kernel release mismatch: %r != %r' %
                           (kernel_release, EXPECTED_KERNEL_RELEASE))

    # 4b. mandatory routed timing acceptance --------------------------------
    timing_acceptance = {}
    timing_acceptance[1] = load_routed_timing_acceptance(
        repo, ROLE_PROFILE[1], 'role 1 (%s) routed timing' % ROLE_PROFILE[1])
    timing_acceptance[2] = {
        'source_path': postroute['outputs']['timing']['source_path'],
        'sha256': postroute['outputs']['timing']['sha256'],
        'design_timing_summary': postroute['timing_summary'],
    }
    base_timing_preserved = {
        'source_path': postroute['data']['base']['base_timing']['path'],
        'sha256': postroute['data']['base']['base_timing']['sha256'],
        'design_timing_summary': postroute['base_timing_summary'],
        'accepted': False,
        'note': ('the original base method timing is negative and is preserved '
                 'as provenance only; it does not block delivery because the '
                 'authenticated post-route optimization passed every gate'),
    }

    # 5. plans ------------------------------------------------------------
    plan_module_path = resolve_rel(repo, PLAN_MODULE_REL)
    if not plan_module_path.is_file():
        raise PackageError('plan generator is missing: %s' % PLAN_MODULE_REL)
    plan_module = import_module_from_path(plan_module_path, 'research2_packaging_plan')
    plan_text: Dict[str, str] = {}
    plan_count: Dict[str, int] = {}
    for stage, expected in PLAN_COUNTS:
        plan = plan_module.build_plan(stage)
        if not isinstance(plan, dict):
            raise PackageError('build_plan(%r) did not return an object' % stage)
        if plan.get('stage') != stage:
            raise PackageError('build_plan(%r) returned stage %r' % (stage, plan.get('stage')))
        cases = plan.get('cases')
        if not isinstance(cases, list):
            raise PackageError('build_plan(%r) has no case list' % stage)
        if len(cases) != expected:
            raise PackageError('build_plan(%r) produced %d cases, want %d' %
                               (stage, len(cases), expected))
        if plan.get('count') != expected:
            raise PackageError('build_plan(%r) count is %r, want %d' %
                               (stage, plan.get('count'), expected))
        plan_text[stage] = json.dumps(plan, indent=2, ensure_ascii=False) + NL
        plan_count[stage] = len(cases)

    # 6. delivery tree ----------------------------------------------------
    out.mkdir(parents=True, exist_ok=False)
    delivery = out / DELIVERY_DIRNAME
    delivery.mkdir(parents=True, exist_ok=False)
    members: List[str] = []

    def add(dst_rel: str, src: pathlib.Path) -> None:
        dst = delivery_path(delivery, dst_rel)
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(str(src), str(dst))
        if sha256_file(dst) != sha256_file(src):
            raise PackageError('copy verification failed: %s' % dst_rel)
        members.append(dst_rel)

    add('artifacts/role1.bit', bits[1][1])
    add('artifacts/role2.bit', bits[2][1])
    add('artifacts/fw_payload.bin', payload_path)
    add('artifacts/idma_research', cli_path)
    for rel in REQUIRED_SCRIPTS:
        add(rel, require_file(repo, rel, 'required script'))
    for rel in REQUIRED_DOCS:
        add(rel, require_file(repo, rel, 'required doc'))
    for rel in OPTIONAL_DOCS:
        optional = resolve_rel(repo, rel)
        if optional.is_file():
            add(rel, optional)
    for rel in PROVENANCE_CONFIGS:
        add('provenance/configs/' + pathlib.PurePosixPath(rel).name,
            require_file(repo, rel, 'required config'))
    for label in ('software', 'e2', 'method'):
        add(lock_delivery_rel[label], lock_paths[label])
    for entry in manifests:
        add('provenance/manifests/' + pathlib.PurePosixPath(entry['rel']).name,
            entry['path'])
    for role in (1, 2):
        add(TIMING_REPORT_DELIVERY_REL_FMT.format(role=role),
            resolve_rel(repo, timing_acceptance[role]['source_path']))
    for rel, src in (
            ('provenance/postroute/manifest.json', postroute['path']),
            ('provenance/postroute/optimize.tcl', resolve_rel(repo, postroute['data']['tcl']['path'])),
            ('provenance/postroute/execution.json', resolve_rel(repo, POSTROUTE_EXECUTION_REL)),
            ('provenance/postroute/input.json', resolve_rel(repo, POSTROUTE_INPUT_REL))):
        add(rel, src)
    for stage, _expected in PLAN_COUNTS:
        rel = 'plans/%s.json' % stage
        dst = delivery_path(delivery, rel)
        dst.parent.mkdir(parents=True, exist_ok=True)
        dst.write_bytes(plan_text[stage].encode('utf-8'))
        members.append(rel)

    hashes: Dict[str, str] = {}
    for rel in members:
        hashes[rel] = sha256_file(delivery_path(delivery, rel))

    # 7. DELIVERY.json ----------------------------------------------------
    host_commit = git_head(repo)
    delivery_manifest = {
        'schema': DELIVERY_SCHEMA,
        'created_at': utc_now(),
        'software': {
            'kernel_release': kernel_release,
            'cli_path': 'artifacts/idma_research',
            'cli_sha256': hashes['artifacts/idma_research'],
            'board_cli_path': BOARD_CLI_PATH,
            'payload': 'artifacts/fw_payload.bin',
            'payload_sha256': hashes['artifacts/fw_payload.bin'],
        },
        'images': [
            {'role': 1,
             'bit': 'artifacts/role1.bit',
             'sha256': hashes['artifacts/role1.bit'],
             'profile': ROLE_PROFILE[1]},
            {'role': 2,
             'bit': 'artifacts/role2.bit',
             'sha256': hashes['artifacts/role2.bit'],
             'profile': ROLE_PROFILE[2]},
        ],
        'scripts': {
            'plan': 'scripts/research2_plan.py',
            'plan_sha256': hashes['scripts/research2_plan.py'],
            'analyzer': 'scripts/research2_results.py',
            'analyzer_sha256': hashes['scripts/research2_results.py'],
            'runner': 'scripts/mac_research2.py',
            'runner_sha256': hashes['scripts/mac_research2.py'],
            'legacy': 'scripts/research_results.py',
            'legacy_sha256': hashes['scripts/research_results.py'],
        },
        'plans': {
            'e2': {'path': 'plans/e2.json', 'sha256': hashes['plans/e2.json'],
                   'count': plan_count['e2']},
            'method': {'path': 'plans/method.json', 'sha256': hashes['plans/method.json'],
                       'count': plan_count['method']},
        },
        'provenance': {
            'host_tools_commit': host_commit,
            'role_builds': {
                'software': {
                    'platform_commit': software['data']['platform_commit'],
                    'sources_lock': {
                        'path': lock_delivery_rel['software'],
                        'source_path': SOFTWARE_SOURCES_LOCK_REL,
                        'sha256': lock_sha['software'],
                        'cva6_commit': EXPECTED_SOFTWARE_CVA6_COMMIT,
                    },
                    'manifest': 'provenance/manifests/' +
                                pathlib.PurePosixPath(software['rel']).name,
                },
                'e2_role1': {
                    'platform_commit': e2['data']['platform_commit'],
                    'sources_lock': {
                        'path': lock_delivery_rel['e2'],
                        'source_path': E2_SOURCES_LOCK_REL,
                        'sha256': lock_sha['e2'],
                        'cva6_commit': EXPECTED_E2_CVA6_COMMIT,
                    },
                    'manifest': 'provenance/manifests/' +
                                pathlib.PurePosixPath(e2['rel']).name,
                },
                'method_role2': {
                    'platform_commit': method['data']['platform_commit'],
                    'sources_lock': {
                        'path': lock_delivery_rel['method'],
                        'source_path': SOURCES_LOCK_REL,
                        'sha256': lock_sha['method'],
                        'cva6_commit': EXPECTED_METHOD_CVA6_COMMIT,
                    },
                    'manifest': 'provenance/manifests/' +
                                pathlib.PurePosixPath(method['rel']).name,
                    'postroute_optimization_manifest': 'provenance/postroute/manifest.json',
                },
            },
            'software_platform_commit': software['data']['platform_commit'],
            'software_sources_lock': {
                'path': lock_delivery_rel['software'],
                'source_path': SOFTWARE_SOURCES_LOCK_REL,
                'sha256': lock_sha['software'],
                'cva6_commit': EXPECTED_SOFTWARE_CVA6_COMMIT,
            },
            'e2_platform_commit': e2['data']['platform_commit'],
            'e2_sources_lock': {
                'path': lock_delivery_rel['e2'],
                'source_path': E2_SOURCES_LOCK_REL,
                'sha256': lock_sha['e2'],
                'cva6_commit': EXPECTED_E2_CVA6_COMMIT,
            },
            'method_platform_commit': method['data']['platform_commit'],
            'method_sources_lock': {
                'path': lock_delivery_rel['method'],
                'source_path': SOURCES_LOCK_REL,
                'sha256': lock_sha['method'],
                'cva6_commit': EXPECTED_METHOD_CVA6_COMMIT,
            },
            'compatibility_note': COMPATIBILITY_NOTE,
            'manifests': {
                'software': 'provenance/manifests/' +
                            pathlib.PurePosixPath(software['rel']).name,
                'e2_role1': 'provenance/manifests/' +
                             pathlib.PurePosixPath(e2['rel']).name,
                'method_role2': 'provenance/manifests/' +
                                 pathlib.PurePosixPath(method['rel']).name,
            },
            'postroute_optimization': {
                'manifest': {
                    'path': 'provenance/postroute/manifest.json',
                    'sha256': hashes['provenance/postroute/manifest.json'],
                },
                'tcl': {
                    'path': 'provenance/postroute/optimize.tcl',
                    'sha256': postroute['data']['tcl']['sha256'],
                },
                'execution': {
                    'path': 'provenance/postroute/execution.json',
                    'sha256': postroute['execution']['sha256'],
                },
                'input': {
                    'path': 'provenance/postroute/input.json',
                    'sha256': postroute['input']['sha256'],
                },
                'base_method_manifest': {
                    'path': postroute['data']['base']['method_manifest']['path'],
                    'sha256': postroute['data']['base']['method_manifest']['sha256'],
                },
                'base_platform_commit': postroute['data']['base']['platform_commit'],
                'base_checkpoint': postroute['data']['base']['checkpoint'],
                'base_timing': postroute['data']['base']['base_timing'],
                'session_id': postroute['data']['tool']['session_id'],
                'exit_code': postroute['data']['tool']['exit_code'],
                'optimized': {
                    'dcp': postroute_output_record(postroute['outputs']['optimized_dcp']),
                    'utilization_report': postroute_output_record(postroute['outputs']['utilization']),
                    'timing_report': postroute_output_record(postroute['outputs']['timing']),
                },
                'timing_acceptance': postroute['timing_summary'],
            },
            'timing_acceptance': {
                str(role): {
                    'source_path': timing_acceptance[role]['source_path'],
                    'sha256': timing_acceptance[role]['sha256'],
                    'delivery_path': TIMING_REPORT_DELIVERY_REL_FMT.format(role=role),
                    'design_timing_summary': timing_acceptance[role]['design_timing_summary'],
                }
                for role in (1, 2)
            },
        },
        'note': ('host-side package only; the plans carry unresolved templates and '
                 'no calibration selection has been applied; the existing software '
                 'build is reused unchanged and is not rebuilt; the E2 role1 BIT and '
                 'manifest are reused unchanged; the method role2 BIT, MCS and timing '
                 'come from the authenticated post-route optimization manifest, not '
                 'from the base method build; the original base method timing is '
                 'negative and is preserved as provenance only and does not block '
                 'delivery because the optimized timing passed every routed-timing '
                 'gate; the routed timing acceptance was validated from the actual '
                 'post-route Design Timing Summary and explicit combinational loops '
                 'count; the legacy clock/IO/CDC methodology is not signed off and '
                 'stale methodology TIMING-16 warnings are not used as timing '
                 'evidence; ' + COMPATIBILITY_NOTE),
    }
    write_json(delivery_path(delivery, 'DELIVERY.json'), delivery_manifest)
    members.append('DELIVERY.json')
    hashes['DELIVERY.json'] = sha256_file(delivery_path(delivery, 'DELIVERY.json'))

    # 8. SHA256SUMS -------------------------------------------------------
    sums_text = ''.join('%s  %s%s' % (hashes[rel], rel, NL) for rel in sorted(hashes))
    sums_path = delivery_path(delivery, SHA256SUMS_FILENAME)
    sums_path.write_bytes(sums_text.encode('utf-8'))
    sums_sha = sha256_file(sums_path)
    members.append(SHA256SUMS_FILENAME)

    # 9. archive ----------------------------------------------------------
    member_rels = sorted(members)
    archive_path = out / ARCHIVE_FILENAME
    with tarfile.open(str(archive_path), 'w:gz') as tar:
        for rel in member_rels:
            src = delivery_path(delivery, rel)
            stat = src.stat()
            info = tarfile.TarInfo(DELIVERY_DIRNAME + '/' + rel)
            info.size = stat.st_size
            info.mtime = int(stat.st_mtime)
            info.mode = 0o644
            info.type = tarfile.REGTYPE
            info.uid = 0
            info.gid = 0
            info.uname = ''
            info.gname = ''
            with open(str(src), 'rb') as handle:
                tar.addfile(info, handle)
    verify_archive(archive_path, delivery, member_rels, hashes, sums_sha)
    archive_sha = sha256_file(archive_path)

    # 10. acceptance.json -------------------------------------------------
    acceptance = {
        'schema': ACCEPTANCE_SCHEMA,
        'generated_at': utc_now(),
        'repo_root': str(repo),
        'out_dir': str(out.resolve()),
        'host_tools_commit': host_commit,
        'board_validation': False,
        'board_validation_note': ('no board validation is performed or claimed: '
                                  'this package is host-side evidence only and '
                                  'nothing in this run flashes, boots or measures '
                                  'either BIT'),
        'software_sources_lock': {'path': SOFTWARE_SOURCES_LOCK_REL,
                                  'sha256': lock_sha['software'],
                                  'cva6_commit': EXPECTED_SOFTWARE_CVA6_COMMIT},
        'e2_sources_lock': {'path': E2_SOURCES_LOCK_REL,
                            'sha256': lock_sha['e2'],
                            'cva6_commit': EXPECTED_E2_CVA6_COMMIT},
        'method_sources_lock': {'path': SOURCES_LOCK_REL,
                                'sha256': lock_sha['method'],
                                'cva6_commit': EXPECTED_METHOD_CVA6_COMMIT},
        'role_builds': {
            'software': {
                'platform_commit': software['data']['platform_commit'],
                'sources_lock': lock_delivery_rel['software'],
            },
            'e2_role1': {
                'platform_commit': e2['data']['platform_commit'],
                'sources_lock': lock_delivery_rel['e2'],
            },
            'method_role2': {
                'platform_commit': method['data']['platform_commit'],
                'sources_lock': lock_delivery_rel['method'],
                'postroute_optimization_manifest': POSTROUTE_MANIFEST_REL,
            },
        },
        'manifests': [
            {'label': entry['label'],
             'path': entry['rel'],
             'sha256': entry['sha256'],
             'role': entry['role'],
             'status': entry['data'].get('status'),
             'platform_commit': entry['data'].get('platform_commit')}
            for entry in manifests
        ],
        'timing_acceptance': {
            str(role): {
                'source_path': timing_acceptance[role]['source_path'],
                'sha256': timing_acceptance[role]['sha256'],
                'delivery_path': TIMING_REPORT_DELIVERY_REL_FMT.format(role=role),
                'design_timing_summary': timing_acceptance[role]['design_timing_summary'],
            }
            for role in (1, 2)
        },
        'postroute_optimization': {
            'manifest': {
                'path': POSTROUTE_MANIFEST_REL,
                'sha256': postroute['sha256'],
            },
            'tcl': {
                'path': postroute['data']['tcl']['path'],
                'sha256': postroute['data']['tcl']['sha256'],
            },
            'execution': {
                'path': POSTROUTE_EXECUTION_REL,
                'sha256': postroute['execution']['sha256'],
            },
            'input': {
                'path': POSTROUTE_INPUT_REL,
                'sha256': postroute['input']['sha256'],
            },
            'base_method_manifest': {
                'path': postroute['data']['base']['method_manifest']['path'],
                'sha256': postroute['data']['base']['method_manifest']['sha256'],
            },
            'base_platform_commit': postroute['data']['base']['platform_commit'],
            'base_checkpoint': postroute['data']['base']['checkpoint'],
            'base_timing': postroute['data']['base']['base_timing'],
            'session_id': postroute['data']['tool']['session_id'],
            'exit_code': postroute['data']['tool']['exit_code'],
            'optimized': {
                'dcp': postroute_output_record(postroute['outputs']['optimized_dcp']),
                'utilization_report': postroute_output_record(postroute['outputs']['utilization']),
                'timing_report': postroute_output_record(postroute['outputs']['timing']),
            },
            'timing_acceptance': postroute['timing_summary'],
        },
        'base_timing_preserved': base_timing_preserved,
        'methodology_note': ('the legacy clock/IO/CDC methodology is not signed off; '
                             'the routed timing acceptance above is based only on the '
                             'actual post-route Design Timing Summary and the actual '
                             'combinational-loop count; stale methodology TIMING-16 '
                             'warnings are not used as timing evidence.'),
        'verified_manifest_artifacts': dict(sorted(verified_artifacts.items())),
        'fixed_inputs': {
            'payload': {'path': PAYLOAD_REL, 'sha256': sha256_file(payload_path)},
            'cli': {'path': CLI_REL, 'sha256': sha256_file(cli_path)},
            'kernel_release': {'path': KERNEL_RELEASE_REL, 'value': kernel_release},
            'bit_role1': {'path': bits[1][0], 'sha256': sha256_file(bits[1][1])},
            'bit_role2': {'path': bits[2][0], 'sha256': sha256_file(bits[2][1])},
            'mcs_role1': {'path': mcs[1][0], 'sha256': sha256_file(mcs[1][1])},
            'mcs_role2': {'path': mcs[2][0], 'sha256': sha256_file(mcs[2][1])},
        },
        'delivery': {
            'path': DELIVERY_DIRNAME,
            'manifest': {'path': 'DELIVERY.json', 'sha256': hashes['DELIVERY.json']},
            'sha256sums': {'path': SHA256SUMS_FILENAME, 'sha256': sums_sha,
                           'entries': len(hashes)},
            'files': dict(sorted(hashes.items())),
            'file_count': len(members),
        },
        'plans': {
            'e2': {'path': 'plans/e2.json', 'count': plan_count['e2'],
                   'sha256': hashes['plans/e2.json']},
            'method': {'path': 'plans/method.json', 'count': plan_count['method'],
                       'sha256': hashes['plans/method.json']},
        },
        'archive': {'path': ARCHIVE_FILENAME, 'sha256': archive_sha,
                    'size': archive_path.stat().st_size,
                    'members': len(member_rels)},
    }
    write_json(out / ACCEPTANCE_FILENAME, acceptance)

    summary = {
        'ok': True,
        'out': str(out.resolve()),
        'delivery': str(delivery.resolve()),
        'archive': str(archive_path.resolve()),
        'acceptance': str((out / ACCEPTANCE_FILENAME).resolve()),
        'host_tools_commit': host_commit,
        'software_platform_commit': software['data']['platform_commit'],
        'e2_platform_commit': e2['data']['platform_commit'],
        'method_platform_commit': method['data']['platform_commit'],
        'software_sources_lock_sha256': lock_sha['software'],
        'e2_sources_lock_sha256': lock_sha['e2'],
        'method_sources_lock_sha256': lock_sha['method'],
        'plans': {stage: plan_count[stage] for stage, _expected in PLAN_COUNTS},
        'delivery_files': len(members),
        'archive_members': len(member_rels),
        'board_validation': False,
    }
    print(json.dumps(summary, indent=2, ensure_ascii=False, sort_keys=True))
    return summary


def main(argv: Optional[Sequence[str]] = None) -> int:
    try:
        run(argv)
    except PackageError as exc:
        sys.stderr.write('package_research2: error: %s%s' % (exc, NL))
        return 2
    return 0


if __name__ == '__main__':
    sys.exit(main())
