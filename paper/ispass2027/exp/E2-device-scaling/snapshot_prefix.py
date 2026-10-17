#!/usr/bin/env python3
"""Freeze one observed E2 prefix1..89; no UART, source editing or retries."""
import argparse
import json
from pathlib import Path
import prefix_common as prefix


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--run',type=Path,required=True)
    p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();result=prefix.freeze(a.run,a.source_root,a.output)
    print(json.dumps({'status':'frozen','observed_cases':result['observed_cases'],'planned_cases':90,
        'campaign_complete':False,'source_board_cases':result['source_board_cases'],
        'unaccepted_commands':len(result['observed_unaccepted_commands']),'output':str(a.output.resolve())}))


if __name__=='__main__':main()
