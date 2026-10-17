# PN SW/control quiet：只读提案

## Material Passport

- ARS experiment-agent，已有 PN 有界合同下的源码核查/finite 验证计划；不启动全 pipeline。
- 状态：**proposal_only；生产修改 0，编译/RTL 执行 0，Vivado 0，板点 0**。
- 顶层独立 worktree `/home/xvhaocheng/work-e6-pn-sw-quiet`，branch `ispass/e6-pn-sw-quiet`，从 `main` 的 `225c9a8087ba590355f01cc06d20584ec54d9f33` 建立。
- 实际 IOMMU 独立 worktree `/home/xvhaocheng/work-e6-pn-sw-quiet-iommu`，同名 branch，固定 `5b7dfbc70e458244a8e5682e5ae3f8571320254c`，当前 clean；未切换或修改 E6 主树。
- 直接核对九份实际源码，全部与固定 commit **及原版 `85b157ebe71a623e326e51fff7b7126fe77e8eb9` 逐字节相同**。原字节、Git blob、SHA256、长度和来源在 [source-map.json](source-map.json) 与 [source/](source/)。来源不是现有 32-blob 摘要的转抄。
- Codex-only；请求模型/强度为 gpt-6.1-sol / ultra，未改模型元数据、上下文或缓存。本包不声称独立验证运行时模型元数据。

## 1. 本批接口与修改边界

只提九文件的末尾参数 `parameter bit PnEnable = 1'b0`、末尾只读端口 `output logic pn_idle_o`，以及已有模块实例间的必要参数/端口传递。原端口、参数的顺序保持；不增加 clock/reset、状态、计数器、FIFO、flush 或 runtime mode 输入。

| 文件（均在实际 IOMMU 内） | 末尾参数位置 / 输出位置 | 必要传递 |
| --- | --- | --- |
| `rtl/software_interface/rv_iommu_cq_handler.sv` | 原 `axi_resp_t` 后 / `mem_req_o` 后 | 无 |
| `rtl/software_interface/rv_iommu_fault_arbiter.sv` | 原 `fault_data_t` 后 / `fq_ready_i` 后 | 无 |
| `rtl/software_interface/rv_iommu_fq_handler.sv` | 原 `axi_resp_t` 后 / `mem_req_o` 后 | 无 |
| `rtl/software_interface/rv_iommu_pq_handler.sv` | 原 `axi_resp_t` 后 / `mem_req_o` 后 | 无 |
| `rtl/software_interface/rv_iommu_msi_ig.sv` | 原 `axi_resp_t` 后 / `mem_req_o` 后 | 无 |
| `rtl/software_interface/rv_iommu_dbg_if_ctl.sv` | 原 `dbg_resp_t` 后 / `resp_ready_o` 后 | 无 |
| `rtl/software_interface/rv_iommu_sw_if_wrap.sv` | 原 `iommu_reg_rsp_t` 后 / `wsi_wires_o` 后 | 上述六个现有子模块；只接独立 local idle wires |
| `vendor/modified/rv_iommu_axi2apb_64_32.sv` | 原 `APB_ADDR_WIDTH` 后 / `PSLVERR` 后 | 无；复用既有 AXI buffer output-valid |
| `rtl/ext_interfaces/rv_iommu_prog_if.sv` | 原 `iommu_reg_rsp_t` 后 / `regmap_resp_i` 后 | 现有 axi2apb 实例 |

所有输出统一拟为：`!PnEnable` 时恒 0；启用后 `rst_ni && 本模块事实谓词`。reset 中不提供 idle 资格。generate 只新增组合旁听逻辑，不能修改任何旧赋值、FSM 或实例的原功能参数。`PnEnable=0` 的新增比较/归约逻辑须由常量分支消除，原模块仍原样存在；这不构成 Vivado 资源证明。

**本批不改** `rv_iommu_top`、Bender、TC/vendor demux/TL/DS/MRIF、前端、observer、外围、IOMMU CSR/cap、Linux、RTL 约束。A1 独占 DS/TC/vendor/TL；A3 独占 frontend/observer tap。不得把新 idle 倒接旧 `in_flight_i` 或 DDTP 写保护。MODE/clear/外层完整 allquiet 由 Root 后续整合。

