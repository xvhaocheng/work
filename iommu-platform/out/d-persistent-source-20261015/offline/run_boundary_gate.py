#!/usr/bin/env python3
"""MQ-BOUNDARY-1 fixed eight-run gate. No retries, tuning, or source modifications.
Preparation only until invoked with --execute. Strict interpretation belongs to root.
"""
from pathlib import Path
import argparse
import datetime
import hashlib
import json
import shutil
import subprocess
import sys
import time
import xml.etree.ElementTree as ET

BASE = Path(__file__).resolve().parent
CONTRACT = BASE.parent / 'root/boundary-contract.json'
ORIGINAL = BASE / 'MQSim-51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16'
INSTRUMENTED = BASE / 'instrumented'
INPUTS = BASE / 'inputs'


def utc():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(chunk)
    return h.hexdigest()


def record(path):
    return {'path': str(path), 'bytes': path.stat().st_size, 'sha256': sha(path)}


def write_json(path, value):
    path.write_text(json.dumps(value, indent=2) + '\n')


def verify_prepared():
    frozen = json.loads((BASE / 'instrumented-prebuild-manifest.json').read_text())
    if sha(CONTRACT) != frozen['contract_sha256']:
        raise RuntimeError('Contract changed from build-time frozen hash')
    # Original source and both complete binary/object trees must match accepted builds.
    for item in json.loads((BASE / 'original-source-manifest.json').read_text())['files']:
        if sha(ORIGINAL / item['path']) != item['sha256']:
            raise RuntimeError('Original source changed: ' + item['path'])
    for item in json.loads((BASE / 'build-artifacts.json').read_text())['artifacts']:
        if sha(ORIGINAL / item['path']) != item['sha256']:
            raise RuntimeError('Original build artifact changed: ' + item['path'])
    for item in frozen['files']:
        if sha(BASE / item['path']) != item['sha256']:
            raise RuntimeError('Instrumented source/patch/input changed: ' + item['path'])
    for item in json.loads((BASE / 'instrumented-build-artifacts.json').read_text()):
        if sha(BASE / item['path']) != item['sha256']:
            raise RuntimeError('Instrumented artifact changed: ' + item['path'])
    contract = json.loads(CONTRACT.read_text())
    if contract['id'] != 'MQ-BOUNDARY-1' or contract['workload']['lengths'] != [512, 2048, 4096, 8192]:
        raise RuntimeError('Unexpected frozen case list')
    if contract['execution_budget'] != {
        'max_runs': 8, 'per_run_wall_timeout_s': 60, 'instrumented': 4,
        'uninstrumented_for_semantic_equivalence': 4, 'no_outcome_based_retuning': True
    }:
        raise RuntimeError('Unexpected run budget')
    return contract


