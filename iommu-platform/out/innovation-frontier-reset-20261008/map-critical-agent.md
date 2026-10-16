# §44 独立关键审查：共享 walk 的完成扇出与结果驻留

日期：2026-10-08。作者与执行：Codex；没有调用 DeepSeek 或 Claude；没有修改 RTL、运行 DUT、构建或重跑旧实验。只定向读取 §43 决策、生产相关接口和两项 primary 来源；下载的官方 gem5 源码保存在本目录 `map-primary/`。

**结论：停止把“共享 walk 后让慢 DMA 不阻塞快 DMA 的结果返回”作为本平台的新机制候选。** 既有合并与共享 TLB 已覆盖重复翻译复用；生产 TC 在请求已接受后无条件接收翻译结果，数据通道的 AW/W 背压发生在结果已经保存之后。若先新增会阻塞完成的 coalescer，再用结果槽、就绪优先扇出或 waiter 位图修复它，这个收益来自所选实现，不能作为独立创新。

本轮只落这一候选及其 kill 理由，不重命名 §43 的 W 顺序/byte-debt 规则，也不扩大为另一张文献地图。剩余值得保留的是一个有严格条件的存储—重复 walk 分析问题；下面同时给出普通同效构造，不能把这个问题已经存在等同于新贡献成立。

## 1. 候选原理与合法事件序列

拟议机制是：相同完整翻译 key 的多个请求只发一次 walk；完成后，向可接收的 subscriber 返回，未接收者保留必要结果/引用；walker 不等待整个扇出完成，再服务另一 DMA。完整 key 至少要保持地址空间与有效配置一致，不能只看 VPN；还必须保留每个请求的访问类型和独立异常判断。本文比较同一配置、同一访问类型的请求，避免把权限分叉隐藏在共享命中里。

候选想利用的反例是：

1. DMA A 与 V 请求同一 key `x`，被合并到一次 walk。
2. walk(x) 完成时 A 的翻译接收端 `ready=0`，V 的 `ready=1`。
3. 一个把整组完成与所有 subscriber 握手捆绑的实现保留 walker/完成项，随后 V 的不同 key `y` 不能进入。
4. 解耦实现把 x 给 V，将 A 的结果停放，马上处理 y。

这是合法的通用 ready/valid 接口序列，但**步骤 2 不由当前正常 DMA 数据背压推出**。生产 TC 的真实序列是请求握手 → AWAIT_RESP → 接收并保存结果 → COMPLETE → 等待物理 AR/AW。因此上述反例不能通过“让 A 的物理 AW 或 W 卡住”在本平台实现。也不能将 pause 的数据准入关闭改成翻译结果 ready 关闭来人为满足步骤 2。

## 2. 生产可达性：结果返回与数据退休确实分开

关键原文路径均相对 `/home/xvhaocheng/work/iommu-platform/`：

| 边界 | 直接源码位置 | 结论 |
|---|---|---|
| 请求接受 | `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:675–681` | REQ_TRANS 的 valid/ready 握手进入 AWAIT_RESP。 |
| 翻译接收 | 同文件 `693–702` | AWAIT_RESP 中 `trans_resp_ready_o = 1'b1`；valid 到来时写 `trans_resp_data_n`，随后进入 COMPLETE/MRIF。 |
| 数据地址背压 | 同文件 `718–728` | COMPLETE 才等待 `axi_comp_resp.ar_ready/aw_ready`。这时翻译结果已保存在 TC。 |
| TL 生命周期 | `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:1545–1567` | 正常结果的 `slow_resp_valid && slow_resp_ready` 使 TL 回 IDLE，不等待数据 RLAST/WLAST/B。 |
| bank 并发约束 | 同文件 `282–286` | 当前接口明确一次/bank，一个结果端口；被拒的非阻塞 probe 不握手，稍后原仲裁路径接受。 |
| 重查已有结果 | 同文件 `271–280, 1280–1331` | 共享 IOTLB lookup；PT 状态看到 IOTLB hit 即转 COMPLETE。pending 请求可以通过普通后续 lookup 获得先前 walk 的填充结果。 |

这个结论限定正常 live translation 请求；它不是 reset、协议错误、未来多请求 TC 或新增端点接口的普遍证明。Fault log 与 two-stage 特殊路径由另一研究席单独核查，不能借未验特殊路径宣称本候选在生产可达。

## 3. 最强前作及其覆盖范围

