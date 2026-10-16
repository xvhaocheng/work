#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
'''Generic read-only audit aggregator for research2 E2 runs/<id> trees.

Usage::

    scripts/e2_audit_aggregate.py INPUT_DIR --output NEW_DIR
    scripts/e2_audit_aggregate.py INPUT_DIR --output NEW_DIR --hash-archives
    scripts/e2_audit_aggregate.py INPUT_DIR --output NEW_DIR \
        --legacy-analyzer scripts/research_results.py

INPUT_DIR is a finished research2 E2 result directory: it carries plan.json
and runs/<case_id>/.  It is opened strictly read-only.  Nothing is written,
renamed, deleted or executed inside it, and no board access is attempted.

Reads per case:
    runs/<case_id>/record.json     CLI record; either the full 4096-word raw
                                   list or the lossless raw_sparse map plus
                                   raw_length (omitted indices are zero)
    runs/<case_id>/metadata.json   argv / exit / taint / host / hash metadata
    runs/<case_id>/*               every other side file is listed and hashed
                                   (dmesg, uname, exit, taint, argv, sha256)
    runs/<case_id>/archive.tar.gz  hashed only with --hash-archives

Writes into NEW_DIR (which must not exist):
    per_run.tsv    one row per planned case, every extracted field
    summary.json   grouped count/min/median/max, fixed checks, board evidence

Rules:
  * The record's own success/validated booleans are recorded, never trusted;
    every statement is re-derived from record fields and raw words.
  * No field is invented.  An absent key stays absent and is reported absent.
  * The sparse raw encoding omits zero words; an explicit zero in the sparse
    map is a malformed record, not a silent accept.
  * scripts/research_results.py is used only with --legacy-analyzer PATH and
    only if it imports cleanly; it is never a hard dependency and its absence
    is never treated as a failure of this aggregator.
  * The tool never runs the board, never retries, and never drops a case.
'''

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import re
import statistics
import sys
from pathlib import Path

RAW_WORDS = 4096
PAGE_BYTES = 4096
TAB = chr(9)
NEWLINE = chr(10)
CARRIAGE = chr(13)

HEX16 = re.compile('[0-9a-fA-F]{16}')

RESEARCH_MAGIC0 = 0x49444D4152455331
OBSERVER_MAGIC = 0x49444D414F425308
CAP_MAGIC = 0x49444D4132434150
E2T_MAGIC = 0x49444D4145325431
CAP_FEATURES = {1: 7, 2: 15}

E2T_BASE = 0xD80
CAP_BASE = 0xF00
CAP_REVERSE_WORD = 0xF10

RECORD_SCHEMA = 'idma-research-v1'
METADATA_SCHEMA = 'research2-case-metadata-v1'

PHASE_STATS_FIELDS = (
    'submitted', 'completed', 'completed_bytes', 'active_cycles',
    'iotlb_miss', 's1_ptw', 's2_ptw',
    'refill_gap_ns_sum', 'refill_gap_ns_max', 'refill_gap_samples',
    'in_flight_at_start', 'in_flight_at_end', 'data_errors', 'quarantined',
)

RAW_FIELDS = (
    ('c_status', 0x002),
    ('c_e2_phase', 0x006),
    ('c_astart', 0x007),
    ('c_a_elapsed', 0x008),
    ('c_event_count', 0x00A),
    ('c_inval_count', 0x00B),
    ('c_e2_ovf', 0x00C),
    ('c_e2_err', 0x00D),
    ('c_update_count', 0x00E),
    ('c_target_valid', 0x01C),
    ('c_fp_done', 0x01D),
    ('c_deadline', 0x01E),
    ('c_probestart', 0x01F),
    ('c_freeze_ts', 0x020),
    ('c_snapshot_valid', 0x021),
    ('c_a_issue_cnt', 0x025),
    ('c_a_done_cnt', 0x026),
    ('c_a_first_ts', 0x027),
    ('c_a_last_ts', 0x028),
    ('d80_magic', 0xD80),
    ('d81_version', 0xD81),
    ('d82_error', 0xD82),
    ('d83_flags', 0xD83),
    ('d84_issue_epoch', 0xD84),
    ('d85_first_select_epoch', 0xD85),
    ('d86_done_epoch', 0xD86),
    ('d87_done_issue_cycles', 0xD87),
    ('d88_first_select_issue_cycles', 0xD88),
    ('d89_v_main_cycles', 0xD89),
    ('d8a_v_complete_cycles', 0xD8A),
    ('d8b_v_select', 0xD8B),
    ('d8c_v_hit', 0xD8C),
    ('d8d_v_miss', 0xD8D),
    ('caps_magic', 0xF00),
    ('caps_version', 0xF01),
    ('caps_role', 0xF02),
    ('caps_features', 0xF03),
    ('raw_reverse_readback', 0xF10),
)

