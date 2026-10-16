#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
'''Independent Verilator runner for the rv_iommu_axi_bc regression test.

Layout
------
The repository root is always <script>/../.. (sim/axi_bc/run.py -> <root>).
There is no way to move it: the testbench sim/axi_bc/axi_bc_tb.sv, the C++
driver sim/axi_bc/axi_bc_test.cpp and the vendor/ packages are always read
from the repository root.

--source is NOT a repository root.  It names a single DUT .sv file that is
compiled instead of the default rtl/transaction_controller/rv_iommu_axi_bc.sv,
so an archived copy of an older revision can be used to reproduce before
behaviour without touching the working tree.

Build model
-----------
The build is verilator --cc --exe --build -Wall -Wno-fatal (classic object
flow; no --binary, no --timing).  -Wall keeps every warning visible, WIDTH
included, and -Wno-fatal lets the build finish, so nothing is silenced into a
green run.  --build-jobs is deliberately not passed because Verilator 4.038 is
the supported floor.  Elaboration order matters: cf_math_pkg.sv must be given
before axi_pkg.sv, then the DUT, the testbench and the C++ driver.

The executable is expected exactly at <out>/axi_bc_test.  There is no
executable scanning, no build-directory cleanup and no automatic removal of
anything on disk.

Outputs
-------
<out>/build.log holds the Verilator output, <out>/run.log holds the test
executable output, and a short summary is printed.  Without --out the build
directory is a fresh mkdtemp outside the repository, so a plain
python3 sim/axi_bc/run.py never writes into the working tree.

Accounting
----------
The default (no --focus) suite is exactly 816064 cases.  That is the sweep the
driver enumerates today; it is not a claim that every AXI LEN value is
exhausted.  The legacy cross-page case incr-cross (ff0, len3, size3) is
rejected by the pre-fix DUT as well, so on its own it does not pin down the
half-length crossing defect; incr-cross-half (ff0, len2, size3, cross-page,
expected reject) is registered here and in sim/axi_bc/axi_bc_test.cpp.

Usage
-----
  python3 sim/axi_bc/run.py                        # full default suite
  python3 sim/axi_bc/run.py --focus fixed-tail     # one named legacy test
  python3 sim/axi_bc/run.py --out /tmp/ab_build    # explicit build directory
  python3 sim/axi_bc/run.py --source /tmp/before/rv_iommu_axi_bc.sv
'''

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

# Named cases the C++ driver exposes through --focus; keep in sync with
# sim/axi_bc/axi_bc_test.cpp.
FOCUS_TESTS = (
    'fixed-tail',
    'incr-cross',
    'incr-cross-half',
    'incr-byte-tail',
    'incr-unaligned',
)

# Informational only: number of sweep cases the default (no --focus) run
# enumerates.  Not an exhaustiveness statement about AXI LEN.
DEFAULT_SUITE_CASES = 816064

EXE_NAME = 'axi_bc_test'
DUT_DEFAULT = 'rtl/transaction_controller/rv_iommu_axi_bc.sv'

TB_SV = 'sim/axi_bc/axi_bc_tb.sv'
TEST_CPP = 'sim/axi_bc/axi_bc_test.cpp'
CF_MATH_PKG = 'vendor/common_cells/src/cf_math_pkg.sv'
AXI_PKG = 'vendor/axi/src/axi_pkg.sv'

# Everything that still comes from the repository root (the DUT does not).
ROOT_FILES = (TB_SV, TEST_CPP, CF_MATH_PKG, AXI_PKG)

INCLUDE_DIRS = (
    'vendor/common_cells/src',
    'vendor/axi/src',
    'rtl/transaction_controller',
)

# -Wall keeps WIDTH and the other lint warnings visible; -Wno-fatal lets the
# build continue.  No --build-jobs: Verilator 4.038 compatibility.
VERILATOR_ARGS = (
    '--cc',
    '--exe',
    '--build',
    '-sv',
    '--top-module',
    'axi_bc_tb',
    '-o',
    EXE_NAME,
    '-Wall',
    '-Wno-fatal',
)


