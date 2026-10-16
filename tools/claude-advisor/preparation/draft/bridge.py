#!/usr/bin/env python3
'''claude-advisor bridge.

A small, auditable bridge from a supervising agent to the official Anthropic
Messages API. It is NOT an autonomous agent and it does NOT:

  * call tools, run shell commands, write files, or browse the web
  * retry requests or follow HTTP redirects
  * print, log, or persist the API key

Two entry points share the same implementation:

  CLI : bridge.py --status
        bridge.py --task TEXT --role plan|discuss|review|ideas|paper|research
  MCP : bridge.py --serve      (stdio JSON-RPC: claude_status, claude_consult)
'''

from __future__ import annotations

import argparse
import json
import os
import stat
import sys
import urllib.error
import urllib.request
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

OFFICIAL_BASE_URL = 'https://api.anthropic.com'
MESSAGES_PATH = '/v1/messages'
DEFAULT_API_VERSION = '2023-06-01'
DEFAULT_TIMEOUT_SECONDS = 600
DEFAULT_API_KEY_ENV = 'ANTHROPIC_API_KEY'

MAX_TASK_CHARS = 200000
MAX_CONTEXT_CHARS = 400000
MAX_CONTEXT_FILES = 128
MAX_CONTEXT_BYTES = 8 * 1024 * 1024

PROTOCOL_VERSIONS = (
    '2024-11-05',
    '2025-03-26',
    '2025-06-18',
    '2025-11-25',
    '2026-07-28',
)
FALLBACK_PROTOCOL_VERSION = '2025-11-25'

BRIDGE_DIR = Path(__file__).resolve().parent
DEFAULT_CONFIG_PATH = BRIDGE_DIR / 'config.json'

CONFIG_ALLOWED_KEYS = frozenset({
    'base_url',
    'model',
    'api_key_env',
    'api_key_file',
    'api_version',
    'workspace_id',
    'max_tokens',
    'thinking',
    'timeout_seconds',
})

SENSITIVE_RESPONSE_KEYS = frozenset({
    'headers',
    '_headers',
    'request_headers',
    'response_headers',
    'api_key',
    'authorization',
    'x-api-key',
})

BLOCKED_NAME_FRAGMENTS = (
    'secret',
    'credential',
    'apikey',
    'api_key',
    'id_rsa',
    'id_ed25519',
    'id_ecdsa',
    'password',
    'passwd',
    'token',
)

SYSTEM_PREFIX = (
    'You are a consulting reviewer for a supervising coding agent. '
    'You have no tools, no shell, no file access, and no network access. '
    'You cannot run commands, read files, browse, or execute code. '
    'Never claim to have inspected files, run commands or tests, or browsed sources. '
    'Label substantive statements as EVIDENCE (directly supported by material quoted '
    'from the supplied context), INFERENCE, ASSUMPTION, or RECOMMENDATION. '
    'Say plainly when information is missing instead of filling the gap. '
    'Cite supplied code as path:line. '
    'Cite literature only when you are confident the work exists; otherwise write: '
    'citation unavailable from the provided context. '
    'Do not fabricate browsing, commands, test results, or citations.'
)

ROLE_PROMPTS: Dict[str, str] = {
    'plan': (
        'Deliver an ordered plan with explicit dependencies, risks, and one concrete '
        'acceptance check per step. Do not write implementation code unless the task '
        'explicitly asks for it.'
    ),
    'discuss': (
        'Weigh alternatives against each other, name the trade-offs, and surface the '
        'assumptions each option depends on. Do not simply agree with the caller.'
    ),
    'review': (
        'Report in this order: (1) fatal problems, (2) worthwhile improvements, '
        '(3) the smallest verification that would confirm or refute each issue, '
        '(4) the condition under which the review should stop. Do not restate what '
        'already works.'
    ),
    'ideas': (
        'Produce a small number of distinctly different ideas. Avoid restating the '
        'obvious and avoid padding the list to look thorough.'
    ),
    'paper': (
        'Structure the main line as problem -> mechanism -> gap -> method -> baselines '
        '-> evidence -> limitations. Do not invent related work that the supplied '
        'material does not support.'
    ),
    'research': (
        'Give at most three research directions. For each, state the judging criterion, '
        'the first concrete step, and the condition under which it should be abandoned.'
    ),
}

