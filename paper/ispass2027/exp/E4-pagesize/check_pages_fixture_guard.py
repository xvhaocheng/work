#!/usr/bin/env python3
"""Focused recursive startup guard checks; retains prior38/14 receipts unchanged."""
import argparse
import copy
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import collect_pages as c

WRAPPER='''import builtins,json,pathlib,runpy,sys
attempts=[]
original=builtins.__import__
def guarded(name,*args,**kwargs):
    if name=='uart':
        attempts.append(name)
        raise RuntimeError('UART import forbidden in focused host test')
    return original(name,*args,**kwargs)
builtins.__import__=guarded
sys.argv=sys.argv[1:]
sys.path.insert(0,str(pathlib.Path(sys.argv[0]).resolve().parent))
try:
    runpy.run_path(sys.argv[0],run_name='__main__')
except AssertionError as error:
    assert 'host fixtures cannot start collection' in str(error),str(error)
else:
    raise AssertionError('nested synthetic identity unexpectedly started collection')
assert not attempts and 'uart' not in sys.modules
print(json.dumps({'nested_fixture_rejected':True,'uart_import_attempts':len(attempts),'uart_imported':'uart' in sys.modules}))
'''


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--e5-dir',type=Path,required=True)
    p.add_argument('--software-manifest',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();identity,_=c.e5_modules(a.e5_dir)
    software=json.loads(a.software_manifest.read_text());c.validate_software(software,c.KERNEL)
    checks=[]
    for value in [{'x':[{'fixture_only':True}]},{'x':{'is_fixture':'1'}},
                  {'x':[{'evidence_level':'synthetic_protocol_fixture'}]},
                  {'x':{'source_kind':'mock'}},{'x':{'boot_id':'HOST-FIXTURE-NO-BOARD'}}]:
        assert c.synthetic_markers(value)
        checks.append({'name':'recursive_marker_'+str(len(checks)+1),'passed':True})
    assert not c.synthetic_markers({'x':[{'fixture_only':False,'is_fixture':'0'}],
                                    'schema':'idma.pages.run.v1','condition':'leaf4k'})
    checks.append({'name':'normal_nested_metadata_and_false_flags_accepted','passed':True})
    with tempfile.TemporaryDirectory(prefix='e4-nested-guard-') as temporary:
        root=Path(temporary);package=root/'package';package.mkdir();(package/'test.bit').write_bytes(b'HOST ONLY')
        delivery={'status':'ready_for_collection','kernel_release':c.KERNEL,'source_record':'source.json',
                  'artifacts':{'ds0_bit':{'file':'test.bit','sha256':c.profiles.BIT_SHA256}}}
        source={'actual_commits':{'platform':c.PLATFORM,'linux':c.LINUX,'cva6':identity.CVA6,'iommu':c.profiles.HARDWARE_IOMMU,'opensbi':software['selected_opensbi']}}
        load={'status':'boot_identity_verified','kernel_release':c.KERNEL,'boot_id':'00000000-0000-0000-0000-000000000000','bit_sha256':c.profiles.BIT_SHA256}
        fpga={'status':'passed','selected_cva6':identity.CVA6,'selected_iommu':c.profiles.HARDWARE_IOMMU,
              'parameters':{'observer_version':4,'PathDepth':4,'histogram_stages':16,'histogram_bins':32,
                            'histogram_counter_bits':32,'histogram_scheme':1,'ds_delay_enable':False,'ds_cycles':[0,0],'clock_hz':50000000},
              'timing':{'wns_ns':.1,'setup_failing':0,'hold_failing':0,'pulse_failing':0},'routing_errors':0,
              'routed_boundary':{'passed':True,'cpu_period_ns':20,'expected_delay_enable':0,'delay_cell_count':0,'forbidden_research_cell_count':0},
              'artifacts':{'test/ariane_xilinx.bit':c.profiles.BIT_SHA256}}
        originals={'package':delivery,'source':source,'load':load,'fpga':fpga,'software':software}
        paths={'package':package/'manifest.json','source':package/'source.json',
               'load':root/'load.json','fpga':root/'fpga.json','software':root/'software.json'}
        raw=[]
        for index,target in enumerate(paths):
            values=copy.deepcopy(originals)
            values[target]['metadata']={'provenance':[{'evidence_level':'synthetic_protocol_fixture'}]}
            for key,path in paths.items():path.write_text(json.dumps(values[key])+'\n')
            output=root/f'never-created-{target}'
            argv=[sys.executable,'-c',WRAPPER,str(Path(c.__file__).resolve()),'--package',str(package),
                  '--load-manifest',str(paths['load']),'--fpga-manifest',str(paths['fpga']),
                  '--software-manifest',str(paths['software']),'--e5-dir',str(a.e5_dir),
                  '--board-dir',str(root/'NO-BOARD'),'--output',str(output)]
            result=subprocess.run(argv,capture_output=True,text=True,timeout=10)
            assert result.returncode==0,(target,result.stderr)
            record=json.loads(result.stdout)
            assert record=={'nested_fixture_rejected':True,'uart_import_attempts':0,'uart_imported':False}
            assert not output.exists()
            raw.append({'target':target,'exit_code':result.returncode,'stdout':result.stdout,'stderr':result.stderr,
                        'output_created':output.exists()})
            checks.append({'name':'nested_'+target+'_blocked_before_mkdir_and_UART','passed':True})
    a.output.write_text(json.dumps({'schema':'ispass2027.e4_pagesize.nested_guard.host.v1','status':'passed',
        'fixture_only':True,'board_cases':0,'collector_sha256':c.sha(c.__file__),
        'acceptance':'Exact accepted E5 recursive synthetic_markers semantics; actual main path before output mkdir/UART import',
        'preserved_prior_receipts':['pages-collector-host.json (38)','pages-identity-host.json (14)'],
        'checks':checks,'raw_cases':raw},indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':len(checks),'actual_startup_rejections':5,'uart_import_attempts':0}))


if __name__=='__main__':main()
