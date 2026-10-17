#!/usr/bin/env python3
"""Offline descriptive E6-PN analysis; only actual passed formal collections."""
import argparse
from collections import defaultdict
import csv
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
GROUP = ('phase', 'ds', 'boot_id', 'condition', 'v_gap_cycles')
METRICS = ('V_ms', 'A_ms', 'union_ms', 'V_MiB_s', 'A_MiB_s', 'union_MiB_s')


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


def imported(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def load_run(run, root, formal, gate):
    reader = gate.Inputs(root, run, False)
    def read(path):
        resolved, value = reader.read(path)
        require(not synthetic(value), 'synthetic source cannot enter paper analysis: ' + str(resolved))
        return resolved, value
    _, manifest = read(run / 'manifest.json')
    _, cached = read(run / 'records.json')
    require(manifest['status'] == 'passed', 'only complete actual passed phases are accepted')
    phase = next(k for k, v in formal.PHASES.items() if v[0] == manifest['phase'])
    script = reader.resolve(manifest['script_source'])
    require(script == Path(formal.__file__).resolve(), 'use the actual saved formal runner source')
    raw_script = script.read_bytes()
    require(gate.sha(raw_script) == manifest['script_sha256'], 'as-executed formal source changed')
    reader.observed[script] = raw_script
    plan = reader.resolve(manifest['plan_source'])
    reader.observed[plan] = plan.read_bytes()
    require(gate.sha(reader.observed[plan]) == manifest['plan_sha256'], 'actual plan changed')
    points = formal.read_plan(plan, phase)
    require(manifest['planned_cases'] == points, 'cached plan differs from actual plan')
    require(manifest['recorded_cases'] == len(cached) == len(points), 'complete phase count required')
    receipts = {}
    for name in ('load_manifest', 'source_manifest', 'software_manifest', 's15_gate'):
        path, value = read(manifest[name])
        require(gate.sha(reader.observed[path]) == manifest[name+'_sha256'], 'receipt changed: '+name)
        receipts[name] = value
    loaded, source, admission = (receipts[x] for x in ('load_manifest', 'source_manifest', 's15_gate'))
    require(loaded['status'] == 'boot_identity_verified' and loaded['kernel_release'] == gate.KERNEL,
            'actual original kernel/load identity required')
    require(manifest['kernel_release'] == gate.KERNEL and manifest['boot_id'] == loaded['boot_id'] and
            manifest['bit_sha256'] == loaded['bit_sha256'], 'collection/load identity mismatch')
    require(source['linux'] == gate.LINUX and admission['status'] == 'passed' and
            admission['eligible_for_DS50'] is True, 'qualified same-source S15 required')
    require(admission['selected_cva6'] == source['selected_cva6'] and
            admission['selected_iommu'] == source['iommu'], 'S15/source mismatch')
    require((loaded['bit_sha256'] != admission['bit_sha256']) if phase == 'ds50'
            else (loaded['bit_sha256'] == admission['bit_sha256']), 'phase BIT/S15 mismatch')
    require(manifest['ds'] == formal.PHASES[phase][1], 'phase DS mismatch')
    if 'ds' in loaded:
        require(loaded['ds'] == manifest['ds'], 'load DS mismatch')
    mapping_path = Path(formal.__file__).resolve().parent.parent / 'tool-source-map.json'
    mapping_raw = mapping_path.read_bytes()
    require(gate.sha(mapping_raw) == gate.MAP_SHA, 'original source map changed')
    reader.observed[mapping_path] = mapping_raw
    mapping = json.loads(mapping_raw)
    require(not synthetic(mapping), 'synthetic source map')
    gate.software(reader, mapping, reader.resolve(manifest['software_manifest']))
    parse, identity = formal.formal_parser(gate, reader, mapping)
    require(identity == manifest['parser'], 'actual parser/adaptation identity mismatch')
    actions = manifest['control_actions']
    require(len(actions) == 2*len(points), 'exact two actual control actions per trial required')
    trials, previous = [], None
    for number, (point, item) in enumerate(zip(points, cached), 1):
        require(all(item[k] == v for k, v in point.items()) and item['case'] == number,
                'trial order/point relabeling')
        raw_path, raw = read(item['raw_command_path'])
        require(raw['exit_code'] == 0 and raw['command'] == gate.command(point['condition']),
                'actual original CLI command/success required')
        parsed = parse(raw['stdout'], point['v_gap_cycles'])
        require(not synthetic(parsed) and all(parsed[k] == item[k] for k in
                ('record', 'observer', 'path', 'metrics', 'boot_id', 'sequence')), 'raw/cache mismatch')
        require(item['boot_id'] == manifest['boot_id'] and
                (previous is None or item['sequence'] == previous+1), 'actual same-boot consecutive sequence required')
        previous = item['sequence']
        record = parsed['record']
        require(record['input']['aggressor_id'] == 1 and
                record['input']['aggressor_pages'] == point['a_pages'] and
                record['result']['victim_did'] == 10 and record['result']['aggressor_did'] == 11,
                'actual roles/pages differ from plan')
        for action, operation, value in zip(actions[2*(number-1):2*number],
                                            ('set-mode', 'set-gap'), (point['mode_word'], point['v_gap_cycles'])):
            require(action['trial_id'] == point['trial_id'] and action['operation'] == operation,
                    'control action order mismatch')
            _, control_raw = read(action['raw_command_path'])
            sent = formal.control_command(f'pn-config {operation} {value}', item['boot_id'], gate.KERNEL)
            control = formal.decode(gate, control_raw, sent, 'idma.pn_config.v1', item['boot_id'])
            require(not synthetic(control) and control['read_only'] is False and control['operation'] == operation and
                    control['requested_value'] == value and control['functional_write_attempts'] == 1 and
                    control['readback_verified'] is True and control['after']['mode_word'] == point['mode_word'],
                    'actual guarded control write/readback required')
            if operation == 'set-gap':
                require(control['after']['gap_word'] == point['v_gap_cycles'], 'gap readback mismatch')
        pn_path, pn_raw = read(item['pn_readback_path'])
        control = formal.decode(gate, pn_raw, formal.control_command('pn-control read', item['boot_id'], gate.KERNEL),
                                'idma.pn_control.v1', item['boot_id'])
        require(not synthetic(control) and control == item['pn_readback'] and
                control['operation'] == 'read' and control['read_only'] is True, 'post-read raw/cache mismatch')
        whole = formal.validate_capture(control, point, parsed)
        require(whole == item['whole_work'], 'whole-work cache mismatch')
        v, a = (record['result'][role] for role in ('victim', 'aggressor'))
        cycles = parsed['observer']['time']['cycles']
        require(cycles > 0 and v['completed_bytes'] > 0, 'positive actual V work/time required')
        trial = {k: item[k] for k in (*GROUP, 'block', 'case', 'trial_id', 'mode', 'mode_word', 'sequence')}
        trial.update(V_ms=cycles/50000, A_ms=whole['a_own_cycles']/50000,
                     union_ms=whole['union_cycles']/50000,
                     V_MiB_s=parsed['metrics']['hardware_MiB_s'], A_MiB_s=whole['a_own_MiB_s'],
                     union_MiB_s=whole['union_MiB_s'], V_bytes=v['completed_bytes'], A_bytes=a['completed_bytes'],
                     n_accept_v=whole['n_accept_v'], n_accept_a=whole['n_accept_a'],
                     raw_command_path=str(raw_path), raw_sha256=gate.sha(reader.observed[raw_path]),
                     pn_readback_path=str(pn_path), pn_sha256=gate.sha(reader.observed[pn_path]))
        require(math.isclose(trial['V_MiB_s'], (trial['V_bytes']/1048576)*50e6/cycles, rel_tol=1e-12),
                'V rate/own hardware denominator mismatch')
        trials.append(trial)
    reader.unchanged()
    return trials, dict(path=str(run), phase=manifest['phase'], ds=manifest['ds'],
                        boot_id=manifest['boot_id'], n=len(trials), sources=reader.sources())


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
    ap.add_argument('--source-root', type=Path)
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
    require(args.run and args.source_root and args.output, '--run, --source-root and --output required')
    root, output = args.source_root.resolve(), args.output.resolve()
    require(output.is_relative_to(HERE) and output != HERE and not output.exists(), 'new output directory under this analysis task required')
    formal = imported(root/'paper/ispass2027/exp/E6-pn/tools/collect_formal.py', 'e6_analysis_formal')
    gate = imported(Path(formal.__file__).parent/'s15_gate.py', 'e6_analysis_gate')
    trials, runs, unique, collections = [], [], set(), set()
    for run in args.run:
        rows, receipt = load_run(run.resolve(), root, formal, gate)
        collection = tuple(receipt[k] for k in ('phase', 'ds', 'boot_id'))
        require(collection not in collections, 'choose one collection per actual phase/DS/boot; no cumulative pooling')
        collections.add(collection)
        for row in rows:
            key = row['boot_id'], row['sequence'], row['raw_sha256']
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
