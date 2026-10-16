# 当前状态 stage 97 2026年10月10日完整批次判别完成

- [本轮结果](iommu-platform/docs/workflow-g0-m0-results-20261010.md)：M0共同就绪64＋64测量已实现并验收，G0登记的10配置全部完成。正常测量双方自然结束，不再以V窗口截断A。
- A8下P/PN的V完成为86594/90283周期；同一普通DS优先后均为76738，A也均为76164。A1四模式完全相同。轨迹中的额外DS返回等待3689周期与原完成差额一致。
- 裁决：结束以该P/PN差额支持新增逐copy门控的分支，不进入G1、不扩矩阵。普通服务顺序已消除本合同差额；共享翻译主问题仍保留，不能据此判全领域无空间。
- 范围：固定COPY微基准、静态映射、CPU关闭、行为DDR模型。DS优先尚非综合硬件，结果不替代旧PN实板归因。N/S的miss及工作量不同，未被本干预解决，也不自动立为新题。
- 验收包括双完成顺序、共同首发延迟128、每路独立watchdog、真实字节/B/quiet守恒、合成CPU读源开关、旧模式回归；14损坏记录及9非法参数均拒绝，2560事件时间戳匹配原始CSV。
- 实际修改测试C++/SYSTEM顶层/运行器/校验器及离线脚本；两种Verilator构建、16次仿真进程（15正常通过，1预期失败；A8 N复用M0记录）。无生产RTL/Linux修改、FPGA完整构建或板测。
- 下一步：保留已验收夹具，归档本分支立题理由。后续轮次须另有应用动机和普通控制的具体失效，不自动重启旧候选；以[研究计划.md](研究计划.md)和[研究方法.md](研究方法.md)为流程依据。
- [根裁决](iommu-platform/out/workflow-g0-m0-20261010/root-result-acceptance.json)／[原始结果](iommu-platform/out/workflow-g0-m0-20261010/results.json)／[执行账本](iommu-platform/out/workflow-g0-m0-20261010/execution-ledger.json)／[模型调用](iommu-platform/out/workflow-g0-m0-20261010/worker-ledger.json)。
- [本轮前状态原样归档](iommu-platform/docs/status-stage95-before-full-batch-g0-20261010.md)。最新研究计划的G0/M0增订已存在；本页同步此前滞后的执行状态。
