#!/usr/bin/env python3
"""Write the compiled 20261002 payload to the registered card; no readback."""
import os
import argparse
import subprocess
import tempfile
from pathlib import Path
from sd_card import ROOT, BOOT_BYTES, identity, write_and_verify
from fpga import save_json, utc

disk = '/dev/disk5'
parser = argparse.ArgumentParser()
parser.add_argument('--release', choices=['next-experiment-20261002', 'research-followup-20261002', 'research-fullstats-20261002', 'research-two-bit-20261003', 'software-update-20261004'], default='next-experiment-20261002')
args = parser.parse_args()
payload_member = 'delivery/artifacts/fw_payload.bin' if args.release == 'research-two-bit-20261003' else 'delivery/fw_payload.bin'
payload_path = ROOT / 'artifacts/software' / args.release / payload_member
payload = payload_path.read_bytes()
if not 0 < len(payload) <= BOOT_BYTES:
    raise RuntimeError('Payload does not fit the boot partition')
info = identity(disk)
out = Path(tempfile.mkdtemp(prefix=args.release + '-', dir=ROOT / 'results/sd'))
record = dict(started_utc=utc(), payload=str(payload_path), payload_bytes=len(payload),
              disk=disk, identity=info, status='not_written', readback_verified=False)
save_json(out / 'status.json', record)
try:
    subprocess.run(['diskutil', 'unmountDisk', disk], check=True)
    info = identity(disk)
    if any(info[x].get('MountPoint') for x in ('boot', 'root')):
        raise RuntimeError('Card is still mounted')
    fd = os.open('/dev/rdisk5s1', os.O_WRONLY)
    try:
        record['status'] = 'writing'
        save_json(out / 'status.json', record)
        write_and_verify(fd, payload + bytes(BOOT_BYTES-len(payload)), verify=False)
    finally:
        os.close(fd)
    subprocess.run(['/bin/sync'], check=True)
    record['status'] = 'written_not_readback_verified'
    subprocess.run(['diskutil', 'eject', disk], check=True)
    record['ejected'] = True
finally:
    record['finished_utc'] = utc()
    save_json(out / 'status.json', record)
    print(out, record['status'], flush=True)
