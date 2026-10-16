# 普通 B5：正常 WRITE／正常 TL fault 的有界所有权闭合

本席独立延续前轮 B5 的 capture→prepare→FIFO issue 普通算法，形成可执行接口合同和逻辑成本上界。它是手推普通控制，不是已发表前作或已实现 RTL。没有仿真、性能、构建、综合或板测。`ordinary-contract.json` 固定状态及事件，`cost-envelope.json` 覆盖原 37 项目录；`static-calculation.json` 仅为本轮新字段计算器的实际一次执行。最终是否接受 R2 由 root 决定。

## 1. 边界与原件

范围是有限公平 run，完整 IDs 0/4/8/12，bank=ID[2]，每 ID 冻结 DID/PID/context；INCR、64-bit beat、non-ATOP、真实完整 W。NormalTL 有成功和 fault，一 request、一 response 共享端口，每 bank 一个 active owner。AW cell 各 bank 4，不能借另一 bank 空槽；TC W FIFO 各 bank 16，上下游 W owner 各 8。复用既有 spill 必须另计其真实两个 packet 槽。reset outstanding、异步 invalidation/remap、ATOP/MRIF、producer 丢 W 不在范围。本文写生命周期结论不增加 R 许可，也不声称完整应用无死锁。

定向原件及其精确行号／原文／整文件 SHA-256 在 `source-manifest.json`。核心入口：

- `rv_iommu_trans_ctl_wrap.sv:107-139,174-180,220-255,325-360,520-597,674-727`：原 AW FIFO、内部 AW 握手增加 W allowance、正常 TL 请求/返回、完整物理 AW、原成功/错误出口。
- `rv_iommu_banked_ctl.sv:158-184,236-305,359-407,419-440,526-584`：实际 4+4 / 16+16、按完整 ID 路由、bank-tag TL 单口、physical AW mux、上游 B debt。
- `axi_demux.sv:51-61,89-181` 和 `axi_demux_simple.sv:69-70,140-227,237-293,537-618`：内部默认 MaxTrans=8，input MaxTrans=16；W/目标锁、first-VALID counter reservation、cut 侧 B 退休。
- `axi_err_slv.sv:90-153`：错误 AW ID FIFO 深 1；真实 sink WLAST 才 push 错误 B；B ID FIFO 深 2。
- `spill_register_flushable.sv:38-103`：enabled spill 真实 A/B 两份 packet + 两 valid，不能计成一个槽。
- `rv_iommu_top.sv:125-165`、`rv_iommu_pkg.sv:79-85,352-358,488,528-529`、两个 AXI typedef：实际 header、TL request/response 宽度。

## 2. 具体普通算法 O-B5N

O-B5N 使用普通环形 FIFO 和阶段所有权迁移，不存全局 32-bit rank。

**状态。** 每 bank 4 个 full raw AW cell，带 2-bit phase（FREE/RAW/READY_OK/READY_FAULT），`head, tail, prepare` 三个 2-bit 索引，3-bit used 和 raw_count。每 bank prepare owner phase 为 2 bit（IDLE/OFFER/WAIT），owner 引用就是稳定 prepare 索引。全局 G FIFO 深 8，每项 3-bit `(bank,slot)`，read/write 各 3 bit，count 4 bit。全局 held 为 `(valid,bank,target)` 三 bit；target=success/error。原生出口保持全部 AW/W/B spill、full-ID 目标绑定、W 目标锁与错误 sink。input W routing 下文明确收费。

**接收。** virtual AW 真实握手时，所选 bank 必须有 FREE cell，G 有信用，并有有界 input W owner。整 raw header 写 tail；phase=RAW；bank.used/raw_count++；G append `(bank,tail)`。AW 只接受一次。首次 AW VALID 后上游原 mux 的 held owner/packet 不撤销。第一次到前端尚未握手的 W 可以保持 VALID，但本算法只在对应 virtual AW 已握手后给它 WREADY；这是合法背压，不虚构接收。已有 8-entry upstream first-offer owner 仍持有该 W 所属源。不可把 source=bank：完整源由 IDs 0/4/8/12 保留。

