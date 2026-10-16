# v4 有界修订交付（中文报告 + 完整代码工件）

本批交付 6 个工件，全部为**提案**，本 worker 未执行任何命令、未编译、未仿真、未上板、未改动任何文件：

| FILE | 类型 |
|---|---|
| `out/n-prototype-20261004/v4-results-review.md` | 完整中文报告（替换此前草稿） |
| `bench/idma_nonblock_read.c` | 完整结果文件（含 `--diagnostics`） |
| `scripts/mac_nonblock_pilot.py` | 6 个精确 patch hunk（行号与原文件一致，未改协议/15 例计划/首错即停/不自动重试/不重编 Linux） |
| `tests/nonblock_observer_top.sv` | 完整结果文件 |
| `tests/nonblock_observer_test.cpp` | 完整结果文件 |
| `tests/test_nonblock_diag_validator.py` | 新增完整小单元测试文件 |

````
## FILE: out/n-prototype-20261004/v4-results-review.md
```markdown
# RESULTS_REVIEW.md（中文 · v4 有界修订版 · 仅基于已提供材料）

> 本文件是对此前报告的**有界修订**，不是新一轮实验报告，也不是未来承诺。修订范围仅限：
> 1. 纠正静态不相容的**错误归因**：正确的不相容点是 live `msiptp.mode=1`（flat MSI 打开）与 v3 守卫静态要求 `msi_mode==0`；**不是** DDTP mode 2 与某个"要求 0"的不相容（DDTP mode 2 在 v3 与 v4 中都被支持）。
> 2. 删除没有材料支撑的"内核与 bit 哈希未变"表述，改为描述 original-03 的**复用启动/复用 flash 记录**事实与**记录到的内核 label**。
> 3. 把含糊的 "prior-work hit" 措辞替换为：hit-under-miss 属**已知既有工作**，因此本原型**不宣称 novelty**。
> 4. 改写为 **v4 已实现代码的实际状态**（以所提供文件可核对者为限），并明确 **not-built / not-on-board**。
> 5. 修正 seen bit0 的含义：**base S1 PTW 活动**，可能来自隐式/调试访问，**不是"仅普通通路"**。
> 6. 删除已过时的 D-1/D-2 推测项：诊断 bank 已在 RTL 中实现，diag 输入端口已提供。
>
> 口径：离线只读汇总 + 静态代码阅读。**本 worker 未执行任何编译、仿真、上板、重跑、重放或文件修改**；未执行即未通过，任何"通过"都必须由实际运行日志支撑。性能数字沿用 50 MHz 假设（1 cycle = 20 ns，推导量）。A 速度只用同一窗口 A 完成字节速率，**不是**总线吞吐。

## 0. 数据状态与边界

- original / banked-serial / banked-hit 三份 pilot 均为 15/15 complete、0 failed、taint 0，数据有效。
- 现有 raw pilot 合计 45 例全部 complete；N 侧 `fastcounts=[0,0,0,0,0]`。
- 本轮不因 fast=0 直接归因；不宣称 novelty；hit-under-miss（未命中期间继续服务命中）是**已知既有方法**，因此本轮既不宣称新颖点，也不提出任何"在既有 miss baseline 上取得新 hit"的主张。
- 旧 raw HPM / miss / PTW 字段在 nonblock 结构下不作为机制证据。
- 不做显著性、不剔除合法样本、不重跑、不重放旧数据。

## 1. 运行身份与完整度

| label | 目录 | BIT | run_id | 15/15 | taint | helper |
|---|---|---|---|---|---|---|
| original-serial | `nonblock-pilot-20261004-original-03` | `original-serial.bit` | `a72442e2e112f401` | 是 | 0 | 无 |
| banked-serial | `nonblock-pilot-20261004-serial-v3-01` | `banked-serial-v3.bit` | `3ae491ffaa3df550` | 是 | 0 | mode 1 |
| banked-hit | `nonblock-pilot-20261004-hit-v3-01` | `banked-hit-v3.bit` | `cc371e91173ce50a` | 是 | 0 | mode 2 |

来源：三份 `summary.json` 的 `status:"complete"`、`completed:15/failed:0`；`analysis-v3-pilot/nonblock-pilot-analysis.json` 的 `completeness.all_cases_complete:true`。

## 2. 长组结果（每条件三轮；median[min,max]）

### Solo（A jobs 恒 0）

| label | cycles median[min,max] | V MiB/s |
|---|---|---|
| original | 2828763 [2828343, 2832793] | 282.809 [282.407, 282.851] |
| serial | 2825110 [2820368, 2825919] | 283.175 [283.094, 283.651] |
| hit | 2825984 [2825890, 2831598] | 283.087 [282.526, 283.097] |

### A1

| label | cycles median[min,max] | V = A MiB/s | A jobs |
|---|---|---|---|
| original | 5359179 [5358650, 5360287] | 149.277 [149.246, 149.291] | 4096×3 |
| serial | 5584177 [5581750, 5585686] | 143.262 [143.223, 143.324] | 4096×3 |
| hit | 5584794 [5579282, 5586215] | 143.246 [143.210, 143.388] | 4096×3 |

### A8

| label | cycles median[min,max] | V = A MiB/s | A jobs |
|---|---|---|---|
| original | 7349054 [7347789, 7353256] | 108.858 [108.795, 108.876] | 4096×3 |
| serial | 7667658 [7666937, 7669765] | 104.334 [104.306, 104.344] | 4096×3 |
| hit | 7667630 [7667627, 7669858] | 104.335 [104.304, 104.335] | 4096×3 |

### A16

| label | cycles median[min,max] | V = A MiB/s | A jobs |
|---|---|---|---|
| original | 7945520 [7942861, 7946297] | 100.686 [100.676, 100.719] | 4096×3 |
| serial | 8274495 [8272332, 8275137] | 96.683 [96.675, 96.708] | 4096×3 |
| hit | 8275122 [8272399, 8277440] | 96.675 [96.648, 96.707] | 4096×3 |

说明：A 条件里同窗口 A 完成为 4096，字节 = 4096×4096 = 16777216 B，与 V 固定工作量相同，故 A MiB/s 数值上等于 V MiB/s。这只是"同窗口完成量相同"，不是命中率，也不是总线吞吐。

## 3. 两段差值（仅描述性，禁止越界解释）

| 条件 | S/原版变化（结构+缓冲） = serial − original | N/S变化（本轮未触发绕过） = hit − serial |
|---|---|---|
| Solo | +0.366 MiB/s（+0.13%） | −0.088（−0.03%） |
| A1 | −6.015（−4.03%） | −0.016（−0.01%） |
| A8 | −4.523（−4.16%） | +0.0004（+0.0004%） |
| A16 | −4.003（−3.98%） | −0.007（−0.008%） |

对应周期数差：A 条件 S/原版方向约 +4.1%〜+4.3%；N/S 方向 |Δ| ≤ 0.02%。

**强制限制（必须随数字一起写）**

- original→S 同时包含银行化 + 缓冲存储/容量/布局；两次各自新鲜启动、物理分配不同、固定顺序、n=3 ⇒ 不得声称显著性、机制因果或性能收益。
- N/S 差值在本轮 **不构成绕过效果证据**：N 的全部 fast 计数为 0，本轮未触发可观测绕过；该差值只能作为描述性窗口周期差保留。
- 只有 S 与 N 同为 512 缓冲时，S→N 的差才可能分离绕过增量；即便如此，本轮 S→N 差异非常接近 0，且 fast 计数全 0。
- A 速度只用同一窗口 A 吞吐评价，不用跨窗口 A 完成数比较。
- 不宣称 novelty：hit-under-miss 是已知既有方法；本轮不主张任何 prior-work hit。

## 4. Live 只读 DC/MMIO 事实（只读快照）与**修正后的**静态不相容点

已提供的只读 live DC/MMIO 证据：

- `out/n-prototype-20261004/nonblock-live-dc-20261004/mmio.json`：
  - `0x50010000` caps = `0x000001F8E6420210` → 与 CAPMSIFLAT=1 相符。
  - `0x50010010` ddtp = `0x0000000020AC4002`。
- `out/n-prototype-20261004/nonblock-live-dc-20261004/dc.json`：
  - `ddtp` = 548159490 = `0x20AC4002`；mode = 2；root = `0x82b10000`；stride = 64 B。
  - DID10：`tc=0x1`、`iohgatp=0x0`、`ta=0x1000`、`fsc=0x8000000000082B89`、`msiptp=0x1000000000082B88`、`msi_addr_mask=0xFF`、`msi_addr_pattern=0x28000`。
  - DID11：`tc=0x1`、`iohgatp=0x0`、`ta=0x2000`、`fsc=0x8000000000082B8D`、`msiptp=0x1000000000082B8B`、`msi_addr_mask=0xFF`、`msi_addr_pattern=0x28000`。
- `out/n-prototype-20261004/v4-live-context-evidence.json`：
  - `msi_modes = {"10": 1, "11": 1}`；即两个上下文的 `msiptp = 0x1000000000082B88 / 0x1000000000082B8B`，`msiptp.mode = 1`（flat MSI 已启用）。

**静态不相容点（修正，替换此前错误归因）**

- **正确的不相容点**：live DC 的 `msiptp.mode = 1`（flat MSI 被启用）与 v3 快速路径守卫的静态条件 `msi_mode == 0`（要求 MSI 关闭）之间的不相容。这是**静态配置层**的不相容：在 flat MSI 打开（`msiptp.mode=1`）时，v3 的快速路径判定前提不成立。该结论与 `v4-live-context-evidence.json` 自带的 `conclusion` 字段（"live RAM DCs use flat MSI mode1, excluded by v3 mode0-only fast guard"）一致。
- **不是** DDTP 的问题：live 的 `ddtp.mode = 2`、`root = 0x82b10000`、extended 64 B stride 在 **v3 与 v4 中都是被支持的形态**。此前报告写成的"与旧 fast 守卫所要求的 DDTP mode 0 静态条件相比，当前 live DC 的 DDTP mode 2 构成静态配置不相容"是**错误归因**，本版废止该表述。
- 不得由此断言（no sole-cause / no opportunity-coverage / no cache proof）：
  - 不得声称运行时 cache 中的 DC 精确等于 RAM 中的 DC；
  - 不得声称已覆盖全部候选机会（candidate opportunities）；
  - 不得声称这是唯一原因（sole cause）。
  live 证据只是**单点只读快照**。
- DID10/11 的 `iohgatp=0`（Bare）、`tc=1`（V）与 flat MSI 快路径的若干静态前提相容，但仍不能据此宣称快路径必然命中。

## 5. Linux 页表 U 位修正：移除此前不支持的候选

- `linux/arch/riscv/include/asm/pgtable.h:157`：
  ```c
  #define _PAGE_BASE (_PAGE_PRESENT | _PAGE_ACCESSED | _PAGE_USER)
  ```
- `linux/arch/riscv/include/asm/pgtable-bits.h:15`：
  ```c
  #define _PAGE_USER (1 << 4)
  ```
- 因此源码默认即置 U 位；此前"IOMMU 页表可能未置 U 位"的候选缺乏已提供源码支持，应从不确定性中移除。
- 这不代表所有门都成立；只说明 U 位这一条不再作为未决阻塞候选。

## 6. 主 FPGA 实际配置事实（沿用此前引用的行号，本次未重新读取该文件）

- `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1322-1327`：非 block 模式下 `nonblock_cfg()` 将 `cfg.MSITrans` 覆盖为 `rvi_iommu_cfg::MSI_BT_ONLY`，保留 flat MSI。
- 同文件 `:1332-1334`：若 `IOMMUNonblockMode > 2` 或 nonblock 与 legacy E3/COST observers 并用，会 `$fatal`。
- 同文件 `:1204`：nonblock 模式下 iDMA 0/1 的 `BUFFER_DEPTH` 为 512；S/N 使用相同 512 缓冲容量。
- 同文件 `:991-1002`：`dma_nonblock_observer` 已连接到 `nonblock_data`，并接收 `nonblock_diag` 作为 `diag_events`。
- 同文件 `:1390-1392`：`rv_iommu_top` 输出 `nonblock_fire_o`、`trans_accept_o`、`nonblock_diag_o`、`trans_did_o`、`trans_type_o`。

## 7. v4 已实现代码的实际状态（可核对者为限）

### 7.1 已实现（在被提供文件文本中可直接核对）

`cva6/corev_apu/fpga/src/dma_nonblock_observer.sv`（观察者侧，本次已实现）：

- `:43` — 新增输入端口 `input logic [15:0] diag_events = 16'd0`。
- `:47` — `localparam logic [63:0] DIAG_MAGIC = 64'h49444D414E444701`。
- `:71-73` — 新增 `diag_seen_q/d[15:0]`、`diag_cnt_q/d[9]`、`diag_overflow_q/d`。
- `:108-110`、`:124-126` — 组合默认保持；ARM 合法时 `diag_seen_d='0`、`diag_overflow_d=0`、`diag_cnt_d[i]='0`。
- `:162-182` — 累加段仍以**寄存器化状态**为门槛：只有 `state_q == S_ACTIVE` 才更新 `diag_seen_d` 与 `diag_cnt_d[0..8]`（事件位 3..11）。因此 **open 边沿被排除、close 边沿（state_q 仍为 S_ACTIVE）被包含、freeze 之后不再累加**。
- `:213-231` — index 16..31 的只读第二 bank 映射（16 magic、17 version、18 Mode、19 `{overflow,60'd0,frozen,active,armed}`、20 seq、21 cycles、22 seen、23..31 九个计数）。
- `:235-252` — 同步复位清零，含 `diag_seen_q`、`diag_overflow_q`、`diag_cnt_q[0..8]`。
- 原 16 字 bank 0..15（`:195-212`）**未改**，word15 依旧是 sticky error。

