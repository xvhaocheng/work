#!/usr/bin/env python3
"""Produce one immutable G paper export after complete40; host only."""
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
PAPER = ROOT / 'paper/ispass2027'
BOARD = PAPER / 'board'


def main():
    receipt = BOARD / 'runs/g-collect01-launch/result.json'
    deadline = time.monotonic() + 6000
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('G collector not finished; no paper export')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0, 'G collection failed'
    run = BOARD / 'runs/g-ds0-20261018-01'
    manifest = json.loads((run / 'manifest.json').read_text())
    assert manifest['status'] == 'passed' and manifest['recorded_cases'] == 40
    assert manifest['fixture_only'] is False and manifest['evidence_level'] == 'fpga_board'
    subprocess.run([sys.executable, str(PAPER / 'exp/E6-mitigation/analyze_g.py'),
        '--run', str(run), '--output', str(PAPER / 'exp/E6-mitigation/paper/g-ds0-final01')],
        cwd=ROOT, check=True, timeout=300)


if __name__ == '__main__':
    main()
