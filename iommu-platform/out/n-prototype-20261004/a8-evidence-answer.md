# 交付物：证据梳理 + 可执行复现测试提案（不含 RTL 改根因、不宣称执行）

## 0. 边界与摘要（先读）

- 我**没有**执行任何命令；下面所有代码都是 **proposal**，未编译、未运行、未在硬件上验证。凡涉及"通过/失败"的陈述都明确标注为*你提供的 raw 证据*或*推理*。
- 我**没有**提出 RTL 根因修复。`gen_dma` / `gen_iommu` 作用域接线 bug 只做证据定位（§3.1），修复归 Codex。
- 我**没有**改动任何已采样 raw，**没有**重新统计旧数据。全文只解码了 1 个 helper 窗口 frame（§6.3），因为拟议 guard 需要它的字段布局；其余旧数据未重算。
- A8 停滞：提供的证据里只有时间线（`serial-a8-live-evidence.log:34-38`），**没有** A8 的 `record.json` / helper 窗口 / `STOPPED.txt`，因此**不能**宣称 A8 通过或失败于 IOMMU；本文只把它作为"必须由 Codex 定根的开放问题"列出候选域（§3.4）。
- 顺序约束（与你的要求一致）：**先修接线 bug → 重新编译两个 BIT → 才谈新 BIT**；本文的仿真测试可在此之前于现有 joint TB 上跑。Hit 相关（`banked-hit`）不跑。

---

## 1. 证据 A：bank / tag / AXI_ID 的实际宽度与转换（file:line）

### 1.1 板级（`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`）

| 事实 | 证据 |
|---|---|
| TR / completion / DS 的 ID 宽度 = `ariane_soc::IdWidth` | `:867`（`iommu_id_t`）、`:876-882`（`AXI_TYPEDEF_EXT_ALL(axi_tr,…)`、`AXI_TYPEDEF_ALL(axi_comp,…)`） |
| 编程 IF / dma_cfg 从端口 ID 宽度 = `ariane_soc::IdWidthSlave` | `:868`、`:904-907` |
| 每 DMA 设备端口 ID 宽度 `DmaAxiIdWidth = IdWidth - $clog2(4) = IdWidth-2` | `:870-872`、`:896-899` |
| DMA→IOMMU 汇聚 `axi_mux`：master ID = `DmaAxiIdWidth + 2`（高位=端口号） | `:1161-1186`（`SlvAxiIDWidth(DmaAxiIdWidth)`，mst 用 `axi_tr_*` 类型） |
| 每个 iDMA 固定发 `AxID = '0` | `:1207`（`.AxID('0)`），注释 `:947` |
| **bank 选择位 = `BankIdBit = DmaAxiIdWidth = IdWidth-2`**，即"端口号最低位"，不是 device/stream id | `:1331` + `banked_ctl:241-242` |
| 每 DMA 的 `DEVICE_ID = 24'(10+i)`、`PROCESS_ID = 20'(20+i)` | `:1205-1206` |
| banked 配置强制 `MSITrans = MSI_BT_ONLY`，禁用 legacy E3/COST 观察者 | `:1317-1326` |

**推论（已证实）**：因为每个 DMA 的 `AxID=0`，`aw/ar.id[BankIdBit]` 只由 `axi_mux` 前插的端口号最低位决定 ⇒ DMA0/2→bank0，DMA1/3→bank1。所以 V(did10, DMA0)=bank0、A(did11, DMA1)=bank1（与 `scripts/mac_nonblock_pilot.py:147-152` 的 `Solo/A1/A8/A16` = aggressor 1 + 1/8/16 pages 一致）。同时**ID 天然按 bank 唯一**（bank 位本身是 ID 的一部分），这是 `axi_demux` 的 B/R 按 ID 路由能工作的前提。

**缺失依赖（未证实，需 Codex 确认）**：`ariane_soc::IdWidth` / `IdWidthSlave` 的**具体值**不在提供文件里（`ariane_soc` 包未提供）。这直接决定 `BankIdBit` 的数值。下面 §4 的 joint TB 用 `AxiIdWidth=4, BankIdBit=2`（`tests/nonblock_banked_top.sv:155-158,338,578`），只有 `IdWidth==4` 时才与板级等价 ⇒ **请确认 `IdWidth`**；若为 6，则 `BankIdBit=4`，TB 的 4-bit 模型不等价（需按 §5 的最小新 harness 或改 TB 常量）。

### 1.2 top（`rv_iommu_top.sv`）

- 生命周期 tag `bank` 是 **1 bit**，放在 `trans_req_data_t.bank`（`:138`）与 `trans_resp_data_t.bank`（`:143`）；`trans_accept_o/trans_did_o/trans_type_o` 直接从 `trans_req_data` 观察（`:184-186`）。
- top 自己做**零次** ID 转换：把板级 `axi_tr_*` / `axi_comp_*` / `axi_ds_*` 类型原样传给 `rv_iommu_banked_ctl`（`:357-374`）。唯一 ID 操作在 `banked_ctl` 内部（见 1.3）。
- `BankIdBit` 只做参数转发（`:349`）；宽度合法性检查在 `banked_ctl:137-140`。
- DSL 侧：**9 个请求者共用一个物理 master 口** `rv_iommu_ds_if.ds_req_o`（`:247-264` 声明，`:303-340` 全部接到同一实例的 `.ds_req_o(ds_req_o)` / `.ds_resp_i(ds_resp_i)`）⇒ DDTW、PDTW、PTW、MSIPTW、MRIF、CQ、FQ、PQ、MSI-IG 在真实设计里共享同一 AXI 存储端口（仲裁在 `rv_iommu_ds_if` 内部，该文件不在证据里）。板级再串一个 `axi_delayer` 到 XBAR（`ariane_peripherals_xilinx.sv:1284-1306`）。

### 1.3 `banked_ctl`（`rv_iommu_banked_ctl.sv`）

- 每 bank 配置：`NumOutstandingTrans/2`、`WFifoDepth/2`，其余不变（`:160-162`）；初始断言要求 `AxiIdWidth>=1`、`BankIdBit<AxiIdWidth`、`NumOutstandingTrans` 偶且 ≥2、`WFifoDepth` 偶且 ≥2、MRIF 禁用、ATS 禁用（`:133-153`）。
- 输入 bank 选择：`aw_sel = tr_axi_req_i.aw.id[BankIdBit]`、`ar_sel = ...ar.id[BankIdBit]`（`:241-242`），`axi_demux(NoMstPorts=2, MaxTrans=NumOutstandingTrans*2, UniqueIds=0, Spill*=0)`（`:244-273`）。
- 出向完成 mux 的**内部 ID 加宽 1 bit**：`comp_id_wide_t = logic [AxiIdWidth:0]`（`:199`），mux 前插 bank 位为 MSB（`:378-409`）。
- **转换细节（已证实）**：AW/AR 去掉冗余 MSB：`aw.id = mux_mst_req.aw.id[AxiIdWidth-1:0]`（`:421`）、`ar.id`（`:443`）；B/R 用**原始** `id[BankIdBit]` 重建 MSB：`{comp_axi_resp_i.b.id[BankIdBit], comp_axi_resp_i.b.id}`（`:471`）、r（`:476`）。其余字段逐字段搬运，无结构体位截断（`:417-456`, `:463-481`）。
- **明确写死的假设**：外部完成从设备必须**原样回显 AW/AR ID**（`:25-31`）。板级完成口直接进 XBAR（`ariane_peripherals_xilinx.sv:1309-1310`），XBAR 的 ID 处理不在证据里 ⇒ 这是 §3 的开放问题之一。
- 请求合流按 `.bank` 覆盖 tag（`:330-337`，round-robin `stream_arbiter` `:339-352`）；响应按 `.bank` 路由（`:360-368`）。
- in_flight 聚合：AW-accept→B、AR-accept→R-LAST、W-LAST-accept→B、W 半包窗口、两个 bank 的 `bank_busy`（`:506-546`），并带 underflow/overflow `$error`（`:552-575`）。legacy E3：`e3_o='0`，只有 `.quiet = !in_flight_o`（`:582-585`，与模块头 `:38-41` 一致）。

### 1.4 测试 harness 的等价性对照（`tests/nonblock_banked_top.sv`）

| 板级 | joint TB | 行号 |
|---|---|---|
| TR/completion ID = `IdWidth` | `AxiIdWidth = 4` | `:155-158` |
| `BankIdBit = IdWidth-2` | `BankIdBit = 2` | `:578` |
| V did10/bank0，A did11/bank1（A_ID=4 ⇒ bit2=1） | `V_DID=10, V_ID=0, A_DID=11, A_ID=4` | `cpp:33-36` |
| 1 级 DDT，DC 走 64B extended | `ddtp.iommu_mode=2'b0010`, `dc_ext_t` | `:540`, `:631` |
| `NumIotlbEntries=16` | `Cfg.NumIotlbEntries=16` | `:323` |
| `RVIOMMUCfgBank`：每 bank OT=2、W=4 | `NumOutstandingTrans=4, WFifoDepth=8` | `:319-320` |
| E3Enable = InclDMA（板上 PTW 走 `rv_iommu_e3_delay`） | **E3Enable=0（纯直通）** | TB `:627`；TL `:715-716`；板上 `:1332` |
| DDTW/PTW 共用 1 个 DS master | **两个独立端口 + 两个独立模型** | TB `:116-143`；`cpp:64,270-304,334-337` |

