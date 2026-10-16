# MAP-OWNER-REPRESENTATION-COST-1：frame借用与record退休的源码边界

2026-10-16 JST。Codex-only，沿用ARS已定机制／定向来源核查阶段。请求配置 `gpt-6.1-sol/high`，本席无法独立取得实际model/effort metadata，均为 `unknown`。只读已存官方Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a` 字节；未联网、实现、运行模型、测试或设备。

## 1. 结论和可以接受的最强条件

**真实调用位置允许把新增B-record的frame读者局限在原frame仍有效的prepare阶段：短锁内查找并复制DMA base／必要值，解锁后只用值准备TX；成功后原frame沿SQ→CQ存活，持久模式CQ无需再取record。** 原生代码没有该B-record、K索引、持久mode或owner回收hook；这里证明可放置的生命周期边界，不是现有内核已实现这一表示。[XD:847–950、659–700；XH:57–102；XF:166–178]

若所有其他新增CPU读者也在同一受保护索引／状态锁中结束record访问、不在锁外保留裸record指针；creator、唯一closer、控制遍历和异步责任都已真正结束；record已断新入口并完成B unmap／必要同步，那么**可在该锁的同步合同下回收这个record槽，不必另等一次逐record RCU grace**。这是一条条件实现路线。frame负责保源页及相应DMA使用期间的映射，锁负责CPU metadata访问；两者不能互相冒充。索引锁／状态锁的精确组合与跨key槽复用由root决定，本席不设计整个索引。

这不是新发现的独占能力：前轮[整合分析](../../mapping-integration-delta-20261016/owner/integration-analysis.md:40)已允许bucket/record锁与“RCU或等价保护”，且明确取消token不取消metadata读侧同步。[同文:68–96] 本轮把一个等价保护分支绑定到frame和cache的真实边界。普通R/O3同享。

**不能接受无条件的“unmap后立即返K”。** 上述全部责任已结束时，UNMAP_DONE与RETIRED可以在同一关闭动作中先后完成、无需独立RCU排队；若仍有creator／worker／裸指针读者，就继续占K。既有XDP allocator RCU、ctx/config/table整体寿命和target关闭CPU同步均不因此消失。

## 2. 证据别名

所有行号相对保存raw根目录 `iommu-platform/out/mapping-admission-20261016/v612-source/raw/`；绝对路径、hash、来源清单与本轮阅读范围见 [source-manifest.json](source-manifest.json)。

| 别名 | 仓库路径 |
| --- | --- |
| XD | drivers/net/ethernet/mellanox/mlx5/core/en/xdp.c |
| XH | drivers/net/ethernet/mellanox/mlx5/core/en/xdp.h |
| RX | drivers/net/ethernet/mellanox/mlx5/core/en_rx.c |
| M | drivers/net/ethernet/mellanox/mlx5/core/en_main.c |
| XC | net/core/xdp.c |
| XF | include/net/xdp.h |
| PP | net/core/page_pool.c |
| H | include/net/page_pool/helpers.h |
| T | include/net/page_pool/types.h |

冻结范围仍为mlx5e cyclic／linear／order0／普通page-backed／非XSK、单buffer、固定B/config及T持续授权条件。单buffer不删除driver bias。S逐对象撤权不能继承T许可。实际部署/backend、运行模式和服务费用均未知。

## 3. frame读者可以在何处结束

### 3.1 prepare与SQ接受

XD:870–885逐帧从 `frames[i]` 读取data/len，现有O0在SQ接受前执行 `dma_map_single`；XD:909–923调用xmit并处理拒绝；XD:925–940只有接受后才把mode、xdpf、dma_addr压入FIFO并增加nxmit。XH:75–102写明现有FRAME FIFO是 `xdpf, dma_addr_1, ...`，不是record handle。

因此可在拟议持久分支替换这个prepare内的映射查找：在仍拥有合法frame责任时，锁内校验当前固定pair与LIVE记录、复制dma_base等必要值；解锁后按实际offset和必要sync提交。**源码不强制把record指针放入FIFO。** 拒绝帧不进入成功前缀；任何仍需读record的失败清理也必须在归还该frame之前完成。原生这里仍是逐次map/unmap，不能把持久分支写成已存在。[XD:877–940]

XF:166–178定义xdp_frame，只含data、长度/headroom/metasize、mem、dev_rx、frame_sz、flags；没有免费B记录字段。XF:300–308把frame放在packet头并从rxq复制mem，因为rxq只在enqueue/NAPI期间有效。因此frame内 `mem` 或 `mem.id` 不是新增B record／domain地址。[XC:445–451表明mem.id标allocator]

### 3.2 正常CQ、残余清理、bulk归还

XD:673–700的FRAME分支取xdpf和短图dma_addr，先unmap、后 `xdp_return_frame_bulk`，没有return后的record访问。正常CQ在XD:783–803调用同一free_desc并flush；停止后的残余descriptor在XD:819–841也调用它并flush。故拟议持久mode若CQ只归还原frame、不读取record，**新增B record的frame读者可在prepare内结束，无需CQ token_put或CQ handle**。这不证明FIFO字节／cacheline缩减，未编译测量。

生命周期的保守边界是“调用return之前”，不能利用bulk有时尚未flush就继续借record：XC:475–477可能直接调用普通return；XC:487–505会在满队列、allocator变更或fragment过程中flush；XC:455–464的flush调用PP bulk。PP:854–855还明确bulk调用后不得再使用data区。即使这次只是排队，交出归还责任后也不应保留新record访问。[XC:407–422、455–509]

原生可以跨SQ/CQ保留的是frame及现有dma_addr值；不存在“源码要求新增record reader一定跨到CQ”的证据。若以后给CQ新增统计、完成回调或record pointer，便是改变本条件的新增设计，需要另付引用／锁／退休合同。

## 4. 最后pp引用、bias和实际cache成员资格

H:227–238定义pp_ref_count是以后通过page_pool API归还的outstanding引用；可设大量bias后批量退还未用部分。RX:274–302确实每次alloc设 `PAGE_SIZE/64` 的bias，释放时退还 `bias − frags`；RX:1838–1841为已交出去的XDP帧增加frags。**一页一包也不能省这个driver责任。**

最后frame和bias无论哪个先结束，H:305–323与PP:869–873只有最后逻辑pp引用才进入put_unrefed／普通回收。H:250–285又把最后引用之后的字段保留／恢复成1。因此 `pp_ref==1` 既可能是活跃最后一个frame，也可能是已空闲缓存页；字段值不能代替事件和容器所有权。

PP:736–740还要求普通page_ref_count==1且非pfmemalloc。PP:764–773在最后pp门之后先做A device sync，然后才direct入cache或提供ring候选。PP:722–733明确cache插入调用者已知page_ref==1，并要求安全上下文。

**cache-owner能够排除合法frame的证据链是：** 页依正常最后pp／bias门到达回收，且仍是private alloc.cache实际成员；owner在既有非并发allocation保护中把它真正摘下；在私有保管期间没有重新发布，所以不可能同时重新分配给合法frame。PP:427–430要求allocation side非并发；T:189–202要求driver提供保护；PP:795–815限制自动direct return的本地上下文。不是“任意softirq都可扫描”，也不是“同CPU永远是owner”。

PP:430、PP:399–415、PP:730–731实际出入private cache；PP:818–829与857–902把不满足direct／cache满的页放ring或真正退出。本轮不把ring页当private cache成员。历史IDLE位、LIVE record、单CQE或普通page_ref==1都不能替代实际成员资格。错误双return、漏记frags或其他API误用超出合法frame合同，原生字段不会自动修复。

## 5. source真正退出必须覆盖的实际位置

统一source true-exit hook必须早于PP:679–692的原return终结段：A unmap／provider release，release_cnt计账，clear_pp_info与put_page。PP:684–685明确这可能释放pool，计账后不能再读pool；不能先计release_cnt再把裸pool指针留给异步closer。要在实际退出之前关闭B关联，不能只等pool最终disconnect。

| 实际调用位置 | 原文允许的退出原因与本条件的义务 |
| --- | --- |
| PP:398–406 | ring refill取到NUMA不合页会true exit；统一hook须覆盖 |
| PP:736–790 | 普通page_ref升高／不可回收等fallback会true exit；last pp不证明所有普通CPU页引用都消失 |
| PP:818–829、898–902 | ring满时单页或bulk余页true exit；不能只钩CQ／direct cache分支 |
| PP:906–934 | frag drain／free在最后逻辑引用后可true exit；要保留全部bias责任 |
| PP:990–1002、1021–1056 | pool scrub清ring/cache；cache清空要求alloc不可并发，在途producer仍可能晚归还 |
| PP:1138–1150 | update_nid在安全owner上下文直接清alloc.cache并true exit |
| M:1371–1384、1017–1043 | RQ关闭先停新posting／同步、destroy RQ、free RX desc，随后page_pool_destroy；不是立即清掉所有远端frame的保证 |

PP:1115–1134允许pool因在途页延迟destroy；PP:1005–1018最终disconnect后才free。它支持“pool关闭不会自动越过合法last-frame门”，但不支持“driver RQ/channel活到最后frame”。因此新增pool_ctx不能向已释放RQ借寿命；晚归还、worker和控制表必须各自有合法anchor。若部署新增主动撤权／迁移／强制unlink在合法frame未结束前撤B映射，就破坏frame借用前提，应另有drain或use协议，不能从这些正常退出位置推它已经安全。

## 6. target关闭和真正需要跨段的原生指针

XH:138–145清XDP_TX_ENABLED并 `synchronize_net`；XD:856–868先检查该开关才定位SQ。M:3197–3216的deactivate先调用这条gate，再deactivate channels。M:2077–2086关闭XDPSQ时清SQ enabled、synchronize_net、destroy SQ，随后free残余desc／SQ。M:2587–2594关闭本channel的XDPSQ再关闭RQ。

这些真实位置支持拟议target gate／CPU进入者退出／descriptor归还的放置，**不单独证明所有source pool的持久B关联已枚举，或下游firmware destroy已给本轮核实的完整DMA stop保证**。本席只读到保存M里的调用，未扩取底层实现。固定B/config若在frame仍活时被unmap、换domain／rights或释放，frame保住源页也救不了旧IOVA。故target真正关闭仍须禁新、排已进入CPU prepare/submit、取得真实DMA完成／合法stop，再对全部相关record唯一关图，config持到最后unmap。

原生确有一个与新增B record不同的跨段metadata指针：XC:480–492从mem_id_ht取得allocator xa并保存 `bq->xa`；XC:455–464后续flush继续读 `xa->page_pool`。XC:84–89删除xa用call_rcu；PP:1005–1018的disconnect/free与XC:92–114登记移除相关；XD:826–841还明确为bulk归还持RCU。**本轮不删除这套allocator RCU。** 这是ctx／allocator整体退休的实证，不能把逐record无grace误写成全路径无RCU／无CPU同步。

本轮没有发现原生对“新增B-record”不可避免的跨锁裸指针reader，因为原生没有B-record。反之，creator在锁外map、closer在锁外unmap、控制遍历暂存record和worker回调若由新设计引入，必须保持slot归属直到结束；这是新责任，不能凭“所有读者只复制值”一笔删除。锁内值复制也不替target/config整体读侧保护。

## 7. 紧邻反例与接受边界

| 反例 | 对应结论 |
| --- | --- |
| reader锁内拿record指针，解锁后才读dma_base；closer同期返K给另一key | bucket锁没有保护越锁解引用；必须锁内复制值或另持record寿命 |
| frame仍活，外扫见pp_ref==1就卸图；owner已在PP:430重新alloc并在RX:285重设bias | pp计数字段和历史idle都不够；必须实际摘下cache成员并隔离重分配 |
| 借frame只到SQ接受，FIFO改存裸record，CQ return后继续统计record | 删除CQ record使用，或给越过return的访问单独保护；frame责任已交还 |
| cache页已摘下，锁内只标CLOSING就立即返K，锁外unmap仍使用旧record字段 | closer本身是活责任；完成unmap及所有后续record访问后才返K |
| 所有普通reader锁内复制值，但异步callback／控制遍历仍持旧pointer | 仍需该责任结束／引用保护；不能声称逐record退休已消失 |
| source true-exit仅在pool disconnect钩B卸图 | ring满／NUMA／refcnt fallback早已清pp_info并put；B旧alias可越过源代际 |
| target关persistent gate后立刻释放domain，O0或已进入submit仍进行DMA | 须关全部DMA入口并排CPU／设备责任；frame不保target配置 |
| 以“无逐record RCU”为由删除XDP bulk的RCU | bq.xa跨调用保存并使用；现有allocator销毁仍用call_rcu，整体寿命不能删除 |

条件正例为：TX锁内复制LIVE B地址，解锁后原frame持续保护，CQ仅按原frame归还；全部片段与bias真正结束后p合法进入cache；source owner在保护中摘下p，锁内断该记录的新入口并取得唯一关闭责任，锁外完成unmap，所有控制／creator／closer责任结束，在同一状态锁中完成移除／返槽；随后才按owner合同重发布p。若没有任何越锁旧record reader，就无需额外逐record RCU grace。这是手工条件偏序，**不是并发证明或运行结果**。

## 8. 完整费用和未解决问题

无独立逐TX token不等无CPU同步。每持久TX仍有索引定位、短锁获取／释放、状态与DMA base读取、offset和必要sync；miss、CREATING／CLOSING竞争、cache扫描／摘放、唯一unmap、source hook、target关闭和ctx/config/table寿命均计费。锁竞争／cacheline迁移与RCU读侧／退休等待之间的实际CPU和容量代价未测，不能预断锁更快。

逐record RCU队列可以在上述条件下没有，但K中CREATING/LIVE/CLOSING及实际未结束writer仍共同占预算；异步unmap或关闭责任不自动消失。若实现只能延后source终结，页实际保管与inflight保持需单独计资源。cache-owner回收B关联不减少源池本来持有的payload；metadata/PTE/IOVA、CPU、物理并集、带宽和有效完成分开报告。

未实现；native CPU/锁费用、record字节、实际退休延迟、K周转率、payload增减、完成延迟、净收益均为 `null`。后续若实现，root需接受精确reader集合、跨key槽复用的锁合同、creator／closer／控制责任、ctx/config整体寿命和真实backend／target drain。hash与行界核对只证明来源定位，不能代替这些验证。

本席仅写本目录的findings与manifest；未改production／canonical／Git，未调用DeepSeek／Claude或子agent。
