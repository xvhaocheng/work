# Intel pending-read merge，US20200402198A1〔正式题名待整理〕

稳定ID：`ref-d7e117ffc557` · 类型：`patent` · 主题：`patents`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：patents资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://patents.google.com/patent/US20200402198A1/en)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-morning-20261009.md:29](../../iommu-platform/docs/research-morning-20261009.md)
- [iommu-platform/out/parallel-innovation-model-20261009/a-codex-cost-rule.md:23](../../iommu-platform/out/parallel-innovation-model-20261009/a-codex-cost-rule.md)
- [iommu-platform/out/shared-read-inflight-join-20261009/codex-design-contract.md:23](../../iommu-platform/out/shared-read-inflight-join-20261009/codex-design-contract.md)
- [iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md:45](../../iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md)
- [iommu-platform/out/shared-read-short-prefix-20261009/codex-mechanism-verdict.md:25](../../iommu-platform/out/shared-read-short-prefix-20261009/codex-mechanism-verdict.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-morning-20261009.md:29](../../iommu-platform/docs/research-morning-20261009.md)：

[DMA-Latte v3](https://arxiv.org/html/2511.06605v3)已支持一次源读取送两个目的，并讨论广播/并行选择；[Intel请求合并专利](https://patents.google.com/patent/US20200402198A1/en)已描述pending scoreboard、动态请求窗口与multicast返回。它背景提到IOMMU，不能宣称前作完全没有翻译。

[iommu-platform/out/parallel-innovation-model-20261009/a-codex-cost-rule.md:23](../../iommu-platform/out/parallel-innovation-model-20261009/a-codex-cost-rule.md)：

| [Intel pending-read merge，US20200402198A1](https://patents.google.com/patent/US20200402198A1/en)，pending scoreboard／return mask 段及 shared SLM、multicast buffer 段 | 待返回块地址、请求／线程／接收者身份；回包接收者掩码；共享数据缓冲及接收者 tag FIFO，或组合 multicast entry 与剩余接收计数 | 同块在途合并、等待窗口、一次数据向多人发送、单份数据保留到全部已登记接收者消费都已描述。不能只把「共同数据＋掩码」重新命名。scoreboard 返还后释放，不等于所有共享缓冲当时也释放。原文没有在已读段落证明独立 IOMMU 晚许可与各自 W 债同一合同。 |

[iommu-platform/out/shared-read-inflight-join-20261009/codex-design-contract.md:23](../../iommu-platform/out/shared-read-inflight-join-20261009/codex-design-contract.md)：

[Intel pending-read merge/multicast](https://patents.google.com/patent/US20200402198A1/en) 已有 outstanding request 合并、动态等待窗口和 return mask；[DMA-Latte v3](https://arxiv.org/html/2511.06605v3) §IV-B 已有一次 source READ 给两个目的的广播 DMA。仅“读一次/动态合读”不能当新贡献。

[iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md:45](../../iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md)：

[Intel pending-read merge/multicast](https://patents.google.com/patent/US20200402198A1/en) 已有pending合并/动态等待窗/返回mask；[DMA-Latte v3](https://arxiv.org/html/2511.06605v3) §IV-B已有一读两写的broadcast。因此，因为当前普通策略更好且原则已有前作，这组性能不能称独立算法贡献。

[返回总目录](../README.md)
