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
import software_profiles as profiles

HERE=Path(__file__).resolve().parent
CVA6=profiles.HARDWARE_CVA6
BIT=profiles.BIT_SHA256


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
    profile=profiles.validate_software(software,software['kernel_release'])
    assert fpga['status']=='passed' and fpga['selected_cva6']==CVA6 and fpga['selected_iommu']==profiles.HARDWARE_IOMMU
    assert fpga['parameters']['observer_version']==4 and fpga['parameters']['PathDepth']==4 and fpga['parameters']['histogram_scheme']==1
    assert fpga['parameters']['ds_cycles']==[0,0] and fpga['parameters']['clock_hz']==50000000
    assert all(fpga['timing'][k]==0 for k in ['setup_failing','hold_failing','pulse_failing']) and fpga['timing']['wns_ns']>=0 and fpga['routing_errors']==0
    bits=[(Path(k),v) for k,v in fpga['artifacts'].items() if Path(k).name=='ariane_xilinx.bit']
    assert len(bits)==1 and bits[0][1]==BIT
    assert transport['delivery_manifest']['sha256']==metadata_sha(a.software_manifest)
    assert transport['kernel_release']==software['kernel_release']
    lock=subprocess.check_output(['git','-C',str(a.platform_repo),'show',software['selected_platform']+':sources.lock.json'])
    components={c['path']:c['commit'] for c in json.loads(lock)['components']}
    assert components['linux']==software['selected_linux'] and components['cva6']==software['selected_cva6']
    software_reference={**{key:software[key] for key in profiles.PROFILE_FIELDS+('selected_opensbi',)},
                        'cva6_role':profiles.SOFTWARE_CVA6_ROLE}
    fpga_reference={**{key:fpga[key] for key in ('selected_platform','selected_cva6','selected_iommu','source_manifest','source_manifest_sha256')},
                    'linux_role':profiles.FPGA_LINUX_ROLE}
    (out/'artifacts').mkdir(parents=True);(out/'metadata').mkdir()
    for source,name in [(a.software_manifest,'software-build-manifest.json'),(a.fpga_manifest,'fpga-build-manifest.json'),(a.transport_metadata,'transport.json')]:
        shutil.copyfile(source,out/'metadata'/name)
    (out/'metadata/sources.lock.json').write_bytes(lock)
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
    source={'actual_commits':{'platform':software['selected_platform'],'cva6':CVA6,'iommu':fpga['selected_iommu'],
                              'linux':software['selected_linux'],'opensbi':software['selected_opensbi']},
            'software_profile':profile,'source_ref':'accepted E4 software profile: '+profile,'lock':'metadata/sources.lock.json',
            'software_build_reference':software_reference,'fpga_build_reference':fpga_reference,
            'component_roles':{'platform':'software platform source/lock; hardware source is fpga_build_reference',
                               'cva6':'actual loaded E5 FPGA source','iommu':'actual loaded E5 FPGA source',
                               'linux':'actual running Linux source','opensbi':'actual software OpenSBI source'},
            'existing_software_manifest':'metadata/software-build-manifest.json','existing_fpga_manifest':'metadata/fpga-build-manifest.json',
            'note':'Original accepted manifests copied byte-for-byte; exact BIT/Image/payload hardlinks, no new binary hash or hardware operation.'}
    manifest={'experiment':'E4 fixed-footprint 4KiB/2MiB leaves plus E5 passive OBS4 histogram','status':'ready_for_collection',
              'source_record':'source.json','software_profile':profile,'kernel_release':software['kernel_release'],'artifacts':artifacts,
              'sd_uses':'artifacts/fw_payload.bin','existing_build_delivery':'metadata/software-build-manifest.json',
              'existing_fpga_build':'metadata/fpga-build-manifest.json','existing_timing':{'ds0':fpga['timing']},
              'planned_measurements':20,'measurements_per_leaf_size':10,'observer_version':4,'histogram_schema':1,'PathDepth':4,
              'scope':'V0/A1; each source/destination2MiB,total8MiB,COPY4096,V4096,W256; only explicit leaf size differs; A final work measured',
              'recovery_policy':'Update under the current kernel uses its own matching SPI modules. Included modules match '+software['kernel_release']+' only; formal measurements load no modules.',
              'results_note':'No load receipt or board measurements created by package preparation; keep these identity manifests immutable.'}
    save(out/'source.json',source);save(out/'manifest.json',manifest)
    receipt={'schema':'ispass2027.e4_e5.package_preparation.v1','status':'prepared','board_operations':False,'new_board_cases':0,
             'package':str(out),'package_manifest_sha256':metadata_sha(out/'manifest.json'),'source_record_sha256':metadata_sha(out/'source.json'),
             'metadata_copies':{name:{'sha256':metadata_sha(out/'metadata'/name),'bytes':(out/'metadata'/name).stat().st_size} for name in ['software-build-manifest.json','fpga-build-manifest.json','transport.json','sources.lock.json']},
             'artifacts':artifacts,'binary_hash_policy':'reused existing manifest hashes; hardlink inode/size match only',
             'kernel_release':software['kernel_release'],'software_profile':profile,
             'software_profile_helper_sha256':metadata_sha(Path(profiles.__file__)),
             'actual_commits':source['actual_commits']}
    with a.receipt.open('x') as stream:stream.write(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps({'status':'prepared','package':str(out),'bit_sha256':BIT,'board_operations':False}))


if __name__=='__main__':main()
