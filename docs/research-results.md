# 研究结果与可复用资产索引

本页按问题索引历史成果，回答“已经做过什么、结论到哪里、还能复用什么”。**不安排新实验，不恢复暂停分支，不重新选择论文主线。** 当前工作与下一步只查[CURRENT_STATUS](../CURRENT_STATUS.md)，执行安排查[唯一总计划](../研究计划.md)。最近一次索引整理：2026-10-17。

每行给出结果、证据类型、停止范围与原文入口。链接先到可读报告或轮次 README，再到其冻结合同、来源 manifest、原始事件/数据和 root 接受记录；导航摘要不代替原始证据。旧报告中的“下一步”已可能被后续轮次取代。

## 如何读结论

- **板测**是指定版本和工作负载上的实际运行；历史 Mac 运行不证明当前 Dell 接板。新 FPGA 性能主张须绑定相应时钟与时序。
- **RTL/有限系统模型**只支持实际实例与环境；行为 DDR、真实 cache 端口、源语义代理和原生应用分别标注。
- **来源核查/条件构造**支持接口能力、必要条件和条件盈亏，不把未知费用置零，不当作原生收益。
- “停止”只限具名版本、动作、费用与证据覆盖范围。普通程序在同新增付费接口上复现策略，不单独证明接口已发表、所有实现等成本或整个思想族无价值。
- 问题发现不要求先有论文机制；机制增量不足与问题尚未证实分开。2026-10-17的[有界发现合同](../iommu-platform/docs/problem-discovery-contract-20261017.md)经用户继续后已实际执行，结果见[发现报告](../iommu-platform/docs/problem-discovery-results-20261017.md)；发现新事实与形成论文机制分别判断。
- M（占用计费候选）、M0（完整任务测量合同）、首 PTE 的 M-ID（具体接口候选）不是同一个对象；P 是容量分区，N 是命中旁路，PN 是组合。V/A 通常指受保护前台/后台干扰者，角色和分母须按各轮合同核对。

## 测量平台与局部因果证据

