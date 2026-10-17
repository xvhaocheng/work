#!/usr/bin/env python3
"""Build lightweight historical package navigation from existing metadata only."""
import argparse
import csv
import json
from pathlib import Path


HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[4]
TABLE = "paper/ispass2027/data/board_results.csv"
HISTORY = "iommu-platform/provenance/source-organization-20261017/history"
INDEX = f"{HISTORY}/index.json"
IDENTITIES = f"{HISTORY}/historical-identities.json"
OUT = "iommu-platform/out"
BATCH = f"{OUT}/e4-batch-20260916/delivery"
PATH = f"{OUT}/e4-path-20260916/delivery"
E3 = f"{OUT}/e3-controlled-20260918/delivery"
E2 = f"{OUT}/research-two-bit-20261003/delivery"
P4M = f"{OUT}/p4m-start-control-20260920/delivery"
CLEAN = f"{OUT}/composition-clean-20261006"
ISOLATED = f"{OUT}/composition-isolated-20261007"
SOFTWARE = {
    "e1": f"{OUT}/manifests/software-migration-4dma-scfix.json",
    "batch_main": f"{BATCH}/software-manifest.json",
    "batch_ext": f"{BATCH}/software-manifest.json",
    "batch_ds0": f"{BATCH}/software-manifest.json",
    "batch_ds50_retest": f"{BATCH}/software-manifest.json",
    "path": f"{PATH}/software-manifest.json",
    "e3v6": f"{E3}/software-manifest.json",
    "e2": f"{E2}/provenance/manifests/software-migration-4dma-research-e2-v2.json",
    "p4m": f"{P4M}/manifests/software-migration-4dma-p4m.json",
    "composition": f"{CLEAN}/software/software-update-state.json",
}
# Raw locations are navigation entries, not a new enumeration or reduction.
EXPERIMENTS = [
    ("e1", "E1 软件供给", ["E1"], [f"{OUT}/mac-e1-stage-20260916-032650-run-rbo67pvd/records.json"],
     "仅正式 80 次进入长表；原始 144 次保留。"),
    ("batch_main", "batch DS50 主矩阵", ["BATCH-DS50"], [f"{OUT}/mac-batch-main-20260916-112954-run-9sa69rhc/records.json"],
     "60 次；两种角色分别保留。"),
    ("batch_ext", "batch DS50 扩展", ["BATCH-DS50"], [f"{OUT}/mac-batch-ext-20260916-114036-run-8pjv_djc/records.json"],
     "30 次；复核统计入口在主矩阵 server-review.json 的 ext 部分。"),
    ("batch_ds0", "batch DS0", ["BATCH-DS0"], [f"{OUT}/mac-batch-ds0-20260916-132332-run-ebgxhirr/records.json"],
     "60 次；同软件的 DS0 对照，独立于 DS50 与回切组。"),
    ("batch_ds50_retest", "batch DS50 回切", ["BATCH-DS50-RETEST"], [f"{OUT}/mac-batch-ds50-20260916-135544-run-eawziww2/records.json"],
     "30 次；回切不与初始 DS50 组混合。"),
    ("path", "path 观察器", ["PATH"], [f"{OUT}/mac-path-main-20260916-164633-run-vvguros8/records.json"],
     "30 次正式样本；事务路径观察窗口与 batch 时间窗口分开。"),
    ("e3v6", "E3v6 控制响应", ["E3-V6-CONTROLLED"], [f"{OUT}/mac-e3-controlled-20260918-164635-run-rk48bhte/all-original-pairs.json"],
     "8 个正式配对与 3 个短验收配对分开；未触发值仍为 unknown/no_trigger。"),
    ("e2", "E2 容量边界", ["E2-V2"], [f"{OUT}/mac-research-two-bit-20261003/e2/runs"],
     "64 次含 4 次短验收；n 是试次，不将每次的两个目标当成两次重复。"),
    ("p4m", "P4M 固定间隔", ["P4M-FORMAL60"], [
        f"{OUT}/p4m-formal60-recovery-20260920/evidence/p4m-formal60-215406",
        f"{OUT}/p4m-complete-review-20261002/retained6",
        f"{OUT}/mac-p4m-remaining29-20261002/collection/unpacked"],
     "60 次按 25/6/29 采集段及 boot 保留；运行内核为 00089，旧 FPGA 锁中的 00088 不是本轮运行内核。"),
    ("composition", "composition 历史隔离 S/N/P/PN", ["COMPOSITION-CLEAN-BOARD", "COMPOSITION-ISOLATED-BOARD"], [
        f"{OUT}/composition-clean-20261006-board/mac-records/composition-clean-20261006-formal01/cases",
        f"{ISOLATED}/mac-records/composition-isolated-20261007-formal01/cases"],
     "clean_history 与 isolated_history 各 18 个选定样本，跨两个 BIT/boot 配对，不合并身份。"),
]


