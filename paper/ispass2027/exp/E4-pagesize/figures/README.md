# E4 实测硬件时间论文图

实际20/20合格点来自Root的不可变 `E4-pagesize/paper/e4-common-v2-final01/`。
图包 `e4-common-v2-final01/` 保存原summary、17列paper.csv、analysis manifest字节副本，
as-executed绘图脚本、原输入SHA、十对boot/sequence/case/raw-SHA与数值核对。
没有新增板点，没有丢弃首点或其他观测。

`e4-leaf-times.svg`为可编辑矢量图，PDF/300dpi PNG在同目录本机保存，二进制不入Git。
已目视检查PNG，所有20点/10配对线与10个比值点可见，未裁掉较慢的首个4K点。
左图水平微移只用于分开block；黑线为每cell median/IQR，IQR很窄所以另标精确数值。
右图是同boot/block的2MiB/4KiB时间比，黑线/灰带为该10个比值的median/IQR，纵轴明确放大至0.760–0.775。
原始样本和配对关系在 `paired-hardware-times.csv`，不能把比值中位数换成两cell中位数之比。

实际4K硬件时间median139.19783ms，IQR0.017655ms；2M median106.88277ms，IQR0.04487ms。
配对2M/4K时间比median0.7678094640，IQR0.0006430457（n=10），对应配对时间减少median23.2190536%。
这是固定V0/A1、4KiB COPY、V4096、每buffer2MiB/四buffer总8MiB、真实4K/2M leaf条件的硬件V窗口结果。
不将该差值称纯PTW成本；HPM为非原子软件括号，A最终工作包含startup/drain，未进入本图的时间统计。

图注候选：

> V hardware time under fixed V0/A1 traffic with 4 KiB versus 2 MiB IOMMU leaves. Each condition uses ten measured trials with the same 8 MiB total buffer allocation and 4 KiB COPY size. Left: every paired trial is shown; black marks report the median and interquartile range. Horizontal offsets distinguish paired blocks. Right: all ten same-block 2 MiB/4 KiB time ratios, with the median and interquartile range. The ratio axis is zoomed. No trial is removed.

复跑必须新建输出目录：

```bash
/tmp/ispass-ds-sweep-plot-venv/bin/python \
  /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E4-pagesize/figures/plot_leaf_times.py \
  --source /home/xvhaocheng/work-root-a/paper/ispass2027/exp/E4-pagesize/paper/e4-common-v2-final01 \
  --output /home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E4-pagesize/figures/e4-common-v2-review-new
```

复用P0既有Python3.10.12/matplotlib3.10.9/Agg环境，没有安装新绘图库。
脚本固定三份输入SHA，拒绝fixture/incomplete/不同tuple的输入；重算Type7、逐值核17列CSV来源指针，
并保存20条唯一boot/sequence引用及10个真实配对。它不再访问UART或重新解析/采集硬件数据。

E5 stage1附图的原真实数据方案见 `e5-stage1-proposal.md`。
Root接受后实施的图包为 `e5-stage1-final01/e5-stage1-intervals.{pdf,svg,png}`，PDF/PNG同样仅本机保存。
两panel分别V read/write；纵轴为base2对数cycles（1cycle20ns），每种leaf在p50/p95/p99画原桶闭区间。
没有中点、精确p99、精确CDF或请求pooling。12条显示区间各代表10个逐trial完全相同的原区间，
图中明确写10/10 trial intervals identical、n是trial数、线段宽度不是trial CI。
40条原trial-stage/120个原分位区间、三个完整原出口字节/输入SHA与全部boot/seq/raw引用保存在图包。
Stage1是TR握手至IOMMU首次输出请求呈现的路径驻留，不解释成纯PTW成本或DMA descriptor latency。

E5图注候选：

> Per-trial p50, p95 and p99 bucket intervals for V read/write bursts in PATH stage 1 under 4 KiB and 2 MiB IOMMU leaves. Vertical axes use a base-2 logarithmic scale in cycles (20 ns per cycle at 50 MHz). Every displayed closed interval is identical across all ten trials of that condition; the spans are histogram quantization bounds, not confidence intervals. No requests are pooled across trials, and no within-bucket point estimate is inferred. Stage 1 measures residence from translation-input handshake to first output-request presentation, including internal processing.

复跑用同一隔离绘图环境和全新输出：

```bash
/tmp/ispass-ds-sweep-plot-venv/bin/python \
  /home/xvhaocheng/work-root-a/paper/ispass2027/exp/E4-pagesize/figures/plot_stage1_intervals.py \
  --source /home/xvhaocheng/work-root-a/paper/ispass2027/exp/E4-pagesize/figures/e5-stage1-final01/inputs \
  --output /home/xvhaocheng/work-root-a/paper/ispass2027/exp/E4-pagesize/figures/e5-stage1-review-new
```
