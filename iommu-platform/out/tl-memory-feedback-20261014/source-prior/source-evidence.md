# NormalTL 双顺序候选的源码边界（第一阶段，2026-10-14）

结论：源码允许在合流端**第一次输出 NormalTL VALID 前**选择两个真实 bank 队头，但这个机会需要两头在同一次新 offer 之前已经共同合法。等待单 PTW 空闲之后重新选择已输出 VALID 的队头不合法；把未排空写源的跨 bank READ 当成合法第二头也不合法。只读源码阶段没有得到“所有合法双头均不可能”的封闭证明，也没有得到候选收益或论文创新的验收。

本席仅阅读指定关键调用链和两篇原作者 PDF；没有执行模型、测试、构建、旧验证器或板操作，没有改生产源码。原始源码摘录、全文件 SHA、实际 Git HEAD 保存在 [source-snapshot.json](/home/xvhaocheng/work/iommu-platform/out/tl-memory-feedback-20261014/source-prior/source-snapshot.json)。范围与条件裁决见 [source-bound-verdict.json](/home/xvhaocheng/work/iommu-platform/out/tl-memory-feedback-20261014/source-prior/source-bound-verdict.json)。公共文档更新由 root 统一负责。

## 1. 实际选择点与三种承诺

1. **bank 内选中头和字段。** [rv_iommu_trans_ctl_wrap.sv:625](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:625) 先选择可用 AW，随后选择满足读准入的 AR；字段写入 `trans_req_data_q`，进入 `REQ_TRANS`。:674–682 在边界检查通过时输出 `trans_req_valid_o=1`，直到 ready 才进入 `AWAIT_RESP`。当前 AW/AR 头直到 :721–725 的完成端 AW/AR handshake 才出队。候选只能调整两个已合法 bank 头的合流次序，不能重新解释它们的权限、字段或 bank 所有权。
2. **合流端第一次 VALID 的顺序承诺。** [rv_iommu_banked_ctl.sv:379](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:379) 直接把两个 bank 的请求送入 RR `stream_arbiter`，不存在“PTW 忙时先不出 VALID”的原生条件。原文连接为 `inp_valid_i(bank_trans_req_valid)`、`oup_valid_o(trans_req_valid_o)`、`oup_ready_i(trans_req_ready_i)`。`stream_arbiter` 注释明确 VALID 一旦输出，数据保持到 handshake；实际 [stream_arbiter_flushable.sv:34](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/stream_arbiter_flushable.sv:34) 的 RR 实例使用 `LockIn(1'b1)`。[rr_arb_tree.sv:149](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/rr_arb_tree.sv:149) 原文为 `lock_d = req_o & ~gnt_i; req_d = (lock_q) ? req_q : req_i;`。因此 `VALID=1, READY=0` 已经锁定顺序，不能因为 PTW 后来变空或反馈改变而换头。
3. **spill 接收的承诺。** [rv_iommu_tl_wrap.sv:200](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:200) 的非 bypass 请求 spill 在三路请求 arbiter 之后。其 [spill_register_flushable.sv:78](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/common_cells/src/spill_register_flushable.sv:78) 以输入 handshake 保存数据，:90 `ready_o = !a_full_q || !b_full_q`，:93 `valid_o = a_full_q | b_full_q`，:96 `data_o = b_full_q ? b_data_q : a_data_q`。它是真实两项存储；接收不等于 PTW 已开始，也不能把已接收条目当成可供新排序的自由槽。

已核两套 common_cells：CVA6 的副本与 IOMMU 的副本 SHA 不同，但 RR 路径的 `LockIn=1`、VALID 后锁定及 spill 两项存储相同。priority 路径不同，不能把其中一份库无条件当成当前所有构建的唯一解析源；本席结论只依赖两份共同的 RR/存储行为。全部对应 SHA 在原始快照。

## 2. 两个合法新头的可达条件

[rv_iommu_banked_ctl.sv:243](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:243) 独立以 AW/AR 原始 AXI ID 的 `BankIdBit` 选 bank。只有一个上游 AR 通道和一个 AW 通道，因此不能在同一沿接收两个新 AR。

写源准入是关键限制。:259–268 中 `restrict_reads=(w_unfilled_q>0)||tr_axi_req_i.aw_valid`；正余额只允许当前写源 owner 的 READ，零余额但新 AW 可见时阻止新 READ 选择；同沿被接受的 AW 还会挡对应 bank 的旧 READ 抢先。bank 内 :646–647 才决定 READ 进入 `REQ_TRANS`，不是以输入 AR 已接收替代该准入。

一个**源码推导、尚未执行**的合法新双头前缀如下：

