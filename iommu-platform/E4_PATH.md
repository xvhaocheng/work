# E4 分段路径观测：上板入口

2026-09-16。承接 DS50/50→0/0→50/50 回切，代码、定向验证及软件/FPGA构建完成后，实板已完成3次短验收和30次正式定位，33次由服务器独立复核通过。分段定位目标达到，详见[结果分析](docs/path-results-review.md)及[E3下一步计划](docs/e3-shared-service-plan.md)。以下保留该版本的口径与操作说明，不要求重新跑已完成矩阵。
只增加被动观测与读出，DS50/50、4KiB硬件批量供给和CPU整批轮询保持。
目标是定位 A8 相对 A1 增加的等待边界；本版本不证明 HOL 或纯 PTW 代价。
构建状态及精确版本见 `out/e4-path-20260916/delivery/README.md` 和构建清单。
两项构建均退出0、manifest passed，BIT/MCS非空；构建源码顶层a2e8475、
CVA6 a1572926、Linux45bf76b15e36。旧BIT、旧软件与结果保留。

## 四段含义

| stage | 开始→结束 | 解释边界 |
|---|---|---|
| 0 入口 | DMA私有地址首次VALID→IOMMU TR地址握手 | 包含私有接口反压、mux及中间缓冲经过时间 |
| 1 内部 | TR地址握手→对应COMP译后地址首次VALID | 包含队列、翻译和出队，不能直接叫纯翻译时间 |
| 2 出口 | COMP地址首次VALID→COMP地址握手 | 下游地址反压 |
| 3 后段 | COMP地址握手→COMP RLAST/B握手 | 数据及返回路径；写包含W供给，不能全称DDR服务时间 |

四DMA私有AXI ID固定0；mux高位保留源DMA。按源ID、读写通道和同ID顺序匹配。
观测输出仅进入MMIO，不驱动DMA/IOMMU/AXI的ready/valid或仲裁。
每个角色、通道、阶段容纳4笔在途；当前页对齐4KiB每任务两个AXI burst，
每路最多一个任务，此容量留有余量。实际超出容量、未匹配返回、意外私有ID、
计数/时间溢出或非空闲ARM均置错误，不能将溢出值当有效数据。
从ARM到窗口结束必须小于2^32周期（50MHz约85.9秒），超时自动标无效。
本次短测/正式实验使用10秒/60秒超时，不用于超长批次。

## 窗口与字段

沿用v2窗口：首项V被后级接受的边沿之后开启，计入末项V完成边沿并冻结。
ARM前软件排空预热。A先启动，因此可在V窗口打开时已有事务。
窗口结束后的CPU完成发现、停止A和排空不进入路径统计。

每个seq保留原三条 `idma_batch_obs`，另加16条 `idma_path_obs`：
V/A × r/w × stage0～3。所有时间单位为50MHz周期（20ns）。

| 字段 | 定义 |
|---|---|
| entered | 此阶段在窗口内开始的事务数 |
| full | 此阶段开始和结束均在窗口内的事务数 |
| carry | 开始于窗口前、结束于窗口内的事务数 |
| sum / max | 仅full集合的总经过周期 / 最大经过周期 |
| start / end | 窗口首尾仍在此阶段的事务数 |
| area | 窗口内每拍该阶段在途数量的积分；包含跨边界事务的窗口内部分 |
| valid | 驱动综合判定；错误结果完整保留，命令失败 |

每段必须满足 `start + entered = full + carry + end`。
相邻段满足前段 `full+carry = 后段entered`。V必须start=end=carry=0、
每段full等于相应V AR/AW计数、area=sum。Solo的A所有字段为0。
V每通道当前预期2N笔，N=8/64/4096时为16/128/8192，板上独立核对。
A各阶段的full集合可不同，不能直接把它们的平均时间拼成端到端均值。
读写和事务之间会重叠，**累计sum或area不能相加当整批时间或损失百分比**。
stage3结束于COMP接口，A的返回可能尚未到达私有接口；不要将其直接等同旧axi_a返回计数。

