#!/usr/bin/env python3
"""Custody + bounded raw cost derivation; independent checker owns acceptance."""
import collections,csv,hashlib,json,pathlib,re
H=pathlib.Path(__file__).resolve().parent;R=H.parents[2]
CASES=['cpu_eager','cpu_batch','single_eager','single_batch','parallel_eager','parallel_batch']
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
load=lambda p:json.loads(p.read_text())
write=lambda p,x:p.write_text(json.dumps(x,indent=2)+'\n')
# Capture actual compiler-observed headers/generated files/HDL, not only claims.
paths=set()
for p in (H/'obj').glob('*.d'):
 for token in p.read_text().replace('\\\n',' ').split():
  token=token.rstrip(':');q=pathlib.Path(token);q=q if q.is_absolute() else H/'obj'/q
  if q.is_file():paths.add(q.resolve())
vdat=H/'obj/Vnonblock_actual_dma_system_top__verFiles.dat'
for line in vdat.read_text().splitlines():
 if line.startswith(('S ','T ')):
  q=pathlib.Path(re.findall(r'"([^"]+)"',line)[-1])
  if q.is_file():paths.add(q.resolve())
for x in load(H/'build-inputs.json')['files']:paths.add((R/x['path']).resolve())
write(H/'actual-compiled-dependencies.json',{'verilator_records':str(vdat.relative_to(H)),'compiler_dependency_records':[str(p.relative_to(H)) for p in sorted((H/'obj').glob('*.d'))],'files':[{'path':str(p),'sha256':sha(p)} for p in sorted(paths)]})
frozen=R/'out/stage117-three-results-20261013/common/fixture.json';fixture=load(frozen)
O=R/'out/stage117-real-cache-handoff-20261013/implementation';old=load(O/'build-inputs.json')
unchanged=[]
for x in old['files']:
 p=R/x['path']
 if '/implementation/' not in x['path'] and p.is_file():
  assert sha(p)==x['sha256'],('original production/frozen input changed',str(p));unchanged.append(x)
