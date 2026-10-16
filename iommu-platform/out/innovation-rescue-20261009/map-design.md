# §49 独立正向规则：预付补译的前缀放弃

2026-10-09，Codex 本席亲自设计、推演、核关键原作。没有 DeepSeek/Claude 调用，没有新 DUT、模型执行、build、生产 RTL/Linux/BIT 修改。下列数字是明确受限服务模型中的手算，不能解释为平台速度。本文补充 root 的可退出共享物理读方向，自己只改变翻译保留与源端准入，不依赖物理数据广播。

**提出一个可实施的新规则：共享源翻译阻挡真正可推进的快任务时，先预约慢任务以后补译的工作，再按连续前沿放弃一段共享保留；慢任务追上这段后统一清算。** 阈值式“继续等还是付一次重取代价”已经有 rental caching 前作，不能把这个原则重新命名为创新。值得实现判别的是下文完整的 DMA 规则、压缩账本及其成本点，而非仅一个 eviction score。

## 1. 具体问题与新的控制对象

§48 的 registered-range/remaining-use 告诉控制器某页还有消费者，不能回答为那个消费者等待多久值得。源译址准备、R 退休、WLAST 和 B 响应又不是同一个前沿：一个 copy 可能已产生真正可推进的新源请求，另一已登记 copy 因前一个 descriptor、private capacity 或慢数据阶段暂时不能提出相应请求。

无限保留会损失快任务与尾部完成阶段的重叠；立即释放则可能把慢任务的近未来 hit 变成关键路径 PTW。只在缓存满时按 LRU/remaining-use 排序也没有“我愿意增加多少次页表读取”的合同。

本规则不是保证所有事件序列都比 LRU 快。它控制两个有意义的量：**快任务因为保护复用而承受的实际阻挡，以及主动放弃后必须付出的额外 PTW 工作**。整体性能通过具体到达/服务轨迹比较，不把第二个量偷换成所有 DMA 的周期上界。

## 2. 首版合法合同

首版是两个已登记 linear INCR copy 的一个共享源区间；各任务的源 READ 权限、目的 WRITE 权限仍独立检查。页表静态，普通缓存 epoch 有效，先取单阶段 Sv39；推广 nested 时将每页冷成功 walk 工作上界从 3 次 PTE 读取换成适用的 15 次。MMIO、atomic/exclusive、动态失效以及软件修页等待不进入首版。遇正常 fault/error 走已有终止/排空路径，不制造修页锁。

源页允许物理碎片化，不能假设一条 base-PA/range recipe 已能描述所有页。所有缓存、private current-page recipe、parent 地址元数据、预取结果都应计入真实可保留源映射容量；不得只数 IOTLB 而把 TC/parent 保存的 PA 当免费。

`q_i` 是成功译址证书覆盖的**连续源范围**，不是最高成功地址、submitted/prepared 或数据 `done`。一般 raw-parent helper 能跳过短父；需要 §48 已核的从 done 开始、沿既有 live/generation/certificate window 停在第一个缺口的扫描，或同成本成功反馈。按源页结算时，只在该页所有必要 raw parents 已有有效成功结果后跨过页边界。

受偿方 rematerialization 期间保留一个 current-page recipe，直到该页必要 raw parents 已覆盖；或者控制器证明现有记录提供同样覆盖。这样一页最多产生一次主动放弃导致的补 walk。这个记录及其 key/epoch/permission 必须计费。若做不到，就按可能重复的 raw-parent walks 预约，不能仍声称 3 次/页。

只抑制尚未首次 offer 的新源工作。已有 AR/AW VALID、accepted R、稳定 RVALID，以及已经承担的 W payload 义务继续按原合同排空；缓存工作预算不授权撤回或等待这些义务。本规则不是 W-debt closure 的替代。如果某个已承担义务必需再访问页表，应先计入独立 mandatory-continuation 预约或总预算，不能写成“额外工作全部 ≤B”却遗漏它。

## 3. 状态、事件与动作

