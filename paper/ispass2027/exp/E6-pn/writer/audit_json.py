#!/usr/bin/env python3
"""Reconstruct snapshot presence and bounded accesses from actual fixture JSON."""
import argparse
import hashlib
import json
from pathlib import Path

CAP=0x49444d41504e0001
GAPCAP=0x49444d4147415001
HISTORY=1<<63
FIELDS=['observer_word','cap_word','mode_word','status_word','gap_word','gap_cap_word']
CHECKS=0

def check(ok,why):
    global CHECKS
    CHECKS+=1
    if not ok:raise AssertionError(why)

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--run',type=Path,required=True);p.add_argument('--out',type=Path,required=True);a=p.parse_args()
    if a.out.exists():raise FileExistsError(a.out)
    m=json.loads((a.run/'manifest.json').read_text());check(m['status']=='passed','actual accepted run')
    normal=json.loads((a.run/'host-execution.json').read_text());san=json.loads((a.run/'host-sanitized-execution.json').read_text())
    check(normal['exit_code']==san['exit_code']==0,'both actual C executions exit0');check(normal['stdout']==san['stdout'],'native and sanitized raw JSON equal');check(not san['stderr'],'sanitizer diagnostics empty')
    rows=[json.loads(line) for line in normal['stdout'].splitlines()];good=bad=0;transitions=set();gap_modes=set()
    for row in rows[:-1]:
        if 'result' not in row:
            check(row['case']=='actual_production_accessor_whitelists_and_faults' and row['fixture_only'] and row['source_board_cases']==0,'separate accessor unit');continue
        r=row['result'];trace=row.get('trace',[]);phase='before';selected=None;count=0;index_count=0;recovered={n:{key:None for key in FIELDS} for n in ['before','after']}
        check(r['fixture_only'] and r['synthetic'] and not r['read_only'] and r['source_board_cases']==0,'fixture must not become board data')
        for i,e in enumerate(trace):
            kind=e['type'];offset=e['offset'];error=e['error'];check(kind in ['R','I','M','G'],'operation whitelist')
            if kind=='I':
                index_count+=1;check(offset==0x70 and e['value'] in [192,193,194],'INDEX store whitelist')
                if not error:selected=e['value']
            elif kind in ['M','G']:
                count+=1;check(count==1,'at most one functional attempt');check(phase=='before','no retry or rollback')
                before=r['before'];check(all(before['present'].values()),'full obtained precondition snapshot')
                check(before['cap_word']==CAP and before['gap_cap_word']==GAPCAP,'capabilities before write')
                check(before['mode_word'] in [0,1,2,3,6] and (before['mode_word']!=6 or before['status_word']&HISTORY),'full current mode/support')
                s=before['status_word'];check(s&0x11==0x11 and not s&0x2c and not s&~(HISTORY|0x3f),'hardware eligibility/quiet/faults/reserved')
                check(before['observer_word']&3==0 and before['gap_word']>>32==0,'observer idle/gap width')
                if kind=='M':
                    check(offset==0x78 and e['value'] in [0,1,2,3,6],'MODE store whitelist')
                    check(i>=2 and trace[i-2]['type']=='I' and trace[i-2]['value']==193 and not trace[i-2]['error'],'mode explicit selector')
                    check(trace[i-1]['type']=='R' and trace[i-1]['offset']==0x70 and trace[i-1]['value']==193 and not trace[i-1]['error'],'mode selector actual readback')
                    check(e['value']!=6 or s&HISTORY,'requested shared history actually supported')
                else:check(offset==0xb0 and e['value'] in [0,1000,4000],'GAP store whitelist')
                phase='after'
            else:
                check(offset in [0x60,0x70,0x78,0xb0,0xb8],'load whitelist')
                if not error:
                    key={0x60:'observer_word',0xb0:'gap_word',0xb8:'gap_cap_word'}.get(offset)
                    if offset==0x78:key={192:'cap_word',193:'mode_word',194:'status_word'}[selected]
                    if key:recovered[phase][key]=e['value']
            if error:check(i==len(trace)-1 and not r['success'],'fault stops immediately without another MMIO')
        check(count==r['functional_write_attempts'] and index_count==r['index_selector_write_attempts'],'reported attempt counts are actual calls')
        for n in ['before','after']:
            for key,value in recovered[n].items():
                check(r[n][key]==value and r[n]['present'][key]==(value is not None),'all obtained raw words/presence preserved')
        if r['success']:
            good+=1;check(count==1 and r['readback_verified'] and r['errno']==0,'success is verified single attempt')
            before=r['before'];after=r['after'];check(after['cap_word']==before['cap_word']==CAP and after['gap_cap_word']==before['gap_cap_word']==GAPCAP,'unchanged capabilities')
            check((after['status_word']^before['status_word'])&HISTORY==0,'unchanged history support')
            if r['operation']=='set-mode':
                check(after['mode_word']==r['requested_value'] and not after['status_word']&2,'mode exact readback/accept status');check(after['gap_word']==before['gap_word'],'MODE preserves gap');transitions.add((before['mode_word'],after['mode_word']))
            else:
                check(after['gap_word']==r['requested_value'] and after['mode_word']==before['mode_word'],'GAP preserves full193');check((after['status_word']^before['status_word'])&2==0,'GAP does not own MODE reject latch');gap_modes.add((before['mode_word'],after['gap_word']))
        else:bad+=1;check(not r['readback_verified'] and r['errno']!=0,'all errors remain failure')
    check(transitions=={(a,b) for a in [0,1,2,3,6] for b in [0,1,2,3,6]},'all25 full-mode transitions including same and6')
    check(gap_modes=={(a,b) for a in [0,1,2,3,6] for b in [0,1000,4000]},'all allowed gap/full-mode combinations')
    receipt={'schema':1,'status':'passed','checks':CHECKS,'fixture_only':True,'source_board_cases':0,'run':str(a.run.resolve()),'manifest_sha256':hashlib.sha256((a.run/'manifest.json').read_bytes()).hexdigest(),'actual_json_cases':len(rows)-1,'successful_configuration_cases':good,'failed_configuration_cases':bad,'mode_transitions':sorted(transitions),'gap_modes':sorted(gap_modes),'scope':'read-only replay of actual native/sanitized C JSON; no new execution or MMIO'}
    a.out.write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({'status':receipt['status'],'checks':CHECKS,'successes':good,'failures':bad}))

if __name__=='__main__':main()
