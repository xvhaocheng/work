#!/usr/bin/env python3
"""Upload probe software after G40, using the current G kernel's SPI modules.

No SD write, reboot or diagnostic is performed by this entry point.
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
    assert json.loads((BOARD / 'runs/g-collect01-launch/result.json').read_text())['returncode'] == 0
    run = json.loads((BOARD / 'runs/g-ds0-20261018-01/manifest.json').read_text())
    assert run['status'] == 'passed' and run['recorded_cases'] == 40
    target_dir = PAPER / 'exp/E2-device-scaling/irq-diagnosis'
    original = target_dir / 'builder-v1/final-build-manifest.json'
    assert hashlib.sha256(original.read_bytes()).hexdigest() == 'b12177ae47ee526b1bea22a08546f78c09f7bf6ae689420c94341285c4228470'
    assert json.loads((PAPER / 'exp/E2-device-scaling/root-validation/timer-image01.json').read_text())['status'] == 'passed'
    target = json.loads((target_dir / 'run-v1/transfer-package.json').read_text())
    current = json.loads((PAPER / 'exp/E6-mitigation/g-software-transport.json').read_text())
    assert target['payload_source']['sha256_from_passed_manifest'] == json.loads(original.read_text())['artifacts']['fw_payload.bin']
    assert current['kernel_release'] == run['kernel_release'] == '6.5.0-rc2-ispass2027-g-v1'
    out = BOARD / 'runs/e2-probe-upload01'
    console = Console(DEFAULT_PORT, out)
    try:
        command = ('test "$(uname -r)" = 6.5.0-rc2-ispass2027-g-v1 && '
            f'test "$(cat /proc/sys/kernel/random/boot_id)" = {run["boot_id"]} && '
            'test "$(cat /proc/sys/kernel/tainted)" = 0')
        assert console.command(command, 20)['exit_code'] == 0, 'current G boot changed; no upload'
        modules = console.put_file(Path(current['spi_archive']['path']),
            '/tmp/ispass2027-g-spi.tar.gz', timeout=120)
        assert modules['byte_count_acknowledged'] and modules['bytes'] == current['spi_archive']['bytes']
        assert modules['sha256'] == current['spi_archive']['sha256']
        (out / 'current-kernel-spi-upload-result.json').write_text(json.dumps(modules, indent=2) + '\n')
        payload = console.put_file(Path(target['payload_gzip']['path']),
            '/tmp/ispass2027-e2-probe-fw.bin.gz', timeout=2400)
        assert payload['byte_count_acknowledged'] and payload['bytes'] == target['payload_gzip']['bytes']
        assert payload['sha256'] == target['payload_gzip']['sha256']
        (out / 'probe-payload-upload-result.json').write_text(json.dumps(payload, indent=2) + '\n')
    finally:
        console.close()


if __name__ == '__main__':
    main()