def repo_root():
    '''<root>/sim/axi_bc/run.py -> <root>.'''
    return Path(__file__).resolve().parents[2]


def resolve_path(value):
    '''Absolute form of *value*, interpreted relative to the cwd.'''
    path = Path(value).expanduser()
    if not path.is_absolute():
        path = Path.cwd() / path
    return path.resolve()


def resolve_verilator(name):
    '''Return an executable path for *name*, or None when it cannot be found.'''
    found = shutil.which(name)
    if found is None and os.path.sep in name and os.access(name, os.X_OK):
        found = name
    return found


def run_logged(cmd, log_path, cwd=None):
    '''Run *cmd* with stdout+stderr captured into *log_path*.

    Returns (returncode, elapsed_seconds).  Raises OSError when *cmd* cannot be
    started at all.
    '''
    start = time.time()
    with open(str(log_path), 'w') as log:
        log.write('$ ' + ' '.join(cmd) + '\n')
        log.flush()
        proc = subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT, cwd=cwd)
    return proc.returncode, time.time() - start


def tail_lines(path, count):
    '''Last *count* non-blank lines of *path* (empty list when unreadable).'''
    try:
        with open(str(path), 'r', errors='replace') as handle:
            lines = [line.rstrip() for line in handle]
    except OSError:
        return []
    return [line for line in lines if line][-count:]


def build_command(root, dut, out_dir, verilator):
    '''Verilator command line for one build.'''
    cmd = [verilator]
    cmd.extend(VERILATOR_ARGS)
    cmd.extend(['--Mdir', str(out_dir)])
    cmd.append('-I' + str(dut.parent))
    for rel in INCLUDE_DIRS:
        cmd.append('-I' + str(root / rel))
    # cf_math_pkg before axi_pkg (axi_pkg imports it), then DUT, TB, driver.
    cmd.append(str(root / CF_MATH_PKG))
    cmd.append(str(root / AXI_PKG))
    cmd.append(str(dut))
    cmd.append(str(root / TB_SV))
    cmd.append(str(root / TEST_CPP))
    return cmd


