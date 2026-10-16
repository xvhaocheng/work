# CPU费用与映射再用机会：可归属边界

2026-10-16。来源绑定的分析合同；没有采集、计时、状态/性能模型或新数字账。固定官方v6.12 native mlx5e linear/cyclic/order0单buffer条件链，实际部署仍未知。下面区分映射出现、有效工作和CPU费用，不能互相替代。

## 1. 现成时间戳到底是什么

`kernel/dma/mapping.c:155–180`：`dma_map_page_attrs`先选direct、dma-iommu或device ops执行map，随后kmsan、trace，再debug，才return。`183–198`：page unmap先执行backend，随后trace和debug。**两种page事件都是单个post标记，不是一对entry/exit。** SG unmap在backend之前（`:301–316`），也不能拿SG的事件侧替代本链page事件。

即便成功且无歧义地配对，`t(unmap-event)−t(map-event)`也只是两个API标记之间的墙钟区间：没有包含前一次map的backend费用，却可能含SQ处理、队列/设备等待、CQ调度和unmap费用，并漏掉事件后的debug部分。SQ未接受时也会立即unmap（`en/xdp.c:908–922`），所以短区间不自动意味着快速完成。不能称它为map成本、翻译延迟或完整packet生命周期。

`dma-mapping.h:380–396`的single包装含inline检查/debug，再进入page API。若只测out-of-line page函数entry/return，得到的是**较窄的page API包含费用**，不是整个driver调用点的single前端费用。完整口径须覆盖实际调用点和所有早退；core DMA事件也漏掉无dma_mask等提前失败，不能把事件数等于所有尝试数。

## 2. 未来若测CPU，最小有效口径

| 量 | 必要边界与字段 | 不能替代它的量 |
| --- | --- | --- |
| 单次B map/unmap前端CPU | driver调用前/返回后，operation ID、device/config epoch、页/offset/size/方向、返回/错误、CPU/执行上下文；注明是否只覆盖page API | map与unmap两个post事件的间距、静态调用次数 |
| inclusive与exclusive | 同一调用包含的IOVA、页表、同步、锁等child费用可计入inclusive；若细分child，不再把parent和child重复相加 | 把所有函数duration直接求和 |
| CPU与墙钟 | 记录clock/CPU迁移、抢占与IRQ归属，分off-CPU和真正执行；IOTLB等待若CPU忙轮询，其on-CPU时间仍属CPU，设备独立等待/睡眠另报 | 不注明时钟/上下文的wall duration、CQ等待时间 |
| 配置/初建与尾部 | pool配置、对象/索引预留、实际首次图、失败回退、source/target退役、worker/Q和必要最终释放分别计；窗口删失单列 | 只测hot hit却声称完整收益；把后台worker设0 |
| 有效工作/服务 | 同流量、route、长度、consumer、接受/完成/drop及服务要求；按真正完成工作规范化CPU | enqueue数、map成功数、CQ批数任一个单独充完成量 |

