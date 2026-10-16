# 已生成 MIG 4.2：读服务与外部控制边界

裁决：在本次只读核查的已生成 MIG AXI shim 中，外部 current-VALID DS AR-first 和提高 PTE 的 ARQOS 都不能实现私有模型的“把已经接受的读请求移到内部读队首”。AR-first 只能改变尚未提交给该接口的请求顺序；ARQOS 在所选 RD_PRI_REG 分支中影响的是读、写两个通道的切换，不是多个读请求之间的排序。已经接受的 payload 读会在 shim 内按序展开和按序返回；首次 RVALID 已挂起后，外部没有替换这拍或改选另一个 ID 的接口。

这是一项 IP 接口能力裁决。它不证明当前正常工作流中自然出现共同双头，不证明 payload 已造成 PTE 的实际额外等待，也不接受完整任务收益、创新或费用等价。实际生产源码、生成实例、构建记录和当前板加载的绑定由 root 独立裁决。

## 证据范围和导航

生成文件根目录为 `/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/`。以下均为本席直接读过的生成源码，哈希和读取范围在 [source-snapshot.json](/home/xvhaocheng/work/iommu-platform/out/ddr-service-boundary-20261014/ip-semantics/source-snapshot.json)。没有修改这些文件，也没有 build、仿真、旧 validator、trace 重解析或板测。

| 标识 | 源码入口 | 决定范围 |
|---|---|---|
| S01 | [axi_mc.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc.v:250) | 250–301：流水、upsizer、ID 与 burst 分支；969–1075：AR/R/仲裁实际连线 |
| S02 | [axi_mc_ar_channel.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_ar_channel.v:138) | 138–178：单个 active AR；180–235：展开与按命令入返回元信息 |
| S03 | [axi_mc_cmd_fsm.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_cmd_fsm.v:107) | 107–118：成功 native command 与重新 ARREADY 的条件 |
| S04 | [axi_mc_incr_cmd.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_incr_cmd.v:134) | 134–199：地址、命令计数、next_pending；213–228：头尾处理 |
| S05 | [axi_mc_cmd_arbiter.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_cmd_arbiter.v:125) | 125–131：只选读/写及 hi_pri 固定值；159–215：所选 RD_PRI_REG |
| S06 | [axi_mc_r_channel.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_r_channel.v:123) | 123–132：容量单位；189–275：数据和元信息 FIFO；280–326：元信息两级 skid |
| S07 | [axi_mc_fifo.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_fifo.v:100) | 100–153：顺序移位 FIFO，无 ID/QoS 选择 |
| S08 | [ddr_a_upsizer.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_ddr_a_upsizer.v:811) | 811–860：QoS 原样与 ID 控制；938–1010：解包计划 FIFO；1061–1111：单个顺序输出寄存 |
| S09 | [ddr_axi_upsizer.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_ddr_axi_upsizer.v:750) | 750–815：read_addr_inst 参数和完整连线 |
| S10 | [ddr_r_upsizer.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_ddr_r_upsizer.v:395) | 395–442：解包和握手推进；844–846：RID 来源；885–933：held R 输出 |
| S11 | [axi_ctrl_top.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_ctrl_top.v:70) 与 [axi_ctrl_read.v](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_ctrl_read.v:88) | ECC AXI-Lite 控制/状态块；不能与 payload DDR read path 混用 |

## AR 不是可按 QoS 随机取出的读请求队列

**源码事实。** S01:265–292 说明是否启用 upsizer 由 AXI 与 MC 数据宽度及 narrow 参数决定；启用时 AR 输出寄存配置为 1。P_SINGLE_THREAD 固定为 0，含义是已知下级返回有序，因此 upsizer 不因不同 ID 阻塞新请求，不是允许重新选择不同 ID 的请求。S09:750–815 把这个参数传入读地址转换模块。

**源码事实。** S08:199–200、989–1010 的深度 32 cmd_queue 储存的是数据解包计划：是否 FIX/修改/WRAP、字位置、offset、mask、step、length。其载荷没有 ARADDR、ARID 或 ARQOS。这不是一个有 32 个可任意调度槽的 AR request queue。S08:1061–1111 只有一个按 READY 更新的 AR 输出寄存。S02:138–178 再保存一个 active AR 的 ID、地址、长度、QoS、burst 与 valid；当这个槽未准备接新请求时，内部输入选择已保存的 active 字段，外部新 ARQOS 不会改写旧请求。

