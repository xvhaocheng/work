# §44 root Codex：三席创新设计与强普通对照

2026-10-08。root 亲自提出规则、推演必要信息并直接读关键 RTL/primary 原文；两席独立质疑。此文件记录设计，不代表 DUT/板卡验收。

## A. 正常结果交付：接口背压不等于可达瓶颈

DeepSeek 从 TL 的 COMPLETE、响应 RR 和逐 bank ready 连接识别了结构性背压，并明确缺少 TC 消费端证据。root 直接检查 `transaction_controller/rv_iommu_trans_ctl_wrap.sv:675–702`：有效请求被接受后 TC 进入 AWAIT_RESP，响应 ready 恒为 1；结果写入 TC 保存状态后才进 COMPLETE 等物理请求处理。当前每 bank 一个请求对应此状态。

所以 AW/W 背压不能在这个正常请求链中钉住 TL 的共享结果。合并 walk 新增 waiter/结果槽再避免其自身阻塞，不能据此宣称解决了当前平台已存在的慢 subscriber。短暂响应仲裁、fault 路径、不同接口仍须分别分析。

## B. root 独立候选一：无关失效命令不放大热译址等待

### 规则与真实接口见证

候选：fast probe 只因匹配其 DC/PC/IOVA/PSCID/GSCID 依赖的失效而取消；无关命令可重试 probe。接收已接受结果之前必须守原失效顺序，不能撤已经提供的有效请求/响应。

源码见证序列：A 的主 walk 处于 PT；V 为另 bank 的单阶段热请求，fast probe 处于 DC/LEAF；CQ 发仅针对其他地址空间/页的命令。TL `rv_iommu_tl_wrap.sv:320–321` 将任意 IOTINVAL/IODIR OR 成 fast invalidate；`rv_iommu_nonblock_hit.sv:149–172` 可使 probe 进入 REJECT，并到 normal_fire 或候选撤下才离开。主缓存的定向失效可保留 V 对应条目，然而 probe 仍被取消。CQ 命令输出端没有普通 TL/PTW busy 输入（`rv_iommu_cq_handler.sv:32–100,300–362`），故不能假定命令必等 A 结束才发生。

这是条件性源码事件序列；未执行动态命令实验。不能据此认定违规、已发生新 bug，或解释旧静态 PN 差距。

### 强普通击杀与裁决

