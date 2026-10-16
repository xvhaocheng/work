# C2 R2：定向原始来源核查记录

日期：2026-10-05。范围是交叉质疑涉及的关键近邻，不是系统综述；“未看到”不是“没有”。只核对所列关键段落。R1 的 Fast & Safe、ATC15、ShareMMU、RISC-V 固定版规范与 Bermuda 证据见 [R1 文献](../round1/C2-literature.md)。下面页码从 PDF 第 1 页起；网页行号是本轮解析器位置，供定位，网页变化后可能不同。

## R2-L1：PASID granularity resource control for IOMMU

- 原始公开文本：[US20230409197A1](https://patents.google.com/patent/US20230409197A1/en)，公开日 2023-12-21。
- 已读摘要、资源描述符与实施细节关键段落，解析 L190–203、L536–547。
- 已确认：PASID/域关联的翻译资源配额、IOTLB 与 page-request 资源控制；内存 QoS 描述符，PASID/context table 中的指针/关联。部分 page-request 指 PRI 类接口，不能偷换为全部内部 PTW 请求。
- 对 C1-A：按身份做配额不是新意；“取得归属描述符前的共享成本如何保证”在所读段落中未得到精确答案。不是认定无人做过，也不作专利法律分析。

## R2-L2：MASK — Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency

- [ASPLOS 2018 作者机构原文](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)，16 页。
- 已核对第 2 页的多层机制概述与第 8 页 Golden/Silver/Normal queue 及 DRAM 控制，解析 L894–946。
- 已确认：TLB fill token、L2 cache bypass、地址空间感知 DRAM 调度；page walk 请求有特别优先路径，兼顾数据请求。
- 边界：不直接对应 RISC-V 冷 DDTC 的前置归属，也不凭这些段落断言覆盖 walk 内抢占；它足以否定“首次跨翻译和内存联合调度”的宽口号。

## R2-L3：Scheduling Page Table Walks for Irregular GPU Applications

- [ISCA 2018 作者镜像原文](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)，13 页。Yale 镜像本轮直开失败，改用作者镜像。
- 已核对第 6 页 §IV，解析 L576–604：walker 空闲时选择待处理请求；按 SIMD 指令聚合，并按估计剩余访存数排序。
- 对 D3/C1-B：翻译成本估计和有感知的 walk 选择已有；所读段落没有确立 C1-B 的 PTE 返回点 continuation 切换。检索不到 preempt 不是缺少抢占机制的完整证明。

## R2-L4：SQUASH: Simple QoS-Aware High-Performance Memory Scheduler for Heterogeneous Systems with Hardware Accelerators

- [CMU 原始技术报告](https://users.ece.cmu.edu/~omutlu/pub/squash_heterogeneous_memory_scheduler_cmu-safari-tr15.pdf)，2015-03-18，20 页；这里引用技术报告，不把它冒充其他会议定稿。
- 已核对第 1 页概述、第 8 页 §4.1/4.2，解析 L573–607。
- 已确认：按当前/预期进度判断紧迫度；短截止期加速器的 urgent period 取决于剩余请求和最坏服务成本，并考虑已在途请求。
- 对 D3：是 deadline/remaining-work 调度的强简单近邻，不是声称它已给出同一 IOMMU 算法。DASH/后续实时控制工作是补充线索，本轮没有逐篇核查。

## R2-L5：IOTLB-SC: An Accelerator-Independent Leakage Source in Modern Cloud Systems

- [作者 arXiv 原文](https://arxiv.org/pdf/2202.11623)，14 页。
- 已核对第 6 页威胁设置，解析 L501–519；第 9 页 §6 设备侧攻击，L855–874；第 10 页 Fig.6；第 12 页防护讨论。
- 已确认：外设之间经共享 IOTLB 的时序侧信道，包含不同 VM 中 GPU 数据库与 FPGA 攻击端，设备用户态 DMA 与计时能力。
- 对 D2/A1：泛称首次 IOMMU 侧信道不可行；未据此证明“容量分区后共享 PTW/控制器的残余通道”与论文完全相同。没有从当前 FPGA 窗口数据推导信息容量。
- 补充线索：[DevIOus 作者机构记录](https://researchconnect.suny.edu/en/publications/devious-device-driven-side-channel-attacks-on-the-iommu/) 仅核对机构摘要，未读全文，不能升级为全部机制已验证。

## R2-L6：官方 RISC-V IOMMU reference model

- [官方仓库 README](https://github.com/riscv-non-isa/riscv-iommu/blob/main/iommu_ref_model/README.md)。本轮直接读主分支页面，未固定 commit，不将其等同当前本地代码版本。
- 已核对解析 L195–205：库、测试范围，以及向模型/实现提供同刺激并比较结果的用途。
- 对 A1-C4：普通参考模型差分验证不是空白；不能据 README 推出它已处理所有非确定性/异步撤销允许结果，也不能说本地已编译可用。

## R2-L7：Standalone verification of IOMMU with virtualization supporting

- [原始期刊页面](https://www.mathnet.ru/php/archive.phtml?jrnid=tisp&option_lang=eng&paperid=424&wshow=paper)，A. A. Petrykin、I. A. Stotland、A. N. Meshkov，Proceedings of ISP RAS 31:3 (2019), 77–84；DOI 10.15514/ISPRAS-2019-31(3)-7。
- 只读原始摘要，提及 Elbrus IOMMU、动态页表、参考模型与 testbench。未读全文；不能声明其 oracle 已覆盖所有异步竞态。

## R2-L8：The Synchronization of Periodic Routing Messages

- [Floyd/Jacobson 原始论文](https://ee.lbl.gov/papers/sync_94.pdf)，1994 年论文版本。
- 已核对摘要/主旨：周期过程相互作用产生同步，以及引入随机化的抑制作用；该 PDF 网页文字提取较差，不引用不可靠解析行号。
- 对 A1：这是结构近邻，说明周期同步和抖动处理本身不是新意；不是当前 DMA/仲裁路径的机制证据，更不能代替本平台的因果实验。

## R2-S1：本地 PR/PW 支持的关键边界

- 直接读取 `/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:348–385`。
- 第 362 行原注释：`TODO: Add IOFENCE support with PR and PW using R/W counters`；后续分支可见 AV 写回与 ATS invalidation 等待。
- 严格结论：不能把当前 branch 的 IOFENCE 执行当成已验证 PR/PW 排空能力，更不能用 172 项既有验收证明 C2-L 安全复用契约。没有在本轮检查所有互连/driver 完成路径，所以不凭 TODO 宣布完整系统存在可利用安全缺陷。

未验证的线索继续保留：PREM/实时 DMA 预留与 C2-T 精确契约；高扩展类别映射是否已有与 C1-A 同构的前置归属规则；异步设备撤销的允许结果集合 oracle 是否已有精确方法先例。后续应按最终候选定向补查，避免无边界铺开文献。
