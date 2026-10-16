#!/usr/bin/env python3
"""Exactly one finite RUN after actual approval, postboot identity and CAPS.

--describe is offline. Runtime calls are deliberately refused until the real
user approval receipt and actual chosen-kernel boot/CAPS exist. A timeout does
not retire the attempt guard, and must never cause another RUN or reset.
"""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import subprocess
import sys
import time

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parent.parent
LIVE = ROOT / 'live'
RUN_COMMAND = (
    '( /tmp/r107-stage111/idma-finite --run --device /dev/idma0 '
    '--data-dir /tmp/r107-stage111/data --output-dir /tmp/r107-stage111/finite-result; '
    'task_pilot_rc=$?; printf "R111_FINITE_CLI_RETURN=%s\\n" "$task_pilot_rc"; '
    'exit "$task_pilot_rc" )'
)


def load(name):
    return json.loads((LIVE / name).read_text())


def require(ok, reason):
    if not ok:
        raise RuntimeError(reason)


def preflight():
    a = load('new-SD-user-approval.json')
    proposal = json.loads((ROOT / 'deployment-proposal.json').read_text())
    require(a.get('approved') is True and a.get('source') == 'persistent_user_task_authorization',
            'real explicit new SD approval is missing')
    require(a.get('target_device') == '/dev/mmcblk0p1' and
            a.get('payload_sha256') == proposal['payload_sha256'],
            'approval does not cover this concrete selected payload')
    identity = load('new-kernel-identity-acceptance.json')
    require(identity.get('accepted') is True and
            identity.get('kernel_release') == proposal['new_release'],
            'actual chosen kernel identity has not passed')
    # CAPS must be acquired with the lightweight current boot probe immediately
    # before this action, not inferred from an old accepted file or CLI inputs.
    c = load('pre-pilot-caps-and-boot.json')
    require(c.get('actual_UART_command_succeeded') is True and
            c.get('boot_ID') == identity.get('boot_ID') and
            c.get('kernel_release') == proposal['new_release'], 'stale or missing pre-pilot live boot')
    age = time.monotonic_ns() - c.get('dell_monotonic_end_ns', 0)
    require(0 <= age <= 30_000_000_000, 'pre-pilot actual boot/CAPS probe is not fresh')
    caps = c['caps_JSON']
    require(caps['ioctl']['return'] == 0 and caps['ioctl']['errno'] == 0 and
            caps['ioctl']['attempted'] is True, 'CAPS failed')
    v = caps['caps']
    for key, value in {'abi_version': 1, 'struct_size': 104,
                       'magic': 0x49444d4146494e01,
                       'observer_magic': 0x49444d414f425309,
                       'composition_magic': 0x49444d4143504e01,
                       'composition_abi': 1, 'composition_config': 0xa00,
                       'composition_idle': 1, 'batch_magic': 0x49444d4142544301,
                       'device_id': 0, 'did': 10, 'quarantined': 0, 'online_cpus': 1}.items():
        require(v.get(key) == value, 'actual CAPS mismatch: ' + key)
    return identity, c


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--describe', action='store_true')
    a = p.parse_args()
    if a.describe:
        print(json.dumps({'kind': 'offline command and gates only', 'command': RUN_COMMAND,
                          'actual_device_commands': 0, 'host_observation_limit_seconds': 1800,
                          'fault_guard_is_not_application_deadline': True,
                          'retry_RUN_after_timeout': False}, indent=2))
        return 0
    # No device action or mutation before all real prerequisites are present.
    identity, caps = preflight()
    guard = LIVE / 'finite-pilot-execution-state.json'
    with guard.open('x') as f:
        json.dump({'state': 'dispatch_started_completion_not_yet_known',
                   'started_at_utc': datetime.now(timezone.utc).isoformat(),
                   'boot_ID': identity['boot_ID'], 'command': RUN_COMMAND,
                   'maximum_RUN_calls_by_this_dispatch': 1, 'automatic_retry': False}, f, indent=2)
        f.write('\n')
    command_path = LIVE / 'finite-pilot-once.command'
    command_path.write_text(RUN_COMMAND + '\n')
    out = LIVE / 'finite-pilot-host-wrapper.stdout.log'
    err = LIVE / 'finite-pilot-host-wrapper.stderr.log'
    with out.open('xb') as stdout, err.open('xb') as stderr:
        completed = subprocess.run([sys.executable, '-B', str(LIVE / 'board_query.py'),
                                    '--command-file', str(command_path),
                                    '--output-prefix', 'finite-pilot-once', '--timeout', '1800'],
                                   stdout=stdout, stderr=stderr)
    r = load('finite-pilot-once-identity-result.json')
    state = json.loads(guard.read_text())
    state.update({'wrapper_process_exit_code': completed.returncode,
                  'wrapper_finished_at_utc': datetime.now(timezone.utc).isoformat(),
                  'framed_command_exit_code': r.get('board_command_exit_code'),
                  'state': 'command_return_observed' if 'board_command_exit_code' in r
                  else 'observation_failed_board_command_not_proven_terminal',
                  'semantic_success': 'unverified_until_original_UAPI_and_all_bytes_validated'})
    guard.write_text(json.dumps(state, indent=2) + '\n')
    print(json.dumps(state, indent=2))
    return completed.returncode


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (RuntimeError, OSError, KeyError, ValueError) as exc:
        print('finite pilot refused: ' + str(exc), file=sys.stderr)
        raise SystemExit(2)
