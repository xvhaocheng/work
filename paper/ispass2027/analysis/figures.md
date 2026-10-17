# ISPASS 2027 十图构建与证据合同

这是实际离线分析与绘图入口；不运行板卡、RTL或历史复核程序。使用 ARS academic-paper visualization 提示，按用户的 IEEE 单栏规范覆盖默认 APA 尺寸：每个 PDF 固定宽 **3.5英寸**，全部文字 **至少8pt**，PNG 300dpi，色盲安全颜色同时配合不同形状、线型或纹理，黑白打印可辨。

在仓库根运行 `paper/ispass2027/analysis/build.sh`。环境只需本目录 `requirements.txt`；本机隔离环境位于忽略的 `.venv/`。需要先安装时运行 `python3 -m venv paper/ispass2027/analysis/.venv`，再用该环境安装 requirements。已有虚拟环境无需重复安装。

脚本依次验证统计/证据边界、重建 B1 补充分析、阶段性合并、重画全部十图、写此说明并核验实际 PDF/PNG。输出位于 `figures/`；`figure-trace.json` 逐图列稳定来源行、版本基座、转换脚本 hash 和局限；`figure-statistics.json` 保存实际图中每组统计和 row_id；`build-validation.json` 是实际产物检查回执。不能把回执升级成科学结论证明。

## 统计和比较规则

- 长表中的17404行是标量，不是17404次独立实验。统一基线从每试次 `statistic=sample,n=1` 取数：100个真实试次、10个DS×条件组，每组n=10。DS0来自一个boot；DS50两段各n=5、来自两个boot。bootstrap不把同boot试次升级成独立重启。
- 中位数、Type7线性插值Q1/Q3/IQR和95% percentile bootstrap CI由 `scripts/statistics_common.py` 产生：**10000次，固定seed=20261017**。bootstrap在每个已观测boot内保持该层原样本n重采样；这是对已观测boot有条件的区间，不能估计跨boot总体。历史P4M的现成Q1/Q3直接画IQR，不能对摘要行再bootstrap。
- relative slowdown默认 `median(shared_elapsed)/median(reference_elapsed)-1`；显式配对模式才计算逐对比值的中位数。二者不能互换。宏中的Extra是同DS相对A1的硬件elapsed增量百分比；图1底部将相对Solo的时间增量分为A1共同压力项与相对A1额外项，不称为纯PTW或整个系统效率损失。B1的完整双侧吞吐/聚合损失采用其已核实的共同窗口字节口径。
- `base_id`约束源码/软件及历史比较范围，BIT仍保留独立身份列；同一固定源码的DS是有名BIT变体。历史E1、batch、path、P4M各采集段、composition两侧各自保守分组。任何一次相对计算、同一比较系列都必须单一base_id；混base和重复row_id会报错。不同历史版本只进明确分开的panel，不与统一基线拼接。
- HPM来自非原子软件快照，路径阶段来自硬件V窗口，二者不作数值相减。路径均值是逐试次sum/full再跨试次统计，不是请求p99，阶段有重叠不能相加归因。CDF只接受 `request_sample` 个体请求；mean/max缺失的分布不补造。

## 实际图状态

| 图 | 问题 | 状态 | 版本/合法比较 | 来源入口 |
| --- | --- | --- | --- | --- |
| 1 [fig01-throughput](figures/fig01-throughput.pdf) | How much throughput is lost without and with translation pressure? | measured | baseline100-fixed-885ed0a7-45bf76b1-pathv3 | `merged-board-results.csv`；100个来源标量/配置引用 |
| 2 [fig02-path-miss](figures/fig02-path-miss.pdf) | How do phase latency and observed misses change? | measured | baseline100-fixed-885ed0a7-45bf76b1-pathv3 | `merged-board-results.csv`；330个来源标量/配置引用 |
| 3 [fig03-ds-sweep](figures/fig03-ds-sweep.pdf) | How sensitive is interference to added DS delay? | partial | baseline100-fixed-885ed0a7-45bf76b1-pathv3 | `merged-board-results.csv`；100个来源标量/配置引用 |
| 4 [fig04-device-count](figures/fig04-device-count.pdf) | How does device count affect interference? | missing | 无已接受数据 | `merged-board-results.csv`；0个来源标量/配置引用 |
| 5 [fig05-workload-size](figures/fig05-workload-size.pdf) | How do workload and transfer size affect interference? | missing | 无已接受数据 | `merged-board-results.csv`；0个来源标量/配置引用 |
| 6 [fig06-page-size](figures/fig06-page-size.pdf) | How does page size change interference? | missing | 无已接受数据 | `merged-board-results.csv`；0个来源标量/配置引用 |
| 7 [fig07-latency-cdf](figures/fig07-latency-cdf.pdf) | What is the request-latency distribution? | missing | 无已接受数据 | `merged-board-results.csv`；0个来源标量/配置引用 |
| 8 [fig08-mitigation-history](figures/fig08-mitigation-history.pdf) | What V/A tradeoff is observed in historical mitigation versions? | historical_only | history-composition-clean_history, history-composition-isolated_history, history-p4m-acq29_newboot | `merged-board-results.csv`；142个来源标量/配置引用 |
| 9 [fig09-capacity](figures/fig09-capacity.pdf) | How does capacity change interference? | simulation | e7-f5-ddr16-mmio-v1 | `/home/xvhaocheng/work-root-b/paper/ispass2027/exp/E7-capacity-sim/results.csv`；12个来源标量/配置引用 |
| 10 [fig10-invalidation](figures/fig10-invalidation.pdf) | What completion tradeoffs follow invalidation scope? | simulation | invalidation-frozen-ba8cdd1-85b157e | `../data/invalidation_sim_results.csv`；36个来源标量/配置引用 |

