# N v1 两版（S 双 bank 串行 / N 双 bank 命中绕过）15 冻结用例后的分支方案草案

状态声明：本文只整理**已给文件里真实存在的接口**，不实现代码、不编译、不板测、不重跑原版 15 例（`docs/nonblock-pilot-plan-20261004.md:98`）。凡是给定接口无法支持的，一律标为"不可执行/待 Codex 决定"，不虚构参数。`hit-under-miss` 本身不是创新（`docs/research-n-prototype-spec-20261004.md:95,:109`）。

---

## 0. 导航式结论（先看这 6 条）

1. **当前 runner/analyzer 的计数不能判定 mode2 快路径是否激活。** mode1 强制 fast 全 0（`scripts/analyze_nonblock_pilot.py:380-383`）；mode2 只要求 V/A 的 fast+normal 之和 > 0（`scripts/mac_nonblock_pilot.py:484-497`、`scripts/analyze_nonblock_pilot.py:365-378`），**从不要求 fast > 0**。因此"fast 全 0 且 normal 全非 0"的用例会被两边判为**技术合格**。
2. **16 字窗口里没有任何 cache hit/miss 计数。** fast/normal 是按**来源**（Vread/Vwrite/Aread/Awrite/Other，`scripts/mac_nonblock_pilot.py:130`、`bench/idma_nonblock_read.c:73-78`）与**路径**（fast/normal）的接收计数；没有 bank 维、没有 hit/miss 维。旧观察器的 lookup hit/miss 索引（`a_lookup_hit_events 0xE2B`、`a_lookup_miss_events 0xE2C`，`scripts/method88_aux.py:46-47`）属于旧 E3/COST/bank 观察器，spec 明确"旧 E3/COST 观察器语义不适用"（`docs/research-n-prototype-spec-20261004.md:105`；`scripts/analyze_nonblock_pilot.py:62,:1608`）。**不能把任何来源计数当命中率。**
3. **四个候选解释中，H2（替换仍在）在当前接口下完全不可观测，H3（读写守卫牺牲并行）在当前 runner 下不可执行**（见 §1.4、§3）。
4. **runner 把所有关键参数写死**：`--a-gap-cycles 0`、`--victim-pages 1`、`--chunk 4096`、aggressor pages ∈{1,8,16}（`scripts/mac_nonblock_pilot.py:243-256`）。一次调用 = 一个 BIT = 一次新鲜启动 = 冻结 15 例，无重试/无续跑（`:8`）。所以**"仅 JTAG 换 BIT"能做的唯一新实验就是换个 BIT 再跑一遍冻结 15 例**（复现）。更细的隔离需要新 BIT 或 runner 改动 → Codex 决定。
5. **两个 v1 BIT 的 guard 相同、容量/仲裁相同**，S→N 只在绕过使能常量上不同（`docs/research-n-prototype-spec-20261004.md:105,:113`；`docs/nonblock-dependency-diagnosis-20261004.md:96`）。因此 S→N 差值 = 同 guard 下的绕过效应；guard 成本**无法**用现有 BIT 单独隔离（没有 guard-off BIT，v0 编译失败，`docs/nonblock-dependency-diagnosis-20261004.md:97`）。
6. **科学阴性 ≠ 技术失败**：合法但"绕过增量为 0/为负"的样本不产生质量问题（`scripts/analyze_nonblock_pilot.py:48,:55,:1640-1642`），照样发布为负证据；只有 raw/exit/taint/helper 质量失败才使该 label 不可发布（`:1250-1302,:1723-1724`）。

---

## 1. 现有接口能支持什么 / 不能支持什么（逐条给证据）

### 1.1 每例可用计数（真实存在的两类窗口）

