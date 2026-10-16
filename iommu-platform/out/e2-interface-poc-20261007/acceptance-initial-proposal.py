#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
'''Read-only acceptance verifier for the E2 bounded completion-pool POC.

Reads only artifacts that already exist: files under
out/e2-interface-poc-20261007, tests/nonblock_completion_pool.sv, and the
production sources named by previous-production-source-hashes.json. It writes
exactly two files into out/e2-interface-poc-20261007: comparison.json and
acceptance.json. It never runs a simulation, build, lint, synthesis or board
step and never modifies evidence. Any read outside the allow-list raises.

Usage (from the project root that contains cva6/ and out/):
    python3 accept_e2_poc.py
    python3 accept_e2_poc.py --report out/e2-interface-poc-20261007/report.md

Exit codes: 0 all required gates pass, 1 a required check failed,
2 fatal precondition or read violation (outputs are still written).
'''
from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from pathlib import Path

OUT_REL = 'out/e2-interface-poc-20261007'
INITIAL_RUNS_NAME = 'initial-runs.json'
RESERVED_RUNS_NAME = 'reserved-runs.json'
HASHES_NAME = 'previous-production-source-hashes.json'
DESIGN_NAME = 'design.md'
POOL_SV_REL = 'tests/nonblock_completion_pool.sv'
COMPARISON_NAME = 'comparison.json'
ACCEPTANCE_NAME = 'acceptance.json'
REPORT_CANDIDATES = ('report.md', 'e2-report.md', 'report_markdown.md', 'e2-interface-poc-report.md')
MANIFEST_CANDIDATES = ('sources-manifest.json', 'sources_manifest.json', 'source-manifest.json',
                       'sources.json', 'manifest.json', 'artifact-manifest.json')
RECORD_KEYS = ('ACTUAL-DMA-DDR-JSON', 'ACTUAL-DMA-PATH-JSON', 'ACTUAL-DMA-WINDOW-JSON',
               'READ-ADMIT-WINDOW-JSON', 'READ-ADMIT-TERMINAL-JSON', 'E2-TERMINAL-JSON')
MODE_BY_PREFIX = {'bypass': 0, 'ordinary': 1, 'selective': 2, 'reserved': 3}
DRAFT_PAYLOAD_BY_MODE = {0: 8192, 1: 4144, 2: 2096, 3: 2096}
EXPECTED_PAYLOAD = {0: (8192, 0), 1: (4144, 4096), 2: (2096, 2048), 3: (2096, 2048)}

DRAFT_RUN_FACTS = {
    'bypass-pn-hot': {'mode': 'pn', 'e2_mode': 0, 'cold': 0, 'W_cycles': 93487, 'fast': 62,
                      'reads': 93, 'pool_slots': 0, 'depth': 512, 'grants_pool': 0,
                      'grants_stream': 0, 'highwater': 0},
    'selective-p-hot': {'mode': 'p', 'e2_mode': 2, 'cold': 0, 'W_cycles': 112451, 'fast': 0,
                        'reads': 93, 'pool_slots': 1, 'depth': 3, 'grants_pool': 960,
                        'grants_stream': 324, 'highwater': 256},
    'selective-p-cold': {'mode': 'p', 'e2_mode': 2, 'cold': 1, 'W_cycles': 112931, 'fast': 0,
                         'reads': 96, 'pool_slots': 1, 'depth': 3, 'grants_pool': 960,
                         'grants_stream': 324, 'highwater': 256},
    'selective-pn-cold': {'mode': 'pn', 'e2_mode': 2, 'cold': 1, 'W_cycles': 112948, 'fast': 0,
                          'reads': 96, 'pool_slots': 1, 'depth': 3, 'grants_pool': 960,
                          'grants_stream': 324, 'highwater': 256},
    'selective-pn-hot': {'mode': 'pn', 'e2_mode': 2, 'cold': 0, 'W_cycles': 112448, 'fast': 0,
                         'reads': 93, 'pool_slots': 1, 'depth': 3, 'grants_pool': 960,
                         'grants_stream': 324, 'highwater': 256},
    'ordinary-p-hot': {'mode': 'p', 'e2_mode': 1, 'cold': 0, 'W_cycles': 97774, 'fast': 0,
                       'reads': 93, 'pool_slots': 2, 'depth': 3, 'grants_pool': 1284,
                       'grants_stream': 0, 'highwater': 254},
    'ordinary-p-cold': {'mode': 'p', 'e2_mode': 1, 'cold': 1, 'W_cycles': 98214, 'fast': 0,
                        'reads': 96, 'pool_slots': 2, 'depth': 3, 'grants_pool': 1284,
                        'grants_stream': 0, 'highwater': 254},
    'ordinary-pn-hot': {'mode': 'pn', 'e2_mode': 1, 'cold': 0, 'W_cycles': 103348, 'fast': 31,
                        'reads': 93, 'pool_slots': 2, 'depth': 3, 'grants_pool': 1284,
                        'grants_stream': 0, 'highwater': 490},
    'ordinary-pn-cold': {'mode': 'pn', 'e2_mode': 1, 'cold': 1, 'W_cycles': 103968, 'fast': 32,
                         'reads': 96, 'pool_slots': 2, 'depth': 3, 'grants_pool': 1284,
                         'grants_stream': 0, 'highwater': 490},
    'reserved-p-hot': {'mode': 'p', 'e2_mode': 3, 'cold': 0, 'W_cycles': 125680, 'fast': 0,
                       'reads': 93, 'pool_slots': 1, 'depth': 3, 'grants_pool': 640,
                       'grants_stream': 644, 'highwater': 256},
    'reserved-p-cold': {'mode': 'p', 'e2_mode': 3, 'cold': 1, 'W_cycles': 126180, 'fast': 0,
                        'reads': 96, 'pool_slots': 1, 'depth': 3, 'grants_pool': 640,
                        'grants_stream': 644, 'highwater': 256},
    'reserved-pn-hot': {'mode': 'pn', 'e2_mode': 3, 'cold': 0, 'W_cycles': 125677, 'fast': 0,
                        'reads': 93, 'pool_slots': 1, 'depth': 3, 'grants_pool': 640,
                        'grants_stream': 644, 'highwater': 256},
    'reserved-pn-cold': {'mode': 'pn', 'e2_mode': 3, 'cold': 1, 'W_cycles': 126177, 'fast': 0,
                         'reads': 96, 'pool_slots': 1, 'depth': 3, 'grants_pool': 640,
                         'grants_stream': 644, 'highwater': 256},
}

