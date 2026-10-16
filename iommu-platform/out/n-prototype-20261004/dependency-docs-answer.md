以下全部是**提案与证据整理**；本次未执行 DS，未修改任何源文件，未在 FPGA 快照/构建期间应用任何变更。下文所有“PASS/FAIL”只复述已有日志，不代表我方新跑过测试，也不构成板级、DDR、真实 DMA、新颖性或性能结论。

---

## 0. 导航

1. 新文档完整稿：`docs/nonblock-dependency-diagnosis-20261004.md`
2. 窄范围 diff 建议
   - `docs/nonblock-pilot-plan-20261004.md`
   - `docs/research-n-prototype-spec-20261004.md`
3. 前端 `BATCH_PORT` 行为证据
4. 后端 outstanding metadata / 共享源 W 宽度证据
5. 若需 capacity corner 的小范围追加测试提案
6. 未决项与 Codex 需裁决点

---

## 1. 新文档完整稿

路径建议：`docs/nonblock-dependency-diagnosis-20261004.md`

````markdown
# Nonblock 依赖诊断（2026-10-04，v1 读接纳守卫，模型范围）

状态：本文件只记录被提供日志与源码能够支持的 scoped C++ 模型诊断。
不覆盖真实 DDR、真实 DMA、板级 A8 根因、全互连行为，也不宣布新颖性、
性能增益或最终通过。所有“36/36 通过”“12/12 通过”仅指已有 scoped
模型日志中的场景与 order。

---

## 1. 一句话摘要

v1 读接纳守卫把此前 banked scoped 模型的 8/12 watchdog 失败压到 0/12：
`coupled-banked-guard2-vv.log:1-26` 为 12 场景通过，
`coupled-extra-orders.json:1-9` 记录 `aa`/`av` 额外 order 退出 0，
legacy 同 order 也通过：`coupled-legacy-vv.log:1-26`。
但这是模型内读写依赖与共享 R/W 观察，不是真实 DDR/DMA 结论。
科学含义上，N 的普通 baseline 若也加同类读接纳守卫，可能减少读绕过/
前向机会，因此 guard 不能直接算作 N 的增益。

---

## 2. 证据清单

### 2.1 状态与结果

- `out/n-prototype-20261004/v1-work-state.json:1-23`
  - `status = two_fpga_builds_running_not_board_accepted`
  - `original_board = 15/15 complete data_errors0 taint0`
  - `core_change = Codex read-admission guard v1; legacy default off; signed AW-minus-WLAST plus visibleAW reservation and simultaneousAW acceptance priority`
  - `actual_checks`：`legacy_control_vv = 12`，`banked_fixed_vv_aa_av = 36`，`joint = PASS`，`banked_ctl = PASS`，`helper_guard = PASS`
  - `not_proven`：板级 hang root 精确等于模型、所有互连/DDR、通用跨流数据依赖、N gain/novelty
- `out/n-prototype-20261004/coupled-banked-guard2-vv.log:1-26`
  - v1 banked guard 的 `vv` 12 场景全部打印 `COUPLED-SUMMARY` 并以 `PASS (12 scenarios)` 结束
- `out/n-prototype-20261004/coupled-legacy-vv.log:1-26`
  - legacy 同 order `vv` 12 场景通过
- `out/n-prototype-20261004/coupled-extra-orders.json:1-9`
  - `aa`、`av` 均 `exit: 0`
- `out/n-prototype-20261004/coupled-banked-vv.log:1-122`
  - 旧 banked 版本在 `mode1-a1-l1` 起出现 watchdog STALL
  - 第二次 `vv` 运行出现观察/打印缺陷：`coupled-banked-vv.log:105-122` 重复 `+order=vv`，a1/a8 场景只打印标题，随后直接 `8/12 scenarios FAILED`
- `out/n-prototype-20261004/joint-after-readguard.log:1-10`
  - joint 数据通路 smoke 场景 `PASS`
- `out/n-prototype-20261004/helper-nonzero-guard.log:1-2`
  - runner/analyzer 的 zero/V/dual counter guard `PASS`

