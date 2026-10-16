# §43 Native Codex：两bank的AW顺序不变量及W退休滞后

2026-10-08，`innovation_rules_40`。Codex亲自推演并直接核指定vendor demux、§42 fragment、banked HeaderPark0/Gate0及out TC W/COMPLETE路径；本席未run/build、未改RTL、未扩文献、未调用Claude/DeepSeek。root随后报告已完成一次自然dualbank build/run；下面明确分开源码证明与root运行的有限佐证。

## 结论

**受限平台中，physical accepted AW轨迹是virtual首次offer轨迹的前缀；没有找到合法的跨bank AW顺序倒置反例。** 根推断的主要结论成立，因为旧bank要交齐virtual W才能切bank，而任何这些W有效byte在其source child出生前就已有对应physical AW接受。

必须区别：两个剩余W队头并非每拍相等。virtual WLAST先进入TC FIFO、physical WLAST晚到DDR时，可以有owner/seq不同；这是**退休滞后**，旧head的全部payload已进入TC，不需要新增source/TL才能排空。它不是交叉顺序环，不能靠这类合法暂时失配重新立创新枝。

本证明只排除由两AW序倒置产生的那条依赖，不从AXI规范推出DDR公平、有限周期或整个IOMMU无死锁。更快/更慢TL可改变延迟，不能在这条路径上单独颠倒AW顺序。

## 1. 模型与四个事件序列

明确限制为：reset时payload有效槽为空；每DMA一次正常DEFAULT线性aligned copy；同copy源/目的有效byte对应；固定原AXI ID及bank选择；正常permission/translation、无error/abort/ATOP/MRIF/WRITE常量数据；所有copy source数据均经过本fragment准入，无另一条未经认证的R/payload入口；单global child；稳定VALID及FIFO不丢/不重复；HeaderPark0、CompletionGate0。

定义全局逻辑row身份 `e=(flow, perflow AW sequence,有效byte区间,bank)`，不能只用bank或owner。原input mux的首次`w_fifo_push`产生virtual offer序列V；physical AW真实握手产生accepted序列P。每层WLAST只退休其对应序列head。

- vw：V已offer数量，fragment `vw_q`。
- pw：P已accepted数量，fragment `pw_q`。
- vr：virtual WLAST已交入bank TC数量，fragment `vr_q`。
- pr：physical WLAST已交DDR数量，fragment `pr_q`。

source raw parent及准备cache不是payload事件。§42 top `988–995`明确virtual offer早于handshake的观测点，以及physical accepted AW、两WLAST的真实观测点。比较的是这个完整不可变序列，不能将已分别pop的两个剩余队列错误地要求始终相等。

## 2. 关键byte证书：读取前目的AW已physical接受

fragment `88–124`选择新的child时要求virtual/physical head owner与seq都匹配；逐项累加同owner、同seq、连续copy区间的两侧prefix；physical ring只在真实physical AW握手入队（`224–229`）。child起点必须等于virtual消费offset（`113–124`），长度不超过该prepared prefix、parent余部及固定上限。

因此，对任何新child内有效byte x，存在唯一所属目的row e(x)，且

`physical_accept(e(x)) < child_birth(x) < source_payload(x) <= virtual_W(x)`。

这里严格小于指事件先后；同步实现中physical ring要先成为已注册状态，随后candidate才可利用。sameflow perflow序号正确归属另由bank内FIFO保证，见§3，**不预先假设全局P顺序来证明这个byte属性**。

reset空payload与正常R→private FIFO输入保证：已有private、held R及TC W数据也继承其出生时的证书。不能用现在有payload来取消其历史上已经发生的目的physical AW接受。source原parent的RLAST恢复不增加额外byte；source child tail可继续返回，但它的每byte仍只覆盖出生时已接受的W row。

已核iDMA transport `265–266`：DEFAULT buffer input由真正R握手填入；prepared helper `103–107`只给PA/permission cache，没有payload口。因此提前翻译、raw header预收、缓存命中或物理读旁路都不能给未accepted新AW凭空供应数据。

## 3. AW顺序证明：bank内＋bank间

### 3.1 Bank内保序，同时消除perflow seq的循环论证

banked `291–319`的输入demux按不可变原ID bit选择，AW没有变换或独立绕行。每bank TC `121–139`的AW stream FIFO保存输入顺序；TC只处理当前head，`721–730`在COMPLETE等待`axi_comp_req/axi_comp_resp`的**内部AW握手**，随后令aw_ready、pop并回IDLE。

该握手在TC normal/error demux的slave端（TC `569–570`）；此demux的AW/W/B/AR/R spill全部开启（`558–562`）。所以TC pop可以早于外部physical AW接受。HeaderPark0/Gate0（banked `432–444`）原样转接bank completion，外层输入demux与输出mux的spill关闭（`305–309,609–614`），**不会消除TC内部spill，也不会把两次AW握手合成同一事件**。

