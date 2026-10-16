# A方法实现的层级与结论范围审查

2026-10-15；Codex独占关键接受审查。仅直接读 `method/design.md`、`method/analyze_contract.py`、`cases/frozen-contract.json` 和 `method/predictions.json`，未运行分析器、模型、执行器或RTL，未重读全仓，未改root文件。本审只回答：是否把source依赖／符号下界升级为实际硬件时序、普通优先足够或完整目标需要更早控制。

**结论：四案现有输出没有作这种完整时间／原生部署升级；UNKNOWN边界明确。规范中一处“需要更早控制”的充足条件有逻辑过强，另有source/offer层级及H2切点字段的歧义，应作下述最小修正。** 修正后可以接受本轮作为有明确条件的source语义与完整依赖闭包产物；这不等于接受真实共享IOMMU部署、完整时延、旧四项时间预测或独立方法新颖性。

## 已正确保留的范围

- 冻结合同 `execution_level` 为untimed source semantics，`source_scope` 明确publisher和共享IOMMU映射是未实现适配；`retirement_order_scope` 明确只是一个允许的无时序完成线性化，而不是HERO/CVA6/MIG顺序。见 `cases/frozen-contract.json` L5、L9、L46、L75；各案 `native_bindings_complete=false`，最大timed/RTL episodes为0（L113）。
- 四案 `full_completion_time_prediction=null`，`full_completion_deployment_decision=UNKNOWN_missing_shared_IOMMU_service_ownership_and_software_cost_binding`；P/N/H1/H2分别见 `method/predictions.json` L387–389、L765–767、L1129–1131、L1511–1513。总输出 `timed_execution_admitted=false`、`old_four_timing_predictions_accepted=false`（L1527–1531）。
- 分析器L110–128只产出事件前驱和持续时间符号，并明确它们是causal constraints，完整值需资源、服务、race guards。它没有把符号名填为任意常数，也没有把逻辑事件索引称为周期。
- `design.md` L49–59已要求完整状态、服务环境选择、资源和owner边；L57明确只有source DAG时只是依赖下界／符号约束。L74–80也保留source发布顺序可能不改变下游到达、SVM关闭与原生绑定缺项。
- P/H1的局部正向值限定为 `ordinary_source_head_selection_suffices_for_the_local_order_choice`，并未输出普通优先满足完整目标。N只禁止改写既定source发布前缀，冻结预测还明确不声称所有后续控制无用。H1减少job和字节没有升级为完整时间收益。

本席没有独立重做八次矩阵／字节／owner执行；其实际通过与否应由执行席和root对原始产物接受。本文确认的是以上四文件的静态结论范围。

## 必须收紧：完整目标“需要更早控制”的充足条件

位置：`method/design.md` L66。当前表格允许以下形式：

`当前位置所有允许动作中，相关关键义务已不可改变 或 完整目标不可满足；更早合法动作满足目标`。

其中前半段的“或”不够。某一已承诺义务不可改，只能证明涉及它的局部重排做不到；晚位置仍可能用另一个合法后继动作满足相同完整任务目标。若更早和更晚都能满足目标，不能称需要更早控制。

**最小修正：将局部和完整量词分开。**

- 对局部动作：若目标明确就是改写该source发布先后／替换同一端口已held packet，可以报告“这个动作需在其承诺之前控制”，仅到该动作。
- 对固定完整任务目标：只有在当前位置没有任何允许、可实现的策略满足该目标，而存在同预算／已计费的更早策略满足该目标时，才报告“该完整目标需要更早控制”。环境服务若不唯一，也应沿用事前声明的存在／全称量词。仅评估几个固定程序时，结论限于这些具名程序，不能变成对全部策略的必要性证明。

这不是本轮四案已输出错误的完整结论：分析器L127–128仍无条件输出null/UNKNOWN，没有实现或触发表中这条充分性规则。修正是为了防止后续绑定或报告误用本规范。

## 必须收紧：source发布承诺与物理offer不得混用

位置：`method/design.md` L78“若A1已启动/首次offer，则晚位置不能把B00插回它之前”；另见L43及L84。

