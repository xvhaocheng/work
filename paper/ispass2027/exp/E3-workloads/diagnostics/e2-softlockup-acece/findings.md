# 旧acece E2 softlockup只读核查

原始失败尾由Root保留在`board/runs/e2-device-scaling-20261017-01/uart/command-fa2f354e990b4f75a77692b55a203131.json`。
本目录`derived.json`绑定原JSON SHA；`raw-excerpts.txt`标原stdout行号，未修改Root原件或将失败点转成paper记录。
实际旧Linux为acece，配置为PREEMPT_NONE/HZ250，SOFTLOCKUP_DETECTOR开启；P0原内核、Image、CLI未改变。

## 能由原件确定的边界

- stdout5／298行报告CPU0 soft lockup，PID173/idma_scaling；EPC为`__do_softirq+0xac`。
  stdout321行seq6的硬件窗口13105262cycles=262.10524ms，done_to_read1136467216cycles=22729.34432ms。
  CLI的CPU软件间隔23013.55944ms；减去这两项仍有22.10988ms。
  因而主要额外时间在硬件V完成到CPU首次读取完成状态之间；不能归因于后续verify或打印开销。
- CLI仍status0，prepared/stopped/drained/verified masks为15，但TAINT从0变16384。
  `source/kernel/watchdog.c:530`明确设置TAINT_SOFTLOCKUP；这是正式数据拒绝条件，CLI data/drain通过不覆盖它。
- A三lane仍连续供给，最后完成451521/450239/450219个COPY；不能把它们整段软件bracket量当成262ms V硬件窗内吞吐。
- `source/drivers/misc/idma.c:382`先读DONE，385才记CPU完成时间；batch loop667先poll V，671成功立即返回，
  674–675仅每1024个未完成循环调用cond_resched。因此旧poll策略是循环数门，不是时间延迟上界。
- `source/include/linux/sched.h:2099`非抢占内核走__cond_resched；
  `source/kernel/sched/core.c:8564`仅should_resched(0)时真正调度，否则仅按需report RCU quiescence。
  cond_resched请求不等于强制sleep或保证已执行一次context switch。

## 不能由这份栈确定的原因

`source/kernel/softirq.c:537`local_irq_enable在539的softirq_vec设置之前；栈仅说明计时IRQ到达时CPU处于该位置，
不证明CPU此前整23s都停在此处，也不证明某个softirq callback独占了23s。
`source/drivers/clocksource/timer-riscv.c:122`在timer IRQ清IE_TIE后调用event_handler；34–48重新编程clockevent。
`source/kernel/watchdog.c:455–460`通过cpu_stop work喂soft watchdog，499–530检测延迟并加taint。
这些调用链说明有需要及时执行的调度工作，不能据此断定timer、MMIO、softirq或poll任一项是已证根因。
watchdog报告时间本身使用`running_clock()>>30`近似秒，不能当精确性能计时。

## Root锁定的单个候选

基于46df仅修合作调度点：multi batch每个未完成loop请求cond_resched；CPU warmup helper新增显式bool，
multi调用true，旧pair/P0/pages/workload wrapper false，仍每1024。N2/N3/N4一致。
始终先poll/处理V完成；成功最终poll直接返回，未增加sleep、轮询计数、MMIO、HZ/PREEMPT、watchdog、RTL/UAPI、
预算、cache或STOP/quiet改变。这只修合作调度边界，不宣称softirq/timer根因已证或板上故障已消除。
后续必须新source/boot/manifest与taint0正式采集；旧5点纯前缀与失败尾保持各自身份，不合入新轮。
