# 资源与生命周期：普通page_pool、额外B引用及DMA_PMD已有退役

结论：**映射存在本身不增加一份payload物理内存；普通page ref保留却会改变mlx5e源pool的回收分支。** 额外B普通引用若留到原consumer最后归还，`page_ref_count!=1`使该页退出A pool、卸A映射并清pool信息，页仍因B引用存在。下一次A取页因此可能用其他cache页或慢分配新页，不能沿用所有策略相同PFN再用轨迹。无额外ref时，源pool本来缓存的页可让B alias不增加物理保留；但普通页真正离pool时需要在物理释放前撤B alias。DMA_PMD已有下层pool的“全idle→unmap_all→RCU→buddy”组合，不能把这部分重新当新增能力。

本轮是来源绑定的条件核查与事件反例，无驱动实现、设备/性能实验、阈值模型或流量分布。请求继续Sol/high，实际运行模型/强度元数据unknown；Codex-only，没有DeepSeek/Claude或新子席。ARS前轮已选的定向source-verification纪律继续使用，不重开完整pipeline。

## 证据范围与版本

仅复用已存官方v6.12 raw（commit `adc218676eef25575469234709c2d87185ca223a`）与原DMA_PMD RFC 02/04/05/07/08文本。`source-manifest.json`列绝对路径、行范围、SHA256、原run manifest入口、旧下载URL/HTTP；本轮0下载，未拼接为运行内核。

下文 `v612/...` 指 `/home/xvhaocheng/work/iommu-platform/out/mapping-admission-20261016/v612-source/raw/...`；`RFC02/04/05/07/08`分别指 `/home/xvhaocheng/work/iommu-platform/out/mapping-organization-source-20261016/dma-pmd/{lifecycle02,allocation04,cap05,domain07,mapping08}.txt`。RFC行号是保存邮件文本行号，包含diff的`+/-/context`，不是已应用源文件的行号。特别区分原02与后续07改变的内容。

## 1. Bcache持哪一种引用，会走哪条释放门

这里“普通pin”仅指额外普通page引用（例如get_page一类），不指已实现功能或GUP/FOLL_PIN。stock B `v612/en/xdp.c:847–950`并没有这种映射cache；下表是加cache后的条件推导。正常DMA读的活跃frame本来有consumer所有权，B完成后return，不能把这份已有引用重复算成额外cache pin。

| Bcache选择 | 正常consumer归还时 | 对A复用和物理释放的作用 | 原文依据 |
|---|---|---|---|
| 无额外page/pp引用，借源allocator保留期保存B alias | pool最后ref且普通page_ref=1时可sync、回cache/ring | A继续能分配同页；源本来保留它时B alias的额外payload字节可为0。普通源页若离pool，alias必须先安全撤掉；没有ref保证cache独立活到以后 | `page_pool.c:736–790,818–829,857–902` |
| 额外普通page ref跨过consumer归还 | last pp-ref后检测page_ref>1，调用page_pool_return_page | **退出源pool**：卸A map、增加release_cnt、清pp信息、put源那一份；B引用保住普通物理页，但它不再位于A的cache/ring，也不再由源pool inflight追踪 | `page_pool.c:736–790,648–698` |
| 额外pool-aware pp_ref（不是普通page ref） | consumer的pool_put看到并非last-ref直接返回 | 暂不回A可分配cache；A map/pp归属与pool inflight仍在，直到cache放掉这份pp-ref。其后普通page_ref=1时才可回pool | `helpers.h:227–323; page_pool.c:607–623,857–873` |
| 只保留某个pool对象user_cnt | 阻止pool对象最后destroy，不改变每页最后consumer/普通ref检查 | **不等于保住某页或禁止逐页离pool**；不提供B alias的逐页物理寿命保证 | `helpers.h:446–449; page_pool.c:1087–1092,1115–1134` |

pp_ref最后用户是`page_pool_is_last_ref`返回逻辑0；v6.12可以把字段重置1，不要求字段存成0 (`helpers.h:245–308`)。mlx5e用bias (`en_rx.c:274–302`)减少原子操作，consumer归还与RX bias drain先后不限；上表的“consumer归还”指相关pool references均已到最后门的时刻。

