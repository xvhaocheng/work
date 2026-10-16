#!/usr/bin/env python3
"""Plot accepted row-DMA records; never execute or modify DMA experiments."""

import argparse
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path
import sys

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def physical_pages(events, page_bytes):
    pages = set()
    for event in events:
        for key in ("src_pa", "dst_pa"):
            first = int(event[key]) // page_bytes
            last = (int(event[key]) + int(event["bytes"]) - 1) // page_bytes
            pages.update(range(first, last + 1))
    return pages


def require(condition, message):
    if not condition:
        raise ValueError(message)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input-dir", type=Path, default=Path(__file__).resolve().parent)
    args = parser.parse_args()
    base = args.input_dir.resolve()
    started = datetime.now(timezone.utc).isoformat()
    inputs = {
        name: base / name
        for name in (
            "row-baseline-register.json",
            "row-prefilter-derived.json",
            "row-p-4k.json",
            "row-p-2m.json",
        )
    }
    input_hashes = {name: sha256(path) for name, path in inputs.items()}
    data = {name: json.loads(path.read_text()) for name, path in inputs.items()}
    register = data["row-baseline-register.json"]
    derived = data["row-prefilter-derived.json"]
    contract = register["contract"]
    window_end = int(derived["steady_final_deadline"])
    require(window_end == 1656250, "Unexpected registered observation window")
    require(contract["cpu_enable"] is False, "Registered CPU background must be off")
    require(contract["background_copies"] == 2048, "Expected finite A=2048")

    cases = []
    for mapping, color, name in (
        ("4k", "#2066a8", "4 KiB leaves"),
        ("2m", "#d87525", "2 MiB leaves"),
    ):
        raw = data[f"row-p-{mapping}.json"]
        require(raw["mapping"] == mapping and raw["mode"] == "p", "Mapping/policy mismatch")
        require(raw["cpu_enable"] == 0 and raw["ds_priority"] is False,
                "Figure requires CPU off and no DS-priority intervention")
        steady = sorted((r for r in raw["rows"] if r["phase"] == "steady"),
                        key=lambda r: r["job"])
        init = [r for r in raw["rows"] if r["phase"] == "initialization"]
        background = sorted(raw["background"], key=lambda r: r["done_cycle"])
        require(len(steady) == contract["steady_rows"] == 477, "Wrong steady-row count")
        require(len(init) == contract["init_rows"] == 3, "Wrong initialization-row count")
        require(len(background) == contract["background_copies"], "Incomplete background")
        latency = []
        budget = []
        slack = []
        for j, row in enumerate(steady):
            release = (j * 31250 + 8) // 9
            deadline = ((j + 1) * 31250) // 9
            require(row["release_cycle"] == row["ready_cycle"] == release,
                    f"Bad release for {mapping} row {j}")
            require(row["deadline_cycle"] == deadline,
                    f"Bad deadline for {mapping} row {j}")
            require(row["done_absolute"] - raw["prepared_t0_absolute"] == row["done_cycle"],
                    f"Bad steady origin for {mapping} row {j}")
            latency.append(int(row["done_cycle"]) - release)
            budget.append(deadline - release)
            slack.append(deadline - int(row["done_cycle"]))
        require(steady[-1]["deadline_cycle"] == window_end, "Wrong final deadline")
        require(min(latency) >= 0, "Completion precedes release")
        missed = sum(value < 0 for value in slack)
        require(missed == raw["deadline_misses"], "Deadline count mismatch")
        init_cycles = raw["init_end_absolute"] - raw["init_start_absolute"]
        prep_cycles = raw["prepared_t0_absolute"] - raw["init_end_absolute"]
        require(init_cycles == raw["initialization_cycles"], "Initialization timing mismatch")
        require(prep_cycles == raw["preparation_cycles"] == 65, "Preparation timing mismatch")
        completed = [e for e in background if 0 <= e["done_cycle"] <= window_end]
        require(sum(e["bytes"] for e in background) == contract["background_bytes"],
                "Background work mismatch")
        events = raw["rows"] + background
        page_bytes = 4096 if mapping == "4k" else 2 * 1024 * 1024
        reserved_bytes = len(physical_pages(events, page_bytes)) * page_bytes
        cost_key = "minimum_4k_bytes" if mapping == "4k" else "three_2m_region_bytes"
        require(reserved_bytes == derived["physical_reservation_cost"][cost_key],
                "Physical reservation differs from registered geometry")
        cases.append({
            "mapping": mapping,
            "label": name,
            "color": color,
            "raw": raw,
            "steady": steady,
            "completed": completed,
            "latency": latency,
            "budget": budget,
            "table": {
                "mapping": mapping,
                "physical_reserved_bytes": reserved_bytes,
                "mapped_leaf_windows": reserved_bytes // page_bytes,
                "physical_cost_kind": "reserved/authorized physical capacity; not host sparse-model RAM",
                "initialization_rows": len(init),
                "initialization_actual_cycles": init_cycles,
                "preparation_actual_cycles": prep_cycles,
                "initialization_plus_preparation_cycles": init_cycles + prep_cycles,
                "runtime_mode_switches": 0,
                "new_controls": 0,
                "steady_rows": len(steady),
                "row_release_to_true_done_cycles": {
                    "min": min(latency),
                    "max": max(latency),
                    "minimum_deadline_margin": min(slack),
                    "deadline_misses": missed,
                },
                "background_full_copies": len(background),
                "background_full_effective_bytes": sum(e["bytes"] for e in background),
                "background_final_true_done_cycle": int(background[-1]["done_cycle"]),
                "background_window_completed_copies": len(completed),
                "background_window_completed_effective_bytes": sum(e["bytes"] for e in completed),
                "background_window_completed_effective_mib": sum(e["bytes"] for e in completed) / 2**20,
                "quiet_cycle_relative_to_prepared_t0": int(raw["quiet_cycle"]),
            },
        })

    padding = cases[1]["table"]["physical_reserved_bytes"] - cases[0]["table"]["physical_reserved_bytes"]
    require(padding == derived["physical_reservation_cost"]["extra_bytes"], "Padding mismatch")
    table = {
        "schema": "row-baseline-cost-results-v1",
        "evidence_kind": "one deterministic behavioral-DDR execution per case",
        "policy": "fixed P; independent unthrottled A; no DS priority; both CPU off",
        "window": {"origin": "prepared_t0 of each run", "start_cycle": 0,
                   "end_cycle": window_end, "same_relative_window_for_both_cases": True},
        "deadline_budget_cycles": {"min": min(cases[0]["budget"]), "max": max(cases[0]["budget"])},
        "background": {"finite_copies": contract["background_copies"],
                       "full_effective_bytes": contract["background_bytes"],
                       "ring_pages_per_source_or_destination": contract["background_pages_per_ring"]},
        "cases": [case["table"] for case in cases],
        "two_mib_additional_padding_bytes": padding,
        "controls": {"runtime_mode_switches": 0, "new_controller_or_gate": False},
        "unmeasured_costs": ["FPGA row-schedule implementation area/timing", "Linux large-page deployment/allocation/authorization costs"],
        "claim_limits": ["not board/MIG evidence", "not full-frame FPS", "not innovation or new-control benefit", "no statistical repetition or significance claim"],
        "input_sha256": input_hashes,
    }

    plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 10,
                         "axes.spines.top": False, "axes.spines.right": False,
                         "pdf.fonttype": 42})
    fig, axes = plt.subplots(1, 2, figsize=(13.8, 5.6))
    fig.subplots_adjust(left=0.065, right=0.98, bottom=0.24, top=0.76, wspace=0.26)
    fig.suptitle("Registered row baseline: 477 steady rows", y=0.965, fontsize=16, fontweight="bold")
    fig.text(0.5, 0.90,
             "Deterministic behavioral DDR | fixed P | both CPU off | finite A = 2,048 copies (8 MiB)",
             ha="center", fontsize=10)
    left, right = axes
    indices = list(range(contract["steady_rows"]))
    for case in cases:
        left.plot(indices, case["latency"], color=case["color"], linewidth=1.2, alpha=0.9,
                  label=f'{case["label"]}: max {max(case["latency"]):,} cycles')
        x = [0]
        y = [0.0]
        cumulative = 0
        for event in case["completed"]:
            cumulative += int(event["bytes"])
            x.append(int(event["done_cycle"]))
            y.append(cumulative / 2**20)
        x.append(window_end)
        y.append(cumulative / 2**20)
        right.step(x, y, where="post", color=case["color"], linewidth=1.6,
                   label=f'{case["label"]}: {len(case["completed"]):,} copies, {y[-1]:.3f} MiB')
    left.plot(indices, cases[0]["budget"], color="#353b42", linewidth=1.2,
              linestyle="--", label=f'Deadline budget: {min(cases[0]["budget"]):,}-{max(cases[0]["budget"]):,} cycles')
    left.set_title("(a) Release to true DMA completion", loc="left", fontsize=12)
    left.set_xlabel("Steady row index j (0-476; input rows 3-479)")
    left.set_ylabel("Cycles")
    left.set_xlim(0, contract["steady_rows"] - 1)
    left.set_ylim(0, max(cases[0]["budget"]) * 1.18)
    left.legend(loc="upper right", fontsize=8.7, frameon=True, framealpha=0.94)
    left.text(0.02, 0.06,
              "All 477 rows meet deadlines in both records.\nCompletion includes real DMA write responses.",
              transform=left.transAxes, fontsize=9,
              bbox={"facecolor": "white", "alpha": 0.92, "edgecolor": "#dce1e7"})
    right.set_title("(b) Background completed effective data", loc="left", fontsize=12)
    right.set_xlabel("Cycles since prepared t0 (same fixed observation window)")
    right.set_ylabel("Completed effective MiB")
    right.set_xlim(0, window_end)
    right.set_ylim(0, contract["background_bytes"] / 2**20 * 1.12)
    right.xaxis.set_major_formatter(FuncFormatter(lambda value, _: f"{value / 1e6:.2f}M"))
    right.legend(loc="upper left", fontsize=8.7, frameon=True, framealpha=0.94)
    right.text(0.02, 0.05,
               f"Fixed window: 0-{window_end:,} cycles\n2 MiB curve saturates when finite A completes.",
               transform=right.transAxes, fontsize=9,
               bbox={"facecolor": "white", "alpha": 0.92, "edgecolor": "#dce1e7"})
    for axis in axes:
        axis.grid(alpha=0.17)
        axis.set_axisbelow(True)
    fig.text(0.065, 0.125,
             "Physical reservation: 4 KiB = " + f'{cases[0]["table"]["physical_reserved_bytes"]:,}'
             + " B; 2 MiB = " + f'{cases[1]["table"]["physical_reserved_bytes"]:,}'
             + " B (three windows); extra padding = " + f"{padding:,} B.", fontsize=9)
    fig.text(0.065, 0.080,
             "Initialization (3 actual rows): "
             + "; ".join(f'{c["label"]} {c["table"]["initialization_actual_cycles"]:,} cycles' for c in cases)
             + f'; preparation {cases[0]["table"]["preparation_actual_cycles"]} cycles each; no mode switches/new control.', fontsize=9)
    fig.text(0.065, 0.035,
             "Reservation is authorized physical capacity, not sparse host RAM. No board, full-frame FPS or innovation claim.",
             fontsize=9, color="#515963")

    png = base / "row-baseline-results.png"
    pdf = base / "row-baseline-results.pdf"
    table_path = base / "row-cost-table.json"
    ledger_path = base / "plot-execution.json"
    fig.savefig(png, dpi=180)
    fig.savefig(pdf, metadata={"Title": "Registered row baseline: behavioral DDR", "Author": "Codex"})
    plt.close(fig)
    table_path.write_text(json.dumps(table, ensure_ascii=False, indent=2) + "\n")
    require(input_hashes == {name: sha256(path) for name, path in inputs.items()}, "Input was modified")
    ledger = {
        "schema": "row-plot-execution-v1",
        "author_and_executor": "Codex",
        "started_utc": started,
        "completed_utc": datetime.now(timezone.utc).isoformat(),
        "status": "completed",
        "exit_code": 0,
        "python_executable": sys.executable,
        "python_version": sys.version.split()[0],
        "matplotlib_version": matplotlib.__version__,
        "script": str(Path(__file__).resolve()),
        "script_sha256": sha256(Path(__file__)),
        "input_sha256": input_hashes,
        "inputs_unchanged": True,
        "outputs": [{"path": str(p), "bytes": p.stat().st_size, "sha256": sha256(p)}
                    for p in (png, pdf, table_path)],
        "checks": ["477 steady releases/deadlines aligned to register", "latencies from true done",
                   "same fixed background window", "3 actual initialization rows and preparation billing",
                   "reserved physical geometry matched derived file", "input hashes unchanged"],
        "execution_counts": {"postprocessing_runs": 1, "dma_simulations": 0, "builds": 0,
                             "board_runs": 0, "new_environments": 0},
        "root_acceptance": "pending",
    }
    ledger_path.write_text(json.dumps(ledger, ensure_ascii=False, indent=2) + "\n")
    print(json.dumps({"cost_table": table, "cost_table_path": str(table_path),
                      "png_path": str(png), "pdf_path": str(pdf),
                      "execution_ledger_path": str(ledger_path)}, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
