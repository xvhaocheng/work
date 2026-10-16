# Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs

稳定ID：`barre2024` · 类型：`research_paper` · 主题：`multigpu`

作者：Yuan Feng; Seonjin Na; Hyesoon Kim; Hyeran Jeon；年份：2024；出处：2024 ACM/IEEE 51st Annual International Symposium on Computer Architecture (ISCA)。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：把同一数据跨 GPU chiplet 映射到相同 local PFN，组成 coalescing group；在 PTE 中编码 coalescing bitmap/order，由 IOMMU 的 PEC 逻辑在一次翻译后计算同组其他页 PFN；F-Barre 用 LCF/RCF Cuckoo filters 在 MCM 内共享 coalescing 信息并合并组，减少 IOMMU 访问。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：4-chiplet MCM GPU，IOMMU 在 CPU 侧，GPU chiplet 经 PCIe 发 ATS，IOMMU 16 PTW/48-entry PW-queue，LASP 页映射（L34-L58, L139-L160, L254-L266, L310-L312, L994-L1015）。；coalescing group 要求同一数据在各 GPU 有 commonly available local PFN；无共同页则回退普通映射（L475-L497, L653-L671）。；PTE unused bits 可用；支持最多 8 GPUs；contiguity-aware 最多 4 GPUs（L499-L507, L857-L859）。；主评估不做 on-demand paging/migration；讨论可集成，migration 时把页从 coalescing group 移除（L286-L290, L824-L863）。

限制：仅 MGPUSim 周期级模拟与 CACTI 面积估算，无硬件原型/实机（L310-L312, L1789-L1795）。；不支持 kernel 执行期间 dynamically allocated data（L824-L838）。；依赖共同 local PFN；不满足则无法 coalesce；组大小受 PTE 位宽和 GPU 数限制（L857-L859）。；F-Barre peer filter best-effort；RCF remote hit 75.3%，LCF 98.4%，更新可能丢失（L1331-L1338）。；migration 会打破 coalescing group；大页 64KB/2MB 收益缩小，16x 输入下 2MB 仅 2%（L1617-L1625, L1652-L1659）。；round-robin 映射下 1.25x，多应用平均 17%（L1755-L1780）；未评估安全/权限隔离与尾延迟。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：multigpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://seonjinna.github.io/assets/pdf/BarreChord_isca24.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/barre2024.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.pdf) · SHA-256 `027228e356ac8a2a62d58337f148d46d6fba4f2c9ace7402572be09083fa3695`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:436](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt:6](../../iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt:65](../../iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt:319](../../iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt:475](../../iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt:653](../../iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt:824](../../iommu-platform/out/shared-translation-literature-20261006/papers/barre2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:213](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:229](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 barre2024：PDF papers/barre2024.pdf; text papers/barre2024.txt; 报告 workers/G2_multigpu.md; 证据卡 workers/G2_multigpu-papers.json；reading=fulltext_worker_reviewed；status=published；year=2024；venue=2024 ACM/IEEE 51st Annual International Symposium on Computer Architecture (ISCA)；doi=10.1109/ISCA59077.2024.00065；source=https://seonjinna.github.io/assets/pdf/BarreChord_isca24.pdf；root_spotchecked=True

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:162](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 barre2024：PDF papers/barre2024.pdf; text papers/barre2024.txt; 报告 workers/G2_multigpu.md; 证据卡 workers/G2_multigpu-papers.json；reading=fulltext_worker_reviewed；status=published；year=2024；venue=2024 ACM/IEEE 51st Annual International Symposium on Computer Architecture (ISCA)；doi=10.1109/ISCA59077.2024.00065；source=https://seonjinna.github.io/assets/pdf/BarreChord_isca24.pdf；root_spotchecked=True

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:228](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **barre2024** Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs — Yuan Feng, Seonjin Na, Hyesoon Kim, Hyeran Jeon；2024 ACM/IEEE 51st Annual International Symposium on Computer Architecture (ISCA)；10.1109/ISCA59077.2024.00065；source=https://seonjinna.github.io/assets/pdf/BarreChord_isca24.pdf；topic=multigpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:161](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **barre2024** Barre Chord: Efficient Virtual Memory Translation for Multi-Chip-Module GPUs — Yuan Feng, Seonjin Na, Hyesoon Kim, Hyeran Jeon；2024 ACM/IEEE 51st Annual International Symposium on Computer Architecture (ISCA)；10.1109/ISCA59077.2024.00065；source=https://seonjinna.github.io/assets/pdf/BarreChord_isca24.pdf；topic=multigpu；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知

[返回总目录](../README.md)
