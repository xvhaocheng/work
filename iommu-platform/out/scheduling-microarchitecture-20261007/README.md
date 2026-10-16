# 调度与微架构探索（2026-10-07，非实验）

先读 [Codex 审定](codex-review.md)，再看 [Codex 独立设计稿](codex-proposal.md)。设计稿写成后才读本轮 DeepSeek 回答，原先 D 优先级已在审定中下调；最终仅保留 C 为优先核查候选，D 作配套/普通对照。未确认创新或收益。

- [原源定位](source-audit.md)：Codex 直接核关键论文/官方手册；ABW仅摘要，NB-Walker全文仍缺。
- [DeepSeek 前作答复](prior-answer.md)／[原JSON](prior-raw.json)／[任务](prior-task.txt)。含未采纳反例，使用时看审定。
- [DeepSeek 接口规则答复](rules-answer.md)／[原JSON](rules-raw.json)／[任务](rules-task.txt)。五规则未经执行，有账本/依赖/状态错误，不能直接实现。
- [调用与范围审计](audit.json)。两调用 thinking/high、393216、stop、未截断，共413463 provider tokens；均在用户最新强调Codex亲自创新分析之前发起，没有追加设计委派。

Codex 是本轮独立设计、关键机制分析和审定作者；DeepSeek 提供补充提案/证据。工作仅阅读、检索、讨论、写记录：生产RTL/Linux/BIT未改，无构建、新仿真、实板、JTAG、SD或网络更新操作。此前B的21/21结果仍是此前实验，不是本轮方案结果。
