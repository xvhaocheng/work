# iDMA → IOMMU 双端 memory-to-memory 窄审查：目的 AW 与源翻译解耦、CQ invalidation/IOFENCE 排空、普通基线资源账本

**范围声明**：本轮只做给定源码的闭合性证据提取，不实施、不仿真、不板测、不承诺创新。所有“可能/需核实”均标为证据缺口，不当作已成立结论。原始问题保留，并明确给出可否证条件（含“允许被否定”）。

---

## 0. 最少根复核窗口（Codex 优先直接复核）

先看这 8 个窗口，它们决定问题 1/2 能否闭合：

| # | 文件:行 | 复核什么 |
|---|---|---|
| W1 | `idma_axi_transport_layer.sv:196–360` | AW 直连、W 数据由 buffer_out、`ready_to_write`/`w_valid` 门 |
| W2 | `idma_backend.sv:471–473` + `:811–858` | `r_ready/w_ready` 门；R-AW coupler / bypass 两条分支 |
| W3 | `idma_pkg.sv:81–93` | `decouple_aw`/`decouple_rw` 语义（“AWs will only be sent after the first corresponding R is received”） |
| W4 | `rv_iommu_trans_ctl_wrap.sv:620–733` | AW 优先、物理 AW/AR 只在 COMPLETE 发出、单笔翻译在途 |
| W5 | `rv_iommu_trans_ctl_wrap.sv:166–255` + `:329–369` | `w_cnt_q` 门控 W 转发、W FIFO、B/R passthrough |
| W6 | `rv_iommu_cq_handler.sv:269–295` + `:297–507` | cqh 推进时机、IOTINVAL 单拍、IOFENCE（含 `TODO` PR/PW） |
| W7 | `rv_iommu_top.sv:136–165` | `trans_req_data_t`/`trans_resp_data_t` 有无 src/dst 关联/tag |
| W8 | `rv_iommu_tl_wrap.sv:284–302, 657–684, 758–803` | policy quiet、IOTLB inval（接线）、单 PTW |

---

## 1. 关键发现表

