# Nonblock 依赖诊断（2026-10-04，v1 读接纳守卫，模型范围）

状态：本文件只记录被提供日志与源码能够支持的 有限范围的 C++ 模型诊断。不覆盖真实 DDR、真实 DMA、板级 A8 根因或全互连行为，也不宣布新颖性、性能增益或最终通过。所有“36 个场景通过”“12 个场景通过”只指已有 有限模型日志中的场景与 order（顺序）。DeepSeek起草；Codex审查关键源码并实际执行仿真与构建。

---

## 1. 一句话摘要

- 旧 banked 有限模型（无守卫）在 `coupled-banked-vv.log:1-122` 出现 8/12 失败（a1 与 a8 系列）。
- v1 读接纳守卫（`coupled-banked-guard-vv.log:1-78`）把失败压到 4/12：只剩 A8 系列（`mode1-a8-l1`、`mode1-a8-l31`、`mode2-a8-l1`、`mode2-a8-l31`）。
- guard2 变化（在 AW 被接纳的同一周期阻止读选择，并叠加 owner admission）后，`coupled-banked-guard2-vv.log:1-26` 12/12 通过；`coupled-legacy-vv.log:1-26` 同 order 也 12/12 通过；`coupled-extra-orders.json:1-9` 记录 `aa`/`av` 额外 order 退出 0。三项合计 36 个 修复后的双银行模型场景通过（vv + aa + av）；legacy 同 order 12 个场景通过。
- 这确认了 AW FIFO 的“一拍可见性”竞争（见 §3.3），但仍是模型内的读写依赖与共享 R/W 观察，不是真实 DDR/DMA 结论，也不能直接算作 N 的增益。

---

## 2. 证据清单

### 2.1 状态与结果

- `out/n-prototype-20261004/v1-work-state.json:1-23`
  - `status = two_fpga_builds_running_not_board_accepted`
  - `original_board = 15/15 complete data_errors 0 taint 0`（板级原始计数，非模型；taint 是板级字段，Verilator 模型里没有 taint）
  - `core_change = Codex read-admission guard v1；legacy 默认关闭；signed AW-minus-WLAST 加可见 AW 预留，以及同周期 AW 接纳优先`
  - `actual_checks`：`legacy_control_vv = 12`，`banked_fixed_vv_aa_av = 36`，`joint = PASS`，`banked_ctl = PASS`，`helper_guard = PASS`
  - `not_proven`：板级 hang 根因精确等于模型、所有互连/DDR、通用跨流数据依赖、N gain/novelty
- `out/n-prototype-20261004/coupled-banked-vv.log:1-122`
  - 旧 banked（无守卫）单次调用：12 个场景标题各一次，`nonblock_coupled_test: 8/12 scenarios FAILED`（`:122`）
  - 8 个 STALL 转储对应 8 个失败场景（a1/a8 系列）
- `out/n-prototype-20261004/coupled-banked-guard-vv.log:1-78`（cycle2 证据，v1 第一版守卫）
  - 8/12 通过，4/12 失败；`nonblock_coupled_test: 4/12 scenarios FAILED`（`:78`）
  - 失败即 A8 系列，STALL 状态见 `:1-13`、`:15-27`、`:29-41`、`:43-55`
- `out/n-prototype-20261004/coupled-banked-guard2-vv.log:1-26`
  - guard2 后 12 场景全部打印 `COUPLED-SUMMARY`，以 `PASS (12 scenarios)` 结束（`:26`）
- `out/n-prototype-20261004/coupled-legacy-vv.log:1-26`
  - legacy 同 order 12 场景通过
- `out/n-prototype-20261004/coupled-extra-orders.json:1-9`
  - `aa`、`av` 均 `exit: 0`
- `out/n-prototype-20261004/joint-after-readguard.log:1-10`
  - joint 数据通路 smoke 场景 `PASS`
