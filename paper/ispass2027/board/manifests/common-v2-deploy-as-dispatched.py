#!/usr/bin/env python3
"""One finite Root-owned upload -> SD update -> boot sequence; no retries."""
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
BOARD = ROOT / 'paper/ispass2027/board'
E3 = ROOT / 'paper/ispass2027/exp/E3-workloads'


def main():
    transport = json.loads((E3 / 'common-v2-software-transport.json').read_text())
    software = json.loads((E3 / 'common-v2-software-build-manifest.json').read_text())
    assert software['status'] == 'passed'
    assert software['selected_linux'] == '65b427946152e3aca27babab0a437821729805d4'
    assert software['selected_platform'] == 'ec99648e3c9a1e48c1e9721e3662bb632eb439bd'
    assert software['kernel_release'] == transport['kernel_release'] == '6.5.0-rc2-ispass2027-full-e3-v1'
    assert software['artifacts']['fw_payload.bin'] == transport['payload']['sha256']
    result_path = BOARD / 'runs/common-v2-payload-upload01-launch/result.json'
    deadline = time.monotonic() + 2500
    while not result_path.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('Upload did not finish; no SD write or boot attempted')
        time.sleep(2)
    assert json.loads(result_path.read_text())['returncode'] == 0, 'Upload failed; stop'
    upload = json.loads((BOARD / 'runs/common-v2-payload-upload01/upload-result.json').read_text())
    assert upload['byte_count_acknowledged'] and upload['bytes'] == transport['gzip']['bytes']
    assert upload['sha256'] == transport['gzip']['sha256']
    command = '''set -e
test "$(uname -r)" = 6.5.0-rc2-ispass2027-p0-multi-v1
test "$(cat /proc/sys/kernel/random/boot_id)" = 2f31b9f4-cf88-4061-b33c-3cf5a5592795
mkdir -p /tmp/ispass2027-common-v2-update
gzip -dc /tmp/ispass2027-current-p0-multi-spi.tar.gz | tar -xf - -C /tmp/ispass2027-common-v2-update
gzip -dc /tmp/ispass2027-full-e3-fw.bin.gz > /tmp/ispass2027-common-v2-update/fw_payload.bin
insmod /tmp/ispass2027-common-v2-update/spi-bitbang.ko
insmod /tmp/ispass2027-common-v2-update/spi-xilinx.ko
sd_wait=0
while ! test -b /dev/mmcblk0p1; do sd_wait=$((sd_wait+1)); test "$sd_wait" -lt 30; sleep 1; done
platform-sd-update --device /dev/mmcblk0p1 --image /tmp/ispass2027-common-v2-update/fw_payload.bin
sync'''
    subprocess.run([sys.executable, str(BOARD / 'uart.py'), '--output',
        str(BOARD / 'runs/common-v2-software-write01'), '--command', command,
        '--timeout', '1800'], cwd=ROOT, check=True, timeout=1850)
    # No collector is started here. Root must accept the new tools and boot first.
    subprocess.run([sys.executable, str(BOARD / 'program_and_boot.py'), '--bit',
        str(BOARD / 'packages/baseline100-20261017/artifacts/ds0.bit'),
        '--expected-sha256', 'e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6',
        '--expected-kernel', software['kernel_release'], '--credential-helper',
        '/home/xvhaocheng/work/iommu-platform/out/r107-execution-gate-20261012/route/login_current_board.py',
        '--output', str(BOARD / 'runs/e2-common-v2-ds0-load01')],
        cwd=ROOT, check=True, timeout=1800)


if __name__ == '__main__':
    main()
