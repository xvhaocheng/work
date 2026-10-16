## 2026-10-02 追加：被动 observer v8 的 E2/自然占用同一包协议与 84 次冻结计划

本节仅追加到本文，不改动其他文件。当前仍为设计/实现中规格：RTL、驱动、runner、构建与上板均未在本机完成；本机（Dell）无板。实际验证结果另行追加，不在此处宣称已通过。

### 1. 两个教学式对照问题

- **E2：翻译状态替换问题。** V 的两个 4 KiB 目标翻译在预热后确实驻留；A 以有限搬运覆盖相关槽位；在固定 T 后完全排空并首次探测 V。要回答的是：A 归属的填入是否实际覆盖 V 目标条目，以及探测是否因此首次 miss。状态证据、miss 证据和性能后果分开报告。
- **自然占用：实际成本问题。** 不先假定“按真实翻译前控制占用计费”一定优于简单计数；先比较同一批 4 KiB 拷贝的实际控制器占用时间能否由拷贝数、翻译请求数、miss 数以及预拟合的 hit/miss 类型权重解释。只有在相同简单计数下仍有可复现剩余差异，且计时能在线获得、相对强基线有可执行差异规则时，才另行冻结 M 方法与强基线。当前 M 未实现，不设精确效能阈值。

### 2. 冻结 84 次包（短 6 + 主 E2 60 + 主 cost 18）

| 阶段 | 条件 | 重复 | 作用 |
|---|---|---|---|
| 短验收 | E2 W0/W1/W16；自然占用 Solo/A8gap0/A16gap0 | 各 1（共 6） | 仅功能/接口/初态/排空/数据验收 |
| E2 主 | W0/1/4/7/8/16 | 各 10（共 60） | 目标覆盖与首次 miss |
| cost 主 | Solo/A1gap0/A8gap0/A16gap0/A8gap1000/A8gap2000 | 各 3（共 18） | 实际占用计时是否超出简单计数 |

- E2 的 W 为 A 源、目的各 W 页；V 固定源、目的各 1 页；所有拷贝 4 KiB；A 有限搬运 112 次（W0 不启 A）；V 预热 2 次，最后 probe 1 次。
- E2 的 A 阶段固定 T=5,000,000 周期（50 MHz 下 100 ms）；截止由硬件在 ARM_A 后精确执行并快照，软件不得临时延长。
- cost 主试次 V 为 4096 次、预热 256 次；短试次 64/8；A 连续工作至 V 结束，再停止并排空。
- 运行命令（计划，板端名）：`platform-research all /tmp/research-v1-01`。仓库 runner 对应 `tests/board-research.sh`，`MODE=all` 按冻结顺序包含 6 个短验收后接 78 个主试次（60 E2 + 18 cost）。短验收也可单独运行 `platform-research short` 仅作功能检查；无自动续跑、无重跑、无失败后跳过。第一处真错停止并保留全部已有原始结果。
- 输出目录必须完整归档；JSON 是主证据，不能只看 UART/串口摘要。原始数据、失败记录和未完成目录一并保留。本机为 Dell 无板，不能在本机执行上板运行。

### 3. E2 精确时序与 CPU 等待

- 命令序列：`CLEAR(6)` -> `MODE=1(E2)` -> `WARM(1)` -> `A(2)` -> 硬件到 `READY(6)` -> `PROBE(3)` -> `FREEZE(4)`；异常用 `ABORT(5)`。
- `T_CYCLES` 默认 5,000,000。`A(2)` 被接受的边沿记录 `astart=epoch_q`（pre-edge）并设置 `deadline=epoch_q+T_CYCLES`；当 `a_elapsed+1 >= T_CYCLES` 时相位变为 `READY`，并把 `deadline_n` 写成该转换前的 `epoch_q`，`a_elapsed` 饱和到 T。即 T 是 ARM_A 接受边沿之后的固定周期数。
- **pre-edge 时间戳/快照**：WARM 快照在 `A(2)` 接受边沿采集，DEADLINE 快照在恰好 T 的边沿采集，FREEZE 快照在 `FREEZE(4)` 接受边沿采集；三者都用该边沿之前的 `entries_i`（旧标签）。事件日志也写 `epoch_q`（pre-edge）以及同一槽位的旧值 `entries_i` 和新值 `entries_next_i`。
- **READY 的 CPU 等待**：READY 是硬件自主状态，软件轮询到 `phase==6` 后才发 `PROBE(3)`。`probestart_q` 记录 PROBE 接受边沿的 `epoch_q`。因此 `CPU_WAIT = probestart - deadline`（模 64 位）必须单独记录，不能并入 T。READY 期间若出现 lookup/update/invalidate/issue/done/select_fire/response_error 或非 quiet，置 `e2_err[5]`。
- 截止时若仍有 A 或未排空，按执行失败停止，不延长；配置失败不是科学阴性。