**P4M 硬件窗口 `record.p4m.raw_window[16]`**（`scripts/mac_nonblock_pilot.py:114-119`；字段图见 `P4M_BASELINE.md:29-49`）：
- `w0` magic `0x49444D4150344D01`（runner `:116,:389`；`P4M_BASELINE.md:33` 索引 448）
- `w1` 窗口周期数（= `raw_observation[0]`，runner `:391-393`）
- `w2` 窗口内 A issue、`w3` 窗口内 A done（`P4M_BASELINE.md:34`；`w3` == `p4m.a_completed_jobs_window`，`scripts/analyze_nonblock_pilot.py:124-137,:642-664`）
- `w13` V issued、`w14` V done（runner `:394-396`）
- `w15` flags（bit0 hasA / bit1 closed / bit2 strict carry，`P4M_BASELINE.md:48`）
- `w4..w12` 边界在途/ID/mask 等（`P4M_BASELINE.md:35-45`；analyzer 只原样保存不解释，`:1563`）

**非阻塞 helper 窗口 `helper.json.words[16]`**（ABI `idma-nonblock-window-v1`，`scripts/mac_nonblock_pilot.py:123-131`；`bench/idma_nonblock_read.c:69-82`）：
- `w2` mode（1=serial, 2=hit，`scripts/mac_nonblock_pilot.py:129`）、`w3` flags==4、`w4` seq、`w5` cycles
- `w6..w10` = fast 计数 [Vread, Vwrite, Aread, Awrite, Other]（`:130`）
- `w11..w13` = normal 计数 [V, A, Other]（`:131`）
- `w14` = accepted = Σ(fast)+Σ(normal)（守恒，`bench/idma_nonblock_read.c:149-166`）
- `w15` errors（`:143-147`）

### 1.2 "路径激活"到底能不能判

| 判断 | mode1 (banked-serial) | mode2 (banked-hit) |
|---|---|---|
| runner 要求 fast>0 | 否 | **否**（`scripts/mac_nonblock_pilot.py:484-497` 只用 6,7,11 / 8,9,12 求和） |
| runner 要求 fast==0 | — | 否 |
| analyzer 要求 fast==0 | **是**（`scripts/analyze_nonblock_pilot.py:380-383`） | 否（`:1649` 明确"Mode2 可大于 0"） |
| analyzer 要求 fast>0 | 否 | **否** |

结论：**只有"mode2 且 Σ(w6..w10) > 0"这条规则能把"快路径激活"与"全部走正常路径"分开，而这条规则现在不存在。** 分支(1)与(2)的判定必须由 Codex 显式加为预注册规则（可只做离线判据，不必改代码）。

### 1.3 能支持的推论

- **快路径是否被触发过**：仅当定义 F_N = Σ(w6..w10) 并只看它（不要混 w11..w13）。
- **是否存在整口收益**：S→N 的 **per-condition** `raw_window[1]`（同窗口 A 用 `w3`）差值（`scripts/analyze_nonblock_pilot.py:49-51,:1284-1292`）。长组每条件只有 n=3 轮（`:101-105,:1206-1213`），且**每个 label 只有一次运行/一次启动，跨 label 差值含运行间波动，脚本不做方差分析**（`:1643-1644`）。
- **满 0 机制窗口不得判通过**：runner/analyzer 已有 liveness guard（`scripts/mac_nonblock_pilot.py:484-497`；`scripts/analyze_nonblock_pilot.py:365-378`）；序号必须跨计划顺序严格递增（`:498-500`；`:1087-1101`）。

### 1.4 **不能**支持的推论（关键）

