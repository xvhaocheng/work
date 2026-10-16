# §43 双bank写序分支停止判别报告

2026-10-08，root Codex设计观测、核源、构建/运行、独立比较原始事件并验收；DeepSeek整理前作证据/文档草稿；native audit仅纸面推演未执行；未调用Claude。root已执行1次新build exit0（5.950741s）与1次新DUT run exit0（0.485378s），完整copy PASS；未复用旧运行作新结果。仅在out副本加无反馈观测，六个源中其余五个逐字节复制§42；SameBankDmasEnable=0，原ID bit2分别0/1，bank 0/1。

## 结果

- 虚拟AW首次offer与物理AW接受各34笔；root对两个原始序列独立按global位置、owner、perflow seq及时间先后比较，34/34一致，各17笔。不是只比总数，不是比较已各自pop后的剩余队列。
- W head lag共64周期：owner不同16、seq不同64（重叠，不相加）；max(vr−pr)=1；virtual empty而physical仍pending另4周期。
- 首个lag：fragment cycle1631，vr=1/pr=0/vw=8/pw=1；virtual V seq1，physical V seq0；offered=busy=candidate_valid=0。物理旧row最后数据已进TC，不需新增source child。fragment cycle与C++ g_cycle采样点不同，不直接互换。
- 终态vw=pw=vr=pr=34；两DMA各4096个8B源R/目的W，即各32768B；source64AR、dest34AW，guard/归属/请求保持/计分板PASS。
- A request g_cycle8315，sealed9469，first quiet11321（request后3006），pause end18315，terminal25261；暂停10000周期。A内部AW义务4→4；source child3→7、childbytes3072→7168（+4个1KiB）；parents5→5；A整job未完成；quiet时A 7KiB；pause end V 32KiB。
- V request R/W 13312/13312→quiet17408/17408。TL66/lookup66/hit32/miss34/PTW102/DDTW2/FAST0。source parent32/child64。单次mode2运行无mode1双bank对照，不能拿§42同bank的不同暂停起点作吞吐/周期因果；Stage42四run未重跑，新3006不可与4115作因果比较。

## 源码推论与边界
root直接核demux首次offer锁bank、w_open只在virtual WLAST退；perbank AW FIFO及normal completion spill保序；source child每byte覆盖匹配且以external physical accepted目的AW前缀；reset无旧payload、默认copy无其他数据入口。由局部FIFO及bank run归纳，accepted P是virtual offer V前缀，且pr≤vr≤pw≤vw。TC COMPLETE的axi_comp_resp.aw_ready在TC内部normal/error demux之前，可先被本bank spill接受，再晚到外部physical AW；不能把两握手当同拍。证明只需bank-local FIFO/normal spill不倒序，证书用外部physical接受点。

head lag可真实发生，但旧physical row的virtual WLAST已完成，剩余数据在TC/output链，W出口独立于当前译址FSM。正常memory/TL服务是排空前提，不证明任意内存公平、固定周期或全系统无死锁。生产不含此prepared-prefix source原型，不能外推生产一般AW保序；error/常量write/预装payload/跨job残留/动态bank未覆盖。固定mapping、DEFAULT CONF1、单job静态finite单例，conditional source induction不是formal model checking，也不是general production theorem。

## 创新判断
因为当前原型demux单bank W run与普通prepared-prefix门已排除AW倒置，合法head lag只是旧数据退休较晚，这个“两个不可撤写序冲突”分支不能立独立新机制。按§42预停条件停止此枝。保留成果是平台限制的源码解释、一项自然佐证和普通有限排空验证；不是已发表新定理、普适最小缓冲、PN胜P/N或实板收益。Codex进一步构造了普通owner-prefix记账：如果准入只取决于队列前缀及缺少的源数据，普通方法就能给出相同决定，所以纯字节负担准入也不列为新机制。下一仅找真实翻译约束或锐资源/延迟界，必须超出普通prefix记账、短burst及完整响应预约的同信息/同资源保证，才进入新DUT；不先编译阈值扫描。

## 引用
[执行账本](../out/dual-write-prefix-20261008/stage-evidence.json) · [34笔原始序列与快照](../out/dual-write-prefix-20261008/order-evidence.json) · [根级验收](../out/dual-write-prefix-20261008/codex-result-acceptance.md) · [写序源码推论](../out/dual-write-prefix-20261008/native-order-invariant.md) · [准入等价控制](../out/dual-write-prefix-20261008/codex-admission-equivalence.md) · [前作勘误](../out/dual-write-prefix-20261008/admission-prior-corrected.md) · [下一判别](../out/dual-write-prefix-20261008/codex-next-decision.md)。
