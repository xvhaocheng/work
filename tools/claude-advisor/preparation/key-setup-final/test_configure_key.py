#!/usr/bin/env python3
'''Offline tests for configure_key.py and for the bridge effort support.

No test in this file touches the network, the real user home directory, a real
API key, or the real codex CLI: the TTY probe, getpass, the HTTP call and
subprocess are all patched. Run with:

    python3 -m unittest test_configure_key

These tests have not been executed yet by their author.
'''

import io
import json
import os
import stat
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))

import bridge  # noqa: E402
import configure_key  # noqa: E402


NEW_KEY = 'sk-new-canary-key'
OLD_KEY = 'sk-old-canary-key'
CODEX_CANARY = 'CODEX-OUTPUT-CANARY'


class FakeResponse:
    def __init__(self, status, body):
        self.status = status
        self._body = body

    def read(self):
        return self._body

    def close(self):
        pass


class FakeOpener:
    '''Stands in for urllib.request.OpenerDirector. Never touches the network.'''

    def __init__(self, payload, status=200):
        self.payload = payload
        self.status = status
        self.requests = []

    def open(self, request, timeout=None):
        self.requests.append(request)
        return FakeResponse(self.status, json.dumps(self.payload).encode('utf-8'))


class FakeTty:
    def isatty(self):
        return True


def message_response(model, text='OK', stop_reason='end_turn'):
    return {
        'id': 'msg_test',
        'type': 'message',
        'role': 'assistant',
        'model': model,
        'content': [{'type': 'text', 'text': text}],
        'stop_reason': stop_reason,
        'usage': {'input_tokens': 11, 'output_tokens': 1},
    }


def completed(args, returncode, stdout='', stderr=''):
    return subprocess.CompletedProcess(args, returncode, stdout=stdout, stderr=stderr)


class InstallerCase(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.tmp_path = Path(self._tmp.name)
        self.home = self.tmp_path / 'home'
        (self.home / '.config').mkdir(parents=True)
        self.repo = self.tmp_path / 'repo'
        self.repo.mkdir()
        self.codex_config_path = self.home / '.codex' / 'config.toml'
        self.config_path = self.repo / 'config.json'
        self.preparation_dir = self.repo / 'preparation'
        self.status_path = self.preparation_dir / 'connection-status.json'
        self.write_config()

        self._patchers = [
            # HOME is never repurposed: only Path.home() is redirected, so no
            # child process or library can inherit a fake home directory.
            mock.patch.object(Path, 'home', return_value=self.home),
            mock.patch.object(configure_key, 'CONFIG_PATH', self.config_path),
            mock.patch.object(configure_key, 'PREPARATION_DIR', self.preparation_dir),
            mock.patch.object(configure_key, 'STATUS_PATH', self.status_path),
        ]
        for patcher in self._patchers:
            patcher.start()

    def tearDown(self):
        for patcher in reversed(self._patchers):
            patcher.stop()
        self._tmp.cleanup()

    def default_key_path(self):
        return self.home / '.config' / 'claude-advisor' / 'anthropic.key'

    def write_config(self, **overrides):
        config = {
            'base_url': 'https://api.anthropic.com',
            'model': 'claude-opus-5-5',
            'api_key_env': 'ANTHROPIC_API_KEY',
            'api_key_file': None,
            'api_version': '2023-06-01',
            'workspace_id': None,
            'max_tokens': 16384,
            'thinking': {'type': 'adaptive'},
            'effort': 'high',
            'timeout_seconds': 600,
        }
        config.update(overrides)
        self.config_path.write_text(json.dumps(config, indent=2), encoding='utf-8')
        return self.config_path

    def start_common_patches(self, tty=True, key=NEW_KEY):
        tty_patcher = mock.patch.object(configure_key, 'tty_available', return_value=tty)
        pass_patcher = mock.patch.object(configure_key.getpass, 'getpass', return_value=key)
        tty_patcher.start()
        pass_mock = pass_patcher.start()
        self.addCleanup(tty_patcher.stop)
        self.addCleanup(pass_patcher.stop)
        return pass_mock

    def run_main(self):
        out = io.StringIO()
        err = io.StringIO()
        with mock.patch.object(sys, 'stdout', out), mock.patch.object(sys, 'stderr', err):
            code = configure_key.main([])
        return code, out.getvalue(), err.getvalue()

    def write_codex_config(self, text=None):
        path = self.codex_config_path
        path.parent.mkdir(parents=True, exist_ok=True)
        if text is None:
            text = '''[mcp_servers.claude-advisor]
command = "/usr/bin/python3"
args = ["/tmp/bridge.py", "--serve", "--config", "/tmp/config.json"]
tool_timeout_sec = 60.0

[other_server]
command = "/bin/true"
'''
        path.write_text(text, encoding='utf-8')
        return path


class TtyAndInputTests(InstallerCase):
    def test_default_key_path_is_under_home_config(self):
        self.assertEqual(
            configure_key.default_key_path(),
            self.home / '.config' / 'claude-advisor' / 'anthropic.key',
        )

    def test_tty_probe_rejects_pipe_without_dev_tty(self):
        with mock.patch.object(sys, 'stdin', io.StringIO()), \
             mock.patch.object(configure_key.os, 'open', side_effect=OSError('no controlling terminal')):
            self.assertFalse(configure_key.tty_available())

    def test_tty_probe_accepts_real_tty(self):
        with mock.patch.object(sys, 'stdin', FakeTty()):
            self.assertTrue(configure_key.tty_available())

    def test_no_tty_fails_without_getpass_or_request(self):
        pass_mock = self.start_common_patches(tty=False)
        with mock.patch.object(configure_key.bridge, 'call_anthropic') as call_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(pass_mock.call_count, 0)
        self.assertEqual(call_mock.call_count, 0)
        summary = json.loads(out)
        self.assertFalse(summary['keyconfigured'])
        self.assertFalse(summary['registered'])
        self.assertEqual(summary['errors'][0]['category'], 'no_tty')
        self.assertFalse(self.default_key_path().exists())
        self.assertFalse(self.preparation_dir.exists())
        self.assertIsNone(json.loads(self.config_path.read_text(encoding='utf-8'))['api_key_file'])

    def test_command_line_arguments_are_rejected(self):
        out = io.StringIO()
        err = io.StringIO()
        with mock.patch.object(sys, 'stdout', out), mock.patch.object(sys, 'stderr', err):
            code = configure_key.main(['sk-should-not-be-on-argv'])
        self.assertEqual(code, 1)
        out_text = out.getvalue()
        self.assertNotIn('sk-should-not-be-on-argv', out_text)
        self.assertNotIn('sk-should-not-be-on-argv', err.getvalue())
        self.assertEqual(json.loads(out_text)['errors'][0]['category'], 'unexpected_arguments')

    def test_getpass_prompt_is_the_chinese_prompt(self):
        pass_mock = self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic',
                               return_value=message_response('claude-opus-5-5')), \
             mock.patch.object(configure_key.subprocess, 'run', side_effect=FileNotFoundError('no codex')):
            self.run_main()
        pass_mock.assert_called_once_with(configure_key.KEY_PROMPT)
        self.assertEqual(configure_key.KEY_PROMPT, '请粘贴 Anthropic API 密钥（输入不会显示）：')

    def test_empty_pasted_value_fails_safely(self):
        self.start_common_patches(key='   ')
        with mock.patch.object(configure_key.bridge, 'call_anthropic') as call_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'empty_key')
        self.assertFalse(self.default_key_path().exists())

    def test_key_with_illegal_header_characters_is_rejected_before_request(self):
        self.start_common_patches(key='sk bad value')
        with mock.patch.object(configure_key.bridge, 'call_anthropic') as call_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'bad_key')
        self.assertNotIn('sk bad value', out)
        self.assertFalse(self.default_key_path().exists())