**推论。** 即使存在多项已完成 command 下发、尚未返回的读，它们也不是外部可寻址、可取消、可重新分配队首的 request pool。要准确计算本实例的 AR 接收容量，必须按 root 绑定的实际 MC 宽度和实例参数展开；不能把上述 32 项解包计划、30 项 native-command 元信息或控制器 bank machine 数量直接当成 32/30 项 AXI burst 容量。

## 一个 AXI burst 如何变成 native command

**源码事实。** S03:107–118 用 `axvalid & data_rdy` 形成 command valid；仅 command 不满且 valid 时产生 next。只有这一 command 同时是 active burst 最后一项（next_pending 为 0）才形成 cmd_en_last，随后重新允许接下一 AR。S02:180–225 把相同 active AR 交给 translator 和此状态逻辑。S04:156–199 的地址、长度计数只随 next 递进，直到该 burst 的 native command 全部下发。

**源码事实。** S01:295–301 在 4:1 模式令 C_MC_BURST_LEN=1；2:1 且 DRAM BL8 时值为 2。S04:169–191 在值 1 时逐 UI beat 展开；值 2 时按两个 UI beat 形成 command，并处理非对齐头尾。物理地址进一步由 [translator:140–187](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/xilinx/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3.gen/sources_1/ip/xlnx_mig_7_ddr3/xlnx_mig_7_ddr3/user_design/rtl/axi/mig_7series_v4_2_axi_mc_cmd_translator.v:140) 转换。

**推论。** 后到 PTE read 不能利用“长 burst 被分成短 native command”这一点，插入同一 active payload burst 的中间。读写通道可以在其仲裁规则允许时切换，但这里没有第二个 active read 供选择。本节 burst 是到 MIG 的一个 AXI transaction，不能推成整个 DMA 任务、整段 4 KiB 或所有 payload 必须完成后才接 PTE。

## RD_PRI_REG 与 QoS 的准确对象

**源码事实。** S05:90–111、125–131 的 arbiter 只有一条 translated read command 输入和一条 translated write command 输入，rnw 选择其中一条。mc_app_hi_pri 固定为 0。S01:1063–1067 接进去的是 S02 当前 active/head read 的 arvalid_int/arqos_int，不是任意外部新到 PTE 的属性。

**源码事实。** S05:202–203 对 QoS 仅做按位归约：非零的 write QoS 在 read burst 最后一项成功下发时可以释放 read hold；非零的 read QoS 对 write hold 对称。源码没有 read vs read 的 QoS comparator，没有把 15 与 1 分成不同读优先级，也没有旧 read QoS 的更新或队内 cancel/reorder 入口。ARQOS 本身会由 S08:811–817 原样传播和寄存；“有传线”不能等同“能对已接受读排优先级”。

**源码事实与限制。** S05:168–195 的 starve 计数在保持该通道时从 2×C_MC_BURST_LEN 起步，只在 mc_app_rdy 且 command 被提交时加 C_MC_BURST_LEN，检查 bit 8；它不是无条件的 256 个墙钟周期等待计数。释放 hold 仍受 cmd_en_last 限制（202–203），所以不能把 256 作为已接受读或 PTE 的响应时延上界。wait 计数只在 mc_app_rdy 且该通道 valid 但无 command 时增加，bit 4 对应 16 的机会量；它可以释放 hold，但也不提供无条件时延保证。S05:217–274 的 RD_PRI_REG_STARVE_LIMIT 是另一 generate 分支，其可配置 C_AXI_WR_STARVE_LIMIT/256-clock read 逻辑不能混入本次 RD_PRI_REG 裁决。

