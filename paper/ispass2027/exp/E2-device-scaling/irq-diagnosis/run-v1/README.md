# Root 专用两点 timer 诊断包：仅准备，未执行

本包只生成命令文本、离线核验已保存的 CLI 输出；[prepare_run.py](prepare_run.py) 没有 UART/JTAG/SD、进程派发或目标命令执行入口。Root 审查本包后才决定何时运行；不占用现成 E4/E5 板时。所有输出只用于诊断，不进入正式性能池，不调用 E2 的90点 main、恢复续点或论文导出。

## 固定身份与两点预算

使用原 E2 DS0 BIT：`/home/xvhaocheng/work-root-a/paper/ispass2027/board/packages/baseline100-20261017/artifacts/ds0.bit`，既有 SHA `e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6`；实际 CPU CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`，CPU50MHz/timebase25MHz，DID10..13，DS0/0。复用既有 BIT manifest，不新增 BIT 构建或重复哈希。

唯一诊断软件为 [实际 passed 构建清单](../builder-v1/final-build-manifest.json)：Linux `1facb309758e3d41622355de71dbc033cd0fac97` / platform `5baab2217eb4877da29d5c38f12749a80c5cdf4f` / OpenSBI `e8d21bfdc2dd753314627968e496c3a1188f6dfb`，release **`6.5.0-rc2-ispass2027-e2-timer-probe-v1`**。本机 image 在 `../builder-v1/artifacts/e2-timer-probe-software01/fw_payload.bin`，SHA `05e96cbf9c27da908ad5e87230c256f49a149c9e839e718589fec06ad0bad577`；29,363,656 bytes。原 passed full-E3 rootfs、全部7个 CLI、config/DTS 输入和 OpenSBI保持原字节；最终 config 只变 rootfs 绝对路径与 release。SPI 模块只供既有软件更新流程，正常诊断启动不加载外部模块。

Root 按[通用上板 skill](../../../../board/skill/genesys2-iommu-board/SKILL.md)另排更新/加载，保存新的 load receipt、完整 boot raw、新 boot ID；taint 必须0。不能沿用旧失败 boot `5df16afb-ce70-4098-a71f-4ff462079c1e` 的 receipt。renderer 核对新 load/kernel/DS0 BIT，且 receipt 完成时间不得早于本次诊断构建；原 load 程序本身核对 boot/kernel。板端每阶段再次核对该新 boot、精确 release、CPU online=0、正常阶段 taint0。

只运行下面两点，各一次，按顺序；不加入 Solo、capability 压力点或额外 A/B 重复：

| 次序 | 目标命令原字节 | 核心预算 / CLI 外层 / host |
| --- | --- | --- |
| 1 低压控制 N2_A1 | `platform-timeout 120 idma_scaling --devices 2 --aggressor-pages 1 --timeout-ms 60000` | 60000ms / 120s / 180s |
| 2 诊断 N4_A8 | `platform-timeout 120 idma_scaling --devices 4 --aggressor-pages 8 --timeout-ms 60000` | 同上 |

原 CLI 内置 COPY4096、victim transfers4096、warmup256，V0 / A1..、selected A1；不改测量预算。原 [command()/parse()](source/collect_multi.as-reviewed.py) 和 profile helper 原字节在 [source-manifest](source-manifest.json)固定。只导入 command/parse，不调用原 collector main 或旧 software profile 的运行入口。

## 命令阶段与人工停止点

[审查模板](review-plan.template.json)保存全部实际命令字符串；其中 boot UUID 是明确的 synthetic 占位，身份 guard 会在真实机器上拒绝，不能直接执行。新 load receipt 到位后，Root 可只在主机渲染绑定实际 boot 的 plan：

```sh
python3 /home/xvhaocheng/work-e2-irq-diagnosis/paper/ispass2027/exp/E2-device-scaling/irq-diagnosis/run-v1/prepare_run.py render \
  --load-manifest /absolute/new-diagnostic-ds0-load/load.json \
  --out /absolute/new-diagnostic-plan.json
