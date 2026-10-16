以下为可保存草案与事实摘要；**未执行命令、未修改文件**。结论先行：本次 172 只跑到第 53 项，`52 completed / 1 failed / 119 not run`，不是正式 172 通过，也不是收益证据；失败现场只到 SSH transport 超时，未取得 CLI JSON/exit/boot.after。链路故障与 RTL 死锁均不能写成已证实。

## 1. 导航与核心事实

- 运行目录：`iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/`
- 汇总：`summary.json`；停止原因：`STOPPED.txt`；运行清单：`manifest.json`；启动身份：`boot.json`；计划：`plan.json`。
- `summary.json:2-3,758-772`：`attempted=53`，`planned=172`，`completed=52`，`failed=1`，`status=stopped`，`through_phase=explore`，`run_id=c226ee7a78b70c46`，`updated_at=2026-10-05T00:31:30Z`。
- `manifest.json:7-23`：attach-only，无 BIT 写入、无 reboot、无 SD change；`phases={short:16, noise:12, explore:144}`。
- `boot.json:1-38`：`boot_id=29d710b2-4949-402d-8055-8734cbaa5220`，`uname_r=6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`，`taint=0`，composition ABI 1、128 words、ioctl 0x11、protected DID 10。
- `plan.json:2586-2594`：冻结计划 `count=172`，`short=16`、`noise=12`、`explore=144`。
- `STOPPED.txt:1-4`：`reason=CompositionError: case explore-b1-g1000-a8-mpn ... ssh password command timed out ... ssh transport failed rc=255 ... boot id changed: None ... record is not a JSON object`。
- 失败 case meta：`cases/explore-b1-g1000-a8-mpn/meta.json:52-72`：`boot_after=null`，`exit=null`，`framed_exit=null`，`taint_after=null`，`problems` 包含 SSH timeout、rc=255、非 JSON、`boot id changed: None`。
- `boot id changed: None` 只表示失败后未能完成 boot 比较，不是“重启已发生/未发生”的证明。

## 2. 计数、阶段、配置覆盖

### 已完成口径

- `short`：`16/16` 技术完成。覆盖 `mode 0/1/2/3` × `a_pages 0/1/8/16`，`transfers=16`，`warmup=16`，`v_gap=0`。`summary.json` 中对应 `short-ms/mn/mp/mpn-a0/a1/a8/a16` 均 `status=completed`，`problems=[]`。
- `noise`：`12/12` 技术完成。覆盖 `pair 1..6` × `slot 0/1`，固定 `mode S(0)`，`a_pages=8`，`transfers=4096`，`warmup=256`，`v_gap=0`。见 `summary.json:228-395`。
- `explore`：`24/144` 技术完成：
  - `explore-b1-g0-a0/a1/a8/a16` 四种 mode 共 `16` 项完成；
  - `explore-b1-g1000-a0` 四种 mode 共 `4` 项完成；
  - `explore-b1-g1000-a1` 四种 mode 共 `4` 项完成。
  合计 `16+4+4=24`。
- 失败：`explore-b1-g1000-a8-mpn`。其 meta 无有效 CLI 结果，`exit=null`、`boot_after=null`、`taint_after=null`。
- 未运行：`172-53=119`。按计划顺序，失败后剩余至少包括：`explore-b1-g1000-a8-ms/mn/mp`、`explore-b1-g1000-a16-ms/mn/mp/mpn`、`b1-g4000` 16 项、`b2` 48 项、`b3` 48 项。

### 配置覆盖边界

- `CLI timeout` 参数为 `15000 ms`：`composition_plan.py:49`，并写入每个 case 的 argv：`composition_plan.py:189-198`；计划条目也显示 `timeout_ms=15000`。
- `SSH` 侧超时为现场 300s；失败 meta 的 `finished_at` 为 `00:31:30Z`，`started_at` 为 `00:25:28Z`，约 6 分 02 秒后停止。此处只记事实，不据此推断原因。
- 未完成的 explore 不能写成收益、机制结论或 172 通过。
- `fast==0`、`miss==0` 仍是科学阴性，不是技术失败：`composition_plan.py:422`。本次失败不是科学阴性，而是 SSH transport/记录缺失类失败。

