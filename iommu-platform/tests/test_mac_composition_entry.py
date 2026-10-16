#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Entry-point tests for scripts/mac_composition.py.

No hardware and no serial port.  The real mac_composition.main() entry point
and the real AttachRunner.run() loop are executed; only the six boundaries that
would leave the host are replaced:

  * check_uart_ownership           (lsof on the real serial device)
  * load_console_class             (imports mac_research2 and opens the UART)
  * AttachRunner._wait_for_shell   (board shell probe)
  * AttachRunner.collect_identity  (uname/boot_id/taint/board CLI --abi)
  * AttachRunner.visit_dmesg       (dmesg over the board shell)
  * AttachRunner.run_case          (the per-case board command)

Keeping AttachRunner.run() real is the point of this file: main() creates --out
once and run() must not create it a second time, because the old
self.out_dir.mkdir(parents=True, exist_ok=False) inside run() always raised
FileExistsError right after main() had already created the directory.
"""
from __future__ import annotations

import contextlib
import importlib.util
import json
import pathlib
import sys
import tempfile
import unittest
from unittest import mock

HERE = pathlib.Path(__file__).resolve()
REPO = HERE.parent.parent
SCRIPT = REPO / 'scripts' / 'mac_composition.py'

EXPECTED_KERNEL = '6.1.0-idma-contract'
BOOT_ID = '01234567-89ab-cdef-0123-456789abcdef'
SERIAL = '/dev/cu.fake-idma-entry'


def load_mac_module():
    spec = importlib.util.spec_from_file_location('mac_composition_entry', SCRIPT)
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load ' + str(SCRIPT))
    module = importlib.util.module_from_spec(spec)
    sys.modules['mac_composition_entry'] = module
    spec.loader.exec_module(module)
    return module


mac = load_mac_module()


class FakeConsole:
    """Records start/close only; it never opens a real serial port."""

    instances = []
    fail_start = False

    def __init__(self, serial, raw_path, events_path):
        self.serial = serial
        self.raw_path = pathlib.Path(raw_path)
        self.events_path = pathlib.Path(events_path)
        self.started = 0
        self.closed = 0
        FakeConsole.instances.append(self)

    def start(self):
        if type(self).fail_start:
            raise RuntimeError('fake console start failed')
        self.started += 1
        self.raw_path.write_bytes(b'')
        self.events_path.write_bytes(b'')

    def close(self):
        self.closed += 1

    def disable_echo(self):
        raise AssertionError('the board must not be touched in this test')

    def drain_input(self):
        raise AssertionError('the board must not be touched in this test')

    def run_shell(self, *args, **kwargs):
        raise AssertionError('the board must not be touched in this test')


STATE = {
    'wait': 0,
    'identity': 0,
    'dmesg': [],
    'cases': [],
    'manifest_at_case': [],
    'plan_at_case': [],
    'fail_first_case': False,
}


def reset_state():
    STATE['wait'] = 0
    STATE['identity'] = 0
    STATE['dmesg'] = []
    STATE['cases'] = []
    STATE['manifest_at_case'] = []
    STATE['plan_at_case'] = []
    STATE['fail_first_case'] = False


def fake_wait_for_shell(self):
    STATE['wait'] += 1


def fake_collect_identity(self):
    STATE['identity'] += 1
    return {
        'collected_at': mac.utc_now(),
        'uname_r': self.expected_kernel,
        'boot_id': BOOT_ID,
        'taint': '0',
        'tool_abi_exit': 0,
        'tool_abi_raw': '{}',
        'tool_abi': {'schema': mac.TOOL_SCHEMA},
    }


def fake_visit_dmesg(self, name):
    STATE['dmesg'].append(name)


def fake_run_case(self, case):
    STATE['cases'].append(case['id'])
    STATE['manifest_at_case'].append((self.out_dir / 'manifest.json').exists())
    STATE['plan_at_case'].append((self.out_dir / 'plan.json').exists())
    if STATE['fail_first_case'] and len(STATE['cases']) == 1:
        self.results.append({
            'id': case['id'],
            'phase': case['phase'],
            'mode': case['mode'],
            'a_pages': case['a_pages'],
            'v_gap_cycles': case['v_gap_cycles'],
            'status': 'failed',
            'exit': None,
            'taint_after': None,
            'dir': 'cases/' + case['id'],
            'v_window_cycles': None,
            'a_completed_jobs_window': None,
            'problems': ['simulated technical failure'],
        })
        self.write_summary('running')
        raise mac.CompositionError(
            'case ' + case['id'] + ': simulated technical failure')
    self.results.append({
        'id': case['id'],
        'phase': case['phase'],
        'mode': case['mode'],
        'a_pages': case['a_pages'],
        'v_gap_cycles': case['v_gap_cycles'],
        'status': 'completed',
        'exit': 0,
        'taint_after': '0',
        'dir': 'cases/' + case['id'],
        'v_window_cycles': None,
        'a_completed_jobs_window': None,
        'problems': [],
    })
    self.write_summary('running')


class MacCompositionEntryTests(unittest.TestCase):
    def setUp(self):
        reset_state()
        FakeConsole.instances = []
        FakeConsole.fail_start = False
        self._tmp = tempfile.TemporaryDirectory(prefix='idma-mac-entry-')
        self.tmp = pathlib.Path(self._tmp.name)
        self.addCleanup(self._tmp.cleanup)

    @contextlib.contextmanager
    def boundaries(self):
        """Replace every host-facing boundary; AttachRunner.run stays real."""
        with contextlib.ExitStack() as stack:
            self.uart_check = stack.enter_context(
                mock.patch.object(mac, 'check_uart_ownership', return_value=SERIAL))
            stack.enter_context(
                mock.patch.object(mac, 'load_console_class', return_value=FakeConsole))
            stack.enter_context(mock.patch.object(
                mac.AttachRunner, '_wait_for_shell', fake_wait_for_shell))
            stack.enter_context(mock.patch.object(
                mac.AttachRunner, 'collect_identity', fake_collect_identity))
            stack.enter_context(mock.patch.object(
                mac.AttachRunner, 'visit_dmesg', fake_visit_dmesg))
            stack.enter_context(mock.patch.object(
                mac.AttachRunner, 'run_case', fake_run_case))
            yield

    def argv(self, out_dir, run=True, with_plan=False):
        argv = ['--out', str(out_dir),
                '--expected-kernel', EXPECTED_KERNEL,
                '--through-phase', 'short',
                '--serial', SERIAL]
        if with_plan:
            argv.append('--plan')
        if run:
            argv.append('--run')
        return argv

    def short_phase_ids(self):
        plan_module = mac.load_plan_module()
        plan = plan_module.build_plan()
        return plan, [case['id'] for case in
                      plan_module.cases_through_phase(plan, 'short')]

    def test_run_creates_out_once_and_runs_the_real_runner_loop(self):
        out_dir = self.tmp / 'run-new'
        plan, expected = self.short_phase_ids()
        self.assertEqual(len(expected), 16)
        with self.boundaries():
            rc = mac.main(self.argv(out_dir))
        self.assertEqual(rc, 0)
        self.uart_check.assert_called_once_with(SERIAL)
        self.assertEqual(len(FakeConsole.instances), 1)
        console = FakeConsole.instances[0]
        self.assertEqual(console.serial, SERIAL)
        self.assertEqual(console.started, 1)
        self.assertEqual(console.closed, 1)
        self.assertEqual(STATE['wait'], 1)
        self.assertEqual(STATE['identity'], 1)
        self.assertEqual(STATE['cases'], expected)
        self.assertTrue(all(STATE['manifest_at_case']))
        self.assertTrue(all(STATE['plan_at_case']))
        self.assertEqual(STATE['dmesg'],
                         ['dmesg.before.txt', 'dmesg.short.end.txt'])
        for name in ('plan.json', 'manifest.json', 'boot.json', 'summary.json',
                     'run.log'):
            self.assertTrue((out_dir / name).is_file(), name)
        manifest = json.loads((out_dir / 'manifest.json').read_text())
        self.assertEqual(manifest['schema'], mac.MANIFEST_SCHEMA)
        self.assertEqual(manifest['mode'], 'attach-only')
        self.assertIs(manifest['auto_rerun'], False)
        self.assertIs(manifest['resume'], False)
        self.assertEqual(manifest['through_phase'], 'short')
        self.assertEqual(manifest['expected_kernel'], EXPECTED_KERNEL)
        self.assertEqual(manifest['planned'], plan['count'])
        summary = json.loads((out_dir / 'summary.json').read_text())
        self.assertEqual(summary['schema'], mac.SUMMARY_SCHEMA)
        self.assertEqual(summary['status'], 'complete')
        self.assertEqual(summary['attempted'], 16)
        self.assertEqual(summary['completed'], 16)
        self.assertEqual(summary['failed'], 0)

    def test_first_technical_failure_stops_without_a_second_case(self):
        out_dir = self.tmp / 'run-fail'
        STATE['fail_first_case'] = True
        with self.boundaries():
            with self.assertRaises(mac.CompositionError):
                mac.main(self.argv(out_dir))
        self.assertEqual(len(STATE['cases']), 1)
        self.assertEqual(FakeConsole.instances[0].closed, 1)
        stopped = (out_dir / 'STOPPED.txt').read_text()
        self.assertIn('simulated technical failure', stopped)
        summary = json.loads((out_dir / 'summary.json').read_text())
        self.assertEqual(summary['status'], 'stopped')
        self.assertEqual(summary['attempted'], 1)
        self.assertEqual(summary['failed'], 1)
        self.assertIn('dmesg.failure.txt', STATE['dmesg'])

    def test_existing_out_is_refused_and_never_overwritten(self):
        out_dir = self.tmp / 'run-existing'
        out_dir.mkdir()
        marker = out_dir / 'plan.json'
        marker.write_text('{"old": true}')
        with self.boundaries():
            with self.assertRaises(SystemExit):
                mac.main(self.argv(out_dir))
        self.uart_check.assert_not_called()
        self.assertEqual(FakeConsole.instances, [])
        self.assertEqual(STATE['cases'], [])
        self.assertEqual(marker.read_text(), '{"old": true}')
        self.assertEqual([p.name for p in out_dir.iterdir()], ['plan.json'])

    def test_plan_only_writes_plan_and_manifest_without_serial(self):
        out_dir = self.tmp / 'plan-new'
        with self.boundaries():
            rc = mac.main(self.argv(out_dir, run=False, with_plan=True))
        self.assertEqual(rc, 0)
        self.uart_check.assert_not_called()
        self.assertEqual(FakeConsole.instances, [])
        self.assertEqual(STATE['cases'], [])
        plan = json.loads((out_dir / 'plan.json').read_text())
        manifest = json.loads((out_dir / 'manifest.json').read_text())
        self.assertEqual(manifest['schema'], mac.MANIFEST_SCHEMA)
        self.assertEqual(manifest['through_phase'], 'short')
        self.assertEqual(manifest['planned'], plan['count'])

    def test_console_start_failure_still_closes_the_console(self):
        out_dir = self.tmp / 'run-startfail'
        FakeConsole.fail_start = True
        with self.boundaries():
            with self.assertRaises(RuntimeError):
                mac.main(self.argv(out_dir))
        self.assertEqual(len(FakeConsole.instances), 1)
        self.assertEqual(FakeConsole.instances[0].started, 0)
        self.assertEqual(FakeConsole.instances[0].closed, 1)
        self.assertEqual(STATE['cases'], [])
        self.assertTrue(out_dir.is_dir())
        self.assertFalse((out_dir / 'manifest.json').exists())


if __name__ == '__main__':
    unittest.main()
