# C117R3 逐包交接可达性：证据导航

稳定裁决：需要具名有界软件适配。来源有及时完成回收/发布需求，硬件有并发与精确copy原语，但当前公开接口缺完整包CPU交接/消费/归还合同。不是硬件不可能，不要求新增RTL。本轮无新性能实验或G0合同，论文未完成。

- [中文报告](../../docs/stage117-packet-handoff-gate-20261013.md)／[root稳定裁决](root-result-acceptance.json)／[本轮合同](task-contract.json)／[实际goal登记](root/goal-registration.json)。
- [root来源裁决](source/result.json)／[13组DPDK原文和四文件哈希](source/evidence.json)：完整逐行片段分别S01–S13.txt，及时poll、完整包join、前缀发布、A/B归还区别。
- [平台裁决](platform/result.json)／[36组平台原文与哈希](platform/evidence.json)／[接受理由、边界和未解项](platform/review.txt)／[平台产物一致性](platform/artifact-check.json)。
- [16B静态缓存行计算](platform/cacheline-layout.json)：原布局跨group行不相交，同包header/payload共享行；未修改输入，不是新实验。
- [具名软件适配范围及成本取舍](adaptation-scope-card.json)：future scope，未实现；不将软件基础能力作为论文创新，不前移全生产恢复要求到所有G0。
- [实际模型/窗口观察](root/model-runtime-observation.json)：root Sol/ultra有效475000，新平台席Astra/ultra有效258400；未切换或伪改模型历史、无手动上下文覆盖、速度倍数未验实。
- [五份执行前文档](before/manifest.json)／[执行前工作树](root/worktree-before.json)／[47个既有修改文件哈希](root/dirty-files-before.json)／[旧接受资产入口哈希](root/referenced-prior-assets.json)。保留独立贡献与关闭范围。
- [root交付核验](root/delivery-verification.json)／[核验实现](root/verify_delivery.py)：246项通过，覆盖13组DPDK片段、36组平台原文、原布局算术、既有资产/历史及69个链接；没有运行旧校验器或性能实验。

上一轮single1161/parallel1601模型周期只裁决原有限phase完成向量；本轮不重跑，不据其440差额宣称逐包性能或翻译独占因果。当前/历史源码及已部署身份分别记录；f9原top配置与tracked不同，本轮没有板探测。

本轮已在root稳定接受、全部计划/状态/交接/验证同步与246项交付检查通过后，由实际update_goal返回complete；工具记录308,316 tokens／29分13秒，无额外token预算，论文未完成。[原始完成回执](root/goal-completion.json)／[完成同步检查](root/completion-sync-check.json)／[执行账本](root/execution-ledger.json)。
