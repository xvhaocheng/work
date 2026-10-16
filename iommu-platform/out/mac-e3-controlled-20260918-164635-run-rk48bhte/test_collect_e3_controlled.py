#!/usr/bin/env python3
"""Regression tests for ``scripts/collect_e3_controlled.py``.

The fixture is the recorded console capture of the accepted run: the clean
short and main assessments must stay green (114 / 304 observation lines), and
every targeted mutation of the full short-phase dmesg dump must be rejected.

The dmesg dump inside a phase is the only authoritative copy of the UART
observations (``assess`` never scans the garbled live printks), so all
mutations below are applied inside the short-phase window, which contains that
dump.  The recorded console log is read only; nothing is written.
"""
import re
import sys
from pathlib import Path

import pytest

_HERE = Path(__file__).resolve()
_REPO_ROOT = _HERE.parents[1]
for _path in (str(_HERE.parent), str(_REPO_ROOT)):
    if _path not in sys.path:
        sys.path.insert(0, _path)

import collect_e3_controlled as collector  # noqa: E402

RAW_LOG = (_REPO_ROOT / 'results' / 'serial' /
           '20260918-164542-console-libjqis3' / 'console.raw.log')

# Observation lines recorded for the accepted run (19 per round: 3 batch parts
# + 16 path records).
CLEAN_OBSERVATION_LINES = {'short': 114, 'main': 304}

SHORT_BEGIN_RE = re.compile(r'(?m)^E3CTL: phase=short case=\d+ begin ')
SHORT_COMPLETE_RE = re.compile(r'(?m)^E3CTL: complete ok mode=short ')
MAIN_BEGIN_RE = re.compile(r'(?m)^E3CTL: phase=main case=1 begin ')


@pytest.fixture(scope='session')
def raw():
    """Recorded console bytes, decoded exactly like the collector does."""
    if not RAW_LOG.exists():
        pytest.fail('recorded console log missing: %s' % RAW_LOG)
    return RAW_LOG.read_bytes().decode('utf-8', errors='replace')


@pytest.fixture(scope='session')
def short_window(raw):
    """``(start, end)`` of the short phase, framed the way ``assess`` does."""
    begin = SHORT_BEGIN_RE.search(raw)
    if begin is None:
        pytest.fail('recorded log has no short phase begin marker')
    rest = raw[begin.start():]
    complete = SHORT_COMPLETE_RE.search(rest)
    if complete is not None:
        end = begin.start() + complete.end()
    else:
        following = MAIN_BEGIN_RE.search(rest)
        end = (begin.start() + following.start()
               if following is not None else len(raw))
    return begin.start(), end


# --- mutation helpers -------------------------------------------------------
def _fields(line):
    """``{key: value}`` for every ``key=value`` token on one observation line."""
    return dict(collector.FIELD_RE.findall(line))


def _set_field(line, key, value):
    """Return ``line`` with its ``key=<token>`` field rewritten."""
    return re.sub(r'(?<![\w])%s=\w+' % re.escape(key),
                  '%s=%s' % (key, value), line)


def _rewrite_lines(text, predicate, key, value):
    """Apply ``_set_field`` on every selected line, returning (text, hits)."""
    lines = []
    hits = 0
    for line in text.splitlines(keepends=True):
        if predicate(line):
            rewritten = _set_field(line, key, value)
            if rewritten != line:
                hits += 1
            line = rewritten
        lines.append(line)
    return ''.join(lines), hits


def _assess_short(raw, short_window, edit):
    """Run ``assess`` on a copy of the log with ``edit`` applied to short."""
    start, end = short_window
    return collector.assess(raw[:start] + edit(raw[start:end]) + raw[end:],
                            'short')


def _is_path_obs(line):
    return 'idma_path_obs seq=' in line


def _is_batch_obs(line):
    return 'idma_batch_obs seq=' in line


def _is_stage0_path(line):
    """A stage 0 path record carrying one entry, i.e. the mutated record."""
    if not _is_path_obs(line):
        return False
    fields = _fields(line)
    return (fields.get('stage') == '0' and fields.get('valid') == '1'
            and fields.get('entered') == '1')


def _is_time_obs(line):
    if not _is_batch_obs(line):
        return False
    fields = _fields(line)
    return fields.get('part') == 'time' and fields.get('issued') == '1'


def _last_path_slot(text):
    """``(seq, role, channel, stage)`` of the last complete path record."""
    for line in reversed(text.splitlines()):
        if not _is_path_obs(line):
            continue
        fields = _fields(line)
        if (fields.get('role') in ('v', 'a')
                and fields.get('channel') in ('r', 'w')
                and fields.get('stage') in ('0', '1', '2', '3')
                and fields.get('seq')):
            return (int(fields['seq']), fields['role'], fields['channel'],
                    fields['stage'])
    return None


