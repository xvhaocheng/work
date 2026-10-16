# 后续确需独占席时的新主会话交接

当前root已完成stage112关键核查，不需要为补例行复审换主会话。本文件只在后续确有独占关键问题且原主会话无法释放席位时使用；不自动创建会话或研究goal。

- 工作区/home/xvhaocheng/work；先读适用AGENTS和CURRENT_STATUS；全Codex，Sol gpt-6.1-sol/ultra，fork_turns=none提供独占任务必要上下文，不调用DeepSeek/Claude，不降强度，不改模型/上下文配置。
- 整体阶段一立题，方法论文未完成。R107有限恢复归档：普通8个真实2MiB叶，恢复1728hit/0walk，全字节/DONE/quiet/checked release已接受；不重跑stage106/108或身份审计。
- stage112亲核固定vLLM67d96c29的RowParallel/GraphAllReduce：TP2每层两个归并位置，持久GPU buffer；真实消费者不是原FPGA CPU检查代理。当前C112 FPGA翻译缺口投影归档，未进入G0/G1，未实现/测新的collective。
- 只有新的自然独立搬运/实际共享路径、完整owner/lifetime/bytes/ready/cost和普通大页/premap/residency/batch/合法singleDMA未覆盖事实成立，才进入最小G0；不恢复关闭场景或人为增加容量/设备隔离/时序压力。
- 三旧席历史任务完成、12关键产物保留；当前pending_init原因未知。本接口无close能力，interrupt不是close，archive未证明释放。零线程关闭/释放实证；真需新席时在新主会话明确指定Sol/ultra，再直接核实际执行证据，不仅看创建参数。
- 原board最后已验r1boot370a9dd6-7d1a-4190-8395-acaf3e25f69c/kernel6.5.0-rc2-r107-ordinary-stage111-r1/f9bit/S0；本轮没有访问板，不把这个旧状态说成当前新读取。不得自动重启/刷写或开始新任务。

导航：本目录root-result-acceptance.json、next-goal-contract.json、source/finite-consumer-byte-contract.json、critical/root-source-review.json、critical/existing-raw-evidence.json、validation/agent-lifecycle-review.json及../../docs/post-r107-demand-gate-20261013.md。完整旧记录原路径保留；摘要不能代替关键原文/原数据。
