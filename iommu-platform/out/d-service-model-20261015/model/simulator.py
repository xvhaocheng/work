#!/usr/bin/env python3
"""Synthetic D-CS1 finite closed-loop service model. stdlib only."""
import json, gzip, hashlib, copy, time, traceback
from pathlib import Path
BASE=Path(__file__).resolve().parent
CONTRACT=json.loads((BASE.parent/'root/service-contract.json').read_text())
HASH=hashlib.sha256((BASE.parent/'root/service-contract.json').read_bytes()).hexdigest()
RANK={'PTE_R':0,'PAYLOAD_W':1,'CQ_W':2,'POLL_R':3}

def prehistory(k,r):
    h=t=0
    for _ in range(k):
        d=1+h//8; t+=d; h=min(64,max(0,h-d)+16)
    return max(0,h-r),t+r

class Device:
    """Provider API receives commands/time only; no mode, TL state, policy or Z."""
    def __init__(self,h,base): self.h=h; self.base=base; self.queue=[]; self.active=None
    def accept(self,key,t): self.queue.append((t,key))
    def cool(self): self.h=max(0,self.h-1)
    def finish(self,t):
        if self.active and self.active[0]==t:
            _,key=self.active; self.active=None; self.h=min(64,self.h+16); return key
    def dispatch(self,t):
        if not self.active and self.queue:
            _,key=self.queue.pop(0); d=self.base+self.h//8; self.active=(t+d,key)
            return key,d,self.h