ALLOWED_CONSULT_ARGS = frozenset({'task', 'role', 'context', 'root', 'files'})

STATUS_NOTES = (
    'status is local only: no network, no API call, no key content read',
    'configured=true only means a key source is declared; it does not verify the key',
    'call_ready additionally requires model and max_tokens to be set',
)


class BridgeError(Exception):
    '''A failure that is safe to serialize back to the caller.'''

    def __init__(self, category: str, message: str, status: Optional[int] = None) -> None:
        super().__init__(message)
        self.category = category
        self.status = status

    def to_json(self) -> Dict[str, Any]:
        error: Dict[str, Any] = {'category': self.category, 'message': str(self)}
        if self.status is not None:
            error['status'] = self.status
        return {'ok': False, 'kind': 'claude_error', 'error': error}


# --------------------------------------------------------------------------- #
# Path helpers
# --------------------------------------------------------------------------- #

def is_within(path: Path, root: Path) -> bool:
    try:
        path.relative_to(root)
    except ValueError:
        return False
    return True


def default_workspace_roots() -> List[Path]:
    '''Directories where the API key file must never live.

    Derived from the bridge location instead of a hard-coded user name: the
    bridge directory and its ancestors, stopping before shared system
    directories and the current user home directory.
    '''
    skip = {
        Path('/'), Path('/home'), Path('/tmp'), Path('/var'), Path('/usr'),
        Path('/etc'), Path('/opt'), Path('/srv'), Path('/root'),
    }
    try:
        skip.add(Path.home().resolve())
    except OSError:
        pass
    roots: List[Path] = []
    current = BRIDGE_DIR
    while current not in skip:
        roots.append(current)
        if current.parent == current or current.parent in skip:
            break
        current = current.parent
    return roots


# --------------------------------------------------------------------------- #
# Configuration
# --------------------------------------------------------------------------- #

def validate_thinking(value: Any, max_tokens: Any) -> List[str]:
    if not isinstance(value, dict):
        return ['thinking must be null or a JSON object']
    errors: List[str] = []
    thinking_type = value.get('type')
    if thinking_type == 'adaptive':
        extra = set(value) - {'type'}
        if extra:
            errors.append('thinking with type adaptive must contain only the type field')
    elif thinking_type == 'enabled':
        extra = set(value) - {'type', 'budget_tokens'}
        if extra:
            errors.append('thinking has unsupported fields: ' + ', '.join(sorted(extra)))
        budget = value.get('budget_tokens')
        if isinstance(budget, bool) or not isinstance(budget, int):
            errors.append('thinking.budget_tokens must be an integer')
        elif budget < 1024:
            errors.append('thinking.budget_tokens must be >= 1024')
        elif isinstance(max_tokens, int) and not isinstance(max_tokens, bool):
            if budget >= max_tokens:
                errors.append('thinking.budget_tokens must be smaller than max_tokens')
    else:
        errors.append("thinking.type must be 'adaptive' or 'enabled'")
    return errors


def validate_config_values(values: Dict[str, Any]) -> List[str]:
    errors: List[str] = []

    base_url = values.get('base_url')
    if base_url is not None and base_url != OFFICIAL_BASE_URL:
        errors.append('base_url must be exactly ' + OFFICIAL_BASE_URL)

    for field in ('model', 'api_key_env', 'api_key_file', 'api_version', 'workspace_id'):
        raw = values.get(field)
        if raw is not None and (not isinstance(raw, str) or not raw.strip()):
            errors.append(field + ' must be null or a non-empty string')

    for field in ('max_tokens', 'timeout_seconds'):
        raw = values.get(field)
        if raw is not None and (isinstance(raw, bool) or not isinstance(raw, int) or raw <= 0):
            errors.append(field + ' must be null or a positive integer')

    thinking = values.get('thinking')
    if thinking is not None:
        errors.extend(validate_thinking(thinking, values.get('max_tokens')))

    return errors


