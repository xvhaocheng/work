# §39 CONF1/2 源码证据清点（只列源事实，不做因果/创新/架构判断）

## 0. 实际 build.argv 与有效配置

- 实际编译 argv 来源：`out/prepared-source-bypass-20261008/stage-evidence.json:5-25`。
- 实际 -G 覆盖：`PolicyEnable=1`、`DmaBufferDepth=3`、`SharedPoolMode=0`、`PoolSlots=1`、`SameBankDmasEnable=1`、`InterleaveAwEnable=0`、`SourceReadGateEnable=0`、`FrontierEnable=0`、`HeaderParkMode=0`、`ReverseReadOrderEnable=0`、`ParentDepth=4`（stage-evidence.json:15-25）。
- 未被 -G 覆盖的顶层默认：`NonblockEnable=1`、`NumOutstandingTrans=8`、`WFifoDepth=32`、`DdrAtomics=0`、`CpuWrites=0`（multi_system_top.sv:71-97；multi_actual_dma_top.sv:43-60）。
- `DmaBufferDepth` 默认 512（multi_actual_dma_top.sv:48；multi_system_top.sv:80），实际被 -G 覆盖为 3（stage-evidence.json:16）。
- `SharedPoolMode=0` 激活 `gen_no_pool`（multi_actual_dma_top.sv:632-653）；`SameBankDmasEnable=1` 使 `BankIdBit=3`（multi_actual_dma_top.sv:941；multi_system_top.sv:1303）。
- `PolicyEnable=1` 转发到 nonblock_banked_top（multi_actual_dma_top.sv:937）和 rv_iommu_tl_wrap（multi_banked_top.sv:825-829）。
- `CompletionGateEnable=SharedPoolMode>=4`，因此为 0（multi_actual_dma_top.sv:938），multi_banked_top.sv:647-660 选择 `rv_iommu_banked_ctl`，不是 `nonblock_completion_banked_ctl`。

## 1. 实例链与文件归属

- 顶层：`nonblock_actual_dma_system_top` = `out/prepared-source-bypass-20261008/multi_system_top.sv`。
- `i_dma`：`nonblock_actual_dma_top`（multi_system_top.sv:1127-1144；multi_actual_dma_top.sv）。
- `i_dma0/i_dma1`：两个 `dma_core_wrap`，`BufferDepth=DmaBufferDepth=3`、`NumAxInFlight=8`、`JobFifoDepth=8`、`MemSysDepth=0`、`RAWCouplingAvail=0`（multi_actual_dma_top.sv:354-411、413-470）。
- `i_dma_iommu_tr_mux`：4 输入 `axi_mux`，`MaxWTrans=8`（multi_actual_dma_top.sv:756-781；axi_mux.sv:28-55、317-333）。
- `i_sys`：`nonblock_banked_top`（multi_actual_dma_top.sv:932-1086；multi_banked_top.sv）。
- `i_sys.gen_banked.i_banked`：`rv_iommu_banked_ctl`（multi_banked_top.sv:652-685；rv_iommu_banked_ctl.sv:314-358）。
- `i_banked.gen_bank[b].i_bank`：`rv_iommu_trans_ctl_wrap`（rv_iommu_banked_ctl.sv:314-358；trans_ctl_write_limit.sv）。
- `i_banked.i_in_axi_demux`：`axi_demux`，SpillAw/W/B/Ar/R=0（rv_iommu_banked_ctl.sv:280-309）。
- `i_banked.i_out_axi_mux`：`axi_mux`，SpillAw/W/B/Ar/R=0（rv_iommu_banked_ctl.sv:418-449）。
- `trans_ctl_wrap.i_iommu_axi_demux_no_mrif`：`axi_demux`，SpillAw/W/B/Ar/R=1（trans_ctl_write_limit.sv:546-574）。
- `multi_system_top.i_xbar`：`axi_xbar`，实际 Cfg 见 multi_system_top.sv:910-924；实例见 927-956。
- `multi_system_top.i_ds_if`：`rv_iommu_ds_if`（multi_system_top.sv:460-482；文件未提供内容）。
- `multi_system_top.i_iommu_ds_delayer`：`axi_delayer`（multi_system_top.sv:485-504；文件未提供内容）。
- `multi_system_top.i_read_admission`：`nonblock_read_admission`（multi_system_top.sv:1302-1372；文件未提供内容）。
- preTL：`i_pre` = `pretl_owner_fragment #(ParentDepth(4))`（multi_actual_dma_top.sv:872-897；compact_pretl_owner_fragment.sv）。
- reuse：`i_reuse` = `parent_translation_reuse #(Depth=4)`（multi_banked_top.sv:810-823；hit_bypass_parent_translation.sv）。