顶层连接：`nonblock_diag_o` → `dma_nonblock_observer.diag_events`（`ariane_peripherals_xilinx.sv:991-1002`，行号沿用此前报告引用；本次未重新读取该文件）。

### 7.2 由上级说明已实现、本报告不描述其 RTL 细节

- v4 的 **flatMSI 保守输入** 与 **cached-S1 physical address filter**：由 Codex 实现。所提供材料中**不含**该过滤器的 RTL 文本，因此本报告不描述其实现细节、不引用其行号，也不对其行为做任何断言。
- 上级提供：**TL 与 TL+4KiB 耦合 RR 回归在这套 v4 RTL 上 exit 0**（由父级 Codex 提供；**本 worker 未执行、未复核日志**，仅作为上级证据转述）。

### 7.3 尚未构建 / 尚未上板（not-built / not-on-board-yet）

- `bench/idma_nonblock_read.c` 的 `--diagnostics`、runner 的 `--diagnostics`、SV/CPP observer 测试的 diag 段：**均为本批提案，尚未应用到仓库、尚未编译、尚未仿真、尚未上板**。
- 没有新的 15 例 pilot，没有新的板端数据；本报告不含任何新实验结论。
- 维持：不复用旧数据、不做 short-only、不改 Linux 00093、无 SD 更新。

## 8. 诊断 ABI 与旧 16 字 ABI 的关系

旧 16 字 ABI 保持不变；旧 word15 仍是 sticky error，不得挪用。此前"把诊断位放进 err15 未用高位"的提案无效：旧 helper 对 `words[15] != 0` 一律拒绝。

选定的诊断 ABI 为独立第二 bank，index 16..31：

```text
INDIRECT raw NB_INDEX_BASE + 16
NB_INDEX_BASE = 512 + 0xD00 = 0xF00
extra bank base = 0xF10
```

第二 bank 精确 16 字（与 RTL `:213-231` 一一对应）：

| diag word | index | 含义 |
|---|---|---|
| 0 | 16 | magic `0x49444D414E444701` |
| 1 | 17 | version 1 |
| 2 | 18 | mode（与 core 相同） |
| 3 | 19 | flags：core status low3 + bit63 diag counter overflow error（必须 0；运行时 exact 4） |
| 4 | 20 | seq（与 core 相同） |
| 5 | 21 | cycles（与 core 相同） |
| 6 | 22 | seen 16-bit mask |
| 7 | 23 | probe_starts |
| 8 | 24 | dc_evaluations |
| 9 | 25 | dc_good |
| 10 | 26 | leaf_evaluations |
| 11 | 27 | leaf_good |
| 12 | 28 | dc_msi_rejects |
| 13 | 29 | leaf_msi_rejects |
| 14 | 30 | leaf_lookup_misses |
| 15 | 31 | fast_accepts（必须等于 core words 6..10 之和；同一窗口精确相等） |

计数采样规则：

- 采样窗口同为 `(open, close]`；open 边沿不计、close 边沿计入。
- reset / 合法 ARM 清零；close 冻结；freeze 后不再累加（见 `:162` 的 `state_q == S_ACTIVE` 门槛）。
- 跨窗口的部分 probe 合法：start/eval/leaf 计数之间不强制完整生命周期关系。
- 全部诊断为 passive，不反馈 DMA/AXI；读 index 16..31 只走只读 `rdata`。

`seen` 位映射（bit0 已修正）：

| bit | 含义 |
|---|---|
| 0 | **base S1 PTW 活动**；该活动**可能来自隐式或调试访问**，不得解释为"仅普通通路/仅显式访问"（bit0 只进 seen，不进任何计数器） |
| 1 | existing full fast window predicate |
| 2 | candidate valid in that window |
| 3 | start |
| 4 | DC eval |
| 5 | DC passes stable stage |
| 6 | LEAF eval |
| 7 | leaf eligibility |
| 8 | DC MSI reject（if DDTC hit） |
| 9 | leaf MSI reject（if IOTLB hit） |
| 10 | leaf lookup miss |
| 11 | fast accept（**该位是唯一必须与 core fast 计数镜像的位**，只在真实 fast accept 周期驱动） |
| 12 | DC lookup miss |
| 13 | DC other reject |
| 14 | leaf other reject |
| 15 | runtime enable |

约定：seen bit0 的语义由 `diag_events` 的生产者侧定义；生产者侧逐位映射不属于本观察者侧证据范围，本报告不据此推断任何候选机会覆盖率。

## 9. 限制与不可主张项

