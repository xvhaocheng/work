# 五项文献题录与原文位置

2026-10-17 核对。只核作者、题名、出处、年份、DOI 和本地原文；不写技术总结，不改变原阅读状态，也不标记用户已读。机器表见 [metadata.json](metadata.json)／[metadata.csv](metadata.csv)。四项已有 `references/papers/` PDF 索引，均解析到既有原件且本轮 SHA-256 与目录记录相同；第 4 项未找到索引原文，只提供获取链接，没有下载缺失论文。

## 1. rIOMMU

**Moshe Malka; Nadav Amit; Muli Ben-Yehuda; Dan Tsafrir.** *rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers*. Proceedings of the Twentieth International Conference on Architectural Support for Programming Languages and Operating Systems (**ASPLOS 2015**), pp. 355–368. DOI：[10.1145/2694344.2694355](https://doi.org/10.1145/2694344.2694355)。

本地：[references/papers/riommu2015.pdf](../../../../references/papers/riommu2015.pdf)；实际原件位于 `iommu-platform/out/shared-translation-literature-20261006/papers/riommu2015.pdf`。SHA-256：`70a58dd66ee033036c821b1dfa6e19eba62e45c806cf8fb30ee019322e450be3`。

作者、会议全名、年份和页码由[出版方提交的 DOI 元数据](https://api.crossref.org/works/10.1145/2694344.2694355)核对，完整题名由 title＋subtitle 拼回；[作者公开原文](https://dants.github.io/papers/riommu-asplos-2015.pdf)也在首页给出会议。没有把同时出现的 SIGPLAN Notices 50(4) 题录另计一篇。

## 2. Kurth 等，ICCD 2018

**Andreas Kurth; Pirmin Vogel; Andrea Marongiu; Luca Benini.** *Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine*. **2018 IEEE 36th International Conference on Computer Design (ICCD)**, pp. 292–300. DOI：[10.1109/ICCD.2018.00052](https://doi.org/10.1109/ICCD.2018.00052)。

本地：[references/papers/socdma2018.pdf](../../../../references/papers/socdma2018.pdf)；实际原件位于 `iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.pdf`。SHA-256：`518f3bb180de2f17a06f03c049be76fc7902513604c69644041a0971e8a7ed97`。本地是 arXiv 作者稿 v1，不冒充 IEEE 排印版本。

作者/年份/DOI/页码由[出版方 DOI 元数据](https://api.crossref.org/works/10.1109/ICCD.2018.00052)、[作者机构记录](https://iris.unimore.it/handle/11380/1179001?mode=complete)及[作者预印本记录](https://arxiv.org/abs/1808.09751)交叉核对。作者稿获取：[arXiv PDF](https://arxiv.org/pdf/1808.09751)。

## 3. Amit、Ben-Yehuda、Yassour 的 IOTLB 论文

**Nadav Amit; Muli Ben-Yehuda; Ben-Ami Yassour.** *IOMMU: Strategies for Mitigating the IOTLB Bottleneck*. **Sixth Annual Workshop on the Interaction between Operating Systems and Computer Architecture (WIOSCA 2010)**。

本地：[references/papers/iotlb_bottleneck2010.pdf](../../../../references/papers/iotlb_bottleneck2010.pdf)；实际原件位于 `iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_bottleneck2010.pdf`。SHA-256：`5889c11577f41f13026198dbd264f429f98a7f3cf1d9180ceb0320bbec6af3b6`。本地首页明确为 WIOSCA 2010 author manuscript，HAL `inria-00493752` v1，日期 2010-06-21；[作者题录页](https://nadav.amit.zone/publications/workshops/2010-iommu.html)同样列 WIOSCA 2010。获取：[作者 PDF](https://nadav.amit.zone/publications/pdfs/amit2010iommu.pdf)。

**年份和 DOI 要按版本分开。** 作者页还链接同题名的 Springer 后续章节：[10.1007/978-3-642-24322-6_22](https://doi.org/10.1007/978-3-642-24322-6_22)，收于 *Computer Architecture: ISCA 2010 International Workshops … Revised Selected Papers*，LNCS 6161，pp. 256–274。Springer 的[当前章节页](https://link.springer.com/chapter/10.1007/978-3-642-24322-6_22)可见引用写 **2011**，但[官方 BibTeX 导出](https://citation-needed.springer.com/v2/references/10.1007/978-3-642-24322-6_22?flavour=citation&format=bibtex)写 **2012**；[书籍页](https://link.springer.com/book/10.1007/978-3-642-24322-6)列纸版 2011-10-28、电子版 2012-02-15。这里保留这些来源差异：引用本地作者稿用 WIOSCA 2010；若选 Springer 章节，可按官方导出用 2012，并明确该版本。未把本地 2010 PDF 自动标成 Springer 版本。

## 4. Zero-Day 团队的 RISC-V IOMMU 设计材料

已确认的该团队官方设计材料为：**Manuel Rodríguez; Francisco Costa; Bruno Sá; Sandro Pinto.** *Open-source RISC-V Input/Output Memory Management Unit (IOMMU) IP*. **RISC-V Summit Europe 2023**, Barcelona, 2023-06-05 至 06-09，**两页 extended abstract，并有一页 poster**。已核来源未发现 DOI；不虚构会议长文或期刊版本。[作者机构页](https://algoritmi.uminho.pt/user/sandro-emanuel-salgado-pinto/)也把它归为 `2023 | conference-abstract`。

`references/` 的题录/原文索引中未找到此项。获取：[官方 extended abstract](https://riscv-europe.org/summit/2023/media/proceedings/posters/2023-06-07-Manuel-RODR%C3%8DGUEZ-abstract.pdf)／[官方 poster](https://riscv-europe.org/summit/2023/media/proceedings/posters/2023-06-07-Manuel-RODR%C3%8DGUEZ-poster.pdf)。没有下载到本地。

**作者显示存在差异：** extended abstract 第一页写 **Francisco Costa**，poster 写 **Francisco Marques**；其余三名一致。以上作者列表严格按两页 abstract，机器记录另存 poster 的完整作者列表，不擅自合并姓名。这里没有找到另一篇可准确认定为该团队“设计与评估长文”的论文；若用户原指其他题目，此项身份仍应再对照精确题名。Koenig、Zelioli、Benini 的 DATE 2025 *Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs* 是另一作者组，不能冒充本项。

## 5. MASK

**Rachata Ausavarungnirun; Vance Miller; Joshua Landgraf; Saugata Ghose; Jayneel Gandhi; Adwait Jog; Christopher J. Rossbach; Onur Mutlu.** *MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency*. Proceedings of the Twenty-Third International Conference on Architectural Support for Programming Languages and Operating Systems (**ASPLOS 2018**), pp. 503–518. DOI：[10.1145/3173162.3173169](https://doi.org/10.1145/3173162.3173169)。

本地：[references/papers/mask2018.pdf](../../../../references/papers/mask2018.pdf)；实际原件位于 `iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.pdf`。SHA-256：`f72e642e761582784d7508dabc743b5e5176f1bb2cb11282260bef18545c41fb`。

完整作者、会议、年份、页码由[出版方 DOI 元数据](https://api.crossref.org/works/10.1145/3173162.3173169)核对；原文及获取入口：[作者研究组 PDF](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)。

## 核对材料与限制

- [metadata.json](metadata.json) 保存全部作者、版本差异、原文实际路径/hash、来源 URL 和缺失链接；[metadata.csv](metadata.csv) 便于后续引用。
- [raw](raw/) 保存三份 DOI 登记元数据、作者/机构网页、Springer 官方 BibTeX；[local-headers.md](local-headers.md) 保存既有原文的题名/作者页眉位置。
- [doi-metadata-receipt.json](doi-metadata-receipt.json) 与 [primary-page-receipts.json](primary-page-receipts.json) 区分成功获取、429/403 失败和 HTTP200 challenge 页面。Springer 直接下载的两个 HTML 是 challenge，**不能作出版方题录原件**；其年份由浏览工具的出版社页面和成功获取的官方 BibTeX核对。没有把失败请求写成已验证。
- [browser-metadata-observations.json](browser-metadata-observations.json) 记录官方网页/在线 PDF 的元数据定位；它是字段摘录，不是缺失论文的本地原文。

本子任务使用 ARS 的 citation-check 范围，由 Codex 查原始来源、核 hash 并整理；未调用 DeepSeek 或其他外部模型。
