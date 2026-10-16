# §42 新控制的Native Codex源码审查

2026-10-08，`innovation_rules_40`。直接读本目录design contract、五份新SV及与§39的差异；只写本审查稿，未改RTL、未构建/运行，未扩文献。C++尚不在本次review范围；root执行实际验证。

## 结论

**在contract指定的一次正常aligned copy/DMA、CONF1、同bank、mode0 private raw捕获、单global child及既有prepared-prefix保护内，未找到阻断仿真的新并发错误。** 输入AW保持门、child birth gate、prepare eligibility及透传可以进入本次有限pause判别。这个结论是源码审查范围，不是DUT通过或通用有限周期证明。

**成本验收阻断项一处：** top `623`称下面所有counter为observation-only，但`dbg_pause_a_end`是64位寄存器（`656,670–672`）并反馈raw-parent admission（`647–649`）及new prep eligibility（`1022–1024`）。它是功能性frontier state。因此本实现新增功能state至少含 `pause_aw_held_q` 1位＋`dbg_pause_a_end` 64位；不能按只有1位控制增量出成本结论。其余counter/saved header目前只有观察、断言/quiet报告用途；若以后把quiet反馈控制，相应状态也须重新归属。更正注释/账本即可，不要求为此重做架构。

## 1. 输入AW门通过的具体范围

top `629,663,715,722`实现了：新AW在pause时关闭；上一拍门后AWVALID且无READY的请求仍可继续；门后VALID和返回A的READY同时受allow控制。`660–665`检查已有held请求的全header不变。

这覆盖两种容易漏的状态：RR已锁而尚未`w_fifo_push`，以及已push而mux尚未接收。已接受进AW spill的请求不依赖门保持位，旧spill、W/B和下游AW都没有被新pause门切断。A门前尚未呈现给mux的新pending AW可保持在DMA私有寄存器中，READY0，不会被伪握手弹出。

read gate仅新增在fragment `244–246`的candidate锁存条件；没有修改`m_ar_valid_o=offered_q`、已offer header或busy RREADY。计数器 `695–699`复制同一birth条件，所以child数量/byte计数对应首次创建，**不是physical AR handshake计数**。

源码还没有一个独立sealed寄存器；这是观察协议而非该门功能缺陷。C++必须按contract在关闭已生效、gate-held清零且最后一次push更新已纳入的状态采sealed snapshot。t0同拍或held晚push可合法提高offers/end；不能把t0旧counter硬固定后认定违规。

## 2. quiet等式为什么覆盖first-offer及TC残留

当前quiet（top `638–642`）要求：

```
gate-held_A = 0
virtual_A_offer_count = physical_A_WLAST_count
virtual_A_offer_count = virtual_A_WLAST_count
physical_A_Bpending = 0
A_child_offered_or_busy = 0
A_background_prepare_offer_or_slow = 0
```

在本配置，一笔A destination从virtual首次内部offer就计入`i_pre.awseq_q[1]`；即使仍锁在输入mux/spill、在TC header等待翻译、或physical首次VALID但还没AWREADY，它的physical WLAST尚未到达，第一组等式就不能成立。因此不额外拥有physical first-offer counter，也能通过更早的virtual obligation count覆盖该行。其后下游physical AW是在推进旧债务，不是新的virtual admission。

虚拟WLAST在TC W FIFO接收时可早于physical WLAST；第二组等式单独不足，但与第一组结合排除了A尚留在TC W FIFO的有效payload。全部对应physical WLAST完成并且没有A新行时，不存在仍需继续转发的正常A W区间；无需要求V队列一起空。pending B又把目标加强到这些accepted A写响应也已消费。

source child是全局唯一；normal/direct AR及其TC译址、header/physical R仍在child `offered_q/busy_q`的生命周期内，直到source child RLAST才清除。因而`633–634`同时观察offered和busy、按锁存owner归属，覆盖了physical AR未发出及已接收后的A source。保留未offer raw-parent余部不会触发该busy，它在本接口是每flow私有metadata，允许留存。

这些推论依赖正常无error、每virtual行只推进成一笔对应physical AW/W、既有序号与byte守恒guard仍有效。不能把它移到有abort、异常W丢弃、另一个untracked source队列或interleaved response配置后继续称充分。

quiet本身未AND `pause_a_i`，reset/普通空闲时也可为真；C++必须仅在pause已request且仍有效的窗口解释它，并在quiet后保持pause的一段V服务中确认A计数不再增长。它不能自行充当永久冻结完成信号。

## 3. R/W/B计数路由审定