| # | 判别点 | 直接证据 | 结论（本证据范围内） |
|---|---|---|---|
| F1 | iDMA 原始 AW 是否在源读之前发出 | `idma_pkg.sv:82–83`；`idma_backend.sv:811–858`；`dma_core_wrap.sv:28,314`；`dma_core_wrap.sv:369,436` | **无法闭合**。取决于 `RAWCouplingAvail`（dma_core_wrap 默认 0，backend 默认 1，实际 SoC 绑定在未给文件）与 `decouple_aw`（frontend 未给），且 coupler 源码未给 |
| F2 | IOMMU 收到原始 AW 后的接收位置 | `rv_iommu_trans_ctl_wrap.sv:121–139, 628–643` | 独立 AW FIFO；AW 优先于 AR 被选入 FSM |
| F3 | IOMMU 物理 AW 发出条件 | `rv_iommu_trans_ctl_wrap.sv:332–344, 716–727` | **物理 AW 必须目的翻译已完成**（`spaddr` 来自 `trans_resp_data_q`）。这一条对“目的翻译”成立，但对“源翻译”无任何依赖 |
| F4 | 目的物理 AW 是否可早于源翻译发出 | `rv_iommu_trans_ctl_wrap.sv:105, 678–702, 628 vs 646` | 控制器单笔翻译串行，且 **AW 先于 AR**。一旦原始 AW 被接纳，物理 AW 的发出与源翻译完全解耦 → **在 IOMMU 层是允许的**（若 F1 的原始 AW 已先到） |
| F5 | W 数据依赖 | `idma_axi_transport_layer.sv:232, 343, 360, 388` | **W 数据物理上依赖 R 数据**（buffer 由 `axi_rsp_i.r.data` 填充）。这是 iDMA 内部依赖，IOMMU 不可见 |
| F6 | W 转发门（IOMMU 侧） | `rv_iommu_trans_ctl_wrap.sv:178–179, 220, 360` | W 只能在 `w_cnt_q != 0`（已有 AW 被 demux 接受）后转发；IOMMU **不会**、也**无法**感知 W 数据来自哪个源读 |
| F7 | B 完成路径 | `idma_axi_transport_layer.sv:447–455`；`rv_iommu_trans_ctl_wrap.sv:362, 365` | B 从内存直通回 iDMA（`b_ready = w_dp_ready_i`），IOMMU 不做匹配/排序 |
| F8 | CQ IOTINVAL 语义 | `rv_iommu_cq_handler.sv:310–341` | 单拍脉冲 `inval_vma/inval_gvma`，随后回 IDLE；**CQ 层无等待、无排空** |
| F9 | CQ IOFENCE 是否等物理 W 排空 | `rv_iommu_cq_handler.sv:348–395`（`:362` `TODO: Add IOFENCE support with PR and PW using R/W counters`） | **没有实现 PR/PW 排空**。IOFENCE 的排空对象是“所有先前已取指命令已完成”，不是未决 DMA 物理读写 |
| F10 | cqh 推进时机 | `rv_iommu_cq_handler.sv:282`（REGISTER 内 `cq_head_o = cq_head_i + 1`） | cqh 在**取指寄存器时刻**推进，而非命令效果完成后；与“cqh 越过 IOFENCE 即保证完成”需要额外核对寄存器块/规范 |
| F11 | 是否存在全局撤销锁/暂停源翻译 | `rv_iommu_trans_ctl_wrap.sv`（无 cancel/暂停输入）；`rv_iommu_top.sv:281–287`（`policy_quiet` 是研究配置静默，非失效排空） | 当前控制器**没有**“更新暂停源翻译”的机制。doc §4 的环需要外部机制成立 |
| F12 | src/dst 关联（基线关键） | `rv_iommu_top.sv:136–145`（无 tag/配对字段）；`idma_legalizer.sv:98–100, 289–299`（r_tf/w_tf 独立）；`idma_backend.sv:378–420`（AR/AW 同 `axi_id`） | **现有接口没有 src/dst 配对关联**。AR 与 AW 只共享 `axi_id`/stream_id/substream_id |

---

## 2. 问题 1 的逐层闭合（iDMA 原始 AW/AR → IOMMU 接收 → 物理 AW/AR → W 数据依赖 → B）

### 2.1 iDMA 原始 AW/AR 发出

- AR 组装：`idma_backend.sv:378–397`（`r_req.ar_req.addr = src_addr`），或 legalizer 路径 `idma_legalizer.sv:326–345`。
- AW 组装：`idma_backend.sv:400–420`（`w_req.aw_req.addr = dst_addr`），或 `idma_legalizer.sv:348–368`。
- 二者携带相同的 `id`/`stream_id`/`ss_id_valid`/`substream_id`（`opt_tf_q.axi_id/device_id/pv/process_id`，`idma_legalizer.sv:327–331, 349–353`）。**没有区分 src/dst 的字段**。

- 发出门：`idma_backend.sv:472–473`
  - `assign r_ready = r_dp_req_in_ready & ar_ready;`
  - `assign w_ready = w_dp_req_in_ready & aw_ready & w_last_ready;`
- `aw_ready` 的两条来源（`idma_backend.sv:811–858`）：
  - `RAWCouplingAvail=1` → `idma_channel_coupler`（端口 `r_rsp_valid_i`、`r_rsp_first_i`、`r_decouple_aw_i`、`aw_decouple_aw_i`…，`:813–836`）
  - `RAWCouplingAvail=0` → 纯 `fall_through_register`（`:841–854`），AW 不被任何 R 事件拖住
- 耦合语义：`idma_pkg.sv:82–83`
  - `decouple_aw`：**“AWs will only be sent after the first corresponding `R` is received”**
  - `decouple_rw`：完全解耦 R/W，“**can cause deadlocks**”
- legalizer 的 R/W 联合推进：`idma_legalizer.sv:452–466`
  - `decouple_rw=0`：`r_tf_ena = w_tf_ena = r_ready_i & w_ready_i & !flush_i`（同拍推进）
  - `decouple_rw=1`：各自推进

