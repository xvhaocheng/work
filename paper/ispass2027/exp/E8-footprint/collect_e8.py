#!/usr/bin/env python3
"""One loaded DS and one E8 subset; no reload, retry, replacement or resume."""
import argparse
import ast
from collections import defaultdict
import datetime
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
if not __debug__:
    raise RuntimeError('Python -O cannot preserve the original parser assertions')
BOARD = Path(__file__).resolve().parents[2] / 'board'
KERNEL = '6.5.0-rc2-ispass2027-pathv3'
LINUX = '45bf76b15e36cf608dff95013d91f8bedb7b1b8e'
# Reserved export interface: records retain the original record/observer/path/metrics.
LONG_TABLE_COLUMNS = ('row_id', 'experiment', 'condition', 'metric', 'statistic',
    'value', 'unit', 'n', 'bitstream_id', 'kernel_id', 'raw_path', 'source_locator',
    'evidence_level', 'value_status', 'identity_status', 'boot_id', 'sample_set')


def save(path, value):
    path.write_text(json.dumps(value, indent=2, allow_nan=False) + '\n')


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def plan(ds, group):
    if group == 'victim':
        if ds != 0:
            raise ValueError('victim subset is DS0 only')
        cells, rounds = [(v, a) for v in (2, 4, 8) for a in (None, 1, 8, 64)], 5
    else:
        cells, rounds = [(1, a) for a in (2, 4, 12, 14, 32)], 10 if ds == 0 else 5
    return [dict(block=b, victim_pages=v, aggressor_pages=1 if a is None else a,
                 condition=f'V{v}_' + ('Solo' if a is None else f'A{a}'), solo=a is None)
            for b in range(rounds)
            for v, a in cells[b % len(cells):] + cells[:b % len(cells)]]


def original_parser(path):
    raw = path.read_bytes()
    fn = next(n for n in ast.parse(raw).body if isinstance(n, ast.FunctionDef) and n.name == 'parse')
    # The sole original check changed is victim_pages == 1; all other AST stays.
    matches = [n for n in ast.walk(fn) if isinstance(n, ast.Compare) and
               ast.unparse(n) == "inputs['victim_pages'] == 1"]
    assert len(matches) == 1, 'original victim-pages assertion changed'
    matches[0].comparators[0] = ast.Name(id='expected_victim_pages', ctx=ast.Load())
    fn.args.args.append(ast.arg(arg='expected_victim_pages'))
    fn.args.defaults.append(ast.Constant(value=1))
    namespace = dict(re=re, json=json, defaultdict=defaultdict)
    exec(compile(ast.fix_missing_locations(ast.Module(body=[fn], type_ignores=[])), str(path), 'exec'), namespace)
    return namespace['parse'], sha(raw)


def command(point):
    aggressor = 'none' if point['solo'] else '1'
    return ('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
        'printf "TAINT="; cat /proc/sys/kernel/tainted; '
        "pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
        'printf "PRE_SEQ=%s\\n" "${pre_seq:-0}"; '
        f'platform-timeout 120 idma_experiment --batch --device /dev/idma0 --aggressor {aggressor} '
        f'--aggressor-pages {point["aggressor_pages"]} --victim-pages {point["victim_pages"]} '
        '--chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000; '
        'case_rc=$?; printf "CASE_EXIT=%s\\n" "$case_rc"; dmesg; '
        'printf "TAINT_AFTER="; cat /proc/sys/kernel/tainted')