### 2.2 关键源码

- `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv`
  - `:244-253`：输入 demux 在 W LAST 前保持 W ownership；跨 bank 读会填满 DMA 的 R->W FIFO，而 AW 不能进入 demux，导致共享内存 R 无法返回当前 owner 的 source data 或 page-table reads，形成 circular wait。守卫选择是延迟“选择”读，而不是撤回 AXI/TL VALID。
  - `:254-266`：`w_unfilled_q` signed balance、`w_owner_q`、`restrict_reads`、`read_owner`、`bank_read_admit[0/1]`。新 AW 还通过 `tr_axi_req_i.aw_valid` 保留 admission；同一周期 AW handshake 进入某 bank 时，用 `!(aw_accept && aw_sel)` 阻止同周期选择旧 AR。
  - `:259-262` 注释明确：新 AW 进入 bank 后，它的 FIFO head 下一周期才可见；不要提前一个周期选择旧 queued AR。
  - `:312-354`：两个 bank 仍实例化 `rv_iommu_trans_ctl_wrap`，但 `ReadAdmissionEnable(1'b1)` 且接入 `read_admit_i`。
  - `:542-582`：`in_flight_o` 聚合 AW/AR/W-LAST/bank busy，不是只看查表器。
- `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`
  - `:23-25`：`ReadAdmissionEnable` 是 banked DMA 集成的新约束，legacy 默认关闭。
  - `:61`：`read_admit_i` 输入。
  - `:621-666`：IDLE 中 AW 优先于 AR；读只在 `(!ReadAdmissionEnable || read_admit_i)` 时进入 `REQ_TRANS`。
  - `:713-723`：COMPLETE 中只有读完成才释放 AR，写完成才释放 AW。
- `out/n-prototype-20261004/coupled-banked-vv.log:1-13`
  - 旧 banked watchdog 的可观察状态。

---

## 3. 两个不同的模型循环

以下两个循环是 scoped C++ 模型中的依赖图；不是实际 DDR/DMA 根因。
日志没有直接打印 A/V 标签，A/V 映射按任务描述与端口/队列状态给出，
因此这里保留“模型解释”标签。

### 3.1 循环一：head A data waits V W

证据：

- `coupled-banked-vv.log:2`
  - `sharedR.head port=0 id=4 addr=0x0000000090000000 len=255 beat=5 latency=0`
- `coupled-banked-vv.log:3`
  - `DMA0 used=1 done=0 half=0 ar_want=0 ar_done=1 aw_want=0 aw_done=1 rx=0 wx=0 r_last=0 b_done=0 cfifo=0`
- `coupled-banked-vv.log:4`
  - `DMA1 used=1 done=0 half=0 ar_want=0 ar_done=1 aw_want=1 aw_done=0 rx=3 wx=0 r_last=0 b_done=0 cfifo=3`
- `coupled-banked-vv.log:5`
  - `caw_q.head id=0 addr=0x0000000080001000 len=255 got=0`
- `coupled-banked-vv.log:9`
  - `AW id=0 addr=0x0000000010001000 len=255`
- `coupled-banked-vv.log:12`
  - `mem_q=2 caw_q=1 cb_q=0 exp_aw_q=0 aw_order=1 w_owner=0 in_flight=1 nonblock_busy=0 nonblock_fire=0`

模型解释：

A 的读数据已经在 `sharedR.head` 上有一个可返回 beat（`beat=5`），但
DMA0 的 `r_last=0`、`b_done=0`；DMA1 侧 `aw_want=1 aw_done=0`、
`cfifo=3`。V 方向仍持有/等待 W owner 的释放，而 A 的读完成依赖
shared R 继续返回后续 beat。该循环可概括为：
“A 的 head 读数据等待 V 的 W owner 释放；V 的 W drain 又未完成”。

这是一个模型级 circular wait，说明“同 ID 同银行 + AW/W 关联”本身
不足以排除共享 R/W 依赖；它不证明真实 DDR 控制器会这样。

### 3.2 循环二：own source A waits its cold AW because AW arrives one cycle later

证据：