固定登记 cohort 有 job generation、context/epoch、共享区间 `[s,e)`、消费者集合。新到达任务另建 cohort，不追溯扩大旧保留合同。首版只需一个快 A、一个滞后 V；多区间/多 DMA 需要区间分段和成员信息，不能说这些位免费。

新增状态如下。原本已有的 job/区间/连续前沿也要在总成本对照中列出。

| 状态 | 含义 |
|---|---|
| `r_V` | 已放弃的共享前缀末端；单调前进。V 尚未覆盖的旧 cohort 页 `<r_V` 不再重新获得长复用保护 |
| `OPEN` | 一次消费者停滞期间的逃生阶段；V 的页级成功前沿实际推进时关闭 |
| `E` | 可推进 head 仅因共享保留受阻的累计 exposure，饱和于开启阈值 |
| `B, spent, reserved` | 本 cohort 允许增加的 PTE-read 工作额度、已支付额度、已预约尚未清算额度 |

动作明确如下。

1. **识别实际受阻。** 只有当 A 的下一 head 尚未 offer、paired destination/private capacity/已有不可撤义务允许推进，且仅解除一个合格共享保留就会放行时才增加 `E`。PTW 自己正忙、A 的 payload parking 已满、没有 head 时不增加。多个保留页造成同一次阻挡只计一次时间，不把同一周期重复乘以保护页数。
2. **选择前缀边界。** 优先放弃最早的共享保留页 `p`，要求 A 已连续 materialize 到 `p+1`，所以不是把一个未首次翻译的页误记成“重译”。前缀选择服务于紧凑清算；它不是宣称最优的全局 eviction 排序。
3. **先预约，后释放。** 从旧 `r_V` 到新 `r'_V` 中仍未由 V 覆盖的页，按每页 `w_max` 预约补译工作。只有 `spent+reserved+Δ≤B` 才推进 `r_V` 并解除保护。实际仍有 private recipe 的页可以保留预约直到清算，首版不为其新建逐页 receipt。
4. **开启逃生阶段。** `E` 达到估计的一次重译服务价后进入 `OPEN`。只要 V 页级前沿不变，A 后续为真实 head 必需的前缀释放可继续预约并推进 `r_V`，不用每页重新等同一个停滞证据。`OPEN` 不提前扫空整个区间；没有 head 就不释放。价可以用近期 PTW 完整服务时间估计，但只有工作额度是硬界。
5. **按真实进度关窗。** V 页级 materialization 推进后关闭 `OPEN`、清 `E`。`r_V` 不回退；此前放弃的页不会因后来重新 fill 而反复 re-pin。尚未放弃的后缀仍可建立正常共享保护。
6. **补译并统一清算。** V 请求放弃区间中的页时，若 hit/已有合法 recipe 就不支付 PTE 工作；若需 walk，实际每次 PTE 读从预约转入 `spent`。首版在连续 `q_V≥r_V`、本段请求结果都完成后统一退还未使用预约。这样不用保存每页实际读了几级的 receipt。取消/错误终止也按匹配 generation 释放无后续义务的预约。
7. **预算不足的动作也是合同。** 保持保护/等待，或采用同信息普通 tile/phase cap；不能把迟到 V 的补译拒绝掉。要使快 A 完成全部已知重叠区间，应给出充分的 `B`，而非声称任意有限预算都能无条件超前。

统一清算是有意的成本取舍：中间页已 hit 但前沿尚未追上时，其预约不会立刻返还，可能提早用尽预算。若为了更早退还而加入每页状态，必须将增加的位数和更新路径作为另一个实现点测量。短父缺口/结果乱序不能仅靠一个最大地址提前退款。

## 4. 两个可检查性质

在上述单次 current-page rematerialization 合同中，每个已放弃页至多增加 `w_max` 次 PTE 读取。释放前预约，实际读只作 `reserved→spent`，阶段清算只释放剩余预约，故对本规则控制的放弃：

`spent + reserved ≤ B`，且累计主动增加的 PTE-read work `≤ B`。

