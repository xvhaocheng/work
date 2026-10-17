#!/usr/bin/env python3
"""Finite synthetic E4+OBS4 protocol checks; no UART import or hardware IO."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import sys
import tempfile
import collect_pages as collector


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--e5-dir',type=Path,required=True)
    parser.add_argument('--fixture',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    _,histogram=collector.e5_modules(args.e5_dir)
    text=json.loads(args.fixture.read_text())['stdout'].replace('\r','')
    seq=int(re.search(r'^PRE_SEQ=(\d+)$',text,re.M)[1])+1
    boot=re.search(r'^BOOT=(.+)$',text,re.M)[1]
    obs='\n'.join(dict.fromkeys(re.findall(r'idma_(?:batch|path)_obs seq='+str(seq)+r'\b[^\n]+',
        text.split('\nCASE_EXIT=0\n',1)[1].split('\nTAINT_AFTER=',1)[0])))
    assert len(obs.splitlines())==19
    cases=json.loads((Path(__file__).parent/'pages-cli-cases.json').read_text())
    templates={r['output']['input']['mapping_page_bytes']:r['output'] for r in cases if r['name']=='success'}
    checks=[]

    def hist(lines,sequence):
        result=[f'idma_hist_info seq={sequence} valid=1 version=4 schema=1 bins=32 count_bits=32 time_bits=32 overflow=0 raw=0x20202001']
        for role,channel,stage,fields in re.findall(r'idma_path_obs seq=\d+ role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)',lines):
            p={k:int(v,0) for k,v in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)',fields)}
            counts=[0]*32;full=p['full']
            def add(value,n):
                if n:counts[value.bit_length() if value>=2 else value]+=n
            if full:
                add(p['max'],1)
                if full>1:
                    low,rem=divmod(p['sum']-p['max'],full-1)
                    assert low+(rem>0)<=p['max']
                    add(low,full-1-rem);add(low+1,rem)
                else:assert p['sum']==p['max']
            result.append(f'idma_path_hist seq={sequence} role={role} channel={channel} stage={stage} valid=1 schema=1 bins=32 full={full} total={full} overflow=0 counts='+','.join(map(str,counts)))
        return '\n'.join(result)

    def raw(condition='leaf4k',mutate=None,change_obs=None,sequence=seq):
        record=copy.deepcopy(templates[collector.CONDITIONS[condition]])
        record['result']['observer_sequence']=sequence
        if mutate:mutate(record)
        lines=obs.replace(f'obs seq={seq} ',f'obs seq={sequence} ')
        if change_obs:lines=change_obs(lines)
        stdout=(f'KERNEL={collector.KERNEL}\nBOOT={boot}\nTAINT=0\nPRE_SEQ={sequence-1}\n'+
                json.dumps(record)+'\nCASE_EXIT=0\n'+lines+'\n'+hist(lines,sequence)+'\nTAINT_AFTER=0\n')
        return {'fixture_only':True,'command':collector.command(condition),'exit_code':0,'stdout':stdout,'raw_path':'/HOST-FIXTURE-NO-BOARD/command.json'}

    def accept(name,value,condition='leaf4k'):
        item=collector.parse(value['stdout'],collector.KERNEL,condition,histogram)
        checks.append({'name':name,'expected':'accept','passed':True});return item

    def reject(name,value,condition='leaf4k'):
        try:collector.parse(value['stdout'],collector.KERNEL,condition,histogram)
        except (AssertionError,ValueError,KeyError,IndexError):
            checks.append({'name':name,'expected':'reject','passed':True})
        else:raise AssertionError(name+' accepted')

    for condition in collector.CONDITIONS:
        item=accept('full_'+condition,raw(condition),condition)
        assert len(item['histogram']['stages'])==16 and item['mapping_page_bytes']==collector.CONDITIONS[condition]
    reject('condition_relabel',raw('leaf2m'),'leaf4k')
    changes={
      'returned_input':lambda r:r['returned_input'].update(mapping_page_bytes=8192),
      'role':lambda r:r['result']['experiment'].update(aggressor_did=12),
      'wrong_footprint':lambda r:r['input'].update(buffer_bytes=4096),
      'wrong_work':lambda r:r['result']['experiment'].update(victim_pages=1),
      'data_failure':lambda r:r['result']['experiment']['victim'].update(data_errors=1),
      'bytes':lambda r:r['result']['experiment']['aggressor'].update(completed_bytes=1),
      'not_active':lambda r:r['result'].update(pre_stop_A_control=1),
      'target_exhausted':lambda r:r['result'].update(pre_stop_A_issued=2**32),
      'end_snapshot':lambda r:r['result']['experiment']['aggressor'].update(in_flight_at_end=0),
      'retained':lambda r:r['result'].update(retained_lane_mask=2),
      'partial_post':lambda r:r['result'].update(verified_after_mask=7),
      'wrong_leaf':lambda r:r['result']['buffers'][0].update(leaf_shift_min=21),
      'short_unmap':lambda r:r['result']['buffers'][1].update(unmapped_bytes=4096),
      'not_empty':lambda r:r['result']['buffers'][1].update(empty_after_cleanup=0),
      'partial_proof':lambda r:r['result']['buffers'][2].update(verified_before_4k_pages=511),
      'pte_PA_mismatch':lambda r:r['result']['buffers'][3].update(after_last_pte=7),
      'reserved':lambda r:r['result']['buffers'][3].update(reserved=1),
      'wrong_sequence':lambda r:r['result'].update(observer_sequence=seq-1),
    }
    for name,change in changes.items():reject(name,raw(mutate=change))
    def rewrite(value,old,new):
        value=copy.deepcopy(value);value['stdout']=value['stdout'].replace(old,new);return value
    base=raw()
    reject('taint',rewrite(base,'TAINT_AFTER=0','TAINT_AFTER=4096'))
    reject('missing_hist',rewrite(base,'idma_hist_info','deleted_hist_info'))
    reject('old_version',rewrite(base,'version=4 schema=1','version=3 schema=1'))
    reject('overflow',rewrite(base,'overflow=0 raw=','overflow=1 raw='))
    no_path=copy.deepcopy(base);no_path['stdout']=re.sub(r'^.*idma_path_obs[^\n]*\n?','',no_path['stdout'],flags=re.M)
    reject('missing_PATH',no_path)
    value=copy.deepcopy(base);value['stdout']=value['stdout'].replace('\nTAINT_AFTER=0\n','\nidma_batch_obs seq=9999 part=time valid=0\nTAINT_AFTER=0\n')
    reject('newer_sequence',value)
    def fields(line,changes):
        for key,delta in changes.items():
            line,n=re.subn(r'\b'+key+r'=(0x[0-9a-fA-F]+|\d+)',lambda m:key+'='+str(int(m[1],0)+delta),line)
            assert n==1
        return line
    def spill(lines):
        lines=re.sub(r'idma_batch_obs seq=\d+ part=axi_a [^\n]+',lambda m:fields(m[0],{'ar':1,'end':65536}),lines)
        return re.sub(r'idma_path_obs seq=\d+ role=a channel=r stage=0 [^\n]+',lambda m:fields(m[0],{'entered':1,'end':1,'area':1}),lines)
    spill_raw=raw(change_obs=spill)
    item=accept('closing_spill_all_conservation',spill_raw)
    assert item['metrics']['private_ar_signed_difference']==-1
    def bad_shared(lines):
        return re.sub(r'idma_batch_obs seq=\d+ part=axi_a [^\n]+',lambda m:fields(m[0],{'shared_ar':1}),spill(lines))
    reject('shared_tamper',raw(change_obs=bad_shared))
    records=[]
    for index,point in enumerate(collector.plan()):
        value=raw(point['condition'],sequence=seq+index)
        item=collector.bind(value,point,index,collector.KERNEL,boot,records[-1]['sequence'] if records else None,histogram)
        records.append(item)
    assert collector.review(records)['sample_count_complete']
    assert not collector.review(records[:-1])['sample_count_complete']
    changed=copy.deepcopy(records);changed[0]['condition']='leaf2m'
    assert not collector.review(changed)['sample_count_complete']
    changed=copy.deepcopy(records);changed[0]['block']=2
    assert not collector.review(changed)['sample_count_complete']
    checks.append({'name':'twenty_point_plan_count_prefix_and_condition_binding','passed':True})
    value=raw();value['command']=collector.command('leaf2m')
    try:collector.bind(value,collector.plan()[0],0,collector.KERNEL,boot,None,histogram)
    except AssertionError:checks.append({'name':'raw_command_relabel_rejected','passed':True})
    else:raise AssertionError('raw command mismatch accepted')
    with tempfile.TemporaryDirectory(prefix='e4-recovery-host-') as directory:
        directory=Path(directory);(directory/'uart').mkdir()
        manifest={'schema':'ispass2027.e4_pagesize.run.v1','planned_cases':collector.plan(),
                  'kernel_release':collector.KERNEL,'boot_id':boot,'ds':'0/0','software_profile':'e4-e5-v1'}
        for key in ['bit_sha256','load_manifest_sha256','source_record_sha256','software_manifest_sha256',
                    'fpga_manifest_sha256','package_manifest_sha256','script_sha256','histogram_validator_sha256',
                    'e5_identity_helper_sha256','software_profile_helper_sha256']:manifest[key]='HOST-FIXTURE-NO-BOARD'
        (directory/'manifest.json').write_text(json.dumps(manifest))
        paths=[]
        for index,point in enumerate(collector.plan()[:5]):
            value=raw(point['condition'],sequence=seq+index)
            path=directory/'uart'/f'command-{index}.json';path.write_text(json.dumps(value));paths.append(path)
        recovered,bound_paths=collector.recover_prefix(directory,manifest,histogram)
        assert len(recovered)==5 and len(bound_paths)==5 and recovered[-1]['case']==5
        checks.append({'name':'complete_same_boot_five_point_prefix_recovered_from_raw','passed':True})
        for kind in ['identity','profile','profile_helper','source','timeout','command','sequence','boot']:
            saved=paths[2].read_bytes();changed=json.loads(saved)
            target=copy.deepcopy(manifest)
            if kind=='identity':target['software_manifest_sha256']='wrong'
            if kind=='profile':target['software_profile']='unknown'
            if kind=='profile_helper':target['software_profile_helper_sha256']='wrong'
            if kind=='source':target['source_record_sha256']='wrong'
            if kind=='timeout':changed['exit_code']=None
            if kind=='command':changed['command']='wrong'
            if kind=='sequence':changed['stdout']=changed['stdout'].replace(f'PRE_SEQ={seq+1}',f'PRE_SEQ={seq+11}')
            if kind=='boot':changed['stdout']=changed['stdout'].replace(boot,'00000000-0000-0000-0000-000000000000')
            paths[2].write_text(json.dumps(changed))
            try:collector.recover_prefix(directory,target,histogram)
            except (AssertionError,ValueError,KeyError):checks.append({'name':'recovery_'+kind+'_rejected','passed':True})
            else:raise AssertionError('recovery '+kind+' accepted')
            paths[2].write_bytes(saved)
    assert 'uart' not in sys.modules, 'host tests imported UART'
    checks.append({'name':'plan_parser_checks_do_not_import_UART','passed':True})
    fixture=args.output.with_name('pages-collector-fixtures.json')
    fixture.write_text(json.dumps({'fixture_only':True,'scope':'Synthetic ABI4+existing OBS data+constructed OBS4 bins; never board measurements',
        'leaf4k':raw(),'leaf2m':raw('leaf2m'),'closing_spill':spill_raw},indent=2)+'\n')
    args.output.write_text(json.dumps({'schema':'ispass2027.e4_pagesize.collector_host.v1','status':'passed','fixture_only':True,
        'board_cases':0,'synthetic_campaign_points':20,'collector_sha256':hashlib.sha256(Path(collector.__file__).read_bytes()).hexdigest(),
        'source_fixture':str(args.fixture.resolve()),'source_fixture_sha256':hashlib.sha256(args.fixture.read_bytes()).hexdigest(),
        'histogram_validator_sha256':hashlib.sha256((args.e5_dir/'hist_records.py').read_bytes()).hexdigest(),
        'fixtures':str(fixture.resolve()),'checks':checks},indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':len(checks),'synthetic_campaign_points':20,'board_cases':0}))


if __name__=='__main__':main()