- `rv_iommu_banked_ctl.sv:244-253`
  - W ownership 在 LAST 前保持；跨 bank source read 可能填满 DMA 的 R->W FIFO，而 AW 不能进入该 demux。
- `rv_iommu_banked_ctl.sv:257-266`
  - `restrict_reads`、`read_owner`、`bank_read_admit` 以及 `!(aw_accept && aw_sel)` 的组合。
- `rv_iommu_banked_ctl.sv:259-262`
  - 新 AW 进入 bank 后，AW FIFO head 下一周期才可见；守卫要避免提前一个周期选择旧 queued AR。
- `rv_iommu_trans_ctl_wrap.sv:621-666`
  - IDLE 中 AW 先于 AR；读是否进入 `REQ_TRANS` 取决于 `read_admit_i`。
- `rv_iommu_trans_ctl_wrap.sv:713-723`
  - COMPLETE 中读响应/写响应分别释放 AR/AW。

模型解释：

A 自己的源读要完成，可能需要 A 自己的“冷 AW”先建立 W drain。
但 A 的 AW 刚被接受到 bank 时，其 FIFO head 下一周期才对 bank 控制器
可见；在同一周期，`bank_read_admit` 可能还不允许选择 A 的旧 AR。
当共享 R/W 同时受压时，这“一个周期”的可见性差会与跨 bank W owner
叠加成第二个循环：
“own source A read 等待自己的 cold AW；cold AW 因 FIFO head latency
晚一个周期可见/可优先”。

该循环的直接源码依据是 `rv_iommu_banked_ctl.sv:259-266` 和
`rv_iommu_trans_ctl_wrap.sv:621-666`；日志里的
`coupled-banked-vv.log:7` `up_aw=yes up_w=(none)`、
`:12` `aw_order=1 w_owner=0` 可作为“AW 已可见但 W 未推进”的模型
状态旁证。它同样不是真实 DDR/DMA 结论。

---

## 4. 已证实、未证实与观察线网问题

### 4.1 已证实（仅限 scoped 模型/已有日志）

- v1 guard 的 banked scoped 模型：`coupled-banked-guard2-vv.log:1-26` 12 场景 PASS。
- legacy 同 order：`coupled-legacy-vv.log:1-26` 12 场景 PASS。
- 额外 order：`coupled-extra-orders.json:1-9` `aa`/`av` exit 0。
- 旧 banked scoped 模型：`coupled-banked-vv.log:1-13` 出现 watchdog STALL。
- joint/helper smoke：`joint-after-readguard.log:1-10`、`helper-nonzero-guard.log:1-2`。

### 4.2 已证实的观察/打印线网问题

`coupled-banked-vv.log:105-122` 的第二次 `vv` 运行与第一次
`:1-104` 不同：它重复 `+order=vv`，a1/a8 场景只打印标题，没有
`COUPLED-SUMMARY` 或 `STALL` 细节，却直接输出
`8/12 scenarios FAILED`。这证明的是观察/打印/harness 线网存在
缺失或复用问题，不足以据此生成新的 RTL 根因。

同样，`sharedR.head` 是模型观察线，不是真实 DDR 探针；
不能把它单点当作板级 A8 根因。

### 4.3 板级 A8 根因：未知

`v1-work-state.json:5` 只记录恢复后的 boot/kernel/taint：
`956fa8d0-... kernel00093 taint0`。
`v1-work-state.json:16-20` 明确列出未证明：
- 板级 hang root 精确等于模型
- 所有互连/DDR 行为
- 通用跨流数据依赖
- N gain 或 novelty

没有板级 A8 重跑样本，因此不能把两个模型循环直接写成板级 A8 根因。

### 4.4 不宣称

- 不宣称真实 DDR/真实 DMA 行为。
- 不宣称板级通过。
- 不宣称性能增益。
- 不宣称新颖性。
- 不宣称完整 root HANDOFF 已读；本文件只基于提供日志与源码片段。

---

## 5. 科学含义：N 的普通 baseline 与 bypass 机会

