# 修订后的导航式说明（相对上一草案的有界修订）

状态：本文件只修订上一草案中与给定 RTL 冲突/不成立的部分；不重跑原版 15 例，不实现 RTL，不假定已有诊断接口。当前 v1 两版仍按冻结 15 试验推进；架构与下一阶段决策由 Codex 负责。

---

## 0. 先看结论（6 条）

1. **fast 接收的精确事件 = `rv_iommu_nonblock_hit.accept_o`，在 `NonblockEnable` 下就是 `tl_wrap.nonblock_fire_o = n_accept`。** `accept_o` 有完整资格（`LEAF` 状态、`enable_i`、候选未变、DDTP/DC/leaf 复核）。`raw fast > 0` 足以证明**实际 fast 接收发生过**，不需要质量校验器再把 fast0 拒绝；不能说“无法判定是否激活”。证据：`rv_iommu_nonblock_hit.sv:81-84`、`:116-127`、`rv_iommu_tl_wrap.sv:276`、`:294`。
2. **适格窗口不是“有 V 有 A”就够了。** `tl_wrap.window_i` 要求 `state_q==PT && e3_ptw_active && en_1S_q && !en_2S_q && !ddt_imp_trans_q && !pdt_imp_trans_q && !req_data.is_debug && trans_req_data_i.bank != req_data.bank && !ats_ddtc_valid_i && !debug_req_valid_i && !ats_trans_req_valid_i`。Solo 没有另一 bank 请求，因此 **Solo 里 fast=0 是正常的**，不能诊断接口坏。证据：`rv_iommu_tl_wrap.sv:286-289`。
3. **新计数不是 cache hit 率，fast0 也不是技术错误。** fast/normal 是按来源与路径的接收计数；Mode2 允许 fast 全 0，现有 runner 只要求 V/A 的 fast+normal 之和 >0。证据：`scripts/analyze_nonblock_pilot.py:60-62`、`:1649`、`:1656-1657`；`scripts/mac_nonblock_pilot.py:475-497`。
4. **现有 raw 能说 / 不能说**：能说实际 fast 接收次数、技术资格、A 是否存在；不能说“适格机会数、是否 start、DC/LEAF 拒绝原因、guard 是否挡住候选、cache hit/miss”。`rv_iommu_nonblock_hit` 没有计数输出，观察器只在 `accept` 时累加分类。证据：`rv_iommu_nonblock_hit.sv:14-34`、`:45-90`、`:100-132`；`dma_nonblock_observer.sv:76-96`、`:155-161`。
5. **禁止建议无保护 guard-off BIT。** 直接移除读接纳守卫会恢复已复现的有限模型循环依赖；guard 成本只能在相同 guard 下比较 S→N，不能单独隔离。证据：`rv_iommu_banked_ctl.sv:244-266`、`:312-354`；`docs/nonblock-dependency-diagnosis-20261004.md:9-12`、`:46-49`、`:70-81`、`:108-113`。
6. **科学阴性不默认重跑同一 15 项。** 若 fast 全 0 或无收益，先用现有 raw 说清可说/不可说；若仍不可判别，只建议增加一个有判别力的 BIT 诊断接口，由 Codex 决定是否构建。

---

## 1. 精确事件定义与资格（RTL 证据）

### 1.1 fast 接收事件

- 实际 fast 接收 = `rv_iommu_nonblock_hit.accept_o`：
  `state_q == LEAF && enable_i && candidate_valid_i && candidate_i == req_q && !invalidate_i && ddtp_i == ddtp_q && dc_ok && ddtc_dc_i == dc_q && leaf_ok`。
  证据：`rv_iommu_nonblock_hit.sv:81-84`。
- `leaf_ok` 要求 IOTLB 命中且 1S 内容有效、U 位、非 1G/2M、读/写权限匹配。证据：`rv_iommu_nonblock_hit.sv:75-80`。
- 在 `NonblockEnable` 下，`accept_o` 就是 `n_accept`，`nonblock_fire_o = n_accept`。证据：`rv_iommu_tl_wrap.sv:276`、`:294`。
- 观察器只在 `accept` 时累加，并按 `fast` 分类到 words[6..10]。因此 `raw fast > 0` = 实际 fast 接收发生。证据：`dma_nonblock_observer.sv:76-96`、`:155-161`；`scripts/mac_nonblock_pilot.py:123-131`、`:475-479`。

