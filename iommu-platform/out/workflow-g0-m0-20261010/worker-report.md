```python
#!/usr/bin/env python3
"""Offline analyzer for the G0/M0 full-batch traces.

Input directory must contain:
  baseline-register.json
  results.json
  <case_id>.csv for each case in baseline-register.json
  (or traces/<case_id>.csv)

Outputs in the same directory:
  trace-statistics.json
  results-table.csv
  completion-comparison.svg
  completion-comparison.png

This script performs offline event matching only.  It does not run or
re-run any simulation.
"""

import argparse
import csv
import json
import sys
from collections import defaultdict, deque
from pathlib import Path


PORT_BY_TAG = {
    32: "port2_payload",
    36: "port3_ds",
}


def die(message):
    raise SystemExit(f"ERROR: {message}")


def load_json(path):
    if not path.is_file():
        die(f"missing required JSON: {path}")
    with path.open("r", encoding="utf-8") as f:
        return json.load(f)


def resolve_trace(input_dir, case_id):
    candidates = [
        input_dir / f"{case_id}.csv",
        input_dir / "traces" / f"{case_id}.csv",
    ]
    for path in candidates:
        if path.is_file():
            return path
    die(f"missing trace CSV for {case_id}; tried {candidates}")


def new_lat():
    return {"count": 0, "sum": 0, "max": 0}


def finalize_lat(lat):
    count = lat["count"]
    return {
        "count": count,
        "sum": lat["sum"],
        "mean": (lat["sum"] / count) if count else None,
        "max": lat["max"],
    }


def remove_at(queue, index):
    """Remove queue[index] while preserving deque order."""
    if index == 0:
        queue.popleft()
    else:
        queue.rotate(-index)
        queue.popleft()
        queue.rotate(index)


def analyze_case_trace(trace_path, summary_ptw_active_cycles):
    buckets = defaultdict(deque)
    per_port = defaultdict(lambda: {
        "ar_to_first_r": new_lat(),
        "ar_to_last_r": new_lat(),
    })
    for port in ("port2_payload", "port3_ds"):
        per_port[port]

    ar_by_tag = defaultdict(int)
    r_by_tag = defaultdict(int)

    total_ar = 0
    expected_r_beats = 0
    actual_r_beats = 0
    completed = 0
    unmatched_r = 0
    order_violations = 0
    beat_mismatch = 0

    ptw_open = defaultdict(deque)
    ptw_intervals = defaultdict(list)
    ptw_unmatched_end = 0
    fullt0_events = 0

    with trace_path.open("r", encoding="utf-8-sig", newline="") as f:
        reader = csv.DictReader(f)
        required = {"cycle", "event", "tag", "address", "value"}
        if not required.issubset(set(reader.fieldnames or [])):
            die(f"{trace_path}: missing columns; need {sorted(required)}")

        for row in reader:
            event = row["event"].strip()
            try:
                cycle = int(row["cycle"])
            except ValueError:
                die(f"{trace_path}: bad cycle value {row['cycle']!r}")

            if event == "full_ddr_ar":
                tag = int(row["tag"])
                addr = int(row["address"])
                value = int(row["value"])

                # Sample trace uses value=255 for a 256-beat request.
                length = value + 1

                ar_by_tag[tag] += 1
                total_ar += 1
                expected_r_beats += length
                buckets[tag].append({
                    "id": tag,
                    "addr": addr,
                    "len": length,
                    "cycle": cycle,
                    "next_beat": 0,
                    "beats": 0,
                    "first_r": None,
                    "last_r": None,
                })

            elif event == "full_ddr_r":
                tag = int(row["tag"])
                addr = int(row["address"])
                beat = int(row["value"])

                actual_r_beats += 1
                r_by_tag[tag] += 1

                queue = buckets.get(tag)
                if not queue:
                    unmatched_r += 1
                    continue

                index = None
                for i, ar in enumerate(queue):
                    if ar["addr"] == addr:
                        index = i
                        break

                if index is None:
                    unmatched_r += 1
                    continue

                if index != 0:
                    order_violations += 1

                ar = queue[index]
                if beat != ar["next_beat"]:
                    beat_mismatch += 1
                    continue

                ar["beats"] += 1
                ar["next_beat"] += 1
                if ar["first_r"] is None:
                    ar["first_r"] = cycle
                ar["last_r"] = cycle

                if ar["beats"] == ar["len"]:
                    remove_at(queue, index)
                    completed += 1

                    port = PORT_BY_TAG.get(tag, f"tag{tag}")
                    lat_first = ar["first_r"] - ar["cycle"]
                    lat_last = ar["last_r"] - ar["cycle"]

                    d = per_port[port]["ar_to_first_r"]
                    d["count"] += 1
                    d["sum"] += lat_first
                    d["max"] = max(d["max"], lat_first)

                    d = per_port[port]["ar_to_last_r"]
                    d["count"] += 1
                    d["sum"] += lat_last
                    d["max"] = max(d["max"], lat_last)

            elif event == "full_ptw_start":
                tag = int(row["tag"])
                ptw_open[tag].append(cycle)

            elif event == "full_ptw_end":
                tag = int(row["tag"])
                if ptw_open[tag]:
                    start = ptw_open[tag].popleft()
                    ptw_intervals[tag].append(cycle - start)
                else:
                    ptw_unmatched_end += 1

            elif event.startswith("fullt0"):
                fullt0_events += 1

    pending = sum(len(queue) for queue in buckets.values())
    ptw_unmatched_start = sum(len(queue) for queue in ptw_open.values())
    matched_ptw_total = sum(sum(intervals) for intervals in ptw_intervals.values())

    tail_ok = (
        completed == total_ar
        and pending == 0
        and beat_mismatch == 0
    )

    if ptw_unmatched_start or ptw_unmatched_end or fullt0_events:
        ptw_consistency = "unmatched_activity"
    elif summary_ptw_active_cycles is None:
        ptw_consistency = "no_summary"
    elif matched_ptw_total == summary_ptw_active_cycles:
        ptw_consistency = "consistent"
    else:
        ptw_consistency = "mismatch"

    return {
        "trace_file": str(trace_path),
        "ar_requests": total_ar,
        "r_beats_actual": actual_r_beats,
        "r_beats_expected": expected_r_beats,
        "completed_ars": completed,
        "pending_ars": pending,
        "unmatched_r_beats": unmatched_r,
        "same_id_order_violations": order_violations,
        "beat_index_mismatches": beat_mismatch,
        "all_requests_tail_complete": tail_ok,
        "all_r_beats_accounted": (
            unmatched_r == 0
            and actual_r_beats == expected_r_beats
        ),
        "same_id_order_preserved": order_violations == 0,
        "ar_by_tag": {str(k): v for k, v in sorted(ar_by_tag.items())},
        "r_beats_by_tag": {str(k): v for k, v in sorted(r_by_tag.items())},
        "per_port": {
            port: {
                "ar_to_first_r": finalize_lat(per_port[port]["ar_to_first_r"]),
                "ar_to_last_r": finalize_lat(per_port[port]["ar_to_last_r"]),
            }
            for port in sorted(per_port)
        },
        "ptw": {
            "banks": {
                str(bank): {
                    "intervals": ptw_intervals[bank],
                    "count": len(ptw_intervals[bank]),
                    "sum_cycles": sum(ptw_intervals[bank]),
                }
                for bank in sorted(ptw_intervals)
            },
            "matched_total_cycles": matched_ptw_total,
            "unmatched_start": ptw_unmatched_start,
            "unmatched_end": ptw_unmatched_end,
            "fullt0_events": fullt0_events,
            "summary_ptw_active_cycles": summary_ptw_active_cycles,
            "consistency": ptw_consistency,
        },
    }


def channel_map(record):
    return {channel["id"]: channel for channel in record.get("channels", [])}


def build_table_rows(baseline, records_by_case, trace_stats_by_case):
    rows = []
    for case in baseline["cases"]:
        case_id = case["id"]
        record = records_by_case[case_id]
        channels = channel_map(record)
        v = channels.get("V", {})
        a = channels.get("A", {})
        trace_stats = trace_stats_by_case[case_id]

        rows.append({
            "case_id": case_id,
            "a_pages": case.get("a_pages", record.get("a_pages")),
            "mode": case.get("mode", record.get("scenario")),
            "ds_priority": int(bool(case.get("ds_priority", record.get("ds_priority")))),
            "t0": record.get("t0"),
            "T_V": record.get("T_V"),
            "T_A": record.get("T_A"),
            "T_data": record.get("T_data"),
            "T_return": record.get("T_return"),
            "V_issue": v.get("issue"),
            "V_done": v.get("done"),
            "A_issue": a.get("issue"),
            "A_done": a.get("done"),
            "V_ar": v.get("ar"),
            "A_ar": a.get("ar"),
            "V_r_beats": v.get("r_beats"),
            "A_r_beats": a.get("r_beats"),
            "batch_complete": record.get("batch_complete"),
            "trace_tail_ok": trace_stats["all_requests_tail_complete"],
            "ptw_active_cycles_summary": record.get("raw", {}).get("ptw_active_cycles"),
            "ptw_trace_matched_cycles": trace_stats["ptw"]["matched_total_cycles"],
            "ptw_consistency": trace_stats["ptw"]["consistency"],
        })
    return rows


def write_results_table(input_dir, rows):
    if len(rows) != 10:
        die(f"results-table.csv requires 10 rows, got {len(rows)}")
    path = input_dir / "results-table.csv"
    with path.open("w", encoding="utf-8", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)
    return path


def plot_completion(input_dir, baseline, records_by_case):
    try:
        import matplotlib
        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
    except Exception as exc:
        die(f"matplotlib Agg backend unavailable: {exc}")

    def cases_for(a_pages):
        return [c for c in baseline["cases"] if c.get("a_pages") == a_pages]

    fig, axes = plt.subplots(1, 2, figsize=(12, 4.8))

    for ax, a_pages, title in (
        (axes[0], 1, "A1 (4 modes)"),
        (axes[1], 8, "A8 (6 modes)"),
    ):
        cases = cases_for(a_pages)
        labels = []
        v_values = []
        a_values = []

        for case in cases:
            case_id = case["id"]
            record = records_by_case[case_id]
            mode = case.get("mode", record.get("scenario"))
            ds = "+DS" if case.get("ds_priority") else ""
            labels.append(f"{case_id}\n{mode}{ds}")
            v_values.append(record["T_V"])
            a_values.append(record["T_A"])

        x = list(range(len(cases)))
        width = 0.38
        ax.bar([i - width / 2 for i in x], v_values, width=width, label="T_V")
        ax.bar([i + width / 2 for i in x], a_values, width=width, label="T_A")
        ax.set_xticks(x)
        ax.set_xticklabels(labels, fontsize=8)
        ax.set_ylabel("completion cycles")
        ax.set_title(title)
        ax.legend(fontsize=8)
        ax.grid(axis="y", alpha=0.25)

    fig.suptitle(
        "Single deterministic run per case; no statistical sampling / no p99",
        fontsize=9,
    )
    fig.tight_layout(rect=[0, 0, 1, 0.95])

    svg_path = input_dir / "completion-comparison.svg"
    png_path = input_dir / "completion-comparison.png"
    fig.savefig(svg_path)
    fig.savefig(png_path, dpi=160)
    plt.close(fig)
    return svg_path, png_path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input-dir", required=True, type=Path)
    args = parser.parse_args()

    input_dir = args.input_dir
    if not input_dir.is_dir():
        die(f"input directory does not exist: {input_dir}")

    baseline = load_json(input_dir / "baseline-register.json")
    results = load_json(input_dir / "results.json")

    cases = baseline.get("cases")
    if not isinstance(cases, list) or len(cases) != 10:
        die(
            "baseline-register.json: expected exactly 10 cases, "
            f"got {len(cases) if isinstance(cases, list) else 'non-list'}"
        )

    records_by_case = {}
    for record in results.get("records", []):
        case_id = record.get("case_id")
        if case_id:
            records_by_case[case_id] = record

    missing = [case["id"] for case in cases if case["id"] not in records_by_case]
    if missing:
        die(f"results.json missing case records: {missing}")

    trace_stats_by_case = {}
    for case in cases:
        case_id = case["id"]
        trace_path = resolve_trace(input_dir, case_id)
        summary_ptw = records_by_case[case_id].get("raw", {}).get("ptw_active_cycles")
        trace_stats_by_case[case_id] = analyze_case_trace(trace_path, summary_ptw)

    stats = {
        "schema": "trace-statistics-v1",
        "input_dir": str(input_dir),
        "note": (
            "offline event matching only; full_ddr_ar value is treated as last beat "
            "index (len=value+1); no simulated rerun"
        ),
        "cases": [trace_stats_by_case[case["id"]] for case in cases],
    }

    stats_path = input_dir / "trace-statistics.json"
    with stats_path.open("w", encoding="utf-8") as f:
        json.dump(stats, f, indent=2)

    rows = build_table_rows(baseline, records_by_case, trace_stats_by_case)
    table_path = write_results_table(input_dir, rows)
    svg_path, png_path = plot_completion(input_dir, baseline, records_by_case)

    print(f"wrote {stats_path}")
    print(f"wrote {table_path}")
    print(f"wrote {svg_path}")
    print(f"wrote {png_path}")

    bad_tails = [
        case_id
        for case_id, case_stats in trace_stats_by_case.items()
        if not case_stats["all_requests_tail_complete"]
    ]
    if bad_tails:
        print(f"ERROR: incomplete request tails for: {bad_tails}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
```

