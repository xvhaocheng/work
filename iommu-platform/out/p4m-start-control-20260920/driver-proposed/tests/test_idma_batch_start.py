#!/usr/bin/env python3
"""Compile and run the production bounded batch-start helpers against a host MMIO model.

This establishes software control flow only, not FPGA timing or hardware acceptance.
Only the production idma_batch_start/idma_batch_start_log helpers and their immediate
dependencies are compiled; kernel MMIO, ktime, HPM and userspace copies are stubbed.
"""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = (
    'idma_quarantine',
    'idma_submit_config', 'idma_submit_commit',
    'idma_batch_start', 'idma_batch_start_log',
)
CASES = (
    'order_both',
    'a_zero_skip_v',
    'v_zero_after_a',
    'solo_timestamps',
    'deadline_no_trigger',
    'signal_no_trigger',
    'irq_restore_paths',
    'log_contract',
)


def extract(source, pattern, label):
    matches = re.findall(pattern, source, re.MULTILINE | re.DOTALL)
    if len(matches) != 1:
        raise RuntimeError(f'Expected exactly one production {label}, got {len(matches)}')
    return matches[0]


class IdmaBatchStartTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        build = Path(cls.temp.name)
        source = Path(os.environ.get('IDMA_SOURCE',
                      ROOT / 'linux/drivers/misc/idma.c')).read_text()
        definitions = '\n'.join(re.findall(r'^#define IDMA_.*$', source, re.MULTILINE))
        structures = '\n'.join(
            extract(source, rf'^struct {name} \{{.*?^\}};', name)
            for name in ('idma_device', 'idma_stream', 'idma_batch_start')
        )
        functions = '\n'.join(
            extract(source, rf'^static [^\n]*\b{name}\(.*?^\}}', name)
            for name in FUNCTIONS
        )
        harness = (ROOT / 'tests/idma_batch_start_harness.c').read_text()
        harness = harness.replace('/* PRODUCTION_TYPES */', definitions + '\n' + structures)
        harness = harness.replace('/* PRODUCTION_FUNCTIONS */', functions)
        (build / 'linux').mkdir()
        shutil.copyfile(ROOT / 'linux/include/uapi/linux/idma.h', build / 'linux/idma.h')
        (build / 'batch_start.c').write_text(harness)
        cls.binary = build / 'batch_start'
        subprocess.run([
            os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O0', '-g', '-Wall',
            '-Wextra', '-Werror', '-I', str(build), str(build / 'batch_start.c'),
            '-o', str(cls.binary),
        ], check=True)

    def test_production_batch_start(self):
        for case in CASES:
            with self.subTest(case=case):
                result = subprocess.run([str(self.binary), case], capture_output=True,
                                        text=True, timeout=5)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn('PASS ' + case, result.stdout)


if __name__ == '__main__':
    unittest.main()
