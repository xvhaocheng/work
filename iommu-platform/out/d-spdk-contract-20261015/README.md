# D：SPDK来源合同与无计时语义原型

2026-10-15。**来源链及无计时闭环语义已接受；服务性能模型仍NOT_ADMITTED。** 全局D主线/I轻量、A5保留及10月15—28日窗口不变，固定M0旧门不变。

## Material Passport

ARS-Codex有界来源核查与experiment-agent/plan。固定SPDK v24.09及其DPDK gitlink，Linux v6.12只作参考实现；来源版本/hash见下列索引。交付范围是映射/事件源码证据、显式研究投影合同及synthetic clockless原型，不是原生测量。Codex-only：证据席请求Sol/high、设计席Sol/xhigh、实现/文档席Sol/medium；实际运行元数据unknown，请求不作运行证明。未恢复用户删除的个人研究方法。

## 证据入口

| 内容 | 文件 |
| --- | --- |
| 映射/IOVA/VFIO/PRP链、QD与固定池边界 | [映射结论](mapping/findings.md) · [来源索引](mapping/sources.json) |
| SUBMIT/COMPLETE字段、成功与drain边界 | [事件结论](events/findings.md) · [来源索引](events/sources.json) |
| S/M/N准入、研究资源/动作/观测/全费用 | [投影设计](design/proposal.md) · [结构化评估](design/assessment.json) |
| 冻结源语义合同与原型 | [合同](root/semantic-prototype-contract.json) · [原型说明](model/README.md) · [验证结果](model/verification.json) · [执行回执](model/run-receipt.json) |
| root判断、原件核查及稳定接受 | [决定分析](root/decision-analysis.md) · [关键源核对](root/critical-source-check.json) · [稳定接受](root/root-result-acceptance.json) |
| 只读重消费、执行与文档完整性 | [root回放](root/semantic-replay-check.json) · [执行账本](root/execution-ledger.json) · [输入清单](root/input-manifest.json) · [文档回执](documentation/receipt.json) |

N4/Q2仅验证常量，8种接口允许callback次序、6个负变异拒绝、2个错误案例通过；不证明全部次序原生可达。root只读重消费已登记synthetic事件，未重跑生成器。实际/联合trace各0，无服务计时/原生/P-PN收益。SC/SCT定义错误，io_completed含错误；perf elapsed在最终drain前采样。stock QD改task池及翻译footprint，固定池admission/pacing须明确实现并计费。

下一关：冻结一个source-to-service投影的provider/参数域、资源/方向/动作、完整O0与付费Z、全费用/完整成功终点、跨quiet持续性、普通强对照和留出。来源绑定显式研究模型不以原PC/SSD原生P/PN、真实joint trace或先实测反号为普遍前置条件；部署主张与校准仍需相应真实证据。每个动作独立演化完成→补发反馈。

本阶段0旧实验重跑/构建/原生DMA/Linux或RTL或UAPI修改/collector/FPGA或板操作。MIG暂停、旧时序失败与债务unknown保持。当前安排见[总计划](../../../研究计划.md)，历史M0来源门见[上一阶段](../d-source-gate-20261015/README.md)。
