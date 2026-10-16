# 完整批次测量与普通服务判别结果

本轮已完成测量适配与登记的10个配置。在固定COPY、CPU关闭的行为DDR模型中，普通DS优先服务消除了P/PN的完成时间差额。因此，结束“以这项差额支持新增逐copy门控”的分支，不据此进入G1或扩展实验矩阵。共享地址翻译干扰的主问题仍保留；这次结果没有证明所有应用都已被普通方法解决。

## 判别结果

V、A各完成64次4KiB COPY，共同就绪时刻为双方配置后、首次NEXT_ID前的t0；双方gap为0，预热256次后排空，静态S1、S2关闭，PTW模型延迟31。A1、A8分别为A源/目的页环大小。单位为模型周期；T_V、T_A来自各自最后一次硬件done，T_return还计入必要协议收尾，离线数据比较与终态寄存器复核耗时不混入硬件完成时间。

| 配置 | T_V | T_A | T_return |
| --- | ---: | ---: | ---: |
| A1四种模式均相同 | 73539 | 72965 | 73544 |
| A8 S | 98544 | 98032 | 98553 |
| A8 P | 86594 | 86082 | 86601 |
| A8 N | 146084 | 145572 | 146089 |
| A8 PN | 90283 | 89771 | 90292 |
| A8 P加DS优先 | 76738 | 76164 | 76745 |
| A8 PN加DS优先 | 76738 | 76164 | 76745 |

原FIFO服务下，PN的V完成比P慢3689周期，即4.26%；DS优先使P和PN的两路完成时间及收尾时间完全一致。相对原P与原PN，V分别缩短11.38%和15.00%，A也同时更快。

![两路完整任务完成时间比较](../out/workflow-g0-m0-20261010/completion-comparison.svg)

P与PN在A8下各有31次慢路径未命中、93笔walk返回；PN另有14次快速响应，不经过同一慢路径lookup口径。原FIFO下walk活跃周期为16483和20172，差值也是3689；DS优先后两者均为13485。A1测量段没有walk，四模式时间相同。这组相邻负例与同规则干预支持：本模型的P/PN反向差额受共享返回服务顺序影响，普通规则已经消除了它。

N与S的未命中和实际walk工作量不同，不能将其差额套用为同一个因果；本轮也不自动将它转成新选题。

原始依据：[10配置结果](../out/workflow-g0-m0-20261010/results.json)、[完整结果表](../out/workflow-g0-m0-20261010/results-table.csv)、[轨迹匹配统计](../out/workflow-g0-m0-20261010/trace-statistics.json)、[根裁决](../out/workflow-g0-m0-20261010/root-result-acceptance.json)。

## 测量验收和实现

完整批次模式独立于旧V窗口，正常路径等待双方自然完成、不STOP A；首发供给等待计入t0。每路逻辑ready由共同t0及上一笔硬件done重建，不冒称新增硬件ready信号。SYSTEM拓扑保持，新增测试用CPU使能，默认开启以保持旧行为。DS优先只修改已有C++服务选择规则的生效区间：从t0至V第64个done，保留同ID顺序与held VALID。

全部成功记录均核对每路64个issue和64个done、真实有效读写各262144字节、128个AW及对应B响应、寄存器完成计数、最终数据、quiet及队列守恒。10配置的2560个issue/done时间戳还与独立CSV记录逐项一致。

验收覆盖A先结束、V先结束、已结束通道不误触watchdog、CPU开启/关闭、首发延迟128周期计费、旧N模式回归。极小watchdog负测按预期失败且无成功记录；独立校验器拒绝14种损坏记录，9种非法参数被拒，实际watchdog判定函数的4项边界测试通过。基本M0记录与A8 N完全同参，直接复用，没有重复仿真。

修改范围是测试C++、SYSTEM测试顶层、Python运行入口、新增结果校验器和离线分析脚本。实际完成两种Verilator构建；生产RTL/Linux未改，没有FPGA完整构建或板测。共16次仿真进程，其中15次正常通过、1次预期watchdog失败；10配置与验收用例有一次复用，不能把它们相加当作独立重复样本。

依据：[M0验收](../out/workflow-g0-m0-20261010/m0-acceptance.json)、[执行账本](../out/workflow-g0-m0-20261010/execution-ledger.json)、[时间戳交叉核验](../out/workflow-g0-m0-20261010/trace-timestamp-crosscheck.json)、[任务开始时源码到实际修改的差异](../out/workflow-g0-m0-20261010/implemented.patch)。

复现单项完整批次：

```sh
python3 tests/test_nonblock_actual_dma.py --system --mode p --full-batch --cpu-enable 0
# 普通服务干预加 --ds-priority；相邻负例加 --a-pages 1。
python3 tests/test_full_batch_contract.py <运行日志> --trace <同次CSV轨迹>
python3 scripts/analyze_full_batch_g0.py --input-dir out/workflow-g0-m0-20261010
```

绘图需要matplotlib；本轮隔离安装环境及版本记录在结果目录的plot-env和plot-requirements.txt。未改变项目生产依赖。

## 研究裁决和边界

这轮实验不支持以“PN组合后变慢”直接推出新增门控的必要性，因为一个独立普通服务规则已消除了本合同中的差额。保留完整批次夹具，归档这项立题理由；后续如继续研究，须重新提出有应用动机的具体失效，并给出它与普通服务排序的差异，不自动恢复旧候选。

DS优先在本轮是行为DDR模型中的可执行干预，并未综合硬件或测量其资源与时序成本；不能把15.00%写成已实现的硬件收益，也不能据此解释旧实板全部差额。微基准采用固定映射和CPU关闭背景，尚未覆盖大页、提前准备、全部旧反馈或真实应用合同。每个配置为确定性单次执行，不报告统计显著性或p99。

问题与对照在实现前登记：[失效卡](../out/workflow-g0-m0-20261010/workflow-failure-card.md)、[对照登记](../out/workflow-g0-m0-20261010/baseline-register.json)。

DeepSeek提供实现与修订提案、校验器、离线脚本及报告草稿；Codex负责关键源码与研究裁决、提案审查和必要修复、实际应用及全部执行验收。[完整调用记录](../out/workflow-g0-m0-20261010/worker-ledger.json)保留提案中被纠正的问题，不以模型意见代替验证。