- R bytes：`691–693`取`pre_raw_r_valid && pre_r_ready`。fragment把ready限定为busy且DMA返回链ready，因此不会计入没有child的PTW或其他R；normal/direct选择与锁存child owner一致。在当前mux SpillR0且aligned全beat copy下，这是交给iDMA的8B有效payload。
- Physical W bytes/WLAST：`676–680`用`i_pre.p_owner_q[i_pre.pr_q]`，**没有用同bank输出mux端口号冒充flow**。该ring在真实physical AW接受时记录`comp_aw_id[2]`，在真实physical WLAST推进，符合本已有prepared-prefix admission：source不能为尚未physical接受的下一AW生成payload，因此这里有已建立的P head。
- Virtual WLAST：`673–675`用真实virtual W握手及当前v-ring owner，匹配fragment同拍处理前的head。
- B pending：`643–645,682–690`用真实physical accepted AW ID及B ID的bit2归A，双事件同拍不改计数，单B时检查下溢。在本正常AXI链中accepted AW只进一次、B只消费一次，这对应本contract更强的响应排空。
- Child bytes：在首次child创建时增加candidate_beats×8，和read gate条件一致；可用于把already offered remainder与关闭后新建child区分，但不要重命名成“新接受physical AR数”。

每拍8B计量仅适用于本aligned/normal、长度为8B倍数的负载。partial strobe、source realignment、其他交易类型不是此代码已验证的计量范围。existing `p_wlast before accepted AW`/ID/length/mask/owner guard应继续保留；不能单用counter代替它们。

top `701–704`读旧counter检查，能在下一拍观察违规；最后终止须继续有完整scoreboard/检查拍，不能以该assert替代terminal byte/guard验收。

## 4. Raw parent及prepare mask通过的具体范围

raw gate（`647–649,709–717`）把A必要性按parent起点<当前committed end判断，允许parent末端超出目标而私有保留。返回ARREADY只给实际被选wrapper接收的flow；mode0入口的valid/credit-ready配合没有被另加held共享AR，不能吞请求。

晚held AW尚未push时，end仍是旧prefix，所以新需求的raw/prepare可能暂时被挡；此前已经push的有限W债务仍可服务，腾出owner slot后held AW真实push扩end，后续需求重新eligible。这是有限等待，不是需要提前虚增end的协议问题。本结论利用CONF1 R独立前进；coupled模式另审。

helper新 `prepare_eligible_i`只加入`147`候选条件；未改变parent validity、cache validity/reuse eligibility、已有`prep_offer_q`输出、accepted `slow_q`或response ready。top `1022–1024`在pause时只筛A的无关新准备，V保留资格；banked `814`原样传给helper，system `1156–1162`将新pause/debug端口完整透传。

prepare busy（`635–637`）只统计background offer/slow，使用已锁存`slow_owner_q`。这避开formal非起点请求owner可能不可靠的问题；foreground source TL由offered/busy child覆盖。新eligibility不会取消旧background offer，旧A准备可在pause后合法结束并延后quiet。

本负载达到全部W完成时，所需parent的source段已返回，对应正常TL结果已处理/缓存；未完成的parent suffix允许留存。若在其他配置允许quiet后仍产生eligible但无关的新A准备，则需要更严格mask；当前单job/static正常处理与data-prefix约束没有给出这种见证。

## 5. 必须由本次执行核的边界，及不扩大的通过范围

root实施后保留完整scoreboard，并一次核：关闭时gate-output held请求稳定、t0/late push纳入sealed end、mode1只停新child而旧child完成、mode2能补同jobsource、首次quiet仍在pause内且A job unfinished、quiet后A计数稳定且V继续服务、resume后两copy完整完成。C++snapshot必须明确是pre-edge还是更新后状态，尤其last held handshake和last WLAST同拍。

所有新observer计数仅reset清零，而`i_pre.awseq_q/pa_q`会在job_fire时清零。因此**本审核只通过每DMA一次copy**。若runner在同一次reset中给A多job，累计WLAST/R/W与perjob offers会失配，end/counter也跨job；须先增加明确job边界处理。它不是目前contract的一次copy blocker，但不能静默扩展测试。

此次读源没有发现要求停止构建的RTL并发缺陷；需要更正64位frontier的功能状态账本。没有执行验收，所以未确认mode2能在10000-cycle窗口内quiet、没有确认最小future R byte等式，也没有证明硬实时或四DMA进度。

因为该控制就是已held保持、ordinary prepared-prefix服务及有限debt排空，本次通过只能验证强普通构造，不提供独立机制新颖性证据。

源码导航：[新top控制](/home/xvhaocheng/work/iommu-platform/out/committed-write-drain-20261008/multi_actual_dma_top.sv:624)、[新child gate](/home/xvhaocheng/work/iommu-platform/out/committed-write-drain-20261008/compact_pretl_owner_fragment.sv:244)、[新prepare mask](/home/xvhaocheng/work/iommu-platform/out/committed-write-drain-20261008/hit_bypass_parent_translation.sv:147)、[banked透传](/home/xvhaocheng/work/iommu-platform/out/committed-write-drain-20261008/multi_banked_top.sv:814)、[system透传](/home/xvhaocheng/work/iommu-platform/out/committed-write-drain-20261008/multi_system_top.sv:1156)。
