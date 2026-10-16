#!/usr/bin/env python3
"""Collect an already-finished board result directory (read-only).

Usage: python3 scripts/collect_p4m_remaining29.py --output NEW_DIR

The board is only read: no experiment, no reboot, no deletion, no printk change.
"""
import argparse
import base64
import fcntl
import hashlib
import json
import os
import re
import shlex
import subprocess
import sys
import tarfile
import termios
import time

from p4m_resume35_driver import Console

DEFAULT_BOARD = "/tmp/p4m-remaining29-20261002"
DEFAULT_PORT = "/dev/cu.usbserial-AV0KRCHT"
TRANSFER_DEADLINE = 1800.0

try:
    TIOCEXCL = termios.TIOCEXCL
except AttributeError:  # Darwin does not export TIOCEXCL via termios
    TIOCEXCL = 0x2000740D

RE_DIR = re.compile(rb"(?m)^P4M_DIR_(OK|MISSING)\r?$")
RE_ARCH = re.compile(rb"(?m)^P4M_ARCH_(EXISTS|ABSENT)\r?$")
RE_TAR_RC = re.compile(rb"(?m)^P4M_TAR_RC=(\d+)\r?$")
RE_GZ = re.compile(rb"(?m)^P4M_GZ=[01]\r?$")
RE_GZ_RC = re.compile(rb"(?m)^P4M_GZ_RC=(\d+)\r?$")
RE_WC_DONE = re.compile(rb"(?m)^P4M_WC_DONE\r?$")
RE_MD5_DONE = re.compile(rb"(?m)^P4M_MD5_DONE\r?$")
RE_B64_BEGIN = re.compile(rb"(?m)^P4M_B64_BEGIN\r?$")
RE_B64_END = re.compile(rb"(?m)^P4M_B64_END\r?$")


