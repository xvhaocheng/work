# 持久映射的地址预算与回退能力裁决

2026-10-16 JST。**本轮最值得保留的版本是：V-C／持久B映射在合法streaming交接之外，还须按实际IOVA费用约束接纳；如要保证普通回退的地址供给，须另外给出可执行的domain容量／参与者合同。只有K槽并“满额就O0”不提供这个保证。** 这项纠正限制完成能力主张，不是已经发现原生耗尽或性能优势。

已找到正常源码来源：同一个mlx5 channel的普通skb TX与XDP redirect SQ都取`c->pdev`。在同一实际B device、稳定配置且使用dma-iommu的投影中，它们使用同domain地址分配资源。普通TX的map失败会计drop并释放skb；入口返回OK不等有效发送完成。

普通按费用配额O3-Q、domain容量合同O3-R以及预映射copy池O-COPY-B都必须获同样机会。**本轮没有找到独占的新预算算法；保留的是部署约束及整合价值，停止“有K就无条件安全回退”的扩大主张。** 未选论文主线，无源码改动、模型或原生实验。

## 1. 新问题与三个合理版本

上一轮已核B整页映射的可见性和完整交接。本轮不再拆该协议、不重算旧复制阈值；新的判别是地址库存是否允许完成相同有效工作。冻结Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`、mlx5e cyclic／linear order0 nativeXDP非XSK、固定B/config；T完整范围持续授权与S分开。实际backend、mask、窗口、strict/FQ、普通流量、压力和耗时未知。

| 版本 | 最强合理用途 | 本轮选择 |
| --- | --- | --- |
| 实际地址费用接纳＋受控容量合同 | 保留持久复用、V-C和现有source寿命，显式保护fresh map所需空间 | 重点分析；普通O3同样可做，不能只用本地K计数宣称全域保障 |
| 预映射B紧凑copy池 | 在有空闲已映射slot时，不依赖本包新分配IOVA；有限在途槽替代历史源页覆盖 | 具名普通替代，不是候选专属；付启动、RAM、copy、sync和完成费用 |
| 失败时清缓存／撤冷图／等待flush | 运用现有allocator恢复或既有owner回收 | 清rcache本来就有；live撤图和FQ实际归还另有费用／时点，不新造重复协议 |

[allocator核查](../allocator/findings.md)给粒度、允许窗口、rcache/FQ时序；[普通TX与对照](../ordinary/findings.md)给同设备来源、失败与完成路径。来源清单及核验见[source-manifest.json](source-manifest.json)和[source-verification.json](source-verification.json)。

## 2. 从源码能确认的资源事实

下文D=`drivers/iommu/dma-iommu.c`、I=`drivers/iommu/iova.c`、H=`include/linux/iova.h`；main/XD/TX为mlx5e对应源码。

- **同设备来源，不借用“同驱动”。** main:1459、1623都将SQ的pdev设为c->pdev；main:2691取该channel实际mdev的DMA device。XD:868、881选择目标channel的redirect SQ并以其pdev map。TX:179、196为同channel普通SQ做head／frags映射，失败查错。不是所有channel／两个mdev共domain的证明。全inline且无fragment的包不产生这里的map需求。实际同B dma-iommu时，D:851–854取同domain cookie／iovad。
- **失败不能算完成。** TX:211–213回滚已成功部分后返回ENOMEM；463–475走drop/free；687–696、788–803的成功CQ路径先调用DMA unmap helper再consume。该helper内部single/page分派未展开，不能把它的调用时刻说成FQ地址已归还。[固定官方TX源码](https://raw.githubusercontent.com/torvalds/linux/adc218676eef25575469234709c2d87185ca223a/drivers/net/ethernet/mellanox/mlx5/core/en_tx.c)。
- **窗口不是mask位宽。** D:671–731、777–803组合起点、有效DMA/bus limit、force aperture及domain；517–598还有reserved洞。低32位偏好失败后本已有完整允许limit重试，不能人为锁死低32位。I:163–221还要求合法连续／对齐空洞；另有GFP_ATOMIC metadata及PTE分配失败。
- **K能界本客户端live长度，不能预约其他用途。** D:866及H:58–66先按g对齐起点偏移和size；I:388–389对n<32的granule数向上取2次幂，较大请求保持长度但仍有自然对齐约束。有效初始化I:45要求g为2次幂且g≤PAGE_SIZE，所以本整CPU页p、offset0、sizePAGE_SIZE恰占PAGE_SIZE IOVA。K条此类live关联至多K·PAGE_SIZE；不含普通映射、reserved区及已脱离record的FQ债务。
- **补齐上一轮一个条件。** 在这条有效初始化的dma-iommu/iova路径内，g≤PAGE_SIZE且按2次幂，整CPU页起点和size天然满足其granule对齐；结合已核D:1179–1180，B整页分支不需要另测g来证明不bounce。实际是否采用这个backend仍未知，也不推出A无bounce、0 cache维护或整页授权天然成立。旧报告保留原文，由本新增来源收窄未知。

## 3. live、可复用cache与FQ债务必须分账

**普通恢复已经存在。** I:376–411先查合适rcache再fresh分配；最终失败时以flush_rcache=true释放online CPU cache与global depot，再重试一次。I:899–934、616–636会删除cached node，不会撤销仍有效的持久map。把这些闲地址都当不可恢复的负例无效，恢复开销也不能默认为零。

**unmap返回不总是IOVA返回。** D:825–845中strict先调用iotlb同步，再free_iova_fast进入cache/tree；FQ则queue_iova。D:149–184要求entry的计数早于已完成flush计数，之后真正free；186–199的timeout与233–235的queue-full可以触发回收。分配失败链D:762–806／I:376–411只清rcache，不主动flush FQ。当前entry是在queue-full的那次flush之后入队，不能把那次flush算作它已回收。

因此，record／K在一次unmap返回并结束其CPU metadata职责后可重新用，不代表该代IOVA已经返给allocator。多代FQ地址债务要另账。后续timer或queue-full能归还，故不是永久泄漏；源码timer配置也不是墙钟保证。这里读到的是通用接口次序／barrier／计数，并未验证具体IOMMU硬件callback、运行strict/FQ或实测延时。

若固定一个共同合法window、将permanent reserved先排除，地址占用可分为：持久live `L_p`、其他live `L_o`、尚未送回allocator的FQ `D_fq`、可依法复用或清理的rcache `A_rc`、tree剩余 `F`。这些不是物理页集合；`A_rc`不得与不可用live合并，`D_fq`不得提前当F。不同mask／大小／对齐、并发会使一个总字节数不足以证明可分配。

## 4. 一个决定性条件预测与相邻反例

以下只有手工来源绑定推导，**没有运行模型，没有给部署填小窗口或失败率**。

**P-IOVA：K回退的反例。** 取一个合法状态：K>0个已完成发送而仍有效的闲B关联，普通非inline B发送仍持有一些同domain映射；清除可回收rcache并考虑完整允许limit之后，已没有满足新请求的合法空洞。当前miss或普通TX fresh map可返回mapping error；XD不接受该帧或TX丢包。普通allocator不会因为K有限而撤销这些live关联。

相邻状态只取消这些闲关联，并在严格归还或真实FQ回收后使新请求有合法空洞、无并发抢占及其他分配失败，那么相同当前有效请求可以获得地址。这说明保留idle映射可能损失即时服务容量，不是O0对所有负载都成功。前缀需真实产生这些页／映射及合法回收状态；source已支持多页与跨recycle再用，但未取得该分布的原生频率。两策略改变控制／回收后，须各自导出分配轨迹，不能把一个固定O0 PFN轨迹硬套为性能结果。

**P-RES：受控容量的有限正向条件。** 仅在共同window、所有请求均为一个g-granule、无形状碎片问题、可执行地约束所有参与者且原子接纳的简化合同中，若非回退的最大live量加最大FQ债务及预留J个同时fresh请求不超过可用granule数，就可以排除这J个请求因地址容量不足失败。若是本整CPU页请求，换成PAGE_SIZE/g的费用，并额外保证可放置的对齐连续空间；总数量不再单独充分。metadata、PTE、device queue及其他失败仍另行处理。这是普通资源配额推导，不是新算法或完整服务保证。

| 相邻改变 | 判定 |
| --- | --- |
| 只有rcache闲node阻碍，或低32位失败但高位合法空间足够 | 必须先计普通清cache／完整limit重试；不能成立上述最终耗尽反例 |
| FQ已完成flush并实际返cache/tree，或所有退出均strict | 不再保留对应地址债务；不能借用旧债解释新失败 |
| 其他client未受约束，接纳用过时free字节快照 | 本地配额不等domain预约；正向容量保证不成立 |
| 有总空闲字节，但缺所需形状／对齐，或metadata/PTE失败 | 不得宣称dma_map成功保证 |
| 服务允许等待有界且实际满足的flush重试，或负载远离IOVA容量边界 | 即时失败或压力故事可能无主要价值；等待费用仍计，不发明时延要求 |
| copy池没有空闲slot、B无队列额度或copy带宽不足 | 已映射不等工作可完成，不能把排队／drop藏掉 |

## 5. 与具名普通方法的实质比较

| 普通方案 | 已有或可做的能力 | 对最强候选的限制与完整费用 |
| --- | --- | --- |
| stock O0／O0-T | 不保留本候选的闲live B map；正常rcache、完整mask重试；O0-T同享整页非bounce | 仍可能受其他live/FQ、碎片或metadata限制。每包建撤图／sync／失败回滚与完成均计 |
| O3-Q实际费用配额 | 同信息、同付费接口能以普通表／slot约束本客户端占用，拒绝接纳或依法退休闲项 | 当前整页live费用可直接由本源码导出，不强迫每次新查询；其他请求、退休反馈及实际参与者仍需来源。不是独占策略，也不等所有实现CPU相同 |
| O3-R共同window和参与者预算 | 静态上界、受控域或真正可消费的地址credit可保护普通余量 | 不能只在本地减R；未受控分配者能占走余量。I:506–549的reserve_iova把区间排除普通alloc；D:848–876普通map没有消费owner预留票据的参数。不能把函数名当已部署保底接口。新接口若需要，普通O3同享并付成本，本轮不实施 |
| O-RCACHE与合法冷项退休 | cache清理本来存在；已有owner协议可在合法idle点撤图 | cache flush不会撤live；owner处理／strict同步／FQ等待／重建与source供给反馈收费，不新开同协议分解 |
| O-COPY-B预映射紧凑目标池 | 启动成功、授权和sync合法、有空slot及TX额度时，承接包不需新分配B IOVA | 池自己预占IOVA及C_app RAM、copy和sync；J槽须覆盖同并发服务要求。普通TX如未适配该池，仍有自己的地址需求；不得拿只覆盖候选包的池保证全B服务 |

普通copy池按实际在途量映射，而持久源关联可覆盖很多历史源页，因此两者可能有不同地址库存范围；这不是自动优势，packing、页/缓存线隔离、最大包与并发会限制它。目标池／错误回退／completion适配仍未实现，不能免费假定本driver已支持。DAMN／DMA shadowing及DMA_PMD的已核持久／复制前史继续约束宽新意；本轮不新增论文或扩大前作覆盖结论。

## 6. 资源归因、完整费用与裁决

IOVA虚拟地址与PTE/metadata RAM分开。撤一个闲B映射时，p仍可能属于A source池S，不据此宣称释放payload物理内存；FQ中的地址条目与页表退休freelist也不是新保留同等payload页的证明。每策略物理按 `S∪P∪C_app∪C_sw∪Q_retire` 去重，源本来持有、B真正延后、手工copy目标、backend副本、退出暂存分别核；控制改变后续分配就重新导出本策略序列。

完整CPU包括byte/形状记账或静态配额检查、并发保护、lookup、streaming、prepare和CQ、rcache失败恢复、owner撤图、strict同步或FQ回收参与、重建、失败回滚、copy池启动／逐包copy与最终关闭。IOVA库存、物理RAM、带宽、有效完成和服务要求分别报告；删掉更多包不能当CPU优势，减少地址占用不能当RAM收益。不得因成本未知自动开发系统。

**裁决：保留“持久复用＋实际地址费用接纳＋显式普通服务容量合同”的具体版本；没有找到超过O3-Q/O3-R的独占预算策略。** 当前最有价值的新增认识是合法IOVA回退的适用条件、source-backed普通B TX需求，以及K metadata信用与FQ地址债务分离。若具名部署中能以更低完整成本给出普通对照尚无的能力，或同有效工作改善CPU／不可回收内存，才据实际差异发展贡献。当前材料只支持来源与条件合同，不支持独立新算法、原生净赢、地址压力必然存在或整个思想族已解决。

[最小后继](next-minimal-contract.json)限定为新匹配部署的窗口／消费者／请求形状／真实退休证据，或完整成本／具体能力差项；不再自动拆相同协议、重搜旧入口、重算阈值或以“还缺成本”追加开发。本轮结束；B1/I/D/A5/A2、独立工程、MIG1792023314暂停与10月15—28日窗口保持。