## 3. 有证据的失败时间线

1. `00:17:44Z` 创建 manifest：`manifest.json:5`，attach-only、planned 172、through explore。
2. `00:17:51Z` 采集 boot：`boot.json:3`，net3、`29d710b2-...`、taint 0。
3. `00:17:57Z` 起进入 `short`，`00:20:16Z` 完成 `short-ms-a0` 至 `short-mpn-a16` 共 16 项。
4. `00:20:19Z` 起进入 `noise`，`00:22:01Z` 完成 `noise-p1-s0` 至 `noise-p6-s1` 共 12 项。
5. `00:22:03Z` 起进入 `explore`：
   - `00:22:03Z` 至 `00:24:13Z` 完成 `b1-g0-a0/a1/a8/a16` 全 mode，共 16 项；
   - `00:24:23Z` 至 `00:24:56Z` 完成 `b1-g1000-a0` 四 mode，共 4 项；
   - `00:24:56Z` 至 `00:25:20Z` 完成 `b1-g1000-a1` 四 mode，共 4 项；
   - `00:25:28Z` 尝试 `explore-b1-g1000-a8-mpn`；
   - `00:31:30Z` run stopped，失败 case 无 CLI JSON/exit/boot.after。
6. 失败后未继续执行，`summary.json:770` 为 `stopped`。

## 4. `idma.c` 与给定 RTL 的超时/清理路径定位

### 4.1 `idma.c`：超时是协作式检查，不是阻塞事务看门狗

| 位置 | 证据 | 含义/候选 |
|---|---|---|
| `linux/drivers/misc/idma.c:66` | `#define IDMA_XFER_TIMEOUT_US 2000000` | 单次传输超时常量 2s。 |
| `linux/drivers/misc/idma.c:69` | `#define IDMA_MAX_RUN_TIMEOUT_MS 300000U` | run timeout 上限 300000ms。 |
| `linux/drivers/misc/idma.c:437-438` | `s->deadline_ns = now + IDMA_XFER_TIMEOUT_US * NSEC_PER_USEC;` | 软件 deadline 只在代码继续执行时有效。 |
| `linux/drivers/misc/idma.c:659-661` | `done = readq(...); if (done != s->transfer_id) return ktime_get_ns() > s->deadline_ns ? -ETIMEDOUT : 0;` | 先 `readq` 返回，才检查 deadline；若 `readq` 不返回，无法超时。 |
| `linux/drivers/misc/idma.c:743-744` | `readq_poll_timeout(..., done == s->transfer_id, 0, IDMA_XFER_TIMEOUT_US);` | drain 也依赖每次 `readq` 返回；它本身不是独立硬件看门狗。 |
| `linux/drivers/misc/idma.c:522-523` | `preempt_disable(); local_irq_save(flags);` | batch start 的触发段关抢占/关本地中断。 |
| `linux/drivers/misc/idma.c:539-544` | `a_id = readq(a->idma->reg + IDMA_NEXT_ID_REG_OFFSET);` | A 的 NEXT_ID MMIO 读若永不返回，不能执行后续超时/cleanup。 |
| `linux/drivers/misc/idma.c:565-568` | `v_id = readq(v->idma->reg + IDMA_NEXT_ID_REG_OFFSET);` | V 的 NEXT_ID MMIO 读同类风险；返回 0 才走 quarantine。 |
| `linux/drivers/misc/idma.c:414-417,451` | 连续 `writeq` 配置，再 `readq NEXT_ID` | 配置/触发都由 MMIO 读写组成；没有单次 MMIO 事务超时。 |
| `linux/drivers/misc/idma.c:2269-2286` | `idma_composition_read` 读 magic/ABI/generation/quiet/config，再写 config、读 status/generation/config | composition configure 全为阻塞 MMIO；无独立超时。 |
| `linux/drivers/misc/idma.c:2296-2320` | `for (i=0; i<IDMA_COMPOSITION_WORDS; i++) p->raw[i]=idma_composition_read(...)`，随后多项校验 | capture 逐字读；若某次读不返回，capture 不完成。 |
| `linux/drivers/misc/idma.c:2329-2335` | `while (idma_composition_read(d,5)!=1) { if (ktime_get_ns()>=deadline) return -ETIMEDOUT; cond_resched(); }` | restore 的 quiet 等待有 1s deadline，但每次 `readq` 仍可能阻塞。 |
| `linux/drivers/misc/idma.c:2415-2421` | composition configure 在 run 中调用，失败则跳 cleanup | configure 失败可返回时才进入清理；未返回则不清理。 |
| `linux/drivers/misc/idma.c:2558-2559` | composition 走 `idma_batch_window`/`idma_run_window` | 实际测量窗口的轮询路径。 |
| `linux/drivers/misc/idma.c:838-845` | `status=readq(...); issued=readq(...); completed=readq(...)` | batch collect 也是 MMIO 读；若读不返回，drain/collect 无法结束。 |
| `linux/drivers/misc/idma.c:2654-2662` | run 尾部 capture/restore composition | 若 capture/restore 阻塞，SSH 侧看不到 CLI JSON/exit。 |

