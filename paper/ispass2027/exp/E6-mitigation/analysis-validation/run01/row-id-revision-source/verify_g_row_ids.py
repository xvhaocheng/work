#!/usr/bin/env python3
"""Focused campaign-prefix regression; reuse the preserved synthetic summary."""
import argparse
import hashlib
import json
from pathlib import Path
import sys

sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(HERE.parent))
import analyze_g as analysis


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--analysis-json',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();value=json.loads(a.analysis_json.read_text())
    assert value['fixture_only'] is True
    all_ids=[]
    for sample_set in ['campaign-a','campaign-b']:
        rows=analysis.long_rows(value,a.analysis_json,sample_set)
        expected='synthetic_G.'+analysis.TABLES.slug('synthetic_'+sample_set)+'.'
        ids={row['row_id'] for row in rows}
        assert len(ids)==len(rows) and all(row.startswith(expected) for row in ids)
        assert all(analysis.TABLES.locate(value,row['source_locator'])==row['value'] for row in rows)
        all_ids.append(ids)
    assert all_ids[0].isdisjoint(all_ids[1])
    result=dict(status='passed',fixture_only=True,evidence_level='synthetic_protocol_fixture',source_board_cases=0,
        checks=['all row IDs retain experiment/sample_set prefix','distinct sample_set ID sets are disjoint'],
        rows_each=len(all_ids[0]),input_summary=str(a.analysis_json.resolve()),
        input_sha256=hashlib.sha256(a.analysis_json.read_bytes()).hexdigest(),
        sources={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [HERE.parent/'analyze_g.py',Path(__file__)]})
    with a.output.open('x') as out:out.write(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':'passed','rows_each':len(all_ids[0]),'output':str(a.output)}))


if __name__=='__main__':main()