- **cache 命中率**：16 字窗口无 hit/miss 维；旧 0xE2B/0xE2C 不适用（见 §0.2）。
- **替换策略是否仍参与**（H2）：spec 要求真实被服务的 hit 必须参与既定替换、probe 不碰 PLRU（`docs/research-n-prototype-spec-20261004.md:51-53,:103`），但没有任何 PLRU/evict 可观测接口。**H2 在当前接口下不可观测。**
- **guard 是否牺牲并行**（H3）：guard 逻辑在 `rv_iommu_banked_ctl.sv:244-266`、`rv_iommu_trans_ctl_wrap.sv:621-666,:713-723`（`docs/nonblock-dependency-diagnosis-20261004.md:46-55`），会让 S 与 N 同时变慢（`:157-162`）。要隔离它必须比较 guard-on/off 两个 BIT；**不存在 guard-off BIT**（v0 失败构建另存，`:97`）。且 runner 把 `--a-gap-cycles` 写死 0（`scripts/mac_nonblock_pilot.py:249`），连"降 A 压力从而缓解 guard"的替代实验也**不能直接用现有 runner 执行**（详见 §3.3）。
- **"V 命中在 A 的 PTW 未完成时被真实前向服务"** 这一 spec §7.1 接受条件：需要 RTL 内部 `iotlb_lu_hit==0 && state_q==PT` 与具体 V 命中时序（`docs/research-n-prototype-spec-20261004.md:72`），CLI/helper 都观测不到；spec 也承认"快速路径计数仍需独立接口"（`:105`）。**属"无接口 → 不可执行"。**
- **真实 DDR/DMA/互连**：只有有限模型结论（`docs/nonblock-dependency-diagnosis-20261004.md:141,:143-149`）。
- **新颖性**：不成立（`docs/research-n-prototype-spec-20261004.md:95,:109`）。
- **bank 来源**：16 字窗口根本没有 bank 维，做不了"哪家银行服务"的统计。

---

## 2. 两个分支的下一实验（每个分支 1 个，最小而有判别力）

### 前置：S/N 主运行与汇总（Codex 已定，命令候选，参数逐条给证据）

两次调用、每次一个 BIT、一次新鲜启动、冻结 15 例（`scripts/mac_nonblock_pilot.py:8,:108,:218-240`）：

```sh
# S = banked-serial（绕过使能常量 0）
python3 scripts/mac_nonblock_pilot.py \
  --label banked-serial \
  --bit   <S>.bit \
  --reader <idma_nonblock_read ELF> \
  --out   out/n-prototype-20261004/run-S-r1 \
  --device /dev/idma0 \
  --interrupt-stalled-seed

# N = banked-hit（同一布局/guard，仅使能常量 1）
python3 scripts/mac_nonblock_pilot.py \
  --label banked-hit \
  --bit   <N>.bit \
  --reader <idma_nonblock_read ELF> \
  --out   out/n-prototype-20261004/run-N-r1 \
  --device /dev/idma0 \
  --interrupt-stalled-seed
```

参数证据：
- `--label` 取值 `banked-serial/banked-hit`：`scripts/mac_nonblock_pilot.py:108,:1648-1649`
- `--reader` 对两个 banked label **必需**：`:111,:1692-1693`；ABI 冻结为 `idma-nonblock-window-v1`：`:123`
- `--bit/--out`：`:1650-1653`；`--out` 必须不存在：`:1686-1687`
- `--device /dev/idma0`：`:54,:1657`
- JTAG 烧写由 runner 自动执行 `openFPGALoader -b genesys2 --freq 1000000 <bit>`：`:52,:60,:686-687`
- `--interrupt-stalled-seed`：`:1675-1679`（默认关；只在 Linux banner 后 `Saving random seed` 静默 30s 时发一次 CTRL-C，`:966-1004`；原版启动确实需要一次该中断，`docs/nonblock-pilot-plan-20261004.md:6`）
- 冻结用例参数（`--chunk 4096 / --victim-pages 1 / --a-gap-cycles 0 / --timeout-ms 60000 / aggressor pages 1/8/16`）：`:243-256,:141-152`；用例表：`:218-240`
- 不写 SD：`:34`（"runner never writes the SD card and never copies firmware"）

离线汇总（**原版 15 例不重跑**，用已有 original 目录即可）：
```sh
python3 scripts/analyze_nonblock_pilot.py \
  --original <已有 original-serial 输出目录> \
  --serial   out/n-prototype-20261004/run-S-r1 \
  --hit      out/n-prototype-20261004/run-N-r1 \
  --out      out/n-prototype-20261004/analysis-r1
```
参数证据：`scripts/analyze_nonblock_pilot.py:1665-1672`；布局/绕过定义与归因禁令：`:49-51,:1290-1292`；发布门：`:1206-1213,:1297-1298`；退出码 0/1/2：`:67-69,:1723-1724`。

