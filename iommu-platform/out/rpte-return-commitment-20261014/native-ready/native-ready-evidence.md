# 原生 RREADY／R→W 排空证据（2026-10-14）

结论：真实一维 iDMA 的正常非零 copy 中，接收 R 的上层响应端恒可接收，直接阻塞条件是当前被选中字节 FIFO 没有空位。字节 FIFO 只在本源 W 数据获得握手时排空。生产四 DMA 的 bank 许可不等于源 W 许可，因而存在一个**接口上合法的同 bank 异源反压条件**；但它是否能由本轮冻结的 DEFAULT iDMA 共同 fresh 初态到达，及 AB／BA 是否改变这个到达过程，本席没有完成证明，值为 unknown。不能据接口前缀宣称实际发生。

默认 private 双 DMA、每源 512 拍、单个对齐 4 KiB 正常作业的自身返回容量足够，不能把此实例减小缓冲或换成生产小源实例制造正例。这是容量排除证明，不是“所有瞬时 ready、metadata 或 PTE 等待均不存在”的证明。

全量原文段、SHA-256、当前仓库 HEAD 与选定旧段的相同哈希入口：[source-snapshot.json](source-snapshot.json)。本席仅作新定向源码核查及数学推导；0 新模型、0 仿真、0 构建、0 旧 validator／实验重跑、0 生产修改。未经 root 新 gate 准入，不进入实现或实验。

## 1. 实例和身份必须分开

| 实例 | 实际源和 ID | 原生 payload buffer | 有效限定 |
|---|---|---|---|
| FPGA 非零 IOMMUNonblockMode 生产配置 | 四源 i=0,1,2,3；各私有 AxID=0，两位源编号拼到高位；进入 IOMMU 的 ID 为 0,4,8,12 | 512,512,3,3 拍，每拍 8 字节 | bank 由 ID[2] 决定，bank0={0,2}、bank1={1,3} |
| `nonblock_actual_dma_top` 默认 private 实例 | 两个 live iDMA，AxID=0；其余两个 mux 输入为零，ID 为 0,4 | 两者均 DmaBufferDepth=512 | 两源时 source index 恰巧等于 bank，不代表生产四源 |

生产根源：`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:870-872` 定义 NumDMADevices=4 与 DmaAxiIdWidth=4−log2(4)=2；`:1228-1253` 为四源 mux；`:1269-1281` 固定 buffer、inflight=8、RAWCouplingAvail=0、IsTwoD=0、DID=10+i、AxID=0；`:1404-1409` 令 BankIdBit=DmaAxiIdWidth。`cva6/corev_apu/tb/ariane_soc_pkg.sv:31` 的 IdWidth=4。

private 根源：`tests/nonblock_actual_dma_top.sv:48` 默认 DmaBufferDepth=512，`:352-433` 仅两个真实 wrapper，`:544-548` 将 source2/3 置零，`:680-705` 保留四端口 mux 的真实 ID 结构。`tests/nonblock_actual_dma_system_top.sv:79,1102-1107` 继续传入同一个参数，而不是生产数组。

生产 completion 经过系统 xbar 后，DDR 的完整 6 位 ID 应为 0x20、0x24、0x28、0x2c。source=(id>>2)&3；bank=(id>>2)&1；DDR 发起端口=(id>>4)&3。不能只保留 bank。`tests/nonblock_actual_dma_test.cpp:1296-1299,1310-1312,1364-1366,1382-1385` 使用 `(id>>2)&1` 为 private 两源计数；该计数器不能原样当四源结果所有权证明。

## 2. 真实返回链与 RREADY

逆向 ready 链为：共享 DDR-facing xbar → completion 系统端口 → bank 输出 mux → bank 内 completion/error demux → bank TR 端口 → bank 输入 demux → 全局 DMA mux 的完整 source ID → 该源 iDMA transport → 八个字节 FIFO。

