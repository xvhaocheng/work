#!/usr/bin/env python3
"""Inspect/back up/update only the identified Genesys 2 SD boot partition on macOS."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import plistlib
import re
import stat
import struct
import subprocess
import sys
import tempfile
import uuid
import zlib

from fpga import ROOT, save_json, utc
from fetch_software import verify as verify_software

CARD_BYTES = 7817134080
BOOT_BYTES = 33554432
BOOT_OFFSET = 1048576
BOOT_UUID = "46298C61-BAC2-44C9-A8FF-8FA61CF2BEE0"
ROOT_UUID = "0AF525DD-76E6-44E8-A5CD-4A66DC5697B8"
BOOT_TYPE = "7412F7D5-A156-4B13-81DC-867174929325"
SOFTWARE = ROOT / "artifacts/software/20260915T140327Z-7bsjoi_r/experiment-v1"
MIB = 1024 * 1024


def sha(data):
    return hashlib.sha256(data).hexdigest()


def diskutil(*args):
    result = subprocess.run(["/usr/sbin/diskutil", *args], capture_output=True, check=True)
    return plistlib.loads(result.stdout)


def validate_info(disk, whole, boot, root):
    if not re.fullmatch(r"/dev/disk[0-9]+", disk):
        raise RuntimeError("请指定整张外置卡，例如 /dev/disk5。")
    name = disk.removeprefix("/dev/")
    if (whole.get("DeviceNode") != disk or whole.get("Internal") is not False
            or whole.get("WholeDisk") is not True or whole.get("VirtualOrPhysical") != "Physical"
            or whole.get("TotalSize") != CARD_BYTES or whole.get("DeviceBlockSize") != 512
            or whole.get("Content") != "GUID_partition_scheme"):
        raise RuntimeError("整盘身份不符：只接受已登记的 7,817,134,080 字节外置 GPT 卡。")
    expected = ((boot, BOOT_UUID, BOOT_BYTES, BOOT_OFFSET, name + "s1"),
                (root, ROOT_UUID, 7782514176, 34603008, name + "s2"))
    for info, uid, size, offset, identifier in expected:
        if (info.get("DiskUUID", "").upper() != uid or info.get("TotalSize") != size
                or info.get("PartitionMapPartitionOffset") != offset
                or info.get("ParentWholeDisk") != name or info.get("DeviceIdentifier") != identifier
                or info.get("Internal") is not False or info.get("DeviceBlockSize") != 512):
            raise RuntimeError("分区 UUID、大小、偏移或父磁盘不符，拒绝操作。")
    if boot.get("Content", "").upper() != BOOT_TYPE:
        raise RuntimeError("第一分区不是已登记的原始启动分区。")


def identity(disk):
    if not re.fullmatch(r"/dev/disk[0-9]+", disk):
        raise RuntimeError("磁盘路径格式错误。")
    whole = diskutil("info", "-plist", disk)
    boot = diskutil("info", "-plist", disk + "s1")
    root = diskutil("info", "-plist", disk + "s2")
    validate_info(disk, whole, boot, root)
    listing = diskutil("list", "-plist", disk)
    partitions = listing["AllDisksAndPartitions"][0]["Partitions"]
    if [p["DeviceIdentifier"] for p in partitions] != [boot["DeviceIdentifier"], root["DeviceIdentifier"]]:
        raise RuntimeError("分区数量或顺序变化。")
    return {"whole": whole, "boot": boot, "root": root}


def read_exact(fd, size, offset):
    data = bytearray()
    while len(data) < size:
        block = os.pread(fd, min(MIB, size - len(data)), offset + len(data))
        if not block:
            raise RuntimeError("读取长度不足，卡可能已断开。")
        data.extend(block)
    return bytes(data)


def validate_gpt(head, tail):
    def header(sector, expected_lba):
        if sector[:8] != b"EFI PART":
            raise RuntimeError("GPT 签名错误。")
        length = struct.unpack_from("<I", sector, 12)[0]
        if not 92 <= length <= 512:
            raise RuntimeError("GPT 头长度异常。")
        expected_crc = struct.unpack_from("<I", sector, 16)[0]
        buf = bytearray(sector[:length])
        buf[16:20] = b"\0" * 4
        if zlib.crc32(buf) != expected_crc or struct.unpack_from("<Q", sector, 24)[0] != expected_lba:
            raise RuntimeError("GPT 头 CRC 或位置错误。")
        lba, count, size, crc = struct.unpack_from("<QIII", sector, 72)
        if count != 128 or size != 128:
            raise RuntimeError("只支持已核对的 128×128 GPT 分区表。")
        return lba, count * size, crc
    end_lba = CARD_BYTES // 512 - 1
    lba, size, crc = header(head[512:1024], 1)
    entries = head[lba * 512:lba * 512 + size]
    backup_lba, backup_size, backup_crc = header(tail[-512:], end_lba)
    start = backup_lba * 512 - (CARD_BYTES - len(tail))
    backup_entries = tail[start:start + backup_size] if start >= 0 else b""
    if len(entries) != size or zlib.crc32(entries) != crc or zlib.crc32(backup_entries) != backup_crc or entries != backup_entries:
        raise RuntimeError("GPT 分区表 CRC 或主备一致性检查失败。")
    if (str(uuid.UUID(bytes_le=bytes(entries[16:32]))).upper() != BOOT_UUID
            or struct.unpack_from("<QQ", entries, 32) != (2048, 67583)):
        raise RuntimeError("原始 GPT 第一项与目标启动区不符。")
    return {"disk_guid": str(uuid.UUID(bytes_le=bytes(head[568:584]))), "primary_crc_ok": True,
            "backup_crc_ok": True, "first_lba": 2048, "last_lba": 67583}


def metadata(raw_disk):
    fd = os.open(raw_disk, os.O_RDONLY)
    try:
        head = read_exact(fd, MIB, 0)
        tail = read_exact(fd, MIB, CARD_BYTES - MIB)
    finally:
        os.close(fd)
    return head, tail, validate_gpt(head, tail)


def durable_file(path, data):
    with path.open("xb") as stream:
        stream.write(data)
        stream.flush()
        os.fsync(stream.fileno())
    if sha(path.read_bytes()) != sha(data):
        raise RuntimeError("本地备份校验失败。")


def write_and_verify(fd, image, verify=True):
    if len(image) != BOOT_BYTES:
        raise RuntimeError("写入镜像必须恰为 32 MiB。")
    position = 0
    while position < len(image):
        count = os.pwrite(fd, image[position:position + MIB], position)
        if count <= 0:
            raise RuntimeError("写入未前进。")
        position += count
    os.fsync(fd)
    if verify and sha(read_exact(fd, BOOT_BYTES, 0)) != sha(image):
        raise RuntimeError("启动分区回读校验失败；保留备份以便恢复。")


def run(args):
    skip_hash = getattr(args, "skip_payload_hash", False)
    skip_readback = getattr(args, "skip_readback", False)
    if (skip_hash or skip_readback) and (args.action != "update" or getattr(args, "release", "") not in ("dma-cache-sync-20260916", "e4-observer-20260916", "e4-batch-20260916", "e4-path-20260916", "e3-service-20260916", "e3-diag-20260918", "e3-followup-20260918", "e3-controlled-20260918", "p4m-baseline-20260920", "p4m-start-control-20260920")):
        raise RuntimeError("本次跳过检查选项只允许用于指定的缓存同步版更新。")
    info = identity(args.disk)
    image = payload = None
    if args.action == "update":
        release = getattr(args, "release", "experiment-v1")
        if release == "e3-followup-20260918":
            from fetch_e3_queue import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = {"sha256": delivery["payload_sha256"]}
        elif release == "p4m-baseline-20260920":
            from fetch_p4m import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = delivery["artifacts"]["fw_payload.bin"]
        elif release == "p4m-start-control-20260920":
            from fetch_p4m_start import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = delivery["artifacts"]["fw_payload.bin"]
        elif release == "e3-controlled-20260918":
            from fetch_e3_controlled import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = {"sha256": delivery["hashes"]["fw_payload.bin"]}
        elif release == "e3-diag-20260918":
            from fetch_e3_diag import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = {"sha256": delivery["payload_sha256"]}
        elif release == "e3-service-20260916":
            from fetch_e3_service import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = delivery["artifacts"]["fw_payload.bin"]
        elif release == "e4-path-20260916":
            from fetch_e4_path import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = delivery["files"]["fw_payload.bin"]
        elif release == "e4-batch-20260916":
            from fetch_e4_batch import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = delivery["artifacts"]["fw_payload.bin"]
        elif release == "e4-observer-20260916":
            from fetch_e4_observer import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = delivery["artifacts"]["fw_payload.bin"]
        elif release == "dma-cache-sync-20260916":
            from fetch_cache_sync import FOLDER, check_metadata
            delivery = check_metadata()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = delivery["artifacts"]["fw_payload.bin"]
        elif release == "dma-smoke-debug-20260916":
            from fetch_dma_debug import FOLDER, verify
            delivery, _ = verify()
            payload = (FOLDER / "fw_payload.bin").read_bytes()
            expected = delivery["artifacts"]["fw_payload.bin"]
        elif release == "experiment-v1":
            delivery, _ = verify_software(SOFTWARE)
            payload = (SOFTWARE / "fw_payload.bin").read_bytes()
            expected = delivery["software_artifacts"]["fw_payload.bin"]
        else:
            raise RuntimeError("未知软件版本。")
        if len(payload) > BOOT_BYTES or (not skip_hash and sha(payload) != expected["sha256"]):
            raise RuntimeError("payload 大小或 SHA256 不符。")
        image = payload + bytes(BOOT_BYTES - len(payload))
    elif args.action == "restore":
        if args.backup is None:
            raise RuntimeError("恢复必须指定 --backup 原备份目录。")
        source = args.backup.resolve()
        prior = json.loads((source / "status.json").read_text())
        if any(prior["identity"][name].get("DiskUUID") != info[name].get("DiskUUID")
               for name in ("boot", "root")):
            raise RuntimeError("备份来自另一张卡。")
        image = (source / "boot-before.bin").read_bytes()
        if len(image) != BOOT_BYTES or sha(image) != prior.get("backup_sha256"):
            raise RuntimeError("恢复备份大小或哈希不符。")
    plan = {"action": args.action, "disk": args.disk, "boot_partition": args.disk + "s1",
            "software_release": getattr(args, "release", "experiment-v1") if payload is not None else None,
            "card_bytes": CARD_BYTES, "boot_partition_bytes": BOOT_BYTES, "boot_uuid": BOOT_UUID,
            "payload_bytes": len(payload) if payload is not None else None,
            "payload_sha256": sha(payload) if payload is not None and not skip_hash else None,
            "expected_payload_sha256": expected["sha256"] if payload is not None else None,
            "payload_hash_verified": not skip_hash if payload is not None else None,
            "skip_readback_requested": skip_readback,
            "write_bytes": len(image) if image is not None else 0,
            "partition2_written": False, "repartition": False}
    print(json.dumps(plan, ensure_ascii=False, indent=2), flush=True)
    if args.dry_run:
        return 0
    base = ROOT / "results/sd"
    base.mkdir(parents=True, exist_ok=True)
    out = Path(tempfile.mkdtemp(prefix=args.action + "-", dir=base))
    record = {"started_utc": utc(), "status": "checking", "identity": info, "plan": plan,
              "hardware_boot_verified": False}
    save_json(out / "status.json", record)
    print(f"备份和记录：{out}", flush=True)
    raw_disk = args.disk.replace("/dev/disk", "/dev/rdisk")
    fd = None
    try:
        if args.action != "inspect":
            subprocess.run(["/usr/sbin/diskutil", "unmountDisk", args.disk], check=True)
        info = identity(args.disk)
        if any(info[name].get("MountPoint") for name in ("boot", "root")):
            raise RuntimeError("卡上仍有已挂载分区。")
        head, tail, gpt = metadata(raw_disk)
        durable_file(out / "disk-head-1MiB.bin", head)
        durable_file(out / "disk-tail-1MiB.bin", tail)
        record["gpt"] = gpt
        flags = os.O_RDWR if image is not None else os.O_RDONLY
        # Only this partition device can ever be opened for writing. Whole disk is read-only.
        fd = os.open(raw_disk + "s1", flags)
        if not stat.S_ISCHR(os.fstat(fd).st_mode):
            raise RuntimeError("目标不是原始分区字符设备。")
        before = read_exact(fd, BOOT_BYTES, 0)
        durable_file(out / "boot-before.bin", before)
        record.update(status="backup_verified", backup_sha256=sha(before),
                      head_sha256=sha(head), tail_sha256=sha(tail))
        record["old_image_markers"] = {marker.decode(): before.find(marker) for marker in (b"OpenSBI", b"Linux version", b"bbl loader")}
        save_json(out / "status.json", record)
        if image is not None:
            identity(args.disk)
            record.update(status="writing", intended_sha256=None if skip_hash else sha(image))
            save_json(out / "status.json", record)
            write_and_verify(fd, image, verify=not skip_readback)
            os.close(fd)
            fd = None
            subprocess.run(["/bin/sync"], check=True)
            if skip_readback:
                record.update(status="written_not_readback_verified", readback_verified=False,
                              disk_head_tail_unchanged=None, payload_readback_sha256=None)
                print("写入完成；按用户要求跳过写后回读，不能标记为回读验证通过。", flush=True)
                return 0
            # Reopen the raw partition for an independent complete readback.
            fd = os.open(raw_disk + "s1", os.O_RDONLY)
            after = read_exact(fd, BOOT_BYTES, 0)
            if sha(after) != sha(image):
                raise RuntimeError("重新打开设备后的完整回读不一致。")
            after_head, after_tail, _ = metadata(raw_disk)
            if (head, tail) != (after_head, after_tail):
                raise RuntimeError("磁盘头/尾保护区发生变化，请检查备份和磁盘。")
            record.update(status="readback_verified", readback_sha256=sha(after),
                          disk_head_tail_unchanged=True)
            if payload is not None:
                record["payload_readback_sha256"] = sha(after[:len(payload)])
        else:
            record["status"] = "backup_verified"
    except BaseException as error:
        record.update(status="failed_or_interrupted", error=str(error))
        raise
    finally:
        if fd is not None:
            os.close(fd)
        record["finished_utc"] = utc()
        save_json(out / "status.json", record)
        # sudo's invoking user should be able to inspect and retain the backup.
        if os.geteuid() == 0 and os.environ.get("SUDO_UID", "").isdigit() and os.environ.get("SUDO_GID", "").isdigit():
            owner = (int(os.environ["SUDO_UID"]), int(os.environ["SUDO_GID"]))
            for file in out.iterdir():
                os.chown(file, *owner, follow_symlinks=False)
            os.chown(out, *owner, follow_symlinks=False)
    print(f"完成：{record['status']}；记录：{out}")
    return 0


def main():
    parser = argparse.ArgumentParser(description="仅更新已登记 SD 卡的 32 MiB 第一启动分区；自动备份和回读。")
    parser.add_argument("action", choices=["inspect", "backup", "update", "restore"])
    parser.add_argument("--disk", required=True, help="当前整卡路径，例如 /dev/disk5；还会检查 UUID/容量/偏移")
    parser.add_argument("--dry-run", action="store_true", help="核对系统信息和软件，仅显示计划")
    parser.add_argument("--backup", type=Path, help="restore 使用的备份目录")
    parser.add_argument("--release", choices=["experiment-v1", "dma-smoke-debug-20260916", "dma-cache-sync-20260916", "e4-observer-20260916", "e4-batch-20260916", "e4-path-20260916", "e3-service-20260916", "e3-diag-20260918", "e3-followup-20260918", "e3-controlled-20260918", "p4m-baseline-20260920", "p4m-start-control-20260920"],
                        default="experiment-v1", help="默认保留旧软件；诊断版须显式指定")
    parser.add_argument("--skip-payload-hash", action="store_true", help="明确跳过新缓存同步版 payload 哈希核对")
    parser.add_argument("--skip-readback", action="store_true", help="明确跳过新缓存同步版写后回读，状态记录为未验证")
    try:
        return run(parser.parse_args())
    except PermissionError as error:
        if os.geteuid() == 0:
            print(f"已取得管理员权限，但 macOS 拒绝访问设备/路径。请检查启动此脚本的应用的磁盘访问权限，或在获得授权的系统终端运行。详情：{error}", file=sys.stderr)
        else:
            print(f"需要管理员权限读取/写入 SD 原始设备，请用 sudo 运行本脚本。详情：{error}", file=sys.stderr)
        return 1
    except (RuntimeError, OSError, ValueError, KeyError, subprocess.CalledProcessError) as error:
        print(f"操作停止：{error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