### 3.1 HPCA 2017：异步加速器之间的重复翻译复用已经是明确目标

Jason Cong、Zhenman Fang、Yuchen Hao、Glenn Reinman，**Supporting Address Translation for Accelerator-Centric Architectures**，HPCA 2017，DOI `10.1109/HPCA.2017.19`。

- [官方作者 PDF](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf)
- 本地 PDF：`out/shared-translation-literature-20261006/papers/accelerator_translation2017.pdf`
- 本地全文：同目录 `accelerator_translation2017.txt`。
- §II-C，PDF 第 4 页附近的 tiling 例子，txt `244–262`：不同加速器不是 lockstep，简单 GPU 时间窗口合并不能充分消除重复需求。
- §IV-C，从 PDF 第 6 页开始，txt `429–440`；第 7 页 txt `481–489`：一个加速器填入共享 TLB 的条目立即能供另一加速器使用。
- 第 7 页 txt `502–504`：共享 TLB 明确有 hit-under-miss；`505–522`讨论独立替换与失效协调。
- §IV-E 的实现段，PDF 第 8 页 txt `598–615`：context 标识、CPU 常见单 walker、等待队列与结果 demux 已被分别讨论。

**覆盖：** 不同步请求的 translation reuse、共享 IOTLB 过滤重复 miss、命中绕过 miss、结果路由。**本文未直接给出的量：** 非接受端点下每个未交付结果的驻留合同与隔离上界。但当前 TC 是接受端点，因此这项未建模范围不能直接提供本平台差异。

### 3.2 官方 gem5 coalescer：结果扇出本身已经有具体实现

