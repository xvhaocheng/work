# E2 第6点内核softlockup：原始失败与候选调度修复

状态：诊断中；无新轮、无重试替换、尚未证明timer/softirq根因。

实际原件为`board/runs/e2-device-scaling-20261017-01/uart/command-fa2f354e990b4f75a77692b55a203131.json`，
N4_A8、seq6、boot2f31b9f4-cf88-4061-b33c-3cf5a5592795，原DS0 BIT/acece p0-multi内核。
TAINT0→16384，watchdog报告CPU0 idma_scaling软锁死23秒。collector停止，原manifest failed/recorded_cases5。
CLI虽status0、prepared/start/stopped/drained/verified masks15、所有data_errors/quarantine0，仍不接受该点。

V硬件13105262 cycles=262.10524ms，软件victim_elapsed23.01355944s，硬件done_to_read1136467216 cycles=22.72934432s。
三A实际各完成约450k个COPY，反映V已完成后CPU收尾延后；不能把该软件23秒当作V硬件延迟。
真实旧vmlinux定位EPC `__do_softirq+0xac`，位于local_irq_enable后首条指令；这不足以单独裁定timer livelock或驱动轮询为根因。
只读诊断确认IOMMU IRQ计数0、timer21082、TIMER softirq4601/RCU6544，进程已退出，内核仍tainted。

配置为PREEMPT_NONE/HZ250/NO_HZ_IDLE，softlockup detector开启、watchdog_thresh10。
原multi批量轮询每1024轮才cond_resched。Root选定一个有界合作调度修复候选：
只对multi的warmup和测量未完成轮每轮cond_resched，所有multi设备数同策略；
先处理V完成再yield，旧pair/P0/pages/workload保持原策略。不开sleep、不改HZ/PREEMPT/watchdog、
不放宽taint，不改RTL/UAPI/预算/STOP/排空/数据/守恒。

A2先提交实际driver小差异和finite+RV64证据，Root审后才编新共同软件；
若只读证据明确否定其作用应先报告。新板轮须新源码/boot/manifest，原5个合格点与失败尾保留为旧版本独立证据。
正式新实验仍要求taint前后0和所有原检查，不为继续矩阵关闭检测或删除慢点。

## 第二轮候选实测失败（2026-10-17T15:48:04.863602+00:00）

新E2轮 `e2-device-scaling-20261018-02` 于15:41:06Z失败：Linux65b427/full-e3-v1、原DS0，前5点taint0接受；第6点N4_A8再次softlockup并host timeout。调度候选不足以修复。新5点和原失败尾已独立冻结 `E2-device-scaling/checkpoints/e2-prefix-failed02-{snapshot,paper}`（5841行17列，partial、各已测cell n=1），不与旧5点或后续轮混合。旧E4依赖job随E2失败停止，未加载硬件。

Root在旧命令结束后一次CTRL-C观察到shell prompt，仅做两组只读/proc诊断，exit0；timer_list显示nr_hangs10836、nr_retries32547，短恢复窗口不再增长，尚不证明DMA quiet或根因。原字节在board/runs/e2-softlockup-console02。当前唯一板动作 `e4-e5-common-v2-load02-launch`：JTAG重载已合格E5 d934 BIT并启动同一full-e3-v1内核；先验新boot/taint0，再独立E420+E5 hist。无需再次写SD。E2暂停性能采集，独立Codex席只读诊断timer/CSR/SBI路径。


新轮命令exit_code=null/error=timeout；collector AssertionError: host command status。第6点不接受，不以硬件时间替代失败试次。新partial包含5个实际试次，不是完成矩阵。只读诊断值是故障后的累计证据，不能还原每次handler耗时或确定AXI拥塞根因。后续原因判断由独立irq-diagnosis证据报告和Root直接源码审查给出。
