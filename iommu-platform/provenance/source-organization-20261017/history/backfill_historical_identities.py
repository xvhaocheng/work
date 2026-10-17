#!/usr/bin/env python3
"""Add historical identity sidecars using existing evidence only.

This program reads an explicit evidence allowlist and Git objects; it never
invokes an experiment, compiler, reducer, board tool, checkout or Git mutation.
Only historical-identities.json and historical-identities.csv are written.
"""

from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
from collections import Counter
from pathlib import Path


HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
PREFIX = HERE.relative_to(ROOT).as_posix()
INDEX = f"{PREFIX}/index.json"
CORE = f"{PREFIX}/core-build-sources.json"
RECOVERY = f"{PREFIX}/historical-lock-recovery.json"
FIRST = "iommu-platform/out/first-pte-paired-local-20261016"
DISCOVERY = "iommu-platform/out/problem-discovery-run-20261017"
PLRU = "iommu-platform/out/plru-ablation-20261017"
SHA1 = re.compile(r"^[0-9a-f]{40}$")
READ_HASHES: dict[str, str] = {}
JSON_CACHE: dict[str, object] = {}
GIT_CHECKS: dict[tuple[str, str], dict] = {}
LOCK_CHECKS: dict[str, dict] = {}


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def read(path: str) -> bytes:
    data = (ROOT / path).read_bytes()
    value = digest(data)
    if path in READ_HASHES:
        assert READ_HASHES[path] == value, f"Concurrent evidence change: {path}"
    READ_HASHES[path] = value
    return data


def load(path: str):
    if path not in JSON_CACHE:
        JSON_CACHE[path] = json.loads(read(path))
    return JSON_CACHE[path]


def ref(path: str, pointer: str | None = None, line: int | None = None) -> dict:
    if path not in READ_HASHES:
        read(path)
    result = {"path": path, "sha256": READ_HASHES[path]}
    if pointer is not None:
        result["json_pointer"] = pointer
    if line is not None:
        result["line"] = line
    return result


def esc(value: str) -> str:
    return value.replace("~", "~0").replace("/", "~1")


def local_path(value: str) -> str | None:
    path = Path(value)
    if path.is_absolute():
        try:
            return path.relative_to(ROOT).as_posix()
        except ValueError:
            return None
    return path.as_posix()


def git_exists(repository: str | None, commit: str) -> dict:
    assert SHA1.fullmatch(commit), commit
    if repository is None:
        return {"status": "not_checked_no_local_git_repository", "commit": commit}
    key = (repository, commit)
    if key not in GIT_CHECKS:
        result = subprocess.run(
            ["git", "--no-optional-locks", "-C", str(ROOT / repository),
             "cat-file", "-t", commit],
            capture_output=True, text=True, check=False,
        )
        GIT_CHECKS[key] = {
            "repository": repository, "commit": commit,
            "status": "commit_object_exists" if result.returncode == 0
            and result.stdout.strip() == "commit" else "commit_object_not_available_locally",
            "git_exit_code": result.returncode,
            "object_type": result.stdout.strip() or None,
            "does_not_prove": "historical checkout, branch, dirty bytes or execution",
        }
    return GIT_CHECKS[key]


def repository(component: str, path: str | None, role: str) -> dict:
    return {
        "component": component, "path": path, "identity_role": role,
        "checkout_branch": None, "checkout_commit": None,
        "selected_ref": None, "selected_commit": None, "dirty": None,
        "field_evidence": {},
        "known_unknowns": {
            "checkout_branch": "Historical branch was not recorded; current refs are not substituted.",
            "checkout_commit": "No checkout snapshot for this repository/event is recorded.",
            "selected_ref": "No separately recorded selected ref.",
            "selected_commit": "Actual selected bytes are not proven to be a clean commit tree.",
            "dirty": "No complete repository dirty-state snapshot for this event.",
        },
    }


def assign(repo: dict, field: str, value, evidence: dict, role: str | None = None):
    repo[field] = value
    repo["field_evidence"][field] = {"evidence": evidence, "role": role or repo["identity_role"]}
    if value is not None:
        repo["known_unknowns"].pop(field, None)