**准备。** 每 bank raw_count>0 且 owner IDLE 时，prepare 指向该 bank 最老 RAW。OFFER 从该 cell 产生正常 WRITE request，完整 IOVA/DID/PID/ttype/priv；shared arbiter 在 stall 时锁 bank。NormalTL request handshake 后进入 WAIT，cell 不移动/回收；同 bank 没有第二个 active request。另一个 bank 可独立 OFFER/WAIT。NormalTL response handshake 必须匹配 bank WAIT owner。正常成功把 zero-extended 56-bit spaddr 写回原 64-bit addr；phase=READY_OK。正常 fault 置 READY_FAULT（无需保存原 IOVA 供 B；错误 B 是固定 SLVERR）。响应全部落实后当拍释放 prepare owner、raw_count--、prepare++。下一 TL 请求可以准备后继，完全不等物理 AW/W/B。原 DID/PID 等 50 bit 扩展字段完成使命，可复用；外部原 context 冻结，不存在旧响应跨重映射。

**全局 issue 与迁移。** held.valid=0 且 G head ready 时，只有该 head 可以向其 bank 原生 completion demux 的 AW 输入呈现完整物理 AW 和 success/error target。prepared header/phase/引用保持到此内部输入握手；credit 后来改变不得撤回 VALID。此握手将所有 header/target 字段迁给已计费的 AW/selection spill。随后立刻 pop G、free cell、bank.head++/used--，锁存 held=(1,bank,target)。held 完全不再读 freed cell；可以同时接受新 raw AW，不把 raw8+held1 错算成总8。held.valid 期间不向任何 bank 提交后继 AW。直到该 token 的真正外部 physical AW handshake（success）或实际错误 sink AW handshake（fault），才清 held。两个 bank 的旧 spill 无未退休 AW；因此 RR physical mux 不能选择后继跨 bank AW 先行。仅按序投 spill、随后让 RR 任意挑两头的弱方案被否决。

**first VALID。** raw/G head 在内部 handshake 前固守；内部 AW spill 移交后保持 full packet/selection；native simple 的 AW first-offer lock 和 global mux 的 lock 保持 downstream VALID。held 到真实目标 AW 才退休。first VALID 可以先承诺有界 W owner；不能把内部 spill handshake 或 first VALID 叫作真实 physical AW handshake。该算法不额外要求 physical AWREADY 先于 WREADY；AXI 合法的 W 先于 AW 握手路径由已锁 AW 和既有 W owner 负责。

**W。** input 原序 W 按已付源 owner/显式 bank owner 投入所选 bank 的 16-entry FIFO；真实输入 WLAST 才退 input owner。每 bank 原生 allowance 在 internal AW→spill handshake 增加，在 W FIFO 输出真实 WLAST handshake 减少。每 bank 2-entry W spill、simple 的 first-AW-offer W target 锁、global output W-owner8 保持 packet 与顺序。success W 从正确 success target 输出；全局 physical W 依真实 physical AW first-offer owner 顺序。fault AW 不去 physical 口；其全部 W，包括先已缓存的拍，走 native error sink。sink 只有实际消费 WLAST 且错误 B FIFO有信用才生成 SLVERR；输入 WLAST 或 W 进入 spill 不能冒充消费完成。same-bank target 切换受 native W lock 限制，后继 W 不能跨越未消费的错误头。各存储满时 WREADY 背压；无无界缓存或丢 payload。

**B 与同 ID。** 保留原 full-ID success/error target binding。不同 target 的年轻 same-ID AW 不通过 cut，直到旧 target 的该 ID debt 清空。success memory B 捕获进入该 bank 的 2-entry B spill；fault sink B 也经过同一 spill。cut counter pop 是 B 真实进入该 spill的握手，不是最终上游 B 退休。此时旧 B 已有实体拥有者；B spill FIFO按捕获顺序持有至 upstream 接受。若旧 error B已被捕获、target lock释放、年轻 success AW 得以发出，那么年轻 same-ID success B只能排在旧 B后面。若旧 error 尚未生成，target lock直接阻止年轻 success 进入物理口，因此没有未付费的 error hole。对同 target 连续 success，正常 downstream same-ID B 次序保证捕获次序。input bank demux 的 full-ID counter、顶层 inflight_aw_q 均直到真正 upstream B handshake 才退休。成功 B先于错误 B提前捕获的“大 B 重排池”只在特意允许 same-ID 异 target 并行的新接口中才需要；本 scope 的最强低成本普通控制无需增加它。

**回收安全。** TL owner唯一、response无重复，rawcell只在 response落实及内部AW字段迁移完成后free，因此没有仍能指向复用cell的迟到 NormalTL response。held只引用bank/target。原序FIFO位置可环绕，存储不可在已锁 firstVALID前覆盖。冻结范围内 generation=0 bits 是明确 lifetime proof，不是漏计异常取消；超出本范围需要重作 epoch/cancel/invalidation 合同。

