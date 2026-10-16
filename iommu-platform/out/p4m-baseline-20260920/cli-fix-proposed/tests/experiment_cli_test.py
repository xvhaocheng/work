#!/usr/bin/env python3
"""Compile the CLI on the host and verify parsing/protocol/error propagation.

The LD_PRELOAD stub returns invented counters. These tests establish no DMA,
HPM, concurrency, calibration, timing, or FPGA performance result.
"""

import errno
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]


def main():
    checks = 0
    with tempfile.TemporaryDirectory(prefix="idma-cli-test-") as temporary:
        temp = Path(temporary)
        include = temp / "uapi"
        (include / "linux").mkdir(parents=True)
        # Only the project-specific header; standard exported headers come from
        # the host sysroot. Raw kernel include/uapi is not userspace-installable.
        shutil.copy2(ROOT / "linux/include/uapi/linux/idma.h", include / "linux/idma.h")
        compiler = os.environ.get("HOSTCC", "cc")
        flags = [compiler, "-std=c11", "-Wall", "-Wextra", "-Werror", "-O2", "-I", str(include)]
        binary = temp / "idma_experiment"
        stub = temp / "ioctl_stub.so"
        subprocess.run(flags + [str(ROOT / "bench/idma_experiment.c"), "-o", str(binary)], check=True)
        subprocess.run(flags + ["-shared", "-fPIC", str(ROOT / "tests/experiment_cli_test_stub.c"), "-o", str(stub)], check=True)
        device = temp / 'fake "device"\n测试'
        device.touch()
        log = temp / "calls.log"

        def run(args=(), mode="ok", good=True, calls=("caps", "run"), use_device=True):
            nonlocal checks
            log.unlink(missing_ok=True)
            env = os.environ.copy()
            env.update(LD_PRELOAD=str(stub), IDMA_TEST_MODE=mode, IDMA_TEST_LOG=str(log))
            cmd = [str(binary)] + (["--device", str(device)] if use_device else []) + list(args)
            result = subprocess.run(cmd, env=env, text=True, capture_output=True, check=False)
            assert (result.returncode == 0) == good, (mode, args, result)
            row = json.loads(result.stdout)
            assert len(result.stdout.splitlines()) == 1, result.stdout
            assert row["success"] == good, row
            actual_calls = log.read_text().splitlines() if log.exists() else []
            assert actual_calls == list(calls), (mode, args, actual_calls)
            assert row["argv"] == cmd, row
            if use_device:
                assert row["input"]["device"] == str(device)
            assert row["semantics"]["calibration"] == "unverified"
            assert "slowdown" not in row and "slowdown" not in (row["result"] or {})
            checks += 1
            return row

        row = run(["--batch"], calls=("caps", "batch"))
        assert row["schema"] == "idma.batch.v1"
        assert row["result"]["victim"]["refill_gap_ns_sum"] is None
        run(["--batch", "--chunk", "256"], good=False, calls=())
        run(["--batch", "--transfers", "1048577"], good=False, calls=())
        run(["--batch"], mode="caps_no_batch", good=False, calls=("caps",))
        row = run()
        assert row["input"]["victim_transfers"] == 4096
        assert row["input"]["aggressor_id"] is None
        assert row["result"]["aggressor"] is None
        assert row["result"]["victim"]["iotlb_miss"] == 3
        assert row["result"]["victim_elapsed_ns"] == 1234567

        row = run(["--aggressor", "1", "--victim-pages", "2", "--aggressor-pages", "32",
                   "--chunk", "256", "--transfers", "100", "--warmup", "0", "--timeout-ms", "1000"])
        assert row["input"]["warmup_transfers"] == 0
        assert row["result"]["victim"]["completed_bytes"] == 25600
        assert row["result"]["aggressor"]["iotlb_miss"] == 13
        assert row["result"]["aggressor"]["in_flight_at_end"] == 1

        row = run(["--caps"], calls=("caps",))
        assert row["result"] is None and not row["run_attempted"]
        row = run(["--caps"], mode="caps_no_hpm", calls=("caps",))
        assert not row["capabilities"]["did_hpm_supported"]

        for args in (["--unknown"], ["--transfers"], ["--chunk", "4096", "--chunk", "256"],
                     ["--aggressor", "4294967295"], ["--transfers", "4294967297"],
                     ["--warmup", "4294967297"], ["--transfers", "18446744073709551616"],
                     ["--chunk", "4294967296"], ["--victim-pages", "0"], ["--transfers", "0"],
                     ["--timeout-ms", "0"], ["--chunk", "0"], ["--aggressor-pages", "0"],
                     ["--warmup", "-1"], ["--transfers", "+1"], ["--chunk", "1k"],
                     ["--chunk", "0x1000"], ["--chunk", " 4096"], ["--chunk", ""],
                     ["--chunk", "2.5"], ["4096"], ["--help", "--unknown"],
                     ["--help", "--help"]):
            row = run(args, good=False, calls=())
            assert row["stage"] == "arguments" and row["result"] is None

        for args in (["--victim-pages", "4097"], ["--aggressor-pages", "4097"],
                     ["--chunk", "8192"], ["--chunk", "3"], ["--aggressor", "0"],
                     ["--timeout-ms", "300001"]):
            run(args, good=False, calls=("caps",))

        for mode in ("caps_enotty", "caps_abi", "caps_size", "caps_reserved", "caps_page",
                     "caps_limit", "caps_no_hpm", "caps_no_refill", "caps_positive"):
            run(mode=mode, good=False, calls=("caps",))

        for mode in ("run_status", "run_data", "run_aggressor_data", "run_quarantine",
                     "run_aggressor_quarantine", "run_hpm", "run_abi", "run_size", "run_count",
                     "run_submitted", "run_bytes", "run_did", "run_elapsed", "run_started",
                     "run_incomplete", "run_positive"):
            run(["--aggressor", "1"], mode=mode, good=False)

        def close(actual, expected, rel=1e-9):
            return abs(actual - expected) <= rel * abs(expected) + 1e-6

        row = run(["--p4m"], calls=("caps", "p4m"))
        assert row["schema"] == "idma.p4m.v1"
        assert row["p4m"]["conservation"] is True
        assert len(row["p4m"]["raw_window"]) == 16
        assert len(row["p4m"]["raw_observation"]) == 35
        assert row["p4m"]["observer_status"] == 4
        assert row["p4m"]["sequence"] == 1
        assert row["p4m"]["hardware_window_ns"] == 20000
        assert row["p4m"]["a_progress_window_bytes"] == 0
        assert row["p4m"]["a_started_before_window_completed"] == 0
        assert row["p4m"]["a_started_before_window_bytes"] == 0
        assert row["p4m"]["a_completed_bytes_per_second"] == 0.0
        assert row["p4m"]["a_completed_jobs_per_second"] == 0.0
        assert row["p4m"]["has_a"] is False
        assert row["p4m"]["closed"] is True
        assert row["p4m"]["flags"] & 0x4 == 0
        assert "a_done_before_open" not in row["p4m"]
        assert "a_completed_jobs_per_ns" not in row["p4m"]

        row = run(["--p4m", "--batch"], calls=("caps", "p4m"))
        assert row["schema"] == "idma.p4m.v1"

        row = run(["--p4m", "--aggressor", "1"], calls=("caps", "p4m"))
        assert row["p4m"]["has_a"] is True
        assert row["p4m"]["conservation"] is True
        assert row["p4m"]["a_progress_window_bytes"] == 10 * 4096
        assert row["p4m"]["a_started_before_window_completed"] == 1
        assert row["p4m"]["a_started_before_window_bytes"] == 4096
        assert row["p4m"]["a_completed_bytes_per_second"] == 10 * 4096 * 50000000 / 1000
        assert row["p4m"]["a_completed_jobs_per_second"] == 10 * 50000000 / 1000

        row = run(["--p4m", "--aggressor", "1", "--a-gap-cycles", "1000"],
                  calls=("caps", "p4m"))
        assert row["input"]["a_gap_cycles"] == 1000
        assert row["p4m"]["requested_gap_cycles"] == 1000

        expected_bytes = 10 * 4096 * 50000000 / 3001
        expected_jobs = 10 * 50000000 / 3001
        assert expected_bytes != int(expected_bytes) and expected_jobs != int(expected_jobs)
        row = run(["--p4m", "--aggressor", "1"], mode="p4m_odd_cycles",
                  calls=("caps", "p4m"))
        assert close(row["p4m"]["a_completed_bytes_per_second"], expected_bytes)
        assert close(row["p4m"]["a_completed_jobs_per_second"], expected_jobs)
        assert row["p4m"]["hardware_window_ns"] == 3001 * 20

        for args in (["--a-gap-cycles", "0"], ["--a-gap-cycles", "1000"],
                     ["--p4m", "--aggressor", "none", "--a-gap-cycles", "1"],
                     ["--p4m", "--a-gap-cycles", "4294967296"],
                     ["--p4m", "--chunk", "256"], ["--p4m", "--transfers", "1048577"],
                     ["--p4m", "--p4m"]):
            row = run(args, good=False, calls=())
            assert row["stage"] == "arguments" and row["result"] is None

        run(["--p4m"], mode="caps_no_p4m", good=False, calls=("caps",))
        run(["--p4m"], mode="caps_no_batch", good=False, calls=("caps",))

        for mode in ("p4m_bank_magic", "p4m_bank_closed", "p4m_cycles", "p4m_cycles_mismatch",
                     "p4m_conservation", "p4m_flags", "p4m_ids", "p4m_vcount",
                     "p4m_legacy_vcount", "p4m_status", "p4m_observer_status", "p4m_sequence",
                     "p4m_strict", "p4m_carry", "p4m_contiguous", "p4m_endid"):
            row = run(["--p4m", "--aggressor", "1"], mode=mode, good=False,
                      calls=("caps", "p4m"))
            assert row["success"] is False
            assert row["p4m"] is not None
            assert row["errno"] == 0
            # Raw bank is retained; every derived rate/time/progress is null.
            assert len(row["p4m"]["raw_window"]) == 16
            assert len(row["p4m"]["raw_observation"]) == 35
            assert row["p4m"]["hardware_window_ns"] is None
            assert row["p4m"]["a_progress_window_bytes"] is None
            assert row["p4m"]["a_started_before_window_bytes"] is None
            assert row["p4m"]["a_completed_bytes_per_second"] is None
            assert row["p4m"]["a_completed_jobs_per_second"] is None
        row = run(["--p4m"], mode="p4m_solo_nonzero", good=False, calls=("caps", "p4m"))
        assert row["success"] is False and row["p4m"] is not None
        row = run(["--p4m", "--aggressor", "1"], mode="p4m_nohasA", good=False,
                  calls=("caps", "p4m"))
        assert row["success"] is False and row["p4m"] is not None
        row = run(["--p4m"], mode="run_enotty", good=False, calls=("caps", "p4m"))
        assert row["errno"] == errno.ENOTTY
        assert row["result"] is None
        assert row["p4m"]["a_completed_bytes_per_second"] is None
        assert row["p4m"]["hardware_window_ns"] is None

        for mode in ("run_enotty", "run_eintr"):
            row = run(mode=mode, good=False)
            assert row["result"] is None
        row = run(mode="run_timeout", good=False)
        assert row["errno"] == errno.ETIMEDOUT
        assert row["result"]["measurement_complete"] == 0
        assert row["result"]["victim"]["completed"] == 7
        row = run(mode="run_warmup_timeout", good=False)
        assert row["result"]["measurement_started"] == 0
        assert row["result"]["victim_elapsed_ns"] is None
        assert row["result"]["victim"]["completed"] is None
        assert row["result"]["victim"]["iotlb_miss"] is None
        assert row["result"]["victim"]["data_errors"] == 0

        row = run(["--transfers", "4294967296", "--warmup", "4294967296"])
        assert row["result"]["victim"]["completed_bytes"] == 4294967296 * 4096
        row = run(["--device", str(temp / "absent")], use_device=False, good=False, calls=())
        assert row["stage"] == "open" and row["errno"] == errno.ENOENT
        result = subprocess.run([str(binary), "--help"], text=True, capture_output=True, check=True)
        assert "Usage:" in result.stdout
        checks += 1
    print(f"PASS: {checks} host CLI cases (parsing/ABI/error propagation only; no hardware validation)")


if __name__ == "__main__":
    main()
