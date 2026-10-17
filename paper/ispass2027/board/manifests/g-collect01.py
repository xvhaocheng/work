#!/usr/bin/env python3
"""Collect G40 once after the separately qualified G load; never reload/retry."""
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
PAPER = ROOT / 'paper/ispass2027'
BOARD = PAPER / 'board'
PACKAGE = BOARD / 'packages/g-common-20261018'


def main():
    receipt = BOARD / 'runs/g-common-load01-launch/result.json'
    deadline = time.monotonic() + 1500
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('G load did not complete; no collection')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0, 'G load failed'
    argv = [sys.executable, str(PAPER / 'exp/E6-mitigation/collect_g.py'),
        '--output', str(BOARD / 'runs/g-ds0-20261018-01'),
        '--load-manifest', str(BOARD / 'runs/g-common-load01/load.json'),
        '--fpga-manifest', str(PACKAGE / 'metadata/fpga-build-manifest.json'),
        '--software-manifest', str(PACKAGE / 'metadata/software-build-manifest.json'),
        '--backend-evidence', str(PAPER / 'exp/E3-workloads/backend-validation/runs/run03/summary.json'),
        '--board-tools', str(BOARD)]
    subprocess.run(argv, cwd=ROOT, check=True, timeout=5500)


if __name__ == '__main__':
    main()
