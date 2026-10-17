#!/usr/bin/env python3
"""Index selected, already reported historical families without executing them.

Only explicitly named report/output families and their shallow metadata are read.
This is an additive source-identity sidecar, never a scientific reanalysis.  JSON
pointers retain the role/context of original fields; an inspected upstream ref is
never promoted to a historical checkout or an executed source tree.
"""
from __future__ import annotations

import csv
import hashlib
import io
import json
import re
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
OUT = ROOT / "iommu-platform/out"

# id | report under platform docs (or explicit workspace path) | output family
# | evidence layer | execution status | precise family-level coverage
SPECS = [
    ("E3-D0-DIAGNOSTIC", "e3-diag-results-review.md", "mac-e3-diag-main-20260918-081411-run-1z6a407q", "board_run", "completed_historical_diagnostic", "9 paired records / 18 subrounds including short and formal; D0/D0 only; not D128"),
    ("P4M-SHORT", "p4m-short-results-review.md", "mac-p4m-short-20260920-180713-run-l7cin11d", "board_run", "completed_historical_short_functional_acceptance", "4 single-trial conditions; not formal benefit evidence"),
    ("P4M-PILOT21", "p4m-pilot-results-review.md", "mac-p4m-pilot-20260920-202441-run-9_mfua8v", "board_run", "completed_historical_pilot", "21 trials; startup/prewindow confounding preserved"),
    ("COMPOSITION-ROUTING-COST", "composition-routing-cost-20261005.md", "composition-trim-20261005", "FPGA_build_resource_and_routing_evidence", "historical_build_evidence_only", "trim/routing resource reports; completion and later board runs remain separately indexed"),
    ("SHARED-READ-EVENT-MODEL", "shared-read-independent-scheduling-20261009.md", "innovation-rescue-20261009", "untimed_or_discrete_event_model", "completed_historical_event_model", "314 bounded event scenarios; event steps are not RTL cycles; later RTL is in original index"),
    ("PARENT-CAPACITY", "parent-capacity-boundary-20261008.md", "parent-capacity-20261008", "finite_RTL_behavioral_memory_run", "completed_historical_RTL_simulation", "3 builds / 6 new runs; 2 old eight-slot traces reused, not rerun"),
    ("MULTI-PARENT-PREPARATION", "multi-parent-preparation-boundary-20261008.md", "multi-parent-preparation-20261008", "finite_RTL_behavioral_memory_run", "completed_historical_RTL_simulation", "5 builds / 18 runs, including 8 ordinary, 4 read-priority, 2 observation, 4 hit-bypass"),
    ("PREPARED-SOURCE-BYPASS", "prepared-source-bypass-boundary-20261008.md", "prepared-source-bypass-20261008", "finite_RTL_behavioral_memory_run", "completed_historical_RTL_simulation", "1 build / 6 complete-copy runs; old post-TL comparison reused"),
    ("ORIGINAL-WINDOW", "original-window-progress-boundary-20261008.md", "original-window-20261008", "finite_RTL_behavioral_memory_run", "completed_and_stalled_historical_RTL_runs", "4 builds / 16 runs: 10 complete and 6 watchdog stalls; capacity and timing variants retained"),
    ("FRAGMENT-DRAIN-GEOMETRY", "fragment-drain-cost-boundary-20261008.md", "fragment-drain-boundary-20261008", "finite_mathematical_enumeration", "completed_historical_math_only", "one geometry program, 11390 partially overlapping classifications; zero DUT runs"),
    ("PERMISSION-RELEASE-COALITION", "research-permission-release-20261009.md", "innovation-unified-cost-20261009", "source_bound_design_and_hand_calculation", "no_new_model_or_RTL_execution", "shared storage and permission examples are hand calculations; no timing/area acceptance"),
    ("TRANSLATION-SHARING-QOS", "translation-sharing-qos-20261008.md", "translation-sharing-qos-20261008", "source_and_prior_art_analysis", "no_new_model_or_build_execution", "registered-use/QoS source and paper rules; not a performance run"),
    ("ROW-G0", "workflow-row-g0-results-20261011.md", "research-execution-20261011", "finite_RTL_behavioral_memory_run_reusing_compiled_model", "completed_historical_two_case_run", "two registered P/4KiB and P/2MiB conditions, each 480 V + 2048 A copies; one private C++ compile; no new Verilator elaboration"),
    ("STREAM-HERO-FUNCTIONAL", "workflow-stream-contract-20261012.md", "workflow-stream-contract-20261012", "host_source_semantic_functional_execution", "completed_historical_host_functional_run", "one C host compile/run; 288 DMA-semantic memcpy jobs / 65536 outputs; not native HERO/DMA timing"),
    ("STREAM-G0", "workflow-stream-g0-results-20261012.md", "workflow-stream-g0-20261012", "finite_RTL_behavioral_memory_with_private_compute_bridge", "completed_historical_RTL_model_with_build_repairs", "stage100 build attempts and accepted runs; private compute bridge timing assumptions, not original eight-PE performance"),
    ("STREAM-PLATFORM-REVIEW", "workflow-stream-platform-review-20261012.md", "workflow-stream-platform-review-20261012", "source_and_previous_result_review", "no_new_model_or_build_execution", "stage101 source/platform review of prior stage100; prior runs not counted anew"),
    ("STREAM-BACKGROUND-DEMAND", "workflow-stream-background-demand-20261012.md", "workflow-stream-background-demand-20261012", "source_contract_review", "no_new_model_or_build_execution", "two official HERO source entries; independent background demand remains unknown"),
    ("R107-BOUNDED-SOURCE", "r107-bounded-decision-20261012.md", "r107-bounded-decision-20261012", "host_source_semantic_functional_execution", "completed_historical_CPU_source_check", "vLLM finite CPU source-function instance / 144 byte-tagged moves; no GPU/native attention timing"),
    ("R107-ORDINARY-BOARD", "r107-ordinary-pilot-20261012.md", "r107-ordinary-pilot-20261012", "board_run_with_frozen_software_build", "completed_historical_board_recovery", "ordinary finite restore pilot; 1728 restore lookups all hit, zero walk; build/failure/repair/load identities remain separate"),
    ("STAGE117-PLAN-G0", "stage117-plan-revision-g0-20261013.md", "stage117-plan-revision-g0-20261013", "finite_RTL_behavioral_memory_run", "completed_historical_four_case_G0", "four finite stage comparisons; source DPDK projection, not native vhost timing"),
    ("STAGE117-PACKET-ADAPTER", "stage117-packet-protocol-adapter-20261013.md", "stage117-packet-protocol-adapter-20261013", "finite_RTL_protocol_model", "completed_historical_protocol_functional_cases", "uncached-assumption adapter, 17 protocol tests/two cases/15 checker selftests; no actual CPU cache"),
    ("STAGE117-REAL-CACHE", "stage117-real-cache-handoff-20261013.md", "stage117-real-cache-handoff-20261013", "finite_RTL_actual_WT_cache_functional_model", "completed_historical_three_run_cache_witness", "three actual-cache witness runs; consumer/peer overlap not established; not full CPU/native app"),
    ("STAGE117-DELIVERY-G0", "stage117-delivery-g0-20261013.md", "stage117-delivery-g0-20261013", "finite_RTL_actual_WT_cache_delivery_model", "completed_historical_six_case_G0", "CPU/single-DMA/two-DMA x eager/batch = six cases; actual compiled dependencies and history-custody limits retained"),
    ("A-CONTROL-POSITION", "control-position-method-20261015.md", "control-position-method-20261015", "host_source_semantic_functional_execution", "completed_historical_semantic_cases", "four cases/eight complete source-semantic executions; native timing unknown"),
    ("A3-CONSUMER-RULES", "a3-consumer-position-rules-20261015.md", "a3-consumer-position-rules-20261015", "source_contract_and_symbolic_rule_checks", "completed_historical_eight_symbolic_checks", "eight logical probes; no payload/AXPY/RTL/kernel execution"),
    ("A4-AXPY-DECISION", "a4-axpy-control-decision-20261015.md", "a4-axpy-control-decision-20261015", "source_bound_design_analysis", "variants_not_applied_compiled_or_run", "O-T1/O-G source receipt chain and design; zero performance conditions"),
    ("D-CS1", "iommu-platform/out/d-service-model-20261015/README.md", "d-service-model-20261015", "synthetic_closed_loop_model_and_independent_event_replay", "completed_historical_model_with_original_generator_hash_gap", "8 configurations / 280 worlds / 2240 target actions / 1578 deduplicated complete event traces; not RTL or SPDK runtime"),
    ("I-SOURCE-PRESCREEN", "independent-research-directions-20261015.md", "research-plan-implementation-20261015/i", "source_and_lifecycle_prescreen", "no_native_IO_or_live_DMA_trace", "normal map/unmap/invalidation source prescreen; backend and external lifecycle source evidence only"),
    ("B1-STRIPE-COST", "iommu-platform/out/b1-stripe-cost-20261016/README.md", "b1-stripe-cost-20261016", "source_bound_capacity_and_cost_analysis", "no_native_or_performance_execution", "one-stripe source/capacity/storage/port/lifecycle costs; no complete net-benefit witness"),
    ("R2-OWNERSHIP-COST", "r2-ownership-cost-20261014.md", "r2-ownership-cost-20261013", "source_bound_lifecycle_design_and_arithmetic", "completed_historical_cost_calculator_no_performance_run", "one initial line-range failure and one accepted 14-check arithmetic run; no RTL/native performance"),
    ("CO1", "completion-obligation-design-20261014.md", "completion-obligation-design-20261014", "source_bound_design_and_hand_proof", "no_new_model_or_performance_execution", "OC1-min paired action/logic cost and corrected fairness proof; four performance conditions not run"),
    ("RCS1", "rcs1-completion-set-20261014.md", "rcs1-completion-set-20261014", "finite_synthetic_completion_set_model", "completed_historical_32_case_model", "four frozen conditions x eight policies; 64B permit is additional engine capability, not production fullbuffer"),
    ("BR1", "blocking-replacement-20261014.md", "blocking-replacement-20261014", "source_bound_functional_graph_and_cost_analysis", "no_four_condition_performance_execution", "cold functional graph and paired cost witness; four performance conditions not run"),
    ("JOINT-SOURCE-CHOICE", "joint-source-choice-20261014.md", "joint-source-choice-20261014", "source_bound_interface_analysis", "no_new_native_performance_execution", "common source-choice point and source contract; not native timing benefit"),
    ("RPTE-RETURN-COMMITMENT", "rpte-return-commitment-decision-20261014.md", "rpte-return-commitment-20261014", "source_bound_commitment_and_conditional_model_analysis", "historical_bounded_analysis_only", "RREADY/buffer/W/first-VALID and envelope eligibility; exact activity in original execution ledger"),
    ("ENVELOPE-COST", "envelope-observable-cost-20261014.md", "envelope-observable-cost-20261014", "source_bound_observability_and_cost_analysis", "no_new_native_performance_execution", "two scores do not represent full write readiness; interface/cost evidence only"),
    ("AW-BARRIER", "aw-barrier-causal-20261014.md", "aw-barrier-causal-20261014", "source_bound_causal_chain_review", "no_new_native_performance_execution", "upstream AW spill and TC input acceptance distinguished; no old experiment rerun"),
    ("DDR-SERVICE-BOUNDARY", "ddr-service-boundary-20261014.md", "ddr-service-boundary-20261014", "generated_MIG_source_and_interface_analysis", "no_new_performance_or_board_execution", "generated MIG4.2 reference binding; accepted-return-order facts are not measured debt"),
    ("SDH1", "iommu-platform/out/source-drain-hit-20261014/README.md", "source-drain-hit-20261014", "untimed_private_protocol_functional_model", "completed_historical_functional_and_equivalence_checks", "six functional groups and 32768 ordinary-selection snapshots on shared paid directOUT adapter; no cache/PTW/DDR timing"),
    ("LR1", "local-read-reclaim-20261015.md", "local-read-reclaim-20261015", "untimed_private_protocol_functional_model", "completed_historical_functional_and_policy_checks", "eight groups/122 checks and 221184 ordinary-policy calls; subsequent source/alias errata retained"),
    ("SHARED-SERVICE-BOUNDARY", "shared-service-control-boundary-20261015.md", "shared-service-control-boundary-20261015", "source_and_historical_artifact_identity_review", "no_new_performance_execution", "DS-first design and historical BIT identity chain; historical BIT is not today's board load"),
    ("SHARED-SERVICE-ADMISSION", "shared-service-mechanism-admission-20261015.md", "shared-service-mechanism-admission-20261015", "source_bound_mechanism_admission_contract", "no_new_model_build_or_board_execution", "normal COPY first-PTE admissibility contract only; paused MIG is not resumed"),
    ("MECHANISM-CONSTRUCTION", "iommu-platform/out/mechanism-construction-20261016/README.md", "mechanism-construction-20261016", "frozen_conditional_event_model", "completed_historical_70_simulation_batch", "one Python batch / 70 simulations / 82 checks; five frozen conditions; not RTL or native"),
    ("FIRST-PTE-INTERFACE-COST", "first-pte-interface-cost-20261016.md", "first-pte-interface-cost-20261016", "source_bound_paired_interface_and_legalizer_design", "no_system_performance_run_in_this_design_round", "M-ID f1 and O-local/O-prepared design; subsequent local RTL runs are already in original index"),
    ("RECONVERGENCE-1", "research-reconvergence-20261016.md", "research-reconvergence-20261016", "critical_source_and_prior_art_design_review", "no_new_performance_execution", "three concrete mechanism designs reviewed; no chosen new performance experiment"),
    ("UPSTREAM-LINUX-IOVA-HPM", "iommu-platform/out/upstream-linux-fixes-20261012/submission.md", "upstream-linux-fixes-20261012", "host_C_regression_on_extracted_actual_functions_and_macros", "completed_historical_host_regressions", "IOVA 16 cases and HPM 32 cases, baseline and patched; no full kernel/new board run"),
    ("UPSTREAM-IOMMU-DS-PTW", "iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json", "upstream-iommu-fixes-20261012", "module_RTL_functional_regression_and_source_review", "completed_historical_before_after_module_tests", "DS ownership/PTW leaf before/after tests and separate PR applicability source review; no new board performance"),
    ("AXI-BOUNDARY-CHECKER", "iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json", "iommu-axi-bc-fix-20261012", "module_RTL_functional_regression", "completed_historical_boundary_checker_regression", "816064 module checks; baseline 112514 failures; no new FPGA/board"),
    ("MIG-DEBT-OBSERVATION", "mig-pte-debt-observation-20261015.md", "mig-pte-debt-observation-20261015", "observer_functional_tests_and_failed_FPGA_timing_builds", "build6_timing_failed_not_loaded_goal_paused", "six build attempts, final BIT/MCS generated but WNS -0.391ns/598 setup violations; zero new load/capture; functionality separate"),
]

