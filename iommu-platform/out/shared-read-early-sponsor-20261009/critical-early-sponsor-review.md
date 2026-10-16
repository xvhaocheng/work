# §59：V 独立启动后的首次共享接力审核

Codex 独立关键协议审核，直接读本目录design、实际source controller/top/qwriter，并只补核真实backend/legalizer的首AW独立进展路径。未改SV、未构建、未运行。§57/58已经核过的permission reuse和sink路径只引用本轮必要变化，不重新做全面审计。

**修正接受范围：旧版存在root实际run13/F255的终端队首环；首次审核的“未发现新阻断”及只按FIFO借槽推导的闭包漏掉了logical-LAST/frontier coverage门，现撤回该旧结论。** 最新source `:128–130` 仅在已获全长WRITE授权的phase3移除该额外coverage门，保留权限和两拍库存限制；在mode3/prefetch1/keep-q1、同reset单job/static normal immutable共享源、每流private容量至少两拍及正常最终服务合同下，该修法排除了下面的真实反例，未发现由此产生的新阻断。A cursor0可由首actual rawAW建立WRITE预取；late共享检查和sink原义务不变。若J<F且F奇数，安全仍需peer raw-W游标准入，不能依靠“同q2网格”。

## 1. bootstrap没有“等A首B才能准备WRITE2”的循环

top `:1055–1060` 的WRITE slots只在actual qwriter `total_q!=0`后有效；context由真实rawAW提取（`:1073–1076`），而slot2地址是已登记job的nextpage（`:1070–1072`）。qwriter `:44` 的原AW-ready仅由parent-live/B-valid决定，**不受source_a_hold**；`:72–81` 在实际rawAW握手时捕获header/total。A的首q-child虽因rA=0而被hold，首rawparent仍可已被私有捕获。

实际platform此首AW路径不要求R0：top两iDMA是RAWCouplingAvail0；真实 `idma_backend.sv:837–854` 为独立AW fall-through，未接 `:811–835` 的R→AW coupler；CONF129的decouple_rw使 `idma_legalizer.sv:453–458` 各方向独立推进、各自按页划分（`:209–214`）。所以有合法事件链：A job进入backend→首rawAW capture→真实WRITE1/WRITE2 prepare→A logical cursor仍为0。它不制造任何假AW/假B/已完成数据。

当前mode3的source_a_hold（source `:88–89`）在phase3前冻结缺payload的新A qW；top `:723–724` 仅对已有完整payload给例外，初始rA0不满足任何非空child。qwriter的existing AW/W/B路径不撤VALID。slot1/2 real WRITE key、TL权限检查、固定page/跨B生命周期沿用§58完整审核；不能把source READ grant充当下一WRITE许可。

若future WR没能在V旧physical child期间成功，或者J==256，controller继续原V→A串行；mode3没有因为“预取已计划”就进入phase1。任意RR公平性不保证PTW能抢过已经held的R，本轮fallback仍必要。

## 2. 首次共享检查现在确实位于late admission

mode3原先跳过初始cohort，所以不能仅沿用mode0/1已有的PA相等假设。当前 source `:101–105` 要求两原AR已捕获、各READ cache正常成功、两WRITE成功前沿256、两original parent未结束，以及旧physical child确在busy。

`start_cutover` 同拍 `:219–235` 又逐项核两个READ PA和cache/prot/qos/region/user一致，并在mode3断言A cursor0。这些字段来自各自原source parent；rawAR入口 `:189–196` 已检查正常INCR/size3/cache2/非lock/非ATS及原ID0或4。READ授权仍来自每context自己的helper READ response，匹配PA并不代替权限。

PA/属性不匹配时当前prototype是fatal，并非自动继续两独立copy；这在预先限定共享同源的fixture中是fail-closed验收。若作为一般coalescer扩展，需把这些条件纳入可选加入门并安全回退，不能将当前fatal描述成一般适应不同源的策略。

start同拍的v_hold与RREADY=0（`:90,:136`）、固定偶数J、phase1无skid且rV=wV=J/E_V<=J（`:242–243`）、phase2真实sink/ID/RLAST（`:305–315`）保留§57门。故mode3首次进入接力，没有因只采旧promise而漏掉同拍的新firstVALID child，也没有把刚收的skid落在J快照之外。

## 3. A从0补读，跨F和J的actual WRITE父保持合法

phase2 builder source `:246–258` 在旧physical offered/busy/skid都清掉后，以A原READ recipe从rA0发256拍physical read。beat<J只给A，beat>=J双给（`:113–116`）。V先前收到[0,J)，A新收到[0,256)，没有重给V[0,J)；每个成员cursor与beat-index必须相等（`:309–310`）。