write(H/'source-and-oracle-binding.json',{'fixture':{'path':str(frozen),'sha256':sha(frozen),'payload_bytes':1639,'record_bytes':1675,'spans':[{'packet':1,'src':131,'dst':8343,'length':60},{'packet':2,'src':397,'dst':8617,'length':65},{'packet':3,'src':791,'dst':9043,'length':503},{'packet':3,'src':3803,'dst':12055,'length':1011}]},'root_contract':{'path':str(H.parent/'comparison-contract.json'),'sha256':sha(H.parent/'comparison-contract.json')},'unchanged_prior_declared_RTL_and_frozen_files':unchanged,'toolchain':{'reused_prior_provenance':str(O/'toolchain/provenance.json'),'provenance_sha256':sha(O/'toolchain/provenance.json'),'verilator':str(O/'toolchain/verilator-5.040/bin/verilator'),'sha256':sha(O/'toolchain/verilator-5.040/bin/verilator'),'flags':['--language','1800-2017','-fno-gate']},'scope':'test-only independentCPUport exposure + controlledsuccess driver; no production source changes','cpu_copy_data_origin':'actual port1 granted/returned source words only','offline_oracle':'expected frames used only initialization and independent/offline audit, no prepublication whole-byte oracle access','COUNT_hardware_batch':{'legal_for_original_four_spans':False,'reason':'Four transfer lengths60,65,503,1011 differ; their src/dst advances also differ. A common LENGTH/COUNT/stride auto batch cannot express them without changing exact transfer bytes; ordinary NEXT/refill and source-supported shared prefix publication used.'}})
metrics={}
for mode in CASES:
 d=H/'results'/mode
 if not (d/'run.json').exists():continue
 run=load(d/'run.json');events=list(csv.DictReader((d/'events.csv').open()));ev=collections.defaultdict(list)
 for e in events:ev[e['event']].append(e)
 def one(name,pid=None):
  rows=ev[name] if pid is None else [e for e in ev[name] if int(e['span'])+1==pid]
  assert len(rows)==1,(mode,name,pid,len(rows));return int(rows[0]['cycle'])
 common=one('common_ready');quiet=one('global_quiet');release=one('release_quiet')
 group=[]
 for q in ev['prefix_claim']:
  g=int(q['id']);ends=[x for x in ev['prefix_release'] if int(x['id'])==g];assert len(ends)==1
  group.append({'group':g,'packet_mask':int(q['address']),'packet_count':int(q['n']),'claim_cycle':int(q['cycle']),'release_cycle':int(ends[0]['cycle']),'claim_to_release_cycles':int(ends[0]['cycle'])-int(q['cycle'])})
 fences=[]
 for e in ev['fence_begin']:
  ends=[x for x in ev['fence_complete'] if x['id']==e['id']];assert len(ends)==1
  fences.append({'packet':int(e['span'])+1,'begin':int(e['cycle']),'complete':int(ends[0]['cycle']),'cycles':int(ends[0]['cycle'])-int(e['cycle']),'receipt':int(e['id'])})
 services=collections.defaultdict(collections.Counter);offers={};pending=collections.deque();returns=[];overlap=collections.Counter();port_rows={};last_source={};copy_queue_rows=[]
 with (d/'cpu_ports.csv').open() as f:
  for x in csv.DictReader(f):
   if not int(x['rst']):continue
   cy=int(x['cycle']);port=int(x['port']);seq=int(x['op_seq']);ph=x['phase'];port_rows.setdefault(cy,{})[port]=x
   if port==1 and int(x['rvalid']):
    assert pending,(mode,'read response pending');q=pending.popleft();q['response']=cy;returns.append(q);services[q['phase']]['grant_to_response_cycles_inclusive']+=cy-q['grant']+1;services[q['phase']]['first_offer_to_response_cycles_inclusive']+=cy-q['begin']+1
    if q['phase']=='cpu_copy':last_source[q['packet']]=cy
   if int(x['req']):
    key=(port,seq);q=offers.setdefault(key,{'phase':ph,'packet':int(x['packet']),'span':int(x['span']),'address':int(x['address']),'be':int(x['be']),'port':port,'seq':seq,'begin':cy})
    if int(x['gnt']):
     q['grant']=cy;services[ph]['accepted_load_ops' if port==1 else 'accepted_store_ops']+=1;services[ph]['logical_load_bytes' if port==1 else 'logical_store_bytes']+=q['be'].bit_count();services[ph]['first_offer_to_grant_cycles_inclusive']+=cy-q['begin']+1
     if port==1:pending.append(q)
     del offers[key]
 assert not pending and not offers,(mode,'CPU operation unfinished')
 for cy,q in port_rows.items():
  a=q[1];b=q[2]
  if a['phase']=='cpu_copy' and b['phase']=='cpu_copy':
   if int(a['req']) and int(b['req']):overlap['copy_read_store_offers']+=1
   if int(a['req']) and int(a['gnt']) and int(b['req']) and int(b['gnt']):overlap['copy_read_store_grants']+=1
  if int(a['rvalid']) and int(a['req']) and int(a['gnt']):overlap['old_read_response_next_grant']+=1
 # Separate actual AXI actor traffic. Every write beat is associated with the
 # actual ordered accepted AW; each completion is matched by transaction ID.
 bus={};bus_scopes={};cache_reads=[]
 for name in ['cache_axi.csv','memory_axi.csv']:
  reads=collections.defaultdict(collections.deque);writes=collections.deque();bs=collections.defaultdict(collections.deque);counts=collections.defaultdict(collections.Counter);scopes=collections.defaultdict(collections.Counter)
  with (d/name).open() as f:
   for x in csv.DictReader(f):
    if not int(x['rst']) or not int(x['valid']) or not int(x['ready']):continue
    channel=x['channel'];tid=int(x['id']);cy=int(x['cycle']);ph=x['phase'];actor='cache' if name=='cache_axi.csv' else {0:'CPU_cache',2:'DMA_payload',3:'IOMMU_walk'}[(tid>>4)&3] if channel not in ['W'] else None
    scope='startup' if cy<common else 'workload_until_globalquiet' if cy<quiet else 'terminal_audit_and_release'
    if channel=='W':
     assert writes,(mode,name,'scopeW withoutAW');scoped_actor=writes[0]['actor']
    else:scoped_actor=actor
    sc=scopes[(scope,scoped_actor)];sc[channel]+=1
    if channel=='AR':sc['requested_read_bus_bytes']+=(int(x['len'])+1)*(1<<int(x['size']))
    elif channel=='R':sc['read_bus_bytes']+=8
    elif channel=='W':sc['write_bus_bytes']+=8;sc['write_strobed_bytes']+=int(x['strb']).bit_count()
    if channel=='AR':
     addr=int(x['address']);n=(int(x['len'])+1)*(1<<int(x['size']));reads[tid].append({'actor':actor,'phase':ph,'packet':int(x['packet']),'address':addr,'bytes':n,'begin':cy,'cacheable':0x80000000<=addr<0xc0000000});counts[actor]['AR']+=1;counts[actor]['requested_read_bus_bytes']+=n
    elif channel=='R':
     assert reads[tid],(mode,name,'R withoutAR');q=reads[tid][0];counts[q['actor']]['read_beats']+=1;counts[q['actor']]['read_bus_bytes']+=8
     if int(x['last']):
      q=reads[tid].popleft();q['complete']=cy;q['cycles_inclusive']=cy-q['begin']+1
      if name=='cache_axi.csv':cache_reads.append(q);counts[q['actor']]['cacheable_refill_bus_bytes' if q['cacheable'] else 'uncached_read_bus_bytes']+=q['bytes']
    elif channel=='AW':
     q={'actor':actor,'phase':ph,'id':tid,'address':int(x['address']),'begin':cy};writes.append(q);bs[tid].append(q);counts[actor]['AW']+=1
    elif channel=='W':
     assert writes,(mode,name,'W withoutAW');q=writes[0];counts[q['actor']]['write_beats']+=1;counts[q['actor']]['write_bus_bytes']+=8;counts[q['actor']]['write_strobed_bytes']+=int(x['strb']).bit_count()
     if int(x['last']):writes.popleft()
    elif channel=='B':
     assert bs[tid],(mode,name,'B withoutAW');q=bs[tid].popleft();counts[q['actor']]['B']+=1
  assert not writes and all(not q for q in reads.values()) and all(not q for q in bs.values()),(mode,name,'outstandingAXI')
  bus[name]={k:dict(v) for k,v in counts.items()};bus_scopes[name]=[{'interval':k[0],'actor':k[1],'counts':dict(v)} for k,v in sorted(scopes.items())]
 write(d/'cache-read-service.json',cache_reads)
 # Actual payload DDR R receipt before positive public DONE; reads may include
 # aligned neighbors, but perpacket useful source-byte retirement is explicit.
 if not mode.startswith('cpu_'):
  wanted={p['packet_id']:set(Tb for s in p['source'] for Tb in range(0x80000000+s['offset'],0x80000000+s['offset']+s['length'])) for p in fixture['packets']}
  seen={p:set() for p in wanted}
  for e in ev['payload_r']:
   cy=int(e['cycle']);a=int(e['address'])
   for p,w in wanted.items():
    hit=w.intersection(range(a,a+8))
    if hit:seen[p].update(hit);last_source[p]=cy
  assert all(seen[p]==w for p,w in wanted.items()),(mode,'sourceR coverage')
 milestones={name:[one(name,pid)-common for pid in [1,2,3]] for name in ['used_publish','consumer_completed','B_returned','A_returned','source_return_metadata_retired','A_read_retired']}
 hold=sum(p['length']*milestones['A_returned'][p['packet_id']-1] for p in fixture['packets'])
 polls={};gaps={}
 for e in ev['done_poll_return']:polls.setdefault(int(e['engine']),[]).append(int(e['cycle']))
 for e,p in polls.items():gaps[str(e)]={'poll_count':len(p),'max_adjacent_return_gap_cycles':max([b-a for a,b in zip(p,p[1:])] or [0]),'first_return_relative_common':p[0]-common,'last_return_relative_common':p[-1]-common}
 lifetime={'startup_until_commonready':common,'commonready_to_last_used':max(milestones['used_publish']),'commonready_to_last_consumer':max(milestones['consumer_completed']),'commonready_to_last_Breturn':max(milestones['B_returned']),'commonready_to_globalquiet':quiet-common,'audit_and_postscan_release':release-quiet,'whole_common_clock_until_release':release,'source_payload_hold_bytecycles':hold,'entire_A_region_allocation_bytes':8192,'entire_A_mapping_lifetime_cycle_end':release,'source_hold_definition':'sum original useful source bytes * logical Areturn cycles relativecommon; logicalreturn does not physically unmap or overwrite originalsource'}
 with (d/'cpu_copy_frontend.csv').open() as f:copy_queue_rows=list(csv.DictReader(f))
 resource={k:max([int(q[k]) for q in copy_queue_rows] or [0]) for k in ['descriptor_count','descriptor_capacity','pending_reads','queued_words','queued_useful_bytes']}
 backend=[]
 for e in ev['backend_accepted']:
  ends=[x for x in ev['backend_response'] if x['engine']==e['engine'] and x['span']==e['span']];assert len(ends)==1;backend.append({'engine':int(e['engine']),'span':int(e['span']),'packet':1 if int(e['span'])==0 else 2 if int(e['span'])==1 else 3,'begin':int(e['cycle']),'complete':int(ends[0]['cycle'])})
 consumer_overlap=[]
 for pid in [1,2,3]:
  begin=one('consumer_start',pid);end=one('consumer_completed',pid);peer=[b for b in backend if b['packet']!=pid and b['begin']<end and b['complete']>begin];consumer_overlap.append({'packet':pid,'begin':begin,'complete':end,'peer_backend_overlaps_any_cycle':bool(peer),'peer_backend_transactions':peer})
 metrics[mode]={'common_ready':common,'milestone_vectors_relative_commonready':milestones,'actual_source_R_retirement_vector':[last_source[p]-common for p in [1,2,3]],'source_R_retirement_definition':'CPU actual source cacheport response; DMA usefulsource byte actual DDRR completion, safe publicDONE bound recorded separately','prefix_groups':group,'fences':fences,'lifecycle_common_clock_cycles':lifetime,'actual_CPU_services_by_phase':{k:dict(v) for k,v in services.items()},'services_cycle_definition':'inclusive firstoffer/grant/response intervals, overlapping port endpoints must not be summed','AXI_actor_traffic':bus,'AXI_traffic_by_lifecycle_interval':bus_scopes,'raw_CPUcopy_concurrency':dict(overlap),'CPU_frontend_resource_raw_peak':resource,'CPU_frontend_resource_sizes_executor_diagnostic':run['CPU_copy_frontend'],'resource_limit':'descriptor storage and queue nodes are host testfront-end resources; allocator/string/deque overhead and fullCPUinstructions/shuffletime null, not deployedCVA6 scratchcost','public_poll_gaps':gaps,'backend_transactions':backend,'consumer_peer_overlap':consumer_overlap,'host_execution_cost_only':run['host_ns_execution_cost_only'],'full_CPU_instruction_latency':None,'CPU_ALU_shuffle_latency':None,'full_software_latency':None,'application_latency':None,'board_latency':None,'independent_acceptance':'checker seat owns originalraw protocol/cache/byte acceptance; these metrics alone not proof'}
