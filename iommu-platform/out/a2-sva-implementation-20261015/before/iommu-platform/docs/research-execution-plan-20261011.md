# A控制位置方法的局部执行计划

2026年10月15日。A2已定位公开enabled HeroSDK并绑定当前真实接口，交付最小SVA重设计；完整任务时间仍UNKNOWN。

1. 继承A1方法/四案八次源语义及最近五天成果，不回放旧实验。新HeroSDK与旧关闭SVM路径分开。
2. 固定论文关联构建；修末DMA→DONE、独立consumer及map/unmap/unpin。准确实验快照未证，不能用源码候选冒充复现。
3. 以当前默认24B/单TC/每lane一normal job为合同，优先实现固定pinned单mm/direct-FSC/noPASID；同VA独立shadow及普通IOVA作对照。实际域/mm生命周期、cache/CQ完成和异常清理先过。
4. 使用[已物化输入](../out/a2-native-sva-binding-20261015/design/native-task-input.json)：三包四span、8页、1639B载荷/1675B文件consumer，公开DONE串行起步；CPU/原source直接writev合法。现FENCE.I缓存能力复用，费用不填旧模型常数。
5. 来源/服务/全成本及自然可选头闭合后，才冻结1正1反2时间留出、预测与失败规则并执行。当前无新时间后端/时间案例运行，不改压力、phase、buffer或许可找胜例。
6. 完整预测和贡献通过才准入后续RTL/FPGA；MIG观测1792023314继续paused，原时序失败/0新采集/债务unknown保持。

[本轮报告](a2-native-sva-binding-20261015.md)／[root最小方案](../out/a2-native-sva-binding-20261015/design/minimal-sva-adaptation.md)／[稳定接受](../out/a2-native-sva-binding-20261015/root-result-acceptance.json)／[实施合同](../out/a2-native-sva-binding-20261015/next-decision-contract.json)／[原方法/语义](control-position-method-20261015.md)。两周10月15—28日不重置，论文未完成，Codex-only；本轮无生产修改或native/RTL/板运行。
