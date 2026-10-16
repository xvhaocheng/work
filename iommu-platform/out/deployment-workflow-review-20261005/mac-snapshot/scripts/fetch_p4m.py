#!/usr/bin/env python3
"""Fetch the pinned P4M delivery; no download hashes or acceptance tests."""
import json
import subprocess
from fpga import ROOT, save_json, utc

FOLDER = ROOT / 'artifacts/software/p4m-baseline-20260920/delivery'
REMOTE = '/home/xvhaocheng/work/iommu-platform/out/p4m-baseline-20260920/delivery'
FILES = ('README.md', 'P4M_BASELINE.md', 'delivery.json', 'build-status.json',
         'platform-p4m', 'migration-4dma-p4m.json', 'fw_payload.bin', 'ariane_xilinx.bit')


def check_metadata():
    d = json.loads((FOLDER / 'delivery.json').read_text())
    if (d['kernel_release'] != '6.5.0-rc2-00088-g9f8da9e7a6a0'
            or d['versions']['linux'] != '9f8da9e7a6a08fb19134146241018eaff0eee124'
            or d['versions']['cva6'] != 'e822bc9d8adf246a5d098585fb229744bafd9cc1'):
        raise RuntimeError('Unexpected P4M release identity')
    for name, size in (('fw_payload.bin', 29363656), ('ariane_xilinx.bit', 11443722)):
        if (FOLDER / name).stat().st_size != size:
            raise RuntimeError('Incomplete delivery file: ' + name)
    return d


def main():
    if not FOLDER.exists():
        FOLDER.mkdir(parents=True)
        subprocess.run(['scp'] + ['dell:' + REMOTE + '/' + n for n in FILES]
                       + [str(FOLDER) + '/'], check=True)
    if not all((FOLDER / n).is_file() for n in FILES):
        raise RuntimeError('Incomplete download retained; refusing to overwrite')
    check_metadata()
    save_json(FOLDER.parent / 'download.json', dict(
        status='downloaded_hash_not_checked', hash_verified=False,
        source=REMOTE, finished_utc=utc(), files=list(FILES)))
    print(FOLDER)


if __name__ == '__main__':
    main()
