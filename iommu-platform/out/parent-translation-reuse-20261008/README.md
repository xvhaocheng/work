# §36 普通父翻译复用／提前准备

- [判断、前作、成本与下一步](../../docs/parent-translation-reuse-boundary-20261008.md)
- [六项有效运行验收](analysis.json)／[可读汇总](analysis.md)／[DeepSeek起草的汇总脚本](reuse_summary.py)
- [Codex直接重算与状态声明](codex-metric-audit.json)／[旧post至少六原父提前可见的CSV见证](lookahead-witness.json)
- [关闭复用与旧pre轨迹相同](off-trace-equality.json)／[76生产SV不变](production-check.json)／[执行与源码哈希清点](stage-evidence.json)

| 有效新运行 | V先 | A先 |
|---|---|---|
| 关闭复用 | [bound0-v-32768.log](bound0-v-32768.log) | [bound0-a-32768.log](bound0-a-32768.log) |
| 普通父内复用 | [bound1-v-32768.log](bound1-v-32768.log) | [bound1-a-32768.log](bound1-a-32768.log) |
| 普通提前准备 | [early2-v-32768.log](early2-v-32768.log) | [early2-a-32768.log](early2-a-32768.log) |

每stem配套`.csv`、`-command.json`直接argv和`-result.json`实际exit/seconds/reused。六项全exit0/PASS，778最终解析条件通过；四份§35 pre/post仅参考，未重跑。

V先双流平均：关闭25799、复用25712、提前25714、旧post18810。复用／提前TL真实调用66，背景准备32、缓存返回64；都34miss/102PTW/64physicalAR/34AW/每流4096R-W，数据与guard检查由原C++驱动完成。普通当前父准备不是同未来可见范围的基线。

首批`reuse0/reuse1-{v,a}`四项均exit0/PASS，但SVplusargs没生效，actual enabled1，不能算关闭对照，明确excluded；文件保留。首构建`build-*`及`.initial`源码快照保存，obj-reuse旧生成代码含enable常量1；C++显式输入的修正模型在obj-reuse-port，提前模型在obj-early。3构建/10运行是实际总数，非6次。

Codex作者：普通关键RTL、绑定、参数修正、提前准备、C++开关适配、成本/可见范围分析与数学口径修正；所有工具执行由Codex完成。DeepSeek：接口证据、汇总草案与offset/字段名修订，完整回复在evidence/summary/revision的task、raw、answer文件。最终汇总里双流均值由Codex改为同一个V先run的V与A，不混两个先发起者。

声明模块state普通3514／提前3518／post3568bits，未综合、非总面积。payload两DMA48B，无新增R缓存；invalidations硬接0、静态正常页内任务，不作production fault／epoch保证。无原tests/生产RTL/Linux/BIT/SD/板卡操作。