我核对了 TB 的页表/PTE 编码自洽性（不是旧数据统计，只是公式核对）：`fsc=0x8000000000000020` ⇒ mode nibble=8(Sv39)、root=0x20000；`0x20000→ppn 0x40→0x40000`；`0x40400→ppn 0x50→0x50000`；`0x50000` PTE `0x200000DF` ⇒ PPN=`(PTE>>10)=0x80000` ⇒ PA=`0x80000000`（与注释 `cpp:187` 一致）；A 侧 `0x30000→0x60000`、`0x60800→0x70000`、`0x70000` PTE `0x240000DF` ⇒ PA `0x90000000`（`cpp:193-196`）。**DC 字段名/word 顺序只有 TB 注释（`cpp:165-182`）为依据，权威布局在未提供的 `rv_iommu_pkg.sv`**。

---

## 2. 证据 B：低层 control head / quiet / IOFENCE / NL TB 共享端口

### 2.1 control head（FIFO 头驱动一切，`trans_ctl_wrap.sv`）

- AW/AR 各自 `stream_fifo`，深度 `NumOutstandingTrans`（bank 内为一半）（`:110-158`）。
- **AW 优先于 AR**：`IDLE` 里先判 `aw_valid && !w_cnt_ovf`（`:619-637`），再判 `ar_valid`（`:640-662`）。注释明说是为了让 read-to-write DMA 先开 W 排空（`:617-618`）。
- W 计数与门控：`incr_w_cnt = comp AW handshake`（`:172`）、`decr_w_cnt = w_valid&w_ready&w_data.last`（`:173`）、`w_ready = (w_cnt_q!=0) ? comp_resp.w_ready : 0`（`:214`）、`comp_req.w_valid = w_valid & (w_cnt_q!=0)`（`:354`）。⇒ **W 数据只有在对应完成 AW 被接受后才可能被转发**；但**从设备侧 W 本身可以在 AW 之前进入 TR W-FIFO**（`:216-234`，`tr_w_ready` 与 AW 无关）。这条对测试的约束就是你的"slave W 严格按 AW 先后"：TB 必须按 AW 顺序喂 W（现有 `up.w_attrib` 正是这么做，`cpp:370-388`），DB 里不能靠搜索 payload 反推归属。
- `e3_o` 的 head 观察：`select_write = aw_valid && !w_cnt_ovf`、`select_id/select_did/select_iova` 取 FIFO 头（`:199-208`）、`quiet = state_q==IDLE && !aw_valid && !ar_valid`（`:198`）。
- 完成总线：AW/AR 地址 = `trans_resp_data_q.spaddr`（`:327,341`）；`r_ready/b_ready` 直通（`:355-356`）；错误翻译走本地 `axi_err_slv`（RESP_SLVERR，`:577-592`），正常走完成口（`:552-563`）。

### 2.2 quiet / IOFENCE 路径

- 板级 quiet 消费点：`dma_research_observer(... .quiet_i(research_dma_quiet && e3_ctl.quiet && !e3_tl.ptw_active))`（`ariane_peripherals_xilinx.sv:1009`）与 policy 观察者（`:1036`）。
  - `e3_ctl` 在 banked 模式下来自 `banked_ctl`，**只有 `.quiet == !in_flight_o` 有意义**，其余字段恒 0（`banked_ctl:38-41,577-585`，top `:540` 还把它用于 `e3_waiting_i`）。
  - `in_flight` 聚合了双 bank 的 AW/AR/W 与两个 `bank_busy`（`banked_ctl:542-546`）⇒ **跨 bank 的 IOFENCE 判据在 ctl 侧是完整的**（这是"两 owner 但 quiet 仍可用"的设计意图）。
  - `e3_tl.ptw_active` 来自 walker 的 `active_o`（TL `:735` → `:696`）。
  - **注意（候选缺口，非结论）**：`quiet_i` 三项里**没有** `nonblock_busy`（`:1009`/`:1036`），而 `nonblock_busy_o` 在 TL 里存在（`:128,277`）。这是 Codex 的架构判断项。
- 软件侧 in_flight 出口：`rv_iommu_sw_if_wrap(... .in_flight_i(in_flight))`（top `:681`）；IOFENCE.C 的具体实现模块不在提供文件里 ⇒ IOFENCE 语义的最终判据未在本证据集内闭合（标注为缺失）。

### 2.3 TL 的共享端口（`rv_iommu_tl_wrap.sv`）

- 慢路径**单请求**状态机，入口是 3 输入 prio arbiter + spill register（`:170-203`）；`req_ready` 只在**响应真的能被取走**时置位：`(debug_resp_valid_o & debug_resp_ready_i) | (slow_resp_valid & slow_resp_ready) | (ats_trans_resp_valid_o & ats_trans_resp_ready_i)`（`:163-165`）。慢路径完成响应经 2 输入 rr arbiter 与 fast 响应合流（`:317-324`）。
- 非阻塞闸门（Mode1 runtime 关键）：`normal_gate = !NonblockEnable || (state_q==IDLE && !req_valid && !n_intercept)`（`:273-274`）；`trans_req_ready_o = n_intercept ? n_accept : (normal_trans_ready && normal_gate)`（`:275`）；`nonblock_fire_o = n_accept`（`:276`）；`n_*` 全部来自 `rv_iommu_nonblock_hit`，其 `.enable_i(nonblock_enable_i)`（`:284-302`）。
  - **依赖（未证实）**：`rv_iommu_nonblock_hit.sv` 不在提供文件里。"runtime=0 时 `n_intercept/n_accept/n_busy` 恒 0"这一点**只能由现有 TB 用例 `case_nonblock_disabled_blocks` 通过（你已确认）反推**，不能从 RTL 文本证明。§4 的测试直接依赖它，已在其机制 guard 中显式声明（失败会报出）。fast 路径是否回显 `.bank` 同样未知（TL `:300` 只把 `fast_resp_data` 送进 arbiter），但本提案在 runtime=0 下**不使用** fast 路径。
- 共享 IOTLB 端口：`iotlb_lu = (state_q==PT)|(state_q==MSI)|n_iotlb_lu`（`:260`），`iotlb_lu_hit = cache_iotlb_hit && !n_iotlb_lu`（`:266`），写口 `iotlb_update = msi? msi_iotlb_update : ptw_iotlb_update`（`:267-269`）；单实例 `rv_iommu_iotlb`（`:628-649`）。
- 共享 DDTC 端口：`ddtc_lu = (state_q==DDT)|ats_ddtc_valid_i|n_ddtc_lu`（`:238`），`ddtc_lu_hit = cache_ddtc_hit && !n_ddtc_lu`（`:241`），`touch_i(!n_ddtc_lu||n_ddtc_touch)`（`:487`）。
- PTW 共享/路径：`ptw_init = (state_q==PT) & ~iotlb_lu_hit`（`:346`）；`E3Enable` 分支把 walker 包在 `rv_iommu_e3_delay` 里（`:705-713`），否则纯直通（`:714-722`）；PTW 实例 `:723-768`。`hpm_events`/`e3_tl`/`research` 是仅有的低层观察面（`:676-704`），本提案用它做"TL 接受/响应/PTW/IOTLB 命中"进度记录。
- 失效入口：`iotinval_i`/`iodirinval_i`（`:99-100`）分别送到 DDTC（`:482`）、IOTLB（`:635`）、MRIFC（`:869`）与非阻塞失效项 `inval_vma||inval_gvma||inval_ddtc||inval_pdtc`（`:292-293`）——**字段名由此证实存在**（§4.1 直接使用，不必猜）。

---

## 3. 已证实 vs 假设；缺失文件；交给 Codex 的开放问题

### 3.1 已证实（文本级）

1. 双 bank 选路 = 原始 AXI ID 的 `BankIdBit` 位；板级该位 = DMA 端口号最低位（`ariane_peripherals_xilinx.sv:1161-1186,1207,1331`；`banked_ctl:241-242`）。
2. 完成 mux 的 ID 加宽/裁掉/重建规则（`banked_ctl:199-211,378-409,417-456,463-481`）。
3. 每 bank 容量被减半（`banked_ctl:160-162`）+ 偶数/≥2 断言（`:147-152`）。
4. 跨 bank `in_flight`/`quiet` 聚合语义（`banked_ctl:506-546,582-585`；板级消费 `:1009,1036`）。
5. TL 慢路径单请求 + `normal_gate` 闸门 + 双响应 arbiter（`tl_wrap:163-203,273-277,317-324`）。
6. TL 的 IOTLB/DDTC 单端口共享与 `n_*` 排除逻辑（`:238-269,486-491,628-649`）。
7. 真实 top 中 9 个 DSL 请求者共用一个 DS master（`rv_iommu_top:247-264,303-340`）+ 板级 `axi_delayer`（`:1284-1306`）。
8. **monitor 作用域 bug（定位，不修）**：`nonblock_data/fire/trans_accept/did/type` 声明在 `gen_dma`（`ariane_peripherals_xilinx.sv:914,930-934`），消费者在 `gen_dma/gen_research`（`:988-1000`），而 `rv_iommu_top` 的 monitor 输出连接在**兄弟** generate 块 `gen_iommu`（`:1274,1381-1382`）⇒ `gen_iommu` 内生成 implicit net，`gen_dma` 侧读到的网永久未驱动（Vivado undriven 警告）。后果：`accept/fast/did/ttype` 恒 0，与"accepted 全 0"一致；**它无法解释 A8 停滞**（只影响观察）。

