# Proposal — two complete files, no execution claims

Repo evidence used (line-cited):

- `scripts/mac_research2.py:335-519` — the reusable framed serial console (`SerialConsole`: `start`, `_reader_loop` writing the raw log, `close`, `_write`, `disable_echo`, `drain_input`, `read_until`, `read_line`, `run_shell`). This is the class actually present in the supplied file; the task's names `SerialCapture` / `SerialShellConsole` do **not** appear in it (see Assumptions A1).
- `scripts/mac_research2.py:522-545` — the existing `genesys2` programmer command shape (`openFPGALoader -b genesys2 --freq <f> <bit>`).
- `scripts/mac_research2.py:686-707` — the by-file module loader pattern that keeps `scripts/` off `sys.path` (the `platform.py` shadowing defence).
- `bench/idma_experiment.c:16-172` — option surface and argument validation (`--p4m` implies `--batch`; Solo P4M requires `--a-gap-cycles 0`; batch requires `chunk_bytes == 4096`; timeout/pages/transfers must be positive).
- `bench/idma_experiment.c:272-414` — the authoritative P4M acceptance check (`validate_p4m`) that the CLI already applies before it can report `success: true`.
- `bench/idma_experiment.c:432-496` — `print_p4m` output field names (`cycles`, `hardware_window_ns`, `raw_window` 16 words, `raw_observation` 35 words, `has_a`, `closed`, `conservation`, `a_completed_jobs_window`, `bank_magic`).
- `bench/idma_experiment.c:498-616` — the top-level record schema (`schema`/`success`/`stage`/`error`/`errno`/`input`/`capabilities`/`result`/`p4m`/`semantics`), and that `result.status` and `result.*.data_errors/quarantined` are printed.
- `out/n-prototype-20261004/linux-gap-answer.md:63-89` — the exact CLI forms and the `--caps` requirement of `CAP_DID_HPM|CAP_P4M|CAP_HW_BATCH`.

---

## File 1 (complete code): `scripts/mac_nonblock_pilot.py`