def verify_repo(repo: dict):
    commits = {repo[k] for k in ["checkout_commit", "selected_commit"] if repo[k]}
    repo["git_verification"] = [git_exists(repo["path"], sha) for sha in sorted(commits)]


def add_platform_snapshot(repos: list, path: str, data: dict, pointer: str, role: str):
    entry = repository("platform", "iommu-platform", role)
    assign(entry, "checkout_commit", data["platform_commit"], ref(path, pointer + "/platform_commit"))
    if "platform_dirty" in data:
        assign(entry, "dirty", data["platform_dirty"], ref(path, pointer + "/platform_dirty"),
               "recorded_platform_dirty; not_complete_component_or_overlay_capture")
    if "timestamp" in data:
        entry["captured_at"] = data["timestamp"]
        entry["field_evidence"]["captured_at"] = {"evidence": ref(path, pointer + "/timestamp"), "role": role}
    entry["known_unknowns"]["selected_commit"] = (
        "Platform checkout identifies the build record, not a complete selected source/overlay tree."
    )
    verify_repo(entry)
    repos.append(entry)


def add_lock(repos: list, manifest: str, data: dict, snapshots: dict):
    expected = data.get("sources_lock_sha256")
    if not expected or expected not in snapshots["by_sha256"]:
        return None
    recovered = snapshots["by_manifest"].get(manifest, snapshots["by_sha256"][expected])
    assert recovered["sha256"] == expected
    path = recovered["snapshot"]
    lock = load(path)
    assert digest(read(path)) == expected, path
    if path not in LOCK_CHECKS:
        commit = recovered["platform_commit"]
        result = subprocess.run(
            ["git", "--no-optional-locks", "-C", str(ROOT / "iommu-platform"),
             "show", f"{commit}:sources.lock.json"], capture_output=True, check=False,
        )
        assert result.returncode == 0 and digest(result.stdout) == expected, path
        LOCK_CHECKS[path] = {
            "repository": "iommu-platform", "commit": commit,
            "git_path": "sources.lock.json", "sha256": expected,
            "matches_preserved_snapshot": True,
        }
    for number, item in enumerate(lock["components"]):
        repo_path = "iommu-platform/" + item["path"]
        entry = repository(item["path"], repo_path, "configured_dependency_lock; checkout_and_build_bytes_not_proven")
        assign(entry, "selected_commit", item["commit"], ref(path, f"/components/{number}/commit"),
               "manifest_hash_bound_dependency_selection; dirty_overlay_separate")
        entry["upstream"] = item.get("upstream")
        entry["field_evidence"]["dependency_lock_binding"] = {
            "evidence": ref(manifest, "/sources_lock_sha256"),
            "lock": ref(path), "git_object": LOCK_CHECKS[path],
            "role": "exact_lock_sha_binding; not_actual_compile_byte_equivalence",
        }
        entry["known_unknowns"]["dirty"] = "Version lock records commits, not component dirty state or workspace overlay."
        verify_repo(entry)
        repos.append(entry)
    return {"manifest": ref(manifest, "/sources_lock_sha256"), "snapshot": ref(path),
            "git_object": LOCK_CHECKS[path], "components": len(lock["components"]),
            "semantics": "configured dependency selection only; no inferred clean checkout or source closure"}


def source_component(path: str) -> tuple[str, str]:
    if path.startswith("iommu-platform/cva6/corev_apu/riscv-iommu"):
        return "iommu", "iommu-platform/cva6/corev_apu/riscv-iommu"
    if path.startswith("iommu-platform/cva6"):
        return "cva6", "iommu-platform/cva6"
    if path.startswith("iommu-platform/linux"):
        return "linux", "iommu-platform/linux"
    return "platform", "iommu-platform"


