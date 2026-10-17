#!/usr/bin/env python3
"""Four concentrated S15 scheduling checks; private injection, zero board cases."""
import argparse
import contextlib
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import sys
from types import SimpleNamespace

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
REL = Path('paper/ispass2027/exp/E6-pn/tools/collect_s15.py')
BOOT = 'HOST-FIXTURE-S15-SCHEDULING'
PRE_SEQ = 42
CAPS = [0x504e000000000001, 0x49444d4147415001]
CHECKS = 0


def check(condition, message):
    global CHECKS
    CHECKS += 1
    if not condition:
        raise AssertionError(message)


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, allow_nan=False) + '\n')


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def load_module(path):
    spec = importlib.util.spec_from_file_location('actual_s15_collector_host', path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def run_scenario(collector, base, name, *, fail_case=None, fail_cleanup=False,
                 fail_preflight=False):
    start_checks = CHECKS
    directory = base / name
    directory.mkdir()
    run = directory / 'paper/ispass2027/exp/E6-pn/runs/synthetic'
    args = SimpleNamespace(source_root=directory, output=run,
        source_manifest=directory / 'source-manifest.json',
        fpga_manifest=directory / 'fpga-manifest.json',
        load_manifest=directory / 'load-manifest.json',
        software_manifest=directory / 'software-manifest.json',
        expected_cva6='1' * 40, expected_iommu='2' * 40,
        port='HOST-FIXTURE-NO-UART')
    reader = collector.gate.Inputs(directory, run, True)
    for field in ('source_manifest', 'fpga_manifest', 'load_manifest', 'software_manifest'):
        path = getattr(args, field)
        save(path, dict(fixture_only=True, evidence_level='synthetic', source_board_cases=0))
        reader.read(path)
    events, parsed = [], []
    expected = [(collector.gate.READ_COMMAND, 60)]
    expected += [(collector.gate.command(condition), 180)
                 for _, condition in collector.gate.plan()]
    expected += [(collector.gate.READ_COMMAND, 60)]

    def parse_fixture(text):
        parsed.append(text)
        return json.loads(text)

    prepared = dict(reader=reader, parse=parse_fixture,
        parser_identity=dict(fixture_only=True, evidence_level='synthetic',
                             role='scheduling stub; not parser qualification', source_board_cases=0),
        loaded=dict(boot_id=BOOT), caps=CAPS, bit='0' * 64)

    class ConsoleFixture:
        def __init__(self, port, output):
            check(port == args.port, name + ': injected port')
            self.output = output
            self.output.mkdir()
            self.commands = []
            self.closed = 0
            events.append(dict(event='construct'))

        def command(self, command, timeout):
            call = len(self.commands)
            check(call < len(expected), name + ': no extra command or retry')
            check((command, timeout) == expected[call], name + ': exact read/trial order')
            self.commands.append((command, timeout))
            events.append(dict(event='command', command=command, timeout=timeout))
            raw = dict(command=command, exit_code=0, fixture_only=True,
                       evidence_level='synthetic', source_board_cases=0)
            if command == collector.gate.READ_COMMAND:
                watermark = PRE_SEQ if call == 0 else PRE_SEQ + 15
                control = dict(schema='idma.pn_control.v1', success=True, errno=0,
                    operation='read', read_only=True, cap=CAPS[0], gap_cap=CAPS[1],
                    mode=0, v_gap_cycles=0, fixture_only=True, source_board_cases=0)
                raw['stdout'] = (f'KERNEL={collector.gate.KERNEL}\nBOOT={BOOT}\n'
                                 f'PRE_SEQ={watermark}\n' + json.dumps(control) + '\n')
            elif call == fail_case:
                raw.update(exit_code=1, stdout='injected first CLI wrapper failure')
            else:
                _, condition = collector.gate.plan()[call - 1]
                item = dict(boot_id=BOOT, sequence=PRE_SEQ + call,
                    fixture_only=True, evidence_level='synthetic', source_board_cases=0,
                    record=dict(input=dict(aggressor_pages=1 if condition == 'Solo'
                                            else int(condition[1:])),
                                result=dict(aggressor=None if condition == 'Solo' else {})),
                    observer=dict(time=dict(cycles=100000 + call)))
                raw['stdout'] = json.dumps(item)
            path = self.output / f'command-{call:02d}.json'
            save(path, raw)
            return dict(raw, raw_path=str(path))

        def close(self):
            self.closed += 1
            events.append(dict(event='close'))
            if fail_cleanup:
                raise RuntimeError('injected console cleanup failure')

    consoles = []

    def factory(port, output):
        console = ConsoleFixture(port, output)
        consoles.append(console)
        return console

    if fail_preflight:
        args.output = directory / 'forbidden-output'
    error, result = None, None
    stdout = io.StringIO()
    try:
        with contextlib.redirect_stdout(stdout):
            if fail_preflight:
                result = collector.collect(args, _console_factory=factory)
            else:
                result = collector.collect(args, _console_factory=factory, _prepared=prepared)
    except BaseException as caught:
        error = type(caught).__name__ + ': ' + str(caught)
    (directory / 'stdout.log').write_text(stdout.getvalue())
    save(directory / 'console-events.json', dict(fixture_only=True,
         evidence_level='synthetic', source_board_cases=0, events=events))

    if fail_preflight:
        check(error is not None and 'task board/experiment run directory' in error,
              name + ': actual preflight rejection')
        check(not consoles and not events and not parsed, name + ': rejected before UART construction')
        check(not args.output.exists(), name + ': rejected before output creation')
        manifest, records = None, []
    else:
        check(len(consoles) == 1 and consoles[0].closed == 1, name + ': one console and one cleanup')
        manifest = json.loads((run / 'manifest.json').read_text())
        records = json.loads((run / 'records.json').read_text())
        n = fail_case - 1 if fail_case is not None else 15
        check(len(records) == manifest['recorded_cases'] == len(parsed) == n,
              name + ': exact successful prefix retained')
        check(manifest['fixture_only'] is True and manifest['source_board_cases'] == 0,
              name + ': synthetic manifest has zero board cases')
        check(all(r['fixture_only'] is True and r['source_board_cases'] == 0 for r in records),
              name + ': synthetic records have zero board cases')
        check([(r['block'], r['condition']) for r in records] ==
              [tuple(p) for p in collector.gate.plan()[:n]], name + ': fixed prefix order')
        check([r['sequence'] for r in records] == list(range(PRE_SEQ + 1, PRE_SEQ + n + 1)),
              name + ': bracket watermark and sequential records')
        check(manifest['automatic_retries'] == 0 and manifest['control_actions'] == [],
              name + ': no retries or configuration actions')
        count = fail_case + 1 if fail_case is not None else 17
        check(consoles[0].commands == expected[:count], name + ': no command after first failure')
        check('pre_readback' in manifest, name + ': saved independent pre-read')
        if fail_case is not None:
            check(error is not None and 'CLI wrapper failed' in error,
                  name + ': original failure propagated')
            check(manifest['status'] == 'failed' and 'post_readback' not in manifest,
                  name + ': failed prefix never reported complete')
        else:
            check('post_readback' in manifest and
                  manifest['post_readback'] != manifest['pre_readback'], name + ': saved distinct post-read')
            if fail_cleanup:
                check(error is not None and 'cleanup failure' in error, name + ': cleanup propagated')
                check(manifest['status'] == 'failed' and 'cleanup_error' in manifest,
                      name + ': cleanup failure invalidates complete collection')
            else:
                check(error is None and result['status'] == manifest['status'] == 'passed',
                      name + ': all15 plus read brackets passed')
    summary = dict(name=name, status='passed', checks=CHECKS - start_checks,
        fixture_only=True, evidence_level='synthetic', source_board_cases=0,
        successful_synthetic_records=len(records), command_calls=sum(e['event'] == 'command' for e in events),
        exception=error, actual_collection_status=None if manifest is None else manifest['status'])
    save(directory / 'check-result.json', summary)
    return summary


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--dependency-root', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    if out.exists():
        raise ValueError('new output required; preserve earlier executions')
    out.mkdir(parents=True)
    candidate = HERE.parents[1] / 'tools/collect_s15.py'
    dep = args.dependency_root.resolve()
    originals = [(candidate, REL),
        (dep / REL.parent / 's15_gate.py', REL.parent / 's15_gate.py'),
        (dep / 'paper/ispass2027/board/uart.py', Path('paper/ispass2027/board/uart.py'))]
    sources = []
    for path, relative in originals:
        raw = path.read_bytes()
        target = out / 'source' / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(raw)
        sources.append(dict(path=str(path), sha256=digest(raw), snapshot=str(target)))
    (out / 'source/check_s15_collector.py').write_bytes(Path(__file__).read_bytes())
    collector = load_module(out / 'source' / REL)
    cases = [run_scenario(collector, out, 'success15'),
             run_scenario(collector, out, 'first-failure-prefix', fail_case=4),
             run_scenario(collector, out, 'cleanup-failure', fail_cleanup=True),
             run_scenario(collector, out, 'preflight-before-uart', fail_preflight=True)]
    check(all(digest(Path(s['path']).read_bytes()) == s['sha256'] for s in sources),
          'all original candidate/dependency bytes unchanged')
    check(candidate.read_bytes() == (dep / REL).read_bytes(), 'Root original copied exactly')
    report = dict(status='passed', fixture_only=True, evidence_level='synthetic', source_board_cases=0,
        checks=CHECKS, scenarios=cases, sources=sources, execution_argv=sys.argv,
        scope='Only collector scheduling with private injections; real accepted readback helper; parser is a stub.',
        exclusions=['No UART/devices', 'No S15 board cases', 'No repeated gate51 matrix',
                    'No production preflight qualification; one early refusal only'])
    save(out / 'summary.json', report)
    print(json.dumps(dict(status='passed', scenarios=len(cases), checks=CHECKS, source_board_cases=0)))


if __name__ == '__main__':
    main()
