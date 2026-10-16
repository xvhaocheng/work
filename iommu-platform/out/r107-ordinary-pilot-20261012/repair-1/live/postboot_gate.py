#!/usr/bin/env python3
"""Accept only this new kernel's framed, read-only boot identity evidence.

This file does not open UART, write SD, reset hardware, or issue an ioctl.
The source-frozen Image/notes/config/DTB are checked independently of uname.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parent.parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def require(ok, reason):
    if not ok:
        raise ValueError(reason)


def accept_identity(record, manifest, notes, old_boot):
    require(record.get('UART_opened') is True and
            record.get('board_command_exit_code') == 0, 'no successful actual UART identity command')
    raw = record.get('board_stdout')
    require(isinstance(raw, str), 'original framed identity stdout missing')
    fields = {}
    for line in raw.replace('\r', '').splitlines():
        match = re.fullmatch(r'(R111_[A-Z_]+)=(.*)', line)
        if match:
            require(match[1] not in fields, 'duplicate identity field')
            fields[match[1]] = match[2]
    require(fields.get('R111_UNAME') == manifest['kernel_release'], 'new release not running')
    boot = fields.get('R111_BOOT_ID', '')
    require(re.fullmatch(r'[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}', boot),
            'actual new boot UUID missing')
    require(boot != old_boot, 'same old boot; new kernel startup unverified')
    require(fields.get('R111_TAINT') == '0', 'tainted or unreadable new kernel')
    require(fields.get('R111_ARCH') == 'riscv64', 'board architecture unverified')
    require(fields.get('R111_IDMA_CDEV_COUNT') == '4', 'four idma character devices unverified')
    require(fields.get('R111_CPU_ONLINE') == '0', 'single online hart precondition unverified')
    artifacts = {Path(x['path']).name: x for x in manifest['artifacts']}
    for key, expected in [('R111_CONFIG_SHA', artifacts['.config']['sha256']),
                          ('R111_DTB_SHA', artifacts['platform.dtb']['sha256']),
                          ('R111_NOTES_SHA', notes['notes_sha256'])]:
        value = fields.get(key, '').split()
        require(value and value[0] == expected, key + ' does not match the chosen build')
    return {'accepted': True, 'kind': 'new chosen ordinary kernel actual boot identity only',
            'boot_ID': boot, 'kernel_release': fields['R111_UNAME'], 'fields': fields,
            'raw_counters_calibrated': False, 'ordinary_DMA_executed': False,
            'limitations': ['This gate binds the new boot to its chosen source/config/notes/DTB. '
                            'It does not replace the accepted bit identity, runtime CAPS or full RUN validation.']}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('record', type=Path)
    p.add_argument('--result', type=Path, required=True)
    a = p.parse_args()
    manifest = json.loads((ROOT / 'build/artifact-manifest.json').read_text())
    notes = json.loads((ROOT / 'build/root-artifact-acceptance.json').read_text())
    old_boot = '67ad5da4-1d72-4ffb-b8ff-033c6393c4ef'
    try:
        r = accept_identity(json.loads(a.record.read_text()), manifest, notes, old_boot)
        r['original_record_sha256'] = sha(a.record)
        r['artifact_manifest_sha256'] = sha(ROOT / 'build/artifact-manifest.json')
        r['notes_acceptance_sha256'] = sha(ROOT / 'build/root-artifact-acceptance.json')
        code = 0
    except (ValueError, KeyError, TypeError, OSError) as exc:
        r = {'accepted': False, 'reason': str(exc), 'ordinary_DMA_executed': False}
        code = 1
    a.result.write_text(json.dumps(r, indent=2) + '\n')
    print(json.dumps(r, indent=2))
    return code


if __name__ == '__main__':
    raise SystemExit(main())
