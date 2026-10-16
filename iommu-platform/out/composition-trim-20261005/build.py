#!/usr/bin/env python3
from pathlib import Path
from datetime import datetime, timezone
import json
import os
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
BASE = ROOT / 'out/composition-trim-20261005'
STATE = BASE / 'build-state.json'
LOG = BASE / 'build.log'
PLATFORM = ROOT / 'scripts/platform.py'
PROFILE = 'migration-4dma-composition'
OVERLAY = 'out/composition-trim-20261005/workspace-overlay.json'
FPGA_STAGE = 'out/composition-trim-20261005/fpga-src'

def utc_now():
    return datetime.now(timezone.utc).isoformat()

def atomic_write_json(path, data):
    tmp = path.with_name(f'.{path.name}.tmp.{os.getpid()}')
    tmp.write_text(json.dumps(data, indent=2, sort_keys=True) + '\n', encoding='utf-8')
    os.replace(tmp, path)

def main():
    BASE.mkdir(parents=True, exist_ok=True)
    try:
        fd = os.open(STATE, os.O_CREAT | os.O_EXCL | os.O_WRONLY, 0o644)
    except FileExistsError:
        print(f'{STATE} exists; refusing to start', file=sys.stderr)
        return 2

    state = {
        'status': 'running',
        'started_at': utc_now(),
        'pid': os.getpid(),
        'profile': PROFILE,
        'workspace_overlay': OVERLAY,
        'fpga_stage': FPGA_STAGE,
    }
    with os.fdopen(fd, 'w', encoding='utf-8') as handle:
        json.dump(state, handle, indent=2, sort_keys=True)
        handle.write('\n')
        handle.flush()
        os.fsync(handle.fileno())

    cmd = [
        sys.executable,
        str(PLATFORM),
        'fpga',
        '--profile', PROFILE,
        '--workspace-overlay', OVERLAY,
        '--fpga-stage', FPGA_STAGE,
    ]
    try:
        with LOG.open('wb') as log:
            result = subprocess.run(
                cmd,
                cwd=ROOT,
                stdin=subprocess.DEVNULL,
                stdout=log,
                stderr=subprocess.STDOUT,
                check=False,
            )
        state.update({
            'status': 'compile_finished' if result.returncode == 0 else 'failed',
            'exit_code': result.returncode,
            'finished_at': utc_now(),
        })
        atomic_write_json(STATE, state)
        return result.returncode
    except BaseException as exc:
        state.update({
            'status': 'failed',
            'exit_code': 1,
            'finished_at': utc_now(),
            'exception': f'{type(exc).__name__}: {exc}',
        })
        atomic_write_json(STATE, state)
        raise

if __name__ == '__main__':
    raise SystemExit(main())
