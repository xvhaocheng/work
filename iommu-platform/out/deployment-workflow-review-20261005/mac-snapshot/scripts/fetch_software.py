#!/usr/bin/env python3
"""Download the reviewed 2026-09-15 software release without overwriting prior copies."""
import json
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import tarfile
import tempfile

from fpga import ROOT, SHA256, digest, prepare, save_json, utc

REMOTE = "/home/xvhaocheng/work/iommu-platform/out/experiment-v1"
DELIVERY_SHA = "1204c6cc66b63495b3bd3436aeca5cd621b6760dffb6c0c8807d875c10cab282"
FILES = {"Image", "README.md", "SHA256SUMS", "build.log", "delivery.json",
         "fw_payload.bin", "idma_experiment", "linux-rv64-cva6.bin", "platform-smoke",
         "platform.dtb", "rootfs.cpio", "software-manifest.json", "sources.lock.json"}


def verify(folder):
    if digest(folder / "delivery.json") != DELIVERY_SHA:
        raise RuntimeError("交付版本与已核对的版本不同，请重新核对服务器版本。")
    checked = {}
    for line in (folder / "SHA256SUMS").read_text().splitlines():
        checksum, name = line.split(maxsplit=1)
        if name not in FILES - {"SHA256SUMS"} or name in checked:
            raise RuntimeError(f"非法或重复校验条目：{name}")
        if digest(folder / name) != checksum:
            raise RuntimeError(f"SHA256 校验失败：{name}")
        checked[name] = checksum
    if set(checked) != FILES - {"SHA256SUMS"}:
        raise RuntimeError("校验清单未覆盖全部交付文件。")
    delivery = json.loads((folder / "delivery.json").read_text())
    for name, info in delivery["software_artifacts"].items():
        if name not in checked or checked[name] != info["sha256"] or (folder / name).stat().st_size != info["bytes"]:
            raise RuntimeError(f"文件与固定交付清单不一致：{name}")
    hardware = delivery["unchanged_fpga_artifacts_outside_this_directory"]
    if not any(name.endswith(".bit") and info["sha256"] == SHA256 for name, info in hardware.items()):
        raise RuntimeError("软件交付引用的 BIT 与本地固定版本不一致。")
    return delivery, checked


def main():
    prepare()
    snapshot = Path(tempfile.mkdtemp(prefix="20260915T140327Z-", dir=ROOT / "artifacts/software"))
    folder = snapshot / "experiment-v1"
    folder.mkdir()
    record = {"started_utc": utc(), "host": "dell", "source": REMOTE, "status": "incomplete"}
    save_json(snapshot / "download.json", record)
    command = shlex.join(["tar", "-C", REMOTE, "-cf", "-", "--"] + sorted(FILES))
    received = set()
    print(f"下载目录：{folder}", flush=True)
    with (snapshot / "transfer.stderr.log").open("wb") as errors:
        process = subprocess.Popen(["ssh", "-o", "BatchMode=yes", "-o", "ConnectTimeout=15", "dell", command],
                                   stdout=subprocess.PIPE, stderr=errors)
        try:
            with tarfile.open(fileobj=process.stdout, mode="r|") as archive:
                for entry in archive:
                    if entry.name not in FILES or entry.name in received or not entry.isfile() or entry.size > 64 * 1024 * 1024:
                        raise RuntimeError(f"拒绝意外归档条目：{entry.name}")
                    with archive.extractfile(entry) as source, (folder / entry.name).open("xb") as destination:
                        shutil.copyfileobj(source, destination)
                    received.add(entry.name)
            if process.wait() != 0 or received != FILES:
                raise RuntimeError(f"下载未完整完成，请查看 {snapshot / 'transfer.stderr.log'}")
        finally:
            process.stdout.close()
            if process.poll() is None:
                process.terminate()
                process.wait()
    delivery, checked = verify(folder)
    record.update(status="verified", finished_utc=utc(), delivery_sha256=DELIVERY_SHA,
                  software_source_commit=delivery["software_source_commit"],
                  linux_commit=delivery["component_revisions"]["linux"],
                  files=checked, sd_disk_image=False, hardware_tested=False)
    save_json(snapshot / "download.json", record)
    print(f"完整软件下载并校验通过：{folder}\n仍需核对 SD 布局及上板验收。")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, RuntimeError, ValueError, tarfile.TarError) as error:
        print(f"错误：{error}", file=sys.stderr)
        sys.exit(1)