### 1.2 适格窗口与 Solo 的正常 fast0

- `window_i` 条件见上，关键是与当前在途请求**不同 bank**的候选、PT 状态、`e3_ptw_active`、仅 S1、无隐式 2S、非 debug、无 ATS。证据：`rv_iommu_tl_wrap.sv:286-289`。
- `start` 还要求 `state_q==IDLE && enable_i && window_i && candidate_valid_i && !invalidate_i`。证据：`rv_iommu_nonblock_hit.sv:45-46`。
- Solo 没有另一 bank 请求，`trans_req_data_i.bank != req_data.bank` 无法满足，因此没有适格机会。**Solo 的 fast0 不能诊断接口坏。**
- 若某 BIT 的 `NonblockEnable=0`，`n_accept=0`、fast=0 是结构性的；这需要 Codex 从构建参数确认。证据：`rv_iommu_tl_wrap.sv:303-316`。

### 1.3 观察器/helper 计数口径

- 观察器 16 字：magic、version、mode、flags、seq、cycles、fast DID10 read/write、fast DID11 read/write、fast other、normal DID10、normal DID11、normal other、accepted、errors。证据：`dma_nonblock_observer.sv:44-51`、`:176-192`。
- `accepted = Σfast + Σnormal`，且仅在 `(open, close]` 累加；open 边沿不计，close 边沿计。证据：`dma_nonblock_observer.sv:148-161`；runner 侧守恒检查 `scripts/mac_nonblock_pilot.py:475-479`；analyzer 侧 `scripts/analyze_nonblock_pilot.py:356-360`。
- Mode1 要求 fast 全 0；Mode2 允许 fast 可大于 0，但不要求 fast>0。证据：`scripts/analyze_nonblock_pilot.py:380-383`、`:1649`；`scripts/mac_nonblock_pilot.py:484-497`。
- 这些 fast 计数不是命中率，也不是全部 miss 数。证据：`scripts/analyze_nonblock_pilot.py:60-62`、`:1656-1657`。

### 1.4 现有 raw 的可说 / 不可说

**可说**
- fast>0：实际 fast 接收发生（激活已证明）。
- fast=0：该窗口没有 fast 接收；不是技术错误；不能证明接口坏。
- 技术资格：exit/taint/raw_window/helper/observer_status/seq。
- A 是否存在：`p4m.has_a` 与 `raw_window[3]`。
- Mode2 的 fast 可大于 0，但现有流程不要求。

**不可说**
- 适格机会数（`window_i && candidate_valid`）。
- 是否 start、是否 intercept。
- DC/LEAF 拒绝原因（`dc_ok`、`leaf_ok`、`iotlb_hit_i` 等）。
- guard 是否挡住候选。
- cache hit/miss、PLRU/替换、V-hit-during-A-PTW 时序。
- H2 替换是否仍参与、H3 guard 是否牺牲并行：现有接口不可分离。

证据：`rv_iommu_nonblock_hit.sv:14-34`、`:45-90`、`:100-132`；`dma_nonblock_observer.sv:76-96`、`:155-168`；`docs/nonblock-dependency-diagnosis-20261004.md:141-149`。

---

## 2. 两分支共用 baseline / metric / confounder / stop

**Baseline**
- 同一 kernel `6.5.0-rc2-00093-gc0f5e13ea81f`，taint 0，每次新鲜启动且 boot_id 必须变。证据：`scripts/mac_nonblock_pilot.py:64`、`:1044-1055`；`scripts/analyze_nonblock_pilot.py:1004-1026`。
- 同一 runner 冻结参数：`--chunk 4096`、`--victim-pages 1`、`--a-gap-cycles 0`、`--timeout-ms 60000`，aggressor pages 按冻结计划 1/8/16。证据：`scripts/mac_nonblock_pilot.py:243-256`、`:141-152`。
- 同一静态 reader ELF，ABI `idma-nonblock-window-v1`。证据：`scripts/mac_nonblock_pilot.py:121-131`、`:1094-1262`。
- S/N 必须同布局、同 guard、同容量/仲裁，仅绕过使能常量不同；S→N 才是绕过效应。证据：`docs/nonblock-dependency-diagnosis-20261004.md:96-97`、`:157-162`；`scripts/analyze_nonblock_pilot.py:49-51`、`:1284-1292`。

