#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''Host-side tests for scripts/package_method88.py.'''
from __future__ import annotations

import hashlib
import importlib.util
import json
import pathlib
import shutil
import sys
import tarfile
import tempfile
import unittest
from unittest import mock

HERE = pathlib.Path(__file__).resolve()
ROOT = HERE.parent.parent
SCRIPTS = ROOT / 'scripts'


def _load_module(module_name, filename):
    spec = importlib.util.spec_from_file_location(module_name, SCRIPTS / filename)
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module


package_method88 = _load_module('test_package_method88_target', 'package_method88.py')
mac_method88 = package_method88.mac_method88
method88 = package_method88.method88


def _make_valid_selection():
    plan2 = method88.plan2
    configs = {}
    for method in method88.METHODS:
        if method == method88.REFERENCE_TAG:
            entry = plan2._unlimited_config(plan2.target_bytes_for(method88.TARGET))
            entry['gap_cycles'] = 2000
            entry['candidate_id'] = 'cal-fixedgap-g2000'
        else:
            mode = int(method[1:])
            entry = dict(plan2.calibration_candidates(mode)[0],
                         target_bytes=plan2.target_bytes_for(method88.TARGET))
            entry['gap_cycles'] = 0
            entry['candidate_id'] = 'cal-m%d-t%d-c0' % (mode, method88.TARGET)
        entry['matched'] = True
        configs[method] = {str(method88.TARGET): entry}
    return {
        'schema': plan2.SELECTION_SCHEMA,
        'version': plan2.SELECTION_VERSION,
        'targets': [method88.TARGET],
        'window_cycles': method88.WINDOW_CYCLES,
        'configs': configs,
        'unmatched': [],
    }