## 2. payload 清点表

说明：payload 指 R/W data 字节；AR/AW header、ID、resp、last、strb、user、queue request record、translation response metadata 单列。

| 元件 / 实例链 | active generate / 条件 | 真实深度 | data width / type | payload bytes | metadata | 输入/输出 ready 与释放事件 |
|---|---|---|---|---|---|---|
| `i_dma0.i_idma_buffer` / `i_dma1.i_idma_buffer`；dma_core_wrap -> idma_backend -> idma_axi_transport_layer -> idma_buffer | Protocol=AXI；BufferDepth=DmaBufferDepth=3；StrbWidth=DataWidth/8 | 每字节 lane 3；8 lane（若 DataWidth=64） | `byte_t[StrbWidth-1:0]`，每 entry 8 bit | 每 DMA 3*8=24B；两个 DMA 共 48B | 无 data 侧 metadata；strb/last/user 在 transport layer 外侧 | fifo_v3 push/pop；ready_o=~full，valid_o=~empty；release on ready_i（idma_buffer.sv:33-52；fifo_v3.sv:42-45） |
| `trans_ctl_wrap.i_w_stream_fifo`；bank0/bank1 各一 | gen_bank b=0..1 | RVIOMMUCfg.WFifoDepth = 32/2 = 16 beats | `axi_tr_w_chan_t` = data64+strb8+last1+user1 = 74 bit | 每 bank 16*8=128B；两 bank 共 256B | strb/last/user 10 bit/beat | `tr_w_ready`=fifo ready；`w_ready=(w_cnt_q!=0)?comp_resp.w_ready:0`；WLAST 时 decr_w_cnt；`w_slots_full` 阻塞 AW（trans_ctl_write_limit.sv:174-185、225-244、633） |
| `trans_ctl_wrap.i_iommu_axi_demux_no_mrif.i_w_spill_reg`；bank0/bank1 各一 | gen_mrif_support_disabled；SpillW=1 | 2 entries | `axi_comp_w_chan_t` = data64+strb8+user1 = 73 bit | 每 bank 2*8=16B；两 bank 共 32B | strb/user 9 bit/entry | spill_register_flushable A/B；release on ready_i（trans_ctl_write_limit.sv:558-562；axi_demux.sv:119-131；spill_register_flushable.sv:78-96） |
| `trans_ctl_wrap.i_iommu_axi_demux_no_mrif.i_r_spill_reg`；bank0/bank1 各一 | gen_mrif_support_disabled；SpillR=1 | 2 entries | `axi_comp_r_chan_t` = data64+id4+resp2+last1+user1 = 72 bit | 每 bank 2*8=16B；两 bank 共 32B | id/resp/last/user 8 bit/entry | spill_register_flushable A/B；release on ready_i（trans_ctl_write_limit.sv:562；axi_demux.sv:175-187；spill_register_flushable.sv:78-96） |
| `i_banked.i_in_axi_demux` | SpillAw/W/B/Ar/R=0 | 无 spill payload | `axi_tr_*` | 0 | ID counters、W select counters | Spill=0 时 spill_register Bypass=~0=1；无 payload 存储（rv_iommu_banked_ctl.sv:280-299；axi_demux.sv:89-187） |
| `i_banked.i_out_axi_mux` | SpillAw/W/B/Ar/R=0 | W FIFO depth MaxWTrans=8 | mux W FIFO `switch_id_t`=1 bit；W data 组合旁路 | 0 | W FIFO 8*1 bit route decision | Spill=0 旁路；W FIFO push on w_fifo_push，pop on WLAST（rv_iommu_banked_ctl.sv:418-449；axi_mux.sv:317-333、365） |
| `i_dma_iommu_tr_mux` | NoSlvPorts=4，MaxWTrans=8；SpillAw=1、SpillAr=1、SpillW=0、SpillB=0、SpillR=0 默认 | AW/AR spill 2 entries；W FIFO 8 | AW/AR header；W FIFO `switch_id_t`=2 bit | W/R payload 0 | AW/AR header spill；W FIFO 8*2 bit route decision | W FIFO push on w_fifo_push，pop on WLAST；R/W Spill=0 旁路（multi_actual_dma_top.sv:756-781；axi_mux.sv:50-55、317-333、451-463） |
| `i_xbar.gen_mst_port_mux[0].i_axi_mux`（DDR master port） | `axi_xbar` 每个 master port 一个 axi_mux；SpillR=Cfg.LatencyMode[0]；SpillW=Cfg.LatencyMode[3] | 若对应 bit=1，R spill 2 entries；W spill 2 entries | `mst_r_chan_t`=data64+id6+resp2+last1+user1=74 bit；W=73 bit | 若 SpillR=1，每 DDR master port R 2*8=16B shared；若 SpillW=1，W 2*8=16B shared | R/W spill A/B；W FIFO depth MaxSlvTrans=8，`switch_id_t`=2 bit | R spill release on `slv_r_readies[switch_r_id]`；W FIFO pop on WLAST（multi_system_top.sv:910-924、927-956；axi_xbar.sv:131-164；axi_mux.sv:451-463、317-333） |
| `i_iommu_ds_delayer` | 实例存在（multi_system_top.sv:485-504） | 未知 | DS page-table path | 未知；非 DMA payload 路径 | 未知 | 文件未提供 |
| `i_read_admission` | SharedPoolMode=0 => ReadTagQueueEnable=0；HeaderParkMode=0 => ReadTagPerStreamEnable=0 | 未知 | AR request/tag | 0（fragment_r_valid/chan 直接旁路，multi_system_top.sv:1294-1296） | AR request records | 文件未提供 |
| `i_pre` / pretl_owner_fragment | ParentDepth=4；两个 flow | 无 R/W payload 存储 | `ar_t` parent headers；W metadata ring D=8 | 0 | `parent_q[2][4]` AR headers；`returned_q`；`v/p_owner_q`、`v/p_seq_q`、`v_offset_q`、`v_beats_q` | R 组合旁路：`s_r_valid_o=busy_q && m_r_valid_i`，`m_r_ready_o=busy_q && s_r_ready_i`；busy_q 直到 `m_r_i.last`（compact_pretl_owner_fragment.sv:34-54、136-141、251-270） |
| `i_reuse` / parent_translation_reuse | Depth=4 | 无 R/W payload 存储 | `normal_resp_t` 83 bit translation response | 0 | `value_q[2][4]`、`seq_q[2][4]`、`prepared_q`、`hit_resp_q`、`slow_req_q` | slow request 串行；`slow_q` 在 `m_resp_valid_i && m_resp_ready_o` 清除（hit_bypass_parent_translation.sv:38-46、151-157、216-233、237-241） |
| `idma_backend.i_r_dp_req` | Protocol=AXI 等 | NumAxInFlight=8 | `r_dp_req_t` metadata | 0 | offset/tailer/shift/decouple_aw | stream FIFO ready/valid（idma_backend.sv:554-570） |
| `idma_backend.i_w_dp_req` | Protocol!=OBI | NumAxInFlight=8 | `w_dp_req_t` metadata | 0 | offset/tailer/num_beats/is_single | stream FIFO ready/valid（idma_backend.sv:590-608） |
| `idma_backend.i_w_last` | if(1'b1) | MetaFifoDepth=BufferDepth+NumAxInFlight+MemSysDepth=3+8+0=11 | `logic[1:0]` | 0 | super_last/last | push on `w_valid & w_ready`，pop on `w_dp_rsp_valid & w_dp_rsp_ready`（idma_backend.sv:632-649） |
| `idma_backend.i_ar_fall_through_register` | Protocol=AXI | 1 header | `ar_chan_t` AR header | 0 | AR header | fall-through；RAWCouplingAvail=0 时 AW 走 `gen_r_aw_bypass`（idma_backend.sv:612-625、837-857） |
| `idma_reg64_frontend` | 始终 | 计数器/寄存器 | batch/launch/gap/counter | 0 | counters、state | 无 R/W payload（idma_reg64_frontend.sv:91-132、458-637） |
| DDR behavioural memory array | C++ testbench，非 RTL | 未知 | 未知 | 未知；不应计为有限在途 tail 存储 | 未知 | multi_system_top.sv:53-54 明确 DLL 是 C++ behavioural model；文件未提供 |

## 3. 名义 48B = 两个 DMA 之和，24B/DMA 的证据

- 两个 `dma_core_wrap` 均以 `BufferDepth(DmaBufferDepth)` 实例化，实际 DmaBufferDepth=3（multi_actual_dma_top.sv:354-370、413-428；stage-evidence.json:16）。
- `idma_backend` 将 `BufferDepth` 传给 `idma_axi_transport_layer`（idma_backend.sv:665-671）。
- `idma_axi_transport_layer` 以 `BufferDepth` 实例化 `idma_buffer`，并以 `StrbWidth=DataWidth/8` 生成每字节 FIFO（idma_axi_transport_layer.sv:468-484）。
- `idma_buffer` 对每个 byte lane 实例化 `idma_stream_fifo #(Depth(BufferDepth))`（idma_buffer.sv:33-52）。
- 实际顶层 AxiDataWidth=64（multi_actual_dma_top.sv:233），因此若 dma_core_wrap 按 64-bit 映射，StrbWidth=8；每个 DMA 的该 buffer payload 容量为 3*8=24B；两个 DMA 共 48B。
- 不要把其他 cut/spill/FIFO 存储加总当作独立停车 B：上表中其余 R/W payload 存储是另外的 bank/shared 元件，且许多表项存的是 AR/AW header、ID/route record、translation response metadata。

## 4. single-global source-child route 分段：原型增加 vs 生产已有

### 4.1 §39/原型测试包装增加段（out/prepared-source-bypass）

- single-global source-child 的直接 AR/R 合并逻辑在 `multi_actual_dma_top.sv:549-576`：
  - `direct_ar_valid=pre_ar_valid && pre_direct`（561）
  - `pre_ar_ready=pre_direct ? comp_ar_ready : tr_ar_ready`（562）
  - `pre_raw_r_valid=pre_direct ? (comp_r_valid && comp_r_id[0]) : tr_r_valid`（563）
  - `tc_comp_ar_ready=comp_ar_ready && !direct_ar_valid`（564）
  - `tc_comp_r_valid=comp_r_valid && !comp_r_id[0]`（565）
  - `comp_r_ready=comp_r_id[0] ? (pre_direct && pre_r_ready) : tc_comp_r_ready`（566）
  - `comp_ar_id=direct_ar_valid ? (pre_ar.id | 4'd1) : tc_comp_ar_id`（569）
  - 注释：IDs 0/4 是唯一原始 source ID；bit0 标记 direct（549-550）。
- owner/parent tagging 在 `multi_actual_dma_top.sv:610-617`：
  - `pre_parent_ar.id = {1'b0, pre_owner, pool_m_req[pre_owner].ar.id}`（614）
  - `pool_m_rsp[0].ar_ready = pre_parent_valid && pre_parent_ready && !pre_owner`（616）
  - `pool_m_rsp[1].ar_ready = pre_parent_valid && pre_parent_ready && pre_owner`（617）
- `i_pre pretl_owner_fragment #(ParentDepth(4))` 实例在 `multi_actual_dma_top.sv:872-897`。该模块是 `compact_pretl_owner_fragment.sv`（文件头 Codex section 33，非生产 RTL）。
- `i_reuse parent_translation_reuse` 在 `multi_banked_top.sv:810-823`，模块是 `hit_bypass_parent_translation.sv`（文件头 Codex section 38，非生产 RTL）。
- `multi_banked_top.sv`、`multi_actual_dma_top.sv`、`multi_system_top.sv` 自身均为 test-only/诊断 wrapper（multi_actual_dma_top.sv:5；multi_system_top.sv:5）。

### 4.2 生产已有段（cva6 目录）

- `rv_iommu_banked_ctl`、`rv_iommu_trans_ctl_wrap`（trans_ctl_write_limit.sv）、两个真实 `dma_core_wrap`/iDMA、`axi_mux`、`axi_demux`、`axi_xbar`、`spill_register`、`fifo_v3` 等为生产/vendor 源。
- 生产 `rv_iommu_banked_ctl` 的 bank 选择、input demux、output mux、in-flight 计数在 rv_iommu_banked_ctl.sv:241-309、314-358、418-586。
- 生产 `rv_iommu_trans_ctl_wrap` 的 AW/AR/W FIFO 与 completion demux 在 trans_ctl_write_limit.sv:121-164、227-245、546-574。
- 生产 `axi_mux` R/W 路由在 axi_mux.sv:317-463；生产 `axi_demux` spill/R 仲裁在 axi_demux.sv:89-187、162-209、383-408。

## 5. R 返回路径阻塞与释放证据

- `compact_pretl_owner_fragment`：
  - `m_ar_valid_o=offered_q`（134）；`m_ar_o=child_q`（135）。
  - `s_r_valid_o = busy_q && m_r_valid_i`（136）。
  - `m_r_ready_o = busy_q && s_r_ready_i`（137）。
  - `s_r_o = m_r_i`（138）；`s_r_o.last` 由 `returned_q[child_owner_q][...] == parent_q[...] .len` 生成（139-141）。
  - 在 `m_ar_valid_o && m_ar_ready_i` 时 `offered_q<=0; busy_q<=1; child_count_o++`（251-255）。
  - `busy_q` 只在 `m_r_valid_i && m_r_ready_o && m_r_i.last` 时清 0（256-270）。
  - 因此该 single-child 元件在 child 全 burst 期间保持 busy_q；只要 downstream `s_r_ready_i` 不为 1，`m_r_ready_o=0`，同物理返回路径被阻塞，直到该 child 的 RLAST 被接受。
- `axi_demux_simple` R 仲裁：`i_r_mux` 使用 `rr_arb_tree`，`AxiVldRdy=1`、`LockIn=1`（axi_demux_simple.sv:383-400）。这是源事实；未提供 `rr_arb_tree` 内容，具体锁定行为需补文件确认。
- `rv_iommu_banked_ctl.i_in_axi_demux` 的 R 仲裁同样来自 `axi_demux_simple`（rv_iommu_banked_ctl.sv:280-309；axi_demux.sv:189-209）。
- `rv_iommu_banked_ctl.i_out_axi_mux` SpillR=0（rv_iommu_banked_ctl.sv:436-440），因此该 mux R 路径为旁路，无额外 R payload spill。

## 6. actual compiled AXI xbar：FIFO R model / 跨 ID 乱序的源事实

- 实际 Cfg：`NoSlvPorts=4`、`NoMstPorts=2`、`MaxMstTrans=8`、`MaxSlvTrans=8`、`FallThrough=0`、`LatencyMode=CUT_ALL_PORTS`、`PipelineStages=1`、`AxiIdWidthSlvPorts=4`、`UniqueIds=0`（multi_system_top.sv:910-924）。
- `axi_xbar` 实例：`ATOPs=1`、`Connectivity='1'`（multi_system_top.sv:927-956）。
- `axi_xbar` 每个 master port 实例化一个 `axi_mux`；SpillR/W/AW/B/AR 来自 `Cfg.LatencyMode[0/3/4/2/1]`（axi_xbar.sv:131-164）。
- `axi_mux` R 通道：
  - `slv_r_chans = {NoSlvPorts{mst_r_chan}}`（446）。
  - `switch_r_id = mst_r_chan.id[SlvAxiIDWidth+:MstIdxBits]`（448）。
  - `slv_r_valids = (mst_r_valid) ? (1 << switch_r_id) : '0`（449）。
  - 可选 `i_r_spill_reg`：`Bypass(~SpillR)`，是两 entry spill，不是 per-ID FIFO（451-463）。
- 源事实：axi_mux 仅按 master R ID 的高位选择 slave port；没有 per-ID 重排逻辑，也没有显式同 ID 顺序保持队列。R 到达后按单条 R stream 路由到被选 slave port。
- `axi_xbar_unmuxed` 未提供内容；其内部 demux/ID counters/R 仲裁/是否有额外存储未知。不能从已给文件断定 entire xbar 无跨 ID 乱序。
- `axi_pkg` 未提供内容；`CUT_ALL_PORTS` 的实际 bit 值未知，因此上述 SpillR/SpillW 是否真正启用需补 axi_pkg。

## 7. metadata-only / request-record 组件（不是 R/W payload）

- `idma_reg64_frontend`：batch/launch/gap counters、state；无 payload（idma_reg64_frontend.sv:91-132、458-637）。
- `idma_backend.i_r_dp_req`、`i_w_dp_req`、`i_w_last`、`i_ar_fall_through_register`、RAWCouplingAvail=0 的 `i_aw_fall_through_register`：request/status metadata（idma_backend.sv:554-608、612-649、837-857）。
- `rv_iommu_trans_ctl_wrap.i_aw_stream_fifo`、`i_ar_stream_fifo`：AW/AR header，每 bank depth=NumOutstandingTrans=4（trans_ctl_write_limit.sv:121-164；rv_iommu_banked_ctl.sv:162-164）。
- `rv_iommu_banked_ctl.i_in_axi_demux`：ID counters、W select counters（axi_demux_simple.sv:210-228、355-373、236-253）。
- `rv_iommu_banked_ctl.i_out_axi_mux.i_w_fifo`：switch_id_t route record，depth=8，1 bit/entry（axi_mux.sv:317-333）。
- `multi_actual_dma_top.i_dma_iommu_tr_mux.i_w_fifo`：switch_id_t route record，depth=8，2 bit/entry（axi_mux.sv:317-333）。
- `multi_system_top.i_xbar` per-master `axi_mux.i_w_fifo`：route record，depth=8，2 bit/entry（axi_xbar.sv:131-164；axi_mux.sv:317-333）。
- `pretl_owner_fragment`：`parent_q[2][4]` AR header、`returned_q`、W metadata ring D=8（compact_pretl_owner_fragment.sv:33-54）。
- `parent_translation_reuse`：`value_q[2][4]` normal_resp_t 83-bit translation response metadata（hit_bypass_parent_translation.sv:38-46、237-241）。
- `nonblock_read_admission`：AR request/tag record；R 数据直接旁路（multi_system_top.sv:1291-1296、1302-1372；文件未提供内容）。
- `axi_mux` AW/AR spill、`axi_demux` AW/AR/B spill：header/response metadata，不是 R/W payload（axi_mux.sv:335-440；axi_demux.sv:89-187）。

## 8. 关键逐行原文

- `multi_actual_dma_top.sv:549-566`
  - 549: `// One global source child: its normal TC AR and direct AR are exclusive.`
  - 550: `// IDs 0/4 are the only original source IDs in this harness; bit0 tags direct.`
  - 561: `assign direct_ar_valid=pre_ar_valid && pre_direct;`
  - 562: `assign pre_ar_ready=pre_direct ? comp_ar_ready : tr_ar_ready;`
  - 563: `assign pre_raw_r_valid=pre_direct ? (comp_r_valid && comp_r_id[0]) : tr_r_valid;`
  - 564: `assign tc_comp_ar_ready=comp_ar_ready && !direct_ar_valid;`
  - 565: `assign tc_comp_r_valid=comp_r_valid && !comp_r_id[0];`
  - 566: `assign comp_r_ready=comp_r_id[0] ? (pre_direct && pre_r_ready) : tc_comp_r_ready;`
- `multi_actual_dma_top.sv:569`
  - `comp_ar_id=direct_ar_valid ? (pre_ar.id | 4'd1) : tc_comp_ar_id;`
- `multi_actual_dma_top.sv:872-897`
  - `pretl_owner_fragment #(.ParentDepth(4), .ar_t(axi_tr4_ar_chan_t), .r_t(axi_tr4_r_chan_t)) i_pre (...)`
- `compact_pretl_owner_fragment.sv:136-141`
  - `s_r_valid_o = busy_q && m_r_valid_i;`
  - `m_r_ready_o = busy_q && s_r_ready_i;`
  - `s_r_o = m_r_i;`
  - `s_r_o.last = busy_q && returned_q[...] == 9'(parent_q[...] .len);`
- `compact_pretl_owner_fragment.sv:251-270`
  - `if (m_ar_valid_o && m_ar_ready_i) begin offered_q <= 0; busy_q <= 1; ... end`
  - `if (m_r_valid_i && m_r_ready_o) begin ... if (m_r_i.last) begin busy_q <= 0; child_direct_q<=0; end end`
- `idma_axi_transport_layer.sv:468-484`
  - `idma_buffer #(.BufferDepth(BufferDepth), .StrbWidth(StrbWidth), ...)`
- `idma_buffer.sv:33-52`
  - `for (genvar i = 0; i < StrbWidth; i++) begin : gen_fifo_buffer`
  - `idma_stream_fifo #(.type_t(byte_t), .Depth(BufferDepth), ...)`
- `idma_backend.sv:665-671`
  - `idma_axi_transport_layer #(.DataWidth(DataWidth), .BufferDepth(BufferDepth), ...)`
- `trans_ctl_write_limit.sv:174-185`
  - `typedef logic [$clog2(RVIOMMUCfg.NumOutstandingTrans):0] w_cnt_t;`
  - `assign incr_w_cnt = (axi_comp_req.aw_valid & axi_comp_resp.aw_ready);`
  - `assign decr_w_cnt = (w_valid & w_ready & w_data.last);`
  - `assign w_slots_full = w_cnt_ovf || (w_cnt_q >= w_cnt_t'(RVIOMMUCfg.NumOutstandingTrans));`
- `trans_ctl_write_limit.sv:225-244`
  - `assign w_ready = (w_cnt_q != '0) ? (axi_comp_resp.w_ready) : (1'b0);`
  - `stream_fifo #(.DEPTH(RVIOMMUCfg.WFifoDepth), .T(axi_tr_w_chan_t)) i_w_stream_fifo (...)`
- `trans_ctl_write_limit.sv:558-562`
  - `.SpillAw(1'b1), .SpillW(1'b1), .SpillB(1'b1), .SpillAr(1'b1), .SpillR(1'b1)`
- `rv_iommu_banked_ctl.sv:162-164`
  - `NumOutstandingTrans : RVIOMMUCfg.NumOutstandingTrans / 2,`
  - `WFifoDepth : RVIOMMUCfg.WFifoDepth / 2,`
- `rv_iommu_banked_ctl.sv:294-298`
  - `.SpillAw(1'b0), .SpillW(1'b0), .SpillB(1'b0), .SpillAr(1'b0), .SpillR(1'b0)`
- `rv_iommu_banked_ctl.sv:436-440`
  - `.SpillAw(1'b0), .SpillW(1'b0), .SpillB(1'b0), .SpillAr(1'b0), .SpillR(1'b0)`
- `axi_mux.sv:50-55`
  - `parameter bit SpillAw = 1'b1; parameter bit SpillW = 1'b0; parameter bit SpillB = 1'b0; parameter bit SpillAr = 1'b1; parameter bit SpillR = 1'b0;`
- `axi_mux.sv:317-333`
  - `fifo_v3 #(.FALL_THROUGH(FallThrough), .DEPTH(MaxWTrans), .dtype(switch_id_t)) i_w_fifo (...)`
- `axi_mux.sv:446-463`
  - `assign slv_r_chans = {NoSlvPorts{mst_r_chan}};`
  - `assign switch_r_id = mst_r_chan.id[SlvAxiIDWidth+:MstIdxBits];`
  - `assign slv_r_valids = (mst_r_valid) ? (1 << switch_r_id) : '0;`
  - `spill_register #(.T(mst_r_chan_t), .Bypass(~SpillR)) i_r_spill_reg (...)`
- `multi_system_top.sv:910-924`
  - `localparam axi_pkg::xbar_cfg_t XbarCfg = '{ NoSlvPorts:4, NoMstPorts:2, MaxMstTrans:8, MaxSlvTrans:8, FallThrough:1'b0, LatencyMode:axi_pkg::CUT_ALL_PORTS, PipelineStages:1, AxiIdWidthSlvPorts:4, AxiIdUsedSlvPorts:4, UniqueIds:1'b0, AxiAddrWidth:64, AxiDataWidth:64, NoAddrRules:2 };`
- `multi_system_top.sv:1302-1372`
  - `nonblock_read_admission #(.BankIdBit(SameBankDmasEnable ? 3 : 2), .IdWidth(4), .ReadTagQueueEnable(SharedPoolMode >= 4), .ReadTagPerStreamEnable(HeaderParkMode != 0), ...)`
- `hit_bypass_parent_translation.sv:151-157`
  - `m_req_o=prep_offer_q ? slow_req_q : s_req_i;`
  - `m_valid_o=!slow_q && (prep_offer_q || (!hit_q && s_valid_i && !reuse));`
  - `s_ready_o=!hit_q && ((reuse && (!slow_q || slow_prepare_q)) || (!reuse && !slow_q && !prep_offer_q && m_ready_i));`
- `hit_bypass_parent_translation.sv:216-233`
  - `if (m_resp_valid_i && m_resp_ready_o) begin ... slow_q<=0; ... end`
- `spill_register_flushable.sv:78-96`
  - `assign a_fill = valid_i && ready_o && (!flush_i);`
  - `assign a_drain = (a_full_q && !b_full_q) || flush_i;`
  - `assign b_fill = a_drain && (!ready_i) && (!flush_i);`
  - `assign b_drain = (b_full_q && ready_i) || flush_i;`
  - `assign ready_o = !a_full_q || !b_full_q;`
  - `assign valid_o = a_full_q | b_full_q;`
  - `assign data_o = b_full_q ? b_data_q : a_data_q;`

## 9. 源级验收检查

- 核对 `stage-evidence.json:5-25` 的 -G 覆盖，而不是默认 DmaBufferDepth=512。
- 核对 DmaBufferDepth=3 是否经 multi_actual_dma_top.sv:363/422 -> idma_backend.sv:667 -> idma_axi_transport_layer.sv:469 -> idma_buffer.sv:37 传到 byte FIFO depth。
- 核对 SharedPoolMode=0 选 gen_no_pool（multi_actual_dma_top.sv:632），无 completion pool。
- 核对 SameBankDmasEnable=1 => BankIdBit=3（multi_actual_dma_top.sv:941；multi_system_top.sv:1303）。
- 核对 pretl_owner_fragment ParentDepth=4（multi_actual_dma_top.sv:872）且 busy_q 只在 RLAST 清除（compact_pretl_owner_fragment.sv:256-270）。
- 核对 48B 只按两个 DMA 的 idma_buffer 24B/DMA 计算；不要把其他 cut/spill/FIFO 存储加总为独立停车 B。
- 补缺失文件后，再核对 DDR C++ array、axi_xbar_unmuxed、dma_core_wrap、rv_iommu_tl_wrap 内部是否存在额外 R/W payload 存储。

## 10. 未知与最小补文件建议

- C++ DDR behavioural memory array 在 `out/prepared-source-bypass-20261008/multi_test.cpp`，内容未提供；不能当成有限在途 tail 存储。
- `dma_core_wrap.sv` 未提供；DataWidth、Protocol、HardwareLegalizer、ErrorCap、RejectZeroTransfers、内部 FIFO 实例未验证。
- `idma_stream_fifo.sv` 未提供；实际 FIFO 实现/深度行为未验证。
- `axi_xbar_unmuxed.sv` 未提供；xbar 内部 demux、R 仲裁、ID counters、spill 未验证。
- `axi_pkg.sv` 未提供；`CUT_ALL_PORTS` 实际 bit 值未知，故 xbar 的 SpillR/SpillW 是否启用未知。
- `axi_delayer.sv` 未提供；DS delayer 内部是否有 payload 寄存器未知。
- `nonblock_read_admission.sv` 未提供；AR/tag queue 深度与是否存 payload 未知。
- `rv_iommu_tl_wrap.sv`、`rv_iommu_ptw.sv`、`rv_iommu_nonblock_hit.sv` 等未提供；TL/PTW 内部缓冲未验证。
- `nonblock_completion_pool.sv`、`nonblock_completion_pool_late.sv` 因 SharedPoolMode=0 未激活，但内容未提供。
- `nonblock_completion_banked_ctl.sv` 因 CompletionGateEnable=0 未激活（multi_banked_top.sv:647-660），但内容未提供。
- `axi_err_slv.sv`、`axi_id_prepend.sv`、`axi_riscv_atomics` 等未提供；DdrAtomics=0 时未激活 atomics，但 error slave 仍可能出现在 completion demux 中。
- `rr_arb_tree.sv`、`stream_arbiter.sv` 未提供；R 仲裁锁定和 ID 行为需补文件确认。
