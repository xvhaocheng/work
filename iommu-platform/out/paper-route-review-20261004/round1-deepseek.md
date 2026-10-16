# 正式五轮交叉审查 Round1：DeepSeek 独立提案（未看 Codex 方案）

> 我未看到 Codex 的独立方案，也不猜其方向；本轮只依据输入证据提出 3–4 条明显不同、可在现有四 DMA / RISC-V IOMMU FPGA 上产生可证伪证据的路线。  
> 我不能执行、写文件或浏览；所有引用只限输入文件及其行号。外部新颖性一律标“待查”，不编造引用、不把输入中已知文献说成穷尽检索。  
> 本轮不写代码，不预先选择最终方向；只给研究问题、假设、机制、实验、基线、指标、停止条件与最小实现代价。

---

## 0. 不可动摇的证据边界

1. **method88 固定 A8 / target=170 的实占计费主路线已按预定条件结束。** 88/88 完成，host exit 0；四个启动×方向对最强匹配简单基线全负：-5.68952%、-8.23689%、-5.15142%、-4.43680%，跨方向 False。p5 转为已测候选/基线，不翻案、不为 p5 再挑条件。证据：`out/method88-execution-20261004/RESULTS.md:17-20,29-38,100-102`；`docs/research-overnight-progress-20261004.md:11-20`。  
2. **E2 局部直接替换证据与 E3 受控 HOL 证据保留。** E2 新版 64 clean/exit0，128 target 中 62 替换后 firstmiss、66 保持 hit；E3 保留有限受控阻塞证据，人工 128 周期只作诊断口径。证据：`out/method88-execution-20261004/RESULTS.md:22,94-95`；`docs/research-literature-review-20261004.md:79-80`。  
3. **v5 机制已实际激活，但性能不能宣布改善成功。** N 在 A8/A16 有真实非零 candidate/probe/DC/leaf/fast accept；A8 N/S 约 +4.15%（输入亦写约 +4.2%），A16 N/S 约 -2.69%/-2.7%；但 N/原版 A8 约 -14.52%、A16 约 -22.09%，N 仍慢于 original。n=3，不同 BIT、不同 fresh boot；fast accept 不是 hit rate。证据：`docs/research-overnight-progress-20261004.md:58,60-81,94,105-110`；`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:27,43-45,75-82,87-89,140-164,170-181,214-224`；`docs/research-n-prototype-spec-20261004.md:183`。  
4. **已知 hit-under-miss、cache partition、token bucket、纳秒级 FPGA 节流、QoSID 身份关联不能当新颖性。** 证据：`docs/research-literature-review-20261004.md:19,23-26,30-42`；`docs/paper-logic.md:116-126`；`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:210,244`。  
5. **论文要解释可迁移问题，不能只写“本实现 bug 修复”。** 由于 v5 是不同 BIT/boot，S/N 差异不能当纯 runtime enable 因果；任何新路线都必须给出跨 BIT、跨配置或跨工作集仍可检验的机制条件，否则降级为工程取舍。证据：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:27,45,159-164,220`。

---

## 1. 一页导航摘要

| 编号 | 临时路线名 | 一句话 RQ | 核心可证伪预测 | 与“限速/N快通路优化”的区别 | 最小实现量级 | 主要停止条件 |
|---|---|---|---|---|---|---|
| R1 | 失效/上下文生命周期干扰 | A 触发的 invalidation / fence / 上下文更新，是否独立于 A 的普通 miss 占用，成为 V 的可测干扰源？ | 固定 A 有效字节与翻译请求数，提高失效频率/改变粒度会改变 V 完成时间与等待阶段；若控制 A 请求后无独立效应，则 H0 成立 | 不调限速、不优化 hit-under-miss；研究失效纪律与 outstanding 排空 | Linux 失效接口 + 被动观测；可能不改 IOMMU 主数据路径 | 仅软件开销变化；失效频率/粒度无可复现独立效应；无法区分 quiet/协议 |
| R2 | 上游 DMA 仲裁/到达相位 vs IOMMU 内部阻塞 | 四 DMA 干扰主导因素是上游 DMA/AXI 仲裁与到达相位，还是 IOMMU 内部单共享前端？ | 保持 A 总请求/字节/页集合，仅改 DMA 仲裁/缓冲，V 等待模式可区分“前端占用主导”与“到达相位主导” | 不是限速；是因子隔离与设计空间 | 改 DMA 仲裁/BIT + 已有诊断 | 仲裁改变只改 A 平均吞吐；V 额外等待不可复现 |
| R3 | 保护单元：DID/槽还是共享翻译域 | 保护 V 的翻译状态应按 DID/槽容量，还是按实际共享匹配域（global/PSCID/GSCID/别名）？ | 相同总容量与 PLRU 下，进入同一放宽匹配域的 V/A 与 DID 隔离情形，V 条目存活/首次探测不同 | 不继续做 cache partition 本身；检验 partition 的“保护单元”是否定义错 | Linux 页表/global/DID 配置 + E2 观测；可能小改 RTL 导出 tag | 替换完全由容量/PLRU 解释；共享域无额外效应；无法导出上下文/tag |
| R4 | 前端 HOL vs 译后出口/数据路径反压 | A 长 PTW 下 V 合法 hit 的等待，主要在前端，还是在译后出口/数据路径反压？ | 2×2：A PTW 长/短 × 出口可受理/受控反压；若出口反压时 V 等待不再随 A PTW 长度增加，则出口主导 | 不优化 N 快通路；把 N 作为基线，寻找 hit-under-miss 的适用边界 | 选择性 PTW 延迟 + 出口反压 + 逐请求阶段观测；RTL 改动较大 | 无法建立选择性 PTW/出口反压；V 等待不随因子变化 |

**共同底线：** 四条路线都不是要求“造一个新算法”；R1、R3、R4 允许主要贡献是 design-space / negative / boundary，但必须回答“在哪些共享结构下，已知机制何时足够、何时失效”，不能只写某版本修好了某 bug。

---

## 2. 所有路线共同遵守的实验与公平原则

1. **同一 BIT、同一 boot、同一仪器、同一时钟、同一工作集、同一完整 4 KiB copy 作动点。** 若硬件资源迫使不同 BIT，则按启动分别报告，不把跨 BIT 差异写成纯因果。依据：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:27,45,159-164,220`；`docs/research-literature-review-20261004.md:72`。  
2. **A 的进度用同一 V 硬件持续时间内的有效完成字节。** 不用不同长度 V 窗口中的 A 字节直接比较；A jobs/bytes 来源与交叉核对见 `out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:115,136`。  
3. **V 主指标是端到端固定工作完成时间/吞吐与短时未达目标分布。** 不是单纯 fast accept 计数；fast 是 accepted requests，不是 hit rate，也不是全部 miss 数。依据：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:170-172`；`docs/research-overnight-progress-20261004.md:79`。  
4. **强基线至少包括：** 现有串行 S；合理有限非阻塞 N；同每类总容量静态预留 P；固定 gap 或按字节/请求/miss/固定类型加权计数的准入；无 A Solo；CPU-only 对照。不得为衬托候选而削弱 N 或 P。依据：`docs/paper-logic.md:224-231,351-353`；`docs/research-literature-review-20261004.md:71`；`out/method88-execution-20261004/RESULTS.md:110-111`。  
5. **n=3 是工程重复，不是统计显著。** 新路线若进入性能比较，必须预注册重复数、配对 block、独立单位与阈值；不得事后调参数。依据：`out/method88-execution-20261004/RESULTS.md:84`；`docs/paper-logic.md:433`。  
6. **所有科学阴性保留。** fast=0、失效无效应、仲裁无效应、共享域无额外效应，都按预定矩阵保留，不自动重跑、不剔除。依据：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:5,222-223,254`。  
7. **不能把已知原则称新。** hit-under-miss、完整缓冲、cache partition、token bucket、纳秒级 FPGA 节流、QoSID 身份关联、按时间计量干扰，均已有输入证据或规范披露。依据：`docs/research-literature-review-20261004.md:19,23-26,30-42`；`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:210,244`。

