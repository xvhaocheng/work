#!/usr/bin/env python3
"""Offline descriptive E6-PN analysis; only actual passed formal collections."""
import argparse
from collections import defaultdict
import csv
import json
import math
from pathlib import Path
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
GROUP = ('phase', 'ds', 'boot_id', 'condition', 'v_gap_cycles')
METRICS = ('V_ms', 'A_ms', 'union_ms', 'V_MiB_s', 'A_MiB_s', 'union_MiB_s')
PHASES = {'e6-pn-main': ('0/0', 150), 'e6-pn-gap': ('0/0', 30), 'e6-pn-ds50': ('50/50', 40)}


def require(value, message):
    if not value:
        raise ValueError(message)


def synthetic(value):
    if isinstance(value, dict):
        for key, child in value.items():
            if key in {'fixture_only', 'is_fixture', 'synthetic', 'simulation_only',
                       'synthetic_fixture', 'mock_fixture'} and str(child).lower() in {'true', '1'}:
                return True
            if isinstance(child, str):
                if key in {'evidence_level', 'evidence_kind', 'source_kind'} and any(
                        x in child.lower() for x in ('fixture', 'synthetic', 'mock')):
                    return True
                if key in {'schema', 'condition'} and any(x in child.lower() for x in ('fixture', 'mock')):
                    return True
                if key == 'boot_id' and child.startswith('HOST-FIXTURE'):
                    return True
            if synthetic(child):
                return True
    elif isinstance(value, list):
        return any(synthetic(x) for x in value)
    return False


def describe(values):
    ordered = sorted(values)
    require(ordered and all(math.isfinite(x) for x in ordered), 'finite nonempty values required')
    def q(p):
        at = (len(ordered) - 1) * p
        lo, hi = math.floor(at), math.ceil(at)
        return ordered[lo] + (ordered[hi] - ordered[lo]) * (at - lo)
    return dict(n=len(ordered), median=q(.5), q1=q(.25), q3=q(.75), iqr=q(.75)-q(.25))


def load_accepted(run):
    """Read only the two accepted collector products; never reopen raw/source."""
    run = Path(run).resolve()
    manifest = json.loads((run / 'manifest.json').read_text())
    records = json.loads((run / 'records.json').read_text())
    require(not synthetic(manifest) and not synthetic(records), 'synthetic products cannot enter paper analysis')
    require(manifest['status'] == 'passed', 'only complete collector-passed phases are accepted')
    expected_ds, expected_n = PHASES[manifest['phase']]
    require(manifest['ds'] == expected_ds and manifest['recorded_cases'] == len(records) == expected_n and
            len(manifest['planned_cases']) == expected_n, 'complete phase/n required')
    seen = set()
    for item in records:
        require(all(item[k] == manifest[k] for k in ('phase', 'ds', 'boot_id')), 'mixed phase/DS/boot')
        key = item['boot_id'], item['sequence']
        require(type(item['sequence']) is int and item['sequence'] > 0 and key not in seen, 'duplicate/invalid boot+sequence')
        seen.add(key)
    return manifest, records