**厂商文档状态。** 官方 [UG586 4.2 读写仲裁章节](https://docs.amd.com/r/en-US/ug586_7Series_MIS/Read-Priority-RD_PRI_REG) 的版本/标题入口已确认，正文提取超时。本席没有把旧版正文或标题当成 4.2 条款；以上具体阈值和 QoS 语义均由实际生成 4.2 RTL 推导。网页没有取得正文页码，页码为 unknown，不能伪填。

## 相同 ID、不同 ID 与返回顺序

**厂商原文已取得。** [AMD UG586 4.2 的 Reordering 章节](https://docs.amd.com/r/en-US/ug586_7Series_MIS/Reordering)（2024-11-13，HTML 的 NORM 和 RELAXED 两个小节）区分两层：控制器 bank/row/column 的实际服务可以改变顺序；UI 会把读返回恢复为原请求顺序。该网页没有纸质页码，准确入口是小节而不是伪造 PDF 页码。原文取得方式和短证据在 [doc-provenance.json](/home/xvhaocheng/work/iommu-platform/out/ddr-service-boundary-20261014/ip-semantics/doc-provenance.json)。

**源码事实。** S06:225–265 依序存入 MC 返回数据和每个成功下发 command 的元信息；元信息含 RID、last、ignore_begin/end。S06:268–326 从当前元信息及两级 skid 依次取出 RID，没有另一个 ID 的候选选择。S07:100–153 是顺序 FIFO，无按 ID/QoS 读取入口。S10:395–439 只按当前解包计划消费 R，直到这笔的最后一拍握手才取下个计划；844–846 传播 MC 返回的 RID。

**推论。** 在这个 shim+UI 路径里，相同 ID 与不同 ID 的后到 PTE 都不能先于较早 accepted payload 的响应完成返回。不同 ID 能合法在 AXI 协议中表示独立事务，不表示这个实现提供了读返回绕行。不能拿控制器内部可能先读到 PTE 所在 bank 的事实，直接推成 AXI 端 PTE 会先返回。

## 三个阶段的外部控制

| payload 的阶段 | AR-first / 提高 PTE ARQOS 能改变的部分 | 不能由该外部动作改变的部分 |
|---|---|---|
| 尚未被 MIG 的 AXI AR 接受 | 上游可以在其拥有、尚未提交的可选请求中先选 PTE；适用前提由 root 核对实际互连和稳定 VALID 承诺。将非零 QoS 放进新事务可能影响以后读写切换。 | 不能把已经呈现且被上游承诺必须保持的 payload VALID 任意替换；不能预先声称所有下游等待都消失。 |
| AR 已接受，尚未完成 R | 可以继续处理后续合法 AR，并为后续未提交请求选择次序；本席不否定普通限流、短 burst、提前 DS 等可实现的其他控制。 | 没有队内 reprioritize/cancel 接口；不能把后到 PTE 放到 active payload 或已发行返回序列之前。提高 ARQOS 不能重写旧 read 的字段。 |
| 首次 RVALID 已挂起且 RREADY=0 | 可以接收这拍后再前进；可以在接口允许的范围内接收/安排以后请求。 | 不能改变 held RID/RDATA/RRESP/RLAST，不能把另一个 ID 的 PTE 代替当前拍。降低 RREADY 会延迟这个顺序返回出口。 |

**held R 的源码链。** S06:194–201 在一拍已可见（不是头尾忽略拍）但未握手时不 pop 数据；273 和299–305 因 done 为 0 不替换正在输出的元信息。S07:117–142 在只有入队没有出队时同时移位和增加读地址，保持最旧项。S10:435–442 仅在握手时推进数据消费；若启用输出寄存，904–922 在 held 状态不更新；未启用时走926–933，仍继承上述顺序源的保持。ARQOS 或下一 ARVALID 没有连线到这些 held R 选择点。

## 接口费用、接受/否决及未解问题

接受：私有模型的已接受读队内 DS 提升不是该 MIG 外部 AR-first/QoS 接口的免费原语。否决：把 RD_PRI_REG、ARQOS 或 native bank service 重排直接等同于这个原语。接受理由是具体源路径中只有 active/head read、顺序展开、顺序返回和读写仲裁，没有可选择的第二读或队内修改端口。

在现有接口之外做这个能力，需要明确拥有请求和数据的重新排序位置。改 shim/native 集成至少要处理可调度读存储、依赖与同 ID 次序、native tag/数据匹配、返回缓冲及排序、有限 credit、head/held VALID 的稳定性、公平/写服务、错误和 reset 清理；也必须计入新增选择逻辑、状态/数据存储、带宽、时序和验证费用。预接收 AR-first 则需要实际的 DS/payload 分类、上游选择/保持逻辑和所需公平状态；设置 QoS 还需要整条真实属性传线及目标选择分支。这里不臆造门数、周期或 Fmax，也不先实施这些改动。

保留 unknown：实际实例与构建/板加载绑定；自然共同双头是否可达；该 normal 工作流是否确有 accepted payload 导致的 PTE wait；固定普通可实现控制及完整费用；改变能力后是否带来全任务独有收益；最强前作覆盖和论文贡献。物理 DDR 服务与旧私有模型不同，只说明旧排除结论的某个前提可能不适用于这里，不能据此接受创新或宣布收益存活。
