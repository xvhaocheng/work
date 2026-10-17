#!/usr/bin/env python3
"""Formal PN main/gap/DS50 only; S15 stays on its accepted readonly collector."""
import argparse
import ast
from collections import defaultdict
import csv
import datetime
import importlib.util
import inspect
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
if not __debug__:
    raise RuntimeError('Original parser assertions require ordinary Python')
HERE = Path(__file__).resolve().parent
BOARD = HERE.parents[2] / 'board'
PHASES = {'main': ('e6-pn-main', '0/0', 150), 'gap': ('e6-pn-gap', '0/0', 30),
          'ds50': ('e6-pn-ds50', '50/50', 40)}


def module(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def save(path, value):
    path.write_text(json.dumps(value, indent=2, allow_nan=False) + '\n')


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def formal_parser(gate, reader, mapping):
    _original, accepted, identity = gate.parser(reader, mapping)
    # Reuse the accepted outer PATH0 checks. Its private adapted namespace is
    # per invocation; no gate/helper source or S15 instance is changed.
    adapted = inspect.getclosurevars(accepted).nonlocals['adapted']
    fn = next(n for n in ast.parse(reader.observed[Path(identity['path'])]).body
              if isinstance(n, ast.FunctionDef) and n.name == 'parse')
    shared = [n for n in fn.body if isinstance(n, ast.Assert) and ast.unparse(n.test) ==
              "a['shared_ar'] == v['ar'] + a['ar'] and a['shared_aw'] == v['aw'] + a['aw']"]
    assert len(shared) == 1
    fn.body.remove(shared[0])  # Same removal already made by gate.parser.
    cadence = [n for n in ast.walk(fn) if isinstance(n, ast.Compare) and
               ast.unparse(n) == "t['gap'] == n - 1"]
    assert len(cadence) == 1
    cadence[0].comparators[0] = ast.Name(id='expected_frontend_gap', ctx=ast.Load())
    namespace = dict(re=re, json=json, defaultdict=defaultdict, expected_frontend_gap=4095)
    exec(compile(ast.fix_missing_locations(ast.Module(body=[fn], type_ignores=[])),
                 identity['path'], 'exec'), namespace)
    adapted['parse'] = namespace['parse']
    def parse(text, gap):
        namespace['expected_frontend_gap'] = 4095 * (gap + 1)
        return accepted(text)
    return parse, dict(identity, formal_gap_rule='gap == (4096-1)*(configured_gap+1); gap0 unchanged')


def read_plan(path, phase):
    name, ds, count = PHASES[phase]
    with path.open(newline='') as handle:
        points = [r for r in csv.DictReader(handle) if r['phase'] == name]
    assert len(points) == count
    integers = ('block', 'position_in_block', 'mode_word', 'history_shared', 'a_pages',
                'v_gap_cycles', 'control_writes_allowed', 'victim_transfers',
                'warmup_transfers', 'chunk_bytes')
    for r in points:
        for key in integers:
            r[key] = int(r[key])
        assert r['ds'] == ds and r['mode_word'] in (0, 1, 2, 3, 6)
        assert r['history_shared'] == int(r['mode_word'] == 6)
        assert r['condition'] == 'A' + str(r['a_pages'])
        assert r['v_gap_cycles'] in (0, 1000, 4000) and r['control_writes_allowed'] == 1
        assert (r['victim_transfers'], r['warmup_transfers'], r['chunk_bytes']) == (4096, 256, 4096)
    expected = ({(m, a, 0) for m in (0, 1, 2, 3, 6) for a in (1, 8, 64)} if phase == 'main'
                else {(m, 8, g) for m in (2, 3) for g in (0, 1000, 4000)} if phase == 'gap'
                else {(m, 8, 0) for m in (0, 1, 2, 3)})
    blocks = defaultdict(list)
    for r in points:
        blocks[r['block']].append(r)
    assert sorted(blocks) == list(range(5 if phase == 'gap' else 10))
    for rows in blocks.values():
        assert {(r['mode_word'], r['a_pages'], r['v_gap_cycles']) for r in rows} == expected
        assert [r['position_in_block'] for r in rows] == list(range(len(expected)))
    return points


def control_command(executable, boot, kernel):
    # Shell checks precede every functional write; mismatch cannot write first.
    return ('pn_k=$(uname -r); pn_b=$(cat /proc/sys/kernel/random/boot_id); '
        'pn_t=$(cat /proc/sys/kernel/tainted); '
        'printf "KERNEL=%s\\nBOOT=%s\\nTAINT=%s\\n" "$pn_k" "$pn_b" "$pn_t"; '
        f'if [ "$pn_k" = "{kernel}" ] && [ "$pn_b" = "{boot}" ] && [ "$pn_t" = 0 ]; '
        f'then {executable}; else false; fi')


def decode(gate, raw, command, schema, boot):
    assert raw['exit_code'] == 0 and raw['command'] == command
    text = raw['stdout'].replace('\r', '')
    assert re.findall(r'^BOOT=(.+)$', text, re.M) == [boot]
    assert re.findall(r'^KERNEL=(.+)$', text, re.M) == [gate.KERNEL]
    assert re.findall(r'^TAINT=(\d+)$', text, re.M) == ['0']
    decoder = json.JSONDecoder()
    found = [decoder.raw_decode(text[m.start():])[0] for m in
             re.finditer(r'\{\s*"schema"\s*:\s*' + re.escape(json.dumps(schema)), text)]
    assert len(found) == 1
    result = found[0]
    assert not gate.markers(result) and result['success'] is True and result['errno'] == 0
    return result


def validate_capture(control, point, item):
    raw = control['raw']
    assert all(type(raw[str(i)]) is int and 0 <= raw[str(i)] < 2**64 for i in range(192, 200))
    assert raw['192'] == 0x49444d41504e0001 and control['gap_cap'] == 0x49444d4147415001
    assert raw['193'] == control['mode_word'] == point['mode_word']
    assert control['v_gap_cycles'] == point['v_gap_cycles']
    status, flags = raw['194'], raw['199']
    assert status & ((1 << 63) - 64) == 0 and status & 63 == 17
    assert point['mode_word'] != 6 or status >> 63 == 1
    assert flags == 7, 'capture must be frozen/A-valid/union-valid, no error/reserved flag'
    n_v, n_a, a_cycles, union_cycles = (raw[str(i)] for i in (195, 196, 197, 198))
    assert n_v < 2**32 and n_a < 2**32
    if not point['mode_word'] & 1:
        assert n_v == n_a == 0, 'N-disabled mode cannot claim N accepts'
    assert a_cycles > 0 and union_cycles >= max(a_cycles, item['observer']['time']['cycles'])
    v, a = (item['record']['result'][role] for role in ('victim', 'aggressor'))
    assert a is not None and a['completed'] > 0 and a['completed_bytes'] == a['completed'] * 4096
    return dict(n_accept_v=n_v, n_accept_a=n_a, a_own_cycles=a_cycles, union_cycles=union_cycles,
        flags=flags, a_own_MiB_s=(a['completed_bytes'] / 1048576) * 50e6 / a_cycles,
        union_MiB_s=((v['completed_bytes'] + a['completed_bytes']) / 1048576) * 50e6 / union_cycles)


def collect(args):
    gate = module(HERE / 's15_gate.py', 'pn_formal_gate')
    reader = gate.Inputs(args.source_root, args.output, False)
    points = read_plan(args.plan, args.phase)
    mapping_path = HERE.parent / 'tool-source-map.json'
    mapping_raw = mapping_path.read_bytes()
    assert gate.sha(mapping_raw) == gate.MAP_SHA
    reader.observed[mapping_path] = mapping_raw
    mapping = json.loads(mapping_raw)
    gate.software(reader, mapping, args.software_manifest)
    loaded = reader.read(args.load_manifest)[1]
    source = reader.read(args.source_manifest)[1]
    admission = reader.read(args.s15_gate)[1]
    assert loaded['status'] == 'boot_identity_verified' and loaded['kernel_release'] == gate.KERNEL
    boot = loaded['boot_id']
    assert re.fullmatch(r'[0-9a-f-]{36}', boot)
    assert source['linux'] == gate.LINUX
    assert admission['status'] == 'passed' and admission['eligible_for_DS50'] is True
    assert admission['selected_cva6'] == source['selected_cva6'] and admission['selected_iommu'] == source['iommu']
    if args.phase != 'ds50':
        assert loaded['bit_sha256'] == admission['bit_sha256']
    else:
        assert loaded['bit_sha256'] != admission['bit_sha256'], 'DS50 needs its distinct loaded BIT'
    if 'ds' in loaded:
        assert loaded['ds'] == PHASES[args.phase][1]
    parse, parser_identity = formal_parser(gate, reader, mapping)
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    manifest = dict(phase=PHASES[args.phase][0], ds=PHASES[args.phase][1], status='running',
        started_utc=now(), kernel_release=gate.KERNEL, boot_id=boot, bit_sha256=loaded['bit_sha256'],
        planned_cases=points, recorded_cases=0, automatic_retries=0, control_actions=[],
        script_source=str(Path(__file__).resolve()), script_sha256=gate.sha(Path(__file__).read_bytes()),
        parser=parser_identity, plan_source=str(args.plan.resolve()), plan_sha256=gate.sha(args.plan.read_bytes()),
        execution_argv=sys.argv)
    for field in ('load_manifest', 'source_manifest', 'software_manifest', 's15_gate'):
        path = reader.resolve(getattr(args, field))
        manifest[field] = str(path)
        manifest[field + '_sha256'] = gate.sha(reader.observed[path])
    records, console, previous = [], None, None
    save(out / 'records.json', records); save(out / 'manifest.json', manifest)
    try:
        for point in points:
            if args.stop_file and args.stop_file.exists():
                manifest['status'] = 'preempted'
                break
            if console is None:
                uart = module(BOARD / 'uart.py', 'pn_formal_uart')
                console = uart.Console(args.port or uart.DEFAULT_PORT, out / 'uart')
            for operation, value in (('set-mode', point['mode_word']), ('set-gap', point['v_gap_cycles'])):
                sent = control_command(f'pn-config {operation} {value}', boot, gate.KERNEL)
                raw = console.command(sent, 60)
                manifest['control_actions'].append(dict(trial_id=point['trial_id'], operation=operation,
                                                       raw_command_path=raw['raw_path']))
                save(out / 'manifest.json', manifest)
                result = decode(gate, raw, sent, 'idma.pn_config.v1', boot)
                assert result['read_only'] is False and result['operation'] == operation and result['requested_value'] == value
                assert result['functional_write_attempts'] == 1 and result['readback_verified'] is True
                assert result['after']['mode_word'] == point['mode_word']
                if operation == 'set-gap': assert result['after']['gap_word'] == point['v_gap_cycles']
            sent = gate.command(point['condition'])
            raw = console.command(sent, 180)
            assert raw['exit_code'] == 0 and raw['command'] == sent
            item = parse(raw['stdout'], point['v_gap_cycles'])
            assert not gate.markers(item) and item['boot_id'] == boot
            assert previous is None or item['sequence'] == previous + 1
            assert item['record']['input']['aggressor_id'] == 1
            assert item['record']['input']['aggressor_pages'] == point['a_pages']
            assert item['record']['result']['victim_did'] == 10 and item['record']['result']['aggressor_did'] == 11
            sent = control_command('pn-control read', boot, gate.KERNEL)
            captured = console.command(sent, 60)
            control = decode(gate, captured, sent, 'idma.pn_control.v1', boot)
            assert control['operation'] == 'read' and control['read_only'] is True
            whole = validate_capture(control, point, item)
            previous = item['sequence']
            item.update(point, case=len(records) + 1, raw_command_path=raw['raw_path'],
                        pn_readback_path=captured['raw_path'], pn_readback=control, whole_work=whole)
            records.append(item)
            manifest['recorded_cases'] = len(records)
            save(out / 'records.json', records); save(out / 'manifest.json', manifest)
            print(json.dumps(dict(trial_id=point['trial_id'], hardware_ms=item['metrics']['hardware_ms'])), flush=True)
        else:
            manifest['status'] = 'passed'
    except BaseException as error:
        manifest.update(status='failed', error=type(error).__name__ + ': ' + str(error))
        raise
    finally:
        try:
            if console is not None: console.close()
        except BaseException as error:
            manifest.update(status='failed', cleanup_error=type(error).__name__ + ': ' + str(error))
            raise
        finally:
            manifest.update(finished_utc=now(), recorded_cases=len(records))
            save(out / 'manifest.json', manifest)
    return manifest


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--phase', choices=PHASES, required=True)
    for field in ('source-root', 'load-manifest', 'source-manifest', 'software-manifest', 's15-gate', 'output'):
        parser.add_argument('--' + field, type=Path, required=True)
    parser.add_argument('--plan', type=Path, default=HERE.parent / 'plans/matrix235-v1.csv')
    parser.add_argument('--stop-file', type=Path)
    parser.add_argument('--port')
    collect(parser.parse_args())
