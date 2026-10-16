# 平台任务入口

- 模型与推理强度按上一级[AGENTS.md的任务分配策略](../AGENTS.md)执行，root和子agent均按当前任务选择，不再固定全树Sol/ultra；该策略是项目约定，非官方为本项目验证的最优配置。共享IOMMU任务继续Codex-only，研究方向、实验授权及暂停状态不因规则更新改变。
- 若可访问上一级工作区，先读[当前状态](../CURRENT_STATUS.md)，按[唯一总计划](../研究计划.md)和[当前研究导航](../研究导航.md)定位任务；需要阶段进展与历史证据时定向读 `../HANDOFF.md`，避免默认全面扫描仓库。旧计划经[历史计划索引](docs/history/plans/README.md)定位。
- 若该交接文件不在当前副本中，先读 `README.md`、`docs/validation.md`、`docs/patches.md`，再按需查 `sources.lock.json` 和相关源码。
- 用简洁中文交流。当前平台为 CVA6 + 四 iDMA + IOMMU + Linux；iDMA 源码由 CVA6 跟踪。
- E3 v5及v6后续板测已完成，受控v6证据见 [docs/e3-controlled-results-review.md](docs/e3-controlled-results-review.md)；旧“v5未上板”仅为早期快照。新性能主张须验对应时钟/时序，不据历史Mac上板推出当前Dell接板。stage97的M0已验收，是可复用测量资产；P/PN差额驱动新门控的该分支已关闭，不表示当前研究阶段或总方向。全局安排、分支局部合同及硬件状态分别记录，局部合同不改写全局方向。
- 小依赖和恢复副本只在编辑器中隐藏，未删组件、未合并 Git；当前构建仍依赖各级 Git 提交和子模块指针。
- 修改前核对相关工作树和实际版本。不要只为熟悉环境运行全量构建或扫描 `.git`、`out/`；不要重复应用 `provenance/` 中已纳入提交的补丁。`make check` 会写 `out/manifests`。
- 结束实质性任务时，在可访问的 `../HANDOFF.md` 保留阶段进展与证据，同步更新 `../CURRENT_STATUS.md` 和对应验证文档；清楚区分源码改动、构建结果、仿真结果、板上结果和候选方案。写入前重新读取并合并并发更新，不用旧快照覆盖独立进展。原始日志、数据、产物和既有脏文档不因整理而删。
