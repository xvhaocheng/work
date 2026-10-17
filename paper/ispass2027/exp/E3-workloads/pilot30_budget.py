#!/usr/bin/env python3
"""Estimate150 from the actual30-point pilot, including collector overhead; no UART."""
import argparse
import hashlib
import json
import math
from pathlib import Path


def require(ok,message):
    if not ok:raise ValueError(message)


def calculate(pilot,setup_seconds,total_seconds=21600):
    require(pilot['fixture_only'] is False and pilot['evidence_level']=='fpga_board' and pilot['source_board_cases']==pilot['source_cases']==30,
            'actual30-point board pilot required; synthetic or partial cells cannot set paper budget')
    require(pilot['protocol']=='equal16MiB_fullVring_v1' and pilot['planned_cases']==150 and len(pilot['rows'])==30
            and all(row['n']==1 for row in pilot['rows']) and pilot['runtime_forecast']['all_cells_observed'],'one actual trial in every fixed candidate cell required')
    require(math.isfinite(setup_seconds) and setup_seconds>=0 and math.isfinite(total_seconds) and total_seconds>setup_seconds+130,'finite real setup/total budget required')
    f=pilot['runtime_forecast'];wall=f['accumulated_collector_wall_seconds'];commands=f['observed_command_seconds']
    require(type(wall) in (float,int) and math.isfinite(wall) and wall>0 and type(commands) in (float,int)
            and math.isfinite(commands) and 0<=commands<=wall+1e-9,'actual collector and command wall times required')
    # Five times the entire first block also accounts conservatively for its
    # parser/startup overhead; no unobserved cell borrows another workload's time.
    projected=5*wall;reserve=130
    return {'schema':'ispass2027.full_e3.pilot30_budget.v1','status':'actual_pilot_estimate','fixture_only':False,
        'source_board_cases':30,'new_board_measurements':0,'source_boot_id':pilot['boot_id'],
        'setup_seconds':setup_seconds,'total_board_budget_seconds':total_seconds,'collector_cap_seconds':total_seconds-setup_seconds,
        'actual_pilot_collector_seconds':wall,'actual_pilot_command_seconds':commands,'pilot_collector_overhead_seconds':max(0,wall-commands),
        'projected150_collector_seconds':projected,'projected_remaining120_seconds':4*wall,
        'projected150_plus_setup_seconds':setup_seconds+projected,'existing_next_point_reserve_seconds':reserve,
        'candidate150_fits_estimate_with_reserve':setup_seconds+projected+reserve<=total_seconds,
        'decision_owner':'Root; no automatic continuation; estimate is not a runtime guarantee',
        'scope':'same150 protocol; pilot already occurs once in continuation; collector cap subtracts setup and reuses accumulated wall time'}


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--analysis',type=Path,required=True)
    p.add_argument('--setup-seconds',type=float,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    require(not a.output.exists(),'new budget output required');raw=a.analysis.read_bytes();result=calculate(json.loads(raw),a.setup_seconds)
    result['source_analysis']={'path':str(a.analysis.resolve()),'sha256':hashlib.sha256(raw).hexdigest()}
    result['script_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    with a.output.open('x') as out:json.dump(result,out,indent=2,allow_nan=False);out.write('\n')
    print(json.dumps({'status':result['status'],'candidate150_fits_estimate_with_reserve':result['candidate150_fits_estimate_with_reserve'],'output':str(a.output)}))

if __name__=='__main__':main()
