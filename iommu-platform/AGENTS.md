# 平台附加规则

继承[工作区规则](../AGENTS.md)。先读[当前状态](../CURRENT_STATUS.md)与[研究导航](../研究导航.md)，按[总计划](../研究计划.md)定位任务；从平台副本独立开始时，读[README](README.md)、[版本锁](sources.lock.json)及相关报告。

- 平台为CVA6＋四iDMA＋RISC-V IOMMU＋Linux，组件入口见[仓库结构](../docs/repository-layout.md)。默认在此开发，旧工程和`platform-archives/`只作历史参考。
- 修改前核对目标组件的工作树、提交与`sources.lock.json`；按需读源码，不默认全量构建。`make check`会写`out/manifests`，不是严格只读。
- 构建、profile、组件提交链和恢复规则见[构建说明](docs/build.md)。`provenance/`是已有补丁来历，不重复应用；不修改内部Git拓扑或自动升级依赖。
- 原始日志、数据、历史快照与验收哈希保留。新分析用新目录，避免运行会覆盖旧输出的包装脚本；`out/`同时有研究证据和可再生构建物，按类型处理。
- 区分源码/主机测试/RTL仿真/构建/板测。当前硬件是否暂停、实际身份与时序以根状态和对应原始记录为准，历史上板不能替代当前证据。
- 完成后按根规则合并状态、交接和[验证记录](docs/validation.md)，同步本次完成范围；模型分工、Codex-only及暂停边界不在此重复定义。
- 版本整理首先落实在本平台及实际子仓库：组件源码先提交，再按 IOMMU → CVA6 → 平台更新 gitlink 与 `sources.lock.json`，核对活动工作区状态。顶层 `work` 的文件同步不能替代内部提交；保留旧历史与数据，不用忽略源码来消除变更计数。