SUMMARY_METRICS = (
    'p0_submitted', 'p0_completed', 'p0_bytes', 'p0_active_cycles',
    'p0_iotlb_miss', 'p0_s1_ptw', 'p0_refill_gap_ns_sum',
    'p1_submitted', 'p1_completed', 'p1_bytes', 'p1_active_cycles',
    'p1_iotlb_miss', 'p1_s1_ptw', 'p1_refill_gap_ns_sum',
    'p2_submitted', 'p2_completed', 'p2_bytes', 'p2_active_cycles',
    'p2_iotlb_miss', 'p2_s1_ptw', 'p2_refill_gap_ns_sum',
    'd84_issue_epoch', 'd85_first_select_epoch', 'd86_done_epoch',
    'd87_done_issue_cycles', 'd88_first_select_issue_cycles',
    'd89_v_main_cycles', 'd8a_v_complete_cycles',
    'd8b_v_select', 'd8c_v_hit', 'd8d_v_miss',
    'c_event_count', 'c_update_count', 'c_inval_count',
    'c_a_issue_cnt', 'c_a_done_cnt', 'c_a_first_ts', 'c_a_last_ts',
    'c_astart', 'c_a_elapsed', 'c_deadline', 'c_probestart', 'c_freeze_ts',
    'input_a_pages', 'input_a_transfers', 'input_window_cycles',
    'caps_role', 'caps_features', 'raw_reverse_readback',
    'checks_failed_count',
)

TSV_COLUMNS = (
    'id', 'ordinal', 'phase', 'condition', 'stage', 'role', 'target',
    'method', 'block', 'a_pages', 'a_transfers', 'reverse',
    'record_state', 'metadata_state', 'run_dir_present',
    'success', 'validated', 'syscall_rc', 'errno', 'stage_value',
    'flush_status', 'raw_captured', 'observer_magic', 'errors_count',
    'input_mode', 'input_a_pages', 'input_a_transfers', 'input_window_cycles',
    'input_warmup', 'input_transfers', 'input_gap_cycles', 'input_timeout_ms',
    'input_raw_words', 'input_abi_version', 'input_struct_size',
    'p0_submitted', 'p0_completed', 'p0_bytes', 'p0_active_cycles',
    'p0_iotlb_miss', 'p0_s1_ptw', 'p0_s2_ptw', 'p0_refill_gap_ns_sum',
    'p0_refill_gap_ns_max', 'p0_refill_gap_samples', 'p0_data_errors',
    'p0_quarantined',
    'p1_submitted', 'p1_completed', 'p1_bytes', 'p1_active_cycles',
    'p1_iotlb_miss', 'p1_s1_ptw', 'p1_s2_ptw', 'p1_refill_gap_ns_sum',
    'p1_refill_gap_ns_max', 'p1_refill_gap_samples', 'p1_data_errors',
    'p1_quarantined',
    'p2_submitted', 'p2_completed', 'p2_bytes', 'p2_active_cycles',
    'p2_iotlb_miss', 'p2_s1_ptw', 'p2_s2_ptw', 'p2_refill_gap_ns_sum',
    'p2_refill_gap_ns_max', 'p2_refill_gap_samples', 'p2_data_errors',
    'p2_quarantined',
    'mapping0', 'mapping1', 'mapping2', 'mapping3',
    'cfg_reverse', 'cfg_policy_mode', 'cfg_window_cycles',
    'cfg_required_role', 'cfg_abi_version', 'cfg_struct_size',
    'caps_magic', 'caps_version', 'caps_role', 'caps_features',
    'raw_reverse_readback',
    'd80_magic', 'd81_version', 'd82_error', 'd83_flags',
    'd84_issue_epoch', 'd85_first_select_epoch', 'd86_done_epoch',
    'd87_done_issue_cycles', 'd88_first_select_issue_cycles',
    'd89_v_main_cycles', 'd8a_v_complete_cycles',
    'd8b_v_select', 'd8c_v_hit', 'd8d_v_miss',
    'c_status', 'c_e2_phase', 'c_astart', 'c_a_elapsed',
    'c_event_count', 'c_inval_count', 'c_e2_ovf', 'c_e2_err',
    'c_update_count', 'c_target_valid', 'c_fp_done', 'c_deadline',
    'c_probestart', 'c_freeze_ts', 'c_snapshot_valid',
    'c_a_issue_cnt', 'c_a_done_cnt', 'c_a_first_ts', 'c_a_last_ts',
    'meta_schema', 'meta_exit', 'meta_exit_code', 'meta_framed_exit',
    'meta_taint_before', 'meta_taint_after', 'meta_recovery',
    'meta_boot_id', 'meta_kernel_release', 'meta_finished_at',
    'meta_bit', 'meta_bit_sha256', 'meta_board_cli', 'meta_board_cli_sha256',
    'meta_record_sha256', 'record_sha256_actual',
    'meta_archive_sha256', 'archive_present', 'archive_sha256_actual',
    'checks_failed_count', 'checks_failed_ids', 'problems',
    'legacy_lifecycle', 'side_files',
)


