#!/usr/bin/env python3
"""Render the offline model report from the saved numerical outputs; no refitting."""
from pathlib import Path
from statistics import median
import csv
import json


ROOT = Path(__file__).resolve().parents[1]


def interval(value, bounds, digits=3):
    return f"{value:.{digits}f} [{bounds[0]:.{digits}f}, {bounds[1]:.{digits}f}]"


def main():
    r = json.loads((ROOT / "interference-model-results.json").read_text())
    o = json.loads((ROOT / "interference-model-observers.json").read_text())
    samples = list(csv.DictReader((ROOT / "interference-model-samples.csv").open()))
    primary = r["primary"]
    corrected = r["positive_ds_step_sensitivity"]["models"]
    pooled = primary["pooled"]
    eta = pooled["apparent_eta"]
    parts = [f"""# DS 曲线的干扰定量模型：源码约束、拟合与条件验证

日期：2026-10-18。ARS experiment-agent / validate；离线分析。原始 CSV、记录、RTL、构建与加载报告只读；没有上板、构建或仿真。

合并模型得到 **t0={pooled['t0_cycles_per_walk']:.3f} cycles/walk，k={pooled['k_cycles_per_walk_per_ds']:.5f}，表观 η={eta['value']:.5f}，95% bootstrap CI [{eta['ci95'][0]:.5f}, {eta['ci95'][1]:.5f}]**。三层 PTE 读支持边际参照值 2L=6，但 HPM 与硬件窗不一致，因此 η 不是已辨识的真实阻塞比例。模型刻画了主要增长趋势，不能声称精确解释全部增量。

## 1. 输入、单位与配对

输入为 [board_results.csv](../data/board_results.csv) 的 `board_baseline100` / `board_ds_sweep`、`statistic=sample`，共 {r['trials']} 次成功试次；不是用汇总中位数回归。DS0/25/50/100 × Solo/A1/A8/A16/A64 各 10 次；同 DS、boot、block、BIT、kernel、run 的 Ak 与 A1 配对。40 个完整块产生 {r['matched_pairs']} 个差值，每个足迹 40 对；Solo 用于保留原始设计身份，不进入此差分模型。所有试次保留。

定义 `y = T_V(Ak,DS) - T_V(A1,DS)`，T 是 V 硬件窗周期；`Nsw = aggressor_s1_ptw` 是 A 的**非原子软件快照差值**，不是硬件窗 walk 计数。A1 的 A S1 计数全部为零。受害者软件 S1 计数分布为 {r['nwalk']['victim_walk_count_distribution']}；其中 DS0 的全部 30 个 Ak 试次均为 2，另有两次正 DS 试次为 2。不能把全部 Ak 称为严格 V 零 miss。

CSV SHA256：`{r['input_csv_sha256']}`。逐试次 row_id、原始 JSON pointer、A1 控制行及三种模型的预测/残差保存在 [samples.csv](interference-model-samples.csv)；全部试次身份见 [trials.csv](interference-model-trials.csv)。采集命令参数、固定源码 commit、四档文件哈希和原文摘录见 [RTL 证据](interference-model-rtl.json)。DS 档位允许使用对应的四个 BIT，禁止同一 DS 混用 BIT/kernel；本模型相关关键 RTL 的差别限于已核对的 DS 参数。实际加载身份与最终时序资格沿用 [时序审计](../related/timing-audit.md)。

Bootstrap 在 DS × boot 内抽取**整块**，保留三个 Ak 共用 A1 的相关性；10,000 次，PCG64，种子 20261018，线性百分位 95% CI。DS50 两个 boot 各 5 块，分别重抽，再合并描述同一配置；不是把两 boot 当成单 boot，也不是独立启动总体的 CI。DS0/25/100 各只有一个 boot；DS 与启动/采集顺序存在混杂。

| DS | boot_id | 块数 |
| --- | --- | ---: |
"""]
    for s in r["bootstrap"]["strata"]:
        parts.append(f"| {s['ds']} | `{s['boot_id']}` | {s['n_blocks']} |\n")
    parts.append("""
## 2. 从真实构建源码确定 L 与 DS 的作用

固定 CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`、Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`，入口是 [source-manifest.json](../board/manifests/source-manifest.json)。核对的是实际编译的冻结副本，而非当前开发分支。下述 `id` 对应 RTL 证据 JSON 的 `evidence[].id`，内含完整路径、精确行号、摘录和 SHA256。

| 问题 | 源码结论 | 可导航证据 id / 核心位置 |
| --- | --- | --- |
| 本负载的叶子大小 | 驱动逐 4KiB 映射，Sv39、S2 Bare；不是 2MiB 叶 | `MAP-4K-loop`/`MAP-4K-call`，idma.c:202/313；`MAP-mode`/`MAP-S2-default` |
| 每 walk 的读数 | 根→中间→末级，成功遍历 **L=3**；每层 ARLEN=0、ARSIZE=3，即单拍 8B PTE 读；串行等待上层响应 | `L-root`/`L-read`/`L-serial`/`L-next-middle`/`L-next-leaf`/`L-leaf`，rv_iommu_ptw.sv:258/313/382/415/506/541 |
| PTE 是否写回 | Linux 预置 A/D；此 PTW 的 AW/W valid 恒零；A/D 缺失报错，不能多算 A/D 写访存 | `MAP-ad`，io_pgtable.c:150；`AD-fault`，rv_iommu_ptw.sv:483 |
| 目录是否命中 | DDTC 8 项，只有 DID10/11；256 次预热且测量前排空，无测量内映射/失效。稳态 DDTC 命中是源码支持的条件预期，未逐试次测得 | `CACHE-size`/`CACHE-DDTC`/`CACHE-inval`/`CACHE-warmup` |
| PDTC | DMA conf/PV 为零，normal DC 不置 PDTV；本负载**绕过 PDTC**，不能称其 warm hit | `CACHE-conf-zero`/`CACHE-pv`/`CACHE-pv-wire`/`CACHE-PDTC-bypass`/`MAP-PDTV` |
| DS 放在哪里 | DS 总口 AR/AW/W 走输入延迟、R/B 走输出延迟；每个 PTE 的 AR 和 R 各经过一次 | `DS-place`，ariane_peripherals_xilinx.sv:1093；`DS-AR`/`DS-R`，axi_delayer.sv:60/112 |
| 一次 HPM 事件 | S1_PTW 为 event7，PT miss 信号经边沿门控只计一次，非每 PTE 或每 stall 周期一次 | `W-etype`/`W-pulse-source`/`W-single-edge`/`W-increment` |

没有中间 PTE 缓存将上述三次访问省略。冷 DDTC 的 DC 读为另一个 8-beat burst；其他 DS 客户端也共享延迟口，不能把所有 DS 流量都等同于 `3×Nsw`。本推导要求成功普通请求、S1-only、4KiB 末级叶、目录稳态且没有外部失效；不适用于嵌套翻译、超级页、异常、冷目录或设备上下文抖动。

还有一个必须披露的实现项：DS0 直接旁路；实际编译的是 CVA6 根 `vendor/pulp-platform/common_cells/src/stream_delay.sv`，不是 IOMMU vendor 下同名副本。其 Idle 装载 d，Valid 倒数到零后进入 Ready；d>1 时从首次输入 VALID 到释放 VALID 为 **d+2** 拍，d=1 另有特判。因此测得的正档 25/50/100，在理想 ready、每次从 Idle 开始时，每 walk 延迟器增加 `6d+12`；正档间边际仍为 `6Δd`。这不是对全部排队情形的精确全窗时长断言。证据：`COMPILE-resolve`/`DS-0`/`DS-FSM`/`DS-load`/`DS-count`。

## 3. 最小二乘、区间与残差

主模型 `y_i = Nsw_i × (t0 + k×DS_i) + e_i`，设计矩阵 `[Nsw, Nsw×DS]`；最小化**周期域**残差平方和，不额外添加常数截距。t0 是相对 A1 的有效每计数 walk 代价，不是单独测量的 PTW 服务延迟。η_app=k/6 仅是条件斜率比。表中参数为估计值 [95% CI]，R² 使用中心化总平方和；RMSE 和相对误差均针对 ΔT。

| 拟合 | t0 (cycles/walk) | k (cycles/walk/DS) | η_app | R² | RMSE (cycles) | 最大逐试次 ΔT 误差 |
| --- | --- | --- | --- | ---: | ---: | ---: |
""")
    for name, m in primary.items():
        parts.append(f"| {name} | {interval(m['t0_cycles_per_walk'], m['coefficient_ci95']['t0'])} | {interval(m['k_cycles_per_walk_per_ds'], m['coefficient_ci95']['k'])} | {interval(m['apparent_eta']['value'],m['apparent_eta']['ci95'])} | {m['delta_time_errors']['centered_r2']:.6f} | {m['delta_time_errors']['rmse']:.0f} | {100*m['delta_time_errors']['max_relative_error']:.2f}% |\n")
    parts.append(f"""
合并模型的完整 V 时间最大误差只有 {100*pooled['full_victim_time_errors']['max_relative_error']:.2f}%，但对应 ΔT 最大误差为 {100*pooled['delta_time_errors']['max_relative_error']:.2f}%；较大的 A1 基线掩盖了部分增量误差。不能用接近 1 的 R² 或完整时间误差替代增量验证。DS0 的 per-walk 代价有足迹相关性与离散，不支持逐点严格相同的 t0；全部有符号逐试次残差均已导出，没有删除偏离点。

归一化 OLS `y/Nsw ~ 1+DS` 是另一种权重（对应周期域 1/Nsw²），合并结果 t0={r['per_walk_normalized_sensitivity']['pooled']['t0_cycles_per_walk']:.3f}、k={r['per_walk_normalized_sensitivity']['pooled']['k_cycles_per_walk_per_ds']:.5f}；完整区间及逐足迹结果见 results.json 的 `per_walk_normalized_sensitivity`。它是敏感性分析，不取代预先说明的周期域目标。

机制修正敏感性模型为 `y=Nsw×[t0+k×(DS+2×I(DS>0))]`，仍只有两个参数；该固定阶跃由 RTL 提出，不是另外拟合的自由阶跃：

| 拟合 | t0 [95% CI] | k [95% CI] | η_app [95% CI] | ΔT RMSE (cycles) | 最大逐试次 ΔT 误差 |
| --- | --- | --- | --- | ---: | ---: |
""")
    for name, m in corrected.items():
        parts.append(f"| {name} | {interval(m['t0_cycles_per_walk'],m['coefficient_ci95']['t0'])} | {interval(m['k_cycles_per_walk_per_ds'],m['coefficient_ci95']['k'])} | {interval(m['apparent_eta']['value'],m['apparent_eta']['ci95'])} | {m['delta_time_errors']['rmse']:.0f} | {100*m['delta_time_errors']['max_relative_error']:.2f}% |\n")
    parts.append("""
修正改善整体残差，但仍有足迹依赖、DS0 偏差和非同窗计数误差，不能称为已经完整辨识硬件阻塞机制。三种估计及联合参数 bootstrap 协方差保存在 [results.json](interference-model-results.json)。

## 4. 留一验证：整格留出，不在重复样本间泄漏

“点”定义为一个 DS×足迹格，留出其全部 10 次重复。逐足迹模型以其他三个 DS 格拟合；合并模型以其他 11 格拟合。另做更严格的整 DS 留出，一次排除该 DS 的三个足迹共 30 次。表中均值与逐试次最大误差由留出预测计算；误差分母是各自实测 ΔT，不是完整 V 时间。

| 留出足迹 | DS | 实测 ΔT 均值 (cycles) | 逐足迹预测均值 (cycles) | 逐足迹最大误差 | 合并整格最大误差 |
| --- | ---: | ---: | ---: | ---: | ---: |
""")
    folds = r["leave_out"]["folds"]
    for f in folds:
        if f["estimator"] != "cycles_ols" or f["family"] != "per_footprint_cell":
            continue
        g = next(x for x in folds if x["estimator"] == "cycles_ols" and x["family"] == "pooled_cell" and x["held_out_footprint"] == f["held_out_footprint"] and x["held_out_ds"] == f["held_out_ds"])
        parts.append(f"| {f['held_out_footprint']} | {f['held_out_ds']} | {f['actual_delta_mean']:.1f} | {f['prediction_delta_mean']:.1f} | {100*f['delta_time_errors']['max_relative_error']:.2f}% | {100*g['delta_time_errors']['max_relative_error']:.2f}% |\n")
    parts.append("""
| 合并模型整 DS 留出 | 实测 ΔT 均值 (cycles) | 预测均值 (cycles) | ΔT 相对 RMSE | 最大 ΔT 误差 | 修正模型最大 ΔT 误差 |
| --- | ---: | ---: | ---: | ---: | ---: |
""")
    for f in folds:
        if f["estimator"] != "cycles_ols" or f["family"] != "pooled_ds":
            continue
        g = next(x for x in folds if x["estimator"] == "positive_ds_step_corrected" and x["family"] == "pooled_ds" and x["held_out_ds"] == f["held_out_ds"])
        parts.append(f"| DS{f['held_out_ds']} | {f['actual_delta_mean']:.1f} | {f['prediction_delta_mean']:.1f} | {100*f['delta_time_errors']['relative_rmse']:.2f}% | {100*f['delta_time_errors']['max_relative_error']:.2f}% | {100*g['delta_time_errors']['max_relative_error']:.2f}% |\n")
    parts.append("""
| 全部留出预测汇总 | 主模型 ΔT 相对 RMSE / 最大误差 | 主模型完整 TV 最大误差 | 修正模型 ΔT 相对 RMSE / 最大误差 |
| --- | --- | ---: | --- |
""")
    for key, label in [("per_footprint_cell", "逐足迹整格"), ("pooled_cell", "合并整格"), ("pooled_ds", "合并整 DS")]:
        a = r["leave_out"]["summary"]["cycles_ols"][key]
        b = r["leave_out"]["summary"]["positive_ds_step_corrected"][key]
        parts.append(f"| {label} | {100*a['delta_time_errors']['relative_rmse']:.2f}% / {100*a['delta_time_errors']['max_relative_error']:.2f}% | {100*a['full_victim_time_errors']['max_relative_error']:.2f}% | {100*b['delta_time_errors']['relative_rmse']:.2f}% / {100*b['delta_time_errors']['max_relative_error']:.2f}% |\n")
    parts.append("""
留出预测仍使用被留出试次**实测的 Nsw**，重建完整 TV 时也使用其同块实测 A1。因此这是**条件验证**，不是实验前盲预测；walk 数会随 DS 和 V 完成时间变化，是内生协变量。合并整格还保留该 DS 的其他足迹和共享 A1，仅整 DS 留出才检查未见 DS。不能把完整时间误差较小解释为已经能够预测新负载。逐点预测、训练/测试身份及三模型结果见 [loo.csv](interference-model-loo.csv)、[loo-samples.csv](interference-model-loo-samples.csv) 和 results.json 的 `leave_out`。

## 5. HPM 非原子窗口：可证明的范围与未识别误差

冻结驱动 `iommu-hpm.c:185` 依次读取 cycle、V miss/S1/S2、A miss/S1/S2；每个 64-bit counter 的 hi/lo/hi 读取防撕裂，但没有全计数器共同锁存。`idma.c:945` 在 A/V 启动前读 before；`idma_batch_window:576` 先启动 A 再启动 V；V 完成后 CPU 轮询发现、停止 A，再读 after，最后才排空 A。因此计数窗口覆盖 V 硬件窗前后的时段，Nsw 可能多计这些时段的事件；不能仅因存在额外时长就断言必有额外 walk。

对**实际登记的 HPM 增量**，在单调且未溢出的正常计数和有序 MMIO 前提下，可给出的保守包含界是 `0 ≤ N_HPM,in-V ≤ Nsw`，窗外多计在 `[0,Nsw]`。它不是有用的窄误差条；把计数再对应到真实 PTW 启动事件还需考虑事件登记延迟/校准。缺少逐事件硬件时间戳，不能声称“最多 ±1/±2 walk”，也不能把 η 的窄 bootstrap CI 当作这项系统误差的范围。若窗外比例随 DS 改变，η_app 连单纯比例校正都不成立。

`Tsw-20×Thw` 是软件区间相对硬件窗的时间余量（ns），HPM 读点覆盖额外 before/after 的一部分；由保存的 aggregate 时长得到保守时间区间 `[Tsw-20Thw, Tsw-20Thw+snapshot_before+snapshot_after]`。这只约束时长，不约束期间 walk 的速率。示例 A8 的中位数如下（毫秒）；全部范围保存在 [observers.json](interference-model-observers.json)。

| DS | Tsw−Thw 中位数 (ms) | before 快照时长中位数 (ms) | after 时长中位数 (ms) |
| --- | ---: | ---: | ---: |
""")
    for g in o["groups"]:
        if g["condition"] == "A8":
            s = g["statistics"]
            parts.append(f"| {g['ds']} | {s['software_minus_hardware_ns_at_configured_clock']['median']/1e6:.3f} | {s['snapshot_before_ns']['median']/1e6:.3f} | {s['snapshot_after_ns']['median']/1e6:.3f} |\n")
    parts.append("""
不能用 A 全程 completed copies 减去 V 窗内字节，直接求出额外 walk：边界存在在途请求，每 copy 的 miss 数也不是常数，V 停止后缓存状态还会改变。helper 中 `Nsw±1/±2` 仅是标注为假设的敏感性字段，不是测得误差界。本分析不以该字段修正样本。

## 6. 第二条证据：V 内部读路径

path_vr1 以硬件记录的进入握手到内部输出首次 VALID 度量，包括翻译、排队与缓冲的驻留，不是纯 PTW 服务时间。每试次 read/write 各 8192 个 burst，全部 path 阶段 full=entered=8192、start/carry/end=0、area=sum。下表先逐块 Ak−A1，再取中位数，单位 cycles/burst：

| 足迹 | DS0 Δvr1 | DS25 Δvr1 | DS50 Δvr1 | DS100 Δvr1 |
| --- | ---: | ---: | ---: | ---: |
""")
    for footprint in ["A8", "A16", "A64"]:
        vals = [next(g for g in o["groups"] if g["ds"] == ds and g["condition"] == footprint)["statistics"]["delta_path_vr1_mean_cycles_vs_a1"]["median"] for ds in [0, 25, 50, 100]]
        parts.append(f"| {footprint} | " + " | ".join(f"{v:.2f}" for v in vals) + " |\n")
    parts.append("""
所有足迹的内部读驻留随 DS 增长，与 ΔT 的增长方向一致。正 DS 下 `8192×Δvr1/ΔTV` 的逐试次比值中位数约从 1.95 增到 1.99，说明该路径也随整窗代价增长；它同时表明不能把每 burst 驻留总和当作 elapsed time：多个在途 burst 的等待发生重叠。路径与 HPM 是不同观测机制，但共享同次运行/boot；这提供趋势佐证，不能独立校准 η 或精确归因所有周期。

## 7. 明确标为外推：每次 PTE 访存增加 50/100/200 ns

主解释将该 ns 数定义为**一次 PTE 读的总往返额外时间**。平台 50MHz，每拍 20ns；由于诊断 DS 在请求、响应各加一次，等价名义 DS 为 `τ/(2×20)=1.25/2.5/5`，不是 2.5/5/10。若 τ 指每个方向单独增加，后者才成立，CSV 已另列。

物理内存情景保持 DS0 的逐块 Nsw 和 A1 不变，边际额外干扰为 `δT = Nsw_DS0 × k × τ/40`。以下使用合并主模型，表示**相对实测 DS0 的新增量**；不是已有总干扰或完整 TV。未把诊断延迟器的 +2 拍启动阶跃加入真实内存延迟。主模型参数 CI 不覆盖固定 Nsw 或物理系统外推的系统误差。

| 每 PTE 总往返增加 | 等价 DS/方向 | 每计数 walk 新增 (cycles) | A8 新增 V 时间 (ms) | A16 新增 (ms) | A64 新增 (ms) |
| --- | ---: | ---: | ---: | ---: | ---: |
""")
    for ns in [50, 100, 200]:
        ds = ns / 40
        vals = []
        for footprint in ["A8", "A16", "A64"]:
            n = [int(s["nwalk"]) for s in samples if s["footprint"] == footprint and int(s["ds"]) == 0]
            vals.append(median(n) * pooled["k_cycles_per_walk_per_ds"] * ds / 50000)
        parts.append(f"| {ns} ns | {ds:g} | {pooled['k_cycles_per_walk_per_ds']*ds:.3f} | " + " | ".join(f"{v:.3f}" for v in vals) + " |\n")
    parts.append("""
逐足迹/合并、主/归一化/机制修正的参数条件区间、以模型 DS0 或实测 DS0 锚定的总 ΔT 与完整 TV，均在 [projections.csv](interference-model-projections.csv)。`physical_memory_increment` 与 `diagnostic_ds_configuration` 明确分列；后者包括诊断器实现项，不可冒充真实内存延迟。

这是**物理场景外推**：等价延迟数值位于已测 DS 范围内，但小数 DS 不是实际 RTL 档位，且未测真实内存技术。前提包括 walk 层数/缓存状态、walk 数、A 供给、V 基线、时钟、并发程度与仲裁规律均不变，额外延迟能映射为相同串行 PTE 路径暴露；不包含带宽/服务时间分布、并行度或协议的改变。若这些前提变化，不给出无条件应用级慢化预测。

## 8. 模型成立的条件 / 不成立的条件

**模型成立的条件**：限定本次相同基座、负载与启动样本，用实测 Nsw 作为条件输入时，三层串行 PTE 与双向 DS 延迟提供机制依据；约 5.5 的边际斜率和内部路径增长支持一阶经验近似。它可用于组织敏感性结果、给出条件估算和检查大偏差。

**模型不成立的条件**：严格的共同 t0/k 精确公式没有覆盖全部点；DS0 留出有结构偏差。DS0 直接旁路使名义 DS 原模型漏掉正档 FSM 阶跃。Nsw 与 V 硬件窗未对齐、V 偶发软件计数不为零、Nsw 内生、DS 与 boot 混杂，均妨碍把 η_app 解释为真实阻塞份额或因果常数。超级页/嵌套翻译/冷目录/不同供给与并发/不同内存体系需要重新校准，不能沿用 L=3 与当前误差保证。

按 ARS validate 作 **11/11** 统计/方法谬误检查：Simpson（逐足迹与合并同向，但参数非完全相同）；生态谬误（不从整窗拟合推单请求精确延迟）；Berkson（仅已接受的固定成功矩阵，外推限于此）；collider（Nsw 内生，条件拟合不作因果控制）；base-rate（非分类诊断，不适用）；回归均值（未按极端值选点）；幸存者（200/200 输入全保留，不推失败运行）；多重搜寻（无显著性检验，不选报 p 值）；分析分叉（探索性模型，三种规格全部保留）；相关即因果（源码机制与经验吻合分开，不称因果识别）；反向因果（V 持续更久也使 A 能执行更多 walk，明确披露）。结论等级为 CAUTION：有机制支持的条件模型，不是无条件硬件定律。

## 9. 图表与复现

![实测与条件模型预测](figures/interference-model-measured-predicted.png)

图 1：120 个配对试次，分别显示逐足迹与合并主模型；浅色区是参数条件 CI，不是新试次预测区间。横向微小抖动仅为避免点重叠，真实 DS 仍为四档。

![每计数 walk 的受害者代价](figures/interference-model-per-walk.png)

图 2：ΔT/Nsw 与 DS；线仍来自周期域主拟合，避免暗中改成归一化目标。两图均 3.5 英寸宽、8pt，颜色加线型/符号区别，PDF 与 PNG 同步保存。PDF：[实测对照](figures/interference-model-measured-predicted.pdf)、[每 walk 代价](figures/interference-model-per-walk.pdf)。

在仓库根运行（Python 3，numpy、matplotlib；无下载/硬件操作）：

```bash
python3 paper/ispass2027/analysis/scripts/interference_model.py --levels 3 --levels-status confirmed --self-test
python3 paper/ispass2027/analysis/scripts/interference_model_observers.py
python3 paper/ispass2027/analysis/scripts/interference_model_rtl_verify.py
python3 paper/ispass2027/analysis/scripts/interference_model_report.py
```

稀疏 worktree 中 observer/RTL 核查可加 `--source-root /home/xvhaocheng/work` 读取原始保存副本；模型脚本默认读取自身所属 worktree 的 CSV。RTL 原冻结目录不随小型分析包复制，缺失时 verifier 会报缺失，不据此宣称原文已重新核验。上述依赖的实际版本、输入 SHA、固定随机数参数和输出 SHA 在 results.json 中记录；数值输出可复现，绝对路径/运行时版本字段可能随环境变化。

[validation.json](interference-model-validation.json) 记录本轮接受检查：真实数值原始 pointer 回查、独立高精度闭式 OLS、bootstrap 数值对照、整格/整 DS 训练泄漏与身份负例、输入未改、实际 PDF 字体/尺寸核查。它是本轮检查记录，不是额外板测。源码证据的全量复核由 rtl_verify 脚本执行；完整 HPM/path 边界算术由 observer 脚本重建。所有论文可用数字及分析结果单独保存，没有改历史数据或报告。
""")
    report = ROOT / "interference-model.md"
    # Later source audits are appended without replacing the original analysis.
    # Preserve them when rebuilding the generated numerical sections above.
    marker = "\n<!-- INTERFERENCE_MODEL_APPENDICES -->\n"
    existing = report.read_text() if report.exists() else ""
    appendix = marker + existing.split(marker, 1)[1] if marker in existing else ""
    report.write_text("".join(parts) + appendix)
    print(report)


if __name__ == "__main__":
    main()