def read_json(path):
    return json.loads((ROOT / path).read_text())


def pointer(value, locator):
    for part in locator.strip("/").split("/") if locator else []:
        part = part.replace("~1", "/").replace("~0", "~")
        value = value[int(part)] if isinstance(value, list) else value[part]
    return value


def ref(path, locator=""):
    return {"path": path, "json_pointer": locator}


def known(value):
    return "unknown" if value is None else value


def artifact(role, local_path, evidence_path, locator, recorded_location="unknown", variant="all"):
    value = pointer(read_json(evidence_path), locator) if locator else None
    return {
        "role": role, "variant": variant, "local_path": local_path,
        "recorded_build_location": recorded_location,
        "recorded_sha256": known(value), "evidence": ref(evidence_path, locator),
        "hash_handling": "existing_metadata_only; not_rehashed_or_revalidated",
        "local_byte_identity_this_task": "unknown",
        "historical_loaded_byte_identity": "unknown",
    }


def package_artifacts(experiment):
    software = SOFTWARE[experiment]
    payload_key = "/artifacts/out~1opensbi~1platform~1fpga~1ariane~1firmware~1fw_payload.bin"
    image_key = "/artifacts/out~1linux~1arch~1riscv~1boot~1Image"
    if experiment == "composition":
        return [
            artifact("BIT", f"{CLEAN}/route-opt/ariane_xilinx.bit", f"{CLEAN}/acceptance.json", "", variant="clean_history"),
            artifact("BIT", f"{ISOLATED}/route-opt/ariane_xilinx.bit", f"{ISOLATED}/board-acceptance.json", "", variant="isolated_history"),
            artifact("SD_payload", f"{CLEAN}/software/delivery/fw_payload.bin", software, "/artifacts/fw_payload.bin/sha256", variant="clean_history;isolated_history_reuses_software"),
            artifact("kernel_Image", f"{CLEAN}/software/delivery/Image", software, "/artifacts/Image/sha256", variant="clean_history;isolated_history_reuses_software"),
        ]
    if experiment == "e1":
        build = f"{OUT}/manifests/fpga-migration-4dma-scfix.json"
        bit = f"{OUT}/migration-4dma-scfix/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit"
        bit_ref = "/artifacts/out~1migration-4dma-scfix~1fpga-src~1cva6~1corev_apu~1fpga~1work-fpga~1ariane_xilinx.bit"
        payload = image = "unknown"
    elif experiment.startswith("batch_"):
        build = f"{OUT}/manifests/fpga-{'baseline' if experiment == 'batch_ds0' else 'migration'}-4dma-batch.json"
        bit = (f"{OUT}/baseline-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit"
               if experiment == "batch_ds0" else f"{BATCH}/ariane_xilinx.bit")
        profile = "baseline-4dma-batch" if experiment == "batch_ds0" else "migration-4dma-batch"
        bit_ref = f"/artifacts/out~1{profile}~1fpga-src~1cva6~1corev_apu~1fpga~1work-fpga~1ariane_xilinx.bit"
        payload, image = f"{BATCH}/fw_payload.bin", "unknown"
    else:
        delivery = {"path": PATH, "e3v6": E3, "e2": E2 + "/artifacts", "p4m": P4M}[experiment]
        profile = {"path": "path", "e3v6": "e3-controlled", "e2": "research-e2-v2", "p4m": "p4m"}[experiment]
        build = f"{OUT}/manifests/fpga-migration-4dma-{profile}.json"
        bit = f"{delivery}/{'role1.bit' if experiment == 'e2' else 'ariane_xilinx.bit'}"
        bit_ref = f"/artifacts/out~1migration-4dma-{profile}~1fpga-src~1cva6~1corev_apu~1fpga~1work-fpga~1ariane_xilinx.bit"
        payload = f"{delivery}/fw_payload.bin"
        image = f"{delivery}/Image" if experiment in ["e3v6", "p4m"] else "unknown"
    return [
        artifact("BIT", bit, build, bit_ref),
        artifact("SD_payload", payload, software, payload_key, "out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin"),
        artifact("kernel_Image", image, software, image_key, "out/linux/arch/riscv/boot/Image"),
    ]


def source_entry(record_id, index_records, identities):
    i, entry = next((i, r) for i, r in enumerate(index_records) if r["experiment_id"] == record_id)
    j, identity = next((i, r) for i, r in enumerate(identities) if r["experiment_id"] == record_id)
    repos = []
    for k, repository in enumerate(identity["repositories"]):
        if repository.get("component") not in ["platform", "opensbi", "linux", "cva6", "cva6/corev_apu/riscv-iommu"]:
            continue
        repos.append({**{key: known(repository.get(key)) for key in [
            "component", "path", "identity_role", "checkout_branch", "checkout_commit", "selected_ref", "selected_commit", "dirty"]},
            "evidence": ref(IDENTITIES, f"/records/{j}/repositories/{k}")})
    return {
        "historical_id": record_id, "index_evidence": ref(INDEX, f"/records/{i}"),
        "identity_evidence": ref(IDENTITIES, f"/records/{j}"),
        "complete_executed_tree_commit": "unknown",
        "repositories": repos,
        "build_inputs": identity["build_inputs"],
        "overlays": identity["overlays"],
        "source_manifest_paths": [x["path"] for x in entry["source_manifests"]],
        "report": entry["report"]["path"],
        "identity_limit": "Recorded checkout/base/lock roles are distinct; dirty overlays and later organized commits do not establish one complete executed tree.",
    }


