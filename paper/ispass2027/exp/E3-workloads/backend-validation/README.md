# P0：真实 iDMA backend 的有界 AXI 验证

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17
- Verification Status: VERIFIED（仅下述合法输入域的有限功能 RTL 执行）
- Version Label: p0_backend_legal13_v1

本目录只实例化冻结基线的真实 `idma_backend`、hardware legalizer、AXI transport、
byte buffer 与生产 FIFO。没有以 burst 公式替代 DUT，没有执行 FPGA/JTAG/UART，
没有修改生产 RTL。DUT 输入直接是 1D descriptor，未执行 frontend、driver、IOMMU 或 CPU。

验收入口为 [run03/summary.json](runs/run03/summary.json)：`status=passed` 的范围仅限
**64 / 1024 / 4096 字节**及下列边界/背压/排空场景，不是 backend 的无条件全面通过。
P0 collector 可从 `burst_per_descriptor` 读取真实测得的 `1 / 1 / 2`；不得沿用于其他大小或地址布局。

| 对齐 descriptor 字节数 | 实际 AR / AW | 实际 ARLEN / AWLEN | RLAST / B | R bytes / WSTRB bytes |
| ---: | ---: | --- | ---: | ---: |
| 64 | 1 / 1 | 7 / 7 | 1 / 1 | 64 / 64 |
| 1024 | 1 / 1 | 127 / 127 | 1 / 1 | 1024 / 1024 |
| 4096 | 2 / 2 | 255+255 / 255+255 | 2 / 2 | 4096 / 4096 |

64-bit bus、AXI 最大256 beats使 legalizer 的实际边界为2048字节。
固定源码见 run03/frozen/cva6 下 `corev_apu/iDMA/src/idma_legalizer.sv:74`、`:82`、`:144`、`:174`、`:326`、`:351`；
AXI=256 来源为 `idma_pkg.sv:107`。实际源码、源 SHA256、编译命令和运行命令由
[manifest](runs/run03/manifest.json)与 summary 关联，全部冻结输入与原DS0构建的已记录源哈希相同。

## 实际完成的 13 项

[完整事件](runs/run03/events.jsonl)保存每次 descriptor 握手、实际 AR/AW、R/W 数据、WSTRB、
RLAST、B 及 backend response。共 **87 个合法 descriptor，87 次完成**，均完整排空。
[独立事件复算](runs/run03/event-audit.json)逐字节对齐真实 R 与 W 事件，确认长度、内容、地址范围、
AR/AW LEN、RLAST/B 与 summary 一致；[各场景计数](runs/run03/case-counts.csv)可直接审阅。

- 三个对齐单 descriptor：64、1024、4096 字节。
- 64字节 packed ring 的64个槽、1024字节 ring 的4个槽，以及连续三个4KiB descriptor。
- 64字节从2048边界前32字节开始、1024字节从页内1536开始，均实际切成2个 burst；
  4096字节从页内1024开始，实际为3个 burst，不套用对齐 packed ring 的2-burst结论。
- 停止供给新请求后，7个已接受 descriptor 继续完成、排空。
- 三档各注入一次 R SLVERR 和一次 B SLVERR，所有握手、字节与完成守恒，数据及目标外 guard 均符合期望。

AR/AW/W/descriptor/response 在背压下检查 payload 保持，内存 slave 按实际 DUT 的 ARLEN/AWLEN
逐 beat 服务，没有由期望公式生成 DUT 请求。每场景最多50000周期，编译300秒、仿真30秒硬限。
实际使用 Verilator 4.038；此处周期来自确定性的功能测试 memory model，不是 FPGA 性能数据。

## 两项边界与零长度原失败

生产 `dma_core_wrap.sv:286` 是 **NO_ERROR_HANDLING**。因此上述 SLVERR 场景验证排空与守恒，
不会证明总线错误向软件传播：backend 的 `idma_rsp.error` 在这些合法请求中仍为0。
backend 没有 STOP 输入；此处停止供给测试不替代 A2 的 frontend/driver STOP 验证。

额外零长度负向检查发现原基线已有问题：零长度 request 可在 `rsp_ready=0` 时握手，
但组合 error response 会在下拍换成合法输入时消失。直接源码为
`idma_backend.sv:294–310`，原始 [run02/manifest.json](runs/run02/manifest.json) 保持 `failed`，
[stderr](runs/run02/stderr.log) 和 [events](runs/run02/events.jsonl) 完整保留。
run02先实际通过了13项合法输入，随后在额外零长度probe触发背压稳定性断言；没有清洗该失败。
run01是初次源文件路径校验失败（`fifo_v2`在deprecated目录），未启动编译，记录也保留。

Root明确接受分开报告，并要求P0三层拒绝零长度：
[upstream-guards.json](upstream-guards.json)保存直接核读的CLI、真实driver、批量RTL长度枚举及来源hash。
三者均只接受64/1024/4096（A固定4096）；该核读不冒充backend编译或A2测试执行。
新增 `known_exclusions` 把零长原失败链接进合格的 P0 summary。

## 复现

从工作树根执行，选择不存在的新输出目录；只编译/执行这套有限测试：

```sh
python3 paper/ispass2027/exp/E3-workloads/backend-validation/run_backend.py \
  --out paper/ispass2027/exp/E3-workloads/backend-validation/runs/reproduce01 \
  --negative-evidence paper/ispass2027/exp/E3-workloads/backend-validation/runs/run02/manifest.json
```

可单独重放已保存事件，不重跑 DUT：

```sh
python3 paper/ispass2027/exp/E3-workloads/backend-validation/audit_events.py \
  --summary paper/ispass2027/exp/E3-workloads/backend-validation/runs/run03/summary.json
```

完整历史 builder/driver 原字节位于各 run 的 `frozen/`，不覆盖。run03执行时的报告路径根初次少了
`paper/` 前缀；[summary.as-executed.json](runs/run03/summary.as-executed.json)和原manifest保持原样。
主summary的 `metadata_normalization` 仅补导航前缀，所有实测数值、DUT、命令和结果不变；
当前runner已修正工作树根，未来复现不需要该路径修正。

唯一测试包边界是常数 `ariane_pkg::FPGA_EN=0`，对应选用的cv64a6配置原值，不引入CPU全树。
vendor内部SVA与既有有限harness相同关闭，外部握手稳定性、bytes、内容和排空由C++及事件复算检查。
编译对象和可执行文件仅本机保留；代码、固定输入、实际回执与原始事件随分支提交。