def load_config(config_path: Path) -> Dict[str, Any]:
    '''Read the config file. Missing file is not an error.'''
    result: Dict[str, Any] = {'present': False, 'errors': [], 'values': {}}
    path = Path(config_path)
    if not path.exists():
        return result
    result['present'] = True
    if not path.is_file():
        result['errors'].append('config path is not a regular file')
        return result
    try:
        text = path.read_text(encoding='utf-8')
    except (OSError, UnicodeDecodeError):
        result['errors'].append('config file is unreadable')
        return result
    try:
        data = json.loads(text)
    except json.JSONDecodeError:
        result['errors'].append('config file is not valid JSON')
        return result
    if not isinstance(data, dict):
        result['errors'].append('config must be a JSON object')
        return result
    unknown = sorted(set(data) - CONFIG_ALLOWED_KEYS)
    if unknown:
        result['errors'].append(
            'config has unsupported fields (plaintext key fields are rejected): '
            + ', '.join(unknown)
        )
    values = {key: data[key] for key in CONFIG_ALLOWED_KEYS if key in data}
    result['values'] = values
    result['errors'].extend(validate_config_values(values))
    return result


def validate_call_config(cfg: Dict[str, Any]) -> None:
    '''Raise unless the config is complete enough for a real API call.'''
    base_url = cfg.get('base_url') or OFFICIAL_BASE_URL
    if base_url != OFFICIAL_BASE_URL:
        raise BridgeError('config_error', 'base_url must be exactly ' + OFFICIAL_BASE_URL)

    model = cfg.get('model')
    if not isinstance(model, str) or not model.strip():
        raise BridgeError('config_error', 'model must be set to a non-empty string before calling')

    max_tokens = cfg.get('max_tokens')
    if isinstance(max_tokens, bool) or not isinstance(max_tokens, int) or max_tokens <= 0:
        raise BridgeError('config_error', 'max_tokens must be a positive integer before calling')

    thinking = cfg.get('thinking')
    if thinking is not None:
        errors = validate_thinking(thinking, max_tokens)
        if errors:
            raise BridgeError('config_error', '; '.join(errors))


# --------------------------------------------------------------------------- #
# API key loading (never echoed, never logged)
# --------------------------------------------------------------------------- #

def read_api_key_file(raw_path: str, forbidden_roots: List[Path]) -> str:
    path = Path(raw_path)
    if not path.is_absolute():
        raise BridgeError('bad_key_file', 'api_key_file must be an absolute path')
    try:
        info = path.lstat()
    except OSError:
        raise BridgeError('bad_key_file', 'api_key_file is not accessible')
    if stat.S_ISLNK(info.st_mode):
        raise BridgeError('bad_key_file', 'api_key_file must not be a symbolic link')
    if not stat.S_ISREG(info.st_mode):
        raise BridgeError('bad_key_file', 'api_key_file must be a regular file')
    if info.st_mode & 0o077:
        raise BridgeError('bad_key_file', 'api_key_file must be private (mode 0600)')
    try:
        resolved = path.resolve(strict=True)
    except OSError:
        raise BridgeError('bad_key_file', 'api_key_file is not resolvable')
    for root in forbidden_roots:
        if is_within(resolved, root):
            raise BridgeError('bad_key_file', 'api_key_file must live outside the workspace')
    try:
        text = resolved.read_text(encoding='utf-8')
    except (OSError, UnicodeDecodeError):
        raise BridgeError('bad_key_file', 'api_key_file is unreadable')
    key = text.strip()
    if not key:
        raise BridgeError('bad_key_file', 'api_key_file is empty')
    return key


def load_api_key(cfg: Dict[str, Any], forbidden_roots: List[Path]) -> Tuple[str, str]:
    env_name = cfg.get('api_key_env') or DEFAULT_API_KEY_ENV
    env_value = os.environ.get(env_name)
    if env_value and env_value.strip():
        return env_value.strip(), 'env'
    raw_file = cfg.get('api_key_file')
    if raw_file:
        return read_api_key_file(raw_file, forbidden_roots), 'file'
    raise BridgeError(
        'missing_api_key',
        'no API key available: set the configured environment variable or api_key_file',
    )


