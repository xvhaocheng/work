## §40 交接插入：翻译寿命边界与创新门槛

- 范围：按根 Codex `codex-platform-gate` 审定和独立席推演整理；不新增机制设计，不作论文等级或分区预期判断。
- 结论：因为当前平台没有“普通译址结果必须等 DMA 数据完成才释放、并以该释放反馈阻塞 IOFENCE”的可达路径，源码可达性门槛否决当前 LTI-style 寿命反馈；不能用该依赖建立新准入机制。
- 已定边界：PR/PW 硬件 TODO 不是创新；软件默认 PR/PW=0；真实 ATS0；CQ fetch 仍可能等待共享内存/互连竞争；CQH 早推进不等 generic completion；§39 静态 6 PASS 不验证 live invalidation。
- 第二席 map agent 仍在收敛；ShareMMU §V-C7 仅写为 stalled miss 假设适用性问题；artifact 链接见 [iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt](iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt)；最终审定合并，不得假造完成。
- 保留窄问题与下一最小判别：错位 burst 尾 buffer/最小分片界；建议先只做 out 抽象模型/规格，不重跑 §39、不磨既有小差距；普通强对照包括配对预译址、共同分段、sequence fence、quiesce、完整 R 尾部预约、每子片独立译址/ready 队列/普通 budget-debt-refund。
- 新增 DUT build=0/run=0/BIT=0/board=0；未改生产 RTL/Linux/原 tests，未写 SD。
- 正式报告：[iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md](iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md)；根审定：[iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md)；第一席：[iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md)；DeepSeek 证据：[iommu-platform/out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md](iommu-platform/out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md)。
- 归属：DeepSeek evidence/doc drafting, Codex source/innovation design/agent/acceptance。
