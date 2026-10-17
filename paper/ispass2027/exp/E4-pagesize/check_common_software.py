#!/usr/bin/env python3
"""Prepare the accepted common-software E4/E5 package and finite host refusals."""
import argparse
import copy
import json
from pathlib import Path
import subprocess
import sys
import collect_pages as collector


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--software-manifest',type=Path,required=True)
    p.add_argument('--fpga-manifest',type=Path,required=True)
    p.add_argument('--transport-metadata',type=Path,required=True)
    p.add_argument('--build-root',type=Path,required=True)
    p.add_argument('--platform-repo',type=Path,required=True)
    p.add_argument('--e5-dir',type=Path,required=True)
    p.add_argument('--package-output',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();out=a.output.resolve();assert out.is_relative_to(collector.HERE) and not out.exists()
    out.mkdir(parents=True);build=out/'build';build.mkdir();checks=[];commands=[]
    paths=[a.software_manifest,a.fpga_manifest,a.transport_metadata]
    original={path:path.read_bytes() for path in paths}
    software=json.loads(a.software_manifest.read_text());fpga=json.loads(a.fpga_manifest.read_text());transport=json.loads(a.transport_metadata.read_text())
    def run(argv):
        result=subprocess.run([sys.executable,*map(str,argv)],capture_output=True,text=True,timeout=30)
        commands.append({'argv':result.args,'exit_code':result.returncode,'stdout':result.stdout,'stderr':result.stderr})
        return result
    def prepare(name,*,sw=a.software_manifest,fg=a.fpga_manifest,tr=a.transport_metadata,root=a.build_root,positive=False):
        target=a.package_output if positive else collector.HERE/'artifacts'/out.name/('refused-'+name)
        receipt=out/('package-receipt.json' if positive else 'never-'+name+'.json')
        argv=[collector.HERE/'prepare_package.py','--software-manifest',sw,'--fpga-manifest',fg,
              '--transport-metadata',tr,'--platform-repo',a.platform_repo,'--output',target,'--receipt',receipt]
        if root is not None:argv+=['--build-root',root]
        result=run(argv)
        if positive:assert result.returncode==0,result.stderr
        else:
            assert result.returncode!=0 and not target.exists() and not receipt.exists(),result.stderr
            checks.append({'name':name+'_refused_before_package_output','passed':True})
    def changed(name,value,mutation):
        item=copy.deepcopy(value);mutation(item);path=build/(name+'.json');collector.save(path,item);return path
    def reject(name,call):
        try:call()
        except (AssertionError,ValueError,KeyError) as error:
            checks.append({'name':name,'passed':True,'observed_error':type(error).__name__+': '+str(error)})
        else:raise AssertionError(name+' accepted')
    status='failed'
    try:
        assert collector.validate_software(software,software['kernel_release'])=='full-e3-v1'
        prepare('wrong_default_Root_metadata_directory',root=None)
        prepare('wrong_explicit_binary_root',root=collector.HERE)
        prepare('unknown_profile',sw=changed('unknown-profile',software,lambda x:x.update(selected_linux='46df97c9820d0d63878215cd553cfee1c1b6b760')))
        prepare('software_manifest_hash',sw=changed('software-hash',software,lambda x:x.update(unaccepted_after_build=True)))
        prepare('FPGA_manifest_bytes',fg=changed('fpga-bytes',fpga,lambda x:x.update(selected_linux=software['selected_linux'])))
        mutations={
            'payload_hash':lambda x:x['payload'].update(sha256='0'*64),
            'gzip_hash':lambda x:x['gzip'].update(sha256='0'*64),
            'archive_hash':lambda x:x['spi_archive'].update(sha256='0'*64),
            'archive_size':lambda x:x['spi_archive'].update(bytes=x['spi_archive']['bytes']+1),
            'archive_member_set':lambda x:x['spi_archive']['members'].update({'extra.ko':{}}),
            'member_hash':lambda x:x['spi_archive']['members']['spi-bitbang.ko'].update(sha256='0'*64),
            'member_release':lambda x:x['spi_archive']['members']['spi-bitbang.ko'].update(vermagic='old SMP'),
            'member_size':lambda x:x['spi_archive']['members']['spi-bitbang.ko'].update(bytes=1),
            'transport_release':lambda x:x.update(kernel_release='old'),
        }
        for name,mutation in mutations.items():prepare(name,tr=changed(name,transport,mutation))
        prepare('accepted_common_package',positive=True)
        source=json.loads((a.package_output/'source.json').read_text());manifest=json.loads((a.package_output/'manifest.json').read_text())
        bound={'kernel_release':software['kernel_release'],'actual_commits':source['actual_commits'],'bit_sha256':collector.profiles.BIT_SHA256}
        assert collector.profiles.validate_bound_software(bound,software,source,fpga)=='full-e3-v1'
        assert source['actual_commits']['cva6']==collector.profiles.HARDWARE_CVA6!=software['selected_cva6']
        checks.append({'name':'accepted_actual_common_software_and_independent_E5_hardware_roles','passed':True})
        for name,mutation in {
            'missing_new_roles':lambda x:x.pop('software_build_reference'),
            'software_CVA6_as_actual_hardware':lambda x:x['actual_commits'].update(cva6=software['selected_cva6']),
            'hardware_CVA6_as_software_reference':lambda x:x['software_build_reference'].update(selected_cva6=collector.profiles.HARDWARE_CVA6),
            'FPGA_reference_rewritten':lambda x:x['fpga_build_reference'].update(source_manifest_sha256='0'*64),
            'software_reference_role':lambda x:x['software_build_reference'].update(cva6_role='loaded hardware'),
        }.items():
            bad=copy.deepcopy(source);mutation(bad);wrong=dict(bound,actual_commits=bad['actual_commits'])
            reject(name,lambda:collector.profiles.validate_bound_software(wrong,software,bad,fpga))
        load=build/'load.fixture.json';collector.save(load,{'fixture_only':True,'status':'boot_identity_verified',
            'kernel_release':software['kernel_release'],'boot_id':'HOST-FIXTURE-NO-BOARD','bit_sha256':collector.profiles.BIT_SHA256})
        argv=[collector.HERE/'collect_pages.py','--package',a.package_output,'--load-manifest',load,
              '--fpga-manifest',a.fpga_manifest,'--software-manifest',a.software_manifest,'--e5-dir',a.e5_dir,
              '--board-dir',build/'NO-BOARD','--output',build/'never-collect']
        checked=run([*argv,'--check-only']);assert checked.returncode==0 and json.loads(checked.stdout)['software_profile']=='full-e3-v1',checked.stderr
        refused=run(argv);assert refused.returncode!=0 and 'host fixtures cannot start collection' in refused.stderr and not (build/'never-collect').exists(),refused.stderr
        checks.append({'name':'actual_common_package_check_only_and_fixture_start_refusal_before_UART','passed':True})
        for path,name in [(a.software_manifest,'software-build-manifest.json'),(a.fpga_manifest,'fpga-build-manifest.json'),(a.transport_metadata,'transport.json')]:
            assert (a.package_output/'metadata'/name).read_bytes()==original[path]
        for item in manifest['artifacts'].values():
            target=a.package_output/item['file'];original_path=Path(item['original_file'])
            assert target.stat().st_ino==original_path.stat().st_ino and target.stat().st_size==original_path.stat().st_size==item['bytes']
        collector.save(out/'prepared-source.json',source);collector.save(out/'prepared-manifest.json',manifest)
        assert all(path.read_bytes()==data for path,data in original.items()) and 'uart' not in sys.modules
        checks.append({'name':'manifest_bytes_sizes_and_hardlinks_verified_no_binary_rehash_or_board_IO','passed':True})
        status='passed'
    finally:
        collector.save(out/'validation.json',{'schema':'ispass2027.e4.common_software.host.v1','status':status,'fixture_only':True,
            'source_board_cases':0,'board_operations':False,'package':str(a.package_output.resolve()),
            'inputs':[{'path':str(path.resolve()),'sha256':collector.sha(path)} for path in paths],
            'collector_sha256':collector.sha(collector.__file__),'profile_helper_sha256':collector.sha(collector.profiles.__file__),
            'package_preparer_sha256':collector.sha(collector.HERE/'prepare_package.py'),'harness_sha256':collector.sha(__file__),
            'checks':checks,'commands':commands})
    print(json.dumps({'status':status,'checks':len(checks),'source_board_cases':0,'board_operations':False}))


if __name__=='__main__':main()
