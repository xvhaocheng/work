#!/usr/bin/env python3
"""Build and execute one finite RTL protocol test. No parameter sweep."""
from pathlib import Path
import hashlib
import json
import re
import subprocess
import time

UNIT = Path(__file__).resolve().parent
OUT = UNIT.parent
PLATFORM = OUT.parents[2]
CC = PLATFORM / "cva6/vendor/pulp-platform/common_cells"
SOURCES = [
    CC / "src/cf_math_pkg.sv",
    CC / "src/lzc.sv",
    OUT / "files/cva6/vendor/pulp-platform/common_cells/src/rr_arb_tree.sv",
    OUT / "files/tests/m_first_pte_gate.sv",
    UNIT / "m_first_pte_unit.sv",
]
CPP = UNIT / "m_first_pte_unit.cpp"


def main():
    record = {
        "schema": "first-pte-m-finite-unit-v1",
        "scope": "actual local gate plus actual modified RR arbiter; producer/consumer boundary stimuli",
        "revision": "r1: C++ clock driver for installed Verilator4.038; DUT unchanged",
        "previous_attempt": "attempt0-unsupported-binary/result.json",
        "verilator_version": subprocess.check_output(["verilator", "--version"], text=True).strip(),
        "fixed_parameters": {"H": 64, "f": 1, "NumIn": 4, "FullIdWidth": 6},
        "cwd": str(PLATFORM),
        "sources": [
            {"path": str(p), "sha256": hashlib.sha256(p.read_bytes()).hexdigest()}
            for p in SOURCES + [CPP]
        ],
        "runs": [],
    }
    commands = [
        (
            "build",
            [
                "verilator", "--cc", "--exe", "--build", "--assert", "-Wno-fatal",
                "--top-module", "m_first_pte_unit", "--Mdir", str(UNIT / "obj"),
                "-j", "2", "-I" + str(CC / "include"),
            ] + [str(p) for p in SOURCES] + [str(CPP)],
            180,
        ),
        ("execute", [str(UNIT / "obj/Vm_first_pte_unit")], 30),
    ]
    for name, command, timeout in commands:
        entry = {"name": name, "argv": command, "timeout_seconds": timeout}
        before = time.monotonic()
        with (UNIT / (name + ".log")).open("w") as log:
            try:
                result = subprocess.run(command, cwd=PLATFORM, stdout=log,
                                        stderr=subprocess.STDOUT, timeout=timeout)
                entry["exit_code"] = result.returncode
            except subprocess.TimeoutExpired:
                entry["error"] = "timeout"
                entry["exit_code"] = None
        entry["elapsed_seconds"] = time.monotonic() - before
        record["runs"].append(entry)
        (UNIT / "result.json").write_text(json.dumps(record, indent=2) + "\n")
        if entry["exit_code"] != 0:
            print(name + " failed; see " + str(UNIT / (name + ".log")))
            return 1
    log_text = (UNIT / "execute.log").read_text()
    match = re.search(r"^PASS (.+)$", log_text, re.MULTILINE)
    record["passed"] = match is not None
    if match:
        record["counts"] = {
            key: int(value) for key, value in re.findall(r"(\w+)=(\d+)", match.group(1))
        }
    (UNIT / "result.json").write_text(json.dumps(record, indent=2) + "\n")
    print("PASS " + match.group(1) if match else "No PASS marker; see execute.log")
    return 0 if match else 1


if __name__ == "__main__":
    raise SystemExit(main())
