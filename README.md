# 共享IOMMU研究工作区

研究多DMA共享地址翻译服务时的干扰、完成依赖与组织成本。开发平台为CVA6＋四iDMA＋RISC-V IOMMU＋Linux。这里同时保存代码、历史实验、条件模型、研究裁决和参考文献；当前探索与硬件状态以状态页为准。

## 新agent上手

1. [当前状态](CURRENT_STATUS.md)：现在推进什么、已有结论和执行边界。
2. [完整研究导航](研究导航.md)：5–10分钟阅读路线，以及按任务找代码、成果、原文的方法。
3. [版本与分支导航](iommu-platform/docs/versions.md)：当前开发工作树、原版/修复基线、研究扩展，以及实验实际选用版本。
4. [唯一总计划](研究计划.md)：本次工作范围；[AGENTS](AGENTS.md)与[执行约定](docs/agent-policy.md)规定如何开展和交付。

## 按需要进入

| 要了解什么 | 入口 | 内容 |
| --- | --- | --- |
| 仓库结构与代码 | [仓库结构](docs/repository-layout.md) | 平台、组件、测试、脚本、产物和历史副本的职责 |
| 选分支与记录实验版本 | [版本导航](iommu-platform/docs/versions.md) · [实验身份记录](docs/agent-policy.md#实验身份记录) | 按任务选源码，核对组件配对；分支、commit、差异和实际编译源分别留存 |
| 已做工作与结果 | [研究成果索引](docs/research-results.md) | 各方向的问题、实际结果、证据类型、停止范围及原报告 |
| 历史过程 | [研究历史](docs/research-history/README.md) · [阶段交接](HANDOFF.md) | 时间线、逐日证据和阶段决策；按题目查阅 |
| 论文与技术资料 | [根文献库](references/README.md) · [结构化目录](references/catalog.json) | 原文、逐项总结、核查状态及研究关联 |
| 平台使用 | [平台README](iommu-platform/README.md) · [构建说明](iommu-platform/docs/build.md) | 版本、profile、构建和恢复；阅读入口不要求运行构建 |
| 验证与旧计划 | [验证记录](iommu-platform/docs/validation.md) · [历史计划](iommu-platform/docs/history/plans/README.md) | 原验收、历史口径与归档定位 |

## 资料如何放置

```text
AGENTS.md / docs/agent-policy.md   稳定工作规则
CURRENT_STATUS.md / 研究计划.md   当前状态与唯一总计划
研究导航.md                      完整上手与按任务导航
references/                      文献目录、总结和原文入口
docs/research-results.md          按主题维护的成果索引
docs/repository-layout.md         代码与资料职责地图
docs/research-history/           历史时间线与证据
iommu-platform/                  当前代码、平台文档和研究原始产物
platform-archives/               历史工程与退役实现
```

当前页负责入口，状态和数字不在多个规则文件中重复维护。增加成果时更新成果索引，使用新文献时补文献条目与核查状态；原始资料保持可追溯。旧研究总览、旧代码地图和旧计划按历史快照阅读，不作为当前任务指令。

原学位论文位于本机`/home/xvhaocheng/thesis`，旧工程`/home/xvhaocheng/riscv-iommu-demo-refact`默认只作历史参考。当前共享IOMMU研究使用[ARS-Codex](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)，按阶段选择工作流。

## GitHub 同步

公开仓库：[xvhaocheng/work](https://github.com/xvhaocheng/work)，默认分支 `main`。以后完成本项目的修改并检查后，将相应改动一并提交推送；同步授权和并发处理规则见 [AGENTS.md](AGENTS.md) 与[执行约定](docs/agent-policy.md#文件交接与同步)。

顶层仓库汇总当前工作区文件，内部组件的本地 Git 历史保持原样；已有源码历史备份也随快照保存。研究记录、日志、原始数据及历史归档均保留，仅筛除构建产物和凭据。顶层快照不自动重建内部组件的 `.git` 元数据。

普通修改只增量同步本次路径。首次收录或需要重建完整清单时，才使用 `tools/github_sync.py` 的 `plan --root .`，检查后执行 `stage --root .`、`audit --root .`；记录在本地 `.git/sync-preparation/`。克隆后可运行 `python3 tools/github_sync.py restore --root .`，按 `.github-sync/large-files.json` 校验并还原分片文件；默认不覆盖内容不同的已有文件。

大文件原件在本机保留，Git 中保存内容寻址分片及 SHA-256 清单。脚本不会自行提交、推送或删除文件。