### 3.2 假设（需验证）

| 假设 | 依据/风险 |
|---|---|
| `ariane_soc::IdWidth == 4`（否则 TB 的 `BankIdBit=2` 不等价） | 包未提供 |
| 外部完成从设备**原样回显** AW/AR ID | `banked_ctl:25-31` 自述；XBAR 代码未提供 |
| runtime=0 时 `n_intercept/n_accept/n_busy = 0` | 由现有 TB 用例通过反推；模块源码未提供 |
| `fast_resp_data.bank` 正确回显 | fast 模块未提供；本提案 runtime=0 下不使用 |
| DC/PTE 字段布局（`dc_ext_t` word 顺序、`ta.pscid` 位置） | 仅 TB 注释；`rv_iommu_pkg.sv` 未提供；TB 号称通过 |
| 失效结构体除 `inval_vma/gvma/ddtc/pdtc` 外的字段未被驱动即为"失效全部" | IOTLB/DDTC 内部失效比较逻辑未提供 ⇒ §4 只做"功能仍正确"检查，**不**断言驱逐发生 |

### 3.3 缺失文件（无法闭合的证据）

`ariane_soc` 包（IdWidth/IdWidthSlave/RVIOMMUCfg）、`rv_iommu_ds_if.sv`（DS 仲裁真实行为）、`rv_iommu_nonblock_hit.sv`、`rv_iommu_e3_delay.sv`、`rv_iommu_iotlb.sv`/`rv_iommu_ddtc.sv`（失效比较逻辑）、XBAR/`axi_xbar` 配置、`rv_iommu_pkg.sv`（DC/失效结构体权威布局）、`rv_iommu_sw_if_wrap`（IOFENCE.C 出口）。

### 3.4 开放问题（Codex 定根；我只给候选域与引用）

1. **A8 停滞候选域**：`n_*` 生命周期（`:284-302`）在"双 bank 同时有候选"时的收敛；`normal_gate` 拒绝的请求是否一定会被后续重试（arbiter 输入被 `normal_gate` 门控，`:179-181`）。
2. **B/R ID 重建 vs XBAR**（`banked_ctl:471,476`）：若互联改动 ID，一个 bank 的完成永不回收 ⇒ `in_flight` 卡高、A 永不完成。A1 已通过削弱但未排除（A1 也有读写）。
3. **板级 E3 delay 在 banked 模式的死端**：top 给 banked ctl `E3Enable=1'b0`（`:348`），但给 TL `E3Enable`（`:470`，板级=InclDMA `:1332`），而 `e3_waiting_i` 由恒 0 的 `victim_waiting` 推出（`:540`）⇒ E3 capture 永不 eligible（TL `:708`）；`rv_iommu_e3_delay` 在只 `enable_i` 时是否仍会推迟 walker 请求**未知**。需要 `rv_iommu_e3_delay.sv`。
4. **quiet 不含 nonblock_busy**（`:1009/1036` vs TL `:128`）。
5. **launcher 在 banked 模式被实例化但输入恒 0**（`:1066-1089` 用 `e3_ctl.select_*`，而 banked ctl 恒 0）——是否属于"可接受空转"？

---

## 4. 复现测试提案（现有 joint TB 扩展；完整聚焦 diff；不改 RTL）

### 4.0 运行入口与文件

- 新增逻辑全部放在**已有两个测试文件**里（harness + driver），因此**运行入口不变**：`python3 tests/test_nonblock_banked.py`（`tests/test_nonblock_banked.py:19,64,99-121,136-137`）。SOURCES 已含 `tests/nonblock_banked_top.sv`（`:64`），无需改脚本。
- 场景配置：`NonblockEnable=1`（harness 默认 `:16-17`，传给 TL `:628`），**runtime `nonblock_enable_i=0`**（driver `reset(false)`，`cpp:499-521` + `cpp:331`）⇒ 对应板级 **Mode1（banked，runtime off）**。
- 每步预算：`MP_STEP_DEADLINE_CYCLES`；整场景硬上限：`MP_TOTAL_DEADLINE_CYCLES`；任一步超时 → 打印**首次未完成状态**并 `abort`。

### 4.1 `tests/nonblock_banked_top.sv` diff（只加观察/激励端口，不动 DUT 实例参数）

**(a) 端口表**：把 `:146-149` 替换为

```systemverilog
    // ---------------- control/trace ------------------------------------------
    input  logic        nonblock_enable_i,
    output logic        nonblock_fire_o,
    output logic        nonblock_busy_o,
    output logic        in_flight_o,

    // ---------------- observation only (test harness, not RTL) --------------
    output logic        dbg_tl_req_valid,
    output logic        dbg_tl_req_ready,
    output logic        dbg_tl_req_bank,
    output logic        dbg_tl_resp_valid,
    output logic        dbg_tl_resp_ready,
    output logic        dbg_tl_resp_bank,
    output logic        dbg_tl_ptw_active,
    output logic        dbg_tl_ptw_r_fire,
    output logic        dbg_tl_iotlb_lookup,
    output logic        dbg_tl_iotlb_hit,

    // ---------------- invalidation stress drive -----------------------------
    input  logic        inval_pulse,
    input  logic [1:0]  inval_kind
);
```

**(b) 失效激励 + 观察 assign**：把 `:544-549` 替换为

```systemverilog
    rv_iommu::iotlb_inval_t iotinval_i;
    rv_iommu::xdtc_inval_t  iodirinval_i;
    rv_iommu::e3_tl_t       tl_e3;

    // Invalidation stress.  Field names are exactly the ones rv_iommu_tl_wrap
    // consumes (rv_iommu_tl_wrap.sv:292-293): inval_vma/inval_gvma on
    // iotlb_inval_t and inval_ddtc/inval_pdtc on xdtc_inval_t.  No addr/pscid/
    // gscid selector is driven, so this is only a best-effort "everything"
    // hint: the driver never asserts that an entry was evicted, only that
    // later translations stay correct (see nonblock_banked_test.cpp).
    always_comb begin
        iotinval_i   = '0;
        iodirinval_i = '0;
        if (inval_pulse) begin
            case (inval_kind)
                2'b00:   iotinval_i.inval_vma    = 1'b1;
                2'b01:   iotinval_i.inval_gvma   = 1'b1;
                2'b10:   iodirinval_i.inval_ddtc = 1'b1;
                default: iodirinval_i.inval_pdtc = 1'b1;
            endcase
        end
    end

    assign dbg_tl_req_valid    = trans_req_valid;
    assign dbg_tl_req_ready    = trans_req_ready;
    assign dbg_tl_req_bank     = trans_req_data.bank;
    assign dbg_tl_resp_valid   = trans_resp_valid;
    assign dbg_tl_resp_ready   = trans_resp_ready;
    assign dbg_tl_resp_bank    = trans_resp_data.bank;
    assign dbg_tl_ptw_active   = tl_e3.ptw_active;
    assign dbg_tl_ptw_r_fire   = tl_e3.ptw_r_fire;
    assign dbg_tl_iotlb_lookup = tl_e3.lookup;
    assign dbg_tl_iotlb_hit    = tl_e3.hit;
```

（`tl_e3` 字段名的证据：`rv_iommu_tl_wrap.sv:689-703`。`trans_req_*` 为 harness 内部线 `:187-192,360-365`。）

**(c) TL 实例**：把 `:686` 的 `.e3_o ()` 改为 `.e3_o (tl_e3)`。其余端口、参数（`.E3Enable(1'b0)` `:627`、`.NonblockEnable(NonblockEnable)` `:628`）**不动**。

### 4.2 `tests/nonblock_banked_test.cpp` 新增/替换代码

**(a) include**（`:12` 之后）

```cpp
#include <functional>
#include <string>
```

**(b) 常量与页映射**（`:49` 之后，紧接 `make_test_word`）

