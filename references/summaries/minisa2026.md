# MINISA: Minimal Instruction Set Architecture for Next-gen Reconfigurable Inference Accelerator

稳定ID：`minisa2026` · 类型：`research_paper` · 主题：`data-lifecycle`

作者：unknown；年份：2026；出处：arXiv 2026。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：可重配置 AI 加速器的控制 overhead：指令 fetch 挤压片内存储、降低 tile size、成为一阶瓶颈（16×256 时 stall 占 96.9%，`minisa2026.txt:15-68, 226-234`）。

机制／已存认识：以 Virtual Neuron（VN = AH-element dot product）为抽象粒度；FEATHER+；8 条指令（SetIVN/SetWVN/SetOVN Layout + ExecuteMapping/Streaming + Load/Write/Activation）；3 位排列编码（`minisa2026.txt:253-270, 241-256, 308-348`）。

支持边界：既有已存全文与逐篇笔记；本轮不重审评价。

限制：未复现论文实验；仅存全文不等于全文精读。；MINISA 解决的是控制面，不能推出它对数据搬运/IOMMU/间接访问有直接贡献——它与本线其它论文正交。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。

原始来源：

- [来源1](https://arxiv.org/pdf/2603.20623v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/minisa2026.pdf) · canonical：[原路径](../../iommu-platform/out/research-landscape-20261005/papers/minisa2026.pdf) · SHA-256 `d981361e9a7d11a6633b916f86745f953a21ce75f545cb40dd97233e9ff3f4d3`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/research-landscape-20261005/source-manifest.json:266](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:107](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:105](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/innovation-council18-20261005/round1/C6.md:23](../../iommu-platform/out/innovation-council18-20261005/round1/C6.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C6.md:23](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C6.md)
- [iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md:101](../../iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md)
- [iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md:143](../../iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md)
- [iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:64](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:107](../../iommu-platform/docs/research-landscape-20261005.md)：

| [MINISA](https://arxiv.org/pdf/2603.20623v1) | ISPASS 2026；本地 arXiv v1 | 可重配置推理加速器的最小控制 ISA | RTL/分析模型；主要优化控制/指令供给，不能直接外推为 IOMMU 性能收益；控制与数据面仍相互影响 |

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:72](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"old": "### 7.1 已具备较强全文/关键节证据的前沿\n\n- [HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)：HPCA 2026，wafer-scale GPU 分布式翻译；重要限制是排除 GPU-to-GPU migration，未评估 TLB shootdown，结果为仿真。\n- [Revelator v2](https://arxiv.org/pdf/2508.02007v2)：ISCA 2026，DOI 10.1109/ISCA66397.2026.00147；v2 标题为 System-Software-Guided；Linux 6.10.8 原型 + Sniper/Virtuoso + RTL 综合；OS 分配协同仍是前提，不照搬 v1 旧数字。\n- [ShareMMU](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf)：MICRO 2026 camera-ready；Permission Tracker、定向 back-invalidation、权限下降局部化；仍等待 dirty write-back/flush acknowledgment，未消灭全部动态更新/排空成本；只作为前沿条件，不据此转安全主线。\n- [DX100](https://arxiv.org/pdf/2505.23073v1)：ISCA 2025，批量 indirection 重排；明确不覆盖 pointer chasing。\n- [MINISA](https://arxiv.org/pdf/2603.20623v1)：ISPASS 2026；本地 arXiv v1；控制面 ISA，与数据搬运正交。\n- [ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf)：ISCA 2026；真机 RTX 3090，采样观测。\n- [OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf)：HPCA 2025，对象感知多 GPU page management。\n- [Tiara](https://arxiv.org/pdf/2606.13708v1)：arXiv 2026 预印本，不是 NSDI 2022 同名负载均衡器；应用间接表≠硬件 OS 页表；Table 1 中 PagedAttention 最优批处理为 2 RTT，不能只拿朴素 160 RTT 讲必要性。",

[iommu-platform/out/innovation-council18-20261005/round1/C6.md:23](../../iommu-platform/out/innovation-council18-20261005/round1/C6.md)：

| [MINISA](https://arxiv.org/pdf/2603.20623v1)，`minisa2026.txt` §III-D、§IV-G、§V、§VI-A/B | VN 抽象压缩合法的控制空间；布局设置、搬运、计算分工明确；mapper 已搜索跨层兼容布局。大规模结果依赖既定指令带宽与 micro-instruction 对照，评估为 RTL/分析模型 | 小 descriptor 或小 ISA 不等于更少物理访存/更少页翻译；“选择合适抽象粒度”不是未被提出的想法。不能把其数字迁到当前 IOMMU |

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C6.md:23](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C6.md)：

| [MINISA](https://arxiv.org/pdf/2603.20623v1)，`minisa2026.txt` §III-D、§IV-G、§V、§VI-A/B | VN 抽象压缩合法的控制空间；布局设置、搬运、计算分工明确；mapper 已搜索跨层兼容布局。大规模结果依赖既定指令带宽与 micro-instruction 对照，评估为 RTL/分析模型 | 小 descriptor 或小 ISA 不等于更少物理访存/更少页翻译；“选择合适抽象粒度”不是未被提出的想法。不能把其数字迁到当前 IOMMU |

[返回总目录](../README.md)
