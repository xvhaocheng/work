#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''Mock tests for scripts/mac_method88.py.

No real board, no real pyserial and no real openFPGALoader are used.  The
serial mock really executes every framed shell line with the host ``/bin/sh``,
so the marker framing, the cwd handling, the exit-code frame and the BusyBox
archive rewrite are exercised for real instead of being regex-matched against a
fake echo.

On hosts without GNU coreutils the mock rewrites ``sha256sum`` to a resolved
sha256 command and turns ``base64 FILE`` into ``base64 < FILE``; the runner\'s
commands themselves are never changed.

This file's FakeResultsModule only validates the IPC/framing flow; raw record
validation is covered by tests/test_research_method88.py.
'''
import hashlib
import importlib.util
import json
import pathlib
import queue
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

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


m88 = load_module(SCRIPTS / 'mac_method88.py', 'mac_method88_under_test')
method88 = m88.method88
plan2 = method88.plan2


# ---------------------------------------------------------------------------
def _sha256_command():
    path = shutil.which('sha256sum')
    if path:
        return shlex.quote(path)
    path = shutil.which('shasum')
    if path:
        return shlex.quote(path) + ' -a 256'
    return 'sha256sum'


_SHA256_CMD = _sha256_command()

# Stand-in shell builtins injected ahead of every framed command.  They live in
# the same /bin/sh that executes the framed body, so the production capture
# gate still runs for real while ``dmesg`` and ``base64 FILE`` keep working on
# an unprivileged host and on hosts without GNU coreutils (BSD/macOS base64
# takes an input file via stdin redirection, not a positional argument).
SHELL_PRELUDE = (
    "dmesg() { echo mock-dmesg; }\n"
    "base64() { command base64 < \"$1\"; }\n"
)

DUMMY_ROLE2_BIT_SHA256 = hashlib.sha256(b'role2-bit-payload').hexdigest()

CAPS_ROLE2 = {
    'schema': 'idma-research2-caps-v1',
    'success': True,
    'magic': '0x49444d4132434150',
    'version': '0x1',
    'role': '0x2',
    'features': '0xf',
    'observer_magic': '0x49444d414f425308',
    'error': '',
}
CAPS_WRONG_FEATURES = dict(CAPS_ROLE2, features='0x7')


class ShellMockSerial:
    '''Serial stand-in that really executes each sent line through /bin/sh.'''

    def __init__(self, taint_file, echo=False, crlf=False, prelude='', cwd=None):
        self._queue = queue.Queue()
        self.written = bytearray()
        self.closed = False
        self.echo = echo
        self.crlf = crlf
        self.prelude = prelude
        self.cwd = cwd
        self.taint_file = str(taint_file)
        self.shell = '/bin/sh'

    def read(self, n):
        try:
            return self._queue.get(timeout=0.02)
        except queue.Empty:
            return b''

    def write(self, data):
        self.written.extend(data)
        for line in data.decode('utf-8', 'replace').split('\n'):
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

    def _rewrite(self, line):
        line = line.replace('/proc/sys/kernel/tainted', shlex.quote(self.taint_file))
        if _SHA256_CMD != 'sha256sum':
            line = line.replace('sha256sum', _SHA256_CMD)
        return line

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
        line = self._rewrite(line)
        try:
            proc = subprocess.run([self.shell, '-c', SHELL_PRELUDE + self.prelude + line],
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
            raise m88.mac2.TechnicalFailure('mock flash failure')
        return {'cmd': ['openFPGALoader', bit_path], 'rc': 0, 'stdout': '', 'stderr': ''}


class FakeResultsModule:
    def validate_record(self, case, record):
        return []

    def metrics(self, record):
        return {'mock': True}


FAKE_CLI_TEMPLATE = '''#!/bin/sh
count=$(cat @@COUNTER@@ 2>/dev/null || echo 0)
for a in "$@"; do
  if [ "$a" = --caps2 ]; then
    count=$((count + 1))
    echo "$count" > @@COUNTER@@
    if [ "$count" -le 1 ]; then
      echo @@BOOT_ONE@@ > @@BOOT@@
      echo @@CAPS_ONE@@
    else
      echo @@BOOT_TWO@@ > @@BOOT@@
      echo @@CAPS_TWO@@
    fi
    exit 0
  fi
  if [ "$a" = FAIL ]; then
    echo simulated-dma-failure 1>&2
    exit 7
  fi