**Metric**
- 主指标：同窗口 V 吞吐 = `transfers×4096 / (raw_window[1]/50MHz)`。
- 副指标：同窗口 A 吞吐 = `raw_window[3]×4096 / 同一 raw_window[1]`；绝不用整轮 A 计数代替。证据：`scripts/analyze_nonblock_pilot.py:37-44`、`:642-664`。
- 绕过效应 = `banked-hit − banked-serial`；`original-serial` 只用于布局效应，不能作为绕过的因果基线。证据：`scripts/analyze_nonblock_pilot.py:49-51`、`:1284-1292`。
- 长组每条件只有 3 轮，只做描述性中位/min/max，不做显著性。证据：`scripts/analyze_nonblock_pilot.py:45`、`:1206-1227`、`:1300`。

**Confounder**
- 时间漂移：两套配对之间交替 S/N 顺序，配对间隔尽量短，记录 boot_id 与时间戳；不作跨长时间因果比较。
- 启动身份：kernel/taint/boot_id 必须符合门；`boot_id` 未变则技术失败。证据：`scripts/mac_nonblock_pilot.py:1053-1055`。
- UART/host：同一主机、同一串口、无其他进程占用。证据：`scripts/mac_nonblock_pilot.py:308-320`。
- SD：runner 不写 SD。证据：`scripts/mac_nonblock_pilot.py:34`。
- Guard：S/N 同 guard；guard 成本计入 baseline，不能单独隔离。证据：`rv_iommu_banked_ctl.sv:312-354`；`docs/nonblock-dependency-diagnosis-20261004.md:96-97`。
- 真实 DDR/DMA/互连、热漂移：未观测，不能宣称。证据：`docs/nonblock-dependency-diagnosis-20261004.md:141-149`。

**Stop**
- 任一技术失败：保留 raw，`effects` 不发布，不重试、不续跑、不剔除。证据：`scripts/mac_nonblock_pilot.py:8`、`:1451-1461`；`scripts/analyze_nonblock_pilot.py:1250-1302`、`:1723-1724`。
- 合法但阴性/低值样本：照常发布为负证据，不重跑、不换参数。证据：`scripts/analyze_nonblock_pilot.py:48`、`:55`、`:1640-1642`。
- 不把 3 轮波动阈值/同号当统计证明；只作为描述性可复现性 gate。

---

## 3. 分支 (1)：fast 实际发生且初步收益为正 → 如何验证独立可复现收益

### 3.1 预注册判据

- `F_N = Σ(helper.words[6..10]) > 0`，且在声称收益的条件里 fast 实际发生。
- `banked-serial` 的 Mode1 fast 必须全 0。证据：`scripts/analyze_nonblock_pilot.py:380-383`。
- helper 守恒：`words[14] == Σfast + Σnormal`。证据：`scripts/mac_nonblock_pilot.py:475-479`；`scripts/analyze_nonblock_pilot.py:356-360`。
- helper seq 在计划顺序上严格递增。证据：`scripts/mac_nonblock_pilot.py:498-500`；`scripts/analyze_nonblock_pilot.py:1087-1101`。
- 技术资格：15/15 complete、exit 0、taint_after 0、raw_window 通过、helper 通过、observer_status==4、analyzer exit 0。证据：`scripts/mac_nonblock_pilot.py:1451-1461`、`:369`；`scripts/analyze_nonblock_pilot.py:564-567`、`:1723-1724`。

### 3.2 最小实验：至少两套新鲜启动配对 S/N

- 每次调用 = 一个 BIT = 一次新鲜启动 = 冻结 15 例；无重试/无续跑。证据：`scripts/mac_nonblock_pilot.py:8`、`:108`、`:218-240`。
- 最小为两套配对（4 次新鲜启动）：
  - Pair p1：先 S 后 N。
  - Pair p2：先 N 后 S（交替顺序控制时间漂移）。