## 版本和缺数据说明

1. **吞吐和增量：** 统一修复基线DS0，V工作固定16MiB，Solo/A1/A8/A16/A64各n=10。A1两侧HPM miss中位0；A8/A16/A64的V HPM miss中位2，不能标“V零miss”。V吞吐下降同时含共享服务分摊，不能仅据此写成总系统带宽损失。
2. **路径/失效观测：** 同一DS0基线。actual CVA6 commit `8aac3fcd345af7686a55af1867783fd6b91a0069` 的 `dma_path_observer.sv` 132–145行定义离散阶段：entry为DMA首VALID到IOMMU ingress handshake；internal为ingress handshake到output首VALID；address为output首VALID到AXI地址握手；return为地址握手到最后响应。图中将其作为离散点，而非连续时间趋势。
3. **E1b DS：** DS0和DS50为原100次锚点。DS25/100只有收到同基座、完整/通过且hash绑定的新增CSV后纳入；不把运行中目录或部分点当成四点曲线，不跨空点插值。DS改变多类共享请求，不能当纯PTW延迟。
4. **E2设备数：** 当前接口仅V/A两流、一个aggressor_id；尚无三/四设备完整矩阵。需要扩接口的部分明确留缺，不以多个进程冒充独立硬件流。
5. **E3负载×大小：** 当前pathv3 batch固定4096B且V/A共用chunk。Root A已有历史DS0离线分析是原50次重算，不是新小尺寸子集，不重复导入。64/1024/4096×负载矩阵未实际测量时保持骨架。
6. **E4页大小：** 没有接受并核实真实2MiB页表叶级的同基座配对时保持骨架；地址步长或分配大小不等于页大小。
7. **E5 CDF：** 现有v3只有sum/count/max，没有请求样本/直方图；骨架内明确“Mean/max cannot reconstruct a CDF”。不得用这些聚合值伪造CDF、p50或p99。
8. **历史缓解：** P4M只画`acq29_newboot`（29个历史试次，按gap独立统计）；不混前两采集段。clean cpn3与isolated cpni1两个版本/boot各自panel，A8下S/N/P/PN每模式n=3。A共同窗服务率逐试次按`a_completed_window*4096*50MHz/W/2**20`计算；4096B与固定50MHz窗口语义已从24个实际原始record逐一核对并存于`history-composition-inputs.json`。这些试次的共同窗A完成数均为4096，因此该服务率与W共用分母；它不提供独立A完整任务时间，也不包括V后尾部。历史loaded-byte身份未知、dirty内核身份照录；不能称为统一基线缓解复测。
9. **E7容量：** Root新RTL仿真只接受16/32/64×Solo/A1/A8/A64完整12格、所有checks_passed且hash绑定的receipt。固定FIFO行为DDR，队首启动等待16周期，之后每周期一拍8B；16不是突发拍数。非FPGA DDR、非Linux执行，A256/V64+2预热及供给模型也与板测不同。每格一个确定性运行，不画统计CI，校准只说明趋势，不宣称与板测数值吻合；历史E2 W7/W8保留正文独立证据，不能与此图混为一个实验。
10. **失效范围：** 独立`data/invalidation_sim_results.csv`的main24中选B=8页、full-api、same/different domain。B完成、A最后COPYDONE和A含最终flush各自除匹配M0；不把A最后COPYDONE当最终flush结束。单位是仿真步骤/比值，CPU/Linux未执行，不能换成FPGA纳秒。B=4和unmap-only仍保留在来源表。

## 图注与逐图追源
### Figure 1: fig01-throughput

DS0 V throughput (95% bootstrap median CI) and elapsed increment relative to Solo. The stacked split uses A1 as a measured co-run reference; it is not a pure PTW decomposition.

局限：Single observed DS0 boot; CI conditional on observed trials.；A1 has zero median HPM misses in software brackets; this does not measure every causal cost.；Additional elapsed is not uniquely attributable to translation.

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig01-throughput.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

### Figure 2: fig02-path-miss

DS0 per-trial sum/full phase means and HPM miss medians. Entry: DMA input assertion to IOMMU ingress handshake; internal: ingress handshake to output address assertion; address: output assertion to address acceptance; return: address acceptance to final response. HPM counts use separate software brackets.