def port_busy(port):
    """True when lsof reports the serial device as open."""
    try:
        res = subprocess.run(["lsof", port], stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
    except FileNotFoundError:
        raise RuntimeError("lsof not found; cannot verify exclusive use")
    if res.returncode not in (0, 1):
        raise RuntimeError("lsof failed: " + res.stderr.decode("utf-8", "replace"))
    return bool(res.stdout.strip())


def _drain(console, acc, prev):
    """Mirror new bytes of console.buf into acc; return updated high-water."""
    cur = bytes(console.buf)
    if len(cur) >= prev:
        acc += cur[prev:]
    else:  # buf shrank; keep everything, md5 still guards correctness
        acc += cur
    return len(cur)


def run_cmd(console, cmd, stop_re, timeout):
    """Send exactly one command and wait for its anchored whole line."""
    acc = bytearray()
    prev = len(console.buf)
    console.command(cmd)
    deadline = time.monotonic() + timeout
    while True:
        prev = _drain(console, acc, prev)
        if stop_re.search(bytes(acc)):
            return True, bytes(acc)
        if time.monotonic() >= deadline:
            return False, bytes(acc)
        console.pump(2.0, idle=0.5)


def transfer(console, target, report):
    """Send the single base64 dump command; never retried, never resent."""
    cmd = "echo P4M_B64_BEGIN; base64 %s; echo P4M_B64_END" % shlex.quote(target)
    acc = bytearray()
    prev = len(console.buf)
    echo_ok = None
    try:
        echo_ok = console.write_echo_counted(cmd.encode() + b"\r", 15)
    except Exception as exc:  # boolean failure must not trigger a resend
        report["errors"].append("write_echo_counted: %r" % exc)
    report["echo_confirmed"] = echo_ok  # bool or None -> recorded uncertainty
    deadline = time.monotonic() + TRANSFER_DEADLINE
    while True:
        prev = _drain(console, acc, prev)
        if RE_B64_END.search(bytes(acc)):
            break
        if time.monotonic() >= deadline:
            report["uncertainties"].append("transfer deadline reached")
            break
        console.pump(5.0, idle=2.0)

    raw = bytes(acc)
    ends = list(RE_B64_END.finditer(raw))
    if not ends:
        report["errors"].append("P4M_B64_END marker not found")
        return None
    me = ends[0]
    mb = None
    for cand in RE_B64_BEGIN.finditer(raw):
        if cand.end() <= me.start():
            mb = cand
        else:
            break
    if mb is None:
        report["errors"].append("P4M_B64_BEGIN marker not found")
        return None
    payload = raw[mb.end():me.start()].replace(b"\r", b"").replace(b"\n", b"")
    try:
        return base64.b64decode(payload, validate=True)
    except Exception as exc:
        report["errors"].append("base64 decode: %r" % exc)
        return None


def safe_extract(archive, dest):
    """Reject unsafe members first, then extract with filter='data'."""
    os.makedirs(dest)
    with tarfile.open(archive) as tf:
        for member in tf.getmembers():
            name = member.name
            if name.startswith("/") or os.path.isabs(name):
                raise RuntimeError("absolute member rejected: %r" % name)
            if ".." in name.replace("\\", "/").split("/"):
                raise RuntimeError("parent member rejected: %r" % name)
            if member.issym() or member.islnk():
                raise RuntimeError("link member rejected: %r" % name)
        try:
            tf.extractall(dest, filter="data")
        except TypeError:
            raise RuntimeError("tarfile filter='data' unsupported (need Py>=3.12)")


def run_collection(console, args, out, report):
    q = shlex.quote
    board = args.board.rstrip("/") or "/"
    arch = board + "-evidence.tar"
    target = arch
    report["board_dir"] = board
    report["archive"] = arch

    found, txt = run_cmd(console,
        "test -d %s && echo P4M_DIR_OK || echo P4M_DIR_MISSING" % q(board),
        RE_DIR, 30)
    if not found or RE_DIR.search(txt).group(1) != b"OK":
        report["errors"].append("board dir missing or unreadable: %s" % board)
        return 1

    found, txt = run_cmd(console,
        "test -e %s && echo P4M_ARCH_EXISTS || echo P4M_ARCH_ABSENT" % q(arch),
        RE_ARCH, 30)
    if not found or RE_ARCH.search(txt).group(1) != b"ABSENT":
        report["errors"].append("archive exists; refusing to overwrite: %s" % arch)
        return 1

    found, txt = run_cmd(console,
        "tar -cf %s -C %s . ; echo P4M_TAR_RC=$?" % (q(arch), q(board)),
        RE_TAR_RC, 900)
    match = RE_TAR_RC.search(txt)
    tar_rc = int(match.group(1)) if match else None
    report["exit_codes"]["tar"] = tar_rc
    if tar_rc != 0:
        report["errors"].append("tar exit code %r" % (tar_rc,))
        return 1

    found, txt = run_cmd(console,
        "command -v gzip >/dev/null 2>&1 && echo P4M_GZ=1 || echo P4M_GZ=0",
        RE_GZ, 30)
    if found and RE_GZ.search(txt).group(0).rstrip(b"\r") == b"P4M_GZ=1":
        gz = arch + ".gz"
        absent, probe = run_cmd(console, "test -e %s && echo P4M_ARCH_EXISTS || echo P4M_ARCH_ABSENT" % q(gz), RE_ARCH, 30)
        if not absent or RE_ARCH.search(probe).group(1) != b"ABSENT":
            raise RuntimeError("gzip archive target exists or unknown; refusing overwrite")
        found, txt = run_cmd(console,
            "gzip -c %s > %s ; echo P4M_GZ_RC=$?" % (q(arch), q(gz)),
            RE_GZ_RC, 900)
        match = RE_GZ_RC.search(txt)
        gz_rc = int(match.group(1)) if match else None
        report["exit_codes"]["gzip"] = gz_rc
        if gz_rc == 0:
            target = gz
        else:
            report["uncertainties"].append("gzip failed; keeping plain tar")
    report["board_target"] = target

    found, txt = run_cmd(console, "wc -c %s ; echo P4M_WC_DONE" % q(target),
                         RE_WC_DONE, 60)
    match = re.search(rb"(?m)^[ \t]*(\d+)[ \t]+"
                      + re.escape(target.encode()) + rb"\r?$", txt)
    if not match:
        report["errors"].append("wc -c line not matched for %s" % target)
        return 1
    wc_bytes = int(match.group(1))

    found, txt = run_cmd(console, "md5sum %s ; echo P4M_MD5_DONE" % q(target),
                         RE_MD5_DONE, 120)
    match = re.search(rb"(?m)^[ \t]*([0-9a-f]{32})[ \t]+"
                      + re.escape(target.encode()) + rb"\r?$", txt)
    if not match:
        report["errors"].append("md5sum line not matched for %s" % target)
        return 1
    board_md5 = match.group(1).decode()
    report["board"] = {"bytes": wc_bytes, "md5": board_md5}

    data = transfer(console, target, report)
    if data is None:
        return 1
    local_md5 = hashlib.md5(data).hexdigest()
    report["transfer"] = {"bytes": len(data), "md5": local_md5}
    if len(data) != wc_bytes or local_md5 != board_md5:
        report["errors"].append(
            "size/md5 mismatch: board %d/%s, local %d/%s"
            % (wc_bytes, board_md5, len(data), local_md5))
        return 1

    saved = os.path.join(out, os.path.basename(target))
    with open(saved, "wb") as handle:
        handle.write(data)
    report["saved_archive"] = saved
    dest = os.path.join(out, "unpacked")
    safe_extract(saved, dest)
    report["unpacked"] = dest
    return 0


def write_report(out, report, console):
    report["finished_local"] = time.strftime("%Y-%m-%dT%H:%M:%S")
    if console is not None:
        try:
            report["console_buf_bytes"] = len(console.buf)
        except Exception:
            pass
    with open(os.path.join(out, "report.json"), "w") as handle:
        json.dump(report, handle, indent=2, sort_keys=True)
    log = os.path.join(out, "console.raw.log")
    if not os.path.exists(log):
        blob = b""
        if console is not None:
            try:
                blob = bytes(console.buf)
            except Exception:
                blob = b""
        with open(log, "wb") as handle:
            handle.write(blob)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", required=True, help="new local directory")
    parser.add_argument("--board", default=DEFAULT_BOARD)
    parser.add_argument("--port", default=DEFAULT_PORT)
    args = parser.parse_args(argv)

    out = os.path.abspath(args.output)
    if os.path.exists(out):
        sys.stderr.write("refuse: output already exists: %s\n" % out)
        return 2
    os.makedirs(out)

    report = {
        "port": args.port,
        "board": args.board,
        "output": out,
        "started_local": time.strftime("%Y-%m-%dT%H:%M:%S"),
        "tiocxcl": TIOCEXCL,
        "errors": [],
        "uncertainties": [],
        "exit_codes": {},
    }

    console = None
    try:
        try:
            if port_busy(args.port):
                report["errors"].append("serial port busy per lsof: %s" % args.port)
                return 2
            console = Console(args.port, os.path.join(out, "console.raw.log"))
            console.open()  # Acquires TIOCEXCL once, raises on failure.
            if not console.login():
                report["errors"].append("Console.login() returned False")
                return 2
            rc = run_collection(console, args, out, report)
            report["exit_code"] = rc
            return rc
        except Exception as exc:
            report["errors"].append("%s: %s" % (type(exc).__name__, exc))
            return 1
    finally:
        if console is not None:
            try:
                console.close()  # local close only; board stays Linux/untouched
            except Exception as exc:
                report["errors"].append("close: %r" % exc)
        write_report(out, report, console)


if __name__ == "__main__":
    sys.exit(main())