- 每套内分别用同一条 `--original` 目录做 `--serial`/`--hit` 分析；`--original` 只提供布局基线，不用于绕过因果。证据：`scripts/analyze_nonblock_pilot.py:1665-1672`、`:49-51`。
- 每套运行后分别汇总：
  ```sh
  python3 scripts/analyze_nonblock_pilot.py \
    --original <已有 original 目录> \
    --serial   <pair 的 S 目录> \
    --hit      <pair 的 N 目录> \
    --out      <pair 的 analysis 目录>
  ```
  参数证据：`scripts/analyze_nonblock_pilot.py:1665-1672`。

### 3.3 独立可复现收益的判定

- 以每套配对为单位计算同一条件的绕过增量：`Δbypass = N − S`，主指标 V 吞吐，副指标同窗口 A 吞吐。
- 独立可复现的 gate（不是显著性检验）：
  1. 两套配对都在**同一个预注册条件**（A 存在的长组条件，且该条件 N 的 `F_N>0`）给出同方向的正收益；
  2. 两套配对的 N 都满足 `F_N>0` 且 S 的 Mode1 fast 全 0；
  3. 两套配对的守恒、seq、observer_status 均通过。
- 若只做到一套配对，不能声称“独立可复现”；只能写“单套新鲜启动观察到初步收益”。
- 不把 3 轮同号/波动阈值当统计证明；以上只是可复现性 gate。跨 label 差异仍含运行间波动。证据：`scripts/analyze_nonblock_pilot.py:1643-1644`。
- 控制时间漂移：p1/p2 顺序交替，配对间隔短；记录每次 boot_id、时间戳；分析以配对为单元，不能把不同启动的 original 当因果基线。

### 3.4 能支持 / 不能支持

- 能支持：在同一布局、同一 guard 下，绕过在至少两套新鲜启动配对中给出同方向的正窗口增量；fast 实际发生。
- 不能支持：guard 成本单独隔离、cache 命中率、真实 DDR/DMA、新颖性、H2/H3 分离。
- Stop：
  - 若 N 的 `F_N==0` → 转分支 (2)。
  - 若初步收益不为正 → 转分支 (2) 的“无收益”路径。
  - 若两套配对方向不一致 → 记为科学阴性，停止性能声明，不重跑同一 15 项，不调参。
  - 若任一技术失败 → 停止，保留 raw，`effects` 不发布。

---

## 4. 分支 (2)：fast 全 0 / 无收益 → 先用现有 raw 定位，再只建议一个诊断 BIT

### 4.1 先用现有 raw 说什么 / 不说什么

**先说技术资格**
- 15/15 complete、exit 0、taint_after 0、raw_window 通过、helper 通过、analyzer exit 0 才进入机制讨论；否则是技术失败，不是科学阴性。证据：`scripts/mac_nonblock_pilot.py:1451-1461`；`scripts/analyze_nonblock_pilot.py:1723-1724`。

**可说**
- `F_N = Σ(words[6..10])`；normal = Σ(words[11..13])；`words[14] = F_N + normal`。证据：`scripts/mac_nonblock_pilot.py:475-479`；`scripts/analyze_nonblock_pilot.py:356-360`。
- Mode1 fast 必须全 0；Mode2 允许 fast 全 0，fast0 不是技术错误。证据：`scripts/analyze_nonblock_pilot.py:380-383`、`:1649`。
- `p4m.has_a` 与 `raw_window[3]` 说明 A 是否存在。证据：`scripts/mac_nonblock_pilot.py:401-404`；`scripts/analyze_nonblock_pilot.py:642-664`。
- Solo 没有另一 bank 请求 → 没有适格机会，fast0 正常。证据：`rv_iommu_tl_wrap.sv:286-289`。
- 若 `F_N>0` 但收益 ≤0：fast 实际发生，但没有收益；现有 raw 不能分离 H2/H3/其他。证据：`docs/nonblock-dependency-diagnosis-20261004.md:157-162`。

**不可说**
- 适格机会数、start/intercept 数。
- DC/LEAF 拒绝原因。
- guard 是否挡住候选。
- cache hit/miss、PLRU/替换、V-hit-during-A-PTW。
- 因此不能从“两个启动 fast 全 0”推断使能坏或接口坏；也不能把 fast0 当技术错误。

