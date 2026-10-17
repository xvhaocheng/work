# Root B 合并交接

任务分支：`ispass/root-b`；工作树：`/home/xvhaocheng/work-root-b`。
本任务不合并main，不切换主目录分支，不修改内部组件仓库。

## 已可复用的结果

1. B-1：`baseline100-extra.md/json`与`scripts/baseline_extra.py`。
   源码摘录、原始操作数行、试次ID、稳定结果ID与公式均保留；
   不是重新采集100次，也不新增板测样本。
2. B-2：`build.sh`重画十图和生成论文数字；`figures.md`列实际状态。
   `merged-board-results.csv`是带base_id/来源列的审查副本。
   **本次板测行仍为17404，未新增板测行，无需覆盖canonical总表。**
   该副本增加审查列，不应未经原数据工具兼容检查直接替换原17列CSV。
3. B-3：`../tex/main.pdf`为可编译的匿名工作稿；`claims.md`和
   `numbers-provenance.json`为内部追源材料。空图和待补实验公开标记，
   不是投稿就绪声明。
4. B-4：`../exp/E7-capacity-sim/results.csv`及`analysis-import.json`，
   12格真实RTL仿真。逐任务论文数据和必要身份清单已同步；事件trace
   与构建/运行日志留本机。它们不进入板测总表。
5. B-5：`../artifact/plan.md`为最小复现和匿名准备计划，不执行发布。

## 合并步骤（由汇总线执行）

- 获取并审查`origin/ispass/root-b`；与main比较时只应看到本任务的
  analysis、tex、artifact、E7目录，不能包含board或共享入口的本任务编辑。
- 将任务分支合入main；若main又更新了`analysis/baseline100-extra.md`，
  保留两份来源和原始证据，采用包含完整JSON/源码操作数追源的版本，
  不丢掉原统计或历史口径。第一次rebase遇到的旧版已逐字保存在
  `history/main-baseline100-extra-f1917a2.md`。
- 新Root A板测按`merge-notes.md`接收完整结果快照，检查row_id、base_id、
  正确性状态与源/运行清单；仅在真实新增板测到达时更新canonical。
  既有E3历史DS0离线分析是原50点重算，不能重复计入。
  实际Root A原生17列输出还需派生base_id/完成receipt交接层，具体
  最小差项见`root-a-import-interface.md`；当前没有新接受样本被阻挡。
- 执行`analysis/build.sh`、`make -C ../tex check`及`make -C ../tex`；
  按`../tex/build-report.md`核对页数、字号、匿名元数据和已知缺口。
- 最后根据最新Root A状态更新共享README/CURRENT。下面是本次建议，
  本任务没有直接写这两个文件。

## 共享入口更新建议

README交付表新增：B-1补充分析、十图构建入口、匿名LaTeX初稿、E7容量
仿真和artifact计划。图9已由真实12格仿真补齐；E2/E3/E4/E5仍有缺口，
E1b当前仅DS0/DS50锚点。DS25/100是否新到位以Root A最终验收为准。

CURRENT可加入：2026-10-17 Root B完成离线统计、图表流水线、匿名稿与
16/32/64项容量仿真。DS0 A1同窗响应记账合计为Solo的105.579%，精确
W字节保留±4096B边界；DS0 A8读内部阶段约142.505周期，DS50约600.914。
A8在32/64项仿真中容量相关增量消失，A64仍超容量。全部为有界配置
证据，不新增板测、不宣称纯PTW成本或通用HOL新原理。

## 潜在冲突与限制

- `analysis/baseline100-extra.md`与main旧报告同名；相关旧CSV仍按原版本
  保留，新JSON和分层bootstrap采用明确的新生成入口，不混两套CI。
- `tex/numbers.tex`由图流水线生成，手工修改将被重建覆盖；正文数字
  应通过来源接口新增宏，不复制四舍五入数值作新的真值。
- Root A的新接口/观测器可能形成新base_id；与本次基座不同时分panel，
  不能改ID冒充同基座。CDF只能用实际请求样本，mean/max不能恢复。
- 10/25头条备忘录目前是`headline-decision.md`预备稿，待新E1b/E3证据
  后由用户决定；11/8图表冻结尚未发生，未设置后台定期任务。