- `out/n-prototype-20261004/helper-nonzero-guard.log:1-2`
  - runner/analyzer 的 zero/V/dual counter guard `PASS`

### 2.2 关键源码

- `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv`
  - `:244-253`：输入 demux 在 W LAST 前保持 W ownership；跨 bank 读会填满 DMA 的 R→W FIFO，而 AW 不能进入该 demux，导致当前 owner 的 source data 或 page-table reads 无法通过共享内存返回，形成 circular wait（循环等待）。守卫的处理是延迟“选择”读，而不是撤回 AXI/TL VALID。
  - `:254-266`：`w_unfilled_q` signed balance、`w_owner_q`、`restrict_reads`、`read_owner`、`bank_read_admit[0/1]`；新 AW 还通过 `tr_axi_req_i.aw_valid` 保留 admission；同一周期 AW handshake 进入某 bank 时，用 `!(aw_accept && aw_sel)` 阻止同周期选择旧 AR。
  - `:259-262` 注释明确：新 AW 进入 bank 后，它的 FIFO head 下一周期才可见；不要提前一个周期选择旧 queued AR。
  - `:312-354`：两个 bank 仍实例化 `rv_iommu_trans_ctl_wrap`，但 `ReadAdmissionEnable(1'b1)` 且接入 `read_admit_i`。
  - `:542-582`：`in_flight_o` 聚合 AW/AR/W-LAST/bank busy，不是只看查表器。
- `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`
  - `:23-25`：`ReadAdmissionEnable` 是 banked DMA 集成的新约束，legacy 默认关闭。
  - `:61`：`read_admit_i` 输入。
  - `:621-666`：IDLE 中 AW 优先于 AR；读只在 `(!ReadAdmissionEnable || read_admit_i)` 时进入 `REQ_TRANS`。
  - `:713-723`：COMPLETE 中只有读完成才释放 AR，写完成才释放 AW。

### 2.3 数据宽度（更正）

- `tests/nonblock_coupled_test.cpp:70-72`：`PAGE_BEATS = 512`、`BURST_BEATS = 256`、`BURST_LEN = 255`（→ 256 拍）。
- 一拍是 64 位字：`:232-250` 的读写按 8 字节一个 beat（`BURST_BEATS 256; // 2 KiB / 8 B`，`:71`）。
- 一个 burst = 256 拍 × 8 B = 2 KiB；一个 page = 512 拍 × 8 B = 4 KiB；两个 256 拍 burst（half 0/1，各 0x800=2048 字节，见 `:361`、`:400`、`:647`）= 4 KiB，正好覆盖一个 page。
- 不能因为“len=255 只覆盖一半”就反推 16 字节/拍。`len=255` 覆盖的是半个 page（2 KiB），所以每拍仍是 8 字节。

---

## 3. 模型循环（模型解释，非真实 DDR/DMA 根因）

以下循环是 scoped C++ 模型中的依赖图。日志没有直接打印 A/V 标签，A/V 映射按端口/队列状态与测试源码给出，因此保留“模型解释”。

### 3.1 旧 banked（无守卫）：8/12 失败

证据 `coupled-banked-vv.log:2-12`：

- `:2` `sharedR.head port=0 id=4 addr=0x0000000090000000 len=255 beat=5`（队列头是 A 的源读数据）
- `:3` DMA0（V）`ar_done=1 aw_done=1 done=0`
- `:4` DMA1（A）`aw_want=1 aw_done=0 rx=3 cfifo=3`
- `:5` `caw_q.head id=0 addr=0x0000000080001000 got=0`（V 目的完成 AW 待处理）
- `:9` `AW id=0 addr=0x0000000010001000`（V 目的 AW）
- `:12` `mem_q=2 aw_order=1 w_owner=0`

