#!/usr/bin/env python3
'''
Independent, single-child recovery launcher for the interrupted
out/paper-route-review-20261004 FPGA build.

  * starts exactly ONE foreground Vivado child:
        vivado -mode batch -notrace -source <recovery-v1/recover-route.tcl>
  * inherits the caller environment, so the caller must have sourced
    Vivado's settings64.sh (or pass --vivado /abs/path/to/vivado)
  * copies the old build state into recovery-v1 as
    interrupted-build-state-evidence.json (annotated interrupted, reason unknown)
    WITHOUT modifying the original file
  * writes recovery-v1/recovery-state.json as running -> complete/failed
  * 'complete' requires: vivado rc == 0 AND the Tcl gates passed AND
    routed dcp + .bit + .mcs all exist and are non-empty

This script never daemonises, never re-forks and never launches a second Vivado.
'''

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path('/home/xvhaocheng/work/iommu-platform')
OUT = ROOT / 'out' / 'paper-route-review-20261004'
REC = OUT / 'recovery-v1'
LOGS = REC / 'logs'
REPORTS = REC / 'reports'

ORIG_STATE = OUT / 'fpga-build-state.json'
INTERRUPTED_EVIDENCE = REC / 'interrupted-build-state-evidence.json'
STATE_FILE = REC / 'recovery-state.json'
TCL_RESULT = REC / 'recovery-tcl-result.json'
TCL_LOG = REC / 'recovery-tcl.log'
VIVADO_LOG = LOGS / 'vivado-recovery.log'

ROUTED_DCP = REC / 'ariane_xilinx_routed.dcp'
BIT = REC / 'ariane_xilinx.bit'
MCS = REC / 'ariane_xilinx.mcs'

DEFAULT_TCL = REC / 'recover-route.tcl'

DCP_CANDIDATES = [
    OUT / 'fpga-src' / 'cva6' / 'corev_apu' / 'fpga' / 'ariane.runs' / 'impl_1' / 'ariane_xilinx_physopt.dcp',
    ROOT / 'out' / 'migration-4dma-composition' / 'fpga-src' / 'cva6' / 'corev_apu' / 'fpga' / 'ariane.runs' / 'impl_1' / 'ariane_xilinx_physopt.dcp',
]

OBSERVED_FACTS = [
    'fpga-build-state.json still says status=running; the crashed build never wrote exit_code/finished_at',
    'no python/Vivado build processes were alive when recovery started',
    'no ariane_xilinx.bit was produced',
    'no successfully routed DCP and no ariane_xilinx_routed_error.dcp were produced',
    'last impl_1 log activity ~20:43; the log stops at globalroute2 with overlaps=41',
    'ariane_xilinx_physopt.dcp (post-place phys_opt, ~18:11) exists and is the newest usable checkpoint',
    'the termination cause (OOM kill / host reboot / manual kill / Vivado crash) is not recorded anywhere -> unknown',
]


def utcnow() -> str:
    return datetime.now(timezone.utc).isoformat()


def write_json(path: Path, obj) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(path.name + '.tmp')
    tmp.write_text(json.dumps(obj, indent=2, sort_keys=False) + chr(10))
    os.replace(tmp, path)


def load_json(path: Path):
    try:
        return json.loads(path.read_text())
    except Exception:
        return None


def file_info(path: Path) -> dict:
    try:
        st = path.stat()
    except OSError:
        return {'path': str(path), 'exists': False, 'size': 0, 'mtime': None}
    return {
        'path': str(path),
        'exists': True,
        'size': st.st_size,
        'mtime': datetime.fromtimestamp(st.st_mtime, timezone.utc).isoformat(),
    }