普通page ref保留导致源pool脱离后，`page_pool_clear_pp_info:642–645`清掉`page->pp`/签名；`return_page:687–692`已经结束pool inflight计账。因此Bcache不能再把旧`page->pp`或旧XDP mem.id当稳定源owner；B日后卸alias并put普通引用也不自动把页重新接回旧A pool。要继续在原池回收需显式、计费且经验证的适配，不能把“普通pin维持refcnt”描述成原stock快速复用。

这也说明普通page pin和pp-ref都有代价但行为不同：前者可使旧pool较早完成destroy、同时页留在B外部；后者可以使旧pool的inflight非零、destroy继续延期。页存在、页可给A分配、pool对象存在是三个不同状态。

## 2. 三类内存：必须同时保留实际所有权与反事实基线

用户要求的三类分开记录，不能见到映射就按页加一份，也不能原frame早归还就减一份总RAM：

| 类别 | 应记录的对象 | 不能误算什么 |
|---|---|---|
| `M_source_baseline` | 同外部到包/路由输入、同源allocator/池配置、无新增Bcache保留/copy策略时，源pool/底层物理allocator本来持有的实际物理页，包括RX库存、正常在途frame、cache/ring及大页中尚未可退役部分 | 既有A常驻map不等于新物理内存；若对照本来用order0而候选新增DMA_PMD，额外大页取得/碎片与保留先记源组织成本，不能假装对照也免费有同样span |
| `M_B_extra_blocked` | Bcache或新增寿命约束相对该基线**实际延后的可回收/可复用资源后果**：外置普通ref保留页、延后pool_put的pp-ref、以及由A不可复用引出的替代分配；记录每页/PMD的原因和释放门 | 基线原本还缓存该页时，不能按“B有alias”再加一个page；也不能只数B持有PFN，因为额外RAM可能表现在A新分配的替代页 |
| `M_copy_target` | 新目标copy池实际物理页及其空闲保留/在途copy；有无新池、容量、退役都要显式说明 | copied payload与源payload是两份实际对象；copy池空闲缓存也占RAM；源早回pool并不等于源物理页已free |

严谨账法：先列本策略的实际**源allocator拥有集合**`Sπ(t)`、**已脱源pool而由B寿命持有集合**`Hπ(t)`、**目标copy物理集合**`Cπ(t)`，以及若存在的**撤销隔离/待退役残余**`Qπ(t)`，按物理页/完整PMD去重计`bytes(Sπ ∪ Hπ ∪ Cπ ∪ Qπ)`，元数据/IOVA/PTE另列。然后与基线实际运行状态`S0(t)`比较，将增加归因给B阻塞/替代分配、源组织或copy。`M_source_baseline`是反事实，不能直接把它的PFN集合和本策略的新源页集合相加；物理所有权账的S/P/C/Q集合（这里H就是root命名的P）与因果归因三类也不能混成同一个未经去重的加法。不可回收、暂时保留、正在供给RX应分别报告；同一个PMD落在多个原因上也只计一次物理字节。

两个直接反例：

- **映射有、额外payload=0**：p在A cache，基线也会保留p；B持久alias不持额外ref且依赖合法源owner退役，物理RAM仍只有p，新增仅映射/关联元数据及相关寿命集成。不能加“一个B映射=另一页”。
- **p原本也保留，仍可能多一页**：基线p可回A cache；B普通pin让p脱源pool，A库存需要补充且无其他可用cache时分配q。本策略实际有p(B持有)+q(A用)，基线可只有p(A复用)。如果只问“p是否基线也保留”会误报额外0；这个P字节差额的直接物理位置是替代q，原因是B使p不可回收供给。若A本有别的cache页，暂时不必新取物理页，不能断言每次pin都立刻多一页。

这些是条件事件反例，不是测量、性能预测或固定流量统计。若B普通ref移除早于pool最后归还检查，则page_ref可能恢复1，走原recycle，不触发上述退出；时间次序必须保留。

## 3. copy/pin怎样改变下一次分配和映射再用

这是源码明确允许的反馈，不能冻结PFN序列后忽略它：