普通范围比较器＋同版本/结果槽可以实现完全相同的取消/重试规则。2022 年公开的 [US20220414016A1](https://patents.google.com/patent/US20220414016A1/en) 也明确辨认匹配的在途 walk，并标记其结果不进入翻译缓存。它没有证明当前 RTL 已做到这些，但足以禁止把“匹配失效才取消”当新概念。

因为本规则有具体同效普通实现和范围匹配前作，所以停止独立机制推荐。若后续需要该支持，归入工程基线；不为它先做大矩阵或 BIT。

## C. root 独立候选二：两阶段 walk 的 phase union 续走记录

### 新问题和具体表示

单级 walk 与 nested walk 的事务数不同。当前正常 Sv39＋Sv39x4、两边三层、4 KiB leaf、无页表缓存的特定全冷路径为 `3×(3+1)+3=15` 笔 PTE 读。整 walk 不可暂停时，另一设备的译址会等待这条长链；每 PTE 返回后重新仲裁已经是前作，不作为新机制。

root 提出的表示以同样保留的 immutable 原请求/context `C` 为基础，保存下一个 PTE 事务前的续走信息：

- INTERMED：G-stage 目标页号，当前下一级物理页表页号，S/G 等级与累计 global。
- S：即将读取的 S-stage PTE 物理页号；其低 12 位由原 IOVA 与 S 等级恢复；仍保留中间目标以满足必要错误报告。
- FINAL：保存 S-stage leaf 的有效页号/必要属性与 S 等级，由它和原 IOVA 推导 final GPA；与 INTERMED 的目标区互斥复用。不同时保存完整 leaf64、final GPA41 和原 IOVA64 的重复信息。

当前 `packages/rv_iommu_pkg.sv:74–82` 明确 `GPLEN39=41`、`GPPNW39=29`、物理 PPN44；`ariane_soc_pkg.sv:127–128` 为 PAddr56/AxiAddr64。不能按 GPLEN39 的名字将目标算成 39 位。

边界只选已返回 PTE 完成检查且下一 AR 尚未首次 VALID 的时刻。已 offer 的 AR、已 accept 未返回的 R、最终结果/错误尚待接收都不能被所谓压缩丢弃。TL 的 request/DC/PC/权限/context 还要保持或保存，`update_o.vpn/pscid/gscid` 引用输入；`C` 不是免费资源。

### 两个不能省的独立量

在 FINAL 的 S-leaf4K、G-next非根级检查点，下一 G 页表物理页号与 S leaf 的 guest 页号可随合法页表内容独立变化：根/context 固定也不能从其中一个推导另一个。不重读前缀且要求相同完整 IOTLB 内容时，两者分别提供最多44与29位信息；w/x/u/global 属性还需恢复。这仅是固定 phase、固定输出合同的条件性信息论见证，不是所有架构实现的统一最小资源定理。

若普通方案允许提前 permission fault、改变合法 cache fill 策略、额外 PWC、重读前缀或其它同合同选择，必须纳入对照。不能强制 ordinary 保存所有寄存器，再宣布候选独有优势。

### 强普通对照与下一裁决

O1 为原请求 token＋G/S target/最近返回 PTE＋等级/权限的普通 continuation；O2 在当前不可撤 PTE 退休后保留原请求并 restart；O3 将同总资源用于 PWC/partial translation＋O1/O2。候选若仅做 offset 推导、phase union、删除不再使用字段，O1 必须允许这些同样具体优化。

[US8140781B2](https://patents.google.com/patent/US8140781B2/en) 的 2009 年公开主体包含原 GPA/请求 tracking、返回 PTE 的 next-level base 与 token、下一层重新仲裁；root 已直接读 Fig3/5/6 和相关正文。它不是现成 RISC-V nested 帧的面积数字，但覆盖续走/逐层切换主体。CVA6 Sv39x4 的父/子 stage 表示和其它公开 nested walker 也要逐字段核查。

两席已完成 union 帧、fault 情况和这些强普通构造的手工核查：普通 retained frame上界82bit、partial G restart上界36bit，均另计C/valid/token/active/在途/terminal；未实现、未综合，不是最小编码。只有出现不同的具体表示/决策，在同原请求资源、同允许优化、同错误/失效合同下获得前作与 O1/O2/O3 未达到的资源—重复工作—释放粒度组合，才继续作为独立成本机制。本轮 O1 同字段同保证追平，停止压缩机制推荐；不因论文未写出一个具体位数便推定新颖。

### 已直接核的表示前作与修正

root 已读本地保存的官方 [Rocket 2022 pinned PTW](https://github.com/chipsalliance/rocket-chip/blob/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala)：158–171 为原请求、r_pte/aux_pte、两组等级与 stage；196–214 从页号和原请求索引重建 PTE 地址；524–565 为 G 返回恢复 S，以及 aux_pte 在 S-parent 与 S-leaf 之间复用。这里是明确的同一表示主体，不是“普通 FSM 能表达任何规则”这种泛泛反驳。

root 也直接读当前树 `cva6/core/mmu_sv39x4/cva6_ptw_sv39x4.sv:105–135,389–415` 的祖先 stage/levels/父子指针与 S-leaf 保存。它们并非现成 82bit 暂停槽或面积结果，但候选的 phase 分解与 index 恢复没有越出这些已公开表示。joint phase tag 的少量编码、受支持能力删除和常规 liveness 剪枝不能单独救回机制贡献。root 由此停止当前紧凑表示的独立机制推荐。

## D. root 增补的强普通 partial restart 与持续进度反例

### 比 whole-walk restart 更强的具体点

在当前无 A/D 写、正常非终态、静态 mapping、原请求/context 保留的合同下，普通方法可只丢当前 G-subwalk 的部分进展：

- INTERMED/S-read 前保留 S-parent guest 目标页号29、S 等级、累计 global；G-root 从保留的 iohgatp 恢复。
- FINAL 前保留 S-leaf guest 页号29、w/x/u/global4 与 S 等级；目标 GPA 从 leaf 与原 IOVA 重建。
- 不必保留当前 G-table 物理页号；在已 offer 的 PTE 返回并检查后停止，恢复时重走当前 G-subwalk，然后继续 S，而不是从整个 S-root 开始。

这些数据是一个小于全逐-PTE continuation 的普通中间成本点。owner/valid/token/C、错误/终态存储和实际编码均另计，本轮不将约数作为已验 bit 数。若在当前 G 已读完但 S-PTE 尚未读取的边界暂停，最多重做3笔 G-PTE；更早暂停重做0–2笔。一次暂停不会重做已处理的 S-prefix。此处只给纸面事务数，不给实际 cycle/综合收益。

### 一次快速交接不等于不断交接时仍能前进

有限反例：A、V 都需三层 G-walk；A 完成 G-root 一笔后存 S-target、丢当前 G-pointer、交给 V；V 同样完成 G-root 后交回 A；两次恢复各自重新读 G-root。继续按每笔 PTE 公平轮流，两者都得到无限多次 memory 服务，却都不能到 G-leaf/S-PTE。这是 partial-restart 策略的 logical-progress 反例，不是当前生产平台已有死锁。

因此“一笔不可撤 PTE 的交接尾部”和“所有设备持续进度”必须分开：若不存子walk进度，普通对照至少需要允许某次 G+S 小段不被反复打断（当前冷路径最多3 G＋1 S 读；final最多3 G读），或给到达/抢占次数等外部条件。不能同时免费声称任何请求都只等一笔 PTE、又对任意持续到达保证有限完成。

### 下一分析题，及不立新条件

收窄为：有限续走存储下，在相同到达条件、持续进度和实际不可撤事务合同中，nested 多设备的交接粒度、累计重复页表读取与必要状态之间，是否存在尚未给出的完整成本界及达到它的规则。

基线必须含：逐 PTE retained continuation、整 walk 运行、whole restart、上述 partial G restart、同预算 PWC/partial translation；不能只拿全快照与全重走两极比较。若最优点只是这些已有构造的选择，或者只复述 restart 可能 livelock 的一般事实，就不能将本分析包装为独立新理论。当前没有完整界、没有新最优算法。只有先得到不被这些点覆盖的可达见证/严格条件式，才考虑最小状态模型；不因候选被否决而自动编译下一个阈值。

## E. 本阶段执行范围

已完成只读源码/primary 核查、三席方案与反驳、DeepSeek 接口证据取回。新 DUT build/run、RTL/Linux 改动、BIT/JTAG、板卡/SD 动作均 0；最新完整 copy 实测仍是 §43。本文件不宣称新延迟/面积收益、已证明新定理或论文分区。
