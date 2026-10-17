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

E5 stage1附图仅有真实数据基础上的方案，见 `e5-stage1-proposal.md`，尚未绘制或pool请求。