**处置**
- 不再为科学阴性默认重复同一 15 项。
- 若现有 raw 无法判别，才建议一个诊断 BIT；是否构建由 Codex 决定。

### 4.2 一个有判别力的 BIT 诊断接口草案（不实现 RTL，不假定已有）

前提：现有 00093 软件能串口上传静态 reader（`scripts/mac_nonblock_pilot.py:1094-1262`）。以下只是诊断窗口提案，不是现有接口；需要 Codex 决定是否新增 RTL tap 与 reader 读取代码。

#### 4.2.1 计数定义草案

**A. 非阻塞探针范围（tap `rv_iommu_nonblock_hit` 内部/边界）**

令：
- `W` = `tl_wrap.window_i`（`rv_iommu_tl_wrap.sv:286-289`）
- `EN` = `nonblock_enable_i`（`rv_iommu_tl_wrap.sv:285`）
- `INV` = `invalidate_i`（`rv_iommu_tl_wrap.sv:292-293`）
- `CV` = `candidate_valid_i`（`rv_iommu_tl_wrap.sv:290`）
- `START` = `rv_iommu_nonblock_hit.start`（`rv_iommu_nonblock_hit.sv:45-46`）
- `ACCEPT` = `rv_iommu_nonblock_hit.accept_o`（`rv_iommu_nonblock_hit.sv:81-84`）

计数：
1. `opp_win_cnt`：每周期 `W && CV && EN && !INV` 加 1。含义：wrapper 级适格窗口且候选有效。
2. `start_cnt`：每周期 `START` 加 1。含义：探针真正启动。
3. `fast_accept_cnt`：每周期 `ACCEPT` 加 1。含义：实际 fast 接收，与 `nonblock_fire_o` 同事件。证据：`rv_iommu_tl_wrap.sv:276`。
4. `opp_no_start_busy_cnt`：每周期 `W && CV && EN && !INV && !START` 加 1。含义：适格窗口存在但探针未启动（如 busy）。
5. `reject_dc_cnt`：每周期 `state_q==DC` 且 DC 拒绝条件成立加 1。拒绝条件：`!enable_i || invalidate_i || ddtp_i != ddtp_q || !dc_ok || !candidate_valid_i || candidate_i != req_q`。证据：`rv_iommu_nonblock_hit.sv:106-113`。
6. `reject_leaf_cnt`：每周期 `state_q==LEAF && !ACCEPT` 加 1。证据：`rv_iommu_nonblock_hit.sv:115-127`。
7. `inflight_at_close_cnt`：在 close 时读 `start_cnt - fast_accept_cnt - reject_dc_cnt - reject_leaf_cnt` 的余额，或读探针未终止状态快照。用于守恒闭合。

**B. DC 拒绝原因（优先级编码，互斥）**

1. `rej_dc_enable_inval_cnt`：`!enable_i || invalidate_i`
2. `rej_dc_ddtp_changed_cnt`：否则 `ddtp_i != ddtp_q`
3. `rej_dc_candidate_changed_cnt`：否则 `!candidate_valid_i || candidate_i != req_q`
4. `rej_dc_dc_ok_false_cnt`：否则 `!dc_ok`

守恒：`reject_dc_cnt = Σ(以上四项)`。

**C. LEAF 拒绝原因（优先级编码，互斥）**

1. `rej_leaf_precond_cnt`：`!enable_i || invalidate_i || ddtp_i != ddtp_q || !candidate_valid_i || candidate_i != req_q || !dc_ok || ddtc_dc_i != dc_q`
2. `rej_leaf_no_iotlb_hit_cnt`：否则 `!iotlb_hit_i`
3. `rej_leaf_bad_leaf_cnt`：否则 `!leaf_ok`

守恒：`reject_leaf_cnt = Σ(以上三项)`。

**D. 可选 guard 范围（仅当 Codex 确认跨模块资格定义）**

