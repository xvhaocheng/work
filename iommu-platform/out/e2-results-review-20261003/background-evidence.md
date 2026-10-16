## 0. 数据集判定：现在能确认的是“旧 E2”，不是“新64 E2”

- **旧 E2 上板数据**：来自 `out/mac-research-v1-20261002/` 与 `out/mac-research-cont-20261002/`，复审汇总在 `out/research-board-review-20261002/`。  
- **新64 E2**：目前附件中没有找到实际上板 raw；`docs/research-two-bit-v2.md:3` 明确说两 BIT 实现和主机测试完成、**未上板**，收益待实验验证。`docs/research-two-bit-v2.md:49` 只给出冻结的 64 项 E2 目标清单。  
- **不能宣称 new64 已在 Dell 完成**。附件没有 new64 的 `collection/unpacked`、`cases.tsv`、`plan.selected.tsv`、`report.json`、`console.raw.log` 或 per-case JSON。  
- asyncuserquestion 仍 pending，因此下面按“旧 E2 / 新64 目标”分支准备，最终实验决策由 Codex 定。

---

## 1. 旧 E2 结果：精确计数（排除 cost Solo）

### 1.1 全旧 84 项与 Solo 计数

| 范围 | 结果 | 证据 |
|---|---:|---|
| 全 84 项 | 80 runner 通过，2 runner 失败，2 未运行；**不是 84/84** | `out/mac-research-cont-20261002/README.md:13`、`out/mac-research-cont-20261002/DELL_RECEIPT.md:5`、`out/research-board-review-20261002/cross-segment-summary.json:55-58` |
| 排除 cost Solo 的非 Solo 80 项 | 80/80 runner 通过 | 同上；E2 63 + cost非Solo 17，见 `out/mac-research-cont-20261002/README.md:13` |
| cost Solo 4 项 | 2 失败：`short-cost-solo`、`cost-b01-solo`；2 未运行：`cost-b02-solo`、`cost-b03-solo` | `out/mac-research-v1-20261002/README.md:5-8`、`out/mac-research-cont-20261002/README.md:8-11`、`cross-segment-summary.json:59-62` |
| E2 专项 | **63/63 runner 通过，0 E2 runner 失败**；但 21 个 target 级 unknown/warning | `cross-segment-summary.json:63-127` |
| E2 组成 | 短 3 + 主 60 | `out/mac-research-cont-20261002/README.md:13` |

> 注意：`cross-segment-summary.json:56-58` 只给 `ok=80, failed=2`，未列失败 ID；失败 ID 来自两个 README。cross summary 的 `missing` 明确只有 `cost-b02-solo`、`cost-b03-solo`（`:59-62`）。

### 1.2 E2 条件级结果

E2 目标级统计来自 `cross-segment-summary.json:63-127`：

| E2 条件 | runs | target 分类 | unknowns |
|---|---:|---|---:|
| short/w0 | 1 | `resident_hit_negative`: 2 | 0 |
| short/w1 | 1 | `resident_hit_negative`: 2 | 0 |
| short/w16 | 1 | `A_evicted_then_first_miss`: 2 | 1 |
| main/w0 | 10 | `resident_hit_negative`: 20 | 0 |
| main/w1 | 10 | `resident_hit_negative`: 20 | 0 |
| main/w4 | 10 | `resident_hit_negative`: 20 | 0 |
| main/w7 | 10 | `resident_hit_negative`: 20 | 0 |
| main/w8 | 10 | `A_evicted_then_first_miss`: 20 | 10 |
| main/w16 | 10 | `A_evicted_then_first_miss`: 20 | 10 |

汇总：
- E2 执行 63 runs。
- target 条目 126 个。
- `resident_hit_negative`：84 个。
- `A_evicted_then_first_miss`：42 个。
- unknown/warning：**21 个**（short/w16 1 + main/w8 10 + main/w16 10）。

### 1.3 21 个 warning 的 advisory 解释

`out/research-board-review-20261002/advisory-cross-check.json:2-191` 列出 21 条 `e2_warning_explained_by_dst_before_src`，每条都有 `raw_replay_checks_passed: true`。  
这些 ID 包括 `short-e2-w16`，以及 `e2-b01-w8` 到 `e2-b10-w16` 的 20 条 W8/W16 例。

解释模式是：
- dst 先 probe，dst refill/再插入，src 后 probe；
- 原 analyzer 仍保留 warning 字符串，没有改写原结果；
- 这是只读 audit，没有改分析代码、驱动或 RTL；`advisory-cross-check.json:377` 明确：“Read-only audit; no analysis code, driver or RTL changed. Existing warning strings retained.”