class ConfigGuardTests(InstallerCase):
    def _run_without_key_input(self):
        pass_mock = self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic') as call_mock, \
             mock.patch.object(configure_key.subprocess, 'run') as run_mock:
            code, out, err = self.run_main()
        return code, out, err, pass_mock, call_mock, run_mock

    def test_missing_config_fails_safely(self):
        self.config_path.unlink()
        code, out, err, pass_mock, call_mock, run_mock = self._run_without_key_input()
        self.assertEqual(code, 1)
        self.assertEqual(pass_mock.call_count, 0)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(run_mock.call_count, 0)
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'config_error')

    def test_missing_model_fails_safely(self):
        self.write_config(model=None)
        code, out, err, pass_mock, call_mock, run_mock = self._run_without_key_input()
        self.assertEqual(code, 1)
        self.assertEqual(pass_mock.call_count, 0)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'config_error')

    def test_invalid_effort_fails_safely_without_echoing_value(self):
        self.write_config(effort='turbo')
        code, out, err, pass_mock, call_mock, run_mock = self._run_without_key_input()
        self.assertEqual(code, 1)
        self.assertEqual(pass_mock.call_count, 0)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'config_error')
        self.assertNotIn('turbo', out)

    def test_key_path_inside_workspace_is_rejected(self):
        self.write_config(api_key_file=str(self.repo / 'anthropic.key'))
        self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'default_workspace_roots',
                               return_value=[self.repo]), \
             mock.patch.object(configure_key.bridge, 'call_anthropic') as call_mock, \
             mock.patch.object(configure_key.subprocess, 'run') as run_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(run_mock.call_count, 0)
        self.assertFalse((self.repo / 'anthropic.key').exists())
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'bad_key_path')

    def test_symlinked_key_directory_is_rejected(self):
        elsewhere = self.tmp_path / 'elsewhere'
        elsewhere.mkdir()
        link = self.home / '.config' / 'claude-advisor'
        link.symlink_to(elsewhere)
        self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic') as call_mock, \
             mock.patch.object(configure_key.subprocess, 'run') as run_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(run_mock.call_count, 0)
        self.assertEqual(list(elsewhere.iterdir()), [])
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'bad_key_dir')

    def test_symlinked_ancestor_config_directory_is_rejected(self):
        elsewhere = self.tmp_path / 'elsewhere-config'
        elsewhere.mkdir()
        config_dir = self.home / '.config'
        config_dir.rmdir()
        config_dir.symlink_to(elsewhere)
        pass_mock = self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic') as call_mock:
            with mock.patch.object(configure_key.subprocess, 'run') as run_mock:
                code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(pass_mock.call_count, 0)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(run_mock.call_count, 0)
        self.assertEqual(list(elsewhere.iterdir()), [])
        self.assertFalse((elsewhere / 'claude-advisor').exists())
        self.assertFalse((elsewhere / 'claude-advisor' / 'anthropic.key').exists())
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'bad_key_dir')

    def test_symlinked_key_file_is_rejected_without_overwrite(self):
        real_target = self.tmp_path / 'real-key'
        real_target.write_text(OLD_KEY + '\n', encoding='utf-8')
        os.chmod(real_target, 0o600)
        key_path = self.default_key_path()
        key_path.parent.mkdir(parents=True)
        key_path.symlink_to(real_target)
        self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic') as call_mock, \
             mock.patch.object(configure_key.subprocess, 'run') as run_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(run_mock.call_count, 0)
        self.assertTrue(key_path.is_symlink())
        self.assertEqual(real_target.read_text(encoding='utf-8'), OLD_KEY + '\n')
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'bad_key_file')

    def test_world_readable_existing_key_is_rejected(self):
        key_path = self.default_key_path()
        key_path.parent.mkdir(parents=True)
        key_path.write_text(OLD_KEY + '\n', encoding='utf-8')
        os.chmod(key_path, 0o644)
        self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic') as call_mock, \
             mock.patch.object(configure_key.subprocess, 'run') as run_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(call_mock.call_count, 0)
        self.assertEqual(run_mock.call_count, 0)
        self.assertEqual(key_path.read_text(encoding='utf-8'), OLD_KEY + '\n')