```

输出仍只是 JSON。Root 沿用 `board/uart.py` 或现有 `Console.command`，每次只选择一个 step 的 `command`/`host_timeout_seconds`，保存新的独立 UART 输出目录；不写遍历 steps 的自动循环。完整计划正常 host 命令预算合计450s，不含更新/加载、人工审查和恢复；这不约束超时后远端 DMA 的存活时长。

1. `preflight`（host30s）：精确 boot/kernel、taint0、CPU online=0；如需只挂载 debugfs，随后须 control=0、snapshot 可读。Root 检查初始 snapshot 的 start/stop/count/events/records_written 均为0，确认此新 boot 尚未启用过 probe。保存固定 `/proc/interrupts`、`softirqs`、`timer_list`、`stat`、`uptime`、watchdog/thresh、current_clocksource。缺失/空输出按实际保留，不假称已有 timer_list 数据；必要证据缺失则不继续。
2. `N2_A1_window`（host180s）：先在 off 状态保存 before snapshot 和同组 proc。保留原 KERNEL/BOOT/TAINT/PRE_SEQ，**在原 CLI 调用前写 control=1**；CLI 返回即写0并核对off，早于原 CASE_EXIT/dmesg/TAINT_AFTER。因此原 CLI JSON 输出和退出到 freeze 的调度尾部计入 probe 窗口；它不是纯硬件 V 窗口。若 CLI 未返回，freeze 也未必执行。
3. Root 离线核验已保存的 N2 CLI raw；原 kernel/boot/taint0、唯一 CLI JSON、ABI/预算、observer sequence、STOP/drain/data/quarantine、AXI/PATH 等门完整保留。N2 预期生命周期 mask=3；N4为15。任一失败即停，不发正常 post，更不发 N4。
4. N2原门通过后才发送 `N2_A1_post`（host30s）：再次要求正常身份、taint0、control0，再读取该次独立 immutable snapshot 与固定 proc。Root 检查 counter 前进、start<stop、handler/rearm count 非零、输出完整；异常只说明该诊断不可接受，不换预算补跑。Root 接受这一个低压控制后才决定发送 N4。
5. `N4_A8_window` 和正常 `N4_A8_post` 完全沿用上述次序与错误停止；N4 CLI 验收还要求同 boot 的已接受 N2 CLI receipt 和连续 observer sequence。完成后保持 off，不追加点、重复或自动回切。

离线 CLI 核验命令不接触硬件，也不派发后续阶段：

```sh
python3 /home/xvhaocheng/work-e2-irq-diagnosis/paper/ispass2027/exp/E2-device-scaling/irq-diagnosis/run-v1/prepare_run.py validate-cli \
  --plan /absolute/new-diagnostic-plan.json --condition N2_A1 \
  --raw /absolute/N2-window-uart/command-UUID.json --out /absolute/new-N2-cli-validation.json
