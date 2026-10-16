# 论文平台当前状态（精简版）

## §84 当前：部分目的写许可下的长读准入（2026-10-09）

- 正向能力：首次VALID后不可撤的长源读，在冻结K个消费者且各自取得成功源读许可、连续目的写前缀Ci、真实私有返回容量Qi时，若每个消费者Ui=N-Ci≤Qi，可在首个ARVALID前整包准入；未授权尾段只进各自私有缓冲，待各自下一页写权限到达后写出。准入前可以主动分段。
- 读写几何：source256字与dest253+3只在decouple_rw=1且deburst=0下成立；decouple_rw=0会按读写共同边界把source切短，是现成普通shared/split对照来源。当前production RAW_COUPLING_AVAIL=0，不能把旧firstRcoupler说成已启用；若新使能coupler，一个source firstR只提供一个AW release credit，两个AW需要显式实际release设计，不能仅凭任务几何记录闭合。
- 安全边界：单流Ui≤Qi只说明普通缓冲可实现的安全边界，不能因为旧自制sponsor原型阻断就判断文献上的创新。剩余具体问题是所有消费者仅有部分目的写许可、独立prefix drain/真实所有接口，以及同等强共享分片/整段提前准备的总成本。
- 执行证据：Python有限状态6图190 states；唯一故意Ui>Qi 的停滞状态(r8,w5,pfalse)，其余5完成；oracle/edge/witness检查exit0。详细分图状态计数见结果文件。
- B独立卡：正例C5/6、Qi3已手推ACK22/24，payload新增0仅使用手例现有私有槽；旧groups barrier不能自动归因该特例，因为两侧prefix child count都是2。普遍heterogeneous C2/6、Qi6/2反例说明independent drain必要；
- 成本与范围：额外control、广播/准入/失效/错误/时序另计；当前production DMA0/1 FIFO512，others3，小Q共享原型是前瞻配置。

链接：
- [接受](iommu-platform/out/innovation-permission-frontier-20261009/root-result-acceptance.json)
- [共享设计](iommu-platform/out/innovation-permission-frontier-20261009/root-shared-frontier-design.json)
下一步：核所有消费者仅具部分目的写许可时的共享长读前作与完整成本；有具体差异才进入RTL。多流结论目前是推导/手例；本轮没有新RTL、Linux、构建或上板动作，最新实际DUT仍为§76–78。

## §83 当前：流程收敛，推进不可撤写承诺的欠账上限（2026-10-09）

- 已按用户要求改进：先新增能力/准确前作/完整强对照/主线可达性/总成本；root亲自设计与接受，两个必要Codex席并行机制，一DeepSeek定向证据。不维持无任务席数，不先扩模型矩阵。
- 正向规则：first AWVALID前固定L≤r+Q，r为专属真实resident前缀、Q3；任意source-starved当前child真债≤3。恢复先真写至少1拍才可再让路，每child≤3转换/≤6null/≤4AW；不代表全job常数或墙钟界。
- root证得三槽resident-only若在peer释放槽之前完成12拍至少4AW，候选手例可2AW；但B追加一个无oracle的adaptive streaming普通控制，normal A17/2AW vs候选18/2AW，gap A114/B11/C108/0null vs候选114/13/108/3null，均3AAW。原两手例不能证明独有性能；下一针对AW已固定后才突然断供的统一欠账能力。
- 新突发gap手例已root核：普通second AW9固定后才断供，真债6，A107/B110；候选first AW6闭合，A110/B14。两边均总3AW/B、真R/W15、无null；是A慢3/peer早96的条件取舍，非运行cycles/全部普通最优对比。
- C完整普通pretranslation＋既有first-R coupler＋one primer逐动作复现，停止其调度创新试验，保留为共同组件。1247新增bit仅字段草案、未含完整失效/路由/时序成本，687复用未证明；生产DMA0/1仍512、其余3，四小flow不是当前实测配置。
- 接口关键：须不claim W地准备整accepted source-return envelope的own WRITE；当前child局部PA不够。child/patch真LAST须在全部W锁之前，保留许可至真写/B结算；native prepare-origin/response与可信PA路径当前不存在，不能用ATS标志或删read gate假装已有。
- CutForward2022、MMU-aware DMA2018、Arm LTI IssueE2026和2014 QoS继承关键原文已root核。generic pretranslation/retained region/parent replay不是独立贡献；如果统一欠账能力同成本未被准确前作覆盖，并体现实际翻译干扰收益，才推进论文/RTL。
- 本轮仅源核、设计与手推；新模型/checker/DUT/RTL/Linux/build/BIT/Mac/SD/JTAG/板卡动作0，最新actual DUT仍§76–78。DS首调代理失败、一次已审联网恢复成功，flash/high/max393216、stop未截断，实际成功provider130907tokens；无Claude。

[§83根接受](iommu-platform/out/innovation-mechanism-focus-20261009/root-result-acceptance.json)／[新增能力与实际接口合同](iommu-platform/out/innovation-mechanism-focus-20261009/root-capability-and-interface-contract.json)／[B规则](iommu-platform/out/innovation-mechanism-focus-20261009/b-positive-design.json)／[C共同组件](iommu-platform/out/innovation-mechanism-focus-20261009/c-mainline-positive-design.json)。下方为历史。

## §82 当前：18组完整发送模型通过，保留有界补写判别题（2026-10-09）

- root亲自设计/核最强前作与源代码，两个Codex关键席并行A结构/B在线成本；DeepSeek提供接口证据、普通模型和一次限定修正。未重跑旧RTL矩阵。
- A具体普通共享tag/PTE两槽166b（同观察168b）在172b动态行内提供同leaf/path与独立权限；专用S-only102→164是+62b/行。已核经典complete-subblock、Unified-TP及Rocket固定源码。这版无独有存储能力，停止模型/RTL。
- B六场景×long/tailguard/recourse＝18实际Python比较；18事件重放、18事先root手推oracle一致、5意义测试通过。常态无peer候选A25/AW1 vs普通28/AW2；尾长gap候选A/B107/28 vs普通107/26且候选多3null，反复gap145/28 vs145/26。是在线开销/等待取舍，非全面性能创新；抽象时刻不是RTL cycles。
- 新root规则：stream patch必须先完成至少一拍真W才允许再次让路，真实未写量严格降；初尾Q3最多3转换/6null、含原AW最多4个A AW/B，旧B结算后才复用patch metadata。仅设计/手推，未实现；下一对比动态尾stream及逐尾resident单拍的完整多peer成本，并核正常成功suffix continuation最近前作。
- 仍以共享IOMMU翻译干扰为主线；fixed source/h0模型不证明译址与数据路径因果。若只通用AXI尾调度改善，不能直接当主线贡献。新production RTL/Linux/DUT/build/BIT/Mac/SD/JTAG/板卡动作均0，最新实际DUT仍§76–78。
- 原模型重复cycle报错/正常B误判原稿保留，限定修正后执行exit0；不是DUT故障。三DS calls flash/high/max393216/stop未截断，实际provider252450tokens；无Claude。

[§82根接受](iommu-platform/out/innovation-recourse-cost-20261009/root-result-acceptance.json)／[18组结果](iommu-platform/out/innovation-recourse-cost-20261009/model-results.json)／[新规则合同](iommu-platform/out/innovation-recourse-cost-20261009/root-monotone-recourse-next.json)。下方为历史。

## §81 当前：六方向并行筛选完成，优先核写尾部继续的能力/成本（2026-10-09）

- 用户要求增加不同方向agent。实际扩为root＋三Codex架构席、两DeepSeek独立提案worker：A缓存结构、B多在途/W债、C失效重映射、D最少接口、E性能保证、F应用布局。六方向不等于六Codex自主agent；本会话native最多四席含root，无Claude。
- A原手例candidate10读/greedy16，但固定PATH-MRU也10、首叶+bypass9；同格式two-hit逐状态匹配。Unified-TP已有统一leaf/PTC及动态驻留；停止reuse算法线，只保留shared双证据行的完整成本未知。
- B提出mux之前per-DMA shim真实null尾排空＋原source R保留＋真实suffix补写，原user B不提前。postcommit longgap手算104/7 vs shared-pool104/108；从任务开始普通adaptive-prefix104/6且少3null，同AW数。新动作不等于独有整体收益；下一只核主动正常成功partial replay前作与完整在线成本。
- C叶级BBM续走同state/通知/PTE端口的partial-cache控制精确重现19，39是弱root-restart；D物理/逻辑完成接口为B成本组件，普通replay ledger可表达，不另算创新。E credit/checkpoint、F demotion/alias两版各停止，root纠正phase-index漏数据与不同context仍共享容量两处错误。
- 本轮无模型/DUT/RTL/Linux/build/BIT/Mac/SD/JTAG/板卡动作，所有新数字是手推。四DS调用flash/high/max393216、stop/未截断，实际provider357381tokens；root亲自D设计/关键原文源核/六方向接受。原稿、修订、负对照和专利原文保存，最新实际copy仍§76–78。

[§81六方向审定](iommu-platform/out/innovation-six-directions-20261009/screen-verdict.json)／[并行合同](iommu-platform/out/innovation-six-directions-20261009/six-direction-contract.json)／[B候选及强对照](iommu-platform/out/innovation-six-directions-20261009/b-multi-route-obligations.json)。下方为历史。

## §80 当前：120项完整小模型比较，三版停止RTL（2026-10-09）

- 三个机制设计席（root C、独立A/B）＋必要DARTS源码席；DeepSeek并行普通实现/强对照及各一次限定修订，root亲自设计、核源、执行和判断。停止已有具体复现的任务，不靠维持agent数量推进。
- A同合同O-ring复现payload保留至实际W与一次晚加入；B普通current-W-head admission barrier逐拍复现热7/8、冷9/10。均为推导，B旧compact source门不可达；不再派模型或RTL。
- C事件模型12测试通过、72基础＋48强普通＝120最终比较，六场景×两profile，完整copy/ACK、容量与LRU/own静态许可/同overlap均核。每个候选完整ACK曲线及walk次数都有具体普通控制复现；不是一个万能在线selector的证明。
- 八组L10 serial候选meanACK61.875/makespan127/walk11，group-first或fixed-k3为56.75/107/9；早期三组先完成掩盖后续重复翻译。B2收益被warm-first精确追平76.625/148/13；L1收益被固定k2(serial)/k1(overlap)精确追平。
- 原测试导入/LRU遗漏及强对照36invalid/旧不完整all_ok记录保留，限定修正后48合法/0invalid；不算DUT故障。DARTS核到固定2025作者代码，公式/消费释放已有，双资源映射仍有确切缺口；派生proxy不冒充原实现。
- 下一先换明确新能力/完整成本假设，核独立许可、不可撤W/B、多route的真实交互与具体强对照；有正例才RTL。toy source-cache1/2＋热目标不是当前16-entry combined PLRU或应用效果。四flash/high/max calls实际provider325650tokens；无Claude、新DUT/build/生产/Linux/BIT/上板/SD动作，最新实测仍§76–78。

[§80根接受](iommu-platform/out/parallel-innovation-model-20261009/root-result-acceptance.json)／[完整候选对照](iommu-platform/out/parallel-innovation-model-20261009/c-comparison-verdict.json)／[DARTS源码判定](iommu-platform/out/parallel-innovation-model-20261009/darts-critical-verdict.md)。下方为历史。

## §79 当前：三方向并行筛贡献，优先核消费者闭包（2026-10-09）

- 用户确认新方式：先能力增量/最强前作/具体简单对照；2–3方向同时推导、反例或简模；有差异才RTL。已固化根AGENTS，根Codex设计A、两个关键席独立设计B/C，三DeepSeek证据任务实际并行完成。
- A晚到共享读：root源核单skid/单logical-R串行交V/A，不能保证任意合法响应下V零额外等待；J/H取回下界仅条件计数，需计全部可重放payload。保留授权/义务的架构成本问题，不新修late/continuous夹具。
- B译址兑现：写承诺/deadline草案在两任务固定W序合同内被普通next-W-owner lookahead同槽同轨迹复现；交换推导已根核，当前草案停止，不进RTL。未来只查共享DDR同时改变walk/排空机会的真实反馈。
- C正向候选：按共享翻译key（不同payload）形成可完成/ACK归还的有限消费者闭包。四组手算平均ACK31.25→29.75（4.8%），makespan同39；普通hot-page追平该例。扩为8组/15desc后，明确安全hot-page截至t28最多1 ACK，候选3；只为纸面早期完成差异，未证明全8组整体收益。
- root拦住原稿提前借未来ACK的容量，关键席一次限定修正，旧稿保留；无DUT失败/模型运行。DARTS2025已有少量transfer解锁many tasks，全文/实现决定门未核；不同payload共享译址setup、WRITE/B及consumer ACK双寿命的确切增量与真实多DMA场景是下一核查重点。
- 下一：定向核DARTS代码/原文＋同等prepare/overlap/目标WRITE成本的完整小模型及真实应用证据；有独有动作/成本收益才RTL。三flash/high calls实际provider tokens406691，全部max393216/stop/未截断；无Claude。新DUT/build/production/Linux/BIT/Mac/SD/上板均0，最新实测仍§76–78。

[§79根筛选](iommu-platform/out/parallel-innovation-screen-20261009/screen-verdict.json)／[C机制](iommu-platform/out/parallel-innovation-screen-20261009/c-codex-design.md)／[实际worker记录](iommu-platform/out/parallel-innovation-screen-20261009/worker-record.json)。下方为历史状态。

## §76–78 当前：真实在途加入通过，强普通更快（2026-10-09）

- §76一SV/7PASS、§77一SV+一C++增量/8PASS、§78三SV/14PASS：合计6成功build步骤、29真实完整copy PASS；不是整个研究历史总量。actual audit76/audit77_78 exit0，六source8+exe快照、R/W/CSV/LOG核过，旧记录不删。
- 模型修复：RR旧指针在idle探测时前进，interval64隐藏catch-up窗口；改实际R handshake提交，held VALID及同ID顺序保留。旧PASS仍支持字节/协议检查，旧interval>0零窗与公平服务结论须限定旧模型；未宣称interval0全部逐字回归。
- §77三项实际4周期宽窗口；§78已真正绑定，同沿req11、退short/禁A fallback、留旧V E/LAST、新W共同网格/peer门。5次actual join（E64/63/256），R257=N256+J1，0新payload，局部1functional FF/9observer FF，未综合。
- 主结果：join V19462/A19465、R257；弱普通 V23492/A23499、R320（平均低17.1607%）；强普通许可已齐起点共享 V19402/A19411、R256，反而更快。其余4组强普通也胜；因此这组17%不能称独立性能算法。许可迟到run13/14均无join/startup，V23244/A23246/R320，CSV/指标逐字相同。
- 关闭新join的§78run02与§77run04 CSV/旧指标回归一致；§76与§75三组pure-observer对照亦逐字同。脚本初稿错误及实际诊断保留，修审计没重跑RTL。
- 下一创新门：真实late提交/晚授权＋热/冷翻译/连续作业，同信息/总资源比较普通pending merge、等待窗、分片和有限history；首拍已交付后8B skid可能仍保留值，J1必须补普通保留响应复用对照（源码推导，未实现/测）。连续作业/epoch、remap/fault及held-physical-R绑定沿尚未测。
- Codex亲自设计、复杂诊断/关键源核/实际执行接受；DeepSeek纯观测/C++验收/普通audit提案及限定修订，6flash/high calls实际provider tokens836285，无Claude。out-only，无production/Linux/BIT/板卡/SD动作。

[§76–78根接受](iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md)／[§78账本](iommu-platform/out/shared-read-inflight-join-20261009/execution-ledger.json)／[下一创新门](iommu-platform/out/shared-read-inflight-join-20261009/codex-next-novelty-gate.md)。下方状态为历史。

## §75 当前：短补读夹具通过，更宽加入门待实测（2026-10-09）

- out-only四build成功、29DUT执行：27完整copy PASS＋run17/21两旧route-not-bound终端拒绝；不是watchdog。修复E=N（无共享尾段）立即绑定独立后run22/23 PASS，R512；7非法CLI另算。
- actual audit75 exit0：27份CSV AR/rfirst/rlast和真实R握手counter、W各256、logical r/W各2048、四source8+exe快照SHA核对。两修复回归＋上一阶段对照CSV逐字一致，旧metrics相同（SOURCE只四新driver字段）。CSV不含逐拍pr；末个raw父长度不是总作业长度，F1末A父255/整A256分开。
- 已实现普通A[0,J)短补读＋正常fallback，V不等A；18次成功short条件严格 rV=rA=rawWV=rawWA=J<E 窗口均0（J8时V已46）。没有实现在途加入，不能由0否定更宽窗口。
- 主线下一门槛：own许可/PA/属性相同、双方logicalR同T、旧W承诺≤T且payload已齐时，rawW可落后；原子绑定要退旧short状态、禁A fallback、保留旧V E/LAST和新W网格/peer门。宽门是待实现合同，具体错误接法死锁反例已手推，非现版DUT故障。
- 补读后合流/速度追赶/有限buffer在经典Patching/piggyback/HMSM已有；如果新门同完整成本不增加前作/强对照能力，就不能称创新；有真实增量才能形成机制贡献。源码新增1 functional FF/134 observer FF/0 payload，未综合；无production/Linux/BIT/board/SD动作。
- Codex亲自创新设计、关键源核/诊断/执行/接受；DeepSeek驱动和验收脚本提案/限定修订，四次flash/high调用实际provider tokens合计532557；无Claude。本段为最新状态，下方夜间状态保留为历史。

[§75根接受](iommu-platform/out/shared-read-short-prefix-20261009/codex-source-acceptance.md)／[机制合同](iommu-platform/out/shared-read-short-prefix-20261009/codex-mechanism-verdict.md)／[验收账本](iommu-platform/out/shared-read-short-prefix-20261009/execution-ledger.json)。

## §70–74 夜间任务完成（2026-10-09 08:00 JST）

- §70：22/22完整copy PASS；带宽受限时共享整体耗时降低约19%–39%，但部分条件V变慢。普通multicast解释少读收益，不能单独称独立算法。
- §71：50次执行40完整copy PASS＋10actual-cutover严格拒绝；两在线规则首批22项选择相同，无新增优势。odd-E/B候选环是关键席手推，定向RTL未复现，不能称DUT故障。
- §72：48次执行40PASS＋8初版真实watchdog；根因是独立A suffix继续依赖已完成V的READ有效项，改为ownA授权后最终24/24PASS，16旧成功CSV/指标逐字回归。4旧/4新版本快照、全ledger已审定。
- §73：30/30PASS；V闭合自身数据债/独立继续不必等A全授权；共享仍核双方真实许可和PA/attrs。计数器改为实际AR握手，4定向CSV/指标回归一致。长预算V慢约53.4%，不是性能创新。
- §74：20/20真实完整copy PASS；最终3条件确认A AR在rawV W闭合前接受，但只省12/14/12cycles（约0.13%），普通流水线解释。最终20份CSV/6build与旧新source/exe快照全部审定，3次早AR是真实握手；8项CSV/原指标逐字回归。
- 最后五阶段合计170DUT执行：152完整copy PASS、8旧watchdog、10严格拒绝；非整个夜间总量，非统计重复/上板。没有production/Linux/BIT/JTAG/SD/Mac动作。
- 创新下一判别：先观察A能否在旧V physicalR仍在途时追到J<E，再验证未来R安全加入，理论N+E→N+J。此机制尚未实现或DUT测试；如果强普通late-read-merge同成本重现，就不能称创新；如果在独立许可/不可撤R/W/B/小buffer下给出额外真实收益，才可成为贡献。
- 已按约定持续探索到08:00 JST并结束本轮；新研究动作等待下一次继续指令。DeepSeek提案/普通验收脚本；Codex亲自机制设计、复杂诊断、源核、执行/接受；无Claude。

[晨报](iommu-platform/docs/research-morning-20261009.md)／[§70](iommu-platform/out/shared-read-memory-service-20261009/codex-result-acceptance.md)／[§71](iommu-platform/out/shared-read-online-choice-20261009/codex-result-acceptance.md)／[§72](iommu-platform/out/shared-read-deferred-binding-20261009/codex-result-acceptance.md)／[§73](iommu-platform/out/shared-read-permission-independent-20261009/codex-result-acceptance.md)／[§74接受](iommu-platform/out/shared-read-overlap-closure-20261009/codex-result-acceptance.md)。

## §64–69 当前状态（2026-10-09 夜间，约06:47 JST）

- §64普通ordered-prefix额度：15copy PASS、全部终点metrics与§63相同，仅5个CSV提早发AW，不能据此主张独立性能算法。
- §65：38copy PASS，奇数E的既有promise保护与短W闭包修复；§66：47copy PASS＋6旧夹具真实死锁保留，Max2 issued-group由相等改≤后六项全完成。普通两B流水线解释压力约19%的改善。
- §67：30copy PASS＋2actual要求拒绝留存，6Max1逐字回归；同两B下minimum-J对burst末端仍有约1%–4%的配置收益，但未据此确认独立创新。
- §68正向新候选：先闭合不可撤W数据债，再让旧V tail与A prefix同时在途，单skid/privateB3不加payload；18copy PASS，13实测AR-before-oldRLAST；E256三项属于普通双独立读、zero sharedsuffix。
- §69强普通并行独立读：15copy PASS，4候选CSV/metrics逐字回归。E160共享R416 vs独立R512少18.75%，但V4550 vs3883慢17.2%；压力V9824 vs7800慢25.9%、A9833相同。因此当前不能把并行补读或少读流量当独立性能创新。下一检验带宽压力与明确保护V进度的在线共享规则，并核经典/最新前作。
- 本轮§65–69合计156新执行：148完整copy PASS、6真实旧死锁＋2actual要求拒绝，全部保留；每个PASS的实际R/W拍数与完整bytes已核，source/可执行快照及SHA ledger已保存。行为DDR真实RTL仿真，非上板/综合。
- 夜间目标到08:00JST仍active；无production/Linux/BIT/JTAG/SD/Mac/board动作。Codex亲自设计、诊断、执行和接受；DeepSeek独立提案/条件审核，原提案错误及修订保留；无Claude。

