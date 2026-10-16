#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''mac_method88.py - thin Mac host two-boot role-2 method88 runner.

This module is a *thin* wrapper around the delivered and reviewed
``scripts/mac_research2.py`` runner.  It never forks that runner:

* the serial framing (``SerialConsole``) is reused unchanged;
* ``Research2Runner._run_one_case`` is reused unchanged for the per-case
  capture / framed CLI / transfer / exit+taint gates / record validation /
  metrics / SAVED;
* ``_verify_stage_saved``, ``_collect_boot_info``, ``_sync``, ``_stop``,
  ``_image_for_role`` and ``OpenFPGALoaderProgrammer`` are reused unchanged.

What is new here, and only here:

* a ``method88-delivery-v1`` manifest schema that packages **only role 2** and
  pins the exact current software baseline (kernel / CLI sha256 / board CLI
  path), with a strict sha256 for every packaged dependency;
* a frozen plan/selection verifier that regenerates the 88-case plan from the
  frozen selection plus order seed through ``scripts/research_method88.py``
  and requires the regenerated plan to be identical to the frozen one;
* a ``Method88Runner`` subclass that flashes the *same* role-2 BIT twice so the
  frozen 88 cases run 44 / 44 over two fresh boots of one BIT;
* a BusyBox-safe archive console that replaces the one ``tar -czf`` command
  with ``tar -cf`` + ``gzip -c`` and leaves base64 / sha256 / allowlist logic
  completely untouched.

