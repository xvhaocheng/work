import os, sys, json, time, subprocess, selectors, tempfile, shutil, hashlib, re
from pathlib import Path
from datetime import datetime, timezone

OUT = Path(__file__).parent
SHARED = Path('/home/xvhaocheng/.codex')
CLI = '/home/xvhaocheng/.local/bin/codex'
CWD = '/home/xvhaocheng/work'
NONCE = os.urandom(32)

def now():
    return datetime.now(timezone.utc).isoformat()

def dump(name, value):
    (OUT / name).write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n')

def scrub(value, key=''):
    if re.search(r'token|password|secret|authorization|cookie|api.?key', key, re.I):
        # Preserve numerical context/token configuration fields.
        if key in ['model_context_window', 'model_auto_compact_token_limit']:
            return value
        return '[sensitive field omitted]'
    if isinstance(value, dict):
        return {k: scrub(v, k) for k, v in value.items()}
    if isinstance(value, list):
        return [scrub(v, key) for v in value]
    return value

def identity(value):
    if value is None:
        return None
    return hashlib.sha256(NONCE + json.dumps(value, sort_keys=True).encode()).hexdigest()[:16]

def catalog(path):
    if not path.is_file():
        return {'exists': False}
    data = json.loads(path.read_text())
    entry = next((x for x in data.get('models', []) if x.get('slug') == 'gpt-6.1-sol'), None)
    return {'exists': True, 'path': str(path), 'fetched_at': data.get('fetched_at'),
            'etag': data.get('etag'), 'client_version': data.get('client_version'),
            'identity_comparison_tag': identity(data.get('identity')),
            'sol': {k: entry.get(k) for k in ['slug', 'context_window', 'max_context_window',
                    'effective_context_window_percent', 'default_reasoning_level']} if entry else None}

def rpc_probe(label, args, env, cache_path):
    before = catalog(cache_path)
    proc = subprocess.Popen(args, env=env, cwd=CWD, stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, bufsize=1)
    sel = selectors.DefaultSelector()
    sel.register(proc.stdout, selectors.EVENT_READ, 'stdout')
    sel.register(proc.stderr, selectors.EVENT_READ, 'stderr')
    transcript, diagnostics = [], []
    next_id = 1
    def request(method, params):
        nonlocal next_id
        rid = next_id; next_id += 1
        sent = {'id': rid, 'method': method, 'params': params}
        transcript.append({'at_utc': now(), 'direction': 'request', 'message': scrub(sent)})
        proc.stdin.write(json.dumps(sent) + '\n'); proc.stdin.flush()
        deadline = time.monotonic() + 55
        while time.monotonic() < deadline:
            ready = sel.select(min(1, deadline - time.monotonic()))
            for key, _ in ready:
                line = key.fileobj.readline()
                if not line:
                    sel.unregister(key.fileobj)
                    continue
                if key.data == 'stderr':
                    # Store only model-manager/path status diagnostics, never headers/body credentials.
                    if 'models_manager' in line or 'models cache:' in line or 'fetch models' in line:
                        diagnostics.append(line.strip())
                    continue
                try:
                    msg = json.loads(line)
                except ValueError:
                    continue
                if msg.get('id') == rid and ('result' in msg or 'error' in msg):
                    return msg
                # Do not persist unrelated thread notifications or perform server requests.
                if 'method' in msg and 'id' in msg:
                    diagnostics.append('Unexpected server request: ' + str(msg.get('method')))
            if proc.poll() is not None:
                raise RuntimeError(label + ' process exited before RPC reply')
        raise TimeoutError(label + ' RPC timed out: ' + method)

    result = {'label': label, 'started_at_utc': now(), 'pid': proc.pid, 'args': args,
              'code_home': env.get('CODEX_HOME'), 'cwd': CWD, 'catalog_before': before}
    try:
        init = request('initialize', {'clientInfo': {'name': 'codex_context_diagnostic',
                           'title': 'Read-only context diagnostics', 'version': '0.160.0'},
                           'capabilities': {'experimentalApi': True}})
        transcript.append({'at_utc': now(), 'direction': 'response', 'message': scrub(init)})
        result['initialize'] = scrub(init)
        if 'error' in init:
            raise RuntimeError('initialize failed')
        proc.stdin.write(json.dumps({'method': 'initialized', 'params': {}}) + '\n'); proc.stdin.flush()
        response = request('config/read', {'cwd': CWD, 'includeLayers': True})
        safe = scrub(response)
        transcript.append({'at_utc': now(), 'direction': 'response', 'message': safe})
        result['config_read'] = safe
        response = request('account/read', {'refreshToken': False})
        account = response.get('result', {}).get('account')
        # Only consistency tags and account kind; no email or workspace/account IDs.
        safe_account = {'type': account.get('type'), 'planType': account.get('planType'),
                        'identity_comparison_tag': identity(account)} if isinstance(account, dict) else None
        result['account_read'] = {'error': scrub(response.get('error')), 'account': safe_account}
        transcript.append({'at_utc': now(), 'direction': 'response', 'method': 'account/read',
                           'message': result['account_read']})
        response = request('model/list', {'includeHidden': True, 'limit': 100})
        result['model_list'] = scrub(response)
        transcript.append({'at_utc': now(), 'direction': 'response', 'message': scrub(response)})
        result['catalog_after'] = catalog(cache_path)
        result['finished_at_utc'] = now()
    except Exception as exc:
        result['error'] = type(exc).__name__ + ': ' + str(exc)
        result['catalog_after'] = catalog(cache_path)
    finally:
        if proc.stdin and not proc.stdin.closed:
            proc.stdin.close()
        try:
            proc.wait(timeout=8)
        except subprocess.TimeoutExpired:
            # Terminate only the diagnostic connection/process we spawned, never the desktop server.
            proc.terminate()
            try:
                proc.wait(timeout=3)
            except subprocess.TimeoutExpired:
                proc.kill(); proc.wait()
        result['probe_connection_exit_code'] = proc.returncode
        result['model_diagnostics'] = diagnostics
        dump(label + '-probe.json', result)
        (OUT / (label + '-rpc-transcript.jsonl')).write_text(''.join(json.dumps(x, ensure_ascii=False) + '\n' for x in transcript))
        sel.close()
    return result

