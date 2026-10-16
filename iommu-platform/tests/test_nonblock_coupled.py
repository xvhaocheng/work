#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the read->write coupled banked nonblocking test.

Builds tests/nonblock_coupled_test.cpp against exactly the same RTL source
list and include directories as tests/test_nonblock_banked.py.  No RTL file is
copied or duplicated: the source list is imported from the sibling runner.

RR overlap sweep (all 12 scenarios in one run):
    python3 tests/test_nonblock_coupled.py --overlap --order rr

v2 full-4KiB coupling buffer (same RTL, runtime flag only; see --couple-depth):
    python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512

Legacy control (default coupling depth 3):
    python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr
Legacy control, v2 depth:
    python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr --couple-depth 512

Opt-in V inter-job gap and IOTLB partition policy (elaborated with
PolicyEnable=1, built in a separate obj dir):
    python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
        --fullbuffer-permit --v-gap-cycles 1000 --policy

Long continuous work (4096 jobs per DMA) with an opt-in 16-page aggressor row.
--scenario is required to select an a16 row; without it the default 12 rows run.
Runtime flags only: same RTL, same obj directories:
    python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
        --job-count 4096 --scenario mode2-a16-l31
"""

import argparse
import importlib.util
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT  = ROOT / "out" / "nonblock-coupled"

TOP = "nonblock_banked_top"


def load_sibling():
    path = Path(__file__).resolve().parent / "test_nonblock_banked.py"
    if not path.is_file():
        sys.exit("sibling runner missing: {}".format(path))
    spec = importlib.util.spec_from_file_location("nonblock_banked_runner", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def tail(path: Path, lines: int = 200) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def require(path: Path) -> Path:
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p is None or p in seen:
            continue
        seen.add(p)
        out.append(p)
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("-G", "--legacy", action="store_true",
                    help="build the legacy control harness "
                         "(BankedEnable=0, NonblockEnable=0)")
    ap.add_argument("--overlap", action="store_true",
                    help="enable the two half-burst overlap stimulus mode "
                         "(same RTL, opt-in).  Builds in a SEPARATE obj "
                         "directory so the default PASS build is never "
                         "overwritten.  Forwarded to the C++ harness as "
                         "--overlap.  The RR sweep is "
                         "--overlap --order rr; add --legacy for the "
                         "legacy control run.")
    ap.add_argument("--order", choices=("vv", "aa", "av", "rr"), default="vv",
                    help="upstream request priority order, forwarded to the "
                         "C++ harness as +order=<...> (default: vv).  "
                         "'rr' selects the independent AR/AW round-robin "
                         "stimulus that interleaves the two DMAs' halves on "
                         "the shared upstream port, e.g. V.h0 -> A.h0 -> "
                         "V.h1 -> A.h1; combine with --overlap to cover all "
                         "12 scenarios with that legal AXI ordering.")
    ap.add_argument("--couple-depth", type=int, choices=(3, 512), default=3,
                    help="per-DMA read->write coupling FIFO depth, forwarded to "
                         "the C++ harness as +couple-depth=<...> (default: 3). "
                         "3 is the historical v1 3-beat coupling depth every "
                         "existing log was produced with; 512 is the v2 full "
                         "4 KiB job (both 2 KiB halves) buffered per DMA lane. "
                         "Runtime flag only: the RTL source list and the obj "
                         "directory are deliberately unchanged, because the same "
                         "harness back-pressures and checks at both depths.")
    ap.add_argument("--fullbuffer-permit", action="store_true",
                    help="drive the 2-bit full-buffer permit vector as 3 during "
                         "the bounded 4 KiB copy (proxy for the real iDMA "
                         "full-buffer certificate). Requires --couple-depth 512. "
                         "With no --scenario this runs all 12 existing RR "
                         "scenarios; each scenario has one 4 KiB job per DMA, so "
                         "the A-enabled scenarios exercise two jobs on the two "
                         "separate 512-deep coupling FIFOs. Default is 0 (legacy "
                         "guard).")
    ap.add_argument("--continuous-aggressor", action="store_true",
                    help="opt-in: when set, and the selected scenario has an A "
                         "aggressor (a_pages > 0), the A DMA keeps re-issuing "
                         "4 KiB jobs (cycling the existing a_pages pages) as soon "
                         "as its previous job completes, until the V victim has "
                         "actually completed its final job; the already-active A "
                         "job is then drained.  Forwarded to the C++ harness as "
                         "--continuous-aggressor.  Default off -> the historical "
                         "fixed job-count run is unchanged.  Runtime flag only: "
                         "same RTL, same obj directory.")
    ap.add_argument("--v-gap-cycles", type=int, choices=(0, 1000, 4000), default=0,
                    help="V inter-job gap: complete ticks the V victim waits "
                         "between finishing one 4 KiB job and starting the next "
                         "one, forwarded to the C++ harness as "
                         "+v-gap-cycles=<...> (default: 0 = historical "
                         "back-to-back issue).  The wait is counted in tick() and "
                         "never gates A and never changes any IOVA/PA or page "
                         "stride.")
    ap.add_argument("--job-count", type=int, choices=(64, 4096), default=64,
                    help="jobs per DMA, forwarded to the C++ harness as "
                         "+job-count=<...> (default: 64).  64 keeps the "
                         "historical 12-scenario run; 4096 is the bounded long "
                         "continuous-work run.  Bounds are fixed (64 or 4096). "
                         "Runtime flag only: it never changes the obj dir; the "
                         "warm-up length is not scaled with it and makes no "
                         "board-warm256 claim.")
    ap.add_argument("--policy", action="store_true",
                    help="opt-in IOTLB replacement partition.  Elaborates the "
                         "test wrapper with -GPolicyEnable=1 (a compile "
                         "capability change, hence a SEPARATE obj directory so "
                         "the default artifact is never overwritten) and forwards "
                         "+policy=1 to the C++ harness, which drives the wrapper "
                         "input policy_partition_i=1.  Protected DID is 10. "
                         "Requires the banked nonblocking build (not --legacy).")
    ap.add_argument("--reduced", action="store_true",
                    help="reduced OT4/W8 config instead of the board OT8/W32 config")
    ap.add_argument("--scenario", default=None,
                    help="run only the named scenario, forwarded to the C++ "
                         "harness as +scenario=.  Without it the historical 12 "
                         "rows run; the opt-in 16-page aggressor rows "
                         "(mode1-a16-l1, mode1-a16-l31, mode2-a16-l1, "
                         "mode2-a16-l31) run ONLY when named here.")
    opts = ap.parse_args()

    if opts.fullbuffer_permit and opts.couple_depth != 512:
        ap.error("--fullbuffer-permit requires --couple-depth 512")
    if opts.policy and opts.legacy:
        ap.error("--policy requires the banked nonblocking build "
                 "(--legacy elaborates NonblockEnable=0, which the "
                 "PolicyEnable composition check rejects)")

    sibling = load_sibling()
    tb = ROOT / "tests" / "nonblock_coupled_test.cpp"
    sources = dedup([require(p) for p in sibling.SOURCES])

    OUT.mkdir(parents=True, exist_ok=True)
    # Default mode keeps its original obj dir so an already-passing build is
    # never overwritten.  Only the opt-in overlap mode uses a sub-directory.
    # --couple-depth is a RUNTIME flag only: same RTL, same Verilator source
    # list, no new SV adapter, so it never appears in the obj dir.  A depth-3
    # run and a depth-512 run of the same mode therefore reuse one binary.
    obj_root = (OUT
                / ("legacy" if opts.legacy else "banked")
                / ("reduced" if opts.reduced else "board"))
    if opts.overlap:
        obj_root = obj_root / "overlap"
    if opts.policy:
        # PolicyEnable is an ELABORATION-time capability of the test wrapper
        # (-GPolicyEnable).  Only this opt-in build changes it, so it gets its
        # own object directory and can never overwrite the default
        # (PolicyEnable=0) artifact.  --v-gap-cycles is runtime-only and
        # therefore never appears here.
        obj_root = obj_root / "policy"
    obj_dir = obj_root / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    # board depth: OT8 / W32 by default; --reduced selects OT4 / W8.
    num_ot, w_fifo = (4, 8) if opts.reduced else (8, 32)
    banked   = 0 if opts.legacy else 1
    nonblock = 0 if opts.legacy else 1

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
        "--top-module",
        TOP,
        "--Mdir",
        str(obj_dir),
        "-GBankedEnable={}".format(banked),
        "-GNonblockEnable={}".format(nonblock),
        "-GNumOutstandingTrans={}".format(num_ot),
        "-GWFifoDepth={}".format(w_fifo),
        # Elaboration-time only; see --policy above.
        "-GPolicyEnable={}".format(1 if opts.policy else 0),
    ]
    for inc in sibling.INCDIRS:
        args += ["-I{}".format(inc)]
    args += [str(p) for p in sources]
    args += [str(tb)]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce {}\n".format(exe))
        return 1

    run_args = [str(exe), "+order={}".format(opts.order),
                "+couple-depth={}".format(opts.couple_depth),
                "+job-count={}".format(opts.job_count)]
    run_args.append("+v-gap-cycles={}".format(opts.v_gap_cycles))
    run_args.append("+policy={}".format(1 if opts.policy else 0))
    if opts.scenario:
        run_args.append("+scenario={}".format(opts.scenario))
    if opts.overlap:
        run_args.append("--overlap")
    if opts.fullbuffer_permit:
        run_args.append("--fullbuffer-permit")
    if opts.continuous_aggressor:
        run_args.append("--continuous-aggressor")
    print("test_nonblock_coupled: job_count={} couple_depth={} order={} "
          "overlap={} fullbuffer_permit={} v_gap_cycles={} policy={} "
          "continuous_aggressor={}".format(
              opts.job_count, opts.couple_depth, opts.order,
              int(opts.overlap), int(opts.fullbuffer_permit),
              opts.v_gap_cycles, int(opts.policy),
              int(opts.continuous_aggressor)))
    run = subprocess.run(run_args)
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