class ConnectivityTests(InstallerCase):
    def test_failed_connectivity_keeps_old_key_and_config(self):
        key_path = self.default_key_path()
        key_path.parent.mkdir(parents=True)
        key_path.write_text(OLD_KEY + '\n', encoding='utf-8')
        os.chmod(key_path, 0o600)
        config_before = self.config_path.read_text(encoding='utf-8')

        self.start_common_patches()
        error = bridge.BridgeError(
            'http_error', 'provider returned HTTP status 401', status=401)
        with mock.patch.object(configure_key.bridge, 'call_anthropic',
                               side_effect=error) as call_mock, \
             mock.patch.object(configure_key.subprocess, 'run') as run_mock:
            code, out, err = self.run_main()

        self.assertEqual(code, 1)
        self.assertEqual(call_mock.call_count, 1)
        self.assertEqual(run_mock.call_count, 0)
        self.assertEqual(key_path.read_text(encoding='utf-8'), OLD_KEY + '\n')
        self.assertEqual(self.config_path.read_text(encoding='utf-8'), config_before)
        summary = json.loads(out)
        self.assertFalse(summary['keyconfigured'])
        self.assertFalse(summary['registered'])
        self.assertEqual(summary['errors'][0]['category'], 'http_error')
        self.assertEqual(summary['errors'][0]['status'], 401)
        self.assertNotIn(NEW_KEY, out)
        self.assertNotIn(NEW_KEY, err)
        self.assertTrue(self.status_path.exists())
        status_text = self.status_path.read_text(encoding='utf-8')
        self.assertNotIn(NEW_KEY, status_text)
        self.assertIn('http_error', status_text)

    def test_model_mismatch_is_rejected(self):
        self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic',
                               return_value=message_response('claude-lite-1')), \
             mock.patch.object(configure_key.subprocess, 'run') as run_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(run_mock.call_count, 0)
        summary = json.loads(out)
        self.assertEqual(summary['errors'][0]['category'], 'model_mismatch')
        self.assertIn('claude-lite-1', summary['errors'][0]['message'])
        self.assertIn('claude-opus-5-5', summary['errors'][0]['message'])
        self.assertNotIn(NEW_KEY, out)
        self.assertFalse(self.default_key_path().exists())

    def test_truncated_response_is_not_counted_as_connected(self):
        self.start_common_patches()
        response = message_response('claude-opus-5-5', stop_reason='max_tokens')
        with mock.patch.object(configure_key.bridge, 'call_anthropic', return_value=response), \
             mock.patch.object(configure_key.subprocess, 'run') as run_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(run_mock.call_count, 0)
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'incomplete_response')
        self.assertFalse(self.default_key_path().exists())

    def test_empty_content_is_not_counted_as_connected(self):
        self.start_common_patches()
        response = message_response('claude-opus-5-5', text='   ')
        with mock.patch.object(configure_key.bridge, 'call_anthropic', return_value=response), \
             mock.patch.object(configure_key.subprocess, 'run') as run_mock:
            code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(run_mock.call_count, 0)
        self.assertEqual(json.loads(out)['errors'][0]['category'], 'invalid_response')

    def test_success_writes_private_key_and_updates_only_api_key_file(self):
        captured = {}

        def fake_call(cfg, payload, api_key, opener=None):
            captured['cfg'] = dict(cfg)
            captured['payload'] = json.loads(json.dumps(payload))
            captured['key'] = api_key
            return message_response(cfg['model'])

        good_entry = {
            'name': 'claude-advisor',
            'enabled': True,
            'transport': {
                'type': 'stdio',
                'command': configure_key.PYTHON_BIN,
                'args': [
                    str(configure_key.BRIDGE_PATH), '--serve',
                    '--config', str(self.config_path),
                ],
                'env': None,
                'env_vars': [],
                'cwd': None,
            },
            'startup_timeout_sec': 20.0,
            'tool_timeout_sec': 660.0,
            'enabled_tools': None,
            'disabled_tools': None,
        }
        executed = []
        get_calls = []

        def fake_run(args, **kwargs):
            executed.append({'args': list(args), 'kwargs': kwargs})
            if list(args)[:3] == ['codex', 'mcp', 'get']:
                get_calls.append(list(args))
                if len(get_calls) == 1:
                    return completed(args, 1, stderr='Error: no mcp server named claude-advisor')
                return completed(args, 0, stdout=json.dumps(good_entry))
            return completed(args, 0, stdout=CODEX_CANARY)

        self.start_common_patches()
        self.write_codex_config()
        with mock.patch.object(configure_key.bridge, 'call_anthropic', side_effect=fake_call), \
             mock.patch.object(configure_key.subprocess, 'run', side_effect=fake_run):
            code, out, err = self.run_main()

        self.assertEqual(code, 0)
        key_path = self.default_key_path()
        self.assertTrue(key_path.is_file())
        self.assertEqual(stat.S_IMODE(key_path.stat().st_mode), 0o600)
        self.assertEqual(stat.S_IMODE(key_path.parent.stat().st_mode), 0o700)
        self.assertEqual(key_path.read_text(encoding='utf-8').strip(), NEW_KEY)

        config_text = self.config_path.read_text(encoding='utf-8')
        config = json.loads(config_text)
        self.assertEqual(config['api_key_file'], str(key_path))
        self.assertEqual(config['max_tokens'], 16384)
        self.assertEqual(config['model'], 'claude-opus-5-5')
        self.assertEqual(config['effort'], 'high')
        self.assertEqual(config['thinking'], {'type': 'adaptive'})
        self.assertNotIn(NEW_KEY, config_text)

        self.assertEqual(captured['payload']['max_tokens'], 512)
        self.assertEqual(captured['cfg']['max_tokens'], 512)
        self.assertEqual(captured['payload']['thinking'], {'type': 'adaptive'})
        self.assertEqual(captured['payload']['output_config'], {'effort': 'high'})
        user_content = captured['payload']['messages'][0]['content']
        self.assertEqual(user_content.count(configure_key.VERIFY_TASK), 1)
        self.assertNotIn('ROLE: research', user_content)
        self.assertEqual(captured['key'], NEW_KEY)

        summary = json.loads(out)
        self.assertTrue(summary['ok'])
        self.assertTrue(summary['key_saved'])
        self.assertTrue(summary['keyconfigured'])
        self.assertTrue(summary['registered'])
        self.assertNotIn(NEW_KEY, out)
        self.assertNotIn(NEW_KEY, err)

        codex_config_text = self.codex_config_path.read_text(encoding='utf-8')
        self.assertIn('tool_timeout_sec = 660', codex_config_text)
        self.assertIn('startup_timeout_sec = 20', codex_config_text)
        self.assertNotIn('tool_timeout_sec = 60.0', codex_config_text)
        self.assertIn('[other_server]', codex_config_text)
        self.assertIn('command = "/bin/true"', codex_config_text)
        self.assertIn('args = ["/tmp/bridge.py", "--serve", "--config", "/tmp/config.json"]', codex_config_text)
        self.assertNotIn(NEW_KEY, codex_config_text)

        status = json.loads(self.status_path.read_text(encoding='utf-8'))
        self.assertEqual(status['status'], 'ok')
        self.assertEqual(status['model'], 'claude-opus-5-5')
        self.assertEqual(status['stop_reason'], 'end_turn')
        self.assertNotIn(NEW_KEY, json.dumps(status))

        add_calls = [entry for entry in executed if entry['args'][:3] == ['codex', 'mcp', 'add']]
        self.assertEqual(len(add_calls), 1)
        self.assertEqual(add_calls[0]['args'], [
            'codex', 'mcp', 'add', 'claude-advisor', '--',
            '/usr/bin/python3', str(configure_key.BRIDGE_PATH),
            '--serve', '--config', str(self.config_path),
        ])
        self.assertTrue(add_calls[0]['kwargs']['capture_output'])
        self.assertEqual(add_calls[0]['kwargs']['timeout'], 20)
        self.assertEqual(add_calls[0]['kwargs']['cwd'], str(configure_key.BRIDGE_DIR))
        self.assertNotIn(CODEX_CANARY, out)
        self.assertNotIn(CODEX_CANARY, err)

    def test_config_update_failure_keeps_new_key_and_reports_not_configured(self):
        self.start_common_patches()
        failure = configure_key.InstallerError('config_error', '配置文件不是合法 JSON，未更新。')
        with mock.patch.object(configure_key.bridge, 'call_anthropic',
                               return_value=message_response('claude-opus-5-5')):
            with mock.patch.object(configure_key, 'update_config_api_key_file',
                                   side_effect=failure):
                with mock.patch.object(configure_key.subprocess, 'run') as run_mock:
                    code, out, err = self.run_main()
        self.assertEqual(code, 1)
        self.assertEqual(run_mock.call_count, 0)
        key_path = self.default_key_path()
        self.assertTrue(key_path.is_file())
        self.assertEqual(stat.S_IMODE(key_path.stat().st_mode), 0o600)
        self.assertEqual(key_path.read_text(encoding='utf-8').strip(), NEW_KEY)
        self.assertIsNone(json.loads(self.config_path.read_text(encoding='utf-8'))['api_key_file'])
        summary = json.loads(out)
        self.assertTrue(summary['key_saved'])
        self.assertFalse(summary['keyconfigured'])
        self.assertFalse(summary['registered'])
        self.assertEqual(summary['errors'][0]['category'], 'config_error')
        self.assertTrue(any('仍待修复' in message for message in summary['messages']))
        self.assertNotIn(NEW_KEY, out)
        self.assertNotIn(NEW_KEY, err)

    def test_unknown_same_name_mcp_entry_is_not_overwritten(self):
        self.write_codex_config()
        foreign_entry = {
            'name': 'claude-advisor',
            'enabled': True,
            'transport': {
                'type': 'stdio',
                'command': '/usr/bin/other-tool',
                'args': ['--serve'],
                'env': None,
                'env_vars': [],
                'cwd': None,
            },
            'startup_timeout_sec': 20.0,
            'tool_timeout_sec': 660.0,
        }
        executed = []

        def fake_run(args, **kwargs):
            executed.append(list(args))
            if list(args)[:3] == ['codex', 'mcp', 'get']:
                return completed(args, 0, stdout=json.dumps(foreign_entry))
            return completed(args, 0, stdout=CODEX_CANARY)

        self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic',
                               return_value=message_response('claude-opus-5-5')), \
             mock.patch.object(configure_key.subprocess, 'run', side_effect=fake_run):
            code, out, err = self.run_main()

        self.assertEqual(code, 0)
        self.assertEqual(executed, [['codex', 'mcp', 'get', 'claude-advisor', '--json']])
        summary = json.loads(out)
        self.assertTrue(summary['key_saved'])
        self.assertTrue(summary['keyconfigured'])
        self.assertFalse(summary['registered'])
        self.assertTrue(any('已原样保留' in message for message in summary['messages']))
        self.assertNotIn(CODEX_CANARY, out)
        self.assertNotIn(CODEX_CANARY, err)

    def test_missing_codex_binary_keeps_keyconfigured_true(self):
        self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic',
                               return_value=message_response('claude-opus-5-5')), \
             mock.patch.object(configure_key.subprocess, 'run',
                               side_effect=FileNotFoundError('no codex')):
            code, out, err = self.run_main()
        self.assertEqual(code, 0)
        summary = json.loads(out)
        self.assertTrue(summary['keyconfigured'])
        self.assertFalse(summary['registered'])
        self.assertTrue(self.default_key_path().is_file())

    def test_subprocess_timeout_keeps_keyconfigured_true(self):
        self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic',
                               return_value=message_response('claude-opus-5-5')), \
             mock.patch.object(configure_key.subprocess, 'run',
                               side_effect=subprocess.TimeoutExpired('codex', 20)):
            code, out, err = self.run_main()
        self.assertEqual(code, 0)
        summary = json.loads(out)
        self.assertTrue(summary['keyconfigured'])
        self.assertFalse(summary['registered'])


