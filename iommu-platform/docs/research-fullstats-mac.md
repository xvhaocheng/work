# Mac 交接：2026-10-02 fullstats 全量搬运分组统计上板

## 一、范围与角色

本稿是 fullstats v1 交付包在 Mac 上板主机上的实操说明。Dell 负责源码、内核/驱动/加载器编译、离线分析和交付暂存；FPGA 板始终在 Mac 上，Dell 不接板，也不把板子移到 Dell 执行。Mac 负责 FPGA、SD/SPI、UART 控制台和 JTAG。软件及 FPGA 完整构建、主机测试已通过，交付包已核对；新板测尚未执行（0/21）。版本与哈希见 SHA256SUMS、DELIVERY.json。

## 二、冻结配置

- 硬件：CVA6 0dc5e333。
- profile 与既有研究一致：50MHz，DS50/50。
- Linux 预期内核：6.5.0-rc2-00092-g5853e425caf8（以 DELIVERY.json 和板上 uname 核对）。
- fullstats 原始索引 D00 magic：IDMAAGG1（0x49444d4141474731）。
- 本轮必须同时使用新的 ariane_xilinx.bit 和新的 fw_payload.bin。只换其中之一不能认为 fullstats 已部署。

## 三、交付物

交付目录 out/research-fullstats-20261002/delivery 应包含：
- ariane_xilinx.bit，新的 fullstats FPGA BIT。
- ariane_xilinx.mcs。
- fw_payload.bin，新的 Linux/软件 payload。
- SHA256SUMS。
- DELIVERY.json，记录 provenance。
- platform-research，runner 可执行文件/脚本。
- research-plan.tsv，冻结 84 行计划。
- analysis/research_results.py。
- analysis/research_plan.py。

实际文件哈希只引用 SHA256SUMS 和 DELIVERY.json；本文不写任何未由交付物计算的哈希。

## 四、执行纪律

- `platform-research cost-fullstats NEWOUTDIR` 会自动先跑 3 个短 cost 用例，再跑 18 个主 cost 用例。不要额外启动第 22 个 CLI 试验，不要在冻结 21 项之外手工补测。
- `--require-fullstats` 是必需 flag，由本包 runner 自动追加到每个 CLI 调用；操作者不要手工补 flag，也不要让 argv 记录与实际调用不一致。本包已通过21项筛选、实参记录和首错停止的主机测试。
- NEWOUTDIR 必须不存在，建议 /tmp/research-fullstats-01。
- 首错即停；不自动重跑、不剔除、不删除，保留现场。旧 E2 不重跑；旧 84 不重跑；不与旧结果合并；新 Solo 不与旧 non-Solo 拼接减速比例。
- 初次上板结果不是方法 M 收益。
- 本轮不裁剪 Linux，不另改 bootrom、SD 分区布局或实验 RTL，不引入新的 JTAG 启动流程。
- dmesg 和 taint 原样保留。taint gate 要求 0 是 runner 合同，不等于无 kernel warning；不得写成 warning-free 或 taint-clean 承诺。

## 五、Mac 上板步骤

1. 确认此刻没有实验在运行，记录使用的串口与加载方式。不要 halt、reset 或写内存打断正在运行的 Linux。
2. 在交付目录执行 `shasum -a 256 -c SHA256SUMS`。
3. 按既有 Mac SD/SPI 流程安装本包。现有流程是旧说明；不要假设物理 SD 已准备。先确认卡、镜像和目标，再安装新的 fw_payload.bin 并配置新的 ariane_xilinx.bit。两者都必需；若断电丢失配置，按既有方式恢复同一个新 BIT。
4. 启动后确认内核为 6.5.0-rc2-00092-g5853e425caf8。保存 fresh boot 的 uname、dmesg 和 taint 原样记录。
5. 只执行一次：
   `platform-research cost-fullstats /tmp/research-fullstats-01`
   该命令应自动完成 3 个短用例和 18 个主用例。短为 64 transfers、warmup 8；主为 4096 transfers、warmup 256；每次 4KiB；V=DMA0，A=DMA1；gap 由冻结计划给出。
6. 回传整个 /tmp/research-fullstats-01、UART 原始日志，以及本次 fresh boot、kernel、BIT/payload 哈希、argv、cases、dmesg、退出码、frozen plan 副本。若有停止，保留 STOPPED.txt。不要丢弃原始文件。

全部在线通过应为 cases=21 total=21 final_exit=0；仍需离线核查。

## 六、fullstats 统计口径

### 窗口与清零

