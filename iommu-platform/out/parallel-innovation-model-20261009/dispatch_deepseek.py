#!/usr/bin/env python3
"""Dispatch one independently scoped research evidence task; preserve raw output."""
import json
from pathlib import Path
import subprocess
import sys

project_root = Path(__file__).resolve().parents[2]
artifact_dir = Path(__file__).resolve().parent
direction = sys.argv[1]
task = json.loads((artifact_dir / "tasks.json").read_text())[direction]
argv = ["/home/xvhaocheng/.local/bin/deepseek", "--task", task["task"],
        "--root", str(project_root), "--files", *task["files"]]
with (artifact_dir / f"{direction}-deepseek-raw.json").open("w") as output:
    with (artifact_dir / f"{direction}-deepseek-stderr.log").open("w") as error:
        completed = subprocess.run(argv, stdout=output, stderr=error)
(artifact_dir / f"{direction}-dispatch-result.json").write_text(
    json.dumps({"direction": direction, "returncode": completed.returncode,
                "files": task["files"]}, ensure_ascii=False, indent=2) + "\n")
print(json.dumps({"direction": direction, "returncode": completed.returncode,
                  "raw_response": str(artifact_dir / f"{direction}-deepseek-raw.json")},
                 ensure_ascii=False))
sys.exit(completed.returncode)
