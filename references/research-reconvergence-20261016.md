# 重新立题：决定性原文的新增核读

2026-10-16，RECONVERGENCE-1；研究结果见[七段报告](../iommu-platform/docs/research-reconvergence-20261016.md)。这是对已有文献库的**局部核读增量**，不是新系统综述或全文全部核读标记。没有修改旧catalog阅读等级，也没有把作者性能结果算成本项目复现。

| 原文 | 本轮实际范围 | 决定性认识与限制 |
| --- | --- | --- |
| [Power, Hill, Wood：Supporting x86-64 Address Translation for 100s of GPU Lanes，HPCA2014](https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf)；[本地PDF](papers/gpummu2014.pdf) | Astra席及root直接读PDF p7，§4.4/Fig9；Astra席渲染核页 | 已有共享walk FSM＋outstanding address/state buffer；不能称续行/单执行器本身新颖。未把本文称已实现本平台TC借用或多domainQoS。使用作者此PDF；旧summary另一链接`binhpham-hpca14.pdf`未作本论文同一原件依据 |
| [Shin等：Neighborhood-aware address translation for irregular GPU applications，MICRO2018](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)；[本地](papers/neighborhood2018.pdf) | Astra席直接读PDF p6/印刷357，§IV-A/B、Fig7，并渲染 | pending buffer的PA/PL/CL与上层响应推进partial walk已有前史；原64B PTE行条件不是本平台8B读的免费能力 |
| [Pratheek, Jawalkar, Basu：Improving GPU Multi-tenancy with Page Walk Stealing，HPCA2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)；[本地](papers/walk_stealing2021.pdf) | Astra席PDF pp5–6，§V–VI，印刷630–631；渲染p5 | 具体DWS按完整walk结束借用walker；这个边界差异不能逃过GPUMMU的更早多线程结构 |
| [Shin等：Scheduling Page Table Walks for Irregular GPU Applications，ISCA2018](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)；[本地](papers/walk_scheduling2018.pdf) | Astra席PDF pp6–7，§IV，印刷185–186；渲染p7 | 空闲walker选pending、instruction分组及PWC引用保护已有；instruction ID不免费等于DMA任务ID |
| [Li, Wang, Tang：Orchestrated Scheduling and Partitioning，DAC2023](https://libingyao.github.io/assets/files/DAC2023.pdf)；[本地](papers/orchestrated2023.pdf) | Astra席PDF p4，§IV | TB压力调度/分区与set sharing；不是本PN实板反作用的完整解释 |
| [Costa, Martins, Pinto：Beyond the Bermuda Triangle of Contention，RTCSA2025](https://arxiv.org/html/2508.19670v1)；[本地](papers/bermuda2025.pdf) | Astra席PDF p9，§V-C | 提出QoS/static mapping等但未完整缓解实验；不能据此说全领域无缓解 |
| [ShareMMU，MICRO2026作者入口](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted)；[本地](papers/sharemmu2026.pdf) | Astra席PDF pp7–8，§V-C1/2/Algorithm1 | Permission Tracker、AA-LRU/access mask限制宽泛跨设备共享/权限主张；没有重新核全部安全与失效证明 |
| [MASK，ASPLOS2018](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)；[本地](papers/mask2018.pdf) | Sol席PDF p8，§5.4；root本轮复用该席原文证据及此前已核章节 | Golden/Silver/Normal、翻译优先及应用配额已有；不等于现有MIG免费实现跨ID返回选择 |
| [AXI-REALM v2](https://arxiv.org/html/2501.10161v2)；[本地](papers/axi_realm2025.pdf) | Sol席和root直接读PDF pp3–4，§III-A1/A2 | 1..256粒度透明分段、原metadata及R.last重构已有；原生IOMMU_COMP纯读接线仍属要收费的适配 |
| [Aromolo等，RTAS2026](https://retis.santannapisa.it/~giorgio/paps/2026/Aromolo-RTAS26.pdf) | Sol席作者PDF pp6–8/印刷280–282，§V/VI | 队外控制、有限chunk、同步与任务分析已有；不是本平台逐PTE硬界 |
| [CTC，NOCS2009](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf) | Sol席作者PDF pp4–6，§3–4 | receiver可eject的端到端credit已有；不让PTE越过MIG原顺序 |
| [IDYLL，MICRO2023](https://par.nsf.gov/servlets/purl/10492257)；[本地](papers/idyll2023.pdf) | root PDF pp6–7/印刷1168–1169，§6.2–6.3；另读p4方法条件 | in-PTE目录/IRMB及保持即时TLB失效；不能把惰性PTE写回误称不撤权，更不等于scope已承诺DMA退休 |
| [HATRIC，ISCA2017作者稿](https://arxiv.org/abs/1701.07517)；[本地](papers/hatric2017.pdf) | root PDF pp5–6，§4.1–4.2 | co-tag、粒度/面积权衡及缓存一致性扩展；CPU/虚拟化结果不自动成为I/O回收实测 |
| [RISC-V队列规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html) | root直接读IOFENCE/PR/PW及精确失效部分，和已存原文交叉核对 | 失效对象、完成前缀、已处理/未抵达请求不同；没有推断本机已经实现PR/PW |
| [Intel VT-d Rev5](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf) | root读既有官方全文提取§6.5.2.4/5、§6.5.4；网络PDF因大小获取失败，使用已存原件 | 已有domain/page/PASID选择失效与相关drain；未核到逐范围内部退休成本，不能声称其必定全局停顿 |

2015年scalable IOMMU/rIOMMU及mapping相关工作本轮仅复用已有核读；本轮没有再次全文阅读。服务席另核US7480784B2 Fig10的IOMMU独立请求/响应VC，作为公开技术前史，非同行评审性能结果；具体原文/源码位置在[服务证据](../iommu-platform/out/research-reconvergence-20261016/service-candidate.md)。定向检索见到但未用于裁决的资料不升级为覆盖结论。

**读取完整性。**局部PDF结构预检共同出现UNAVAILABLE（缺pypdf），不是PASS；Poppler提取成功，Astra席对四个决定性页面另作渲染。其七篇sidecar字段/sha保存在[续行证据§8](../iommu-platform/out/research-reconvergence-20261016/translation-candidate.md#8-可導航原始入口讀取完整性與執行界線)，root IDYLL/HATRIC预检在[本轮目录](../iommu-platform/out/research-reconvergence-20261016/literature/)。不因成功提取某页而认证全文结构、题录全部别名或所有引用正确。

**新意边界。**直接原文覆盖、独立普通适配和候选同付费接口消融分别记录；后两者不伪装成已发表的完整同名系统。本文不证明领域查新穷尽，不把未读或未找到的内容当作不存在。