`docs/research-n-prototype-spec-20261004.md:51-53` 要求 fair baseline
使用相同 replacement 更新规则；真实被服务 hit 必须参与既定替换策略。
`:99-109` 又说明两银行原型只是前人已知有限 hit-under-miss 的基线，
不能把静态银行改名成额外保护方法，必须公平对照找剩余瓶颈。

读接纳守卫的逻辑是：在 W owner 未释放或新 AW 可见前，延迟新读的
“选择”。这避免了一类 circular wait，但同时会减少读在写未完成前
绕过/前向的机会。因此：

- v1 guard 的模型通过不能直接解释为 N 的收益。
- 若 N 的普通 baseline 也加同类 guard，N 可展示的 bypass 机会可能减少。
- 公平比较必须保持每类容量、仲裁/替换规则相同，并把 guard 作为
  baseline 成本计入。
- N 的“前进”必须来自真实被服务的合法 hit，而不是只读 probe 或
  guard 下的延迟选择。
````

---

## 2. 窄范围 diff 建议

以下 diff 只改当前 v1 状态/约束，不删除历史 v0 记录。提案，未应用。

### 2.1 `docs/nonblock-pilot-plan-20261004.md`

#### 2.1.1 `CURRENT_STATUS` 顶部段替换建议

当前顶部为 `docs/nonblock-pilot-plan-20261004.md:1-6`：

```markdown
# Nonblock engineering pilot — frozen run plan (2026-10-04)

Status (2026-10-04): original-serial **15/15 completed**, no data errors, taint=0.
The two banked FPGA versions are still compiling; no banked hardware result or
novelty claim is made. The original boot required one documented startup-script
interrupt before any experiment began.
```

建议替换为：

```markdown
# Nonblock engineering pilot — frozen run plan (2026-10-04)

CURRENT_STATUS (2026-10-04, v1 update; v0 history preserved below)
- original-serial: **15/15 completed**, data_errors=0, taint=0.
- two FPGA v1 builds: running, not board accepted; no SD update, no original rerun.
- scoped model status: 36 coupled scenarios over 3 orders PASS; legacy 12 matching
  order PASS; previous banked v0 model 8/12 FAIL. The v1 read-admission guard is a
  scoped model guard, not a board/real-DDR/real-DMA proof, and not a novelty or
  performance claim.
- board A8 hang root: unknown. Only restored boot/kernel/taint is recorded
  (`out/n-prototype-20261004/v1-work-state.json:5,16-20`).
- historic v0 record (original serial pilot and previous banked failures) remains
  valid below and must not be deleted.
```

#### 2.1.2 短追加条目

建议追加在 `docs/nonblock-pilot-plan-20261004.md:87-89` 之后，不改写上方 v0 记录：

```markdown
## 2026-10-04 v1 读接纳守卫模型状态（追加，不改写 v0）

- v1 guard 后 banked scoped 模型：`out/n-prototype-20261004/coupled-banked-guard2-vv.log:1-26`
  12 场景 PASS；legacy 同 order `out/n-prototype-20261004/coupled-legacy-vv.log:1-26`
  12 场景 PASS；额外 order `out/n-prototype-20261004/coupled-extra-orders.json:1-9`
  `aa`/`av` exit 0。
- 旧 banked v0 模型：`out/n-prototype-20261004/coupled-banked-vv.log:1-13`
  出现 watchdog STALL，`sharedR.head` 的 A 读与 V W owner 形成模型循环。
- 上述只是 scoped C++ 模型诊断，不是真实 DDR/DMA，不是板级 A8 根因，
  不是性能/新颖性结论。
- 读接纳守卫的约束：只延迟新读选择，不撤回 AXI/TL VALID；新 AW 进入
  bank 后 FIFO head 下一周期才可见，因此 AW admission priority 必须计入
  CLK 与 bank FIFO head latency（`rv_iommu_banked_ctl.sv:244-266`）。
- 旧“银行未修改”“AW/W 关联本身已足够”的说法已被 superseded。
```

### 2.2 `docs/research-n-prototype-spec-20261004.md`

建议在 `:99-109` 的“实现更新：两银行生命周期”段做窄替换/插入，不删除历史 v0：

