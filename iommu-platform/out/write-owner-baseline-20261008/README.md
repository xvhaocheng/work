# §32 普通准确源读准入对照

2026-10-08。新普通源读门BUF3两启动顺序都完成32KiB错位任务，同CONF0大缓冲对照也两项完成；新运行4/PASS4。普通小缓冲写终点27894、大缓冲25146，非独立译址时间/稳态吞吐。解除停滞本身不作新贡献，I仍未实现。

- [完整结果与下一机制门槛](../../docs/write-owner-baseline-20261008.md)
- [四项命令/退出码、原日志/CSV与行号](runs.json)
- [构建、当前source/binary哈希、生产边界与worker用量](stage-evidence.json)
- [128次匹配当前virtualW的源读验收](gate-admission-audit.json)
- [Codex独立运行前规格](codex-baseline-spec.md)、[关键审定及worker推断更正](codex-review.md)
- [Codex新普通门](ordinary_copy_owner_gate.sv)、[独立top副本](owner_actual_dma_top.sv)、[接入差分](owner-top.patch)
- [观测printf派生](coupled-metric.patch)、[本轮写容量副本](trans_ctl_write_limit.sv)
- [生产76文件前](production-before.json)/[后](production-after.json)：哈希不变

build-owner为一次新SV+C++，full-coupled-relink只重链接旧obj-full-buffer；都有*-command.json、*.log、*-result.json且exit0。owner-{v,a}-32768与full-{v,a}-32768各有独立命令、日志、CSV、result。输入ptw-latency31/warmup0/S，无强制逆读/交错，无CPU/预热/batch/新返回服务。

DeepSeek [worker任务](worker-task.md)/[完整答复](worker-answer.md)/[raw](worker-raw.json)，[聚焦修订任务](revision-task.md)/[答复](revision-answer.md)/[raw](revision-raw.json)。[prepare_owner_baseline.py](prepare_owner_baseline.py)由DeepSeek草拟、Codex核实际code后执行，仅準备材料；[manifest.json](manifest.json)是准备快照。Codex随后仅给CONF0 driver加不推进clock的统计printf，最终哈希另见stage，不把manifest的not_run当最终状态。optional CONF1命令均未执行。

两路FIFO3=48B、512=8192B，不计公共FIFO/cuts、门的metadata/控制；未综合，不是LUT收益。仅两个constant-ID normal active job的同bank重分类和串行FIFO行为DDR，非生产四路混合512/3与MIG。生产/tests/Linux/BIT/SD/板卡均未改变。
