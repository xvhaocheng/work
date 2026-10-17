#!/usr/bin/env python3
"""Render Root-only UART command text or validate saved CLI output; never execute it."""
import argparse
import datetime as dt
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
KERNEL = '6.5.0-rc2-ispass2027-e2-timer-probe-v1'
BIT_SHA = 'e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6'
BUILD_MANIFEST_SHA = 'b12177ae47ee526b1bea22a08546f78c09f7bf6ae689420c94341285c4228470'
COLLECTOR_SHA = '60735c5be244153ae2dcb98aceba0d996a9d1b6d5ef86f832d8634b3006a4607'
PROFILES_SHA = 'b79c51592be5c16fe313e56f1f7f201e4df19ae9f8125c5e5f5edc5086758479'
CONTROL = '/sys/kernel/debug/riscv_timer_probe/control'
SNAPSHOT = '/sys/kernel/debug/riscv_timer_probe/snapshot'
CASES = ('N2_A1', 'N4_A8')
PROC_FILES = ('/proc/interrupts', '/proc/softirqs', '/proc/timer_list',
              '/proc/stat', '/proc/uptime', '/proc/sys/kernel/watchdog',
              '/proc/sys/kernel/watchdog_thresh',
              '/sys/devices/system/clocksource/clocksource0/current_clocksource')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def original_collector():
    require(sha(HERE/'source/collect_multi.as-reviewed.py') == COLLECTOR_SHA, 'original collector changed')
    require(sha(HERE/'source/e2_software_profiles.py') == PROFILES_SHA, 'original profile helper changed')
    sys.path.insert(0, str(HERE/'source'))
    spec = importlib.util.spec_from_file_location('e2_original_parse_command', HERE/'source/collect_multi.as-reviewed.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def identity_guard(boot, taint_zero=True):
    require(re.fullmatch(r'[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}', boot), 'invalid boot ID')
    guard = (f'[ "$(uname -r)" = "{KERNEL}" ] || exit 61; '
             f'[ "$(cat /proc/sys/kernel/random/boot_id)" = "{boot}" ] || exit 62; '
             '[ "$(cat /sys/devices/system/cpu/online)" = "0" ] || exit 63; ')
    if taint_zero:
        guard += '[ "$(cat /proc/sys/kernel/tainted)" = "0" ] || exit 64; '
    return guard


def frozen_snapshot(label):
    return (f'[ "$(cat {CONTROL})" = "0" ] || exit 65; '
            f'printf "PROBE_{label}_BEGIN\\n"; cat {SNAPSHOT} || exit 66; '
            f'printf "PROBE_{label}_END\\n"; ')


def proc_evidence(label):
    return ''.join(f'printf "PROC_{label}_BEGIN {name}\\n"; cat {name} || exit 67; '
                   f'printf "PROC_{label}_END {name}\\n"; ' for name in PROC_FILES)


def make_plan(boot, load_manifest=None, review_template=False):
    original = original_collector()
    preflight = identity_guard(boot)
    preflight += (f'if [ ! -r {CONTROL} ]; then mount -t debugfs none /sys/kernel/debug || exit 68; fi; '
                  f'printf "DIAG_KERNEL="; uname -r; printf "DIAG_BOOT="; cat /proc/sys/kernel/random/boot_id; '
                  'printf "DIAG_TAINT="; cat /proc/sys/kernel/tainted; '
                  'printf "DIAG_CPU_ONLINE="; cat /sys/devices/system/cpu/online; '
                  f'printf "DIAG_CONTROL="; cat {CONTROL} || exit 69; ')
    preflight += frozen_snapshot('INITIAL') + proc_evidence('INITIAL') + 'true'
    steps = [{'name': 'preflight', 'host_timeout_seconds': 30, 'command': preflight,
              'gate_before_next': 'Root requires new boot/taint0/CPU0/control0 and unused initial snapshot; no DMA yet.'}]
    for condition in CASES:
        nr, pages = original.CONDITION_GEOMETRY[condition]
        cli = f'platform-timeout 120 idma_scaling --devices {nr} --aggressor-pages {pages} --timeout-ms 60000'
        baseline = original.command(condition)
        anchor = cli + '; case_rc=$?; '
        require(baseline.count(anchor) == 1, 'original CLI command boundary changed')
        # Keep all original identity/taint/observer/CLI/dmesg text and the CLI budget.
        # Freeze immediately on CLI return, before the original CASE_EXIT/dmesg output.
        replacement = (f'printf "1\\n" > {CONTROL} || exit 70; ' + anchor +
                       f'printf "0\\n" > {CONTROL} || exit 71; '
                       f'[ "$(cat {CONTROL})" = "0" ] || exit 72; ')
        command = (identity_guard(boot) + frozen_snapshot('BEFORE') + proc_evidence('BEFORE') +
                   baseline.replace(anchor, replacement))
        steps.append({'name': condition+'_window', 'condition': condition, 'host_timeout_seconds': 180,
                      'original_cli': cli, 'original_command': baseline, 'command': command,
                      'gate_before_next': 'Any timeout/nonzero/incomplete/parse or taint/STOP/drain/data error stops. Root validates saved original CLI result before normal post-read.'})
        post = identity_guard(boot) + frozen_snapshot('AFTER') + proc_evidence('AFTER') + 'true'
        steps.append({'name': condition+'_post', 'condition': condition, 'host_timeout_seconds': 30,
                      'command': post,
                      'gate_before_next': 'Root inspects frozen counters/proc and accepts the low-pressure control before dispatching N4_A8; never auto-continue.'})
    recovery_freeze = (identity_guard(boot, taint_zero=False) +
                       f'printf "0\\n" > {CONTROL} || exit 71; '
                       f'printf "RECOVERY_CONTROL="; cat {CONTROL} || exit 72; '
                       'printf "RECOVERY_TAINT="; cat /proc/sys/kernel/tainted')
    recovery_read = (identity_guard(boot, taint_zero=False) + frozen_snapshot('RECOVERY') +
                     proc_evidence('RECOVERY') + 'true')
    return {'schema': 'e2-timer-probe-root-only-plan-v1', 'review_template': review_template,
            'diagnostic_only': True, 'formal_performance_data': False, 'executor': 'Root only; this file has no UART executor',
            'load_manifest': load_manifest, 'boot_id': boot, 'kernel_release': KERNEL,
            'bit_sha256': BIT_SHA, 'build_manifest_sha256': BUILD_MANIFEST_SHA,
            'cases_once_in_order': list(CASES), 'automatic_continuation': False,
            'automatic_retry_or_ctrl_c_or_reset': False, 'steps': steps,
            'recovery_proposals_not_automatic': [
                {'name': 'freeze_once', 'host_timeout_seconds': 10, 'command': recovery_freeze,
                 'condition': 'Only Root may decide to send after preserving timeout bytes and independently regaining a prompt. Freezing the probe does not stop or prove quiet DMA.'},
                {'name': 'read_frozen_once', 'host_timeout_seconds': 30, 'command': recovery_read,
                 'condition': 'Only after Root separately decides a bounded read is possible and worth the disturbance. A returned prompt/CTRL-C/frozen probe is not proof of DMA quiet.'}],
            'planned_host_command_budget_seconds': 450,
            'budget_limit': 'Excludes load/update/manual review/recovery. Host timeout never bounds or stops remote DMA lifetime.'}


def validate_cli(plan, raw, condition, previous=None):
    require(not plan['review_template'], 'review template cannot validate real trial data')
    require(condition in CASES, 'only the two fixed cases')
    require(raw.get('exit_code') == 0, 'host timeout/error: stop, retain raw, no follow-up')
    expected = next(row['command'] for row in plan['steps'] if row['name'] == condition+'_window')
    require(raw.get('command') == expected, 'actual target command differs from reviewed plan')
    if condition == 'N4_A8':
        require(previous is not None and previous['status'] == 'original_cli_gates_passed' and
                previous['condition'] == 'N2_A1' and previous['boot_id'] == plan['boot_id'],
                'N4 requires previously accepted N2 CLI evidence; Root also reviews N2 post snapshot')
    parsed = original_collector().parse(raw['stdout'], KERNEL, condition)
    require(parsed['boot_id'] == plan['boot_id'], 'boot changed')
    if condition == 'N4_A8':
        require(parsed['sequence'] == previous['observer_sequence']+1, 'two-point observer sequence gap')
    return {'schema': 'e2-timer-probe-cli-validation-v1', 'status': 'original_cli_gates_passed',
            'condition': condition, 'boot_id': parsed['boot_id'], 'observer_sequence': parsed['sequence'],
            'diagnostic_only': True, 'formal_performance_data': False, 'record': parsed['record'],
            'observer': parsed['observer'], 'path': parsed['path'],
            'not_validated_here': 'Probe snapshot semantics, runtime errors/per-open lifetime/cost, quiet after timeout, root cause. Root separately reviews frozen post evidence before any next command.'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    actions = parser.add_subparsers(dest='action', required=True)
    render = actions.add_parser('render')
    render.add_argument('--load-manifest', type=Path, required=True)
    render.add_argument('--build-manifest', type=Path, default=HERE.parent/'builder-v1/final-build-manifest.json')
    render.add_argument('--out', type=Path, required=True)
    validate = actions.add_parser('validate-cli')
    validate.add_argument('--plan', type=Path, required=True)
    validate.add_argument('--raw', type=Path, required=True)
    validate.add_argument('--condition', choices=CASES, required=True)
    validate.add_argument('--previous', type=Path)
    validate.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if args.action == 'render':
        require(sha(args.build_manifest) == BUILD_MANIFEST_SHA, 'diagnostic build manifest changed')
        build = json.loads(args.build_manifest.read_text())
        loaded = json.loads(args.load_manifest.read_text())
        require(loaded['status'] == 'boot_identity_verified', 'verified new load receipt required')
        require(loaded['kernel_release'] == KERNEL and loaded['bit_sha256'] == BIT_SHA, 'wrong diagnostic kernel or original DS0 BIT')
        require(loaded['boot_id'] != '5df16afb-ce70-4098-a71f-4ff462079c1e', 'old failed E2 boot')
        require(dt.datetime.fromisoformat(loaded['finished_utc']) >= dt.datetime.fromisoformat(build['end']),
                'load receipt predates diagnostic build')
        result = make_plan(loaded['boot_id'], str(args.load_manifest.resolve()))
        result['load_manifest_sha256'] = sha(args.load_manifest)
        result['script_sha256'] = sha(__file__)
    else:
        plan = json.loads(args.plan.read_text())
        previous = json.loads(args.previous.read_text()) if args.previous else None
        result = validate_cli(plan, json.loads(args.raw.read_text()), args.condition, previous)
        result.update(raw_path=str(args.raw.resolve()), raw_sha256=sha(args.raw), plan_sha256=sha(args.plan))
    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open('x') as output:
        output.write(json.dumps(result, indent=2)+'\n')
    print(json.dumps({'status': 'host_only', 'written': str(args.out), 'board_operations': 0}))


if __name__ == '__main__':
    main()
