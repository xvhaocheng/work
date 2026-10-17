#!/usr/bin/env python3
"""Index existing metadata and compare explicit source hashes; never run experiments.

Writes only beside this script. The selected metadata paths are deliberate;
this program does not walk source/build trees or execute their tools.
"""
from __future__ import annotations

import csv
import hashlib
import json
import shutil
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
PLATFORM = ROOT / "iommu-platform"
OUT = PLATFORM / "out"
CAPTURED = datetime.now(timezone.utc).isoformat()
FILES: dict[str, dict] = {}
ROWS: list[dict] = []
BUILDS: dict[str, dict] = {}


def rel(p: Path | str) -> str:
    p = Path(p)
    if not p.is_absolute():
        p = ROOT / p
    p = p.resolve()
    try:
        return p.relative_to(ROOT).as_posix()
    except ValueError:
        return str(p)


def sha(p: Path) -> str | None:
    if not p.is_file():
        return None
    h = hashlib.sha256()
    with p.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def ref(path: str | Path, role="metadata", snapshot=True) -> dict:
    p = Path(path)
    if not p.is_absolute():
        p = ROOT / p
    key = rel(p)
    if key not in FILES:
        item = {"path": key, "exists": p.is_file(), "sha256": sha(p), "roles": [role]}
        if p.is_file():
            item["bytes"] = p.stat().st_size
            # Copy only small metadata/report originals, never executables or traces.
            if snapshot and p.stat().st_size < 500_000 and p.suffix in {".json", ".md", ".patch", ".txt"}:
                target = HERE / "metadata" / key
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(p, target)
                item["snapshot"] = rel(target)
        FILES[key] = item
    elif role not in FILES[key]["roles"]:
        FILES[key]["roles"].append(role)
    return {k: FILES[key][k] for k in ["path", "exists", "sha256"]}


def load(path: str | Path):
    p = Path(path)
    if not p.is_absolute():
        p = ROOT / p
    ref(p)
    return json.loads(p.read_text())


def feature(*ids):
    return [{"component": c, "feature_id": f} for c, f in (x.split(":", 1) for x in ids)]


COMMON = feature("cva6:CVA6-INTEGRATION", "cva6:CVA6-SC-FIX", "cva6:CVA6-DS-DELAY", "iommu:A1", "iommu:F1", "iommu:F2")
COMPOSITION = COMMON + feature("cva6:CVA6-FULLBUFFER-CERT", "cva6:CVA6-FULLBUFFER-RAM", "cva6:CVA6-NONBLOCK-OBS", "cva6:CVA6-COMPOSITION-CONTROL", "cva6:CVA6-BANKED-PN-WIRING", "iommu:F3", "iommu:F4", "iommu:O1", "iommu:B1", "iommu:N1", "iommu:P1", "iommu:P2")


def row(experiment_id, date, layer, report, output_dir, metadata=(), config=None,
        classification="historical_research_platform_not_stock_certified",
        reconstruction="partial_metadata_only", features=(), notes=(), commits=None,
        overlays=(), evidence_lines=()):
    r = {
        "experiment_id": experiment_id,
        "date": date,
        "evidence_layer": layer,
        "report": ref(report, "report"),
        "report_line_anchors": list(evidence_lines),
        "output_dir": rel(output_dir),
        "input_commits": commits or [],
        "source_manifests": [ref(p, "source_or_execution_metadata") for p in metadata],
        "overlay": [ref(p, "overlay") for p in overlays],
        "config": config or {"status": "unknown"},
        "known_extension_flags": {},
        "baseline_classification": classification,
        "stock_baseline_completed": False,
        "reconstruction_status": reconstruction,
        "feature_mapping": list(features),
        "new_commits": [],
        "new_commit_mapping_status": "pending_component_manifests; semantic_association_only",
        "mapping_limit": "Feature association is not byte-equivalence, a full dependency closure, historical enablement proof, or proof of execution on an organized commit.",
        "unknowns": [],
        "notes": list(notes),
    }
    ROWS.append(r)
    return r


def attach_fpga(r, p):
    j = load(p)
    r["build_identity"] = {
        k: j[k] for k in ["timestamp", "action", "status", "platform_commit", "platform_dirty", "sources_lock_sha256", "build_start", "profile", "tools", "artifacts"] if k in j
    }
    for stage, commit in [("manifest_end", j.get("platform_commit")), ("build_start", j.get("build_start", {}).get("platform_commit"))]:
        if commit:
            r["input_commits"].append({"component": "iommu-platform", "commit": commit, "role": stage, "dirty": j.get("platform_dirty", "unknown")})
    recovery_path=HERE/"historical-lock-recovery.json"
    if recovery_path.is_file():
        recovery=load(recovery_path)
        lock_entry=next((x for x in recovery["records"] if x["manifest"]==rel(p)),None)
        if lock_entry and lock_entry.get("matches_manifest"):
            sl=ROOT/lock_entry["snapshot"]
            sj=load(sl)
            r["source_manifests"].append(ref(sl,"historical_git_source_lock_matches_manifest_sha256"))
            r["historical_source_lock"]={"file":ref(sl),"git_object":lock_entry["git_object"],"matches_manifest_sha256":True}
            r["input_commits"] += [{"component":x["path"],"commit":x["commit"],"role":"historical_git_lock_sha_verified; dirty_overlay_separate"} for x in sj.get("components",[]) if x["path"] in {"cva6","cva6/corev_apu/riscv-iommu","linux","opensbi"}]
    r["unknowns"] += ["Full build-time dirty source tree and every include/header hash are not reconstructed from this FPGA manifest.", "A passed build alone does not establish loaded board identity or a historical performance baseline."]
    return j


