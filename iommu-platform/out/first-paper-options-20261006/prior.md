## 证据表

| 文件 | 最接近机制 | 已实施 / 未来 | 限速 / 页预算 / 批量 / 按队列 | 关键定位 | 短引（每篇<25词） |
|---|---|---|---|---|---|
| `bermuda2025.txt` | SMMUv2 IOTLB/TCU竞争；per-Stream-ID QoS；static mapping/page coloring；63页循环与N=56/1992压力 | QoS、page coloring、prefetching、adaptive TLB partitioning 均为未来/未评估；实验只做竞争表征 | 未实施翻译请求限速；无不同页预算机制；无提交批量；仅未来per-stream-ID QoS，不是当前队列限制 | V.C `bermuda2025.txt:635-652`；III `:435-439,:449-452`；IV `:491-494` | “IOMMU interference can delay DMA transactions by up to 1.79×” |
| `iotlb_wall2023.txt` | HPA hugepage-aware allocator；2MiB IOTLB映射；LRO/TSO；locality-aware recycling；描述符batching建议 | HPA RX侧原型已实现并评估；大页恢复吞吐下降；background allocation、TX支持、locality改进、thrashing recovery为未来 | 未实施翻译请求限速；页预算是hugepage映射粒度，不是按地址空间的页数预算；batching只是建议；无按队列限速 | Abstract `iotlb_wall2023.txt:18-21`；Contrib `:120-127`；Dependencies `:341-344`；Takeaways `:153-155`；Future `:884-912` | “using hugepage-backed buffers can completely recover the throughput drop” |
| `spimpact2025.txt` | cache coloring；memory bandwidth reservation，按budget/period；框架面向LLC/系统总线 | 框架支持两类缓解；本评估实际排除MBR；IOMMU未纳入评估；QoS on-chip/DRAM traffic为未来 | 未实施IOMMU翻译请求限速；MBR是内存访问预算，不是IOMMU页预算；无翻译批量；无IOMMU按队列限制 | Abstract `spimpact2025.txt:42-45`；Background `:173-176`；Eval `:909-911`；Discussion `:1483-1485`；Related `:1527-1530` | “cache coloring and memory bandwidth reservation” |
| `riommu2015.txt` | 环形平表IOMMU；每ring一个IOTLB entry；burst末端才显式invalidate；N≥L；允许乱序使用已映射IOVA | 设计并模拟/评估；非rIOMMU模式为真实系统；rIOMMU本身用模拟 | 未实施翻译请求限速；页预算表现为per-ring平表大小N；有“burst末端失效”批量，但不是提交/翻译限速；per-rRING一个IOTLB entry是缓存配额，不是队列限速 | Abstract `riommu2015.txt:24-31`；Design `:417-420,:537-542,:558-565`；Applicability `:544-553,:585-596` | “each rRING has at most one rIOTLB_entry allocated” |
| `composition-v1-spec-20261004.md` | 当前平台：16 IOTLB条、mode S/N/P/PN、protected DID、P4M、V/A/other计数、GAP实际占用周期、实际IOTLB快照 | 规格冻结接口，未称上板通过；配置和观测已有定义 | 未见翻译请求限速接口；未见不同页预算接口；未见提交批量控制；未见按队列限速；只有offset2可写 | `composition-v1-spec-20261004.md:4-7,12,26,41-48` | “只有offset2可写” |

补充判定边界：上表只记录原文明确状态；术语未出现不作为“创新”证据，也不作为“未覆盖”的唯一依据。

## 对候选最强3个质疑

1. **大页、复用、缩ring是否已直接替代大部分收益。**  
   `iotlb_wall2023.txt`已实现并评估2MiB IOTLB映射，且“completely recover the throughput drop”；同文还建议locality-aware recycling。`bermuda2025.txt`也明确考虑过superpage，只是因cache coloring/灵活性等原因弃用，不能据此说平台不能做大页。若候选只是减少翻译压力，必须证明相对HPA、buffer recycling、ring/descriptor缩减仍有增量。