class McpRegistrationTests(InstallerCase):
    def entry(self, command=None, args=None, tool_timeout=660.0, startup_timeout=20.0, enabled=True):
        return {
            'name': 'claude-advisor',
            'enabled': enabled,
            'transport': {
                'type': 'stdio',
                'command': configure_key.PYTHON_BIN if command is None else command,
                'args': self.expected_args() if args is None else args,
                'env': None,
                'env_vars': [],
                'cwd': None,
            },
            'startup_timeout_sec': startup_timeout,
            'tool_timeout_sec': tool_timeout,
            'enabled_tools': None,
            'disabled_tools': None,
        }

    def expected_args(self):
        return [
            str(configure_key.BRIDGE_PATH), '--serve',
            '--config', str(self.config_path),
        ]

    def run_installer(self, fake_run):
        self.start_common_patches()
        with mock.patch.object(configure_key.bridge, 'call_anthropic',
                               return_value=message_response('claude-opus-5-5')):
            with mock.patch.object(configure_key.subprocess, 'run', side_effect=fake_run):
                return self.run_main()

    def test_fresh_registration_writes_persistent_timeouts(self):
        config_path = self.write_codex_config()
        executed = []
        get_calls = []

        def fake_run(args, **kwargs):
            executed.append(list(args))
            if list(args)[:3] == ['codex', 'mcp', 'get']:
                get_calls.append(list(args))
                if len(get_calls) == 1:
                    return completed(args, 1, stderr='no mcp server named claude-advisor')
                return completed(args, 0, stdout=json.dumps(self.entry()))
            return completed(args, 0, stdout=CODEX_CANARY)

        code, out, err = self.run_installer(fake_run)

        self.assertEqual(code, 0)
        self.assertEqual(len(get_calls), 2)
        add_calls = [call for call in executed if call[:3] == ['codex', 'mcp', 'add']]
        self.assertEqual(len(add_calls), 1)
        self.assertEqual(add_calls[0], [
            'codex', 'mcp', 'add', 'claude-advisor', '--',
            configure_key.PYTHON_BIN, str(configure_key.BRIDGE_PATH),
            '--serve', '--config', str(self.config_path),
        ])
        text = config_path.read_text(encoding='utf-8')
        self.assertIn('tool_timeout_sec = 660', text)
        self.assertIn('startup_timeout_sec = 20', text)
        self.assertNotIn('tool_timeout_sec = 60.0', text)
        self.assertIn('[other_server]', text)
        self.assertIn('command = "/bin/true"', text)
        self.assertIn('args = ["/tmp/bridge.py", "--serve", "--config", "/tmp/config.json"]', text)
        self.assertNotIn(NEW_KEY, text)
        summary = json.loads(out)
        self.assertTrue(summary['key_saved'])
        self.assertTrue(summary['keyconfigured'])
        self.assertTrue(summary['registered'])
        self.assertNotIn(CODEX_CANARY, out)
        self.assertNotIn(CODEX_CANARY, err)

    def test_matching_existing_entry_is_reused_without_add(self):
        config_path = self.write_codex_config()
        before = config_path.read_text(encoding='utf-8')
        executed = []

        def fake_run(args, **kwargs):
            executed.append(list(args))
            if list(args)[:3] == ['codex', 'mcp', 'get']:
                return completed(args, 0, stdout=json.dumps(self.entry()))
            return completed(args, 0, stdout=CODEX_CANARY)

        code, out, err = self.run_installer(fake_run)

        self.assertEqual(code, 0)
        self.assertEqual(executed, [['codex', 'mcp', 'get', 'claude-advisor', '--json']])
        self.assertEqual(config_path.read_text(encoding='utf-8'), before)
        summary = json.loads(out)
        self.assertTrue(summary['registered'])
        self.assertTrue(summary['keyconfigured'])
        self.assertTrue(any('直接复用' in message for message in summary['messages']))

    def test_existing_entry_with_short_timeouts_gets_timeouts_written(self):
        config_path = self.write_codex_config()
        executed = []
        get_calls = []

        def fake_run(args, **kwargs):
            executed.append(list(args))
            if list(args)[:3] == ['codex', 'mcp', 'get']:
                get_calls.append(list(args))
                if len(get_calls) == 1:
                    return completed(args, 0, stdout=json.dumps(self.entry(tool_timeout=60.0)))
                return completed(args, 0, stdout=json.dumps(self.entry()))
            return completed(args, 0, stdout=CODEX_CANARY)

        code, out, err = self.run_installer(fake_run)

        self.assertEqual(code, 0)
        self.assertFalse(any(call[:3] == ['codex', 'mcp', 'add'] for call in executed))
        self.assertEqual(len(get_calls), 2)
        text = config_path.read_text(encoding='utf-8')
        self.assertIn('tool_timeout_sec = 660', text)
        self.assertIn('startup_timeout_sec = 20', text)
        summary = json.loads(out)
        self.assertTrue(summary['registered'])

    def test_timeout_write_failure_reports_pending_but_key_saved(self):
        executed = []

        def fake_run(args, **kwargs):
            executed.append(list(args))
            if list(args)[:3] == ['codex', 'mcp', 'get']:
                return completed(args, 0, stdout=json.dumps(self.entry(tool_timeout=60.0)))
            return completed(args, 0, stdout=CODEX_CANARY)

        code, out, err = self.run_installer(fake_run)

        self.assertEqual(code, 0)
        self.assertEqual(len(executed), 1)
        summary = json.loads(out)
        self.assertTrue(summary['key_saved'])
        self.assertTrue(summary['keyconfigured'])
        self.assertFalse(summary['registered'])
        self.assertTrue(any('pending' in message for message in summary['messages']))
        self.assertTrue(self.default_key_path().is_file())

    def test_disabled_existing_entry_is_reported_pending(self):
        self.write_codex_config()
        executed = []

        def fake_run(args, **kwargs):
            executed.append(list(args))
            if list(args)[:3] == ['codex', 'mcp', 'get']:
                return completed(args, 0, stdout=json.dumps(self.entry(enabled=False)))
            return completed(args, 0, stdout=CODEX_CANARY)

        code, out, err = self.run_installer(fake_run)

        self.assertEqual(code, 0)
        self.assertEqual(len(executed), 1)
        summary = json.loads(out)
        self.assertFalse(summary['registered'])
        self.assertTrue(summary['keyconfigured'])
        self.assertTrue(any('disabled' in message for message in summary['messages']))

    def test_generic_get_failure_does_not_add(self):
        self.write_codex_config()
        executed = []

        def fake_run(args, **kwargs):
            executed.append(list(args))
            return completed(args, 1, stderr='error: failed to read config.toml')

        code, out, err = self.run_installer(fake_run)

        self.assertEqual(code, 0)
        self.assertEqual(executed, [['codex', 'mcp', 'get', 'claude-advisor', '--json']])
        summary = json.loads(out)
        self.assertFalse(summary['registered'])
        self.assertTrue(summary['keyconfigured'])
        self.assertTrue(any('未执行注册' in message for message in summary['messages']))

    def test_not_found_then_add_failure_reports_pending(self):
        self.write_codex_config()
        executed = []

        def fake_run(args, **kwargs):
            executed.append(list(args))
            if list(args)[:3] == ['codex', 'mcp', 'get']:
                return completed(args, 1, stderr='no mcp server named claude-advisor')
            return completed(args, 1, stderr=CODEX_CANARY)

        code, out, err = self.run_installer(fake_run)

        self.assertEqual(code, 0)
        self.assertEqual(len(executed), 2)
        summary = json.loads(out)
        self.assertFalse(summary['registered'])
        self.assertTrue(summary['keyconfigured'])
        self.assertTrue(any('未注册' in message for message in summary['messages']))
        self.assertNotIn(CODEX_CANARY, out)
        self.assertNotIn(CODEX_CANARY, err)