class Sim:
    def __init__(self,cfg,h,mode='P_M',N=4):
        self.cfg=cfg; self.mode=mode; self.N=N; self.t=0; self.events=[]; self.public=[]
        self.device=Device(h,cfg['base_d']); self.cache=[[],[]]; self.cap={2:[1,1],3:[2,1],4:[2,2]}[cfg['C']]
        self.sub=[0,0]; self.succ=[0,0]; self.gen=[[0,0],[0,0]]; self.live={}; self.free=set(); self.head=[[],[]]
        self.normal=None; self.probe=None; self.rejected=set(); self.resp=None; self.resp_rr='slow'; self.normal_rr=0
        self.mem={'RW':[]} if cfg['topology']=='coupled' else {'R':[],'W':[]}; self.active={s:None for s in self.mem}; self.pending=[]
        self.cpu=None; self.cq=[[],[]]; self.poll_rr=0; self.initial=[(o,b) for o in range(2) for b in range(2)]
        self.z=[0,0,0]; self.overflow=[False]*3; self.window=False; self.maxocc={s:0 for s in self.mem}; self.final_callback=[None,None]
    def ev(self,event,key=None,public=False,**kw):
        e={'t':self.t,'event':event,**kw}
        if key is not None: e['key']=list(key)
        self.events.append(e)
        if public: self.public.append(e.copy())
    def advance(self,n):
        for _ in range(n): self.device.cool(); self.t+=1
    def cost(self,event,n=1,**kw): self.ev(event,public=True,cost=n,**kw); self.advance(n)
    def touch(self,o,b):
        a=self.cache[o]
        if b in a:a.remove(b)
        a.append(b)
        while len(a)>self.cap[o]:a.pop(0)
    def setup(self):
        self.ev('invocation_start',public=True,mode=self.mode,N=self.N)
        for o in range(2):
            for b in range(2):
                self.cost('buffer_allocate',owner=o,task=b,iova=0x100000+o*0x10000+b*4096)
                self.cost('map_prepare',owner=o,task=b,bytes=4096,w=True,immutable=True)
        for o in range(2):self.cost('queue_setup',owner=o,Q=2)
        self.cost('iotlb_clear');self.cache=[[],[]]
        for o,b in [(0,0),(1,0),(0,1),(1,1)]:
            self.ev('warm_begin',public=True,owner=o,task=b)
            self.cost('warm_lookup',owner=o,task=b)
            if b not in self.cache[o]:
                for p in range(3):self.cost('warm_PTE_R',owner=o,task=b,phase=p)
                self.touch(o,b)
            else:self.touch(o,b)
            self.cost('warm_response',owner=o,task=b)
        self.ev('warm_end',public=True,cache=copy.deepcopy(self.cache))
    def server(self,kind):return 'RW' if 'RW' in self.mem else ('R' if kind.endswith('_R') else 'W')
    def enqueue(self,kind,key,phase=0):
        self.pending.append({'ready':self.t,'kind':kind,'key':key,'phase':phase,'work':self.cfg['W'] if kind=='PAYLOAD_W' else 1})
        self.ev('memory_ready',key,kind=kind,phase=phase)
    def keyorder(self,j):return (j['ready'],RANK[j['kind']],*j['key'],j['phase'])
    def accept_pending(self):
        keep=[]
        for j in sorted(self.pending,key=self.keyorder):
            s=self.server(j['kind']); count=len(self.mem[s])+int(self.active[s] is not None)
            if count>=8:keep.append(j);continue
            self.mem[s].append(j);self.ev('memory_accept',j['key'],kind=j['kind'],phase=j['phase'],server=s,occupancy=count+1)
            if j['kind']=='PAYLOAD_W':
                o=j['key'][0];assert self.head[o][0]==j['key'];self.head[o].pop(0)
                self.ev('AW_accept',j['key'])
            self.maxocc[s]=max(self.maxocc[s],count+1)
        self.pending=keep
    def submit(self,o,b):
        assert self.sub[o]<self.N and (o,b) not in [(k[0],k[1]) for k in self.live]
        self.gen[o][b]+=1;k=(o,b,self.gen[o][b]);lba=self.sub[o];self.sub[o]+=1;self.live[k]='device';self.window=True
        self.device.accept(k,self.t);self.ev('submit',k,public=True,lba=lba,iova=0x100000+o*0x10000+b*4096,sealed=self.sub[o]==self.N,status=0)
    def cpu_finish(self,kind,k):
        if kind=='submit':self.submit(k[0],k[1]);self.cpu=None
        elif kind=='callback':
            o,b,g=k;self.succ[o]+=1;self.final_callback[o]=self.t;self.ev('callback',k,public=True,status=0)
            del self.live[k]
            self.cpu=(self.t+1,'submit' if self.sub[o]<self.N else 'free',k)
            if self.sub[o]<self.N:self.ev('submit_begin',k,public=True)
            else:self.ev('free_begin',k,public=True)
        elif kind=='free':self.free.add(k[:2]);self.ev('free',k,public=True);self.cpu=None
        elif kind=='gap':self.cpu=None
    def finish_mem(self,j):
        k=j['key'];kind=j['kind'];p=j['phase'];self.ev('memory_finish',k,kind=kind,phase=p,server=self.server(kind))
        if kind=='PTE_R':
            assert self.normal and self.normal['key']==k
            if p<2:self.enqueue(kind,k,p+1)
            else:
                self.touch(k[0],k[1]);self.normal['state']='ready';self.ev('walk_complete',k)
        elif kind=='PAYLOAD_W':self.live[k]='payload_done';self.ev('payload_done',k);self.enqueue('CQ_W',k)
        elif kind=='CQ_W':self.live[k]='cq';self.cq[k[0]].append(k);self.ev('CQ_visible',k)
        else:
            o=k[0];item=self.cq[o].pop(0) if self.cq[o] else None
            self.ev('poll',item,public=True,owner=o,empty=item is None,status=None if item is None else 0)
            if item:
                self.ev('CQ_observe',item,public=True,status=0);self.cpu=(self.t+1,'callback',item);self.ev('callback_begin',item,public=True)
            else:self.cpu=(self.t+1,'gap',k)
    def tick(self):
        assert self.t<20000,'tick guard'
        # Completions: all writes, then all reads. Snapshot category before callbacks enqueue more work.
        done=[(s,a[1]) for s,a in self.active.items() if a and a[0]==self.t]
        for s,j in sorted(done,key=lambda x:(x[1]['kind'].endswith('_R'),self.keyorder(x[1]))):
            self.active[s]=None;self.finish_mem(j)
        k=self.device.finish(self.t)
        if k:self.head[k[0]].append(k);self.live[k]='translation';self.ev('device_finish',k,h=self.device.h)
        if self.normal and self.normal['state']=='lookup' and self.normal['end']==self.t:
            k=self.normal['key'];hit=k[1] in self.cache[k[0]];self.ev('lookup_result',k,hit=hit)
            if hit:self.touch(k[0],k[1]);self.normal['state']='ready'
            else:self.normal['state']='walk';self.enqueue('PTE_R',k)
        if self.probe and self.probe.get('end')==self.t:
            p=self.probe;k=p['key']
            if p['state']=='DC':p['state']='LEAF';p['end']=self.t+1;self.ev('probe_leaf_begin',k)
            elif p['state']=='LEAF':
                hit=k[1] in self.cache[k[0]];self.ev('probe_check',k,hit=hit,permission=hit)
                if hit:self.touch(k[0],k[1]);p['state']='ready';p['end']=None
                else:self.rejected.add(k);self.probe=None
        if self.resp and self.resp['end']==self.t:
            r=self.resp;k=r['key'];self.ev('response_finish',k,path=r['path']);self.enqueue('PAYLOAD_W',k)
            if r['path']=='slow':self.normal=None
            else:self.probe=None
            self.resp=None
        if self.cpu and self.cpu[0]==self.t:
            _,kind,k=self.cpu;self.cpu_finish(kind,k)
        self.accept_pending()
        # Dispatch shared response, held until positive service completion.
        if not self.resp:
            ready={}
            if self.normal and self.normal['state']=='ready':ready['slow']=self.normal
            if self.probe and self.probe['state']=='ready':ready['fast']=self.probe
            if ready:
                path=self.resp_rr if self.resp_rr in ready else next(iter(ready));p=ready[path];p['state']='response'
                self.resp={'end':self.t+1,'key':p['key'],'path':path};self.resp_rr='fast' if path=='slow' else 'slow';self.ev('response_start',p['key'],path=path)
        occupied={p['key'] for p in (self.normal,self.probe,self.resp) if p}
        # A returned translation is held at owner head while its AW is pending.
        occupied.update(j['key'] for j in self.pending if j['kind']=='PAYLOAD_W')
        if self.mode=='PN_M' and not self.probe and self.normal and self.normal['state']=='walk':
            o=1-self.normal['key'][0]
            if self.head[o] and self.head[o][0] not in occupied|self.rejected:
                k=self.head[o][0];self.probe={'key':k,'state':'DC','end':self.t+1};occupied.add(k);self.ev('probe_start',k,walk_key=list(self.normal['key']))
        if not self.normal and not self.probe:
            for o in [self.normal_rr,1-self.normal_rr]:
                if self.head[o] and self.head[o][0] not in occupied:
                    k=self.head[o][0];self.rejected.discard(k);self.normal={'key':k,'state':'lookup','end':self.t+1};self.normal_rr=1-o;self.ev('lookup_start',k);break
        if self.cpu is None:
            if self.initial:
                o,b=self.initial.pop(0);self.cpu=(self.t+1,'submit',(o,b,0));self.ev('submit_begin',public=True,owner=o,task=b)
            else:
                owners=[o for o in [self.poll_rr,1-self.poll_rr] if self.succ[o]<self.N]
                if owners:
                    o=owners[0];self.poll_rr=1-o;self.cpu=(-1,'poll',(o,-1,0));self.enqueue('POLL_R',(o,-1,0))
        self.accept_pending()
        d=self.device.dispatch(self.t)
        if d:k,work,h=d;self.ev('device_start',k,work=work,h=h)
        for s in self.mem:
            if self.active[s] is None and self.mem[s]:
                j=min(self.mem[s],key=self.keyorder);self.mem[s].remove(j);self.active[s]=(self.t+j['work'],j);self.ev('memory_start',j['key'],kind=j['kind'],phase=j['phase'],work=j['work'],server=s)
        if self.window:
            ptes=[j for j in self.pending if j['kind']=='PTE_R']+[j for q in self.mem.values() for j in q if j['kind']=='PTE_R']+[a[1] for a in self.active.values() if a and a[1]['kind']=='PTE_R']
            payload=any(a and a[1]['kind']=='PAYLOAD_W' for a in self.active.values());serving=any(a and a[1]['kind']=='PTE_R' for a in self.active.values())
            bits=[bool(ptes) and payload,bool(ptes) and not serving,bool(self.normal and self.normal['state']=='walk')]
            for i,b in enumerate(bits):
                if b:
                    if self.z[i]==65535:self.overflow[i]=True
                    else:self.z[i]+=1
        self.ev('tick_state',live=len(self.live),heads=[len(a) for a in self.head],normal=None if not self.normal else self.normal['state'],probe=None if not self.probe else self.probe['state'],memory={s:len(self.mem[s])+int(self.active[s] is not None) for s in self.mem})
        if not self.quiet(): self.advance(1)
    def quiet(self):
        return (self.succ==[self.N]*2 and not self.live and self.cpu is None and not self.device.active and not self.device.queue and not any(self.head) and not self.normal and not self.probe and not self.resp and not self.pending and not any(self.mem.values()) and not any(self.active.values()) and not any(self.cq))
    def run(self):
        self.setup()
        while not self.quiet():self.tick()
        self.window=False;self.ev('shared_quiet',public=True,successful=self.succ,submitted=self.sub,free_count=len(self.free));q=self.t
        assert len(self.free)==4
        for o in range(2):
            for b in range(2):self.cost('map_release',owner=o,task=b)
        for o in range(2):self.cost('queue_destroy',owner=o)
        self.ev('end',public=True,successful=self.succ,submitted=self.sub,live=0,free_count=len(self.free),empty=True)
        return {'duration':self.t,'end_h':self.device.h,'Z':self.z,'Z_overflow':self.overflow,'quiet_time':q,'owner_final_callback_times':self.final_callback,'max_memory_occupancy':self.maxocc,'O0_events':self.public}