模型解释：共享有序 R 返回队列的队头是 A 的源读数据；A 的 cfifo 已满（3）无法消费队头；V 侧目的完成 AW 仍未落地（`caw_q` 非空），W 无法推进。读完成依赖共享 R 继续返回后续 beat，而形成模型级 circular wait（循环等待）。说明：同 ID 同 bank + AW/W 关联本身不足以排除共享 R/W 依赖；这不能证明真实 DDR 控制器会这样。

### 3.2 v1 守卫后仍失败的 A8：cycle2 证据与推断

证据 `coupled-banked-guard-vv.log:1-13`（`mode1-a8-l1` STALL；`:15-27`、`:29-41`、`:43-55` 同构）：

- `:2` `sharedR.head port=0 id=4 addr=0x0000000090000000 len=255 beat=21 latency=0`（队头是 A 的源读数据，beat 21）
- `:3` DMA0（V）`half=1 ar_done=1 aw_want=1 aw_done=0 rx=0 wx=0`
- `:4` DMA1（A）`half=0 ar_done=1 aw_want=0 aw_done=1 rx=19 wx=16 cfifo=3`（A 已发 16 个 W beat，另有 3 个在耦合 FIFO）
- `:5` `caw_q.head (empty)`
- `:6` `exp_aw_q.head id=4 iova=0x0000000020008000 pa=0x0000000090100000 len=255`（A 目的 IOVA→PA 的完成 AW 尚未产生）
- `:7` `last up_ar=yes up_aw=yes up_w=yes up_r=yes up_b=yes`
- `:8` `AR id=0 addr=0x0000000010000800`（V 源 AR，half=1）
- `:9` `AW id=4 addr=0x0000000020008000`（**A 目的 AW 已在上游被接纳**）
- `:10` `W data=0x11f075d0211af943 strb=0xff last=0`
- `:11` `R data=0x2b19e304a37b7502 last=0 resp=0`
- `:12` `TL req_v=0 req_r=0 req_bank=1 resp_v=0 resp_r=0 resp_bank=0 ptw_active=1 ptw_r_fire=0 iotlb_look=0 iotlb_hit=0`（**PTW 活跃 = 1**）
- `:13` `mem_q=2 caw_q=0 cb_q=0 exp_aw_q=1 aw_order=1 w_owner=1 in_flight=1 nonblock_busy=0 nonblock_fire=0`

推断（明确标为推断）：

- “A 的源数据 burst 排在目的 AW 之前”：共享有序 R 返回队列的队头是 A 的源读数据（`id=4`、`0x90000000`、`beat=21`，`:2`）。
- “目的 AW 没有被物理转发”：A 的目的 AW 已提交并被上游接纳（`:7` `up_aw=yes`、`:9` `AW id=4 addr=0x20008000`），但没有对应的完成 AW（`:5` `caw_q` 空、`:6` `exp_aw_q=1`），写通道没有真正推进。
- “PTW 返回被堵在数据后面”：PTW 活跃（`:12` `ptw_active=1`），其返回在同一个有序队列里排在 A 源数据之后；A 的耦合 FIFO 已满（`:4` `cfifo=3`），队头那颗 A 源 beat 不能被消费，队列不前进，PTW 返回因此被堵在数据后面（`mem_q=2`，`:13`）。

不判定：日志没有 did 追踪，**不能断言这个 active 的 PTW 一定属于 A**；它也可能是共享转换引擎在为 V 工作。因此只写“PTW 活跃且其返回被数据挡住”，不写归属。

### 3.3 已被 guard2 确认的一拍可见性竞争

