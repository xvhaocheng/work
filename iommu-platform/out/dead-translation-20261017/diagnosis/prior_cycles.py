#!/usr/bin/env python3
"""Read-only aggregation of prior traces; no RTL execution and no historical rewrites."""
import csv,gzip,json,bisect
from pathlib import Path
HERE=Path(__file__).resolve().parent
OLD=HERE.parents[1]/'invalidation-interference-20261017/runs/matrix01'
rows=[]
for domain in ('different','same'):
  for pages in (4,8):
    for mode in ('m1','m2','m3'):
      case=f'{domain}-b{pages}-{mode}-full-api';path=OLD/case
      s=json.loads((path/'summary.json').read_text());m=s['metrics'];t0=s['timing']['t0']
      starts={};copies=[];flush_start=None;flushes=[];cq_states=[]
      with gzip.open(path/'events.csv.gz','rt') as stream:
        for e in csv.DictReader(stream):
          c=int(e['cycle']);typ=e['event']
          if typ=='cq_state':cq_states.append((c,int(e['value'])))
          if e['era']!='measure':continue
          if typ=='copy_launch' and e['lane']=='1':starts[e['job']]=c
          elif typ=='copy_done' and e['lane']=='1':copies.append((starts.pop(e['job']),c))
          elif typ=='cq_round_begin':
            assert flush_start is None;flush_start=c
          elif typ=='cq_round_complete':
            assert flush_start is not None;flushes.append((flush_start,c));flush_start=None
      assert not starts and flush_start is None
      intervals=sorted([(a,b,'copy') for a,b in copies]+[(a,b,'flush') for a,b in flushes])
      assert all(b<=cc for (a,b,_),(cc,d,__) in zip(intervals,intervals[1:]))
      copy=sum(b-a for a,b in copies);fl=sum(b-a for a,b in flushes);complete=m['A_final_flush_end']
      state_totals={str(state):0 for state in range(6)}
      state_cycles=[c for c,_ in cq_states]
      for a,b in flushes:
        idx=bisect.bisect_right(state_cycles,a)-1;assert idx>=0
        pos=a
        while pos<b:
          end=min(b,cq_states[idx+1][0] if idx+1<len(cq_states) else b)
          state_totals[str(cq_states[idx][1])]+=end-pos;pos=end;idx+=1
      assert sum(state_totals.values())==fl
      assert fl==m['cq_total_waiting']
      rows.append(dict(case_id=case,source_summary=str(path/'summary.json'),source_trace=str(path/'events.csv.gz'),a_final_flush=complete,a_last_hw_done=m['T_A'],a_copy_issue_to_done=copy,a_flush_wait=fl,a_residual=complete-copy-fl,copy_intervals=len(copies),flush_intervals=len(flushes),cq_flush_state_cycles=state_totals,a_walks=m['A_walks'],a_pte_bytes=m['A_ptw_memory_bytes'],b_walks=m['B_walks'],T_B=m['T_B']))
result={'scope':'Existing legacy main cases only; half-open [event_begin,event_end), no reruns. copy/flush are disjoint; residual includes host/MMIO/poll/software-model gaps. This is an observed occupancy decomposition, not additive causal recovery.', 'rows':rows}
(HERE/'prior-cycles.json').write_text(json.dumps(result,indent=2)+'\n')
for r in rows:print(r['case_id'],r['a_final_flush'],r['a_copy_issue_to_done'],r['a_flush_wait'],r['a_residual'],r['b_walks'],r['cq_flush_state_cycles'])
