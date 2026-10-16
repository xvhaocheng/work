# Scheduling Page Table Walks for Irregular GPU Applications

稳定ID：`walk_scheduling2018` · 类型：`research_paper` · 主题：`gpu`

作者：Seunghee Shin; Guilherme Cox; Mark Oskin; Gabriel H. Loh; Yan Solihin; Abhishek Bhattacharjee; Arkaprava Basu；年份：2018；出处：2018 ACM/IEEE 45th Annual International Symposium on Computer Architecture (ISCA), pp. 180-192。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：IOMMU 内 SIMT-aware page walk 调度：优先服务同一条最近已调度 SIMD 指令的 walk（时间批处理），否则选 score 最低者（预计完成该指令全部翻译所需访存数，1-256）；每条 walk 请求携带 20-bit instruction ID；含防饿死老化机制与 PWC 2-bit 饱和计数器。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：IOMMU 走与 CPU 共享的 x86-64 四级页表（HSA/SVM 前提）；IOMMU 含两级 TLB、8 个 PTW、256 项 IOMMU buffer、PWC 缓存前三级；基线按 FCFS 取请求；score 在请求到达时估算并用 PWC 状态近似，允许不精确；老化阈值设为 200 万个请求；PTW 以 cache line 粒度访存；负载为 OpenCL/HCC 不改动程序

限制：纯模拟（execution-driven gem5 + 自建 IOMMU 模型），无实机测量；内存足迹仅几百 MB；明确因仿真时间不可行而未评估大页；不涉及多租户隔离、ASID/权限、跨设备失效、远端 PTW、尾延迟；score 为近似值，PWC 内容变化会使估算失准

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)
- [来源2](https://doi.org/10.1109/ISCA.2018.00025)
- [来源3](https://www.cs.yale.edu/homes/abhishek/sshin-gcox-isca18.pdf)
- [来源4](https://www.csa.iisc.ac.in/~arkapravab/papers/GPU_page_walk_scheduler_ISCA_18.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/walk_scheduling2018.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.pdf) · SHA-256 `a559d550ee41d30e53f057d32154234dd4ba8bbfb22d89699c812f5fce4b3ef1`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:21712](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt:16](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt:203](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt:436](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt:640](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt:745](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt:828](../../iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:28](../../iommu-platform/docs/research-landscape-20261005.md)：

第二阶段是多加速器共享与 I/O 软件控制面：[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)、[Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 研究 GPU 多应用/多租户下共享 TLB 与 walker 的干扰，说明增加 TLB/walker 不自动消除竞争；[rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)、[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)、[IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) 则把 I/O 路径的 IOVA 分配、失效批处理和 IOTLB miss 暴露为一阶成本。已解决的是：软件 IOVA 管理、ring 语义、批量/延迟失效、hugepage mapping 都有可测收益。新条件使原假设吃紧：10–40 Gbps 到 100–200 Gbps、单租户到多租户、性能目标到时间可预测性与不可信设备安全共享。

[iommu-platform/docs/translation-continuation-frontier-20261008.md:47](../../iommu-platform/docs/translation-continuation-frontier-20261008.md)：

HPCA21 DWS（[PDF](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)）p1/p4：owned walker 经 idle stealing 使 owner 新请求至多等一个 foreign walk，并未把整个 nested walk 等同一笔 PTE。ISCA18 Scheduling Page Table Walks（[PDF](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)）：按整体剩余 work 与同批调度。二者支持“续走/剩余工作调度已有前史”，不替代具体普通 checkpoint/restart/PWC 构造。

[iommu-platform/docs/composition-history-isolation-20261007.md:206](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

[Scheduling PTW，ISCA2018](https://www.csa.iisc.ac.in/~arkapravab/papers/GPU_page_walk_scheduler_ISCA_18.pdf)已依据指令完成所需工作量调度并批处理翻译；[BIS，ASPLOS2012](https://users.ece.cmu.edu/~omutlu/pub/bottleneck-identification-and-scheduling_asplos12.pdf)依据其他线程等待识别关键瓶颈。因此A的宽泛“帮助阻挡别人的工作”并不新。本轮BIS网页全文超时，原文检索及既有本地全文可核；没有把网页超时写成已取得新全文。

[iommu-platform/docs/paper-scenarios-mitigation.md:171](../../iommu-platform/docs/paper-scenarios-mitigation.md)：

| Shin 等，ISCA 2018，*Scheduling Page Table Walks for Irregular GPU Applications*：[作者 PDF](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[正式 DOI](https://doi.org/10.1109/ISCA.2018.00025) | 按 SIMD 指令所需翻译工作量调度并批量服务其 walks，考虑页数及 page-walk cache 后的剩余访问。以翻译工作量替代数据字节量并非空白；独立 DMA 的服务与完成语义差异仍须转化为具体算法差异。 |

[返回总目录](../README.md)