for suffix in ["organization-source", "admission", "lifetime-design", "integration-delta", "reuse-opportunity", "evidence-input", "recycle-reclaim", "owner-publish", "owner-representation", "dma-visibility", "iova-budget"]:
    layer = "source_bound_conditional_design_analysis"
    status = "not_implemented_no_native_performance_execution"
    scope = "source manifests and conditional design only; no production implementation/native workload/performance benefit"
    if suffix == "admission":
        layer = "source_review_and_finite_fee_ledger"
        status = "completed_historical_fee_arithmetic_only"
        scope = "official v6.12/DAMN sources, three fee-script invocations and one independent scalar check; no timing model"
    if suffix == "reuse-opportunity":
        layer = "source_review_and_readonly_host_identity_probe"
        status = "completed_historical_readonly_metadata_probe_no_workload"
        scope = "one read-only host identity check and source contracts; host differs from fixed Linux v6.12 target; no traffic/probe attachment"
    if suffix == "evidence-input":
        layer = "source_and_public_dataset_prefix_inspection"
        status = "completed_historical_input_screen_no_dataset_program_execution"
        scope = "8MiB prefix/70 tar headers/three complete small texts; qualified matched native inputs zero, not measured benefit zero"
    directory = f"mapping-{suffix}-20261016"
    SPECS.append((f"MAPPING-{suffix.upper()}", f"iommu-platform/out/{directory}/README.md", directory, layer, status, scope))

