#!/usr/bin/env python3
"""Mac / Genesys 2 preparation and console. Python standard library only."""
import argparse
import datetime as dt
import fcntl
import glob
import hashlib
import json
import math
import os
import re
from pathlib import Path
import select
import shlex
import shutil
import signal
import stat
import subprocess
import sys
import tarfile
import tempfile
import termios
import time
import tty

ROOT = Path(__file__).resolve().parents[1]
SERVER = "/home/xvhaocheng/work"
REMOTE_BIT = "iommu-platform/out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit"
BIT = ROOT / "artifacts/migration-4dma/ariane_xilinx.bit"
SHA256 = "cd49d4aba0195d5176618fb7d03071154bcd5b347425b43f854945149cceed70"
BIT_BYTES = 11443722
REFERENCES = [
    "HANDOFF.md",
    "iommu-platform/docs/board-host-setup.md",
    "iommu-platform/docs/validation.md",
    "iommu-platform/docs/experiment-v1.md",
    "iommu-platform/tests/board-smoke.sh",
    "iommu-platform/out/experiment-v1/README.md",
    "iommu-platform/out/experiment-v1/delivery.json",
    "iommu-platform/out/experiment-v1/SHA256SUMS",
    "iommu-platform/out/experiment-v1/software-manifest.json",
    "iommu-platform/cva6/corev_apu/fpga/src/bootrom/src/main.c",
    "iommu-platform/cva6/corev_apu/fpga/src/bootrom/src/gpt.c",
    "iommu-platform/cva6/corev_apu/fpga/ariane.cfg",
]


def utc():
    return dt.datetime.now(dt.timezone.utc).isoformat()