```diff
@@ docs/research-n-prototype-spec-20261004.md
-先前“仍缺身份/出队方案”是草案状态。当前选定两银行方案：按原始 AXI ID 的 DMA 来源位固定分银行，每个银行沿用原控制器并保留自己的完整 FIFO 头；一银行至多一笔翻译在途，响应携带银行号回到自己的头。总 AW/AR 槽与 W FIFO 深度均分，缓存与查表器仍各一份。输出 AXI mux 临时增加银行位，外部仍使用原 ID，响应按原来源位还原；同 ID 始终在同银行，AW/W 由现有 demux/mux 保持关联。
+先前“仍缺身份/出队方案”是草案状态。当前选定两银行方案：按原始 AXI ID 的 DMA 来源位固定分银行，每个银行保留自己的完整 FIFO 头；一银行至多一笔翻译在途，响应携带银行号回到自己的头。总 AW/AR 槽与 W FIFO 深度均分。**银行不是“未修改”**：v1 banked_ctl 在输入 demux 后增加 read_admit_i 守卫，并在新 AW 可见/进入 bank FIFO head 前优先保留写方向（`rv_iommu_banked_ctl.sv:244-266`）。该守卫只延迟新读的选择，不撤回 AXI/TL VALID；AW admission priority 必须计入 CLK 与 bank FIFO head 可见性的一周期 latency。旧的“同 ID 同银行 + 现有 AW/W 关联已足够”已被 `coupled-banked-vv.log:1-13` 的 8/12 watchdog 失败推翻；AW/W 关联本身不足以排除共享 R/W 循环。
@@ docs/research-n-prototype-spec-20261004.md
-首轮真实 TL 仿真及旧 IOTLB 行为对照已通过；追加测试与联合数据通路验证仍未完成，无新板测/性能结论。
+首轮真实 TL 仿真及旧 IOTLB 行为对照已通过。v1 读接纳守卫后的 scoped coupled 模型：36/36 通过（3 orders），legacy 12/12 同 order 通过，但这是模型观察，不是板级、DDR 或性能结论。追加测试与联合数据通路验证仍未完成；板级 A8 根因仍未知。
```

---

## 3. 前端证据：`BATCH_PORT` 到底会不会流式排队多个 job？

结论：**当前 `idma_reg64_frontend.sv` 的 `BATCH_PORT` 不会一次流式发最多 8 个 queued jobs；它在每个 job 发出后进入 `WAIT`，等 `trans_complete_i` 后才发下一个。** `JobFifoDepth` 存在，但不由该 FSM 单独填满到 8。

证据：

- `idma_reg64_frontend.sv:143`
  - `batch_active_o = BatchPort && batch_enable_q && state_q != IDLE`
- `idma_reg64_frontend.sv:151-152`
  - `batch_task_issue = batch_active_o && state_q == COUNT && ready_i`
  - `batch_task_done = batch_active_o && state_q == WAIT && trans_complete_i`
- `idma_reg64_frontend.sv:483-509`
  - `next_id.re` 启动 batch；若 `batch_enable_q`，设置 `transf_counter_n = batch_target_q`，进入 `ADMIT` 或 `COUNT`。
  - 若 `batch_enable_q && !ready_i`，`issue = 0`，`state_n = IDLE`。
- `idma_reg64_frontend.sv:512-535`
  - `COUNT`：`valid_o = 1`；`burst_req_o.src_addr/dst_addr` 使用当前 `transf_offset_q`；若 `ready_i`，进入 `WAIT` 并递减 counter。
- `idma_reg64_frontend.sv:537-563`
  - `WAIT`：必须等 `trans_complete_i`。非最后一笔时，可能进入 `GAP`、`ADMIT` 或 `COUNT`；最后一笔才 `IDLE`。
- `dma_core_wrap.sv:170-187`
  - frontend 和 backend 之间有 `stream_fifo_jobs_oned`，深度 `JobFifoDepth`。
  - 但由于 frontend 在 `WAIT` 等 `trans_complete_i`，它不会在 `JobFifoDepth` 里连续压入下一 job；`JobFifoDepth` 不是被 `BATCH_PORT` 自动填满的 outstanding 队列。
