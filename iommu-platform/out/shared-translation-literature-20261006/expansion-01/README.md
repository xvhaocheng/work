# 共享地址翻译文献增补与深化（2026-10-06）

本轮将专题从72项扩展到**123项独立工作**：104项取得全文，共105个版本；19项为题录/摘要。新增51项独立工作（43项全文、8项题录）；另为旧题录CLAP和Translation Ranger补齐全文。基础文献扩展至1989年，最新窗口包含2026年已发表、已接收及预印本，状态分开记录。

- [深化综述：七条脉络、六组矛盾、20篇必读](../../../docs/shared-translation-deepening-20261006.md)
- [完整索引](../INDEX.md) · [CSV](../catalog.csv) · [BibTeX](../references.bib)
- [关键原文审定与纠正](ROOT_REVIEW.md) · [实际调用和来源审计](audit.json) · [本轮导航验收](records/delivery-checks.json)
- [72项基线快照](baseline/INDEX.md)：仅用于计数与内容历史；其中原相对链接应从旧目录位置理解，不作为当前导航入口。

## 增量来源

本轮增加45份全文版本：42份从作者、机构、会议等公开来源取得；3份复用此前广域地图已保存的IMPICA、OASIS、vAttention全文。不是45篇全部首次下载，也不与广域地图计数简单相加。来源、SHA256、文本提取与成功/失败记录在上一层的download-batch7至11.json。

CDFD与LIBRA的作者PDF链接返回404，保留为题录。HELM只取得出版社摘要/索引片段；SagePTE机构页面标注全文公开时间为2026-11-04。没有从这些材料推断完整协议或采用聚合网站的性能数字。LIBRA出版元数据的首次请求429，稍后一次定向请求成功；正式目录DOI据成功的出版社注册记录。1989 shootdown会议/同期刊物两个DOI只计一项工作，作者按五人原稿。

## 分组与分工

| 组 | 新增全文版本 | 范围与完整证据报告 |
|---|---:|---|
| G10 | 8 | [DIPTA、DeACT、VBI、Utopia、NDPage、DTL、IMPICA、Clio](../workers/G10_nearmemory.md) |
| G11 | 5 | [CLAP、SoftWalker、FS-HPT、LAEGIS、OASIS](../workers/G11_gpu_expansion.md) |
| G12 | 6 | [Shootdown、RadixVM、Hydra、Shoot4U、Ptlbmalloc2、FPR](../workers/G12_update.md) |
| G13 | 7 | [Memtis、Soar/Alto、Nomad、Pond、gpu_ext、vAttention、PACT](../workers/G13_tiering.md) |
| G14 | 8 | [Translation Ranger、SpecTLB、CA paging、Ingens、DPT、EMT、Midgard、Memento](../workers/G14_structure.md) |
| G15 | 5 | [DAMN、vIOMMU、RDMA ODP、TeRM、ODRP](../workers/G15_io_expansion.md) |
| G16 | 6 | [M2NDP、COATCheck、ASAP、CVA6-RT、2010/2007 IOMMU经典](../workers/G16_semantics.md) |

DeepSeek以deepseek-flash、thinking/high和393216预算完成七组全文证据提取；另完成缺口线索、下载工具提案、前沿题录、引用纠正、综述草稿与定向修订，共13次实际调用。完整输入、API返回、用量、报告均保留，未关闭思考或缩小预算。Codex执行检索、下载、文本提取、工具应用、元数据核查，并直接审定影响研究判断的原文与最终综述。本轮未调用Claude或新建Codex子agent。

## 原始答复与修订

- [E0缺口线索](workers/E0-gap.md)只用于引导检索，未经来源核定的作者/年份不导入目录。
- [Etool下载工具提案](workers/Etool.md)由Codex审阅后应用；现有目录生成器仅扩大分组文件匹配范围，未重造文献系统。
- G11原稿错误地声称cuPTW引用SoftWalker，并混淆其调度优先级；[定向纠正](workers/EfixG11.md)经原文核对后同步到派生报告和卡片。原API返回与[原始报告](baseline/G11_gpu_expansion.original.md)保留。
- [综述草稿](workers/Esynthesis.md)、[修订提案](workers/Erevise.md)、[应用记录](records/synthesis-application.json)保留。一个替换锚点只有空格不一致，经唯一匹配核对后应用。最终研究停止条件与私有ATC/共享主机服务边界由Codex审定；测得困难本身不等于应停止研究。
- 元数据提案、接受版与正式注册记录分开保存；部分作者稿页脚过时，出版状态按截至检索日的官方来源修正。全文未印DOI且未补核的字段留空。

## 验收含义

目录生成器核对全文、文本、阅读卡及版本归并；交付检查核对123条CSV/BibTeX、105份PDF和本地跳转。当前24项工作标有根关键核查，其中12项来自本次增补；此标记不表示全文逐字复核。

这些是文献与文件验收，不是论文实验复现，也未确立新颖性或性能收益。没有修改生产代码、RTL或Linux，没有新增构建、上板、写卡或硬件实验；既有172/172功能验收不变。