[§65接受](iommu-platform/out/shared-read-join-boundary-20261009/codex-result-acceptance.md)／[§66接受](iommu-platform/out/shared-read-write-pipeline-20261009/codex-result-acceptance.md)／[§67接受](iommu-platform/out/shared-read-join-pipeline-20261009/codex-result-acceptance.md)／[§68接受](iommu-platform/out/shared-read-parallel-repair-20261009/codex-result-acceptance.md)／[§69接受](iommu-platform/out/shared-read-independent-repair-control-20261009/codex-result-acceptance.md)。各目录execution-ledger.json和critical稿。

## §52–56 当前状态（2026-10-09 夜间推进）

- §52早恢复保留cohort：8份真实完整copy PASS；早恢复平均耗时降33%，A约减半但V增加4.35%，明确tradeoff。长暂停与旧策略同轨迹；普通取消不必要退组规则，不能据此命名新算法。[验收](iommu-platform/out/shared-read-rejoin-20261009/codex-result-acceptance.md)。
- §53已有B3私有FIFO允许有限先读：1 build/12 copy PASS，12CSV独立sourceR/targetW计数一致；早恢复约0.6%改善，普通q256仍与长读完全同轨迹，停止独立贡献主张。[验收](iommu-platform/out/shared-read-lookahead-20261009/codex-result-acceptance.md)。
- §54跨页16 copy PASS；强普通prefix/suffix同R384，仅多一AR、差19cycles，B64/背压耗时同，停止独立算法主张。[验收](iommu-platform/out/shared-read-permission-frontier-20261009/codex-result-acceptance.md)。
- §55权限末拍取得W承诺，2build/8copy PASS，实际cross-ID R交错2–3次。普通同样提前准备后只差33cycles，交错收益不能据此命名独立创新。[验收](iommu-platform/out/shared-read-frontier-closure-20261009/codex-result-acceptance.md)。
- §56两build/9完整copy PASS＋旧F1死锁1次实测：实际R覆盖firstVALID W数据债后清futureR成员、保留旧W/B。强普通共同R/W切分也消环，F1平均6153.5比候选6469更低，不能据此立性能创新。[验收](iommu-platform/out/shared-read-data-obligation-20261009/codex-result-acceptance.md)。
- §57补读接力1SVbuild+1C++增量/28完整copy PASS+1actual未触发拒绝；F1短K16接力平均5741 vs共同切分6153.5，低6.70%，但PTE1000接力比旧长R整体更差，有tradeoff。新增11功能位+9观测位/0payload，未综合，不据6.7%宣称独立创新。[验收](iommu-platform/out/shared-read-sponsor-handover-20261009/codex-result-acceptance.md)。
- §58强普通next-WR预取1SV+1C++增量/46完整copy PASS，46CSV独立拍数核对。F63以上多数预取超过接力；F3/16接力均值占优但多读64拍，压力F1只差约0.51%。激活额外WRITE recipe，非等综合面积。下一检验V独立早启动＋有条件接力，普通独立策略也获同预取。[验收](iommu-platform/out/shared-read-early-write-prefetch-20261009/codex-result-acceptance.md)。
- §59 27新执行26copy PASS＋F255真实死锁1次留存；最终build03 8/8 PASS。F1/K2均值低7.83%但R298 vs256/V较独立慢19.31%。phase3末拍parking修复已授权AW/B循环，接力仍是有条件候选。下一§60字节进度W准入强简单挑战已构建/driver准备。[接受](iommu-platform/out/shared-read-early-sponsor-20261009/codex-result-acceptance.md)。
- §60/61各10copy PASS，普通resident在q2追平peer。§62三构建/37copy PASS，增大Q出现差距，但§63标准capacity-envelope一构建/15copy PASS、11对CSV逐字同，能解释peer收益，停止独立算法主张。§64已完成15copy PASS，终点metrics均与§63相同；当前进展见上方§64–69。[§62接受](iommu-platform/out/shared-read-quantum-credit-20261009/codex-result-acceptance.md)／[§63接受](iommu-platform/out/shared-read-credit-envelope-20261009/codex-result-acceptance.md)。
- 08:00 JST夜间目标继续活动；没有production/Linux/BIT/JTAG/SD/board动作。Codex亲自机制设计和源核验收；DeepSeek普通夹具提案，无Claude。

## §51 当前状态（2026-10-09，普通分片与两轮暂停判别）

- out-only真实RTL两build成功、15新执行：14完整copy PASS，其中最终build02六项都执行两轮pause；run09第二次quiet失败留存。根修：暂停不能挡住已first-VALID短W需要的R，singleton新AR需整段已有W覆盖。
- 普通mode2/q256与长读mode1四对CSV SHA逐字相同：无pause5657/5650；一次pause5161/19848；两次5161/29793；两次+B64/背压13277/28956。均同keep-q和四recipe。因为普通同成本同效，长读/q2的34%不能作为新算法贡献。
- sourceR无pause256、有pause504，目标W各256；14CSV独立计数核对。只给A可暂停、V继续的窄合同；logical quiet≠physical/backend idle。实际DMA为NO_ERROR_HANDLING，TC SLVERR不等于job可恢复fault处理。
- 夜间目标仍活动，到08:00 JST。接着核提前resume避免不必要退组/重读，以及独立context迟到/失败授权与source R交错的真正约束；不编BIT、不上板、不改生产Linux/RTL。
- [根验收](iommu-platform/out/shared-read-frontier-20261009/codex-result-acceptance.md)／[义务边界](iommu-platform/out/shared-read-frontier-20261009/critical-obligation-boundary.md)／[账本](iommu-platform/out/shared-read-frontier-20261009/execution-ledger.json)。DeepSeek driver提案，Codex设计/诊断/执行验收，无Claude。

## §50 当前状态（2026-10-09，真实 RTL 完整拷贝已接受）

- 完成 out-only shared source read + q-W/B 汇合原型；真实两 iDMA/IOMMU、独立 context 源 READ/目标 WRITE 四成功 recipe、同 CONF129。最终 RTL 12 项有效完整 copy PASS（run14–21、24–27），无 BIT/板卡动作。
- 两种真依赖环已修：未来 WRITE walk 被 held 长 R 阻塞；提前恢复 A 的长 AW 抢在 V 前。分别加目标同页 WRITE recipe 门、V logical RLAST 前冻结 A 新 AW；旧 AW/RVALID 不撤，quiet 不是 backend_idle。
- 无暂停 latency31：V/A5657/5650 vs q2读对照8579/8572，平均耗时低34.1%，AR1 vs128、源R同256；latency100正确生效后6692/6685 vs18377/18370。行为DDR，非MIG/上板。
- 相同 cut64B 长暂停：AR2 vs7、源R同504，V3438 vs3571（低3.72%），A13450 vs13496；B64+背压 V3763 vs3802（低1.03%），长暂停 A同13712。提前resume3181、实际detach3254仍完整完成。
- 27次新执行：最终12＋旧版7成功，6失败原始记录留存，22/23未知参数无效对比排除。新状态审计只有局部声明；56B只两DMA FIFO+skid payload，未综合/未完整公共成本。
- 下一：强化普通可变分片/有限先读对照，按同信息/总成本/暂停债务比较，再扩连续作业、重复pause及多页成功前沿。当前只验A一次pause/resume、V继续；退出后longW不保证第二次q界。如果普通同效同成本就不能称新算法；共享/broadcast和一般死锁原理已有前作，不以34%直接认独立创新。
- [报告](iommu-platform/docs/shared-read-rtl-progress-20261009.md)／[根判断](iommu-platform/out/shared-read-rtl-20261009/codex-result-acceptance.md)／[原始数据与SHA](iommu-platform/out/shared-read-rtl-20261009/execution-ledger.json)。DeepSeek测试/观察器/文稿提案，Codex亲自设计/诊断/审读/执行验收；4flash/high/max calls共711997 provider tokens，无Claude。最新实际完整copy已是§50，下方§49为历史模型。

## §49 当前状态（2026-10-09，root接受事件模型）

- 主线有可运行机制：两作业共享长physical R，A清完first-VALID短W/冻结R后退出，V沿原AR继续；A恢复按原cursor补读。不是只筛掉旧方案。
- run03 310场景/7540checks + run04 4定向场景/104checks，合计314/7644、exit0；两次失败598/143与代码保留。事件模型，非DUT/FPGA；delayed-B及实际权限未执行。
- 同N16/B3/q2/P3/resume200：候选与强tile同read28/AW6/最终214 event steps，AR2 vs4；无pause同48步、AR1 vs8。AR减少是具体差异，不是平台速度证据；共享读和退出补传原则有前作。
- B3二流FIFO48B+skid8B=56B，仅payload，须另计parents/recipe/控制。生产默认4KiB/flow可停2KiB，小缓冲负例不是默认bug。<=2q是受限工作量推导，非cycle期限/逐场景形式证明。
- 下一项：一个out-only真实iDMA/IOMMU原型+同完整成本强tile，未开始RTL构建；候选与对照同设可修改normal-memory属性（CONF129），旧CONF1不直接合读。核physicalAR/AW/B、TL/PTW、V/A时间、完整成本后判断贡献，不先编BIT。
- [报告](iommu-platform/docs/shared-read-independent-scheduling-20261009.md)／[接受](iommu-platform/out/innovation-rescue-20261009/codex-result-acceptance.md)／[RTL接口合同](iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md)。本阶段无production RTL/Linux/BIT/JTAG/board/SD动作；latest actual完整copy仍§43。Codex设计/核源/执行审定，DeepSeek模型与文档提案，无Claude。

## §48 当前状态（2026-10-08研究，2026-10-09交接，root接受）

- 唯一逻辑缓存模型run exit0：9个trace/E组×4普通策略=36records，64requests/24不同翻译；两小DP比较均6一致。无cycle/data路径仿真，不能当36项DUT或完整copy实验。
- 实际未命中：E8/gap4 LRU30→普通remaining-use24达首次下界；E4按页配对全24；E16所有策略24。因同信息interval/refcount可逐项复现用途规则，停止将“remaining-use取交/OR、dead-first”命名独立算法，不把模型miss减少当速度。
- 源码审定：done是R退休、prepared是已提交、valid才是匹配的成功结果；一般src3072短首父可被helper跳过、后父先成功（未DUT运行），原全长对齐fixture可条件性prefix。普通Depth4扫描既有window不用另存holes；短父及时成功证书/跨模块反馈仍要计费。
- 下一仅查译址materialization前沿控制共享源超前＋保留数据重叠＋committed W有限继续；普通tile barrier、有限窗口/ref保护、translate-first/lookahead均获同信息/完整资源。如果同决策同成本就不能称新机制；有具体性能/成本增量才做最小out completecopy，不先编译BIT。
- [报告](iommu-platform/docs/translation-sharing-window-20261008.md)／[根设计](iommu-platform/out/translation-sharing-window-20261008/codex-window-proof-and-design.md)／[验收](iommu-platform/out/translation-sharing-window-20261008/codex-result-acceptance.md)／[下一判别](iommu-platform/out/translation-sharing-window-20261008/codex-next-decision.md)。本轮DUT/build/production RTL/Linux/tests/BIT/JTAG/board/SD均0，最新actualcopy仍§43。DeepSeek证据/模型/稿件，Codex亲自设计/核源/执行审定，两席关键质疑，无Claude。

## §47 当前状态（2026-10-08，root接受）

- 保留具体候选：已登记copy任务的剩余翻译用途反馈＋共享条目保留/准入/回收；提前传任务、普通prefetch/pin、历史过滤、合并扣费本身已有前作，不作为独立机制。
- Codex根亲自核frontend/legalizer/TC/最新helper及2007/2018/2023/2025原文，两席完成关键质疑。native每方向FIFO支持条件性保守页游标，但§43 prepared-parent快慢返回不能直接套；需producer连续完成边界或计费holes。
- 下一场景：两DMA共享只读输入、写不同输出、进度错开。独立单调copy可能已被last-page/shared-entry pointer覆盖；先核额外非compulsory PTW，再与同任务信息/总资源的范围用途、位图、指针/引用保护等强对照比完整copy性能及成本。
- 局部232/464/128bit是手算载荷，不是完整成本/面积；如果新反馈组织或具体决策超过强对照就可以论证独立微架构贡献，如果同信息普通同成本同收益就不能称新算法。当前未实现、无新实测收益，不承诺分区。
- [报告](iommu-platform/docs/translation-sharing-qos-20261008.md)／[设计](iommu-platform/out/translation-sharing-qos-20261008/codex-candidate-design.md)／[下一判别](iommu-platform/out/translation-sharing-qos-20261008/codex-next-decision.md)／[账本](iommu-platform/out/translation-sharing-qos-20261008/stage-evidence.json)。本轮math/DUT/build/production RTL/Linux/tests/BIT/JTAG/board/SD均0，最新实际copy仍§43。DeepSeek证据/文稿，Codex设计/核源/审定；无Claude。

## §46 当前状态（2026-10-08，root接受）

- 源码/原文核查完成：现有 IOTLB 存储与 PTW 进度寄存器分开，没有假设的统一 slot 竞争。最新 A/V fixture 是不同 PSCID/root、stage1-only，不能当跨设备共享或 nested 实测。
- root 构造合法同 PSCID/同 root 的跨 DID 共享：两笔不同页面可共享上两级 PTE，leaf/最终判决分别保留。NB-Walker 固定作者源码已有共享 fetch、每请求 Info、有限 MSHR/PTW reservation；因为普通覆盖所列能力，简单组合不能称独立创新。
- 新工程边界：resident-way 历史隔离不等于 requester 历史隔离；A 合法命中 V 条目可改变 V 后续 victim。旧 S3 有意允许该行为，不称 RTL 违背旧合同或解释旧 PN 变慢。实际 hit class 不能用 main fill mask 代替；共享 hit/requester-specific replacement 又有 2008 直接前作，只作为强普通对照。
- 下一只查合法共享场景中，同资源强普通策略做不到的 V/A 整体性能点，不自动追加硬 requester 隔离合同。必要时复用 TL bench 补定向 out 基线；当前未实现/运行，不为已有机制编译 BIT。
- [报告](iommu-platform/docs/translation-sharing-progress-20261008.md)／[根级证明](iommu-platform/out/translation-sharing-progress-20261008/codex-requester-history-boundary.md)／[下一判别](iommu-platform/out/translation-sharing-progress-20261008/codex-next-decision.md)／[账本](iommu-platform/out/translation-sharing-progress-20261008/stage-evidence.json)。本轮新 math/DUT/build、production RTL/Linux/tests、BIT/JTAG/board/SD 均0；最新实际 copy 仍§43。DeepSeek证据/补正/文稿，Codex亲自设计、核源并审定，两席关键质疑，无Claude。

## §45 当前状态（2026-10-08，root接受）

- 一次数学图运行exit0：五个具体普通保存族、6450配置、mismatch0；所有可完成配置共30笔PTE读。受限max-gap iff已手工证明，root核原文/代码与实际路径，两席独立质疑。
- 强普通q3/B77按3/1/3/1/3/1/3零重读；q4/B36按4/4/4/3零重读；q3/B36无解。q2专用80bit只纸面构造，未加入五族图；五族曲线不是全架构最优frontier。
- 因普通提前让路/保留中间映射达到本合同读取下界，固定链checkpoint/codec调度不能称独立创新。q是PTE数、B仅停车帧；30限各15步、无共享合并/PWC。存在策略不等于任意公平策略完成，不推出周期/面积/持续到达界。
- 下一单一判别：真实共享请求中复用记录与保留进度是否竞争同一有限资源，先允许动态checkpoint/cache/pin/普通分槽的强对照；若普通能消差异就停，不先做RTL/BIT。
- [报告](iommu-platform/docs/translation-progress-frontier-20261008.md)／[验收](iommu-platform/out/translation-progress-frontier-20261008/codex-result-acceptance.md)／[下一判别](iommu-platform/out/translation-progress-frontier-20261008/codex-next-decision.md)。新DUT/build/run、production RTL/Linux/tests、BIT/JTAG/board/SD均0；最新实际copy仍§43，不重跑。DeepSeek起草脚本/修订/报告，Codex设计、执行、审定；无Claude。

## §44 当前状态（2026-10-08，root接受）

- root＋两席 Codex 已完成独立设计与交叉质疑，DeepSeek补接口证据/报告草稿；三席复用既有agent，无Claude。
- 四条规则停止立独立机制：正常TC已接受请求恒ready，不能由AW/W迟滞推出慢subscriber；无关失效选择取消有具体scope guard前作；每PTE续走/phase union被2009 continuation与Rocket2022父/叶复用、索引恢复覆盖；FQ credit/FIFO/去重可同效。
- 核清两个普通成本点：逐PTE因式续走帧上界82bit、partial G restart上界36bit；均另计原请求/context、token、active/在途与terminal，未实现/综合，非最小位数或面积。
- 新反例：公平地反复读G-root再restart，两设备可都无S进展。36bit一次让路可只等当前PTE、每pause至多多3次G读；持续进度需G+S语义quantum最多4笔或有限暂停/连续服务，不能偷换成统一一笔等待界。
- 下一仅核固定到达/持续进度合同的状态容量—重复PTE工作—让路完整边界；当前无新锐界或最优算法。若只是已有checkpoint/restart的选择，就不能称独立创新，不先编译阈值扫描。
- [报告](iommu-platform/docs/translation-continuation-frontier-20261008.md)／[逐字段审定](iommu-platform/out/innovation-frontier-reset-20261008/continuation-critical-agent.md)／[下一判别](iommu-platform/out/innovation-frontier-reset-20261008/codex-next-decision.md)。本轮DUT build/run/数学运行/生产RTL/Linux/BIT/JTAG/板卡/SD均0，最新实际copy仍§43。只读源核＋纸面构造不是实验通过。

## §43 当前状态（2026-10-08，root接受）

- 1新build＋1新双bank仿真，两个32KiB完整copy PASS；旧§42四run未重跑。
- 虚拟/物理AW序34/34逐位置、owner、seq匹配。W队首滞后64周期（owner16/seq64，重叠）、最多1笔；它是旧payload待内存接收，没有AW倒置。
- A request8315→quiet11321，3006周期；Aoffers4→4、child3→7（新增4KiB）、parents5→5；quiet A7KiB/V17KiB，pause-end V32KiB。不能与§42不同起点的4115做周期因果比较。
- 根级源码归纳：当前source物理已接受前缀门＋demux单bank写数据组排除跨bankAW倒置；TC内部spill握手与外部AW接受分开。限当前静态DEFAULT copy原型，不外推生产一般保序或全系统无死锁。
- 创新门：双写序冲突枝停止；纯byte-debt准入可由普通owner-prefix记账得到相同决策，也不立新机制。下一只查真实翻译相关约束/锐界，若普通同信息同资源可实现，就不能称独立创新；不编译阈值扫描。
- [报告](iommu-platform/docs/dual-write-order-boundary-20261008.md)／[准入等价分析](iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md)／[下一判别](iommu-platform/out/dual-write-prefix-20261008/codex-next-decision.md)。仅out观测，production/tests/Linux/BIT/board/SD无动作；DeepSeek证据/文档草稿，Codex架构/核源/执行/验收。

## §42 当前状态（2026-10-08，root 接受）

- 阶段：committed-write drain boundary；root 已执行 1 build + 4 run，4/4 PASS。
- 严格停：V 先/A 先窗口内均无 first A quiet；pause-end V W=11264/0，A W=0/0。
- 普通强对照：V 先 request→quiet 4115，pause-end V W=32768、A W=7168；A 先 5436，V W=19456、A W=7168。
- 必要继续：V 先 ≥15360 R/≥15368 W；A 先仅 A 7168/7168。
- 普通两例均增加 7 个 1KiB A 子读请求，无新 A 写请求；父请求数 4→4，A 整 job 未完成。
- 下一步：multi-bank 两 W 序精确前缀释放 + 普通对照；仅单有序队列守恒则停分支。
- [报告](iommu-platform/docs/committed-write-drain-boundary-20261008.md)／[下一判别](iommu-platform/out/committed-write-drain-20261008/codex-next-decision.md)。功能状态至少65bit、未综合；无生产/BIT/板卡/SD操作。DeepSeek起草，Codex设计/执行/审定。

## §41（2026-10-08）

