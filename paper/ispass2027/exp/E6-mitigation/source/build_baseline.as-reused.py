#!/usr/bin/env python3
"""Build the fixed ISPASS baseline without changing production checkouts.

No board, UART, JTAG or network-control operations are implemented here.
Each phase writes its own manifest before execution and refuses old outputs.
"""
from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import stat
import subprocess
import sys
import tarfile
import tempfile

sys.dont_write_bytecode = True
WORK = Path(__file__).resolve().parents[3]
PLATFORM = WORK / "iommu-platform"
HERE = Path(__file__).resolve().parent
BASE = "ba8cdd10659a777ccff0080bf020be64b5c1e064"
DONOR = "e39dce27530a94c21d8c0bf285e5e2949e7e1bc8"
IOMMU = "85b157ebe71a623e326e51fff7b7126fe77e8eb9"
LINUX = "45bf76b15e36cf608dff95013d91f8bedb7b1b8e"
SBI = "e8d21bfdc2dd753314627968e496c3a1188f6dfb"
OLD_PLATFORM = "a2e847539bb638662f98dc920de0e8fd63d046e3"
REF = "refs/heads/codex/ispass2027-baseline-20261017"
PINNED_CVA6 = "8aac3fcd345af7686a55af1867783fd6b91a0069"
PINNED_PLATFORM = "885ed0a7ea90ed42dae2b7a2dadfdd0acba15273"
VIVADO_BIN = "/tools/Xilinx/Vivado/2023.1/bin"
DS_CYCLES = {"ds0": 0, "ds25": 25, "ds50": 50, "ds100": 100}
ENV = dict(os.environ, PATH=VIVADO_BIN + ":" + os.environ["PATH"],
           XILINXD_LICENSE_FILE="2100@opus4.apal.cs.waseda.ac.jp")
ENV.setdefault("RISCV", "/home/xvhaocheng/RISCV/riscv_tools")


def now():
    return dt.datetime.now(dt.timezone.utc).isoformat()


def sha(path):
    h = hashlib.sha256()
    with Path(path).open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def save(path, obj):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    temp = path.with_suffix(path.suffix + ".partial")
    temp.write_text(json.dumps(obj, indent=2, ensure_ascii=False) + "\n")
    temp.replace(path)


def git(repo, *args, data=None, env=None):
    return subprocess.check_output(["git", "-C", str(repo), *args], input=data,
                                   env=env or ENV)


def show(repo, commit, name):
    return git(repo, "show", commit + ":" + name)