典型例子 `e2-b01-w8`：`cross-segment-summary.json:562-862`
- probe order `[1, 0]`，不是简单 0→1。
- src 的 A removal event 是 17，dst 是 16；A 阶段事件 16/17 移除了两个目标。
- dst probe 在 `5030007`，dst refill 在 `5030411`，src probe 在 `5030426`。
- dst 的 `presence_cross_check` 出现 deadline `False` 到 probe0 live bitmap `0x20` 的 presence 变化，原 analyzer 记为 unknown；advisory 认为这是 dst-before-src 时序解释。
- 该例的 `note` 说 `ready_to_probe` 是 CPU/driver delay，不是 A-window 时间（`:572`）。

**边界**：这些是 advisory 解释，不是独立硬件语义验收；`cross-segment-summary.json:866` 的原 scope 写明：科学结论仍需 critical review。

---

## 2. 旧 E2 的实验序列与混淆项

### 2.1 实际序列

1. **首次上板**：运行 `platform-research all /tmp/research-v1-01`。  
   - 前 3 项 `short-e2-w0/w1/w16` CLI exit0，runner 计 3 项完成。  
   - 第 4 项 `short-cost-solo` CLI exit1，`idma_research: run: Invalid argument`。  
   - runner `final_exit=1`, `cases_done=3`, `total=84`, 80 项未运行。  
   证据：`out/mac-research-v1-20261002/README.md:5-8`。

2. **后续补跑 80 项**：复用 Linux00090 和 research BIT，没有重载、重编译、修改驱动/RTL 或实验参数，没有重跑前 4 项。  
   证据：`out/mac-research-cont-20261002/README.md:3-7`。

3. **tail80 临时选择**：选 ordinal5..84，共 80 项。  
   - ordinal5..66 共 62 项完成：2 项 cost 短试次 + 60 项 E2 主试次。  
   - ordinal67 `cost-b01-solo` CLI exit1，runner 停止。  
   证据：`out/mac-research-cont-20261002/README.md:7-9`。

4. **pair15 临时选择**：选 ordinal68..84，去掉 ordinal78/83，共 15 项。  
   - 15/15 完成，runner/host exit0，`collection_failed=0`。  
   - ordinal78/83 是两个相同参数的主 Solo，明确未运行。  
   证据：`out/mac-research-cont-20261002/README.md:10-11`。

### 2.2 混淆/边界

- **分段执行**：初始 4 项、tail80、pair15 是三段，不是一次连续 84 项运行。
- **STOPPED/plan mismatch**：  
  - initial 段存在 `STOPPED.txt`，run stopped before completion。  
  - tail80 段 `plan.selected.tsv` 不匹配前 84 generator rows，`cases.tsv` ids/order 与 frozen prefix 不同，且存在 `STOPPED.txt`。  
  - pair15 段同样有 `plan.selected.tsv` 不匹配和 `cases.tsv` ids/order 不同。  
  证据：`cross-segment-summary.json:12-17, 28-34, 46-51`。
- **但段内选择行/哈希**：三段均记录 `all_selected_rows_match_frozen_original: true`、`executed_is_prefix_of_own_selection: true`、`input_hashes_unchanged: true`。  
  证据：`cross-segment-summary.json:8-10, 23-25, 41-43`。  
  这意味着复审对“实际选中的行”做了核对，但不等于全局 84/84 完成。
- **JTAG/加载**：首次 JTAG 默认 6MHz 未完成配置 DONE0，后按 1MHz 加载同一 BIT 成功。  
  证据：`out/mac-research-v1-20261002/README.md:10-11`。
- **UART 警告**：原始日志含 hrtimer 警告，未清理或声称无内核警告。  
  证据：`out/mac-research-cont-20261002/README.md:17`。
- **无独立语义验收**：多个 README 明确未做独立 JSON 语义验收，不能称完整实验通过。  
  证据：`out/mac-research-v1-20261002/README.md:5`、`out/mac-research-cont-20261002/README.md:13`、`DELL_RECEIPT.md:5`。

---

## 3. 新64 E2 冻结目标：它到底新增/改变什么

来自 `docs/research-two-bit-v2.md`（v2 合同，未上板）：

- **总 64 项**：`docs/research-two-bit-v2.md:49`  
  - 短 4：W0 正向 N0、W1 正向 N112、W8 反向 N8、W16 反向 N1。  
  - 边界 6 条件 × 5：W0 正向 N0、W1 正向 N112、W7 正向 N112、W8 正向 N112、W7 反向 N112、W8 反向 N112。  
  - 前缀 W8/16 正向各 N1/8/16 各 5，共 30。  
  - 每组 5 轮循环移位顺序；所有用例同时采集首访计时。  
