#!/usr/bin/env python3
"""After successful new E2, load E5 once and collect E4 plus histograms."""
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
    receipt = BOARD / 'runs/e2-common-v2-collect01-launch/result.json'
    deadline = time.monotonic() + 13000
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('New E2 has not completed; E4 not started')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0, 'E2 failed; Root must assess before any next load'
    e2 = json.loads((BOARD / 'runs/e2-device-scaling-20261018-02/manifest.json').read_text())
    assert e2['status'] == 'passed' and e2['recorded_cases'] == 90
    assert e2['software_profile'] == 'full-e3-v1'
    subprocess.run([sys.executable, str(BOARD / 'program_and_boot.py'), '--bit',
        str(PACKAGE / 'artifacts/ds0.bit'), '--expected-sha256',
        'd934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c',
        '--expected-kernel', '6.5.0-rc2-ispass2027-full-e3-v1',
        '--credential-helper', '/home/xvhaocheng/work/iommu-platform/out/r107-execution-gate-20261012/route/login_current_board.py',
        '--output', str(BOARD / 'runs/e4-e5-common-v2-load01')],
        cwd=ROOT, check=True, timeout=1800)
    argv = [sys.executable, str(PAPER / 'exp/E4-pagesize/collect_pages.py'),
        '--package', str(PACKAGE), '--load-manifest', str(BOARD / 'runs/e4-e5-common-v2-load01/load.json'),
        '--fpga-manifest', str(PACKAGE / 'metadata/fpga-build-manifest.json'),
        '--software-manifest', str(PACKAGE / 'metadata/software-build-manifest.json'),
        '--e5-dir', str(PAPER / 'exp/E5-latency'), '--board-dir', str(BOARD),
        '--output', str(BOARD / 'runs/e4-pagesize-20261018-01')]
    subprocess.run(argv + ['--check-only'], cwd=ROOT, check=True, timeout=20)
    subprocess.run(argv, cwd=ROOT, check=True)


if __name__ == '__main__':
    main()
