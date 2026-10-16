#!/usr/bin/env python3
"""Host-side tests for the bench/idma_research.c research2 (v2) CLI.

Uses an LD_PRELOAD ioctl stub. No hardware, no root, no /dev/mem. The stub only
substitutes kernel ioctl responses; every predicate exercised here is the
userspace CLI's own parsing, range/policy validation, capability/role/timing/
policy raw-map checks and JSON encoding.

These are host-compiled CLI tests. They do not establish board behaviour.
"""

import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]


def main():
    checks = 0
    with tempfile.TemporaryDirectory(prefix="idma-research2-test-") as temporary:
        temp = Path(temporary)
        include = temp / "uapi"
        (include / "linux").mkdir(parents=True)
        shutil.copy2(ROOT / "linux/include/uapi/linux/idma.h", include / "linux/idma.h")
        compiler = os.environ.get("HOSTCC", "cc")
        flags = [compiler, "-std=c11", "-Wall", "-Wextra", "-Werror", "-O2",
                 "-I", str(include)]
        binary = temp / "idma_research"
        stub = temp / "research2_ioctl_stub.so"
        subprocess.run(flags + [str(ROOT / "bench/idma_research.c"), "-o", str(binary)],
                       check=True)
        subprocess.run(flags + ["-shared", "-fPIC",
                                str(ROOT / "tests/research2_cli_stub.c"), "-o", str(stub)],
                       check=True)
        device = temp / "idma0"
        device.touch()
        log = temp / "calls.log"

        def raw_run(args, stub_mode):
            log.unlink(missing_ok=True)
            env = os.environ.copy()
            env.update(LD_PRELOAD=str(stub), IDMA_TEST_MODE=stub_mode,
                       IDMA_TEST_LOG=str(log))
            cmd = [str(binary), "--device", str(device)] + list(args)
            result = subprocess.run(cmd, env=env, text=True, capture_output=True,
                                    check=False)
            assert len(result.stdout.splitlines()) == 1, (stub_mode, args, result.stdout)
            row = json.loads(result.stdout)
            actual = log.read_text().splitlines() if log.exists() else []
            return result.returncode, row, actual

        def run_v2(args, stub_mode="ok", good=True, calls=("caps", "run2")):
            nonlocal checks
            rc, row, actual = raw_run(args, stub_mode)
            assert (rc == 0) == good, (stub_mode, args, rc, row)
            assert row["schema"] == "idma-research-v1", row
            assert row["success"] == good, (stub_mode, args, row)
            assert actual == list(calls), (stub_mode, args, actual)
            checks += 1
            return row

        def reject(args, needle):
            row = run_v2(args, good=False, calls=())
            assert row["validated"] is False, (args, row)
            assert any(needle in message for message in row["errors"]), (
                args, needle, row["errors"])
            return row

        def run_caps(args, stub_mode="ok", good=True, calls=("caps2",)):
            nonlocal checks
            rc, row, actual = raw_run(args, stub_mode)
            assert (rc == 0) == good, (stub_mode, args, rc, row)
            assert row["schema"] == "idma-research2-caps-v1", row
            assert row["success"] == good, (stub_mode, args, row)
            assert actual == list(calls), (stub_mode, args, actual)
            checks += 1
            return row

        # --- legacy v1 path regression ---
        rc, row, calls = raw_run(["--e2"], "ok")
        assert rc == 0 and row["success"] is True and row["mode"] == "e2"
        assert row["schema"] == "idma-research-v1" and "research2" not in row
        assert calls == ["caps", "run"]
        rc, row, calls = raw_run(["--cost", "--a-pages", "0"], "ok")
        assert rc == 0 and row["success"] is True and row["mode"] == "cost"
        assert calls == ["caps", "run"]
        checks += 1

        # --- --e2-ext success and shape ---
        row = run_v2(["--e2-ext"])
        assert row["mode"] == "e2" and row["validated"] is True
        assert row["stage"] == 5 and row["raw_captured"] == 4096
        assert len(row["raw"]) == 4096
        assert row["research2"]["api_version"] == 2
        assert row["research2"]["role"] == 1
        assert row["research2"]["config"]["required_role"] == 1
        assert row["research2"]["config"]["policy_mode"] == 0
        assert row["research2"]["config"]["window_cycles"] == 0
        assert row["research2"]["config"]["target_bytes"] == 0
        assert row["research2"]["caps"]["magic"] == "0x49444d4132434150"
        assert row["research2"]["caps"]["version"] == 1
        assert row["research2"]["caps"]["role"] == 1
        assert row["input"]["a_pages"] == 8 and row["input"]["a_transfers"] == 112

        row = run_v2(["--e2-ext", "--a-pages", "0"])
        assert row["input"]["a_transfers"] == 0
        row = run_v2(["--e2-ext", "--a-pages", "16", "--a-transfers", "1", "--reverse", "1"])
        assert row["input"]["a_transfers"] == 1
        assert row["research2"]["config"]["reverse"] == 1

        # --- --method success, defaults and readback ---
        row = run_v2(["--method"])
        assert row["mode"] == "cost"
        cfg = row["research2"]["config"]
        assert row["research2"]["role"] == 2 and cfg["required_role"] == 2
        assert cfg["policy_mode"] == 0
        assert cfg["window_cycles"] == 16384
        assert cfg["target_bytes"] == 51540
        assert cfg["deadband_bytes"] == 4096
        assert cfg["initial_budget"] == 8000
        assert cfg["min_budget"] == 1
        assert cfg["max_budget"] == 32768
        assert cfg["budget_step"] == 512
        assert cfg["credit_cap"] == 65536
        assert cfg["hit_weight"] == 1 and cfg["miss_weight"] == 100
        assert cfg["ptw_weight"] == 100

        row = run_v2(["--method", "--policy-mode", "6"])
        cfg = row["research2"]["config"]
        assert cfg["min_budget"] == 0 and cfg["max_budget"] == 200000
        assert cfg["credit_cap"] >= cfg["max_budget"]

        row = run_v2(["--method", "--a-pages", "0"])
        assert row["input"]["a_pages"] == 0

        row = run_v2(["--method", "--policy-mode", "3", "--policy-window", "4096",
                      "--target-bytes", "65536", "--deadband-bytes", "1024",
                      "--initial-budget", "9000", "--min-budget", "2",
                      "--max-budget", "65536", "--budget-step", "256",
                      "--credit-cap", "131072", "--hit-weight", "2",
                      "--miss-weight", "50", "--ptw-weight", "75"])
        cfg = row["research2"]["config"]
        assert cfg["policy_mode"] == 3 and cfg["window_cycles"] == 4096
        assert cfg["target_bytes"] == 65536 and cfg["deadband_bytes"] == 1024
        assert cfg["initial_budget"] == 9000 and cfg["min_budget"] == 2
        assert cfg["max_budget"] == 65536 and cfg["budget_step"] == 256
        assert cfg["credit_cap"] == 131072 and cfg["hit_weight"] == 2
        assert cfg["miss_weight"] == 50 and cfg["ptw_weight"] == 75

        # --- argument/range/policy rejection (no ioctl) ---
        reject(["--e2-ext", "--a-pages", "3"], "invalid E2 extension --a-pages")
        reject(["--e2-ext", "--a-pages", "0", "--a-transfers", "8"], "invalid E2 extension --a-transfers")
        reject(["--e2-ext", "--a-pages", "1", "--a-transfers", "2"], "invalid E2 extension --a-transfers")
        reject(["--e2-ext", "--policy-mode", "1"], "does not accept policy configuration")
        reject(["--e2-ext", "--reverse", "2"], "invalid --reverse")
        reject(["--e2-ext", "--method"], "specify exactly one")
        reject(["--e2-ext", "--e2-ext"], "duplicate option")
        reject(["--e2-ext", "--unknown"], "unknown argument")
        reject(["--method", "--policy-mode", "8"], "invalid --policy-mode")
        reject(["--method", "--policy-mode", "1", "--a-pages", "0"], "requires aggressor pages")
        reject(["--method", "--a-gap-cycles", "1000", "--policy-mode", "1"], "requires --policy-mode 0")
        reject(["--method", "--a-gap-cycles", "250"], "invalid method --a-gap-cycles")
        reject(["--method", "--policy-mode", "6", "--max-budget", "300000"], "exceeds 200000")
        reject(["--method", "--policy-window", "100"], "invalid --policy-window")
        reject(["--method", "--target-bytes", "0"], "invalid --target-bytes")
        reject(["--method", "--deadband-bytes", "100000"], "exceeds --target-bytes")
        reject(["--method", "--min-budget", "0"], "min-budget >= 1")
        reject(["--method", "--budget-step", "0"], "invalid --budget-step")
        reject(["--method", "--credit-cap", "10"], "invalid --credit-cap")
        reject(["--method", "--min-budget", "9000", "--initial-budget", "8000"], "budget ordering")
        reject(["--caps2", "--e2"], "standalone query")
        reject(["--caps2", "--policy-mode", "1"], "standalone query")

        # --- F00 capability/role checks on the run path ---
        row = run_v2(["--e2-ext"], stub_mode="caps_bad_magic", good=False)
        assert any("F00" in m for m in row["errors"])
        row = run_v2(["--e2-ext"], stub_mode="caps_bad_version", good=False)
        assert any("F01" in m for m in row["errors"])
        row = run_v2(["--e2-ext"], stub_mode="caps_bad_role", good=False)
        assert any("F02" in m for m in row["errors"])
        row = run_v2(["--method"], stub_mode="caps_bad_role", good=False)
        assert any("F02" in m for m in row["errors"])

        # --- E2 timing D80 block ---
        row = run_v2(["--e2-ext"], stub_mode="timing_bad_magic", good=False)
        assert any("D80" in m for m in row["errors"])
        row = run_v2(["--e2-ext"], stub_mode="timing_bad_version", good=False)
        assert any("D81" in m for m in row["errors"])
        row = run_v2(["--e2-ext"], stub_mode="timing_error", good=False)
        assert any("D82" in m for m in row["errors"])
        row = run_v2(["--e2-ext"], stub_mode="timing_bad_flags", good=False)
        assert any("D83" in m for m in row["errors"])
        row = run_v2(["--e2-ext"], stub_mode="timing_bad_eq", good=False)
        assert any("done-issue" in m for m in row["errors"])

        # --- method E00 block + readback ---
        row = run_v2(["--method"], stub_mode="policy_bad_magic", good=False)
        assert any("E00 magic" in m for m in row["errors"])
        row = run_v2(["--method"], stub_mode="policy_bad_version", good=False)
        assert any("E01" in m for m in row["errors"])
        row = run_v2(["--method"], stub_mode="policy_error", good=False)
        assert any("E02" in m for m in row["errors"])
        row = run_v2(["--method"], stub_mode="policy_not_frozen", good=False)
        assert any("frozen" in m for m in row["errors"])
        row = run_v2(["--method"], stub_mode="policy_echo_mismatch", good=False)
        assert any("echo" in m for m in row["errors"])

        # --- old fullstats semantics retained on the method path ---
        row = run_v2(["--method", "--require-fullstats"], stub_mode="fullstats_good")
        assert row["success"] is True
        row = run_v2(["--method", "--require-fullstats"], stub_mode="ok", good=False)
        assert any("fullstats magic missing" in m for m in row["errors"])

        # --- standalone --caps2 query ---
        row = run_caps(["--caps2"])
        assert row["magic"] == "0x49444d4132434150"
        assert row["version"] == 1 and row["role"] == 1 and row["features"] == 7
        assert row["observer_magic"] == "0x49444d414f425308"
        assert row["error"] == ""
        row = run_caps(["--caps2"], stub_mode="caps2_bad_magic", good=False)
        assert row["error"]
        row = run_caps(["--caps2"], stub_mode="caps2_enotty", good=False)
        assert row["error"]

        result = subprocess.run([str(binary), "--help"], text=True, capture_output=True,
                                check=True)
        assert "Usage:" in result.stdout
        checks += 1

    print(f"PASS: {checks} host research2 CLI cases (parsing/range/ABI/raw-map only; no hardware validation)")


if __name__ == "__main__":
    main()
