# cuPTW: Leveraging Idle Compute Units for Massively Parallel GPU Page Table Walks

稳定ID：`cuptw2026` · 类型：`research_paper` · 主题：`gpu`

作者：Zihang Chen; Tianao Ge; Lieven Eeckhout; Hongyuan Liu; Jiayi Huang；年份：2026；出处：Proc. ACM Meas. Anal. Comput. Syst. 10, 2, Article 35 (June 2026)。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：把translation request offload到轻量translation wavefront，用空闲SIMD/scalar单元执行page walk；LDS实现software-managed page walk cache；多线程translation wavefront；L2 TLB MSHR饱和时选择性转发

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：GPU存在大量空闲compute unit资源；page walk瓶颈来自walker并行度与L2 TLB MSHR饱和；4KB页、4-level radix page table；可用ASID和PTBR描述翻译上下文；LDS可被重用于page walk cache

限制：主评估为MGPUSim仿真，RTL/综合只覆盖小逻辑；依赖空闲资源与MSHR饱和场景；Low-MPKI收益有限；Medium场景有CU负载不均；未覆盖真实硅后、FPGA、IOMMU专门共享、跨设备安全和尾延迟

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://maxkev1n.github.io/publications/sigmetrics-2026/3805633.pdf)
- [来源2](https://doi.org/10.1145/3805633)
- [来源3](https://maxkev1n.github.io/publications/sigmetrics-2026/)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/cuptw2026.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.pdf) · SHA-256 `b8afc78ee17479e55d162e1ddeb582178c4b5eb6493a239816873c1f8320470b`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:3322](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt:123](../../iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt:410](../../iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt:777](../../iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt:1285](../../iommu-platform/out/shared-translation-literature-20261006/papers/cuptw2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md:163](../../iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md)
- [iommu-platform/docs/shared-translation-literature-20261006.md:69](../../iommu-platform/docs/shared-translation-literature-20261006.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/shared-translation-literature-20261006.md:69](../../iommu-platform/docs/shared-translation-literature-20261006.md)：

**5. 翻译执行的位置与数据获取次序还有变化空间。** 本轮补充SoftWalker、DPT、FS-HPT、EMT及近内存结构工作，详见深化综述。 SPARTA把部分翻译移向内存侧，Avatar与Revelator探索推测后验证，cuPTW借用计算资源，CATO研究争用下的卸载。从这些工作可提出一个更开放的问题：在具体系统中，翻译必须占用哪些专用资源、必须在哪一步完成，哪些依赖可以安全重排？但每条新路径都要付验证、回退、带宽或计算资源成本，不能把“隐藏延迟”当成取消工作。[cuPTW作者与全文入口](https://maxkev1n.github.io/publications/sigmetrics-2026/)

[iommu-platform/docs/paper-driven-innovation-20261008.md:27](../../iommu-platform/docs/paper-driven-innovation-20261008.md)：

| [cuPTW，POMACS/SIGMETRICS 2026](https://maxkev1n.github.io/publications/sigmetrics-2026/3805633.pdf)，§5.3–5.4 | 闲置 CU 执行 walk；MSHR 饱和时在 L2 TLB 转发，避免在更后级转发仍被前级占用挡住 | 前沿已关注“算得快但前级资源没释放”；具体瓶颈位置与资源寿命必须明确 |

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/shared-translation-literature-20261006.md:67](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/shared-translation-literature-20261006.md)：

**5. 翻译执行的位置与数据获取次序还有变化空间。** SPARTA把部分翻译移向内存侧，Avatar与Revelator探索推测后验证，cuPTW借用计算资源，CATO研究争用下的卸载。从这些工作可提出一个更开放的问题：在具体系统中，翻译必须占用哪些专用资源、必须在哪一步完成，哪些依赖可以安全重排？但每条新路径都要付验证、回退、带宽或计算资源成本，不能把“隐藏延迟”当成取消工作。[cuPTW作者与全文入口](https://maxkev1n.github.io/publications/sigmetrics-2026/)

[iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md:163](../../iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md)：

- DOI：`https://doi.org/10.1145/3805633`【cuptw2026.txt:L32,L48】。

[返回总目录](../README.md)
