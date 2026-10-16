# §65：在旧physical burst末端接合的独立关键审查

Codex直接读本目录design、shared_pretl_read/multi_actual_dma_top/multi_system_top三份变更SV和q_write_splitter。只写本稿，未改RTL或日志/CSV，未构建、运行或复制root统计；结论属于纸面及实际源码审查，不是DUT/board证明。

**版本说明：§1–6保留build01初审及其奇数端点反例，不表示最新build02仍有相同缺陷。§7–8为直接重读build02修复后的新接受结论。** root报告build01最初18个even场景均PASS且快照封存；这些结果不能证明old build01支持odd K。root报告build02两个compile参数均构建成功，奇数量子新矩阵由root执行；本稿没有执行或提前认定其PASS。

**build01限定接受：mode3、A原cursor0、同reset单job/static normal immutable、独立READ/双页WRITE全成功、keep-q1/q2/private B3、旧E偶数且E<256时，新phase1的V准入例外能持续达到J=E，并保留原AW/R/W/RLAST义务。存在明确未守卫的奇数E反例：`ClosureAtBurstEnd`直接覆盖J后，既不检查旧promise<=E，也不把新V qchild截到E；当时read_quantum仍允许任意1..256。故不能把build01参数化分支宣称支持任意合法K。** 计划K2/16/64/128都避开这个洞，但需要合同显式约束或相应门，不能由偶数实验外推。

## 1. 新逻辑真正改了什么

- source `:97–101`先计算旧minimal J=ceil_even(max(rV,promiseV))，随后ClosureAtBurstEnd直接把它覆盖成`child_offset+child_beats=E`；`:103–107`仍只检查busy/noSkid、原parents/READ成功、双WRITE cert256及J<256/J<=E。覆盖后J<=E是恒真，**没有保留旧的J>=promiseV不变量**。
- system `:81,:1159`→actual top `:49,:973`→source `:7`正确透传这个compile-time选择，没有新增phase/J/payload寄存器。
- top `:724–726`仍先检查当前真实WRITE recipe；仅phase1的V新child `q_next_end<=v_cut`可以在payload尚未齐时越过source_v_hold。A没有该缺payload例外，已有full-payload例外保留。qwriter `:83–89`才注册child；现有offered/W路径及held稳定检查 `:65–70`不依赖新hold，不能撤回。
- phase3原A prefix裁J/peer实际W游标门 `top:710–719`保留，source `:130–133`也保留§59修复：phase3有成功授权的末拍允许有限private停车，不要求未来actual AW先建立coverage。

这个phase1例外对两种J选择共同生效，不能把相对于旧§59的全部变化只归于J=E。两极比较应使用本轮同一实现除compile参数外一致的配置。

## 2. 偶数E下的firstVALID和phase1闭包

先限定V唯一active source recipient：mode3在触发前source solo builder `:284–299`只服务V，A data cursor0；`:235–238`触发时对此断言并置detached。A首actual AW仍可私有捕获并提供真实WRITE key，READ0及WRITE1/2成功不是假数据。晚加入前PA/cache/prot/qos/region/user检查 `:228–234`保留。

V原actual WRITE是aligned、总256拍，从0按q2切。单child/B后再发意味着每个V新child的start为实际W前缀的偶数端点，end=start+2。触发要求busy且noSkid：当前旧physical RLAST还没真正接收，故`rV<E`；W不能消费未收到payload，`wV<=rV`。若E偶数，当时active q2 child的start最多E−2，end最多E；若没有active child，新start同样不可能已达E后再承诺。因此在这组窄不变量下，既有registered/held promise确实<=E。这个证明依赖V网格从0且一直q2，不能自动推广到任意量子、任意V页尾或odd E。

start_cutover同拍 source_v_hold=1（`:91`）、source Rready阻断（`:138`），phase1例外此时尚未生效。即使同拍qwriter创建一个新V child，原full-payload例外也要求next_end<=rV<=E，故不会引入越过已选J的新promise。原heldAW已计入qwriter promised_end（`:31–32`，child_live包含registered offered）；global mux尚未选择不意味着可抛掉。既有heldR不修改数据，只降低ready；触发noSkid避免换段时遗漏logical pending recipient。

