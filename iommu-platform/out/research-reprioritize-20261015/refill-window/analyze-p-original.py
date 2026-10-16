#!/usr/bin/env python3
"""One streaming read of existing p_row0 COPY warm; never launches a model."""
import collections,csv,hashlib,json,time
from pathlib import Path
OUT=Path(__file__).resolve().parent
ROOT=OUT.parents[1]
RAW=ROOT/'d-dram-action-20261015/implementation/runs/p_row0/bridge-events.csv'
SOURCE=ROOT/'d-dram-action-20261015/implementation/preparation-harness.cpp'
VPTE={0x20000,0x40400,0x50000,0x50008}
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def ref(r,line):return {'line':line,'rtl_tick':int(r['rtl_tick']),'dram_tick':int(r['dram_tick']),'event':r['event'],'parent_uid':int(r['parent_uid']),'child_uid':int(r['child_uid']),'address':int(r['address']),'value':int(r['value'])}
def role(a):
 if a in VPTE:return 'V_PTE'
 if a in (0x30000,0x60800) or 0x70000<=a<=0x70078:return 'A_PTE'
 if 0x102c0<=a<0x10300:return 'A_DDT'
 if 0x90000000<=a<0x90008000:return 'A_source_payload'
 if 0x90100000<=a<0x90108000:return 'A_destination_payload'
 return 'unknown'
start=time.monotonic();digest=hashlib.sha256();busy=[False,False];issues=[0,0];done=[0,0];nexts=[0,0];swmatched=[0,0];lastid=[None,None];match_seen=[False,False];done_baseline=[None,None]
children={};records={};active={};windows=[];unassigned=None;current_gen=0;generation_windows={};due=collections.deque();counts=collections.Counter();rolecounts=collections.Counter();raw_rows=0

def snap():
 return {'V_hardware_busy':busy[0],'V_PTE_pending_callback':[p for p,q in active.items() if any(not records[p]['children'][c].get('callback') for c in q['children'])], 'V_PTE_pending_actual_R':[p for p,q in active.items() if not records[p].get('R_last')]}
def point(r,line):return dict(ref(r,line),**snap())
def observe_return():
 while due:
  w=due[0]
  if eventpos<=(w['software_return_earliest_dram_tick'],0.5):break
  due.popleft();w['at_earliest_software_return']=snap()

def lines(f):
 for line in f:
  digest.update(line.encode());yield line
with RAW.open(newline='') as f:
 for line,r in enumerate(csv.DictReader(lines(f)),2):
  raw_rows+=1;t=int(r['dram_tick']);rtl=int(r['rtl_tick']);p=int(r['parent_uid']);c=int(r['child_uid']);a=int(r['address']);v=int(r['value']);e=r['event']
  # At tick t, provider events from preceding RTL tick finish before pre-edge
  # events of the following RTL tick. cfg_xact returns between these stages.
  stage=1 if rtl>=0 and t==rtl+256 else 0
  eventpos=(t,stage);observe_return();counts[e]+=1
  if e=='hardware_done':
   for i in (0,1):
    if v&(1<<i):busy[i]=False;done[i]+=1
  elif e=='hardware_issue':
   for i in (0,1):
    if v&(1<<i):busy[i]=True;issues[i]+=1
   if v&2:
    current_gen=issues[1]
    if unassigned is not None:
     unassigned['next_A_hardware_issue']=point(r,line);generation_windows[current_gen]=unassigned
  elif e=='child_create':children.setdefault(p,[]).append(c)
  elif e in ('AR','AW'):
   cs=children.pop(p,[])
   if e=='AR' and a in VPTE:
    rec={'AR':ref(r,line),'children':{uid:{} for uid in cs},'len':v};records[p]=rec;active[p]={'children':cs}
   ar=role(a);rolecounts[ar]+=1
   if ar.startswith('A_') and current_gen in generation_windows:
    w=generation_windows[current_gen]
    if 'first_next_A_physical' not in w:w['first_next_A_physical']=dict(point(r,line),role=ar)
    group='payload' if 'payload' in ar else 'metadata'
    key='first_next_A_'+group
    if key not in w:w[key]=dict(point(r,line),role=ar)
  elif e=='callback' and p in active:
   if c in records[p]['children']:records[p]['children'][c]['callback']=ref(r,line)
  elif e=='actual_R_data' and p in active:
   records[p].setdefault('first_actual_R',ref(r,line))
  elif e=='R' and p in active:
   if v==records[p]['len']:
    records[p]['R_last']=ref(r,line);del active[p]
  elif e=='cfg_read' and a==0x28:
   nexts[p]+=1;lastid[p]=v;match_seen[p]=False
   if p==1 and unassigned is not None:
    unassigned['next_A_NEXT_capture']=point(r,line);unassigned['next_A_NEXT_id']=v;unassigned=None
  elif e=='cfg_read' and a==0x30:
   if done_baseline[p] is None:done_baseline[p]=v
   if lastid[p] is not None and v==lastid[p] and not match_seen[p]:
    swmatched[p]+=1;match_seen[p]=True
    if p==1:
     w={'A_done_id':v,'DONE_capture':point(r,line),'software_return_earliest_dram_tick':t+1,'software_return_next_rtl_tick':rtl+2}
     windows.append(w);unassigned=w;due.append(w)
