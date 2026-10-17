#!/usr/bin/env python3
"""Resolve the outline's figure selectors to stable IDs and current CSV lines."""
import csv
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent


def selected(figure, row):
    e, m, s, k = (row[x] for x in ['experiment','metric','statistic','row_id'])
    if figure == 2:
        modes = dict(part.split('=', 1) for part in row['condition'].split(';') if '=' in part)
        return ((e.startswith('batch_') and '.v0-' in k and m in ['hardware_ms','hardware_MiB_s','victim_iotlb_miss','aggressor_iotlb_miss'] and s == 'median')
                or (e in ['board_baseline100', 'board_ds_sweep'] and s == 'median' and row['n'] == '10'
                    and row['sample_set'] in ['within_one_run_phase_and_boot',
                                              'pooled_first_and_last_with_explicit_boots']
                    and m in ['hardware_ms','hardware_MiB_s','victim_iotlb_miss','aggressor_iotlb_miss'])
                or (e in ['board_baseline100', 'board_ds_sweep'] and s == 'sample'
                    and m == 'hardware_cycles' and modes.get('attempt_status') == 'accepted'
                    and modes.get('mode') in ['A1', 'A8', 'A16', 'A64']))
    if figure == 3:
        modes = dict(part.split('=', 1) for part in row['condition'].split(';') if '=' in part)
        return ((e == 'path' and m.startswith(('v_r_','v_w_')) and m.endswith('mean_cycles') and s == 'median')
                or (e == 'board_baseline100' and s == 'median' and row['n'] == '10'
                    and modes.get('mode') in ['Solo', 'A1', 'A8']
                    and row['sample_set'] in ['within_one_run_phase_and_boot',
                                              'pooled_first_and_last_with_explicit_boots']
                    and m in [f'path_v{channel}{stage}_mean_cycles'
                              for channel in 'rw' for stage in range(4)]))
    if figure == 4:
        return e == 'e3v6' and row['sample_set'] == 'formal' and m in ['delta_A_remaining','delta_V_remaining','delta_V_wait','delta_v_hw_cycles_delta']
    if figure == 5:
        return e == 'e2' and k.startswith(('e2.boundary-w7-','e2.boundary-w8-')) and m in ['d87_done_issue_cycles','d89_v_main_cycles','d8d_v_miss'] and s == 'median'
    if figure == 6:
        return ((e == 'p4m' and m in ['v_mib_s','a_window_mib_s'] and s == 'median')
                or (e == 'composition' and 'isolated_history' in k and m in ['W','w_pct_change','a_miss_diff','a_miss'] and s in ['sample','paired_difference']))
    if figure == 10:
        return e == 'invalidation_sim' and row['sample_set'] == 'main24' and m in ['T_A','T_B','A_final_flush_end','B_time_over_M0','B_invalidation_attributed_misses']
    return False


def main():
    sources = ['data/board_results.csv','data/invalidation_sim_results.csv']
    result = []
    for filename in sources:
        with (HERE/filename).open() as f:
            rows = list(csv.DictReader(f))
        for number, row in enumerate(rows, 2):
            for figure in [2,3,4,5,6,10]:
                if selected(figure,row):
                    result.append({'figure':figure,'row_id':row['row_id'],'csv_file':filename,
                                   'csv_line':number,'condition':row['condition'],'metric':row['metric']})
    result.sort(key=lambda r:(r['figure'],r['csv_file'],r['csv_line']))
    with (HERE/'figure-rows.csv').open('w',newline='') as f:
        writer=csv.DictWriter(f,fieldnames=['figure','row_id','csv_file','csv_line','condition','metric'])
        writer.writeheader();writer.writerows(result)
    manifest={'schema':'ispass2027-figure-map-v1','rows':len(result),
              'csv_sha256':{p:hashlib.sha256((HERE/p).read_bytes()).hexdigest() for p in sources},
              'figures':{str(n):sum(r['figure']==n for r in result) for n in range(1,11)},
              'non_quantitative_figures':[1],'pending_measurement_figures':[7,8,9]}
    if (HERE/'data/ds-sweep-summary.json').exists():
        manifest['derived_figure_sources'] = {'2/E1b': {
            'summary': 'data/ds-sweep-summary.json', 'generator': 'scripts/plot_ds_sweep.py',
            'estimator': 'median of within-DS, within-block Ak/A1 hardware-cycle ratios minus one',
            'error_bars': 'Type-7 Q1 to Q3 of ten paired ratios',
            'operand_metric': 'hardware_cycles', 'operand_statistic': 'sample'}}
    (HERE/'figure-rows.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(json.dumps(manifest,indent=2))


if __name__=='__main__':main()