| 时刻 | 初始条件及事件 | 源码后果 |
|---|---|---|
| t0 前 | 两 bank 空闲；各 AW/AR/W FIFO 可接收；无未完成源 W；合流无已 held VALID；边界检查将通过 | 单 PTW/缓存/原有容量不变 |
| t0 沿 | 同时接收 bank0 的 AR、bank1 的 AW，以及 bank1 的单 beat `WLAST`；AW 随 handshake 后撤销 | AW/AR 是独立通道；`w_unfilled_q += aw_accept - w_last_accept` 净变化 0 |
| t1 周期／沿 | 两 FIFO 的头已可见；没有新 AW，写源余额为 0 | bank0 READ 准入恢复，bank1 选择 WRITE；双方进入 `REQ_TRANS` |
| t2 周期 | 两个 `check_valid` 均真，第一次同时输出 bank-local 请求 VALID，合流第一次 offer | 在这次合流首次 VALID 之前可以决定先后；此后锁定 |

单 beat AW/WLAST 同沿接收并非假想额外接口：[axi_demux_simple.sv:179](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv:179) 在合法新 AW 上立即设 `w_cnt_up=1`，:252–253 的 `w_select_valid=w_cnt_up|(|w_open)` 当周期即可选择 W；:432–435 转发 W 并按 WLAST handshake 关闭源 ownership。bank W FIFO :222–239 可以先保存 W；它的下游 drain :220 仍等完成端 AW。bank 配置 :163–164 只拆原有容量，没有增加容量。

这个前缀要求 **AW 和 WLAST 确实都被接收**。仅同时接收 AR0/AW1，随后仍有 bank1 写源欠账时，bank0 READ 不是可排的合法第二头。FullBuffer 的现有准入例外（bank 内 :646–647、bank wrapper :319/:343）是另一条有条件路径，不能默认所有 READ 都可利用。

上述前缀只证明 RTL 接口协议存在一个可达路径；没有证明现有 iDMA 工作流会生成这个前缀，也没有证明一次实验开始处可以凭空把两个头置为 ready。四条件模型前必须给共同 ready 前缀，或接受 root 指定的等效可达初态证明；identity 交换时也要重新满足同一准入规则。

## 3. NativeN 旁路必须保留

[rv_iommu_tl_wrap.sv:284](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:284) 的 `normal_gate` 在 NonblockEnable 情况下限制慢路径进入条件；它没有阻止 bank RR 先 offer。:303–306 的 `n_window` 在一个 S1-only PTW 活跃且另一 bank 的 head 可见时允许非阻塞 hit 探测。:318 直接把**当前合流 candidate VALID**交给 hit 单元。:286 在 intercept 时通过 `n_accept` handshake。

[rv_iommu_nonblock_hit.sv:47](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:47) 需要 `candidate_valid_i` 启动，:100–103 在真实 DC/IOTLB 命中、配置不变、candidate 不变等条件下接受，:104–107 只有接受才 touch cache，:150/:172 拒绝探测时不 handshake，等慢路径接受或 candidate 合法撤销。

如果新增 gate 把所有 NormalTL VALID 推迟到 PTW 空闲，就会隐藏 native hit 旁路候选；那是移除了已有能力的不同控制，不能充当原生 NativeN 对照。singleton、已 held offer、已 spill 接收的条目应照原路径推进。

## 4. PTW 与共享 DDR 路径

[rv_iommu_tl_wrap.sv:759](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:759) 只实例化一个普通 `rv_iommu_ptw`；MSI 等结构访问不能混成第二个普通 PTW 额度。

[rv_iommu_ptw.sv:275](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:275) 每个 PTE 使用 ID0、`len=0`、`size=3'b011` 的 8-byte 单 beat AR。:384–389 在 `MEM_ACCESS` 维持 AR VALID，handshake 后转 `PROC_PTE`；:395–397 收到 R VALID 才处理 PTE；:510/:530/:563 由**刚收到的 PTE PPN**构造下一层地址。:480–504 区分最终更新、implicit 完成及权限/页错。因此未返回的下一层 PTE/最终 PA、实际未来 walk 长度和未来 fault 不是选择点可见事实。

[rv_iommu_ds_if.sv:81](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv:81) 把 PTW、DDTW、PDTW、CQ 等 7 类 AR 合流，经 :95–108 的非 bypass spill 到 DS 端。:239 以返回 ID0 路由 PTW，:249–262 的返回 spill 又隔一层。PTW 端 AR 接收与外部 DS AR 接收是两个时间点，不能当成同一事件。

