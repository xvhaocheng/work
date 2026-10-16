#!/usr/bin/env python3
"""Recover the first already-executed case, then execute only missing cases."""
import json
import shlex
import shutil
import sys
from pathlib import Path
from run_research2_mac import module as m, LoggedInRunner

class PackingConsole(m.SerialConsole):
    def run_shell(self, command, timeout, cwd=None, **kwargs):
        if command.startswith('tar -czf '):
            parts = shlex.split(command)
            if len(parts) != 6 or parts[3] != '-C' or parts[5] != '.':
                raise m.TechnicalFailure('unexpected archive command')
            target, source = parts[2], parts[4]
            plain = target + '.plain.tar'
            command = ('test ! -e ' + shlex.quote(plain) + ' && test ! -e ' + shlex.quote(target)
                       + ' && tar -cf ' + shlex.quote(plain) + ' -C ' + shlex.quote(source)
                       + ' . && gzip -c ' + shlex.quote(plain) + ' > ' + shlex.quote(target))
            self._log_event('BusyBox archive command: ' + command)
        return super().run_shell(command, timeout, cwd=cwd, **kwargs)

original = Path('results/experiments/20261003-research-two-bit')
out = Path('results/experiments/20261003-research-two-bit-continued')
package = Path('artifacts/software/research-two-bit-20261003/delivery')
args = m.build_arg_parser().parse_args([
    '--manifest', str(package/'DELIVERY.json'), '--package-root', str(package),
    '--out', str(out), '--serial', '/dev/cu.usbserial-AV0KRCHT', '--stage', 'both',
    '--startup-timeout', '900', '--jtag-freq', '1000000'])
manifest = m.load_manifest(Path(args.manifest), None)
m.verify_manifest_package_files(manifest, package)
oldrun = json.loads((original/'run.json').read_text())
oldboot = json.loads((original/'e2/boot.json').read_text())
plan = json.loads((original/'e2/plan.json').read_text())
pending = plan['cases'][0]
cid = pending['id']
oldmeta = json.loads((original/'e2/runs'/cid/'metadata.json').read_text())
if oldmeta['exit'] != 0 or oldmeta['id'] != 'e2-short-w0-forward-n0':
    raise m.TechnicalFailure('first case execution evidence is not the expected successful CLI')
if 'tar exit=1' not in oldmeta.get('transfer_error', ''):
    raise m.TechnicalFailure('unexpected failure; refuse automatic recovery')
if out.exists():
    raise m.TechnicalFailure('continuation output already exists')
shutil.copytree(original, out)
(out/'STOPPED.txt').rename(out/'STOPPED.initial-transfer.txt')
console = PackingConsole(args.serial, out/'console.raw.log', out/'console.events.log')
runner = LoggedInRunner(args, manifest, package, out, console,
                        m.OpenFPGALoaderProgrammer(freq=1000000),
                        m._load_module('research2_plan'), m._load_module('research2_results'),
                        manifest_sha256=m.sha256_file(Path(args.manifest)))
runner.run_id = oldrun['run_id']
runner.board_root = oldrun['board_root']
runner.current_stage = 'e2'
runner.stage_dir = out/'e2'
runner.flashed_roles = [1]
runner.boot = oldboot
runner.boots = {1: oldboot}
try:
    console.start()
    if console.run_shell('true', timeout=30).exit_code != 0:
        raise m.TechnicalFailure('existing shell unavailable')
    current = runner._collect_boot_info(1)
    if current['boot_id'] != oldboot['boot_id']:
        raise m.TechnicalFailure('board rebooted; retained first-case evidence unavailable')
    if plan != runner.plan_module.build_plan('e2'):
        raise m.TechnicalFailure('frozen E2 plan changed')
    runner._write_json('recovery.json', {'started_at': m.utc_now(), 'source': str(original),
                       'pending_case': cid, 'first_case_reexecuted': False,
                       'change': 'tar -cf followed by gzip -c; preserve validators and experiment argv'})
    run_dir = runner.stage_dir/'runs'/cid
    (run_dir/'metadata.json').rename(run_dir/'metadata.initial-transfer.json')
    runner.current_case = cid
    transfer = runner._collect_and_transfer(cid, runner.board_root+'/'+cid, run_dir)
    if transfer['exit_code'] != oldmeta['exit'] or transfer['taint_before'].strip() != '0' or transfer['taint_after'].strip() != '0':
        raise m.TechnicalFailure('retained first case exit/taint failed')
    record = json.loads((run_dir/'record.json').read_text())
    errors = runner.results_module.validate_record(pending, record)
    if errors:
        raise m.TechnicalFailure('retained first case validation: '+repr(errors))
    meta = dict(oldmeta)
    meta.pop('transfer_error')
    meta.update(transfer, framed_exit=oldmeta['exit'], finished_at=m.utc_now(),
                recovery='original case collected without executing CLI again')
    runner._write_json('e2/runs/'+cid+'/metadata.json', meta)
    runner._write_json('e2/runs/'+cid+'/metrics.json', runner.results_module.metrics(record))
    (run_dir/'SAVED').write_text(m.utc_now()+'\n')
    runner.completed_cases.append(('e2', cid))
    runner._log('case '+cid+' recovered and saved; CLI not reexecuted')
    for case in plan['cases'][1:]:
        runner._run_one_case('e2', 1, runner._resolve_case(case, None))
    runner._verify_stage_saved('e2', plan['cases'])
    runner._write_json('e2/stage-done.json', {'schema': 'research2-stage-done-v1',
                       'stage': 'e2', 'role': 1, 'cases': len(plan['cases']),
                       'finished_at': m.utc_now(), 'selection_frozen': False})
    runner._run_stage('method', 2)
    runner._write_json('COMPLETE.json', {'schema': 'research2-complete-v1',
                       'finished_at': m.utc_now(), 'stages': ['e2', 'method'],
                       'completed_cases': [{'stage': s, 'id': c} for s,c in runner.completed_cases],
                       'boots': {str(k): v['boot_id'] for k,v in runner.boots.items()},
                       'recovery_source': str(original)})
except BaseException as exc:
    runner._stop(type(exc).__name__+': '+str(exc))
    raise
finally:
    console.close()