---

## 3. 路线 R1：失效与上下文生命周期干扰

### R1.1 Research Question
在四 DMA 共享 RISC-V IOMMU 的锁定实现上，当 A 的翻译/数据流量与显式 IOTLB 失效、上下文更新或 fence 并发时，V 的固定工作完成时间中，有多少来自**失效生命周期与 outstanding 请求排空/顺序纪律**，而不是 A 的普通 miss 占用或数据带宽竞争？

### R1.2 Hypothesis
- **H1a（可证伪）：** 在保持 A 有效字节、A 翻译请求数、V 工作量与页集合不变时，提高 A 触发的 invalidation/fence 频率，会独立增加 V 的端到端完成时间或 V 合法 hit 的等待。
- **H1b（可证伪）：** 该增加随失效粒度（DID 范围、页范围、全量）和 outstanding 深度呈可预测变化；若失效必须等待排空，则 V 的等待应集中在失效到 quiet 的区间。
- **H0：** 控制 A 请求后，失效频率与粒度没有独立效应；V 时间只由 A 数据/翻译流量解释。

### R1.3 Novelty 与待查
- 输入已知：共享 IOTLB 干扰已有表征（`docs/research-literature-review-20261004.md:17`）；IOTLB 侧信道与 set/way 分区已有研究（`docs/paper-scenarios-mitigation.md:165`）；失效/映射/coloring 在 Bermuda §V-C 只列候选，完整评价超出该文范围（`docs/research-literature-review-20261004.md:17,32`）。因此“IOMMU 会有干扰”或“失效可能变慢”本身不新。
- 可能差别：把**失效/上下文生命周期**作为可独立调制的干扰源，在 RISC-V IOMMU 多 DMA 上测“失效纪律 → outstanding 排空 → 合法 V hit 被挡”的条件与边界。但这是待查命题，不是已成立新颖性。
- **待检索：** IOMMU invalidation / ATS / PASID invalidation / shootdown 的实时干扰；SMMU、RISC-V IOMMU 规范中的 invalidation 与 outstanding 语义；CPU TLB shootdown 与 IOMMU shootdown 的差异；是否已有把 invalidation 频率作为独立变量的 DMA 干扰论文。输入未提供逐页证据，不能声称空白。