**固定项（baseline）**：kernel `6.5.0-rc2-00093-gc0f5e13ea81f`（`scripts/mac_nonblock_pilot.py:64`）、taint 0（`:1044-1050`）、boot_id 每次必须变（`:1053-1055`）、caps（`:1058-1074`）、device、chunk、victim-pages=1、a-gap-cycles=0、transfers 64/4096、warmup 16/256、observer status==4（`:119,:369`）。

**统计窗口**：每个数字都取同一硬件窗口——A 用 `p4m.raw_window[3]`、周期用 `raw_window[1]`、V 吞吐 = transfers×4096 / (cycles/50 MHz)（`scripts/analyze_nonblock_pilot.py:37-44,:252-264,:642-664`；50 MHz 只作换算假设，`:93`）。helper 窗口语义为 `(open, close]`，序号取 `words[4]`（`:166,:1647-1650`），并要求 `helper.words[5] == raw_window[1]`（`scripts/mac_nonblock_pilot.py:470`）。长组每条件 n=3 轮、只做描述性中位/min/max、**不做显著性**（`:45,:1300`）。

---

### 分支 (1)：板端数据/退出全正常且 N 窗口 fast 接收非零

**判据（预注册，可离线）**：令 F_N = Σ(helper.words[6..10]) > 0；同时所有 case 满足 `exit==0 / taint_after=="0" / raw_window 校验通过 / helper 校验通过`（`scripts/mac_nonblock_pilot.py:1451-1461`；`scripts/analyze_nonblock_pilot.py:833-851`），且 analyzer 退出码 0。

**四假设的判别读法（先用现有数据，不需要新跑）**：
- H1 快路径未激活 → F_N==0。已由判据排除。
- H4 实际改善 → 要求 S→N 的 per-condition 绕过增量**同号且随 A 压力增大**（长组 Solo < A1 < A8 < A16；A 存在性由 `p4m.has_a`，`scripts/mac_nonblock_pilot.py:401-404`），并且 F_N 与周期下降同向。
- H2 替换仍在 / H3 guard 牺牲 → 表现为"...增量不随 A 增长，甚至在 A8/A16 变负"，**但这两个当前不可分开**（见 §1.4）。

**下一实验（唯一一个）：顺序反转的 S/N 新鲜启动复现对。**
理由：analyzer 自己声明"每个 label 只对应一次运行、一次新鲜启动；跨 label 差异含运行间波动，不做方差分析"（`scripts/analyze_nonblock_pilot.py:1643-1644`），所以任何绕过结论必须是**可复现**的。同 runner、同冻结 15 例、仅换 `--out` 与执行顺序：

```sh
# 反向顺序：先 N 后 S，各一次新鲜启动
python3 scripts/mac_nonblock_pilot.py --label banked-hit    --bit <N>.bit --reader <ELF> \
  --out out/n-prototype-20261004/run-N-r2 --device /dev/idma0 --interrupt-stalled-seed
python3 scripts/mac_nonblock_pilot.py --label banked-serial --bit <S>.bit --reader <ELF> \
  --out out/n-prototype-20261004/run-S-r2 --device /dev/idma0 --interrupt-stalled-seed

python3 scripts/analyze_nonblock_pilot.py \
  --original <已有 original 目录> \
  --serial out/n-prototype-20261004/run-S-r2 \
  --hit    out/n-prototype-20261004/run-N-r2 \
  --out    out/n-prototype-20261004/analysis-r2
```

**预注册判定**：在至少一个 A 存在的长组条件（A8 或 A16）上，(a) 绕过增量在两套 analysis 中**同号**；(b) F_N 在两套里都 > 0（新加的离线判条）；(c) 该增量大于两套之间的运行间散布。三者都满足 → 可写成"同布局同 guard 下绕过带来可复现的窗口增量"；任一不满足 → 记 **科学阴性**，不重跑、不换参数。