```

N4使用 `--condition N4_A8` 并另传 `--previous /absolute/new-N2-cli-validation.json`。它只核验原 CLI 门；**不把 CLI receipt 当成 probe runtime/成本/根因验收**。Root 仍须审查冻结 snapshot 与 proc；没有正常 snapshot、缺失 timer_list、或 counter 不前进都不能写成诊断成功。

## CLI/softirq 无响应或任何错误

host 超时、非零、缺少结束标记、taint、STOP/drain/data、probe 控制/输出错误均立即停止当前包。保留 `uart.raw.log`、command JSON 的全部 `received`/stdout、主机起止时间和原 dispatch/status；不自动发送下一命令、CTRL-C、再次 enable、重试、重复点或 reset。不覆盖/补写未观测的 CLI、freeze、snapshot、quiet 或完成结果。

原120s平台 timeout 本身也可能得不到 CPU；host180s超时不等于 CLI 已退出，更不等于 DMA 停止。CTRL-C、恢复 prompt、probe control0 都不能证明 DMA quiet；若之后命令返回，原目标 shell 也可能才继续执行其排队的 freeze/dmesg 尾部。旧 failure 的最终 drain 不能转移为新 failure 的 quiet 证据。

模板另列两个**不自动执行**的 Root 恢复候选：独立 regain prompt 后一次 `freeze_once`（10s），及 Root 另判有必要且能完成的一次 `read_frozen_once`（30s）。恢复读允许记录已有 taint，以保留失败诊断，不放宽正常点 taint0 接受门；freeze 只关闭 probe，不操作 DMA。若尝试不响应则保留已收字节停止，由 Root 决定既有 reset/load 流程。即使响应，也须明示是否有实际 STOP/drain/data 证据、当时 DMA 压力是否未知；不能把恢复读计数当作压力窗口内的独立测量。

## 读数边界与下一判断

handler count/sum/max 测量 `event_handler`，rearm count/sum/max 测量原 `sbi_set_timer`；cycle/instret 包含其间 M-mode 指令。每类第64个事件或新 cycle max 才进入最后64条固定环，属于有偏选择，**不是CDF或随机样本**；较早 max 对应记录可能被覆盖，aggregate max 仍保留。`cycle_start+cycles` 是本地测量终点，`sample_cycle → SIP → SIE`是稍后有限指令边界的 pending 观察，不能视为 SBI 返回同一时刻，更不能以 STIP=1 单独判 bug。

探针自身扰动未测：每个有效事件一般新增5次原生 CSR 读，选中样本8次；启用/首次冻结各1次cycle。还有 active flag、聚合及有时的 ring stores。自身 aggregate/ring 工作位于其本地计时终点后；handler 时间包括嵌套 rearm 及其探针成本，两类 sum 不可相加。normal off 也保留 flag/branch，不能称零成本。前后 proc/snapshot 均在 off 读取；CLI 原 JSON 输出和 freeze 调度尾部仍在 active 窗口。没有本版本额外 off 重复点来实测成本，单次低压控制不能给出扰动上限。

Root 可据这一次有限数据决定下一代码设计，接受/否决范围如下；不增加新的实验性能门：

- **SBI rearm 本体耗尽已请求间隔：** 固定 CPU/timebase=2 的合同成立时，kind2若 `delta>0` 且 `cycles >= 2*delta`，说明被包住的 SBI 区段本身已至少消耗该请求间隔；它发生在已有 raw_now 之后，因此足以定位一类迟编程。若小很多，只否定该条区段本体单独耗尽间隔；不能排除此前 emulated rdtime/取数延迟、未保留事件或其他迟延。不能据此归因为 AXI。
- **handler 大量占用CPU：** 用 `handler.cycles_sum/(stop_cycle-start_cycle)`、count/原生cycle窗口和 instret 描述观测；同一 handler 包含嵌套 rearm成本。较高占用支持受测 handler 区域消耗CPU时间，较低只限制该受测区域。tickless/highres 下 IRQ rate 不必等于HZ250，单凭IRQ频率或 __do_softirq 栈不判周期错误。
- **CPU/AXI根因：** 长cycle、低instret/cycle可支持这些区段退休进展少，不能区分CPU/缓存/CLINT访问/AXI仲裁等原因。本包没有独立AXI进展测量，不能接受AXI单因结论；小rearm成本也不能排除计时读陷入/其他CPU路径。
- **复现与控制：** 低压控制失败则停止并保留，诊断未定；N4不复现也不能声称修复。恢复窗口正常tick只描述恢复窗口，proc计数多为累计值且Linux now与rdtime链相关；不能反推故障时正常，也不能仅据累计 hang 把全部归到N4。

counter权限原件、完整源码和 off 路径证据见 [probe-v1](../probe-v1/README.md)。Runtime错误码、per-open生命周期、实际开销/扰动、两个诊断点及恢复候选在本批均未执行。[23项主机检查](validation/host-checks.json)只检查阶段顺序、计时预算、shell语法和原错误停止；其中成功/损坏CLI夹具显式复用旧N2输出并替换 synthetic header/命令绑定，不是新 Image、probe 或板测数据。