def load_run(run):
    manifest, records = load_accepted(run)
    trials = []
    for index, item in enumerate(records):
        whole = item['whole_work']
        v, a = (item['record']['result'][role] for role in ('victim', 'aggressor'))
        v_cycles, a_cycles, union_cycles = item['observer']['time']['cycles'], whole['a_own_cycles'], whole['union_cycles']
        v_bytes, a_bytes = v['completed_bytes'], a['completed_bytes']
        require(min(v_cycles, a_cycles, union_cycles, v_bytes, a_bytes) > 0, 'positive actual work/time required')
        trial = {k: item[k] for k in (*GROUP, 'block', 'case', 'trial_id', 'mode', 'mode_word', 'sequence')}
        trial.update(V_ms=v_cycles/50000, A_ms=a_cycles/50000, union_ms=union_cycles/50000,
                     V_MiB_s=v_bytes*50e6/(1048576*v_cycles), A_MiB_s=a_bytes*50e6/(1048576*a_cycles),
                     union_MiB_s=(v_bytes+a_bytes)*50e6/(1048576*union_cycles), V_bytes=v_bytes, A_bytes=a_bytes,
                     n_accept_v=whole['n_accept_v'], n_accept_a=whole['n_accept_a'],
                     raw_command_path=item['raw_command_path'], pn_readback_path=item['pn_readback_path'],
                     records_path=str(run / 'records.json'), record_index=index)
        trials.append(trial)
    references = {k: manifest[k] for k in ('script_source', 'plan_source', 'load_manifest', 'source_manifest',
                                           'software_manifest', 's15_gate') if k in manifest}
    return trials, dict(path=str(run), manifest_path=str(run/'manifest.json'), phase=manifest['phase'],
                        ds=manifest['ds'], boot_id=manifest['boot_id'], n=len(trials), source_references=references)


def statistics(trials):
    groups, blocks = defaultdict(list), defaultdict(dict)
    for trial in trials:
        base = tuple(trial[k] for k in GROUP)
        for metric in METRICS:
            groups[base+(trial['mode'], metric)].append(trial[metric])
        if trial['phase'] != 'e6-pn-ds50' and trial['mode'] in ('P', 'PN'):
            key = base+(trial['block'],)
            require(trial['mode'] not in blocks[key], 'duplicate mode in one actual block')
            blocks[key][trial['mode']] = trial
    absolute = [dict(zip((*GROUP, 'mode', 'metric'), key), **describe(values))
                for key, values in sorted(groups.items())]
    pairs, ratio_groups = [], defaultdict(list)
    for key, modes in sorted(blocks.items()):
        require(set(modes) == {'P', 'PN'}, 'incomplete actual PN/P block; never fabricate a partner')
        p, pn = modes['P'], modes['PN']
        for metric in METRICS:
            require(p[metric] > 0 and pn[metric] > 0, 'positive paired ratio inputs required')
            ratio = pn[metric]/p[metric]
            pairs.append(dict(zip((*GROUP, 'block'), key), metric=metric, ratio_PN_over_P=ratio,
                              P_trial_id=p['trial_id'], PN_trial_id=pn['trial_id'],
                              P_sequence=p['sequence'], PN_sequence=pn['sequence']))
            ratio_groups[key[:-1]+(metric,)].append(ratio)
    paired = [dict(zip((*GROUP, 'metric'), key), **describe(values))
              for key, values in sorted(ratio_groups.items())]
    return absolute, pairs, paired


def write_csv(path, rows):
    if rows:
        with path.open('w', newline='') as handle:
            writer = csv.DictWriter(handle, fieldnames=list(rows[0]))
            writer.writeheader()
            writer.writerows(rows)


