#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Independent acceptance checker for the full-batch contract.

CONTRACT CHECKER ONLY -- NOT A BENCHMARK.  It never produces, derives or
endorses a performance number.  It decides whether ONE
ACTUAL-DMA-FULL-BATCH-JSON record (optionally cross-checked against the
matching +event-trace CSV) is internally consistent.  This checks internal consistency, not record authenticity. Evidence also
requires the actual run log, source version and execution provenance.

Checked, independently of the C++ harness bookkeeping:
  * per-channel counts:  issue == done == target
  * ordering:            ready[0] == t0; ready[j] == done[j-1];
                         ready <= issue <= done; issue and done strictly
                         increasing; issue[j] >= done[j-1] (single active)
  * payload conservation: r_beats*8 == r_bytes == rd_bytes == target*4096,
                         wr_bytes == target*4096, aw == b, err == 0
  * registers:           reg_issued == reg_done == target, batch_complete,
                         not batch_stopped
  * quiet == 1, T_V >= 0, T_A >= 0, T_data == max(T_V, T_A), T_return >= T_data
  * scenario_budget present and positive
  * (optional) the event trace's full_dma_issue / full_dma_done pulse counts
    match the JSON issue/done counts bit for bit.

Usage
-----
    python3 tests/test_full_batch_contract.py <log-with-full-json> \
        [--trace <event-trace.csv>] [--target 64]

    # negative-test plan against a REAL successful record:
    python3 tests/test_full_batch_contract.py --selftest <real-success.log>
