# 多 DMA 共享 IOMMU 全景路线审查

日期 2026-10-10 ｜ stage 95 ｜ 性质：只审查，不改原研究计划与生产源码

## 1. 直接结论

**原始方向合理，近期推进确实发生偏离，最新计划只完成了部分纠偏。** 我的建议是保留共享 IOMMU 干扰问题域的一次有限立题机会，但取消“逐 copy 状态准入必然是主解法”的默认地位。这个建议来自已有证据可继续判别问题、补充判断的成本相对可控，不代表已证明该方向更容易产生创新。

原学位论文研究单个 DMA 的远端页表遍历代价；后来转向多个 DMA 共享 IOMMU 时，一个设备的翻译活动怎样拖慢另一个设备。这是合理的新问题，却不是原论文自然保证会产出的新方法。原文把访问控制覆盖当拓扑前提，用 FPGA 延迟模型研究性能，见[原论文目标与方法](/home/xvhaocheng/thesis/2026_thesis_eng2/main.tex:180)。因此，后续不必继续绑定 Chiplet，也不能把原平台当作真实 Chiplet P2P 安全或完整互连行为的验证。

偏离发生在问题与候选的关系上：后期越来越多工作围绕“怎样让这个机制获得独有优势”，而真实需求、固定目标和可接受成本没有同步稳定下来。P/N/PN 是有价值的诊断；但 P 与 N 各自有作用，并不意味着组合必须获胜。10月7日至9日转向分段、共享载荷、晚选目的与在途加入时，需要新的独立问题依据，原来的固定目的、分离缓冲 COPY 不能自动提供这些依据。

**最新计划。** G0→G1→G2→G3、共同工作量口径、完整成本账、留出与消融都是进步。但研究计划原文的 G0 没有明确允许基线先判别；允许一次有界既有基线判别是本次/先前审查建议，不是已落实条款，也不等于测量实现已修。计划未改，接口也未适配：现有模型只测首笔 V issue 到最后一笔 V done 后停 A，不能原样用于共同就绪、双方各 64 次完成的共同完成时间。copy-state gate 仍是候选，不是默认主解法；一阶段一次的 DMA 前端 WAIT 会先完成当前 A copy，上一笔 copy 状态是历史状态，V 可能仍活跃，门控不能回溯修复已承诺的当前 copy，候选必须写清哪个状态对哪个决策及时。

**下一步建议。** 只做一件有界的事：在有动机的固定工作流里，检验某个具体、可执行的普通控制是否仍留下有用且可归因的损失。不是让普通控制真实失效；如果普通控制已经够用，那是有效结果，不人为造失败。这些都是后续建议，本轮未改测量接口或启动实验。

## 2. 时间线

