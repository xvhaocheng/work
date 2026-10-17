#!/usr/bin/env python3
"""Execute one explicitly selected reviewed probe step; never advance a plan."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path('/home/xvhaocheng/work-root-a')
BOARD = ROOT / 'paper/ispass2027/board'
RENDERER = ROOT / 'paper/ispass2027/exp/E2-device-scaling/irq-diagnosis/run-v1/prepare_run.py'


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--plan', type=Path, required=True)
    ap.add_argument('--step', choices=['preflight', 'N2_A1_window', 'N2_A1_post',
                                      'N4_A8_window', 'N4_A8_post'], required=True)
    ap.add_argument('--output', type=Path, required=True)
    a = ap.parse_args()
    plan = json.loads(a.plan.read_text())
    assert plan['script_sha256'] == hashlib.sha256(RENDERER.read_bytes()).hexdigest()
    step, = [x for x in plan['steps'] if x['name'] == a.step]
    # Root reviews the previous stage before each separate invocation.
    subprocess.run([sys.executable, str(BOARD / 'uart.py'), '--output',
        str(a.output), '--command', step['command'], '--timeout',
        str(step['host_timeout_seconds'])], cwd=ROOT, check=True,
        timeout=step['host_timeout_seconds'] + 20)


if __name__ == '__main__':
    main()