原H/gap、done_to_read、AXI占用、HPM和JSON保持。HPM仍为非原子软件快照，
与硬件窗口不同。单段均值为sum/full（full=0输出空值），最大值只是该样本极值。
比较各条件的分段分布、V总窗口和miss模式；不预设增加必在内部。

## 版本与软件

新observer magic `0x49444d414f425303`。0x58～0x78 MMIO不变；
ARM bit26开启path（必须同时bit25 batch），错误高32位bit6表示路径无效。
索引0～34保持；64～191共128项：`64 + lane*32 + stage*8 + field`，
lane=V读/V写/A读/A写，field按entered/full/carry/sum/max/start/end/area排列。

新Linux默认 `idma.path_observe=Y`：批量必须v3，遇旧v2 BIT在提交前明确报错。
配套内核版本为 `6.5.0-rc2-00083-g45bf76b15e36`。
旧批量兼容时可显式关闭 `/sys/module/idma/parameters/path_observe`，但关闭后的
结果不能作为本次分段实验。普通非批量模式仍可使用v1/v2/v3。
CLI/UAPI未改，仍用 `--batch` / `idma.batch.v1`；path通过dmesg输出，JSON本身无新增字段。
每次输出在排空、数据校验和HPM释放之后，不向被测窗口加入打印。

## Mac上板顺序

同时更新新BIT及SD的fw_payload.bin，按照已有JTAG/SD流程启动；本机没有FPGA。
先保存完整UART，检查新内核版本和 `path_observe=Y`，暂时降低控制台printk级别，
避免串口实时打印缺字；试次后从dmesg读取完整结果并在结束时恢复原级别。

先固定V=DMA0，无预热，连续两次8项Solo：

```sh
platform-timeout 120 idma_experiment --batch --device /dev/idma0 \
  --aggressor none --victim-pages 1 --chunk 4096 \
  --transfers 8 --warmup 0 --timeout-ms 10000
rc=$?
echo "path exit=$rc"
dmesg | grep -E 'idma_batch_obs|idma_path_obs' | tail -19
```

再固定V=DMA0/A=DMA1，64项A8、无预热：

```sh
platform-timeout 120 idma_experiment --batch --device /dev/idma0 \
  --aggressor 1 --victim-pages 1 --aggressor-pages 8 --chunk 4096 \
  --transfers 64 --warmup 0 --timeout-ms 10000
rc=$?
echo "path exit=$rc"
dmesg | grep -E 'idma_batch_obs|idma_path_obs' | tail -19
```

命令失败立即停止，保留完整dmesg而不是只取tail。每次必须是同seq的3+16条完整行，
time及16条path均valid=1、status=4、任务/AXI/分段守恒通过，数据错误/quarantine/taint为0。
旧脚本若只保存三条observer必须改为保存全部19条；不要丢掉path行。

三次短测通过后，固定角色Solo/A1/A8各10次，N4096/warmup256/timeout60000ms，
轮换条件顺序并逐次保存命令、JSON、19条观测、退出码和版本。
V miss=0/2均保留，不补采挑选。先确认计数可信且历史慢化仍在，再看等待在哪段增加；
若性能分布变了，先检查新增观测影响/条件，不能直接接着做E3。

## 主机验证与边界

`tests/test_dma_path.py`运行真实RTL定向事件：四端口/读写/重复ARM、同ID四笔在途、
零周期同拍、入口及出口反压、A跨首尾、冻结、ID/顺序/容量错误，以及缩位溢出。
旧observer和frontend联测保持通过；驱动真实函数验证缺v3、路径守恒/计数/area错误和carry合法情形。
这些是RTL事件模型与主机MMIO模型，不是完整IOMMU仿真或板上验收。
构建成功只代表产物生成；按用户要求不审查成功日志/时序报告，原时钟/CDC/IO审查待办保留。