### R1.4 Mechanism
- 失效可能要求更新 generation、等待在途翻译或数据请求达到安全点；单 FSM、单份 `trans_req_data_q/trans_resp_data_q`、单响应口会把这一等待传播到其他合法请求。输入依据：`docs/research-n-prototype-spec-20261004.md:16-18,56-61`；`docs/paper-logic.md:170`；`docs/paper-scenarios-mitigation.md:183`。
- v5 已证明有限快通路可在 A8/A16 触发，但 A16 反而低于 S；这提示“有 fast accept”不等于“总时间改善”。R1 把失效生命周期作为另一个可能被忽略的等待源。依据：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:75-82,87-89,161-162`。

### R1.5 实验与混杂变量
- **因子：** 失效频率（0、每 copy、每 k copy）× 失效粒度（DID、页、全量）× 是否有 A 数据流量 × outstanding 深度（通过工作集/请求队列控制）。
- **控制：** A 有效字节、A 翻译请求数、V 工作量、页表映射、CPU 轮询节奏尽量固定；CPU-only 失效无 A、A 无失效、空失效三种对照。
- **观测：** 失效触发、失效到 quiet 的时间、outstanding 计数、V 的 miss/hit、V 完成时间、A 同窗有效字节。
- **混杂：** Linux ioctl 软件开销；SD/Linux 更新；不同 BIT/boot；A 因失效改变自身到达率；下游 DDR/数据路径反压；失效范围影响页表缓存；CPU 轮询供给。必须把软件开销与硬件 quiet 时间分开。
- 若无法区分“软件发起失效的开销”和“硬件生命周期等待”，则 R1 不成立。

### R1.6 公平强 baseline
- 现有 S；合理 N；固定 gap；按字节/请求/miss 的准入；无失效 A；CPU-only 失效；同总容量静态 P。
- 不把 A 数据量压低来制造 V 收益；A 进度按同一 V 窗口有效字节报告。依据：`docs/paper-logic.md:224-231`；`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:115,136`。

### R1.7 Metrics
- 主：V 端到端完成时间/吞吐、短时未达目标分布、失效到 quiet 延迟。
- A：同一 V 硬件窗口内有效完成字节、总完成工作、失效导致的 A 尾部。
- 诊断：V 的 miss/hit、outstanding、失效粒度、上下文编号、DDT/PDT/IOTLB 更新来源。
- 资源：LUT/FF/BRAM/Fmax；若只改软件则报告 CPU 开销与 ioctl 频率上限。

### R1.8 预期贡献
- 若 H1 成立：给出“失效生命周期是共享 IOMMU 可迁移干扰源”的条件，说明保护翻译缓存或做 hit-under-miss 仍不够；这是设计空间贡献，不是修本实现 bug。
- 若 H0 成立：给出负面边界——在当前实现中，失效频率不是独立主导因素；结合不同 BIT/boot 说明该负面结论的适用范围。
- 不能声称“首次发现失效会干扰”，只能争取“在何种共享结构下失效纪律成为主导边界”。

### R1.9 停止条件
- 失效频率/粒度操纵不改变 V 时间，或只改变软件发起开销；无法把软件与硬件 quiet 分开；协议/观测错误停试保留；重复 n 内无可复现方向。  
- 若需要无界等待或破坏既有失效语义才能制造效应，停止该路线。

### R1.10 最小可执行实现代价
- **首选 Linux-only + 被动观测：** 在现有驱动/iol 接口上增加可控失效/上下文更新触发；记录 boot id、失效参数、quiet、outstanding。关键文件需要：`linux/drivers/misc/idma.c`、`linux/drivers/misc/idma_research.h`、现有 IOMMU invalidation/fence 路径；RTL 观测可先用现有 observer。
- **若观测不足：** 小改 `rv_iommu_iotlb.sv`、`rv_iommu_trans_ctl_wrap.sv`、`dma_nonblock_observer.sv` 导出失效/quiet/outstanding，不算大规模重构。
- **未知：** 当前驱动是否有可用 invalidation/fence ioctl；RISC-V IOMMU 是否支持页/DID 粒度失效；失效与 outstanding 的 RTL 顺序是否可观测。

---

## 4. 路线 R2：上游 DMA 仲裁与到达相位 vs IOMMU 内部共享阻塞

### R2.1 Research Question
在四 DMA 共享 RISC-V IOMMU 的 FPGA 上，跨 DMA 干扰的主导可迁移来源，是 IOMMU 内部单共享翻译前端，还是上游 DMA/AXI 仲裁与缓冲决定的请求到达相位、突发和重叠窗口？

### R2.2 Hypothesis
- **H1（可证伪）：** 保持 A 总请求数、总字节、页集合和 V 工作量不变，仅改变上游 DMA 仲裁（现有、轮转、加权、固定优先）与每路缓冲深度，V 的等待分布会改变；改变模式可用于区分“IOMMU 前端占用主导”与“上游到达相位主导”。
- **H0：** 上游仲裁只改变 A 平均吞吐，不改变 V 在 A PTW 期间的额外等待；V 损失由 IOMMU 内部单 FSM/单响应口解释。

### R2.3 Novelty 与待查
- 输入已知：四 DMA 已存在；DMA0/1 在 nonblock 模式下缓冲为 512 拍，DMA2/3 为 3 拍；`BUFFER_DEPTH` 绑定在 `ariane_peripherals_*.sv`，路径经 `dma_core_wrap`。依据：`docs/research-n-prototype-spec-20261004.md:58,117-129`；`docs/research-n-prototype-spec-20261004.md:101`。
- 已知 QoS/仲裁/身份关联不能当新算法；RISC-V IOMMU QoSID 只定义身份关联，不保证内部调度。依据：`docs/research-literature-review-20261004.md:23`。
- **待检索：** NoC/AXI 仲裁对共享 IOMMU 干扰的影响；DMA engine arbitration 与 QoS；是否已有工作把上游到达相位与 IOMMU 内部阻塞做正交因子。输入未给逐页证据，不能声称空白。

### R2.4 Mechanism
- 四 DMA 的 AW/AR 选择、bank 划分、buffer 深度、demux/mux 决定 A 到达 IOMMU 的时间与突发；IOMMU 内部单 FSM/单请求响应决定一旦 A 占有前端后 V hit 能否推进。两条链可能混合。依据：`docs/research-n-prototype-spec-20261004.md:99-107,111-115,117-121`；`docs/paper-logic.md:46,170`。
- 若 V 的额外等待只与“A 占有前端窗口”相关，而改变仲裁不改变该窗口的条件概率，则内部主导；若 V 等待随 A 到达相位/突发显著变化而前端占用近似不变，则上游主导。

### R2.5 实验与混杂变量
- **因子：** DMA 仲裁策略 × buffer 深度（已有 3/512 限制可作起点） × A 工作集（1/8/16 页） × V 到达相位（与 A 请求对齐/错开）。
- **控制：** 总 A 字节、总请求数、页集合、V 工作量、时钟、仪器、Linux 不变；不同 BIT 分启动报告。
- **观测：** A 的 AR/AW 到达间隔分布、DMA 仲裁选择、IOMMU 前端 busy/状态、V 等待阶段、candidate/probe/fast 计数、A 同窗字节。
- **混杂：** 改变仲裁可能改变 A 的完成时间，从而改变窗口内 A 字节；必须同时报告“总 A 工作”和“同 V 窗口 A 字节”。不同 BIT/boot、DMA 软件轮询、cache 状态、bank 布局。
- 输入已明确 v5 不同 BIT/boot 不能作纯 enable 因果；R2 必须用同 BIT 内可切换模式或分启动报告。依据：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:27,45,160,220`。

