# 根协调者新增的关键查新证据

这是根 Codex 的原始来源核查，用于 R2 公共材料。不是新增实验，不代表全领域查新完备。R1 各席独立输出不回写；阅读权不对称须披露：Codex 席可浏览，API 席只能读所传材料，R2 统一补充关键新增证据。

## ShadowUpdate（ISCA 2026）：迁移时同步传播映射已经有人做

- 作者所在机构记录及原始摘要：[Yonsei University](https://yonsei.elsevierpure.com/en/publications/reducing-page-faults-via-invalidation-based-mapping-propagation-i/)，正式 DOI [10.1109/ISCA66397.2026.00110](https://doi.org/10.1109/ISCA66397.2026.00110)。作者主页也列正式论文：[Junsung Kim](https://junsungkim97.github.io/)。
- 题名：Reducing Page Faults via Invalidation-Based Mapping Propagation in Multi-GPU Systems。
- 已核范围：机构原始摘要及作者题录，尚未取得全文，不声称审查完整协议或复现作者结果。
- 摘要描述：迁移时原来只有目标 GPU 获得新映射，其他 GPU 后续会 fault；该工作把新映射搭载在已有失效广播中传播，使用迁移在途跟踪器暂存翻译请求，复制完成后再释放。
- 审核影响（根推断）：单纯“迁移时推送新映射”“把失效与映射更新合并”“用在途跟踪器保证复制完成前不访问”已经有高度直接的先行工作。候选仍须比较具体一致性模型、设备范围与成本，而不能因地图未收录就认定新颖。

## DATE 2025 RISC-V IOMMU SVA：普通 LLC 是必须考虑的强对手

- [作者预印本全文](https://arxiv.org/html/2502.17398v1)，[作者/平台海报](https://pulp-platform.org/docs/date2025/Cyril_Koenig_IOMMU_DATE_poster.pdf)。题名 Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs。
- 根已读摘要、引言和 §III-A，并定位 §IV-C；该范围不构成复现或完整性能审计。
- 工作在 FPGA 上评估 CVA6/加速器 SVA，基线配置仅四个 IOTLB 项；加入共享 LLC 后，所测 gemm 的翻译开销大幅减小，并检查来自主机的合成并发流量。
- 审核影响（根推断）：不能将无页表缓存/无LLC条件下暴露的开销直接当普遍应用痛点。必须保留暖页表、LLC、合理预取/批量/大页等适用普通优化；该结果也不证明所有多设备/多租户负载的翻译干扰都已解决。

## ASM（MICRO 2015）：联合 cache 与 memory 的减速建模不是空白

- [作者原始论文](https://users.ece.cmu.edu/~omutlu/pub/application-slowdown-model_micro15.pdf)，[作者实验程序](https://github.com/CMU-SAFARI/Cache-Memory-Hog)。题名 The Application Slowdown Model: Quantifying and Controlling the Impact of Inter-Application Interference at Shared Caches and Main Memory。
- 根已核原始摘要和作者 artifact 说明；C2 正在独立读相关原文。ASM 研究共享缓存与主存对应用减速的联合影响。
- 审核影响（根推断）：仅提出“跨层联合建模”“按进度而非命中率分配资源”不足以证明创新，需要落到传统模型失效的具体结构、预测失败及必要新增信息。

## 审核准则

这些文献是强反例入口，不等于否定所有邻近创新。请逐项区分：已覆盖、尚不确定、真实新差别及其代价。不得用未读全文证明别人没有做某件事。若无法建立真实应用和强基线上的剩余收益，先淘汰具体提案；不由此推断整个研究领域没有价值。