1. original→S 被混淆：同时含银行化 + 缓冲存储/容量/布局；两次各自新鲜启动、物理分配不同、固定顺序、n=3 ⇒ 不得声称显著性、机制因果、性能收益。
2. N fast 未被本次仪表观测到：15/15 `fastcounts=[0,0,0,0,0]`，`normalcounts` 为正，word15 错误位 0。该事实不等于路径损坏、不等于零命中率、不等于零 legacy 资格；快速路径事件数不是命中率，也不是全部 miss 数。
3. 不使用旧 raw HPM/miss/PTW：`record.json` 中 `victim/aggressor.iotlb_miss/s1_ptw/s2_ptw` 属旧字段，在 nonblock 结构下非有效 metric。
4. **original-03 的启动/闪存事实（替换此前"内核与 bit 哈希未变"表述）**：original-03 **未重新烧写 BIT**，它复用 original-02 的 flash 记录（runner 的 `--recover-startup-from` 路径写出 `recovered_startup: true`、`reflashed: false`，并仅把来源 `flash.json` 的 `bit_sha256` 记为 `bit_sha256_reused`，**不重新计算**也未重新哈希 BIT）；Buildroot 在 "Saving random seed" 处由**单次人工 CTRL-C** 解除阻塞；**记录到的内核 label** 为 `6.5.0-rc2-00093-gc0f5e13ea81f`（runner `collect_boot_identity()` 用 `uname -r` 做相等检查，不等则中止）。注意：所提供的材料**不构成**对 original-03 运行期间 FPGA 上实际 bit 内容的独立哈希验证，也**不构成**内核镜像哈希比对，因此本报告**不再写"内核与 bit 哈希未变"**。精确停滞子步骤同样未被断言。
5. 无 SD 更新、Linux 00093 未改：本次不需要重编 Linux；后续若不改 ABI/接口，不应提议重编。
6. 每 label 一次运行一次启动，跨 label 差异含运行间波动；不做方差分析、不剔除合法样本、不重跑。
7. 只读 live DC 是单点快照；不得声称运行时 cache DC 精确等于 RAM，不得声称已覆盖全部候选机会，不得声称唯一原因。
8. 不宣称 novelty：hit-under-miss 是已知既有工作；本轮也不提出任何 prior-work hit 主张。

## 10. 软件侧提案与验收检查（提案，未应用、未构建）

提案内容（见本批 FILE 工件）：

- `bench/idma_nonblock_read.c`：新增可选 `--diagnostics`；无该参数时输出与旧 JSON 完全一致；有该参数时额外读第二 bank index 16..31，并校验 magic/version/mode/flags/seq/cycles、读诊断期间 core seq/flags 未变、`diag[15] == sum(core words 6..10)`（溢出检查）。
- `scripts/mac_nonblock_pilot.py`：新增 `--diagnostics`；`_reader_command()` 用 `getattr(self.args,'diagnostics',False)` 决定是否追加 `--diagnostics`；新增 `validate_helper_diagnostic()`，**拒绝 bool / 非整数 / 负数 / > 2**64−1 的诊断字**，并保留"缺省即不启用"的兼容性；仅 banked label + `--reader` 允许该参数；诊断校验失败使该 case fail；`diag-window.json` 只在 helper 给出 diagnostic 对象时保存。
- `tests/nonblock_observer_top.sv`：新增顶层 `diag_events[15:0]` 端口并连到 DUT。
- `tests/nonblock_observer_test.cpp`：保留旧 16 字检查（含三个故意非法用例）不动；新增独立 diag bank 段：ARM/open 清零点、事件位 3..10 计数、**Mode2 仅两次 fast accept（mode1 不驱动 fast ⇒ core fast 与 diag[31] 均为 0）**、close 边沿计入、`diag[31] == sum(core 6..10)`、**freeze 后忽略新事件**、**下一次 ARM 清 seen/计数且 seq 自增**。
- `tests/test_nonblock_diag_validator.py`：`validate_helper_diagnostic` 的边界单测（bool/float/str/None/负数/2**64/2**64−1/字段不匹配/长度/非对象/不变量不改动输入/`getattr` 缺省兼容）。

验收检查（草案）：

- C helper：编译无警告；无参数输出与旧 schema/字段/值完全一致；`--diagnostics` 只额外增加 `diagnostic` 对象；未知参数打印 usage 且非零退出；所有错误路径恢复原 INDEX；不伪造数据。
- Python runner：`py_compile` 通过；默认 15 例计划 / 超时 / 首错即停 / 不自动重试 / 不重编 Linux 行为不变；`--diagnostics` 对 `original-serial` 被拒；`_reader_command` 仅在 flag 为真时追加；validator 对 bool/非整数/负数/>2**64−1 一律拒绝；`diag-window.json` 只在 diagnostic 对象存在时写出。
- Observer 测试：顶层 `diag_events[15:0]` 已连接；旧 16 字检查与旧非法用例不变；Mode1 下 fast 计数为 0、Mode2 下 fast 接受数为 2；freeze 后新事件不改变 seen/计数；下一次 ARM 清零 seen/计数且 seq 自增；close 边沿计入；`diag[31] == sum(core 6..10)`；不对部分 probe 窗口强加顺序断言。
- 硬件/RTL：观察者侧诊断 bank 已由 Codex 实现在 index 16..31（本批测试按此 ABI 编写）；本批**不含**任何 RTL 修改建议。

## 11. 不确定性 / 未决

- D-1（原"diag 语义待确认"）：**已删除**。观察者侧 diag 输入端口与第二 bank 已在 `dma_nonblock_observer.sv` 实现（`:43`、`:71-73`、`:162-182`、`:213-231`），本报告不再把它列为推测接口。
- D-2（原"diag bank RTL 由 Codex 完成"）：**已删除**，同上。
- D-3：已移除"U 位未置"候选；leaf/dc 其它门是否成立仍需运行时诊断确认（本轮不预判）。
- D-4：候选 overlap 频率与 benefit 尚未观察；live DC 是单点只读快照。
- D-5：不得声称运行时 cache DC 精确等于 RAM、覆盖全部候选机会或为唯一原因。
- D-6：v4 的 flatMSI 保守输入与 cached-S1 physical address filter 未在所提供材料中给出 RTL 文本，本报告不描述、不评估其实现。
- D-7：core 16 字窗口的"负值 / 超 uint64"未在 `validate_helper_window` 中拒绝（该函数只拒绝 bool/非整数）；本批**刻意不扩大改动面**，仅在诊断校验路径上做越界拒绝。
- D-8：本 worker **未执行、未编辑、未烧写、未编译、未仿真**；以上全部为提案与静态证据汇总。

## 12. 无执行声明

- 本 worker 未运行任何工具、编译、lint、仿真或板端测试；**没有任何一项"通过"是可主张的**。
- `TL 与 TL+4KiB 耦合 RR 回归 exit 0` 由上级 Codex 提供，本报告仅转述，未复核。
- 本批 6 个工件除报告中已说明的 6 处修订外，不改变协议、不改变默认 15 例计划、不改变"技术第一错即停"、不引入自动重试、不触碰 Linux/SD。
```
````

````
## FILE: bench/idma_nonblock_read.c
```c
// SPDX-License-Identifier: GPL-2.0
/*
 * idma_nonblock_read -- fixed-address reader for the passive non-block
 * observer window bank.
 *
 * Reads only /dev/mem at the iDMA0 register block (0x50000000). It selects the
 * 16-word non-block bank through the index register at 0x70 and reads the data
 * register at 0x78. The observer must already be frozen (status low bits == 4)
 * before this helper runs: it never arms, opens, closes, or configures the DMA
 * or the observer. No address or count is taken from the command line.
 *
 * One optional flag:
 *   --diagnostics   additionally read the separate passive diagnostic bank at
 *                   index 16..31 and print it as an extra "diagnostic" JSON
 *                   object.  Without the flag the output is exactly the frozen
 *                   16-word ABI (byte-for-byte the same fields and values).
 */
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define IDMA0_PHYS   0x50000000ULL
#define MAP_SIZE     0x1000ULL

#define REG_STATUS   0x60u   /* observer status (read only here) */
#define REG_INDEX    0x70u   /* observer bank index             */
#define REG_DATA     0x78u   /* observer bank data              */

#define NB_WORDS     16
#define NB_MAGIC     0x49444D414E424C01ULL
#define NB_VERSION   1ULL

/*
 * Register-block index base of the non-block window bank.
 * Task specification: index = 512 + 0xD00 + i  (== 0xF00 + i).
 */
#define NB_INDEX_BASE (512ULL + 0xD00ULL)

/* Second, passive diagnostic bank: index NB_INDEX_BASE + 16 .. + 31. */
#define DIAG_INDEX_BASE ((uint64_t)(NB_INDEX_BASE + NB_WORDS))

#define DIAG_MAGIC        0x49444D414E444701ULL
#define DIAG_VERSION      1ULL
#define DIAG_FLAGS_FROZEN 4ULL   /* low bits: frozen=1, active=0, armed=0 */
#define DIAG_FAST_WORD    15u    /* diag[15] == sum(core words 6..10)  */

#if defined(__riscv)
#define NB_BARRIER() __asm__ __volatile__("fence iorw, iorw" ::: "memory")
#else
#define NB_BARRIER() __sync_synchronize()
#endif

static volatile uint64_t *g_regs;

static inline void nb_write(uint32_t off, uint64_t val)
{
    NB_BARRIER();
    g_regs[off / 8u] = val;
    NB_BARRIER();
}

static inline uint64_t nb_read(uint32_t off)
{
    uint64_t val;

    NB_BARRIER();
    val = g_regs[off / 8u];
    NB_BARRIER();
    return val;
}