class UsageError(Exception):
    pass


def _sha256_file(path, chunk=1 << 20):
    digest = hashlib.sha256()
    with open(path, 'rb') as handle:
        while True:
            block = handle.read(chunk)
            if not block:
                break
            digest.update(block)
    return digest.hexdigest()


def _load_json_state(path):
    if not path.is_file():
        return None, 'missing'
    try:
        text = path.read_text(encoding='utf-8', errors='replace')
    except OSError:
        return None, 'unreadable'
    try:
        return json.loads(text), 'ok'
    except ValueError:
        return None, 'malformed'


def _hex64_to_int(value):
    if isinstance(value, bool):
        return None
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        text = value.strip()
        if len(text) > 2 and text[:2].lower() == '0x' and HEX16.fullmatch(text[2:]):
            return int(text[2:], 16)
    return None


def _sparse_index(key):
    if isinstance(key, bool):
        return None
    if isinstance(key, int):
        return key
    if isinstance(key, str):
        text = key.strip()
        try:
            if text[:2].lower() == '0x':
                return int(text, 16)
            return int(text, 10)
        except ValueError:
            return None
    return None


def _sparse_value(value):
    if isinstance(value, bool):
        return None
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        return _hex64_to_int(value)
    return None


def build_raw_words(record, problems):
    '''Re-derive the 4096-word capture from raw or raw_sparse; None on error.'''
    raw_list = record.get('raw')
    sparse = record.get('raw_sparse')
    length = record.get('raw_length')

    if raw_list is not None:
        if not isinstance(raw_list, list):
            problems.append('record.raw is not a list')
            return None
        if len(raw_list) != RAW_WORDS:
            problems.append('record.raw has %d words, want %d'
                            % (len(raw_list), RAW_WORDS))
            return None
        words = []
        for index, item in enumerate(raw_list):
            value = _hex64_to_int(item)
            if value is None:
                problems.append(
                    'record.raw[0x%03x] is not an exact 0x + 16 hex digit word: %r'
                    % (index, item))
                return None
            words.append(value)
        return words

    if sparse is not None:
        if not isinstance(sparse, dict):
            problems.append('record.raw_sparse is not an object')
            return None
        if not isinstance(length, int) or isinstance(length, bool):
            problems.append(
                'record.raw_sparse present but record.raw_length is %r' % (length,))
            return None
        if length != RAW_WORDS:
            problems.append('record.raw_length is %d, want %d'
                            % (length, RAW_WORDS))
            return None
        words = [0] * length
        seen = set()
        for key, value in sparse.items():
            index = _sparse_index(key)
            if index is None or not 0 <= index < length:
                problems.append(
                    'record.raw_sparse key %r is not an index in [0, %d)'
                    % (key, length))
                return None
            if index in seen:
                problems.append('record.raw_sparse key %r is duplicated' % (key,))
                return None
            seen.add(index)
            got = _sparse_value(value)
            if got is None:
                problems.append(
                    'record.raw_sparse[%r] is not a 64-bit integer: %r'
                    % (key, value))
                return None
            if got == 0:
                problems.append(
                    'record.raw_sparse[%r] is an explicit zero; the sparse '
                    'encoding omits zeros, so this is a malformed record'
                    % (key,))
                return None
            words[index] = got
        return words

    problems.append('record carries neither raw nor raw_sparse')
    return None


def plan_cases(plan_doc):
    if not isinstance(plan_doc, dict):
        return []
    cases = plan_doc.get('cases')
    if isinstance(cases, list):
        return cases
    merged = []
    stages = plan_doc.get('stages')
    if isinstance(stages, dict):
        for key in sorted(stages):
            stage = stages[key]
            if isinstance(stage, dict) and isinstance(stage.get('cases'), list):
                merged.extend(stage['cases'])
    return merged


def _arg(value):
    if isinstance(value, list):
        return ' '.join(str(item) for item in value)
    if value is None:
        return None
    return str(value)


def _load_legacy(path):
    if not path:
        return None
    candidate = Path(path)
    if not candidate.is_file():
        return None
    spec = importlib.util.spec_from_file_location('e2_audit_legacy', candidate)
    if spec is None or spec.loader is None:
        return None
    module = importlib.util.module_from_spec(spec)
    try:
        spec.loader.exec_module(module)
    except Exception:
        return None
    return module


