#!/usr/bin/env python3
'''claude-advisor key installer (Python 3.10 standard library only).

Run this yourself in a real interactive terminal:

    python3 /absolute/path/to/tools/claude-advisor/configure_key.py

Behaviour, in order:

  1. refuses to run without an interactive terminal, and never lets getpass
     fall back to echoing stdin in place of a TTY;
  2. reads the new Anthropic API key with getpass (never echoed, never on the
     command line, never printed, never logged, never read back from the
     config file or from any other file);
  3. strips the pasted value and checks only that it is a non-empty value that
     may legally be placed in an HTTP header;
  4. performs exactly ONE short connectivity request with the NEW key
     (max_tokens 512, the configured thinking and effort preserved), bypassing
     any key that may exist in the environment;
  5. only after that request succeeds writes the key file (0600, atomic
     replace) and updates ONLY api_key_file in config.json;
  6. registers a key-free MCP entry with `codex mcp add` when no entry with the
     same name exists yet.

The installer never reads workspace source files, never prints the key or any
part or length of it, never stores the key anywhere except the dedicated key
file, and never writes the daily max_tokens value back into the config.
'''

from __future__ import annotations

import getpass
import json
import os
import stat
import subprocess
import sys
import warnings
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

BRIDGE_DIR = Path(__file__).resolve().parent
if str(BRIDGE_DIR) not in sys.path:
    sys.path.insert(0, str(BRIDGE_DIR))

import bridge  # noqa: E402  (module import only; no source file is read)

BRIDGE_PATH = BRIDGE_DIR / 'bridge.py'
CONFIG_PATH = bridge.DEFAULT_CONFIG_PATH
PREPARATION_DIR = BRIDGE_DIR / 'preparation'
STATUS_PATH = PREPARATION_DIR / 'connection-status.json'

KEY_DIR_NAME = 'claude-advisor'
KEY_FILE_NAME = 'anthropic.key'

KEY_PROMPT = '请粘贴 Anthropic API 密钥（输入不会显示）：'
REPLACE_NOTICE = '新密钥验证后将替换本地旧密钥'
VERIFY_TASK = '仅用于 API 连通性检查，请只回复 OK。'
VERIFY_ROLE = 'review'
VERIFY_MAX_TOKENS = 512

MCP_SERVER_NAME = 'claude-advisor'
PYTHON_BIN = '/usr/bin/python3'
CODEX_TIMEOUT_SECONDS = 20

RETURN_HINT = (
    '密钥已保存，可回到对话继续；claude-advisor 工具需要在 Codex 重新加载或新会话后才会暴露。'
)

SUMMARY_NOTES = (
    'registered=true 只说明 codex mcp add 返回成功，不能证明宿主应用已经能看到该工具。',
    '需要 reload / 新会话后 Codex 才会暴露 claude-advisor 工具，当前会话的工具列表不保证立即更新。',
    'claude_consult 仍然只在用户当次请求明确点名 Claude/Anthropic 时可用。',
    '本次只更新 api_key_file，日常 max_tokens / thinking / effort 配置未被改动。',
    '连通性检查是唯一一次网络请求（max_tokens 512），不用于研究数据。',
)


class InstallerError(Exception):
    '''A failure with a fixed, non-secret message.'''

    def __init__(self, category: str, message: str) -> None:
        super().__init__(message)
        self.category = category


def eprint(message: str) -> None:
    sys.stderr.write(message + '\n')
    sys.stderr.flush()


def emit_summary(summary: Dict[str, Any]) -> None:
    sys.stdout.write(json.dumps(summary, ensure_ascii=False, indent=2) + '\n')
    sys.stdout.flush()


def build_summary() -> Dict[str, Any]:
    return {
        'ok': False,
        'keyconfigured': False,
        'registered': False,
        'key_path': None,
        'config_path': str(CONFIG_PATH),
        'mcp_server': MCP_SERVER_NAME,
        'connection': None,
        'messages': [],
        'errors': [],
        'notes': list(SUMMARY_NOTES),
    }


