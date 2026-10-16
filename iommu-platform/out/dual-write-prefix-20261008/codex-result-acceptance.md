# §43 根级验收：双bank写序分支的停止判别

2026-10-08，Codex root直接设计观测、核关键源、构建/运行、对原始事件比较及作研究判断。1次新build exit0（5.950741s），1次新DUT run exit0（0.485378s），完整copy PASS。未复用旧运行作为新运行；只在out副本加无反馈观测，六个源中其余五个逐字节复制§42。SameBankDmasEnable=0，V/A原ID bit2分别为0/1，实际bank为0/1；均单个正常CONF1各32KiB copy。

## 原始结果

- V offer/P accepted各34笔；root对两个原始逐行序列独立比较global位置、owner、perflow seq及时间先后，34/34一致，两流各17笔。不是仅比较总数，不是比较已各自pop过的剩余队列。
- W head lag共64周期；其中owner不同16周期、seq不同64周期（重叠，不能相加）；最大vr−pr=1；virtual empty而physical仍pending另4周期。
- 第一个lag：fragment cycle1631，vr=1/pr=0/vw=8/pw=1；virtual V seq1，physical V seq0；offered=busy=candidate_valid=0。这时物理旧row最后数据已进TC，不需要新增source child。fragment cycle与C++ g_cycle采样点不同，不直接互换。
- 终态vw=pw=vr=pr=34；两DMA各4096个8B源R/目的W，即各32768B；source64AR、dest34AW，guard、R归属、请求保持、读写/响应计分板沿用并PASS。
- A request g_cycle8315，sealed9469，first quiet11321（request后3006），pause end18315，terminal25261；暂停10000周期。A内部AW义务4→4，无新AW；source child3→7、childbytes3072→7168，新增4个1KiB child；parents5→5，不需要新raw parent；A整job未完成。V request R/W13312/13312→quiet17408/17408；pause end V32KiB/A7KiB。
- TL66/lookup66/hit32/miss34/PTW102/DDTW2/FAST0。source parent32/child64。单次mode2运行没有mode1双bank对照，不能拿§42同bank的不同暂停起点作吞吐优劣或周期因果实验。

## 源码推论与边界

根直接核demux首次offer锁bank、w_open只在virtual WLAST退；perbank AW FIFO及normal completion spill保序；source child每byte覆盖匹配且**external physical accepted**的目的AW前缀；reset无旧payload、默认copy无其它数据入口。由局部FIFO及bank run归纳，accepted P是virtual offer V的前缀，且pr≤vr≤pw≤vw。

注意TC COMPLETE的axi_comp_resp.aw_ready在TC内部normal/error demux之前；它可以先被本bank spill接受，再晚到外部physical AW。不能把两握手当同拍。本证明只需要bank-local FIFO/normal spill不倒序，以及source证书使用外部physical接受点；不需要TC pop与外部接受等同。

head lag可真实发生，但旧physical row的virtual WLAST已完成，剩余数据在TC/output链；W出口独立于当前译址FSM。正常memory/TL获得服务是排空的前提，不证明任意内存公平或固定周期，也不证明全系统无死锁。生产不含此prepared-prefix source原型，不能外推生产一般AW保序。error/constant write/预装payload/跨job残留/动态bank等均未覆盖。

## 创新判断

因为当前原型的demux单bank W run与普通prepared-prefix门已排除AW倒置，而合法head lag只是旧数据退休较晚，这个“两个不可撤写序冲突”的分支不能立独立新机制。按§42预先停止条件停止此枝，不人为去除门或强制换顺序制造优势。

保留成果是平台限制的源码解释、一项自然佐证和普通有限排空验证；不是已发表新定理、普适最小缓冲、PN胜P/N或实板收益。

根补充审定见codex-admission-equivalence.md：纯字节负担准入可由普通owner-prefix记账给相同决策，归为普通对照。下一仅核真实翻译相关额外约束/锐界，必须超出普通记账、短burst及完整响应预约才进入新DUT；仅改计数器/阈值不能立独立创新。具体判别见codex-next-decision.md。

归属：DeepSeek整理接口/前作证据，native Codex独立挑写序不变量反例（未执行）；root Codex设计关键观测、直接核源并执行、验收。未调用Claude；production/Linux/tests/BIT/board/SD无动作。
