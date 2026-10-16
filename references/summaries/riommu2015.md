# rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers

稳定ID：`riommu2015` · 类型：`research_paper` · 主题：`io`

作者：Moshe Malka; Nadav Amit; Muli Ben-Yehuda; Dan Tsafrir；年份：2015；出处：ASPLOS '15 (ACM International Conference on Architectural Support for Programming Languages and Operating Systems)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：用每环扁平表替代层次页表，每环至多一个 IOTLB 项；IOVA 为索引，顺序映射/失效，I/O 突发结束才失效。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：高带宽设备通过循环 ring 与 OS 交互，driver/device 顺序消费 descriptor。；IOVA 短暂且按序 map/unmap；driver 可合理设置 rRING size；target buffer pinned。；每个 ring 的活跃 IOVA 数可被 N 限定；无常态 I/O page fault。；rIOMMU 只补充而非替代 baseline IOMMU；不适用 RDMA，SATA 太慢无收益。

限制：只针对 ring buffer 高带宽设备；未处理多设备共享 IOTLB 或跨设备共享翻译。；rIOMMU 评估是软件模拟：执行 rIOMMU 驱动代码并忙等 2150 cycles 模拟 IOTLB 失效，不是硬件原型。；性能模型假设吞吐由 CPU 周期决定，忽略 IOMMU/设备硬件并行差异，强绑定其 10/40GbE 平台。；不覆盖动态迁移、远端 PTW、多租户、尾延迟分布、失效延迟尾部。；I/O page fault 标准虽存在，但论文称当时系统通常不支持。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：io主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)
- [来源2](https://dants.github.io/papers/riommu-asplos-2015.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/riommu2015.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.pdf) · SHA-256 `70a58dd66ee033036c821b1dfa6e19eba62e45c806cf8fb30ee019322e450be3`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:15319](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt:4](../../iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt:58](../../iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt:23](../../iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt:128](../../iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt:310](../../iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt:553](../../iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:28](../../iommu-platform/docs/research-landscape-20261005.md)：

第二阶段是多加速器共享与 I/O 软件控制面：[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)、[Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 研究 GPU 多应用/多租户下共享 TLB 与 walker 的干扰，说明增加 TLB/walker 不自动消除竞争；[rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)、[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)、[IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) 则把 I/O 路径的 IOVA 分配、失效批处理和 IOTLB miss 暴露为一阶成本。已解决的是：软件 IOVA 管理、ring 语义、批量/延迟失效、hugepage mapping 都有可测收益。新条件使原假设吃紧：10–40 Gbps 到 100–200 Gbps、单租户到多租户、性能目标到时间可预测性与不可信设备安全共享。

[iommu-platform/docs/composition-history-isolation-20261007.md:385](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

重新核[MMU-aware DMA ICCD2018 §IV-C](https://arxiv.org/pdf/1808.09751)的burst身份/失败排空重发，以及[rIOMMU ASPLOS2015](https://dants.github.io/papers/riommu-asplos-2015.pdf)的ring下一项预取；未把相近构件说成本候选精确同式。前述强前作及NB-Walker全文缺口仍在。DeepSeek三次完整high/393216草案与修订，共492799 provider tokens，未执行；Codex独立规格、关键源码/原文/机制分析、补丁修正、实际运行与验收。无Claude或新Codex子agent。

[iommu-platform/docs/higher-model-mechanism-proposal-20261009.md:18](../../iommu-platform/docs/higher-model-mechanism-proposal-20261009.md)：

- [rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers, ASPLOS 2015](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf) (DOI 10.1145/2694344.2694355)：flat ring / current+next、隐式替换/末尾失效。**边界**：限制在 map/unmap 可预测顺序；有效 DMA 可乱序，不得误写。

[iommu-platform/docs/research-landscape-20261005.md:90](../../iommu-platform/docs/research-landscape-20261005.md)：

| [rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf) | ASPLOS 2015 | ring flat table，一 ring 一 IOTLB entry，burst 末失效 | 依赖 ring 顺序；部分模拟；非通用 IOMMU |

[返回总目录](../README.md)