"""

import argparse
import copy
import json
import sys
from pathlib import Path

SCHEMA      = "actual-dma-full-batch-v1"
PAGE_BYTES  = 4096
FULL_TARGET = 64
CHANNEL_IDS = ("V", "A")


# ---------------------------------------------------------------------------
# parsing
# ---------------------------------------------------------------------------
def load_records(text):
    """Return every ACTUAL-DMA-FULL-BATCH-JSON record in `text`."""
    recs = []
    for line in text.splitlines():
        line = line.strip()
        if line.startswith("ACTUAL-DMA-FULL-BATCH-JSON "):
            recs.append(json.loads(line.split(" ", 1)[1]))
    return recs


def load_first_record(path):
    recs = load_records(Path(path).read_text(errors="replace"))
    if not recs:
        raise SystemExit("no ACTUAL-DMA-FULL-BATCH-JSON record in {}".format(path))
    if len(recs) != 1:
        raise SystemExit("expected exactly one full-batch record")
    if "nonblock_actual_dma_test: PASS" not in Path(path).read_text() or "SCENARIO-FAIL" in Path(path).read_text():
        raise SystemExit("run must finish with PASS and no SCENARIO-FAIL")
    return recs[0]


def count_trace_pulses(path):
    """Count full_dma_issue / full_dma_done tag bits from the CSV trace."""
    v_iss = a_iss = v_done = a_done = 0
    for line in Path(path).read_text(errors="replace").splitlines():
        parts = line.rstrip("\n").split(",")
        if len(parts) != 5:
            continue
        _cyc, ev, tag, _addr, _val = parts
        try:
            mask = int(tag)
        except ValueError:
            continue
        if ev == "full_dma_issue":
            if mask & 1:
                v_iss += 1
            if mask & 2:
                a_iss += 1
        elif ev == "full_dma_done":
            if mask & 1:
                v_done += 1
            if mask & 2:
                a_done += 1
    return v_iss, a_iss, v_done, a_done


# ---------------------------------------------------------------------------
# checks
# ---------------------------------------------------------------------------
def _need(cond, msg, fails):
    if not cond:
        fails.append(msg)


def check_channel(chan, target, fails):
    tag = chan.get("id", "?")
    ready = chan.get("ready")
    issue = chan.get("issue_cyc")
    done = chan.get("done_cyc")

    _need(chan.get("issue") == target,
          "{}: issue {} != {}".format(tag, chan.get("issue"), target), fails)
    _need(chan.get("done") == target,
          "{}: done {} != {}".format(tag, chan.get("done"), target), fails)

    if not (isinstance(ready, list) and isinstance(issue, list) and
            isinstance(done, list)):
        fails.append("{}: ready/issue_cyc/done_cyc must be arrays".format(tag))
        return

    _need(len(ready) == target,
          "{}: ready array len {} != {}".format(tag, len(ready), target), fails)
    _need(len(issue) == target,
          "{}: issue array len {} != {}".format(tag, len(issue), target), fails)
    _need(len(done) == target,
          "{}: done array len {} != {}".format(tag, len(done), target), fails)
    if not (len(ready) == len(issue) == len(done) == target):
        return

    _need(ready[0] == issue[0] or ready[0] >= 0,
          "{}: ready[0] must be a real cycle".format(tag), fails)
    for j in range(target):
        _need(ready[j] <= issue[j] <= done[j],
              "{}: order ready[{}]<=issue[{}]<=done[{}] violated".format(
                  tag, j, j, j), fails)
    for j in range(1, target):
        _need(ready[j] == done[j - 1],
              "{}: ready[{}] {} != done[{}] {}".format(
                  tag, j, ready[j], j - 1, done[j - 1]), fails)
        _need(issue[j] >= done[j - 1],
              "{}: issue[{}] precedes done[{}] (single-active)".format(
                  tag, j, j - 1), fails)
        _need(issue[j] > issue[j - 1],
              "{}: issue times not strictly increasing at {}".format(tag, j), fails)
        _need(done[j] > done[j - 1],
              "{}: done times not strictly increasing at {}".format(tag, j), fails)

    _need(chan.get("r_beats", 0) * 8 == chan.get("r_bytes"),
          "{}: r_beats*8 != r_bytes".format(tag), fails)
    _need(chan.get("rd_bytes") == target * PAGE_BYTES,
          "{}: rd_bytes {} != {}".format(tag, chan.get("rd_bytes"),
                                         target * PAGE_BYTES), fails)
    _need(chan.get("wr_bytes") == target * PAGE_BYTES,
          "{}: wr_bytes {} != {}".format(tag, chan.get("wr_bytes"),
                                         target * PAGE_BYTES), fails)
    _need(chan.get("r_bytes") == chan.get("rd_bytes"),
          "{}: r_bytes != rd_bytes".format(tag), fails)
    _need(chan.get("b") == chan.get("aw"),
          "{}: b {} != aw {}".format(tag, chan.get("b"), chan.get("aw")), fails)
    _need(chan.get("ar", 0) > 0 and chan.get("aw", 0) > 0,
          "{}: no AR/AW traffic observed".format(tag), fails)
    _need(chan.get("err") == 0,
          "{}: err {} != 0".format(tag, chan.get("err")), fails)
    _need(chan.get("reg_issued") == target,
          "{}: reg_issued {} != {}".format(tag, chan.get("reg_issued"), target), fails)
    _need(chan.get("reg_done") == target,
          "{}: reg_done {} != {}".format(tag, chan.get("reg_done"), target), fails)
    _need(chan.get("batch_complete") == 1,
          "{}: batch not complete".format(tag), fails)
    _need(chan.get("batch_stopped") == 0,
          "{}: batch reported STOP".format(tag), fails)


def check_record(rec, target=FULL_TARGET):
    """Return a (possibly empty) list of contract violations."""
    fails = []
    _need(rec.get("schema") == SCHEMA,
          "unexpected schema {!r}".format(rec.get("schema")), fails)
    _need(rec.get("quiet") == 1, "quiet flag is not 1", fails)

    chans = rec.get("channels")
    if not (isinstance(chans, list) and len(chans) == 2):
        fails.append("expected exactly two channels")
        return fails
    by_id = {}
    for c in chans:
        by_id[c.get("id")] = c
    if set(by_id) != set(CHANNEL_IDS):
        fails.append("channel ids are not V/A")
        return fails

    check_channel(by_id["V"], target, fails)
    check_channel(by_id["A"], target, fails)
    for label in CHANNEL_IDS:
        c = by_id[label]
        if len(c.get("ready", [])) == target and len(c.get("done_cyc", [])) == target and len(c.get("issue_cyc", [])) == target:
            _need(c["ready"][0] == rec.get("t0"), label + ": first ready != t0", fails)
            _need(c["first_issue"] == c["issue_cyc"][0], label + ": first issue mismatch", fails)
            _need(c["final_done"] == c["done_cyc"][-1], label + ": final done mismatch", fails)
            _need(rec.get("T_" + label) == c["final_done"] - rec.get("t0", 0), label + ": elapsed time mismatch", fails)

    _need(rec.get("T_V", -1) >= 0, "T_V is negative", fails)
    _need(rec.get("T_A", -1) >= 0, "T_A is negative", fails)
    _need(rec.get("T_data") == max(rec.get("T_V", -1), rec.get("T_A", -1)),
          "T_data != max(T_V,T_A)", fails)
    _need(rec.get("T_return", -1) >= rec.get("T_data", -1),
          "T_return < T_data", fails)

    sb = rec.get("scenario_budget")
    _need(isinstance(sb, (int, float)) and sb > 0,
          "scenario_budget missing or <= 0", fails)
    return fails


def check_trace(rec, trace_path, target=FULL_TARGET, fails=None):
    fails = [] if fails is None else fails
    v_iss, a_iss, v_done, a_done = count_trace_pulses(trace_path)
    by_id = {c.get("id"): c for c in rec.get("channels", [])}
    if "V" in by_id:
        _need(v_iss == target,
              "trace: V issue pulses {} != {}".format(v_iss, target), fails)
        _need(v_done == target,
              "trace: V done pulses {} != {}".format(v_done, target), fails)
        _need(v_iss == by_id["V"].get("issue") and
              v_done == by_id["V"].get("done"),
              "trace: V pulse counts disagree with the JSON record", fails)
    if "A" in by_id:
        _need(a_iss == target,
              "trace: A issue pulses {} != {}".format(a_iss, target), fails)
        _need(a_done == target,
              "trace: A done pulses {} != {}".format(a_done, target), fails)
        _need(a_iss == by_id["A"].get("issue") and
              a_done == by_id["A"].get("done"),
              "trace: A pulse counts disagree with the JSON record", fails)
    return fails


# ---------------------------------------------------------------------------
# negative-test plan: mutate a REAL successful record; every mutant must FAIL
# ---------------------------------------------------------------------------
def _mutations(rec):
    cases = []

    def add(name, mut):
        m = copy.deepcopy(rec)
        mut(m)
        cases.append((name, m))

    add("drop_last_V_done",
        lambda r: r["channels"][0]["done_cyc"].pop())
    add("add_extra_A_done",
        lambda r: r["channels"][1]["done_cyc"].append(
            r["channels"][1]["done_cyc"][-1] + 1))
    add("duplicate_V_done_timestamp",
        lambda r: r["channels"][0]["done_cyc"].__setitem__(
            3, r["channels"][0]["done_cyc"][2]))
    add("break_V_ready_alignment",
        lambda r: r["channels"][0]["ready"].__setitem__(
            2, r["channels"][0]["ready"][2] + 1))
    add("swap_V_issue_order",
        lambda r: r["channels"][0]["issue_cyc"].__setitem__(
            4, r["channels"][0]["issue_cyc"][3] - 1))
    add("bump_V_rd_bytes",
        lambda r: r["channels"][0].__setitem__(
            "rd_bytes", r["channels"][0]["rd_bytes"] + 8))
    add("bump_A_wr_bytes",
        lambda r: r["channels"][1].__setitem__(
            "wr_bytes", r["channels"][1]["wr_bytes"] + 64))
    add("break_V_aw_b_conservation",
        lambda r: r["channels"][0].__setitem__("b", r["channels"][0]["b"] - 1))
    add("set_A_error",
        lambda r: r["channels"][1].__setitem__("err", 1))
    add("shift_t0", lambda r: r.__setitem__("t0", r["t0"] + 1))
    add("false_T_V", lambda r: r.__setitem__("T_V", r["T_V"] + 1))
    add("clear_quiet",
        lambda r: r.__setitem__("quiet", 0))
    add("zero_scenario_budget",
        lambda r: r.__setitem__("scenario_budget", 0))
    add("shrink_reg_done",
        lambda r: r["channels"][0].__setitem__(
            "reg_done", r["channels"][0]["reg_done"] - 1))
    return cases


def selftest(path, target=FULL_TARGET):
    rec = load_first_record(path)
    base = check_record(rec, target)
    if base:
        print("REFERENCE RECORD IS ALREADY INVALID:")
        for f in base:
            print("  " + f)
        return 1
    print("reference record accepted ({} channels, target {})".format(
        len(rec.get("channels", [])), target))
    bad = 0
    for name, mutant in _mutations(rec):
        fails = check_record(mutant, target)
        if fails:
            print("REJECT  {}".format(name))
        else:
            print("ACCEPTED(!) {}".format(name))
            bad += 1
    if bad:
        print("{} mutant(s) NOT rejected -- checker is too weak".format(bad))
        return 1
    print("all {} mutants rejected".format(len(_mutations(rec))))
    return 0


# ---------------------------------------------------------------------------
def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("log", nargs="?", default=None,
                    help="file containing an ACTUAL-DMA-FULL-BATCH-JSON record")
    ap.add_argument("--trace", default=None,
                    help="optional +event-trace CSV to cross-check pulse counts")
    ap.add_argument("--target", type=int, default=FULL_TARGET,
                    help="per-channel job target (default 64)")
    ap.add_argument("--selftest", metavar="REAL_LOG", default=None,
                    help="load a REAL successful record and require every "
                         "mutation of it to be rejected")
    opts = ap.parse_args(argv)

    if opts.selftest:
        return selftest(opts.selftest, opts.target)

    if not opts.log:
        ap.error("provide a log path or --selftest REAL_LOG")
    rec = load_first_record(opts.log)
    fails = check_record(rec, opts.target)
    if opts.trace:
        fails = check_trace(rec, opts.trace, opts.target, fails)

    if fails:
        print("FULL-BATCH CONTRACT: REJECT")
        for f in fails:
            print("  " + f)
        return 1
    print("FULL-BATCH CONTRACT: ACCEPT "
          "(counts/order/ready/bytes/B/quiet/errors all consistent)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