# --------------------------------------------------------------------------- #
# Caller-supplied file context
# --------------------------------------------------------------------------- #

def validate_root(root_value: Any) -> Path:
    if not isinstance(root_value, str) or not root_value.strip():
        raise BridgeError('bad_root', '--root is required and must be an absolute path')
    root = Path(root_value)
    if not root.is_absolute():
        raise BridgeError('bad_root', '--root must be an absolute path')
    try:
        resolved = root.resolve(strict=True)
    except OSError:
        raise BridgeError('bad_root', '--root is not resolvable')
    if resolved == Path('/'):
        raise BridgeError('bad_root', '--root must not be the filesystem root')
    try:
        home = Path.home().resolve()
    except OSError:
        home = None
    if home is not None and resolved == home:
        raise BridgeError('bad_root', '--root must not be the home directory')
    if not resolved.is_dir():
        raise BridgeError('bad_root', '--root must be an existing directory')
    return resolved


def validate_relative_file(rel: Any, root: Path) -> Path:
    if not isinstance(rel, str) or not rel.strip():
        raise BridgeError('bad_file', 'file path must be a non-empty string')
    if rel.startswith('/') or rel.startswith('\\'):
        raise BridgeError('bad_file', 'file path must be relative: ' + rel)
    parts = Path(rel).parts
    if not parts:
        raise BridgeError('bad_file', 'file path must be relative: ' + rel)
    for part in parts:
        if part in ('', '.'):
            continue
        if part == '..':
            raise BridgeError('bad_file', "file path must not contain '..': " + rel)
        if part.startswith('.'):
            raise BridgeError('bad_file', 'hidden path segments are not allowed: ' + rel)
        lowered = part.lower()
        if any(fragment in lowered for fragment in BLOCKED_NAME_FRAGMENTS):
            raise BridgeError('bad_file', 'credential-like file names are not allowed: ' + rel)
    candidate = root / rel
    if candidate.is_symlink():
        raise BridgeError('bad_file', 'symlinked files are not allowed: ' + rel)
    try:
        resolved = candidate.resolve(strict=True)
    except OSError:
        raise BridgeError('bad_file', 'file does not exist or is not readable: ' + rel)
    if not is_within(resolved, root):
        raise BridgeError('bad_file', 'path escapes the authorized root: ' + rel)
    if not resolved.is_file():
        raise BridgeError('bad_file', 'not a regular file: ' + rel)
    return resolved


def collect_files(root_value: Any, rel_paths: List[str]) -> List[Dict[str, str]]:
    root = validate_root(root_value)
    if len(rel_paths) > MAX_CONTEXT_FILES:
        raise BridgeError('bad_file', 'at most ' + str(MAX_CONTEXT_FILES) + ' files are allowed')
    entries: List[Dict[str, str]] = []
    total = 0
    for rel in rel_paths:
        resolved = validate_relative_file(rel, root)
        try:
            size = resolved.stat().st_size
        except OSError:
            raise BridgeError('bad_file', 'unreadable file: ' + rel)
        total += size
        if total > MAX_CONTEXT_BYTES:
            raise BridgeError('context_too_large', 'file context exceeds the byte budget')
        try:
            data = resolved.read_bytes()
        except OSError:
            raise BridgeError('bad_file', 'unreadable file: ' + rel)
        try:
            text = data.decode('utf-8')
        except UnicodeDecodeError:
            raise BridgeError('bad_file', 'not valid UTF-8: ' + rel)
        entries.append({'path': rel, 'text': text})
    return entries


def render_files(entries: List[Dict[str, str]]) -> str:
    chunks: List[str] = []
    for entry in entries:
        rel = entry['path']
        chunks.append('### FILE: ' + rel)
        for number, line in enumerate(entry['text'].split('\n'), start=1):
            chunks.append(rel + ':' + str(number) + ': ' + line.rstrip('\r'))
    return '\n'.join(chunks)


# --------------------------------------------------------------------------- #
# Prompt and payload construction
# --------------------------------------------------------------------------- #