这不是 nested 任意错误/重试情形的一般定理，也不包括未计费的 mandatory continuation。依赖实际 read-service latency 上界和可持续公平服务时才能转成时间界；目前不提供生产平台这样的 latency 保证。

`r_V` 单调和 cohort 固定，使同一旧消费者/页不会因多次 refill 重复开启相同共享保护。若一个仍 pending 的已登记区间有 N 页、可为未来 V 保留 C 个独立映射、V 在 A 完成前完全不能 materialize，A 要完成且不再等待 V，至少有 N−C 页需要以后重取或用其他存储保存 recipe。工作预算、额外 recipe 空间、快 DMA 等待三者必须有一个承担成本。这里的 C 包含一切可复用 PA 状态；页映射可压缩时，这个计数界的“独立映射”条件不成立。

单个固定重译价 `P` 下，累计有用阻挡到 P 再放弃，具有熟悉的 ski-rental 两倍型成本分析；**这是已知前作，不是本文的新理论**。DMA 多页、并发服务、未知速率不能从单页结论推出全 copy 的竞争比。

## 5. 可计算的收益见证与失败见证

### 5.1 长停滞时，让快 copy 提前进入终结 tail

这是受限事件模型，不是当前生产 DUT 的实验：两个已登记任务复制同 N=8 个碎片化源页，目的独立；未来 V 的有效源映射保留容量合计 C=2，V 在 `T=10c` 前因前一个合法 descriptor/资源阶段不能 materialize。`c` 是共享数据路径处理一页 copy 所需的服务单位，取两 copy 数据总工作可按页串行服务。A 最后 WLAST 后还有独立的终结 B-response tail `Δ=10c`；该 tail 不占 W-owner，其他 DMA 可以继续数据服务。首版正常 AXI 允许这个事件，但它是否在本平台有足够大小，必须由后续测量决定。

| 项目 | 永久保护到 V 使用 | 预付前缀放弃 |
|---|---:|---:|
| A 在 T 前可做的数据 | 至多 2 页 | 8 页，外加不超过 ε 的准入/重译代价 |
| A 完成下界/见证 | `T+(N−C)c+Δ=26c` | `Nc+Δ+ε=18c+ε` |
| V 完成见证 | 数据至少从 T 起服务 | `T+Nc+ε=18c+ε` |
| 源映射额外重取 | 0 | 最早 6 页；单阶段上界 `6×3=18` 次 PTE 读 |

永久保护的 A 下界即使把 T 后所有数据优先给 A 也成立。逃生序列则是 A 先完成 8 页 W payload、释放 W-owner 并等待尾 B；V 就绪后做 8 页数据，前 6 页从额度补译、最后 2 页正常 hit。设 A、V 两阶段的全部译址/控制开销分别为 ε_A、ε_V、ε=ε_A+ε_V，则 A 完成不超过 `18c+ε_A`，V 完成不超过 `max(10c,8c+ε_A)+8c+ε_V≤18c+ε`。因此当 `ε<8c`，该合法模型的整体 makespan 有严格收益，且不要求 A 恰好在 T 前完成全部数据。

ε 包括正常首次 source/destination walk，不能把它们删去只剩补译。若本服务模型中一次完整 walk 固定不超过 p、两任务的 16 个 destination pages 各首次翻译一次，则保守总计是源首次 8p、源补译 6p、目的首次 16p、一次开启 exposure p，即 `ε≤31p+σ`，σ 是其他控制开销。只有把普通首次翻译明确吸收到共同时间包络后，才可以将**增量**写为 `7p`；那不是本表的绝对总 ε。工作额度 B=18 PTE reads 仍只统计主动放弃导致的 6 次补 walk，不把一次 PTE read 当完整 walk。

如果允许普通 lookahead 保存全部 8 页 source recipe，这个例子不成立；所以必须配平**整个映射保留空间**。如果真实容量不是 2，将 N 扩到 C 以上再测，而不是硬称默认 IOTLB 只有两项。若尾 B 一直占用全局共享队列，或 A 没有可提前开展的尾阶段，以上重叠收益同样不成立。

