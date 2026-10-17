#!/usr/bin/env python3
"""Plot actual complete40 G own-window pairs; no fixture rendering or board I/O."""
import argparse
import csv
import hashlib
import importlib.util
import io
import json
import math
from pathlib import Path
import platform
import sys

HERE = Path(__file__).resolve().parent
CONDITIONS = ('solo', 'a8-gap0', 'a8-gap1000', 'a8-gap2000')
PROTOCOL = 'G_independent_W256_each_then_4096_each_natural_v1'
INPUTS = ('analysis.json', 'long-table.csv', 'snapshot.json')
SOURCES = {
    'analyze_g.py': 'a88b4d67078e21657c74d1dfe4871cc6a8fefa9a13f54416fcc855a7947f66c1',
    'collect_g.py': '8aa1b1f08e88a6f24a35408fe03a08cb07ff7ef80f34873f5368e9ff740bc211',
    'g_records.py': '70c79c5f99c395f9310e79242e27df42f9ea640642d2a48dd4f1949ad4723845',
    'collector-source/collect_workload.as-reused.py': 'dc2802d0ff240dd0801a0db6f49bb7372ee4d71e45f43040b28f78ac4a3c7fcc',
    'collector-source/hist_records.as-reused.py': '844ebcbaa3a4bd6361d77e7d5d4cfc4789056448677a239a96ab5f6025c6e666',
    'collector-source/legacy_obs_g.py': '7c1c8a3a62cd58f5bc3f6ae84d98a94fd2bb69fa033f74f99d5b9927ae2f008a',
    'collector-source/p0_records.as-reused.py': 'cc2f00df77afa1428eaf7b004544077f07c3f42a8c9c03f643cd03d1a044e14c',
    'analysis-source/table_schema.as-reused.py': '423a745aa9a689b25ec0e5a4233dde11789b7abc1f76612551966b5e66f80462',
    'analysis-source/g_statistics.py': '796caee1c06d1b5e5888e8ae35a8782743ccb135d740fb5fa8d5b5a487ec0e4b',
}


def require(ok, message):
    if not ok: raise ValueError(message)


def sha(raw): return hashlib.sha256(raw).hexdigest()
def encoded(value): return json.dumps(value, indent=2, ensure_ascii=False, allow_nan=False)+'\n'


def synthetic(value):
    if isinstance(value, dict):
        for key, child in value.items():
            if key in {'fixture_only', 'is_fixture', 'simulation_only', 'synthetic_fixture', 'mock_fixture'} and (child is True or str(child).lower() in ('true', '1')): return True
            if key in {'evidence_level', 'evidence_kind', 'source_kind', 'schema', 'condition'} and isinstance(child, str) and any(s in child.lower() for s in ('fixture', 'synthetic', 'mock')): return True
            if key == 'boot_id' and isinstance(child, str) and child.startswith('HOST-FIXTURE'): return True
            if key == 'fixtures' and child: return True
            if synthetic(child): return True
    elif isinstance(value, list): return any(synthetic(x) for x in value)
    return False


def describe(values):
    helper = HERE/'g_statistics.as-reused.py'
    require(sha(helper.read_bytes()) == SOURCES['analysis-source/g_statistics.py'], 'accepted Type7 helper changed')
    spec = importlib.util.spec_from_file_location('g_figure_statistics', helper)
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    return module.describe(values)


def close(actual, expected):
    return type(actual) in (int, float) and type(expected) in (int, float) and math.isfinite(actual) and math.isfinite(expected) and math.isclose(actual, expected, rel_tol=1e-12, abs_tol=1e-12)


def check_stats(values, accepted):
    fresh = describe(values)
    require(fresh.keys() == accepted.keys() and all(v == accepted[k] if v is None else close(v, accepted[k]) for k, v in fresh.items()), 'accepted Type7 statistic differs from individual values')
    return fresh


def validate_header(data):
    require(not synthetic(data), 'synthetic/fixture paper input forbidden recursively')
    require(data['schema'] == 'ispass2027-g-analysis-v1' and data['status'] == 'complete_analysis'
            and data['fixture_only'] is False and data['evidence_level'] == 'fpga_board'
            and data['source_cases'] == data['source_board_cases'] == 40 and data['campaign_complete'] is True,
            'actual complete40 analysis required')
    require(data['protocol'] == PROTOCOL and data['clock_hz'] == 50000000
            and data['kernel_release'] == '6.5.0-rc2-ispass2027-g-v1'
            and data['identity_status'] == 'collector_load_receipt_hash_boot_bound', 'accepted G protocol/identity required')