EXTRA = {
    "E3-D0-DIAGNOSTIC": ["mac-e3-diag-short-20260918-081411-run-1z6a407q/run.json"],
    "ROW-G0": ["research-execution-20261011/proposed/compile-argv.json", "research-execution-20261011/proposed/source-manifest.json"],
    "R107-ORDINARY-BOARD": [f"r107-ordinary-pilot-20261012/repair-1/build/{n}" for n in ["linux-baseline-source-manifest.json", "linux-reviewed-source-manifest.json", "linux-source-delta.json", "opensbi-frozen-source-manifest.json", "frozen-input-manifest.json", "actual-command-ledger.json", "reviewed-build.json", "execution-state.json"]],
    "STREAM-G0": ["workflow-stream-g0-20261012/source-freeze-final.json"],
    "STAGE117-DELIVERY-G0": ["stage117-delivery-g0-20261013/implementation/build-history-custody.json", "stage117-delivery-g0-20261013/implementation/build-inputs.json", "stage117-delivery-g0-20261013/implementation/actual-compiled-dependencies.json"],
    "I-SOURCE-PRESCREEN": ["d-source-gate-20261015/i-backend/findings.md", "independent-directions-20261015/lifecycle/assessment.md", "research-plan-implementation-20261015/i/source-evidence.md"],
    "AXI-BOUNDARY-CHECKER": ["iommu-axi-bc-fix-20261012/before-head.txt", "iommu-axi-bc-fix-20261012/rtl-fix.patch"],
    "UPSTREAM-LINUX-IOVA-HPM": ["upstream-linux-fixes-20261012/0001-iova-leaf-offset.patch", "upstream-linux-fixes-20261012/0002-hpm-macro-arguments.patch", "upstream-iommu-fixes-20261012/linux-fix-acceptance.json", "upstream-iommu-fixes-20261012/submission.json"],
    "UPSTREAM-IOMMU-DS-PTW": ["upstream-iommu-fixes-20261012/submission.json", "upstream-iommu-fixes-20261012/maintenance-evidence.json", "upstream-iommu-fixes-20261012/local-fixes.patch"],
    "MIG-DEBT-OBSERVATION": ["mig-pte-debt-observation-20261015/root/final-build6-manifest.json", "mig-pte-debt-observation-20261015/root/goal-pause.json", "mig-pte-debt-observation-20261015/root/sixth-final-timing.json", "mig-pte-debt-observation-20261015/capture/artifact-manifest.json"],
}

