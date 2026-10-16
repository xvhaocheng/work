#!/usr/bin/env python3
"""Run the audited flat32..60 segment once; preserve logs and fail closed."""
import argparse
import fcntl
import hashlib
import json
import os
import re
import subprocess
import sys
import termios
import time

from p4m_resume35_driver import Console
from make_remaining29_runner import EXPECTED_REMAINING29

OUT_DIR = "/tmp/p4m-remaining29-20261002"
SCRIPT = "/tmp/platform-p4m-remaining29-20261002.sh"
EXPECTED_KERNEL = "6.5.0-rc2-00089-gefd9461d6459"
EXPECTED_TAINTED = "0"
TAIL_FLATS = (26, 27, 28, 29, 30, 31)
DEADLINE_S = 7200.0
PUMP_S = 2.0

ID_RE = re.compile(r"^formal-b([0-9]+)-g([0-9]+)$")
HOST_EXIT_RE = re.compile(rb"(?m)^REMAINING29_HOST_EXIT=([0-9]+)\r?$")
CASE_RE = re.compile(
    rb"(?m)^P4M: case_end id=(\S+) exit=(-?[0-9]+) cases_done=([0-9]+) total=([0-9]+)\r?$")
FINAL_RE = re.compile(
    rb"(?m)^P4M: final_exit=(-?[0-9]+) collection_failed=(-?[0-9]+) mode=(\S+) "
    rb"cases_done=([0-9]+) total=([0-9]+) output=(\S+)\r?$")
COMPLETE_RE = re.compile(
    rb"(?m)^P4M: complete ok mode=remaining29 cases=([0-9]+) total=([0-9]+) output=(\S+)\r?$")
SEGMENT_RE = re.compile(
    rb"(?m)^P4M: remaining29_segment_complete cases=([0-9]+) total=([0-9]+) "
    rb"full_formal60=not_claimed new_boot_segment=1 .*output=(\S+)\r?$")
TARGETS_ABSENT_RE = re.compile(rb"(?m)^TARGETS_ABSENT\r?$")
TARGETS_PRESENT_RE = re.compile(rb"(?m)^TARGETS_PRESENT\r?$")
SYNTAX_OK_RE = re.compile(rb"(?m)^SYNTAX_OK\r?$")
SYNTAX_BAD_RE = re.compile(rb"(?m)^SYNTAX_BAD\r?$")
XFER_OK_RE = re.compile(rb"(?m)^XFER_OK\r?$")


class Failure(Exception):
    """Expected, already diagnosed failure: report it and stop, never retry."""


def fail(message):
    raise Failure(message)


def read_bytes(path):
    try:
        with open(path, "rb") as handle:
            return handle.read()
    except OSError as exc:
        fail("cannot read %s: %s" % (path, exc))


def norm_entry(item):
    if isinstance(item, dict):
        return str(item["runner"]), int(item["flat"]), int(item["gap"])
    runner, flat, gap = item
    return str(runner), int(flat), int(gap)


def load_plan():
    try:
        plan = [("formal-b%02d-g%d" % (block, gap), i, gap)
                for i, (block, gap) in enumerate(EXPECTED_REMAINING29, 32)]
    except (KeyError, TypeError, ValueError) as exc:
        fail("unusable EXPECTED_REMAINING29 entry: %r" % (exc,))
    if len(plan) != 29:
        fail("EXPECTED_REMAINING29 holds %d entries, expected 29" % len(plan))
    return plan


def check_port_free(port):
    try:
        proc = subprocess.run(["lsof", "-t", str(port)], stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE, timeout=30)
    except (OSError, subprocess.SubprocessError) as exc:
        fail("lsof -t %s could not run: %s" % (port, exc))
    if proc.returncode == 1:
        return
    if proc.returncode == 0:
        pids = proc.stdout.decode("ascii", "replace").strip()
        fail("port %s is occupied (lsof rc=0 pids=%r)" % (port, pids))
    fail("lsof -t %s rc=%d stderr=%r"
         % (port, proc.returncode, proc.stderr.decode("utf-8", "replace").strip()))


