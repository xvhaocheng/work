# Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine

稳定ID：`socdma2018` · 类型：`research_paper` · 主题：`accelerators`

作者：Andreas Kurth; Pirmin Vogel; Andrea Marongiu; Luca Benini；年份：2018；出处：Proceedings of the 36th IEEE International Conference on Computer Design (ICCD)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：混合IOMMU软件管理TLB；编译器生成PHT预取TLB项，并行MHT处理miss，DMA retirement buffer记录未命中burst并重发，无需大数据缓冲。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：hybrid IOMMU，软件PTW；PMCA共享TLB；编译器可生成PHT/MHT；DMA burst不跨页；L1 SPM可共享MHT状态；AXI4传输

限制：Zynq FPGA emulation，非优化硅后；仅两个基准；PHT生成算法不支持把单条访存流分给两个PHT；未讨论安全/不可信隔离；未评估TLBI等待/排空、页面迁移、A/D位；软件PTW消耗PMCA PE资源

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：accelerators主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://arxiv.org/pdf/1808.09751)
- [来源2](https://arxiv.org/abs/1808.09751)
- [来源3](https://arxiv.org/abs/1808.09751v1)
- [来源4](https://arxiv.org/html/1808.09751v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/socdma2018.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.pdf) · SHA-256 `518f3bb180de2f17a06f03c049be76fc7902513604c69644041a0971e8a7ed97`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:17145](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt:65](../../iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt:25](../../iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt:376](../../iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt:478](../../iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt:545](../../iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt)
- [iommu-platform/docs/composition-history-isolation-20261007.md:210](../../iommu-platform/docs/composition-history-isolation-20261007.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/mechanism-design-handoff-20261009-round05.md:25](../../iommu-platform/docs/mechanism-design-handoff-20261009-round05.md)：

本稿为 DeepSeek 起草的普通中文记录，未执行命令、未修改文件、未运行测试；Codex root 亲自设计、关键核查与验收并保存，1 个关键 Codex 席。证据：裁决、L-M 控制、L-W 控制、round04 机制卡。前作组件范围：[ICCD 2018，DOI 10.1109/ICCD.2018.00052](https://arxiv.org/pdf/1808.09751)（锁翻译后再编程 DMA、并发成本；支持 pin 组件旧，不覆盖精确 set-valued 合同）；[Barr 等，ISCA 2010，DOI 10.1145/1815961.1815970](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)（虚拟前缀部分翻译跳多级）；[US9542332B2](https://patents.google.com/patent/US9542332B2/en)（高优先级需求中断低优先级准备）。以上只支持组件/边界，不宣称全文合同已覆盖。

[iommu-platform/docs/composition-history-isolation-20261007.md:354](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

补核强前作：[Criticality ISCA2013](https://users.ece.cmu.edu/~saugatag/papers/13isca.pdf)、[SMS ISCA2012](https://people.inf.ethz.ch/omutlu/pub/staged-memory-scheduling_isca12.pdf)、[DASH](https://people.inf.ethz.ch/omutlu/pub/dash_deadline-aware-heterogeneous-memory-scheduler_taco16.pdf)、[MMU-aware DMA ICCD2018 §IV-C](https://arxiv.org/pdf/1808.09751)、[AXI-REALM §III-A](https://arxiv.org/pdf/2501.10161)、[Arm DMA-350 §5.8.1](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)。它们已覆盖关键性/进度、分来源队列、DMA burst身份和重试、拆分/写数据齐备、完整返回空间等构件。另核[ABW机构摘要](https://researchportal.hkust.edu.hk/en/publications/adaptive-burst-writes-abw-memory-requests-scheduling-to-reduce-wr/)的自适应 DRAM 写批次，不等于 AXI 单 burst；其正文及 NB-Walker 精确机制仍有缺口，未排除全部前作。

[iommu-platform/docs/mechanism-design-handoff-20261009-round04.md:57](../../iommu-platform/docs/mechanism-design-handoff-20261009-round04.md)：

完整强对照包括：精确 set-aware pin+bypass；普通全 WRITE prepare 并保留结果；无条件源 bypass；源/目的角色 bypass；带正确缓冲成本的复用历史 dpPred 式预测器；MASK token/选择性准入；Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine (ICCD2018), DOI 10.1109/ICCD.2018.00052, [论文原稿](https://arxiv.org/pdf/1808.09751) 的预取与加锁；DAMN/io_uring/池回收；SDM overlap；普通分配器局部性；local k-choice guard（不相交池的更便宜消融）；更大普通 IOTLB 容量、合法 mask 重划分、hugepage 与页局部池布局。它们都要在相等完整预算下执行；两份小例与两路/P2 草图不构成对 16 项设计的证据，不允许强制 PLRU victim 或人为单子限制。

[iommu-platform/docs/research-route-audit-20261009.md:50](../../iommu-platform/docs/research-route-audit-20261009.md)：

两项比较是纸面构造和手推，不能改称已运行模型。根接受记录与关键控制原件见round05。独立关键审查核对了[ICCD2018](https://arxiv.org/pdf/1808.09751)、[Barr ISCA2010](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)和[需求优先取消专利](https://patents.google.com/patent/US9542332B2/en)：它们支持pin、部分翻译缓存及取消组件已有前史，不等于证明本项目整个集合COPY合同已被发表。

[返回总目录](../README.md)