SKIP_DIRS = {"before", "before-sync", "before-routefix", "pre-final-sync", "pre-sync", "documentation", "venv", ".Xil", "fpga-src", "source-blobs", "raw", "sources", "source", "final-build6", "data", "host", "live", "results", "runs", "source-freeze", "linux", "opensbi", "l-aurora"}
META = re.compile(r"manifest|source|provenance|execution|contract|acceptance|evidence|freeze|argv|command|hashes|build-ledger|history-custody|raw-manifest|run\.json|stage.*lock|delivery|artifact-sha|functional-results|policy-checks|ordinary-equivalence|results-summary|stage.*identity", re.I)
SKIP_FILES = re.compile(r"before-write|document|delivery-verification|goal-|runtime|model-runtime|doc-change|snapshot|before-completion|completion-artifact|state-sync|final-artifact|root-source-manifest", re.I)
SOURCE_SUFFIXES = {".sv", ".svh", ".v", ".vh", ".c", ".h", ".cc", ".cpp", ".hpp", ".py", ".sh", ".tcl", ".dts", ".dtsi", ".config", ".patch", ".toml", ".yaml", ".yml"}
SHA = re.compile(r"^[0-9a-f]{40}$")
HASH = re.compile(r"^[0-9a-f]{64}$")
COMMIT_KEYS = re.compile(r"^(?:.*_)?(?:commit|head|sha|revision|ref|branch)(?:_.*)?$|^git$|^peeled_commit$", re.I)
CMD_KEYS = re.compile(r"^(?:argv|command|cmd|build_command|run_command|compile_argv|run_argv|build_argv)$")