def logical_path(s: str) -> str | None:
    if "/overlay/" in s:
        return s.split("/overlay/", 1)[1]
    p = Path(s)
    if p.is_absolute():
        try:
            return str(p.relative_to(PLATFORM))
        except ValueError:
            return None
    return s.removeprefix("iommu-platform/")


def build_detail(p: Path) -> dict:
    key = rel(p)
    if key in BUILDS:
        return BUILDS[key]
    j = load(p)
    entries = []
    explicit = list(j.get("sources", []))
    if isinstance(j.get("cpp"), dict):
        explicit.append(dict(j["cpp"], role="cpp"))
    for e in explicit:
        lp = "tests/nonblock_actual_dma_test.cpp" if e.get("role") == "cpp" else e.get("logical_path") or logical_path(e["path"])
        actual = Path(e["path"])
        if not actual.is_absolute():
            actual = PLATFORM / actual
        current = PLATFORM / lp if lp is not None else None
        actual_sha = sha(actual)
        current_sha = sha(current) if current is not None else None
        item = dict(e)
        item.update({
            "logical_path": lp,
            "recorded_path_exists_at_index": actual.is_file(),
            "recorded_path_sha256_at_index": actual_sha,
            "recorded_bytes_match_at_index": actual_sha == e.get("sha256"),
            "current_workspace_base_path": rel(current) if current is not None else None,
            "current_workspace_base_sha256_at_index": current_sha,
            "matches_current_workspace_base_at_index": current_sha == e.get("sha256"),
            "difference_origin": "historical_overlay_or_test_cpp" if ("/overlay/" in str(actual) or e.get("role") == "cpp") else "explicit_source_path; compare hash without inferring historical enablement",
        })
        if actual_sha == e.get("sha256"):
            blob = HERE / "source-blobs" / (actual_sha + actual.suffix)
            blob.parent.mkdir(parents=True, exist_ok=True)
            if not blob.exists():
                shutil.copyfile(actual, blob)
            item["source_blob"] = rel(blob)
        else:
            item["source_blob"] = None
            item["missing_recorded_bytes"] = "Only the recorded hash is retained here; do not substitute current revised bytes."
        entries.append(item)
    detail = {
        "manifest": ref(p, "build_manifest"),
        "captured_utc": CAPTURED,
        "metadata": {k: j[k] for k in ["schema", "state", "exit_code", "policy", "variant", "top", "overlay", "shared_overlay", "argv", "includes", "vendor_replacements", "baseline_manifest", "source_differences", "unmapped_baseline_sources", "executable", "started_unix", "ended_unix"] if k in j},
        "explicit_sources": entries,
        "counts": {"explicit_sv": len(j.get("sources", [])), "cpp": int(isinstance(j.get("cpp"), dict)), "recorded_path_hash_match": sum(x["recorded_bytes_match_at_index"] for x in entries), "recorded_path_hash_mismatch": sum(not x["recorded_bytes_match_at_index"] and x["recorded_path_exists_at_index"] for x in entries), "recorded_path_missing": sum(not x["recorded_path_exists_at_index"] for x in entries), "current_workspace_base_hash_match": sum(x["matches_current_workspace_base_at_index"] for x in entries)},
        "coverage": "All explicitly enumerated SV inputs and CPP are preserved as recorded hashes. Include directories, environment, generated compiler inputs and external tools are not a proven complete byte-locked dependency closure. No rebuild/replay was performed.",
    }
    BUILDS[key] = detail
    return detail