## 3. input 早接收与两个合法接口版本

原 `axi_demux_simple.sv:175-177` 有 `w_open==0 || old_bank==new_bank` 限定；因此未经改变，不能声称上一写 virtual W未LAST时即可跨 bank 接收新 virtual AW。

O-B5N-queue 付出一个深 8 的 1-bit bank-owner FIFO及 10-bit ptr/count（18 bit）。它以 virtual AW first eligible offer 预留 owner，首次 stall 时只 push 一次（1-bit commitment lock），直到真实输入 WLAST才 pop；首项未 virtual-AW handshake时 WREADY=0。需要另一个 4-bit accepted-AW-minus-input-WLAST count，表示有已接受的 head；owner reserve包括尚未握手的offer。修改 input AW admission 移除原 same-bank W lock；保留 full-ID B credit；W选择来自 bank-owner head，完整源身份仍在 upstream source owner/header中。这样每 bank cell额度不变，cross-bank early acceptance是真实付费接口。计费额外为18+1+4=23 bit；不能把源 mux 内部 queue 当现有裸接口。

O-B5N-tap 可作为更强普通成本上界：显式新增 upstream source-owner8 的 head read/tap接口与线，bank=完整源序号低bit，不复制 bank-owner8；前端 4-bit accepted count门控输入 W。upstream mux负责 first-offer锁和owner reservation。这个版本额外状态4 bit，另付metadata read port和source2-bit导出线。它是合法集成提案，并非当前源码现成接口。两版本均不得让有新AWoffer但未accepted的W被误认成旧 accepted 写；非交织原序及 accepted count证明head是已accepted的前缀。

若 root 候选保留原 samebank AW admission lock，对照可同样保留；本文保留这项 native 版本，但不拿它限制 queue/tap 最强普通实现。

## 4. 阶段成本与并发端口

原 AR 路径仍有每 bank 的 request118、response185、controller state3，共 `2*(118+185+3)=612` bit。WRITE 的 2-bit owner phase 与原 READ 的 REQ/AWAIT/COMPLETE 互斥；同 bank 不允许 WRITE WAIT 与 READ WAIT 同时存在，bank-only response 不能免费识别两者。WRITE 响应落实 cell 后退休；READ COMPLETE 等原 AR transfer 时仍占该 bank owner。WRITE request 可从稳定 cell 引用，省掉的是新增 WRITE shadow，绝不是移除这612 bit公共READ状态。读请求/返回的总额度仍为两 bank 共最多2 owner；WRITE/READ互斥门、两路 request mux和response owner-phase decode收费。本文有限公平写进度需要已有READ owner若存在按其既有合同最终退休，不新增read/drain保证。

一个明确可复核的安全编码上界：AW bankcell/G/prepare/held核心 `8*(153+U)+16+24+24+10+4+3+4=1317 bit`（U1）；queue输入版本另23 bit，共1340；tap输入版本另4 bit，共1321并付source-owner读接口。既有14 bit AW FIFO控制被24 bit bankring控制替换，不能把两者相加。公共原生spill/W/B/AR/READ/controller/diagnostics另在JSON完整收费；这些数字不是整模块面积，也不是普通控制最低界。

实际 A=64、I=4、P=56，U依生产 config，当前具名 cv64a6_imafdc_sv39 的 FetchUserEn/DataUserEn均0→U=1。test硬编码U=1；其 localDMA ID=2 不可代替生产最终ID=4。full final raw AW=`153+U`=154，physical AW=`103+U`=104，raw AR=`147+U`=148，physical AR=`97+U`=98，W=`73+U`=74，B=`6+U`=7，R=`71+U`=72。historical local raw AR146/R70 是 I=2。NormalTL request118、response185，其中 MRIF102、cause12；被保留的原 TC perbank req/resp shadow各118/185不能忽略。

单个 raw cell resident上界154bit；同 cell prepared-success需要104bit，原 addr64被spaddr覆写，释放50bit扩展属性；prepared-fault目标descriptor可沿用相同104bit上界而无需physical地址语义。phase单列2bit。宣称成本差应比较最大同时resident字段：`n_raw*154 + n_ready*104`，`n_raw+n_ready<=8`，再加所有owner/ptr/held/native spill。两表示使用同样8×154的物理逻辑存储预算时，阶段字段可回收并不自动缩小存储器。全prepared8时有832bit有效packet字段，但mixed/raw峰值1232；绝不能把832当完整容量。进入AWspill后cell早退，最多8 raw/prepared +1 held AW token；spill实际仍声明4 physical packets，任务窗口最多一个AWtoken驻其中，不因此把声明的4份硬件算成1。