def digest(p: Path) -> str:
    h = hashlib.sha256()
    with p.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def relative(p: Path) -> str:
    return str(p.resolve().relative_to(ROOT))


def ref(p: Path, pointer: str | None = None) -> dict:
    r = {"path": relative(p), "sha256": digest(p), "hash_capture_role": "checked_now_original_evidence_bytes; not_a_new_execution_source_signature"}
    if pointer is not None:
        r["json_pointer"] = pointer
    return r


def esc(s: str) -> str:
    return s.replace("~", "~0").replace("/", "~1")


def local_link(text: str, owner: Path) -> Path | None:
    text = text.split("#", 1)[0]
    text = re.sub(r":\d+(?:-\d+)?$", "", text)
    if not text or "://" in text or text.startswith("mailto:"):
        return None
    p = (owner.parent / text).resolve()
    try:
        p.relative_to(ROOT)
    except ValueError:
        return None
    return p if p.is_file() else None


def metadata(report: Path, directory: Path, extra: list[str]) -> list[Path]:
    files = {report}
    readme = directory / "README.md"
    if readme.is_file():
        files.add(readme)
    # Report and family README links, then linked family-local Markdown once.
    frontier = list(files)
    for depth in range(2):
        follow = []
        for p in frontier:
            if p.suffix != ".md":
                continue
            for link in re.findall(r"\]\(([^)]+)\)", p.read_text(errors="replace")):
                q = local_link(link, p)
                if q is None or q.suffix not in {".json", ".md", ".txt", ".patch"}:
                    continue
                if q != report and not q.is_relative_to(directory):
                    continue
                if q.is_relative_to(directory) and any(x in q.relative_to(directory).parts[:-1] for x in {"before", "documentation"}):
                    continue
                if q not in files:
                    files.add(q)
                    if q.suffix == ".md":
                        follow.append(q)
        frontier = follow
    # Shallow metadata only; never descend into Linux/RTL/build/object trees.
    dirs = [directory]
    if directory.exists():
        dirs += [p for p in directory.iterdir() if p.is_dir() and p.name not in SKIP_DIRS and not p.name.startswith(("obj", "build-attempt"))]
    for d in dirs:
        for p in d.glob("*.json"):
            if META.search(p.name) and not SKIP_FILES.search(p.name):
                files.add(p)
    for e in extra:
        p = OUT / e
        if p.is_file():
            files.add(p)
    return sorted(files)