def validate_text(name: str, value: Any, max_chars: int, required: bool) -> Optional[str]:
    if value is None:
        if required:
            raise BridgeError('invalid_arguments', name + ' is required')
        return None
    if not isinstance(value, str):
        raise BridgeError('invalid_arguments', name + ' must be a string')
    if required and not value.strip():
        raise BridgeError('invalid_arguments', name + ' must not be empty')
    if len(value) > max_chars:
        raise BridgeError('invalid_arguments', name + ' exceeds ' + str(max_chars) + ' characters')
    return value


def build_system_prompt(role: str) -> str:
    return SYSTEM_PREFIX + '\n\nRole-specific instructions:\n' + ROLE_PROMPTS[role]


def build_user_content(role: str, task: str, context: Optional[str], files_block: str) -> str:
    parts: List[str] = ['ROLE: ' + role, '', 'TASK:', task.strip()]
    if context:
        parts.extend(['', 'ADDITIONAL CONTEXT (provided by caller):', context])
    if files_block:
        parts.extend(['', 'FILES (exact content, cited as path:line):', files_block])
    else:
        parts.extend(['', 'FILES: none supplied for this request.'])
    return '\n'.join(parts)


def build_payload(
    cfg: Dict[str, Any],
    role: str,
    task: str,
    context: Optional[str],
    files_block: str,
) -> Dict[str, Any]:
    payload: Dict[str, Any] = {
        'model': cfg['model'],
        'max_tokens': cfg['max_tokens'],
        'system': build_system_prompt(role),
        'messages': [
            {'role': 'user', 'content': build_user_content(role, task, context, files_block)}
        ],
    }
    thinking = cfg.get('thinking')
    if thinking is not None:
        payload['thinking'] = thinking
    return payload


def build_headers(cfg: Dict[str, Any], api_key: str) -> Dict[str, str]:
    headers = {
        'content-type': 'application/json',
        'x-api-key': api_key,
        'anthropic-version': cfg.get('api_version') or DEFAULT_API_VERSION,
    }
    workspace_id = cfg.get('workspace_id')
    if workspace_id:
        headers['anthropic-workspace-id'] = workspace_id
    return headers


# --------------------------------------------------------------------------- #
# HTTP
# --------------------------------------------------------------------------- #

class _NoRedirectHandler(urllib.request.HTTPRedirectHandler):
    '''Refuse redirects instead of following them.'''

    def redirect_request(self, req, fp, code, msg, headers, newurl):
        return None


def get_opener() -> urllib.request.OpenerDirector:
    return urllib.request.build_opener(_NoRedirectHandler)


def sanitize_provider_response(raw: Any) -> Optional[Dict[str, Any]]:
    if not isinstance(raw, dict):
        return None
    cleaned: Dict[str, Any] = {}
    for key, value in raw.items():
        if str(key).lower() in SENSITIVE_RESPONSE_KEYS:
            continue
        cleaned[key] = value
    return cleaned


def call_anthropic(
    cfg: Dict[str, Any],
    payload: Dict[str, Any],
    api_key: str,
    opener: Any = None,
) -> Dict[str, Any]:
    body = json.dumps(payload, ensure_ascii=False).encode('utf-8')
    request = urllib.request.Request(
        OFFICIAL_BASE_URL + MESSAGES_PATH,
        data=body,
        headers=build_headers(cfg, api_key),
        method='POST',
    )
    if opener is None:
        opener = get_opener()
    timeout = cfg.get('timeout_seconds') or DEFAULT_TIMEOUT_SECONDS
    try:
        response = opener.open(request, timeout=timeout)
    except urllib.error.HTTPError as exc:
        raise BridgeError(
            'http_error',
            'provider returned HTTP status ' + str(exc.code),
            status=exc.code,
        )
    except urllib.error.URLError as exc:
        if isinstance(exc.reason, TimeoutError):
            raise BridgeError('timeout', 'request timed out before the provider answered')
        raise BridgeError('network_error', 'network failure while contacting the provider')
    except TimeoutError:
        raise BridgeError('timeout', 'request timed out before the provider answered')
    except OSError:
        raise BridgeError('network_error', 'network failure while contacting the provider')

    status = getattr(response, 'status', None)
    if status is None:
        status = getattr(response, 'code', None)
    try:
        raw_bytes = response.read()
    except OSError:
        raise BridgeError('network_error', 'failed while reading the provider response')
    finally:
        close = getattr(response, 'close', None)
        if callable(close):
            try:
                close()
            except OSError:
                pass

    if isinstance(status, int) and 300 <= status < 400:
        raise BridgeError(
            'redirect_refused',
            'provider attempted a redirect (status ' + str(status) + ')',
            status=status,
        )

    try:
        raw = json.loads(raw_bytes.decode('utf-8'))
    except (ValueError, UnicodeDecodeError):
        raise BridgeError('invalid_response', 'provider returned a body that is not JSON')
    if not isinstance(raw, dict):
        raise BridgeError('invalid_response', 'provider returned an unexpected JSON shape')
    return raw