def _legacy_view(case):
    base = case.get('base') or {}
    role = case.get('role')
    condition = case.get('condition') or ''
    if role == 1 and base.get('a_pages') == 0:
        condition = 'w0'
    return {
        'id': case.get('id'),
        'ordinal': case.get('ordinal'),
        'stage': case.get('stage'),
        'mode': 'e2' if role == 1 else 'cost',
        'condition': condition if condition else None,
        'apages': base.get('a_pages'),
        'acount': base.get('a_transfers'),
        'window': base.get('window_cycles'),
        'warmup': base.get('warmup'),
        'vcount': base.get('transfers'),
        'gap': base.get('gap_cycles'),
        'timeout': base.get('timeout_ms'),
    }


def audit_case(base_dir, case, hash_archives, legacy_module, preview_bytes):
    case_id = str(case.get('id'))
    run_dir = base_dir / 'runs' / case_id
    row = {
        'id': case_id,
        'ordinal': case.get('ordinal'),
        'phase': case.get('phase'),
        'condition': case.get('condition'),
        'stage': case.get('stage'),
        'role': case.get('role'),
        'target': case.get('target'),
        'method': case.get('method'),
        'block': case.get('block'),
        'a_pages': case.get('a_pages'),
        'a_transfers': case.get('a_transfers'),
        'reverse': case.get('reverse'),
        'run_dir_present': run_dir.is_dir(),
    }
    problems = []
    checks = {}

    def check(name, condition, detail=None):
        ok = bool(condition)
        checks[name] = ok
        if not ok:
            suffix = '' if detail is None else ': ' + str(detail)
            problems.append('check %s failed%s' % (name, suffix))

    record_path = run_dir / 'record.json'
    record, record_state = _load_json_state(record_path)
    metadata, metadata_state = _load_json_state(run_dir / 'metadata.json')
    row['record_state'] = record_state
    row['metadata_state'] = metadata_state

    if record_path.is_file():
        try:
            row['record_sha256_actual'] = _sha256_file(record_path)
        except OSError as exc:
            problems.append('record.json could not be hashed: %s' % exc)
    else:
        problems.append('runs/%s/record.json is %s' % (case_id, record_state))

    if record is not None:
        row['success'] = record.get('success')
        row['validated'] = record.get('validated')
        row['syscall_rc'] = record.get('syscall_rc')
        row['errno'] = record.get('errno')
        row['stage_value'] = record.get('stage')
        row['flush_status'] = record.get('flush_status')
        row['raw_captured'] = record.get('raw_captured')
        row['observer_magic'] = record.get('observer_magic')
        errors = record.get('errors')
        row['errors_count'] = len(errors) if isinstance(errors, list) else None
        if isinstance(errors, list) and errors:
            problems.append('record.errors is not empty: %r' % (errors,))

        inputs = record.get('input')
        if isinstance(inputs, dict):
            row['input_mode'] = inputs.get('mode')
            row['input_a_pages'] = inputs.get('a_pages')
            row['input_a_transfers'] = inputs.get('a_transfers')
            row['input_window_cycles'] = inputs.get('window_cycles')
            row['input_warmup'] = inputs.get('warmup')
            row['input_transfers'] = inputs.get('transfers')
            row['input_gap_cycles'] = inputs.get('gap_cycles')
            row['input_timeout_ms'] = inputs.get('timeout_ms')
            row['input_raw_words'] = inputs.get('raw_words')
            row['input_abi_version'] = inputs.get('abi_version')
            row['input_struct_size'] = inputs.get('struct_size')
        else:
            problems.append('record.input is missing or not an object')

        mapping = record.get('mapping')
        for index in range(4):
            if isinstance(mapping, list) and len(mapping) > index:
                row['mapping%d' % index] = mapping[index]
            else:
                row['mapping%d' % index] = None
        if not isinstance(mapping, list) or len(mapping) != 4:
            problems.append('record.mapping is not a 4-word list')

        research2 = record.get('research2')
        if isinstance(research2, dict):
            config = research2.get('config')
            caps = research2.get('caps')
            if isinstance(config, dict):
                row['cfg_reverse'] = config.get('reverse')
                row['cfg_policy_mode'] = config.get('policy_mode')
                row['cfg_window_cycles'] = config.get('window_cycles')
                row['cfg_required_role'] = config.get('required_role')
                row['cfg_abi_version'] = config.get('abi_version')
                row['cfg_struct_size'] = config.get('struct_size')
            if isinstance(caps, dict):
                row['caps_magic_meta'] = caps.get('magic')
                row['caps_version_meta'] = caps.get('version')
                row['caps_role_meta'] = caps.get('role')
                row['caps_features_meta'] = caps.get('features')
        else:
            problems.append('record.research2 is missing or not an object')

        stats = record.get('phase_stats')
        if isinstance(stats, list) and len(stats) == 3:
            for index, prefix in enumerate(('p0', 'p1', 'p2')):
                entry = stats[index]
                for field in PHASE_STATS_FIELDS:
                    row['%s_%s' % (prefix, field)] = (
                        entry.get(field) if isinstance(entry, dict) else None)
                row['%s_bytes' % prefix] = (
                    entry.get('completed_bytes') if isinstance(entry, dict) else None)
        else:
            problems.append('record.phase_stats is not a 3-entry list')

        words = build_raw_words(record, problems)
        if words is not None:
            for name, index in RAW_FIELDS:
                row[name] = words[index]

            check('raw-magic0', words[0x000] == RESEARCH_MAGIC0,
                  'raw[0x000]=%#x' % words[0x000])
            check('raw-observer-magic', words[0x001] == OBSERVER_MAGIC,
                  'raw[0x001]=%#x' % words[0x001])
            check('raw-mode-e2', words[0x004] == 1, 'raw[0x004]=%d' % words[0x004])
            check('raw-phase-freeze', words[0x006] == 4, 'raw[0x006]=%d' % words[0x006])
            check('raw-e2-ovf-zero', words[0x00C] == 0)
            check('raw-e2-err-zero', words[0x00D] == 0)
            check('raw-snapshot-valid-7', words[0x021] == 7)
            check('raw-target-valid-3', words[0x01C] == 3)
            check('raw-fp-done-3', words[0x01D] == 3)
            check('raw-caps-magic', words[CAP_BASE] == CAP_MAGIC,
                  'raw[0xF00]=%#x' % words[CAP_BASE])
            check('raw-caps-version', words[CAP_BASE + 1] == 1)
            check('raw-caps-role', words[CAP_BASE + 2] == case.get('role'))
            expected_features = CAP_FEATURES.get(case.get('role'))
            if expected_features is not None:
                check('raw-caps-features', words[CAP_BASE + 3] == expected_features)
            check('raw-e2t-magic', words[E2T_BASE] == E2T_MAGIC,
                  'raw[0xD80]=%#x' % words[E2T_BASE])
            check('raw-e2t-version', words[E2T_BASE + 1] == 1)
            check('raw-e2t-error-zero', words[E2T_BASE + 2] == 0)
            check('raw-e2t-flags-issue-select-done', words[E2T_BASE + 3] == 0b111)

            issue = words[E2T_BASE + 4]
            first_select = words[E2T_BASE + 5]
            done = words[E2T_BASE + 6]
            check('e2t-order', issue <= first_select <= done,
                  '%d/%d/%d' % (issue, first_select, done))
            check('e2t-done-issue', words[E2T_BASE + 7] == done - issue,
                  'raw[0xD87]=%d done-issue=%d' % (words[E2T_BASE + 7], done - issue))
            check('e2t-select-issue', words[E2T_BASE + 8] == first_select - issue,
                  'raw[0xD88]=%d select-issue=%d'
                  % (words[E2T_BASE + 8], first_select - issue))
            check('e2t-select-hit-miss',
                  words[E2T_BASE + 0x0B] == words[E2T_BASE + 0x0C] + words[E2T_BASE + 0x0D],
                  'select=%d hit=%d miss=%d'
                  % (words[E2T_BASE + 0x0B], words[E2T_BASE + 0x0C], words[E2T_BASE + 0x0D]))
            check('e2t-reserved-zero',
                  words[E2T_BASE + 0x0E] == 0 and words[E2T_BASE + 0x0F] == 0)

            reverse_word = words[CAP_REVERSE_WORD]
            check('raw-reverse-reserved-bits', reverse_word & ~0x1 == 0,
                  'raw[0xF10]=%#x' % reverse_word)
            expected_reverse = 1 if case.get('reverse') else 0
            check('raw-reverse-readback', (reverse_word & 0x1) == expected_reverse,
                  'raw[0xF10] bit0=%d case.reverse=%r'
                  % (reverse_word & 0x1, case.get('reverse')))

            if isinstance(inputs, dict):
                check('input-a-pages', inputs.get('a_pages') == case.get('a_pages'))
                check('input-a-transfers',
                      inputs.get('a_transfers') == case.get('a_transfers'))
                base_block = case.get('base') or {}
                for field in ('window_cycles', 'warmup', 'transfers',
                              'gap_cycles', 'timeout_ms'):
                    check('input-%s' % field,
                          inputs.get(field) == base_block.get(field),
                          '%r vs %r' % (inputs.get(field), base_block.get(field)))
                check('input-raw-words', inputs.get('raw_words') == RAW_WORDS)

            if isinstance(inputs, dict):
                expected_a = case.get('a_transfers')
                check('raw-a-issue-count', words[0x025] == expected_a,
                      '%r vs %r' % (words[0x025], expected_a))
                check('raw-a-done-count', words[0x026] == expected_a,
                      '%r vs %r' % (words[0x026], expected_a))
                window = (case.get('base') or {}).get('window_cycles')
                if window is not None:
                    check('raw-a-elapsed', words[0x008] == window,
                          '%r vs %r' % (words[0x008], window))
                    check('raw-deadline', words[0x01E] == words[0x007] + window)

            if legacy_module is not None:
                legacy_problems = []
                legacy_unknowns = []
                try:
                    hexes = ['0x%016x' % value for value in words]
                    raw_obj = legacy_module.parse_raw_words(hexes)
                    mapping_values = None
                    if isinstance(mapping, list) and len(mapping) == 4:
                        mapping_values = [
                            legacy_module.parse_hex64_field(item, 'mapping[%d]' % i)
                            for i, item in enumerate(mapping)]
                    evidence = legacy_module.analyze_e2(
                        raw_obj, _legacy_view(case), mapping_values,
                        legacy_problems, legacy_unknowns)
                    row['legacy_lifecycle'] = {
                        'targets': [
                            {
                                'role': target.get('role'),
                                'classification': target.get('classification'),
                                'cause': target.get('cause'),
                                'resident_warm': target.get('resident_warm'),
                                'resident_deadline': target.get('resident_deadline'),
                                'resident_freeze': target.get('resident_freeze'),
                                'first_probe_hit': (target.get('first_probe') or {}).get('hit'),
                                'A_removal_events': target.get('A_removal_events'),
                            }
                            for target in evidence.get('targets', [])
                        ],
                        'probe_order': evidence.get('probe_order'),
                        'post_warm_events': evidence.get('post_warm_events'),
                        'warm_phase_events': evidence.get('warm_phase_events'),
                        'no_a_anomaly': evidence.get('no_a_anomaly'),
                        'problems': legacy_problems,
                        'unknowns': legacy_unknowns,
                    }
                    for item in legacy_problems:
                        problems.append('legacy: %s' % item)
                except Exception as exc:
                    row['legacy_lifecycle'] = {'error': str(exc)}

    if metadata is None:
        problems.append('runs/%s/metadata.json is %s' % (case_id, metadata_state))
    else:
        row['meta_schema'] = metadata.get('schema')
        row['meta_exit'] = metadata.get('exit')
        row['meta_exit_code'] = metadata.get('exit_code')
        row['meta_framed_exit'] = metadata.get('framed_exit')
        row['meta_taint_before'] = metadata.get('taint_before')
        row['meta_taint_after'] = metadata.get('taint_after')
        row['meta_recovery'] = metadata.get('recovery')
        row['meta_boot_id'] = metadata.get('boot_id')
        row['meta_kernel_release'] = metadata.get('kernel_release')
        row['meta_finished_at'] = metadata.get('finished_at')
        row['meta_bit'] = metadata.get('bit')
        row['meta_bit_sha256'] = metadata.get('bit_sha256')
        row['meta_board_cli'] = metadata.get('board_cli')
        row['meta_board_cli_sha256'] = metadata.get('board_cli_sha256')
        row['meta_record_sha256'] = metadata.get('record_sha256')
        row['meta_archive_sha256'] = metadata.get('archive_sha256')

        check('meta-schema', metadata.get('schema') == METADATA_SCHEMA)
        check('meta-exit-zero', metadata.get('exit') == 0,
              'exit=%r' % (metadata.get('exit'),))
        check('meta-framed-exit-zero', metadata.get('framed_exit') == 0)
        check('meta-taint-before-zero',
              str(metadata.get('taint_before', '')).strip() == '0')
        check('meta-taint-after-zero',
              str(metadata.get('taint_after', '')).strip() == '0')
        check('meta-argv-vs-plan',
              _arg(metadata.get('argv')) == _arg(case.get('argv')),
              '%r vs %r' % (_arg(metadata.get('argv')), _arg(case.get('argv'))))
        declared = metadata.get('record_sha256')
        actual = row.get('record_sha256_actual')
        if declared is not None and actual is not None:
            check('meta-record-sha256', declared == actual,
                  '%s vs %s' % (declared, actual))
        check('meta-role', metadata.get('role') == case.get('role'))

    archive = run_dir / 'archive.tar.gz'
    row['archive_present'] = archive.is_file()
    if hash_archives and archive.is_file():
        try:
            row['archive_sha256_actual'] = _sha256_file(archive)
        except OSError as exc:
            problems.append('archive.tar.gz could not be hashed: %s' % exc)
    if archive.is_file() and row.get('meta_archive_sha256') and \
            row.get('archive_sha256_actual'):
        if row['meta_archive_sha256'] != row['archive_sha256_actual']:
            problems.append(
                'metadata archive_sha256 %s does not match the hashed archive %s'
                % (row['meta_archive_sha256'], row['archive_sha256_actual']))

    side_files = []
    if run_dir.is_dir():
        for item in sorted(run_dir.iterdir()):
            if not item.is_file() or item.name in ('record.json', 'metadata.json'):
                continue
            info = {'name': item.name}
            try:
                info['bytes'] = item.stat().st_size
            except OSError:
                info['bytes'] = None
            if item.name == 'archive.tar.gz':
                if hash_archives:
                    try:
                        info['sha256'] = _sha256_file(item)
                    except OSError:
                        info['sha256'] = None
            else:
                try:
                    info['sha256'] = _sha256_file(item)
                except OSError:
                    info['sha256'] = None
                if preview_bytes and item.suffix == '.txt':
                    try:
                        info['preview'] = item.read_text(
                            encoding='utf-8', errors='replace')[:preview_bytes]
                    except OSError:
                        pass
            side_files.append(info)
    row['side_files'] = side_files

    failed_ids = sorted(name for name, ok in checks.items() if not ok)
    row['checks_failed_count'] = len(failed_ids)
    row['checks_failed_ids'] = failed_ids
    row['problems'] = problems
    return row