DRAFT_SUMMARY_TABLE = [
    {'method': 'mode0-bypass', 'payload_bytes': 8192, 'condition': 'pn-hot (v-cold-dst=0)',
     'P_cycles': None, 'PN_cycles': 93487, 'P_fast': None, 'PN_fast': 62,
     'pn_vs_p_time_percent': None},
    {'method': 'mode1-ordinary', 'payload_bytes': 4144, 'condition': 'hot (v-cold-dst=0)',
     'P_cycles': 97774, 'PN_cycles': 103348, 'P_fast': 0, 'PN_fast': 31,
     'pn_vs_p_time_percent': 5.7009},
    {'method': 'mode1-ordinary', 'payload_bytes': 4144, 'condition': 'cold (v-cold-dst=1)',
     'P_cycles': 98214, 'PN_cycles': 103968, 'P_fast': 0, 'PN_fast': 32,
     'pn_vs_p_time_percent': 5.8586},
    {'method': 'mode2-selective', 'payload_bytes': 2096, 'condition': 'hot (v-cold-dst=0)',
     'P_cycles': 112451, 'PN_cycles': 112448, 'P_fast': 0, 'PN_fast': 0,
     'pn_vs_p_time_percent': -0.0027},
    {'method': 'mode2-selective', 'payload_bytes': 2096, 'condition': 'cold (v-cold-dst=1)',
     'P_cycles': 112931, 'PN_cycles': 112948, 'P_fast': 0, 'PN_fast': 0,
     'pn_vs_p_time_percent': 0.0151},
    {'method': 'mode3-reserved', 'payload_bytes': 2096, 'condition': 'hot (v-cold-dst=0)',
     'P_cycles': 125680, 'PN_cycles': 125677, 'P_fast': 0, 'PN_fast': 0,
     'pn_vs_p_time_percent': -0.0024},
    {'method': 'mode3-reserved', 'payload_bytes': 2096, 'condition': 'cold (v-cold-dst=1)',
     'P_cycles': 126180, 'PN_cycles': 126177, 'P_fast': 0, 'PN_fast': 0,
     'pn_vs_p_time_percent': -0.0024},
]

DRAFT_DERIVED_RATIOS = {
    'selective_vs_ordinary_pn_hot_percent': 8.8052,
    'reserved_vs_selective_pn_hot_percent': 11.7645,
    'reserved_vs_ordinary_pn_hot_percent': 21.6056,
    'pn_hot_vs_mode0_bypass_percent': {'ordinary': 10.5480, 'selective': 20.2820, 'reserved': 34.4326},
}

REFERENCE_PREVIOUS_RUN_EXCLUDED = {
    'label': 'strong ordinary large-private full-buffer walk (previous run, NOT one of the 13)',
    'P_cycles': 92334,
    'PN_cycles': 91727,
    'included_in_13_run_set': False,
}

LIMITATIONS = (
    'capacity points are elaborated/tested DATA bytes only: response metadata and control are excluded and no synthesis area or LUT figure is measured or guaranteed',
    'scope is two DMAs, normal aligned 4 KiB jobs, one executing job per DMA; no real CPU, MIG, general AXI behaviour or board measurement',
    'modes 1-3 share one active AR per DMA; this is not a best ordinary global-pool implementation and not a matched outstanding-depth comparison',
    'mode 3 is a bounded prototype variant, not a proven architecture; the pool-allocation hypothesis alone is insufficient per the observed mode 3 regression',
    'the reported first-pair event ordering (bypass AR before Rlast; selective/reserved AR after Rlast) comes from Codex reading the event CSVs; the CSVs are referenced but were not supplied, so this verifier only records their presence and does not recompute the ordering',
    'the three new test sources other than tests/nonblock_completion_pool.sv, and the sources manifest mapping schema, were not supplied with the request',
)

REPORT_FORBIDDEN_PATTERNS = (
    ('claim_all_13_first_try', '13/13'),
    ('claim_measured_lut', 'measured[ ]+LUT'),
    ('claim_measured_area_savings', 'measured[ ]+area[ ]+savings'),
    ('claim_board_measurement', '板上实测'),
    ('claim_hardware_measurement', 'hardware[- ]measured'),
)


class Violation(RuntimeError):
    '''Raised when a read or write outside the allow-list is attempted.'''


def is_within(child, parent):
    try:
        child.relative_to(parent)
    except ValueError:
        return False
    return True


def sha256_file(path):
    digest = hashlib.sha256()
    with path.open('rb') as handle:
        for chunk in iter(lambda: handle.read(1 << 20), b''):
            digest.update(chunk)
    return digest.hexdigest()


class Evidence:
    '''Guarded reader: out_dir, the prototype test source and declared sources only.'''

    def __init__(self, cwd, out_dir):
        self.cwd = cwd
        self.out_dir = out_dir
        self.allowed = set()
        self.reads = []
        self.violations = []

    def allow(self, path):
        self.allowed.add(Path(path).resolve())

    def resolve(self, raw, why):
        path = Path(str(raw))
        resolved = path.resolve() if path.is_absolute() else (self.cwd / path).resolve()
        if is_within(resolved, self.out_dir) or resolved in self.allowed:
            return resolved
        message = 'refusing to read %s (%s): outside out_dir and not declared' % (resolved, why)
        self.violations.append(message)
        raise Violation(message)

    def open_text(self, raw, why):
        path = self.resolve(raw, why)
        if not path.is_file():
            return path, None
        self.reads.append({'path': str(path), 'why': why})
        return path, path.read_text(encoding='utf-8', errors='replace')

    def digest(self, raw, why):
        path = self.resolve(raw, why)
        if not path.is_file():
            return path, None
        self.reads.append({'path': str(path), 'why': why})
        return path, sha256_file(path)

    def out_path(self, name):
        path = (self.out_dir / name).resolve()
        if not is_within(path, self.out_dir):
            raise Violation('refusing to write %s: outside out_dir' % path)
        return path


def parse_command(command):
    params = {}
    for token in command or []:
        if isinstance(token, str) and token.startswith('+'):
            key, _, value = token[1:].partition('=')
            params[key] = value
    return params


def extract_event_trace(command):
    for token in command or []:
        if isinstance(token, str) and token.startswith('+event-trace='):
            return token.split('=', 1)[1]
    return None


