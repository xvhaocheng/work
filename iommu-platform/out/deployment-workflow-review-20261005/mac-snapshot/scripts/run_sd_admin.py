#!/usr/bin/env python3
"""Stage reviewed inputs outside Desktop for macOS administrator execution."""
import argparse
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys
import tempfile

from fpga import ROOT, save_json, utc
from sd_card import SOFTWARE


def main():
    parser = argparse.ArgumentParser(description="通过系统管理员认证，备份并更新已登记的 SD 启动分区。")
    parser.add_argument("--disk", required=True)
    args = parser.parse_args()
    if not re.fullmatch(r"/dev/disk[0-9]+", args.disk):
        parser.error("磁盘路径格式错误")
    # All metadata and payload checks run before the system password dialog.
    subprocess.run([sys.executable, str(ROOT / "scripts/sd_card.py"), "update",
                    "--disk", args.disk, "--dry-run"], check=True)
    stage = Path(tempfile.mkdtemp(prefix="genesys-sd-", dir="/private/tmp"))
    (stage / "scripts").mkdir()
    for name in ("fpga.py", "fetch_software.py", "sd_card.py"):
        shutil.copy2(ROOT / "scripts" / name, stage / "scripts" / name)
    shutil.copytree(SOFTWARE, stage / SOFTWARE.relative_to(ROOT))
    (stage / "results").mkdir()
    logbase = ROOT / "results/sd-admin"
    logbase.mkdir(parents=True, exist_ok=True)
    logdir = Path(tempfile.mkdtemp(prefix="launch-", dir=logbase))
    record = {"started_utc": utc(), "staging_directory": str(stage), "disk": args.disk,
              "status": "awaiting_administrator", "copied_result_directories": []}
    save_json(logdir / "launch.json", record)
    command = shlex.join([sys.executable, str(stage / "scripts/sd_card.py"), "update", "--disk", args.disk])
    # Only the freshly created private temporary results tree changes ownership.
    # This lets the ordinary user archive root-generated backups without giving root Desktop access.
    command += "; sd_update_exit=$?; " + shlex.join(["/usr/sbin/chown", "-R", f"{os.getuid()}:{os.getgid()}", str(stage / "results")])
    command += '; exit "$sd_update_exit"'
    apple_literal = '"' + command.replace("\\", "\\\\").replace('"', '\\"') + '"'
    print(f"临时目录：{stage}\n授权及归档记录：{logdir}", flush=True)
    try:
        result = subprocess.run(["/usr/bin/osascript", "-"],
                                input="do shell script " + apple_literal + " with administrator privileges\n",
                                text=True, capture_output=True)
        (logdir / "administrator.stdout.log").write_text(result.stdout)
        (logdir / "administrator.stderr.log").write_text(result.stderr)
        record.update(exit_code=result.returncode, status="finished" if result.returncode == 0 else "failed_or_cancelled")
        print(result.stdout, end="")
        print(result.stderr, end="", file=sys.stderr)
        source = stage / "results/sd"
        target = ROOT / "results/sd"
        target.mkdir(parents=True, exist_ok=True)
        if source.exists():
            for folder in source.iterdir():
                destination = target / folder.name
                shutil.copytree(folder, destination)
                record["copied_result_directories"].append(str(destination))
                print(f"已归档备份和结果：{destination}")
        return result.returncode
    finally:
        record["finished_utc"] = utc()
        save_json(logdir / "launch.json", record)
        # Retain the exact staged inputs and results for diagnosing any interrupted update.


if __name__ == "__main__":
    sys.exit(main())
