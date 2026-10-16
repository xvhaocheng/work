# E3：D0/D0 双轮前史诊断

2026-09-18：本轮只增加软件诊断，沿用 9 月 16 日的 observer v4、DS50/50 E3 BIT。构建及交付状态见 [交付说明](out/e3-diag-20260918/delivery/README.md)。新版还需上板验收。

## 为什么做这一步

上一轮正式 A8 没有一对两轮同时触发。更关键的是，第 8、9 对两轮都没有触发，第二轮仍快约 8 ms，A miss 从 6160 变成约 5136。因此不能把双轮耗时差归因于 D128。详见 [实板结果复核](docs/e3-results-review.md)。

现在两轮都用 D0，仍在同一映射生命周期内运行。保留分别预热、排空、核验和重新初始化缓冲的流程，观察实际发生了多少预热任务、结束在哪个页、启动两路相隔多久。目的是区分预热/启动前史与后续硬件状态的影响，不是再次测定 D128 的效果。

固定 V=DMA0/DID10、一页；A=DMA1/DID11、一页或八页；4 KiB 硬件接续，CPU 轮询整批。没有增加 IOTLB 清空操作，也没有统一硬件启动。FENCE.I 是本平台缓存同步手段，不是 IOTLB 重置。

## 更新和有限采样

沿用已上板的 `migration-4dma-e3` BIT，SHA256 `a99ce9565b82449ca5eb0a8afc18c8b79cdf9e23a5a4e9197aafae02b5df98d4`。只按既有 SD 原始分区流程更新本轮 `fw_payload.bin`。新工具已经内置 rootfs，不能只替换旧系统上的 CLI，因为需要新内核接口。

新参数 `--diagnostic 1` 固定 D0/D0；此时 `--first-delay 128` 会在发起实验前拒绝。未加诊断参数或使用 `--diagnostic 0` 时，仍是原 D0/D128 协议。原 ioctl 0x09 和 `idma.e3_pair.v1` 不变；诊断使用独立能力位、ioctl 0x0a 和 `idma.e3_diag.v1`。

先做三个短验收，每条命令含两轮：

```sh
idma_e3_pair --diagnostic 1 --aggressor none --transfers 8 --warmup 0
idma_e3_pair --diagnostic 1 --aggressor 1 --aggressor-pages 1 --transfers 64 --warmup 32
idma_e3_pair --diagnostic 1 --aggressor 1 --aggressor-pages 8 --transfers 64 --warmup 0
```

通过后只做六对诊断，顺序 A8、A8、A1、A8、A8、A1。每对各运行下面相应命令一次，不因无触发或异常前史补采：

```sh
# A8：共四对
idma_e3_pair --diagnostic 1 --aggressor 1 --aggressor-pages 8 --transfers 4096 --warmup 256
# A1：共两对，插在上述位置
idma_e3_pair --diagnostic 1 --aggressor 1 --aggressor-pages 1 --transfers 4096 --warmup 256
```

总计九对、十八子轮。程序只在两轮退出后向 stdout 输出一条 JSON；继续保存完整 UART、退出码、taint，以及按两条 sequence 分别收齐的每轮 3 条 batch + 16 条 path。采集器需要接受新 schema 和 `diagnostic` 对象，不能沿用“第二轮必须是互补 D”的检查。每轮 `delay` 和 `e3_words[3]` 都应是 0。无触发允许，真实错误立即停止，保留失败 JSON，不自动重试。

## 新字段怎么讲

`rounds[r].diagnostic` 属于第 r 轮；`lane[0]` 是 V，`lane[1]` 是 A。旧 HPM、软件耗时及 256 个 E3 原始字段仍保留。

| 字段 | 含义 |
|---|---|
| `warm_submitted` | 预热成功取得任务 ID 的提交数，不含测量批次 |
| `warm_completed` | 预热轮询期间已经观察到完成的数目 |
| `warm_drained` | 退出预热后，在排空中确认完成的任务数；正常每路最多 1 |
| `warm_in_flight` | 退出预热循环时软件尚未确认完成的任务；不代表硬件此刻一定仍忙 |
| `warm_last_offset` / `warm_next_offset` | 预热末次提交偏移 / 下一次待提交偏移，单位字节；零预热时为 0。批量配置随后会将起点重置到第 0 页 |
| `warm_hpm` | 两路按 DID 的预热 HPM 差值：active_cycles、iotlb_miss、s1_ptw、s2_ptw，包含末尾 A 排空 |
| `warm_begin_ns` / `warm_end_ns` | 预热快照之后、提交之前 / 预热循环退出之后的软件时间 |
| `warm_drained_ns` | 预热已排空、末快照之前的软件时间 |
| `warm_complete` | 预热成功排空且末快照完成；失败时不能把不完整字段当正常数据 |
| `arm_begin_ns` / `arm_end_ns` | 观测器 ARM 操作前后时间，ARM 的硬件生效时刻在区间内 |
| `launch_begin_ns` / `launch_end_ns` | 每路批量启动配置写入之前 / NEXT_ID 触发返回有效 ID 之后的时间 |

所有 `*_ns` 都是本次 Linux 启动内的单调软件时钟，不是硬件握手时间，不与 E3 的 ARM 相对周期直接相减。未到达的阶段为 0；失败数据须先看状态和完成标记。A absent 时 lane[1] 全零。`warm_hpm` 不是原子快照，计数口径沿用既有 HPM 边界。

正常每轮 V 的 `warm_submitted=warm_completed=warmup`；每个存在的 lane 都应有 `warm_submitted=warm_completed+warm_drained`。A 的提交量由实际完成/补发决定，不要求与 V 相同，也不要求两轮相同。工具会核查计数和时间顺序，但不会因两轮不同而判为执行失败。

最直接的启动指标是 `V.launch_begin_ns - A.launch_end_ns`，表示 A 提交调用返回到 V 开始配置的间隔；分别查看两个 launch 区间宽度，以识别长延迟落在哪段。真实触发的先后间隔只能由两个区间给出上下界，不能把区间端点当作精确发射点。ARM 到 V 启动的总间隔可帮助定位上轮出现的毫秒级启动异常，但不能据此指认具体中断。

## 怎么据结果决定下一步

先逐对列出两轮的 A 预热数量、末页、预热 HPM、ARM/启动区间、正式 V/A miss、E3 首查及硬件批次时间，再与 A1 控制对照。少量样本用于诊断，不做显著性结论。

- 若 A 的实际预热数量或末页不同，优先考虑有限且可复现的预热序列，再比较两轮。
- 若启动间隔明显波动，下一步考虑统一硬件启动或有限请求实验，不能简单将整批差值扣掉。
- 若上述外部记录接近而 miss/耗时仍不同，保留未观测 IOTLB/替换状态和硬件到达次序等候选解释；这些记录相同仍不等于内部状态相同。

诊断增加了计数写入、阶段时间戳和两次预热 HPM 快照，本身可能改变前史。该数据独立成组，不与旧版直接合并，也不称为无扰动复现。

当前 BIT 的触发仍只识别 AR 队头为 V，A 读占头时看不到后面的 V。本轮不修这一硬件覆盖限制。诊断结果回传后再结合前史问题修订触发识别/门槛观测和必要的启动控制，重新构建 BIT，最后恢复 D0/D128 因果对照。当前不进入缓解方法评估。
