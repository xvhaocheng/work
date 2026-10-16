# D/I首批证据与研究入口整理

2026年10月15日。**本批完成入口统一、12份原文归档和D/I第一轮证据整理。D继续处于来源判别关口；现M0没有所需反号状态对，I的真实共置部署尚未就绪。** 后续实验关口保持，本批交付不代表论文创新成立。

当前执行从[唯一总计划](../../../研究计划.md)和[研究导航](../../../研究导航.md)进入，实际进展见[状态页](../../../CURRENT_STATUS.md)。两周窗口仍为10月15—28日。A5及其他分支、独立工程贡献保留；MIG1792023314继续暂停。

## 首批研究结论

| 问题 | 已接受的证据 | 仍缺什么 / 当前裁决 |
| --- | --- | --- |
| D实际动作 | P/PN仅在quiet边界配置，配置与clear、generation核对有源码依据；用于下一任务 | CPU/MMIO、quiet等待、clear、预热与切换费用尚未测得，不能假设任务内免费切换 |
| D现有完整效果 | 6份M0原始日志：A1持平；A8/FIFO的PN−P为(3689,3689,3691)周期；A8模型DS优先后持平，向量为(T_V,T_A,T_return) | 没有PN获益侧；已知页环和服务配置也能区分各组。当前资料不能支持O0不可分且效果反号的状态对 |
| D信息可得性 | 现有板端窗口计数与模型完整事件分别列明；模型`t0→quiet`有双方完整COPY证据 | 当次事后结果不能成为同次事前信息；候选跨owner联合摘要的身份、及时性和部署费用未知 |
| I正常生命周期 | ixgbe普通TX的map→DD完成检查→unmap，以及条件DMA-IOMMU/FQ调用链有原始源码 | 当前机器/FPGA的共置实例、domain、实际DMA后端、live预算、命令与回收联合trace均未取得：not_ready |
| I普通对照 | 当前host失效未设PSCID/ADDR，而IOTLB具备精确匹配分支；批处理、持久映射、分domain须给足机会 | 尚未证明正确普通方法不足。作者提出的偶发full-walk容忍预算反解释必须检验 |

[D来源与动作费用](d/source-and-action-costs.md) · [观测可得性](d/observation-availability.md) · [假设卡](d/hypothesis-card.md) · [完整源摘录/原始记录](d/assessment.json)。[I预筛](i/prescreen.md) · [源码和原讨论定位](i/source-evidence.md) · [评估](i/assessment.json)。

## 贡献边界与下一最小证据

已核的最近工作Dead on Arrival v2 §5.1—5.4、§6—7.3已有相似统计下的结构区分、不同保护效果与组合退化；CounterPoint、MISE/ASM、ObservUVM已有观测、干预、付费反馈选策。其方法位置与阅读限制复用[前轮比较](../../docs/independent-research-directions-20261015.md)，本批没有重新广搜。D要增加的是：给普通方案相同可得结构信息后，仍存在影响P/PN完整选择的信息缺口；新增及时且付费的联合信息能够纠正未见状态上的决策，并覆盖完整取得和执行成本。当前只接受此条件问题与缺项，不接受新意或净收益。

D下一交付限定为一对来源卡：同一任务合同、自然输入/就绪的源码或轨迹来源、共同目标/后台预算、全部事前O0及其精度、合法P/PN动作和预期反号完整后果。第一步找不到来源或已知信息足够，就结束该版本的信息缺口主张；不扩gap、修改guard、搭collector或重跑旧矩阵寻找赢家。只有来源门满足，才冻结有界模型验证合同。

I下一步仍是取得一个正常实际部署的有限联合切片及live full-walk余量，先核普通精确失效等是否足够。D未通过不能自动提高I优先级，也不自动退回A；I缺证不等于方向被否决。

## 文档与归档

- [归档索引](../../docs/history/plans/README.md)保存12份原文、124106字节、1039行；SHA、字节和行号一致。两个相对symlink保留旧行号路径，另10个旧路径移除。
- [迁移清单](../../docs/history/plans/manifest.json)登记原/新路径、用途、后继及每个旧本地链接的定位；三份状态快照原有14个失效链接独立标注。归档原文中的相对链接未重写，由索引定位。
- 13份维护报告仅修16个Markdown链接目标；[回执](archive/archive-acceptance.json)和[变更表](archive/maintenance-link-changes.json)保留实际验证。
- 根计划/导航/README、根与平台AGENTS、平台README及历史目录入口统一。[入口同步回执](entry-sync/receipt.md)保留修改前原件与检查；旧内容图和9月代码地图仍为历史快照。
- [A5局部合同](../../docs/research-execution-plan-20261011.md)和[含实际结果的stage117报告](../../docs/stage117-plan-revision-g0-20261013.md)留原位；全局与分支安排分开记录。

## 接受、核验与执行边界

[root接受](root/root-result-acceptance.json) · [关键原件核对](root/critical-evidence-check.json) · [交付完整性](root/delivery-verification.json) · [执行与配置账本](root/execution-ledger.json)。完整性通过只覆盖文件、链接、hash及记录一致性，不能冒充原生应用或性能验证。

本批没有Linux/RTL/ABI修改、新模型运行、旧实验回放、编译或FPGA/板操作，未创建/恢复goal。读取旧6份日志并计算差值属于证据核对。M0未覆盖的应用配置、预热、consumer与free费用保持未知，不将E3、PN板测与M0拼成同一正反试验。

继续Codex-only。新入口同步子任务请求`gpt-6.1-sol / medium`，实际配置因工具未回传而标未知；已完成旧席结果复用，不因新规则重跑。主会话负责关键源码/原件核对与最终接受；不能把文档更新或请求配置当成已经切换当前会话模型的证明。
