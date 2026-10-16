# 第一阶段手算：反馈能改变顺序，但这个最小pair被O-turn覆盖

本文件是条件数学推导，未运行实验、旧测试、validator或模型。它不证明原生RTL存在下面的两队头和统一R/W服务器。整个joint kernel不因这个pair被覆盖而关闭；这个pair不能通过差异门。

## 公共输入与冻结服务条件

两个已合法且尚未merged NormalTL首次VALID的bank头。A的历史预估为一笔PTE、孤立slow-TL服务1、一个已拥有WRITE beat；B为两笔PTE、孤立slow-TL服务3、两个已拥有READ beat。A较老，RR初值A；若有合法相关owner信息，owner为A。初始held/fair guard均clear。两者所有payload义务均已由真实接口接收且需该头翻译才能释放；没有未来DMA任务或PTE地址oracle。

条件服务器是统一、不可抢占的memory service，PTE和READ属于R类，每笔PTE和每beat均服务1时间单位。B的第二PTE在第一PTE完成后1单位才就绪；下一walk可在上一个最终PTE完成时开始。DS/PTE在每次服务器空闲选择时优先，已选中的payload及turn setup均不可撤销；同类连续访问无setup，R↔W切换花τ。t=0 memory已处于R，且无未记录carry-in请求。W完成之后B回应立即退休；READ beat到达即有足够现有接收空间并完成对应任务。这些条件只便于手算，不把W完成等同于任意真实AXI B完成。

历史值可来自此前完成事务的统计预测。决策时不能直接读取隐藏MIG direction或队列。下面精确手算假定预测命中；预测准确性、warm成本、history更新和观测硬件都是待验成本。

## 正例：τ=2

| 顺序 | 区间 | 服务/事件 |
|---|---:|---|
| A→B | 0–1 | PTE A；A的W义务释放 |
| | 1–2 | PTE B1，优先于已就绪A W |
| | 2–4 | 已选择A W的R→W setup；B2在3就绪但不能抢占 |
| | 4–5 | A W，A在5完成 |
| | 5–7 | W→R setup |
| | 7–8 | PTE B2；B READ义务释放 |
| | 8–10 | B的两个READ beat，B在10完成 |
| B→A | 0–1 | PTE B1 |
| | 1–2 | B依赖空隙；没有已翻译payload可填 |
| | 2–3 | PTE B2；B READ义务释放 |
| | 3–4 | PTE A，DS优先 |
| | 4–6 | B READ，B在6完成 |
| | 6–8 | R→W setup |
| | 8–9 | A W，A在9完成 |

从共同ready起点，A→B的完整完成向量(C_A,C_B)=(5,10)，rank=(10,15)；B→A为(9,6)，rank=(9,15)。joint规则选B→A。PTE数3、READ beat数2、WRITE beat数1、正常B数1完全相同；差异来自一次额外direction turn与一次可填的依赖空隙，不来自增加负载或缓冲。

RR、FCFS、相关owner优先、短TL、少欠账均选A。最大欠账和release-density可以选B。因此不能只列前五个弱规则便称新能力。

## 相邻反例：同work，仅τ=0

A→B：PTE A 0–1、PTE B1 1–2、A W 2–3、PTE B2 3–4、B READ 4–6，完成(3,6)，rank=(6,9)。

B→A：PTE B1 0–1、空隙1–2、PTE B2 2–3、PTE A 3–4、B READ 4–6、A W 6–7，完成(7,6)，rank=(7,13)。joint选A。最大欠账/release-density的输入work未变，仍选B；它们不能完全复现这一对。

在τ≥0的本固定工作族，A→B总完成为6+2τ，B→A为7+τ。因此τ>1选B，τ<1选A；τ=1两者makespan均8，而sum分别12、14，应选A。O-turn的完整冻结规则“读写混合且历史τ>历史PTE gap则READ优先，否则短TL”恰好实现此pair与其阈值邻域，τ=1走短TL也给A。这是具体普通实现覆盖，不是把joint提案原样加到普通控制后否定它。两者的观察/估计成本须公平计费；O-turn只需廉价统计和比较，未计算两份完整服务轨迹。

机械核对的完整分段为（本族假定**对称**R→W/W→R同为τ，历史准确给τ和g=1）：