**软件 timeout 为什么不保证退出未返回的 MMIO/总线事务**：这些 timeout 都是“下一行代码有机会执行”时检查 `ktime`。`readq`/`writeq` 本身没有内核超时包装；`readq_poll_timeout` 也每轮先做 `readq`。如果 AXI/MMIO 读或写不返回，CPU 可能停在 `readq`/`writeq` 内，`idma_poll`、`idma_drain`、`idma_composition_*` 的 deadline 检查不会被执行。`idma_batch_start` 还在 `local_irq_save` 后读 `NEXT_ID`，若该读不返回，定时器中断也无法打断该路径。以上是机制候选/待证据，不是本 run 的根因结论。

### 4.2 给定 RTL：被动观测/控制路径存在无超时等待点

| 位置 | 证据 | 含义/候选 |
|---|---|---|
| `dma_composition_control.sv:5-8` | 模块只暴露 mode/protected DID/clear pulse，对 AXI/IOTLB 被动 | 本身不解决上游 MMIO 不返回。 |
| `dma_composition_control.sv:183-185` | `clear_o = rst_ni && cfg_write_i && index_i==2 && !cfg_busy && supported_i && cfg_data_legal && generation_q != all-ones` | clear 是组合条件脉冲；若条件不满足，不产生 clear。 |
| `dma_composition_control.sv:188-208` | config 写入更新 mode/DID/generation；busy/非法/溢出写 status | 无超时状态机，仅状态位。 |
| `dma_composition_control.sv:229-278` | `arm_i/open_i/close_i` 状态迁移 | 若无正确 arm/open/close，窗口不进入/不冻结；无硬件超时。 |
| `dma_composition_control.sv:280-419` | `S_ACTIVE` 下计 cycles/counters | 计数在窗口 active 时持续；无 abort timeout。 |
| `rv_iommu_nonblock_hit.sv:36` | `IDLE, DC, LEAF, OUT, REJECT` | 有限状态机，无超时状态。 |
| `rv_iommu_nonblock_hit.sv:47-50` | `start...; intercept_o = start || busy_o` | busy 时拦截，不是超时保护。 |
| `rv_iommu_nonblock_hit.sv:100-109` | `accept_o=...; response_valid_o = state_q==OUT` | 接受条件不满足会 REJECT/等待。 |
| `rv_iommu_nonblock_hit.sv:143-174` | FSM：`OUT: if(response_ready_i) state<=IDLE; REJECT: if(normal_fire_i || !candidate_valid_i) state<=IDLE` | `OUT` 等 `response_ready_i`，`REJECT` 等 `normal_fire_i` 或 candidate 撤除；均无超时。 |

