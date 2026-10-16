#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
mac_nonblock_pilot.py - Mac host runner for the frozen nonblock engineering pilot.

One invocation == one BIT == one fresh boot == fifteen frozen cases.
There is no selection, no tuning, no retry, no resume and no rerun.

Frozen plan
-----------
short group (transfers=64, warmup=16)     : Solo, A1, A8
long  group (transfers=4096, warmup=256)  : Solo/A1/A8/A16 over three rounds
    round 1 order : Solo, A1, A8, A16
    round 2 order : A16, A8, A1, Solo     (reversed)
    round 3 order : Solo, A1, A8, A16     (base)

Every case is one P4M hardware-window run with
    --chunk 4096 --victim-pages 1 --a-gap-cycles 0 --timeout-ms 60000
and an aggressor of none (Solo) or DMA instance 1 with 1/8/16 pages.

Reported metrics are deliberately narrow: the P4M window cycle count and the A
jobs completed inside the window.  Raw HPM counters stay inside the per-case
record but are explicitly not treated as a validated metric here.

Reused from mac_research2.py: the framed serial console only (reader thread,
raw log, begin/end marker framing, read_until/read_line).  The manifest,
role-2 capability, stage-save/hash and tar/base64 archive flows of
mac_research2.py are deliberately not used.