class RenderTimeoutsTests(unittest.TestCase):
    def test_missing_or_ambiguous_table_is_not_guessed(self):
        other = '''[mcp_servers.other]
x = 1
'''
        commented = '''# [mcp_servers.claude-advisor]
'''
        doubled = '''[mcp_servers.claude-advisor]
tool_timeout_sec = 1

[mcp_servers.claude-advisor]
tool_timeout_sec = 2
'''
        self.assertIsNone(configure_key.render_server_timeouts(other, 660, 20))
        self.assertIsNone(configure_key.render_server_timeouts(commented, 660, 20))
        self.assertIsNone(configure_key.render_server_timeouts(doubled, 660, 20))

    def test_quoted_header_is_supported_and_other_text_is_kept(self):
        text = '''[mcp_servers."claude-advisor"]
command = "x"

[other]
keep = 1
'''
        rendered = configure_key.render_server_timeouts(text, 660, 20)
        self.assertIsNotNone(rendered)
        self.assertTrue(rendered.startswith('[mcp_servers."claude-advisor"]\n'))
        self.assertIn('tool_timeout_sec = 660', rendered)
        self.assertIn('startup_timeout_sec = 20', rendered)
        self.assertIn('command = "x"', rendered)
        self.assertIn('[other]', rendered)
        self.assertIn('keep = 1', rendered)

    def test_supported_headers_are_the_nested_table_only(self):
        self.assertEqual(
            configure_key.MCP_TABLE_HEADERS,
            ('[mcp_servers.claude-advisor]', '[mcp_servers."claude-advisor"]'),
        )

    def test_whole_header_quoted_as_a_single_key_is_refused(self):
        text = '["mcp_servers.claude-advisor"]\ncommand = "x"\n'
        self.assertIsNone(configure_key.render_server_timeouts(text, 660, 20))

    def test_prefix_before_the_table_is_kept_byte_for_byte(self):
        prefix = (
            'model = "keep-this"\n'
            '\n'
            '[mcp_servers.deepseek]\n'
            'command = "keep-worker"\n'
            '\n'
        )
        claude = (
            '[mcp_servers.claude-advisor]\n'
            'command = "/usr/bin/python3"\n'
            'args = ["/tmp/bridge.py"]\n'
            'tool_timeout_sec = 60.0\n'
        )
        suffix = (
            '\n'
            '[mcp_servers.other]\n'
            'command = "keep-other"\n'
            '\n'
            '[settings]\n'
            'note = "中文 note # not a comment"\n'
        )
        rendered = configure_key.render_server_timeouts(prefix + claude + suffix, 660, 20)
        self.assertIsNotNone(rendered)
        self.assertTrue(rendered.startswith(prefix), rendered)
        self.assertTrue(rendered.endswith(suffix), rendered)
        self.assertEqual(rendered.count('tool_timeout_sec ='), 1)
        self.assertEqual(rendered.count('startup_timeout_sec ='), 1)
        self.assertIn('tool_timeout_sec = 660\n', rendered)
        self.assertIn('startup_timeout_sec = 20\n', rendered)
        self.assertNotIn('= 60.0', rendered)
        for kept in (
            'model = "keep-this"',
            '[mcp_servers.deepseek]',
            'command = "keep-worker"',
            'command = "/usr/bin/python3"',
            'args = ["/tmp/bridge.py"]',
            '[mcp_servers.other]',
            'command = "keep-other"',
            '[settings]',
            'note = "中文 note # not a comment"',
        ):
            self.assertIn(kept, rendered)

    def test_table_at_the_end_of_file_keeps_its_prefix(self):
        text = (
            '[settings]\n'
            'model = "keep-this"\n'
            '\n'
            '[mcp_servers."claude-advisor"]\n'
            'command = "keep-worker"'
        )
        self.assertEqual(
            configure_key.render_server_timeouts(text, 660, 20),
            '[settings]\n'
            'model = "keep-this"\n'
            '\n'
            '[mcp_servers."claude-advisor"]\n'
            'tool_timeout_sec = 660\n'
            'startup_timeout_sec = 20\n'
            'command = "keep-worker"',
        )

    def test_header_on_the_last_line_without_newline_is_terminated(self):
        text = '[settings]\nmodel = "keep-this"\n\n[mcp_servers.claude-advisor]'
        self.assertEqual(
            configure_key.render_server_timeouts(text, 660, 20),
            '[settings]\n'
            'model = "keep-this"\n'
            '\n'
            '[mcp_servers.claude-advisor]\n'
            'tool_timeout_sec = 660\n'
            'startup_timeout_sec = 20\n',
        )

    def test_comment_and_nested_subtable_after_the_table_are_kept(self):
        text = (
            '# top\n'
            '[mcp_servers.claude-advisor]\n'
            'startup_timeout_sec = 1\n'
            'command = "/usr/bin/python3"\n'
            '# keep me\n'
            'tool_timeout_sec = 2\n'
            '\n'
            '[mcp_servers.claude-advisor.env]\n'
            'FOO = "bar"\n'
        )
        rendered = configure_key.render_server_timeouts(text, 660, 20)
        self.assertTrue(rendered.startswith('# top\n'))
        self.assertIn('# keep me\n', rendered)
        self.assertIn('command = "/usr/bin/python3"\n', rendered)
        self.assertIn('[mcp_servers.claude-advisor.env]\nFOO = "bar"\n', rendered)
        self.assertEqual(rendered.count('tool_timeout_sec ='), 1)
        self.assertEqual(rendered.count('startup_timeout_sec ='), 1)
        self.assertIn('tool_timeout_sec = 660\n', rendered)
        self.assertIn('startup_timeout_sec = 20\n', rendered)

    def test_crlf_file_keeps_crlf_and_gets_crlf_timeouts(self):
        text = (
            '[settings]\r\n'
            'model = "keep-this"\r\n'
            '\r\n'
            '[mcp_servers.claude-advisor]\r\n'
            'tool_timeout_sec = 60.0\r\n'
            'command = "keep-this-too"\r\n'
        )
        rendered = configure_key.render_server_timeouts(text, 660, 20)
        self.assertEqual(
            rendered,
            '[settings]\r\n'
            'model = "keep-this"\r\n'
            '\r\n'
            '[mcp_servers.claude-advisor]\r\n'
            'tool_timeout_sec = 660\r\n'
            'startup_timeout_sec = 20\r\n'
            'command = "keep-this-too"\r\n',
        )
        self.assertNotIn('\n', rendered.replace('\r\n', ''))

    def test_multiline_prompt_hiding_a_header_lookalike_is_refused(self):
        text = (
            'model = "keep-this"\n'
            '\n'
            '[mcp_servers.deepseek]\n'
            'prompt = """\n'
            '[mcp_servers.claude-advisor]\n'
            '"""\n'
            '\n'
            '[mcp_servers.claude-advisor]\n'
            'command = "/usr/bin/python3"\n'
        )
        self.assertIsNone(configure_key.render_server_timeouts(text, 660, 20))

    def test_literal_multiline_value_is_refused(self):
        text = (
            '[mcp_servers.claude-advisor]\n'
            'command = "/usr/bin/python3"\n'
            "note = '''\n"
            '[mcp_servers.other]\n'
            "'''\n"
        )
        self.assertIsNone(configure_key.render_server_timeouts(text, 660, 20))


