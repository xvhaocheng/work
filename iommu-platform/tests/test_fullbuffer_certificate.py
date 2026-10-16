#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the v5 idma_fullbuffer_certificate bounded unit test.

Builds tests/fullbuffer_certificate_test_top.sv with two real DUT instances:
CapacityBytes=4096 and CapacityBytes=24.  The C++ harness owns the clocks and
checks every permit pre-edge.

If the certificate RTL imports idma_pkg, add that package to `required`
before the certificate source and keep the package order before the DUT.
"""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
IDMA = ROOT / "cva6" / "corev_apu" / "iDMA"
OUT  = ROOT / "out" / "fullbuffer-certificate"
TOP  = "fullbuffer_certificate_test_top"


def tail(path: Path, lines: int = 200) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def require(path: Path):
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
    src = IDMA / "src"
    inc = IDMA / "include"

    required = [
        src / "idma_fullbuffer_certificate.sv",
        ROOT / "tests" / "fullbuffer_certificate_test_top.sv",
        ROOT / "tests" / "fullbuffer_certificate_test.cpp",
    ]

    sources = dedup([require(p) for p in required])

    include_dirs = [src]
    if inc.is_dir():
        include_dirs.append(inc)
    for d in include_dirs:
        if not d.exists():
            sys.exit("required include directory missing: {}".format(d))

    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

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
        "-GCapacityBytesLarge=4096",
        "-GCapacityBytesSmall=24",
    ]
    args += ["-I{}".format(d) for d in include_dirs]
    args += [str(p) for p in sources]

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

    run = subprocess.run([str(exe)])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
