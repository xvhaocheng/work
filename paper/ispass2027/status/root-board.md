# Root A 板测与代码线：2026-10-17 JST

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: UNVERIFIED（尚无本轮新增板测）
- Version Label: root-a-status-v1

## 今日进展

- 已创建独立 worktree `/home/xvhaocheng/work-root-a`，分支 `ispass/root-a`，起点 main `01f981a9c603fa909fdb2fb22450aa7c5197271f`。未切主目录分支，未改共享汇总文件。
- 已分派 A1 接续 DS25/DS100 构建与 E1b 运行包、A2 准备 E3、A3 核查 E2/E5/E4/E6。所有 agent 使用 gpt-6.1-sol / ultra，遵守共享 IOMMU Codex-only；子 agent 不操作板。
- 已发现并接续原有两个运行中的 DS 构建，输出在主工作区 `iommu-platform/out/ispass2027-20261017-ds25-01`、`...-ds100-01`；不重复构建、不改变其输入。验收尚未完成。
- 已建立[唯一板测队列](../board/queue.md)。板最后已知为 DS50/pathv3，本轮尚未访问 UART/JTAG。

## 关键数字

- 本轮计划 E1b：两档 × 五条件 × 十次 = 100 次新增板测。
- E3 优先小子集：三种 V 大小 × 三种 A 条件 × 五次 = 45 次；尚未测量。
- 历史基线每点平均采集开销约 15.7 秒，仅作队列预算；不作新硬件性能结果。

## 阻塞与决定项

E3 初核显示真实 pathv3 batch 在 CLI、driver、RTL 均固定 4096B，并且 V/A 共用 chunk。正在直接核查实际冻结调用链；不能仅改测试程序完成指定独立 V/A 大小。最小必要变更和用户决定项由 A2 放入 `exp/E3-workloads/` 后由 Root 审核。

E2 设备数、E4 2MiB 映射、E5 请求延迟分布及角色互换仍在能力核查，不提前宣布支持。所有需扩大内核、driver 或 RTL 修改范围的项目保留 change-request，不自行实施。

## 待合并内容

最终仅推送任务分支。共享结果表的新增行由实验目录 CSV 提供，README/CURRENT_STATUS 更新建议在最终交接中列明；本会话不直接编辑这些共享文件，也不自行合 main。
