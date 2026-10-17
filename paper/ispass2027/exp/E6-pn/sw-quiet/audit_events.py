#!/usr/bin/env python3
"""Read-only independent replay of required-domain raw SW/gold/AXI evidence."""
import argparse
from collections import Counter, deque
import hashlib
import json
from pathlib import Path
import re

CHECKS=0

def check(value,label):
    global CHECKS
    CHECKS+=1
    if not value:raise AssertionError(label)

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def audit_run(root,run,omitted):
    manifest=json.loads((run/'manifest.json').read_text())
    check(manifest['status']=='passed_finite_sw_only',str(run)+': status')
    check(manifest['board_trials']==0,'no board trials')
    for entry in manifest['inputs']:
        p=run/entry['path'];check(p.is_file() and sha(p)==entry['sha256'],'frozen input '+entry['path'])
    for name in ['build.log','run.log','events.jsonl']:
        check(sha(run/name)==manifest[name]['sha256'],'raw log SHA '+name)
    names={Path(e['path']).stem for e in json.loads((root/'source-map-v2.json').read_text())['files']}
    for rel in json.loads((root/'source-map-v2.json').read_text())['files']:
        original=(root/rel['snapshot']).read_bytes();stem=Path(rel['path']).stem
        transformed,n=re.subn(rb'(?m)^module '+stem.encode()+rb'\b',b'module '+stem.encode()+b'_gold',original,count=1);check(n==1,'gold declaration '+stem)
        for child in sorted(names):transformed=re.sub(rb'\b'+child.encode()+rb'(\s*#\()',child.encode()+b'_gold'+rb'\1',transformed)
        check(transformed==(run/'source'/(stem+'_gold.sv')).read_bytes(),'independent original gold recovery '+stem)
    source_check=json.loads((root/'functional-source-check01.json').read_text())
    for item in source_check['files']:
        check(sha(run/'source'/Path(item['path']).name)==item['candidate_sha256'],'actual tested candidate SHA '+item['path'])
    cases={};active=None;summary=None
    with (run/'events.jsonl').open() as stream:
        for lineno,line in enumerate(stream,1):
            row=json.loads(line);event=row['event']
            if event=='case':
                check(row['name'] not in cases,'unique case');active=row['name']
                cases[active]={'samples':{},'counts':[Counter() for _ in range(4)],'reads':[deque() for _ in range(4)],'writes':[deque() for _ in range(4)],'replies':[deque() for _ in range(4)],'bytes':{},'pass':False}
                continue
            if event=='summary':summary=row;continue
            name=row.get('case',row.get('name'));check(name==active,'sequential immutable case log')
            c=cases[name]
            if event=='case_pass':
                check(not c['pass'],'single case completion');c['pass']=True;c['cycles']=row['cycles'];continue
            if event=='sample':
                key=(row['cycle'],row['phase']);check(key not in c['samples'],'unique sample');c['samples'][key]=row
                check(row['eq']==3,'DUT equality flags')
                check(int(row['old_on'],16)==int(row['old_off'],16)==int(row['old_gold'],16),'all raw original outputs bit-exact')
                check(int(row['old_gold'],16).bit_length()<=8192,'all output bits fit frozen width')
                check(row['sw']&2==0 and row['prog']&2==0,'disabled taps0')
                if row['reset']:
                    check(row['sw']==0 and row['prog']==0,'reset has no eligibility');continue
                predicted=any(((row['irq_update_de_d']>>(2*k))&3)==3 and not(row['irq']&(1<<k)) for k in range(3)) and bool(row['msi_enabled'])
                check(bool(row['irq_predict'])==predicted,'known next-edge IRQ prediction')
                if predicted:check(row['sw']==0,'known queued MSI cannot be quiet')
                if row['ddtp_pending']:check(row['sw']==0 and row['regmap_idle']==0,'deferred DDTP remains busy')
                if row['bridge'] or row['bridge_valids'] or row['penable'] or row['reg_valid']:check(row['prog']==0,'current/buffered program transaction')
                if row['cq'] or row['cqh']!=row['cqt']:check(not(row['child']&1),'CQ main/queue pending')
                if row['fault_inputs'] or row['fault_spill'] or row['fault_fifo']:check(not(row['child']&2),'real stored/offered fault')
                if row['fq']:check(not(row['child']&4),'FQ write/ERROR')
                if row['pending'] or row['msi']:check(not(row['child']&16),'MSI pending/FSM')
                if row['go'] or row['debug'] or row['debug_offer']:check(not(row['child']&32),'debug GO/held/ongoing')
                if row['sw']:
                    check(row['child']==63 and row['regmap_idle']==1 and row['reg_valid']==0 and not predicted,'all SW child facts')
                if omitted:check(row['child']&48==48 and row['msi_enabled']==0,'omitted MSI/debug idle identities')
                continue
            if event=='program':continue # Runtime C++ separately checks actual returned ID/RESP and stalled R/B.
            check(event in ['AR','AW','R','W','B'],'known raw event')
            port=row['port'];check(0<=port<4,'actual SW memory port');c['counts'][port][event]+=1
            if event in ['AR','AW']:
                check(row['size']<=3,'legal memory data width');request=dict(row,index=0)
                c['reads' if event=='AR' else 'writes'][port].append(request)
            elif event=='R':
                check(bool(c['reads'][port]),'R has real AR');q=c['reads'][port][0]
                check(row['id']==q['id'] and row['last']==int(q['index']==q['len']),'read ID/length/RLAST')
                q['index']+=1
                if row['last']:c['reads'][port].popleft()
            elif event=='W':
                check(bool(c['writes'][port]),'W has real AW');q=c['writes'][port][0]
                check(row['last']==int(q['index']==q['len']),'write WLAST at actual AWLEN')
                check(row['addr']==q['addr']+(q['index']<<q['size']),'actual memory address')
                for lane in range(8):
                    if row['strb']&(1<<lane):c['bytes'][(row['addr']&~7)+lane]=(row['data']>>(8*lane))&255
                q['index']+=1
                if row['last']:c['replies'][port].append(q['id']);c['writes'][port].popleft()
            elif event=='B':
                check(bool(c['replies'][port]),'B only after complete real W burst');check(row['id']==c['replies'][port].popleft(),'real B ID')
    check(summary is not None and summary['status']=='passed','complete raw summary')
    sample_count=0;cycle_count=0;out=[]
    for name,c in cases.items():
        check(c['pass'],'all declared cases finish');n=c['cycles'];cycle_count+=n;sample_count+=len(c['samples'])
        check(set(c['samples'])=={(i,phase) for i in range(n) for phase in ['pre','post']}|{(n,'final')},'complete pre/post/final samples')
        check(c['samples'][(n,'final')]['sw']==1 and c['samples'][(n,'final')]['prog']==1,'natural final quiet')
        check(all(not q for family in ['reads','writes','replies'] for q in c[family]),'no protocol debt at final')
        counts=c['counts']
        if name=='fault_spills_fifo_fq_b_then_unread_ring':check(counts[1]['AW']==12 and counts[1]['W']==48 and counts[1]['B']==12,'twelve real FQ records')
        if name.startswith('msi_'):check(counts[3]['AW']==1 and counts[3]['W']==1 and counts[3]['B']==1,'one real MSI write retired')
        if name=='msi_error_fault_real_fq_chain':check(counts[1]['AW']==2 and counts[1]['B']==2,'MSI error really writes second FQ record')
        out.append({'name':name,'cycles':n,'sample_rows':len(c['samples']),'memory_counts':[dict(v) for v in counts]})
    check(summary['cases']==len(cases) and summary['cycles']==cycle_count,'actual summary counts')
    check(summary['gold_comparisons']==2*sample_count,'Pn0/Pn1 comparison count from raw samples')
    return {'path':str(run),'cases':out,'sample_rows':sample_count,'cycles':cycle_count,'gold_comparisons':2*sample_count,'events_sha256':sha(run/'events.jsonl')},cases

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--root',type=Path,required=True);ap.add_argument('--out',type=Path,required=True);a=ap.parse_args();root=a.root.resolve()
    if a.out.exists():raise FileExistsError(a.out)
    receipt={'scope':'actual finite SW only; independent raw bit/AXI replay and source binding; zero new RTL/board executions','status':'running','runs':[]}
    try:
        main_run,main_cases=audit_run(root,root/'runs/run06-target',False);receipt['runs'].append(main_run)
        omitted_run,_=audit_run(root,root/'runs/run07-omitted',True);receipt['runs'].append(omitted_run)
        before={}
        for line in (root/'runs/run05-before-irq-fix/events.jsonl').open():
            row=json.loads(line)
            if row['event']=='sample':before[(row['cycle'],row['phase'])]=row
        after=main_cases['msi_error_fault_real_fq_chain']['samples'];differences=[]
        for key,old in before.items():
            new=after[key];check(old['old_on']==new['old_on'] and old['old_off']==new['old_off'] and old['old_gold']==new['old_gold'],'before/after prediction original outputs identical')
            if old['sw']!=new['sw']:differences.append({'cycle':key[0],'phase':key[1],'old_idle':old['sw'],'new_idle':new['sw'],'irq':new['irq'],'irq_update_de_d':new['irq_update_de_d'],'predicted':new['irq_predict']})
        check(any(d['cycle']==50 and d['phase']=='pre' and d['old_idle']==1 and d['new_idle']==0 for d in differences),'actual original IRQ gap closed')
        receipt['before_after_irq_differences']=differences;receipt['status']='passed';receipt['checks']=CHECKS
    except Exception as exc:
        receipt['status']='failed';receipt['error']=repr(exc);receipt['checks']=CHECKS;raise
    finally:
        a.out.write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({'status':receipt['status'],'checks':CHECKS,'out':str(a.out)}))

if __name__=='__main__':main()