def build_result(cfg: Dict[str, Any], role: str, raw: Dict[str, Any]) -> Dict[str, Any]:
    content = raw.get('content')
    if not isinstance(content, list):
        content = []
    answer_parts: List[str] = []
    for block in content:
        if isinstance(block, dict) and block.get('type') == 'text':
            text = block.get('text')
            if isinstance(text, str):
                answer_parts.append(text)
    answer = '\n'.join(answer_parts)
    stop_reason = raw.get('stop_reason')
    truncated = (stop_reason == 'max_tokens') or (not answer)
    return {
        'ok': True,
        'kind': 'claude_consult',
        'model': raw.get('model') or cfg.get('model'),
        'role': role,
        'answer': answer,
        'content': content,
        'usage': raw.get('usage'),
        'stop_reason': stop_reason,
        'truncated': truncated,
        'provider_response': sanitize_provider_response(raw),
    }


# --------------------------------------------------------------------------- #
# Shared consult path (CLI and MCP)
# --------------------------------------------------------------------------- #

def run_consult(
    config_path: Path,
    task: Any,
    role: Any,
    context: Any = None,
    root: Any = None,
    files: Any = None,
    opener: Any = None,
) -> Dict[str, Any]:
    if not isinstance(role, str) or role not in ROLE_PROMPTS:
        raise BridgeError(
            'invalid_arguments',
            'role must be one of: ' + ', '.join(ROLE_PROMPTS),
        )
    task_text = validate_text('task', task, MAX_TASK_CHARS, required=True)
    context_text = validate_text('context', context, MAX_CONTEXT_CHARS, required=False)

    info = load_config(config_path)
    if info['errors']:
        raise BridgeError('config_error', '; '.join(info['errors']))
    cfg = info['values']
    validate_call_config(cfg)

    rel_files = list(files) if isinstance(files, (list, tuple)) else []
    if rel_files and not root:
        raise BridgeError('bad_root', '--root is required when --files is used')

    forbidden_roots = default_workspace_roots()
    if root:
        try:
            forbidden_roots.append(Path(root).resolve())
        except OSError:
            pass

    api_key, key_source = load_api_key(cfg, forbidden_roots)
    entries = collect_files(root, rel_files) if rel_files else []
    files_block = render_files(entries) if entries else ''
    payload = build_payload(cfg, role, task_text, context_text, files_block)
    raw = call_anthropic(cfg, payload, api_key, opener=opener)

    result = build_result(cfg, role, raw)
    result['key_source'] = key_source
    result['files_used'] = [entry['path'] for entry in entries]
    return result


# --------------------------------------------------------------------------- #
# Status (local only)
# --------------------------------------------------------------------------- #