- §41（2026-10-08）完成一次静态数学运行与根级来源审定；未做 DUT build/run、BIT、板卡、SD、production、Linux 或 tests 改动。
- 纠正一：B>0 时一个 cut 可支持多个停止点；反例 N=10、B=2、H=4、D={0,4,6,10} 真实 1 cuts，旧 node-cost 算 2。
- 纠正二：H 只是安全目的点间距，不是 budget-off 后的真实工作或周期界。
- 数学：7585+3805=11390 分类，部分重复覆盖，非 11390 DUT 试验；greedy/brute mismatch=0，数学程序 1 次 exit0。
- 静态 32KiB：B=0/24 时 H=2/4/32KiB 给 source 32/24/16、AW17；B=1024 给 source16、AW17。B=24 只是数学假设，不是 24B/DMA+48B 两路或 TC shared W 128B/bank 的 private B。
- 软件 tiling 3KiB+7×4KiB+1KiB、9 subjobs、理想 source24/AW17、8 cuts 达下界，但未运行，不声称同 TL/cost/interface。
- 根新增审定：§39 `compact_pretl_owner_fragment.sv:87–123` 已有两层匹配且 physical-accepted W prefix 准确门，当前 child 不会因“未来目的 AW 未准备”准入；这版不能拿纸面未准备 AW 反例当新 bug。实际剩余问题改为预算暂停时较远 W-owner 债务需更多 source child/同 job raw parent 接收，仅是待验证假设；`multi_actual_dma_top.sv:610–622` raw AR 移出共享 mux、perflow credit 捕获，限 out 原型非生产通用保证。
- [报告](iommu-platform/docs/fragment-drain-cost-boundary-20261008.md)与[下一判别](iommu-platform/out/fragment-drain-boundary-20261008/codex-next-decision.md)已落盘；两名研究席的两轮审核均已完成，全文已保存，根已直接核关键原文、实际源码及两份最终分析；这不等于硬件验收。真实义务须含首次 offer、mux owner 提前 push、held requests、两 W 前缀；强对照须同 B/同 payload/同 metadata/同释放点；剩余仅 tight continuation work/cost 候选，未证明不承诺分区。

归属：DeepSeek整理证据、提出脚本与文档草案；Codex设计、核源、实际执行并审定；未调用Claude。

## §40 翻译寿命边界与创新门槛（2026-10-08）

- 本阶段 Codex 已执行只读源码命令并写入证据/草案文件；没有 DUT build/run/BIT/board，没有改 production RTL/Linux/test，没有写 SD。报告为正式版。
- 根 Codex 已直接核关键源码并审定：因为当前平台没有“普通译址结果必须等 DMA 数据完成才释放、并以该释放反馈阻塞 IOFENCE”的可达路径，当前 LTI-style 寿命反馈被源码可达性门槛否决；Arm LTI §6.2.1 独立完成义务与 §6.2.2 多事务寿命必须分清，不能直接套用。
- PR/PW 的硬件 TODO 不是创新；Linux 默认 IOFENCE 设置 opcode/func 与 dword1=0，普通同步调用 PR/PW=0；真实 FPGA 配置 InclATS=0；CQH 早推进不等 generic completion；§39 静态 6 PASS 不验证 live invalidation。普通 fault/error+discard 是强进度控制，当前 fault 排空未验。
- 两个 native Codex 研究席均已完成：第一席 `innovation_rules_40` artifact 为 [iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md)；第二席 map artifact 为 [iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md)。ShareMMU §V-C7 只写适用性问题，不称生产死锁。
- 本轮的常规规则、PT 着色、共享计账、walker 续表转存具体提案均未排除强前作／普通方法，停止将这些提案作为独立机制，理由为强前作覆盖；NB-Walker 新作者 artifact 已取得代码并只读保存，但论文全文仍未得、未复现实验。
- 下一单一判别规格以 [codex-next-decision.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-next-decision.md) 为准：固定映射，long copy 内可控让路，小缓冲下最小合法前沿/分片成本界；temporary translation lock 只属 ShareMMU 适用性反例，不是优先给生产新加锁，也不是当前 IOTINVAL 语义。
- 主报告没有排除强普通的 new rule，因此不能认定独立机制；保留待证明锐界作为分析候选。下一判别未 run；禁止假造完成实验/新算法/锐界证明。
- 正式报告：[iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md](iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md)。根审定、证据、下一判别：[codex-platform-gate.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md)、[source-evidence.json](iommu-platform/out/translation-lifetime-boundary-20261008/source-evidence.json)、[deepseek-source-evidence.md](iommu-platform/out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md)。
- 归属：DeepSeek 整理证据和起草文档；Codex 根与 2 研究席作创新推演、关键源/原文核查、最终审定；未使用 Claude。

## 2026-10-08（§39）：普通物理读旁路消除97.45%差距，创新门槛更新

- 1新SV+C++构建、6新仿真、6完整拷贝PASS；关闭旁路的两份CSV与§38逐字节同。代码限out副本；76生产SVhash同，未进入BIT／实板。
- 同原窗口4／父槽4／48B FIFO／CONF1／各32KiB：V先双方均值24751→普通物理旁路18975，旧post18824；差距5927→151，缩小97.4523%，post只再低0.7958%（分母18975）。不是原23.95%独有规则收益。
- 普通cache直接62／TC回退2；真实TC等待写翻译时直通AR为V先25／A先16。等准备直通64却均19565更慢。六项TL66/hit32/miss34/PTW102/DDTW2/AR64/AW34/每流4096R-W，bytes／guard／R归属／hold／排空过。
- cache A先均19008，交换后两流各晚33，非完美镜像；off/wait镜像。首AR/R普通早897，但后续相对差有正负；不能把151全归两次回退、固定流水或带宽。声明功能state4598 vs4602未综合、非等总面积。
- 因为普通旁路已有前史且几乎追平，就不能用位置或原性能差距立独立机制。下一先核平台真实的结果释放／目的W／失效等待边界，再做普通按子片译址／预约／重译强对照；若普通同保证同成本，就不能称独立创新。因为只验静态性能而未证独有性质，就不能立新机制，不承诺分区。[报告](iommu-platform/docs/prepared-source-bypass-boundary-20261008.md)／[根审计](iommu-platform/out/prepared-source-bypass-20261008/codex-raw-audit.json)／[下一具体门槛](iommu-platform/out/prepared-source-bypass-20261008/codex-next-boundary.md)。下方为历史阶段。

## 2026-10-08（§38）：多父准备边界：普通缓存命中直通与条件性独立创新判据

- 5 新构建、18 新运行、18 PASS；8 次普通模式、4 次读优先、2 次观察运行、4 次命中直通；`analysis.md` 只覆盖最初 8 项，根审计覆盖全部 18 项；新增 10 项是 DUT 运行，不是另加根审计。
- 已完成最强普通缓存命中直通 AW 优先 V 先启动 23380/26122 mean24751；旧 post window4 V 先启动 17485/20163 mean18824，均值降低 23.946507%，绝对 5927。它不是普通 TC 旁路；源子片仍进共享 TC。
- prepare0/1/2 均值均 25869.5，但 prepare0/2 CSV+prepseq 相同、prepare1 不同，不能当 3 个独立性能点；读优先非统一最好：串行 lazy 25630、串行 prep 25847.5、缓存命中直通 read 24992，后者比 24751 差。
- 因果见证：源 V0 缓存填充 1019；写 REQUEST 2421→写 RESPONSE 2861 同一 pending 持续 440 周期等待写；源命中接受 2868 在写完成之后，不能称 2868 时 TC 仍忙写。原窗口高水位 4/4，未来父 30/32。
- 声明功能位 pre3316+helper1281=4597 与 post4602 未综合，不是等面积；76 production SV 不变；无故障／失效／世代通用验收。因为 Arm LTI 已覆盖 translation-before-ordering／lookaside、同 page+context share 与 completion deadline，仅缓存读绕过 TC 或背景未命中不能称为独立创新。
- 下一最强普通对照（已准备源片物理 AR 在 TC 写未命中期间通过，full permission／lifetime／BC／error／routing，same original window／saved metadata／payload）未实现；下一优先检验普通物理AR旁路。证据：[codex-raw-audit.json](iommu-platform/out/multi-parent-preparation-20261008/codex-raw-audit.json)、[analysis.md](iommu-platform/out/multi-parent-preparation-20261008/analysis.md)、[报告](iommu-platform/docs/multi-parent-preparation-boundary-20261008.md)、[下一对照规格](iommu-platform/out/multi-parent-preparation-20261008/codex-next-control.md)。

## 2026-10-08（§37）：配平原请求窗口后仍差6.7%，单父槽进度缺口已证
- Codex out原AR→父RLAST入口credit、物理父深度1/2/4和普通owner准备时机；4新build exit0、16新run：10完整拷贝PASS／6看门狗停滞，363+48采集条件过。不把采集有效说成全DUT通过，无旧DUT重跑。
- 同原窗口1：post V/A22282/25697均23989.5，比旧普通提前25714低6.706%；新owner资格普通25713仍差1723.5（6.703%）。窗口8 post18810/CSV等旧；原27%不能全归独有调度。
- 窗口2/4＋父槽1停滞；窗口2＋槽2恢复20151，窗口4＋槽2仍停、＋槽4恢复18824。冻结现场TC卡A、A父槽占着、双W头等V下一父；是研究原型组合进度缺口，不是生产或旧PN根因。普通加槽已解本例，不能把加槽或等待环本身立新。
- 成功组TL66/hit32/miss34/PTW102/DDTW2/AR64/AW34/每流4096R-W；原FIFO48B无新payload。post声明state含credit3576/3918/4602bits，普通3518，未综合/非总预算等同；virtual/AW仍8。76生产SV同，无原tests/Linux/BIT/SD/板卡动作。
- 下一普通前置多原父header提前准备、匹配窗口4与总metadata预算。完整normal响应83bit＋已有父header复用是预算约束，尚未实现。若普通同预算同效则联合规则不能称独立创新；独有进度/成本且核前作才可立贡献，不先BIT。
- DeepSeek3次汇总与修订完整返回；Codex关键设计/核源/前作、执行及修正跨窗口混平均与误把两方法当相等的指标检查。[报告](iommu-platform/docs/original-window-progress-boundary-20261008.md)／[raw清点](iommu-platform/out/original-window-20261008/README.md)。下方为前阶段。

## 2026-10-08（§36）：普通复用／当前父准备仅改善0.33%，下一配平未来请求可见范围
- Codex实现out TC↔TL父内复用与普通提前准备，6有效新仿真6PASS/778最终raw条件过。前置关闭轨迹同§35；真实双流平均25799→复用25712/提前25714，仅低0.337/0.329%，旧post18810，残差6902/6904。TL98→66，miss34/PTW102/物理AR64/AW34/每流4096R-W不变。
- 提前准备32父全部真实译址/64child缓存返回；未增加R payload，48B原FIFO。旧post第一拍R2404前已接收6原父，pre当前每流1raw父；普通当前父准备没有同未来可见范围，不能说普通提前准备普遍无效或残差是独有算法。
- 声明功能state普通3514/提前3518/post3568bits，未综合/非总面积；无失效/fault/epoch通用验收。Arm LTI6.2.2已明确同页同上下文译址共享、1.1已有排序前译址；复用/预取/单独加队列不能直接当新规则。
- 实际3新SV+C++构建/10新run均exit0，其中首4开关未生效actual enabled1，明确excluded；显式C++输入修正后6项有效。只读4旧参考，无重跑旧结果。DeepSeek汇总offset/字段名修订，Codex修正跨启动混算双流均值，最终数学与raw另核。
- 下一普通同原父可见范围/总状态预算对照，复用header队列或稳VALID下一父的无副作用准备，未实现。若普通同效同成本就不能立位置规则独立创新；新约束下独有进度/成本且核清前作才可立贡献。
- 76生产SVhash同，无原tests/生产/Linux/BIT/SD/板卡动作。DeepSeek3完整返回接口/汇总（259312 provider tokens），Codex关键设计、核源/前作、执行与审定。[报告](iommu-platform/docs/parent-translation-reuse-boundary-20261008.md)／[raw清点](iommu-platform/out/parent-translation-reuse-20261008/README.md)。下方为前阶段。

## 2026-10-08（§35）：分片位置实测正收益，下一排除普通翻译复用
- Codex实现out前置固定分片与§34单父后置基线；1新SV+C++构建、3仅C++重链接均exit0，8新仿真8PASS，最终raw汇总902条件通过。原post/S轨迹与§34同序CSV相同，无重复DUT。
- CONF1/两真DMA同bank/各32KiB/48B FIFO，V先真实issue→done前置24162/27436、后置17471/20149，均值25799→18810，降低27.09%；A先镜像。TL98→66、hit64→32，但miss34/PTW102/physicalAR64/AW34/每流4096R-W不变，非页表流量下降或旧PN修复。
- 四N FAST0、CSV同对应S；源码窗口要求跨bank，本例未触发，不能当有效快路径对照。声明state前2502/后3568bits，未综合/非总面积，逻辑入口保存容量不同，不称等总资源或全部时差来自32hit。
- 下一普通原父翻译复用，必要时加提前准备，计全部metadata/TC容量。若同成本普通同效，当前位置规则不能称独立创新；若新约束下独有决定/进度/成本并核清前作，才可立贡献。§33动态负结论保持，不承诺二区，不先BIT。
- 76生产SV哈希同；无原tests/生产RTL/Linux/BIT/SD/板卡动作。DeepSeek观测/汇总草案修订（3完整返回，332487 provider tokens），Codex独立位置设计、关键源码/前作核查、执行审定。[完整判断](iommu-platform/docs/fragment-placement-boundary-20261008.md)／[八项raw与清点](iommu-platform/out/fragment-placement-20261008/README.md)。下方为前阶段。

## 2026-10-08（§34）：固定基线每流1父槽同效，继续收紧创新门槛
- 独立缩小已译址父保存槽1／2／4，3次SV+C++构建exit0、6新仿真6PASS；复用2份§33的8槽基准，不重复编译／运行。480项条件通过，六新CSV与对应8槽逐字节相同。
- CONF1／两真DMA同bank／S／各32KiB／48B iDMA FIFO；均32逻辑AR、34AW、64物理子AR，各流4096R/W拍。V先末DDR W17530／20213，A先镜像。1槽无可观测损失，不证明8槽原高水位1或通用单槽进度；virtual关联／AW队列仍8。
- 模块声明非统计state5962→3568bits，少2394仅物理父metadata，观测160另计、未综合／非总面积。动态独立主创新仍未成立，不能用8槽原型成本作虚弱对照。
- 新核AMD2009 IOMMU依赖环／独立VC前史；下一同CONF1/AW几何/总成本比较前置固定分片、后置固定分片，必要时加普通预翻译准备强对照。仅规格，未实现；不把TL请求少等同miss/walk少，不承诺二区。
- 76生产SV哈希同，无原tests／生产RTL／Linux／BIT／SD／板卡动作；不是PN退化解释。DeepSeek接口／汇总草案修订，Codex独立容量设计、核源／前作、实际执行审定。[完整判别](iommu-platform/docs/parent-capacity-boundary-20261008.md)／[证据](iommu-platform/out/parent-capacity-20261008/README.md)。下方为前阶段。

## 2026-10-08（§33）：动态后译址分片已判别，当前不立为独立主创新
- Codex独立实现固定1KiB／动态最长已准备双W序前缀，共用两流各8parent、原ID/RLAST恢复、无新增R数据缓存；1次SV+C++构建成功，6仿真6PASS，轨迹汇总516项通过。
- CONF1两真DMA各32KiB/src0/dst3072，固定／动态均32逻辑源AR＋34目的AW、各路4096R/W拍、两iDMA FIFO48B；动态物理源AR64→55，少9次但无独有译址/进度收益。
- 读延迟31：最后DDR W20213→20766，晚553，两启动顺序镜像；100：31805→31737早68，但V晚130/A早68，双方平均末W晚31。参数影响全部行为DDR读，非单独PTW干预/稳态吞吐。
- 新模块声明非统计state5962bits＋观测160bits，非最终面积且不含公共FIFO/cuts；固定实现还可裁剪扫描。独立主贡献立项不通过，保留固定强基线，下一先证总资源/进度必要条件或强固定做不到的明确差异，不自动上BIT。未证明世界已有完全同式规则，不承诺二区。
- 76生产SV哈希同，无原tests/生产RTL/Linux/BIT/SD/板卡动作；仍两流同bank行为DDR，不是四DMA/MIG/旧PN修复。DeepSeek证据/验收草案与修订，Codex关键设计、核源/原文、执行审定。[判定报告](iommu-platform/docs/posttl-independent-verdict-20261008.md)／[六项证据](iommu-platform/out/posttl-independent-20261008/README.md)。下方为前阶段。

## 2026-10-08（§32）：普通小缓冲门已完成，创新门槛改为提前译址与总成本
- Codex实现out独立准确源AR准入门，确认两侧W owner/段序、目的准备与源目的偏移/len；保持首次VALID。1新SV+C++构建＋1旧大缓冲C++重链接，4新仿真4PASS。
- 同CONF0/每流32KiB错位，BUF3＋门和BUF512无门各V先/A先均通过；各路R/W4096拍、总64AR/64AW、bytes/guards/quiet正确。两份门控轨迹128次源AR准确匹配当前virtualW。
- 两路iDMA数据FIFO48/8192B，排除公共FIFO、门metadata/控制、非LUT/面积。最后DDR W27894/25146，晚2748，仅reset-relative终点差，非纯译址时长/V-A平均吞吐；普通门也改变自然AW出现顺序。
- “解除该等待环”不能当新贡献。下一先实现固定译址后分片＋同owner调度，与普通源门/大缓冲/动态I同总资源比较，核AR属性与parent容量；固定同效同成本就停动态主贡献。I尚未实现，创新/二区未成立。
- 76生产SV哈希同，无生产/tests/Linux/BIT/SD/板卡变化；仍仅两真DMA同bank行为DDR，非四路/MIG/旧PN解释。DeepSeek接口/准备脚本，Codex独立设计RTL、核源纠错、执行验收。[完整结果](iommu-platform/docs/write-owner-baseline-20261008.md)／[原始记录](iommu-platform/out/write-owner-baseline-20261008/README.md)。下方为前一阶段。

## 2026-10-08（§31）：自然写序等待环已观察，新机制尚未实现
- 两真实iDMA同bank、无强制逆读/AW交错、普通S；3次SV+C++构建＋2次C++重链接成功，13仿真6PASS/7watchdog。CONF1非对齐32KiB长任务，非旧PN/四DMA/MIG实板结论。
- C0：第8个bank已接受AW使计数q=0/overflow=1，W被关闭；只在out副本提前限制到配置4项，消除溢出，生产尚未修正。
- C1：修正后仍有V读尾→A页表读取→A写队头→V后写段的等待环；后一V目的AW已准备，关键是实际W顺序空洞。普通CONF0短读仍停，普通512拍缓冲两启动顺序均完成。
- 两路iDMA FIFO总量48/8192B，仅配置数据存储，非总面积/新机制资源收益；76生产SV哈希不变，无Linux/BIT/SD/板卡动作。
- Codex修订I为译址后片边界＋W-owner服务选择，未实现；下一与固定1KiB＋同调度、普通短读＋准确owner门、尾部预约/per-ID队列同资源比较。创新与二区未成立。
- [完整审定](iommu-platform/docs/natural-write-order-boundary-20261008.md)／[命令与验收](iommu-platform/out/natural-write-order-20261008/README.md)。DeepSeek驱动/观测草案和修订，Codex独立设计、核源/前作、诊断执行验收。下方为前一阶段。

## 2026-10-08（§30）：读写几何两项通过，候选 I 在该场景无收益前提
- 0 次 RTL 构建，仅复用已有模型编译 C++；CONF0 两次读 1024、CONF1 一次读 2048，目的均两次写 1024。两项 exit 0 / PASS，完整数据和未写区域正确。
- 第二目的 AW=1456，源 AR=1899、首 R=1931；目的早已准备好，本场景不能展示自适应分段收益。512 拍普通缓冲、单 DMA、旧诊断模型参数，不能当自然多 DMA/面积/实板证据。
- Codex 独立候选 I：源译址之后、物理 AR 首次 VALID 前，按目的译址及具体写序决定片边界；尚未实现。AXI-REALM 和 2026 可抢占 VC 前作已核，透明分片/读写破环不能称新贡献。
- 下一门槛是关闭诊断强制排序，核自然多 DMA 写序是否产生部分目的可推进的真实状态；再同资源比较强简单方法。创新及论文等级未成立。
- [审定与下一步](iommu-platform/docs/post-translation-fragment-witness-20261008.md)／[实际运行](iommu-platform/out/adaptive-fragment-witness-20261008/README.md)。DeepSeek取证与驱动草案，Codex独立设计、核源/原文、执行验收。下方为前一阶段。