**缺口 G1**：`idma_channel_coupler.sv` 未给。没有它，"AW 是否等到对应 R"、"first R 是哪一笔"无法判定。
**缺口 G2**：`idma_reg64_frontend.sv` 未给。无法判定 batch memory-to-memory copy 实际下的 `decouple_aw`/`decouple_rw`/`ttype` 值。
**缺口 G3**：`ariane_peripherals_xilinx.sv` 未给（doc 引用其 `:1202` 绑 `BUFFER_DEPTH`）。无法判定 `RAW_COUPLING_AVAIL`/`IS_TWO_D`/`ResearchPort` 在生成本平台的真实绑定。注意 `dma_core_wrap.sv:28` 默认 `RAWCouplingAvail = 32'd0`，而 `idma_backend.sv:34` 默认 `1'b1` —— 两个默认相反，必须以 SoC 绑定为准。

### 2.2 IOMMU 接收

- AW/AR 各自入 FIFO：`rv_iommu_trans_ctl_wrap.sv:121–139`（AW）、`:146–164`（AR），深度 `RVIOMMUCfg.NumOutstandingTrans`。
- FSM 接收选择：`:620–669`。**AW 分支在前**（`:628`），AR 分支在后（`:646`），且 AW 分支只要 `aw_valid && !w_cnt_ovf` 即进入。这是 doc §7 所说的“已 AW 优先”，**属既有实现，不作新点**。
- AR 可选门：`:646–647`，`(!ReadAdmissionEnable || read_admit_i || (fullbuffer_job_i && ar_data.stream_id == FullBufferDID))`。串行实例把 `read_admit_i=1'b1`、`fullbuffer_job_i=1'b0`（`rv_iommu_top.sv:448–449`），且未绑定 `ReadAdmissionEnable`（`:419–445`）→ 默认 0 → 读无条件被接纳。

### 2.3 IOMMU 物理 AW/AR 发出

- `completion_bus`（`:329–369`）：
  - `axi_comp_req.aw.addr = addr_t'(trans_resp_data_q.spaddr)`（`:333`），`aw_valid = complete_write`（`:344`）
  - `axi_comp_req.ar.addr = addr_t'(trans_resp_data_q.spaddr)`（`:347`），`ar_valid = complete_read`（`:357`）
- `COMPLETE` 状态（`:716–727`）：只有 `aw_ready/is_write` 或 `ar_ready/is_read` 才回到 IDLE。
- `is_write`/`is_read` 取自 `trans_req_data_q.ttype`（`:266–273`）。

**结论**：物理 AW 以**目的翻译完成为前提**；但 IOMMU 在物理 AW 发出时不检查源翻译，因为两者是独立请求。只要原始 AW 先到达（见 F1/G1–G3），**IOMMU 允许目的物理 AW 发出而源翻译尚未完成** —— 这不是 IOMMU 的依赖，它只是不阻止。

### 2.4 W 数据依赖

- W 数据源：`idma_axi_transport_layer.sv:232`（`buffer_in` ← `axi_rsp_i.r.data` 经 barrel shifter）→ `idma_buffer.sv:34–52`（每字节一个 stream FIFO）。
- W 输出门：`:343`（`ready_to_write`）、`:360`（`axi_req_o.w_valid = ready_to_write | write_q`）、`:388`（`w.data = buffer_out`）。
- AW 与 W 在 transport layer 是分离信号：`:289–291`（AW 直连）vs `:356–360`（W 受 buffer 门控）。**transport layer 本身不把 AW 等待 R**；是否等待由更上层 coupler/legalizer 决定。

### 2.5 B 完成

- `idma_axi_transport_layer.sv:447–455`：`w_dp_rsp = b.resp/b.user`，`axi_req_o.b_ready = w_dp_ready_i`。
- `rv_iommu_trans_ctl_wrap.sv:362, 365`：`axi_comp_req.b_ready = tr_axi_req_i.b_ready`，`tr_axi_resp_o = axi_comp_resp`。IOMMU 对 B 只做直通。

