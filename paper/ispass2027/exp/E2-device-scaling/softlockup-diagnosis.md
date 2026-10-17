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
