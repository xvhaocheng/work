#!/usr/bin/env python3
"""Build the isolated SC fix profile and preserve status/logs without overwriting."""
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
out = root / 'out/board-bringup-20260915-mac'
out.mkdir(parents=True, exist_ok=True)
status_path = out / 'fpga-build-status.json'
log = (out / 'fpga-build.log').open('x')
if status_path.exists():
    raise SystemExit('Existing build status; inspect before retrying')
env = dict(os.environ)
env['PATH'] = '/tools/Xilinx/Vivado/2023.1/bin:' + env['PATH']
env['XILINXD_LICENSE_FILE'] = '2100@opus4.apal.cs.waseda.ac.jp'
cmd = ['make', 'fpga', 'PROFILE=migration-4dma-scfix', 'JOBS=12']
status = {'status':'running', 'started_utc':datetime.now(timezone.utc).isoformat(),
          'command':cmd, 'wrapper_pid':os.getpid()}
status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Building migration-4dma-scfix; log: '+str(log.name), flush=True)
try:
    p = subprocess.Popen(cmd, cwd=root, env=env, stdout=log, stderr=subprocess.STDOUT)
    status['make_pid'] = p.pid
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    status['exit_code'] = p.wait()
    status['status'] = 'passed' if p.returncode == 0 else 'failed'
    if p.returncode == 0:
        base = root/'out/migration-4dma-scfix/fpga-src/cva6/corev_apu/fpga/work-fpga'
        status['artifacts'] = {}
        for name in ('ariane_xilinx.bit','ariane_xilinx.mcs'):
            path = base/name
            data = path.read_bytes()
            status['artifacts'][name] = {'path':str(path),'bytes':len(data),
                                         'sha256':hashlib.sha256(data).hexdigest()}
except BaseException as exc:
    status['status'] = 'failed'
    status['error'] = repr(exc)
    raise
finally:
    log.close()
    status['finished_utc'] = datetime.now(timezone.utc).isoformat()
    status_path.write_text(json.dumps(status,indent=2)+'\n')
    print(json.dumps(status,indent=2), flush=True)
raise SystemExit(status['exit_code'])
