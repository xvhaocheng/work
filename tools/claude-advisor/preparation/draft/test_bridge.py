#!/usr/bin/env python3
'''Offline unit tests for bridge.py.

No test in this file performs network I/O: every HTTP interaction is served by an
in-process fake opener. Nothing here has been executed yet by the author of the
bridge; run it with `python3 -m unittest test_bridge`.
'''

import io
import json
import os
import sys
import tempfile
import unittest
import urllib.error
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))

import bridge  # noqa: E402


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

    def __init__(self, status=200, payload=None, raw_body=None, raise_error=None):
        self.status = status
        self.payload = payload
        self.raw_body = raw_body
        self.raise_error = raise_error
        self.requests = []

    def open(self, request, timeout=None):
        self.requests.append(request)
        if self.raise_error is not None:
            raise self.raise_error
        if self.raw_body is not None:
            body = self.raw_body
        elif self.payload is not None:
            body = json.dumps(self.payload).encode('utf-8')
        else:
            body = b''
        return FakeResponse(self.status, body)


def _message_payload(text='hello', stop_reason='end_turn', content=None):
    if content is None:
        content = [{'type': 'text', 'text': text}]
    return {
        'id': 'msg_test',
        'type': 'message',
        'role': 'assistant',
        'model': 'claude-test-model',
        'content': content,
        'stop_reason': stop_reason,
        'usage': {'input_tokens': 5, 'output_tokens': 2},
    }


class BaseCase(unittest.TestCase):
    def write_config(self, directory, **overrides):
        config = {
            'base_url': 'https://api.anthropic.com',
            'model': 'claude-test-model',
            'api_key_env': 'ANTHROPIC_API_KEY',
            'api_version': '2023-06-01',
            'max_tokens': 1024,
            'timeout_seconds': 30,
        }
        config.update(overrides)
        path = Path(directory) / 'config.json'
        path.write_text(json.dumps(config), encoding='utf-8')
        return path


class ConfigTests(BaseCase):
    def test_missing_config_is_not_an_error(self):
        with tempfile.TemporaryDirectory() as tmp:
            info = bridge.load_config(Path(tmp) / 'absent.json')
        self.assertFalse(info['present'])
        self.assertEqual(info['errors'], [])

    def test_unknown_field_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = self.write_config(tmp, surprise_field='x')
            info = bridge.load_config(path)
        self.assertTrue(any('unsupported fields' in e for e in info['errors']))

    def test_plaintext_key_field_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = self.write_config(tmp, api_key='sk-plaintext')
            info = bridge.load_config(path)
        self.assertTrue(info['errors'])
        self.assertTrue(any('api_key' in e for e in info['errors']))

    def test_non_official_base_url_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = self.write_config(tmp, base_url='https://proxy.example.com')
            info = bridge.load_config(path)
        self.assertTrue(any('base_url' in e for e in info['errors']))

    def test_thinking_budget_must_be_below_max_tokens(self):
        errors = bridge.validate_thinking({'type': 'enabled', 'budget_tokens': 4096}, 2048)
        self.assertTrue(errors)

    def test_thinking_adaptive_is_accepted(self):
        self.assertEqual(bridge.validate_thinking({'type': 'adaptive'}, 2048), [])

    def test_thinking_unknown_type_is_rejected(self):
        self.assertTrue(bridge.validate_thinking({'type': 'always'}, 2048))