from replay import verify

def configs():
    a=[]
    for topology in ['coupled','split_rw']:
        for W in [1,4]:a.append(dict(config_id=f'M_{topology}_W{W}',split='main',topology=topology,W=W,base_d=1,C=3,extra_boundary_idle_b=0,N_target_each=6))
    for h in CONTRACT['holdouts']:a.append(dict(config_id=h['name'],split='holdout',**{k:v for k,v in h.items() if k!='name'}))
    return a

def main():
    started=time.time();(BASE/'traces').mkdir(exist_ok=True);cache={};checks=0
    def invocation(c,h,mode,N):
        nonlocal checks
        key=json.dumps([c,h,mode,N],sort_keys=True);digest=hashlib.sha256(key.encode()).hexdigest()[:20]
        if key not in cache:
            s=Sim(c,h,mode,N);out=s.run();verify(s.events,N,mode,c,h);checks+=1
            file=f'traces/{digest}.json.gz'
            with gzip.open(BASE/file,'wt') as f:json.dump({'start_key':json.loads(key),'events':s.events},f,separators=(',',':'))
            out.update(trace_file=file,memoization_key=key);cache[key]=(out,s.events)
        return copy.deepcopy(cache[key][0])
    worlds=[]
    for c in configs():
        for k in range(7):
            for r in [0,2,4,10,12]:
                h,elapsed=prehistory(k,r);ref=invocation(c,h,'P_M',4);targets=[]
                for mode in ['P_M','PN_M']:
                    for wait in range(4):
                        boundary=c['extra_boundary_idle_b']+wait+int(mode=='PN_M');th=max(0,ref['end_h']-boundary);out=invocation(c,th,mode,c['N_target_each']);out.pop('O0_events')
                        out.update(mode=mode,wait=wait,boundary_elapsed=boundary,additional_time=boundary+out['duration'],total_J=ref['duration']+boundary+out['duration']);targets.append(out)
                worlds.append(dict(config_id=c['config_id'],split=c['split'],public_config={**c,'contract_sha256':HASH,'implementation_contract_sha256':hashlib.sha256((BASE/'implementation-contract.json').read_bytes()).hexdigest()},prehistory=dict(k=k,r=r,h0=h,elapsed=elapsed,weight='1/35'),reference=ref,targets=targets))
    (BASE/'worlds.json').write_text(json.dumps(worlds,separators=(',',':')))
    # Negatives are full reruns from explicit states, not reused arrival calendars.
    negatives={'public_initial_state':{'rule':'augment O0 with concrete prehistory tuple and h0','world_rows':'worlds.json'},'full_reset':[],'independent_refresh':[],'all_hit':[]}
    for c in configs():
        for mode in ['P_M','PN_M']:
            for wait in range(4):
                charge=c['extra_boundary_idle_b']+1+wait+int(mode=='PN_M');out=invocation(c,0,mode,c['N_target_each']);out.pop('O0_events');negatives['full_reset'].append(dict(config_id=c['config_id'],mode=mode,wait=wait,reset_and_action_charge=charge,target=out,additional_time=charge+out['duration']))
                for k in range(7):
                    for r in [0,2,4,10,12]:
                        h,elapsed=prehistory(k,r);charge=c['extra_boundary_idle_b']+1+elapsed+wait+int(mode=='PN_M');th=max(0,h-wait-int(mode=='PN_M'));out=invocation(c,th,mode,c['N_target_each']);out.pop('O0_events');negatives['independent_refresh'].append(dict(config_id=c['config_id'],future_prehistory=dict(k=k,r=r,h=h,elapsed=elapsed,weight='1/35'),mode=mode,wait=wait,reset_maintenance_action_charge=charge,target=out,additional_time=charge+out['duration'],old_world_pairing='Cartesian product with every old row of same config; future weight1/35 independent of old'))
        ac={**c,'C':4}
        for h in sorted({prehistory(k,r)[0] for k in range(7) for r in [0,2,4,10,12]}):
            p=invocation(ac,max(0,h-1),'P_M',c['N_target_each']);n=invocation(ac,max(0,h-1),'PN_M',c['N_target_each'])
            pe=cache[p['memoization_key']][1];ne=cache[n['memoization_key']][1]
            strip=lambda es:[{k:v for k,v in e.items() if not(k=='mode' and e['event']=='invocation_start')} for e in es]
            assert strip(pe)==strip(ne),'all-hit matched elapsed mismatch'
            negatives['all_hit'].append(dict(config_id=c['config_id'],h=h,P_wait=1,PN_wait=0,duration=p['duration'],traces=[p['trace_file'],n['trace_file']],equal=True))
    (BASE/'negative-controls.json').write_text(json.dumps(negatives,separators=(',',':')))
    # Each corruption must fail independent replay.
    sample=next((ev for out,ev in cache.values() if any(e['event']=='callback' for e in ev)));mutations={}
    for name in ['early_callback','early_reuse','fast_accept_on_miss','premature_end']:
        es=copy.deepcopy(sample)
        if name=='early_callback':
            e=next(copy.deepcopy(e) for e in es if e['event']=='callback');es.insert(0,e)
        elif name=='early_reuse':
            i=next(i for i,e in enumerate(es) if e['event']=='submit');e=copy.deepcopy(es[i]);e['key'][2]+=1;es.insert(i+1,e)
        elif name=='fast_accept_on_miss':
            e=next(e for e in es if e['event']=='response_start');e['path']='fast'
        else:
            i=next(i for i,e in enumerate(es) if e['event']=='submit');e=copy.deepcopy(es[-1]);es.insert(i+1,e)
        try:verify(es,4,'P_M');raise RuntimeError('mutation escaped '+name)
        except AssertionError as ex:mutations[name]=str(ex) or 'rejected'
    report=dict(status='PASS',worlds=len(worlds),registered_action_rows=sum(len(w['targets']) for w in worlds),unique_invocation_replays=checks,negative_mutations=mutations,all_hit_cases=len(negatives['all_hit']),independent_refresh_future_rows=len(negatives['independent_refresh']),elapsed_seconds=time.time()-started,contract_sha256=HASH,native_performance=None,area=None,CPU_calibration=None)
    (BASE/'verification.json').write_text(json.dumps(report,indent=2));print(json.dumps(report))

if __name__=='__main__':
    try:main()
    except Exception:
        p=BASE/f'failure-{time.time_ns()}.txt';p.write_text(traceback.format_exc());raise
