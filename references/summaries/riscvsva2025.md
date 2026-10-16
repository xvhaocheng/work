# Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs

稳定ID：`riscvsva2025` · 类型：`research_paper` · 主题：`accelerators`

作者：Cyril Koenig; Enrico Zelioli; Luca Benini；年份：2025；出处：DATE 2025。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：RISC-V嵌入式异构SoC采用IOMMU共享虚拟地址时，零拷贝便利与页表遍历/软件卸载开销之间存在成本取舍。

机制／已存认识：在开源RISC-V SoC集成IOMMU，硬件IOTLB/PTW支持SVA；shared LLC缓存host和IOMMU PTW，DMA绕过LLC并由主机self-invalidate。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：RISC-V IOMMU v1.0；设备目录项按(device, process)缓存根页表；shared LLC只服务host和IOMMU PTW，DMA绕过；Linux 6.9补丁和OpenMP offload；FPGA emulation加DRAM delayer

限制：FPGA emulation，非硅后；低时钟运行，DRAM延迟用delayer模拟；IOMMU仅4个IOTLB项；需要shared LLC才能把overhead降到2%以下；DMA需绕过LLC，主机需self-invalidate；未评估TLB shootdown、页面迁移、A/D位；本地保存arXiv作者稿；DATE2025官方program核定会议，DOI由题录交叉核定。

与研究关系：同开源IOMMU族的部署与成本近邻；单任务/host内存流不同于已证明多DMA控制净收益。

相邻资料：accelerators主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://arxiv.org/pdf/2502.17398v1)
- [来源2](https://arxiv.org/abs/2502.17398)
- [来源3](https://arxiv.org/html/2502.17398v1)
- [来源4](https://arxiv.org/abs/2502.17398v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/riscvsva2025.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.pdf) · SHA-256 `4610cbf0fc9eec49e41618c3dad28c4ba609c4f596efc26d19ec6548cd98c319`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:15522](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt:14](../../iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt:239](../../iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt:263](../../iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt:284](../../iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt:341](../../iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt)
- [iommu-platform/docs/flow-iommu-candidate-review-20261014.md:11](../../iommu-platform/docs/flow-iommu-candidate-review-20261014.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-literature-review-20261004.md:22](../../iommu-platform/docs/research-literature-review-20261004.md)：

| Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs（Koenig, Zelioli, Benini；2025；DATE） | https://arxiv.org/html/2502.17398v1 | `out/method88-literature-20261004/web-evidence.json`；原文 L85、L186-L189、L200-L201 | 开源 RISC-V 异构 SoC（64-bit CVA6 + 8-core Snitch，FPGA 仿真）；IOVA 翻译可占 gemm 运行时间至 17.6%；host 与 IO page table walker 共享 LLC 后降至 <2%（摘要口径 0.4%/0.7%）；建议对 device DMA bypass 该共享 cache（L188） | 单任务 + host 并发内存流；不是多个 DMA 翻译流之间的保护与代价 | 重叠：**同一个开源 RISC-V IOMMU**（引用 [7] RISC-V Summit Europe 2023 的 open-source RISC-V IOMMU IP、[8] Zero Day Labs riscv-iommu，L200-L201）；LLC/PTW 优化。区别：它优化单流 PTW 代价，本课题问共享服务保护边界 |

[iommu-platform/docs/research-novelty-assessment-20261015.md:45](../../iommu-platform/docs/research-novelty-assessment-20261015.md)：

| **Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs，DATE 2025，§III–IV** | 已在RISC-V异构SoC评价IOMMU零拷贝、软件映射/卸载开销和页表缓存；§IV-A含应用级成本，§IV-B另测不含host同步的kernel执行 | SVA、零拷贝及“要算软件开销”均有先例。A的潜在增量是多DMA共享翻译中，控制/完成/所有权位置如何改变真实消费者目标；需要任务证据及公平普通IOVA对照。[原文](https://arxiv.org/html/2502.17398v1) |

[iommu-platform/docs/flow-iommu-candidate-review-20261014.md:11](../../iommu-platform/docs/flow-iommu-candidate-review-20261014.md)：

两个背景例子可以核实：IOTLB Wall在200Gbps测试中报告相对关闭IOMMU最高约20%的吞吐下降，其hugepage-aware allocator消除对应损失；DATE2025的RISC-V SVA研究报告gemm翻译开销4.2%至17.6%，加入LLC后降到0.4%至0.7%。这些结果支持问题存在，也要求新候选面对合法大页和页表缓存等强普通方法。[IOTLB Wall](https://aliireza.github.io/files/iotlb-peerj23.pdf)、[RISC-V SVA评估](https://arxiv.org/abs/2502.17398)。

[iommu-platform/docs/paper-scenarios-mitigation.md:168](../../iommu-platform/docs/paper-scenarios-mitigation.md)：

| Koenig、Zelioli、Benini，2025，*Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs*：[作者全文](https://arxiv.org/html/2502.17398v1) | 在开放 RISC-V FPGA SoC 上评估共享虚拟地址、页表路径 LLC 和 host 并发内存流量。因此平台类别与 PTW 内存竞争并非空白；本课题拟检验的是多个 DMA 翻译流之间的机制和保护代价。 |

[返回总目录](../README.md)
