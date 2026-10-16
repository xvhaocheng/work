# A控制位置候选的局部研究计划

2026年10月15日。A4限定判断已完成；A是待独立比较候选，不恢复默认优先。完整时间UNKNOWN。

1. 真实任务固定既有HERO双缓冲AXPY默认32768元素/64tiles/四个2048B槽，L1 FMA和host结果consumer全保留。源码单DMA，不冒充当前四iDMA共享；CPU FMA允许，三包文件基线不重跑。
2. 固定DMA接受ID/有序B退休前缀已核。root给出普通O-T1最后tile只等后发RY一个ID（逻辑4B），一般O-G双ID加Y回收guard（逻辑8B）；尾排空与全部cache/owner释放保留。未应用/编译/运行。
3. 已接受义务按具体动作区分：源端重排太晚不等于固件consumer放行也太晚。源端排序/节流/串行/输出优先、原buffer、CPU、独立页表/适用shadow、合法DS-first同信息同预算开放。当前不需新接口。
4. 固定cfg bit2被截断、serialize=0，B有序不作全job串行。完整时间需实际FIFO/AR/R/AW/W/B/TCDM服务和host全费用；输入事件序不当周期，未知保持null。A3符号8项不重跑、不当四时间案。
5. 决定保留校准例，停止该动作必须新增早接口的具体版本；不修外围软件找胜例。将来若继续，先取自然默认尾窗口/真实服务，才决定最后两tile小后端或测量；共享主张先绑定真实第二publisher/实际应用约束，未见抽取/部署价值另验。
6. **本轮不编译、不仿真、不上板、不改Linux/RTL**，不自动补Hero三个硬件gate或恢复MIG/旧实验。A2产物继承，gate仍未实现/default deny；时间正反与两留出尚未冻结。

[A4报告](a4-axpy-control-decision-20261015.md)／[具体方法](../out/a4-axpy-control-decision-20261015/method/design.md)／[稳定接受](../out/a4-axpy-control-decision-20261015/root-result-acceptance.json)／[最小后继](../out/a4-axpy-control-decision-20261015/next-decision-contract.json)／[A3资产](a3-consumer-position-rules-20261015.md)／[A2基线](a2-sva-implementation-20261015.md)。两周10月15—28日与历史时间保留，论文未完成，Codex-only；原MIG1792023314暂停及时序失败/0采集/债务unknown事实不变。