def add_first_pte(repos: list):
    contract = f"{FIRST}/root/input-contract.json"
    data = load(contract)
    entry = repository("workspace_top", ".", "campaign_input_checkout; not_component_checkout_or_build_tree")
    assign(entry, "checkout_commit", data["input_head"], ref(contract, "/input_head"))
    assign(entry, "checkout_branch", data["branch"], ref(contract, "/branch"),
           "workspace_top_only; never_propagated_to_platform_or_components")
    verify_repo(entry)
    repos.append(entry)
    path = f"{FIRST}/m/README.md"
    lines = read(path).decode().splitlines()
    number = next(i + 1 for i, line in enumerate(lines) if "current component inputs" in line)
    line = lines[number - 1]
    for name, pattern, repo_path in [
        ("cva6", r"CVA6 HEAD `([0-9a-f]{40})`", "iommu-platform/cva6"),
        ("iommu", r"IOMMU HEAD `([0-9a-f]{40})`", "iommu-platform/cva6/corev_apu/riscv-iommu"),
    ]:
        entry = repository(name, repo_path, "campaign_overlay_authorship_input_HEAD; not_fresh_per_build_snapshot")
        assign(entry, "checkout_commit", re.search(pattern, line).group(1), ref(path, line=number))
        assign(entry, "dirty", True, ref(path, line=number), "existing_worktree_edits_explicitly_retained_in_copies")
        entry["known_unknowns"]["selected_commit"] = "Selected source mixes dirty working bytes and M/O/test overlays; use explicit build source hashes."
        verify_repo(entry)
        repos.append(entry)


def execution_status(layer: str) -> str:
    return {
        "source_change_only": "source_only; contemporaneous_build_and_run_unknown",
        "historical_table_only": "historical_run_reported; precise_execution_identity_unknown",
        "source_preservation_and_build": "build_recorded; board_execution_not_established_by_this_record",
        "board_run": "board_run_recorded; loaded_identity_strength_follows_original_receipts",
        "finite_RTL_behavioral_memory_run": "finite_RTL_behavioral_memory_run_recorded; no_board_claim",
        "software_build_and_layered_function_checks; no_kernel_or_DMA_runtime": "software_build_and_host_checks; kernel_and_DMA_runtime_not_performed",
        "source_contract_and_offline_dependency_check_only": "source_contract_and_offline_check; no_RTL_or_performance_run",
        "implemented_overlay_not_performance_run": "overlay_implemented; performance_run_not_performed",
        "build_preparation_failure": "preparation_failed; not_a_successful_runtime_result",
        "upstream_source_inspection_only": "source_inspected_only; no_run_against_selected_upstream",
        "configuration_inventory; run_status_separate": "configuration_inventory_only; attached_build_receipts_do_not_establish_profile_run",
    }[layer]