### 2.6 问题 1 的小结（保留原问，允许否定）

原问题：**双端 mem-to-mem DMA 是否允许目的 AW 物理发出、而 W 数据依赖尚未完成的源翻译？**

- 在 **IOMMU 侧**：**允许**。物理 AW 只由目的翻译门控（F3），源翻译独立且被排后（F4）。IOMMU 无法看到 W 数据的来源。
- 在 **iDMA 侧**：**未闭合**。是否让原始 AW 早于源读发出，取决于 `RAWCouplingAvail`/`decouple_aw`/coupler 实现（G1–G3）。若 coupler 生效且 `decouple_aw=0`，原始 AW 等第一个对应 R，则物理 AW 也不可能早于“源翻译+R 返回”，环被排除；若 coupler 被 bypass 或 `decouple_aw=1`，则物理 AW 可早于源翻译。
- **该环还缺少两段边**：(a) 一个合法的“更新暂停源翻译”机制（当前控制器无 cancel/暂停，F11；CQ invalidation 只是单拍脉冲，F8）；(b) 该更新的排空范围必须覆盖目的 B。doc §4 自己也说“当前平台没有这里假定的全局撤销锁”，本证据支持这一点。**因此单靠本证据不能把该环当作成立的反例。**

---

## 3. CQ invalidation / IOFENCE 是否真的暂停后续翻译并等物理 W 排空

### 3.1 IOTINVAL（`rv_iommu_cq_handler.sv:310–341`）

- 在 DECODE 单拍置 `iotinval_o.av/gv/pscv/vpn/gscid/pscid`，并置 `inval_vma`/`inval_gvma`（`:312–339`）。
- `state_n = IDLE`（`:299`），命令执行不等待任何在途翻译或物理访问。
- 失效信号只在同一拍发给 TL/IOTLB（`rv_iommu_top.sv:692–693`→`rv_iommu_tl_wrap.sv:537–538, 657–684`）。
- 是否“暂停后续翻译 lookup”取决于 `rv_iommu_iotlb.sv`（未给）的 `inval_i` 实现（research spec 文档引用 `rv_iommu_iotlb.sv:205–303, 306–320`）。**缺口 G4**。

### 3.2 IOFENCE（`rv_iommu_cq_handler.sv:348–395`）

- 注释 `:344–347` 明确 IOFENCE 的语义是“cqh 越过该 IOFENCE 后，保证**所有先前从 CQ 取指的命令已完成并提交**”——对象是 CQ 命令，不是未决 DMA 读写。
- `:362`：`// TODO: Add IOFENCE support with PR and PW using R/W counters` → **PR/PW 排空未实现**。
- `:366–370`：AV=1 时把 4 B DATA 写到 `ADDR`（走 `WRITE` 状态，`:467–495`，AW→W→B）。这是“命令完成通知写内存”，**不是**等 DMA 写排空。
- `:373–376`：WSI 时置 `cqcsr.fence`。
- `:379–392`：仅当 `atsinval_inflight_i` 时等 ATS fence；否则不等待任何东西。

### 3.3 cqh 推进（`rv_iommu_cq_handler.sv:269–295`）

- `REGISTER` 状态中，`r.last` 时即 `cq_head_o = (cq_head_i + 1) & idx_mask`（`:282`）。即取指入 `cmd_q` 时推进 head。
- 这与 `:344–347` 引用的“cqh 越过 IOFENCE 才保证完成”存在需核对的差异：本实现下 cqh 可能在 IOFENCE 的 AV=1 写或其它效果完成前就前进。**需核对** `rv_iommu_sw_if_wrap.sv` / regmap 是否对 `cq_head_wen_o` 另加约束。**缺口 G5**。

### 3.4 是否等物理 W 排空

