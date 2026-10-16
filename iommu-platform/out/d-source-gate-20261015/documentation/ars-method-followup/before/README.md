# 论文工作区

研究多个DMA共享RISC-V IOMMU时的地址翻译干扰。当前开发平台为`iommu-platform/`。

- [当前状态](CURRENT_STATUS.md)：全局安排、各分支实际进展、硬件暂停和独立工程事项。
- [研究总计划](研究计划.md)：唯一总计划；本轮D主线、I轻量预筛，10月15—28日立题窗口。
- [研究导航](研究导航.md)：当前方向、成果证据与必要代码入口。
- [研究方法](研究方法.md)与[任务规则](AGENTS.md)：如何比较真实问题、前作、创新及关键证据。
- [阶段交接](HANDOFF.md)、[验证记录](iommu-platform/docs/validation.md)、[历史计划归档](iommu-platform/docs/history/plans/README.md)：保留历史事实、原文和引用定位。
- [平台使用与构建](iommu-platform/README.md)；[A5局部合同](iommu-platform/docs/research-execution-plan-20261011.md)是分支资产，不替代总计划。

E3 v5及v6后续板测已完成，[受控结果](iommu-platform/docs/e3-controlled-results-review.md)仅支持限定的局部等待传播。新实验和硬件工作按当前计划及实际验收条件推进，MIG仍暂停。

原学位论文位于`/home/xvhaocheng/thesis`。旧工程`/home/xvhaocheng/riscv-iommu-demo-refact`及[platform-archives](platform-archives)默认只作历史参考。旧论文逻辑、9月代码地图和早期研究内容图经[研究导航](研究导航.md)进入，按历史快照使用。
