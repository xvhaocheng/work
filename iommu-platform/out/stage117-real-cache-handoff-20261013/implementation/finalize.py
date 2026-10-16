#!/usr/bin/env python3
import json,pathlib,hashlib,re,csv,subprocess,collections
H=pathlib.Path(__file__).resolve().parent;R=H.parents[2]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
# Compiler-observed actual HDL + header + C++ deps, independent of declared list.
paths=set()
for p in (H/'obj').glob('*.d'):
 s=p.read_text().replace('\\\n',' ')
 for token in s.split():
  token=token.rstrip(':')
  q=pathlib.Path(token)
  if not q.is_absolute():q=H/'obj'/q
  if q.is_file():paths.add(q.resolve())
vdat=H/'obj/Vnonblock_actual_dma_system_top__verFiles.dat'
if vdat.exists():
 for token in [re.findall(r'"([^"]+)"',line)[-1] for line in vdat.read_text().splitlines() if line.startswith(('S ','T '))]:
  q=pathlib.Path(token)
  if q.is_file():paths.add(q.resolve())
for p in json.loads((H/'build-inputs.json').read_text())['files']:paths.add((R/p['path']).resolve())
# Add global includes actually used by compiler/dependencies (all tracked paths preserved).
entries=[{'path':str(p),'sha256':sha(p)} for p in sorted(paths) if p.is_file()]
(H/'actual-compiled-dependencies.json').write_text(json.dumps({'compiler_dependency_records':[str(p.relative_to(H)) for p in sorted((H/'obj').glob('*.d'))],'verilator_records':str(vdat.relative_to(H)),'files':entries},indent=2)+'\n')
metrics={}
for mode in ['single_FIFO_ahead','parallel_FIFO_ahead']:
 d=H/'results'/mode
 if not (d/'run.json').exists():continue
 run=json.loads((d/'run.json').read_text());events=list(csv.DictReader((d/'events.csv').open()));ev=collections.defaultdict(list)
 for e in events:ev[e['event']].append(e)
 operations=[]
 for e in ev['fence_begin']:
  ends=[x for x in ev['fence_complete'] if x['id']==e['id']];assert len(ends)==1
  operations.append({'packet':int(e['span'])+1,'begin':int(e['cycle']),'complete':int(ends[0]['cycle']),'cycles':int(ends[0]['cycle'])-int(e['cycle']),'receipt':int(e['id'])})
 phase=collections.Counter();cpu_grants=collections.Counter();cache_axi=collections.Counter()
 with (d/'cache_control.csv').open() as f:
  for x in csv.DictReader(f):
   if int(x['rst']):phase[x['phase']]+=1
 with (d/'cpu_ports.csv').open() as f:
  for x in csv.DictReader(f):
   if int(x['rst']) and int(x['req']) and int(x['gnt']):cpu_grants[(x['phase'],'store' if int(x['we']) else 'load')]+=1
 with (d/'cache_axi.csv').open() as f:
  for x in csv.DictReader(f):
   if int(x['rst']) and int(x['valid']) and int(x['ready']):cache_axi[(x['phase'],x['channel'])]+=1
 # Raw observed backend transactions determine overlap at consumer start/end.
 backend=[]
 for e in ev['backend_accepted']:
  responses=[x for x in ev['backend_response'] if x['engine']==e['engine'] and x['span']==e['span']];assert len(responses)==1
  backend.append({'engine':int(e['engine']),'span':int(e['span']),'packet':1 if int(e['span'])==0 else 2 if int(e['span'])==1 else 3,'begin':int(e['cycle']),'complete':int(responses[0]['cycle'])})
 overlap=[]
 for p in [1,2,3]:
  start=next(int(e['cycle']) for e in ev['consumer_start'] if int(e['span'])+1==p);end=next(int(e['cycle']) for e in ev['consumer_completed'] if int(e['span'])+1==p)
  peers=[b for b in backend if b['packet']!=p and b['begin']<=start<b['complete']]
  anywhere=[b for b in backend if b['packet']!=p and b['begin']<end and b['complete']>start]
  milestones={}
  for name in ['fence_begin','cache_acquired','used_publish','A_reused','consumer_start','consumer_completed','B_returned']:
   ee=[e for e in ev[name] if int(e['span'])+1==p];assert len(ee)==1;cy=int(ee[0]['cycle']);active=[b for b in backend if b['packet']!=p and b['begin']<=cy<b['complete']];milestones[name]={'cycle':cy,'peer_backend_active':bool(active),'peer_transactions':active}
  overlap.append({'packet':p,'consumer_start':start,'consumer_complete':end,'peer_backend_active_at_consumer_start':bool(peers),'peer_transactions':peers,'peer_backend_overlaps_any_consumer_cycle':bool(anywhere),'peer_transactions_during_consumer':anywhere,'milestones':milestones})
 def event(name,pid=None):
  rows=ev[name] if pid is None else [e for e in ev[name] if int(e['span'])+1==pid]
  assert len(rows)==1,(name,pid,len(rows))
  return int(rows[0]['cycle'])
 intervals={'startup_until_common_ready':{'begin':0,'end':event('common_ready'),'cycles':event('common_ready')},'terminal_checked_quiet':{'begin':event('global_quiet'),'end':event('release_quiet'),'cycles':event('release_quiet')-event('global_quiet')},'packets':[]}
 for pid in [1,2,3]:
  row={'packet':pid}
  for label,a,b in [('validation','cache_acquired','payload_validated'),('used_metadata_and_drain','payload_validated','used_publish'),('source_reuse_and_retirement_metadata','used_publish','A_reused'),('consumer_metadata_record_guard_done_and_drain','consumer_start','consumer_completed'),('B_return_metadata_and_drain','consumer_completed','B_returned')]:
   x,y=event(a,pid),event(b,pid);row[label]={'begin':x,'end':y,'cycles':y-x}
  intervals['packets'].append(row)
 # Actual public CPU port service: request wait and load response time, with
 # stores separately charged through physical WT retirement/drain intervals.
 pending={};services=collections.defaultdict(lambda:collections.Counter())
 with (d/'cpu_ports.csv').open() as f:
  for x in csv.DictReader(f):
   if not int(x['rst']):continue
   cycle=int(x['cycle']);seq=int(x['op_seq']);ph=x['phase']
   if int(x['req']):
    q=pending.setdefault(seq,{'phase':ph,'begin':cycle,'we':int(x['we']),'be':int(x['be'])})
    if int(x['gnt']):
     q['grant']=cycle;services[ph]['accepted_store_ops' if q['we'] else 'accepted_load_ops']+=1;services[ph]['logical_store_bytes' if q['we'] else 'logical_load_bytes']+=q['be'].bit_count();services[ph]['request_to_grant_cycles']+=cycle-q['begin']+1
     if q['we']:del pending[seq]
   if int(x['rvalid']):
    q=pending.pop(seq);assert not q['we'];services[q['phase']]['grant_to_response_cycles']+=cycle-q['grant']+1;services[q['phase']]['request_to_response_cycles']+=cycle-q['begin']+1
 assert not pending,'CPU pending at delivery'
 # Actual cache refill read service measured at cache AXI boundary, separating
 # cacheable linefills from uncached table reads; no host CPU timing inference.
 arq=collections.defaultdict(collections.deque);refills=[];counts=collections.Counter()
 with (d/'cache_axi.csv').open() as f:
  for x in csv.DictReader(f):
   if not int(x['rst']) or not int(x['valid']) or not int(x['ready']):continue
   cycle=int(x['cycle']);tid=int(x['id'])
   if x['channel']=='AR':
    a=int(x['address']);n=(int(x['len'])+1)*(1<<int(x['size']));arq[tid].append({'phase':x['phase'],'packet':int(x['packet']),'address':a,'bytes':n,'begin':cycle,'cacheable':0x80000000<=a<0xc0000000})
   if x['channel']=='R' and int(x['last']):
    q=arq[tid].popleft();q['complete']=cycle;q['cycles']=cycle-q['begin']+1;refills.append(q);counts['cacheable_line_refills' if q['cacheable'] else 'uncached_read_transactions']+=1;counts['cacheable_refill_bus_bytes' if q['cacheable'] else 'uncached_read_bus_bytes']+=q['bytes']
 assert all(not q for q in arq.values()),'AXI read pending at delivery'
 (d/'cache-read-service.json').write_text(json.dumps(refills,indent=2)+'\n')
 intervals['actual_CPU_services_by_phase']={k:dict(v) for k,v in services.items()};intervals['actual_cache_read_service_counts']=dict(counts);intervals['actual_cache_read_service_file']=str((d/'cache-read-service.json').relative_to(H))
 metrics[mode]={'exact_common_clock_cost_intervals':intervals,'CPU_service_cycle_definition':'Inclusive pre-edge first request/grant/response intervals, so overlapping request and response interval endpoints must not be summed. WT physical completion is counted by separate drain/event intervals.','pass':run['pass'],'fences':operations,'trace_label_common_clock_cycles':dict(phase),'trace_label_limit':'Labels persist through host-driven public MMIO waits; these counts are diagnostic labels, not cache operation latency','CPU_grants_by_phase':[{'phase':k[0],'kind':k[1],'count':v} for k,v in sorted(cpu_grants.items())],'cache_AXI_handshakes_by_phase':[{'phase':k[0],'channel':k[1],'count':v} for k,v in sorted(cache_axi.items())],'peer_overlap':overlap,'actual_cache':run['actual_cache'],'host_execution_cost':run['host_ns_execution_cost_only'],'application_latency':None,'full_CPU_latency':None,'board_latency':None}