static void emit_json(const uint64_t w[NB_WORDS], int diagnostics,
                      const uint64_t d[NB_WORDS], uint64_t core_fast_sum)
{
    unsigned i;

    printf("{\"schema\":\"idma-nonblock-window-v1\",");
    printf("\"mode\":%" PRIu64 ",", w[2]);
    printf("\"windowseq\":%" PRIu64 ",", w[4]);
    printf("\"cycles\":%" PRIu64 ",", w[5]);
    printf("\"fastcounts\":[");
    for (i = 6; i <= 10; i++)
        printf("%s%" PRIu64, i == 6 ? "" : ",", w[i]);
    printf("],\"normalcounts\":[");
    for (i = 11; i <= 13; i++)
        printf("%s%" PRIu64, i == 11 ? "" : ",", w[i]);
    printf("],\"words\":[");
    for (i = 0; i < NB_WORDS; i++)
        printf("%s%" PRIu64, i == 0 ? "" : ",", w[i]);
    printf("]");
    if (diagnostics) {
        printf(",\"diagnostic\":{\"words\":[");
        for (i = 0; i < NB_WORDS; i++)
            printf("%s%" PRIu64, i == 0 ? "" : ",", d[i]);
        printf("],\"core_fast_sum\":%" PRIu64 "}", core_fast_sum);
    }
    printf("}\n");
}

static void usage(const char *prog)
{
    fprintf(stderr, "usage: %s [--diagnostics]\n", prog);
}

int main(int argc, char **argv)
{
    uint64_t words[NB_WORDS];
    uint64_t diag[NB_WORDS];
    uint64_t orig_index, status, flags, sum, tmp;
    uint64_t core_fast_sum = 0, core_seq_before = 0, core_seq_after = 0;
    uint64_t core_flags_before = 0, core_flags_after = 0;
    void *map;
    int fd, rc = 0, ovf, diagnostics = 0;
    unsigned i;
    int argi;

    for (argi = 1; argi < argc; argi++) {
        if (strcmp(argv[argi], "--diagnostics") == 0) {
            diagnostics = 1;
        } else {
            usage(argv[0]);
            return 2;
        }
    }

    memset(words, 0, sizeof(words));
    memset(diag, 0, sizeof(diag));

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        fprintf(stderr, "open /dev/mem: %s\n", strerror(errno));
        return 2;
    }

    map = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
               (off_t)IDMA0_PHYS);
    if (map == MAP_FAILED) {
        fprintf(stderr, "mmap 0x%llx: %s\n",
                (unsigned long long)IDMA0_PHYS, strerror(errno));
        close(fd);
        return 2;
    }
    g_regs = (volatile uint64_t *)map;

    orig_index = nb_read(REG_INDEX);
    status = nb_read(REG_STATUS);

    if ((status & 0x7ULL) != 0x4ULL) {
        fprintf(stderr,
                "observer not frozen (status=0x%016" PRIx64
                "): require low bits == 4\n", status);
        rc = 3;
        goto out_restore;
    }

    for (i = 0; i < NB_WORDS; i++) {
        nb_write(REG_INDEX, NB_INDEX_BASE + (uint64_t)i);
        words[i] = nb_read(REG_DATA);
    }

    flags = words[3];
    if (words[0] != NB_MAGIC) {
        fprintf(stderr, "bad magic: 0x%016" PRIx64 "\n", words[0]);
        rc = 4;
    }
    if (words[1] != NB_VERSION) {
        fprintf(stderr, "bad version: %" PRIu64 "\n", words[1]);
        rc = 4;
    }
    if (words[2] != 1 && words[2] != 2) {
        fprintf(stderr, "bad mode: %" PRIu64 "\n", words[2]);
        rc = 4;
    }
    if ((flags & 0x7ULL) != 0x4ULL) {
        fprintf(stderr,
                "flags not frozen/!active/!armed: 0x%016" PRIx64 "\n", flags);
        rc = 4;
    }
    if (words[15] != 0) {
        fprintf(stderr,
                "observer error bits set: 0x%016" PRIx64 "\n", words[15]);
        rc = 4;
    }

    sum = 0;
    ovf = 0;
    for (i = 6; i <= 13; i++) {
        if (__builtin_add_overflow(sum, words[i], &tmp)) {
            ovf = 1;
            break;
        }
        sum = tmp;
    }
    if (ovf) {
        fprintf(stderr, "counter sum overflows 64 bits\n");
        rc = 4;
    } else if (sum != words[14]) {
        fprintf(stderr,
                "conservation failed: sum(6..13)=%" PRIu64
                " != words[14]=%" PRIu64 "\n", sum, words[14]);
        rc = 4;
    }

    /*
     * Optional second bank.  Read-only, passive: reading index 16..31 must not
     * perturb the frozen core window, so core seq/flags are re-read afterwards.
     * The bank is only printed when it validated; nothing here is faked.
     */
    if (rc == 0 && diagnostics) {
        core_seq_before = words[4];
        core_flags_before = words[3];

        for (i = 0; i < NB_WORDS; i++) {
            nb_write(REG_INDEX, DIAG_INDEX_BASE + (uint64_t)i);
            diag[i] = nb_read(REG_DATA);
        }

        nb_write(REG_INDEX, NB_INDEX_BASE + 4ULL);
        core_seq_after = nb_read(REG_DATA);
        nb_write(REG_INDEX, NB_INDEX_BASE + 3ULL);
        core_flags_after = nb_read(REG_DATA);

        if (diag[0] != DIAG_MAGIC) {
            fprintf(stderr, "bad diagnostic magic: 0x%016" PRIx64 "\n",
                    diag[0]);
            rc = 4;
        }
        if (diag[1] != DIAG_VERSION) {
            fprintf(stderr, "bad diagnostic version: %" PRIu64 "\n", diag[1]);
            rc = 4;
        }
        if (diag[2] != words[2]) {
            fprintf(stderr,
                    "diagnostic mode %" PRIu64 " != core mode %" PRIu64 "\n",
                    diag[2], words[2]);
            rc = 4;
        }
        if (diag[3] != DIAG_FLAGS_FROZEN) {
            fprintf(stderr, "bad diagnostic flags: 0x%016" PRIx64 "\n",
                    diag[3]);
            rc = 4;
        }
        if (diag[4] != words[4]) {
            fprintf(stderr,
                    "diagnostic seq %" PRIu64 " != core seq %" PRIu64 "\n",
                    diag[4], words[4]);
            rc = 4;
        }
        if (diag[5] != words[5]) {
            fprintf(stderr,
                    "diagnostic cycles %" PRIu64 " != core cycles %" PRIu64
                    "\n", diag[5], words[5]);
            rc = 4;
        }
        if (core_seq_after != core_seq_before) {
            fprintf(stderr,
                    "core seq changed during diagnostic read: %" PRIu64
                    " -> %" PRIu64 "\n", core_seq_before, core_seq_after);
            rc = 4;
        }
        if (core_flags_after != core_flags_before) {
            fprintf(stderr,
                    "core flags changed during diagnostic read: 0x%016" PRIx64
                    " -> 0x%016" PRIx64 "\n",
                    core_flags_before, core_flags_after);
            rc = 4;
        }

        core_fast_sum = 0;
        ovf = 0;
        for (i = 6; i <= 10; i++) {
            if (__builtin_add_overflow(core_fast_sum, words[i], &tmp)) {
                ovf = 1;
                break;
            }
            core_fast_sum = tmp;
        }
        if (ovf) {
            fprintf(stderr, "diagnostic core fast sum overflows 64 bits\n");
            rc = 4;
        } else if (core_fast_sum != diag[DIAG_FAST_WORD]) {
            fprintf(stderr,
                    "diagnostic fast_accepts %" PRIu64
                    " != sum(core fast 6..10) %" PRIu64 "\n",
                    diag[DIAG_FAST_WORD], core_fast_sum);
            rc = 4;
        }
    }

    if (rc == 0)
        emit_json(words, diagnostics, diag, core_fast_sum);

out_restore:
    nb_write(REG_INDEX, orig_index);
    munmap(map, MAP_SIZE);
    close(fd);
    return rc;
}
```
````

````
## FILE: scripts/mac_nonblock_pilot.py — exact patches (6 hunks, line numbers match the supplied original)
```diff
diff --git a/scripts/mac_nonblock_pilot.py b/scripts/mac_nonblock_pilot.py
--- a/scripts/mac_nonblock_pilot.py
+++ b/scripts/mac_nonblock_pilot.py
@@ -129,6 +129,16 @@
 READER_MODE_FOR_LABEL = {'banked-serial': 1, 'banked-hit': 2}
 READER_FAST_WORDS = (6, 7, 8, 9, 10)   # Vread, Vwrite, Aread, Awrite, Other
 READER_NORMAL_WORDS = (11, 12, 13)     # V, A, Other
+
+# Optional second 16-word read-only diagnostic bank (index 16..31).  It is
+# read only when the helper is invoked with --diagnostics; the frozen 16-word
+# core ABI above is unchanged.
+READER_DIAG_WORDS = 16
+READER_DIAG_MAGIC = 0x49444D414E444701
+READER_DIAG_VERSION = 1
+READER_DIAG_FLAGS_FROZEN = 4
+READER_DIAG_FAST_ACCEPT_WORD = 15
+READER_DIAG_UINT64_MAX = 0xFFFFFFFFFFFFFFFF
 UPLOAD_CHUNK = 1024
 # Raw-UART heredoc upload tuning.  At 115200 8N1 one 1024-byte base64 line
 # costs about 89 ms of wire time, so each payload line is followed by a short
