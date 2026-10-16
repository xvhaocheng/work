# Accelerating Pointer Chasing in 3D-Stacked Memory: Challenges, Mechanisms, Evaluation

稳定ID：`impica2016` · 类型：`research_paper` · 主题：`accelerators`

作者：Kevin Hsieh; Samira Khan; Nandita Vijaykumar; Kevin K. Chang; Amirali Boroumand; Saugata Ghose; Onur Mutlu；年份：2016；出处：ICCD 2016。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：3D-stacked memory 逻辑层内的 pointer chasing accelerator；address engine/access engine 解耦地址生成与访存，用 region-based page table 在 accelerator 内做地址翻译。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：pointer chasing 代码 offload 到 IMPICA；数据通过 special API 分配在连续 IMPICA regions；OS 预留 VA 并管理 IMPICA RPT/CPU 页表一致性；IMPICA 页表与 CPU 页表解耦；coherence 通过所有 IMPICA regions 操作都在 accelerator 执行来避免共享；当前 workload 不做 CPU 更新

限制：无硬件原型，仅 gem5/DRAMSim2 仿真；需要 special API 和程序修改；coherence 简化，不支持 CPU 与 IMPICA 同时更新；无安全/多租户；仅 pointer chasing；依赖连续 IMPICA regions

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[sparta2020](sparta2020.md)：SPARTA 引用 IMPICA 为 ICCD 2016。；[dipta2017](dipta2017.md)：两者都面向 near-memory translation，但 IMPICA 是 application-specific pointer chasing + RPT，DIPTA 是通用 MPU 页表机制。

原始来源：

- [来源1](https://pdl.cmu.edu/PDL-FTP/associated/16iccd_impica.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/impica2016.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.pdf) · SHA-256 `c299e13c1f024888cc7ace650041e0469d789bf866dbeff4f953307fdcc6b9bc`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:8328](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt:83](../../iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt:338](../../iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt:345](../../iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt:452](../../iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt:458](../../iommu-platform/out/shared-translation-literature-20261006/papers/impica2016.txt)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:171](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:49](../../iommu-platform/docs/research-landscape-20261005.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:49](../../iommu-platform/docs/research-landscape-20261005.md)：

间接访存必须分开两类：地址依赖远端返回数据的真串行链，和地址已可批量发出的 scatter/gather。[IMPICA](https://pdl.cmu.edu/PDL-FTP/associated/16iccd_impica.pdf) 系统化 pointer chasing 的串行依赖；[DX100](https://arxiv.org/pdf/2505.23073v1) 明确不支持 pointer chasing，只做 array-based indirection；[Tiara](https://arxiv.org/pdf/2606.13708v1) Table 1 中 PagedAttention 的朴素 stop-and-wait 是 160 RTT，但最优批处理基线是 2 RTT，不能用 160 RTT 讲必要性；其应用间接表遍历不是硬件 OS 页表，已知地址的散读不能算不可并行的指针链。[NDS](https://par.nsf.gov/servlets/purl/10321761) 从维度不匹配处理 marshalling 与带宽利用，[GPS](https://harinimuthukrishnan.net/publication/gps/gps.pdf) 用 publish-subscribe 和弱内存模型合并写，[WIC](https://www.usenix.org/system/files/atc25-zhang-jiajian.pdf) 用 warp 级中断替代轮询。原证据卡见 L3。本稿不把“只能 PIM/NIC”写成结论；只能写：对真串行地址依赖，已见 PIM/近 NIC/近内存解链方案，其他预测、helper thread、多链并行仍需逐案验证。

[iommu-platform/docs/research-landscape-20261005.md:101](../../iommu-platform/docs/research-landscape-20261005.md)：

| [IMPICA](https://pdl.cmu.edu/PDL-FTP/associated/16iccd_impica.pdf) | ICCD 2016 | 3D 堆叠内存内 pointer chasing + address decoupling | gem5；仅适合可区域化链表等结构 |

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:41](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"new": "间接访存必须分开两类：地址依赖远端返回数据的真串行链，和地址已可批量发出的 scatter/gather。[IMPICA](https://pdl.cmu.edu/PDL-FTP/associated/16iccd_impica.pdf) 系统化 pointer chasing 的串行依赖；[DX100](https://arxiv.org/pdf/2505.23073v1) 明确不支持 pointer chasing，只做 array-based indirection；[Tiara](https://arxiv.org/pdf/2606.13708v1) Table 1 中 PagedAttention 的朴素 stop-and-wait 是 160 RTT，但最优批处理基线是 2 RTT，不能用 160 RTT 讲必要性；其应用间接表遍历不是硬件 OS 页表，已知地址的散读不能算不可并行的指针链。[NDS](https://par.nsf.gov/servlets/purl/10321761) 从维度不匹配处理 marshalling 与带宽利用，[GPS](https://harinimuthukrishnan.net/publication/gps/gps.pdf) 用 publish-subscribe 和弱内存模型合并写，[WIC](https://www.usenix.org/system/files/atc25-zhang-jiajian.pdf) 用 warp 级中断替代轮询。原证据卡见 L3。本稿不把“只能 PIM/NIC”写成结论；只能写：对真串行地址依赖，已见 PIM/近 NIC/近内存解链方案，其他预测、helper thread、多链并行仍需逐案验证。"

[iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:47](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)：

间接访存必须分开两类：地址依赖远端返回数据的真串行链，和地址已可批量发出的 scatter/gather。[IMPICA](https://pdl.cmu.edu/PDL-FTP/associated/16iccd_impica.pdf) 系统化 pointer chasing 的串行依赖；[DX100](https://arxiv.org/pdf/2505.23073v1) 明确不支持 pointer chasing，只做 array-based indirection；[Tiara](https://arxiv.org/pdf/2606.13708v1) Table 1 中 PagedAttention 的朴素 stop-and-wait 是 160 RTT，但最优批处理基线是 2 RTT，不能用 160 RTT 讲必要性；其应用间接表遍历不是硬件 OS 页表，已知地址的散读不能算不可并行的指针链。[NDS](https://par.nsf.gov/servlets/purl/10321761) 从维度不匹配处理 marshalling 与带宽利用，[GPS](https://harinimuthukrishnan.net/publication/gps/gps.pdf) 用 publish-subscribe 和弱内存模型合并写，[WIC](https://www.usenix.org/system/files/atc25-zhang-jiajian.pdf) 用 warp 级中断替代轮询。原证据卡见 L3。本稿不把“只能 PIM/NIC”写成结论；只能写：对真串行地址依赖，已见 PIM/近 NIC/近内存解链方案，其他预测、helper thread、多链并行仍需逐案验证。

[返回总目录](../README.md)