局限：Stage means are not latency quantiles or fractions of total runtime.；Phase latencies overlap and must not be added as a causal decomposition.；DS0 A8/A16/A64 V HPM median is 2, not zero.

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig02-path-miss.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

### Figure 3: fig03-ds-sweep

Frozen-baseline DS sensitivity; accepted trial groups are shown without interpolation across missing groups. DS changes multiple shared request classes and is not a pure PTW-latency sweep.

局限：DS0/50 each n=10 per condition; DS50 spans two observed boots.；Missing accepted DS x condition groups: [(25, 'Solo'), (100, 'Solo'), (25, 'A1'), (100, 'A1'), (25, 'A8'), (100, 'A8'), (25, 'A16'), (100, 'A16'), (25, 'A64'), (100, 'A64')].

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig03-ds-sweep.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

### Figure 4: fig04-device-count

E2 device-count matrix pending
Current interface has one aggressor

局限：Explicit empty figure scaffold; contains no measured values.

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig04-device-count.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

### Figure 5: fig05-workload-size

E3 workload x size matrix pending
Current batch is fixed at 4096B

局限：Explicit empty figure scaffold; contains no measured values.

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig05-workload-size.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

### Figure 6: fig06-page-size

E4 page-size pair pending
No verified 2 MiB leaf sample

局限：Explicit empty figure scaffold; contains no measured values.

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig06-page-size.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

### Figure 7: fig07-latency-cdf

E5 request-latency distribution absent
Mean/max cannot reconstruct a CDF

局限：Explicit empty figure scaffold; contains no measured values.

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig07-latency-cdf.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

### Figure 8: fig08-mitigation-history

Historical P4M newboot acquisition (median/IQR) and separately rendered clean/isolated S/N/P/PN implementations (median/95% conditional bootstrap CI). A common-window service rate is calculated per trial from A completed jobs x 4096 bytes divided by W/50MHz; it is not A full-task throughput. The composition panels are distinct versions/boots.

局限：Historical loaded-byte identity is unknown; dirty kernels retained.；P4M panel selects only acq29_newboot; earlier acquisitions remain separate.；A rate excludes post-V tail and uses the original fixed-50MHz assumption; independent A full-task completion time is not measured by this panel.；Composition common-window completion counts equal 4096 in these trials, so the derived A rate shares the measured W denominator.；Unified-baseline mitigation reruns remain missing.

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig08-mitigation-history.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

### Figure 9: fig09-capacity

Capacity sensitivity in RTL simulation with fixed FIFO behavioral DDR: 16-cycle head startup, then one 8-byte beat/cycle. V complete-task cycles and A walks are shown; V walks are zero in the accepted matrix. Each point is one deterministic configuration; no statistical CI or FPGA timing is inferred.

局限：Simulation only; model service differs from FPGA DDR.；No repeated trials or bootstrap CI.；A256/V64+2 warmup and the simulation supply model differ from the board campaign; calibration supports qualitative trends only.

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig09-capacity.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

### Figure 10: fig10-invalidation

Fixed-work invalidation RTL simulation: B completion, A last COPYDONE, and A including final flush, each divided by its matching same-domain/page-count M0 metric. B=8-page panels are descriptive deterministic configurations.

局限：Behavioral DDR and simulation steps only; CPU/Linux not executed.；Selected B=8-page full-api subset; B=4 and unmap-only remain in source table.；No repeated trials or confidence intervals.

LaTeX：`\includegraphics[width=\columnwidth]{../analysis/figures/fig10-invalidation.pdf}`。全部稳定行号见`figure-trace.json`相同artifact_id；未声明尚未逐句核对的正文claim。

## 合并与论文数字接口

`merged-board-results.csv`只是阶段性副本，**从不覆盖**共享`data/board_results.csv`。`merge-manifest.json`记录本次输入hash、base分组和每个新增实验的纳入/拒绝理由。Root A未跟踪结果可显式只读传入：`build.sh --experiment-root /home/xvhaocheng/work-root-a/paper/ispass2027/exp`，并可额外传入原工作树exp。只有明确complete/passed且hash绑定的结果被接受；细节见[合并合同](merge-notes.md)。

论文数字统一由`tex/numbers.tex`产生，纯数字不带百分号。`tex/numbers-provenance.json`和此目录同名副本提供宏→公式→row_id或B1 result_id；B1 JSON继续指向操作数原始文件、pointer和行。历史E3 own-service=4的八个原始pointer与文件hash另存`history-number-inputs.json`。这些最小原文出口不追认新的运行身份。

当前构建检查覆盖：已知中位/IQR例子、固定seed与boot内分层、相对慢化定义、缺数据不补零、混base拒绝、重复ID拒绝、摘要行不能当重复、未完成/CSV哈希不符拒绝；实际十图PDF/PNG存在及3.5英寸尺寸已验证。图1、2、8和10另外经过本会话目视检查；其余骨架按同一可见标记生成。此说明由实际trace重建，不是新的实验结果。