@@ -498,9 +508,67 @@
     if previous_seq is not None and window[4] <= previous_seq:
         problems.append('helper window[4] sequence ' + repr(window[4])
                         + ' did not increase past ' + repr(previous_seq))
     return problems
 
 
+def validate_helper_diagnostic(window: Any, diagnostic: Any,
+                               label: str) -> List[str]:
+    '''Validate the optional second 16-word diagnostic bank.
+
+    The helper validates this bank before printing it; this runner repeats the
+    structural checks so a malformed helper.json can never be recorded as a
+    completed case.  The bank is passive: its words are not HPM counters and
+    never replace the frozen 16-word core ABI.  Words must be real uint64
+    values: bool, non-integer, negative and > 2**64 - 1 words are rejected
+    before any arithmetic, so a coerced value can never look valid.
+    '''
+    problems: List[str] = []
+    if not isinstance(window, list) or len(window) != READER_WINDOW_WORDS:
+        return ['diagnostic check needs the core 16-word window']
+    if not isinstance(diagnostic, dict):
+        return ['helper diagnostic is not an object']
+    words = diagnostic.get('words')
+    if not isinstance(words, list) or len(words) != READER_DIAG_WORDS:
+        return ['helper diagnostic.words is not '
+                + str(READER_DIAG_WORDS) + ' words']
+    for value in words:
+        if isinstance(value, bool) or not isinstance(value, int):
+            return ['helper diagnostic.words contains non-integer words']
+        if value < 0 or value > READER_DIAG_UINT64_MAX:
+            return ['helper diagnostic.words contains a word outside uint64: '
+                    + repr(value)]
+    if words[0] != READER_DIAG_MAGIC:
+        problems.append('helper diagnostic[0] magic ' + repr(words[0]))
+    if words[1] != READER_DIAG_VERSION:
+        problems.append('helper diagnostic[1] version ' + repr(words[1]))
+    expected_mode = READER_MODE_FOR_LABEL.get(label)
+    if words[2] != expected_mode:
+        problems.append('helper diagnostic[2] mode ' + repr(words[2])
+                        + ' != ' + repr(expected_mode) + ' for ' + label)
+    if words[3] != READER_DIAG_FLAGS_FROZEN:
+        problems.append('helper diagnostic[3] flags ' + repr(words[3])
+                        + ' != ' + str(READER_DIAG_FLAGS_FROZEN))
+    if words[4] != window[4]:
+        problems.append('helper diagnostic[4] seq ' + repr(words[4])
+                        + ' != core window[4] ' + repr(window[4]))
+    if words[5] != window[5]:
+        problems.append('helper diagnostic[5] cycles ' + repr(words[5])
+                        + ' != core window[5] ' + repr(window[5]))
+    total = 0
+    for index in READER_FAST_WORDS:
+        value = window[index]
+        if isinstance(value, bool) or not isinstance(value, int):
+            return ['diagnostic check needs integer core fast words']
+        total += value
+    if total > READER_DIAG_UINT64_MAX:
+        problems.append('helper diagnostic core fast sum overflows 64 bits')
+    elif words[READER_DIAG_FAST_ACCEPT_WORD] != total:
+        problems.append('helper diagnostic[15] fast_accepts '
+                        + repr(words[READER_DIAG_FAST_ACCEPT_WORD])
+                        + ' != sum(core fast 6..10) ' + repr(total))
+    return problems
+
+
 # --------------------------------------------------------------------------
 # capability gate (--caps)
 # --------------------------------------------------------------------------
@@ -1086,9 +1154,11 @@
     def _reader_target(self) -> str:
         return self.board_root + '/nonblock-reader.elf'
 
     def _reader_command(self) -> str:
         # Assumed helper invocation; align with the ELF once it is compiled.
-        mode = READER_MODE_FOR_LABEL.get(self.args.label)
-        return shlex.quote(self._reader_target())
+        command = shlex.quote(self._reader_target())
+        if getattr(self.args, 'diagnostics', False):
+            command += ' --diagnostics'
+        return command
 
     def _upload_reader(self) -> Dict[str, Any]:
@@ -1344,10 +1414,26 @@
             self._write_json('cases/' + cid + '/helper-window.json', {
                 'schema': 'nonblock-pilot-helper-window-v1',
                 'id': cid,
                 'label': self.args.label,
                 'abi': READER_ABI,
                 'mode': READER_MODE_FOR_LABEL.get(self.args.label),
                 'window': window,
                 'note': 'full frozen 16-word window saved verbatim; not HPM',
             })
+            if getattr(self.args, 'diagnostics', False):
+                diagnostic = (helper_doc.get('diagnostic')
+                              if isinstance(helper_doc, dict) else None)
+                problems.extend(validate_helper_diagnostic(
+                    window, diagnostic, self.args.label))
+                if isinstance(diagnostic, dict):
+                    self._write_json('cases/' + cid + '/diag-window.json', {
+                        'schema': 'nonblock-pilot-diag-window-v1',
+                        'id': cid,
+                        'label': self.args.label,
+                        'abi': READER_ABI,
+                        'mode': READER_MODE_FOR_LABEL.get(self.args.label),
+                        'diagnostic': diagnostic,
+                        'note': ('second 16-word diagnostic bank saved '
+                                 'verbatim; passive only, not HPM'),
+                    })
         return problems, window
@@ -1664,5 +1750,9 @@
     parser.add_argument('--reader', default=None,
                         help='static RISC-V ELF helper; required for '
                              'banked-serial/banked-hit, forbidden for original-serial')
+    parser.add_argument('--diagnostics', action='store_true',
+                        help='read the optional second 16-word diagnostic bank '
+                             'through the uploaded reader; requires a banked '
+                             'label and --reader')
     parser.add_argument('--upload-timeout', type=float, default=DEFAULT_UPLOAD_TIMEOUT)
     parser.add_argument('--pre-flash-boot-id', action='store_true',
@@ -1697,6 +1787,12 @@
                              + ' (no RO hardware on that bitstream)')
         if not pathlib.Path(args.reader).is_file():
             raise SystemExit('--reader not found: ' + args.reader)
+    if getattr(args, 'diagnostics', False):
+        if args.label not in READER_REQUIRED_LABELS:
+            raise SystemExit('--diagnostics requires a banked label with RO '
+                             'hardware: ' + ', '.join(READER_REQUIRED_LABELS))
+        if not args.reader:
+            raise SystemExit('--diagnostics requires --reader')
     if args.recover_startup_from:
         if args.interrupt_stalled_seed:
             raise SystemExit('--interrupt-stalled-seed does not apply with '
```

注释（供 Codex 复核，非 diff 内容）：

- hunk 1 `@@ -129,6 +129,16 @@`、hunk 2 `@@ -498,9 +508,67 @@`、hunk 3 `@@ -1086,9 +1154,11 @@`、hunk 4 `@@ -1344,10 +1414,26 @@`、hunk 5 `@@ -1664,5 +1750,9 @@`、hunk 6 `@@ -1697,6 +1787,12 @@`（右侧行号按前序 hunk 累积平移；左侧行号严格取自原文件）。
- hunk 3 顺带删除了 `_reader_command()` 内那行未被使用的 `mode = READER_MODE_FOR_LABEL.get(self.args.label)`（仅死代码，无行为变化），这是该函数内唯一的结构性清理。
- hunk 4 依赖同函数内已存在的 `helper_doc`（原文件 `:1306`），且位于 `if window is None: ... else:` 的 `else` 分支内（`window` 此时非 None）。
- 未改动：`build_case_plan()` 的 15 例计划、`LONG_ROUND_ORDERS`、`validate_record`、`validate_helper_window`、`_upload_reader`、首错即停与不自动重试逻辑、`EXPECTED_UNAME_R`、`--recover-startup-from` / `--interrupt-stalled-seed` 语义、Linux/SD 相关项。
````

````
## FILE: tests/nonblock_observer_top.sv
```systemverilog
// SPDX-License-Identifier: Solderpad-0.51
//
// Structural top for the non-block observer. Mode 2 by default; +define+NB_MODE1
// selects the Mode 1 variant. No SV timing constructs.
//
`ifdef NB_MODE1
`define NB_MODE_VAL 1
`else
`define NB_MODE_VAL 2
`endif

module nonblock_observer_top (
    input  logic        clk,
    input  logic        rst,
    input  logic        arm,
    input  logic        open,
    input  logic        close,
    input  logic [7:0]  index,
    output logic [63:0] rdata,
    input  logic        accept,
    input  logic        fast,
    input  logic [23:0] did,
    input  logic [5:0]  ttype,
    input  logic [15:0] diag_events
);
    dma_nonblock_observer #(.Mode(`NB_MODE_VAL)) dut (
        .clk    (clk),
        .rst    (rst),
        .arm    (arm),
        .open   (open),
        .close  (close),
        .index  (index),
        .rdata  (rdata),
        .accept (accept),
        .fast   (fast),
        .did    (did),
        .ttype  (ttype),
        .diag_events(diag_events)
    );
endmodule
```
````

````
## FILE: tests/nonblock_observer_test.cpp
```cpp
// SPDX-License-Identifier: Solderpad-0.51
//
// Verilator 4.038 harness for dma_nonblock_observer. Simple manual clock, no SV
// timing. Mode-agnostic: reads word 2 to learn the configured Mode.
//
#include <cstdint>
#include <cstdio>
#include "Vnonblock_observer_top.h"
#include "verilated.h"

static Vnonblock_observer_top *dut;
static int g_fail = 0;

