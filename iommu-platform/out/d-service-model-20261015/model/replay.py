"""Independent trace invariant replay, without simulator transitions or aggregate trust."""
def verify(events,N,mode,cfg=None,h0=None):
    cfg=cfg or {'C':3,'base_d':1};caps={2:[1,1],3:[2,1],4:[2,2]}[cfg['C']]
    cache=[[],[]]
    def touch(k):
        o,b=k[:2]
        if b in cache[o]:cache[o].remove(b)
        cache[o].append(b)
        while len(cache[o])>caps[o]:cache[o].pop(0)
    live=set();last={};freed=set();payload=set();cq=set();observed=set();cb=set();sub=[0,0];success=[0,0]
    ready={};accepted={};active={};finished=set();heads=[[],[]];normal=None;probe=None;response=None
    translated=set();dev=None;devq=[];chains={};lookup={};checks={};ends=0;warm=None;warmph=[];callback_begin={};clock=-1
    h=h0;ht=0
    token=lambda e:(e['kind'],tuple(e['key']),e['phase'])
    for index,e in enumerate(events):
        t=e['t'];assert t>=clock,'nonmonotonic trace';clock=t
        typ=e['event'];k=tuple(e['key']) if 'key' in e else None
        if typ=='warm_lookup':warm=(e['owner'],e['task']);warmph=[]
        elif typ=='warm_PTE_R':
            assert warm==(e['owner'],e['task']) and e['phase']==len(warmph);warmph.append(e['phase'])
        elif typ=='warm_response':
            assert warm==(e['owner'],e['task']);assert warm[1] in cache[warm[0]] or warmph==[0,1,2];touch(warm);warm=None
        elif typ=='warm_end':assert e['cache']==cache,'warm cache'
        elif typ=='submit':
            assert not any(x[:2]==k[:2] for x in live),'early reuse';assert k[:2] not in freed,'reuse after free'
            assert k[2]==last.get(k[:2],0)+1,'generation';last[k[:2]]=k[2];live.add(k);sub[k[0]]+=1
            assert sub[k[0]]<=N and sum(x[0]==k[0] for x in live)<=2,'owner credit';assert len(live)<=4
            assert e['lba']==sub[k[0]]-1 and e['sealed']==(sub[k[0]]==N);devq.append(k)
        elif typ=='device_start':
            assert dev is None and devq and devq.pop(0)==k,'device FCFS'
            if h is not None:
                h=max(0,h-(t-ht));ht=t;assert e['h']==h and e['work']==cfg['base_d']+h//8,'provider work'
            dev=(k,t+e['work'])
        elif typ=='device_finish':
            assert dev==(k,t);dev=None;heads[k[0]].append(k)
            if h is not None:h=min(64,max(0,h-(t-ht))+16);ht=t;assert e['h']==h
        elif typ=='lookup_start':
            assert normal is None and probe is None,'normal/probe exclusion';assert heads[k[0]] and heads[k[0]][0]==k,'stable head'
            normal=(k,'lookup',t);lookup[k]=None;chains[k]=[]
        elif typ=='lookup_result':
            assert normal and normal[0]==k and normal[1]=='lookup' and t==normal[2]+1
            hit=k[1] in cache[k[0]];assert e['hit']==hit,'lookup cache oracle';lookup[k]=hit;normal=(k,'ready' if hit else 'walk',t)
            if hit:touch(k)
        elif typ=='walk_complete':
            assert normal and normal[:2]==(k,'walk');assert chains[k]==[0,1,2],'walk dependency';touch(k);normal=(k,'ready',t)
        elif typ=='probe_start':
            assert mode=='PN_M' and probe is None;assert normal and normal[1]=='walk' and normal[0][0]!=k[0],'probe without other-owner active walk'
            assert tuple(e['walk_key'])==normal[0] and heads[k[0]][0]==k;probe=(k,'DC',t)
        elif typ=='probe_leaf_begin':assert probe and probe[:2]==(k,'DC') and t==probe[2]+1;probe=(k,'LEAF',t)
        elif typ=='probe_check':
            assert probe and probe[:2]==(k,'LEAF') and t==probe[2]+1
            hit=k[1] in cache[k[0]];assert e['hit']==hit and e['permission']==hit,'probe cache oracle'
            checks[k]=hit
            if hit:touch(k);probe=(k,'ready',t)
            else:probe=None
        elif typ=='response_start':
            assert response is None,'response overlap'
            if e['path']=='fast':assert probe and probe[:2]==(k,'ready') and checks[k],'fast accept on miss';probe=(k,'response',t)
            else:assert normal and normal[:2]==(k,'ready'),'incomplete slow translation';normal=(k,'response',t)
            response=(k,t+1,e['path'])
        elif typ=='response_finish':
            assert response==(k,t,e['path']),'response duration/identity';response=None;translated.add(k)
            if e['path']=='slow':assert normal and normal[:2]==(k,'response');normal=None
            else:assert probe and probe[:2]==(k,'response');probe=None
        elif typ=='memory_ready':
            tok=token(e);assert tok not in ready and tok not in accepted;ready[tok]=t
            if e['kind']=='PAYLOAD_W':assert k in translated,'payload untranslated'
            if e['kind']=='CQ_W':assert k in payload,'CQ before payload'
            if e['kind']=='PTE_R':
                assert normal and normal[:2]==(k,'walk');assert chains[k]==list(range(e['phase'])),'dependent PTE ready'
        elif typ=='memory_accept':
            tok=token(e);assert tok in ready;ready.pop(tok);accepted[tok]=e['server'];assert sum(s==e['server'] for s in accepted.values())<=8,'memory capacity'
        elif typ=='AW_accept':
            assert heads[k[0]] and heads[k[0]].pop(0)==k;assert k in translated;translated.remove(k)
        elif typ=='memory_start':
            tok=token(e);s=e['server'];assert accepted.get(tok)==s and s not in active,'memory overlap/unaccepted';active[s]=(tok,t+e['work'])
            if cfg.get('topology')=='split_rw':assert s==('R' if e['kind'].endswith('_R') else 'W'),'split direction'
            if e['kind']=='PTE_R':assert chains[k]==list(range(e['phase'])),'dependent PTE start'
        elif typ=='memory_finish':
            tok=token(e);s=e['server'];assert active.pop(s)==(tok,t);assert accepted.pop(tok)==s;finished.add(tok)
            if e['kind']=='PTE_R':assert chains[k]==list(range(e['phase']));chains[k].append(e['phase'])
        elif typ=='payload_done':assert ('PAYLOAD_W',k,0) in finished,'fabricated payload';payload.add(k)
        elif typ=='CQ_visible':assert ('CQ_W',k,0) in finished and k in payload;cq.add(k)
        elif typ=='CQ_observe':assert k in cq and k not in observed;observed.add(k)
        elif typ=='callback_begin':assert k in observed;callback_begin[k]=t
        elif typ=='callback':
            assert k in live and k in observed and t==callback_begin[k]+1,'early callback';live.remove(k);cb.add(k);success[k[0]]+=1
        elif typ=='free':assert k in cb and k[:2] not in freed,'early/duplicate free';freed.add(k[:2])
        elif typ=='shared_quiet' or typ=='end':
            assert sub==success==[N,N] and not live and not devq and dev is None and not ready and not accepted and not active and not any(heads) and normal is None and probe is None and response is None and not translated,'premature end'
            assert freed=={(0,0),(0,1),(1,0),(1,1)},'unfreed tasks'
            assert cq==observed==cb,'unconsumed completion'
            if typ=='end':assert index==len(events)-1,'post-end events';ends+=1
    assert ends==1,'missing endpoint'
    return True
