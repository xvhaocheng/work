#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
mac_research2.py - Mac host sequential two-BIT research2 runner.

Design constraints:
- read-only preflight
- one openFPGALoader invocation per BIT, no automatic retry
- the serial reader starts before the JTAG flash
- every board command is framed by a begin/end marker pair that is built from
  shell parts, so a terminal echo of the command line can never be mistaken for
  the marker itself, and the command echo/prompt/chatter is stripped
- every case is gzip+base64 transferred to the Mac and SHA256-verified
- the first stage is fully saved and hash-verified before the second BIT flashes
- a technical failure stops immediately; a scientific negative is not a failure
- no automatic rerun, no resume, no mutation of the original files
- only fixed safe case ids; no eval injection
'''
from __future__ import annotations

import argparse
import base64
import hashlib
import importlib.util
import io
import json
import pathlib
import re
import secrets
import shlex
import shutil
import subprocess
import sys
import tarfile
import threading
import time
from typing import Any, Dict, List, Optional, Sequence, Tuple

SCHEMA = 'research2-delivery-v1'
RUN_SCHEMA = 'research2-run-v1'
METADATA_SCHEMA = 'research2-case-metadata-v1'
SELECTION_AUDIT_SCHEMA = 'research2-selection-audit-v1'
PLAN_HASH_SCHEMA = 'research2-plan-hash-v1'
BOARD = 'genesys2'

# Research2 capability block: linux/include/uapi/linux/idma.h and the
# bench/idma_research.c --caps2 JSON printer.
IDMA2CAP_MAGIC = 0x49444D4132434150
OBSERVER_MAGIC = 0x49444D414F425308
CAPS_SCHEMA = 'idma-research2-caps-v1'
CAPS_VERSION = 1
# bit0 extended E2, bit1 E2 probe timing, bit2 reverse A order,
# bit3 method control (role 2 only).
EXPECTED_FEATURES = {1: 0b0111, 2: 0b1111}

DEFAULT_BOARD_CLI = '/usr/bin/idma_research'
DEFAULT_BOOT_ID_PATH = '/proc/sys/kernel/random/boot_id'
DEFAULT_STARTUP_TIMEOUT = 300.0
DEFAULT_COMMAND_TIMEOUT = 120.0
DEFAULT_CASE_TIMEOUT = 1800.0
DEFAULT_TRANSFER_TIMEOUT = 900.0
DEFAULT_BAUDRATE = 115200
MAX_ARCHIVE_BYTES = 2 * 1024 * 1024 * 1024

# The board case archive may contain exactly these entries and nothing else, so
# an archive can never overwrite SAVED / metadata.json / archive.tar.gz or any
# other host-side artefact.
CASE_ARCHIVE_FILES = (
    'record.json',
    'stderr.txt',
    'exit.txt',
    'argv.txt',
    'taint.before',
    'taint.after',
    'dmesg.before',
    'dmesg.after',
    'uname.before',
    'uname.after',
)

BEGIN_PREFIX = '__R2B'
END_PREFIX = '__R2E'

# Shell fragments are assembled from these so that the Python source stays free
# of embedded escapes; the emitted shell line is the real thing.
_DQUOTE = chr(34)
_SQUOTE = chr(39)
_SHELL_NL = chr(92) + 'n'

_CASE_ID_RE = re.compile(r'^[a-z0-9][a-z0-9-]{0,127}$')
_SHA256_RE = re.compile(r'^[0-9a-f]{64}$')
_B64_WS = frozenset(bytes([32, 9, 13, 10, 11, 12]))


class Research2Error(Exception):
    pass


class ManifestError(Research2Error):
    pass


class PreflightError(Research2Error):
    pass


class TechnicalFailure(Research2Error):
    pass


class ShellResult:
    def __init__(self, stdout: bytes, exit_code: int):
        self.stdout = stdout
        self.exit_code = exit_code

    def __repr__(self) -> str:
        return ('ShellResult(exit_code=' + repr(self.exit_code) +
                ', stdout_len=' + str(len(self.stdout)) + ')')


def utc_now() -> str:
    return time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: pathlib.Path) -> str:
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b''):
            h.update(chunk)
    return h.hexdigest()


def _canonical_json(obj: Any) -> bytes:
    return json.dumps(obj, sort_keys=True, separators=(',', ':'),
                      ensure_ascii=False).encode('utf-8')


def _text(data: Any) -> str:
    if isinstance(data, bytes):
        return data.decode('utf-8', errors='replace')
    return str(data)


def _first_field(text: str) -> str:
    parts = str(text).split()
    if not parts:
        return ''
    return parts[0].strip().lower()


def _read_text(path: pathlib.Path) -> Optional[str]:
    try:
        return path.read_text(encoding='utf-8', errors='replace')
    except OSError:
        return None


def _is_relative_to(path: pathlib.Path, root: pathlib.Path) -> bool:
    try:
        path.relative_to(root)
        return True
    except ValueError:
        return False


def safe_package_relpath(rel: str) -> pathlib.PurePosixPath:
    if not isinstance(rel, str) or not rel:
        raise ManifestError('empty path')
    if chr(0) in rel:
        raise ManifestError('NUL in path')
    if chr(92) in rel:
        raise ManifestError('backslash rejected: ' + rel)
    p = pathlib.PurePosixPath(rel)
    if p.is_absolute():
        raise ManifestError('absolute path rejected: ' + rel)
    parts = p.parts
    if not parts:
        raise ManifestError('empty path')
    for part in parts:
        if part in ('', '.', '..'):
            raise ManifestError('path escape rejected: ' + rel)
    return p


def resolve_package_path(root: pathlib.Path, rel: str, must_exist: bool = True) -> pathlib.Path:
    root_resolved = pathlib.Path(root).resolve()
    relp = safe_package_relpath(rel)
    candidate = root_resolved.joinpath(*relp.parts)
    cur = root_resolved
    for part in relp.parts:
        cur = cur / part
        if cur.is_symlink():
            resolved = cur.resolve()
            if not _is_relative_to(resolved, root_resolved):
                raise ManifestError('symlink escapes package: ' + rel)
    resolved = candidate.resolve()
    if not _is_relative_to(resolved, root_resolved):
        raise ManifestError('path escapes package: ' + rel)
    if must_exist and not resolved.exists():
        raise ManifestError('missing package file: ' + rel)
    return resolved


def verify_sha256(path: pathlib.Path, expected: str, label: str) -> str:
    if not isinstance(expected, str) or not re.fullmatch(r'[0-9a-fA-F]{64}', expected):
        raise ManifestError(label + ': invalid sha256')
    actual = sha256_file(path)
    if actual.lower() != expected.lower():
        raise ManifestError(label + ': sha256 mismatch: ' + actual + ' != ' + expected)
    return actual


def load_manifest(manifest_path: pathlib.Path, manifest_sha256: Optional[str] = None) -> Dict[str, Any]:
    manifest_path = pathlib.Path(manifest_path)
    if manifest_sha256:
        verify_sha256(manifest_path, manifest_sha256, 'manifest')
    with open(manifest_path, 'r', encoding='utf-8') as f:
        data = json.load(f)
    if not isinstance(data, dict):
        raise ManifestError('manifest must be a JSON object')
    if data.get('schema') != SCHEMA:
        raise ManifestError('schema must be ' + SCHEMA)
    sw = data.get('software')
    if not isinstance(sw, dict):
        raise ManifestError('software must be an object')
    if not isinstance(sw.get('kernel_release'), str) or not sw['kernel_release']:
        raise ManifestError('software.kernel_release missing')
    if not isinstance(sw.get('cli_sha256'), str):
        raise ManifestError('software.cli_sha256 missing')
    if sw.get('board_cli_path') is not None and not isinstance(sw['board_cli_path'], str):
        raise ManifestError('software.board_cli_path must be a string')
    images = data.get('images')
    if not isinstance(images, list) or len(images) != 2:
        raise ManifestError('images must contain exactly two entries')
    roles: Dict[int, Any] = {}
    for img in images:
        if not isinstance(img, dict):
            raise ManifestError('image entry must be an object')
        role = img.get('role')
        if role not in (1, 2):
            raise ManifestError('image role must be 1 or 2')
        if role in roles:
            raise ManifestError('duplicate image role')
        if not isinstance(img.get('bit'), str):
            raise ManifestError('image.bit missing')
        if not isinstance(img.get('sha256'), str):
            raise ManifestError('image.sha256 missing')
        roles[role] = img
    if set(roles.keys()) != {1, 2}:
        raise ManifestError('images must define roles 1 and 2')
    scripts = data.get('scripts')
    if not isinstance(scripts, dict):
        raise ManifestError('scripts must be an object')
    for name in ('plan', 'analyzer'):
        if not isinstance(scripts.get(name), str):
            raise ManifestError('scripts.' + name + ' missing')
    return data


def verify_manifest_package_files(manifest: Dict[str, Any], package_root: pathlib.Path) -> Dict[str, str]:
    root = pathlib.Path(package_root)
    verified: Dict[str, str] = {}
    for img in manifest['images']:
        p = resolve_package_path(root, img['bit'])
        label = 'image role ' + str(img['role'])
        verified['image[' + str(img['role']) + ']'] = verify_sha256(p, img['sha256'], label)
    scripts = manifest.get('scripts', {})
    for name in ('plan', 'analyzer'):
        rel = scripts.get(name)
        if rel:
            p = resolve_package_path(root, rel)
            expected = scripts.get(name + '_sha256')
            if expected:
                verified['script.' + name] = verify_sha256(p, expected, 'script ' + name)
            else:
                verified['script.' + name] = sha256_file(p)
    sw = manifest.get('software', {})
    cli_path = sw.get('cli_path')
    if cli_path:
        p = resolve_package_path(root, cli_path)
        verified['software.cli'] = verify_sha256(p, sw['cli_sha256'], 'software cli')
    return verified


def run_preflight(manifest_path: pathlib.Path, package_root: pathlib.Path, serial_port: str,
                  manifest_sha256: Optional[str] = None) -> Dict[str, Any]:
    manifest = load_manifest(manifest_path, manifest_sha256)
    verified = verify_manifest_package_files(manifest, package_root)
    openfpgaloader = shutil.which('openFPGALoader')
    if not openfpgaloader:
        raise PreflightError('openFPGALoader not found in PATH')
    if importlib.util.find_spec('serial') is None:
        raise PreflightError('pyserial is not installed')
    serial_mod = importlib.util.module_from_spec(importlib.util.find_spec('serial'))
    try:
        serial_mod = __import__('serial')
        ser = serial_mod.Serial(serial_port, baudrate=DEFAULT_BAUDRATE, timeout=0.2, write_timeout=0.2)
        ser.close()
    except Exception as exc:
        raise PreflightError('serial ' + serial_port + ' is not free: ' + repr(exc))
    software = manifest['software']
    return {
        'schema': 'research2-preflight-v1',
        'manifest_path': str(pathlib.Path(manifest_path).resolve()),
        'manifest_schema': manifest['schema'],
        'kernel_release': software['kernel_release'],
        'cli_sha256': software['cli_sha256'],
        'board_cli_path': software.get('board_cli_path') or DEFAULT_BOARD_CLI,
        'verified_files': verified,
        'openfpgaloader': openfpgaloader,
        'serial_port': serial_port,
        'serial_free': True,
        'pyserial': True,
        'expected_features': {str(role): EXPECTED_FEATURES[role] for role in (1, 2)},
    }


def _default_serial_factory(port: str, baudrate: int, timeout: float):
    import serial
    return serial.Serial(
        port,
        baudrate=baudrate,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=timeout,
        write_timeout=timeout,
    )


class SerialConsole:
    '''Framed serial console.

    Every command is sent as one line that first prints a begin marker built
    from shell parts, then runs the command, then prints an end marker built
    from the same parts followed by the exit status.  Because the marker text
    never appears literally in the command line, an echoed command line, a
    shell prompt or any other boot chatter can never be mistaken for a marker.
    '''

    def __init__(self, port: str, raw_log_path: pathlib.Path, events_log_path: pathlib.Path,
                 serial_factory=None, baudrate: int = DEFAULT_BAUDRATE):
        self.port = port
        self.baudrate = baudrate
        self.raw_log_path = pathlib.Path(raw_log_path)
        self.events_log_path = pathlib.Path(events_log_path)
        self.serial_factory = serial_factory
        self._ser = None
        self._thread = None
        self._stop = threading.Event()
        self._buffer = bytearray()
        self._cond = threading.Condition()
        self._write_lock = threading.RLock()

    def start(self, timeout: float = 10.0) -> None:
        self.raw_log_path.parent.mkdir(parents=True, exist_ok=True)
        self.events_log_path.parent.mkdir(parents=True, exist_ok=True)
        factory = self.serial_factory or _default_serial_factory
        self._ser = factory(self.port, self.baudrate, 0.1)
        self._thread = threading.Thread(target=self._reader_loop, name='serial-reader', daemon=True)
        self._thread.start()

    def _log_event(self, text: str) -> None:
        try:
            with open(self.events_log_path, 'a', encoding='utf-8') as f:
                f.write(utc_now() + ' ' + text + chr(10))
        except Exception:
            pass

    def _reader_loop(self) -> None:
        try:
            with open(self.raw_log_path, 'ab', buffering=0) as raw:
                while not self._stop.is_set():
                    try:
                        chunk = self._ser.read(4096)
                    except Exception as exc:
                        self._log_event('serial read error: ' + repr(exc))
                        break
                    if not chunk:
                        continue
                    raw.write(chunk)
                    with self._cond:
                        self._buffer.extend(chunk)
                        self._cond.notify_all()
        except Exception as exc:
            self._log_event('serial reader fatal: ' + repr(exc))

    def close(self) -> None:
        self._stop.set()
        if self._thread is not None:
            self._thread.join(timeout=2.0)
        if self._ser is not None:
            try:
                self._ser.close()
            except Exception:
                pass

    def _write(self, data: bytes) -> None:
        with self._write_lock:
            self._ser.write(data)
            self._ser.flush()

    def disable_echo(self) -> None:
        '''Blindly switch the board console echo off before any framing.

        This runs before the first framed command so that the command line is
        not typed back by the serial terminal.  The framing itself is safe even
        when echo is still on.
        '''
        if self._ser is None:
            return
        try:
            self._write(b'stty -echo 2>/dev/null || true' + bytes([10]))
        except Exception as exc:
            self._log_event('disable_echo failed: ' + repr(exc))

    def drain_input(self) -> bytes:
        with self._cond:
            data = bytes(self._buffer)
            self._buffer.clear()
            return data

    def read_until(self, marker: bytes, timeout: float) -> bytes:
        deadline = time.monotonic() + timeout
        out = bytearray()
        with self._cond:
            while True:
                idx = self._buffer.find(marker)
                if idx >= 0:
                    out.extend(self._buffer[:idx])
                    del self._buffer[:idx + len(marker)]
                    return bytes(out)
                remaining = deadline - time.monotonic()
                if remaining <= 0:
                    out.extend(self._buffer)
                    self._buffer.clear()
                    raise TimeoutError('timeout waiting for marker ' + repr(marker))
                self._cond.wait(min(remaining, 0.2))

    def read_line(self, timeout: float) -> bytes:
        '''Read one LF-terminated line; a trailing CR is stripped (CRLF buses).'''
        deadline = time.monotonic() + timeout
        with self._cond:
            while True:
                idx = self._buffer.find(bytes([10]))
                if idx >= 0:
                    line = bytes(self._buffer[:idx])
                    del self._buffer[:idx + 1]
                    if line.endswith(bytes([13])):
                        line = line[:-1]
                    return line
                remaining = deadline - time.monotonic()
                if remaining <= 0:
                    raise TimeoutError('timeout waiting for line')
                self._cond.wait(min(remaining, 0.2))

    def new_marker_parts(self) -> Tuple[str, str]:
        return secrets.token_hex(8), secrets.token_hex(8)

    def _run_framed(self, full_command: str, begin: str, end: str, timeout: float) -> ShellResult:
        self.drain_input()
        self._log_event('send: ' + full_command)
        self._write((full_command + chr(10)).encode('utf-8'))
        self.read_until(begin.encode('ascii'), timeout)
        data = self.read_until(end.encode('ascii'), timeout)
        raw_exit = self.read_line(timeout)
        text = raw_exit.decode('utf-8', errors='replace').strip()
        if not re.fullmatch(r'-?[0-9]+', text):
            raise TechnicalFailure('bad exit marker line: ' + repr(raw_exit))
        code = int(text)
        self._log_event('recv ' + end + ' exit=' + str(code) + ' bytes=' + str(len(data)))
        return ShellResult(stdout=data, exit_code=code)

    def run_shell(self, command: str, timeout: float, cwd: Optional[str] = None,
                  exit_var: Optional[str] = None) -> ShellResult:
        if not isinstance(command, str) or not command:
            raise ValueError('empty command')
        if chr(10) in command or chr(13) in command:
            raise ValueError('command must not contain newlines')
        use_exit_var = bool(exit_var)
        if use_exit_var:
            if not isinstance(exit_var, str) or not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', exit_var):
                raise ValueError('exit_var must be a shell variable identifier')
        n1, n2 = self.new_marker_parts()
        begin = BEGIN_PREFIX + '_' + n1 + '_' + n2 + '__'
        end = END_PREFIX + '_' + n1 + '_' + n2 + '__'
        rc_var = '__r2_rc_' + n1
        body = command
        if cwd:
            # Group the whole command so a failed cd skips every semicolon part.
            body = ('{ cd ' + shlex.quote(str(cwd)) + ' && { ' + command +
                    '; }; }')
        if use_exit_var:
            sentinel = '__R2UNSET_' + n1 + '_' + n2 + '__'
            setup_exit = exit_var + '=' + sentinel + '; '
            tail = ('if [ ' + _DQUOTE + '$' + '{' + exit_var + '}' + _DQUOTE +
                    ' = ' + _DQUOTE + sentinel + _DQUOTE + ' ]; then ' +
                    exit_var + '=$' + rc_var + '; fi; ' +
                    'printf ' + _SQUOTE + '%s%s' + _SHELL_NL + _SQUOTE + ' ' +
                    _DQUOTE + '$em' + _DQUOTE + ' ' +
                    _DQUOTE + '${' + exit_var + '}' + _DQUOTE)
        else:
            setup_exit = ''
            tail = ('printf ' + _SQUOTE + '%s%s' + _SHELL_NL + _SQUOTE + ' ' +
                    _DQUOTE + '$em' + _DQUOTE + ' ' +
                    _DQUOTE + '$' + rc_var + _DQUOTE)
        setup = ('b0=' + BEGIN_PREFIX + '; e0=' + END_PREFIX +
                 '; m1=' + n1 + '; m2=' + n2 +
                 '; bm=' + _DQUOTE + '${b0}_${m1}_${m2}__' + _DQUOTE +
                 '; em=' + _DQUOTE + '${e0}_${m1}_${m2}__' + _DQUOTE)
        head = ('printf ' + _SQUOTE + '%s' + _SHELL_NL + _SQUOTE + ' ' +
                _DQUOTE + '$bm' + _DQUOTE)
        full = (setup + '; ' + head + '; ' + setup_exit + body + '; ' +
                rc_var + '=$?; ' + tail)
        return self._run_framed(full, begin, end, timeout)


class OpenFPGALoaderProgrammer:
    def __init__(self, board: str = BOARD, freq: int = 1000000, timeout: float = 120.0):
        self.board = board
        self.freq = int(freq)
        self.timeout = timeout

    def flash(self, bit_path: str) -> Dict[str, Any]:
        cmd = ['openFPGALoader', '-b', self.board, '--freq', str(self.freq), str(bit_path)]
        try:
            proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                  timeout=self.timeout, check=False)
        except subprocess.TimeoutExpired as exc:
            raise TechnicalFailure('openFPGALoader timeout: ' + repr(exc))
        if proc.returncode != 0:
            raise TechnicalFailure(
                'openFPGALoader failed rc=' + str(proc.returncode) + ': '
                + proc.stderr.decode(errors='replace')
            )
        return {
            'cmd': cmd,
            'rc': proc.returncode,
            'stdout': proc.stdout.decode(errors='replace'),
            'stderr': proc.stderr.decode(errors='replace'),
        }


def _as_int(value: Any, label: str) -> int:
    '''Parse a caps number: JSON integer, decimal string or 0x hex string.'''
    if isinstance(value, bool):
        raise TechnicalFailure(label + ' is a boolean, not a number')
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        text = value.strip()
        try:
            if text[:2].lower() == '0x':
                return int(text, 16)
            return int(text, 10)
        except ValueError:
            raise TechnicalFailure(label + ' is not a number: ' + repr(value))
    raise TechnicalFailure(label + ' is not a number: ' + repr(value))


def parse_caps(text: str) -> Dict[str, Any]:
    '''Parse the exact --caps2 stdout.  No regex fallback, no guessing.'''
    stripped = str(text).strip()
    if not stripped:
        raise TechnicalFailure('caps output is empty')
    try:
        obj = json.loads(stripped)
    except ValueError as exc:
        raise TechnicalFailure('caps output is not JSON: ' + repr(stripped[:200]) + ' (' + str(exc) + ')')
    if not isinstance(obj, dict):
        raise TechnicalFailure('caps output is not a JSON object')
    return obj


def check_caps_for_role(caps: Dict[str, Any], role: int) -> int:
    '''Full research2 capability gate; returns the feature bits.

    Required: schema, success true, empty error, magic, version 1, the exact
    role and the exact feature bits of that role, and the observer magic.
    '''
    role = int(role)
    if not isinstance(caps, dict):
        raise TechnicalFailure('caps is not an object')
    if caps.get('schema') != CAPS_SCHEMA:
        raise TechnicalFailure('caps schema ' + repr(caps.get('schema')) + ' is not ' + repr(CAPS_SCHEMA))
    if caps.get('success') is not True:
        raise TechnicalFailure('caps success is not true (error=' + repr(caps.get('error')) + ')')
    error = caps.get('error')
    if isinstance(error, str) and error.strip():
        raise TechnicalFailure('caps reported an error: ' + error)
    magic = _as_int(caps.get('magic'), 'caps magic')
    if magic != IDMA2CAP_MAGIC:
        raise TechnicalFailure('caps magic mismatch: ' + repr(caps.get('magic')))
    version = _as_int(caps.get('version'), 'caps version')
    if version != CAPS_VERSION:
        raise TechnicalFailure('caps version mismatch: ' + str(version))
    caps_role = _as_int(caps.get('role'), 'caps role')
    if caps_role != role:
        raise TechnicalFailure('caps role mismatch: got ' + str(caps_role) + ', expected ' + str(role))
    expected = EXPECTED_FEATURES.get(role)
    if expected is None:
        raise TechnicalFailure('unknown role ' + str(role))
    features = _as_int(caps.get('features'), 'caps features')
    if features != expected:
        raise TechnicalFailure('caps features mismatch: got ' + str(features) + ', expected ' + str(expected))
    observer = _as_int(caps.get('observer_magic'), 'caps observer_magic')
    if observer != OBSERVER_MAGIC:
        raise TechnicalFailure('caps observer magic mismatch: ' + repr(caps.get('observer_magic')))
    return features


def quote_argv(argv: Sequence[Any]) -> str:
    return ' '.join(shlex.quote(str(a)) for a in argv)


def safe_case_id(cid: Any) -> str:
    if not isinstance(cid, str) or not _CASE_ID_RE.fullmatch(cid):
        raise TechnicalFailure('unsafe case id: ' + repr(cid))
    return cid


def safe_extract_tar_gz(data: bytes, dest: pathlib.Path,
                        allowed_names: Optional[Sequence[str]] = None,
                        max_total_size: int = MAX_ARCHIVE_BYTES) -> pathlib.Path:
    '''Extract a case archive.

    Rejects absolute paths, traversal, backslashes/NUL, duplicate entries, any
    link, device or fifo, any nesting, and - when an allowlist is given - any
    entry outside it.
    '''
    dest = pathlib.Path(dest).resolve()
    dest.mkdir(parents=True, exist_ok=True)
    allowed = None if allowed_names is None else set(allowed_names)
    seen = set()
    total = 0
    with tarfile.open(fileobj=io.BytesIO(data), mode='r:gz') as tar:
        for member in tar.getmembers():
            name = member.name
            if not name or name.startswith('/') or name.startswith(chr(92)):
                raise TechnicalFailure('tar absolute path rejected: ' + repr(name))
            if chr(92) in name or chr(0) in name:
                raise TechnicalFailure('tar backslash/NUL rejected: ' + repr(name))
            parts: List[str] = []
            for part in pathlib.PurePosixPath(name).parts:
                if part in ('', '.'):
                    continue
                if part == '..':
                    raise TechnicalFailure('tar traversal rejected: ' + repr(name))
                parts.append(part)
            if not parts:
                continue
            if member.issym() or member.islnk():
                raise TechnicalFailure('tar link rejected: ' + repr(name))
            if member.isdev() or member.isfifo():
                raise TechnicalFailure('tar device/fifo rejected: ' + repr(name))
            key = '/'.join(parts)
            if key in seen:
                raise TechnicalFailure('tar duplicate entry rejected: ' + repr(name))
            seen.add(key)
            if allowed is not None and (len(parts) != 1 or parts[0] not in allowed):
                raise TechnicalFailure('tar entry outside the case allowlist: ' + repr(name))
            target = dest.joinpath(*parts).resolve()
            if not _is_relative_to(target, dest):
                raise TechnicalFailure('tar escapes dest: ' + repr(name))
            if member.isdir():
                target.mkdir(parents=True, exist_ok=True)
                continue
            if not member.isfile():
                raise TechnicalFailure('tar unsupported type: ' + repr(name))
            total += member.size
            if total > max_total_size:
                raise TechnicalFailure('tar too large')
            target.parent.mkdir(parents=True, exist_ok=True)
            src = tar.extractfile(member)
            if src is None:
                raise TechnicalFailure('tar member has no data: ' + repr(name))
            with src, open(target, 'wb') as dst:
                shutil.copyfileobj(src, dst)
    return dest


def _load_module(name: str):
    '''Load research2_plan / research2_results by file path.

    The scripts directory is deliberately not added to sys.path: a local module
    name must never shadow a stdlib module (the classic failure is scripts/
    platform.py breaking uuid/random).
    '''
    script_dir = pathlib.Path(__file__).resolve().parent
    path = script_dir / (name + '.py')
    if not path.is_file():
        raise TechnicalFailure('cannot find ' + name + '.py next to the runner')
    module_name = 'research2_host_' + name
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        raise TechnicalFailure('cannot load ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    try:
        spec.loader.exec_module(module)
    except Exception as exc:
        raise TechnicalFailure('cannot import ' + name + ': ' + repr(exc))
    return module


class Research2Runner:
    def __init__(self, args, manifest: Dict[str, Any], package_root: pathlib.Path,
                 out_dir: pathlib.Path, console, programmer, plan_module, results_module,
                 manifest_sha256: Optional[str] = None):
        self.args = args
        self.manifest = manifest
        self.package_root = pathlib.Path(package_root)
        self.out_dir = pathlib.Path(out_dir)
        self.console = console
        self.programmer = programmer
        self.plan_module = plan_module
        self.results_module = results_module
        self.manifest_sha256 = manifest_sha256
        self.run_id = secrets.token_hex(8)
        self.board_root = '/tmp/research2-' + self.run_id
        software = manifest.get('software') or {}
        self.board_cli = str(software.get('board_cli_path') or DEFAULT_BOARD_CLI)
        self.boot_id_path = DEFAULT_BOOT_ID_PATH
        self.stage_dir: Optional[pathlib.Path] = None
        self.current_stage: Optional[str] = None
        self.current_case: Optional[str] = None
        self.boot: Dict[str, Any] = {}
        self.boots: Dict[int, Dict[str, Any]] = {}
        self.selection: Optional[Dict[str, Any]] = None
        self.completed_cases: List[Tuple[str, str]] = []
        self.flashed_roles: List[int] = []
        self.stop_written = False

    def _log(self, msg: str) -> None:
        line = utc_now() + ' ' + msg
        try:
            with open(self.out_dir / 'run.log', 'a', encoding='utf-8') as f:
                f.write(line + chr(10))
        except Exception:
            pass
        print(line, file=sys.stderr)

    def _write_json(self, rel: str, obj: Any) -> None:
        p = self.out_dir / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        with open(p, 'w', encoding='utf-8') as f:
            json.dump(obj, f, ensure_ascii=False, indent=2, sort_keys=True)
            f.write(chr(10))

    def _stop(self, reason: str) -> None:
        if self.stop_written:
            return
        self.stop_written = True
        try:
            self.out_dir.mkdir(parents=True, exist_ok=True)
            with open(self.out_dir / 'STOPPED.txt', 'w', encoding='utf-8') as f:
                f.write('stopped_at=' + utc_now() + chr(10))
                f.write('reason=' + reason + chr(10))
                f.write('stage=' + str(self.current_stage) + chr(10))
                f.write('case=' + str(self.current_case) + chr(10))
        except Exception:
            pass

    def _image_for_role(self, role: int) -> Dict[str, Any]:
        for img in self.manifest['images']:
            if int(img['role']) == int(role):
                return img
        raise TechnicalFailure('no image for role ' + str(role))

    def _sync(self) -> None:
        res = self.console.run_shell('sync', timeout=self.args.command_timeout)
        if res.exit_code != 0:
            raise TechnicalFailure('sync before the next BIT failed exit=' + str(res.exit_code))

    def _flash(self, image: Dict[str, Any]) -> None:
        bit_path = resolve_package_path(self.package_root, image['bit'])
        result = self.programmer.flash(str(bit_path))
        self._write_json(self.current_stage + '/flash.json', result)

    def _wait_for_root(self, timeout: float) -> None:
        '''Wait for a usable root shell.

        The probe is a harmless builtin (no experiment is ever re-run) and the
        framing makes an echoed command line or a shell prompt harmless.
        '''
        deadline = time.monotonic() + timeout
        last_err: Optional[BaseException] = None
        self.console.disable_echo()
        while time.monotonic() < deadline:
            probe_timeout = max(1.0, min(5.0, deadline - time.monotonic()))
            try:
                res = self.console.run_shell('true', timeout=probe_timeout)
                if res.exit_code == 0:
                    return
                last_err = TechnicalFailure('readiness probe exit=' + str(res.exit_code))
            except Exception as exc:
                last_err = exc
            self.console.disable_echo()
            time.sleep(1.0)
        raise TechnicalFailure('root shell not ready after ' + str(timeout) + 's: ' + repr(last_err))

    def _collect_boot_info(self, role: int) -> Dict[str, Any]:
        info: Dict[str, Any] = {'role': role, 'collected_at': utc_now()}
        res = self.console.run_shell('uname -r', timeout=self.args.command_timeout)
        if res.exit_code != 0:
            raise TechnicalFailure('uname -r failed exit=' + str(res.exit_code))
        info['uname_r'] = _text(res.stdout).strip()
        expected_kernel = self.manifest['software']['kernel_release']
        if info['uname_r'] != expected_kernel:
            raise TechnicalFailure('kernel mismatch: ' + repr(info['uname_r']) + ' != ' + repr(expected_kernel))

        res = self.console.run_shell('sha256sum ' + shlex.quote(self.board_cli),
                                     timeout=self.args.command_timeout)
        if res.exit_code != 0:
            raise TechnicalFailure('board CLI sha256sum failed exit=' + str(res.exit_code))
        cli_sha = _first_field(_text(res.stdout))
        if not _SHA256_RE.fullmatch(cli_sha):
            raise TechnicalFailure('bad board CLI sha256 line: ' + repr(_text(res.stdout)))
        info['board_cli'] = self.board_cli
        info['board_cli_sha256'] = cli_sha
        expected_cli = str(self.manifest['software']['cli_sha256']).strip().lower()
        if cli_sha != expected_cli:
            raise TechnicalFailure('board CLI sha256 mismatch: ' + cli_sha + ' != ' + expected_cli)

        res = self.console.run_shell(shlex.quote(self.board_cli) + ' --caps2',
                                     timeout=self.args.command_timeout)
        info['caps_exit'] = res.exit_code
        info['caps_raw'] = _text(res.stdout)
        if res.exit_code != 0:
            raise TechnicalFailure('caps2 failed exit=' + str(res.exit_code) +
                                   ' stdout=' + repr(info['caps_raw'][:400]))
        caps = parse_caps(info['caps_raw'])
        info['caps'] = caps
        info['features'] = check_caps_for_role(caps, role)

        res = self.console.run_shell('cat ' + shlex.quote(self.boot_id_path),
                                     timeout=self.args.command_timeout)
        if res.exit_code != 0:
            raise TechnicalFailure('boot_id read failed exit=' + str(res.exit_code))
        info['boot_id'] = _text(res.stdout).strip()
        if not info['boot_id']:
            raise TechnicalFailure('boot_id is empty')
        return info

    def _check_boot_change(self, role: int, boot: Dict[str, Any]) -> None:
        if not self.boots:
            self.boots[role] = boot
            return
        for other, info in self.boots.items():
            if info.get('boot_id') == boot.get('boot_id'):
                raise TechnicalFailure('boot_id did not change after the BIT switch: role ' +
                                       str(other) + ' and role ' + str(role) + ' both booted ' +
                                       repr(boot.get('boot_id')))
        self.boots[role] = boot

    @staticmethod
    def _needs_selection(case: Dict[str, Any]) -> bool:
        if case.get('needs_selection') is True:
            return True
        argv = case.get('argv')
        return not isinstance(argv, list) or not argv

    def _freeze_selection(self, stage: str, cases: List[Dict[str, Any]]) -> Dict[str, Any]:
        '''Freeze the calibration parameters.

        Only records of plan cases whose phase is exactly calibration are handed
        to select_configs; there is no all-completed fallback, so formal /
        boundary / holdout data can never leak into the selection.
        '''
        records: Dict[str, Any] = {}
        for case in cases:
            if case.get('phase') != 'calibration':
                continue
            cid = safe_case_id(case.get('id'))
            rec_path = self.stage_dir / 'runs' / cid / 'record.json'
            if not rec_path.is_file():
                raise TechnicalFailure('calibration record missing: ' + cid)
            try:
                records[cid] = json.loads(rec_path.read_text(encoding='utf-8'))
            except Exception as exc:
                raise TechnicalFailure('bad calibration record ' + cid + ': ' + repr(exc))
        if not records:
            raise TechnicalFailure('cannot freeze the selection: no calibration records in the plan')
        selection = self.plan_module.select_configs(records)
        if not isinstance(selection, dict):
            raise TechnicalFailure('select_configs must return a dict')
        audit = {
            'schema': SELECTION_AUDIT_SCHEMA,
            'stage': stage,
            'frozen_at': utc_now(),
            'calibration_case_ids': sorted(records),
            'input_file_sha256': {
                cid: sha256_file(self.stage_dir / 'runs' / cid / 'record.json')
                for cid in sorted(records)
            },
            'selection_sha256': sha256_bytes(_canonical_json(selection)),
        }
        self._write_json(stage + '/selection.json', selection)
        self._write_json(stage + '/selection.audit.json', audit)
        return selection

    def _resolve_case(self, case: Dict[str, Any], selection: Optional[Dict[str, Any]]) -> Dict[str, Any]:
        '''Call the strict two-argument plan API; there is no arity fallback.'''
        try:
            resolved = self.plan_module.resolve_case(case, selection)
        except Exception as exc:
            raise TechnicalFailure('resolve_case failed for ' + str(case.get('id')) + ': ' + repr(exc))
        if not isinstance(resolved, dict):
            raise TechnicalFailure('resolve_case returned a non-dict for ' + str(case.get('id')))
        return resolved

    def _capture_state(self, board_dir: str, tag: str) -> None:
        specs = (
            ('taint.' + tag, 'cat /proc/sys/kernel/tainted'),
            ('dmesg.' + tag, 'dmesg'),
            ('uname.' + tag, 'uname -a'),
        )
        for name, command in specs:
            res = self.console.run_shell(command + ' > ' + shlex.quote(name) + ' 2>&1',
                                         timeout=self.args.command_timeout, cwd=board_dir)
            if res.exit_code != 0:
                raise TechnicalFailure('failed to capture ' + tag + ' state: ' + name +
                                       ' exit=' + str(res.exit_code))

    def _collect_and_transfer(self, cid: str, board_case_dir: str, run_dir: pathlib.Path) -> Dict[str, Any]:
        '''Hash, tar, base64, verify and safely extract one case.

        The raw base64 stream is written to disk before it is decoded, and the
        archive bytes are kept even when the transfer is bad.
        '''
        board_hashes: Dict[str, str] = {}
        for name in ('record.json', 'stderr.txt', 'exit.txt'):
            res = self.console.run_shell('sha256sum ' + shlex.quote(name),
                                         timeout=self.args.command_timeout, cwd=board_case_dir)
            if res.exit_code != 0:
                raise TechnicalFailure('case ' + cid + ': sha256sum ' + name +
                                       ' exit=' + str(res.exit_code))
            digest = _first_field(_text(res.stdout))
            if not _SHA256_RE.fullmatch(digest):
                raise TechnicalFailure('case ' + cid + ': bad sha256sum output for ' + name +
                                       ': ' + repr(_text(res.stdout)))
            board_hashes[name] = digest
        archive_path = board_case_dir + '.tar.gz'
        res = self.console.run_shell('tar -czf ' + shlex.quote(archive_path) +
                                     ' -C ' + shlex.quote(board_case_dir) + ' .',
                                     timeout=self.args.command_timeout)
        if res.exit_code != 0:
            raise TechnicalFailure('case ' + cid + ': tar exit=' + str(res.exit_code))
        res = self.console.run_shell('sha256sum ' + shlex.quote(archive_path),
                                     timeout=self.args.command_timeout)
        if res.exit_code != 0:
            raise TechnicalFailure('case ' + cid + ': archive sha256sum exit=' + str(res.exit_code))
        archive_sha = _first_field(_text(res.stdout))
        if not _SHA256_RE.fullmatch(archive_sha):
            raise TechnicalFailure('case ' + cid + ': bad archive sha256: ' + repr(_text(res.stdout)))

        res = self.console.run_shell('base64 ' + shlex.quote(archive_path),
                                     timeout=self.args.transfer_timeout)
        raw_b64 = res.stdout
        with open(run_dir / 'archive.b64', 'wb') as f:
            f.write(raw_b64)
        if res.exit_code != 0:
            raise TechnicalFailure('case ' + cid + ': base64 exit=' + str(res.exit_code))
        cleaned = bytes(b for b in raw_b64 if b not in _B64_WS)
        try:
            data = base64.b64decode(cleaned, validate=True)
        except Exception as exc:
            raise TechnicalFailure('case ' + cid + ': base64 decode failed: ' + repr(exc))
        actual_sha = sha256_bytes(data)
        if actual_sha != archive_sha:
            raise TechnicalFailure('case ' + cid + ': archive sha mismatch: ' +
                                   actual_sha + ' != ' + archive_sha)
        with open(run_dir / 'archive.tar.gz', 'wb') as f:
            f.write(data)
        with open(run_dir / 'archive.sha256', 'w', encoding='utf-8') as f:
            f.write(archive_sha + '  archive.tar.gz' + chr(10))

        safe_extract_tar_gz(data, run_dir / 'board', allowed_names=CASE_ARCHIVE_FILES)
        board_record = run_dir / 'board' / 'record.json'
        if not board_record.is_file():
            raise TechnicalFailure('case ' + cid + ': record.json is missing in the archive')
        record_sha = sha256_file(board_record)
        if record_sha != board_hashes['record.json']:
            raise TechnicalFailure('case ' + cid + ': record.json sha mismatch against the board hash')
        shutil.copyfile(board_record, run_dir / 'record.json')
        with open(run_dir / 'record.sha256', 'w', encoding='utf-8') as f:
            f.write(record_sha + '  record.json' + chr(10))

        exit_text = _read_text(run_dir / 'board' / 'exit.txt')
        exit_code: Optional[int] = None
        if exit_text is not None:
            stripped = exit_text.strip()
            if re.fullmatch(r'-?[0-9]+', stripped):
                exit_code = int(stripped)
        return {
            'exit_code': exit_code,
            'taint_before': _read_text(run_dir / 'board' / 'taint.before'),
            'taint_after': _read_text(run_dir / 'board' / 'taint.after'),
            'record_sha256': record_sha,
            'archive_sha256': archive_sha,
        }

    def _run_one_case(self, stage: str, role: int, case: Dict[str, Any]) -> None:
        cid = safe_case_id(case.get('id'))
        self.current_case = cid
        phase = case.get('phase')
        argv = case.get('argv')
        if not isinstance(argv, list) or not argv:
            raise TechnicalFailure('case ' + cid + ': missing argv')
        for item in argv:
            if not isinstance(item, str) or not item:
                raise TechnicalFailure('case ' + cid + ': argv entries must be non-empty strings')
            if chr(10) in item or chr(13) in item or chr(0) in item:
                raise TechnicalFailure('case ' + cid + ': unsafe argv entry')
        image = self._image_for_role(role)
        board_case_dir = self.board_root + '/' + cid
        run_dir = self.stage_dir / 'runs' / cid
        run_dir.mkdir(parents=True, exist_ok=False)
        cli_cmd = self.board_cli + ' ' + quote_argv(argv)
        inner = cli_cmd + ' > record.json 2> stderr.txt; ec=$?; echo ' + _DQUOTE + '$ec' + _DQUOTE + ' > exit.txt'
        with open(run_dir / 'board-command.txt', 'w', encoding='utf-8') as f:
            f.write('cd ' + board_case_dir + ' && ' + inner + chr(10))

        res = self.console.run_shell('mkdir -p ' + shlex.quote(board_case_dir),
                                     timeout=self.args.command_timeout)
        if res.exit_code != 0:
            raise TechnicalFailure('case ' + cid + ': cannot create the board case directory')
        self._capture_state(board_case_dir, 'before')
        res = self.console.run_shell('printf %s ' + shlex.quote(json.dumps(argv, ensure_ascii=False)) +
                                     ' > argv.txt',
                                     timeout=self.args.command_timeout, cwd=board_case_dir)
        if res.exit_code != 0:
            raise TechnicalFailure('case ' + cid + ': cannot record argv.txt on the board')

        framed_exit: Optional[int] = None
        framed_error: Optional[BaseException] = None
        try:
            res = self.console.run_shell(inner, timeout=self.args.case_timeout,
                                         cwd=board_case_dir, exit_var='ec')
            framed_exit = res.exit_code
        except Exception as exc:
            framed_error = exc

        capture_error: Optional[BaseException] = None
        try:
            self._capture_state(board_case_dir, 'after')
        except Exception as exc:
            capture_error = exc

        try:
            transfer = self._collect_and_transfer(cid, board_case_dir, run_dir)
        except Exception as exc:
            # Preserve what we know before stopping: the command line and the
            # failure reason, even when nothing could be transferred.
            self._write_json(stage + '/runs/' + cid + '/metadata.json', {
                'schema': METADATA_SCHEMA,
                'id': cid,
                'stage': stage,
                'role': role,
                'phase': phase,
                'argv': argv,
                'command': cli_cmd,
                'exit': framed_exit,
                'taint_before': None,
                'taint_after': None,
                'boot_id': self.boot.get('boot_id'),
                'kernel_release': self.boot.get('uname_r'),
                'board_cli': self.board_cli,
                'board_cli_sha256': self.boot.get('board_cli_sha256'),
                'bit': image['bit'],
                'bit_sha256': image['sha256'],
                'transfer_error': repr(exc),
            })
            raise

        exit_code = transfer['exit_code']
        taint_before = transfer['taint_before']
        taint_after = transfer['taint_after']
        metadata = {
            'schema': METADATA_SCHEMA,
            'id': cid,
            'stage': stage,
            'role': role,
            'phase': phase,
            'argv': argv,
            'command': cli_cmd,
            'exit': exit_code,
            'framed_exit': framed_exit,
            'taint_before': taint_before,
            'taint_after': taint_after,
            'boot_id': self.boot.get('boot_id'),
            'kernel_release': self.boot.get('uname_r'),
            'board_cli': self.board_cli,
            'board_cli_sha256': self.boot.get('board_cli_sha256'),
            'bit': image['bit'],
            'bit_sha256': image['sha256'],
            'record_sha256': transfer['record_sha256'],
            'archive_sha256': transfer['archive_sha256'],
            'finished_at': utc_now(),
        }
        self._write_json(stage + '/runs/' + cid + '/metadata.json', metadata)

        failures: List[str] = []
        if framed_error is not None:
            failures.append('framed command raised: ' + repr(framed_error))
        if capture_error is not None:
            failures.append('state capture raised: ' + repr(capture_error))
        if exit_code is None:
            failures.append('exit.txt is missing or not an integer')
        elif exit_code != 0:
            failures.append('CLI exit ' + str(exit_code))
        if exit_code is not None and framed_exit is not None and framed_exit != exit_code:
            failures.append('framed exit ' + str(framed_exit) + ' != exit.txt ' + str(exit_code))
        if taint_after is None or taint_after.strip() != '0':
            failures.append('taint after=' + repr(taint_after))
        if failures:
            raise TechnicalFailure('case ' + cid + ': ' + '; '.join(failures))

        try:
            record = json.loads((run_dir / 'record.json').read_text(encoding='utf-8'))
        except Exception as exc:
            raise TechnicalFailure('case ' + cid + ': record.json is not usable JSON: ' + repr(exc))
        errors = self.results_module.validate_record(case, record)
        if errors:
            raise TechnicalFailure('case ' + cid + ': validation errors: ' + repr(errors))
        try:
            metrics = self.results_module.metrics(record)
        except Exception as exc:
            raise TechnicalFailure('case ' + cid + ': metrics raised: ' + repr(exc))
        self._write_json(stage + '/runs/' + cid + '/metrics.json', metrics)
        with open(run_dir / 'SAVED', 'w', encoding='utf-8') as f:
            f.write(utc_now() + chr(10))
        self.completed_cases.append((stage, cid))
        self._log('case ' + cid + ' saved')

    def _verify_stage_saved(self, stage: str, cases: List[Dict[str, Any]]) -> None:
        '''Every file of every case must exist and re-hash to its stored digest.'''
        for case in cases:
            cid = safe_case_id(case.get('id'))
            d = self.stage_dir / 'runs' / cid
            for name in ('record.json', 'record.sha256', 'metadata.json',
                         'archive.tar.gz', 'archive.sha256', 'archive.b64', 'SAVED'):
                if not (d / name).is_file():
                    raise TechnicalFailure('stage ' + stage + ' case ' + cid + ' missing ' + name)
            archive_expected = _first_field(_read_text(d / 'archive.sha256') or '')
            archive_actual = sha256_file(d / 'archive.tar.gz')
            if archive_expected != archive_actual:
                raise TechnicalFailure('stage ' + stage + ' case ' + cid + ' archive sha mismatch')
            record_expected = _first_field(_read_text(d / 'record.sha256') or '')
            record_actual = sha256_file(d / 'record.json')
            if record_expected != record_actual:
                raise TechnicalFailure('stage ' + stage + ' case ' + cid + ' record sha mismatch')
            try:
                meta = json.loads((d / 'metadata.json').read_text(encoding='utf-8'))
            except Exception as exc:
                raise TechnicalFailure('stage ' + stage + ' case ' + cid +
                                       ' metadata.json unreadable: ' + repr(exc))
            if meta.get('exit') != 0:
                raise TechnicalFailure('stage ' + stage + ' case ' + cid + ' metadata exit is not 0')
            if str(meta.get('taint_after')).strip() != '0':
                raise TechnicalFailure('stage ' + stage + ' case ' + cid + ' metadata taint is not 0')

    def _run_stage(self, stage: str, role: int) -> None:
        self.current_stage = stage
        self.stage_dir = self.out_dir / stage
        self.stage_dir.mkdir(parents=True, exist_ok=False)
        image = self._image_for_role(role)
        if self.flashed_roles:
            # A failed sync must stop the run before the next BIT is flashed.
            self._sync()
        self._log('stage ' + stage + ': flashing role ' + str(role) + ' ' + str(image['bit']))
        self._flash(image)
        self.flashed_roles.append(role)
        self._wait_for_root(self.args.startup_timeout)
        boot = self._collect_boot_info(role)
        self._check_boot_change(role, boot)
        self.boot = boot
        self._write_json(stage + '/boot.json', boot)

        plan = self.plan_module.build_plan(stage)
        if not isinstance(plan, dict):
            raise TechnicalFailure('build_plan returned a non-dict')
        if not isinstance(plan.get('schema'), str) or not plan['schema']:
            raise TechnicalFailure('the plan has no schema string')
        cases = plan.get('cases')
        if not isinstance(cases, list) or not cases:
            raise TechnicalFailure('the plan has no cases')
        if plan.get('count', len(cases)) != len(cases):
            raise TechnicalFailure('the plan count does not match its case list')
        if plan.get('stage', stage) != stage:
            raise TechnicalFailure('the plan stage is not ' + stage + ': ' + repr(plan.get('stage')))
        self._write_json(stage + '/plan.json', plan)
        self._write_json(stage + '/plan.sha256.json', {
            'schema': PLAN_HASH_SCHEMA,
            'stage': stage,
            'plan_sha256': sha256_bytes(_canonical_json(plan)),
        })

        selection: Optional[Dict[str, Any]] = None
        for case in cases:
            self.current_case = case.get('id')
            if case.get('role') != role:
                raise TechnicalFailure('plan case ' + str(case.get('id')) + ' has role ' +
                                       str(case.get('role')) + ', stage role is ' + str(role))
            if selection is None and self._needs_selection(case):
                selection = self._freeze_selection(stage, cases)
                self.selection = selection
            resolved = self._resolve_case(case, selection)
            self._run_one_case(stage, role, resolved)
        self._verify_stage_saved(stage, cases)
        self._write_json(stage + '/stage-done.json', {
            'schema': 'research2-stage-done-v1',
            'stage': stage,
            'role': role,
            'cases': len(cases),
            'finished_at': utc_now(),
            'selection_frozen': selection is not None,
        })

    def run(self) -> int:
        self.out_dir.mkdir(parents=True, exist_ok=False)
        software = self.manifest.get('software') or {}
        self._write_json('run.json', {
            'schema': RUN_SCHEMA,
            'run_id': self.run_id,
            'started_at': utc_now(),
            'stage': self.args.stage,
            'manifest_schema': self.manifest.get('schema'),
            'manifest_sha256': self.manifest_sha256,
            'kernel_release': software.get('kernel_release'),
            'cli_sha256': software.get('cli_sha256'),
            'board_cli': self.board_cli,
            'board_root': self.board_root,
        })
        try:
            self.console.start()
            stages: List[Tuple[str, int]] = []
            if self.args.stage in ('e2', 'both'):
                stages.append(('e2', 1))
            if self.args.stage in ('method', 'both'):
                stages.append(('method', 2))
            for stage, role in stages:
                self._run_stage(stage, role)
            self._write_json('COMPLETE.json', {
                'schema': 'research2-complete-v1',
                'finished_at': utc_now(),
                'stages': [name for name, _ in stages],
                'completed_cases': [{'stage': s, 'id': c} for s, c in self.completed_cases],
                'boots': {str(role): info.get('boot_id') for role, info in sorted(self.boots.items())},
            })
            return 0
        except KeyboardInterrupt:
            self._stop('KeyboardInterrupt')
            raise
        except Exception as exc:
            self._stop(type(exc).__name__ + ': ' + str(exc))
            raise
        finally:
            try:
                self.console.close()
            except Exception:
                pass


def build_arg_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description='Mac host sequential two-BIT research2 runner')
    p.add_argument('--manifest', required=True)
    p.add_argument('--package-root', default=None)
    p.add_argument('--out', default=None)
    p.add_argument('--serial', required=True)
    p.add_argument('--stage', choices=('e2', 'method', 'both'), default='both')
    p.add_argument('--preflight', action='store_true')
    p.add_argument('--manifest-sha256', default=None)
    p.add_argument('--startup-timeout', type=float, default=DEFAULT_STARTUP_TIMEOUT)
    p.add_argument('--command-timeout', type=float, default=DEFAULT_COMMAND_TIMEOUT)
    p.add_argument('--case-timeout', type=float, default=DEFAULT_CASE_TIMEOUT)
    p.add_argument('--transfer-timeout', type=float, default=DEFAULT_TRANSFER_TIMEOUT)
    p.add_argument('--jtag-freq', type=int, default=1000000)
    return p


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = build_arg_parser().parse_args(argv)
    manifest_path = pathlib.Path(args.manifest)
    package_root = pathlib.Path(args.package_root) if args.package_root else manifest_path.parent
    if args.preflight:
        result = run_preflight(manifest_path, package_root, args.serial, args.manifest_sha256)
        print(json.dumps(result, indent=2, ensure_ascii=False, sort_keys=True))
        return 0
    if not args.out:
        raise SystemExit('--out is required unless --preflight')
    out_dir = pathlib.Path(args.out)
    if out_dir.exists():
        raise SystemExit('--out already exists: ' + str(out_dir))
    manifest = load_manifest(manifest_path, args.manifest_sha256)
    verify_manifest_package_files(manifest, package_root)
    manifest_sha256 = sha256_file(manifest_path)
    plan_module = _load_module('research2_plan')
    results_module = _load_module('research2_results')
    console = SerialConsole(args.serial, out_dir / 'console.raw.log', out_dir / 'console.events.log')
    programmer = OpenFPGALoaderProgrammer(freq=args.jtag_freq)
    runner = Research2Runner(args, manifest, package_root, out_dir, console, programmer,
                             plan_module, results_module, manifest_sha256=manifest_sha256)
    return runner.run()


if __name__ == '__main__':
    sys.exit(main())
