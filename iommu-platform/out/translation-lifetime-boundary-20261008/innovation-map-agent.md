# 第二研究席：整图筛选与可排空的最小翻译前沿

2026-10-08。Codex 亲自推演、核关键原文与源接口。本席只读生产代码；没有构建、运行 DUT、改生产、调用 Claude 或 DeepSeek。普通物理旁路已经消掉旧位置收益的 97.45%，不能沿用原 23.95% 为新机制作证。

## 结论

**只保留一个值得做小判别的研究问题：映射更新开始时，小 FIFO 流式 DMA 究竟必须完成多少翻译与数据义务才能排空；能否在同资源、同顺序下，比普通按片暂停／配对预译址／序号 fence 更小？**

因为 ShareMMU 的进度证明有明确的设备假设，而流式 copy 可以违反该假设，这个适用边界值得核。因为 LTI 已约束完成不能依赖另一笔 Stall 翻译，普通按片 quiesce、预译址与旧映射延期也能表达排空，所以**发现等待环、增加旧 epoch 许可、加一个逃生 credit，都不能作为独立创新**。

当前没有一条已排除强普通对照的新规则。下面的前沿规格是可审查的研究候选／诊断基线。如果它只等于“暂停新读，正常翻译和写出剩余字节，然后再失效”，就作为工程基线收尾。若能证明并实测一个更小的必要前沿／资源—撤销延迟边界，且强普通方法在同信息下不能达到同成本点，才可考虑将那个明确增量作为贡献。

**普通 fault/error completion 也是强控制。** 现 TL ERROR 可以返回错误，而非一直等软件修页。若错误写响应、丢弃已读 payload 并准确报告 copy 失败能安全排空，就不能把“必须继续翻译目的”说成标准失效安全的必要条件。保持整个 copy 成功是额外的应用合同，必须明示并对照普通先 quiesce；不能用它人为排除错误完成。

## 1. 最强直接前作与精确证据

### ShareMMU 的假设

- 官方 [NVIDIA 论文页](https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted) 链出 [camera-ready PDF](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf)。会议为 MICRO 2026，当前尚待召开。
- §V-C7 始于 **PDF 第 8 页**；关键无死锁论证在 **PDF 第 9 页左栏**，Fig. 11 同页。
- 本地 [PDF](../shared-translation-literature-20261006/papers/sharemmu2026.pdf)；[txt 第 508 行起](../shared-translation-literature-20261006/papers/sharemmu2026.txt:508)，流程 508–521／552–557，关键假设 **558–567**。
- 流程先锁 shared TLB，等待设备写回与在途访存排空；权限在排空后提交。其无死锁论证依赖：停在 TLB 的 miss 不占设备内部执行资源，已译址 hit 不被这些 miss 阻止，因而能在有限硬件时间内完成。

这个假设是全文的适用条件，不能写成“ShareMMU 所有实验错误”或“它没有处理在途请求”。它已经处理 back-invalidation、权限有效期、确认与回收。

### Arm LTI 已经约束完成依赖

