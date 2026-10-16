# P4M 启动控制协议（draft）

**状态：协议草案。本轮只冻结运行时协议与日志契约，软件构建状态 pending，尚未在板上运行，没有任何新的构建或板上证据。本文不构成性能结论。**

## 范围

- 只允许跑固定 12 次 `idma_experiment`：先 4 次 short，再 8 次 compare；compare 仅在 short 全部通过后执行。
- short（64 transfers、warmup 8、4 KiB、`platform-timeout 120`、`--timeout-ms 60000`）：
  1. Solo `--p4m --aggressor none --a-gap-cycles 0`
  2. A1 gap0 `--p4m --aggressor 1 --aggressor-pages 1 --a-gap-cycles 0`
  3. A8 gap0 `--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0`
  4. A8 gap1000 `--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000`（仅工程检查）
- compare（4096 transfers、warmup 256、4 KiB），冻结顺序 `B8 P8g0 P8g0 B8 B8 P8g0 P8g0 B8`：
  - B8 = `--batch --aggressor 1 --aggressor-pages 8`（legacy batch，无 P4M bank）
  - P8g0 = `--p4m --aggressor 1 --aggressor-pages 8 --a-gap-cycles 0`
- 无重试、无额外 benchmark、不写 FPGA/SD、不清 dmesg；首个失败即停，产物全部保留。
- 默认输出目录 `/tmp/p4m-start-$$`；实际目录由用户指定，且必须不存在。

## 启动记录契约（软件实现）

每条普通 batch 尝试在窗口结束、drain 与校验之后，向 dmesg 输出**一行**：

```
idma_batch_start version=1 seq=<observer sequence> method=irq_pair v_did=10 a_did=11|4294967295 attempted=1 has_a=.. a_triggered=.. v_triggered=1 a_begin_ns=.. a_end_ns=.. v_begin_ns=.. v_end_ns=.. irq_span_ns=.. a_to_v_ns=.. error=0
```

- `method=irq_pair`：只在受保护的“先 A 后 V + 软件时间戳读”区间使用；普通非 batch 路径不会伪称 irq_pair。
- `attempted=1`：进入了触发区间。
- `a_triggered`：表示**发起过 A 侧 NEXT_ID 读**（读尝试）。读回 0 也仍算尝试过；只在 `has_a=1` 的配置下才有意义。
- solo（`--aggressor none`）：`has_a=0`、`a_triggered=0`、`a_did=4294967295`、`a_begin_ns=0`、`a_end_ns=0`、`a_to_v_ns=0`，不伪造 A 字段。
- `error`：本次会话最终错误号；0 表示本行对应的尝试没有错误。
- A/V 的四个时间戳是**软件触发读点的括号**：`a_begin_ns <= a_end_ns <= v_begin_ns <= v_end_ns`，`a_to_v_ns = v_begin_ns - a_begin_ns`。它们**不是硬件 issue 时刻**，也不是中断延迟的度量。
- `irq_span_ns` 是受保护区间软件括号的**下界**，只保证为正，不当作硬件时序精度证据。
- runner 容忍未知的额外字段（例如 `a_id`/`v_id`）；若出现，必须是**非零整数**。

## runner 检查什么

- 每个 case 在命令前后各取一次 dmesg：新记录必须**恰好一条**且为本次新增（计数法）；缺失、陈旧或复用同一 `seq` 一律拒绝。
- 新记录字段齐全、前缀为 `idma_batch_start version=1 seq=`、`method=irq_pair`、`attempted=1`、`v_triggered=1`、`v_did=10`、`error=0`、整数域可解析、时间戳有序、`irq_span_ns>0`；solo 与 A 的差异按上表。
- CLI JSON（窄口径、只读顶层标量）：`success=true`、`batch_polled=1`、V issued/done 等于本次 transfers（64 或 4096）；`--p4m` 追加 `time_valid=1`、`status=0x4`；B8 无 P4M bank，不要求 P4M 窗口字段。
- **A 打开 ID（opening ID）与 A 进度只记录、不判定**：打开 ID 与预期不同、以及 A 进度为 0，都不算失败。原始 JSON 原样保留并在 UART 打印，打开 ID 可从其中读取。
- 其它门：`uname -r` 不能是已知旧版 `00088`（新 revision hash 不硬编码，只记录 `uname -r`/`uname -a`）；`path_observe=Y`（B8 只记录观测值，因为它本来就强制走观察路径）；taint 在 preflight/per-case/结束时都必须为 0。

## 证据与人工选择

- 每个 case 保留：原始 JSON、原始 stderr、`<case>.dmesg.before`、`<case>.dmesg.raw`、校验通过的 `<case>.startup`；另有 `argv.txt`、`input.txt`、`matrix.txt`、`params.txt`、`path_observe.txt`，以及收尾的 `dmesg.raw`、`taint.txt`。
- stdout 打印原始 CLI JSON（UART 可见），控制行与 dmesg 走 stderr，由主机侧 UART 抓取保存；失败也保留全部样本，不做挑选。

## 不声称什么

- 这不是正式 P4M 结论：新版软件需要自己的 gap0 基线；legacy batch 的 A 窗口仍然未知；A/V 启动间隔只是软件触发括号内的观测量，不能直接当作硬件 issue 间隔。
- gap500 仍保留在旧 pilot-21 记录中，本轮不重复，也不据此给结论。
- 没有新的 FPGA 编译、没有 RTL 时序 signoff、没有性能收益或退化的判定。