This file is a proposal.  It has not been executed against hardware.
'''
from __future__ import annotations

import argparse
import base64
import hashlib
import importlib.util
import json
import pathlib
import re
import secrets
import shlex
import subprocess
import sys
import time
from typing import Any, Dict, List, Optional, Sequence, Tuple

# --------------------------------------------------------------------------
# constants
# --------------------------------------------------------------------------

BOARD = 'genesys2'
DEFAULT_BOARD_CLI = '/usr/bin/idma_experiment'
DEFAULT_DEVICE = '/dev/idma0'
DEFAULT_SERIAL = '/dev/cu.usbserial-AV0KRCHT'
DEFAULT_OPENFPGALOADER = '/opt/homebrew/bin/openFPGALoader'
DEFAULT_BOOT_ID_PATH = '/proc/sys/kernel/random/boot_id'
DEFAULT_TAINT_PATH = '/proc/sys/kernel/tainted'
DEFAULT_BAUDRATE = 115200
DEFAULT_JTAG_FREQ = 1000000

# The pilot only ever targets the one frozen engineering kernel; the value is
# checked, not merely recorded.
EXPECTED_UNAME_R = '6.5.0-rc2-00093-gc0f5e13ea81f'
BOOT_ID_RE = re.compile(r'^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-'
                        r'[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$')

# --recover-startup-from: the original-serial-02 run stopped at the Buildroot
# "Saving random seed" step.  Its pre-flash boot_id (the boot that ran before
# the BIT was flashed) is known from its console/run log and is reused only so
# the recovered boot can be shown to be a different boot.
RECOVER_KNOWN_PRE_FLASH_BOOT_ID = '236b6316-cca3-49be-b52b-48f3963fa515'
RECOVER_MANUAL_CTRL_C_NOTE = (
    'the stopped boot was unblocked by a single manual operator CTRL-C at the '
    'Buildroot "Saving random seed" step; no experiment was re-run')
RECOVER_PROMPT_ELICIT_DELAY = 1.0

# --interrupt-stalled-seed: a CTRL-C is only ever sent after a real fresh Linux
# banner, and only on a visible "Saving random seed" stall.
STALLED_SEED_MARKER = 'Saving random seed'
STALLED_SEED_SILENCE = 30.0

# struct idma_experiment_caps is eight __u32 followed by reserved[8] __u32, i.e.
# exactly 64 bytes.  The caps block describes that structure and must never be
# compared with input.struct_size, which is sizeof(struct idma_experiment).
CAPS_ABI_VERSION = 1
CAPS_STRUCT_SIZE = 64
CAPS_DEVICE_ID = 0
CAPS_DID = 10
# CAP_DID_HPM | CAP_HW_BATCH | CAP_P4M
CAPS_REQUIRED_FLAGS = (1 << 0) | (1 << 2) | (1 << 7)
CAPS_RESERVED_WORDS = 8
DEV_MEM_PATH = '/dev/mem'

DEFAULT_BOOT_TIMEOUT = 300.0
DEFAULT_COMMAND_TIMEOUT = 60.0
DEFAULT_CASE_TIMEOUT = 600.0
DEFAULT_EXPORT_TIMEOUT = 120.0
DEFAULT_FLASH_TIMEOUT = 180.0
PRE_FLASH_PROBE_TIMEOUT = 20.0
DEFAULT_UPLOAD_TIMEOUT = 300.0

DEFAULT_CHUNK = 4096
DEFAULT_TIMEOUT_MS = 60000
MAX_AGGRESSOR_PAGES = 16
MAX_EXPORT_BYTES = 8 * 1024 * 1024

LABELS = ('original-serial', 'banked-serial', 'banked-hit')
# --reader is mandatory for the two banked labels (only those bitstreams have
# the RO hardware) and forbidden for original-serial.
READER_REQUIRED_LABELS = ('banked-serial', 'banked-hit')
READER_FORBIDDEN_LABELS = ('original-serial',)

P4M_SCHEMA = 'idma.p4m.v1'
CAPS_SCHEMA = 'idma.experiment.v1'
P4M_BANK_MAGIC = 0x49444D4150344D01
P4M_WINDOW_WORDS = 16
P4M_OBS_WORDS = 35
P4M_OBSERVER_STATUS_OK = 4

# Static RISC-V ELF helper, ABI idma-nonblock-window-v1.  The ELF is built
# later; this runner only uploads it and validates the JSON it prints.
READER_ABI = 'idma-nonblock-window-v1'
READER_WINDOW_WORDS = 16
READER_MAGIC = 0x49444D414E424C01
READER_VERSION = 1
READER_FLAGS_FROZEN = 4          # window[3]
READER_ERRORS_OK = 0             # window[15]
READER_MODE_FOR_LABEL = {'banked-serial': 1, 'banked-hit': 2}
READER_FAST_WORDS = (6, 7, 8, 9, 10)   # Vread, Vwrite, Aread, Awrite, Other
READER_NORMAL_WORDS = (11, 12, 13)     # V, A, Other

# Optional second 16-word read-only diagnostic bank (index 16..31).  It is
# read only when the helper is invoked with --diagnostics; the frozen 16-word
# core ABI above is unchanged.
READER_DIAG_WORDS = 16
READER_DIAG_MAGIC = 0x49444D414E444701
READER_DIAG_VERSION = 1
READER_DIAG_FLAGS_FROZEN = 4
READER_DIAG_FAST_ACCEPT_WORD = 15
READER_DIAG_UINT64_MAX = 0xFFFFFFFFFFFFFFFF
UPLOAD_CHUNK = 1024
# Raw-UART heredoc upload tuning.  At 115200 8N1 one 1024-byte base64 line
# costs about 89 ms of wire time, so each payload line is followed by a short
# pause to keep the host write queue drained.  The write timeout is raised only
# for this upload and is always restored; read_timeout is never touched.
UPLOAD_WRITE_TIMEOUT = 5.0
UPLOAD_LINE_DELAY = 0.15
HELPER_FILES = ('helper.json', 'helper.stderr.txt', 'helper.exit.txt')

SHORT_TRANSFERS = 64
SHORT_WARMUP = 16
LONG_TRANSFERS = 4096
LONG_WARMUP = 256

# condition -> (--aggressor value, --aggressor-pages)
CONDITIONS: Dict[str, Tuple[str, int]] = {
    'Solo': ('none', 8),
    'A1': ('1', 1),
    'A8': ('1', 8),
    'A16': ('1', 16),
}

LONG_ROUND_ORDERS: Tuple[Tuple[str, ...], ...] = (
    ('Solo', 'A1', 'A8', 'A16'),
    ('A16', 'A8', 'A1', 'Solo'),
    ('Solo', 'A1', 'A8', 'A16'),
)

# dmesg lines that must be preserved as a flag, never dropped.
WARNING_PATTERNS: Tuple[re.Pattern, ...] = (
    re.compile(r'\bWARNING:'),
    re.compile(r'\bBUG:'),
    re.compile(r'\bOops\b'),
    re.compile(r'Call Trace'),
    re.compile(r'general protection fault', re.IGNORECASE),
    re.compile(r'\bKernel panic\b', re.IGNORECASE),
)

# Root-shell / login evidence for the post-flash wait.  The shell prompt is not
# newline terminated, so it is matched against the accumulated drain buffer.
# Buildroot's real root shell prints a bare '# ' at the start of a line, which
# is the form the board shows; '/ #' and the 'root@host:path#' form are kept.
# The root@ branch must contain its trailing '#' so a hostname colon on its own
# can never be mistaken for a prompt.
ROOT_PROMPT_RE = re.compile(r'(?m)^#(?: |$)|^/ #|root@\S*#')
LOGIN_PROMPT_RE = re.compile(r'login:\s*$')
PASSWORD_PROMPT_RE = re.compile(r'Password:\s*$')

_DQUOTE = chr(34)
_B64_WS = frozenset(bytes([32, 9, 13, 10, 11, 12]))
_INT_RE = re.compile(r'-?[0-9]+')

# The reused console class must expose these; anything else is rejected.
REQUIRED_CONSOLE_METHODS = ('start', 'close', 'run_shell', 'read_line', 'drain_input')


class NonblockError(Exception):
    """Technical failure.  Stops the run after the current case evidence is kept."""


# --------------------------------------------------------------------------
# small helpers
# --------------------------------------------------------------------------

def utc_now() -> str:
    return time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())


def sha256_file(path: pathlib.Path) -> str:
    digest = hashlib.sha256()
    with open(path, 'rb') as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b''):
            digest.update(chunk)
    return digest.hexdigest()


def text_of(data: Any) -> str:
    if isinstance(data, bytes):
        return data.decode('utf-8', errors='replace')
    return str(data)


# --------------------------------------------------------------------------
# frozen plan
# --------------------------------------------------------------------------

def build_case_plan() -> List[Dict[str, Any]]:
    '''Return the fifteen frozen cases in execution order.'''
    cases: List[Dict[str, Any]] = []
    for condition in ('Solo', 'A1', 'A8'):
        cases.append({
            'id': 'short-' + condition.lower(),
            'group': 'short',
            'condition': condition,
            'round': None,
            'transfers': SHORT_TRANSFERS,
            'warmup': SHORT_WARMUP,
        })
    for number, order in enumerate(LONG_ROUND_ORDERS, start=1):
        for condition in order:
            cases.append({
                'id': 'long-r' + str(number) + '-' + condition.lower(),
                'group': 'long',
                'condition': condition,
                'round': number,
                'transfers': LONG_TRANSFERS,
                'warmup': LONG_WARMUP,
            })
    return cases


def build_case_argv(case: Dict[str, Any], device: str) -> List[str]:
    aggressor, pages = CONDITIONS[case['condition']]
    return [
        '--device', device,
        '--p4m',
        '--aggressor', aggressor,
        '--a-gap-cycles', '0',
        '--victim-pages', '1',
        '--aggressor-pages', str(pages),
        '--chunk', str(DEFAULT_CHUNK),
        '--transfers', str(case['transfers']),
        '--warmup', str(case['warmup']),
        '--timeout-ms', str(DEFAULT_TIMEOUT_MS),
    ]


# --------------------------------------------------------------------------
# reused serial console
# --------------------------------------------------------------------------

def load_mac_research2():
    '''Load mac_research2.py by file path.

    The scripts directory is deliberately never put on sys.path, so a sibling
    module name (the classic scripts/platform.py) cannot shadow a stdlib module.
    Mirrors scripts/mac_research2.py:686-707.
    '''
    here = pathlib.Path(__file__).resolve().parent
    path = here / 'mac_research2.py'
    if not path.is_file():
        raise NonblockError('mac_research2.py not found next to this runner: ' + str(path))
    spec = importlib.util.spec_from_file_location('nonblock_mac_research2', path)
    if spec is None or spec.loader is None:
        raise NonblockError('cannot load ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules['nonblock_mac_research2'] = module
    spec.loader.exec_module(module)
    return module


def pick_console_class(module: Any):
    for name in ('SerialShellConsole', 'SerialCapture', 'SerialConsole'):
        cls = getattr(module, name, None)
        if cls is None:
            continue
        missing = [m for m in REQUIRED_CONSOLE_METHODS
                   if not callable(getattr(cls, m, None))]
        if missing:
            raise NonblockError(name + ' cannot be reused, missing: ' + ', '.join(missing))
        return cls
    raise NonblockError('mac_research2.py exposes no reusable serial console class')


def make_pilot_console_class(base_class):
    '''Minimal extension: the reused console can run framed commands but has no
    public way to type a bare line, which is needed to answer the login prompt.'''

    class PilotConsole(base_class):
        def write_line(self, text: str) -> None:
            self._write((text + chr(10)).encode('utf-8'))

    PilotConsole.__name__ = 'Pilot' + base_class.__name__
    return PilotConsole


def check_uart_ownership(serial_port: str) -> str:
    '''Read-only ownership check.  Never kills another process.'''
    device = serial_port if serial_port.startswith('/dev/') else '/dev/' + serial_port
    if not pathlib.Path(device).exists():
        raise NonblockError('serial device is not present: ' + device)
    try:
        proc = subprocess.run(['lsof', '--', device], stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE, check=False)
    except OSError as exc:
        raise NonblockError('lsof is not available: ' + repr(exc))
    if proc.returncode == 0 and proc.stdout.strip():
        raise NonblockError('UART ' + device + ' is already owned by another process:\n'
                            + proc.stdout.decode('utf-8', errors='replace'))
    return device


# --------------------------------------------------------------------------
# validation of one P4M record (few constant fields + CLI-enforced code)
# --------------------------------------------------------------------------

def validate_record(case: Dict[str, Any], record: Any) -> List[str]:
    problems: List[str] = []
    if not isinstance(record, dict):
        return ['record is not a JSON object']
    if record.get('schema') != P4M_SCHEMA:
        problems.append('schema ' + repr(record.get('schema')) + ' is not ' + P4M_SCHEMA)
    if record.get('success') is not True:
        problems.append('success is not true')
    error = record.get('error')
    if isinstance(error, str) and error.strip():
        problems.append('error: ' + error)

    result = record.get('result')
    if not isinstance(result, dict):
        problems.append('result is not an object')
    else:
        if result.get('status') != 0:
            problems.append('result.status ' + repr(result.get('status')))
        for side in ('victim', 'aggressor'):
            stats = result.get(side)
            if side == 'aggressor' and case['condition'] == 'Solo':
                if stats is not None:
                    problems.append('result.aggressor is not null for a Solo case')
                continue
            if not isinstance(stats, dict):
                problems.append('result.' + side + ' is not an object')
                continue
            if stats.get('data_errors') != 0:
                problems.append('result.' + side + '.data_errors '
                                + repr(stats.get('data_errors')))
            if stats.get('quarantined') != 0:
                problems.append('result.' + side + '.quarantined '
                                + repr(stats.get('quarantined')))

    p4m = record.get('p4m')
    if not isinstance(p4m, dict):
        problems.append('p4m is not an object')
        return problems

    if p4m.get('bank_magic') != P4M_BANK_MAGIC:
        problems.append('p4m.bank_magic ' + repr(p4m.get('bank_magic')))
    if p4m.get('observer_status') != P4M_OBSERVER_STATUS_OK:
        problems.append('p4m.observer_status ' + repr(p4m.get('observer_status')))
    if p4m.get('closed') is not True:
        problems.append('p4m.closed is not true')

    cycles = p4m.get('cycles')
    if isinstance(cycles, bool) or not isinstance(cycles, int) or cycles <= 0:
        problems.append('p4m.cycles ' + repr(cycles))
    if p4m.get('hardware_window_ns') is None:
        problems.append('p4m.hardware_window_ns is null')

    window = p4m.get('raw_window')
    observation = p4m.get('raw_observation')
    if not isinstance(window, list) or len(window) != P4M_WINDOW_WORDS:
        problems.append('p4m.raw_window is not ' + str(P4M_WINDOW_WORDS) + ' words')
    if not isinstance(observation, list) or len(observation) != P4M_OBS_WORDS:
        problems.append('p4m.raw_observation is not ' + str(P4M_OBS_WORDS) + ' words')

    transfers = int(case['transfers'])
    if isinstance(window, list) and isinstance(observation, list) and window and observation:
        if window[0] != P4M_BANK_MAGIC:
            problems.append('raw_window[0] magic ' + repr(window[0]))
        if window[1] != observation[0]:
            problems.append('raw_window[1] ' + repr(window[1])
                            + ' != raw_observation[0] ' + repr(observation[0]))
        if len(window) > 14 and (window[13] != transfers or window[14] != transfers):
            problems.append('raw_window V issued/done '
                            + repr((window[13], window[14])) + ' != ' + str(transfers))
        if len(observation) > 8 and (observation[7] != transfers or observation[8] != transfers):
            problems.append('raw_observation V issued/done '
                            + repr((observation[7], observation[8])) + ' != ' + str(transfers))

    expected_has_a = case['condition'] != 'Solo'
    if bool(p4m.get('has_a')) != expected_has_a:
        problems.append('p4m.has_a ' + repr(p4m.get('has_a'))
                        + ' for condition ' + case['condition'])
    return problems


def case_metrics(case: Dict[str, Any], record: Dict[str, Any]) -> Dict[str, Any]:
    '''Metrics are deliberately limited to two window numbers.'''
    p4m = record.get('p4m') or {}
    return {
        'group': case['group'],
        'condition': case['condition'],
        'round': case['round'],
        'transfers': case['transfers'],
        'warmup': case['warmup'],
        'v_window_cycles': p4m.get('cycles'),
        'a_completed_jobs_window': p4m.get('a_completed_jobs_window'),
        'hpm': 'raw-only, incomplete under nonblock; not a validated metric',
    }


def helper_mechanism_expectations(record: Any) -> Tuple[bool, bool]:
    '''Which mechanism counters the helper window must show as non-zero.

    A normal P4M case that ran victim transfers moved V, so the helper must
    have counted at least one V accept; a case whose window reports has_a must
    likewise show at least one A accept.  When the record cannot be read the V
    guard stays armed, so an all-zero window can never be reported as a pass.
    The raw window is never rewritten; the guard only adds problems.
    '''
    expect_v = True
    expect_a = False
    if isinstance(record, dict):
        p4m = record.get('p4m')
        if isinstance(p4m, dict):
            expect_a = bool(p4m.get('has_a'))
    return expect_v, expect_a


def validate_helper_window(window: Any, label: str, expected_cycles: Any,
                           previous_seq: Optional[int],
                           expect_v_movement: bool = True,
                           expect_a: bool = False) -> List[str]:
    '''Validate the static-ELF helper window (ABI idma-nonblock-window-v1).

    The full 16-word window is kept verbatim elsewhere; nothing here is
    reinterpreted as an HPM counter.  The mechanism liveness guards below are
    additive: an all-zero window satisfies window[14] == fast + normal
    trivially, so without them a run in which nothing was counted at all would
    be accepted as a pass.  The raw words are never modified.
    '''
    problems: List[str] = []
    if not isinstance(window, list) or len(window) != READER_WINDOW_WORDS:
        return ['helper window is not ' + str(READER_WINDOW_WORDS) + ' words']
    if any(isinstance(value, bool) or not isinstance(value, int)
           for value in window):
        return ['helper window contains non-integer words']
    if window[0] != READER_MAGIC:
        problems.append('helper window[0] magic ' + repr(window[0]))
    if window[1] != READER_VERSION:
        problems.append('helper window[1] version ' + repr(window[1]))
    expected_mode = READER_MODE_FOR_LABEL.get(label)
    if window[2] != expected_mode:
        problems.append('helper window[2] mode ' + repr(window[2])
                        + ' != ' + repr(expected_mode) + ' for ' + label)
    if window[3] != READER_FLAGS_FROZEN:
        problems.append('helper window[3] flags ' + repr(window[3])
                        + ' != ' + str(READER_FLAGS_FROZEN))
    if window[5] != expected_cycles:
        problems.append('helper window[5] cycles ' + repr(window[5])
                        + ' != p4m.raw_window[1] ' + repr(expected_cycles))
    if window[15] != READER_ERRORS_OK:
        problems.append('helper window[15] errors ' + repr(window[15]))
    fast = sum(window[i] for i in READER_FAST_WORDS)
    normal = sum(window[i] for i in READER_NORMAL_WORDS)
    if window[14] != fast + normal:
        problems.append('helper accepted ' + repr(window[14]) + ' != fast '
                        + repr(fast) + ' + normal ' + repr(normal))

    # --- mechanism liveness guards (banked labels only; see call site) ------
    # V fast words: 6=Vread, 7=Vwrite; V normal word: 11
    # A fast words: 8=Aread, 9=Awrite; A normal word: 12
    if expect_v_movement or expect_a:
        if window[14] <= 0:
            problems.append('helper window[14] accepted is not > 0 although '
                            'the case ran V/A transfers')
    if expect_v_movement:
        v_counts = (window[6], window[7], window[11])
        if sum(v_counts) <= 0:
            problems.append('helper V fast+normal counts ' + repr(v_counts)
                            + ' are not sum > 0 although the case ran V transfers')
    if expect_a:
        a_counts = (window[8], window[9], window[12])
        if sum(a_counts) <= 0:
            problems.append('helper A fast+normal counts ' + repr(a_counts)
                            + ' are not sum > 0 although an A aggressor ran')
    if previous_seq is not None and window[4] <= previous_seq:
        problems.append('helper window[4] sequence ' + repr(window[4])
                        + ' did not increase past ' + repr(previous_seq))
    return problems


def validate_helper_diagnostic(window: Any, diagnostic: Any,
                               label: str) -> List[str]:
    '''Validate the optional second 16-word diagnostic bank.

    The helper validates this bank before printing it; this runner repeats the
    structural checks so a malformed helper.json can never be recorded as a
    completed case.  The bank is passive: its words are not HPM counters and
    never replace the frozen 16-word core ABI.  Words must be real uint64
    values: bool, non-integer, negative and > 2**64 - 1 words are rejected
    before any arithmetic, so a coerced value can never look valid.
    '''
    problems: List[str] = []
    if not isinstance(window, list) or len(window) != READER_WINDOW_WORDS:
        return ['diagnostic check needs the core 16-word window']
    if not isinstance(diagnostic, dict):
        return ['helper diagnostic is not an object']
    words = diagnostic.get('words')
    if not isinstance(words, list) or len(words) != READER_DIAG_WORDS:
        return ['helper diagnostic.words is not '
                + str(READER_DIAG_WORDS) + ' words']
    for value in words:
        if isinstance(value, bool) or not isinstance(value, int):
            return ['helper diagnostic.words contains non-integer words']
        if value < 0 or value > READER_DIAG_UINT64_MAX:
            return ['helper diagnostic.words contains a word outside uint64: '
                    + repr(value)]
    if words[0] != READER_DIAG_MAGIC:
        problems.append('helper diagnostic[0] magic ' + repr(words[0]))
    if words[1] != READER_DIAG_VERSION:
        problems.append('helper diagnostic[1] version ' + repr(words[1]))
    expected_mode = READER_MODE_FOR_LABEL.get(label)
    if words[2] != expected_mode:
        problems.append('helper diagnostic[2] mode ' + repr(words[2])
                        + ' != ' + repr(expected_mode) + ' for ' + label)
    if words[3] != READER_DIAG_FLAGS_FROZEN:
        problems.append('helper diagnostic[3] flags ' + repr(words[3])
                        + ' != ' + str(READER_DIAG_FLAGS_FROZEN))
    if words[4] != window[4]:
        problems.append('helper diagnostic[4] seq ' + repr(words[4])
                        + ' != core window[4] ' + repr(window[4]))
    if words[5] != window[5]:
        problems.append('helper diagnostic[5] cycles ' + repr(words[5])
                        + ' != core window[5] ' + repr(window[5]))
    total = 0
    for index in READER_FAST_WORDS:
        value = window[index]
        if isinstance(value, bool) or not isinstance(value, int):
            return ['diagnostic check needs integer core fast words']
        total += value
    if total > READER_DIAG_UINT64_MAX:
        problems.append('helper diagnostic core fast sum overflows 64 bits')
    elif words[READER_DIAG_FAST_ACCEPT_WORD] != total:
        problems.append('helper diagnostic[15] fast_accepts '
                        + repr(words[READER_DIAG_FAST_ACCEPT_WORD])
                        + ' != sum(core fast 6..10) ' + repr(total))
    return problems


# --------------------------------------------------------------------------
# capability gate (--caps)
# --------------------------------------------------------------------------

def validate_caps(caps: Any, device: str) -> List[str]:
    problems: List[str] = []
    if not isinstance(caps, dict):
        return ['caps is not a JSON object']
    if caps.get('schema') != CAPS_SCHEMA:
        problems.append('caps schema ' + repr(caps.get('schema')) + ' is not ' + CAPS_SCHEMA)
    if caps.get('success') is not True:
        problems.append('caps success is not true')
    error = caps.get('error')
    if isinstance(error, str) and error.strip():
        problems.append('caps error: ' + error)

    inp = caps.get('input')
    if not isinstance(inp, dict):
        problems.append('caps input is not an object')
        inp = {}
    if inp.get('caps_only') is not True:
        problems.append('caps input.caps_only is not true')
    if inp.get('device') != device:
        problems.append('caps input.device ' + repr(inp.get('device'))
                        + ' != ' + repr(device))

    caps_block = caps.get('capabilities')
    if not isinstance(caps_block, dict):
        problems.append('caps capabilities is null')
        return problems

    if caps_block.get('page_bytes') != DEFAULT_CHUNK:
        problems.append('caps page_bytes ' + repr(caps_block.get('page_bytes'))
                        + ' != ' + str(DEFAULT_CHUNK))
    if caps_block.get('did_hpm_supported') is not True:
        problems.append('caps did_hpm_supported is not true')
    max_pages = caps_block.get('max_pages')
    if not isinstance(max_pages, int) or max_pages < MAX_AGGRESSOR_PAGES:
        problems.append('caps max_pages ' + repr(max_pages)
                        + ' < ' + str(MAX_AGGRESSOR_PAGES))
    max_timeout = caps_block.get('max_run_timeout_ms')
    if not isinstance(max_timeout, int) or max_timeout < DEFAULT_TIMEOUT_MS:
        problems.append('caps max_run_timeout_ms ' + repr(max_timeout)
                        + ' < ' + str(DEFAULT_TIMEOUT_MS))
    # The caps block describes struct idma_experiment_caps, whose size (64) is a
    # different struct from the input struct_size; comparing the two is invalid
    # and is deliberately not done here.
    if caps_block.get('abi_version') != CAPS_ABI_VERSION:
        problems.append('caps abi_version ' + repr(caps_block.get('abi_version'))
                        + ' != ' + str(CAPS_ABI_VERSION))
    if caps_block.get('struct_size') != CAPS_STRUCT_SIZE:
        problems.append('caps struct_size ' + repr(caps_block.get('struct_size'))
                        + ' != ' + str(CAPS_STRUCT_SIZE))
    if caps_block.get('device_id') != CAPS_DEVICE_ID:
        problems.append('caps device_id ' + repr(caps_block.get('device_id'))
                        + ' != ' + str(CAPS_DEVICE_ID))
    if caps_block.get('did') != CAPS_DID:
        problems.append('caps did ' + repr(caps_block.get('did'))
                        + ' != ' + str(CAPS_DID))
    flags = caps_block.get('flags')
    if isinstance(flags, bool) or not isinstance(flags, int) or \
            (flags & CAPS_REQUIRED_FLAGS) != CAPS_REQUIRED_FLAGS:
        problems.append('caps flags ' + repr(flags) + ' lack required bits '
                        + hex(CAPS_REQUIRED_FLAGS))
    reserved = caps_block.get('reserved')
    if not isinstance(reserved, list) or len(reserved) != CAPS_RESERVED_WORDS \
            or any(value != 0 for value in reserved):
        problems.append('caps reserved is not ' + str(CAPS_RESERVED_WORDS)
                        + ' zero words: ' + repr(reserved))
    # CAP_DID_HPM / CAP_HW_BATCH / CAP_P4M are checked through the printed
    # flags word above; the per-case CLI enforces them too.  See doc A3.
    return problems


# --------------------------------------------------------------------------
# runner
# --------------------------------------------------------------------------

class PilotRunner:
    def __init__(self, args, console, out_dir: pathlib.Path) -> None:
        self.args = args
        self.console = console
        self.out_dir = pathlib.Path(out_dir)
        self.board_cli = args.board_cli
        self.run_id = secrets.token_hex(8)
        self.board_root = '/tmp/nonblock-' + self.run_id
        self.case_plan = build_case_plan()
        self.results: List[Dict[str, Any]] = []
        self.current_case: Optional[str] = None
        self.boot_id_before: Optional[str] = None
        self.last_helper_seq: Optional[int] = None
        self.started_at = utc_now()
        self.stop_written = False
        self.recovery: Optional[Dict[str, Any]] = None
        self._recovery_boot_id_source = 'recorded-constant'

    # ---- logging / writing -------------------------------------------------

    def _log(self, message: str) -> None:
        line = utc_now() + ' ' + message
        try:
            with open(self.out_dir / 'run.log', 'a', encoding='utf-8') as handle:
                handle.write(line + chr(10))
        except OSError:
            pass
        print(line, file=sys.stderr)

    def _write_json(self, relative: str, obj: Any) -> None:
        path = self.out_dir / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        tmp = path.with_suffix(path.suffix + '.tmp')
        with open(tmp, 'w', encoding='utf-8') as handle:
            json.dump(obj, handle, ensure_ascii=False, indent=2, sort_keys=True)
            handle.write(chr(10))
        tmp.replace(path)

    def _stop(self, reason: str) -> None:
        if self.stop_written:
            return
        self.stop_written = True
        try:
            with open(self.out_dir / 'STOPPED.txt', 'w', encoding='utf-8') as handle:
                handle.write('stopped_at=' + utc_now() + chr(10))
                handle.write('reason=' + reason + chr(10))
                handle.write('case=' + str(self.current_case) + chr(10))
        except OSError:
            pass

    # ---- board helpers -----------------------------------------------------

    def _board_case_dir(self, cid: str) -> str:
        return self.board_root + '/cases/' + cid

    def _export_file(self, board_path: str, local_b64_path: pathlib.Path,
                     local_path: pathlib.Path) -> bytes:
        '''Bounded base64 read-back of one board file using the framed console.'''
        result = self.console.run_shell('base64 ' + shlex.quote(board_path),
                                        timeout=self.args.export_timeout)
        raw = result.stdout
        local_b64_path.parent.mkdir(parents=True, exist_ok=True)
        with open(local_b64_path, 'wb') as handle:
            handle.write(raw)
        if result.exit_code != 0:
            raise NonblockError('base64 exit=' + str(result.exit_code)
                                + ' for ' + board_path)
        if len(raw) > MAX_EXPORT_BYTES:
            raise NonblockError('base64 output too large for ' + board_path
                                + ': ' + str(len(raw)))
        cleaned = bytes(byte for byte in raw if byte not in _B64_WS)
        try:
            data = base64.b64decode(cleaned, validate=True)
        except Exception as exc:
            raise NonblockError('base64 decode failed for ' + board_path + ': ' + repr(exc))
        local_path.parent.mkdir(parents=True, exist_ok=True)
        with open(local_path, 'wb') as handle:
            handle.write(data)
        return data

    def _capture_taint(self, board_dir: str, name: str) -> None:
        result = self.console.run_shell(
            'cat ' + DEFAULT_TAINT_PATH + ' > ' + shlex.quote(name),
            timeout=self.args.command_timeout, cwd=board_dir)
        if result.exit_code != 0:
            raise NonblockError('taint capture failed: ' + name
                                + ' exit=' + str(result.exit_code))

    def _capture_dmesg(self, name: str) -> None:
        result = self.console.run_shell('dmesg', timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise NonblockError('dmesg exit=' + str(result.exit_code))
        if len(result.stdout) > MAX_EXPORT_BYTES:
            raise NonblockError('dmesg output too large: ' + str(len(result.stdout)))
        with open(self.out_dir / name, 'wb') as handle:
            handle.write(result.stdout)

    # ---- flash / boot ------------------------------------------------------

    def flash_once(self) -> Dict[str, Any]:
        '''Flash the BIT exactly once for this run (fresh boot resets policy).'''
        bit = pathlib.Path(self.args.bit).resolve()
        if not bit.is_file():
            raise NonblockError('BIT file not found: ' + str(bit))
        command = [self.args.openfpgaloader, '-b', BOARD, '--freq',
                   str(self.args.jtag_freq), str(bit)]
        self._log('flash: ' + ' '.join(shlex.quote(part) for part in command))
        self.console.drain_input()
        try:
            proc = subprocess.run(command, stdout=subprocess.PIPE,
                                  stderr=subprocess.PIPE,
                                  timeout=self.args.flash_timeout, check=False)
        except subprocess.TimeoutExpired as exc:
            raise NonblockError('openFPGALoader timed out: ' + repr(exc))
        except OSError as exc:
            raise NonblockError('openFPGALoader could not start: ' + repr(exc))
        document = {
            'schema': 'nonblock-pilot-flash-v1',
            'board': BOARD,
            'cmd': command,
            'returncode': proc.returncode,
            'stdout': proc.stdout.decode('utf-8', errors='replace'),
            'stderr': proc.stderr.decode('utf-8', errors='replace'),
            'bit': str(bit),
            'bit_sha256': sha256_file(bit),
            'finished_at': utc_now(),
        }
        self._write_json('flash.json', document)
        if proc.returncode != 0:
            raise NonblockError('openFPGALoader rc=' + str(proc.returncode)
                                + ': ' + document['stderr'][-400:])
        return document

    def _recover_pre_flash_boot_id(self, source: pathlib.Path) -> str:
        '''Best evidence of the pre-flash boot_id of the stopped run.

        Preference order: the source run.log 'pre-flash boot_id=' line, then
        the operator-recorded constant.  Only a valid UUID is accepted; the
        value is used solely by the boot_id-different check on the recovered
        boot and is never treated as a gate on its own.
        '''
        self._recovery_boot_id_source = 'recorded-constant'
        log_path = source / 'run.log'
        if log_path.is_file():
            text = log_path.read_text(encoding='utf-8', errors='replace')
            match = re.search(r'pre-flash boot_id=([0-9a-fA-F-]{36})', text)
            if match:
                value = match.group(1).strip().lower()
                if BOOT_ID_RE.fullmatch(value):
                    self._recovery_boot_id_source = 'run.log'
                    return value
        return RECOVER_KNOWN_PRE_FLASH_BOOT_ID

    def recover_startup(self) -> Dict[str, Any]:
        '''Reuse the flash record of a previously stopped run.

        Read-only against the source; the current output must still not exist
        (checked by main).  This never re-flashes the BIT, never re-hashes the
        BIT, never tars anything and never re-runs an experiment.  It only
        proves the previous run flashed the same BIT/label and collected no
        sample, records the old pre-flash boot_id and marks this boot as a
        recovered startup.  Every later gate (uname/kernel, boot_id-different,
        caps, reader mode, taint) still runs unchanged.
        '''
        source = pathlib.Path(self.args.recover_startup_from).resolve()
        if not source.is_dir():
            raise NonblockError('--recover-startup-from is not a directory: '
                                + str(source))
        if source == self.out_dir.resolve():
            raise NonblockError('--recover-startup-from must differ from --out')
        if not (source / 'STOPPED.txt').is_file():
            raise NonblockError('recovery source has no STOPPED.txt: ' + str(source))

        flash_path = source / 'flash.json'
        if not flash_path.is_file():
            raise NonblockError('recovery source has no flash.json: ' + str(source))
        try:
            flash = json.loads(flash_path.read_text(encoding='utf-8',
                                                    errors='replace'))
        except ValueError as exc:
            raise NonblockError('recovery flash.json is not JSON: ' + repr(exc))
        if not isinstance(flash, dict):
            raise NonblockError('recovery flash.json is not an object')
        if flash.get('returncode') != 0:
            raise NonblockError('recovery source flash did not succeed: '
                                + repr(flash.get('returncode')))
        want_bit = pathlib.Path(self.args.bit).resolve()
        try:
            have_bit = pathlib.Path(str(flash.get('bit'))).resolve()
        except Exception:
            have_bit = None
        if have_bit != want_bit:
            raise NonblockError('recovery source flashed a different BIT: '
                                + repr(flash.get('bit'))
                                + ' != ' + repr(str(want_bit)))

        manifest_path = source / 'manifest.json'
        if not manifest_path.is_file():
            raise NonblockError('recovery source has no manifest.json: '
                                + str(source))
        try:
            manifest = json.loads(manifest_path.read_text(encoding='utf-8',
                                                          errors='replace'))
        except ValueError as exc:
            raise NonblockError('recovery manifest.json is not JSON: ' + repr(exc))
        if not isinstance(manifest, dict):
            raise NonblockError('recovery manifest.json is not an object')
        if manifest.get('label') != self.args.label:
            raise NonblockError('recovery source label '
                                + repr(manifest.get('label'))
                                + ' != ' + repr(self.args.label))
        if manifest.get('cases') != []:
            raise NonblockError('recovery source already holds samples; refusing: '
                                + repr(manifest.get('cases')))
        if manifest.get('completed') not in (0, None) \
                or manifest.get('failed') not in (0, None):
            raise NonblockError('recovery source manifest is not empty: completed='
                                + repr(manifest.get('completed')) + ' failed='
                                + repr(manifest.get('failed')))
        # manifest.cases == [] alone does not prove that no sample was started:
        # a half-written case directory can exist while the manifest still
        # reads empty.  Reject the source if its cases/ directory holds
        # anything.  This lists that one directory only (no tree walk).
        cases_dir = source / 'cases'
        if cases_dir.is_dir():
            try:
                started = sorted(entry.name for entry in cases_dir.iterdir())
            except OSError as exc:
                raise NonblockError('recovery source cases/ cannot be listed: '
                                    + repr(exc))
            if started:
                raise NonblockError(
                    'recovery source already has case directories (a case '
                    'started); refusing: ' + repr(started[:8]))
        if manifest.get('bit') is not None:
            try:
                have_mbit = pathlib.Path(str(manifest.get('bit'))).resolve()
            except Exception:
                have_mbit = None
            if have_mbit != want_bit:
                raise NonblockError('recovery source manifest BIT mismatch: '
                                    + repr(manifest.get('bit')))

        boot_id_before = self._recover_pre_flash_boot_id(source)
        self.boot_id_before = boot_id_before

        recovered_flash = dict(flash)
        recovered_flash['recovered_startup'] = True
        recovered_flash['recovery_source'] = str(source)
        recovered_flash['reused_from'] = str(flash_path)
        recovered_flash['reflashed'] = False
        recovered_flash['bit_sha256_reused'] = flash.get('bit_sha256')
        recovered_flash['manual_ctrl_c'] = True
        recovered_flash['manual_ctrl_c_note'] = RECOVER_MANUAL_CTRL_C_NOTE
        recovered_flash['recorded_at'] = utc_now()
        self._write_json('flash.json', recovered_flash)

        self.recovery = {
            'schema': 'nonblock-pilot-recovery-v1',
            'source_dir': str(source),
            'flash_reused': True,
            'reflashed': False,
            'bit_sha256_reused': flash.get('bit_sha256'),
            'manual_ctrl_c': True,
            'boot_id_before': boot_id_before,
            'boot_id_before_source': self._recovery_boot_id_source,
            'note': RECOVER_MANUAL_CTRL_C_NOTE,
        }
        self._write_json('boot-recovery.json', {
            'schema': 'nonblock-pilot-boot-recovery-v1',
            'recovered': True,
            'source_dir': str(source),
            'interrupt': 'manual CTRL-C (operator) during the previous run',
            'count': 1,
            'note': (RECOVER_MANUAL_CTRL_C_NOTE
                     + '; this run re-flashed nothing and re-ran no experiment'),
            'recovered_at': utc_now(),
        })
        self._log('recover-startup-from ' + str(source)
                  + ': flash reused, pre-flash boot_id=' + repr(boot_id_before)
                  + ' (' + self._recovery_boot_id_source + ')')
        return recovered_flash

    def _wait_for_root_shell(self, timeout: Optional[float] = None,
                             already_booted: bool = False,
                             recovered: bool = False) -> None:
        '''Wait for a usable root shell without blind post-flash writes.

        While the SD loader and the guest boot, the UART is left alone except
        for the two guest login answers: exactly one 'root' at the first
        'login:' prompt (username) and exactly one 'root' at the following
        'Password:' prompt.  Each answer is sent only for a prompt that has
        just arrived, never for a stale prompt already sitting in the tail, and
        there is no password guessing and no retry: at most two prompt answers
        are ever written.  No framed command is typed until a root prompt
        ('# ', '/ #', 'root@...#') is actually seen.  The root prompt is not
        newline terminated, so the reader buffer is drained and accumulated
        (never discarded).  Exactly one framed 'true' probe then confirms the
        shell answers.

        recovered=True: the BIT was already flashed by a previous run, so the
        board is already booted and its prompt was printed before this reader
        started; a single bare newline is then allowed to elicit the prompt.
        No Ctrl-C and no framed command is sent before the prompt is seen.

        interrupt_stalled_seed (fresh boot only): after a real 'Linux version'
        banner has been seen, a 'Saving random seed' line that is followed by
        NO further UART output for STALLED_SEED_SILENCE seconds gets exactly
        one CTRL-C, recorded in boot-recovery.json.  Progress is judged from
        the time of the last non-empty drain (not from when the marker was
        first seen), so a boot that keeps printing is never interrupted.  Once
        a login, Password or root prompt appears the seed interrupt is turned
        off, so a completed startup is never poked by the old marker, and the
        SD loader stage (no Linux banner) is never poked at all.
        '''
        if already_booted:
            # Optional pre-flash probe: the guest is already up, so one probe
            # is allowed without waiting for a prompt.
            self.console.disable_echo()
            budget = self.args.boot_timeout if timeout is None else timeout
            result = self.console.run_shell('true', timeout=budget)
            if result.exit_code != 0:
                raise NonblockError('pre-flash readiness probe exit='
                                    + str(result.exit_code))
            return

        total = self.args.boot_timeout if timeout is None else timeout
        deadline = time.monotonic() + total
        started = time.monotonic()
        tail = ''
        # Only newly arrived bytes are examined for the two guest login
        # prompts; a stale 'login:'/'Password:' left in the accumulated tail is
        # never answered twice.  prompt_pos is the offset already examined.
        prompt_buf = ''
        prompt_pos = 0
        login_sent = False
        password_sent = False
        newline_sent = False
        saw_linux = False
        seed_stall_armed = False
        last_output_at = started
        interrupt_sent = False
        while time.monotonic() < deadline:
            chunk = text_of(self.console.drain_input())
            if chunk:
                if 'Linux version ' in chunk:
                    saw_linux = True
                tail = (tail + chunk)[-8192:]
                combined = prompt_buf + chunk
                dropped = max(0, len(combined) - 4096)
                prompt_buf = combined[dropped:]
                prompt_pos = max(0, prompt_pos - dropped)
                last_output_at = time.monotonic()
            if ROOT_PROMPT_RE.search(tail):
                self.console.disable_echo()
                budget = max(1.0, min(5.0, deadline - time.monotonic()))
                try:
                    result = self.console.run_shell('true', timeout=budget)
                except Exception as exc:
                    raise NonblockError('root shell did not answer one framed '
                                        'probe: ' + repr(exc))
                if result.exit_code != 0:
                    raise NonblockError('root shell framed probe exit='
                                        + str(result.exit_code))
                return
            fresh = prompt_buf[prompt_pos:]
            match = LOGIN_PROMPT_RE.search(fresh)
            if not login_sent and match:
                self.console.write_line('root')
                login_sent = True
                prompt_pos += match.end()
                continue
            match = PASSWORD_PROMPT_RE.search(fresh)
            if not password_sent and match:
                self.console.write_line('root')
                password_sent = True
                prompt_pos += match.end()
                continue
            if recovered:
                # The recovered board printed its prompt before this reader
                # started, so elicit it once with a bare newline.  This is not
                # a command and is sent at most once.
                if (not login_sent and not newline_sent
                        and time.monotonic() - started >= RECOVER_PROMPT_ELICIT_DELAY):
                    self.console._write(bytes([10]))
                    newline_sent = True
            elif self.args.interrupt_stalled_seed and not (login_sent or password_sent):
                if 'Linux version ' in tail:
                    saw_linux = True
                if saw_linux and not interrupt_sent:
                    marker_pos = tail.rfind(STALLED_SEED_MARKER)
                    linux_pos = tail.rfind('Linux version ')
                    if marker_pos != -1 and marker_pos > linux_pos:
                        seed_stall_armed = True
                    if (seed_stall_armed
                            and time.monotonic() - last_output_at
                            >= STALLED_SEED_SILENCE):
                        self._send_stalled_seed_interrupt()
                        interrupt_sent = True
            time.sleep(0.2)
        raise NonblockError('root shell not reached within ' + str(total) + 's')

    def _send_stalled_seed_interrupt(self) -> None:
        '''Send exactly one CTRL-C to a boot that stalled after the Linux banner.

        Never sent during the SD loader stage; never repeated; never a blind
        command.  Only records the recovery and returns to the same wait loop.
        '''
        self._log('interrupt-stalled-seed: one CTRL-C after '
                  + repr(STALLED_SEED_MARKER))
        self.console._write(bytes([3]))
        self._write_json('boot-recovery.json', {
            'schema': 'nonblock-pilot-boot-recovery-v1',
            'recovered': False,
            'interrupt': 'CTRL-C',
            'count': 1,
            'source': ('fresh boot: Linux banner seen, then "'
                       + STALLED_SEED_MARKER + '" followed by no UART output for '
                       + str(STALLED_SEED_SILENCE) + 's'),
            'sd_loader_untouched': True,
            'sent_at': utc_now(),
            'note': ('the startup-script interruption is a host recovery '
                     'action, not re-sampling; the booted kernel is unchanged'),
        })

    def probe_boot_id_before_flash(self) -> None:
        '''Best effort only: never fatal, never a gate.'''
        try:
            self._wait_for_root_shell(timeout=PRE_FLASH_PROBE_TIMEOUT,
                                      already_booted=True)
            result = self.console.run_shell('cat ' + DEFAULT_BOOT_ID_PATH,
                                            timeout=self.args.command_timeout)
            if result.exit_code == 0:
                value = text_of(result.stdout).strip()
                if value:
                    self.boot_id_before = value
                    self._log('pre-flash boot_id=' + value)
        except Exception as exc:
            self._log('pre-flash boot_id probe skipped: ' + repr(exc))

    def collect_boot_identity(self) -> Dict[str, Any]:
        info: Dict[str, Any] = {'collected_at': utc_now()}

        result = self.console.run_shell('uname -r', timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise NonblockError('uname -r exit=' + str(result.exit_code))
        info['uname_r'] = text_of(result.stdout).strip()
        info['uname_expected'] = EXPECTED_UNAME_R
        if info['uname_r'] != EXPECTED_UNAME_R:
            raise NonblockError('uname -r ' + repr(info['uname_r'])
                                + ' != ' + repr(EXPECTED_UNAME_R))

        result = self.console.run_shell('cat ' + DEFAULT_BOOT_ID_PATH,
                                        timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise NonblockError('boot_id read exit=' + str(result.exit_code))
        info['boot_id'] = text_of(result.stdout).strip()
        if not info['boot_id']:
            raise NonblockError('boot_id is empty')
        if not BOOT_ID_RE.fullmatch(info['boot_id']):
            raise NonblockError('boot_id is not a UUID: ' + repr(info['boot_id']))

        result = self.console.run_shell('cat ' + DEFAULT_TAINT_PATH,
                                        timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise NonblockError('tainted read exit=' + str(result.exit_code))
        info['taint'] = text_of(result.stdout).strip()
        if info['taint'] != '0':
            raise NonblockError('kernel taint is not 0 at boot: ' + repr(info['taint']))

        info['boot_id_before'] = self.boot_id_before
        if self.boot_id_before and info['boot_id'] == self.boot_id_before:
            raise NonblockError('boot_id did not change after flashing the BIT: '
                                + repr(info['boot_id']))
        return info

    def check_caps(self) -> Dict[str, Any]:
        command = (shlex.quote(self.board_cli) + ' --device '
                   + shlex.quote(self.args.device) + ' --caps')
        result = self.console.run_shell(command, timeout=self.args.command_timeout)
        raw = text_of(result.stdout)
        if result.exit_code != 0:
            raise NonblockError('caps exit=' + str(result.exit_code)
                                + ' raw=' + repr(raw[:300]))
        try:
            parsed = json.loads(raw)
        except ValueError as exc:
            raise NonblockError('caps output is not JSON: ' + repr(raw[:300])
                                + ' (' + str(exc) + ')')
        problems = validate_caps(parsed, self.args.device)
        if problems:
            raise NonblockError('caps validation failed: ' + '; '.join(problems))
        return {'raw': raw, 'parsed': parsed, 'collected_at': utc_now()}

    def _check_dev_mem(self) -> None:
        '''The static helper reads MMIO through /dev/mem; require it present.'''
        result = self.console.run_shell('test -c ' + DEV_MEM_PATH,
                                        timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise NonblockError(DEV_MEM_PATH + ' is not a character device')

    def _reader_enabled(self) -> bool:
        return bool(self.args.reader) and self.args.label in READER_REQUIRED_LABELS

    def _reader_target(self) -> str:
        return self.board_root + '/nonblock-reader.elf'

    def _reader_command(self) -> str:
        # Assumed helper invocation; align with the ELF once it is compiled.
        command = shlex.quote(self._reader_target())
        if getattr(self.args, 'diagnostics', False):
            command += ' --diagnostics'
        return command

    def _upload_reader(self) -> Dict[str, Any]:
        '''Raw UART upload of the static helper into /tmp.

        The ELF is base64 encoded on the host and typed to the board as a shell
        here-document through the raw writer (run_shell forbids newlines).  The
        begin/end markers are built from shell parts so an echoed line can never
        be mistaken for a marker.

        The write timeout is raised only for this transfer and restored in a
        finally block; read_timeout is never changed.  The payload is paced one
        line at a time (UPLOAD_LINE_DELAY): 1024 base64 bytes cost about 89 ms
        of wire time at 115200 8N1, so without pacing the host write queue fills
        and pyserial raises SerialTimeoutException.  PS2 is cleared for the
        heredoc (to suppress the secondary prompt on every payload line) and
        restored afterwards.  The whole transfer is bounded by one
        --upload-timeout; the header is confirmed by reading the real begin
        marker before any payload byte is sent, and exactly one begin/end marker
        pair is used.  On any failure the transfer stops immediately (no retry,
        no resume, no partial helper run) and bounded metadata (bytes/lines
        attempted, phase, error) is written to upload-failure.json; the ELF and
        its base64 are never stored there.
        '''
        source = pathlib.Path(self.args.reader).resolve()
        if not source.is_file():
            raise NonblockError('--reader ELF not found: ' + str(source))
        data = source.read_bytes()
        encoded = base64.b64encode(data)
        lines = [encoded[i:i + UPLOAD_CHUNK].decode('ascii')
                 for i in range(0, len(encoded), UPLOAD_CHUNK)]
        target = self._reader_target()
        n1 = secrets.token_hex(8)
        n2 = secrets.token_hex(8)
        begin = 'NBB_' + n1 + '_' + n2 + '__'
        end = 'NBE_' + n1 + '_' + n2 + '__'
        delim = 'NBD_' + n1 + '_' + n2 + '__'
        nl = chr(92) + 'n'

        # Header through the heredoc opener.  The begin marker is printed before
        # the heredoc, so reading it proves the header ran before any payload
        # byte is sent.  PS2 is saved and cleared so the heredoc does not echo
        # '> ' for every payload line; it is restored once the upload finishes.
        header: List[str] = [
            'stty -echo 2>/dev/null',
            'mkdir -p ' + shlex.quote(self.board_root),
            '__nb_ps2="${PS2-}"; PS2=',
            'b0=NBB; e0=NBE; m1=' + n1 + '; m2=' + n2
            + '; bm="${b0}_${m1}_${m2}__"; em="${e0}_${m1}_${m2}__"',
            "printf '%s" + nl + "' " + _DQUOTE + '$bm' + _DQUOTE,
            '{ base64 -d > ' + shlex.quote(target) + '; __nb_rc=$?; printf '
            + "'%s %s" + nl + "' " + _DQUOTE + '$em' + _DQUOTE + ' '
            + _DQUOTE + '$__nb_rc' + _DQUOTE + '; } <<' + "'" + delim + "'",
        ]

        old_write_timeout: Optional[float] = None
        try:
            old_write_timeout = self.console._ser.write_timeout
            self.console._ser.write_timeout = UPLOAD_WRITE_TIMEOUT
        except Exception as exc:
            old_write_timeout = None
            self._log('reader upload: cannot raise write_timeout: ' + repr(exc))

        deadline = time.monotonic() + self.args.upload_timeout
        lines_attempted = 0
        bytes_attempted = 0
        phase = 'write-header'
        upload_error: Optional[BaseException] = None
        try:
            for line in header:
                self.console._write((line + chr(10)).encode('utf-8'))

            phase = 'await-begin'
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                raise NonblockError('reader upload header timed out')
            self.console.read_until(begin.encode('ascii'), remaining)

            phase = 'write-payload'
            for line in lines:
                remaining = deadline - time.monotonic()
                if remaining <= 0:
                    raise NonblockError(
                        'reader upload payload timed out after '
                        + str(lines_attempted) + ' lines')
                self.console._write((line + chr(10)).encode('utf-8'))
                lines_attempted += 1
                bytes_attempted += len(line)
                time.sleep(UPLOAD_LINE_DELAY)
            self.console._write((delim + chr(10)).encode('utf-8'))

            phase = 'await-end'
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                raise NonblockError('reader upload end marker timed out')
            self.console.read_until(end.encode('ascii'), remaining)

            phase = 'check-exit'
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                raise NonblockError('reader upload exit marker timed out')
            raw_rc = self.console.read_line(remaining)
            rc_text = text_of(raw_rc).strip()
            if not _INT_RE.fullmatch(rc_text):
                raise NonblockError(
                    'reader upload exit marker is not an integer: '
                    + repr(raw_rc))
            if int(rc_text) != 0:
                raise NonblockError('reader base64 decode exit=' + rc_text)

            phase = 'verify'
            check = self.console.run_shell(
                'PS2="${__nb_ps2-}"; chmod 700 ' + shlex.quote(target)
                + ' && test -s ' + shlex.quote(target),
                timeout=self.args.command_timeout)
            if check.exit_code != 0:
                raise NonblockError('reader chmod/verify exit='
                                    + str(check.exit_code))
        except Exception as exc:
            upload_error = exc
            failure = {
                'schema': 'nonblock-pilot-reader-upload-failure-v1',
                'reader_path': target,
                'reader_mode': READER_MODE_FOR_LABEL.get(self.args.label),
                'elf_bytes': len(data),
                'transfer_bytes': len(encoded),
                'chunk_bytes': UPLOAD_CHUNK,
                'chunk_count': len(lines),
                'lines_attempted': lines_attempted,
                'bytes_attempted': bytes_attempted,
                'phase': phase,
                'error': repr(exc),
                'auto_retry': False,
                'resumed': False,
                'failed_at': utc_now(),
            }
            try:
                self._write_json('upload-failure.json', failure)
            except OSError as write_exc:
                self._log('cannot write upload-failure.json: '
                          + repr(write_exc))
        finally:
            if old_write_timeout is not None:
                try:
                    self.console._ser.write_timeout = old_write_timeout
                except Exception as exc:
                    self._log('reader upload: cannot restore write_timeout: '
                              + repr(exc))

        if upload_error is not None:
            self._log('reader upload failed in phase ' + phase + ': '
                      + repr(upload_error) + ' lines=' + str(lines_attempted)
                      + ' bytes=' + str(bytes_attempted))
            raise NonblockError('reader upload failed: ' + repr(upload_error))

        record = {
            'schema': 'nonblock-pilot-reader-upload-v1',
            'reader_path': target,
            'reader_mode': READER_MODE_FOR_LABEL.get(self.args.label),
            'elf_bytes': len(data),
            'transfer_bytes': len(encoded),
            'chunk_bytes': UPLOAD_CHUNK,
            'chunk_count': len(lines),
            'mode': '700',
            'hash_readback': False,
            'uploaded_at': utc_now(),
        }
        self._write_json('reader-upload.json', record)
        self._log('reader uploaded: ' + target + ' bytes=' + str(len(encoded))
                  + ' mode=' + str(record['reader_mode']))
        return record

    def _run_helper_case(self, cid: str, board_dir: str, local_dir: pathlib.Path,
                         record: Any) -> Tuple[List[str], Optional[List[int]]]:
        '''Run the uploaded helper once and validate its frozen window.

        Returns (problems, full_window).  Reached only when the CLI exited 0 and
        the kernel is untainted, so MMIO is never read from a busy or failed run.
        '''
        problems: List[str] = []
        command = (self._reader_command() + ' > helper.json 2> helper.stderr.txt; '
                   'ec=$?; echo ' + _DQUOTE + '$ec' + _DQUOTE
                   + ' > helper.exit.txt')
        framed_exit: Optional[int] = None
        try:
            result = self.console.run_shell(command, timeout=self.args.case_timeout,
                                            cwd=board_dir, exit_var='ec')
            framed_exit = result.exit_code
        except Exception as exc:
            problems.append('helper command raised: ' + repr(exc))
        for name in HELPER_FILES:
            try:
                self._export_file(board_dir + '/' + name,
                                  local_dir / (name + '.b64'),
                                  local_dir / name)
            except Exception as exc:
                problems.append('export ' + name + ': ' + repr(exc))

        helper_exit: Optional[int] = None
        exit_path = local_dir / 'helper.exit.txt'
        if exit_path.is_file():
            value = exit_path.read_text(encoding='utf-8',
                                        errors='replace').strip()
            if _INT_RE.fullmatch(value):
                helper_exit = int(value)
        if helper_exit is None:
            problems.append('helper.exit.txt is missing or not an integer')
        elif helper_exit != 0:
            problems.append('helper exit ' + str(helper_exit))
        elif framed_exit is not None and framed_exit != helper_exit:
            problems.append('helper framed exit ' + str(framed_exit)
                            + ' != helper.exit.txt ' + str(helper_exit))

        window: Optional[List[int]] = None
        helper_doc: Any = None
        json_path = local_dir / 'helper.json'
        if json_path.is_file():
            try:
                helper_doc = json.loads(json_path.read_text(encoding='utf-8',
                                                            errors='replace'))
            except ValueError as exc:
                problems.append('helper.json is not valid JSON: ' + repr(exc))
        if isinstance(helper_doc, dict):
            abi = helper_doc.get('abi', helper_doc.get('schema'))
            if abi != READER_ABI:
                problems.append('helper ABI ' + repr(abi) + ' != ' + READER_ABI)
            candidate = helper_doc.get('words')
            if isinstance(candidate, list):
                window = candidate
        else:
            problems.append('helper.json is not a JSON object')

        expected_cycles: Any = None
        if isinstance(record, dict):
            p4m = record.get('p4m')
            if isinstance(p4m, dict):
                raw = p4m.get('raw_window')
                if isinstance(raw, list) and len(raw) > 1:
                    expected_cycles = raw[1]
        if expected_cycles is None:
            problems.append('no p4m.raw_window[1] for the helper cycles')

        if window is None:
            problems.append('helper window is missing')
        else:
            expect_v, expect_a = helper_mechanism_expectations(record)
            window_problems = validate_helper_window(
                window, self.args.label, expected_cycles, self.last_helper_seq,
                expect_v_movement=expect_v, expect_a=expect_a)
            problems.extend(window_problems)
            if not window_problems:
                self.last_helper_seq = window[4]
            self._write_json('cases/' + cid + '/helper-window.json', {
                'schema': 'nonblock-pilot-helper-window-v1',
                'id': cid,
                'label': self.args.label,
                'abi': READER_ABI,
                'mode': READER_MODE_FOR_LABEL.get(self.args.label),
                'window': window,
                'note': 'full frozen 16-word window saved verbatim; not HPM',
            })
            if getattr(self.args, 'diagnostics', False):
                diagnostic = (helper_doc.get('diagnostic')
                              if isinstance(helper_doc, dict) else None)
                problems.extend(validate_helper_diagnostic(
                    window, diagnostic, self.args.label))
                if isinstance(diagnostic, dict):
                    self._write_json('cases/' + cid + '/diag-window.json', {
                        'schema': 'nonblock-pilot-diag-window-v1',
                        'id': cid,
                        'label': self.args.label,
                        'abi': READER_ABI,
                        'mode': READER_MODE_FOR_LABEL.get(self.args.label),
                        'diagnostic': diagnostic,
                        'note': ('second 16-word diagnostic bank saved '
                                 'verbatim; passive only, not HPM'),
                    })
        return problems, window

    # ---- one case ----------------------------------------------------------

    def _collect_case_evidence(self, board_dir: str,
                               local_dir: pathlib.Path
                               ) -> Tuple[Dict[str, Any], List[str]]:
        '''Never raises: partial evidence is kept and reported.'''
        problems: List[str] = []
        raw: Dict[str, bytes] = {}
        for name in ('record.json', 'stderr.txt', 'exit.txt',
                     'taint.before', 'taint.after'):
            try:
                raw[name] = self._export_file(board_dir + '/' + name,
                                              local_dir / (name + '.b64'),
                                              local_dir / name)
            except Exception as exc:
                problems.append('export ' + name + ': ' + repr(exc))

        record: Any = None
        if 'record.json' in raw:
            try:
                record = json.loads(raw['record.json'].decode('utf-8', errors='replace'))
            except ValueError as exc:
                problems.append('record.json is not valid JSON: ' + repr(exc))

        exit_code: Optional[int] = None
        if 'exit.txt' in raw:
            value = raw['exit.txt'].decode('utf-8', errors='replace').strip()
            if _INT_RE.fullmatch(value):
                exit_code = int(value)

        evidence = {
            'record': record,
            'exit': exit_code,
            'taint_before': (raw['taint.before'].decode('utf-8', errors='replace').strip()
                             if 'taint.before' in raw else None),
            'taint_after': (raw['taint.after'].decode('utf-8', errors='replace').strip()
                            if 'taint.after' in raw else None),
        }
        return evidence, problems

    def run_one_case(self, case: Dict[str, Any]) -> Dict[str, Any]:
        cid = case['id']
        self.current_case = cid
        started_at = utc_now()
        argv = build_case_argv(case, self.args.device)
        cli_cmd = shlex.quote(self.board_cli) + ' ' + ' '.join(shlex.quote(a) for a in argv)
        board_dir = self._board_case_dir(cid)
        local_dir = self.out_dir / 'cases' / cid
        local_dir.mkdir(parents=True, exist_ok=False)
        self._log('case ' + cid + ': start')

        result = self.console.run_shell('mkdir -p ' + shlex.quote(board_dir),
                                        timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise NonblockError('case ' + cid + ': cannot create the board case dir')

        self._capture_taint(board_dir, 'taint.before')

        inner = (cli_cmd + ' > record.json 2> stderr.txt; ec=$?; echo '
                 + _DQUOTE + '$ec' + _DQUOTE + ' > exit.txt')

        framed_error: Optional[BaseException] = None
        framed_exit: Optional[int] = None
        try:
            result = self.console.run_shell(inner, timeout=self.args.case_timeout,
                                            cwd=board_dir, exit_var='ec')
            framed_exit = result.exit_code
        except Exception as exc:
            framed_error = exc

        capture_error: Optional[BaseException] = None
        try:
            self._capture_taint(board_dir, 'taint.after')
        except Exception as exc:
            capture_error = exc

        evidence, export_problems = self._collect_case_evidence(board_dir, local_dir)
        record = evidence['record']
        exit_code = evidence['exit']

        helper_problems: List[str] = []
        helper_window: Optional[List[int]] = None
        if self._reader_enabled() and exit_code == 0 \
                and evidence['taint_after'] == '0':
            # Only touch MMIO through the helper when the CLI exited 0 and the
            # kernel is untainted; never probe a busy or failed board.
            helper_problems, helper_window = self._run_helper_case(
                cid, board_dir, local_dir, record)

        problems: List[str] = list(export_problems) + helper_problems
        if framed_error is not None:
            problems.append('framed command raised: ' + repr(framed_error))
        if capture_error is not None:
            problems.append('taint after capture raised: ' + repr(capture_error))
        if exit_code is None:
            problems.append('exit.txt is missing or not an integer')
        elif exit_code != 0:
            problems.append('CLI exit ' + str(exit_code))
        elif framed_exit is not None and framed_exit != exit_code:
            problems.append('framed exit ' + str(framed_exit)
                            + ' != exit.txt ' + str(exit_code))
        if evidence['taint_after'] != '0':
            problems.append('taint after=' + repr(evidence['taint_after']))
        if isinstance(record, dict):
            problems.extend(validate_record(case, record))
        else:
            problems.append('record.json is not a JSON object')

        metrics = case_metrics(case, record) if isinstance(record, dict) else None
        if metrics is not None:
            self._write_json('cases/' + cid + '/metrics.json', metrics)

        case_document = {
            'schema': 'nonblock-pilot-case-v1',
            'id': cid,
            'group': case['group'],
            'condition': case['condition'],
            'round': case['round'],
            'transfers': case['transfers'],
            'warmup': case['warmup'],
            'argv': argv,
            'command': cli_cmd,
            'board_dir': board_dir,
            'framed_exit': framed_exit,
            'exit': exit_code,
            'taint_before': evidence['taint_before'],
            'taint_after': evidence['taint_after'],
            'started_at': started_at,
            'finished_at': utc_now(),
            'problems': problems,
            'metrics': metrics,
            'helper_window': helper_window,
            'helper_problems': helper_problems,
        }
        self._write_json('cases/' + cid + '/case.json', case_document)

        self.results.append({
            'id': cid,
            'group': case['group'],
            'condition': case['condition'],
            'round': case['round'],
            'status': 'completed' if not problems else 'failed',
            'exit': exit_code,
            'taint_after': evidence['taint_after'],
            'dir': 'cases/' + cid,
            'v_window_cycles': (metrics or {}).get('v_window_cycles'),
            'a_completed_jobs_window': (metrics or {}).get('a_completed_jobs_window'),
            'problems': problems,
            'helper_window': helper_window,
        })
        self._write_manifest()

        if problems:
            raise NonblockError('case ' + cid + ': ' + '; '.join(problems))
        self._log('case ' + cid + ': completed')
        return case_document

    # ---- manifest / summary ------------------------------------------------

    def _write_manifest(self) -> None:
        completed = sum(1 for entry in self.results if entry['status'] == 'completed')
        failed = sum(1 for entry in self.results if entry['status'] == 'failed')
        self._write_json('manifest.json', {
            'schema': 'nonblock-pilot-manifest-v1',
            'run_id': self.run_id,
            'label': self.args.label,
            'bit': str(pathlib.Path(self.args.bit).resolve()),
            'device': self.args.device,
            'board_root': self.board_root,
            'started_at': self.started_at,
            'updated_at': utc_now(),
            'planned': len(self.case_plan),
            'completed': completed,
            'failed': failed,
            'remaining': len(self.case_plan) - len(self.results),
            'recovered_startup': bool(self.args.recover_startup_from),
            'recovery_source': (str(pathlib.Path(self.args.recover_startup_from).resolve())
                                if self.args.recover_startup_from else None),
            'cases': self.results,
        })

    def _write_summary(self, status: str) -> None:
        warnings_found: List[str] = []
        end_path = self.out_dir / 'dmesg.end.txt'
        if end_path.is_file():
            try:
                content = end_path.read_text(encoding='utf-8', errors='replace')
            except OSError:
                content = ''
            for line in content.splitlines():
                if any(pattern.search(line) for pattern in WARNING_PATTERNS):
                    warnings_found.append(line.strip())
        notes = [
            'metrics are limited to the P4M window cycles and the A jobs '
            'completed inside the window',
            'raw HPM counters are retained per case but are not a validated '
            'metric under this nonblock design',
            'no selection, no tuning, no retry, no resume',
        ]
        if self.args.recover_startup_from:
            notes.append(
                'recovered_startup: reusing the flash record of '
                + str(pathlib.Path(self.args.recover_startup_from).resolve())
                + '; no BIT was re-flashed, no BIT was re-hashed and no '
                'experiment was re-run before this run')
        if (self.args.recover_startup_from
                or (self.out_dir / 'boot-recovery.json').is_file()):
            notes.append(
                'startup interruption limitation: the Buildroot startup was '
                'unblocked by a single CTRL-C at "Saving random seed"; this is '
                'a host-side recovery action, not re-sampling, and the booted '
                'kernel is unchanged.  The exact stalled sub-step is not '
                'asserted (it is not claimed to be an entropy shortage)')
        self._write_json('summary.json', {
            'schema': 'nonblock-pilot-summary-v1',
            'run_id': self.run_id,
            'label': self.args.label,
            'status': status,
            'finished_at': utc_now(),
            'planned': len(self.case_plan),
            'completed': sum(1 for e in self.results if e['status'] == 'completed'),
            'failed': sum(1 for e in self.results if e['status'] == 'failed'),
            'cases': self.results,
            'unexpected_warnings': warnings_found,
            'notes': notes,
        })

    def _finalize(self) -> None:
        try:
            self._capture_dmesg('dmesg.end.txt')
        except Exception as exc:
            self._log('dmesg end capture failed: ' + repr(exc))

    # ---- top level ---------------------------------------------------------

    def run(self) -> int:
        self._write_json('plan.json', {
            'schema': 'nonblock-pilot-plan-v1',
            'label': self.args.label,
            'device': self.args.device,
            'cases': self.case_plan,
        })
        self._write_manifest()
        recovered = bool(self.args.recover_startup_from)
        try:
            if recovered:
                self.recover_startup()
            else:
                self.flash_once()
            self._wait_for_root_shell(recovered=recovered)
            identity = self.collect_boot_identity()
            if recovered:
                identity['recovered_startup'] = self.recovery
            self._write_json('boot.json', identity)
            self._log('boot uname=' + repr(identity['uname_r'])
                      + ' boot_id=' + repr(identity['boot_id'])
                      + (' recovered_startup' if recovered else ''))
            caps = self.check_caps()
            self._write_json('caps.json', caps)
            if self._reader_enabled():
                self._check_dev_mem()
                self._upload_reader()
            self._capture_dmesg('dmesg.baseline.txt')
            for case in self.case_plan:
                self.run_one_case(case)
            self._finalize()
            self._write_summary('complete')
            return 0
        except KeyboardInterrupt:
            self._stop('KeyboardInterrupt')
            self._finalize()
            self._write_summary('interrupted')
            raise
        except Exception as exc:
            self._stop(type(exc).__name__ + ': ' + str(exc))
            self._finalize()
            self._write_summary('stopped')
            raise
        finally:
            try:
                self.console.close()          # release the UART, leave Linux running
            except Exception:
                pass


# --------------------------------------------------------------------------
# CLI
# --------------------------------------------------------------------------

def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description='Mac host runner for the frozen nonblock pilot '
                    '(one BIT, one fresh boot, fifteen cases)')
    parser.add_argument('--label', required=True, choices=LABELS,
                        help='which BIT this run belongs to')
    parser.add_argument('--bit', required=True,
                        help='BIT flashed exactly once at the start of this run')
    parser.add_argument('--out', required=True,
                        help='output directory; must not exist yet')
    parser.add_argument('--serial', default=DEFAULT_SERIAL)
    parser.add_argument('--openfpgaloader', default=DEFAULT_OPENFPGALOADER)
    parser.add_argument('--board-cli', default=DEFAULT_BOARD_CLI)
    parser.add_argument('--device', default=DEFAULT_DEVICE)
    parser.add_argument('--boot-timeout', type=float, default=DEFAULT_BOOT_TIMEOUT)
    parser.add_argument('--command-timeout', type=float, default=DEFAULT_COMMAND_TIMEOUT)
    parser.add_argument('--case-timeout', type=float, default=DEFAULT_CASE_TIMEOUT)
    parser.add_argument('--export-timeout', type=float, default=DEFAULT_EXPORT_TIMEOUT)
    parser.add_argument('--flash-timeout', type=float, default=DEFAULT_FLASH_TIMEOUT)
    parser.add_argument('--jtag-freq', type=int, default=DEFAULT_JTAG_FREQ)
    parser.add_argument('--reader', default=None,
                        help='static RISC-V ELF helper; required for '
                             'banked-serial/banked-hit, forbidden for original-serial')
    parser.add_argument('--diagnostics', action='store_true',
                        help='read the optional second 16-word diagnostic bank '
                             'through the uploaded reader; requires a banked '
                             'label and --reader')
    parser.add_argument('--upload-timeout', type=float, default=DEFAULT_UPLOAD_TIMEOUT)
    parser.add_argument('--pre-flash-boot-id', action='store_true',
                        help='best effort: read the running boot_id over the UART '
                             'before flashing, so the new boot_id can be shown to differ')
    parser.add_argument('--recover-startup-from', default=None,
                        help='reuse the flash record of a previously stopped '
                             'run with the same BIT/label and no samples; '
                             'never re-flashes and never re-hashes the BIT')
    parser.add_argument('--interrupt-stalled-seed', action='store_true',
                        help='optional: after a real fresh Linux banner, send '
                             'exactly one CTRL-C if the boot stalls at '
                             '"Saving random seed" with no root prompt for '
                             '30s (default off; never used at the SD loader)')
    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = build_arg_parser().parse_args(argv)
    out_dir = pathlib.Path(args.out)
    if out_dir.exists():
        raise SystemExit('--out already exists: ' + str(out_dir))
    if not pathlib.Path(args.bit).is_file():
        raise SystemExit('--bit not found: ' + args.bit)
    if not pathlib.Path(args.openfpgaloader).is_file():
        raise SystemExit('--openfpgaloader not found: ' + args.openfpgaloader)
    if args.label in READER_REQUIRED_LABELS and not args.reader:
        raise SystemExit('--reader is required for label ' + args.label)
    if args.reader:
        if args.label in READER_FORBIDDEN_LABELS:
            raise SystemExit('--reader is forbidden for label ' + args.label
                             + ' (no RO hardware on that bitstream)')
        if not pathlib.Path(args.reader).is_file():
            raise SystemExit('--reader not found: ' + args.reader)
    if getattr(args, 'diagnostics', False):
        if args.label not in READER_REQUIRED_LABELS:
            raise SystemExit('--diagnostics requires a banked label with RO '
                             'hardware: ' + ', '.join(READER_REQUIRED_LABELS))
        if not args.reader:
            raise SystemExit('--diagnostics requires --reader')
    if args.recover_startup_from:
        if args.interrupt_stalled_seed:
            raise SystemExit('--interrupt-stalled-seed does not apply with '
                             '--recover-startup-from (no fresh boot is started)')
        if not pathlib.Path(args.recover_startup_from).is_dir():
            raise SystemExit('--recover-startup-from is not a directory: '
                             + args.recover_startup_from)

    check_uart_ownership(args.serial)                 # read-only, never kills
    console_class = make_pilot_console_class(
        pick_console_class(load_mac_research2()))
    out_dir.mkdir(parents=True, exist_ok=False)
    console = console_class(args.serial, out_dir / 'console.raw.log',
                            out_dir / 'console.events.log')
    console.start()                                   # reader starts before JTAG

    runner = PilotRunner(args, console, out_dir)
    if args.pre_flash_boot_id:
        try:
            runner.probe_boot_id_before_flash()
        except BaseException:
            # A failed pre-flash probe must release the UART as well.
            try:
                console.close()
            except Exception:
                pass
            raise
    return runner.run()


if __name__ == '__main__':
    sys.exit(main())
