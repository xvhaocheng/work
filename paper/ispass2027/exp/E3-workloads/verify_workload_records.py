#!/usr/bin/env python3
"""Synthetic UART/PATH/hist protocol checks built from real-main CLI fixtures."""
import argparse
import copy
import json
from pathlib import Path
import model_full_coverage as model
from workload_records import backend_evidence,geometry,validate,parse
HERE=Path(__file__).resolve().parent
KERNEL='6.5.0-rc2-ABI5-PROTOCOL-FIXTURE'
BOOT='00000000-0000-0000-0000-000000000005'


def fixture(record,closing_spill=True):
    record=copy.deepcopy(record);p,r=record['input'],record['result'];r['observer_sequence']=17
    g=geometry(p['victim_geometry'],p['victim_shape'],p['victim_size']);n=r['victim']['completed']
    transactions=model.prefix(g,n)['predicted_AXI_bursts_source_model'];has_a=p['aggressor_id'] is not None
    private_a=(5 if closing_spill else 4) if has_a else 0;leave_a=4 if has_a else 0
    cycles=4*n+100
    obs={'time':{'valid':1,'status':4,'v_did':10,'a_did':11 if has_a else 0xffffffff,
                 'cycles':cycles,'h':cycles-n+1,'gap':n-1,'hmax':100,'gapmax':1,'issued':n,'done':n,'batch_polled':1,'done_to_read':0},
         'axi_v':{'neither':0,'v_only':cycles//2 if has_a else cycles,'a_only':0,'both':cycles-cycles//2 if has_a else 0,
                  'ar':transactions,'aw':transactions,'rlast':transactions,'b':transactions,'ar_wait':0,'aw_wait':0,'start':0,'end':0},
         'axi_a':{'ar':private_a,'aw':private_a,'rlast':0,'b':0,'ar_wait':0,'aw_wait':0,'start':0,
                  'end':(private_a<<16)+private_a,'shared_ar':transactions+leave_a,'shared_aw':transactions+leave_a,'shared_ar_wait':0,'shared_aw_wait':0}}
    paths={}
    for role in 'va':
        for channel in 'rw':
            for stage in range(4):
                entered=transactions if role=='v' else private_a if stage==0 else leave_a
                full=transactions if role=='v' else leave_a if stage<3 else 0
                paths[role+channel+str(stage)]={'valid':1,'entered':entered,'full':full,'carry':0,
                    'sum':full,'max':int(full>0),'start':0,'end':entered-full,'area':entered}
    return {'record':record,'observer':obs,'path':paths,'sequence':17,'boot_id':BOOT}


def output(item):
    seq=item['sequence']
    lines=['KERNEL='+KERNEL,'BOOT='+BOOT,'TAINT=0','PRE_SEQ='+str(seq-1),json.dumps(item['record']),'CASE_EXIT=0']
    for part,value in item['observer'].items():lines.append(f'idma_batch_obs seq={seq} part={part} '+' '.join(f'{k}={v}' for k,v in value.items()))
    for key,value in item['path'].items():lines.append(f'idma_path_obs seq={seq} role={key[0]} channel={key[1]} stage={key[2]} '+' '.join(f'{k}={v}' for k,v in value.items()))
    lines.append(f'idma_hist_info seq={seq} valid=1 version=5 schema=2 bins=32 count_bits=32 time_bits=32 depth=64 overflow=0 raw=0x0040000020202002')
    for key,value in item['path'].items():
        counts=[0]*32;counts[1]=value['full']
        lines.append(f'idma_path_hist seq={seq} role={key[0]} channel={key[1]} stage={key[2]} valid=1 schema=1 bins=32 full={value["full"]} total={value["full"]} overflow=0 counts='+','.join(map(str,counts)))
    return '\n'.join(lines+['TAINT_AFTER=0'])+'\n'


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--CLI-report',type=Path,required=True)
    p.add_argument('--p0-backend',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False)
    backend=backend_evidence(a.p0_backend,HERE/'test-results/long-backend04-summary.json',HERE/'test-results/packet-backend01-summary.json')
    cases=json.loads(a.CLI_report.read_text())['cases'];checks=[];closing=None
    for case in cases:
        if case['expected_success'] and not case['record']['input']['caps_only']:
            item=fixture(case['record']);validate(item,backend);parsed=parse(output(item),KERNEL,backend)
            assert parsed['histogram']['info']['version']==5
            checks.append({'case':case['args'],'accepted':True})
            if item['record']['input']['aggressor_id'] is not None:closing=item
    assert closing is not None
    assert validate(closing,backend)['shared_minus_private_ar']==-1
    mutations=[
        ('wrong byte count',lambda x:x['record']['result']['victim'].__setitem__('completed_bytes',1)),
        ('mapped-as-active coverage',lambda x:x['record']['result']['victim_coverage'].__setitem__('measured_distinct_bytes',1)),
        ('wrong A completed prefix',lambda x:x['record']['result']['aggressor_coverage'].__setitem__('measured_distinct_pages',256)),
        ('wrong warmup union',lambda x:x['record']['result']['victim_coverage'].__setitem__('verified_union_bytes',0)),
        ('wrong V transaction count',lambda x:x['observer']['axi_v'].__setitem__('ar',2*x['record']['result']['victim']['completed']+1)),
        ('shared private boundary equality',lambda x:x['observer']['axi_a'].__setitem__('shared_ar',x['observer']['axi_a']['shared_ar']+1)),
        ('A private stock',lambda x:x['observer']['axi_a'].__setitem__('end',0)),
        ('PATH stock',lambda x:x['path']['ar0'].__setitem__('end',0)),
        ('quarantine',lambda x:x['record']['result']['aggressor'].__setitem__('quarantined',1)),
        ('no complete V warmup ring',lambda x:x['record']['input'].__setitem__('victim_warmup_operations',0)),
    ]
    for name,mutate in mutations:
        item=copy.deepcopy(closing);mutate(item)
        try:validate(item,backend)
        except ValueError as e:checks.append({'check':name,'rejection':str(e)})
        else:raise AssertionError('accepted '+name)
    raw=output(closing)
    variants={
        'taint':raw.replace('TAINT_AFTER=0','TAINT_AFTER=16384'),
        'softlockup taint before':raw.replace('TAINT=0','TAINT=16384'),
        'old info version':raw.replace('version=5 schema=2','version=4 schema=2'),
        'wrong depth':raw.replace('depth=64','depth=4'),
        'wrong info format':raw.replace('schema=2 bins=32','schema=1 bins=32'),
        'bad histogram total':raw.replace('total=4 overflow','total=5 overflow',1),
        'newer seq':raw.replace('TAINT_AFTER=0','idma_hist_info seq=18 valid=1\nTAINT_AFTER=0'),
        'missing stage':raw.replace('idma_path_hist seq=17 role=v channel=r stage=0','ignored_stage seq=17 role=v channel=r stage=0'),
        'returned seq':raw.replace('"observer_sequence": 17','"observer_sequence": 18'),
    }
    for name,text in variants.items():
        try:parse(text,KERNEL,backend)
        except ValueError as e:checks.append({'check':name,'rejection':str(e)})
        else:raise AssertionError('accepted '+name)
    report={'status':'passed','fixture_only':True,'new_board_measurements':0,'scope':'synthetic CLI/PATH/hist records; real finite backend evidence loader','checks':checks}
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':len(checks),'output':str(a.output)}))

if __name__=='__main__':main()
