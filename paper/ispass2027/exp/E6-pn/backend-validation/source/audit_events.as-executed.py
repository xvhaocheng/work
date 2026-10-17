#!/usr/bin/env python3
"""Independent raw-event byte/handshake/certificate accounting; no DUT rerun."""
import argparse
from collections import deque
import hashlib
import json
from pathlib import Path


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run',required=True,type=Path)
    parser.add_argument('--unit',required=True,type=Path)
    parser.add_argument('--elaboration',required=True,type=Path)
    parser.add_argument('--negative',required=True,type=Path)
    parser.add_argument('--output',required=True,type=Path)
    a=parser.parse_args();checks=0
    def require(ok,why):
        nonlocal checks
        if not ok:raise ValueError(why)
        checks+=1
    m=json.loads((a.run/'manifest.json').read_text())
    require(m['status']=='passed' and m['board_points']==0 and m['full_fpga_builds']==0,'finite-only final passed domain')
    for name,digest in m['inputs'].items():require(sha(a.run/name)==digest,'input changed '+name)
    require(m['inputs_unchanged'],'compiled inputs changed')
    for c in m['commands']:require(c['exit_code']==0 and sha(a.run/c['log'])==c['sha256'],'command/raw log changed')
    results=[]
    template=bytes(((i*37)^(i>>3)^(i>>11)^0xa5)&255 for i in range(1<<20))
    for enable in (0,1):
        raw=a.run/f'enable{enable}-run.log';cases={}
        for line_number,line in enumerate(raw.read_text().splitlines(),1):
            event=json.loads(line)
            if event['event']=='all_passed':continue
            cases.setdefault(event['case'],[]).append((line_number,event))
        require(len(cases)==(4 if enable==0 else 13),'fullwrap case count')
        for name,rows in cases.items():
            memory=bytearray(template);memory[0x40000:0x60000]=b'\xcc'*0x20000;expected=memory.copy()
            pending=deque();read_queue=deque();write_queue=deque();reply_ids=deque()
            jobs=[];ar=aw=rlast=b=rbytes=wbytes=retires=peak=permit_changes=0;permit=False
            summary=None;summary_line=None;plateau=None;capacity=None;last_cycle=None
            type_=1 if name=='N_ineligible_3' else 2 if name=='N_ineligible_4' else 0
            for lineno,e in rows:
                tag=e['event']
                if tag=='case_summary':summary=e;summary_line=lineno;continue
                if e['cycle']!=last_cycle:
                    peak=max(peak,max(0,rbytes-wbytes));last_cycle=e['cycle']
                x,y,z=e['x'],e['y'],e['z']
                if tag=='job_fifo_fire':
                    require(not pending,'serial accepted job overlap')
                    index=len(jobs)+1
                    n_selected=enable and name.startswith('N_') and not(name=='N_S_N_no_reset' and index==2)
                    eligible=bool(n_selected and type_==0 and x==4096 and y%4096==0 and z%4096==0)
                    job={'length':x,'src':y,'dst':z,'eligible':eligible,'type':type_,'fire_cycle':e['cycle']}
                    jobs.append(job);pending.append(job)
                    if type_==0:expected[z:z+x]=memory[y:y+x]
                    if type_==2:expected[z:z+x]=(0x8877665544332211).to_bytes(8,'little')*(x//8)
                elif tag=='retire':
                    require(bool(pending),'retire without prior real jobFIFO fire');pending.popleft();retires+=1
                    require(x==retires,'retirement count')
                elif tag=='permit':
                    require(bool(x)!=permit,'duplicate permit edge');permit=bool(x);permit_changes+=1
                    require(not permit or(bool(pending) and pending[0]['eligible']),'permission before/without eligible real frontend acceptance')
                    require(permit or not pending,'permission dropped before natural retire in a required legal trace')
                    require((y,z)==(len(jobs),retires),'permit bound to actual fire/retire counters')
                elif tag=='AR':
                    require(bool(pending),'AR without accepted real job')
                    require(x%8==0 and (x%2048)+(y+1)*8<=2048,'AR burst geometry')
                    read_queue.append([x,y+1,0,z]);ar+=1
                elif tag=='R':
                    require(bool(read_queue),'R without AR');head=read_queue[0]
                    require(x==head[0]+8*head[2] and z==int(head[2]+1==head[1]),'R address/RLAST order')
                    require(y==int.from_bytes(memory[x:x+8],'little'),'raw R data does not match actual memory image')
                    head[2]+=1;rbytes+=8
                    if z:read_queue.popleft();rlast+=1
                elif tag=='AW':
                    require(bool(pending),'AW without accepted real job')
                    require(x%8==0 and (x%2048)+(y+1)*8<=2048,'AW burst geometry')
                    write_queue.append([x,y+1,0,z]);aw+=1
                elif tag=='W':
                    require(bool(write_queue),'W without AW');head=write_queue[0]
                    require(x==head[0]+8*head[2] and (z>>8)==int(head[2]+1==head[1]),'W address/WLAST order')
                    for lane in range(8):
                        if z&(1<<lane):memory[x+lane]=(y>>(8*lane))&255;wbytes+=1
                    head[2]+=1
                    if z>>8:write_queue.popleft();reply_ids.append(head[3])
                elif tag=='B':
                    require(bool(reply_ids) and x==reply_ids.popleft(),'B without completed AW/W burst');b+=1
                elif tag=='AW_blocked_plateau':
                    plateau=e;require((x,y,z)==(2048,256,1) and rbytes==2048 and wbytes==0,'actual original AW coupling boundary')
                elif tag=='capacity_probe':
                    capacity=e;require((x,y,z)==(4096,512,1) and rbytes==4096 and wbytes==0,'actual 512byte-lane occupancy / 4096 bytes / no W')
                else:raise ValueError('unknown raw event '+tag)
            peak=max(peak,max(0,rbytes-wbytes))
            require(summary is not None,'missing actual summary')
            require(memory==expected,'raw full memory/untouched-guard reconstruction differs')
            require(not pending and not read_queue and not write_queue and not reply_ids and not permit,'raw drain debts remain')
            require(ar==rlast and aw==b and len(jobs)==retires,'raw request/return/retire conservation')
            observed={'job_fifo_fires':len(jobs),'retires':retires,'ar':ar,'aw':aw,'rlast':rlast,'b':b,
                      'read_bytes':rbytes,'write_bytes':wbytes,'peak_buffered_bytes':peak,'permit_edges':permit_changes}
            require(all(summary[k]==v for k,v in observed.items()),'cached summary differs from original raw events')
            exp_r=sum(j['length'] for j in jobs if type_!=2);exp_w=sum(j['length'] for j in jobs if type_!=1)
            require((rbytes,wbytes)==(exp_r,exp_w),'actual per-type total physical bytes')
            require(summary['backend_fires']==len(jobs) and summary['drained'] and summary['data_and_guards_equal'],'actual lifecycle result')
            if name=='N_fullbuffer4096_natural':require(plateau is not None and capacity is not None,'both distinct capacity phases required')
            results.append({'enable':enable,'case':name,'raw_summary_line':summary_line,'raw_log':str(raw.resolve()),
                            'raw_sha256':sha(raw),'cycles':summary['cycles'],'gold_comparisons':summary['gold_comparisons'],**observed})
    unit_path=a.unit/'manifest.json';unit=json.loads(unit_path.read_text())
    require(unit['status']=='passed' and unit['fullwrap_cases']==0 and unit['result']['checks']==121,'separate real certificate unit scope')
    for name,digest in unit['inputs'].items():require(sha(a.unit/name)==digest,'unit source changed')
    elaboration_path=a.elaboration/'manifest.json';elab=json.loads(elaboration_path.read_text())
    require(elab['status']=='passed' and [r['additional_backend_and_certificates_present'] for r in elab['results']]==[False,True], 'actual elaboration branch proof')
    for result in elab['results']:require(sha(a.elaboration/result['verbatim_cells'])==result['sha256'],'raw cells changed')
    negative_path=a.negative/'manifest.json';negative=json.loads(negative_path.read_text())
    require(negative['status']=='expected_negative_observed' and negative['actual_exit_code']==1 and negative['actual_event']['z']>0,
            'preserved original WRITE64 expected negative')
    require(sha(a.negative/'test.log')==negative['raw_log_sha256'],'negative raw changed')
    result={'schema':'pn-backend-raw-audit-v1','status':'passed','fixture_only':True,'board_points':0,'full_fpga_builds':0,
            'evidence_level':'finite_actual_fullwrap_raw_audit','checks':checks,'fullwrap_cases':len(results),
            'job_fifo_fires':sum(r['job_fifo_fires'] for r in results),'retires':sum(r['retires'] for r in results),
            'cycles':sum(r['cycles'] for r in results),'gold_comparisons':sum(r['gold_comparisons'] for r in results),
            'cases':results,'separate_certificate_unit':unit['result'],
            'input_manifests':[{'path':str(p.resolve()),'sha256':sha(p)} for p in (a.run/'manifest.json',unit_path,elaboration_path,negative_path)],
            'known_exclusions':{'WRITE64':negative['qualification'],'IOMMU_P_N_system':'Not instantiated by this wrapper-only harness.',
                'data_AXI_B_backpressure':'Actual data B READY never stalled in these serial traces (0 cycles); delayed B arrivals and held control B/R are covered. Do not claim a forced data B stall.',
                'RAM_fit_timing':'No synthesis or FPGA build.'}}
    with a.output.open('x') as f:f.write(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','checks','fullwrap_cases','job_fifo_fires','gold_comparisons')}))


if __name__=='__main__':main()
