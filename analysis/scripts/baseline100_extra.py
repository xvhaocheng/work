#!/usr/bin/env python3
"""Read the 100 saved trials; generate new offline tables without changing inputs.

Python standard library only. Quantiles use Type 7 linear interpolation. The CI
is the percentile interval for the trial median, with 10,000 bootstrap samples.
For DS50 pooled summaries, resample within each observed boot (5 + 5 trials).
"""

import argparse
import csv
import hashlib
import io
import json
import math
from pathlib import Path
import random
import statistics


ROOT = Path(__file__).resolve().parents[2]
BASE = Path("paper/ispass2027/board/runs/baseline100-20261017-01")
OUT = Path("paper/ispass2027/analysis")
PHASES = ("ds0-middle", "ds50-first", "ds50-last")
CONDITIONS = ("Solo", "A1", "A8", "A16", "A64")
SEED = 20261017
RESAMPLES = 10000
CLOCK_HZ = 50_000_000
BURST_BYTES = 2048  # Fixed-source 64-bit AXI, 256 beats; see report source audit.
UNITS = {
    "hardware_ms": "ms", "victim_MiB_s": "MiB/s",
    "aggressor_response_bytes": "bytes", "aggressor_MiB_s_proxy": "MiB/s",
    "total_MiB_s_proxy": "MiB/s", "total_over_solo_percent": "%",
    "victim_over_solo_percent": "%", "victim_over_half_solo_percent": "%",
    "time_over_solo": "ratio", "time_beyond_two_solo_ms": "ms",
    "aggressor_whole_batch_bytes": "bytes",
    "aggressor_stage3_full_response_bytes": "bytes",
    "aggressor_payload_lower_bytes": "bytes", "aggressor_payload_upper_bytes": "bytes",
    "total_MiB_s_payload_lower": "MiB/s", "total_MiB_s_payload_upper": "MiB/s",
    "total_over_solo_lower_percent": "%", "total_over_solo_upper_percent": "%",
    "victim_iotlb_miss_software_window": "count",
    "aggressor_iotlb_miss_software_window": "count",
}
for channel in "rw":
    for stage in range(4):
        UNITS[f"v{channel}{stage}_trial_mean_cycles"] = "cycles"


def quantile(sorted_values, p):
    position = (len(sorted_values) - 1) * p
    lo = math.floor(position)
    hi = math.ceil(position)
    return sorted_values[lo] + (sorted_values[hi] - sorted_values[lo]) * (position - lo)


def csv_text(rows):
    stream = io.StringIO(newline="")
    writer = csv.DictWriter(stream, fieldnames=list(rows[0]), lineterminator="\n")
    writer.writeheader()
    for row in rows:
        writer.writerow({k: "unknown" if v is None else v for k, v in row.items()})
    return stream.getvalue()


