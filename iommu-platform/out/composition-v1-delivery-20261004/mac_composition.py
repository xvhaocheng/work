#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
mac_composition.py - Mac host attach-only runner for the composition plan.

Attach-only: the operator has already swapped the SD card and the new BIT booted
on its own.  This runner never writes a BIT, never reboots and never changes the
SD card.  It reuses mac_research2.SerialConsole for the framed serial link and
does not depend on the old runner's hardcoded kernel gate.

Without --run the serial port is never opened; --plan only writes the plan
manifest and exits.  The full plan is always generated before any case runs.
There is no automatic resume and no automatic rerun.  A technical failure stops
the run immediately and writes STOPPED.txt.

The first phase is short, then noise (the S/S6 long pairs), then explore.  Only
short + noise technical acceptance gates the exploration; an exploration result
alone never establishes a mechanism or a benefit.
"""
from __future__ import annotations

import argparse
import importlib.util
import json
import pathlib
import re
import secrets
import shlex
import subprocess
import sys
import time

DEFAULT_SERIAL = '/dev/cu.usbserial-AV0KRCHT'
DEFAULT_BOARD_CLI = '/usr/bin/idma_composition'
DEFAULT_DEVICE = '/dev/idma0'
DEFAULT_BOOT_ID_PATH = '/proc/sys/kernel/random/boot_id'
DEFAULT_TAINT_PATH = '/proc/sys/kernel/tainted'

DEFAULT_ATTACH_TIMEOUT = 30.0
DEFAULT_COMMAND_TIMEOUT = 60.0
DEFAULT_CASE_TIMEOUT = 300.0
DEFAULT_DMESG_TIMEOUT = 60.0

PHASES = ('short', 'noise', 'explore')

MANIFEST_SCHEMA = 'composition-manifest-v1'
SUMMARY_SCHEMA = 'composition-summary-v1'
CASE_META_SCHEMA = 'composition-case-meta-v1'
TOOL_SCHEMA = 'idma-composition-tool-v1'

BOOT_ID_RE = re.compile(
    r'^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-'
    r'[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$')

NEWLINE = chr(10)


class CompositionError(Exception):
    """Technical failure: stop after the current case evidence is kept."""


def utc_now():
    return time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())


def text_of(data):
    if isinstance(data, bytes):
        return data.decode('utf-8', errors='replace')
    return str(data)


def _load_module(name):
    here = pathlib.Path(__file__).resolve().parent
    path = here / (name + '.py')
    if not path.is_file():
        raise CompositionError('cannot find ' + name + '.py next to this runner')
    spec = importlib.util.spec_from_file_location('composition_host_' + name, path)
    if spec is None or spec.loader is None:
        raise CompositionError('cannot load ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules['composition_host_' + name] = module
    spec.loader.exec_module(module)
    return module


def load_plan_module():
    return _load_module('composition_plan')


def load_console_class():
    module = _load_module('mac_research2')
    cls = getattr(module, 'SerialConsole', None)
    if cls is None:
        raise CompositionError('mac_research2.py exposes no SerialConsole')
    for method in ('start', 'close', 'run_shell', 'drain_input', 'disable_echo'):
        if not callable(getattr(cls, method, None)):
            raise CompositionError('SerialConsole lacks ' + method)
    return cls


def write_json(path, obj):
    path = pathlib.Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + '.tmp')
    with open(tmp, 'w', encoding='utf-8') as handle:
        json.dump(obj, handle, ensure_ascii=False, indent=2, sort_keys=True)
        handle.write(NEWLINE)
    tmp.replace(path)


def check_uart_ownership(port):
    """Read-only exclusive check; never kills another process."""
    device = port if port.startswith('/dev/') else '/dev/' + port
    if not pathlib.Path(device).exists():
        raise CompositionError('serial device is not present: ' + device)
    try:
        proc = subprocess.run(['lsof', '--', device], stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE, check=False)
    except OSError as exc:
        raise CompositionError('lsof is not available: ' + repr(exc))
    if proc.returncode == 0 and proc.stdout.strip():
        raise CompositionError('UART ' + device + ' is already owned by another process:'
                               + NEWLINE + proc.stdout.decode('utf-8', errors='replace'))
    return device


class AttachRunner:
    def __init__(self, args, console, out_dir, plan_module):
        self.args = args
        self.console = console
        self.out_dir = pathlib.Path(out_dir)
        self.plan = plan_module
        self.board_cli = args.board_cli
        self.device = args.device
        self.expected_kernel = args.expected_kernel
        self.through = args.through_phase
        self.run_id = secrets.token_hex(8)
        self.board_root = '/tmp/composition-' + self.run_id
        self.identity = {}
        self.results = []
        self.current_case = None
        self.current_phase = None
        self.plan_total = 0
        self.stop_written = False

    def _log(self, message):
        line = utc_now() + ' ' + message
        try:
            with open(self.out_dir / 'run.log', 'a', encoding='utf-8') as handle:
                handle.write(line + NEWLINE)
        except OSError:
            pass
        print(line, file=sys.stderr)

    def _write_json(self, relative, obj):
        write_json(self.out_dir / relative, obj)

    def _stop(self, reason):
        if self.stop_written:
            return
        self.stop_written = True
        try:
            with open(self.out_dir / 'STOPPED.txt', 'w', encoding='utf-8') as handle:
                handle.write('stopped_at=' + utc_now() + NEWLINE)
                handle.write('reason=' + reason + NEWLINE)
                handle.write('phase=' + str(self.current_phase) + NEWLINE)
                handle.write('case=' + str(self.current_case) + NEWLINE)
        except OSError:
            pass

    def _board_dir(self, cid):
        return self.board_root + '/cases/' + cid

    def _wait_for_shell(self):
        deadline = time.monotonic() + self.args.attach_timeout
        last = None
        self.console.disable_echo()
        while time.monotonic() < deadline:
            budget = max(1.0, min(5.0, deadline - time.monotonic()))
            try:
                result = self.console.run_shell('true', timeout=budget)
                if result.exit_code == 0:
                    return
                last = 'probe exit=' + str(result.exit_code)
            except Exception as exc:
                last = repr(exc)
            try:
                self.console.drain_input()
                self.console.disable_echo()
            except Exception:
                pass
            time.sleep(1.0)
        raise CompositionError('root shell not reachable within attach timeout: ' + repr(last))

    def collect_identity(self):
        info = {'collected_at': utc_now()}
        result = self.console.run_shell('uname -r', timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise CompositionError('uname -r exit=' + str(result.exit_code))
        info['uname_r'] = text_of(result.stdout).strip()
        if info['uname_r'] != self.expected_kernel:
            raise CompositionError('kernel mismatch: ' + repr(info['uname_r'])
                                   + ' != expected ' + repr(self.expected_kernel))

        result = self.console.run_shell('cat ' + DEFAULT_BOOT_ID_PATH,
                                        timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise CompositionError('boot_id read exit=' + str(result.exit_code))
        info['boot_id'] = text_of(result.stdout).strip()
        if not BOOT_ID_RE.fullmatch(info['boot_id']):
            raise CompositionError('boot_id is not a UUID: ' + repr(info['boot_id']))

        result = self.console.run_shell('cat ' + DEFAULT_TAINT_PATH,
                                        timeout=self.args.command_timeout)
        if result.exit_code != 0:
            raise CompositionError('tainted read exit=' + str(result.exit_code))
        info['taint'] = text_of(result.stdout).strip()
        if info['taint'] != '0':
            raise CompositionError('kernel taint at attach is not 0: ' + repr(info['taint']))

        result = self.console.run_shell(shlex.quote(self.board_cli) + ' --abi',
                                        timeout=self.args.command_timeout)
        info['tool_abi_exit'] = result.exit_code
        info['tool_abi_raw'] = text_of(result.stdout)
        if result.exit_code != 0:
            raise CompositionError('board CLI --abi exit=' + str(result.exit_code)
                                   + ' raw=' + repr(info['tool_abi_raw'][:300]))
        try:
            abi = json.loads(info['tool_abi_raw'])
        except ValueError as exc:
            raise CompositionError('board CLI --abi is not JSON: ' + repr(exc))
        info['tool_abi'] = abi
        if abi.get('schema') != TOOL_SCHEMA:
            raise CompositionError('board CLI --abi schema ' + repr(abi.get('schema')))
        if abi.get('record_schema') != 'idma-composition-v1':
            raise CompositionError('board CLI record_schema ' + repr(abi.get('record_schema')))
        if abi.get('composition_abi') != 1:
            raise CompositionError('board CLI composition_abi ' + repr(abi.get('composition_abi')))
        return info

    def visit_dmesg(self, name):
        try:
            result = self.console.run_shell('dmesg', timeout=DEFAULT_DMESG_TIMEOUT)
        except Exception as exc:
            self._log('dmesg ' + name + ' raised: ' + repr(exc))
            return
        if result.exit_code != 0:
            self._log('dmesg ' + name + ' exit=' + str(result.exit_code))
            return
        (self.out_dir / name).write_bytes(result.stdout)

    def _cat(self, bdir, name):
        try:
            result = self.console.run_shell('cat ' + shlex.quote(name),
                                            timeout=self.args.command_timeout, cwd=bdir)
        except Exception as exc:
            return None, None, repr(exc)
        if result.exit_code != 0:
            return None, None, 'exit=' + str(result.exit_code)
        return text_of(result.stdout), result.stdout, None

    def run_case(self, case):
        cid = case['id']
        self.current_case = cid
        started = utc_now()
        argv = self.plan.case_argv(case, self.device)
        cli = shlex.quote(self.board_cli) + ' ' + ' '.join(shlex.quote(a) for a in argv)
        bdir = self._board_dir(cid)
        rdir = self.out_dir / 'cases' / cid
        rdir.mkdir(parents=True, exist_ok=False)

        mkdir = self.console.run_shell('mkdir -p ' + shlex.quote(bdir),
                                       timeout=self.args.command_timeout)
        if mkdir.exit_code != 0:
            raise CompositionError('case ' + cid + ': cannot create the board case directory')

        before = self.console.run_shell('cat ' + DEFAULT_TAINT_PATH,
                                        timeout=self.args.command_timeout)
        taint_before = text_of(before.stdout).strip() if before.exit_code == 0 else None

        inner = (cli + ' 2> stderr.txt; ec=$?; echo $ec > exit.txt; '
                 + 'cat ' + DEFAULT_TAINT_PATH + ' > taint.after; '
                 + 'cat ' + DEFAULT_BOOT_ID_PATH + ' > boot.after')
        framed_exit = None
        framed_error = None
        record_raw = b''
        try:
            result = self.console.run_shell(inner, timeout=self.args.case_timeout,
                                            cwd=bdir, exit_var='ec')
            framed_exit = result.exit_code
            record_raw = result.stdout
        except Exception as exc:
            framed_error = exc

        stderr_text, stderr_bytes, stderr_err = self._cat(bdir, 'stderr.txt')
        exit_text, _, exit_err = self._cat(bdir, 'exit.txt')
        taint_after_text, _, taint_after_err = self._cat(bdir, 'taint.after')
        boot_after_text, _, boot_after_err = self._cat(bdir, 'boot.after')

        taint_after = taint_after_text.strip() if taint_after_text is not None else None
        boot_after = boot_after_text.strip() if boot_after_text is not None else None
        exit_value = None
        if exit_text is not None:
            stripped = exit_text.strip()
            if stripped and all(ch in '0123456789-' for ch in stripped):
                try:
                    exit_value = int(stripped)
                except ValueError:
                    exit_value = None

        with open(rdir / 'record.raw.json', 'wb') as handle:
            handle.write(record_raw)
        if stderr_bytes is not None:
            with open(rdir / 'stderr.txt', 'wb') as handle:
                handle.write(stderr_bytes)

        record = None
        problems = []
        if framed_error is not None:
            problems.append('framed command raised: ' + repr(framed_error))
        try:
            record = json.loads(record_raw.decode('utf-8', errors='replace').strip())
        except ValueError as exc:
            problems.append('CLI stdout is not JSON: ' + repr(exc))
        if isinstance(record, dict):
            write_json(rdir / 'record.json', record)
        if stderr_err:
            problems.append('stderr.txt read: ' + stderr_err)
        if exit_err:
            problems.append('exit.txt read: ' + exit_err)
        if taint_after_err:
            problems.append('taint.after read: ' + taint_after_err)
        if boot_after_err:
            problems.append('boot.after read: ' + boot_after_err)
        if taint_before != '0':
            problems.append('taint before=' + repr(taint_before))
        if taint_after != '0':
            problems.append('taint after=' + repr(taint_after))
        if boot_after != self.identity.get('boot_id'):
            problems.append('boot id changed: ' + repr(boot_after))
        if framed_exit != 0:
            problems.append('CLI framed exit ' + repr(framed_exit))
        if exit_value is not None and framed_exit is not None and exit_value != framed_exit:
            problems.append('exit.txt ' + str(exit_value) + ' != framed exit '
                            + str(framed_exit))
        if isinstance(record, dict):
            try:
                problems.extend(self.plan.validate_case(case, record, meta={
                    'exit': framed_exit,
                    'taint_before': taint_before,
                    'taint_after': taint_after,
                }))
            except Exception as exc:
                problems.append('validate_case raised: ' + repr(exc))
        else:
            problems.append('record is not a JSON object')

        metrics = None
        if isinstance(record, dict):
            try:
                metrics = self.plan.case_metrics(case, record)
                write_json(rdir / 'metrics.json', metrics)
            except Exception as exc:
                problems.append('case_metrics raised: ' + repr(exc))

        meta = {
            'schema': CASE_META_SCHEMA,
            'id': cid,
            'phase': case['phase'],
            'block': case['block'],
            'pair': case['pair'],
            'slot': case['slot'],
            'mode': case['mode'],
            'a_pages': case['a_pages'],
            'v_gap_cycles': case['v_gap_cycles'],
            'transfers': case['transfers'],
            'warmup': case['warmup'],
            'argv': argv,
            'command': cli,
            'framed_exit': framed_exit,
            'exit': exit_value,
            'taint_before': taint_before,
            'taint_after': taint_after,
            'boot_id': self.identity.get('boot_id'),
            'boot_after': boot_after,
            'uname_r': self.identity.get('uname_r'),
            'board_cli': self.board_cli,
            'board_cli_abi': self.identity.get('tool_abi'),
            'started_at': started,
            'finished_at': utc_now(),
            'problems': problems,
        }
        write_json(rdir / 'meta.json', meta)

        self.results.append({
            'id': cid,
            'phase': case['phase'],
            'mode': case['mode'],
            'a_pages': case['a_pages'],
            'v_gap_cycles': case['v_gap_cycles'],
            'status': 'completed' if not problems else 'failed',
            'exit': framed_exit,
            'taint_after': taint_after,
            'dir': 'cases/' + cid,
            'v_window_cycles': (metrics or {}).get('v_window_cycles'),
            'a_completed_jobs_window': (metrics or {}).get('a_completed_jobs_window'),
            'problems': problems,
        })
        self.write_summary('running')

        if problems:
            raise CompositionError('case ' + cid + ': ' + '; '.join(problems))
        self._log('case ' + cid + ' completed')

    def write_summary(self, status):
        completed = sum(1 for entry in self.results if entry['status'] == 'completed')
        failed = sum(1 for entry in self.results if entry['status'] == 'failed')
        notes = [
            'attach-only run: no BIT was written, no reboot and no SD change',
            'old HPM fields are raw_only, not a validated banked-mode metric',
            'fast == 0 and miss == 0 are scientific negatives, never technical failures',
            'exploration alone does not establish a mechanism or a benefit',
            'no automatic resume and no automatic rerun',
        ]
        self._write_json('summary.json', {
            'schema': SUMMARY_SCHEMA,
            'run_id': self.run_id,
            'status': status,
            'through_phase': self.through,
            'updated_at': utc_now(),
            'planned': self.plan_total,
            'attempted': len(self.results),
            'completed': completed,
            'failed': failed,
            'cases': self.results,
            'notes': notes,
        })

    def _phase_report(self, phase):
        entries = [entry for entry in self.results if entry['phase'] == phase]
        completed = sum(1 for entry in entries if entry['status'] == 'completed')
        failed = sum(1 for entry in entries if entry['status'] == 'failed')
        self._log('phase ' + phase + ': completed=' + str(completed)
                  + ' failed=' + str(failed))

    def run(self):
        # main() is the single owner of the out directory.  Re-creating it here
        # with exist_ok=False made every --run invocation raise FileExistsError
        # right after main() had already created the same directory.
        plan = self.plan.build_plan()
        self._write_json('plan.json', plan)
        self.plan_total = plan['count']
        self._write_json('manifest.json', {
            'schema': MANIFEST_SCHEMA,
            'run_id': self.run_id,
            'mode': 'attach-only',
            'auto_rerun': False,
            'resume': False,
            'through_phase': self.through,
            'expected_kernel': self.expected_kernel,
            'board_cli': self.board_cli,
            'device': self.device,
            'serial': self.args.serial,
            'plan_schema': plan['schema'],
            'plan_generation': plan['plan_generation'],
            'planned': plan['count'],
            'phases': plan['phases'],
            'created_at': utc_now(),
        })
        try:
            self._wait_for_shell()
            self.identity = self.collect_identity()
            self._write_json('boot.json', self.identity)
            self._log('attached uname=' + repr(self.identity.get('uname_r'))
                      + ' boot_id=' + repr(self.identity.get('boot_id')))
            self.visit_dmesg('dmesg.before.txt')
            selected = self.plan.cases_through_phase(plan, self.through)
            for phase in PHASES:
                phase_cases = [case for case in selected if case['phase'] == phase]
                if not phase_cases:
                    continue
                self.current_phase = phase
                for case in phase_cases:
                    self.run_case(case)
                self.visit_dmesg('dmesg.' + phase + '.end.txt')
                self._phase_report(phase)
                self.write_summary('running')
            self.write_summary('complete')
            return 0
        except KeyboardInterrupt:
            self._stop('KeyboardInterrupt')
            self.visit_dmesg('dmesg.failure.txt')
            self.write_summary('interrupted')
            raise
        except Exception as exc:
            self._stop(type(exc).__name__ + ': ' + str(exc))
            self.visit_dmesg('dmesg.failure.txt')
            self.write_summary('stopped')
            raise


def build_arg_parser():
    parser = argparse.ArgumentParser(
        description='Mac host attach-only runner for the composition plan')
    parser.add_argument('--serial', default=DEFAULT_SERIAL)
    parser.add_argument('--out', required=True)
    parser.add_argument('--expected-kernel', required=True,
                        help='required: the kernel release the builder actually shipped')
    parser.add_argument('--board-cli', default=DEFAULT_BOARD_CLI)
    parser.add_argument('--device', default=DEFAULT_DEVICE)
    parser.add_argument('--plan', action='store_true',
                        help='write the plan manifest and exit; never opens the serial port')
    parser.add_argument('--run', action='store_true',
                        help='access the serial port and execute; without it no serial access')
    parser.add_argument('--through-phase', choices=PHASES, default='short')
    parser.add_argument('--attach-timeout', type=float, default=DEFAULT_ATTACH_TIMEOUT)
    parser.add_argument('--command-timeout', type=float, default=DEFAULT_COMMAND_TIMEOUT)
    parser.add_argument('--case-timeout', type=float, default=DEFAULT_CASE_TIMEOUT)
    return parser


def write_plan_only(out_dir, plan_module, args):
    out_dir.mkdir(parents=True, exist_ok=False)
    plan = plan_module.build_plan()
    write_json(out_dir / 'plan.json', plan)
    write_json(out_dir / 'manifest.json', {
        'schema': MANIFEST_SCHEMA,
        'mode': 'attach-only',
        'auto_rerun': False,
        'resume': False,
        'through_phase': args.through_phase,
        'expected_kernel': args.expected_kernel,
        'board_cli': args.board_cli,
        'device': args.device,
        'serial': args.serial,
        'plan_schema': plan['schema'],
        'plan_generation': plan['plan_generation'],
        'planned': plan['count'],
        'phases': plan['phases'],
        'created_at': utc_now(),
    })
    print('plan written: ' + str(out_dir / 'plan.json')
          + ' (' + str(plan['count']) + ' cases)')
    return 0


def main(argv=None):
    args = build_arg_parser().parse_args(argv)
    out_dir = pathlib.Path(args.out)
    plan_module = load_plan_module()
    if args.plan or not args.run:
        if out_dir.exists():
            raise SystemExit('--out already exists: ' + str(out_dir))
        return write_plan_only(out_dir, plan_module, args)
    if out_dir.exists():
        raise SystemExit('--out already exists: ' + str(out_dir))
    check_uart_ownership(args.serial)
    console_class = load_console_class()
    # main() creates --out exactly once; an existing directory is always
    # refused above, so no old output is ever overwritten or resumed.
    out_dir.mkdir(parents=True, exist_ok=False)
    console = console_class(args.serial, out_dir / 'console.raw.log',
                            out_dir / 'console.events.log')
    try:
        console.start()
        runner = AttachRunner(args, console, out_dir, plan_module)
        return runner.run()
    finally:
        try:
            console.close()
        except Exception:
            pass


if __name__ == '__main__':
    sys.exit(main())
