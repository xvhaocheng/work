# 新主会话研究交接入口（2026年10月13日，Asia/Tokyo）

用户已明确要求换新主会话继续研究。旧主会话stage112交付结束，旧goal complete；本轮只准备交接，没有登记或执行新的研究goal。新主会话以gpt-6.1-sol／ultra继续，实际登记并执行下一个有界来源选择关口，无额外token预算。启动文字不能替代实际模型选择或运行证据。

启动提示词：[START_PROMPT.txt](START_PROMPT.txt:1)。新主会话的有界合同：[next-goal-contract.json](next-goal-contract.json:1)。它承接原stage112 inactive合同，明确用户已经要求继续来源选择；没有把未成立的实验入口改成已接受G0。

## 1. 最小读取顺序

1. 工作区[CURRENT_STATUS.md](/home/xvhaocheng/work/CURRENT_STATUS.md:1)及两级[AGENTS.md](/home/xvhaocheng/work/AGENTS.md:1)、[平台AGENTS.md](/home/xvhaocheng/work/iommu-platform/AGENTS.md:1)。必要时核适用祖先指令，不默认扫描全仓或全部历史。
2. 本文件及本目录next-goal-contract.json。旧主会话ID为01a12eff-c971-7c62-8071-a08b646a4b61；新席身份分列。可读时记录本席实际线程ID/model/effort；缺字段标未验实，不用旧运行记录证明新席。
3. [整体计划](/home/xvhaocheng/work/研究计划.md:1)、[执行计划当前条目](/home/xvhaocheng/work/iommu-platform/docs/research-execution-plan-20261011.md:5)，按决定问题定向读[研究方法](/home/xvhaocheng/work/研究方法.md:1)及关键原文/源码。
4. 亲核下面稳定裁决及必要原数据。历史HANDOFF不要求一次全读；不重做已验收源核或矩阵，仅在新问题依赖的关键边界定向核原始证据。

## 2. 当前结论与关闭范围

| 分支 | 已接受事实与结论 | 新会话不得自行做的事 |
|---|---|---|
| stage111有限R107 | 真实FPGA四4MiB backing、8真实2MiB叶、总16MiB；priorout/restore各1,769,472B；恢复1728hit/0miss/0walk，完整字节/DONE/quiet/checked release通过。本有限卡归档 | 不增加槽/序列、缩页、冷却、强制多DID或加压力复开；不重跑stage106/108或此pilot |
| stage112 C112当前FPGA投影 | 固定vLLM TP2每层两处RowParallel归并及持久GraphAllReduce buffer成立；本有限形状每rank活动12,288B。原板joint layer只是全恢复后CPU字节核验，新partial/collective/实际共享requester合同未成立；当前投影归档 | 不把CPU检查说成GPU消费、CUDA stream说成DID，不搭collective模拟器继续追收益 |
| 更早有限收益分支 | 固定A8 p5／旧P-PN差额门控、行预取、HERO／固定流分支、共同ready冷权重、可选中转/共享copy等已按各自条件结束 | 不恢复同输入收益主张或跑已验矩阵；不能把有限分支结束说成整个共享IOMMU领域失败 |

仍处于阶段一立题，方法论文未完成；没有新G0/G1或接受的新规则。保留合法大页、预映射、驻留、批量、布局和授权单DMA合并；不同任务角色不自动要求不同安全owner或DID。

## 3. 决定性证据导航

- R107：[稳定裁决](/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012/root-result-acceptance.json:1)、[完整校验接受](/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json:1)、[原始receipt](/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/live/finite-pilot-result/receipt.json:1)。重点JSON路径run.composition、run.hpm_before/after、run.leaves/batches/stages。receipt为单行，原目录含完整pool与UAPI二进制，必要时读原数据，不能只用摘要。
- R107[全成本：周期、nominal、Dell条件界](/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/costs-and-boundaries.json:1)。kernel whole名义21.960371360s、restore名义45.192160ms只属原有限任务；新源值、GPU计算、到达、期限未知，不用旧时间填新任务。
- C112：[稳定裁决](/home/xvhaocheng/work/iommu-platform/out/post-r107-demand-gate-20261013/root-result-acceptance.json:1)、[完整报告](/home/xvhaocheng/work/iommu-platform/docs/post-r107-demand-gate-20261013.md:5)、[root关键引文/hash](/home/xvhaocheng/work/iommu-platform/out/post-r107-demand-gate-20261013/critical/root-source-review.json:1)、[有限字节合同](/home/xvhaocheng/work/iommu-platform/out/post-r107-demand-gate-20261013/source/finite-consumer-byte-contract.json:1)。
- 旧E3仅提供有限A-miss→pending V-hit等待传播能力：[case6原始对与字段](/home/xvhaocheng/work/iommu-platform/out/post-r107-demand-gate-20261013/critical/existing-e3-boundary.json:1)、[受控复核](/home/xvhaocheng/work/iommu-platform/docs/e3-controlled-results-review.md:33)。它不是新应用到达、自然cold miss、期限或缓解收益证明；不重跑。
- [stage112账本](/home/xvhaocheng/work/iommu-platform/out/post-r107-demand-gate-20261013/execution-ledger.json:1)、[实际goal complete回执](/home/xvhaocheng/work/iommu-platform/out/post-r107-demand-gate-20261013/goal-completion.json:1)。此后没有新研究实验。
- 原依据：[stage112 inactive合同](/home/xvhaocheng/work/iommu-platform/out/post-r107-demand-gate-20261013/next-goal-contract.json:1)。本目录新合同区分来源核查goal与实验准入，保留原关闭范围和G0/G1门槛。