def main(argv=None):
    parser = argparse.ArgumentParser(
        prog='run.py',
        description='Verilator regression runner for rv_iommu_axi_bc',
    )
    parser.add_argument(
        '--source',
        default=None,
        metavar='DUT.sv',
        help='DUT SystemVerilog file to compile (default: <root>/' + DUT_DEFAULT +
             '); point it at an archived copy to reproduce a before revision',
    )
    parser.add_argument(
        '--out',
        default=None,
        metavar='DIR',
        help='build directory (default: a fresh unique temp directory outside '
             'the repository)',
    )
    parser.add_argument(
        '--focus',
        action='append',
        default=[],
        metavar='NAME',
        help='run only this named case; repeatable or comma separated (' +
             ', '.join(FOCUS_TESTS) + ')',
    )
    parser.add_argument(
        '--max-failures',
        type=int,
        default=16,
        help='printed failures (default 16)',
    )
    parser.add_argument(
        '--no-build',
        action='store_true',
        help='reuse an existing <out>/' + EXE_NAME,
    )
    parser.add_argument(
        '--list-tests',
        action='store_true',
        help='print the named cases and exit',
    )
    parser.add_argument(
        '--verilator',
        default=os.environ.get('VERILATOR', 'verilator'),
        help='verilator executable (default: $VERILATOR or verilator)',
    )
    args = parser.parse_args(argv)

    if args.list_tests:
        sys.stdout.write('\n'.join(FOCUS_TESTS) + '\n')
        return 0

    root = repo_root()

    focus = []
    for item in args.focus:
        for name in str(item).split(','):
            name = name.strip()
            if not name:
                continue
            if name not in FOCUS_TESTS:
                sys.stderr.write('run.py: unknown focus test ' + repr(name) +
                                 ' (known: ' + ', '.join(FOCUS_TESTS) + ')\n')
                return 2
            if name not in focus:
                focus.append(name)

    dut = resolve_path(args.source) if args.source else root / DUT_DEFAULT
    if not dut.is_file():
        sys.stderr.write('run.py: DUT source file not found: ' + str(dut) + '\n')
        return 2
    if dut.suffix.lower() != '.sv':
        sys.stderr.write('run.py: --source must be a .sv file: ' + str(dut) + '\n')
        return 2

    missing = [str(root / rel) for rel in ROOT_FILES if not (root / rel).is_file()]
    if missing:
        sys.stderr.write('run.py: missing repository files:\n  ' +
                         '\n  '.join(missing) + '\n')
        return 2

    if args.out:
        out_dir = resolve_path(args.out)
    else:
        out_dir = Path(tempfile.mkdtemp(prefix='axi_bc_build_'))
    out_dir.mkdir(parents=True, exist_ok=True)
    try:
        out_dir.relative_to(root)
    except ValueError:
        pass
    else:
        sys.stderr.write('run.py: warning: build dir is inside the repository: ' +
                         str(out_dir) + '\n')

    build_log = out_dir / 'build.log'
    run_log = out_dir / 'run.log'
    exe = out_dir / EXE_NAME

    build_seconds = None
    if not args.no_build:
        verilator = resolve_verilator(args.verilator)
        if verilator is None:
            sys.stderr.write('run.py: verilator executable not found: ' +
                             args.verilator + '\n')
            return 2
        try:
            ver = subprocess.run(
                [verilator, '--version'],
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                universal_newlines=True,
            )
        except OSError as exc:
            sys.stderr.write('run.py: cannot run verilator: ' + str(exc) + '\n')
            return 2
        if ver.stdout:
            sys.stderr.write('run.py: ' + ver.stdout.strip() + '\n')
        sys.stderr.write('run.py: build dir ' + str(out_dir) + '\n')
        sys.stderr.write('run.py: DUT ' + str(dut) + '\n')

        try:
            rc, build_seconds = run_logged(build_command(root, dut, out_dir, verilator),
                                           build_log)
        except OSError as exc:
            sys.stderr.write('run.py: cannot start verilator: ' + str(exc) + '\n')
            return 2
        if rc != 0:
            sys.stderr.write('run.py: verilator build failed (rc={}); see {}\n'.format(
                rc, build_log))
            for line in tail_lines(build_log, 40):
                sys.stderr.write('  | ' + line + '\n')
            return rc

    if not exe.is_file():
        sys.stderr.write('run.py: expected executable not found: ' + str(exe) + '\n')
        if args.no_build:
            sys.stderr.write('run.py: --no-build needs a previous successful build '
                             'in ' + str(out_dir) + '\n')
        return 2

    run_cmd = [str(exe), '--max-failures', str(args.max_failures)]
    for name in focus:
        run_cmd.extend(['--focus', name])

    try:
        rc, run_seconds = run_logged(run_cmd, run_log)
    except OSError as exc:
        sys.stderr.write('run.py: cannot run ' + str(exe) + ': ' + str(exc) + '\n')
        return 2

    if focus:
        focus_text = ', '.join(focus)
    else:
        focus_text = 'default suite ({} cases)'.format(DEFAULT_SUITE_CASES)

    timing = 'run {:.1f}s'.format(run_seconds)
    if build_seconds is not None:
        timing = 'build {:.1f}s, '.format(build_seconds) + timing

    summary = [
        'run.py: summary',
        '  dut       : ' + str(dut),
        '  exe       : ' + str(exe),
        '  focus     : ' + focus_text,
        '  exit code : ' + str(rc),
        '  elapsed   : ' + timing,
        '  build log : ' + str(build_log),
        '  run log   : ' + str(run_log),
    ]
    last = tail_lines(run_log, 1)
    if last:
        summary.append('  last line : ' + last[0][:160])
    sys.stdout.write('\n'.join(summary) + '\n')

    if rc != 0:
        tail = tail_lines(run_log, 20)
        if tail:
            sys.stderr.write('run.py: last lines of run.log:\n')
            for line in tail:
                sys.stderr.write('  | ' + line + '\n')
    return rc


if __name__ == '__main__':
    sys.exit(main())