- **不重跑旧60，新对照自成版本，旧60不并入**：`:41`、`:49`。
- **A 顺序 0 正向 / 1 反向**：反向在 batch 起始从最后页开始，页内不变；短前缀记录真实覆盖，不假称完整工作集。`:15`
- **E2 新增硬件时间块 D80..D8D**：`IDMAE2T1`，记录 probe issue epoch、首次 V select epoch、V done epoch、done-issue 周期、first_select-issue 周期、V `REQ_TRANS/AWAIT_RESP` 占用、V `COMPLETE` 周期、V select/hit/miss 次数；仅成功接受 PROBE 后、V issue 至 done 区间记录；仅 role1 正式执行。`:17`
- **计时解释边界**：只测首次 select 前等待、实际翻译控制占用和整次 DMA 完成时间；总时长减翻译不等于纯等待；5,000,000 周期不是替换代价。`:19`
- **X3 随同采集**：`:41` 提到 E2 新版小量同版对照 + W7/8 顺序复核 + W8/16 前缀，X3 随同采集。附件未给 X3 定义，需查 runner/生成器。
- **新 UAPI/能力**：`IDMA_IOC_RUN_RESEARCH2` 0x0f；`IDMA2CAP` magic/role/feature bits；bit1 E2 probe timing。`:9-11`
- **两 BIT**：role1 是 E2 profile，role2 是 method；Mac 先保存第一段后才切第二 BIT。`:7`

### 通俗解释 W / N / 反向 / 前缀

- **W**：E2 case 档位，如 W0/1/4/7/8/16。旧汇总里只有 `main/w0..w16`、`short/w0..w16` 这些 condition 名。v2 合同把 W 与 A 次数 N 一起配置：W0 仅 N0，其他 W 可配 N1/8/16/112。W 的具体硬件含义附件未展开，不能擅自解释成页数或周期数。  
- **N**：合同写作“A次数”，如 N0、N1、N8、N16、N112。  
- **正向/反向**：A 访问顺序，0 正向、1 反向；反向从 batch 起始的最后页开始，页内顺序不变。  
- **前缀**：W8/W16 正向只做 N1/8/16 的短前缀，用于记录真实覆盖，不冒充完整工作集。  
- **E2 timing**：新增硬件计时块，不只是旧 analyzer 里的 `ready_to_probe` / `probe_to_freeze`；旧例明确 `ready_to_probe` 是 CPU/driver delay，不是 A-window 时间。

---

## 4. 结构化对比：旧 E2 已有结果 vs 新64 E2 冻结目标

| 维度 | 旧 E2（已上板汇总） | 新64 E2（v2 冻结目标，未上板） | 证据/边界 |
|---|---|---|---|
| 数据状态 | 有 runner/analyzer 汇总，无完整 raw 附件 | 无 raw；文档说未上板 | `cross-segment-summary.json:2,866`；`docs/research-two-bit-v2.md:3` |
| 规模 | 短3 + 主60 = 63 | 短4 + 边界30 + 前缀30 = 64 | `out/mac-research-cont-20261002/README.md:13`；`docs/research-two-bit-v2.md:49` |
| 短项 | short/w0,w1,w16 | W0 正向 N0；W1 正向 N112；W8 反向 N8；W16 反向 N1 | `cross-segment-summary.json:63-84`；`docs/research-two-bit-v2.md:49` |
| 主项 | main/w0,w1,w4,w7,w8,w16 各10 | 边界 W0/W1/W7/W8 正向 + W7/W8 反向，各5 | `cross-segment-summary.json:85-126`；`docs/research-two-bit-v2.md:49` |
| 前缀覆盖 | 附件未给 N 分层；主 W8/W16 各10 | W8/16 正向 N1/8/16 各5，共30 | `docs/research-two-bit-v2.md:49` |
| A 顺序 | 附件旧汇总未见 reverse 字段；不能断言旧 E2 做了反向 | 显式正向/反向；反向 batch 起始从最后页开始 | `docs/research-two-bit-v2.md:15` |
| 计时 | 有 astart/a_elapsed/deadline/probestart/freeze/ready_to_probe/probe_to_freeze | 新增 D80..D8D 硬件 E2 timing；所有用例同采首访计时 | `cross-segment-summary.json:562-573`；`docs/research-two-bit-v2.md:17,49` |
| 计时解释 | 明确 ready_to_probe 是 CPU/driver delay，不是 A-window 时间 | 明确总时长减翻译不等于纯等待；5,000,000 周期不是替换代价 | `cross-segment-summary.json:572`；`docs/research-two-bit-v2.md:19` |
| 采集伴随 | 旧有 hrtimer 警告，未清理 | X3 随同采集；X3 定义附件缺失 | `out/mac-research-cont-20261002/README.md:17`；`docs/research-two-bit-v2.md:41` |
| 版本关系 | 旧60不并入新64 | 新对照自成版本，不重跑旧60 | `docs/research-two-bit-v2.md:41,49` |
| runner 结果 | E2 63/63 runner 通过；21 target unknowns | 无结果 | `cross-segment-summary.json:63-127` |
| 可宣称 | 只能宣称 runner/CLI 级与 advisory 解释；不能称 84/84 或独立语义验收 | 不能宣称任何结果 | `cross-segment-summary.json:866` |