def derive(data):
    """Pure pair calculations, also used by explicitly synthetic arithmetic tests."""
    groups = {row['condition']: row for row in data['rows']}
    require(len(data['rows']) == 4 and set(groups) == set(CONDITIONS), 'exact four G conditions required')
    samples = {}
    for condition, group in groups.items():
        require(group['n'] == len(group['samples']) == 10, 'ten actual trials per G condition required')
        samples[condition] = {s['block']: s for s in group['samples']}
        require(set(samples[condition]) == set(range(10)), 'ten distinct G blocks required')
        for metric in ('V_whole_cycles', 'V_whole_MiB_s', 'A_whole_cycles', 'A_whole_MiB_s'):
            check_stats([s['metrics'][metric] for s in group['samples']], group['statistics'][metric])
    trials = []
    for condition in CONDITIONS:
        for block in range(10):
            sample = samples[condition][block]; base = samples['a8-gap0'][block]
            m, denominator = sample['metrics'], base['metrics']; has_a = condition != 'solo'
            require(sample['boot_id'] == base['boot_id'] == data['boot_id'], 'same-boot G pairing required')
            for role in ('V', 'A'):
                cycles, rate = m[role+'_whole_cycles'], m[role+'_whole_MiB_s']
                if role == 'A' and not has_a:
                    require(cycles is None and rate is None, 'Solo A must remain N/A')
                else:
                    require(type(cycles) is int and cycles > 0 and close(rate, 16*50000000/cycles), 'own16MiB cycles/throughput disagree')
            trials.append(dict(condition=condition, block=block, case=sample['case'], boot_id=sample['boot_id'], sequence=sample['sequence'],
                raw_command_path=sample['raw_command_path'], raw_sha256=sample.get('raw_sha256'),
                denominator_case=base['case'], denominator_sequence=base['sequence'], denominator_raw_command_path=base['raw_command_path'],
                denominator_raw_sha256=base.get('raw_sha256'),
                V_whole_cycles=m['V_whole_cycles'], V_whole_MiB_s=m['V_whole_MiB_s'],
                A_whole_cycles=m['A_whole_cycles'], A_whole_MiB_s=m['A_whole_MiB_s'],
                gap0_V_whole_cycles=denominator['V_whole_cycles'], gap0_V_whole_MiB_s=denominator['V_whole_MiB_s'],
                gap0_A_whole_cycles=denominator['A_whole_cycles'], gap0_A_whole_MiB_s=denominator['A_whole_MiB_s'],
                V_time_over_gap0=m['V_whole_cycles']/denominator['V_whole_cycles'],
                A_throughput_over_gap0=m['A_whole_MiB_s']/denominator['A_whole_MiB_s'] if has_a else None))
    comparisons = {('V_slowdown_vs_Solo_ratio'): ('V_whole_cycles', 'ratio'),
        'V_time_vs_Ggap0_ratio': ('V_whole_cycles', 'ratio'),
        'A_own_whole_time_vs_Ggap0_ratio': ('A_whole_cycles', 'ratio'),
        'A_own_whole_throughput_vs_Ggap0_ratio': ('A_whole_MiB_s', 'ratio'),
        'A_own_whole_MiB_s_change_vs_Ggap0': ('A_whole_MiB_s', 'difference')}
    require(len(data['paired_comparisons']) == 11, 'accepted eleven G comparisons required')
    for comparison in data['paired_comparisons']:
        metric, operation = comparisons[comparison['metric']]
        require(comparison['operation'] == operation and comparison['n'] == len(comparison['pairs']) == 10, 'accepted G paired shape differs')
        values = []
        for block, pair in enumerate(comparison['pairs']):
            n, d = samples[comparison['condition']][block], samples[comparison['reference']][block]
            nv, dv = n['metrics'][metric], d['metrics'][metric]; value = nv/dv if operation == 'ratio' else nv-dv
            require(pair['block'] == block and pair['numerator_case'] == n['case'] and pair['denominator_case'] == d['case']
                    and close(pair['numerator'], nv) and close(pair['denominator'], dv) and close(pair['value'], value), 'actual same-block G pair differs')
            values.append(value)
        check_stats(values, comparison['statistics'])
    statistics = {condition: {metric: describe(t[metric] for t in trials if t['condition'] == condition)
        for metric in ('V_time_over_gap0', 'A_throughput_over_gap0')} for condition in CONDITIONS}
    return trials, statistics


def locate(value, pointer):
    for field in pointer[1:].split('/'):
        key = field.replace('~1', '/').replace('~0', '~'); value = value[int(key)] if isinstance(value, list) else value[key]
    return value


