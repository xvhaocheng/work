# F1：共享服务两顺序预测（root正向设计）

2026-10-14。此卡是新的有界机制探索，不恢复R2、§79B或其他关闭版本。唯一动作是在两个合法bank队头尚未被NormalTL首次VALID承诺时选择其先后。不是增加walker、返回池、PTE暂停/续走或改变W所有权。

## 可推翻主张

若先完成某个翻译所释放的已拥有payload，确实改变后续PTE的服务等待，而公开的当前欠账及完成历史足以预测这一差别，则两顺序的完整义务预测可能比固定owner、短walk和便宜欠账规则给出更好的完整完成向量。若廉价的一个完整在线规则复现这项选择/成本，或正例依赖源路径没有的服务动作，则该实例不能进入四条件microbenchmark。

## 执行规则与接口

1. 输入只读每bank现有合法队头及当前owned obligations。NormalTL原118bit请求不是全部元数据：full AXI ID、len/size、AR/AW债务及最终R/B退休须由原头/原追踪表提供有费用的tap，或由完整有界observer重建；不能用2个TL生命周期tag替代所有数据债务。
2. 任一head首次VALID已承诺时沿用native lock，不能因PTW后来空闲、反馈变化或另一head后来到达而改选。只有一个可见合法头时立即按native路径处理，不为凑两个头等待；nativeN合法命中能力保留。
3. 两头均尚未承诺且历史有效时，预测A→B/B→A。每个预测逐次推进依赖PTE和已拥有payload的共享服务；不能仅给walk加一个固定常数再做独立两阶段排序。
4. 评分为(max predicted final completion of current owned obligations, sum of those completions, older head first, RR)。历史不足或所需反馈不能由公开接口获得时回退RR。
5. 对同一个持续等待head，越过一次后下一次可选时必须服务它。强制条件优先于评分；记账依据实际选中且被承诺的head，不在idle探测时推进RR或记一次服务。held首次VALID保持到握手，force记录不能跨head退休误用于下一head。
6. 预测不改变正常翻译/permission/fault判断、AXI firstVALID保证、fullsourceID、同ID顺序、W owner、真实WLAST、B/R最终退休或信用额度。任何选择/历史更新延迟计入共同就绪后的完成时间，不能免费在t0前准备。

## 预测器设计边界

预测模型参数只能来自已完成请求的PTE数量、服务时间和公开事务债务。现有源不暴露MIG行状态、内部方向切换或未来PTE/PA，不能把它们作为真实输入。历史估计可有误差，但必须具名记录它估计的是哪个服务模型；未观察参数不能凭空置为有效。模型内shadow copies只有评估用途，不拥有真实事务或额外服务额度。

本轮最小可手推实例采用PTE与R/W同一不可抢占服务器、方向切换代价tau、DS优先；它是条件预测器实例，并不是由当前behavioral DDR已实现的机制。source和普通席负责独立核此实例的进入门。这个实例被否决不等于所有共享服务联合预测被否决。

## 所有权与完成

bank原始AW/AR头在COMPLETE之前仍owned；NormalTL请求接受仅转移翻译owner，响应仅给许可/错误，completion地址内部spill接受可释放bank控制，不能据此退休全部R/B义务。读计至准确fullID的RLAST；写计至真实W字节/WLAST及对应B，错误走原生error sink/完整终结。普通和候选使用同一有限公平完成条件、相同实际bytes/IDs/context/queues。只测较早的TL response、AW接受或单一路done均不足以验收。

## 成本边界

完整表达式为C_native + C_head_taps + C_owned_obligation_tracking + C_history_and_validity + C_two_order_prediction + C_age_and_one_bypass_guard + C_selection_and_firstVALID_lock + C_control/feedback/clock_wires + C_software_end_to_end。

C_native包括现有IoTLB16/onePTW/DDTC/PDTC、原生FIFO/spill、W/R/B owner/ID/credit、DMA data/metadata、DS/xbar和完整共同完成义务；仅按原图保留一次，不能把R2的1329bit写请求私有编码当作本预测器的成本或面积。新增元数据若复用既有字段仍计read/view接口；shadow若重复存储须单列，不能称payload=0就成本=0。两个16bit年龄及2bit强制bank有效状态仅是一个有界示意，不是完整状态预算，饱和/代际更新成本也需列出。

没有实现/综合或运行测量的prediction状态数、选择周期、面积/Fmax/能耗/软件/应用收益均为null。普通控制若需要相同观察也付同样费用；不能给候选额外队列容量，也不能要求候选击败将候选整体复制后的同一算法。

## 四条件与停止规则

先通过合法源路径、具体选择差异和完整逻辑成本门，才运行反馈正例、同work feedback-off邻反例、冻结身份交换、冻结另一合法读写交错。最多四个新条件；相同压力/容量/任务，不重跑旧M0或旧validator。一个具名普通规则覆盖本轮正例时关闭该实例；来源缺口记为源门未通过，不写成真实FPGA性能失败、发表整体覆盖或全领域不可能。
