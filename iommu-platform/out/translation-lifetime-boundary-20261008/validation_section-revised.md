## §40 验证边界（2026-10-08）

- 本轮为源码可达性门槛与正式文档，不是新 DUT、板测或通用正确性证明。本阶段 Codex 已执行只读源码命令并写入证据/草案文件；没有 DUT build/run/BIT/board，没有改 production RTL/Linux/test，没有写 SD。
- 新增 DUT build=0、run=0、BIT build=0、board run=0；前阶段数值只引用，不在本节展开。没有把文件哈希当成硬件正确性测试。
- §39 静态 DUT 把 CQ 请求、IOTINVAL、IODIRINVAL 固定为零，因此其 6 项 PASS 不验证 live invalidation、不验证 IOFENCE PR/PW 排空，也不验证 CQH 早推进与 generic completion 差异。
- CQ fetch 仍可能通过 DS/内存通路受互连竞争影响，这不是 LTI-style 寿命反馈；CQH 早推进不等于 fence 已执行或前序命令已 commit。
- 普通 fault/error+discard 是强进度控制；保持 copy 成功是性能额外合同，不混安全或成功工作量。当前 fault 排空未验。
- 第二席 map 已完成；ShareMMU §V-C7 只按根审定写为 stalled miss 假设适用性问题，不称生产死锁。两席 artifact 与根审定、source-evidence 均保留在 `../out/translation-lifetime-boundary-20261008/`。
- 下一最小判别规格见 [translation-lifetime-and-innovation-gate-20261008.md](translation-lifetime-and-innovation-gate-20261008.md)；当前未 run，不假造完成实验/新算法/锐界证明。
- 归属：DeepSeek 整理证据和起草文档；Codex 根与 2 研究席作创新推演、关键源/原文核查、最终审定；未使用 Claude。
