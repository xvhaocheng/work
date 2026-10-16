# Codex 审定与候选收窄（2026-10-08，§28）

最终对外报告：[依赖信息、翻译寿命与写承诺](../../docs/dependency-contract-innovation-20261008.md)。本文件由 Codex 亲自作架构与证据裁决，非 worker 摘要验收。原答案完整保留，不覆写其中错误。

## 1. 本轮真正新增的证据

- 新取得五份公开 PDF/TXT：MetaSys TACO2022、TAPA TRETS2023、XDMA arXiv2025（未核正式发表）、Arm LTI IHI0089A 2020、Intel DSA 341204-006US rev3.0 June2025。下载失败记录及成功来源/hash分别在 papers/sources*.json；每份成功TXT来自 pdftotext -layout exit0。
- Codex直接核：LTI简介和lookaside、SPEC、LC/tag、§6.2.1/6.2.2；TAPA §3.4/6.1；MetaSys接口/与XMem差异；XDMA配置/数据两阶段；Intel DSA §8.3.11。既有 ISCA2018 Walk Scheduling §IV重新核：同SIMD指令批处理、剩余walk成本、最低score及老化/同分先老；非新下载论文。
- 必须区分 source 内 descriptor/copy 语义、外部 AXI-IOMMU 消息以及测试额外层次信号。不是“iDMA没有copy依赖”。
- tests/nonblock_actual_dma_top.sv:638–646 从 iDMA 内部导出job/src/dst/length；AXI-IOMMU typedef.svh:37–81没有定义这套copy关联合同，通用USER可扩展，故不是通用不可表达定理。
- trans_ctl_wrap.sv:716–726的 COMPLETE 在完成地址侧AR/AW ready后回IDLE。此处不是Arm LTI completion，更不是失效安全的全球可观察点。

## 2. 第一答复纠正

1. socdma2018 PFN去重/重发不证明源读对应哪段目的写；iDMA描述符/dataflow本来有内部copy语义。
2. 普通 C 的构造能力不是已发表 C 的存在证据；“由普通原语能表达”也不能单独否定组合创新。
3. reliable_prefix需与当前区间相交并连续；原段长度求和双计。
4. 消费即归还与结束再归还不得叠加；会双重增加free。
5. AXI-REALM收齐W后才转发AW/W是实质强对照，不写成“无drain”而略去它。

## 3. 修订答复仍不接受的部分

- 第二答复的credit方案B：消费时不归还，最后只归还 reserved-consumed，会泄漏consumed对应额度。方案A写成 sum(reserved)+free=N 同样不守恒，因为reserved未随归还减少。
- 第二答复的prefix还缺连续性检查：seg.lo大于front时必须停止，不能从间隙后继续加。
- “LTI即时使用、不可缓存”不等于完全不能在ordering就绪前准备/短暂等待；简介明确支持先翻译后满足ordering。该标准没有给我们copy联合W调度策略，但这不是空白证明。
- SPEC不能用作真实事务译址；ATST则指已被ATS翻译的请求，不是“不可使用的预准备”。
- 不能以scope推断任何copy必有两个不同LAID；相同ID在规定ordering条件可合法复用，而且src/dst同页也必须分别考虑R/W属性。首判限制为两个独立不同页。
- XDMA“不同事务无依赖”的作者分析不是“copy内无R→W依赖”的证明。它描述两端协作grant再传数据，足以覆盖宽泛协作主张。
- 单留一个槽必须说明给mate用，新单边持有者不能占该槽。也不要求“同一步原子释放两个pin”才有进度；正常有限处理与公平释放足够，异步释放仍可安全。
- 有限pin槽反例不是生产RTL已见死锁。该策略/缓冲尚不存在，不能把它当当前平台bug。
- 把返回payload池的D与译址pin池的F直接作同槽比较混淆资源单位；两问题必须分开。
- worker引用行号按工具文件包装的splitlines可能把formfeed计为行，本报告用物理LF/nl -ba定位，关键行号已重新核。

## 4. 接受的安全记账规格（非已实现代码）

先限对齐R/W，L为实际需经过受限返回池的总字节数。非对齐填充需先有独立丢弃保证，否则同样计入L。

front = ar_lo
遍历按逻辑offset排序的目的段：
  if seg.hi <= front: continue
  if seg.lo > front: break
  if not 独立排空承诺(seg): break
  front = min(seg.hi, ar_hi)
  if front == ar_hi: break
p = front - ar_lo

准入：outstanding[r]=L-p；free减该值。
只在预约后缀中实际退休k字节：要求k<=outstanding[r]；outstanding[r]-=k；free+=k。
直通前缀不归还pool额度；结束时要求outstanding[r]=0，不再加free。
守恒：free + sum(outstanding[r]) = pool_capacity。
物理地址分配/碎片化可能另有代价，这只是credit规格，不是实现验收或最小容量定理。

## 5. 本轮候选的裁决

D：准确普通per-segment cut-through + byte credit可表达同预约，暂无独立规则差异。原理可组合，不据此宣布“已有同款发表”；是否有低成本实现贡献仍未定。当前CONF0不产生示例，不先跑弱D大矩阵。
E：额外copy信息可改变桥可观察的依赖，但单纯把测试层次信号搬成接口只是工程化。MetaSys/XMem/TAPA/XDMA覆盖宽泛信息/流控/两端协作。
F：先准备src/dst再取得W顺序有真实平台动机，但LTI、DSA翻译预取、Walk Scheduling批处理、AXI-REALM写就绪后提交构成强先行；普通整对预约/ready队列必须加入。
下一保留的是“受写服务进度约束的翻译准备寿命”这个窄研究问题：准备期只作可丢弃缓存/提示；真实提交前复核两端和必要进度资源；仅对将使用的首片段建立真实在途映射合同。不是把普通预取+ready队列直接命名新算法，未有区别它的收益证据。

主贡献若成立须包括：合法接口上的容量/映射寿命/服务进度边界；比强普通基线有代表性优势的具体联合决策；真实iDMA验证及全部代价。无需每个原语全新，也不要求每一负载都胜；但源码bug修复或强简单方法完全同效同成本不足支撑新机制。
LTI §6.2.1限制LC不依赖另一Stall响应；§6.2.2要求使用它的事务全球可观察后LC。F原草案“地址握手后退handle”不能当作LTI/失效安全实现。首判用静态映射，不声称完成通用失效/故障证明。

## 6. 实际状态

仅研究材料和文档更新。新生产/test/Linux代码、测试/仿真/构建/BIT/JTAG/SD/板卡动作均0。最新实际实验仍§26四次两DMA RTL仿真，FAST等待1051/1052cycle不是可直接相加的收益上界。本轮没有四真实DMA实验。
DeepSeek成功2次high/thinking/max393216、均stop未截断，126940+239955=366895 provider tokens；另1次文件验证失败未返回usage。Codex独立设计、核关键原文/源接口、纠错裁决并实际下载/保存/更新文档；无Claude或新Codex子agent。