| 条件 | A→B完成向量/rank | B→A完成向量/rank | 无guard时两规则选择 |
|---|---|---|---|
| 0≤τ<1 | `(3+τ,6+2τ)` / `(6+2τ,9+3τ)` | `(7+τ,6)` / `(7+τ,13+τ)` | A，先比较makespan即胜 |
| τ=1 | `(4,8)` / `(8,12)` | `(8,6)` / `(8,14)` | A，makespan tie而sum胜；age/RR无需参与 |
| τ>1 | `(3+τ,6+2τ)` / `(6+2τ,9+3τ)` | `(7+τ,6)` / `(7+τ,13+τ)` | B，先比较makespan即胜 |

若已offered/held，candidate和O-turn都维持同一既有选择；若fair reservation已指定任一持续等待bank，两规则均按相同guard服务该bank；若所需hist缺失或溢出，两规则均RR；只有一legal head也同选它。本族无两轨迹max/sum完全相同的τ，因此age只能在其他输入/guardtie中按共同wrapper发生，不能拿τ=1时年龄改变sum已决的选择。公平状态和history条件须从同一起点给两规则，不能为某一规则单独清除。

**覆盖仅限此固定工作、对称τ、准确历史的条件族及上述共同guard。** 若R→W与W→R分别为x、y，总完成分别为`6+x+y`与`7+x`，关键阈值是`y>1`；本冻结O-turn使用`min(x,y)>1`，在`x≤1<y`范围不等价，不能宣称覆盖所有非对称反馈。本轮不新增该scene或运行它；这里只明确否决任何从对称族扩展到全族的证明。

## 原生适用性核查：这个pair不准进入性能模型

1. `tests/nonblock_actual_dma_test.cpp:653-668`单独给AW/W ready并维护写尾暂停；`:679-719`另从read queue挑R。`:692-697`可保持held-R与同ID约束的DS优先。这里没有上述统一R/W不可抢占服务和τ切换；不能把数学τ塞入模型后声称已证明当前共享DDR反馈。
2. `rv_iommu_banked_ctl.sv:259-268`在current W未填完或可见新AW时限制另一bank READ，并排除同沿新AW之前选旧AR。上面“A相关W owner+B尚未被选READ”的头组合不能直接视为合法。
3. `rv_iommu_trans_ctl_wrap.sv:625-669`每bank一个FSM、WRITE优先且W-counter约束。不能在同bank放两个可独立选择头；两个新AR也不能假定同沿接收。WRITE bank1与READ bank0是否先已合法入REQ_TRANS，须root给完整来源时间线。`:678-683`一旦VALID即保持原请求，后来的feedback不能换头。
4. 原生NormalTL `rv_iommu_banked_ctl.sv:366-392`是稳定RR合流；`tl_wrap:282-306`的native hit-bypass/窗口/quiet必须原样留给所有普通和候选规则。

因此这个pair的裁决是“数学反馈选择成立；O-turn完整覆盖；当前源码服务域/双头可达性未支持，差异门失败”，不是“joint kernel全族已被覆盖”，也不是“原生测得10与9”。root若提出另一source-bound witness，本席只按已冻结普通算法与真实输入裁决，不扩场景搜索。

## 两阶段顺序的精确覆盖域

若翻译和payload是两个**相互独立**的串行机器，固定成本t_i/d_i与顺序无关、两stage按同一合法顺序且无其他carry-in工作，对(i,j)：

`C_i=t_i+d_i; C_j=max(t_i+t_j,C_i)+d_j=t_i+max(t_j,d_i)+d_j`。

比较(i,j)/(j,i)的`(C_j,C_i+C_j,age,RR)`就是O-two-stage，完全覆盖此域两任务joint预测的顺序选择。把翻译称为PTW、payload称为DMA不会增加能力；这个覆盖是明确数学等价。

[Johnson 1954出版商原始摘要](https://onlinelibrary.wiley.com/doi/10.1002/nav.3800010110)说明问题是两machine、每machine一次一个item、各item依次stage1再stage2且setup+work已知（出版商页面lines62–64）；其题目/DOI为 *Optimal two- and three-stage production schedules with setup times included*, DOI `10.1002/nav.3800010110`，页61–68。只取12词原文：“The setup time plus work time for each item for each machine”作为固定代价前提的定位片段。原PDF链接重定向到摘要，未读全文；这里的两任务公式是本席独立推导，不能把摘要冒充对所有joint shared-memory反馈的原文覆盖证据。

当前统一反馈例不满足独立stage条件，当前drive_ddr也没有因此自动满足两stage固定成本条件。发表前作的完整joint覆盖仍须由专门证据席/root核关键原文。仅知道“二顺序枚举/优化目标是一般排程手段”不能据此判断本文目标域完全已有；必须看新增公开信息、具体接口动作和完整成本是否仍有差异。
