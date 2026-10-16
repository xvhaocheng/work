# 容量回收设计裁决：由源池取出空闲页，只撤目标映射

2026-10-16 JST。**本轮选中一个相对V1-N/K有具体能力增量的版本：映射槽有压力时，由源池原分配执行方显式取出private alloc.cache中的空闲页，撤销其B持久映射后放回。** 它能处理“冷页仍留源池、以后不再自然归还，旧关联一直占K槽”的条件场景，同时不要求新增逐TX独立引用token。它不是已经实现的驱动，也没有净收益数据。

最接近的DMA_PMD已使用“全部使用结束＋禁止重新分配”的安全回收原理；同信息普通R和dma-buf适配也可获得本轮新接口。因此本轮只接受**明确的原生源池整合假设和能力扩展**，不把安全cut、替换策略或B-only撤图认领为独占论文新机制；仍未选论文主线。

## 1. 真实问题及本轮覆盖范围

来源不是新造隔离需求，而是两个已有事实的组合：v6.12 page_pool会把已归还的页保存在cache/ring供未来使用；V1-N/K的关联库存有限，原版只在source真正退出或B整体关闭时撤图。于是合法地可能出现：所有K记录仍对应源池持有的冷页，新需求只能走O0，即使这些页已无在途frame。**这是来源绑定的容量反例，不是已测的常见分布或应用死锁；O0仍可完成工作。** K是候选预算，不来自pool_size或硬件IOTLB参数。

工作流仍为官方v6.12 mlx5e cyclic/linear native-XDP、普通order0、单buffer、非XSK/clone/retry，A接收后单次FIB到B或条件本地UDP。B/config固定；T完整授权范围仍为条件部署合同，S另列。这里只回收B的持久关联，不修改A映射，也不把其他NIC、PMD布局或历史TCP数据拼成当前运行事实。

## 2. 三个可选扩展与选择

| 具体版本 | 增加的动作 | 判断 |
| --- | --- | --- |
| 归还时检查pressure | 在最后pp引用成立后、cache/ring发布前，按普通规则撤B | 安全cut可成立，代价小但不解决已经沉在cache/ring且不再归还的冷页；不能承诺K轮转 |
| **源池执行方显式取出private cache页** | 请求源分配执行方服务；在原有非并发cache访问合同下取下空闲页，完成B撤图及必要退休后周转关联 | **本轮选中。** 无需该页再次自然return，不增加独立每TX或每alloc引用状态；付owner调度、扫描/查找、撤图、metadata退休及未来重建成本。首版不含ring |
| 外部线程扫描idle标志或pp_ref_count | 后台挑页直接撤B，或增加IDLE/ACTIVE/EVICTING状态 | 只读计数不安全；完整版本需与每alloc重新激活串行化，不能把新原子状态/交接当免费。暂不选 |

pp_ref_count最后归还后会恢复为1，值1也可见于新分配/单引用阶段（`helpers.h:245–291`），所以不能靠扫描它判断页一直idle。[来源与安全cut核查](../source-cut/findings.md)给出正常路径和限制。**不新增逐TX独立token，不等于零热路径成本：满额fallback可能要检查/合并pressure请求，正常frame的原有pp引用、索引查询和DMA同步照付。**

## 3. 选中版本的执行位置与合同