def main():
    index = load(INDEX)
    core = load(CORE)
    recovery = load(RECOVERY)
    recovered_locks = [row for row in recovery["records"] if row.get("matches_manifest")]
    snapshots = {
        "by_sha256": {row["sha256"]: row for row in recovered_locks},
        "by_manifest": {row["manifest"]: row for row in recovered_locks},
    }
    # Check all selected original metadata, not mutable current checkouts.
    for record in index["records"]:
        for item in record["source_manifests"] + record.get("overlay", []):
            if item.get("exists"):
                assert digest(read(item["path"])) == item["sha256"], item["path"]

    records = []
    for number, original in enumerate(index["records"]):
        rid = original["experiment_id"]
        row = {
            "experiment_id": rid, "date": original["date"],
            "evidence_layer": original["evidence_layer"],
            "execution_status": execution_status(original["evidence_layer"]),
            "original_index_record": ref(INDEX, f"/records/{number}"),
            "repositories": [], "build_inputs": [], "run_inputs": [], "overlays": [],
            "source_evidence": [], "known_unknowns": [],
            "organized_source_mapping": {
                "relation": "semantic_feature_family_association_only",
                "historical_source_equivalence": "not_asserted",
                "historical_execution_on_organized_commits": False,
                "feature_mapping": ref(INDEX, f"/records/{number}/feature_mapping"),
                "organized_commits": ref(INDEX, f"/records/{number}/new_commits"),
                "limit": original["mapping_limit"],
            },
        }
        repos = row["repositories"]
        manifests = [item["path"] for item in original["source_manifests"]]
        for path in manifests:
            role = "original_evidence; execution_strength_depends_on_record_layer"
            if "receipt" in path or path.endswith("/execution.json") or path.endswith("/run.json"):
                row["run_inputs"].append({"evidence": ref(path), "role": role})
            elif path.endswith("/result.json") or "acceptance" in path or "summary.json" in path:
                row["run_inputs"].append({"evidence": ref(path), "role": "result_or_acceptance_receipt; not_source_commit_selection"})
            else:
                row["source_evidence"].append({"evidence": ref(path), "role": role})

        # FPGA manifests record checkout at start/end and a hash-bound old lock.
        platform_manifests = []
        for path in manifests:
            if not path.endswith(".json"):
                continue
            data = load(path)
            if isinstance(data, dict) and "platform_commit" in data:
                platform_manifests.append(path)
        # Two board records omitted their retained stage build-inputs from the old
        # index; locate the exact stage named by their build evidence.
        extra = {
            "COMPOSITION-CLEAN-BOARD": "iommu-platform/out/composition-clean-20261006/fpga-src/build-inputs.json",
            "COMPOSITION-ISOLATED-BOARD": "iommu-platform/out/composition-isolated-20261007/fpga-src/build-inputs.json",
        }.get(rid)
        if extra:
            assert (ROOT / extra).is_file()
            platform_manifests.append(extra)
        if rid in ["PROFILE-migration-4dma-n-hit", "PROFILE-migration-4dma-n-serial"]:
            recovery_path = manifests[-1]
            extra = "iommu-platform/" + load(recovery_path)["source_inputs"]
            if (ROOT / extra).is_file():
                platform_manifests.append(extra)

        seen_locks = set()
        for path in platform_manifests:
            data = load(path)
            if data.get("build_start"):
                add_platform_snapshot(repos, path, data["build_start"], "/build_start", "build_start_checkout_snapshot")
            add_platform_snapshot(repos, path, data, "", "build_stage_snapshot" if path.endswith("build-inputs.json") else "manifest_end_checkout_snapshot")
            lock_binding = None
            if data.get("sources_lock_sha256") not in seen_locks:
                lock_binding = add_lock(repos, path, data, snapshots)
                seen_locks.add(data.get("sources_lock_sha256"))
            item = {"manifest": ref(path), "role": "historical_FPGA_build_metadata; not_complete_compiled_input_hash_list",
                    "dependency_lock": lock_binding,
                    "selected_source_hashes": ref(path, "/workspace_overlay") if "workspace_overlay" in data else None,
                    "build_start": ref(path, "/build_start") if "build_start" in data else None,
                    "artifacts": ref(path, "/artifacts") if "artifacts" in data else None,
                    "full_dependency_byte_closure": "unknown"}
            row["build_inputs"].append(item)
            if "workspace_overlay" in data:
                row["overlays"].append({"evidence": ref(path, "/workspace_overlay"),
                                        "role": "historical_stage_overlay_path_and_hash_list; separate_from_lock_commits"})

        if rid.startswith("SOURCE-"):
            path = manifests[0]
            commit = original["input_commits"][0]["commit"]
            entries = load(path)["commits"]
            i = next(i for i, item in enumerate(entries) if item["commit"] == commit)
            entry = repository(entries[i]["repository"].split("/")[-1], entries[i]["repository"], "historical_source_change_inspected; no_execution_claim")
            assign(entry, "selected_commit", commit, ref(path, f"/commits/{i}/commit"))
            entry["known_unknowns"]["checkout_commit"] = "Source-change Git object is known; no contemporaneous experiment checkout snapshot exists."
            verify_repo(entry)
            repos.append(entry)

        if rid == "M0-FULL-COPY":
            path = manifests[0]
            data = load(path)
            for component, repo_path, field in [("platform", "iommu-platform", "platform_head"), ("cva6", "iommu-platform/cva6", "cva6_head")]:
                entry = repository(component, repo_path, "source_provenance_checkout_HEAD; actual_source_hashes_recorded_separately")
                assign(entry, "checkout_commit", data[field], ref(path, "/" + field))
                if component == "platform":
                    assign(entry, "dirty", True, ref(path, "/after"), "changed_and_new_test_sources; not_complete_dirty_manifest")
                verify_repo(entry)
                repos.append(entry)
            row["overlays"].append({"evidence": ref(path, "/after"), "role": "changed_test_and_analysis_source_hashes; not_full_build_closure"})

        if rid.startswith("FIRST-PTE-"):
            add_first_pte(repos)
            for path, pointer, role in [
                (f"{FIRST}/m/manifest.json", "/files", "M_overlay_original_and_replacement_hashes"),
                (f"{FIRST}/o/base-manifest.json", "", "O_overlay_base_and_replacement_hashes"),
                (f"{FIRST}/combined/assembly.json", "/origins", "assembled_overlay_source_origins"),
                (f"{FIRST}/runner/cpp-provenance.json", "", "CPP_fixture_input_and_output_hashes"),
            ]:
                row["overlays"].append({"evidence": ref(path, pointer), "role": role,
                                        "scope": "campaign_source_asset; per_build hashes govern actual selected revision"})

        if rid.startswith("DISCOVERY-"):
            path = f"{DISCOVERY}/input-version.json"
            for i, item in enumerate(load(path)["components"]):
                repo_path = item["path"]
                name = "workspace_top" if repo_path == "." else repo_path.removeprefix("iommu-platform/")
                actual_repo = "iommu-platform/cva6" if repo_path.endswith("/iDMA") else repo_path
                entry = repository(name, actual_repo, "pre_execution_input_checkout_snapshot")
                if repo_path != actual_repo:
                    entry["source_subdirectory"] = repo_path
                    entry["identity_role"] = "iDMA_directory_is_part_of_CVA6_git_repository; not_separate_iDMA_commit"
                assign(entry, "checkout_commit", item["head"], ref(path, f"/components/{i}/head"))
                if "tracked_status" in item:
                    assign(entry, "dirty", bool(item["tracked_status"].strip()), ref(path, f"/components/{i}/tracked_status"), "tracked_status_only; untracked_file_closure_unknown")
                verify_repo(entry)
                repos.append(entry)

        if rid.startswith("PLRU-"):
            path = f"{PLRU}/variants/source-identity.json"
            data = load(path)
            entry = repository("iommu", "iommu-platform/cva6/corev_apu/riscv-iommu", "variant_source_identity_component_HEAD; not_clean_selected_source")
            assign(entry, "checkout_commit", data["component_head"], ref(path, "/component_head"))
            entry["known_unknowns"]["dirty"] = "component_head records history, while original/variant RTL hashes identify changed selected bytes; full repository status was not captured."
            verify_repo(entry)
            repos.append(entry)
            variant = rid.split("-")[1]
            row["overlays"].append({"evidence": ref(path, "/variants/" + variant), "role": "selected_IOTLB_variant_path_and_sha256"})

        if rid == "A2-SVA-SOFTWARE":
            path = "iommu-platform/out/a2-sva-implementation-20261015/kernel/build-lock.json"
            data = load(path)
            entry = repository("linux", "iommu-platform/linux", "recorded_Linux_base_plus_full_tracked_patch_and_untracked_manifest")
            assign(entry, "selected_commit", data["Linux"]["base_commit"], ref(path, "/Linux/base_commit"), "working_tree_base_only; exact_built_source_also_requires_patch_and_untracked_files")
            assign(entry, "dirty", True, ref(path, "/Linux/full_tracked_overlay"), "full_working_tree_patch_and_untracked_source_manifest_recorded")
            verify_repo(entry)
            repos.append(entry)
            patch = str(Path(path).parent / data["Linux"]["full_tracked_overlay"])
            assert digest(read(patch)) == data["Linux"]["overlay_sha256"]
            row["overlays"].extend([
                {"evidence": ref(patch), "role": "Linux_full_tracked_working_tree_patch"},
                {"evidence": ref(path, "/Linux/untracked_manifest"), "role": "Linux_untracked_input_source_hashes"},
                {"evidence": ref(manifests[1]), "role": "production_source_before_and_after_hashes"},
                {"evidence": ref(manifests[4]), "role": "isolated_HERO_patch_source_hashes"},
            ])
            row["build_inputs"].append({"manifest": ref(path), "role": "Linux_software_build_only; not_loaded",
                                        "outputs": ref(manifests[2]), "full_dependency_byte_closure": "not_claimed"})
            hero_path = manifests[3]
            for name, source in load(hero_path)["fixed_sources"].items():
                entry = repository("HERO-" + name, None, "isolated_HERO_fixed_source; per_component_execution_requires_original_build_record")
                entry["upstream"] = source["repository"]
                assign(entry, "selected_commit", source["commit"], ref(hero_path, "/fixed_sources/" + name + "/commit"))
                verify_repo(entry)
                repos.append(entry)
            row["source_evidence"].append({"evidence": ref(hero_path, "/archives"), "role": "isolated_source_archive_hashes; not_native_paper_snapshot_or_hardware_execution"})

        if rid == "A5-WAIT-SCOPE":
            path = manifests[0]
            for key, source in load(path)["sources"].items():
                entry = repository(source["repo"], None, "external_source_inspected_for_contract; not_executed_RTL")
                assign(entry, "selected_ref", source["ref"], ref(path, f"/sources/{key}/ref"))
                assign(entry, "selected_commit", source["ref"], ref(path, f"/sources/{key}/ref"), "exact_source_ref; not_checkout_or_experiment_commit")
                entry["source_file"] = source["repo_path"]
                source_path = local_path(source["path"])
                assert source_path and digest(read(source_path)) == source["sha256"]
                entry["field_evidence"]["selected_source_bytes"] = {"evidence": ref(source_path), "role": "inspected_source_file_bytes"}
                verify_repo(entry)
                repos.append(entry)

        if rid == "UPSTREAM-IOMMU-READONLY-20261017":
            path = manifests[0]
            data = load(path)
            entry = repository("upstream-iommu", "iommu-platform/cva6/corev_apu/riscv-iommu", "public_raw_source_inspection_only; not_local_checkout_or_runtime")
            assign(entry, "selected_commit", data["commit"], ref(path, "/commit"))
            entry["observed_remote_default_branch"] = data["default_branch"]
            entry["field_evidence"]["observed_remote_default_branch"] = {"evidence": ref(path, "/default_branch"), "role": "remote_default_branch_only; not_local_checkout_branch"}
            verify_repo(entry)
            repos.append(entry)
            for i, item in enumerate(load(manifests[1])["repositories"]):
                entry = repository(item["path"], item["path"], "local_comparison_source_checkout_at_inspection; no_execution_claim")
                assign(entry, "checkout_commit", item["head"], ref(manifests[1], f"/repositories/{i}/head"))
                verify_repo(entry)
                repos.append(entry)

        # Preserve the exact old index's explicit build mapping, including failed
        # preparations. Full lists remain in core-build-sources rather than copied.
        for path in manifests:
            if path in core["builds"]:
                build = core["builds"][path]
                missing = []
                for i, source in enumerate(build["explicit_sources"]):
                    blob = source.get("source_blob")
                    if not blob:
                        missing.append({"source_index": i, "recorded_source_path": source["path"],
                                        "expected_sha256": source["sha256"],
                                        "reason": "recorded path was overwritten before collection; expected byte snapshot absent",
                                        "evidence": ref(CORE, "/builds/" + esc(path) + f"/explicit_sources/{i}")})
                row["build_inputs"].append({
                    "manifest": ref(path), "role": "explicit_compiled_source_manifest" if original["evidence_layer"] != "build_preparation_failure" else "failed_build_preparation_input_manifest",
                    "source_mapping": ref(CORE, "/builds/" + esc(path)),
                    "explicit_source_count": len(build["explicit_sources"]),
                    "explicit_source_hash_counts": build["counts"],
                    "missing_expected_source_bytes": missing,
                    "full_dependency_byte_closure": "unknown; includes_environment_generated_inputs_and_tools_not_fully_byte_locked",
                })
                if load(path).get("overlay"):
                    row["overlays"].append({"evidence": ref(path, "/overlay"), "role": "actual_build_overlay_directory; exact_inputs_in_source_mapping"})
                for field in ["argv", "includes", "vendor_replacements", "source_differences"]:
                    if field in load(path):
                        row["build_inputs"][-1][field] = ref(path, "/" + field)

        for item in original.get("overlay", []):
            row["overlays"].append({"evidence": ref(item["path"]), "role": "historical_patch_or_overlay; execution_binding_follows_original_manifest"})
        if rid in ["COMPOSITION-CLEAN-BOARD", "COMPOSITION-ISOLATED-BOARD"]:
            row["overlays"].append({"evidence": ref(manifests[0]), "role": "selected_FPGA_overlay_source_hash_map"})
        if rid == "ACTUAL-DMA-SYSTEM-PN":
            row["overlays"].append({"evidence": ref(manifests[0]), "role": "selected_RTL_sources_equal_isolated_BIT_snapshot; not_git_commit_equivalence"})
        if rid == "SHARED-READ-RTL":
            row["build_inputs"].append({"manifest": ref(manifests[0], "/final_sha256"),
                                        "role": "selected_final_source_and_executable_hashes; not_complete_build_inputs",
                                        "version_mapping": ref(manifests[0], "/version_mapping"),
                                        "argv": ref("iommu-platform/out/shared-read-rtl-20261009/build-argv.json"),
                                        "full_dependency_byte_closure": "unknown"})

        # Show explicit unknown identities for relevant participating components
        # even where original evidence has only selected file hashes.
        required = []
        if not rid.startswith("SOURCE-") and rid not in ["A2-SVA-SOFTWARE", "A5-WAIT-SCOPE", "UPSTREAM-IOMMU-READONLY-20261017"]:
            required = [("platform", "iommu-platform"), ("cva6", "iommu-platform/cva6"),
                        ("iommu", "iommu-platform/cva6/corev_apu/riscv-iommu")]
            if original["evidence_layer"] in ["board_run", "historical_table_only", "configuration_inventory; run_status_separate", "source_preservation_and_build"]:
                required += [("linux", "iommu-platform/linux"), ("opensbi", "iommu-platform/opensbi")]
        for component, path in required:
            if not any(entry["path"] == path for entry in repos):
                entry = repository(component, path, "identity_not_recorded_in_selected_original_evidence")
                entry["field_evidence"]["scope_of_search"] = {"evidence": row["original_index_record"], "role": "record_original_manifest_allowlist; no_current_ref_substitution"}
                verify_repo(entry)
                repos.append(entry)

        row["known_unknowns"] = [
            {"field": "repositories.checkout_branch", "reason": "Unrecorded historical component branches remain null. Branch membership/aliases today cannot recover a past checkout."},
            {"field": "build_inputs.full_dependency_byte_closure", "reason": "Selected file hashes and configured commits do not establish complete historical includes, untracked files, generated inputs, tools or environment."},
            {"field": "organized_source_mapping", "reason": "Later feature commits are navigation associations only; they are excluded from historical checkout and selected source fields."},
        ]
        row["inherited_unknowns"] = original.get("unknowns", [])
        if not row["build_inputs"]:
            row["known_unknowns"].append({"field": "build_inputs", "reason": "No complete per-build input manifest is bound to this record; source/config-only records do not imply an unrecorded execution."})
        records.append(row)

    # Ensure every former input-commit is either recovered with original evidence,
    # or retained as a qualified historical association, never silently dropped.
    for i, (old, row) in enumerate(zip(index["records"], records)):
        found = {entry[k] for entry in row["repositories"] for k in ["checkout_commit", "selected_commit"] if entry[k]}
        row["unpromoted_index_commit_associations"] = [
            {"commit": item.get("commit", item.get("head")),
             "component_or_path": item.get("component", item.get("path")),
             "role": "old_index_association; no_additional_original_field_binding_found; not_promoted_to_execution_source",
             "evidence": ref(INDEX, f"/records/{i}/input_commits/{j}")}
            for j, item in enumerate(old.get("input_commits", []))
            if item.get("commit", item.get("head")) not in found
        ]

    unique_builds = core["builds"]
    absent = [source for build in unique_builds.values() for source in build["explicit_sources"] if not source.get("source_blob")]
    summary = {
        "record_count": len(records),
        "by_evidence_layer": dict(Counter(row["evidence_layer"] for row in records)),
        "records_with_any_recorded_checkout_branch": sum(any(entry["checkout_branch"] is not None for entry in row["repositories"]) for row in records),
        "records_with_recorded_component_checkout_branch": sum(any(entry["checkout_branch"] is not None and entry["path"] != "." for entry in row["repositories"]) for row in records),
        "records_with_any_checkout_commit": sum(any(entry["checkout_commit"] for entry in row["repositories"]) for row in records),
        "records_with_any_selected_commit": sum(any(entry["selected_commit"] for entry in row["repositories"]) for row in records),
        "records_with_any_historical_commit_role": sum(any(entry["selected_commit"] or entry["checkout_commit"] for entry in row["repositories"]) for row in records),
        "records_with_explicit_build_source_mapping": sum(any("source_mapping" in b for b in row["build_inputs"]) for row in records),
        "existing_core_builds": len(unique_builds),
        "existing_core_explicit_input_occurrences": sum(len(b["explicit_sources"]) for b in unique_builds.values()),
        "core_missing_expected_byte_occurrences": len(absent),
        "core_missing_expected_byte_unique_sha256": sorted({s["sha256"] for s in absent}),
        "lock_snapshots_rechecked_against_git_objects": len(LOCK_CHECKS),
        "git_commit_checks": len(GIT_CHECKS),
        "git_commit_check_status": dict(Counter(c["status"] for c in GIT_CHECKS.values())),
        "unpromoted_index_commit_associations": sum(len(r["unpromoted_index_commit_associations"]) for r in records),
    }
    output = {
        "schema": "historical-execution-identities-v1",
        "provenance": {"date": "2026-10-17", "authorship": "Codex only; no DeepSeek or Claude",
                       "method": "field-level backfill from original manifests, preserved source/lock evidence and read-only Git object checks"},
        "scope": {"new_experiments": 0, "new_builds": 0, "new_reductions": 0, "board_operations": 0,
                  "original_evidence_modified": False, "writes": ["historical-identities.json", "historical-identities.csv"]},
        "null_semantics": {
            "default": "not recorded or not proven at the stated identity role/event; not an empty branch, clean tree or missing repository",
            "checkout_branch": "historical checkout branch unrecorded; detached HEAD is not inferred",
            "checkout_commit": "no direct checkout snapshot; selected/locked/source-change commits remain separate",
            "selected_ref": "no separately recorded selected ref; a recorded checkout branch or remote default branch is not substituted",
            "selected_commit": "no selected commit proved at this entry's identity_role; non-null lock/base/inspection commits name only that role, never the complete executed source tree",
            "dirty": "dirty-state scope not recorded; false only when original evidence explicitly says so",
            "build_inputs_empty": "no per-build record in this evidence scope; does not establish build/run absence unless execution_status explicitly says not performed",
        },
        "inputs": [ref(INDEX), ref(CORE), ref(RECOVERY)],
        "records": records, "summary": summary,
        "validation": {"original_read_files": [ref(path) for path in sorted(READ_HASHES)],
                       "read_originals_unchanged_during_generation": True,
                       "git_checks": list(GIT_CHECKS.values()),
                       "git_lock_object_checks": list(LOCK_CHECKS.values()),
                       "limits": "static evidence inspection only; no rerun, rebuild, result reduction or behavioral revalidation"},
    }
    for path, expected in READ_HASHES.items():
        assert digest((ROOT / path).read_bytes()) == expected, f"Original changed: {path}"
    target = HERE / "historical-identities.json"
    target.write_text(json.dumps(output, ensure_ascii=False, indent=2) + "\n")
    fields = ["experiment_id", "date", "evidence_layer", "execution_status", "checkout_branches", "checkout_commits", "selected_refs", "selected_commits", "repositories", "build_inputs", "run_inputs", "overlays", "known_unknowns", "organized_source_mapping"]
    with (HERE / "historical-identities.csv").open("w", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        for row in records:
            csv_row = {key: row[key] for key in fields if key in row}
            for plural, singular in [("checkout_branches", "checkout_branch"), ("checkout_commits", "checkout_commit"), ("selected_refs", "selected_ref"), ("selected_commits", "selected_commit")]:
                csv_row[plural] = [{"component": entry["component"], "path": entry["path"], "role": entry["identity_role"], "value": entry[singular]} for entry in row["repositories"] if entry[singular] is not None]
            for key, value in csv_row.items():
                if isinstance(value, (dict, list)):
                    csv_row[key] = json.dumps(value, ensure_ascii=False, separators=(",", ":"))
            writer.writerow(csv_row)
    print(json.dumps(summary, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