def check_log(ev, run, name):
    log_raw = run.get('log')
    results = []
    if not log_raw:
        results.append({'id': 'log_path_declared', 'pass': False, 'required': True, 'kind': 'log',
                        'detail': 'no log path in run entry'})
        return results, {'log_basename': None, 'exists': False}
    try:
        path, text = ev.open_text(log_raw, 'original run log for %s' % name)
    except Violation as exc:
        results.append({'id': 'log_path_allowed', 'pass': False, 'required': True, 'kind': 'log',
                        'detail': str(exc)})
        return results, {'log_basename': None, 'exists': False, 'violation': str(exc)}
    exists = text is not None
    results.append({'id': 'log_file_exists', 'pass': exists, 'required': True, 'kind': 'log',
                    'detail': str(path)})
    if not exists:
        return results, {'log_basename': path.name, 'exists': False, 'path': str(path)}
    tail_lines = [line for line in text.splitlines() if line.strip()][-10:]
    results.append({'id': 'log_contains_PASS', 'pass': 'PASS' in text, 'required': True, 'kind': 'log',
                    'detail': 'final PASS marker expected in the original log'})
    results.append({'id': 'log_has_no_SCENARIO_FAIL', 'pass': 'SCENARIO-FAIL' not in text,
                    'required': True, 'kind': 'log', 'detail': 'SCENARIO-FAIL must be absent'})
    results.append({'id': 'log_tail_contains_PASS',
                    'pass': any('PASS' in line for line in tail_lines),
                    'required': True, 'kind': 'log', 'detail': 'last non-empty lines'})
    advisory = (
        ('log_zero_fault_visible', 'fault[ ]*[=:][ ]*0', 'per-tick fault counter reported as 0'),
        ('log_bytes_check_visible', 'bytes', 'bytes accounting line'),
        ('log_quiet_check_visible', 'quiet', 'quiet-path check line'),
        ('log_fullbuffer_zero_visible', 'full[ ]?buffer[ ]*[=:][ ]*0',
         'non-zero modes must report fullbuffer 0'),
    )
    for check_id, pattern, label in advisory:
        found = re.search(pattern, text, re.IGNORECASE) is not None
        results.append({'id': check_id, 'pass': found, 'required': False, 'kind': 'log_advisory',
                        'detail': 'advisory only (%s); exact log wording was not supplied, tighten the pattern once confirmed' % label})
    info = {'log_basename': path.name, 'exists': True, 'path': str(path), 'bytes': len(text),
            'tail_lines': tail_lines, 'contains_scenario_fail': 'SCENARIO-FAIL' in text}
    return results, info