- 官方 [Arm LTI IHI0089A，2020](https://documentation-service.arm.com/static/5fc5124e6620e02e2b67ebb4)。本地 **PDF**：`out/dependency-contract-innovation-20261008/papers/lti2020.pdf`；**txt**：`out/dependency-contract-innovation-20261008/papers/lti2020.txt`。
- [txt 1733–1738](../dependency-contract-innovation-20261008/papers/lti2020.txt:1733)，**§6.2.1**：收到翻译 response 后，completion 不得依赖后续软件活动，或其它 LAFLOW=Stall 的 translation response 返回。
- [txt 1741–1752](../dependency-contract-innovation-20261008/papers/lti2020.txt:1741)，**§6.2.2**：同一结果可用于多笔事务，但 completion 仍须在合理时间返回；所有使用事务须已全球可观察，还须避免与等待失效完成的 CPU barrier 形成循环。
- [txt 706–727](../dependency-contract-innovation-20261008/papers/lti2020.txt:706)，§1.4.1 已明确 Stall 流可能因软件依赖形成系统死锁。

因此“完成必须覆盖依赖”以及“不能持着结果等任意后续翻译”已有规范前史。本平台没有实现这套 LTI completion；不能把 TC 地址握手即 IDLE 解释成 LC 未退休。

### 当前平台能证明什么

- [iDMA transport 254–257](../../cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:254)：RREADY 取决于输入 byte FIFO 是否有空间。
- [iDMA transport 343–360](../../cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:343)：W 能推进才弹出 FIFO 字节。
- [iDMA buffer 33–50](../../cva6/corev_apu/iDMA/src/idma_buffer.sv:33)：每个字节通道是有限 stream FIFO；小配置不是完整 burst 缓冲。
- [真实 copy 元数据接口](../../tests/nonblock_actual_dma_top.sv:638) 导出了 job 身份、src/dst/length；使用它须计算接口与状态代价。
- [自然写序报告 §4](../../docs/natural-write-order-boundary-20261008.md) 已有真实 R 尾部→FIFO→W 队头→其它 DMA 翻译的等待见证，但该记录**没有映射更新／失效暂停**。
- 根本轮已核：当前 FPGA `InclATS=0`，Linux 默认 IOFENCE `PR/PW=0`，CQ 没有 TC/data retirement 反馈。现有静态实验不能证明下面的 live-update 状态已在生产可达。

## 2. 小反例：仅否定“任何流式 DMA 都满足 ShareMMU 假设”

先写清这是**可配置的新原型中的 temporary translation lock 测试**，不是现生产 bug，也不是当前 Linux dma_unmap 合同允许任意在途撤销。普通 live IOTINVAL 不等于 ShareMMU 的 shared-TLB 全局锁；不能把自然失效命令当作第 3 步 gate。若必须加入新锁／epoch 协议才出现这个状态，范围就仅属于这个新协议。

1. 设备 copy 已合法接受一个源物理 AR，剩余返回量为 `X`；该请求使用的旧映射仍有效。
2. 目的端的一段 W 尚需一个未返回的翻译。copy 的读写流水允许这个时刻出现；源／目的页边界不同会增加该机会。
3. FIFO 空闲容量为 `C`，在遇到该目的写阻塞前，可写出的源数据为 `P`，且 `X > P+C`。
4. 更新流程停止这个目的 miss，并把失效／更新完成置于在途源 R 排空之后。

此时源 R 最多再接收 `P+C` 字节就停住；它的尾部不退休。目的 W 等翻译，翻译等更新完成，更新完成又等源 R 排空。被停住的 miss 已通过 FIFO 依赖占住设备资源，故不能使用上述“miss 被动等待”的证明。

**它没有证明新协议必要。** 普通目的预译址、完整返回缓冲、先暂停并排空 DMA、或允许正常旧权限服务直至 quiesce，均可能消除这个例子。普通 fault/error completion 也可能让未完成写事务返回失败、丢弃 FIFO 字节并完成正确错误处理，从而安全排空；这项路径本席未运行验证。针对一个未修改的目的页，用精确页／上下文失效避免全局锁更是强普通解法，必须纳入，不能只对比 ShareMMU 的全局锁。

## 3. 先分开三种资格，避免发明失效豁免

| 状态 | 合法性与测试约束 |
|---|---|
| 失效开始前已给出或接受的物理源 AR | 不撤回 VALID，不修改 burst；该源请求的旧授权可用于完成已接受事务，旧页必须保留至真实数据义务完成。 |
| 失效开始前已接受但尚未完成翻译的目的请求 | “已接收”不能自动授权旧 PA。须明确 IOTINVAL 对在途 walk／填充的规定及实现；允许它继续时，旧 PTE／上下文必须仍有效，且最终失效完成必须覆盖其结果。 |
| 失效开始后才派生出的目的请求／源补充请求 | copy 属于旧 job 也不自动给旧映射资格。只能在软件尚未修改 PTE、尚未完成失效前，作为 quiesce 的受控排空工作正常翻译；若要求 after-fence 仍访问旧映射，则必须另有体系结构合同，当前不能假设。 |

下方候选首判采用最保守时序：**先冻结新义务，PTE 不改；排空有限工作；随后更新 PTE、执行正常 IOTINVAL＋IOFENCE；新请求再走新映射。** 这本身就是普通 quiesce，不把它改名成新 epoch 协议。下一次若试验真正的在途失效，也应单列标准合同与 gate，不能用诊断 stall 冒充完整失效实现。

## 4. Codex 提出的有限前沿规则：作为研究规格，不认定新颖

研究对象是固定已接受工作在暂停点 `t0` 的**最小排空集合**，不是完整 job 的所有未来页。

输入只用同样可给强普通控制的信息：copy ID 与字节区间、已给出／接受的源 AR、剩余 R 字节、目的 AW 区间与真实 W 序列、FIFO 使用量、翻译上下文及有效代数。不能硬编码实验 PA，不能把瞬时 WREADY 当保证。

规则：

1. 在源 child AR **首次 VALID 之前**冻结新增源工作；已保持 VALID 的请求仍握手。记录 `t0` 时已经形成的 R 返回义务及 AW 写义务。
2. 将每份 R 剩余字节映射到它必须消费的目的 W 区间。需要补目的译址的区间加入集合；沿真实 W 顺序加入挡住这些区间的先行 W 义务。
3. 若先行 W 尚无数据，将为其提供数据的准确源区间加入集合。只准为这些已形成义务补工作，不准因整 job 尚未完成就继续无限发新源读。请求边界不可修改时，按真实 burst 扩张，不能假装只付有效字节成本。
4. 迭代至不再增加区间。若得到的集合可在现有 FIFO、metadata 和顺序下排空，就完成这份集合，然后执行普通映射更新与失效；若集合无法在容量内前进，返回“需要普通更大缓冲／更早成对预译址／更小源 burst／更广 quiesce”，不偷偷放宽容量。
5. 首判仅静态旧映射、正常对齐／单个错位 copy、无 fault、无 ATS。不在结果中宣称已经支持任意失效、epoch 取消、故障恢复或 LTI LC。

这个迭代只是把已有义务表达为可核的闭包。**如果普通按片暂停＋相同依赖扫描计算出同一个集合，就不能把闭包算法作为独立机制。** 若一个更便宜的普通 conservative 集合在所有候选输入同效，也停止精细化。

### 可尝试证明的性质与锐界

- 对选择的更新点，若 `X > P+C` 且没有其它合法消费者，则只停 miss 无法排空。这是一个容量必要条件，不是世界首次发现 buffering 的主张。
- 对最终集合，若每笔必要翻译与数据服务均有有限上界、没有集合外请求占住不可旁路 W 前序、服务选择公平，则有限集合可以退休。**这些前提必须逐项验证，集合有限本身不保证死锁自由。**
- 真正值得研究的增量是：在给定 payload／metadata 预算和最大更新等待 `D` 下，普通整 job pin／固定成对准备与精确前沿的成本可达范围是否不同。如果理想前沿也无额外空间，就不写新 RTL。
- 不能把 `Σ独立请求延迟` 当撤销时间。物理 burst、W 序、PTW 串行、共享数据返回和重叠服务均进入边界；需要给最坏与负例。

## 5. 最强普通控制与唯一最小判别

| 控制 | 必须配平的内容 |
|---|---|
| 整 job quiesce／pin | 完整 job 范围，测等待时间与旧映射保留量；不能让新规则只测一个已完成片段。 |
| 普通按片暂停 source，再让目的译址和 W 排空 | 相同暂停点、copy 信息、源 window、FIFO 与 W 序；这是最容易覆盖候选的基线。 |
| 普通 sequence fence／epochs | 覆盖已给出 VALID、已接收未译址请求、派生写义务；不能故意只数物理 AR/AW 握手。 |
| 源／目的成对预译址，预约最坏尾部 | 相同未来可见范围、全部结果／header／credit 状态与权限语义。 |
| 完整返回缓冲／W 齐备再 AW | 含真实所有 payload、公共 FIFO、cuts；不能只算新增模块。 |
| 精确页／上下文失效＋正常 quiesce | 对未改映射的必要目的请求持续服务；避免人为全局锁。 |
| 普通 fault/error completion 与合法 discard/drain | 报告真实任务失败，保持 AXI beat／ID／响应与残余数据处理合法；既不能伪报 copy 成功，也不能因为研究偏好成功就不测此进度控制。当前未验。 |

下一次只建议一个**新诊断副本**，不先生产或 BIT：两真实 DMA／同窗口、同 FIFO，加入由真实状态触发的 `pause_new_source` 与翻译服务暂停 gate；记录首次 R→W 依赖、每笔 old obligation 和暂停/恢复/失效完成事件。可先做单 DMA 反例，随后加入一个跨 DMA W 先行义务。正常无 pause 的轨迹应与引用的普通旁路基线一致。

最小输入选 `X>P+C`、源／目的页边界错开的一段，暂停点固定在**真实已接受 AR 与未完成目的准备同时出现**的时刻。若找不到这种状态，就不能用 driver 强制非法 AW/AR 顺序补反例。首判可以只实现 pause/quiesce，不称完整 IOTINVAL 功能验收。

判别顺序：

1. 先验证普通按片暂停＋排空是否完成、集合是否正好等于候选。
2. 若同效同成本，当场淘汰新机制；保留 ShareMMU 适用性边界记录即可。
3. 只有集合或可达成本点确有差异，才比较固定成对／精确失效／完整缓冲，随后做最多两种预算的成本—更新延迟点。

验收不以微小 mean 百分比为主：成功 copy 要求正确完整数据；错误控制要求实际返回失败、准确处理允许的部分写与残余数据。两者都验证 guard、R ID/RLAST、保持 VALID、W 归属、停止集合外新工作、最终排空；明确 mapper 在何时可改 PTE／回收旧页。记录延迟、payload、全部 metadata、重译、PTW 与 data 物理流量、未被更新流的尾延迟。不把功能 state bits 当综合面积，也不把成功 copy 合同等同标准失效安全。

**退出条件**：普通 source pause／sequence fence 同信息同资源复现决定；精确页失效已经去掉环；真实合法工作流只允许整 job 完成后 unmap 且没有按片暂停需要；理想前沿无净成本空间；主要问题由原型共享返回顺序或已知工程 bug 决定；只有诊断 gate 制造收益。任一成立，就不能把该路线立为独立机制。

## 6. 其它整图路线：本轮淘汰，不继续凑机制

| 路线 | 最强覆盖与淘汰原因 |
|---|---|
| PTW／数据物理放置、bank 着色 | Mitosis、MGvm、CLAP 已显式安排 PT／data 布局；[Phoenix 作者稿](https://arxiv.org/abs/2502.10923)还联合 scheduler、PT placement 与带宽 QoS；[Virtuoso 官方代码](https://github.com/CMU-SAFARI/Virtuoso)及 ASPLOS2025讲稿已显式纳入 PT／data DRAM 干扰。普通 per-stream arena／PT pool 着色／复制尚未被排除，所以不能把“把页表搬到空闲 bank”立新。现有 FIFO DDR 模型也没有实际 bank/row 调度，直接沿用旧 DUT 不能验该主张。 |
| 同 walk 多 DMA 的收益／费用归属 | DWS 已研究公平的 walker 份额；ShareMMU 已用 sharer-aware AA-LRU 管理共享资源；[RISC-V 官方 QoS 扩展](https://docs.riscv.org/reference/cbqri/qos_iommu.html) 已把 PTW 的 RCID／MCID 归于 DC.ta。第一请求者、均分、按 bytes 分摊都还只是普通计费选择；fractional DRR 与翻译后流量门尚未被排除，没有具体必要工作流和独有性质，故不立新。 |
| 非阻塞 walker／spill continuation | 新找到 [NB-Walker 作者 artifact，v2，2026-07-22](https://zenodo.org/records/21488827)，原图仅有题录。作者代码显示 primary 等内存时可接受 secondary；secondary continuation 发到 L1V 后释放本地槽，后续返回再接续。SoftWalker 的 In-TLB MSHR 与 LATPC 压缩也已有资源转用前史。不能把“释放 walker 占用、存续表状态”当空白。 |

NB-Walker 只读证据已保存：`map-agent-sources/NBWalker-MICRO2026.tar.gz`；只取 README/setup 指定 commit `8837da7dda5591837877d20d4ad51b042c988aad` 的六个必要源文件，位于 [源码目录](map-agent-sources/nbwalker-source/simulator/mem/vm/mmu/nbwalker/NBWalkerMMU.go)。未执行 artifact 脚本、构建或复现实验；没有取得其论文全文，不从代码反推所有论文结论。相关核心函数：`CanAccept`、`walkSecondaryPageTable`、`sendWriteReqToL1V`、`handleL1ReadResponse`。

本席读根／平台 AGENTS 与当前 §39，定向核研究地图、LTI、ShareMMU、iDMA buffer/transport 和真实接口；没有默认重读 Linux、生产 .git 或全部 out。原始来源读取与候选推演由 Codex 完成；没有把模型共识或未运行的规格当实验。