class PackageMethod88Tests(unittest.TestCase):
    def setUp(self):
        self.tmp = pathlib.Path(tempfile.mkdtemp(prefix='method88-package-test-'))
        self.addCleanup(shutil.rmtree, self.tmp, ignore_errors=True)
        self.dummy_software = {
            'kernel_release': 'dummy-kernel',
            'cli_sha256': 'dummy-cli',
            'board_cli_path': '/usr/bin/idma_research',
        }
        self.dummy_bit = b'method88-dummy-role2-bit\n' * 97
        self.dummy_sha = hashlib.sha256(self.dummy_bit).hexdigest()
        self.patches = [
            mock.patch.object(mac_method88, 'ROLE2_BIT_SHA256', self.dummy_sha),
            mock.patch.object(mac_method88, 'EXPECTED_SOFTWARE', dict(self.dummy_software)),
            mock.patch.object(package_method88, 'EXPECTED_ROLE2_BIT_BYTES', len(self.dummy_bit)),
        ]
        for patcher in self.patches:
            patcher.start()
            self.addCleanup(patcher.stop)

    def _source_delivery(self, bit_bytes=None, software=None, role2_sha=None):
        source = self.tmp / 'source'
        delivery = source / 'delivery'
        (delivery / 'artifacts').mkdir(parents=True)
        bit_bytes = self.dummy_bit if bit_bytes is None else bit_bytes
        (delivery / 'artifacts' / 'role2.bit').write_bytes(bit_bytes)
        software = dict(self.dummy_software if software is None else software)
        software.setdefault('cli_path', package_method88.EXPECTED_SOURCE_SOFTWARE_EXTRA['cli_path'])
        software.setdefault('payload_sha256', package_method88.EXPECTED_SOURCE_SOFTWARE_EXTRA['payload_sha256'])
        role2_sha = self.dummy_sha if role2_sha is None else role2_sha
        manifest = {
            'schema': package_method88.EXPECTED_OLD_SCHEMA,
            'images': [
                {'role': 1, 'bit': 'artifacts/role1.bit', 'sha256': '0' * 64},
                {'role': 2, 'bit': 'artifacts/role2.bit', 'sha256': role2_sha},
            ],
            'software': software,
        }
        (delivery / 'DELIVERY.json').write_text(json.dumps(manifest), encoding='utf-8')
        return delivery

    def _selection(self):
        selection = self.tmp / 'selection.json'
        selection.write_text(json.dumps(_make_valid_selection()), encoding='utf-8')
        return selection

    def test_refuses_existing_output_and_preserves_marker(self):
        source = self._source_delivery()
        selection = self._selection()
        output = self.tmp / 'existing-output'
        output.mkdir()
        marker = output / 'marker.txt'
        marker.write_text('keep\n', encoding='utf-8')
        with self.assertRaises(package_method88.PackageError):
            package_method88.package(source, selection, output, seed=method88.ORDER_SEED,
                                     repo_root=ROOT)
        self.assertEqual(marker.read_text(encoding='utf-8'), 'keep\n')
        self.assertFalse((output / 'delivery').exists())

    def test_source_bit_tamper_is_refused(self):
        source = self._source_delivery(bit_bytes=b'tampered\n')
        selection = self._selection()
        output = self.tmp / 'tampered-output'
        with self.assertRaises(package_method88.PackageError):
            package_method88.package(source, selection, output, seed=method88.ORDER_SEED,
                                     repo_root=ROOT)
        self.assertFalse(output.exists())

    def test_package_shape_role2_only_and_frozen_counts(self):
        source = self._source_delivery()
        selection = self._selection()
        output = self.tmp / 'package-output'
        package_method88.package(source, selection, output, seed=method88.ORDER_SEED,
                                 repo_root=ROOT)
        delivery = output / 'delivery'
        manifest = json.loads((delivery / 'DELIVERY.json').read_text(encoding='utf-8'))
        self.assertEqual(manifest['schema'], 'method88-delivery-v1')
        self.assertEqual(manifest['software'], self.dummy_software)
        self.assertEqual(len(manifest['images']), 1)
        self.assertEqual(manifest['images'][0]['role'], 2)
        self.assertEqual(manifest['images'][0]['bit'], 'artifacts/role2.bit')
        self.assertEqual(manifest['images'][0]['sha256'], self.dummy_sha)
        self.assertNotIn('artifacts/role1.bit', manifest['files'])
        self.assertNotIn('artifacts/fw_payload.bin', manifest['files'])
        self.assertNotIn('artifacts/idma_research', manifest['files'])
        self.assertEqual(set(manifest['files']), set(mac_method88.REQUIRED_FILES))
        frozen_plan = json.loads((delivery / 'frozen' / 'plan.json').read_text(encoding='utf-8'))
        self.assertEqual(frozen_plan['count'], 88)
        self.assertEqual(frozen_plan['counts']['short'], 4)
        self.assertEqual(frozen_plan['counts']['long'], 84)
        self.assertEqual((delivery / 'frozen' / 'selection.json').read_bytes(),
                         selection.read_bytes())
        self.assertTrue((output / 'README.md').is_file())
        self.assertTrue((output / package_method88.SHA256SUMS_NAME).is_file())
        self.assertTrue((output / package_method88.ACCEPTANCE_NAME).is_file())
        acceptance = json.loads((output / package_method88.ACCEPTANCE_NAME).read_text(encoding='utf-8'))
        self.assertEqual(acceptance['checks']['frozen_plan_cases'], 88)
        self.assertFalse(acceptance['limits']['board_tested'])
        self.assertFalse(acceptance['limits']['hardware_rebuilt'])
        self.assertFalse(acceptance['limits']['software_rebuilt'])

    def test_archive_members_match_runtime_hashes_and_are_regular_files(self):
        source = self._source_delivery()
        selection = self._selection()
        output = self.tmp / 'archive-output'
        package_method88.package(source, selection, output, seed=method88.ORDER_SEED,
                                 repo_root=ROOT)
        archive = output / package_method88.ARCHIVE_NAME
        expected = {}
        sums_path = output / package_method88.SHA256SUMS_NAME
        sums_member = package_method88.ARCHIVE_PREFIX + '/' + package_method88.SHA256SUMS_NAME
        expected[sums_member] = hashlib.sha256(sums_path.read_bytes()).hexdigest()
        listed_members = set()
        for line in sums_path.read_text(encoding='utf-8').splitlines():
            if not line.strip():
                continue
            digest, rel = line.split('  ', 1)
            member = package_method88.ARCHIVE_PREFIX + '/' + rel
            self.assertNotEqual(member, sums_member)
            listed_members.add(member)
            expected[member] = digest
        self.assertNotIn(sums_member, listed_members)
        with tarfile.open(archive, 'r:gz') as tar:
            members = tar.getmembers()
            self.assertEqual(sorted(member.name for member in members), sorted(expected))
            for member in members:
                self.assertFalse(member.issym())
                self.assertFalse(member.islnk())
                self.assertTrue(member.isfile())
                extracted = tar.extractfile(member)
                self.assertIsNotNone(extracted)
                actual = hashlib.sha256(extracted.read()).hexdigest()
                self.assertEqual(actual, expected[member.name])

    def test_source_is_read_only_and_no_symlink_paths_escape(self):
        source = self._source_delivery()
        selection = self._selection()
        before_bit = (source / 'artifacts' / 'role2.bit').read_bytes()
        before_selection = selection.read_bytes()
        output = self.tmp / 'read-only-output'
        package_method88.package(source, selection, output, seed=method88.ORDER_SEED,
                                 repo_root=ROOT)
        self.assertEqual((source / 'artifacts' / 'role2.bit').read_bytes(), before_bit)
        self.assertEqual(selection.read_bytes(), before_selection)
        output_resolved = output.resolve()
        for path in output.rglob('*'):
            self.assertFalse(path.is_symlink(), 'unexpected symlink: %s' % path)
            resolved = path.resolve()
            self.assertTrue(resolved == output_resolved or output_resolved in resolved.parents)


if __name__ == '__main__':
    unittest.main()