def status_report(config_path: Path) -> Dict[str, Any]:
    info = load_config(config_path)
    errors = list(info['errors'])
    cfg = info['values']

    env_name = cfg.get('api_key_env') or DEFAULT_API_KEY_ENV
    env_value = os.environ.get(env_name)
    env_present = bool(env_value and env_value.strip())
    file_configured = bool(cfg.get('api_key_file'))

    model = cfg.get('model')
    max_tokens = cfg.get('max_tokens')
    model_ok = isinstance(model, str) and bool(model.strip())
    tokens_ok = (
        isinstance(max_tokens, int)
        and not isinstance(max_tokens, bool)
        and max_tokens > 0
    )
    configured = bool(info['present']) and not errors and (env_present or file_configured)

    return {
        'ok': True,
        'kind': 'claude_status',
        'configured': configured,
        'call_ready': configured and model_ok and tokens_ok,
        'config_present': bool(info['present']),
        'config_path': str(config_path),
        'base_url': cfg.get('base_url') or OFFICIAL_BASE_URL,
        'api_version': cfg.get('api_version') or DEFAULT_API_VERSION,
        'model': model,
        'max_tokens': max_tokens,
        'thinking': cfg.get('thinking'),
        'timeout_seconds': cfg.get('timeout_seconds') or DEFAULT_TIMEOUT_SECONDS,
        'workspace_id_configured': bool(cfg.get('workspace_id')),
        'api_key_env': env_name,
        'api_key_env_present': env_present,
        'api_key_file_configured': file_configured,
        'errors': errors,
        'notes': list(STATUS_NOTES),
    }


# --------------------------------------------------------------------------- #
# MCP stdio server
# --------------------------------------------------------------------------- #

MCP_SERVER_INFO = {'name': 'claude-advisor-bridge', 'version': '1.0.0'}

TOOL_DEFINITIONS: List[Dict[str, Any]] = [
    {
        'name': 'claude_status',
        'description': (
            'Report local bridge configuration status. Performs no network request, '
            'makes no API call, and reads no key material.'
        ),
        'inputSchema': {
            'type': 'object',
            'properties': {},
            'additionalProperties': False,
        },
    },
    {
        'name': 'claude_consult',
        'description': (
            'Consult the Anthropic advisor for plan, discuss, review, ideas, paper, or '
            'research work. The bridge does not run tools, write files, browse, or retry.'
        ),
        'inputSchema': {
            'type': 'object',
            'properties': {
                'task': {'type': 'string'},
                'role': {'type': 'string', 'enum': list(ROLE_PROMPTS)},
                'context': {'type': 'string'},
                'root': {'type': 'string'},
                'files': {'type': 'array', 'items': {'type': 'string'}},
            },
            'required': ['task', 'role'],
            'additionalProperties': False,
        },
    },
]


def rpc_error(msg_id: Any, code: int, message: str) -> Dict[str, Any]:
    return {'jsonrpc': '2.0', 'id': msg_id, 'error': {'code': code, 'message': message}}


def tool_text_result(msg_id: Any, payload: Dict[str, Any]) -> Dict[str, Any]:
    return {
        'jsonrpc': '2.0',
        'id': msg_id,
        'result': {
            'content': [{'type': 'text', 'text': json.dumps(payload, ensure_ascii=False)}],
            'isError': not bool(payload.get('ok')),
        },
    }


def handle_tool_call(msg_id: Any, params: Dict[str, Any], config_path: Path) -> Dict[str, Any]:
    name = params.get('name')
    arguments = params.get('arguments')
    if arguments is None:
        arguments = {}
    if not isinstance(arguments, dict):
        return rpc_error(msg_id, -32602, 'tool arguments must be an object')

    if name == 'claude_status':
        unknown = sorted(set(arguments))
        if unknown:
            return tool_text_result(msg_id, BridgeError(
                'invalid_arguments', 'unknown arguments: ' + ', '.join(unknown)).to_json())
        return tool_text_result(msg_id, status_report(config_path))

    if name == 'claude_consult':
        unknown = sorted(set(arguments) - ALLOWED_CONSULT_ARGS)
        if unknown:
            return tool_text_result(msg_id, BridgeError(
                'invalid_arguments', 'unknown arguments: ' + ', '.join(unknown)).to_json())
        try:
            result = run_consult(
                config_path,
                arguments.get('task'),
                arguments.get('role'),
                arguments.get('context'),
                arguments.get('root'),
                arguments.get('files'),
            )
        except BridgeError as exc:
            return tool_text_result(msg_id, exc.to_json())
        return tool_text_result(msg_id, result)

    return rpc_error(msg_id, -32602, 'unknown tool: ' + str(name))