def describe_failure(exc: BaseException) -> str:
    '''A short, fixed description: no key, no headers, no raw provider body.'''
    if isinstance(exc, bridge.BridgeError):
        text = '连通性检查失败：' + exc.category
        if exc.status is not None:
            text += '（HTTP 状态 ' + str(exc.status) + '）'
        return text
    if isinstance(exc, InstallerError):
        return str(exc)
    return '安装器发生内部错误，未做进一步更改。'


def error_entry(exc: BaseException) -> Dict[str, Any]:
    '''Serialize a failure safely.

    BridgeError carries only category/status: its message and any provider body
    are deliberately dropped. InstallerError messages are written by this file
    and never contain key material.
    '''
    if isinstance(exc, bridge.BridgeError):
        entry: Dict[str, Any] = {'category': exc.category}
        if exc.status is not None:
            entry['status'] = exc.status
        return entry
    if isinstance(exc, InstallerError):
        return {'category': exc.category, 'message': str(exc)}
    return {'category': 'installer_error', 'message': '安装器发生内部错误。'}


# --------------------------------------------------------------------------- #
# Interactive input
# --------------------------------------------------------------------------- #

def tty_available() -> bool:
    '''True only when the key can be read without getpass echoing stdin.

    getpass falls back to reading (and echoing) sys.stdin when it cannot open
    /dev/tty. That fallback is never acceptable for secret input, so we refuse
    to continue unless stdin is a TTY or /dev/tty can be opened.
    '''
    try:
        if sys.stdin.isatty():
            return True
    except (ValueError, OSError):
        pass
    try:
        fd = os.open('/dev/tty', os.O_RDWR)
    except (OSError, ValueError):
        return False
    try:
        os.close(fd)
    except OSError:
        pass
    return True


def prompt_for_key() -> str:
    '''Read the pasted key once. Never echoes, never re-prompts.'''
    try:
        with warnings.catch_warnings():
            warnings.simplefilter('error', getpass.GetPassWarning)
            raw = getpass.getpass(KEY_PROMPT)
    except getpass.GetPassWarning:
        raise InstallerError('no_tty', '无法使用安全终端读取密钥；密钥未读取、未保存。')
    except (EOFError, KeyboardInterrupt):
        raise InstallerError('input_aborted', '密钥输入被中断；未做任何更改。')
    except Exception:
        raise InstallerError('input_failed', '无法读取密钥输入；未做任何更改。')
    return raw.strip()


def validate_key(key: str) -> None:
    '''Only strip + HTTP-header legality. The value itself is never reported.'''
    if not key:
        raise InstallerError('empty_key', '未收到密钥内容；未做任何更改。')
    try:
        bridge.validate_api_key_header_value(key)
    except bridge.BridgeError:
        raise InstallerError(
            'bad_key',
            '密钥包含无法作为 HTTP 头发送的字符（例如空白或不可打印字符）；未做任何更改。',
        )


# --------------------------------------------------------------------------- #
# Storage path rules
# --------------------------------------------------------------------------- #

def current_uid() -> Optional[int]:
    get_uid = getattr(os, 'getuid', None)
    if get_uid is None:
        return None
    try:
        return int(get_uid())
    except OSError:
        return None


def default_key_path() -> Path:
    try:
        home = Path.home()
    except (RuntimeError, OSError):
        raise InstallerError('bad_key_path', '无法确定当前用户主目录。')
    return home / '.config' / KEY_DIR_NAME / KEY_FILE_NAME


def resolve_key_path(cfg: Dict[str, Any]) -> Path:
    '''Default key path, unless the config already declares an absolute one.'''
    raw = cfg.get('api_key_file')
    if isinstance(raw, str) and raw.strip():
        candidate = Path(raw.strip())
        if not candidate.is_absolute():
            raise InstallerError('bad_key_path', 'config 中的 api_key_file 必须是绝对路径。')
        return candidate
    return default_key_path()