SW 与 program 是两个独立出口：`sw_if_wrap.pn_idle_o` **不包含**程序 AXI bridge 的内部缓冲；`prog_if.pn_idle_o` 单独反映它。未来 top 需要同时 AND 两者，不能因 SW 当前 `regmap_req_i.valid=0` 就省掉 program quiet。

## 2. 精确拟用事实（尚未写入生产源码）

下列 `mem_quiet` 是文档缩写，不新增状态/公共类型：

```systemverilog
!mem_req_o.ar_valid && !mem_req_o.aw_valid && !mem_req_o.w_valid
&& !mem_resp_i.r_valid && !mem_resp_i.b_valid
```

READY 为 1 不表示有债务；所有 current VALID 无论 ready 值如何都阻止 quiet。只读取既有 `_q`、现有输入/输出，不使用 `_n`、MODE intent 或由 MODE 产生的 clear。

### CQ

在 [CQ 原字节](source/rtl/software_interface/rv_iommu_cq_handler.sv) 中：enable 稳定只由 `:133–143` 的 `cq_en_q/cqcsr_en_i` 表示；索引 mask 是 `:129–130`；`:282–283` 已推进 head 并进入 DECODE，head==tail 不表示命令完成。`:483–490` 收到 B 后仅主状态回 IDLE，旧 `wr_state_q` 可继续是 B_RESP。

拟用：

```systemverilog
(state_q == IDLE)
&& (cqcsr_en_i == cq_en_q)
&& ((cq_tail_i & idx_mask) == cq_head_i)
&& mem_quiet
&& !iotinval_o.inval_vma && !iotinval_o.inval_gvma
&& !iodirinval_o.inval_ddtc && !iodirinval_o.inval_pdtc
&& !atsinval_valid_o && !atsprgr_valid_o && !ats_fence_valid_o
&& !atsinval_inflight_i
```

四个 inval 有效位来自 `:334/338/427/428/432`；不能把 `av/gv/dv/did/pid` 等 payload 当 VALID。`atsinval_inflight_i` 是既有外部债务事实（`:379–390` 的 fence 等待），拟保守纳入；本目标 InclATS=0 原样 tie-off。**不要求** `wr_state_q==AW_REQ`、`cmd_q==0` 或 sticky fence IRQ0。ERROR 到原 CSR 清错前保守 nonquiet。tail 比较沿原 FSM 的 masked-tail 与当前 head，不另发明环指针规则。

### Fault arbiter

[原字节](source/rtl/software_interface/rv_iommu_fault_arbiter.sv) `:44–63` 每输入 spill、`:66–83` 仲裁 valid、`:87–107` 末 FIFO；`:112` 把 FIFO valid 交 FQ。

```systemverilog
!(|fault_valid_i) && !(|fault_valid_q)
&& !arbited_valid_d && !arbited_valid_q && !fq_valid_o
```

复用真实有效位，不加 shadow 计数。上游已撤新 fault 时仍要检查全部 spill 和 FIFO。`stream_fifo.valid_o=~empty` 的真实依赖也已核读；原 head payload 非零不表示非空。

### FQ / PQ

[FQ](source/rtl/software_interface/rv_iommu_fq_handler.sv) `:80–113,129–131,249–279` 与 [PQ](source/rtl/software_interface/rv_iommu_pq_handler.sv) `:84–118,134–138,255–283`：

```systemverilog
// FQ
(state_q == IDLE) && (fqcsr_en_i == fq_en_q)
&& !fq_valid_i && mem_quiet
// PQ
(state_q == IDLE) && (pqcsr_en_i == pq_en_q)
&& !pq_valid_i && mem_quiet
```

主 WRITE 包含 AW/W/等待 B 的整个生命周期；ERROR 保守 nonquiet，READY 可能仍为 1 不能推 idle。**不要求 head==tail**：FQ `:117` / PQ `:122` 可因已完成但未被软件读取的内存记录维持 IRQ，这不是未返 AXI 债务。enable transition 必须等原 q 稳定；record 寄存器、旧计数、IRQ/HPM 都不另判忙。

### MSI generator

[原字节](source/rtl/software_interface/rv_iommu_msi_ig.sv) `:65–89` 的 main state、pending、edged 和 `:141–174` 的 pending/新边沿分支：

