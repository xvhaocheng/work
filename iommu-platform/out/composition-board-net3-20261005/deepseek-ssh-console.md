## What this patch does (scope)

Only `scripts/mac_composition.py` changes, plus one new bounded test file. `composition_plan`, bench, driver, RTL, acceptance and the per‑case execution count are untouched. The full 172‑case loop still runs through the *same* `AttachRunner`; `main` still refuses an existing `--out`.

- Adds `--transport {uart,ssh}` (default `uart`), `--board-host`, `--credential-script`, `--ssh-helper`.
- SSH mode:
  - never calls `check_uart_ownership`, never opens a serial port;
  - loads the helper by explicit path and reads the login response into memory only;
  - builds a console class that **inherits `mac_research2.SerialConsole`**, reuses its `run_shell` frame builder unchanged, and overrides only `start / close / disable_echo / drain_input / _run_framed`;
  - runs the framed command through the helper’s existing `build_ssh_argv` + `default_ssh_runner` (password mode);
  - strips banner / password prompt by taking only the bytes between the unique random BEGIN/END markers, normalises CRLF, reads the real exit marker, and returns `ShellResult(stdout bytes, exit_code)`;
  - treats **ssh non‑zero rc, missing/duplicate/malformed markers, or a missing exit line as a hard failure**, and preserves a non‑zero command exit.
- Manifest (both `--run` and `--plan`) records `transport`, `board_host`, `ssh_helper`; in SSH mode `serial` is `None`.
- No host_ip / HTTP / download / card write / JTAG / reboot path is invoked.

---

## 1. Unified diff — `scripts/mac_composition.py`