class WriteServerTimeoutsTests(unittest.TestCase):
    '''write_server_timeouts against real temporary files: no network, no CLI.'''

    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.dir = Path(self._tmp.name)
        self.path = self.dir / 'config.toml'

    def tearDown(self):
        self._tmp.cleanup()

    def write(self, data, mode=0o600):
        self.path.write_bytes(data)
        os.chmod(self.path, mode)
        return self.path

    def test_prefix_suffix_and_file_mode_are_preserved(self):
        prefix = (
            '# codex config\n'
            'model = "keep-this"\n'
            '\n'
            '[mcp_servers.deepseek]\n'
            'command = "keep-worker"\n'
            'args = ["--serve"]\n'
            '\n'
        ).encode('utf-8')
        claude = (
            '[mcp_servers.claude-advisor]\n'
            'command = "/usr/bin/python3"\n'
            'tool_timeout_sec = 60.0\n'
            'startup_timeout_sec = 5\n'
        ).encode('utf-8')
        suffix = (
            '\n'
            '[mcp_servers.claude-advisor.env]\n'
            'FOO = "bar"\n'
            '\n'
            '[settings]\n'
            'note = "中文 # 注释"\n'
        ).encode('utf-8')
        self.write(prefix + claude + suffix, mode=0o644)

        configure_key.write_server_timeouts(self.path)

        after = self.path.read_bytes()
        self.assertTrue(after.startswith(prefix), after)
        self.assertTrue(after.endswith(suffix), after)
        self.assertEqual(after.count(b'tool_timeout_sec ='), 1)
        self.assertEqual(after.count(b'startup_timeout_sec ='), 1)
        self.assertIn(b'tool_timeout_sec = 660\n', after)
        self.assertIn(b'startup_timeout_sec = 20\n', after)
        self.assertNotIn(b'= 60.0', after)
        self.assertNotIn(b'startup_timeout_sec = 5\n', after)
        self.assertIn(b'command = "/usr/bin/python3"\n', after)
        self.assertIn('中文 # 注释'.encode('utf-8'), after)
        self.assertEqual(stat.S_IMODE(self.path.stat().st_mode), 0o644)

    def test_crlf_file_stays_crlf_after_the_write(self):
        original = (
            '[settings]\r\n'
            'model = "keep-this"\r\n'
            '\r\n'
            '[mcp_servers.claude-advisor]\r\n'
            'tool_timeout_sec = 60.0\r\n'
        ).encode('utf-8')
        self.write(original)

        configure_key.write_server_timeouts(self.path)

        after = self.path.read_bytes()
        self.assertTrue(after.startswith(b'[settings]\r\nmodel = "keep-this"\r\n\r\n'))
        self.assertIn(b'tool_timeout_sec = 660\r\n', after)
        self.assertIn(b'startup_timeout_sec = 20\r\n', after)
        self.assertNotIn(b'\n', after.replace(b'\r\n', b''))

    def test_already_correct_timeouts_leave_the_file_untouched(self):
        original = (
            '[mcp_servers.claude-advisor]\n'
            'tool_timeout_sec = 660\n'
            'startup_timeout_sec = 20\n'
        ).encode('utf-8')
        self.write(original)

        configure_key.write_server_timeouts(self.path)

        self.assertEqual(self.path.read_bytes(), original)
        self.assertEqual(sorted(p.name for p in self.dir.iterdir()), ['config.toml'])

    def test_multiline_lookalike_is_refused_and_the_file_is_untouched(self):
        original = (
            '[settings]\n'
            'prompt = """\n'
            '[mcp_servers.claude-advisor]\n'
            '"""\n'
            '\n'
            '[mcp_servers.claude-advisor]\n'
            'command = "/usr/bin/python3"\n'
        ).encode('utf-8')
        self.write(original)

        with self.assertRaises(configure_key.InstallerError) as ctx:
            configure_key.write_server_timeouts(self.path)

        self.assertEqual(ctx.exception.category, 'mcp_config_error')
        self.assertEqual(self.path.read_bytes(), original)

    def test_whole_quoted_header_is_refused_and_the_file_is_untouched(self):
        original = b'["mcp_servers.claude-advisor"]\ncommand = "x"\n'
        self.write(original)

        with self.assertRaises(configure_key.InstallerError):
            configure_key.write_server_timeouts(self.path)

        self.assertEqual(self.path.read_bytes(), original)

    def test_non_utf8_file_is_refused(self):
        original = b'[mcp_servers.claude-advisor]\nnote = "\xff\xfe"\n'
        self.write(original)

        with self.assertRaises(configure_key.InstallerError) as ctx:
            configure_key.write_server_timeouts(self.path)

        self.assertEqual(ctx.exception.category, 'mcp_config_error')
        self.assertEqual(self.path.read_bytes(), original)


