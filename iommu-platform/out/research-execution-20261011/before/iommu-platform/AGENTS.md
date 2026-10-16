# 平台任务入口

- 若可访问上一级工作区，先读 `../CURRENT_STATUS.md`；需要阶段进展与历史证据时定向读 `../HANDOFF.md`，再按任务查代码地图和验证记录，避免默认全面扫描仓库。
- 若该交接文件不在当前副本中，先读 `README.md`、`docs/validation.md`、`docs/patches.md`，再按需查 `sources.lock.json` 和相关源码。
- 用简洁中文交流。当前平台为 CVA6 + 四 iDMA + IOMMU + Linux；iDMA 源码由 CVA6 跟踪。
- 2026-09-18：E3 v5 已完成主机验证及软件/FPGA 完整构建，BIT/MCS 非空，但 v5 尚未上板；Mac 已上板的是旧版 E3 BIT，不能推出当前主机/Dell 接板。原时钟约束审查仍待完成。详见 [../CURRENT_STATUS.md](../CURRENT_STATUS.md) 和 [E3_QUEUE_PAIR.md](E3_QUEUE_PAIR.md)。
- 小依赖和恢复副本只在编辑器中隐藏，未删组件、未合并 Git；当前构建仍依赖各级 Git 提交和子模块指针。
- 修改前核对相关工作树和实际版本。不要只为熟悉环境运行全量构建或扫描 `.git`、`out/`；不要重复应用 `provenance/` 中已纳入提交的补丁。`make check` 会写 `out/manifests`。
- 结束实质性任务时，在可访问的 `../HANDOFF.md` 保留阶段进展与证据，同步更新 `../CURRENT_STATUS.md` 和对应验证文档；清楚区分源码改动、构建结果、仿真结果、板上结果和候选方案。原始日志、数据、产物和既有脏文档不因整理而删。
