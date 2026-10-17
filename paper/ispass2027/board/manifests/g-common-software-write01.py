#!/usr/bin/env python3
"""Wait for one acknowledged upload, then update SD once. No BIT load or run."""
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
PAPER = ROOT / 'paper/ispass2027'
BOARD = PAPER / 'board'


def main():
    receipt = BOARD / 'runs/g-common-upload01-launch/result.json'
    deadline = time.monotonic() + 3000
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('G upload did not complete; SD not touched')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0, 'G upload failed'
    transport = json.loads((PAPER / 'exp/E6-mitigation/g-software-transport.json').read_text())
    upload = json.loads((BOARD / 'runs/g-common-upload01/g-payload-upload-result.json').read_text())
    assert upload['byte_count_acknowledged'] and upload['bytes'] == transport['gzip']['bytes']
    assert upload['sha256'] == transport['gzip']['sha256']
    assert json.loads((PAPER / 'exp/E3-workloads/root-validation/g-software-profile01.json').read_text())['status'] == 'passed'
    assert json.loads((BOARD / 'runs/e4-pagesize-20261018-01/manifest.json').read_text())['status'] == 'passed'
    command = '''set -e
test "$(uname -r)" = 6.5.0-rc2-ispass2027-full-e3-v1
test "$(cat /proc/sys/kernel/random/boot_id)" = bf3063bb-b908-4333-bcb9-54a08511210e
test "$(cat /proc/sys/kernel/tainted)" = 0
mkdir /tmp/ispass2027-g-common-update
gzip -dc /tmp/ispass2027-full-e3-spi.tar.gz | tar -xf - -C /tmp/ispass2027-g-common-update
gzip -dc /tmp/ispass2027-g-fw.bin.gz > /tmp/ispass2027-g-common-update/fw_payload.bin
insmod /tmp/ispass2027-g-common-update/spi-bitbang.ko
insmod /tmp/ispass2027-g-common-update/spi-xilinx.ko
sd_wait=0
while ! test -b /dev/mmcblk0p1; do sd_wait=$((sd_wait+1)); test "$sd_wait" -lt 30; sleep 1; done
platform-sd-update --device /dev/mmcblk0p1 --image /tmp/ispass2027-g-common-update/fw_payload.bin
sync'''
    subprocess.run([sys.executable, str(BOARD / 'uart.py'), '--output',
        str(BOARD / 'runs/g-common-software-write01'), '--command', command,
        '--timeout', '1800'], cwd=ROOT, check=True, timeout=1850)


if __name__ == '__main__':
    main()