- **否**。CQ 路径没有连接 iDMA 的未决 AW/W/B，也没有等待 IOMMU completion 端口上的未决写。设备级“quiet”在研究侧是 `rv_iommu_top.sv:281–287` 的 `policy_quiet_o`（组合观察 `in_flight`、`tr_req`、`comp_req`、`ds_req` 等 VALID），它是**研究配置静默条件**，不是规范 IOFENCE 的排空。
- **结论**：把“冲刷请求/ack”当作整个 device quiescence 是**不成立**的。当前 IOFENCE 只做到：命令级串行 + 可选完成写 + ATS fence；**未覆盖未决 DMA 读写排空**。这与用户的前提一致，且被 `:362` 的 `TODO` 直接佐证。
- 注意一个可被误用的“近似”：`w_cnt_q`（`rv_iommu_trans_ctl_wrap.sv:174–180`）只反映已完成翻译并被 demux 接受的 AW，不代表内存侧 W 已落地。

---

## 4. 普通基线资源账本草稿（逐 chunk 两端预翻译再物理发出）

### 4.1 固定量（给定证据可推）

| 量 | 值/来源 | 备注 |
|---|---|---|
| B（数据缓冲） | `dma_core_wrap.sv:27,312`；doc §v2：DMA0/1 = 512 拍 × 8 B = 4096 B/路；DMA2/3 = 3×8 = 24 B | `MaskInvalidData=1'b0`（`:315`），`HardwareLegalizer=1'b1`（`:316`） |
| `NumAxInFlight` | `dma_core_wrap.sv:311`；doc §2 述顶层为 8 | 实际值在 `ariane_peripherals_xilinx.sv`（未给） |
| `MemSysDepth` | `dma_core_wrap.sv:318`；doc 述为 0 | MetaFifoDepth = BufferDepth+NumAxInFlight+MemSysDepth（`idma_backend.sv:112`） |
| legalizer chunk 粒度 | `idma_legalizer.sv:82–83, 143–148, 173–178, 201–202, 209–214` | StrbWidth=8 → OffsetWidth=3；默认 `reduce_len=0` ⇒ `r_page_addr_width=3+8=11` ⇒ **chunk ≤2048 B**（非 4096）。4 KiB job 在 legalizer 层至少切 2 段。**需按真实 `DataWidth`/`src_reduce_len` 复核** |
| 主 PTW 并发 | `rv_iommu_tl_wrap.sv:758`（单 `rv_iommu_ptw`） | 并发=1 |
| IOTLB 容量 | `rv_iommu_top.sv`/`rv_iommu_tl_wrap.sv:137–138`（`NumIotlbEntries==16` 约束） | 16 项 |
| 翻译在途（K） | `rv_iommu_trans_ctl_wrap.sv:105`（`state_q != IDLE` 单笔） | 现状 K=1；banked 版在未给文件 |

### 4.2 逐 chunk 两端预翻译的资源类别（基线）

在“每 chunk 先完成源、目的翻译，再发物理 AW/AR”的基线下，需要按下列类别记账（同等容量/仲裁/替换规则下比较）：

1. **配对槽（K）**：保存一对 `<src IOVA→SPA, dst IOVA→SPA>` 在“已翻译、待物理发出”状态。
2. **预翻译预留（可取消）**：目的/源翻译结果被持有但在失效时可取消，且**已合法取得的结果可以保留为有效缓存项**。
3. **数据缓冲 B**：固定不变。
4. **PTW 并发**：固定=1（若基线不改 PTW）。
5. **IOTLB 条目占用**：记账为“占用的 entry 数”，**不等于“锁 way”**。
6. **握手/正确性约束**：同 ID 顺序、AW/W 关联、失效代语义。

### 4.3 现有接口是否有 src/dst 关联？

**没有。** 证据：

- `trans_req_data_t` 字段只有 `iova, did, pid_valid, pid, ttype, priv, is_debug, bank`（`rv_iommu_top.sv:136–145`），无 tag/配对 ID。
- `trans_resp_data_t`（`:148–165`）也无配对字段。
- iDMA 侧：AR 与 AW 是两条独立 AXI 通道（`idma_backend.sv:378–420`），`idma_legalizer.sv` 用独立的 `r_tf_q`/`w_tf_q`（`:98–100`），二者共享 `axi_id`（`:327, 349`）。同一 copy 的同一 chunk 的 AR/AW 有相同 `axi_id`，但**同 ID 可被同一源重复使用**，无法作为唯一配对键。
- 已有的 `FullBufferDID`/`fullbuffer_job_i` 机制（`rv_iommu_trans_ctl_wrap.sv:58, 63, 646–647`；`dma_core_wrap.sv:173–184` 的 `idma_fullbuffer_certificate`；`rv_iommu_top.sv:82`）关联的是**一个 DID**（“整 job 可全缓冲”），不是 src/dst 页对。

