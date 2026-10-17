#!/usr/bin/env python3
"""Exact G profile negatives at actual collector/exporter entry; no real UART."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys
import types
import collect_workload as collector
import analyze_workload as analysis
import workload_software_profiles as profiles


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--g-run',type=Path,required=True)
    p.add_argument('--legacy-run',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    g=json.loads((a.g_run/'continued/manifest.json').read_text());old=json.loads((a.legacy_run/'continued/manifest.json').read_text())
    raw=Path(g['sources']['software']['path']).read_bytes();software=json.loads(raw);assert hashlib.sha256(raw).hexdigest()==profiles.G_MANIFEST_SHA
    checks=[];selected=profiles.select(software,profiles.G_MANIFEST_SHA)
    assert selected['software_cva6_reference']==profiles.G_REFERENCE and selected['required_actual_fpga_cva6']==collector.CVA6==profiles.HARDWARE_CVA6
    assert selected['kernel_release']==profiles.G_RELEASE
    checks.append('actual passed G manifest accepted with cfb9 software reference and independent required1d actual BIT')
    calls=[]
    class Console:
        def __init__(self,*args):calls.append(args);raise AssertionError('UART reached')
    sys.modules['uart']=types.SimpleNamespace(Console=Console,DEFAULT_PORT='/NO-REAL-UART-G-PROFILE')
    def invoke(output,software_path,resume=None):
        sys.argv=['collect_workload.py','--output',str(output),'--board-tools',str(a.output),'--max-cases','150']
        for flag,key in [('load-manifest','load'),('fpga-manifest','fpga'),('software-manifest','software'),
                         ('p0-backend','p0_backend'),('long-backend','long_backend'),('packet-backend','packet_backend')]:
            sys.argv+=['--'+flag,str(software_path) if key=='software' else g['sources'][key]['path']]
        if resume:sys.argv+=['--resume-run',str(resume)]
        collector.main(_test_fixture=True)
    mutations=[('schema',lambda s:s.update(schema='unknown')),('linux',lambda s:s.update(selected_linux='0'*40)),
        ('platform',lambda s:s.update(selected_platform='0'*40)),('cva-reference',lambda s:s.update(selected_cva6=profiles.HARDWARE_CVA6)),
        ('kernel-release',lambda s:s.update(kernel_release=profiles.LEGACY_RELEASE)),
        ('reference-only',lambda s:s.update(selected_cva6_reference_only=False)),('reference-role',lambda s:s.update(selected_cva6_role='loaded_bit')),
        ('driver',lambda s:s['kernel_critical_sources'].update({'drivers/misc/idma.c':'0'*64})),
        ('workload-uapi',lambda s:s['kernel_critical_sources'].update({'include/uapi/linux/idma_workload.h':'0'*64})),
        ('rootfs-count',lambda s:s.update(preserved_rootfs_entries=413)),
        ('common-parent',lambda s:s.update(common_software_manifest_sha256='0'*64)),
        ('actual-Image',lambda s:s['artifacts'].update({'linux-build/arch/riscv/boot/Image':'0'*64})),
        ('status',lambda s:s.update(status='running'))]
    for name in profiles.PRESERVED:
        mutations.append(('old-executable-'+name.split('/')[-1],lambda s,key=name:s['preserved_executables'].update({key:'0'*64})))
    for name,change in mutations:
        s=copy.deepcopy(software);change(s)
        try:profiles.select(s,profiles.G_MANIFEST_SHA)
        except ValueError:pass
        else:raise AssertionError('semantic G profile accepted '+name)
        path=a.output/(name+'.json');path.write_text(json.dumps(s))
        try:invoke(a.output/(name+'-rejected'),path)
        except ValueError:pass
        else:raise AssertionError('actual collector accepted '+name)
        assert not calls and not (a.output/(name+'-rejected')).exists()
        checks.append('actual collector and typed profile reject '+name+' before UART/output')
    path=a.output/'same-values-changed-bytes.json';path.write_bytes(raw+b'\n')
    try:invoke(a.output/'manifest-byte-change-rejected',path)
    except ValueError:pass
    else:raise AssertionError('changed manifest bytes accepted')
    assert not calls;checks.append('even unchanged G JSON values with changed manifest bytes rejected')
    try:invoke(a.output/'legacy-prefix-on-g-rejected',Path(g['sources']['software']['path']),a.legacy_run/'pilot')
    except ValueError as e:assert 'frozen resume identity changed' in str(e),str(e)
    else:raise AssertionError('cross profile prefix resumed')
    assert not calls;checks.append('legacy65b prefix cannot resume on G source/boot/profile')
    # Actual exporter reparses every G fixture raw. Internal mode always stays0 board.
    result,_=analysis.read_collection(a.g_run/'continued/records.json',a.g_run/'continued/manifest.json',_fixture_mode=True)
    assert result['software_profile']==selected and result['source_board_cases']==0 and result['fixture_only'] and not result['campaign_complete']
    try:analysis.read_collection(a.g_run/'continued/records.json',a.g_run/'continued/manifest.json')
    except ValueError as e:assert 'fixture input cannot publish' in str(e),str(e)
    else:raise AssertionError('G profile let synthetic data publish')
    checks.append('G actual exporter binds profile and fresh raw, internal0 board; default publication still rejects fixture')
    legacy,_=analysis.read_collection(a.legacy_run/'continued/records.json',a.legacy_run/'continued/manifest.json',_fixture_mode=True)
    assert legacy['software_profile']['name']=='full_e3_65b_v1'
    for before,after in zip(legacy['rows'],result['rows']):
        for metric in before['statistics']:
            if metric!='command_wall_seconds':assert before['statistics'][metric]==after['statistics'][metric]
    assert legacy['paired_comparisons']==result['paired_comparisons']
    checks.append('legacy65b and G synthetic protocol arrays/stats/pairs identical except actual host wall timing and software identity')
    report={'status':'passed','fixture_only':True,'new_board_measurements':0,'count':len(checks),'checks':checks,
        'software_profile':selected,'source_sha256':{n:hashlib.sha256((Path(__file__).parent/n).read_bytes()).hexdigest() for n in
            ['collect_workload.py','analyze_workload.py','workload_software_profiles.py','verify_workload_profiles.py']}}
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'status':'passed','checks':len(checks),'new_board_measurements':0}))

if __name__=='__main__':main()
