# 多 DMA 共享 IOMMU：三方向机制设计与决定性验证计划（§88 · 2026-10-09）

> 归属：Codex root 及两名关键设计席亲自设计/核查；DeepSeek 源码证据和文档起草；Codex 审核保存。路径与证据见文末。

## 0. 一段大白话

我们在问一件事：一条很长的 DMA 拷贝，**读源头的权限能不能随着"实际读完了"一页页自动关掉，把源页尽早还给生产者重用**；还是说普通做法（按静态端点分段 + 游标、逐页 targeted 撤销、多开几页 DDR 做双缓冲）其实已经能做到同样的事。这一轮**只做文献/源码核查和手算**：0 模型、0 RTL/Linux 改动、0 构建、0 上板；旧 §76–78 的 29 次 COPY 结果只作引用，不算本轮验证。下面技术细节较多，前面这段是给非本方向读者看的入口。

任务 V 是受保护任务，任务 A 是竞争者；下文“候选 A/B/C”只是方案编号。AR/R 是读地址与读返回，AW/W/B 是写地址、写数据与写响应，walker 是逐级查页表的硬件。平台仍是 CVA6、四路 iDMA、RISC-V IOMMU、Linux 与 Genesys2；四路 DMA 不等于四路 walker。本轮关注端到端性能与成本，正确性是约束；如果新增合同只增强保护、没有同合同下的性能或成本优势，就不算当前性能主线成功。

## 1. 领域边界

### 1.1 主审边界（root-primary-review）