done
echo @@RECORD@@
exit 0
'''


def shlex_quote(value):
    return "'" + str(value).replace("'", "'\\''") + "'"


def host_uname_r():
    proc = subprocess.run(['/bin/sh', '-c', 'uname -r'], stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE)
    return proc.stdout.decode('utf-8', 'replace').strip()


# ---------------------------------------------------------------------------
# Frozen plan / selection fixture
# ---------------------------------------------------------------------------
def make_selection():
    if plan2 is None:
        raise RuntimeError('research2_plan.py is not loadable')
    target = 170
    gaps = sorted(int(g) for g in plan2.FIXED_GAP_VALUES)
    configs = {}
    for method in method88.METHODS:
        if method == method88.REFERENCE_TAG:
            mode = plan2.POLICY_UNLIMITED
            gap = gaps[0]
        else:
            mode = int(method[1:])
            gap = 0
        values = {
            'policy_mode': mode,
            'window_cycles': method88.WINDOW_CYCLES,
            'target_bytes': plan2.target_bytes_for(target),
            'deadband_bytes': method88.DEADBAND_BYTES,
            'initial_budget': 1000,
            'min_budget': 1,
            'max_budget': 16000,
            'budget_step': 125,
            'credit_cap': 32000,
            'hit_weight': 1,
            'miss_weight': 1,
            'ptw_weight': 1,
        }
        entry = {field: values.get(field, 0) for field in plan2.CONFIG_FIELDS}
        entry['gap_cycles'] = gap
        entry['candidate_id'] = None
        entry['matched'] = True
        configs[method] = {str(target): entry}
    return {
        'schema': plan2.SELECTION_SCHEMA,
        'version': plan2.SELECTION_VERSION,
        'window_cycles': method88.WINDOW_CYCLES,
        'configs': configs,
        'unmatched': [],
    }


def build_package(tmp):
    '''Build a self-consistent method88-delivery-v1 package under tmp/pkg.'''
    root = tmp / 'pkg'
    files = {}
    for rel in m88.REQUIRED_FILES:
        if rel.startswith('frozen/'):
            continue
        path = root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        if rel.endswith('.bit'):
            path.write_bytes(b'role2-bit-payload')
        else:
            path.write_text('placeholder for ' + rel + '\n', encoding='utf-8')
        files[rel] = m88.mac2.sha256_file(path)
    selection = make_selection()
    source = root / 'source_selection.json'
    source.write_text(json.dumps(selection, indent=2) + '\n', encoding='utf-8')
    method88.freeze(source, root / 'frozen')
    for name in m88.FROZEN_FILES:
        rel = 'frozen/' + name
        files[rel] = m88.mac2.sha256_file(root / rel)
    manifest = {
        'schema': m88.SCHEMA,
        'images': [{'role': 2, 'bit': 'artifacts/role2.bit',
                    'sha256': files['artifacts/role2.bit']}],
        'software': dict(m88.EXPECTED_SOFTWARE),
        'frozen': {'root': 'frozen'},
        'files': files,
    }
    manifest_path = root / 'DELIVERY.json'
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
    return root, manifest_path, manifest


# ---------------------------------------------------------------------------
# Runner harness
# ---------------------------------------------------------------------------
def write_fake_board_cli(tmp, counter_start, boot_one, boot_two, caps_one, caps_two):
    counter = tmp / 'boot.counter'
    counter.write_text(counter_start)
    boot_file = tmp / 'boot.id'
    boot_file.write_text('boot-zero')
    taint_file = tmp / 'tainted'
    taint_file.write_text('0')
    script = tmp / 'idma_research'
    text = (FAKE_CLI_TEMPLATE
            .replace('@@COUNTER@@', shlex_quote(str(counter)))
            .replace('@@BOOT@@', shlex_quote(str(boot_file)))
            .replace('@@BOOT_ONE@@', boot_one)
            .replace('@@BOOT_TWO@@', boot_two)
            .replace('@@CAPS_ONE@@', shlex_quote(json.dumps(caps_one)))
            .replace('@@CAPS_TWO@@', shlex_quote(json.dumps(caps_two)))
            .replace('@@RECORD@@', shlex_quote(json.dumps({
                'schema': 'idma-research-v1', 'success': True, 'validated': True, 'mode': 'cost'}))))
    script.write_text(text, encoding='utf-8')
    script.chmod(0o755)
    return script, boot_file, taint_file


class Method88TestRunner(m88.Method88Runner):
    '''Real framing core; only the login banner handshake is bypassed so the
    /bin/sh mock does not have to fake a full boot log.'''

    def _wait_for_root(self, timeout):
        return m88.mac2.Research2Runner._wait_for_root(self, timeout)


def make_args(tmp):
    return m88.argparse.Namespace(
        stage='method',
        startup_timeout=10.0,
        command_timeout=10.0,
        case_timeout=30.0,
        transfer_timeout=30.0,
        jtag_freq=1000000,
        out=str(tmp / 'out'),
        serial='/dev/fake',
    )


def build_runner(tmp, frozen_plan, *, manifest=None, boot_one='boot-one', boot_two='boot-two',
                 counter_start='0', caps_one=None, caps_two=None, kernel=None, cli_sha=None,
                 echo=False, programmer=None):
    cli, boot_file, taint_file = write_fake_board_cli(
        tmp, counter_start, boot_one, boot_two,
        caps_one if caps_one is not None else CAPS_ROLE2,
        caps_two if caps_two is not None else CAPS_ROLE2)
    if manifest is None:
        manifest = {
            'schema': m88.SCHEMA,
            'images': [{'role': 2, 'bit': 'artifacts/role2.bit', 'sha256': '0' * 64}],
            'software': {
                'kernel_release': kernel if kernel is not None else host_uname_r(),
                'cli_sha256': cli_sha if cli_sha is not None else m88.mac2.sha256_file(cli),
                'board_cli_path': str(cli),
            },
            'frozen': {'root': 'frozen'},
            'files': {},
        }
    (tmp / 'artifacts').mkdir(parents=True, exist_ok=True)
    (tmp / 'artifacts' / 'role2.bit').write_bytes(b'role2-bit-payload')
    selection = make_selection()
    src = tmp / 'source_selection.json'
    src.write_text(json.dumps(selection, indent=2) + '\n', encoding='utf-8')
    method88.freeze(src, tmp / 'frozen')
    serial = ShellMockSerial(taint_file, echo=echo)
    console = m88.Method88Console('/dev/fake', tmp / 'console.raw.log',
                                  tmp / 'console.events.log',
                                  serial_factory=lambda *a, **k: serial)
    runner = Method88TestRunner(make_args(tmp), manifest, tmp, tmp / 'out', console,
                               programmer if programmer is not None else MockProgrammer(),
                               method88, FakeResultsModule(),
                               frozen_plan=frozen_plan, frozen_selection=selection)
    runner.board_root = str(tmp / 'board')
    runner.boot_id_path = str(boot_file)
    return runner, serial, boot_file


def frozen_plan_from_selection(tmp):
    selection = make_selection()
    src = tmp / 'sel.json'
    src.write_text(json.dumps(selection, indent=2) + '\n', encoding='utf-8')
    out = tmp / 'frozen_src'
    method88.freeze(src, out)
    return json.loads((out / 'plan.json').read_text(encoding='utf-8'))


def synthetic_plan(first_argv=None, per_boot=1):
    cases = []
    for boot in (1, 2):
        for index in range(per_boot):
            argv = ['--method', '--a-pages', '8']
            if boot == 1 and index == 0 and first_argv is not None:
                argv = list(first_argv)
            cases.append({'id': 'm88-b%d-c%02d' % (boot, index), 'boot': boot,
                          'phase': 'short' if index == 0 else 'long', 'argv': argv})
    for ordinal, case in enumerate(cases, start=1):
        case['ordinal'] = ordinal
    return {'schema': method88.PLAN88_SCHEMA, 'count': len(cases), 'cases': cases}


# ---------------------------------------------------------------------------
class TestBusyBoxRecipe(unittest.TestCase):
    def test_exact_six_token_command_is_rewritten_without_z(self):
        command = 'tar -czf /tmp/x/a.tar.gz -C /tmp/x/case .'
        out = m88._busybox_tar_command(command)
        self.assertNotIn('-czf', out)
        self.assertNotIn(' -z', out)
        self.assertIn('tar -cf', out)
        self.assertIn('gzip -c', out)
        self.assertIn('test ! -e', out)
        self.assertIn('> /tmp/x/a.tar.gz', out)
        self.assertIn('.plain.tar', out)

    def test_quoting_survives_spaces_and_quotes(self):
        command = "tar -czf '/tmp/a b/x.tar.gz' -C '/tmp/c d/e' ."
        out = m88._busybox_tar_command(command)
        self.assertIn(shlex.quote('/tmp/a b/x.tar.gz.plain.tar'), out)
        self.assertIn(shlex.quote('/tmp/a b/x.tar.gz'), out)
        self.assertIn(shlex.quote('/tmp/c d/e'), out)

    def test_non_tar_command_is_untouched(self):
        self.assertIsNone(m88._busybox_tar_command('sync'))
        self.assertIsNone(m88._busybox_tar_command('sha256sum record.json'))

    def test_malformed_tar_command_is_refused(self):
        with self.assertRaises(m88.mac2.TechnicalFailure):
            m88._busybox_tar_command('tar -czf /tmp/a.tar.gz -C /tmp/case')
        with self.assertRaises(m88.mac2.TechnicalFailure):
            m88._busybox_tar_command('tar -czf /tmp/a.tar.gz -X /tmp/case .')

    def test_console_preserves_cwd_and_exit_var(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            taint = tmp / 'tainted'
            taint.write_text('0')
            console = m88.Method88Console('/dev/fake', tmp / 'raw.log', tmp / 'events.log',
                                          serial_factory=lambda *a, **k: ShellMockSerial(taint))
            framed = []
            original = console._run_framed

            def spy(full_command, begin, end, timeout):
                framed.append(full_command)
                return m88.mac2.ShellResult(b'', 0)

            console._run_framed = spy
            console.run_shell('tar -czf T -C S .', timeout=1.0, cwd='/work', exit_var='ec')
            self.assertIn('tar -cf', framed[-1])
            self.assertIn('cd /work', framed[-1])
            self.assertIn('ec=', framed[-1])
            framed.clear()
            console.run_shell('sync', timeout=1.0)
            self.assertIn('sync', framed[-1])
            self.assertNotIn('tar -cf', framed[-1])


# ---------------------------------------------------------------------------
class TestManifest(unittest.TestCase):
    def setUp(self):
        patcher = mock.patch.object(m88, 'ROLE2_BIT_SHA256', DUMMY_ROLE2_BIT_SHA256)
        patcher.start()
        self.addCleanup(patcher.stop)

    def test_accepts_a_consistent_package(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            root, manifest_path, _ = build_package(tmp)
            manifest = m88.load_manifest(manifest_path)
            verified = m88.verify_package_files(manifest, root)
            self.assertIn('image[2]', verified)
            self.assertIn('file[artifacts/role2.bit]', verified)
            frozen = m88.verify_frozen(manifest, root)
            self.assertEqual(frozen['plan']['count'], 88)

    def test_schema_and_software_are_pinned(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            _, manifest_path, manifest = build_package(tmp)
            bad = dict(manifest, schema='other')
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)
            bad = json.loads(json.dumps(manifest))
            bad['software']['kernel_release'] = 'other-kernel'
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)

    def test_only_role_two_is_allowed(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            _, manifest_path, manifest = build_package(tmp)
            bad = json.loads(json.dumps(manifest))
            bad['images'] = [dict(bad['images'][0], role=1)]
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)
            bad = json.loads(json.dumps(manifest))
            bad['images'].append(dict(bad['images'][0]))
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)

    def test_unpinned_bit_path_and_hash_are_refused(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            _, manifest_path, manifest = build_package(tmp)
            bad = json.loads(json.dumps(manifest))
            bad['images'][0]['bit'] = 'artifacts/other.bit'
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)
            bad = json.loads(json.dumps(manifest))
            bad['images'][0]['sha256'] = '0' * 64
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)
            bad = json.loads(json.dumps(manifest))
            bad['files']['artifacts/role2.bit'] = '0' * 64
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)
            bad = json.loads(json.dumps(manifest))
            bad['frozen']['root'] = 'other'
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)

    def test_unsafe_file_paths_and_bad_hashes_are_refused(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            _, manifest_path, manifest = build_package(tmp)
            bad = json.loads(json.dumps(manifest))
            bad['files']['../escape'] = '0' * 64
            del bad['files']['docs/mac-method88.md']
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)
            bad = json.loads(json.dumps(manifest))
            bad['files']['docs/mac-method88.md'] = 'ZZ' * 32
            manifest_path.write_text(json.dumps(bad), encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.load_manifest(manifest_path)

    def test_tampered_payload_hash_is_refused(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            root, manifest_path, manifest = build_package(tmp)
            (root / 'docs/mac-method88.md').write_text('tampered\n', encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.verify_package_files(manifest, root)

    def test_tampered_plan_or_selection_is_refused(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            root, _, manifest = build_package(tmp)
            plan_path = root / 'frozen' / 'plan.json'
            plan = json.loads(plan_path.read_text(encoding='utf-8'))
            plan['cases'][0]['argv'] = ['--tampered']
            plan_path.write_text(json.dumps(plan, indent=2) + '\n', encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.verify_frozen(manifest, root)
            # Build a fresh, independent package for the selection tamper.
            other = tmp / 'other'
            other.mkdir()
            fresh_root, _, fresh_manifest = build_package(other)
            selection_path = fresh_root / 'frozen' / 'selection.json'
            selection = json.loads(selection_path.read_text(encoding='utf-8'))
            selection['window_cycles'] = 1
            selection_path.write_text(json.dumps(selection, indent=2) + '\n', encoding='utf-8')
            with self.assertRaises(m88.mac2.ManifestError):
                m88.verify_frozen(fresh_manifest, fresh_root)

    def test_symlink_escape_is_refused(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            root, _, manifest = build_package(tmp)
            outside = tmp / 'outside.md'
            outside.write_text('x', encoding='utf-8')
            (root / 'docs/mac-method88.md').unlink()
            (root / 'docs/mac-method88.md').symlink_to(outside)
            with self.assertRaises(m88.mac2.ManifestError):
                m88.verify_package_files(manifest, root)


# ---------------------------------------------------------------------------
class TestPreflight(unittest.TestCase):
    def setUp(self):
        patcher = mock.patch.object(m88, 'ROLE2_BIT_SHA256', DUMMY_ROLE2_BIT_SHA256)
        patcher.start()
        self.addCleanup(patcher.stop)

    def test_preflight_reads_only(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            root, manifest_path, _ = build_package(tmp)
            probes = []
            with mock.patch.object(m88, '_pyserial_available', lambda: True), \
                    mock.patch.object(m88, '_serial_probe', lambda port: probes.append(port)), \
                    mock.patch('shutil.which', return_value='/usr/bin/openFPGALoader'):
                after = list(root.iterdir())
                result = m88.run_preflight(manifest_path, root, '/dev/fake')
                self.assertEqual(probes, ['/dev/fake'])
                self.assertEqual(sorted(p.name for p in root.iterdir()),
                                 sorted(p.name for p in after))
            self.assertEqual(result['kernel_release'], m88.BASELINE_KERNEL_RELEASE)
            self.assertEqual(result['cases'], 88)
            self.assertEqual(result['board_commands_run'], 0)
            self.assertEqual(result['plan_sha256'],
                             json.loads((root / 'frozen' / 'plan.json').read_text())['plan_sha256'])
            self.assertFalse((root / 'out').exists())

    def test_preflight_refuses_a_free_serial_failure(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            root, manifest_path, _ = build_package(tmp)
            with mock.patch.object(m88, '_pyserial_available', lambda: True), \
                    mock.patch.object(m88, '_serial_probe',
                                      lambda port: (_ for _ in ()).throw(OSError('busy'))), \
                    mock.patch('shutil.which', return_value='/usr/bin/openFPGALoader'):
                with self.assertRaises(m88.mac2.PreflightError):
                    m88.run_preflight(manifest_path, root, '/dev/fake')


# ---------------------------------------------------------------------------
class TestRunnerOrder(unittest.TestCase):
    def test_full_88_two_role2_boots(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            plan = frozen_plan_from_selection(tmp)
            runner, serial, boot_file = build_runner(tmp, plan)
            self.assertEqual(runner.run(), 0)
            self.assertEqual(len(runner.programmer.calls), 2)
            self.assertTrue(all('role2.bit' in c for c in runner.programmer.calls))
            out = tmp / 'out'
            saved = sorted((out / 'method' / 'runs').iterdir())
            self.assertEqual(len(saved), 88)
            for case_dir in saved:
                self.assertTrue((case_dir / 'SAVED').is_file(), case_dir)
            self.assertEqual(sorted(runner.boots), [1, 2])
            self.assertNotEqual(runner.boots[1]['boot_id'], runner.boots[2]['boot_id'])
            for boot in (1, 2):
                self.assertTrue((out / 'method' / 'boots' / (str(boot) + '.json')).is_file())
                self.assertTrue((out / 'method' / 'boots' / (str(boot) + '.flash.json')).is_file())
            self.assertTrue((out / 'run.json').is_file())
            self.assertTrue((out / 'method' / 'plan.json').is_file())
            self.assertTrue((out / 'method' / 'selection.json').is_file())
            complete = json.loads((out / 'COMPLETE.json').read_text(encoding='utf-8'))
            self.assertEqual(complete['boots'], {'1': runner.boots[1]['boot_id'],
                                                 '2': runner.boots[2]['boot_id']})
            self.assertFalse((out / 'STOPPED.txt').exists())
            meta = json.loads((saved[0] / 'metadata.json').read_text(encoding='utf-8'))
            self.assertIn(meta['boot'], (1, 2))
            self.assertEqual(meta['role'], 2)
            self.assertIn('kernel_release', meta)
            self.assertEqual(meta['kernel_release'],
                             runner.boots[meta['boot']]['uname_r'])

    def test_unchanged_boot_id_stops_before_boot_two_cases(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            with mock.patch.object(m88, 'CASES_PER_BOOT', 1), \
                    mock.patch.object(m88, 'TOTAL_CASES', 2):
                runner, serial, boot_file = build_runner(
                    tmp, synthetic_plan(), boot_one='same-boot', boot_two='same-boot')
                with self.assertRaises(m88.mac2.TechnicalFailure):
                    runner.run()
            self.assertEqual(len(runner.programmer.calls), 2)
            self.assertEqual(len(runner.completed_cases), 1)
            self.assertTrue((tmp / 'out' / 'method' / 'runs' / 'm88-b1-c00' / 'SAVED').is_file())
            self.assertFalse((tmp / 'out' / 'method' / 'runs' / 'm88-b2-c00').exists())
            self.assertTrue((tmp / 'out' / 'STOPPED.txt').is_file())

    def test_first_stage_hash_break_blocks_second_flash(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            with mock.patch.object(m88, 'CASES_PER_BOOT', 1), \
                    mock.patch.object(m88, 'TOTAL_CASES', 2):
                runner, serial, boot_file = build_runner(tmp, synthetic_plan())
                original = runner._verify_stage_saved

                def corrupt(stage, cases):
                    target = runner.stage_dir / 'runs' / cases[0]['id'] / 'archive.tar.gz'
                    target.write_bytes(b'corrupted-after-save')
                    return original(stage, cases)

                runner._verify_stage_saved = corrupt
                with self.assertRaises(m88.mac2.TechnicalFailure):
                    runner.run()
            self.assertEqual(len(runner.programmer.calls), 1)
            self.assertTrue((tmp / 'out' / 'STOPPED.txt').is_file())

    def test_sync_failure_blocks_second_flash(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            with mock.patch.object(m88, 'CASES_PER_BOOT', 1), \
                    mock.patch.object(m88, 'TOTAL_CASES', 2):
                runner, serial, boot_file = build_runner(tmp, synthetic_plan())
                real_sync = runner._sync

                def failing_sync():
                    raise m88.mac2.TechnicalFailure('mock sync failure')

                runner._sync = failing_sync
                with self.assertRaises(m88.mac2.TechnicalFailure):
                    runner.run()
            self.assertEqual(len(runner.programmer.calls), 1)
            self.assertTrue((tmp / 'out' / 'STOPPED.txt').is_file())

    def test_wrong_kernel_stops_before_any_case(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            with mock.patch.object(m88, 'CASES_PER_BOOT', 1), \
                    mock.patch.object(m88, 'TOTAL_CASES', 2):
                runner, serial, boot_file = build_runner(tmp, synthetic_plan(),
                                                         kernel='not-the-board-kernel')
                with self.assertRaises(m88.mac2.TechnicalFailure):
                    runner.run()
            self.assertEqual(runner.completed_cases, [])
            self.assertTrue((tmp / 'out' / 'STOPPED.txt').is_file())

    def test_wrong_cli_hash_stops_before_any_case(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            with mock.patch.object(m88, 'CASES_PER_BOOT', 1), \
                    mock.patch.object(m88, 'TOTAL_CASES', 2):
                runner, serial, boot_file = build_runner(tmp, synthetic_plan(), cli_sha='0' * 64)
                with self.assertRaises(m88.mac2.TechnicalFailure):
                    runner.run()
            self.assertEqual(runner.completed_cases, [])

    def test_wrong_caps_stops_before_any_case(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            with mock.patch.object(m88, 'CASES_PER_BOOT', 1), \
                    mock.patch.object(m88, 'TOTAL_CASES', 2):
                runner, serial, boot_file = build_runner(tmp, synthetic_plan(),
                                                         caps_one=CAPS_WRONG_FEATURES)
                with self.assertRaises(m88.mac2.TechnicalFailure):
                    runner.run()
            self.assertEqual(runner.completed_cases, [])

    def test_technical_case_failure_is_not_retried(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            with mock.patch.object(m88, 'CASES_PER_BOOT', 1), \
                    mock.patch.object(m88, 'TOTAL_CASES', 2):
                runner, serial, boot_file = build_runner(
                    tmp, synthetic_plan(first_argv=['--method', 'FAIL']))
                with self.assertRaises(m88.mac2.TechnicalFailure):
                    runner.run()
            self.assertEqual(len(runner.programmer.calls), 1)
            case_dir = tmp / 'out' / 'method' / 'runs' / 'm88-b1-c00'
            self.assertTrue((case_dir / 'archive.tar.gz').is_file())
            self.assertTrue((case_dir / 'archive.b64').is_file())
            self.assertFalse((case_dir / 'SAVED').exists())
            meta = json.loads((case_dir / 'metadata.json').read_text(encoding='utf-8'))
            self.assertEqual(meta['exit'], 7)
            self.assertTrue((tmp / 'out' / 'STOPPED.txt').is_file())

    def test_plan_grouping_is_enforced(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            plan = dict(synthetic_plan(per_boot=3))
            runner, serial, boot_file = build_runner(tmp, plan)
            with self.assertRaises(m88.mac2.TechnicalFailure):
                runner.run()
            self.assertEqual(runner.programmer.calls, [])

    def test_flash_drains_input_before_flashing(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            events = []

            class SpyConsole:
                def drain_input(self):
                    events.append('drain')
                    return b''

            class SpyProgrammer:
                def flash(self, path):
                    events.append('flash')
                    return {}

            runner, serial, boot_file = build_runner(tmp, synthetic_plan())
            runner.console = SpyConsole()
            runner.programmer = SpyProgrammer()
            runner.out_dir.mkdir(parents=True, exist_ok=True)
            runner.plan_boot = 1
            runner._flash({'role': 2, 'bit': 'artifacts/role2.bit', 'sha256': '0' * 64})
            self.assertEqual(events, ['drain', 'flash'])

    def test_existing_output_directory_is_refused(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = pathlib.Path(td)
            existing = tmp / 'out'
            existing.mkdir()
            with self.assertRaises(SystemExit):
                m88.main(['--manifest', str(tmp / 'DELIVERY.json'), '--package-root', str(tmp),
                          '--out', str(existing), '--serial', '/dev/fake'])


if __name__ == '__main__':
    unittest.main()
