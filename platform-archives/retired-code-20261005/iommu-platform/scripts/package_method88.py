#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''package_method88.py - build a self-contained method88 host delivery package.

This script packages the reviewed two-boot role-2 method88 host runner.  It is
host-side only: it copies existing files, regenerates the frozen 88-case plan
from the frozen selection through the reviewed ``research_method88.freeze``
interface, verifies the resulting package with the reviewed
``scripts/mac_method88.py`` verifier, and writes a deterministic tar.gz.

It never touches the board, never rebuilds Linux/BIT/software, never writes an
SD card, never asks for approval and never uses the network.
'''
from __future__ import annotations

import argparse
import gzip
import hashlib
import importlib.util
import json
import pathlib
import shutil
import sys
import tarfile
from typing import Any, Dict, List, Optional, Sequence

HERE = pathlib.Path(__file__).resolve()
SCRIPTS_DIR = HERE.parent
REPO_ROOT = SCRIPTS_DIR.parent

ARCHIVE_PREFIX = 'method88-20261004'
ARCHIVE_NAME = 'method88-20261004.tar.gz'
SHA256SUMS_NAME = 'SHA256SUMS'
ACCEPTANCE_NAME = 'acceptance.json'
DELIVERY_DIRNAME = 'delivery'
README_NAME = 'README.md'

EXPECTED_OLD_SCHEMA = 'research2-delivery-v1'
ROLE2_BIT_REL = 'artifacts/role2.bit'
EXPECTED_ROLE2_BIT_BYTES = 11443722

EXPECTED_SOURCE_SOFTWARE_EXTRA = {
    'cli_path': 'artifacts/idma_research',
    'payload_sha256': 'af9f703b6d00420598282a983201361983b17ed731bc245284b67689dcd9e748',
}

SOURCE_SCRIPT_FILES = (
    'scripts/mac_research2.py',
    'scripts/research2_plan.py',
    'scripts/research2_results.py',
    'scripts/research_results.py',
    'scripts/research_method88.py',
    'scripts/mac_method88.py',
)

SOURCE_DOCS_FILE = 'docs/mac-method88.md'


class PackageError(Exception):
    '''A refused or impossible host-side packaging request.'''


def _load_module(module_name, filename):
    path = SCRIPTS_DIR / filename
    if not path.is_file():
        raise PackageError('cannot find %s next to %s' % (filename, HERE))
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        raise PackageError('cannot load %s' % path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module


mac_method88 = _load_module('method88_packager_mac_method88', 'mac_method88.py')
method88 = mac_method88.method88


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, 'rb') as handle:
        for chunk in iter(lambda: handle.read(1 << 20), b''):
            digest.update(chunk)
    return digest.hexdigest()


def _read_json(path, label):
    try:
        text = path.read_text(encoding='utf-8')
    except OSError as exc:
        raise PackageError('%s is unreadable: %r' % (label, exc))
    try:
        return json.loads(text)
    except ValueError as exc:
        raise PackageError('%s is not JSON: %r' % (label, exc))


def _is_same_or_inside(child, parent):
    child = pathlib.Path(child).resolve()
    parent = pathlib.Path(parent).resolve()
    return child == parent or parent in child.parents


def _validate_output_path(output, source_delivery, selection):
    output = pathlib.Path(output)
    if output.exists():
        raise PackageError('output path already exists: %s' % output)
    output_resolved = output.resolve()
    repo_resolved = REPO_ROOT.resolve()
    if output_resolved == repo_resolved:
        raise PackageError('output path must not be the repository root')
    for label, protected in (('source delivery', source_delivery),
                             ('selection', selection)):
        protected_resolved = pathlib.Path(protected).resolve()
        if _is_same_or_inside(output_resolved, protected_resolved):
            raise PackageError('output path must not be inside %s: %s' % (label, output))
        if _is_same_or_inside(protected_resolved, output_resolved):
            raise PackageError('output path must not contain %s: %s' % (label, output))


def _validate_source_delivery(source_delivery):
    manifest_path = source_delivery / 'DELIVERY.json'
    data = _read_json(manifest_path, 'source DELIVERY.json')
    if not isinstance(data, dict):
        raise PackageError('source DELIVERY.json must be a JSON object')
    if data.get('schema') != EXPECTED_OLD_SCHEMA:
        raise PackageError('source DELIVERY.json schema must be %s' % EXPECTED_OLD_SCHEMA)

    software = data.get('software')
    if not isinstance(software, dict):
        raise PackageError('source DELIVERY.json software must be an object')
    for key, expected in mac_method88.EXPECTED_SOFTWARE.items():
        if software.get(key) != expected:
            raise PackageError('source software.%s must be %r, got %r'
                               % (key, expected, software.get(key)))
    for key, expected in EXPECTED_SOURCE_SOFTWARE_EXTRA.items():
        if software.get(key) != expected:
            raise PackageError('source software.%s must be %r, got %r'
                               % (key, expected, software.get(key)))

    images = data.get('images')
    if not isinstance(images, list):
        raise PackageError('source DELIVERY.json images must be a list')
    role2 = [image for image in images
             if isinstance(image, dict) and image.get('role') == mac_method88.ROLE]
    if len(role2) != 1:
        raise PackageError('source DELIVERY.json must carry exactly one role 2 image')
    image = role2[0]
    if image.get('bit') != ROLE2_BIT_REL:
        raise PackageError('source role 2 bit must be %s' % ROLE2_BIT_REL)
    if image.get('sha256') != mac_method88.ROLE2_BIT_SHA256:
        raise PackageError('source role 2 sha256 is not the pinned constant')

    bit_path = source_delivery / ROLE2_BIT_REL
    if not bit_path.is_file():
        raise PackageError('source role 2 bit is missing: %s' % bit_path)
    size = bit_path.stat().st_size
    if size != EXPECTED_ROLE2_BIT_BYTES:
        raise PackageError('source role 2 bit size is %d, expected %d'
                           % (size, EXPECTED_ROLE2_BIT_BYTES))
    digest = sha256_file(bit_path)
    if digest != mac_method88.ROLE2_BIT_SHA256:
        raise PackageError('source role 2 bit sha256 is %s, expected %s'
                           % (digest, mac_method88.ROLE2_BIT_SHA256))
    return data


def _copy_regular_file(source, target):
    if not source.is_file():
        raise PackageError('required source file is missing: %s' % source)
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(source, target)
    if not target.is_file():
        raise PackageError('copy failed: %s' % target)


def _write_text_lf(path, text):
    # Python 3.8 compatible: Path.write_text has no newline= argument there.
    # POSIX does not translate \n, and every caller supplies explicit LF text.
    path.write_text(text, encoding='utf-8')


def _write_json_lf(path, document):
    _write_text_lf(path, json.dumps(document, indent=2, ensure_ascii=False) + '\n')


def _build_files_map(delivery):
    files = {}
    for rel in mac_method88.REQUIRED_FILES:
        path = delivery / rel
        if not path.is_file():
            raise PackageError('required package file is missing: %s' % rel)
        rel_norm = pathlib.PurePosixPath(rel).as_posix()
        if rel_norm != rel:
            raise PackageError('required package path is not posix: %r' % (rel,))
        files[rel] = sha256_file(path)
    return files


def _write_archive(archive_path, root, relative_files):
    members = sorted(set(relative_files))
    with open(archive_path, 'wb') as raw:
        with gzip.GzipFile(fileobj=raw, mode='wb', mtime=0) as gz:
            with tarfile.open(fileobj=gz, mode='w|', format=tarfile.GNU_FORMAT) as tar:
                for rel in members:
                    source = root / rel
                    if not source.is_file():
                        raise PackageError('archive member source is missing: %s' % source)
                    info = tarfile.TarInfo(ARCHIVE_PREFIX + '/' + rel)
                    info.size = source.stat().st_size
                    info.mtime = 0
                    info.mode = 0o644
                    info.uid = 0
                    info.gid = 0
                    info.uname = ''
                    info.gname = ''
                    with open(source, 'rb') as handle:
                        tar.addfile(info, handle)


def _verify_archive(archive_path, root, relative_files):
    expected = {ARCHIVE_PREFIX + '/' + rel: rel for rel in sorted(set(relative_files))}
    seen = {}
    symlinks = []
    with tarfile.open(archive_path, 'r:gz') as tar:
        for member in tar.getmembers():
            name = member.name
            if member.issym() or member.islnk():
                symlinks.append(name)
                continue
            if not member.isfile():
                raise PackageError('archive carries a non-file member: %s' % name)
            if name not in expected:
                raise PackageError('archive carries an unexpected member: %s' % name)
            rel = expected[name]
            extracted = tar.extractfile(member)
            if extracted is None:
                raise PackageError('archive member cannot be read: %s' % name)
            digest = hashlib.sha256()
            for chunk in iter(lambda: extracted.read(1 << 20), b''):
                digest.update(chunk)
            actual = digest.hexdigest()
            source = root / rel
            if not source.is_file():
                raise PackageError('archive source disappeared: %s' % source)
            wanted = sha256_file(source)
            if actual != wanted:
                raise PackageError('archive member %s hash %s != source %s'
                                   % (rel, actual, wanted))
            seen[name] = actual
    missing = sorted(set(expected) - set(seen))
    if missing:
        raise PackageError('archive is missing members: %s' % ', '.join(missing))
    if symlinks:
        raise PackageError('archive carries symlinks: %s' % ', '.join(symlinks))
    return {'members': sorted(seen), 'count': len(seen), 'sha256': sha256_file(archive_path)}


def _build_acceptance(output, delivery, archive, manifest, frozen,
                      selection_bytes, archive_check, sha256sums_lines):
    frozen_selection = delivery / 'frozen' / 'selection.json'
    selection_preserved = False
    if frozen_selection.is_file():
        selection_preserved = frozen_selection.read_bytes() == selection_bytes
    return {
        'schema': 'method88-package-acceptance-v1',
        'status': 'prepared_not_run',
        'output': str(output),
        'delivery': str(delivery),
        'archive': str(archive),
        'checks': {
            'source_delivery_schema': EXPECTED_OLD_SCHEMA,
            'source_software_pinned': True,
            'source_role2_sha256': manifest['images'][0]['sha256'],
            'source_role2_bytes': EXPECTED_ROLE2_BIT_BYTES,
            'manifest_sha256': sha256_file(delivery / 'DELIVERY.json'),
            'files_map_verified': True,
            'files_count': len(manifest['files']),
            'frozen_plan_sha256': frozen.get('plan_sha256'),
            'frozen_plan_cases': frozen.get('count'),
            'frozen_plan_counts': frozen.get('counts'),
            'selection_source_byte_preserved': selection_preserved,
            'archive_members_verified': True,
            'archive_member_count': archive_check['count'],
            'archive_sha256': archive_check['sha256'],
            'sha256sums_lines': sha256sums_lines,
        },
        'limits': {
            'board_tested': False,
            'hardware_rebuilt': False,
            'software_rebuilt': False,
            'linux_rebuilt': False,
            'bit_rebuilt': False,
            'sd_rewritten': False,
            'network_used': False,
        },
        'note': 'host-side packaging only; no board, compiler or network access was used',
    }


def package(source_delivery, selection, output, seed=None, repo_root=None):
    source_delivery = pathlib.Path(source_delivery).resolve()
    selection = pathlib.Path(selection).resolve()
    output = pathlib.Path(output).resolve()
    repo_root = pathlib.Path(repo_root).resolve() if repo_root else REPO_ROOT
    if seed is None:
        seed = method88.ORDER_SEED

    _validate_output_path(output, source_delivery, selection)
    _validate_source_delivery(source_delivery)

    if not selection.is_file():
        raise PackageError('selection file is not a file: %s' % selection)
    selection_bytes = selection.read_bytes()
    try:
        selection_doc = json.loads(selection_bytes.decode('utf-8'))
    except (UnicodeDecodeError, ValueError) as exc:
        raise PackageError('selection is not readable JSON: %r' % exc)
    try:
        method88.validate_selection(selection_doc)
    except Exception as exc:
        raise PackageError('selection is rejected: %r' % exc)

    output.mkdir(parents=True, exist_ok=False)
    delivery = output / DELIVERY_DIRNAME
    delivery.mkdir()

    role2_source = source_delivery / ROLE2_BIT_REL
    role2_target = delivery / ROLE2_BIT_REL
    _copy_regular_file(role2_source, role2_target)
    if sha256_file(role2_target) != mac_method88.ROLE2_BIT_SHA256:
        raise PackageError('copied role 2 bit does not match the pinned hash')
    if role2_target.stat().st_size != EXPECTED_ROLE2_BIT_BYTES:
        raise PackageError('copied role 2 bit size changed')

    for rel in SOURCE_SCRIPT_FILES:
        _copy_regular_file(repo_root / rel, delivery / rel)
    _copy_regular_file(repo_root / SOURCE_DOCS_FILE, delivery / SOURCE_DOCS_FILE)

    frozen_dir = delivery / 'frozen'
    frozen = method88.freeze(selection, frozen_dir, seed)

    files_map = _build_files_map(delivery)
    manifest = {
        'schema': mac_method88.SCHEMA,
        'software': dict(mac_method88.EXPECTED_SOFTWARE),
        'images': [{
            'role': mac_method88.ROLE,
            'bit': ROLE2_BIT_REL,
            'sha256': mac_method88.ROLE2_BIT_SHA256,
        }],
        'frozen': {'root': 'frozen'},
        'files': files_map,
    }
    _write_json_lf(delivery / 'DELIVERY.json', manifest)

    _copy_regular_file(repo_root / SOURCE_DOCS_FILE, output / README_NAME)

    loaded = mac_method88.load_manifest(delivery / 'DELIVERY.json')
    mac_method88.verify_package_files(loaded, delivery)
    frozen_verified = mac_method88.verify_frozen(loaded, delivery)
    if frozen_verified['plan'].get('plan_sha256') != frozen.get('plan_sha256'):
        raise PackageError('frozen plan hash changed between freeze and verify')

    rel_files = ['README.md', 'delivery/DELIVERY.json']
    for rel in mac_method88.REQUIRED_FILES:
        rel_files.append('delivery/' + rel)
    rel_files = sorted(set(rel_files))
    sums_lines = []
    for rel in rel_files:
        path = output / rel
        if not path.is_file():
            raise PackageError('SHA256SUMS target is missing: %s' % rel)
        sums_lines.append('%s  %s' % (sha256_file(path), rel))
    _write_text_lf(output / SHA256SUMS_NAME, '\n'.join(sums_lines) + '\n')

    archive = output / ARCHIVE_NAME
    archive_rel_files = rel_files + [SHA256SUMS_NAME]
    _write_archive(archive, output, archive_rel_files)
    archive_check = _verify_archive(archive, output, archive_rel_files)

    acceptance = _build_acceptance(output, delivery, archive, manifest, frozen,
                                   selection_bytes, archive_check, len(sums_lines))
    _write_json_lf(output / ACCEPTANCE_NAME, acceptance)

    return {
        'schema': 'method88-package-summary-v1',
        'status': 'prepared_not_run',
        'output': str(output),
        'delivery': str(delivery),
        'manifest': str(delivery / 'DELIVERY.json'),
        'manifest_sha256': sha256_file(delivery / 'DELIVERY.json'),
        'archive': str(archive),
        'archive_sha256': sha256_file(archive),
        'acceptance': str(output / ACCEPTANCE_NAME),
        'seed': seed,
        'plan_sha256': frozen.get('plan_sha256'),
        'cases': frozen.get('count'),
        'counts': frozen.get('counts'),
        'files_count': len(files_map),
        'board_tested': False,
        'hardware_rebuilt': False,
        'software_rebuilt': False,
    }


def build_arg_parser():
    parser = argparse.ArgumentParser(
        description='Package the host-side method88 role-2 delivery.',
    )
    parser.add_argument('--source-delivery', required=True)
    parser.add_argument('--selection', required=True)
    parser.add_argument('--output', required=True, metavar='NEWDIR')
    parser.add_argument('--seed', type=int, default=method88.ORDER_SEED)
    return parser


def main(argv=None):
    args = build_arg_parser().parse_args(argv)
    try:
        summary = package(args.source_delivery, args.selection, args.output,
                          seed=args.seed)
    except PackageError as exc:
        print('package_method88: %s' % exc, file=sys.stderr)
        return 2
    print(json.dumps(summary, indent=2, ensure_ascii=False, sort_keys=True))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