1. `guard_delayed_read_cnt`：在 `rv_iommu_banked_ctl` 中，每周期 `tr_axi_req_i.ar_valid && !bank_read_admit[ar_sel]` 加 1。含义：guard 延迟了该读的选择。证据：`rv_iommu_banked_ctl.sv:263-266`。
2. 子原因位：`guard_same_cycle_aw` = `aw_accept && aw_sel == ar_sel`。证据：`rv_iommu_banked_ctl.sv:259-266`。
3. 注意：这不是“非阻塞候选被挡”的直接计数；要链接到非阻塞资格，必须由 Codex 定义跨模块 qualify。否则该字只作 guard 范围诊断，不参与非阻塞守恒。

#### 4.2.2 守恒约束草案

- `opp_win_cnt >= start_cnt`。
- `opp_win_cnt = start_cnt + opp_no_start_busy_cnt`（若还有其它 no-start 原因，需加项）。
- `start_cnt = fast_accept_cnt + reject_dc_cnt + reject_leaf_cnt + inflight_at_close_cnt`。
- `reject_dc_cnt = Σ(rej_dc_*_cnt)`，按上述优先级编码。
- `reject_leaf_cnt = Σ(rej_leaf_*_cnt)`，按上述优先级编码。
- `fast_accept_cnt == Σ(words[6..10])`（同一 `(open, close]` 窗口，若现有 fast 分类同时有效）；不等则窗口/ABI 不一致或分类错误。
- 现有 ABI 守恒不变：`words[14] == Σfast + Σnormal`。证据：`scripts/mac_nonblock_pilot.py:475-479`；`scripts/analyze_nonblock_pilot.py:356-360`。
- 窗口语义不变：只在 `(open, close]` 累加；open 边沿不计，close 边沿计；溢出置 sticky 不回绕。证据：`dma_nonblock_observer.sv:148-161`、`:152-160`。
- `inflight_at_close_cnt` 应为 0 才能严格闭合；若 >0，置 `E_INFLIGHT_AT_CLOSE` 并将该窗口标为不完整。

#### 4.2.3 读数与判别

- `opp_win_cnt=0` 于全部 A 存在条件：没有适格机会。检查上游：bank mismatch、S1-only PT 窗口、`e3_ptw_active`、无 debug/ATS、guard 是否在更早处挡住候选。
- `opp_win_cnt>0`、`start_cnt=0`：有适格窗口但未启动；看 `opp_no_start_busy_cnt` 与 enable/invalidate。
- `start_cnt>0`、`fast_accept_cnt=0`：探针启动但被拒；看 DC/LEAF 原因计数。`rej_leaf_no_iotlb_hit_cnt` 主导 → IOTLB/资格；`rej_dc_dc_ok_false_cnt` 主导 → DC 不适格；`rej_leaf_bad_leaf_cnt` 主导 → leaf 权限/大小。
- `fast_accept_cnt>0` 但无收益：fast 已实际发生，不是接口问题；转性能/架构解释（H2/H3、替换、guard 成本），由 Codex 决策。
- `fast_accept_cnt == Σ(words[6..10])` 交叉校验：证明现有 fast 计数是“实际 fast 接收”，不是命中率。

### 4.3 分支 (2) 的 baseline / stop

- Baseline 同第 2 节。
- 先用现有 raw 定位；不重跑同一 15 项。
- 只有 Codex 批准才构建一个诊断 BIT；不假定该接口已存在。
- 一个诊断 BIT 后：
  - `opp_win_cnt=0` → 上游/guard 候选问题；
  - `start_cnt>0 && fast_accept_cnt=0` → 拒绝原因问题；
  - `fast_accept_cnt>0` → 性能/收益问题。
- 不再无 Codex 决策地迭代 BIT。

---

## 5. 删除/禁止的旧说法（相对上一草案）

- 删除“两个启动 fast 全 0 即可判断使能/接口坏”。
- 删除“fast>0 仍无法判定是否激活”的说法；fast>0 即证明实际 fast 接收。
- 删除不必要的 N 单侧新鲜启动复现；fast 全 0/无收益走现有 raw 定位 + 一个诊断 BIT。
- 不把 3 次波动阈值/同号当统计证明。
- 禁止建议无保护 guard-off BIT；移除 guard 会恢复已复现有限模型循环依赖。
- 新计数不是 cache hit 率；fast0 不是技术错误。

**当前 v1 两版仍按冻结 15 试验推进。架构与下一阶段决策由 Codex 负责。**