def preflight(args, report):
    runner = read_bytes(args.runner)
    text = runner.decode("utf-8", "replace")
    token = "NEWBOOT_BOOT_ID='%s'" % args.boot_id
    if token not in text:
        fail("runner content lacks %s" % token)
    if "remaining29" not in text:
        fail("runner content does not declare the remaining29 segment")
    report["runner_bytes"] = len(runner)

    try:
        audit = json.loads(read_bytes(args.audit_report).decode("utf-8"))
    except ValueError as exc:
        fail("audit report is not valid JSON: %s" % exc)
    if audit.get("permit_ok") is not True:
        fail("audit permit_ok is not true")

    cases = audit.get("cases")
    if not isinstance(cases, list) or len(cases) != 6:
        fail("audit cases must hold exactly 6 entries")
    for entry in cases:
        if entry.get("status") != "verified_offline":
            fail("audit case %r status is not verified_offline"
                 % entry.get("runner_case_id"))

    plan = load_plan()
    expected_tail = sorted(["formal-b09-g0", "formal-b09-g1000", "formal-b10-g0",
                            "formal-b10-g2000", "formal-b10-g1000", "formal-b11-g2000"])
    tail_ids = sorted(str(entry.get("runner_case_id")) for entry in cases)
    if len(set(tail_ids)) != 6 or tail_ids != expected_tail:
        fail("audit case ids do not match flats 26..31: %r != %r"
             % (tail_ids, expected_tail))
    report["audit_tail_cases"] = tail_ids

    remaining = audit.get("remaining_ids")
    if not isinstance(remaining, list):
        fail("audit remaining_ids is not a list")
    try:
        observed = [norm_entry(item) for item in remaining]
    except (KeyError, TypeError, ValueError) as exc:
        fail("unusable audit remaining_ids entry: %r" % (exc,))
    if observed != plan:
        fail("audit remaining_ids does not equal EXPECTED_REMAINING29")

    check_port_free(args.port)
    return plan


def capture(console, base, returned=None):
    """Live buffer first; fall back to command() text only if the driver drained it."""
    raw = bytes(console.buf[base:])
    if not raw and returned is not None:
        raw = str(returned).encode("latin1", "replace")
    return raw


def board_identity(console, args, report):
    base = len(console.buf)
    returned = console.command(
        "uname -r; cat /proc/sys/kernel/random/boot_id; cat /proc/sys/kernel/tainted",
        settle=4)
    console.pump(1.0)
    lines = [line.strip() for line in capture(console, base, returned).split(b"\n")]
    wanted = (("kernel", EXPECTED_KERNEL.encode("ascii")),
              ("boot_id", args.boot_id.encode("utf-8")),
              ("tainted", EXPECTED_TAINTED.encode("ascii")))
    positions = []
    for label, value in wanted:
        if value not in lines:
            fail("board identity: no exact %s line %r" % (label, value.decode()))
        positions.append(lines.index(value))
    if positions != sorted(positions):
        fail("board identity lines out of order: %r" % (positions,))
    report["board_identity"] = [value.decode() for _label, value in wanted]


def board_targets_absent(console):
    command = ("test ! -e %s && test ! -e %s && echo TARGETS_ABSENT || echo TARGETS_PRESENT"
               % (OUT_DIR, SCRIPT))
    base = len(console.buf)
    returned = console.command(command, settle=4)
    console.pump(1.0)
    raw = capture(console, base, returned)
    if TARGETS_ABSENT_RE.search(raw) is None:
        fail("board: standalone TARGETS_ABSENT not observed")
    if TARGETS_PRESENT_RE.search(raw) is not None:
        fail("board: TARGETS_PRESENT observed, board paths are already in use")


