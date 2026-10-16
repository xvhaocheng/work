# D完整COPY准备：选定前史对局部停止

2026-10-15，D-DRAM-ACTION-CONTRACT-1。ARS-Codex有界来源投影与确定性正确性验证；共享研究Codex-only。文档请求gpt-6.1-sol/medium，实际model/effort元数据unknown。

四例P/PN×row0/row1的完整COPY准备投影与独立核账通过，各完成V256/A257；同模式两前史在pre_t0与strong drain的有效DRAM状态、公开clock及next refresh相同，仅过期历史不同。准备过程六类轨迹不同，不能称同O_phys/O_host或整个RTL/IOTLB等价，也没有P/PN完整收益结论。局部停止这对前史的“准备后DRAM新增观测”版本，不准入完整P/PN动作比较。

实际4次新prepare-only run，0次完整64/64测量，0旧重跑、生产修改或硬件操作；host构建4次尝试/3次成功/1次失败，0 DRAM库/RTL构建。固定M0 warm256/gap0/guards未放宽。B是私有WR+16推导数据末，另保留至WR+34恢复；不是原生完成保证。D主线、I轻量not_ready、A5/A2与10月15—28日窗口保持；MIG1792023314 paused、WNS−0.391ns/598 setup失败/0新load，实际债务unknown。

下一仅D-BOUNDARY-SOURCE-2，尚未执行：先从真实来源确认正常task/phase边界，分清一次试验初始化与每任务准备，核quiet/config/clear、完整费用、持久task/buffer及最早信息；比较同信息public history/layout预测器与一个具体可达的新关系。Linux可请求warm不授权降低固定M0 warm256追正例；若没有真实边界或剩余缺口，明确裁决D优先级，不无休止扫状态，不自动转I或A。

## 裁决与冻结合同

- [root完整裁决](root/decision-analysis.md)、[稳定接受](root/root-result-acceptance.json)、[下一最小合同](root/next-minimal-contract.json)。下一合同尚未执行，本轮前史对不进入完整P/PN动作比较。
- [事前冻结投影合同](root/preparation-projection-contract.json)、[原COPY准备核查](preparation/findings.md)、[准备合同](preparation/preparation-contract.json)。
- [写完成源级卡](completion/completion-card.md)、[桥接合同](completion/bridge-contract.json)、[root实现审查](root/implementation-review.json)。
- [有效状态比较](root/effective-state-comparison.json)、[phase摘录](root/phase-excerpts.json)、[分析更正](root/analysis-corrections.json)。仅DRAM服务域等价，不扩为整个RTL/IOTLB等价；准备六类轨迹不同。

## 私有实现与四例原始证据

[实现交付说明](implementation/README.md)是工作席的构建时快照，其中“本席未运行”“unknown”记录当时状态，已绑定在运行前输入中，不回写它；后续实际运行与接受以[root执行账本](root/execution-ledger.json)、[稳定接受](root/root-result-acceptance.json)和[核账报告](verification/README.md)为准。[私有C++夹具](implementation/preparation-harness.cpp)、[完整私有差异](implementation/private-source.patch)、[桥接实现](implementation/ramulator-bridge.cpp)、[构建回执](implementation/build-receipt.json)保留。

| 案例 | 实际运行回执 | 原始事件 | 官方命令 | pre_t0状态 | strong状态 | 原始输出 |
| --- | --- | --- | --- | --- | --- | --- |
| p_row0 | [receipt](implementation/runs/p_row0/run-receipt.json) | [CSV](implementation/runs/p_row0/bridge-events.csv) | [trace](implementation/runs/p_row0/official-trace.ch0) | [pre_t0](implementation/runs/p_row0/phase-pre_t0_after_configs.yaml) | [strong](implementation/runs/p_row0/phase-strong_final_drain.yaml) | [stdout](implementation/runs/p_row0/stdout.log) |
| p_row1 | [receipt](implementation/runs/p_row1/run-receipt.json) | [CSV](implementation/runs/p_row1/bridge-events.csv) | [trace](implementation/runs/p_row1/official-trace.ch0) | [pre_t0](implementation/runs/p_row1/phase-pre_t0_after_configs.yaml) | [strong](implementation/runs/p_row1/phase-strong_final_drain.yaml) | [stdout](implementation/runs/p_row1/stdout.log) |
| pn_row0 | [receipt](implementation/runs/pn_row0/run-receipt.json) | [CSV](implementation/runs/pn_row0/bridge-events.csv) | [trace](implementation/runs/pn_row0/official-trace.ch0) | [pre_t0](implementation/runs/pn_row0/phase-pre_t0_after_configs.yaml) | [strong](implementation/runs/pn_row0/phase-strong_final_drain.yaml) | [stdout](implementation/runs/pn_row0/stdout.log) |
| pn_row1 | [receipt](implementation/runs/pn_row1/run-receipt.json) | [CSV](implementation/runs/pn_row1/bridge-events.csv) | [trace](implementation/runs/pn_row1/official-trace.ch0) | [pre_t0](implementation/runs/pn_row1/phase-pre_t0_after_configs.yaml) | [strong](implementation/runs/pn_row1/phase-strong_final_drain.yaml) | [stdout](implementation/runs/pn_row1/stdout.log) |

## 独立核账与执行来源

- [核账入口](verification/README.md)、[checker](verification/verify.py)、[正式报告](verification/report.json)、[实际执行回执](verification/execution-receipt.json)。四例已通过，5类负变异各在4例小状态被拒绝；查看结果无需重扫CSV。
- [raw绑定与六类轨迹首差](verification/evidence-bindings.json)、[历史有效性与源码](verification/history-and-source.json)、[服务数闭合](verification/service-bijections.json)。
- [root最终109项来源/生产源码/旧产物保护核验](root/final-source-verification.json)、[root实际执行账](root/execution-ledger.json)、[64项运行前输入](root/run-inputs.json)、[文档保护与链接回执](documentation/receipt.json)、[文档核验说明](documentation/README.md)。
- [前轮DRAM来源关](../d-dram-boundary-20261015/README.md)、[MQSim局部停止](../d-mqsim-identity-20261015/README.md)、[唯一总计划](../../../研究计划.md)、[当前状态](../../../CURRENT_STATUS.md)。前轮来源保留不等于本轮完整准备后仍保留差异；历史未执行字段按当时记录阅读。