## 4. 下一个有界关口

建议stage113。问题：在一个未关闭、有真实来源的工作流中，自然独立数据搬运能否合法投影到现有共享iDMA/IOMMU路径，并暴露完整普通控制之外的翻译缺口？GPU/AI优先、不排除其他真实需求。新主会话负责定向选择一份来源，不要求用户先替它找到新事实。

先说明问题、已有证据、来源选择范围、进入/结束条件，再用create_goal登记并立即执行来源与可达性裁决，不设token预算。这里只授权定向选择一个来源工作流；不是预先接受新场景矩阵、实现、部署或关闭分支复活。选定后固定名称/版本/原出处及相对关闭卡的实质新增事实；没有证据就交付具体缺项与裁决，不自动加第二个工作流凑候选。

交付至多一张源合同卡：完整任务/owner/producer/consumer/依赖/lifetime/bytes、自然共享路径及当前合法接口、强普通对照、全部准备/供给/执行/消费者/验证/排空/转换/释放与资源成本、正条件与相邻反条件、推进/条件保留/归档决定及下一步。真实应用、有限投影、模型/RTL、FPGA执行分开；没有原生计算、到达或期限证据时留未知。

任务/接口成立后才考虑必要实现；具体可达、可避免的普通控制外残余成立才登记最小G0。本关口默认结束在来源/接口裁决，不顺手开始G0或设计G1；后续另以接受结果确定有界合同。具体规则的准确前作与完整成本仍须亲核，不以模型共识验收创新。

## 5. 新席与物理平台边界

- 主及必要新增各级席gpt-6.1-sol／ultra；原生上下文/自动压缩默认，不加model_context_window/model_auto_compact_token_limit/model_catalog_json。旧258400与已验475000记录都不是新会话实际窗口证明；本任务不再做完整窗口审计。
- 独占关键问题确有需要才新建Sol/ultra席，fork_turns="none"，给精确证据、接口、接受条件及充分必要上下文。不恢复旧席、不默认fork全部历史、不例行重复复审。名额及实际运行看本次工具结果，不假定旧树已释放。
- 旧三席历史任务完成、12入口保留，但原主会话缺close能力，实际关闭0、释放未验；interrupt/archive不当close。[最近清理记录](/home/xvhaocheng/work/iommu-platform/out/agent-cleanup-20261012T165130Z/result.json:1)。本文件不声称新会话已建立或旧席已关闭。
- 主平台Genesys2/CVA6＋四iDMA＋RISC-V IOMMU/Linux；服务器核源、构建/仿真。GPU/AI优先不等于部署真实GPU系统。本关口不默认启动长构建、仿真或板测。
- 最后已验板端r1 boot370a9dd6-7d1a-4190-8395-acaf3e25f69c、kernel6.5.0-rc2-r107-ordinary-stage111-r1、已批准f9fa9434…冻结bit、S0；f9不含独立生产RTL57eb562。本交接没新读板，不把旧身份当新运行，不追JTAG IDCODE1/旧横幅重复审计，不自动重启、写SD或刷bit。
- 独立Linux、上游PR、RTL维护不在此来源goal内；保留其工作树、PR和CURRENT_STATUS尾部。凭据仅用既有非秘密引用和Secret Service，不写入提示词、产物或子席上下文。

## 6. 完成与记账

完整产物保存独立新目录，保留原文/源码/原数据、hash、精确行号、实际执行与模型账本、稳定root-result-acceptance.json和下一步；同步CURRENT_STATUS/HANDOFF/相关验证记录，保留旧历史。交付齐备才update_goal complete；goal工具不可用则保存同等合同继续，如实写未登记。不能为结束而缩小或替换未完成goal。