def plots(output, absolute, paired):
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    for phase, ds, boot in sorted({tuple(row[k] for k in GROUP[:3]) for row in absolute}):
        independent = phase == 'e6-pn-ds50'
        rows = [r for r in (absolute if independent else paired)
                if (r['phase'], r['ds'], r['boot_id']) == (phase, ds, boot)]
        if not rows:
            continue
        fig, axes = plt.subplots(2, 3, figsize=(10, 5), constrained_layout=True)
        for ax, metric in zip(axes.flat, METRICS):
            selected = [r for r in rows if r['metric'] == metric]
            sortkey = (lambda r: r['mode']) if independent else (lambda r: r['v_gap_cycles'] if phase.endswith('gap') else int(r['condition'][1:]))
            selected.sort(key=sortkey)
            x = list(range(len(selected)))
            ax.errorbar(x, [r['median'] for r in selected],
                        yerr=[[r['median']-r['q1'] for r in selected], [r['q3']-r['median'] for r in selected]],
                        fmt='o', capsize=3)
            labels = [r['mode'] if independent else str(r['v_gap_cycles']) if phase.endswith('gap') else r['condition'] for r in selected]
            ax.set_xticks(x, [f'{label}\nn={r["n"]}' for label, r in zip(labels, selected)])
            ax.set_title(metric)
            ax.set_ylabel(('ms' if metric.endswith('_ms') else 'MiB/s') if independent else 'PN / P block ratio')
            if not independent:
                ax.axhline(1, color='0.5', linewidth=.8)
        fig.suptitle(f'{phase}, DS {ds}, boot {boot[:8]} — median / Type7 IQR')
        name = phase+'-'+boot
        fig.savefig(output/(name+'.pdf'))
        fig.savefig(output/(name+'.png'), dpi=180)
        plt.close(fig)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--run', type=Path, action='append', default=[])
    ap.add_argument('--output', type=Path)
    ap.add_argument('--plots', action='store_true')
    ap.add_argument('--check', action='store_true', help='tiny pure math/boundary check; no collection or paper output')
    args = ap.parse_args()
    if args.check:
        require(describe([1, 2, 3, 4]) == dict(n=4, median=2.5, q1=1.75, q3=3.25, iqr=1.5), 'Type7 check')
        require(describe([2/1, 9/3, 20/4])['median'] == 3, 'ratios are computed before quantiles')
        require(synthetic({'nested': [{'synthetic': True}]}), 'recursive fixture boundary')
        print('passed three pure helper checks; 0 board records; no output or figures')
        return
    require(args.run and args.output, '--run and --output required')
    output = args.output.resolve()
    require(output.is_relative_to(HERE) and output != HERE and not output.exists(), 'new output directory under this analysis task required')
    trials, runs, unique, collections = [], [], set(), set()
    for run in args.run:
        rows, receipt = load_run(run.resolve())
        collection = tuple(receipt[k] for k in ('phase', 'ds', 'boot_id'))
        require(collection not in collections, 'choose one collection per actual phase/DS/boot; no cumulative pooling')
        collections.add(collection)
        for row in rows:
            key = row['boot_id'], row['sequence']
            require(key not in unique, 'overlapping actual trial cannot be counted twice')
            unique.add(key)
        trials.extend(rows)
        runs.append(receipt)
    absolute, pairs, paired = statistics(trials)
    output.mkdir(parents=True)
    for name, rows in [('trials', trials), ('descriptive', absolute), ('paired_blocks', pairs), ('paired_summary', paired)]:
        write_csv(output/(name+'.csv'), rows)
    zero = [dict(zip((*GROUP, 'mode'), key)) for key in sorted({tuple(r[k] for k in (*GROUP, 'mode')) for r in trials if r['mode_word'] & 1})
            if not any(r['n_accept_v']+r['n_accept_a'] for r in trials if tuple(r[k] for k in (*GROUP, 'mode')) == key)]
    summary = dict(schema='ispass2027.e6_pn.analysis.v1', source_board_cases=len(trials), fixture_only=False,
                   runs=runs, quantiles='Type7', paired_ratio='per actual block PN/P, then Type7',
                   DS50='independent mode description only; no historical or cross-boot pairs',
                   zero_accept_cells=zero, descriptive=absolute, paired=paired)
    (output/'summary.json').write_text(json.dumps(summary, indent=2, allow_nan=False)+'\n')
    (output/'report.md').write_text(f'Actual successful trials: {len(trials)}. Descriptive n and Type7 median/Q1/Q3/IQR are in the CSV files.\n\n'
        'PN/P ratios use actual same-phase/DS/boot/block partners, then summarize block ratios. A complete work amount is separately retained; A duration changes need not imply an equal-work latency change. HPM brackets and selected PATH windows are not own/union elapsed denominators.\n\n'
        f'N-enabled cells with all real accept counts zero: {len(zero)}; no activation conclusion is inferred. DS50 is independent descriptive context. No CI, significance claim, cross-boot pairing or missing-point imputation.\n')
    if args.plots:
        plots(output, absolute, paired)
    print(json.dumps(dict(output=str(output), actual_trials=len(trials))))


if __name__ == '__main__':
    main()