本轮分析器的 `committed` 仅由 `case.prefix` 中kind为`publish`的节点组成（`analyze_contract.py` L85–90）。这能证明某段source publication历史不能被改写；并没有建模AXI同一端口的完整held packet、accept握手、跨端口路由或下游可重排队列。已在source较早发布的A1，能否在另一个下游服务位置被B00赶超，仍由实际接口／队列合同决定。

**最小修正：** L78改为“若A1的source publication已进入共同前缀，晚于这个发布点的控制不能改写该source发布先后；若同一物理端口已有A1 held offer，则不能在该端口用B00替换它。下游是否可赶超另核”。L43的held规则明确约束“该端口／该通道的packet”，不把一处held头推广为其他端口也无动作。L84将当前实现能力写为“source发布前缀保护”，将物理offer/accept保护保留为设计规范与待绑定检查。

相关字段 `local_deployment_decision`、`legal_orders` 虽已有source/local限定值，仍建议改名为 `local_source_order_decision`、`source_publication_order_projections`，或在紧邻字段补 `decision_level=source_publication_only`。这属于机器可读输出的最小消歧，不需要新增性能实验。

## 必须消歧：H2同一对象使用了两个切点

位置：`analyze_contract.py` L39–45从短 `case.prefix` 求ready；L99–108遇到 `prefix_until=K11` 时，用K11祖先集合另做切点证书，未更新前面的ready字段。

因此H2的 `predictions.json` L1150–1158同时含：

- `enabled_unpublished_after_fixed_prefix=[pub_B00,pub_A1]`，对应初始短前缀A0发布之后；
- `late_position_has_no_remaining_A_B_publication_choice`，其证书cut为K11、所有A/B发布属于此前，`remaining_publications=[pub_C1]`，对应另一个切点。

两项各自可成立，但不是同一状态。消费者若直接把它们合起来，会得到“K11切点A1/B00仍未发布但又无可选A/B”的矛盾。K11的祖先集合也不是一条已执行、含所有可选独立完成事件的完整prefix；它能证明必须先发生哪些事件，不能据此断言C0实际完成状态。

**最小修正：** 将旧ready明确标成 `enabled_after_initial_declared_prefix`，为K11证书写 `cut_basis=causal_ancestor_relation_only`；若要报告K11真实ready、已完成事件或C0状态，则引用独立执行器生成的共同prefix／状态快照。继续区分 `C0_must_have_completed_at_cut=false`（不是因果必需）与 `C0 actually incomplete`（要执行证据）。现有代码前者没有冒充后者；需要避免其他字段混用切点。

## 实现接受的准确描述

当前检查器核的是给定source事件表的无环性、前驱闭包、若干冻结source所有权／完成边、发布/完成/wait身份集、job/字节预算及符号约束。它尚未执行 `design.md` L35–49描述的通用资源/cache/协议状态空间，也未证明缺失的原生IOMMU绑定。此局限已经由输出UNKNOWN保留；可把本轮产物描述为**source合同结构检查器与符号因果约束生成器**，将协议／实现级状态探索描述为后续绑定接口。

修正上述三组问题后，无需改变四案null/UNKNOWN和旧时间预测未通过的状态。语义执行通过只支持真实矩阵、字节、source所有权与完整消费／释放流程；对共享翻译的部署结论和性能仍需要实际publisher、ready共现、协议所有权、服务及费用绑定。

## 本审快照

以下SHA256标识本次实际读取的版本；若root修订，本文的原行号与问题描述仍指本快照，不冒作新版本已复查。

|文件|SHA256|
|---|---|
|`method/design.md`|`1763a1d4a462d6d6cdddaaf4eeddb508289a4ce5461fd18d4aa0844ca0ab1714`|
|`method/analyze_contract.py`|`5a54f3d7f77a8670c306abfbf1a7aba27de951c54af70c7871f03e4726171864`|
|`cases/frozen-contract.json`|`fe077ddbd64508909ac1fbec072ac2ec6297163589a529e89c4b1dbd5d772e5b`|
|`method/predictions.json`|`7e865b182f9d4bcba6369ce467e6b89008f9d6a7e833bc13fd0806753a581e5b`|
