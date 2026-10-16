#!/usr/bin/env python3
"""One delay-free component build and four bounded protocol invocations."""
from pathlib import Path
import hashlib
import json
import resource
import subprocess
import time

P = Path(__file__).resolve().parent
ROOT = P.parents[3]
resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
cc = ROOT / "cva6/corev_apu/riscv-iommu/vendor/common_cells"
obj = P / "component-obj"
cmd = [
    "verilator", "--cc", "--exe", "--build", "-j", "2", "-Wno-fatal",
    "+define+ASSERTS_OFF", "-CFLAGS", "-std=c++17",
    "-I" + str(cc / "include"), "--top-module", "research_ar_component_tb",
    "--Mdir", str(obj), str(cc / "src/cf_math_pkg.sv"),
    str(cc / "src/lzc.sv"), str(cc / "src/rr_arb_tree.sv"),
    str(P / "research_ar_selector.sv"), str(P / "component_tb.sv"),
    str(P / "component_test.cpp"),
]
start = time.monotonic()
with (P / "component-build.log").open("w") as log:
    build = subprocess.run(cmd, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT, timeout=120)
result = {
    "attempt": 2,
    "attempt1_preserved": str(P / "component-attempt1-preservation.json"),
    "core_dump_limit": 0,
    "helper_sha256": hashlib.sha256((P / "research_ar_selector.sv").read_bytes()).hexdigest(),
    "build": {
        "command": cmd, "exit_code": build.returncode,
        "elapsed_seconds": time.monotonic() - start,
        "log": str(P / "component-build.log"),
    },
    "cases": [],
}
if build.returncode == 0:
    exe = obj / "Vresearch_ar_component_tb"
    cases = [
        ("rr-held", ["+ar-ds-first=0"], True),
        ("ds-f4-held", ["+ar-ds-first=1"], True),
        ("changed-held-rejected", ["+ar-ds-first=1", "+bad-held=1"], False),
        ("invalid-mode-rejected", ["+ar-ds-first=3"], False),
    ]
    for label, args, want_pass in cases:
        invocation = [str(exe)] + args
        log_path = P / ("component-" + label + ".log")
        started = time.monotonic()
        with log_path.open("w") as log:
            run = subprocess.run(invocation, cwd=ROOT, stdout=log,
                                 stderr=subprocess.STDOUT, timeout=10)
        output = log_path.read_text()
        summaries = [json.loads(line.split(" ", 1)[1]) for line in output.splitlines()
                     if line.startswith("AR-OWNER-SUMMARY-JSON ")]
        quiet = (len(summaries) == 1 and summaries[0]["held"] == 0 and
                 summaries[0]["first_offer_count"] == summaries[0]["accept_count"])
        if want_pass:
            accepted = run.returncode == 0 and "COMPONENT-PASS" in output and quiet
        else:
            message = "held" if "held" in label else "ar-ds-first must be"
            accepted = run.returncode != 0 and message in output
        result["cases"].append({
            "case": label, "command": invocation, "exit_code": run.returncode,
            "elapsed_seconds": time.monotonic() - started,
            "log": str(log_path), "expected_success": want_pass,
            "summary": summaries[0] if len(summaries) == 1 else None,
            "accepted": accepted,
        })
result["passed"] = (build.returncode == 0 and len(result["cases"]) == 4 and
                    all(case["accepted"] for case in result["cases"]))
(P / "component-results.json").write_text(json.dumps(result, indent=2) + "\n")
print(json.dumps(result))
raise SystemExit(0 if result["passed"] else 1)
