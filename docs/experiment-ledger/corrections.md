# 后续补充与结论修正

关系方向为“旧轮次 → 后续轮次”。“补充”不撤销原结果；“修正”限定旧解读；“替代”只针对具名版本。旧计划不授权继续执行。

| 原轮次 | 关系 | 后续轮次 | 理由 |
| --- | --- | --- | --- |
| [H-COMPLETION-RESERVE-LATE](periods/10-05-09.md#h-completion-reserve-late) | 同轮汇总 | [POOL-RESERVE](periods/10-05-09.md#pool-reserve) | 该原报告与总账拆分条目有覆盖重叠；不相加成独立实验数。 |
| [H-COMPOSITION-TRIM](periods/10-05-09.md#h-composition-trim) | 后续终态 | [COMPOSITION-CLEAN](periods/10-05-09.md#composition-clean) | 构建／裁剪与后续clean板测分开，不把旧尚未上板当现状。 |
| [H-COMPOSITION-V1-DELIVERY](periods/10-01-04.md#h-composition-v1-delivery) | 后续终态 | [COMPOSITION-PTWFIX](periods/10-05-09.md#composition-ptwfix) | 交付计划已有后续实际172完成，但首次失败仍保留。 |
| [H-D-DRAM-ACTION](periods/10-15-18.md#h-d-dram-action) | 后继 | [H-D-BOUNDARY-SOURCE](periods/10-15-18.md#h-d-boundary-source) | 选定前史被洗去后限制来源／动作版本。 |
| [H-D-DRAM-BOUNDARY](periods/10-15-18.md#h-d-dram-boundary) | 后继 | [H-D-DRAM-ACTION](periods/10-15-18.md#h-d-dram-action) | 来源状态差异是否经完整COPY准备仍保留。 |
| [H-D-MQSIM-IDENTITY](periods/10-15-18.md#h-d-mqsim-identity) | 后继 | [H-D-DRAM-BOUNDARY](periods/10-15-18.md#h-d-dram-boundary) | SSD有限残留停止后，独立核查DRAM状态持续性。 |
| [H-D-PERSISTENT-SOURCE](periods/10-15-18.md#h-d-persistent-source) | 后继 | [H-D-MQSIM-IDENTITY](periods/10-15-18.md#h-d-mqsim-identity) | 定位并修复缺完成根因；不重写初轮失败。 |
| [H-DEAD-TRANSLATION](periods/10-15-18.md#h-dead-translation) | 归因及模型边界修正 | [H-IOFENCE-INTERFERENCE](periods/10-15-18.md#h-iofence-interference) | 披露TL→TC后尚未到COMP的请求漏出旧快照；未证旧28组实际触发，不改旧数据。 |
| [H-E2-INTERFACE-POC](periods/10-05-09.md#h-e2-interface-poc) | 同轮汇总 | [POOL-RESERVE](periods/10-05-09.md#pool-reserve) | 该原报告与总账拆分条目有覆盖重叠；不相加成独立实验数。 |
| [H-E3-CONTROLLED-RESULTS-REVIEW](periods/07-09.md#h-e3-controlled-results-review) | 证据等级修正 | [CORRECTION-E3-TIMING](periods/10-15-18.md#correction-e3-timing) | 127–130周期属于时序未收敛构成，不是时序签核后的定量因果。 |
| [H-E3-DIAG-RESULTS-REVIEW](periods/07-09.md#h-e3-diag-results-review) | 后继 | [H-E3-QUEUE-RESULTS-REVIEW](periods/07-09.md#h-e3-queue-results-review) | v5已实际执行。 |
| [H-E3-QUEUE-RESULTS-REVIEW](periods/07-09.md#h-e3-queue-results-review) | 后继 | [H-E3-CONTROLLED-RESULTS-REVIEW](periods/07-09.md#h-e3-controlled-results-review) | v6增强配对与独立请求。 |
| [H-E3-RESULTS-REVIEW](periods/07-09.md#h-e3-results-review) | 诊断 | [H-E3-DIAG-RESULTS-REVIEW](periods/07-09.md#h-e3-diag-results-review) | D0/D0诊断触发与前史，不是D128因果对照。 |
| [H-BATCH-RESULTS-REVIEW](periods/07-09.md#h-batch-results-review) | 对照 | [H-DS0-BATCH-RESULTS-REVIEW](periods/07-09.md#h-ds0-batch-results-review) | DS0与DS50的附加延迟敏感性对照。 |
| [H-E4-RESULTS-REVIEW](periods/07-09.md#h-e4-results-review) | 后继 | [H-BATCH-RESULTS-REVIEW](periods/07-09.md#h-batch-results-review) | 自主批次形成真实持续重叠；不同版本不合并样本。 |
| [H-PATH-RESULTS-REVIEW](periods/07-09.md#h-path-results-review) | 后继 | [H-E3-RESULTS-REVIEW](periods/07-09.md#h-e3-results-review) | 从路径相关转入受控请求配对。 |
| [H-EXPERIMENT-V1](periods/07-09.md#h-experiment-v1) | 后续终态 | [H-E1-RESULTS-REVIEW](periods/07-09.md#h-e1-results-review) | 软件入口和主机回归之后，9月16日已有实际板测；旧未上板仅是早期状态。 |
| [H-FIRST-PTE-INTERFACE-COST](periods/10-15-18.md#h-first-pte-interface-cost) | 实际判别 | [H-FIRST-PTE-PAIRED-LOCAL](periods/10-15-18.md#h-first-pte-paired-local) | 局部M=普通base且mask0，关闭冻结收益版本。 |
| [H-FRAGMENT-PLACEMENT-BOUNDARY](periods/10-05-09.md#h-fragment-placement-boundary) | 公平对照修正 | [H-ORIGINAL-WINDOW-PROGRESS-BOUNDARY](periods/10-05-09.md#h-original-window-progress-boundary) | 同原始父窗口后27.09%缩至6.706%。 |
| [H-FRAGMENT-PLACEMENT-BOUNDARY](periods/10-05-09.md#h-fragment-placement-boundary) | 强对照补充 | [H-PREPARED-SOURCE-BYPASS-BOUNDARY](periods/10-05-09.md#h-prepared-source-bypass-boundary) | 普通预准备PA旁路使大部分差额消失，剩余不能无依据归因。 |
| [H-INNOVATION-COUNCIL18](periods/10-05-09.md#h-innovation-council18) | 同轮资料 | [H-INNOVATION-COUNCIL18-04867a](periods/10-05-09.md#h-innovation-council18-04867a) | README提案索引与根审定是同一18提案/18审核的不同资料。 |
| [H-INNOVATION-FOCUS](periods/10-05-09.md#h-innovation-focus) | 来源修正 | [H-INNOVATION-FOLLOWUP](periods/10-05-09.md#h-innovation-followup) | 先前503源码后续已取得，原返回环可由普通预留排除。 |
| [H-INNOVATION-FOLLOWUP](periods/10-05-09.md#h-innovation-followup) | 条件修正 | [H-INNOVATION-DEEPER](periods/10-05-09.md#h-innovation-deeper) | 实际早发AW前缀不证明完整更新环，更弱普通对照须纳入。 |
| [H-INVALIDATION-INTERFERENCE](periods/10-15-18.md#h-invalidation-interference) | 补充诊断 | [H-DEAD-TRANSLATION](periods/10-15-18.md#h-dead-translation) | 主线strict/FQ/ORACLE比较与死占用门槛；不把walk改善等同清死条目收益。 |
| [H-INVALIDATION-INTERFERENCE](periods/10-15-18.md#h-invalidation-interference) | 归因及模型边界修正 | [H-IOFENCE-INTERFERENCE](periods/10-15-18.md#h-iofence-interference) | 旧PR=PW=0两fence取数差不能叫主线PR/PW drain；已处理请求前沿有覆盖缺口。 |
| [H-LOCAL-READ-RECLAIM](periods/10-15-18.md#h-local-read-reclaim) | 端点勘误 | [H-SHARED-SERVICE-CONTROL-BOUNDARY](periods/10-15-18.md#h-shared-service-control-boundary) | TC内部移交、Intel行号和多别名成功范围更正。 |
| [H-E1-RESULTS-REVIEW](periods/07-09.md#h-e1-results-review) | 补充 | [H-E4-RESULTS-REVIEW](periods/07-09.md#h-e4-results-review) | E4定位完成发现与供给落点，限制对E1总慢化的解释。 |
| [H-MAC-P4M-FORMAL60-20260920-215406-RUN-CICGGIL4](periods/07-09.md#h-mac-p4m-formal60-20260920-215406-run-cicggil4) | 后续终态 | [H-P4M-COMPLETE-REVIEW](periods/10-01-04.md#h-p4m-complete-review) | 原采集统计与最终补齐60是同一正式计划的不同资料，不能累计重复。 |
| [H-P4M-PILOT-RESULTS-REVIEW](periods/07-09.md#h-p4m-pilot-results-review) | 后继 | [H-P4M-COMPLETE-REVIEW](periods/10-01-04.md#h-p4m-complete-review) | 启动与正式矩阵完成，保留25/6/29实际采集段。 |
| [H-MAC-P4M-REMAINING29](periods/10-01-04.md#h-mac-p4m-remaining29) | 终态归集 | [H-P4M-COMPLETE-REVIEW](periods/10-01-04.md#h-p4m-complete-review) | 29补点与旧25、保留6构成正式60，不是独立再测60。 |
| [H-P4M-SHORT-RESULTS-REVIEW](periods/07-09.md#h-p4m-short-results-review) | 后继 | [H-P4M-PILOT-RESULTS-REVIEW](periods/07-09.md#h-p4m-pilot-results-review) | 短功能不代替pilot收益。 |
| [H-MAC-P4M-START-20260920-211940-RUN-46NVMWGI](periods/07-09.md#h-mac-p4m-start-20260920-211940-run-46nvmwgi) | 同轮资料 | [P4M-START12](periods/07-09.md#p4m-start12) | 原采集与启动控制拆分条目属于同一次12项。 |
| [H-MAC-RESEARCH-CONT](periods/10-01-04.md#h-mac-research-cont) | 同轮资料 | [RESEARCH84](periods/10-01-04.md#research84) | 续跑只完成原80可运行项，不重跑前四项；原84计划仍不完整。 |
| [H-MAC-RESEARCH-FOLLOWUP](periods/10-01-04.md#h-mac-research-followup) | 后续验收 | [SOLO-FOLLOWUP11](periods/10-01-04.md#solo-followup11) | 采集回执当时未离线验收，后继复核已完成；不把旧未验收当现状。 |
| [H-MAC-RESEARCH-FULLSTATS](periods/10-01-04.md#h-mac-research-fullstats) | 后续验收 | [FULLSTATS21](periods/10-01-04.md#fullstats21) | 21次在线采集后有专门完整复核。 |
| [H-MAC-RESEARCH-V1](periods/10-01-04.md#h-mac-research-v1) | 同轮资料 | [RESEARCH84](periods/10-01-04.md#research84) | 初次采集及后续失败归因属于同一84计划。 |
| [H-MECHANISM-CONSTRUCTION](periods/10-15-18.md#h-mechanism-construction) | 成本修正 | [H-FIRST-PTE-INTERFACE-COST](periods/10-15-18.md#h-first-pte-interface-cost) | 强普通复用TC保持，不需要新header FIFO；旧字段小计不是面积。 |
| [H-METHOD-RESULTS-REVIEW](periods/10-01-04.md#h-method-results-review) | 强对照修正 | [H-METHOD88-EXECUTION](periods/10-01-04.md#h-method88-execution) | 冻结选择后两boot四格强匹配均负，关闭本版收益。 |
| [H-P4M-FORMAL60-RECOVERY](periods/07-09.md#h-p4m-formal60-recovery) | 终态补齐 | [H-P4M-COMPLETE-REVIEW](periods/10-01-04.md#h-p4m-complete-review) | 恢复后60完整，原失败／缺exit保持。 |
| [H-PAPER-COUNCIL](periods/10-05-09.md#h-paper-council) | 优先级修正 | [H-PAPER-COUNCIL-DECISION](periods/10-05-09.md#h-paper-council-decision) | 用户异议后继续性能研究、接口备选，不把初稿当最终一致意见。 |
| [H-PERFORMANCE-COUNCIL](periods/10-05-09.md#h-performance-council) | 优先级修正 | [H-PERFORMANCE-COUNCIL-DECISION](periods/10-05-09.md#h-performance-council-decision) | 后续原决定已撤销把四PLRU对照作为论文探索前置条件。 |
| [H-PROBLEM-DISCOVERY-RESULTS](periods/10-15-18.md#h-problem-discovery-results) | 直接证据补充 | [H-PLRU-ABLATION](periods/10-15-18.md#h-plru-ablation) | 原轮只有源码推导，后继直接victim追踪与LRU修正。 |
| [H-R107-BOUNDED-DECISION](periods/10-10-14.md#h-r107-bounded-decision) | 实际执行 | [H-R107-ORDINARY-PILOT](periods/10-10-14.md#h-r107-ordinary-pilot) | 最终普通有限恢复已板测，早期待板测状态被终态补充。 |
| [H-R107-BOUNDED-DECISION](periods/10-10-14.md#h-r107-bounded-decision) | 后继 | [H-R107-EXECUTION-GATE](periods/10-10-14.md#h-r107-execution-gate) | 从源合同进入实际部署前置核查。 |
| [H-R107-EXECUTION-GATE](periods/10-10-14.md#h-r107-execution-gate) | 身份恢复 | [H-R107-PLATFORM-IDENTITY-RECOVERY](periods/10-10-14.md#h-r107-platform-identity-recovery) | JTAG器件与实际S0启动已在后继核实。 |
| [H-R107-PLATFORM-IDENTITY-RECOVERY](periods/10-10-14.md#h-r107-platform-identity-recovery) | 实际执行 | [H-R107-ORDINARY-PILOT](periods/10-10-14.md#h-r107-ordinary-pilot) | 后来完成唯一普通有限恢复pilot，旧未运行只代表当时。 |
| [H-RCS1-COMPLETION-SET](periods/10-10-14.md#h-rcs1-completion-set) | 能力勘误 | [H-BLOCKING-REPLACEMENT](periods/10-10-14.md#h-blocking-replacement) | 64B fullbuffer例外为旧条件engine额外能力，生产许可为对齐4KiB。 |
| [H-RESEARCH-FOLLOWUP-REVIEW](periods/10-01-04.md#h-research-followup-review) | 同轮资料 | [SOLO-FOLLOWUP11](periods/10-01-04.md#solo-followup11) | 11项复核与原采集属于同一后续轮次。 |
| [H-RESEARCH-FULLSTATS-REVIEW](periods/10-01-04.md#h-research-fullstats-review) | 同轮资料 | [FULLSTATS21](periods/10-01-04.md#fullstats21) | 原派生报告与21项拆分条目不累计为两轮采集。 |
| [H-RESEARCH-NOVELTY-ASSESSMENT](periods/10-15-18.md#h-research-novelty-assessment) | 独立比较 | [H-INDEPENDENT-RESEARCH-DIRECTIONS](periods/10-15-18.md#h-independent-research-directions) | 后继比较D/I/A等方向；投稿模式下不恢复旧优先级。 |
| [H-RESEARCH-REAPPRAISAL](periods/10-15-18.md#h-research-reappraisal) | 排名撤回 | [H-RESEARCH-NOVELTY-ASSESSMENT](periods/10-15-18.md#h-research-novelty-assessment) | A内部发展不等于跨方向比较后最有希望。 |
| [H-RESEARCH-REPRIORITIZE](periods/10-15-18.md#h-research-reprioritize) | 实际交付 | [H-B1-STRIPE-COST](periods/10-15-18.md#h-b1-stripe-cost) | 旧尚未执行的B1成本卡已有后继交付。 |
| [H-SHARED-READ-BYTE-PROGRESS](periods/10-05-09.md#h-shared-read-byte-progress) | 强普通补充 | [H-SHARED-READ-RESIDENT-ADMISSION](periods/10-05-09.md#h-shared-read-resident-admission) | 普通WRITE提前准备／resident准入须纳入；局部加速不作为独有贡献。 |
| [H-SHARED-READ-EARLY-SPONSOR](periods/10-05-09.md#h-shared-read-early-sponsor) | 强普通补充 | [H-SHARED-READ-RESIDENT-ADMISSION](periods/10-05-09.md#h-shared-read-resident-admission) | 普通WRITE提前准备／resident准入须纳入；局部加速不作为独有贡献。 |
| [H-SHARED-READ-JOIN-BOUNDARY](periods/10-05-09.md#h-shared-read-join-boundary) | 后续完整对照 | [H-SHARED-READ-INFLIGHT-JOIN](periods/10-05-09.md#h-shared-read-inflight-join) | 后继真正加入成立，但同许可普通起点共享更快；失败与旧版本不抹去。 |
| [H-SHARED-READ-JOIN-PIPELINE](periods/10-05-09.md#h-shared-read-join-pipeline) | 后续完整对照 | [H-SHARED-READ-INFLIGHT-JOIN](periods/10-05-09.md#h-shared-read-inflight-join) | 后继真正加入成立，但同许可普通起点共享更快；失败与旧版本不抹去。 |
| [H-SHARED-READ-ONLINE-CHOICE](periods/10-05-09.md#h-shared-read-online-choice) | 后续完整对照 | [H-SHARED-READ-INFLIGHT-JOIN](periods/10-05-09.md#h-shared-read-inflight-join) | 后继真正加入成立，但同许可普通起点共享更快；失败与旧版本不抹去。 |
| [H-SHARED-READ-PERMISSION-INDEPENDENT](periods/10-05-09.md#h-shared-read-permission-independent) | 后续完整对照 | [H-SHARED-READ-INFLIGHT-JOIN](periods/10-05-09.md#h-shared-read-inflight-join) | 后继真正加入成立，但同许可普通起点共享更快；失败与旧版本不抹去。 |
| [H-SHARED-READ-RTL-PROGRESS](periods/10-05-09.md#h-shared-read-rtl-progress) | 后继 | [H-SHARED-READ-INFLIGHT-JOIN](periods/10-05-09.md#h-shared-read-inflight-join) | 在途加入功能后继；同许可普通起点共享更快。 |
| [H-SHARED-READ-SHORT-PREFIX](periods/10-05-09.md#h-shared-read-short-prefix) | 后续完整对照 | [H-SHARED-READ-INFLIGHT-JOIN](periods/10-05-09.md#h-shared-read-inflight-join) | 后继真正加入成立，但同许可普通起点共享更快；失败与旧版本不抹去。 |
| [H-SHARED-READ-SPONSOR-HANDOVER](periods/10-05-09.md#h-shared-read-sponsor-handover) | 强普通补充 | [H-SHARED-READ-RESIDENT-ADMISSION](periods/10-05-09.md#h-shared-read-resident-admission) | 普通WRITE提前准备／resident准入须纳入；局部加速不作为独有贡献。 |
| [H-SOURCE-DRAIN-HIT](periods/10-10-14.md#h-source-drain-hit) | 结论范围修正 | [H-SHARED-SERVICE-CONTROL-BOUNDARY](periods/10-15-18.md#h-shared-service-control-boundary) | 同付费接口策略等价不证明完整接口发表覆盖或全部原生成本等价。 |
| [H-STAGE117-PACKET-PROTOCOL-ADAPTER](periods/10-10-14.md#h-stage117-packet-protocol-adapter) | 证据增强 | [H-STAGE117-REAL-CACHE-HANDOFF](periods/10-10-14.md#h-stage117-real-cache-handoff) | uncached假设不能代替真实缓存；后继接入WT端口。 |
| [H-STAGE117-R2-PLATFORM-FIRST](periods/10-10-14.md#h-stage117-r2-platform-first) | 来源修订 | [H-STAGE117-R3-ASTRA](periods/10-10-14.md#h-stage117-r3-astra) | dmafwd原地TX对照不足后改为B预发布buffer的vm2vm合同。 |
| [H-STAGE117-REAL-CACHE-HANDOFF](periods/10-10-14.md#h-stage117-real-cache-handoff) | 后继 | [H-STAGE117-DELIVERY-G0](periods/10-10-14.md#h-stage117-delivery-g0) | 六案观察到消费重叠，不能沿早期未观察概括全部阶段。 |
| [H-STAGE117-THREE-RESULTS](periods/10-10-14.md#h-stage117-three-results) | 关口修正 | [H-STAGE117-PLAN-REVISION-G0](periods/10-10-14.md#h-stage117-plan-revision-g0) | 功能满足不等于并发性能已否定，后继四案G0实际比较。 |
| [H-STAGE118-C117-INTERFACE-REACHABILITY](periods/10-10-14.md#h-stage118-c117-interface-reachability) | 来源修订 | [H-STAGE117-R2-PLATFORM-FIRST](periods/10-10-14.md#h-stage117-r2-platform-first) | 图像接口需适配后改做新来源，未实施旧stage119。 |
| [H-WORKFLOW-STREAM-CONTRACT](periods/10-10-14.md#h-workflow-stream-contract) | 模型后继 | [H-WORKFLOW-STREAM-G0-RESULTS](periods/10-10-14.md#h-workflow-stream-g0-results) | 源语义和私有计算桥RTL投影分开，不冒充native性能。 |
| [H-COMPOSITION-HANDOFF](periods/10-05-09.md#h-composition-handoff) | 后续终态 | [COMPOSITION-PTWFIX](periods/10-05-09.md#composition-ptwfix) | 恢复构建失败之后另有修复构成与fresh172；不改原失败。 |
| [H-COMPOSITION-ROUTING-COST](periods/10-05-09.md#h-composition-routing-cost) | 后续构建 | [H-COMPOSITION-TRIM](periods/10-05-09.md#h-composition-trim) | 旧running/拥堵记录后有裁剪构成的时序验收。 |
| [H-DS0-BATCH-RESULTS-REVIEW](periods/07-09.md#h-ds0-batch-results-review) | 复测 | [H-DS50-RETEST-RESULTS-REVIEW](periods/07-09.md#h-ds50-retest-results-review) | 独立回切批次，不计成原DS0重复。 |
| [H-MECHANISM-DESIGN-HANDOFF-20261009-ROUND02](periods/10-05-09.md#h-mechanism-design-handoff-20261009-round02) | 后续判断 | [H-MECHANISM-DESIGN-HANDOFF-20261009-ROUND05](periods/10-05-09.md#h-mechanism-design-handoff-20261009-round05) | 普通early-bind+pin／checkpoint与部分cache等覆盖具体条件，未形成已支持独立贡献。 |
| [H-MECHANISM-DESIGN-HANDOFF-20261009-ROUND03](periods/10-05-09.md#h-mechanism-design-handoff-20261009-round03) | 后续判断 | [H-MECHANISM-DESIGN-HANDOFF-20261009-ROUND05](periods/10-05-09.md#h-mechanism-design-handoff-20261009-round05) | 普通early-bind+pin／checkpoint与部分cache等覆盖具体条件，未形成已支持独立贡献。 |
| [H-MECHANISM-DESIGN-HANDOFF-20261009-ROUND04](periods/10-05-09.md#h-mechanism-design-handoff-20261009-round04) | 后续判断 | [H-MECHANISM-DESIGN-HANDOFF-20261009-ROUND05](periods/10-05-09.md#h-mechanism-design-handoff-20261009-round05) | 普通early-bind+pin／checkpoint与部分cache等覆盖具体条件，未形成已支持独立贡献。 |
| [H-NONBLOCK-PILOT-PLAN](periods/10-01-04.md#h-nonblock-pilot-plan) | 后续终态 | [N-V5](periods/10-01-04.md#n-v5) | v0–v5实际失败、构建及板测单列；旧构建中状态不覆盖最终结果。 |
| [H-PAPER-LOGIC](periods/10-01-04.md#h-paper-logic) | 证据等级修正 | [CORRECTION-E3-TIMING](periods/10-15-18.md#correction-e3-timing) | 旧E3概括需附未闭合时序限制；历史原文不重写。 |
| [H-PAPER-SCENARIOS-MITIGATION](periods/10-01-04.md#h-paper-scenarios-mitigation) | 证据等级修正 | [CORRECTION-E3-TIMING](periods/10-15-18.md#correction-e3-timing) | 旧受控证据摘要不能无条件当时序验证后的定量因果。 |
| [H-RESEARCH-FULLSTATS-V1](periods/10-01-04.md#h-research-fullstats-v1) | 实际执行 | [FULLSTATS21](periods/10-01-04.md#fullstats21) | 完整分组统计的实际21项与交付文件分别定位。 |
| [H-RESEARCH-N-PROTOTYPE-SPEC](periods/10-01-04.md#h-research-n-prototype-spec) | 实现结果 | [N-V2](periods/10-01-04.md#n-v2) | v2资源失败是实际后继，不将规格当通过。 |
| [H-RESEARCH-N-PROTOTYPE-SPEC](periods/10-01-04.md#h-research-n-prototype-spec) | 后续终态 | [N-V5](periods/10-01-04.md#n-v5) | v3–v5 RAM/诊断/资格修订与实际板测分别保存。 |
| [H-RESEARCH-ROUTE-AUDIT](periods/10-10-14.md#h-research-route-audit) | 同文资料 | [H-RESEARCH-ROUTE-AUDIT-20261010-SUPPLEMENT](periods/10-10-14.md#h-research-route-audit-20261010-supplement) | 已有历史核查指出正文同文；不是两个独立实验。 |
| [H-RESEARCH-TWO-BIT-V2](periods/10-01-04.md#h-research-two-bit-v2) | 实际执行 | [H-E2-RESULTS-REVIEW](periods/10-01-04.md#h-e2-results-review) | 交付后role1新版64已板测。 |
| [H-RESEARCH-TWO-BIT-V2](periods/10-01-04.md#h-research-two-bit-v2) | 实际执行 | [H-METHOD-RESULTS-REVIEW](periods/10-01-04.md#h-method-results-review) | 交付后role2探索462已执行。 |
| [H-TWO-BIT-BUILD-ACCEPTANCE](periods/10-01-04.md#h-two-bit-build-acceptance) | 实际执行 | [H-E2-RESULTS-REVIEW](periods/10-01-04.md#h-e2-results-review) | 构建验收不替代后继E2实际运行。 |
| [H-TWO-BIT-BUILD-ACCEPTANCE](periods/10-01-04.md#h-two-bit-build-acceptance) | 实际执行 | [H-METHOD-RESULTS-REVIEW](periods/10-01-04.md#h-method-results-review) | 构建验收不替代后继方法探索实际运行。 |
| [H-WORKFLOW-STREAM-PREFILTER](periods/10-10-14.md#h-workflow-stream-prefilter) | 来源补齐 | [H-WORKFLOW-STREAM-CONTRACT](periods/10-10-14.md#h-workflow-stream-contract) | HERO实际双缓冲语义后继补齐，不沿早期缺block尺寸概括全部。 |
| [EARLY-REMOTE-PTW](periods/07-09.md#early-remote-ptw) | 补充 | [H-E1-RESULTS-REVIEW](periods/07-09.md#h-e1-results-review) | 后续独立V窗口替代早期公共窗口的研究用途，不追认旧运行身份。 |
| [ISPASS-G1](periods/10-15-18.md#ispass-g1) | 独立新基线 | [ISPASS-BASELINE100](periods/10-15-18.md#ispass-baseline100) | 9次历史复现和100次新基线分别计数。 |
| [ISPASS-BASELINE100](periods/10-15-18.md#ispass-baseline100) | 扩展 | [ISPASS-DS-SWEEP](periods/10-15-18.md#ispass-ds-sweep) | 复用DS0/DS50，仅新增DS25/DS100共100次。 |
| [ISPASS-BASELINE100](periods/10-15-18.md#ispass-baseline100) | 离线补充 | [ISPASS-BASELINE100-EXTRA](periods/10-15-18.md#ispass-baseline100-extra) | 合计吞吐、路径和分boot CI重用原100次，无新板测。 |
| [RELATED-G2](periods/10-15-18.md#related-g2) | 主张限制 | [CORRECTION-E3-TIMING](periods/10-15-18.md#correction-e3-timing) | 前作比较不能补齐E3的时序闭合；有条件通过不是贡献充分证明。 |
| [RESEARCH84](periods/10-01-04.md#research84) | 功能修正 | [SOLO-FOLLOWUP11](periods/10-01-04.md#solo-followup11) | Solo参数在发DMA前拒绝，最小适配后独立新boot11项。 |
| [RESEARCH84](periods/10-01-04.md#research84) | 后继 | [H-E2-RESULTS-REVIEW](periods/10-01-04.md#h-e2-results-review) | 最终新版64项提供专门替换链，不把84失败改成通过。 |
| [FULLSTATS21](periods/10-01-04.md#fullstats21) | 后继 | [H-METHOD-RESULTS-REVIEW](periods/10-01-04.md#h-method-results-review) | 信息差异之后检验反馈控制，不由H-M0推H-M1。 |
| [N-V0](periods/10-01-04.md#n-v0) | 功能修订 | [N-V1](periods/10-01-04.md#n-v1) | 读守卫实现后仍需端到端正确性。 |
| [N-V1](periods/10-01-04.md#n-v1) | 后继 | [N-V2](periods/10-01-04.md#n-v2) | v1功能拒收后改完整缓冲；v2资源失败。 |
| [N-V2](periods/10-01-04.md#n-v2) | 实现修正 | [N-V3](periods/10-01-04.md#n-v3) | 局部RAM解决布局；不补写v2成功。 |
| [N-V3](periods/10-01-04.md#n-v3) | 观测补充 | [N-V4](periods/10-01-04.md#n-v4) | 增加MSI兼容和candidate/probe计数。 |
| [N-V4](periods/10-01-04.md#n-v4) | 资格修正 | [N-V5](periods/10-01-04.md#n-v5) | 受限4KiB资格真实触发，但性能不稳定。 |
| [COMPOSITION-FIRST](periods/10-05-09.md#composition-first) | 功能修正 | [COMPOSITION-DSFIX](periods/10-05-09.md#composition-dsfix) | DS写归属修复后另暴露PTW合法叶错误。 |
| [COMPOSITION-DSFIX](periods/10-05-09.md#composition-dsfix) | 功能修正 | [COMPOSITION-PTWFIX](periods/10-05-09.md#composition-ptwfix) | 新boot172全过，旧失败保留。 |
| [COMPOSITION-PTWFIX](periods/10-05-09.md#composition-ptwfix) | 新构成 | [COMPOSITION-CLEAN](periods/10-05-09.md#composition-clean) | 裁剪观测后独立clean172验收。 |
| [COMPOSITION-CLEAN](periods/10-05-09.md#composition-clean) | 干预 | [COMPOSITION-ISOLATED](periods/10-05-09.md#composition-isolated) | 分区历史隔离去掉miss数差，仍有PN退化。 |
| [COMPOSITION-ISOLATED](periods/10-05-09.md#composition-isolated) | 供给对照 | [COMPOSITION-GAP18](periods/10-05-09.md#composition-gap18) | 短供给间隔对照量化敏感性。 |
| [COMPOSITION-ISOLATED](periods/10-05-09.md#composition-isolated) | 模型解释 | [PN-ACTUAL-DMA](periods/10-05-09.md#pn-actual-dma) | 行为DDR提供一条反馈链，非完整板级定量解释。 |
| [PN-ACTUAL-DMA](periods/10-05-09.md#pn-actual-dma) | 测量合同修正 | [H-WORKFLOW-G0-M0-RESULTS](periods/10-10-14.md#h-workflow-g0-m0-results) | 共同就绪与双方完整任务避免V完成即STOP A。 |
| [B1-PLRU-CAUSAL](periods/10-05-09.md#b1-plru-causal) | 独立后继 | [H-PLRU-ABLATION](periods/10-15-18.md#h-plru-ablation) | 10月6局部合成前史与10月17实际DMA masked偏置不同，不能混算。 |