| 阶段 | 做了什么 | 判据/状态 | 事后怎么看 |
|---|---|---|---|
| 原学位论文 | 把访问控制覆盖当拓扑前提，在 FPGA 延迟模型里评估远端 PTW 代价；链接 [main.tex:90](/home/xvhaocheng/thesis/2026_thesis_eng2/main.tex:90)、[main.tex:180](/home/xvhaocheng/thesis/2026_thesis_eng2/main.tex:180) | 模型内代价成立 | 合理；但没有用实验证明真实 Chiplet P2P 安全，也没有规定后续必须留在 Chiplet |
| 9 月 | 测量修复（E4 no-overlap 与 autonomous-supply overlap 报告支持修测量）、E3 有界因果诊断；P4M 进度测量形成 | 修测量与局部因果成立 | 修测量是对的；E3建立了当前实现内的局部等待传播证据 |
| 10-02 | P4M 补齐 60 条记录：53 在线加 7 离线，跨启动 | 见 [root-result-acceptance.json](../out/research-panorama-audit-20261010/root-result-acceptance.json) 文档更正 | 补齐后此前 P4M 未完成说法应撤回 |
| 10-03 | E2：W7/W8 forward 各 5 条原始记录，中位数 696 与 1483 cycles，probe miss 0 对 2 | 直接抽核见 [raw-spotcheck.json](../out/research-panorama-audit-20261010/raw-spotcheck.json) | 结合旧条目替换重放，支持有限条件下的替换与首次再用代价；不是缓解方法收益 |
| 10-04 | method88：用冻结匹配规则比四个格子 | 四格候选相对匹配强基线的额外收益均为负 | 有效否证固定 A8 加该版 p5 的额外收益主张；不能推广成所有翻译感知控制无用 |
| 随后 | P/N/PN：同一 BIT 下做 P、N、PN 干预与双方观测 | PN 对 P 的实板 W 差约 12.67–12.79% | 是有用诊断与反例，不能据此认定候选有可用收益空间 |
| 10-07 至 10-09 | 碎片、共享载荷、晚选目的、在途加入等契约 | 机制先行 | 具体失效能停掉具体主张，但不能否掉整个子领域；在途加入对短补读加尾段共享的平均耗时低17.16%，但许可已齐时普通起点共享还比在途加入快57周期，接受路径见 [codex-result-acceptance.md](../out/shared-read-inflight-join-20261009/codex-result-acceptance.md)，但局部例不足以建立独立新颖性或净系统收益 |
| 10-10 | 最新计划 G0 到 G3、共同工作量与成本账、留出 | 先找普通失效 | 方向对，但仍提前偏向 copy-state 准入；计划未改，接口未适配 |

## 3. 地图、贡献与四项裁决