```cpp
// ---------------------------------------------------------------------------
// Multipage IOTLB-capacity-contention scenario.
//   Mode1 = joint TB built with NonblockEnable=1 (nonblock_banked_top.sv:628)
//           but nonblock_enable_i driven to 0 at runtime (reset(false)).
//   18 distinct 4 KiB pages against NumIotlbEntries = 16
//   (nonblock_banked_top.sv:323):
//     V: did10, AXI id 0 (bank0)  -> 1 source page + 1 destination page
//     A: did11, AXI id 4 (bank1)  -> 8 source pages + 8 destination pages
//   A cycles its 8+8 pages while V stays hot on its single page pair.
// ---------------------------------------------------------------------------
static const uint32_t MP_BANK_BIT      = 2;     // nonblock_banked_top.sv:578
static const uint32_t MP_V_PAGES       = 2;
static const uint32_t MP_A_PAGES       = 16;
static const uint32_t MP_IOTLB_ENTRIES = 16;
static const uint32_t MP_WARM_ROUNDS   = 16;    // warm rounds per repeat
static const uint32_t MP_TRANSFER_RUNS = 64;    // 4 KiB transfers per path/repeat
static const uint32_t MP_REPEATS       = 2;     // warm/run/drain/invalidate
static const uint32_t MP_BURST_LEN     = 255;   // 256 beats x 8 B = 2 KiB
static const uint32_t MP_BURST_BEATS   = 256;
static const uint32_t MP_PAGE_BEATS    = 512;   // 4 KiB
static const long     MP_STEP_DEADLINE_CYCLES  = 400000;
static const long     MP_TOTAL_DEADLINE_CYCLES = 4000000;   // hard scenario cap

static_assert(MP_V_PAGES + MP_A_PAGES > MP_IOTLB_ENTRIES,
              "multipage scenario needs more pages than IOTLB entries");

// Unique full-width pattern per (page, word).  make_test_word is injective on
// u64 (odd multiplier + xor), so distinct (page, i) give distinct values.
static uint64_t mp_word(uint32_t page, uint32_t i) {
    return make_test_word(((uint64_t)page << 9) | (uint64_t)i);
}

// Pre-fill of a destination page.  Every byte must differ from the copied
// value so that a missing or mis-addressed full-strobe write is visible in the
// final per-address memory check.
static uint64_t mp_old_word(uint32_t page, uint32_t i) {
    return 0xD000000000000000ULL ^ ((uint64_t)page << 40) ^ (uint64_t)i;
}
```

**(c) Tb 成员**（`:131` `g_pending_writes` 之后）

```cpp
    // ---- multipage scenario state ------------------------------------------
    struct MpPage { uint64_t iova; uint64_t pa; uint32_t page; };
    MpPage mp_v[MP_V_PAGES];
    MpPage mp_a[MP_A_PAGES];

    struct MpRead {
        bool     active = false;
        uint32_t id = 0;
        uint64_t exp_pa = 0;
        uint32_t page = 0;
        uint32_t ars = 0;     // completion ARs accepted (expect 2)
        uint32_t beats = 0;   // R beats verified        (expect 512)
        uint32_t lasts = 0;   // R-LAST beats seen       (expect 2)
    };
    MpRead mp_rd[2];          // indexed by bank

    long     mp_scenario_start = 0;
    uint64_t mp_ptw_ar_total = 0, mp_ddtw_ar_total = 0;
    uint64_t mp_iotlb_lookup_total = 0, mp_iotlb_hit_total = 0;
    uint64_t mp_ptw_r_fire_total = 0, mp_ptw_active_cycles = 0;
    uint64_t mp_tl_accept_total = 0, mp_tl_resp_total = 0;
    uint64_t mp_nonblock_fire_total = 0;
    uint64_t mp_up_r_total = 0, mp_up_b_total = 0;
    int      mp_tl_last_accept_bank = -1;
    uint64_t mp_last_ptw_ar_addr = 0;
    uint32_t mp_last_ptw_ar_id = 0;
    uint64_t mp_total_beats = 0;
    std::vector<std::string> mp_steps;

    bool     inval_pulse = false;
    uint32_t inval_kind = 0;

    uint32_t mp_bank_of(uint32_t id) const { return (id >> MP_BANK_BIT) & 1u; }
```

**(d) 激励驱动**：在 `drive_comp()`（`:256-268`）之后新增并在 `tick()` 里调用

```cpp
    void drive_aux() {
        top->inval_pulse = inval_pulse ? 1 : 0;
        top->inval_kind  = inval_kind;
    }
```

并在 `tick()` 的 `drive_comp();`（`:333`）之后插入 `drive_aux();`。

**(e) `tick()` 计数器块**（`:367-368` 之后插入）

```cpp
        // ---- multipage / shared-path instrumentation -----------------------
        if (p_ar_fire) {
            ++mp_ptw_ar_total;
            mp_last_ptw_ar_addr = p_ar_addr;
            mp_last_ptw_ar_id   = p_ar_id;
        }
        if (d_ar_fire) ++mp_ddtw_ar_total;
        if (top->dbg_tl_iotlb_lookup) {
            ++mp_iotlb_lookup_total;
            if (top->dbg_tl_iotlb_hit) ++mp_iotlb_hit_total;
        }
        if (top->dbg_tl_ptw_active)   ++mp_ptw_active_cycles;
        if (top->dbg_tl_ptw_r_fire)   ++mp_ptw_r_fire_total;
        if (top->nonblock_fire_o)     ++mp_nonblock_fire_total;
        if (top->dbg_tl_req_valid && top->dbg_tl_req_ready) {
            ++mp_tl_accept_total;
            mp_tl_last_accept_bank = top->dbg_tl_req_bank ? 1 : 0;
        }
        if (top->dbg_tl_resp_valid && top->dbg_tl_resp_ready) ++mp_tl_resp_total;
```

**(f) `tick()` 的 `up_r_fire` 块**（`:389-393`）替换为

```cpp
        if (up_r_fire) {
            L_up_r.push_back({g_cycle, top->tr_r_id, top->tr_r_data,
                              (bool)top->tr_r_last, (uint32_t)top->tr_r_resp});
            ++mp_up_r_total;
            mp_note_r_beat((uint32_t)top->tr_r_id, (uint64_t)top->tr_r_data,
                           (bool)top->tr_r_last, (uint32_t)top->tr_r_resp);
        }
```

**(g) `tick()` 的 `cmp_ar_fire` 块**（`:416-427`）在 `L_cmp_ar.push_back(...)` 之后插入

```cpp
            mp_note_comp_ar(top->comp_ar_id, top->comp_ar_addr,
                            (uint32_t)top->comp_ar_len);
```

**(h) `reset()`**（`:499-521`）在 `g_pending_writes.clear();`（`:516`）之后插入

```cpp
        for (uint32_t b = 0; b < 2; ++b) mp_rd[b] = MpRead();
        mp_steps.clear();
        inval_pulse = false; inval_kind = 0;
```

**(i) 新场景完整实现**（插在 `case_same_bank_ordering()` 结束（`:1048`）之后、`};`（`:1049`）之前）

