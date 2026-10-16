# Translation Caching: Skip, Don't Walk (the Page Table)

稳定ID：`translation_cache2010` · 类型：`research_paper` · 主题：`foundations`

作者：Thomas W. Barr; Alan L. Cox; Scott Rixner；年份：2010；出处：ISCA'10 (Saint-Malo, France)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：MMU cache设计空间（page table cache vs translation cache；unified/split/path）；提出UPTC/SPTC/UTC/STC/TPC；最优为unified translation cache + 变插入点LRU(VI-LRU)。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：x86-64四级radix page table；trace不含instruction/page table loads；4KB页；L2 data cache 1MB；TLB配置近似Opteron；页表一致性由软件维护

限制：trace-driven模拟，无实机；未涉及多核共享/IOMMU/设备/SVA；未讨论安全

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/translation_cache2010.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/translation_cache2010.pdf) · SHA-256 `a52aca9dea3052d93351fc9525642847a960318aab2cb84d7582cfdca5e45ed7`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:19607](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:14](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/mechanism-design-handoff-20261009-round04.md:49](../../iommu-platform/docs/mechanism-design-handoff-20261009-round04.md)
- [iommu-platform/docs/mechanism-design-handoff-20261009-round05.md:25](../../iommu-platform/docs/mechanism-design-handoff-20261009-round05.md)
- [iommu-platform/docs/research-landscape-20261005.md:26](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:67](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-route-audit-20261009.md:50](../../iommu-platform/docs/research-route-audit-20261009.md)
- [iommu-platform/docs/research-route-audit-20261010-supplement.md:50](../../iommu-platform/docs/research-route-audit-20261010-supplement.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/mechanism-design-handoff-20261009-round05.md:25](../../iommu-platform/docs/mechanism-design-handoff-20261009-round05.md)：

本稿为 DeepSeek 起草的普通中文记录，未执行命令、未修改文件、未运行测试；Codex root 亲自设计、关键核查与验收并保存，1 个关键 Codex 席。证据：裁决、L-M 控制、L-W 控制、round04 机制卡。前作组件范围：[ICCD 2018，DOI 10.1109/ICCD.2018.00052](https://arxiv.org/pdf/1808.09751)（锁翻译后再编程 DMA、并发成本；支持 pin 组件旧，不覆盖精确 set-valued 合同）；[Barr 等，ISCA 2010，DOI 10.1145/1815961.1815970](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)（虚拟前缀部分翻译跳多级）；[US9542332B2](https://patents.google.com/patent/US9542332B2/en)（高优先级需求中断低优先级准备）。以上只支持组件/边界，不宣称全文合同已覆盖。

[iommu-platform/docs/research-landscape-20261005.md:26](../../iommu-platform/docs/research-landscape-20261005.md)：

第一阶段是经典缓存、覆盖与访存解耦：[DAE](https://people.eecs.berkeley.edu/~kubitron/courses/cs252-F00/handouts/papers/p231-smith.pdf) 把地址计算/访存与执行解耦，用队列隐藏延迟；[Translation Caching](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf) 说明缓存部分翻译可跳过层级，TLB miss 不应默认等于多次未缓存 DRAM 访问；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 用连续物理区域绕过 TLB。已解决的是：在给定页表、缓存和直接段条件下，翻译覆盖与延迟有明确优化空间。新条件使原假设吃紧：GPU/IOMMU/ATS 把翻译推到多设备、多级共享结构，缓存覆盖率不再单独决定结果。

[iommu-platform/docs/research-route-audit-20261009.md:50](../../iommu-platform/docs/research-route-audit-20261009.md)：

两项比较是纸面构造和手推，不能改称已运行模型。根接受记录与关键控制原件见round05。独立关键审查核对了[ICCD2018](https://arxiv.org/pdf/1808.09751)、[Barr ISCA2010](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)和[需求优先取消专利](https://patents.google.com/patent/US9542332B2/en)：它们支持pin、部分翻译缓存及取消组件已有前史，不等于证明本项目整个集合COPY合同已被发表。

[iommu-platform/docs/research-route-audit-20261010-supplement.md:50](../../iommu-platform/docs/research-route-audit-20261010-supplement.md)：

两项比较是纸面构造和手推，不能改称已运行模型。根接受记录与关键控制原件见round05。独立关键审查核对了[ICCD2018](https://arxiv.org/pdf/1808.09751)、[Barr ISCA2010](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)和[需求优先取消专利](https://patents.google.com/patent/US9542332B2/en)：它们支持pin、部分翻译缓存及取消组件已有前史，不等于证明本项目整个集合COPY合同已被发表。

[返回总目录](../README.md)
