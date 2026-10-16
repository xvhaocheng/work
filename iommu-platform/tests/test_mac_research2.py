#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''Mock tests for scripts/mac_research2.py.

No real board and no real pyserial are used.  The serial mock executes every
framed command line with the host /bin/sh, so the marker syntax, the exit-code
frame, the cwd handling, the CRLF handling and the echo-immunity are exercised
for real instead of being regex-matched against a fake echo.

The plan/results modules are loaded by file path so that scripts/ never lands on
sys.path (scripts/platform.py shadowing the stdlib platform module was the
original import failure).
'''
import argparse
import hashlib
import importlib.util
import io
import json
import pathlib
import queue
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile
import threading
import unittest

HERE = pathlib.Path(__file__).resolve()
REPO = HERE.parent.parent
SCRIPTS = REPO / 'scripts'


def load_module(path, name):
    spec = importlib.util.spec_from_file_location(name, str(path))
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


m2 = load_module(SCRIPTS / 'mac_research2.py', 'mac_research2_under_test')
plan2 = load_module(SCRIPTS / 'research2_plan.py', 'research2_plan_under_test')

CAPS_ROLE1 = {
    'schema': 'idma-research2-caps-v1',
    'success': True,
    'magic': '0x49444d4132434150',
    'version': '0x1',
    'role': '0x1',
    'features': '0x7',
    'observer_magic': '0x49444d414f425308',
    'error': '',
}
CAPS_ROLE2 = dict(CAPS_ROLE1, role='0x2', features='0xf')

FAKE_CLI_TEMPLATE = '''#!/bin/sh
# Simulated board CLI.  One fresh boot is produced per --caps2 call.
count=$(cat @@COUNTER@@ 2>/dev/null || echo 0)
for a in $@; do
if [ $a = --caps2 ]; then
count=$((count + 1))
echo $count > @@COUNTER@@
if [ $count -le 1 ]; then
echo @@BOOT_ONE@@ > @@BOOT@@
echo @@CAPS_ONE@@
else
echo @@BOOT_TWO@@ > @@BOOT@@
echo @@CAPS_TWO@@
fi
exit 0
fi
if [ $a = FAIL ]; then
echo simulated-DMA-failure 1>&2
exit 7
fi
done
echo @@RECORD@@
exit 0
'''


def write_fake_board_cli(tmp, boot_one='boot-one', boot_two='boot-two', counter_start='0'):
    counter = tmp / 'boot.counter'
    counter.write_text(counter_start)
    boot_file = tmp / 'boot.id'
    boot_file.write_text('boot-zero')
    script = tmp / 'idma_research'
    text = FAKE_CLI_TEMPLATE
    text = text.replace('@@COUNTER@@', shlex_quote(str(counter)))
    text = text.replace('@@BOOT@@', shlex_quote(str(boot_file)))
    text = text.replace('@@BOOT_ONE@@', boot_one)
    text = text.replace('@@BOOT_TWO@@', boot_two)
    text = text.replace('@@CAPS_ONE@@', shlex_quote(json.dumps(CAPS_ROLE1)))
    text = text.replace('@@CAPS_TWO@@', shlex_quote(json.dumps(CAPS_ROLE2)))
    text = text.replace('@@RECORD@@', shlex_quote(json.dumps({
        'schema': 'idma-research-v1', 'success': True, 'validated': True, 'mode': 'cost'})))
    script.write_text(text)
    script.chmod(0o755)
    return script, boot_file


def shlex_quote(value):
    return "'" + str(value).replace("'", "'\\''") + "'"


def host_uname_r():
    proc = subprocess.run(['/bin/sh', '-c', 'uname -r'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return proc.stdout.decode('utf-8', 'replace').strip()


class ShellMockSerial:
    '''Serial stand-in that really executes each sent line through /bin/sh.'''

    def __init__(self, *args, echo=False, crlf=False, prelude='', cwd=None):
        self._queue = queue.Queue()
        self.written = bytearray()
        self.closed = False
        self.echo = echo
        self.crlf = crlf
        self.prelude = prelude
        self.cwd = cwd
        self.shell = '/bin/sh'

    def read(self, n):
        try:
            return self._queue.get(timeout=0.02)
        except queue.Empty:
            return b''

    def write(self, data):
        self.written.extend(data)
        text = data.decode('utf-8', 'replace')
        for line in text.split('\n'):
            if line:
                self._dispatch(line)
        return len(data)

    def flush(self):
        pass

    def close(self):
        self.closed = True

    def _put(self, data):
        if not data:
            return
        if self.crlf:
            data = data.replace(b'\n', b'\r\n')
        self._queue.put(data)

    def _dispatch(self, line):
        if self.echo:
            self._put((line + '\n').encode('utf-8'))
        if line.strip().startswith('stty'):
            try:
                subprocess.run([self.shell, '-c', line], stdout=subprocess.DEVNULL,
                               stderr=subprocess.DEVNULL, timeout=5, cwd=self.cwd)
            except Exception:
                pass
            return
        try:
            proc = subprocess.run([self.shell, '-c', self.prelude + line],
                                  stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                  timeout=30, cwd=self.cwd)
            blob = proc.stdout + proc.stderr
        except Exception:
            blob = b''
        self._put(blob)


class MockProgrammer:
    def __init__(self, fail_on=None):
        self.calls = []
        self.fail_on = fail_on

    def flash(self, bit_path):
        self.calls.append(bit_path)
        if self.fail_on and self.fail_on in bit_path:
            raise m2.TechnicalFailure('mock flash failure')
        return {'cmd': ['openFPGALoader', bit_path], 'rc': 0, 'stdout': '', 'stderr': ''}


class RecordingConsole:
    def __init__(self, exit_code=0):
        self.commands = []
        self.exit_code = exit_code

    def start(self):
        pass

    def close(self):
        pass

    def run_shell(self, command, timeout, cwd=None, exit_var=None):
        self.commands.append((command, cwd))
        return m2.ShellResult(b'', self.exit_code)


class DummyConsole:
    def start(self):
        pass

    def close(self):
        pass

    def disable_echo(self):
        pass

    def run_shell(self, *a, **k):
        raise AssertionError('DummyConsole.run_shell should not be called')


class FakePlanModule:
    def __init__(self, cases_by_stage):
        self.cases_by_stage = cases_by_stage
        self.select_calls = []

    def build_plan(self, stage):
        cases = [dict(case) for case in self.cases_by_stage.get(stage, [])]
        for ordinal, case in enumerate(cases, start=1):
            case['ordinal'] = ordinal
            case.setdefault('stage', stage)
        return {'schema': 'idma-research2-plan', 'version': 1, 'stage': stage,
                'count': len(cases), 'cases': cases}

    def select_configs(self, records):
        self.select_calls.append(sorted(records))
        return {'schema': 'idma-research2-selection', 'configs': {},
                'unmatched': [], 'input_sha256': 'mock'}

    def resolve_case(self, case, selection):
        return dict(case)


class FakeResultsModule:
    def validate_record(self, case, record):
        return []

    def metrics(self, record):
        return {'mock': True}


def default_cases():
    e2 = [
        {'id': 'e2-short-w0-forward-n0', 'phase': 'short', 'role': 1,
         'argv': ['--e2-ext', '--a-pages', '0', '--a-transfers', '0',
                  '--window-cycles', '5000000', '--warmup', '2', '--transfers', '1',
                  '--a-gap-cycles', '0', '--timeout-ms', '2000',
                  '--reverse', '0', '--policy-mode', '0', '--policy-window', '0']},
    ]
    method = [
        {'id': 'm-cal-fixedgap-g0', 'phase': 'calibration', 'role': 2,
         'argv': ['--method', '--a-pages', '8', '--policy-mode', '0',
                  '--policy-window', '16384', '--target-bytes', '51540',
                  '--a-gap-cycles', '0', '--timeout-ms', '10000']},
        {'id': 'm-formal-p5-t120-r1', 'phase': 'formal', 'role': 2, 'needs_selection': True,
         'argv': ['--method', '--a-pages', '8', '--policy-mode', '5',
                  '--policy-window', '16384', '--target-bytes', '51540',
                  '--a-gap-cycles', '0', '--timeout-ms', '10000']},
    ]
    return {'e2': e2, 'method': method}


def make_manifest(tmp, board_cli, kernel, images=None):
    (tmp / 'plan.py').write_text('')
    (tmp / 'analyzer.py').write_text('')
    if images is None:
        (tmp / 'role1.bit').write_bytes(b'role1')
        (tmp / 'role2.bit').write_bytes(b'role2')
        images = [
            {'role': 1, 'bit': 'role1.bit', 'sha256': hashlib.sha256(b'role1').hexdigest()},
            {'role': 2, 'bit': 'role2.bit', 'sha256': hashlib.sha256(b'role2').hexdigest()},
        ]
    return {
        'schema': m2.SCHEMA,
        'software': {
            'kernel_release': kernel,
            'cli_sha256': m2.sha256_file(board_cli),
            'board_cli_path': str(board_cli),
        },
        'images': images,
        'scripts': {'plan': 'plan.py', 'analyzer': 'analyzer.py'},
    }


def make_args(tmp, stage='both'):
    return argparse.Namespace(
        stage=stage,
        startup_timeout=10.0,
        command_timeout=10.0,
        case_timeout=30.0,
        transfer_timeout=30.0,
        jtag_freq=1000000,
        out=str(tmp / 'out'),
        serial='/dev/fake',
    )


class ShellRunner(m2.Research2Runner):
    '''Runs everything through the framed /bin/sh path; only the board /proc and
    dmesg sources are stubbed, and the flash call is mocked.'''

    def _capture_state(self, board_dir, tag):
        for name, value in (('taint.' + tag, '0'),
                            ('dmesg.' + tag, 'mock dmesg ' + tag),
                            ('uname.' + tag, 'mock uname ' + tag)):
            res = self.console.run_shell('printf %s ' + shlex_quote(value) + ' > ' + shlex_quote(name),
                                         timeout=self.args.command_timeout, cwd=board_dir)
            if res.exit_code != 0:
                raise m2.TechnicalFailure('mock capture ' + tag + ' failed')

    def _flash(self, image):
        self.programmer.flash(image['bit'])
        self._write_json(self.current_stage + '/flash.json', {'mock': image['bit']})


def build_e2e_run(tmp, stage='both', cases=None, counter_start='0',
                  boot_one='boot-one', boot_two='boot-two', echo=True):
    board_cli, boot_file = write_fake_board_cli(tmp, boot_one=boot_one, boot_two=boot_two,
                                                counter_start=counter_start)
    manifest = make_manifest(tmp, board_cli, host_uname_r())
    args = make_args(tmp, stage=stage)
    plan_module = FakePlanModule(cases if cases is not None else default_cases())
    programmer = MockProgrammer()
    serial = ShellMockSerial(echo=echo)
    console = m2.SerialConsole('/dev/fake', tmp / 'console.raw.log', tmp / 'console.events.log',
                               serial_factory=lambda *a, **k: serial)
    runner = ShellRunner(args, manifest, tmp, tmp / 'out', console, programmer,
                         plan_module, FakeResultsModule())
    runner.board_root = str(tmp / 'board')
    runner.boot_id_path = str(boot_file)
    return runner, programmer, tmp / 'out', serial


class TestHelpers(unittest.TestCase):
    def test_quote_argv(self):
        self.assertEqual(m2.quote_argv(['a b', 'c']), chr(39) + 'a b' + chr(39) + ' c')

    def test_resolve_package_path_rejects_escape(self):
        with tempfile.TemporaryDirectory() as td:
            root = pathlib.Path(td)
            (root / 'ok').write_text('x')
            with self.assertRaises(m2.ManifestError):
                m2.resolve_package_path(root, '../ok')
            with self.assertRaises(m2.ManifestError):
                m2.resolve_package_path(root, '/abs')

    def test_resolve_package_path_rejects_external_symlink(self):
        with tempfile.TemporaryDirectory() as td:
            root = pathlib.Path(td) / 'pkg'
            root.mkdir()
            outside = pathlib.Path(td) / 'outside.txt'
            outside.write_text('x')
            (root / 'link').symlink_to(outside)
            with self.assertRaises(m2.ManifestError):
                m2.resolve_package_path(root, 'link')

    def test_verify_sha256_mismatch(self):
        with tempfile.TemporaryDirectory() as td:
            p = pathlib.Path(td) / 'f'
            p.write_bytes(b'x')
            with self.assertRaises(m2.ManifestError):
                m2.verify_sha256(p, '0' * 64, 'test')

    def test_safe_case_id(self):
        self.assertEqual(m2.safe_case_id('m-formal-p5-t120-r1'), 'm-formal-p5-t120-r1')
        for bad in ('../x', 'A-b', 'a/b', '.hidden', 'a b', ''):
            with self.assertRaises(m2.TechnicalFailure):
                m2.safe_case_id(bad)


def tar_bytes(entries):
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode='w:gz') as tar:
        for name, kind in entries:
            if kind == 'dir':
                info = tarfile.TarInfo(name)
                info.type = tarfile.DIRTYPE
                tar.addfile(info)
            elif kind == 'symlink':
                info = tarfile.TarInfo(name)
                info.type = tarfile.SYMTYPE
                info.linkname = '../evil'
                tar.addfile(info)
            else:
                payload = b'x'
                info = tarfile.TarInfo(name)
                info.size = len(payload)
                tar.addfile(info, io.BytesIO(payload))
    return buf.getvalue()


class TestSafeExtract(unittest.TestCase):
    def test_rejects_traversal(self):
        with tempfile.TemporaryDirectory() as td:
            with self.assertRaises(m2.TechnicalFailure):
                m2.safe_extract_tar_gz(tar_bytes([('../evil', 'file')]), pathlib.Path(td))

    def test_rejects_symlink(self):
        with tempfile.TemporaryDirectory() as td:
            with self.assertRaises(m2.TechnicalFailure):
                m2.safe_extract_tar_gz(tar_bytes([('link', 'symlink')]), pathlib.Path(td))

    def test_rejects_duplicate_entries(self):
        with tempfile.TemporaryDirectory() as td:
            with self.assertRaises(m2.TechnicalFailure):
                m2.safe_extract_tar_gz(tar_bytes([('record.json', 'file'), ('record.json', 'file')]),
                                       pathlib.Path(td))

    def test_rejects_entries_outside_the_allowlist(self):
        with tempfile.TemporaryDirectory() as td:
            with self.assertRaises(m2.TechnicalFailure):
                m2.safe_extract_tar_gz(tar_bytes([('SAVED', 'file')]), pathlib.Path(td),
                                       allowed_names=m2.CASE_ARCHIVE_FILES)

    def test_extracts_the_allowlisted_case_archive(self):
        with tempfile.TemporaryDirectory() as td:
            dest = pathlib.Path(td) / 'board'
            m2.safe_extract_tar_gz(tar_bytes([('./', 'dir'), ('./record.json', 'file')]), dest,
                                   allowed_names=m2.CASE_ARCHIVE_FILES)
            self.assertTrue((dest / 'record.json').is_file())


class TestCaps(unittest.TestCase):
    def test_accepts_the_real_cli_caps_format_with_hex_numbers(self):
        caps = m2.parse_caps(json.dumps(CAPS_ROLE1))
        self.assertEqual(m2.check_caps_for_role(caps, 1), 0x7)
        caps2 = m2.parse_caps(json.dumps(
            dict(CAPS_ROLE2, magic='0x49444D4132434150', observer_magic='0x49444D414F425308')))
        self.assertEqual(m2.check_caps_for_role(caps2, 2), 0xf)

    def test_accepts_decimal_numbers_as_well(self):
        caps = {'schema': 'idma-research2-caps-v1', 'success': True,
                'magic': m2.IDMA2CAP_MAGIC, 'version': 1, 'role': 1, 'features': 7,
                'observer_magic': m2.OBSERVER_MAGIC, 'error': ''}
        self.assertEqual(m2.check_caps_for_role(caps, 1), 7)

    def test_rejects_schema_and_success(self):
        with self.assertRaises(m2.TechnicalFailure):
            m2.check_caps_for_role(dict(CAPS_ROLE1, schema='other'), 1)
        with self.assertRaises(m2.TechnicalFailure):
            m2.check_caps_for_role(dict(CAPS_ROLE1, success=False, error='open: nope'), 1)

    def test_rejects_magic_role_features_and_observer(self):
        with self.assertRaises(m2.TechnicalFailure):
            m2.check_caps_for_role(dict(CAPS_ROLE1, magic='0x1'), 1)
        with self.assertRaises(m2.TechnicalFailure):
            m2.check_caps_for_role(dict(CAPS_ROLE1, role='0x2'), 1)
        with self.assertRaises(m2.TechnicalFailure):
            m2.check_caps_for_role(dict(CAPS_ROLE1, features='0xf'), 1)
        with self.assertRaises(m2.TechnicalFailure):
            m2.check_caps_for_role(dict(CAPS_ROLE1, observer_magic='0x0'), 1)

    def test_rejects_free_text_and_regex_like_input(self):
        with self.assertRaises(m2.TechnicalFailure):
            m2.parse_caps('magic: idma2cap version=1 role=1 features=7')
        with self.assertRaises(m2.TechnicalFailure):
            m2.parse_caps('')


class TestFramedShell(unittest.TestCase):
    def make_console(self, serial):
        tmp = pathlib.Path(tempfile.mkdtemp())
        self.addCleanup(shutil.rmtree, str(tmp), ignore_errors=True)
        console = m2.SerialConsole('/dev/fake', tmp / 'raw.log', tmp / 'events.log',
                                   serial_factory=lambda *a, **k: serial)
        console.start()
        self.addCleanup(console.close)
        return console

    def test_frame_reports_stdout_and_exit(self):
        console = self.make_console(ShellMockSerial())
        res = console.run_shell('echo hello-frame', timeout=10.0)
        self.assertEqual(res.exit_code, 0)
        self.assertIn(b'hello-frame', res.stdout)

    def test_frame_reports_failure_exit(self):
        console = self.make_console(ShellMockSerial())
        res = console.run_shell('false', timeout=10.0)
        self.assertEqual(res.exit_code, 1)

    def test_frame_reports_cwd_failure(self):
        console = self.make_console(ShellMockSerial())
        res = console.run_shell('pwd', timeout=10.0, cwd='/definitely/not/here')
        self.assertNotEqual(res.exit_code, 0)

    def test_frame_honours_exit_var(self):
        console = self.make_console(ShellMockSerial())
        res = console.run_shell('ec=9; true', timeout=10.0, exit_var='ec')
        self.assertEqual(res.exit_code, 9)

    def test_marker_is_never_literal_in_the_sent_command(self):
        serial = ShellMockSerial()
        console = self.make_console(serial)
        console.run_shell('true', timeout=10.0)
        sent = bytes(serial.written).decode('utf-8', 'replace')
        self.assertNotIn('__R2B_', sent)
        self.assertNotIn('__R2E_', sent)
        self.assertIn('${b0}_${m1}_${m2}__', sent)

    def test_an_echoed_command_line_is_never_read_as_output(self):
        console = self.make_console(ShellMockSerial(echo=True))
        res = console.run_shell('echo echoed-token', timeout=10.0)
        self.assertEqual(res.exit_code, 0)
        self.assertNotIn(b'__R2B_', res.stdout)
        self.assertNotIn(b'__R2E_', res.stdout)
        self.assertIn(b'echoed-token', res.stdout)
        self.assertNotIn(b'printf', res.stdout)

    def test_crlf_output(self):
        console = self.make_console(ShellMockSerial(crlf=True))
        res = console.run_shell('printf crlf-token', timeout=10.0)
        self.assertEqual(res.exit_code, 0)
        self.assertIn(b'crlf-token', res.stdout)

    def test_chunked_output(self):
        console = self.make_console(ShellMockSerial())
        res = console.run_shell('printf aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa', timeout=10.0)
        self.assertEqual(res.exit_code, 0)
        self.assertEqual(res.stdout.count(b'a'), 48)

    def test_missing_cwd_never_runs_semicolon_commands_even_with_stale_exit_var(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            sentinel = tmp / 'sentinel.txt'
            sentinel.write_text('PRIOR')
            missing = tmp / 'missing-cwd'
            serial = ShellMockSerial(prelude='ec=0; ', cwd=str(tmp))
            console = self.make_console(serial)
            res = console.run_shell('true; echo SHOULD_NOT_RUN > sentinel.txt',
                                    timeout=10.0, cwd=str(missing), exit_var='ec')
            self.assertNotEqual(res.exit_code, 0)
            self.assertEqual(sentinel.read_text(), 'PRIOR')
            res2 = console.run_shell('printf ok-after', timeout=10.0)
            self.assertEqual(res2.exit_code, 0)
            self.assertIn(b'ok-after', res2.stdout)


class TestCaptureState(unittest.TestCase):
    def make_runner(self, console, tmp):
        manifest = make_manifest(tmp, write_dummy_cli(tmp), host_uname_r())
        return m2.Research2Runner(make_args(tmp), manifest, tmp, tmp / 'out', console,
                                  MockProgrammer(), FakePlanModule({}), FakeResultsModule())

    def test_capture_state_commands_and_exit_checks(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            console = RecordingConsole(exit_code=0)
            runner = self.make_runner(console, tmp)
            runner._capture_state('/tmp/case', 'before')
            commands = [c for c, _ in console.commands]
            self.assertEqual(commands, [
                'cat /proc/sys/kernel/tainted > taint.before 2>&1',
                'dmesg > dmesg.before 2>&1',
                'uname -a > uname.before 2>&1',
            ])
            self.assertEqual({cwd for _, cwd in console.commands}, {'/tmp/case'})

            failing = RecordingConsole(exit_code=1)
            runner = self.make_runner(failing, tmp)
            with self.assertRaises(m2.TechnicalFailure):
                runner._capture_state('/tmp/case', 'after')


def write_dummy_cli(tmp):
    cli = tmp / 'idma_research_dummy'
    if not cli.exists():
        cli.write_text('#!/bin/sh\nexit 0\n')
        cli.chmod(0o755)
    return cli


class TestPlanOutline(unittest.TestCase):
    def test_e2_plan_outline(self):
        plan = plan2.build_plan('e2')
        self.assertEqual(plan['schema'], 'idma-research2-plan')
        self.assertEqual(plan['count'], 64)
        self.assertEqual(len(plan['cases']), 64)
        phases = {}
        for case in plan['cases']:
            phases[case['phase']] = phases.get(case['phase'], 0) + 1
            self.assertEqual(case['role'], 1)
            self.assertFalse(case['needs_selection'])
            self.assertTrue(case['argv'])
            self.assertIn('--e2-ext', case['argv'])
        self.assertEqual(phases, {'short': 4, 'boundary': 30, 'prefix': 30})

    def test_method_plan_outline(self):
        plan = plan2.build_plan('method')
        self.assertEqual(plan['count'], 462)
        self.assertEqual(plan['counts'], {'short': 9, 'reference': 12, 'calibration': 89,
                                          'formal': 192, 'boundary': 40, 'holdout': 120,
                                          'total': 462})
        for case in plan['cases']:
            self.assertEqual(case['role'], 2)
            if case['needs_selection']:
                self.assertIsNone(case['argv'])
            else:
                self.assertIn('--method', case['argv'])
                self.assertIn('--policy-window', case['argv'])
                self.assertEqual(case['argv'][case['argv'].index('--policy-window') + 1],
                                 str(case['config']['window_cycles']))

    def test_resolve_case_strict_two_argument_api(self):
        plan = plan2.build_plan('method')
        formal = [c for c in plan['cases'] if c['phase'] == 'formal'][0]
        with self.assertRaises(ValueError):
            plan2.resolve_case(formal, None)
        with self.assertRaises(TypeError):
            plan2.resolve_case(formal)
        entry = {'policy_mode': 5, 'window_cycles': 16384, 'target_bytes': 51540,
                 'deadband_bytes': 4096, 'initial_budget': 4000, 'min_budget': 1,
                 'max_budget': 16000, 'budget_step': 125, 'credit_cap': 32000,
                 'hit_weight': 0, 'miss_weight': 0, 'ptw_weight': 0,
                 'gap_cycles': 0, 'candidate_id': 'cal-m5-t150-c0', 'matched': True}
        targets = {str(target): dict(entry) for target in (120, 150, 170)}
        selection = {'configs': {tag: dict(targets) for tag in plan2.FORMAL_METHOD_TAGS}}
        resolved = plan2.resolve_case(formal, selection)
        self.assertFalse(resolved['needs_selection'])
        self.assertIn('--policy-window', resolved['argv'])
        self.assertEqual(resolved['argv'][resolved['argv'].index('--policy-window') + 1], '16384')


class TestSelectionFreeze(unittest.TestCase):
    def test_only_calibration_records_are_frozen(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            runner, programmer, out, serial = build_e2e_run(tmp, stage='method', counter_start='1')
            runner.stage_dir = out / 'method'
            runner.stage_dir.mkdir(parents=True, exist_ok=True)
            for cid, payload in (('m-cal-1', {'kind': 'calibration'}),
                                 ('m-hold-1', {'kind': 'holdout'})):
                d = runner.stage_dir / 'runs' / cid
                d.mkdir(parents=True)
                (d / 'record.json').write_text(json.dumps(payload))
            cases = [{'id': 'm-cal-1', 'phase': 'calibration'},
                     {'id': 'm-hold-1', 'phase': 'holdout', 'needs_selection': True}]
            selection = runner._freeze_selection('method', cases)
            self.assertEqual(runner.plan_module.select_calls, [['m-cal-1']])
            written = json.loads((out / 'method' / 'selection.json').read_text())
            self.assertEqual(written, selection)
            self.assertNotIn('selection', written)
            audit = json.loads((out / 'method' / 'selection.audit.json').read_text())
            self.assertEqual(audit['calibration_case_ids'], ['m-cal-1'])
            self.assertEqual(list(audit['input_file_sha256']), ['m-cal-1'])
            self.assertNotIn('m-hold-1', audit['input_file_sha256'])

    def test_missing_calibration_never_falls_back_to_holdout(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            runner, programmer, out, serial = build_e2e_run(tmp, stage='method', counter_start='1')
            runner.stage_dir = out / 'method'
            runner.stage_dir.mkdir(parents=True, exist_ok=True)
            d = runner.stage_dir / 'runs' / 'm-hold-1'
            d.mkdir(parents=True)
            (d / 'record.json').write_text(json.dumps({'kind': 'holdout'}))
            cases = [{'id': 'm-hold-1', 'phase': 'holdout', 'needs_selection': True}]
            with self.assertRaises(m2.TechnicalFailure):
                runner._freeze_selection('method', cases)
            self.assertEqual(runner.plan_module.select_calls, [])


class TestVerifyStageSaved(unittest.TestCase):
    def test_every_hash_file_is_required_and_verified(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            runner, programmer, out, serial = build_e2e_run(tmp, stage='e2')
            runner.stage_dir = out / 'e2'
            d = runner.stage_dir / 'runs' / 'e2-short-w0-forward-n0'
            d.mkdir(parents=True)
            (d / 'record.json').write_text('{}')
            (d / 'record.sha256').write_text(m2.sha256_file(d / 'record.json') + '  record.json' + chr(10))
            (d / 'archive.tar.gz').write_bytes(b'fake')
            (d / 'archive.sha256').write_text('0' * 64 + '  archive.tar.gz' + chr(10))
            (d / 'archive.b64').write_bytes(b'ZmFrZQ==')
            (d / 'SAVED').write_text('x')
            with self.assertRaises(m2.TechnicalFailure):
                runner._verify_stage_saved('e2', [{'id': 'e2-short-w0-forward-n0'}])  # metadata.json missing
            (d / 'metadata.json').write_text(json.dumps({'exit': 0, 'taint_after': '0'}))
            with self.assertRaises(m2.TechnicalFailure):
                runner._verify_stage_saved('e2', [{'id': 'e2-short-w0-forward-n0'}])  # bad archive sha
            (d / 'archive.sha256').write_text(m2.sha256_file(d / 'archive.tar.gz') + '  archive.tar.gz' + chr(10))
            runner._verify_stage_saved('e2', [{'id': 'e2-short-w0-forward-n0'}])


class TestEndToEnd(unittest.TestCase):
    def test_both_stages_save_hashed_cases(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            runner, programmer, out, serial = build_e2e_run(tmp)
            self.assertEqual(runner.run(), 0)
            self.assertEqual(programmer.calls, ['role1.bit', 'role2.bit'])
            case_dir = out / 'e2' / 'runs' / 'e2-short-w0-forward-n0'
            for name in ('record.json', 'record.sha256', 'metadata.json', 'archive.tar.gz',
                         'archive.sha256', 'archive.b64', 'SAVED'):
                self.assertTrue((case_dir / name).is_file(), name)
            self.assertEqual(m2.sha256_file(case_dir / 'archive.tar.gz'),
                             (case_dir / 'archive.sha256').read_text().split()[0])
            meta = json.loads((case_dir / 'metadata.json').read_text())
            self.assertEqual(meta['exit'], 0)
            self.assertEqual(meta['taint_after'].strip(), '0')
            self.assertEqual(meta['role'], 1)
            self.assertEqual(meta['bit_sha256'], hashlib.sha256(b'role1').hexdigest())
            self.assertEqual(meta['argv'][0], '--e2-ext')
            self.assertTrue((out / 'e2' / 'plan.json').is_file())
            self.assertTrue((out / 'method' / 'plan.json').is_file())
            self.assertTrue((out / 'method' / 'selection.json').is_file())
            self.assertTrue((out / 'COMPLETE.json').is_file())
            self.assertFalse((out / 'STOPPED.txt').exists())
            self.assertEqual(runner.plan_module.select_calls, [['m-cal-fixedgap-g0']])

    def test_technical_cli_failure_still_archives_then_stops(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            cases = {'e2': [{'id': 'e2-short-w0-forward-n0', 'phase': 'short', 'role': 1,
                             'argv': ['--e2-ext', '--a-pages', '8', 'FAIL']}]}
            runner, programmer, out, serial = build_e2e_run(tmp, stage='e2', cases=cases)
            with self.assertRaises(m2.TechnicalFailure):
                runner.run()
            self.assertEqual(programmer.calls, ['role1.bit'])
            case_dir = out / 'e2' / 'runs' / 'e2-short-w0-forward-n0'
            self.assertTrue((case_dir / 'archive.tar.gz').is_file())
            self.assertTrue((case_dir / 'archive.b64').is_file())
            self.assertTrue((case_dir / 'board-command.txt').is_file())
            self.assertIn('FAIL', (case_dir / 'board-command.txt').read_text())
            self.assertFalse((case_dir / 'SAVED').exists())
            meta = json.loads((case_dir / 'metadata.json').read_text())
            self.assertEqual(meta['exit'], 7)
            self.assertTrue((out / 'STOPPED.txt').is_file())

    def test_first_stage_hash_break_blocks_the_second_flash(self):
        class CorruptingRunner(ShellRunner):
            def _verify_stage_saved(self, stage, cases):
                if stage == 'e2':
                    d = self.stage_dir / 'runs' / cases[0]['id'] / 'archive.tar.gz'
                    d.write_bytes(b'corrupted-after-save')
                super()._verify_stage_saved(stage, cases)

        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            runner, programmer, out, serial = build_e2e_run(tmp)
            runner.__class__ = CorruptingRunner
            with self.assertRaises(m2.TechnicalFailure):
                runner.run()
            self.assertEqual(programmer.calls, ['role1.bit'])
            case_dir = out / 'e2' / 'runs' / 'e2-short-w0-forward-n0'
            self.assertTrue((case_dir / 'record.json').is_file())
            self.assertEqual(m2.sha256_file(case_dir / 'record.json'),
                             (case_dir / 'record.sha256').read_text().split()[0])
            self.assertTrue((out / 'STOPPED.txt').is_file())

    def test_unchanged_boot_id_stops_after_the_second_flash(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            runner, programmer, out, serial = build_e2e_run(tmp, boot_one='boot-same', boot_two='boot-same')
            with self.assertRaises(m2.TechnicalFailure):
                runner.run()
            self.assertEqual(programmer.calls, ['role1.bit', 'role2.bit'])
            self.assertTrue((out / 'e2' / 'runs' / 'e2-short-w0-forward-n0' / 'SAVED').is_file())
            self.assertFalse((out / 'method' / 'runs').exists())

    def test_one_argument_resolve_case_is_not_papered_over(self):
        class OneArgPlan(FakePlanModule):
            def resolve_case(self, case):
                return case

        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            runner, programmer, out, serial = build_e2e_run(tmp, stage='e2')
            runner.plan_module = OneArgPlan(default_cases())
            with self.assertRaises(m2.TechnicalFailure) as ctx:
                runner.run()
            self.assertIn('resolve_case', str(ctx.exception))
            self.assertEqual(programmer.calls, ['role1.bit'])


if __name__ == '__main__':
    unittest.main()