def assert_outside_workspace(key_path: Path) -> None:
    '''The key file (and therefore its directory) must not live in the repo.'''
    try:
        resolved = key_path.resolve(strict=False)
    except OSError:
        raise InstallerError('bad_key_path', '无法解析密钥路径。')
    for root in bridge.default_workspace_roots():
        try:
            root_resolved = root.resolve(strict=False)
        except OSError:
            continue
        if resolved == root_resolved or bridge.is_within(resolved, root_resolved):
            raise InstallerError('bad_key_path', '密钥文件不能位于工作区内。')


def ensure_private_directory(directory: Path) -> None:
    '''Create/verify the key directory: not a symlink, owner-only, mode 0700.'''
    if directory.is_symlink():
        raise InstallerError('bad_key_dir', '密钥目录是符号链接，已拒绝使用。')
    try:
        directory.mkdir(mode=0o700, parents=True, exist_ok=True)
    except OSError:
        raise InstallerError('bad_key_dir', '无法创建密钥目录。')
    try:
        info = directory.lstat()
    except OSError:
        raise InstallerError('bad_key_dir', '无法检查密钥目录。')
    if stat.S_ISLNK(info.st_mode):
        raise InstallerError('bad_key_dir', '密钥目录是符号链接，已拒绝使用。')
    if not stat.S_ISDIR(info.st_mode):
        raise InstallerError('bad_key_dir', '密钥目录路径不是目录。')
    uid = current_uid()
    if uid is not None and info.st_uid != uid:
        raise InstallerError('bad_key_dir', '密钥目录不属于当前用户，已拒绝使用。')
    if stat.S_IMODE(info.st_mode) != 0o700:
        try:
            os.chmod(directory, 0o700)
        except OSError:
            raise InstallerError('bad_key_dir', '密钥目录权限必须为 0700。')
        try:
            info = directory.lstat()
        except OSError:
            raise InstallerError('bad_key_dir', '无法检查密钥目录。')
        if stat.S_IMODE(info.st_mode) != 0o700:
            raise InstallerError('bad_key_dir', '密钥目录权限必须为 0700。')


def check_existing_key_file(key_path: Path) -> bool:
    '''Return True when a private key file will later be replaced.

    The old credential is never read and never touched here: it stays valid
    until the new key has been verified. A non-private or non-regular target is
    refused rather than silently overwritten.
    '''
    try:
        info = key_path.lstat()
    except FileNotFoundError:
        return False
    except OSError:
        raise InstallerError('bad_key_file', '无法检查现有密钥文件。')
    if stat.S_ISLNK(info.st_mode):
        raise InstallerError('bad_key_file', '现有密钥路径是符号链接，已拒绝覆盖。')
    if not stat.S_ISREG(info.st_mode):
        raise InstallerError('bad_key_file', '现有密钥路径不是普通文件，已拒绝覆盖。')
    uid = current_uid()
    if uid is not None and info.st_uid != uid:
        raise InstallerError('bad_key_file', '现有密钥文件不属于当前用户，已拒绝覆盖。')
    if info.st_mode & 0o077:
        raise InstallerError('bad_key_file', '现有密钥文件权限过宽（需 0600），已拒绝覆盖。')
    return True


# --------------------------------------------------------------------------- #
# Atomic writes
# --------------------------------------------------------------------------- #

def sync_directory(directory: Path) -> None:
    try:
        fd = os.open(str(directory), os.O_RDONLY)
    except OSError:
        return
    try:
        os.fsync(fd)
    except OSError:
        pass
    finally:
        try:
            os.close(fd)
        except OSError:
            pass


