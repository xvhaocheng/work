# Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems

稳定ID：`bermuda2025` · 类型：`research_paper` · 主题：`io`

作者：Diogo Costa; José Martins; Sandro Pinto；年份：2025；出处：RTCSA 2025。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：混合关键性系统中多个DMA共享Arm SMMUv2翻译结构时，IOMMU本身可能成为可测时延干扰来源。

机制／已存认识：在 ZCU104 上逆向 SMMUv2 TBU/TCU 缓存，测量多设备共享 TBU/TCU 的干扰；小传输 worst-case 延迟 1.79x。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：ZCU104 + ARM CoreLink MMU-500（SMMUv2）；TBU 分布式，TCU 含 macro-TLB/PTW cache/prefetch buffer。；PMU 只能测 micro-TLB，macro-TLB 深度按规格 2000。；不同 StreamID 代表不同设备/流；即使访问同一页，也需要独立 translation entry。；FPGA DMA IP 16B payload、63 pages circular、4KB page、未用 superpage/cache coloring。

限制：单一 COTS 平台 ZCU104/MMU-500；跨架构结论只是 hypothesis。；PMU 不能完整测 macro-TLB；macro-TLB 深度未实测。；16B synthetic payload；未测 superpage/cache coloring；缓解策略未评测。；未覆盖动态映射、迁移、失效、远端 PTW、SVA、多租户。；平均吞吐对很多场景不敏感，但 worst-case latency 受影响。

与研究关系：共享DMA翻译干扰的实物动机；不把单平台测量外推为本RISC-V平台的根因或一般延迟保证。

相邻资料：io主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://arxiv.org/pdf/2508.19670)
- [来源2](https://arxiv.org/abs/2508.19670)
- [来源3](https://arxiv.org/html/2508.19670v1)
- [来源4](https://arxiv.org/abs/2508.19670v1)
- [来源5](https://arxiv.org/pdf/2508.19670v1)
- [来源6](https://doi.org/10.1109/RTCSA66114.2025.00027)
- [来源7](https://ieeexplore.ieee.org/document/11153110/)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/bermuda2025.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.pdf) · SHA-256 `d85ffeaa949a3ad5f17e950871a2f85c49b61bf237e70fca813a2535bb8a51f8`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:767](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:12](../../iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:10](../../iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:169](../../iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:240](../../iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:380](../../iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:499](../../iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-panorama-audit-20261010.md:32](../../iommu-platform/docs/research-panorama-audit-20261010.md)：

地图（研究地图、共享翻译地图）起两个作用：提供近邻机制与必须比较的强对照，并限定主张边界。盲区同样要写清：地图组织相邻机制与约束，不是已被证明的开放问题清单，不是六条必须同时实现的任务；没看到同名工作不等于空白；地图不能替代工作负载动机，也不能证明某个具体信息或动作在这块板上有效。文献只按根裁决已直接核部分使用：[Bermuda RTCSA2025](https://arxiv.org/html/2508.19670v1) 支持 DMA 共享 IOMMU 干扰已有直接前作，且其自身缓解评估不完整，不能据此作领域级新颖性主张；[MASK ASPLOS2018 §5.4](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) 支持数据/翻译争用、PTE 优先级与应用感知数据调度已有前作，但 GPU 控制点不自动映射到 DMA；[DAC2023](https://libingyao.github.io/assets/files/DAC2023.pdf) 图 10-11 附近支持分区/调度交互与命中率/性能分离已有前作，但不是同一 PN 实验；[DWS HPCA2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 的直接核对只支持把 MASK 与 DWS 组合不一定带来显著额外收益，这是组合收益不自动叠加，不是本板 slowdown 的复现。

[iommu-platform/docs/research-literature-review-20261004.md:17](../../iommu-platform/docs/research-literature-review-20261004.md)：

| Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems（Costa, Martins, Pinto；2025；RTCSA 2025） | https://arxiv.org/html/2508.19670v1 | `out/method88-literature-20261004/web-evidence.json`；`out/method88-literature-20261004/supplement.json`；原文 §V-A/§V-C/§V-D（L516-L553） | ZCU104/Arm SMMUv2 上共享 IOTLB 在并发 DMA 下引入可测延迟；小 payload 延迟最高 1.79×、16B 吞吐降约 32% | 缓解措施未做完整实验评价（§V-C 明确完整实验评价超出本文范围，L532）；§V-D 只列未来方向；SMMUv3 micro/macro-TLB 与 PTW cache 的讨论属于 discussion，不是已实测验证 SMMUv3 显著吸收干扰 | 重叠：共享 IOTLB 干扰表征、按 stream-ID 的 QoS 调节提议、page coloring 分区建议。区别：Arm SMMUv2 单点表征；本课题锁定开源 RISC-V IOMMU，问的是控制计费是否优于简单计数 |

[iommu-platform/docs/paper-scenarios-mitigation.md:164](../../iommu-platform/docs/paper-scenarios-mitigation.md)：

| Costa、Martins、Pinto，RTCSA 2025，*Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems*：[作者全文](https://arxiv.org/html/2508.19670v1)、[正式 DOI](https://doi.org/10.1109/RTCSA66114.2025.00027) | 已在 Arm SMMUv2/ZCU104 上测量共享 IOMMU 的 DMA 干扰，并做 SMMU 关闭对照。第 V-C 节讨论流量调节、映射/coloring 等候选，但明确完整缓和实验超出本文范围。因此“首次发现跨 DMA 干扰”不成立，措施概念也需进一步对照。 |

[iommu-platform/docs/shared-translation-literature-20261006.md:67](../../iommu-platform/docs/shared-translation-literature-20261006.md)：

**4. 平均吞吐、公平性、尾延迟与可保证的进度之间仍有距离。** MASK、DWS等研究已关心并发与公平；F&S已有尾延迟数据，Bermuda已有实物干扰测量。更强的目标需要覆盖完整翻译服务链，明确负载约束及资源供给。仅把IOTLB分区，不能据此推出PTW、互连、命令队列和DRAM也互不干扰。此处是组合问题和证明范围，不是简单宣称此前无人测量干扰。[Bermuda正式出版](https://ieeexplore.ieee.org/document/11153110/) · F&S全文

[返回总目录](../README.md)
