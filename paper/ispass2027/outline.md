# ISPASS 2027：论文大纲与图表合同

定位：FPGA实测刻画论文。正文9页（图表、附录计入，参考文献不计），IEEE双栏、双盲。本文工作笔记保留本地身份；匿名稿不包含会暴露作者的仓库链接，保留第三人称自引，并按最终使用范围披露实质AI辅助。[官方要求与前作核查](related/prior-work.md#venue)。

## 一句话主张与五条结论草稿

**主张：** 在一个开源RISC-V IOMMU的多DMA FPGA平台上，结合自主批量供给、分段与逐请求观测以及受控干预，区分零miss共跑损失、miss阻塞hit与容量替换，并量化缓解对双方性能的取舍及组合冲突。

1. **V自身没有翻译miss也会明显慢化。** 新统一DS50基线每条件10次，中位吞吐Solo/A1/A8/A64为282.94/149.53/103.27/94.84 MiB/s；A1双方miss为0。A8约剩Solo的36.5%，A64约剩33.5%，分别绑定工作集，不能把全部损失称为翻译miss成本。历史主矩阵、扩展及新基线分panel。
2. **等待传播与容量挤占是不同证据链。** 新统一DS50中V读侧内部每请求平均经过时间的跨试次中位数从25升至600.91周期，V miss为0，复现历史path结果；E3 v6四个64B cold互补对把A剩余服务与V剩余等待的变化关联到具体请求。E2的W7/W8则分别观察驻留与逐出后首次miss，探测总周期中位数696/1483。它们不构成整个A8慢化的可加分摊。
3. **服务延迟条件会显著改变干扰强度。** E1b的DS0/25/50/100四档中，A8相对同区组A1的额外硬件慢化中位数为10.40%/24.32%/44.78%/76.08%，A64为16.35%/36.82%/57.63%/98.12%，每点十配对并带IQR。A64接近线性；A8/A16在50→100段边际增幅趋缓，尚未出现平台式饱和。[四档完整统计与图](data/ds-sweep-summary.md)。这是DS附加延迟敏感性，不等于纯PTW成本；DS0是直接连线，正延迟档启用诊断delayer。DS0的A8/A16/A64中V HPM miss中位数为2，软件快照与硬件窗口分开。旧报告中“中位数之比”的统计仍保留原口径。
4. **缓解需同时核算双方代价，组合不保证收益相加。** P4M通过减缓A换取V加速；隔离替换历史后P/PN的A miss数量差为0，PN仍比P慢12.67%–12.79%（三个描述性配对）。板上差额的完整根因尚未解释，不把行为模型当板测归因。
5. **失效范围还有独立的性能取舍，仅有仿真证据。** 10/17原版IOTLB加必要修复的24主配置与4对照中，宽失效拖慢B；按域/地址缩小范围消除已归因miss，但A完整时间可能增加。本节明确标为Verilator/行为DDR仿真，不能写成Linux或FPGA结果。

上述数字由[板测长表](data/board_results.csv)及[口径说明](data/README.md)追源。独立G1九次通过冻结阈值；新统一100次按实际25/50/25完成，DS50的每条件10次跨两个boot，DS0在一个boot，不能称为十次独立重启。今后同配置集中测完，默认50/50两阶段，可复用已有有效启动。前作比较承认Bermuda的Arm多DMA实测、ETH的RISC-V FPGA及host干扰、Rodríguez同平台硕论及HERO/Kurth前史；差异落在具体内部等待证据和负载/缓解数据，Procedia与Hur/Ro全文仍未决。[定位及未核范围](related/prior-work.md#comparison)。

## 九页结构与叙事顺序

| 部分 | 页数预算 | 内容和证据 |
| --- | ---: | --- |
| 1 Introduction | 0.75 | 问题、主张、五条发现；不认领首次发现多DMA干扰 |
| 2 Background and related work | 0.75 | Bermuda、ETH、16项共享IOTLB与串行服务；区分前作事实及待核近邻 |
| 3 Platform and methodology | 1.25 | 图1，软件/硬件窗口，实际版本和被动观测边界，G1与统一基线 |
| 4 Characterization and causal evidence | 3.00 | 图2–5：零miss基线→路径定位→局部干预→容量边界 |
| 5 Mitigation tradeoffs | 1.00 | 图6；P4M双方取舍、S/N/P/PN和组合退化 |
| 6 Sensitivity and applicability | 0.90 | 图7–9的待补结果；未测前不写结论，不能用历史极值冒充p99 |
| 7 Invalidation scope: simulation | 0.75 | 图10；行为DDR、无CPU/Linux执行，B与A完成分别报告 |
| 8 Limitations and conclusion | 0.60 | 适用范围、数据缺口及可复用观测方法；合计9.00页 |

图可用多panel紧凑排版，页数预算包含相应图表。若待补图尚未完成，保留明确缺口，不以示意数据或模型预测填入实测曲线。

## 十张图的取数清单

每个稳定`row_id`及**当前CSV物理行号**在[figure-rows.csv](figure-rows.csv)逐行列出，并由[生成脚本](figure_rows.py)按当前表重建；数据增量后重建映射，正文不依赖会移动的旧行号。

| 图 | 回答的问题与拟绘内容 | CSV行选择与来源 | 现状 |
| --- | --- | --- | --- |
| 1 | 可观测哪些边界？四DMA→共享IOMMU→DS/COMP/DDR，标出供给、四段路径及E3干预位置 | 非定量架构图，无CSV数值；固定修复源码、batch/path协议及E3原始合同 | 已有源码，按实际构建版本画图 |
| 2 | V的共跑损失多大，DS改变什么？硬件时间/吞吐、V/A miss与工作集，历史和新基线分panel | `batch_main.v0-{Solo,A1,A8}.hardware_{ms,MiB_s}.median`；`batch_ext.v0-{A16,A32,A64}.*`；`batch_ds0`、`batch_ds50_retest`同名行；`board_baseline100.baseline100-20261017-01.*`中完整n10硬件时间/吞吐及miss，完整ID见行映射 | 历史、新统一100次和E1b新增100次均已有；图2的四档配对曲线由scripts/plot_ds_sweep.py生成，原始操作数及派生统计见data/ds-sweep-summary.json；不跨版本合并 |
| 3 | 等待增长在哪段？读写各四段的每请求均值，明确重叠、禁止堆叠成总慢化份额 | `path.{Solo,A1,A8}.v_{r,w}_{0,1,2,3}_mean_cycles.median`；新统一系列`path_v{r,w}{0,1,2,3}_mean_cycles.median`的Solo/A1/A8完整n10行 | 历史30试次及新统一100次均已有，DS与版本分开 |
| 4 | A miss如何挡住V hit？一条请求时间线与四个互补对变化，另放D0/D0和hot阴性 | `e3v6.main-case{2,4,6,8}.delta_{A_remaining,V_remaining,V_wait,v_hw_cycles_delta}.paired_difference`及原始事件；图映射保留全部正式对 | 已有，64B局部诊断，n=4，不推广完整归因 |
| 5 | 驻留何时变逐出？W7/W8快照与首次再用miss/探测时间，方向分开 | `e2.boundary-w{7,8}-{forward,reverse}-n112.{d8d_v_miss,d89_v_main_cycles,d87_done_issue_cycles}.median` | 已有，每条件5次，特定访问/替换布局 |
| 6 | 缓解保护V付出什么，PN是否优于P？P4M前后台取舍和隔离后的S/N/P/PN | `p4m.acq*-gap*.{v_mib_s,a_window_mib_s}.median`；`composition.isolated_history-*`的W及三个A8配对退化，见完整映射 | 历史已足描述性比较；统一基线复测待补 |
| 7 | 设备数增加后如何变化？2→3→4设备，V延迟和其他设备进度 | 尚无对应板测行；新运行须生成独立`baseline_scaling.*`系列 | 待补，本次不执行 |
| 8 | 负载形态和传输大小能解释与Bermuda的差别吗？持续顺序、持续随机、周期突发×大小扫描 | 尚无对应板测行；新`baseline_shapes.*`系列；前作条件见related比较，不跨平台相除绝对带宽 | 待补，本次不执行 |
| 9 | 4KiB/2MiB如何改变吞吐与请求尾延迟？按页大小画CDF/分位及最大观测值 | 尚无完整分布行；新`baseline_pages_latency.*`系列；旧path只有均值和有限最大值，不能补出p50/p99 | 待补，需要有界分布采集能力 |
| 10 | 缩小失效范围如何改变双方完成和归因miss？模式对M0归一化、同时画A代价 | 独立`invalidation_sim_results.csv`中`main24`的`T_A/T_B/A_final_flush_end/B_time_over_M0/B_invalidation_attributed_misses`，不使用board表 | 已有24+4仿真；图题和轴注明仿真步 |

## 待补实验与局限

待补均在新统一修复基线上评价，先确认现有CLI/硬件是否具备能力，再准备有界扩展；本次只登记，不实施或新增判别门。

- **设备数2→3→4：** 同一V，增加独立DMA干扰源；保持每源及合计压力分别可辨，检查各自完成/排空。
- **三类负载及传输大小：** 持续顺序、持续随机、周期突发；说明工作集、复用和供给的控制关系，扫描传输大小，优先覆盖前作16B→4KiB范围中本平台合法部分，超出现有接口的点标为能力缺口。
- **4KiB与2MiB：** 验证实际页表叶级和对齐，不以虚拟地址步长或内存分配大小冒充页大小；固定同工作量。
- **延迟分布：** 请求级p50/p99/最大观测值；当前sum/full与max无法重建分布，须新增有界直方图或追踪并报告丢失/溢出与采样范围。
- **缓解统一复测：** P4M及S/P/N/PN独立版本，同输入/软件/频率，报告V、A共同窗口进度和完整双方完成；不把旧版本的最好数拼成统一结果。

局限须保留：单个控制器与FPGA不能代表RISC-V规范或商用IOMMU；合成COPY不等于真实应用/PCIe；修复改变服务顺序；观测逻辑也有资源/布线影响；历史BIT/内核与加载验证不齐；多数重复来自同启动，不能当独立重启；WNS达标不等于全部CDC/IO问题已签核；E3有限干预、E2有限布局、PN n=3均不能支持普遍统计结论；失效部分无Linux和板测；Procedia近邻全文及前作图文冲突仍有未核项。