def check_run(ev, source_file, index, run):
    name = str(run.get('name', '%s[%d]' % (source_file, index)))
    facts = DRAFT_RUN_FACTS.get(name)
    checks = []

    def add(check_id, ok, detail='', required=True, kind='check'):
        checks.append({'id': check_id, 'pass': bool(ok), 'required': bool(required),
                       'kind': kind, 'detail': str(detail)})

    def add_eq(check_id, got, expected, required=True, kind='check'):
        add(check_id, got == expected, 'got %r, expected %r' % (got, expected), required, kind)

    records = run.get('records') if isinstance(run.get('records'), dict) else {}
    ddr = records.get('ACTUAL-DMA-DDR-JSON') or {}
    path_rec = records.get('ACTUAL-DMA-PATH-JSON') or {}
    win = records.get('ACTUAL-DMA-WINDOW-JSON') or {}
    radw = records.get('READ-ADMIT-WINDOW-JSON') or {}
    radt = records.get('READ-ADMIT-TERMINAL-JSON') or {}
    term = records.get('E2-TERMINAL-JSON') or {}
    window = win.get('window') if isinstance(win.get('window'), dict) else {}
    command = run.get('command') if isinstance(run.get('command'), list) else []
    params = parse_command(command)
    binary = str(command[0]) if command else None
    trace = extract_event_trace(command)
    pool_cap = term.get('pool_capacity_bytes')
    private_bytes = term.get('private_payload_bytes')
    payload = pool_cap + private_bytes if isinstance(pool_cap, int) and isinstance(private_bytes, int) else None
    mode = win.get('e2_mode')

    add('name_known_to_draft', facts is not None, name)
    add_eq('exit_code_zero', run.get('exit_code'), 0)
    for key in RECORD_KEYS:
        add('record_present_%s' % key, key in records)
    add('log_path_declared', bool(run.get('log')), run.get('log'))
    add('binary_path_declared', bool(binary), binary)
    add('event_trace_declared', bool(trace), trace)

    for key, expected in (('system', '1'), ('a-pages', '8'), ('v-gap', '0'), ('transfers', '64'),
                          ('warmup', '256'), ('ptw-latency', '31'), ('read-admit', 'buffer'),
                          ('read-budget', '256'), ('read-period', '256'), ('ds-priority', '0'),
                          ('owner-only', '0')):
        add_eq('cmd_%s' % key, params.get(key), expected)
    add('cmd_mode_known', params.get('mode') in ('p', 'pn'), params.get('mode'))
    add('cmd_v_cold_dst_known', params.get('v-cold-dst') in ('0', '1'), params.get('v-cold-dst'))

    modes = [ddr.get('mode'), path_rec.get('mode'), win.get('mode'), radw.get('mode'),
             radt.get('mode'), term.get('scenario')]
    add('mode_agreement_across_records', len(set(modes)) == 1 and modes[0] in ('p', 'pn'), modes)
    add_eq('cmd_mode_matches_records', params.get('mode'), win.get('mode'))
    add_eq('cmd_v_cold_dst_matches_window', params.get('v-cold-dst'), str(win.get('v_cold_dst')))

    for check_id, got, expected in (
        ('window_a_pages', win.get('a_pages'), 8),
        ('window_v_transfers', win.get('v_transfers'), 64),
        ('window_warmup', win.get('warmup'), 256),
        ('window_ptw_latency', win.get('ptw_latency'), 31),
        ('window_v_gap', win.get('v_gap'), 0),
        ('window_owner_only', win.get('owner_only'), 0),
        ('window_runtime_N_matches_mode', win.get('runtime_N'), 1 if win.get('mode') == 'pn' else 0),
        ('window_v_completed_64', window.get('v_completed'), 64),
        ('window_a_completed_64', window.get('a_completed'), 64),
        ('window_ddr_aw_expected', window.get('ddr_aw'), 256),
        ('window_ddr_b_expected', window.get('ddr_b'), 256),
        ('window_ddr_w_expected', window.get('ddr_w'), 65536),
        ('window_comp_aw_fire_expected', window.get('comp_aw_fire'), 256),
        ('window_comp_w_fire_expected', window.get('comp_w_fire'), 65536),
        ('window_ddr_aw_v_region_expected', window.get('ddr_aw_v_region'), 128),
        ('window_ddr_aw_a_region_expected', window.get('ddr_aw_a_region'), 128),
        ('read_admit_terminal_buffer_empty_expected', radt.get('buffer_empty'), 1),
        ('read_admit_terminal_debt_expected', radt.get('debt'), 0),
    ):
        add_eq(check_id, got, expected)

    for check_id, left, right in (
        ('static_e2_mode_agreement', term.get('e2_mode'), win.get('e2_mode')),
        ('static_pool_slots_agreement', term.get('pool_slots'), win.get('pool_slots')),
        ('static_dma_buffer_depth_agreement', term.get('dma_buffer_depth'), win.get('dma_buffer_depth')),
        ('static_scenario_agreement', term.get('scenario'), win.get('mode')),
        ('terminal_received_equals_replayed', term.get('received_beats'), term.get('replayed_beats')),
        ('terminal_occupancy_zero', term.get('occupancy_beats'), 0),
        ('terminal_reserved_zero', term.get('reserved_beats'), 0),
        ('terminal_pending_reads_zero', term.get('pending_reads'), 0),
        ('terminal_scope_cumulative_including_warm', term.get('scope'),
         'cumulative_including_warm_not_window'),
        ('ddr_ds_reads_equals_ptw_r_fire', ddr.get('completed_window_ds_reads'), window.get('ptw_r_fire')),
        ('ddr_ar_slv3_ds_equals_ds_reads', window.get('ddr_ar_slv3_ds'),
         ddr.get('completed_window_ds_reads')),
        ('window_fast_equals_read_admit_fastread_captures', window.get('fast'),
         radw.get('fastread_captures')),
        ('window_fast_split_consistent',
         (window.get('fast_bank0') or 0) + (window.get('fast_bank1') or 0), window.get('fast')),
        ('window_W_cycles_equals_close_minus_open', win.get('W_cycles'),
         (win.get('win_close_cycle') or 0) - (win.get('win_open_cycle') or 0)),
        ('window_unavailable_fields_declared', win.get('unavailable'), ['comp_aw_stall', 'comp_ar_stall']),
        ('read_admit_window_captures_eq_offers', radw.get('captures'), radw.get('offer_handshakes')),
        ('read_admit_window_firstoffers_eq_offers', radw.get('firstoffers'), radw.get('offer_handshakes')),
        ('read_admit_terminal_captures_eq_fires', radt.get('captures'), radt.get('down_ar_fires')),
    ):
        add_eq(check_id, left, right)

    expected_payload, expected_pool_cap = EXPECTED_PAYLOAD.get(mode, (None, None))
    add_eq('payload_bytes_by_mode', payload, expected_payload)
    add_eq('pool_capacity_bytes_by_mode', pool_cap, expected_pool_cap)
    add_eq('e2_mode_matches_method_prefix', mode, MODE_BY_PREFIX.get(name.split('-')[0]))

    if mode == 0:
        for check_id, got, expected in (
            ('mode0_private_payload_2x4096', private_bytes, 8192),
            ('mode0_pool_slots_zero', term.get('pool_slots'), 0),
            ('mode0_grants_pool_zero', term.get('grants_pool'), 0),
            ('mode0_grants_stream_zero', term.get('grants_stream'), 0),
            ('mode0_depth_512', term.get('dma_buffer_depth'), 512),
            ('mode0_received_zero', term.get('received_beats'), 0),
            ('mode0_replayed_zero', term.get('replayed_beats'), 0),
            ('mode0_highwater_zero', term.get('highwater_beats'), 0),
        ):
            add_eq(check_id, got, expected)
    else:
        add_eq('nonzero_mode_depth_3', term.get('dma_buffer_depth'), 3)
        add_eq('nonzero_mode_private_payload_2x24', private_bytes, 48)
        grants_pool = term.get('grants_pool')
        grants_stream = term.get('grants_stream')
        received = term.get('received_beats')
        if all(isinstance(value, int) for value in (grants_pool, grants_stream, received)):
            add('total_grants_times_256_equals_received',
                (grants_pool + grants_stream) * 256 == received,
                '(%d + %d) * 256 vs %d' % (grants_pool, grants_stream, received))
        else:
            add('total_grants_times_256_equals_received', False, 'non-integer grant/received fields')
        highwater = term.get('highwater_beats')
        slots = term.get('pool_slots')
        if isinstance(highwater, int) and isinstance(slots, int):
            add('highwater_within_pool_capacity', highwater <= slots * 256,
                '%d <= %d * 256' % (highwater, slots))
        else:
            add('highwater_within_pool_capacity', False, 'non-integer highwater/slots fields')
        add_eq('nonzero_mode_received_328704', received, 328704)
        if mode == 1:
            add_eq('mode1_all_response_pool_slots_2', term.get('pool_slots'), 2)
            add_eq('mode1_no_stream_grants', term.get('grants_stream'), 0)
        if mode in (2, 3):
            add_eq('mode2or3_pool_slots_1', term.get('pool_slots'), 1)
            add('mode2or3_has_stream_grants', isinstance(grants_stream, int) and grants_stream > 0,
                grants_stream)

    if facts is not None:
        for check_id, got, expected in (
            ('evidence_e2_mode', win.get('e2_mode'), facts['e2_mode']),
            ('evidence_condition_v_cold_dst', win.get('v_cold_dst'), facts['cold']),
            ('evidence_mode_string', win.get('mode'), facts['mode']),
            ('evidence_W_cycles', win.get('W_cycles'), facts['W_cycles']),
            ('evidence_window_fast', window.get('fast'), facts['fast']),
            ('evidence_ds_reads', ddr.get('completed_window_ds_reads'), facts['reads']),
            ('evidence_pool_slots', term.get('pool_slots'), facts['pool_slots']),
            ('evidence_dma_buffer_depth', term.get('dma_buffer_depth'), facts['depth']),
            ('evidence_grants_pool', term.get('grants_pool'), facts['grants_pool']),
            ('evidence_grants_stream', term.get('grants_stream'), facts['grants_stream']),
            ('evidence_highwater_beats', term.get('highwater_beats'), facts['highwater']),
        ):
            add_eq(check_id, got, expected, kind='matches_draft')

    log_checks, log_info = check_log(ev, run, name)
    checks.extend(log_checks)

    raw_facts = {
        'name': name,
        'source_file': source_file,
        'index': index,
        'mode': win.get('mode') if win.get('mode') in ('p', 'pn') else params.get('mode'),
        'e2_mode': mode,
        'cold_dst': win.get('v_cold_dst'),
        'W_cycles': win.get('W_cycles'),
        'window_open_cycle': win.get('win_open_cycle'),
        'window_close_cycle': win.get('win_close_cycle'),
        'v_completed': window.get('v_completed'),
        'a_completed': window.get('a_completed'),
        'fast': window.get('fast'),
        'completed_window_ds_reads': ddr.get('completed_window_ds_reads'),
        'pool_slots': term.get('pool_slots'),
        'dma_buffer_depth': term.get('dma_buffer_depth'),
        'payload_bytes': payload,
        'pool_capacity_bytes': pool_cap,
        'private_payload_bytes': private_bytes,
        'grants_pool': term.get('grants_pool'),
        'grants_stream': term.get('grants_stream'),
        'received_beats': term.get('received_beats'),
        'replayed_beats': term.get('replayed_beats'),
        'highwater_beats': term.get('highwater_beats'),
        'log_path': run.get('log'),
        'log_basename': log_info.get('log_basename'),
        'log_exists': log_info.get('exists'),
        'event_trace_path': trace,
        'binary_path': binary,
    }
    required_failed = [check['id'] for check in checks if check['required'] and not check['pass']]
    advisory_failed = [check['id'] for check in checks if not check['required'] and not check['pass']]
    return {'name': name, 'source_file': source_file, 'index': index, 'checks': checks,
            'facts': raw_facts, 'log': log_info, 'pass': not required_failed,
            'required_failures': required_failed, 'advisory_failures': advisory_failed}