class StatusTests(BaseCase):
    def test_status_without_config_is_false_and_does_not_raise(self):
        with tempfile.TemporaryDirectory() as tmp:
            report = bridge.status_report(Path(tmp) / 'absent.json')
        self.assertTrue(report['ok'])
        self.assertFalse(report['configured'])
        self.assertFalse(report['call_ready'])

    def test_status_without_key_is_not_configured(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = self.write_config(tmp, model=None, max_tokens=None)
            env = {k: v for k, v in os.environ.items() if k != 'ANTHROPIC_API_KEY'}
            with mock.patch.dict(os.environ, env, clear=True):
                report = bridge.status_report(path)
        self.assertFalse(report['api_key_env_present'])
        self.assertFalse(report['configured'])

    def test_status_never_reads_key_file_content(self):
        with tempfile.TemporaryDirectory() as tmp:
            key_file = Path(tmp) / 'key.txt'
            key_file.write_text('sk-status-canary-value', encoding='utf-8')
            config_path = self.write_config(tmp, api_key_file=str(key_file))
            report = bridge.status_report(config_path)
        serialized = json.dumps(report, ensure_ascii=False)
        self.assertNotIn('sk-status-canary-value', serialized)
        self.assertTrue(report['api_key_file_configured'])

    def test_status_with_env_key_reports_presence_only(self):
        with tempfile.TemporaryDirectory() as tmp:
            config_path = self.write_config(tmp)
            with mock.patch.dict(os.environ, {'ANTHROPIC_API_KEY': 'sk-presence-canary'}):
                report = bridge.status_report(config_path)
        self.assertTrue(report['api_key_env_present'])
        self.assertTrue(report['configured'])
        self.assertNotIn('sk-presence-canary', json.dumps(report, ensure_ascii=False))


class FileAccessTests(BaseCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.root = Path(self._tmp.name) / 'project'
        self.root.mkdir()

    def tearDown(self):
        self._tmp.cleanup()

    def test_plain_relative_file_is_accepted(self):
        target = self.root / 'a.py'
        target.write_text('print(1)\n', encoding='utf-8')
        entries = bridge.collect_files(str(self.root), ['a.py'])
        self.assertEqual(entries[0]['path'], 'a.py')
        rendered = bridge.render_files(entries)
        self.assertIn('a.py:1:', rendered)

    def test_absolute_path_is_rejected(self):
        with self.assertRaises(bridge.BridgeError):
            bridge.collect_files(str(self.root), ['/etc/passwd'])

    def test_parent_traversal_is_rejected(self):
        with self.assertRaises(bridge.BridgeError):
            bridge.collect_files(str(self.root), ['../outside.txt'])

    def test_hidden_and_git_paths_are_rejected(self):
        for rel in ['.env', '.git/config', 'sub/.hidden']:
            with self.assertRaises(bridge.BridgeError):
                bridge.collect_files(str(self.root), [rel])

    def test_credential_like_name_is_rejected(self):
        with self.assertRaises(bridge.BridgeError):
            bridge.collect_files(str(self.root), ['notes/credentials.json'])

    def test_symlink_escape_is_rejected(self):
        outside = Path(self._tmp.name) / 'outside.txt'
        outside.write_text('secret-ish\n', encoding='utf-8')
        link = self.root / 'link.txt'
        link.symlink_to(outside)
        with self.assertRaises(bridge.BridgeError):
            bridge.collect_files(str(self.root), ['link.txt'])

    def test_root_must_not_be_filesystem_root(self):
        with self.assertRaises(bridge.BridgeError):
            bridge.validate_root('/')


class CallTests(BaseCase):
    def test_payload_shape_and_key_absent_from_result(self):
        with tempfile.TemporaryDirectory() as tmp:
            config_path = self.write_config(tmp)
            opener = FakeOpener(payload=_message_payload())
            with mock.patch.dict(os.environ, {'ANTHROPIC_API_KEY': 'sk-canary-value'}):
                result = bridge.run_consult(
                    config_path,
                    'Review the proposed plan.',
                    'review',
                    'caller supplied context',
                    None,
                    None,
                    opener=opener,
                )
        self.assertNotIn('sk-canary-value', json.dumps(result, ensure_ascii=False))
        self.assertEqual(result['answer'], 'hello')
        self.assertFalse(result['truncated'])
        self.assertEqual(result['stop_reason'], 'end_turn')
        self.assertIn('usage', result)
        self.assertEqual(result['key_source'], 'env')

        request = opener.requests[0]
        self.assertEqual(request.full_url, 'https://api.anthropic.com/v1/messages')
        self.assertEqual(request.get_header('X-api-key'), 'sk-canary-value')
        self.assertEqual(request.get_header('Anthropic-version'), '2023-06-01')
        sent = json.loads(request.data.decode('utf-8'))
        self.assertEqual(sent['model'], 'claude-test-model')
        self.assertEqual(sent['max_tokens'], 1024)
        self.assertIn('system', sent)
        self.assertIn('EVIDENCE', sent['system'])
        self.assertEqual(sent['messages'][0]['role'], 'user')
        self.assertIn('Review the proposed plan.', sent['messages'][0]['content'])
        self.assertIn('caller supplied context', sent['messages'][0]['content'])
        self.assertNotIn('thinking', sent)

    def test_thinking_is_forwarded_when_configured(self):
        with tempfile.TemporaryDirectory() as tmp:
            config_path = self.write_config(
                tmp, max_tokens=8192, thinking={'type': 'enabled', 'budget_tokens': 2048}
            )
            opener = FakeOpener(payload=_message_payload())
            with mock.patch.dict(os.environ, {'ANTHROPIC_API_KEY': 'sk-canary-value'}):
                bridge.run_consult(config_path, 'x', 'plan', None, None, None, opener=opener)
        sent = json.loads(opener.requests[0].data.decode('utf-8'))
        self.assertEqual(sent['thinking'], {'type': 'enabled', 'budget_tokens': 2048})

    def test_missing_model_fails_before_any_request(self):
        with tempfile.TemporaryDirectory() as tmp:
            config_path = self.write_config(tmp, model=None)
            opener = FakeOpener(payload=_message_payload())
            with mock.patch.dict(os.environ, {'ANTHROPIC_API_KEY': 'sk-canary-value'}):
                with self.assertRaises(bridge.BridgeError) as ctx:
                    bridge.run_consult(config_path, 'x', 'plan', None, None, None, opener=opener)
        self.assertEqual(ctx.exception.category, 'config_error')
        self.assertEqual(opener.requests, [])

    def test_truncated_on_max_tokens_stop_reason(self):
        with tempfile.TemporaryDirectory() as tmp:
            config_path = self.write_config(tmp)
            opener = FakeOpener(payload=_message_payload('partial', stop_reason='max_tokens'))
            with mock.patch.dict(os.environ, {'ANTHROPIC_API_KEY': 'sk-canary-value'}):
                result = bridge.run_consult(
                    config_path, 'x', 'review', None, None, None, opener=opener
                )
        self.assertTrue(result['truncated'])

    def test_truncated_when_no_text_blocks(self):
        with tempfile.TemporaryDirectory() as tmp:
            config_path = self.write_config(tmp)
            opener = FakeOpener(payload=_message_payload(content=[{'type': 'thinking'}]))
            with mock.patch.dict(os.environ, {'ANTHROPIC_API_KEY': 'sk-canary-value'}):
                result = bridge.run_consult(
                    config_path, 'x', 'review', None, None, None, opener=opener
                )
        self.assertEqual(result['answer'], '')
        self.assertTrue(result['truncated'])

    def test_http_error_does_not_leak_key(self):
        error = urllib.error.HTTPError(
            'https://api.anthropic.com/v1/messages',
            401,
            'Unauthorized',
            {},
            io.BytesIO(b'{"error":"echoed server text"}'),
        )
        with tempfile.TemporaryDirectory() as tmp:
            config_path = self.write_config(tmp)
            opener = FakeOpener(raise_error=error)
            with mock.patch.dict(os.environ, {'ANTHROPIC_API_KEY': 'sk-canary-value'}):
                with self.assertRaises(bridge.BridgeError) as ctx:
                    bridge.run_consult(
                        config_path, 'x', 'review', None, None, None, opener=opener
                    )
        payload = ctx.exception.to_json()
        self.assertEqual(payload['error']['category'], 'http_error')
        self.assertEqual(payload['error']['status'], 401)
        serialized = json.dumps(payload, ensure_ascii=False)
        self.assertNotIn('sk-canary-value', serialized)
        self.assertNotIn('echoed server text', serialized)

    def test_network_error_is_reported_safely(self):
        error = urllib.error.URLError('name resolution failed')
        with tempfile.TemporaryDirectory() as tmp:
            config_path = self.write_config(tmp)
            opener = FakeOpener(raise_error=error)
            with mock.patch.dict(os.environ, {'ANTHROPIC_API_KEY': 'sk-canary-value'}):
                with self.assertRaises(bridge.BridgeError) as ctx:
                    bridge.run_consult(
                        config_path, 'x', 'review', None, None, None, opener=opener
                    )
        self.assertEqual(ctx.exception.category, 'network_error')

    def test_bad_role_is_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            config_path = self.write_config(tmp)
            with self.assertRaises(bridge.BridgeError):
                bridge.run_consult(config_path, 'x', 'admin', None, None, None)


class McpTests(BaseCase):
    def test_initialize_echoes_known_version(self):
        response = bridge.handle_rpc(
            {'jsonrpc': '2.0', 'id': 1, 'method': 'initialize',
             'params': {'protocolVersion': '2025-06-18'}},
            Path('absent-config.json'),
        )
        self.assertEqual(response['result']['protocolVersion'], '2025-06-18')

    def test_initialize_falls_back_for_unknown_version(self):
        response = bridge.handle_rpc(
            {'jsonrpc': '2.0', 'id': 1, 'method': 'initialize',
             'params': {'protocolVersion': '1999-01-01'}},
            Path('absent-config.json'),
        )
        self.assertEqual(response['result']['protocolVersion'], bridge.FALLBACK_PROTOCOL_VERSION)

    def test_notifications_are_ignored(self):
        result = bridge.handle_rpc(
            {'jsonrpc': '2.0', 'method': 'notifications/initialized'},
            Path('absent-config.json'),
        )
        self.assertIsNone(result)

    def test_ping(self):
        response = bridge.handle_rpc(
            {'jsonrpc': '2.0', 'id': 7, 'method': 'ping'}, Path('absent-config.json')
        )
        self.assertEqual(response['result'], {})

    def test_tools_list_exposes_both_tools(self):
        response = bridge.handle_rpc(
            {'jsonrpc': '2.0', 'id': 2, 'method': 'tools/list'}, Path('absent-config.json')
        )
        names = sorted(tool['name'] for tool in response['result']['tools'])
        self.assertEqual(names, ['claude_consult', 'claude_status'])

    def test_unknown_method_is_reported(self):
        response = bridge.handle_rpc(
            {'jsonrpc': '2.0', 'id': 3, 'method': 'tools/nope'}, Path('absent-config.json')
        )
        self.assertEqual(response['error']['code'], -32601)

    def test_status_tool_works_without_config(self):
        with tempfile.TemporaryDirectory() as tmp:
            response = bridge.handle_rpc(
                {'jsonrpc': '2.0', 'id': 4, 'method': 'tools/call',
                 'params': {'name': 'claude_status', 'arguments': {}}},
                Path(tmp) / 'absent.json',
            )
        self.assertFalse(response['result']['isError'])
        payload = json.loads(response['result']['content'][0]['text'])
        self.assertFalse(payload['configured'])

    def test_status_tool_rejects_unknown_arguments(self):
        response = bridge.handle_rpc(
            {'jsonrpc': '2.0', 'id': 5, 'method': 'tools/call',
             'params': {'name': 'claude_status', 'arguments': {'bogus': 1}}},
            Path('absent-config.json'),
        )
        self.assertTrue(response['result']['isError'])
        self.assertIn('unknown arguments', response['result']['content'][0]['text'])

    def test_consult_tool_fails_safely_without_config(self):
        with tempfile.TemporaryDirectory() as tmp:
            response = bridge.handle_rpc(
                {'jsonrpc': '2.0', 'id': 6, 'method': 'tools/call',
                 'params': {'name': 'claude_consult',
                            'arguments': {'task': 'x', 'role': 'review'}}},
                Path(tmp) / 'absent.json',
            )
        self.assertTrue(response['result']['isError'])
        payload = json.loads(response['result']['content'][0]['text'])
        self.assertEqual(payload['error']['category'], 'config_error')
        self.assertIn('model', payload['error']['message'])

    def test_consult_tool_rejects_unknown_arguments(self):
        response = bridge.handle_rpc(
            {'jsonrpc': '2.0', 'id': 8, 'method': 'tools/call',
             'params': {'name': 'claude_consult',
                        'arguments': {'task': 'x', 'role': 'review', 'temperature': 0.5}}},
            Path('absent-config.json'),
        )
        self.assertTrue(response['result']['isError'])


if __name__ == '__main__':
    unittest.main()
