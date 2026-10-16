# MAP-REUSE-OPPORTUNITY-1：正常source再用与退出条件

2026-10-16 JST。**固定普通cyclic RQ、native-XDP linear单buffer切面后，mlx5e为每次RX allocation提供整页stride，一页同次分配内多packet不是本切面的复用机会。B映射重复机会必须来自同页保持source所有权期间，真正recycle后再次分配。** 源码支持这种正常回收路径，也支持consumer未归还、跨CPU回流、有限ring容量及NUMA变化限制它；不能仅由这些路径推出现实再用频率或CPU净收益。

本席沿用ARS-Codex已定有界证据分析，只核source。请求席`gpt-6.1-sol/xhigh`；实际运行model/effort元数据`unknown`，未声称切换。Codex-only。未重开owner协议、原语表或旧阈值账；无新下载、采集、实验、模型、驱动/硬件运行；未写canonical。trace与CPU测量入口由其他席/root处理。

## 1. 原文身份与实际配置边界

仅复用官方Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`已存raw。下文PP=`net/core/page_pool.c`，H/T=`include/net/page_pool/{helpers.h,types.h}`，RX=`drivers/net/ethernet/mellanox/mlx5/core/en_rx.c`，XD=`…/en/xdp.c`，PAR=`…/en/params.c`，MAIN=`…/en_main.c`，XC=`net/core/xdp.c`；都位于`out/mapping-admission-20261016/v612-source/raw/`。绝对路径、hash和定向范围见[source-manifest.json](source-manifest.json)。没有混入RFC、开发内核或PMD布局。

复用既定工作流：`mapping-admission-20261016/root/steering-contract.md:15–16`的普通单buffer线性RX、非AF_XDP、XDP FIB转B或PASS里的本机UDP条件子集；`decision-analysis.md:12–14`的非PEEK、足够用户buffer、copy成功后的消费条件。PASS本身不是本机交付证书；当前设备、MTU、RQ类型/参数及实际consumer配置没有运行记录，以下冻结的是**来源绑定条件切面**。

| 配置连线 | 原文事实与本切面的约束 |
| --- | --- |
| PAR:256–281 | XDP线性stride至少PAGE_SIZE；注释明确此driver的XDP不支持每页多个packet。不能因小packet而按PAGE_SIZE/packet_len想象同页可装多个XDP包 |
| PAR:296–317；693–707 | linear_skb条件要求数据和相应头尾能放CPU页；普通XDP且linear时，stride恰为PAGE_SIZE，`num_frags=1`、`wqe_index_mask=0` |
| PAR:939–945→MAIN:911–912→MAIN:604–640 | cyclic参数frags_info传给rq；frame0_sz使用frag_stride；页分区按stride前进，超过PAGE_SIZE换frag_page。因此本切面每页每allocation一个packet位置 |
| MAIN:924–950 | 普通非XSK创建order0 page_pool，使用DMA_MAP/SYNC_DEV、设napi/nid/pool_size并登记PAGE_POOL类型；未选unreadable-netmem provider |
| RX:274–302；1833–1841 | driver仍用pp-ref bias批量记责任；XDP_XMIT成功消耗的packet会增加该页已交付fragment计数。**一页一packet不消除bias最后归还门** |

“single-buffer”一般并不自动推出“page一packet”；本轮是一条具名driver配置链额外证明这一点。多buffer、非XDP较小stride、其他driver的同页多个fragment均不混入正例。generic片段规则用于解释last-ref，不作为当前切面额外再用次数。

## 2. join、alloc、frame归还、recycle、true exit是五种事件

1. **join(g)**：order0慢分配批量从allocator取得页；A图成功后调用set_pp_info，增加hold_cnt（PP:538–559；626–639）。同一批新页可能暂留source alloc.cache，尚未成为RX frame。这里建立的`g=(pool实例/epoch, page, 本次source所有权)`不是每个packet id。
2. **alloc_round(a)**：`page_pool_alloc_netmem`先取cache，再经ring refill，缺页才慢分配（PP:370–436；577–590）。同一页从cache/ring再次交driver会开启新的allocation round，**不调用新的set_pp_info、也不结束g**。RX:281–289为这次round重设片段bias。
3. **consumer/frame return**：B普通CQ处理先卸本帧短图，再`xdp_return_frame_bulk`（XD:673–700）。XC:469–508先排入frame bulk，最终XC:455–464才调用page_pool_put_page_bulk；CQ reclaim时刻与真正pp引用归还不是同一事件。B flush/doorbell也不是CQ。
4. **recycle**：PP:866–873先过逻辑last-pp-ref；未到最后则只归还当前引用。最后引用且普通page_ref=1、非pfmemalloc才可recycle（PP:736–773）。依据上下文进入alloc.cache或ptr_ring；这只增加可供RX再取的页，不保证下一次一定取到它。
5. **true exit(g)**：PP:673–698卸A图、增加release_cnt、清pp_info、put_page。g到此结束；以后即便同PFN重获分配也属于新source代际。普通page_ref可能仍由其他用户保留，故source退出也**不必等于buddy物理free**。在相反方向，B CQ→recycle更加不等于buddy free。

最后pp-ref是逻辑门，不要求字段物理值为0：H:245–308可能把计数字段恢复为1。RX bias和全部consumer责任都结束才可进入最终pool回收；不能见一个B CQ就把整套引用清零。PP hold/release是source池页所有权计账，也不是每次RX alloc次数。

## 3. 分配／回流真实顺序

| 执行点 | 确定性顺序 | 能推出与不能推出 |
| --- | --- | --- |
| alloc cache | PP:422–436从`cache[--count]`取，非空先用本地栈 | 给定栈顶和事件次序可以精确判下一页；源码不能推所有包都按固定PFN轮换 |
| ring refill | PP:370–419在cache空时从ptr_ring取页，NUMA匹配才入cache；最多补至REFILL，然后取最后入栈页 | ring中较早进入的一批可能以cache栈顺序交出；不能把ring入队顺序直接当逐包出队顺序，也不能据“p已在ring”断言下一页是p |
| fresh order0 | PP:538–565批量申请，逐页A map/set_pp_info，成功页填cache后取最后一个 | cache/ring缺供给时才走新物理分配；DMA map失败页不成为已join正常供给。实际批量返回数量不是源码常数保证 |
| direct return | PP:722–733、764–773；795–815判断安全本地NAPI，最后ref可压栈 | 成功压栈后，若没有改变栈顶的中间操作，下一次cache alloc取该页；这是状态／顺序条件，不是任意时间gap或频率假设 |
| remote/bulk return | PP:864–892用page_pool_napi_local；非本地通常走ring，生产者锁下批量入队 | 跨CPU本身不必source退出；ring有空位就可后续再用。跨CPU不等于NUMA不同 |
| driver refill | RX:457–478按refill_unit分批，**每小批先free旧RX WQE，再alloc新WQE**；PAR:638–674根据RQ与stride计算bulk/refill_unit；RX:893–911受enabled和missing阈值约束 | 这是正常回收改善局部供给的真实位置；driver的先free不代表在途consumer页已能回pool，仍需bias+consumer最后门 |

T:50–51的alloc cache容量128/refill64是源码常数，PP:194–210的ring容量取实际pool_size或generic默认；MAIN:929传的是driver算出的pool_size。**这些是空闲cache/ring容量，不是source总页数上限。** packet仍在consumer时，source可继续补其他页；不能用cache+ring容量替代整个source占用、未来再用次数或候选K上限。

## 4. 正常条件切面A：跨recycle仍有重复B使用

冻结上述cyclic/XDP/linear/order0、source NAPI稳定在同NUMA、普通page_ref=1且非pfmemalloc、配置仍open、外部两次真实入包都由既定FIB送同B。下面仅给**合法事件偏序**，没有指定相隔多少时间、流量概率或重复分布：

| 顺序 | 来源绑定事件 | 状态含义 |
| --- | --- | --- |
| A1 | p在g的一次alloc round进入RX；收到一个packet，XD:327–335成功redirect；RX:1838–1841记交付责任；B尝试map且实际SQ接受 | 这轮只有这个packet占该页；B未来CQ仍未知 |
| A2 | B实际CQ先到并完成frame bulk归还；driver旧WQE剩余bias尚未drain | H:245–308及RX:295–302允许当前return并非最后pp用户；p此时未必已recycle |
| A3 | 正常RX refill在RX:471先free相应旧WQE，drain剩余bias，逻辑last-ref成立；`allow_direct=true`且cache有位置，p入cache；条件切面取p为该小批最后一个成功direct入cache的已转B页 | driver bias与consumer归还顺序在此闭合。没有给idle B缓存额外普通page/pp引用 |
| A4 | 同一正常refill随后在RX:472进入alloc，在源allocation侧既有非并发保护下从PP:430栈顶取p；新的packet仍按外部FIB送B | 新alloc round，仍同g。这个局部LIFO顺序由“最后成功入栈→紧接alloc”确定，不要求任意时间gap；不能预先赋给策略或当全体页的常态 |
| A5 | B实际map尝试/接受/CQ再发生；p仍满足正常回收条件 | 同g存在跨recycle的重复B尝试及接受。持久mapping若已接纳且T合法可有避免重复map的机会；CPU多少不由源代码给出 |

更宽的正常正向路径是A2在driver bias已drain后才发生：B bulk最后归还可进本地cache，或跨CPU进仍有空位的ring，随后source refill实际取到p。它不要求B CQ一定先于driver refill，只要求在**那次实际再次取p之前**全部责任已结束、p未true exit。两种顺序都不保证当前工作负载频率。

这个切面没有用同allocation多个小包构造r>1，也没有要求下一包在某个任意固定gap到来。它用真实正常路径说明**可达性**；实际每个refill里p是否栈顶、何时达到RX missing阈值、B CQ何时处理和下一包路由结果，均是缺失的运行输入。

## 5. 正常限制切面B：consumer未结束，当前补RX不能再取p

仍用相同source配置与外部工作，不设人为reuse gap。只改变一个正常事件偏序：**需要执行RX补充时，p对应的B accepted frame尚无CQ/还未flush归还，或p这轮走本机UDP且其skb仍由正常consumer持有。** RX:295–302归还driver未用bias后，H:245–308仍有consumer责任；PP:866–871不进入最终pool回收，p不在alloc cache/ring。

- 在这次真实补充点，source只能取已有其他cached/ring页；若这些也空，PP:577–590走慢分配。不是必定新增一页：source可能还有别页，慢分配也可能失败；实际补充量、内存压力和工作完成必须分开。
- **本切面跨allocation再用被推迟**；固定linear XDP一页一packet，所以此页不能靠同一round剩余packet继续增加B次数。不能把一个长在途interval算作很多次B reuse，更不能拿“source寿命变长”单独推更高r。
- B后续完成/UDP消费后，p仍可能正常进cache/ring再用，亦可能遇到下一节的normal退出；所以“这次不能取p”不证明g总共只能B一次。非PEEK UDP copy成功以后consumer处理的是自己的副本，也不能把用户后续计算一概算成继续持有原RX页（复用前轮工作流`decision-analysis.md:14`；本轮RX:1701–1708只核到recyclable skb交付）。
- 纯本机UDP页若从未送B，其`r_B=0`，不能把source recycle次数统统当作B映射命中。混合路由还需逐round识别实际目的；XDP_PASS不自动意味着本机UDP。

这是正常已有consumer所有权的限制，不新增逐包撤权、固定长等待数值或额外cache pin。B CQ频率、UDP socket排队/消费速度、真实到包次序只能由输入记录确定。

## 6. 正常限制切面C：远端回流／容量或NUMA使g真实结束

**C1，跨CPU回流且ring满。** T:204–211明确允许远端CPU归还；PP:795–815在不满足local NAPI时不direct cache。最后引用、page_ref=1的页虽然可recycle，仍须把页放ring；PP:885–902的bulk若ring不能再放，就将剩余页交page_pool_return_page，g结束。普通单页return同样有PP:818–829。条件是**真实配置的ring当时已满或这批归还超过其真实剩余容量、source drain尚未消费出空位**，不是任设reuse分布。跨CPU且ring有位置时可以保住g；仅cache满也会先尝试ring，不能把cache_full计数全当true exit。

**C2，source实际NUMA变化。** RX:899–900正常post-RX路径调用page_pool_nid_changed；H:451–454在nid不同才更新，PP:1139–1150清alloc cache并使其中页true exit；ring旧页在PP:385–406 refill NUMA检查不匹配也会被waive/true exit。稳定同node不触发；跨CPU同node不推出该现象。它提供实际驱动触发连线，但没有当前NAPI迁移频率数据。

| 其他源码存在的true-exit原因 | 精确位置 | 当前固定切面的边界 |
| --- | --- | --- |
| 普通page_ref不为1或pfmemalloc导致不适合recycle | PP:736–790 | 是真实API分支；不能把额外B cache pin人为加到stock，亦未证明当前native-XDP input经常有普通额外ref。XD:328还注明redirect时page-ref=1 |
| pool destroy清alloc cache/ring及最后片段 | PP:924–934；990–1056；1115–1134 | 正常队列拆卸/重建条件下会结束g；在途consumer仍按inflight晚回，不能把destroy调用瞬间全当物理free |
| fragment allocator drain不再recycle | PP:906–934 | generic真实门；本切面mlx5e使用自己的bias路径，不能把generic fragment allocator当同一次路径重复计一次 |

source exit使当前g的后续B次数不再增长。随后同PFN被同/另一pool重新获得必须另记g；不把source pool地址/指针复用或相同PFN自动接回旧映射。退出原因实际存在不证明某原因主导工作负载，不能由分支数目给退出率排名。

## 7. r的回溯计数口径：尝试、接受和CQ分开

`r`只描述已发生的真实事件，不能免费给在线接纳策略未来值。完整身份至少是source pool实例/epoch、page/source代际g、当前B及其固定DMA配置；同g跨recycle不换代，但target配置关闭/重建应另分mapping episode。

| 口径 | 精确执行点 | 不混算什么 |
| --- | --- | --- |
| `a(g)` | 该g内page_pool把p实际交driver的alloc rounds | 初始批量join但未出cache不算RX round；refill页数不等于packet数 |
| `r_try(g,B)` | XD:870–885中本页frame真正执行B dma_map_single那一次；early ENETDOWN/参数/队列拒绝另列未进入map | FIB判B、redirect记成功、整批ndo_xmit调用次数都不等于map尝试次数 |
| `r_map_ok(g,B)` | 对应try没有dma_mapping_error | 映射成功后SQ仍可拒绝，XD:909–922会卸图；这是有CPU费用但无接受的工作 |
| `r_accept(g,B)` | XD:909–940实际ret为真、FIFO保存frame后计nxmit | map成功/SQ失败不算；只处理成功前缀。不是包已送达consumer的证明 |
| `r_CQ_reclaim(g,B)` | XD:783–803由CQ推进并对FRAME desc执行XD:673–700的packet级回收 | 一个CQE可覆盖多个WQE/packet；不能拿cqes替代r。XD:793–799还可报告异常CQE，正常/错误结果另列 |
| `r_stop_cleanup(g,B)` | XD:819–840以NULL CQ上下文回收残余desc | 不是正常CQ成功数；其真实DMA排空前提由driver合同另核 |
| `r_pp_return(g)` | XC:455–464调用PP bulk，再经H/PP最后引用判断 | CQ reclaim可以先排frame bulk；一次非last引用归还不是整页recycle；pool recycle不是source exit/buddy free |

对于从join开始、匹配了全部已发生事件的窗口，有`0 ≤ r_CQ_reclaim ≤ r_accept ≤ r_map_ok ≤ r_try`（stop cleanup另列，错误CQ标识保留）。窗口若缺join或前序accept，存在carry-in，不能硬用该式补齐配对。对完整排空的合法driver账，应核accept是否由CQ reclaim/合法stop cleanup逐frame闭合；不满足时保留未配对，不能默补成有效完成。

在**本切面的单页单packet、FIB单次转B且不clone/广播/软件重发**合同内，`r_try(g,B) ≤ a(g)`；同allocation内重复B尝试不是本轮合法收益来源。跨recycle重复B尝试的数量至多`max(r_try−1,0)`，它仍只是可避免首后mapping工作的机会界：首map失败、K满、T不适用、配置换代等都可能令可用持久图更少。源代码不提供所有g的r分布、跨recycle命中率、K覆盖率或有意义的非零频率下界。

固定一个有限真实外部到包/路由/consumer窗口，只能把尝试数界在对应实际单包工作数、source actual alloc rounds之内；没有真实窗口及alloc事件就不填数。cache/ring容量不限制一个g未来可经历多少次循环，源码也不保证它至少循环一次。不能把回溯r当未来接纳阈值输入。

## 8. 对价值判断可推进到哪里

**已支持**：正常source recycle确实能保住同page同g并再次供RX；driver“先free小批、再alloc”提供具体机会位置；B completion/UDP consumer与driver bias的正常顺序可以使机会出现或受限；容量与NUMA条件可在没有额外cache pin的stock里自然结束g。相同正常机制也可给V1-N/K和具名普通适配，当前没有独占贡献结论。

**未支持**：实际部署重复比例、每g尝试/接受/CQ分布、周期/吞吐、总RAM或不可回收内存改善、映射是否热点、K是否足够、任一切面在实际工作中占主导。无匹配source记录时只能给以上条件可达性和上下界；这不是“等用户痛点”或“等一般授权”。

若候选source退出进入Q或改变返还/分配，后继alloc.cache/ring内容、慢分配和B页身份可能改变。可用O0真实轨迹评估**O0来源机会**，不能直接把同一PFN序列抄成候选闭环结果。真实source物理集合、P/C/Q与metadata/PTE/IOVA应按策略分别计，B CQ早return本身不产生总物理省内存证明。

**能改变价值判断的具体缺项**是一份能将实际配置（XDP/linear/RQ stride、有效pool ring及NAPI/NUMA）与**同g跨recycle后的B尝试—接受—CQ/清理、真实source退出原因**配对的有限事件记录，并由root合并同backend O0 map/unmap CPU费用。若实际跨recycle B机会有规模且退出/容量约束留下余量，推进完整整合价值验证；若多数真实g只尝试B一次、频繁true exit或consumer/Q使服务资源不成立，限制该具名部署版本。不存在这份记录不授权自动采集、建模或开发，也不把“源码可达”当“收益已经存在”。
