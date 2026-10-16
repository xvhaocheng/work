## §40 翻译寿命边界与创新门槛（2026-10-08）

- 本阶段 Codex 已执行只读源码命令并写入证据/草案文件；没有 DUT build/run/BIT/board，没有改 production RTL/Linux/test，没有写 SD。报告为正式版。
- 根 Codex 已直接核关键源码并审定：因为当前平台没有“普通译址结果必须等 DMA 数据完成才释放、并以该释放反馈阻塞 IOFENCE”的可达路径，当前 LTI-style 寿命反馈被源码可达性门槛否决；Arm LTI §6.2.1 独立完成义务与 §6.2.2 多事务寿命必须分清，不能直接套用。
- PR/PW 的硬件 TODO 不是创新；Linux 默认 IOFENCE 设置 opcode/func 与 dword1=0，普通同步调用 PR/PW=0；真实 FPGA 配置 InclATS=0；CQH 早推进不等 generic completion；§39 静态 6 PASS 不验证 live invalidation。普通 fault/error+discard 是强进度控制，当前 fault 排空未验。
- 两个 native Codex 研究席均已完成：第一席 `innovation_rules_40` artifact 为 [iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md)；第二席 map artifact 为 [iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md)。ShareMMU §V-C7 只写适用性问题，不称生产死锁。
- 第二席裁掉常规规则、PT 着色、共享计账、nonblocking walker 的独立创新可能，理由为强前作覆盖；NB-Walker 新作者 artifact 已取得代码并只读保存，但论文全文仍未得、未复现实验。
- 下一单一判别规格以 [codex-next-decision.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-next-decision.md) 为准：固定映射，long copy 内可控让路，小缓冲下最小合法前沿/分片成本界；temporary translation lock 只属 ShareMMU 适用性反例，不是优先给生产新加锁，也不是当前 IOTINVAL 语义。
- 主报告没有排除强普通的 new rule，因此不能认定独立机制；保留待证明锐界作为分析候选。下一判别未 run；禁止假造完成实验/新算法/锐界证明。
- 正式报告：[iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md](iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md)。根审定、证据、下一判别：[codex-platform-gate.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md)、[source-evidence.json](iommu-platform/out/translation-lifetime-boundary-20261008/source-evidence.json)、[deepseek-source-evidence.md](iommu-platform/out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md)。
- 归属：DeepSeek 整理证据和起草文档；Codex 根与 2 研究席作创新推演、关键源/原文核查、最终审定；未使用 Claude。