```cpp
    // -----------------------------------------------------------------------
    // Multipage IOTLB-capacity contention (Mode1: NonblockEnable=1 parameter,
    // nonblock_enable_i = 0 at runtime).
    // -----------------------------------------------------------------------
    void mp_setup_memory() {
        setup_memory();                      // DDT, DCs, V/A roots and L1s

        // Aggressor page table: iova 0x20000000 + n*0x1000
        //   VPN2 = 0        -> root entry  0x30000 (already written)
        //   VPN1 = 0x100    -> L1A  entry  0x60800 (already written)
        //   VPN0 = n        -> L0A  0x70000 + n*8
        // n = 0..7  : source pages PA 0x90000000 + n*0x1000
        // n = 8..15 : destination pages PA 0x90100000 + (n-8)*0x1000
        for (uint32_t n = 0; n < MP_A_PAGES; ++n) {
            uint64_t pa = (n < 8) ? (0x90000000ULL + (uint64_t)n * 0x1000)
                                  : (0x90100000ULL + (uint64_t)(n - 8) * 0x1000);
            write64(0x70000 + (uint64_t)n * 8, ((pa >> 12) << 10) | 0xDFULL);
        }

        mp_v[0] = {VSRC_IOVA, VSRC_PA, 0};
        mp_v[1] = {VDST_IOVA, VDST_PA, 1};
        for (uint32_t n = 0; n < MP_A_PAGES; ++n) {
            mp_a[n].iova = A_IOVA + (uint64_t)n * 0x1000;
            mp_a[n].pa   = (n < 8) ? (0x90000000ULL + (uint64_t)n * 0x1000)
                                   : (0x90100000ULL + (uint64_t)(n - 8) * 0x1000);
            mp_a[n].page = 2 + n;
        }

        // Unique source content, distinct pre-fill in every destination page.
        for (uint32_t i = 0; i < MP_PAGE_BEATS; ++i) {
            write64(mp_v[0].pa + (uint64_t)i * 8, mp_word(mp_v[0].page, i));
            write64(mp_v[1].pa + (uint64_t)i * 8, mp_old_word(mp_v[1].page, i));
            for (uint32_t n = 0; n < 8; ++n) {
                write64(mp_a[n].pa + (uint64_t)i * 8, mp_word(mp_a[n].page, i));
                write64(mp_a[8 + n].pa + (uint64_t)i * 8,
                        mp_old_word(mp_a[8 + n].page, i));
            }
        }
    }

    void mp_begin_step(const std::string& name) {
        mp_steps.push_back(name);
        std::printf("MP-STEP %s start cycle=%ld\n", name.c_str(), g_cycle);
    }

    // First-incomplete-state dump: accepted AR/AW/W/B/R, TL accept/response/
    // bank, PTW, DDTW, IOTLB, per-bank read progress, queues, quiet.
    void mp_dump_stuck(const std::string& name, long budget) {
        const MpRead& v = mp_rd[0];
        const MpRead& a = mp_rd[1];
        std::fprintf(stderr,
            "MP-STUCK step=%s budget=%ld cycle=%ld scenario_cycles=%ld\n"
            "  accepted  up_ar=%zu up_aw=%zu up_w=%zu up_b=%zu up_r=%zu\n"
            "  comp      cmp_ar=%zu cmp_aw=%zu cmp_w=%zu\n"
            "  tl        accept=%lu resp=%lu last_accept_bank=%d req_v=%d req_r=%d"
            " resp_v=%d resp_r=%d resp_bank=%d\n"
            "  ptw       active=%d active_cycles=%lu r_fire=%lu ar_total=%lu"
            " last_ar_addr=0x%016lx last_ar_id=%u\n"
            "  ddtw      ar_total=%lu\n"
            "  iotlb     lookup=%lu hit=%lu\n"
            "  banks     v{active=%d ars=%u beats=%u lasts=%u}"
            " a{active=%d ars=%u beats=%u lasts=%u}\n"
            "  queues    r_q=%zu b_q=%zu caw_q=%zu comp_w_q=%zu"
            " pending_writes=%zu wq=%zu w_attrib=%zu\n"
            "  quiet     in_flight=%d nonblock_busy=%d nonblock_fire=%d\n",
            name.c_str(), budget, g_cycle, g_cycle - mp_scenario_start,
            L_up_ar.size(), L_up_aw.size(), L_up_w.size(), L_up_b.size(),
            L_up_r.size(), L_cmp_ar.size(), L_cmp_aw.size(), L_cmp_w.size(),
            (unsigned long)mp_tl_accept_total, (unsigned long)mp_tl_resp_total,
            mp_tl_last_accept_bank,
            (int)top->dbg_tl_req_valid, (int)top->dbg_tl_req_ready,
            (int)top->dbg_tl_resp_valid, (int)top->dbg_tl_resp_ready,
            (int)top->dbg_tl_resp_bank,
            (int)top->dbg_tl_ptw_active, (unsigned long)mp_ptw_active_cycles,
            (unsigned long)mp_ptw_r_fire_total, (unsigned long)mp_ptw_ar_total,
            (unsigned long)mp_last_ptw_ar_addr, mp_last_ptw_ar_id,
            (unsigned long)mp_ddtw_ar_total,
            (unsigned long)mp_iotlb_lookup_total,
            (unsigned long)mp_iotlb_hit_total,
            (int)v.active, v.ars, v.beats, v.lasts,
            (int)a.active, a.ars, a.beats, a.lasts,
            r_q.size(), b_q.size(), caw_q.size(), comp_w_q.size(),
            g_pending_writes.size(), up.wq.size(), up.w_attrib.size(),
            (int)top->in_flight_o, (int)top->nonblock_busy_o,
            (int)top->nonblock_fire_o);
    }

    bool mp_wait(const std::string& step, const std::function<bool()>& done,
                 long budget = MP_STEP_DEADLINE_CYCLES) {
        long start = g_cycle;
        for (;;) {
            if (done()) return true;
            if (g_cycle - start >= budget) {
                mp_dump_stuck(step, budget);
                CHECK_AT(false, "multipage step exceeded its cycle budget");
                return false;
            }
            if (g_cycle - mp_scenario_start >= MP_TOTAL_DEADLINE_CYCLES) {
                mp_dump_stuck(step, MP_TOTAL_DEADLINE_CYCLES);
                CHECK_AT(false, "multipage scenario exceeded its cycle deadline");
                return false;
            }
            tick();
        }
    }

    void mp_arm_read(uint32_t id, const MpPage& p) {
        MpRead& rd = mp_rd[mp_bank_of(id)];
        CHECK_AT(!rd.active, "mp_arm_read: bank already has an armed read");
        rd.active = true;
        rd.id     = id;
        rd.exp_pa = p.pa;
        rd.page   = p.page;
        rd.ars = 0; rd.beats = 0; rd.lasts = 0;
    }

    bool mp_read_done(uint32_t id) const {
        const MpRead& rd = mp_rd[mp_bank_of(id)];
        return rd.active && rd.ars == 2 && rd.beats == MP_PAGE_BEATS;
    }

    // Inline R checker.  Silent for any beat that is not part of the multipage
    // case, so the existing cases are untouched.
    void mp_note_r_beat(uint32_t id, uint64_t data, bool last, uint32_t resp) {
        MpRead& rd = mp_rd[mp_bank_of(id)];
        if (!rd.active) return;
        CHECK_AT(id == rd.id, "multipage R beat id mismatch");
        CHECK_AT(resp == 0, "multipage R resp not OKAY");
        CHECK_AT(rd.beats < MP_PAGE_BEATS, "too many R beats for one multipage read");
        CHECK_AT(data == read64(rd.exp_pa + (uint64_t)rd.beats * 8),
                 "multipage R data mismatch");
        if (last) {
            CHECK_AT(rd.beats == MP_BURST_BEATS - 1 ||
                     rd.beats == MP_PAGE_BEATS - 1,
                     "multipage R-LAST at an unexpected beat");
            ++rd.lasts;
        } else {
            CHECK_AT(rd.beats != MP_BURST_BEATS - 1 &&
                     rd.beats != MP_PAGE_BEATS - 1,
                     "missing multipage R-LAST");
        }
        ++rd.beats;
        ++mp_total_beats;
    }

    // Address mapping check for reads: in-bank burst order must be
    // exp_pa, exp_pa + 0x800.
    void mp_note_comp_ar(uint32_t id, uint64_t addr, uint32_t len) {
        MpRead& rd = mp_rd[mp_bank_of(id)];
        if (!rd.active) return;
        CHECK_AT(id == rd.id, "multipage comp AR id mismatch");
        CHECK_AT(len == MP_BURST_LEN, "multipage comp AR len mismatch");
        CHECK_AT(rd.ars < 2, "too many completion ARs for one multipage read");
        CHECK_AT(addr == rd.exp_pa + (uint64_t)rd.ars * 0x800,
                 "multipage comp AR address mismatch");
        ++rd.ars;
    }

    void mp_issue_read_burst(uint32_t id, uint32_t did, const MpPage& p,
                             uint32_t half, const std::string& tag) {
        CHECK_AT(mp_rd[mp_bank_of(id)].active,
                 "mp_issue_read_burst: no armed read for this bank");
        size_t prev_ar = L_up_ar.size();
        start_read(id, p.iova + (uint64_t)half * 0x800, MP_BURST_LEN, did);
        mp_wait(tag + " AR accept", [&] { return L_up_ar.size() > prev_ar; });
    }

    // V and A each get both 2 KiB read bursts in flight, so both banks' slow
    // translations are contending for the single TL slow path and the walkers.
    void mp_read_4kib_pair(const MpPage& vp, const MpPage& ap,
                           const std::string& tag) {
        mp_begin_step(tag + " reads");
        mp_arm_read(V_ID, vp);
        mp_arm_read(A_ID, ap);
        mp_issue_read_burst(V_ID, V_DID, vp, 0, tag);
        mp_issue_read_burst(A_ID, A_DID, ap, 0, tag);
        mp_issue_read_burst(V_ID, V_DID, vp, 1, tag);
        mp_issue_read_burst(A_ID, A_DID, ap, 1, tag);
        mp_wait(tag + " reads complete",
                [&] { return mp_read_done(V_ID) && mp_read_done(A_ID); });
        CHECK_AT(mp_rd[mp_bank_of(V_ID)].lasts == 2, "V read saw != 2 R-LAST");
        CHECK_AT(mp_rd[mp_bank_of(A_ID)].lasts == 2, "A read saw != 2 R-LAST");
        mp_rd[mp_bank_of(V_ID)].active = false;
        mp_rd[mp_bank_of(A_ID)].active = false;
    }

    void mp_touch(const char* who, const MpPage& p, uint32_t id, uint32_t did) {
        mp_begin_step(std::string(who) + " warm page " + std::to_string(p.page));
        size_t prev_r  = L_up_r.size();
        size_t prev_ar = L_cmp_ar.size();
        start_read(id, p.iova, 0, did);
        mp_wait(std::string(who) + " warm read",
                [&] { return L_up_r.size() > prev_r; });
        CHECK_AT(L_up_r.size() == prev_r + 1, "warm read: unexpected R count");
        CHECK_AT(L_cmp_ar.size() == prev_ar + 1, "warm read: missing comp AR");
        CHECK_AT(L_cmp_ar.back().addr == p.pa, "warm read: comp AR addr mismatch");
        CHECK_AT(L_cmp_ar.back().id == id, "warm read: comp AR id mismatch");
        UpRRec r = L_up_r.back();
        CHECK_AT(r.id == id, "warm read: R id mismatch");
        CHECK_AT(r.resp == 0, "warm read: R resp not OKAY");
        CHECK_AT(r.last, "warm read: R last not set");
        CHECK_AT(r.data == read64(p.pa), "warm read: R data mismatch");
    }

    void mp_warm_round() {
        for (uint32_t k = 0; k < MP_V_PAGES; ++k) mp_touch("V", mp_v[k], V_ID, V_DID);
        for (uint32_t k = 0; k < MP_A_PAGES; ++k) mp_touch("A", mp_a[k], A_ID, A_DID);
    }

    // One 4 KiB write, split into the two legal 2 KiB bursts, strictly one AW
    // outstanding at a time (slave W follows AW order, no payload search).
    void mp_write_4kib(uint32_t id, uint32_t did, const MpPage& dst,
                       const MpPage& src, const std::string& tag) {
        mp_begin_step(tag + " write");
        for (uint32_t half = 0; half < 2; ++half) {
            std::vector<Beat> beats;
            for (uint32_t i = 0; i < MP_BURST_BEATS; ++i) {
                Beat b;
                b.data = mp_word(src.page, half * MP_BURST_BEATS + i);
                b.strb = 0xFF;
                b.last = (i == MP_BURST_BEATS - 1);
                beats.push_back(b);
            }
            g_pending_writes.push_back({dst.pa + (uint64_t)half * 0x800,
                                        MP_BURST_LEN, beats});
            size_t prev_b  = L_up_b.size();
            size_t prev_aw = L_cmp_aw.size();
            size_t prev_w  = L_cmp_w.size();
            start_write(id, dst.iova + (uint64_t)half * 0x800, MP_BURST_LEN,
                        did, beats);
            mp_wait(tag + " write B", [&] { return L_up_b.size() > prev_b; });
            CHECK_AT(L_up_b.size() == prev_b + 1, "unexpected extra B");
            CHECK_AT(L_up_b.back().id == id, "write B id mismatch");
            CHECK_AT(L_up_b.back().resp == 0, "write B resp not OKAY");
            CHECK_AT(L_cmp_aw.size() == prev_aw + 1, "comp AW missing");
            CHECK_AT(L_cmp_aw.back().id == id, "comp AW id mismatch");
            CHECK_AT(L_cmp_aw.back().addr ==
                     dst.pa + (uint64_t)half * 0x800, "comp AW address mismatch");
            CHECK_AT(L_cmp_aw.back().len == MP_BURST_LEN, "comp AW len mismatch");
            CHECK_AT(L_cmp_w.size() == prev_w + MP_BURST_BEATS,
                     "comp W beat count mismatch");
            ++mp_up_b_total;
        }
        // End-to-end integrity, addressed by the translated destination page.
        for (uint32_t i = 0; i < MP_PAGE_BEATS; ++i) {
            CHECK_AT(read64(dst.pa + (uint64_t)i * 8) == mp_word(src.page, i),
                     "multipage destination content mismatch");
        }
    }

    void case_multipage_iotlb_capacity_mode1() {
        std::printf("== Mode1 (NonblockEnable=1, nonblock_enable_i=0) "
                    "multipage IOTLB capacity contention ==\n");
        mem.clear();
        mp_setup_memory();
        reset(false);                        // Mode1 runtime: enable_i = 0

        for (uint32_t b = 0; b < 2; ++b) mp_rd[b] = MpRead();
        mp_steps.clear();
        mp_ptw_ar_total = mp_ddtw_ar_total = 0;
        mp_iotlb_lookup_total = mp_iotlb_hit_total = 0;
        mp_ptw_r_fire_total = mp_ptw_active_cycles = 0;
        mp_tl_accept_total = mp_tl_resp_total = 0;
        mp_nonblock_fire_total = 0;
        mp_up_r_total = mp_up_b_total = mp_total_beats = 0;
        mp_tl_last_accept_bank = -1;
        mp_last_ptw_ar_addr = 0; mp_last_ptw_ar_id = 0;
        inval_pulse = false; inval_kind = 0;
        mp_scenario_start = g_cycle;

        for (uint32_t rep = 0; rep < MP_REPEATS; ++rep) {
            // warm: repeated touches of all 18 pages (16-entry IOTLB)
            for (uint32_t w = 0; w < MP_WARM_ROUNDS; ++w) mp_warm_round();

            // run: 4 KiB per path per transfer, A cycling its 8+8 pages
            for (uint32_t t = 0; t < MP_TRANSFER_RUNS; ++t) {
                const MpPage& asrc = mp_a[t % 8];
                const MpPage& adst = mp_a[8 + (t % 8)];
                std::string tag = "rep" + std::to_string(rep) +
                                  "/t" + std::to_string(t);
                mp_read_4kib_pair(mp_v[0], asrc, tag);
                mp_write_4kib(V_ID, V_DID, mp_v[1], mp_v[0], tag + "/V");
                mp_write_4kib(A_ID, A_DID, adst, asrc, tag + "/A");
            }

            // drain: every queue must be empty
            check_quiet();

            // invalidation stress: fill every invalidation kind we can name
            // (iotlb vma/gvma, ddtc, pdtc).  No eviction is asserted; only the
            // post-invalidate translations must stay correct.
            mp_begin_step("invalidate rep" + std::to_string(rep));
            for (uint32_t k = 0; k < 4; ++k) {
                inval_kind = k;
                inval_pulse = true;  tick();
                inval_pulse = false; tick();
            }
            mp_begin_step("post-invalidate rep" + std::to_string(rep));
            mp_touch("V", mp_v[0], V_ID, V_DID);
            mp_touch("A", mp_a[8], A_ID, A_DID);
            check_quiet();
        }

        // Mechanism guards: a run in which nothing moved must never be a pass.
        CHECK_AT(mp_iotlb_lookup_total >= (uint64_t)(MP_V_PAGES + MP_A_PAGES),
                 "no slow-path IOTLB lookups observed for the 18 cold pages");
        CHECK_AT(mp_ptw_ar_total >= (uint64_t)(MP_V_PAGES + MP_A_PAGES),
                 "no PTW walk requests observed for the 18 cold pages");
        CHECK_AT(mp_ddtw_ar_total >= 2, "no DDTW walk requests (2 DIDs)");
        CHECK_AT(mp_tl_accept_total >= (uint64_t)(MP_V_PAGES + MP_A_PAGES),
                 "no translation accepted by the TL");
        CHECK_AT(mp_tl_resp_total >= (uint64_t)(MP_V_PAGES + MP_A_PAGES),
                 "no translation response returned by the TL");
        CHECK_AT(mp_nonblock_fire_total == 0,
                 "nonblock fast path fired although enable_i is 0");
        CHECK_AT(mp_up_b_total > 0, "no upstream B");
        CHECK_AT(mp_up_r_total > 0, "no upstream R");

        std::printf("MP-SUMMARY repeats=%u warm_rounds=%u transfers_per_path=%u "
                    "pages=%u iotlb_entries=%u cycles=%ld tl_accept=%lu "
                    "tl_resp=%lu ddtw_ar=%lu ptw_ar=%lu ptw_r=%lu "
                    "iotlb_lookup=%lu iotlb_hit=%lu ptw_active_cycles=%lu "
                    "nonblock_fire=%lu beats=%lu\n",
                    MP_REPEATS, MP_WARM_ROUNDS, MP_TRANSFER_RUNS,
                    MP_V_PAGES + MP_A_PAGES, MP_IOTLB_ENTRIES,
                    g_cycle - mp_scenario_start,
                    (unsigned long)mp_tl_accept_total,
                    (unsigned long)mp_tl_resp_total,
                    (unsigned long)mp_ddtw_ar_total,
                    (unsigned long)mp_ptw_ar_total,
                    (unsigned long)mp_ptw_r_fire_total,
                    (unsigned long)mp_iotlb_lookup_total,
                    (unsigned long)mp_iotlb_hit_total,
                    (unsigned long)mp_ptw_active_cycles,
                    (unsigned long)mp_nonblock_fire_total,
                    (unsigned long)mp_total_beats);
        check_quiet();
    }
```