```systemverilog
(state_q == IDLE) && !(|pending_q)
&& !(msi_ig_en_i && (|(intp_i & ~edged_q)))
&& mem_quiet && !fault_valid_o
```

masked pending 必须阻止 quiet，包含 `msi_ig_en_i=0` 时仍未退休的 pending。已经 edged 的 sticky IRQ 高可 quiet；不能要求 `intp_i==0` 或 `edged_q==0`。`:190–211` 直到 B 才退出 WRITE；`:218–223` 错误 fault 未被真实 FQ 接收前也不 quiet。MSI ERROR 转入 fault arbiter 后必须由后者/FQ 接续非空事实，不能只看此 MSI FSM。

### Debug

[原字节](source/rtl/software_interface/rv_iommu_dbg_if_ctl.sv) `:47–51,93–104`：

```systemverilog
(state_q == IDLE) && !dbg_ctl_i.go.q
&& !req_valid_o && !resp_valid_i
```

GO1 但 TL 尚不 ready 时 FSM 仍 IDLE，必须明确挡住。ONGOING 等真实 response，`resp_ready_o=1` 的默认值 (`:84`) 不代表 busy。GO 清除保持原 regmap `:2337–2357` 的硬件 busy 更新，不能新增 quiet/reset 清 GO。`req_data_o.is_debug=1` (`:63`) 不动。

### AXI→APB bridge / program IF

[桥原字节](source/vendor/modified/rv_iommu_axi2apb_64_32.sv) `:172–178` 为 CS；`:213–352` 是真实五通道 FIFO，内部请求输出 `AWVALID/WVALID/ARVALID`，内部响应输入 `RVALID/BVALID`，外部响应输出 `RVALID_o/BVALID_o`。

```systemverilog
(CS == IDLE) && !PENABLE
&& !AWVALID_i && !WVALID_i && !ARVALID_i
&& !AWVALID   && !WVALID   && !ARVALID
&& !RVALID    && !BVALID
&& !RVALID_o  && !BVALID_o
```

这覆盖当前 incoming offer、已收但未执行的 AW/W/AR、已产生但仍在返回 FIFO 的 R/B、APB 当前事务。`axi_single_slice.valid_o=~empty` 见实际 vendor `axi_single_slice.sv:28–48`，不增加或修改其端口。**PSEL 恒 1** (`:211`)，不能判忙；旧 `AWLEN_Q/ARLEN_Q/data` 不判忙。`:448–512` 的 IDLE 可有 AW-only 或单独 buffered W，不能只查 CS；`:534–542` 的内部 B 可已入 FIFO、CS 回 IDLE，外部 B 仍 held，必须查两侧响应 VALID。

[program 原字节](source/rtl/ext_interfaces/rv_iommu_prog_if.sv) 活跃模块仅 `:25–140`；`:141` 以后是整块注释中的历史替代模块，不动。拟用 `bridge_idle && !regmap_req_o.valid`，再显式合并 `!prog_req_i.aw_valid/w_valid/ar_valid` 和 `!prog_resp_o.r_valid/b_valid`（对桥事实的边界重申）。`regmap_req_o.valid=psel & penable` (`:135`)，不查 psel。程序总线新 tail/GO 写即使尚未更新 CSR，也已由 current/buffered AXI/bridge/APB 阻止 quiet。

### SW 汇总

[原字节](source/rtl/software_interface/rv_iommu_sw_if_wrap.sv) `:171–406,476–519,548–583` 既有实例接线不变。新增 local wires 对 CQ、fault arbiter、FQ、PQ、MSI、debug 六个 child idle 取 AND，再合并：

```systemverilog
!regmap_req_i.valid
&& !(|fault_valid) && !fq_valid && !pq_valid_i
&& !dbg_req_valid_o && !dbg_resp_valid_i
&& !atsinval_valid_o && !atsprgr_valid_o && !ats_fence_valid_o
&& !atsinval_inflight_i
&& !iotinval_o.inval_vma && !iotinval_o.inval_gvma
&& !iodirinval_o.inval_ddtc && !iodirinval_o.inval_pdtc
&& four_SW_memory_interfaces_have_no_AR_AW_W_R_B_VALID
```