1. `rv_iommu_banked_ctl.sv:495,503-520` 传 ready，按原 ID[BankIdBit] 恢复冗余 bank 高位，原 ID 的 source 位保持不变。输出 mux 的 SpillR=0（`:433-440`）。
2. `rv_iommu_trans_ctl_wrap.sv:361-368` 将 RREADY/BREADY 送往 completion/error 选择器，R/B 响应再回 TR；正常 copy 走 successful completion，错误走 error slave（`:527-568`）。SpillR=1，不能忽略其真实返回槽。
3. 两个 axi_mux 副本的多源分支均按完整 source 前缀分发 R。private 的 IOMMU vendor `axi_mux.sv:448-462` 选择 `mst_r_chan.id[SlvAxiIDWidth+:MstIdxBits]`，ready 取该源；生产副本同义代码在 `cva6/vendor/pulp-platform/axi/src/axi_mux.sv:372-386`。两副本未当作同一文件；private 选择由 `tests/test_nonblock_actual_dma.py:95-123` 确认。其差异在退化单端口分支，当前两／四源路径均非退化。
4. `cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:254-257`：

   `in_ready = &(buffer_in_ready | ~mask_in)`；`r_ready = in_ready & r_dp_ready_i`。

   `:265-270` 仅在 RVALID、in_ready 和响应 ready 同时成立时写入字节 FIFO，末拍握手才能释放该 burst 的 read metadata。对齐正常 copy 的 shift=0、各 beat mask=8'hff（`:187-235`），不能把 RVALID 当成已消费的数据。
5. `dma_core_wrap.sv:206-207,316-319` 的当前一维 wrapper 使用恒 1 idma_rsp_ready、RejectZeroTransfers=1、NO_ERROR_HANDLING；`idma_backend.sv:519-532,684-689` 将正常非零作业的 r_dp_rsp_ready 直接连 rsp_ready。因此本范围不存在另一个非恒定的“上层完成响应消费者不 ready”原因。零长度拒绝分支、TwoD 或更换 ErrorCap 不属于本次正常 copy 条件，不据它們制造见证。
6. `idma_buffer.sv:33-50` 的 BufferDepth 是八个 byte FIFO 各自的深度，不是额外八倍 beat 容量。`idma_stream_fifo.sv:104-123` 选 stream_fifo，512×8 的 byte 队列使用已有 FPGA RAM。生产 `common_cells/src/stream_fifo.sv:43-46` ready=`~full`，`fifo_v3.sv:65` full=`status_cnt_q==DEPTH`。

RREADY 可以因任一被使用字节 lane 满而为 0；对齐 copy 各 lane 同进同出，因此条件简化为该源 beat FIFO 满。该公式不依赖未来 PA、未来 PTE 或未来 RREADY oracle。

## 3. 真实写出链：W source、bank、翻译成功不是同一许可

最上游全局 DMA mux 的 W FIFO 存的是 source index。private `axi_mux.sv:283-313,317-365`，生产副本 `axi_mux.sv:207-237,241-289` 显示：AW 首次仲裁／offer 即推 source 决策；AW stall 时锁住该决策。只有 WVALID&&WREADY&&WLAST 才 pop；B 不能提前释放此 source W 头。没有 WID，所以后源不能越过前源 WLAST。

bank 输入 demux 再按 bank 固定 W 选择直到 LAST。`rv_iommu_banked_ctl.sv:243-268` 的 w_owner_q 只有 1 位 bank；`restrict_reads=(w_unfilled_q>0)||AWVALID`，read_admit 也只有两位 bank 许可。输入 demux 配置 Spill*=0、MaxTrans=2×NumOutstandingTrans、full-ID lookup（`:280-309`），不会把 source2 改为 source0，但 **source2 和 source0 同 bank 的 read selection 会获得同一个 bank 许可**。新 AW 同拍进入该 bank 时还会挡住 read selection 一拍，不可忽略。

bank 内部的原生 W FIFO 深度是总配置 WFifoDepth/2；当前总配置32对应每bank16，NumOutstandingTrans总8对应每bank4（`rv_iommu_banked_ctl.sv:158-164`）。`rv_iommu_trans_ctl_wrap.sv:178-179,220-240,359-360` 显示它可以先接收属于当前全局源 W 头的数据；向 completion 转发需要 w_cnt_q≠0，而 w_cnt_q 来自该 bank 的 completion AW 接收，按 completion WLAST 减计。因此至少存在三层不同许可：

- 全局源 mux 已轮到本 source；否则该源在进入 bank W FIFO之前就 WREADY=0。
- 输入 bank demux 固定在对应 bank，bank W FIFO有空位。
- completion AW 已使 w_cnt>0，completion W 当前可握手。

`idma_axi_transport_layer.sv:343-360,390-438` 的 `write_happening=ready_to_write & axi_rsp_i.w_ready` 才产生 `buffer_out_ready`。仅有“paired WRITE 翻译已成功”不能绕过全局前源 W 头，B 已退休也不等于任意后源现在取得 W 权限。

