# 本轮机制卡：先判残余反馈，再判自然初态

真实问题：翻译先后所释放的payload，是否改变同一PTE的相对生成／AR准入／返回／消费等待，使两顺序预测获得一个完整普通在线规则之外的收益。

沿用原候选动作：只在两个原生合法安全头尚未merged首次VALID承诺时选A先或B先；单头立即处理，完整walk／NativeN／同ID／held／所有R/W/B/fault义务保持。可观察输入限当前owned义务和已完成历史；future PTE、未接受请求和隐藏MIG状态禁止。

本轮先用源码排除条件，未假设已有fresh capture。四路径证明见[four-path-proof.md](four-path-proof.md)，普通控制在结果前固定为原生NormalTL RR＋冻结O-DS-AR currentVALID/fairness4＋完整DS返回优先。其零新增注册选择周期是抽象合同而非硬件验收；完整费用边界见[cost-and-ownership.json](cost-and-ownership.json)。九个其他普通规则保留，未拿事后best-of作在线对照。

新增来源事实：有限域QDDR≤6<16；从空复位归纳shared AR无payload反压；正常S1 slow owner使DDTW／PTW完整义务串行。旧DDTW可造成共同R背压，自有512容量不能把它删除；最后DS消费后，四段payload Rspill的保守恢复界8拍小于新DS输入准备窗口52。于是该条件服务／普通控制下同PTE相对等待不随payload先后变化。

本域反馈正例被源码排除；相邻反例是违反一个显式前提的条件状态（例如任意预置queue满／旧held／更多DS），没有正常来源证据，不能运行或当收益。本轮没有选择差异、A/B、预测器或四性能条件。该因果主张在限定域停止，F1全族／物理DDR服务及费用继续开放；不恢复任何关闭分支。

后继只接新原始事实指出哪一个排除前提失效并给出普通规则外完整因果链；通过后才核一个新固定正常来源，再一对A/B及最多四条件，随后GPU／AI实际共享翻译路径与FPGA。无新事实不复跑当前前缀、扫相位或自动扩题；原立题窗口累计，论文未完成。
