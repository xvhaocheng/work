# A Multicast-Capable AXI Crossbar for Many-core Machine Learning Accelerators，2025〔正式题名待整理〕

稳定ID：`arxiv-2502.19215` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/html/2502.19215v1)
- [来源2](https://arxiv.org/html/2502.19215v1#S2)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-morning-20261009.md:31](../../iommu-platform/docs/research-morning-20261009.md)
- [iommu-platform/docs/shared-read-rtl-progress-20261009.md:34](../../iommu-platform/docs/shared-read-rtl-progress-20261009.md)
- [iommu-platform/out/innovation-rescue-20261009/codex-cross-field-boundary.md:8](../../iommu-platform/out/innovation-rescue-20261009/codex-cross-field-boundary.md)
- [iommu-platform/out/innovation-rescue-20261009/codex-primary-review.md:7](../../iommu-platform/out/innovation-rescue-20261009/codex-primary-review.md)
- [iommu-platform/out/innovation-rescue-20261009/rules-design.md:170](../../iommu-platform/out/innovation-rescue-20261009/rules-design.md)
- [iommu-platform/out/innovation-rescue-20261009/rules-design.md:176](../../iommu-platform/out/innovation-rescue-20261009/rules-design.md)
- [iommu-platform/out/parallel-innovation-model-20261009/a-codex-cost-rule.md:26](../../iommu-platform/out/parallel-innovation-model-20261009/a-codex-cost-rule.md)
- [iommu-platform/out/shared-read-frontier-20261009/critical-translation-frontier-design.md:123](../../iommu-platform/out/shared-read-frontier-20261009/critical-translation-frontier-design.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/shared-read-rtl-progress-20261009.md:34](../../iommu-platform/docs/shared-read-rtl-progress-20261009.md)：

[DMA-Latte v3](https://arxiv.org/html/2511.06605v3) §IV-B/VI 已有同源识别与一次读多写；[Multicast AXI XBAR](https://arxiv.org/html/2502.19215v1) §II-A 已有有限缓存下的 W 顺序死锁与 acquire/commit。因此不能把共享读、一般死锁修正或34%直接称为独立创新。如果在同信息、同总成本、同暂停义务下，更强普通分片控制仍不能同时获得此进度与效率，且前作没有覆盖具体控制，就可以论证独立微架构贡献；如果普通控制同效同成本，就不能将当前实现命名为新算法。

[iommu-platform/docs/research-morning-20261009.md:31](../../iommu-platform/docs/research-morning-20261009.md)：

[Torrent](https://arxiv.org/html/2512.17589v1)有grant/data/finish阶段；[AXI multicast crossbar](https://arxiv.org/html/2502.19215v1)处理AW/W/B、多播前排空相关单播。已读正文没有给出本原型完整的独立权限、长READ、logical LAST、旧W/B义务切换合同；这是核查边界，不是证明所有已有研究都没有。

[iommu-platform/out/innovation-rescue-20261009/rules-design.md:170](../../iommu-platform/out/innovation-rescue-20261009/rules-design.md)：

| [A Multicast-Capable AXI Crossbar for Many-core Machine Learning Accelerators，2025](https://arxiv.org/html/2502.19215v1#S2) | II-A，web L105–124：多目的W的B join；有限Wbuffer下不同slave相反AW顺序可死锁；以一致选择/all-slave acquire和aw.commit保证。 | “finite buffer + multicast + commit避免死锁”已有直接先例，不能泛立新。其机制是写multi-slave acquisition；本平台不是制造相反AW顺序，目标是串行owner下两独立返R和成员退出。仍允许ordinary采用同样acquire/commit能力。 |

[iommu-platform/out/shared-read-rtl-20261009/codex-result-acceptance.md:27](../../iommu-platform/out/shared-read-rtl-20261009/codex-result-acceptance.md)：

本轮直接复核 [DMA-Latte v3 §IV-B/VI](https://arxiv.org/html/2511.06605v3)：源地址/size 识别 broadcast、一次读多写已明确已有。复核 [Multicast AXI XBAR §II-A](https://arxiv.org/html/2502.19215v1)：finite-buffer W 顺序死锁和 acquire/commit 已有；该文讨论的是 multicast AW/W/B，并不由此证明其覆盖我们的 READ/TL/early-resume 组织，也不能仅靠接口不同认定新颖。

[返回总目录](../README.md)