bank IDLE 优先 AW（`trans_ctl_wrap.sv:620-647`），完成地址请求要经过 REQ_TRANS、AWAIT_RESP、COMPLETE，直到 completion AW/AR ready 才 pop 对应 AX FIFO（`:674-725`）。这是现有 WRITE 准备动作；不能把它隐藏成候选独占能力。

## 4. 当前 DEFAULT 的边界，不能从合法接口前缀跳到真实初态

private 普通单作业与 batch 都把 REG_CONF 写 0（`actual_dma_test.cpp:1499-1505,2023-2032`）：ttype=DEFAULT、decouple_rw=0。frontend 的 `opt.beo.decouple_aw=0`（`idma_reg64_frontend.sv:366-373`）；RAWCouplingAvail=0，所以 backend 走 AW fall-through bypass（`idma_backend.sv:837-857`），不能悄悄启用 R-first coupler。

coupled legalizer 在两路 datapath／meta storage 都可接收时成对推进 R/W（`idma_legalizer.sv:459-469`、`idma_backend.sv:471-473`）。AW 和 AR 有不同的全局 arbiter 与保持状态，但“它们独立”本身不是本轮 DEFAULT 前缀能产生任意次序的证明。尤其不能把任意先前 READ-only／WRITE-only 历史、手工设置 FIFO、故意推迟 source0 AR 或缩减 buffer 带入共同初态。

以下只是一条**条件接口前缀**，不是已证明实际 iDMA 可达或运行过的 AB／BA：

| 条件事件 | 合法性／结果 | 本席证据类型 |
|---|---|---|
| source0 为全局 W FIFO 头且 WLAST 尚未发生；source2 是同 bank0 后源 | 全局 WREADY[2]=0；bank read_admit[0] 可为 1 | 当前源码逻辑 |
| source2 的正常 AR 已经先进入 bank AR FIFO并成功完成翻译／DDR AR握手，source0 的必要 source 返回尚不可供给 | read_admit 没有 source 检查；同 bank 不会凭 full-ID 自动把不同 source 的 AR 重排成 W 顺序 | 接口条件成立时合法；该前缀从冻结 DEFAULT 初态的实际可达性 unknown |
| source2 正常 4 KiB copy 的第一 256-beat burst 先返回，而它无法 W 排空 | iDMA 3 beat capacity 加有限原生 R 槽最终不足以消费完整 burst | 下节容量推导；不是测得周期 |
| 下一 PTE 在 RVALID&&!RREADY 之后才入队 | 内存模型不能撤回当前 R；DSpriority 的 `!ddr_r_held` 禁止换头，PTE 等待 held R 解除 | 该 PTE 生成／准入时序由 root 独立任务判定，本席不代填 true |

若 source0 尚缺 WRITE 准备，根因属于现有 WRITE preparation。若 source2 在前 source 的 W 所有权后等待，根因是已有 source W ownership／完整 return envelope，而不是新增的“第三种源依赖”。本席未发现正常一维 copy 中 r_dp_ready_i 的其他阻塞原因。

## 5. 原生返回槽与容量推导

在 RTL xbar 的 DDR-facing 正常 read 边界到 iDMA 之间，不含 board opaque MIG／clock converter／data width converter，也不含可选 atomics wrapper：

| 已存在的 R 存储 | native 槽数 | 原始代码 |
|---|---:|---|
| xbar DDR 输出 mux R spill，CUT_ALL_PORTS | 2 | `axi_xbar.sv:131-154`、`axi_mux.sv:375-386` |
| completion↔DDR cross 的 PipelineStages=1 multicut | 2 | `axi_xbar_unmuxed.sv:231-251`、`axi_multicut.sv:47-79`、`axi_cut.sv:100-111` |
| xbar completion 输入 demux R spill，CUT_ALL_PORTS | 2 | `axi_xbar_unmuxed.sv:181-199`、`axi_demux.sv:175-186` |
| bank 内 successful completion/error demux R spill | 2 | `trans_ctl_wrap.sv:551-557`、IOMMU `axi_demux.sv:175-186` |
| bank 输出 mux／bank 输入 demux／DMA mux R spill | 0 | `banked_ctl.sv:294-298,433-440`；DMA mux默认 SpillR=0 |
| 合计 RTL 中间槽 | **8** | 两槽 spill 的 A/B寄存器与 ready 式：生产 `spill_register_flushable.sv:37-95`；IOMMU副本同义段 |