class BridgeEffortTests(unittest.TestCase):
    def test_effort_levels_are_the_fixed_allowlist(self):
        self.assertEqual(bridge.EFFORT_LEVELS, ('low', 'medium', 'high', 'xhigh', 'max'))

    def test_valid_effort_values_are_accepted(self):
        for value in bridge.EFFORT_LEVELS:
            self.assertEqual(bridge.validate_effort(value), [])
        self.assertEqual(bridge.validate_effort(None), [])

    def test_invalid_effort_values_are_rejected(self):
        for value in ['ultra', 'HIGH', 'high ', 3, True, {'effort': 'high'}, ['high']]:
            self.assertTrue(bridge.validate_effort(value), msg=repr(value))

    def test_invalid_effort_value_is_not_echoed(self):
        errors = bridge.validate_effort('extreme-secret-value')
        self.assertTrue(errors)
        self.assertNotIn('extreme-secret-value', json.dumps(errors))

    def test_config_values_accept_effort_and_reject_unknown_value(self):
        self.assertEqual(bridge.validate_config_values({'effort': 'high'}), [])
        self.assertTrue(bridge.validate_config_values({'effort': 'turbo'}))

    def test_load_config_keeps_effort(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'config.json'
            path.write_text(json.dumps({
                'base_url': 'https://api.anthropic.com',
                'model': 'claude-opus-5-5',
                'max_tokens': 16384,
                'effort': 'high',
            }), encoding='utf-8')
            info = bridge.load_config(path)
        self.assertEqual(info['errors'], [])
        self.assertEqual(info['values']['effort'], 'high')

    def test_load_config_rejects_invalid_effort(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'config.json'
            path.write_text(json.dumps({
                'model': 'claude-opus-5-5',
                'max_tokens': 16384,
                'effort': 'turbo',
            }), encoding='utf-8')
            info = bridge.load_config(path)
        self.assertTrue(any('effort' in error for error in info['errors']))
        self.assertNotIn('turbo', json.dumps(info['errors']))

    def test_build_payload_forwards_effort_as_output_config(self):
        cfg = {'model': 'claude-opus-5-5', 'max_tokens': 1024, 'effort': 'xhigh'}
        payload = bridge.build_payload(cfg, 'review', 'task text', None, '')
        self.assertEqual(payload['output_config'], {'effort': 'xhigh'})

    def test_build_payload_omits_output_config_without_effort(self):
        cfg = {'model': 'claude-opus-5-5', 'max_tokens': 1024}
        payload = bridge.build_payload(cfg, 'review', 'task text', None, '')
        self.assertNotIn('output_config', payload)

    def test_validate_call_config_rejects_bad_effort(self):
        cfg = {'model': 'claude-opus-5-5', 'max_tokens': 1024, 'effort': 'turbo'}
        with self.assertRaises(bridge.BridgeError) as ctx:
            bridge.validate_call_config(cfg)
        self.assertEqual(ctx.exception.category, 'config_error')

    def test_status_reports_effort(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'config.json'
            path.write_text(json.dumps({
                'base_url': 'https://api.anthropic.com',
                'model': 'claude-opus-5-5',
                'max_tokens': 16384,
                'effort': 'high',
            }), encoding='utf-8')
            with mock.patch.dict(os.environ, {}, clear=True):
                report = bridge.status_report(path)
        self.assertEqual(report['effort'], 'high')
        self.assertEqual(report['errors'], [])

    def test_run_consult_sends_output_config_and_keeps_daily_max_tokens(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'config.json'
            path.write_text(json.dumps({
                'base_url': 'https://api.anthropic.com',
                'model': 'claude-opus-5-5',
                'max_tokens': 16384,
                'thinking': {'type': 'adaptive'},
                'effort': 'high',
                'timeout_seconds': 30,
            }), encoding='utf-8')
            opener = FakeOpener(message_response('claude-opus-5-5'))
            with mock.patch.dict(os.environ, {'ANTHROPIC_API_KEY': 'sk-env-canary'}):
                result = bridge.run_consult(
                    path, 'plan the work', 'plan', None, None, None, opener=opener,
                    user_request='请让 Claude 看一下',
                )
        sent = json.loads(opener.requests[0].data.decode('utf-8'))
        self.assertEqual(sent['output_config'], {'effort': 'high'})
        self.assertEqual(sent['max_tokens'], 16384)
        self.assertEqual(sent['thinking'], {'type': 'adaptive'})
        self.assertTrue(result['ok'])
        self.assertNotIn('sk-env-canary', json.dumps(result, ensure_ascii=False))


if __name__ == '__main__':
    unittest.main()