### 4.3 候选与还需证据（不写成诊断）

- 候选 A：阻塞式 MMIO 读/写使软件 timeout 无法到达，例如 `idma_batch_start` 的 IRQ-off `NEXT_ID`、`idma_poll/drain` 的 DONE 读、composition capture/restore 读。还需：JTAG halt 位置、PC/backtrace、MMIO 地址、总线/PLIC 状态、板端 dmesg、是否有 CPU 存活证据。
- 候选 B：SSH helper/transport 300s 超时先发生，CLI 15s timeout 不代表 CLI 已超时返回。还需：TCP22 诊断、ping/ARP、en11 计数、SSH verbose、板端进程列表、串口是否仍输出。
- 候选 C：`rv_iommu_nonblock_hit` 或 `dma_composition_control` 内部等待点无超时。还需：可读 raw/状态位、ILA/JTAG 后现场、失败时 RTL 状态。
- 候选 D：composition configure/capture/restore 的 quiet 或 config 状态未达，导致后续读取不完整。还需：失败 case 的 raw[5]、raw[6]、raw[9] 等；本 run 未取得。
- 目前缺失：失败 case CLI JSON、stderr、exit.txt、boot.after、taint.after、dmesg、UART 失败时日志、SSH/TCP22 结果。重新插 UART 不能补回插线之前日志。

## 5. RESULTS.md 草案

> 若保存到 run 目录：`iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/RESULTS.md`，下列相对链接成立。

```markdown
# composition route-opt net3 172：停止在第 53 项

状态：`stopped`。本轮**不是正式 172 通过**，也不是收益或机制结论。

## 计数与阶段

- planned 172；attempted 53；completed 52；failed 1；not run 119。
- short 16/16 技术完成；noise 12/12 技术完成；explore 24/144 完成，失败 `explore-b1-g1000-a8-mpn`。
- 证据：[summary.json](summary.json)、[manifest.json](manifest.json)、[plan.json](plan.json)、[boot.json](boot.json)、[STOPPED.txt](STOPPED.txt)。

## 配置覆盖

- short：mode 0/1/2/3 × a_pages 0/1/8/16，transfers 16，warmup 16，v_gap 0。
- noise：mode S，a_pages 8，pairs 1..6，slots 0/1，transfers 4096，warmup 256，v_gap 0。
- explore 完成：b1-g0 a0/a1/a8/a16 四 mode；b1-g1000 a0 四 mode；b1-g1000 a1 四 mode。
- 失败：`cases/explore-b1-g1000-a8-mpn/`。失败 meta：[meta.json](cases/explore-b1-g1000-a8-mpn/meta.json)。
- 未运行：失败时点之后全部计划项，共 119。

## 失败记录

- `STOPPED.txt` 记录 SSH 密码命令超时、SSH transport rc=255、CLI stdout 非 JSON、`boot id changed: None`。
- 失败 case 未取得 CLI JSON、exit、boot.after、taint.after；`boot id changed: None` 不是重启证据。
- CLI 参数 timeout=15s；SSH helper 现场超时 300s。两者不是同一层计时。
- Mac en11 carrier active，ping 2/0；UART 已接回，旧正确 115200 参数下被动 3s 与换行后 8s 均无字节；新 TCP22 诊断待回。
- 未证实链路故障，未证实 RTL 死锁。

## 边界

- 未完成探索不得写成收益。
- 科学阴性（fast==0、miss==0）不是技术失败；本轮停止记录属于技术/传输失败类记录缺失。
- 恢复状态待确认，Codex 收到现场后按实际事实更新。
```

## 6. 文档增量草案

### 6.1 `CURRENT_STATUS.md` 顶部旧运行段落替换

