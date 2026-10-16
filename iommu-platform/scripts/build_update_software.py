#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Build the cpn2-net1 software update delivery (authorized working tree).

Only the software side is built here.  This module never runs the pinned clean
check() and never runs fpga()/Vivado: the component trees are intentionally
dirty (authorized working tree) and RTL/fpga-src must not change in this batch.

The caller owns stdout/stderr; this module prints nothing and redirects
nothing.  The state file in out/software-update-20261004 is running -> complete
or failed, and finished_at is always written from a finally block.  On failure
the original exception (and its exit code, when present) is preserved.
"""
from __future__ import annotations

import argparse
import importlib.util
import json
import shutil
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

BASE = ROOT / 'out' / 'software-update-20261004'
DELIVERY = BASE / 'delivery'
PREVIOUS = BASE / 'previous'
STATE_PATH = BASE / 'software-update-state.json'

LOCAL_VERSION = '-cpn2-net1'
DEFAULT_OUTPUT_DIR = 'out/software-update-20261004'
DEFAULT_LOCAL_VERSION = '-cpn2-net1'
JOBS = 12
MAX_PAYLOAD = 33554432  # 32 MiB OpenSBI payload window

KERNEL_IMAGE = ROOT / 'out' / 'linux' / 'arch' / 'riscv' / 'boot' / 'Image'
PLATFORM_DTB = ROOT / 'out' / 'linux' / 'platform.dtb'
ROOTFS = ROOT / 'out' / 'rootfs.cpio'
FW_PAYLOAD = ROOT / 'out' / 'opensbi' / 'platform' / 'fpga' / 'ariane' / 'firmware' / 'fw_payload.bin'
SD_TOOL = ROOT / 'out' / 'bench' / 'platform-sd-update'
KERNEL_CONFIG = ROOT / 'out' / 'linux' / '.config'
KERNEL_RELEASE = ROOT / 'out' / 'linux' / 'include' / 'config' / 'kernel.release'
MAC_UPDATE_REBOOT = ROOT / 'scripts' / 'mac_update_reboot.py'
LEGACY_STATE = ROOT / 'out' / 'paper-route-review-20261004' / 'software-build-state.json'

PREVIOUS_SOURCES = {
    'Image': KERNEL_IMAGE,
    'platform.dtb': PLATFORM_DTB,
    'rootfs.cpio': ROOTFS,
    'fw_payload.bin': FW_PAYLOAD,
    'software-build-state.json': LEGACY_STATE,
}

DELIVERY_SOURCES = {
    'Image': KERNEL_IMAGE,
    'platform.dtb': PLATFORM_DTB,
    'rootfs.cpio': ROOTFS,
    'fw_payload.bin': FW_PAYLOAD,
    'platform-sd-update': SD_TOOL,
    'linux.config': KERNEL_CONFIG,
    'kernel-release': KERNEL_RELEASE,
    'mac_update_reboot.py': MAC_UPDATE_REBOOT,
    'board-network.interfaces': ROOT / 'configs' / 'board-network.interfaces',
    'board-dhcpcd.conf': ROOT / 'configs' / 'board-dhcpcd.conf',
}

INPUT_PATTERNS = {
    'config': ['configs/linux_defconfig'],
    'dts': ['configs/platform.dts'],
    'driver': [
        'linux/drivers/misc/idma.c',
        'linux/drivers/misc/idma_research.h',
        'linux/include/uapi/linux/idma.h',
        'linux/drivers/net/ethernet/lowrisc/**/*.c',
        'linux/drivers/net/ethernet/lowrisc/**/*.h',
    ],
    'lowrisc_kconfig': [
        'linux/drivers/net/ethernet/lowrisc/Kconfig',
        'linux/drivers/net/ethernet/lowrisc/Makefile',
    ],
    'ethernet_kconfig': [
        'linux/drivers/net/ethernet/Kconfig',
        'linux/drivers/net/ethernet/Makefile',
    ],
    'firmware': [
        'opensbi/platform/fpga/ariane/platform.c',
        'opensbi/lib/utils/reset/*.c',
    ],
    'rootscripts': ['scripts/rootfs.py'],
    'network': [
        'configs/board-network.interfaces',
        'configs/board-dhcpcd.conf',
    ],
    'board_helpers': [
        'scripts/platform.py',
        'scripts/build_update_software.py',
        'tests/board-net-up.sh',
        'tests/board-fetch-update.sh',
    ],
    'tool': ['bench/platform_sd_update.c'],
}

REQUIRED_CONFIG_Y = (
    'CONFIG_RISCV',
    'CONFIG_BLK_DEV_INITRD',
    'CONFIG_SPI_XILINX',
    'CONFIG_MMC',
    'CONFIG_MMC_BLOCK',
    'CONFIG_MMC_SPI',
    'CONFIG_LOWRISC_ETH',
    'CONFIG_PHYLIB',
    'CONFIG_MDIO_BITBANG',
    'CONFIG_REALTEK_PHY',
)


def utc_now():
    return datetime.now(timezone.utc).isoformat()


def write_state(state, base=BASE):
    base.mkdir(parents=True, exist_ok=True)
    state_path = base / 'software-update-state.json'
    tmp = state_path.with_suffix(state_path.suffix + '.tmp')
    tmp.write_text(json.dumps(state, indent=2) + '\n')
    tmp.replace(state_path)


def load_platform():
    spec = importlib.util.spec_from_file_location('cpn_platform', ROOT / 'scripts' / 'platform.py')
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load scripts/platform.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def ensure_previous(sha, base=BASE):
    previous = base / 'previous'
    manifest_path = previous / 'previous-manifest.json'
    if previous.exists():
        if manifest_path.is_file():
            return json.loads(manifest_path.read_text())
        return {'existing': True}
    previous.mkdir(parents=True, exist_ok=False)
    manifest = {}
    for name, source in PREVIOUS_SOURCES.items():
        entry = {'source': str(source), 'copied': False}
        if source.is_file() and source.stat().st_size:
            destination = previous / name
            shutil.copy2(source, destination)
            entry['copied'] = True
            entry['size'] = destination.stat().st_size
            entry['sha256'] = sha(destination)
        else:
            entry['missing'] = True
        manifest[name] = entry
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
    return manifest


def capture_inputs(sha):
    inputs = {}
    for group, patterns in INPUT_PATTERNS.items():
        entries = {}
        for pattern in patterns:
            for path in sorted(ROOT.glob(pattern)):
                if not path.is_file():
                    continue
                relative = str(path.relative_to(ROOT))
                entries[relative] = {'size': path.stat().st_size, 'sha256': sha(path)}
        inputs[group] = entries
    return inputs


def read_config():
    path = ROOT / 'out' / 'linux' / '.config'
    if not path.is_file():
        raise RuntimeError('missing kernel config: ' + str(path))
    values = {}
    for raw in path.read_text(errors='replace').splitlines():
        line = raw.strip()
        if not line or line.startswith('#') or '=' not in line:
            continue
        key, value = line.split('=', 1)
        values[key.strip()] = value.strip()
    return values


def read_kernel_release():
    path = KERNEL_RELEASE
    if not path.is_file():
        raise RuntimeError('missing kernel release file: ' + str(path))
    text = path.read_text(errors='replace').strip()
    if not text:
        raise RuntimeError('kernel release file is empty: ' + str(path))
    return text


def check_config(values):
    for key in REQUIRED_CONFIG_Y:
        if values.get(key) != 'y':
            raise RuntimeError('critical kernel config is not y: '
                               + key + '=' + repr(values.get(key)))
    source = values.get('CONFIG_INITRAMFS_SOURCE', '')
    expected = '"' + str(ROOT / 'out' / 'rootfs.cpio') + '"'
    if source != expected:
        raise RuntimeError('CONFIG_INITRAMFS_SOURCE is ' + repr(source)
                           + ', expected ' + repr(expected))
    return {
        'required_y': {key: values.get(key) for key in REQUIRED_CONFIG_Y},
        'initramfs_source': source,
        'initramfs_source_expected': expected,
    }


def check_payload():
    if not FW_PAYLOAD.is_file() or FW_PAYLOAD.stat().st_size == 0:
        raise RuntimeError('missing firmware payload: ' + str(FW_PAYLOAD))
    size = FW_PAYLOAD.stat().st_size
    if size > MAX_PAYLOAD:
        raise RuntimeError('fw_payload.bin is ' + str(size) + ' bytes, over ' + str(MAX_PAYLOAD))
    return size


def deliver(sha, base=BASE):
    delivery = base / 'delivery'
    delivery.mkdir(parents=True, exist_ok=True)
    manifest = {}
    for name, source in DELIVERY_SOURCES.items():
        if not source.is_file() or source.stat().st_size == 0:
            raise RuntimeError('delivery artifact missing or empty: ' + str(source))
        destination = delivery / name
        shutil.copy2(source, destination)
        manifest[name] = {'source': str(source), 'size': destination.stat().st_size,
                          'sha256': sha(destination)}
    return manifest


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output-dir', default=DEFAULT_OUTPUT_DIR,
                        help='delivery base directory (default: %(default)s)')
    parser.add_argument('--local-version', default=DEFAULT_LOCAL_VERSION,
                        help='kernel LOCALVERSION (default: %(default)s)')
    args = parser.parse_args(argv)
    base = Path(args.output_dir)
    if not base.is_absolute():
        base = ROOT / base
    local_version = args.local_version
    base.mkdir(parents=True, exist_ok=True)
    state = {
        'started_at': utc_now(),
        'source_mode': 'authorized_working_tree',
        'check_bypassed': 'authorized_software_working_tree',
        'fpga': 'not_run',
        'localversion': local_version,
        'base': str(base),
        'secrets_excluded': True,
        'status': 'running',
    }
    write_state(state, base)
    try:
        platform = load_platform()
        platform.ENV['LOCALVERSION'] = local_version
        state['platform_commit'] = platform.capture(['git', 'rev-parse', 'HEAD'])
        state['platform_dirty'] = bool(platform.capture(['git', 'status', '--porcelain']))

        state['previous'] = ensure_previous(platform.sha, base)
        state['inputs'] = capture_inputs(platform.sha)

        platform.kernel(JOBS)
        platform.firmware(JOBS)

        state['config'] = check_config(read_config())
        state['kernel_release'] = read_kernel_release()
        state['fw_payload_size'] = check_payload()
        state['artifacts'] = deliver(platform.sha, base)

        state['status'] = 'complete'
        state['exit_code'] = 0
    except BaseException as exc:
        state['status'] = 'failed'
        state['exit_code'] = int(getattr(exc, 'returncode', None) or 1)
        state['error'] = type(exc).__name__ + ': ' + str(exc)
        raise
    finally:
        state['finished_at'] = utc_now()
        write_state(state, base)


if __name__ == '__main__':
    main()
