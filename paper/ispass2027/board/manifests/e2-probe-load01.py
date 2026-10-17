#!/usr/bin/env python3
"""Wait for the single probe SD update, load original DS0, verify new boot."""
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
BOARD = ROOT / 'paper/ispass2027/board'


def main():
    receipt = BOARD / 'runs/e2-probe-software-write01-launch/result.json'
    deadline = time.monotonic() + 2400
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('probe SD update has no completion receipt; no load')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0, 'SD update failed; no load'
    session = json.loads((BOARD / 'runs/e2-probe-software-write01/session.json').read_text())
    assert session['exit_code'] == 0, 'SD session failed; no load'
    subprocess.run([sys.executable, str(BOARD / 'program_and_boot.py'),
        '--bit', str(BOARD / 'packages/baseline100-20261017/artifacts/ds0.bit'),
        '--expected-sha256', 'e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6',
        '--expected-kernel', '6.5.0-rc2-ispass2027-e2-timer-probe-v1',
        '--credential-helper', '/home/xvhaocheng/work/iommu-platform/out/r107-execution-gate-20261012/route/login_current_board.py',
        '--output', str(BOARD / 'runs/e2-probe-ds0-load01')],
        cwd=ROOT, check=True, timeout=1300)


if __name__ == '__main__':
    main()