替换当前第 3 行整段：

```markdown
**2026-10-05 纯网口 172 当前在探索第 53 项停止；未完成，不能称为正式通过。** 新 route-opt BIT 已通过最终时序并 JTAG 加载 1 次，SD 写 0 次；net3 启动 UUID `29d710b2-4949-402d-8055-8734cbaa5220`。纯 SSH 一次运行中 short 16、noise 12 技术完成，explore 完成 24；`explore-b1-g1000-a8-mpn` 失败，剩余 119 项未运行。`summary.json`/`STOPPED.txt` 显示 SSH transport 超时，未取得该失败的 CLI JSON、exit、boot.after/taint.after；`boot id changed: None` 不是重启证据；CLI 参数 timeout=15s，SSH helper 超时 300s。Mac en11 carrier active、ping 2/0；UART 已接回，旧正确 115200 参数下被动 3s 与换行后 8s 均无字节，新 TCP22 诊断待回。链路故障或 RTL 死锁均未证实；未运行项不得写入收益。恢复状态待确认，Codex 收到现场后按实际事实更新。[当前证据](iommu-platform/out/composition-board-net3-20261005/execution-state.json) · [run summary](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/summary.json)。
```

### 6.2 `HANDOFF.md` 追加

插入到 `# 论文平台交接` 后、现有最新段之前：

```markdown
## 2026-10-05 纯网口 172 第 53 项 SSH transport 超时停止（恢复状态待确认）

- 运行：attach-only，run `c226ee7a78b70c46`，planned 172，attempted 53，completed 52，failed 1，not run 119，status stopped，updated_at 2026-10-05T00:31:30Z。证据：[summary.json](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/summary.json)、[STOPPED.txt](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/STOPPED.txt)。
- 阶段/覆盖：short 16/16 技术完成；noise 12/12 技术完成；explore 24/144 完成，失败 `explore-b1-g1000-a8-mpn`；其余 119 未运行。配置覆盖见 [plan.json](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/plan.json)。
- 环境：新 route-opt BIT 最终时序通过；JTAG 1、SD 0；net3 boot `29d710b2-4949-402d-8055-8734cbaa5220`，uname `6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`，taint 0。证据：[boot.json](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/boot.json)。
- 失败记录：`STOPPED.txt` 为 SSH 密码命令超时、SSH transport rc=255、CLI stdout 非 JSON、`boot id changed: None`；失败 case meta 无 CLI JSON/exit/boot.after/taint.after。CLI timeout=15s；SSH helper 现场超时=300s。`boot id changed: None` 不是重启证据。
- 现场/未决：Mac en11 carrier active、ping 2/0；UART 已接回，旧正确 115200 参数下被动 3s 与换行后 8s 均无字节；新 TCP22 诊断待回。未证实链路故障，未证实 RTL 死锁。重新插 UART 不能补回插线之前日志。
- 边界：本轮不是正式 172 通过，不是收益；未运行项不得当结果。恢复状态待确认，Codex 收到现场后机械更新。
```

### 6.3 `validation.md` 追加

插入到 `# 验证记录（2026-09-08）` 后、现有最新段之前：