进入phase1以后，V每个新child只要end<=E、当前WRITE授权有效便可提前firstVALID。R只需送V，physical旧AR又已不可撤销且包含到E的所有beat，缺payload child可以靠它继续填；不会被A新缺payloadW置于前面。A在mode3初始rA0无payload，不会由full-payload例外发新W。V单flow旧W头长度2、权限全成功，正常最终服务可以退W/B，再注册下个q2，有限步达到rV=wV=E。没有每次都等待payloadresident后再译址的额外门。

关闭条件 source `:244–246`同时要求noSkid、rV=J、wV=J、promiseV<=rV。因此V当前child数据已全部离开DMA private FIFO，即使其B仍在途；不要求父job/所有B退休。旧B在qwriter `:100–109`独立兑现，下一新qchild仍须等B，不能误读成取消B或释放parent metadata。

## 3. 奇数E的两个最小机制反例

**A：已有不可撤q2越过E。** K1的第一旧physical child E1是当前入口合法值（source `:174`只限制1..256）。V原qW `[0,2)`可在首source AR接受前已注册/firstVALID；A WRITE2已成功而cursor仍0。start_cutover强制J1并允许触发，但promiseV2>J1。phase1最多给V R0，旧qW还欠R1，wV不能达到1且promise<=r也不能成立。old physical child只有这一拍、下一AR builder在phase1关闭；它无法自行恢复。这个例子是直接依据当前握手门的纸面可达构造，未在DUT执行。

**B：即使没有越E旧promise，也无法在odd J造最后短W。** K3/E3，触发时rV0、promiseV2。phase1给V R0/R1，qW `[0,2)`全部完成，随后R2可借现有private槽并使旧physical RLAST/逻辑rV达到3。V下一candidate仍为 `[2,4)`：top phase1准入例外不满足end4<=J3，payload也只有到3，故它被hold。phase1未把child_quantum裁到`J-start`，所以无法造 `[2,3)`短child，wV停2；关闭条件要求wV3，永远失败。无需人为不公平memory或未成功的权限。

若希望支持odd E，至少须：触发前保留`promiseV<=E`（越界held promise不能事后裁短）；并将phase1 **尚未firstVALID** 的V新child截到E，或只在对齐的E选择该分支。只增加promise检查不能排B，只允许新的短child不能排A。当前计划只测even K，可把ClosureAtBurstEnd的even endpoint限制明确写入合同并拒绝其它值；未来实验不应隐含支持odd K。

## 4. 真physical RLAST、odd A页尾、E256回退

J=E时不会sink `[J,E)`，但仍必须真收旧physical RLAST：source `:307–317`核child ID/last/正常resp并只在真实last握手清busy，`:313–315`旧最后拍仍先入logical skid。phase1在该拍收到后通常busy0但skid1；`:244`等实际logical返回和W consume/noSkid才phase2。下一周期phase2 builder `:248–260`发现offered/busy/skid全0，即可造A原READ recipe从FA到256的repair。**此时不必再等一个RLAST事件**，旧LAST已经真正退休；也不能跳过busy0检查、把仍held的physicalR当已sink。

E<256的旧physicalLAST不是原logicalLAST：source `:146–147`从原parent len255生成logicalLAST，`:319–323`才按真正logical握手改returned/done。V保持原R metadata等repair的[J,256)，A从原cursor0补完整parent；ID/连续游标检查保留。没有拿新physical RLAST提前完成原DMA job。

Native A的odd F不会改变V在phase1的偶数网格。若F<J，A repair prefix跨首actual WRITE parent，经真实原B捕获AW2继续；top `:713–715`把尚未offer的最后prefix qchild截到J，允许q1。若F>J，joint以后在F发生A q1/V q2错位，不能声称始终同网格；安全仍依top `:718–719` joint新start<=peer实际raw_written与source phase3终端parking。尤其F255的旧反例排除门保留，不能回退到“logical RLAST必须等AW2 coverage”。当前两页WRITE都成功、current key按actual页选择，A terminalR先于首parentB不改变最终job对最后WRITE B的依赖。