- 聚合提交事件严格等于 `win_act && a_done_ok && lane_in_window_q[1]`，不再带每 16 次抽样相位。使用同一边沿的 `cur_a_main_n/comp_n/dur_n/req_n/miss_n`。
- 完成边沿在 close 上计入；close 之后完成不计入；窗口外开始的搬运不计入。issue 处于 opening edge 沿用原 `win_act_issue` 定义；统计周期仍为 (open, close]。
- 硬复位、acc_clear 及合法 p4m_arm 清零聚合。合法 arm 为 `p4m_arm_i && mode_q==MODE_COST && quiet_i && !cost_active_q && e2_phase_q==E2_IDLE`。清零优先于提交。非法 arm/clear 不得擦除已有聚合证据；ABORT/close 冻结已有结果，之后仅合法 clear/arm 清零。
- 聚合器不能驱动 DMA、IOMMU、ready、仲裁、控制状态或中断，只接收观察信号并提供只读数据。新错误通过独立聚合错误字报告，不改变现有错误位定义。

### 分组与字段

- 六组：组0..4 为 requests=4 且 IOTLB misses 分别为 0..4；组5 为其他所有组合，不得静默丢弃。
- 每组记录 count、sum_main、sum_complete、sum_duration、sum_req、sum_miss、main/complete/duration 的 min/max。
- main 和 complete 沿用现有互斥状态计数；duration 是整个搬运区间；三者不得混称。请求数是 select 事件；miss 是 IOTLB lookup miss；没有 DDT/PDT/PTW 级别类型细分，属于 limited req/miss type coverage，不宣称完整类型加权基线已被覆盖。
- count 和各 sum 为 64 位无符号；min/max 为 32 位。空组所有字段为 0。溢出可回卷但必须 sticky 报错；任何非零错误使整次 run 不可作有效统计。每组累计单次提交、无延迟遗漏，连续边沿事件也要正确。
- 守恒：sum_main+sum_complete<=sum_duration，sum_miss<=sum_req；逐 copy 也检查相同关系。均值=sum/count，不能从这些字段推导中位数或分位数。组5 仍列出实际计数和范围，不虚构固定 req/miss 键。

### 本地 raw 索引

- D00 magic IDMAAGG1。
- D01 version 1。
- D02 groups 6。
- D03 stride 16 words。
- D04 sticky errors：bit0 count overflow；bit1 sum overflow；bit2 single-copy miss>req；bit3 single-copy main+complete>duration；其余 0。
- D05 independent submission total，必须等于 C14 eligible 以及所有组 count 之和。
- D06..D0F reserved 0。
- 组 g 基址 0xD10+g*16：0=count；1=sum_main；2=sum_complete；3=sum_duration；4=sum_req；5=sum_miss；6={max_main,min_main}；7={max_complete,min_complete}；8={max_duration,min_duration}；9..15 reserved 0。
- 未映射地址读 0；配置写不改变聚合内容。

## 七、离线分析：新版本独立目录

命令形状：

```
INPUTDIR=/绝对路径/research-fullstats-01
NEW_ANALYSIS_DIR=/绝对路径/analysis-fullstats-01
[ ! -e "$NEW_ANALYSIS_DIR" ] || exit 2
python3 analysis/research_results.py cost-fullstats "$INPUTDIR" --output "$NEW_ANALYSIS_DIR"
```

目录结构要求：

- INPUTDIR 是 Mac 回传的整个 runner 输出目录。应包含 plan.copy.tsv、plan.data.tsv、plan.selected.tsv、params.txt、argv.tsv、cases.tsv、taint.txt、uname.before/after、dmesg.raw/err，以及每个 `<case>.json`、`<case>.err`、`<case>.exit`、`<case>.cmd`、`<case>.dmesg.before`、`<case>.dmesg.after`；若停止还含 STOPPED.txt。
- NEW_ANALYSIS_DIR 必须是新目录、位于 INPUTDIR 之外，不得与旧 84/E2 结果或旧版本合入。

分析行为要求：

- fullstats 模式期望 21 项并强制全量字段；旧模式仍兼容历史 JSON。
- 报告全量均值/范围与旧抽样均值/范围、分组覆盖数；明确抽样中位与总体均值不是同一量。没有原始全量记录就不宣称总体中位。
- short 与 main、各次 run、软件版本、BIT 版本分开；不能把组内搬运当板级重复。
- 输出为 `results.json` 和 `report.zh.txt`；不要手工覆盖 INPUTDIR。分析时必须记录脚本版本、argv 和退出码。

## 八、回传清单

- 整个 runner 输出目录。
- UART 原始日志。
- fresh boot 日志、kernel 版本、BIT/payload 哈希（引用 SHA256SUMS/DELIVERY.json）。
- argv、cases、dmesg、frozen plan 副本、原始退出码。
- 若停止，STOPPED.txt 和完整现场。
- 不丢弃、不裁剪、不重跑。主机通过不代表板测通过。

## 九、本轮不做

- 不裁剪 Linux。
- 不另改 bootrom、SD 分区布局或实验 RTL。
- 不跑完整 84 例。
- 不重跑旧 E2。
- 不把旧 nonSolo 与新结果拼接 slowdown。
- 不引入新 JTAG 启动流程。
- 不把初次上板结果当方法 M 收益。
