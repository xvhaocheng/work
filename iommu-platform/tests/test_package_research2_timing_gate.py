#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''Focused unittest tests for the package_research2 routed timing gate.

These tests do not run Vivado, flash a board or execute the packaging script.
They exercise the parser and the fail-closed validator with succinct realistic
Vivado Design Timing Summary fixtures.
'''
from __future__ import annotations

import importlib.util
import pathlib
import sys
import tempfile
import unittest


def _load_package_research2():
    here = pathlib.Path(__file__).resolve()
    root = here.parents[1]
    path = root / 'scripts' / 'package_research2.py'
    spec = importlib.util.spec_from_file_location('package_research2_timing_test', str(path))
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load %s' % path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


pkg = _load_package_research2()


def make_routed_report(wns, whs, wpws, tns, ths, tpws,
                       tns_fail, ths_fail, tpws_fail, loops):
    '''Build a succinct but realistic wrapper around a Design Timing Summary.'''
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


class RoutedTimingParserTests(unittest.TestCase):
    def test_positive_design_timing_summary_parses(self):
        report = make_routed_report('0.177', '0.028', '0.062',
                                    '0.000', '0.000', '0.000',
                                    '0', '0', '0', '0')
        summary = pkg.parse_design_timing_summary(report, 'test', 'test.rpt')
        self.assertEqual(summary['wns'], 0.177)
        self.assertEqual(summary['whs'], 0.028)
        self.assertEqual(summary['wpws'], 0.062)
        self.assertEqual(summary['tns'], 0.0)
        self.assertEqual(summary['ths'], 0.0)
        self.assertEqual(summary['tpws'], 0.0)
        self.assertEqual(summary['tns_fail'], 0)
        self.assertEqual(summary['ths_fail'], 0)
        self.assertEqual(summary['tpws_fail'], 0)

    def test_negative_setup_summary_parses_and_is_rejected(self):
        report = make_routed_report('-3.979', '0.028', '0.062',
                                    '-17075.918', '0.000', '0.000',
                                    '9454', '0', '0', '0')
        summary = pkg.parse_design_timing_summary(report, 'test', 'test.rpt')
        summary['loops'] = pkg.parse_combinational_loops(report, 'test', 'test.rpt')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')

    def test_missing_design_timing_summary_is_rejected(self):
        report = 'Clock Summary\n--------------\nclock period\n'
        with self.assertRaises(pkg.PackageError):
            pkg.parse_design_timing_summary(report, 'test', 'test.rpt')

    def test_missing_loops_is_rejected(self):
        report = make_routed_report('0.177', '0.028', '0.062',
                                    '0.000', '0.000', '0.000',
                                    '0', '0', '0', '0')
        report = report.replace('9. checking loops (0)\n', '')
        with self.assertRaises(pkg.PackageError):
            pkg.parse_combinational_loops(report, 'test', 'test.rpt')

    def test_garbled_nonfinite_wns_is_rejected(self):
        report = make_routed_report('INF', '0.028', '0.062',
                                    '0.000', '0.000', '0.000',
                                    '0', '0', '0', '0')
        with self.assertRaises(pkg.PackageError):
            pkg.parse_design_timing_summary(report, 'test', 'test.rpt')

    def test_garbled_first_row_before_positive_numeric_row_is_rejected(self):
        report = make_routed_report('0.177', '0.028', '0.062',
                                    '0.000', '0.000', '0.000',
                                    '0', '0', '0', '0')
        data_row = ('    0.177  0.000  0  100  0.028  0.000  0  100  '
                    '0.062  0.000  0  100')
        self.assertIn(data_row, report)
        report = report.replace(data_row, '    MALFORMED ROW\n' + data_row, 1)
        with self.assertRaises(pkg.PackageError):
            pkg.parse_design_timing_summary(report, 'test', 'test.rpt')

    def test_contradictory_loop_counts_are_rejected(self):
        report = make_routed_report('0.177', '0.028', '0.062',
                                    '0.000', '0.000', '0.000',
                                    '0', '0', '0', '0')
        report += '10. checking loops (1)\n'
        with self.assertRaises(pkg.PackageError):
            pkg.parse_combinational_loops(report, 'test', 'test.rpt')

    def test_zero_failures_but_negative_wns_is_rejected(self):
        report = make_routed_report('-0.001', '0.028', '0.062',
                                    '0.000', '0.000', '0.000',
                                    '0', '0', '0', '0')
        summary = pkg.parse_design_timing_summary(report, 'test', 'test.rpt')
        summary['loops'] = pkg.parse_combinational_loops(report, 'test', 'test.rpt')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')


class RoutedTimingValidatorTests(unittest.TestCase):
    def _summary(self, wns='0.177', whs='0.028', wpws='0.062',
                 tns='0.000', ths='0.000', tpws='0.000',
                 tns_fail='0', ths_fail='0', tpws_fail='0', loops='0'):
        report = make_routed_report(wns, whs, wpws, tns, ths, tpws,
                                    tns_fail, ths_fail, tpws_fail, loops)
        summary = pkg.parse_design_timing_summary(report, 'test', 'test.rpt')
        summary['loops'] = pkg.parse_combinational_loops(report, 'test', 'test.rpt')
        return summary

    def test_positive_gate_accepts(self):
        pkg.validate_routed_timing(self._summary(), 'test', 'test.rpt')

    def test_negative_hold_is_rejected(self):
        summary = self._summary(whs='-0.001')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')

    def test_negative_pulse_is_rejected(self):
        summary = self._summary(wpws='-0.001')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')

    def test_nonzero_tns_is_rejected(self):
        summary = self._summary(tns='0.001')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')

    def test_nonzero_ths_is_rejected(self):
        summary = self._summary(ths='0.001')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')

    def test_nonzero_tpws_is_rejected(self):
        summary = self._summary(tpws='0.001')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')

    def test_setup_failing_endpoint_is_rejected(self):
        summary = self._summary(tns_fail='1')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')

    def test_hold_failing_endpoint_is_rejected(self):
        summary = self._summary(ths_fail='1')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')

    def test_pulse_failing_endpoint_is_rejected(self):
        summary = self._summary(tpws_fail='1')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')

    def test_combinational_loop_is_rejected(self):
        summary = self._summary(loops='1')
        with self.assertRaises(pkg.PackageError):
            pkg.validate_routed_timing(summary, 'test', 'test.rpt')


class RoutedTimingFileTests(unittest.TestCase):
    def test_load_routed_timing_acceptance_missing_report_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaises(pkg.PackageError):
                pkg.load_routed_timing_acceptance(pathlib.Path(tmp), 'profile', 'label')

    def test_load_routed_timing_acceptance_positive_report(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = pathlib.Path(tmp)
            profile = 'test-profile'
            report_rel = pkg.TIMING_REPORT_REL_FMT.format(profile=profile)
            report_path = root.joinpath(*pathlib.PurePosixPath(report_rel).parts)
            report_path.parent.mkdir(parents=True, exist_ok=True)
            report_path.write_text(make_routed_report('0.177', '0.028', '0.062',
                                                      '0.000', '0.000', '0.000',
                                                      '0', '0', '0', '0'),
                                   encoding='utf-8')
            result = pkg.load_routed_timing_acceptance(root, profile, 'test')
            self.assertEqual(result['design_timing_summary']['wns'], 0.177)
            self.assertEqual(result['design_timing_summary']['loops'], 0)
            self.assertEqual(len(result['sha256']), 64)


if __name__ == '__main__':
    unittest.main()