**(j) `main()`**（`:1063` 之后、PASS 打印之前）

```cpp
    tb.case_multipage_iotlb_capacity_mode1();
```

### 4.3 这个场景**能**证明 / **不能**证明什么

**能（若通过）**：在 `NonblockEnable=1`、runtime=0 的 Mode1 配置下，真实 `rv_iommu_banked_ctl + rv_iommu_tl_wrap` 在 **18 个 4 KiB 页 vs 16 项 IOTLB** 的容量压力下，V(固定 2 页) 与 A(循环 8+8 页) 的 4 KiB 读/写全部按地址映射完成；双 bank 同时把翻译送入**同一个 TL 慢路径**并触发真实 cold walk；每一步都有有限 cycle 上限与"首次未完成状态"转储；结束时所有队列排空、`in_flight_o`/`nonblock_busy_o` 均低；失效脉冲后功能仍正确。

**不能（必须写进结论）**：

1. **不是 `rv_iommu_top` 的共享 AXI 存储结构端口互连**：TB 的 DDTW 与 PTW 是两个独立顶层端口（`nonblock_banked_top.sv:116-143`），由两个独立模型服务（`nonblock_banked_test.cpp:64,270-304,334-337`），**没有** `rv_iommu_ds_if` 的仲裁、没有板级 `axi_delayer`（`ariane_peripherals_xilinx.sv:1284-1306`）、没有 XBAR、没有真实 ID remap。所以"同一物理 AXI 仲裁路径上的真实 PTW 共享内存响应"在这个 harness 里只是**被逼近**（两个 walker 的 AR 由 TB 顺序接受、由同一段 C++ 逻辑服务），**不是**同一条 AXI 通路 ⇒ 需要 §5 的最小新 harness 才能真正复现。
2. **不能解释/复现 A8 停滞**：本场景是 runtime=0（fast 路径不使用），而 A8 是 Mode2（`nonblock_enable_i=1`）路径；且 monitor 作用域 bug 未修、BIT 未重编。因此本测试通过**不得**被表述为"A8 已修复/已通过"。
3. **不断言 IOTLB 驱逐/命中**：`iotlb_inval_t` 除 vma/gvma 外的选择器未驱动、IOTLB 内部失效比较逻辑与替换策略不在证据里（`rv_iommu_iotlb.sv` 未提供）⇒ 只记录 `iotlb_lookup/hit` 与 `ptw_ar` 计数，不作"某页被驱逐"的判断。两个"≥18"机制 guard 只要求"18 个冷页至少各产生一次 look-up / 至少一次 walk AR"，是保守下界。
4. **依赖未提供模块**：`nonblock_enable_i=0 ⇒ n_intercept/n_accept/n_busy=0`（`rv_iommu_nonblock_hit.sv` 未提供；由现有 `case_nonblock_disabled_blocks` 通过反推）。若该假设不成立，测试会以 `MP-STUCK` 明确失败而不是假通过（这正是我们想要的）。