本合同 data cell 每bank有2 read：prepare(raw header→NormalTL)和issue(prepared header→internal AW)；全局只有issue head一项可选。每bank可同拍接收raw fullheader和写TL response addr/phase，故2 data write（不同cell）；加commit free时最多3 phase updates。单R/单W存储实现可以按具名公平优先级 serialize，代价是这些事件不能同拍；不能在声称同吞吐/同成本时偷用免费口。NormalTL global request1/response1，两个bank各active1；response写总1/拍。globalAW internal submit≤1/拍，但held等待真实target决定使同一cycle retiring/next提交需明确组合旁路，否则采用下一拍提交。physical AW1、physical W1，另外两个bank错误sink可各独立真实消费1拍。input W总1/拍，两bank FIFO各1R/1W，4 native W spill packets另计。

普通globalFIFO既无min-rank comparator，也无8-entry搜索。bankprepare cursor明确最老raw，free用ringtail；metadata读两项而非免费 FIFOhead。2×(count>0/full compare)、2×ownerphase decode；globalhead phase equality2、heldtarget/bank判断各1；native full-ID lookups/comparators、目标绑定、Wselect equality、RR mux都完整保留。形成当前raw/ready动作无需job/epoch/offset匹配或R资格；R门原样保留，不能把准备许可等同于read/drain许可。

## 5. B额度不是8与闭合理由

`axi_demux_id_counters` 是按fullID的计数数组，不是global MaxTrans个transaction槽。input MaxTrans16→4bit counter +overflow→full15；当前selected IDs有4个，全局OR full gate下保守上界≤4×15=60，单push逐拍及无overflow的进一步约束是≤15+3×14=57（包含first-offer reservation）。每bank内部默认MaxTrans8→3bitcounter/full7；该bank两selected IDs的cut debt保守≤14，ORfull单push界≤7+6=13。cut侧已capt获B另在每bank2-entryBspill，上游debt仍在。rawcell8、spill未通过cut的pending最多held1、input firstofferreserved尚未accepted等与cut范围分开。以下均为上界，不能叠加重复token或当最低界：`B_accepted <= input57`；本O-B5N的pendingraw≤8、pendingheld≤1、cut≤26、Bspill≤4，故安全宽松组合界≤39（其中部分重叠）。不强制totalB≤8或perID≤8。

一个证明“8不足”的接口状态序列：ID0 issue 7 normal success并供齐W；B返回两项进入Bspill、upstream BREADY=0；cut计数降到5，再issue2并供W，cut=7且Bspill持有2，已有9项end-to-endB debt；可再保有rawcell但不必使用。input full15尚未满，Wowner用真实WLAST退回，故这个状态被本控制允许。它是静态合法事件构造，不是新nativeiDMA trace/RTL结果，不据此猜实际source最大Bdebt；原 iDMA额外metadata容量仍见成本字段。

正常fault complete：TLfault→internalheader transfer→errorAW真正接纳→全部W真正消费→errorB产生→Bspill捕获→upstream B握手。正常success complete：TLsuccess→internalheader transfer→physicalAW真实接纳／已锁firstoffer所允W→真实physicalW最后拍→正常memoryB捕获→upstreamB握手。每一步有唯一paid owner，先后并不要求WREADY晚于AWREADY。有限公平、payload完整、TL每owner最终一个response、memory每成功写最终B、upstream最终BREADY条件下，各最老token的阻塞均等待较老有界token或外部公平事件；无年轻写才能解除的内部依赖。这个手推范围lifecycle可闭合，不是generalAXI异常/形式验证。

## 6. 接受／否决及未解问题

接受 O-B5N 为该具名 scope 的可执行普通控制：真实normalTL、两个bankowner、提前prep与原序physicalAW/W、正常fault真实消费、sameID B、cell/held/spill/B退休都已明确；保留existing target lock是具体强对照，不用64个新B记录制造普通成本。

否决以“普通必另复制8个physicalheader”“普通必保留32bit全局rank”“所有cell必须活到B”“Bdebt只有8”“internalAW就是physicalAW授权”为差异依据，因为上述算法在相同信息与原生额度下给出反例。普通FIFO也能通过合法同cell覆写和ownership transfer省掉新增header复制。本文不预测root R2裁决，不把设计对应等同发表前作或架构最低成本。

