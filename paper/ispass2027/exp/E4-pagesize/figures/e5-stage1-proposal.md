# E5 stage1 V读/写分位桶区间附图方案（仅设计）

已只读核Root的 `E5-latency/paper/e4-common-v2-final01/` 实际完整20 trial出口。
输入summary / trial-stage-intervals.csv / histogram-bins.csv的SHA保存在 `e5-stage1-actual-inputs.json`，
只选择V、stage1的40条trial-stage原行，另存 `e5-stage1-actual-intervals.csv`（20trial×read/write）。
每种leaf×channel有10个独立trial，原boot/sequence/block/raw引用均保留；没有新板点。

实际逐trial原桶区间如下。每个cell的10个trial在该分位均落相同区间（已逐行检查40条/120个分位区间）：

| V stage1 | leaf | p50（cycles） | p95（cycles） | p99（cycles） | trial n |
|---|---|---|---|---|---|
| read | 4KiB | [1024,2047] | [1024,2047] | [1024,2047] | 10 |
| read | 2MiB | [16,31] | [16,31] | [16,31] | 10 |
| write | 4KiB | [256,511] | [512,1023] | [512,1023] | 10 |
| write | 2MiB | [8,15] | [8,15] | [8,15] | 10 |

建议附图用read/write两panel、横轴p50/p95/p99、纵轴log2 cycles，
每个分位各画两种leaf的闭区间线段，不放中点，不用精确p99标记。
注明“10/10 trial intervals identical”，n指trial数，不是合并后的81920请求。
因同cell十条区间完全相同，可以明确标重合次数而不伪造trial间差异；
如Root希望每trial全部显式展示，可将每分位下十个block水平分开，实际端点不变。

本方案不对请求跨trial池化、不插值桶内分布，不画精确CDF、不比较猜测p99比值。
V stage1各trial full=8192、carry/opening/closing=0的原记录随CSV保留；
如果后续条件存在carry，应保持full-only范围并另列库存，不能悄悄纳入桶。

stage1沿用已审被动observer的源码事件边界，是AXI read/write burst在该路径段的驻留周期，
不称单个DMA descriptor延迟，也不称纯PTW或单独页表访问成本。
附图若实施，必须绑定本次E4相同boot/BIT/Linux与原区间CSV、保留原量化分辨率。