### R2.6 公平强 baseline
- 现有 DMA 仲裁；S；合理 N；同总容量静态 P；固定 gap；按字节/请求/miss 计数准入。
- 不把加权仲裁本身称为新方法；只把它作为因子。依据：`docs/research-literature-review-20261004.md:23,42`；`docs/paper-logic.md:342`。

### R2.7 Metrics
- 主：V 完成时间、V 等待分布、A 同 V 窗口有效字节、总 A 完成时间。
- 诊断：A 到达间隔、前端占用周期、V 到达是否与 A 前端占用重叠、DMA 仲裁选择、fast accept（仅作机制计数）。
- 资源/Fmax：仲裁逻辑、buffer 变化、LUT/FF/BRAM。

### R2.8 预期贡献
- 若上游主导：论文可解释“可迁移问题”在系统集成与请求到达，而不是本 IOMMU FSM 的单独 bug；N 快通路优化不是充分条件。
- 若内部主导：给出上游仲裁不能解决的边界，说明需要前端解耦或出口处理。
- 无论正负，都需给出设计空间条件，而非“换了一个仲裁器更快”。

### R2.9 停止条件
- 仲裁策略变化对 V 额外等待无可复现影响，或只改变 A 平均吞吐；无法测量 A 到达相位；不同 BIT/boot 混杂无法分离；协议错误停试。

### R2.10 最小可执行实现代价
- 修改 DMA 仲裁/BIT 参数，使用现有四 DMA、bank、buffer 参数；观测可复用 `dma_nonblock_observer.sv` 与 v4/v5 诊断 bank。依据：`docs/research-n-prototype-spec-20261004.md:101,117-121,166-167`。
- 关键文件需要：`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`、`cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv`、`dma_nonblock_observer.sv`、`axi_demux_simple` 相关路径。
- 未知：DMA 仲裁是否已有可运行时切换接口；切换是否需要重新构建 BIT；四 DMA 选择是否能保持 A 总工作量不变。

---

## 5. 路线 R3：保护单元是 DID/槽，还是实际共享翻译域？

### R3.1 Research Question
在 RISC-V IOMMU 的 IOTLB tag/匹配与 global/共享规则下，A 对 V 的翻译状态干扰，应按 DID/槽容量保护，还是应按**实际共享匹配域**（global、PSCID/GSCID、相同 IOVA 别名、1S/2S 匹配）保护？容量替换之外，是否存在“放宽匹配/共享域”导致的伪共享或保护失效？

### R3.2 Hypothesis
- **H1（可证伪）：** 在相同 IOTLB 总容量、相同 PLRU/替换策略、相同 V 工作量下，若 V/A 的翻译进入同一可共享/放宽匹配域，则 V 原条目存活、首次探测 miss 或共跑等待，会不同于“DID 不同但域隔离”的条件。
- **H0：** DID 不同即足够隔离；干扰只由总容量、PLRU 和请求流量解释。

