# §46 Codex关键推演：共享缓存与请求进度并非天然同槽

Codex研究席亲自推演、直接核关键来源；无DeepSeek/Claude调用，无RTL修改、build/run或§45重跑。root负责当前真实映射/模块证据，map席负责primary汇集；本文只给成本明确的反例合同及强普通击杀。导航：[§45合同](../translation-progress-frontier-20261008/codex-next-decision.md)、[map交叉审查](map-critical-agent.md)、本文§2字段反例/成本、§3有限服务与在途义务、§4真实history边界、§5停止裁决。

## 1. 先分三类，而非把progress全部称为不可丢

**已完成的cache记录**提供以后少读PTE的机会。静态正常且无A/D副作用时，驱逐它通常只改变重读/性能，不删除一个已经接受请求的归属。若合同要求零重读或固定工作上界，保留它成为该性能合同的需求，不是AXI强制它永久住在同一数组。

**已处理checkpoint**保存request的已完成semantic进度。若要求精确恢复且不重读，它引用的全部字段必须保留；若允许restart，可以合法丢一部分，但必须保存restart根与所需parent，计重读，并提供真正推进logical request的持续服务。checkpoint不全都是不可撤的I/O义务。

**first VALID/已accepted、未退休的PTE请求**带有不可撤的传输义务。VALID/address稳定、响应路由、owner/target、错误判决归属必须保留或转移到等价有计费结构，直到正常退休；不能直接删除并称之为cache miss。响应已合法收到之后选择restart，与提前丢掉唯一返回关联是两回事。

这三者可共用一个pool，也可分别放入cache、per-request frame、MSHR/message/response queue。总字节和端口当然有限，但它们**并非语义上必须争用同一组记录**。若生产本来分开，先新增unified slot再展示冲突，只说明选择的布局有冲突。

## 2. 一项具体反例合同Γ：共享G-root结果不是共享whole continuation

### 2.1 合法输入与允许/禁止信息

在正常Sv39 S×Sv39x4 G路径中，两个已接受请求A/V都读完S1 nonleaf，再完成当前G-root读取，停在同一个受限phase `I(Slevel=2,Glevel=2)`。S1返回的下一guest table PPN分别为 `T_A=1`、`T_V=512`。它们都是29位合法T，高11位相同为0、低18位不同。

可用相同S-root下两个不同S1 index（例如原IOVA为0与1GiB）构造这两个S nonleaf返回值；共同G-root/context为这两个target提供合法正常映射。G-root索引是GPA[40:30]，即T[28:18]，因此两者读取同一个G-root PTE，得到同一个下一物理G-table PPN Q44。但后续地址为：

`G2Address_A = (Q << 12) | (T_A[17:9] << 3) = (Q << 12)`；

`G2Address_V = (Q << 12) | (T_V[17:9] << 3) = (Q << 12) + 8`。

之后G3还需要各自T[8:0]。T来自已退休的S nonleaf，其值依赖memory image，不能由immutable原IOVA、root或C本身重建。把T写进所谓原请求/context，或保留最近S PTE而不计它的存放位置，都不合法。允许使用C的真实原请求、roots、context身份、访问类型；允许共享正确key的Q；禁止oracle、隐藏进度、额外active副本和免费transport Info。

Γ要求：静态正常、有限两个borrowers；暂停后恢复时，正确发出各自下一G2/G3地址，不重新读G-root；对引用期间的共同record维持正确key；保留独立请求归属。此Γ是一个具体恢复边界合同，不冒充整个nested请求的所有fault/大页或持续到达latency定理。

### 2.2 最少足够的明确普通字段

partial translation cache的共享记录可以是：

`P = {G-context/generation key H, target-prefix11, Q44, valid1, owners[A,V]2}`。

各请求自己的记录是：

`D_i = {T_i suffix18, Sglobal1, header_i τ}`，C另行双方同计。

τ包含本实现实际需要的phase/level、归属/有效状态等；本受限phase中若某字段为常量，可双方同时省去。由于这里只一条P，owners位图已经给关联，不再收一个虚构的row pointer；扩大到R条时要计`log2R`引用或实际owners bitmap，双方选相同关联表示。

于是这个直接构造的可变结构上界为：

`|P|+Σ|D_i| = H + 11+44+1+2 + 2(18+1+τ) = H+96+2τ bit`，

另加双方同样真实的C、active执行寄存器、在途AR/响应、terminal路径和allocator/lookup端口。它是Γ的普通充分上界，不是minimum codec或area。Q与prefix共享一次、suffix/global逐请求独立；没有将地址key与progress重复收费，也没有借C藏suffix。

若candidate只保留P、声称一份cache记录能恢复两个whole continuation，它在上述A/V就无法选择正确G2地址。若candidate补上D，它已经是普通`one result+per-request Info`结构。**主动击杀Γ的候选优势：** 普通分槽P与D、P在live reference期间pin、最后引用释放后转best-effort cache，使用完全相同字段和位数，正确实现Γ。动态pool也可保存相同P/D union；不需要新调度概念。

