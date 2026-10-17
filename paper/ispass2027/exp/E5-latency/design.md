# E5：被动分段延迟直方图的最小实现提案

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: plan（既定论文实验的接口设计，不恢复机制探索）
- Origin Date: 2026-10-17
- Verification Status: UNVERIFIED（提案；尚无 E5 RTL、构建或板测）
- Version Label: e5_passive_histogram_v1

本提案供 Root 审查后实施。独立工作树为 `/home/xvhaocheng/work-a1-e5-latency`，
顶层分支 `ispass/e5-latency`；不修改当前 DS 构建输入、P0 分支或板卡。

## 选择的基座

选择已冻结、已用于正式 DS0/DS50 基线的 **4KiB batch + path v3**，不在尚在接续的 P0 前端/驱动上叠加 E5。
这样 E5 只改变被动观测，不将新增传输大小接口混入延迟分布的版本差异。

| 层级 | 固定基座 |
| --- | --- |
| platform | `885ed0a7ea90ed42dae2b7a2dadfdd0acba15273` |
| CVA6 | `8aac3fcd345af7686a55af1867783fd6b91a0069` |
| IOMMU | `85b157ebe71a623e326e51fff7b7126fe77e8eb9`，保持原 child |
| Linux | `45bf76b15e36cf608dff95013d91f8bedb7b1b8e` |
| OpenSBI | `e8d21bfdc2dd753314627968e496c3a1188f6dfb` |

实际对照源来自原 DS0 导出副本与 software01 导出副本；组件修改时另建同名分支的独立 worktree，
不切活动组件或在运行的构建导出目录上编辑。平台 gitlink / lock 随已完成组件提交后更新。
第一套候选产物为 DS50/50 + histogram；若 Root 图表矩阵还需 DS0，再在同一来源下仅改变既有 DS overlay。
这不是新增机制、服务排序、缓存组织或 DMA 供给方式。

## 原文事件边界与采样总体

原 `dma_path_observer.sv:132–137` 已定义四 lane（V/A × read/write）和五个事件，E5 直接复用：

| 分段 | 进入 | 离开 | 直方图样本含义 |
| --- | --- | --- | --- |
| 0 | 被选 DMA 首次呈现 AR/AW valid（held-valid 仅计一次） | 共用 IOMMU TR 口 valid && ready，按 mux ID 选 DMA | 进入共用翻译口前的等待 |
| 1 | TR 握手 | IOMMU completion/request 输出首次 valid（held-valid 仅计一次） | 翻译及内部处理至输出请求呈现 |
| 2 | 输出首次 valid | 输出 valid && ready | 输出端请求背压 |
| 3 | 输出握手 | 同 ID 的 RLAST/B 握手 | 输出请求至该 AXI burst 返回完成 |

这里是 AXI read/write burst 的各段驻留周期，不是单个 DMA descriptor 的分位数，也不是单独 PTW 成本。
stage 0 的进入有意沿用“首次 valid”而非 DMA 侧 ready 握手；不得改口径。
现有每段 Depth=4、ID/private-ID=0 合同、时间戳32位、计数32位、sum48位都保持。

只有原 `dma_path_stage` 中 **`run_i && active_i && matched && whole && !bad`**
这条已使 `full_q` 加1的事件才进入直方图；年龄直接复用 `age`。
因此同拍 enter+leave 且原队列空时，年龄0有独立 bin；carry-in、carry-out 和未配对事件不伪造样本。
carry/start/end 原字段继续报告，A 的截断总体必须在分析中可见。
窗口启停仍由现有 ARM/first V issue/BATCH_READ 及 abort 决定，不追加观测周期。

## 最小 RTL 和 MMIO 接口

计划修改 CVA6 三处：`dma_path_observer.sv`、`dma_observer.sv`、`ariane_peripherals_xilinx.sv`。
IOMMU、iDMA backend/frontend、总线 ready/valid 驱动与约束不改。
新增 compile-time `HistEnable`，默认0；E5独立顶层显式启用1。

每段32个32位计数，共 **16 × 32 × 32 = 16384 bits（2KiB 计数存储）**。
这只是计数容量，实际 FF/LUT/布线开销须看综合/实现报告。
bin scheme 1 具体为：

- bin0 = 0 cycles；bin1 = 1 cycle；
- bin `i=2..30` = `[2^(i-1), 2^i-1]` cycles；
- bin31 = `[2^30, 2^32-1]` cycles（末桶显式较宽，不当作无限精度尾部）。

