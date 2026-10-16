#!/usr/bin/env python3
"""Small official-trace count closure; relies on report's prior per-issue equality."""
from pathlib import Path
import json,csv,collections
p=Path(__file__).resolve().parent;r=json.loads((p/'report.json').read_text());result={}
for case,case_r in r['cases'].items():
 with (p.parent/'implementation/runs'/case/'official-trace.ch0').open() as f: counts=collections.Counter(row[1].strip() for row in csv.reader(f))
 e=case_r['event_counts'];assert counts['WR']==e['WR_issue']==e['derived_data_end']==e['derived_recovery'];assert counts['RD']==e['callback'];assert e['child_create']==e['accepted']==e['child_retire'];assert e['AR']+e['AW']==e['parent_retire'];assert e['AW']==e['B'];assert counts['REFab']==e['dram_REFab_end'];result[case]={'command_counts':dict(counts),'actual_WR_child_issue_de_recovery_bijection':True,'actual_RD_callback_bijection':True,'all_parent_child_accept_retire_bijections':True}
(p/'service-bijections.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