def run_one(length, variant, runs):
    verify_prepared()  # Also catches any changes between runs.
    source = ORIGINAL if variant == 'original' else INSTRUMENTED
    binary = source / 'MQSim'
    directory = runs / str(length) / variant
    directory.mkdir(parents=True, exist_ok=False)
    config = directory / 'ssdconfig.xml'
    workload = directory / 'workload.xml'
    shutil.copy2(INPUTS / 'ssdconfig.xml', config)
    shutil.copy2(INPUTS / ('workload-%d.xml' % length), workload)
    trace = INPUTS / ('write-%d.trace' % length)
    if ET.parse(workload).findtext('.//File_Path') != str(trace):
        raise RuntimeError('Copied workload does not point to frozen trace')
    command = [str(binary), '-i', str(config), '-w', str(workload)]
    source_paths = sorted(p for p in (source / 'src').rglob('*') if p.is_file()) + [source / 'Makefile']
    manifest = {
        'recorded_utc': utc(), 'length': length, 'variant': variant,
        'command': command, 'cwd': str(directory), 'wall_timeout_seconds': 60,
        'stdin': 'DEVNULL (main cin.get receives EOF; source unchanged)',
        'contract': record(CONTRACT), 'runner': record(Path(__file__).resolve()),
        'binary': record(binary), 'sources': [record(p) for p in source_paths],
        'config': record(config), 'workload_xml': record(workload), 'trace': record(trace),
        'source_files_are_build_bound': True,
    }
    write_json(directory / 'pre-run-manifest.json', manifest)
    result = {'length': length, 'variant': variant, 'start_utc': utc(),
              'command': command, 'cwd': str(directory), 'timeout_seconds': 60,
              'pre_run_manifest_sha256': sha(directory / 'pre-run-manifest.json')}
    start = time.monotonic()
    with (directory / 'stdout.log').open('wb') as out, (directory / 'stderr.log').open('wb') as err:
        try:
            process = subprocess.run(command, cwd=directory, stdin=subprocess.DEVNULL,
                                     stdout=out, stderr=err, timeout=60, check=False)
            result.update(exit_code=process.returncode, timed_out=False)
        except subprocess.TimeoutExpired:
            # subprocess.run kills and waits for the direct simulator process.
            result.update(exit_code=None, timed_out=True, failure='wall_timeout')
        except OSError as exc:
            result.update(exit_code=None, timed_out=False, failure='launch_error',
                          diagnostic=str(exc))
    result.update(end_utc=utc(), elapsed_seconds=time.monotonic() - start)
    result['output_xml'] = [record(p) for p in sorted(directory.glob('*scenario*.xml'))]
    result['stdout'] = record(directory / 'stdout.log')
    result['stderr'] = record(directory / 'stderr.log')
    result['binary_after'] = record(binary)
    result['trace_after'] = record(trace)
    result['config_after'] = record(config)
    result['workload_after'] = record(workload)
    result['input_or_binary_changed'] = any(
        manifest[key]['sha256'] != result[key + '_after']['sha256']
        for key in ('binary', 'trace', 'config')) or (
        manifest['workload_xml']['sha256'] != result['workload_after']['sha256'])
    if variant == 'instrumented':
        selected = []; malformed = []
        with (directory / 'stderr.log').open(errors='replace') as stream:
            for line_number, line in enumerate(stream, 1):
                try:
                    event = json.loads(line)
                    if event.get('event') in ('host_boundary_after_complete_event', 'engine_exit_state', 'engine_exit'):
                        selected.append(event)
                except (ValueError, AttributeError):
                    malformed.append(line_number)
        result['observer_selected_events'] = selected
        result['stderr_non_json_line_numbers'] = malformed
    result['execution_ok'] = result['exit_code'] == 0 and not result['timed_out'] and not result['input_or_binary_changed']
    write_json(directory / 'run-result.json', result)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--execute', action='store_true', help='Execute the frozen maximum eight runs')
    args = parser.parse_args()
    if not args.execute:
        parser.error('No execution without --execute; preparation-only delivery')
    contract = verify_prepared()
    runs = BASE / 'runs'
    # Never overwrite or extend an earlier execution, including an incomplete attempt.
    runs.mkdir(exist_ok=False)
    summary = {'contract': record(CONTRACT), 'script': record(Path(__file__).resolve()),
               'start_utc': utc(), 'max_runs': 8, 'runs': [],
               'interpretation': 'Extraction/execution only; strict acceptance by root separately'}
    write_json(runs / 'execution-summary.json', summary)
    for length in contract['workload']['lengths']:
        for variant in ('original', 'instrumented'):
            try:
                result = run_one(length, variant, runs)
            except Exception as exc:
                summary.update(stopped_early=True, stop_reason='preparation_or_manifest_error',
                               diagnostic=str(exc), end_utc=utc())
                write_json(runs / 'execution-summary.json', summary)
                return 1
            summary['runs'].append(result)
            if not result['execution_ok']:
                summary.update(stopped_early=True, stop_reason='run_failed_or_input_changed', end_utc=utc())
                write_json(runs / 'execution-summary.json', summary)
                return 1
            write_json(runs / 'execution-summary.json', summary)
    summary.update(stopped_early=False, end_utc=utc(), executed_runs=len(summary['runs']))
    write_json(runs / 'execution-summary.json', summary)
    return 0


if __name__ == '__main__':
    sys.exit(main())
