# §80 · DARTS 与 C 消费者闭包：关键前作裁决

2026-10-09，Codex C2 亲自源核与语义推导。范围限于 C 合同、DARTS 作者实现及 HFP 原作模型；没有模型执行、源码实现、构建或板测。根协调最终接受本裁决。

**最强结论：取得的 2025 年作者实现并未原样覆盖 C 的“不同 payload 共用翻译 setup、独立翻译容量与输入容量、按最终消费者完成形成多 key 闭包”的完整规则。它已经覆盖以传输解锁计算的比值选择，以及传输完成、消费、受引用保护的释放这一运行时骨架。** 因此不能把密度公式、many-task unlock 或完成通知单独认作创新；也不能仅把 translation metadata 命名成 data object 就否决 C。下面给出实际选择规则与具体失配，而不是用全文缺失替代判断。

对这项前作而言，可以保留的正向增量是：**以独立的翻译驻留/已兑现映射状态和不可提前归还的输入占用为约束，选择能完成最终消费者的多 key 生产计划。** 如果同信息、同成本的忠实 DARTS 适配已经作出同样选择和完成曲线，这部分独有收益不成立；如果它在明确状态下选择不同，而 C 的新增计划能力产生计入实现成本的收益，就能据此论证相对 DARTS 的机制增量。两者都不等于已经完成整个领域的首创审查。

## 1. 实际取得与阅读范围