正常路径中，每bank AW FIFO、TC按head处理和内部normal AW spill共同保持row顺序；输出mux只仲裁各bank已保序的AW流。更慢的当前head翻译会延迟后面的row，后续row也可以在旧row外部接受前进入内部spill，但不能越过旧row。

对同一个正常row，必要事件链为：bank输入AW接受 → TC处理head/译址完成 → 内部AW spill接受（TC可pop）→ 外部physical AW接受 → source child出生 → source payload到达 → virtual W交入TC → 内部W spill接受 → physical W交DDR。其中payload/W部分对所属byte及其承载beat分别成立，不要求全row数据都先进入TC才开始物理写。FIFO相邻事件可以同拍；§2严格要求的边界仍是**外部physical AW接受先于利用已注册physical ring的child出生**。内部AW接受、TC pop或内部W credit变化均不能代替这个外部接受证书。

每copy固定ID、同flow固定bank，因此同flow AW sequence无论其他bank如何延迟，都保持物理局部顺序。这使fragment的perflow `paseq_q`与真实row一一对应，byte证书不依赖待证明的全局顺序。不同flow同bank时，也受同一AW FIFO保序。

### 3.2 Demux一次只允许一个bank拥有未交齐的virtual W

vendor simple `175–181`只在w_open=0或目标bank等于当前w_select时首次forward AW，并在**first AW offer**增加w_open，早于AW READY。`154–163,196–198`保持已经锁住的AW，不能借预算门撤回。`243–253`根据first offer和真实该bank virtual WLAST维护w_open与bank；`429–435`仅当前w_select得到W。

所以不能在旧bank还有未交齐W时先让一个快译址的新bank AW走进它的TC。先锁bank再等AW接受比“只按accepted AW计open”更强，first-offer早于accept并不是漏洞。

AW输入可见时WREADY可以理论上出现，AXI路径没有要求所有W等AW握手；但这里新row的W payload不能在它的physical AW接受前出现（§2），而physical接受必然在该row进入TC之后。因此一般W-before-AW合法性没有给本受限DEFAULT copy一条可用旁路。

### 3.3 全局前缀，不能跨run越过旧row

将输入demux实际forward AW按bank分成连续runs。它改变bank前，上一个run的w_open必须归0，且没有旧held AW挡在输入；这意味着该run所有virtual WLAST已经交入旧bank TC。

由byte证书，每一个已交齐W的row，其physical AW早已接受。因此新的bank run能首次进入TC之前，前一个run的所有physical AW已经按其bank内FIFO顺序接受。新run无法在输出mux凭快hit越过尚未接受的旧run AW，因为旧run根本没有这种剩余AW。

run内按§3.1保序，run间按上述先后，归纳得到 `P=V的一个前缀`。输入muxAW spill虽然可以提前存后面virtual rows，却是FIFO，不让一个后row越过被demux挡住的head；它不改变上述run顺序。

同一论证还约束physical首次AW offer序：bank间新的offer不能比旧run的accepted AW更早；bank内内部normal AW spill保序转出，外层输出mux保持已offer但未接受的AW。TC current COMPLETE可能早已结束，这不允许后续AW越过spill中的旧row。不是只有最后accepted trace侥幸一致。

## 4. 精确夹序与合法head lag

每physical WLAST由TC中已经virtual接受的数据构成，因此pr≤vr。每virtual WLAST的所有byte有physical AW先接受证书，因此vr≤pw。§3给pw≤vw并保证这些count对应同一全局row顺序。所以在每个一致采样状态有

`0 <= pr <= vr <= pw <= vw`。

若pr=vr且两head都存在，head身份相同。若pr<vr，physical head是更早的同一序列row，其virtual WLAST已经完成。旧row的未写suffix全部在TC W FIFO、内部W spill或shared output链，**没有缺未来source或目的TL的债务**。TC W FIFO内的数据按w_cnt与内部spill READY继续（`225,364–365`）；已交入内部W spill的数据按输出mux/DDR READY继续，即使本地w_cnt已经递减。两段都不要求当前REQ_TRANS/AWAIT_RESP完成新的译址。正常下游持续获得服务时，该lag可排空。

明确反例于“head每拍相等”：A/bank1最后一拍virtual W被TC FIFO接受，而DDR暂时WREADY0；virtual head推进到V/bank0，physical head仍A。demux已可切bank，V AW也可译址并physical接受，所以可出现virtual剩余 `[V0]`、physical剩余 `[A0,V0]`。源child gate暂时等待，旧A末拍本来就在TC，WREADY恢复便可退A，随后head重新匹配。这个时序合法、无需额外source/TL，**不是P=[V0,A0]的倒置**。

若physical queue空且virtual queue还有未接受head，则pr=pw；夹序要求vr=pw。所有已授权source范围只覆盖旧accepted prefix，旧physical W全部完成又说明其source payload已到齐；不会有为了这个新未接受AW而held的正常data R。因而不能在该状态声称held source R阻住它的目的PTE，来再造已被准入排除的环。