static void tick() {
    dut->clk = 0; dut->eval();   // settle inputs before posedge
    dut->clk = 1; dut->eval();   // posedge captures inputs
    dut->clk = 0; dut->eval();
}

static void idle() {
    dut->arm = 0; dut->open = 0; dut->close = 0;
    dut->accept = 0; dut->fast = 0;
    dut->did = 0; dut->ttype = 0; dut->diag_events = 0;
}

static uint64_t rd(int idx) {
    dut->index = (uint8_t)idx;
    dut->eval();
    return (uint64_t)dut->rdata;
}

static void check(const char *what, uint64_t got, uint64_t want) {
    if (got != want) {
        std::fprintf(stderr,
                     "FAIL %s: got %llu (0x%llx) want %llu (0x%llx)\n",
                     what,
                     (unsigned long long)got, (unsigned long long)got,
                     (unsigned long long)want, (unsigned long long)want);
        g_fail++;
    }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vnonblock_observer_top;

    idle();
    dut->rst = 0; dut->index = 0; dut->clk = 0; dut->eval();

    // ---- reset ----
    dut->rst = 1; idle(); tick();
    dut->rst = 0; idle(); dut->eval();
    check("magic",       rd(0),  0x49444D414E424C01ULL);
    check("version",     rd(1),  1);
    uint64_t mode = rd(2);
    if (mode != 1 && mode != 2) {
        std::fprintf(stderr, "FAIL mode=%llu\n", (unsigned long long)mode);
        g_fail++;
    }
    check("rst flags",  rd(3),  0);
    check("rst seq",    rd(4),  0);
    check("rst cycles", rd(5),  0);
    check("rst err",    rd(15), 0);

    // ---- ARM ----
    idle(); dut->arm = 1; tick();
    idle(); dut->eval();
    check("armed", rd(3), 0x1);
    check("seq1",  rd(4), 1);

    // ---- pre-open accepted event must be excluded (still ARMED) ----
    idle(); dut->accept = 1; dut->fast = 0; dut->did = 10; dut->ttype = 0x2; tick();
    idle(); dut->eval();
    check("pre-open excluded", rd(14), 0);

    // ---- open edge with a coincident accept: excluded, opens the window ----
    idle();
    dut->open = 1; dut->accept = 1; dut->fast = 0; dut->did = 10; dut->ttype = 0x2;
    tick();
    idle(); dut->eval();
    check("active",            rd(3),  0x2);
    check("open-edge excl 14", rd(14), 0);
    check("open-edge excl 11", rd(11), 0);

    // ---- active window ----
    // A: fast DID10 write
    idle(); dut->accept = 1; dut->fast = 1; dut->did = 10; dut->ttype = 0x3; tick();
    // B: normal DID11
    idle(); dut->accept = 1; dut->fast = 0; dut->did = 11; dut->ttype = 0x3; tick();
    // C: fast other (DID 12)
    idle(); dut->accept = 1; dut->fast = 1; dut->did = 12; dut->ttype = 0x2; tick();
    // D: idle cycle (cycles++ only)
    idle(); tick();
    // E: close edge with fast DID11 read, included
    idle();
    dut->accept = 1; dut->fast = 1; dut->did = 11; dut->ttype = 0x2; dut->close = 1;
    tick();
    idle(); dut->eval();

    check("frozen", rd(3),  0x4);
    check("w6",     rd(6),  0);
    check("w7",     rd(7),  1);
    check("w8",     rd(8),  1);
    check("w9",     rd(9),  0);
    check("w10",    rd(10), 1);
    check("w11",    rd(11), 0);
    check("w12",    rd(12), 1);
    check("w13",    rd(13), 0);
    check("w14",    rd(14), 4);
    check("cycles", rd(5),  5);

    // ---- conservation, overflow-safe ----
    uint64_t sum = 0, tmp = 0;
    int ovf = 0;
    for (int i = 6; i <= 13; i++) {
        if (__builtin_add_overflow(sum, rd(i), &tmp)) { ovf = 1; break; }
        sum = tmp;
    }
    check("conserve overflow", ovf, 0);
    check("conserve sum",      sum, rd(14));

    // ---- frozen read stability ----
    uint64_t a[16], b[16];
    for (int i = 0; i < 16; i++) a[i] = rd(i);
    for (int i = 0; i < 16; i++) b[i] = rd(i);
    for (int i = 0; i < 16; i++) check("stable", a[i], b[i]);

    // ---- ignored unknown index reads 0 ----
    check("unknown idx -> 0", rd(200), 0);

    // ---- new ARM resets seq++ and clears the window ----
    idle(); dut->arm = 1; tick();
    idle(); dut->eval();
    check("seq2",           rd(4),  2);
    check("armed2",         rd(3),  0x1);
    check("cycles cleared", rd(5),  0);
    check("errors cleared", rd(15), 0);

    // ---- invalid close (not active) -> bit1 ----
    idle(); dut->close = 1; tick();
    idle(); dut->eval();
    check("close-no-active", (rd(15) >> 1) & 1, 1);

    // ---- invalid fast without accept -> bit4 (+ Mode1 bit5) ----
    idle(); dut->fast = 1; dut->accept = 0; dut->did = 10; dut->ttype = 0x2; tick();
    idle(); dut->eval();
    check("fast-no-accept", (rd(15) >> 4) & 1, 1);
    if (mode == 1)
        check("mode1 fast err", (rd(15) >> 5) & 1, 1);
    else
        check("mode2 no mode1", (rd(15) >> 5) & 1, 0);

    // ---- focused reset + ARM/OPEN overlap contract ----
    dut->rst = 1; idle(); tick();
    dut->rst = 0; idle(); dut->eval();
    check("overlap-reset flags",  rd(3),  0);
    check("overlap-reset seq",    rd(4),  0);
    check("overlap-reset cycles", rd(5),  0);
    check("overlap-reset err",    rd(15), 0);

    // ARM and OPEN asserted together is invalid: overlap error, no window.
    idle(); dut->arm = 1; dut->open = 1; tick();
    idle(); dut->eval();
    check("arm+open overlap err", (rd(15) >> 2) & 1, 1);
    check("arm+open not active",  (rd(3) >> 1) & 1, 0);
    check("arm+open not armed",   (rd(3) >> 0) & 1, 0);
    check("arm+open seq held",    rd(4), 0);

    // A clean ARM after the overlap is accepted and clears the error.
    idle(); dut->arm = 1; tick();
    idle(); dut->eval();
    check("clean-arm err clear", (rd(15) >> 2) & 1, 0);
    check("clean-arm armed",     rd(3), 0x1);
    check("clean-arm seq",       rd(4), 1);

    // The open edge remains excluded and the close edge remains counted.
    idle(); dut->open = 1; tick();
    idle(); dut->eval();
    check("close-edge open active", (rd(3) >> 1) & 1, 1);
    check("close-edge open cycles", rd(5), 0);

    idle(); dut->close = 1; tick();
    idle(); dut->eval();
    check("close-edge frozen", (rd(3) >> 2) & 1, 1);
    check("close-edge cycles", rd(5), 1);

    // ---- optional second diagnostic bank (index 16..31) --------------------
    // Fresh reset/ARM/open/close for one bounded diagnostic window.  The core
    // 16-word checks above are untouched; this section reads only the separate
    // passive bank.  No ordering relation between the probe counters is
    // asserted: a probe can legitimately cross the window boundary.
    //
    // Mode handling: only Mode 2 drives the core fast path here.  In Mode 1
    // fast is a protocol error, so a *valid* diagnostic window must not drive
    // fast at all; the core fast counters then stay 0 and so does the mirrored
    // diagnostic event bit 11 (diag word 31).  Both modes spend exactly the
    // same number of active cycles, so the cycle checks stay mode-agnostic.
    dut->rst = 1; idle(); tick();
    dut->rst = 0; idle(); dut->eval();

    idle(); dut->arm = 1; tick();
    idle(); dut->eval();
    check("diag magic",      rd(16), 0x49444D414E444701ULL);
    check("diag version",    rd(17), 1);
    check("diag mode",       rd(18), mode);
    check("diag arm flags",  rd(19), 0x1);
    check("diag arm seq",    rd(20), 1);
    check("diag arm cycles", rd(21), 0);
    check("diag arm seen",   rd(22), 0);
    for (int i = 23; i <= 31; i++)
        check("diag arm counter", rd(i), 0);

    idle(); dut->open = 1; tick();
    idle(); dut->eval();
    check("diag open flags",  rd(19), 0x2);
    check("diag open cycles", rd(21), 0);
    check("diag open seen",   rd(22), 0);
    for (int i = 23; i <= 31; i++)
        check("diag open counter", rd(i), 0);

    // Probe-stage seen bits 3..10 and their counters.  Bit 11 is deliberately
    // not set here: it mirrors the core fast path and is driven only in Mode 2.
    idle(); dut->diag_events = (1u << 3) | (1u << 4) | (1u << 5); tick();
    idle(); dut->diag_events = (1u << 6) | (1u << 7); tick();
    idle(); dut->diag_events = (1u << 8) | (1u << 9) | (1u << 10); tick();

    // One real fast accept: the core fast event and its diagnostic mirror are
    // driven together.  Mode 1 must not drive fast; the cycle is spent either
    // way so both modes keep the same cycle count.
    idle();
    if (mode == 2) {
        dut->accept = 1; dut->fast = 1; dut->did = 10; dut->ttype = 0x2;
        dut->diag_events = (1u << 11);
    }
    tick();

    // Remaining seen-only bits: no core fast count and no probe counter.
    idle();
    dut->diag_events = (1u << 0) | (1u << 1) | (1u << 2) |
                       (1u << 12) | (1u << 13) | (1u << 14) | (1u << 15);
    tick();

    // Closing edge: state is still ACTIVE at the edge, so both the core fast
    // event and its diagnostic mirror are counted (same rule as the legacy
    // section above).
    idle();
    if (mode == 2) {
        dut->accept = 1; dut->fast = 1; dut->did = 11; dut->ttype = 0x2;
        dut->diag_events = (1u << 11);
    }
    dut->close = 1;
    tick();
    idle(); dut->eval();

    check("diag frozen flags",     rd(19), 0x4);
    check("diag window core err",  rd(15), 0);
    check("diag seen mask",        rd(22), (mode == 2) ? 0xFFFFULL : 0xF7FFULL);
    check("diag start",            rd(23), 1);
    check("diag dc eval",          rd(24), 1);
    check("diag dc good",          rd(25), 1);
    check("diag leaf eval",        rd(26), 1);
    check("diag leaf good",        rd(27), 1);
    check("diag dc msi rej",       rd(28), 1);
    check("diag leaf msi rej",     rd(29), 1);
    check("diag leaf miss",        rd(30), 1);
    check("diag fast accepts",     rd(31), (mode == 2) ? 2 : 0);
    check("diag cycles",           rd(21), 6);
    check("diag seq same",         rd(20), rd(4));
    check("diag cycles same",      rd(21), rd(5));

    // Diagnostic word 31 must equal the core fast sum in the same window.
    uint64_t core_fast_sum = 0, core_fast_tmp = 0;
    int core_fast_ovf = 0;
    for (int i = 6; i <= 10; i++) {
        if (__builtin_add_overflow(core_fast_sum, rd(i), &core_fast_tmp)) {
            core_fast_ovf = 1;
            break;
        }
        core_fast_sum = core_fast_tmp;
    }
    check("diag core fast sum overflow", core_fast_ovf, 0);
    check("diag fast accepts == core fast sum", rd(31), core_fast_sum);
    check("diag overflow bit", (rd(19) >> 63) & 1, 0);

    // ---- a frozen window ignores further diagnostic events ----
    uint64_t seen_frozen = rd(22);
    uint64_t frozen_counters[9];
    for (int i = 0; i < 9; i++) frozen_counters[i] = rd(23 + i);
    idle(); dut->diag_events = 0xFFFF; tick();
    idle(); dut->eval();
    check("diag frozen ignores seen", rd(22), seen_frozen);
    for (int i = 0; i < 9; i++)
        check("diag frozen ignores counter", rd(23 + i), frozen_counters[i]);
    check("diag frozen flags stay", rd(19), 0x4);

    // ---- the next ARM clears seen/counters and increments seq ----
    uint64_t seq_before = rd(20);
    idle(); dut->arm = 1; tick();
    idle(); dut->eval();
    check("diag rearm seq",         rd(20), seq_before + 1);
    check("diag rearm seq == core", rd(20), rd(4));
    check("diag rearm flags",       rd(19), 0x1);
    check("diag rearm cycles",      rd(21), 0);
    check("diag rearm seen",        rd(22), 0);
    for (int i = 23; i <= 31; i++)
        check("diag rearm counter", rd(i), 0);

    delete dut;
    if (g_fail) {
        std::fprintf(stderr, "%d check(s) failed\n", g_fail);
        return 1;
    }
    std::printf("PASS (mode=%llu)\n", (unsigned long long)mode);
    return 0;
}
```
````

````
## FILE: tests/test_nonblock_diag_validator.py
```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
Unit tests for validate_helper_diagnostic() in scripts/mac_nonblock_pilot.py.

