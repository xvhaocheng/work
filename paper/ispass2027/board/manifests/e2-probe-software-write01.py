#!/usr/bin/env python3
"""Use one acknowledged upload to update SD once; no load or experiment."""
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path('/home/xvhaocheng/work-root-a')
PAPER = ROOT / 'paper/ispass2027'
BOARD = PAPER / 'board'


def main():
    assert json.loads((BOARD / 'runs/e2-probe-upload01-launch/result.json').read_text())['returncode'] == 0
    upload = json.loads((BOARD / 'runs/e2-probe-upload01/probe-payload-upload-result.json').read_text())
    transport = json.loads((PAPER / 'exp/E2-device-scaling/irq-diagnosis/run-v1/transfer-package.json').read_text())
    assert upload['byte_count_acknowledged'] and upload['bytes'] == transport['payload_gzip']['bytes']
    assert upload['sha256'] == transport['payload_gzip']['sha256']
    for name in ('timer-image01.json', 'timer-run-host01.json'):
        assert json.loads((PAPER / 'exp/E2-device-scaling/root-validation' / name).read_text())['status'] == 'passed'
    command = '''set -e
test "$(uname -r)" = 6.5.0-rc2-ispass2027-g-v1
test "$(cat /proc/sys/kernel/random/boot_id)" = 79a6975d-877d-4a9d-8e19-08834275be7a
test "$(cat /proc/sys/kernel/tainted)" = 0
mkdir /tmp/ispass2027-e2-probe-update01
gzip -dc /tmp/ispass2027-g-spi.tar.gz | tar -xf - -C /tmp/ispass2027-e2-probe-update01
gzip -dc /tmp/ispass2027-e2-probe-fw.bin.gz > /tmp/ispass2027-e2-probe-update01/fw_payload.bin
insmod /tmp/ispass2027-e2-probe-update01/spi-bitbang.ko
insmod /tmp/ispass2027-e2-probe-update01/spi-xilinx.ko
sd_wait=0
while ! test -b /dev/mmcblk0p1; do sd_wait=$((sd_wait+1)); test "$sd_wait" -lt 30; sleep 1; done
platform-sd-update --device /dev/mmcblk0p1 --image /tmp/ispass2027-e2-probe-update01/fw_payload.bin
sync'''
    subprocess.run([sys.executable, str(BOARD / 'uart.py'), '--output',
        str(BOARD / 'runs/e2-probe-software-write01'), '--command', command,
        '--timeout', '1800'], cwd=ROOT, check=True, timeout=1850)


if __name__ == '__main__':
    main()
