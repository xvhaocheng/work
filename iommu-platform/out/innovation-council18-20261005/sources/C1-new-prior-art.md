# C1 定向查新：方向 1 的新增近邻

日期：2026-10-05。由 Codex C1 实际检索。以下是来源摘记，不是复现，也不把搜索摘要当完整论文。未下载新的 PDF。仅列直接关联本方向的原始作者、机构、论文来源；搜索中的聚合站、评论与专利没有用于新颖性裁决。

| 来源 | 当前阅读范围 | 对本轮提案的约束 |
|---|---|---|
| [Utopia，MICRO 2023](https://arxiv.org/abs/2211.12205) | arXiv 原始摘要及题录；v4，2023-10-06 | 已组合限制映射与自由映射；不能把“热点用规则映射、其他页面 fallback”作为新意。机制细节与 NUMA 范围仍须全文复核。 |
| [Mosaic Pages，ASPLOS 2023](https://www.cs.unc.edu/~porter/pubs/mosaic.pdf) | 作者 PDF 的搜索解析片段：概览、Iceberg hashing、未来工作 | 已用小候选集合压缩小页翻译；作者报告高占用率仍能运行。不能把“多哈希候选提高利用率”直接当新意。与 MICRO 2017 GPU Mosaic 是不同论文。 |
| [CLAP，MICRO 2025](https://pure.skku.edu/en/publications/leveraging-chiplet-locality-for-efficient-memory-mapping-in-multi/)；[正式 DOI](https://doi.org/10.1145/3725843.3756090) | 机构完整摘要；DOI 搜索解析的 §4–4.2 及部分 MMA/RT 描述。直接 DOI 打开 403，未核全篇 | 已根据 chiplet-locality 选择连续区域/映射粒度。单纯“同时顾及翻译覆盖与 chiplet 放置”不新。需复核动态变化、碎片与已有对象重布局。 |
| [Chiplet-Contiguous Layout，2026 预印本](https://arxiv.org/abs/2606.11718) | arXiv 摘要及题录；未全文 | 已通过数据布局让 LLM GEMM 的 chiplet 局部性适配页粒度，且摘要主张不改 OS/硬件。任何硬件布局提案必须把这种软件方案作为强对手。 |
| [CATO，作者页标 MICRO 2026](https://skku-compasslab.github.io/compasslab/publications/260710-micro-cato/) | 作者页面的完整搜索索引介绍；直接打开 404，未核全文或正式论文题录 | 已提出本地拥堵时向 CXL 卸载翻译，并使用末级 PTE 哈希副本。“慢层可能更快”“翻译流量也应调度”不是空白；不能把作者页面对特定 OS 的页表放置描述泛化成规范限制。 |
| [Avatar / CAVA，MICRO 2024](https://pure.skku.edu/en/publications/a-case-for-speculative-address-translation-with-rapid-validation-/)；[作者 PDF](https://zoon17.github.io/pdfs/avatar_micro24.pdf) | 机构摘要、PDF 搜索解析的介绍与迁移片段；未通读全文 | 已将映射信息嵌入压缩数据 sector 以快速验证推测地址，并讨论迁移。不能宣称“到数据端验证”本身新。 |
| [Midgard，ISCA 2021](https://midgard.epfl.ch/wp-content/uploads/2022/01/midgard-rebooting-virtual-memory-1.pdf) | 作者 PDF 的 §III 等搜索解析片段；未通读全文 | 已分离前端命名/保护与内存侧物理翻译。新增一个地址空间或延后翻译不是充分差异。 |
| [VBI，ISCA 2020](https://users.ece.cmu.edu/~saugatag/papers/20isca_vbi.pdf) | 作者 PDF 的 §4–5 搜索解析片段；未通读全文 | 已探索可编程内存翻译层与物理分配/迁移；“把分配下沉到内存端”不能直接当新意。 |
| [Trans-FW，HPCA 2023](https://jiemingyin.github.io/docs/HPCA2023.pdf) | 作者 PDF 的介绍、基线架构搜索解析片段；未通读全文 | 已做远端翻译转发。需要明确区分“转发翻译”与“在验证地点执行数据读取”。 |
| [WaferTrans，2026-09-05 预印本](https://arxiv.org/abs/2609.06125)；[原始全文 HTML](https://arxiv.org/html/2609.06125v1) | 已读 HTML 全部正文 §I–V；图内配置与坐标未独立核对 | 已用 PTE presence directory 与域内更新移除 CPU-IOMMU 依赖；§III-B 示例最后返回翻译结果，§IV-D 的低更新流量以迁移不频繁为条件。不能把“分布式定位 PTE”当新意。 |

WaferTrans 与 HDPAT 的 CPU 位置/系统组织假设不同，不可直接比较论文 speedup。WaferTrans §II-C 把某种 CPU/GPU 同晶圆集成说成不可实现，不在这里接受为普适技术事实；只按其选定拓扑理解。全文也不足以单独证明任意迁移/权限并发交错都正确。

Revelator 的[公开 artifact 文档](https://github.com/CMU-SAFARI/Virtuoso/blob/main/docs/revelator.md)已列 NUMA 与 THP 实现变体；同时本地 v2 §4.6 已明确 node-policy-first。C1 的候选不能通过把该论文误读成 NUMA-unaware 获得差异。

优先补全文：CLAP、Mosaic Pages、Utopia、Avatar；其次 CATO 的正式论文、Chiplet-Contiguous Layout、Trans-FW。WaferTrans 新增原文足以推翻“晶圆内分布式翻译仍为空白”的泛泛命题，但不足以判定 C1-P2 必然重复。