def board_upload(console, args, report):
    data = read_bytes(args.runner)
    base = len(console.buf)
    returned = console.upload(data, SCRIPT, chunk=96, label="platform-p4m-remaining29")
    raw = str(returned).encode("latin1", "replace") or bytes(console.buf[base:])
    if XFER_OK_RE.search(raw) is None:
        fail("upload: standalone XFER_OK not observed")
    path = re.escape(SCRIPT.encode("ascii"))
    size_match = re.search(rb"(?m)^[ \t]*([0-9]+)[ \t]+" + path + rb"\r?$", raw)
    md5_match = re.search(rb"(?mi)^([0-9a-f]{32})[ \t]+" + path + rb"\r?$", raw)
    if size_match is None or md5_match is None:
        fail("upload: bare wc size line and/or md5 line not observed")
    size = int(size_match.group(1))
    md5 = md5_match.group(1).decode("ascii").lower()
    if size != len(data):
        fail("upload: reported size %d != local runner size %d" % (size, len(data)))
    local_md5 = hashlib.md5(data).hexdigest()
    if md5 != local_md5:
        fail("upload: reported md5 %s != local md5 %s" % (md5, local_md5))
    report["upload_size"] = size
    report["upload_md5"] = md5


def board_syntax(console):
    base = len(console.buf)
    returned = console.command("sh -n %s && echo SYNTAX_OK || echo SYNTAX_BAD" % SCRIPT,
                               settle=4)
    console.pump(1.0)
    raw = capture(console, base, returned)
    if SYNTAX_OK_RE.search(raw) is None:
        fail("board: standalone SYNTAX_OK not observed")
    if SYNTAX_BAD_RE.search(raw) is not None:
        fail("board: SYNTAX_BAD observed")


def board_run(console, report, plan):
    command = ("sh %s remaining29 %s; p4m_rc=$?; echo REMAINING29_HOST_EXIT=$p4m_rc\r"
               % (SCRIPT, OUT_DIR))
    base = len(console.buf)
    report["run_sent"] = True
    confirmed = bool(console.write_echo_counted(command.encode("ascii"), timeout=30))
    report["echo_confirmed"] = confirmed
    if not confirmed:
        report["errors"].append(
            "write_echo_counted reported False; the runner may already be running - "
            "continuing capture, never resending")
    deadline = time.monotonic() + DEADLINE_S
    previous = -1
    marker = None
    while True:
        raw = bytes(console.buf[base:])
        marker = HOST_EXIT_RE.search(raw)
        done = len(CASE_RE.findall(raw))
        if done != previous:
            previous = done
            print("progress: cases_done=%d total=29" % done, flush=True)
        if marker is not None:
            break
        report["cases"] = [m[0].decode("ascii") for m in CASE_RE.findall(raw)]
        if b"buildroot login:" in raw[-1000:]:
            fail("login prompt returned; execution state unknown")
        if time.monotonic() >= deadline:
            fail("no REMAINING29_HOST_EXIT within %d s" % int(DEADLINE_S))
        console.pump(PUMP_S)
    console.pump(PUMP_S)
    validate_run(bytes(console.buf[base:]), marker, report, plan)


def validate_run(raw, marker, report, plan):
    report["host_exit"] = int(marker.group(1))
    actual_final = FINAL_RE.search(raw)
    report["final_line"] = actual_final.group(0).decode("latin1").strip() if actual_final else None
    matches = CASE_RE.findall(raw)
    ids = [match[0].decode("latin1") for match in matches]
    report["cases"] = ids
    if len(matches) != 29:
        fail("expected 29 case_end lines, observed %d" % len(matches))
    if ids != [runner for runner, _flat, _gap in plan]:
        fail("observed case ids/order differ from the remaining plan")
    for index, match in enumerate(matches, start=1):
        case_id = match[0].decode("latin1")
        rc, done, total = int(match[1]), int(match[2]), int(match[3])
        if rc != 0:
            fail("case %s exit=%d" % (case_id, rc))
        if done != index or total != 29:
            fail("case %s counters cases_done=%d total=%d" % (case_id, done, total))

    host_exit = int(marker.group(1))
    report["host_exit"] = host_exit
    report["host_exit_line"] = marker.group(0).decode("latin1").strip()
    if host_exit != 0:
        fail("REMAINING29_HOST_EXIT=%d" % host_exit)

    final = FINAL_RE.search(raw)
    if final is None:
        fail("missing P4M: final_exit line")
    if (int(final.group(1)) != 0 or int(final.group(2)) != 0
            or final.group(3).decode("latin1") != "remaining29"
            or int(final.group(4)) != 29 or int(final.group(5)) != 29
            or final.group(6).decode("latin1") != OUT_DIR):
        fail("final line not accepted: %r" % final.group(0).decode("latin1").strip())
    report["final_line"] = final.group(0).decode("latin1").strip()

    complete = COMPLETE_RE.search(raw)
    if complete is None:
        fail("missing P4M: complete ok line")
    if (int(complete.group(1)) != 29 or int(complete.group(2)) != 29
            or complete.group(3).decode("latin1") != OUT_DIR):
        fail("complete line not accepted: %r" % complete.group(0).decode("latin1").strip())
    report["complete_line"] = complete.group(0).decode("latin1").strip()

    segment = SEGMENT_RE.search(raw)
    if segment is None:
        fail("missing P4M: remaining29_segment_complete line")
    if (int(segment.group(1)) != 29 or int(segment.group(2)) != 29
            or segment.group(3).decode("latin1") != OUT_DIR):
        fail("segment line not accepted: %r" % segment.group(0).decode("latin1").strip())
    report["segment_line"] = segment.group(0).decode("latin1").strip()


