# §71 online choice：绑定安全与 prefix→joint 阻断

Codex 独立关键源审，直接读本目录 source/top/system/C++ 与 §69 source diff。没有改 RTL、构建、运行或验收结果 CSV；下述反例是合法接口事件的手工推演，不是 DUT 已观测失败。引用行号对应本次读到的实现。

**裁决：choice 在 start_cutover 锁定本身未引入旧 AR/R/ID/RLAST 不一致；32bit 成本乘法在窄 N256 范围安全。然而共享分支沿用了一个可达的 prefix→joint 进展漏洞：parallel repair 把 logical join 改为 E，未保证 V rawW 已到 E，phase3 又只裁 A prefix、只对 start≥E 使用 peer gate。旧 B 只需有限延迟就能让 V 新缺 R 的 W头跨 E 并堵住 A 全resident旧prefix。解决该门以前，不能接受本轮 shared-choice 进展合同。**

## 1. 首个阻断：V 未消费完 E 前缀时的新 W 可以跨 E

当前实际门：[multi_actual_dma_top.sv:732](multi_actual_dma_top.sv:732) 的 phase3 prefix裁短只有 `f==1`；[740](multi_actual_dma_top.sv:740) peer gate只检查 `start_beat >= v_cut`。parallel shared builder在 [source:334](shared_pretl_read.sv:334) 只等双方 logical r=E 和两个 physical route退休，然后置 `v_cut=E, phase3`；没有等双方 rawW=E。这与旧非parallel handover的“V已实际写到J再join”的前提不同。

### 最小角色与可达状态

使用当前允许的 mode3、MaxOutstanding2、Q2/B3、source256、A native F1（dst offset4088）、static成功READ/双页WRITE cert全256，当前 old V physical end E105，例如 K15 的第七个 source child。J可以96或98；phase1早已实际闭合该J。

phase2末双方各自 prefix读取到105；原 V physical RLAST与A prefixphysical RLAST已真实接受、最后skid交完。允许下面的实际状态：

| 状态 | V | A |
|---|---:|---:|
| logical returned |105|105|
| actual rawWritten |104|103|
| 当前 payloadchild |空|空|
| pendingB |2|2|
| 已送完 payload的旧qW |最近两笔，末端104|最近两笔，末端103|
| 私有FIFO未消费payload |index104|index103、104|

这不是凭空的满 FIFO：phase2允许 source借两槽，且注册新W需全部payload resident [top:746](multi_actual_dma_top.sv:746)；两个 pendingB 可以禁止创建下一个全resident child。A从F1以后q2网格为奇数末端，故103合法；V偶数q2网格末端104合法。两边 older pendingB所对应的 W已送完，因此该状态不需任何新R才能返回这些B。

### 有限 handshake 序列

1. joint builder看到两方r105且route/skid空，创建共享AR[105,256)，置 phase3/J105。该拍两边pendingB2，splitter均不能创建尾child；所以不存在事先注册的V q1[104,105)。
2. 返回一笔V旧B，V pending降为1。次拍 V创建qW[104,106)：start104<E105，当前phase3既不裁V prefix，也不检查peerA rawW103。它先到input mux first-VALID并成为全球W-owner头。A旧B稍后才返回；均是有限延迟、正常响应。
3. 返回一笔A旧B后，A创建全resident qW[103,105)，它在V token后面。这个次序甚至可以符合全局旧B FIFO：旧payload顺序可为V[100,102)、A[99,101)、V[102,104)、A[101,103)，因此V第一credit早一拍释放足以抢到first-VALID。
4. V头可以消费index104，使V rawW105；末beat index105还没有logicalR。A qW的103/104在FIFO，但被V W头挡住，A rawW仍103。
5. 共享R index105要求双方接收。A `returned−rawWritten=105−103=2` 且尚无覆盖index105的A AW，触发 [source:162](shared_pretl_read.sv:162) private lead2门，整个physicalR被held。A若要减lead先要发W103；W103又排在缺R105的V head之后。

