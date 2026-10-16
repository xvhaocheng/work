#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
composition_plan.py - frozen plan and record validator for the same-BIT
composition experiment (idma-composition-v1).

Import-only: no third-party dependencies, no serial port, no device node.  It
provides the fixed 172-case plan in execution order, the per-case board CLI
arguments, the frozen-contract validator and the narrow window metrics.

Nothing here re-runs, re-orders or reselects a sample.  A scientific negative
(fast == 0, miss == 0, a P/PN throughput regression) is never turned into a
technical failure; only out-of-contract evidence is.
"""
from __future__ import annotations

PLAN_SCHEMA = 'idma-composition-plan-v1'
RECORD_SCHEMA = 'idma-composition-v1'

PLAN_GENERATION = 1

PHASES = ('short', 'noise', 'explore')

MODE_S = 0
MODE_N = 1
MODE_P = 2
MODE_PN = 3
MODE_ORDER = (MODE_S, MODE_N, MODE_P, MODE_PN)
MODE_NAMES = {MODE_S: 's', MODE_N: 'n', MODE_P: 'p', MODE_PN: 'pn'}
# N is enabled in N and PN; a fast accept while N is disabled is a violation.
N_ENABLED_MODES = frozenset((MODE_N, MODE_PN))
N_DISABLED_MODES = frozenset((MODE_S, MODE_P))

A_PAGE_CHOICES = (0, 1, 8, 16)
V_GAP_CHOICES = (0, 1000, 4000)

SHORT_TRANSFERS = 16
SHORT_WARMUP = 16
NOISE_A_PAGES = 8
NOISE_TRANSFERS = 4096
NOISE_WARMUP = 256
NOISE_PAIRS = 6
EXPLORE_TRANSFERS = 4096
EXPLORE_WARMUP = 256
EXPLORE_BLOCKS = 3

DEFAULT_TRANSFERS = 4096
DEFAULT_WARMUP = 256
DEFAULT_TIMEOUT_MS = 15000
DEFAULT_DEVICE = '/dev/idma0'

PROTECTED_DID = 10
AGGRESSOR_INSTANCE = 1
AGGRESSOR_DID = 11
CHUNK_BYTES = 4096
PLATFORM_HZ = 50000000
CYCLE_NS = 20

UINT64_MAX = 0xFFFFFFFFFFFFFFFF
RAW_WORDS = 128

COMPOSITION_MAGIC = 0x49444D4143504E01
COMPOSITION_ABI = 1
P4M_BANK_MAGIC = 0x49444D4150344D01
P4M_OBSERVER_STATUS_OK = 4
P4M_WINDOW_WORDS = 16
P4M_OBS_WORDS = 35
WINDOW_FLAG_ARMED = 0x1
WINDOW_FLAG_ACTIVE = 0x2
WINDOW_FLAG_FROZEN = 0x4

RAW_MAGIC = 0
RAW_ABI = 1
RAW_LIVE_CONFIG = 2
RAW_LIVE_GENERATION = 3
RAW_LAST_STATUS = 4
RAW_LIVE_QUIET = 5
RAW_WINDOW_FLAGS = 6
RAW_WINDOW_SEQUENCE = 7
RAW_WINDOW_CYCLES = 8
RAW_WINDOW_ERRORS = 9
RAW_ARM_CONFIG = 10
RAW_ARM_GENERATION = 11
RAW_V_LOOKUP = 12
RAW_V_HIT = 13
RAW_V_MISS = 14
RAW_A_LOOKUP = 15
RAW_A_HIT = 16
RAW_A_MISS = 17
RAW_OTHER_LOOKUP = 18
RAW_OTHER_HIT = 19
RAW_OTHER_MISS = 20
RAW_V_FAST = 21
RAW_A_FAST = 22
RAW_OTHER_FAST = 23
RAW_V_GAP = 24
RAW_A_GAP = 25
RAW_V_FILL = 26
RAW_A_FILL = 27
RAW_OTHER_FILL = 28
RAW_REPLACE_PSCID = 29
RAW_P_ALLOC_VIOLATION = 30
RAW_INVAL_CYCLES = 31
RAW_BAD_LOOKUP = 32
RAW_FAST_N_DISABLED = 33
RAW_PROTECTED_MASK = 34
RAW_RESERVED35 = 35
RAW_PLRU_BEFORE = 36
RAW_PLRU_AFTER = 37
RAW_V_PSCID = 38
RAW_A_PSCID = 39
RAW_ENTRIES_BEFORE = 64
RAW_ENTRIES_AFTER = 96


def _case_id(phase, mode, a_pages, v_gap_cycles, block, pair, slot):
    name = MODE_NAMES[mode]
    if phase == 'short':
        return 'short-m%s-a%d' % (name, a_pages)
    if phase == 'noise':
        return 'noise-p%d-s%d-m%s-a%d' % (pair, slot, name, a_pages)
    return 'explore-b%d-g%d-a%d-m%s' % (block, v_gap_cycles, a_pages, name)


def _make_case(phase, mode, a_pages, v_gap_cycles, transfers, warmup, block, pair, slot):
    return {
        'id': _case_id(phase, mode, a_pages, v_gap_cycles, block, pair, slot),
        'phase': phase,
        'block': block,
        'pair': pair,
        'slot': slot,
        'mode': mode,
        'mode_name': MODE_NAMES[mode],
        'a_pages': a_pages,
        'v_gap_cycles': v_gap_cycles,
        'transfers': transfers,
        'warmup': warmup,
        'timeout_ms': DEFAULT_TIMEOUT_MS,
        'auto_rerun': False,
    }


def build_plan():
    """Return the full frozen 172-case plan in original execution order."""
    cases = []
    for mode in MODE_ORDER:
        for a_pages in A_PAGE_CHOICES:
            cases.append(_make_case('short', mode, a_pages, 0,
                                    SHORT_TRANSFERS, SHORT_WARMUP,
                                    None, None, None))
    for pair in range(1, NOISE_PAIRS + 1):
        for slot in range(2):
            cases.append(_make_case('noise', MODE_S, NOISE_A_PAGES, 0,
                                    NOISE_TRANSFERS, NOISE_WARMUP,
                                    None, pair, slot))
    for block in range(1, EXPLORE_BLOCKS + 1):
        for gap_index, gap in enumerate(V_GAP_CHOICES):
            for a_index, a_pages in enumerate(A_PAGE_CHOICES):
                rotation = (block - 1 + gap_index + a_index) % len(MODE_ORDER)
                order = [MODE_ORDER[(rotation + k) % len(MODE_ORDER)]
                         for k in range(len(MODE_ORDER))]
                for mode in order:
                    cases.append(_make_case('explore', mode, a_pages, gap,
                                            EXPLORE_TRANSFERS, EXPLORE_WARMUP,
                                            block, None, None))
    phases = {}
    for name in PHASES:
        phases[name] = sum(1 for case in cases if case['phase'] == name)
    return {
        'schema': PLAN_SCHEMA,
        'plan_generation': PLAN_GENERATION,
        'auto_rerun': False,
        'resume': False,
        'calibration': 'unverified',
        'count': len(cases),
        'phases': phases,
        'cases': cases,
    }


def cases_through_phase(plan, phase):
    if phase not in PHASES:
        raise ValueError('unknown phase: ' + repr(phase))
    limit = PHASES.index(phase)
    allowed = set(PHASES[:limit + 1])
    return [case for case in plan['cases'] if case['phase'] in allowed]


def case_argv(case, device=DEFAULT_DEVICE):
    return [
        '--device', device,
        '--mode', str(int(case['mode'])),
        '--a-pages', str(int(case['a_pages'])),
        '--v-gap', str(int(case['v_gap_cycles'])),
        '--transfers', str(int(case['transfers'])),
        '--warmup', str(int(case['warmup'])),
        '--timeout-ms', str(int(case['timeout_ms'])),
    ]


def expected_live_config(mode, protected_did=PROTECTED_DID):
    return (int(mode) & 0x3) | ((int(protected_did) & 0xFFFFFF) << 8)


def _word(value):
    if isinstance(value, bool) or not isinstance(value, int):
        return None
    if value < 0 or value > UINT64_MAX:
        return None
    return value


def _check_words(values, label, problems):
    if not isinstance(values, list):
        problems.append(label + ' is not a list')
        return False
    for index, value in enumerate(values):
        if _word(value) is None:
            problems.append(label + '[%d] is not a uint64: %r' % (index, value))
            return False
    return True


def validate_case(case, record, meta=None):
    """Return the list of contract problems for one case record.

    A scientific negative (fast == 0, miss == 0, no P/PN benefit) is never a
    problem.  meta, when given, carries the runner-owned exit code and taint.
    """
    problems = []
    if not isinstance(record, dict):
        return ['record is not a JSON object']
    if record.get('schema') != RECORD_SCHEMA:
        problems.append('schema %r is not %s' % (record.get('schema'), RECORD_SCHEMA))
    if record.get('success') is not True:
        problems.append('success is not true')
    error = record.get('error')
    if isinstance(error, str) and error.strip():
        problems.append('error: ' + error)
    if record.get('errno') != 0:
        problems.append('errno %r' % (record.get('errno'),))

    request = record.get('request')
    if not isinstance(request, dict):
        problems.append('request is not an object')
    else:
        expected = (
            ('mode', int(case['mode'])),
            ('a_pages', int(case['a_pages'])),
            ('victim_gap_cycles', int(case['v_gap_cycles'])),
            ('victim_transfers', int(case['transfers'])),
            ('warmup_transfers', int(case['warmup'])),
            ('protected_did', PROTECTED_DID),
        )
        for key, want in expected:
            got = request.get(key)
            if isinstance(got, bool) or got != want:
                problems.append('request.%s %r != expected %r' % (key, got, want))

    result = record.get('result')
    if not isinstance(result, dict):
        problems.append('result is not an object')
        return problems

    if result.get('abi_version') != COMPOSITION_ABI:
        problems.append('result.abi_version %r' % (result.get('abi_version'),))
    if result.get('status') != 0:
        problems.append('result.status %r' % (result.get('status'),))
    if result.get('restore_status') != 0:
        problems.append('result.restore_status %r' % (result.get('restore_status'),))
    if result.get('configured') != 1:
        problems.append('result.configured %r' % (result.get('configured'),))
    if result.get('raw_captured') != 1:
        problems.append('result.raw_captured %r' % (result.get('raw_captured'),))

    generation = _word(result.get('generation'))
    restored = _word(result.get('restored_generation'))
    if generation is None:
        problems.append('result.generation is not a uint64')
    if restored is None:
        problems.append('result.restored_generation is not a uint64')
    if generation is not None and restored is not None and restored <= generation:
        problems.append('restored_generation %r did not advance past generation %r'
                        % (restored, generation))

    experiment = result.get('experiment')
    if not isinstance(experiment, dict):
        problems.append('result.experiment is not an object')
    else:
        _validate_experiment(case, experiment, problems)

    raw = result.get('raw')
    raw_ok = _check_words(raw, 'result.raw', problems)
    if raw_ok and len(raw) != RAW_WORDS:
        problems.append('result.raw is not %d words' % RAW_WORDS)
        raw_ok = False

    window = None
    observation = None
    window_ok = False
    observation_ok = False
    p4m = result.get('p4m')
    if not isinstance(p4m, dict):
        problems.append('result.p4m is not an object')
    else:
        window = p4m.get('window')
        observation = p4m.get('observation')
        window_ok = _check_words(window, 'p4m.window', problems)
        observation_ok = _check_words(observation, 'p4m.observation', problems)
        if window_ok and len(window) != P4M_WINDOW_WORDS:
            problems.append('p4m.window is not %d words' % P4M_WINDOW_WORDS)
            window_ok = False
        if observation_ok and len(observation) != P4M_OBS_WORDS:
            problems.append('p4m.observation is not %d words' % P4M_OBS_WORDS)
            observation_ok = False
        if p4m.get('observer_status') != P4M_OBSERVER_STATUS_OK:
            problems.append('p4m.observer_status %r' % (p4m.get('observer_status'),))
        sequence = p4m.get('sequence')
        if isinstance(sequence, bool) or not isinstance(sequence, int) or sequence <= 0:
            problems.append('p4m.sequence %r' % (sequence,))

    if window_ok and observation_ok and raw_ok:
        _validate_frozen(case, window, observation, raw, generation, problems)

    if meta is not None:
        _validate_meta(meta, problems)
    return problems


def _validate_experiment(case, experiment, problems):
    if experiment.get('status') != 0:
        problems.append('experiment.status %r' % (experiment.get('status'),))
    if experiment.get('victim_did') != PROTECTED_DID:
        problems.append('experiment.victim_did %r != %r'
                        % (experiment.get('victim_did'), PROTECTED_DID))
    victim = experiment.get('victim')
    if not isinstance(victim, dict):
        problems.append('experiment.victim is not an object')
    else:
        if victim.get('data_errors'):
            problems.append('experiment.victim.data_errors %r' % (victim.get('data_errors'),))
        if victim.get('quarantined'):
            problems.append('experiment.victim.quarantined %r' % (victim.get('quarantined'),))
    if int(case['a_pages']) == 0:
        if experiment.get('aggressor') is not None:
            problems.append('experiment.aggressor is not null for a Solo case')
    else:
        aggressor = experiment.get('aggressor')
        if not isinstance(aggressor, dict):
            problems.append('experiment.aggressor is not an object')
        else:
            if aggressor.get('data_errors'):
                problems.append('experiment.aggressor.data_errors %r'
                                % (aggressor.get('data_errors'),))
            if aggressor.get('quarantined'):
                problems.append('experiment.aggressor.quarantined %r'
                                % (aggressor.get('quarantined'),))
        if experiment.get('aggressor_did') == PROTECTED_DID:
            problems.append('experiment.aggressor_did equals the protected DID')


def _validate_frozen(case, window, observation, raw, generation, problems):
    mode = int(case['mode'])
    a_pages = int(case['a_pages'])
    transfers = int(case['transfers'])
    config = expected_live_config(mode)

    if raw[RAW_MAGIC] != COMPOSITION_MAGIC:
        problems.append('raw[0] magic %r' % (raw[RAW_MAGIC],))
    if raw[RAW_ABI] != COMPOSITION_ABI:
        problems.append('raw[1] abi %r' % (raw[RAW_ABI],))
    if raw[RAW_LIVE_CONFIG] != config:
        problems.append('raw[2] live config %r != %r' % (raw[RAW_LIVE_CONFIG], config))
    if raw[RAW_ARM_CONFIG] != config:
        problems.append('raw[10] frozen config %r != %r' % (raw[RAW_ARM_CONFIG], config))
    if generation is not None:
        if raw[RAW_LIVE_GENERATION] != generation:
            problems.append('raw[3] live generation %r != result generation %r'
                            % (raw[RAW_LIVE_GENERATION], generation))
        if raw[RAW_ARM_GENERATION] != generation:
            problems.append('raw[11] frozen generation %r != result generation %r'
                            % (raw[RAW_ARM_GENERATION], generation))
    if not (raw[RAW_WINDOW_FLAGS] & WINDOW_FLAG_FROZEN):
        problems.append('raw[6] window flags %r lack the frozen bit'
                        % (raw[RAW_WINDOW_FLAGS],))
    if raw[RAW_WINDOW_SEQUENCE] <= 0:
        problems.append('raw[7] window sequence is not > 0')
    if raw[RAW_WINDOW_CYCLES] <= 0:
        problems.append('raw[8] window cycles is not > 0')
    if raw[RAW_WINDOW_CYCLES] != window[1]:
        problems.append('raw[8] cycles %r != p4m.window[1] %r (not the same window)'
                        % (raw[RAW_WINDOW_CYCLES], window[1]))
    if raw[RAW_WINDOW_ERRORS] != 0:
        problems.append('raw[9] window error bits %r' % (raw[RAW_WINDOW_ERRORS],))
    if raw[RAW_FAST_N_DISABLED] != 0:
        problems.append('raw[33] fast events while N is disabled %r'
                        % (raw[RAW_FAST_N_DISABLED],))

    if raw[RAW_V_LOOKUP] != raw[RAW_V_HIT] + raw[RAW_V_MISS]:
        problems.append('V lookup conservation %r != %r + %r'
                        % (raw[RAW_V_LOOKUP], raw[RAW_V_HIT], raw[RAW_V_MISS]))
    if raw[RAW_A_LOOKUP] != raw[RAW_A_HIT] + raw[RAW_A_MISS]:
        problems.append('A lookup conservation %r != %r + %r'
                        % (raw[RAW_A_LOOKUP], raw[RAW_A_HIT], raw[RAW_A_MISS]))
    if raw[RAW_OTHER_LOOKUP] != raw[RAW_OTHER_HIT] + raw[RAW_OTHER_MISS]:
        problems.append('other lookup conservation %r != %r + %r'
                        % (raw[RAW_OTHER_LOOKUP], raw[RAW_OTHER_HIT], raw[RAW_OTHER_MISS]))

    requested_gap = int(case['v_gap_cycles'])
    if requested_gap == 0:
        if raw[RAW_V_GAP] != 0:
            problems.append('raw[24] V gap %r for a Vgap0 case' % (raw[RAW_V_GAP],))
    elif raw[RAW_V_GAP] < requested_gap:
        problems.append('raw[24] V gap %r < requested %r' % (raw[RAW_V_GAP], requested_gap))
    if raw[RAW_A_GAP] != 0:
        problems.append('raw[25] A gap %r although Agap is fixed at 0' % (raw[RAW_A_GAP],))

    if mode in N_DISABLED_MODES:
        if raw[RAW_V_FAST] or raw[RAW_A_FAST] or raw[RAW_OTHER_FAST]:
            problems.append('fast accepts %r although N is disabled'
                            % [raw[RAW_V_FAST], raw[RAW_A_FAST], raw[RAW_OTHER_FAST]])
    # fast == 0 and miss == 0 are scientific observations, never technical failures.

    if window[13] != transfers or window[14] != transfers:
        problems.append('p4m.window V issued/done %r != %r'
                        % ([window[13], window[14]], transfers))
    if observation[7] != transfers or observation[8] != transfers:
        problems.append('p4m.observation V issued/done %r != %r'
                        % ([observation[7], observation[8]], transfers))

    if a_pages == 0:
        if raw[RAW_A_LOOKUP] or raw[RAW_A_HIT] or raw[RAW_A_MISS]:
            problems.append('A normal lookups %r in a Solo case'
                            % [raw[RAW_A_LOOKUP], raw[RAW_A_HIT], raw[RAW_A_MISS]])
        if window[15] & 0x1:
            problems.append('p4m.window has_a is set in a Solo case')
    else:
        if not (window[15] & 0x1):
            problems.append('p4m.window has_a is missing for an aggressor case')


def _validate_meta(meta, problems):
    if not isinstance(meta, dict):
        problems.append('meta is not an object')
        return
    if meta.get('exit') != 0:
        problems.append('CLI exit %r' % (meta.get('exit'),))
    for key in ('taint_before', 'taint_after'):
        value = meta.get(key)
        if str(value).strip() != '0':
            problems.append('%s %r' % (key, value))


def case_metrics(case, record):
    """The narrow window metrics only; never the whole-round interpretation."""
    result = record.get('result') if isinstance(record, dict) else None
    p4m = result.get('p4m') if isinstance(result, dict) else None
    window = p4m.get('window') if isinstance(p4m, dict) else None
    raw = result.get('raw') if isinstance(result, dict) else None
    window = window if isinstance(window, list) else []
    raw = raw if isinstance(raw, list) else []
    return {
        'id': case['id'],
        'phase': case['phase'],
        'mode': case['mode'],
        'a_pages': case['a_pages'],
        'v_gap_cycles': case['v_gap_cycles'],
        'transfers': case['transfers'],
        'warmup': case['warmup'],
        'v_workload_bytes': int(case['transfers']) * CHUNK_BYTES,
        'v_window_cycles': window[1] if len(window) > 1 else None,
        'a_completed_jobs_window': window[3] if len(window) > 3 else None,
        'v_actual_gap_cycles': raw[RAW_V_GAP] if len(raw) > RAW_V_GAP else None,
        'a_completed_note': 'A jobs completed inside the (open,close] window, not the whole round',
        'v_gap_note': 'raw[24] is the actual occupied gap; subtracting the nominal gap is not service latency',
        'platform_hz': PLATFORM_HZ,
        'cycle_ns': CYCLE_NS,
        'hpm': 'raw_only, not a validated metric',
    }
