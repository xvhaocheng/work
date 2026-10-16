# A4：AXPY真实动作与控制位置决定

2026年10月15日。**HERO双缓冲AXPY有真实L1消费者，也有合法的等待范围选择；现成按ID完成接口已能表达这个动作。本轮无需新增Linux或RTL。** 实际窗口、完整消费者净收益及共享多DMA价值仍未证明。A保留候选，不恢复跨方向默认优先级。

定向补核了A3缺少的物理回执链：`dmcpy`返回表示descriptor已接受；固定FIFO→reshaper→AW→B链在同ID有序返回下构成累计完成前缀。完成依赖最后写分段B退休，但不检查R/B错误，也不保证host cache/失效或可free。[原始证据、固定ref与15份哈希](../out/a4-axpy-control-decision-20261015/source/receipt.md)

root把普通候选加强为 **O-T1：最后tile仅保存、等待后发输入RY的一个32位ID**；其前缀已覆盖RX。原四个2048B槽不变，保留最后wait-all/fence/barrier、DONE、cache交接、独立consumer与全部释放。一般循环另给O-G按ID版本：下一RX可先发布，复用Y前等旧输出ID；两个逻辑ID就能表达。均未应用、编译或运行。

这次位置判断的要点是：**旧输出接受后不能再由源端重排，但固件仍可释放另一bank的计算。** IOMMU入口优先无法直接删除wait-all依赖；它仍可能加快服务，不能据此声称全部晚策略无效。当前用途优先、源端节流/串行、普通ID、原buffer、CPU FMA、适用shadow/cache与DS-first均保留同信息同资源机会。

完整推导同时跟踪最后输入、旧/新输出、计算、设备DONE、host消费和资源归还。还发现固定runtime的cfg bit2在RTL被截断，实际serialize=0；不能从B有序推出整个job串行，也不能把“旧B后再加固定DMA时间”当原生预测。完整服务和并发费用未闭合，因此时间保持UNKNOWN；没有冻结或执行四个时间正反/留出案例。[具体任务、算法边界、强对照、预测及失败判据](../out/a4-axpy-control-decision-20261015/method/design.md)

本例支持已有接口的动作能力，尚不支持新的共享IOMMU硬件机制或独立分析贡献：固定Snitch仅一个DMA，真实第二publisher缺失；源码也没有排除CPU实现的业务预算。标准依赖/状态分析获得同一信息后能表达这些关系。A可继续发展为可复用的接口摘要与部署判别，但需未见任务和有用的新决定。[条件式新意与准确近邻](../out/a4-axpy-control-decision-20261015/method/design.md#8-最近前作与方法增量)

决定：保留AXPY为普通控制校准任务，停止“此动作必须新增早期控制接口”的具体版本。最少后续证据是固定端点的成功/服务合同、默认自然运行的最后两tile窗口及完整费用；若研究共享主张，还要真实第二publisher或实际设备/CPU约束。未取得这些事实前，不修外围软件寻找胜例。Hero三个硬件后端缺实现、默认拒绝pin的状态不变。[下一决定合同](../out/a4-axpy-control-decision-20261015/next-decision-contract.json)

本轮仅取源、核源、设计和文档核验；0编译、仿真、符号模型重跑、原生DMA、FPGA或板操作，未改生产代码。原MIG继续paused，build6 WNS−0.391ns/598setup失败、0新加载采集、实际债务unknown；A2产物和旧实验保留。Codex-only，root设计/关键核源/最终接受，独占Sol席补固定DMA回执，无DeepSeek/Claude。

[稳定接受](../out/a4-axpy-control-decision-20261015/root-result-acceptance.json)／[完整导航](../out/a4-axpy-control-decision-20261015/README.md)／[交付核验](../out/a4-axpy-control-decision-20261015/root/delivery-verification.json)。
