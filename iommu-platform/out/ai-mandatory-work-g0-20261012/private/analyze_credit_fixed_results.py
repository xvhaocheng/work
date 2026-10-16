#!/usr/bin/env python3
"""Summarize registered G0 cases, keeping scope and completion distinct."""
import csv
import hashlib
import json
from pathlib import Path

BATCH = Path(__file__).resolve().parents[1]


def main():
    register = json.loads((BATCH / 'credit-fixed-g0-register.json').read_text())
    rows, costs, raw = [], [], {}
    for case in register['cases']:
        path = BATCH / 'credit-fixed-runs' / case['id'] / 'summary.json'
        s = json.loads(path.read_text())
        assert s['case_id'] == case['id'] and s['page_bytes'] == case['page_bytes']
        assert s['useful_bytes'] == len(case['offered_roles']) * 9447936
        assert len(s['jobs']) == 6 * len(case['offered_roles'])
        assert all(j['ready'] == 0 and j['issue'] <= j['done'] <= j['done_reg_cycle']
                   and j['next_id'] == j['done_id'] for j in s['jobs'])
        raw[case['id']] = s
        roles = s['roles']
        rows.append(dict(case=case['id'], useful_bytes=s['useful_bytes'],
                         jobs=len(s['jobs']),
                         role0_hardware_done=roles[0]['hardware_done_cycle'],
                         role1_hardware_done=roles[1]['hardware_done_cycle'],
                         role0_polled_done=roles[0]['polled_done_cycle'],
                         role1_polled_done=roles[1]['polled_done_cycle'],
                         all_hardware_done=s['all_hardware_done_cycle'],
                         natural_quiet=s['natural_quiet_cycle'],
                         validation_end=s['validation_end_cycle'],
                         other_completed_bytes=s['other_role_completed_useful_bytes_at_first_complete']))
        assert s['private_credit_boundary_fix'] and s['credit_observer_only']
        assert s['credit_terminal']==[0,0] and all(x<=8 for x in s['credit_max'])
        m = s['mapping']
        costs.append(dict(case=case['id'], mapped_payload_bytes=41943040,
                          model_payload_backing_bytes=50331648,
                          pte_pages=m['pte_pages'], pte_reserved_bytes=m['pte_reserved_bytes'],
                          ddt_reserved_bytes=m['ddt_reserved_bytes'],
                          metadata_reserved_bytes=m['metadata_reserved_bytes'],
                          pte_init_write64_count=m['pte_init_write64_count'],
                          ddt_init_write64_count=m['ddt_init_write64_count'],
                          cfg_writes=s['cfg']['writes'], cfg_reads=s['cfg']['reads'],
                          cfg_write_cycles=s['cfg']['write_cycles'],
                          cfg_read_cycles=s['cfg']['read_cycles'],
                          metadata_ar=s['metadata_traffic']['ar'],
                          metadata_ar_bytes=s['metadata_traffic']['ar_bytes']))
    comparisons = []
    for mapping in ['4k', '2m']:
        paired = raw[f'{mapping}-parallel']
        for role in [0, 1]:
            solo = raw[f'{mapping}-solo{role}']
            p, i = paired['roles'][role], solo['roles'][role]
            comparisons.append(dict(mapping=mapping, role=role,
                                    parallel_hardware_done=p['hardware_done_cycle'],
                                    solo_hardware_done=i['hardware_done_cycle'],
                                    shared_and_supply_excess_cycles=p['hardware_done_cycle'] - i['hardware_done_cycle'],
                                    hardware_slowdown=p['hardware_done_cycle'] / i['hardware_done_cycle'],
                                    polled_slowdown=p['polled_done_cycle'] / i['polled_done_cycle'],
                                    interpretation='combined shared path and supply; not PTW-only attribution'))
    sensitivity = []
    for dispatch in ['solo0', 'solo1', 'parallel', 'single']:
        a, b = raw[f'4k-{dispatch}'], raw[f'2m-{dispatch}']
        sensitivity.append(dict(dispatch=dispatch, reduction_in_all_hardware_done_cycles=a['all_hardware_done_cycle'] - b['all_hardware_done_cycle'],
                                reduction_percent=100 * (1 - b['all_hardware_done_cycle'] / a['all_hardware_done_cycle']),
                                interpretation='matched mapping/metadata sensitivity under retained model'))
    decomposed = []
    for case_id, s in raw.items():
        for j in s['jobs']:
            decomposed.append(dict(case=case_id, id=j['id'], role=j['role'], channel=j['channel'],
                                   ready_to_config=j['config_start'], ready_to_issue=j['issue'],
                                   hardware_service=j['done'] - j['issue'],
                                   hardware_to_polled_done=j['done_reg_cycle'] - j['done']))
    for name, values in [('credit-fixed-core-results.csv', rows), ('credit-fixed-costs.csv', costs), ('credit-fixed-job-timing.csv', decomposed)]:
        with (BATCH / name).open('w') as f:
            writer = csv.DictWriter(f, fieldnames=list(values[0]))
            writer.writeheader()
            writer.writerows(values)
    analysis = dict(stage=106, core=rows, costs=costs, shared_comparisons=comparisons,
                    mapping_sensitivity=sensitivity,
                    private_credit_boundary_fix=True, production_changes=False,
                    credit={case_id:{k:s[k] for k in ('credit_max','credit_full_samples','credit_full_drains','credit_terminal')} for case_id,s in raw.items()},
                    boundaries=['solo completes only one package; pair completes both',
                                'single requester is a legal ordinary replacement, not identity-matched causal intervention',
                                'host metadata initialization excluded from RTL t0 and charged as explicit operations/reserve',
                                'no GPU, AI, FPGA board, or complete setup net-performance claim'],
                    summary_hashes={case_id: hashlib.sha256((BATCH / 'credit-fixed-runs' / case_id / 'summary.json').read_bytes()).hexdigest()
                                    for case_id in raw})
    (BATCH / 'credit-fixed-result-analysis.json').write_text(json.dumps(analysis, indent=2) + '\n')
    print(json.dumps(dict(case_count=len(rows), shared_comparisons=comparisons, mapping_sensitivity=sensitivity)))


if __name__ == '__main__':
    main()