def tail(path: Path, n: int = 40) -> str:
    try:
        lines = path.read_text(errors='replace').splitlines()
    except OSError:
        return ''
    return chr(10).join(lines[-n:])


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--vivado', default=os.environ.get('VIVADO_BIN', 'vivado'),
                    help='vivado executable (default: from the inherited PATH)')
    ap.add_argument('--tcl', default=str(DEFAULT_TCL), help='recovery Tcl script')
    ap.add_argument('--physopt-dcp', default=None, help='explicit ariane_xilinx_physopt.dcp')
    ap.add_argument('--timeout', type=float, default=0.0,
                    help='seconds before killing Vivado (0 = no timeout)')
    args = ap.parse_args(argv)

    for d in (REC, LOGS, REPORTS):
        d.mkdir(parents=True, exist_ok=True)

    state = {
        'task': 'route-recovery-from-physopt-checkpoint',
        'profile': 'migration-4dma-composition',
        'status': 'running',
        'phase': 'preflight',
        'started_at': utcnow(),
        'launcher_pid': os.getpid(),
        'out_dir': str(REC),
        'recovery_log': str(VIVADO_LOG),
        'note': 'independent recovery run; the original fpga-build-state.json is left untouched '
                'and copied to interrupted-build-state-evidence.json',
    }
    write_json(STATE_FILE, state)

    evidence = {
        'evidence_kind': 'interrupted-build-state-copy',
        'captured_at': utcnow(),
        'source_file': str(ORIG_STATE),
        'annotated_status': 'interrupted',
        'interrupted_reason': 'unknown',
        'original_state': load_json(ORIG_STATE),
        'observed_facts': OBSERVED_FACTS,
        'interpretation': 'The original build is INTERRUPTED with an UNKNOWN cause. This file is a copy; '
                          'the original state file was not modified and may still read running, which must '
                          'never be interpreted as success.',
    }
    write_json(INTERRUPTED_EVIDENCE, evidence)

    tcl = Path(args.tcl)
    dcp = Path(args.physopt_dcp) if args.physopt_dcp else None
    if dcp is None:
        for cand in DCP_CANDIDATES:
            if cand.exists() and cand.stat().st_size > 0:
                dcp = cand
                break

    vivado = args.vivado
    if os.path.isabs(vivado):
        vivado_path = vivado if os.path.exists(vivado) else None
    else:
        vivado_path = shutil.which(vivado)

    preflight = []
    if not tcl.exists():
        preflight.append('Tcl script not found: ' + str(tcl))
    if dcp is None:
        preflight.append('physopt checkpoint not found; candidates: ' +
                         ', '.join(str(c) for c in DCP_CANDIDATES))
    if vivado_path is None:
        preflight.append('vivado executable not found (' + str(vivado) +
                         '); source <Vivado>/settings64.sh first or pass --vivado')

    if preflight:
        state.update({
            'status': 'failed',
            'phase': 'preflight',
            'finished_at': utcnow(),
            'exit_code': None,
            'physopt_dcp': str(dcp) if dcp else None,
            'failure_reasons': preflight,
        })
        write_json(STATE_FILE, state)
        print('PREFLIGHT FAILED: ' + '; '.join(preflight), file=sys.stderr)
        return 1

    env = os.environ.copy()
    env['RECOVERY_PHYSOPT_DCP'] = str(dcp)

    cmd = [str(vivado_path), '-mode', 'batch', '-notrace', '-log', str(LOGS / 'vivado.log'), '-journal', str(LOGS / 'vivado.jou'), '-source', str(tcl)]
    state.update({
        'phase': 'routing',
        'command': cmd,
        'physopt_dcp': str(dcp),
        'child_started_at': utcnow(),
        'timeout_s': args.timeout,
    })
    write_json(STATE_FILE, state)
    print('LAUNCH: ' + ' '.join(cmd))
    print('log: ' + str(VIVADO_LOG))

    rc = None
    timed_out = False
    with open(VIVADO_LOG, 'wb') as logf:
        logf.write(('# ' + ' '.join(cmd) + chr(10)).encode())
        logf.write(('# cwd=' + str(ROOT) + chr(10)).encode())
        logf.write(b'# environment inherited from the calling shell\n')
        logf.flush()
        try:
            cp = subprocess.run(cmd, cwd=str(ROOT), env=env, stdout=logf,
                                stderr=subprocess.STDOUT,
                                timeout=(args.timeout if args.timeout and args.timeout > 0 else None))
            rc = cp.returncode
        except subprocess.TimeoutExpired:
            timed_out = True
            rc = -1
            logf.write((chr(10) + '# TIMEOUT after ' + str(args.timeout) + 's: Vivado was killed' + chr(10)).encode())
        except OSError as exc:
            rc = -2
            logf.write((chr(10) + '# OSError launching vivado: ' + str(exc) + chr(10)).encode())

    tcl_result = load_json(TCL_RESULT)
    gates_passed = False
    if isinstance(tcl_result, dict):
        try:
            gates_passed = int(tcl_result.get('gates_passed', 0)) == 1
        except (TypeError, ValueError):
            gates_passed = False

    artifacts = {
        'routed_dcp': file_info(ROUTED_DCP),
        'bitstream': file_info(BIT),
        'mcs': file_info(MCS),
        'tcl_result': file_info(TCL_RESULT),
        'tcl_log': file_info(TCL_LOG),
        'vivado_log': file_info(VIVADO_LOG),
    }

    reasons = []
    if timed_out:
        reasons.append('timeout after ' + str(args.timeout) + 's')
    if rc != 0:
        reasons.append('vivado exit code ' + str(rc))
    if not gates_passed:
        reasons.append('Tcl gates not passed (timing/route/DRC)')
    for key in ('routed_dcp', 'bitstream', 'mcs'):
        info = artifacts[key]
        if not info['exists'] or info['size'] < 1:
            reasons.append('artifact missing or empty: ' + str(info['path']))

    status = 'complete' if not reasons else 'failed'
    state.update({
        'status': status,
        'phase': 'done' if status == 'complete' else 'stopped',
        'finished_at': utcnow(),
        'exit_code': rc,
        'timed_out': timed_out,
        'gates_passed': 1 if gates_passed else 0,
        'gate_failures': tcl_result.get('failures') if isinstance(tcl_result, dict) else None,
        'tcl_result': tcl_result,
        'artifacts': artifacts,
        'failure_reasons': reasons,
        'success_definition': 'complete == vivado rc 0 AND tcl gates_passed 1 AND '
                              'routed dcp + .bit + .mcs all exist and are non-empty',
    })
    write_json(STATE_FILE, state)

    if status != 'complete':
        print(tail(VIVADO_LOG, 40))
    print('---- recovery status: ' + status + ' (rc=' + str(rc) + ', gates_passed=' + str(gates_passed) + ') ----')
    if reasons:
        print('failure reasons: ' + '; '.join(reasons), file=sys.stderr)
    return 0 if status == 'complete' else 1


if __name__ == '__main__':
    sys.exit(main())
