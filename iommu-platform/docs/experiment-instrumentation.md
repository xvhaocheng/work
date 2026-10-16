# 实验请求源、观测与软件/RTL 分工（讨论稿）

日期：2026-09-08。接续 [实验总设计](history/plans/experiment-plan.md)。本文定义候选硬件观测接口与实现顺序；这些 RTL 观测尚未接入。软件时间和 per-DID HPM 会话已实现并构建，范围见 [实验入口 v1](experiment-v1.md)；尚未上板校准。

最新路线修正：FPGA 是实验主线，仿真列为可选辅助；下面的仿真 trace 是可用手段的设计，不要求先搭通仿真再开展板上实验。实施顺序以 [FPGA 实施计划](history/plans/experiment-implementation-plan.md) 为准。

## 两种环境承担不同证据

| 能力 | Linux/FPGA 系统实验 | 真实 IOMMU RTL 定向仿真 |
|---|---|---|
| 主要用途 | E0/E1、E4 的系统耗时与实际压力；后续复核机制触发条件 | E0/E2/E3、E4 的请求时序与因果对照 |
| 请求源 | 初版可用驱动提交，但须暴露供给限制 | 符合 AXI 协议的可控设备请求源 |
| 配置 | 驱动设置映射、DID 事件过滤、预热及测量边界 | 通过编程接口配置真实 DUT，内存模型提供目录/页表 |
| 观测 | 软件任务时间、per-DID HPM、每路实际流量；必要时补硬件监测 | 波形/被动 monitor 读取入口、FIFO、翻译、缓存、PTW、出口及响应 |
| 可后置 | FPGA 上的自主流量发生器、完整逐请求 trace 寄存器 | 完整 CPU/Linux 启动和真实 DDR 模型 |

仿真首版可从完整 `rv_iommu_top` 的请求入口开始；如果未包含外部 DMA mux，结论不覆盖该 mux。需要判断外部仲裁时，再接入与平台相同的 mux 和独立请求端口。设备请求源不等于完整 iDMA，后续应与真实 iDMA 的读写轨迹对应。

## 每个请求的时间线

| 事件 | 定义 | 为什么需要 |
|---|---|---|
| t_offer | 在观测入口第一次提出该 AR/AW 请求，VALID 保持至受理 | 记录受理前等待；不能把尚未受理的请求当作尚未到达 |
| t_accept | 同一入口 VALID 与 READY 握手 | 将外部接入等待与内部排队分开 |
| t_trans | 请求被翻译引擎接口受理 | 发现请求已进 FIFO、却尚未开始翻译的等待 |
| t_lookup | 对该请求实际执行 IOTLB lookup | 有效 hit/miss 的观察点；lookup 前无 miss 事件不等于 hit |
| t_trans_done | 对应翻译响应被控制器接收 | 包含目录/上下文处理的翻译服务时间，不能直接当作纯 PTW 时间 |
| t_issue | 对应翻译后 AR/AW 在外部 comp 接口被受理 | 区分翻译服务与出口排队/反压 |
| t_done | 上游对应 RLAST 或 B 响应握手 | 请求完成时间；写事务还包含 W 数据供给影响 |

最先用于定向仿真的分段量为接入等待 `t_accept−t_offer`、内部队列等待 `t_trans−t_accept`、翻译服务 `t_trans_done−t_trans`、出口等待 `t_issue−t_trans_done` 和其后的完成区间。各段必须关联同一请求及相同路径；不能把未关联的全局时间差相减。

实际计时事件依仿真采样约定统一到时钟边沿，同周期事件差可为 0。软件 Victim 任务时间另行保存，包含描述符间隙和软件观测延迟；不可用单请求硬件时间或 active 累加代替整批任务完成时间。

初版 trace 记录 `(cycle, event, device, channel, request_sequence, AXI_ID, IOVA, context, state)`。不同边界的 AXI ID 可能经 mux 扩展，同一 ID 也可能重复使用；需要按端口、读写通道及事务顺序关联。内部 translation request 结构无 AXI ID，应由 monitor/scoreboard 沿 FIFO 与控制器选择路径维护映射，不能假设仅凭 DID 即可唯一识别事务。

## E2/E3 的最小额外观测

- E2：V 相关条目标签/有效位、lookup 与 hit、更新来源、实际替换槽、失效操作；状态初始化、A 排空和首次探测均可控。按板上资源选择触发式 trace/状态观测，也可在需要时用仿真补充；没有直接条目证据时保留为翻译状态影响。
- E3：正在处理的请求身份、FIFO 接收/取出与顺序、控制器状态、PTW 起止及所属请求、页表响应时刻、出口 ready。用旁路观察确认 V 条目驻留且上下文匹配，不能通过 force DUT 内部 hit/ready 来伪造正常路径。
- 页表内存模型分别控制请求受理和响应返回，遵守握手/ID/突发协议。页表、DDT/PDT 等结构地址范围和响应类型需可识别；只延迟目标页表访问，不把所有 DS 请求一并拖慢作为纯 PTW 对照。
- 测试须同时检查请求最终完成、顺序/ID 和数据正确性；不能通过不返回合法数据来人为制造目标之外的阻塞。