def generate():
    index_records = read_json(INDEX)["records"]
    identities = read_json(IDENTITIES)["records"]
    with (ROOT / TABLE).open(newline="") as handle:
        rows = list(csv.DictReader(handle))
    key_groups = read_json("paper/ispass2027/data/key_rows.json")["groups"]
    selected_keys = {x["row_id"] for group in key_groups.values() for x in group}
    files = {}
    navigation = []
    for experiment, label, ids, raw_locations, note in EXPERIMENTS:
        selected = [r for r in rows if r["experiment"] == experiment]
        assert selected, f"No existing rows for {experiment}"
        software_path = SOFTWARE[experiment]
        software = read_json(software_path)
        result = {
            "schema": "ispass2027-historical-package-navigation-v1",
            "experiment": experiment, "label": label,
            "status": "historical_navigation; no_new_measurement_or_artifact_acceptance",
            "results": {
                "table": TABLE, "selector": {"experiment": experiment},
                "row_id_prefix": experiment + ".", "row_count": len(selected),
                "key_row_ids": sorted(r["row_id"] for r in selected if r["row_id"] in selected_keys)[:18],
                "sample_sets": sorted({r["sample_set"] for r in selected}),
                "n_semantics": "repeated_trials_or_paired_trials; row_count_is_not_n",
                "per_row_original_locator": "table.raw_path + table.source_locator",
            },
            "original_data": {
                "review_sources_from_table": sorted({r["raw_path"] for r in selected}),
                "raw_locations": raw_locations,
                "note": note,
            },
            "runtime_identity_from_results": {
                "kernel_releases": sorted({r["kernel_id"] for r in selected}),
                "bitstream_ids": sorted({r["bitstream_id"] for r in selected}),
                "boot_ids": sorted({r["boot_id"] for r in selected}),
                "identity_status": sorted({r["identity_status"] for r in selected}),
                "role": "Existing result-table identity, not FPGA dependency-lock selection.",
            },
            "source_identity": [source_entry(x, index_records, identities) for x in ids],
            "software_build": {
                "evidence": ref(software_path),
                "platform_commit": known(software.get("platform_commit")),
                "platform_dirty": known(software.get("platform_dirty")),
                "build_start": {k: known(software.get("build_start", {}).get(k)) for k in ["platform_commit", "platform_dirty"]},
                "sources_lock_sha256": known(software.get("sources_lock_sha256")),
                "complete_executed_tree_commit": "unknown",
                "role": "Recorded software build, separate from FPGA build and configured dependency locks.",
            },
            "artifacts": package_artifacts(experiment),
            "storage": "Metadata and local path links only; no result table or binary copied.",
            "unknown_policy": "unknown is not zero, clean, current HEAD, or a verified loaded hash.",
        }
        for a in result["artifacts"]:
            if a["local_path"] != "unknown":
                assert (ROOT / a["local_path"]).is_file(), f"Missing navigation target: {a['local_path']}"
        assert all(r["row_id"].startswith(result["results"]["row_id_prefix"]) for r in selected)
        files[f"{experiment}.json"] = json.dumps(result, ensure_ascii=False, indent=2) + "\n"
        navigation.append({"experiment": experiment, "manifest": f"{experiment}.json", "label": label, "result_rows": len(selected)})
    files["index.json"] = json.dumps({
        "schema": "ispass2027-historical-packages-index-v1", "results_table": TABLE,
        "historical_source_index": INDEX, "experiment_count": len(navigation),
        "result_rows": sum(x["result_rows"] for x in navigation), "experiments": navigation,
        "scope": "Existing metadata navigation only; no historical hashes recomputed, data re-extracted, binaries copied, or board operations performed.",
    }, ensure_ascii=False, indent=2) + "\n"
    return files


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="compare only these generated navigation files")
    args = parser.parse_args()
    files = generate()
    for name, content in files.items():
        target = HERE / name
        if args.check:
            if not target.exists() or target.read_text() != content:
                raise SystemExit(f"Navigation differs: {target}")
        else:
            target.write_text(content)
    print(f"{'Checked' if args.check else 'Wrote'} {len(files) - 1} historical manifests and index; no artifact contents read")


if __name__ == "__main__":
    main()