地图（[研究地图](research-landscape-20261005.md)、[共享翻译地图](shared-translation-literature-20261006.md)）起两个作用：提供近邻机制与必须比较的强对照，并限定主张边界。盲区同样要写清：地图组织相邻机制与约束，不是已被证明的开放问题清单，不是六条必须同时实现的任务；没看到同名工作不等于空白；地图不能替代工作负载动机，也不能证明某个具体信息或动作在这块板上有效。文献只按根裁决已直接核部分使用：[Bermuda RTCSA2025](https://arxiv.org/html/2508.19670v1) 支持 DMA 共享 IOMMU 干扰已有直接前作，且其自身缓解评估不完整，不能据此作领域级新颖性主张；[MASK ASPLOS2018 §5.4](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) 支持数据/翻译争用、PTE 优先级与应用感知数据调度已有前作，但 GPU 控制点不自动映射到 DMA；[DAC2023](https://libingyao.github.io/assets/files/DAC2023.pdf) 图 10-11 附近支持分区/调度交互与命中率/性能分离已有前作，但不是同一 PN 实验；[DWS HPCA2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 的直接核对只支持把 MASK 与 DWS 组合不一定带来显著额外收益，这是组合收益不自动叠加，不是本板 slowdown 的复现。

**问题域价值：成立（有保留）。** 共享 IOMMU 下 DMA 之间的翻译与服务干扰既有直接前作，也在这块板上有可观测现象，值得保留一次有限可行性推进。

**近期候选没有形成已确立的独立主贡献，理由各不相同。** 在途加入的现有正例被普通起点共享以更少读取、更短完成时间击败，因此不能用该例证明独有加速。L-M的静态输出集合例被提前分配加短期pin复现；L-W的三次PTE访问被有成本的checkpoint、取消与部分翻译缓存复现，不能继续把少一次PTE当作独占能力。不过，这些对照没有证明全部动态场景、完整延迟和面积都相等，也不是完整新合同已经发表的证据。[在途加入接受记录](../out/shared-read-inflight-join-20261009/codex-result-acceptance.md)、[round05原始裁决](../out/higher-model-mechanism-20261009/round05/root-result-acceptance.json)。

当前copy-state gate还没有新实验；它需要说明可达的决定差异，而不是仅靠增加字段立题。新信息不是创新的必要条件；用已有信息形成新的规则、降低完整实现成本或提出有预测力的设计边界，都可能有贡献，前提是相对准确前作有具体、可核证的增量。不能要求候选击败装入其完整新规则的“普通可编程控制”，那可能只是同一方法的另一实现。

**性能增量：旧method88已经否定该版p5的额外收益主张，本轮确认该有限结论；近期候选仍缺净系统收益证据。** method88 四格的 -5.68952036%、-8.23688932%、-5.15142380%、-4.43680473% 是相对增益百分比，不是百分点；它们来自按冻结规则逐格选出的匹配强对照，不能推出不存在通用在线策略，只能说明 best-per-cell 包络不是已部署的在线策略。E2/E3 是局部因果证据，不是候选收益；PN 约 12.67–12.79% 是诊断差额，不能据此认定候选有可用收益空间。

**论文贡献：作为主贡献未由证据建立。** 平台功能验收、计数正确与协议正确只能说明工程可用，不能说明论文有贡献。协议正确、机制新颖、性能增量、论文贡献四件事必须分开裁决。用户受控首篇目标不应自动等同于必须提供新算法，方法优先；若最终只能给出新的可预测设计边界或完整成本优势，也可能有贡献，但不自动改投为表征论文。

## 4. 偏离与最新计划

这并不是现在才发现的流程问题。[9月9日首篇审查](research-review-2026-09-09.md)已经要求围绕一个能影响设计选择的主张收敛，并警告把实现特性写成普遍发现。10月5日至6日的[性能路线讨论](performance-council-decision-20261005.md)与[创新预审](novelty-gate-20261006.md)又要求固定工作流、复用强基线、先说明增量。后续工程和候选仍持续扩张，说明仅增加审查文档不足以改变推进方式。

停止经过检验的p5收益主张是正确的；另一个需要纠正的风险，是把“一例被追平”“旧组件可组合”“整个方法没有贡献”混为一谈。[既有关键裁决审查](../out/route-audit-20261009/critical-verdict-audit.json)已记录这种门槛缺陷及后续纠正。本次不将它扩大成所有历史否决都错，也不自动复活旧候选。此前Codex参与了机制优先推进及过严门槛的设定，应承担相应判断责任；没有实测依据量化浪费比例或宣称某种流程节省多少成本。

最新计划把顺序拉回先找普通失效、再谈新机制，这是进步。但 G0 原文没有明确允许基线先判别；允许一次有界既有基线判别是本次/先前审查建议，不是已落实。计划未改，接口未适配。现有模型源码口径见 [tests/nonblock_actual_dma_test.cpp:1630](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:1630) 的首 V issue 到末笔 V done 窗口，以及 [tests/nonblock_actual_dma_test.cpp:2046](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2046) 的 waitV 后 STOP A；[tests/nonblock_actual_dma_test.cpp:2006](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:2006) 的 A/V 触发顺序也不能原样用于共同就绪。控制侧 [idma_reg64_frontend.sv:519](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:519) 保留 COUNT VALID、等当前完成再 GAP/ADMIT；[dma_research_policy.sv:285](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_research_policy.sv:285)、[dma_research_policy.sv:375](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_research_policy.sv:375) 已观测翻译事件并把 V 进度反馈进 budgets/gap；[dma_composition_control.sv:155](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_composition_control.sv:155) 有 quiet/config/clear 约束。这些直接读到的时序说明：共同就绪 64+64 口径尚未适配，copy-state gate 的及时性也仍需论证。

## 5. 建议

1. G0 先写清楚要检验的具体普通控制、它可能留下的有用损失、以及实际还能控制什么；若证据不足，允许一次预先登记、有界的既有基线判别。注意这不是计划原文已明确允许的条款，而是审查建议，尚未执行；不要求候选存在，也不要求候选与基线选不同动作。
2. 若需要共同就绪、双方各 64 次完成的完成时间口径，先做最小测量适配；现有模型不能原样复用。共同逻辑就绪时刻起分别记录V与A的完成时间，把准入等待计入；正常结束须验证双方各64笔真实读写和B完成及数据有效，不能V结束就提前STOP A。旧窗口可保留为辅助观测。
3. 判别出可操作缺口，才进 G1 选信息/动作并跑一正一反；否则结束本轮或暂停该分支，不自动恢复旧候选，也不自动改投表征。
4. 机制新颖性与性能收益在 G2 分别裁决；累计建议不超过 10 个工作日作立题上限，它只是管理建议，不是科学定理或已启动计时，可提前结束。这些建议不自动恢复旧实现队列。

## 6. 证据范围与核查记录

本次只审查。研究计划与生产源码不改；没有新性能实验、没有 RTL/Linux 修改、没有构建、没有上板；不声称实施路线已经修订，也不声称新的科学结论已被验证。以下数字只作有界复核，直接抽核见 [raw-spotcheck.json](../out/research-panorama-audit-20261010/raw-spotcheck.json)，历史完整验收分别复用[E3报告](e3-controlled-results-review.md)、[E2报告](../out/e2-results-review-20261003/RESULTS.md)、[method88正式结果](../out/method88-execution-20261004/RESULTS.md)。原学位论文数据未重新复现，未重验全部历史日志，不宣称PN板端完整归因或领域前作穷尽。

- method88 四格为 -5.68952036%、-8.23688932%、-5.15142380%、-4.43680473%（相对增益百分比）；本次无新的显著性检验，也不作通用在线最优性主张。
- E3 四对互补配对的 A 剩余延迟与 V 剩余延迟增量都是 127、129、129、130 cycles，V first-hit 全为 1；仅限一次开机的 64B 诊断，是局部因果证据，不是缓解性能结果。
- E2 W7/W8 forward 各 5 条原始记录，中位数 696 与 1483 cycles，probe miss 0 对 2；本轮直接抽核覆盖 timing/probe counters，完整 eviction replay 复用已接受的历史审计，本轮未重跑。直接抽核与复用旧完整证据要分开。
- PN 实板三次 PN 对 P 的 W 差是 12.79215853%、12.67441373%、12.76202370%，同窗 A4096、V4096、raw17 下 A miss 2048；旧 HPM 语义是 raw-only，不作已验收 banked 指标。该差额不能据以认定候选有可用收益空间。模型侧约 1.744% 是历史模型结果，可引用 [composition-history-isolation-20261007.md §8](composition-history-isolation-20261007.md)，与实板口径不同，不能互相替代或用模型数宣布收益。
- 64+64 是第一个机制微基准，不是已部署应用或带真实期限的负载，也不足以证明一般有用性。进入最小判别无需先移植完整应用；后续推广应检验失效条件，而不能凭空限制大页、提前准备或已有资源。
- copy-state gate 是候选，不是默认主解法；候选必须写清哪个状态对哪个决策及时。
- round05 的原始 root-assignment-pin-review.json、prefix-control-critical.json 与在途加入接受记录见 [root-result-acceptance.json](../out/higher-model-mechanism-20261009/round05/root-result-acceptance.json)；静态 mask/PTE 计数复现不等于完整延迟/成本等价，也不等于已覆盖整个提案。
- 保留资产：修测量、E2/E3 的局部因果、P4M 与 method88 的可复用进度测量与对照，以及已有审查的证据组织方式。历史文本里的 E2 未证与旧 HPM 可疑属于当时时点，不能当现在的全局故障证据。
- 根裁决与账本：[root-result-acceptance.json](../out/research-panorama-audit-20261010/root-result-acceptance.json)、[worker-ledger.json](../out/research-panorama-audit-20261010/worker-ledger.json)；计划与状态：[研究计划.md](../../研究计划.md)、[CURRENT_STATUS.md](../../CURRENT_STATUS.md)。
- 来源冲突简短附注：两份 research-route-audit Markdown 当前 byte-identical，不是两份独立审查；其称原论文只凭总览回忆，与旧根接受记录中带行号直接核对的说法冲突，本次只记录，不推断成因。
- 分工：Codex 负责实质设计、关键源码与因果验收、路线与机制裁决、最终验收；DeepSeek在本任务提供历史与定向证据导航、报告草稿和一次定向修订；Codex执行原始数据抽核、核稿修正及实际保存。本轮没有新增子agent或调用Claude。