## FPGA 初版应做多少

先让软件实现独立 V 窗口、A 停止/排空、映射与预热控制、错误传播、per-DID HPM 校准和原始结果记录。用于系统影响表征时，可保留软件供给方式，但明确其范围。

若要用板上结果声称实际硬件请求形成了竞争，须增加或使用能观察请求的手段。每路 mux 前 AR/AW 的提出/受理、返回完成、`VALID && !READY` 周期、在途计数，是候选最小监测项；FIFO 内排队并不会体现在这些入口 stall 中，因此仅此还不能定位阻塞。两路同时存在未完成请求的统计也不能替代具体共享资源上的 pending 证据。

板上精确阶段时间戳、内部 FIFO/控制器计数及触发式 trace，按既定因果要求及板上或仿真暴露的证据缺口选择。若新增计数器，需定义同时快照/门控边界、计数位宽、读写响应归属和测量开销；不能在一个长窗口结束后凭多个累计值还原逐请求因果。

硬件自主提交、循环描述符或可控流量发生器可后置：先导数据若表明 CPU 补交间隙使目标压力无法成立，再选择最小改动。采用软件供给下的负结果只能描述该供给条件；不能否定仿真已证明、但板上未触发的机制。

## 本轮局部代码依据

1. `ariane_peripherals_xilinx.sv:880` 定义每路扩展 AXI 请求/响应，`:938` 附近实例化 `i_dma_iommu_tr_mux`，mux 前后均有现成接口边界可供监测。
2. `rv_iommu_top.sv:75` 的 `tr_req_i/tr_resp_o` 是请求入口，`:79` 的 comp 是翻译后数据路径，`:83` 的 ds 是数据结构路径，`:87` 的 prog 是编程路径。内部 `trans_req_valid/ready` 和 response 握手可被仿真观察。
3. `rv_iommu_trans_ctl_wrap.sv:113` 和 `:138` 附近分别实例化 AW/AR FIFO，usage 输出未连接。`:579` 起控制器选择请求，依次经历 REQ_TRANS、AWAIT_RESP、COMPLETE，直到内部 comp 地址请求被受理才取出对应 FIFO 头并返回 IDLE。这提供应测排队/前进条件的候选，尚不是已完成的 HOL 实验证据；也不表示所有数据响应完全串行。
4. `rv_iommu_tl_wrap.sv:238` 将 IOTLB lookup 与 PT/MSI 状态关联；`:266` 根据 PT 状态和 miss 触发 PTW。`i_rv_iommu_ptw.active_o` 在 `:579` 当前未连接到父层；仿真可观察子模块，板上导出需新增连接/逻辑。
5. `rv_iommu_top.sv:280` 附近 DS IF 聚合 DDT、PDT、PTW、队列等请求。平台 `ariane_peripherals_xilinx.sv:1042` 附近 delayer 位于汇合后的 DS 路径。因此既有 50/50 配置不等于仅延迟 PTW，也不宜单独作为 E3 的选择性 PTW 干预。

以上都是定向静态核查。未执行新的编译、系统仿真或上板测量，未将内部可见信号描述成已经可从 Linux 读取的寄存器。

## 实现顺序与完成标准

1. 定好事件和结果规范，重写 Linux 测量入口，准备实际 FPGA 软硬件组合及验收。
2. 在 FPGA 上完成 E0 和 per-DID 校准，按需要补最小观测，开展 E1/E2 与匹配对照；检查真实压力，不要求一定出现 slowdown。
3. 根据既定 E3 因果要求选择板上触发/trace/请求控制；压力或可控性不足时才评估最小硬件补充，或用定向仿真辅助内部机制判断。
4. 达到测量条件后执行正式矩阵与扩展。仿真不成为板上实验的前置依赖，仿真和板上结果分别标注。

机制假设和判据已提前规划；最小软件实现见 [实验入口 v1](experiment-v1.md)，本文的额外硬件观测和因果控制仍待按实验需要实施。

## 2026-10-02 追加：被动 observer v8 的 E2/自然占用同一包协议与 84 次冻结计划

本节记录 observer v8 的执行协议。主机验证和构建状态见 docs/validation.md 最新条目；本机（Dell）未接板，尚无本版板测结果。

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
- 板端运行命令：`platform-research all /tmp/research-v1-01`。仓库 runner 对应 `tests/board-research.sh`，`MODE=all` 按冻结顺序包含 6 个短验收后接 78 个主试次（60 E2 + 18 cost）。短验收也可单独运行 `platform-research short` 仅作功能检查；无自动续跑、无重跑、无失败后跳过。第一处真错停止并保留全部已有原始结果。
- 输出目录必须完整归档；JSON 是主证据，不能只看 UART/串口摘要。原始数据、失败记录和未完成目录一并保留。本机为 Dell 无板，不能在本机执行上板运行。

### 3. E2 精确时序与 CPU 等待

