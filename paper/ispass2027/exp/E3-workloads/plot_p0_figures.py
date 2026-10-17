#!/usr/bin/env python3
"""Plot the frozen E3 P0 45-case result, without collecting new observations.

The input directory must be the accepted p0-final01 checkpoint. All ratios are
reconstructed from same-boot/block/size A8 and A1 hardware cycles. The accepted
summary and size-effects hashes are pinned to keep this figure set immutable.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import math
import platform
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D


EXPECTED_SHA256 = {
    "summary.json": "8dcbd566277c97abc711e0cb84a306df1677fe939df980792eaf89fb61c58af2",
    "size-effects.json": "97ebd307d1b6b5e1ddc2e1352566f6d5a0db514cd5b881a2e9bccd4aa503d529",
}
SIZES = (64, 1024, 4096)
BLOCKS = tuple(range(5))
CONDITIONS = ("Solo", "A1", "A8")
SAMPLE_COUNT = 45
TRANSFERS = 4096
QUANTILE_METHOD = "Type 7: h=(n-1)*p; linear interpolation"
STATS = ("n", "min", "q1", "median", "q3", "max", "iqr")


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read_json(path: Path):
    return json.loads(path.read_text(encoding="utf-8"))


def close(actual: float, expected: float, description: str) -> None:
    require(math.isclose(actual, expected, rel_tol=1e-12, abs_tol=1e-10),
            f"{description}: {actual!r} != {expected!r}")


def descriptive(values: list[float]) -> dict:
    ordered = sorted(values)

    def quantile(p: float) -> float:
        position = (len(ordered) - 1) * p
        lower, upper = math.floor(position), math.ceil(position)
        return ordered[lower] + (position - lower) * (ordered[upper] - ordered[lower])

    q1, median, q3 = (quantile(p) for p in (0.25, 0.5, 0.75))
    return dict(n=len(ordered), min=ordered[0], q1=q1, median=median,
                q3=q3, max=ordered[-1], iqr=q3 - q1)


def check_stats(values: list[float], accepted: dict, description: str) -> dict:
    result = descriptive(values)
    for statistic in STATS:
        close(result[statistic], accepted[statistic], f"{description}/{statistic}")
    return result


def load_trials(source: Path) -> tuple[dict, dict, list[dict], dict]:
    for filename, expected in EXPECTED_SHA256.items():
        require(digest(source / filename) == expected, f"Frozen {filename} SHA256 mismatch")
    summary = read_json(source / "summary.json")
    effects = read_json(source / "size-effects.json")
    require(effects["source_summary_sha256"] == digest(source / "summary.json"),
            "size-effects does not bind the supplied summary bytes")
    require(summary["campaign_complete"] and summary["collection_status"] == "passed",
            "Input campaign is incomplete or failed")
    require(summary["source_board_cases"] == summary["source_cases"] == SAMPLE_COUNT,
            "Input must contain exactly the accepted 45 board cases")
    require(effects["source_board_cases"] == SAMPLE_COUNT, "Unexpected effect case count")
    require(summary["fixture_only"] is False and effects["fixture_only"] is False,
            "Fixture data are not accepted")

    # Verify the retained records' existing SHA binding. Root already audited
    # all 51 retained sources and raw UART commands; do not repeat that audit.
    index = read_json(source / "sources/index.json")
    require(index["sources"] == summary["snapshot"]["sources"], "Source index differs")
    require(len(index["sources"]) == 51, "Expected the accepted 51-source index")
    record_source = summary["sources"]["records"]
    record_entries = [item for item in index["sources"]
                      if item["original_path"] == record_source["path"]]
    require(len(record_entries) == 1, "Cannot locate retained record source")
    record_entry = record_entries[0]
    records_path = source / "sources" / Path(record_entry["snapshot_path"]).name
    require(digest(records_path) == record_entry["sha256"] == record_source["sha256"],
            "Retained record SHA256 binding failed")
    records = read_json(records_path)
    require(len(records) == SAMPLE_COUNT, "Retained record count differs")
    command_hashes = {item["path"]: item["sha256"] for item in summary["raw_command_sources"]}
    require(len(command_hashes) == SAMPLE_COUNT, "Expected 45 distinct raw command references")

    trials = []
    for row in summary["rows"]:
        size, condition = row["victim_chunk_bytes"], row["condition"]
        require(size in SIZES and condition in CONDITIONS and row["n"] == 5,
                "Unexpected P0 cell")
        for sample in row["samples"]:
            record = records[sample["records_index"]]
            for field in ("boot_id", "sequence", "case", "block", "raw_command_path"):
                require(record[field] == sample[field], f"Trial {field} differs")
            require(record["condition"] == condition and record["victim_chunk_bytes"] == size,
                    "Trial cell differs")
            cycles = record["observer"]["time"]["cycles"]
            require(cycles == sample["hardware_cycles"] and cycles > 0,
                    "Observer hardware cycles differ")
            close(cycles * 1000 / summary["clock_hz"], sample["hardware_ms"], "Hardware ms")
            configuration = record["record"]["input"]
            victim = record["record"]["result"]["victim"]
            require(configuration["victim_transfers"] == victim["completed"] == TRANSFERS,
                    "V descriptor count differs")
            require(configuration["warmup_transfers"] == 256, "Warmup count differs")
            require(victim["completed_bytes"] == size * TRANSFERS, "V effective bytes differ")
            command_sha = command_hashes[sample["raw_command_path"]]
            key = f"{sample['boot_id']}:{sample['sequence']}:{command_sha}"
            trials.append(dict(trial_key=key, boot_id=sample["boot_id"],
                               sequence=sample["sequence"], case=sample["case"],
                               block=sample["block"], victim_chunk_bytes=size,
                               condition=condition, hardware_cycles=cycles,
                               victim_completed_bytes=victim["completed_bytes"],
                               records_index=sample["records_index"],
                               raw_command_sha256=command_sha,
                               raw_command_path=sample["raw_command_path"]))
    require(len(trials) == len({trial["trial_key"] for trial in trials}) == SAMPLE_COUNT,
            "Duplicate or missing trial identity")
    require({trial["boot_id"] for trial in trials} == {summary["boot_id"]},
            "Trials are not from the accepted single boot")
    trial_map = {(trial["block"], trial["victim_chunk_bytes"], trial["condition"]): trial
                 for trial in trials}
    require(set(trial_map) == {(block, size, condition) for block in BLOCKS
                              for size in SIZES for condition in CONDITIONS},
            "P0 block/cell coverage differs")
    bindings = dict(records_snapshot=str(records_path), records_sha256=digest(records_path),
                    source_index_entries=len(index["sources"]),
                    source_index_sha256=digest(source / "sources/index.json"),
                    raw_source_audit="Root completed the 51-source/raw-command audit; not repeated here")
    return summary, effects, sorted(trials, key=lambda item: item["case"]), bindings


def derive(summary: dict, effects: dict, trials: list[dict]) -> tuple[list, list, list]:
    lookup = {(trial["block"], trial["victim_chunk_bytes"], trial["condition"]): trial
              for trial in trials}
    accepted_pairs = {item["victim_chunk_bytes"]: item for item in summary["paired_comparisons"]}
    accepted_effects = {item["victim_chunk_bytes"]: item for item in effects["effects"]}
    pairs, statistics = [], []
    for size in SIZES:
        accepted = accepted_pairs[size]
        require(accepted["n"] == 5 and not accepted["unmatched_A8_blocks"]
                and not accepted["unmatched_A1_blocks"], "Unmatched A8/A1 observations")
        for block in BLOCKS:
            a8, a1 = (lookup[(block, size, condition)] for condition in ("A8", "A1"))
            require(a8["boot_id"] == a1["boot_id"], "Cross-boot pair")
            ratio = a8["hardware_cycles"] / a1["hardware_cycles"]
            pair = dict(victim_chunk_bytes=size, boot_id=a8["boot_id"], block=block,
                        A8_case=a8["case"], A1_case=a1["case"],
                        A8_trial_key=a8["trial_key"], A1_trial_key=a1["trial_key"],
                        A8_hardware_cycles=a8["hardware_cycles"],
                        A1_hardware_cycles=a1["hardware_cycles"],
                        time_ratio_A8_over_A1=ratio,
                        time_increase_percent=100 * (ratio - 1),
                        throughput_loss_percent=100 * (1 - 1 / ratio))
            accepted_pair = next(item for item in accepted["pairs"] if item["block"] == block)
            accepted_effect = next(item for item in accepted_effects[size]["samples"]
                                   if item["block"] == block)
            for field in ("boot_id", "block", "A8_case", "A1_case"):
                require(pair[field] == accepted_pair[field] == accepted_effect[field],
                        f"Accepted pair {field} differs")
            close(ratio, accepted_pair["time_ratio_A8_over_A1"], "Accepted time ratio")
            for metric in ("time_increase_percent", "throughput_loss_percent"):
                close(pair[metric], accepted_effect[metric], metric)
            pairs.append(pair)
        for metric in ("time_increase_percent", "throughput_loss_percent"):
            values = [pair[metric] for pair in pairs if pair["victim_chunk_bytes"] == size]
            result = check_stats(values, accepted_effects[size][metric], f"V{size}/{metric}")
            statistics.append(dict(metric=metric, victim_chunk_bytes=size,
                                   reference_victim_chunk_bytes="", **result))

    pair_lookup = {(pair["block"], pair["victim_chunk_bytes"]): pair for pair in pairs}
    accepted_contrasts = {item["victim_chunk_bytes"]: item for item in effects["size_contrasts"]}
    contrasts = []
    for size in SIZES[:-1]:
        accepted = accepted_contrasts[size]
        for block in BLOCKS:
            small, reference = pair_lookup[(block, size)], pair_lookup[(block, 4096)]
            require(small["boot_id"] == reference["boot_id"], "Cross-boot size contrast")
            contrast = dict(victim_chunk_bytes=size, reference_victim_chunk_bytes=4096,
                            boot_id=small["boot_id"], block=block,
                            small_A8_case=small["A8_case"], small_A1_case=small["A1_case"],
                            reference_A8_case=reference["A8_case"],
                            reference_A1_case=reference["A1_case"],
                            time_increase_difference_pp=small["time_increase_percent"]
                            - reference["time_increase_percent"],
                            throughput_loss_difference_pp=small["throughput_loss_percent"]
                            - reference["throughput_loss_percent"])
            accepted_sample = next(item for item in accepted["samples"] if item["block"] == block)
            for field in ("boot_id", "block", "small_A8_case", "small_A1_case",
                          "reference_A8_case", "reference_A1_case"):
                require(contrast[field] == accepted_sample[field], f"Contrast {field} differs")
            for metric in ("time_increase_difference_pp", "throughput_loss_difference_pp"):
                close(contrast[metric], accepted_sample[metric], metric)
            contrasts.append(contrast)
        for metric in ("time_increase_difference_pp", "throughput_loss_difference_pp"):
            values = [item[metric] for item in contrasts if item["victim_chunk_bytes"] == size]
            result = check_stats(values, accepted[metric], f"V{size}-V4096/{metric}")
            statistics.append(dict(metric=metric, victim_chunk_bytes=size,
                                   reference_victim_chunk_bytes=4096, **result))
    return pairs, contrasts, statistics


def write_csv(path: Path, rows: list[dict]) -> None:
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def plot_points(output: Path, name: str, samples: list[dict], statistics: list[dict],
                metric: str, ylabel: str, sizes: tuple[int, ...], limits: tuple[float, float],
                yticks: tuple[int, ...], is_contrast: bool = False) -> list[str]:
    fig, ax = plt.subplots(figsize=(3.5, 2.75))
    fig.subplots_adjust(left=0.18, right=0.97, bottom=0.20, top=0.82)
    offsets = (-0.22, -0.14, -0.06, 0.02, 0.10)  # Fixed block order, no random jitter.
    median_handle = None
    for position, size in enumerate(sizes):
        selected = sorted((item for item in samples if item["victim_chunk_bytes"] == size),
                          key=lambda item: item["block"])
        require([item["block"] for item in selected] == list(BLOCKS), "Plot needs five blocks")
        ax.scatter([position + offset for offset in offsets], [item[metric] for item in selected],
                   s=15, facecolors="white", edgecolors="0.35", linewidths=0.8, zorder=3)
        stats = next(item for item in statistics if item["metric"] == metric
                     and item["victim_chunk_bytes"] == size)
        median = stats["median"]
        median_handle = ax.errorbar(position + 0.28, median,
                                   yerr=[[median - stats["q1"]], [stats["q3"] - median]],
                                   fmt="_", color="black", markersize=10, markeredgewidth=1.6,
                                   elinewidth=1.1, capsize=3, capthick=0.9, zorder=4)
        ax.text(position + 0.05, max(item[metric] for item in selected) + 0.18,
                f"{median:.2f}", ha="center", va="bottom", fontsize=8)
    ax.set_xlim(-0.45, len(sizes) - 0.35)
    ax.set_ylim(*limits)
    ax.set_yticks(yticks)
    ax.set_xticks(range(len(sizes)), ["64 B" if size == 64 else f"{size // 1024} KiB" for size in sizes])
    ax.set_xlabel("V transfer size" + (" (reference: 4 KiB)" if is_contrast else ""), labelpad=6)
    ax.set_ylabel(ylabel, labelpad=6)
    ax.set_title("DS0 · same boot · n = 5 " + ("contrasts/size" if is_contrast else "pairs/size"),
                 fontsize=8, pad=8)
    ax.grid(axis="y", color="0.88", linewidth=0.5, zorder=0)
    ax.spines[["top", "right"]].set_visible(False)
    ax.tick_params(axis="both", width=0.6, length=3)
    if is_contrast:
        ax.axhline(0, color="0.45", linewidth=0.7)
    raw_handle = Line2D([], [], marker="o", linestyle="none", markerfacecolor="white",
                        markeredgecolor="0.35", markeredgewidth=0.8, markersize=4)
    point_label = "Block contrasts" if is_contrast else "Actual block pairs"
    fig.legend([raw_handle, median_handle], [point_label, "Median / Q1-Q3"],
               loc="upper center", bbox_to_anchor=(0.56, 1.01), ncol=2,
               frameon=False, fontsize=7.4, handlelength=1.5, columnspacing=0.9)
    files = []
    for extension in ("pdf", "svg", "png"):
        path = output / f"{name}.{extension}"
        if extension == "pdf":
            metadata = dict(Creator="plot_p0_figures.py", CreationDate=None, ModDate=None)
        elif extension == "svg":
            metadata = dict(Creator="plot_p0_figures.py", Date=None)
        else:
            metadata = dict(Software="plot_p0_figures.py")
        fig.savefig(path, dpi=300, metadata=metadata)
        if extension == "svg":
            path.write_text("\n".join(line.rstrip() for line in path.read_text().splitlines())
                            + "\n", encoding="utf-8")
        files.append(path.name)
    plt.close(fig)
    return files


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, required=True, help="Frozen p0-final01 checkpoint")
    parser.add_argument("--output", type=Path, required=True, help="New independent output directory")
    args = parser.parse_args()
    source, output = args.input.resolve(), args.output.resolve()
    require(source != output and source not in output.parents and output not in source.parents,
            "Input and output directories must not overlap")
    require(not output.exists(), "Output must be a new directory; retain existing figure sets")
    summary, effects, trials, bindings = load_trials(source)
    pairs, contrasts, statistics = derive(summary, effects, trials)
    output.mkdir(parents=True, exist_ok=False)
    plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 8,
                         "axes.labelsize": 8.5, "xtick.labelsize": 8, "ytick.labelsize": 8,
                         "axes.linewidth": 0.6, "pdf.fonttype": 42, "ps.fonttype": 42,
                         "svg.fonttype": "none", "svg.hashsalt": "e3-p0-main01-v1"})
    write_csv(output / "paired-samples.csv", pairs)
    write_csv(output / "size-contrasts.csv", contrasts)
    write_csv(output / "descriptive-statistics.csv", statistics)
    artifacts = ["paired-samples.csv", "size-contrasts.csv", "descriptive-statistics.csv"]
    artifacts += plot_points(output, "p0-extra-time", pairs, statistics,
                             "time_increase_percent", "Extra time, A8 vs A1 (%)", SIZES,
                             (9.6, 15.6), (10, 11, 12, 13, 14, 15))
    artifacts += plot_points(output, "p0-size-contrast", contrasts, statistics,
                             "time_increase_difference_pp", "Extra-time difference (pp)", SIZES[:-1],
                             (0, 5.2), (0, 1, 2, 3, 4, 5), is_contrast=True)
    manifest = dict(
        schema="ispass2027.e3.p0.figures.v1", status="offline_descriptive_analysis_verified",
        input_checkpoint=str(source), source_board_cases=SAMPLE_COUNT,
        additional_board_cases=0, trial_counting="Reuse of Root's same 45 P0 trials; do not add as new data",
        boot_id=summary["boot_id"], bit_sha256=summary["bit_sha256"],
        kernel_release=summary["kernel_release"], clock_hz=summary["clock_hz"],
        inputs={name: dict(path=str(source / name), sha256=digest(source / name))
                for name in ("summary.json", "size-effects.json", "paper.csv", "README.md", "sources/index.json")},
        source_bindings=bindings, analysis_script_sha256=digest(Path(__file__)),
        runtime=dict(python=platform.python_version(), matplotlib=matplotlib.__version__, backend="Agg"),
        definitions=dict(time_increase_percent="100*(C_A8/C_A1-1)",
                         throughput_loss_percent="100*(1-C_A1/C_A8)",
                         time_increase_difference_pp="same block: small-size time increase minus 4KiB time increase"),
        pairing="same boot, block, victim size; size contrasts pair those ratios within the same block",
        quantile_method=QUANTILE_METHOD, samples_per_size=5,
        workload=dict(victim_descriptors=TRANSFERS, warmup_descriptors=256,
                      victim_completed_bytes={str(size): size * TRANSFERS for size in SIZES},
                      aggressor_transfer_bytes=4096, A1_pages=1, A8_pages=8),
        graphical_encoding=dict(actual_points="five hollow circles, left to right in block order 0..4",
                                summary="black crossbar: median; capped vertical line: Type7 Q1–Q3",
                                canvas_inches=[3.5, 2.75], png_dpi=300,
                                primary_y_axis="9.6..15.6 percent; dot plot uses a nonzero origin"),
        limits=["IQR is descriptive, not a confidence interval; no significance test or star annotations",
                "Single boot and n=5 pairs per size; no platform-wide generalization",
                "Fixed descriptor count changes total V bytes with size; only same-size A8/A1 times are divided",
                "A1 includes co-run bandwidth/arbitration and shared-path costs; ratios do not isolate pure translation"],
        validation=dict(frozen_summary_and_size_effects_sha256="passed",
                        size_effects_to_summary_sha256="passed", retained_records_sha256="passed",
                        trial_keys_and_9_cells_by_5_blocks="passed",
                        cycle_derived_pairs_vs_summary_and_size_effects="passed",
                        paired_type7_statistics_and_within_block_size_contrasts="passed",
                        raw_source_reaudit="not repeated; accepted Root audit retained"),
        trial_keys=trials,
        artifacts={name: dict(sha256=digest(output / name), bytes=(output / name).stat().st_size)
                   for name in artifacts})
    (output / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(dict(output=str(output), source_board_cases=SAMPLE_COUNT,
                          additional_board_cases=0, paired_estimates=len(pairs),
                          within_block_contrasts=len(contrasts), artifacts=artifacts)))


if __name__ == "__main__":
    main()