```markdown
# G0/M0 结果草稿（2026-10-10）

> 草稿状态：目标保存为 `docs/workflow-g0-m0-results-20261010.md`。root 已执行全部 10 个 case，均 rc0 且 checker pass；无需重新生成研究计划。图 `completion-comparison.svg/png` 与 `trace-statistics.json` 由 root 执行 [`scripts/analyze_full_batch_g0.py`](../scripts/analyze_full_batch_g0.py) 产生；本文不伪造 trace 统计结果或运行耗时。

## 结论

- 在固定 `full64-v1` 行为级 DDR 合同下，普通 DS priority 服务消除了本合同的 P/PN 差额，因此基于该差额支持的新 gate 分支到此结束。
- 研究主问题仍保留：这不是对所有普通策略、所有应用、N/S 差异或硬件实现的证明。
- DS 是 behavioral DDR model 的服务规则干预，不是硬件创新，也没有 FPGA 构建或板测。
- 下一步不自动扩矩阵、不自动重启旧候选；任何新轮次需单独论证 loss 或应用合同。

## 关键数值

数值来自 [`root-result-acceptance.json`](../out/workflow-g0-m0-20261010/root-result-acceptance.json) 与 [`results.json`](../out/workflow-g0-m0-20261010/results.json)。

| 组 | T_V | T_A | T_return | 说明 |
| --- | ---: | ---: | ---: | --- |
| A1 s/p/n/pn | 73539 | 72965 | 73544 | 四模式相同；A1 无 PTW walks，邻近负例 |
| A8 P | 86594 | 86082 | 86601 | FIFO 基线 |
| A8 PN | 90283 | 89771 | 90292 | PN V penalty 3689 cycles（+4.26%） |
| A8 P+DS | 76738 | 76164 | 76745 | 普通 DS priority |
| A8 PN+DS | 76738 | 76164 | 76745 | DS 后 P/PN 相等 |

DS V 相对 P 下降 11.38%，相对 PN 下降 15.00%。FIFO P/PN 的 `ptw_active_cycles` 为 16483/20172，DS 两者均为 13485；两个 PN 模式 walk beats 均为 93。

S/N 保留在图和 raw 中；N 有不同 miss/work counts，不能把 N 的差异偷移为同一因果。完整 10 行数据见 [`results-table.csv`](../out/workflow-g0-m0-20261010/results-table.csv)；图保留 A1 四模式、A8 六模式。

## M0 接受检查

[`m0-acceptance.json`](../out/workflow-g0-m0-20261010/m0-acceptance.json) 与 [`baseline-register.json`](../out/workflow-g0-m0-20261010/baseline-register.json) 记录：真实事件驱动、bytes/B 闭合、双顺序（A-first / V-first）、start_delay=128、CPU on/off、watchdog（4 个功能 case、失败不伪装成功）、旧 N 回归、14 个 real-record mutants 被拒、9 个 invalid args。实现仅为测试 C++/SV/runner/checker 与分析脚本；生产 RTL/Linux 未改，没有 FPGA 构建或板测。

## 工件

- [`results.json`](../out/workflow-g0-m0-20261010/results.json)
- [`m0-acceptance.json`](../out/workflow-g0-m0-20261010/m0-acceptance.json)
- [`root-result-acceptance.json`](../out/workflow-g0-m0-20261010/root-result-acceptance.json)
- [`baseline-register.json`](../out/workflow-g0-m0-20261010/baseline-register.json)
- [`workflow-failure-card.md`](../out/workflow-g0-m0-20261010/workflow-failure-card.md)
- [`completion-comparison.svg`](../out/workflow-g0-m0-20261010/completion-comparison.svg)
- [`completion-comparison.png`](../out/workflow-g0-m0-20261010/completion-comparison.png)
- [`trace-statistics.json`](../out/workflow-g0-m0-20261010/trace-statistics.json)
- [`results-table.csv`](../out/workflow-g0-m0-20261010/results-table.csv)
- [`trace-format-sample.csv`](../out/workflow-g0-m0-20261010/trace-format-sample.csv)
- [`scripts/analyze_full_batch_g0.py`](../scripts/analyze_full_batch_g0.py)

## 范围与限制

- fixed static mapping COPY microbenchmark，A1/A8，warm256，gap0，CPU disabled，现有 serial behavioral DDR。
- M0 限制：SYSTEM behavioral DDR model only；CPU on/off 测试不声称固定 CPU workload。
- DS priority 是 C++ 服务模型干预，不是 synthesized hardware 或已评估资源成本。
- N 与 S 有不同 miss/work counts；不由本 P/PN 干预解决，也不自动提升为新主题。
- 每个 case 为单次确定性执行；不把重复确定性当统计样本，不计算 p99。

## 下一步

- Archive this candidate rationale；保留已验证的 full-completion harness。
- 不基于该 P/PN 差额进入 G1 或扩矩阵。
- 不自动重启旧机制；后续轮次需单独论证 loss 或 application contract。
```