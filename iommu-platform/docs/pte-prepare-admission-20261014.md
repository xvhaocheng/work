# PTE准备期读准入：合法动作、普通覆盖与完整任务边界

**本轮执行完成机制裁决：新gate可在PTE已被接受、尚未共享出口接受时暂停新的payload读；PTE等待/填空档版本被具体普通规则覆盖。完整任务max/sum方向保持开放，当前来源缺完整R/W/B正例，因此不准入微benchmark。** 实际goal1791946350，无额外token预算，半工作日界限；原立题窗口累计，论文未完成。

PTE真实所有权来自PTW→DS接口AR握手，现有两项spill可在后续delay尚未READY时持有它。固定input delay50的Idle/Valid/Ready链推导为：该头首次delay输入VALID后52个同域时钟间隔才首次输出VALID；空spill接受后最早53，旧头等待另计。它不是DDR接受或服务期限，counter必须与phase和P所属队头共同使用。本轮只有源码推导，没有新仿真或板测。

root设计gate位于IOMMU physical completion AR与xbar source2之间，只限制新下游firstVALID；完整header由原producer保持，out首次VALID后held至握手。AW/W/R/B、已进cut/atomics/CDC/MIG的工作、原read-admission/fullbuffer、fullID/same-ID和命中旁路保留。P_pending从PTW接受到同一P共享DDR目的AR出口接受；出口在atomics/CDC之前，与MIG接受、PTW消费及copy最后B均不同。普通DS优先selector也是待实现、付费的共同对照，不能把gate-only当免费严格DDR优先。

四个普通在线控制已冻结：当前DS VALID优先+nativeRR、accepted-PTE提前暂停、认证owner/阻塞路径、最近正常历史形成的便宜准备/欠账阈值；无效输入回退，held和原安全优先。root把owner冲突进一步固定为字典序直接释放证书+current fallback，没有按场景拼赢家。

只在固定同一有序R服务、无W/B及其他干扰、P ready与a/b跨策略不变的交换域，g是P准备空档，先放行L对P的延迟差为`max(0,a-g)`。保护P由提前暂停实现，填空档由a<=g单门限实现，关闭的是这些有限贡献主张。它不是发表前作覆盖完整joint预测的证据。

仍有明确的正向开放边界：同一R服务器上，P经h释放已拥有Q时，完整L/Q的max/sum最优为`ADMIT iff min(a,h)<=g`。保持a>g>0，h大于g应WAIT、h小于g应ADMIT；四冻结规则没有一个统一覆盖这对符号条件。cheap双阈值可以覆盖该窄代数域，但不是预先冻结对照，不能事后加入逐场景取最佳。该式**未被接受为当前copy正例**：真实R/W streaming、W owner/真实B/后继job/STOP/drain和h的付费可观察性尚未闭合，不能删掉它们或人为插延迟来落地。

前作本轮定向核MASK §5.2 tokens（限制TLB fill）及§5.4 DRAM translation/data优先与公平组件，未把token当payload准入；未得到本新gate及完整max/sum的全文覆盖或独立创新证明。[MASK原文](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)。

37成本分类与完整所有权继续保留。概念pending+held至少两控制bit不是总FF；现有counter6/phase2可tap不等于免费端口。pending身份/job关联、R/W/B债务、历史生成、端口/同沿更新、max/sum/owner/scalar、选择延迟/held/公平、必要CDC与CPU STOP/drain均计费。数字state/ports/latency、双方完成与净收益、LUT/FF/BRAM/面积/Fmax/energy/CPU/应用全部null，原容量增量0只表示授权范围。

下一步只接受同一正常来源的一包新原始事实：合法承诺前快照、已拥有释放依赖、固定完整工作量、全部R/W/B终点、有效付费历史和冻结普通外差异；完整差异与成本依据成立才另立有限microbenchmark合同。随后最多四条件，再一个GPU/AI真实共享翻译工作流及RTL/FPGA。本轮不自动搜新来源、重开F1停止实例或关闭分支；F1全族及完整新准入家族保持open/unknown。

root与唯一新独占ordinary-bound席均实际gpt-6.1-sol/ultra，原生有效475000、fork none，无上下文覆盖；Codex-only。0新/旧实验、模型、build、旧validator/trace回放、板/生产/bench修改、压力/容量/并发扩张、旧席恢复或DeepSeek/Claude/Astra。必要新交付核验仅核源码摘录/身份、交付边界和五文档保留，非性能或创新证明。五文档及核验通过后actualgoal已complete；真实回执227,188 tokens、33分22秒，无额外token预算。

[机制卡](../out/pte-prepare-admission-20261014/root/mechanism-card.md)／[原始行号与SHA](../out/pte-prepare-admission-20261014/root/source-snapshot.json)／[普通控制和条件推导](../out/pte-prepare-admission-20261014/ordinary-bound/controls-and-bound.md)／[完整成本](../out/pte-prepare-admission-20261014/root/full-cost.json)／[稳定裁决](../out/pte-prepare-admission-20261014/root-result-acceptance.json)／[下一门](../out/pte-prepare-admission-20261014/next-goal-contract.json)／[核验](../out/pte-prepare-admission-20261014/root/delivery-verification.json)／[完成回执](../out/pte-prepare-admission-20261014/root/goal-completion.json)。
