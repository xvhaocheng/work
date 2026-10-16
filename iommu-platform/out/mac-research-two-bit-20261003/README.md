# 两 BIT 实验原始记录（2026-10-03启动，2026-10-04 JST完成）

共526项：e2 64项，method 462项。采集进程退出0，COMPLETE.json记录全部试次；两段stage-done.json保留各段完成状态。内核6.5.0-rc2-00093-gc0f5e13ea81f，来源DELIVERY.json。

- e2/、method/：逐项原始JSON、stdout/stderr、退出码、dmesg、taint、argv、归档及原runner生成文件；计划、启动和加载记录完整保留。
- console.raw.log、console.events.log及run日志：完整采集过程；e2-console-prefix.raw.log为先行回传时的前缀快照，最终以完整console.raw.log为准。
- host-tools/：本次主机登录和打包兼容适配器及交付runner来源。
- STOPPED.initial-transfer.txt与恢复记录：首项CLI执行exit0后tar-z兼容错误；只恢复原首项采集，没有重跑。原首次现场另保留于Dell initial-transfer-failure/，本地原目录20261003-research-two-bit/未修改。

Dell目标：/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/。回传核对状态见DELL_RECEIPT.md。

用户要求从现在起只保留原始数据，不做结果分析。没有新增统计/机制报告或独立验收；已经产生的runner文件原样保留。正常结束不等于已得出科学结论。FPGA Linux保持运行，UART已释放。
