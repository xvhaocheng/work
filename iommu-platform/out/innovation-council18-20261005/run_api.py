"""Mechanical orchestration of existing model CLIs; no network retries."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor, as_completed
import json
import hashlib
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work')
BASE = Path(__file__).resolve().parent
REQUEST = 'codex claude deepseek 分别创建6个agent，分别学习这个map，然后寻找可创新点。然后交叉审核'
ROUND = sys.argv[1]
SELECT = set(sys.argv[2:])
ROSTER = json.loads((BASE / 'roster.json').read_text())

def run(seat):
    sid = seat['id']
    dest = BASE / ROUND
    raw = dest / f'{sid}.json'
    if raw.exists():
        return {'id': sid, 'skipped_existing': True}
    task = (dest / f'{sid}-task.txt').read_text()
    extra_file = dest / f'{sid}-files.json'
    files = json.loads(extra_file.read_text()) if extra_file.exists() else seat['files']
    file_meta = [{'path': f, 'sha256': hashlib.sha256((ROOT / f).read_bytes()).hexdigest(), 'bytes': (ROOT / f).stat().st_size} for f in files]
    record = {'files_meta': file_meta, 'seat': sid, 'round': ROUND, 'task': task, 'files': files,
              'root': str(ROOT), 'user_request': REQUEST, 'started': time.time()}
    (dest / f'{sid}-input.json').write_text(json.dumps(record, ensure_ascii=False, indent=2))
    if sid.startswith('D'):
        argv = ['/home/xvhaocheng/.local/bin/deepseek', '--task', task,
                '--root', str(ROOT), '--files', *files]
    else:
        argv = ['/usr/bin/python3', str(ROOT / 'tools/claude-advisor/bridge.py'),
                '--task', task, '--role', 'ideas' if ROUND.startswith('round1') else 'review',
                '--user-request', REQUEST, '--root', str(ROOT), '--files', *files]
    if sid.startswith('A') and Path('/tmp/innovation-council18-claude-config.json').exists():
        argv.extend(['--config', '/tmp/innovation-council18-claude-config.json'])
        record['max_tokens_override'] = 32768
        (dest / f'{sid}-input.json').write_text(json.dumps(record, ensure_ascii=False, indent=2))
    print(json.dumps({'event': 'started', 'seat': sid, 'round': ROUND}), flush=True)
    with raw.open('w') as out, (dest / f'{sid}.stderr').open('w') as err:
        result = subprocess.run(argv, cwd=ROOT, stdout=out, stderr=err)
    summary = {'seat': sid, 'round': ROUND, 'exit_code': result.returncode,
               'elapsed_seconds': round(time.time() - record['started'], 1)}
    try:
        data = json.loads(raw.read_text())
        answer = data.get('answer', '')
        if answer:
            (dest / f'{sid}.md').write_text(answer)
        summary.update({k: data.get(k) for k in ['model', 'usage', 'truncated',
                       'finish_reason', 'stop_reason', 'error', 'error_code',
                       'requested_max_tokens', 'effective_max_tokens']})
        summary['answer_chars'] = len(answer)
    except (ValueError, TypeError) as exc:
        summary['parse_error'] = type(exc).__name__
    (dest / f'{sid}-result.json').write_text(json.dumps(summary, ensure_ascii=False, indent=2))
    print(json.dumps(summary, ensure_ascii=False), flush=True)
    return summary

if __name__ == '__main__':
    seats = [s for s in ROSTER if s['id'][0] in 'AD' and (not SELECT or s['id'] in SELECT)]
    with ThreadPoolExecutor(max_workers=6) as pool:
        futures = [pool.submit(run, seat) for seat in seats]
        summaries = [f.result() for f in as_completed(futures)]
    (BASE / ROUND / 'api-summary.json').write_text(json.dumps(summaries, ensure_ascii=False, indent=2))