E256时强制J256，source `:107`拒绝触发，phase0继续原V到其真logicalRLAST，随后原mode3 builder服务A；没有phase1/2/3、没有真实shared尾。`cutover_seen`应为0，J/oldend观察寄存器未必记录256，不能把未latched debug0解释成job-length0或伪称handover完成。这个安全fallback仍履行已有AR/AW/W/B，原state不会撤VALID。

## 5. 相同R工作量只说明什么

令N256，触发时A逻辑前缀FA在repair开始前不再增长；此前物理源请求组成连续[0,E)，没有额外独立A读或重复读取。旧child不能撤，任何合法J都真实接收其全尾到E。随后A独立READ recipe发[FA,N)，其中[FA,J)给A、[J,N)给两member。总physical source R为：

`R_total = E + (N-FA)`。

内部minimalJ会discard[J,E)，boundaryJ=E会先实际交给V[J,E)，但它们都在A repair中再次读这些地址，所以**仅在两次run的实际E、FA以及所有此前物理读取一致时**，物理R相等。当前mode3断言FA0，成为256+E；例如E112总368。不同许可完成时刻/选择门可能让trigger落在不同旧child，E就不同。E256 fallback则实际V/A各N，共512；它虽等于公式在E=N/FA0的数值，却不是一段已发生的handover。

相同beat数不保证相同AR数、cycle、V/A done或Pareto优越。对同E状态，interior phase2可无条件吞旧tail，通常能更早释放old physicalR并开始A repair；boundary需要V真实FIFO/skid/W准入/多个child B服务到E，会使A起点推后，但V更早取得并消费[J,E)。后续两个flow qchild/B能否重叠、native F错位、held R/READY间歇和DDR服务决定哪项latency最终占优。这些没有在计数恒等式内。

仅给“正常最终服务”时甚至没有可推出的uniform cycle bound：两个策略产生不同时间的AR/AW/W请求，合法有限READY间歇可以使一个赶上服务窗口、另一个错过；公平只保证下一窗口最终出现。因此不具备从R恒等式推出任一策略对所有合法服务的V/A Pareto保证的依据。若要更强支配结论，须固定额外memory/B/READY模型并证明，不是以同流量自动认定。当前root的同条件实测应分别报告V/A/mean/makespan，不以一个metric替其它。

## 6. 普通控制和创新边界

J=E是普通burst-boundary catch-up：先兑现不可撤旧burst的V数据，再从A游标补读，A追到E后共享尾；拥有相同READ/WRITE cert、原parent游标、qwriter/peer门即可复现全部动作，增加compile selector不增加状态。它没有形成新的翻译授权信息、减少此次R总工作量或新的held endpoint撤销能力。因此应把它保留为最强实际普通比较，不把端点选择命名为独立创新。

minimal J的旧sink可能在V数据慢排空时更早给A服务，boundary可能改善V前缀进度；若下一贡献是在线决定J，需要另给可用信息、目标和强普通两极/中间选择对照的具体收益或界。当前本稿只接受even E窄合同的协议及成本恒等式，拒绝无条件odd K、Pareto或生产泛化。没有提出新实验或修改root文件。

## 7. build02修复：保留既有promise、短切新child，odd J可安全闭合

本席重新直接核实际source `:103–108`：start_cutover新增`closure_target>=promised_write_end_i[0]`。qwriter的promised_end来自 `:31–32`：当前parent绝对base+offset+live child已注册count，既有local firstVALID尚未global选择也包含在内。已有child比E长时，现在仅不选择这个E，不能把held AW header/count裁短、撤回或吞掉。

直接核top `:710–715`：phase1仅flowV、未创建的child预算改为`max(0,min(old_quantum,J-start))`；qwriter `:35–37`把真实next_count再与原parent剩余/Q取min。后面的phase1 source_hold例外 `top:732–733`用这个已裁next_end，允许合法q1在payload未齐时先offer。qwriter只在 `:83–89`新建时取next_count，已有live/offered的child_q/count_q不受新quantum改变。因此两个新门分别排旧反例A和B，功能作用不能互相替代。