```python
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
DEFAULT_SERIAL = 'cu.usbserial-AV0KRCHT'
DEFAULT_OPENFPGALOADER = '/opt/homebrew/bin/openFPGALoader'
DEFAULT_BOOT_ID_PATH = '/proc/sys/kernel/random/boot_id'
DEFAULT_TAINT_PATH = '/proc/sys/kernel/tainted'
DEFAULT_BAUDRATE = 115200
DEFAULT_JTAG_FREQ = 1000000

DEFAULT_BOOT_TIMEOUT = 300.0
DEFAULT_COMMAND_TIMEOUT = 60.0
DEFAULT_CASE_TIMEOUT = 600.0
DEFAULT_EXPORT_TIMEOUT = 120.0
DEFAULT_FLASH_TIMEOUT = 180.0
PRE_FLASH_PROBE_TIMEOUT = 20.0

DEFAULT_CHUNK = 4096
DEFAULT_TIMEOUT_MS = 60000
MAX_AGGRESSOR_PAGES = 16
MAX_EXPORT_BYTES = 8 * 1024 * 1024

LABELS = ('original-serial', 'banked-serial', 'banked-hit')

P4M_SCHEMA = 'idma.p4m.v1'
CAPS_SCHEMA = 'idma.experiment.v1'
P4M_BANK_MAGIC = 0x49444D4150344D01
P4M_WINDOW_WORDS = 16
P4M_OBS_WORDS = 35
P4M_OBSERVER_STATUS_OK = 4

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
    if caps_block.get('abi_version') != inp.get('abi_version'):
        problems.append('caps abi_version does not match input abi_version')
    if caps_block.get('struct_size') != inp.get('struct_size'):
        problems.append('caps struct_size does not match input struct_size')
    # CAP_P4M / CAP_HW_BATCH are not printed as booleans by --caps; the per-case
    # CLI enforces them and exits non-zero when they are missing.  See doc A3.
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
        self.started_at = utc_now()
        self.stop_written = False

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

    def _wait_for_root_shell(self, timeout: Optional[float] = None) -> None:
        '''Wait for a usable root shell, answering a login prompt if one appears.'''
        total = self.args.boot_timeout if timeout is None else timeout
        deadline = time.monotonic() + total
        last: Optional[str] = None
        self.console.disable_echo()
        while time.monotonic() < deadline:
            tail = text_of(self.console.drain_input()).lower()
            if tail:
                if 'password:' in tail:
                    self.console.write_line('')          # root has no password
                    self.console.disable_echo()
                elif tail.rstrip().endswith('login:'):
                    self.console.write_line('root')
                    self.console.disable_echo()
            budget = max(1.0, min(5.0, deadline - time.monotonic()))
            try:
                result = self.console.run_shell('true', timeout=budget)
                if result.exit_code == 0:
                    return
                last = 'readiness probe exit=' + str(result.exit_code)
            except Exception as exc:
                last = repr(exc)
            time.sleep(0.5)
        raise NonblockError('root shell not reached within ' + str(total)
                            + 's: ' + str(last))

    def probe_boot_id_before_flash(self) -> None:
        '''Best effort only: never fatal, never a gate.'''
        try:
            self._wait_for_root_shell(timeout=PRE_FLASH_PROBE_TIMEOUT)
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

        result = self.console.run_shell('cat ' + DEFAULT_BOOT_ID_PATH,
                                        timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise NonblockError('boot_id read exit=' + str(result.exit_code))
        info['boot_id'] = text_of(result.stdout).strip()
        if not info['boot_id']:
            raise NonblockError('boot_id is empty')

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

        problems: List[str] = list(export_problems)
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
            'notes': [
                'metrics are limited to the P4M window cycles and the A jobs '
                'completed inside the window',
                'raw HPM counters are retained per case but are not a validated '
                'metric under this nonblock design',
                'no selection, no tuning, no retry, no resume',
            ],
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
        try:
            self.flash_once()
            self._wait_for_root_shell()
            identity = self.collect_boot_identity()
            self._write_json('boot.json', identity)
            self._log('boot uname=' + repr(identity['uname_r'])
                      + ' boot_id=' + repr(identity['boot_id']))
            caps = self.check_caps()
            self._write_json('caps.json', caps)
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
    parser.add_argument('--pre-flash-boot-id', action='store_true',
                        help='best effort: read the running boot_id over the UART '
                             'before flashing, so the new boot_id can be shown to differ')
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

    check_uart_ownership(args.serial)                 # read-only, never kills
    console_class = make_pilot_console_class(
        pick_console_class(load_mac_research2()))
    out_dir.mkdir(parents=True, exist_ok=False)
    console = console_class(args.serial, out_dir / 'console.raw.log',
                            out_dir / 'console.events.log')
    console.start()                                   # reader starts before JTAG

    runner = PilotRunner(args, console, out_dir)
    if args.pre_flash_boot_id:
        runner.probe_boot_id_before_flash()
    return runner.run()


if __name__ == '__main__':
    sys.exit(main())
```

---

## File 2 (complete Markdown): `docs/nonblock-pilot-plan-20261004.md`