write(H/'costs-and-overlap.json',metrics)
ledger=load(H/'execution-ledger.json');passed={x['case']:x for x in ledger if x['returncode']==0};binary=H/'obj/Vnonblock_actual_dma_system_top'
assert set(passed)==set(CASES+['interface_probe']),(set(passed),'expected sixcases+one newprobe');assert len({x['binary_sha256'] for x in passed.values()})==1 and sha(binary)==next(iter(passed.values()))['binary_sha256']
failures=[x for x in ledger if x['returncode']];write(H/'failure-ledger.json',{'failed_executions':failures,'policy':'all attempts retained; retry only actual fault correction; no old suite/protocol17/matrix calls'})
artifacts=[]
for p in sorted(H.rglob('*')):
 if not p.is_file() or 'obj' in p.relative_to(H).parts or p.name in ['artifact-sha256.json','delivery.json']:continue
 artifacts.append({'path':str(p.relative_to(H)),'sha256':sha(p),'bytes':p.stat().st_size})
artifacts.append({'path':str(binary.relative_to(H)),'sha256':sha(binary),'bytes':binary.stat().st_size});write(H/'artifact-sha256.json',artifacts)
write(H/'delivery.json',{'schema':'s117-delivery-g0-implementation-v1','status':'stable_for_root_independent_review','author':'Codex gpt-6.1-sol ultra','interface_plan':'interface-plan.json','gate_ready':'gate-ready.json','source_and_oracle_binding':'source-and-oracle-binding.json','actual_compiled_dependencies':'actual-compiled-dependencies.json','build_inputs':'build-inputs.json','build_ledger':'build-ledger.json','execution_ledger':'execution-ledger.json','failure_ledger':'failure-ledger.json','same_binary':str(binary.relative_to(H)),'same_binary_sha256':sha(binary),'new_interface_probe':'results/interface_probe/probe.json','cases':{k:str(pathlib.Path(passed[k]['output'])/'run.json') for k in CASES},'metrics':'costs-and-overlap.json','raw_signals':['cpu_ports.csv','cache_control.csv','cache_axi.csv','memory_axi.csv','cpu_copy_frontend.csv'],'original_DMA_events':'events.csv','artifact_manifest':'artifact-sha256.json','artifact_manifest_sha256':sha(H/'artifact-sha256.json'),'executor_completion':all(load(H/passed[k]['output']/'run.json')['pass'] for k in CASES),'independent_checker_acceptance':None,'limits':{'deterministic_optimistic_CPUport_frontend':True,'native_CVA6_memcpy_execution':False,'cache_ports_physical_no_MMU':True,'Icache_inactive':True,'controlled_nofault_success_projection':True,'production_RTL_driver_ABI_change':False,'source_overwrite':False,'prepublication_whole_record_validation':False,'pressure_delay_arrival_deadline_ringloss_changes':False,'statistical_performance_claim':False,'translation_causality_claim':False,'full_CPU_instruction_latency':None,'CPU_ALU_shuffle_latency':None,'full_software_latency':None,'application_latency':None,'board_latency':None}})
print(json.dumps({'delivery':'delivery.json','cases':{k:v['milestone_vectors_relative_commonready']['used_publish'] for k,v in metrics.items()},'actual_dependency_files':len(paths),'artifacts':len(artifacts),'failures':len(failures)}))
