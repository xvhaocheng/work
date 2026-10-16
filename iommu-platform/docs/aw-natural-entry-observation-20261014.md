# 自然入口：一次新AW／配置观测（2026-10-14）

**原AW前驱与B两次预留／A零次预留自然出现，但完整L/P选择入口未出现。**这一个冻结的冷、sole-copy供给停止；没有性能对比，也不能由此否定future-AW／F1／完整准入全族。论文未完成。

actualgoal1791965564，无额外token预算。用户在规划中选择“允许有界新观测”，随后要求执行；本轮按新合同扩大原source-only范围，仅进行一个隔离被动观测。actual root gpt-6.1-sol／ultra，native有效475000，0新席／旧席恢复，没有DeepSeek或Claude。

## 问题、进入条件和边界

问题仍是原候选入口：B当前job已有两次output AW reservation，A当前job尚无reservation；B有合法、未承诺新下游VALID的物理载荷头L，A有存活的读PTW P，所有身份与原许可完整。reservation不是AW握手，也不是FIFO当前usage。

原可追溯入口来自留存sole-copy程序：两路真实dma_core_wrap各一个DEFAULT对齐4096B，512beat私有缓冲，真实四输入AW/AR mux（两个live源），现有banked CTL／NormalTL／N／onePTW／16项IoTLB。两路先配置，再按原source0→source1串行NEXT触发，最终匹配原DONE并quiet。无warmup、batch、CPU流量、STOP、新后继；没有改变READY、仲裁、相位、延迟、容量、walker或请求供给。

依赖沿用原runner的混合但唯一vendor选择，82份本次构建输入保存SHA。它是约减的RTL集成模型，不是整块FPGA：cfg访问绕开真实CPU，缺MIG、DDR时钟／宽度转换和缓存。原DS50/50、行为DDR head-only准备31、queue16以及原DSpriority生命周期均保留。因此只证明声明模型内的事实，不能代替板端或GPU工作流证据；不把其他源码副本的额度直接搬来解释它。

本轮先冻结20000个观察样本上限、原完成／quiet终止条件和一次run额度。加入的555个接口均为现有信号的组合读取；删除这些输出、记录语句和诊断上限后，top与供给／DDR程序分别逐字还原原件。原编译依赖指纹一致。仅一次新隔离编译及一次新原始采集，未运行旧矩阵、旧validator或旧trace。

## 新原始事实与入口裁决

时间标签：每行c是边沿c之前的稳定样本，VALID&&READY在该边沿握手，结果进入c+1状态；不要将同拍新预留算进边沿前状态。

| 事件 | 实际采样拍 |
| --- | --- |
| source0／source1前端接受唯一job | 65／70 |
| source0／source1后端接受job及opt | 66／71 |
| source0两AW在DMA源侧接受 | 67、68 |
| source1两AW在DMA源侧接受 | 72、73 |
| source0两AW在IOMMU input接受 | 68、69 |
| source1两AW在IOMMU input接受 | 3242、3243 |
| source0两次output AW预留 | 1030、1556 |
| source1两次output AW预留 | 3686、3693 |

两份**实际accepted opt**都是decouple_rw0／decouple_aw0／src_reduce_len0／dst_reduce_len0；DEFAULT由独立ttype0核验，不由FullBuffer猜配置。每源只有一个job，原stream DID10/11，source AXI ID均0，IOMMU完整ID0/4、DDR payload完整ID32/36；substream有效位0，未用bank标签冒充任务依赖。

新记录直接验证旧端点修正：source1第二AR在源侧第73拍已接受，早于该源首AW的input接受3242，也早于source0首raw WLAST2954。原AW spill峰值确为两项。coupled配置并不推出“第二AR必须等后一级input AW／另一任务raw WLAST”。

source0是B、source1是A时，B两次／A零次且两job仍存活共1695拍（1557–3251）。进一步要求A当前读P存活，只剩429拍（1557–1985，DID11、IOVA0x20000000）；**这429拍中B物理AR头L全为无效。**另一角色方向也没有满足入口。这里仅检查一个完整入口的必要前提；必要前提为0足以拒绝本次准入，没有把不完整前提为真误称完整guard为真。