def _drop_path_slot(text, slot):
    """Delete every copy (live and dump) of one path record."""
    seq, role, channel, stage = slot
    kept = []
    dropped = 0
    for line in text.splitlines(keepends=True):
        if _is_path_obs(line):
            fields = _fields(line)
            if (fields.get('role') == role
                    and fields.get('channel') == channel
                    and fields.get('stage') == stage
                    and fields.get('seq') == str(seq)):
                dropped += 1
                continue
        kept.append(line)
    return ''.join(kept), dropped


# --- clean fixture ----------------------------------------------------------
@pytest.mark.parametrize('phase', sorted(CLEAN_OBSERVATION_LINES))
def test_recorded_phase_passes(raw, phase):
    """The accepted run still passes with its recorded observation lines."""
    report = collector.assess(raw, phase)
    assert report['errors'] == []
    assert report['passed'] is True
    assert len(report['unique_observation_lines']) == \
        CLEAN_OBSERVATION_LINES[phase]
    assert len(report['records']) > 0
    assert all(item['valid'] for item in report['records'])


# --- mutations --------------------------------------------------------------
def test_path_valid_zero_is_rejected(raw, short_window):
    """stage=0 valid=1 entered=1 -> valid=0 must fail (the reported escape)."""
    def edit(text):
        text, hits = _rewrite_lines(text, _is_stage0_path, 'valid', 0)
        assert hits >= 1, 'no stage=0 path observation line found to mutate'
        return text

    report = _assess_short(raw, short_window, edit)
    assert report['passed'] is False
    assert any('valid=0 != 1' in e for e in report['errors']), report['errors']


def test_missing_path_line_is_rejected(raw, short_window):
    def edit(text):
        slot = _last_path_slot(text)
        assert slot is not None, 'no complete path record found to delete'
        text, dropped = _drop_path_slot(text, slot)
        assert dropped >= 1, 'path record was not deleted'
        return text

    report = _assess_short(raw, short_window, edit)
    assert report['passed'] is False
    assert any('path keys missing' in e for e in report['errors']), \
        report['errors']


def test_time_issued_zero_is_rejected(raw, short_window):
    def edit(text):
        text, hits = _rewrite_lines(text, _is_time_obs, 'issued', 0)
        assert hits >= 1, 'no batch time record found to mutate'
        return text

    report = _assess_short(raw, short_window, edit)
    assert report['passed'] is False
    assert any('issued/done/batch_polled=0/1/1' in e
               for e in report['errors']), report['errors']


def test_nonzero_case_exit_is_rejected_and_json_preserved(raw, short_window):
    def edit(text):
        text, hits = re.subn(r'(E3CTL: phase=short case=1 exit=)0',
                             r'\g<1>1', text)
        assert hits >= 1, 'short case 1 exit marker not found'
        return text

    report = _assess_short(raw, short_window, edit)
    assert report['passed'] is False
    assert any('case exit=1' in e for e in report['errors']), report['errors']

    clean = collector.assess(raw, 'short')
    first = report['records'][0]
    assert first['valid'] is False
    assert first['record'] is not None                     # evidence kept
    assert first['record'] == clean['records'][0]['record']


def test_conflicting_duplicate_path_is_rejected(raw, short_window):
    def edit(text):
        lines = text.splitlines(keepends=True)
        index = None
        for i, line in enumerate(lines):
            if _is_stage0_path(line):
                index = i
        assert index is not None, 'no stage=0 path line to duplicate'
        duplicate = _set_field(lines[index], 'entered', 2)
        assert duplicate != lines[index]
        if not duplicate.endswith('\n'):
            duplicate += '\n'
        lines.insert(index + 1, duplicate)
        return ''.join(lines)

    report = _assess_short(raw, short_window, edit)
    assert report['passed'] is False
    assert any('conflicting duplicate path' in e for e in report['errors']), \
        report['errors']


def test_missing_complete_marker_is_rejected(raw):
    mutated, hits = re.subn(
        r'(?m)^E3CTL: complete ok mode=short [^\r\n]*\r?\n', '', raw)
    assert hits >= 1, 'short phase complete marker not found'

    report = collector.assess(mutated, 'short')
    assert report['passed'] is False
    assert any('phase complete missing' in e for e in report['errors']), \
        report['errors']


def test_kernel_warning_is_rejected(raw, short_window):
    start, _ = short_window
    # Inject inside the phase, after its first begin line; preceding boot logs
    # are intentionally outside this phase's validation boundary.
    start = raw.index('\n', start) + 1
    mutated = (raw[:start] + '[   12.345678] WARNING: synthetic mutation\n'
               + raw[start:])

    report = collector.assess(mutated, 'short')
    assert report['passed'] is False
    assert any('Kernel error' in e for e in report['errors']), report['errors']