[rv_iommu_top.sv:337](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv:337) 把 PTW 接 DS mux；[ariane_peripherals.sv:753](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_peripherals.sv:753)／:761 分别把 DS／completion AXI 接上外围实例端口；[ariane_xilinx.sv:850](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/ariane_xilinx.sv:850)／:851 接系统 xbar 的两个 source 口。:288 DRAM 地址规则、:307–319 共用 xbar，:294–298 原配置每侧 8 事务、CUT_ALL_PORTS、一层 pipeline。:944–945 的 atomics 把 `master[DRAM]` 接 `dram`；:1006–1089 是跨时钟转换；:1110–1178 的 KINTEX7 MIG 连接仅提供 AXI 端口。其他 board 宏使用另外分支，不能据 KINTEX7 分支断言当前板类型。

这证明 PTW 与被允许的数据请求最后汇合共享 DDR 服务路径；不能由共线路径直接推出一个顺序相关服务函数、确定干扰强度或任何本轮性能数字。:165–166 的 DS delay 常量也只是此源码默认参数，不是当前已生成 profile 的数值证据。

## 5. 能观察什么，不能借什么

| 层次 | 可合法观测的事实 | 必须计入的限制 |
|---|---|---|
| bank 选择端 | 两 bank header 的 IOVA/DID/PID/ttype/bank、valid/ready；本地实际 AW/AR FIFO 头 | [top:135–145](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv:135) 的合流 header **没有** AXI ID、len、size、剩余 W、完成债务；获取这些需真实额外接线/状态，不能称零成本现成输入 |
| PTW/DS 端 | PTE AR VALID/READY，返回 R VALID/READY/RESP，已返回 PTE 内容；过去服务时间/请求数 | walker 与 DS 之间还有 arbiter/spill；排队、时钟转换、consumer backpressure 混在端到端时延里；不能直接命名为纯 DDR 服务时间 |
| completion 端 | 已允许 AR/AW 的地址/长度/ID及握手，实际 W beat/WLAST drain，R-LAST/B 返回及停顿 | bank 自身在完成端 AW/AR handshake 后进入 IDLE（local :721–725），这**早于**全 R/B 退休；bank tag 的至多一笔待翻译不等于全系统至多两笔 owned 债务 |
| 系统 DRAM AXI 端 | xbar 后当前 AXI 事件与有 ID 的返回；可由有界计数器记录真实历史 | 此观察未原生接入 IOMMU 排序器，需新增跨层连线/存储成本；CPU/debug 流量是否纳入必须明确 |
| MIG 内部 | 本轮排序器没有开放的可读状态接口 | 不能读取 open row、银行忙闲、内部 command/read/write queue occupancy、内部 reordering、未来 refresh/服务进展作为 oracle |

MIG 的 `app_*` 状态还在 :1128–1134 留空或请求绑零；源码没有把内部服务队列连接到 NormalTL 选择端。允许的是**过去可观测 AXI 反馈形成的估计**，不是精确知道未来服务轨迹。过往 PTE 地址/PPN 也要遵守上下文、无效化及配置变化边界；历史存在不构成将未完成 walk 的未来返回偷看出来的授权。

bank wrapper 注释 :38–41 与实际 `e3_o` 赋值不支持旧单 owner observer 的非 quiet 计数。完整退休应核真实上游 AW/AR 接收、W 全部接收/排空、R-LAST/B、fault、quiet；:523–553 的真实事件可以作为计数依据，不能用旧单 owner E3 数字代替。

## 6. 当前私有 DDR 驱动不支持统一 R/W turn 假设

经 root 新指令定向补核 [nonblock_actual_dma_test.cpp:650](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:650) 的现有 `drive_ddr()`，本席没有执行它。:653–654 原文先设 `ddr_aw_ready=1; ddr_w_ready=1;`；:655–666 只在明确设置的 255-beat completion WRITE 的特定尾部位置可人为拉低 WREADY。:668 ARREADY 取决于 `mem_q.size()<16`；:669–677 独立从 `cb_q` 输出 B。

R/PTE 的共享是实质存在的：:679–719 用唯一 `mem_q` 头产生一个 R beat；可选 :692–697 DS 优先只合法提升最老 DS 请求，且不得替换已 held R；可选 :701–713 规定已有 read-latency 的准备是否从各请求接收时刻重叠。[该文件:1301](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:1301) 原文把每个已接收 DDR AR 统一 `mem_q.push_back(r)`，包含 completion READ、PTE 等 DS READ。