env = os.environ.copy()
env.pop('CODEX_THREAD_ID', None); env.pop('CODEX_SESSION_ID', None)
# Supported proxy transport connects to the existing desktop server. No thread/turn is created.
desktop_env = env.copy(); desktop_env['CODEX_HOME'] = str(SHARED)
desktop = rpc_probe('desktop', [CLI, 'app-server', 'proxy'], desktop_env, SHARED / 'models_cache.json')

# Separate CODEX_HOME prevents competing cache/database/control-socket writes.
with tempfile.TemporaryDirectory(prefix='codex-context-isolated-') as tmp:
    home = Path(tmp); os.chmod(home, 0o700)
    for name in ['config.toml', 'auth.json']:
        shutil.copyfile(SHARED / name, home / name); os.chmod(home / name, 0o600)
    assert not (home / 'models_cache.json').exists()
    isolated_env = env.copy(); isolated_env['CODEX_HOME'] = str(home)
    isolated_env['RUST_LOG'] = 'warn,codex_models_manager=info'
    isolated = rpc_probe('isolated-cli', [CLI, 'app-server', '--listen', 'stdio://'],
                         isolated_env, home / 'models_cache.json')
    isolated['config_copy_matches_shared'] = (home / 'config.toml').read_bytes() == (SHARED / 'config.toml').read_bytes()
    isolated['shared_auth_source_copied_without_output'] = True
    # Persist native Sol response entry and metadata only, never private credentials or identity.
    cache = home / 'models_cache.json'
    if cache.is_file():
        full = json.loads(cache.read_text()); sol = next((x for x in full.get('models', []) if x.get('slug') == 'gpt-6.1-sol'), None)
        dump('isolated-native-sol.json', {'fetched_at': full.get('fetched_at'), 'etag': full.get('etag'),
                                        'client_version': full.get('client_version'), 'sol': sol})
    dump('isolated-cli-probe.json', isolated)

consistency = {'captured_at_utc': now(),
    'account_read_comparison': desktop.get('account_read') == isolated.get('account_read'),
    'catalog_identity_comparison': desktop.get('catalog_after', {}).get('identity_comparison_tag') == isolated.get('catalog_after', {}).get('identity_comparison_tag'),
    'desktop_sol': desktop.get('catalog_after', {}).get('sol'),
    'isolated_sol': isolated.get('catalog_after', {}).get('sol'),
    'isolated_private_home_removed': True,
    'no_thread_or_turn_started': True, 'no_manual_shared_cache_edits': True}
dump('paired-comparison.json', consistency)
for label, result in [('desktop', desktop), ('isolated-cli', isolated)]:
    print(json.dumps({'label': label, 'error': result.get('error'),
                     'initialize': result.get('initialize'), 'account_read': result.get('account_read'),
                     'catalog_before': result.get('catalog_before'), 'catalog_after': result.get('catalog_after'),
                     'diagnostics': result.get('model_diagnostics')}, ensure_ascii=False))
print(json.dumps(consistency, ensure_ascii=False))