def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        description="run the remaining29 board segment once, fail closed")
    parser.add_argument("--runner", required=True,
                        help="host path of the remaining29 runner payload")
    parser.add_argument("--output", required=True,
                        help="host output directory, must not exist yet")
    parser.add_argument("--boot-id", required=True, dest="boot_id",
                        help="boot_id the board must report and the runner must embed")
    parser.add_argument("--audit-report", required=True, dest="audit_report")
    parser.add_argument("--port", default="/dev/cu.usbserial-AV0KRCHT", help="serial port of the board")
    return parser.parse_args(argv)


def emit(report, path):
    text = json.dumps(report, indent=2, sort_keys=True)
    if path is not None:
        try:
            with open(path, "w", encoding="utf-8") as handle:
                handle.write(text + "\n")
        except OSError as exc:
            print("cannot write report %s: %s" % (path, exc), file=sys.stderr)
    print(text, flush=True)


def main(argv=None):
    args = parse_args(argv)
    report = {
        "status": "starting",
        "output": args.output,
        "port": args.port,
        "runner": args.runner,
        "boot_id": args.boot_id,
        "case_ids": [],
        "cases": [],
        "host_exit": None,
        "echo_confirmed": None,
        "final_line": None,
        "errors": [],
        "linux_kept_running": True,
    }

    # Refuse an existing output path before any write, so nothing can be clobbered.
    if os.path.lexists(args.output):
        report["status"] = "refused_existing_output"
        report["errors"].append("output path already exists: %s" % args.output)
        emit(report, None)
        return 3

    try:
        plan = preflight(args, report)
    except Failure as exc:
        report["status"] = "preflight_failed"
        report["errors"].append(str(exc))
        emit(report, None)
        return 2

    try:
        os.makedirs(args.output)
    except OSError as exc:
        report["status"] = "refused_output"
        report["errors"].append("cannot create %s: %s" % (args.output, exc))
        emit(report, None)
        return 3

    report["case_ids"] = [runner for runner, _flat, _gap in plan]
    console = None
    try:
        console = Console(args.port, os.path.join(args.output, "driver.log"))
        console.open()
        # Console.open() now acquires exclusivity once and propagates failure.
        # Repeating TIOCEXCL on macOS returns EBUSY even for this same fd.
        if not console.login():
            fail("login() returned False; no Ctrl-C sent, no automatic rerun")
        board_identity(console, args, report)
        board_targets_absent(console)
        board_upload(console, args, report)
        board_syntax(console)
        board_run(console, report, plan)
        report["status"] = "ok"
        return 0
    except Failure as exc:
        report["status"] = "unknown" if report.get("run_sent") and report["host_exit"] is None else "failed"
        report["errors"].append(str(exc))
        return 1
    except Exception as exc:  # fail closed: report, close host, never retry
        report["status"] = "unknown" if report.get("run_sent") and report["host_exit"] is None else "failed"
        report["errors"].append("unexpected error: %r" % (exc,))
        return 1
    finally:
        if console is not None:
            try:
                console.close()  # closes the host session only; board Linux keeps running
            except Exception as exc:
                report["errors"].append("console.close failed: %r" % (exc,))
        emit(report, os.path.join(args.output, "report.json"))


if __name__ == "__main__":
    sys.exit(main())