`fault_valid` 采用现有配置对应真实输入向量 (`:243–285`)，不凭未实例化 feature 的虚拟输入判忙。MSI 或 debug 因原配置缺省不生成时，相应 local child idle 取恒 1 作为 AND 的单位元，不新增虚假模块；整个 SW `PnEnable=0` 仍输出 0。主目标保持 BOTH/MSI_BT_MRIF/PC/debug/ATS0，不为简化测试关 feature。

不要求 `in_flight_i==0`（它仍供原写保护，TL/TC quiet 另由 Root 汇总）、WSI 线0、IRQ寄存器0、HPM计数0。HPM/WSI/寄存器的原输出和更新逐位 gold。只读 INDEX/DATA 或 DMA0 MODE 访问不经此 IOMMU program 口，不能统一拿外围 cfg VALID 当本域 busy。

## 3. 真实 finite DUT / 原 SW gold 计划（全部尚未执行）

拟两次编译：`PnEnable=0` 和 `PnEnable=1`，同一完整 test top 各包含 candidate 的 **真实 program IF + 真实桥五通道 buffer + 完整 SW wrapper + 真 regmap/HPM/WSI + 全部真实 child**，以及原 85b 独立 gold。九份 fixed5b7 与 85b 字节相同已核实。gold 只做 module namespace 和相应子实例 binding 重命名；不能让两侧共用已修改 child、用自写 FSM 替代 SW，或只对新 idle 表达式做镜像测试。原 regmap/HPM/WSI/vendor FIFO 每侧分别实例化，状态独立。

原所有输出在每个 pre/post edge **完整逐位**比较（包含无效周期 payload、AXI/CSR/IRQ/HPM/debug/inval/ATS）。PnEnable0 新出口恒0；Enabled 的新出口按真实状态/有效位和可观察未退休事务交叉验收。有限 environment 是 AXI program master、四个 DS memory responder、debug translation responder、fault producer，均保持 stalled VALID/payload，严格核地址/长度/拍数/WLAST/ID/返回/内存 guard。不用 force/FSM后门、手动塞 CSR、假 FQ/假 debug 完成。只读 test tap 可读取真实内部状态作为证据，生产不加层级引用。

目标 RVIOMMUCfg 使用 Root 固定实际目标：NumOutstanding8/WFifo32/MrifFifo8/FaultFifo8，MSI_BT_MRIF、IGS BOTH、InclPC1/InclDbg1/InclATS0、NumIntVec16、NumHpmCounters31、实际 AXI64/地址/ID/user 宽度；从固定 top config 与类型输入冻结，不按 `NullCfg` 编译后冒称目标。额外 WSI_ONLY / InclDbg0 是静态分支 smoke，只能单列自身配置，不能替代主目标。

| 必要有限情境 | 真输入与应观察的边界 |
| --- | --- |
| 稳态/复位/disabled | Pn0 与 Pn1 原输出双 gold；未访问时 PSEL1 仍可 idle；reset 新出口0；非零旧 payload/计数不阻止静态 idle |
| Program inventory | AW-only、W-before-AW、同拍 AW/W、buffer 深度2、AR 优先下的 buffered write、R/B backpressure；外部已撤 offer 但 buffer 未空必须 nonquiet |
| APB phase | 32bit/64→32 两半实际 CSR 读写；真实 program 单独配置 tail/GO。桥 standalone 再延迟 PREADY 测 WAIT_*，不宣称真实 regmap 本来会产生该 wait |
| CQ head==tail | 真实内存16B command fetch，两 R拍后 head已追平但 DECODE/invalidation 仍 nonquiet；queued tail 与 enable transition；mask/环回绕 |
| CQ write/B | 合法 IOFENCE.C 真实 AW/W，B延迟时 busy；B后主IDLE且陈旧wr_state=B_RESP 时可 quiet。原 fence sticky IRQ不误挡 |
| CQ ERROR/ATS边界 | 合法 error response或非法 command经真实CSR报错与清错恢复；ATS0原tie-off逐位保留；可单列 CQ unit held ATS/fence输入事实，不泛称完整 ATS1 |
| Fault库存 | 三输入合法 fault并发，经真实spill/arbiter/FIFO入FQ；持住 FQ memory B/填足backpressure，让新输入撤掉后仅内部spill/FIFO仍阻 quiet，最终真实排空 |
| FQ记录 | 实际32B四W拍+延迟B，tail更新后故意不读内存ring；无债务时quiet恢复，IRQ可保持高；记录full/ERROR按旧CSR清错，不由quiet清错 |
| MSI | 实际IRQ→配置向量AW/W/B；sticky已edged只发一次而quiet恢复；masked pending保持nonquiet，unmask自然退休；B错误经真实MSI fault→arbiter→FQ继续写，债务不能在交接时丢失 |
| Debug | 真 program GO写→req held→TL接受→ONGOING无valid空窗→真实response→原GO清除；每一阶段阻quiet，结束自然恢复；debug feature与字段不动 |
| PQ / ATS0 | 主目标保持原PQ输入tie-off/CSR能力，核启用/空闲/当前offer的local事实；主动PQ写的已知原源码边界单列，见§4，不以非法内存模型制造成功 |
| feature省略分支 | WSI_ONLY/InclDbg0 的 enabled 汇总单位元正确，原输出和原能力逐位gold，不在主目标关MSI/debug |