def build_summary_table(lookup, payload_by_mode):
    def cycles(key):
        entry = lookup.get(key)
        return entry.get('W_cycles') if entry else None

    def fast(key):
        entry = lookup.get(key)
        return entry.get('fast') if entry else None

    def row(method, mode, condition, p_key, pn_key):
        p_cycles = cycles(p_key)
        pn_cycles = cycles(pn_key)
        percent = None
        if isinstance(p_cycles, int) and p_cycles and isinstance(pn_cycles, int):
            percent = round((pn_cycles - p_cycles) * 100.0 / p_cycles, 4)
        return {'method': method, 'payload_bytes': payload_by_mode.get(mode), 'condition': condition,
                'P_cycles': p_cycles, 'PN_cycles': pn_cycles, 'P_fast': fast(p_key),
                'PN_fast': fast(pn_key), 'pn_vs_p_time_percent': percent}

    return [
        row('mode0-bypass', 0, 'pn-hot (v-cold-dst=0)', None, 'bypass-pn-hot'),
        row('mode1-ordinary', 1, 'hot (v-cold-dst=0)', 'ordinary-p-hot', 'ordinary-pn-hot'),
        row('mode1-ordinary', 1, 'cold (v-cold-dst=1)', 'ordinary-p-cold', 'ordinary-pn-cold'),
        row('mode2-selective', 2, 'hot (v-cold-dst=0)', 'selective-p-hot', 'selective-pn-hot'),
        row('mode2-selective', 2, 'cold (v-cold-dst=1)', 'selective-p-cold', 'selective-pn-cold'),
        row('mode3-reserved', 3, 'hot (v-cold-dst=0)', 'reserved-p-hot', 'reserved-pn-hot'),
        row('mode3-reserved', 3, 'cold (v-cold-dst=1)', 'reserved-p-cold', 'reserved-pn-cold'),
    ]


def build_derived_ratios(lookup):
    def cycles(key):
        entry = lookup.get(key) or {}
        return entry.get('W_cycles')

    def percent(newer, older):
        if not isinstance(newer, int) or not isinstance(older, int) or older == 0:
            return None
        return round((newer - older) * 100.0 / older, 4)

    return {
        'selective_vs_ordinary_pn_hot_percent':
            percent(cycles('selective-pn-hot'), cycles('ordinary-pn-hot')),
        'reserved_vs_selective_pn_hot_percent':
            percent(cycles('reserved-pn-hot'), cycles('selective-pn-hot')),
        'reserved_vs_ordinary_pn_hot_percent':
            percent(cycles('reserved-pn-hot'), cycles('ordinary-pn-hot')),
        'pn_hot_vs_mode0_bypass_percent': {
            'ordinary': percent(cycles('ordinary-pn-hot'), cycles('bypass-pn-hot')),
            'selective': percent(cycles('selective-pn-hot'), cycles('bypass-pn-hot')),
            'reserved': percent(cycles('reserved-pn-hot'), cycles('bypass-pn-hot')),
        },
    }


def compare_tables(draft, built):
    problems = []
    if len(draft) != len(built):
        problems.append('row count draft %d vs evidence %d' % (len(draft), len(built)))
        return problems
    for index, (row_draft, row_built) in enumerate(zip(draft, built)):
        for key in ('method', 'payload_bytes', 'condition', 'P_cycles', 'PN_cycles', 'P_fast', 'PN_fast'):
            if row_draft.get(key) != row_built.get(key):
                problems.append('row %d %s: draft %r vs evidence %r' %
                                (index, key, row_draft.get(key), row_built.get(key)))
        pct_draft = row_draft.get('pn_vs_p_time_percent')
        pct_built = row_built.get('pn_vs_p_time_percent')
        if (pct_draft is None) != (pct_built is None):
            problems.append('row %d percent: draft %r vs evidence %r' % (index, pct_draft, pct_built))
        elif pct_draft is not None and abs(float(pct_draft) - float(pct_built)) > 0.01:
            problems.append('row %d percent: draft %r vs evidence %r' % (index, pct_draft, pct_built))
    return problems


def compare_ratios(draft, built):
    problems = []
    for key, value in draft.items():
        got = built.get(key)
        if isinstance(value, dict):
            if not isinstance(got, dict):
                problems.append('%s: missing in evidence' % key)
                continue
            for sub_key, sub_value in value.items():
                sub_got = got.get(sub_key)
                if sub_got is None or abs(float(sub_got) - float(sub_value)) > 0.01:
                    problems.append('%s.%s: draft %r vs evidence %r' % (key, sub_key, sub_value, sub_got))
        elif got is None or abs(float(got) - float(value)) > 0.01:
            problems.append('%s: draft %r vs evidence %r' % (key, value, got))
    return problems


def normalize_manifest(data):
    def from_mapping(mapping, role=None):
        entries = []
        for key, value in mapping.items():
            if isinstance(value, dict):
                entries.append({'path': str(key), 'sha256': value.get('sha256') or value.get('hash'),
                                'role': value.get('role') or role})
            elif isinstance(value, str) or value is None:
                entries.append({'path': str(key),
                                'sha256': value if isinstance(value, str) else None, 'role': role})
        return entries

    if isinstance(data, list):
        entries = []
        for item in data:
            if isinstance(item, str):
                entries.append({'path': item, 'sha256': None, 'role': None})
            elif isinstance(item, dict):
                entries.append({'path': str(item.get('path') or item.get('file') or item.get('name')),
                                'sha256': item.get('sha256') or item.get('hash'),
                                'role': item.get('role')})
        return entries, 'list_of_entries'
    if isinstance(data, dict):
        for key in ('sources', 'files', 'entries', 'artifacts', 'paths'):
            value = data.get(key)
            if isinstance(value, dict):
                return from_mapping(value, data.get('role')), 'mapping_and_%s' % key
            if isinstance(value, list):
                entries, _ = normalize_manifest(value)
                return entries, 'mapping_and_%s' % key
        flat = from_mapping(data)
        if flat:
            return flat, 'flat_path_to_hash'
    return None, 'unsupported'


def load_manifest(ev, out_dir):
    for name in MANIFEST_CANDIDATES:
        path, text = ev.open_text(out_dir / name, 'sources manifest')
        if text is None:
            continue
        try:
            data = json.loads(text)
        except ValueError as exc:
            return {'found': True, 'path': str(path), 'schema': 'unparsable_json',
                    'error': str(exc), 'entries': [], 'interpreted': False}
        entries, schema = normalize_manifest(data)
        return {'found': True, 'path': str(path), 'schema': schema, 'entries': entries or [],
                'interpreted': entries is not None}
    return {'found': False, 'path': None, 'schema': None, 'entries': [], 'interpreted': True,
            'note': 'no sources manifest found in out_dir; verification falls back to previous-production-source-hashes.json'}