### R3.3 Novelty 与待查
- 输入已知：当前 IOTLB tag 不含 DID，按页号、PSCID/GSCID、翻译阶段匹配；一级 global 页会放宽 PSCID 条件；不能由不同 DID 直接推不同缓存域，也不能把普通 coloring 当硬分区。依据：`docs/paper-scenarios-mitigation.md:115`；`docs/paper-scenarios-mitigation.md:182`。
- 已知 set/way 分区、IOTLB 侧信道、IOVA 偏移/局部性已有研究。依据：`docs/paper-scenarios-mitigation.md:165-166`。因此“cache partition”本身不新。
- **待查：** RISC-V IOMMU 规范中 global/1S/PSCID/GSCID 的实际匹配与共享语义；DID/PID 与翻译域的关系；是否已有论文把“保护单元定义错误”作为干扰设计边界；SMMU global/ASID 共享的先例。输入未给逐页证据，不能声称空白。

### R3.4 Mechanism
- IOTLB tag/lookup/fill/PLRU 与 DDT/PDT cache 决定 V 条目是否被 A 覆盖；若匹配域放宽，A 的填充可能进入本应属于 V 的同一可共享条目或影响替换候选。依据：`docs/paper-scenarios-mitigation.md:182-184`；`docs/research-n-prototype-spec-20261004.md:16-24,49-54,56-61`。
- E2 已有有限替换证据，但尚未把“容量替换”与“共享域/别名”分离。依据：`out/method88-execution-20261004/RESULTS.md:94`；`docs/research-literature-review-20261004.md:79`。

### R3.5 实验与混杂变量
- **E2 风格：** V 预热 → A 有限访问 → 完全排空 → V 首次探测；同时做共跑性能。
- **因子：** DID 相同/不同；global 页开/关；相同 IOVA 别名/不同 IOVA；PSCID/GSCID 相同/不同；1S/2S 组合；页大小固定 4 KiB 起步。
- **观测：** V 原条目、A 填充、替换来源、首次探测 hit/miss、上下文快照、DDT/PDT/IOTLB 更新来源。
- **混杂：** Linux 页表配置与 global 位；失效操作；不同 BIT/boot；页表布局；A 工作集；V/A 映射变化可能改变 TLB/页表路径；不能从 DID 不同直接推独立条目。
- 输入已警告：仅凭 DID 不同不能断言独立翻译条目；不能把 miss 增多直接叫容量干扰。依据：`docs/paper-logic.md:54-60`；`docs/paper-scenarios-mitigation.md:115`。

### R3.6 公平强 baseline
- 无 A；A 不同页；A 同 global 页；同总容量静态预留 q；合理 N；固定 gap；S。
- 若做分区 q，必须同总容量、同替换更新规则；不能用只读 probe 代替真实被服务 hit。依据：`docs/research-n-prototype-spec-20261004.md:49-54`。

### R3.7 Metrics
- 主：V 首次探测 miss/命中、V 完成时间、共跑等待。
- 诊断：目标条目旧/新状态、A 填充来源、替换候选、实际上下文、global/PSCID/GSCID。
- A：同窗有效字节；资源/Fmax。

### R3.8 预期贡献
- 若共享域主导：贡献是“保护单元应是实际匹配域而非 DID/槽”，这是设计空间/负面贡献；可解释为什么某些 cache partition 在本结构上无效。
- 若容量/PLRU 解释一切：给出负面边界，说明静态分区按 DID 或槽位即可；但必须说明适用范围。
- 不能把 cache partition 本身称新。

### R3.9 停止条件
- 替换完全由容量/PLRU 解释，共享域无额外效应；无法导出 tag/上下文快照；global/PSCID/GSCID 配置不可控；只观察到 miss 变化而无条目证据；协议错误停试。

### R3.10 最小可执行实现代价
- Linux 侧：页表 global 位、不同 DID/PID、相同 IOVA 别名配置；E2 runner 复用。关键文件：`linux/drivers/misc/idma.c`、`idma_research.h`、页表/映射路径。
- RTL 观测：`rv_iommu_iotlb.sv`、`rv_iommu_trans_ctl_wrap.sv`、`rv_iommu_top.sv`、DDT/PDT cache；可能小改导出 tag/上下文快照。
- 未知：当前是否可导出实际 tag/匹配域；global 页规则在锁定 RTL 中的实际实现；相同 IOVA 别名是否被硬件允许且不破坏正确性。

---

## 6. 路线 R4：翻译前端 HOL vs 译后出口/数据路径反压

### R4.1 Research Question
在 A 长 PTW 与四 DMA 数据竞争下，V 合法 hit 的等待主要发生在翻译前端（单 FSM、单请求/响应、A 占有 REQ_TRANS/AWAIT_RESP），还是发生在译后出口/数据路径反压（COMPLETE 等地址受理、单响应口、FIFO/下游数据）？哪个边界更可迁移到其他共享 IOMMU 结构？

### R4.2 Hypothesis
- **H1a：** 当 A PTW 长且出口可受理时，V 等待随 A PTW 长度增加；N 快通路应减少前端等待。
- **H1b：** 当出口受控反压时，V 等待主要由出口反压持续时间决定，不再随 A PTW 长度增加。
- **H0：** V 等待只由总数据/翻译流量解释，前端/出口因子无正交效应。

