# DMA-Latte: Expanding the Reach of DMA Offloads to Latency-bound ML Communication，v3 2026-09-14〔正式题名待整理〕

稳定ID：`arxiv-2511.06605` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/html/2511.06605v3)
- [来源2](https://arxiv.org/html/2511.06605v3#S6)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-morning-20261009.md:29](../../iommu-platform/docs/research-morning-20261009.md)
- [iommu-platform/docs/shared-read-rtl-progress-20261009.md:34](../../iommu-platform/docs/shared-read-rtl-progress-20261009.md)
- [iommu-platform/out/innovation-rescue-20261009/codex-cross-field-boundary.md:7](../../iommu-platform/out/innovation-rescue-20261009/codex-cross-field-boundary.md)
- [iommu-platform/out/innovation-rescue-20261009/codex-primary-review.md:19](../../iommu-platform/out/innovation-rescue-20261009/codex-primary-review.md)
- [iommu-platform/out/innovation-rescue-20261009/rules-design.md:172](../../iommu-platform/out/innovation-rescue-20261009/rules-design.md)
- [iommu-platform/out/innovation-rescue-20261009/rules-design.md:178](../../iommu-platform/out/innovation-rescue-20261009/rules-design.md)
- [iommu-platform/out/shared-read-frontier-20261009/critical-translation-frontier-design.md:122](../../iommu-platform/out/shared-read-frontier-20261009/critical-translation-frontier-design.md)
- [iommu-platform/out/shared-read-inflight-join-20261009/codex-design-contract.md:23](../../iommu-platform/out/shared-read-inflight-join-20261009/codex-design-contract.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/shared-read-rtl-progress-20261009.md:34](../../iommu-platform/docs/shared-read-rtl-progress-20261009.md)：

[DMA-Latte v3](https://arxiv.org/html/2511.06605v3) §IV-B/VI 已有同源识别与一次读多写；[Multicast AXI XBAR](https://arxiv.org/html/2502.19215v1) §II-A 已有有限缓存下的 W 顺序死锁与 acquire/commit。因此不能把共享读、一般死锁修正或34%直接称为独立创新。如果在同信息、同总成本、同暂停义务下，更强普通分片控制仍不能同时获得此进度与效率，且前作没有覆盖具体控制，就可以论证独立微架构贡献；如果普通控制同效同成本，就不能将当前实现命名为新算法。

[iommu-platform/docs/research-morning-20261009.md:29](../../iommu-platform/docs/research-morning-20261009.md)：

[DMA-Latte v3](https://arxiv.org/html/2511.06605v3)已支持一次源读取送两个目的，并讨论广播/并行选择；[Intel请求合并专利](https://patents.google.com/patent/US20200402198A1/en)已描述pending scoreboard、动态请求窗口与multicast返回。它背景提到IOMMU，不能宣称前作完全没有翻译。

[iommu-platform/out/innovation-rescue-20261009/rules-design.md:172](../../iommu-platform/out/innovation-rescue-20261009/rules-design.md)：

| [DMA-Latte: Expanding the Reach of DMA Offloads to Latency-bound ML Communication，v3 2026-09-14](https://arxiv.org/html/2511.06605v3#S6) | IV-B L179–182：MI300X bcst一source/two destinations、source只读一次；VI L308：runtime检查hipMemcpyBatchAsync的source/size透明推断broadcast；V-B5–8实际比较bcst/b2b/pcpy/size范围。 | 独立copy自动归broadcast以及减少sourceR均已覆盖；不能靠显式广播命令较弱或v1缺少runtime段立差异。候选必须比较成员暂停、已有firstVALID保持、logical parent精确补读和work界；性能对照允许size-based broadcast/b2b/pcpy选优。 |

[iommu-platform/out/shared-read-rtl-20261009/codex-result-acceptance.md:27](../../iommu-platform/out/shared-read-rtl-20261009/codex-result-acceptance.md)：

本轮直接复核 [DMA-Latte v3 §IV-B/VI](https://arxiv.org/html/2511.06605v3)：源地址/size 识别 broadcast、一次读多写已明确已有。复核 [Multicast AXI XBAR §II-A](https://arxiv.org/html/2502.19215v1)：finite-buffer W 顺序死锁和 acquire/commit 已有；该文讨论的是 multicast AW/W/B，并不由此证明其覆盖我们的 READ/TL/early-resume 组织，也不能仅靠接口不同认定新颖。

[返回总目录](../README.md)
