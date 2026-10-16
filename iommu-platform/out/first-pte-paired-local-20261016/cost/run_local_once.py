#!/usr/bin/env python3
"""Run one explicitly selected, bounded OOC synthesis; no retries."""
from pathlib import Path
from datetime import datetime, timezone
import argparse
import hashlib
import json
import subprocess
import time

HERE = Path(__file__).resolve().parent


def now():
    return datetime.now(timezone.utc).isoformat()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("kind", choices=["o", "m"])
    parser.add_argument("policy", type=int, choices=range(4))
    parser.add_argument("cone", type=int, choices=range(2))
    parser.add_argument("output", help="new directory relative to cost/")
    args = parser.parse_args()
    out = (HERE / args.output).resolve()
    if HERE not in out.parents:
        raise SystemExit("Output must be below cost/")
    out.mkdir(parents=True, exist_ok=False)
    tool_argv = ["/tools/Xilinx/Vivado/2023.1/bin/vivado", "-mode", "batch", "-nojournal",
                 "-log", "vivado.log", "-source", str(HERE / "synth_local.tcl"),
                 "-tclargs", args.kind, str(args.policy), str(args.cone), str(out)]
    argv = ["timeout", "--signal=TERM", "--kill-after=10s", "600s"] + tool_argv
    record = {
        "kind":args.kind,"policy":args.policy,"cone":args.cone,
        "argv":argv,"tool_argv":tool_argv,"cwd":str(out),
        "timeout_seconds":600,"started_at":now(),"finished_at":None,"returncode":None,
        "tcl_sha256":hashlib.sha256((HERE / "synth_local.tcl").read_bytes()).hexdigest(),
        "source_map_sha256":hashlib.sha256((HERE / "source-map.json").read_bytes()).hexdigest(),
    }
    record_path = out / "invocation.json"
    record_path.write_text(json.dumps(record,indent=2)+"\n")
    started = time.monotonic()
    with (out / "console.log").open("wb") as log:
        result = subprocess.run(argv,cwd=out,stdout=log,stderr=subprocess.STDOUT)
    record.update(finished_at=now(),returncode=result.returncode,
                  elapsed_seconds=time.monotonic()-started)
    record_path.write_text(json.dumps(record,indent=2)+"\n")
    print(json.dumps(record,indent=2),flush=True)
    raise SystemExit(result.returncode)


if __name__ == "__main__":
    main()
