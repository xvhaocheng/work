#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Focused tests for the optional v4 diagnostic bank in scripts/analyze_nonblock_pilot.py.

Covers, without mirroring the whole analyzer:
  * diagnostic validity (a clean bank passes),
  * boundary checks (flags overflow bit, uint64 / bool range),
  * mismatches (magic / mode / seq / cycles / fast_accept / core_fast_sum),
  * scientific semantics: core fast counts all zero must NOT fail a case.

No hardware, no runner import, no device writes. These tests build a synthetic
single-case directory tree in a temp dir and call analyzer functions directly.
"""

from __future__ import annotations

import importlib.util
import json
import pathlib
import sys
import tempfile
import unittest

_HERE = pathlib.Path(__file__).resolve()
_ANALYZER_PATH = _HERE.parent.parent / 'scripts' / 'analyze_nonblock_pilot.py'

_spec = importlib.util.spec_from_file_location(
    'analyze_nonblock_pilot', _ANALYZER_PATH)
assert _spec is not None and _spec.loader is not None
an_mod = importlib.util.module_from_spec(_spec)
sys.modules.setdefault('analyze_nonblock_pilot', an_mod)
_spec.loader.exec_module(an_mod)

P4M_BANK_MAGIC = an_mod.P4M_BANK_MAGIC
DIAG_MAGIC = an_mod.DIAG_MAGIC
HELPER_MAGIC = an_mod.HELPER_MAGIC

# A real banked-serial long-r1-a1 core window (fast counts all zero).
CORE_WORDS = [
    5279429606066244609, 1, 1, 4, 5, 5581750,
    0, 0, 0, 0, 0,          # fast 6..10 (all zero)
    16384, 16383, 0,        # normal 11..13
    32767,                  # accepted 14
    0,                      # errors 15
]

RAW_WINDOW = [
    5279429606098881793, 5581750, 4096, 4096, 1, 1, 1, 0,
    51, 4147, 51, 4146, 0, 4096, 4096, 7,
]

RAW_OBSERVATION = [
    5581750, 5577655, 0, 4095, 1406, 0, 1, 4096, 4096, 1, 0, 8192, 8191,
    5565367, 8192, 8192, 8192, 8192, 0, 0, 8192, 8192, 8192, 8192, 0, 0,
    16384, 16384, 0, 5384641, 0, 131074, 0, 131074, 16530,
]


def good_diag_words():
    """seen bits 0..3 set, one DC MSI reject, fast_accepts == 0 == Σcore fast."""
    seen = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)
    return [
        DIAG_MAGIC, 1, 1, 4, 5, 5581750,
        seen,
        1,   # probe_starts
        1,   # dc_evaluations
        0,   # dc_good
        0,   # leaf_evaluations
        0,   # leaf_good
        1,   # dc_msi_rejects
        0,   # leaf_msi_rejects
        0,   # leaf_lookup_misses
        0,   # fast_accepts
    ]


def diag_object(words=None, core_fast_sum=0, with_core_fast_sum=True):
    obj = {'words': list(good_diag_words() if words is None else words)}
    if with_core_fast_sum:
        obj['core_fast_sum'] = core_fast_sum
    return obj


def _write_json(path, obj):
    path.write_text(json.dumps(obj), encoding='utf-8')


def _make_case(root, spec, diag_words=None, include_diagnostic=True,
               helper_words=None, core_fast_sum=0):
    case_dir = pathlib.Path(root) / 'cases' / spec['id']
    case_dir.mkdir(parents=True, exist_ok=True)
    core = list(helper_words if helper_words is not None else CORE_WORDS)

    _write_json(case_dir / 'case.json', {
        'exit': 0, 'framed_exit': 0, 'taint_before': '0', 'taint_after': '0',
        'problems': [],
    })
    (case_dir / 'exit.txt').write_text('0\n', encoding='utf-8')
    (case_dir / 'helper.exit.txt').write_text('0\n', encoding='utf-8')

    _write_json(case_dir / 'metrics.json', {
        'a_completed_jobs_window': 4096, 'condition': spec['condition'],
        'group': spec['group'], 'round': spec['round'],
        'transfers': spec['transfers'], 'warmup': spec['warmup'],
        'v_window_cycles': 5581750,
    })

    _write_json(case_dir / 'record.json', {
        'schema': 'idma.p4m.v1', 'success': True, 'error': None,
        'result': {
            'status': 0,
            'victim': {'data_errors': 0, 'quarantined': 0},
            'aggressor': {'data_errors': 0, 'quarantined': 0},
        },
        'p4m': {
            'bank_magic': P4M_BANK_MAGIC,
            'observer_status': 4,
            'raw_window': list(RAW_WINDOW),
            'raw_observation': list(RAW_OBSERVATION),
            'cycles': 5581750,
            'closed': True,
            'has_a': True,
            'a_completed_jobs_window': 4096,
        },
    })

    helper = {
        'abi': 'idma-nonblock-window-v1',
        'words': core,
        'mode': 1,
        'cycles': 5581750,
        'windowseq': 5,
        'fastcounts': [0, 0, 0, 0, 0],
        'normalcounts': [16384, 16383, 0],
    }
    if include_diagnostic:
        helper['diagnostic'] = diag_object(diag_words, core_fast_sum=core_fast_sum)
    _write_json(case_dir / 'helper.json', helper)

    _write_json(case_dir / 'helper-window.json', {
        'schema': 'nonblock-pilot-helper-window-v1',
        'window': core,
    })
    return case_dir


LONG_A1_SPEC = {
    'id': 'long-r1-a1', 'order': 5, 'group': 'long', 'condition': 'A1',
    'round': 1, 'transfers': 4096, 'warmup': 256,
}


class DiagnosticValidationTests(unittest.TestCase):
    def test_valid_diagnostic_passes(self):
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(), CORE_WORDS)
        self.assertEqual(problems, [])

    def test_magic_and_mode_mismatch(self):
        words = good_diag_words()
        words[0] = 0
        words[2] = 2  # wrong mode for banked-serial
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words}, CORE_WORDS)
        self.assertTrue(any('magic' in p for p in problems))
        self.assertTrue(any('mode' in p for p in problems))

    def test_uint64_range_and_bool_rejected(self):
        neg = good_diag_words()
        neg[5] = -1
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': neg}, CORE_WORDS)
        self.assertTrue(any('uint64' in p for p in problems))

        boolean = good_diag_words()
        boolean[5] = True
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': boolean}, CORE_WORDS)
        self.assertTrue(any('bool' in p or 'uint64' in p for p in problems))

    def test_flags_overflow_boundary(self):
        words = good_diag_words()
        words[3] = 4 | (1 << 63)  # frozen low bits, overflow bit set
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words}, CORE_WORDS)
        self.assertTrue(any('overflow' in p for p in problems))
        self.assertFalse(any('frozen 低位' in p for p in problems))

    def test_seq_and_cycles_mismatch(self):
        words = good_diag_words()
        words[4] = 6       # core seq is 5
        words[5] = 1       # core cycles is 5581750
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words}, CORE_WORDS)
        self.assertTrue(any('seq' in p for p in problems))
        self.assertTrue(any('cycles' in p for p in problems))

    def test_fast_accept_must_equal_core_fast_sum(self):
        words = good_diag_words()
        words[15] = 3  # Σ core fast 6..10 == 0
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words}, CORE_WORDS)
        self.assertTrue(any('fast_accept' in p for p in problems))

    def test_core_fast_sum_present_but_invalid_value(self):
        # Field present as explicit None -> invalid (must be an int).
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(core_fast_sum=None), CORE_WORDS)
        self.assertTrue(any('core_fast_sum' in p for p in problems))

        # Field present as bool -> invalid.
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(core_fast_sum=True), CORE_WORDS)
        self.assertTrue(any('core_fast_sum' in p for p in problems))

        # Field present as non-int -> invalid.
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(core_fast_sum='0'), CORE_WORDS)
        self.assertTrue(any('core_fast_sum' in p for p in problems))

    def test_core_fast_sum_explicit_arg_and_doc_must_agree(self):
        # Explicit arg equals doc field and Σcore -> clean.
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(core_fast_sum=0), CORE_WORDS, 0)
        self.assertEqual(problems, [])

        # Explicit arg disagrees with doc field -> reported, not silently ignored.
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(core_fast_sum=0), CORE_WORDS, 5)
        self.assertTrue(any('core_fast_sum' in p for p in problems))

        # Explicit None is distinct from omitted and is invalid.
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(with_core_fast_sum=False), CORE_WORDS, None)
        self.assertTrue(any('core_fast_sum' in p for p in problems))

    def test_core_fast_sum_optional_verifier_if_present(self):
        # Absent -> no core_fast_sum problem, still valid.
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(with_core_fast_sum=False), CORE_WORDS)
        self.assertEqual(problems, [])
        # Present but wrong -> problem.
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', diag_object(core_fast_sum=5), CORE_WORDS)
        self.assertTrue(any('core_fast_sum' in p for p in problems))

    def test_nonzero_core_fast_matches(self):
        core = list(CORE_WORDS)
        core[6] = 2
        core[9] = 3           # A write
        core[14] = 32767 + 5  # keep accept conservation sensible
        words = good_diag_words()
        words[15] = 5
        problems = an_mod.validate_helper_diagnostic(
            'banked-serial', {'words': words, 'core_fast_sum': 5}, core)
        self.assertEqual(problems, [])


class FastZeroScientificTests(unittest.TestCase):
    def test_fast_zero_is_not_a_technical_failure(self):
        with tempfile.TemporaryDirectory() as tmp:
            case_dir = _make_case(tmp, LONG_A1_SPEC)
            an = an_mod.Analysis()
            entry = an_mod.analyze_case(
                an, 'banked-serial', LONG_A1_SPEC, case_dir, diagnostics=True)

            self.assertEqual(entry['state'], 'complete')
            self.assertIsNotNone(entry['fast'])
            self.assertTrue(entry['fast']['all_zero'])
            self.assertEqual(entry['fast']['total'], 0)
            self.assertIsNotNone(entry['diagnostic'])
            self.assertTrue(entry['diagnostic']['ok'])
            # No quality issue from the diagnostic bank, and the case qualifies.
            self.assertEqual([q for q in an.quality if q['code'] == 'helper_diagnostic'], [])
            self.assertTrue(entry['qualified'], entry['qualified_reasons'])
            self.assertEqual(entry['qualified_reasons'], [])

    def test_missing_diagnostic_fails_only_when_requested(self):
        with tempfile.TemporaryDirectory() as tmp:
            case_dir = _make_case(tmp, LONG_A1_SPEC, include_diagnostic=False)

            an_off = an_mod.Analysis()
            entry_off = an_mod.analyze_case(
                an_off, 'banked-serial', LONG_A1_SPEC, case_dir, diagnostics=False)
            self.assertTrue(entry_off['qualified'], entry_off['qualified_reasons'])

            an_on = an_mod.Analysis()
            entry_on = an_mod.analyze_case(
                an_on, 'banked-serial', LONG_A1_SPEC, case_dir, diagnostics=True)
            self.assertFalse(entry_on['qualified'])
            self.assertTrue(any('诊断' in r for r in entry_on['qualified_reasons']))
            self.assertTrue(any(q['code'] == 'helper_diagnostic' for q in an_on.quality))


if __name__ == '__main__':
    unittest.main()