def atomic_write_bytes(target: Path, data: bytes, mode: int) -> None:
    '''Write via a fresh O_CREAT|O_EXCL|O_NOFOLLOW temp file, then os.replace.'''
    directory = target.parent
    flags = os.O_WRONLY | os.O_CREAT | os.O_EXCL | getattr(os, 'O_NOFOLLOW', 0)
    fd: Optional[int] = None
    tmp_path: Optional[Path] = None
    try:
        for _ in range(16):
            candidate = directory / ('.' + target.name + '.' + os.urandom(12).hex() + '.tmp')
            try:
                fd = os.open(str(candidate), flags, mode)
            except FileExistsError:
                fd = None
                continue
            except OSError:
                raise InstallerError('write_failed', '无法创建安全的临时文件。')
            tmp_path = candidate
            break
        if fd is None or tmp_path is None:
            raise InstallerError('write_failed', '无法创建安全的临时文件。')
        try:
            os.fchmod(fd, mode)
        except OSError:
            raise InstallerError('write_failed', '无法设置临时文件权限。')
        try:
            handle = os.fdopen(fd, 'wb')
        except OSError:
            raise InstallerError('write_failed', '无法写入临时文件。')
        fd = None
        with handle:
            handle.write(data)
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(str(tmp_path), str(target))
        tmp_path = None
        sync_directory(directory)
    finally:
        if fd is not None:
            try:
                os.close(fd)
            except OSError:
                pass
        if tmp_path is not None:
            try:
                os.unlink(str(tmp_path))
            except OSError:
                pass


def update_config_api_key_file(config_path: Path, key_path: Path) -> None:
    '''Rewrite config.json changing only api_key_file, atomically.'''
    try:
        text = config_path.read_text(encoding='utf-8')
    except (OSError, UnicodeDecodeError):
        raise InstallerError('config_error', '无法读取配置文件，未更新。')
    try:
        data = json.loads(text)
    except ValueError:
        raise InstallerError('config_error', '配置文件不是合法 JSON，未更新。')
    if not isinstance(data, dict):
        raise InstallerError('config_error', '配置文件不是 JSON 对象，未更新。')
    data['api_key_file'] = str(key_path)
    mode = 0o600
    try:
        existing = config_path.lstat()
        if stat.S_ISREG(existing.st_mode):
            mode = stat.S_IMODE(existing.st_mode) or 0o600
    except OSError:
        pass
    serialized = json.dumps(data, ensure_ascii=False, indent=2, sort_keys=True) + '\n'
    atomic_write_bytes(config_path, serialized.encode('utf-8'), mode)


# --------------------------------------------------------------------------- #
# Connectivity check (the only network request)
# --------------------------------------------------------------------------- #

def load_ready_config() -> Dict[str, Any]:
    info = bridge.load_config(CONFIG_PATH)
    if info['errors']:
        raise InstallerError(
            'config_error',
            '配置文件无效，未做任何更改：' + '; '.join(info['errors']),
        )
    if not info['present']:
        raise InstallerError(
            'config_error',
            '未找到配置文件（需由 Codex 先写入非敏感配置）：' + str(CONFIG_PATH),
        )
    cfg = info['values']
    try:
        bridge.validate_call_config(cfg)
    except bridge.BridgeError as exc:
        raise InstallerError(
            'config_error',
            '配置不可用于调用（model / max_tokens / thinking / effort 之一不合法）：' + str(exc),
        )
    return cfg


def safe_usage(usage: Any) -> Optional[Dict[str, int]]:
    if not isinstance(usage, dict):
        return None
    safe: Dict[str, int] = {}
    for name in (
        'input_tokens',
        'output_tokens',
        'cache_creation_input_tokens',
        'cache_read_input_tokens',
    ):
        value = usage.get(name)
        if isinstance(value, int) and not isinstance(value, bool):
            safe[name] = value
    return safe or None


def build_verification_payload(cfg: Dict[str, Any]) -> Tuple[Dict[str, Any], Dict[str, Any]]:
    '''Build the single short check request, without touching the real config.

    Only max_tokens is overridden (512). thinking and effort are taken from the
    configured values, and nothing is written back to config.json.
    '''
    dry_cfg = dict(cfg)
    dry_cfg['max_tokens'] = VERIFY_MAX_TOKENS
    try:
        bridge.validate_call_config(dry_cfg)
    except bridge.BridgeError as exc:
        raise InstallerError('config_error', '配置不可用于连通性检查：' + str(exc))
    payload = bridge.build_payload(dry_cfg, VERIFY_ROLE, VERIFY_TASK, None, '')
    return dry_cfg, payload