---

## 5. 需要 spotcheck 的原始文件与缺失 raw

### 5.1 已有汇总/摘录（可直接复核）

- `out/mac-research-v1-20261002/README.md:5-8`：首次 4 项、失败与未运行。
- `out/mac-research-cont-20261002/README.md:7-13`：tail80/pair15、累计计数、E2/cost 拆分。
- `out/mac-research-cont-20261002/DELL_RECEIPT.md:3-5`：回传校验与累计计数。
- `out/research-board-review-20261002/cross-segment-summary.json:55-62`：`ok=80, failed=2, missing=cost-b02-solo/cost-b03-solo`。
- `out/research-board-review-20261002/cross-segment-summary.json:63-127`：E2 条件级统计。
- `out/research-board-review-20261002/cross-segment-summary.json:562-862`：`e2-b01-w8` warning 例子。
- `out/research-board-review-20261002/advisory-cross-check.json:2-191`：21 条 dst-before-src 解释。
- `out/research-board-review-20261002/advisory-cross-check.json:377`：只读 audit 边界。
- `out/research-board-review-20261002/critical-excerpts.txt:1-158`：analyzer 和 RTL 摘录。
- `docs/research-two-bit-v2.md:3,7,9,11,15,17,19,41,43,49`：新64 冻结合同。

### 5.2 建议 spotcheck 的 raw 段（附件未含 raw）

- `out/mac-research-v1-20261002/collection/unpacked/`：初始 4 项。
- `out/mac-research-cont-20261002/collection/unpacked/research-tail80-01/`：tail80 63 项。
- `out/mac-research-cont-20261002/collection/unpacked/research-pair15-01/`：pair15 15 项。
- 每段重点看：`cases.tsv`、`plan.selected.tsv`、per-case JSON、`report.json`、`console.raw.log`、stderr/exit/dmesg。  
  证据说明见 `out/mac-research-cont-20261002/README.md:17-19`。

### 5.3 缺失 raw

- 附件没有旧 E2 的完整 UART、`cases.tsv`、`plan.selected.tsv`、per-case JSON、`report.json`。
- 附件没有新64 E2 的任何 raw。
- 因此本合成只基于汇总/摘录，不能替代 raw 复核，也没有 fake validation。

### 5.4 建议查找 new64 的路径（未确认存在）

- Dell：`/home/xvhaocheng/work/iommu-platform/out/` 下查找 v2/new64 E2 collection，如 `collection/unpacked`、`report.json`、`console.raw.log`、`plan.selected.tsv`、`cases.tsv`。  
- Mac：按 `docs/research-two-bit-v2.md:7`，Mac 脚本应在切第二 BIT 前保存第一段；可查找 `out/` 下新日期/v2 段目录。  
- 具体目录名附件未给出，不能凭空断言。

---

## 6. 候选 E2 下一步：取决于用户指哪套数据

### 分支 A：用户指旧 E2（更可能，因为只有旧数据在主机上）

1. **不重跑旧60**；做现有 raw 的独立 spotcheck/语义复核。  
2. 校验三段 raw 的 `plan.selected.tsv` / `cases.tsv` / per-case JSON / `console.raw.log`。
3. 对 21 个 unknown 做 raw 级复核：确认 dst-before-src、refill 事件、probe 顺序，决定保留 warning 还是解释性标注。
4. 把 cost Solo 的 2 失败/2 未运行与 E2 63 项严格分开；E2 结论不背 Solo 失败。
5. 可宣称边界：E2 63/63 runner 通过、21 target unknown/advisory 解释；不能宣称 84/84 或独立语义验收。

### 分支 B：用户指新64 E2

1. **先定位/取回 new64 原始结果**；若找不到，结论只能是“未定位/未上板/无结果”，不能分析。
2. 若找到，先验：role1、`IDMA_IOC_RUN_RESEARCH2` 0x0f、`IDMA2CAP`、`IDMAE2T1`、plan hash、64 项、X3、短4/边界30/前缀30、D80..D8D timing。
3. 再按 W/N/正向反向/前缀分组统计；new64 与旧60 不合并比较。
4. 新64 的新增价值主要在：反向 A 顺序、W7/W8 边界复核、W8/16 前缀 N1/8/16、硬件 E2 timing、X3 同采。

### 分支 C：用户混指两套

- 明确告知：旧 E2 = 63 项已上板汇总；新64 = 64 项 v2 冻结目标、未上板/未定位。  
- 不能把旧结果当作新64结果，也不能把新64目标当作已完成。  
- 最终是否先分析旧 E2、还是先取回新64，由 Codex 定。

本次不修改任何文件；以上只是 evidence synthesis。