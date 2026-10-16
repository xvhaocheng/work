# P4M 启动控制协议（软件实现）

**状态**：软件全量构建已通过（`make software PROFILE=migration-4dma-p4m JOBS=12`，退出 0，2026-09-20 12:12–12:13 UTC）；板上未跑（`board_tested=false`），本文无板上结论。版本：内核 `6.5.0-rc2-00089-gefd9461d6459`，Linux `efd9461d6459049f59c9ea5466f3703e45413bb3`，顶层 `a08426f46720b646ae0fa052a0a27136a347b69f`，CVA6 `e822bc9d`、IOMMU `137f5e5`。沿用既有 BIT `7af1ccbc…`（无新 FPGA 编译），新 payload `fc8e8a73…`；交付位置 `out/p4m-start-control-20260920/delivery`。

## 0. 背景与改动

pilot-21 第三次 gap0 的开窗 A ID 为 1448（前两次为 4/4），说明开窗前 A 运行量不同，两路启动相位/预窗口历史混淆了同批比较（见 [p4m-pilot-results-review.md](docs/p4m-pilot-results-review.md)）。新软件让 legacy batch 与 P4M 共用同一 batch start 控制：A/V 先完成配置，再用短的非睡眠临界区罩住两次硬件 trigger 并读软件时间戳，随后立即恢复 IRQ/抢占。commit/poll/drain/log 与此前一样在全局互斥下执行，且不在新增的关中断/禁抢占区间内。CPU 轮询逻辑不变。

## 1. 运行范围

- 12 次 `idma_experiment`：先 4 次短试次全部通过，才允许 8 次比较试次（4 对）；失败即停，不自动重跑、不追加基准。
- 短试次（64 transfers、warmup 8、chunk 4096）：Solo / A1 gap0 / A8 gap0 / A8 gap1000。
- 比较试次（4096 transfers、warmup 256、chunk 4096）冻结顺序 `B P P B B P P B`：B = `--batch --aggressor 1 --aggressor-pages 8`；P = `--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0`。
- 板卡命令 `platform-p4m-start all /tmp/p4m-start-01`；输出目录须不存在。`all` 自动门强制先 short 后 compare；手动 compare 无法程序化确认同版本已先跑 short，需用户自行负责。

## 2. 启动记录 version=1

每次 batch 在窗口结束、排空、校验后输出恰好一行；只要输出该行即字段完整，preflight 可在输出前直接返回（不产生该行），不存在「字段缺省」记录。

```
idma_batch_start version=1 seq=.. method=irq_pair v_did=10 a_did=11|4294967295 attempted=.. has_a=.. a_triggered=.. v_triggered=.. a_id=.. v_id=.. a_begin_ns=.. a_end_ns=.. v_begin_ns=.. v_end_ns=.. irq_span_ns=.. a_to_v_ns=.. error=..
```

- 成功试次要求 `attempted=1`、`error=0`，`a_id`/`v_id` 严格非零；Solo 例外：`has_a=0`、`a_id=0`、`a_did=4294967295`。ID 读失败时 ID 记为 0，flags 如实反映。
- 时间戳为软件 trigger 括号：`a_begin_ns ≤ a_end_ns ≤ v_begin_ns ≤ v_end_ns`；`irq_span_ns = v_end_ns − (has_a ? a_begin_ns : v_begin_ns)`，`a_to_v_ns = has_a ? v_begin_ns − a_begin_ns : 0`。runner 校验 `irq_span_ns` 恒等式，不只看为正。

## 3. 窗口记录与 CLI JSON

- 同一 seq 恰好一条 `idma_batch_obs ... part=time ...`；runner 要求 `valid=1`、`status=0x4`、`issued=done=<transfers>`、`batch_polled=1`。
- CLI JSON 是嵌套结构：顶层不止 `success` 一个键，runner 只用顶层 `success` 校验；`result` 下有 `status`、`hpm_configured`、`measurement_started`、`measurement_complete`、`victim_did`、`aggressor_did`、`victim.*`、pair 的 `aggressor.*`；`--p4m` 另有 `p4m.{sequence,observer_status,v_issued,v_done}`。无顶层窗口字段。

## 4. runner 检查与证据

- 每例前后各取一次 `dmesg`：运行前不得出现该 seq 的 batch 时间记录，运行后必须恰好出现一条 `idma_batch_start`，且 `seq` 严格大于运行前最大值（已实现）；对应 `part=time` 必须存在。
- 其他门：`uname -r` 不得为旧 `00088`；taint 全程 0；任何 `--p4m` 前要求 `path_observe=Y`。
- 每例保留原始 JSON/stderr、`dmesg` before/raw、校验过的 `startup`/`batch_time` 等；失败保留全部样本。UART 与 stdout/stderr 由 Mac 侧捕获，Dell 未接板。

## 5. 历史 E3 诊断字段

E3 诊断通道 `idma_e3_diag_lane` 的 `launch_begin_ns`（= 配置写入起点，语义未变）与 `launch_end_ns` 保留原义；新增的 A/V 配置与 trigger 括号与其重叠，不是把旧字段重标为硬件时间戳。旧 pilot-21 的 21/21、gap500 反例与全部原始记录继续保留。

## 6. 读出、判据与不声称

| 项 | 内容 |
|---|---|
| 读出面 | A→V trigger 跨度及离散、开窗 A ID、V 窗口 cycles、同 block B/P 配对比值 |
| 判据 | 不预设阈值、不看数据后定 cutoff；旧 pilot n=3 与本次 4 对均只作描述 |
| 不声称 | 无 IRQ 延迟/中断根因结论，`irq_span_ns` 仅为软件下界；无新 FPGA 编译；正式参数未冻结 |

结果回来后：启动稳定但 gap0 差异仍在，先查两条路径的观测与控制语义；启动稳定且结果可比，再冻结少量 gap0/1000/2000 的正式实验参数与重复数。旧 gap500 反例继续保留。
