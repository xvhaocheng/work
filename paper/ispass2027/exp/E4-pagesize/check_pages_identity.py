#!/usr/bin/env python3
"""Finite synthetic receipts plus actual passed software receipt; never UART."""
import argparse
import copy
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import collect_pages as c


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--e5-dir',type=Path,required=True)
    p.add_argument('--software-manifest',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    identity,_=c.e5_modules(a.e5_dir)
    software=json.loads(a.software_manifest.read_text())
    c.validate_software(software,c.KERNEL)
    checks=[{'name':'actual_passed_shared_software_receipt','passed':True,'path':str(a.software_manifest.resolve()),'sha256':c.sha(a.software_manifest)}]
    for key,value in [('status','running'),('selected_linux','0'*40),('selected_platform','0'*40),('kernel_release','old')]:
        bad=copy.deepcopy(software);bad[key]=value
        try:c.validate_software(bad,c.KERNEL)
        except AssertionError:checks.append({'name':'software_'+key+'_rejected','passed':True})
        else:raise AssertionError(key)
    with tempfile.TemporaryDirectory(prefix='e4-host-identity-') as temp:
        root=Path(temp);package=root/'package';package.mkdir();(package/'fixture.bit').write_text('SYNTHETIC HOST; NO BIT')
        delivery={'fixture_only':True,'status':'ready_for_collection','kernel_release':c.KERNEL,
                  'source_record':'source.json','artifacts':{'ds0_bit':{'file':'fixture.bit','sha256':'b'*64}}}
        source={'fixture_only':True,'actual_commits':{'platform':c.PLATFORM,'linux':c.LINUX,'cva6':identity.CVA6,'iommu':'c'*40,'opensbi':'e'*40}}
        load={'fixture_only':True,'status':'boot_identity_verified','kernel_release':c.KERNEL,'boot_id':'HOST-FIXTURE-NO-BOARD','bit_sha256':'b'*64}
        fpga={'fixture_only':True,'status':'passed','selected_cva6':identity.CVA6,'selected_iommu':'c'*40,
              'parameters':{'observer_version':4,'PathDepth':4,'histogram_stages':16,'histogram_bins':32,
                            'histogram_counter_bits':32,'histogram_scheme':1,'ds_delay_enable':False,'ds_cycles':[0,0],'clock_hz':50000000},
              'timing':{'wns_ns':.1,'setup_failing':0,'hold_failing':0,'pulse_failing':0},'routing_errors':0,
              'routed_boundary':{'passed':True,'cpu_period_ns':20,'expected_delay_enable':0,'delay_cell_count':0,'forbidden_research_cell_count':0},
              'artifacts':{'fixture/ariane_xilinx.bit':'b'*64}}
        def write():
            for path,obj in [(package/'manifest.json',delivery),(package/'source.json',source),(root/'load.json',load),(root/'fpga.json',fpga)]:
                path.write_text(json.dumps(obj)+'\n')
        write();bound=identity.identity(package,root/'load.json',root/'fpga.json')
        assert bound['histogram_required'] and bound['actual_commits']['linux']==c.LINUX
        checks.append({'name':'synthetic_final_OBS4_receipts_identity_only','passed':True})
        for target,key,value in [(load,'bit_sha256','f'*64),(load,'kernel_release','old'),
                                 (fpga,'status','running'),(fpga['parameters'],'observer_version',3),
                                 (fpga['parameters'],'ds_cycles',[50,50]),(fpga['timing'],'wns_ns',-.1)]:
            old=target[key];target[key]=value;write()
            try:identity.identity(package,root/'load.json',root/'fpga.json')
            except ValueError:checks.append({'name':'identity_'+key+'_rejected','passed':True})
            else:raise AssertionError(key)
            target[key]=old
        write()
        result=subprocess.run([sys.executable,str(Path(c.__file__)), '--check-only','--package',str(package),
            '--load-manifest',str(root/'load.json'),'--fpga-manifest',str(root/'fpga.json'),
            '--software-manifest',str(a.software_manifest),'--e5-dir',str(a.e5_dir),
            '--board-dir',str(root/'NO-BOARD'),'--output',str(root/'never-created')],capture_output=True,text=True,timeout=10)
        assert result.returncode==0,result.stderr
        assert json.loads(result.stdout)['actual_commits']['linux']==c.LINUX and not (root/'never-created').exists()
        checks.append({'name':'check_only_exits_without_board_import_or_output_directory','passed':True})
        actual_args=[arg for arg in result.args if arg!='--check-only']
        refused=subprocess.run(actual_args,capture_output=True,text=True,timeout=10)
        assert refused.returncode!=0 and 'host fixtures cannot start collection' in refused.stderr
        assert not (root/'never-created').exists()
        checks.append({'name':'explicit_fixture_receipt_refuses_actual_collection_before_UART','passed':True})
    assert 'uart' not in sys.modules
    a.output.write_text(json.dumps({'schema':'ispass2027.e4_pagesize.identity_host.v1','status':'passed','fixture_only':True,
        'board_cases':0,'collector_sha256':c.sha(c.__file__),'checks':checks},indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':len(checks),'board_cases':0}))


if __name__=='__main__':main()
