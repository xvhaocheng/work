#!/usr/bin/env python3
"""Complete one acknowledged original pathv3 update and boot. No measurements."""
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
BOARD = ROOT / 'paper/ispass2027/board'


def main():
    receipt = BOARD / 'runs/pn-original-upload01-launch/result.json'
    deadline = time.monotonic() + 2800
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('original software upload did not finish; no SD write')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0
    package = json.loads((BOARD / 'packages/e6-pn-common-20261018/transport.json').read_text())
    upload = json.loads((BOARD / 'runs/pn-original-upload01/pathv3-payload-upload-result.json').read_text())
    assert upload['byte_count_acknowledged'] and upload['bytes'] == package['payload_gzip']['bytes']
    assert upload['sha256'] == package['payload_gzip']['sha256']
    spi = json.loads((BOARD / 'runs/pn-original-upload01/probe-spi-upload-result.json').read_text())
    assert spi['byte_count_acknowledged'] and spi['sha256'] == package['current_kernel_spi_archive']['sha256']
    boot = json.loads((BOARD / 'runs/e2-probe-recovery-load01/load.json').read_text())
    assert boot['status'] == 'boot_identity_verified'
    command = f'''set -e
test "$(uname -r)" = 6.5.0-rc2-ispass2027-e2-timer-probe-v1
test "$(cat /proc/sys/kernel/random/boot_id)" = {boot['boot_id']}
test "$(cat /proc/sys/kernel/tainted)" = 0
mkdir /tmp/ispass2027-pn-original-update01
gzip -dc /tmp/ispass2027-pn-probe-spi.tar.gz | tar -xf - -C /tmp/ispass2027-pn-original-update01
gzip -dc /tmp/ispass2027-pn-pathv3-fw.bin.gz > /tmp/ispass2027-pn-original-update01/fw_payload.bin
insmod /tmp/ispass2027-pn-original-update01/spi-bitbang.ko
insmod /tmp/ispass2027-pn-original-update01/spi-xilinx.ko
sd_wait=0
while ! test -b /dev/mmcblk0p1; do sd_wait=$((sd_wait+1)); test "$sd_wait" -lt 30; sleep 1; done
platform-sd-update --device /dev/mmcblk0p1 --image /tmp/ispass2027-pn-original-update01/fw_payload.bin
sync'''
    subprocess.run([sys.executable, str(BOARD / 'uart.py'), '--output',
        str(BOARD / 'runs/pn-original-software-write01'), '--command', command,
        '--timeout', '1800'], cwd=ROOT, check=True, timeout=1850)
    session = json.loads((BOARD / 'runs/pn-original-software-write01/session.json').read_text())
    assert session['exit_code'] == 0
    subprocess.run([sys.executable, str(BOARD / 'program_and_boot.py'),
        '--bit', str(BOARD / 'packages/baseline100-20261017/artifacts/ds0.bit'),
        '--expected-sha256', 'e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6',
        '--expected-kernel', package['kernel_release'],
        '--credential-helper', '/home/xvhaocheng/work/iommu-platform/out/r107-execution-gate-20261012/route/login_current_board.py',
        '--output', str(BOARD / 'runs/pn-original-pathv3-load01')],
        cwd=ROOT, check=True, timeout=1300)


if __name__ == '__main__':
    main()
