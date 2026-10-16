# 原论文驱动的创新筛选：翻译准备、读写分段与有限缓冲（2026-10-08）

本轮结论：暂停继续增加复杂扫描。保留一个更窄的研究命题——**共享 IOMMU 下，源读和目的写分段不一致时，怎样兼顾长读 burst、较小返回缓冲与正常前进。** 目前只有候选规格及文献/源码证据，尚未证明独立创新、性能收益、面积收益或论文分区。

这是既有多 DMA 翻译干扰的性能/成本主线。它扩展到 DMA 搬运约束，但不能解释当前 CONF=0、对齐 4 KiB 实验的 PN 退化。后者仍沿[上一轮原始证据](../out/read-service-boundary-20261008/codex-review.md)定位写顺序/目的准备。

## 1. 本轮实际阅读与资料

对照下表 14 篇原论文的关键机制段落；这不表示新增 14 篇，也不表示逐字通读所有全文。DeepSeek 首轮处理七篇全文及现有审定，之后两轮针对候选质疑；Codex 亲自阅读关键原文、实际 legalizer/frontend/Linux，设计并裁决。

新增本地三份公开 PDF 及转换文本：iDMA 2024、CTC 2009、AlphaServer 1996，[下载来源与 SHA256](../out/paper-driven-innovation-20261008/papers/sources.json)。其余复用既有全文；2026 Preemptive VC 本轮阅读作者 arXiv HTML。下表链接均指原论文；AlphaServer 是原期刊 PDF 的镜像。没有以博客、模型共识或检索不到替代机制证据。