- 源码依据：`rv_iommu_banked_ctl.sv:254-266`（`bank_read_admit`、`read_owner` 与 `!(aw_accept && aw_sel)`）与 `:259-262` 注释；`rv_iommu_trans_ctl_wrap.sv:621-666`（IDLE 中 AW 先于 AR，读是否进入 `REQ_TRANS` 取决于 `read_admit_i`）；`:713-723`（COMPLETE 中读释放 AR、写释放 AW）。
- 模型解释：新 AW 被接受到 bank 后，其 FIFO head 下一周期才对 bank 控制器可见；若同一周期还允许选择旧 queued AR，就可能出现一拍可见性竞争。guard2 的做法是：在 AW 被接纳的同一周期阻止读选择，并叠加 owner admission。
- 结果确认：guard2 后 A8 系列 4 个场景全部通过（`coupled-banked-guard2-vv.log:11-13`、`:23-25` 均打印 `COUPLED-SUMMARY` 并以 `PASS (12 scenarios)` 结束）。这把 §3.2 的“一拍可见性竞争”从推断提升为“被 guard2 修复所确认的模型内机制”。
- 仍是模型结论，不是真实 DDR/DMA 结论。

---

## 4. 已证实、未证实与观察线网问题

### 4.1 已证实（仅限 有限模型 / 已有日志）

- 旧 banked（无守卫）：`coupled-banked-vv.log:1-122` 出现 8/12 失败。
- v1 守卫第一版：`coupled-banked-guard-vv.log:1-78` 8/12 通过、4/12 失败（只剩 A8）。
- guard2：`coupled-banked-guard2-vv.log:1-26` 12 场景 PASS。
- legacy 同 order：`coupled-legacy-vv.log:1-26` 12 场景 PASS。
- 额外 order：`coupled-extra-orders.json:1-9` `aa`/`av` exit 0。
- joint/helper smoke：`joint-after-readguard.log:1-10`、`helper-nonzero-guard.log:1-2`。

### 4.2 观察与线网问题（更正）

- `coupled-banked-vv.log` 是**一次调用**，不是两次运行：
  - `+order=vv` 头只出现一次（`:105`），12 个场景标题各出现一次（`:106`—`:121`），没有“重复 `+order=vv`”。
  - 失败场景（a1/a8 共 8 个）只打印场景标题、不打印 `COUPLED-SUMMARY`，原因是场景标题先打印（`tests/nonblock_coupled_test.cpp:986-987`），汇总只在场景成功走到末尾时打印（`:1053`），失败则在汇总前抛异常（`:1116-1124`）。这是“失败所以没有成功汇总”，不是观察/打印线网缺陷。
  - 日志里 stderr 的 STALL 转储（`:1-104`）与 stdout 的场景标题/汇总在文件中先后出现，只是 stdout/stderr 缓冲与混合顺序造成的表面顺序差，属于同一进程的同一批输出。
  - 因此，之前“第二次 vv 运行 / 观察或打印线网缺陷”的说法及其结论整体删除，不作为 RTL 根因。
- 真实的观察线网问题在别处：
  - Vivado 把 `nonblock_fire`、`nonblock_trans_accept`、`nonblock_did`、`nonblock_type` 当作未声明符号、默认 `wire`（`ariane_peripherals_xilinx.sv:1381-1382`，见 `wire-scope-evidence`，`fpga-serial.log:5193-5196/9908-9911`、`fpga-hit.log:5193-5196/9908-9911`）。这是 generate 作用域问题；源码已移到模块作用域，待新构建验证。
  - 板级 helper 在 CLI 字节正确的情况下接受计数为 0，也是观察/线网层面的问题，需要在修复后重跑板级才能闭环。

### 4.3 板级 A8 根因：未知

`v1-work-state.json:5` 只记录恢复后的 boot/kernel/taint（`956fa8d0-... kernel00093 taint0`）。`v1-work-state.json:16-20` 明确列出未证明：板级 hang 根因精确等于模型、所有互连/DDR 行为、通用跨流数据依赖、N gain 或 novelty。没有“修复后板级重跑”的样本，因此**板级 A8 根因在修复后的板级重跑之前不能确认**，也不能把 §3 的模型循环直接写成板级 A8 根因。

### 4.4 不宣称

- 不宣称真实 DDR / 真实 DMA 行为。
- 不宣称板级通过，也不宣称板级 A8 根因已定。
- 不宣称性能增益。
- 不宣称新颖性。
- 不宣称完整 root HANDOFF 已读；本文件只基于提供日志与源码片段。

