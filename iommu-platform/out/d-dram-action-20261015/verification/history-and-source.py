#!/usr/bin/env python3
"""Independent small phase/source supplement, without replaying model or CSV."""
import json
from pathlib import Path
import yaml
from verify import BASE, CASES, sha,need
out=Path(__file__).resolve().parent
source=BASE/'implementation/preparation-harness.cpp';bridge=BASE/'implementation/ramulator-bridge.cpp'
s=source.read_text();b=bridge.read_text()
checks={
 'AR_AW_8B_INCR_source_guard':'size==3&&burst==1&&(a&7)==0&&len<=255' in b,
 'A_all_completed_ring_pages_source_and_destination_512_words':all(x in s for x in ('const uint32_t pages_done','p < pages_done','i < PAGE_BEATS','"warm A source page was modified"','"warm A destination page was not copied"')),
 'V_source_and_destination_512_words':all(x in s for x in ('i<PAGE_BEATS','"V source page was modified"','"V warm destination page does not match its expected state"')),
 'PAGE_BEATS_equals512':'PAGE_BEATS = 512' in s,
 'preparation_return_before_full_batch':s.index('PREPARATION-ONLY PASS')<s.index('return;',s.index('PREPARATION-ONLY PASS'))<s.index('run_full_batch(name, enable);',s.index('PREPARATION-ONLY PASS')),
 'no_byte_check_marker_without_CHECK':s.index('"full_warm_byte_checks_pass"')>s.index('"V warm destination page does not match its expected state"')}
need(all(checks.values()),'source supplement guard failure')
ledger=json.loads((out/'report.json').read_text())
r={'source_checks':checks,'source_sha256':{str(source):sha(source),str(bridge):sha(bridge)},'nFAW_ticks':26,'nFAW_source':'fixed DDR4.cpp:386-396 x8/2400, and :473 ACT window4','phases':{},'scope':'DDR4 provider state only; full RTL/IOTLB entry equality not captured'}
for case in CASES:
 r['phases'][case]={}
 for label in ('pre_t0_after_configs','strong_final_drain'):
  s=yaml.safe_load((BASE/'implementation/runs'/case/f'phase-{label}.yaml').read_text());now=s['dram']['clock'];long=[v for n in s['dram']['nodes'] for h in n['command_history'] if len(h)>1 for v in h if v>=0]
  maxdeadline=max((v+26 for v in long),default=-1);need(maxdeadline<=now,'ACT window history not yet mature')
  nrefi=s['refresh_0']['nREFI'];expected_refreshes=now//nrefi
  for rank in ('0','1'):
   need(ledger['cases'][case]['refresh_enqueues'].get(rank)==expected_refreshes and ledger['cases'][case]['refresh_completions'].get(rank)==expected_refreshes,'per-rank refresh enqueue/completion period mismatch')
  r['phases'][case][label]={'each_rank_expected_and_completed_refreshes':expected_refreshes,'nREFI':nrefi,'clock':now,'long_history_nonnegative_entries':len(long),'max_ACT_history_plus_nFAW26':maxdeadline,'all_long_history_mature':True}
(out/'history-and-source.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({'source_checks':checks,'history_checks':'8 phase snapshots PASS'}))