```diff
--- a/scripts/mac_composition.py
+++ b/scripts/mac_composition.py
@@ -90,9 +90,118 @@
 def load_console_class():
-    module = _load_module('mac_research2')
-    cls = getattr(module, 'SerialConsole', None)
-    if cls is None:
-        raise CompositionError('mac_research2.py exposes no SerialConsole')
-    for method in ('start', 'close', 'run_shell', 'drain_input', 'disable_echo'):
-        if not callable(getattr(cls, method, None)):
-            raise CompositionError('SerialConsole lacks ' + method)
-    return cls
+    return _load_serial_module().SerialConsole
+
+
+def _load_serial_module():
+    module = _load_module('mac_research2')
+    cls = getattr(module, 'SerialConsole', None)
+    if cls is None:
+        raise CompositionError('mac_research2.py exposes no SerialConsole')
+    for method in ('start', 'close', 'run_shell', 'drain_input', 'disable_echo',
+                   'new_marker_parts', '_run_framed'):
+        if not callable(getattr(cls, method, None)):
+            raise CompositionError('SerialConsole lacks ' + method)
+    if getattr(module, 'ShellResult', None) is None:
+        raise CompositionError('mac_research2.py exposes no ShellResult')
+    return module
+
+
+def load_ssh_helper(path):
+    """Load the external ssh helper module by its explicit path."""
+    path = pathlib.Path(path).expanduser()
+    if not path.is_file():
+        raise CompositionError('ssh helper is not a local file: ' + str(path))
+    spec = importlib.util.spec_from_file_location('composition_host_ssh_helper', path)
+    if spec is None or spec.loader is None:
+        raise CompositionError('cannot load ssh helper ' + str(path))
+    module = importlib.util.module_from_spec(spec)
+    sys.modules['composition_host_ssh_helper'] = module
+    spec.loader.exec_module(module)
+    return module
+
+
+def transport_manifest(args):
+    """Manifest transport fields; the ssh transport records no serial port."""
+    if getattr(args, 'transport', 'uart') == 'ssh':
+        return {
+            'transport': 'ssh',
+            'serial': None,
+            'board_host': getattr(args, 'board_host', None),
+            'ssh_helper': getattr(args, 'ssh_helper', None),
+        }
+    return {
+        'transport': 'uart',
+        'serial': args.serial,
+        'board_host': None,
+        'ssh_helper': None,
+    }
+
+
+def build_ssh_console_class(serial_module, ssh_module):
+    """SSH transport console class.
+
+    It inherits the real mac_research2.SerialConsole and keeps its run_shell
+    frame builder untouched; only start / close / disable_echo / drain_input and
+    _run_framed are overridden so the framed command travels over the ssh
+    helper's existing password runner instead of a serial port.  No serial port
+    is opened, no credential is sent or logged here, and a missing or malformed
+    frame, or a non-zero ssh exit, is a hard failure.
+    """
+    serial_console_cls = serial_module.SerialConsole
+    shell_result_cls = serial_module.ShellResult
+    build_ssh_argv = ssh_module.build_ssh_argv
+    run_ssh = ssh_module.default_ssh_runner
+    newline = bytes([10])
+
+    class SshConsole(serial_console_cls):
+        def __init__(self, raw_log_path, events_log_path, board_host,
+                     credential_response):
+            super().__init__(None, raw_log_path, events_log_path)
+            self.board_host = board_host
+            self._credential_response = credential_response
+
+        def start(self, timeout=10.0):
+            self.raw_log_path.parent.mkdir(parents=True, exist_ok=True)
+            self.events_log_path.parent.mkdir(parents=True, exist_ok=True)
+            self._log_event('ssh console start host=' + str(self.board_host))
+
+        def close(self):
+            self._stop.set()
+
+        def disable_echo(self):
+            return
+
+        def drain_input(self):
+            return b''
+
+        def _run_framed(self, full_command, begin, end, timeout):
+            argv = build_ssh_argv(key=None, password_mode=True,
+                                  board_host=self.board_host,
+                                  remote_command=full_command)
+            self._log_event('ssh send host=' + str(self.board_host))
+            proc = run_ssh(argv, timeout, self._credential_response)
+            transcript = bytes(getattr(proc, 'stdout', b'') or b'')
+            returncode = getattr(proc, 'returncode', None)
+            if returncode != 0:
+                raise CompositionError('ssh transport failed rc=' + str(returncode))
+            begin_bytes = begin.encode('ascii')
+            end_bytes = end.encode('ascii')
+            if transcript.count(begin_bytes) != 1 or transcript.count(end_bytes) != 1:
+                raise CompositionError('ssh frame markers are missing or not unique')
+            begin_at = transcript.index(begin_bytes)
+            end_at = transcript.index(end_bytes)
+            if end_at <= begin_at:
+                raise CompositionError('ssh frame end marker precedes the begin marker')
+            data = transcript[begin_at + len(begin_bytes):end_at]
+            tail = transcript[end_at + len(end_bytes):]
+            line_end = tail.find(newline)
+            if line_end < 0:
+                raise CompositionError('ssh frame exit marker has no line end')
+            exit_text = tail[:line_end].decode('ascii', errors='replace').strip()
+            if not re.fullmatch(r'-?[0-9]+', exit_text):
+                raise CompositionError('ssh frame has a bad exit marker: ' + repr(exit_text))
+            data = data.replace(b'\r\n', b'\n')
+            self._log_event('ssh recv ' + end + ' exit=' + exit_text
+                            + ' bytes=' + str(len(data)))
+            return shell_result_cls(data, int(exit_text))
+
+    return SshConsole
@@ -453,17 +453,19 @@
-        self._write_json('manifest.json', {
+        manifest = {
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
-        })
+        }
+        manifest.update(transport_manifest(self.args))
+        self._write_json('manifest.json', manifest)
@@ -505,3 +505,14 @@
     parser.add_argument('--serial', default=DEFAULT_SERIAL)
+    parser.add_argument('--transport', choices=('uart', 'ssh'), default='uart',
+                        help='uart: framed serial console (default); ssh: framed console over '
+                             'the existing ssh password helper')
+    parser.add_argument('--board-host', dest='board_host', default=None,
+                        help='board ssh target; required with --transport ssh, never scanned')
+    parser.add_argument('--credential-script', dest='credential_script', default=None,
+                        help='existing Python script whose unique _write(bytes) literal is the '
+                             'board login response (required with --transport ssh)')
+    parser.add_argument('--ssh-helper', dest='ssh_helper', default=None,
+                        help='explicit path to the existing ssh helper module '
+                             '(mac_update_reboot.py) required with --transport ssh')
     parser.add_argument('--out', required=True)
@@ -526,16 +526,17 @@
-    write_json(out_dir / 'manifest.json', {
+    manifest = {
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
-    })
+    }
+    manifest.update(transport_manifest(args))
+    write_json(out_dir / 'manifest.json', manifest)
@@ -547,26 +547,52 @@
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
-    check_uart_ownership(args.serial)
-    console_class = load_console_class()
-    # main() creates --out exactly once; an existing directory is always
-    # refused above, so no old output is ever overwritten or resumed.
-    out_dir.mkdir(parents=True, exist_ok=False)
-    console = console_class(args.serial, out_dir / 'console.raw.log',
-                            out_dir / 'console.events.log')
+    if args.transport == 'ssh':
+        if not args.board_host:
+            raise SystemExit('--board-host is required with --transport ssh')
+        if not args.credential_script:
+            raise SystemExit('--credential-script is required with --transport ssh')
+        if not args.ssh_helper:
+            raise SystemExit('--ssh-helper is required with --transport ssh')
+        serial_module = _load_serial_module()
+        ssh_module = load_ssh_helper(args.ssh_helper)
+        for name in ('load_credential_response', 'build_ssh_argv',
+                     'default_ssh_runner'):
+            if not callable(getattr(ssh_module, name, None)):
+                raise SystemExit('ssh helper lacks ' + name)
+        # The existing login response is only read into memory here: it is never
+        # sent by this runner, never put in argv or the environment and never
+        # logged.  The helper's own runner injects it at the ssh prompt.
+        credential_response = ssh_module.load_credential_response(
+            args.credential_script)
+        ssh_console_class = build_ssh_console_class(serial_module, ssh_module)
+        # main() creates --out exactly once; an existing directory is always
+        # refused above, so no old output is ever overwritten or resumed.
+        out_dir.mkdir(parents=True, exist_ok=False)
+        console = ssh_console_class(out_dir / 'console.raw.log',
+                                    out_dir / 'console.events.log',
+                                    args.board_host, credential_response)
+    else:
+        check_uart_ownership(args.serial)
+        console_class = load_console_class()
+        # main() creates --out exactly once; an existing directory is always
+        # refused above, so no old output is ever overwritten or resumed.
+        out_dir.mkdir(parents=True, exist_ok=False)
+        console = console_class(args.serial, out_dir / 'console.raw.log',
+                                out_dir / 'console.events.log')
     try:
         console.start()
         runner = AttachRunner(args, console, out_dir, plan_module)
         return runner.run()
     finally:
         try:
             console.close()
         except Exception:
             pass
```