- 命令序列：`CLEAR(6)` -> `MODE=1(E2)` -> `WARM(1)` -> `A(2)` -> 硬件到 `READY(6)` -> `PROBE(3)` -> `FREEZE(4)`；异常用 `ABORT(5)`。
- `T_CYCLES` 默认 5,000,000。`A(2)` 被接受的边沿记录 `astart=epoch_q`（pre-edge）并设置 `deadline=epoch_q+T_CYCLES`；当 `a_elapsed+1 >= T_CYCLES` 时相位变为 `READY`，并把 `deadline_n` 写成该转换前的 `epoch_q`，`a_elapsed` 饱和到 T。即 T 是 ARM_A 接受边沿之后的固定周期数。
- **pre-edge 时间戳/快照**：WARM 快照在 `A(2)` 接受边沿采集，DEADLINE 快照在恰好 T 的边沿采集，FREEZE 快照在 `FREEZE(4)` 接受边沿采集；三者都用该边沿之前的 `entries_i`（旧标签）。事件日志也写 `epoch_q`（pre-edge）以及同一槽位的旧值 `entries_i` 和新值 `entries_next_i`。
- **READY 的 CPU 等待**：READY 是硬件自主状态，软件轮询到 `phase==6` 后才发 `PROBE(3)`。`probestart_q` 记录 PROBE 接受边沿的 `epoch_q`。因此 `CPU_WAIT = probestart - deadline`必须单独记录，不能并入 T。READY 期间若出现 lookup/update/invalidate/issue/done/select_fire/response_error 或非 quiet，置 `e2_err[5]`。
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
| `[52]/[53]` | S1 的 1 GiB / 2 MiB 标志 |
| `[54]/[55]` | S2 的 1 GiB / 2 MiB 标志 |
| `[56]` | global 标志 |
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
- `[20:16]` 最后变化槽索引；old==new 时该字段为 0，实际写槽须从 replace_mask 得到
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

### 7. cost：启动至结束计数 vs 共同窗口

- **启动至结束**：`cost_active` 从 `p4m_arm` 到 `p4m_close`，`cost_*` 计数器覆盖整段会话（含 open 之前和 close 边沿）。`cost_cycles` 与 `cost_main/comp/sel/hit/miss/issue/done` 是启动至结束口径。
- **共同硬件窗口**：沿用旧 P4M 的 `(open, close]`：open 边沿不计入，close 边沿计入。`win_*` 只在 `win_act`（`cost_active && window_open_q && !window_closed_q`）累计。窗口内发起的判断用 `win_act_issue`，在 open 边沿可与 `p4m_open_i` 同时成立，因此“发起在窗口内”的 A 拷贝可能包含 open 边沿；但 `win_*` 周期计数本身仍按寄存器化 `window_open_q` 排除 open 边沿。报告时必须明确用的是启动至结束还是窗口口径，不能两者混用。
- `carry_in` 在 open 时记录 A lane 是否已活跃；`carry_out` 在 close 时记录 A lane 是否仍活跃。

### 8. 被动原型与版本兼容

- 本 observer 是**纯被动只读 tap**：输出不驱动 DMA、IOTLB、翻译逻辑、命令队列或中断。
- **容量配置**：本次 research profile 设置 `legacy_e3_observer=false`，不综合旧 E3 详细追踪模块；旧 E3 命令不用于本包，复现旧 E3 仍需其原 BIT。新研究记录、P4M/path 计数和 DMA/IOMMU 数据通路保留；其他 profile 未指定此项时默认保留旧追踪。
- **observer v8 与 kernel/BIT 必须匹配**：驱动在 `idma_research_begin` 中检查 `IDMA_OBS_MAGIC_V8` 与研究 bank magic，不匹配返回 `-EOPNOTSUPP`。新 ioctl 为 `IDMA_IOC_RUN_RESEARCH (0x0e)`。现有 v7 P4M 旧 hook 保持不变，但**不得声称 v7/v8 两个 BIT 之间性能等价**，也不得用 v8 结果当作 v7 基线复现。
- 完整软件与 FPGA 构建均退出 0；最终产物、哈希及主机验证记录见 docs/validation.md。尚未上板。
- 分析侧：返回后计划用 `python3 scripts/research_results.py all INPUT --output NEWDIR` 处理；该脚本已通过主机验证，输出 results.json 与 report.zh.txt；尚未分析本版真实板测数据。

### 9. E2 因果链与自然占用结论边界

- E2 阳性链必须同时包含：真实上下文中两个 V 目标初始驻留 -> A 归属的实际 old->new 覆盖事件（事件日志，含旧/新完整标签）-> 截止时全部槽位中目标键不存在 -> 首次探测真实 miss。阳性链不完整时不能认定该次存在 A 替换因果；正常驻留或命中仍是可保留结果。出现主动失效、未知归属、溢出、上下文或页尺寸不符则属于采集／协议异常，在线遇错停止。
- 探测自身引起的替换单独记录；不能把探测造成的损失算作 A 的作用。没有替换、没有 miss 的试次是有效阴性，必须保留，不是失败。
- 自然占用目前只是筛选诊断：未实现 M，不宣称新方法收益，不设精确效能阈值。简单计数/类型加权仍解释剩余差异时，不进入 M；“有波动”本身不是创新或收益证据。
