# BypassD: Enabling fast userspace access to shared SSDs

稳定ID：`bypassd2024` · 类型：`research_paper` · 主题：`lifecycle`

作者：Sujay Yadalam; Chloe Alverti; Vasileios Karakostas; Jayneel Gandhi; Michael Swift；年份：2024；出处：ASPLOS '24, Volume 1。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：扩展 IOMMU 翻译文件 VBA 到设备 LBA，并用 PASID/FTE/DevID 做权限检查与共享 SSD 文件直访。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：kernel 与硬件属于 TCB，用户进程和 UserLib 可恶意；NVMe 设备；ext4 且无 data journaling；只支持同步操作；文件全映射，不做 CoW/partial map

限制：IOMMU 硬件未商用，VBA 翻译靠模拟；cold fmap 大文件代价高；不支持 CoW/btrfs/非阻塞写；不支持 BypassD 与 kernel 接口并发访问；不支持跨 VM 文件共享

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://cgi.di.uoa.gr/~vkarakos/papers/asplos24_bypassd.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/bypassd2024.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.pdf) · SHA-256 `37855d743f3c60c71205bf0b0b48da8d9ec282c12c13444b18f6eed37990a90e`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:1160](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt:31](../../iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt:263](../../iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt:460](../../iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt:659](../../iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt:903](../../iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:204](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/performance-council-decision-20261005.md:98](../../iommu-platform/docs/performance-council-decision-20261005.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:93](../../iommu-platform/docs/research-landscape-20261005.md)：

| [BypassD](https://cgi.di.uoa.gr/~vkarakos/papers/asplos24_bypassd.pdf) | ASPLOS 2024 | 用户态共享 SSD，复用 IOMMU 做 VBA→LBA + 权限检查 | 真实软件/SSD 实验；硬件 IOMMU 改动用模型评估；ext4 without data journaling，不能写完全无日志 |

[iommu-platform/docs/performance-council-decision-20261005.md:98](../../iommu-platform/docs/performance-council-decision-20261005.md)：

- [BypassD，ASPLOS 2024](https://cgi.di.uoa.gr/~vkarakos/papers/asplos24_bypassd.pdf)，摘要/引言：把 IOMMU 用于文件偏移到设备块地址的翻译，支持共享 SSD 用户态访问。它展示的是从系统需求改变接口用途的思路；本项目不能据此声称新的通用转换引擎尚无人研究。

[iommu-platform/out/innovation-council18-20261005/round2/C2.md:30](../../iommu-platform/out/innovation-council18-20261005/round2/C2.md)：

| S7 | [BypassD，ASPLOS 2024](https://cgi.di.uoa.gr/~vkarakos/papers/asplos24_bypassd.pdf)：R2 核 §3.5 的撤销竞争，347–363；[ShareMMU，MICRO 2026 camera-ready](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf)：R2 核 §V 的 permission update/transient state，410–455、503–523。全文分别在 `/home/xvhaocheng/work/iommu-platform/out/research-landscape-20261005/papers/bypassd2024.txt`、`/home/xvhaocheng/work/iommu-platform/out/research-landscape-20261005/papers/sharemmu2026.txt`；本席本轮未重读两篇完整评估。 |

[iommu-platform/out/innovation-council18-20261005/round1/C3.md:157](../../iommu-platform/out/innovation-council18-20261005/round1/C3.md)：

- **S4 BypassD (ASPLOS 2024)**：[原文](https://cgi.di.uoa.gr/~vkarakos/papers/asplos24_bypassd.pdf)。本地 `iommu-platform/out/research-landscape-20261005/papers/bypassd2024.txt`。读正文，重点 §3.6、§4.5.2、§6。§3.6 的块复用约束可在约 361 行定位。硬件 IOMMU 延迟评价包含模型；不能描述成全部已经有目标硬件实现。

[返回总目录](../README.md)