官方固定 revision：`392c1ced53827198652f5eda58e1874246b024f4`，committer 日期 **2020-09-10 23:02:28 UTC**，由[官方 commit 页](https://gem5.googlesource.com/public/gem5/+/392c1ced53827198652f5eda58e1874246b024f4)核对。该 commit 的主题是 terminology 替换，不能说合并机制首次发表于该日；它证明这些实现至少已出现在该版本。

- [官方 cc 源文件](https://gem5.googlesource.com/public/gem5/+/392c1ced53827198652f5eda58e1874246b024f4/src/gpu-compute/tlb_coalescer.cc)
- [官方 hh 源文件](https://gem5.googlesource.com/public/gem5/+/392c1ced53827198652f5eda58e1874246b024f4/src/gpu-compute/tlb_coalescer.hh)
- 本地精确副本：`out/innovation-frontier-reset-20261008/map-primary/tlb_coalescer.cc` 与 `.hh`；URL、bytes、SHA256 见 `map-primary/sources.json`。以下行号是 **本地原始源码行号**，不是 Gitiles HTML 行号。
- cc `95–124`：同页与同读写模式检查；不据此推定所有上下文检查都在这个函数内，也不据此声称实现错误。
- cc `140–211`：为 represented requests 更新 PA，并沿各自 return port 扇出结果。
- cc `211, 214–222, 369–374`：此版本没有使用 `sendTimingResp` 的返回值，完成后安排同周期清理；接收下层结果的函数返回 true。说明这条路径没有实现拒收结果的停放/重试合同。
- cc `393–414, 435–447`：同一页仍有 outstanding 请求时延后 probe。它并非只有一个“合并”标题，而是同时具有 pending-key 跟踪、请求暂存与返回扇出。

**覆盖：** 相同 key 合并、waiter 跟踪、完成传播及 pending-key 抑制。**遗漏范围：** 拒收结果的端点合同。该模型边界与当前 TC 的实际接受行为相容；不能据此声称论文或模拟器评估错误。

## 4. 最强普通 baseline：同信息、同资源下怎样复现

第一对照是**现有共享 IOTLB 的 deferred relookup**。在串行 miss 路径上，x 的第一次 walk 填充 IOTLB 后，同 key 的后续请求重查即可命中。若不存在中间失效/驱逐/配置变化，两次同 key 请求只需一次 walk；增加显式 coalescer 至多减少部分 request/lookup 服务，不能把“一次 walk”当成新机制的独有收益。这个条件命题不承诺任意并发/替换下都只 walk 一次。

第二对照是**已有逐 bank 结果寄存器＋普通 waiter 位图**。每个已接受请求都占本来存在的 TC 槽；共享 walk 完成时写对应槽并逐个返回，释放 walker；数据通道后来何时接受 AR/AW 不影响结果驻留。若扩成多 outstanding/bank，就给同总字节预算下的逐请求 mailboxes，或共享结果 entry＋consumer bitmap。位图、结果槽、按 ready 扇出不是特殊 IOMMU 规则。

若担心复制结果浪费空间，普通共享结果＋引用计数同样只存一份 payload；若担心慢 subscriber 钉住共享表，普通 per-tenant 完成槽预约和待发队列已经提供隔离。若允许丢掉可重建结果，则 deferred-hit/replay 也必须作为控制，不能把所有 replay 一概排除以制造新容量优势。payload、key、waiter ID、位图、TC 寄存器、IOTLB 保留项和重查端口成本均计入总预算。

## 5. Codex 推演：留下的边界在哪里，为什么还不是机制贡献

限定固定映射、完整语义 key 相同才共享，允许任意 endpoint 暂时不 ready。令 `U(t)` 是已经完成但至少一位 subscriber 未收到的**互异结果 key**集合。若同时要求：

1. 这些已接受请求最终成功返回；
2. 不再读取 PTE/重新 walk；
3. 结果不能由已知公式或外部保存的 PA 重建；
4. 翻译结果可任意取值，压缩无法保证更低最坏空间；

那么硬件某处必须保留足以区分 `U(t)` 中全部结果的信息。其位置可以是完成表、逐端点寄存器或仍保留的 IOTLB，不能只统计 walker 内部存储。每个 request 的关联身份也必须另行保留。

小例子：A 的 x 结果未交付，V 已收到 x 并继续请求 y。若只有一份可保留的 payload 空间，普通实现与候选都必须在“保留 x”“推迟 y”“允许日后重建 x”间选择。共享引用位图可以压缩**同 key 的多个 subscriber**，却不能压缩两个任意不同的 PA 结果。若允许 IOTLB 免费保留 x，则两个实现都能将义务转移到 IOTLB；该空间必须计费。

这里可以研究更复杂端点下的存储/额外 PTE 流量/服务隔离 Pareto 边界。但上面只是通用信息保留和队列预约条件，普通 mailbox、共享 MSHR 和 replay 都能落在同一边界上，**不能把这条简单容量命题或图闭包当新贡献**。

**如果**未来一个自然需要多 outstanding/bank 的 DMA 场景在同总 payload/metadata 下确实打破 deferred relookup 与普通完成槽的最优成本点，并且差异来自真实 IOMMU 语义或现有端点约束，而非自造 ready=0/锁/禁止 replay，才值得提出独立机制或组合定理。届时还需要审查经典非阻塞缓存/MSHR/multicast 的最优同效构造；本轮没有证明这些方法不能复现。**因为**当前接收合同与普通构造已经同效，所以停止这个候选作为 §44 正机制。

## 6. 最小判别规格与停止条件

当前不请求新 RTL 或新 DUT run。源码已经判定最初的反例不可达，先做源级 stopping decision 是最便宜的决定性检查。

若后续基于真实功能需求增加并行 walker 或多 outstanding TC，首次判别应限定：两个合法 DMA，同一稳定地址空间，固定映射；x 同 key 的两个请求与另一 key y 交错，保留正常 successful-copy 语义。比较原共享 IOTLB＋deferred relookup、普通逐请求完成槽/共享结果位图和候选三者；总槽字节、key/waiter 元数据、TLB 容量、lookup 带宽及 physical bypass 完全对齐。量计 PTW/PTE AR 数、每次结果实际交付与槽释放、V 的 y 完成延迟和 A 的 copy 正确性。

停止条件：

- 从 AW/W stall 无法产生一个已经接受的正常请求的翻译结果 ready=0：立即停止完成背压假设，当前已满足。
- deferred relookup 已产生相同 walk 数，收益仅来自节省一两次 lookup：不能立 walk-sharing 新机制。
- 普通逐结果槽/共享引用位图/replay 在相同总预算下复现全部事件选择和成本：停止候选，而不是改名字。
- 获益依赖新增全局锁、永久禁止 replay、强制“所有 subscriber 一起提交”、免费额外槽或忽略 TC/IOTLB 保存空间：判别不成立。
- 只能证明通用的 `|U(t)|` 信息保留需求，没有 IOMMU 特有的更强必要/充分条件：作为规格说明，不立论文主贡献。

因此本轮交付是一个直接淘汰结论和可复用的判别接口；没有得到一个未被强普通方法覆盖的正机制，也没有用“未搜到前作”代替新颖性证据。
