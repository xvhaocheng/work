#!/usr/bin/env python3
"""Compile E2 CLI and exercise it with wrapped syscalls, never hardware."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--linux", type=Path, required=True)
    args = parser.parse_args()
    root = Path(__file__).resolve().parent
    build = root / "multi-cli-build"
    include = build / "include" / "linux"
    include.mkdir(parents=True, exist_ok=True)
    headers = {}
    for name in ("idma.h", "idma_multi.h"):
        source = args.linux / "include/uapi/linux" / name
        shutil.copyfile(source, include / name)
        headers[str(source)] = digest(source)
    commands = []

    def command(argv, name):
        result = subprocess.run(argv, capture_output=True, text=True, timeout=60)
        log = root / name
        log.write_text(result.stdout + result.stderr)
        commands.append({"argv": list(map(str, argv)), "exit_code": result.returncode,
                         "log": name, "log_sha256": digest(log)})
        if result.returncode:
            raise RuntimeError(f"{argv[0]} failed: {log}")

    common = ["-std=gnu11", "-O2", "-Wall", "-Wextra", "-Werror", "-I", str(include.parent)]
    source = root / "idma_scaling.c"
    native = build / "idma_scaling-native"
    rv64 = build / "idma_scaling-rv64"
    mock = build / "idma_scaling-mock"
    command(["cc", *common, str(source), "-o", str(native)], "multi-cli-native-compile.log")
    command(["riscv64-linux-gnu-gcc", *common, "-static", str(source), "-o", str(rv64)],
            "multi-cli-rv64-compile.log")
    command(["cc", *common, "-g", "-fsanitize=undefined", "-fno-sanitize-recover=all",
             str(source), str(root / "multi_cli_wrap.c"), "-Wl,--wrap=open,--wrap=ioctl,--wrap=close",
             "-o", str(mock)], "multi-cli-mock-compile.log")
    cases = []

    def case(name, options, success, errno=0, calls="open=1 caps=1 run=1 close=1"):
        result = subprocess.run([str(mock), *options], capture_output=True, text=True,
                                env={**os.environ, "E2_CLI_CASE": name}, timeout=10)
        obj = json.loads(result.stdout)
        assert result.returncode == (0 if success else 1), (name, result)
        assert obj["success"] == success and obj["errno"] == errno, (name, obj)
        assert result.stderr == f"MOCK_SYSCALLS {calls}\n", (name, result.stderr)
        assert len(obj["result"]["stats"]) == 4
        assert len(obj["input"]["reserved"]) == 8
        assert len(obj["capabilities"]["reserved"]) == 7
        cases.append({"name": name, "argv": options, "exit_code": result.returncode,
                      "stderr": result.stderr, "output": obj})
        return obj

    for nr in (2, 3, 4):
        for pages in (1, 8, 6 // (nr - 1)):
            obj = case("success", ["--devices", str(nr), "--aggressor-pages", str(pages)], True)
            assert obj["input"] == obj["returned_input"]
            assert obj["input"]["nr_devices"] == nr and obj["input"]["pages"][1] == pages
    for invalid in ([], ["--devices", "1"], ["--devices", "5"], ["--devices", "-2"],
                    ["--devices", "2x"], ["--devices", ""], ["--devices"],
                    ["--devices", "2", "--devices", "2"], ["--caps", "--caps"],
                    ["--aggressor-pages", "0"], ["--aggressor-pages", "7"],
                    ["--devices", "3", "--aggressor-pages", "6"],
                    ["--timeout-ms", "0"], ["--timeout-ms", "300001"],
                    ["--timeout-ms", "999999999999999999999999999999"], ["--unknown"]):
        if not invalid:
            case("default", invalid, True)
        else:
            case("bad_options", invalid, False, 22, "open=0 caps=0 run=0 close=0")
    case("open_error", [], False, 2, "open=1 caps=0 run=0 close=0")
    case("caps_error", [], False, 25, "open=1 caps=1 run=0 close=1")
    for mode in ("caps_bad_abi", "caps_bad_reserved"):
        case(mode, [], False, 71, "open=1 caps=1 run=0 close=1")
    case("caps_no_path", [], False, 95, "open=1 caps=1 run=0 close=1")
    case("caps_no_path", ["--caps"], True, calls="open=1 caps=1 run=0 close=1")
    failed = case("run_error", [], False, 5)
    assert failed["result"]["status"] == -5 and failed["result"]["stats"][1]["quarantined"] == 1
    assert failed["result"]["drained_mask"] == 1
    for mode in ("bad_nr", "bad_bytes", "bad_mask", "bad_active", "bad_target", "bad_error",
                 "bad_end", "bad_start", "bad_launch_order", "bad_did", "bad_data",
                 "bad_quarantine", "bad_sequence"):
        obj = case(mode, [], False, 71)
        if mode == "bad_nr":
            assert obj["input"]["nr_devices"] == 2 and obj["returned_input"]["nr_devices"] == 99
    raw = root / "multi-cli-cases.json"
    raw.write_text(json.dumps(cases, indent=2) + "\n")
    receipt = {"schema": "ispass2027.e2_multi_cli.host.v1", "status": "passed",
               "scope": "Host syscall wrappers + native/RV64 static compile; no hardware invocation",
               "cases": len(cases), "commands": commands, "sources": {
                   str(source): digest(source), str(root / "multi_cli_wrap.c"): digest(root / "multi_cli_wrap.c"),
                   **headers}, "outputs": {str(p): {"sha256": digest(p), "bytes": p.stat().st_size}
                                           for p in (native, rv64, mock, raw)}}
    (root / "multi-cli-validation.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(f"PASS {len(cases)} CLI mock cases; native + RV64 static compiles; no real device opened")


if __name__ == "__main__":
    main()
