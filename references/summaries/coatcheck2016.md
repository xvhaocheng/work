# COATCheck: Verifying Memory Ordering at the Hardware-OS Interface

稳定ID：`coatcheck2016` · 类型：`research_paper` · 主题：`lifecycle`

作者：Daniel Lustig; Geet Sethi; Margaret Martonosi; Abhishek Bhattacharjee；年份：2016；出处：ASPLOS '16, April 2-6, 2016, Atlanta, Georgia, USA。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：用 mu-spec DSL、mu-hb 图和约束求解器，将 OS syscall、IPI TLB shootdown、页表遍历微码、TLB occupancy、synonym 与 PTE status bits 纳入地址翻译感知的内存次序验证。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：目标硬件模型为 Sandy Bridge-like，基于公开文档、专利与合理猜测，不是精确商业实现；Linux synopsis 只建模与分页结构和同步相关的内存访问，不建模完整 OS 决策/数据结构；x86-style 页表遍历由硬件执行，TLB coherence 由 OS 通过 invlpg+IPI 管理；mu-spec 模型由硬件设计者提供，OS synopsis 由 OS 知识者提供；Coq 形式证明尚未完成；litmus tests 与 ELT 需覆盖虚拟地址、物理地址、PTE 状态与 ghost instructions

限制：是静态验证工具，不是运行时 IOMMU/SVA 系统，也不是攻击/安全论文；未验证商业 RTL，未自动从 Verilog/HDL 抽取 mu-spec；未完成 Coq 形式正确性证明；不覆盖 IOMMU IOTLB、设备 ATC、PASID、多设备共享翻译域；不给出共享 IOMMU 的 performance/tail latency 数据；OS synopsis 不建模完整 OS 行为，只包含与分页和同步相关的内存访问

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[m2ndp2024](m2ndp2024.md)：COATCheck 管 CPU/OS 翻译次序正确性；M2NDP 管 CXL NDP 性能与设备翻译 reach，二者覆盖不同正确性/性能维度。；[prefetched2019](prefetched2019.md)：COATCheck 提醒 TLB prefetch/coalescing/shootdown 必须保持 ordering；2019 的 ASAP 用完整 walk 校验预取，方向一致但对象是 CPU page walk。

原始来源：

- [来源1](https://check.cs.princeton.edu/papers/dlustig_ASPLOS16.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/coatcheck2016.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.pdf) · SHA-256 `9ec2403bb5610deeb8522002750ccf52a7fb2b8c711f51a8342de40ff3e9dac0`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:2167](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt:22](../../iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt:100](../../iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt:130](../../iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt:211](../../iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt:519](../../iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt:762](../../iommu-platform/out/shared-translation-literature-20261006/papers/coatcheck2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:398](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:437](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 coatcheck2016：PDF papers/coatcheck2016.pdf; text papers/coatcheck2016.txt; 报告 workers/G16_semantics.md; 证据卡 workers/G16_semantics-papers.json；reading=fulltext_worker_reviewed；status=published；year=2016；venue=ASPLOS '16, April 2-6, 2016, Atlanta, Georgia, USA；doi=10.1145/2872362.2872399；source=https://check.cs.princeton.edu/papers/dlustig_ASPLOS16.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:436](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **coatcheck2016** COATCheck: Verifying Memory Ordering at the Hardware-OS Interface — Daniel Lustig, Geet Sethi, Margaret Martonosi, Abhishek Bhattacharjee；ASPLOS '16, April 2-6, 2016, Atlanta, Georgia, USA；10.1145/2872362.2872399；source=https://check.cs.princeton.edu/papers/dlustig_ASPLOS16.pdf；topic=lifecycle；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:398](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2016 / published | <a id="coatcheck2016"></a>[COATCheck: Verifying Memory Ordering at the Hardware-OS Interface](https://check.cs.princeton.edu/papers/dlustig_ASPLOS16.pdf) | coatcheck2016 | coatcheck2016 | foundation |

[返回总目录](../README.md)
