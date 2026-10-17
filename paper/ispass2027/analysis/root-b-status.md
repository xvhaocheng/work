# Root B：分析、写作与仿真线

日期：2026-10-17 JST。独立工作树 `/home/xvhaocheng/work-root-b`，
分支 `ispass/root-b`，起点 `01f981a9c603fa909fdb2fb22450aa7c5197271f`。
本任务不使用 FPGA，不切换主目录或内部组件分支，不修改生产 RTL。

## 分工与产物

| 工作 | 产物 | 验收范围 |
|---|---|---|
| B-1 | `baseline100-extra.md/json`、`scripts/baseline_extra.py` | 原始计数器语义、窗口边界、分 boot、逐试次统计与固定种子 CI |
| B-2 | `build.sh`、`scripts/`、`figures/`、`figures.md` | 十图骨架与有数据图；拒绝混基座；缺数据不造数 |
| B-3 | `../tex/` | 匿名 IEEE 双栏稿、引用与 claim 映射、实际 PDF 编译 |
| B-4 | `../exp/E7-capacity-sim/` | 真实 iDMA/IOMMU 测试副本；16项校准，容量矩阵与原始记录 |
| B-5 | `../artifact/plan.md` | 最小复现集合与匿名处理计划，不执行发布 |

三个 Codex 子任务分别负责 B-1、B-2、B-3/B-5；root 负责 E7、关键
来源核对和最终验收。本项目遵守 Codex-only，未调用 DeepSeek 或 Claude。
执行依据为用户已指定的分析、论文撰写和 E7 仿真任务，ARS 按相应阶段
使用；未重新启动完整研究 pipeline 或历史机制探索。

## 已完成与验收

- B-1重算100试次、20分组和600项统计；逐字节重建核对、负向输入、
  1000个硬件/路径标量与40个原摘要中位数通过。DS0 A1合计响应记账
  吞吐为Solo的105.579%；精确同窗W字节仍有±4096B边界。
- B-2十图流水线已执行，统计/证据8项检查通过。E2/E3/E4/E5仍为空图；
  E1b当前只有DS0/DS50锚点。E7完整12格经独立receipt进入图9。
- B-3匿名IEEE 10pt双栏初稿已编译；数字由流水线宏生成，主张绑定
  原始证据。最终页数及可见版面检查见`../tex/build-report.md`。
- B-4 16/32/64×Solo/A1/A8/A64全部完成；每格4096次V COPY。
  `../exp/E7-capacity-sim/analysis-import.json`绑定12格的真实结果。
  A8在32/64项时walk归零并与A1同时间；A64共130个翻译不能由64项
  完全容纳，压力仍有残余。仅仿真，普通MMIO供给，不是新板测。
- B-5最小复现/匿名准备计划位于`../artifact/plan.md`，尚未实施发布。
- 已按用户追加要求分批推送**论文可用数据**；事件trace、UART/构建
  日志不属于途中同步的数据，相关新仿真日志仅本机留存。

## Root A 当日快照

来源：`/home/xvhaocheng/work-root-a/paper/ispass2027/status/root-board.md`，
2026-10-17 JST 只读取得。该快照尚无本轮新增板测；后续结果以其最终
实验目录和通过的运行 manifest 为准，不能把计划点数当结果。

- E1b：DS25/DS100 构建接续中，计划新增100点，尚无新增接受数据。
- E3：当前 pathv3 batch 固定4096B且 V/A 共用 chunk。最终只读复核时
  Root A状态已更新：用户批准仅P0最小改动、构建和45点板测，Root A
  正在分批实施；尚未产生新尺寸结果。本线不实施该扩展。
- E2：当前接口只有一名 aggressor，三至四路需接口扩展。Root A已接受
  有界multi HPM软件接口准备，待P0接口稳定后整合；没有新增规模结果。
- E5：v3 只有 sum/count/max，没有逐请求样本或直方图，不能恢复 CDF。
- E6：现有 BIT/driver 支持角色互换，采集包仍在准备。
- E5当日另有统一100次PATH阶段数据派生CSV（1600行/160组），来自原
  记录，不是新板测或请求CDF；本线不重复加样本数。

## 隔离与合并

用户开头的 Git 工作方式优先：不直接编辑共享
`data/board_results.csv`、`README.md` 或根 `CURRENT_STATUS.md`。
候选总表、更新建议及检查结果保存在本任务 analysis 目录；最终仅推送
任务分支，不自行合并 main。实际合并时应重读 Root A 状态，按稳定 ID
去重并检查 base_id，再更新共享入口。

首次同步前已`pull --rebase origin main`，纳入main的`f1917a2`。
同名B-1报告的add/add冲突已定向合并：本任务报告保留完整源码和操作数
追源，main旧版原文另存`history/main-baseline100-extra-f1917a2.md`，
没有覆盖其历史。共享README/CURRENT及Root A实验目录不由本任务修改。

## 后续检查点

- 2026-10-25：`headline-decision.md` 只能在明确区分已测结果与未到位
  E1b/E3 数据的前提下给出候选和建议；最后头条由用户选择。
- 2026-11-08：图表冻结尚未发生；当前十图为工作骨架，缺口明确保留。
- 本文件是当次工作记录，不代表已设置每日后台运行或未来自动执行。
