# E1b DS25/DS100配套实验包

两档构建及DS25→DS100全部100次采集均通过，每档五条件各10次，无失败、重试或补采。
[执行报告](../../ds-sweep.md)、[manifest](manifest.json)、[四档中位数/IQR](../../../data/ds-sweep-summary.md)关联实际结果。
旧DS0/DS50的100次结果留在[原包](../baseline100-20261017/README.md)，不覆盖、不重测。

| BIT | WNS/WHS/WPWS (ns) | setup/hold/pulse失败端点 | routing errors | delayer单元 |
| --- | --- | --- | --- | --- |
| DS25，50MHz | +0.177/+0.048/+0.062 | 0/0/0 | 0 | 120 |
| DS100，50MHz | +0.177/+0.045/+0.062 | 0/0/0 | 0 | 145 |

- [source.json](source.json)：同一固定源码commit及本轮诊断延迟overlay。
- [profiles](metadata/profiles.json)：DS0/25/50/100配置；DS25、DS100除延迟外与DS50相同。
- [实际构建入口快照](metadata/build_baseline.as-dispatched.py)：每档输出独立build manifest和日志。
- [离线接口检查](metadata/host-interface-validation.json)：仅替身检查轮转、标签、顺序与失败停止，无硬件操作。
- `artifacts/`本机保存新BIT及共用pathv3 payload/Image；二进制不进Git。

本轮复用已写入SD的payload，不重写、不回读SD。按DS25全部50次→DS100全部50次执行，
每档五条件各10次，失败保留并停止；不以旧G1性能阈值筛选本轮样本。

两次boot为`0a1ae461-d2a5-4b43-8580-4544f313e058`（DS25）、
`5cc1560a-b07e-4956-b1f2-e95e3cced13b`（DS100）。
原始[DS25 records](../../runs/ds-sweep-20261017-01/ds25-all/records.json)、
[DS100 records](../../runs/ds-sweep-20261017-01/ds100-all/records.json)、
[campaign](../../runs/ds-sweep-20261017-01/campaign.json)及[有限任务返回0](../../runs/ds-sweep-20261017-01-launch/result.json)均保留。
`results`链接同一原始目录，不另复制或改写原始记录。
结束时保持DS100/原pathv3并[释放UART](../../runs/ds-sweep-20261017-01/board-release.json)，本会话不再操作板卡。
