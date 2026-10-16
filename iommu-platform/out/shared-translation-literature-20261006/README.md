# 共享地址翻译文献库（2026-10-06）

**123项独立工作：104项取得全文，105个全文版本；19项仅题录、摘要或索引片段。** STAR的两份版本合并计数。本库经典扩展至1989年，前沿检索到2026-10-06。不是宣称穷尽整个领域，也不是123项都经过同样深度的人工审阅。

- [剩余覆盖与收敛：应用路径、软件语义、4篇新全文](gap-audit-03/README.md)（独立补充来源，非全部精读）
- [后续缺口审计：20项优先补充、16项新全文](gap-audit-02/README.md)（独立审计批，尚未全部完成全文卡，不计入上方123项）
- [本轮深化综述：七条脉络与强前作对照](../../docs/shared-translation-deepening-20261006.md) · [增补来源与调用记录](expansion-01/README.md)
- [研究地图与领域瓶颈](../../docs/shared-translation-literature-20261006.md)
- [全部论文索引](INDEX.md)：按主题、年份查找，直接打开本地PDF或阅读证据卡。
- [CSV](catalog.csv)：可筛选年份、主题、相关性、出版状态与全文可得性。
- [BibTeX](references.bib)：一项独立工作一条，首选正式版本；未知字段省略，未虚构DOI。
- [JSON完整目录](catalog.json) · [统计](statistics.json) · [目录检查](catalog-checks.json)
- [根审阅与纠正](ROOT_REVIEW.md) · [首轮调用记录](audit.json) · [增补轮审计](expansion-01/audit.json)

## 阅读深度

`fulltext_worker_reviewed`表示已取得全文，由DeepSeek按主题提取完整证据卡；`root_spotchecked`另标Codex直接核查的关键内容，不意味着该篇每一句都已人工核验。题录项区分`metadata_only`和`abstract_or_indexed_excerpt`。论文的`published`、`preprint`、`accepted_upcoming`状态独立保存。

所有原始PDF在[papers/](papers/)。文本是供检索、模型阅读的提取版本，保留原PDF供核对图表、双栏和公式。NUL清除及来源哈希见下载记录；个别特殊字符或字体提取不完整时，以PDF为准。CoLT从公开MICRO2012论文集的PDF第278—289页提取，原件与来源记录保留。

## 分组阅读报告

| 组 | 覆盖内容 | 原始报告 |
|---|---|---|
| S0 | 范围、引用链与收集缺口 | [完整答复](workers/S0.md)；部分计数／分级意见已由ROOT_REVIEW纠正 |
| G1 | GPU翻译、并发、预取、调度 | [9个版本](workers/G1_gpu.md) |
| G2 | 多GPU、chiplet和晶圆级翻译 | [6篇](workers/G2_multigpu.md) |
| G3 | I/O吞吐、严格保护、干扰 | [6篇](workers/G3_io.md) |
| G4 | 异构加速器、SVA、DMA | [8篇](workers/G4_accelerators.md) |
| G5 | 共享TLB经典、覆盖与页表放置近邻 | [10篇](workers/G5_foundations.md) |
| G6 | 一致性、迁移、失效、观测 | [13篇](workers/G6_lifecycle.md) |
| G7 | GPU MMU基础、Avatar、cuPTW、STAR正式版等 | [6个版本](workers/G7_supplement.md) |
| G8 | GRIT动态页放置与CoLT基础 | [2篇](workers/G8_grit_colt.md) |
| G10 | 近内存、受限映射、FAM与解耦内存 | [8篇新增全文](workers/G10_nearmemory.md) |
| G11 | CLAP、SoftWalker、FS-HPT、OASIS与LAEGIS | [5篇新增全文；引用关系已修正](workers/G11_gpu_expansion.md) |
| G12 | 经典shootdown、NUMA更新、分配与回收 | [6篇新增全文](workers/G12_update.md) |
| G13 | 分层内存、关键性与GPU策略接口 | [7篇新增全文](workers/G13_tiering.md) |
| G14 | 连续性、页表表示与OS翻译框架 | [8篇新增全文](workers/G14_structure.md) |
| G15 | 网络IOMMU、RDMA ODP与远端分页 | [5篇新增全文](workers/G15_io_expansion.md) |
| G16 | IOMMU经典、CXL NDP与翻译次序 | [6篇新增全文](workers/G16_semantics.md) |

原始JSON包含实际模型、用量、截断状态及预算；任务与输入文件列表一并保留。所有worker都是外部DeepSeek文本服务，没有执行命令、修改文件、浏览或复现实验。Codex执行检索、下载、提取、工具应用和检查；本专题没有调用Claude或新增Codex子agent。

## 来源与版本处理

`records/web-search-*`及`web-metadata-*`保存检索／打开结果；`index-pages*`和作者页面快照保存原始来源。只将作者、论文原文、机构、会议或出版社作为科学事实的依据；搜索结果里的聚合网站仅用于线索，不将其自动作为主证据。

`download-batch*.json`保留成功和失败，404／403／500不伪装成已取得正文，也没有使用账号或绕过访问限制。后续成功不会删除早先失败。`metadata-only.json`明确19项未全文阅读的来源和限制。

`root-overrides.json`记录经原始来源补核的出版信息；`work-families.json`保留版本归并关系。原始API答复JSON不覆盖；派生报告的局部更正另留历史和说明，以便看到“输入正文未印会议信息”与“后来从大会官网补核”的区别。

目录工具只作用于本证据包。增补轮沿用该工具，仅将分组文件匹配扩展到两位组号。第一次通用化草案未采用，修订后的离线脚本经Codex审阅后使用。目录/文件检查与论文实验验证严格分开；无生产源码、RTL、Linux、构建、上板或SD操作。