### 4. 实际 RTL128 键编码（含 GSCID）

`ctx_key_128` / `id_match` 的匹配键为：

| 位域 | 含义 |
|---|---|
| `[28:0]` | IOVA VPN：`lookup_iova_i[40:12]` |
| `[48:29]` | PSCID |
| `[49]` | stage-1 enable |
| `[50]` | stage-2 enable |
| `[51]` | valid（探测键置 1；普通 4K 判断另看条目） |
| `[79:64]` | GSCID |
| `[63:57]`、`[127:80]` | 0 |

- `id_match` 只比较 vpn、pscid、gscid、en1、en2；**不比较 DID**。DID 只用于所有权/条目归属（V=DID10，A=DID11）。
- 普通 4K 目标条目条件：valid=1、stage1=1、stage2=0、S1/S2 的 1G/2M 均为 0、非 global。
- 原始 128 位条目字：低 64 位含 vpn/pscid/en/valid/几何/global；高 64 位低 16 位含 GSCID；其余高位为零。

### 5. raw 4096 字导航（研究 bank 相对索引）

驱动 `idma_research_read(d, i)` 实际访问 `IDMA_RESEARCH_BANK + i`；下列为研究 bank 相对索引的读映射分组。

| 索引 | 内容 |
|---|---|
| `0x0000-0x0032` | magic/status/error、mode/cmd/phase、astart/a_elapsed/epoch、事件/失效/更新/quiet 计数、V 两个目标键与上下文、首次探测记录、snapshot_valid/PLRU、A 计数、IOVA 配置 |
| `0x0100-0x011F` | WARM 快照：16 槽 x 2 字（旧标签） |
| `0x0120-0x013F` | DEADLINE 快照：16 槽 x 2 字（恰好 T 的旧标签） |
| `0x0140-0x015F` | FREEZE 快照：16 槽 x 2 字 |
| `0x0200-0x07FF` | EVENT RAM：最多 256 事件 x 6 字；只读前 `event_count*6` 字有效 |
| `0x0800-0x0BFF` | SAMPLE RAM：最多 256 样本 x 4 字；只读前 `sample_count*4` 字有效 |
| `0x0C00-0x0C2D` | COST 会话寄存器组：magic、active/open/closed、错误、会话计数、窗口计数、当前 A/V 上下文、样本/进位标志、DID 常量 |

COST 组常用点：`0x0C00` magic=`0x49444d4152454331`；`0x0C01` active/open/closed；`0x0C02` cost_err；`0x0C03` cost_cycles；`0x0C04/05` cost_main[V/A]；`0x0C06/07` cost_comp；`0x0C08/09` cost_sel；`0x0C0A/0B` cost_hit/miss lane0；`0x0C0C/0D` lane1；`0x0C0E` owner_err；`0x0C0F/10` A issue/done；`0x0C11/12` V issue/done；`0x0C13` A done all；`0x0C14` eligible；`0x0C15` sample_count；`0x0C16` sample_ovf；`0x0C17/18` carry_in/out；`0x0C19` win_cycles；`0x0C1A/1B` win_main；`0x0C1C/1D` win_comp；`0x0C1E/1F` win_sel；`0x0C20/21` win_hit/miss lane0；`0x0C22/23` lane1；`0x0C24-0x0C29` cur_a_*；`0x0C2A/2B` cur_v_*；`0x0C2C` lane_active；`0x0C2D` DID 常量。

### 6. event meta、首次探测与 sample flags

**事件日志（6 字/事件）**：`[0]` pre-edge epoch，`[1]` evt_meta，`[2:3]` 旧槽低/高 64 位，`[4:5]` 新槽低/高 64 位。只有 `update_i && replace_mask 一位有效` 时才记录；相同 old==new 也会记录。

`evt_meta` 常用位：