### 4.4 验收检查（未执行；供 Codex 运行）

1. `python3 tests/test_nonblock_banked.py`：现有 9 个用例仍 `PASS`（含 `case_nonblock_disabled_blocks`），新增用例打印 `MP-STEP …` 与 `MP-SUMMARY …`，末行 `nonblock_banked_test: PASS`。
2. 三种"应当失败"的可观测性自检（可临时注入，不得提交）：`MP_TOTAL_DEADLINE_CYCLES=1000` → 必须出现 `MP-STUCK` 且含 `tl/ptw/banks/queues/quiet` 五行；把 `mp_ptw_ar_total` 计数注释掉 → 必须因 `no PTW walk requests` 失败；把 `inval_pulse` 恒 1 → 不得产生协议错误（仅功能检查）。
3. 报告周期数（`MP-SUMMARY cycles=`）与 `ptw_ar`/`iotlb_lookup` 计数，作为"18 页/16 项"是否真的产生重复 walk 的机制证据。

---

## 5. 最小新 top harness / driver **建议**（共享 DS 端口；本项是建议，不是完整实现）

**目的**：把"DDTW/PTW 走同一条真实 AXI 读通路"变成可复现事实。最小改动 = 在两个 walker 端口前加一个 `axi_mux`（与 `rv_iommu_top` 把 9 个请求者合成 1 个 `ds_req_o` 同一类原语，`:303-340`；也与板级 `axi_mux` 用法 `:1161-1186` 同族），其余 DUT/检查逻辑复用 §4 的 driver。

**新文件 `tests/nonblock_banked_shared_top.sv`（骨架；`--top-module` 用它时不要同时把 `nonblock_banked_top.sv` 放进源列表）**

```systemverilog
`include "axi/typedef.svh"

// Same banked_ctl + tl_wrap pair as tests/nonblock_banked_top.sv, but the two
// walker read ports are merged into ONE AXI read master through axi_mux, which
// is the shape rv_iommu_top uses for all nine data-structure requestors
// (rv_iommu_top.sv:247-340 -> rv_iommu_ds_if.ds_req_o).
module nonblock_banked_shared_top /* ports: same as nonblock_banked_top, but
    ddtw_* / ptw_* are replaced by one ds_* read port (AR/R only) */ ( ... );

    // Slave-side (walker) types: 4-bit IDs, identical to the joint harness
    // (`AXI_TYPEDEF_*` block of nonblock_banked_top.sv:305-311).
    // Master-side: 4 + $clog2(2) = 5-bit IDs, as axi_mux prepends the port bit
    // (same convention as banked_ctl's comp_*_wide_t, banked_ctl.sv:199-211).
    `AXI_TYPEDEF_AW_CHAN_T(ds_aw_chan_t, logic [63:0], logic [4:0], logic [0:0])
    `AXI_TYPEDEF_W_CHAN_T (ds_w_chan_t,  logic [63:0], logic [7:0], logic [0:0])
    `AXI_TYPEDEF_B_CHAN_T (ds_b_chan_t,  logic [4:0],  logic [0:0])
    `AXI_TYPEDEF_AR_CHAN_T(ds_ar_chan_t, logic [63:0], logic [4:0], logic [0:0])
    `AXI_TYPEDEF_R_CHAN_T (ds_r_chan_t,  logic [63:0], logic [4:0], logic [0:0])
    `AXI_TYPEDEF_REQ_T (ds_req_t,  ds_aw_chan_t, ds_w_chan_t, ds_ar_chan_t)
    `AXI_TYPEDEF_RESP_T(ds_resp_t, ds_b_chan_t,  ds_r_chan_t)

    axi_req_t  [1:0] ds_slv_req;
    axi_resp_t [1:0] ds_slv_resp;
    ds_req_t         ds_mst_req;
    ds_resp_t        ds_mst_resp;

    // the harness drives the DUT's walker ports from axi_*_req/resp wires
    assign ds_slv_req[0] = ddtw_axi_req;      // walker 0 = DDTW, id[4] = 0
    assign ds_slv_req[1] = ptw_axi_req;       // walker 1 = PTW,  id[4] = 1
    assign ddtw_axi_resp = ds_slv_resp[0];
    assign ptw_axi_resp  = ds_slv_resp[1];

    axi_mux #(
        .SlvAxiIDWidth (4),
        .slv_aw_chan_t (axi_aw_chan_t), .mst_aw_chan_t (ds_aw_chan_t),
        .w_chan_t      (axi_w_chan_t),
        .slv_b_chan_t  (axi_b_chan_t),  .mst_b_chan_t  (ds_b_chan_t),
        .slv_ar_chan_t (axi_ar_chan_t), .mst_ar_chan_t (ds_ar_chan_t),
        .slv_r_chan_t  (axi_r_chan_t),  .mst_r_chan_t  (ds_r_chan_t),
        .slv_req_t     (axi_req_t),     .slv_resp_t    (axi_resp_t),
        .mst_req_t     (ds_req_t),      .mst_resp_t    (ds_resp_t),
        .NoSlvPorts    (32'd2),
        .MaxWTrans     (32'd4),
        .FallThrough   (1'b0),
        .SpillAw(1'b0), .SpillW(1'b0), .SpillB(1'b0), .SpillAr(1'b0), .SpillR(1'b0)
    ) i_ds_mux (
        .clk_i, .rst_ni, .test_i(1'b0),
        .slv_reqs_i  (ds_slv_req),
        .slv_resps_o (ds_slv_resp),
        .mst_req_o   (ds_mst_req),
        .mst_resp_i  (ds_mst_resp)
    );

    // ds_mst_req.ar -> ds_ar_* output, ds_ar_ready from input,
    // ds_r_* inputs   -> ds_mst_resp.r    (AW/W/B left idle: walkers never write)
    // msiptw/pdtw keep their own tie-offs exactly as today
    // (nonblock_banked_top.sv:672-677), or are merged later if needed.
endmodule
```

**driver 增量（`tests/nonblock_banked_shared_test.cpp`，以 §4 的 driver 为基础）**

- 把 `ddtw_rd` / `ptw_rd` 两个 `AxiReadState`（`cpp:64`）与 `drive_ddtw_r`/`drive_ptw_r`（`cpp:270-304`）合并为单个 `DsRead ds_rd` + `reads_by_id`：
  ```cpp
  struct DsRead { bool pending=false; uint64_t addr=0; uint8_t len=0,size=0,id=0; int beat=0; };
  DsRead ds_rd;
  uint64_t ds_ar_count[2] = {0,0};      // [id[4]]  DDTW / PTW AR fires
  uint64_t ds_r_count[2] = {0,0};
  ...
  top->ds_ar_ready = ds_rd.pending ? 0 : 1;
  bool ds_ar_fire = top->ds_ar_valid && top->ds_ar_ready;
  if (ds_ar_fire) {
      CHECK_AT(!ds_rd.pending, "shared DS AR while a read is still pending");
      ++ds_ar_count[(top->ds_ar_id >> 4) & 1];
      ds_rd.pending = true; ds_rd.addr = top->ds_ar_addr; ds_rd.len = top->ds_ar_len;
      ds_rd.size = top->ds_ar_size; ds_rd.id = top->ds_ar_id; ds_rd.beat = 0;
  }
  // R: identical math to the current drive_ptw_r(), but id = ds_rd.id (echo)
  ```
  主机会证明：**两个 walker 的 AR 现在争用同一个 AR/R 通道**（`ds_ar_count[0]>0 && ds_ar_count[1]>0`，可作为机制 guard）。
- 其余（`mp_*` 场景、`deadline`、`stuck dump`）完全复用 §4。
- 局限说明（写进注释）：该 harness 仍不是 `rv_iommu_top`（没有 CQ/FQ/PQ/MSI-IG 请求者、没有 XBAR、没有 ID remap）；`axi_mux` 的 `Spill*=0` 意味着 R 突发不可被跨端口 interleave——**这是 harness 约定，不声称等于板级 `rv_iommu_ds_if` 的 ID 编码**。

**运行入口增量（`tests/test_nonblock_banked.py`，最小）**

