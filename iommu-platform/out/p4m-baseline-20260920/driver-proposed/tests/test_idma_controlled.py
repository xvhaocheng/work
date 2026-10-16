#!/usr/bin/env python3
"""Compile and run production controlled-E3 helpers against a host MMIO model.

This establishes software control flow only.  It is not FPGA timing, IOTLB,
cache or hardware acceptance evidence.  Kernel DMA mapping, MMIO ordering,
userspace copies, HPM ownership, the global S1 flush and the complex observer
finishers (idma_observation_finish / idma_e3_finish) plus idma_stream_prepare
are replaced by harness stubs; every other production helper is compiled
unchanged.

The harness models the real contract of dma_controlled_launcher.sv and
idma_reg64_frontend.sv: ABORT never fabricates completion, held reservations
are cancelled (ID retired without a copy), COUNT offers that are never accepted
cannot be cancelled, the status word is held[11:8]/active[15:12]/PREP/RUNNING/
FROZEN/ABORTED/error[63:32], attached A is released by the E3 select tap on the
A read, and reserved IDs come from a per-frontend monotonic generator.  ABORT
observed in PREP escalates the launcher to RUNNING and masks a still-registered
GO pulse.
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
    'idma_pattern', 'idma_stream_cleanup', 'idma_quarantine', 'idma_submit',
    'idma_poll', 'idma_drain', 'idma_verify',
    'idma_snapshot', 'idma_snapshot_delta',
    'idma_observation_prepare', 'idma_observation_arm',
    'idma_stream_reseed',
    'idma_launcher_terminal', 'idma_launcher_held', 'idma_launcher_active',
    'idma_launcher_read_words', 'idma_observer_indexed',
    'idma_controlled_validate', 'idma_controlled_environment',
    'idma_controlled_prep_task', 'idma_controlled_account',
    'idma_controlled_apply', 'idma_controlled_validate_success',
    'idma_controlled_cleanup', 'idma_controlled_formal',
    'idma_controlled_one_round', 'idma_run_e3_controlled',
)
CASES = (
    'invalid_before_mmio',
    'cold_solo_ok',
    'v7_controlled_ok',
    'hot_pair_lane',
    'submit_counts',
    'two_round_reset',
    'v_done_late_observer',
    'no_terminal_quarantine',
    'first_error_preserved',
    'a_never_accepts_quarantine',
    'a_never_completes_quarantine',
    'prep_quarantine_visible',
    'partial_prepare_zero_a',
    'zero_id_v',
    'bad_count_quarantine',
    'bad_id_quarantine',
    'bad_mask_quarantine',
    'go_error_hpm_after_drain',
    'preflight_busy_reject',
    'partial_held_abort',
)


def extract(source, pattern, label):
    matches = re.findall(pattern, source, re.MULTILINE | re.DOTALL)
    if len(matches) != 1:
        raise RuntimeError(
            f'Expected exactly one production {label}, got {len(matches)}')
    return matches[0]


class IdmaControlledTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        build = Path(cls.temp.name)
        source = Path(os.environ.get(
            'IDMA_SOURCE', ROOT / 'linux/drivers/misc/idma.c')).read_text()
        definitions = '\n'.join(
            re.findall(r'^#define IDMA_.*$', source, re.MULTILINE))
        structures = '\n'.join(
            extract(source, rf'^struct {name} \{{.*?^\}};', name)
            for name in ('idma_device', 'idma_stream', 'idma_observation')
        )
        functions = '\n'.join(
            extract(source, rf'^static [^\n]*\b{name}\(.*?^\}}', name)
            for name in FUNCTIONS
        )
        harness = (ROOT / 'tests/idma_controlled_harness.c').read_text()
        harness = harness.replace('/* PRODUCTION_TYPES */',
                                  definitions + '\n' + structures)
        harness = harness.replace('/* PRODUCTION_FUNCTIONS */', functions)
        (build / 'linux').mkdir()
        shutil.copyfile(ROOT / 'linux/include/uapi/linux/idma.h',
                        build / 'linux/idma.h')
        (build / 'controlled.c').write_text(harness)
        cls.binary = build / 'controlled'
        subprocess.run([
            os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O0', '-g', '-Wall',
            '-Wextra', '-Werror', '-I', str(build), str(build / 'controlled.c'),
            '-o', str(cls.binary),
        ], check=True)

    def test_production_controlled_control_flow(self):
        for case in CASES:
            with self.subTest(case=case):
                result = subprocess.run(
                    [str(self.binary), case], capture_output=True,
                    text=True, timeout=10)
                self.assertEqual(result.returncode, 0,
                                 result.stdout + result.stderr)
                self.assertIn('PASS ' + case, result.stdout)


if __name__ == '__main__':
    unittest.main()
