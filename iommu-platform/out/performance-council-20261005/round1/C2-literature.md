# C2 · 性能讨论 R1 原始来源与读取边界

2026-10-05。定向近邻核查，非系统综述；没有穷尽检索或“无先例”结论。以下页码为 PDF 页序，从 1 起；解析行号仅供定位。没有复制大段原文。

## P-L1：MASK

- primaryURL：https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf
- 题目：MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency，ASPLOS 2018。
- 先前已核第 2 页概述和第 8 页调度。本轮补读第 5–7 页：§4.2、§4.3、§5 概述、§5.2；解析 L546–618、L718–729、L803–848。
- 确认范围：共享 TLB 竞争会带来不同 miss 率；翻译请求与数据请求在共享存储层次相互影响；TLB-fill token、L2 bypass 与地址空间感知内存调度组成联合设计。§5.2 的 token 控制依据 epoch 间 miss 率变化。
- 使用边界：足以反对“首次联合缓存与翻译服务”的宽声明；未由这些段落确认“固定条目分区 × hit-under-miss × 完成驱动 DMA”的整个交互已被解决。论文里的 bypass 含义必须逐处区分，不直接等同本地 N。

## P-L2：Improving GPU Multi-tenancy with Page Walk Stealing

- primaryURL：https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf
- 题目与作者：B. Pratheek、Neha Jawalkar、Arkaprava Basu，HPCA 2021，DOI 10.1109/HPCA51647.2021.00059。
- **读取状态受限：直接获取 PDF 多次失败；实际读到的是搜索系统从该作者机构 PDF 抽取的摘要与引言（印刷第 626 页），不是完整 PDF。**
- 可确认的窄结论：作者将共享 walker 指为多租户干扰源，特别说明高 walk 强度租户可占用 walker，使另一租户等待；提出 dynamic page walk stealing，做 walker 软分区与借用。
- 不可据此宣称：已核清其全部算法、缓存分区实验、队列/互连模型及消融；或者它未覆盖当前候选。若候选最终涉及 walker 资源分配，应优先补读该全文，先于新增更远文献。

## P-L3：Supporting Address Translation for Accelerator-Centric Architectures

- primaryURL：https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf
- HPCA 2017，12 页。本轮复核第 6 页 non-blocking design（解析 L569–576），连同 private TLB 容量/访问延迟讨论 L517–555；先前已核 shared TLB 的相关段落。
- 确认范围：设计明确支持 hit-under-miss；容量、访问成本与加速器访问特征存在取舍。
- 使用边界：N 的抽象机制不是新的；未由这段断言小 IOMMU、分区和有限 DMA 供给下所有性能交互已有定论。

## P-L4：Open Versus Closed: A Cautionary Tale

- primaryURL：https://static.usenix.org/events/nsdi06/tech/full_papers/schroeder/schroeder_html/index.html
- 会议记录：https://www.usenix.org/conference/nsdi-06/presentation/open-versus-closed-cautionary-tale
- 作者：Bianca Schroeder、Adam Wierman、Mor Harchol-Balter；NSDI 2006。
- 已直接读官方全文 HTML 的摘要、§1–3 和 §4.1，解析 L24–145；未宣称本轮审完后半篇推导。
- 确认范围：完成后 think time 的闭环与独立到达的开环不同；即便保持负荷/需求等比较条件，调度效果也可能相差很大；包括 partly-open 模型，并明确闭环负荷受多因素影响。
- 使用边界：闭环影响调度评价是已有知识；文中的服务器案例不直接证明本地 DMA gap 改善的原因。也不能从该文推出开环必然是真实或唯一公平的工作负载。

## 复用来源，未在本轮重新铺开

- Scheduling Page Table Walks for Irregular GPU Applications，ISCA 2018，https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf ：前轮直接核过 §IV，第 6 页，根据指令分组/估计工作选择 walk 的相关段落。说明带成本感知的 walker 调度已有；候选需要它时再补最邻近细节。
- 既有核查记录：[第一次讨论 R2 文献](/home/xvhaocheng/work/iommu-platform/out/paper-council-20261005/round2/C2-literature.md)。
- 本轮事实输入：`/home/xvhaocheng/work/iommu-platform/out/performance-council-20261005/BRIEF.md`。不把共同材料里的已验收事实说成 C2 又执行了一遍。

本轮没有使用二手综述、其他席位答复或模型共识替代原始文献证据。尚缺的精确差异写为待核，而不是新颖性确认。