def inspect_json(p: Path) -> tuple[list[dict], list[dict], dict, list[dict]]:
    """Extract navigable original fields; do not infer their execution role."""
    x = json.loads(p.read_text())
    origin = ref(p)
    git_facts, commands, file_hashes = [], [], []
    stats = Counter()
    hash_groups = set()

    def visit(o, pointer=""):
        if isinstance(o, dict):
            # Original file/hash pairs remain observations, not today's bytes.
            h = o.get("sha256")
            path = next((o[k] for k in ["path", "local_path", "repo_path", "file", "source", "frozen", "original"] if isinstance(o.get(k), str)), None)
            if path and isinstance(h, str) and HASH.fullmatch(h):
                stats["recorded_file_hash_entries"] += 1
                hash_groups.add(pointer.rsplit("/", 1)[0] if pointer else "")
                if Path(path).suffix.lower() in SOURCE_SUFFIXES or Path(path).name in {"Makefile", ".config", "sources.lock.json"}:
                    stats["recorded_source_or_config_hash_entries"] += 1
                    # Avoid duplicating 80K-file snapshots: the full manifest is
                    # bound by SHA and group pointers; retain navigable samples.
                    if len(file_hashes) < 24:
                        file_hashes.append({"recorded_path": path, "recorded_sha256": h, "evidence": {**origin, "json_pointer": pointer}, "role": "original_recorded_file_hash; inspect_parent_context_for_build_vs_reference_vs_guard", "hash_time_basis": "hash_declared_in_original_evidence; not_recomputed_as_original_execution_hash"})
            for k, v in o.items():
                ptr = pointer + "/" + esc(str(k))
                # Earlier stage ledgers often use {source_path: sha256} maps,
                # and module acceptances name rtl/runner hashes as scalars.
                if isinstance(v, str) and HASH.fullmatch(v):
                    is_source_path = Path(k).suffix.lower() in SOURCE_SUFFIXES
                    is_named_hash = k.endswith(("_sha256", "_hash")) and any(t in k.lower() for t in ["rtl", "source", "runner", "binary", "contract", "input", "overlay", "patch"])
                    if is_source_path or is_named_hash:
                        stats["recorded_named_or_mapped_identity_hash_entries"] += 1
                        hash_groups.add(pointer if is_source_path else ptr)
                        if is_source_path:
                            stats["recorded_source_or_config_hash_entries"] += 1
                        if len(file_hashes) < 24:
                            file_hashes.append({"recorded_path": k if is_source_path else None, "recorded_field": k, "recorded_sha256": v, "evidence": {**origin, "json_pointer": ptr}, "role": "original_declared_source_or_named_identity_hash; inspect_original_context", "hash_time_basis": "hash_declared_in_original_evidence; not_recomputed_as_original_execution_hash"})
                if COMMIT_KEYS.search(k) and isinstance(v, str) and len(v) < 300:
                    # Git refs only, not model IDs, cache-head indices, hashes.
                    has_git_context = any(t in pointer.lower() for t in ["git", "repo", "checkout"]) or any(z in o for z in ["commit", "head", "HEAD", "repository", "repo", "git_head"])
                    is_branch = (k.lower() == "branch" or k.lower().endswith("_branch")) and has_git_context and re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9._/-]*", v)
                    if SHA.fullmatch(v) or re.fullmatch(r"[0-9a-f]{7,39}", v) or is_branch:
                        hint = {kk: o[kk] for kk in ["repo", "repository", "repository_path", "component", "name", "path", "url"] if isinstance(o.get(kk), str)}
                        if "blob" in k.lower() or k.lower() == "sha" and ("path" in o or "type" in o):
                            role = "git_object_identifier_not_assumed_commit"
                        elif "lock" in k.lower() or "sources.lock" in str(p):
                            role = "configured_lock_or_source_reference; not_execution_checkout"
                        elif "build" in p.parts or "build" in k.lower() or "freeze" in p.name:
                            role = "recorded_build_or_freeze_source_reference; not_inferred_checkout"
                        else:
                            role = "source_inspection_or_original_reference; not_execution_checkout"
                        git_facts.append({"field": k, "value": v, "context": hint, "evidence": {**origin, "json_pointer": ptr}, "role": role})
                if CMD_KEYS.fullmatch(k) and isinstance(v, (str, list)):
                    commands.append({"field": k, "evidence": {**origin, "json_pointer": ptr}, "classification": "original_command_field; build_run_or_proposal_role_is_in_original_context"})
                visit(v, ptr)
        elif isinstance(o, list):
            for i, v in enumerate(o):
                visit(v, pointer + "/" + str(i))

    visit(x)
    stats["top_level_entries"] = len(x) if isinstance(x, (dict, list)) else 1
    detail = dict(stats)
    detail["complete_original_hash_group_references"] = [{**origin, "json_pointer": ptr} for ptr in sorted(hash_groups)]
    return git_facts, commands, detail, file_hashes