def _cell(value):
    if value is None:
        return ''
    if isinstance(value, bool):
        return 'true' if value else 'false'
    if isinstance(value, float):
        return repr(value)
    if isinstance(value, (list, tuple)):
        return ';'.join(_cell(item) for item in value)
    if isinstance(value, dict):
        return json.dumps(value, ensure_ascii=False, sort_keys=True)
    text = str(value)
    return text.replace(TAB, ' ').replace(NEWLINE, ' ').replace(CARRIAGE, ' ')


def _describe(values):
    numbers = [value for value in values
               if isinstance(value, (int, float)) and not isinstance(value, bool)]
    if not numbers:
        return {'count': 0, 'min': None, 'median': None, 'max': None}
    return {
        'count': len(numbers),
        'min': min(numbers),
        'median': statistics.median(numbers),
        'max': max(numbers),
    }


def build_summary(rows, plan_doc, base_dir, hash_archives, legacy_path):
    groups = {}
    for row in rows:
        key = '%s/%s' % (row.get('phase'), row.get('condition'))
        groups.setdefault(key, []).append(row)

    grouped = {}
    for key in sorted(groups):
        members = groups[key]
        metrics = {}
        for name in SUMMARY_METRICS:
            metrics[name] = _describe([item.get(name) for item in members])
        grouped[key] = {
            'runs': len(members),
            'ids': [item.get('id') for item in members],
            'a_pages': sorted({item.get('a_pages') for item in members
                               if item.get('a_pages') is not None}),
            'a_transfers': sorted({item.get('a_transfers') for item in members
                                   if item.get('a_transfers') is not None}),
            'reverse': sorted({item.get('reverse') for item in members
                               if item.get('reverse') is not None}),
            'metrics': metrics,
            'v_probe_regime': sorted({
                'hit=%s,miss=%s' % (item.get('d8c_v_hit'), item.get('d8d_v_miss'))
                for item in members if item.get('d8b_v_select') is not None}),
        }

    def distinct(field):
        return sorted({str(item.get(field)) for item in rows
                       if item.get(field) is not None})

    archive_hashes = sorted({str(item.get('meta_archive_sha256')) for item in rows
                             if item.get('meta_archive_sha256')})
    bad = [item.get('id') for item in rows if item.get('checks_failed_count')]
    missing = [item.get('id') for item in rows if item.get('record_state') != 'ok']
    recovery = [item.get('id') for item in rows if item.get('meta_recovery')]

    return {
        'schema': 'e2-audit-aggregate-v1',
        'generated_by': 'scripts/e2_audit_aggregate.py',
        'input_dir': str(base_dir),
        'plan_schema': (plan_doc or {}).get('schema'),
        'plan_stage': (plan_doc or {}).get('stage'),
        'plan_count': (plan_doc or {}).get('count'),
        'plan_counts': (plan_doc or {}).get('counts'),
        'cases_read': len(rows),
        'cases_clean': len(rows) - len(bad),
        'cases_with_failed_checks': bad,
        'cases_with_unreadable_record': missing,
        'cases_with_recovery_field': recovery,
        'hash_archives': bool(hash_archives),
        'legacy_analyzer': legacy_path,
        'board_evidence': {
            'boot_id': distinct('meta_boot_id'),
            'kernel_release': distinct('meta_kernel_release'),
            'bit_sha256': distinct('meta_bit_sha256'),
            'board_cli': distinct('meta_board_cli'),
            'board_cli_sha256': distinct('meta_board_cli_sha256'),
            'exit_values': distinct('meta_exit'),
            'framed_exit_values': distinct('meta_framed_exit'),
            'taint_before_values': distinct('meta_taint_before'),
            'taint_after_values': distinct('meta_taint_after'),
            'archive_sha256_count': len(archive_hashes),
            'archive_sha256': archive_hashes,
        },
        'fixed_checks': {
            'note': 'each run independently re-checks the research magic, observer '
                    'magic, E2 mode/FREEZE phase, zero error/overflow counters, '
                    'snapshot_valid/target_valid/fp_done, IDMA2CAP F00..F03, the '
                    'IDMAE2T1 D80..D8F block and its d87/d88/d8b identities, the '
                    'F10 reverse readback, the frozen input envelope, metadata '
                    'exit/taint/argv and the declared record sha256',
            'always_applied': True,
        },
        'limits': {
            'no_board_access': True,
            'no_retry': True,
            'no_case_dropped': True,
            'no_input_mutation': True,
            'cli_booleans_trusted': False,
            'statistics': 'descriptive count/min/median/max only; short cases are '
                          'single runs and boundary has 5 runs per condition, so '
                          'no significance, equivalence or novelty claim follows',
        },
        'groups': grouped,
    }