```markdown
## 2026-10-05 纯网口 172 停止在第 53 项：回传口径核对（非新板测）

- 范围：仅核对已回传 run 文件；本轮未执行板卡、SSH、JTAG、串口或重跑。
- 计数：`summary.json` 为 attempted 53、completed 52、failed 1、planned 172、not run 119、status stopped、through explore、run `c226ee7a78b70c46`。阶段为 short 16/16、noise 12/12、explore 24/144 完成。
- 配置覆盖：short 为 mode 0/1/2/3 × a_pages 0/1/8/16；noise 为 mode S × a_pages 8 × pairs 1..6 × slots 0/1；explore 完成为 b1-g0 四 mode×四页数、b1-g1000-a0 四 mode、b1-g1000-a1 四 mode。失败为 `explore-b1-g1000-a8-mpn`，之后 119 未运行。
- 失败记录：`STOPPED.txt` 记录 SSH 密码命令超时、SSH transport rc=255、CLI stdout 非 JSON、`boot id changed: None`；失败 case [meta.json](../out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/cases/explore-b1-g1000-a8-mpn/meta.json) 中 exit/framed_exit/boot_after/taint_after 均为 null。CLI 参数 timeout=15s；SSH helper 现场超时=300s；`boot id changed: None` 不是重启证据。
- 环境：新 route-opt BIT 已通过最终时序并实际 JTAG 加载 1 次，SD 写 0 次；net3 boot `29d710b2-4949-402d-8055-8734cbaa5220`、uname `6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`、taint 0。见 [boot.json](../out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/boot.json)。
- 未决：Mac en11 carrier active、ping 2/0；UART 已接回，旧正确 115200 参数下被动 3s 与换行后 8s 均无字节；新 TCP22 诊断待回。未证实链路故障，未证实 RTL 死锁。重新插 UART 不能补回插线之前日志。
- 边界：以上是停止现场与缺失项记录，不是 172 完成验收，不是收益或机制结论；恢复状态待确认。
```

### 6.4 `composition-v1-mac-20261004.md` 当前状态替换

替换 `### 当前硬件与网络` 下现有 bullets（约当前文档第 7–12 行）：

```markdown
### 当前硬件与网络

- 用户已确认使用 03:06 时序通过的 `tools/composition-trim-20261005/route-opt/ariane_xilinx.bit`；实际 JTAG 加载 1 次，SD 写 0 次。
- 加载后 net3 从原 SD 启动：`uname -r = 6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`，`boot_id = 29d710b2-4949-402d-8055-8734cbaa5220`。
- 纯 SSH 一次运行中，planned 172；short 16、noise 12 技术完成，explore 完成 24；`explore-b1-g1000-a8-mpn` 在 SSH transport 超时后停止，未取得该失败 CLI JSON/exit/boot.after，剩余 119 未运行。CLI timeout=15s；SSH helper 超时=300s；`boot id changed: None` 不是重启证据。不是正式 172 通过或收益。
- 用户已接回 UART；旧正确 115200 参数下被动 3 秒与换行后 8 秒均无字节；Mac en11 carrier active、ping 2/0；新 TCP22 诊断待回。未确认链路故障或 RTL 死锁。
- JTAG 保留；恢复状态待确认，Codex 收到现场后按实际事实更新。重新插 UART 不能补回插线之前日志。
```

## 7. 假设、未决、下一个关键源码

- 假设：上述计数以已回传 `summary.json` 为汇总口径；未重新运行验证逻辑。失败 case 无有效 record，`boot id changed: None` 来自失败后读取失败，不等价于实际 boot 比较。
- 未决：失败瞬间板端 Linux/SSH/TCP22 状态、CLI 进程状态、dmesg、UART 输出、bus/MMIO 状态、JTAG halt 位置、RTL 状态、失败 case raw 是否可事后读取。恢复状态待确认。
- 下一个关键源码：
  - `iommu-platform/linux/drivers/misc/idma.c`：重点看 `522-568`、`659-661`、`743-744`、`2269-2286`、`2296-2320`、`2329-2335`、`2415-2421`、`838-845`、`2654-2662`。
  - `iommu-platform/cva6/corev_apu/fpga/src/dma_composition_control.sv`：重点看 `183-208`、`229-278`、`280-419`。
  - `iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv`：重点看 `36`、`47-50`、`100-109`、`143-174`。
  - `iommu-platform/scripts/composition_plan.py`：确认计划覆盖与 timeout 参数，不重跑已有验证。
- 明确边界：链路故障、RTL 死锁、MMIO 卡死均只是候选；本轮证据只能证明“run 在第 53 项停止，后续未运行，失败 case 记录不完整”。