无保护 LRU/立即放弃也能达到长停滞点。这一例子证明永久 pin 不总值得，不单独证明本规则优于所有 baseline。

### 5.2 短停滞时，避免把慢 consumer 的关键 hit 变成 walk

另一个保留策略的合法序列：A 的新页 q 现在可发起 miss，q 是一次 10 单位即可成功的 Sv39 root-level leaf；旧共享页 x 为三层 4KiB leaf，重译需 30 单位。V 将在 15 后请求 x，之后有 1000 单位关键数据/终结工作。普通 allocate-on-fill 在 10 将 x 换出，则 V 在 1045 完成；保护 x 到 V 在 15 命中、之后才提交 q，则 V 在 1015 完成，A 在 q 的 10 单位翻译和其后 10 单位短工作后于 35 完成。30 的开启阈值会保留这个短停滞。这样没有把一个原本需要 30 单位才能 fill 的新页假称在 0 就驱逐旧页，也没有撤回已 offer 的请求。

这只是保留/准入的条件性见证。普通 skip-fill/no-allocation 可能直接向 TC 提交 q、保留 x、让 V 以后补译 q；它必须作为强对照。若 private/parent PA state 能继续保留 q 并合法向 V 复用，连这次补译也可能避免，应计入 C。因而不把这一例子说成当前平台已经可达的固定时间轨迹或独有收益。

固定 tile barrier 能命中短停滞，但在长停滞浪费重叠；立即 LRU 命中长停滞，但在短停滞损坏 V 关键路径。这是构造两种速率的必要测试集，不是说普通自适应租约无法匹配。应用到根的 data-fanout 时，数据成员退出与翻译放弃必须分开；放弃广播不必立即放弃低成本译址复用。

**负例：** 缓存保留解除后突然出现高优先级 PTW 流量，V 的补译变慢；或者 V 原本恰在阈值之后就能使用共享页。规则会比始终保留多工作且可能更慢。B 仍约束增加的读次数，但不保证 V 的周期退化上界。多页高频 OPEN/关闭还可能付多次开启 exposure；要测真实 rate change，而不是只测无限慢 V。

## 6. 最强同信息对照与总成本

必须同时比较：未节流 LRU；remaining-use/refcount 永久保护；固定 tile/lag cap；普通固定 TTL；用完全相同 ready-blocked exposure 的普通 rental 阈值；同信息动态短保留/长保留；普通 private future-recipe lookahead；保留旧页但对新页 skip-fill/no-allocation 后直接交 TC 的策略。额外 descriptor/context/成功证书、current-page 状态、counter、任何 software recipe table 的存储与读取，都不隐藏在免费输入里。

首版一个共享区间中，增量 `r_V` 需要 `b_N=ceil(log2(N+1))` 位，OPEN 1 位、exposure `b_P` 位；工作 spent/reserved 各 `b_B=ceil(log2(B+1))` 位，加 cohort generation/有效位和必要消息字段。B 可为配置常量但计数器仍计。原登记区间、q/frontier、权限和 page-current recipe 成本另外列出。N=8、B=18 时，r 为 4 位，两 work counters 共 10 位，再加 OPEN、E 与身份位；不是“完整机制仅 15 位”。

逐页 recipe 至少含任意 PAddr56 的 44 位 PPN，另加 permission/key/epoch/valid；但短区间可压缩、已有 parent 可能保存相同信息。所以不能凭此直接给面积优势。真实贡献条件是**同总存储/接口下**，本规则的紧凑区间清算可给出不同的等待—补译—完成成本点；或者实现证明统一清算相比逐页 refundable credit 的成本与保守性值得。

若 plain rental baseline 加两个预算计数器和同样单调前缀就逐事件、逐位得到相同策略，算法应归于那个完整同效构造；不是将实现中的名称作论文机制。反过来，仅说“ordinary 程序可以加入全部新字段和动作”并不构成已发表覆盖证据。