def digest(path):
    h = hashlib.sha256()
    with open(path, "rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def save_json(path, data):
    path.write_text(json.dumps(data, ensure_ascii=False, indent=2) + "\n")


def prepare():
    for folder in ("artifacts/migration-4dma", "artifacts/software", "references/dell",
                   "results/preflight", "results/jtag", "results/serial", "results/experiments"):
        (ROOT / folder).mkdir(parents=True, exist_ok=True)


def run_dir(category, label):
    prepare()
    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    return Path(tempfile.mkdtemp(prefix=f"{stamp}-{label}-", dir=ROOT / "results" / category))


def verify_bit(path=BIT):
    if not path.is_file():
        raise RuntimeError("缺少 BIT，请先运行 fetch。")
    actual = digest(path)
    if path.stat().st_size != BIT_BYTES or actual != SHA256:
        raise RuntimeError(f"BIT 校验失败，已停止。文件：{path}；实际 SHA256：{actual}")
    return {"path": str(path), "bytes": BIT_BYTES, "sha256": actual, "profile": "migration-4dma", "ds_cycles": [50, 50]}


def ports():
    return sorted(glob.glob("/dev/cu.*"))


def check(_args):
    out = run_dir("preflight", "check")
    result = {"created_utc": utc(), "serial_ports": ports(), "checks": {}, "hardware_tested": False}
    ok = sys.platform == "darwin"
    commands = {"system": ["uname", "-sm"], "macos": ["sw_vers"],
                "loader_version": ["openFPGALoader", "-V"],
                "boards": ["openFPGALoader", "--list-boards"]}
    for name, command in commands.items():
        try:
            p = subprocess.run(command, capture_output=True, text=True, timeout=20)
            result["checks"][name] = {"exit_code": p.returncode, "output": p.stdout + p.stderr}
            ok = ok and p.returncode == 0
            if name == "boards":
                supported = any(line.split()[:1] == ["genesys2"] for line in p.stdout.splitlines())
                ok = ok and supported
                result["genesys2_supported"] = supported
        except (OSError, subprocess.TimeoutExpired) as error:
            result["checks"][name] = {"error": str(error)}
            ok = False
    try:
        result["bit"] = verify_bit()
    except RuntimeError as error:
        result["bit"] = {"error": str(error)}
        ok = False
    result["passed"] = ok
    save_json(out / "check.json", result)
    print(f"本地准备检查：{'通过' if ok else '未通过'}\n记录：{out / 'check.json'}")
    print("串口候选：" + (", ".join(ports()) or "无"))
    print("此检查不访问 JTAG；板卡连接及 Linux 启动仍需实测。")
    return 0 if ok else 1


def fetch(args):
    prepare()
    # Every download is isolated; failed/incomplete transfers remain available for diagnosis.
    staging = Path(tempfile.mkdtemp(prefix="incoming-", dir=ROOT / "references/dell"))
    expected = [REMOTE_BIT] + REFERENCES
    command = " ".join(shlex.quote(x) for x in ["tar", "-C", SERVER, "-cf", "-", "--"] + expected)
    received = set()
    manifest = {"started_utc": utc(), "host": args.host, "server_root": SERVER,
                "status": "incomplete", "files": [],
                "software_status": "This command fetches hardware and reference metadata only; use fetch_software.py for software binaries."}
    save_json(staging / "download.json", manifest)
    with (staging / "transfer.stderr.log").open("wb") as errors:
        process = subprocess.Popen(["ssh", "-o", "BatchMode=yes", "-o", "ConnectTimeout=15", args.host, command],
                                   stdout=subprocess.PIPE, stderr=errors)
        try:
            with tarfile.open(fileobj=process.stdout, mode="r|") as archive:
                for member in archive:
                    if member.name not in expected or member.name in received or not member.isfile():
                        raise RuntimeError(f"拒绝意外归档条目：{member.name}")
                    if member.size > 32 * 1024 * 1024:
                        raise RuntimeError(f"归档条目过大：{member.name}")
                    target = staging / member.name
                    target.parent.mkdir(parents=True, exist_ok=True)
                    with archive.extractfile(member) as source, target.open("xb") as destination:
                        shutil.copyfileobj(source, destination)
                    received.add(member.name)
            if process.wait() != 0:
                raise RuntimeError(f"SSH 下载失败，查看 {staging / 'transfer.stderr.log'}")
        finally:
            process.stdout.close()
            if process.poll() is None:
                process.terminate()
                process.wait()
    if received != set(expected):
        raise RuntimeError(f"下载缺少文件：{set(expected) - received}")
    candidate = staging / REMOTE_BIT
    verify_bit(candidate)
    for name in expected:
        file = staging / name
        manifest["files"].append({"source": SERVER + "/" + name, "relative_path": name,
                                  "bytes": file.stat().st_size, "sha256": digest(file)})
    if BIT.exists():
        verify_bit()  # Never overwrite a different local artifact.
    else:
        # Hard link is atomic and refuses an existing destination; snapshot retains provenance.
        os.link(candidate, BIT)
    manifest.update(status="verified", finished_utc=utc(), local_bit=str(BIT))
    save_json(staging / "download.json", manifest)
    print(f"BIT 已下载并通过固定 SHA256 校验：{BIT}\n资料和下载清单：{staging}")
    print("此命令仅获取 BIT 和资料；软件下载入口：python3 scripts/fetch_software.py。")
    return 0


def configuration_status(output):
    values = re.findall(r"Register raw value:\s*(0x[0-9a-fA-F]+)", output)
    if not values:
        return None
    value = int(values[-1], 16)
    return {"stat": hex(value), "done": bool(value & (1 << 14)),
            "eos": bool(value & (1 << 4)), "gwe": bool(value & (1 << 6)),
            "crc_error": bool(value & 1), "id_error": bool(value & (1 << 15)),
            "startup_state": (value >> 18) & 7}


def verify_profile(profile="migration-4dma"):
    if profile == "migration-4dma-p4m":
        from fetch_p4m import FOLDER, check_metadata
        delivery = check_metadata()
        return {"path": str(FOLDER / "ariane_xilinx.bit"), "bytes": 11443722,
                "sha256": None, "hash_verified": False,
                "expected_sha256": delivery["artifacts"]["ariane_xilinx.bit"]["sha256"],
                "profile": profile, "ds_cycles": [50, 50], "cva6_commit": delivery["versions"]["cva6"]}
    if profile == "migration-4dma-e3-queue":
        from fetch_e3_queue import FOLDER, check_metadata
        delivery = check_metadata()
        return {"path": str(FOLDER / "ariane_xilinx.bit"), "bytes": 11443722,
                "sha256": None, "hash_verified": False,
                "expected_sha256": delivery["bit_sha256"],
                "profile": profile, "ds_cycles": [50, 50], "cva6_commit": delivery["versions"]["cva6"]}
    if profile == "migration-4dma-e3-controlled":
        from fetch_e3_controlled import FOLDER, check_metadata
        delivery = check_metadata()
        return {"path": str(FOLDER / "ariane_xilinx.bit"), "bytes": 11443722,
                "sha256": None, "hash_verified": False,
                "expected_sha256": delivery["hashes"]["ariane_xilinx.bit"],
                "profile": profile, "ds_cycles": [50, 50], "cva6_commit": delivery["versions"]["cva6"]}
    if profile == "migration-4dma-e3":
        from fetch_e3_service import FOLDER, check_metadata
        delivery = check_metadata()
        return {"path": str(FOLDER / "ariane_xilinx.bit"), "bytes": 11443722,
                "sha256": None, "hash_verified": False,
                "expected_sha256": delivery["artifacts"]["ariane_xilinx.bit"]["sha256"],
                "profile": profile, "ds_cycles": [50, 50], "cva6_commit": delivery["current_commits"]["cva6"]}
    if profile == "migration-4dma-path":
        from fetch_e4_path import FOLDER, check_metadata
        delivery = check_metadata()
        return {"path": str(FOLDER / "ariane_xilinx.bit"), "bytes": 11443722,
                "sha256": None, "hash_verified": False,
                "expected_sha256": delivery["files"]["ariane_xilinx.bit"]["sha256"],
                "profile": profile, "ds_cycles": [50, 50], "cva6_commit": delivery["cva6_commit"]}
    if profile == "baseline-4dma-batch":
        from fetch_ds0_batch import check_metadata
        return check_metadata()
    if profile == "migration-4dma-batch":
        from fetch_e4_batch import FOLDER, check_metadata
        delivery = check_metadata()
        return {"path": str(FOLDER / "ariane_xilinx.bit"), "bytes": 11443722,
                "sha256": None, "hash_verified": False,
                "expected_sha256": delivery["artifacts"]["ariane_xilinx.bit"]["sha256"],
                "profile": profile, "ds_cycles": [50, 50], "cva6_commit": delivery["cva6_commit"]}
    if profile == "migration-4dma-observe":
        from fetch_e4_observer import FOLDER, check_metadata
        delivery = check_metadata()
        return {"path": str(FOLDER / "ariane_xilinx.bit"), "bytes": 11443722,
                "sha256": None, "hash_verified": False,
                "expected_sha256": delivery["artifacts"]["ariane_xilinx.bit"]["sha256"],
                "profile": profile, "ds_cycles": [50, 50], "cva6_commit": delivery["cva6_commit"]}
    if profile == "migration-4dma-scfix":
        from fetch_scfix import verify_local
        return verify_local()
    if profile != "migration-4dma":
        raise RuntimeError("未知 BIT 配置。")
    return verify_bit()


def jtag(args):
    info = None
    selected_bit = BIT
    if args.action == "program":
        info = verify_profile(getattr(args, "profile", "migration-4dma"))
        selected_bit = Path(info.get("path", BIT))
    command = ["openFPGALoader", "-b", "genesys2"]
    if getattr(args, "freq", None):
        command += ["--freq", str(args.freq)]
    command += [str(selected_bit)] if args.action == "program" else (["--read-register", "STAT"] if args.action == "status" else ["--detect"])
    if args.dry_run:
        print("仅预览：" + shlex.join(command))
        return 0
    out = run_dir("jtag", args.action)
    status = {"started_utc": utc(), "command": command, "bit": info, "status": "running",
              "operation": args.action, "linux_boot_validated": False}
    save_json(out / "status.json", status)
    print(f"日志：{out}", flush=True)
    captured = bytearray()
    try:
        with (out / "output.log").open("wb") as log:
            p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            try:
                for block in iter(lambda: p.stdout.read1(4096), b""):
                    captured.extend(block)
                    log.write(block)
                    log.flush()
                    sys.stdout.buffer.write(block)
                    sys.stdout.buffer.flush()
                code = p.wait()
            finally:
                p.stdout.close()
                if p.poll() is None:
                    p.terminate()
                    p.wait()
        status.update(status="command_succeeded" if code == 0 else "failed", exit_code=code)
        if args.action in ("program", "status"):
            decoded = configuration_status(captured.decode(errors="replace"))
            status["configuration"] = decoded
            if decoded is not None and code == 0:
                complete = decoded["done"] and decoded["eos"] and decoded["gwe"] and not decoded["crc_error"] and not decoded["id_error"]
                status["configuration_complete"] = complete
                status["tool_exit_code"] = code
                if not complete:
                    status.update(status="startup_incomplete", exit_code=2)
                    code = 2
            elif args.action == "status" and code == 0:
                status.update(status="status_unavailable", tool_exit_code=code, exit_code=2)
                code = 2
    except BaseException as error:
        status.update(status="failed_or_interrupted", error=str(error))
        raise
    finally:
        status["finished_utc"] = utc()
        save_json(out / "status.json", status)
    return code


def configure_serial(fd):
    attrs = termios.tcgetattr(fd)
    attrs[0] = 0  # raw input, no XON/XOFF
    attrs[1] = 0
    attrs[2] = termios.CS8 | termios.CREAD | termios.CLOCAL  # 8N1, no RTS/CTS
    attrs[3] = 0
    attrs[4] = attrs[5] = termios.B115200
    attrs[6][termios.VMIN] = 1
    attrs[6][termios.VTIME] = 0
    termios.tcsetattr(fd, termios.TCSANOW, attrs)


def console(args):
    if args.capture_seconds is None and not sys.stdin.isatty():
        raise RuntimeError("交互串口需要终端；仅采集可使用 --capture-seconds 秒数。")
    device = Path(args.port)
    if not device.exists() or not stat.S_ISCHR(device.stat().st_mode):
        raise RuntimeError("请选择实际串口字符设备；先运行 ports 查看候选。")
    out = run_dir("serial", "console")
    metadata = {"started_utc": utc(), "port": str(device), "baud": 115200,
                "format": "8N1", "flow_control": "none", "status": "opening", "received_bytes": 0}
    save_json(out / "session.json", metadata)
    print(f"串口日志：{out / 'console.raw.log'}\n退出：Ctrl+]。请在采集开始后复位/启动板卡。", flush=True)
    fd = None
    original_serial = original_stdin = None
    try:
        fd = os.open(device, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
        fcntl.ioctl(fd, termios.TIOCEXCL)
        original_serial = termios.tcgetattr(fd)
        configure_serial(fd)
        # Blocking writes avoid silently dropping partially transmitted input.
        os.set_blocking(fd, True)
        interactive = args.capture_seconds is None
        if interactive:
            original_stdin = termios.tcgetattr(sys.stdin.fileno())
            tty.setraw(sys.stdin.fileno())
        deadline = time.monotonic() + args.capture_seconds if not interactive else None
        metadata["status"] = "capturing"
        save_json(out / "session.json", metadata)
        with (out / "console.raw.log").open("xb", buffering=0) as log:
            while True:
                remaining = max(0, deadline - time.monotonic()) if deadline else 0.5
                if deadline and remaining == 0:
                    break
                ready, _, _ = select.select([fd, sys.stdin.fileno()] if interactive else [fd], [], [], min(remaining, 0.5))
                if fd in ready:
                    block = os.read(fd, 65536)
                    if not block:
                        raise RuntimeError("串口连接已关闭。")
                    log.write(block)
                    metadata["received_bytes"] += len(block)
                    sys.stdout.buffer.write(block)
                    sys.stdout.buffer.flush()
                if interactive and sys.stdin.fileno() in ready:
                    block = os.read(sys.stdin.fileno(), 4096)
                    if not block:
                        break
                    leave = b"\x1d" in block
                    block = block.split(b"\x1d", 1)[0]
                    while block:
                        block = block[os.write(fd, block):]
                    if leave:
                        break
        metadata["status"] = "capture_finished"  # Does not assert Linux boot success.
    except BaseException as error:
        metadata.update(status="failed_or_interrupted", error=str(error))
        raise
    finally:
        if original_stdin is not None:
            termios.tcsetattr(sys.stdin.fileno(), termios.TCSANOW, original_stdin)
        if fd is not None:
            try:
                if original_serial is not None:
                    termios.tcsetattr(fd, termios.TCSANOW, original_serial)
            except termios.error:
                pass  # A physically disconnected USB port cannot be restored.
            os.close(fd)
        metadata["finished_utc"] = utc()
        save_json(out / "session.json", metadata)
        print(f"\n采集结束，记录：{out}")
    return 0


def new_run(args):
    info = verify_profile(getattr(args, "profile", "migration-4dma"))
    out = run_dir("experiments", "run")
    save_json(out / "run.json", {"created_utc": utc(), "label": args.label, "bit": info,
              "software_sha256": None, "serial_log": None, "smoke_passed": None,
              "linux_boot_passed": None, "hpm_calibrated": None, "status": "not_started"})
    (out / "notes.md").write_text("# 实验记录\n\n填写板卡/SD 镜像版本、串口日志路径、命令、结果和异常。\n")
    print(out)
    return 0


def positive_seconds(value):
    number = float(value)
    if not math.isfinite(number) or number <= 0:
        raise argparse.ArgumentTypeError("秒数必须是有限正数")
    return number


def main():
    parser = argparse.ArgumentParser(description="Genesys 2 Mac 准备工具；结果自动存入工作文件夹。")
    sub = parser.add_subparsers(dest="action", required=True)
    sub.add_parser("check", help="检查本地工具与 BIT；不访问 JTAG").set_defaults(func=check)
    p = sub.add_parser("fetch", help="从 dell 获取固定 BIT 和资料快照")
    p.add_argument("--host", default="dell", choices=["dell"])
    p.set_defaults(func=fetch)
    sub.add_parser("ports", help="列出串口候选").set_defaults(func=lambda _: print("\n".join(ports()) or "未发现串口候选") or 0)
    for action in ("detect", "program", "status"):
        p = sub.add_parser(action, help={"detect": "JTAG 识别", "program": "校验后加载易失 BIT", "status": "只读配置状态，检查 DONE/EOS/GWE"}[action])
        p.add_argument("--dry-run", action="store_true", help="仅预览命令，不连接板卡")
        if action == "program":
            p.add_argument("--profile", choices=["migration-4dma", "migration-4dma-scfix", "migration-4dma-observe", "migration-4dma-batch", "baseline-4dma-batch", "migration-4dma-path", "migration-4dma-e3", "migration-4dma-e3-queue", "migration-4dma-e3-controlled", "migration-4dma-p4m"],
                           default="migration-4dma", help="默认仍为历史 BIT；修复版须显式选择 scfix")
        p.add_argument("--freq", type=int, choices=[1000000, 6000000], default=1000000,
                       help="JTAG 频率 Hz；默认 1 MHz（本机首次上板验证通过）")
        p.set_defaults(func=jtag)
    p = sub.add_parser("console", help="115200 8N1 串口控制台，自动保存原始接收日志")
    p.add_argument("port", help="ports 列出的实际 /dev/cu.* 路径")
    p.add_argument("--capture-seconds", type=positive_seconds, help="限时只接收，不读取键盘输入")
    p.set_defaults(func=console)
    p = sub.add_parser("new-run", help="为一次实验创建记录目录，不执行实验")
    p.add_argument("--label", default="first-board")
    p.add_argument("--profile", choices=["migration-4dma", "migration-4dma-scfix", "migration-4dma-observe", "migration-4dma-batch", "baseline-4dma-batch", "migration-4dma-path", "migration-4dma-e3", "migration-4dma-e3-queue", "migration-4dma-e3-controlled", "migration-4dma-p4m"], default="migration-4dma")
    p.set_defaults(func=new_run)
    args = parser.parse_args()
    # Ensure SIGTERM follows the same cleanup path as a keyboard interrupt.
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(143))
    try:
        return args.func(args)
    except (OSError, RuntimeError, tarfile.TarError, termios.error) as error:
        print(f"错误：{error}", file=sys.stderr)
        return 1
    except KeyboardInterrupt:
        print("\n已中断。", file=sys.stderr)
        return 130


if __name__ == "__main__":
    sys.exit(main())
