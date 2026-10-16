## §40 翻译寿命边界与创新门槛（2026-10-08）

- 根 Codex 已直接核关键源码并审定：因为当前平台没有“普通译址结果必须等 DMA 数据完成才释放、并以该释放反馈阻塞 IOFENCE”的可达路径，当前 LTI-style 寿命反馈被源码可达性门槛否决；Arm LTI 完成寿命规则不能直接当作本平台已实现约束。
- PR/PW 的硬件 TODO 不是创新；Linux 默认 IOFENCE 只写 dword0、PR/PW=0；真实 FPGA 配置 InclATS=0；CQ fetch 仍可能因共享内存/互连竞争等待，但 CQH 早推进不等 generic completion；§39 静态 6 PASS 不验证 live invalidation。
- 两个独立研究席已真实创建；第一席 `innovation_rules_40` 已有完整 artifact，第二席 map agent 仍在收敛。报告中 ShareMMU §V-C7 反例仅按根审定写为“小缓冲 copy 中 stalled miss 假设的适用性问题”，不声称已发现生产死锁；最终审定再合并其收敛结果，根会修正状态。
- 保留窄问题：错位 burst 尾 buffer / 最小分片界。普通配对预译址、共同分段、sequence fence、先停整 job/quiesce 必须作为强对照。
- 本轮新增 DUT build=0、run=0、BIT build=0、board run=0；未改生产 RTL/Linux/原 tests，未写 SD。正式报告见 [iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md](iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md)。根审定与证据见 [iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md)、[iommu-platform/out/translation-lifetime-boundary-20261008/source-evidence.json](iommu-platform/out/translation-lifetime-boundary-20261008/source-evidence.json)、[iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md)、[iommu-platform/out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md](iommu-platform/out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md)。
- 归属：DeepSeek evidence/doc drafting, Codex source/innovation design/agent/acceptance。