| 条件事件 | copy早归源 | Bcache普通page ref | 无额外ref、原frame持至B完成 |
|---|---|---|---|
| 正在B发的原包尚未完成，CPU拷贝已结束；RX bias已drain；A可安全缓存 | 若copy adapter已接管输入责任并把原frame正确pool_put，p此时可回A cache | 即使原consumer结束，普通ref仍使p脱源pool；A cache没有p | p仍是合法TX在途consumer，未回源cache |
| 下次A需取页，cache恰好只可能得到p或为空 | 可以取回p，减少一次补充分配的条件成立 | 用其他页或走慢分配q，不能把下一包PFN仍写p | 若B先完成则可能p，若未完成则其他页/慢分配；完成顺序决定 |
| 新包路由到B | p若仍在同授权源区且B图持久，可能命中已建图 | 新q的源区/domain关系可以不同，旧p cache热度不能免费算给q | 同样按实际取出的页/区与路由计，不用未来r作为在线信息 |
| 源pool或底层extent退役 | copy_target还需等B完成/归copy池；源是否physfree须另看source baseline保留门 | 先撤B alias再drop普通ref，最终free不由旧pool继续追踪 | 源owner只在已有consumer释放且映射安全退出后退役 |

源码支点：`page_pool.c:422–436`先取alloc.cache，`:370–419`从ring补入，`:577–590`缓存耗尽才慢分配，`:538–564`批量取新页/映射/回填；`en_rx.c:457–474`明确先free一小批RX WQE再alloc补充，因而归还时机能影响cache供给。页的具体LIFO位置、其他cached页、并发return/CPU/NAPI上下文、NUMA匹配与batch都会影响下一页，不能承诺copy后必定取p。NUMA不匹配可逐页离pool (`page_pool.c:398–406`)，`update_nid:1141–1151`还可清alloc cache。

copy若早于B SQ最终接受，需要正确错误adapter：失败不得一边让原frame提前归源、一边又把原指针留给devmap返未接受后缀。接受前缀/拒绝后缀合同与B map先于SQ检查均按前轮v6.12链保留；本轮没有重实现或假定copy池现成可用。copy输出后用户计算/发送状态也不能延长已经交回source pool的原frame逻辑持有期。

## 4. stock普通NIC实际有些什么retire/release点

**有实际控制面钩子，但不是现成跨B逐源区cache协议。**

- **停止源A补RX**：`en_main.c:1371–1375`清RQ enabled并`synchronize_net`，明确同步NAPI防止post RX WQE。`close_rq:1377–1384`先destroy RQ、free RX desc、再free_rq；`:1017–1043`调用page_pool_destroy与xdp_rxq_info_unreg。scope是队列，不是任意源region的B映射admission gate。
- **停止B新ndo与清理B SQ**：`en/xdp.h:138–150`清B XDP_TX_ENABLED并同步network readers，`en/xdp.c:856–858`使后继ndo拒绝。`en_main.c:2077–2086`清SQenabled、syncNAPI、destroy_sq后free_xdpsq_descs；后者`en/xdp.c:819–840`回收残余frame。正常完成仍用CQ (`:673–700,748–803`)。本报告只核到driver发出destroy_sq及随后清描述符的次序，未追硬件FW销毁语义，不把`synchronize_net`或RCU冒充“设备DMA已完成”。
- **pool页离开**：`page_pool.c:368`给`page_pool_return_page`内部static linkage；`:648–698`只处理A map与普通page归还，未调用Bcache逐页veto/retire回调。它还可在softirq返回路径发生，不能在此无条件阻塞等同一NAPI的B完成或睡眠式撤销。若新Bcache要借这点退役，需要新增、普通对照也可使用且计费的适配，不能假装已有公开release-notifier。
- **pool init callback**：`types.h:89–91`明确注释private/test-only，`page_pool.c:626–639`只在新页加入pool设置pp信息时调用。它不是逐页release回调，单靠它不能防源离pool时B alias还存活。
- **pool disconnect**：`page_pool.c:1005–1018`在pool最终销毁才调用disconnect；`:1049–1056`必须先scrub并达到inflight=0。XDP通过`xdp.c:323–324`注册`mem_allocator_disconnect`，`:92–113`做mem.id表项移除/RCU回收。它不是所有页free前的事务，也不是能任意覆盖而不丢XDP registry清理的hook。普通B引用让页离pool后，pool的release计账已完成，disconnect不等Bcache普通ref结束。
- **pool destroy本身**：`page_pool.c:1028–1030`要求caller保证不再并发alloc；`:1115–1134`根据user/inflight延期，不在alloc快路径检查一个“本页禁止B建图”的状态。停止新B map、收掉已有有效DMA、撤所有B alias、再允许physfree的region级顺序，仍需要源owner/映射cache/driver之间明确联动。

