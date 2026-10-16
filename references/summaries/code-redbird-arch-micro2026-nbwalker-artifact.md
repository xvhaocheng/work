# redbird-arch/micro2026-nbwalker-artifact〔正式题名待整理〕

稳定ID：`code-redbird-arch-micro2026-nbwalker-artifact` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/redbird-arch/micro2026-nbwalker-artifact)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/composition-history-isolation-20261007.md:153](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md:133](../../iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md)
- [iommu-platform/out/translation-sharing-progress-20261008/map-critical-agent.md:36](../../iommu-platform/out/translation-sharing-progress-20261008/map-critical-agent.md)
- [iommu-platform/out/translation-sharing-progress-20261008/rules-critical-agent.md:75](../../iommu-platform/out/translation-sharing-progress-20261008/rules-critical-agent.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/composition-history-isolation-20261007.md:153](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

最新近邻：[NB-Walker: Non-blocking Page Table Walker to Enhance Address Translation in NUMA GPUs](https://www.microarch.org/micro59/program/)已列入MICRO 2026正式议程，本次另核到[作者公开artifact](https://github.com/redbird-arch/micro2026-nbwalker-artifact)。本地此前只有题录；这次也未获得论文全文，README不足以判定其与N的机制关系或性能。不能根据“non-blocking”题名声称我们的N相同或不同。另有同场次contention-aware translation offloading题目，仅作待核线索，未纳入已读机制对照。

[iommu-platform/out/translation-sharing-progress-20261008/map-critical-agent.md:36](../../iommu-platform/out/translation-sharing-progress-20261008/map-critical-agent.md)：

作者公开 artifact 标题 **NB-Walker: Non-blocking Page Table Walker to Enhance Address Translation in NUMA GPUs**；[官方 Zenodo v2，2026-07-22](https://zenodo.org/records/21488827)，DOI `10.5281/zenodo.21488827`。原文件已在 §40 取得。setup 指定 NB 分支 commit `8837da7dda5591837877d20d4ad51b042c988aad`。[作者仓库](https://github.com/redbird-arch/micro2026-nbwalker-artifact)，本次副本用该既有作者 repository 的只读 `git show` 取得，没有执行 setup/build/run。网页 raw 在本轮返回 cache miss，不能说本轮重新下载成功；本地精确文件和 SHA 可直接核。

[iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md:133](../../iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md)：

| NB-Walker, MICRO 2026 | [作者页面](https://maxkev1n.github.io/publications/micro-2026/)；[作者artifact仓库](https://github.com/redbird-arch/micro2026-nbwalker-artifact) README；[Zenodo作者artifact](https://zenodo.org/records/21488605) | 作者/代码发布入口能确认题目与NUMA GPU nonblocking PTW议题；公开作者页面无正文/PDF链接，repo可见README/config/script不足以核完整核心机制。因此只列关键未闭合前作，不凭名称宣布它覆盖E/F，也不伪造论文内容。需要root若推进walker类论文主贡献时定向取得正文/核心代码；本轮不扩大下载/复现 |

[iommu-platform/out/translation-sharing-progress-20261008/rules-critical-agent.md:75](../../iommu-platform/out/translation-sharing-progress-20261008/rules-critical-agent.md)：

固定作者源码commit `8837da7dda5591837877d20d4ad51b042c988aad`：[作者仓库](https://github.com/redbird-arch/micro2026-nbwalker-artifact)、[作者发布artifact v2](https://zenodo.org/records/21488827)。只核所列源码，未取得论文全文，不能声称作者证明本文nested fault或per-DMA fairness。

[返回总目录](../README.md)