def write_outputs(output_dir, rows, summary):
    output_dir.mkdir(parents=True)
    columns = list(TSV_COLUMNS)
    for row in rows:
        for key in row:
            if key not in columns:
                columns.append(key)
    with open(output_dir / 'per_run.tsv', 'w', encoding='utf-8') as handle:
        handle.write(TAB.join(columns) + NEWLINE)
        for row in rows:
            handle.write(TAB.join(_cell(row.get(name)) for name in columns) + NEWLINE)
    text = json.dumps(summary, indent=2, ensure_ascii=False) + NEWLINE
    (output_dir / 'summary.json').write_text(text, encoding='utf-8')


def audit(input_dir, output_dir, hash_archives=False, legacy_path=None,
          preview_bytes=400):
    base = Path(input_dir)
    if not base.is_dir():
        raise UsageError('input directory is not a directory: %s' % input_dir)
    out = Path(output_dir)
    if out.exists():
        raise UsageError('output directory already exists: %s' % output_dir)
    if out.resolve() == base.resolve() or base.resolve() in out.resolve().parents:
        raise UsageError('the output directory must not be the input directory or inside it')

    plan_doc, plan_state = _load_json_state(base / 'plan.json')
    if plan_doc is None:
        raise UsageError('plan.json is %s in %s' % (plan_state, input_dir))
    cases = plan_cases(plan_doc)
    if not cases:
        raise UsageError('plan.json contains no cases')

    legacy_module = _load_legacy(legacy_path)
    if legacy_path and legacy_module is None:
        print('e2_audit_aggregate: --legacy-analyzer %s could not be imported; '
              'continuing without it' % legacy_path, file=sys.stderr)

    rows = [audit_case(base, case, hash_archives, legacy_module, preview_bytes)
            for case in cases]
    summary = build_summary(rows, plan_doc, base, hash_archives,
                            legacy_path if legacy_module is not None else None)
    write_outputs(out, rows, summary)
    return summary