1. B准入遇K不足时，记录合并的pressure请求；当前工作按合法O0或原有错误路径完成，不等回收。请求必须绑定source pool、B/config及其有效期，不能只存可复用裸指针。
2. 在该源池原allocation owner的安全执行位置处理一个有界控制批次。只检查**当时实际存于private alloc.cache**的普通页，不按外部idle标志推断。`page_pool.c:422–436,721–732`要求cache访问非并发；`types.h:50–54`当前cache上限128，不是K、pool总页上限或扫描费用实测。
3. 暂时从cache移除选中页，阻止再次分配。其正常frame/fragment责任已经结束；未结束driver bias的页不能作为候选。`en_rx.c:274–302`和`helpers.h:245–323`仍适用，一包一页也不消除bias。
4. 对该页的B关联取得唯一关闭责任，停止新cache命中，保留关闭枚举责任；稳定同backend下在适当上下文完成原DMA前端的B unmap。不在数据锁下等待；若已由B关闭者持有责任，安全跳过/归还，不在NAPI读侧等它完成。
5. B撤图/同步完成、旧record不会再解引用页后，可重新发布该页给源cache。A映射、source归属、pool inflight不因此结束；不增加source release计数、不clear pp_info、不put回buddy。旧metadata仍被CPU读者持有时，继续退休；**K槽须到必要退休结束才返还**，不能在unmap发起时即返槽。
6. 新B需求只有在槽真正可用后才准入。以后该页重新访问B需要重新建图；是否值得清退由同信息普通规则决定，没有未来r输入。控制批大小、临时保存页/位置的空间与请求库存均有界并计费。

这是拟议的新接口合同，v6.12没有因此自动获得上述回收操作。原生安全位置有具体依据，但新索引发布、唯一unmapper、配置/pool寿命、控制请求交接与关闭竞争尚未经过实现/并发验证。

### 一个新的可达性依据：不必等新的RX包才尝试服务

root定向核到`en_main.c:2646–2651`的`mlx5e_trigger_napi_sched`，通过BH保护调用`napi_schedule`；同文件`:932`把page_pool连到RQ的NAPI。`en_txrx.c:124–198`中，positive-budget poll即使RX工作数为0也能走到post_wqes；budget=0在`:165–166`提前退出。因此可以把新控制hook投影到positive-budget poll、post之前，使用现成调度入口请求源执行方服务，**而不是假定某个冷页以后必会再次归还**。

这不是已完成的control pump：仍须新增并核对pending/ack、失唤醒、NAPI关闭和pool/config寿命协议，也不证明调度/撤图有墙钟时限。更关键的是该poll从`:143`到`:254`处于RCU读侧，**hook不能同步等待覆盖自身的CPU grace**。metadata可延后退休，返K槽也可能因此延后；CPU grace仍不替代设备DMA完成。所核片段未覆盖NAPI核心状态机，所以没有宣称失唤醒问题已解决。

## 4. 决定性预测与相邻失败条件

下面是来源绑定的手工偏序，不是状态模型、原生轨迹或性能结果。

**正向判别：** source cache中有已无所有frame引用、仍留B映射的冷页p；K已满，其他页q的本次B需求先走O0。pressure发布后，源owner获得一次合法positive-budget服务，在取下p期间不存在cache分配竞争；唯一关闭、B撤图和必要CPU metadata退休完成，再把p归回cache并返槽。下一个合格需求可以使用该槽。这个进展**不要求p再次RX分配、再次return、source退出或B全局排空**。旧V1无这个主动动作；只加return hook也不能据此前提保证腾槽。普通R获得同一接口同样通过。

**相邻失败一：** 所有B关联都在ring或仍在途，private cache无合格页。本版不会腾槽；工作仍走O0。不能把ring页悄悄纳入能力保证，也不能为找页擅自干扰其他执行方。

**相邻失败二：** 没有owner服务、B/config或pool已进入不允许新控制的关闭阶段、CPU metadata退休不前进。没有返槽时限保证；更不能把请求已发、页已取下或unmap已开始当成功。

**相邻失败三：** 外部控制者看到旧idle值后，页已重新分配并被B提交，然后才撤图。这是允许重新分配但未取得独占的竞态反例。需要owner实际取页，或另付每alloc/使用串行化协议；单独读pp_ref_count==1不能修复。

**价值失败条件：** K压力少、cache里没有适当冷页、清退造成频繁重建，或调度/扫描/同步/退休和源供给影响超过少做O0的费用。本轮未测这些频率与费用，不以一次容量序列推CPU净收益。

## 5. 与具名普通方案和最接近工作的差别

