#!/usr/bin/env python3
"""Collect E4/E5 once after Root's explicit post-failure E5 reload."""
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
PAPER = ROOT / 'paper/ispass2027'
BOARD = PAPER / 'board'
PACKAGE = BOARD / 'packages/e4-e5-common-v2-20261018'


def main():
    receipt = BOARD / 'runs/e4-e5-common-v2-load02-launch/result.json'
    deadline = time.monotonic() + 2000
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('E5 recovery load did not finish; no collection')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0, 'E5 load failed'
    load = BOARD / 'runs/e4-e5-common-v2-load02/load.json'
    argv = [sys.executable, str(PAPER / 'exp/E4-pagesize/collect_pages.py'),
        '--package', str(PACKAGE), '--load-manifest', str(load),
        '--fpga-manifest', str(PACKAGE / 'metadata/fpga-build-manifest.json'),
        '--software-manifest', str(PACKAGE / 'metadata/software-build-manifest.json'),
        '--e5-dir', str(PAPER / 'exp/E5-latency'), '--board-dir', str(BOARD),
        '--output', str(BOARD / 'runs/e4-pagesize-20261018-01')]
    subprocess.run(argv + ['--check-only'], cwd=ROOT, check=True, timeout=20)
    subprocess.run(argv, cwd=ROOT, check=True, timeout=7200)


if __name__ == '__main__':
    main()