# No later source events expected; flush possible final scheduled point.
eventpos=(10**30,1);observe_return()
refills=[w for w in windows if 'next_A_NEXT_capture' in w];drains=[w for w in windows if 'next_A_NEXT_capture' not in w]
assert nexts==issues==done==swmatched==[256,257],(nexts,issues,done,swmatched)
assert len(refills)==256 and len(drains)==1
for w in refills:
 assert 'next_A_hardware_issue' in w and 'first_next_A_physical' in w
 for q in ('metadata','payload'):
  event=w.get('first_next_A_'+q)
  if event:
   existing=w['at_earliest_software_return']['V_PTE_pending_actual_R'];existingcb=w['at_earliest_software_return']['V_PTE_pending_callback']
   event['same_existing_V_PTE_still_pending_actual_R']=sorted(set(existing)&set(event['V_PTE_pending_actual_R']))
   event['same_existing_V_PTE_still_pending_callback']=sorted(set(existingcb)&set(event['V_PTE_pending_callback']))
 for stage in ('DONE_capture','at_earliest_software_return','first_next_A_physical','first_next_A_metadata','first_next_A_payload'):
  if stage not in w:continue
  event=w[stage];tick=w['software_return_earliest_dram_tick'] if stage=='at_earliest_software_return' else event['dram_tick']
  event['V_PTE_lifetimes']=[{'parent_uid':uid,'AR_dram_tick':records[uid]['AR']['dram_tick'],'callback_dram_ticks':[records[uid]['children'][c].get('callback',{}).get('dram_tick') for c in records[uid]['children']], 'actual_R_last_tick':records[uid].get('R_last',{}).get('dram_tick'),'remaining_until_last_R_ticks':records[uid].get('R_last',{}).get('dram_tick',tick)-tick} for uid in event['V_PTE_pending_actual_R']]
stats={}
for stage in ('DONE_capture','at_earliest_software_return','first_next_A_physical','first_next_A_metadata','first_next_A_payload'):
 xs=[w[stage] for w in refills if stage in w]
 stats[stage]={'windows':len(xs),'V_hw_busy':sum(x['V_hardware_busy'] for x in xs),'V_PTE_callback_pending':sum(bool(x['V_PTE_pending_callback']) for x in xs),'V_PTE_actual_R_pending':sum(bool(x['V_PTE_pending_actual_R']) for x in xs),'busy_and_PTE_callback_pending':sum(x['V_hardware_busy'] and bool(x['V_PTE_pending_callback']) for x in xs),'busy_and_PTE_R_pending':sum(x['V_hardware_busy'] and bool(x['V_PTE_pending_actual_R']) for x in xs),'same_existing_at_return_still_callback_pending':sum(bool(x.get('same_existing_V_PTE_still_pending_callback')) for x in xs),'same_existing_at_return_still_R_pending':sum(bool(x.get('same_existing_V_PTE_still_pending_actual_R')) for x in xs)}
report={'material_passport':{'scope':'one existing p_row0 COPY warm trace; read-only source-bound projection','requested_model':'gpt-6.1-sol','requested_effort':'medium','actual_model':'unknown','actual_effort':'unknown','DeepSeek_used':False},'raw_sha256':digest.hexdigest(),'source_sha256':{str(SOURCE):sha(SOURCE)},'source_lines':{'cfg_xact_capture_then_extra_tick':[1430,1451],'warm_V_first_A_second_refill_and_final_drain':[2130,2197]},'raw_rows_streamed':raw_rows,'crosscheck':{'NEXT_counts':nexts,'hardware_issue':issues,'hardware_done':done,'software_first_matching_DONE':swmatched,'DONE_baseline':done_baseline,'legal_A_refill_windows':len(refills),'excluded_A_final_drain':len(drains)},'stats':stats,'windows':refills,'excluded_drain':drains,'V_PTE_lifecycles':records,'role_counts':dict(rolecounts),'duration_seconds':time.monotonic()-start,'no_model_execution':True,'no_counterfactual_run':True,'signal_implementation_and_information_value_unmeasured':True}
(OUT/'analysis.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'stats':stats,'crosscheck':report['crosscheck'],'duration_seconds':report['duration_seconds']},indent=2))
