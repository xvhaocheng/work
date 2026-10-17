#!/usr/bin/env python3
"""Offline fixture checks: never import UART or operate hardware."""
import copy
import json
from pathlib import Path
import subprocess
import sys
import tempfile
from types import SimpleNamespace
import unittest

import collect_e5
from test_hist_records import FIXTURE, item_from_fixture

HERE = Path(__file__).resolve().parent


def save(path, value):
    path.write_text(json.dumps(value) + '\n')


class CollectorTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(dir=HERE, prefix='.collector-fixture-')
        self.root = Path(self.temp.name)
        self.package = self.root / 'package'
        self.package.mkdir()
        (self.package / 'fixture.bit').write_bytes(b'HOST FIXTURE; NOT A BITSTREAM')
        self.delivery = {'status': 'ready_for_collection', 'kernel_release': 'FIXTURE-KERNEL-NO-BOARD',
                         'source_record': 'source.json', 'artifacts': {'ds0_bit': {'file': 'fixture.bit', 'sha256': 'b' * 64}}}
        self.source = {'actual_commits': {'platform': 'a' * 40, 'cva6': collect_e5.CVA6, 'iommu': 'c' * 40,
                                         'linux': 'd' * 40, 'opensbi': 'e' * 40}}
        self.load = {'status': 'boot_identity_verified', 'bit_sha256': 'b' * 64,
                     'kernel_release': 'FIXTURE-KERNEL-NO-BOARD', 'boot_id': 'HOST-FIXTURE-NO-BOARD'}
        self.fpga = {'status': 'passed', 'selected_cva6': collect_e5.CVA6, 'selected_iommu': 'c' * 40,
                     'parameters': {'observer_version': 4, 'PathDepth': 4, 'histogram_stages': 16, 'histogram_bins': 32,
                                    'histogram_counter_bits': 32, 'histogram_scheme': 1, 'ds_delay_enable': False,
                                    'ds_cycles': [0, 0], 'clock_hz': 50000000},
                     'timing': {'wns_ns': .1, 'setup_failing': 0, 'hold_failing': 0, 'pulse_failing': 0},
                     'routing_errors': 0, 'routed_boundary': {'passed': True, 'cpu_period_ns': 20,
                          'expected_delay_enable': 0, 'delay_cell_count': 0, 'forbidden_research_cell_count': 0},
                     'artifacts': {'fixture/ariane_xilinx.bit': 'b' * 64}}

    def tearDown(self):
        self.temp.cleanup()

    def bind(self):
        save(self.package / 'manifest.json', self.delivery)
        save(self.package / 'source.json', self.source)
        save(self.root / 'load.json', self.load)
        save(self.root / 'fpga.json', self.fpga)
        return collect_e5.identity(self.package, self.root / 'load.json', self.root / 'fpga.json')

    def test_identity_and_host_only_entry(self):
        bound = self.bind()
        self.assertTrue(bound['histogram_required'])
        self.assertEqual(bound['actual_commits'], self.source['actual_commits'])
        target = self.root / 'never-created-output'
        result = subprocess.run([sys.executable, str(HERE / 'collect_e5.py'), '--package', str(self.package),
                                 '--load-manifest', str(self.root / 'load.json'), '--fpga-manifest', str(self.root / 'fpga.json'),
                                 '--output', str(target), '--board-dir', str(self.root / 'NONEXISTENT-BOARD'),
                                 '--check-only'], capture_output=True, text=True, timeout=10)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(json.loads(result.stdout)['bit_sha256'], 'b' * 64)
        self.assertFalse(target.exists())

    def test_wrong_bit_kernel_and_short_identity_reject(self):
        for target, field, value in [(self.load, 'bit_sha256', 'f' * 64),
                                     (self.load, 'kernel_release', 'wrong'),
                                     (self.source['actual_commits'], 'linux', 'acece3a')]:
            old = target[field]; target[field] = value
            with self.subTest(field=field), self.assertRaises(ValueError): self.bind()
            target[field] = old

    def test_unaccepted_fpga_and_wrong_hist_version_depth_reject(self):
        for target, field, value in [(self.fpga, 'status', 'running'),
                                    (self.fpga['parameters'], 'observer_version', 5),
                                    (self.fpga['parameters'], 'PathDepth', 64),
                                    (self.fpga['parameters'], 'ds_cycles', [50, 50]),
                                    (self.fpga['timing'], 'wns_ns', -.1),
                                    (self.fpga, 'routing_errors', 1)]:
            old = copy.deepcopy(target[field]); target[field] = value
            with self.subTest(field=field), self.assertRaises(ValueError): self.bind()
            target[field] = old

    def test_preserves_parser_arguments_and_records_code_identity(self):
        bound = self.bind()
        calls, saved = [], []
        def parser(output, *args, **kwargs):
            calls.append((output, args, kwargs))
            return item_from_fixture()
        collector = SimpleNamespace(parse=parser, save=lambda path, value: saved.append((str(path), value)))
        collect_e5.configure(collector, bound)
        output = '\nCASE_EXIT=0\n' + FIXTURE + '\nTAINT_AFTER=0\n'
        item = collector.parse(output, 'SAME_KERNEL', True, expected_n=4096, expected_warmup=256)
        self.assertEqual(calls, [(output, ('SAME_KERNEL', True), {'expected_n': 4096, 'expected_warmup': 256})])
        self.assertEqual(len(item['histogram']['stages']), 16)
        collector.save('manifest.json', {'status': 'running'})
        self.assertEqual(saved[0][1]['e5_identity'], bound)
        self.assertTrue(saved[0][1]['histogram_required'])
        self.assertEqual(len(saved[0][1]['e5_wrapper']['sha256']), 64)
        with self.assertRaises(ValueError): collector.parse(output.replace('CASE_EXIT=0', 'CASE_EXIT=1'))


if __name__ == '__main__':
    unittest.main()