def handle_rpc(message: Dict[str, Any], config_path: Path) -> Optional[Dict[str, Any]]:
    method = message.get('method')
    msg_id = message.get('id')
    params = message.get('params')
    if not isinstance(params, dict):
        params = {}

    if method == 'initialize':
        requested = params.get('protocolVersion')
        negotiated = requested if requested in PROTOCOL_VERSIONS else FALLBACK_PROTOCOL_VERSION
        return {
            'jsonrpc': '2.0',
            'id': msg_id,
            'result': {
                'protocolVersion': negotiated,
                'capabilities': {'tools': {'listChanged': False}},
                'serverInfo': MCP_SERVER_INFO,
            },
        }

    if isinstance(method, str) and method.startswith('notifications/'):
        return None

    if method == 'ping':
        return {'jsonrpc': '2.0', 'id': msg_id, 'result': {}}

    if method == 'tools/list':
        return {'jsonrpc': '2.0', 'id': msg_id, 'result': {'tools': TOOL_DEFINITIONS}}

    if method == 'tools/call':
        return handle_tool_call(msg_id, params, config_path)

    return rpc_error(msg_id, -32601, 'method not found: ' + str(method))


def write_message(payload: Dict[str, Any]) -> None:
    sys.stdout.write(json.dumps(payload, ensure_ascii=False) + '\n')
    sys.stdout.flush()


def run_mcp_server(config_path: Path) -> int:
    for line in sys.stdin:
        stripped = line.strip()
        if not stripped:
            continue
        try:
            message = json.loads(stripped)
        except json.JSONDecodeError:
            write_message(rpc_error(None, -32700, 'parse error'))
            continue
        if not isinstance(message, dict):
            write_message(rpc_error(None, -32600, 'invalid request'))
            continue
        response = handle_rpc(message, config_path)
        if response is not None:
            write_message(response)
    return 0


# --------------------------------------------------------------------------- #
# CLI
# --------------------------------------------------------------------------- #

def _ensure_utf8_stdout() -> None:
    reconfigure = getattr(sys.stdout, 'reconfigure', None)
    if callable(reconfigure):
        try:
            reconfigure(encoding='utf-8')
        except (ValueError, OSError):
            pass


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog='bridge.py',
        description=(
            'Local status and on-demand Anthropic consultation for a supervising agent. '
            'Not an autonomous agent.'
        ),
    )
    parser.add_argument('--status', action='store_true',
                        help='print local status as JSON and exit (always exit code 0)')
    parser.add_argument('--serve', action='store_true',
                        help='run the stdio MCP server')
    parser.add_argument('--task', help='the question or work item to consult about')
    parser.add_argument('--role', choices=list(ROLE_PROMPTS), help='consultation role')
    parser.add_argument('--root', help='absolute project root used to resolve --files')
    parser.add_argument('--files', nargs='+', action='extend', default=None,
                        help='explicit paths relative to --root')
    parser.add_argument('--context', help='additional caller-supplied context text')
    parser.add_argument('--config', default=str(DEFAULT_CONFIG_PATH),
                        help='path to the bridge config JSON')
    return parser


def main(argv: Optional[List[str]] = None) -> int:
    _ensure_utf8_stdout()
    parser = build_parser()
    args = parser.parse_args(sys.argv[1:] if argv is None else argv)

    if args.serve:
        return run_mcp_server(Path(args.config))

    if args.status:
        print(json.dumps(status_report(Path(args.config)), ensure_ascii=False))
        return 0

    if args.task is None:
        parser.error('one of --status, --serve, or --task with --role is required')
    if args.role is None:
        parser.error('--role is required together with --task')

    try:
        result = run_consult(
            Path(args.config), args.task, args.role, args.context, args.root, args.files
        )
    except BridgeError as exc:
        print(json.dumps(exc.to_json(), ensure_ascii=False))
        return 1

    print(json.dumps(result, ensure_ascii=False))
    return 0


if __name__ == '__main__':
    sys.exit(main())