### R4.3 Novelty 与待查
- 输入已知：E3 保留有限受控 HOL；HPCA2017 已知 hit-under-miss；单份 `trans_req_data_q/trans_resp_data_q`、单响应口、COMPLETE 等地址受理是当前结构线索。依据：`out/method88-execution-20261004/RESULTS.md:95`；`docs/research-literature-review-20261004.md:19`；`docs/paper-logic.md:170`；`docs/research-n-prototype-spec-20261004.md:16-18,56-61`。
- 不能把 hit-under-miss 或“加缓冲”称新；完整 4 KiB 缓冲已有先例。依据：`docs/research-literature-review-20261004.md:19,89`。
- **待查：** 共享 IOMMU 前端阻塞与译后出口反压的正交实验；non-blocking cache MSHR 耗尽与 hit 阻挡；NoC/AXI 出口反压传播；是否已有工作给出“前端 vs 出口”的设计条件。输入未给逐页证据，不能声称空白。

### R4.4 Mechanism
- 翻译完成到地址受理之间可能继续占住公共控制；A 的 PTW 或出口等待可传播给 V。输入 `trans_ctl_wrap.sv` 单状态、FIFO 头与单份响应配对、COMPLETE 等 comp 地址受理，是静态线索。依据：`docs/paper-logic.md:170,270`；`docs/paper-scenarios-mitigation.md:183`。
- 现有 DS delayer 聚合多类请求，不能称 PTW 专属延迟；要建立选择性 PTW 延迟与出口反压。依据：`docs/paper-logic.md:79`；`docs/research-literature-review-20261004.md:75`。

### R4.5 实验与混杂变量
- **2×2：** A PTW 长/短 × 出口可受理/受控反压；V 在 A 前/中/后到达。
- **控制：** V 输出路径具备可服务条件；A 数据出口与 PTW 出口尽量分离；总 A 字节/请求数固定；同 BIT/boot。
- **观测：** 逐请求到达、接收前等待、准入、lookup、结果、完成；翻译完成到受理等待；出口占用；V miss/hit；A 同窗字节。
- **混杂：** 出口反压同时影响 A/V；DDR/数据路径不是纯 IOMMU；CPU 供给；相位；不同 BIT/boot；人工延迟只作诊断。依据：`docs/paper-logic.md:70-79`；`docs/research-literature-review-20261004.md:54`；`out/method88-execution-20261004/RESULTS.md:87`。

### R4.6 公平强 baseline
- S；合理 N；同总容量 P；固定 gap；按字节/请求/miss 准入；无 A；CPU-only 出口反压；同数据量。
- 不优化 N 快通路来证明候选；N 是边界检验基线。依据：`docs/paper-logic.md:206,224-231,351`。

### R4.7 Metrics
- 主：V 完成时间；翻译完成到地址受理等待；出口反压持续时间；短时未达目标分布。
- A：同 V 窗口有效字节、总完成时间。
- 诊断：前端状态占用、candidate/probe/DC/leaf/fast accept、出口 FIFO/响应口占用、逐请求阶段。
- 资源/Fmax。

### R4.8 预期贡献
- 若出口主导：论文可说明 N/hit-under-miss 的适用边界——“让 hit 越过 miss”不自动解决译后出口传播；这是可迁移到单响应口/共享出口结构的负面/设计空间贡献。
- 若前端主导：确认 E3 有限 HOL 的可迁移条件，并给出何时需要前端解耦。
- 不能声称“首次发现 HOL”或“首次 hit-under-miss”。

### R4.9 停止条件
- 无法建立选择性 PTW 延迟或出口反压；V 等待不随因子变化；出口反压无法与数据流量分离；协议错误停试；人工延迟结果无法在自然流量中复现。

### R4.10 最小可执行实现代价
- 需要选择性 PTW 延迟控制、出口反压控制、逐请求阶段观测；当前 DS delayer 聚合多类请求，不能直接当 PTW 专属开关。依据：`docs/paper-logic.md:79`；`docs/research-literature-review-20261004.md:75`。
- 关键文件需要：`rv_iommu_tl_wrap.sv`、`rv_iommu_trans_ctl_wrap.sv`、`rv_iommu_top.sv`、`dma_e3_observer.sv`、`dma_nonblock_observer.sv`；可能改 `rv_iommu_iotlb.sv` 以观察 lookup。
- 未知：出口反压能否在不改变 A 数据工作量的情况下独立控制；逐请求首次 valid/接收等待是否已建设；是否需要新 BIT 与 SD 更新。

---

## 7. 论文可迁移性的写法建议

无论最终选哪条路线，论文不应写成“我们修了当前 RTL 的某个串行 bug”。更可发表的骨架是：

