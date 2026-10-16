#!/usr/bin/env python3
"""Compile and run the production idma observation prepare/arm helpers on a host model.

This establishes software control flow only, not FPGA timing or hardware acceptance.
Only the production idma_observation_prepare/idma_observation_arm helpers and their
immediate dependencies are compiled; kernel MMIO, the instance lookup and the
path_observe module parameter are stubbed.
"""
import os
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
STRUCTURES = ('idma_device', 'idma_observation')
FUNCTIONS = ('idma_observation_prepare', 'idma_observation_arm')
CASES = (
    'v9_prepare',
    'v9_arm',
    'v8_path',
    'v9_denials',
    'unknown_magic',
    'busy_and_clear',
    'no_instance',
)


def extract(source, pattern, label):
    matches = re.findall(pattern, source, re.MULTILINE | re.DOTALL)
    if len(matches) != 1:
        raise RuntimeError(f'Expected exactly one production {label}, got {len(matches)}')
    return matches[0]


class IdmaObservationTest(unittest.TestCase):
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
            for name in STRUCTURES
        )
        functions = '\n'.join(
            extract(source, rf'^static [^\n]*\b{name}\(.*?^\}}', name)
            for name in FUNCTIONS
        )
        harness = (ROOT / 'tests/idma_observation_harness.c').read_text()
        harness = harness.replace('/* PRODUCTION_TYPES */', definitions + '\n' + structures)
        harness = harness.replace('/* PRODUCTION_FUNCTIONS */', functions)
        (build / 'observation.c').write_text(harness)
        cls.binary = build / 'observation'
        subprocess.run([
            os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O0', '-g', '-Wall',
            '-Wextra', '-Werror', str(build / 'observation.c'), '-o', str(cls.binary),
        ], check=True)

    def test_production_observation(self):
        for case in CASES:
            with self.subTest(case=case):
                result = subprocess.run([str(self.binary), case], capture_output=True,
                                        text=True, timeout=5)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn('PASS ' + case, result.stdout)


if __name__ == '__main__':
    unittest.main()