source physical child可以跨A WRITE父页界，因为已独立获得两页WRITE成功，且payload目的地写仍经actualTC授权。A first actual AW长度F、next actual AW长度256−F，并未被改写为一个跨页AW。qwriter `:35–37` 总是取current actualparent的剩余长度；top `:712–714` 再把repair-prefix内部child截到J。若F<J，会在F返回首original B、捕获第二rawAW、改用已成功slot2，再继续到J；若J<F，则prefix在原first parent内结束，joint后才遇F。

qwriter `:96–106` 分别检查original WLAST、生成child WLAST，并只在整actual parent的全部child Bs回来后生成original B。因此在F处结束一个actualWRITE parent不会提前结束原sourceAR256，也不会被当作DMA整job完成。

## 4. J<F时“共同网格”不成立，但实际门更强

具体合法轨迹：F127、J4。V从偶数J继续q2；A在native firstparent末端用[126,127)的q1，后续native parent从127继续[127,129)。V同时有[126,128)。所以共同网格在F后错开，design中“联合尾段同q2 grid”不能用于这一路证明。

当前actual top并未依赖该说法：phase3所有joint新child的job-global start必须<=peer **实际raw_written**，否则quantum0（`:715–718`）；保持detached/独立groups（`:725`），keep-q1禁止long child。上例A next[127,129)不能在V w仍126时offer；V旧[126,128)已经有R126/R127，可以先取走R126，使V w达到127。A then nextchild才可firstVALID，V旧AW/W因此不会被它抢到前面并形成missing-payload WHEAD环。

### 4.1 旧版run13的真实反例：FIFO未满，终端coverage门仍可造环

root报告旧版run13/F255/K16/prefetch1/cutover1停住：两成员returned均255，V offset254/total256/live1、A offset254/total255/live1，均offers128/groups127。V已firstVALID的qW `[254,256)` 排在全局W头，缺最后R255；A已firstVALID的旧qW `[254,255)` 有R254，但在V头后。A第一actual WRITE父的B必须等该qW完成，故第二actual rawAW和其coverage256尚不能建立。旧logical-LAST coverage门又要求A coverage覆盖R255，于是R255→V末W→A旧W/B→A AW2→R255形成环。

这里**A第二页WRITE权限已预取成功，certified_end=256**；欠缺的是第二actual AW的coverage，不是TL权限。两flow库存此刻仅一拍，`r-w=1`，所以本稿原“只有`r-w>=2`才能挡R”证明不适用。peer start准入本身允许双方start254，是合法可达状态。失败日志/CSV属于root的实际证据，本代理没有修改或执行它们。

### 4.2 最新phase3修法与完整受限闭包

直接核当前source `:119` 仍禁止`beat_index>=certified_end`；`:128–130` 仍禁止负库存、库存已>=2，并把末拍/认证末端额外coverage条件改为仅`cutover_phase!=3`时生效。`:131` 的pause/detach门和其它phase原合同保留。phase3只能由 `:248–249` 双READ成功、双WRITE前沿256的builder进入；固定单job/static上下文下该授权不会被A旧B撤回。因此此处允许**有授权、暂未有AW coverage的payload私有停车**，不允许未经WRITE认证的下一页访问或撤回任何held AW/R。

run13状态的R255现在可进入skid并实际送给两DMA；每流库存最多由1增加至2。V头 `[254,256)` 获得完整payload后可消耗并退休；A `[254,255)`随后退休、第一original B返回、第二actual rawAW捕获。A剩余R255一直保留在FIFO中，再供已获slot2权限的新一拍W。old original B与future actual AW之间的时序间隔不再反向卡住源R。

修正版一般证明：joint源R每拍经同一个skid完整返回各member后才收下一拍，空skid时两logical游标同为r；phase3下一拍0..255已有成功cert覆盖，且已取消额外last/frontier coverage门。若此时flow f因借槽条件挡下一R，必有`r-w_f>=2`。它当前或更旧的自身qW child最多长2，未消费末端<=w_f+2<=r。对排在它前面的peer child g，准入时 `start_g<=w_f(当时)`；w_f单调，故 `end_g<=start_g+2<=当前w_f+2<=r`。该WHEAD所需payload已实际交给peer，或已进入TC，正常最终服务可使其退去，不依赖被挡的下一R。若flow尚无新child，最小w的flow满足peer start门；旧child已耗完而等待B也只依正常完成服务。