---

## 5. 科学含义：N 的 baseline（基线）与 bypass 机会

`docs/research-n-prototype-spec-20261004.md:51-53` 要求 fair baseline 使用相同 replacement 更新规则，真实被服务 hit 必须参与既定替换策略。`:99-109` 说明两 bank 原型只是前人已知的有限 hit-under-miss 基线，不能把静态 bank 改名成额外保护方法，必须公平对照找剩余瓶颈。

读接纳守卫的逻辑是：在 W owner 未释放或新 AW 可见前，延迟新读的“选择”。这避免了一类 circular wait，但同时会减少读在写未完成前绕过/前向的机会。因此：

- 守卫的模型通过不能直接解释为 N 的收益。
- 若 N 的普通 baseline 也加同类守卫，N 可展示的 bypass 机会可能减少。
- 公平比较必须保持每类容量、仲裁/替换规则相同，并把守卫作为 baseline 成本计入。
- N 的“前进”必须来自真实被服务的合法 hit，而不是只读 probe 或守卫下的延迟选择。

---

## 附：当前状态与交接（只列当前前沿）

- 当前两个 FPGA 构建仍在运行；没有最终时序，也没有新的板级运行结果。
- 观察线网问题（Vivado 未声明符号 / generate 作用域）已定位，源码已移到模块作用域，待新构建验证。
- 旧串行BIT的Solo/A1只读窗口接收计数为0，搬运字节正确；新BIT须重新确认计数非零。
- 板级 A8 根因只有在修复后的板级重跑后才可能确认。
- 模型一致不等于证明；以上全部只限 有限模型。

---


### v2当前进展：整份4KiB缓冲、有限模型通过、BIT构建中

DMA0/1 BUFFER_DEPTH512，DMA2/3仍3；S/N相同，数据净增8144B、MetaFifoDepth11→520，实际综合资源待测。batch硬件要求4096B和源/目的页对齐、一次一个job等完成。本次新增depth512+overlap+rr模型banked12/12、legacy12/12通过；不是完整实际DMA/DDR验证。原depth3 rr banked8/12失败/legacy12通过，证据保留。两v2 BIT已开始完整构建，未上板；现有Linux00093/reader/ABI与SD沿用。原版15不重跑，S/N各15仍按冻结计划；原版→S差异含布局+缓冲，S→N同容量比较才评估绕过增量，不称创新或已取得性能收益。


### N v3局部RAM修复开始（待验证）

v2两版布局失败，未产出接受BIT。v3保留DMA0/1各4KiB数据缓冲，复用production common_cells fifo_v3既有AsyncDpRam分支；新增默认关闭FORCE_FPGA_RAM，仅idma_stream_fifo Depth512且type_t8位时启用，其他FIFO/全局FPGA_EN不改变。复位和flush只需保持有效队列语义；无效存储内容不要求相同。Codex负责关键RTL，DeepSeek正在起草实际模块RAM/FF对照加独立数据队列验证；测试、资源、时序、上板尚未验收。失败构建移至out/n-prototype-20261004/placement-failed-v2-builds，无Linux/SD变更。


<!-- APPEND to docs/nonblock-dependency-diagnosis-20261004.md -->

### v4诊断追加：实际 DDR DC、MSI guard 与 passive diag bank（追加，不改写历史）

以下只记录已提供证据能支持的状态；未执行新诊断，不声称板级根因、full DMA integration 或 universal deadlock proof。

