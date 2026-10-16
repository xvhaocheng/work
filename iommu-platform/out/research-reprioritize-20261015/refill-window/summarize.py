#!/usr/bin/env python3
"""Derive interval intersections from saved lifecycle JSON; never rereads CSV."""
import json,hashlib,datetime
from pathlib import Path
OUT=Path(__file__).resolve().parent
inputs={'p_row0':OUT/'analysis.json','pn_row0':OUT/'pn_row0-analysis.json'}
# Preserve initial P result before writing the combined analysis artifact.
if (OUT/'p_row0-analysis.json').exists():inputs['p_row0']=OUT/'p_row0-analysis.json'
else:(OUT/'p_row0-analysis.json').write_bytes(inputs['p_row0'].read_bytes());inputs['p_row0']=OUT/'p_row0-analysis.json'
reports={case:json.loads(p.read_text()) for case,p in inputs.items()}
def key(x):return (x['dram_tick'],1 if x['dram_tick']==x['rtl_tick']+256 else 0)
summary={}
for case,r in reports.items():
 intervals=[]
 for i,w in enumerate(r['windows']):
  lo=(w['software_return_earliest_dram_tick'],.5);hi=key(w['next_A_hardware_issue']);pending=[];new=[]
  # Source has no intervening V cfg NEXT in the A program_single/trigger block.
  # Do not equate this limited pre-submit block with post-submit job identity.
  for uid,p in r['V_PTE_lifecycles'].items():
   if key(p['AR'])<hi and key(p['R_last'])>lo:
    pending.append(int(uid))
    if key(p['AR'])>lo:new.append(int(uid))
  intervals.append({'window_index':i,'A_done_id':w['A_done_id'],'software_earliest_return':lo[0],'known_next_A_already_hw_issued_tick':hi[0],'PTE_pending_at_any_point_before_next_A_hw_issue':pending,'newly_accepted_PTE_after_decision_before_next_A_hw_issue':new})
 witnesses=[]
 for i,w in enumerate(r['windows']):
  if w['at_earliest_software_return']['V_PTE_pending_actual_R']:
   for uid in w['at_earliest_software_return']['V_PTE_pending_actual_R']:
    p=r['V_PTE_lifecycles'][str(uid)];rt=p['R_last']['dram_tick'];ct=max(x['callback']['dram_tick'] for x in p['children'].values())
    witnesses.append({'window_index':i,'parent_uid':uid,'child_uids':list(map(int,p['children'])),'PTE_address':p['AR']['address'],'AR':p['AR'],'A_DONE_capture':w['DONE_capture'],'software_earliest_return_tick':w['software_return_earliest_dram_tick'],'callback':[x['callback'] for x in p['children'].values()],'actual_R_last':p['R_last'],'next_A_NEXT_capture':w['next_A_NEXT_capture'],'next_A_first_metadata':w.get('first_next_A_metadata'),'next_A_first_payload':w.get('first_next_A_payload'),'callback_ticks_remaining_at_decision':ct-w['software_return_earliest_dram_tick'],'R_ticks_remaining_at_decision':rt-w['software_return_earliest_dram_tick'],'R_ticks_remaining_at_next_NEXT_capture':rt-w['next_A_NEXT_capture']['dram_tick'],'R_already_expired_ticks_before_A_metadata':w['first_next_A_metadata']['dram_tick']-rt,'R_already_expired_ticks_before_A_payload':w['first_next_A_payload']['dram_tick']-rt})
 summary[case]={'raw_sha256':r['raw_sha256'],'raw_rows_streamed':r['raw_rows_streamed'],'duration_seconds':r['duration_seconds'],'crosscheck':r['crosscheck'],'stats':r['stats'],'interval_count_with_PTE_pending':sum(bool(x['PTE_pending_at_any_point_before_next_A_hw_issue']) for x in intervals),'interval_count_with_new_PTE_after_decision':sum(bool(x['newly_accepted_PTE_after_decision_before_next_A_hw_issue']) for x in intervals),'intervals':intervals,'witnesses':witnesses,'same_V_generation_at_first_A_metadata_or_payload':'unknown: snapshots retain busy but not V generation; no third CSV read','busy_vs_walk_scope':'Pending counts include any currently accepted V PTE at each checkpoint, including successors; zero at a checkpoint does not assert that the whole translation walk ended.'}
combined={'material_passport':{'method':'ARS-Codex bounded source/raw evidence extraction','requested_model':'gpt-6.1-sol','requested_effort':'medium','actual_model':'unknown','actual_effort':'unknown','DeepSeek_used':False},'cases':summary,'limits':{'CSV_full_reads':{'p_row0':1,'pn_row0':1},'P_bounded_schema_excerpt_lines':[6025,6048],'no_row1':True,'no_model_or_build_or_old_checker':True,'not_SPDK_native':True,'counterfactuals_information_value_and_signal_implementation':'unmeasured'}}
(OUT/'analysis.json').write_text(json.dumps(combined,indent=2)+'\n')
print(json.dumps({k:{'interval_count_with_PTE_pending':v['interval_count_with_PTE_pending'],'interval_count_with_new_PTE_after_decision':v['interval_count_with_new_PTE_after_decision']} for k,v in summary.items()}))