2. **“最近窗口新页数”是否只是misses/locality的另一种说法。**  
   `iotlb_wall2023.txt`已用“IOTLB misses per MiB”和address locality解释吞吐下降；`riommu2015.txt`依赖ring顺序并允许乱序使用。候选若按源/目的分别计新页，需证明它能解释超出misses/MiB、hugepage、reuse、N/ring大小的方差，否则只是重新命名既有局部性压力。

3. **completion后gap不能当作在途上限，per-queue/流限制也不是现成机制。**  
   `iotlb_wall2023.txt`明确“maximum number of outstanding translation requests (aka I/O page walkers)”是硬件不可改特性。`riommu2015.txt`只把N≥L作为设计约束，不是可控在途上限。当前平台`composition-v1-spec-20261004.md:26,48`只给GAP实际占用周期，并强调列实际gap、不把名义扣减当服务延迟。Bermuda的per-Stream-ID QoS是未来设想；rIOMMU的每ring一个entry是缓存配额，不是限速。

## 还可保留的最窄假设（可以否定）

在相同源/目的地址空间大小、相同页大小、相同复用/池/ring大小、相同已完成后台字节量、相同DMA队列/StreamID数、相同关键负载与频率下：**限制最近窗口内的新src页数和新dst页数，或延后新页翻译提交，会降低延迟敏感DMA的尾延迟**；且收益不能被大页、buffer recycling、ring/descriptor缩减、已完成后台工作下降、控制开销增加所解释。  
可否定条件：尾延迟不降；或降幅完全由后台完成字节下降导致；或HPA/reuse/ring shrink在同等有效工作下达到相同尾延迟且成本更低。

## 首个判别图应该控制什么才能不把减流量冒充收益

- **同有效工作**：后台以“完成字节/完成事务数”为横轴，或固定完成量；不能只按时间或offered rate画。
- **同关键负载**：同一V/A任务、同一DMA lane、同一payload、同一页大小、同一复用池/ring/descriptor数、同一SMMU模式、同一频率。
- **同地址空间**：固定源/目的页数、窗口、StreamID/队列数；源和目的分别计数，分别作为副横轴或分层。
- **必带基线**：default、hugepage/HPA、buffer recycling、ring/descriptor缩减、已有batch/quiet路径；不得人为禁用。
- **纵轴主指标**：关键DMA延迟尾部与均值；副指标：IOTLB misses/PTW per MiB、每完成字节的翻译次数、GAP实际占用周期。
- **后台指标**：后台完成字节/事务、后台实际有效工作、控制成本（CPU周期、失效次数、配置开销）。
- **画法**：若候选曲线只在更低完成量处好看，则无效；必须在相同完成字节或归一化为“每完成字节的关键延迟”后比较。若收益只来自减少后台完成量，则判为减流量冒充收益。

## 有限阅读缺口

- 当前平台是否支持大页、复用、缩ring作为同boot baseline：`composition-v1-spec-20261004.md`未给；只给chunk4096、Apages、Vpages等。
- `composition-v1-spec-20261004.md`中“batch”在drain/quiet中的确切语义，及其是否可作提交批量baseline，需读RTL/驱动。
- GAP计数是翻译等待、完成等待还是完成后退避：`:26`“GAP状态实际占用周期”未给硬件定义。
- DMA0/1“同512拍缓冲”是否为在途/完成队列：`:4`；未读源码不能当在途上限。
- ZCU104 SMMUv2是否实际支持superpage且能否与cache coloring共存：`bermuda2025.txt:435-439`只说不采用。
- SP-IMPact的MBR能否作用于DMA/IOMMU路径，还是仅CPU内存访问：`spimpact2025.txt:173-176,909-911`。
- rIOMMU的ring语义能否映射到当前SMMUv2/ZCU104队列模型：`riommu2015.txt:585-596`只讨论x86/Intel与SATA/RDMA边界。
- 原文未给源/目的地址空间分立的翻译压力代理；需自行定义并验证其相对misses/MiB、hugepage、reuse的增量。