```python
TOP = "nonblock_banked_top"

VARIANTS = {
    "joint":  ("nonblock_banked_top",        "nonblock_banked_test.cpp"),
    "shared": ("nonblock_banked_shared_top", "nonblock_banked_shared_test.cpp"),
}

def main(variant="joint") -> int:
    top_module, tb_name = VARIANTS[variant]
    tb = ROOT / "tests" / tb_name
    ...
    args = ["verilator", "--cc", "--exe", "--build", "-Wno-fatal",
            "+define+ASSERTS_OFF", "--top-module", top_module,
            "--Mdir", str(OUT / "obj-" + variant), ...]
    # harness source: replace ROOT/tests/nonblock_banked_top.sv by the variant .sv
```
（并在 `build_arg_parser()` 加 `--variant {joint,shared}`，默认 `joint`，保证旧行为不变。）

---

## 6. runner 最小机制验收 guard（`scripts/mac_nonblock_pilot.py`）

### 6.1 为什么必须加（只解码 1 个已提供 frame）

`out/n-prototype-20261004/serial-a8-live-evidence.log:16-19` 的 base64 帧解码为（我的解码，非硬件重取）：

```json
{"schema":"idma-nonblock-window-v1","mode":1,"windowseq":2,"cycles":86610,
 "fastcounts":[0,0,0,0,0],"normalcounts":[0,0,0],
 "words":[5279429606066244609,1,1,4,2,86610,0,0,0,0,0,0,0,0,0,0]}
```

- 字段语义与 runner 一致：`words[0]` magic、`[1]` version=1、`[2]` mode=1、`[3]` flags=4、`[4]` windowseq、`[5]` cycles；`[6..10]` = fastcounts(Vread,Vwrite,Aread,Awrite,Other)、`[11..13]` = normalcounts(V,A,Other)、`[14]` = accepted、`[15]` = errors（`mac_nonblock_pilot.py:126-131,436-456`）。
- 该帧 `windowseq=2` / `cycles=86610` 与**提供的** `short-a1` 记录吻合（`out/…/cases/short-a1/record.json:1` 的 `"sequence":2`、`"cycles":86610`），据此可归属到 `short-a1`（这是按数值匹配的推断，不是重算旧数据）。
- 现有检查在该帧上**全部通过**：`window[15]==0`、`window[14]==fast+normal`（0==0）、`window[5]==p4m.raw_window[1]` ⇒ 现有规则会把一个**全 0 机制窗口**判为通过。这正是需要补的洞。

### 6.2 调用点（`:1293-1298`）

```python
        if window is None:
            problems.append('helper window is missing')
        else:
            expect_v, expect_a = helper_mechanism_expectations(record)
            window_problems = validate_helper_window(
                window, self.args.label, expected_cycles, self.last_helper_seq,
                expect_v_movement=expect_v, expect_a=expect_a)
```

### 6.3 新增辅助函数（放在 `validate_helper_window` 之前）

```python
def helper_mechanism_expectations(record: Any) -> Tuple[bool, bool]:
    '''Which mechanism counters the helper window must show as non-zero.

    A normal P4M case that ran victim transfers moved V, so the helper must
    have counted at least one V accept; a case whose window reports has_a must
    likewise show at least one A accept.  When the record cannot be read the V
    guard stays armed, so an all-zero window can never be reported as a pass.
    '''
    expect_v = True
    expect_a = False
    if isinstance(record, dict):
        inp = record.get('input')
        if isinstance(inp, dict):
            transfers = inp.get('victim_transfers')
            expect_v = (isinstance(transfers, int)
                        and not isinstance(transfers, bool)
                        and transfers > 0)
        p4m = record.get('p4m')
        if isinstance(p4m, dict):
            expect_a = bool(p4m.get('has_a'))
    return expect_v, expect_a
```

### 6.4 `validate_helper_window` 完整替换（`:423-460`；只加 guard，不动已有检查、不动 raw）

```python
def validate_helper_window(window: Any, label: str, expected_cycles: Any,
                           previous_seq: Optional[int],
                           expect_v_movement: bool = True,
                           expect_a: bool = False) -> List[str]:
    '''Validate the static-ELF helper window (ABI idma-nonblock-window-v1).

    The full 16-word window is kept verbatim elsewhere; nothing here is
    reinterpreted as an HPM counter.

    Mechanism liveness guards (additive, minimal): an all-zero window satisfies
    `window[14] == fast + normal` trivially, so a run in which nothing was
    counted at all would otherwise be accepted as a pass.  When the case moved V
    (and, if it reported an A aggressor, A) the accepted total and the matching
    fast+normal sums must be strictly positive.  The guards only read the
    sampled words; they never change them and never rewrite the raw record.
    '''
    problems: List[str] = []
    if not isinstance(window, list) or len(window) != READER_WINDOW_WORDS:
        return ['helper window is not ' + str(READER_WINDOW_WORDS) + ' words']
    if any(isinstance(value, bool) or not isinstance(value, int)
           for value in window):
        return ['helper window contains non-integer words']
    if window[0] != READER_MAGIC:
        problems.append('helper window[0] magic ' + repr(window[0]))
    if window[1] != READER_VERSION:
        problems.append('helper window[1] version ' + repr(window[1]))
    expected_mode = READER_MODE_FOR_LABEL.get(label)
    if window[2] != expected_mode:
        problems.append('helper window[2] mode ' + repr(window[2])
                        + ' != ' + repr(expected_mode) + ' for ' + label)
    if window[3] != READER_FLAGS_FROZEN:
        problems.append('helper window[3] flags ' + repr(window[3])
                        + ' != ' + str(READER_FLAGS_FROZEN))
    if window[5] != expected_cycles:
        problems.append('helper window[5] cycles ' + repr(window[5])
                        + ' != p4m.raw_window[1] ' + repr(expected_cycles))
    if window[15] != READER_ERRORS_OK:
        problems.append('helper window[15] errors ' + repr(window[15]))
    fast = sum(window[i] for i in READER_FAST_WORDS)
    normal = sum(window[i] for i in READER_NORMAL_WORDS)
    if window[14] != fast + normal:
        problems.append('helper accepted ' + repr(window[14]) + ' != fast '
                        + repr(fast) + ' + normal ' + repr(normal))

    # --- mechanism liveness guards (added) --------------------------------
    # V fast words: 6=Vread, 7=Vwrite; V normal word: 11  (lines 130-131)
    # A fast words: 8=Aread, 9=Awrite; A normal word: 12  (lines 130-131)
    if expect_v_movement or expect_a:
        if window[14] <= 0:
            problems.append('helper window[14] accepted is not > 0 although '
                            'the case ran V/A transfers')
    if expect_v_movement:
        v_counts = (window[6], window[7], window[11])
        if sum(v_counts) <= 0:
            problems.append('helper V fast+normal counts ' + repr(v_counts)
                            + ' are not sum > 0 although the case ran V transfers')
    if expect_a:
        a_counts = (window[8], window[9], window[12])
        if sum(a_counts) <= 0:
            problems.append('helper A fast+normal counts ' + repr(a_counts)
                            + ' are not sum > 0 although an A aggressor ran')

    if previous_seq is not None and window[4] <= previous_seq:
        problems.append('helper window[4] sequence ' + repr(window[4])
                        + ' did not increase past ' + repr(previous_seq))
    return problems
```

**对提供的 `short-a1` 帧的预期效果**（可离线单测，不需要硬件）：`expect_v=True, expect_a=True` ⇒ 新增 2 条问题（`V fast+normal … not sum > 0`、`A fast+normal … not sum > 0`）外加 `window[14] … not > 0`；未加 guard 时返回 0 条问题。**不改动** `helper-window.json` 里保存的 16 词原始窗口（`:1301-1309` 原样保存）。

可选（**超出最小改动，供 Codex 取舍**）：在 `validate_record` 里要求 `p4m.has_a` 时 `a_completed_jobs_window > 0`（`:401-404` 附近）——我没有写进去，以保持"仅加必要 guard"。

---

## 7. 状态声明与执行顺序

1. **未执行、未编译、未在硬件上跑**；本提案不声称任何 PASS。joint TB 的新用例、共享 DS harness、runner guard 都必须由 Codex 编译/运行后才能被称为通过。
2. **A8**：仅时间线证据（`serial-a8-live-evidence.log:34-38`，`:37` a1 completed 21:54:11，`:38` a8 start 21:54:11 后无完成帧）；host 侧 `--case-timeout` 默认 600 s（`mac_nonblock_pilot.py:97,1617`，`run_shell` 调用 `:1376-1378`），guest CLI 自身 `--timeout-ms 60000`（`:104,255`）。后续 capture/export 不响应与"逐文件 try/except + `capture_error`"路径（`:1326-1327,1384-1386,1465`）一致。**结论：A8 不能记为通过，且提供的证据不足以定根**（候选域见 §3.4）。
3. **monitor 作用域 bug**（`ariane_peripherals_xilinx.sv:914,930-934` vs `:1274,1381-1382`）只解释 accepted/fast/did/type 恒 0，**不解释 A8 停滞**；修复归 Codex。
4. **顺序**：Codex 修接线 → 重编两个 BIT（timing 已 pass、功能失败，必须重编）→ 才允许任何新 BIT 运行；`banked-hit`（未测 Hit）暂不跑。
5. 最终由 Codex 核对的高风险项：`ariane_soc::IdWidth` 是否为 4（决定 `BankIdBit` 与 TB 等价性）；完成口 ID 回显假设（`banked_ctl:25-31`）；`rv_iommu_nonblock_hit.sv` 的 enable 门控；板级 banked 模式下 `rv_iommu_e3_delay` 的 passthrough 行为。