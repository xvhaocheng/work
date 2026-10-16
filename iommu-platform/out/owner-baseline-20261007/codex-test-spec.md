# 普通owner对照：Codex预先定义（2026-10-07）

## 本轮要判别什么

C尚未实现，没人做过/效果差均不能断言。本轮测一个实际存在的普通强对照能否消除已有退化及保留N作用；结果不能称C收益或C/普通完整等价。

普通对照：在真实DMA中保留4KiB缓冲，但测试侧不把完整缓冲证书作为TC非owner源读的提前准入许可。原banked read_owner/restrict_reads/bank_read_admit继续决定源读选择；AW-first、所有已选AXI/TL事务、FIFO/owner规则不改变。它是已有读guard的保守配置，非新算法。别把输入W owner当physical mux owner。

## 为什么不在单槽completion代理里再加owner=0额度

单槽代理若已经抓住非owner读，再等owner释放，有可能挡住排在后面的owner源读，产生测试侧HOL甚至环。准入应发生在TC选择读之前，而不是撤回已声明VALID。直接复用banked guard、掩掉test harness输入的fullbuffer许可避免新造这些逻辑。现有completion代理保持buffer模式用于相同接口/容量对照。

## 实际接口／范围

仅tests：
- nonblock_actual_dma_top.sv增加默认0的owner_only_i，送入nonblock_banked_top的fullbuffer_jobs_i为真实certificate AND !owner_only_i；真实cert输出与实际FIFO容量保持。
- nonblock_actual_dma_system_top.sv同input，传递i_dma，不改DDR/DS/代理mode。
- C++ +owner-only=0|1；warm/reset期间drive0，WARM-SUMMARY后测量阶段才启用，STOP A和最终排空期间保持启用。scenario重用时先清measurement_phase。JSON明确记录owner_only。
无需新增owner记录器、prepared位或oracle未来标签。默认0必须重现旧模型。只在同策略及同工作集的P/PN之间比较，不把PN关闭N效应当创新。

## 最小运行范围与验收

一次Verilator policy/system测试构建，独立out目录不覆盖上一模型；不是BIT构建。预计6次短运行：
1. owner_only=0、hot PN校准93487；
2. owner_only=0、cold PN校准94427；
3/4. owner_only=1、hot P/PN；
5/6. owner_only=1、cold P/PN。
各A8、V64、4KiB、gap0、warm256、模型31、read-admit=buffer、DS priority=0、同原DS50/50。
已有hot P91874、cold P92334从原结果引用，若默认校准精确再复用；不重跑全部矩阵。每个新模式P/PN各有同资源/同许可对照。
必须原source/destination字节验证、TL/DMA/CPU quiet、memory queues empty、代理captured==issued/empty/debt0；V/A实际完成量、窗口周期、FAST和PTW统计/事件必须真实记录。事件区分input permission与真实buffer certificate，不把缓存命中推出目的已准备。
失败先诊断，不跳过case、不扩大矩阵。若证据需要仅定向额外run。

## 预期和判定（先于运行）

- 若退化消失，但FAST几乎消失、PN≈P：普通规则是可用工程缓解；C尚无独立增量。不能把“禁掉大部分N”当新并发方法。
- 若保留明显FAST且双方收益：更强普通基线，C必须超过它/成本优于它，不能把该规则自身宣布创新。
- 若性能变差/卡住：直接核TC选择/owner/certificate与实际错误，不能认定C获胜。
- 下一创新问题需具体合法差异/成本，而非仅P/PN正负；若普通配置解释当前机会则降低C主贡献主张。不能穷尽世界前作、宣称完整等价或外推板端MIG。

DeepSeek整理接口和起草单文件C++；Codex亲自定义普通对照、检查准入位置/HOL风险、测试侧SV接口、实际运行和最终研究判断。

## 六组之后追加的定向对照（先于追加运行）

六组揭示cold owner-only的源页PTE0x50000在目的PTE0x50010之后额外读取，V fill从1变2；因此需要比较一种限流位置在翻译之后的已有普通规则。只追加cold P/PN +read-admit=walk、budget256、owner_only0；+read-admit-measure-only=1使warm仍为buffer，measure/STOP/drain才使用原walk额度。先检查warm摘要与原cold buffer同mode逐字一致；P没有FAST限制预期精确92334。无需SV再构建，只CPP重编链接；不重跑六组。比较是否保留源翻译访问顺序/只有1 V fill并改善双方完成速度；这是旧普通walk规则的强对照，不是C实现或创新。