实际可部署的粗scope候选是协调停止相关A/B队列，采用各driver stop/destroy合同后卸新增B图、释放cache引用、让源pool/物理allocator按原门回收；是否接受停队列、FW完成语义与完整成本本轮未验证。更细粒度region gate、in-flight B计数与异步退役是**拟议接口边界**，不是当前源码实现或新意证据。不要求物理free前撤A跨包合法map以外的逐包S合同；T专用范围到退役可持续授权。

## 5. v6.12 memory-provider/netmem不能当普通mlx5e现成通用hook

`page_pool.c:278–297`只在`PP_FLAG_ALLOW_UNREADABLE_NETMEM`时从RX queue配置取`mp_priv`并初始化`mp_dmabuf_devmem`；`:587–590`慢分配才选择专用provider；`:679–692`离pool调用专用release函数的bool决定是否清pp/put普通page；无provider则仍普通A dma_unmap→put_page。`types.h:155–157`的provider参数只有`mp_priv`，不是在这里公开的任意provider operations表。

所选mlx5e普通RX创建`en_main.c:924–950`只设置DMA_MAP|DMA_SYNC_DEV，参数清零，没设置ALLOW_UNREADABLE_NETMEM。因此该普通链不走这个provider，仍用CPU可读struct page，`page_pool_alloc_pages:595–598`及mlx5e page_address/XDP约束不能忽略。这里已有netmem是表示/专用分支基础，不等于Bcache可注册一个通用“先拒新map、等B DMA、撤alias、再free”的provider callback。

特别是provider release返回false也不表示“维持page_pool inflight等我批准”——`:687–688`仍增加release_cnt。provider内部实现本轮未下载、未核；不能据callsite推它能保护任意普通CPU page、支持mlx5e XDP或允许阻塞等B DMA。扩展普通provider/netmem适配可能是一种实现位置，但需要接口/权限/执行上下文验证；**缺少现成接口不自动形成novelty**。

## 6. DMA_PMD原来已经组合了什么

以下是已存RFC proposed code，**不在官方v6.12里**，不能说本机已移植。它能排除几种过宽的新能力主张：