## 7. 两项 primary 实读及精确边界

只新增核两项作品，版本/hash/路径见 [sources.json](sources.json)。

1. **Khare & Young，arXiv:1208.2724v5，2012-10-18。** [固定版本](https://arxiv.org/abs/1208.2724v5) / [官方 PDF](https://arxiv.org/pdf/1208.2724v5)；本地 [PDF](primary/rental-caching-1208.2724v5.pdf)，[文本](primary/rental-caching-1208.2724v5.txt)。Definition 1 文本 50–66；§3.2、Theorem 3.3/Corollary 3.2，纸面页 8–9、文本 415–449：租金加 retrieval 成本，单文件两次请求间的保留/evict 化为 ski-rental。直接覆盖本文的阈值原则和单页竞争式论证；它没有在已读段落给出 DMA 两前沿、不可撤 W 义务及前缀补译预约接口。后者需按本文具体合同实验，不因没写 DMA 就自动创新。
2. **University of Rochester，US20200310985A1，2020-10-01。** [原公开说明书](https://patents.google.com/patent/US20200310985A1/en)；本地 [HTML](primary/US20200310985A1.html)、[逐段文本](primary/US20200310985A1-description.txt)。¶0082/0085（文本 86/89）以 access 计时并到期 evict；¶0267–0270（326–329）用增量 hits / cache space-time 的 PPUC 分配 lease；¶0415–0416、0423–0431（510–526）给 lease-active/end 标记、程序化 eviction 和与 host replacement 的组合。直接覆盖成本/价值比、lease 标记和 programmed release，不能将这些单独宣称新；已读段落不提供本案 prepaid replay band/连续 materialization 清算合同。未下载/未读其原 PDF，不能把 HTML 行号说成 PDF 行号。

两篇来源的标题/日期已核；阅读范围是上述关键原文，不声称全论文/全专利逐页实读，更不声称全球没有相同机制。对 ski-rental 与 lease 原则作明确排除之后，仍要保留正向具体规则测试。

## 8. 最小决定性下一步及停止条件

最有用的下一步是一个完整 copy 的有限事件模型，先把 C 定义为**全部可复用 source mapping 状态容量**，包括 parent/TC/private 记录；显式建正常 stage1 PTE 请求、source preparation、R/W、WLAST/B、真实连续证书和至少一种短父缺口。只扫两类轨迹：V 短暂停、V 长停滞+可重叠终结 tail，再加入停滞结束后 PTW 压力变化。报告两 DMA completion、makespan、每页首次/额外 PTE work、source等待、映射/账本位数，不用 hit 数替代速度。

同信息 plain rental 和 private-lookahead 对照若能以同总位数、同 extra-work 额度得到逐事件相同结果，压缩清算分支停止单独算法立名。如果本规则在短/长两类轨迹中没有同预算下的性能/成本新点，或预约必须靠大逐页记录才能准确实现，就修规则或停止这项；不因一次对 naive pin 的胜出接受创新。

**给根 data-fanout 分支的一条关键质疑：** sink 退出后是否仍必须保留完整旧 raw-parent translation recipe 到 replay？若 parent 地址/权限保存支持 cursor 补读，这些记录与 shared-TLB 保留一起计 C；若退出后原记录释放，回读必须重新独立 READ-check 并计 replay。不能同时按“保留 recipe 不用补译”节约工作，又按“退出释放全部状态”节约容量。数据 fanout 放弃、译址保留放弃是两个不同动作，首版可以选择只退出前者。

**可以形成独立机制贡献的条件：** 上述完整区间逃生与清算规则在同信息、总状态和 PTW 工作预算下，得到普通租约/phase cap/private-lookahead没有的可复现实例或更好的成本点，并完成正确的 mandatory-continuation 组合。**不能作为独立贡献的部分：** ski-rental 阈值、hit/space-time 价值比、descriptor future-use/refcount、lease-active/end 以及将单页竞争比直接套到整个 DMA。