**能支持**：H4 vs 噪声；H1（F_N==0）。**不能支持**：H2 vs H3 的分离（需新 BIT）、cache 命中率、真实 DDR 行为、新颖性。

---

### 分支 (2)：数据全正常但 N fast 始终 0 或吞吐无改善

**判据**：F_N==0（跨全部 15 例），或长组每条件绕过增量 ≤0 且三 label 均技术合格。

**四假设的判别读法（先用现有数据）**：
- F_N==0 且在 **Solo**（无 A）也 0 → 要么快路径与 A 无关地不工作，要么其资格条件要求"有 A 在途"（spec 未给资格条件，`docs/research-n-prototype-spec-20261004.md:103`）→ 由 §1.4 不可判，转 Codex。
- F_N==0 只在 A1/A8/A16（guard 激活）而 Solo>0 → 指向 H3（guard/调度），但**当前 runner 无法进一步放松 guard**（`scripts/mac_nonblock_pilot.py:249` 写死 `--a-gap-cycles 0`）。
- F_N>0 但增量 ≤0 → 指向 H2（换了但没赚）或"服务了本来就命中的那些 hit"。

**下一实验（唯一一个）：N 单侧新鲜启动复现 + 同窗口 fast 记账按条件分解。**
```sh
python3 scripts/mac_nonblock_pilot.py --label banked-hit --bit <N>.bit --reader <ELF> \
  --out out/n-prototype-20261004/run-N-r2 --device /dev/idma0 --interrupt-stalled-seed

python3 scripts/analyze_nonblock_pilot.py \
  --original <已有 original 目录> \
  --serial   out/n-prototype-20261004/run-S-r1 \
  --hit      out/n-prototype-20261004/run-N-r2 \
  --out      out/n-prototype-20261004/analysis-b2
```
读数（新增离线判条，不改 RTL）：把 `helper.words[6..10]` 与 `words[11..13]` 与 `p4m.has_a`/`raw_window[3]` 按 15 例逐条对照，看 F_N 是否在任何 **A 存在**的窗口非零、以及 `words[4]` 序号是否稳定递增（`:498-500`）。

**预注册判定**：若两个新鲜启动里 F_N 在全部 A 存在的窗口都为 0，则"solved on the board"的最简解释不再是 guard（guard 无法在无 A 时被触发），而是**快路径未被选中或未被实现/使能**——此时不再板测调参，升级为 BIT 级资格/计数接口问题交 Codex（对应 spec"快速路径计数仍需独立接口"，`docs/research-n-prototype-spec-20261004.md:105`）。若 F_N 只在 guard 激活的条件为 0，则记录为 H3 方向证据，并请 Codex 决定是否做 guard-relaxed BIT。

#### 3.3 明确"不可执行"的更强替代（不要误当成可跑）
- `--a-gap-cycles > 0` 的降 A 压力实验：runner 的 `build_case_argv` 把该值固定为 `'0'`（`scripts/mac_nonblock_pilot.py:249`），**现有 runner 跑不了**。要跑必须：(a) Codex 给 runner 加一个参数透传，或 (b) 手工串口跑 CLI 并手工完成 helper 上传（上传流程 `:1094-1262` 是 base64 heredoc，复杂）。**是否采用由 Codex 决定。**
- guard-on/off 对照：无 guard-off BIT（`docs/nonblock-dependency-diagnosis-20261004.md:97`），**不可执行**。
- 替换策略/PLRU 观测、命中率、V-hit-during-A-PTW 时序：**无接口，不可执行**。
- 改 `--victim-pages`（本应 >1 才能让 16 项 IOTLB 发生替换，spec `docs/research-n-prototype-spec-20261004.md:58`）也不在冻结计划里（runner 写死 1，`:250`），属"调参"，本文不建议。

---