- `dma_core_wrap.sv:190-191`
  - `be_trans_complete = idma_rsp_valid`；`idma_rsp_ready = 1'b1`。frontend 的 `trans_complete_i` 就是 backend response valid。

因此，若现有 coupled 模型是“one job per DMA / two serialized halves”，它与 `BATCH_PORT` 前端 job 级行为一致：**job 级串行**。但后端内部仍可能有多个 AXI burst/metadata 在飞，见下一节。

---

## 4. 后端 outstanding metadata / 共享源 W 宽度

### 4.1 后端 outstanding 元数据

- `idma_backend.sv:24`
  - `NumAxInFlight`
- `idma_backend.sv:112`
  - `MetaFifoDepth = BufferDepth + NumAxInFlight + MemSysDepth`
- `idma_backend.sv:554-570`
  - `i_r_dp_req`：读 datapath request FIFO，深度 `NumAxInFlight`
- `idma_backend.sv:590-608`
  - `i_w_dp_req`：写 datapath request FIFO，深度 `NumAxInFlight`
- `idma_backend.sv:633-649`
  - `i_w_last`：last flag FIFO，深度 `MetaFifoDepth`
- `idma_backend.sv:813-836`
  - RAWCouplingAvail 时 `idma_channel_coupler` 也带 `NumAxInFlight`

结论：一个 DMA backend 的 outstanding 能力由 `NumAxInFlight`、`BufferDepth`、`MemSysDepth` 共同决定；metadata/last flag 深度是三者之和。具体到 CVA6 集成，`dma_core_wrap` 把 `AxiDataWidth` 和 `NumAxInFlight` 传给 `idma_backend`：`dma_core_wrap.sv:287-309`。

### 4.2 共享源 W 宽度

- `idma_backend.sv:16-18`
  - `DataWidth`
- `idma_backend.sv:104-107`
  - `ttype_i`、`wdata_i`、`rdata_o`、`rdata_valid_o`
- `dma_core_wrap.sv:288-289`
  - `.DataWidth(AxiDataWidth)`、`.AddrWidth(AxiAddrWidth)`
- `rv_iommu_banked_ctl.sv:160-163`
  - `NumOutstandingTrans : RVIOMMUCfg.NumOutstandingTrans / 2`
  - `WFifoDepth : RVIOMMUCfg.WFifoDepth / 2`
- `rv_iommu_banked_ctl.sv:147-152`
  - `NumOutstandingTrans` 必须偶数且 >=2；`WFifoDepth` 必须偶数且 >=2。
- `rv_iommu_banked_ctl.sv:220-238`
  - 每个 bank 自己的 W FIFO 使用 `RVIOMMUCfg.WFifoDepth/2`。

结论：共享源/目的通路的 W 数据宽度由参数 `AxiDataWidth` 决定，不由日志或 banked wrapper 硬编码。日志 `coupled-banked-vv.log:2` 显示 `len=255`，即 256 beats；若总长 4 KiB，则隐含 16 B/beat，但 `coupled-banked-vv.log:10` 只打印了 `0x0ea1877cb8c2b452`（64-bit 展示）。因此**仅凭日志不能确定实际 W 宽度**；需要查集成参数或 TB 的 `AxiDataWidth`。这是一个不确定性。

---

## 5. 若 Codex 判定需要 capacity corner 的追加测试（提案，未运行）

只在 Codex 认为当前 coupled 模型缺少多 in-flight/capacity corner 时使用。不要重写 joint/core，不要改 RTL，不要声明未跑通的 pass。

### 5.1 方案 A：复用现有 C++ job queue 模型

建议追加到现有 `nonblock_coupled_test` 的 scenario 表，或新建一个独立小测试。下面代码是提案骨架，因未提供现有 C++ harness 头文件，`TODO(adapter)` 处需按实际 API 替换。

