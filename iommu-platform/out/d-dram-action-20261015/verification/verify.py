#!/usr/bin/env python3
"""Read-only streaming audit of D-DRAM-PREP-PROJECTION-1. Never launches models."""
import argparse, collections, copy, csv, hashlib, itertools, json, re, sys, time
from pathlib import Path
import yaml
BASE=Path(__file__).resolve().parent.parent
CASES=('p_row0','p_row1','pn_row0','pn_row1')
MASK=(1<<64)-1
class Invalid(Exception): pass
def need(ok,msg):
    if not ok: raise Invalid(msg)
def sha(path):
    h=hashlib.sha256()
    with Path(path).open('rb') as f:
        for b in iter(lambda:f.read(1024*1024),b''): h.update(b)
    return h.hexdigest()
def details(s): return dict(re.findall(r'(\w+)=([^;]*)',s))
def vec(s): return tuple(map(int,re.search(r'vec=([\d;\-]+)',s)[1].strip(';').split(';')))
def word(page,i): return ((((page<<9)|i)*0x9E3779B97F4A7C15)&MASK)^0xA5C3E10D2B4F9678
def expected(a):
    if 0x80000000<=a<0x80001000: return word(0,(a-0x80000000)//8)
    if 0x90000000<=a<0x90008000: return word(1+(a-0x90000000)//4096,((a-0x90000000)%4096)//8)
    fixed={0x10280:1,0x10290:0xA000,0x10298:0x8000000000000020,0x102c0:1,0x102d0:0xB000,0x102d8:0x8000000000000030,0x20000:0x10001,0x40400:0x14001,0x50000:0x200000df,0x50008:0x200004df,0x50010:0x200008df,0x30000:0x18001,0x60800:0x1c001}
    if 0x70000<=a<0x70080:
        n=(a-0x70000)//8; pa=(0x90000000+n*4096) if n<8 else (0x90100000+(n-8)*4096)
        return ((pa>>12)<<10)|0xdf
    return fixed.get(a,0)
def expected_w(a):
    if 0x80001000<=a<0x80002000:return word(0,(a-0x80001000)//8)
    if 0x90100000<=a<0x90108000:return word(1+(a-0x90100000)//4096,((a-0x90100000)%4096)//8)
    raise Invalid('W destination outside frozen COPY profile '+hex(a))
def encode(av,org):
    a=0;shift=6
    for i in (0,5,1,2,3,4):
        bits=(org[i]-1).bit_length()-(3 if i==5 else 0)
        need(av[i]>=0,'negative data command address');a|=av[i]<<shift;shift+=bits
    return a
class Audit:
    def __init__(self,run):
        self.run=Path(run);self.parents={};self.children={};self.building=collections.defaultdict(list)
        self.offers=collections.deque();self.rq=collections.deque();self.wq=collections.deque();self.bq=collections.deque()
        self.read_pending=collections.defaultdict(collections.deque);self.wr_pending=collections.defaultdict(list)
        self.rd_issues=collections.defaultdict(collections.deque);self.last_rd=None;self.last_wr=None;self.last_offer=None
        self.live=[0,0];self.credits=0;self.peaks=[0,0,0];self.count=collections.Counter();self.hwissue=[0,0];self.hwdone=[0,0]
        self.cfg_next=[0,0];self.cfg_done=[0,0];self.cfg_base=[None,None];self.last_parent=0;self.last_child=0;self.mutations={};self.phases={};self.functional={};self.post=False;self.line=0;self.now=0;self.b_before_recovery=0;self.max_recovery=-1
        self.hashes={k:hashlib.sha256() for k in ('O_phys','dram_commands','cfg','hardware','actual_R','actual_B')}
        cold=yaml.safe_load((self.run/'phase-cold.yaml').read_text());self.org=cold['dram']['organization'];self.nRFC=cold['dram']['nRFC'];self.nREFI=cold['dram']['nREFI']
        self.cap=[0]*32;self.rows=[None]*32;self.refresh={};self.maintenance=collections.Counter();self.cap_enqueues=collections.Counter();self.refresh_enqueues=collections.Counter();self.refresh_ends=collections.Counter()
        self.trace=(self.run/'official-trace.ch0').open();self.trace_rows=csv.reader(self.trace,skipinitialspace=True)
    def hashed(self,k,x):
        if self.post:self.hashes[k].update((json.dumps(x,separators=(',',':'))+'\n').encode())
    def child(self,c):need(c in self.children,'unknown child '+str(c));return self.children[c]
    def parent(self,p):need(p in self.parents,'unknown parent '+str(p));return self.parents[p]
    def fork(self):
        q=Audit.__new__(Audit);q.__dict__=self.__dict__.copy()
        for k in ('parents','children','building','offers','rq','wq','bq','read_pending','wr_pending','rd_issues','live','peaks','count','hwissue','hwdone','functional','phases','cfg_next','cfg_done','cfg_base'):
            q.__dict__[k]=copy.deepcopy(self.__dict__[k])
        q.hashes={k:h.copy() for k,h in self.hashes.items()};return q
    def reject_probe(self,name,row,change):
        if name in self.mutations:return
        q=self.fork();q.mutations=self.mutations.copy();q.mutations[name]={'probing':True};q.mutations['_B_probing']=True;change(q)
        try:q.step(row)
        except Invalid as e:self.mutations[name]={'rejected':True,'diagnostic':str(e),'source_line':self.line,'dram_tick':self.now}
        else:raise Invalid('negative mutation accepted: '+name)
    def step(self,r):
        self.line+=1;t=int(r['dram_tick']);self.now=t;e=r['event'];p=int(r['parent_uid']);uid=int(r['child_uid']);a=int(r['address']);v=int(r['value']);d=details(r['detail']);self.count[e]+=1
        if e=='B' and not self.mutations.get('_B_probing'):
            cs=self.parent(p)['children']
            self.reject_probe('B_at_WR_issue',r,lambda q:[q.children[c].update(wr=t,de=False) for c in cs])
            self.reject_probe('missing_WR_retains_B',r,lambda q:q.children[cs[0]].update(wr=None))
            self.reject_probe('missing_W_retains_B',r,lambda q:q.parents[p].update(wbeat=q.parents[p]['wbeat']-1))
        if e=='child_retire' and self.child(uid)['w'] and 'recovery_immature_retire' not in self.mutations:
            self.reject_probe('recovery_immature_retire',r,lambda q:q.children[uid].update(rec=False))
        if e=='child_create':
            need(uid>self.last_child and uid not in self.children and p not in self.parents,'duplicate/late child create');self.last_child=uid
            self.children[uid]={'p':p,'a':a,'w':v,'accepted':False,'queued':not v,'callback':False,'rd':None,'wr':None,'de':False,'rec':False}
            self.building[p].append(uid)
            if not v:self.offers.append(uid)
        elif e in ('AR','AW'):
            need(p>self.last_parent and p not in self.parents,'duplicate parent');self.last_parent=p;wr=e=='AW';ids=self.building.pop(p,[])
            need(a%8==0 and 0<=v<=255 and a//4096==(a+8*(v+1)-1)//4096,'illegal AXI address/len')
            # size/burst are source guard assertions; absent from raw AR/AW rows.
            if 'size' in d:need(int(d['size'])==3 and int(d['burst'])==1,'illegal AXI size/burst')
            want=list(range(a//64*64,(a+8*(v+1)-1)//64*64+1,64));need([self.child(c)['a'] for c in ids]==want,'per-parent line coverage')
            need(all(self.child(c)['p']==p and self.child(c)['w']==wr for c in ids),'cross-parent/type child merge')
            if wr:need(a%64==0 and (v+1)%8==0,'partial write scope')
            self.parents[p]={'w':wr,'a':a,'len':v,'id':int(d['id']),'children':ids,'wbeat':0,'rbeat':0,'returned':False,'history':False,'actual':None}
            self.live[wr]+=1;self.credits+=len(ids);self.peaks=[max(self.peaks[0],self.live[0]),max(self.peaks[1],self.live[1]),max(self.peaks[2],self.credits)]
            need(self.live[0]<=16 and self.live[1]<=16 and self.credits<=1024,'capacity exceeded')
            (self.wq if wr else self.rq).append(p)
        elif e=='history_read_discard':
            q=self.parent(p);need(not q['w'] and not q['returned'] and self.rq.popleft()==p,'history discard ownership');q['history']=q['returned']=True;self.live[0]-=1
        elif e=='W':
            q=self.parent(p);need(self.wq and self.wq[0]==p and q['w'],'W FIFO owner');i=q['wbeat']
            need(a==q['a']+i*8 and int(d['strb'])==255 and int(d['last'])==(i==q['len']),'W address/strobe/WLAST')
            need(v==expected_w(a),'W data mismatch');q['wbeat']+=1
            if q['wbeat']%8==0:
                c=q['children'][q['wbeat']//8-1];need(not self.child(c)['queued'],'duplicate W line');self.child(c)['queued']=True;self.offers.append(c)
            if i==q['len']:self.wq.popleft();self.bq.append(p)
        elif e=='offer':
            c=self.child(uid);need(self.offers and self.offers[0]==uid and not c['accepted'],'non-FIFO or repeated offer')
            need(self.last_offer is None or self.last_offer[0]!=t,'multiple offers in one tick');self.last_offer=(t,uid,v)
        elif e=='accepted':
            c=self.child(uid);need(self.last_offer==(t,uid,1) and not c['accepted'] and self.offers.popleft()==uid,'accepted without successful unique offer')
            need(v==c['w'] and a==c['a'] and p==c['p'],'accepted identity');c['accepted']=True;c['accept_tick']=t
            (self.wr_pending if c['w'] else self.read_pending)[a].append(uid)
            self.hashed('O_phys',[t,a,v])
        elif e=='callback':
            c=self.child(uid);need(not c['w'] and c['accepted'] and not c['callback'],'duplicate/invalid callback')
            need(self.rd_issues[a],'callback lacks actual RD');rd=self.rd_issues[a].popleft();need(v==rd+20 and t>=v and int(d['depart'])==v and int(d['arrive'])==c['accept_tick'],'callback actual RD+20/arrival timing')
            need(uid in self.read_pending[a],'callback parent child identity');self.read_pending[a].remove(uid);c['callback']=True;c['rd']=rd
            if 'duplicate_callback' not in self.mutations:self.reject_probe('duplicate_callback',r,lambda q:None)
        elif e=='actual_R_data':
            q=self.parent(p);need(not q['history'] and not q['w'] and q['actual'] is None,'actual R ownership');i=q['rbeat']
            need(a==q['a']+i*8 and v==expected(a) and int(d['id'])==q['id'] and int(d['last'])==(i==q['len']),'actual R data/ID/LAST/address')
            q['actual']=(t,a,v);self.hashed('actual_R',[int(r['rtl_tick']),t,a,v,q['id'],i==q['len']])
        elif e=='R':
            q=self.parent(p);need(self.rq and self.rq[0]==p and not q['w'] and not q['returned'],'R FIFO parent');i=q['rbeat'];c=self.child(q['children'][(a//64)-(q['a']//64)])
            need(v==i and a==q['a']+8*i and c['callback'] and q['actual'] and q['actual'][:2]==(t,a),'R beat/actual/callback');need(int(d['id'])==q['id'] and int(d['last'])==(i==q['len']),'R ID/LAST')
            q['actual']=None;q['rbeat']+=1
            if i==q['len']:q['returned']=True;self.rq.popleft();self.live[0]-=1
        elif e=='WR_issue':
            c=self.child(uid);need(c['w'] and c['accepted'] and c['wr'] is None and a==c['a'] and v==t and self.last_wr==(t,a),'WR unique actual identity')
            pending=self.wr_pending[a];need(pending==[uid],'WR address ambiguous or missing child');pending.remove(uid);c['wr']=t;self.max_recovery=max(self.max_recovery,t+34)
        elif e in ('derived_data_end','derived_recovery'):
            c=self.child(uid);rec=e=='derived_recovery';key='rec' if rec else 'de';off=34 if rec else 16
            need(c['wr'] is not None and not c[key] and t==v==c['wr']+off,'derived deadline without actual WR/exact time');c[key]=True
        elif e=='B':
            q=self.parent(p);need(q['w'] and not q['returned'] and self.bq and self.bq.popleft()==p,'duplicate/B FIFO parent');need(q['wbeat']==q['len']+1 and v==q['id'] and a==q['a'],'B missing WLAST or identity')
            cs=[self.child(c) for c in q['children']];need(all(c['wr'] is not None and c['de'] and t>=c['wr']+16 for c in cs),'B before actual WR data-end')
            self.b_before_recovery+=any(not c['rec'] for c in cs);q['returned']=True;self.live[1]-=1;self.hashed('actual_B',[int(r['rtl_tick']),t,a,v])
        elif e=='child_retire':
            c=self.child(uid);q=self.parent(p);need(q['returned'] and (c['rec'] if c['w'] else c['callback']),'retire before completion/recovery');del self.children[uid]
        elif e=='parent_retire':
            q=self.parent(p);need(q['returned'] and all(c not in self.children for c in q['children']),'parent retire before children');self.credits-=len(q['children'])
            need(v==len(q['children']) and int(d['credits'])==self.credits and int(d['ar_live'])==self.live[0] and int(d['aw_live'])==self.live[1],'retirement credits/live mismatch');del self.parents[p]
        elif e.startswith('dram_'): self.dram(r,e,t,d)
        elif e=='phase_common_reset_start':self.post=True
        elif e.startswith('phase_'):self.phases[e[6:]]={'tick':t,'rtl':int(r['rtl_tick']),'credits':v}
        elif e=='functional_queues':self.functional[d['label']]={k:int(z) for k,z in d.items() if k!='label'}
        elif e in ('hardware_issue','hardware_done'):
            target=self.hwissue if e=='hardware_issue' else self.hwdone
            for i in (0,1):target[i]+=bool(v&(1<<i))
            need(v&~3==0,'unexpected DMA hardware mask');self.hashed('hardware',[int(r['rtl_tick']),t,e,v])
        elif e in ('cfg_read','cfg_write'):
            need(d.get('error')=='0','cfg error');self.hashed('cfg',[int(r['rtl_tick']),t,e,p,a,v,d])
            if e=='cfg_read' and a==0x28:self.cfg_next[p]+=1
            if e=='cfg_read' and a==0x30:
                if self.cfg_base[p] is None:self.cfg_base[p]=v
                self.cfg_done[p]=max(self.cfg_done[p],v)
    def dram(self,r,e,t,d):
        av=vec(r['detail']);cmd=r['detail'].split(';')[0];clock=int(d['event_clock']);need(clock==t,'DRAM/controller clock mismatch')
        rank=av[1];flat=av[3]+av[2]*4+rank*16 if av[2]>=0 and av[3]>=0 else None
        if e=='dram_priority_send':
            need(d.get('accepted')=='1' and 'request_type' in d,'maintenance enqueue not certified')
            if flat is None:
                need(av[2:]==(-1,-1,-1,-1) and d['request_type']=='2','refresh rank address/type scope');self.refresh_enqueues[rank]+=1;self.maintenance[('ref',rank)]+=1
            else:
                need(d['request_type']=='4','cap PRE request type');self.cap_enqueues[(clock,flat)]+=1;self.maintenance[('cap',flat)]+=1
        elif e=='dram_issued':
            official=next(self.trace_rows,None);need(official is not None and int(official[0])==clock and official[1].strip()==cmd and tuple(map(int,official[2:]))==av,'official trace differs from actual issue')
            self.hashed('dram_commands',[clock,cmd,av])
            if cmd=='ACT':need(flat is not None and self.rows[flat] is None,'ACT on open bank');self.rows[flat]=av[4]
            elif cmd in ('RD','WR'):
                need(self.rows[flat]==av[4],'data command row mismatch');a=encode(av,self.org);self.cap[flat]+=1
                if self.cap[flat]>=4:need(self.cap_enqueues[(clock,flat)]==1,'cap4 lacks same-tick successful PRE enqueue');self.cap[flat]=0
                if cmd=='RD':need(self.read_pending[a],'RD lacks accepted read');self.rd_issues[a].append(clock)
                else:self.last_wr=(clock,a)
            elif cmd in ('PRE','PREA'):
                banks=[flat] if flat is not None else list(range(rank*16,rank*16+16))
                for b in banks:self.rows[b]=None;self.cap[b]=0
                if flat is not None and self.maintenance[('cap',flat)]:self.maintenance[('cap',flat)]-=1
            elif cmd=='REFab':
                need(self.maintenance[('ref',rank)]>0 and rank not in self.refresh,'REFab lacks enqueue/duplicate');self.maintenance[('ref',rank)]-=1;self.refresh[rank]=clock
                for b in range(rank*16,rank*16+16):self.rows[b]=None;self.cap[b]=0
        elif e=='dram_REFab_end':
            need(rank in self.refresh and clock==self.refresh.pop(rank)+self.nRFC-1,'REFab end deadline');self.refresh_ends[rank]+=1
    def finish(self):
        need(not self.parents and not self.children and not self.offers and not self.building and not self.rq and not self.wq and not self.bq and self.live==[0,0] and self.credits==0,'terminal active ownership')
        need(not any(self.read_pending.values()) and not any(self.wr_pending.values()) and not any(self.rd_issues.values()),'terminal missing actual service')
        need(next(self.trace_rows,None) is None,'official trace has unmatched tail');self.trace.close()
        need(not self.refresh and not any(self.maintenance.values()),'terminal maintenance/recovery debt')
        need(self.now>=self.max_recovery,'drain before WR recovery')
        for label in ('pre_t0_after_configs','strong_final_drain'):
            state=yaml.safe_load((self.run/f'phase-{label}.yaml').read_text());self.snapshot(state,label)
        f=self.functional.get('strong_final_drain',{});need(all(f.get(k)==0 for k in ('mem_q','caw_q','cb_q','bridge_b_uids','held_R','dbg_in_flight')) and f.get('dbg_dma_job_quiet')==3 and all(f.get(k)==1 for k in ('dbg_cpu_quiet','dbg_cpu_wr_quiet','rst_ni')),'functional/RTL strong quiet missing')
        need(self.count['full_warm_byte_checks_pass']==1 and self.count['phase_strong_final_drain']==1,'missing byte/terminal marker')
        stdout=(self.run/'stdout.log').read_text();need('PREPARATION-ONLY PASS' in stdout and 'no t0 or measurement launch executed' in stdout,'not preparation-only PASS')
        warm=re.search(r'WARM-SUMMARY[^\n]*',stdout);need(warm is not None,'missing warm summary');wd=details(warm[0].replace(' ',';'))
        counts={k:int(v) for k,v in wd.items() if v.isdigit()}
        need(counts.get('v_submitted')==256 and counts.get('v_completed')==256,'V warm count not256')
        need(counts.get('a_submitted')==counts.get('a_completed') and counts.get('a_completed',0)>0,'A feedback incomplete')
        need(self.hwissue==self.hwdone==self.cfg_next==[self.cfg_done[i]-self.cfg_base[i] for i in (0,1)]==[256,counts['a_completed']],'hardware/cfg NEXT/DONE mismatch')
        for key,got in (('hw_v_issue',256),('hw_v_done',256),('hw_a_issue',counts['a_completed']),('hw_a_done',counts['a_completed'])):need(counts.get(key)==got,'warm hardware summary mismatch '+key)
        receipt=json.loads((self.run/'run-receipt.json').read_text());need(receipt.get('exit_code')==0 and receipt.get('full_task_executed') is False,'failed/full task receipt')
        return {'status':'PASS','negative_mutations':self.mutations,'cfg_NEXT_counts':self.cfg_next,'cfg_DONE_baseline':self.cfg_base,'cfg_DONE_final':self.cfg_done,'rows_streamed':self.line,'event_counts':dict(self.count),'peaks_ar_aw_credits':self.peaks,'hardware_issues':self.hwissue,'hardware_done':self.hwdone,'warm_summary':counts,'B_after_data_end_before_recovery_count':self.b_before_recovery,'last_recovery':self.max_recovery,'last_event_tick':self.now,'refresh_enqueues':dict(self.refresh_enqueues),'refresh_completions':dict(self.refresh_ends),'normalized_sha256':{k:h.hexdigest() for k,h in self.hashes.items()},'raw_size_burst_evidence':'not logged; constrained by frozen accept() source guard, not independently observed raw fields','full_task_executed':False,'O_phys_does_not_establish_O_host_sufficiency':True}
    def snapshot(self,s,label):
        b=s['bridge'];now=b['dram_tick'];need(now>=self.max_recovery,'phase recovery immature')
        for k in ('parents','children','line_credits','offers','ar_live','aw_live'):need(b[k]==0,'snapshot outstanding '+k)
        need([b['max_ar_live'],b['max_aw_live'],b['max_credits']]==self.peaks,'peak accounting mismatch')
        for k in ('dram','controller_0','refresh_0'):need(s[k]['clock']==now,'phase clocks')
        need(all(s['controller_0'][k]==0 for k in ('read','write','active','priority','pending')),'provider five queues not empty')
        need(s['dram']['future_count']==0 and not s['dram']['future_actions'] and s['dram']['refreshing_banks']==0 and s['refresh_0']['next_refresh']>now,'future/refresh active')
        # Reconstruct at final event boundary only; pre_t0 can precede a final added wait.
        if label=='strong_final_drain':
            banks=[n for n in s['dram']['nodes'] if n['level']==3];need(len(banks)==32,'not full32bank snapshot')
            for n in banks:
                path=n['path'];i=path[3]+path[2]*4+path[1]*16;rows=n.get('rows') or []
                need(([z['row'] for z in rows] or [None])==[self.rows[i]],'snapshot row reconstruction mismatch')
            need(s['cap_0']['column_accesses']==self.cap,'snapshot cap reconstruction mismatch')
def rows(path):
    with Path(path).open(newline='') as f:yield from csv.DictReader(f)
def audit(run):
    a=Audit(run)
    try:
        for row in rows(Path(run)/'bridge-events.csv'):a.step(row)
        return a.finish()
    except Exception as e:
        return {'status':'FAIL','line':a.line,'dram_tick':a.now,'error':str(e),'exception':type(e).__name__,'event_counts':dict(a.count)}

def effective(s):
    now=s['dram']['clock'];return {'clock':now,'next_refresh':s['refresh_0']['next_refresh'],'cap':s['cap_0'],'future':s['dram']['future_actions'],'nodes':[{'path':n['path'],'state':n['state'],'rows':n['rows'],'effective_ready':[max(0,x-now) for x in n['command_ready_clock']]} for n in s['dram']['nodes']]}
def first_diff(a,b,path=''):
    if type(a)!=type(b):return {'path':path,'a':a,'b':b}
    if isinstance(a,dict):
        for k in sorted(a.keys()|b.keys()):
            if k not in a or k not in b:return {'path':path+'/'+str(k),'missing':True}
            z=first_diff(a[k],b[k],path+'/'+str(k))
            if z:return z
    elif isinstance(a,list):
        if len(a)!=len(b):return {'path':path,'lengths':[len(a),len(b)]}
        for i,(x,y) in enumerate(zip(a,b)):
            z=first_diff(x,y,path+'/'+str(i))
            if z:return z
    elif a!=b:return {'path':path,'a':a,'b':b}
    return None

def compare(base,x,y,reports):
    out={}
    for label in ('pre_t0_after_configs','strong_final_drain'):
        a=yaml.safe_load((base/x/f'phase-{label}.yaml').read_text());b=yaml.safe_load((base/y/f'phase-{label}.yaml').read_text())
        out[label]={'effective_first_difference':first_diff(effective(a),effective(b)),'raw_snapshot_first_difference':first_diff(a,b)}
    out['normalized_stream_equal']={k:reports[x]['normalized_sha256'][k]==reports[y]['normalized_sha256'][k] for k in reports[x].get('normalized_sha256',{})}
    # One bounded streaming paired pass only if a differing sequence needs its first witness.
    def physical(run):
        post=False
        for r in rows(run/'bridge-events.csv'):
            if r['event']=='phase_common_reset_start':post=True
            if post and r['event']=='accepted':yield [int(r['dram_tick']),int(r['address']),int(r['value'])]
    if out['normalized_stream_equal'].get('O_phys') is False:
        for i,(a,b) in enumerate(itertools.zip_longest(physical(base/x),physical(base/y))):
            if a!=b:out['first_O_phys_difference']={'index':i,'a':a,'b':b};break
    out['no_O_host_sufficiency_inference']=True
    return out

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--runs',type=Path,default=BASE/'implementation/runs');ap.add_argument('--output',type=Path,default=Path(__file__).resolve().parent/'report.json');ap.add_argument('--case',choices=CASES);args=ap.parse_args();start=time.monotonic()
    report={'material_passport':{'mode':'ARS bounded read-only raw ledger validation','requested_model':'gpt-6.1-sol','requested_effort':'medium','actual_model':'unknown','actual_effort':'unknown','DeepSeek_used':False},'contract_sha256':sha(BASE/'root/preparation-projection-contract.json'),'cases':{},'no_model_execution':True}
    for case in ((args.case,) if args.case else CASES):
        run=args.runs/case
        if not (run/'run-receipt.json').exists() or 'exit_code' not in json.loads((run/'run-receipt.json').read_text()):report['cases'][case]={'status':'PENDING'};continue
        report['cases'][case]=audit(run)
    if not args.case:
        for mode in ('p','pn'):
            x,y=mode+'_row0',mode+'_row1'
            if all(report['cases'][z]['status']=='PASS' for z in (x,y)):report.setdefault('comparisons',{})[mode]=compare(args.runs,x,y,report['cases'])
    report['duration_seconds']=time.monotonic()-start;report['checker_sha256']=sha(__file__);args.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v['status'] for k,v in report['cases'].items()}));return 1 if any(v['status']=='FAIL' for v in report['cases'].values()) else 0
if __name__=='__main__':sys.exit(main())
