# P4M 基线说明（组会讲解版）

**状态：P4M-1 实现完成；板上短验收四例功能通过（Solo/A1gap0/A8gap0/A8gap1000，n=1），不证明正式收益；复核脚本修订中。** 证据见 [短验收复核](docs/p4m-short-results-review.md)、[console.raw.log](out/mac-p4m-short-20260920-180713-run-l7cin11d/console.raw.log)、[review.json](out/p4m-short-review-20260920/review.json)；修订后复核结果 {{REVIEW_RESULT}}（待实际运行填充）。配套版本为 observer v7 / Linux `6.5.0-rc2-00088-g9f8da9e7a6a0`；主机/RTL 构建证据见 [验证记录](out/p4m-baseline-20260920/validation.json) 和 [构建状态](out/p4m-baseline-20260920/build-status.json)。

## 一句话

P4M 在同一个硬件 V 窗口 `(open, close]` 内，数 A 完成了多少个完整 4 KiB copy、边界上还有多少在途，再和 V 的耗时/等待放在一起画曲线；先确认 gap0 自对照，再决定是否值得做复杂解耦。

## 访问模式

- V 有 1 个源页和 1 个目的页，即 source/dest 各一个页映射，共 2 个页映射。
- A1 有 2 个页映射，A8 有 16 个页映射；这里只说明工作集/映射规模，**不单独把容量断言为干扰原因**。
- 两路都按页号环回 `i % pages`，顺序做完整 4 KiB copy；A 的下一次描述符间隔由硬件在上一整个 copy 完成并 drain 之后定步，不由 CPU 每请求轮询定步。
- 固定角色：V = DMA0/DID10，A = DMA1/DID11；DS 50/50、50 MHz、4 DMA、IOTLB 16。

## 窗口语义

- 硬件窗口 `A=(open, close]`；开边界事件不计入 issue/done 汇总，开边界在途在事件之后采样；关边界事件计入汇总，关边界在途在事件之后采样。
- V 的 issued/done 计数包含首个被测任务。
- `start + issue = done + end`；`carry_done <= start`、`carry_done <= done`、`carry_done <= 1`。
- A done 可以为 0，仍然是合法窗口；`a_started_before_window_completed` 是在 `open` 之前发出、但在窗口内完成的 A copy，单独报告，上界为 1 个 job / 4096 bytes。
- flags bit2 是**初始不可变**的 strict carry 标记；`carry_done` 只把这个初始在途 job 的完成计一次。
- 每 lane 的 sticky error 会一直保持到硬件 reset；CLEAR 和 ARM 不会清掉它。

## 寄存器与字段

- gap MMIO：写 `d0`，格式 low32 为 `gap_cycles`，full64-bit strobe，拒绝 upper bits。gap 配置写只在 IDLE 时被接受，配置后在 active run 期间固定不变；引擎 active 时写 gap 属于非法控制写，会被拒绝并置 sticky error。只有受控 launch 序列中被拒绝的写不产生 poison（不污染 batch_error）。capability 读 `d8`，magic `0x49444d4147415001`。
- 精确额外周期：非最终 batch 后端完成边 `n`，gap0 时下一任务在 `n+1` 提供；gap>0 时在 `n+1+gap` 提供。也就是相对 gap0 精确增加 `gap` 个周期，且首份 copy 不延迟。只有 WAIT 非最终 batch 在 `gap>0` 进入 GAP；`gap=0` 仍走原 WAIT->COUNT。
- P4M observer bank `448..463`（16 words）：

| index | 含义 |
|---|---|
| 448 | P4M capability magic `0x49444d4150344d01` |
| 449 | 窗口周期数 cycles，同现有 count0 |
| 450 | 窗口内 A issue events |
| 451 | 窗口内 A done events |
| 452 | 开边界任务在途 0/1 |
| 453 | 关边界任务在途 0/1 |
| 454 | strict carry cohort 的完成次数（最多 1） |
| 455 | 开边界 A issue 0/1 |
| 456 | 开边界在途 ID 或 0 |
| 457 | 关边界在途 ID 或 0 |
| 458 | 窗口内首个 A done ID 或 0 |
| 459 | 窗口内最后 A done ID 或 0 |
| 460 | task/error mask |
| 461 | V issued |
| 462 | V done |
| 463 | flags: bit0 hasA, bit1 closed, bit2 strict pre-opening initial carry |

- Solo 时 450..460 为零（error 除外），flags 无 hasA；closed 有效。
- 溢出/错误 sticky；主状态 bit40（error_q[8]）会使 P4M 无效；bank error word 460/12 可读。无效或 aborted 结果绝不当作 successful。

## 指标口径

- `cycles * 20 ns`：固定 50 MHz 平台假设；不是每次板级校准结果。
- `A_progress_window = A_completed_jobs_window * 4096`：窗口内完整 A copy 数乘 chunk；**不是** AXI/总线字节数。
- 主速率取 JSON 中的 `a_completed_bytes_per_second`（浮点值，= 窗口内完成 copy 的字节数 / 硬件窗口时长）；它只表示完成字节数速率，**不是**实际总线吞吐。
- `result.aggressor` 是整轮 A 统计，包含启动与最终 drain；必须与窗口内 A 统计分开。
- HPM 软件快照窗口单独标记，不能直接当成 P4M 硬件窗口。

## 基线要求

- 必须使用**同版本 v7 自身的 gap0 自对照**；不能把旧 V 周期基线直接拼成新版本结论。
- 选择性 PTW 128 干预在连续性能基线里关闭，避免把人工放大当成自然收益。
- gap 点与重复数尚未冻结，由预试决定；本卡不编造正式参数。

## 板上命令

```
platform-p4m short /tmp/p4m-v7-01
```

短验收 4 次，每次 64 transfers、warmup 8、4 KiB：

1. Solo：`--aggressor none --a-gap-cycles 0`
2. A1 gap0：`--aggressor 1 --aggressor-pages 1 --a-gap-cycles 0`
3. A8 gap0：`--aggressor 1 --aggressor-pages 8 --a-gap-cycles 0`
4. A8 gap1000：`--aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000`（仅工程检查，不是正式 gap 点）

脚本保留 JSON、原始文件与 taint；原始 UART 输出由脚本打印、主机侧捕获保存；错误、quarantine、timeout 或缺少命令时停止。必须搭配同一轮 P4M BIT 与 payload；SD 更新沿用既有原始分区流程，本文件不发明偏移。

## 边界

- 软件与 FPGA 构建均已通过 exit 0；新版尚未上板。
- 时钟/CDC/外部 IO 审查沿用既有边界，本轮没有新增 timing signoff。
- 成功构建只证明构建；主机模型不等于板卡验收。
- 正式 gap 点与重复数未冻结；不把短验收或 gap1000 工程检查写成正式结论。
