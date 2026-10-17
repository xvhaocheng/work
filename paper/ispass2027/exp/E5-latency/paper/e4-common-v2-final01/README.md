# E5：附加在E4同20试次上的真实直方图

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Verification Status: VERIFIED（20trial、320stage行、10240个真实桶行）
- Version Label: e5-e4-common-v2-final01

这是E4的相同20个试次，不是额外20次。4KiB/2MiB各10个实际试次，全部OBS4/INFO1/Depth4、16段×32桶通过；数据、boot、seq、CLI、最终batch/PATH/hist均从原命令重解析并与缓存逐项核对。

| 映射 | V通道 stage1 | 每试次p50所在周期区间 | 每试次p99所在周期区间 | 实际试次n |
| --- | --- | --- | --- | --- |
| leaf4k | read | [1024,2047] | [1024,2047] | 10 |
| leaf4k | write | [256,511] | [512,1023] | 10 |
| leaf2m | read | [16,31] | [16,31] | 10 |
| leaf2m | write | [8,15] | [8,15] | 10 |

表中每一个区间在对应条件的10个试次里一致。1cycle=20ns；例如V read p99所在桶为4KiB的20.48–40.94us，对2MiB的0.32–0.62us。它们是nearest-rank真实请求分位数所在的桶边界，**不是精确p99，也不能用桶中心补造值**。stage1覆盖翻译及IOMMU内部处理至输出请求呈现，不是单独PTW；这些是AXI burst分段，不是DMA descriptor时延。

[trial-stage-intervals.csv](trial-stage-intervals.csv)含每试次16段的实际sum/full/mean/max、分位桶区间及carry/opening/closing；[histogram-bins.csv](histogram-bins.csv)保留全部512桶/试次；[summary.json](summary.json)给出等试次权重的端点描述统计，[analysis-manifest.json](analysis-manifest.json)绑定原始来源。

分位总体只含完全位于V硬件窗口内的full请求，carry-in/out不伪造样本；A窗口截断和原库存单列。请求数不能当独立试次n，不pool跨试次请求、不把段分位相加、不作统计显著性结论。配置/实际leaf证明与性能结果见[E4完整包](../../../E4-pagesize/paper/e4-common-v2-final01/README.md)。
