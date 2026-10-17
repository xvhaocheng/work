#!/usr/bin/env python3
"""Wait for this one accepted deployment, then collect one new E2 matrix."""
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
PAPER = ROOT / 'paper/ispass2027'


def main():
    receipt = PAPER / 'board/runs/common-v2-deploy01-launch/result.json'
    deadline = time.monotonic() + 6200
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('Common deployment not completed; no collection attempted')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0, 'Deployment failed; stop'
    loaded = json.loads((PAPER / 'board/runs/e2-common-v2-ds0-load01/load.json').read_text())
    assert loaded['status'] == 'boot_identity_verified'
    assert loaded['boot_id'] != '2f31b9f4-cf88-4061-b33c-3cf5a5592795', 'Old tainted boot cannot be reused'
    argv = [sys.executable, str(PAPER / 'exp/E2-device-scaling/collect_multi.py'),
        '--ds', '0/0', '--expected-kernel', '6.5.0-rc2-ispass2027-full-e3-v1',
        '--load-manifest', str(PAPER / 'board/runs/e2-common-v2-ds0-load01/load.json'),
        '--source-record', str(PAPER / 'board/packages/baseline100-20261017/source.json'),
        '--software-manifest', str(PAPER / 'exp/E3-workloads/common-v2-software-build-manifest.json'),
        '--output', str(PAPER / 'board/runs/e2-device-scaling-20261018-02')]
    subprocess.run(argv + ['--check-only'], cwd=ROOT, check=True, timeout=20)
    # The collector checks actual pre/post taint, data, lifecycle and observer
    # evidence at every point. Any failure stops this run without replacements.
    subprocess.run(argv, cwd=ROOT, check=True)


if __name__ == '__main__':
    main()
