#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Tests for scripts/composition_plan.py.

These tests exercise the frozen plan shape, the ordering/Latin rotation and the
record validator (a synthesised valid record must pass; a polluted record must
fail).  They are intentionally behaviour tests: a scientific negative such as
fast == 0 must not be reported as a technical failure.
"""
import importlib.util
import pathlib
import sys
import unittest

HERE = pathlib.Path(__file__).resolve()
REPO = HERE.parent.parent
SCRIPT = REPO / 'scripts' / 'composition_plan.py'


def load_plan_module():
    spec = importlib.util.spec_from_file_location('composition_plan_under_test', SCRIPT)
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load ' + str(SCRIPT))
    module = importlib.util.module_from_spec(spec)
    sys.modules['composition_plan_under_test'] = module
    spec.loader.exec_module(module)
    return module


plan = load_plan_module()


def make_stats():
    return {
        'submitted': 1, 'completed': 1, 'completed_bytes': 4096, 'active_cycles': 1,
        'iotlb_miss': 0, 's1_ptw': 0, 's2_ptw': 0,
        'refill_gap_ns_sum': 0, 'refill_gap_ns_max': 0, 'refill_gap_samples': 0,
        'in_flight_at_start': 0, 'in_flight_at_end': 0,
        'data_errors': 0, 'quarantined': 0,
    }


def base_record(case):
    mode = case['mode']
    a_pages = case['a_pages']
    transfers = case['transfers']
    generation = 7
    config = plan.expected_live_config(mode)

    raw = [0] * plan.RAW_WORDS
    raw[plan.RAW_MAGIC] = plan.COMPOSITION_MAGIC
    raw[plan.RAW_ABI] = plan.COMPOSITION_ABI
    raw[plan.RAW_LIVE_CONFIG] = config
    raw[plan.RAW_LIVE_GENERATION] = generation
    raw[plan.RAW_WINDOW_FLAGS] = plan.WINDOW_FLAG_FROZEN
    raw[plan.RAW_WINDOW_SEQUENCE] = 4
    raw[plan.RAW_WINDOW_CYCLES] = 2000
    raw[plan.RAW_WINDOW_ERRORS] = 0
    raw[plan.RAW_ARM_CONFIG] = config
    raw[plan.RAW_ARM_GENERATION] = generation
    raw[plan.RAW_V_LOOKUP] = 100
    raw[plan.RAW_V_HIT] = 70
    raw[plan.RAW_V_MISS] = 30
    if a_pages:
        raw[plan.RAW_A_LOOKUP] = 50
        raw[plan.RAW_A_HIT] = 20
        raw[plan.RAW_A_MISS] = 30
    raw[plan.RAW_OTHER_LOOKUP] = 12
    raw[plan.RAW_OTHER_HIT] = 5
    raw[plan.RAW_OTHER_MISS] = 7
    raw[plan.RAW_V_GAP] = case['v_gap_cycles']
    raw[plan.RAW_A_GAP] = 0
    raw[plan.RAW_FAST_N_DISABLED] = 0

    window = [0] * plan.P4M_WINDOW_WORDS
    window[0] = plan.P4M_BANK_MAGIC
    window[1] = raw[plan.RAW_WINDOW_CYCLES]
    window[2] = transfers
    window[3] = transfers
    window[13] = transfers
    window[14] = transfers
    window[15] = 0x3 if a_pages else 0x2

    observation = [0] * plan.P4M_OBS_WORDS
    observation[0] = raw[plan.RAW_WINDOW_CYCLES]
    observation[7] = transfers
    observation[8] = transfers

    experiment = {
        'abi_version': 1, 'struct_size': 1, 'status': 0,
        'victim_did': plan.PROTECTED_DID,
        'aggressor_did': (plan.AGGRESSOR_DID if a_pages else None),
        'hpm_configured': 1, 'measurement_started': 1, 'measurement_complete': 1,
        'victim_elapsed_ns': 100, 'snapshot_before_ns': 1,
        'snapshot_after_ns': 1, 'drain_ns': 1,
        'victim': make_stats(),
        'aggressor': (make_stats() if a_pages else None),
    }
    p4m = {
        'aggressor_gap_cycles': 0,
        'sequence': 4,
        'observer_status': plan.P4M_OBSERVER_STATUS_OK,
        'observation': observation,
        'window': window,
    }
    return {
        'schema': plan.RECORD_SCHEMA,
        'success': True,
        'error': None,
        'errno': 0,
        'request': {
            'mode': mode,
            'a_pages': a_pages,
            'victim_gap_cycles': case['v_gap_cycles'],
            'victim_transfers': transfers,
            'warmup_transfers': case['warmup'],
            'protected_did': plan.PROTECTED_DID,
        },
        'result': {
            'abi_version': plan.COMPOSITION_ABI,
            'status': 0, 'restore_status': 0,
            'configured': 1, 'raw_captured': 1,
            'generation': generation,
            'restored_generation': generation + 1,
            'experiment': experiment,
            'p4m': p4m,
            'raw': raw,
        },
    }


class PlanShapeTests(unittest.TestCase):
    def setUp(self):
        self.plan = plan.build_plan()
        self.cases = self.plan['cases']

    def test_total_count_is_172(self):
        self.assertEqual(self.plan['count'], 172)
        self.assertEqual(len(self.cases), 172)

    def test_phase_counts(self):
        self.assertEqual(self.plan['phases']['short'], 16)
        self.assertEqual(self.plan['phases']['noise'], 12)
        self.assertEqual(self.plan['phases']['explore'], 144)

    def test_ids_are_unique(self):
        ids = [case['id'] for case in self.cases]
        self.assertEqual(len(ids), len(set(ids)))

    def test_original_order_is_preserved(self):
        phases = [case['phase'] for case in self.cases]
        first_noise = phases.index('noise')
        first_explore = phases.index('explore')
        self.assertTrue(all(p == 'short' for p in phases[:first_noise]))
        self.assertTrue(all(p == 'noise' for p in phases[first_noise:first_explore]))
        self.assertTrue(all(p == 'explore' for p in phases[first_explore:]))

    def test_short_shape(self):
        short = [c for c in self.cases if c['phase'] == 'short']
        self.assertEqual(len(short), 16)
        for case in short:
            self.assertEqual(case['transfers'], 16)
            self.assertEqual(case['warmup'], 16)
            self.assertEqual(case['v_gap_cycles'], 0)
        self.assertEqual(len({(c['mode'], c['a_pages']) for c in short}), 16)

    def test_noise_shape(self):
        noise = [c for c in self.cases if c['phase'] == 'noise']
        self.assertEqual(len(noise), 12)
        for case in noise:
            self.assertEqual(case['mode'], plan.MODE_S)
            self.assertEqual(case['a_pages'], 8)
            self.assertEqual(case['v_gap_cycles'], 0)
            self.assertEqual(case['transfers'], 4096)
            self.assertEqual(case['warmup'], 256)
        self.assertEqual(sorted({c['pair'] for c in noise}), [1, 2, 3, 4, 5, 6])
        for pair in range(1, 7):
            self.assertEqual(sum(1 for c in noise if c['pair'] == pair), 2)

    def test_explore_shape(self):
        explore = [c for c in self.cases if c['phase'] == 'explore']
        self.assertEqual(len(explore), 144)
        combos = {(c['block'], c['v_gap_cycles'], c['a_pages'], c['mode']) for c in explore}
        self.assertEqual(len(combos), 144)

    def test_explore_rotation_is_not_always_s_first(self):
        explore = [c for c in self.cases if c['phase'] == 'explore']
        groups = {}
        for case in explore:
            key = (case['block'], case['v_gap_cycles'], case['a_pages'])
            groups.setdefault(key, []).append(case['mode'])
        self.assertEqual(len(groups), 36)
        for modes in groups.values():
            self.assertEqual(sorted(modes), [0, 1, 2, 3])
        firsts = [modes[0] for modes in groups.values()]
        self.assertTrue(any(m != 0 for m in firsts))
        self.assertEqual(set(firsts), {0, 1, 2, 3})

    def test_no_auto_rerun_semantics(self):
        self.assertIs(self.plan['auto_rerun'], False)
        self.assertIs(self.plan['resume'], False)
        for case in self.cases:
            self.assertIs(case['auto_rerun'], False)

    def test_case_argv(self):
        case = self.cases[0]
        argv = plan.case_argv(case, '/dev/idma0')
        self.assertEqual(argv[0], '--device')
        self.assertEqual(argv[argv.index('--mode') + 1], str(case['mode']))
        self.assertEqual(argv[argv.index('--a-pages') + 1], str(case['a_pages']))
        self.assertEqual(argv[argv.index('--v-gap') + 1], str(case['v_gap_cycles']))

    def test_cases_through_phase(self):
        self.assertEqual(len(plan.cases_through_phase(self.plan, 'short')), 16)
        self.assertEqual(len(plan.cases_through_phase(self.plan, 'noise')), 28)
        self.assertEqual(len(plan.cases_through_phase(self.plan, 'explore')), 172)


class ValidatorTests(unittest.TestCase):
    def setUp(self):
        self.plan = plan.build_plan()
        self.cases = self.plan['cases']
        self.solo = next(c for c in self.cases if c['a_pages'] == 0)
        self.aggr = next(c for c in self.cases if c['a_pages'] == 8)
        self.gap0 = next(c for c in self.cases if c['v_gap_cycles'] == 0)
        self.gapn = next(c for c in self.cases if c['v_gap_cycles'] == 4000)
        self.s_mode = next(c for c in self.cases if c['mode'] == plan.MODE_S)
        self.n_mode = next(c for c in self.cases if c['mode'] == plan.MODE_N)
        self.p_mode = next(c for c in self.cases if c['mode'] == plan.MODE_P)

    def meta(self):
        return {'exit': 0, 'taint_before': '0', 'taint_after': '0'}

    def test_valid_solo_passes(self):
        self.assertEqual(plan.validate_case(self.solo, base_record(self.solo), self.meta()), [])

    def test_valid_aggressor_passes(self):
        self.assertEqual(plan.validate_case(self.aggr, base_record(self.aggr), self.meta()), [])

    def test_valid_gap_case_passes(self):
        self.assertEqual(plan.validate_case(self.gapn, base_record(self.gapn), self.meta()), [])

    def test_tainted_sample_fails(self):
        meta = self.meta()
        meta['taint_after'] = '1'
        self.assertTrue(plan.validate_case(self.solo, base_record(self.solo), meta))

    def test_exit_code_fails(self):
        meta = self.meta()
        meta['exit'] = 1
        self.assertTrue(plan.validate_case(self.solo, base_record(self.solo), meta))

    def test_wrong_window_cycles_fails(self):
        record = base_record(self.solo)
        record['result']['raw'][plan.RAW_WINDOW_CYCLES] += 1
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_bad_magic_fails(self):
        record = base_record(self.solo)
        record['result']['raw'][plan.RAW_MAGIC] = 0
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_wrong_mode_config_fails(self):
        record = base_record(self.solo)
        record['result']['raw'][plan.RAW_LIVE_CONFIG] = plan.expected_live_config(
            (self.solo['mode'] + 1) % 4)
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_negative_input_param_fails(self):
        record = base_record(self.solo)
        record['request']['mode'] = -1
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_lookup_conservation_fails(self):
        record = base_record(self.solo)
        record['result']['raw'][plan.RAW_V_MISS] += 1
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_gap0_inconsistent_fails(self):
        record = base_record(self.gap0)
        record['result']['raw'][plan.RAW_V_GAP] = 5
        self.assertTrue(plan.validate_case(self.gap0, record, self.meta()))

    def test_restored_generation_must_advance(self):
        record = base_record(self.solo)
        record['result']['restored_generation'] = record['result']['generation']
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_fast_when_n_disabled_fails(self):
        record = base_record(self.s_mode)
        record['result']['raw'][plan.RAW_V_FAST] = 1
        self.assertTrue(plan.validate_case(self.s_mode, record, self.meta()))

    def test_fast_n_disabled_for_p_mode_fails(self):
        record = base_record(self.p_mode)
        record['result']['raw'][plan.RAW_P_ALLOC_VIOLATION]
        record['result']['raw'][plan.RAW_OTHER_FAST] = 2
        self.assertTrue(plan.validate_case(self.p_mode, record, self.meta()))

    def test_scientific_fast_zero_does_not_fail(self):
        case = self.n_mode
        record = base_record(case)
        self.assertEqual(plan.validate_case(case, record, self.meta()), [])

    def test_scientific_miss_zero_does_not_fail(self):
        record = base_record(self.solo)
        record['result']['raw'][plan.RAW_V_MISS] = 0
        record['result']['raw'][plan.RAW_V_HIT] = 40
        record['result']['raw'][plan.RAW_V_LOOKUP] = 40
        self.assertEqual(plan.validate_case(self.solo, record, self.meta()), [])

    def test_metrics_use_window_not_whole_round(self):
        record = base_record(self.aggr)
        metrics = plan.case_metrics(self.aggr, record)
        self.assertEqual(metrics['a_completed_jobs_window'], record['result']['p4m']['window'][3])
        self.assertEqual(metrics['v_workload_bytes'], self.aggr['transfers'] * plan.CHUNK_BYTES)
        self.assertEqual(metrics['cycle_ns'], 20)

    def test_metrics_read_the_frozen_locations(self):
        record = base_record(self.aggr)
        metrics = plan.case_metrics(self.aggr, record)
        self.assertEqual(metrics['v_window_cycles'], record['result']['p4m']['window'][1])
        self.assertEqual(metrics['v_actual_gap_cycles'],
                         record['result']['raw'][plan.RAW_V_GAP])

    def test_experiment_lives_at_result_level(self):
        record = base_record(self.solo)
        self.assertIn('experiment', record['result'])
        self.assertNotIn('experiment', record['result']['p4m'])
        self.assertNotIn('raw', record['result']['p4m'])
        self.assertEqual(plan.validate_case(self.solo, record, self.meta()), [])

    def test_malformed_result_returns_problems_not_crash(self):
        record = base_record(self.solo)
        record['result'] = ['not', 'an', 'object']
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_malformed_p4m_returns_problems_not_crash(self):
        record = base_record(self.solo)
        record['result']['p4m'] = 'not-an-object'
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_malformed_experiment_returns_problems_not_crash(self):
        record = base_record(self.solo)
        record['result']['experiment'] = None
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_malformed_raw_returns_problems_not_crash(self):
        record = base_record(self.solo)
        record['result']['raw'] = {'not': 'a list'}
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))


if __name__ == '__main__':
    unittest.main()
