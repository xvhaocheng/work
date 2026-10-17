#!/usr/bin/env python3
"""Focused OBS4 experiment gate with the widened OBS4/OBS5 shared helper."""
import argparse
import copy
import json
from pathlib import Path
import re
import sys
import collect_pages as collector


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--e5-dir',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();_,hist=collector.e5_modules(a.e5_dir)
    path=Path(collector.__file__).with_name('pages-collector-fixtures.json')
    raw=json.loads(path.read_text())['leaf4k']
    item=collector.parse(raw['stdout'],collector.KERNEL,'leaf4k',hist)
    assert item['histogram']['info']['version']==4 and item['histogram']['info']['schema']==1
    changed=copy.deepcopy(raw)
    changed['stdout']=re.sub(r'(idma_hist_info[^\n]+)version=4 schema=1 bins=32 count_bits=32 time_bits=32 overflow=0 raw=0x20202001',
        r'\1version=5 schema=2 bins=32 count_bits=32 time_bits=32 overflow=0 raw=0x0040000020202002 depth=64',changed['stdout'])
    assert changed['stdout']!=raw['stdout']
    attachment=hist.attach_from_output(changed['stdout'],item)
    assert attachment['info']['version']==5 and attachment['info']['schema']==2 and attachment['info']['depth']==64
    try:collector.parse(changed['stdout'],collector.KERNEL,'leaf4k',hist)
    except AssertionError as error:assert 'E4 requires OBS4/INFO1 without depth' in str(error)
    else:raise AssertionError('self-consistent OBS5 escaped E4 version gate')
    assert 'uart' not in sys.modules
    a.output.write_text(json.dumps({'schema':'ispass2027.e4_pagesize.hist_version.host.v1','status':'passed',
        'fixture_only':True,'source_board_cases':0,'collector_sha256':collector.sha(collector.__file__),
        'histogram_validator_sha256':collector.sha(a.e5_dir/'hist_records.py'),'locked_fixture_sha256':collector.sha(path),
        'checks':[{'name':'original_OBS4_INFO1_unchanged_accept','passed':True},
                  {'name':'self_consistent_OBS5_accepted_by_shared_helper_rejected_by_E4','passed':True}],
        'uart_imported':False},indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':2,'source_board_cases':0,'uart_imported':False}))


if __name__=='__main__':main()