环为 **V新qW缺R105 → sharedR等待A私有槽 → A旧全resident qW等待V全球W头**。旧B已经按有限合法序列返回，继续公平返回剩余B不能解除它；没有永关budget、不公平memory或新增TL锁。选择rule0/1都可能合法选择share，M足够大即可，所以把choice锁定不会排除这条轨迹。

### 需要的动作门（任选其一，再审实际实现）

- phase3也把V `start<E` 的**尚未注册**child裁到E，如当前A prefix；V到E后所有joint新start需peer实际rawW≥E，A旧prefix便可先排空。
- 或将phase3 `new start <= peer rawW` 应用于所有新child，包括start<E，而非仅joint后缀。上述V start104在Araw103时会被阻止；A start103≤Vraw104可先推进。
- 或shared suffix builder在双方rawW都E才join。phase2的全resident W和正常B服务足以排空旧prefix，不能把原 parentB全退休作为额外必要条件。

所有门只能作用于尚未first-VALID的child；已经注册/offer的[104,106)不能事后裁成q1。当前source的旧held AR/R/W/B保持门与该反例相容；不能靠撤VALID修复。反例不会否定独立choice分支，因为它一直phase2、每个新W保持全resident。

## 2. choice 锁定与旧义务：未发现新增 binding race

[source:120](shared_pretl_read.sv:120) start须phase0 busy、无skid、两parent和真实READ/WR成功，且closure覆盖本地registered V promise。start拍 [173](shared_pretl_read.sv:173) 强制physical RREADY0；此时没有held logicalR。start前旧物理AR已由source真实握手，source内部offered为空；外层spill如果仍held旧AR，保存的是原header，不会依赖后来重用的main metadata。

[source:274](shared_pretl_read.sv:274) 同一沿 latch choice/decided/geometry并进入phase1。phase1没有A repair builder；choice至少在phase2首次AAR以前稳定。新candidate/ordinary分支均使用 `repair_shared_active`：Arepair_end [140](shared_pretl_read.sv:140)、独立Voffer [312](shared_pretl_read.sv:312)、shared suffix [334](shared_pretl_read.sv:334)、V续段LAST [417](shared_pretl_read.sv:417)、newWclip [top:717](multi_actual_dma_top.sv:717)。system参数透传 [1166](multi_system_top.sv:1166) 与top [1004](multi_actual_dma_top.sv:1004) 一致。

旧V物理段下一index始终<E；无论choice取0/1，LAST endpoint都仍E。只有独立V continuation握手后rV从E开始，endpoint才为256。旧source ID与A main ID/len/got仍分开，choice不重写旧childheader。phase1旧registeredAW/W由splitter继续兑现，new clipping不修改held `child_q`。

`repair_shared_q` 除reset和start无其他赋值，因此同一job中按构造不可变。[source:217](shared_pretl_read.sv:217) 实际assert检查“已decided又start”的重绑定事件，不是独立的 `$past(choice)` 值稳定assert；设计文档不应把它写成已执行的稳定验证。当前singlejob/reset合同足够，不支持不停reset的多job choice复用。

## 3. 计数是真实因果观测，但不是纯 DDR bandwidth

[source:208](shared_pretl_read.sv:208) 只在Adaptive、未decided、非start、phase0 busy、当前child_got非0时采样；每项16bit饱和：

- M：RREADY1/RVALID0；H：真实R握手；C：RVALID1/RREADY0。
- first handshake发生前child_got0，M/C/H全不计。新的AR handshake清got，所以每child初始等待都排除；**H也漏该child第一笔成功R**。H≥8意味着至少9笔同child握手，或者跨多个child累计至少8笔after-first握手；不是“总共收到8 beats”。K1所有child都没有after-first样本，H0保守独立，是明确算法边界。
- 这三类并不覆盖所有cycle。RVALID0/RREADY0不计；RVALID被global skid挡、DMA FIFO阻塞或目标依赖造成的ready0都属于C，不能把C叫纯consumer带宽。
- 输入RVALID是实际direct-source返回；PTW不是直接加进H/M。但其共享DDR/互连竞争可让source缺valid，从而增加M。M描述本地观察到的外部服务缺口，不能区分memory节奏、仲裁、其他PTW流量。排除首R latency不等于所有翻译/固定开销都被排除。

