## §40 交接插入：翻译寿命边界与创新门槛

- 范围：根 Codex `codex-platform-gate` 审定，第一席 `innovation_rules_40`、第二席 map 均已完成；不新增机制设计，不作论文等级或分区预期。
- 结论：因为当前平台没有“普通译址结果必须等 DMA 数据完成才释放、并以该释放反馈阻塞 IOFENCE”的可达路径，源码可达性门槛否决当前 LTI-style 寿命反馈；Arm LTI §6.2.1 独立完成义务与 §6.2.2 多事务寿命分清后也不能直接套用。
- 已定边界：PR/PW 硬件 TODO 不是创新；Linux 默认 IOFENCE 设置 opcode/func 与 dword1=0，普通同步调用 PR/PW=0；真实 InclATS=0；CQH 早推进不等 generic completion；§39 静态 6 PASS 不验证 live invalidation。
- 强进度控制：普通 fault/error+合法 discard 必须列为强对照，保持 copy 成功是性能额外合同，不混安全或成功工作量；当前 fault 排空未验。
- 第二席裁掉常规规则、PT 着色、共享计账、nonblocking walker 的独立创新可能；NB-Walker 新作者 artifact 已取得代码但论文全文仍未得、未复现实验。
- 保留窄问题与下一最小判别：错位 burst 尾 buffer/最小分片成本界；以 [codex-next-decision.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-next-decision.md) 为准：固定映射、long copy 内可控让路、小缓冲下最小合法前沿/分片成本界。temporary translation lock 只属 ShareMMU 适用性反例，不是生产新加锁，也不是当前 IOTINVAL 语义。
- 主报告没有排除强普通 new rule，因此不能认定独立机制；待证明锐界只作分析候选。下一判别未 run，不假造实验/算法/证明。
- 报告：[iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md](iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md)；根审定：[iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md)；两席：[innovation-rules-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md)、[innovation-map-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md)；证据：[source-evidence.json](iommu-platform/out/translation-lifetime-boundary-20261008/source-evidence.json)、[deepseek-source-evidence.md](iommu-platform/out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md)。
- 归属：DeepSeek 整理证据和起草文档；Codex 根与 2 研究席作创新推演、关键源/原文核查、最终审定；未使用 Claude。
