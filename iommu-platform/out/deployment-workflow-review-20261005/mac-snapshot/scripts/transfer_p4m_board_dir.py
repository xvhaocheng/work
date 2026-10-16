#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Read-only transfer of one board directory over the console (v2).

The board tar is streamed raw between ASCII markers and verified locally
against the md5sum the board prints.  A corrupted stream fails the comparison
and is retried, so a glitch can never be mistaken for evidence.
"""

import argparse
import hashlib
import importlib.util
import json
import os
import re
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
spec = importlib.util.spec_from_file_location(
    "p4m_driver", os.path.join(HERE, "p4m_resume35_driver.py"))
driver = importlib.util.module_from_spec(spec)
spec.loader.exec_module(driver)


def md5_bytes(data):
    return hashlib.md5(data).hexdigest()


def prep_archive(console, remote_dir, archive):
    path = "/tmp/%s" % archive
    cmd = ("rm -f %s && tar -cf %s -C %s . && echo TAR_OK && "
           "wc -c < %s && md5sum < %s && echo PREP_DONE" % (path, path, remote_dir, path, path))
    out = console.command(cmd, settle=25)
    size_m = re.search(r"(?m)^[ \t]*([0-9]+)[ \t]*\r?$", out)
    md5_m = re.search(r"(?m)^[ \t]*([0-9a-f]{32})[ \t]+\S*[ \t]*\r?$", out)
    meta = {"prep_command": cmd, "prep_output": out, "archive": path}
    if "TAR_OK" not in out or "PREP_DONE" not in out or not size_m or not md5_m:
        meta["error"] = "tar/wc/md5 did not complete"
        return meta, None, None
    meta["expected_size"] = int(size_m.group(1))
    meta["expected_md5"] = md5_m.group(1)
    return meta, meta["expected_size"], meta["expected_md5"]


def stream_archive(console, remote_path, expected_size, expected_md5,
                   local_path, attempts=3):
    meta = {"attempts": []}
    budget = max(90.0, expected_size / 11520.0 * 2.5 + 60.0)
    for attempt in range(1, attempts + 1):
        begin = "P4M_XFER_BEGIN_%d" % attempt
        end = "P4M_XFER_END_%d" % attempt
        console.command("echo %s" % begin, settle=3)
        # The board tty translates every LF to CRLF under ONLCR, which inflates
        # a binary stream by one byte per newline (measured: ~4 KB on 1.8 MB).
        # Disable output post-processing and echo for the duration of the dump.
        console.command("stty -opost -echo", settle=3)
        start = len(console.buf)
        if not console.write_echo_counted(("cat %s\r" % remote_path).encode(), timeout=60):
            console.command("stty opost echo", settle=3)
            meta["attempts"].append({"attempt": attempt, "error": "cat echo incomplete"})
            continue
        deadline = time.monotonic() + budget
        while time.monotonic() < deadline:
            console.pump(1.0)
            if console.total - start > expected_size:
                break
        console.command("stty opost echo", settle=3)
        console.command("echo %s" % end, settle=4)
        blob = bytes(console.buf[start:])
        end_idx = blob.find(end.encode())
        if end_idx < 0:
            meta["attempts"].append({"attempt": attempt, "error": "end marker missing",
                                     "captured": len(blob)})
            continue
        # Locate the tar stream by its own header magic instead of guessing at
        # prompt/newline boundaries: a POSIX tar header carries "ustar" at
        # offset 257, so the header starts 257 bytes earlier.  Then take exactly
        # the size the board reported.
        magic_idx = blob.find(b"ustar")
        if magic_idx < 257:
            meta["attempts"].append({"attempt": attempt, "error": "tar magic not found",
                                     "captured": len(blob)})
            continue
        payload = blob[magic_idx - 257:magic_idx - 257 + expected_size]
        record = {"attempt": attempt, "expected_size": expected_size,
                  "captured_size": len(payload), "expected_md5": expected_md5,
                  "captured_md5": md5_bytes(payload)}
        record["size_ok"] = record["captured_size"] == expected_size
        record["md5_ok"] = record["captured_md5"] == expected_md5
        meta["attempts"].append(record)
        if record["size_ok"] and record["md5_ok"]:
            os.makedirs(os.path.dirname(local_path), exist_ok=True)
            with open(local_path, "wb") as handle:
                handle.write(payload)
            meta.update({"ok": True, "attempt_used": attempt,
                         "local_path": local_path, "local_size": len(payload),
                         "local_md5": record["captured_md5"]})
            return meta
    meta["ok"] = False
    return meta


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", default=driver.DEFAULT_PORT)
    parser.add_argument("--log", required=True)
    parser.add_argument("--report", required=True)
    parser.add_argument("--out-root", required=True)
    parser.add_argument("--remote-dir", required=True)
    parser.add_argument("--archive", required=True)
    parser.add_argument("--local-name", required=True)
    parser.add_argument("--state", default=None)
    args = parser.parse_args()

    console = driver.Console(args.port, args.log)
    console.open()
    report = {"started_utc": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
              "remote_dir": args.remote_dir, "archive": args.archive}
    try:
        report["login_ok"] = console.login()
        if not report["login_ok"]:
            report["abort"] = "login_failed"
            raise SystemExit(2)
        if args.state:
            state_cmds = {
                "identity": "uname -r; uptime; cat /proc/sys/kernel/tainted; "
                            "cat /sys/module/idma/parameters/path_observe; "
                            "cat /sys/module/idma/parameters/observe; "
                            "cat /proc/sys/kernel/printk",
                "dmesg_seq": "dmesg | grep -c 'idma_batch_start '; "
                             "dmesg | grep 'idma_batch_start ' | awk '{for(i=1;i<=NF;i++) "
                             "if ($i ~ /^seq=/) {s=substr($i,5)+0; if (s>m) m=s}} "
                             "END {print \"max_seq=\" m}'; "
                             "dmesg | grep 'idma_batch_start ' | tail -1",
                "dir_listing": "ls -la %s | head -40; ls %s | wc -l"
                               % (args.remote_dir, args.remote_dir),
            }
            report["board_state"] = {k: console.command(v, settle=6)
                                     for k, v in state_cmds.items()}
            with open(args.state, "w", encoding="utf-8") as handle:
                for name, text in report["board_state"].items():
                    handle.write("=== %s ===\n%s\n" % (name, text))

        prep_meta, size, md5 = prep_archive(console, args.remote_dir, args.archive)
        report["prep"] = prep_meta
        if size is None:
            report["ok"] = False
        else:
            local = os.path.join(args.out_root, args.local_name)
            report["transfer"] = stream_archive(
                console, "/tmp/%s" % args.archive, size, md5, local)
            report["ok"] = bool(report["transfer"].get("ok"))
        console.command("exit", settle=3)
    finally:
        console.close()

    report["finished_utc"] = time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())
    report["received_bytes"] = console.total
    with open(args.report, "w", encoding="utf-8") as handle:
        json.dump(report, handle, ensure_ascii=False, indent=2)
        handle.write("\n")
    print(json.dumps({k: report.get(k) for k in
                      ("ok", "login_ok", "received_bytes")}, ensure_ascii=False))
    if report.get("transfer"):
        print("transfer:", json.dumps(report["transfer"].get("attempts"),
                                      ensure_ascii=False))
    else:
        print("prep output tail:", (report.get("prep") or {}).get("prep_output", "")[-600:])
    return 0 if report.get("ok") else 1


if __name__ == "__main__":
    sys.exit(main())
