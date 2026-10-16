## §40 验证边界（2026-10-08）

- 本轮为源码可达性门槛与文档起草，不是新 DUT、板测或通用正确性证明。
- 新增 DUT build=0、run=0、BIT build=0、board run=0；未改生产 RTL、Linux 或原 tests，未写 SD/加载 FPGA；前阶段数值只引用，不在本节展开。
- §39 的静态 DUT 把 CQ 请求、IOTINVAL、IODIRINVAL 固定为零，因此其 6 项 PASS 不验证 live invalidation、不验证 IOFENCE PR/PW 排空，也不验证 CQH 早推进与 generic completion 的差异。
- CQ fetch 仍可能通过 DS/内存通路受互连竞争影响，这不是 LTI-style 寿命反馈；CQH 早推进不等于 fence 已执行或前序命令已 commit。
- 第二席 map agent 仍在收敛；ShareMMU §V-C7 反例只按根审定写为“stalled miss 不占用加速器执行资源、已命中有限时间排空”假设的适用性问题，不声称已发现生产死锁；最终审定合并。
- 下一最小判别规格：见 `translation-lifetime-and-innovation-gate-20261008.md` 的受限模型和检查清单；当前未 run。普通强对照包括配对预译址、共同分段、sequence fence、quiesce、完整 R 尾部预约、每子片独立译址/ready 队列/普通 budget-debt-refund。
- 归属：DeepSeek evidence/doc drafting, Codex source/innovation design/agent/acceptance。
