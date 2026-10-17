#!/usr/bin/env python3
"""After the one recovery boot, upload exact original software once. No SD write."""
import json
from pathlib import Path
import sys
import time

ROOT = Path('/home/xvhaocheng/work-root-a')
BOARD = ROOT / 'paper/ispass2027/board'
sys.path.insert(0, str(BOARD))
from uart import Console, DEFAULT_PORT


def main():
    receipt = BOARD / 'runs/e2-probe-recovery-load01-launch/result.json'
    deadline = time.monotonic() + 1300
    while not receipt.exists():
        if time.monotonic() >= deadline:
            raise TimeoutError('recovery boot did not finish; no UART upload')
        time.sleep(2)
    assert json.loads(receipt.read_text())['returncode'] == 0
    boot = json.loads((BOARD / 'runs/e2-probe-recovery-load01/load.json').read_text())
    assert boot['status'] == 'boot_identity_verified'
    package = json.loads((BOARD / 'packages/e6-pn-common-20261018/transport.json').read_text())
    assert package['kernel_release'] == '6.5.0-rc2-ispass2027-pathv3'
    assert package['selected_linux'] == '45bf76b15e36cf608dff95013d91f8bedb7b1b8e'
    spi = package['current_kernel_spi_archive']
    assert spi['module_metadata']['kernel_release'] == boot['expected_kernel']
    output = BOARD / 'runs/pn-original-upload01'
    console = Console(DEFAULT_PORT, output)
    try:
        command = ('set -e\n'
            'test "$(uname -r)" = 6.5.0-rc2-ispass2027-e2-timer-probe-v1\n'
            f'test "$(cat /proc/sys/kernel/random/boot_id)" = {boot["boot_id"]}\n'
            'test "$(cat /proc/sys/kernel/tainted)" = 0')
        assert console.command(command, 20)['exit_code'] == 0
        for label, source, remote in [
            ('probe-spi', spi, '/tmp/ispass2027-pn-probe-spi.tar.gz'),
            ('pathv3-payload', package['payload_gzip'], '/tmp/ispass2027-pn-pathv3-fw.bin.gz'),
        ]:
            result = console.put_file(Path(source['path']), remote,
                                      timeout=2400 if label == 'pathv3-payload' else 120)
            assert result['byte_count_acknowledged'] and result['bytes'] == source['bytes']
            assert result['sha256'] == source['sha256']
            (output / (label + '-upload-result.json')).write_text(json.dumps(result, indent=2) + '\n')
    finally:
        console.close()


if __name__ == '__main__':
    main()
