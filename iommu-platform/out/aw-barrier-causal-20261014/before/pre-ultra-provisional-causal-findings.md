# 部分W前缀＋AW准备：待ultra裁决的原始发现

actualgoal1791959885为blocked（运行强度条件未满足；未complete），无额外预算。以下是本轮源码发现及待复核接口，**没有最终贡献/覆盖/收益裁决**。本轮主席实际记录6.1-sol/medium，要求为ultra；本机默认已ultra。证据原样保留，需新turn记录ultra后亲核关键边并继续原goal；不重跑实验。

## 已读源码中的必要条件

1. 原peripherals:1269–1272给DMA0/1条件容量512、NumAxInFlight=8、RAW_COUPLING_AVAIL=0。backend:811–858因此走AW fall-through bypass，而不是可选channel_coupler。frontend强制decouple_aw=0，不代表该可选coupler被启用，也不等于decouple_rw=0。
2. frontend:366–373：decouple_rw来自实际conf.decouple；deburst也来自实际conf。FullBuffer certificate:17–20只核capacity/normal_copy/length/alignment/sole job，**不检查这两项选项**。不能用DEFAULT、FullBuffer或本机默认配置代替已accepted job的实际选项。
3. backend:472–473将r_ready绑定read-meta/AR，w_ready绑定write-meta/AW/lastflag。legalizer:452–465在decouple_rw=0时，R/W请求成对推进；decouple_rw=1时分别推进。backend的AW bypass存储是fall_through_register，两个已读common_cells副本均DEPTH=1。
4. 对sole aligned4KiB、deburst=0且**实际accepted decouple_rw=0**的两256beat配置：当A第一个AW已经占住这单槽而被输入demux阻挡，槽full使aw_ready/w_ready为0，第二对AR/AW不能生成。因而`emit(A.AR1) >= accept_raw(A.AW0) >= raw_WLAST(B)`，依原FIFO逐拍实现可能更晚。没有新增队列或强制改选项。
5. 对accepted decouple_rw=1，R生成不受这个w_ready条件阻挡；不能将上条边用于该分支。同样不能为制造收益切换配置。本轮没有取得实际accepted job快照，两个分支都应保留条件标签。
6. 输入demux:175–181在raw W仍属于B时不转发A AW；A bank的AW FIFO可以仍为空，FullBuffer允许其第一个AR被选择。因此第一A读翻译P与B的W前缀并未被上述单槽边排除。TC:625–669的AW优先只针对自己bank内已可见的AW。
7. TL:303–306的native N window要求candidate.bank不同于慢P.bank，TC一次只处理一个请求。因此A自己的写翻译不能借同bank N在A当前读P上并行穿过；要保留read slow-response、TC physical AR spill接受与后续AW选择边。B的合法缓存hit可以属于另一bank N路径，仍需原上下文/权限真实证书。
8. PTW:384–421/507–566在真实PTE返回后才判leaf/nonleaf、形成下个完整PTE请求。下一PTE准备可能与已承诺L的R服务重叠，但未来leaf/实际剩余PTE不是免费输入，必须由完成记录估计且完整walk继续执行。

## 两策略必须保留的链（未填未来数值）

| 端点 | ADMIT L | WAIT同一个P的共享AR接受 |
|---|---|---|
| P完整walk/slow response | L可能竞争当前与后续PTE服务 | 仅先让当前P进入；随后释放L，不能把WAIT延长到整walk结束 |
| B真实R→raw WLAST | 由L实际数据/原buffer/W ready决定 | L延后多少以及是否被既有W积压遮住，均需真实服务约束 |
| A first raw AW | 等B原raw W owner清空；native held不撤回 | 同一原门槛随B进展变化 |
| A.AR1生成 | coupled分支必须先接受A.AW0；decoupled分支独立 | 同一实际accepted选项，不跨分支套公式 |
| A write translation/physical AW | TC当前read/spill与主NormalTL/完整walk/hit、原credit决定 | 同样逐端点计费，不能塞入跨策略同H |
| A两段R与逐拍W | 不把P释放AR0写成释放整份512beat | 第二段未发工作是已accepted job意图，但不是已accepted packet |
| 两job B/退休/quiet | 各自真实B与last-burst metadata；正常RESP另核 | 不共用z、不先验CA=max；保持原后继供给 |

这张表是待完善的因果接口表，不是已取得时序trace或运行模型，不给预测/完成时间数值。

## 已冻结对照与尚缺的决定信息

冻结见frozen-controls.json：保留父轮currentDS+RR/earlyPause/owner/cheapDebt/dualGap原规则及端点澄清，补原native RR和明确current raw W owner有合法关联payload头则ADMIT、否则currentDS+RR。冻结发生在本轮新推导前；不按结果改对照。

单独“P早返回”不构成完整job收益；“下一PTE准备被L遮住”也需要与原dualGap完整walk释放间隔比较。是否存在统一普通不能复现的完整差异仍待ultra裁决，不能从当前源码摘要宣布覆盖或独有收益。

缺项为：实际accepted decouple_rw/deburst与存活packet/job/P快照；逐段R/rawW/outputW/各B的同端点正常history与策略相关共享服务约束；未发AR1的真实供给/选择、peer AW准备、完整原后继供给及必要数字费用。actual prefix、history、bytes、PTE/R/W/B/completion/netgain、area/Fmax均未取得。

不开展正反条件矩阵、观测/model/bench/RTL/FPGA。当前仅保存已读原文和接口发现，恢复约定强度后继续相同有界任务；不恢复任何关闭分支。
