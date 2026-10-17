#!/usr/bin/env python3
"""One qualified PN boot, tool install and original-command S15; no retries."""
import json
from pathlib import Path
import subprocess
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
BOARD = ROOT / 'paper/ispass2027/board'
EXP = ROOT / 'paper/ispass2027/exp/E6-pn'
sys.path.insert(0, str(BOARD))
from uart import Console, DEFAULT_PORT


def run(script, *args):
    subprocess.run([sys.executable, str(script), *map(str, args)], cwd=ROOT, check=True)


def main():
    receipt = BOARD / 'runs/e6-pn-ds0-load01-launch/result.json'
    deadline = time.monotonic() + 1300
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('PN load did not finish; no UART actions')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0
    load = BOARD / 'runs/e6-pn-ds0-load01/load.json'
    boot = json.loads(load.read_text())
    assert boot['status'] == 'boot_identity_verified'
    assert boot['bit_sha256'] == '3755410594c7b28a8843abd78169aaa782f2de501e516c478ace15989e84f1e7'
    assert boot['kernel_release'] == '6.5.0-rc2-ispass2027-pathv3'
    output = BOARD / 'runs/e6-pn-tools-install01'
    console = Console(DEFAULT_PORT, output)
    try:
        pre = ('set -e\n'
               'test "$(uname -r)" = 6.5.0-rc2-ispass2027-pathv3\n'
               f'test "$(cat /proc/sys/kernel/random/boot_id)" = {boot["boot_id"]}\n'
               'test "$(cat /proc/sys/kernel/tainted)" = 0')
        assert console.command(pre, 20)['exit_code'] == 0
        uploaded = console.put_file(BOARD / 'packages/e6-pn-tools-20261018/artifacts/pn-tools.tar.gz',
                                   '/tmp/ispass2027-pn-tools.tar.gz', timeout=240)
        assert uploaded['byte_count_acknowledged'] and uploaded['bytes'] == 542652
        assert uploaded['sha256'] == 'd2707af0ac137732c31553f59b27c33c9a6ff1eafca8f50ec6668615a860e29c'
        install = ('set -e\nmkdir /tmp/ispass2027-pn-tools\n'
                   'cd /tmp/ispass2027-pn-tools\n'
                   'gzip -dc /tmp/ispass2027-pn-tools.tar.gz | tar -xf -\n'
                   'cp pn-control pn-config /usr/bin/\n'
                   'chmod 755 /usr/bin/pn-control /usr/bin/pn-config')
        assert console.command(install, 60)['exit_code'] == 0
    finally:
        console.close()
    common = ['--source-root', ROOT, '--source-manifest', EXP / 'build/source-manifest.json',
              '--fpga-manifest', EXP / 'build/completed-ds0-01/build-manifest.json',
              '--expected-cva6', '109b48fdde862b668d072bb20ca13c27dbade1fe',
              '--expected-iommu', '86c2cee6b82fc0588e233000926afe3ab9a2fb8f']
    collected = BOARD / 'runs/e6-pn-s15-ds0-01'
    run(EXP / 'tools/collect_s15.py', *common, '--output', collected,
        '--load-manifest', load, '--software-manifest', BOARD / 'manifests/software-build.json')
    run(EXP / 'tools/s15_gate.py', *common, '--run', collected,
        '--output', EXP / 's15/ds0-first01')


if __name__ == '__main__':
    main()