The rest of the file (`AttachRunner.run`, `_wait_for_shell`, `collect_identity`, `run_case`, etc.) is unchanged.

---

## 2. New test file — `tests/test_mac_composition_ssh.py`

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Bounded tests for the mac_composition.py --transport ssh console.

No hardware, no network and no real credential are used.  The real
mac_research2.SerialConsole frame builder is exercised through the real
build_ssh_console_class(); only the ssh process runner is replaced - either by
a local /bin/sh that runs the framed remote command, or by a canned
transcript.  The SSH frame parser is the production one; nothing here mirrors
it.
"""
from __future__ import annotations

import importlib.util
import json
import pathlib
import subprocess
import sys
import tempfile
import types
import unittest
from unittest import mock

HERE = pathlib.Path(__file__).resolve()
REPO = HERE.parent.parent
SCRIPTS = REPO / 'scripts'

EXPECTED_KERNEL = '6.1.0-idma-contract'
BOARD_HOST = 'board.local'
SERIAL = '/dev/cu.fake-idma-ssh'
CREDENTIAL = b'\x00fake-response\x00'


def _load(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


mac = _load(SCRIPTS / 'mac_composition.py', 'mac_composition_ssh_test')
mac_research2 = _load(SCRIPTS / 'mac_research2.py', 'mac_research2_ssh_test')
helper = _load(SCRIPTS / 'mac_update_reboot.py', 'mac_update_reboot_ssh_test')


class RecordingRunner:
    """Returns a canned transcript; never touches the network."""

    def __init__(self, transcript=b'', returncode=0):
        self.transcript = transcript
        self.returncode = returncode
        self.calls = []

    def __call__(self, argv, timeout, password=None):
        self.calls.append((list(argv), timeout, password))
        return helper.CommandResult(self.returncode, stdout=self.transcript)


class LocalShellRunner:
    """Runs the framed remote command through the local shell; no network."""

    def __init__(self):
        self.calls = []

    def __call__(self, argv, timeout, password=None):
        self.calls.append((list(argv), timeout, password))
        proc = subprocess.run(['/bin/sh', '-c', argv[-1]],
                              stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                              timeout=timeout)
        return helper.CommandResult(proc.returncode, stdout=proc.stdout)


class FakeSshConsole:
    instances = []

    def __init__(self, raw_log_path, events_log_path, board_host,
                 credential_response):
        self.raw_log_path = pathlib.Path(raw_log_path)
        self.events_log_path = pathlib.Path(events_log_path)
        self.board_host = board_host
        self.credential_response = credential_response
        self.started = 0
        self.closed = 0
        FakeSshConsole.instances.append(self)

    def start(self):
        self.started += 1
        self.raw_log_path.parent.mkdir(parents=True, exist_ok=True)
        self.raw_log_path.write_bytes(b'')
        self.events_log_path.write_bytes(b'')

    def close(self):
        self.closed += 1


class FakeSshHelperModule:
    @staticmethod
    def load_credential_response(path):
        return b'\x00fake-secret\x00'

    @staticmethod
    def build_ssh_argv(**kwargs):
        raise AssertionError('no ssh argv is built in this test')

    @staticmethod
    def default_ssh_runner(argv, timeout, password=None):
        raise AssertionError('no ssh is run in this test')


class SshTransportTests(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory(prefix='idma-mac-ssh-')
        self.tmp = pathlib.Path(self._tmp.name)
        self.addCleanup(self._tmp.cleanup)
        FakeSshConsole.instances = []
        self._counter = 0

    def build_console(self, runner):
        self._counter += 1
        ssh_module = types.SimpleNamespace(
            build_ssh_argv=helper.build_ssh_argv,
            default_ssh_runner=runner)
        console_cls = mac.build_ssh_console_class(mac_research2, ssh_module)
        raw = self.tmp / ('console-%d.raw.log' % self._counter)
        events = self.tmp / ('console-%d.events.log' % self._counter)
        return console_cls(raw, events, BOARD_HOST, CREDENTIAL)

    # -- reuse of the real frame builder, cwd and exit_var semantics --------

    def test_ssh_console_reuses_run_shell_framing_with_cwd_and_exit_var(self):
        runner = LocalShellRunner()
        console = self.build_console(runner)
        console.start()
        case_dir = self.tmp / 'board-case'
        case_dir.mkdir()
        (case_dir / 'marker.txt').write_text('hello-board')

        res = console.run_shell('cat marker.txt', timeout=30.0, cwd=str(case_dir))
        self.assertEqual(res.exit_code, 0)
        self.assertEqual(res.stdout.strip(), b'hello-board')

        res = console.run_shell('cat marker.txt; ec=$?', timeout=30.0,
                                cwd=str(case_dir), exit_var='ec')
        self.assertEqual(res.exit_code, 0)

        res = console.run_shell('false; ec=$?', timeout=30.0, exit_var='ec')
        self.assertEqual(res.exit_code, 1)

        res = console.run_shell('true', timeout=30.0)
        self.assertEqual(res.exit_code, 0)

        argv, _timeout, password = runner.calls[-1]
        self.assertEqual(argv[0], 'ssh')
        self.assertEqual(argv[-2], 'root@' + BOARD_HOST)
        self.assertEqual(password, CREDENTIAL)

    # -- start / close / echo / drain never touch a serial port -------------

    def test_ssh_console_start_close_never_opens_serial(self):
        console = self.build_console(RecordingRunner(b''))
        self.assertIsNone(console.port)
        self.assertIsNone(console._ser)
        console.start()
        self.assertIsNone(console._ser)
        self.assertIsNone(console.disable_echo())
        self.assertEqual(console.drain_input(), b'')
        console.close()
        self.assertTrue(console._stop.is_set())

    # -- banner / password prompt / CRLF stripping --------------------------

    def test_ssh_frame_strips_banner_password_prompt_and_crlf(self):
        begin = '__R2B_dead_beef__'
        end = '__R2E_dead_beef__'
        transcript = (
            b"Warning: Permanently added 'board.local' (ED25519) to the "
            b"list of known hosts.\r\n"
            b"root@board.local's password: \r\n"
            + begin.encode('ascii') + b'\r\n'
            b'{"answer": 42}\r\n'
            + end.encode('ascii') + b'0\r\n')
        console = self.build_console(RecordingRunner(transcript))
        res = console._run_framed('true', begin, end, 30.0)
        self.assertEqual(res.exit_code, 0)
        self.assertEqual(res.stdout.strip(), b'{"answer": 42}')
        self.assertNotIn(b'Warning', res.stdout)
        self.assertNotIn(b'password', res.stdout)
        self.assertNotIn(b'\r', res.stdout)

    # -- non-zero command exit is preserved ---------------------------------

    def test_ssh_frame_preserves_a_nonzero_remote_exit(self):
        begin = '__R2B_aa_bb__'
        end = '__R2E_aa_bb__'
        transcript = (begin.encode('ascii') + b'\r\nboom\r\n'
                      + end.encode('ascii') + b'7\r\n')
        console = self.build_console(RecordingRunner(transcript))
        res = console._run_framed('cli', begin, end, 30.0)
        self.assertEqual(res.exit_code, 7)

    # -- failure paths -------------------------------------------------------

    def test_ssh_transport_nonzero_exit_fails(self):
        runner = RecordingRunner(b'Permission denied (publickey,password).\r\n',
                                 returncode=255)
        console = self.build_console(runner)
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    def test_ssh_missing_end_marker_fails(self):
        transcript = b'__R2B_a_b__\r\npartial output\r\n'
        console = self.build_console(RecordingRunner(transcript))
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    def test_ssh_duplicate_marker_fails(self):
        transcript = (b'__R2B_a_b__\r\none\r\n__R2B_a_b__\r\n'
                      b'__R2E_a_b__0\r\n')
        console = self.build_console(RecordingRunner(transcript))
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    def test_ssh_bad_exit_marker_fails(self):
        transcript = b'__R2B_a_b__\r\nout\r\n__R2E_a_b__not-a-number\r\n'
        console = self.build_console(RecordingRunner(transcript))
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    def test_ssh_exit_marker_without_line_end_fails(self):
        transcript = b'__R2B_a_b__\r\nout\r\n__R2E_a_b__0'
        console = self.build_console(RecordingRunner(transcript))
        with self.assertRaises(mac.CompositionError):
            console._run_framed('true', '__R2B_a_b__', '__R2E_a_b__', 30.0)

    # -- main() entry: ssh never touches UART / serial ----------------------

    def test_ssh_main_does_not_touch_uart_or_serial(self):
        out_dir = self.tmp / 'ssh-main'
        with mock.patch.object(mac, 'check_uart_ownership') as uart_check, \
                mock.patch.object(mac, 'load_console_class') as console_class, \
                mock.patch.object(mac, '_load_serial_module', return_value=object()), \
                mock.patch.object(mac, 'load_ssh_helper',
                                  return_value=FakeSshHelperModule), \
                mock.patch.object(mac, 'build_ssh_console_class',
                                  return_value=FakeSshConsole), \
                mock.patch.object(mac.AttachRunner, 'run', return_value=0):
            rc = mac.main(['--out', str(out_dir),
                           '--expected-kernel', EXPECTED_KERNEL,
                           '--run', '--transport', 'ssh',
                           '--board-host', BOARD_HOST,
                           '--credential-script', '/tmp/cred.py',
                           '--ssh-helper', '/tmp/helper.py'])
        self.assertEqual(rc, 0)
        uart_check.assert_not_called()
        console_class.assert_not_called()
        self.assertEqual(len(FakeSshConsole.instances), 1)
        console = FakeSshConsole.instances[0]
        self.assertEqual(console.board_host, BOARD_HOST)
        self.assertEqual(console.started, 1)
        self.assertEqual(console.closed, 1)
        self.assertTrue(out_dir.is_dir())
        events = (out_dir / 'console.events.log').read_bytes()
        self.assertNotIn(b'fake-secret', events)

    def test_ssh_run_requires_board_host_credential_and_helper(self):
        out_dir = self.tmp / 'missing-args'
        with self.assertRaises(SystemExit):
            mac.main(['--out', str(out_dir),
                      '--expected-kernel', EXPECTED_KERNEL,
                      '--run', '--transport', 'ssh'])
        self.assertFalse(out_dir.exists())

    # -- manifest transport fields ------------------------------------------

    def test_plan_only_records_ssh_transport_without_a_serial_port(self):
        out_dir = self.tmp / 'plan-ssh'
        rc = mac.main(['--out', str(out_dir),
                       '--expected-kernel', EXPECTED_KERNEL,
                       '--plan', '--transport', 'ssh',
                       '--board-host', BOARD_HOST,
                       '--ssh-helper', '/tmp/helper.py',
                       '--credential-script', '/tmp/cred.py'])
        self.assertEqual(rc, 0)
        manifest = json.loads((out_dir / 'manifest.json').read_text())
        self.assertEqual(manifest['transport'], 'ssh')
        self.assertIsNone(manifest['serial'])
        self.assertEqual(manifest['board_host'], BOARD_HOST)
        self.assertEqual(manifest['ssh_helper'], '/tmp/helper.py')

    def test_plan_only_keeps_the_uart_serial_port(self):
        out_dir = self.tmp / 'plan-uart'
        rc = mac.main(['--out', str(out_dir),
                       '--expected-kernel', EXPECTED_KERNEL,
                       '--plan', '--serial', SERIAL])
        self.assertEqual(rc, 0)
        manifest = json.loads((out_dir / 'manifest.json').read_text())
        self.assertEqual(manifest['transport'], 'uart')
        self.assertEqual(manifest['serial'], SERIAL)
        self.assertIsNone(manifest['board_host'])


if __name__ == '__main__':
    unittest.main()
```

---

## 3. Run commands

```bash
# new bounded SSH tests (no hardware, no network, no real credential)
python3 tests/test_mac_composition_ssh.py -v

# quick dry plan for the SSH transport (no board contact at all)
python3 scripts/mac_composition.py \
  --out /tmp/maccomp-plan-ssh \
  --expected-kernel <shipped-kernel> \
  --plan --transport ssh \
  --board-host <board-host> \
  --credential-script <existing-credential-script> \
  --ssh-helper scripts/mac_update_reboot.py
```

Real run (operator only, one shot, the existing 172 cases):

```bash
python3 scripts/mac_composition.py \
  --out /tmp/maccomp-<fresh-id> \
  --run --transport ssh \
  --board-host <board-host> \
  --credential-script <existing-credential-script> \
  --ssh-helper scripts/mac_update_reboot.py \
  --expected-kernel <shipped-kernel> \
  --through-phase explore \
  --board-cli /usr/bin/idma_composition \
  --device /dev/idma0
```

---

## 4. Assumptions / risks

- **Where the helper and credential are loaded.** `main` loads the helper (explicit `--ssh-helper`) and reads the credential into memory *before* constructing the console and *before* creating `--out`; the console’s `start` only creates log directories. Nothing in this path sends or logs the credential. If you prefer the load to literally happen inside `start`, it is a local move without behaviour change — flagging, not doing it, to avoid touching the constructed‑class contract the test relies on.
- **Password‑only.** `build_ssh_argv` is called with `password_mode=True`; the key path is unused. Only `load_credential_response` / `build_ssh_argv` / `default_ssh_runner` are called from the helper — no HTTP, host_ip, JTAG, download, card write or reboot.
- **Verification is still via `uname -r`, `boot_id`, taint, board‑CLI `--abi` and the per‑case JSON**, all over the framed SSH channel; that is the same `AttachRunner` logic.
- **Frame parsing.** `_run_framed` requires exactly one BEGIN and one END marker, END after BEGIN, and a numeric exit line ending in `\n`; CRLF is normalised to LF. Any SSH warning / password prompt / echoed text before BEGIN is discarded. A non‑zero ssh rc or a missing/duplicated/malformed frame raises `CompositionError` (a hard stop). A non‑zero *command* exit is preserved in `ShellResult.exit_code` and is treated as a case failure by `AttachRunner`, not as success.
- **CRLF normalisation** changes the bytes stored in `record.raw.json` from the serial path (CR removed). This matches the task’s “CRLF removal” requirement and is harmless for the JSON/`cat` payloads used here; it would only matter if a case deliberately emitted raw CR bytes.
- **Plan.json** is produced by `composition_plan.build_plan()` and is not modified; transport is recorded in `manifest.json` (run and plan-only). If you also want a transport field inside `plan.json`, that is a one‑line change in `composition_plan` — out of scope here.
- **The helper’s PTY EOF wait race** fixed by Codex is the very code path now used by `_run_framed` for password SSH; this patch does not duplicate any of that PTY code.

## 5. Suggested acceptance checks (for Codex to run/verify)

1. `python3 tests/test_mac_composition_ssh.py -v` — all pass (no network, no serial).
2. `python3 tests/test_mac_composition_entry.py -v` — still pass (uart default; new manifest keys must not break it).
3. On a live board: one `--run --transport ssh` invocation produces `plan.json`, `manifest.json` with `transport="ssh"`, `serial=null`, `board_host`, `ssh_helper`; `boot.json`; and one `cases/<id>/record.raw.json` per case, with the 172‑case count unchanged for `--through-phase explore`.
4. Confirm `manifest.json` for a uart run still records the serial port, and that no `console.raw.log`/`console.events.log` ever contains the credential literal.

I have not modified files or run any tests; the above is a proposed patch and a proposed bounded test file for Codex to apply and verify.