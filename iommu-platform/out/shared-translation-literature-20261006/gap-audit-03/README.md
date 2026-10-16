# 剩余覆盖与收敛审计（2026-10-06）

用户再次问“是否还需要补充”。结论：需要定向补应用路径、接口语义与关键全文；现在可以开始候选—强前作矩阵，不必等全领域穷尽才推进。主123项目录及上一轮20项清单保持，以下为独立补充来源。

## 本轮核实的来源

| 来源 | 用途及边界 | 本地资料 |
|---|---|---|
| [VIO / To PRI or Not To PRI, That's the question（OSDI 2025）](https://www.usenix.org/conference/osdi25/presentation/wang-yun) | 设备直通、I/O缺页与回收的直接近邻；VirtIO描述符探查/影子队列先处理缺页，不能泛化为任意设备均已解决翻译干扰 | [PDF](papers/vio2025.pdf) · [文本](papers/vio2025.txt) |
| [BaM（ASPLOS 2023）](https://github.com/ZaidQureshi/bam) | GPU发起存储访问的真实应用/公开实现入口；不是共享IOMMU翻译瓶颈的直接证据 | [作者稿](papers/bam2023.pdf) · [文本](papers/bam2023.txt) |
| [GPUrdma（ROSS 2016）](https://marksilberstein.com/wp-content/uploads/2020/04/ross16net.pdf) | 作者原文搜索索引核到题录与摘要；直接链接重定向为主页，本轮未获得PDF，不能宣称已精读 | [实际下载记录](records/download-batch1.json) |
| [The Landscape of GPU-Centric Communication](https://arxiv.org/abs/2409.09874) | 2026-04-23修订作者稿，初稿2024；用于发现通信库/控制路径，再回到原始工作核实。综述不替代具体机制证据 | [PDF](papers/gpu_centric_survey2026.pdf) · [文本](papers/gpu_centric_survey2026.txt) |
| [Binoculars（USENIX Security 2022）](https://www.usenix.org/conference/usenixsecurity22/presentation/zhao-zirui) | 补页表遍历与其它访存交互/争用的测量近邻；借鉴观测方法，不将侧信道放大量当正常工作负载减速，也不因此把课题转向安全 | [PDF](papers/binoculars2022.pdf) · [文本](papers/binoculars2022.txt) |
| [NVIDIA GPUDirect RDMA官方文档](https://docs.nvidia.com/cuda/gpudirect-rdma/index.html) | 核对注册缓存、撤销回调、内存顺序与IOMMU条件；是正式技术资料，不计作新增论文 | [检索摘录](web-4.txt) |

新补5篇论文/综述来源中4篇全文成功保存，GPUrdma仅题录/作者原文索引；另1份官方文档。这里只核题录、摘要与必要关键段，未完整精读/复现4篇。

## 会改变研究决策的约束

1. NVIDIA本次打开的GPUDirect RDMA文档§3.4明确要求相关PCI设备看到同样物理地址，并限制为1:1映射/直通。因此GPU–NIC通信存在，不等于其经过候选假设的共享IOMMU翻译结构。该约束按文档所述路径使用，不推广成所有GPU/互连架构的永久限制。
2. 同文档§3.1–3.3已经描述延迟解除固定、注册缓存和撤销回调；必须区分注册记录、GPU BAR映射、IOTLB与设备ATS缓存，不能把各层生命周期混为同一种翻译副本。§3.7又要求正确的可见性/顺序机制，DMA操作完成和消费者可安全读取不是一句话能等同。
3. VIO的正式论文与§3设计说明已有在设备消费队列前处理页面可达性的路径。若候选涉及“提交前先准备映射/页”，必须与这种软件供给路径比较，而非只对照硬件PTW。

## 剩余补充按四类推进

- **关键全文：** 原19项缺全文，加上轮CATS/Trail/SPMC/Make Every Batch Count；优先查直接威胁对应候选者。拿不到则保留未排除的覆盖风险。
- **真实执行路径：** 选定应用后画出虚拟地址、IOVA、物理地址、设备本地映射各层；核实际转换/缓存/同步位置，随后才判断哪里发生干扰。
- **成熟软件基线：** 注册缓存、预固定、按需分页、运行时依赖、提交与撤销；比较与硬件机制相同成本及语义条件下的能力。
- **测量与评估：** 继续读已得CounterPoint/Virtuoso/Coz等，补必要的争用测量前作；分辨应用进度、翻译等待、内存竞争和软件供给，另行取得轨迹证据。

收敛门槛不是论文总数：对每个拟保留问题，能列出最强直接前作、通用替代基线、真实应用/实际路径、尚未排除的覆盖与验证风险，即可开始矩阵并设计最小判别实验。相邻引用追踪持续只回到已有机制时可以降低广搜优先级，但“没搜到”不是创新证明。无需先证明创新或取得所有轨迹才能开始比较矩阵。

## 角色与验收边界

DeepSeek负责目录覆盖审计：[输入](input.json)、[原答复](worker.md)、[完整响应](worker.json)。Codex执行联网检索、现成下载器与关键语义核查并作收敛决定。[审计](audit.json)保留用量/检查；[下载记录](records/download-batch1.json)保留GPUrdma失败，未重复下载既有全文。

未采纳worker的下列过强表述：题录未见不等于全领域不存在；既有I/O论文不能未经逐篇核对统称CPU发起；GPUDirect RDMA允许数据直达GPU不等于GPU负责发起；不能必须先找到既有同型论文才能建立新问题；不能以未取得真实轨迹为理由禁止建立前作矩阵。worker给出的原文行号未逐项验收，不整体认证为精确引用。

本轮无RTL/Linux变更、构建、仿真、上板或新性能实验；论文作者结果不写成我们的结果。