```cpp
// 提案文件：out/n-prototype-20261004/coupled_multi_inflight_capacity_test.cpp
// 或作为现有 nonblock_coupled_test 的一个追加 scenario。
// 未编译、未运行；仅 capacity corner，不重写 joint/core。

#include <cstdint>
#include <string>

struct Job {
  int dma;
  std::uint64_t src;
  std::uint64_t dst;
  unsigned len;
  bool write;
};

static void run_multi_inflight_capacity(NonblockCoupledModel& m, Report& rep) {
  // TODO(adapter): 按现有 harness 的实际方法名替换。
  m.reset();
  m.set_runtime_enable(true);          // v1 guard on
  m.set_read_admission_enable(true);
  m.set_ptw_latency(1);
  m.set_gap_cycles(0);
  m.set_job_queue_capacity(8);         // 只用于模型 capacity corner

  constexpr int kDma = 2;
  constexpr int kJobsPerDma = 4;

  for (int d = 0; d < kDma; ++d) {
    for (int j = 0; j < kJobsPerDma; ++j) {
      Job job;
      job.dma = d;
      job.src = 0x90000000ull + (std::uint64_t)d * 0x10000ull
                              + (std::uint64_t)j * 0x1000ull;
      job.dst = 0x80001000ull + (std::uint64_t)d * 0x10000ull
                              + (std::uint64_t)j * 0x1000ull;
      job.len = 4096;
      job.write = (j % 2) != 0;
      m.enqueue_job(job);              // 仅模型 job queue，不编辑 RTL
    }
  }

  m.start_all();

  constexpr int kCycleLimit = 200000;
  int cycle = 0;
  while (!m.all_done() && cycle < kCycleLimit) {
    m.tick();
    ++cycle;
  }

  EXPECT_TRUE(m.all_done()) << m.dump_watchdog_state();
  EXPECT_EQ(m.watchdog_stall_count(), 0);
  EXPECT_EQ(m.data_errors(), 0);
  EXPECT_EQ(m.taint(), 0);
  EXPECT_GE(m.max_job_queue_occupancy(), 2);   // 确实制造多 job
  EXPECT_LE(m.max_job_queue_occupancy(), 8);   // 不超过 capacity
  EXPECT_TRUE(m.aw_w_b_consistency());
  EXPECT_TRUE(m.shared_r_returns_all_beats());
}
```

### 5.2 方案 B：复用现有 Tb 的预生成写

若现有 Tb 更容易：

- 在 bank0 预生成 W beats 到 `WFifoDepth/2`，最后一拍 `last=1`，全部 full-strobe。
- 同时让 bank1 发 AR。
- 只测 capacity corner：bank0 W 能否 drain、bank1 AR 是否被 guard 延迟但不死锁、shared R 是否最终返回所有 beat。
- 不改 joint/core，不改 RTL，只加 TB scenario。

### 5.3 验收检查

- 所有 job 在 cycle limit 前完成，无 watchdog。
- `in_flight_o` 回到 0；`inflight_aw_q`、`inflight_ar_q`、`inflight_w_q`、`w_partial_q` 全部归零。
- `data_errors=0`、`taint=0`。
- AW/W/B 关联与 B/R ID 映射保持。
- 若测 `BATCH_PORT` 串行性：断言 `batch_issued_q - batch_completed_q <= 1`，且 `JobFifoDepth` 不由 `BATCH_PORT` 自动填满。
- 若模型不能表达 `max_job_queue_occupancy >= 2`，应输出明确 skip/fail，不得写 pass。

---

## 6. 未决项

- 实际 `AxiDataWidth`、`WFifoDepth`、`NumOutstandingTrans` 的集成参数值未在提供日志中直接给出。
- 现有 coupled C++ harness 的真实 API 未提供，方案 A 的 `TODO(adapter)` 需 Codex 按实际头文件替换。
- 板级 A8 hang root 仍未知；`v1-work-state.json:16-20` 已列明未证明项。
- `coupled-banked-vv.log:105-122` 的观察/打印线网缺失已被证实，但不能据此生成 RTL root cause。
- 本次未执行 DS；以上均为提案与证据整理，最终 core 验收由 Codex 决定。