| 日期／问题 | 实际结果与证据类型 | 停止范围／可复用价值 | 原文与证据入口 |
| --- | --- | --- | --- |
| 7–8 月：多 DMA 的起点 | 7 月软件新增多设备描述、长度与 Pair/Multi ioctl，继承大传输 AW 修复；四路 RTL/HPM/Independent 等后在 9 月迁移保存。原始 Git 证据；8 月逐日日期仍有缺口 | 软件入口存在不证明当时四路硬件已上板；可复用接口沿革与版本来源 | [7 月起进度](../iommu-platform/docs/research-progress-from-july-20261014.md#七月的多dma扩展与早期证据边界)、[原始提交证据](../iommu-platform/out/research-progress-from-july-20261014/evidence.json)、[历史时间线](research-history/timeline.md) |
| 9 月 8–16 日：可追溯平台与校准 | 建立当前平台、锁定来源与交付；修 SC.W、缓存同步及 IOVA→PA 偏移等功能问题，保存 HPM 与数据核验边界 | 属于实验正确性基础，不能算干扰缓解；E0/P2、约束/CDC 等未签核项不被后续结果抹去 | [平台](../iommu-platform/README.md)、[补丁来历](../iommu-platform/docs/patches.md)、[验证记录](../iommu-platform/docs/validation.md)、[9 月历史审阅](research-history/timeline.md#2026年9月研究历史审阅) |
| 9 月 16 日 E1：共跑慢化 | 双角色软件窗口中稳定慢化；V miss 为零，A1 也无 miss，不能把全部时间变化归给翻译。实板 JSON 复核 | 得到系统现象，未确认 HOL/容量因果；保留角色交换、数据核验与计时口径 | [E1 报告及 144 条记录入口](../iommu-platform/docs/e1-results-review.md) |
| 9 月 16 日 E4：完成/供给落点 | 主样本增量约 99.08%–99.92% 落在硬件完成至 DONE 读取；40 个共跑主样本 AXI 重叠为零。被动观测与实板 | 时间落点不是纯 CPU 因果；该版本不能用来研究同时硬件争用。复用 H/P/R 与守恒核验 | [E4 报告/原始记录/独立复核](../iommu-platform/docs/e4-results-review.md)、[观测协议](../iommu-platform/E4_OBSERVER.md) |
| 9 月 16 日 batch / DS / path：形成真实压力 | 自主有限批次形成约 99.7% 在途重叠并测得硬件慢化；DS0/回切支持延迟敏感性，path 定位内部排队。实板多轮 | DS 汇合多类请求，不能把差值叫纯 PTW 成本；路径统计不能直接相加归因全部慢化。复用连续供给、分段观测与回切对照 | [batch](../iommu-platform/docs/batch-results-review.md)、[DS0](../iommu-platform/docs/ds0-batch-results-review.md)、[DS50 回切](../iommu-platform/docs/ds50-retest-results-review.md)、[path](../iommu-platform/docs/path-results-review.md) |
| 9 月 17–18 日 E3：miss 会不会拖住后来 hit？ | v4/诊断/v5 存在触发、前史、身份或截断问题；v6 独立 64B 受控配对中，四组 cold 的 A 剩余服务与 V 剩余等待同步增加，支持本实现局部 A-miss→V-hit 等待传播。实板 | v5/v6 后续已上板；旧“v5 未上板”是早期快照。此结果不解释全部自然 4KiB 慢化，不证明通用 HOL 或新方法 | [早期 E3](../iommu-platform/docs/e3-results-review.md)、[诊断](../iommu-platform/docs/e3-diag-results-review.md)、[v5 配对](../iommu-platform/docs/e3-queue-results-review.md)、[v6 受控结果/原始验收](../iommu-platform/docs/e3-controlled-results-review.md) |
| 10 月 3 日 E2：容量替换因果链 | 64 项实板记录串联 V 驻留→A 覆盖→探测前 V 条目消失→V 首次再用 miss；16 项 IOTLB 的 W7/W8 为该布局边界 | 有限布局的替换与首 miss 成立，不等于全部稳态慢化归因；复用可控状态与首再用协议 | [E2 正式报告及原始证据](../iommu-platform/out/e2-results-review-20261003/RESULTS.md) |

## 缓解基线、组合效应与完整任务

| 日期／问题 | 实际结果与证据类型 | 停止范围／可复用价值 | 原文与验收入口 |
| --- | --- | --- | --- |
| 9 月 20 日–10 月 2 日 P4M：前台改善与后台代价 | 固定 gap 给出 V 加速、A 同窗进度下降的取舍；formal60 后续补齐，跨采集段/启动。短测、pilot、完整实板分开 | 不是最佳 gap、最优调度或独立创新；在线失败与离线认可保留，复用共同窗口/carry/进度分母 | [短验收](../iommu-platform/docs/p4m-short-results-review.md)、[pilot](../iommu-platform/docs/p4m-pilot-results-review.md)、[恢复过程](../iommu-platform/docs/p4m-formal60-recovery.md)、[最终完整结果](../iommu-platform/out/p4m-complete-review-20261002/RESULTS.md) |
| 10 月 2–4 日 M/p5：占用信息能否优于强基线？ | 完整分组观测、462 项探索后，method88 两启动×两方向强对照中，固定 A8/target170 的 p5 相对保护匹配强基线均负。实板 | 关闭这一版 p5 独有收益；不是所有反馈控制不可行。保留控制器、fullstats、强基线与负结果 | [fullstats](../iommu-platform/docs/research-fullstats-v1.md)、[method88](../iommu-platform/docs/mac-method88.md)、[最终结果](../iommu-platform/out/method88-execution-20261004/RESULTS.md)、[退役代码](../platform-archives/retired-code-20261005/) |
| 10 月 4–7 日 P/N/PN：优化能否直接相加？ | P 容量分区、N 合格命中旁路。PLRU 历史隔离后，A miss 差消失，实板 PN 仍比 P 慢约 12.67%–12.79%；模型揭示数据提前放行反压 DS 的一条链 | 行为模型未完整解释实板差额；组合不必更快。DS 写归属/PTW 叶修复是功能维护，不能算方法收益 | [组合规格](../iommu-platform/docs/composition-v1-spec-20261004.md)、[历史隔离与模型边界](../iommu-platform/docs/composition-history-isolation-20261007.md)、[路由成本](../iommu-platform/docs/composition-routing-cost-20261005.md) |
| 10 月 10 日 M0/stage97：双方完整工作量 | 共同就绪，双方各 64 次自然完成并核实际字节/B/drain；行为 DDR 中普通 DS 优先消除具名 P/PN 完成差额 | 关闭“必须新增逐 copy 门控才能收回此差额”的理由；不是实板差额已消除，不是当前所有服务已最优。复用完整双方完成夹具 | [M0 结果与验收](../iommu-platform/docs/workflow-g0-m0-results-20261010.md)、[全景核查](../iommu-platform/docs/research-panorama-audit-20261011.md)、[实际 DMA 模型夹具](../iommu-platform/tests/nonblock_actual_dma_test.cpp) |

## 来源任务、应用投影与分支资产

| 日期／分支 | 实际结果与证据类型 | 停止范围／可复用价值 | 原文与验收入口 |
| --- | --- | --- | --- |
| 10 月 11–12 日行与双缓冲流 | 来源任务、释放/复用与普通控制被展开；有限行预取/固定双缓冲的主要收益被普通控制覆盖。来源核查与有限 G0 | 结束对应有限版本，不关闭应用族；复用任务合同、普通准备与回收基线 | [row 预筛](../iommu-platform/docs/workflow-row-prefilter-20261011.md)、[row G0](../iommu-platform/docs/workflow-row-g0-results-20261011.md)、[stream 合同](../iommu-platform/docs/workflow-stream-contract-20261012.md)、[后台需求](../iommu-platform/docs/workflow-stream-background-demand-20261012.md) |
| 10 月 12 日 R107：有限 KV 恢复 | 固定 vLLM/PagedAttention 来源→有限实例→普通大页/驻留实板恢复完成；恢复阶段 1728 lookup 全 hit、0 walk，准备/换出/核验/回收另计 | all-ready 有限卡归档；不证明原生 GPU attention/token 性能，也不关闭所有 AI/翻译需求。早期“待板测”已被最终结果取代 | [有限合同](../iommu-platform/docs/r107-bounded-decision-20261012.md)、[最终普通实板恢复](../iommu-platform/docs/r107-ordinary-pilot-20261012.md)、[后续需求 gate](../iommu-platform/docs/post-r107-demand-gate-20261013.md) |
| 10 月 13 日 stage117：逐包交付与真实消费者 | 经来源转向、协议适配、真实 CVA6 cache/CPU-DMA 共享 backing、metadata 与 consumer，完成 CPU/单 DMA/双 DMA ×即时/整批六案有限模型 | 普通控制有早包/末包取舍；没有证明具体可避免的共享翻译损失，不进入 G1。复用可见性、所有权、发布与消费闭环 | [来源转向](../iommu-platform/docs/stage117-plan-revision-g0-20261013.md)、[协议适配](../iommu-platform/docs/stage117-packet-protocol-adapter-20261013.md)、[真实 cache](../iommu-platform/docs/stage117-real-cache-handoff-20261013.md)、[六案实际结果/验收](../iommu-platform/docs/stage117-delivery-g0-20261013.md) |
| 10 月 15 日 A/A2：控制位置方法与真实 SVA | 四案八次完整源语义执行，公开 enabled HeroSDK 谱系定位；A2 私有驱动/ABI/程序/rootfs 实现与内核 Image 构建及分层功能检查完成 | 时间预测仍 UNKNOWN；未运行新内核或真实 DMA，未复现准确论文实验版本。原 buffer 可直接交付的任务不能冒充新增能力 | [A 方法](../iommu-platform/docs/control-position-method-20261015.md)、[SVA 来源](../iommu-platform/docs/a2-native-sva-binding-20261015.md)、[A2 实现与边界](../iommu-platform/docs/a2-sva-implementation-20261015.md) |
| 10 月 15 日 A3–A5：真实消费者与等待范围 | AXPY 真实 L1 消费者、按 transaction ID 等待与最后 B 完成链；普通 O-T1/双 ID guard 可表达动作。A5 两 episode 依赖核对与同信息标准 DAG 一致 | 是源级完成域/等待证书；变体未应用/编译，不是周期性能或已测共享翻译竞争。保留 O-G 复用 guard 与 xbar/接收能力问题 | [A3](../iommu-platform/docs/a3-consumer-position-rules-20261015.md)、[A4](../iommu-platform/docs/a4-axpy-control-decision-20261015.md)、[A5](../iommu-platform/docs/a5-wait-scope-contract-20261015.md)、[稳定接受](../iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json) |
| 10 月 15 日 D/CS1：决策价值与费用 | 完成→补发闭环的合成模型与独立事件回放；免费 Z 信息在主配置无价值，付费 Z 在八配置均不及最佳固定动作；等时间普通等待解释特定反号 | 停止本版 Z/费用/菜单选策实现与扩参，不关闭 D。非原生 SPDK/RTL 周期；首次生成缺完整源 hash，事件回放不等于完整生成版本复现认证 | [CS1 轮次/证据/接受](../iommu-platform/out/d-service-model-20261015/README.md)、[模型结果](../iommu-platform/out/d-service-model-20261015/model/README.md)、[root 因果复核](../iommu-platform/out/d-service-model-20261015/root/causal-check.json) |
| 10 月 15 日 I：失效与回收干扰 | 驱动未设置 PSCID/范围循环与硬件支持须区分；已有 IDYLL/ATC2015/rIOMMU 限制宽泛主张。来源预筛 | 尚未有匹配正常 I/O 与 live 任务的联合原始轨迹；范围修复先作为普通基线，未开发新控制 | [独立方向报告 I](../iommu-platform/docs/independent-research-directions-20261015.md)、[I 原始来源摘录](../iommu-platform/out/research-plan-implementation-20261015/i/source-evidence.md)、[生命周期审查](../iommu-platform/out/independent-directions-20261015/lifecycle/assessment.md) |
| 10 月 15–16 日 B1：早翻译与单条带成本 | HERO 合法准备窗口和 debug 纯翻译入口已核；条带/容量与存储、端口、生命周期卡形成。来源与成本条件分析 | 缺具体残余与完整成本见证；one-shot/ATC/O-PAQ、debug 预热仍是强普通对照。缺 adopt 接口不证明新意，也未证明普通预热足够 | [方向审查 B1](../iommu-platform/out/independent-directions-20261015/ownership/assessment.md)、[单条带来源与成本/接受](../iommu-platform/out/b1-stripe-cost-20261016/README.md) |

## 机制探索与有边界的负结果

这组材料用于避免重做已解释的正例，同时保留新问题。逐行“普通覆盖”是其原文给出的覆盖，不统一升级成已发表方法或整个架构的否决。

| 日期／主题 | 实际结果与证据类型 | 停止范围／可复用价值 | 原文入口 |
| --- | --- | --- | --- |
| 10 月 6–9 日共享结果与资源组织 | 六族/四方法比较及后续分片、共享源读、在途加入、部分 walk 等；部分 RTL 功能完成，具体性能例被普通准备/起点共享追平，部分来源仍缺 | 容量＋并发保护、页表优先等有明确前史；不能靠组合立新。保留动态依赖、共享结果与成本问题，不整体否决 | [六族候选](../iommu-platform/docs/innovation-council18-20261006.md)、[四方法比较](../iommu-platform/docs/translation-comparison-20261006.md)、[QoS](../iommu-platform/docs/translation-sharing-qos-20261008.md)、[共享读 RTL](../iommu-platform/docs/shared-read-rtl-progress-20261009.md)、[独立调度](../iommu-platform/docs/shared-read-independent-scheduling-20261009.md)、[路线审查](../iommu-platform/docs/research-route-audit-20261009.md) |
| 10 月 9 日共享长读/各自暂停恢复 | 314 个受限事件场景与后续真实 iDMA/IOMMU RTL 的 12 个完整 copy 验收；暂停/重读、字节/B 与失败版本均保留 | 有限可行性和相对具名 q2 控制的周期/AR 差，非同总成本最优或独立贡献；单页静态成功、一次暂停，无板测。生产 FIFO 与小缓冲反例不可混同 | [事件模型](../iommu-platform/docs/shared-read-independent-scheduling-20261009.md)、[RTL 结果与完整接受](../iommu-platform/docs/shared-read-rtl-progress-20261009.md) |
| 10 月 15 日 Q/G 方向比较 | Q 联合容量/在途/共享服务，G 不规则 gather、消费者分组、访问—更新布局及多 GPU 共享，均保留具体发展空间；是来源与方向判断 | Q 不能以 P+N+限流组合立新；G 缺具名任务冲突且近邻正文有未核项。不是都做过性能实验，也不是整族否决 | [六方向定义、Q 前史与 G 保留条件](../iommu-platform/docs/independent-research-directions-20261015.md) |
| 10 月 8–9 日容量/owner/前移控制边界 | 固定原窗口、parent 容量、准备源 bypass、写顺序、fragment 排空及共享入口的逐项比较 | 具体普通提前准备、共享槽及在途额度不能人为删除；最大安全长度不等于最快长度。复用合法控制点与费用清单 | [容量](../iommu-platform/docs/parent-capacity-boundary-20261008.md)、[多 parent](../iommu-platform/docs/multi-parent-preparation-boundary-20261008.md)、[准备 bypass](../iommu-platform/docs/prepared-source-bypass-boundary-20261008.md)、[原窗口](../iommu-platform/docs/original-window-progress-boundary-20261008.md)、[分片排空](../iommu-platform/docs/fragment-drain-cost-boundary-20261008.md)、[权限/释放](../iommu-platform/docs/research-permission-release-20261009.md) |
| 10 月 13–14 日 R2 | O-B5S 以相同状态/端口/调度实现原位头复用及原序 WRITE 许可准备。源码生命周期与逻辑成本配对 | 关闭该版独有动作和逻辑成本优势，不进性能模型；不是发表覆盖证明。复用成功/fault、W/B 与退休责任 | [R2 完整成本裁决/接受](../iommu-platform/docs/r2-ownership-cost-20261014.md) |
| 10 月 14 日 CO1 | OC1-min 复现阶段引用保护、释放链排序、最后用途解除与缓存让路。具名普通程序与费用比较 | 关闭本版准入动作和必要逻辑成本主张；不否定协作族。复用用途与完成义务分别退休 | [CO1](../iommu-platform/docs/completion-obligation-design-20261014.md) |
| 10 月 14 日 RCS1 | 冻结四条件中固定 SJF 整任务保护始终等于或优于候选完整完成。有限模型；32 案的 64B permit 属额外 engine 能力 | 关闭该版完整性能；保全更多完整集合不自动更快。后续勘误禁止把 64B 条件称生产 fullbuffer 原生能力 | [RCS1 与顶部勘误](../iommu-platform/docs/rcs1-completion-set-20261014.md) |
| 10 月 14 日 BR1 | 固定图可精确普通化简；形成新冷功能图与费用账，未跑四条件 | 结束 BR1 本轮性能入口，动态共享服务族未决；功能图不当收益证据 | [BR1 与 fullbuffer 源码纠正](../iommu-platform/docs/blocking-replacement-20261014.md) |
| 10 月 14 日 F1/包络/返回封口/AW | 核共同可选点、真实 RREADY→buffer→W、首次 VALID 与 owner 寿命；旧两 score 不能在线代表完整写就绪，源 AW 握手不等于 TC input 接受 | 当前具名入口未准入性能；future-AW/F1/完整准入族保留。上游 spill 可前瞻，旧第二 AR 前驱推断撤回 | [共同来源选择](../iommu-platform/docs/joint-source-choice-20261014.md)、[R/PTE 封口](../iommu-platform/docs/rpte-return-commitment-decision-20261014.md)、[包络可见性/费用](../iommu-platform/docs/envelope-observable-cost-20261014.md)、[AW 最新因果裁决](../iommu-platform/docs/aw-barrier-causal-20261014.md) |
| 10 月 14 日 DDR 接受顺序 | 生成 MIG4.2 参考绑定；承诺前 DS-first 可改 offer，已接受读不能被后来的 PTE 越过，held R 必须保持。源码/接口条件证据 | 私有模型允许的重排不能当实际 DDR 能力；没有新收益运行或实际债务测量。复用首 PTE 窗口与承诺边界 | [DDR 服务边界与稳定裁决](../iommu-platform/docs/ddr-service-boundary-20261014.md) |
| 10 月 14–15 日 SDH1 | 私有非时序协议原型与 32768 选择配对；共同 direct-OUT 付费架构上选择与 O-NBO 一致，构造成本配对 | 关闭这版策略，不证明原生 N 已有同 bank 全路由，不否定付费接口整合价值；无 cache/PTW/DDR/native 性能 | [SDH1 原型/对照/接受](../iommu-platform/out/source-drain-hit-20261014/README.md)、[后续范围勘误](../iommu-platform/docs/shared-service-control-boundary-20261015.md) |
| 10 月 15 日 LR1/共享服务盲窗 | LR1 私有功能原型交付，普通分阶段/排空程序复现事件链；普通 DS-first 只可改变未承诺选择，无法撤销 MIG 已接受载荷 | 关闭选定规则/成本版本；真实债务和新意接口净价值未知。LR1 顶部勘误优先于旧端点摘录 | [LR1 与勘误](../iommu-platform/docs/local-read-reclaim-20261015.md)、[服务边界](../iommu-platform/docs/shared-service-control-boundary-20261015.md)、[机制准入](../iommu-platform/docs/shared-service-mechanism-admission-20261015.md) |

## 映射组织与首 PTE 条件构造

以下均为 2026-10-16 的有界来源/设计轮次，除明确标出的条件模型外，未实现生产机制、未取得原生净收益。每行链接的轮次首页汇总该轮 source、root 接受与执行账；“后继”按当前状态判别，不自动执行旧 gate。

| 轮次／问题 | 已取得的结果 | 停止范围与可复用价值 | 原文入口 |
| --- | --- | --- | --- |
| DMA_PMD＋XDP 来源组织 | 固定 mlx5e/XDP 生命周期、整页转交与授权条件、近邻比较 | 旧整页协议是历史版本；不从一包一页推同页混包，不从缺接口认新意 | [organization-source](../iommu-platform/out/mapping-organization-source-20261016/README.md) |
| 映射接纳累计费用 | 官方 v6.12/DAMN 与有限费用账闭合；累计超额阈值就是普通 O3 | 停止该版独立控制机制主张；CPU 小计不是完整净赢，不关闭整体映射组织 | [admission](../iommu-platform/out/mapping-admission-20261016/README.md) |
| V1 生命周期与整合差项 | 分离源 owner、B 可撤关联和一次 TX 使用；V1-N/K 复用 frame/片段寿命，固定 B/config、K sparse 关联、满额走 O0 | 普通 R/slot/O3 同享能力；R 是具名普通构造，非已发表同名网络系统。stock 无 idle B pin，不天然节省 payload RAM | [lifetime-design](../iommu-platform/out/mapping-lifetime-design-20261016/README.md)、[integration-delta](../iommu-platform/out/mapping-integration-delta-20261016/README.md) |
| 正常再用与输入资格 | 建立跨 recycle 再用、L0/完整 CPU 配对条件；有界验收公开数据前缀和小文本，但匹配输入为零 | 输入不匹配不等于再用/收益为零；不自动继续相同搜索、采集器或系统开发 | [reuse-opportunity](../iommu-platform/out/mapping-reuse-opportunity-20261016/README.md)、[evidence-input](../iommu-platform/out/mapping-evidence-input-20261016/README.md) |
| owner 辅助冷 B 关联回收 | private alloc.cache-only 扩展、pending/kick/ACK/退休与关闭的条件发布协议 | 无 owner 服务/返槽时限保证，未实现；回收 B 关联库存不等 payload RAM 下降。近邻已有 idle 隔离原理，同接口策略不独占 | [recycle-reclaim](../iommu-platform/out/mapping-recycle-reclaim-20261016/README.md)、[owner-publish](../iommu-platform/out/mapping-owner-publish-20261016/README.md) |
| 关联表示、同步与 IOVA | V-C 固定值 sparse 表与 RCU/锁/密表比较；每轮合法 CPU/A/B sync、整页 map 对齐与实际 IOVA 预算合同 | seqcount 不是新算法；K 不保证普通地址供给、总 RAM 差或完成时限。普通整页 fresh map 也获对齐分支收益，native 费用未知 | [owner-representation](../iommu-platform/out/mapping-owner-representation-20261016/README.md)、[dma-visibility](../iommu-platform/out/mapping-dma-visibility-20261016/README.md)、[iova-budget](../iommu-platform/out/mapping-iova-budget-20261016/README.md) |
| 从 M0 负结果构造首 PTE 票据 | 实际 PTW start→payload 首次 AR 承诺间的付费早意图；保持原 burst/返回。一次冻结条件模型保留完整两 COPY/B/drain，含正例与相邻失败条件 | 同付费接口 if/else 逐事件等价，零暴露费用普通分段可更快；值得比较的是完整接口实现成本，未证独占调度或原生收益 | [construction 总入口](../iommu-platform/out/mechanism-construction-20261016/README.md)、[设计](../iommu-platform/out/mechanism-construction-20261016/design/construction.md)、[模型结果/限制](../iommu-platform/out/mechanism-construction-20261016/model/README.md)、[root 接受](../iommu-platform/out/mechanism-construction-20261016/root/root-result-acceptance.json) |
| 首PTE接口与legalizer成对设计 | M-ID f1的时序/身份/资格与O-local/O-prepared的真实接线落定；强O复用TC保持，1个完成sticky，无新header FIFO | 结束票据天然少状态/便宜的理由；原2060/2058只属旧模型。当时保留局部实现提案；后继实际结果见下行 | [六段报告](../iommu-platform/docs/first-pte-interface-cost-20261016.md)、[接受](../iommu-platform/out/first-pte-interface-cost-20261016/root/root-result-acceptance.json)、[唯一后继](../iommu-platform/out/first-pte-interface-cost-20261016/root/next-minimal-contract.json) |
| 首PTE局部RTL/成本判别 | M与普通不分段base完整quiet4481/1749且已记录事务逐行相同；强O-prepared实际4519/1794并验证提前翻译/TC保持 | 结束固定正常COPY/PN/cold-hot收益版本：合格V写walk时A数据已回完，M实际mask0。保留实现，未扩O-local/job性能或相位搜索，不外推整个接口族 | [六段结果](../iommu-platform/docs/first-pte-paired-local-20261016.md)、[原始分析](../iommu-platform/out/first-pte-paired-local-20261016/root/paired-analysis.json)、[接受](../iommu-platform/out/first-pte-paired-local-20261016/root/root-result-acceptance.json) |
| 重新立题RECONVERGENCE-1 | 原始失败机制复核；TC借用续行、译后分段/返回预约、范围旧代回收三个具体设计与最近原文比较 | 前两版无具体残余能力/成本差项；第三缺匹配正常残余。均不立项，不强选主线或排等价实验；不是新性能结果或全方向否决 | [七段报告](../iommu-platform/docs/research-reconvergence-20261016.md)、[原始核查](../iommu-platform/out/research-reconvergence-20261016/failure-evidence.md)、[接受](../iommu-platform/out/research-reconvergence-20261016/root-result-acceptance.json) |
| 10月17日DISCOVERY-REUSE-1 | 真实DMA/IOMMU局部RTL＋行为DDR：64parents/路，目的R1/8/16×P/PN及两普通2MiB诊断；8条件/1024parents核验通过。R8足迹9仍39walk；PN与P同walk但完整时间更慢；后续PTE的已承诺payload前驱已定位 | 源码推导masked tree-PLRU热源/子树偏置，未直接采victim；本专用arena普通2MiB使双方完整75797步/3初始walk。归发现与普通优化依据，未立新机制、不恢复M或MIG，不外推4KiB受限部署 | [中文结果](../iommu-platform/docs/problem-discovery-results-20261017.md)、[原始输出/归约](../iommu-platform/out/problem-discovery-run-20261017/README.md)、[接受](../iommu-platform/out/problem-discovery-run-20261017/acceptance.json) |

资源归因始终分开源池本来持有、B 关联实际延后、复制目标新增及退休占用，按物理并集去重；策略改变分配/回收时各自推导再用序列。CPU、物理内存、PTE/IOVA、带宽与有效完成分别报告。相同工作/服务下 CPU 或不可回收内存的改善可以是主要价值，无需强造时间独占收益。

## 独立工程贡献与硬件边界

这些是独立资产，不能自动改变研究分支裁决。PR 与远端的最新动态只查[当前状态](../CURRENT_STATUS.md)，这里保留完成事实与证据入口。

| 日期／事项 | 已完成与边界 | 入口 |
| --- | --- | --- |
| 10 月 12 日 Linux IOVA/HPM | 单提交单文件补丁与主机回归已完成并推送个人分支；目标为具名 RISC-V IOMMU 分支，非 Linux 主线缺陷声明。创建 PR 曾遇集成 403，不能说已有 PR | [submission/补丁/回归](../iommu-platform/out/upstream-linux-fixes-20261012/submission.md) |
| 10 月 12 日 IOMMU DS/PTW | DS 写数据归属与合法叶修复独立交付；用户创建上游 PR51/52。其他 PR 的只读适用性裁决不等实施 | [平台 PR 接受记录](../iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)、[补丁历史](../iommu-platform/docs/patches.md) |
| 10 月 12 日 AXI boundary checker | 页尾误拒、INCR 半长度漏检及 WRAP 位宽修复；模块回归通过。CVA6/平台锁指针未因此自动更新，无对应新板测 | [独立 RTL 接受](../iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json) |
| 10 月 15 日 MIG 债务观测 | 第六次构建生成 BIT/MCS 但时序失败，WNS −0.391ns、598 setup 违例；未加载、未采集，实际债务 unknown | [MIG 报告](../iommu-platform/docs/mig-pte-debt-observation-20261015.md)、[暂停回执](../iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json)。本轮整理不恢复此工作、不重启原日期窗口 |
| 10 月 16 日 GitHub 汇总同步 | 顶层公开仓库与无损大文件分片/清单工具建立，保留内部组件历史；同步不改变研究授权 | [根 README 同步说明](../README.md)、[工具](../tools/github_sync.py)、[持续同步规则](../AGENTS.md) |

## 找更细的历史

- [历史审阅 README](research-history/README.md)：历史核读范围、结果纠正与 coverage；其中 dated 状态只代表当时快照。
- [7–10 月时间线](research-history/timeline.md)：按原始提交/正文实验日期定位早期任务、失败与转向，含原始目录和验收表。
- [7 月起进度回顾](../iommu-platform/docs/research-progress-from-july-20261014.md)：简洁因果脉络；其中旧 gate/旧方法入口不替代当前政策。
- [HANDOFF](../HANDOFF.md)：用本页的日期、分支名或 stage 号定位原段，不要求全文阅读；[validation](../iommu-platform/docs/validation.md)用于工程验证交叉核对。
- [历史计划](../iommu-platform/docs/history/plans/README.md)与[文献库](../references/README.md)：分别定位当时合同和前作原文/阅读记录。

维护时新增或修订对应主题行，链接新报告/接受记录，并写清旧结论被哪项新事实修正；不把每轮完整正文追加到本页，不改写历史原始产物。