```markdown
# Nonblock engineering pilot — frozen run plan (2026-10-04)

Status: **proposal, not executed.** No result, throughput or novelty claim is
made here. Nothing in this document has been validated on hardware yet.

Runner: `scripts/mac_nonblock_pilot.py`.

---

## 1. Scope

A single, fully frozen engineering pilot that answers one question only:

> under a fresh boot of one BIT, what do the P4M hardware window cycle count and
> the number of A jobs completed inside that window look like across a fixed
> Solo / A1 / A8 / A16 footprint ladder?

Everything else (selection, tuning, calibration, statistics, novelty) is out of
scope and must not be produced from this data until the results exist.

Three labels are planned, one runner invocation each, one BIT flashed per
invocation because a fresh boot resets the policy:

| label             | role of the BIT                          | legacy cost run |
|-------------------|------------------------------------------|-----------------|
| `original-serial` | original serial reference                | no              |
| `banked-serial`   | banked serial reference                  | no              |
| `banked-hit`      | banked target                            | no              |

The original role-2 legacy cost run is **not** part of this pilot.

Firmware is already the SD kernel `00093`. The runner never writes the SD card
and never copies firmware.

---

## 2. Frozen case plan (15 cases)

Every case is one `idma_experiment --p4m` run with

* `--chunk 4096`
* `--victim-pages 1` (victim DMA instance 0, `/dev/idma0`)
* `--a-gap-cycles 0`
* `--timeout-ms 60000`
* aggressor `none` (Solo) or DMA instance `1` with 1/8/16 pages

### 2.1 Short group — `transfers=64`, `warmup=16`

| # | case id      | condition | aggressor       |
|---|--------------|-----------|-----------------|
| 1 | `short-solo` | Solo      | none            |
| 2 | `short-a1`   | A1        | id 1, pages 1   |
| 3 | `short-a8`   | A8        | id 1, pages 8   |

### 2.2 Long group — `transfers=4096`, `warmup=256`, three rounds

Condition order is fixed per round and is never re-selected:

| round | order                     |
|-------|---------------------------|
| 1     | Solo, A1, A8, A16 (base)  |
| 2     | A16, A8, A1, Solo (reverse) |
| 3     | Solo, A1, A8, A16 (base)  |

| # | case id         | round | condition | aggressor       |
|---|-----------------|-------|-----------|-----------------|
| 4 | `long-r1-solo`  | 1     | Solo      | none            |
| 5 | `long-r1-a1`    | 1     | A1        | id 1, pages 1   |
| 6 | `long-r1-a8`    | 1     | A8        | id 1, pages 8   |
| 7 | `long-r1-a16`   | 1     | A16       | id 1, pages 16  |
| 8 | `long-r2-a16`   | 2     | A16       | id 1, pages 16  |
| 9 | `long-r2-a8`    | 2     | A8        | id 1, pages 8   |
|10 | `long-r2-a1`    | 2     | A1        | id 1, pages 1   |
|11 | `long-r2-solo`  | 2     | Solo      | none            |
|12 | `long-r3-solo`  | 3     | Solo      | none            |
|13 | `long-r3-a1`    | 3     | A1        | id 1, pages 1   |
|14 | `long-r3-a8`    | 3     | A8        | id 1, pages 8   |
|15 | `long-r3-a16`   | 3     | A16       | id 1, pages 16  |

Total = 3 + 12 = **15 cases**, executed in the table order, once each.

Exact CLI form per case:

```
/usr/bin/idma_experiment --device /dev/idma0 --p4m \
  --aggressor none|1 --a-gap-cycles 0 \
  --victim-pages 1 --aggressor-pages 1|8|16 \
  --chunk 4096 --transfers 64|4096 --warmup 16|256 --timeout-ms 60000
```

These constraints follow `bench/idma_experiment.c:106-166` (Solo P4M requires
`--a-gap-cycles 0`; `--p4m` implies batch; batch requires `chunk_bytes == 4096`).

---

## 3. Run sequence (per invocation, one BIT)

1. Refuse to start if `--out` exists, if `--bit`/`--openfpgaloader` are missing,
   or if the UART is already held (`lsof`, read-only, **never kills another
   process**).
2. Create the output directory.
3. **Start the UART reader before JTAG.** The reader thread writes the complete,
   unfiltered UART stream to `console.raw.log`.
4. Optional only (`--pre-flash-boot-id`): best-effort login + read of
   `/proc/sys/kernel/random/boot_id` from the currently running Linux. Never
   fatal, never a gate.
5. Flash the BIT **exactly once** with
   `openFPGALoader -b genesys2 --freq 1000000 <bit>`
   (same command shape as the existing `genesys2` programmer).
6. Wait for a Linux shell. If a `login:` prompt appears, answer `root`; if a
   `Password:` prompt appears, answer empty (root has no password). Once a
   shell answers a framed `true`, continue.
7. Collect the **actual** running identity: `uname -r`, `boot_id`, taint.
   Fail if taint is not `0`. If a pre-flash `boot_id` was captured, fail when
   the new one is identical.
8. Capability gate: `/usr/bin/idma_experiment --device /dev/idma0 --caps`.
9. Capture `dmesg.baseline.txt`.
10. Run the fifteen cases in order. After each case, export evidence, write
    `cases/<id>/case.json`, and rewrite the incremental `manifest.json`.
11. Capture `dmesg.end.txt`; write `summary.json`.
12. Release the UART and **leave Linux running**. No reboot, no poweroff.

---

## 4. Evidence layout

```
<out>/
  plan.json                     frozen 15-case plan
  manifest.json                 incremental, rewritten after every case
  summary.json                  final counts + unexpected-warning flag
  run.log
  console.raw.log               complete, unfiltered UART stream
  console.events.log
  flash.json                    one openFPGALoader invocation + BIT sha256
  boot.json                     uname, boot_id, boot_id_before, taint
  caps.json                     raw + parsed --caps output
  dmesg.baseline.txt
  dmesg.end.txt
  cases/<case-id>/
    record.json / record.b64    CLI stdout (JSON) and its raw base64 stream
    stderr.txt  / stderr.b64
    exit.txt    / exit.b64
    taint.before / taint.before.b64
    taint.after  / taint.after.b64
    metrics.json                two window numbers only
    case.json                   per-case metadata + problems list
  STOPPED.txt                   only on a technical failure