def qualified_bindings(identifier: str, facts: list[dict]) -> list[dict]:
    """Promote only explicitly reviewed facts to bounded, named source roles."""
    bindings = []
    if identifier == "R107-ORDINARY-BOARD":
        for component, name in [("linux", "linux-parent-frozen-source-manifest.json"), ("opensbi", "opensbi-frozen-source-manifest.json")]:
            f = next(f for f in facts if f["field"] == "base_commit" and f["evidence"]["path"].endswith("/build/" + name) and "/repair-1/" not in f["evidence"]["path"])
            bindings.append({"repository": "iommu-platform/" + component, "checkout_branch": None, "checkout_commit": None, "selected_source_commit": f["value"], "identity_role": "frozen_build_source_base_commit", "evidence": f["evidence"], "reason": "Original frozen manifest establishes archive base plus tracked overlays, not an unmodified checkout or its branch. Final reviewed/repair source manifests and buildgroup identities below bind the compiled bytes."})
    if identifier == "R107-BOUNDED-SOURCE":
        f = next(f for f in facts if f["field"] == "platform_git_head")
        bindings.append({"repository": "iommu-platform", "checkout_branch": None, "checkout_commit": f["value"], "identity_role": "historical_source_check_checkout_snapshot_not_board_execution", "evidence": f["evidence"], "reason": "Original source-version check records platform HEAD/dirty; no branch and no kernel/board execution is established by this field."})
        f = next(f for f in facts if f["field"] == "vllm_commit")
        bindings.append({"repository": "vllm-project/vllm", "checkout_branch": None, "checkout_commit": None, "selected_source_commit": f["value"], "identity_role": "inspected_and_CPU_source_semantic_instance_reference", "evidence": f["evidence"], "reason": "Fixed source contract, not native GPU runtime checkout."})
    if identifier.startswith("MAPPING-"):
        f = next((f for f in facts if f["value"] == "adc218676eef25575469234709c2d87185ca223a"), None)
        if f:
            bindings.append({"repository": "torvalds/linux", "checkout_branch": None, "checkout_commit": None, "selected_source_commit": f["value"], "identity_role": "source_inspection_fixed_Linux_v6.12", "evidence": f["evidence"], "reason": "Official version used for source/lifecycle design; not a native kernel build or workload execution."})
    if identifier == "FIRST-PTE-INTERFACE-COST":
        f = next(f for f in facts if f["evidence"]["path"].endswith("/root/input-binding.json") and f["field"] == "head")
        b = next(f for f in facts if f["evidence"]["path"].endswith("/root/input-binding.json") and f["field"] == "branch")
        bindings.append({"repository": "work", "checkout_branch": b["value"], "checkout_commit": f["value"], "identity_role": "historical_analysis_checkout_not_system_performance_execution", "evidence": [f["evidence"], b["evidence"]], "reason": "Input binding explicitly records branch/HEAD for this source-driven design round; it does not identify later RTL runs."})
    if identifier == "AXI-BOUNDARY-CHECKER":
        f = next(f for f in facts if f["field"] == "commit")
        bindings.append({"repository": "iommu-platform/cva6/corev_apu/riscv-iommu", "checkout_branch": None, "checkout_commit": None, "selected_source_commit": f["value"], "identity_role": "accepted_fix_commit_with_separate_recorded_RTL_hashes", "evidence": f["evidence"], "reason": "Acceptance identifies fix commit; exact test RTL is separately named by rtl_sha256/before_rtl_sha256. The field does not record the test-time branch."})
    return bindings


def report_anchors(report: Path) -> list[int]:
    if report.suffix != ".md":
        return []
    hits = [i for i, line in enumerate(report.read_text(errors="replace").splitlines(), 1) if re.search(r"实际|执行|运行|构建|commit|manifest|sha256|source|identity|模型|边界", line, re.I)]
    return hits[:20]


