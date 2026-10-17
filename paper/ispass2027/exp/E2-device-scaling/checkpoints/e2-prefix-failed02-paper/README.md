# E2 第二轮：5个已接受试次（矩阵未完成）

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Verification Status: PARTIAL（FPGA实测，campaign failed，5/90接受）
- Version Label: e2-new-common-failed02-prefix

[paper.csv](paper.csv) 为17列统一长表，共5841行；[summary.json](summary.json)保存逐试次指标、实际n和未测N/A。分析出口passed只表示5点的原始数据绑定与导出通过，不表示原campaign成功。

DS0原硬件、50MHz，Linux `65b427946152e3aca27babab0a437821729805d4`，release `6.5.0-rc2-ispass2027-full-e3-v1`；实际硬件CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`，软件构建中的1d587只是reference。boot `5df16afb-ce70-4098-a71f-4ff462079c1e`、observer seq1–5、block0，各点taint0。完整身份和源命令见[analysis-manifest.json](analysis-manifest.json)及[不可变原始快照](../e2-prefix-failed02-snapshot/manifest.json)。

| 条件 | 实际n | V硬件时间(ms) | V吞吐(MiB/s) |
| --- | --- | --- | --- |
| N2_A1 | 1 | 107.65036 | 148.629 |
| N3_A1 | 1 | 159.72518 | 100.172 |
| N4_A1 | 1 | 212.49078 | 75.297 |
| N2_A8 | 1 | 119.34984 | 134.060 |
| N3_A8 | 1 | 192.74010 | 83.013 |
| N4_A8 | 0 | N/A | N/A |
| N2_A6 | 0 | N/A | N/A |
| N3_A3 | 0 | N/A | N/A |
| N4_A2 | 0 | N/A | N/A |

第6点N4_A8反复softlockup，命令timeout，未接受。新合作调度内核不足以解决问题，失败尾保留但不得用其硬件数字补成性能点。本包各已测cell只有n=1，不支持完整设备数趋势、显著性或总足迹对照结论；DMA2/3无PATH时仍为N/A。

这些是本次新boot的5个实际试次，与旧内核failed01的5点分开；不得混成n=2，也不得与本轮其他累计视图重复追加。后续实验另起身份和run。

Root已实际执行snapshot_prefix.py与export_prefix.py；工具再次核原顺序/命令/软件profile/source/数据/taint门，原记录未改写。
