#!/usr/bin/env python3
"""Host-side tests for bench/idma_research.c using an LD_PRELOAD ioctl stub.

These tests do not use hardware, root, /dev/mem, or a real driver. The stub
replaces the kernel UAPI ioctl responses; all validation logic under test is
the userspace CLI's own parsing, ABI/capability checks, raw-map checks and JSON
encoding.
"""

import errno
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


def close(actual, expected):
    assert actual is not None, (actual, expected)
    assert abs(actual - expected) < 1e-6, (actual, expected)
    return True


ROOT = Path(__file__).resolve().parents[1]


def main():
    checks = 0
    with tempfile.TemporaryDirectory(prefix="idma-research-test-") as temporary:
        temp = Path(temporary)
        include = temp / "uapi"
        (include / "linux").mkdir(parents=True)
        shutil.copy2(ROOT / "linux/include/uapi/linux/idma.h", include / "linux/idma.h")
        compiler = os.environ.get("HOSTCC", "cc")
        flags = [compiler, "-std=c11", "-Wall", "-Wextra", "-Werror", "-O2", "-I", str(include)]
        binary = temp / "idma_research"
        stub = temp / "research_ioctl_stub.so"
        subprocess.run(flags + [str(ROOT / "bench/idma_research.c"), "-o", str(binary)], check=True)
        subprocess.run(flags + ["-shared", "-fPIC", str(ROOT / "tests/research_cli_stub.c"), "-o", str(stub)], check=True)
        device = temp / 'fake "idma0"\n测试'
        device.touch()
        log = temp / "calls.log"

        def run(args=(), mode="ok", good=True, calls=("caps", "run"), use_device=True):
            nonlocal checks
            log.unlink(missing_ok=True)
            env = os.environ.copy()
            env.update(LD_PRELOAD=str(stub), IDMA_TEST_MODE=mode, IDMA_TEST_LOG=str(log))
            cmd = [str(binary)] + (["--device", str(device)] if use_device else []) + list(args)
            result = subprocess.run(cmd, env=env, text=True, capture_output=True, check=False)
            assert (result.returncode == 0) == good, (mode, args, result.returncode, result.stdout, result.stderr)
            row = json.loads(result.stdout)
            assert len(result.stdout.splitlines()) == 1, result.stdout
            assert row["schema"] == "idma-research-v1", row
            assert row["success"] == good, row
            actual_calls = log.read_text().splitlines() if log.exists() else []
            assert actual_calls == list(calls), (mode, args, actual_calls)
            if use_device:
                assert row["device"] == str(device), row
            checks += 1
            return row

        row = run(["--e2", "--a-pages", "0"])
        assert row["mode"] == "e2"
        assert row["validated"] is True
        assert row["status"] == 0
        assert row["flush_status"] == 0
        assert row["stage"] == 5
        assert row["stage_name"] == "complete"
        assert row["raw_captured"] == 4096
        assert len(row["raw"]) == 4096
        assert len(row["mapping"]) == 4
        assert row["input"]["a_pages"] == 0
        assert row["input"]["a_transfers"] == 0
        assert row["phase_stats"][0]["submitted"] == 2
        assert row["phase_stats"][1]["submitted"] == 0
        assert row["phase_stats"][2]["submitted"] == 1
        assert row["p4m"]["sequence"] == "0x0000000000000001"
        assert row["observer_magic"] == "0x49444d414f425308"

        row = run(["--e2", "--a-pages", "8"])
        assert row["input"]["a_pages"] == 8
        assert row["input"]["a_transfers"] == 112
        assert row["phase_stats"][1]["submitted"] == 112

        row = run(["--cost", "--a-pages", "0"])
        assert row["mode"] == "cost"
        assert row["validated"] is True
        assert row["flush_status"] == -errno.EINPROGRESS
        assert row["p4m"]["observer_status"] == "0x0000000000000004"
        # raw[0xc19]=1000000 cycle window; V done raw[0xc12]=4096.
        assert close(row["rates"]["v_mib_per_s"], 800.0)
        assert close(row["rates"]["a_common_mib_per_s"], 0.0)
        assert close(row["rates"]["a_common_jobs_per_s"], 0.0)

        row = run(["--cost", "--a-pages", "8"])
        assert row["input"]["a_pages"] == 8
        assert row["input"]["gap_cycles"] == 0
        # A same-window done is p4m.window[3]=1024 while whole-arm
        # raw[0xc04]=2000000, so only the common-window denominator matches.
        assert close(row["rates"]["v_mib_per_s"], 800.0)
        assert close(row["rates"]["a_common_mib_per_s"], 200.0)
        assert close(row["rates"]["a_common_jobs_per_s"], 51200.0)

        row = run(["--cost", "--a-pages", "8", "--a-gap-cycles", "1000"])
        assert row["input"]["a_pages"] == 8
        assert row["input"]["gap_cycles"] == 1000
        assert close(row["rates"]["a_common_mib_per_s"], 200.0)

        row = run(["--cost", "--a-pages", "8", "--a-gap-cycles", "2000", "--transfers", "64", "--warmup", "8"])
        assert row["input"]["transfers"] == 64
        assert row["input"]["warmup"] == 8
        assert close(row["rates"]["v_mib_per_s"], 12.5)
        assert close(row["rates"]["a_common_mib_per_s"], 200.0)

        def expect_error(error_args, error_mode, needle):
            bad_row = run(error_args, mode=error_mode, good=False, calls=("caps", "run"))
            assert bad_row["validated"] is True, (error_mode, bad_row)
            assert bad_row["success"] is False, (error_mode, bad_row)
            assert any(needle in message for message in bad_row["errors"]), (
                error_mode, needle, bad_row["errors"])
            return bad_row

        row = run(["--cost", "--a-pages", "8", "--require-fullstats"], mode="fullstats_good")
        assert row["success"] is True
        assert row["validated"] is True
        assert row["raw"][0xd00] == "0x49444d4141474731"
        assert row["input"]["a_pages"] == 8
        # The good record carries coherent A common-window counters: 0xc1b
        # bounds main, 0xc1d complete and 0xc1f/0xc23 the select/miss totals.
        assert int(row["raw"][0xc1b], 16) >= 16
        assert int(row["raw"][0xc1d], 16) >= 16
        assert int(row["raw"][0xc1f], 16) >= 64
        assert int(row["raw"][0xc23], 16) >= 24

        row = run(["--cost", "--a-pages", "0", "--require-fullstats"], mode="fullstats_good")
        assert row["success"] is True
        assert row["raw"][0xc14] == "0x0000000000000000"

        row = run(["--cost", "--a-pages", "8"], mode="fullstats_good")
        assert row["success"] is True

        # Old BIT without the fullstats magic: --require-fullstats must fail
        # with the targeted missing-magic message, not a generic error.
        row = run(["--cost", "--a-pages", "8", "--require-fullstats"], mode="ok", good=False, calls=("caps", "run"))
        assert row["validated"] is True
        assert any("fullstats magic missing" in message for message in row["errors"]), row["errors"]

        row = run(["--cost", "--a-pages", "8"], mode="fullstats_corrupt", good=False, calls=("caps", "run"))
        assert row["validated"] is True
        assert any("fullstats sticky error nonzero" in message for message in row["errors"]), row["errors"]

        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_corrupt", "fullstats sticky error nonzero")
        # A large-invalid record must fail on the fullstats wide-product mean
        # check, not on an unrelated cost-window check.
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_large", "fullstats group mean out of range")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_magic", "fullstats magic unknown")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_version", "fullstats version mismatch")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_groups", "fullstats group count mismatch")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_stride", "fullstats group stride mismatch")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_header_reserved", "fullstats header reserved nonzero")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_group_reserved", "fullstats group reserved nonzero")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_empty", "fullstats empty group nonzero")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_minmax", "fullstats group min/max order")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_total_main", "fullstats total main exceeds common window")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_total_complete", "fullstats total complete exceeds common window")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_total_req", "fullstats total requests exceed common window")
        expect_error(["--cost", "--a-pages", "8", "--require-fullstats"], "fullstats_total_miss", "fullstats total misses exceed common window")

        # Ordinary S1-only live-tag lookup and hit must agree, as enforced
        # by the actual observer. A contradictory record is not evidence.
        row = run(["--e2"], mode="fp_miss", good=False)
        assert row["success"] is False
        assert row["rates"]["v_mib_per_s"] is None

        # A first-probe miss is valid when self live/presence is empty and the
        # recorded target key is the lookup context with the forced valid bit.
        row = run(["--e2"], mode="fp_valid_miss")
        assert row["success"] is True
        assert row["validated"] is True
        assert row["raw"][0x15] == "0x0000000000000000"
        assert row["raw"][0x19] == "0x0000000000000000"
        assert int(row["raw"][0x16], 16) & (1 << 51)
        assert int(row["raw"][0x1a], 16) & (1 << 51)

        for args in (
            [],
            ["--unknown"],
            ["--e2", "--cost"],
            ["--cost", "--e2"],
            ["--e2", "--e2"],
            ["--cost", "--cost"],
            ["--e2", "--device"],
            ["--e2", "--a-pages", "3"],
            ["--e2", "--a-transfers", "1"],
            ["--e2", "--window-cycles", "1"],
            ["--e2", "--warmup", "1"],
            ["--e2", "--transfers", "2"],
            ["--e2", "--a-gap-cycles", "1"],
            ["--e2", "--timeout-ms", "1"],
            ["--cost", "--a-pages", "3"],
            ["--cost", "--a-transfers", "1"],
            ["--cost", "--window-cycles", "1"],
            ["--cost", "--a-gap-cycles", "1000", "--a-pages", "1"],
            ["--cost", "--transfers", "1"],
            ["--cost", "--warmup", "1"],
            ["--cost", "--timeout-ms", "1"],
            ["--e2", "--a-pages", "-1"],
            ["--e2", "--a-pages", "4294967296"],
            ["--e2", "--a-pages", "+1"],
            ["--e2", "--a-pages", "1k"],
            ["--e2", "--a-pages", ""],
            ["--help", "--unknown"],
            ["--e2", "--require-fullstats"],
            ["--cost", "--require-fullstats", "--require-fullstats"],
        ):
            row = run(args, good=False, calls=())
            assert row["validated"] is False, (args, row)
            assert row["errors"], (args, row)
            assert row["rates"]["v_mib_per_s"] is None, (args, row)

        for mode in (
            "caps_no_research",
            "caps_bad_abi",
            "caps_bad_size",
            "caps_bad_device",
            "caps_bad_did",
            "caps_bad_page",
            "caps_enotty",
        ):
            row = run(["--e2"], mode=mode, good=False, calls=("caps",))
            assert row["validated"] is False, (mode, row)
            assert row["errno"] == (errno.ENOTTY if mode == "caps_enotty" else 0), (mode, row)
            assert row["rates"]["v_mib_per_s"] is None, (mode, row)

        for mode, args in {
            "magic": ["--e2"],
            "context": ["--e2"],
            "timeout": ["--e2"],
            "count": ["--e2"],
            "rawmissing": ["--e2"],
            "overflow": ["--cost", "--a-pages", "8"],
            "p4m_window_mismatch": ["--cost", "--a-pages", "8"],
            "p4m_magic": ["--cost", "--a-pages", "8"],
        }.items():
            row = run(args, mode=mode, good=False, calls=("caps", "run"))
            assert row["validated"] is True, (mode, row)
            assert row["errors"], (mode, row)
            assert row["success"] is False, (mode, row)
            assert row["rates"]["v_mib_per_s"] is None, (mode, row)

        row = run(["--e2"], mode="errno_mismatch", good=False, calls=("caps", "run"))
        assert row["validated"] is True
        assert row["errno"] == errno.EPERM
        assert row["errors"], row

        row = run(["--e2"], mode="status", good=False, calls=("caps", "run"))
        assert row["errno"] == errno.EIO
        assert row["status"] == -errno.EIO
        assert row["errors"], row

        row = run(["--cost", "--a-pages", "0"], mode="flush", good=False, calls=("caps", "run"))
        assert row["errors"], row

        row = run(["--e2"], mode="stage", good=False, calls=("caps", "run"))
        assert row["errors"], row

        row = run(["--e2", "--device", str(temp / "absent")], use_device=False, good=False, calls=())
        assert row["stage_name"] == "open"
        assert row["errno"] == errno.ENOENT
        assert row["validated"] is False

        result = subprocess.run([str(binary), "--help"], text=True, capture_output=True, check=True)
        assert "Usage:" in result.stdout
        checks += 1

    print(f"PASS: {checks} host research CLI cases (parsing/ABI/raw-map only; no hardware validation)")


if __name__ == "__main__":
    main()
