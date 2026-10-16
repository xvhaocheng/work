# 独立只读核账

`verify.py` 仅解析既有 `events.yaml` 和 `official-trace.ch0`，不会调用模型、构建、改动原始日志或生产源码。Python 3 + PyYAML（含 `CSafeLoader`）即可运行。

```sh
python3 /home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/verification/verify.py \
  --runs /home/xvhaocheng/work/iommu-platform/out/d-dram-boundary-20261015/runs
```

默认写本目录 `report.json`；可用 `--report` 指定本目录内其他文件。退出码：通过0，失败1，尚缺数据2。没有原始数据时只报告 pending，不声称核账或负变异通过。`frozen-inputs.json` 固定 root 合同、运行前预测及接口卡的SHA-256；后续合同改变须由root明确处理，脚本不会自动更新冻结值。

检查包括冻结8个私有case和1个原库control；逐请求send/唯一callback/公开地址映射/实际RD加20拍；连续完整post_tick与时钟、refresh deadline及排空谓词；32个bank的ACT/PRE/PREA/REF状态和cap4；priority enqueue接受、cap4触发PRE、实际issue、所有rank REF end和nRFC−1；全量fullsnapshot中的行、计数、队列内容和future；官方TraceRecorder与真实issue迹完全相等；原库callback/官方迹相同；replay全记录归一化相同。REF end时ACT的路径deadline单独核验并记录一拍差，不用RD timing-ready代替行命中。

排空只按已接受输入核账；未来冻结target不算未完成请求。callback只登记完成，所有排空判定在post_tick/boundary核对。洗去对照比较全部bank有效状态、cap和全部节点ready deadline成熟性，不要求历史数组或过期deadline原始值清零。预测命中独立列出，不作为证据核账的替代。

通过真实数据核账后，脚本在内存副本分别注入重复callback、漏rank1刷新end、漏cap PRE enqueue、pending尚存伪drained四类错误，要求拒绝；不改raw，不重跑模型。

独立脚本只检验本批物理read模型证据；不判断P/PN收益、传感器新意、O_host充分性或原生IOMMU/硬件表现。root仍负责关键源码审查、实际日志抽核与最终接受。建议/请求配置Sol/medium，实际运行模型/强度元数据unknown。

首次核账暴露验证脚本误用了open-row枚举3作为close-row；原始DDR4请求枚举证实close-row=4，已修脚本并保存首次失败 `diagnostic-initial-request-enum.json`。此处修复没有改模型、输入或原始数据。