def check_source_hashes(ev, hashes, source_base):
    entries = []
    matched = 0
    mismatched = 0
    missing = 0
    for rel in sorted(hashes):
        expected = hashes[rel]
        if not isinstance(expected, str):
            entries.append({'path': str(rel), 'status': 'bad_hash_value', 'expected': expected})
            mismatched += 1
            continue
        ev.allow(source_base / str(rel))
        try:
            path, digest = ev.digest(source_base / str(rel), 'declared production source')
        except Violation as exc:
            entries.append({'path': str(rel), 'status': 'outside_allow_list', 'detail': str(exc)})
            mismatched += 1
            continue
        if digest is None:
            entries.append({'path': str(rel), 'resolved': str(path), 'status': 'missing',
                            'expected': expected})
            missing += 1
        elif digest == expected:
            entries.append({'path': str(rel), 'resolved': str(path), 'status': 'match'})
            matched += 1
        else:
            entries.append({'path': str(rel), 'resolved': str(path), 'status': 'mismatch',
                            'expected': expected, 'found': digest})
            mismatched += 1
    return {'source_base': str(source_base), 'total': len(hashes), 'matched': matched,
            'mismatched': mismatched, 'missing': missing, 'entries': entries}


def check_report(ev, out_dir, requested):
    if requested:
        candidate = Path(requested)
        path = candidate if candidate.is_absolute() else (Path.cwd() / candidate)
    else:
        path = None
        for name in REPORT_CANDIDATES:
            if (out_dir / name).is_file():
                path = out_dir / name
                break
    if path is None:
        return {'found': False, 'note': 'no report file present in out_dir; pass --report to scan one'}, []
    try:
        resolved, text = ev.open_text(path, 'optional report markdown')
    except Violation as exc:
        return {'found': True, 'path': str(path), 'readable': False, 'detail': str(exc)}, []
    if text is None:
        return {'found': True, 'path': str(resolved), 'readable': False}, []
    checks = []
    hits = []
    for check_id, pattern in REPORT_FORBIDDEN_PATTERNS:
        found = re.search(pattern, text, re.IGNORECASE) is not None
        if found:
            hits.append(check_id)
        checks.append({'id': 'report_%s' % check_id, 'pass': not found, 'required': True,
                       'kind': 'report', 'detail': 'forbidden claim pattern %s' % pattern})
    info = {'found': True, 'path': str(resolved), 'bytes': len(text), 'forbidden_hits': hits,
            'scanned_patterns': [pattern for _, pattern in REPORT_FORBIDDEN_PATTERNS]}
    return info, checks


def _abs(raw, cwd):
    path = Path(str(raw))
    return path.resolve() if path.is_absolute() else (cwd / path).resolve()


def _write_json(path, payload):
    path.write_text(json.dumps(payload, indent=2) + '\n', encoding='utf-8')


