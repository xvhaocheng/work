#!/usr/bin/env python3
"""Fetch the pinned 00087 E3 v6 controlled delivery; no download hash calculation by request."""
import json
import shlex
import shutil
import subprocess
import tarfile
from fpga import ROOT, save_json, utc

FOLDER = ROOT / 'artifacts/software/e3-controlled-20260918/delivery'
REMOTE = '/home/xvhaocheng/work/iommu-platform/out/e3-controlled-20260918/delivery'
BIT_SHA = 'b55f5afbba667a7540ed003dd724d078e0e6e4bff5d30fe7e796abba5b7f2ecc'
PAYLOAD_SHA = '3ec5ff7f5e376e94862f148739c10a0a2f487f7ca5e8afa755e08bf21f40a914'
FILES = {'README.md','SHA256SUMS','delivery.json','build-status.json','fw_payload.bin','ariane_xilinx.bit','E3_CONTROLLED.md','E3_QUEUE_PAIR.md','E3_SERVICE.md','E3_DIAGNOSTIC.md','idma_e3_controlled','platform-e3-controlled','idma.h','platform.dtb','sources.lock.json','profile.json','software-manifest.json','fpga-manifest.json','validation.json'}

def check_metadata():
    d = json.loads((FOLDER/'delivery.json').read_text())
    if (d['profile'] != 'migration-4dma-e3-controlled'
            or d['kernel_release'] != '6.5.0-rc2-00087-g2de370cd344e'
            or d['versions']['linux'] != '2de370cd344ed2b6bb1b0fa5b42bfe4eeb96214b'
            or d['versions']['cva6'] != '2b04936d2b4fe970bba4c97ab99f0a46a00eec06'
            or d['versions']['cva6/corev_apu/riscv-iommu'] != '137f5e5fb1f2cc65a1346359226b16247c3f8445'
            or d['platform']['software']['build_start_platform_commit'] != '9839f447d4db2bf3f90704d67cdec4a658748a95'
            or d['platform']['fpga']['build_start_platform_commit'] != '57b3bcf1c0a64731ff1bedd007def70c71372ed3'
            or d['software_exit_code'] != 0 or d['fpga_exit_code'] != 0
            or d['hashes']['ariane_xilinx.bit'] != BIT_SHA
            or d['hashes']['fw_payload.bin'] != PAYLOAD_SHA
            or (FOLDER/'fw_payload.bin').stat().st_size != 29363656
            or (FOLDER/'ariane_xilinx.bit').stat().st_size != 11443722):
        raise RuntimeError('Unexpected release identity/length')
    return d

def main():
    if not FOLDER.exists():
        FOLDER.mkdir(parents=True)
        save_json(FOLDER.parent/'download.json',dict(status='incomplete',source=REMOTE,started_utc=utc()))
        command = shlex.join(['tar','-C',REMOTE,'-cf','-','--']+sorted(FILES))
        process = subprocess.Popen(['ssh','-o','BatchMode=yes','dell',command],stdout=subprocess.PIPE)
        received=set()
        try:
            with tarfile.open(fileobj=process.stdout,mode='r|') as archive:
                for entry in archive:
                    if entry.name not in FILES or entry.name in received or not entry.isfile() or entry.size>32*1024*1024:
                        raise RuntimeError('Unexpected archive member')
                    with archive.extractfile(entry) as src, (FOLDER/entry.name).open('xb') as dst:
                        shutil.copyfileobj(src,dst)
                    received.add(entry.name)
            if process.wait() or received != FILES:
                raise RuntimeError('Incomplete download retained')
        finally:
            process.stdout.close()
            if process.poll() is None:
                process.terminate(); process.wait()
    if any(not (FOLDER/n).is_file() for n in FILES):
        raise RuntimeError('Incomplete delivery retained; do not reuse')
    check_metadata()
    save_json(FOLDER.parent/'download.json',dict(status='downloaded_hash_not_checked',hash_verified=False,source=REMOTE,finished_utc=utc(),files=sorted(FILES)))
    print(FOLDER)

if __name__ == '__main__':
    main()