1. **源PMD的真实取得和本来保留**：RFC04:188–232分配PMD_ORDER、拆成blocks、初始化dirty bitmap、持pool kref并publish pooled；`:237–299,327–377`优先partial/idle取blocks，再新取PMD。RFC05:142–174加入全局cap/失败回退，`:181–265`shrink只取完全idle PMD。A page_pool若保留任一个block的普通page_ref=1，下层该block仍在使用，PMD就未全idle，不能把这个底层物理保留全算B alias新增。把v6.12接到RFC allocator需另做适配，本轮没做。
2. **per-region/domain持久图已存在**：RFC02:526–552有`domains_mapped` bitset/map_lock；RFC08:302–319,323–410在合格domain首次使用建立整PMD图，以bit表示存在，后续bit-test+IOVA算术；`:306–308,327–337`使用整PMD CACHE|READ|WRITE，权限适配不能免费当只读。per-buffer unmap通过window owns判定直接return（RFC08:83–91）。它不为每份映射另get一个普通page ref；底层pool所有权和block最后free共同提供物理保留与退役范围。源码中的“fast path”成本注释不是本轮测量。
3. **真正block最后引用→可用bit**：RFC02:348–449明确`__dma_pmd_free_page`在普通引用到最后free时拦截、设dirty/free bit、计算是否all idle；`:738–743`在`mm/page_alloc.c`的free prepare加入hook。这是已经设计出的allocator所有权组合，不是“bitset仅记录映射、不管free”。普通B额外page ref同样会阻止block走最后free，从而阻止最后PMD全idle；不需要所有PMD字节都被B逐个pin也可形成整页不可回收后果。
4. **全idle才物理退役并撤所有domain图**：RFC02:143–215是分阶段release、RCU、buddy顺序；该02阶段`:207`尚是占位，不能单独声称已调用unmap_all。RFC07:351–378将其接为真实`dma_pmd_unmap_all(meta)`；`:202–233`读取/清bitset，对所有仍活domain做`iommu_unmap(PMD_SIZE)`；随后清pooled、RCU，再RFC02:168–193逐block unfreeze/free。不能把“所有alias在physfree前卸掉”“退役等最后block引用”认领为新机制。还要保持调用前提：RFC07:209–212在清bit后即释放map_lock，实际unmap在:231，随后才清pooled；所以unmap_all单独不是region关闭/拒绝新map门。源码安全组合依靠全idle PMD已摘出allocator、没有合法活block使用者；若外置cache打破这个前提，要另给admission/ownership合同，不能拿bitset函数名代替。
5. **domain终止/索引重用也已有处理**：RFC07:269–346,386–408清registry slot、标draining、遍历各pool三链表清该domain bit，flush reclaim并synchronize_srcu，防止unmapper访问已释放domain或旧bit被新domain index误用。这是已有的domain寿命/SRCU处理，不是B DMA完成计数，也不证明任意外部cache已适配。
6. **pool终止与在途块已分开**：RFC02:318–343设destroyed、摘idle、drop creator kref，partial/full由各PMD kref存活，最后block free触发后续retire；RFC04:334–337 allocator看到destroyed返回0（现有API条件，不据此完成并发正确性审计）。RFC07:418–451替换destroy注释并增加SRCU保护，但仍不是一个会主动等B CQ的函数。原02:305–310曾明确“caller first quiesce device DMA”，该段被07替换成in-flight blocks晚free的描述，不能把diff里的删除句冒充最终文字。合法driver需在DMA仍使用时保住block；SRCU/RCU只保护CPU读者/域寿命，不能用作硬件DMA完成证书。
7. **供给查询和全局内存压力不是新接口**：RFC04:381–402已有`dma_pmd_pool_has_free`，明确advisory/stale并考虑copy replacement；RFC05已有cap与idle shrinker。单说“按有无pool供给选copy”“idle回收映射”不足以提出超越它的新能力。partial PMD不能被该shrinker逐块回收，若提出更细资源能力，需正面比较block/PMD粒度与移动/复制/重映射完整成本，不拿不存在的免费compaction当基线。

## 7. 可核条件裁决与后继接口

- 若源范围由DMA_PMD式allocator拥有、所有DMA操作遵守持block至完成、B关联图进入同一domains bitset/unmap_all，T允许整范围持续授权，则**仅为了alias有效而额外普通pin不是必要条件**；其安全physfree顺序已有原型来源。应比较集成成本/rights能力，而非再造已有退役算法。
- 若只在stock v6.12外置一张Bcache、完全不持引用且没有覆盖每个源页退出/底层allocator退役的联动，则缓存不能安全跨任意源page free；需要普通适配。若选择普通ref简单保活，则能在卸B map前阻止物理free，却破坏原A pool recycle，必须承认源供给/映射再用反馈。
- 若把cache寿命改成pp-ref，能利用现有last-pool-ref阻止recycle并保A map，但它占着源可分配页及pool inflight，不能同时宣称“cache不pin、不阻碍复用、无需retire适配”。
- 若copy使p提前可复用，只有在A接下来确实需要这份供给且没其他可用cache时，才可能避免source替代分配；copy_target及其空闲库存也占内存，总物理改善方向须按实际集合/归还顺序判，不能由早return单独推出。

本席只完成这些生命周期与资源边界，不裁决论文主线/新意，不选系统方案。下一必要证据是root冻结一种真实source owner+cache引用合同，给同信息普通实现相同接口，保留真实alloc/return事件反馈并核完整成本；不需为了补缺项先开发驱动。当前源码缺口与RFC已有能力都已经定位，未扩大为全族否定或正收益证明。