def main(argv=None):
    parser = argparse.ArgumentParser(description='E2 POC evidence-only acceptance verifier')
    parser.add_argument('--out-dir', default=OUT_REL)
    parser.add_argument('--source-base', default='.')
    parser.add_argument('--report', default=None)
    args = parser.parse_args(argv)

    cwd = Path.cwd().resolve()
    out_dir = _abs(args.out_dir, cwd)
    source_base = _abs(args.source_base, cwd)

    acceptance = {
        'schema': 'e2-interface-poc-acceptance-v1',
        'generated_by': 'accept_e2_poc.py',
        'claims_execution': False,
        'ran_simulation': False,
        'ran_build': False,
        'ran_lint_or_synthesis': False,
        'project_root': str(cwd),
        'out_dir': str(out_dir),
        'source_base': str(source_base),
        'fatal': [],
        'gates': [],
        'per_run': [],
        'required_check_failures': [],
        'required_gate_failures': [],
        'source_hash_check': {},
        'manifest_check': {},
        'report_check': {},
        'prototype_source_scope': {},
        'trace_evidence': [],
        'reads_performed': [],
        'read_violations': [],
        'overall_pass': False,
        'notes': [
            'evidence-only: no simulation, build, lint, synthesis or board step is executed',
            'raw evidence JSON fields and run-log strings are checked; the reported first-pair trace ordering is not recomputed',
        ],
    }
    comparison = {
        'schema': 'e2-interface-poc-comparison-v1',
        'generated_by': 'accept_e2_poc.py',
        'claims_execution': False,
        'fixture': {
            'system': 'actual DMA system, two DMAs, one executing job per DMA',
            'job': 'normal aligned 4096-byte copy',
            'a_pages': 8,
            'transfers': 64,
            'warmup': 256,
            'ptw_latency': 31,
            'v_gap': 0,
            'read_admit': 'buffer',
            'read_budget': 256,
            'read_period': 256,
            'window_definition': 'hardware_first_V_issue_to_final_V_done_(open,close]',
            'metric': 'W_cycles = win_close_cycle - win_open_cycle',
        },
        'conditions': {'hot': 'v-cold-dst=0 (destination warm, fill_v not exercised)',
                       'cold': 'v-cold-dst=1 (fill_v exercised)'},
        'modes': {
            '0': 'unchanged bypass, private depth 512 per DMA, no pool',
            '1': 'ordinary all-response pool: 2 x 2048 B slots plus 2 x 24 B private',
            '2': 'selective streaming plus one 2048 B slot (also a generic dependency-aware pool)',
            '3': 'same capacity as mode 2, withholds the current W owner read-ahead pool allocation',
        },
        'summary_table': [],
        'derived_ratios_percent': {},
        'reference_previous_run_excluded_from_13': REFERENCE_PREVIOUS_RUN_EXCLUDED,
        'runs': [],
        'evidence_paths': [],
        'limitations': list(LIMITATIONS),
        'unverified_claims': [
            'first-pair event ordering (bypass second V DDR AR 26 before first V Rlast 334; selective second V AR 1234 after first V Rlast 330; reserved 1238 after 334)',
            'the three new test sources other than tests/nonblock_completion_pool.sv',
            'sources manifest mapping schema (file absent from the supplied evidence)',
        ],
    }

    ev = Evidence(cwd, out_dir)

    if not out_dir.is_dir():
        acceptance['fatal'].append({'id': 'out_dir_missing', 'detail': str(out_dir)})

    initial = []
    reserved = []
    hashes = {}
    load_errors = []
    for name, key in ((INITIAL_RUNS_NAME, 'initial'), (RESERVED_RUNS_NAME, 'reserved'),
                      (HASHES_NAME, 'hashes')):
        path, text = ev.open_text(out_dir / name, 'required evidence %s' % name)
        if text is None:
            load_errors.append({'file': name, 'error': 'missing or unreadable'})
            continue
        try:
            data = json.loads(text)
        except ValueError as exc:
            load_errors.append({'file': name, 'error': 'invalid json: %s' % exc})
            continue
        if key == 'initial':
            initial = data if isinstance(data, list) else []
        elif key == 'reserved':
            reserved = data if isinstance(data, list) else []
        else:
            hashes = data if isinstance(data, dict) else {}
    if load_errors:
        acceptance['fatal'].append({'id': 'required_evidence_unusable', 'detail': load_errors})

    per_run = []
    lookup = {}
    for source_file, runs in ((INITIAL_RUNS_NAME, initial), (RESERVED_RUNS_NAME, reserved)):
        for index, run in enumerate(runs):
            if not isinstance(run, dict):
                per_run.append({'name': '%s[%d]' % (source_file, index), 'source_file': source_file,
                                'index': index,
                                'checks': [{'id': 'run_entry_is_object', 'pass': False,
                                            'required': True, 'kind': 'check',
                                            'detail': 'run entry is not a JSON object'}],
                                'facts': {}, 'log': {}, 'pass': False,
                                'required_failures': ['run_entry_is_object'], 'advisory_failures': []})
                continue
            detail = check_run(ev, source_file, index, run)
            per_run.append(detail)
            lookup[detail['name']] = detail['facts']

    acceptance['per_run'] = [
        {'name': run['name'], 'source_file': run['source_file'], 'pass': run.get('pass'),
         'required_failures': run.get('required_failures', []),
         'advisory_failures': run.get('advisory_failures', []),
         'checks': run['checks']} for run in per_run
    ]

    trace_entries = []
    for run in per_run:
        raw = run['facts'].get('event_trace_path')
        entry = {'name': run['name'], 'event_trace_path': raw, 'exists': False}
        if raw:
            try:
                resolved = ev.resolve(raw, 'event trace existence check')
                entry['path'] = str(resolved)
                entry['exists'] = resolved.is_file()
            except Violation as exc:
                entry['violation'] = str(exc)
        trace_entries.append(entry)
    acceptance['trace_evidence'] = trace_entries

    payload_by_mode = {}
    for facts in lookup.values():
        if isinstance(facts.get('e2_mode'), int):
            payload_by_mode.setdefault(facts['e2_mode'], facts.get('payload_bytes'))
    comparison['summary_table'] = build_summary_table(lookup, payload_by_mode)
    table_problems = compare_tables(DRAFT_SUMMARY_TABLE, comparison['summary_table'])
    comparison['derived_ratios_percent'] = build_derived_ratios(lookup)
    ratio_problems = compare_ratios(DRAFT_DERIVED_RATIOS, comparison['derived_ratios_percent'])
    comparison['runs'] = [run['facts'] for run in per_run]

    required_failures = []
    for run in per_run:
        for check in run['checks']:
            if check['required'] and not check['pass']:
                required_failures.append({'run': run['name'], 'check': check['id'],
                                          'detail': check['detail']})

    for rel in hashes:
        ev.allow(source_base / str(rel))
    source_hash = check_source_hashes(ev, hashes, source_base)
    acceptance['source_hash_check'] = source_hash

    manifest = load_manifest(ev, out_dir)
    manifest_hash_problems = []
    for entry in manifest.get('entries') or []:
        rel = entry.get('path')
        expected = entry.get('sha256')
        if not rel or not expected:
            continue
        found = None
        for candidate in (source_base / str(rel), cwd / str(rel), out_dir / str(rel)):
            try:
                ev.allow(candidate)
                resolved, digest = ev.digest(candidate, 'sources manifest declared source')
            except Violation as exc:
                manifest_hash_problems.append('%s: %s' % (rel, exc))
                break
            if digest is not None:
                found = digest
                break
        if found is None:
            manifest_hash_problems.append('%s: not found under source_base, cwd or out_dir' % rel)
        elif found != expected:
            manifest_hash_problems.append('%s: expected %s found %s' % (rel, expected, found))
    manifest['hash_verification'] = {
        'checked': len([e for e in (manifest.get('entries') or []) if e.get('sha256')]),
        'problems': manifest_hash_problems,
    }
    acceptance['manifest_check'] = manifest

    design_problems = []
    design_advisory = []
    design_path, design_text = ev.open_text(out_dir / DESIGN_NAME, 'design notes')
    if design_text is None:
        design_problems.append('design.md not readable at %s' % design_path)
    else:
        match = re.search('mode0 ([0-9]+) B, mode1 ([0-9]+) B, mode2 ([0-9]+) B', design_text)
        if match is None:
            design_problems.append('design.md capacity sentence not found')
        else:
            stated = tuple(int(value) for value in match.groups())
            if stated != (8192, 4144, 2096):
                design_problems.append('design.md capacity sentence states %s' % (stated,))
        for marker, label in (('private depth 512', 'mode0 private depth 512'),
                              ('private depth 3', 'private depth 3'),
                              ('two 2KiB slots', 'mode1 two 2KiB slots')):
            if marker not in design_text:
                design_problems.append('design.md does not state: %s' % label)
        if 'mode 3' not in design_text.lower() and 'mode3' not in design_text.lower():
            design_advisory.append('design.md does not document mode 3 semantics (documented in tests/nonblock_completion_pool.sv and in the draft report)')
    comparison['design_check'] = {'path': str(design_path), 'problems': design_problems,
                                  'advisory': design_advisory}

    ev.allow(cwd / POOL_SV_REL)
    pool_path, pool_text = ev.open_text(cwd / POOL_SV_REL, 'prototype test source')
    scope_checks = []
    pool_digest = None
    if pool_text is None:
        scope_checks.append({'id': 'prototype_test_source_present', 'pass': False, 'required': True,
                             'kind': 'scope', 'detail': str(cwd / POOL_SV_REL)})
    else:
        pool_digest = sha256_file(pool_path)
        scope_checks.append({'id': 'prototype_test_source_present', 'pass': True, 'required': True,
                             'kind': 'scope', 'detail': str(pool_path)})
        for check_id, pattern in (('scope_test_only_header', 'TEST ONLY'),
                                  ('scope_not_general_axi_bridge', 'Not a general AXI bridge'),
                                  ('scope_segment_match_present', 'function automatic logic segment_match'),
                                  ('scope_mode3_branch_present', 'Mode == 3'),
                                  ('scope_256_beat_slot_budget', 'storage [Slots][256]'),
                                  ('scope_grant_counters_present', 'grant_pool_o')):
            scope_checks.append({'id': check_id, 'pass': pattern in pool_text, 'required': True,
                                 'kind': 'scope', 'detail': pattern})
        fault_bits = sorted({int(value) for value in re.findall('fault_o[[]([0-9]+)[]]', pool_text)})
        scope_checks.append({'id': 'scope_fault_bits_cover_0_to_10',
                             'pass': fault_bits == list(range(11)), 'required': True, 'kind': 'scope',
                             'detail': 'fault bits: %s' % fault_bits})
    acceptance['prototype_source_scope'] = {
        'declared_new_test_sources': 4,
        'supplied_new_test_sources': [str(pool_path)] if pool_text is not None else [],
        'supplied_new_test_source_sha256': pool_digest,
        'unresolved_new_test_source_count': 3 if pool_text is not None else 4,
        'checks': scope_checks,
        'note': 'the other three new test source files were not supplied; resolve their names from the sources manifest before claiming full prototype scope coverage',
    }

    report_info, report_checks = check_report(ev, out_dir, args.report)
    acceptance['report_check'] = dict(report_info)
    acceptance['report_check']['checks'] = report_checks
    acceptance['report_check']['claims_execution'] = False

    gates = []

    def gate(gate_id, ok, detail='', required=True):
        gates.append({'id': gate_id, 'pass': bool(ok), 'required': bool(required),
                      'detail': str(detail)})

    gate('initial_run_count_9', len(initial) == 9, 'initial-runs entries: %d' % len(initial))
    gate('reserved_run_count_4', len(reserved) == 4, 'reserved-runs entries: %d' % len(reserved))
    gate('total_run_count_13', len(per_run) == 13, 'total entries: %d' % len(per_run))
    gate('run_names_unique_and_match_draft',
         len(lookup) == len(per_run) and set(lookup) == set(DRAFT_RUN_FACTS),
         'evidence names: %s' % sorted(lookup))
    gate('all_runs_pass_required_checks', not required_failures,
         '%d required run-check failures' % len(required_failures))
    gate('summary_table_matches_raw_evidence', not table_problems, '; '.join(table_problems))
    gate('payload_points_match_terminal_records', payload_by_mode == DRAFT_PAYLOAD_BY_MODE,
         'evidence %s vs declared %s' % (payload_by_mode, DRAFT_PAYLOAD_BY_MODE))
    gate('derived_ratios_match_reported_values', not ratio_problems, '; '.join(ratio_problems))
    gate('production_sources_unchanged', source_hash['mismatched'] == 0,
         '%d mismatch of %d declared files' % (source_hash['mismatched'], source_hash['total']))
    gate('production_source_files_present', source_hash['missing'] == 0,
         '%d missing of %d declared files (source_base %s)' %
         (source_hash['missing'], source_hash['total'], source_base))
    if manifest.get('found'):
        gate('sources_manifest_processed', bool(manifest.get('interpreted')),
             'schema=%s path=%s entries=%d' % (manifest.get('schema'), manifest.get('path'),
                                              len(manifest.get('entries') or [])))
    else:
        gate('sources_manifest_processed', True, manifest.get('note', ''), required=False)
    gate('sources_manifest_hashes_match', not manifest_hash_problems, '; '.join(manifest_hash_problems))
    gate('prototype_scope_is_test_only',
         not any(not check['pass'] for check in scope_checks if check['required']),
         'checks=%d' % len(scope_checks))
    gate('design_capacity_points_documented', not design_problems, '; '.join(design_problems))
    gate('design_documents_mode3', not design_advisory, '; '.join(design_advisory), required=False)
    if report_info.get('found'):
        gate('report_scanned_and_clean', not report_info.get('forbidden_hits'),
             'forbidden hits: %s' % report_info.get('forbidden_hits'))
    else:
        gate('report_scanned_and_clean', True, report_info.get('note', ''), required=False)
    gate('no_read_violations', not ev.violations, '; '.join(ev.violations))

    mode0_facts = [facts for facts in lookup.values() if facts.get('e2_mode') == 0]
    gate('mode0_unchanged_bypass_zero_pool',
         len(mode0_facts) == 1 and mode0_facts[0].get('dma_buffer_depth') == 512 and
         mode0_facts[0].get('pool_slots') == 0 and mode0_facts[0].get('grants_pool') == 0 and
         mode0_facts[0].get('grants_stream') == 0 and mode0_facts[0].get('payload_bytes') == 8192 and
         mode0_facts[0].get('W_cycles') == 93487 and mode0_facts[0].get('fast') == 62,
         'mode0 evidence: %s' % (mode0_facts,))

    selective_hot = lookup.get('selective-pn-hot') or {}
    reserved_hot = lookup.get('reserved-pn-hot') or {}
    gate('mode3_fewer_pool_more_stream_grants_than_mode2',
         isinstance(reserved_hot.get('grants_pool'), int) and
         isinstance(selective_hot.get('grants_pool'), int) and
         reserved_hot['grants_pool'] < selective_hot['grants_pool'] and
         isinstance(reserved_hot.get('grants_stream'), int) and
         isinstance(selective_hot.get('grants_stream'), int) and
         reserved_hot['grants_stream'] > selective_hot['grants_stream'],
         'mode3 %s vs mode2 %s' % (reserved_hot.get('grants_pool'), selective_hot.get('grants_pool')))
    gate('mode3_fast_zero_and_slower_than_mode2',
         reserved_hot.get('fast') == 0 and isinstance(reserved_hot.get('W_cycles'), int) and
         isinstance(selective_hot.get('W_cycles'), int) and
         reserved_hot['W_cycles'] > selective_hot['W_cycles'],
         'mode3 W %s fast %s vs mode2 W %s' % (reserved_hot.get('W_cycles'), reserved_hot.get('fast'),
                                               selective_hot.get('W_cycles')))
    gate('reference_previous_run_kept_out_of_13',
         not any(facts.get('W_cycles') in (92334, 91727) for facts in lookup.values()),
         'no run in the 13 may carry the previous large-private reference cycles')
    gate('selective_pn_hot_uses_awtap_fix_log',
         str((lookup.get('selective-pn-hot') or {}).get('log_basename') or '').endswith('awtap-fix.log'),
         'log basename: %s' % (lookup.get('selective-pn-hot') or {}).get('log_basename'), required=False)
    gate('event_trace_files_present',
         all(entry.get('exists') for entry in trace_entries),
         'present: %d of %d' % (sum(1 for entry in trace_entries if entry.get('exists')), len(trace_entries)),
         required=False)

    acceptance['gates'] = gates
    acceptance['required_check_failures'] = required_failures
    acceptance['required_gate_failures'] = [item for item in gates if item['required'] and not item['pass']]
    acceptance['advisory_gate_failures'] = [item['id'] for item in gates if not item['required'] and not item['pass']]
    acceptance['reads_performed'] = ev.reads
    acceptance['read_violations'] = ev.violations
    acceptance['overall_pass'] = (not acceptance['fatal'] and not ev.violations and
                                  not required_failures and
                                  not acceptance['required_gate_failures'])
    comparison['evidence_paths'] = sorted({read['path'] for read in ev.reads})

    _write_json(ev.out_path(COMPARISON_NAME), comparison)
    _write_json(ev.out_path(ACCEPTANCE_NAME), acceptance)

    if acceptance['fatal'] or ev.violations:
        return 2
    if not acceptance['overall_pass']:
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())

