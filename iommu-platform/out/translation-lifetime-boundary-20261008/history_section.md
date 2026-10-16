## §40 翻译寿命边界与创新门槛（2026-10-08）

- 新增 DOC：`translation-lifetime-and-innovation-gate-20261008.md`，整理根 Codex §40 审定、第一席 `innovation_rules_40` 推演、DeepSeek 证据和 source-evidence.json。
- 结论：源码可达性门槛否决当前 LTI-style 寿命反馈；PR/PW TODO 不是创新；软件默认 PR/PW=0；真实 ATS0；CQ fetch 仍可能等待共享内存；CQH 早推进不等 generic completion；§39 静态 6 PASS 不验证 live invalidation。
- 保留窄问题：错位 burst 尾 buffer/最小分片界；ShareMMU §V-C7 仅作为 stalled miss 假设适用性问题；第二席 map agent 仍在收敛，最终审定合并。
- 强普通对照：配对预译址、共同分段、sequence fence、先停整 job/quiesce、完整 R 尾部预约、每子片独立译址/ready 队列/普通 budget-debt-refund；如果普通方案同信息同资源达到同样保证，就不能称独立创新。
- 本轮新增 DUT build=0/run=0/BIT=0/board=0；未改生产 RTL/Linux/原 tests，未写 SD。
- 归属：DeepSeek evidence/doc drafting, Codex source/innovation design/agent/acceptance。
