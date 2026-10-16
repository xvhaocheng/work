#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''Focused unittest tests for the role 2 post-route finalizer.

These tests do not run Vivado, flash a board or execute the real packaging
pipeline.  They build small self-consistent fixtures in temporary directories
and exercise the fail-closed validation and the additive manifest writer.
'''
from __future__ import annotations

import hashlib
import importlib.util
import json
import pathlib
import sys
import tempfile
import unittest
import contextlib
from unittest import mock


def _load_finalizer():
    here = pathlib.Path(__file__).resolve()
    root = here.parents[1]
    path = root / 'scripts' / 'finalize_research2_postroute.py'
    spec = importlib.util.spec_from_file_location('finalize_research2_postroute_test', str(path))
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load %s' % path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


fin = _load_finalizer()


def _sha256(path):
    digest = hashlib.sha256()
    with open(str(path), 'rb') as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b''):
            digest.update(chunk)
    return digest.hexdigest()


def _write_json(path, obj):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(obj, indent=2, sort_keys=True) + '\n', encoding='utf-8')


def make_routed_report(wns='0.177', whs='0.028', wpws='0.062',
                       tns='0.000', ths='0.000', tpws='0.000',
                       tns_fail='0', ths_fail='0', tpws_fail='0', loops='0'):
    lines = [
        'Report : timing -verbose -max_paths 1',
        'Design Timing Summary',
        '---------------------',
        '',
        '    WNS(ns)      TNS(ns)  TNS Failing Endpoints  TNS Total Endpoints      WHS(ns)      THS(ns)  THS Failing Endpoints  THS Total Endpoints     WPWS(ns)     TPWS(ns)  TPWS Failing Endpoints  WPWS Total Endpoints',
        '    -------      -------  ---------------------  -------------------      -------      -------  ---------------------  -------------------     --------     --------  ----------------------  --------------------',
        '    %s  %s  %s  100  %s  %s  %s  100  %s  %s  %s  100' % (
            wns, tns, tns_fail, whs, ths, ths_fail, wpws, tpws, tpws_fail),
        '',
        '1. checking no_clock (0)',
        '9. checking loops (%s)' % loops,
    ]
    return '\n'.join(lines) + '\n'


def _build_valid_fixture(repo, expected_platform='test-platform',
                         expected_cva6_commit='test-cva6',
                         execution_exit_code=0,
                         optimized_timing_report=None,
                         base_timing_report=None):
    optimization_dir = repo / fin.POSTROUTE_DIR_REL
    optimization_dir.mkdir(parents=True, exist_ok=True)
    base_manifest_path = repo / 'out/manifests/fpga-migration-4dma-research-method-v2.json'
    checkpoint_path = repo / 'out/migration-4dma-research-method-v2/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_routed.dcp'
    base_timing_path = repo / 'out/migration-4dma-research-method-v2/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx_timing_summary_routed.rpt'
    tcl_path = optimization_dir / 'optimize.tcl'
    sources_lock_path = repo / 'sources.lock.json'
    bit_path = optimization_dir / 'ariane_xilinx.bit'
    mcs_path = optimization_dir / 'ariane_xilinx.mcs'
    dcp_path = optimization_dir / 'optimized.dcp'
    utilization_path = optimization_dir / 'utilization.rpt'
    timing_path = optimization_dir / 'timing.rpt'

    checkpoint_path.parent.mkdir(parents=True, exist_ok=True)
    checkpoint_path.write_bytes(b'checkpoint-data')
    base_timing_path.parent.mkdir(parents=True, exist_ok=True)
    if base_timing_report is None:
        base_timing_report = make_routed_report(wns='-0.150', tns='-0.179',
                                                tns_fail='2', whs='0.028',
                                                wpws='0.062', ths='0.000',
                                                tpws='0.000', ths_fail='0',
                                                tpws_fail='0', loops='0')
    base_timing_path.write_text(base_timing_report, encoding='utf-8')
    tcl_path.write_text('open_checkpoint ...\n', encoding='utf-8')
    dcp_path.write_bytes(b'optimized-dcp')
    bit_path.write_bytes(b'bit-data')
    mcs_path.write_bytes(b'mcs-data')
    utilization_path.write_text('utilization report\n', encoding='utf-8')
    if optimized_timing_report is None:
        optimized_timing_report = make_routed_report()
    timing_path.write_text(optimized_timing_report, encoding='utf-8')

    lock_data = {'schema': 1, 'components': [{'path': 'cva6', 'commit': expected_cva6_commit}]}
    _write_json(sources_lock_path, lock_data)
    lock_sha = _sha256(sources_lock_path)

    base_manifest = {
        'status': 'passed',
        'platform_commit': expected_platform,
        'profile': {'research_role': 2},
        'sources_lock_sha256': lock_sha,
    }
    _write_json(base_manifest_path, base_manifest)

    def entry(path):
        return {'path': path.relative_to(repo).as_posix(),
                'sha256': _sha256(path),
                'bytes': path.stat().st_size}

    input_data = {
        'base_manifest': entry(base_manifest_path),
        'input_checkpoint': entry(checkpoint_path),
        'tcl': entry(tcl_path),
        'base_timing': entry(base_timing_path),
    }
    _write_json(optimization_dir / 'input.json', input_data)
    _write_json(optimization_dir / 'execution.json',
                {'session_id': 46536, 'exit_code': execution_exit_code})
    return {
        'repo': repo,
        'platform_commit': expected_platform,
        'cva6_commit': expected_cva6_commit,
        'lock_sha256': lock_sha,
        'checkpoint_path': checkpoint_path,
        'base_manifest_path': base_manifest_path,
        'base_timing_path': base_timing_path,
        'tcl_path': tcl_path,
        'bit_path': bit_path,
        'timing_path': timing_path,
        'optimization_dir': optimization_dir,
    }


class FinalizePostrouteTests(unittest.TestCase):
    def test_valid_fixture_writes_additive_manifest(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo)
            fin.finalize(
                repo,
                expected_platform_commit=fixture['platform_commit'],
                expected_lock_sha256=fixture['lock_sha256'],
                expected_cva6_commit=fixture['cva6_commit'],
                expected_session_id=46536)
            manifest_path = repo / fin.POSTROUTE_MANIFEST_REL
            self.assertTrue(manifest_path.is_file())
            data = json.loads(manifest_path.read_text(encoding='utf-8'))
            self.assertEqual(data['schema'], fin.POSTROUTE_SCHEMA)
            self.assertEqual(data['action'], fin.POSTROUTE_ACTION)
            self.assertEqual(data['status'], 'passed')
            self.assertEqual(data['outputs']['bit']['sha256'], _sha256(fixture['bit_path']))
            self.assertTrue(data['outputs']['timing']['accepted'])
            self.assertGreaterEqual(data['outputs']['timing']['design_timing_summary']['wns'], 0)
            self.assertFalse(data['base']['base_timing']['accepted'])

    def test_missing_input_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            optimization_dir = repo / fin.POSTROUTE_DIR_REL
            optimization_dir.mkdir(parents=True)
            with self.assertRaises(fin.pkg.PackageError):
                fin.finalize(repo)

    def test_execution_nonzero_exit_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo, execution_exit_code=1)
            with self.assertRaises(fin.pkg.PackageError):
                fin.finalize(
                    repo,
                    expected_platform_commit=fixture['platform_commit'],
                    expected_lock_sha256=fixture['lock_sha256'],
                    expected_cva6_commit=fixture['cva6_commit'])

    def test_negative_optimized_timing_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(
                repo, optimized_timing_report=make_routed_report(wns='-0.001'))
            with self.assertRaises(fin.pkg.PackageError):
                fin.finalize(
                    repo,
                    expected_platform_commit=fixture['platform_commit'],
                    expected_lock_sha256=fixture['lock_sha256'],
                    expected_cva6_commit=fixture['cva6_commit'])

    def test_existing_manifest_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo)
            (repo / fin.POSTROUTE_MANIFEST_REL).write_text('{}', encoding='utf-8')
            with self.assertRaises(fin.pkg.PackageError):
                fin.finalize(
                    repo,
                    expected_platform_commit=fixture['platform_commit'],
                    expected_lock_sha256=fixture['lock_sha256'],
                    expected_cva6_commit=fixture['cva6_commit'])

    def test_input_hash_mismatch_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo)
            fixture['checkpoint_path'].write_bytes(fixture['checkpoint_path'].read_bytes() + b'x')
            with self.assertRaises(fin.pkg.PackageError):
                fin.finalize(
                    repo,
                    expected_platform_commit=fixture['platform_commit'],
                    expected_lock_sha256=fixture['lock_sha256'],
                    expected_cva6_commit=fixture['cva6_commit'])


class FinalizePackageIntegrationTests(unittest.TestCase):
    def _finalize(self, repo, fixture):
        fin.finalize(
            repo,
            expected_platform_commit=fixture['platform_commit'],
            expected_lock_sha256=fixture['lock_sha256'],
            expected_cva6_commit=fixture['cva6_commit'],
            expected_session_id=46536)

    def _patched_constants(self, repo, fixture):
        pkg = fin.pkg
        return [
            mock.patch.object(pkg, 'EXPECTED_METHOD_PLATFORM_COMMIT', fixture['platform_commit']),
            mock.patch.object(pkg, 'EXPECTED_METHOD_CVA6_COMMIT', fixture['cva6_commit']),
            mock.patch.object(pkg, 'EXPECTED_METHOD_LOCK_SHA256', fixture['lock_sha256']),
            mock.patch.object(pkg, 'EXPECTED_POSTROUTE_BASE_MANIFEST_SHA256', _sha256(fixture['base_manifest_path'])),
            mock.patch.object(pkg, 'EXPECTED_POSTROUTE_CHECKPOINT_SHA256', _sha256(fixture['checkpoint_path'])),
            mock.patch.object(pkg, 'EXPECTED_POSTROUTE_TCL_SHA256', _sha256(fixture['tcl_path'])),
            mock.patch.object(pkg, 'EXPECTED_POSTROUTE_BASE_TIMING_SHA256', _sha256(fixture['base_timing_path'])),
            mock.patch.object(pkg, 'EXPECTED_POSTROUTE_SESSION_ID', 46536),
        ]

    @contextlib.contextmanager
    def _loaded(self, repo, fixture):
        self._finalize(repo, fixture)
        patches = self._patched_constants(repo, fixture)
        for patch in patches:
            patch.start()
        try:
            yield fin.pkg
        finally:
            for patch in reversed(patches):
                patch.stop()

    def _assert_package_rejects(self, repo, fixture, tamper):
        self._finalize(repo, fixture)
        tamper()
        patches = self._patched_constants(repo, fixture)
        for patch in patches:
            patch.start()
        try:
            with self.assertRaises(fin.pkg.PackageError):
                fin.pkg.load_postroute_manifest(repo)
        finally:
            for patch in reversed(patches):
                patch.stop()

    def test_finalize_then_package_load_is_serializable(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo)
            with self._loaded(repo, fixture) as pkg:
                postroute = pkg.load_postroute_manifest(repo)
                optimized = {
                    'dcp': pkg.postroute_output_record(postroute['outputs']['optimized_dcp']),
                    'utilization_report': pkg.postroute_output_record(postroute['outputs']['utilization']),
                    'timing_report': pkg.postroute_output_record(postroute['outputs']['timing']),
                }
                text = json.dumps(optimized, sort_keys=True)
                self.assertIn('source_path', text)
                self.assertNotIn('resolved', text)
                self.assertNotIn('path', optimized['dcp'])
                self.assertEqual(set(optimized['dcp']), {'source_path', 'sha256', 'bytes'})

    def test_package_rejects_tampered_input_json(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo)
            def tamper():
                input_path = repo / fin.POSTROUTE_INPUT_REL
                data = json.loads(input_path.read_text(encoding='utf-8'))
                data['base_manifest']['bytes'] += 1
                input_path.write_text(json.dumps(data, indent=2, sort_keys=True) + '\n', encoding='utf-8')
            self._assert_package_rejects(repo, fixture, tamper)

    def test_package_rejects_tampered_checkpoint(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo)
            self._assert_package_rejects(
                repo, fixture,
                lambda: fixture['checkpoint_path'].write_bytes(fixture['checkpoint_path'].read_bytes() + b'x'))

    def test_package_rejects_tampered_tcl(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo)
            self._assert_package_rejects(
                repo, fixture,
                lambda: fixture['tcl_path'].write_bytes(fixture['tcl_path'].read_bytes() + b'x'))

    def test_package_rejects_tampered_execution(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo)
            def tamper():
                path = repo / fin.POSTROUTE_EXECUTION_REL
                data = json.loads(path.read_text(encoding='utf-8'))
                data['exit_code'] = 1
                path.write_text(json.dumps(data, indent=2, sort_keys=True) + '\n', encoding='utf-8')
            self._assert_package_rejects(repo, fixture, tamper)

    def test_package_rejects_tampered_timing(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            fixture = _build_valid_fixture(repo)
            self._assert_package_rejects(
                repo, fixture,
                lambda: fixture['timing_path'].write_bytes(fixture['timing_path'].read_bytes() + b'x'))


if __name__ == '__main__':
    unittest.main()