**结论（基线所需的新增信息）**：
- 需要一个稳定、可判别到“同一 chunk 的源与目的翻译结果”的关联（显式 tag，或可证明唯一的 ID+顺序规则）。
- 需要为“已翻译未发出”的配对定义预留/取消状态（当前 `rv_iommu_trans_ctl_wrap.sv:96–102` 只有 IDLE/REQ_TRANS/AWAIT_RESP/MRIF/COMPLETE，无取消）。
- 需要确保取消时**不丢弃已合法取得的结果**，而是转成有效 IOTLB 条目（这正对应“两个 PTE ≠ 必须锁两个 TLB way”）。

### 4.4 不得使用的做法（按用户约束）

- 不把“全 job 预锁”当唯一基线；基线是逐 chunk 两端预翻译再发出。
- 不把已存在的 AW 优先当新点（`rv_iommu_trans_ctl_wrap.sv:623–624, 628`）。
- 不把两个 PTE 的预翻译等价于“必须锁两个 TLB way”；有效结果可保留。
- 不发明可测数据（本轮无实测/仿真结果）。

---

## 5. 未知与证据缺口（必要未给文件）

| 缺口 | 需要文件 | 为什么闭不上 |
|---|---|---|
| G1 | `cva6/corev_apu/iDMA/src/idma_channel_coupler.sv` | R-AW 耦合实际行为；`decouple_aw` 是否让 AW 等第一个 R |
| G2 | `cva6/corev_apu/iDMA/src/idma_reg64_frontend.sv` | batch 任务下 `decouple_aw`/`decouple_rw`/`ttype` 真实值；ADMIT/allow |
| G3 | `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv` | `RAW_COUPLING_AVAIL`、`BUFFER_DEPTH`、`IS_TWO_D`、`ResearchPort`、`fullbuffer_jobs` 的真实绑定 |
| G4 | `.../translation_logic/rv_iommu_iotlb.sv` | `inval_i` 是否阻塞后续 lookup / 是否即时；`research_o` 语义 |
| G5 | `.../software_interface/rv_iommu_sw_if_wrap.sv`（及 regmap） | cqh 推进与 IOFENCE 完成语义的真实耦合 |
| G6 | `.../transaction_controller/rv_iommu_banked_ctl.sv` | banked/nonblock 控制器下的在途数与 AW/AR 交互 |
| G7 | `cva6/corev_apu/iDMA/src/idma_stream_fifo.sv` | flush/深度语义（与 `MetaFifoDepth` 关联） |
| G8 | `cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv` | `permit_o` 与 `CapacityBytes` 判定口径 |
| G9 | 内存互连/`rv_iommu_ds_if.sv` + `axi_demux` 配置 | 确认 IOMMU completion 端口到 DRAM 的路径与 `SpillAw/W` 行为 |
| G10 | `rv_iommu_nonblock_hit.sv`（本轮未给，doc 部分引用） | 快速路径与失效的关系（若要把 K>1 与失效语义合看） |

---

## 6. 可证伪条件（保留原问，允许否定）

**原命题（目的物理 AW 已发出、W 依赖未完成源翻译）：**

