# ShareMMU: Supporting Secure Address Translation Sharing among Untrusted Accelerators

稳定ID：`sharemmu2026` · 类型：`research_paper` · 主题：`accelerators`

作者：Faiz Alam; Mohamed Tarek Ibn Ziad; Yuanchao Xu; Abdulrahman Mahmoud; Greg Byrd; Aamer Jaleel；年份：2026；出处：MICRO 2026。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：跨加速器复用翻译时，不可信私有TLB/设备可能忽略失效或伪造物理地址，因此需要在共享路径继续检查每设备权限。

机制／已存认识：IOMMU内Permission Tracker以PPN为标签记录每加速器权限和VPN，验证预翻译访问，支持共享TLB翻译重用、back-invalidation与AA-LRU。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：共享页表，叶节点存每加速器权限；共享TLB和IOMMU可信，加速器/private TLB不可信；OS可信并设置合法权限；Permission Tracker inclusive于私有TLB；加速器会响应back-invalidation并写回脏行/flush私有TLB；context switch间隔5ms，private TLB flush，shared TLB保留

限制：gem5 PARADE仿真，无真实硬件；威胁模型不覆盖全部侧信道和TEE；Permission Tracker容量和结合度有上界；未显式讨论A/D位；context switch flush Permission Tracker，PT延迟敏感；别名和disjoint working set可能导致PT thrash

与研究关系：约束跨设备翻译共享、有效期和排空主张；§V-C7的stalled-miss前提必须与实际设备资源依赖分开核对。

相邻资料：accelerators主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf)
- [来源2](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf?VersionId=5BjALNe5CpmAmE5xIkXa6I31Ml7zg1ZO)
- [来源3](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/sharemmu2026.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.pdf) · SHA-256 `a9759951dc4a17ec99e95bd82b15c7234a18aab4cce0e696c24aa798af6dcfd2`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:15963](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt:1001](../../iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt:351](../../iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt:419](../../iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt:487](../../iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt:777](../../iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:277](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/flow-iommu-candidate-review-20261014.md:9](../../iommu-platform/docs/flow-iommu-candidate-review-20261014.md)：

容量干扰、命中请求等待和重复页表服务是不同问题，用户的三层划分有助于将动作与原因对应。当前E2/E3已分别留下有限替换与等待传播证据。但“现有翻译系统缺少协同和拥塞控制”作为全领域结论过宽：MASK已有翻译缓存fill-token、旁路缓存和共享内存调度；Neighborhood-aware Translation已有不同最终页的在途上层访问合并；ShareMMU已有跨不可信加速器的翻译复用与逐设备权限协议。[MASK](https://rausavar.github.io/pubs/mask-asplos18.pdf)、[Neighborhood-aware Translation](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)、[ShareMMU作者入口](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)。

[iommu-platform/docs/innovation-followup-20261006.md:23](../../iommu-platform/docs/innovation-followup-20261006.md)：

主来源：[作者论文页](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)、[公开 artifact](https://doi.org/10.5281/zenodo.21543532)。本次概念 DOI 对应记录 22681559，文件为 `sharemmu-micro59-ae-v3.zip`。只提取必要源码，没有运行该模拟器；完整来源和本地文件 SHA256 见清单。

[iommu-platform/docs/innovation-focus-20261006.md:24](../../iommu-platform/docs/innovation-focus-20261006.md)：

[ShareMMU，MICRO 2026](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted) 的 Algorithm 1 使用共享计数与 LRU 项选择 Permission Tracker 的牺牲项；正文允许调整权重和随机替换。本地全文 的物理行 443–495、688、903–914 是直接核查位置。其默认计分可写为：

[iommu-platform/docs/research-landscape-20261005.md:95](../../iommu-platform/docs/research-landscape-20261005.md)：

| [ShareMMU](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf) | MICRO 2026 camera-ready | Permission Tracker 验证 pre-translated access，支持不可信加速器共享 | gem5 PARADE；无真实硅/独立复现；非商用结论 |

[返回总目录](../README.md)
