# §51：根设计与判别，执行前

2026-10-09，执行前。持续研究到08:00 JST。Codex亲自设计RTL和合同；DeepSeek仅起草C++参数、第二次暂停与观察代码。

关键假设：两独立context，一次对齐2KiB正常不可变共享源、各私有目标、同页READ/WRITE授权成功、V始终可服务，A可两次有限暂停。不是所有设备同时停、不是硬周期界。

1. 加普通可变source-read quantum 1..256；q-W仍每流最多一个2拍child。保持每个原raw AW/AR的ID、last、真实child B汇合。
2. 双方同可选keep-q：退出后W继续短片；共享成员两人时source quantum独立于q-W，退出后singleton source quantum回到q。read-long候选和ordinary quantum=256必须允许同决策，不能故意削弱普通。
3. 单成员短AR首次VALID前，必须已经有整段virtual first-offer W coverage；否则暂停关掉未来AW，已accepted R会等尚无授权的未来W，q-R本身也不足以排空。仅阻止新AR；offer之后不能改变。
4. A提前resume仍在V logical RLAST前冻结新AW，避免旧真实长R等待与future A fill的W队头环。首次退出sticky，后续暂停保留job和cursor。

预期区分三层：A逻辑义务清完、共享物理AR还由V消费、最后成员exclusive物理AR排空。原dbg_pause_a_quiet只证明第一层；不能把它当所有DDR/xbar backend_idle。复制两轮暂停不得通过reset伪造。

公平门：如果ordinary quantum256在相同keep-q、授权和成员退出合同得到相同轨迹/成本，那么长共享读部分不能称新算法；不将旧q2基线34%直接转成创新结论。对q=2,3,4,8,16,64,256比较实际physical AR/R、logical R/W、每流完结与pause drain；必要有效场景才运行，不把每个配置包装成统计重复。

后续独立候选重点：多个context成功/失败/迟到翻译前沿与共同长R之间的依赖，而非把普通广播、退组/修复或translate-first重命名。读写成功门、mask冻结、原parent代际及所有公共状态须计成本。