def main():
    history = OUT / "research-progress-from-july-20261014/evidence.json"
    h = load(history)
    # Dated source changes, deliberately not historical experiment runs.
    for short, date, why in [("0cb9874", "2026-07-09", "second DMA DTS and length ioctl"), ("3e409bad", "2026-07-11", "parent CVA6 pointer to June29 AW fix"), ("48185ad", "2026-07-26", "Pair/Multi ioctl and four DTS nodes; title says three")]:
        candidates = [x for x in h["commits"] if x["commit"].startswith(short)]
        r = row("SOURCE-" + short, date, "source_change_only", PLATFORM / "docs/research-progress-from-july-20261014.md", history.parent,
                [history], config={"scope": why, "historical_compile_and_runtime": "unknown"},
                classification="early_platform_extension; no_stock_or_board_claim", reconstruction="dated_git_source_only; experiment_identity_unknown",
                features=feature("iommu:F1") if short=="3e409bad" else [], commits=[{"component": x["repository"], "commit": x["commit"], "author_date": x["author_date"]} for x in candidates], evidence_lines=[19,20,21],
                notes=["Git source changes do not prove which board/software image ran. June29 AW fix must not be redated as July11 implementation."])
        r["unknowns"] = ["Exact run argv, dirty/overlay source, image/load identity and output data for any contemporaneous run are unknown."]
    row("EARLY-2DMA-BARRIER", "before-2026-09-08; exact_dates_unknown", "historical_table_only", ROOT / "研究总览_2026-09-08.md", ROOT / "platform-archives/legacy-20260908", [history, PLATFORM / "provenance/migration.json"], config={"mode": "2DMA Barrier", "version_argv_raw_logs": "unknown"}, reconstruction="not_reconstructed", features=feature("cva6:CVA6-INTEGRATION"), notes=["Motivating table is not proof of translation blocking; timing was a shared window, not independent completion."])
    migration=row("MIGRATION-20260908", "2026-09-08", "source_preservation_and_build", PLATFORM / "docs/patches.md", PLATFORM / "provenance", [PLATFORM / "provenance/migration.json", PLATFORM / "out/manifests/fpga-migration-4dma.json"], config={"num_dma":4,"device_ids":[10,11,12,13],"ds_delay_input":50,"ds_delay_output":50}, reconstruction="historical_lock_and_migration_patches_available; not_historical_board_identity", features=COMMON, overlays=[PLATFORM / "provenance/cva6-working.patch", PLATFORM / "provenance/cva6__corev_apu__riscv-iommu-working.patch", PLATFORM / "provenance/linux-working.patch"])
    attach_fpga(migration,PLATFORM/"out/manifests/fpga-migration-4dma.json")
    board_specs = [
        ("E1", "2026-09-16", "e1-results-review.md", "mac-e1-stage-20260916-032650-run-rbo67pvd", "migration-4dma-scfix", ["cva6:CVA6-ACTIVE-CYCLES"], {"kernel":"6.5.0-rc2-00080-g3b47ab1645e4","chunk":4096,"transfers":4096,"warmup":256,"roles":"DMA0/1 exchange"}),
        ("E4", "2026-09-16", "e4-results-review.md", "mac-e4-stage-20260916-094001-run-kzi_s0zc", "migration-4dma-observe", ["cva6:CVA6-LIFECYCLE","iommu:O1"], {"observer":"E4 passive timing/AXI","paired_toggle":"36 records; scope per report"}),
        ("BATCH-DS50", "2026-09-16", "batch-results-review.md", "e4-batch-20260916", "migration-4dma-batch", ["cva6:CVA6-BATCH","cva6:CVA6-LIFECYCLE"], {"supply":"autonomous finite batch; 4KiB page ring","DS":[50,50]}),
        ("BATCH-DS0", "2026-09-16", "ds0-batch-results-review.md", "out/manifests", "baseline-4dma-batch", ["cva6:CVA6-BATCH"], {"supply":"same finite batch protocol","DS":[0,0]}),
        ("BATCH-DS50-RETEST", "2026-09-16", "ds50-retest-results-review.md", "e4-batch-20260916", "migration-4dma-batch", ["cva6:CVA6-BATCH"], {"DS":[50,50],"role":"switchback"}),
        ("PATH", "2026-09-16", "path-results-review.md", "e4-path-20260916", "migration-4dma-path", ["cva6:CVA6-BATCH","cva6:CVA6-PATH"], {"observer":"v3 four-stage transaction path","DS":[50,50]}),
        ("E3-V4", "2026-09-17", "e3-results-review.md", "mac-e3-main-20260917-222837-run-p0lbvbv7", "migration-4dma-e3", ["cva6:CVA6-E3-ACTIVE","iommu:E1"], {"DS":[50,50],"response_delay_pair":[0,128],"validity":"trigger/history limitations remain"}),
        ("E3-V5", "2026-09-18", "e3-queue-results-review.md", "mac-e3-queue-20260918-130545-run-d01lc9wp", "migration-4dma-e3-queue", ["cva6:CVA6-E3-ACTIVE","iommu:E1"], {"DS":[50,50],"response_delay_pair":[0,128],"validity":"queue identity/history/truncation limitations remain"}),
        ("E3-V6-CONTROLLED", "2026-09-18", "e3-controlled-results-review.md", "mac-e3-controlled-20260918-164635-run-rk48bhte", "migration-4dma-e3-controlled", ["cva6:CVA6-E3-ACTIVE","cva6:CVA6-HELD-LAUNCH","iommu:E1"], {"DS":[50,50],"bytes":64,"response_delay_pair":[0,128],"short_pairs":3,"formal_pairs":8}),
        ("E2-V2", "2026-10-03", "../out/e2-results-review-20261003/RESULTS.md", "mac-research-two-bit-20261003/e2", "migration-4dma-research-e2-v2", ["cva6:CVA6-E2-COST","cva6:CVA6-LOG-RAM"], {"DS":[50,50],"research_role":1,"records":64,"iotlb_entries":16,"W7_W8":"layout-specific replacement boundary"}),
        ("P4M-FORMAL60", "2026-09-20..2026-10-02", "../out/p4m-complete-review-20261002/RESULTS.md", "p4m-complete-review-20261002", "migration-4dma-p4m", ["cva6:CVA6-P4M-GAP"], {"DS":[50,50],"policy":"fixed A submission gap","records":60,"online_pass":53,"offline_accepted":7}),
        ("METHOD88-P5", "2026-10-04", "../out/method88-execution-20261004/RESULTS.md", "method88-execution-20261004", "migration-4dma-research-method-v2", ["cva6:CVA6-RESEARCH-POLICY","cva6:CVA6-COST-AGGREGATE"], {"DS":[50,50],"research_role":2,"A_pages":8,"target":170,"policy":"p5","startups":2,"directions":2}),
    ]
    for eid,date,report,odir,profile,ids,conf in board_specs:
        m = OUT / "manifests" / ("fpga-" + profile + ".json")
        output = PLATFORM / odir if odir.startswith("out/") else OUT / odir
        metas = [m]
        for extra in [output/"server-review.json", output/"summary.json", output/"run.json"]:
            if extra.is_file(): metas.append(extra)
        if eid == "E2-V2": metas += [OUT / "mac-research-two-bit-20261003/DELIVERY.json", OUT / "e2-results-review-20261003/input-sha256-after.json"]
        r = row(eid,date,"board_run",PLATFORM/"docs"/report,output,metas,conf,features=COMMON+feature(*ids))
        attach_fpga(r,m)
        deliveries = {"E4":"e4-observer-20260916", "BATCH-DS50":"e4-batch-20260916", "BATCH-DS50-RETEST":"e4-batch-20260916", "PATH":"e4-path-20260916", "E3-V4":"e3-service-20260916", "E3-V5":"e3-followup-20260918", "E3-V6-CONTROLLED":"e3-controlled-20260918"}
        delivery = deliveries.get(eid)
        if delivery:
            sl = OUT / delivery / "delivery/sources.lock.json"
            if sl.is_file():
                sj = load(sl)
                r["source_manifests"].append(ref(sl,"historical_delivery_source_lock"))
                r["historical_delivery_lock_matches_fpga_manifest"] = sha(sl) == r["build_identity"].get("sources_lock_sha256")
                r["input_commits"] += [{"component":x["path"],"commit":x["commit"],"role":"historical_delivery_lock; dirty overlay still separate"} for x in sj.get("components",[]) if x["path"] in {"cva6","cva6/corev_apu/riscv-iommu","linux","opensbi"}]
        r["config"]["profile_name"] = profile
        r["known_extension_flags"] = {"injected_DS_delay": conf.get("DS",[50,50]),"E3_active_delay": eid.startswith("E3-"),"P_N_PN_enablement": "not_certified_by_profile_name"}
        if eid == "METHOD88-P5":r["notes"].append("Delivered method role2 used authenticated post-route optimized manifest; base method manifest is provenance only. See DELIVERY.json, not a standalone build-status interpretation.")
    # Full source-bound comparison receipts for the system model and composition.
    for eid,date,report,odir,meta,conf,overlay in [
        ("COMPOSITION-CLEAN-BOARD","2026-10-06","composition-history-isolation-20261007.md","composition-clean-20261006-board",["composition-clean-20261006/overlay-source-hashes.json","composition-clean-20261006/acceptance.json","composition-clean-20261006-board/board-acceptance.json"],{"DS":[50,50],"modes":["S","N","P","PN"],"replacement_history":"before isolation"},[]),
        ("COMPOSITION-ISOLATED-BOARD","2026-10-07","composition-history-isolation-20261007.md","composition-isolated-20261007",["composition-isolated-20261007/overlay.json","composition-isolated-20261007/build-command.json","composition-isolated-20261007/board-acceptance.json"],{"DS":[50,50],"modes":["S","N","P","PN"],"replacement_history":"partition-local"},["composition-isolated-20261007/production.patch"]),
        ("ACTUAL-DMA-SYSTEM-PN","2026-10-07","composition-history-isolation-20261007.md","pn-data-path-20261007",["pn-data-path-20261007/source-provenance.json","pn-data-path-20261007/acceptance.json","pn-data-path-20261007/comparison.json"],{"system":True,"modes":["P","PN"],"DS":[50,50],"ptw_latency":31,"A_pages":8,"V_gap":0,"transfers":64,"warmup":256,"ds_priority":"0 and 1 intervention; behavioral DDR"},["pn-data-path-20261007/applied-helper.patch"]),
        ("SHARED-READ-RTL","2026-10-09","shared-read-rtl-progress-20261009.md","shared-read-rtl-20261009",["shared-read-rtl-20261009/execution-ledger.json","shared-read-rtl-20261009/state-audit.md"],{"accepted_build":"build06","accepted_runs":"14-21,24-27","accepted_copies":12,"pause_resume":"one pause per lane scope; real iDMA/IOMMU RTL"},[]),
        ("M0-FULL-COPY","2026-10-10","workflow-g0-m0-results-20261010.md","workflow-g0-m0-20261010",["workflow-g0-m0-20261010/source-provenance.json","workflow-g0-m0-20261010/baseline-register.json","workflow-g0-m0-20261010/execution-ledger.json","workflow-g0-m0-20261010/root-result-acceptance.json"],{"each_lane_copies":64,"full_batch":True,"cpu":False,"complete_both":True,"modes":["S","N","P","PN"],"service":"FIFO and behavioral DS-priority","configurations":10},["workflow-g0-m0-20261010/implemented.patch"]),
    ]:
        r=row(eid,date,"board_run" if eid.endswith("BOARD") else "finite_RTL_behavioral_memory_run",PLATFORM/"docs"/report,OUT/odir,[OUT/x for x in meta],conf,reconstruction="selected_source_hashes_and_overlay_available; full_closure_unknown",features=COMPOSITION,overlays=[OUT/x for x in overlay])
        r["unknowns"]=["Do not infer complete build dependency closure from selected-source hashes.","Current organized feature commits were not the commits on which these runs executed."]
        if eid=="ACTUAL-DMA-SYSTEM-PN":r["report_line_anchors"]=[86,101,105]
        if eid=="M0-FULL-COPY":r["input_commits"]=[{"component":"iommu-platform","commit":"0a570deda89636631462bb188d39c058409df364","dirty":True},{"component":"cva6","commit":"a1b08934391833aed21e6ad63b545634040808ac","dirty":True}]
    # A2's actual build and A5's source/DAG check are deliberately different layers.
    a2=OUT/"a2-sva-implementation-20261015"
    lock=load(a2/"kernel/build-lock.json")
    r=row("A2-SVA-SOFTWARE","2026-10-15","software_build_and_layered_function_checks; no_kernel_or_DMA_runtime",PLATFORM/"docs/a2-sva-implementation-20261015.md",a2,[a2/"kernel/build-lock.json",a2/"patches/source-manifest.json",a2/"kernel/final-output-manifest.json",a2/"hero/config/version-lock.json",a2/"hero/evidence/changed-source-manifest.json",a2/"root-result-acceptance.json"],lock["profile"],reconstruction="software_lock_overlay_untracked_config_compiler_available; rebuild_not_performed",features=COMMON,commits=[{"component":"linux","commit":lock["Linux"]["base_commit"],"dirty":True}],overlays=[a2/"patches/linux-a2.patch",a2/"patches/platform-a2.patch",a2/"kernel/linux-full-working-tree.patch"],evidence_lines=[3,11,12,19,92,115])
    r["known_extension_flags"]={"iommu.research":"runtime opt-in, default off","idma.sva_enable":"runtime opt-in, default off","source_default":"mode0/4 DMA/24B/single TC", "actual_kernel_runtime":False,"actual_real_DMA":False}
    r["unknowns"]=["Real hardware/kernel run identity unknown; hardware gate not passed.","Hero three completion/cache/invalidation hooks remain default-reject; compiled artifacts are not a runnable original paper experiment."]
    a5=OUT/"a5-wait-scope-contract-20261015"
    r=row("A5-WAIT-SCOPE","2026-10-15","source_contract_and_offline_dependency_check_only",PLATFORM/"docs/a5-wait-scope-contract-20261015.md",a5,[a5/"source/translation-legs.json",a5/"case-contract.json",a5/"case-freeze.json",a5/"validation/execution-ledger.json",a5/"root-result-acceptance.json"],{"HERO_AXPY_elements":32768,"tiles":64,"L1_slots":4,"slot_bytes":2048,"episodes":2,"jobs":384,"variant_applied":False,"variant_compiled":False,"native_run":False},classification="external_HERO_source_assets; not_current_platform_stock_or_performance",reconstruction="fixed_source_refs_and_hashes_available; no_timing_execution_to_reconstruct",features=[],evidence_lines=[7,16,42,46,72])
    a5_sources=load(a5/"source/translation-legs.json")["sources"]
    r["input_commits"]=[{"component":s.get("repo"),"commit":s.get("ref"),"role":"fixed_source_ref_only; no_runtime_identity","source_path":s.get("path"),"sha256":s.get("sha256")} for s in a5_sources.values()]
    r["unknowns"]=["Actual IOVA/context/PTE, clock, same-ID cross-destination timing and complete native time remain unknown."]
    # First-PTE actual builds/runs. Failed preparations retain explicit separate rows.
    fp=OUT/"first-pte-paired-local-20261016"
    contract=load(fp/"root/input-contract.json")
    common_meta=[fp/"root/input-contract.json",fp/"root/execution-ledger.json",fp/"m/manifest.json",fp/"m/README.md",fp/"o/base-manifest.json",fp/"combined/assembly.json",fp/"runner/cpp-provenance.json"]
    for b in ["build-policy0","build-policy1","build-policy1-v2","build-policy1-v3","build-policy3","lint","lint-v2","runner/dry-run","runner/dry-run-v2"]:
        p=fp/b/"build-manifest.json"
        if p.is_file():build_detail(p)
    for case in contract["executed_policies"]:
        label={0:"base",1:"m",3:"prepared"}[case]
        b={0:"build-policy0",1:"build-policy1-v3",3:"build-policy3"}[case]
        for history in ["cold","hot"]:
            od=fp/"results"/(label+"-"+history)
            ex=load(od/"execution.json")
            result=load(od/"result.json")
            r=row("FIRST-PTE-"+label.upper()+"-"+history.upper(),"2026-10-16","finite_RTL_behavioral_memory_run",PLATFORM/"docs/first-pte-paired-local-20261016.md",od,common_meta+[fp/b/"build-manifest.json",od/"execution.json",od/"result.json"],dict(contract["frozen"],history=history,policy=case),classification="PN_plus_common_DS_first_F4_and_M_O_overlays; not_stock",reconstruction="explicit_build_sources_hashed; see_core_source_comparison; full_closure_unknown",features=COMPOSITION,commits=[{"component":"workspace_top","commit":contract["input_head"],"role":"input_contract_head; not_full_dirty_source_identity"}],overlays=[fp/"m/overlay.patch",fp/"o/o-overlay.patch"])
            r["execution_metadata"]=ex
            r["input_commits"] += [{"component":"cva6","commit":"a1b08934391833aed21e6ad63b545634040808ac","dirty":True,"role":"m/README.md line10 component input"},{"component":"iommu","commit":"57eb5621802ef9ed8d77c684b9a03205c6630391","dirty":True,"role":"m/README.md line10 component input; lock older238a39e is not actual overlay bytes"}]
            r["build_detail_key"]=rel(fp/b/"build-manifest.json")
            r["known_extension_flags"]={"PN":True,"common_DS_first_F4":True,"policy":case,"M_O_scope":"overlay only","production_modified":False}
            r["unknowns"]=["Build manifests do not hash every implicitly included header/environment.","Some combined overlay/CPP paths were revised during preparation; recorded manifest hash is authoritative, not present bytes at that path."]
            r["notes"].append("Ordinary base and M have equal complete times; case report/old verdict unchanged. This index neither reruns nor re-adjudicates it.")
    for p in [2,4]:
        row("FIRST-PTE-UNRUN-POLICY-"+str(p),"2026-10-16","implemented_overlay_not_performance_run",PLATFORM/"docs/first-pte-paired-local-20261016.md",fp/"o",common_meta,{"policy":p,"name":{2:"tail-local",4:"job128"}[p],"performance_run":False},classification="unexecuted_research_overlay",reconstruction="source_assets_only; no_result_to_reconstruct",features=COMPOSITION,overlays=[fp/"o/o-overlay.patch"])
    for b in ["build-policy1","build-policy1-v2","lint"]:
        row("FIRST-PTE-PREP-"+b,"2026-10-16","build_preparation_failure",PLATFORM/"docs/first-pte-paired-local-20261016.md",fp/b,[fp/b/"build-manifest.json",fp/"root/execution-ledger.json"],{"state":"failed_preparation; later fixed in separate build"},classification="historical_failed_preparation",reconstruction="failure_metadata_only",features=COMPOSITION)
    # Discovery exact run receipts, followed by PLRU each mode/ring variant.
    disc=OUT/"problem-discovery-run-20261017"
    inputv=load(disc/"input-version.json")
    bm=disc/"build01/build-manifest.json"
    build_detail(bm)
    for case_dir in sorted((disc/"runs").iterdir()):
        attempt=case_dir/"attempt01"
        receipt=load(attempt/"receipt.json")
        r=row("DISCOVERY-"+case_dir.name,"2026-10-17","finite_RTL_behavioral_memory_run",PLATFORM/"docs/problem-discovery-results-20261017.md",attempt,[disc/"input-version.json",bm,attempt/"receipt.json",attempt/"result.json",disc/"acceptance.json"],{"mode":receipt["mode"],"A_destination_ring":receipt["ring"],"A_leaf":receipt["a_leaf"],"copy_bytes":4096,"copies_each":64,"DS":[50,50],"DDR":"FIFO head-only startup31, independent R/W","DS_first_F4":True,"M_O":False,"CPU":False,"return_promotion":False,"queued_startup_overlap":False},classification="P_or_PN_plus_common_DS_first_F4; not_stock",reconstruction="explicit_build_sources_hashed; full_closure_unknown",features=COMPOSITION,commits=inputv["components"],evidence_lines=[19,34,44,75])
        r["execution_metadata"]=receipt
        r["build_detail_key"]=rel(bm)
        r["known_extension_flags"]={"P":True,"N":receipt["mode"]=="pn","DS_first_F4":True,"DS_delay":[50,50],"M_O":False,"production_modified":False}
        r["unknowns"]=["Input-version captures tracked dirty status, not a complete untracked-file manifest.","85 explicit SV input hashes plus CPP do not lock every implicit include/tool/environment."]
    plru=OUT/"plru-ablation-20261017"
    identity=load(plru/"variants/source-identity.json")
    for variant in ["v0","v1","v2"]:
        bm=plru/"builds"/(variant+"-build01")/"build-manifest.json"
        build_detail(bm)
        for mode in ["p","pn"]:
            for ring in [1,8,16]:
                case=f"{variant}-{mode}-r{ring}"
                receipts=[]
                for a in sorted((plru/"runs"/case).glob("attempt*/receipt.json")):
                    receipts.append(load(a))
                paths=[plru/"variants/source-identity.json",bm,plru/"run-admission.json",plru/"verification.json"]
                for a in sorted((plru/"runs"/case).glob("attempt*/receipt.json")):paths += [a,a.parent/"result.json"]
                r=row("PLRU-"+case,"2026-10-17","finite_RTL_behavioral_memory_run",plru/"README.md",plru/"runs"/case,paths,{"variant":variant,"mode":mode,"A_destination_ring":ring,"copy_bytes":4096,"copies_each":64,"DS":[50,50],"DS_first_F4":True,"DDR":"same frozen discovery FIFO startup31","attempts":len(receipts),"replacement": {"v0":"original functionality plus simulation-only passive fill observer","v1":"near-balanced mask-remapped tree, original touch/fill semantics","v2":"exact LRU plus invalid-first plus fill-MRU"}[variant]},classification="ablation_of_discovery_P_PN_research_fixture; V0_not_stock",reconstruction="explicit_build_sources_and_variant_hashes_available; no_rebuild_performed",features=COMPOSITION,commits=[{"component":"iommu","commit":identity["component_head"],"dirty":True,"role":"component_HEAD_only; not_source_of_entire_overlay"}])
                r["execution_metadata"]=receipts
                r["build_detail_key"]=rel(bm)
                r["known_extension_flags"]={"P":True,"N":mode=="pn","DS_first_F4":True,"DS_delay":[50,50],"passive_fill_trace":True,"PLRU_variant":variant,"production_modified":False}
                r["unknowns"]=["Variant/source hash identity does not prove a clean upstream baseline or full compiler dependency closure."]
                r["notes"]=["V1/V2 are historical out/ overlays only; they are not added to the organized production feature chain.","V0 result/parent/event byte match is the historical verification.json claim, distinct from its IOTLB source hash (which includes passive fill trace)."]
    # Read-only upstream check is explicitly not an experiment or a historical baseline.
    up=load(plru/"generality/upstream-source-receipt.json")
    row("UPSTREAM-IOMMU-READONLY-20261017","2026-10-17","upstream_source_inspection_only",plru/"generality/README.md",plru/"generality",[plru/"generality/upstream-source-receipt.json",plru/"generality/source-manifest.json"],{"upstream_receipt":up,"ran_against_this_upstream":False},classification="readonly_new_upstream_check; not_historical_baseline",reconstruction="source_receipt_only",features=[],notes=["18ad... upstream main is only a read-only 10-17 comparison object; no historical run is relabeled to this commit."])
    # Current named profiles are configuration inventory entries, independent of runs.
    profile_features={
        "observe":["cva6:CVA6-LIFECYCLE","iommu:O1"],"batch":["cva6:CVA6-BATCH"],"path":["cva6:CVA6-PATH"],"e3":["cva6:CVA6-E3-ACTIVE","iommu:E1"],"e3-queue":["cva6:CVA6-E3-ACTIVE","iommu:E1"],"e3-controlled":["cva6:CVA6-E3-ACTIVE","cva6:CVA6-HELD-LAUNCH","iommu:E1"],"p4m":["cva6:CVA6-P4M-GAP"],"research-v1":["cva6:CVA6-E2-COST","cva6:CVA6-LOG-RAM"],"research-e2-v2":["cva6:CVA6-E2-COST","cva6:CVA6-LOG-RAM"],"research-method-v2":["cva6:CVA6-RESEARCH-POLICY","cva6:CVA6-COST-AGGREGATE"],"composition":["iommu:B1","iommu:N1","iommu:P1","iommu:P2","cva6:CVA6-BANKED-PN-WIRING"],"n-serial":["iommu:B1","cva6:CVA6-BANKED-PN-WIRING"],"n-hit":["iommu:B1","iommu:N1","cva6:CVA6-BANKED-PN-WIRING"],
    }
    for p in sorted((PLATFORM/"configs").glob("*.json")):
        if not (p.stem.startswith("baseline-4dma") or p.stem.startswith("migration-4dma")):continue
        config=load(p)
        m=OUT/"manifests"/("fpga-"+p.name)
        metas=[p]
        if m.is_file():metas.append(m)
        suffix=p.stem.removeprefix("migration-4dma-").removeprefix("baseline-4dma-")
        r=row("PROFILE-"+p.stem,"snapshot-2026-10-17","configuration_inventory; run_status_separate",PLATFORM/"docs/build.md",p.parent,metas,config,classification="name_is_not_stock_certificate; source_and_compile_flags_must_be_checked",reconstruction="current_profile_bytes_only; historical_build_association_when_present",features=COMMON+feature(*profile_features.get(suffix,[])))
        r["known_extension_flags"]={k:config[k] for k in ["ds_delay_input","ds_delay_output","legacy_e3_observer","research_role","nonblock_mode"] if k in config}
        r["known_extension_flags"]["absent_parameters"]="unknown_or_build_script_default; absence_does_not_prove_disabled"
        if m.is_file():
            j=attach_fpga(r,m)
            recorded=j.get("build_start",{}).get("profile_sha256")
            r["current_profile_matches_recorded_build_profile_sha256"]=sha(p)==recorded if recorded else None
            r["historical_build_profile_sha256"]=recorded
            recovery = OUT/"manifests"/("fpga-"+p.stem+"-v5-recovery.json")
            if recovery.is_file():
                recovery_data = load(recovery)
                r["source_manifests"].append(ref(recovery,"recovery_build_metadata"))
                r["recovery_build_identity"] = recovery_data
                r["notes"].append("Initial named build failed; a distinct v5 recovery receipt exists. Preserve both states instead of treating the initial failure as the final profile status.")
        else:r["unknowns"].append("No named FPGA manifest found in the selected manifest inventory; this row proves configuration presence only.")
        r["notes"].append("No new build/run was triggered. Historical descriptions saying pending or building are kept verbatim in snapshots and do not override later reports.")
    # Link the now-created feature chain without changing any historical identity.
    component_manifests={}
    feature_commits={}
    for component in ["iommu","cva6"]:
        mp=HERE.parent/component/"manifest.json"
        if not mp.is_file():
            continue
        manifest=load(mp)
        component_manifests[component]=ref(mp,"organized_component_manifest")
        for s in manifest.get("steps",[]):
            feature_commits[(component,s["feature_id"])]=s["commit"]
    for r in ROWS:
        unresolved=[]
        for f in r["feature_mapping"]:
            key=(f["component"],f["feature_id"])
            if key not in feature_commits:
                unresolved.append(f)
                continue
            r["new_commits"].append(dict(f,commit=feature_commits[key],relation="semantic_feature_family_association_only",historical_source_equivalence="not_asserted",organized_manifest=component_manifests[f["component"]]))
        if not r["feature_mapping"]:
            r["new_commit_mapping_status"]="not_applicable; historical_external_or_source_only_asset"
        elif unresolved:
            r["new_commit_mapping_status"]="partial; missing_component_or_feature_metadata"
            r["unknowns"].append({"unresolved_feature_mappings":unresolved})
        else:
            r["new_commit_mapping_status"]="linked_to_final_component_manifests; semantic_association_only; no_historical_reexecution"
        r["input_commit_coverage"]="known_entries_only; dirty_sources_and_dependency_closure_separate" if r["input_commits"] else "unknown_or_not_applicable; no_commit_binding_extracted_for_this_record"
        if not r["input_commits"] and r["evidence_layer"] in {"board_run","finite_RTL_behavioral_memory_run","historical_table_only"}:
            r["unknowns"].append("Complete component commit binding is not extracted for this older record; selected source hashes/overlay receipts remain the available identity.")
    # Freeze selected metadata, source receipts and static comparison products.
    data={"schema":"source-organization-history-v1","captured_utc":CAPTURED,"scope":"existing history and metadata only; no experiments/builds/reductions/board actions","records":ROWS,"record_count":len(ROWS),"source_hash_details":"core-build-sources.json","evidence_inventory":"evidence-files.json","organized_component_manifests":component_manifests,"mapping_semantics":"new feature IDs are navigational semantic associations; execution on organized commits is never inferred","no_stock_retroactive_reclassification":True}
    (HERE/"index.json").write_text(json.dumps(data,ensure_ascii=False,indent=2)+"\n")
    core={"schema":"explicit-build-source-hash-index-v1","captured_utc":CAPTURED,"builds":BUILDS}
    (HERE/"core-build-sources.json").write_text(json.dumps(core,ensure_ascii=False,indent=2)+"\n")
    with (HERE/"core-build-sources.csv").open("w",newline="") as f:
        keys=["build_manifest","logical_path","recorded_path","recorded_sha256","source_blob","recorded_bytes_match_at_index","current_workspace_base_path","current_workspace_base_sha256_at_index","matches_current_workspace_base_at_index","difference_origin"]
        w=csv.DictWriter(f,fieldnames=keys);w.writeheader()
        for k,b in BUILDS.items():
            for s in b["explicit_sources"]:
                w.writerow(dict(build_manifest=k,logical_path=s["logical_path"],recorded_path=s["path"],recorded_sha256=s["sha256"],**{x:s[x] for x in keys[4:]}))
    with (HERE/"index.csv").open("w",newline="") as f:
        keys=["experiment_id","date","evidence_layer","report","output_dir","input_commits","source_manifests","overlay","config","known_extension_flags","baseline_classification","reconstruction_status","feature_mapping","new_commits","unknowns"]
        w=csv.DictWriter(f,fieldnames=keys);w.writeheader()
        for r in ROWS:w.writerow({k:json.dumps(r[k],ensure_ascii=False,separators=(",",":")) if isinstance(r[k],(list,dict)) else r[k] for k in keys})
    (HERE/"evidence-files.json").write_text(json.dumps({"schema":"selected-original-metadata-files-v1","captured_utc":CAPTURED,"files":list(FILES.values())},ensure_ascii=False,indent=2)+"\n")
    summary={"record_count":len(ROWS),"by_layer":dict(Counter(r["evidence_layer"] for r in ROWS)),"explicit_builds":len(BUILDS),"source_hash_counts":{k:v["counts"] for k,v in BUILDS.items()},"selected_original_files":len(FILES),"missing_selected_originals":[x["path"] for x in FILES.values() if not x["exists"]],"new_experiments":0,"new_builds":0,"new_reductions":0,"board_operations":0,"authorship":"Codex only; no DeepSeek or Claude"}
    (HERE/"collection-summary.json").write_text(json.dumps(summary,ensure_ascii=False,indent=2)+"\n")
    print(json.dumps({k:v for k,v in summary.items() if k!="source_hash_counts"},ensure_ascii=False,indent=2))


if __name__=="__main__":
    main()
