# 2026-10-15 归档与旧文档增量核查

结论：12份历史计划/状态快照已搬到`/home/xvhaocheng/work/iommu-platform/docs/history/plans/`，此次只改变导航与当前入口组织，不构成新实验或科学结论。今日只读独立比较确认12/12归档SHA同时匹配manifest及10月14日本任务本地快照；2个旧路径为相对symlink、另10个旧路径已移除。历史全文阅读从10月14日账本继承，不能把12条40字符的`EXACT CONTENT MATCH WITH PRIOR SNAPSHOT`当作今日重读12篇全文。

## 今日实际阅读与边界

独占reading-chunks.json的16—34、39—42、46、48、50、57，共27块。15块含新增索引或链接修改上下文，合计19,449字符；12块为相同字节标记。全部已实际输出阅读全文；新归档README为21、22两块完整阅读，其余13块仅当前行段，未重复读对应旧报告全文。逐块账本见daily-archive-ledger.json。SSH补核证据保存daily-archive-evidence.json，包括归档逐文件SHA、旧路径状态、14个坏链的当前候选定位；没有读取全部实验原始数据、重新验收科学结论或改写远端文件。

本任务复用既有agent。当前AGENTS按任务选择模型/强度的策略已获知；现有agent没有原位切换工具，实际运行模型/强度元数据本席无法核验，故标unknown，未声称已换模型。归档和引用整理属配置建议中的普通文档整理，不能用建议配置冒充实际运行证明。

## 12份归档与兼容安排

全部新路径前缀为`/home/xvhaocheng/work/iommu-platform/docs/history/plans/`；原路径前缀为`/home/xvhaocheng/work/iommu-platform/docs/`。

|文件|原始行数|旧路径当前状态|
|---|---:|---|
|research-convergence-plan-20261010.md|142|移除|
|next-mechanism-plan.md|80|移除|
|e3-next-method-plan.md|71|移除|
|experiment-plan.md|179|symlink → history/plans/experiment-plan.md|
|experiment-implementation-plan.md|97|移除|
|e3-shared-service-plan.md|62|移除|
|e4-supply-timing-plan.md|71|移除|
|nonblock-pilot-plan-20261004.md|129|symlink → history/plans/nonblock-pilot-plan-20261004.md|
|nonblock-v5-qualified-read-plan-20261004.md|167|移除|
|status-stage94-before-main-plan-20261010-root.md|15|移除|
|status-stage95-before-comprehensive-plan-20261010-root.md|14|移除|
|status-stage95-before-plan-review-20261010.md|12|移除|

归档验收记载12份124106字节、1039行，来源匹配12、漂移空。两个symlink的理由是保留已有行号引用：experiment-plan旧`:57`，nonblock-pilot旧`:115`及`:109–124`。文件原字节/原行号保存，不加banner或重写归档原件相对链接。直接在新目录点旧原文内相对链接仍可能走错位置，必须用README的12张定位表；不能写成“归档原文所有链接已修复”。旧HANDOFF、validation、其他冻结status和旧out字面路径未全局改写，其导航靠迁移索引。

## 引用维护的实际范围

验收记录为13份维护报告、16处Markdown链接目标修改，行数和非链接内容保持；索引75个链接（62相对＋13绝对），其中61原有效、14迁移前已坏。今日分配的13块上下文恰覆盖这13份报告：e1-results-review、e3-controlled-results-review、experiment-design-v1、experiment-instrumentation、experiment-v1-presentation、nonblock-dependency-diagnosis、paper-logic、paper-scenarios-mitigation、path-results-review、research-n-prototype-spec、research-overnight-progress、research-review-2026-09-09、workflow-framing。paper-logic一块有两个行段，链接目标不只一个。

这些增量主要将E0—E4、P4M、nonblock v5、收敛计划导航改到`history/plans/`，上下文仍是原日期历史状态。例如“E2未证”“v5构建中”“E3待实现”不能因10/15改链接或mtime变化就当作10/15研究状态；已有10/14综述的E2新64、method88负结果与v5实际触发记录仍有效。研究日期以正文、合同与运行身份为准。

## 14个迁移前坏链与今日时点差异

14个坏链仅来自三份原样状态快照，不是12篇各有14个：

- stage94-before-main-plan：1条，原行3的`研究方法.md`。
- stage95-before-comprehensive-plan：7条，原行3的研究计划/研究方法，原行12的main-plan回执/ledger/收敛计划/stage94，原行14的plan-review回执。
- stage95-before-plan-review：6条，原行3的研究计划/研究方法，原行12的main-plan回执/ledger/收敛计划/stage94。

归档README标`preexisting_broken`，仅给候选正确定位，不修改旧文/旧接受hash。验收在2026-10-15T11:04:40Z记录14个候选目标存在；本席稍后只读实际检查得到**11条当前存在、3条不存在**。3条均指根`/home/xvhaocheng/work/研究方法.md`，与最新AGENTS说明用户主动删除旧方法、改用ARS-Codex相容。应表述为“历史验收14/14；当前11/14且缺失3条是同一旧方法文件”，不能声称当前14条都能打开，也不恢复已删除方法。其余11条只核路径存在，不宣称标题锚点或科学内容重新验收。

## 对统一研究整理的影响

1. 更新来源导航路径；保留旧行号和10月14日历史全文阅读账本，不虚增今日已读统计。
2. 将归档视为“旧计划退出当前执行入口”，不解释为相关科学结论关闭/失败，也不恢复暂停goal、MIG采集或板测。
3. README开头D主线/I轻量预筛属于归档时的研究安排记录，且明确不是创新通过；用户后来取消A默认优先级与当前方向判断由根入口新增证据整合，不能由这篇归档索引单独裁定。
4. root/private wiki历史来源若写旧裸路径，可记录迁移映射；无须回写被冻结实验记录。旧方法三处候选定位标“已按用户要求删除，历史引用保留”，不要伪造现存文件或补建旧方法。

## 可核查来源

- `/home/xvhaocheng/work/iommu-platform/docs/history/plans/README.md`（全文新索引）。
- `/home/xvhaocheng/work/iommu-platform/docs/history/plans/manifest.json`（定向读取files/计数/坏链字段，并独立核SHA及存在性；首次全文工具输出截断，未计全文已读）。
- `/home/xvhaocheng/work/iommu-platform/out/research-plan-implementation-20261015/archive/archive-acceptance.json`（定向标量、scope与limitations；非重新实验验收）。
- 本地`daily-archive-evidence.json`与`daily-archive-ledger.json`保存本轮字段与阅读边界。

本轮远端只读，无构建、测试、仿真、板卡动作、代码修改、凭据读取或外部消息。唯一需在统一材料中注明的导航缺口是上述已删除旧方法文件；不是本轮需要修复的研究阻断。