def load_trials():
    inputs = {}

    def load(path):
        data = (ROOT / path).read_bytes()
        inputs[str(path)] = hashlib.sha256(data).hexdigest()
        return json.loads(data)

    source = load(Path("paper/ispass2027/board/packages/baseline100-20261017/source.json"))
    trials = []
    manifests = {}
    for phase in PHASES:
        path = BASE / phase / "records.json"
        records = load(path)
        manifest = manifests[phase] = load(BASE / phase / "manifest.json")
        expected = 50 if phase == "ds0-middle" else 25
        assert manifest["status"] == "passed" and len(records) == expected
        assert len({r["boot_id"] for r in records}) == 1
        blocks = range(5, 10) if phase == "ds50-last" else range(expected // 5)
        assert {(r["block"], r["condition"]) for r in records} == {
            (block, condition) for block in blocks for condition in CONDITIONS}
        solos = {r["block"]: (i, r) for i, r in enumerate(records) if r["condition"] == "Solo"}
        for index, r in enumerate(records):
            result = r["record"]["result"]
            arguments = r["record"]["input"]
            time = r["observer"]["time"]
            v, a = r["observer"]["axi_v"], r["observer"]["axi_a"]
            assert r["ds"] == ("0/0" if phase == "ds0-middle" else "50/50")
            assert arguments["chunk_bytes"] == 4096 and arguments["victim_transfers"] == 4096
            assert arguments["warmup_transfers"] == 256
            assert result["status"] == 0 and r["record"]["success"]
            assert time["valid"] == 1 and time["issued"] == time["done"] == 4096
            assert time["cycles"] == time["h"] + time["gap"] and time["gap"] == 4095
            assert v["ar"] == v["aw"] == v["rlast"] == v["b"] == 8192
            assert v["start"] == v["end"] == 0
            assert result["victim"]["completed_bytes"] == 16 * 2**20
            for lane in (result["victim"], result["aggressor"]):
                if lane is not None:
                    assert lane["data_errors"] == lane["quarantined"] == 0
                    assert lane["submitted"] == lane["completed"]
            for lane in (v, a):
                assert (lane["start"] >> 16) + lane["ar"] == lane["rlast"] + (lane["end"] >> 16)
                assert (lane["start"] & 65535) + lane["aw"] == lane["b"] + (lane["end"] & 65535)
            is_solo = r["condition"] == "Solo"
            assert (result["aggressor"] is None) == is_solo
            if not is_solo:
                # These are observed properties, not invented completed descriptors.
                assert a["ar"] == a["aw"] == a["rlast"] == a["b"] == 8192
                assert a["start"] == a["end"] == (2 << 16) + 2
            seconds = time["cycles"] / CLOCK_HZ
            victim_rate = result["victim"]["completed_bytes"] / 2**20 / seconds
            a_bytes = None if is_solo else a["b"] * BURST_BYTES
            total_rate = victim_rate if is_solo else victim_rate + a_bytes / 2**20 / seconds
            solo_index, solo = solos[r["block"]]
            solo_seconds = solo["observer"]["time"]["cycles"] / CLOCK_HZ
            solo_rate = solo["record"]["result"]["victim"]["completed_bytes"] / 2**20 / solo_seconds
            metrics = {
                "hardware_ms": time["cycles"] / 50000, "victim_MiB_s": victim_rate,
                "aggressor_response_bytes": a_bytes,
                "aggressor_MiB_s_proxy": None if is_solo else a_bytes / 2**20 / seconds,
                "total_MiB_s_proxy": total_rate,
                "total_over_solo_percent": total_rate / solo_rate * 100,
                "victim_over_solo_percent": victim_rate / solo_rate * 100,
                "victim_over_half_solo_percent": None if is_solo else victim_rate / (solo_rate / 2) * 100,
                "time_over_solo": seconds / solo_seconds,
                "time_beyond_two_solo_ms": None if is_solo else (seconds - 2 * solo_seconds) * 1000,
                "aggressor_whole_batch_bytes": None if is_solo else result["aggressor"]["completed_bytes"],
                "aggressor_stage3_full_response_bytes": None if is_solo else r["path"]["aw3"]["full"] * BURST_BYTES,
                # One 4KiB task at a time: outstanding W-byte/B-credit stock
                # at either boundary is in [0, 4096]. This is interface traffic,
                # conditional on legal AXI, not a physical DDR timing guarantee.
                "aggressor_payload_lower_bytes": None if is_solo else a_bytes - 4096,
                "aggressor_payload_upper_bytes": None if is_solo else a_bytes + 4096,
                "total_MiB_s_payload_lower": total_rate if is_solo else total_rate - 4096 / 2**20 / seconds,
                "total_MiB_s_payload_upper": total_rate if is_solo else total_rate + 4096 / 2**20 / seconds,
                "total_over_solo_lower_percent": (total_rate if is_solo else total_rate - 4096 / 2**20 / seconds) / solo_rate * 100,
                "total_over_solo_upper_percent": (total_rate if is_solo else total_rate + 4096 / 2**20 / seconds) / solo_rate * 100,
                "victim_iotlb_miss_software_window": result["victim"]["iotlb_miss"],
                "aggressor_iotlb_miss_software_window": None if is_solo else result["aggressor"]["iotlb_miss"],
            }
            for channel in "rw":
                for stage in range(4):
                    key = f"v{channel}{stage}"
                    p = r["path"][key]
                    assert p["valid"] == 1 and p["entered"] == p["full"] == 8192
                    assert p["start"] == p["carry"] == p["end"] == 0
                    assert p["area"] == p["sum"]
                    value = p["sum"] / p["full"]
                    assert value == r["metrics"][f"path_{key}_mean_cycles"]
                    metrics[f"{key}_trial_mean_cycles"] = value
            assert math.isclose(metrics["hardware_ms"], r["metrics"]["hardware_ms"], abs_tol=1e-12)
            assert math.isclose(victim_rate, r["metrics"]["hardware_MiB_s"], abs_tol=1e-12)
            trials.append({
                "phase": phase, "ds": r["ds"], "condition": r["condition"],
                "boot_id": r["boot_id"], "block": r["block"], "case": r["case"],
                "raw_path": str(path), "source_locator": f"/{index}",
                "paired_solo_locator": f"/{solo_index}", "hardware_cycles": time["cycles"],
                "a_rlast": a["rlast"], "a_b": a["b"], "a_start": a["start"], "a_end": a["end"],
                "a_write_stage3_full": r["path"]["aw3"]["full"],
                "a_write_stage3_carry": r["path"]["aw3"]["carry"],
                **metrics,
            })
    assert len(trials) == 100 and len({t["boot_id"] for t in trials}) == 3
    assert manifests["ds50-first"]["bit_sha256"] == manifests["ds50-last"]["bit_sha256"]
    return trials, inputs, manifests, source


def summarize(trials):
    rows = []
    for scope in (*PHASES, "ds50-pooled"):
        for condition in CONDITIONS:
            selected = [t for t in trials if t["condition"] == condition and (
                t["phase"] == scope if scope != "ds50-pooled" else t["ds"] == "50/50")]
            strata = [[i for i, t in enumerate(selected) if t["boot_id"] == boot]
                      for boot in sorted({t["boot_id"] for t in selected})]
            group_seed = int.from_bytes(hashlib.sha256(f"{SEED}:{scope}:{condition}".encode()).digest()[:8], "big")
            rng = random.Random(group_seed)
            draws = [[rng.choice(stratum) for stratum in strata for _ in stratum]
                     for _ in range(RESAMPLES)]
            for metric, unit in UNITS.items():
                values = [t[metric] for t in selected]
                known = [v for v in values if v is not None]
                assert not known or len(known) == len(values)
                if known:
                    ordered = sorted(known)
                    boot = sorted(statistics.median(values[i] for i in draw) for draw in draws)
                    q1, q3 = quantile(ordered, .25), quantile(ordered, .75)
                    numbers = dict(median=statistics.median(known), q1=q1, q3=q3, iqr=q3-q1,
                                   minimum=min(known), maximum=max(known),
                                   ci95_low=quantile(boot, .025), ci95_high=quantile(boot, .975))
                else:
                    numbers = dict.fromkeys(("median", "q1", "q3", "iqr", "minimum", "maximum", "ci95_low", "ci95_high"))
                rows.append(dict(scope=scope, ds=selected[0]["ds"], condition=condition,
                                 boot_ids=";".join(sorted({t["boot_id"] for t in selected})),
                                 n_trials=len(selected), n_values=len(known), metric=metric, unit=unit,
                                 **numbers, ci_target="trial_median", resamples=RESAMPLES,
                                 bootstrap="within_observed_boot", seed=SEED, group_seed=group_seed,
                                 status="not_applicable" if not known else "observed_or_derived"))
    return rows


def tables(stats):
    index = {(r["scope"], r["condition"], r["metric"]): r for r in stats}
    throughput, paths, pooling = [], [], []
    for scope in (*PHASES, "ds50-pooled"):
        for condition in CONDITIONS:
            def med(metric):
                return index[scope, condition, metric]["median"]
            throughput.append(dict(scope=scope, ds=index[scope, condition, "hardware_ms"]["ds"],
                                   condition=condition, n=index[scope, condition, "hardware_ms"]["n_trials"],
                                   **{m: med(m) for m in list(UNITS)[:12]}))
            for channel in "rw":
                paths.append(dict(scope=scope, condition=condition, channel=channel,
                                  **{f"stage{s}_median_cycles": med(f"v{channel}{s}_trial_mean_cycles") for s in range(4)}))
    for condition in CONDITIONS:
        first = index["ds50-first", condition, "hardware_ms"]["median"]
        last = index["ds50-last", condition, "hardware_ms"]["median"]
        pooling.append(dict(condition=condition, first_median_ms=first, last_median_ms=last,
                            last_minus_first_ms=last-first, last_over_first_change_percent=(last/first-1)*100))
    return throughput, paths, pooling


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="Recompute and compare outputs without writing anything")
    args = parser.parse_args()
    trials, inputs, manifests, source = load_trials()
    stats = summarize(trials)
    throughput, paths, pooling = tables(stats)
    outputs = {
        "baseline100-extra-samples.csv": csv_text(trials),
        "baseline100-extra-statistics.csv": csv_text(stats),
        "baseline100-extra-throughput.csv": csv_text(throughput),
        "baseline100-extra-path.csv": csv_text(paths),
        "baseline100-extra-boots.csv": csv_text(pooling),
    }
    # The narrative is generated from the same numbers as the CSV files.
    outputs["baseline100-extra.md"] = render_report(stats, throughput, paths, pooling, manifests, source)
    outputs["baseline100-extra-inputs.json"] = json.dumps({
        "schema": "baseline100-extra-inputs-v1", "seed": SEED, "resamples": RESAMPLES,
        "clock_hz": CLOCK_HZ, "burst_bytes": BURST_BYTES,
        "inputs_sha256": inputs, "n_trials": len(trials),
        "outputs_sha256": {name: hashlib.sha256(text.encode()).hexdigest() for name, text in outputs.items()},
    }, indent=2) + "\n"
    for relative, digest in inputs.items():
        assert hashlib.sha256((ROOT / relative).read_bytes()).hexdigest() == digest, relative
    if not args.check:
        (ROOT / OUT).mkdir(parents=True, exist_ok=True)
    for name, text in outputs.items():
        path = ROOT / OUT / name
        if args.check:
            assert path.read_bytes() == text.encode(), f"Reproduction mismatch: {path}"
        else:
            path.write_bytes(text.encode())
    print(f"{'VERIFIED' if args.check else 'GENERATED'}: 100 trials, 20 groups, {len(stats)} metric summaries; input bytes unchanged.")


