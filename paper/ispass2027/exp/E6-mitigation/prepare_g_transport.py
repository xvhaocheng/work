#!/usr/bin/env python3
"""Prepare deterministic gzip payload/SPI archive for the existing bytesack flow."""
import argparse
import gzip
import hashlib
import io
import json
from pathlib import Path
import subprocess
import tarfile


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--build-manifest',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();manifest=json.loads(a.build_manifest.read_text());root=a.build_manifest.resolve().parent
    assert manifest['status']=='passed' and manifest['kernel_release']=='6.5.0-rc2-ispass2027-g-v1'
    payload=root/'fw_payload.bin';body=payload.read_bytes();compressed=root/'fw_payload.bin.gz'
    assert sha(body)==manifest['artifacts']['fw_payload.bin']
    with compressed.open('xb') as raw:
        with gzip.GzipFile(filename='',mode='wb',fileobj=raw,compresslevel=6,mtime=0) as packed:packed.write(body)
    zipped=compressed.read_bytes();assert gzip.decompress(zipped)==body
    tar=io.BytesIO();module_records={}
    with tarfile.open(fileobj=tar,mode='w',format=tarfile.USTAR_FORMAT) as archive:
        for name in ['spi-bitbang.ko','spi-xilinx.ko']:
            path=root/'spi-recovery'/name;data=path.read_bytes()
            assert sha(data)==manifest['artifacts']['spi-recovery/'+name]
            report=subprocess.check_output(['riscv64-linux-gnu-readelf','-p','.modinfo',str(path)],text=True)
            vermagic=next(line.split('vermagic=',1)[1].strip() for line in report.splitlines() if 'vermagic=' in line)
            assert vermagic.startswith(manifest['kernel_release']+' ')
            info=tarfile.TarInfo(name);info.size=len(data);info.mode=0o644;info.mtime=0;info.uid=info.gid=0
            archive.addfile(info,io.BytesIO(data))
            module_records[name]={'bytes':len(data),'sha256':manifest['artifacts']['spi-recovery/'+name],'vermagic':vermagic}
    modules=root/'spi-recovery-g.tar.gz'
    with modules.open('xb') as raw:
        with gzip.GzipFile(filename='',mode='wb',fileobj=raw,compresslevel=6,mtime=0) as packed:packed.write(tar.getvalue())
    with tarfile.open(fileobj=io.BytesIO(gzip.decompress(modules.read_bytes())),mode='r:') as archive:
        assert archive.getnames()==list(module_records)
        assert all(archive.extractfile(n).read()==(root/'spi-recovery'/n).read_bytes() for n in module_records)
    result={'schema':'ispass2027.g.transport.v1','status':'prepared','board_operations':False,
            'delivery_manifest':{'path':str(a.build_manifest.resolve()),'sha256':sha(a.build_manifest.read_bytes())},
            'kernel_release':manifest['kernel_release'],
            'payload':{'path':str(payload),'bytes':len(body),'sha256':manifest['artifacts']['fw_payload.bin']},
            'gzip':{'path':str(compressed),'bytes':len(zipped),'sha256':sha(zipped),'compresslevel':6,'mtime':0,'embedded_filename':''},
            'round_trip_host_verified':True,
            'spi_archive':{'path':str(modules),'bytes':modules.stat().st_size,'sha256':sha(modules.read_bytes()),'members':module_records},
            'board_unpack_payload':'gzip -dc /tmp/ispass2027-g-fw.bin.gz > /tmp/ispass2027-g-fw.bin',
            'board_unpack_modules':'mkdir -p /tmp/ispass2027-g-spi; gzip -dc /tmp/ispass2027-g-spi.tar.gz | tar -xf - -C /tmp/ispass2027-g-spi',
            'module_policy':'Archive modules match the new G kernel and are only for later updates under that kernel. The current boot must use modules matching its own kernel release. Formal measurements load no modules.',
            'upload_protocol':'existing uart.py gzip bytesack; Root alone owns UART; no board operation performed'}
    with a.output.open('x') as out:out.write(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':'prepared','gzip_bytes':len(zipped),'module_archive_bytes':modules.stat().st_size,'board_operations':False}))


if __name__=='__main__':main()