### 2.3 Pin为何不能自动替代其它状态

若P被驱逐而D只有suffix18，连partial G restart所需完整T29都不再可重建。因此三种选择都要明确计费：

1. 保持P的prefix/Q受保护，继续精确恢复；
2. 将prefix11与必要Q44复制入有容量的private frame，再取消P引用；
3. 若允许partial G restart，只需先让完整T29和Sglobal/phase留在私有或其它有计费结构，可以丢Q；若连T也丢掉，只能按合同从更早的S-semantic根或whole请求重走。

缓存与checkpoint的保护范围取决于实际保留信息，而非record被命名为cache还是progress。一旦两个请求推进到不同Q，各自新Q不能因为它们来自同一个旧Q就共享。若同时要求永久保留旧Q供未来复用、保留paused新Q、以及active另一个新Q，三者可以独立；任意编码都必须保存可区分这些信息或允许重取。这是普通存储守恒，不是新机制。candidate与ordinary都不能把已淘汰的Q当免费缓存继续命中。

固定分槽可能浪费当前未使用的一类容量，但强普通对照允许动态借槽、pin/ref与private sidecar。只打败“每逻辑对象整占一个slot、不能alias、不能借槽”的固定切分，不能证明独有收益。

### 2.4 context、权限与fault边界

相同IOVA或相同Q值不是partial mapping的身份。缓存key至少必须区分实际G-root/context、地址prefix、translation mode及有效代；静态合同可简化generation，双方同时简化。不同S-context可以在同G-context与同target-prefix上合法共享P，但仍保留各自S-level/global和返回身份。

若只是合并**相同physical PTE bytes**的fetch，许可可以比复用完整mapping更宽，但每个请求仍独立解释PTE、权限与fault。不能用读请求的successful verdict替代写请求的检查。反过来，不同context即便physical地址偶然相同，也不能在没有正确memory/版本/访问语义证明时合并最终translation verdict。

真实PTW未来fault又强化独立target需求：S高PPN leaf错误将stage置INTERMED，bad GPA使用保留下来的gpa_x；完整G target也参与后续G fault归属。F phase的Sleaf属性不是G-cache权限。本文Γ固定正常阶段所以不将这些额外字段塞进96位；推广时必须补字段，不能拿Γ成本免费宣传完整fault合同。

## 3. 原作给出的强普通结构及持续进度条件

### 3.1 NB-Walker具体实现，而非泛泛cache比喻