未来 runner 拟限定每个 compile ≤900秒、每个执行 ≤60秒、每情境 ≤20000 cycles、总 ≤200000 cycles，超时/错误保留原输出并停止。使用新输出 `sw-quiet/runs/runNN`、exist_ok=false；记录实际命令、工具版本、源码/依赖/Git状态/原 gold转换/SHA，原失败不覆写。这里只是预算及接口计划，**runner 尚不存在、没有可声称已执行的命令或通过数**。

依赖沿实际 IOMMU Bender 只读列出的 packages/regmap/原 SW、common_cells 的 spill/stream_fifo/stream_arbiter、axi_slice 五通道 buffer/axi_single_slice、modified/fifo；不编译 TC/TL/DS/整 IOMMU top。当前独立组件的 submodule 工作树未展开，不在他席树初始化/修改；未来从已存在固定依赖归档复制实际文件并锁 SHA：AXI e72230ef、common_cells f36828f、axi_slice bc864aae、register_interface 3a7b9e28，完整 gitlink 在 source-map。Bender 的远端声明rev与实际固定gitlink不同，**以实际使用文件/Git SHA 为准**，不自动下载最新依赖。

现有可借鉴 runner 只有执行/快照格式：Root `E6-pn/ds-validation/run_ds.py` 与 `n-validation/run_n_tl.py`。不能把它们的 DS/TL 通过结果算作新 SW 验证；采用已存在 Verilator5.040 与原依赖归档，不安装/升级系统工具。本批未启动这些 runner。

## 4. 源级未决边界与接受范围

**PQ 主动记录的原协议疑点（source-only，未运行）**：`rv_iommu_pq_handler.sv:150` 是 AWLEN3（64bit时四拍），但 `:238–249` 的 1bit `wr_cnt_q` 在第二 W 拍置 WLAST并进入 B_RESP。九文件原85b与5b7均同字节；另外 `:81–82` 与 `:121–122` 对 ipsr 两输出存在原重复连续赋值。不得为 quiet 提案修原状态/长度/IRQ，也不得让严格 AXI responder 提前给B后宣称合法PQ全链过。

拟主目标有限接受域仍严格是 ATS0（原 tie-off），PQ quiet 的组合tap保留其真实状态/enable/offer事实。若 Root 要求额外主动 PQ 的自然 B退休全链，先由 Root锁定独立原问题处理/范围；可以原同源 gold 负probe保存失败，但不在当前只读阶段执行。待实际 compile 若原重复驱动构成诊断，也原样保留，不以新RTL改旧功能“让测试能过”。本提案没有声称已证明 PQ AXI 功能故障或修复。

CQ旧 fence PR/PW TODO、MSI masked pending 永不由软件解除、ERROR需原CSR清错都是现存行为；保守 nonquiet 不保证软件会自动前进。SW quiet 不覆盖任何 TC/TL/PTW/MRIF/DS 已接受未返回库存；program quiet不覆盖DMA0 cfg；新出口并不构成完整 MODE eligible、observer allquiet、PN/S15、FPGA fit/timing 或论文板测证据。

Root 接受本端口/谓词及上述 finite 范围后，再分小批实施九文件；实际生产 diff 和真实执行证据随后单独交审。当前组件保持 clean5b7，不创建虚假组件变更commit/bundle，也不推上游远端。
