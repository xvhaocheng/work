# PTE准入的完整R/W/B收益判别（2026-10-14）

**本轮形成稳定有范围停止：完整读写/B终点已纳入推导，条件下最优动作会反转；但这对条件被一个具体闭式完成包络规则统一实现，且实际共享DDR/正常接受前缀与有效历史未绑定，不准入microbenchmark。** actualgoal1791949728，无额外token预算，半工作日界限。F1全族与完整physical-AR准入方向open，全部旧关闭及停止入口保持；论文未完成。

本轮root独自执行gpt-6.1-sol/ultra，实际原生有效475000，无子席/外部模型或上下文覆盖。继承已验gate、held、PTE拥有/共享出口和52拍准备边界，不重新核旧硬件绑定或运行旧实验。

新原始事实是：TC目标demux的physical AR packet/select都有SpillAr=1，bank output mux才是SpillAr=0。L可以已经迁入原spill，而TC正在处理下一头；预测不能拿TC当前metadata当L身份，亦不能仅凭外侧mux无spill断言P必在另bank。保持存活packet、原job/owner关联与端口费用，不增加AR槽。

完整链已经由直接源码闭合到接口层：已接受job→raw目的AW→当前P及全部剩余walk→正常完整翻译/原fault→physical AW与native W授权；真实R逐拍进入原byte buffer→逐拍W数据可用→原input/output W owner与WLAST→真实对应B→backend last-burst metadata→1D frontend完成→原后续COUNT/STOP与quiet。W可以在R-last前发生，W owner推进与B退休不同。当前NO_ERROR_HANDLING的job完成标志不证明RESP或字节正确，后续验证仍必须核实际R/B RESP和数据。

五个普通控制在新手算前固定：currentDS+RR、earlyPause、认证owner、cheapDebt和上轮dualGap。root只澄清dualGap的h需从同一个R-service终点覆盖DS返回/PTW消费/完整remaining walk与dependent payload，阈值不变。没有逐场景挑普通赢家或修改容量。

新的正向推导保留逐拍W availability与原W顺序。只有额外固定有效服务/W顺序/B终结条件成立时，完整两job终点可写成：

`C_A(admit)=max(D,a+E)+z`，`C_A(wait)=max(D+g+b,g+E)+z`。

相同a>g>0、h>a与任务工作量下，读依赖主导D+b<=E应WAIT；原写排空主导D>=a+E应ADMIT。R-only双门限不能直接当完整copy最优。这是root手算的条件方向，**不是新增仿真、两个实际合法prefix或物理DDR收益**：不变读写服务、固定B终结/owner顺序及有效历史尚未由当前来源证实，完整nonleaf walk会使释放时间内生。

同一条件域存在明确的O-envelope-derived：两个max/add score比较，平分按更早B完成选ADMIT。它统一覆盖本手算candidate的选择，故关闭“该条件对必须完整两轨迹rollout才有选择能力”的主张。它是本轮推导的具体实现，不是五个预先冻结性能对照或已发表前作；不能事后加入五规则拼最佳。完整准入全族/独立创新未据此关闭或接受。

成本边界补齐packet/head/job关联、slow owner、remaining R/W/B与W顺序、同端点历史/valid/overflow/epoch及生成工作、端口/同沿事件/一致snapshot、必要CDC、闭式或virtual预测算术/扫描/选择延迟、held/年龄/公平和CPU/原后续供给/STOP/drain。37分类保留，不借R2旧整数。已有paid D/E/a/g/b时闭式选择表达式可按4加法、2max和1最终compare描述；不是LUT/FF/时序测量，通用candidate也可用同一代数。总bits/ports/latency/全部硬件资源、面积/Fmax/energy/CPU/应用/实际字节与完成/净收益全部null。

原V16/A target2^32的source-window工作量问题保留。取当前一对job不能自动删掉先完成后自然发出的新job与CPU/STOP影响；本轮未得到固定完整供给/最终清空证书，也未恢复停止入口执行。下一轮仅接新的同源accepted snapshot、完整owned R/W/B和原供给、共享service/history证据，或明确的新机制修订；完整差异及成本依据成立才另立有限microbenchmark合同。之后最多四条件，再GPU/AI实际共享翻译路径与RTL/FPGA。不自动扩大来源、重复延长同纸面模型或改论文定位。

0新旧实验、模型、bench、build、旧validator/trace回放、RTL/driver/ABI/板、压力/容量/walker/并发/相位/warmup/延迟修改、旧席/关闭分支恢复；新核验只检交付、源码身份和文档保存。五文档及必要新交付核验通过后actualgoal已complete；工具真实回执142,443 tokens、31分42秒，无额外token预算，原立题窗口累计。

[机制与所有权图](../out/pte-admission-full-rwb-20261014/root/mechanism-card.md)／[完整条件推导](../out/pte-admission-full-rwb-20261014/root/streaming-completion-bound.md)／[冻结普通](../out/pte-admission-full-rwb-20261014/root/frozen-controls.json)／[成本](../out/pte-admission-full-rwb-20261014/root/full-cost.json)／[稳定裁决](../out/pte-admission-full-rwb-20261014/root-result-acceptance.json)／[下一门](../out/pte-admission-full-rwb-20261014/next-goal-contract.json)／[核验](../out/pte-admission-full-rwb-20261014/root/delivery-verification.json)／[完成回执](../out/pte-admission-full-rwb-20261014/root/goal-completion.json)。