def load(source, lock_path):
    lock_raw = lock_path.read_bytes(); lock = json.loads(lock_raw)
    require(not synthetic(lock) and lock['schema'] == 'ispass2027.g.figure_inputs.v1'
            and lock['fixture_only'] is False and lock['source_board_cases'] == 40 and set(lock['inputs_sha256']) == set(INPUTS), 'reviewed actual complete40 input lock required')
    raw = {name: (source/name).read_bytes() for name in INPUTS}
    require(all(sha(raw[name]) == lock['inputs_sha256'][name] for name in INPUTS), 'frozen G export bytes changed')
    data, snapshot = json.loads(raw['analysis.json']), json.loads(raw['snapshot.json'])
    validate_header(data)
    require(not synthetic(snapshot) and snapshot['fixture_only'] is False and snapshot['files'] == data['input_snapshot'], 'actual analysis snapshot index differs')
    originals = {}; snapshot_raw = {}
    for entry in snapshot['files']:
        name = Path(entry['snapshot_path']).name; body = (source/'inputs'/name).read_bytes()
        require(name not in snapshot_raw and entry['original_path'] not in originals and len(body) == entry['bytes'] and sha(body) == entry['sha256'], 'snapshot duplicate/SHA/length differs')
        if name.endswith('.json'): require(not synthetic(json.loads(body)), 'synthetic nested snapshot source forbidden')
        snapshot_raw[name] = body; originals[entry['original_path']] = entry
    for name, digest in SOURCES.items():
        found = [entry for path, entry in originals.items() if path.endswith('/E6-mitigation/'+name)]
        require(len(found) == 1 and found[0]['sha256'] == digest, 'accepted G analysis source changed: '+name)
    records = data['validated_records']; require(len(records) == 40, 'forty validated raw trials required')
    for index, record in enumerate(records):
        condition = CONDITIONS[(index % 4 + index//4 % 4) % 4]
        require(record['case'] == index+1 and record['block'] == index//4 and record['condition'] == condition
                and record['boot_id'] == data['boot_id'] and (not index or record['sequence'] == records[index-1]['sequence']+1), 'G frozen plan/boot/sequence differs')
    for row in data['rows']:
        for sample in row['samples']:
            record = records[sample['case']-1]; entry = originals[sample['raw_command_path']]
            require(all(sample[k] == record[k] for k in ('case', 'block', 'boot_id', 'sequence', 'raw_command_path', 'snapshot_raw_path'))
                    and row['condition'] == record['condition'], 'G displayed trial/raw reference differs')
            require(all(sample['metrics'][k] == record['metrics'][k] for k in ('V_whole_cycles', 'A_whole_cycles', 'V_whole_MiB_s', 'A_whole_MiB_s')), 'G displayed metrics differ from validated trial')
            raw_trial = json.loads(snapshot_raw[Path(entry['snapshot_path']).name])
            require(raw_trial['exit_code'] == 0, 'failed raw G trial cannot plot')
            sample['raw_sha256'] = entry['sha256']
    # Added provenance fields are not exported metrics; the original summary bytes are retained unchanged.
    trials, statistics = derive(data)
    reader = csv.DictReader(io.StringIO(raw['long-table.csv'].decode())); count = 0
    require(len(reader.fieldnames) == 17, 'accepted common17 CSV required')
    for item in reader:
        if item['metric'] not in ('V_whole_cycles', 'A_whole_cycles', 'V_whole_MiB_s', 'A_whole_MiB_s') or item['statistic'] != 'sample': continue
        require(item['evidence_level'] == 'fpga_board' and item['boot_id'] == data['boot_id'], 'G CSV evidence/boot differs')
        value = locate(data, item['source_locator'])
        require(item['value'] == '' if value is None else close(float(item['value']), value), 'G CSV sample pointer/value differs')
        count += 1
    require(count == 160, 'G CSV must retain all40 V/A cycle/throughput samples including Solo N/A')
    return data, raw, snapshot_raw, lock_raw, trials, statistics


def draw(trials, statistics, output):
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    plt.rcParams.update({'font.family': 'DejaVu Sans', 'font.size': 8, 'pdf.fonttype': 42, 'svg.fonttype': 'none', 'axes.spines.top': False, 'axes.spines.right': False})
    fig, axes = plt.subplots(1, 2, figsize=(7.15, 2.8), layout='constrained')
    fig.suptitle('10 paired blocks; all points and Type7 median/IQR', fontsize=8)
    for ax, metric, title, ylabel in zip(axes, ('V_time_over_gap0', 'A_throughput_over_gap0'),
            ('(a) V own 16 MiB time', '(b) A own 16 MiB throughput'), ('V time / G gap0', 'A throughput / G gap0')):
        conditions = CONDITIONS if metric.startswith('V') else CONDITIONS[1:]
        positions = {condition: i for i, condition in enumerate(conditions)}
        for block in range(10):
            pairs = [next(t for t in trials if t['condition'] == c and t['block'] == block) for c in CONDITIONS[1:]]
            offset = (block-4.5)*.025
            ax.plot([positions[t['condition']]+offset for t in pairs], [t[metric] for t in pairs], color='.75', lw=.65, zorder=1)
        for condition in conditions:
            selected = [t for t in trials if t['condition'] == condition]; x = positions[condition]
            color = '#666666' if condition == 'solo' else '#0072B2'
            ax.scatter([x+(t['block']-4.5)*.025 for t in selected], [t[metric] for t in selected], s=16,
                marker='D' if condition == 'solo' else 'o', color=color, linewidth=.35, edgecolor='white', zorder=3)
            stats = statistics[condition][metric]
            ax.errorbar(x+.21, stats['median'], yerr=[[stats['median']-stats['q1']], [stats['q3']-stats['median']]],
                fmt='_', color='black', markersize=9, capsize=3, lw=1.3, zorder=4)
        ax.axhline(1, color='.4', ls='--', lw=.7, zorder=0); ax.grid(axis='y', color='.92', lw=.5)
        ax.set(xticks=list(positions.values()), xticklabels=['Solo\nG reference' if c == 'solo' else c.removeprefix('a8-gap') for c in conditions],
            xlabel='A inter-COPY gap (cycles)', ylabel=ylabel, title=title, xlim=(-.3, len(positions)-.6))
        ax.margins(y=.16)
        if metric.startswith('A'): ax.text(.02, .97, 'Solo A: N/A', ha='left', va='top', transform=ax.transAxes, fontsize=7)
    for extension in ('pdf', 'svg', 'png'):
        metadata = {'CreationDate': None, 'ModDate': None} if extension == 'pdf' else {'Date': None} if extension == 'svg' else {}
        fig.savefig(output/('g-own-windows.'+extension), dpi=300, metadata=metadata)
    plt.close(fig)
    return {'python': platform.python_version(), 'executable': sys.executable, 'matplotlib': matplotlib.__version__, 'backend': 'Agg'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True); parser.add_argument('--input-lock', type=Path, required=True); parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args(); output = args.output.resolve()
    require(output.is_relative_to(HERE.resolve()) and not output.exists(), 'new figures-local output required')
    data, raw, snapshots, lock_raw, trials, statistics = load(args.source.resolve(), args.input_lock.resolve())
    output.mkdir(); inputs = output/'inputs'; inputs.mkdir(); originals = inputs/'inputs'; originals.mkdir()
    for name, body in raw.items(): (inputs/name).write_bytes(body)
    for name, body in snapshots.items(): (originals/name).write_bytes(body)
    (inputs/'input-lock.json').write_bytes(lock_raw)
    for name in ('plot_g_own_windows.py', 'g_statistics.as-reused.py'): (output/name.replace('.py', '.as-executed.py')).write_bytes((HERE/name).read_bytes())
    with (output/'paired-own-windows.csv').open('x', newline='') as stream:
        writer = csv.DictWriter(stream, list(trials[0]), lineterminator='\n'); writer.writeheader(); writer.writerows(trials)
    (output/'statistics.json').write_text(encoded(statistics))
    runtime = draw(trials, statistics, output)
    manifest = dict(schema='ispass2027.g.own_window_figure.v1', status='passed', fixture_only=False, source_board_cases=40, new_board_measurements=0,
        source=str(args.source.resolve()), input_sha256={name: sha(body) for name, body in raw.items()}, input_lock_sha256=sha(lock_raw),
        script_sha256=sha(Path(__file__).read_bytes()), boot_id=data['boot_id'], bit_sha256=data['bit_sha256'], kernel_release=data['kernel_release'],
        protocol=data['protocol'], windows=data['windows'], quantile_method=data['quantile_method'], runtime=runtime,
        rendering='40 V ratios;30 A ratios;ten paired blocks per condition;Solo A N/A;black median/IQR not CI;all points shown;no assumed direction',
        paired_references=trials, numerical_statistics=statistics,
        artifacts={str(p.relative_to(output)): dict(sha256=sha(p.read_bytes()), bytes=p.stat().st_size) for p in sorted(output.rglob('*')) if p.is_file()})
    (output/'manifest.json').write_text(encoded(manifest)); print(json.dumps(dict(status='passed', source_board_cases=40, new_board_measurements=0, output=str(output))))


if __name__ == '__main__': main()