现有function graph可能提供某些函数边界，但实际编译可追踪函数、过滤、return覆盖和运行开销尚未核验。官方[ftrace 6.12说明](https://docs.kernel.org/6.12/trace/ftrace.html)明确sleep-time会改变调度外时间的计入，graph-time影响profiler的子调用计入，缓冲覆盖也会丢事件。因此不能把开了一个tracer或关sleep-time当已取得精确CPU费用。这里没有启动tracer；源码位置/方法建议不是已部署观测能力。

原生测量身份至少区分运行kernel commit/config、DMA backend/domain配置、debug/kmsan/trace状态、PAGE_SIZE与dma_addr宽度。`mapping.c:168–173`支持多种backend分派；出现DMA事件不证明IOMMU开启，IOMMU group不证明同domain或共享翻译服务。当前host只读结果是6.8.0-60-generic、e1000e/cdc_ncm、0个mlx5物理netdev，tracefs元数据EACCES；不是冻结v6.12双mlx5e工作流的运行验收，也不说明内核没有trace功能。

## 3. 能由来源给出的条件界

冻结一个source ownership episode和B配置/权限有效期的交集`g`。`r_map,g`只数其中已识别为本链B、成功建立的O0 page map；失败、SQ拒绝、接受、完成另分桶。对本轮linear/cyclic切面，实际每次RX allocation一页一packet（`params.c:256–281,693–707`及配置连线）。因此重复B请求须跨真正recycle/再次alloc，不能把同页小包packing或driver bias当额外packet次数。

只有在全部事件完整、episode闭合、每个成功图有正确结束、T允许整粒度、无B配置切换、首次接纳成功且忽略有限K/新增成本的理想化条件下，**最多可避免的重复map/unmap配对数量**为

\[
P_{\rm ideal}=\sum_g\max(r_{\rm map,g}-1,0).
\]

这是按O0来源轨迹描述的机会界，不是新的接纳算法、候选闭环结果或CPU数值。若只看真正接受且有效完成的子集，改用相应`r_complete,g`且保留拒绝/失败实际CPU费用；不能把两者混在一个胜率里。未闭合、丢失和歧义段只给明确的观测下界/未知，不补造有限上界。source没有给r的频率数据，所以当前`P_ideal`数值为unknown，不运行假数据算术来填它。

有限K、CREATING冲突、槽预算被Q/CPU回收占用、normal source exit与worker滞后都可能减少实际命中；ordinary O3/R/slot适配同享相同信息、接口、优化及成本。未来r不是在线免费输入。候选若改变分配、Q或backpressure，原O0 PFN序列不能直接作候选轨迹；该机会界的适用范围必须随之标注。

## 4. CPU机会界不等于总性能界

在同一已定义工作段，设`M0_B`为O0中B map/unmap完整前端实际CPU费用，`C0`为相同口径全部CPU费用。**只删除这些调用、其他路径费用冻结且不加入任何成本时**，直接节省的CPU不超过`M0_B`，占比不超过`M0_B/C0`。当前两者均未测，不能填百分比或速度比。

真实候选仍有首次/退役map、lookup/sync/索引、失败/O0回退、控制面/worker及source分配的变化。总CPU差应写成

\[
C_0-C_V=(M_{0,B}-M_{V,B}-H_V)+(A_0-A_V)+(R_0-R_V),
\]

其中`H_V`是新增管理CPU，`A`是source供给/回收CPU，`R`是其余实际路径CPU；定义时各项互斥，不把同步或后台费用重复/漏计。若候选改变其他路径，`M0_B`只是直接调用移除的预算界，**不是所有系统改善的普适上界**；也不能据此推多核吞吐或端到端加速。降低CPU可作主贡献，但须超过实际新增费用并满足同工作/服务要求，不设置无来源的任意百分比门槛。

物理RAM继续按每策略实际S/P/C/Q去重，元数据/PTE与IOVA另列，带宽和完成另报。stock O0无idle B pin；page_pool的cache/ring容量不等于全部source页上限；K是候选关联预算，不是从stock pool_size读出的现成资源上限。

## 5. 本轮判别结果

可接受：正常跨recycle的重复B使用有源码可达路径；已有标准事件可在合格episode/config下提供L0映射出现证据；额外entry/return、frame/SQ接受与CQ关系有明确最小字段；CPU直接预算和新增费用的比较边界可冻结。

不能接受：已有自然再用分布、原生CPU瓶颈或净收益已被证实；仅由hold/release、批sent或map/unmap间距重建完成/CPU；拿说明PMD账或M0/E3测量填原生网络费用；以当前机器不匹配或缺数据否定整个研究思想。

当前应保留V1-N/K作为低成本整合假设，停止重复同一原语/阈值/条件数字账。后继要改变价值判断，应取得来源和运行身份相符的最小L0记录及分离的CPU边界证据，或使用明确限定的条件分析；不能因缺这些就自动开发驱动/采集系统，也不等待用户提供保证成立的痛点。