| 方案 | 已有能力与本轮差异 |
| --- | --- |
| stock O0：mlx5e跨设备逐TX map/unmap | **原本就能单独撤B且A页仍留pool。** 因而“B-only＋保留A”本身也不是新增贡献；本轮讨论的是持久关联仍可复用时的主动容量轮转 |
| V1-N/K旧版 | 能跨正常recycle保留B映射，无idle eviction。本轮增加owner显式取出冷cache页的动作；不再把该扩展偷偷算入旧版结果 |
| DMA_PMD RFC lifecycle02/cap05/allocation04/domain07 | 最后block归还后入idle，shrinker与分配器以同pool锁摘取，锁外卸全部域并退役整PMD；**安全idle cut＋禁止再分配的原理已存在**。不是直接实现普通page_pool基础页B关联单独轮转 |
| R-OB4K-T加本轮owner cut（具名普通构造） | 可实现相同选择、frame保护和控制动作，须同付hook/pump/index/close/退休。覆盖的是构造能力，不证明已发表网络实现或所有完整成本相同；不能认领候选策略独占 |
| dma-buf slot/聚合适配 | 获得同一owner取页接口，允许K个重用对象和热cache。标准sleepable unmap需要合法worker/保留页交接；私有DMA前端适配可取同位置，新增协议计费，不强加每packet export/map/token |

最接近原文位置：`lifecycle02:432–440`入idle、`cap05:230–249`隔离后退休、`allocation04:341–365`同锁再分配；`domain07:350–378`把02中的后续占位替换成真实`dma_pmd_unmap_all`。不能把系列中间占位当完整卸图实现。[前作核查](../closest-reclaim/findings.md)给出细节。

“同PMD中一个block已idle、另一个在途”只能作RFC粒度条件例，不能当本v6.12 order0应用实测。基础页选择更细，同时可能增加映射/PTE和失去大leaf覆盖收益；不能只拿PMD整退役的限制作弱对照。普通R基础页也有相同粒度机会。

## 6. 资源归因、贡献论证与下一最小动作

本动作回收的是B关联库存；源pool本来持有的物理页仍在，**不据此宣称payload RAM减少**。临时取页、metadata退休、PTE/IOVA和控制空间分别算；若变成worker交接而延后source真正退出，才另记实际B延后部分。复制池C没有凭本动作出现。可以恢复局部cache位置以减少不必要乱序，但控制CPU时间和回流时机会改变后续分配，不能拿O0固定PFN序列冒充候选实际序列。

可核查的贡献判断是：DMA_PMD已有allocator协同idle回收，O0已有保留A的B-only卸图；剩余具体问题是**普通page_pool保留源布局/消费者时，如何在有限持久关联预算下主动回收冷cache项，借既有frame寿命省掉独立TX token，同时支付控制和关闭成本**。本轮给出了owner执行位置、隔离/重发布合同和有条件进展序列。其潜在意义是以较低整合费用支持容量轮转；同接口普通构造可复现，因此安全原理和策略不构成独立新机制。能否形成接口/实现论文贡献，仍取决于相对具名前作/合理适配的实际部署或完整成本增量，本轮没有证明它。

**裁决：保留本cache-only扩展进入一个有界控制协议判别；归还时检查pressure作为受限变体，外扫pp_ref直接撤图版不成立。** 下一[最小合同](next-minimal-contract.json)只核pressure请求→owner服务→ack/退休的发布、失唤醒和pool/B关闭寿命，给一条可接受控制序列及相邻失唤醒/自等待反例；不重新扫描数据、不启动driver/collector/RTL/完整系统或状态/性能模型。若不能避免逐packet保护或费用/动作范围没有实际差项，限制该具体接口版本，保留已有条件结论。

B1/I/D/A5/A2、并发独立工程、MIG暂停及10月15—28日窗口均保持；不自动升位其他方向，也不等待用户重新确认一般授权、CPU/内存价值或提供保证成立痛点。
