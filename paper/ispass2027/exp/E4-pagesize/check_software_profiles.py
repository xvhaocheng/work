#!/usr/bin/env python3
"""Finite host-only profile and package checks; no builds or board operations."""
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
    p.add_argument('--platform-repo',type=Path,required=True)
    p.add_argument('--e5-dir',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();out=a.output.resolve()
    assert out.is_relative_to(collector.HERE) and not out.exists()
    out.mkdir(parents=True)
    paths=[a.software_manifest,a.fpga_manifest,a.transport_metadata]
    originals={path.resolve():path.read_bytes() for path in paths}
    software=json.loads(a.software_manifest.read_text());fpga=json.loads(a.fpga_manifest.read_text())
    checks=[];commands=[]
    def reject(name,call):
        try:call()
        except (AssertionError,ValueError,KeyError) as error:
            checks.append({'name':name,'passed':True,'observed_error':type(error).__name__+': '+str(error)})
        else:raise AssertionError(name+' accepted')
    def run(argv):
        result=subprocess.run([sys.executable,*map(str,argv)],capture_output=True,text=True,timeout=30)
        commands.append({'argv':result.args,'exit_code':result.returncode,'stdout':result.stdout,'stderr':result.stderr})
        return result
    status='failed'
    try:
        profile=collector.validate_software(software,software['kernel_release'])
        assert profile=='e4-e5-v1'
        checks.append({'name':'actual_passed_legacy_software_exact_tuple_accepted','passed':True})
        for key,value in [('status','running'),('selected_linux','0'*40),('selected_platform','0'*40),
                          ('selected_cva6','0'*40),('kernel_release','unregistered-release')]:
            changed=copy.deepcopy(software);changed[key]=value
            reject('software_'+key+'_rejected',lambda:collector.validate_software(changed,software['kernel_release']))
        changed=copy.deepcopy(software);changed['handwritten_inputs']['bench/idma_pages.c']='0'*64
        reject('changed_E4_CLI_rejected',lambda:collector.validate_software(changed,software['kernel_release']))
        changed=copy.deepcopy(software);changed['recovery']['kernel_release']='old'
        reject('wrong_SPI_release_rejected',lambda:collector.validate_software(changed,software['kernel_release']))
        unknown=copy.deepcopy(software)
        unknown.update(selected_linux='46df97c9820d0d63878215cd553cfee1c1b6b760',
                       selected_platform='0'*40,selected_cva6='1d587125eee690e623e6ffae13e70f9bbe989adc',
                       kernel_release='UNFROZEN-ABI5-NO-EXECUTION',fixture_only=True)
        unknown['recovery']['kernel_release']=unknown['kernel_release']
        reject('unfrozen_ABI5_tuple_rejected',lambda:collector.validate_software(unknown,unknown['kernel_release']))
        unknown_path=out/'unfrozen-software.fixture.json';collector.save(unknown_path,unknown)
        package=collector.HERE/'artifacts'/out.name/'legacy-package'
        arguments=[collector.HERE/'prepare_package.py','--fpga-manifest',a.fpga_manifest,
                   '--transport-metadata',a.transport_metadata,'--platform-repo',a.platform_repo]
        refused=run([*arguments,'--software-manifest',unknown_path,'--output',package,'--receipt',out/'never-created.json'])
        assert refused.returncode!=0 and 'unknown E4 software profile' in refused.stderr
        assert not package.exists() and not (out/'never-created.json').exists()
        checks.append({'name':'unknown_profile_package_refused_before_output_or_git_lock','passed':True})
        prepared=run([*arguments,'--software-manifest',a.software_manifest,'--output',package,'--receipt',out/'package-receipt.json'])
        assert prepared.returncode==0,prepared.stderr
        source=json.loads((package/'source.json').read_text())
        collector.save(out/'prepared-source.json',source)
        collector.save(out/'prepared-manifest.json',json.loads((package/'manifest.json').read_text()))
        load_path=out/'load.fixture.json'
        collector.save(load_path,{'fixture_only':True,'status':'boot_identity_verified',
            'kernel_release':software['kernel_release'],'boot_id':'HOST-FIXTURE-NO-BOARD',
            'bit_sha256':collector.profiles.BIT_SHA256})
        refused=run([collector.HERE/'collect_pages.py','--package',package,'--load-manifest',load_path,
                     '--fpga-manifest',a.fpga_manifest,'--software-manifest',unknown_path,'--e5-dir',a.e5_dir,
                     '--board-dir',out/'NO-BOARD','--output',out/'never-collect'])
        assert refused.returncode!=0 and 'unknown E4 software profile' in refused.stderr
        assert not (out/'never-collect').exists()
        checks.append({'name':'unknown_profile_actual_collector_start_refused_before_output_and_UART','passed':True})
        bound={'kernel_release':software['kernel_release'],'actual_commits':source['actual_commits'],
               'bit_sha256':collector.profiles.BIT_SHA256}
        assert collector.profiles.validate_bound_software(bound,software,source,fpga)==profile
        # The original package does not need the newly introduced role fields.
        legacy={'actual_commits':source['actual_commits']}
        assert collector.profiles.validate_bound_software(bound,software,legacy,fpga)==profile
        checks.append({'name':'legacy_package_and_pre_role_immutable_source_accepted','passed':True})
        for key in source['actual_commits']:
            wrong=copy.deepcopy(bound);wrong['actual_commits'][key]='0'*40
            wrong_source={'actual_commits':wrong['actual_commits']}
            reject('actual_component_'+key+'_rejected',
                   lambda:collector.profiles.validate_bound_software(wrong,software,wrong_source,fpga))
        wrong=dict(bound,bit_sha256='0'*64)
        reject('different_BIT_rejected',lambda:collector.profiles.validate_bound_software(wrong,software,source,fpga))
        for path,name in [(a.software_manifest,'software-build-manifest.json'),(a.fpga_manifest,'fpga-build-manifest.json')]:
            assert (package/'metadata'/name).read_bytes()==originals[path.resolve()]
        manifest=json.loads((package/'manifest.json').read_text())
        for item in manifest['artifacts'].values():
            target=package/item['file'];original=Path(item['original_file'])
            assert target.stat().st_ino==original.stat().st_ino and target.stat().st_size==original.stat().st_size
        checks.append({'name':'original_build_manifest_bytes_and_binary_hardlinks_preserved_without_rehash','passed':True})
        assert all(path.read_bytes()==raw for path,raw in originals.items()) and 'uart' not in sys.modules
        checks.append({'name':'inputs_unchanged_no_UART_import_no_board_cases','passed':True})
        status='passed'
    finally:
        collector.save(out/'validation.json',{'schema':'ispass2027.e4.software_profiles.host.v1','status':status,
            'fixture_only':True,'source_board_cases':0,'board_operations':False,
            'collector_sha256':collector.sha(collector.__file__),
            'harness_sha256':collector.sha(__file__),
            'profile_helper_sha256':collector.sha(collector.profiles.__file__),
            'package_preparer_sha256':collector.sha(collector.HERE/'prepare_package.py'),
            'inputs':[{'path':str(path),'sha256':collector.sha(path)} for path in originals],
            'checks':checks,'commands':commands})
    print(json.dumps({'status':status,'checks':len(checks),'board_operations':False,'source_board_cases':0}))


if __name__=='__main__':main()
