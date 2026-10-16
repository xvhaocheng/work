# Tiara: A Programmable Line-Rate ISA for Remote Memory Access

稳定ID：`tiara2026` · 类型：`research_paper` · 主题：`data-lifecycle`

作者：unknown；年份：2026；出处：arXiv 2026。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：Indirection Wall——RDMA one-sided verb 需精确远端地址，地址依赖远端数据时每层代价一个 RTT（`tiara2026.txt:60-68`）。

机制／已存认识：NIC-side 静态可验证 ISA；预注册 operator（类 eBPF）；register-chained load 使 Load 结果可直接当下一 Load 地址；unified addressing；async+wait；注册期静态验证终止性与内存区域（`tiara2026.txt:22-32, 262-331`）。

支持边界：既有已存全文与逐篇笔记；本轮不重审评价。

限制：未复现论文实验；仅存全文不等于全文精读。；不能推出"offsloading 一律优于 RPC"；也不能把 FPGA 的 0.79μs/hop 当作 ASIC 性能。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。

原始来源：

- [来源1](https://arxiv.org/pdf/2606.13708v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/tiara2026.pdf) · canonical：[原路径](../../iommu-platform/out/research-landscape-20261005/papers/tiara2026.pdf) · SHA-256 `1cdbb3638e32a2b9fdebcc0a004023e63c3654e3e362e4f0aaf4bfa369553b05`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/research-landscape-20261005/source-manifest.json:288](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/innovation-council18-20261006.md:71](../../iommu-platform/docs/innovation-council18-20261006.md)
- [iommu-platform/docs/research-landscape-20261005.md:49](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:105](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:47](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:103](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/innovation-council18-20261005/round1/C4.md:14](../../iommu-platform/out/innovation-council18-20261005/round1/C4.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/A4.md:38](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/A4.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:49](../../iommu-platform/docs/research-landscape-20261005.md)：

间接访存必须分开两类：地址依赖远端返回数据的真串行链，和地址已可批量发出的 scatter/gather。[IMPICA](https://pdl.cmu.edu/PDL-FTP/associated/16iccd_impica.pdf) 系统化 pointer chasing 的串行依赖；[DX100](https://arxiv.org/pdf/2505.23073v1) 明确不支持 pointer chasing，只做 array-based indirection；[Tiara](https://arxiv.org/pdf/2606.13708v1) Table 1 中 PagedAttention 的朴素 stop-and-wait 是 160 RTT，但最优批处理基线是 2 RTT，不能用 160 RTT 讲必要性；其应用间接表遍历不是硬件 OS 页表，已知地址的散读不能算不可并行的指针链。[NDS](https://par.nsf.gov/servlets/purl/10321761) 从维度不匹配处理 marshalling 与带宽利用，[GPS](https://harinimuthukrishnan.net/publication/gps/gps.pdf) 用 publish-subscribe 和弱内存模型合并写，[WIC](https://www.usenix.org/system/files/atc25-zhang-jiajian.pdf) 用 warp 级中断替代轮询。原证据卡见 L3。本稿不把“只能 PIM/NIC”写成结论；只能写：对真串行地址依赖，已见 PIM/近 NIC/近内存解链方案，其他预测、helper thread、多链并行仍需逐案验证。

[iommu-platform/docs/research-landscape-20261005.md:105](../../iommu-platform/docs/research-landscape-20261005.md)：

| [Tiara](https://arxiv.org/pdf/2606.13708v1) | arXiv 2026；预印本 | NIC-side 可验证 ISA，折叠远程 indirection | 主延迟为Verilator周期模型、吞吐由延迟/槽位推导，主要对手为分析模型；另有单MP FPGA构建/时序及BF-2实物点。PagedAttention最优批处理基线2 RTT，不是朴素160 RTT |

[iommu-platform/docs/innovation-council18-20261006.md:71](../../iommu-platform/docs/innovation-council18-20261006.md)：

- **Tiara不能简写为“实板性能证明”。** §4.1主延迟是Verilator周期模型，吞吐由延迟/槽位推导，主要非Tiara基线是分析模型，另有BF-2实物点；单MP FPGA构建不等于所有性能曲线实测。根核本地363–378行，已修地图的评估类型。[Tiara预印本](https://arxiv.org/pdf/2606.13708v1)

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:72](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"old": "### 7.1 已具备较强全文/关键节证据的前沿\n\n- [HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)：HPCA 2026，wafer-scale GPU 分布式翻译；重要限制是排除 GPU-to-GPU migration，未评估 TLB shootdown，结果为仿真。\n- [Revelator v2](https://arxiv.org/pdf/2508.02007v2)：ISCA 2026，DOI 10.1109/ISCA66397.2026.00147；v2 标题为 System-Software-Guided；Linux 6.10.8 原型 + Sniper/Virtuoso + RTL 综合；OS 分配协同仍是前提，不照搬 v1 旧数字。\n- [ShareMMU](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf)：MICRO 2026 camera-ready；Permission Tracker、定向 back-invalidation、权限下降局部化；仍等待 dirty write-back/flush acknowledgment，未消灭全部动态更新/排空成本；只作为前沿条件，不据此转安全主线。\n- [DX100](https://arxiv.org/pdf/2505.23073v1)：ISCA 2025，批量 indirection 重排；明确不覆盖 pointer chasing。\n- [MINISA](https://arxiv.org/pdf/2603.20623v1)：ISPASS 2026；本地 arXiv v1；控制面 ISA，与数据搬运正交。\n- [ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf)：ISCA 2026；真机 RTX 3090，采样观测。\n- [OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf)：HPCA 2025，对象感知多 GPU page management。\n- [Tiara](https://arxiv.org/pdf/2606.13708v1)：arXiv 2026 预印本，不是 NSDI 2022 同名负载均衡器；应用间接表≠硬件 OS 页表；Table 1 中 PagedAttention 最优批处理为 2 RTT，不能只拿朴素 160 RTT 讲必要性。",

[返回总目录](../README.md)