def main() -> None:
    original_index = HERE / "index.json"
    old = json.loads(original_index.read_text())
    original_dirs = {r.get("output_dir") for r in old["records"]}
    all_originals, records = {}, []
    for identifier, report_name, directory_name, layer, status, scope in SPECS:
        report = ROOT / report_name if report_name.startswith("iommu-platform/") else ROOT / "iommu-platform/docs" / report_name
        directory = OUT / directory_name
        if not report.is_file() or not directory.is_dir():
            raise SystemExit(f"missing explicit family: {identifier}: {report}, {directory}")
        sources = metadata(report, directory, EXTRA.get(identifier, []))
        identities, commands, hashes, summaries = [], [], [], []
        for p in sources:
            all_originals[relative(p)] = digest(p)
            if p.suffix != ".json":
                continue
            try:
                facts, cmds, stats, samples = inspect_json(p)
            except (json.JSONDecodeError, UnicodeError) as e:
                raise SystemExit(f"invalid selected original JSON {p}: {e}")
            identities += facts
            commands += cmds
            hashes += samples
            if stats.get("recorded_file_hash_entries", 0) or stats.get("recorded_named_or_mapped_identity_hash_entries", 0):
                summaries.append({"evidence": ref(p), **stats, "original_hash_role": "inspect_original_context; source_analysis_inputs_and_source_guards_are_not_automatically_actual_build_inputs", "full_recorded_hashes": "retained_in_original_manifest; samples_below_are_not_a_complete_input_closure"})
        record = {
            "experiment_id": identifier,
            "coverage_scope": {"unit": "historical_family_or_buildgroup; not_individual_run", "description": scope, "output_directories": [relative(directory)], "source": "docs/research-results.md and its linked reports/README; bounded metadata followup", "duplicates_original_index_output_directory": relative(directory) in original_dirs},
            "evidence_layer": layer,
            "execution_status": status,
            "original_report": {**ref(report), "line_anchors": report_anchors(report)},
            "original_evidence": [ref(p) for p in sources],
            "repositories": [{"repository": "historical_family_analysis_or_build_checkout", "checkout_branch": None, "checkout_commit": None, "reason": "A complete contemporaneous family-level checkout record is not established by the selected evidence. Original Git/ref facts below retain source/inspection/freeze/build roles; current and later organized/archive commits are not substituted.", "recorded_git_facts": identities}],
            "build_inputs": {"source_manifest_summaries": summaries, "recorded_source_hash_samples": hashes, "complete_input_closure_verified": False, "scope_limit": "Original source hash lists/commands are bound by file SHA256 and JSON pointers. Referenced, protected or analyzed sources are not all compiled inputs. No new source closure or rebuild is claimed."},
            "build_and_run_command_fields": commands,
            "buildgroups": [],
            "result_archival_commits": {"execution_source_equivalence": "not_asserted", "reason": "Subsequent feature/source organization and work/main archive commits are not historical execution identities."},
            "known_unknowns": ["Historical checkout branch/HEAD is null unless explicitly established; original recorded Git facts retain their context.", "No complete dirty/untracked/include/generated-input dependency closure is certified by this supplemental index.", "Missing build/run/load fields are not reconstructed from current checkout, current refs, later commits, or a rerun."],
            "performed_in_this_backfill": "static_read_hash_and_JSON_pointer_extraction_only; no_analyzer_experiment_compile_synthesis_route_or_board_action",
        }
        record["repositories"] += qualified_bindings(identifier, identities)
        if identifier == "R107-ORDINARY-BOARD":
            base = directory / "build"
            repair = directory / "repair-1/build"
            record["buildgroups"] = [
                {"buildgroup_id": "stage111-initial", "role": "initial_software_build_and_failed_pilot_before_idle_repair", "manifest": ref(base / "artifact-manifest.json"), "source_manifest": ref(base / "linux-reviewed-source-manifest.json"), "run_relation": "not_the_final_successful_repair1_result"},
                {"buildgroup_id": "stage111-repair1", "role": "repaired_software_build_used_for_final_ordinary_pilot", "manifest": ref(repair / "artifact-manifest.json"), "source_manifest": ref(repair / "linux-reviewed-source-manifest.json"), "source_delta": ref(repair / "linux-source-delta.json"), "run_receipt": ref(directory / "repair-1/live/finite-pilot-result/receipt.json"), "run_validation": ref(directory / "repair-1/validation/actual-finite-pilot-validation.json")},
            ]
        if identifier == "D-CS1":
            record["known_unknowns"].append("Original generation omitted full source hashes; later enhanced replay/current source hashes cannot certify the original generator version. Original event replay remains distinct.")
        if identifier == "ROW-G0":
            record["known_unknowns"].append("Reused compiled model metadata/source guards were checked at that historical run, not a source signature from its earlier build. Installed Verilator differed from old build metadata.")
        if identifier == "STAGE117-DELIVERY-G0":
            record["known_unknowns"].append("Consult original build-history-custody.json before claiming old reused archive inputs were signed at original build time.")
        if identifier == "MIG-DEBT-OBSERVATION":
            record["known_unknowns"].append("BIT/MCS generation is not timing acceptance; final build was not loaded, no debt capture exists; paused goal remains paused.")
        records.append(record)
    originals_after = {p: digest(ROOT / p) for p in all_originals}
    if originals_after != all_originals:
        raise SystemExit("An original changed during static indexing; do not publish stale refs.")
    payload = {
        "schema": "supplemental-experiment-identities-v1",
        "scope": "Additive static backfill of important families linked from the result/navigation reports, excluding the original 79-record scope and the latest invalidation campaign handled by sibling sidecars. No old files altered or experiments rerun.",
        "coverage": {"record_unit": "family_or_buildgroup_not_run", "supplemental_family_count": len(records), "original_index_record_count": len(old["records"]), "original_index": ref(original_index), "evidence_file_count": len(all_originals), "originals_unchanged_during_backfill": True, "evidence_layers": dict(Counter(r["evidence_layer"] for r in records)), "enumeration_boundary": "Not an exhaustive scan of all out/, retired source trees, Linux or build outputs. Includes result-index-linked important source/model/functional/build/board families and directly linked precursor rounds; original index families and invalidation run rows are maintained by separate sidecars.", "historical_checkout_null_policy": "Unknown or not established is JSON null with a reason; recorded source refs are retained separately and never treated as checkout/execute proof.", "new_ref_policy": "Current organized refs and archival commits are not used to fill missing historical execution branch/HEAD."},
        "records": records,
    }
    json_path = HERE / "supplemental-identities.json"
    json_path.write_text(json.dumps(payload, ensure_ascii=False, indent=2) + "\n")
    fields = list(records[0])
    buf = io.StringIO(newline="")
    writer = csv.DictWriter(buf, fieldnames=fields, lineterminator="\n")
    writer.writeheader()
    for r in records:
        writer.writerow({k: json.dumps(r[k], ensure_ascii=False, separators=(",", ":")) if isinstance(r[k], (dict, list)) else r[k] for k in fields})
    (HERE / "supplemental-identities.csv").write_text(buf.getvalue(), newline="")
    print(json.dumps({"family_count": len(records), "evidence_file_count": len(all_originals), "json_bytes": json_path.stat().st_size, "recorded_git_fact_count": sum(len(r["repositories"][0]["recorded_git_facts"]) for r in records), "source_manifest_count": sum(len(r["build_inputs"]["source_manifest_summaries"]) for r in records)}, ensure_ascii=False))


if __name__ == "__main__":
    main()