Hard rules kept from mac_research2.py: no automatic retry, no resume, no
overwrite of an existing output directory, first technical error stops and
preserves the raw evidence, and ``--preflight`` never creates the output
directory, never runs the board CLI and never queries the board.
'''
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import pathlib
import re
import shlex
import shutil
import sys
import time
from typing import Any, Dict, List, Optional

# ---------------------------------------------------------------------------
# Module loading.  scripts/ is deliberately NEVER added to sys.path, so a local
# scripts/platform.py can not shadow a stdlib module; every module is loaded by
# file path exactly like mac_research2._load_module does.
# ---------------------------------------------------------------------------
HERE = pathlib.Path(__file__).resolve()
SCRIPTS_DIR = HERE.parent


def _load_module(module_name: str, filename: str):
    path = SCRIPTS_DIR / filename
    if not path.is_file():
        raise RuntimeError('cannot find ' + filename + ' next to ' + str(HERE))
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module


mac2 = _load_module('method88_mac_research2', 'mac_research2.py')
method88 = _load_module('method88_research_method88', 'research_method88.py')
results2 = _load_module('method88_research2_results', 'research2_results.py')

# ---------------------------------------------------------------------------
# method88-delivery-v1 schema and the pinned current software baseline.
# ---------------------------------------------------------------------------
SCHEMA = 'method88-delivery-v1'
PREFLIGHT_SCHEMA = 'method88-preflight-v1'
RUN_SCHEMA = 'method88-run-v1'
COMPLETE_SCHEMA = 'method88-complete-v1'

ROLE = 2
CASES_PER_BOOT = 44
TOTAL_CASES = 88

BASELINE_KERNEL_RELEASE = '6.5.0-rc2-00093-gc0f5e13ea81f'
BASELINE_CLI_SHA256 = '3af284790ba91c9536a23d754f9e22de855b230d071322b84798b92c0043a0ef'
BASELINE_BOARD_CLI_PATH = '/usr/bin/idma_research'
ROLE2_BIT_SHA256 = '31e18ee96e6edd47d444ed3cf9f4279cfa1ba494b53497d37183a3d7d0c33651'

EXPECTED_SOFTWARE = {
    'kernel_release': BASELINE_KERNEL_RELEASE,
    'cli_sha256': BASELINE_CLI_SHA256,
    'board_cli_path': BASELINE_BOARD_CLI_PATH,
}

# Every packaged dependency: manifest['files'] must be exactly this key set.
REQUIRED_FILES = (
    'scripts/mac_research2.py',
    'scripts/research2_plan.py',
    'scripts/research2_results.py',
    'scripts/research_results.py',
    'scripts/research_method88.py',
    'scripts/mac_method88.py',
    'frozen/plan.json',
    'frozen/plan.sha256',
    'frozen/selection.json',
    'frozen/selection.sha256',
    'artifacts/role2.bit',
    'docs/mac-method88.md',
)

FROZEN_FILES = ('plan.json', 'plan.sha256', 'selection.json', 'selection.sha256')

_SHA256_RE = re.compile(r'^[0-9a-f]{64}$')


# ---------------------------------------------------------------------------
# Small helpers
# ---------------------------------------------------------------------------
def _read_json(path: pathlib.Path, label: str) -> Any:
    try:
        text = path.read_text(encoding='utf-8')
    except OSError as exc:
        raise mac2.ManifestError(label + ' is unreadable: ' + repr(exc))
    try:
        return json.loads(text)
    except ValueError as exc:
        raise mac2.ManifestError(label + ' is not JSON: ' + repr(exc))


def _parse_kv(text: str) -> Dict[str, str]:
    out: Dict[str, str] = {}
    for line in str(text).splitlines():
        line = line.strip()
        if not line or '=' not in line:
            continue
        key, value = line.split('=', 1)
        out[key.strip()] = value.strip()
    return out


# ---------------------------------------------------------------------------
# Manifest: load and verify
# ---------------------------------------------------------------------------
def load_manifest(manifest_path: pathlib.Path,
                  manifest_sha256: Optional[str] = None) -> Dict[str, Any]:
    '''Load and validate a method88-delivery-v1 manifest.

    This never reuses mac_research2.load_manifest, which requires exactly two
    images (roles 1 and 2); method88 packages only role 2.
    '''
    manifest_path = pathlib.Path(manifest_path)
    if manifest_sha256:
        mac2.verify_sha256(manifest_path, manifest_sha256, 'manifest')
    with open(manifest_path, 'r', encoding='utf-8') as f:
        data = json.load(f)
    if not isinstance(data, dict):
        raise mac2.ManifestError('manifest must be a JSON object')
    if data.get('schema') != SCHEMA:
        raise mac2.ManifestError('schema must be ' + SCHEMA)

    software = data.get('software')
    if not isinstance(software, dict):
        raise mac2.ManifestError('software must be an object')
    for key, expected in EXPECTED_SOFTWARE.items():
        if software.get(key) != expected:
            raise mac2.ManifestError('software.' + key + ' must be '
                                     + repr(expected) + ', got '
                                     + repr(software.get(key)))

    images = data.get('images')
    if not isinstance(images, list) or len(images) != 1:
        raise mac2.ManifestError('images must contain exactly one entry (role 2)')
    image = images[0]
    if not isinstance(image, dict):
        raise mac2.ManifestError('image entry must be an object')
    if image.get('role') != ROLE:
        raise mac2.ManifestError('image role must be ' + str(ROLE))
    if image.get('bit') != 'artifacts/role2.bit':
        raise mac2.ManifestError('image.bit must be artifacts/role2.bit')
    if not isinstance(image.get('sha256'), str) or not _SHA256_RE.fullmatch(image['sha256']):
        raise mac2.ManifestError('image.sha256 must be a lowercase sha256')
    if image['sha256'] != ROLE2_BIT_SHA256:
        raise mac2.ManifestError('image.sha256 is not the pinned role2 BIT hash')

    frozen = data.get('frozen')
    if not isinstance(frozen, dict) or not isinstance(frozen.get('root'), str) or not frozen['root']:
        raise mac2.ManifestError('frozen.root missing')
    if frozen['root'] != 'frozen':
        raise mac2.ManifestError('frozen.root must be frozen')
    mac2.safe_package_relpath(frozen['root'])

    files = data.get('files')
    if not isinstance(files, dict):
        raise mac2.ManifestError('files must be an object')
    missing = [rel for rel in REQUIRED_FILES if rel not in files]
    if missing:
        raise mac2.ManifestError('files is missing ' + repr(missing))
    extra = sorted(set(files) - set(REQUIRED_FILES))
    if extra:
        raise mac2.ManifestError('files carries unexpected entries ' + repr(extra))
    for rel, digest in files.items():
        mac2.safe_package_relpath(rel)
        if not isinstance(digest, str) or not _SHA256_RE.fullmatch(digest):
            raise mac2.ManifestError('files[' + rel + '] must be a lowercase sha256')
    if files.get('artifacts/role2.bit') != image['sha256']:
        raise mac2.ManifestError('files[artifacts/role2.bit] must equal image.sha256')
    return data


def verify_package_files(manifest: Dict[str, Any], package_root: pathlib.Path) -> Dict[str, str]:
    '''Verify every image and every packaged file with the shared strict path
    resolver (symlinks may never escape the package root).'''
    root = pathlib.Path(package_root)
    verified: Dict[str, str] = {}
    for image in manifest['images']:
        path = mac2.resolve_package_path(root, image['bit'])
        verified['image[' + str(image['role']) + ']'] = mac2.verify_sha256(
            path, image['sha256'], 'image role ' + str(image['role']))
    for rel in sorted(manifest['files']):
        path = mac2.resolve_package_path(root, rel)
        verified['file[' + rel + ']'] = mac2.verify_sha256(
            path, manifest['files'][rel], 'file ' + rel)
    return verified


def verify_frozen(manifest: Dict[str, Any], package_root: pathlib.Path) -> Dict[str, Any]:
    '''Regenerate the frozen 88-case plan and require an exact match.'''
    root = pathlib.Path(package_root)
    frozen_root_rel = manifest['frozen']['root']
    frozen_dir = mac2.resolve_package_path(root, frozen_root_rel)
    if not frozen_dir.is_dir():
        raise mac2.ManifestError('frozen root is not a directory: ' + frozen_root_rel)
    paths: Dict[str, pathlib.Path] = {}
    for name in FROZEN_FILES:
        paths[name] = mac2.resolve_package_path(root, frozen_root_rel.rstrip('/') + '/' + name)

    plan_doc = _read_json(paths['plan.json'], 'frozen/plan.json')
    selection = _read_json(paths['selection.json'], 'frozen/selection.json')
    if not isinstance(plan_doc, dict):
        raise mac2.ManifestError('frozen/plan.json must be a JSON object')

    try:
        method88.validate_selection(selection)
    except Exception as exc:
        raise mac2.ManifestError('frozen selection is rejected: ' + repr(exc))

    canonical = method88.selection_digest(selection)
    if plan_doc.get('selection_sha256') != canonical:
        raise mac2.ManifestError('frozen plan selection_sha256 '
                                 + repr(plan_doc.get('selection_sha256'))
                                 + ' does not match the selection canonical hash '
                                 + canonical)
    try:
        regenerated = method88.build_plan(selection, plan_doc.get('seed'))
    except Exception as exc:
        raise mac2.ManifestError('frozen plan cannot be regenerated: ' + repr(exc))
    regenerated['plan_sha256'] = method88.plan_digest(regenerated)
    if plan_doc.get('plan_sha256') != regenerated['plan_sha256']:
        raise mac2.ManifestError('frozen plan_sha256 mismatch: '
                                 + repr(plan_doc.get('plan_sha256')) + ' != '
                                 + regenerated['plan_sha256'])
    if plan_doc != regenerated:
        raise mac2.ManifestError('the frozen plan is not exactly the regenerated 88-case plan')
    if regenerated.get('count') != TOTAL_CASES:
        raise mac2.ManifestError('the frozen plan must hold ' + str(TOTAL_CASES) + ' cases')

    plan_sha_kv = _parse_kv(paths['plan.sha256'].read_text(encoding='utf-8', errors='replace'))
    if plan_sha_kv.get('plan_sha256') != plan_doc.get('plan_sha256'):
        raise mac2.ManifestError('frozen/plan.sha256 does not carry the plan hash')
    if plan_sha_kv.get('selection_sha256') != canonical:
        raise mac2.ManifestError('frozen/plan.sha256 does not carry the selection canonical hash')

    selection_bytes = paths['selection.json'].read_bytes()
    sel_sha_kv = _parse_kv(paths['selection.sha256'].read_text(encoding='utf-8', errors='replace'))
    if sel_sha_kv.get('selection_file_sha256') != hashlib.sha256(selection_bytes).hexdigest():
        raise mac2.ManifestError('frozen/selection.sha256 does not carry the source file hash')
    if sel_sha_kv.get('selection_canonical_sha256') != canonical:
        raise mac2.ManifestError('frozen/selection.sha256 does not carry the canonical hash')
    return {'plan': plan_doc, 'selection': selection}


# ---------------------------------------------------------------------------
# Preflight: read-only, no output directory, no board CLI, serial open/close only
# ---------------------------------------------------------------------------
def _pyserial_available() -> bool:
    return importlib.util.find_spec('serial') is not None


def _serial_probe(port: str) -> None:
    module = __import__('serial')
    ser = module.Serial(port, baudrate=mac2.DEFAULT_BAUDRATE, timeout=0.2, write_timeout=0.2)
    ser.close()


def run_preflight(manifest_path: pathlib.Path, package_root: pathlib.Path, serial_port: str,
                  manifest_sha256: Optional[str] = None) -> Dict[str, Any]:
    manifest = load_manifest(manifest_path, manifest_sha256)
    verified = verify_package_files(manifest, package_root)
    frozen = verify_frozen(manifest, package_root)
    openfpgaloader = shutil.which('openFPGALoader')
    if not openfpgaloader:
        raise mac2.PreflightError('openFPGALoader not found in PATH')
    if not _pyserial_available():
        raise mac2.PreflightError('pyserial is not installed')
    try:
        _serial_probe(serial_port)
    except Exception as exc:
        raise mac2.PreflightError('serial ' + serial_port + ' is not free: ' + repr(exc))
    software = manifest['software']
    return {
        'schema': PREFLIGHT_SCHEMA,
        'manifest_path': str(pathlib.Path(manifest_path).resolve()),
        'manifest_schema': manifest['schema'],
        'kernel_release': software['kernel_release'],
        'cli_sha256': software['cli_sha256'],
        'board_cli_path': software['board_cli_path'],
        'verified_files': verified,
        'openfpgaloader': openfpgaloader,
        'serial_port': serial_port,
        'serial_free': True,
        'pyserial': True,
        'role': ROLE,
        'expected_features': mac2.EXPECTED_FEATURES[ROLE],
        'plan_sha256': frozen['plan']['plan_sha256'],
        'selection_sha256': frozen['plan']['selection_sha256'],
        'cases': frozen['plan']['count'],
        'cases_per_boot': CASES_PER_BOOT,
        'board_commands_run': 0,
    }


# ---------------------------------------------------------------------------
# BusyBox archive console
# ---------------------------------------------------------------------------
def _busybox_tar_command(command: str) -> Optional[str]:
    '''Return a BusyBox-safe replacement for an exact ``tar -czf`` command.

    The command the runner emits is exactly six tokens:
    ``tar -czf <target> -C <source> .``.  Anything else starting with
    ``tar -czf`` is refused instead of guessed at.  The replacement never uses
    ``-z``, guards against overwriting either the intermediate plain tar or the
    final archive, and keeps every argument shell-quoted.
    '''
    if not command.startswith('tar -czf '):
        return None
    parts = shlex.split(command)
    if (len(parts) != 6 or parts[0] != 'tar' or parts[1] != '-czf'
            or parts[3] != '-C' or parts[5] != '.'):
        raise mac2.TechnicalFailure('unexpected tar -czf command: ' + repr(command))
    target = parts[2]
    source = parts[4]
    plain = target + '.plain.tar'
    return ('test ! -e ' + shlex.quote(plain)
            + ' && test ! -e ' + shlex.quote(target)
            + ' && tar -cf ' + shlex.quote(plain)
            + ' -C ' + shlex.quote(source)
            + ' . && gzip -c ' + shlex.quote(plain)
            + ' > ' + shlex.quote(target))


class Method88Console(mac2.SerialConsole):
    '''SerialConsole that keeps the framed cwd/exit-var behaviour but rewrites
    the one archive command the board BusyBox tar can not run.'''

    def run_shell(self, command: str, timeout: float, cwd: Optional[str] = None,
                  exit_var: Optional[str] = None) -> mac2.ShellResult:
        replacement = _busybox_tar_command(command)
        if replacement is not None:
            self._log_event('busybox archive: ' + replacement)
            command = replacement
        return super().run_shell(command, timeout, cwd=cwd, exit_var=exit_var)


# ---------------------------------------------------------------------------
# Runner
# ---------------------------------------------------------------------------
class Method88Runner(mac2.Research2Runner):
    '''Thin subclass that runs the frozen 88-case plan 44 / 44 over two fresh
    boots of the *same* role-2 BIT.'''

    def __init__(self, args, manifest: Dict[str, Any], package_root: pathlib.Path,
                 out_dir: pathlib.Path, console, programmer, plan_module, results_module,
                 frozen_plan: Dict[str, Any], frozen_selection: Dict[str, Any],
                 manifest_sha256: Optional[str] = None):
        super().__init__(args, manifest, package_root, out_dir, console, programmer,
                         plan_module, results_module, manifest_sha256=manifest_sha256)
        self.frozen_plan = frozen_plan
        self.frozen_selection = frozen_selection
        self.plan_boot: Optional[int] = None
        self.boots: Dict[int, Dict[str, Any]] = {}
        self.flash_records: Dict[int, Dict[str, Any]] = {}

    # -- metadata boot stamp -------------------------------------------------
    def _write_json(self, rel, obj) -> None:
        text = str(rel)
        if (isinstance(obj, dict) and obj.get('schema') == mac2.METADATA_SCHEMA
                and text.endswith('/metadata.json')):
            obj = dict(obj)
            obj['boot'] = self.plan_boot
        super()._write_json(rel, obj)

    # -- capture gate --------------------------------------------------------
    def _capture_state(self, board_dir: str, tag: str) -> None:
        super()._capture_state(board_dir, tag)
        if tag != 'before':
            return
        res = self.console.run_shell('cat ' + shlex.quote('taint.before'),
                                     timeout=self.args.command_timeout, cwd=board_dir)
        if res.exit_code != 0:
            raise mac2.TechnicalFailure('cannot read taint.before: exit=' + str(res.exit_code))
        value = mac2._text(res.stdout).strip()
        if value != '0':
            raise mac2.TechnicalFailure('taint before is not 0: ' + repr(value))

    # -- flash ---------------------------------------------------------------
    def _flash(self, image: Dict[str, Any]) -> None:
        # The reader thread is already running; drain whatever the previous boot
        # left in the frame buffer before the JTAG reload resets the board.
        self.console.drain_input()
        bit_path = mac2.resolve_package_path(self.package_root, image['bit'])
        result = self.programmer.flash(str(bit_path))
        self.flash_records[self.plan_boot] = result
        self._write_json('method/boots/' + str(self.plan_boot) + '.flash.json', result)

    # -- login / fresh boot --------------------------------------------------
    def _wait_for_root(self, timeout: float) -> None:
        '''Wait for fresh boot evidence plus a root shell, then fall back to the
        delivered harmless framed ``true`` readiness probe.'''
        deadline = time.monotonic() + timeout
        cursor = 0
        attempts = 0
        fresh_boot = False
        while time.monotonic() < deadline:
            with self.console._cond:
                data = bytes(self.console._buffer)
                recent = data[cursor:]
                if b'Linux version ' in data or b'login:' in data:
                    fresh_boot = True
                if fresh_boot and re.search(rb'(?m)^[#\$]\s*$', recent):
                    break
                prompt = re.search(rb'(?:login:|Password:)\s*$', recent)
                if not prompt:
                    self.console._cond.wait(timeout=min(1.0, max(0.01, deadline - time.monotonic())))
                    continue
                attempts += 1
                if attempts > 6:
                    raise mac2.TechnicalFailure('root login failed after repeated prompts')
                cursor = len(data)
            self.console._write(b'root' + bytes([13]))
        else:
            raise mac2.TechnicalFailure('root login startup timeout after ' + str(timeout) + 's')
        remaining = deadline - time.monotonic()
        if remaining <= 0:
            raise mac2.TechnicalFailure('root readiness startup timeout')
        return super()._wait_for_root(remaining)

    # -- boots ---------------------------------------------------------------
    def _record_boot(self, boot_number: int, info: Dict[str, Any]) -> None:
        boot_id = info.get('boot_id')
        for other, previous in self.boots.items():
            if previous.get('boot_id') == boot_id:
                raise mac2.TechnicalFailure(
                    'boot_id did not change between boots: boot ' + str(other)
                    + ' and boot ' + str(boot_number) + ' both ' + repr(boot_id))
        self.boots[boot_number] = info

    def _copy_frozen(self) -> None:
        frozen_dir = mac2.resolve_package_path(self.package_root, self.manifest['frozen']['root'])
        for name in FROZEN_FILES:
            shutil.copyfile(frozen_dir / name, self.stage_dir / name)

    def _run_boot(self, boot_number: int, cases: List[Dict[str, Any]]) -> None:
        self.plan_boot = boot_number
        image = self._image_for_role(ROLE)
        self._log('boot ' + str(boot_number) + ': flashing role ' + str(ROLE) + ' ' + str(image['bit']))
        self._flash(image)
        self.flashed_roles.append(ROLE)
        self._wait_for_root(self.args.startup_timeout)
        info = self._collect_boot_info(ROLE)
        self._record_boot(boot_number, info)
        self.boot = info
        self._write_json('method/boots/' + str(boot_number) + '.json', info)
        for case in cases:
            self.current_case = case.get('id')
            self._run_one_case('method', ROLE, case)

    # -- top level -----------------------------------------------------------
    def run(self) -> int:
        self.out_dir.mkdir(parents=True, exist_ok=False)
        software = self.manifest.get('software') or {}
        self._write_json('run.json', {
            'schema': RUN_SCHEMA,
            'run_id': self.run_id,
            'started_at': mac2.utc_now(),
            'stage': 'method',
            'role': ROLE,
            'manifest_schema': self.manifest.get('schema'),
            'manifest_sha256': self.manifest_sha256,
            'kernel_release': software.get('kernel_release'),
            'cli_sha256': software.get('cli_sha256'),
            'board_cli': self.board_cli,
            'board_root': self.board_root,
        })
        try:
            self.console.start()
            self.current_stage = 'method'
            self.stage_dir = self.out_dir / 'method'
            self.stage_dir.mkdir(parents=True, exist_ok=False)
            self._copy_frozen()

            cases = self.frozen_plan.get('cases')
            if not isinstance(cases, list):
                raise mac2.TechnicalFailure('the frozen plan has no case list')
            boot1 = [c for c in cases if int(c.get('boot', 0)) == 1]
            boot2 = [c for c in cases if int(c.get('boot', 0)) == 2]
            if len(boot1) != CASES_PER_BOOT or len(boot2) != CASES_PER_BOOT:
                raise mac2.TechnicalFailure(
                    'the frozen plan must hold ' + str(CASES_PER_BOOT) + ' cases per boot, got '
                    + str(len(boot1)) + ' and ' + str(len(boot2)))
            if len(boot1) + len(boot2) != TOTAL_CASES:
                raise mac2.TechnicalFailure('the frozen plan must hold ' + str(TOTAL_CASES) + ' cases')

            self._run_boot(1, boot1)
            self._verify_stage_saved('method', boot1)
            # Only after the first 44 cases are fully saved and hash-verified may
            # the board be synced and the same BIT flashed again.
            self._sync()
            self._run_boot(2, boot2)
            self._verify_stage_saved('method', boot2)

            self._write_json('COMPLETE.json', {
                'schema': COMPLETE_SCHEMA,
                'finished_at': mac2.utc_now(),
                'stages': ['method'],
                'role': ROLE,
                'completed_cases': [{'stage': s, 'id': c} for s, c in self.completed_cases],
                'boots': {str(b): info.get('boot_id') for b, info in sorted(self.boots.items())},
                'cases_per_boot': {str(b): len(group) for b, group in ((1, boot1), (2, boot2))},
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


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------
def build_arg_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description='Mac host two-boot role-2 method88 runner')
    p.add_argument('--manifest', required=True)
    p.add_argument('--package-root', default=None)
    p.add_argument('--out', default=None)
    p.add_argument('--serial', required=True)
    p.add_argument('--preflight', action='store_true')
    p.add_argument('--manifest-sha256', default=None)
    p.add_argument('--startup-timeout', type=float, default=mac2.DEFAULT_STARTUP_TIMEOUT)
    p.add_argument('--command-timeout', type=float, default=mac2.DEFAULT_COMMAND_TIMEOUT)
    p.add_argument('--case-timeout', type=float, default=mac2.DEFAULT_CASE_TIMEOUT)
    p.add_argument('--transfer-timeout', type=float, default=mac2.DEFAULT_TRANSFER_TIMEOUT)
    p.add_argument('--jtag-freq', type=int, default=1000000)
    return p


def main(argv=None) -> int:
    args = build_arg_parser().parse_args(argv)
    manifest_path = pathlib.Path(args.manifest)
    package_root = pathlib.Path(args.package_root) if args.package_root else manifest_path.parent
    if args.preflight:
        try:
            result = run_preflight(manifest_path, package_root, args.serial, args.manifest_sha256)
        except (mac2.PreflightError, mac2.ManifestError) as exc:
            print('mac_method88: preflight failed: ' + str(exc), file=sys.stderr)
            return 2
        print(json.dumps(result, indent=2, ensure_ascii=False, sort_keys=True))
        return 0
    if not args.out:
        raise SystemExit('--out is required unless --preflight')
    out_dir = pathlib.Path(args.out)
    if out_dir.exists():
        raise SystemExit('--out already exists: ' + str(out_dir))
    # No --stage, no --resume, no selection tuning: the stage is always method.
    args.stage = 'method'
    manifest = load_manifest(manifest_path, args.manifest_sha256)
    verify_package_files(manifest, package_root)
    frozen = verify_frozen(manifest, package_root)
    manifest_sha256 = mac2.sha256_file(manifest_path)
    console = Method88Console(args.serial, out_dir / 'console.raw.log',
                              out_dir / 'console.events.log')
    programmer = mac2.OpenFPGALoaderProgrammer(freq=args.jtag_freq)
    runner = Method88Runner(args, manifest, package_root, out_dir, console, programmer,
                            method88, results2,
                            frozen_plan=frozen['plan'], frozen_selection=frozen['selection'],
                            manifest_sha256=manifest_sha256)
    return runner.run()


if __name__ == '__main__':
    sys.exit(main())
