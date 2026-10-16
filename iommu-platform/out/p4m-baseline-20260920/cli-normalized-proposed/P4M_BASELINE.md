# P4M 基线说明

**状态：板上尚未运行；构建状态待定；本文件不编造版本、哈希或结果。**

本文件说明 P4M 共同窗口测量的访问模式、读法与短验收命令。它不主张已测量收益，也不承诺“小改动”“创新”或“有效果”。

## 访问模式

- V 使用 1 页源/目的；A1 使用 1 页，A8 使用 8 页；IOTLB 16 项。
- 短验收保持既有轮询方式。A 描述符间固定间隔由硬件在上一整个 copy 完成并 drain 之后插入，**不由 CPU 每请求轮询定步**。
- `gap = 0` 不引入额外等待，也不改变原握手与任务顺序。
- 本卡只做短验收：Solo、A1、A8 gap0，加一次 A8 gap1000 工程检查；不是正式 gap 网格。
- 后续正式预试默认 4096 transfers、256 warmup，但本卡不自动运行完整 4096/256。

## 窗口语义

- 硬件窗口为 `(open, close]`。
- 开边界事件不计入 issue/done 汇总；开边界在途采样发生在事件之后。
- 关边界事件计入汇总；关边界在途采样发生在事件之后。
- 在 `open` 之前发出、但在窗口内完成的 A copy 单独报告为 `a_done_before_open`（`window[6]`），不当作窗口内启动的进展。
- 守恒关系：`start + issue = done + end`；`carry done <= start`、`<= done`、`<= 1`。
- Solo 时 A 相关字段为零，flags 无 `hasA`，`closed` 有效。

## 基线要求

- 必须使用**同版本 v7 自身的 gap0 自对照**；不能把旧 V 周期基线直接拼成新版本结论。
- 选择性 PTW 128 干预在连续性能基线里关闭，避免把人工放大当成自然收益。
- 正式比较前先冻结 gap 点与重复数；本卡不编造这些参数。

## 指标

- `cycles * 20 ns`：固定 50 MHz 平台假设；不是每次板级校准结果。
- `A_progress_window = A_completed_jobs_window * 4096`：窗口内完整 A copy 数乘 chunk；**不是** AXI/总线字节数。
- `A_completed_rate` 使用窗口内 done jobs / 硬件窗口时长；**不是**总线吞吐。
- `result.aggressor` 是整轮 A 统计，包含启动与最终 drain；必须与窗口内 A 统计分开。
- HPM 软件快照窗口单独标记，不能直接当成 P4M 硬件窗口。

## 板上命令

```
platform-p4m short [OUTPUT_DIR]
```

短验收 4 次，每次 64 transfers、warmup 8、4 KiB：

1. Solo：`--aggressor none --a-gap-cycles 0`
2. A1 gap0：`--aggressor 1 --aggressor-pages 1 --a-gap-cycles 0`
3. A8 gap0：`--aggressor 1 --aggressor-pages 8 --a-gap-cycles 0`
4. A8 gap1000：`--aggressor 1 --aggressor-pages 8 --a-gap-cycles 1000`

脚本保留 JSON、stderr、dmesg 原始输出与 taint；错误、quarantine、timeout 或缺少命令时停止。