## 4. 科学阴性 vs 技术失败的处理

| | 科学阴性（合法但无效） | 技术失败 |
|---|---|---|
| 判定 | 15/15 complete、`exit==0`、`taint_after=="0"`、raw_window 通过、helper 通过、analyzer exit 0 | 任一质量问题：CLI exit≠0、taint≠0、record schema/success/status/data_errors/quarantined 不符、raw_window 形状/magic/observer_status/closed、helper 缺失/ABI/mode/flags/errors/守恒/序号/liveness、`exit.txt` 不一致 |
| 依据 | `scripts/analyze_nonblock_pilot.py:48,:55,:1640-1642,:1723-1724`；`scripts/mac_nonblock_pilot.py:1451-1461` | 同上 + `scripts/mac_nonblock_pilot.py:328-405,:441-501` |
| 处置 | 照常发布（可为 0/负增量），**不重跑、不换参数、不剔除** | 保留 raw，`effects` 不发布（只列 reasons），**runner 内不得重试/续跑**（`scripts/mac_nonblock_pilot.py:8`；`scripts/analyze_nonblock_pilot.py:1250-1300`） |
| 退出码 | 0 | 1（`scripts/analyze_nonblock_pilot.py:1723-1724`） |

---

## 5. 假设与未决问题

**假设**
- A1：`idma-nonblock-window-v1` 的静态 ELF 存在并可上传（runner 注释"ELF is built later"，`scripts/mac_nonblock_pilot.py:121-123`；本地 helper guard 曾 PASS，`docs/nonblock-dependency-diagnosis-20261004.md:41`）。
- A2：两个 v1 BIT 完成且时序可签核（当前仍在编译，`docs/nonblock-pilot-plan-20261004.md:98`；`docs/nonblock-dependency-diagnosis-20261004.md:168`）。
- A3：已有 original-serial 输出目录可用于 analyzer `--original`（analyzer 强制需要，`scripts/analyze_nonblock_pilot.py:1665-1666`）。
- A4：P4M 字图沿用 `P4M_BASELINE.md:29-49`（该文档时代为 00088/observer v7），当前为 00093；magic 与 runner 校验一致，但**跨版本字段语义未在本次阅读中逐一复核**。

**待 Codex 决定**
1. `helper.words[6..10]`/`[11..13]` 的**精确事件定义**是什么？"fast" 是否就是命中绕过接收事件？（spec `:105` 说该计数"仍需独立接口"，未确认已定义。）
2. 快路径的**资格条件**是否要求"有 A 在途/有 miss 在途"？这直接决定 Solo 里 F_N==0 是否正常。
3. mode2 是否应把 **F_N==0 判为质量失败**？现在不是（§1.2）。
4. 是否存在**guard-off BIT**或是否允许 Codex 构建？否则 H3 无法与 H4 分离。
5. 是否允许给 runner 加 `--a-gap-cycles` 透传（否则 §3.3 的 guard 放松实验不可执行）？
6. 真实被服务的 hit 是否参与 PLRU（spec `:103`）？有无任何可观测？
7. 是否把本文的 F_N 判据写进分析脚本/流程（当前缺，使分支(1)/(2)无法自动区分）？

**验收检查（建议）**
- 每次运行：runner 退出码 0；`manifest.json` planned==completed==15；无 `STOPPED.txt`；`summary.status=="complete"`；每例 taint 0（`scripts/mac_nonblock_pilot.py:1514-1534,:1536-1580`）。
- 汇总：`analyze_nonblock_pilot.py` 退出码 0，`completeness.all_cases_complete==true`，`quality_issues` 为空（`:1112-1130,:1723-1724`）。
- 分支(1) 声明成立：两套 analysis 在 ≥1 个 A 存在的长组条件上绕过增量同号且超过运行间散布，且 F_N>0 两次。
- 分支(2) 声明成立：两套 analysis 在全部 A 存在的窗口 F_N==0，且无技术失败 → 升级为接口/使能问题，不调参。