- v4 代码已实现；两版 BIT 构建进行中；**未上板**。
- 原版15项加v3 S/N各15项，共45项技术性通过；N 上 fast 计数为 0。该 0 不是 hit-rate，也不是 no-opportunities；不能据此判定“没有绕过机会”或“N 有收益”。
- 实际运行时 DDR/DC：DID 10/11 的 live DC 为 flat MSI mode1（`out/n-prototype-20261004/v4-live-context-evidence.json:23-27`）。v3 的 MSI guard 是 mode0-only 静态排除，故该实际内容在 v3 guard 下被排除；DDTP mode2 受支持（`rv_iommu_nonblock_hit.sv:91-93`）。没有理由默认 U 位未设置；v4 leaf 仍要求实际 `content_1S.u`（同文件 `:94-95`）。
- v4 MSI 边界：输入 MSI 匹配用 `VLEN39`/`VPNW39`，cached S1 PPN 用 `GPPNW39`，leaf 检查使用已捕获 DC `dc_q`（`rv_iommu_nonblock_hit.sv:62-79`）。仅 mode0 或 flat1 ordinary（input+leaf 均非 MSI）可走 fast；MSI范围内的读写及保留模式回退。原有权限/身份/稳定性 guard 保留（`:84-103`）。
- Passive 16-word 新 bank：observer局部索引16..31（研究索引D10..D1F，对应前端raw F10..F1F）；旧 0..15（0x00..0x0F，F00..F0F）ABI 与 word15 error 不变（`dma_nonblock_observer.sv:196-232`）。seen 位映射：0 base S1PTW（含 implicit/debug），1 full window，2 candidate，3 start，4 DC，5 DC good，6 LEAF，7 leaf good，8 DC MSI rej，9 leaf MSI rej，10 leaf miss，11 actual accept，12 DC miss，13 DC other，14 leaf other，15 enable（`rv_iommu_nonblock_hit.sv:115-131`、`dma_nonblock_observer.sv:223`）。
- 计数器：diag bank offset 7..15（绝对 word 23..31）计 event bits 3..11（`dma_nonblock_observer.sv:164-175`、`:223-232`）。窗口按冻结 `(open, close]`；diag bank 复用同一 core seq/cycles；跨窗口 partial probes 合法，因此**不得断言完整 lifecycle 守恒**；`diag fast == core fast sum`（validator `:29`）。
- 已有实际证据：TL exit0；RR 4 KiB proxy 12/12 exit0；MSI actual pass7 / reject10 / accept8 / responses8（含 BP）/ touches8（`v4-msi-regression.log:1`）；observer actual Mode1/2 exit0（`v4-observer-regression-fixed.log:16,27`）；validator 21 exit0（`v4-diag-validator-regression.log:42-44`）；static C crosscompile `-static -Wall -Wextra -Werror` exit0。
- Verilator 4.038 兼容临时源只移除已显式连线的 input default；production 不变（`v4-observer-regression-fixed.log:1-5`）。
- 边界：finite unit/proxy；不是 full DMA integration；不是 universal deadlock proof；不能把模型/单元通过写成板级 A8 根因已确认。
- S/N 同为 4 KiB buffer（DMA0/1）；旧 DMA2/3 不变；local RAM。原版 3 拍 vs S 512 拍混合了结构 + buffer 差异。v3 归档映射见 `v3-accepted-builds/RELOCATION.json`。
- 计划：新增 matched S/N v4，各 15 个 case，15 default sequence + `--diagnostics`；不重跑已有样本，不改 Linux/SD。技术性错误首次出现即停止；`fast0` 是科学负结果，不是技术性停止。先分析 actual candidate/reject/accept，再谈 cause/benefit。
- 不推测未知 build resource/timing 或 novelty。归属：DeepSeek proposal；Codex 关键 RTL、apply 与实际 test。


### 2026-10-04 v5 后续入口

v4两版30项通过但N候选/探测/快速接收0。v5资格读入口实现已通过有限主机验证，两版FPGA构建中；最新规格、适用范围与冻结板测计划见 [nonblock-v5-qualified-read-plan-20261004.md](nonblock-v5-qualified-read-plan-20261004.md)。实际状态以 `out/n-prototype-20261004/v5-build-state.json` 和工作区CURRENT_STATUS.md为准。