def collect(args):
    points = plan(args.ds, args.group)
    refs = {}
    for field in ('load_manifest', 'source_record', 'software_manifest'):
        path = getattr(args, field).resolve()
        raw = path.read_bytes()
        refs[field] = dict(path=str(path), sha256=sha(raw), value=json.loads(raw))
    loaded, source, software = (refs[k]['value'] for k in
                               ('load_manifest', 'source_record', 'software_manifest'))
    assert loaded['status'] == 'boot_identity_verified' and loaded['kernel_release'] == KERNEL
    assert software['status'] == 'passed' and software['selected_linux'] == LINUX
    assert software['kernel_release'] == KERNEL and source['actual_commits']['linux'] == LINUX
    if 'ds' in loaded:
        assert loaded['ds'] in (args.ds, f'{args.ds}/{args.ds}'), 'loaded DS differs'
    parse, parser_sha = original_parser(args.parser_source.resolve())
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    manifest = dict(phase='e8-footprint', ds=f'{args.ds}/{args.ds}', group=args.group,
        status='running', started_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
        kernel_release=KERNEL, boot_id=loaded['boot_id'], bit_sha256=loaded['bit_sha256'],
        planned_cases=points, recorded_cases=0, automatic_retries=0,
        script_source=str(Path(__file__).resolve()), script_sha256=sha(Path(__file__).read_bytes()),
        parser_source=str(args.parser_source.resolve()), parser_sha256=parser_sha,
        parser_change='Only victim_pages==expected_victim_pages; all original assertions retained',
        execution_argv=sys.argv, stop_file=str(args.stop_file.resolve()) if args.stop_file else None,
        long_table_columns=list(LONG_TABLE_COLUMNS))
    for field, ref in refs.items():
        manifest[field] = ref['path']
        manifest[field + '_sha256'] = ref['sha256']
    records, console, previous = [], None, None
    save(out / 'records.json', records)
    save(out / 'manifest.json', manifest)
    try:
        for index, point in enumerate(points):
            if args.stop_file and args.stop_file.exists():
                manifest['status'] = 'preempted'
                break
            if console is None:
                spec = importlib.util.spec_from_file_location('e8_original_uart', BOARD / 'uart.py')
                uart = importlib.util.module_from_spec(spec)
                spec.loader.exec_module(uart)
                console = uart.Console(args.port or uart.DEFAULT_PORT, out / 'uart')
            sent = command(point)
            raw = console.command(sent, 180)
            assert raw['exit_code'] == 0 and raw['command'] == sent, 'command failed; stop without retry'
            item = parse(raw['stdout'], KERNEL, True, expected_victim_pages=point['victim_pages'])
            assert item['boot_id'] == loaded['boot_id'], 'boot changed'
            assert previous is None or item['sequence'] == previous + 1, 'nonconsecutive observer'
            assert (item['record']['result']['aggressor'] is None) == point['solo']
            assert item['record']['input']['aggressor_pages'] == point['aggressor_pages']
            previous = item['sequence']
            item.update(point, case=index + 1, ds=manifest['ds'], raw_command_path=raw['raw_path'])
            records.append(item)
            manifest['recorded_cases'] = len(records)
            save(out / 'records.json', records)
            save(out / 'manifest.json', manifest)
            print(json.dumps(dict(case=index + 1, condition=point['condition'],
                                  hardware_ms=item['metrics']['hardware_ms'])), flush=True)
        else:
            manifest['status'] = 'passed'
    except BaseException as error:
        manifest.update(status='failed', error=type(error).__name__ + ': ' + str(error))
        raise
    finally:
        try:
            if console is not None:
                console.close()
        except BaseException as error:
            manifest.update(status='failed', cleanup_error=type(error).__name__ + ': ' + str(error))
            raise
        finally:
            manifest.update(recorded_cases=len(records),
                            finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
            save(out / 'manifest.json', manifest)
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ds', type=int, choices=(0, 100), required=True)
    parser.add_argument('--group', choices=('victim', 'aggressor'), required=True)
    for name in ('load-manifest', 'source-record', 'software-manifest', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--parser-source', type=Path, default=BOARD / 'collect.py')
    parser.add_argument('--stop-file', type=Path)
    parser.add_argument('--port')
    collect(parser.parse_args())


if __name__ == '__main__':
    main()