## 5. 已有payload、spill及其他边界为何没有提供反例

- **Private payload/已在途source R**：不是自由库存；从reset归纳有对应physical AW已接受的byte证书。即使同一个child跨多个W row，它只跨出生时两侧匹配的accepted prefix。
- **TC W FIFO及内部W spill**：可使virtual退休早于physical退休。内部W spill还可以在external WLAST之前接受WLAST并使TC本地w_cnt递减。它们保存旧row全部余部，不会成为“用旧row数据填未接受新row”的许可；iDMA正常copy的有效byte序保持。
- **Input AW spill与TC内部spill**：input mux spill可让virtual first-offer观察早于demux进入，但FIFO不跨越head。本指定banked路径仅外层输入demux及输出mux的spill关闭（`305–309,609–614`）；TC normal/error demux各channel spill仍开启（TC `558–562`）。普通保序spill本身不创造payload或反转FIFO，只会改变观测/滞后边界；变更配置时应重新连接事件点，不能无证据外推。
- **W-before-AW**：demux逻辑确支持准备好的W先交；本copy数据供给证书使未physical接受的row没有有效W可交。不能将“协议允许”替换成“此状态从reset可达”。
- **背景cache/TL**：只保存normal permission/PA，不预装copy payload；译址快慢无法让AW绕过本bank FIFO或demux旧W run。
- **COMPLETE不等外部AW或数据退休**：TC在内部AW spill接受后就可选择下一个事务，可早于本row的外部physical AW接受，更不会等待physical WLAST。后续metadata与旧row外部AW/W可以重叠；AW FIFO及内部normal AW spill保序，byte证书则继续以外部physical AW接受为准。

本证明不覆盖：预装/另源payload、WRITE常量/计算生成数据、绕过source certificate的读、同flow bank/ID动态变化、fault/error/discard、重新排列AW的缓冲、残留跨job payload/seq世代混用、另一条没有纳入V的写入路径。若移除证书且给旧row足够数据、TC FIFO又容纳其全部W，可在旧bank慢译址时先交齐virtual W，再让新bank快AW先physical接受；那是改变模型后的具体反例，不是此平台现路径的见证。

## 6. 对本次自然佐证和研究枝的判定

本次自然dualbank run检查physical accepted AW global index/owner/seq等于对应virtual offer row，另记录pr<vr及首个head lag。观察assert不驱动调度；发现lag不报倒置。root报告：一次build/run、mode2/Vfirst/各32KiB完整copy PASS；34笔physical accepted AW全部匹配virtual first-offer序，head lag共64周期、owner lag16周期、seq lag64周期，max(vr−pr)=1。这支持AW顺序与W退休滞后确为两个问题，但单次通过只为源码不变量的有限佐证；本席未重跑或独立复算这些运行数。若出现顺序反例，应以完整边界state检查证书或FIFO假设哪里不成立。

没有找到需要为了“两个独立W序交叉”新增规则的可达见证。**因为现demux bank run加ordinary prepared-prefix供给已排除AW倒置，而head lag已有独立旧payload可排空，不能把该组合重新描述成未解决的两序调度创新。** 只保留普通有限排空与资源/等待成本问题，不额外人为制造脱离正常copy供给的W。

必要进度前提是TL正常完成、真实W返回路径获得服务、counter/FIFO不溢出且VALID稳定。该不变量排除AW顺序环；它不证明任何AXI DDR slave的R/W服务都不互相等待，不证明任意PTW/互连拓扑、无限其他流量的公平或固定周期界。

## 7. 直接核查导航

- [Vendor demux单bank W run](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_demux_simple.sv:175)：175–198 first-offer/lock；233–253 w_open/select；429–435真实W路由与LAST。
- [实际banked外层无spill/无park路径](/home/xvhaocheng/work/iommu-platform/tests/nonblock_completion_banked_ctl.sv:291)：291–319输入demux；432–444原样completion；592–623输出mux；634–635真实AW转出。
- [§42 source byte证书](/home/xvhaocheng/work/iommu-platform/out/committed-write-drain-20261008/compact_pretl_owner_fragment.sv:88)：88–124匹配已accepted连续prefix及源offset；224–240 physical行与退休；244–273 child出生/归属。
- [TC FIFO、内部spill与独立W流](/home/xvhaocheng/work/iommu-platform/out/natural-write-order-20261008/trans_ctl_write_limit.sv:121)：121–139 AW FIFO；225–259 W FIFO及credit；364–365 W输出；558–574内部normal/error demux及全channel spill；721–730 COMPLETE内部握手。
- [DEFAULT R→payload FIFO](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:265)：265–266。
- [cache只给PA](/home/xvhaocheng/work/iommu-platform/out/committed-write-drain-20261008/hit_bypass_parent_translation.sv:103)：103–107。