def verify_key(cfg: Dict[str, Any], key: str, opener: Any = None) -> Dict[str, Any]:
    '''The ONE real connectivity request, made directly with the NEW key.

    This deliberately bypasses bridge.run_consult: run_consult resolves a key
    from the environment or from the old file, would demand a user_request, and
    applies the daily max_tokens. Here the user authorized a short check by
    running this installer, and the check must use the freshly pasted key.
    Nothing sensitive is printed: the caller receives a non-secret summary and
    the raw response never reaches stdout.
    '''
    dry_cfg, payload = build_verification_payload(cfg)
    raw = bridge.call_anthropic(dry_cfg, payload, key, opener=opener)
    return summarize_connection(cfg, raw)


def summarize_connection(cfg: Dict[str, Any], raw: Any) -> Dict[str, Any]:
    '''Accept only a real, complete message; otherwise fail safely.'''
    if not isinstance(raw, dict):
        raise InstallerError('invalid_response', '连通性检查返回了意外的响应结构；未保存密钥。')
    if raw.get('type') != 'message':
        raise InstallerError('invalid_response', '连通性检查未返回 message 响应；未保存密钥。')

    actual_model = raw.get('model')
    expected_model = cfg.get('model')
    if not isinstance(actual_model, str) or not actual_model.strip():
        raise InstallerError('invalid_response', '响应缺少模型标识；未保存密钥。')
    if isinstance(expected_model, str) and expected_model.strip() and actual_model != expected_model:
        raise InstallerError(
            'model_mismatch',
            '响应模型与配置不一致：期望 ' + expected_model + '，实际 ' + actual_model + '；未保存密钥。',
        )

    content = raw.get('content')
    texts: List[str] = []
    if isinstance(content, list):
        for block in content:
            if (
                isinstance(block, dict)
                and block.get('type') == 'text'
                and isinstance(block.get('text'), str)
            ):
                texts.append(block['text'])
    if not texts or not any(item.strip() for item in texts):
        raise InstallerError('invalid_response', '响应没有非空文本内容；未保存密钥。')

    stop_reason = raw.get('stop_reason')
    if stop_reason != 'end_turn':
        raise InstallerError(
            'incomplete_response',
            '连通性检查未正常结束（stop_reason 不是 end_turn）；未保存密钥。',
        )

    return {
        'status': 'ok',
        'model': actual_model,
        'usage': safe_usage(raw.get('usage')),
        'stop_reason': stop_reason,
    }


def failure_record(category: str) -> Dict[str, Any]:
    return {'status': category, 'model': None, 'usage': None, 'stop_reason': None}


def write_connection_status(record: Dict[str, Any]) -> Optional[str]:
    '''Persist a non-sensitive summary. No key, no headers, no raw body.'''
    try:
        PREPARATION_DIR.mkdir(mode=0o700, parents=True, exist_ok=True)
    except OSError:
        return '无法创建 preparation 目录，连接摘要未保存。'
    try:
        payload = json.dumps(record, ensure_ascii=False, indent=2, sort_keys=True) + '\n'
        atomic_write_bytes(STATUS_PATH, payload.encode('utf-8'), 0o600)
    except (InstallerError, OSError):
        return '连接摘要写入失败。'
    return None


# --------------------------------------------------------------------------- #
# MCP registration (key-free)
# --------------------------------------------------------------------------- #

def run_codex(args: List[str]) -> Optional[subprocess.CompletedProcess]:
    '''Run the codex CLI once, capturing output; output is never echoed.'''
    try:
        return subprocess.run(
            args,
            cwd=str(BRIDGE_DIR),
            capture_output=True,
            text=True,
            timeout=CODEX_TIMEOUT_SECONDS,
            check=False,
        )
    except (OSError, subprocess.SubprocessError, ValueError):
        return None


