# WaferTrans: Enabling IOMMU-free Distributed Virtual Address Translation for Wafer-scale GPUs

稳定ID：`wafertrans2026` · 类型：`preprint` · 主题：`multigpu`

作者：Xinru Tang; Jingxiang Hou; Guanghong Wu; Yang Hu; Shouyi Yin；年份：2026；出处：arXiv preprint arXiv:2609.06125v1。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：PTE-PC 一致性模型只跟踪 PTE insertion/removal；每 GPU 配备 PPD（Cuckoo filter，key=VPN+gpu id）定位持有 PTE 的 GPU；DPB 用 domain-membership register 广播更新；把阵列划分 presence domains，LSG 含每 domain 一个 GPU，本地 miss 后只查询 LSG 成员，在 wafer 内完成远端翻译并去掉 CPU-IOMMU 路径。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：WSG 为 mesh-connected GPU cluster，compute/DRAM dies 在 wafer interposer，I/O dies 在边缘；CPU 与 GPU 工艺不同，不能集成到 wafer（L69-L80, L185-L193）。；UM 中 host 持有完整 PTE 集，每个 GPU 只存本地 resident page 的 PTE；PTE 更新只由 page allocation 和 migration 产生；GPU 不直接修改 PTE 内容（L161-L194）。；PTE-PC 只维护 presence metadata 而非 PTE 内容；PPD 每 GPU 复制；DPB N-bit domain membership register；PTE update 只广播到 domain members（L174-L241）。；划分 presence domains，LSG 含每 domain 一个 GPU；local miss 后查询 LSG；page migration 只更新涉及设备的 PTE（L244-L274）。；16-GPU wafer baseline；PPD 1024 slots、11-bit fingerprint；wafer links 1.85TB/s、38-cycle；Wafer2CPU 180-cycle；PTE updates 占 6% 平均流量（L202-L224, L333-L337, L361-L377）。

限制：arXiv preprint；题头 IEEE JOURNAL OF LATEX CLASS 是模板占位，不是正式 venue（L1, L28, L82）。；仅 MGPUSim 模拟与 TSMC 28nm RTL 综合估算；无流片/FPGA 实机测量（L278-L279, L333-L337）。；PTE-PC 只跟踪 insert/remove，不维护 PTE 内容一致性/权限；未评估 TLB shootdown、并发迁移/失效与 page fault 处理（L174-L194）。；假设 page migration 只更新涉及设备且 migration infrequent；若迁移/失效频繁，domain 广播与 PPD 更新成本未评估（L245-L258, L361-L377）。；依赖 LSG 查询和 PPD 覆盖；PPD 容量 1024 slots 才饱和；PPD false-positive <0.39% 仍需处理（L202-L224, L318-L337）。；未评估安全/权限隔离、多租户、ASID 与尾延迟。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：multigpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://arxiv.org/pdf/2609.06125v1)
- [来源2](https://arxiv.org/html/2609.06125v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/wafertrans2026.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.pdf) · SHA-256 `cf4c86d2ea0a2ccb05b3370defb8a338736df0bf4c756e2d7961426a4f7ff5d7`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:21532](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt:6](../../iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt:28](../../iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt:161](../../iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt:244](../../iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt:333](../../iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt:361](../../iommu-platform/out/shared-translation-literature-20261006/papers/wafertrans2026.txt)
- [iommu-platform/out/innovation-council18-20261005/round1/C1.md:84](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round2/C1.md:40](../../iommu-platform/out/innovation-council18-20261005/round2/C1.md)：

**关键来源。** [HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf) §IV-F（本地 `hdpat2026.txt:555–604`，本轮再次读）的重定向明确指向持有 PTE 的 GPM，收益依赖后者实际存储/服务翻译；它不是只用一个 presence bit 凭空完成 walk。[WaferTrans](https://arxiv.org/html/2609.06125v1) §III 的 PTE presence 域、目录查询与转发更直接覆盖分布式位置层；本席 R1 已读正文，范围记录见 `sources/C1-new-prior-art.md`。这些证明“位置结构已有”，不单独证明任意新接口都重复。

[iommu-platform/out/innovation-council18-20261005/round1/C1.md:84](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)：

- 新查到的 [WaferTrans](https://arxiv.org/html/2609.06125v1) §III–IV 已用 presence domain 定位 PTE，并维护插入/移除。其示例返回翻译结果，更新开销评估受迁移不频繁影响。这是更近且更强的基线，不能只打集中 IOMMU。

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md:84](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md)：

- 新查到的 [WaferTrans](https://arxiv.org/html/2609.06125v1) §III–IV 已用 presence domain 定位 PTE，并维护插入/移除。其示例返回翻译结果，更新开销评估受迁移不频繁影响。这是更近且更强的基线，不能只打集中 IOMMU。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:253](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 wafertrans2026：PDF papers/wafertrans2026.pdf; text papers/wafertrans2026.txt; 报告 workers/G2_multigpu.md; 证据卡 workers/G2_multigpu-papers.json；reading=fulltext_worker_reviewed；status=preprint；year=2026；venue=arXiv preprint arXiv:2609.06125v1；doi=未知；source=https://arxiv.org/pdf/2609.06125v1；root_spotchecked=True

[返回总目录](../README.md)