```

Board-side per-case raw files persist under a unique
`/tmp/nonblock-<run_id>/cases/<case-id>/` and are **never deleted** by the
runner.

Transfer is one framed `base64 <file>` per file. The raw base64 stream is
written to disk *before* decoding, so a decode failure still leaves the raw
bytes on the host. Output is bounded by `MAX_EXPORT_BYTES` (8 MiB).

---

## 5. Per-case validation

The CLI already runs the authoritative acceptance test
(`bench/idma_experiment.c:272-414`, `:683-703`). The runner adds only:

* `exit.txt == 0`, and `framed exit == exit.txt`
* kernel taint after the case is `0`
* record schema `idma.p4m.v1`, `success == true`, `error` empty
* `result.status == 0`
* `result.victim.data_errors == 0` and `quarantined == 0`
  (same for `result.aggressor`, which must be `null` for Solo)
* `p4m.bank_magic == 0x49444d4150344d01`, `p4m.observer_status == 4`,
  `p4m.closed == true`, `p4m.cycles > 0`, `p4m.hardware_window_ns` present
* `len(p4m.raw_window) == 16`, `len(p4m.raw_observation) == 35`
* `raw_window[1] == raw_observation[0]`
* `raw_window[13] == raw_window[14] == raw_observation[7] == raw_observation[8]
  == transfers`
* `p4m.has_a` matches the condition (false for Solo, true otherwise)

Nothing else is verified. There is no CLI hashing, no pack/unpack, no readback,
no tar archive, no archive-hash or stage-save-hash step.

---

## 6. Metrics

Metrics are deliberately narrow:

| field                     | source                  |
|---------------------------|-------------------------|
| `v_window_cycles`         | `p4m.cycles` (window[1]) |
| `a_completed_jobs_window` | `p4m.a_completed_jobs_window` (window[3]) |

Raw HPM counters (`result.victim.iotlb_miss`, `s1_ptw`, `s2_ptw`, …) stay inside
`record.json` and are explicitly marked not validated and incomplete under the
nonblock design. They are never promoted into the metric set, and no old
N-miss / HPM aggregate is synthesised.

No rates, no bytes/second, no statistics, no novelty claim are produced here.

---

## 7. Failure policy

* Any technical failure **stops the run at that case**. There is no automatic
  retry, no resume and no rerun.
* All evidence gathered for the failing case is written before the stop
  (`case.json`, exported files, `manifest.json`, `STOPPED.txt`).
* The short Solo case is known to be able to surface a bug in the earlier run
  summary. Stopping on a technical error is the correct behaviour here — a
  failing Solo case must never be silently skipped.
* Unexpected dmesg warnings are preserved as a flag in `summary.json`, not
  dropped and not silently treated as noise.

---

## 8. Assumptions and unresolved dependencies

* **A1.** `mac_research2.py` as supplied exposes `SerialConsole`, not
  `SerialCapture` / `SerialShellConsole`. The runner accepts any of the three
  names but requires the interface `start / close / run_shell / read_line /
  drain_input`. It adds one tiny subclass method `write_line` to answer the
  login prompt. If the real `mac_research2.py` exposes a differently-shaped
  class under another name, this must be reconciled before use.
* **A2.** `A1` / `A8` / `A16` are read as **aggressor page counts** with the
  aggressor DMA instance fixed at `1` (only instance `1` exists when the victim
  is instance `0`). If they were meant as DMA instance ids, `CONDITIONS` in the
  runner is the single place to change and the plan must be revised.
* **A3.** `linux/include/uapi/linux/idma.h` was not provided, so the
  `CAP_P4M` / `CAP_HW_BATCH` bit positions in `capabilities.flags` are unknown.
  The `--caps` gate therefore checks the printed booleans (`did_hpm_supported`,
  `page_bytes`, `max_pages`, `max_run_timeout_ms`, ABI struct match) and relies
  on the per-case CLI (`bench/idma_experiment.c:179-181`) to enforce
  `CAP_DID_HPM | CAP_P4M | CAP_HW_BATCH`.
* **A4.** `--bit` and `--label` are required. If a future flow reuses an
  already-flashed board, `--bit` would have to become optional, which
  contradicts the fresh-boot-per-BIT policy this pilot relies on.
* **A5.** The pre-flash `boot_id` probe is opt-in and best-effort. It sends a
  framed probe before JTAG, which presupposes a responsive terminal; it is
  never a gate.
* **A6.** `console.raw.log` is the complete UART record of the run, including
  everything before, during and after flashing. It is never truncated.

---

## 9. Acceptance checks for the parent

Run these against the produced artefacts after the first real invocation:

1. `scripts/mac_nonblock_pilot.py` compiles (`python3 -m py_compile`) and
   `--help` lists `--label`, `--bit`, `--out`, `--serial`,
   `--openfpgaloader`, `--device`, `--pre-flash-boot-id`.
2. `plan.json` contains **exactly** the 15 case ids from §2 in plan order, and
   `manifest.json` reports `planned: 15`.
3. `flash.json` exists exactly once per run, with `returncode == 0`.
4. `boot.json` has `taint == "0"` and, when a pre-flash id was captured,
   `boot_id != boot_id_before`.
5. `caps.json` `parsed.success == true` and
   `parsed.capabilities.did_hpm_supported == true`.
6. For every completed case, `cases/<id>/case.json` has `problems == []`,
   `exit == 0`, `taint_after == "0"`, and a `metrics.json` containing only
   `v_window_cycles` and `a_completed_jobs_window` as numeric window values.
7. `summary.json` `completed == 15` and `failed == 0` on a clean run, and
   `unexpected_warnings` is either `[]` or a non-empty list of preserved lines.
8. On a forced technical failure, `STOPPED.txt` exists, `summary.json.status`
   is `"stopped"`, and every artefact for the failing case is present.
9. After the run the board is still at a Linux shell and the UART is released
   (`lsof /dev/<serial>` empty).
10. `console.raw.log` is non-empty and spans the flash boundary.
```

---

## Notes for the parent (verification, not execution claims)

- I did not compile or run anything; every statement above is a proposal.
- The one reuse the runner depends on is the framing contract of
  `SerialConsole.run_shell` (`scripts/mac_research2.py:478-519`): it returns
  `ShellResult(stdout, exit_code)` and raises on marker timeout. If the real
  class differs, only `_export_file`, `_capture_taint`, `_capture_dmesg`,
  `_wait_for_root_shell` and `run_one_case` need adjusting.
- The reused class's `_reader_loop` writes every chunk to the raw log
  (`mac_research2.py:374-390`), which is what makes `console.raw.log` the
  complete UART record; the runner never filters it.
- Deliberately **not** used from `mac_research2.py`: `load_manifest`,
  `verify_manifest_package_files`, `_freeze_selection`, `_verify_stage_saved`,
  `_collect_and_transfer`, `safe_extract_tar_gz`, `check_caps_for_role`,
  `parse_caps` — none of the old manifest / role-2 capability / stage-hash /
  archive flows are reachable from this runner.
- Assumption A2 (A1/A8/A16 = aggressor pages, not DMA instance ids) and A3
  (`--caps` P4M/HW_BATCH bit positions unknown) are the two items most likely to
  need a decision before the first real run.