B第一读翻译头在1557出现，1558–1559的原N查询有cached context而IOTLB miss，没有接受；没有用额外warmup修复。B当前源读仍需其完整Normal walk（1992、2137、2282的三个实际PTE），最后PTE在2426消费，B首次物理AR头在2430才出现。A此前读walk最后PTE在1985已经消费。因此不能在这一固定前缀比较“先释放B载荷L”与“先服务A读P”。

这不是全局不可达定理，也不是完整任务普通覆盖／创新否决。接受的是新原顺序、选项及端点见证；停止的是这一固定供给的入口尝试。旧R1/R2/R107/C112及其他关闭结论不恢复。

## 完整工作、错误与费用

新记录4151行、618列、6713165字节，覆盖62–4212拍。按source、input、TC target、bank physical output逐笔核同ID AR→R、LEN／RLAST、AW→ordered W／WSTRB／WLAST以及独立B，不把W-last当job完成。各源各2AR／2AW、512R／512W、实际WSTRB4096B、2B，响应均正常；两个目的4096B逐字节校验通过。共有12个实际PTE请求与12次消费、四份三层walk；没有候选干预。

两job的后端完成与各自最后B分别同在3251、4206拍。以共同就绪62拍为起点，含完成握手拍的硬件完成为3190／4145周期；首共同硬件quiet在4207，即4145周期。原DONE发现为3196／4150，原返回quiet为4151；后6拍仍计入原供给／轮询模型，不当作优化收益。

第一次新记录分析错误地要求idma_rsp.last为1；亲核backend／legalizer／wrapper后纠正：它实际是super_last，本两job均为0；真正rsp_valid由w_last_burst筛出，wrapper以rsp_valid完成。新核验使用独立最后B与真实后端完成，并保留super_last0原值。内部last-burst FIFO未直接导出，不宣称取得其完整内容；只修分析，没有重采。

观察成本：新增simulation tap宽度之和4874bit（含别名／重复字段），不是最小硬件存储或面积；SV观察逻辑无新增寄存器／队列／控制延迟，未实现生产反馈端口。另有host64bit上限计数器和6.7MB原始journal。实际一次编译6.586秒、instrumented host采集0.164秒；后者不是无探针性能或原任务运行开销。完整在线observer／关联／history／预测器／选择／fairness／CDC成本、面积、Fmax、功耗、净收益及应用收益继续留null。

新原始记录核验通过，包括held VALID完整payload、实际字节／事务／独立B／job关联／quiet。旧普通规则冻结文件只引用及核指纹，定义未改，没有挑各场景赢家拼普通对照；本轮没有候选性能delta、反事实模型或四条件。

## 后继合同与交付

当前一次采集额度已用完，不再调相位、扩大压力、补预热、换映射或采第二次；microbenchmark不准入。后继只接受**其他已存在原供给自身产生的B源读/context历史及完整存活L/P入口**，或者明确新增合法能力的独立合同。没有这种原始来源，就冻结当前候选，不能继续扩写相同纸面因果图。

E1的原accepted选项和AW前驱在本供给中得到原始事实，完整入口明确未见；E4本两sole jobs完整完成已核。E2仅本记录的实际walk/service，E3仅基线R/W/B，E5完整在线费用和E6候选完整比较仍未闭合。方法探索→有界microbenchmark→GPU/AI工作流→FPGA路线保留，8/9月成果继续复用，立题窗口累计。

完整导航：[README](../out/aw-natural-entry-observation-20261014/README.md)。原5页保存在before；root稳定裁决、原始记录、来源／反向还原证明、两次分析的纠正、费用、账本和下一合同均留存。五页同步及新交付检查通过后，才调用实际goal complete；状态及用量以原样工具回执为准，本轮不表示论文完成。

实际goal已complete；工具回执191168 tokens、30分17秒，无额外预算。五页同步和192项新交付检查在complete之前通过，之后仅写实际回执／记账及完成注记，未重跑build、model、旧trace或源码／新原始解析。论文未完成。
