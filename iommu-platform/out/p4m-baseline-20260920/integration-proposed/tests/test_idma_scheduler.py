#!/usr/bin/env python3
"""Run production scheduler/cleanup functions against a deterministic host MMIO model.

These tests establish software control flow only, not FPGA timing or HPM validity.
Kernel DMA mapping, MMIO ordering, userspace copies and HPM ownership are stubbed.
"""
import os
from itertools import product
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = (
    'idma_pattern', 'idma_stream_cleanup', 'idma_quarantine', 'idma_submit',
    'idma_poll', 'idma_run_window', 'idma_drain', 'idma_verify',
    'idma_snapshot', 'idma_snapshot_delta', 'idma_validate',
    'idma_observation_prepare', 'idma_observation_arm', 'idma_observation_finish',
    'idma_batch_configure', 'idma_batch_stop', 'idma_batch_collect',
    'idma_batch_disable', 'idma_batch_window', 'idma_e3_finish', 'idma_stream_reseed',
    'idma_observer_indexed', 'idma_gap_preflight', 'idma_gap_program_readback',
    'idma_p4m_clear_outputs', 'idma_p4m_validate', 'idma_p4m_finish',
    'idma_run_session', 'idma_run_p4m', 'idma_run_e3_pair', 'idma_run_experiment_mode', 'idma_run_experiment',
)
CASES = (
    "queue_ok", "queue_reverse", "queue_solo", "queue_old_hw", "queue_bank_bad",
    "queue_gate_bad", "queue_insert_bad", "queue_truncated", "queue_second_bad",
    "queue_no_trigger", "queue_v5_legacy", "queue_v5_diag",
    "diag_ok", "diag_solo", "diag_no_warmup", "diag_fast_a", "diag_trigger",
    "diag_invalid", "diag_reserved", "diag_warm_timeout", "diag_second_bad",
    "diag_zero_id", "diag_interrupt",
    'pair_ok', 'pair_trigger', 'pair_reverse', 'pair_solo', 'pair_missing',
    'pair_invalid', 'pair_bad', 'pair_second_bad', 'pair_timeout',
    'path_ok', 'path_carry', 'path_missing_v3', 'path_bad_stock', 'path_bad_count', 'path_bad_area',
    'batch_ok', 'batch_busy', 'batch_no_warmup', 'batch_solo', 'batch_swapped', 'batch_missing',
    'batch_missing_observer', 'batch_timeout', 'batch_interrupt', 'batch_zero_id', 'batch_data_error',
    'p4m_ok', 'p4m_solo_ok', 'p4m_timeout', 'p4m_interrupt', 'p4m_old_hw',
    'p4m_cap_missing', 'p4m_readback_mismatch', 'p4m_solo_gap_nonzero',
    'p4m_wrong_roles', 'p4m_bad_reserved', 'p4m_bad_counts', 'p4m_bad_status',
    'p4m_bad_bank', 'p4m_bad_flags', 'p4m_bad_conservation', 'p4m_bad_carry',
    'p4m_bad_carry_done',
    'p4m_bad_ids', 'p4m_zero_a_progress_drained', 'p4m_zero_a_progress_pending',
    'p4m_strict_carry_ok', 'p4m_open_edge_ok', 'p4m_bad_unknown_flag',
    'p4m_bad_zero_cycles', 'p4m_bad_id_sequence', 'p4m_bad_id_overflow',
    'p4m_sticky_error', 'p4m_direct_finish_not_armed',
    'victim_finishes_first', 'both_ready_no_aggressor_refill', 'aggressor_refills',
    'warmup_not_counted', 'expired_before_submit', 'interrupted_before_submit',
    'timeout_then_safe_drain', 'interrupt_then_safe_drain',
    'timeout_quarantine_blocks_reuse', 'interrupt_quarantines_both',
    'zero_victim_id_retains', 'zero_aggressor_id_retains',
    'verify_source_diagnostic', 'verify_destination_diagnostic',
    'verify_stale_source_diagnostic', 'verify_stale_destination_diagnostic',
    'observer_ok', 'observer_roles_swapped', 'observer_data_error', 'observer_missing', 'observer_busy', 'observer_bad_sum',
    'observer_wrong_count', 'observer_bad_axi', 'observer_stuck', 'observer_timeout', 'observer_arm_error',
)
CACHE_CASES = ('cache_solo', 'cache_corun', 'cache_real_corruption')


def extract(source, pattern, label):
    matches = re.findall(pattern, source, re.MULTILINE | re.DOTALL)
    if len(matches) != 1:
        raise RuntimeError(f'Expected exactly one production {label}, got {len(matches)}')
    return matches[0]


class IdmaSchedulerTest(unittest.TestCase):
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
            for name in ('idma_device', 'idma_stream', 'idma_observation')
        )
        functions = '\n'.join(
            extract(source, rf'^static [^\n]*\b{name}\(.*?^\}}', name)
            for name in FUNCTIONS
        )
        harness = (ROOT / 'tests/idma_scheduler_harness.c').read_text()
        harness = harness.replace('/* PRODUCTION_TYPES */', definitions + '\n' + structures)
        harness = harness.replace('/* PRODUCTION_FUNCTIONS */', functions)
        (build / 'linux').mkdir()
        shutil.copyfile(ROOT / 'linux/include/uapi/linux/idma.h', build / 'linux/idma.h')
        (build / 'scheduler.c').write_text(harness)
        cls.binary = build / 'scheduler'
        subprocess.run([
            os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O0', '-g', '-Wall',
            '-Wextra', '-Werror', '-I', str(build), str(build / 'scheduler.c'),
            '-o', str(cls.binary),
        ], check=True)

    def test_production_control_flow(self):
        for case in CASES:
            with self.subTest(case=case):
                result = subprocess.run([str(self.binary), case], capture_output=True,
                                        text=True, timeout=5)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn('PASS ' + case, result.stdout)

    def test_small_chunk_device_validation(self):
        cases = ('small_chunk_copy', 'small_chunk_wrong_source')
        for chunk, victim_did, aggressor_did, case in product(
                (1, 2, 4, 8), range(10, 14), range(10, 14), cases):
            if victim_did == aggressor_did:
                continue
            with self.subTest(chunk=chunk, victim_did=victim_did,
                              aggressor_did=aggressor_did, case=case):
                result = subprocess.run(
                    [str(self.binary), case, str(chunk), str(victim_did), str(aggressor_did)],
                    capture_output=True, text=True, timeout=5)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn('PASS ' + case, result.stdout)

    def test_cache_visibility(self):
        for case in CACHE_CASES:
            with self.subTest(case=case):
                result = subprocess.run([str(self.binary), case], capture_output=True,
                                        text=True, timeout=5)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertIn('PASS ' + case, result.stdout)


if __name__ == '__main__':
    unittest.main()
