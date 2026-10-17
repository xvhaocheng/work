#!/usr/bin/env python3
"""Host-only package metadata and hardlinks from accepted E4/E5 manifests.

No FPGA programming, UART, SD update or repeated binary hashing. Root copies
small metadata and links binaries into its formal board package when scheduled.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

HERE=Path(__file__).resolve().parent
LINUX='da6b69fc007ad5e671dc288dd8038d02e3ebe43d'
PLATFORM='a68603f0fc668771bc0c9e3a86fcefe289bfa17b'
CVA6='6c5c666d45979d3a03fb98cff89ba7ad095f6125'
BIT='d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c'


def save(path,value):path.write_text(json.dumps(value,indent=2)+'\n')


def metadata_sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--software-manifest',type=Path,required=True)
    p.add_argument('--fpga-manifest',type=Path,required=True)
    p.add_argument('--transport-metadata',type=Path,required=True)
    p.add_argument('--platform-repo',type=Path,default=Path('/home/xvhaocheng/work/iommu-platform'))
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--receipt',type=Path,required=True)
    a=p.parse_args();out=a.output.resolve()
    assert out.is_relative_to((HERE/'artifacts').resolve()) and not out.exists(),'new E4 artifacts package required'
    software=json.loads(a.software_manifest.read_text());fpga=json.loads(a.fpga_manifest.read_text());transport=json.loads(a.transport_metadata.read_text())
    assert software['status']==fpga['status']=='passed'
    assert software['selected_linux']==LINUX and software['selected_platform']==PLATFORM and software['selected_cva6']==fpga['selected_cva6']==CVA6
    assert fpga['parameters']['observer_version']==4 and fpga['parameters']['PathDepth']==4 and fpga['parameters']['histogram_scheme']==1
    assert fpga['parameters']['ds_cycles']==[0,0] and fpga['parameters']['clock_hz']==50000000
    assert all(fpga['timing'][k]==0 for k in ['setup_failing','hold_failing','pulse_failing']) and fpga['timing']['wns_ns']>=0 and fpga['routing_errors']==0
    bits=[(Path(k),v) for k,v in fpga['artifacts'].items() if Path(k).name=='ariane_xilinx.bit']
    assert len(bits)==1 and bits[0][1]==BIT
    assert transport['delivery_manifest']['sha256']==metadata_sha(a.software_manifest)
    (out/'artifacts').mkdir(parents=True);(out/'metadata').mkdir()
    for source,name in [(a.software_manifest,'software-build-manifest.json'),(a.fpga_manifest,'fpga-build-manifest.json'),(a.transport_metadata,'transport.json')]:
        shutil.copyfile(source,out/'metadata'/name)
    lock=subprocess.check_output(['git','-C',str(a.platform_repo),'show',PLATFORM+':sources.lock.json'])
    (out/'metadata/sources.lock.json').write_bytes(lock)
    components={c['path']:c['commit'] for c in json.loads(lock)['components']}
    assert components['linux']==LINUX and components['cva6']==CVA6
    build=a.software_manifest.resolve().parent
    artifacts={}
    def link(label,original,name,digest):
        original=Path(original).resolve();target=out/'artifacts'/name
        assert original.is_file()
        target.hardlink_to(original)
        assert target.stat().st_ino==original.stat().st_ino and target.stat().st_size==original.stat().st_size
        artifacts[label]={'file':str(target.relative_to(out)),'original_file':str(original),'sha256':digest,
                          'bytes':original.stat().st_size,'hash_source':'existing accepted build/transport manifest; binary not rehashed'}
    link('ds0_bit',bits[0][0],'ds0.bit',BIT)
    link('sd_payload',build/'fw_payload.bin','fw_payload.bin',software['artifacts']['fw_payload.bin'])
    link('linux_Image',build/'linux-build/arch/riscv/boot/Image','Image',software['artifacts']['linux-build/arch/riscv/boot/Image'])
    link('payload_gzip',transport['gzip']['path'],'fw_payload.bin.gz',transport['gzip']['sha256'])
    link('spi_recovery_archive',transport['spi_archive']['path'],'spi-recovery-e4-e5.tar.gz',transport['spi_archive']['sha256'])
    for name in ['spi-bitbang.ko','spi-xilinx.ko']:
        link(name,build/'spi-recovery'/name,name,software['artifacts']['spi-recovery/'+name])
    source={'actual_commits':{'platform':PLATFORM,'cva6':CVA6,'iommu':fpga['selected_iommu'],'linux':LINUX,'opensbi':software['selected_opensbi']},
            'source_ref':'refs/heads/ispass/a3-pagesize','lock':'metadata/sources.lock.json',
            'existing_software_manifest':'metadata/software-build-manifest.json','existing_fpga_manifest':'metadata/fpga-build-manifest.json',
            'note':'Original accepted manifests copied byte-for-byte; exact BIT/Image/payload hardlinks, no new binary hash or hardware operation.'}
    manifest={'experiment':'E4 fixed-footprint 4KiB/2MiB leaves plus E5 passive OBS4 histogram','status':'ready_for_collection',
              'source_record':'source.json','kernel_release':software['kernel_release'],'artifacts':artifacts,
              'sd_uses':'artifacts/fw_payload.bin','existing_build_delivery':'metadata/software-build-manifest.json',
              'existing_fpga_build':'metadata/fpga-build-manifest.json','existing_timing':{'ds0':fpga['timing']},
              'planned_measurements':20,'measurements_per_leaf_size':10,'observer_version':4,'histogram_schema':1,'PathDepth':4,
              'scope':'V0/A1; each source/destination2MiB,total8MiB,COPY4096,V4096,W256; only explicit leaf size differs; A final work measured',
              'recovery_policy':'Current acece boot uses its own p0-multi modules for update. Included modules only match the new e4-e5 kernel; formal measurements load no modules.',
              'results_note':'No load receipt or board measurements created by package preparation; keep these identity manifests immutable.'}
    save(out/'source.json',source);save(out/'manifest.json',manifest)
    receipt={'schema':'ispass2027.e4_e5.package_preparation.v1','status':'prepared','board_operations':False,'new_board_cases':0,
             'package':str(out),'package_manifest_sha256':metadata_sha(out/'manifest.json'),'source_record_sha256':metadata_sha(out/'source.json'),
             'metadata_copies':{name:{'sha256':metadata_sha(out/'metadata'/name),'bytes':(out/'metadata'/name).stat().st_size} for name in ['software-build-manifest.json','fpga-build-manifest.json','transport.json','sources.lock.json']},
             'artifacts':artifacts,'binary_hash_policy':'reused existing manifest hashes; hardlink inode/size match only',
             'kernel_release':software['kernel_release'],'actual_commits':source['actual_commits']}
    with a.receipt.open('x') as stream:stream.write(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps({'status':'prepared','package':str(out),'bit_sha256':BIT,'board_operations':False}))


if __name__=='__main__':main()
