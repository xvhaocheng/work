# 同一初态AB／BA事件边界

完整ID、owned债务、逐事件和未知字段以 [abba-event-table.json](abba-event-table.json) 为准。本表是源码及条件偏序证明，**没有执行新场景或测得周期**。

## K0：现有private对齐sole4KiB／512拍

| 阶段 | AB | BA |
|---|---|---|
| 共同t0 | 原生两源0/1，IOMMU ID0/4、DDR32/36，DEFAULT，各512拍，无前作业残留；双头fresh条件才比较 | 同一状态 |
| NormalTL/完整walk/真实AR | 保持原生承诺、完整walk和所有权；事件周期及PTE数unknown | 同样约束，顺序交换 |
| payload首次RVALID/held | 一源最多512拍，最后一拍前至多511占用，不会因本作业容量产生第513拍held | 同一容量排除 |
| W/B/fault/quiet | 仍需真实完整退休；未执行，不能宣布完成或收益 | 同样未执行 |

这只排除具名own-capacity held正例，不能声称所有瞬时RREADY高、PTE/meta没有等待或所有安全READ顺序等效。

## K1：生产同bank条件返回cut

同一X0：A=source2/bank0/DID12，IOMMU8、DDR40，已捕获第一READ256拍/2048B，buffer3；B=source1/bank1/DID11，IOMMU4、DDR36，真实sole4KiB/512拍证书。已有全局W owner=source0/bank0/DID10，IOMMU0、DDR32；其末W仍缺READ0，bank0 AR队列为`[A, READ0]`。A的正常WRITE准备和后序W义务已经计入；没有省略WRITE准备构造根因。两头必须同时fresh、未mergedVALID/未forced，B只凭真实fullbuffer例外准入；否则这个X0不成立。

**X0从冻结DEFAULT共同初态的实际可达性unknown。** 不将任意接口FIFO前缀视为真实iDMA运行。如下偏序还限定completion R按当前已知FIFO规律、DS每拍优先且尊重held、W/B独立有限服务、固定合法映射；不是MIG板测。

| 事件 | AB：A先 | BA：B先 |
|---|---|---|
| 完整NormalTL | A walk先，B若早已offer则锁住 | B walk先，A后 |
| PTE生成/接受 | 每次由上一收到PTE算下一pointer；已DDR接受DS可在未held时提升 | 同一实际链；不提前消费future PTE |
| physical payload AR | A的DDR AR仍早于READ0 | B可先进入，但A仍早于READ0 |
| payload firstVALID/held | A不能得到W drain；本明确RTL边界最多3+8=11拍，无drain最终held；B PTE可已完成或之后被挡 | B返回可装下但W/B未完成；A仍无drain，最终受同一11拍上界 |
| W/R因果cut | A RLAST要W2 drain；W2要W0 LAST；W0要R0；FIFO R0要A RLAST | 完全同一cut |
| PTE未生成/已队列分支 | 未生成者无优先对象；已队列者也不能撤回held A；16队列准入满不能取消A | 同样；确切首次held与PTE先后unknown |
| 完整R/W/B/faultquiet | 固定合法映射无自动fault退出；A、B及carried owner债务没有finite全部成功/quiet | 同样。B更早TL或payload R不是完整完成 |

返回槽8是xbar DDR-facing→iDMA明确RTL区间，不能叫全板容量；共享槽已有占用只减少本源空间。板IP额外容量/服务unknown，首held拍unknown。该cut在条件成立时不受两个bank之间先后救济；若能提供owner0已有足够数据、R0早于A、合法完成R重排、不同依赖，则已经改变本X0，需另立合同。

## K2：唯一安全头先的具体普通覆盖

前提是同一fresh、同一已付完整source／finite-footprint／slot／WRITE供给与W租约证书：一个安全头，一个已证危险READ。若F1的声称动作仅安全头先，**一个冻结O-envelope-select选择相同头**；没有预测历史时F1走RR，不能替它假造选择。held／forced fairness对二者均优先，unknown不是unsafe。

若两规则同时刻offer同样immutable头，且后续原生控制与环境相同，其所有R/W/B/fault事件由同一状态产生。尚未冻结两规则select时延，因此不能把这条条件逻辑等价写成实际逐拍或完整数字成本相等。K1进一步证明其具名cut下任何有限选择时延都不能创造全部完成；K2不关闭双安全头服务预测域。
