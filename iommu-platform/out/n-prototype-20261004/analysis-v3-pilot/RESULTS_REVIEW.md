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