def register_mcp_server() -> Tuple[bool, List[str]]:
    '''Register the key-free MCP entry unless one with the same name exists.'''
    get_result = run_codex(['codex', 'mcp', 'get', MCP_SERVER_NAME])
    if get_result is not None and get_result.returncode == 0:
        return False, ['已有同名入口，密钥已保存，需Codex核查；未做覆盖。']

    add_result = run_codex([
        'codex', 'mcp', 'add', MCP_SERVER_NAME, '--',
        PYTHON_BIN, str(BRIDGE_PATH), '--serve', '--config', str(CONFIG_PATH),
    ])
    if add_result is None:
        return False, ['无法运行 codex 命令（未找到二进制或超时）；密钥已保存，注册需由 Codex 后续人工完成。']
    if add_result.returncode == 0:
        return True, ['已注册 claude-advisor MCP 入口；需重启或新会话后宿主才会暴露该工具。']
    return False, ['codex mcp add 返回非零状态，未注册；密钥已保存，注册可由 Codex 后续人工完成。']


# --------------------------------------------------------------------------- #
# Entry point
# --------------------------------------------------------------------------- #

def main(argv: Optional[List[str]] = None) -> int:
    raw_args = sys.argv[1:] if argv is None else list(argv)
    summary = build_summary()

    if raw_args:
        eprint('本安装器不接受任何命令行参数：密钥只在交互终端输入。')
        summary['errors'].append({'category': 'unexpected_arguments'})
        emit_summary(summary)
        return 1

    key_path: Optional[Path] = None
    try:
        if not tty_available():
            raise InstallerError(
                'no_tty',
                '未检测到交互终端；密钥未读取、未保存。请在真实终端中重新运行。',
            )

        cfg = load_ready_config()
        key_path = resolve_key_path(cfg)
        summary['key_path'] = str(key_path)
        assert_outside_workspace(key_path)
        ensure_private_directory(key_path.parent)
        if check_existing_key_file(key_path):
            eprint(REPLACE_NOTICE)

        new_key = prompt_for_key()
        validate_key(new_key)

        try:
            connection = verify_key(cfg, new_key)
        except bridge.BridgeError as exc:
            write_connection_status(failure_record(exc.category))
            raise
        except InstallerError as exc:
            write_connection_status(failure_record(exc.category))
            raise
        except Exception:
            write_connection_status(failure_record('installer_error'))
            raise

        summary['connection'] = connection
        write_connection_status(connection)

        atomic_write_bytes(key_path, (new_key + '\n').encode('utf-8'), 0o600)
        update_config_api_key_file(CONFIG_PATH, key_path)
        summary['keyconfigured'] = True
    except InstallerError as exc:
        summary['errors'].append(error_entry(exc))
        eprint(describe_failure(exc))
        emit_summary(summary)
        return 1
    except bridge.BridgeError as exc:
        summary['errors'].append(error_entry(exc))
        eprint(describe_failure(exc))
        emit_summary(summary)
        return 1
    except KeyboardInterrupt:
        summary['errors'].append({'category': 'interrupted'})
        eprint('操作被中断。')
        emit_summary(summary)
        return 1
    except Exception:
        summary['errors'].append(error_entry(RuntimeError('unexpected installer failure')))
        eprint('安装器发生内部错误，未做进一步更改。')
        emit_summary(summary)
        return 1

    try:
        registered, messages = register_mcp_server()
    except Exception:
        registered = False
        messages = ['MCP 注册阶段发生内部错误；密钥已保存，注册可由 Codex 后续人工完成。']
    summary['ok'] = True
    summary['registered'] = bool(registered)
    summary['messages'].extend(messages)
    if not registered:
        summary['messages'].append(RETURN_HINT)
    emit_summary(summary)
    return 0


if __name__ == '__main__':
    sys.exit(main())