未解：生产综合面积/时序/能量，read侧既有资格与全应用成本，native四iDMA确切shortburst/segmented时间序，hardware memory实现的2R/2W开销，保留native diagnostics的完整消除/映射结果，source最大的实际Bdebt。它们在JSON有具名unknown/null。若R2存在额外scope内动作（例如sameID异target独立并行），必须先为双方补相同B ownership合同；现合同不偷偷扩该能力。没有成本/动作差异证据前不运行性能实验。

## 7. 有界补充：更低普通上界 O-B5S，冻结于本节

上文1340/1321是可执行安全上界，不是最低界。下面两项是普通 FIFO 合法编码选择，均有具体状态/事件证明；它们不扩成功/错误/READ owner能力。

**去 raw_count。** 每 bank 只保留 head/tail/prepare各2bit、used3bit（每bank9，合18 bit）。prepare资格为 owner IDLE、`((prepare-head) mod 4)<used`、prepare项phase=RAW。used=0时范围判定禁止；used=4时全部槽occupied，再由RAW/READY区分满环是否仍有未译项。NormalTL response后prepare++，此项变READY；issue只能释放已READY项，因此不可能free未经过prepare的RAW。满环全prepared时prepare可回到head/tail，但其READY阻止重发；当该槽合法迁移free再被tail重分配为RAW，cursor恰可译新项。即使前方仍有老prepared项，新项也是最老RAW，不能跳过另一RAW。active OFFER/WAIT期间cursor及该RAWcell不可free，NormalTL稳定性不变。省6 bit，付两bank各一个2-bit模差、3-bit范围比较和已付prepare view的phase读取；不声称比较器或布线免费。

**仅 virtual AW accept push input owner。** 显式8-entry bank-owner FIFO只在真实前端virtualAW握手时push，真实inputWLAST才pop。其4-bit occupancy就是acceptedAW减inputWLAST，故不另存accepted count或own firstoffer lock。未接受AW及其W仍由upstream mux的已付firstoffer源owner/AWspill负责；input owner空则WREADY=0，VALID稳定等待，不偷偷peek源queue。保留input full-ID信用，但计数push改为真实virtualAWaccept；原源VALID锁仍有效。老accepted W owner有序，AW只顺序接受，故前端FIFOhead与sourceWhead的accepted前缀一致。输入状态共18 bit；可跨bank early接受且不借slot。source owner和output owner均仍深8；B counter不随WLAST消掉。此实现比原firstoffer-private版本省额外4+1bit。

**组合结果与端口。** O-B5S-queue = `8*154 +16phase +18bankring +24Grefs +10Gctrl +4TLphase +3held +4sharedRR +18inputowner =1329 bit`。共享nativeRR上界按N2的 `1rr +1lock +2request =4bit` 计一次；若另选3bit selected-bank编码，双方普通/候选都可选，不能将1bit编码差作为特有收益。1317旧core减6=1311；再加18=1329。paid tap版本旧1321减6=1315，额外source读接口/2bit连线仍收费。所有原生spill/W/B/AR/READ612bit以及具名公共未知项在37目录中保持一次计费；1329不是完整模块总面积。

O-B5S同样可选每bank2R/1dataW：NormalTL response优先，若与新virtualAW capture同bank同拍冲突，就背压AW；另bank仍可capture。G/nativeheld与phase free可独立更新，RAW/fullheader存储只需1dataW。有限run且源VALID锁正确时此串行化不失owner或改变最终AW/W/B序；它会改变同拍接收机会，必须与采用同调度的对照比较。上文2W是另一明确吞吐上界，不能免费等同于1W。没有为补充编码再运行模型、RTL或全calculator；以上是有界人工公式和lifetime判断。

另一个静态Bdebt>8例：同bank IDs0/8各5项normal success，W真实供齐、B尚未返回，共10 debt。每ID5≤internalfull7、≤source名义8，inputfull15及globalWowner8（已由WLAST归还）均不禁止该状态。此例避免把9同ID包装接口例误读成突破实际iDMA额度；两个例均未运行nativeiDMA trace，不据此宣称source能产生特定完整时间序。

本席最终接受 O-B5S 为本具名scope的普通可执行低成本上界；没有发现同scope的inplace/headerhandoff/NormalTL许可动作只能由新规则完成。最终候选成本与新颖性判断仍留root。唯一新字段计算器最初因原件实际末行103、摘录请求末行104而在算术前失败；定向修到103后完成一次14项字段算术。原失败及成功回执在执行账本保留，不改写成一次无失败运行。
