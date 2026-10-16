# §31 自然多 DMA 写序实验

2026-10-08。两真实 DMA 同 bank，无强制逆读/交错；13 项运行 6 PASS、7 watchdog。**工程写计数溢出已经与读写顺序等待环区分；新分段调度尚未实现或证明创新。**

- [完整审定与下一步](../../docs/natural-write-order-boundary-20261008.md)
- [运行清单、命令/日志/轨迹、退出码及行号](runs.json)
- [构建、源码/二进制哈希、DeepSeek 用量与限制](stage-evidence.json)
- [排除计数溢出后的依赖见证](dependency-witness.json)
- [逐字轨迹比较](trace-equivalence.json)
- [运行前 Codex 独立设计与 C0 风险假设](codex-experiment.md)
- [副本写容量限制补丁](write-limit.patch)、[修改后的副本](trans_ctl_write_limit.sv)
- [生产源实验前](production-before.json)／[实验后](production-after.json)：76 个哈希不变

原计数器短任务 natural-{v,a}-2048 均通过，长任务 natural-{v,a}-32768 均 overflow/watchdog。副本限制容量后 limit-{v,a}-2048 通过、limit-{v,a}-32768 仍因写序等待失败；queue-v-32768 仅增加失败队列快照，轨迹与 limit-v-32768 完全相同。普通 CONF0 的 coupled-{v,a}-32768 仍停滞，普通 512 拍缓冲的 full-{v,a}-32768 都通过。每一项均保留 `*-command.json`、`*-result.json`、`.log`、`.csv`。

3 个完整构建：build-natural、build-write-limit、build-full-buffer；2 个 C++ 重链接：natural_queue_test-build、natural_coupled_test-build。均有命令、结果和日志，全部 exit0；没有重复旧完整回归或上板。首轮生成器 self-check 的一次笔误失败及 DeepSeek 修订记录另存，不当作 DUT 构建结果。

DeepSeek 两次完整任务/答案分别为 [worker-task.md](worker-task.md)／[worker-answer.md](worker-answer.md)／[worker-raw.json](worker-raw.json)，以及 [revision-task.md](revision-task.md)／[revision-answer.md](revision-answer.md)／[revision-raw.json](revision-raw.json)。其驱动草案由 Codex 应用，修订日志 [worker-revision.patch.log](worker-revision.patch.log)，最终副本 [natural_test.cpp](natural_test.cpp)／[system_top.sv](system_top.sv)。软件队列快照与普通 CONF0 两个派生 C++、计数器副本为 Codex 在关键诊断/验收阶段修改。

两路 BufferDepth3 合计 48 B、512 合计 8192 B，只统计 iDMA byte FIFO；没有包括公共缓冲、cuts、metadata 或综合面积。测试 port0/1 同 bank 不等于生产四路/混合512+3；单序列行为 DDR 不等于 MIG。新场景 CONF1 非对齐长任务不能解释旧 PN 结果。生产修正仍未应用，新机制/异常保证未实现。