Pure validator tests plus two tiny optional-flag compatibility tests.  Nothing
here touches hardware, serial ports, /dev/mem, subprocesses or the filesystem:
PilotRunner is only constructed (its __init__ performs no I/O) to call
_reader_command().

Run with:
    python3 -m unittest discover -s tests -p 'test_nonblock_diag_validator.py'
    python3 tests/test_nonblock_diag_validator.py

The runner is loaded by file path so scripts/ is never added to sys.path (the
runner itself avoids that for the same reason).  If the runner patch has not
been applied, the import-time lookup or the flag tests fail loudly instead of
silently skipping.
'''
from __future__ import annotations

import argparse
import importlib.util
import pathlib
import sys
import unittest
from typing import Any, Dict, List, Optional

HERE = pathlib.Path(__file__).resolve().parent

CANDIDATE_RUNNERS = (
    HERE.parent / 'scripts' / 'mac_nonblock_pilot.py',
    HERE.parent / 'mac_nonblock_pilot.py',
    HERE / 'mac_nonblock_pilot.py',
)


def load_runner(path_hint: Optional[str] = None):
    candidates = ([pathlib.Path(path_hint)] if path_hint else [])
    candidates += list(CANDIDATE_RUNNERS)
    for path in candidates:
        if not path.is_file():
            continue
        spec = importlib.util.spec_from_file_location(
            'nonblock_runner_under_test', path)
        if spec is None or spec.loader is None:
            continue
        module = importlib.util.module_from_spec(spec)
        sys.modules[spec.name] = module
        spec.loader.exec_module(module)
        return module, path
    raise RuntimeError(
        'mac_nonblock_pilot.py not found; tried: '
        + ', '.join(str(p) for p in candidates))


RUNNER, RUNNER_PATH = load_runner()

FAST_WORDS_MODE1 = (0, 0, 0, 0, 0)
FAST_WORDS_MODE2 = (0, 1, 1, 0, 0)


def make_window(mode: int = 2) -> List[int]:
    '''A frozen, self-consistent 16-word core window (idma-nonblock-window-v1).'''
    fast = FAST_WORDS_MODE2 if mode == 2 else FAST_WORDS_MODE1
    window = [0] * 16
    window[0] = RUNNER.READER_MAGIC
    window[1] = RUNNER.READER_VERSION
    window[2] = mode
    window[3] = RUNNER.READER_FLAGS_FROZEN
    window[4] = 7                       # windowseq
    window[5] = 6                       # cycles
    for offset, value in enumerate(fast):
        window[6 + offset] = value
    window[11], window[12], window[13] = 3, 1, 0
    window[14] = sum(fast) + 4
    window[15] = RUNNER.READER_ERRORS_OK
    return window


def make_diag(mode: int = 2) -> List[int]:
    '''A structurally valid second bank, restricted to what the validator sees.'''
    fast = FAST_WORDS_MODE2 if mode == 2 else FAST_WORDS_MODE1
    diag = [0] * 16
    diag[0] = RUNNER.READER_DIAG_MAGIC
    diag[1] = RUNNER.READER_DIAG_VERSION
    diag[2] = mode
    diag[3] = RUNNER.READER_DIAG_FLAGS_FROZEN
    diag[4] = 7                         # seq, must equal window[4]
    diag[5] = 6                         # cycles, must equal window[5]
    diag[6] = 0xF7FF if mode == 1 else 0xFFFF   # seen mask (bit 11: Mode 2 only)
    for index in range(7, 15):
        diag[index] = 1
    diag[RUNNER.READER_DIAG_FAST_ACCEPT_WORD] = sum(fast)
    return diag


def document(mode: int = 2) -> Dict[str, Any]:
    return {'words': make_diag(mode)}


class ValidateHelperDiagnosticTests(unittest.TestCase):

    def check(self, window: Any, diagnostic: Any,
              label: str = 'banked-hit') -> List[str]:
        return RUNNER.validate_helper_diagnostic(window, diagnostic, label)

    # ---- accepted shapes -------------------------------------------------

    def test_mode2_valid(self):
        self.assertEqual(self.check(make_window(2), document(2)), [])

    def test_mode1_valid(self):
        self.assertEqual(
            self.check(make_window(1), document(1), 'banked-serial'), [])

    def test_uint64_max_is_in_range(self):
        diag = document(2)
        diag['words'][0] = RUNNER.READER_DIAG_UINT64_MAX   # in range, wrong magic
        problems = self.check(make_window(2), diag)
        self.assertTrue(problems)
        self.assertIn('diagnostic[0] magic', problems[0])
        for problem in problems:
            self.assertNotIn('outside uint64', problem)

    # ---- shape rejections ------------------------------------------------

    def test_missing_window(self):
        expected = ['diagnostic check needs the core 16-word window']
        self.assertEqual(self.check(None, document(2)), expected)
        self.assertEqual(self.check([0] * 15, document(2)), expected)
        self.assertEqual(self.check('x', document(2)), expected)

    def test_diagnostic_not_an_object(self):
        expected = ['helper diagnostic is not an object']
        for bad in (None, [], 'x', 3, True):
            self.assertEqual(self.check(make_window(2), bad), expected)

    def test_words_not_sixteen(self):
        expected = ['helper diagnostic.words is not 16 words']
        for bad in ([0] * 15, [0] * 17, [], None, 'x', 16):
            self.assertEqual(
                self.check(make_window(2), {'words': bad}), expected)

    def test_bool_word_rejected(self):
        for index in (0, 3, 15):
            diag = document(2)
            diag['words'][index] = True
            self.assertEqual(
                self.check(make_window(2), diag),
                ['helper diagnostic.words contains non-integer words'])

    def test_non_integer_word_rejected(self):
        for value in (1.0, 1.5, -0.5, '1', None, [1], {'a': 1}):
            diag = document(2)
            diag['words'][3] = value
            self.assertEqual(
                self.check(make_window(2), diag),
                ['helper diagnostic.words contains non-integer words'],
                repr(value))

    def test_negative_word_rejected(self):
        diag = document(2)
        diag['words'][5] = -1
        problems = self.check(make_window(2), diag)
        self.assertEqual(len(problems), 1)
        self.assertIn('outside uint64', problems[0])
        self.assertIn('-1', problems[0])

    def test_above_uint64_rejected(self):
        diag = document(2)
        diag['words'][6] = 1 << 64
        problems = self.check(make_window(2), diag)
        self.assertEqual(len(problems), 1)
        self.assertIn('outside uint64', problems[0])

    def test_core_fast_word_not_integer(self):
        window = make_window(2)
        window[7] = '1'
        self.assertEqual(
            self.check(window, document(2)),
            ['diagnostic check needs integer core fast words'])

    # ---- field mismatches ------------------------------------------------

    def test_field_mismatches(self):
        cases = (
            (0, 0, 'diagnostic[0] magic'),
            (1, 2, 'diagnostic[1] version'),
            (3, 0, 'diagnostic[3] flags'),
            (4, 99, 'diagnostic[4] seq'),
            (5, 99, 'diagnostic[5] cycles'),
            (15, 99, 'diagnostic[15] fast_accepts'),
        )
        for index, value, needle in cases:
            diag = document(2)
            diag['words'][index] = value
            problems = self.check(make_window(2), diag)
            self.assertTrue(problems, (index, value))
            self.assertIn(needle, problems[0], (index, value))

    def test_mode_mismatch_for_label(self):
        problems = self.check(make_window(2), document(2), 'banked-serial')
        self.assertTrue(problems)
        self.assertIn('diagnostic[2] mode', problems[0])

    def test_fast_accept_must_equal_core_fast_sum(self):
        problems = self.check(make_window(2), document(1))
        self.assertTrue(
            any('diagnostic[15] fast_accepts' in p for p in problems),
            problems)

    def test_core_fast_sum_overflow_is_reported(self):
        window = make_window(2)
        window[6] = RUNNER.READER_DIAG_UINT64_MAX
        window[7] = 1
        problems = self.check(window, document(2))
        self.assertTrue(
            any('core fast sum overflows 64 bits' in p for p in problems),
            problems)

    # ---- the validator must not touch its inputs -------------------------

    def test_inputs_are_not_mutated(self):
        window = make_window(2)
        diag = document(2)
        window_before = list(window)
        diag_before = {'words': list(diag['words'])}
        self.check(window, diag)
        self.assertEqual(window, window_before)
        self.assertEqual(diag, diag_before)


class DiagnosticsFlagCompatibilityTests(unittest.TestCase):
    '''The optional flag must default to "not enabled" and must be read with
    getattr so an args object without the attribute keeps working.'''

    def parse(self, extra: Optional[List[str]] = None):
        argv = ['--label', 'banked-hit', '--bit', 'b.bit', '--out', 'o',
                '--reader', 'r.elf'] + list(extra or [])
        return RUNNER.build_arg_parser().parse_args(argv)

    def test_flag_defaults_to_false(self):
        args = self.parse()
        self.assertIn('diagnostics', vars(args))
        self.assertFalse(getattr(args, 'diagnostics', False))

    def test_flag_sets_true(self):
        args = self.parse(['--diagnostics'])
        self.assertTrue(getattr(args, 'diagnostics', False))

    def test_unknown_flag_is_rejected(self):
        with self.assertRaises(SystemExit):
            self.parse(['--diagnostics-typo'])

    def test_reader_command_ignores_a_missing_attribute(self):
        args = self.parse(['--diagnostics'])
        del args.diagnostics            # older / partial Namespace
        pilot = RUNNER.PilotRunner(args, console=None,
                                   out_dir=pathlib.Path('.'))
        self.assertNotIn('--diagnostics', pilot._reader_command())

    def test_reader_command_appends_the_flag_when_true(self):
        args = self.parse(['--diagnostics'])
        pilot = RUNNER.PilotRunner(args, console=None,
                                   out_dir=pathlib.Path('.'))
        self.assertTrue(
            pilot._reader_command().endswith(' --diagnostics'))


if __name__ == '__main__':
    unittest.main(verbosity=2)
```
````

## 小结（findings / acceptance / no execution）

**关键修正（报告 4 处 + ABI 1 处 + 状态 1 处）**

1. **静态不相容点归因纠正**：正确的不相容是 live `msiptp.mode=1`（flat MSI 打开，见 `v4-live-context-evidence.json` 的 `msi_modes {10:1, 11:1}`）与 v3 守卫 `msi_mode==0` 之间；DDTP mode 2（`ddtp=0x20AC4002`、root `0x82b10000`、stride 64 B）在 v3 与 v4 均受支持，**不是**不相容点。报告同时写入 no-sole-cause / no-opportunity-coverage / no-cache-equality 三条禁止主张。
2. **删除"内核与 bit 哈希未变"**：改为描述 original-03 复用 original-02 flash 记录（`reflashed:false`、`bit_sha256_reused` 仅转抄）、单次人工 CTRL-C 解除 "Saving random seed" 阻塞、以及记录到的内核 label `6.5.0-rc2-00093-gc0f5e13ea81f`；明确该复用不构成 bit/内核哈希的独立验证。
3. **prior-work 措辞替换**：hit-under-miss 为已知既有方法 ⇒ 不宣称 novelty，不主张 prior-work hit（原三处模糊表述已替换）。
4. **v4 状态改为"已实现"而非未来承诺**：逐行引用 `dma_nonblock_observer.sv:43/47/71-73/108-110/124-126/162-182/213-231/235-252` 证明诊断 bank 已落地；flatMSI 保守输入与 cached-S1 physical filter 明确标注"由 Codex 实现、材料未含 RTL 文本、本报告不描述"；软件工件明确 **not-built / not-on-board-yet**；TL 与 TL+4KiB 耦合 RR exit0 明确标注为**上级提供、本 worker 未执行**。
5. **seen bit0 修正**：base S1 PTW 活动，可能来自隐式/调试访问，不是"仅普通通路"。
6. **删除过时 D-1/D-2**，并新增 D-7（core 窗口越界未拒绝，刻意不改）、D-8（无执行）。

**新增测试的边界覆盖（`tests/test_nonblock_diag_validator.py`）**：bool / float(1.0,1.5,−0.5) / str / None / list / dict → 非整数拒绝；`-1` 与 `1<<64` → 越界拒绝；`2**64-1` → 在范围内（仅报 magic 错）；15/17/空/None/str 长度的 words → 长度拒绝；窗口缺/非 16 字 → 前置拒绝；magic/version/flags/seq/cycles/fast_accepts 逐个不匹配；label↔mode 不匹配；core fast 溢出；输入不被改写；`--diagnostics` 默认 False、未知 flag 被 argparse 拒绝、`del args.diagnostics` 后 `_reader_command()` 仍工作（验证 `getattr(...,False)` 兼容）。

**Observer 测试的行为覆盖**：ARM/open 清零点与 seen=0；事件位 3..10 各自计数 1；**Mode2 两次 fast accept（中段 + close 边沿），Mode1 不驱动 fast ⇒ core fast 与 diag[31] 均为 0**；`diag[31] == sum(core 6..10)`；`diag[21]==6` 与 `diag[21]==rd(5)`（close 边沿计入、open 边沿排除）；freeze 后灌入 `0xFFFF` 事件后 seen 与 9 个计数不变；下一次 ARM 后 seen=0、9 计数=0、`rd(20)==rd(4)==seq_before+1`。旧 16 字检查与三个故意非法用例逐字保留。

**假设与未决（供 Codex 复核）**

- diff 的 hunk 头按前序 hunk 累积平移计算（hunk1→hunk2/+10、hunk2→hunk3/+58、hunk3→hunk4/+2、hunk4→hunk5/+16、hunk5→hunk6/+6）；若 Codex 的分支与所提供原文有偏差，请以左侧行号为准重定位。
- hunk 3 删除的是 `_reader_command()` 内的死代码赋值（`mode = ...`），不改变行为；如不希望删除，可只保留 `+` 行。
- C helper 只重读 core 的 seq/flags 做"诊断读取未扰动"证明；cycles 的稳定性由 `diag[5] != words[5]` 覆盖（两 bank 同源）。如需要，可再加 core cycles 重读。
- `validate_helper_window` 仍未对 core 16 字做负值/超 uint64 拒绝（只拒 bool/非整数），本批刻意不动；因此 `validate_helper_diagnostic` 内对 core fast 字加了"非整数即返回问题"的守卫以防 TypeError。
- `diag-window.json` 会把 helper 的 `diagnostic` 对象（含 `core_fast_sum`）整体原样保存；validator 未交叉校验 `diagnostic['core_fast_sum']` 字段（与 `words[15]` 冗余），如需可另开一批添加。
- RTL 侧 `diag_events` 生产者（`nonblock_diag_o` 逐位来源）不在本次证据范围内；测试仅按已实现的 observer 侧 ABI（index 16..31）驱动 `diag_events`。

**无执行声明**：本 worker 未编译、未 lint、未仿真、未上板、未运行任何测试、未改动任何文件；上述所有数值、行号与"通过"均为静态阅读或上级转述，需由 Codex 实际执行后确认。