每 stage 每周期最多命中一桶，counter 自增只发生在 full 样本完成事件；没有逐样本 tracing FIFO。
计数上限 `2^32-1`；桶满则饱和并置粘性 overflow，不绕回。overflow 同时使现有 PATH validity 失败。
现有队列溢出、非法 return、计数溢出及整个 armed session 的32位 timer wrap仍走原错误路径；
不丢掉错误后继续声称直方图有效。clear/ARM 与现有统计同步清桶、清新 overflow。

MMIO仍使用现有 `MAGIC/CONTROL/TARGET/INDEX/DATA`（0x58/60/68/70/78），只将 INDEX 保存宽度由8扩至10位：

| INDEX | 语义 |
| --- | --- |
| 0..34 | 原有计时/AXI计数，保持字节和语义 |
| 64..191 | 原有16 × 8 PATH 字段，保持地址和语义 |
| 192 | HIST_INFO：bits7:0格式1、15:8桶数32、23:16桶计数宽32、31:24时间宽32、47:32每stage overflow mask；其余0 |
| 256..767 | `256 + lane*128 + stage*32 + bin`；DATA低32位是计数，其余0 |

四 lane 顺序仍为 V-read、V-write、A-read、A-write；stage0..3不变。
启用直方图的 MAGIC 为 `0x49444d414f425304`；HistEnable=0保留原v3/v2版本。
PATH原DATA口继续读原字段；新直方图DATA口是只读 mux，不控制任何 DMA/AXI 信号。
时间单位保持实际 observer 50MHz时钟：1 cycle=20ns。

## Linux与采集

Linux仅改 `drivers/misc/idma.c`：识别v4并记 `o->hist`，继续接受旧v3路径接口，
不改 ioctl/UAPI、工作负载或 CLI 参数。E5专用采集器要求v4及完整直方图，不能以旧BIT静默过关。

原 `idma_observation_finish()` 已在 drain/verify 后读冻结结果并打印，所有新MMIO读取和日志也只放在这里。
每次只读一段32桶，使用至多32个u64的临时数组，避免增加4KiB内核栈数组。
读HIST_INFO、核桶数/位宽/overflow、逐段检查 `sum(bins)==原PATH.full`；任何不符返回-EIO。
沿用同 sequence/boot/kernel 身份，新增16条：

```text
idma_path_hist seq=... role=v channel=r stage=0 valid=1 schema=1 bins=32 counts=0,1,...
```

每行32个十进制计数，最坏约0.5KiB，保留完整dmesg转储；旧16条PATH及batch日志格式不改。
新Linux image/BIT使用独立实验包、现有build/run manifest配对，新二进制本机保存不进Git。
采集仍由Root串行排队；同一Linux/BIT所需条件集中采完再换，A1不访问UART/JTAG。

输出是每试次/role/channel/stage/bin的实际计数、明确周期边界、full/carry/start/end、valid和overflow。
CDF只能由实际桶计数得到；分位数只能报告桶边界范围，不从均值推造p95/p99，也不能相加各段分位数。
跨试次/boot聚合必须显式记录权重和实际重复数，不把桶内交易数当成独立重启次数。

## 接受条件与有限验证

1. 原v3与新分支HistEnable=0逐周期对照：原35计时/AXI字段、16×8 PATH值及握手输入一致。
2. 全四段、读写/V/A、age0/1/2/3及每个二进制边界、同拍enter/leave、环形队列满时pop+push、
   carry-in/out、open/close边界、reset/clear/abort、ID/hold错误均有限覆盖；每段 `sum(bins)==full`。
3. 缩小HistogramCountWidth与TimeWidth的独立测试强制桶饱和、timer wrap和队列容量错误，确认粘性失效，
   不将错误图计作有效样本。生产宽度保持32/32。
4. 主机驱动读取/日志解析有限测试覆盖v3兼容、v4格式、缺桶/错序列/overflow/计数不守恒的错误停止；
   不以mock执行替代后续板测。
5. Linux/UAPI编译，CVA6定向检查，独立50MHz Vivado实现：WNS≥0，setup/hold/pulse失败端点及routing errors全0；
   计入真实新增资源。失败即停，不改时钟约束或实现策略。
6. 所有完成组件先提交，再落实平台gitlink与来源锁；交Root代码/原始报告/image/BIT后才排队上板。

当前未实施RTL、未建立新Linux/BIT，也没有E5性能数据。Root审查本基座、四段样本口径、32桶边界和MMIO布局后再实施。
