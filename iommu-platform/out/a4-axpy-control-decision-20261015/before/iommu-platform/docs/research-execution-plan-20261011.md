# A控制位置方法的局部执行计划

2026年10月15日。A3先做真实consumer＋判别规则，A2基础继承；完整时间保持UNKNOWN。

1. 复用精确AXPY/runtime/硬件及已读近邻，不重新广查或回放旧实验。AXPY单DMA/L1消费者已绑定，CPU仍可实现算子，多端点共享任务仍缺。
2. 先给已有transaction-ID等待完整机会：root提出最后tile双ID、末wait-all全保留的普通候选，原四槽不增；中间tile需Y回收guard。先核完成ID物理/ordering，后实施编译，当前未执行。
3. 方法W等待范围/owner、L全策略下界、S普通可行、N完整无影响已具体化；8项符号检查通过并与同信息标准依赖参考一致，非时延/留出或新算法证明。
4. 新真实第二publisher/consumer及应用设备位置/CPU预算由源码/需求提供；不人为限制普通CPU、原buffer、页表/cache、DONE/ID节流串行及适用DS-first。
5. 真实链/服务/全费用闭合后才冻结时间正反及两留出。当前无新原生/RTL/构建/FPGA/板操作；不自动补硬件gate或恢复MIG。

[A3报告](a3-consumer-position-rules-20261015.md)／[规则与源码](../out/a3-consumer-position-rules-20261015/README.md)／[稳定接受](../out/a3-consumer-position-rules-20261015/root-result-acceptance.json)／[下一合同](../out/a3-consumer-position-rules-20261015/next-decision-contract.json)／[A2基线](a2-sva-implementation-20261015.md)。两周10月15—28日与历史时间保留，论文未完成，Codex-only；原MIG1792023314暂停及时序失败/0采集事实不变。
