# 论文工作区

研究多个DMA共享RISC-V IOMMU时的地址翻译干扰。当前开发平台为`iommu-platform/`。

- [当前状态](CURRENT_STATUS.md)：全局安排、各分支实际进展、硬件暂停和独立工程事项。
- [研究总计划](研究计划.md)：唯一总计划；本轮D主线、I轻量预筛，10月15—28日立题窗口。
- [研究导航](研究导航.md)：当前方向、成果证据与必要代码入口。
- [ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)与[任务规则](AGENTS.md)：以ARS-Codex对应工作流为方法入口；按当前任务阶段选择，不能把来源/条件预测当测量结论；遵守已确认研究边界与用户创新判断要求，不默认强制全pipeline。
- [阶段交接](HANDOFF.md)、[验证记录](iommu-platform/docs/validation.md)、[历史计划归档](iommu-platform/docs/history/plans/README.md)：保留历史事实、原文和引用定位。
- [平台使用与构建](iommu-platform/README.md)；[A5局部合同](iommu-platform/docs/research-execution-plan-20261011.md)是分支资产，不替代总计划。

E3 v5及v6后续板测已完成，[受控结果](iommu-platform/docs/e3-controlled-results-review.md)仅支持限定的局部等待传播。新实验和硬件工作按当前计划及实际验收条件推进，MIG仍暂停。

原学位论文位于`/home/xvhaocheng/thesis`。旧工程`/home/xvhaocheng/riscv-iommu-demo-refact`及[platform-archives](platform-archives)默认只作历史参考。旧论文逻辑、9月代码地图和早期研究内容图经[研究导航](研究导航.md)进入，按历史快照使用。

## GitHub 同步

顶层仓库汇总当前工作区文件，内部组件的本地 Git 历史保持原样；已有源码历史备份也随快照保存。研究记录、日志、原始数据及历史归档均保留，仅筛除构建产物和凭据。顶层快照不自动重建内部组件的 `.git` 元数据。

文件筛选和大文件还原入口为 `tools/github_sync.py`：先运行 `plan --root .`，检查清单后运行 `stage --root .`、`audit --root .`。计划和执行记录保存在本地 `.git/sync-preparation/`。克隆后运行 `python3 tools/github_sync.py restore --root .`，即可按 `.github-sync/large-files.json` 校验并还原分片文件；默认不覆盖内容不同的已有文件。

大文件原件在本机保留，Git 中保存内容寻址分片及 SHA-256 清单。脚本不会自行提交、推送或删除文件。