repair-prefix旧child的end<=J，进入joint前其全部logical R也已交入A；V旧prefix已经raw_written==J。已有firstVALID不因该修法改变。若被挡的是skid里的logical返回，物理接收前的`lead<2`限制与至少两拍真实private容量保证当前待返回拍有空间；实际DMA ready继续作最终容量检查。以上闭包要求每流一个child、长度<=2、keep-q1、shared-skid有序双返回、peer真实W游标准入、static双WRITE全成功、same-bank正常TC顺序及memory/TL最终服务。它不是任意容量、动态fault/invalidation、任意memory或cycles上界定理。

删掉peer-W准入就有实际结构反例：A不等V旧W游标，先offer一个错位[3,5)，V只有[2,4)和两拍库存，下一R4被借槽门挡住，A[3,5)又缺R4；这正是§57找到的反例。当前代码保留了排它的门。

## 5. 原logical parent/metadata/RLAST没有随sink提前结束

新stage的rawparent仍是各一次AR255，original ID0/4；source `:144–145` 从各original parent生成logical ID/last，`:317–321` 只在真实logical handshake更新returned/done。sink只增长physical child_got、核对应child RID/真实RLAST/正常resp（`:305–315`），不对DMA发discarded tail。

真实iDMA的R-DP metadata pop条件仍仅为logical RLAST handshake，`idma_axi_transport_layer.sv:268–270`；中间child的physical RLAST不进入这个接口。V原AR可以先收到J拍，保持原metadata，再从repair中收到[J,256)；A保持其最初未消费metadata直到repair完成256拍。最终各原请求恰一个logical RLAST，A跨actual WRITE parents不影响此语义。

**terminal logical R可以早于A第一WRITE父B，但不早完成job。** 直接核transport `:254–270`：真正logical R握手把数据推入buffer，RLAST只弹R metadata；R-DP FIFO在backend `:554–570`，与WRITE-DP FIFO `:590–607`独立。transport `:354–357` 仅在真实raw W握手消费对应buffer字节，`:392–396` 仅在原WLAST消费后弹WRITE metadata，`:450–455` WRITE完成response来自B。qwriter `:96–106`保持A第一actualparent的WLAST/B边界，`:108–109`只有该原B握手才释放parent。正常DEFAULT copy且NO_ERROR_HANDLING的backend `:519–532` 用`w_dp_rsp_valid && w_last_burst`产生最终response，last/super_last由 `:637–648` 的WRITE末标记FIFO提供；不是R-DP的RLAST。source `:185–187`也仅在实际job_done后清active，并核原logical R已全返回。因此A rawR metadata可先退，末拍payload仍在buffer供未来AW2/W，最终job_done仍等最后WRITE B。

## 6. 成本、退化和验证边界

相对§57/58，这个mode3 late admission没有新增功能寄存器。相对无handover强普通，仍需phase2bit+vcut9bit=11功能位及控制逻辑；oldend9bit用于观察。WRITE预取仍占真实A slot2响应/seq/valid/prepared布局与匹配逻辑，应给两强普通同能力和完整计费，不能把它当闲槽零成本。

令E为被sink的当前V child绝对末端，已完成的V早期physical child加当前child共读E拍；A cursor0的repair再读256。所以handover总source R是 **E+256**，不是J+256。root本轮提供的J96/E112→R368只是与此算式一致；本代理未读运行日志、未执行9copy，源码接受不等于独立实测验收。极迟WRITE许可导致不handover时，mode3继续V全256后A全256，共512。

必要实际对照是同预取mode3不handover、mode2普通共同clip和variable short-K；source R增减、V/A done、mean和makespan需分别陈述。因为独立早启动与普通预取本身已有普通实现，不能把全部改善归于late join。根正在推演的data-resident qAW强普通尚未实现，本稿不预判其结果，不增加文献或实验任务。

root补报修法build03成功、F255原死锁run20完整PASS（V5654/A5681，R368/J96/E112），并扩F3/127/129/193，build03共8copy PASS。这是root执行的结果，本代理本轮只直接核新 `:128–130` 和actual iDMA R/W metadata/jobdone路径；未独立运行或复核这些日志。旧run13保留为真实设计反例，不能被早期9copy PASS或后续修法抹去。

本次修正接受：首actualAW bootstrap、late独立READ/属性检查、J/sink原义务、F/J跨父、peer-data前沿加phase3终端coverage解耦、original RLAST，均在上述窄合同通过手工/source检查。动态fault/invalidation、不同源、未对齐byte、multiple jobs或all-member pause继续在合同外。