**旧E小于已有firstVALID end时的继续路径**：拒绝start后phase仍0，source_v_hold不因这个候选E拉高；原physical child按真实RLAST退休，logical skid如常返回。原solo builder `source:249,:263,:285–300`仍能在offered/busy/skid清空后发下一V source child，补齐现有qW欠拍。当后续E足以覆盖promised，才能触发；若始终到了E256，则回退原独立完成。这里等待合法切点不会自造停止所有后续V AR的环。例如old K1/E1、promise2现在不切，下一child覆盖[1,2)，E2才可能切；这是事件推演，未DUT运行。

**同沿race仍闭合**：start时phase1预算尚未生效，但source_v_hold同拍已经拉高；旧full-payload例外仅能创建end<=rV的child。busy/noSkid保证rV<E，因此新同沿child不会越过已选E。切点允许现有promise<=J；phase1以后所有新Vchild明确end<=J。firstVALID之后的W/B路径继续，不受hold撤回。

**odd E的新有限闭合**：K3/E3、promise2的旧反例B现在先完成[0,2)，下一child取count=min(2,J3−start2)=1，成为[2,3)。旧source R2正好供这笔W，rV/wV均能达到3。phase1 `source:245–247`仍要求noSkid、实际rV=wV=J、promised<=rV；不要求最后child B完成。其B仍由qwriter独立兑现。J=E最后真实physical RLAST已清busy，进入phase2后 builder `:249–261`可以立即修复，不等待另一次LAST；未改变ID/last/resp校验 `:308–318`。

**odd J的phase3不依赖偶数网格**：V从J开始继续每child<=2，到N256时最后[255,256)合法q1。A从FA0追到J，prefix预算 `top:720–722`精确截到J，必要时也是q1；joint以后 `:725–726`保留新start<=peer实际raw_written。即使两网格在native F处错位，受限证明只用child长度<=2，不用start偶数：若共同R在成员k的库存`r−w_k>=2`处被挡，任何更旧peer W头的end<=其准入时w_k+2<=当前w_k+2<=r；它全部payload已实际返回，正常服务可退头。prefix旧child的end<=J已全部有payload；V实际wV在接合前已经到J。若有B等待，最多推迟新child创建，不引入下一R才能完成旧B的缺payload关系。

F1/F255与odd J的原WRITE父边界仍保留：F<J时prefix可以先经历首原B再捕获actual AW2，source cert256来自真实两页授权；F>J时接合后仍可能遇旧父短尾，但phase3 `source:131–134`已移除额外logical-LAST/frontier AW coverage门、保留lead<2和成功cert门。末R先暂存、旧W头退、原B/下一AW再前进的§59修复不需要J奇偶。qwriter `:95–109`检查original WLAST并最后聚合原B，V/A最终job response不因短q1或physical RLAST提前发生。

**最新源码接受**：上述single-job/static/full-permission/q2/B3/normal最终服务合同内，build02两新门排掉本稿build01奇数反例，odd J接合及terminal q1未发现新增协议阻断。条件不扩展为动态fault/invalidation、任意量子、多job或任意held通路抢占定理。某E因promise而跳过是合法结果；比较最小J和E时必须读取实际选择的E，不能把配置K当作两次相同的触发E。

## 8. odd J的W请求成本与本轮未验证项

R计数恒等式仍为同实际E/FA/既往读取下`E+256−FA`。但是odd J的新q1改变W请求数：V原WRITE parent256在J处被分为两段，q2 children数为`ceil(J/2)+ceil((256−J)/2)`，even J128、odd J129。每流W payload仍256，但odd J多一笔V真实AW和child B；不能由相同R宣称相同全部事务成本。

A native页界F>0又在J精确切分，按排序端点`{0,F,J,256}`的非空相邻段`s_a`计算child数`sum ceil(s_a/2)`。F1/F255为奇数时，无论J奇偶，恰有两段odd，通常仍129；aligned F0时odd J也使A128变129。该公式是本席按固定q2/currentparent/仅F及J裁分的手工推导，**不是已读CSV值**；实际driver应严格核256个W payload、各实际child AW/B及original parents，而不把129AW错判成重复数据。

截至本稿修订，本席只收到root的build02成功和旧build01 18even PASS消息，尚未收到odd K矩阵验收。root拟运行K1/3/7/15/127、F1/F255及压力条件；本席没有执行、改动失败证据或提前称其通过。普通catch-up控制、无Pareto保证和同状态创新边界仍与§5–6一致。