| 原论文 | 关键机制及适用场景 | 对本路线的约束 |
|---|---|---|
| [AlphaServer I/O，1996](https://shiftleft.com/mirrors/www.hpl.hp.com/hpjournal/dtj/vol8num4/vol8num4art5.pdf)，pp.71–72 | PCI 桥预取数据；第三预取缓冲为后续 DMA 及 scatter-gather 翻译保留资源 | “给翻译留一个空间”和预取/缓冲联动是经典方法 |
| [CTC，NOCS 2009](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)，§4 | 按消费归还 credit；credit 耗尽可结束当前 NoC packet、释放路径，逻辑连接继续 | 共享 credit/解除返回路径占有本身不新；其 packet 可以切断，外部 AXI 已接受 burst 不能直接照搬 |
| [BIS，ASPLOS 2012](https://people.inf.ethz.ch/omutlu/pub/bottleneck-identification-and-scheduling_asplos12.pdf)，§1/机制 | 按线程等待周期识别并加速实际瓶颈 | “谁挡住别人就优先谁”已有强经典，移植到 DMA 不是自动创新 |
| [MASK，ASPLOS 2018](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)，§4 | 多应用 GPU；warp token 控制 TLB 填充，miss-rate epoch 调整；PTE/应用关键性调度 | 分配份额、填充保护、PTE 优先已有；token 不等于完成头的实际占用 credit |
| [Scheduling PTW，ISCA 2018](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)，§3 | 同指令 walk 批处理，按 PWC 估计总工作量选择短批次，含防饥饿 | 指令依赖和剩余工作调度已有；没有直接证明等价于 AXI R→W 的容量/顺序合同 |
| [MMU-aware DMA，ICCD 2018](https://arxiv.org/pdf/1808.09751)，§IV/§V | 异构 SoC；记录 burst 元数据，TLB miss 后排空在途，再按序重试；降低完整数据缓冲需要 | 是直接、强先行工作。区别只能建立在明确的“无故障重试/不中断有效传输”合同及成本上 |
| [DWS，HPCA 2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)，§V | walker 按租户分配，空闲时借用；DWS++ 按待处理量放宽借用 | 动态共享/软隔离、吞吐与公平折中已有 |
| [Orchestrated，DAC 2023](https://libingyao.github.io/assets/files/DAC2023.pdf)，§III | 按 SM 翻译 miss-rate 调度 TB，配合 TB 缓存集合分配/共享 | 联合调度＋缓存分配不宜再作为宽泛新意 |
| [iDMA，IEEE TC 2024](https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf)，§II-C | 源/目的合法化分段、byte-stream transport、小 FIFO；必要时完整传输缓冲 | 读写解耦、直通、小 FIFO/大 SRAM 取舍已明确讨论 |
| [AXI-REALM，2025 完整版本](../out/rule-novelty-check-20261007/papers/axi-realm2025.txt)，§II/irealm | AXI 写无 ID 导致地址/数据绑定；碎片化后收齐写数据才放 AW/W | 必须对比碎片化＋完整写缓冲，不能把 W 预留问题当新发现；[DATE 2024 短版本](https://past.date-conference.com/proceedings-archive/2024/DATA/493_pdf_upload.pdf)与本地完整版本区分 |
| [SoftWalker，MICRO 2025](https://zoon17.github.io/pdfs/SoftWalker_micro25.pdf)，机制部分 | GPU 软件 walker、翻译专用存储及调度支持 | 复用闲置计算/缓存资源、增加翻译并行度已有 |
| [Soar/Alto，OSDI 2025](https://www.usenix.org/system/files/osdi25-liu.pdf)，§2/设计 | 分层 CPU 内存，访存关键性还受延迟与 MLP 影响 | miss 数/热度不等于端到端收益；不是 DMA 缓冲规则的直接前作 |
| [cuPTW，POMACS/SIGMETRICS 2026](https://maxkev1n.github.io/publications/sigmetrics-2026/3805633.pdf)，§5.3–5.4 | 闲置 CU 执行 walk；MSHR 饱和时在 L2 TLB 转发，避免在更后级转发仍被前级占用挡住 | 前沿已关注“算得快但前级资源没释放”；具体瓶颈位置与资源寿命必须明确 |
| [Preemptive VC，arXiv 2026](https://arxiv.org/html/2607.01430v1)，§III | DMA 内部 R→W 与共享 NoC 链路可形成等待环；比较物理分离及 VC，实现 flit 级抢占 | 通用 DMA 环路消除不是空白；候选须与分离流量/普通 VC 比成本，不能撤销外部已持有的 AXI VALID |

关键原文定位：cuPTW 文本 `765–779`；MMU-aware DMA `380–418`；AXI-REALM `305–354`；iDMA 新文本 `284–298,325–328`；CTC 新文本 `473–490`；AlphaServer 新文本 `780–804`。文本位于本轮资料目录或既有 `out/shared-translation-literature-20261006/papers/`，行号属于具体本地转换版本。

## 2. 从论文比较得到的筛选

| 候选 | 最强适用先行方法/普通对照 | 当前判断 |
|---|---|---|
| A：先预翻译，再决定谁进入全局写顺序 | MMU-aware DMA/预翻译＋ready queue＋普通数据缓冲 | 宽泛思想已有。若做，必须把选择移到源 mux 首次 AW VALID 之前；仅延迟后级 AW 无法改已承诺 W 顺序。没有同合同成本优势，暂不实施 |
| B：沿实际阻塞链选择 PTE/数据服务对象 | PTW batching/SJF、MASK criticality、BIS/优先级继承 | 尚无合法、非闭环且普通 owner 优先做不到的性能反例；保留问题，不认证新调度 |
| C：完成头持有至消费/退休，按生命周期收取额度 | 普通有限队列、credit、容量预留 | 普通资源记账强烈重叠；单独不推荐为贡献 |
| 本轮 D：长读返回先直通可排空前缀，只预约其余后缀 | iDMA 小 FIFO、整返回预留、短 burst、准确 per-segment credit 的直通共享池 | 条件保留。需要证明新的容量/服务边界或更低实现成本；不能把普通 cut-through 改名当创新 |

先行论文没有直接覆盖某个合同，不能推出“没有人做过”。同样，“普通硬件可编程地表达同一规则”也不是否决一切新算法的标准。应比较**已经给出的具体策略、保证、代价与实际结果**；若最强普通策略在同条件下同决定、同成本，则该机制贡献确实不足。

## 3. 平台可达性：纠正配置前提

实际链路：

- 前端配置 bit0 送入 `opt.beo.decouple_rw`：[frontend:367](../cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv#L367)，寄存器定义 [hjson:55](../cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.hjson#L55)。
- [legalizer:202](../cva6/corev_apu/iDMA/src/idma_legalizer.sv#L202)：关闭解耦时，R/W 都采用两端更近的共同边界；开启后各自分段。独立前進握手在 `453–464`。
- 当前 64-bit 数据宽度、256 beats，对应合法化虚拟分段上限 **2048B**，不是直接 4096B：[legalizer:82](../cva6/corev_apu/iDMA/src/idma_legalizer.sv#L82)。
- [Linux:419](../linux/drivers/misc/idma.c#L419)及[test:1225](../tests/nonblock_actual_dma_test.cpp#L1225)、`1715` 都写 CONF=0。因此当前实验没有启用独立读写分段。
- `RAWCouplingAvail=0` 是另一个编译参数，不能与 `decouple_rw` 混为一谈；[backend:811](../cva6/corev_apu/iDMA/src/idma_backend.sv#L811)决定 R→AW coupler 是否实例化。

源码算例，**没有运行 RTL**：2 KiB copy，src 在 4 KiB 页首，dst 页内偏移 3072B，均按 8B 对齐。

| 配置 | 源读字节序列 | 目的写字节序列 |
|---|---|---|
| CONF=0 | 1024,1024 | 1024,1024 |
| 仅 bit0=1 | 2048 | 1024,1024 |

第二种允许“一次源读跨越两个目的翻译页”，源读本身仍不跨源页。真实场景可从不同页内位置之间搬运局部数据、稀疏块/张量片段引入；当前实验应用不具备这个前提，不能假定这种形状具有代表性。需说明为什么软件对齐/共同分段不是足够便宜的解决办法。

## 4. Codex 独立提出的窄机制与容量命题

例子：源物理 AR 返回 2048B，目的第一段 1024B 已获有效翻译、当前确有资格写出；第二段目的页尚待 PTW。

候选动作：

1. 在新的物理 AR 首次 VALID 前，识别本次 copy 的**逻辑字节区间**及准确目的覆盖；不靠“同 DMA”或累计 AW 字节数匹配。
2. 只把不会等待该读返回之后的翻译/数据来解锁的连续前缀计为 `p`。本例候选 `p=1024B`。
3. 后缀 `L−p=1024B` 在共享池中独占预约；前缀使用共同小 FIFO 流向当前写段，必要时合法反压 R。
4. 第一写段实际消费后，将后缀完整吸收，物理 RLAST 可以握手，释放此前被该响应占有的返回服务路径。
5. 后续 PTW 获得服务、第二目的段可写后，再向真实 DMA 回放后缀。原逻辑 burst 的 RLAST 仍只在全部数据交付完毕时出现。
6. 无可靠前缀就按整 burst 预约；空间不够时不发新的物理 AR。已持有 VALID 和已接受 AR 不撤销，不制造假成功响应。

**条件性容量命题草案**：在“已承诺前缀最终能独立排空、R 可合法反压、前缀的流水暂存有共同 guard、后缀在物理响应结束前没有消费者、物理响应结束后待处理 PTW 确能被公平服务”的合同内，后缀池至少要保存 `L−p` 字节；用同量独占预约加共同 guard 有望充分。它是本例的守恒论证及待形式化规格，**不是全系统最低容量定理或新颖性证明**。

相较整响应池，本例名义后缀容量 1024B 对 2048B。不能称 LUT/面积减少 50%，也没有测到吞吐增益。相较共同分段短读，候选保持一个源 AR，对照有两个；少一次请求不必然更快，当前 DDR 可以流水多少地址请求仍需测定。

仅有 WREADY 当前为 1 不能保证未来；必须证明前缀所依赖的写资源不会转赠，且写服务不以这个读 RLAST/PTW 为前提。没有这种服务合同，`p` 应为 0；这也是必须检查的真实架构边界。

多 AR 的后缀预约总和必须不超过池容量，逐消费释放。一次前缀保证不能给两个 AR 重复使用。**AR 已承诺后不能随意减小 p**：减少保证意味着需增加尾部预约，必须先原子取得新增空间，否则原保证保持到实际消费完成。job/source/ID/属性/权限/失效 epoch 也属于接口合同，不能省掉。

## 5. 为什么普通方法可能已经解决

最关键对照不是弱的整响应池，而是**同样知道目的每段覆盖、写顺序、可排空条件的普通 cut-through＋byte-credit**。

- 若它同样预约后缀，动作与 D 相同：D 没有独立机制新意。不能故意去掉其信息或 credit 来制造优势。
- 若它只在池满时反压，会不会把唯一返回通路留给等待后续 PTW 的尾部，须按实际响应能否交错判断；“允许 RREADY=0”与“反压后别的 PTE 响应能越过”是两件事。
- 若普通方案能在 response 内切换 ID/VC，D 的返回路径问题可能消失；需把这种方案的队列、端点与互连成本计算进去。
- 若源 AR 与目的分段同步缩短已解决，且额外请求能被流水隐藏，D 可能只有缓冲代价、没有性能空间。
- 如果每个目的翻译都已准备或 `p=0`，D 退化为直通/整预约，不能构造伪收益。

**论文有可能的产出**是“真实接口合同下的容量—长 burst—翻译服务边界＋较低成本的实现＋强对照”，而不是先声称发明 credit。若普通方法已经给出同样边界及实现代价，停止 D 主贡献。

## 6. 对 DeepSeek 否定/安全论证的直接审定

三份完整答案保留，错误不在原稿里抹去；[资料导航](../out/paper-driven-innovation-20261008/README.md)。

已纠正第二轮：无时间的四 DMA 进度模型不能排除性能调度空间；已有 AR 顺序元数据不是“不可知未来”；MASK miss-rate token 不等于完成头 occupancy credit。第三轮接受了这些修正。

第三轮仍有四项不采用：

1. **“只有物理 R 不可反压，D 才有意义”不成立。** AXI 可反压，但占有的返回路径可能不能被后续 PTE 越过；相反完全不可反压还需要实际写速率/全前缀缓冲保证，门槛更高。
2. **“p 只可下调”不安全。** 下调 p 会增大容量义务；已有预约不足时，必须保持原承诺或先补充预约。
3. **把 WREADY 或已准备 AW 当成排空证书不成立。** 存在既有写顺序、bank 锁定和内存端点依赖；保守合同不能凭瞬时 ready 建立。
4. **“per-segment 信息可表达，因此无独立规则”不足以完成论文判断。** 信息可得不证明已有具体最强策略已满足同进度/成本；但若实际策略确能同样预约，候选机制就应降级。是否等价需要明确对照，不能由“普通”这个名字推导。

第三轮以 128-bit 举算例，不能直接充当当前 64-bit 平台证据；本报告已用实际源码 2 KiB 合法化边界重算。cuPTW 选择转发原文位置以本地 `765–779` 为准，未沿用首轮略错的引用行号。

## 7. 下一步最小判别与停止条件

先做窄判别，不进入 Linux/BIT 全构建：

1. 复用真实 iDMA 测试入口，只加独立分段及上述偏移形状，确认 R2048/W1024+1024、实际 W 承诺和响应返回顺序；这一步尚未执行。
2. 同时比较：普通整返回预约 F、共同分段短读 S、普通准确 per-segment 直通池 C、候选 D。保持相同字节/在途/翻译状态/公共 FIFO/服务模型，统计 V/A 完成与吞吐、burst 数、PTE 等待、预约峰值和全部缓冲/元数据成本。
3. 优先检查 `p` 保证是否在真实 AW/W/DDR 依赖下成立；若只能 `p=0`，停止本例 D。
4. C 与 D 若同准入、同容量、同成本，停止机制创新推荐；S 若同吞吐且更简单，停止长读优势推荐。
5. 至少一个有代表性的形状通过上述差异判别，才继续四 DMA、公平性、失效/故障合同及综合；不凭一条受控轨迹自动上板。

本轮**没有实现 D、没有测试/仿真/构建，没有 RTL/Linux/SD/板卡操作**。最新真实结果仍是§26四次 RTL 仿真；本报告的字节算例、进度合同、文献空缺均不当实验结果。

DeepSeek 贡献为七篇证据整理、三候选质疑及 D 审查，三次 high/thinking/max393216、均 stop/未截断，总 provider tokens **448607**；它没有命令/写文件/浏览能力。Codex 亲自提出 A/B/C/D、阅读关键原文和接口源码、裁决与修正规格，并下载/保存资料及更新交接。本輪未调用 Claude 或新 Codex 子 agent。