(H/'costs-and-overlap.json').write_text(json.dumps(metrics,indent=2)+'\n')
ledger=json.loads((H/'execution-ledger.json').read_text());passed={x['case']:x for x in ledger if x['returncode']==0};assert set(passed)=={'microtests','single_FIFO_ahead','parallel_FIFO_ahead'}
binary=H/'obj/Vnonblock_actual_dma_system_top';assert len({x['binary_sha256'] for x in passed.values()})==1 and sha(binary)==next(iter(passed.values()))['binary_sha256']
# Hash every final source/raw/result/ledger dependency artifact; generated objects not required separate from deps/binary.
artifacts=[]
for p in sorted(H.rglob('*')):
 if not p.is_file() or any(part in ['obj','toolchain'] for part in p.relative_to(H).parts) or p.name in ['artifact-sha256.json','delivery.json']:continue
 artifacts.append({'path':str(p.relative_to(H)),'sha256':sha(p),'bytes':p.stat().st_size})
artifacts.append({'path':str(binary.relative_to(H)),'sha256':sha(binary),'bytes':binary.stat().st_size})
(H/'artifact-sha256.json').write_text(json.dumps(artifacts,indent=2)+'\n')
delivery={'schema':'s117-real-cache-delivery-v1','status':'stable_for_root_independent_review','implementation_authorship':'Codex gpt-6.1-sol ultra','source_and_oracle_binding':'source-and-oracle-binding.json','scope':'actual CVA6 Genesys2 WT16B cache ports and actual controller fence architectural input; original two-DMA/IOMMU/fixed fixture','interface_plan':'interface-plan.json','build_inputs':'build-inputs.json','actual_compiled_dependencies':'actual-compiled-dependencies.json','build_ledger':'build-ledger.json','execution_ledger':'execution-ledger.json','microtests':'results/microtests/microtests.json','same_binary':str(binary.relative_to(H)),'same_binary_sha256':sha(binary),'cases':{k:str(pathlib.Path(v['output'])/'run.json') for k,v in passed.items() if k!='microtests'},'raw_signals':['cpu_ports.csv','cache_control.csv','cache_axi.csv','memory_axi.csv'],'original_DMA_protocol_events':'events.csv','metrics':'costs-and-overlap.json','artifact_manifest':'artifact-sha256.json','toolchain_ledger':['toolchain/tool-build-ledger.json','toolchain/tool-build-5.040-ledger.json'],'toolchain_provenance':'toolchain/provenance.json','excluded_toolchain_tree':'Large official source/build trees excluded from artifact manifest; source archives and exact compiler binaries pinned by provenance receipts.','functional_pass':True,'independent_checker_acceptance':None,'limits':{'no_full_CPU_fence_instruction_execution':True,'Icache_fetch_inactive':True,'physical_CPU_ports_no_MMU':True,'same_owner_bounded_protocol_not_production_API':True,'original_fixture_1639_payload_1675_records_unchanged':True,'original_DMA_buffers_delays_no_pressure_added':True,'production_RTL_driver_ABI_changed':False,'old_suites_replayed':False,'new_G0_or_innovation_claim':False,'application_latency':None,'full_CPU_latency':None,'board_latency':None}}
(H/'delivery.json').write_text(json.dumps(delivery,indent=2)+'\n');print(json.dumps({'delivery':'delivery.json','dependency_files':len(entries),'artifacts':len(artifacts),'metrics':{k:[x['peer_backend_active_at_consumer_start'] for x in v['peer_overlap']] for k,v in metrics.items()}}))