production xbar 配置见此前已验且本轮哈希相同的 `ariane_xilinx.sv:291-305`；private system 配置见 `nonblock_actual_dma_system_top.sv:885-899`。private DdrAtomics 默认 0（`:72`），走 `gen_ddr_bypass`（`:1037-1039`）。**8 是这个明确 RTL 边界的总量，不是整个板端总返回容量；opaque IP 容量和服务次序均为 unknown。**

对齐源2，在完全不能获 W 的条件下，3 个 buffer beat + 8 个空中间槽最多容纳 11 个本源 R beat。256-beat 原生 AXI burst 的12及以后 beat 不能全数握手，必须在排空前产生某处反压；在继续供给同一源且全部11槽已被占用后，DDR-facing RREADY必为低。首个低的时钟或拍号没有执行测量，不能把“DMA第4拍需空位”写成“DDR第4拍 held”。若已有其他 R 占槽，只能减少本源可用槽，不允许把它们当新增容量。

对齐 solejob 4 KiB、src0/1 的512深度容量证明：每字节 lane 最多接收512字节；第512个 beat 在原 occupancy≤511时可接收，作业没有第513个 payload beat。W排空只会增加空位。没有前作业残留、无配置更改、已保留该完整作业返回权时，不会因为该作业的 iDMA payload容量不足而持有未消费 R。frontend COUNT接受后 WAIT，正常copy直到最后 B响应才发下一个作业（`idma_reg64_frontend.sv:519-559`；backend `:523-532,631-649`）；certificate只许可一个对齐4KiB正常copy并在重叠／非法退休时撤权（`idma_fullbuffer_certificate.sv:16-45`）。

此证明不声称瞬时传播 ready 永远为1，不替 PTE ready／metadata资源作全局证明，不排除其他源／其他合法任务先占据共享 R 槽，也不扩到大于4KiB、多作业重叠、未对齐流或 source2/3。

## 6. 首次 VALID、读队列和完整退休

memory源见 `actual_dma_test.cpp:647-720`：DDR AW/W 默认恒 ready，write_tail_pause 默认0（`:422`）；AR队列最多16。首次可返回时当前 mem_q.front 生成 RVALID。DSpriority 只能在 `!ddr_r_held` 时旋转最老 DS请求，原完整 ID、latency、beat state 都保留（`:681-719`）。`:829-834,1290` 记录 RVALID&&!RREADY；`:1371-1399` 只在 R handshake 后增beat／pop；RVALID本身不释放 queue。

该 memory只实例化上述private 两源，所以共享queue源机制可复用为类型／协议边界，但其运行记录不能冒充生产四源已发生。full-ID/same-ID约束、上一拍heldR与firstVALID承诺仍须在 root 的共同初态事件表中保留。

R/B退休不混用：`banked_ctl.sv:549-586` 的 AR到upstream RLAST握手，AW及WLAST到upstream B握手；Wpartial也保持inflight。bank内w_cnt在completion AW握手增、completion WLAST握手减（`trans_ctl_wrap.sv:178-179`）。iDMA的r metadata到R末拍握手退休（transport`:268-270`）；W metadata到W末拍握手，last/super_last metadata到B握手，正常作业response取last-burst B（backend`:523-526,631-649`）。memory本身AW进入caw_q，W依其队头归属写入，WLAST才产生cb_q，B握手才pop（cpp`:1331-1370`）。translation成功或最后payload R不会替代本源W/B退休。

## 7. 本席交回 root 的门结论

1. **已验类型／逻辑**：真实正常一维copy的 R容量与 W排空依赖；四源full-ID、bank≠source；原生RTL8个R中间槽；不同所有权层和完整 R/W/B退休；DEFAULT成对生成R/W。
2. **已验数学范围**：当前对齐sole4KiB×512实例自身payload容量不会制造持久heldR；小源3+RTL8容量条件若无法排空，256 beat返回不能全收。
3. **unknown**：小源条件前缀是否能从本轮实际DEFAULT共同fresh初态产生，source2 AR是否真的先进入bank FIFO；下一PTE准入时序、AB/BA关系、普通完整selector是否等效、首held周期、board/MIG现象。它们没有被负面结论替代。
4. **不据此准入新实验**：接口条件依赖的现有source Wownership和return-envelope已属必须完整计入的普通边界；root须比较同fresh点的完整普通selector。没有共同native前缀与普通差异，不进入新模型/RTL。

产物和变更范围：只新写 `out/rpte-return-commitment-20261014/native-ready/` 下的本报告、source snapshot和verdict；未改公共文档、生产源码、旧资产。所有数值运行结果（周期、收益、Fmax、面积）均 null／未执行。
