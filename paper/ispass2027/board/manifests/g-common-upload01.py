#!/usr/bin/env python3
"""After complete E4, upload current-kernel recovery modules and new G payload.

Only new /tmp files are written on the board. No SD update, reboot or trial.
"""
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path('/home/xvhaocheng/work-root-a')
PAPER = ROOT / 'paper/ispass2027'
BOARD = PAPER / 'board'
sys.path.insert(0, str(BOARD))
from uart import Console, DEFAULT_PORT


def main():
    assert json.loads((BOARD / 'runs/e4-e5-common-v2-collect02-launch/result.json').read_text())['returncode'] == 0
    done = json.loads((BOARD / 'runs/e4-pagesize-20261018-01/manifest.json').read_text())
    assert done['status'] == 'passed' and done['recorded_cases'] == 20
    original = PAPER / 'exp/E6-mitigation/software-source/build01/build-manifest.json'
    assert hashlib.sha256(original.read_bytes()).hexdigest() == '4bee1eadd8d8102e652129833f171ba869e5edd8a1a039973d13559a3a0b2f6c'
    software = json.loads(original.read_text())
    transport = json.loads((PAPER / 'exp/E6-mitigation/g-software-transport.json').read_text())
    current = json.loads((PAPER / 'exp/E3-workloads/common-v2-software-transport.json').read_text())
    assert software['status'] == 'passed' and software['artifacts']['fw_payload.bin'] == transport['payload']['sha256']
    assert transport['round_trip_host_verified'] is True
    assert software['payload_bytes'] <= software['payload_limit_bytes'] == 33554432
    out = BOARD / 'runs/g-common-upload01'
    console = Console(DEFAULT_PORT, out)
    try:
        check = console.command('test "$(uname -r)" = 6.5.0-rc2-ispass2027-full-e3-v1 && '
            'test "$(cat /proc/sys/kernel/random/boot_id)" = bf3063bb-b908-4333-bcb9-54a08511210e && '
            'test "$(cat /proc/sys/kernel/tainted)" = 0', 20)
        assert check['exit_code'] == 0, 'current boot changed; no upload'
        modules = console.put_file(Path(current['spi_archive']['path']),
            '/tmp/ispass2027-full-e3-spi.tar.gz', timeout=120)
        assert modules['byte_count_acknowledged'] and modules['bytes'] == current['spi_archive']['bytes']
        (out / 'current-kernel-spi-upload-result.json').write_text(json.dumps(modules, indent=2) + '\n')
        payload = console.put_file(Path(transport['gzip']['path']),
            '/tmp/ispass2027-g-fw.bin.gz', timeout=2400)
        assert payload['byte_count_acknowledged'] and payload['bytes'] == transport['gzip']['bytes']
        assert payload['sha256'] == transport['gzip']['sha256']
        (out / 'g-payload-upload-result.json').write_text(json.dumps(payload, indent=2) + '\n')
    finally:
        console.close()


if __name__ == '__main__':
    main()
