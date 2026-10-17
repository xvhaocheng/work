#!/usr/bin/env python3
"""Actual delivery identities plus finite host refusals; no UART or builds."""
import argparse
import ast
import copy
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import collect_multi as collector


def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--legacy-software',type=Path,required=True)
    p.add_argument('--new-software',type=Path,required=True)
    p.add_argument('--hardware-source',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();out=a.output.resolve();assert out.is_relative_to(Path(__file__).resolve().parent) and not out.exists()
    out.mkdir(parents=True);build=out/'build';build.mkdir();checks=[];commands=[]
    paths=[a.legacy_software,a.new_software,a.hardware_source]
    originals={path:path.read_bytes() for path in paths}
    source=json.loads(a.hardware_source.read_text())
    def save(path,value):path.write_text(json.dumps(value,indent=2)+'\n')
    def reject(name,call):
        try:call()
        except (AssertionError,ValueError,KeyError) as error:
            checks.append({'name':name,'passed':True,'observed_error':type(error).__name__+': '+str(error)})
        else:raise AssertionError(name+' accepted')
    status='failed'
    try:
        for label,path in [('p0-multi-v1',a.legacy_software),('full-e3-v1',a.new_software)]:
            software=json.loads(path.read_text());kernel=software['kernel_release']
            load={'fixture_only':True,'status':'boot_identity_verified','kernel_release':kernel,
                  'boot_id':'HOST-FIXTURE-NO-BOARD','bit_sha256':collector.DS0_BIT_SHA256}
            identity=collector.profiles.bind_identity(software,kernel,source,load)
            assert identity['software_profile']==label and identity['runtime_commits']['linux']==software['selected_linux']
            assert identity['runtime_commits']['cva6']==collector.profiles.BASELINE_COMMITS['cva6']
            assert identity['software_build_reference']['selected_cva6']==software['selected_cva6']
            checks.append({'name':'actual_passed_'+label+'_distinct_software_hardware_roles','passed':True,'manifest_sha256':sha(path)})
            load_path=build/(label+'.load.fixture.json');save(load_path,load)
            argv=[sys.executable,str(Path(collector.__file__)), '--ds','0/0','--expected-kernel',kernel,
                  '--load-manifest',str(load_path),'--source-record',str(a.hardware_source),
                  '--software-manifest',str(path),'--output',str(build/('never-'+label))]
            for check_only in (True,False):
                result=subprocess.run([*argv,*(['--check-only'] if check_only else [])],capture_output=True,text=True,timeout=10)
                commands.append({'argv':result.args,'exit_code':result.returncode,'stdout':result.stdout,'stderr':result.stderr})
                if check_only:assert result.returncode==0 and json.loads(result.stdout)==identity,result.stderr
                else:assert result.returncode!=0 and 'host fixtures cannot start collection' in result.stderr,result.stderr
                assert not (build/('never-'+label)).exists()
            checks.append({'name':label+'_check_only_and_fixture_startup_refusal_before_output','passed':True})
            for key,value in [('status','running'),('selected_linux','0'*40),('selected_platform','0'*40),
                              ('selected_cva6','0'*40),('selected_opensbi','0'*40),('kernel_release','unregistered')]:
                changed=copy.deepcopy(software);changed[key]=value
                reject(label+'_'+key+'_rejected',lambda:collector.validate_software(changed,kernel))
            changed=copy.deepcopy(software);changed['handwritten_inputs']['bench/idma_scaling.c']='0'*64
            reject(label+'_CLI_change_rejected',lambda:collector.validate_software(changed,kernel))
            wrong=dict(load,bit_sha256='d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c')
            reject(label+'_E5_BIT_not_original_DS0_rejected',lambda:collector.profiles.bind_identity(software,kernel,source,wrong))
        software=json.loads(a.new_software.read_text());kernel=software['kernel_release']
        manifest={'schema':'ispass2027.e2_multi.run.v1','planned_cases':collector.plan(),'ds':'0/0',
                  'kernel_release':kernel,'bit_sha256':load['bit_sha256'],'boot_id':load['boot_id'],
                  'load_manifest_sha256':'fixture-load','source_record_sha256':sha(a.hardware_source),
                  'software_manifest_sha256':sha(a.new_software),'selected_source':source,'selected_software':software,
                  'script_sha256':sha(collector.__file__),**identity}
        old=json.loads(json.dumps(manifest));collector.validate_recovery(old,manifest)
        checks.append({'name':'same_profile_boot_source_recovery_identity_accepted','passed':True})
        for key in ('software_profile','software_profile_helper_sha256','runtime_commits','software_build_reference',
                    'source_roles','boot_id','kernel_release','source_record_sha256','software_manifest_sha256','script_sha256'):
            wrong=copy.deepcopy(old);wrong[key]='changed'
            reject('recovery_'+key+'_rejected',lambda:collector.validate_recovery(wrong,manifest))
        for component in source['actual_commits']:
            wrong=copy.deepcopy(source);wrong['actual_commits'][component]='0'*40
            reject('hardware_reference_'+component+'_rejected',lambda:collector.profiles.bind_identity(software,kernel,wrong,load))
        current=Path(collector.__file__).read_text()
        original=subprocess.check_output(['git','show','e008972b00098331f0a9071ded06df531abdabe2:paper/ispass2027/exp/E2-device-scaling/collect_multi.py'],text=True)
        def functions(text):return {node.name:ast.get_source_segment(text,node) for node in ast.parse(text).body if isinstance(node,ast.FunctionDef)}
        before,after=functions(original),functions(current)
        for name in ('parse','plan','command','review','validate_receipt'):
            assert before[name]==after[name],name
            checks.append({'name':'unchanged_original_'+name,'passed':True,'source_sha256':hashlib.sha256(after[name].encode()).hexdigest()})
        assert all(path.read_bytes()==raw for path,raw in originals.items()) and 'uart' not in sys.modules
        checks.append({'name':'original_manifests_unchanged_and_no_UART_import','passed':True})
        status='passed'
    finally:
        save(out/'validation.json',{'schema':'ispass2027.e2.software_profiles.host.v1','status':status,'fixture_only':True,
            'source_board_cases':0,'board_operations':False,'collector_sha256':sha(collector.__file__),
            'profile_helper_sha256':collector.profiles.helper_sha256(),'harness_sha256':sha(__file__),
            'inputs':[{'path':str(path.resolve()),'sha256':sha(path)} for path in paths],'checks':checks,'commands':commands})
    print(json.dumps({'status':status,'checks':len(checks),'source_board_cases':0,'board_operations':False}))


if __name__=='__main__':main()
