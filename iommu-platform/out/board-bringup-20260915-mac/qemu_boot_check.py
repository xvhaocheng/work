#!/usr/bin/env python3
"""Bounded Linux login/proc regression on dell's QEMU virt, not an FPGA test."""
import argparse
import hashlib
import json
from pathlib import Path
import select
import subprocess
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--image", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--timeout", type=int, default=90)
    parser.add_argument("--init-shell", action="store_true",
                        help="Boot a diagnostic shell; does not test login authentication")
    args = parser.parse_args()
    image = args.image.resolve()
    args.output.mkdir(parents=True, exist_ok=False)
    command = ["qemu-system-riscv64", "-machine", "virt", "-m", "1G",
               "-smp", "1", "-nographic", "-monitor", "none", "-nic", "none",
               "-no-reboot", "-kernel", str(image), "-append",
               "console=ttyS0 earlycon=sbi panic=-1"]
    if args.init_shell:
        command[-1] += " rdinit=/bin/sh"
    result = {"command": command, "image_sha256": hashlib.sha256(image.read_bytes()).hexdigest(),
              "scope": "QEMU virt Linux login/proc only; no CVA6/iDMA/IOMMU hardware validation"}
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT, bufsize=0)
    received = bytearray()
    sent_login = False
    sent_probe = False
    deadline = time.monotonic() + args.timeout
    try:
        with (args.output / "console.log").open("xb") as log:
            while time.monotonic() < deadline:
                if not select.select([process.stdout], [], [], 1)[0]:
                    continue
                chunk = process.stdout.read(65536)
                if not chunk:
                    break
                log.write(chunk)
                log.flush()
                received.extend(chunk)
                if not sent_login and b"login:" in received:
                    process.stdin.write(b"root\n")
                    sent_login = True
                if (sent_login or args.init_shell) and not sent_probe and b"# " in received[-2048:]:
                    # Split the success marker so terminal echo cannot count as success.
                    probe = (b"mount -t proc proc /proc; id; uname -r; i=0; ok=1; while [ $i -lt 100 ]; do "
                             b"cat /proc/self/status >/dev/null && readlink /proc/self/exe >/dev/null "
                             b"&& (exit 0) || { ok=0; break; }; i=$((i+1)); done; "
                             b"[ $ok -eq 1 ] && printf '\\nPROC_%s_%s\\n' CHECK PASS; "
                             b"cat /proc/sys/kernel/tainted\n")
                    process.stdin.write(probe)
                    sent_probe = True
                if b"\r\nPROC_CHECK_PASS\r\n" in received or b"\nPROC_CHECK_PASS\n" in received:
                    # Read the taint output and any delayed warnings.
                    for _ in range(3):
                        if select.select([process.stdout], [], [], 1)[0]:
                            more = process.stdout.read(65536)
                            log.write(more)
                            received.extend(more)
                    break
    finally:
        process.terminate()
        try:
            process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait()
        result.update(login_attempted=sent_login, probe_sent=sent_probe,
                      probe_passed=b"\nPROC_CHECK_PASS" in received,
                      init_shell=args.init_shell,
                      kernel_fault=any(x in received for x in
                                       (b"Oops [", b"Kernel panic", b"WARNING: CPU:", b"BUG:")),
                      console_bytes=len(received), qemu_exit_code=process.returncode)
        result["passed"] = result["probe_passed"] and not result["kernel_fault"]
        (args.output / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result, indent=2))
    return 0 if result["passed"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
