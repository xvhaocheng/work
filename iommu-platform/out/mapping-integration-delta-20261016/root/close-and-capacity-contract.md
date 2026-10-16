# V1-N/K：局部关闭、发布与库存合同

2026-10-16。root直接源码核查后的条件设计；没有C实现、状态模型、形式证明或设备运行。这里的规则同样提供给O3与普通RFC基础页适配，不能用它们认领独占策略。

## 固定范围与最小表示

保留官方v6.12普通page-backed mlx5e RX→native XDP→B的单buffer链；**单buffer不等于一页只有一个frame**。固定一个B及其DMA配置/方向类别，T须覆盖实际完整映射粒度和后续用途。首版不提供独立idle eviction、活跃期domain/权限替换或任意backing迁移。改变这些条件须另履行关闭/排空，不能继续使用无token证明。

从K个预留记录中仅给实际接纳的源页分配关联；未接纳页不需要独立owner wrapper。pool级ctx拥有cookie、source索引、B可枚举索引和预留管理。记录含source page/owner实例、映射状态、B配置寿命、DMA base、唯一关闭状态、retire节点及CPU metadata回收责任。不能覆盖A的dma_addr、pp_magic或pp_ref字段。任一索引/record准备不足走合法O0；已经产生的映射和退出责任不能因预算不足丢失。

首次B需求持原frame责任，claim一个CREATING记录，发布至source与B可枚举集合，然后执行原DMA API。并发CREATING不睡等，可短map fallback；成功需按发布顺序使DMA地址先于LIVE可见，失败清理实际已做工作。仍需精确实现索引原子出版、target配置门与失败回收；文字不是并发正确性证明。

源page_pool最后用户协议而非新增TX token保护页。`helpers.h:227–238,245–323`说明pp_ref与bias；`en_rx.c:274–302`实际使用fragment bias；`page_pool.c:866–873`只在最后pp_ref归还后进入回收。因此在本窄合同中，合法TX/frame尚在时源正常return不可达；持久map直到源退出或B真正停止/排空才撤，不再每次token_get/put。

TX热路径仍读source记录/ready/DMA地址并做必要sync；**不是零查找、零metadata或全局无锁**。CQ只归frame，归还后不得再读该记录；持久mode可用mode+xdpf两项逻辑FIFO信息，不需要binding handle。普通适配同享此表示；未编译，不称sizeof/缓存行或速度改善。

## 源退出与B关闭

真正源退出统一在`page_pool_return_page:673–698`的release计账、clear_pp_info、put之前：关source新关联门、取现存记录、完成B旧图撤销后才执行原终结段。不能只接pool最终disconnect：缓存满、NUMA不匹配及最后fragment drain也会退出（`page_pool.c:390–407,818–829,884–934,990–1035`）。

稳定B、无关闭竞争且同步调用满足现有backend合同时，可在锁外同步撤B图。新依据是同一前端关系：已存`include/linux/dma-mapping.h:380–396`中single map转page map，single unmap直接转page unmap；stock B `en/xdp.c:683–684`和source A `page_pool.c:661–665`本就使用这个前端家族。因此同步分支有现有调用位置依据，不能仅因它位于source return就宣称必需worker。**这不证明任意锁上下文、任意backend时限、实际部署或新关闭协议已验证。** 大小/offset/attrs必须与持久map正确配对；IOMMU未同步或失败时不能先交回页。

B控制面先关闭新持久/短期TX及prepare gate，等待已有CPU/NAPI发布者退出，再完成真实设备DMA停止/合法排空，才枚举并关闭旧B映射。实际驱动有`xdp.h:138–150`禁新并synchronize_net，`en_main.c:3197–3216,3403–3418`先禁XDP后close channels，`:2077–2086`关闭SQ并清残余描述符。**synchronize_net只支持CPU访问同步；没有在本轮追到FW命令的真实DMA停止合同，不能冒充完成证明。** 配置/device/domain寿命必须覆盖最后unmap，普通get_device或RCU本身不够。

source/B并发关闭要唯一unmapper：记录从LIVE/CREATING取得关闭责任后，另一个关闭者不能双unmap，也不能把尚未撤图的记录从所有枚举路径摘掉。等待方不可在softirq持RCU或数据锁睡等。一个具体失败序列是：target已claim记录，等CPU grace；source在该读侧内退出并等target完成，形成循环等待。共同修复可选择先禁新并排完CPU读者再claim，及/或让source把页留Q后退出读侧；仍须证明worker、索引引用与关闭者不会构成另一环。

需要Q时，原source拥有页与pool inflight保持；未完不得先增加release_cnt、clear pp_info或put。预留retire节点接管真实退出责任，worker完成撤图/同步与必要metadata回收后调用一次原终结段，不再次递归hook。无活TX不免除CPU控制面引用、target关闭或后端同步义务。

## K约束的条件库存界

所有CREATING/LIVE/CLOSING/Q记录共用K个slot；未完成撤图和必要回收前不返还slot；每个slot至多关联一个order0源页。于是**关联所造成的Q库存至多K页**，record/retire节点预留同样有界。源页未建B图则不进入这个关联Q；槽满让新需求走O0，不得为准入无限增长Q。

这只是设计中的局部库存界，不是总RAM净差≤K页，更不是尾延迟或最终排空时间界。source替代分配、正常cache保留、其他引用、元数据和fallback IOVA/PTE另计；同一页属于S和Q时只算一次。Q worker或backend没有有界服务事实时，K只能限制积压数量，不能保证何时退役。所有普通对照获得同一K槽和回退合同。

## 本轮可接受与不可接受的主张

可接受：无idle eviction的窄合同不强制每TX独立token；source真实退出有统一位置；可安全延迟退出的责任和K库存界可以明确；一般owner-bound普通构造能使用同样原语。可以据此减少后续最小实现范围。

不可接受：该协议已通过并发验证；2个逻辑槽已省实际字节；同步unmap已满足硬件/时延；K保证总内存改善；取消token、Q或基础页本身就是新意。若要恢复独立eviction、迁移或细粒度撤权，必须重新引入相应使用/关闭机制及费用。
