# composition-v1 布线耗时与实验电路开销（2026-10-05）

当前结论：实际日志报告 Route 35-447 布线拥堵；新布局资源使用率很高，新增实验控制与观测电路存在明显开销。不能仅据面积唯一归因为新观测模块，也不能把长时间编译当成布线/时序已失败。当前恢复继续运行，没有据此修改正在构建的RTL。

|证据|数值|阶段/边界|
|---|---:|---|
|新版全设计 LUT|183014 / 203800，89.80%|Fully Placed，未获得最终 Routed 数字|
|新版全设计寄存器|145452|Fully Placed|
|新版占用 Slice|50949 / 50950|几乎每个物理逻辑单元都有电路，单元内部仍可能有空资源|
|新增 dma_composition_control|5754 LUT / 4171 FF|Synthesized，含配置控制、计数、双快照与读出电路|
|旧 N v5 全设计|176476 LUT / 141170 寄存器|Routed；与新版阶段不同，不能精确拆分增量来源|

四模式共用同一BIT中的同一IOMMU；两位运行时配置控制N和P，不是复制四套。P仍为总16条IOTLB，保护2条/其余14；DMA0/1的512拍缓冲沿用上一版，并非本次新加。双快照在源码为开始/结束各16×128位，另有计数器与128索引读出路径；综合会优化常量/未使用位，不能用声明位数当最终寄存器数。

当前判断：此次实验仪器的面积预算和物理布局余量应更早审查。若当前路线最终不能布通或时序不过，优先审查必要观测字段、快照存储和读出结构、继承观测器的重复成本；保持关键论文证据、原ABI和时序口径。尚未实施精简、未作具体瓶颈归因。后续论文需将方法硬件开销与实验观测开销分开报告。

## 同一综合报告中的观测与控制模块拆分

|模块|LUT|寄存器|来源|
|---|---:|---:|---|
|DMA及数据路径观测 `dma_observer`|14065|9293|既有；包含路径观测8982 LUT，不再重复相加|
|研究观测 `dma_research_observer`|11326|10448|既有；包含成本分组及E2计时子模块|
|命中绕过诊断 `dma_nonblock_observer`|2245|1305|既有|
|本轮组合实验控制 `dma_composition_control`|5754|4171|新增|
|以上四个同级模块合计|33390|25217|同一Synthesized阶段，包含子模块且无重复相加|

原始层级报告第1268–1270、1319行支持以上数字。关键连接在 `ariane_peripherals_xilinx.sv` 第966、1011、1022、1052行：当前配置保留路径观测、研究观测、N诊断，同时增加组合实验控制。旧专用E3观测已关闭，不能声称所有历史电路都仍启用。当前测量窗口及控制接口仍复用既有观测器，因此这33390 LUT不是全部可删除的冗余，也不是方法本身的成本。

对用户关于实现质量的质疑：观测功能持续叠加、未在构建前完成按实验配置的必要性与面积审查，属于本次工程设计不足，由Codex承担关键设计及验收责任。现有证据支持这是布线压力的重要来源之一；还不能证明它是拥塞的唯一原因。后续应先列明172项实验真正需要的字段及Linux依赖，再裁剪历史观测和调整存储/读出结构；不能靠删除关键证据让构建通过。本次仅完成诊断记录，没有改动正在编译的RTL或另起构建。

直接证据：
- 新版布局报告：`../out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/ariane.runs/impl_1/ariane_xilinx_utilization_placed.rpt`，LUT/寄存器表及Slice表。
- 新版综合层级报告：`../out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/reports/ariane.utilization.rpt`，dma_composition_control行。
- 旧版：`../out/n-prototype-20261004/hit-v5-build-acceptance.json`；旧层级报告Design State=Routed。
- 恢复日志：`../out/paper-route-review-20261004/recovery-v1/logs/vivado-recovery.log`。
- 关键连接：`../cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv` 的composition_mode及PolicyEnable/NonblockEnable；观测源码：`../cva6/corev_apu/fpga/src/dma_composition_control.sv`。
- DeepSeek完整提取：`../out/paper-route-review-20261004/composition-routing-cost-review.md`。

DeepSeek整理资源报告与说明草案；Codex核查原始数字、综合/布局/布线阶段差异及关键RTL连接，负责限定结论。未新增测试、未重新启动构建、未上板。

## 2026-10-05 02:06 JST：按证据裁剪，重新完整编译中

仅composition profile移除旧research事件记录器及旧非阻塞诊断器，保留通用/path统计与新composition快照/计数和全部方法逻辑。10项联合仿真、4项构建入口检查已通过；独立构建正在`out/composition-trim-20261005/`运行，尚无新的最终资源、布线及时序验收。旧失败目录保留。[本次修改与实际检查](composition-trim-20261005.md)。

## 裁剪后综合资源：同阶段实际对比

旧/新均为Vivado2023.1、ariane_xilinx、xc7k325t、Synthesized：LUT172284→152609，减少19675（11.42%），器件占比84.54%→74.88%；寄存器136576→124728，减少11848（8.68%）。BRAM48、DSP27均不变。新报告2026-10-05 02:26:47，尚非最终布局布线结果，不能从面积下降宣称布线/时序通过。来源旧`out/migration-4dma-composition/fpga-src/cva6/corev_apu/fpga/ariane.runs/synth_1/ariane_xilinx_utilization_synth.rpt`与新`out/composition-trim-20261005/fpga-src/cva6/corev_apu/fpga/ariane.runs/synth_1/ariane_xilinx_utilization_synth.rpt`。DeepSeek提取计算，Codex核对原报告；[完整比较](../out/composition-trim-20261005/synthesis-resource-comparison.json)。

## 最终优化版已验收（2026-10-05 03:06 JST）

Routed实际LUT163673/203800=80.31%，Register133604，occupiedSlice50670/50950=99.45%；综合阶段11.42%/8.68%减少结论仍为同synth阶段，不把跨阶段数字混用。原base WNS−0.380ns、setup失败30，单次phys_opt Explore后WNS+0.058ns/WHS+0.052ns/WPWS+0.062ns、三类失败0、全273893线布通/error0。已生成并接受route-opt BIT/MCS；面积仍密集、IO/CDC全审查与板测尚未完成，不据此宣称方法收益。[最终验收](../out/composition-trim-20261005/acceptance.json)。