| 来源 | 本轮证据与限制 |
|---|---|
| [JPDC2025 出版商页](https://www.sciencedirect.com/science/article/pii/S0743731525001376)，DOI 10.1016/j.jpdc.2025.105170 | 搜索返回摘要/章节片段；直接全文访问 403。未读期刊最终正文/全部算法。 |
| [HAL v2](https://hal.science/hal-04146714v2/document)、[SSRN](https://papers.ssrn.com/sol3/Delivery.cfm/e5eaf285-1d5a-4016-af32-5d70b9d4ec80-MECA.pdf?abstractid=5177575&mirid=1) | HAL 下载为 HTTP 200 的 HTML 验证页，不是 PDF；SSRN 403。保存失败/类型记录，不声称读过。 |
| [作者主页](https://maximegonthier.github.io/)与[官方镜像](https://github.com/starpu-runtime/starpu) | 作者明确介绍 JPDC2025 对应 DARTS 已在 StarPU 主分支；镜像自述是 Inria 开发仓库的官方镜像。由此进入实际实现，而非第三方重写。 |
| [2025 固定版 darts.c](https://github.com/starpu-runtime/starpu/blob/ed79140c3bc7a1b0ac85761627f8893896d2eec3/src/sched_policies/darts.c) | 固定 commit `ed79140c3bc7a1b0ac85761627f8893896d2eec3`，2025-06-03；这是截至 2025-08-31 该文件最近一次提交，**不是已核实的论文 artifact tag**。直接读主选择、planned/pulled 状态、驱逐和完成 hook；同提交的 perfmodel/memalloc/coherency 直接调用也已核。未运行实现。 |
| [FGCS2023 出版商页](https://www.sciencedirect.com/science/article/pii/S0167739X23000328)，DOI 10.1016/j.future.2023.01.024 | 已读摘要/模型与算法节片段，正式全文未取得。其算法节列的是 HFP，不能把它与 2025 DARTS 当同名同版算法。 |
| [作者 2021 HFP 原稿](https://easychair.org/publications/preprint/6rPl/open) | 成功取得 13 页 PDF，直接读 §2 模型、§3 Algorithm 1/合包及 §4 实现设置。它是 2023 工作的早期同作者近邻，**不能替代 2023 最终全文**。 |

下载和 SHA 在 [darts-primary/code-provenance.json](/home/xvhaocheng/work/iommu-platform/out/parallel-innovation-model-20261009/darts-primary/code-provenance.json)、`dependency-provenance.json`、`hfp-provenance.json`；实际阅读区间在 [read-scope.json](/home/xvhaocheng/work/iommu-platform/out/parallel-innovation-model-20261009/darts-primary/read-scope.json)。另下载了 2026 当前版，未用它反推 2025 已有能力。以下代码行号均指保存的 2025 固定版。

## 2. 源码实际选择什么

**单位是 data handle，不是 VPN/translation key。** `darts.c:2031–2062` 遍历引用候选 handle 的任务，对该任务其他 handle 查询 `starpu_data_is_on_node`；候选以外一个也不缺才计入本次可解锁集合。默认 `SIMULATE_MEMORY=0`；可选值 1 还把 planned/pulled 中的 handle 视作可获得，不能把该可选预备能力故意移除。

设候选对象为 d，F(d) 为“加入 d 后其余输入已可用”的任务。默认 `DOPT_SELECTION_ORDER=7`，主分数实际是

`expected_transfer_time(d) / Σ[T∈F(d)] expected_compute_time(T)`。

F 为空时取无穷；随后才依可解锁任务数、priority、one-from-free 数与剩余工作等破同分。直接证据：`darts.c:1729–1761, 2016–2105, 3256–3268`。因此这不是简单的热门对象引用计数，也不只是最短任务优先。它重视**解锁的计算时长**，与 C 当前“最终 ready 组数／包含 ACK 停顿的生产时间”不是同一目标。

若能解锁，实际把有关任务加入 planned list（`2303–2373`）。如果选中的对象之外还缺一项，代码记录 `one_from_free`，最后挑其中一个最高优先级任务补齐（`2089–2098, 2384–2442`），**未在此路径枚举“同一双 key 集合的多个消费者 + ACK 容量时间线”**。第一任务还有启动特例（`1887–1958`），完整对照须保留或从共同启动后状态比较，不能把起点偶然性冒充核心差异。

**费用已核到直接依赖。** [perfmodel.c](/home/xvhaocheng/work/iommu-platform/out/parallel-innovation-model-20261009/darts-primary/perfmodel.c:434)先检查该 handle 在目标 memory node 是否可用，再按 handle size 和传输路径预测；零 size 明确返回零，包括抽象 void handle。DARTS 调它时使用 `STARPU_R`。这条实际调用没有接收共享 translation key、IOTLB 驻留或映射兑现槽参数；总体测得的传输时间当然可能含翻译开销，但**把同一份平均开销隐含在每个不同 handle 的预测中，不等于显式只为它们的共享冷 key 收一次 setup**。

**消费生命周期已有实质支持。** [coherency.c:1147–1154](/home/xvhaocheng/work/iommu-platform/out/parallel-innovation-model-20261009/darts-primary/coherency.c:1147)规定先完成输入 fetch，再执行任务；执行后输出/释放路径见 `1375–1433`。非 detached fetch 增加引用，release 才减少（`876–887, 992–1003`）；[memalloc.c:243–271,536–568](/home/xvhaocheng/work/iommu-platform/out/parallel-innovation-model-20261009/darts-primary/memalloc.c:243)禁止驱逐正在被引用的数据；DARTS 的 `post_exec_hook` 还更新 pulled 计数（`3430–3469,3491`）。这反驳“DARTS 一看到数据 ready 就无条件释放”的弱对照。

但“允许安全驱逐/再取回”与 C 的“copy dispatch 起预留，直到 consumer ACK 才允许归还这一输入槽”也不能混同。DARTS 对已取入但尚未被活动消费引用的数据可按策略驱逐；C 当前合同不允许以这类驱逐借出 ACK 前的空间。准确适配必须保留 C 的槽租期，或明确双方都改成允许丢弃/重取并计其成本。

## 3. 尝试完整映射：能映射的部分与明确失配

| C 事件/资源 | 最强自然 DARTS/StarPU 对应 | 等价所需条件、缺口 |
|---|---|---|
| sealed 消费者及独有 payload | consumer task 与各自 input handle | 地址/依赖都显式登记可表达；不能把不同 payload 合成一个可复用字节对象。 |
| READ+WRITE/B 后 copyready | 输入传输完成，或一个显式 producer task 完成 | 抽象边界能对应；需实际 driver 完成包括目标写入及所需可见性。这里未核某 AXI driver，不能把 source R 或任意 B 自动当全局可见。 |
| consumer ACK | consumer task 真正完成/释放输入引用 | 若 ACK 就是 runtime 所见完成，可对应；若另有异步外部 ACK，需桥接并计通知和持有成本。不是“ACK”名称不同就新颖。 |
| 独立输入池 B、dispatch 预留、ACK 才归还 | 分配/引用/释放机制 | 基础能力已有；精确租期和所有不可撤在途预留仍需相同适配。不能免费移除 C 的中间持有，也不能免费禁止 DARTS 的原有 overlap。 |
| 不同片段同 translation key 一次 setup | 人工共享 metadata handle M_k | 只有正确反映 context/epoch/访问权限、驻留/兑现与失效、实付 setup 后才可能等价；现有 data node residency 不是硬件 IOTLB 状态。 |
| key 可在 copy 后淘汰，payload 仍持有到 ACK | metadata 与 payload 两类独立寿命/容量 | 若都当 consumer input，会把 metadata 错绑到消费结束；若拆 producer/consumer，寿命可表达，但调度单位和本次解锁的任务就变了。 |
| 多 key、多个最终消费者的闭包 | DARTS 单 handle 解锁，或 HFP 输入交集合包 | 现行 DARTS 主分数没有这份计划的最终消费者数、所有 copy/WRITE、两种容量峰值和未来 ACK 停顿。把它们写入新宏对象的 cost callback 是新增适配/规则，不能当已发表证据。 |

### “metadata 也算 data”为何不足以完成等价证明

1. **零大小 token 失去费用。** 原函数把 size=0 的 handle 费用设为 0，不能代表 L>0 的 PTW。给它非零大小/人工传输时间可以修正一部分，却要说明它占哪类容量，实际何时失效，以及预测如何随当前驻留变化。
2. **单池不能随意替换两类独立额度。** 例如 Q=1 个 key、B=4 个 payload。状态 `(q,p)=(1,4)` 合法，`(2,0)` 与 `(0,5)` 非法。若仅靠每类固定非负对象大小 a、b 与一个总容量 M 精确表示，必须同时有 `a+4b≤M<2a` 和 `M<5b`，推出 `a>4b` 且 `a<b`，矛盾。这只是反驳这个具体单池映射，**不是新的调度定理，也不是说 StarPU 框架不能增加另一资源模型**。
3. **把 M_Y/M_Z 都挂在最终 consumer 上会延长其寿命。** C 可以译 Y、复制 y，释放/替换 Y 的翻译，再译 Z、复制 z；consumer 使用的是已完成 payload，不需要 Y/Z 翻译同时驻留。Q=1 时，上述自然 handle 映射却要求两 metadata 在 consumer 执行时均可用，改变了可行计划。
4. **拆成 copy tasks 能修寿命，却不自动修目标。** 让每个 copy task 只读 M_k、产出独有 payload，再由真正 consumer 消费，可以表达两阶段；但 DARTS 此时主要为“可立即运行的 copy tasks”评分，最终 consumer 常仍受依赖阻挡。现有 descendants priority 是可选破同分信息，不等于计算某组 copy 是否封闭多个 AND 依赖、其 B4 峰值和 ACK 停顿。

因此本次没有证明“任意 runtime 抽象都不能表达 C”；证明的是：**目前三种直接映射至少各丢失一项费用、可行性或选择语义，不能据此声称同信息/总成本完全等价。** 一个忠实适配仍可做，而且应给前作使用；但应把实际新增的 bridge 与多 key 决策分别收费、分别归属，不能把我们写入的全部闭包规则倒算为前作已有。

## 4. 一条代码级不同决定，以及 HFP 的确切约束

**从共同启动后的普通调度边界比较**：令无 planned/pulled 干扰，X 只有独有输入 x；G1–G3 分别有不同的 yi/zi，均未驻留，任务时长均为 1，priority 相同，所有传输成本有限且为正。原样以 payload handle 表达时：候选 x 的 F={X}，分数有限；任一 yi/zi 的 F 为空，分数无穷。因此默认 DARTS 选 x，随后安排 X。它并不因为 yi 共享 translation key Y，就把它们当同一个 e->D；这是 `2031–2064` 的实际 handle 相等/驻留判定。

C 的 L=10、d=1、B4、真实 ACK 合同下，已有纸面规则选择 {Y,Z} 三消费者闭包。这给出**原 DARTS 选择与 C 选择的具体差别**，但还不是性能赢前作的证明：原样 DARTS 未被赋予共享 key 适配，且四组例已被普通 hot-page 追平。根的小模型须继续比较忠实适配，不应仅把这条直接映射当最终强基线。

2021 HFP 原稿 §2 明确：等大输入、一个 M 池、任务执行时所有输入在内存，目标是总 loads；§3 Algorithm 1 按**实际输入集合交集**选择合包，先要求 union 容量可放入，再做 meta-package/翻转。由这些明确规则可知，它已有容量内多任务合包，不能说“多任务闭包”空白；但把不同 payload 共用的翻译标记算作共享输入，会遇到上面的费用/双容量/寿命问题。2023 最终版的所有修改与具体 HFP 实现未核完，不能宣判其所有变体都不覆盖。

## 5. 下一项精确决定门

**不用再全面收论文，也不先做 RTL。下一门是一个有明确语义的 DARTS 适配对照，接入根正在做的完整小模型。**

先冻结共同执行层：每个 payload 独立付 READ/目标 WRITE/完成成本；翻译 key 含完整 context/epoch/权限类别；Q/IOTLB、prepare 结果槽和 B 输入池分别记账；双方相同 setup/data overlap、目标翻译、dispatch 预留与 consumer ACK 归还。硬件驻留/兑现反馈若需要新接口，两边得到同信息且计其开销；没有真实反馈就使用共同的明确估计，不能只让候选知道未来命中或 ACK。

在该共同层中保留两种**明示为适配、未经原实现执行**的最近邻：

- 保留 DARTS 的单候选对象/立即解锁计算分数及实际一项缺失分支，映射 copy/consumer 时记录究竟把谁算进 F，不能偷偷把最终消费者闭包分数塞给它再称“原 DARTS”。必须有安全的共同容量准入；不拿必死锁的四个组头策略作对手。
- 对 HFP/固定 tile 给合法的重排、同页 batching 与相同 prepare 能力，比较多个合理合法切法；没有实际算法映射的部分标为未验证，不杜撰 HFP 数字。代码取得不必阻挡根先完成其他已定义强对照，但阻挡“已赢全部 DARTS/HFP”的结论。

决定时完整执行八组，不只截 t28；报告每组 ready/ACK、平均或加权完成、makespan、setup 次数、payload/WRITE 工作、Q/prepare/B 峰值及决策/通知成本。**若新增多 key 计划在这一相同执行层选择不同并取得净目标收益，可把独立机制主张收窄到“面向最终消费者的、双资源寿命约束下的有限闭包选择”；若只靠增加翻译可见性或移除前作预备能力获胜，先修公平合同；若忠实前作适配/既定强简单对照追平，停止该条件下的独有收益主张。** 不要求击败所有可以想象的新程序，也不把“普通可再加上整个提案”当已存在的对照。

机制新颖性仍须最终全文/实现版本边界核实；独有收益由上述同合同结果决定；论文主贡献还要实际可提前完成的消费者场景、合理映射/暖缓存后 setup 的重要性，以及可承担的部署成本。三者分别判定。

## 6. 根模型所用 DARTS 派生代理：最低规则与收费

根提出的 `single-key marginal-unlock + one_from_free 最高 priority 组` 可以是有用强对照，但应命名 **DARTS 派生的 key 代理**，不是“原 DARTS 已复现”。把单个 payload handle 变成多个 payload 共用的 key，本身就是明确的抽象适配。

1. **候选及 F 定义。** 每个候选 k 只尝试补当前这个 key 下的必要 payload；`F(k)` 是完成这些独立 copy 后，所有其余输入也已完整到达的最终消费者。输入还没 copy 时，不能因 key 热就认作 ready。已有 ready 消费者不得重复算收益。若采用 copy-task 映射，则 F 计的是 copy tasks，必须另列；不能在同一代理中择利切换这两个口径。
2. **分数。** 原规则是 `transfer / Σ expected_compute`，不是 `transfer / count`。根四/八组模型所有 consumer 服务均为 1，二者在此相等；推广异构服务时必须恢复实际时长。key 代理应说明 numerator 的适配：可采用“把 F(k) 所需不同 payload 全部送达的边际生产成本”，包括一次必要 setup、逐 payload READ/目标 WRITE/B、容量等候与通知；这比原 handle 费用更贴 C，是给予前作的明确强化，不倒算成作者原算法。若另测 setup-only 分数，也必须把实际缺失 copy 成本交给共同执行层，且不能只报告这个较弱变体。
3. **两缺项分支。** 没有可立即解锁组时，保留 one-from-free 候选统计及原 priority/同分顺序；选中后只补其中**一个最高 priority 组**的完整缺项，不能直接完成具有相同双 key 的所有组。后一动作应放到单独的 `two-key max-closure` 强简单对照里。双方 priority 来源一致；C 用 age 就明确给代理相同 age，但标明这是一种适配，不凭空说是原程序默认 age。
4. **有限容量与 overlap。** 同一安全准入层扣除所有已发未退的 payload 预留，只有本边界实际收到 ACK 才归还；准入层不能向代理隐藏可用空间，也不允许其用将来 ACK 提前 dispatch。对 ready、in-flight、prepared-but-not-copied、translation-resident、materialized mapping 五种状态分别记账。prepare 槽保留映射后，IOTLB 淘汰不能无条件再收 setup；prepare 槽用完也不能免费保留无限映射。WRITE 翻译若冷，按双方相同实际资源计费。
5. **重排与公平。** 代理允许与 C 相同的 source setup/data overlap、目标准备、合法单 key batching、消费者并发；若一次只安排一组后允许 opportunistic 同 key 搬运，记录准确范围及租期，不能拿一种不利 FIFO 当唯一 DARTS 对照。C 的计划扫描、key 比较、group/ACK 状态、软件提交/缓存维护和代理自身的扫描都纳入同一成本口径。
6. **输出归属。** 原实现已证单 handle 分数/分支、派生代理的 key 适配、cheap 双 key max-closure、C 完整密度/容量计划四者分列。若 cheap 双 key 对照追平，说明现例未证明更复杂密度或 ACK 预测的独有收益；不因此声称 DARTS 论文原来就有这个新增对照。

**根必读最小源范围：** 本文 §2–3 与本节；`darts-2025.c:1729–1761,2009–2105,2384–2442,3256–3268`（决策），`perfmodel.c:434–475`（费用/零大小），`memalloc.c:243–271,536–568`（引用/驱逐），`coherency.c:1147–1154,1375–1433`（执行前后完成边界）。若审消费回收再读 `darts-2025.c:3430–3469`。这些是根能直接点验并改变模型决定的证据，不必重复读整份实现。

**当前裁决：否决“DARTS 已完整覆盖，所以 C 立即结束”的说法；也否决“DARTS 没写 IOMMU，所以 C 已首创”的说法。保留一项已有源码级差异的正向规则，进入根的同合同模型门。**