W 是独立驱动：:1331–1333 AW 放入 `caw_q`，:1335–1353 在真实 W handshake 上直接更新 memory，WLAST 后把 ID 送 `cb_q`。这段不检查当前 R/PTE 是否在服务，也没有 R/W turn 状态、共同排他 server、读写方向切换 `tau`、或者方向切换期间双方均不得服务的条件。

因此，**若本轮初始实例用“统一 R/W 服务 turn + 每次切换 tau”生成两顺序差额，该实例在当前来源门不准入**。共享一个 AXI DDR 端口与 R/PTE 共队没有支持这个额外服务规则。真实 MIG 物理读写切换可能存在，并不能由这份行为驱动确认其具体规则、可见输入或数值；本席不把隐藏 MIG 状态补成 oracle，也不以此否定所有共享 memory 联合反馈方向。这个来源缺口足以停止该初始 turn-feedback 实例进入四条件模型，不需要运行新场景来“验证”自行添加的 tau。

## 7. 名义板源码、私有已验模块和旧闭合范围

`ariane_soc_pkg.sv:110–115` 默认 8 outstanding、32 W beats、16 IOTLB；bank wrapper 按 :163–164 拆为每 bank 4 outstanding、16 W beats。**名义 FPGA 源实例本身没有证明 bank 已启用**：`rv_iommu_top.sv:28–29` BankedEnable/NonblockEnable 默认 0，`ariane_peripherals.sv:767–801` 没有覆盖；`ariane_soc_pkg.sv:123` 默认 MSI_BT_MRIF 又被 bank wrapper :143–145 拒绝。这是 nominal board 文件的边界，不是 banked 机制的关闭理由。

当前私有 actual-DMA 源接口与 nominal board 不同：[nonblock_actual_dma_top.sv:788](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_top.sv:788) 明确传 `BankedEnable=1`、`NonblockEnable=NonblockEnable`、`BankIdBit=SameBankDmasEnable?3:2`。[nonblock_banked_top.sv:366](/home/xvhaocheng/work/iommu-platform/tests/nonblock_banked_top.sv:366) 构造 cfg，:370 是16 IOTLB，:374–378 禁 PC/BC/debug/ATS、MSI_BT_ONLY；:647–651 接真实 bank controller（未开 TEST_COMPLETION_GATE 时），:795–798 接真实 TL 的 NonblockEnable。outer [system_top.sv:71](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv:71) 默认 NativeN=1、:76–77 总8 outstanding/32 W beats并传给 inner，:95–96 DS delay50/50、:468–469 接真实 delayer。joint standalone 的总4/8默认被 actual-DMA outer 参数覆盖，不能误当所有实际回归的固定容量。

这些源码确认私有已验资产的 banked/NativeN 接口真实存在，但不核验某个旧运行或板 bit 的实际覆盖值。本轮实际运行配置须用 root 已有固定 manifest 引用；本席没有展开旧 out、运行旧 builder、重跑旧 validator 或验证当前 bit。

[HANDOFF.md:757](/home/xvhaocheng/work/HANDOFF.md:757) 的 §79B 明确限定旧 B：同两结果槽、固定 W 序、序无关译址成本，具体例同轨迹，因此旧动作版本不进 RTL；它同时明确“真实共享 DDR 改变 walk 和 W drain 的联合反馈”是可重开问题。新的共享 memory 内核只有在保持真实所有权/原有容量的情况下，实际给出**顺序改变以后，由已合法数据服务影响后续 PTW/数据的因果反馈**，才超出这个旧证明的前提。仅把常数 walk latency 换一个名称或记录由 oracle 预先分配的 AB/BA cost，没有超出旧证明。

## 8. 四条件模型前必要门槛

- 固定实际 banked/NativeN profile；给 fresh 双头共同 ready 的合法前缀，或明确 root 接受的等价可达初态。已 held/spill-accepted 的顺序保持。
- 把所有输入分成“选择时现有字段”“已完成反馈”“额外观察状态”；给完整接线/寄存器/运算/初始化及等待成本，不输入未知 PTE/PA/MIG状态。
- 保留单 PTW whole-walk 非抢占、16 IOTLB、现有 AW/AR/W/返回容量、写源准入和 NativeN；一旁路公平规则不绕过首次 VALID 锁。
- 服务内核须对 AB/BA 使用同一个冻结下游规则和同一合法 R/W 时序；次序只能通过真实先发生的事件改变后继服务，不能分别塞入两份预定 latency oracle。
- 在进入任何四条件执行前，root 先接受相对预冻结普通控制的**具体选择差异**；相同初态、观测预算、完整成本，比较双方完整完成而非只比翻译结束或最后被“优先”的一头。

目前这里只提供上述来源门的证据与条件，不代替 root 对机制、控制、成本和模型入口的裁决。