固定作者源码commit `8837da7dda5591837877d20d4ad51b042c988aad`：[作者仓库](https://github.com/redbird-arch/micro2026-nbwalker-artifact)、[作者发布artifact v2](https://zenodo.org/records/21488827)。只核所列源码，未取得论文全文，不能声称作者证明本文nested fault或per-DMA fairness。

* `NBWalkerMMU.go:314–340`：每请求`NBWalkerBlock{PID,Address,PPNWithOffset,Level,MsgID}`进write.Info，成功发送才清本地secondary。
* `walkerStage.go:118–140,194–210`：以PID/PTEBlockID查共同MSHR，独立Requests聚合，最多8个waiters。
* `respondstage.go:47–61`：每请求返回其原Info，成功发送才移除transaction；`NBWalkerMMU.go:558–580`按独立Address/PID/level恢复。
* `walkermshr.go:17–59,63–78,106–120`：data与PTW共用entries，key又区分PTW；`directory.go:173–179,260–266`普通data准入在PTW预约阈值被拒，PTW路径只查IsFull。

本席直接核上述关键片段。固定原件及SHA/URL在[本轮primary目录](primary/nbwalker/)与[来源清单](primary/sources.json)。raw网页抓取本轮cache miss，源码证据来自map保存的作者repository固定commit副本；Zenodo发布页本轮正常可访问。消息/MSHR/返回队列里的Info全部仍是实际存储，不能将状态搬离walker等同于状态消失。

这已经具体展示共同fetch与独立continuation，以及共池上的类别capacity保护。其reservation是PTW-vs-data，不能直接宣传为不同DMA都公平；Go对象/slice和TrafficBytes也不是硬件bit成本。

### 3.2 有限到达与持续服务不能混合

Γ仅有限两borrowers，且可用完整checkpoint或合适的正常语义chunk推进；有限memory服务和确定的正进度调度足以使每个borrower最终释放引用。只保留S-semantic parent又反复过早暂停仍可无logical progress；§45已经证明这一普通反例，本轮不重跑、不将其立新。

持续到达下，最多K个waiters本身**不保证某shared record最终unpin**：旧waiter移除同时总有新waiter加入，可以使reference数永远非零。可用普通封闭cohort、限制下一cohort准入、先materialize到有容量的private frame，或保留足够独立progress容量。所有waiter/ref/队列/服务配额均计费；同时要给admitted请求数、最小服务与arrival合同，才能给bounded progress/latency。

正常TC的translation response恒ready，本轮没有慢subscriber把结果锁住的结构证据。不能人为新增consumer背压或永关budget，再宣称这就是当前平台不可避免的cache-progress循环。

## 4. root新增真实可达边界：共享命中不等于requester历史隔离

本席定向直接核当前源文件`rv_iommu_iotlb.sv:40,47–57,132–142,384–401`及`rv_iommu_tl_wrap.sv:271–277,670–679`：IOTLB lookup没有DID/class，不以fill policy mask限制hit；**实验policy已启用、16ways且mask为固定partition之一**时，PartitionLocalHistory touch以resident way是否<2选择local nodes。默认参数PartitionLocalHistory=0，不能将本段外推为默认生产的QoS合同。fast lookup与main lookup是mux出的不同请求，而fill policy mask属于稳定main请求。

因此root给出的合法DID10/11、共同PSCID/root、相同IOVA且正常权限合同确实允许A命中V protected ways并touch V local PLRU。原A/V分别PSCID10/11且无global的fixture没有激活这个alias；不能据旧fixture将“physical-way分历史”扩成“V requester历史不受A影响”。本席又直接核`tests/composition_iotlb_isolated_test.cpp:570–576`：S3明确调用`touch_hit(A_MASK,vT1)`并期待node7改变。**旧合同就是resident-way history；这个期待由旧测试覆盖，不能将新requester合同称为旧实验失败。**

两protectedways的纸面反例：V最近访问way1，故下一V replacement本应选way0；A命中way0后若按resident way touch，其下一V victim改为way1。条目内容可以保持完全相同，差异来自A requester的history事件。这证明两个合同不同：

* 如果原合同只按resident-way节点拆PLRU，A hit V way更新该节点符合该合同，并非实现bug。
* 如果合同要求V requester的history对A事件noninterference，这个alias是实际可达反例，原定义/接口不足。

root的不可区分对进一步证明现接口不能自行猜class：main A的正常alias hit，与main A忙时fast V的同key hit，可以同时给IOTLB相同`mask=fffc/VPN/PSCID/touch=1`及同stage/无update输入，但前者正确class=A、后者=V。IOTLB当前输入没有DID/class或对应lookup origin，确定逻辑对这两种输入必做相同touch。仅按main mask过滤会错误抑制fastV；至少要加入区分这两种实际lookup来源的信息。二分类可以是一位class sideband，但解码来源及所有必要pipeline存储均需计费，不能把“fast”本身永久等同V。

最强普通修正明确且可复现：从**当前实际lookup**传入requesterclass，用`hit requesterclass+resident eligibility`过滤touch；lookup命中/内容仍可共享。fast hit用fast原请求真实class，main hit用main请求class，fill仍用main稳定class。fastV在mainA占用时不能用main fillmask误判；同bank亦不能代表同DID。candidate同样需要上述真实class信息。

该规则还有精确技术前史：[US7415575B1《Shared cache with client-specific replacement policy》](https://patents.google.com/patent/US7415575B1/en)的Fig7 step702/706/708接收实际client ID并仅在restricted requester命中时更新restricted历史，claim6明确写该分支；其他client仍可命中共享条目。本席直接核本轮primary文本`:744–771,859–865`与网页原文。原作不要求strict LRU，也讨论各client subset，所以换成local PLRU/gate不消除覆盖。它不是本文已实现的fast/main接线，但直接覆盖requester-specific共享TLB历史的规则。

这给一个具体的工程QoS合同差异，但无需新的调度/缓存表示。过滤touch不能单独保证端到端latency，也不处理A准入/填充/失效可能造成的其他变化；不能把局部PLRU不受A touch作为全系统保证。旧隔离测试已观测resident-way primitive，真正DID alias经完整TL产生的新路径仍只有源码/纸面构造，本席未修改代码或运行。由于无创新差异，不为此扩大DUT实验。

## 5. 接受与停止

可接受的结构事实：缓存复用单位可小于continuation状态；正确恢复可以由共享P与独立D联合提供；在途route不能静默丢弃；共享record的pin具有实际容量代价。这些都有明确字段例与NB-Walker具体普通结构，不是独立新机制。

因为Γ的candidate补足suffix/owner后，普通P/D分槽、pin和动态checkpoint能用相同字段、相同容量得到同恢复/共享结果，**停止把unified cache-progress alias或slot争用当独立贡献**。因为有限已完成cache与live progress在当前平台是否共池尚待真实接口证明，不能从新增统一槽推出当前生产瓶颈。因为requester-history alias的强普通hitclass gate可同效，**把它定位为已有隔离合同的工程修正**。

root已用原测试、原文/源码、不可区分对和两way纸面轨迹完成本轮最小判别；允许普通gate/分槽后没有独有差异，**本枝收尾**。requester class若以后作为工程修复实施，验收必须覆盖main A alias和main A忙时fastV两来源，而不是重跑既有isolated primitive冒充完整TL alias实测；本轮不实施，不做新RTL/BIT或阈值扫描。未证明任意joint编码的最优容量、连续arrival latency、完整nested fault实现或综合面积，本稿不以这些未覆盖点补新颖性。
