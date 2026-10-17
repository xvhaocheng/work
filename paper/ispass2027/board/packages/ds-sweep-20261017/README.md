# E1b DS25/DS100配套实验包

当前构建进行中，结果以[执行报告](../../ds-sweep.md)和实际manifest为准。
旧DS0/DS50的100次结果留在[原包](../baseline100-20261017/README.md)，不覆盖、不重测。

- [source.json](source.json)：同一固定源码commit及本轮诊断延迟overlay。
- [profiles](metadata/profiles.json)：DS0/25/50/100配置；DS25、DS100除延迟外与DS50相同。
- [实际构建入口快照](metadata/build_baseline.as-dispatched.py)：每档输出独立build manifest和日志。
- [离线接口检查](metadata/host-interface-validation.json)：仅替身检查轮转、标签、顺序与失败停止，无硬件操作。
- [构建审核](metadata/build-review.json)：与DS50的冻结输入差异；构建尚未完成时不提供可加载资格。
- [Root执行与任务内导入入口](../../../exp/E1b-ds-sweep/README.md)：实际命令、输出边界与交接。
- `artifacts/`本机保存新BIT及共用pathv3 payload/Image；二进制不进Git。

本轮复用已写入SD的payload，不重写、不回读SD。按DS25全部50次→DS100全部50次执行，
每档五条件各10次，失败保留并停止；不以旧G1性能阈值筛选本轮样本。