所有计数都来自binding之前，geometry E来自已不可撤的physical child endpoint、cursor来自实际logical delivery；没有读取DDR-period knob、未来completion、尚未观察的A写延迟。start拍不采样，因此freeze后输出正是用于决策的历史值。它们是合法预测输入，但两条具有相同prefix/H/M/C/E的合法memory轨迹可在之后分别快速/慢速服务，故任何此局部规则都无普遍最优或tail-cycle保证。当前文档已把规则称heuristic，应保留该限制。

## 4. 32bit 成本算术的当前边界

[source:84](shared_pretl_read.sv:84) 对每个乘数cast32，C+H也是32bit相加，不会先在16bit内溢出。binding时由job/cursor检查保证 `0≤E≤256`、`0≤rV−rA≤256`；负lag在 [279](shared_pretl_read.sv:279) fatal。

即使用饱和计数最坏上界，`(256−E)*M ≤256*65535=16776960 <2^24`；`lag*(C+H) ≤256*131070=33553920 <2^25`。32bit乘积足够。算术在phase2中仍组合变化、可能出现负lag转unsigned大数，但choice只在已检查合法lag的start拍读取它；不是后续rebind输入。不能把此结论推广到未限长job、不同counter宽度或dynamic Q。

Rule0可能在E256且M>C时置sharebit1；没有可共享suffix，实际仍读512。Rule1 E256的左成本0，严格大于不成立，会选独立。必须区分“sharebit选择”与真正merged suffix，不把E256的bit1计为成功共享。

## 5. fallback、adaptive R 期望与参数范围

[multi_test.cpp:1568](multi_test.cpp:1568) 只有实际cutover才要求decided，并依锁定choice算shared `E+(N−FA)` 或独立2N；没有以已测Rcount反推expected。原J/FA/E检查、logical完整bytes [1589](multi_test.cpp:1589)、physical目标256/256 [1602](multi_test.cpp:1602) 和后续byteguards未由此diff放宽。它核route/count一致性，**没有重新从H/M/C算rule的正确性**；规则本身这里通过源和位宽审查，而非已运行独立oracle。

未发生start/cutover时，reset的share值可仍是1但decided0；mode3旧fallback顺序完成两独立copy，C++保留512旧期望，不用未决定choice替换它。不得报告这类run为一次online选择。E256实际cutover但没有sharedtail，两路期望恰重合512，真实merged观测才区分是否共享。

当前允许的完整adaptive绑定合同须是 **ParallelPrefixRepair1、mode3/FA0、同一次static job**。两项范围外问题：

1. 非parallel phase2 builder [source:350](shared_pretl_read.sv:350) 仍固定进入原shared phase3，没有服从`repair_shared_active`。若Adaptive1/Parallel0选独立，报告2N与实际旧shared work不一致；本轮Parallel1不触发。需要显式参数合同/拒绝该组合，不能将它称为支持配置。
2. mode1已共享返回A前缀FA>0时，独立补读从FA而非0开始，实际work=2N−FA；当前独立expected2N仅对mode3/FA0正确。前作审核已有此限制，adaptive宏未消除它。

## 6. 成本和当前停止条件

相对fixed选择增加48bit计数、choice/decided两bit声明状态；decision cursor/E共18bit只用于observer报告。rule1的两个32bit组合成本不是新增寄存器，却有真实算术逻辑成本；没有综合面积。已有独立offer/oldV route/private payload沿用，不能把历史窗口/选择器当零资源。

online binding保留不可撤握手并复用现有有限payload合同，是可实现的正向动作；固定share/独立作为公平对照仍必要。现在阻断是第1节具体prefixW门，不是阈值起新名称或泛泛优劣。修掉该门后，再按实际冻结choice对照同fixed route与完整byte/count检查；本文不预测构建/仿真通过，不提供生产/board/fault/general-arrival theorem。