def main(argv=None):
    parser = argparse.ArgumentParser(
        prog='e2_audit_aggregate.py',
        description='Read-only audit aggregator for research2 E2 run directories.',
    )
    parser.add_argument('input_dir', help='finished E2 directory (read-only)')
    parser.add_argument('--output', required=True, metavar='NEWDIR',
                        help='new directory for per_run.tsv and summary.json')
    parser.add_argument('--hash-archives', action='store_true',
                        help='also hash runs/<id>/archive.tar.gz (slow)')
    parser.add_argument('--legacy-analyzer', default=None, metavar='PATH',
                        help='optional scripts/research_results.py for the E2 '
                             'lifecycle replay')
    parser.add_argument('--preview-bytes', type=int, default=400,
                        help='preview length kept for side *.txt files')
    args = parser.parse_args(argv)
    try:
        summary = audit(args.input_dir, args.output,
                        hash_archives=args.hash_archives,
                        legacy_path=args.legacy_analyzer,
                        preview_bytes=args.preview_bytes)
    except UsageError as exc:
        print('e2_audit_aggregate: %s' % exc, file=sys.stderr)
        return 2
    except OSError as exc:
        print('e2_audit_aggregate: output failed: %s' % exc, file=sys.stderr)
        return 4
    print('e2_audit_aggregate: cases=%d clean=%d out=%s'
          % (summary['cases_read'], summary['cases_clean'], args.output),
          file=sys.stderr)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