- [Scheduling Page Table Walks for Irregular GPU Applications, ISCA 2018](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf) (DOI 10.1109/ISCA.2018.00025)：同一 SIMD 指令成组、估计整组工作量、空闲 walker 时选择。**边界**：常规分组/完成优先重命名不算贡献。
- [MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency, ASPLOS 2018](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) (DOI 10.1145/3173162.3173169)：TLB-fill token、旁路、翻译与应用感知内存调度已有。**边界**：单独翻译优先不是新能力。
- [SDM: Sharing-Enabled Disaggregated Memory System, PACT 2023](https://kwanseokchoi.github.io/assets/pubs/pact23-sdm.pdf)：权限与数据推测重叠已有。**边界**：不能据本文宣布未授权数据可直接写出，也不能把 CXL 适用条件当本平台已满足。
- [rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers, ASPLOS 2015](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf) (DOI 10.1145/2694344.2694355)：flat ring / current+next、隐式替换/末尾失效。**边界**：限制在 map/unmap 可预测顺序；有效 DMA 可乱序，不得误写。
- [DMAAUTH: A Lightweight Pointer Integrity-based Secure Architecture to Defeat DMA Attacks, USENIX Security 2024](https://wenboshen.org/publications/papers/DMAAuth.pdf)：按映射身份、范围、方向检查，dma_unmap 先清 RW 并换身份。**边界**：自主"实际进度耗尽"需单独核对，不把普通权限门当新。
- [NVIDIA PTX ISA cp.async.bulk.wait_group.read（PTX8.0，8.4 归档）](https://docs.nvidia.com/cuda/archive/12.4.0/parallel-thread-execution/index.html#data-movement-and-conversion-instructions-cp-async-bulk-wait-group)：read 只等源读取，不含目的写与可见性。**边界**：源完成与全 COPY 完成分离不能作为新贡献。
- [Protection Strategies for Direct Access to Virtualized I/O Devices, USENIX ATC 2008](https://www.cs.columbia.edu/~cdall/candidacy/pdf/Willmann2008.pdf)：single-use / shared / persistent mapping 与软件单次 descriptor 已有。**边界**：共享计数、持久映射不是新。
- [DASH: Deadline-Aware High-Performance Memory Scheduler, ACM TACO](https://people.inf.ethz.ch/omutlu/pub/dash_deadline-aware-heterogeneous-memory-scheduler_taco16.pdf) (DOI 10.1145/2847255)：deadline / progress 感知优先级已有。**边界**：FIFO 前沿换一个 priority 名称不充分。
- [NB-Walker artifact, MICRO 2026 (v2)](https://zenodo.org/records/21488827) (DOI 10.5281/zenodo.21488827)：传递 PID、原地址、层级和 identity 给缓存后释放 secondary。**边界**：只确认代码动作，未跑 artifact、未取全文。
- [Marching Page Walks, HPCA 2025](https://ipoom-jeong.com/publication/marching-page-walks-batching-and-concurrent-page-table-walks-for-enhancing-gpu-throughput/) (DOI 10.1109/HPCA61900.2025.00123)：摘要支持一个 walker 批量并发。**边界**：细节不能据摘要裁决。
- [The DMA Streaming Framework, arXiv 2026 预印本](https://arxiv.org/abs/2603.10030)：现代 buffer lifecycle/ring/credit 集成线索。**边界**：不是已读全机制或同行评审证明。
- [RISC-V IOMMU specification — In-memory queue interface, v20260222](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)：翻译失效不等于数据退休；本机 CQ 源码 PR/PW 仍 TODO。**边界**：新自定义证书不能冒充标准 fence。
- [LMAX Disruptor 官方技术文档, 2011](https://lmax-exchange.github.io/disruptor/disruptor.html)：claim/commit、多消费者最小序号避免覆盖已是普通并发原语。**边界**：区间/游标本身不算新。

### 1.2 Walker/多 context 边界（root walk_preemption_design 直接读）

- [Supporting x86-64 Address Translation for 100s of GPU Lanes, HPCA 2014](https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf)：共享多线程 walker 同时保留多个遍历请求。
- [Improving GPU Multi-tenancy with Page Walk Stealing, HPCA 2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) (DOI 10.1109/HPCA51647.2021.00059)：按租户分配 walker，空闲时借用。
- [XiangShan 官方 L2 TLB 文档](https://xiangshan-doc-en.readthedocs.io/en/latest/memory/mmu/l2tlb/)：上层 walker 遍历到末级前把工作交 Miss Queue；末级多 ID 并发。

### 1.3 端点/翻译驻留边界（root endpoint mechanism 直接读）

- [Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs (ICCD 2018)](https://arxiv.org/abs/1808.09751)：整 DMA 传输所需 TLB 项先锁定、完成后释放，并提逐 burst 退休消除该约束。
- [Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs, DAC 2023](https://libingyao.github.io/assets/files/DAC2023.pdf)：TLB-thrashing-aware 放置 + 划分 + 相邻 set 动态共享。
- [Overcoming the IOTLB wall for multi-100-Gbps Linux-based networking, PeerJ CS 2023](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) (DOI 10.7717/peerj-cs.1385)：共享页 buffer 放连续 descriptor、复用/回收、批处理改善 IOTLB 局部性。
- [Avoiding TLB Shootdowns through Self-invalidating TLB Entries, PACT 2017](https://pages.cs.wisc.edu/~basu/papers/pact2017_final_version.pdf)：translation-validity lease 使项过期。**边界**：replacement-residency lease 不同，绝不能延长 mapping/权限有效期。

### 1.4 未取全文/未完成核查（诚实标注）

- *True IOMMU Protection from DMA Attacks*（ASPLOS 2016）作者 PDF 本轮 fetch timeout，仅保留 bounce-copy 控制，未据未读全文裁决。
- Marching Page Walks、NB-Walker **论文全文未取得**；前者仅摘要，后者另读作者源码。
- SDM 本轮未重新读完评估。

## 2. 三候选机制卡

### 候选 A：未来读集合与未退休读承诺的区间补集回收（条件主线）

**真实场景**：多个 SID（如 V、A）对同一注册源池做长 COPY；A 因应用依赖晚读、V 先推进；CPU 逐页产生下一代数据希望尽早就地复用已读源页。

**新增动作（相对 native）**：把一条长 COPY 授予的**有限源 READ 权限**分解为"尚可产生的读范围 F_i"与"已公开承诺但未完成读范围 U_i"；源页只在无未来读、无未完成读、无未知别名时交还；**源页可交还而目的 W/B 独立继续**；翻译可保留但旧代权限不能复用。

**信号位置**：AR first VALID 前预留 obligation 槽并 commit 进 U（这是必须 before-VALID 记账的关键点）；只有**完整成功真实 R** 才从 U 删除范围；目的 AW/W/B 独立退休；证书放在可背压范围输出队列。

**状态**：reader records（2 项）；obligations（最多 18 项保守方案）；pool 记录 + 4 项证书队列 + 读写指针/计数。

**伪代码（要点）**：

1. ARM：仅登记源范围、映射身份、SID/context、generation 并 seal 参与读者集合；不预先取得普通权限结果，root 没有免费 prepare 接口。普通 IOMMU 权限沿既有路径进行，必须在真实内存访问前成功；固定映射保留到 slow-path teardown。
2. RESERVE/PUBLISH：在本条源请求的首次 publicVALID 之前/同拍预留 obligation 槽（start,end,owner,epoch），同时令 I_i += len，将该范围从未来集合 F_i 移到 U_i。无槽不公开新请求；这一步与授予下一代租约是两个事件。
3. HELD：AR payload/owner 保持不变直到握手；accepted 单独记；不因 gate 前进丢 U。
4. R_RETIRE：只按真实 accepted R beats、匹配 RLAST、数量正确且 success 才 retire U；error/abort 保持 quarantine。
5. RECLAIM_QUERY(page)：页在注册池内、与任何 F_i 和 U_i 都无交集、无未知别名，才原子拒绝旧代新权限并发出带 generation 的证书。
6. ARM_NEXT：首版按整池换代。producer 完成所有下一代源页的实际写入及缓存/内存同步，旧代未来读范围与 U 已空、旧身份不能再出现时，授予下一代租约。授予租约本身不向 U 加读承诺；每条新读仍在第 2 步首次公开时记账。目的 W/B 由原有独立身份继续合法完成。
7. B_RETIRE：目的义务独立完成，之后才报整 COPY 完成。

**A 区间补集合同（准确）**：F_i=[I_i,end_i)，`Q = union_i (F_i ∪ U_i)`；首次 PUBLISH 同时执行 I_i += len 并把该范围从 F_i 移到 U_i，公开 VALID 后稳定。登记源池 P 中的整页 p，只在 p∩Q=∅ 且既有 CPU/DMA 缓存所有权条件成立时交还。不能因 epoch 门通过就当普通权限成功；普通 IOMMU 权限必须另行走既有路径并在真实内存访问前成功。`registered_pool \ Q` 可含**孔洞**，不能只取一个全局最小前沿。前缀 min 只是特例（所有 SID 读同一全范围时 min_i R_i 给出连续前缀）。

**证明要点**：(1) 所有合法未来读包含在 F_i、所有不可撤回未读完请求包含在 U_i，故 P\Q 内无任何未来或未完成源读；(2) PUBLISH 把区间从 F_i 移到 U_i，Q 不因仅发请求而缩小，所以 firstVALID 阶段必须先记账；(3) AR 接收只改 held/accepted，Q 不变，只有完整成功 R 才删 U；(4) 所有读者集在本代封闭、映射静态、CPU 写只在交还后发生，补集才可回收，否则停止发证并回退；(5) 固定 F 个源区间 + 最多 K 个公开未退休区间，Q 端点数 ≤ 2(F+K)，用原端点分段/更新 cursor 即可，**不需要每页一条永久硬件项**。

**字段/端口/额外工作成本（只用 root 18 义务槽参考方案）**：

- reader_records：每 reader IOVA64+begin32+end32+published32+SID24+PSCID20+pool_id8+generation32+flags4 = 248 bit，2 项 496 bit。
- obligations：每项 owner1+generation32+start32+end32+valid/accepted/error 各 1+remaining_beats9 = 109 bit，18 项 1962 bit。
- pool+certificate：pool 记录 base64+length32+generation32+reader_mask2+flags4 = 134 bit；4 项证书各 pool8+generation32+first_page32+page_count16+status8 = 96 bit（共 384 bit）；读/写指针各 3 + 计数 3 = 9 bit。
- **未对齐字段总量 496+1962+134+384+9 = 2985 bit ≈ 374 bytes**。这是给定结构的字段总量，**不是实际面积/净增量/全系统成本**。
- **另有**：软件页表/ownership 元数据（如 2-bit/page 状态 + 4-byte/page generation 即 P*(2+32) bit，不能藏进已有 Linux 页结构称零开销；若只整池换代可省逐页 generation）；spills；端口与未知 pipeline 成本。
- **端口/关键路径**：每公开源请求做 generation、范围、cursor 检查；firstVALID 前完成或打一拍，不把慢区间并集比较放到现有 AR 关键路径。退休判断可逐项扫描 F+K=20 项（每项 1 拍为上界示例）；两个 retire 并发必须仲裁/排队或双写口；并行比较 20 区间需 40 边界比较器 + 归约树，不能免费。
- **复杂度**：硬件可 O(F+K)，但**软件可能 O(P)**，不能声称全系统 O(F+K)。

**权限/背压/顺序/公平/错误回退**：

- 权限：新读 offer 只可由已授予 owner/epoch、下一合法源地址产生；地址合法与普通 IOMMU 权限成功都必要；已公开 U cookie 是原承诺所有权，关闭后不能用新 gate 阻止其合法完成。仅静态注册映射、不可回访、完整读者集合；新代不能在旧 epoch 仍可出现时回绕。
- 背压：源读**不增加数据 FIFO**；若现有缓冲无法保存已读且未写数据，必须背压，**不能先发回收证书再重读源**。保留 native 8 头槽/32 TC 写字及 spill（nonblock 前两 DMA 各 512×64bit=4KiB，其他 3 字）。
- 顺序：源读地址按递增无重叠顺序；firstVALID 后不可取消/换 owner。
- 公平/证书背压及查询：采用已更新的 `root-cost-and-gates.json` 的 `certificate_backpressure` 和 `fairness`。证书/通知 FIFO 满时只允许丢弃通知并置 sticky overflow，不丢 U/权限账、不阻止真实 R 退休；软件不得复用未确认页，但可发起 page-query。查询固定 generation，期间冻结新的 ARM/PUBLISH 以防 F 向 U 转移撕裂；已公开 AXI/R 照常退休；扫描为保守查询，计 20 比较拍上界与 CPU 流量。轮转/age 保证 query 和新 admission 不互相饿死；无外部响应上界时不称 cycle 保证。原生无此接口，费用未测。
- 错误回退：R 错误/许可错误/未知别名冻结证书并合法排空已公开 AXI；原生 NO_ERROR_HANDLING **不能证明**此回退已存在；失效设备无限期不回应时不保证完成。

**信任边界（关键）**：若既有可信 DMA descriptor 接口已保证绝不重访旧前缀且不能任意重编程，**可靠源进度通知本身可能已足够，额外 IOMMU 门可能冗余**；不能靠改变威胁模型（增加安全需求）制造性能主张。须分别比较相同可信 DMA 合同下的最简进度实现，与原本要求逐代访问收回的 IOMMU 合同。

### 候选 B：PTE 返回边界暂停与续走（停止为独立路线；普通 walker 控制）

**真实场景**：A 为持续大范围 4KiB 页 DMA，V 为稀疏到达短 copy；V 到达时 A 已占据唯一 miss walker；V 可能用大页或命中 PWC，只需短剩余遍历。

**新增动作（对 native）**：PTE 返回后暂停 A 剩余遍历，V 先完成；A 保留已完成层级而非从根重走；数据内容及源/目的权限不共享，也不跳检查。

**信号位置/状态**：两个 owned miss context，每个 FREE/READY/AR_HELD/WAIT_R/RESULT；每个 owned context 必须快照所有仍会被使用的 permission/root/context 字段（如 iova、pscid、gscid、en_1S/en_2S、iohgatp 根、is_store 等），不能只存内部 pointer/level。一个单 beat PTE issue engine，初版最多一个在途 PTE；一个结果出口，结果 backpressure 保存在原 context，owned 结果成功必须显式 access_ok 并背压到消费。安全切换点位于原 PTE R 握手且完成有效性/权限/superpage/reserved/RRESP 检查之后、下一个 AR 首次 VALID 之前。

**关键伪代码（沿 walk-critical）**：

1. 只有存在 owned request/context/result 槽和最终返回路由时，才为第二个 miss 分配 owned context；快照所有仍用 permission/root/context 字段，不隐含授权。
2. 若已有 AR_HELD，保持地址、ID、owner 和元数据不变直到 AR 握手；调度器不得覆盖。
3. R 握手后识别准确 owner，完整检查 PTE 与总线错误，再更新该 owner 的 next pointer/stage/level；完成则产生显式 owner-tagged success/error 结果，否则置 READY。
4. 仅当无 AR_HELD 且有发射额度时发射：V_READY 且有服务额度则选 V，否则选最老 eligible READY；只有选中 PTE 的 AR 实际被接收才消耗服务量子。
5. READY 选择使用 V/age 回退；A 年龄预算到期时在下一个未承诺选择点强制一个 A PTE quantum，但不抢占已公开 VALID 或在途内存操作。
6. invalidation 阻止新的受影响 admission，标记受影响 saved context 为 stale；排空已发 PTE 请求，抑制晚返回旧 fill/access_ok，不复活旧 fill，并按既有架构合同重启或失败后再确认失效。
7. 故障/错误按 owner 隔离；结果槽被占用时继续占用原 context 或已计费结果槽，直到 owned 结果被消费。

**资源/公平/故障**：同一规则可由普通 MSHR walker 逐事件复现；两个 context 的共享端口需真实仲裁。资源账沿用 walk-critical：原 PTW 全 q 状态 A+P+171 bit，加仍存活输入最多 187 bit；示例 A64/P56 为 478 bit/context、两项 956 bit，此子集之外还有 owned request/DC-PC/route/result。公平采用 V/age 回退和 per-flow 服务量子；无有界 memory/output 服务时只称仲裁公平，不称 cycle 上界。故障/invalidation 按上述排空与不复活规则处理。

**手算数字（引自 walk-critical.json，假设保持）**：A 在 t=0 到达、V 在 t=1 到达；双方 prep=2 周期，PTE 往返=10 周期，data copy=4 周期并含完整源 AR/R 与目的 AW/W/B；整 walk 原生 C0 (A,V) = 36/46；候选与 C1 = 46/26；C2（两在途） = 36/17。示例取 zero 切换开销，真实模型需扣 δ；M2 同物理端口不增加带宽，两返回 12/13 分拍。**V 尾延迟改善不等于吞吐提升**；邻反例（V 目的叶改 4KiB，需 3 次 PTE）C0 与候选 makespan 均 66，但完成时刻和 102→112，变差。

**停止理由（准确表述）**：因为完整两 context 普通多线程 walker（C1）可以逐请求、逐返回、逐结果**一对一**复现候选（把 saved continuation 命名为 MSHR，收到同一 R 执行同一状态函数，选择器从同一 READY 集选同一 owner），且 HPCA 2014 已有多线程 walker、NB-Walker 2026 作者代码已实际外放 continuation 释放本地槽，**所以不能把裸暂停/续走作为独立贡献**。资源账：原 PTW 全 q 状态 A+P+171 bit，加仍存活输入最多 187 bit；示例 A64/P56 为 478 bit/context、两项 956 bit，此子集之外还有 owned request/DC-PC/route/result。**不为当前规则建模型。**

### 候选 C：动态 COPY 片段切分/合并与有限翻译驻留共同准入（条件备选）

**真实场景**：一批独立小 COPY gather/重分布页驻留对象；源复用与目的复用争有限 IOTLB 工作集；不同 copy 可能属 V/A，减少总 miss 不足以保护双方。

**合法重排边界**：只在**显式无序批**内、且在该 descriptor first VALID 之前重排；源对批不可变；目的字节范围互不重叠且不与源重叠；有序 descriptor、fence、MMIO、生产者-消费者 COPY 界定批；parent 完成仍绑定所有真实 child W/B。

**新增动作**：first child VALID 前，同时选择"哪些字节区间成为下一批 COPY child"与"有限个未来使用的翻译项保留"；决策用**实际成功覆盖与替换压力**，而非仅静态端点名；组可在承诺前缩小/解散。

**状态/伪代码**：最多 5 个 parent COPY 记录 + 合法重排组标注；对每个候选枚举三种长度（普通最大合法 burst/页边界、已授予端点覆盖边界截短、源+目的都连续且同 context/options 且无排序边界的合并）；score = 估计避免 miss 服务 − 额外 child/frontend 服务 − 计费驻留机会成本；grant 至多 K_lease 替换项、B_lease 未来字节、年龄界 T_lease，**原子整组获取或全不获取**；commit 恰一个已选 child 的不可变请求记录后再发 firstVALID；child 真退休后更新 parent 完成字节/error 与竞争者欠额，释放无剩余可见收益的租约并重算。

**score 单位（以 root-cost-and-gates 纠正为准）**：机会成本须与其它 score 项单位一致——用**无量纲服务欠额权重 q 乘预计持有时间 tau（cycles）**，即单位是 dimensionless q × hold_cycles；**不能**把以 cycles 计的 deficit 再乘 cycles 当 cycles 减。q 取已观测值，tau 上界 T_lease。此评分仍为**待验证启发式，不称新定理**。

**成本（引自 endpoint-design.json）**：N=5 parent 需 5*(2A+L+O) bit，另有 valid1+owner2+ID3+age8+status3+offsetL+count ceil(log2(H+1)) = 每项 17+L+ceil(log2(H+1)) bit，保留当前+完成两 offset 再加 5L；E 个 IOTLB lease 项 E*(lease_valid1+lease_owner3+budget_width+expiry_width)（共享 owner 集改 E*5 或引用结构）；顺序扫 5 项两端点最多 10 次 lookup，并行需真实端口/比较器；所有 child R/W/permission/last/B 及 parent 归属收费。

**权限/背压/顺序/公平/错误回退**：驻留只影响替换而非权限；新权限仍特定于 context/operation/range/当前失效 epoch，绝不把读权限授权写；invalidation 撤销 offer 并按架构完成规则阻止复用，已接受 AXI 合法完成；all-or-none 有界租约准入避免循环部分预留，永远为未租用 miss 保留容量或抢占路径；错误可留部分数据（普通 DMA 错误合同），不声明原子全或无。

**重要纠正**：完整 COPY 的 pre-VALID 重排**本身不强制增加 4 个 active backend 或改变 AXIID**（仅 firstVALID 前选一个完整 COPY 顺序执行可保留现有 ID 与一个 active legalizer，但仍需描述符窗口与逻辑结果归属）；**只有真正多 parent child 交错才另收 parent 状态/完成路由费**。

## 3. 强对照、手算正例与邻反例（每个完整可执行对照）

### 3.1 候选 A（主手例只用 root 单服务器轨迹）

**共同前提**：一台共享服务服务器；每个 4KiB 源页 READ、目的页 WRITE 或 CPU 页 WRITE 各占 1 单位，不重叠；DMA 页服务单位含该页普通权限与实际传输预算；CPU 每页另需 1 单位计算，可与 DMA 服务重叠；共同初始登记/descriptor/映射成本 H 另加；下一代提交/授权 1 单位明确加入。**这是归一化服务时间，不是平台 cycle 或成功实验。**

| 控制 | 旧 V 完成（扣共同 H） | A 完成（扣共同 H） | 下一代 V 完成（扣共同 H） | 整批完成（含共同 H） |
|---|---|---|---|---|
| candidate | 10 | 14 | 24 | H+24 |
| weak global-prefix | 8 | 14 | 30 | H+30 |
| whole-source range，先服务 A | 8 | 14 | 26 | H+26 |
| whole-source range，先服务 CPU | 8 | 15 | 25 | H+25 |
| ordinary 同合同 cursor | 10 | 14 | 24 | H+24 |

**完整可执行强对照简表（A）**：

| 强控制 | 可执行动作 | 不得弱化/计费 |
|---|---|---|
| 可信 single-use DMA 仅进度 | 可信 descriptor 保证不重访旧前缀且不可重编程，只暴露真实源进度；不新增 IOMMU 门 | 允许自动 descriptor 生成；DDR/SRAM/CPU 分类型计费 |
| 静态 reader 端点分段 cursor | 按静态 reader 端点切段并维护每段 cursor/真实 R 退休 | 允许合并通知/失效与已有并行；同完整新合同时只能否决额外复杂规则或独有速度主张 |
| whole-source + 正确范围引用 | 整条源完成并结合正确范围引用释放，不必等 A | 同上，不得把替代控制写成弱化版 |
| 逐页源完成 + 预装门/定向撤销 | 每页源完成即用预装权限门和定向撤销回收 | 允许自动 descriptor 生成、合并通知/失效、已有并行 |
| rIOMMU 多 ring | flat ring/current+next 多 ring 顺序 map/unmap | 相同真实资源；DDR/SRAM/CPU 分类型收费 |
| 额外源页双缓冲/bounce-copy | 用额外源页缓冲隐藏生产者延迟 | 真实占用 DDR/SRAM/CPU 流量；允许已有并行 |
| 普通 cursor 实现同一完整新合同 | 同一合同用普通 cursor 实现 | 不等于已有发表覆盖该完整合同；只能否决额外复杂规则或独有速度主张，不能自动归档完整合同 |

- **candidate schedule**：0–1 V R(P0)；1–2 V W(D0)；2–3 V R(P1)；3–4 V W(D1)+CPU 算新 P1；4–5 CPU W(P1)；5–6 V R(P2)；6–7 V W(D2)+CPU 算新 P2；7–8 CPU W(P2)；8–9 V R(P3)；9–10 V W(D3)+CPU 算新 P3，旧 V 在 10 完成；10–11 CPU W(P3)；11–12 空闲；12–13 A R(P0)；13–14 A W(E0)+CPU 算新 P0，A 在 14 完成；14–15 CPU W(P0)，下一代 4 页就绪；15–16 提交/授权；16–24 完成 4 组源 R/目的 W，下一代 24 完成。
- **具体可执行 whole-source range 对照**：旧 V 在 0–8 执行、源 R 在 7 完成。若保 A 在 14 完成，CPU 的 compute/store 为 P1:7–8/8–9，P2:9–10/10–11，P3:11–12/14–15（A 的 R/W 占 12–14），P0:15–16/16–17；下一代 17–18 提交、18–26 完成，结果 **(8,14,26)**。若改让 CPU 在 12–13 先写 P3，A 的 R/W 延至 13–14/14–15，P0 在 14–15 计算、15–16 写，新 V 在 25 完成，结果 **(8,15,25)**。这是两种合法取舍，不声称全局最优。
- **算术修正**：初稿的 (8,14,25) 在 P3 尚未 store 时就开始计算 P0，违反已声明的逐页串行 producer；最终只采用上面两条合法轨迹。详见[修正记录](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-arithmetic-correction.json)。
- **普通同合同 cursor（最强对照）**：普通按静态 reader 端点切段（P0 与 P1..P3）+ 每段 cursor/真实 R 退休，**可逐步生成完全相同的 24 轨迹**。

**必须同时报告**：候选、保 A 的 whole-source、先 CPU 的 whole-source 三项完成时刻之和都为 **48**。候选整批完成 24，两个普通范围控制为 26/25，但旧 V 首项从 8 延到 10；它调整了完成次序，不能说每个任务都更快。普通同合同 cursor 也为 24。共同 H、真实权限和调度开销必须加入；不把弱 global-prefix 的 30/24 包装成平台 25% 收益。

**全工作计数**：3 个软件 COPY descriptor（V 旧 4 页、A 旧 1 页、V 新 4 页），9 页真实源 READ 与 9 页目的 WRITE，各 4608 个 64bit data beats；CPU 额外 4 页 WRITE 共 2048 beats；按 64bit/256beat 最大 burst，18 源 AR/R burst、18 目的 AW/W/B burst、36 次普通地址/权限请求。**实际 PTE 访存与上下文获取取决于 cache/mapping，未来必须按 trace 计算，不能从 36 请求直接称 36 miss。** 容量：每次 DMA 页 R 后紧接 W，最多保留 1 页载荷；不借多页 FIFO；CPU 生成数据写回已可回收源页，另需真实 4 页注册源池；不同源 IOVA 同 PA 不共享权限检查。

**明确哪个正例被哪个强对照复现**：普通同合同 cursor/静态端点分段**完整复现 candidate 的 24 轨迹**；因此该正例**没有独有系统收益**。相对范围正确 whole-source，保 A 时差 2 单位，允许 A 晚 1 时差 1 单位；普通同合同 cursor 完全持平。

**邻反例**：(1) 仅把 A 源范围 P0 改为 P0..P3 并让它晚读——这些页仍在 A 未来集合，Q 阻止提前回收，优势消失；(2) 仅要求 CPU 必须先完成新 P0 才能生成新 P1——直到 A 读完 P0 才可开始，端到端优势消失；(3) 仅令旧 V B 晚于所有方案 producer 就绪时刻——原生 WAIT 挡住下一代，源回收改善被 B 瓶颈覆盖；(4) 给对照同类型预算下足够额外源页双缓冲，或给出同成本已封闭区间 cursor 合同——若完全复现或更快，停止独有性能宣称。

**生产/暂存假设未实测**：轨迹假定 CPU 同一 producer 串行执行每页 compute 随后 page-store，允许页间改序但无预生成整页隐藏暂存；若 CPU 能在源页释放前算好并暂存，应让对照使用真实 CPU/cache/DDR 资源并重排，可能消除上述小幅完工差。**此假设尚无真实工作负载测量，不能包装成决定性系统独有正例。**

> 另一套服务假设的手推保存在[生命周期重设计卡](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/lifecycle-redesign.json)，未用于本方案主表。

### 3.2 候选 B（数字引自 walk-critical.json，假设保持）

完整可执行强对照：C0 原生整 walk；**C1 两 context 普通多线程 walker，单 PTE 在途（同 context 字段/读写端口/在途额度/结果槽与发射规则）**；C2 两 context、两 PTE 在途（A 等 R 时发 V 的 AR，ID/owner tag 分别路由）；C3 两个完整 walker + DWS 式借用；C4 前移准备与页表缓存。

- 正例（对 C0）：C0 (A,V)=36/46；候选与 **C1=46/26**（完成时刻和 82→72，V 延迟 45→25，A 延迟 36→46，makespan 46 不变；sum(solo/shared latency) 1.356→1.423，仅说明调度取舍，非吞吐翻倍）。**C1 逐周期完全相同 → 无独有系统收益**。
- M2 对照：给双方合法 M=2 与两返回槽，普通 C2 在 A 等待首 PTE 时 t=3 发 V，V 13 返回、17 完成；A 12/22/32 三 PTE、36 完成；makespan 36、完成时刻和 53（比 46/72 更好）。**若候选也在 A 等 R 时发 V，就正是可执行 C2**。
- 邻反例：V 目的叶改 4KiB（3 次 PTE），C0 与候选 makespan 均 66，完成时刻和 102→112，**变差**——保护 V 不能自动推出 V/A 联合改善。
- 条件界：A 剩余 r 个 PTE、每次 ≤L、切换开销 δ，在该次 A 响应到达后转 V 可最多消除 (r−1)L 的 V 前置阻塞，付 δ；但 V 必须有真实 context/result credit，且 C1 同取得此界；没有有界 memory/output 服务时不能宣称绝对 cycle 上界。

### 3.3 候选 C（数字引自 endpoint-design.json，假设保持）

**静态手例共同起点**：空两入口全相联 LRU IOTLB；5 个 descriptor 全可见；授权稳定同 context 4KiB 页；源/目的集合不相交；一次一个 COPY；每个 COPY 先查源再查目的。5 个真实单区间 COPY（e0..e4）来自 endpoint-design.json：

| id | 源 | 目的 | 字节 |
|---|---|---:|---:|
| e0 | S0+0 | D0+0 | 64 |
| e1 | S0+64 | D2+0 | 64 |
| e2 | S1+0 | D2+64 | 64 |
| e3 | S2+0 | D2+128 | 64 |
| e4 | S0+128 | D1+0 | 64 |

6 个唯一翻译页 → 冷启动 6 miss 下界。fixed COPY=24 周期完整构成为：2 个端点 lookup + 1 个源 AR + 8 个 64-bit R beats + 1 个目的 AW + 8 个 W beats + 1 个 B + 3 个 frontend/retirement 周期。数据请求计数：5 个 COPY、320 useful bytes、320 read bytes、320 written bytes；5 个源 data AR、40 个 data R beats、5 个目的 AW、40 个 W beats、5 个 B、5 个 parent 完成。

| 控制 | 顺序 | 每 COPY misses | 总 misses | makespan |
|---|---|---|---|---|
| 双端 hit-first、再 oldest | e0,e1,e2,e3,e4 | 2,1,1,1,2 | 7 | 260 |
| 普通源页分组 | e0,e1,e4,e2,e3 | 2,1,1,2,1 | 7 | 260 |
| proposed bridge-last | e0,e4,e1,e2,e3 | 2,1,1,1,1 | 6 | 240 |
| **普通目的页分组（精确强对照）** | e0,e4,e1,e2,e3 | 2,1,1,1,1 | 6 | 240 |
| 同窗口软件穷举 | 120 排列取最小 misses | — | 6 | 数据/翻译服务下界 240，另加 CPU/发表窗口成本 |

- miss_cost=20 是抽象整权限 miss 服务时间，不是实测，也不是每 PTE 20；固定 COPY=24 如上。
- **公平性必须报告**：该重排使 e4 从 260 提前到 108，但 e1/e2/e3 各晚 44；若 e4 为 V，这是非 Pareto，不能写成 V/A 同时变快。普通目的页分组也精确匹配 makespan 和 sum completion。
- K=2 只是有效配额，不是整机只有两个 IOTLB 项；K=4 和从 A 重分配两项是强制对照，必须计 A 的损失。
- 相同 PA 不代表相同 lookup identity；不同 PSCID/GSCID 下不自动共享翻译。

- 假设：miss_cost=20（抽象整权限 miss 服务时间，非实测、非每 PTE 20）；fixed COPY=24；producer planning/lookup/lease 净非重叠开销 20 cycles 会抹掉其 1 miss makespan 优势。
- **明确哪个正例被哪个强对照复现**：bridge-last 的 6 miss / 260→240 被**普通目的页分组**（且穷举排序也达 6 页冷下界）**精确复现**。
- 邻反例：(1) 仅把 e4 源改为新页 S3——greedy 变 7 = 新七页冷下界，bridge 保持 8 miss = 280（差于 260）；(2) K 2→4——greedy 保留 S0 直达 6 冷 miss，新选择/租约开销只会拖累。
- **结论**：**因为**普通目的分组就能精确复现静态双端排序的全部收益，**所以不能把静态 7→6 miss / 260→240 当作 C 的成功背书**；C 只剩"动态 split/merge + 有费用驻留"的可证伪假设，**没有独有正例**。

**C 的可执行重设计（正向）**："Costed fragment-group admission with revocable translation residency"——first child VALID 前联合选择 child 粒度与有限驻留项；用实际成功覆盖 + 替换压力（非静态端点名）；组在承诺前可缩可散。

**完整可执行强对照（C）**：

- 普通 dynamic legalizer + 相同反馈 bounded pinning：普通动态边界合法器，使用相同成功覆盖/替换压力反馈，并做有界 pin/unpin。
- 全许可预准备 / WRITE 前瞻一页：所有需要权限先准备，或提前一页准备写权限；相同 owned 元数据/结果槽与成功许可测试。
- 同 window 软件预切 / SG 生成：同窗口五 parent 软件源/目的分组、合法顺序和自动 SG/2D 生成 child，不人为只向控制收 MMIO。
- 同资源真实在途/容量：保留 native 8 请求头、32 写数据字和已有并行；给对照相同真实容量/在途额度。
纯排序手例不能当动态机制的成功证据。其最短判别问题：**两条真实 DMA trace、同初始 5-parent 窗口、合法的不同运行时翻译覆盖/争用事件，能否要求不同的片段-组选择，使联合合同有普通"完整动态边界 + 同信息有界 pinning"控制无法达到的有用 V/A 完工点？** 若后者能在同成本选出相同 child/lease/permission/commit，则在建模前停止该机制。

## 4. 三种独立判断

| 候选 | 新颖性（规则层） | 独有系统收益 | 工程可行性 |
|---|---|---|---|
| A | 区间算术/源完成/权限门已有；**完整自耗尽跨 SID 合同**的准确覆盖仍有待核；不能以换领域或"没搜到"证明首创 | root 手例 candidate 24、whole-source 保 A 为 26/先 CPU 为 25（A 晚 1）、global-min 30、普通同合同 cursor 24；**普通 cursor 复现 24 → 无独有系统收益证据** | 可在真实 AR/R 身份路径加 sidecar/gate/证书；字段/端口/CPU 工作已计；native 无该合同且 PR/PW TODO，错误恢复未知 |
| B | 普通两 context MSHR walker 可逐事件等价，经典多线程 walker/最新作者 continuation 代码已有 → 裸暂停/续走不能作独立贡献 | 正例被 C1 完全复现；M2 时 C2 更快 → **无独有系统收益** | 静态 stage1 两 context 可实现，但要重构 TL owned request/稳定权限上下文/返回归属/结果缓存，非改一个 FSM |
| C | 静态双端排序/锁定由普通目的分组及 ICCD2018 覆盖；保留的是动态粒度选择 + 有费用驻留合同，准确独立性待判 | 静态 260→240 被普通目的分组复现；**动态候选无独有正例（不捏造）** | 需 owned parent/child、成功 coverage 信息、replacement-only lease、端口/expiry/回退；限制 firstVALID 之前可调整；未综合/未验证 native 正确性 |

## 5. 主线 A / 备选 C 的条件式创新结论

- **A 条件式结论**：**如果**同一任务/信任合同本来就需要源页交接，准确前作没有覆盖"由真实 DMA 未来范围与公开读义务推导的自动权限耗尽合同"，并且最简实现相对已有软件逐页控制或 ring 方案减少完整资源/软件交互且改善 V/A 联合端到端表现，**则可形成独立接口与微架构机制贡献**。**因为**静态分段 cursor 可以复现 24 的轨迹，**所以不能把区间补集或 min 运算作为独立新调度算法**；本轮手算普通同合同 cursor 完全复现；whole-source 保 A 控制差 2 单位、先 CPU 控制差 1 单位且 A 晚 1，**故本轮没有独有系统收益证据**。主线推荐的是**先核合同/成本的最短路径（研究判别顺序），不是已成立贡献，也不承诺二区**。
- **C 条件式结论**：**如果**成功许可的实际覆盖范围与即时替换压力，使联合片段切分与有限驻留合同在同资源、完整完成成本下，得到固定切分/普通页分组/双端 hit-first/同窗口软件重排/普通并行**无法复现**的有效完工点，且最接近前作未覆盖该联合动作，**则可形成独立机制贡献**。**因为**静态 7→6 miss/260→240 已被普通目的分组精确复现，**所以不能把它当动态机制的成功**；**需要**先给出动态覆盖/替换事件下"完整普通 dynamic legalizer + 同信息有界 pinning"无法复现的手算点，否则不启动 C 模型。
- **B 结论**：**因为**普通两 context MSHR walker 可逐事件等价复现且已有具体既有机制，**所以不能把裸 PTE 边界暂停/续走作为独立贡献**；只保留为两 context 普通 walker 强对照，不为其建模型。

## 6. 本轮仅计划的 9 组模型、模型前门槛、可交实现者的文件/接口与推进停止条件

### 6.1 模型前门槛（任何模型前必须先过）

1. **A 先固定与对照一致的信任/权限合同**：可信 DMA 已保证源不重读时，先比较只暴露真实进度的最简方案，不能靠增加安全要求强造性能贡献。
2. **A 对比**普通静态 reader 端点分段 cursor、rIOMMU、逐页源完成 + 预装 gate、双缓冲，先在字段/端口/软件交互账上找到可检验差异；若是既有已覆盖合同被完全复现，则归档该独立主张；若只是候选最简实现同合同同成本，则用最简实现继续核前作/成本贡献，不自动归档完整合同。
3. **C 先得到**动态覆盖/替换事件下"完整普通 dynamic legalizer + 同信息 pinning"无法复现的手算点；否则以下 C 模型不启动。
4. **B 已有等价证明**，不再为裸 continuation 建模型。

### 6.2 9 组未来模型场景（本轮全部未执行）

| ID | 候选 | 问题 | 推进 / 停止条件 |
|---|---|---|---|
| M1 | A | 2 SID，V4 页/A1 页，完整 producer 和下一代 COPY；真实两 permission 生命周期、原有在途/数据资源、全部 PTW 与 W/B，比较范围正确 whole-source 和 cursor | 同合同成本后出现可解释端到端差异 / 只能胜 global-min，或收益被 CPU/描述符/B 消除 |
| M2 | A | 保持每 DMA 在途额度，改变注册长区间页数；比较逐页预装门/descriptor 与压缩区间合同的控制请求、软件元数据、端口和完成成本 | 完整资源或软件工作减少且系统有收益 / 普通分段 cursor/ring 同成本精确复现，或复杂度抵消收益 |
| M3 | A | 仅把 A 读 P0 改为读全部 4 页，保留其它输入 | 被 A 仍需读取的页不能提前发证，此前范围差异消失 |
| M4 | A | 仅加入 producer 页间依赖，必须先生产 P0 再生产 P1..P3；另记录真实 CPU 暂存能力 | 可回收空洞不一定有用；没有应用收益就不算性能贡献 |
| M5 | A | 仅增加旧 V 写 B 延迟，使其晚于各控制 producer 就绪时间 | native WAIT 遮蔽提前生产收益；报告原瓶颈而非增加无依据并行 |
| M6 | A | 同资源类型账加入额外源页双缓冲/bounce-copy 和实际缓存暂存，测 CPU+DDR 流量及 V/A 整体完工 | 候选位于有用成本—完成前沿 / 简单缓冲或可信进度方案更便宜且同样快 |
| M7 | A | held VALID、乱序 R、证书背压、旧 epoch、许可失败/RRESP 错误定向状态集合 | 只有真实退休且全别名封闭才发证，公开请求稳定，无丢失完成 / 任一提前回收；未建恢复合同则明确隔离停止，不冒充 fault recovery 通过 |
| M8 | C | 先过手推门槛，再用同一真实 parent 窗口的两种合法运行时驻留/覆盖事件，比较联合 split/merge+lease 和普通动态切段/分组/pinning | 完整成本下 V/A 有控制无法复现的有用完工点 / 简单动态规则可选同 child 与同 lease 序列 |
| M9 | C | 工作集全驻留，及 V 年龄上界打断 group 的邻近场景；给强对照相同资源扩容选择 | 没有可省 miss 时退化为普通路径；不以饿死 A、改变权限或削弱并行取得收益 |

### 6.3 可交实现者的文件/接口（无 RTL）

- frontend descriptor 身份：[idma_reg64_frontend.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:323)（正文范围 323–340、478–554；普通 num_bytes 路径支持候选长描述符入口，batch 4KiB；COUNT 首次 VALID 不可改，WAIT 等完整 trans_complete 才下项）。
- transport 真实 R 退休：[idma_axi_transport_layer.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:170)（正文范围 170–360；273 最后 R，282 响应可能含 error；只用 r_dp_valid 不证明完整成功 R 退休）。
- 默认 COPY 完成点：[idma_backend.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:519)（正文范围 519–532；默认在 w_dp_rsp_valid 与 w_last_burst；NO_ERROR_HANDLING 不具备已验证故障恢复）。
- DMA 配置：[ariane_peripherals_xilinx.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1255)（正文范围 1255–1277；4 DMA，nonblock 前两 BUFFER_DEPTH 512，其余 3；JOBFIFO 8，NumAx 8）。
- IOMMU gate 可覆盖来源与 PR/PW TODO：[rv_iommu_cq_handler.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:330)（正文范围 330–390；362 PR/PW counter TODO）。
- PTW owner/输入来源：[rv_iommu_ptw.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:90)（正文范围 90–148、192/218、384–405；单 FSM，公开 AR 到 R 期间 owner 不可换）。
- 端点接口（C）：[idma_legalizer.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:200)（正文范围 200–214、284–312、443–469）、[idma_backend.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:75)（正文范围 75–87、551–569、629–648）、[typedef.svh](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/include/idma/typedef.svh:20)（正文范围 20–42）。
- 画出 firstVALID 到 R/证书的 owner 生存期；读指定行，不扫描仓库。

### 6.4 实现推进/停止条件

- **先交接口/账单，无 RTL**；A 完成同合同强控制与状态成本比较后，才复用现有事件模型接 M1/M2；先做成功路径范围 + 真实返回，不立即声称支持动态 remap。
- 若 A 或 C 的手推/模型门成立，未来 RTL 修改**仅限** source-gate/退休 sidecar/证书接口或 descriptor 调度/驻留接口的**小批次**；需单独核 map/invalidation/error 合同；**本轮不执行**。
- **没有新差异不重跑旧 29 次 COPY 验证、不编译 BIT**；原有结果保留。
- A 最简实现若与普通 cursor/静态端点分段同合同同成本，应采用最简形式并继续核前作/成本贡献；只有既有已覆盖合同被完全复现才归档该独立主张，**不声称另有复杂新算法**，也不自动归档完整合同。

---

## 附：路径与证据

- 工作区：[work](/home/xvhaocheng/work)
- 平台：[iommu-platform](/home/xvhaocheng/work/iommu-platform)
- Proposal 目标：[higher-model-mechanism-proposal-20261009.md](/home/xvhaocheng/work/iommu-platform/docs/higher-model-mechanism-proposal-20261009.md)
- 证据目录：[round01](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/)
  - [root-result-acceptance.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-result-acceptance.json)、[root-range-complement-design.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-range-complement-design.json)、[root-cost-and-gates.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-cost-and-gates.json)（最终权威）
  - [root-primary-review.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-primary-review.json)、[root-source-review.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-source-review.json)、[walk-critical.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/walk-critical.json)、[endpoint-design.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/endpoint-design.json)（细节）
  - [lifecycle-redesign.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/lifecycle-redesign.json)（未采用的独立手推，另一套资源假设）
- stage87 归档：[status-stage87-20261009.md](/home/xvhaocheng/work/iommu-platform/docs/status-stage87-20261009.md)
