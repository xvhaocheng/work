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
    runs/<case_id>/*               every side file directly under the run
                                   directory is listed and hashed
    runs/<case_id>/board/*         the board capture directory: record.json,
                                   exit.txt, taint.before/after, uname.before/
                                   after, argv.txt, stderr.txt, dmesg.before/
                                   after and the record.sha256 / archive.sha256
                                   notes are read, hashed and checked;
                                   board/record.json must be byte-identical to
                                   runs/<case_id>/record.json
    runs/<case_id>/archive.tar.gz  hashed with --hash-archives; its members are
                                   always compared, read-only through tarfile,
                                   against the local board files (nothing is ever
                                   extracted to disk)

Writes into NEW_DIR (which must not exist):
    per_run.tsv       one row per planned case, every extracted field
    summary.json      grouped count/min/median/max, fixed checks, board evidence
    board-audit.json  full per-case board capture and legacy replay evidence
                      (file hashes, sha256 notes, dmesg before/after added
                      lines, archive member comparison, legacy events)

Rules:
  * The record's own success/validated booleans are recorded, never trusted;
    every statement is re-derived from record fields and raw words.
  * No field is invented.  An absent key stays absent and is reported absent.
  * The sparse raw encoding omits zero words; an explicit zero in the sparse
    map is a malformed record, not a silent accept.
  * scripts/research_results.py is used only with --legacy-analyzer PATH.
    When the option is given the module must import and replay every case: a
    failed import, a raising replay or a legacy problem is a failure, never a
    silent pass.  Without the option no lifecycle evidence is claimed at all.
  * The tool never runs the board, never retries, and never drops a case.
  * Fail-closed: a missing board capture, an unreadable or mismatched sha256
    note, an archive or hash mismatch, a nonzero exit.txt or taint bit, an uname
    or argv that disagrees with the metadata, an archive member that differs
    from the local board file, and any recorded problem all make the case
    unclean, and one unclean case makes the tool exit 1.

Exit status:
    0  every planned case is present and clean and no global problem is open
    1  the outputs were written but at least one case failed, or a global
       problem (legacy analyzer import, plan, board evidence) is open
    2  usage error or unusable input directory
    4  the output directory could not be written
'''

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import re
import statistics
import sys
import tarfile
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

# Files every board capture directory (runs/<id>/board/) must carry.
BOARD_REQUIRED = (
    'record.json', 'exit.txt', 'taint.before', 'taint.after',
    'uname.before', 'uname.after', 'argv.txt', 'stderr.txt',
    'dmesg.before', 'dmesg.after',
)
# Per-run detail that belongs in board-audit.json, never in per_run.tsv.
DETAIL_KEYS = ('audit_detail',)
# The legacy event log tags ARM_A updates with phase 2.
LEGACY_A_PHASE = 2

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
    'fp0_time', 'fp0_live', 'fp0_hit', 'fp0_live_self', 'fp0_live_other',
    'fp1_time', 'fp1_live', 'fp1_hit', 'fp1_live_self', 'fp1_live_other',
    'a_src_vpn', 'a_dst_vpn',
    'visited_A_src_pages', 'visited_A_dst_pages', 'visited_A_other_pages',
    'visited_A_page_total',
    'classification_src', 'classification_dst',
    'first_probe_hit_src', 'first_probe_hit_dst',
    'A_removal_events_src', 'A_removal_events_dst',
    'meta_schema', 'meta_exit', 'meta_exit_code', 'meta_framed_exit',
    'meta_taint_before', 'meta_taint_after', 'meta_recovery',
    'meta_boot_id', 'meta_kernel_release', 'meta_finished_at',
    'meta_bit', 'meta_bit_sha256', 'meta_board_cli', 'meta_board_cli_sha256',
    'meta_record_sha256', 'record_sha256_actual',
    'meta_archive_sha256', 'archive_present', 'archive_sha256_actual',
    'board_present', 'board_files', 'board_record_sha256',
    'board_record_matches_root', 'board_exit', 'board_exit_ok',
    'board_taint_before', 'board_taint_after', 'board_taint_ok',
    'board_uname_before', 'board_uname_after',
    'board_uname_matches_metadata', 'board_argv',
    'board_argv_matches_plan', 'board_argv_matches_metadata',
    'board_stderr_present', 'board_dmesg_new_lines',
    'record_sha256_note', 'archive_sha256_note', 'archive_member_mismatches',
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


def _plan_case_base(case):
    '''The frozen plan's per-run workload block; an absent base is empty.'''
    base = case.get('base')
    return base if isinstance(base, dict) else {}


def _plan_field(case, name):
    '''Frozen plan value of *name*: case["base"][name] first, then top level.

    The real plan keeps the workload counters (notably a_transfers) only in
    case["base"], never at the case top level; a_pages happens to exist in both
    places.  Reading the base block first is the single row provenance.
    '''
    base = _plan_case_base(case)
    if name in base:
        return base[name]
    return case.get(name)


def _plan_a_transfers(case):
    '''The frozen A transfer count, which never lives at the case top level.'''
    return _plan_field(case, 'a_transfers')


def _read_text_state(path):
    if path is None or not path.is_file():
        return None, 'missing'
    try:
        return path.read_text(encoding='utf-8', errors='replace'), 'ok'
    except OSError:
        return None, 'unreadable'


def _find_capture_file(board_dir, run_dir, name):
    '''First existing capture file, board directory before run directory.'''
    for candidate in (board_dir / name, run_dir / name):
        if candidate.is_file():
            return candidate
    return None


def _first_sha256_token(text):
    '''First 64-hex token of a sha256sum-style note, lowercased, or None.'''
    if text is None:
        return None
    match = re.search('[0-9a-fA-F]{64}', text)
    return match.group(0).lower() if match else None


def _strip_lines(text):
    if text is None:
        return None
    return [line.rstrip('\r') for line in text.split('\n')]


def _new_lines(before, after):
    '''Multiset difference after - before: only genuinely new lines.

    A warning that is already present in dmesg.before is a historical line and
    is never reported as a new warning, so a repeated warning is not counted
    again.
    '''
    if after is None:
        return None
    remaining = {}
    for line in (before or []):
        remaining[line] = remaining.get(line, 0) + 1
    fresh = []
    for line in after:
        count = remaining.get(line, 0)
        if count:
            remaining[line] = count - 1
        else:
            fresh.append(line)
    return fresh


def _argv_json(text):
    '''Parse a captured argv.txt (a JSON list) into one space-joined string.'''
    if text is None:
        return None, 'missing'
    stripped = text.strip()
    if not stripped:
        return None, 'empty'
    try:
        value = json.loads(stripped)
    except ValueError:
        return stripped, 'raw'
    return _arg(value), 'json'


def _archive_members(path, problems):
    '''sha256 of every regular archive member, read through tarfile only.

    extractfile() streams a member into memory; nothing is ever extracted to
    disk.
    '''
    members = {}
    try:
        with tarfile.open(path, 'r:gz') as archive:
            for info in archive.getmembers():
                if not info.isfile():
                    continue
                handle = archive.extractfile(info)
                if handle is None:
                    members[info.name] = None
                    continue
                digest = hashlib.sha256()
                while True:
                    block = handle.read(1 << 20)
                    if not block:
                        break
                    digest.update(block)
                members[info.name] = digest.hexdigest()
    except (OSError, tarfile.TarError) as exc:
        problems.append('archive.tar.gz could not be read as a tar file: %s' % exc)
        return None
    return members


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
    base = _plan_case_base(case)
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


def _legacy_target_fields(lifecycle, row):
    '''Lift the per-role first-probe result into flat row columns.'''
    if not isinstance(lifecycle, dict):
        return
    targets = lifecycle.get('targets')
    if not isinstance(targets, list):
        return
    for target in targets:
        if not isinstance(target, dict) or target.get('role') not in ('src', 'dst'):
            continue
        role = target['role']
        hit = target.get('first_probe_hit')
        row['first_probe_hit_%s' % role] = None if hit is None else bool(hit)
        removals = target.get('A_removal_events')
        row['A_removal_events_%s' % role] = (
            len(removals) if isinstance(removals, list) else None)
        row['classification_%s' % role] = target.get('classification')


def _legacy_coverage(legacy_module, evidence, mapping, case, problems):
    '''A-context page coverage from the committed A-phase update records.

    The W/N coverage is the union of the *new* ordinary valid entries of the
    events the legacy analyzer classifies as ARM_A (phase 2).  The deadline
    snapshot is only the final residency of the A window and is deliberately
    not used as coverage.  mapping[0:2] describe the V targets, mapping[2:4]
    the A pages: mapping[2] is the A source page and mapping[3] the A
    destination page, so a new A entry is attributed by its vpn/pscid.
    '''
    out = {
        'a_src_vpn': None,
        'a_dst_vpn': None,
        'visited_A_src_pages': [],
        'visited_A_dst_pages': [],
        'visited_A_other_pages': [],
        'visited_A_page_total': 0,
        'visited_A_coverage_empty': None,
        'coverage_source': None,
    }

    def page_vpn(word):
        if isinstance(word, bool) or not isinstance(word, int):
            return None
        return (word >> 12) & ((1 << 29) - 1)

    if not isinstance(mapping, list) or len(mapping) != 4:
        problems.append('mapping is not a 4-word list: the W/N A-page coverage '
                        'cannot be attributed to src/dst')
        return out
    src_vpn = page_vpn(mapping[2])
    dst_vpn = page_vpn(mapping[3])
    out['a_src_vpn'] = src_vpn
    out['a_dst_vpn'] = dst_vpn
    if src_vpn is None or dst_vpn is None:
        problems.append('mapping[2]/mapping[3] are not integers: the W/N '
                        'A-page coverage cannot be attributed to src/dst')
        return out

    events = evidence.get('events') if isinstance(evidence, dict) else None
    if not isinstance(events, list):
        problems.append('legacy evidence carries no event list: the W/N '
                        'A-page coverage is unknown')
        return out
    out['coverage_source'] = (
        'legacy evidence.events, phase==ARM_A (2), union of new ordinary valid '
        'entries; the deadline snapshot is only the final residency and is not '
        'coverage')
    seen = set()
    for event in events:
        if not isinstance(event, dict) or event.get('phase') != LEGACY_A_PHASE:
            continue
        new_hex = event.get('new')
        if not isinstance(new_hex, list) or len(new_hex) != 2:
            continue
        lo = _hex64_to_int(new_hex[0])
        hi = _hex64_to_int(new_hex[1])
        if lo is None or hi is None:
            continue
        if not legacy_module.entry_is_ordinary(lo, hi):
            continue
        identity = event.get('new_identity') or {}
        vpn = identity.get('vpn')
        pscid = identity.get('pscid')
        item = {
            'event': event.get('index'),
            'ts': event.get('ts'),
            'did_name': event.get('did_name'),
            'vpn': vpn,
            'pscid': pscid,
            'gscid': identity.get('gscid'),
        }
        key = (vpn, pscid)
        if key in seen:
            continue
        seen.add(key)
        width = _plan_field(case, 'a_pages')
        if src_vpn <= vpn < src_vpn + width:
            out['visited_A_src_pages'].append(item)
        elif dst_vpn <= vpn < dst_vpn + width:
            out['visited_A_dst_pages'].append(item)
        else:
            out['visited_A_other_pages'].append(item)
    out['visited_A_page_total'] = len(seen)
    out['visited_A_coverage_empty'] = (
        bool(_plan_field(case, 'a_pages')) and not seen)
    return out


def audit_board(run_dir, case, row, record_root_sha, metadata, problems,
                check, hash_archives):
    '''Read-only board capture checks for one case.

    Everything is read from runs/<case_id>/board (falling back to the run
    directory for the sha256 notes and the archive) and nothing is written or
    extracted anywhere inside the input tree.  Errors go into *problems* and
    failed comparisons into *checks*, so the caller's fail-closed aggregation
    sees them.
    '''
    case_id = row['id']
    board_dir = run_dir / 'board'
    board = {
        'id': case_id,
        'dir': str(board_dir),
        'present': board_dir.is_dir(),
    }
    row['board_present'] = board['present']
    check('board-dir-present', board['present'],
          'runs/%s/board is missing' % case_id)
    if not board['present']:
        return board

    files = {}
    file_list = []
    for item in sorted(board_dir.iterdir()):
        if not item.is_file():
            continue
        info = {'name': item.name}
        try:
            info['bytes'] = item.stat().st_size
        except OSError:
            info['bytes'] = None
        try:
            info['sha256'] = _sha256_file(item)
        except OSError as exc:
            info['sha256'] = None
            problems.append('board/%s could not be hashed: %s'
                            % (item.name, exc))
        files[item.name] = info['sha256']
        file_list.append(info)
    board['files'] = file_list
    row['board_files'] = [entry['name'] for entry in file_list]

    for name in BOARD_REQUIRED:
        if name not in files:
            problems.append('runs/%s/board/%s is missing' % (case_id, name))

    board_record = files.get('record.json')
    row['board_record_sha256'] = board_record
    if board_record is not None and record_root_sha is not None:
        same = board_record == record_root_sha
        row['board_record_matches_root'] = same
        check('board-record-matches-root', same,
              '%s vs %s' % (board_record, record_root_sha))
    else:
        row['board_record_matches_root'] = None
        check('board-record-hashable', False,
              'runs/%s/record.json or runs/%s/board/record.json is not '
              'hashable' % (case_id, case_id))

    exit_text, exit_state = _read_text_state(board_dir / 'exit.txt')
    board['exit_text'] = exit_text
    row['board_exit'] = None if exit_text is None else exit_text.strip()
    exit_ok = (
        exit_state == 'ok'
        and row['board_exit'].lstrip('-').isdigit()
        and int(row['board_exit']) == 0
    )
    row['board_exit_ok'] = exit_ok
    check('board-exit-zero', exit_ok,
          'exit.txt=%r (%s)' % (row['board_exit'], exit_state))

    before_text, before_state = _read_text_state(board_dir / 'taint.before')
    after_text, after_state = _read_text_state(board_dir / 'taint.after')
    board['taint_before_text'] = before_text
    board['taint_after_text'] = after_text
    row['board_taint_before'] = (
        None if before_text is None else before_text.strip())
    row['board_taint_after'] = (
        None if after_text is None else after_text.strip())
    taint_ok = (
        before_state == 'ok' and after_state == 'ok'
        and row['board_taint_before'] == '0'
        and row['board_taint_after'] == '0'
    )
    row['board_taint_ok'] = taint_ok
    check('board-taint-zero', taint_ok,
          'before=%r (%s) after=%r (%s)'
          % (row['board_taint_before'], before_state,
             row['board_taint_after'], after_state))

    uname_before, uname_before_state = _read_text_state(
        board_dir / 'uname.before')
    uname_after, uname_after_state = _read_text_state(board_dir / 'uname.after')
    board['uname_before'] = uname_before
    board['uname_after'] = uname_after
    row['board_uname_before'] = (
        None if uname_before is None else uname_before.strip())
    row['board_uname_after'] = (
        None if uname_after is None else uname_after.strip())
    release = metadata.get('kernel_release') if isinstance(metadata, dict) else None
    if not isinstance(release, str) or not release.strip():
        problems.append('metadata kernel_release is missing, so the uname '
                        'before/after comparison is impossible')
    uname_ok = (
        uname_before_state == 'ok' and uname_after_state == 'ok'
        and bool(row['board_uname_before'])
        and row['board_uname_before'] == row['board_uname_after']
        and isinstance(release, str) and release.strip() in uname_before
    )
    row['board_uname_matches_metadata'] = uname_ok
    check('board-uname-matches-metadata', uname_ok,
          'before=%r after=%r kernel_release=%r'
          % (row['board_uname_before'], row['board_uname_after'], release))

    argv_text, argv_state = _read_text_state(board_dir / 'argv.txt')
    board['argv_text'] = argv_text
    captured_argv, argv_kind = _argv_json(argv_text)
    board['argv_kind'] = argv_kind
    plan_argv = _arg(case.get('argv'))
    meta_argv = _arg(metadata.get('argv')) if isinstance(metadata, dict) else None
    row['board_argv'] = captured_argv
    row['board_argv_matches_plan'] = (
        None if captured_argv is None else captured_argv == plan_argv)
    row['board_argv_matches_metadata'] = (
        None if captured_argv is None else captured_argv == meta_argv)
    check('board-argv-matches-plan',
          captured_argv is not None and captured_argv == plan_argv,
          '%r (%s) vs %r' % (captured_argv, argv_kind, plan_argv))
    check('board-argv-matches-metadata',
          captured_argv is not None and captured_argv == meta_argv,
          '%r vs %r' % (captured_argv, meta_argv))

    stderr_text, stderr_state = _read_text_state(board_dir / 'stderr.txt')
    board['stderr_text'] = stderr_text
    row['board_stderr_present'] = stderr_state == 'ok'
    check('board-stderr-present', stderr_state == 'ok',
          'stderr.txt is %s' % stderr_state)

    before_dmesg, before_dmesg_state = _read_text_state(
        board_dir / 'dmesg.before')
    after_dmesg, after_dmesg_state = _read_text_state(board_dir / 'dmesg.after')
    board['dmesg_before_state'] = before_dmesg_state
    board['dmesg_after_state'] = after_dmesg_state
    board['dmesg_new_lines'] = _new_lines(_strip_lines(before_dmesg),
                                          _strip_lines(after_dmesg))
    row['board_dmesg_new_lines'] = (
        None if board['dmesg_new_lines'] is None
        else len(board['dmesg_new_lines']))
    check('board-dmesg-present',
          before_dmesg_state == 'ok' and after_dmesg_state == 'ok',
          'before=%s after=%s' % (before_dmesg_state, after_dmesg_state))

    record_note_path = _find_capture_file(board_dir, run_dir, 'record.sha256')
    archive_note_path = _find_capture_file(board_dir, run_dir, 'archive.sha256')
    record_note_text, record_note_state = _read_text_state(record_note_path)
    archive_note_text, archive_note_state = _read_text_state(archive_note_path)
    board['record_sha256_note'] = record_note_text
    board['archive_sha256_note'] = archive_note_text
    record_note = _first_sha256_token(record_note_text)
    archive_note = _first_sha256_token(archive_note_text)
    row['record_sha256_note'] = record_note
    row['archive_sha256_note'] = archive_note
    check('board-record-sha256-note', record_note is not None,
          'record.sha256 is %s' % record_note_state)
    check('board-archive-sha256-note', archive_note is not None,
          'archive.sha256 is %s' % archive_note_state)
    declared_record = (
        metadata.get('record_sha256') if isinstance(metadata, dict) else None)
    declared_archive = (
        metadata.get('archive_sha256') if isinstance(metadata, dict) else None)
    if record_note is not None:
        check('board-record-sha256-note-vs-root',
              record_root_sha is not None and record_note == record_root_sha,
              '%s vs %s' % (record_note, record_root_sha))
        check('board-record-sha256-note-vs-metadata',
              declared_record is not None and record_note == declared_record,
              '%s vs %s' % (record_note, declared_record))
    if archive_note is not None:
        check('board-archive-sha256-note-vs-metadata',
              declared_archive is not None and archive_note == declared_archive,
              '%s vs %s' % (archive_note, declared_archive))

    archive_path = _find_capture_file(board_dir, run_dir, 'archive.tar.gz')
    board['archive'] = None if archive_path is None else str(archive_path)
    row['archive_present'] = archive_path is not None
    check('archive-present', archive_path is not None,
          'archive.tar.gz is missing from runs/%s and runs/%s/board'
          % (case_id, case_id))
    if archive_path is not None:
        if hash_archives:
            try:
                row['archive_sha256_actual'] = _sha256_file(archive_path)
            except OSError as exc:
                problems.append('archive.tar.gz could not be hashed: %s' % exc)
        actual_archive = row.get('archive_sha256_actual')
        if actual_archive is not None and declared_archive is not None:
            check('meta-archive-sha256', declared_archive == actual_archive,
                  '%s vs %s' % (declared_archive, actual_archive))
        if actual_archive is not None and archive_note is not None:
            check('board-archive-sha256-note-vs-actual',
                  archive_note == actual_archive,
                  '%s vs %s' % (archive_note, actual_archive))
        members = _archive_members(archive_path, problems)
        board['archive_members'] = members
        mismatches = []
        if members is not None:
            for name in sorted(members):
                base_name = name.rsplit('/', 1)[-1]
                local = files.get(base_name)
                if local is None:
                    mismatches.append(
                        '%s: archive member has no local board file' % name)
                elif members[name] != local:
                    mismatches.append('%s: archive %s vs board %s'
                                      % (name, members[name], local))
            for name in sorted(files):
                if name.endswith('.sha256'):
                    continue
                if not any(member.rsplit('/', 1)[-1] == name
                           for member in members):
                    mismatches.append(
                        '%s: local board file is not an archive member' % name)
        row['archive_member_mismatches'] = mismatches
        for item in mismatches:
            problems.append('archive member mismatch: %s' % item)
    return board


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
        'a_pages': _plan_field(case, 'a_pages'),
        'a_transfers': _plan_a_transfers(case),
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
            for target, time_index, live_index in ((0, 0x014, 0x015), (1, 0x018, 0x019)):
                live = words[live_index]
                row['fp%d_time' % target] = words[time_index]
                row['fp%d_live' % target] = live
                row['fp%d_hit' % target] = bool((live >> 32) & 1)
                row['fp%d_live_self' % target] = live & 0xffff
                row['fp%d_live_other' % target] = (live >> 16) & 0xffff

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
                check('input-a-pages',
                      inputs.get('a_pages') == _plan_field(case, 'a_pages'),
                      '%r vs %r' % (inputs.get('a_pages'),
                                    _plan_field(case, 'a_pages')))
                check('input-a-transfers',
                      inputs.get('a_transfers') == _plan_a_transfers(case),
                      '%r vs %r' % (inputs.get('a_transfers'),
                                    _plan_a_transfers(case)))
                for field in ('window_cycles', 'warmup', 'transfers',
                              'gap_cycles', 'timeout_ms'):
                    check('input-%s' % field,
                          inputs.get(field) == _plan_field(case, field),
                          '%r vs %r' % (inputs.get(field),
                                        _plan_field(case, field)))
                check('input-raw-words', inputs.get('raw_words') == RAW_WORDS)

            if isinstance(inputs, dict):
                expected_a = _plan_a_transfers(case)
                check('raw-a-issue-count', words[0x025] == expected_a,
                      '%r vs %r' % (words[0x025], expected_a))
                check('raw-a-done-count', words[0x026] == expected_a,
                      '%r vs %r' % (words[0x026], expected_a))
                window = _plan_field(case, 'window_cycles')
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
                    _legacy_target_fields(row['legacy_lifecycle'], row)
                    row.update(_legacy_coverage(legacy_module, evidence,
                                                mapping_values, case, problems))
                    width = _plan_field(case, 'a_pages')
                    visited = min(width, _plan_a_transfers(case))
                    offsets = set(range(width - visited, width) if case.get('reverse')
                                  else range(visited))
                    for role in ('src', 'dst'):
                        observed = {entry['vpn'] - row['a_%s_vpn' % role]
                                    for entry in row['visited_A_%s_pages' % role]}
                        check('A-%s-page-coverage' % role, observed == offsets,
                              '%s vs %s' % (sorted(observed), sorted(offsets)))
                    check('A-no-other-page-coverage', not row['visited_A_other_pages'])
                    row.setdefault('audit_detail', {})['legacy_evidence'] = evidence
                except Exception as exc:
                    row['legacy_lifecycle'] = {
                        'error': str(exc),
                        'error_type': type(exc).__name__,
                    }
                    problems.append(
                        'legacy lifecycle replay failed: %s: %s'
                        % (type(exc).__name__, exc))

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
            if item.name == 'archive.tar.gz' and not hash_archives:
                info['sha256'] = None
            else:
                try:
                    info['sha256'] = _sha256_file(item)
                except OSError:
                    info['sha256'] = None
            if preview_bytes and item.suffix in ('.txt', '.sha256'):
                try:
                    info['preview'] = item.read_text(
                        encoding='utf-8', errors='replace')[:preview_bytes]
                except OSError:
                    pass
            side_files.append(info)
    row['side_files'] = side_files

    # The board capture lives in runs/<id>/board; the run directory only keeps
    # side files, so the nested capture is audited separately and in full.
    audit_detail = row.setdefault('audit_detail', {})
    audit_detail['board'] = audit_board(
        run_dir, case, row, row.get('record_sha256_actual'), metadata,
        problems, check, hash_archives)

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


def build_summary(rows, plan_doc, base_dir, hash_archives, legacy_path,
                  global_problems=None, legacy_error=None):
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
        classifications = {}
        first_probe_hits = {'src': [0, 0], 'dst': [0, 0]}
        for item in members:
            lifecycle = item.get('legacy_lifecycle')
            targets = (lifecycle.get('targets')
                       if isinstance(lifecycle, dict) else None)
            if not isinstance(targets, list):
                continue
            for target in targets:
                if not isinstance(target, dict):
                    continue
                name = target.get('classification')
                classifications[name] = classifications.get(name, 0) + 1
                role = target.get('role')
                if role in first_probe_hits:
                    hit = target.get('first_probe_hit')
                    if hit is not None:
                        first_probe_hits[role][1] += 1
                        if hit:
                            first_probe_hits[role][0] += 1
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
            'classification_counts': classifications,
            'first_probe_hit': {
                'src': {'hits': first_probe_hits['src'][0],
                        'targets': first_probe_hits['src'][1]},
                'dst': {'hits': first_probe_hits['dst'][0],
                        'targets': first_probe_hits['dst'][1]},
            },
            'A_removal_events': {
                'src': _describe([item.get('A_removal_events_src')
                                  for item in members]),
                'dst': _describe([item.get('A_removal_events_dst')
                                  for item in members]),
            },
            'visited_A_pages': {
                'src': sum(len(item.get('visited_A_src_pages') or [])
                           for item in members),
                'dst': sum(len(item.get('visited_A_dst_pages') or [])
                           for item in members),
                'other': sum(len(item.get('visited_A_other_pages') or [])
                             for item in members),
                'source': 'legacy A-phase (phase 2) new ordinary entry union; '
                          'the deadline snapshot is not coverage',
            },
        }

    def distinct(field):
        return sorted({str(item.get(field)) for item in rows
                       if item.get(field) is not None})

    def present(field):
        return [item.get('id') for item in rows if item.get(field)]

    archive_hashes = sorted({str(item.get('meta_archive_sha256')) for item in rows
                             if item.get('meta_archive_sha256')})
    bad = [item.get('id') for item in rows
           if item.get('checks_failed_count') or item.get('problems')]
    missing = [item.get('id') for item in rows if item.get('record_state') != 'ok']
    recovery = [item.get('id') for item in rows if item.get('meta_recovery')]
    new_dmesg = {
        item.get('id'): len(item['audit_detail']['board'].get('dmesg_new_lines') or [])
        for item in rows
        if isinstance(item.get('audit_detail'), dict)
        and item['audit_detail'].get('board', {}).get('dmesg_new_lines')
    }
    empty_coverage = [
        item.get('id') for item in rows
        if item.get('a_pages')
        and isinstance(item.get('legacy_lifecycle'), dict)
        and not item['legacy_lifecycle'].get('error')
        and not item.get('visited_A_page_total')]

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
        'cases_with_problems_or_failed_checks': bad,
        'cases_with_unreadable_record': missing,
        'cases_with_recovery_field': recovery,
        'cases_with_new_dmesg_lines': sorted(new_dmesg),
        'new_dmesg_line_counts': new_dmesg,
        'cases_with_empty_A_coverage': empty_coverage,
        'hash_archives': bool(hash_archives),
        'legacy_analyzer': legacy_path,
        'legacy_analyzer_error': legacy_error,
        'global_problems': list(global_problems or []),
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
            'board_dir_cases': len(present('board_present')),
            'board_record_matches_root_cases': len(
                present('board_record_matches_root')),
            'board_exit_ok_cases': len(present('board_exit_ok')),
            'board_taint_ok_cases': len(present('board_taint_ok')),
            'board_uname_matched_cases': len(
                present('board_uname_matches_metadata')),
            'board_argv_plan_matched_cases': len(
                present('board_argv_matches_plan')),
            'board_argv_metadata_matched_cases': len(
                present('board_argv_matches_metadata')),
            'archive_present_cases': len(present('archive_present')),
            'archive_member_mismatch_cases': present('archive_member_mismatches'),
            'warnings_note': 'dmesg.before/after added lines are listed per case '
                             'as warnings in board-audit.json; a zero taint bit is '
                             'not a statement that no warning exists',
        },
        'fixed_checks': {
            'note': 'each run independently re-checks the research magic, observer '
                    'magic, E2 mode/FREEZE phase, zero error/overflow counters, '
                    'snapshot_valid/target_valid/fp_done, IDMA2CAP F00..F03, the '
                    'IDMAE2T1 D80..D8F block and its d87/d88/d8b identities, the '
                    'F10 reverse readback, the frozen input envelope, metadata '
                    'exit/taint/argv and the declared record sha256; plus the '
                    'board capture (board/record.json == record.json, exit.txt, '
                    'taint.before/after, uname before/after vs kernel_release, '
                    'argv.txt vs plan and metadata, stderr, dmesg before/after '
                    'added lines, record.sha256/archive.sha256 first token, and '
                    'the archive members against the local board files)',
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
            if key in DETAIL_KEYS or key in columns:
                continue
            columns.append(key)
    with open(output_dir / 'per_run.tsv', 'w', encoding='utf-8') as handle:
        handle.write(TAB.join(columns) + NEWLINE)
        for row in rows:
            handle.write(TAB.join(_cell(row.get(name)) for name in columns) + NEWLINE)
    text = json.dumps(summary, indent=2, ensure_ascii=False) + NEWLINE
    (output_dir / 'summary.json').write_text(text, encoding='utf-8')
    detail = {
        'schema': 'e2-audit-board-v1',
        'generated_by': 'scripts/e2_audit_aggregate.py',
        'input_dir': summary.get('input_dir'),
        'note': 'full read-only board capture and legacy replay evidence per case; '
                'dmesg_new_lines are the lines the after capture adds to the '
                'before capture, so a repeated historical warning is not a new '
                'warning',
        'cases': [row.get('audit_detail') for row in rows],
    }
    (output_dir / 'board-audit.json').write_text(
        json.dumps(detail, indent=2, ensure_ascii=False, default=str) + NEWLINE,
        encoding='utf-8')


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

    global_problems = []
    legacy_error = None
    legacy_module = _load_legacy(legacy_path)
    if legacy_path and legacy_module is None:
        legacy_error = ('--legacy-analyzer %s could not be imported: the E2 '
                        'lifecycle replay and the W/N A-page coverage are '
                        'missing for every case' % legacy_path)
        global_problems.append(legacy_error)
        print('e2_audit_aggregate: ' + legacy_error, file=sys.stderr)

    rows = [audit_case(base, case, hash_archives, legacy_module, preview_bytes)
            for case in cases]
    summary = build_summary(rows, plan_doc, base, hash_archives,
                            legacy_path if legacy_module is not None else None,
                            global_problems=global_problems,
                            legacy_error=legacy_error)
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
    clean_run = (
        summary['cases_clean'] == summary['cases_read']
        and not summary.get('global_problems')
    )
    return 0 if clean_run else 1


if __name__ == '__main__':
    raise SystemExit(main())