def render_report(stats, throughput, paths, pooling, manifests, source):
    index = {(r["scope"], r["condition"], r["metric"]): r for r in stats}
    labels = {"ds0-middle": "DS0", "ds50-first": "DS50 前 boot",
              "ds50-last": "DS50 后 boot", "ds50-pooled": "DS50 合并"}

    def med(scope, condition, metric):
        return index[scope, condition, metric]["median"]

    def fmt(value, digits=3):
        return "—" if value is None else f"{value:.{digits}f}"

    def table(headers, rows):
        return "\n".join(["| " + " | ".join(headers) + " |",
                          "| " + " | ".join("---" for _ in headers) + " |",
                          *("| " + " | ".join(map(str, row)) + " |" for row in rows)])

    a1_share = med("ds0-middle", "A1", "victim_over_solo_percent")
    a1_total = med("ds0-middle", "A1", "total_MiB_s_proxy")
    a1_total_ratio = med("ds0-middle", "A1", "total_over_solo_percent")
    a1_vs_half = med("ds0-middle", "A1", "victim_over_half_solo_percent")
    ds0_iqr = [index["ds0-middle", c, "hardware_ms"]["iqr"] for c in CONDITIONS]
    ds50_iqr = [index["ds50-pooled", c, "hardware_ms"]["iqr"] for c in CONDITIONS]
    lines = [
        "# 统一基线100次：合计吞吐、路径等待与离散度", "",
        "## Material Passport", "",
        "Origin Skill: academic-research-suite / experiment-agent；Mode: validate；Date: 2026-10-17（JST）；"
        "Verification Status: VERIFIED（仅离线重算，不表示重跑硬件）；Version Label: baseline100_extra_v1。", "",
        "仅使用本轮100个已接受试次；G1、旧板测、仿真均未混入。DS0 是原始直连硬件的论文主结果，"
        "DS50 是额外50/50延迟敏感性；两者均沿用本轮固定的原版加必要修复、批量供给与被动观测配置。", "",
        "## 1. 双方合计吞吐", "",
        f"80个共跑试次的 A 在 V 硬件窗口内均完成8192个写响应，按固定2048字节突发记账为16 MiB；"
        f"严格同窗 W 有效字节只能给出16 MiB ±4096字节的边界区间，不能把 A 整批排空后的字节数当成同窗量。"
        f"DS0 A1 的 V 保留 Solo 的{a1_share:.3f}%，比理想50%高{a1_share-50:.3f}个百分点，"
        f"相对半份带宽高{a1_vs_half-100:.3f}%，因此没有超出平分的额外损失；"
        f"合计约{a1_total:.3f} MiB/s、为同区组Solo的{a1_total_ratio:.3f}%，是接近平分且总量略增，并非严格守恒。"
        f"DS0 A8/A16/A64合计为Solo的"
        f"{med('ds0-middle','A8','total_over_solo_percent'):.3f}%/"
        f"{med('ds0-middle','A16','total_over_solo_percent'):.3f}%/"
        f"{med('ds0-middle','A64','total_over_solo_percent'):.3f}%，此时才出现低于Solo的总吞吐损失；"
        "DS50的相应损失更大，但不解释为纯PTW成本。", "",
        "以下均为逐试次计算后的中位数，COPY字节只记一次，不把读流量与写流量再相加；"
        "A与合计列是按响应完成量折算的近似吞吐（proxy）。"
        "“合计/Solo”先与同phase、同block的Solo配对，再取中位，故可能与两列中位数相除略有差异；"
        "“边界区间”是该配对比值下界/上界各自的中位数，不是置信区间。", "",
        table(["配置", "条件", "n", "A响应记账 MiB", "V MiB/s", "A MiB/s ≈", "合计 MiB/s ≈", "合计/Solo %", "边界区间 %"],
              [[labels[r["scope"]], r["condition"], r["n"],
                fmt(None if r["aggressor_response_bytes"] is None else r["aggressor_response_bytes"] / 2**20, 6),
                fmt(r["victim_MiB_s"]), fmt(r["aggressor_MiB_s_proxy"]), fmt(r["total_MiB_s_proxy"]),
                fmt(r["total_over_solo_percent"]),
                f"[{med(r['scope'],r['condition'],'total_over_solo_lower_percent'):.3f}, "
                f"{med(r['scope'],r['condition'],'total_over_solo_upper_percent'):.3f}]"] for r in throughput]), "",
        f"DS0 A1 配对时间比的中位数为{med('ds0-middle','A1','time_over_solo'):.6f}×；"
        f"相对2×Solo的配对时间差中位为{med('ds0-middle','A1','time_beyond_two_solo_ms'):.5f} ms。"
        "总吞吐略高于Solo只说明本配置下完成记账的观测关系，现有数据不足以区分具体共享资源的利用率变化，"
        "也不能据此证明每条路径的带宽上限。", "",
        "## 2. V 的 IOMMU 路径等待", "",
        f"DS0读路径内部阶段的试次均值中位数为Solo/A1=25、A8="
        f"{med('ds0-middle','A8','vr1_trial_mean_cycles'):.3f}、A16="
        f"{med('ds0-middle','A16','vr1_trial_mean_cycles'):.3f}、A64="
        f"{med('ds0-middle','A64','vr1_trial_mean_cycles'):.3f}周期，表明原始直连配置也有内部等待增长。"
        f"DS50合并的A8为{med('ds50-pooled','A8','vr1_trial_mean_cycles'):.3f}周期，"
        "因此既有25→601周期结论是延迟敏感性结果，不能替代DS0的25→143周期。"
        "两种DS下入口均为1周期、出口地址反压均为0，A1主要增加后段时延；"
        "这些是每笔突发的阶段延迟，读写及事务之间重叠，不能相加解释整批慢化。", "",
        "每个 V 阶段每试次均有8192个完整阶段样本（start=carry=end=0），"
        "先算sum/full，再跨试次取中位。单位为50MHz时钟周期；"
        "下表也保留DS50两boot各n=5的独立结果。", "",
        table(["配置", "条件", "读入口", "读内部", "读出口地址反压", "读后段", "写入口", "写内部", "写出口地址反压", "写后段"],
              [[labels[scope], c,
                *[fmt(med(scope,c,f"v{ch}{s}_trial_mean_cycles")) for ch in "rw" for s in range(4)]]
               for scope in (*PHASES,"ds50-pooled") for c in CONDITIONS]), "",
        "四阶段分别为：私有DMA地址首次VALID→IOMMU入口地址握手；"
        "该握手→IOMMU出口地址首次VALID；出口首次VALID→地址握手；"
        "出口地址握手→读RLAST/写B返回。内部阶段包括IOMMU与其内部缓冲/排队，"
        "不是纯页表遍历服务时间；后段包含下游数据/响应服务。", "",
        "## 3. 离散度与95% bootstrap置信区间", "",
        f"DS0五条件硬件时间IQR为{min(ds0_iqr):.5f}–{max(ds0_iqr):.5f} ms，"
        f"DS50合并为{min(ds50_iqr):.5f}–{max(ds50_iqr):.5f} ms；"
        "全部样本保留，包括DS50前boot的A64最小值。"
        f"DS50后boot各条件时间中位数比前boot高{min(r['last_over_first_change_percent'] for r in pooling):.3f}%–"
        f"{max(r['last_over_first_change_percent'] for r in pooling):.3f}%，两boot的方向一致但存在小幅偏移，"
        "所以同时报告分boot结果与等权合并结果。"
        "置信区间描述已观察boot条件下试次中位数的不确定性，不能代表跨重启总体、硬件配置等价性或系统性测量误差。", "",
        "### V 硬件时间（ms）", "",
    ]
    for metric in ("hardware_ms", "victim_MiB_s"):
        if metric == "victim_MiB_s":
            lines += ["### V 吞吐（MiB/s）", ""]
        rows = [r for r in stats if r["metric"] == metric]
        lines += [table(["配置", "条件", "n", "中位数", "IQR", "最小值", "最大值", "中位数95% CI"],
                        [[labels[r["scope"]],r["condition"],r["n_trials"],
                          *[fmt(r[k],5) for k in ("median","iqr","minimum","maximum")],
                          f"[{r['ci95_low']:.5f}, {r['ci95_high']:.5f}]"] for r in rows]), ""]
    lines += [
        "### DS50 合并依据与方法", "",
        "前后两个boot使用相同BIT声明、同一pathv3 payload、相同负载参数与条件轮转，每条件各5次；"
        "合并是对这两个已观察boot的等权描述，不以“不显著”作为等价证明。"
        "下表完整显示偏移；两boot以外的温度、运行顺序等混杂因素未被排除。", "",
        table(["条件", "前boot中位 ms", "后boot中位 ms", "后−前 ms", "相对变化 %"],
              [[r["condition"],fmt(r["first_median_ms"],5),fmt(r["last_median_ms"],5),
                fmt(r["last_minus_first_ms"],5),fmt(r["last_over_first_change_percent"],6)] for r in pooling]), "",
        table(["采集段", "boot_id", "每条件n", "BIT声明SHA256"],
              [[labels[phase],index[phase,"Solo","hardware_ms"]["boot_ids"],
                index[phase,"Solo","hardware_ms"]["n_trials"],manifests[phase]["bit_sha256"]] for phase in PHASES]), "",
        f"IQR=Q3−Q1，采用Type 7线性插值。每个分组/指标做{RESAMPLES}次有放回重采样，"
        f"固定基础种子{SEED}，分组种子为SHA256(基础种子:scope:condition)的前8字节大端整数；"
        "同组各指标共用重采样索引。分boot组各抽原样本数，DS50合并组每次在前/后boot内各抽5次再合并10次，"
        "取bootstrap中位数分布的2.5%和97.5%分位；不对两个boot再做总体重启重采样。", "",
        "相对Solo的指标按完整区组配对后重采样；区间均为逐指标pointwise percentile CI，不是多指标同时区间。"
        "n=5时区间常覆盖完整观察范围，这是小样本离散bootstrap的正常现象；"
        "未声称正态性、跨试次独立性或已消除串行相关，区间条件于组内试次可交换这一近似。"
        "路径、A记账量、合计吞吐、配对比值及HPM计数的中位数/Q1/Q3/IQR/min/max/95% CI"
        "全部在[statistics CSV](baseline100-extra-statistics.csv)，不只保留上表两项。", "",
        "## 口径不确定性与可确认边界", "",
        "### A 的跨窗口事务", "",
        "1. 窗口从V首个任务issue之后开始，包含第4096个任务最终完成边沿，"
        "软件poll的done_to_read不在窗内；`record.result.aggressor.completed_bytes`是A启动至最后排空的整批完成量，"
        "包含V窗口前后的工作，不能除以V时间。本分析只采用相同observer序列的`axi_a.b`，"
        "读RLAST用于交叉核对；80个共跑样本均为ar=aw=rlast=b=8192，"
        "A开/关窗的已接受未响应读写突发各为2，V均为0。", "",
        "2. 固定配置为4KiB COPY、64位AXI、最长256 beat且不deburst，故每COPY分为两个2048字节突发；"
        "`C_A=2048×B_A=16777216 bytes`是按响应完成的名义COPY字节记账。"
        "跨窗前已发起而窗内响应的突发计入C，窗内发起但窗后响应的不计入；"
        "它不是“整个COPY从发起到完成都落在窗口内”的字节数。", "",
        "3. 严格窗口内DMA私有接口的W握手有效字节未记录，不能精确恢复。"
        "对合法AXI，设D(t)为边界前已W握手但尚未取得B记账的字节，"
        "则`S_A=C_A+D_end−D_start`；实际frontend每DMA只允许一个4KiB任务在途，"
        "故`0≤D≤4096`，从而`C_A−4096≤S_A≤C_A+4096`（"
        f"[{16*2**20-4096}, {16*2**20+4096}] bytes）。"
        f"A字节/吞吐边界相对误差至多{4096/(16*2**20)*100:.7f}%，"
        "合计吞吐的相对误差至多一半；W可早于AW，不能仅凭AW在途数再缩小界限。", "",
        "4. `path.aw3.full`只保证出口AW→B这一阶段两端在窗内；"
        "开窗时在内部阶段的事务也可随后成为aw3.full，不能将它当作整个写事务或COPY的同窗完成量。"
        "边界公式衡量私有AXI接口传输，不是物理DDR写入的时间；observer未保存每个W beat/WSTRB或每个BRESP。"
        "因此上述±4096仅是合法AXI及固定长度条件下的窗口边界误差，"
        "不约束未观测的错误响应、物理落地或每次重复COPY的逐次正确性；"
        "已有status=0、排空与末态data_errors=0均保留，不能据此杜撰逐次校验。", "",
        "### 路径、HPM与因果限制", "",
        "V路径阶段均完整覆盖，A跨窗不污染V的sum/full中位数。HPM来自非原子软件窗口，"
        "DS0 A8/A16/A64的V miss中位2仍保留在CSV，不能据此定位其发生于V硬件窗口；"
        "DS50−DS0也不等于纯PTW费用。总吞吐低于Solo是描述性损失，"
        "当前计数不足以把它全部归到IOMMU内部某一阶段或某个具体带宽瓶颈。", "",
        "## 原始证据与实际源码", "",
        "原始输入仅为三段`records.json`、对应`manifest.json`与"
        "[固定执行身份](../board/packages/baseline100-20261017/source.json)；"
        "逐试次CSV提供raw_path、JSON pointer和同block Solo pointer。"
        "源码口径按实际执行commit读取，不以当前研究checkout替代：", "",
        f"- CVA6 `{source['actual_commits']['cva6']}`，下列路径均相对该仓库："
        "`corev_apu/fpga/src/dma_observer.sv:53–74,148–204`为共享活动窗口、开闭沿和AXI累计；"
        "`corev_apu/fpga/src/dma_path_observer.sv:27–44,62–85,132–142`为完整阶段/跨窗计数；"
        "`corev_apu/fpga/src/ariane_peripherals_xilinx.sv:912–946`为私有接口RLAST/B与路径实际接线。",
        "- 同一CVA6 commit的`corev_apu/fpga/src/ariane_xilinx.sv:158–159`定义64位data；"
        "`corev_apu/iDMA/src/idma_pkg.sv:107–110`与`corev_apu/iDMA/src/idma_legalizer.sv:75–85,143–178,235–267,335–359`"
        "推得每burst2048字节；`corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:96–102,332–363`"
        "限定4KiB且COUNT→WAIT直至完成才发下一任务。",
        "- 单任务完成链为同一CVA6 commit的`corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:180–181,283–286`"
        "→`corev_apu/iDMA/src/idma_backend.sv:519–532,643–648`"
        "→`corev_apu/iDMA/src/idma_axi_transport_layer.sv:447–455`，"
        "即最后burst的B触发任务完成、NO_ERROR_HANDLING不检查BRESP；"
        "transport同文件:354–360的WVALID不要求AW已握手。",
        f"- Linux `{source['actual_commits']['linux']}` 的`drivers/misc/idma.c:341–346`写CONF=0；"
        "同文件:556–565,590–615,963–998给出A先启动、STOP/drain后才读取整批completed_bytes。"
        f"实际软件platform输入`{source['software_platform_inputs']}`的`bench/idma_experiment.c:323–331`"
        "也在record.semantics中明示整批与软件窗口。", "",
        "例如可只读核查：", "", "```sh",
        f"git -C iommu-platform/cva6 show {source['actual_commits']['cva6']}:corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv",
        f"git -C iommu-platform/linux show {source['actual_commits']['linux']}:drivers/misc/idma.c",
        "```", "",
        "该CVA6 commit中的`corev_apu/iDMA`是普通Git tree（非独立gitlink），"
        "以上iDMA文件均直接从该固定CVA6 commit读取。", "",
        "## 统计验证与复现", "",
        "统计谬误检查覆盖11/11：分boot报告检查聚合掩盖（Simpson）；单位固定为试次避免生态推断；"
        "100条全保留、无筛选/丢失避免选择与幸存偏差；未按结果分层调整以避免collider；"
        "不涉及诊断基率或极值选组再测；不做多重显著性筛选；固定全部条件、指标、分位算法和种子，"
        "明确这是事后描述分析；不把关联解释为已定位因果，也不反推因果方向。"
        "小样本、试次可交换性及跨boot推广仍为CAUTION，不将CI当等价检验。", "",
        "```sh", "python3 analysis/scripts/baseline100_extra.py",
        "python3 analysis/scripts/baseline100_extra.py --check", "```", "",
        "第一条只生成本分析目录的文件；第二条只读重算并逐字节比较全部输出。"
        "每次都比较读取前/后的输入SHA256，任何输入或输出不一致即失败；未运行采集器、构建、仿真或板测。", "",
        "| 输出 | 用途 |", "| --- | --- |",
        "| [samples CSV](baseline100-extra-samples.csv) | 100试次的原始定位、配对、边界计数和派生指标 |",
        "| [statistics CSV](baseline100-extra-statistics.csv) | 20组×全部指标；未知/不适用显式标记 |",
        "| [throughput CSV](baseline100-extra-throughput.csv) | 四种分组口径的吞吐与平分比较中位数 |",
        "| [path CSV](baseline100-extra-path.csv) | 四种分组口径的读写四阶段中位数 |",
        "| [boots CSV](baseline100-extra-boots.csv) | DS50前后boot偏移 |",
        "| [inputs JSON](baseline100-extra-inputs.json) | 输入与本分析输出SHA256、固定seed及重采样次数 |", "",
    ]
    return "\n".join(lines)


if __name__ == "__main__":
    main()
