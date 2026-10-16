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
JOBS = 12
MAX_PAYLOAD = 33554432  # 32 MiB OpenSBI payload window

KERNEL_IMAGE = ROOT / 'out' / 'linux' / 'arch' / 'riscv' / 'boot' / 'Image'
PLATFORM_DTB = ROOT / 'out' / 'linux' / 'platform.dtb'
ROOTFS = ROOT / 'out' / 'rootfs.cpio'
FW_PAYLOAD = ROOT / 'out' / 'opensbi' / 'platform' / 'fpga' / 'ariane' / 'firmware' / 'fw_payload.bin'
SD_TOOL = ROOT / 'out' / 'bench' / 'platform-sd-update'
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
    'firmware': [
        'opensbi/platform/fpga/ariane/platform.c',
        'opensbi/lib/utils/reset/*.c',
    ],
    'rootscripts': ['scripts/rootfs.py'],
    'tool': ['bench/platform_sd_update.c'],
}

REQUIRED_CONFIG_Y = (
    'CONFIG_RISCV',
    'CONFIG_BLK_DEV_INITRD',
)


def utc_now():
    return datetime.now(timezone.utc).isoformat()


def write_state(state):
    BASE.mkdir(parents=True, exist_ok=True)
    tmp = STATE_PATH.with_suffix(STATE_PATH.suffix + '.tmp')
    tmp.write_text(json.dumps(state, indent=2) + '\n')
    tmp.replace(STATE_PATH)


def load_platform():
    spec = importlib.util.spec_from_file_location('cpn_platform', ROOT / 'scripts' / 'platform.py')
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load scripts/platform.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def ensure_previous(sha):
    manifest_path = PREVIOUS / 'previous-manifest.json'
    if PREVIOUS.exists():
        if manifest_path.is_file():
            return json.loads(manifest_path.read_text())
        return {'existing': True}
    PREVIOUS.mkdir(parents=True, exist_ok=False)
    manifest = {}
    for name, source in PREVIOUS_SOURCES.items():
        entry = {'source': str(source), 'copied': False}
        if source.is_file() and source.stat().st_size:
            destination = PREVIOUS / name
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


def check_config(values):
    for key in REQUIRED_CONFIG_Y:
        if values.get(key) != 'y':
            raise RuntimeError('critical kernel config is not y: '
                               + key + '=' + repr(values.get(key)))
    source = values.get('CONFIG_INITRAMFS_SOURCE', '')
    if not source:
        raise RuntimeError('CONFIG_INITRAMFS_SOURCE is empty; the initramfs would not be embedded')
    return {
        'required_y': {key: values.get(key) for key in REQUIRED_CONFIG_Y},
        'initramfs_source': source,
    }


def check_payload():
    if not FW_PAYLOAD.is_file() or FW_PAYLOAD.stat().st_size == 0:
        raise RuntimeError('missing firmware payload: ' + str(FW_PAYLOAD))
    size = FW_PAYLOAD.stat().st_size
    if size > MAX_PAYLOAD:
        raise RuntimeError('fw_payload.bin is ' + str(size) + ' bytes, over ' + str(MAX_PAYLOAD))
    return size


def deliver(sha):
    DELIVERY.mkdir(parents=True, exist_ok=True)
    manifest = {}
    for name, source in DELIVERY_SOURCES.items():
        if not source.is_file() or source.stat().st_size == 0:
            raise RuntimeError('delivery artifact missing or empty: ' + str(source))
        destination = DELIVERY / name
        shutil.copy2(source, destination)
        manifest[name] = {'source': str(source), 'size': destination.stat().st_size,
                          'sha256': sha(destination)}
    return manifest


def main():
    BASE.mkdir(parents=True, exist_ok=True)
    state = {
        'started_at': utc_now(),
        'source_mode': 'authorized_working_tree',
        'check_bypassed': 'authorized_software_working_tree',
        'fpga': 'not_run',
        'localversion': LOCAL_VERSION,
        'secrets_excluded': True,
        'status': 'running',
    }
    write_state(state)
    try:
        platform = load_platform()
        platform.ENV['LOCALVERSION'] = LOCAL_VERSION
        state['platform_commit'] = platform.capture(['git', 'rev-parse', 'HEAD'])
        state['platform_dirty'] = bool(platform.capture(['git', 'status', '--porcelain']))

        state['previous'] = ensure_previous(platform.sha)
        state['inputs'] = capture_inputs(platform.sha)

        platform.kernel(JOBS)
        platform.firmware(JOBS)

        state['config'] = check_config(read_config())
        state['fw_payload_size'] = check_payload()
        state['artifacts'] = deliver(platform.sha)

        state['status'] = 'complete'
        state['exit_code'] = 0
    except BaseException as exc:
        state['status'] = 'failed'
        state['exit_code'] = int(getattr(exc, 'returncode', None) or 1)
        state['error'] = type(exc).__name__ + ': ' + str(exc)
        raise
    finally:
        state['finished_at'] = utc_now()
        write_state(state)


if __name__ == '__main__':
    main()