def load_module(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def validate_source(path):
    """Accept only this frozen source combination, before output creation."""
    path = Path(path)
    if not path.is_file():
        raise ValueError("--source must name an existing source-manifest.json file")
    selected = json.loads(path.read_text())
    if not isinstance(selected, dict):
        raise ValueError("source manifest must be a JSON object")
    expected_fields = {"schema": "ispass2027-baseline-source-v1", "status": "prepared",
                       "base_cva6": BASE, "measurement_donor": DONOR,
                       "selected_cva6": PINNED_CVA6, "selected_platform": PINNED_PLATFORM,
                       "iommu": IOMMU, "linux": LINUX, "opensbi": SBI}
    for name, expected in expected_fields.items():
        if selected.get(name) != expected:
            raise ValueError("frozen source identity mismatch: " + name)
    records = selected.get("components")
    if not isinstance(records, list) or not all(isinstance(c, dict) for c in records):
        raise ValueError("source manifest needs a component list")
    if any(not isinstance(c.get("path"), str) or
           not isinstance(c.get("commit"), str) for c in records):
        raise ValueError("source component paths and commits must be strings")
    components = {c.get("path"): c.get("commit") for c in records}
    if len(components) != len(records):
        raise ValueError("duplicate source component paths")
    exporter = load_module(PLATFORM / "provenance/source-organization-20261017/export_sources.py", "ispass_guard_export")
    actual = exporter.resolve({"components": [{"path": "cva6", "commit": PINNED_CVA6},
                                               {"path": "linux", "commit": LINUX},
                                               {"path": "opensbi", "commit": SBI}]})
    expected_components = {c["path"]: c["commit"] for c in actual}
    if components != expected_components:
        raise ValueError("component identities disagree with frozen parent gitlinks")
    if components.get("cva6/corev_apu/riscv-iommu") != IOMMU:
        raise ValueError("selected IOMMU child is not the fixed F5 source")
    return selected, components


def tree_hashes(root):
    result = {}
    for p in sorted(Path(root).rglob("*")):
        if p.is_file() and not p.is_symlink():
            result[str(p.relative_to(root))] = sha(p)
        elif p.is_symlink():
            result[str(p.relative_to(root))] = {"symlink": os.readlink(p)}
    return result


def repo_identity(repo):
    return {"path": str(repo),
            "checkout_branch": git(repo, "branch", "--show-current").decode().strip(),
            "checkout_commit": git(repo, "rev-parse", "HEAD").decode().strip(),
            "status": git(repo, "--no-optional-locks", "status", "--porcelain").decode(),
            "unstaged_diff": git(repo, "diff", "--binary", *(["--", "paper/ispass2027/board"] if repo == WORK else [])).decode(),
            "staged_diff": git(repo, "diff", "--cached", "--binary", *(["--", "paper/ispass2027/board"] if repo == WORK else [])).decode()}


def archive(repo, commit, dest):
    # Reuse the established archive filter, including relative vendor symlinks.
    module = load_module(PLATFORM / "scripts/platform.py", "ispass_archive")
    module.stage_repo({"path": str(repo.relative_to(PLATFORM)), "commit": commit}, dest)


def run(command, cwd, log, manifest, manifest_path):
    manifest.setdefault("commands", []).append({"argv": list(map(str, command)),
                                                "cwd": str(cwd), "start": now()})
    save(manifest_path, manifest)
    log.write(("\n+ " + " ".join(map(str, command)) + "\n").encode())
    log.flush()
    proc = subprocess.run(list(map(str, command)), cwd=cwd, env=ENV,
                          stdout=log, stderr=subprocess.STDOUT)
    manifest["commands"][-1].update(end=now(), exit_code=proc.returncode)
    save(manifest_path, manifest)
    if proc.returncode:
        raise RuntimeError("command failed, exit=" + str(proc.returncode))


def commit_index(repo, parent, changes, message):
    fd, name = tempfile.mkstemp(prefix="ispass-index-")
    os.close(fd)
    os.unlink(name)
    env = dict(ENV, GIT_INDEX_FILE=name)
    try:
        git(repo, "read-tree", parent, env=env)
        for path, mode, value in changes:
            oid = value if mode == "160000" else git(repo, "hash-object", "-w", "--stdin", data=value).decode().strip()
            git(repo, "update-index", "--add", "--cacheinfo", mode, oid, path, env=env)
        tree = git(repo, "write-tree", env=env).decode().strip()
        return git(repo, "commit-tree", tree, "-p", parent,
                   data=(message + "\n").encode()).decode().strip()
    finally:
        Path(name).unlink(missing_ok=True)


def prepare_complete(out):
    """Preparation body with ref creation separated from immutable source construction."""
    out.mkdir(parents=True, exist_ok=False)
    manifest = {"schema": "ispass2027-baseline-source-v1", "start": now(), "status": "preparing",
                "base_cva6": BASE, "measurement_donor": DONOR, "iommu": IOMMU,
                "linux": LINUX, "opensbi": SBI,
                "repositories": [repo_identity(p) for p in
                                 [WORK, PLATFORM, PLATFORM / "cva6", PLATFORM / "cva6/corev_apu/riscv-iommu",
                                  PLATFORM / "linux", PLATFORM / "opensbi"]]}
    save(out / "source-manifest.json", manifest)
    repo = PLATFORM / "cva6"
    names = git(repo, "diff", "--name-only", BASE, DONOR).decode().splitlines()
    assert len(names) == 8 and "corev_apu/riscv-iommu" not in names
    content = {p: show(repo, DONOR, p) for p in names}
    top_path = "corev_apu/fpga/src/ariane_xilinx.sv"
    p_path = "corev_apu/fpga/src/ariane_peripherals_xilinx.sv"
    top = content[top_path].decode()
    top = top.replace("localparam int unsigned IOMMUDSFixedDelayInput = 50;",
                      "// Direct DS wire in production; the delayer is diagnostic only.\n"
                      "localparam bit IOMMUDSDelayEnable = 0;\nlocalparam int unsigned IOMMUDSFixedDelayInput = 0;")
    top = top.replace("localparam int unsigned IOMMUDSFixedDelayOutput = 50;", "localparam int unsigned IOMMUDSFixedDelayOutput = 0;")
    top = top.replace(".IOMMUDSFixedDelayInput  ( IOMMUDSFixedDelayInput  ),",
                      ".IOMMUDSDelayEnable      ( IOMMUDSDelayEnable      ),\n    .IOMMUDSFixedDelayInput  ( IOMMUDSFixedDelayInput  ),")
    p = content[p_path].decode().replace("parameter int unsigned IOMMUDSFixedDelayInput  = 0,",
               "parameter bit IOMMUDSDelayEnable = 0,\n    parameter int unsigned IOMMUDSFixedDelayInput  = 0,")
    begin = p.index("        axi_ds_req_t  axi_iommu_ds_delayed_req;")
    end = p.index("        // AXI Bus between IOMMU Completion", begin)
    p = (p[:begin] + "        if (IOMMUDSDelayEnable) begin : gen_ds_delay_diagnostic\n" +
         "\n".join("    " + line for line in p[begin:end].splitlines()) + "\n"
         "        end else begin : gen_ds_direct\n"
         "            `AXI_ASSIGN_FROM_REQ(iommu_ds, axi_iommu_ds_req)\n"
         "            `AXI_ASSIGN_TO_RESP(axi_iommu_ds_resp, iommu_ds)\n"
         "        end\n\n" + p[end:])
    assert top.count("localparam bit IOMMUDSDelayEnable = 0;") == 1 and p.count("gen_ds_direct") == 1
    content[top_path], content[p_path] = top.encode(), p.encode()
    commit = commit_index(repo, BASE, [(n, "100644", b) for n, b in content.items()],
                          "Add bounded batch/path instrumentation to corrected ISPASS baseline")
    git(repo, "update-ref", REF, commit, "0" * 40)
    exporter = load_module(PLATFORM / "provenance/source-organization-20261017/export_sources.py", "ispass_export")
    components = exporter.resolve({"components": [{"path": "cva6", "commit": commit},
                                                   {"path": "linux", "commit": LINUX},
                                                   {"path": "opensbi", "commit": SBI}]})
    lock = json.loads((PLATFORM / "sources.lock.json").read_text())
    prior = {x["path"]: x for x in lock["components"]}
    lock["components"] = [dict(prior.get(c["path"], {}), path=c["path"], commit=c["commit"]) for c in components]
    configs = {"ispass2027-baseline-ds0": {"ds_delay_enable": False, "ds_delay_input": 0, "ds_delay_output": 0},
               "ispass2027-diagnostic-ds50": {"ds_delay_enable": True, "ds_delay_input": 50, "ds_delay_output": 50}}
    for c in configs.values():
        c.update(board="genesys2", cpu_hz=50000000, timebase_hz=25000000,
                 num_dma=4, device_ids=[10, 11, 12, 13], iotlb_entries=16,
                 observer_version=3, batch_supply=True, iommu_commit=IOMMU,
                 research_services=False, selected_cva6_commit=commit)
    parent = git(PLATFORM, "rev-parse", "HEAD").decode().strip()
    changes = [("cva6", "160000", commit), ("linux", "160000", LINUX), ("opensbi", "160000", SBI),
               ("sources.lock.json", "100644", (json.dumps(lock, indent=2) + "\n").encode())]
    changes += [("configs/" + n + ".json", "100644", (json.dumps(c, indent=2) + "\n").encode()) for n, c in configs.items()]
    platform_commit = commit_index(PLATFORM, parent, changes, "Pin independent ISPASS corrected batch/path baseline profiles")
    git(PLATFORM, "update-ref", REF, platform_commit, "0" * 40)
    git(PLATFORM / "linux", "update-ref", REF, LINUX, "0" * 40)
    git(PLATFORM / "opensbi", "update-ref", REF, SBI, "0" * 40)
    (out / "baseline-cva6.patch").write_bytes(git(repo, "diff", "--binary", BASE, commit))
    save(out / "sources.lock.json", lock)
    save(out / "profiles.json", configs)
    manifest.update(status="prepared", end=now(), selected_cva6=commit,
                    selected_platform=platform_commit, components=components, source_ref=REF,
                    measurement_overlay_files={n: hashlib.sha256(v).hexdigest() for n, v in content.items()},
                    exclusions=["E3", "O1", "banked", "P/N/PN", "service scheduling changes"],
                    profiles=configs, script_sha256=sha(__file__))
    save(out / "source-manifest.json", manifest)
    save(HERE / "baseline-source.json", {"source_manifest": str(out / "source-manifest.json"),
                                        "sha256": sha(out / "source-manifest.json"),
                                        "platform_commit": platform_commit, "cva6_commit": commit,
                                        "iommu_commit": IOMMU, "linux_commit": LINUX, "opensbi_commit": SBI,
                                        "profiles": configs})
    print(json.dumps({"status": "prepared", "out": str(out), "cva6": commit, "platform": platform_commit}))


def phase_manifest(out, source, action):
    out.mkdir(parents=True, exist_ok=False)
    m = {"schema": "ispass2027-baseline-build-v1", "action": action,
         "start": now(), "status": "running", "source_manifest": str(source),
         "source_manifest_sha256": sha(source), "script_sha256": sha(__file__),
         "environment": {k: ENV[k] for k in ["PATH", "RISCV", "XILINXD_LICENSE_FILE"]}}
    save(out / "build-manifest.json", m)
    return m


def software(out, source, jobs):
    _, components = validate_source(source)
    m = phase_manifest(out, source, "software")
    manifest_path = out / "build-manifest.json"
    try:
        with (out / "build.log").open("xb") as log:
            linux, sbi = out / "src/linux", out / "src/opensbi"
            archive(PLATFORM / "linux", components["linux"], linux)
            archive(PLATFORM / "opensbi", components["opensbi"], sbi)
            inputs = out / "inputs"
            inputs.mkdir()
            for name in ["bench/idma_experiment.c", "configs/linux_defconfig", "configs/platform.dts"]:
                p = inputs / name
                p.parent.mkdir(parents=True, exist_ok=True)
                p.write_bytes(show(PLATFORM, OLD_PLATFORM, name))
            for name in ["bench/platform_timeout.c", "bench/platform_sd_update.c"]:
                p = inputs / name
                p.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(PLATFORM / name, p)
            shutil.copyfile(PLATFORM / "assets/rootfs-base.cpio", inputs / "rootfs-base.cpio")
            m.update(selected_linux=components["linux"], selected_opensbi=components["opensbi"],
                     selected_platform_software_inputs=OLD_PLATFORM,
                     handwritten_inputs=tree_hashes(inputs),
                     kernel_critical_sources={n: sha(linux / n) for n in
                       ["drivers/misc/idma.c", "include/uapi/linux/idma.h", "drivers/iommu/riscv/io_pgtable.c",
                        "drivers/iommu/riscv/iommu.c", "drivers/iommu/riscv/iommu-hpm.c"]})
            save(manifest_path, m)
            bins = out / "bin"
            bins.mkdir()
            uapi = out / "uapi/linux"
            uapi.mkdir(parents=True)
            shutil.copyfile(linux / "include/uapi/linux/idma.h", uapi / "idma.h")
            for name in ["idma_experiment", "platform_timeout", "platform_sd_update"]:
                target = name.replace("_", "-") if name.startswith("platform_") else name
                run(["riscv64-linux-gnu-gcc", "-O2", "-static", "-Wall", "-Wextra",
                     "-I", uapi.parent, inputs / ("bench/" + name + ".c"), "-o", bins / target], out, log, m, manifest_path)
            rootfs_module = load_module(PLATFORM / "scripts/rootfs.py", "ispass_rootfs")
            raw = (inputs / "rootfs-base.cpio").read_bytes()
            entries = list(rootfs_module.read_newc(raw))
            packed = bytearray()
            binaries = {n: p.read_bytes() for p in bins.iterdir() for n in [p.name]}
            for name, fields, body in entries:
                if Path(name).name.startswith("idma_") or Path(name).name in binaries:
                    continue
                rootfs_module.write_entry(packed, name, fields, body)
            ino = max(fields[0] for _, fields, _ in entries) + 1
            for name, body in binaries.items():
                rootfs_module.write_entry(packed, "usr/bin/" + name,
                    [ino, stat.S_IFREG | 0o755, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0], body)
                ino += 1
            rootfs_module.write_entry(packed, "TRAILER!!!", [0] * 13, b"")
            packed.extend(b"\0" * ((-len(packed)) % 512))
            rootfs = out / "rootfs.cpio"
            rootfs.write_bytes(packed)
            reread = {n: b for n, _, b in rootfs_module.read_newc(packed)}
            assert all(reread["usr/bin/" + n] == b for n, b in binaries.items())
            kernel = out / "linux-build"
            kernel.mkdir()
            config = kernel / ".config"
            shutil.copyfile(inputs / "configs/linux_defconfig", config)
            run([linux / "scripts/config", "--file", config,
                 "--set-str", "INITRAMFS_SOURCE", rootfs,
                 "--set-str", "LOCALVERSION", "-ispass2027-pathv3",
                 "--disable", "LOCALVERSION_AUTO"], out, log, m, manifest_path)
            common = ["make", "-C", linux, "ARCH=riscv", "O=" + str(kernel), "CROSS_COMPILE=riscv64-linux-gnu-"]
            run(common + ["olddefconfig"], out, log, m, manifest_path)
            m["kernel_config_sha256"] = sha(config)
            m["rootfs_sha256"] = sha(rootfs)
            save(manifest_path, m)
            run(common + ["-j" + str(jobs), "Image"], out, log, m, manifest_path)
            image = kernel / "arch/riscv/boot/Image"
            dtb = out / "platform.dtb"
            run(["dtc", "-I", "dts", "-O", "dtb", "-o", dtb, inputs / "configs/platform.dts"], out, log, m, manifest_path)
            loader = linux / "tools/lloader"
            run(["make", "CROSS_COMPILE=riscv64-unknown-elf-", "ARCH=rv64", "IMAGE=" + str(image),
                 "DTB=" + str(dtb), "TARGET=linux-rv64-cva6"], loader, log, m, manifest_path)
            sbi_build = out / "opensbi-build"
            run(["make", "-C", sbi, "O=" + str(sbi_build), "CROSS_COMPILE=riscv64-linux-gnu-",
                 "PLATFORM=fpga/ariane", "FW_PAYLOAD=y", "FW_PAYLOAD_PATH=" + str(loader / "linux-rv64-cva6.bin"),
                 "-j" + str(jobs)], out, log, m, manifest_path)
            payload = sbi_build / "platform/fpga/ariane/firmware/fw_payload.bin"
            shutil.copyfile(payload, out / "fw_payload.bin")
            m.update(status="passed", kernel_release=(kernel / "include/config/kernel.release").read_text().strip(),
                     artifacts={str(p.relative_to(out)): sha(p) for p in
                                [out / "fw_payload.bin", image, dtb, rootfs, *bins.iterdir()]})
            assert (out / "fw_payload.bin").stat().st_size <= 33554432
    except BaseException as exc:
        m.update(status="failed", error=repr(exc))
        raise
    finally:
        m["end"] = now()
        save(manifest_path, m)
        save(HERE / "software-build.json", {"manifest": str(manifest_path), "sha256": sha(manifest_path),
                                            "status": m["status"], "payload": str(out / "fw_payload.bin"),
                                            "kernel_release": m.get("kernel_release")})


def fpga(out, source, variant):
    selected, component_commits = validate_source(source)
    m = phase_manifest(out, source, "fpga-" + variant)
    manifest_path = out / "build-manifest.json"
    try:
        cycles = DS_CYCLES[variant]
        m.update(selected_platform=selected["selected_platform"],
                 execution_repository=repo_identity(WORK))
        with (out / "build.log").open("xb") as log:
            stage = out / "src"
            exporter = load_module(PLATFORM / "provenance/source-organization-20261017/export_sources.py", "ispass_fpga_export")
            components = [c for c in selected["components"] if c["path"] == "cva6" or c["path"].startswith("cva6/")]
            for c in components:
                exporter.PLATFORM = PLATFORM
                archive(PLATFORM / c["path"], c["commit"], stage / c["path"])
            cva6 = stage / "cva6"
            top = cva6 / "corev_apu/fpga/src/ariane_xilinx.sv"
            if cycles:
                text = top.read_text()
                for name, value in [("IOMMUDSDelayEnable", 1), ("IOMMUDSFixedDelayInput", cycles), ("IOMMUDSFixedDelayOutput", cycles)]:
                    text, n = re.subn(r"(localparam (?:bit|int unsigned) " + name + r"\s*=\s*)\d+", lambda x: x[1] + str(value), text)
                    assert n == 1, name
                top.write_text(text)
            shutil.copytree(PLATFORM / "configs/boards", stage / "boards")
            for tcl in [cva6 / "corev_apu/fpga/scripts/prologue.tcl", *cva6.glob("corev_apu/fpga/xilinx/*/tcl/run.tcl")]:
                tcl.write_text("set_param board.repoPaths [list " + str(stage / "boards") + "]\n" + tcl.read_text())
            m.update(variant=variant, selected_cva6=selected["selected_cva6"],
                     selected_iommu=component_commits["cva6/corev_apu/riscv-iommu"],
                     components=components, sources=tree_hashes(stage),
                     parameters={"ds_delay_enable": bool(cycles), "ds_cycles": [cycles, cycles],
                                 "iotlb_entries": 16, "observer_version": 3, "clock_hz": 50000000},
                     direct_ds_wire=variant == "ds0")
            save(manifest_path, m)
            run(["make", "-C", cva6, "BOARD=genesys2", "fpga"], out, log, m, manifest_path)
            result = cva6 / "corev_apu/fpga/work-fpga"
            timing = result / "ariane_xilinx_timing_summary_routed.rpt"
            rows = timing.read_text().splitlines()
            i = next(i for i, line in enumerate(rows) if "WNS(ns)" in line)
            values = rows[i + 2].split()
            m["timing"] = {"wns_ns": float(values[0]), "tns_ns": float(values[1]), "setup_failing": int(values[2]),
                           "whs_ns": float(values[4]), "hold_failing": int(values[6]),
                           "wpws_ns": float(values[8]), "pulse_failing": int(values[10]),
                           "report": str(timing), "sha256": sha(timing), "full_io_cdc_signoff": False}
            artifacts = [result / "ariane_xilinx.bit", result / "ariane_xilinx.mcs", timing,
                         result / "ariane_xilinx_route_status.rpt", result / "ariane_xilinx_utilization_placed.rpt"]
            m["artifacts"] = {str(p): sha(p) for p in artifacts}
            routing = (result / "ariane_xilinx_route_status.rpt").read_text()
            errors = re.search(r"# of nets with routing errors\.*\s*:\s*(\d+)", routing, re.I)
            if errors is None:
                raise ValueError("routing error count absent from route status report")
            m["routing_errors"] = int(errors[1])
            m["status"] = "passed" if (m["timing"]["wns_ns"] >= 0 and m["timing"]["setup_failing"] == 0 and
                                                  m["timing"]["hold_failing"] == 0 and m["timing"]["pulse_failing"] == 0 and
                                                  m["routing_errors"] == 0) else "timing_failed"
            if m["status"] != "passed":
                raise RuntimeError("FPGA timing or routing acceptance failed; do not load")
            if m["status"] == "passed":
                boundary = out / "routed-boundary.json"
                run([VIVADO_BIN + "/vivado", "-mode", "batch", "-nojournal", "-nolog",
                     "-source", HERE / "verify_routed.tcl", "-tclargs",
                     result / "ariane_xilinx_routed.dcp", int(bool(cycles)), boundary], out, log, m, manifest_path)
                m["routed_boundary"] = json.loads(boundary.read_text())
                m["routed_boundary_sha256"] = sha(boundary)
    except BaseException as exc:
        m.update(status="failed", error=repr(exc))
        raise
    finally:
        m["end"] = now()
        save(manifest_path, m)
        save(HERE / ("fpga-" + variant + "-build.json"), {"manifest": str(manifest_path), "sha256": sha(manifest_path),
                                                        "status": m["status"], "timing": m.get("timing"), "artifacts": m.get("artifacts")})


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("action", choices=["prepare", "software", "fpga"])
    ap.add_argument("--out", required=True, type=Path)
    ap.add_argument("--source", type=Path)
    ap.add_argument("--variant", choices=list(DS_CYCLES))
    ap.add_argument("--jobs", type=int, default=16)
    args = ap.parse_args()
    if args.jobs <= 0:
        ap.error("--jobs must be positive")
    if args.action != "prepare":
        if args.source is None:
            ap.error(args.action + " requires --source source-manifest.json")
        if args.action == "fpga" and args.variant is None:
            ap.error("fpga requires --variant")
        try:
            validate_source(args.source)
        except (ValueError, OSError, subprocess.CalledProcessError) as exc:
            ap.error(str(exc))
    out = args.out.resolve()
    if args.action == "prepare":
        prepare_complete(out)
    elif args.action == "software":
        software(out, args.source.resolve(), args.jobs)
    else:
        if not args.variant:
            ap.error("fpga requires --variant")
        fpga(out, args.source.resolve(), args.variant)


if __name__ == "__main__":
    main()
