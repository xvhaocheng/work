# TL/PTW共享服务反馈：本轮实例止步于进入门

2026-10-14。执行了用户批准的机制探索入口。root交付两顺序预测的正向设计，独立来源席与普通控制席分别核证。结论是：**最小对称读写切换实例不准入四条件microbenchmark**。没有关闭共享服务反馈的整个方向，也没有确认论文创新。

## 新事实和设计

F1在NormalTL第一次VALID之前比较两个bank各自合法队头，按当前owned义务的预测最晚完成、完成时间之和、年龄及RR选择；缺有效历史回退RR，持续等待头被越过一次后必须服务。保留一个PTW、16项IoTLB、全walk执行、现有容量、NativeN命中旁路及原有W/ID/退休义务。[root机制卡](../out/tl-memory-feedback-20261014/root/mechanism-card.md)给出完整设计与成本边界。

源码允许一个条件双头前缀：t0同沿接受AR0、AW1和bank1单beat WLAST；写源余额净0，t1分别选择READ/WRITE，t2在两头首次合流VALID前选择。仅AR0/AW1同沿而W未供齐不够；两个新AR不能在一个AR通道同沿接受。这是接口源码推导，不是实际iDMA轨迹或板测。Native合流是eager VALID，PTW后来空闲不允许重选；已接受spill也不能重新排序。

NormalTL的118bit头没有完整AXI ID、LEN/SIZE和最终R/B欠账。两个TL tag只覆盖翻译生命周期；bank在completion地址内部接受后可空闲，数据/B仍有所有权。新增预测需要有费用的头元数据、完整owned义务、历史、读写端口、选择逻辑与延迟，不能拿旧observer/诊断记录当免费在线wire。来源原文及配置区分见[来源证据](../out/tl-memory-feedback-20261014/source-prior/source-evidence.md)。

## 为什么当前实例不进入性能验证

现有私有DDR驱动把READ/PTE放在同一个mem_q中，可在保持同ID与held R的条件下优先DS；AW/W/B由独立caw_q/cb_q服务。它没有统一R/W排他服务器、方向状态或切换惩罚tau。共用DDR端口不能证明这个额外服务规则；加入tau后运行只能测人为增加的规则，不能验证当前路径。

独立条件手算仍产生有用边界：A一PTE加一WRITE beat，B两PTE（依赖空隙1）加两READ beat，假设统一服务器、准确历史和对称切换tau。两顺序完成向量是A→B `(3+tau,6+2tau)`，B→A `(7+tau,6)`。tau=2时最晚完成10对9，tau=0时6对7；单位是数学服务单位，不是RTL周期或实测。tau=1最晚都8，但sum12对14，仍选A。

一个提前冻结的完整在线O-turn规则——历史tau大于PTE依赖空隙时读优先，否则短翻译——覆盖这个固定对称族及相同held/公平/缺历史guard。它不计算两份完整轨迹。这个具体选择见证因此被普通控制解释；没有证明真实面积优劣、全部joint算法等价或非对称服务族等价。合法源码前缀和这个数学pair也是两份不同证据，不能拼成一次原生运行。[手算及严格覆盖域](../out/tl-memory-feedback-20261014/ordinary-cost/hand-witness.md)保留完整时间线。

## 强对照、前作和完整成本

冻结九项完整在线普通规则：RR、FCFS/年龄、owner/阻塞路径、短翻译、少欠账、多欠账、释放密度、O-turn及有严格适用域的两阶段排序。各规则同信息/原生容量；可删除未用观测逻辑，不强加预测器来人为等成本。不同场景事后最优的包络不是一个在线规则。

亲读MASK与ISCA2018原作者正文：前者覆盖PTE优先及公平反馈，后者覆盖翻译工作量估计、同指令聚批和年龄保护。没有获得它们整体覆盖F1的证据，也不能据两篇没有该组合就确认创新。Johnson只获取出版商原始摘要；固定独立两阶段公式是独立推导，不能外推到共享服务反馈。[前作边界](../out/tl-memory-feedback-20261014/source-prior/prior-art.md)保留正文页码、URL和原PDF。

37项成本分类已列完整公共继承、新观察、owned元数据、历史/有效性、两个预测、运算/端口/扫描/选择延迟及软件费用。参数宽度和实现未冻结，所以精确总FF、面积、Fmax、能耗及应用成本保持null；不是完整数字成本已闭合，也不借R2的1329bit当新预测器总成本。[完整接口成本卡](../out/tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md)给出公式和寿命。

## 实际执行与后继

26份定向源码、65段原文与SHA，两份原作者PDF、九项算法卡、root机制卡和稳定裁决已交付。0新性能运行、0microbenchmark、0RTL/build/综合/板操作、0旧实验或validator重跑，无生产改动。四条件执行依赖门未通过，全部未运行；没有为得到正结果添加压力或服务惩罚。

root和两个新独占席实际均gpt-6.1-sol/ultra，运行记录有效上下文475000；forknone，无外部模型、旧席恢复或上下文覆盖。未由有效窗口推断未直接观察的原生nominal500000。首轮未超过两工作日，原立题窗口累计。

唯一后继入口是现有R/PTE共队、W独立服务和真实准入/held VALID之下的**新、合法、可观察反馈见证**，并说明哪个完整普通规则未覆盖；不自动扩大tau矩阵、恢复关闭版本或安排FPGA。存活机制后才选一个GPU/AI优先工作流核实际共享翻译路径。R2及8/9月以来全部资产保留；论文未完成。

根裁决：[root-result-acceptance.json](../out/tl-memory-feedback-20261014/root-result-acceptance.json)。后继范围：[next-goal-contract.json](../out/tl-memory-feedback-20261014/next-goal-contract.json)。实际goal先登记、无额外预算，在五文档同步与交付核验后才结束；最终状态/用量见[root/goal-state.json](../out/tl-memory-feedback-20261014/root/goal-state.json)，完整回执见[root/goal-completion.json](../out/tl-memory-feedback-20261014/root/goal-completion.json)。
