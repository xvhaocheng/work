# PrincetonUniversity/hypersio〔正式题名待整理〕

稳定ID：`code-PrincetonUniversity-hypersio` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/PrincetonUniversity/hypersio)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-literature-review-20261004.md:26](../../iommu-platform/docs/research-literature-review-20261004.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-literature-review-20261004.md:26](../../iommu-platform/docs/research-literature-review-20261004.md)：

| HyperTRIO: Hyper-Tenant Translation of I/O Addresses（Lavrov, Wentzlaff；2020；ISCA） | https://par.nsf.gov/servlets/purl/10213947；作者仓库 https://github.com/PrincetonUniversity/hypersio | `out/method88-literature-20261004/supplement.json`（NSF 搜索片段 + 作者仓库）；`out/method88-literature-20261004/supplement.json`（直开失败） | 有限可用片段：分区 DevTLB 与 L[1-4]TLB；Pending Translation Buffer；为避免 IOMMU 二维页表走的队头阻塞，PTB 支持乱序翻译完成；作者自述在多层翻译结构分区方面作了尝试；相关工作段记录 eviction policy 不改变 IOTLB miss/hit ratio、相邻页 prefetch、page coloring 隔离 | **全文直接访问失败**：`https://parallel.princeton.edu/papers/ISCA20-hypertrio.pdf` 返回不可访问（`out/method88-literature-20261004/supplement.json` L0）；`par.nsf.gov/servlets/purl/10213947` 直开与 find 均返回 Internal Error（`out/method88-literature-20261004/supplement.json` 末尾，view0/view1/view2） | 重叠：HOL 阻塞与 2D page walk、IOMMU 翻译结构分区。**逐页证据不可得，不得伪造逐页已核对**；只按片段与作者仓库作有限引用 |

[返回总目录](../README.md)
