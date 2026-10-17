#!/usr/bin/env python3
"""Export one immutable E2 prefix, explicitly incomplete; full exporter unchanged."""
import argparse
import csv
import json
from pathlib import Path
import prefix_common as prefix


def export(snapshot,output,sample_set,*,_fixture_mode=False):
    output=Path(output).resolve()
    prefix.require(output.is_relative_to(prefix.HERE) and not output.exists(),'new E2 task-local table directory required')
    snapshot=Path(snapshot).resolve()
    prefix.require(not output.is_relative_to(snapshot),'table output must be outside immutable snapshot')
    header,manifest,records,markers,observed=prefix.read_snapshot(snapshot,_fixture_mode=_fixture_mode)
    prefix.require(not output.is_relative_to(Path(header['source_run'])),'table output must be outside original run')
    result=prefix.summarize(header,manifest,records,markers);n=len(records)
    rows=prefix.full.long_rows(result,output/'summary.json',f'prefix{n}of90.'+sample_set)
    for row in rows:
        row['experiment']='synthetic_e2_prefix' if result['fixture_only'] else 'board_e2_prefix'
        row['row_id']=row['row_id'].replace('synthetic_e2.','synthetic_e2_prefix.',1).replace('board_e2.','board_e2_prefix.',1)
        row['sample_set']=f"e2:partial:{n}/90:"+('fixture:' if result['fixture_only'] else 'accepted:')+sample_set
        row['condition']+=f";partial=1;observed_total={n};planned_total=90;source_board_cases={result['source_board_cases']};source_campaign_status={result['original_campaign_status']};source_status_after_capture={result['producer_status_after_capture']};unaccepted_commands={len(result['observed_unaccepted_commands'])};overlapping_views_do_not_pool=1"
    prefix.require(all(path.read_bytes()==raw for path,raw in observed.items()),'frozen input changed during analysis')
    output.mkdir(parents=True,exist_ok=False)
    (output/'summary.json').write_text(prefix.full.encoded(result))
    with (output/'paper.csv').open('x',newline='') as stream:
        writer=csv.DictWriter(stream,prefix.full.FIELDS,lineterminator='\n');writer.writeheader();writer.writerows(rows)
    (output/'analysis-manifest.json').write_text(prefix.full.encoded({'schema':'ispass2027.e2_prefix.export.v1','status':'passed',
        'derived_snapshot':True,'fixture_only':result['fixture_only'],'evidence_level':result['evidence_level'],
        'source_board_cases':result['source_board_cases'],'source_cases':n,'planned_cases':90,'campaign_complete':False,
        'source_run':header['source_run'],'accepted_case_range':[1,n],'members':result['members'],'overlap_policy':result['overlap_policy'],
        'snapshot_manifest':str((Path(snapshot)/'manifest.json').resolve()),
        'inputs':[{'path':str(path),'sha256':prefix.sha(raw)} for path,raw in observed.items()],
        'exporter_sha256':prefix.sha(Path(__file__).read_bytes()),'prefix_validator_sha256':prefix.sha(Path(prefix.__file__).read_bytes()),
        'unchanged_full_exporter_sha256':prefix.sha(Path(prefix.full.__file__).read_bytes()),'rows':len(rows)}))
    return result,rows


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--snapshot',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--sample-set',required=True)
    a=p.parse_args();result,rows=export(a.snapshot,a.output,a.sample_set)
    print(json.dumps({'status':result['status'],'observed_cases':result['source_cases'],'campaign_complete':False,
        'source_board_cases':result['source_board_cases'],'rows':len(rows),'output':str(a.output.resolve())}))


if __name__=='__main__':main()