- `[15:0]` `replace_mask`（一位有效槽位）
- `[20:16]` 最后变化槽索引
- `[23:21]` E2 相位
- `[47:24]` `update_did`
- `[48]` 恒 1（记录有效）
- `[49]` `changed_count==0`（old==new）
- `[50]` `changed_count>1`
- `[51]` `changed_count==1 && changed_mask != replace_mask`
- `[52]/[53]` 目标 0/1 在旧槽存在、但在新槽不再存在
- `[54]` `replace_mask` 不是一位有效

**首次探测（每 V 页）**：`fp_time` 是第一次合格 V lookup 的 pre-edge epoch；`fp_hit` 是实际 `lookup_hit`；`fp_live_self/fp_live_oth` 是探测前 16 槽中各自/另一目标键仍 valid 的位图；`fp_key` 是 IOTLB 中命中的普通 4K 条目键，否则为探测键。探测自身的 lookup 造成的替换必须单独识别，不能算作 A 的作用。

**每第 16 个完整窗口内完成 A 拷贝的样本（4 字）**：`[0]` A 的 `done_id`，`[1]` `{cur_a_main, cur_a_comp}`，`[2]` `{cur_a_req, cur_a_miss}`，`[3]` `{cur_a_dur, cur_a_flags}`。样本条件是：该 A 拷贝在窗口内发起（`lane_in_window`）、在窗口仍活跃时完成（`win_act`），并按 `eligible[3:0]==0xF` 每 16 个完整包含的完成抽取一次。`cur_a_flags`：`[0]` 发起前窗口未开，`[1]` 发起时窗口已关，`[2]` 发起在窗口内，`[3]` 完成时 `p4m_close`。跨边界拷贝由 `carry_in`/`carry_out` 单独计数，不混入“完整包含”样本。

### 7. cost：整臂计数 vs 共同窗口

- **整臂**：`cost_active` 从 `p4m_arm` 到 `p4m_close`，`cost_*` 计数器覆盖整段会话（含 open 之前和 close 边沿）。`cost_cycles` 与 `cost_main/comp/sel/hit/miss/issue/done` 是整臂口径。
- **共同硬件窗口**：沿用旧 P4M 的 `(open, close]`：open 边沿不计入，close 边沿计入。`win_*` 只在 `win_act`（`cost_active && window_open_q && !window_closed_q`）累计。窗口内发起的判断用 `win_act_issue`，在 open 边沿可与 `p4m_open_i` 同时成立，因此“发起在窗口内”的 A 拷贝可能包含 open 边沿；但 `win_*` 周期计数本身仍按寄存器化 `window_open_q` 排除 open 边沿。报告时必须明确用的是整臂还是窗口口径，不能两者混用。
- `carry_in` 在 open 时记录 A lane 是否已活跃；`carry_out` 在 close 时记录 A lane 是否仍活跃。

### 8. 被动原型与版本兼容

- 本 observer 是**纯被动只读 tap**：输出不驱动 DMA、IOTLB、翻译逻辑、命令队列或中断。
- **observer v8 与 kernel/BIT 必须匹配**：驱动在 `idma_research_begin` 中检查 `IDMA_OBS_MAGIC_V8` 与研究 bank magic，不匹配返回 `-EOPNOTSUPP`。新 ioctl 为 `IDMA_IOC_RUN_RESEARCH (0x0e)`。现有 v7 P4M 旧 hook 保持不变，但**不得声称 v7/v8 两个 BIT 之间性能等价**，也不得用 v8 结果当作 v7 基线复现。
- 当前构建状态为**pending**，未宣称通过；不编造 hash/版本号。实际编译、上板与验证结果将在执行后另行追加。
- 分析侧：返回后计划用 `scripts/research_results.py INPUT --output NEWDIR` 处理；该脚本当前与实现并行开发，尚未产出结果。

### 9. E2 因果链与自然占用结论边界

- E2 阳性链必须同时包含：真实上下文中两个 V 目标初始驻留 -> A 归属的实际 old->new 覆盖事件（事件日志，含旧/新完整标签）-> 截止时全部槽位中目标键不存在 -> 首次探测真实 miss。任一环节缺失或存在主动失效/未知归属/溢出/上下文或页尺寸不符，该次无效并停机。
- 探测自身引起的替换单独记录；不能把探测造成的损失算作 A 的作用。没有替换、没有 miss 的试次是有效阴性，必须保留，不是失败。
- 自然占用目前只是筛选诊断：未实现 M，不宣称新方法收益，不设精确效能阈值。简单计数/类型加权仍解释剩余差异时，不进入 M；“有波动”本身不是创新或收益证据。