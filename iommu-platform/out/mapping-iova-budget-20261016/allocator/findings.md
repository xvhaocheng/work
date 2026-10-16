# IOVA分配、缓存与FQ合同核查

结论：**K只能界定受K完整记账的持久B live关联，不能为普通短映射预留地址，也不能保证其成功。** 本切面每个成功整页B关联的allocator占用恰为PAGE_SIZE；普通短map、O0-T及同一DMA domain的其他dma-iommu映射共享该domain cookie中的iovad。分配失败前已有rcache回收及完整允许地址上限重试，故不能把rcache空闲库存或32位偏好失败当最终耗尽；但固定v6.12的分配失败链**没有**主动冲刷FQ。FQ地址在满足flush完成计数后、实际free进入rcache/tree前，不是可分配供给。

这是Codex协助的有界原始源码核查，非native测量或最终贡献裁决。请求配置gpt-6.1-sol/high；独立实际model/effort metadata unknown。无DeepSeek、代码修改、模型、测试、构建、设备probe或板测。

## 1. 版本、入口与已核边界

固定Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`。下文D为复用原件`drivers/iommu/dma-iommu.c`，I/H为本run新增`drivers/iommu/iova.c`和`include/linux/iova.h`。逐文件绝对路径、字节hash、读取范围与HTTP记录在[source-manifest.json](source-manifest.json)、[download-manifest.json](download-manifest.json)。官方源码入口：[D](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/drivers/iommu/dma-iommu.c)、[I](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/drivers/iommu/iova.c)、[H](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/include/linux/iova.h)。

继承上轮已接受的full-granule T、合法streaming CPU/A/B交接与非bounce整页条件；不重做mlx5 TX链。这里仅核通用dma-iommu/iova胶水和allocator；实际IOMMU driver、架构、设备mask、domain运行类型、其他用户库存及耗时未知。真实driver是否落实其TLB同步接口不能由本报告的调用名推出。

## 2. 粒度、rounding和占用

| 原始位置与条件 | 已核事实及边界 |
| --- | --- |
| D:684–698、714–715；I:36–55；H:48–80 | granule g取domain->pgsize_bitmap最小支持位；iovad start至少1并受aperture_start影响。I:45要求g为power-of-two且g≤PAGE_SIZE。PFN单位是g，不是CPU页数。 |
| D:848–876；H:58–66 | map先取物理起点对g偏移o，映射/分配请求字节m=ALIGN(size+o,g)，IOVA基址映射phys-o，返回base+o。请求有效payload长度与实际PTE覆盖范围分别记。 |
| D:774–775；I:382–389 | 请求n=m/g个granule。I原文关键条件：`if (size < (1 << (IOVA_RANGE_CACHE_MAX_SIZE - 1)))`；常数为6（I:19），因此n<32时向上取power-of-two，n≥32保持原大小。n=32本身已是power-of-two。 |
| I:163–175、208–210、239–263 | fresh tree分配基址按roundup_power_of_two(allocator长度)自然对齐，实际node长度仍是分配长度。非power-of-two的大请求不能把对齐间隙都算成该map的node长度；碎片会影响可达。 |
| I:416–429、801–809 | free_iova_fast按原始请求size的order选缓存桶；小请求的分配rounding保证free桶匹配。缓存失败/超范围才删tree node。 |

定义q(n)=roundup_pow_of_two(n)（n<32），否则q(n)=n，allocator实占为q(n)g。对p页基址、offset=0、size=PAGE_SIZE的order0整页B map，g≤PAGE_SIZE且两者为power-of-two，所以n=PAGE_SIZE/g已是power-of-two，q(n)g=PAGE_SIZE。**K个此类live关联至多K·PAGE_SIZE的IOVA node长度**，条件是每个关联只有一个映射、K涵盖所有live关联；这不含其他映射、reserved范围、尚未完成的退出或FQ债务，也不证明有一个连续空洞给下次map。若K槽先释放而IOVA仍退休，退休量须另账；不能继续把“record数≤K”当总占用界。

普通payload map的o、len与bucket rounding可能令其占用不同于payload bytes；O0-T整页fresh map的单次占用同为PAGE_SIZE。source物理页本来存在不代表新添PAGE_SIZE RAM；本节只是IOVA地址占用，PTE/元数据RAM另计。

## 3. 共享域与允许地址范围

D:60–86把唯一iovad与FQ置于domain cookie；D:391–400为domain建立cookie。D:851–854、1163–1173和1376–1381分别从device取得DMA domain再取同一cookie/iovad，D:868、1470都走同一iommu_dma_alloc_iova。D:1659–1660的普通coherent分配也以该device的domain进入同一map helper，但用coherent_dma_mask；故**同一B DMA domain**内的普通短map、O0-T、SG及此类分配竞争同一个地址库存，不能按用途名分开。不同domain或绕过该backend的映射不在此结论内；同一个设备的DMA mask与coherent mask也可不同。

D:777–780先将调用方dma_limit与非零bus_dma_limit约束合并，force_aperture时再受aperture_end约束。D:517–598还把PCI windows、DMA window外范围和IOMMU reserved regions纳入tree预留（SW_MSI特殊处理）。因此设备宣称的mask位宽不是全部连续可用容量；保留洞、起点、碎片和调用方mask都须按实际域核。

I:505–547的reserve_iova是把范围插入/合并到iovad tree，使普通alloc_iova不再分配该区间；它没有owner票据或交给某次dma_map_page消费的参数。D:848–876的普通map也没有接收预留句柄/指定预留IOVA的入口。因此不能以reserve_iova函数存在，宣称已有“给普通fallback留地址且map时自动消费”的保底API；若只reserve一块再临时释放，释放与普通fresh map之间的并发竞争仍须另解，且源码未给原子兑换合同。

D:793–803：当dma_limit>32位且该device的pci_32bit_workaround仍真时，先以32位上限、flush_rcache=false尝试；失败后清除此device标志，随后按完整有效dma_limit、flush_rcache=true尝试。D:1747–1755显示PCI初始偏好受iommu.forcedac控制。**不能把32位第一次失败当最终失败，不能固定假设永久只用低32位。** 反之若真正有效mask仅32位，不能借用高地址救场。标志按device、iovad按domain，二者作用域不同。

## 4. 分配零值和已有恢复能力

I:366–411的真实链是：先合适size桶的本CPU loaded/prev/depot候选，再fresh tree分配；若fresh失败且flush_rcache=true，则释放所有online CPU该iovad的缓存与global depot，重试fresh一次。I:899–934及616–636证实释放缓存会删除仍在tree中的cached node，而非撤销live map；I:817–865限制cache取值满足size桶及limit。跨CPU缓存不能在简单统计中当永远不可回收的地址。

关键原文锚：I:403 `Try replenishing IOVAs by flushing rcache.`；I:405–407的online CPU与global释放后I:408回到retry。该路径**没有调用domain flush、FQ drain或等待timer**。D全文fq_flush_iotlb引用只落在timeout与queue-full路径（见下节），D:762–806分配helper也无此调用。此否定限定于已核固定文件/commit的这条分配链，不声称所有Linux版本或其他IOMMU实现如此。

I:258–267表明fresh分配还可能因GFP_ATOMIC元数据分配失败返回NULL；I:163–221也可能因无满足大小/对齐/limit的空洞失败。I:400–408最终返回0，D:805将PFN转字节地址仍为0；D:869–870把它转成DMA_MAPPING_ERROR，**0是allocator内部失败sentinel，不是上层成功DMA地址，也不等于已证明地址空间总字节用尽**。D:666–669、684–698确保dma-iommu不分配零页；D:872–874另有PTE建图失败并归还IOVA的分支。普通消费者必须按DMA API检查mapping error，不能仅以非零等同map成功。

rcache恢复的费用是锁、遍历、删tree和重试，源码不提供native低成本保证；但强普通对照本来就有此能力。持久live关联在未走unmap前不会进入rcache，allocator缓存flush不会主动驱逐它。

## 5. strict、FQ与实际可分配的时间点

| 阶段 | strict路径 | FQ路径 | 来源 |
| --- | --- | --- | --- |
| 退出原map | 先将base/size还原成granule覆盖范围 | 同左 | D:825–838 |
| 撤图请求 | 调iommu_unmap_fast；期望完整size被撤，WARN检查 | 同左；gather.queued由当前fq_domain确定 | D:838–841 |
| TLB接口次序 | iommu_iotlb_sync返回后才free_iova_fast | 此处不调用sync，进入queue_iova，旧translation可能待flush | D:843–845、808–822 |
| 排队与flush | 无该FQ环节 | queue_iova中的smp_mb明确排序此前pagetable update与跨CPU flush观察；它不是DMA数据交接替代品 | D:210–217 |
| 归还准入 | free_iova_fast可进rcache或删除tree | entry.counter必须严格小于fq_flush_finish_cnt才free_iova_fast；计数在flush_iotlb_all callback返回后增加 | D:149–184 |
| 触发回收 | 后续可cache复用或fresh tree取 | timeout先flush再遍历相应queues释放；queue入队前先清已完成项，满时flush并清后再加入当前entry | D:186–199、224–243 |

此处PTE无效的可核边界是**通用胶水已调用unmap hook并检查返回尺寸**，而非已核具体driver的PTE写入、硬件walk停止或体系结构完整屏障。严格路径通过iotlb_sync先于free表达接口完成次序；FQ通过明确barrier及完成计数表达延迟地址回收次序。真实driver的callback必须兑现其接口语义；本席未读driver，不能用函数名当实际硬件完成证明。

特别注意：queue-full分支flush发生在加入当前entry之前，当前entry随后采样新的start计数；不能用这一次flush当当前entry已经安全可复用。已完成全域flush但还没fq_ring_free的entry仍未送入allocator；pending FQ entry既不是live PTE承诺、也不是已可分配cache。默认timeout/队列常数（D:105–111）只属源码配置，不是runtime时延上界或native事实。

因此，即使K记录在正确unmap返回后才释放，**FQ下K_live仍不一定界domain IOVA占用**：unmap已返回并清record，不等对应entry已经经flush计数检查并free_iova_fast；连续合法换页可以留下多代退出地址债务。严格路径返回前已按接口同步并归还allocator，但地址可能保留在rcache以待复用；cached、FQ pending和live三类应分别记账，不能把rcache retention当live授权，也不能把FQ pending算入立即空闲。

## 6. 可以交给root的判别边界与相邻反例

1. **K不会预留普通fallback。** 若只约束持久关联数，其他live map/保留洞/对齐或退休可以占满某次普通请求允许的空间；并发也可抢走账面剩余。普通fallback欲获得保证，需要覆盖其实际size/limit/并发及失败分支的资源合同。此处不选择实现机制。
2. **cached地址不能充当不可恢复耗尽反例。** 若阻碍只来自rcache空闲node，完整limit调用已清online caches/global并重试；如果回收后有合格空洞且元数据分配成功，不能继续称普通分配必失败。若低32位失败但完整mask仍有空间，也必须计其正常fallback。
3. **pending FQ债务不能当即已回收，也不能当永久不可回收。** 源码允许分配在下一次timeout/有效queue-full回收前返回失败，因为本分配链不主动flush FQ；但timer/queue机制后续可回收。若服务合同允许等其回收重试，或实际退出走strict，本局部即时失败就不能推出永久不可服务、无解或候选独占能力。
4. **live关联可阻碍即时fresh map，rcache flush不能解除。** 只有当真实退出/驱逐完成并进入可复用状态，地址才成为供给；不能在决策时只减K记录数、把原IOVA立即计为空闲。持久方案若主动驱逐，也须计strict同步/FQ延迟、控制与重新接纳费用。
5. **总空闲字节并非成功充分条件。** limit、reserved洞、bucket rounding和自然对齐会限制可放置空洞；metadata失败又独立于地址库存。因此不得仅用K·PAGE_SIZE与一个未经核查的窗口相减宣称success guarantee。

以上均是来源约束与条件推导；没有填IOVA窗口、压力、时间、失败率或吞吐数字，没有选择论文主线，也不裁决地址预算策略是否独立创新。

## root最小直接复核范围

- D:762–876：有效limit、32位偏好回退、共用allocator、map失败及strict/FQ分流。
- I:376–429：cache rounding、失败清cache一次重试、free缓存语义。
- D:149–251：flush计数、明确barrier、timeout和queue-full；确认分配链没有额外FQ drain。
- I:36–55和H:48–80：g与PAGE_SIZE约束；I:163–221补对齐和真实node长度。
- D:1163–1173、1376–1381；D:517–598按需：共用cookie和reserved洞。
- I:505–547：reserve_iova只是排除普通分配，无owner预留票据兑换。

## 检索与验证receipt

新增2份HTTP200原件；复用D无再下载。实际5次本地逻辑HTTP请求：raw iova.c 429、github/raw iova.c 429、github/raw iova.h 429，随后在停止网络指令送达前已完成官方GitHub contents API两次HTTP200恢复（raw媒体格式，非网页转录）。不隐去失败；底层redirect次数未独立计量。官方web open1次（I的固定commit blob），无search、无新论文。原件sha256/bytes/line_count及git blob SHA1由保存字节计算，HTTP字段原样保存；复用D与既有manifest hash/bytes/blob一致。验证只检查保存材料与引文范围，不运行源码。
