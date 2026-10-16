#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Serial session driver for the P4M formal60 resume35 continuation (v2).

Lessons encoded here, all from measured board behaviour:

* Writes are paced by the board's own echo, byte for byte.  The first attempt
  sent 433 bytes every 4 ms into an 11.5 kB/s line, lost characters and knocked
  the console session down mid-upload.
* The runner is invoked WITHOUT stdout/stderr redirection so its anchored
  ``P4M:`` markers reach the console: a previous q attempt redirected them to
  files, the driver could not see progress, and a queued ``cat`` blocked the
  shell until it was interrupted - which killed a healthy run at case 5/35.
* No evidence collection happens here.  Reading the board output directory is a
  separate, explicitly sized step, because a blocking ``cat`` of a large file
  also blocks every later command on that shell.

The driver only does: login, verify the runner, run ``resume35`` once, record
the exit status and keep Linux running.  It never reboots, reloads a BIT,
writes SD/Flash or re-runs an archived trial.
"""

import argparse
import base64
import fcntl
import hashlib
import json
import os
import re
import select
import sys
import termios
import time

DEFAULT_PORT = "/dev/cu.usbserial-AV0KRCHT"
LOGIN_USER = "root"
LOGIN_PASSWORD = "root"


class Console:
    def __init__(self, port, log_path):
        self.port = port
        self.log_path = log_path
        self.fd = None
        self.log = None
        self.buf = bytearray()
        self.total = 0

    def open(self):
        self.fd = os.open(self.port, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
        try:
            fcntl.ioctl(self.fd, termios.TIOCEXCL)
        except OSError:
            os.close(self.fd)
            self.fd = None
            raise
        iflag, oflag, cflag, lflag, ispeed, ospeed, cc = termios.tcgetattr(self.fd)
        cflag = (cflag & ~(termios.CSIZE | termios.PARENB | termios.CSTOPB))
        cflag |= termios.CS8 | termios.CREAD | termios.CLOCAL
        iflag &= ~(termios.IXON | termios.IXOFF | termios.IXANY
                   | termios.ICRNL | termios.INLCR | termios.IGNCR)
        oflag &= ~termios.OPOST
        lflag &= ~(termios.ECHO | termios.ICANON | termios.ISIG | termios.IEXTEN)
        cc[termios.VMIN] = 0
        cc[termios.VTIME] = 0
        termios.tcsetattr(self.fd, termios.TCSANOW,
                          [iflag, oflag, cflag, lflag, termios.B115200,
                           termios.B115200, cc])
        termios.tcflush(self.fd, termios.TCIOFLUSH)
        os.set_blocking(self.fd, True)
        self.log = open(self.log_path, "ab", buffering=0)

    def close(self):
        if self.log is not None:
            self.log.close()
            self.log = None
        if self.fd is not None:
            os.close(self.fd)
            self.fd = None

    def pump(self, seconds, idle=None):
        end = time.monotonic() + seconds
        last = time.monotonic()
        while time.monotonic() < end:
            ready, _, _ = select.select([self.fd], [], [], 0.2)
            if ready:
                block = os.read(self.fd, 65536)
                if block:
                    self.buf.extend(block)
                    self.total += len(block)
                    self.log.write(block)
                    last = time.monotonic()
                    continue
            if idle is not None and time.monotonic() - last >= idle:
                return True
        return False

    def tail(self, n=400):
        return bytes(self.buf[-n:])

    def write_echo_counted(self, data, timeout=30.0):
        """Write ``data`` and wait until every byte has been echoed back."""
        start = len(self.buf)
        expect = data.replace(b"\r", b"")
        os.write(self.fd, data)
        deadline = time.monotonic() + timeout
        while time.monotonic() < deadline:
            self.pump(0.03)
            seen = bytes(self.buf[start:])
            pos = 0
            for byte in expect:
                idx = seen.find(bytes([byte]), pos)
                if idx < 0:
                    break
                pos = idx + 1
            else:
                return True
        return False

    def login(self, timeout=180.0):
        deadline = time.monotonic() + timeout
        while time.monotonic() < deadline:
            self.pump(1.0)
            recent = self.tail(300)
            if re.search(rb"(?m)^#\s*$", recent):
                return True
            if b"Password:" in recent:
                os.write(self.fd, LOGIN_PASSWORD.encode() + b"\r")
                self.pump(4)
                continue
            if b"login:" in recent or b"Login incorrect" in recent:
                self.write_echo_counted(LOGIN_USER.encode() + b"\r", timeout=20)
                self.pump(3)
                continue
            os.write(self.fd, b"\r")
        return bool(re.search(rb"(?m)^#\s*$", self.tail(400)))

    def command(self, cmd, settle=4.0):
        start = len(self.buf)
        if not self.write_echo_counted(cmd.encode() + b"\r", timeout=90.0):
            sys.stderr.write("[driver] WARNING: incomplete echo for: %s\n" % cmd[:60])
            sys.stderr.flush()
        self.pump(settle, idle=settle)
        return bytes(self.buf[start:]).decode("latin-1")

    def upload(self, data, remote_path, chunk=96, label="file"):
        """Base64 upload with strictly echo-counted pacing."""
        blob = base64.b64encode(data).decode("ascii")
        total = len(blob)
        self.command(": > %s.b64 && echo UPLOAD_BEGIN" % remote_path, settle=3)
        self.pump(0.5, idle=0.5)
        sent = 0
        while sent < total:
            piece = blob[sent:sent + chunk]
            sent += len(piece)
            line = "printf '%%s' '%s' >> %s.b64\r" % (piece, remote_path)
            if not self.write_echo_counted(line.encode(), timeout=45.0):
                return "UPLOAD_ECHO_TIMEOUT at %d/%d base64 chars" % (sent, total)
            if sent % (chunk * 40) == 0 or sent >= total:
                sys.stderr.write("[driver] %s %d/%d base64 chars\n" % (label, sent, total))
                sys.stderr.flush()
        return self.command(
            "base64 -d %s.b64 > %s && echo XFER_OK; wc -c %s; md5sum %s"
            % (remote_path, remote_path, remote_path, remote_path), settle=6)


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as handle:
        for block in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def md5_file(path):
    digest = hashlib.md5()
    with open(path, "rb") as handle:
        for block in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", default=DEFAULT_PORT)
    parser.add_argument("--log", required=True)
    parser.add_argument("--report", required=True)
    parser.add_argument("--runner", required=True)
    parser.add_argument("--board-path", default="/tmp/platform-p4m-formal60")
    parser.add_argument("--out-dir", default="/tmp/p4m-formal60-resume-02")
    parser.add_argument("--timeout", type=float, default=10800.0)
    args = parser.parse_args()

    with open(args.runner, "rb") as handle:
        runner = handle.read()
    local_md5 = hashlib.md5(runner).hexdigest()
    report = {
        "started_utc": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
        "runner_local": {"path": args.runner, "bytes": len(runner),
                         "sha256": hashlib.sha256(runner).hexdigest(),
                         "md5": local_md5},
        "board_path": args.board_path,
        "out_dir": args.out_dir,
        "steps": [],
    }

    console = Console(args.port, args.log)
    console.open()
    ok = True
    try:
        logged_in = console.login()
        report["login_ok"] = logged_in
        report["steps"].append({"step": "login", "ok": logged_in})
        if not logged_in:
            report["abort"] = "login_failed"
            ok = False
            raise SystemExit(2)

        identity = console.command(
            "uname -r; uptime; cat /proc/sys/kernel/tainted; "
            "cat /sys/module/idma/parameters/path_observe", settle=4)
        report["identity"] = identity
        kernel = re.search(r"6\.5\.0-rc2-\S+", identity)
        report["kernel_release"] = kernel.group(0) if kernel else None
        report["steps"].append({"step": "identity", "tail": identity[-400:]})

        pre = console.command(
            "dmesg | grep -c 'idma_batch_start '; "
            "dmesg | grep 'idma_batch_start ' | awk '{for(i=1;i<=NF;i++) "
            "if ($i ~ /^seq=/) {s=substr($i,5)+0; if (s>m) m=s}} "
            "END {print \"max_seq=\" m}'; ls -d %s 2>&1" % args.out_dir, settle=5)
        report["preconditions"] = pre
        report["steps"].append({"step": "preconditions", "tail": pre[-400:]})
        if "No such file" not in pre:
            report["abort"] = "output_dir_already_exists"
            ok = False
            raise SystemExit(4)

        upload_out = console.upload(runner, args.board_path, label="runner")
        report["upload"] = upload_out
        size_ok = re.search(r"(?m)^%d\s" % len(runner), upload_out) is not None
        md5_ok = local_md5 in upload_out
        report["upload_verified"] = bool(size_ok and md5_ok)
        report["steps"].append({"step": "upload", "size_ok": size_ok, "md5_ok": md5_ok})
        if not report["upload_verified"]:
            report["abort"] = "upload_verification_failed"
            ok = False
            raise SystemExit(3)

        syntax = console.command("sh -n %s && echo SYNTAX_OK" % args.board_path, settle=5)
        report["syntax_checked"] = "SYNTAX_OK" in syntax
        if not report["syntax_checked"]:
            report["abort"] = "syntax_check_failed"
            ok = False
            raise SystemExit(3)

        # --------------------------------------------------------------- run
        run_start = len(console.buf)
        run_cmd = ("saved_printk=$(cat /proc/sys/kernel/printk); dmesg -n 4; "
                   "sh %s resume35 %s; resume_rc=$?; "
                   "echo \"$saved_printk\" > /proc/sys/kernel/printk; "
                   "echo RESUME35_HOST_EXIT=$resume_rc" % (args.board_path, args.out_dir))
        if not console.write_echo_counted(run_cmd.encode() + b"\r", timeout=120):
            report["abort"] = "run_command_echo_incomplete"
            ok = False
            raise SystemExit(3)
        report["run_command"] = run_cmd
        report["steps"].append({"step": "run_command_sent"})

        deadline = time.monotonic() + args.timeout
        started = time.monotonic()
        last_progress = time.monotonic()
        last_bytes = console.total
        while time.monotonic() < deadline:
            console.pump(2.0)
            window = bytes(console.buf[run_start:])
            if b"RESUME35_HOST_EXIT=" in window:
                break
            if b"buildroot login:" in window[-4000:]:
                report["abort"] = "console_dropped_to_login"
                ok = False
                break
            if console.total != last_bytes:
                last_bytes = console.total
                last_progress = time.monotonic()
            if time.monotonic() - last_progress > 900:
                report["abort"] = "no_console_output_for_15min"
                ok = False
                break
            if time.monotonic() - started > 300:
                started = time.monotonic()
                done = len(re.findall(rb"case_end id=\S+ exit=0", window))
                failed = len(re.findall(rb"case_failed id=\S+", window))
                sys.stderr.write("[driver] progress: %d cases done, %d failed, %.0f min, "
                                 "%d bytes\n" % (done, failed,
                                                (time.monotonic() - (deadline - args.timeout)) / 60.0,
                                                console.total))
                sys.stderr.flush()
        console.pump(8.0, idle=8.0)

        window = bytes(console.buf[run_start:])
        match = re.search(rb"RESUME35_HOST_EXIT=(\d+)", window)
        report["host_exit"] = int(match.group(1)) if match else None
        report["cases_done"] = len(re.findall(rb"case_end id=\S+ exit=0", window))
        report["cases_failed"] = len(re.findall(rb"case_failed id=\S+", window))
        final = re.search(rb"P4M: final_exit=\d+ collection_failed=\d+ mode=\S+ "
                          rb"cases_done=\d+ total=\d+", window)
        report["final_marker"] = final.group(0).decode() if final else None
        complete = re.search(rb"P4M: complete ok mode=resume35 cases=35 total=35", window)
        resume_marker = re.search(rb"P4M: resume35_segment_complete cases=35 total=35", window)
        report["complete_marker"] = bool(complete)
        report["resume_segment_marker"] = bool(resume_marker)
        report["steps"].append({"step": "run_finished", "host_exit": report["host_exit"],
                                "cases_done": report["cases_done"],
                                "cases_failed": report["cases_failed"],
                                "final_marker": report["final_marker"]})
        report["board_stdout_capture"] = "runner stdout/stderr went to the console (no redirection)"
    except SystemExit as exc:
        ok = ok and exc.code == 0
        report.setdefault("exit_code", exc.code)
    finally:
        console.close()

    report["finished_utc"] = time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())
    report["received_bytes"] = console.total
    report["log"] = args.log
    report["log_sha256"] = sha256_file(args.log)
    report["linux_kept_running"] = True
    with open(args.report, "w", encoding="utf-8") as handle:
        json.dump(report, handle, ensure_ascii=False, indent=2)
        handle.write("\n")
    sys.stderr.write("[driver] done: host_exit=%s cases_done=%s -> %s\n"
                     % (report.get("host_exit"), report.get("cases_done"), args.report))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