1. **给出共享结构条件：** 单共享前端、单响应口、失效生命周期、上游 DMA 仲裁、共享翻译域、译后出口反压。  
2. **用正交因子或独立变量证明主导边界：** 不是只给一个加速百分比，而是给出何时已知 hit-under-miss / cache partition / 限速足够，何时不足。  
3. **保留负面与设计空间：** 若 R1/R3/R4 的假设被否定，仍可作为 design-space 贡献，说明该结构下哪些因素不是主导、适用范围在哪里。  
4. **跨 BIT/boot/配置复现：** 至少一个条件在不同 BIT 或不同启动下复现；否则降为工程修复。依据 v5 的 BIT/boot 限制：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:27,45,160,220`。  
5. **不把已知原则改名为新算法：** hit-under-miss、cache partition、token bucket、QoSID、纳秒级 FPGA 节流、按时间计量干扰均不作新意。依据：`docs/research-literature-review-20261004.md:42`；`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:210,244`。

---

## 8. 证据文件与行号索引

### 8.1 method88 与 p5 终止
- `out/method88-execution-20261004/RESULTS.md:17-20`：88/88、公平规则、四格负、路线结束。
- `out/method88-execution-20261004/RESULTS.md:29-38`：四格表与跨方向 False。
- `out/method88-execution-20261004/RESULTS.md:44-51`：p5 对 fixedgap/p6 的反向复现与边界。
- `out/method88-execution-20261004/RESULTS.md:65-79`：辅助占用统计、每 copy miss 不恒定、0xE2E 不能作真实分母、COMPLETE 侧可观测。
- `out/method88-execution-20261004/RESULTS.md:84-87`：n=3、5% 仅工程参考、人工 delay 只诊断。
- `out/method88-execution-20261004/RESULTS.md:94-95`：E2 已完成 64、E3 保留。
- `out/method88-execution-20261004/RESULTS.md:100-102`：p5 转已测候选、不宣称普遍无用/已证明新机制。
- `docs/research-overnight-progress-20261004.md:11-20`：四格与结论边界。
- `docs/research-overnight-progress-20261004.md:43-46`：两条线计数、v3/v4 诊断限制、v5 构建/板测。
- `docs/research-overnight-progress-20261004.md:58`：v5 阶段表，N 非零、S 有 candidate 无 probe。
- `docs/research-overnight-progress-20261004.md:60-71`：v5 中位数与相对变化。
- `docs/research-overnight-progress-20261004.md:73-81`：N 真实 candidate/probe/DC/leaf/fast 计数与解释限制。
- `docs/research-overnight-progress-20261004.md:94`：v5 boot id、Linux 版本、无需 SD 更新。
- `docs/research-overnight-progress-20261004.md:105-112`：当前状态更新。

### 8.2 v5 结果复核
- `out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:27`：S/N 不同 BIT、不同 fresh boot；original 复用旧基线。
- `...:43-45`：S/N boot id 与不同 BIT 限制。
- `...:58`：quality_issues=0、runner exit 0。
- `...:75-82`：N/S 机制激活与 S 有 candidate 无 probe。
- `...:87-89`：主机模型无总周期改善、板测 N/S 描述性、S/原版混杂。
- `...:115,136`：raw_window[1]、A jobs/bytes 来源、同窗比较。
- `...:140-164`：相对变化、A8 三轮波动、N/原版仍低、不能纯因果。
- `...:170-181`：诊断 bank 不是旧 HPM、fast 不是 hit rate、窗口语义。
- `...:183-188`：A8/A16 逐条件计数、short-a8 只验收。
- `...:201-210`：解释限制、已知 hit-under-miss 不能称新。
- `...:214-224`：数据问题、S/N 不同 BIT、旧 HPM 不作证据、fast=0 合法。
- `...:231-252`：能说/不能说与结论。

### 8.3 文献与新颖性边界
- `docs/research-literature-review-20261004.md:15-26`：定向文献矩阵；Bermuda、MemPol、HPCA2017、HyperTRIO、QoSID、专利、MemCoRe、DATE2025。
- `...:30-42`：没有前人已证明走不通；概念已被提出；明确不构成创新的清单。
- `...:44-56`：RQ-1 与 T1–T5 终止判据。
- `...:58-75`：选项 A 终止、选项 B 候选、三相 S/N/P、指标与前置未满足。
- `...:85-91`：读写依赖与缓冲先例；完整缓冲不是新创新。
- `docs/paper-logic.md:114-126`：新颖性可争取链与不能称新。
- `docs/paper-scenarios-mitigation.md:158-175`：第一轮先行工作核对与规范 QoS 扩展。

### 8.4 当前实现结构
- `docs/research-n-prototype-spec-20261004.md:16-24`：完整 header、内部 tag、V 自持上下文、权限/order、写路径纪律。
- `...:34-47`：AR FIFO head 与年轻响应提取矛盾。
- `...:49-54`：单 lookup 端口、timemux、probe 与真实 hit、公平 baseline 替换规则。
- `...:56-61`：容量、单响应口、失效、fault。
- `...:63-68`：Linux 特性位、generation 元数据、SD 更新未定。
- `...:70-76`：三功能接受条件，未板测。
- `...:99-109`：两银行生命周期、命中绕过的有限支持、静态银行已改变资源分配。
- `...:111-115`：v1 读接纳守卫、overlap rr banked 8/12 失败、legacy 12/12 通过、未上板。
- `...:117-137`：v2 4 KiB 缓冲、容量核算、不能外推、仍不主张 novelty。
- `...:151-153`：v3 FIFO 存储验证通过。
- `...:158-173`：v4 实现、诊断 bank 映射、边界。
- `...:176-183`：v5 后续入口与执行结果。
- `docs/paper-logic.md:170`：`trans_ctl_wrap.sv:616,683,706` 单状态与出口等待；`tl_wrap.sv:149,241,874,1413`。
- `docs/paper-scenarios-mitigation.md:182-184`：IOTLB tag/lookup/fill/PLRU、事务控制、翻译控制代码依据。

### 8.5 公平与指标
- `docs/paper-logic.md:104-112`：评价同时交代 V/A；窗口结束偏差；计时包括准入等待。
- `docs/paper-logic.md:224-231`：N/P 共享契约、容量分类、不得都称 MSHR。
- `docs/paper-logic.md:258-263`：验收/测量/停止。
- `docs/paper-logic.md:403-411`：最终九项研究链。
- `docs/paper-logic.md:413-420`：最小控制合同。
- `docs/paper-logic.md:433`：预注册阈值、重复数、独立单位。
- `docs/research-literature-review-20261004.md:69-74`：三相 S/N/P 与指标。
- `out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:115,136,170-172`：同窗、A jobs/bytes、fast 语义。

---

## 9. 未知点清单（必须由 Codex 或后续轮次确认）

1. **失效接口：** 当前 Linux 驱动是否有可控 IOMMU invalidation/fence/context update 接口？RTL 是否支持 DID/页/全量粒度？失效与 outstanding 请求顺序是否可观测？未知。
2. **失效 quiet：** 是否存在可读的 quiet/outstanding 状态？证书内部 `fault_q` sticky 未导出，板级计数 0 不能反推 fault。依据：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:209`；`docs/research-overnight-progress-20261004.md:80`。
3. **DMA 仲裁可配置性：** 四 DMA 仲裁策略能否运行时切换或必须重建 BIT？切换是否改变 A 总工作量？未知。
4. **IOTLB tag/上下文导出：** 能否导出实际 tag、PSCID/GSCID、global、上下文快照？输入只给静态代码线索，未给完整可观测 ABI。依据：`docs/paper-scenarios-mitigation.md:115,182`。
5. **选择性 PTW 延迟与出口反压：** 当前 DS delayer 聚合多类请求，不能称 PTW 专属；出口反压是否可独立控制未知。依据：`docs/paper-logic.md:79`；`docs/research-literature-review-20261004.md:75`。
6. **逐请求阶段观测：** 请求真实到达、接收前等待、准入、lookup、结果、完成等是否已建设？输入称前置未满足。依据：`docs/research-literature-review-20261004.md:75`；`docs/paper-logic.md:260-261`。
7. **不同 BIT/boot 资源与 Fmax：** 新路线若跨 BIT，必须报告 LUT/FF/BRAM/Fmax、boot id、Linux 版本；v5 已显示不同 BIT/boot。依据：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md:27,43-45`；`docs/research-overnight-progress-20261004.md:46,94`。
8. **外部新颖性：** 所有路线均需定向检索；输入不是穷尽检索。尤其 R1 的 invalidation 生命周期、R2 的上游仲裁因子隔离、R3 的共享域保护单元、R4 的前端/出口正交边界，均待查。输入未提供的来源不能引用。

---

## 10. 最小关键代码/接口需要（本轮不写实现）

### 10.1 Linux / 驱动
- 固定工作集、DID/PID、页表 global/PSCID/GSCID、失效触发、boot id 记录。  
- 关键文件：`linux/drivers/misc/idma.c`、`linux/drivers/misc/idma_research.h`。  
- 输入已提示：旧特性 mask 额外 bit 不必然拒绝，但新实验不得假 role、不得与旧 raw 计量混用；E2/Cost validate 不得放宽。依据：`docs/research-n-prototype-spec-20261004.md:63-68`。

### 10.2 RTL 观测
- 逐请求身份/到达/接受/等待/准入/结果/完成。  
- 失效触发、quiet、outstanding、上下文 generation。  
- IOTLB tag/匹配域/替换来源/global/PSCID/GSCID 快照。  
- DMA 仲裁选择、AR/AW 到达间隔、buffer 占用。  
- 出口反压、翻译完成到受理等待、单响应口仲裁。  
- 关键文件：`rv_iommu_iotlb.sv`、`rv_iommu_tl_wrap.sv`、`rv_iommu_trans_ctl_wrap.sv`、`rv_iommu_top.sv`、`rv_iommu_nonblock_hit.sv`、`dma_nonblock_observer.sv`、`dma_e3_observer.sv`、`ariane_peripherals_xilinx.sv`、`dma_core_wrap.sv`、`axi_demux_simple`。

### 10.3 构建与部署
- 若改 BIT：需要新构建、时序/资源验收、boot id、Linux 版本、SD 是否更新；不得把“无需 SD 更新”外推到所有路线。依据：`docs/research-overnight-progress-20261004.md:94`；`docs/research-n-prototype-spec-20261004.md:67`。  
- 同一路线尽量同一 BIT 内可切换模式；若不能，分启动报告，不把跨 BIT 差异当纯因果。依据：`docs/research-literature-review-20261004.md:72`。

---

## 11. 结束语

本轮提出四条独立路线：R1 失效/生命周期干扰，R2 上游 DMA 仲裁与到达相位，R3 保护单元与共享翻译域，R4 前端 HOL 与译后出口反压。四条都不以“继续调限速”或“优化 N 快通路”为核心；R4 明确把 N/hit-under-miss 当基线而非新意，R3 明确不把 cache partition 称新，R2 明确不把仲裁/QoS 身份称新，R1 明确不把“失效会慢”当新。

这些是待交叉攻击的提案，不是最终方向；外部新颖性均待查，所有实验设想均未执行。Method88 阴性、E2/E3 有限证据、v5 非零机制但未证明性能改善，均作为不可动摇的事实边界保留。