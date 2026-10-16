"""One scoped advisor request with categorical, non-secret HTTP diagnostics."""
import importlib.util
import json
from pathlib import Path
import sys
import urllib.error

BASE = Path(__file__).resolve().parent
ROOT = Path('/home/xvhaocheng/work')
spec = importlib.util.spec_from_file_location('advisor_bridge', ROOT / 'tools/claude-advisor/bridge.py')
bridge = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = bridge
spec.loader.exec_module(bridge)
original = bridge.get_opener

class DiagnosticOpener:
    def __init__(self):
        self.delegate = original()

    def open(self, request, timeout=None):
        try:
            return self.delegate.open(request, timeout=timeout)
        except urllib.error.HTTPError as exc:
            # Never save provider body, headers, request, key, or arbitrary message.
            try:
                body = json.loads(exc.read(65536))
                error = body.get('error', {})
                message = str(error.get('message', '')).lower()
                kind = str(error.get('type', ''))
            except Exception:
                message, kind = '', ''
            allowed = {'invalid_request_error', 'authentication_error', 'permission_error',
                       'not_found_error', 'request_too_large', 'rate_limit_error',
                       'api_error', 'overloaded_error'}
            categories = {term: term in message for term in ['credit', 'balance', 'billing',
                          'quota', 'rate limit', 'max_tokens', 'maximum', 'context',
                          'too long', 'token', 'model', 'thinking', 'effort', 'stream',
                          'temperature', 'invalid', 'budget', '10 minutes']}
            result = {'http_status': exc.code, 'error_type': kind if kind in allowed else 'other',
                      'message_categories': categories, 'message_present': bool(message)}
            (BASE / 'claude-http-diagnosis.json').write_text(json.dumps(result, indent=2))
            raise

bridge.get_opener = DiagnosticOpener
record = json.loads((BASE / 'round1-recovery/A4-input.json').read_text())
args = ['--task', record['task'], '--role', 'ideas', '--user-request', record['user_request'],
        '--root', record['root'], '--files', *record['files']]
raise SystemExit(bridge.main(args))