**2026-10-08 创新反证：翻译优先对照2/PASS2，新机制尚未成立。** 复用旧binary、0编译/源码改；P88989、PN91547，V/A各64、FAST0/62；DS DDR首R延迟两者均32，PN仍慢2.875%，闭环CPU流量不同，非全翻译时长/带宽或板上结论。原文确认独立VC/优先/动态burst/RW环整预约已有；新增两PDF。Codex独立G/H探索目的可消费前缀与跨copy依赖决定源读边界，未证有效独有决定/成本，未实现；拒绝worker虚构共享PTE/payload信用反例。下一先给同信息同资源的真实差异见证，不先BIT。DeepSeek证据/质疑，Codex设计、核原文/源、执行审定。[规格与结果](iommu-platform/docs/translation-lane-and-burst-rule-20261008.md)／[§29](iommu-platform/docs/composition-history-isolation-20261007.md#29-翻译返回对照与新准入规则反证)／[资料](iommu-platform/out/translation-lane-check-20261008/README.md)。下方为此前阶段。


**2026-10-08 继续创新筛选：补齐接口与翻译寿命前作，独立创新仍待证。** 新取得LTI2020、Intel DSA2025两规范及MetaSys/TAPA/XDMA三论文全文，核关键段落；预取、元数据、ready队列和成对预约不能直接作新贡献。源码确认现test额外导出iDMA copy信息；LTI的SPEC/LC寿命不能与当前TC COMPLETE混用。下一收窄为可丢弃准备与真实W提交之间的映射寿命/成本规则，先与强普通联合准备＋ready队列、预取复核和W数据齐备门作最小判别；未实现、未实验，不承诺二区。旧D独立分段判别未执行，不先BIT。生产/test/Linux修改、新实验/构建/板卡动作0；最新真实仍§26。[研究规格](iommu-platform/docs/dependency-contract-innovation-20261008.md)／[报告§28](iommu-platform/docs/composition-history-isolation-20261007.md#28-继续创新筛选依赖信息翻译寿命与写承诺)／[资料](iommu-platform/out/dependency-contract-innovation-20261008/README.md)。DeepSeek证据/修订，Codex亲自设计、核原文和接口、纠错裁决。下方为此前阶段。

**2026-10-08 原论文驱动创新筛选：独立创新尚未成立，保留分段错配的成本候选。** 对照14篇机制段落，新增iDMA/CTC/AlphaServer三份原PDF；普通预留、直通、ready调度已有强前作。Codex提出“长读只预约暂不能写出的后缀”，待与整预约/短读/准确per-segment直通池同资源比较；尚未实现或证明收益。源码确认当前Linux/test CONF=0，读写共同分段；候选需启用独立分段，不能解释既有PN退化。下一先复用真实iDMA作偏移2KiB最小判别，普通方法同效同成本就停；不先构建BIT。本轮仅论文/源码审定与文档，新实验/构建/代码/板卡动作0。[完整审定](iommu-platform/docs/paper-driven-innovation-20261008.md)／[报告§27](iommu-platform/docs/composition-history-isolation-20261007.md#27-原论文驱动创新筛选分段错配与部分返回预约)／[资料](iommu-platform/out/paper-driven-innovation-20261008/README.md)。DeepSeek证据/质疑，Codex独立设计、核原文/源码并裁决。下方为此前阶段。

**2026-10-08 瓶颈定位推进：PN提前翻译后仍等写路径，独立创新未成立。** 复用两真实DMA/两bank普通无池模型，1次C++重链接、0次SV重建、4次仿真PASS；正常P/PN92109/92374，行为内存读准备延迟可重叠后85339/85984，PN仍略慢，不能归咎单一串行延迟。62次FAST→源读捕获1051/1052cycles，捕获→DDR均5cycles；首例跨A页表/数据等待后V AW才准备，定位提前翻译后的写顺序/准备约束，未独立量化265cycle总差因果。serial差额账本主要为读队列空+241，非内存带宽饱和证明。四DMA接口已核，324次受限抽象分析显示盲目共享可停滞、普通W-bank预留2头即无模型停滞；没有四真实DMA RTL/面积/通用证明。仅1个C++测试源、76生产SV哈希不变，无Linux/BIT/板卡操作。下一核跨bank AW/W锁定与源读供给的最小安全改动及强普通方法差异，停止扩复杂扫描。[报告§26](iommu-platform/docs/composition-history-isolation-20261007.md#26-提前翻译后仍等待写路径读延迟干预与四dma容量边界)／[关键审定](iommu-platform/out/read-service-boundary-20261008/codex-review.md)／[清点](iommu-platform/out/read-service-boundary-20261008/stage-evidence.json)。DeepSeek接口/脚本提案，Codex独立设计、原源核查、诊断执行与验收。下方为此前阶段。

**2026-10-08 新准入判别完成：有工程改善，复杂扫描创新未成立。** tests两真实DMA/两bank，有限64cycle DDR写尾反压；准确物理前缀已供给门PN102686→96267（耗时少6.25%），更简单普通virtual-W-head＋matching prepared-AW也96267且已记录CSV逐字相同。P101731→95692，最强普通PN仍慢P0.6009%；V/A各64、PN FAST62。无夹具早期两版PN92374、额外动作0。8次仿真PASS（2早期校准＋6最后模型），5SV构建成功/1函数名笔误失败后修正，4测试源改动、76生产SV哈希不变；无Linux/BIT/板卡操作。完整扫描降工程对照，下一主线核四DMA/两bank同资源下提前翻译、进度、小缓冲三者的真实边界；未实施四DMA，不承诺二区。[报告§25](iommu-platform/docs/composition-history-isolation-20261007.md#25-物理写尾未排完时提前放行源读改善与普通规则相同)／[关键审定](iommu-platform/out/payload-frontier-poc-20261007/codex-review.md)／[清点](iommu-platform/out/payload-frontier-poc-20261007/stage-evidence.json)。DeepSeek观测/指标草案，Codex独立设计、关键源/原文审定、修订执行与验收。下方为此前阶段。

**2026-10-07 请求头暂存PoC完成：机制创新推荐停止，保留工程对照。** 共享2头＋直通解除同bank受控停滞；H1/无直通H2失败，普通共享3头与每DMA2头都PASS。正常P/PN两方法精确92109/92374、FAST62，无新增收益；2vs强普通3仅少一个头槽，未综合成本或证明通用进度。7SV构建、9运行7PASS＋2预期watchdog；6测试源、76生产SV不变，无Linux/BIT/SD/板卡动作。下一先找真实4DMA/2bank下普通同信息/资源方法遗漏的进度约束，不能包装普通shared-select/bypass为新机制。[结果与判定](iommu-platform/out/header-park-poc-20261007/README.md)／[报告§24](iommu-platform/docs/composition-history-isolation-20261007.md#24-请求头暂存原型已实现解除本例停滞独立机制创新未成立)。下方为此前阶段。

**2026-10-07 二区目标推进：确认共享bank的真实进度边界，创新仍待证。** 两真实DMA同bank＋有限AR背压，普通无池48B停滞；清洁现场V/V头挡住A源读，四AW已备、DDR数据AR0、TC COMPLETE、spill双满。相同夹具普通8192B完整缓冲与普通源端准确写准备门48B都PASS，双bank对照也PASS；S/warmup2/V64，A窗63/64不同，不能算净性能收益/PN。修复本身是工程强基线。Codex新规格为少量已翻译头暂存＋当前义务源读直接通路，保留提前翻译并核容量/进度；H1/H2与普通perDMA队列下一先做小PoC，候选未实现，(K−1)L不是定理，不能承诺二区。此前AW延迟64/4096未死锁，已有TC双AR槽已核。[报告§23](iommu-platform/docs/composition-history-isolation-20261007.md#23-二区目标推进共享bank的真实停滞边界与下一机制规格)／[审定与规格](iommu-platform/out/write-order-boundary-20261007/codex-review.md)／[阶段清点](iommu-platform/out/write-order-boundary-20261007/stage-evidence.json)。10SV成功构建、2C++重编，13调用7PASS＋3预期watchdog/3其他失败记录；tests5源、76生产SV不变，无Linux/BIT/SD/板卡操作。DeepSeek证据/验收草案，Codex亲自设计、诊断、实现执行、审定。下方为此前阶段。

**2026-10-07 完成阶段准入推进：21项矩阵核对通过，更简单的无池对照优于共享池。** Mode7按虚拟/物理写队头及紧邻下一段放行源读，热P/PN92109/92374、冷92549/92814 cycles，V/A各64次4KiB；热PN较本轮原93487耗时少1.19%，仍慢同法P约0.29%。活跃DMA payload仅两3拍FIFO共48B（排除共同TC FIFO/元数据/控制，未综合面积），共享2KiB池不再作为主贡献。完成阶段单槽PN123804→紧邻下一段99432→普通无池92374；详细[报告§22](iommu-platform/docs/composition-history-isolation-20261007.md#22-完成阶段准入与强普通对照共享池主贡献停止)／[Codex审定](iommu-platform/out/completion-reserve-late-20261007/codex-review.md)／[验收](iommu-platform/out/completion-reserve-late-20261007/acceptance.json)。Codex发现旧testproxy单FAST标签被提前排队覆盖，补逐读标签FIFO后8项通过，FAST读62/64、fault0/queued0；原13条读分类撤销但buffer数据/cycles有效，旧8条Mode7保留不计最终矩阵。walk修标签后仍无额外动作/收益。六次成功SV构建＋一次lint，实际29次完整拷贝执行、最终21配置；76生产SV哈希不变，无Linux/BIT/板卡动作。创新未成立；下一先核双写顺序不一致是否合法可达及普通前缀门的安全/前作边界，再考虑最小回退或顺序约束，不自动上BIT。DeepSeek验收/独立质疑草案，Codex亲自设计、诊断、执行与审定。下方为此前阶段。

**2026-10-07 推进到共享返回缓冲RTL原型：修正后13项完整拷贝通过，当前实现无性能收益。** 真iDMA各3拍FIFO＋共享完整burst返回区，数据容量8192→2096B（非面积/LUT），V/A各64次4KiB、字节及排空守恒通过。原热PN93487/FAST62精确校准；普通两槽PN103348/FAST31，选择一槽112448/FAST0，改属主预取分配125677/FAST0更慢。当前入口预约＋每DMA一未消费AR改变请求流水；不是公平同在途深度或最强pool对照，创新未成立。下一主线保留元数据排队，在每bank完成AR进入共享完成mux前按具体段预约，先恢复FAST/进度，再同队列/容量对比普通精确依赖池；候选未实现。[报告§21](iommu-platform/docs/composition-history-isolation-20261007.md#21-共享返回缓冲原型13项通过容量下降并行机会被当前入口限制压住)／[审定](iommu-platform/out/e2-interface-poc-20261007/codex-review.md)／[已执行验收](iommu-platform/out/e2-interface-poc-20261007/acceptance.json)。仅tests新模块、3源和原启动器一行加载；76编译生产SV哈希不变，无Linux/BIT/板卡动作。首测AW前后寄存观测错配等为Codex原型错误，已改正、失败日志保留；13/13非一次通过。DeepSeek接口/C++/结果草案，Codex亲自设计、核源、应用修补及执行。下方为此前阶段。

**2026-10-07 继续探索：保留E2成本预研候选，创新仍未成立。** Codex独立设计“真可排空读段流式、其余提前读在共享仲裁选择前预约完整burst返回”；整job共享E1与普通pool重叠，不推荐为主创新。候选2KiB共享＋两3beat FIFO=2096B对现8192B仅理论payload点，非面积/性能/无死锁结果。已核4DMA→2bank但全copy保证仅前2，普通walk额度不约束全部旧数据/PTE等待；未复现新bug。[报告§20](iommu-platform/docs/composition-history-isolation-20261007.md#20-继续探索按可排空读段预约保留成本预研候选)／[Codex审定](iommu-platform/out/completion-reserve-exploration-20261007/codex-review.md)。下一先核write-burst匹配与返回接口成本，再决定2DMA小PoC，与普通pool＋准确owner门及walk强基线共比。仅研究文档/证据，源码/tests/构建/新实验/板卡无动作，最新实际仍8/8。DeepSeek两次证据/质疑509490 provider tokens，Codex创新设计、原源审定纠错。

**2026-10-07 普通准入强对照8/8通过；停止旧C首选创新推荐。** owner保守准入PN hot92857/cold93797，比原PN快约0.67%但FAST=0且仍慢于原P；新冷页源在目的fill后额外walk，V fill1→2。已有翻译后整walk额度在同buffer预热下P92334/PN91727，保留64 FAST及V fill1，A/V各64，PN比P快0.657%、A吞吐+0.662%。这是普通方法新强基线，非C实现/创新/板端收益；此前C“没人做过/实测差”均不能断言。下一先找强普通方法不能兼顾的合法反例或同效果成本差，当前未有。[报告§19](iommu-platform/docs/composition-history-isolation-20261007.md#19-普通准入强对照88通过停止旧c首选推荐)／[审定](iommu-platform/out/owner-baseline-20261007/codex-review.md)／[验收](iommu-platform/out/owner-baseline-20261007/acceptance.json)。仅tests3文件，一次成功SV构建（初次入口不支持input默认值已修正）＋一次C++重编；无生产/BIT/板卡动作。DeepSeek接口/草案270546 tokens，Codex设计/关键分析、执行验收。下方为此前阶段。

**2026-10-07 C可达性小判别：5/5通过，创新未成立。** 仅tests新冷目的页夹具，复用SV模型、C++编译/链接一次，旧热PN精确93487；冷页FIFO P92334/PN94427，优先返回干预P80252/PN79911，A/V均完成64。一般源先于自己的冷目的成立，但首V源非FAST且P也如此；PN额外A FAST读先于占W路径V的根PTE，FIFO服务延迟32→577，优先干预回32。候选C未实现/未升级；下一核额外prepare信息能否超出普通owner有界准入的合法决定，找不到就停C主贡献。仍性能主线；DS返回干预非已实现MIG对照，未改生产RTL/Linux/BIT或动板卡。[报告§18](iommu-platform/docs/composition-history-isolation-20261007.md#18-c可达性小判别一般源先行成立独立创新仍未成立)／[Codex审定](iommu-platform/out/c-readiness-check-20261007/codex-review.md)／[验收](iommu-platform/out/c-readiness-check-20261007/acceptance.json)。DeepSeek夹具/解析3次492799 provider tokens，Codex独立设计/关键分析、执行验收。下方为此前阶段。

**2026-10-07 调度＋微架构探索：Codex 独立设计，创新仍待证。** 用户明确要求Codex亲自做创新设计/分析，已写入AGENTS。独立提出“目的准备状态指导源读提前量”C和“可执行等待者触发逐段缩短”D；核关键源码/原文后D降为配套或普通对照，C先查AW-first/单TC下状态可达及与两端预翻译＋普通额度是否等价。[报告§17](iommu-platform/docs/composition-history-isolation-20261007.md#17-调度与微架构继续探索codex-独立设计候选仍待判别)／[Codex审定](iommu-platform/out/scheduling-microarchitecture-20261007/codex-review.md)。DeepSeek两份补充思考提案共413463 provider tokens，若干依赖/账本错误未采纳；Codex先独立设计、后核材料和最终判断。本轮无生产改动/构建/新实验/板卡操作，不把此前B的21/21当新候选验证。下一仅定向可达性与强普通等价判别；不自动扩矩阵或BIT。下方为此前阶段。

**2026-10-07 B最小差异验证完成：21/21通过，独立价值未证。** 同buffer P/N/PN为91874/151947/93487周期；B逐PTE补额92362（比PN快1.203%，仍比P慢0.531%），普通period1024为92227、整walk额度91091、在途额度92362。页表优先PN79491/B79486仅5周期差；A/V均完成64、同时比较吞吐。仅tests代理、一次Verilator构建，补齐同条件S/N；事件与排空验收通过。当前不继续推荐B主方法，下一先核完成依赖与强普通方法失效的真实反例，不自动扩矩阵或改生产RTL/BIT。[报告§16](iommu-platform/docs/composition-history-isolation-20261007.md#16-b最小差异验证普通对照仍更强)／[验收](iommu-platform/out/pte-progress-poc-20261007/acceptance.json)。非实板/MIG收益，复杂豁免未覆盖。DeepSeek4次草案/修订441009 provider tokens，Codex关键审查、修补、实际执行与审定；Linux/板卡/SD未操作。下方为此前判断。

**2026-10-07 两条规则创新查验完成：均未确立创新。** A最后一级多拍读FAST门控仅作工程对照；B入口确认＋按实际PTE阶段补额在已核原文未找到同式，但尚无强普通基线之外的独立效果/成本证据，保留待验证差异，不进入生产实现。[报告§15](iommu-platform/docs/composition-history-isolation-20261007.md#15-两条规则的创新查验一条工程对照一条未验证差异)。新补AXI-REALM/ABU原文，已有AXI入口预算、burst拆分、write-data-ready；其周期补额不等同PTE补额。直接核p4是事件收费、固定窗口补额，纠正DeepSeek初版等价论证及历史数据混淆；不以FSM可表达否决创新。DeepSeek2次证据/修订231129 provider tokens，Codex原源/关键源码审定；无新实验/实现/构建/板卡操作。下一项仅保留B差异价值判别，当前N同条件仍缺。

**2026-10-07 Codex和DeepSeek均提出具体新规则草案，创新仍未通过。** DeepSeek提“最后一级PTE期间拒绝多拍读FAST、回退普通路径”；Codex提“共享入口PTE确认＋按真实PTE进展计量的旁路承诺”，已纠正AR接收≠读不可抢占及预算保证范围。[报告§14](iommu-platform/docs/composition-history-isolation-20261007.md#14-codex与deepseek分别提出具体规则尚未通过创新预审)／[关键审定](iommu-platform/out/new-rule-20261007/codex-review.md)。MASK已覆盖walk-depth/PTE优先/数据配额，AuRORA已有动态request rate；两案只是待查候选，没有世界首创/收益验收。若继续，先与普通PTE优先、固定credit作同条件小模型对照，最终补N；没有启动实现/编译/新实验/板卡操作。DeepSeek1次完整提案218227 provider tokens，Codex也独立提案并核关键源/原论文。下方无新增模型调用是此前阶段。

**2026-10-07 用户指出四法均有前作，创新推荐已下调。** §12是工程入口与待核问题，撤回B首选/A优先的创新路线推荐；当前未交付可立项新增机制或具体反例。[审定§13](iommu-platform/docs/composition-history-isolation-20261007.md#13-用户指出均有前作撤回创新路线优先推荐)。MASK/BIS/Scheduling PTW/非叶缓存已有覆盖，DWS也有组合无额外收益前史；后续先明确强前作在真实约束下留下的具体差异，再决定概念验证，允许探索PN之外。Codex关键审定，无新增模型调用/实验/构建/板卡操作。下方§12优先级为用户质疑之前的阶段判断。

**2026-10-07 PN其他解法筛选完成。** 优先保留“按读写作用／完成依赖协调N”为候选，owner排空服务为需先证可达的备选；动态burst与小页表缓存作普通基线。已有93次模型PTE读中62次重复同两个上级项，仅频数证据、未测缓存收益；4KiB完整缓冲证书已能豁免普通读guard，不能把已有规则重新立题。关键性、翻译感知、缓存均有前史，创新未确认。[报告§12](iommu-platform/docs/composition-history-isolation-20261007.md#12-其他解法筛选请求作用完成依赖与页表访问削减)。DeepSeek提案/定向修订、Codex源码及原源审定；无生产改动/构建/新仿真/板卡操作。下一小判别先补测试侧角色/owner对应，再与§9普通准入方案比较，不重跑172。

**2026-10-07 PN第三项瓶颈与创新条件已核。** 页表优先、短burst、R→AW耦合、缓冲/信用是已有解法；当前coupler未综合、前端仅单拍deburst、banked只有单owner计数，不能直接取消guard。改变工作集可形成P/N互补条件；PN胜P/N仍只是性能证据，需具体协调/约束/因果结论及适用普通基线差异，尚未认定创新。[报告§11](iommu-platform/docs/composition-history-isolation-20261007.md#11-第三项瓶颈的现成解法与-pn-创新验收条件)。DeepSeek证据提案、Codex关键源码与原源审定；无实现/构建/新仿真/板卡操作，当前结果与下一小判别保持下方状态。

**2026-10-07 PN相关研究定向核查完成。** MASK已处理数据/翻译共享内存干扰；DWS §VII-D明确MASK+DWS组合可能无显著额外收益；DAC2023有简单分区变慢、命中率提高未转为加速的实例。另核Scheduling PTW、真实IOMMU Bermuda及NB-Walker正式议程/公开artifact（后者未获全文）。这些是强前作，不能直接归因本平台PN或认定创新；下一实验计划仍见下方板测条目。[报告§10](iommu-platform/docs/composition-history-isolation-20261007.md#10-pn组合收益与下游干扰的相关研究)。DeepSeek筛选，Codex核原文；无新仿真/板卡操作。

**2026-10-07 PN板端小对照18/18通过。** 同一isolated BIT/boot，A8、V64、gap0/1000/4000、每组3次；PN相对P的V耗时分别+12.85%/+3.73%/+0.08%，A有效速度−11.39%/−3.85%/+0.03%。gap0短段复现双方变慢；大gap增加V绝对耗时，且gap4000的N快速请求仅5–8，不能当作修复。V9没有板端PTE时序计数，尚未证明具体等待链或创新。约2分40秒，数据错误/taint/新内核告警0，未编译/重载/重启/写卡，Linux仍运行。下一步只在现成真实DMA仿真判别保留提前翻译、约束V数据读准入，再决定最小RTL改动，不重复gap矩阵。[报告§9](iommu-platform/docs/composition-history-isolation-20261007.md#9-同一-bit-与启动下的-v-供给间隔板测)／[验收](iommu-platform/out/pn-board-path-20261007/board-acceptance.json)。DeepSeek提案，Codex实际执行、关键审查与数值验收；本阶段未追加Claude。下方为此前阶段记录。

**2026-10-07 PN真实DMA仿真已发现一条因果机制，板上根因仍待核。** 复用现成真实两iDMA/DS50/50/shared-xbar，A8/gap0/V64四组数据及窗口完成检查通过。FIFO模型P91878/PN93480周期（PN慢1.744%）；仅窗口内DS读优先返回时P79847/PN79484（PN快0.455%）。93次DDR DS读地址顺序完全相同，PN服务等待另增5505周期，等于PTW active增量；事件显示V快通路数据读先于A叶子PTE进入DDR。仅本模型可达与干预证据，未完整解释板上12.74%；不宣称确定bug或创新。生产RTL/Linux、FPGA/板卡/SD均未改。下一步板端小反事实验证对应服务顺序，不自动添加大诊断硬件或重跑172。[报告§8](iommu-platform/docs/composition-history-isolation-20261007.md#8-真实-dma-小段复现与-ddr-因果对照)／[验收](iommu-platform/out/pn-data-path-20261007/acceptance.json)。DeepSeek接口/报告提案，Codex关键诊断、执行与审定；本阶段未追加Claude。

**2026-10-07 PN三方算法审核完成，根因未定。** DeepSeek首轮及修订、Claude独立有限审核后完整交叉审定、Codex关键源码/实际执行共同核查；尚无能解释PN额外约896482周期的确定bug，不代表无bug。否证“PN独有取消主预取”“跨bank AW owner过时”的当前实例反例。Codex复用既有TL模型补52个局部时序case均通过，A响应最多晚1周期（仅该测试范围）。既有板测差额全部在硬件执行phase，入口AW反压另增863488周期，仅为定位线索，不能归因或与其他计数相加。下一步复用actual_dma小段合法P/PN事件，保留DS50/50，区分页表读/数据突发/写队列等待；未执行完整路径复现，未改生产RTL/Linux、未重编FPGA/上板/写卡。[审核结论](iommu-platform/out/pn-algorithm-audit-20261007/audit-summary.json)／[报告§7](iommu-platform/docs/composition-history-isolation-20261007.md#7-pn-算法源码审核与定向时序检查)。

**2026-10-07 替换历史隔离已上板，22/22通过。** 固定2/14分区的PLRU历史隔离已实现；新BIT最终WNS +0.022ns/WHS +0.051ns，LUT156186。JTAG加载1次，Linux仍从SD启动，未更新Linux或写卡；boot_id `d61e7517-2b15-4702-86f2-3f338ddd5345`，网口操作正常，UART已释放。A8/gap0旧A miss为P5117/PN6582，隔离后两者均2048；绝对耗时P下降21.44%～21.82%、PN下降20.69%～20.74%，但新PN仍比P慢12.67%～12.79%。S/N跨版变化在0.28%以内。跨BIT/boot、每条件3次描述性结果；额外miss差消失不等于剩余退化已归因，更不证明创新。[报告](iommu-platform/docs/composition-history-isolation-20261007.md)／[板测证据](iommu-platform/out/composition-isolated-20261007/board-acceptance.json)。下一步先核合法DMA/TL事件顺序与约0.90M周期差额，性能主线保持，不复活固定A8的p5方案。DeepSeek起草脚本/测试/报告，Codex设计审查RTL并执行构建、上板及验收。以下为此前阶段记录。

**2026-10-07 中文汇报稿：** 已按日语稿另存[10页中文讲稿](iommu-platform/docs/research-progress-presentation-zh-20261007.md)，含逐页要点、口头稿、问答与证据索引。明确最新172为裁剪后的复跑、旧P/PN退化仍未归因、替换历史隔离仅是后续对照计划；日语原稿未改。DeepSeek起草/润色，Codex审核保存；35个本地链接有效，无新实验、编译或板卡操作。

**2026-10-07 汇报材料：** 已衔接10月2日旧稿，完成[新的日语逐页讲稿](iommu-platform/docs/research-progress-presentation-ja-20261007.md)，10页、约8分钟，口头稿约2267字符。内容为E2新实板证据→占用信息与p5负结果→同BIT四模式/P-PN退化→局部PLRU仿真→下一判别；172/172及资源削减只作基盘进展，附录解释旧84失败/补测与数据版本。DeepSeek起草，Codex核关键来源并修订边界/措辞；无新实验、编译、性能重算、上板或对外发送。板卡状态仍如下。

**2026-10-06 23:38 JST 当前：新裁剪 BIT 上板172/172全部通过。** short16/noise12/explore144，runner exit0、failed0，逐项problems空、V及参与的A数据错误0、taint0、恢复状态0；日志未见新内核告警。复用cpn3-clean同一启动 `05f32544-9e7d-4a2b-bc56-9b5dcabb8138`，约27分41秒完成；未重编/写卡/重载或额外预跑。Mac `results/composition-clean-20261006-formal01`全部原始数据及UART日志已回传，Linux保持运行、UART已释放。[板测验收](iommu-platform/out/composition-clean-20261006-board/board-acceptance.json)。这是新BIT的功能实验结论，尚未比较性能或证明创新；前次旧系统更新软锁死根因仍未确认。DeepSeek核现成入口，Codex执行及验收。下方“未重跑172”为此次用户授权之前的阶段。

**2026-10-06 23:03 JST 当前：裁剪 BIT 与配套 Linux 已完成编译和上板启动。** 最终 WNS +0.169 ns、WHS +0.038 ns，时序失败/布线错误均0；整板 LUT **156215，比上一版少9320**，FF127713、Slice50138/50950。Linux `6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty` 已网传、写入 SD 第一32MiB分区并同步，JTAG加载新BIT后SSH确认新内核/boot_id `05f32544-9e7d-4a2b-bc56-9b5dcabb8138`，Linux保持运行、UART已释放。[验收](iommu-platform/out/composition-clean-20261006/acceptance.json)。首次更新旧内核在`wc`读文件时软锁死，保存现场、重载旧BIT恢复后第二次更新成功（34分34秒）；根因尚未确认。编译期间按用户要求待机；未加哈希/回读、未重跑172或新研究实验。既有172/172属于旧BIT，当前只确认构建与更新启动闭环。下方“尚未完整构建/上板”为此前阶段。

**2026-10-06 当前：按用户澄清清理占 LUT 的历史诊断硬件，源码与定向验收完成。** 删除 USER4 卡死快照/AXI与原子单元诊断连线、附带计数器和约束；composition 关闭旧 `dma_path_observer`，保留正式窗口计数与 IOTLB 快照。裁剪接口 V9，Linux 已兼容 V9/V8；驱动对象交叉编译、7 个驱动场景、109 个观测器场景及10个联动场景通过。同条件**模块级**综合 LUT 14420→5561（−8859）、FF 9199→3222（−5977），不是整板节省量。[验收与构建 overlay](iommu-platform/out/debug-cleanup-20261006/acceptance.json)。尚未完整 FPGA 构建、打包新 Linux 或上板；部署裁剪 BIT 时需先更新配套 Linux，历史 BIT/日志/数据保留。

**同轮 P/PN 工程核查：历史已记载的共享 PLRU 四例现已完成。** 实际 IOTLB 与分区局部历史 shadow 对照：有效 V touch 后 A 替换 way8/2；touch=0 和同拍 fill 对照均2/2；后继页可使结果 miss/hit 或 hit/miss。4例/474次求值通过，原回归25834次求值通过，生产 IOTLB/TL 未改。[局部判别验收](iommu-platform/out/b1-causal-20261006/acceptance.json)。这证明容量分区未隔离替换历史；尚未证明完整 DMA 可达、板上 P→PN 退化归因或论文创新。后续研究先核合法请求序列，暂不实现新替换机制或重跑172项。

**2026-10-06 继续推进空间：仅保留B1一次低成本因果判别，未确认创新。** [审定§8](iommu-platform/docs/novelty-gate-20261006.md#8-用户再问推进空间保留一次因果判别不升级创新承诺)：复算旧A8/gap0的P→PN，份额不变、V miss均0，A miss5117→6582，W增加11.58%～12.22%；共享PLRU历史是待排除的普通解释。核A完成为独立lane事件计数，不接受“只缺另一个boot”。新增核DAAT-MCS/ECRTS2026及PLRU近邻，纠正SP-IMPact DOI为`.5`；下一项是短仿真事件序列的可行性/因果判别，尚未执行。DeepSeek证据草案、Codex原源核查和旧数据复算；无生产改动、构建、仿真、新实验/板卡操作。C2不通过结论保持。

**2026-10-06 最新创新裁定：当前C2机制表述不通过立项。** 用户追问是否已有；[明确答复](iommu-platform/docs/innovation-deeper-20261006.md#7-用户追问这些是否早已有过能否算创新)：预翻译/重放、R→AW耦合、缓冲配额/在途控制、失效进度均有直接前史；“少量依赖信息改善准入”仍只是问题描述，未给出有效新增机制或新结论。不将此前“还有空间”当立项推荐，不为此自动改RTL或加大实验。合理增量论文允许旧构件组合/新的因果结论，不要求全场景胜出；但当前差异证据不足。Codex直接核最近前作并作最终判断，无新增模型委派、生产变化、构建或实验。

**2026-10-06 最新：进一步核实早发写路径，C2仍未确立创新。** [审定](iommu-platform/docs/innovation-deeper-20261006.md)：实际RAW_COUPLING_AVAIL=0；serial/banked均允许目的物理AW先于源翻译，读写同拍生成不排除它。但CQ的IOFENCE PR/PW排空为TODO，当前没有所假定的完整更新环。更强普通基线是按接收域预留读返回空间＋保证已发写的数据不再依赖新翻译；限定模型内不必发读前拿齐两端翻译。另核Intel DSA既有翻译/部分完成/资源控制。K配对记录≠控制器槽/PTW数，不必锁两个TLB way。尚缺真实场景及超出这些普通办法的信息/资源增量，暂不升C2为论文主线。DeepSeek2次完整308551 provider tokens，Codex核关键源及规范并作条件推演；[记录](iommu-platform/out/innovation-deeper-20261006/README.md)。无生产改动、编译、新测试/仿真/实验或板卡操作。

**2026-10-06 最新：创新审查已推进到公开实现与普通基线，尚未确认新机制。** [审定](iommu-platform/docs/innovation-followup-20261006.md)：成功定向提取 ShareMMU v3 的20份源码，发现替换计分与论文伪代码不同、DMA接口不是双memory端；C1原公式不能直接当代码反例。普通逐读返回空间预留排除C2原读环；当前平台已有AW优先。进一步保留“已发写等尚未翻译的源读”的条件环，但真实可达性/更新协议未证，逐块成对预翻译与有效性处理是强普通解法。下一证据是合法请求序列及同约束资源账本，未放行大规模RTL/编译。DeepSeek3次完整返回374901 provider tokens，Codex直接核原文/关键源码并裁定；[记录](iommu-platform/out/innovation-followup-20261006/README.md)。无生产改动、构建、仿真、新实验或板卡操作，不重做限速。以下为此前阶段。

**2026-10-06 最新：进一步创新核查保留两个具体边界，仍未确认新机制。** [审定](iommu-platform/docs/innovation-focus-20261006.md)：C1 为 ShareMMU 历史共享计数与近期收益的分离，公式偏置可推演，但 SHARP/aging/加权 AA-LRU 是强近邻；C2 为有限缓冲 DMA 在翻译暂停／回失效下的排空契约，优先核协议语义，不能主张首次死锁。已核 Arm DTI 既有进度规则、2018 MMU-aware DMA 重放；取得 ShareMMU artifact 元数据与目录，源码区间 HTTP503，锁范围仍未知。两者均不自动放行 RTL 或大实验。DeepSeek4次完整返回499021 provider tokens，Codex核关键论文/规范/源码并裁定；[完整记录](iommu-platform/out/innovation-focus-20261006/README.md)。无生产代码改动、构建、新实验或板卡操作，不重做限速。

**2026-10-06 最新：实现前创新审查未放行 A/B 机制实现。** [审定与两张收窄卡](iommu-platform/docs/novelty-gate-20261006.md)补核TCWS/DAWS、NeuMMU/mNPUsim等：页感知源端调度、预测限流、突发与容量/服务耦合均有强前史；当前未给出足够具体的前作失效约束。A1“局部性与消费者等待”、B1“P/N组合边界”仅为待证问题，B1可复用旧证据查因果，未升级为论文主线。暂停下方旧“先软件基线/实现A”的安排，不重做限速。DeepSeek4次完整证据/修订共552503 provider tokens，Codex核原文及policy并裁定；[记录](iommu-platform/out/novelty-gate-20261006/README.md)。无生产源码改动、构建、新实验或板卡操作。下一项是具体工作流/前作差异与可推翻预测，满足后才做最低成本概念验证，不要求此前先取得完整性能结果。

**2026-10-06 限速历史纠正：** 用户提醒后确认已做P4M固定gap（pilot21/21，formal60记录24在线＋1离线追认、35未见续跑完成）及method88七方法88/88反馈准入。p5对四组匹配强基线均负；不能将最新172无A-gap扫描外推为全项目未限速。撤回从零建立基线建议，先复用旧结果/实现、明确新A相对旧方法差异，只补缺口。[修订审查](iommu-platform/docs/platform-feasibility-20261006.md)。DeepSeek定向证据、Codex核原记录并纠正；无新实验/编译/板卡操作。

**2026-10-06 实际平台可行性审查：** [审定说明](iommu-platform/docs/platform-feasibility-20261006.md)确认A可先仅改软件开放后台gap/页数、复用BIT测普通基线；逐任务反馈需接入当前禁用的ADMIT控制并重编。B可先离线分析/批次边界选择；当前172项没有A控制数据。当前最终Slice99.60%、WNS+0.031ns，不能按剩余LUT承诺扩展。五个关键RTL与ptwfix构建快照一致，三组旧指标复算完成。DeepSeek两组证据，Codex核关键路径/资源并裁定；无源码修改、构建或新实验。下一步建议软件基线曲线，有稳定缺口才加控制RTL。

**2026-10-06 创新边界补判：** [首篇方案§6](iommu-platform/docs/first-paper-options-20261006.md#6-用户追问这两点能否突破已有研究边界)明确A提交控制、B配置选择目前仅为入口；简单页预算/四模式扫描不等于突破。优先检验“强普通规则在合理工作流中判断错误—遗漏信息/约束—小机制修正—未见条件净收益”，B可支撑A形成一篇。Codex直接核地图及强前作，无新DeepSeek/Claude调用、实现或实验，未确认创新。

## 2026-10-06 用户收窄目标：博一首篇合理论文，优先小切口

[新选题方案](iommu-platform/docs/first-paper-options-20261006.md)：首选核“页面工作集感知DMA提交控制”，备选“共享IOMMU配置适用边界与选择规则”。长期四候选保留；不再把大跨设备依赖框架设为首篇入口，也不把PLRU修复/增加日志单独当贡献。下一证据为强普通基线的敏感任务耗时—后台有效吞吐曲线及实际接口可行性；机制/收益尚未验证。DeepSeek两组共292480 tokens（high/393216/完整），Codex核Bermuda、IOTLB Wall、rIOMMU、SP-IMPact关键段并审定；[记录](iommu-platform/out/first-paper-options-20261006/README.md)。未新增实验/代码/编译/上板，172/172原义保持；发表门槛待用户明确。

## 2026-10-06 四候选比较矩阵完成，开始按可证伪问题收敛

[对照与最小验证规格](iommu-platform/docs/translation-comparison-20261006.md)将强前作、真实场景/假设、约束、基线和停止条件合并。①跨设备依赖优先核真实翻译路径及收益空间；③观测先支撑①/②；②生命周期条件保留、机制实施暂缓；④表示共存暂缓宽泛主张。补核 ShadowUpdate/HELM 对主张的限制，关键全文仍有缺口，本轮无新增 PDF。DeepSeek 两组提取/草拟共715497 provider tokens，Codex核关键原文、分层与闭环判据后审定；[完整记录](iommu-platform/out/translation-comparison-20261006/README.md)。下一产物是实际应用路径卡，尚未采集轨迹/仿真/修改源码/构建/上板；172/172仍仅为既有功能验收。

## 2026-10-06 后续补充判断：补路径与语义，开始收敛

[第三轮审计](iommu-platform/out/shared-translation-literature-20261006/gap-audit-03/README.md)新增核5项论文/综述来源、取得4篇全文，另核GPUDirect RDMA文档。VIO补设备直通/缺页软件基线；官方GPUDirect路径要求1:1/直通，不能直接当共享IOMMU翻译争用应用证据。已同步候选说明。接下来定向补关键全文、真实地址路径、注册/撤销/同步和测量方法；现在开始一至两个候选的强前作矩阵，不等穷尽文献。主123项和上一批20项统计不变；DeepSeek一次86896 tokens、Codex核来源及关键语义，无实验/源码/板卡操作。

## 2026-10-06 文献缺口审计：20项优先补充、16项全文已取得

[补充说明](iommu-platform/docs/shared-translation-gaps-20261006.md)与[资料/审计](iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md)已保存。新核PFM、CounterPoint、SPMC、Trail、UVM故障合并、Dead on Arrival等直接前沿，并补BIS/Criticality/Coz/MISE/ASM/RMM等强前作；原123项主目录和19项缺全文状态不变，未把新下载计为全部已精读。Codex直接核5项关键段，PFM是PA→硬件地址双视图而非改VA→PA，仍限制第四候选宽泛新意；CounterPoint不是在线最优控制。四候选文档已收紧，未证创新。DeepSeek两次完整返回合计212971 tokens（high/393216）；无代码/RTL/Linux/板卡变化或新实验。

## 2026-10-06 基于123项地图筛选创新候选

已完成[四项候选与强前作/停止条件](iommu-platform/docs/translation-opportunities-20261006.md)：优先验证跨设备计算依赖指导翻译服务；保留映射生命周期下的共享范围选择；因果观测作为支撑/条件性方法课题；不同设备翻译表示共存为高风险探索。新核2024年ECMDQ讨论已有ATS队头阻塞/无关失效并行，CATO作者索引已有CXL争用感知卸载，故普通多命令队列与拥塞选路降级。DeepSeek一次294288 tokens证据聚合（high/393216、无截断），Codex直接核关键原文与规范并作排序；未证创新、未采集新轨迹/仿真/改RTL或上板。详见[本轮审计](iommu-platform/out/translation-opportunities-20261006/audit.json)。

## 2026-10-06 共享翻译文献继续拓宽：123项与七条深化脉络

- 最新交付：[深化综述](iommu-platform/docs/shared-translation-deepening-20261006.md)、[完整文献库](iommu-platform/out/shared-translation-literature-20261006/README.md)。专题从72增至123项独立工作，104项有全文/105版本，19项仅题录/摘要；新增43项全文工作、8项题录，另补全CLAP和Translation Ranger。经典扩至1989年，前沿截至2026-10-06，112已发表/6已接收/5预印本。
- 深化内容：七条演进脉络、六组困难与强前作/否定条件、20篇分阶段必读。CLAP已有结合迁移的扩展；SoftWalker最高调度优先级与cuPTW空闲时发射不同，原错误引用关系已纠正；Hydra跟踪页表级NUMA sharer；Soar/Alto与PACT已研究关键性。研究价值仍需真实应用和同约束强基线，未确认创新。
- 本轮DeepSeek 13次成功调用（thinking/high、393216、无截断）完成分组证据/题录/工具提案/草稿修订；Codex实际执行检索下载并直接核关键原文、应用和验收。新45版本中42下载、3复用已有广域地图。完整[增补审计](iommu-platform/out/shared-translation-literature-20261006/expansion-01/audit.json)保留原答复和纠正；不调用Claude或新增Codex子agent。
- 目录/导航验收通过：123条CSV/BibTeX、105份PDF与卡、781个本地跳转、20篇阅读清单；不是论文复现。ShadowUpdate/LATPC/Clover/NB-Walker/CATO/CDFD/LIBRA等仍待全文。无生产代码、RTL/Linux、构建、上板、写卡或新实验；172/172功能验收不变。下一步仍先核具体应用与强前作条件，不自动启动机制实现。

**2026-10-06 最新：按用户要求完成共享地址翻译专题的大规模文献收集与瓶颈地图。** 72项独立工作，59项有全文／60个版本，13项为题录或摘要；覆盖1993—2026，已补多GPU、分布式翻译、排队、迁移与最新执行路径。DeepSeek完成8组全文证据卡，Codex核12项关键原文并纠正题录／版本；目录、CSV、BibTeX与403个本地链接检查通过。基本共享机制有成熟前史，具体创新仍未确立；下一步应针对应用条件比较最强前作，优先补ShadowUpdate、CLAP、LATPC、Clover、NB-Walker、CATO、LAMP全文。当前不自动执行此前候选实验。[专题地图](iommu-platform/docs/shared-translation-literature-20261006.md) · [72项目录](iommu-platform/out/shared-translation-literature-20261006/INDEX.md) · [来源与阅读记录](iommu-platform/out/shared-translation-literature-20261006/README.md)。本轮无RTL/Linux修改、构建、上板或SD操作，既有172/172验收保留。下方为历史阶段。

**2026-10-06 最新：6方向×Codex/Claude/DeepSeek共18席研究与交叉审核完成。** 18份独立提案+18份审核，26个正式候选；Claude充值后已补齐，当前无余额阻塞。尚无候选确立论文创新：已淘汰直接重叠或论证错误的主张，其他收窄为有停止条件的问题。下一步建议先做三份证据卡：控制命令对无关设备进度、搬运批次与消费者开工、GPU观测及时性；未启动这些后续审计/实验，不自动转安全或按FPGA适配度选题。[审定结果](iommu-platform/docs/innovation-council18-20261006.md) · [36份答复与审计](iommu-platform/out/innovation-council18-20261005/README.md)。研究地图已补新近邻及Tiara评估类型修正；本轮无RTL/Linux修改、新实验/仿真/构建/上板/SD操作，既有172/172功能验收保留。下方为历史阶段。

**2026-10-05 最新：按用户要求，先完成经典—前沿—瓶颈研究地图，再选论文命题。** 已整理 36 篇独立论文的 38 份全文版本及 4 份官方文档，覆盖翻译、共享干扰、I/O 管理、搬运接口、动态分配、迁移与观测；另列 4 项局部来源。DeepSeek 分组提取与起草，Codex 核关键原文、题录/版本与适用边界；纠正 vAttention 仅列旧预印本、IDYLL 实测/仿真混写等问题。此前三候选取消预设优先级，不自动转安全、不先做 PLRU 对照；下一步按真实应用条件对照最强适用先行工作。具体创新未确立，本轮无源码/测试/构建/上板/SD操作，172/172 功能验收保留。[研究地图](iommu-platform/docs/research-landscape-20261005.md) · [来源与调用记录](iommu-platform/out/research-landscape-20261005/README.md)。下方为此前安排。

**2026-10-05 最新：用户要求跳出普通方案，研究优先级改为开放创新搜索。** 已完成两份 DeepSeek 独立提案及 Codex 原始文献核查；此前四个 PLRU 对照转为工程诊断待办，不再是论文探索前置条件。优先核查“零散、动态映射内存上的稀疏访存加速”，另保留跨设备布局视图、取消后的细粒度回收两项发散问题；均未证新颖性或收益，不自动转安全。纠正模型将实验模式配置 quiet 误当通用 IOMMU 失效限制的推断。下一交付为应用需求/先行工作差异与最小性能空间评估，未开始 RTL/实验/构建/板测，172/172 功能验收保留。[最新筛选与边界](iommu-platform/docs/performance-council-decision-20261005.md#7-开放创新搜索问题先于当前实现) · [两份原始提案](iommu-platform/out/innovation-search-20261005/)。下条是本次用户质疑前的历史安排。

**2026-10-05 最新：第二次六席五轮讨论完成，继续性能主线。** Codex2、DeepSeek3、Claude1共30份正式答复；研究问题收窄为有限在途DMA中，命中绕过/缓存保护经替换历史与共享服务影响双方进度。源码确认共享PLRU依赖，但尚未归因板上退化或证明论文创新。下一交付：复用现有IOTLB测试，完成显式touch的四个局部对照与实际结果，再按缺口选择联合片段或最小实物观察。[审定计划](iommu-platform/docs/performance-council-decision-20261005.md) · [本次记录](iommu-platform/out/performance-council-20261005/README.md)。当前仅规格已完成，新增测试/构建/板测未执行；172/172验收保留。DeepSeek整理证据与草案，Claude参与质疑，Codex核关键来源并裁决。

**2026-10-05 最新研究决定（用户质疑后修订）：继续多DMA共享IOMMU干扰与缓解的性能主线。** 先围绕已有PN退化、N局部收益与gap效应明确竞争解释及最小区分设计；内存安全复用接口研究降为备选。五轮否定的是具体提案尚未证明创新，不足以推导应放弃性能领域；根Codex纠正先前优先级。具体论文贡献仍未成立。[修订计划](iommu-platform/docs/paper-council-decision-20261005.md) · [五轮原始记录](iommu-platform/out/paper-council-20261005/README.md)。Codex2、DeepSeek3、Claude1的30份历史答复保留；本次没有新模型调用、实验、编译或SD操作，既有172/172验收保留。

**2026-10-05 当前：** 172/172 正式验收通过，两处历史源码 bug 已修复；结果已回传，WORKFLOW.md、平台 docs 与根 MAC_FPGA_HANDOFF.md 已应用并固化 172 结论，Linux 仍运行、SD 未写、串口/JTAG 已释放。现行 BIT 见 [验收](iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json)，流程见 [composition-v1](iommu-platform/docs/composition-v1-mac-20261004.md)。

**下方为历史进展，其中“正在运行／尚未通过”指当时状态。**

**2026-10-05 新BIT纯网口实验在第53项停止：52项完成、1项失败、119项未运行；板卡已恢复。** short16＋noise12＋explore24完成；`explore-b1-g1000-a8-mpn` 后网口失联，UART接回后也无响应，根因未确认。原始结果已完整回传，不自动重跑。JTAG重载同一route-opt BIT后，net3/SSH恢复，boot_id `476dcf54-3dfb-441f-b95c-3440589736f5`、taint0、carrier1，UART启动日志已保存；本轮累计JTAG2次、SD写0次。现阶段实验保留UART诊断，日常操作走网口；下一步针对失败项定位卡住的位置。[结果记录](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/RESULTS.md) · [实际状态](iommu-platform/out/composition-board-net3-20261005/execution-state.json)。

**2026-10-05 新上板请求：用户重新授权新 BIT 完成及时序通过后执行既定172项。** Mac现成脚本及UART已就绪、无占用；沿用已通过net3 Linux/SD，只重载指定BIT，不重复软件构建或写卡。当前Dell无运行中的Vivado，最新已通过产物仍是03:06 composition route-opt；已向用户确认是否这份或另一新编译，未选定/加载BIT、实验0项。[本次准备与待确认状态](iommu-platform/out/composition-board-net3-20261005/execution-state.json)。

**2026-10-05 最新：Linux纯网口更新流程已固化并通过实板。** cpn2-net3从SD启动后自动获得192.168.77.2，SSH读到新boot_id 1a955cce-bcdd-4f0a-9012-6e0c7eda8322；Mac直连IP也已持久保存。复用mac_update_reboot入口：网传→仅32MiB p1写卡/同步→指定BIT JTAG重载→SSH验新启动，exit0、实际写卡/JTAG各1次、实验0项。日常可拔UART，保留JTAG；本轮用旧已验收N v5 BIT、不改RTL。整轮34.45分钟，速度尚未优化。[固定入口](iommu-platform/docs/software-update-20261004.md) · [实板结果](iommu-platform/out/ethernet-workflow-20261005/ethernet-workflow-20261005-03/result.json)。

**2026-10-05 03:06 JST：composition优化版最终构建验收通过。** 原base仍未接受；单次布线后物理优化exit0，WNS+0.058ns/WHS+0.052ns/WPWS+0.062ns，三类时序失败0、全部273893线路布通/error0、组合/锁存环0，nativeBIT DRC错误0。可交接BIT为`iommu-platform/out/composition-trim-20261005/route-opt/ariane_xilinx.bit`（11443722字节），MCS31472748字节。最终LUT163673/203800=80.31%，Slice占99.45%，与先前综合数字分开。沿用原RTL/50MHz/约束/ABI、现有Linux/SD；未上传Mac、未碰UART/JTAG或启动172项。仍不等于板测/完整IO/CDC签核。[验收及产物](iommu-platform/out/composition-trim-20261005/acceptance.json) · [修复说明](iommu-platform/docs/composition-trim-20261005.md)。以下running/未通过为先前阶段。

**2026-10-05 02:49 JST：裁剪构建exit0，但时序未验收；用户要求继续修复。** 全部273890线路布通、错误0；建立WNS−0.380ns/失败30，保持+0.052ns与脉宽+0.062ns均失败0。原BIT/MCS已生成但不能上板，[验收未过记录](iommu-platform/out/composition-trim-20261005/base-acceptance.json)。报告/检查点已由Makefile移至该stage的`work-fpga/`，下方running为早期快照。已从已布通DCP启动单次物理优化（PID3230656），保持原RTL/50MHz/约束/接口；状态在`route-opt/execution-state.json`，完成后验收最终报告，不更改另一对话Linux/板卡工作。

**2026-10-05 工作区实际清理完成**：已结束的method88五脚本/三测试（4,796行）、两目录测试生成代码、三份`.orig/.rej`残留移出开发目录至[归档清单](platform-archives/retired-code-20261005/manifest.json)。两个测试默认输出改入`out/`；当前composition计划/串口及E2/rootfs模块实际加载通过，活动源码无method88引用。共享依赖、原始数据和当前独立FPGA编译目录保留；未重启编译或动板卡。DeepSeek审查依赖、Codex核对并执行归档。

**2026-10-05 02:06 JST RTL修复后重新完整编译中**：用户要求处理旧构建失败，Codex仅在composition profile裁掉两套旧观测器，保留本轮快照、计数和方法逻辑；10项联合仿真与4项构建入口检查通过。独立构建`out/composition-trim-20261005/`，PID3110218，尚未验收BIT或最终布线资源；同阶段综合LUT已减少11.42%、寄存器减少8.68%；不改Linux、不重做SD、不占另一对话的板卡/UART，不自动启动172项。DeepSeek起草辅助脚本、测试和交接，Codex负责关键RTL、执行与验收。[修复说明](iommu-platform/docs/composition-trim-20261005.md)。下方旧composition失败是此前构建结果，新构建以本条路径为准。

**2026-10-05 最新：Linux百兆网口更新闭环已通过。** Mac网传29363656B → FPGA只写SD的32MiB第一分区并同步exit0 → 同旧N v5 BIT JTAG再加载 → 实际cpn2-net2启动，boot_id f7e78b10-b4a5-44b5-9114-10d0aa21251a，eth0/192.168.77.2、carrier1。SD全程未拔、仅改Linux驱动开放100/1000全双工、未改RTL/BootROM、实验0项；Linux保持运行、UART释放。自动开机IP/永久拔UART尚待；传输+写卡约24.3分钟，速度待优化。原composition编译已失败、172暂不启动。[完整结果与原始证据](iommu-platform/out/composition-board-execution-20261005/LINUX_ETHERNET_STATUS.md)。

**2026-10-05 01:40 JST最终构建结果：composition-v1恢复编译失败，exit1。** 31处布线冲突，最终WNS−3.268ns/WHS+0.042ns；Tcl门槛未过，无BIT/MCS，不能上板。失败检查点/报告保留，后续由另一对话接手修复再编译，用户已插好配套cpn1 SD。见 [交接说明](iommu-platform/docs/composition-handoff-20261005.md)。以下running/生成报告为较早快照。

**2026-10-05 01:39 JST编译实查更新**：composition-v1恢复布线结束但未合法布通，Route 35-2确认31处线路冲突、35-77为completed with failures。失败布线DCP已保存，程序当时仍生成报告，无BIT/MCS；必须以最终state/报告继续处理，不能上板或把route_design命令正常返回当验收成功。后续仍由另一对话接手，参见 [接手说明](iommu-platform/docs/composition-handoff-20261005.md)。

**最新范围收窄（2026-10-05）**：用户明确“只需要测试能否加载linux，不要进行实验”。本轮只验证旧已通过BIT下新版Linux启动及以太网更新/再次启动，172项暂不启动，不自动运行任何实验用例。现有新BIT编译保留。

**最新安排（2026-10-05）**：按用户补充要求，等待新BIT期间先用已通过板测的旧N v5 BIT测试cpn2-net1启动、以太网传Linux/板端SD写入/JTAG重载。原composition恢复编译继续，最终完成及时序通过后再一次运行固定172项；不因调整顺序重编或重跑旧实验。

**2026-10-05 本对话已接手等待与上板**：用户明确授权现有 composition 恢复编译完成且最终时序通过后，SSH Mac 一次执行固定172项并完整回传，再测试以太网下载Linux→板端写32MiB启动分区→同步→同BIT JTAG重载。UART/JTAG/Ethernet均由用户确认已连接，SD已插回FPGA。宿主机实际确认PID3064145/3064187仍在，保留原构建、不重复编译。版本按最后实际写卡cpn2-net1记录准备，并在启动后读实际uname匹配入口；下条原转交的cpn1为较早版本快照。[接手状态](iommu-platform/out/composition-board-execution-20261005/execution-state.json)。

**2026-10-05 composition-v1转交另一对话**：最后实际核查00:44 JST，独立恢复编译仍在Phase4.3布线，冲突节点194，尚无BIT；PID3064145/3064187保留，禁止重复启动。用户已写入cpn1配套SD并插入FPGA，授权接手在编译及最终时序通过后直接SSH Mac加载并运行既定172项。本对话不继续上板，未安装自动上板任务；后续读取实时state。[直接接手说明](iommu-platform/docs/composition-handoff-20261005.md)。下方cpn2-net1是另一软件任务，不替换本轮已写SD的cpn1。

**协作工具配置（2026-10-04）**：用户授权选择模型及配置密钥，已配置官方 `claude-opus-5-5`、自适应思考/high、初始生成上限16,384；隐藏输入与注册程序准备完成，113 项离线检查通过。真实密钥仍待用户在Dell终端输入，尚未注册 MCP 或实际请求 Claude。**只有用户主动明确点名时，才用于当次议题的计划、创造力与三方交叉讨论/审计**；日常分工保持。下一步运行输入程序，成功后短检查/私有保存/工具注册；以后以实际连接摘要为准。见 [使用说明](tools/claude-advisor/README.md) 和 HANDOFF 本日条目。此配置不改变下述研究/构建状态。

**最新用户要求（2026-10-05）：通过 SSH Mac 将新版 Linux 写入已插入的 SD；追求部署速度，复用现有脚本，不增加下载哈希、写后回读或额外测试。上一版 RTL 仍由原任务处理，本轮不改 RTL。**

**部署流程已核对（2026-10-05）**：按用户要求读取两端现成脚本，已在 [MAC_FPGA_HANDOFF.md](MAC_FPGA_HANDOFF.md) 补当前入口索引。写卡/旧 BIT 的默认值均指向历史版本，须显式选择；四模式 runner 只 attach，不负责加载 BIT。Mac 检查位置尚无新版重启 helper 与四模式 BIT，SD 已写完不重复写。本轮只读源码/文件状态并记录交接，未部署、测试或核查另一任务的实时构建。

## 2026-10-05 当前：新版 Linux 已写入 SD，同步并安全推出

- 已在 Mac 复用 `scripts/write_research_sd.py`，仅追加本次软件版本选项；写入 `/dev/rdisk5s1` 的 32 MiB 原始启动分区，payload 29363656 字节、内核 `cpn2-net1`，其余分区不写入。
- 现有脚本完成写入、零填充、fsync/sync 和安全推出，退出码 0。按用户要求未新增下载哈希、写后回读或额外测试；[原始日志与结果](iommu-platform/out/sd-install-20261005/receipt.json)已回传保存，Mac 交接也已更新。
- 下一步：用户将 SD 插回 FPGA；后续上板任务加载已确认的 BIT 并启动新 Linux，再接通网口更新。当前未加载 BIT 或启动验证，不声明板端 SD/网口已可用。本次由 Codex 执行现有脚本，未调用 DeepSeek。

## 2026-10-04 软件阶段：SD 常驻更新软件构建完成

- Linux/OpenSBI 已实际构建成功，内核 `6.5.0-rc2-cpn2-net1-00093-gc0f5e13ea81f-dirty`；`fw_payload.bin` 29363656 字节，能放入 32 MiB 启动分区。独立[软件交付包](iommu-platform/out/software-update-20261004/software-update-20261004.tar.gz)与[操作说明](iommu-platform/docs/software-update-20261004.md)已准备，旧 cpn1 产物和交付目录保留。
- 已开启 Xilinx SPI/MMC/SD 驱动，适配实际 lowRISC 网口控制器及 Realtek PHY，修正 SPI/网口 PLIC 中断单元。打包新增静态网络设置、HTTP 下载和分区更新工具；只写经核对的第一项 32 MiB 原始分区，完成 fsync 后才报成功。
- 65 项主机测试/实际接收 RTL 仿真通过，驱动单独编译与代码检查通过；最终核验当前 rootfs→Image→fw_payload 的嵌入关系、驱动配置和交付文件哈希。[验收证据](iommu-platform/out/software-update-20261004/embedded-artifact-acceptance.json)。这些不代替真实 SD/PHY/中断验证。
- 当前 BIT 缺 CPU 可控复位设备，普通 `reboot` 尚不可作为已交付能力。软件补了 OpenSBI 标准复位设备初始化；当前正常更新使用 Mac 串口同步确认→JTAG 重载同一已验证 BIT→验证目标内核和新 boot_id。静态 IP 每次启动后需重新配置，暂未完成纯网口控制/配置持久化或坏镜像自动回退。
- 软件阶段未写真实 SD、上板、重启或修改 RTL；首次 SD 安装已于 2026-10-05 完成，以上节为准。板端 SD/网口和 JTAG 更新闭环仍待接通；后续部署遵守用户最新的速度与复用要求，不额外重复验收。DeepSeek 起草工具/测试/文档并定向修订；Codex审查并修正关键驱动/分区边界、执行测试构建和交付核验。

## 2026-10-04 前一阶段：五轮论文路线审查完成；四模式已实现，Linux完成、曾核查BIT布线中断

- 已完成Codex与DeepSeek的独立提案、交叉攻击、重构、三类审稿与收敛。主线为同BIT下P2/14缓存份额与N命中先服务的可行性验证；已有技术本身不算创新，method88负结果与v5未确立收益保留。
- 新RTL、独立Linux ioctl、CLI和172项Mac执行入口已实现；Linux/OpenSBI完整构建成功，内核 `6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty`。缓存/观测/工具及Mac入口测试已通过，补充TL组合仿真修订后日志为PASS。尚无本轮BIT、未开始新板测；最近交接记载SD未写，用户已允许本次更新SD。
- 22:38–22:43 JST只读核查：FPGA日志最后更新20:43:36，停在Phase4.3布线；启动记录PID2993231/2993275均已不存在，主机无Vivado/make，JSON的running已过期。中断原因及退出码尚未确立；中间WNS−1.377ns与拥塞警告不是最终时序结论。Mac“上板”聊天也确认中断，正在诊断、暂不加载。先查中断并恢复构建，再验收最终时序/产物、交付与执行16+12+144项；本轮核查未重建或上板。证据见[验证记录](iommu-platform/docs/validation.md)。
- [五轮结论](iommu-platform/docs/paper-route-five-round-review-20261004.md) · [冻结接口与172项分阶段计划](iommu-platform/docs/composition-v1-spec-20261004.md)。
- 后续已按用户要求完成 SD 常驻更新的软件阶段，状态以上节为准；本节 cpn1 与 BIT 中断核查是较早记录。

## 2026-10-04 历史：v5 两版30/30完成并分析，按用户要求暂停（无需SD）

- v5 S/N两版各15/15完成，runner退出0，全部原始数据已回传Dell；既有分析器退出0、complete=True、quality_issues=0。无新增数据错误/内核警告，逐项taint_after=0。长组每条件3次；N的快速处理计数A8每轮6144/6146/6146、A16每轮8193，Solo/A1及S版均0。N/S速度中位变化A8+4.15%、A16−2.69%；N相对原版A8−14.52%、A16−22.09%。A8三轮104.263/92.987/93.048 MiB/s的波动全部保留。机制已实际触发，但尚不能宣布性能改善成功；不同BIT/新启动、原版结构与缓冲差异限制因果归因，n=3仅描述。
- N当前板卡boot `c2bd10b5-26e7-4de6-ac4f-d3a04c14498c`，Linux `00093`，runner已退出、UART已释放；Linux/SD未更新，两版BIT构建及既有约束内时序通过。未启动下一轮。
- [昨夜以来做了什么](iommu-platform/docs/research-overnight-progress-20261004.md)；[本轮结果分析](iommu-platform/out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md)。N原型线完整功能通过105项，原版15项复用不重复计；另有凌晨method88公平对照88/88完成，p5对最强匹配简单方案四格全负，该主方案路线已结束。两线分列，早期启动失败/部分样本与主机模型不并入。

**下方为历史阶段记录；其中“正在运行/待编译”等不代表当前状态。**

## 2026-10-04 最新：v4 两版BIT完成，S15项通过、N板测进行中，无需更新SD

- v3 原版/S/N 共45项技术通过，但N全部快速接收计数为0；不能据此评价绕过收益。
- 已只读核查实际设备上下文：DMA DID10/11 均配置 flat MSI（msiptp.mode=1），v3快路径仅接受 MSI mode0。这是已确认的静态兼容性限制，尚未证明它是未触发的唯一原因；DDTP mode2本身受支持。
- Codex已实现v4有界修正：flat MSI下仅允许输入及缓存物理地址都不属于MSI范围的普通4KiB请求；MSI/保留模式仍走原路径。新增独立被动诊断bank，旧16字接口保持。
- TL、4KiB交错代理模型、MSI边界/背压和observer双模式验证实际通过；DeepSeek工具提案已由Codex应用验证，诊断检查21/21、分析器12/12通过，静态读取程序已编译并传Mac。S版完整编译exit0，WNS+0.124ns/WHS+0.046ns/时序失败0；BIT/MCS均非空，S v4已15/15技术通过，exit0/problems空/taint0/无内核警告，诊断bank有效，原始记录已回传。N版完整编译exit0，WNS+0.177ns/WHS+0.039ns/时序失败0，产物已传Mac并启动N v4 15项板测。Mac当前N v3 Linux00093仍运行；不需要人工SD、旧数据不重跑。
- v3产物保留于 `iommu-platform/out/n-prototype-20261004/v3-accepted-builds/`，迁移映射 `RELOCATION.json`；实际上下文证据 `v4-live-context-evidence.json`，构建状态 `v4-build-state.json`。编译完成后做S/N v4各15项新试验，检查快路径是否真正触发及双方吞吐。

## 2026-10-04 N v2整份缓冲模型通过，但FPGA资源超限；v3两版BIT完成、S/N各15项通过、N绕过接收全0，诊断规划中（无需SD更新）

- 原版对照15/15已完成，无数据错误；长试验V中位Solo282.81/A1 149.28/A8 108.86/A16 100.69 MiB/s，原版不重跑。
- v0接收计数全0/短A8停滞；v1修接线/读接纳且两BIT时序通过，但追加轮询双突发重叠模型banked8/12失败、legacy12/12通过，v1功能拒绝、未上板。旧构建与原始失败证据已保留。
- v2只给DMA0/1每路4KiB缓冲（512拍，旧24B/3拍）；S/N相同，DMA2/3仍3拍。净增数据8144B，MetaFifoDepth11→520，实际面积/时序待综合。现有读接纳守卫保留，TL缓存/查表器/槽/W FIFO不增。
- v2轮询双突发重叠模型banked12/12、legacy12/12通过；是有限模型，不是完整真实DMA/DDR覆盖。两版完整BIT构建均exit1，Place 30-487布局失败：combined LUT S207069/N207226超过203800，total LUT S237003/N237306；控制集11563导致额外打包压力。新增深FIFO FF/逻辑实现过大，正在审查RAM映射，未产出可接受v2 BIT；原Linux继续运行。
- v3已局部复用既有AsyncDpRam存储分支：FIFO新增默认关闭选项，只对512深度×8位DMA数据字节通道启用；全局FPGA_EN保持0，FIFO指针/握手/清空逻辑不改。实际FIFO等价测试9/9通过（454278检查、13150周期），生成模型确认RAM/寄存器分支选择；两版v3 BIT/MCS已完成：LUT S173518/N173667，LUT memory均5639；WNS S+0.177/N+0.145ns，保持+0.031/+0.038ns，时序失败0。S初始构建在BIT生成后Vivado打开报告时崩溃，已复用生成产物补齐MCS（exit0）；未重新综合。S版15/15通过，exit0/检查问题空/taint0，原始结果已回传；N版同样15/15通过，所有fast V/A接收计数均0，normal/accepted非零；不能判定绕过收益或认定根因。完整三版45项离线汇总complete=True/quality_issues0，DeepSeek整理证据和诊断草案，Codex审查关键门控。失败v2目录已迁移至placement-failed-v2-builds，保留清单。
- 当前4KiB页对齐batch一次只发一个job、等整份完成，限定了整缓冲论证范围；不外推一般AXI无死锁。旧原版→S比较包含布局+缓冲合并开销，S/N同容量才评估绕过增量。创新/收益均未成立。
- Mac仍运行恢复的原版Linux00093、boot956fa8d0-0bf3-4104-a8b1-2af736624a01；v1 BIT虽已传Mac但未加载。无需重编Linux/人工SD；原Linux/静态reader/P4M ABI沿用。
- [规格](iommu-platform/docs/research-n-prototype-spec-20261004.md) · [诊断](iommu-platform/docs/nonblock-dependency-diagnosis-20261004.md)。DeepSeek起草仿真/工具/文档，Codex负责关键RTL、工具执行与验收。

## 2026-10-04 最新：method88 88/88 完成，固定 A8 实占主创新路线结束

- 状态：method88 已正式完成。88/88 项 host_exit0，原始记录已回传；正式报告 `iommu-platform/out/method88-execution-20261004/RESULTS.md`；文献矩阵 `iommu-platform/docs/research-literature-review-20261004.md`。
- 结论：固定 A8 / target=170 的实占计费（p5）对最强匹配简单对照四个启动×方向单元格全负（-5.68952% / -8.23689% / -5.15142% / -4.43680%），跨方向 False。按预定条件结束固定 A8 主创新路线；p5 转为已测试候选/基线。不继续为 p5 找有利条件。
- 执行事实：kernel `6.5.0-rc2-00093-gc0f5e13ea81f`；boot 1 身份 `93406a68-fc8a-4040-9dc6-ae4e4a670d36`（复用既有启动，有既往 462 项活动）；boot 2 身份 `236b6316-cca3-49be-b52b-48f3963fa515`（新启动）；4 short + 84 long，每启动 44 项。UART 已 live 返回 uname `00093` + bootid second，确认 second boot，无需重启；串口已关闭。
- E2 新版 64 项已取得局部替换直接证据；E3 有有限命中阻塞机制证据。两者均不等于解释全部共跑损失。
- 下一步：已否决同设备上下文的初版 N 草案。完成[跨上下文原型约束](iommu-platform/docs/research-n-prototype-spec-20261004.md)，仍须闭合完整请求/结果身份、顺序与失效接口，尚未就绪实现。先行工作已有命中绕过、分区和限流，新方法创新尚未成立。

[88项结果](iommu-platform/out/method88-execution-20261004/RESULTS.md) · [文献与下一步](iommu-platform/docs/research-literature-review-20261004.md)。DeepSeek整理报告、文献与规格；Codex核对关键证据、执行并作最终决策。

下方待回传、未分析或未上板条目均为历史状态，以本节及链接报告为准。


## 2026-10-04 00:52 role2三次核对完成：仍待实际进度或数据

按本轮用户授权，于00:22:53、00:37:53、00:52:53（15/30/45分钟）检查Dell，同批目录仍仅E2，无method结果/完成标记，也无新回传目录或交接更新。只能确认数据尚未回传，不能判断Mac板端完成/运行/失败状态。三次检查进程退出0，无后续自动监视。需要Mac实际进度或完整method目录后再分析并定计划；当前保留冻结计划，E2局部机制验收结论不变。[检查记录](iommu-platform/out/role2-status-checks-20261004-0007/STATUS_CHECKS.md)。本轮纯机械核对，未调用DeepSeek、重跑、调参或编译。

## 2026-10-03 23:45 role2定时核对：三次已完成，数据待回传

按用户要求于23:35/23:40/23:45检查Dell同批目录，均未见method结果或完成标记；仅E2已回传，状态/交接未变。无法据此判断Mac板端是否已完成，暂不分析方法收益。三次检查已结束，不自动续设监视；收到完整method数据后按冻结判据分析再定计划。[检查记录](iommu-platform/out/role2-status-checks-20261003-2330/STATUS_CHECKS.md)。E2局部机制验收结论不变，缓存保护仅为条件满足后再研究的分支。

## 2026-10-03 最新：新版E2 64项离线验收完成，E2阶段收尾

- Linux00093/role1的新版64项（短4＋边界30＋前缀30）冻结分析器64 ok，最终汇总64 clean/exit0；1,226份输入SHA未变。归档与板端记录、参数、exit/taint核对通过，逐项dmesg无新增、stderr空。首项tar兼容故障只重新采集已存在的记录，未重跑。
- E2直接证据：V预热驻留→A提交替换→探测前消失→A停止排空后V首探未命中。W7正反各5次保留两目标，W8正反各5次均替换；全64共62个目标替换后首miss、66个保持命中，无未决类别。128目标不是128独立试验。
- 边界V单次4KiB耗时中位：W7正反均696周期（13.92µs），W8正/反1483/1486周期（29.66/29.72µs）；全部64首次接纳均5周期，增加的是翻译服务占用，不能称纯排队。结论限定本实现/布局/访问方式，单次代价不是持续吞吐或方法收益。
- [完整报告](iommu-platform/out/e2-results-review-20261003/RESULTS.md)。E2暂不补测、不改Linux/BIT、不重新上板。下一主线待Mac方法462回传后按冻结规则评估公平保护下A进度、开销和独立验证；按最新交接方法仍在运行，本轮未分析方法数据，不宣称收益或创新。
- DeepSeek整理证据、起草脚本/报告；Codex关键源码与原值复核、执行验收、修订汇总及实验决策。新版路径待定位已解决。下方0/526未上板等为交付时历史状态。

## 2026-10-03 两BIT共用软件 v2：代码、编译、时序验收和交付完成

- 一份Linux `00093-gc0f5e13ea81f`＋E2(role1)/方法(role2)两BIT已准备好；当前00092缺新接口，只需集中更新SD一次，随后Mac经JTAG切BIT并重启。新板测0/526，尚未传输或上板。
- E2 64项＋方法462项已冻结；Mac工具先保存并校验E2结果再切方法BIT，短验收→校准选参→冻结对照→独立验证。技术错误停止、不自动重跑、所有原始样本保留。方法收益/创新仍待板测。
- E2最终WNS +0.177ns；方法版经物理优化WNS +0.048ns。两者建立/保持/脉宽失败端点与组合回路均0；旧IO/clock/CDC约束全面审查仍待完成。方法基础构建的负时序BIT不可用于上板，必须使用包内role2.bit；原失败产物保留。
- [交付包](iommu-platform/out/research-two-bit-20261003/research-two-bit-20261003.tar.gz)，21,734,376字节，SHA256 `95fbddda5488230e7bca03dcd0890883f3a9cef5201a06f6b1bec03242eb045f`；[Mac说明](iommu-platform/docs/research-two-bit-mac-20261003.md)、[验收](iommu-platform/out/research-two-bit-20261003/acceptance.json)、[解包核验](iommu-platform/out/research-two-bit-20261003/package-smoke.json)。30个包成员/全部SHA/工具依赖/冻结计划已实际核对。
- 实现与主机测试见 [验收记录](iommu-platform/docs/two-bit-build-acceptance-20261003.md)，合同见 [research-two-bit-v2.md](iommu-platform/docs/research-two-bit-v2.md)。软件构建481bd47、E2构建10fd189、方法基础构建7430772/CVA6a1b08934；独立优化manifest保留真实检查点/Tcl/实际退出码/输出哈希；交付工具提交0a570de。
- 下一步：交给Mac按包内说明更新一次payload、顺序执行两BIT并回传完整目录。DeepSeek起草实现/测试/交付脚本，Codex审查关键RTL、实际执行测试/编译与最终验收。正常编译日志没有巡查，只在失败时读错误并在结束后检查必要报告。

以下为历史阶段记录，当前状态以本节为准。

## 2026-10-03 最新：fullstats21离线接受，下一轮准备成套方法实验

**最新交付偏好：一份共用Linux＋两份兼容BIT，交给Mac端AI轮流执行。** 用户可接受一次上板期间JTAG换BIT，避免为切换实验人工更换SD镜像。优先沿用当前SD；两BIT与现有00092的兼容性待验证，必要软件支持交付前集中补齐。计划Mac依次加载/启动核对/验收/实验/保存结果，再切换另一BIT；E2/M分别准备，不强塞同BIT或互为前置。自动流程和新实现尚未完成；本轮无构建。[最新部署计划](iommu-platform/docs/research-next-single-deployment-20261003.md)。

- 本轮21/21在线与离线通过；Codex直接核对全部21份raw计数/吞吐，198份回传文件SHA256未变，无重跑或构建。[完整结果](iommu-platform/out/research-fullstats-review-20261003/RESULTS.md)
- 固定16抽样存在组覆盖偏差：A8gap1000漏掉1536/2047份两miss搬运。全量同4KiB/req4/miss2占用均值仍随gap约1408/1101/841周期；简单计数不能精确表示成本，但未排除丰富类型计数，也未证明M控制收益。A16前两次V miss2保留，短A8 hrtimer警告与板端哈希来源限制保留。
- 旧E2局部替换、E3有限阻塞证据及P4M60保留。下一步进入有界M原型/公平基线准备；创新尚未成立。[下一轮同包六组实验](iommu-platform/docs/research-next-single-deployment-20261003.md)
- 用户强调一次上板多收集有用证据：同包主方法、保护强度、工作集/反例、计费依据对照、未参与调参条件、开销稳定性。自动短验收→有限校准→冻结比较→独立验证；科学阴性继续预定用例，正确性错误停止。精确运行总表/模式接口须板前冻结；新控制方法尚未实现或编译。本轮仅分析和计划，不重跑旧E2/fullstats、不另烧观测版。


## 2026-10-03 最新：fullstats v1已上板，21/21在线完成

新BIT＋Linux00092，同次启动cost-fullstats固定21项原runner通过，退出0、collection_failed0。完整原始结果在iommu-platform/out/mac-research-fullstats-20261003；尚未独立离线语义核查/统计分析，不作方法M收益结论。Linux保持运行、UART释放；无额外试次、旧E2重跑或代码/构建操作。下方0/21未上板为历史。

## 2026-10-02 最新：全量分组统计实现、软件/FPGA构建及交付完成

- 全量覆盖共同硬件窗口内完整A搬运，按req/miss分六组记录次数、总和和范围；旧每16次抽样保留。未实现方法M，新板测0/21。[规格与验收](iommu-platform/docs/research-fullstats-v1.md)
- 新 `cost-fullstats` 固定21项：3短＋Solo/A1g0/A8g0/A16g0/A8g1000/A8g2000各3主实验。原84计划不变，首错即停、不自动重跑，旧E2不重跑，不跨版本拼接。
- 主机测试通过；软件和FPGA完整构建退出0。内核 `00092-g5853e425caf8`，CVA6 `0dc5e333`，平台实现 `3f060f3`。首次沙箱许可证连接失败已保留；授权网络构建成功。正常编译日志未巡查，无新增时钟约束结论。
- 交付包 `iommu-platform/out/research-fullstats-20261002/research-fullstats-20261002.tar.gz`，24,405,743字节；包SHA256 `9080fe9e95b25756b3718dff88469d7738bfcc13729fb9f220a572b8c2a7146f`。新BIT与payload均必需，包内SHA及解包成员已核对。[Mac上板说明](iommu-platform/docs/research-fullstats-mac.md)
- 下一步：Mac按说明同包加载新BIT＋镜像，在新启动运行一次21项，回传整目录/UART/版本与哈希后离线分析。Dell不接板，本次没有执行板测或替换Mac文件。DeepSeek起草实现/测试/文档；Codex审查关键接口并执行验证、编译和打包。

## 2026-10-02 最新：00091 cost-followup 11/11 离线核查通过

- Dell用现有分析器复算退出0、11/11 ok；直接从原JSON raw C19/P4M window3和完成字节数核对全部11份，Solo A无实际活动、数据错误0。105份分析输入及完整回传115份文件SHA均未变。[结果与下一步](iommu-platform/out/research-followup-review-20261002/RESULTS.md)
- 主实验各3次中位：Solo V283.485 MiB/s；A8gap0 V/A108.857/108.857；A8gap2000 V/A171.488/57.115。V耗时56.440/146.982/93.301ms；共跑相对Solo耗时2.604/1.653倍。gap2000对gap0的V速率+57.54%、A速率−47.53%，仅本版本次启动描述。
- 9个主实验V硬件窗口16384次查找均命中、miss0；不能由此把全部损失归因IOMMU。原E2/E3局部证据保留，新11不与旧00090拼接；旧84仍80/2/2。短A8阶段hrtimer慢中断记录保留，taint0不代表无警告。
- 相同4KiB/req4/miss2的抽样占用，中位gap0为1406/1405/1408周期，gap2000为832/836/832。已完成固定计数解释边界检查，仅这些计数的固定权重不能同时精确还原两组值；未否定更丰富类型计数，也未证明M增量控制价值。
- 下一步建议先补测量：审查事件类型覆盖，设计共同窗口内完整A搬运的全量分组聚合统计，排除每16次固定抽样相位偏向，再决定小型M原型与公平基线。具体规格待冻结；本轮没有实现新RTL/驱动、编译或重跑板测。

## 2026-10-02 最新板测：Solo修复版11/11在线完成

Linux00091＋原研究BIT，同次新启动cost-followup 11/11原runner通过，退出0，Solo恢复正常。完整原始结果回传iommu-platform/out/mac-research-followup-20261002。尚未离线语义核查/统计分析；旧84项状态保留，不跨版本拼接，不重跑E2。Linux保持运行，UART已释放。下方“新版0/11未上板”为历史状态。

## 2026-10-02 Mac 分工交接与 Solo 修复软件包已完成

- FPGA 接在 Mac，Dell 负责源码/编译/离线分析；[Mac 说明](iommu-platform/docs/mac-followup-20261002.md) 已写好，当前工具不能直接发送到 Mac 任务，尚未传输或上板。
- Solo 兼容修复已审查、测试并提交：Linux `9ad05d00cca7`，新内核 `6.5.0-rc2-00091-g9ad05d00cca7`；软件构建退出 0。沿用现有研究 BIT `7e7024fc…`，不重编 FPGA、不裁 Linux、不改 SD/bootrom。
- 下一次为固定 `cost-followup` 11 项：2 短（Solo/A8g0）＋3 条件各 3 主（Solo/A8g0/A8g2000）。同新软件同启动对照；旧 E2 不重跑，新 Solo 不与旧 non-Solo 拼接。新板测 **0/11**。
- 主机验证：计划/runner 40、分析器 63、真实驱动适配/validator 提取与旧行变异测试 1 项通过；镜像 rootfs 中 runner/计划/CLI 与来源一致。驱动测试使用宿主机 stub，不是真实 DMA 执行。
- 三段原数据复算仍 **80 成功/2 失败/2 未运行**，709 文件 SHA 不变；21 条 E2 时点误报及14条 cost 关窗锁存误报已修正，原失败/不完整状态及原始内核警告保留。固定每16次抽样相位偏向仍是限制。
- [交付包](iommu-platform/out/research-followup-20261002/research-followup-software-20261002.tar.gz)；[执行说明](iommu-platform/out/research-followup-20261002/delivery/RUN_FIRST.txt)；[验收记录](iommu-platform/out/research-followup-20261002/acceptance.json)。构建正常日志仅保存，未反复检查。

## 2026-10-02 启动慢核查：SD整分区读32 MiB，镜像约28 MiB

用户明确慢在FPGA从SD加载，并询问串口/JTAG替代。当前payload 29,363,656B（28.003 MiB）、Image 23.013 MiB，内含已gzip压缩rootfs约3.257 MiB。启动日志首分区LBA 0x800–0x107ff共32 MiB；bootrom按完整分区读取，所以只裁镜像不改读取范围，SD读取量仍不变。[尺寸记录](iommu-platform/out/image-size-review-20261002/boot-measurements.json)

有USB/SATA/其他平台内置驱动及旧E3用户工具可裁剪候选；当前research、P4M/验收工具、watchdog、基础库和实验相关功能保留，不采用DeepSeek误将idma_research列为旧工具的建议。JTAG已有CVA6调试模块、Genesys2引脚和OpenOCD配置，可探索DDR加载但未实测速度；当前115200串口按8N1传本镜像理论下限约42.5分钟，且bootrom无串口收镜像流程。建议先减少无效SD读取，再按依赖裁剪；JTAG需一次连接/加载/启动和计时验证。仅分析，无删减、SD修改、编译或板端操作。

## 2026-10-02 Solo 已定位；完成两轮讨论和原数据初审，未启动编译

- cost Solo 将零A页数传入旧公共校验，在 DMA／映射／观测启动前返回 EINVAL；E2 W0和双路cost不受这一失败条件影响。工作区已有未提交修复草案，本轮未改、未构建或部署。
- 三段逐项检查80ok/2失败，原计划另2未运行；709份输入哈希未变，82个已尝试ID无重复。已核对临时runner仅选取原计划子集；全84分析仍failed/incomplete，不伪合并原始数据。[审计摘要](iommu-platform/out/research-board-review-20261002/cross-segment-summary.json)
- E2主试次W0/1/4/7各10次两目标保持命中，W8/16各10次出现A覆盖后首miss。21份E2附加警告与14份cost警告已定位为分析时点／关窗冻结状态口径，逐例核对保留；须修脚本后原数据重算，不为此重跑采集。cost固定每16次抽样有相位偏向，样本分布不能外推总体。
- 下一步建议：先修分析口径；审查Solo修复、真实驱动链回归后，同BIT更新软件，下一次上板先短Solo验收，再同版同段比较Solo/A8 gap0/gap2000。完整五条件slowdown则需覆盖全部五种双路条件；不把新Solo与旧80跨版本配对。完整M与采样RTL改动暂不启动。DeepSeek两轮质疑，Codex源码和证据复核；本轮无编译／新板测。

## 2026-10-02 最新：Research可运行项完成，累计80通过/2失败/2未运行

用户要求继续后80项，本轮新增77项CLI/原runner通过，主Solo1项Invalid argument，另2项同参数Solo未运行；合计E2 63项与非Solo cost17项通过，2项Solo失败、2项未运行。不是84/84完成，未独立语义分析；完整原始证据回传iommu-platform/out/mac-research-cont-20261002，首次4项在原目录。Linux00090保持运行、UART释放，未重跑/改驱动RTL/构建；Solo问题待后续处理，下方3/84为历史记录。

## 2026-10-02 最新板测：Research v1 在第4项停止

Linux00090/新配套BIT已上板，84次计划实际3项完成、第4项short-cost-solo因Invalid argument退出1、后80项未运行。未重跑/修改代码/额外验收；Linux保持运行。完整证据已回传iommu-platform/out/mac-research-v1-20261002/并核对一致；详细见README/HANDOFF。根因待后续诊断，本轮未实施修复；下方“0/84未上板”为历史状态。

## 2026-10-02 日语发表原稿已整理

已按用户要求写成 [日语 Markdown 原稿](iommu-platform/docs/research-progress-presentation-ja-20261002.md)：约8分钟、10页建议内容、逐页讲稿、结果表及答疑。使用完整60份 P4M 结果，明确 E3 有限机制证据、E2 未证与新84次已编译但0/84未上板。仅准备文本，未制作 PPT/PDF；尚未确认实际发表。DeepSeek 返回格式异常，无有效稿件，本稿由 Codex 接手完成并核对。

## 2026-10-02 最新交付：同包 84 次诊断已实现并完整编译，未上板

- 冻结 6 次短验收＋E2 六条件各 10 次＋自然占用六条件各 3 次；[计划](iommu-platform/docs/experiment-plan.md#2026-10-02-执行规格e2-与自然占用诊断同包-84-次)。用于补 E2 生命周期证据、筛选实际占用计时的价值；未实现完整 M，创新和改善收益均未证明。
- 软件／FPGA 完整构建均退出 0。Linux `00090-ga8d28cd44f35`，CVA6 `8c1e08b3`，顶层 `6e4ec65`。BIT `7e7024fc…`，payload `9eb5bc20…`；[交付说明](iommu-platform/out/next-experiment-20261002/delivery/RUN_FIRST.txt)、[完整包](iommu-platform/out/next-experiment-20261002/research-v1-20261002.tar.gz)。各产物／原始构建清单和校验值已保留。
- 主机：新 observer 164 检查；实际 IOTLB 对照 703 次；P4M 含 research 配置 60 场景；CLI 56、分析 46、runner 33、rootfs 13 项通过。修复记录 RAM 综合问题，并仅在研究 profile 关闭旧 E3 详细追踪以满足布局容量；旧 E3 仍用旧 BIT。未声称跨 BIT 性能等价，原完整时钟约束审查仍独立待办。
- **下一步由 Mac 上板**：配套载入 BIT 和 payload 后执行 `platform-research all /tmp/research-v1-01`；已包含短验收，遇错停止、不重跑、不删样本。回传整个输出目录后离线分析。当前本包板测 **0/84**；不能把主机验证当成 E2 阳性或方法有效。
- 下方同日“只文档／未新增代码”等属于前一阶段历史，以本条为当前状态。

## 2026-10-02 最新裁决：正式五轮交叉审查完成

- Codex与DeepSeek完成用户指定的五轮；第一轮本轮新提案相互隔离并保存冻结记录，后续交叉攻击、收敛及三领域角色审稿均已留档。[最终研究链与路线](iommu-platform/docs/paper-logic.md#2026-10-02-五轮交叉审查最终裁决)；[五轮原文](iommu-platform/out/research-five-rounds-20261002/)。旧两轮预审另计，原始提案不是执行规格。
- 七项含重叠的候选收敛为一个有条件M：检验按A实际翻译控制占用时间计费，是否比字节、请求/miss及按类型加权计数、简单V反馈更有用。IOMMU信用、反馈、时间额度均有先行工作；**创新及收益未证明**，不把两模型意见一致当证据。
- P4M60完成、E3有限因果证据保留、E2未证。下一步E2条目生命周期实验与自然4KiB成本诊断并行准备，单板分开执行；先冻结接口/小RTL验证。N/P及预审R不再默认完整实施，M也未证明超过它们。
- 尽量同包提供E2和诊断；完整M/强基线工程须看自然成本诊断是否值得做，可能需后续部署，不承诺一次烧录。正式参数、重复数和统计阈值待校准预注册。
- 本轮只审查计划/更新文档；未新增代码、构建、仿真或板测。下方历史条目按本条解读。

## 2026-10-02 最新裁决：两轮交叉审查完成，创新尚未成立

- Codex与DeepSeek完成两轮实际质疑/回应，并核对关键源码与原始文献。[裁决与路线](iommu-platform/docs/paper-logic.md#2026-10-02-交叉审查裁决路线保留先验证方法是否值得实现)；原文在`iommu-platform/out/route-cross-review-20261002/`，其中被否决建议不可直接执行。
- P4M60份已齐、E3有限机制证据保留；E2未证明。普通非阻塞N、静态预留P属常规对照；“联合资源依赖与顺序约束选择保护位置/容量”的R仍为假设，缺可执行规则与创新差异，不能包装为已形成方法。
- E2基于现串行S：主矩阵60候选保留，原前缀30次降选配、事前冻结；拟同包增加少量4KiB自然流量诊断，定位等待机会。另一线并行做生命周期规格与小型实际RTL原型；完整性能工程须通过方法门槛，不等E2阳性，不承诺同次烧录两个架构。
- 本轮仅文档、源码和文献复核，无新RTL/驱动/runner、无构建/仿真/板测。下一步是记录接口与被动观测规格、E3顺序/生命周期和可证伪的方法假设；不是立即编译完整新架构。下面同日90次、直接推进N的段落为上一版历史。

## 2026-10-02 最新分析：P4M 60份复算完成，E2／改善原型并行计划已更新

- 原25＋保留6＋新启动29＝60份，每gap20，53在线＋7离线追认。Dell只读复算退出0，必需失败／警告／缺项均0，191份输入SHA256未变；分析自检通过，未重跑板测。跨启动与采集段标记保留。[完整结果](iommu-platform/out/p4m-complete-review-20261002/RESULTS.md)
- gap0/1000/2000：V吞吐中位108.813／150.603／171.389 MiB/s；同硬件窗口A为108.813／75.301／57.088。固定等待收益与代价基线完成本轮收尾，暂不追加gap扫描；未宣称最优或统计等价。
- **新边界：** b15-g0的V整轮miss/S1 PTW=2/2，其余59份为0/0；该份gap0反而最快且启动／开窗状态不同。全部保留；整轮计数无法定位发生相位，不能据此宣称缓存替换或因果。旧“全部V零miss”不能用于完整60份。
- 两条支线：[最新并行计划](iommu-platform/docs/paper-logic.md)。E2保留单次部署6条件主矩阵，增加A8仅完成1/8/16次的前缀对照，候选90次（含各10次工程重复，短验收另计），原布局小复核延后；E3改善先做合理普通非阻塞，必要才同容量静态预留。开发／主机验证并行，单板顺序，每支线交付一个完整自动执行包；不承诺两个硬件路线同次烧录。
- 本轮仅分析脚本、结果报告与计划／交接更新；未实现新增实验硬件／驱动，未构建或启动E2／改善板测。下一步是共用记录接口、E2观测与runner、E3请求生命周期规格的并行准备。

## 2026-10-02 最新完成状态：P4M剩余29次已补齐

- 29/29原runner检查通过，exit0、数据错误0、完成数正确、taint0。完整结果已回传并内容核对一致：`iommu-platform/out/mac-p4m-remaining29-20261002/`。
- 原25份＋额外6份离线复核＋本次29份＝60份，每gap20（53在线＋7离线追认）。断电后新启动补缺，不是同一启动60次在线通过，后续分析须保留分段。
- Linux00089保持运行，UART已释放。没有追加实验、构建或E2操作；下面E2建议仍为候选，不自动执行。详情见结果目录README及HANDOFF最新条目；旧reported_running由本条更新。

## 2026-10-02 最新补充：续跑进行中；建议 E2 最小实验并行

- 用户报告剩余 35 次正在运行（reported_running）；本轮未接收或审计新增结果，完成数未知。已复核基线仍为 25 次可用，不标记 60 次完成。下方历史段落的“35 未跑”不再代表最新运行状态。
- 建议将 E2 作为与 E3／改善设计并行的小规模机制验证分支，不作为后者的前置条件：V 预热→A 有限访问→A 完全停止排空→V 首次探测，包含等时无 A、小重复集合、更大集合三类对照。范围及判据见 [实验计划 E2](iommu-platform/docs/experiment-plan.md)。
- 仅 miss/PTW 增加不能直接认定 IOTLB 容量替换，需排除主动失效、上下文变化等原因并核对条目变化。E2 结果用于判断改善方案是否需要处理翻译记录受损；不自动要求双机制联合方案，也不把有限阴性结果推广到所有条件。本轮仅评议和状态交接，未启动 E2 实现、构建或板测。
- 用户要求减少重复上板：已完善为同一 BIT＋软件包的 E2 单次部署设计，短验收→主矩阵→预设小复核自动执行。候选 W=0/1/4/7/8/16，运行时参数切换，补齐目标条目生命周期与探测前快照，避免只有 miss 而不能判断替换。每条件 10 次、A 总次数 112 为待核验冻结的工程候选；方案详见 [实验计划末节](iommu-platform/docs/experiment-plan.md)。仍为设计，未实现、未编译、未上板；不承诺一次调试即成功。

## 2026-09-20 P4M formal60 recovery：采集修复已应用，resume35 prepared_not_run

原始在线运行仍为 24 在线通过、25 attempted、35 unrun、runner exit5；第 25 次 formal-b09-g2000 因 `startup_line_count before=36 after=36 want=37` 停止，判定为 runner 记录计数假设失效，不是 DMA 失败或新硬件根因。第 25 次作为离线复核追认保留，原始 run.json 与 flags 不改写，不伪称 25 在线通过。

采集修复脚本 `iommu-platform/tests/board-p4m-formal60.sh` 新 resume35 模式已由 Codex 应用，交付复制到 `iommu-platform/out/p4m-formal60-recovery-20260920/delivery/platform-p4m-formal60`。resume35 只跑 flat26..60，分配 12/12/11，合并已有 8/8/9 可成 20/20/20。Dell 无接板，本次 35 尚未运行，状态 prepared_not_run；Mac 侧按串口步骤执行后只读归档再合并审计。主机验证：test_board_p4m_start.py 41 项、test_p4m_formal60_recovery.py 16 项、test_p4m_formal60_resume.py 15 项通过；shell syntax 0；offline_revalidate.py 25/25 all_passed=true；原始 14 份 SHA 机械核对一致，25 份 before-dmesg 错误文件皆空。详见 [恢复报告](iommu-platform/docs/p4m-formal60-recovery.md) 与 [续跑交付说明](iommu-platform/out/p4m-formal60-recovery-20260920/delivery/README.md)。

## 2026-09-20 P4M 启动控制软件：软件构建通过，板上未跑

本轮完成 P4M 启动控制软件（驱动 batch start、测试、runner、finalizer）与配套文档：DeepSeek 起草，Codex 复核并执行。`make software PROFILE=migration-4dma-p4m JOBS=12` 退出 0（2026-09-20 12:12–12:13 UTC）。

- 版本：内核 `6.5.0-rc2-00089-gefd9461d6459`，Linux `efd9461d6459049f59c9ea5466f3703e45413bb3`，顶层 `a08426f46720b646ae0fa052a0a27136a347b69f`，CVA6 `e822bc9d`、IOMMU `137f5e5`；新内核是独立软件基线，不与 `00088` 混算。
- 硬件不变：沿用既有 BIT `7af1ccbc…`（未重建 FPGA），新 payload `fc8e8a73…`；交付目录 `out/p4m-start-control-20260920/delivery`，产物哈希、rootfs 内的新脚本及复用 BIT 来源均已核验。
- 主机检查 6 套件全 0：batch-start 10 场景、scheduler、controlled、CLI、rootfs、board-p4m-start runner 41 例（含主机编译 CLI 与 BusyBox ash/awk）；均为主机检查，非板级证据。
- 板上未跑（board_tested=false）：无功能/性能结论。板卡命令 `platform-p4m-start all /tmp/p4m-start-01`；UART 与日志由 Mac 侧捕获，Dell 未接板。
- 协议、字段契约、runner 门限与判据见 [P4M 启动控制协议](iommu-platform/P4M_START_CONTROL.md)，验证见 [validation.md](iommu-platform/docs/validation.md)，产物见 [out](iommu-platform/out/p4m-start-control-20260920/)。
- 下一步：4 短试次 + 8 比较试次（4 对）均只作描述，正式参数未冻结。

## 2026-09-20 P4M pilot-21 完成（历史保留；21/21 与 gap500 反例不变）

P4M 板上短验收已执行：`platform-p4m short /tmp/p4m-v7-01`，Solo/A1gap0/A8gap0/A8gap1000 四例 64 jobs、warmup 8、4 KiB，板卡退出 0、final taint=0、collection_failed=0。原始证据：[console.raw.log](iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/console.raw.log)、[records.json](iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/records.json)、[run.json](iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/run.json)。复核脚本与报告：[p4m-short-results-review.md](iommu-platform/docs/p4m-short-results-review.md)、[review.json](iommu-platform/out/p4m-short-review-20260920/review.json)。修订后复核结果：正常复核 exit 0，自测 PASS，原始输入哈希未变。

版本仍为 P4M profile `migration-4dma-p4m`/observer v7，内核 `6.5.0-rc2-00088-g9f8da9e7a6a0`，CVA6 `e822bc9d8adf`，Linux `9f8da9e7a6a0`，IOMMU `137f5e5fb1f2`。短验收只做功能验收，不证明正式收益；n=1。

P4M-1/P4M-3 短验收功能完成；P4M-2 的 legacy `--batch` 与 `--p4m` gap0 显式对照已在 P4M-4 pilot-21 中执行，但结论未定（启动相位/预窗口历史混淆，n=3 仅描述性）。P4M-4 pilot-21 已完成：21/21 功能通过，原始记录与全量结果见 [p4m-pilot-results-review.md](iommu-platform/docs/p4m-pilot-results-review.md)、[RESULTS.md](iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/RESULTS.md)。正式 gap 点与重复数未冻结；保留 0/500/1000/2000 候选和 gap500 反例，不把 n=3 写成正式等价或正式收益。第三次 gap0 的开窗 A ID 为 1448，前两次均为 4；这表明开窗前 A 运行量不同。下一步优先收紧两路启动间隔并记录触发时间，先于正式 P4M-5；本轮只做分析/文档，不改代码、不跑板、不构建。

## 2026-09-18 E3 v6 上板及服务器复核完成（历史）

E3 v6 / Linux00087 已完成 3 对短验收 + 8 对正式实验；服务器独立复核 exit 0、9 组检查通过。11 对 / 22 轮、418 条唯一 UART、114 条相同重复去重，11 份完整 CLI JSON 与 UART 对象逐字段一致，原始输入哈希未变；8 项程序检查（正常对照 1、故障注入 7）均符合预期。原始回传在 [mac-e3-controlled](iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/)，[复核证据](iommu-platform/out/e3-controlled-review-20260918/server-review.json)。run.json 的 pending_upload 是上传前快照；现已在 Dell，未宣称验证 upload receipt。

核心结果：4 对 cold 的 D128−D0，A 剩余服务和 V 剩余等待均增加 127/129/129/130 周期，V 自身翻译恒为 4 周期；14 个 cold 轮次 V 被选中均比 A 翻译响应晚 9 周期。身份、目标 A 的 PTE、前缀计数和完整插入身份配对一致，V 首查命中；D0/D0 的剩余等待差为 −2/0。结合串行控制器/AR FIFO 代码，支持本实现、此配置下 A miss 引起 V hit 等待的机制。样本仅 4 对，预触发耗时并非全部相同；不宣称完整 E3 矩阵完成、所有旧 A8 损失已解释或已有方法收益。[结果分析](iommu-platform/docs/e3-controlled-results-review.md)。

整体方向保留，当前优先推进 P4M：共同 V 硬件窗口内的 A 完成量与边界在途计数，以及 A 完整拷贝结束后的硬件固定间隔基线。先验证 gap0/短正确性，再预试、冻结少量参数并画 V slowdown—A 完成速率曲线；具体间隔和重复数待预试，不自动扩旧 E3 矩阵。E2 按证据触发，P5 在规则冻结后评价。[下一步实施卡](iommu-platform/docs/e3-next-method-plan.md)。该阶段只做分析程序和文档；DeepSeek 提供程序/文档草案，Codex 核对关键证据与 RTL、做科学决策并实际执行复核。

## 平台、版本与证据边界

- 开发目录 `iommu-platform/`；Genesys 2、50 MHz、4 DMA、IOTLB 16、DS50/50，CPU 保留完成轮询。原工程和 archives 只作历史参考。
- 最新板上版本（已完成短验收与 21 次预试）：P4M observer v7 / Linux00088，交付版本 CVA6 `e822bc9d`、Linux `9f8da9e7a6a0`、IOMMU `137f5e5`。原始启动日志确认内核；BIT/payload 下载哈希与 SD 回读证据仍未补齐。上板与回传由 Mac 完成，Dell 本机仍未接板。
- [v6 配套产物](iommu-platform/out/e3-controlled-20260918/delivery/README.md)的软硬件构建已通过；该阶段未改实验源码或重新构建。旧 build-status 的 board 状态是交付时快照。
- 时钟/CDC/外部 IO 审查、下载哈希和 SD 回读仍有未验证边界；本轮 run.json 的 smoke_passed/hpm_calibrated 为 null，不补写通过。详见 [验证记录](iommu-platform/docs/validation.md)。
- E1 共跑现象、硬件连续供给、DS 回切复现及分段等待定位证据保留。v6 是独立 64 B 机制诊断；下一阶段回到既有连续 4 KiB 工作量，两类数据不混算。
- 已获得范围明确的 E3 干预证据，尚无经过评价的缓解方法或已确认创新点。总体目标仍是方法论文；E2 与剩余 E3/E4 分支按方法设计需要展开。

## 工作规则

- 开任务先读本文件；需要行为约定读 [AGENTS.md](AGENTS.md)，平台内读 [iommu-platform/AGENTS.md](iommu-platform/AGENTS.md)。
- 按任务定向读说明和代码；不要默认扫描整个 Linux、`.git`、`out/` 或恢复副本；没有新改动/验证需求时不重跑已完成完整构建。
- 核查版本先用 `git --no-optional-locks status --short`、`git rev-parse HEAD`，再对照平台 `sources.lock.json`。`make check` 会写 `out/manifests`，不是严格只读。
- 组件修改按实际依赖顺序提交并更新父 gitlink 和 lock：IOMMU → CVA6 → 顶层。不要重复应用 `provenance/` 中已纳入提交的补丁。
- 构建成功只证明构建；仿真/主机模型不等于板卡验收；板上结果才写板卡结论。日志默认只保存，有实际错误再读。
- 当前构建/恢复脚本不自动烧板、写 SD、推送 GitHub 或升级依赖；不要假定本机接了 FPGA/SD。板上操作沿用已有流程。
- 旧 bundle/恢复副本覆盖迁移阶段，不能假定包含最新实验提交；迁移新实验前重新运行 `make bundle`。这里没有个人远端，直接 `clone --recursive` 不保证取到本地提交。
- 完成实质工作后同步更新本文件的当前状态，并在 [HANDOFF.md](HANDOFF.md) 中保留阶段进展与证据；详细过程链接到专门文档和 [validation.md](iommu-platform/docs/validation.md)，不复制完整日志。
- 研究目标是争取方法论文，尚无已确认创新点；论文逻辑与优先级见 [paper-logic.md](iommu-platform/docs/paper-logic.md)。

## 最小按需阅读路线

- 新任务入口：[AGENTS.md](AGENTS.md) → 本文件。
- 平台使用/构建：[iommu-platform/README.md](iommu-platform/README.md)、[build.md](iommu-platform/docs/build.md)。
- 最新 E3 v6 结果：[结果分析](iommu-platform/docs/e3-controlled-results-review.md)；下一步：[P4M 实施卡](iommu-platform/docs/e3-next-method-plan.md)、[P4M 基线](iommu-platform/P4M_BASELINE.md)。
- 已执行 v6 协议：[E3_CONTROLLED.md](iommu-platform/E3_CONTROLLED.md)；旧 v5：[E3_QUEUE_PAIR.md](iommu-platform/E3_QUEUE_PAIR.md)。
- E3 原协议/字段：[E3_SERVICE.md](iommu-platform/E3_SERVICE.md)；D0/D0 软件诊断字段：[E3_DIAGNOSTIC.md](iommu-platform/E3_DIAGNOSTIC.md)。
- 当前代码地图：[repository-map-2026-09-18.md](iommu-platform/docs/repository-map-2026-09-18.md)。
- 结果复核：[e3-diag-results-review.md](iommu-platform/docs/e3-diag-results-review.md)、[e3-results-review.md](iommu-platform/docs/e3-results-review.md)。
- 验证历史：[validation.md](iommu-platform/docs/validation.md)；补丁来历：[patches.md](iommu-platform/docs/patches.md)。
- 各阶段工作与证据：[HANDOFF.md](HANDOFF.md)；清理前快照：[历史归档](docs/history/HANDOFF-2026-09-18-before-cleanup.md)。

## 历史任务来源

- 需要历史决策原文时定向查源任务：当前标题「平台构建」，曾称「整理论文工作文件夹」；任务 ID `01a0806d-e7d2-7092-964c-0edd0919d17e`，host ID `remote-ssh-discovered:dell`。不要默认重读完整聊天。

更新：2026-09-20。DeepSeek 起草，Codex 负责关键设计/复核与实际执行。

## 2026-09-30 组会汇报资料

已准备 2026-10-01 组会的 8 页、约 9 分钟中文 PPT/PDF、逐页讲稿、12 项问答和两张证据图，见 [资料入口](iommu-platform/out/group-meeting-20261001/README.md)。原始数字复算、8 页备注与 PDF 版面检查完成。本轮无新板测或平台编译；实验状态仍为前述 24 在线通过＋第 25 次离线认可、35 待续跑，今天板卡状态未确认。

## 2026-09-30 汇报衔接与日语说明

上次已汇报到 A/V 共跑变慢（用户确认，日期未知）。本次按 8 分钟、纯文字表格准备日语口述，拟推进到 E3 有限阻塞机制证据，并概述 P4M 吞吐取舍与下一步；尚未确认实际汇报完成。剩余计时定义、干预因果链与两次汇报边界已追加 [HANDOFF.md](HANDOFF.md)。本轮没有新增板测或编译，formal60 仍为 24 在线通过＋第 25 次离线认可、35 待运行；用户日后确认实际讲到的位置后再更新下一次衔接点。

## 2026-09-30 方法创新讨论（未实施）

Codex 与 DeepSeek 两轮讨论后，优先评估有限资源下的命中请求／在途查表解耦；固定 gap 保留为基线，反馈准入为备选。**尚无已确认新颖性**：HPCA2017 已有 hit-under-miss，需明确相对合理通用非阻塞的规则、成本或双方进度优势。先完成现有 formal60 并准备对照设计卡，差异明确后再考虑原型，不重开全部机制矩阵。本轮只查文献/关键代码与更新记录，没有改实验源码、构建或板测。详见 [paper-logic.md](iommu-platform/docs/paper-logic.md) 最新讨论及 [HANDOFF.md](HANDOFF.md)。

补充（2026-09-30，继续讨论）：先建立合理有限非阻塞对照规格，再评估同容量静态保护；研究候选为必要保护位置/容量选择，动态借用暂不立项。尚未实现或验证该候选，不能把槽耗尽写为新发现。原 E3 结论与 formal60 续35状态不变，详见论文主线最新节。

设计卡补充（2026-09-30）：已形成 N/P 筛选草案，N/P 共用处理路径和仲裁，P 仅改资源分配；结果容量分配时机仍待评估。新增计时契约覆盖接收前等待与 V 空闲期固定窗口。P 是静态对照，不是已确认新方法；尚缺逐分支正确性规格与实现。实验状态不变，见论文主线最新设计卡。