- **可否证条件 A**：若 SoC 实际绑定 `RAW_COUPLING_AVAIL=1` 且 frontend 下 `decouple_aw=0`，并且 `idma_channel_coupler` 确实把原始 AW 扣到“同一 chunk 的第一个 R”之后，则原始 AW 不可能在源翻译+R 返回前到达 IOMMU ⇒ 物理 AW 也不可能早于源翻译 ⇒ **命题在该路径下被否定**。
- **可否证条件 B**：若不存在合法更新事件能“暂停源翻译”，或该更新排空范围不覆盖目的 B（例如 IOTINVAL 只是单拍脉冲、IOFENCE 无 PR/PW 排空，见 F8/F9/F11），则环缺少两条边 ⇒ **命题不成立**。
- **可否证条件 C**：若源与目的落在同一 2048 B legalizer chunk 内且 legalizer 默认 `decouple_rw=0`（`idma_legalizer.sv:452–466`），则 R/W 同拍推进，AW 不可能在 R 之前单独发出 ⇒ 该 chunk 内**命题被否定**。
- **成立条件 D**（若出现，则为可继续研究的形态）：`RAWCouplingAvail=0`（或 `decouple_aw=1`）**且** `decouple_rw=1` **且** 存在一个合法的、覆盖源翻译暂停与目的 B 排空的更新事件。三条同时成立才具备环。

**CQ invalidation/IOFENCE 命题：**

- **可否证条件 E**：若 `rv_iommu_iotlb.sv` 的 `inval_i` 被证明会在接受失效时阻塞所有后续 lookup 直至完成，且该阻塞覆盖在途翻译，则“inv 不暂停后续翻译”可被否定（但当前 CQ 单拍脉冲仍不等物理 W）。
- **已可确认**：无论 G4 如何，CQ IOFENCE 的 `TODO`（`:362`）表明**未实现 PR/PW 排空**；因此“IOFENCE = 等物理 W 排空”不成立（除非有本文件外的补偿机制，需 G5 证伪）。

**基线命题：**

- **可否证条件 F**：若现有接口已经能把同一 chunk 的 src/dst 翻译结果一一配对（例如通过 ID+顺序在未给文件中保证唯一），则“缺 src/dst 关联”被否定。基于给定文件（F12）目前不成立。
- **可否证条件 G**：若逐 chunk 成对预翻译在相同 B/K 下没有额外代价（`min(src 剩余, dst 剩余, 剩余长度)` 已经在 legalizer 内切好，见 `idma_legalizer.sv:201–214`），则“需要新机制”的主张不成立。这属于待测，本轮不判定。

---

## 7. 下一文件（按优先级）

1. `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`（genDMA：`RAW_COUPLING_AVAIL`、`BUFFER_DEPTH`、`IS_TWO_D`、`ResearchPort`、`fullbuffer_jobs_i`）
2. `cva6/corev_apu/iDMA/src/idma_channel_coupler.sv`
3. `cva6/corev_apu/iDMA/src/idma_reg64_frontend.sv`（batch/option/ADMIT）
4. `.../translation_logic/rv_iommu_iotlb.sv`（`inval_i`、`research_o`）
5. `.../software_interface/rv_iommu_sw_if_wrap.sv` + regmap（cqh 语义）
6. `.../transaction_controller/rv_iommu_banked_ctl.sv`
7. `cva6/corev_apu/iDMA/src/idma_stream_fifo.sv`、`idma_fullbuffer_certificate.sv`

---

## 8. 假设与边界（供 Codex 裁定）

- 本稿只引用给定文件与行号；未运行任何命令、仿真或综合，未声明任何测试通过。
- `idma_legalizer` chunk = 2048 B 是基于 `DataWidth=64`、`src_reduce_len=0` 的推导；若 SoC 实际 `DataWidth` 不同（`idma_backend.sv:870–872` 允许 16/32/64/128/256/512/1028），chunk 大小需重算。
- `RAWCouplingAvail` 在 `dma_core_wrap.sv:28` 默认 0、`idma_backend.sv:34` 默认 1，**两个默认互相矛盾**，必须以 `ariane_peripherals_xilinx.sv` 实际绑定为准，不能任选其一。
- 关于 cqh 在取指时推进（`rv_iommu_cq_handler.sv:282`）与引用规范语义的差异，本稿只报告观察，不作 bug 判定，需 G5 复核。
- 架构/创新裁定、是否继续该题、是否需要全局撤销锁语义，超出本提取范围，留给 Codex。
