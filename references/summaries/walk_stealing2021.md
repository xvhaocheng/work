# Improving GPU Multi-tenancy with Page Walk Stealing

稳定ID：`walk_stealing2021` · 类型：`research_paper` · 主题：`gpu`

作者：B. Pratheek; Neha Jawalkar; Arkaprava Basu；年份：2021；出处：2021 IEEE International Symposium on High-Performance Computer Architecture (HPCA), pp. 626-639。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：GPU多租户的page walks在共享walker中交错，使租户相互干扰；静态walker分配又可能浪费空闲资源。

机制／已存认识：多租户下对共享 PTW 池软分区 + 工作窃取：DWS 仅在 walker 空闲且其 owner 无 pending walk 时窃取他人 walk（每 walk 最多等一条别人的 walk）；DWS++ 在租户排队 walk 数差异超阈值（DIFF_THRES，按 epoch 内到达比例动态调）时也允许窃取，并用 QUEUE_THRES 与 is_stolen 位限制连续窃取。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：租户数在设计时固定，但支持动态增删并重算分区（NVIDIA MPS 也限量）；tenantID 2 租户 1 bit；默认 16 walker、2 租户、192 项队列，总状态 192 bit；初始 walker 均分；epoch 默认 200 walk；QUEUE_THRES=0.51；DIFF_THRES 0.3-0.9；共享 L2 TLB 1024 项 16 路、PWC 128 项、L1 TLB 32 项私有；以 2 租户模拟为主，3-4 租户抽样验证

限制：纯模拟；不涉及页错误、shootdown、跨设备共享翻译、远端 PTW；DWS 可能损公平（BLK.3DS 等）；DWS++ 以吞吐换公平；HH 类完全资源受限时无解；TLB 严重抖动时收益有限；用平均/吞吐/加权指标，未做尾延迟度量

与研究关系：说明walker分配、借用及与MASK结合已有前作；当前单PTW接口不能免费拥有多walker选择能力。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)
- [来源2](https://doi.org/10.1109/HPCA51647.2021.00059)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/walk_stealing2021.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.pdf) · SHA-256 `5672fecbdbbd99e8f5a3ccccd5fde3f5877c8f36124f5010186dfd21f633d22a`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:21902](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt:25](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt:96](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt:163](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt:469](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt:696](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt:867](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:28](../../iommu-platform/docs/research-landscape-20261005.md)：

第二阶段是多加速器共享与 I/O 软件控制面：[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)、[Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 研究 GPU 多应用/多租户下共享 TLB 与 walker 的干扰，说明增加 TLB/walker 不自动消除竞争；[rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)、[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)、[IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) 则把 I/O 路径的 IOVA 分配、失效批处理和 IOTLB miss 暴露为一阶成本。已解决的是：软件 IOVA 管理、ring 语义、批量/延迟失效、hugepage mapping 都有可测收益。新条件使原假设吃紧：10–40 Gbps 到 100–200 Gbps、单租户到多租户、性能目标到时间可预测性与不可信设备安全共享。

[iommu-platform/docs/research-panorama-audit-20261010.md:32](../../iommu-platform/docs/research-panorama-audit-20261010.md)：

地图（研究地图、共享翻译地图）起两个作用：提供近邻机制与必须比较的强对照，并限定主张边界。盲区同样要写清：地图组织相邻机制与约束，不是已被证明的开放问题清单，不是六条必须同时实现的任务；没看到同名工作不等于空白；地图不能替代工作负载动机，也不能证明某个具体信息或动作在这块板上有效。文献只按根裁决已直接核部分使用：[Bermuda RTCSA2025](https://arxiv.org/html/2508.19670v1) 支持 DMA 共享 IOMMU 干扰已有直接前作，且其自身缓解评估不完整，不能据此作领域级新颖性主张；[MASK ASPLOS2018 §5.4](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) 支持数据/翻译争用、PTE 优先级与应用感知数据调度已有前作，但 GPU 控制点不自动映射到 DMA；[DAC2023](https://libingyao.github.io/assets/files/DAC2023.pdf) 图 10-11 附近支持分区/调度交互与命中率/性能分离已有前作，但不是同一 PN 实验；[DWS HPCA2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 的直接核对只支持把 MASK 与 DWS 组合不一定带来显著额外收益，这是组合收益不自动叠加，不是本板 slowdown 的复现。

[iommu-platform/docs/research-landscape-20261005.md:40](../../iommu-platform/docs/research-landscape-20261005.md)：

共享资源上，静态容量分配能提供部分隔离但可能低利用；动态共享能提升吞吐但可能破坏可预测性。[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf) 显示多地址空间共享 GPU L2 TLB 和 page walker 时，translation 会成为一阶瓶颈，token/bypass/DRAM 调度有收益；[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 显示静态划分 walker 会 under-utilization，动态偷取提升多租户吞吐，而 DWS++ 为了改善公平，会牺牲部分总吞吐——这不是无条件定律；[KPart](https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf) 在真实 Intel LLC 上显示 per-app way partition 粒度粗、associativity 低，聚类后才改善；其背景先行点也要保留：给后台更多缓存可降低后台内存带宽需求、改善 moses 前台尾延迟，因此现有 P/N 组合现象不自动新。原证据卡见 L2。剩余约束：本次证据未确立跨平台通用隔离结论，不能写成“隔离与利用率不能同时最优”的形式化不可能定理。

[iommu-platform/docs/research-novelty-assessment-20261015.md:44](../../iommu-platform/docs/research-novelty-assessment-20261015.md)：

| **MASK，ASPLOS 2018，§5.2–5.4；DWS，HPCA 2021，§V–VI** | MASK已联合管理TLB、cache和内存服务，优先PTE同时照顾应用data；DWS用walker分配/借用限制跨租户walk交错，依赖walker内可用的选择能力 | A关注现有端点与不可撤回义务下能部署的控制及消费者结果；不能把“翻译与数据联合考虑”或“非抢占残余”当新点，也不能把GPU能力免费移植到当前单PTW平台。[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)；[DWS](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) |

[返回总目录](../README.md)
