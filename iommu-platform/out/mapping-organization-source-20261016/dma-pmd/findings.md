# DMA_PMD 原始来源定向核查（2026-10-16 JST）

结论：10月3日RFC的关键代码确实把一个pool大页在首次使用它的domain中整页映射为CACHE/RW，普通pooled-buffer unmap不拆映射；它同时已经有多domain位图、domain销毁处理、全局cap、idle-only shrinker和partial优先/MRU组织。10月14日作者谈到的隔离选项、较小per-CPU cache及每设备RX pool，是正在做的更新说明；该回复没有贴修订代码。以上只证明所读RFC与回复的内容，不证明性能、实际漏洞或合入状态。

本席请求配置为Sol/high；实际独立运行模型与推理强度元数据unknown。共享IOMMU Codex-only，未用DeepSeek/Claude。采用ARS deep-research/fact-check的来源核查方法；主张范围限定在原始邮件、所选patch代码及作者/评审陈述，不套医学证据等级或期刊检查。作者报告自己的实现和性能，存在普通的自评利益关联；评审意见也未经本席复现。

## 来源与版本

所有相对路径均在本目录，全文取得日期/HTTP结果/字节及文本SHA256见 `source-manifest.json` 和 `download-receipts.json`。HTML是实际下载响应，txt是HTML内pre区去标签、解实体的可引用文本，行号不是Linux仓库源文件行号。

| ID | 日期、作者、版本 | 原始URL | 入口 |
|---|---|---|---|
| rfc00 | 2026-10-03 21:22:19 UTC，Luigi Rizzo，RFC 00/22 | https://lists.openwall.net/linux-kernel/2026/10/03/1066 | rfc00.txt:2–4,22 |
| lifecycle02 | 同日21:22:21 UTC，Luigi，02/22 | https://lists.openwall.net/linux-kernel/2026/10/03/1073 | lifecycle02.txt:2–4,22 |
| allocation04 | 同日21:22:23 UTC，Luigi，04/22 | https://lists.openwall.net/linux-kernel/2026/10/03/1068 | allocation04.txt:2–4,22 |
| cap05 | 同日21:22:24 UTC，Luigi，05/22 | https://lists.openwall.net/linux-kernel/2026/10/03/1079 | cap05.txt:2–4,22–23 |
| domain07 | 同日21:22:26 UTC，Luigi，07/22 | https://lists.openwall.net/linux-kernel/2026/10/03/1070 | domain07.txt:2–4,22–23 |
| mapping08 | 同日21:22:27 UTC，Luigi，08/22 | https://lists.openwall.net/linux-kernel/2026/10/03/1082 | mapping08.txt:2–4,22–23 |
| review1011 | 2026-10-11 09:02:51 UTC，Pranjal Shrivastava，对00/22回复 | https://lists.openwall.net/linux-kernel/2026/10/11/319 | review1011.txt:2–4 |
| review1014 | 2026-10-14 19:28:16 +0200，Dragos Tatulea，对00/22回复 | https://lists.openwall.net/linux-kernel/2026/10/14/2211 | review1014.txt:2–4 |
| reply1014 | 2026-10-14 20:14:41 +0200，Luigi，回复Dragos | https://lists.openwall.net/linux-kernel/2026/10/14/2319 | reply1014.txt:2–5,23–25 |

这里没有独立“10/14修订版”代码版本：所读10/14邮件是讨论回复。未核整个22-patch系列的所有相互覆盖，也未核作者分支、后续revision或最新全局合入。

## 决定性事实

1. **问题与作者性能报告。** 00/22说用物理连续2MiB页和大leaf映射、重用和延迟unmap降低IOTLB及CPU开销（rfc00.txt:45–55）。2×200G NIC双向样例是作者报告：DMA-FQ约440Gbps，DMA_PMD约745Gbps；计数器也同时变化（:94–112）。这不是本席测量，原文没有给本席所需的完整设备—IOMMU共享拓扑，不能直接当共享IOMMU干扰的因果证据。

2. **整页权限确为代码，不仅是评审描述。** `dma_pmd_dma_map_phys`把`block_prot`设为`IOMMU_CACHE | IOMMU_READ | IOMMU_WRITE`，首次在domain中映射`dma_pmd_meta_to_phys(meta), PMD_SIZE, block_prot`（mapping08.txt:323–338,378–402）。请求额外不支持权限位、非coherent、跨PMD范围等会返回错误，让外层走普通map（:71–79,336–338）；不应说所有设备/所有mapping必定使用该路径。单纯DMA方向所对应的READ或WRITE子集不使pool leaf收窄，代码也没有每buffer撤权。

3. **域追踪和快速复用已经存在。** domain07.txt:25–44描述64-entry registry与每PMD位图，实际registry在:149–176。mapping08.txt:236–273检查PMD页大小支持并取得domain index和IOVA window；:378–402先查位，首次map成功才置位。IOVA为`win->base + phys`（:445–449），window来自domain的IOVA分配器，并保留给此用途（:153–187）。这是多domain追踪，不等于所有参与设备位于同一硬件IOMMU或同一domain。

4. **普通unmap不撤leaf。** `iommu_dma_unmap_phys`发现DMA handle属于domain的DMA_PMD window就return，先于普通`__iommu_dma_unmap`（mapping08.txt:83–99）；patch说明明确无页表walk/IOTLB flush（:25–40）。SG路径把成功pool mapping标为bus-address（:103–119）；本席未扩展审计SG所有unmap消费者，因此SG细节不作完整调用链保证。普通unmap的逻辑生命周期结束与硬件授权结束必须分开表述。

5. **退役页的顺序。** `dma_pmd_release_page`由07/22加入`dma_pmd_unmap_all(meta)`，之后延续02/22的`meta->pooled=false; call_rcu(...)`（domain07.txt:375–381；lifecycle02.txt:205–215）。unmap_all先取出并清零位图，再对仍有效registry slot调用`iommu_unmap(domain, base+phys, PMD_SIZE)`（domain07.txt:202–233）；RCU callback随后逐block unfreeze/`__free_pages`并释放pool引用（lifecycle02.txt:168–193）。所以代码层可确认“调用unmap → 清membership → RCU宽限期 → buddy释放”。00/22把这称为同步IOTLB flush后释放（rfc00.txt:54–55）；本席没有取该RFC基线的通用iommu_unmap及硬件backend正文，故实际flush完成/失败语义不冒充已端到端验证。代码确未检查iommu_unmap返回值（domain07.txt:231），它支持评审担忧的静态事实，不证明真实失败已发生或能被利用。

6. **pool销毁不是立刻收回在途对象。** destroy设`destroyed=true`并取出idle链，锁外退役，flush reclaim work（lifecycle02.txt:318–343；domain07.txt:431–451增加SRCU保护）。07/22明确in-flight blocks或socket/SKB持有页保留pool ref，最后block释放后自动回收（domain07.txt:424–427）；allocation04.txt:334–337拒绝已destroyed pool的新分配。destroy的work flush也不等于等待所有未来消费者结束，RCU callback还可在其后运行。

7. **domain teardown的处理已经有，但不是每个buffer或每次解绑钩子。** 调用点是`iommu_put_dma_cookie`开始（domain07.txt:74–86）。domain_release把registry slot置NULL/draining，遍历pool的partial/idle/full清该domain位；调度并flush reclaim work，`synchronize_srcu`等已有unmapper，再允许index复用（:293–341,386–408）。forget_domain只清记录，不自行unmap；页表/IOVA的整体销毁由domain owner做（:237–244）。不能把这个实现描述为“没有domain销毁处理”，也不能由此推定每次driver unbind或DMA所有权移交都已撤授权。

8. **cap和shrinker已经存在且只对idle释放。** cap05.txt:32–43给全局PMD页数上限及shrinker，新增分配前原子计数检查在:143–150；默认约RAM/8、最少16个PMD页（:272–285）。shrinker count只累加`num_idle_pages`，scan只从`pool->idle`拆页（:194–249）。降低cap只阻止增长，现有页待idle再回收（:281–282）；不提供把仍有活对象的PMD立即拆回buddy的能力。源码的`freed`记账发生于detach阶段，而物理buddy释放还需unmap和RCU，不能把shrinker返回值直接当实际释放延迟测量。

9. **partial/idle/full与MRU紧凑复用已存在。** lifecycle02.txt:581–588定义partial是MRU、idle全部usable blocks空闲、full无free；:429–439把从full首次释放的页移到partial头、全空闲页入idle或异步回收。allocation04.txt:345–365先取partial头、再idle，最后才加新PMD；take_blocks耗尽后转full（:270–296）。这是具体已有策略，不能以“已有实现不主动复用partial”构造优势；也没有来自源码的保证说MRU在所有寿命混合下都能形成足够idle页。

10. **10/11评审的真实等级。** Pranjal质疑长期整页RW与strict模式隔离声明，提到共享TX pool、DMA-after-unmap、untrusted设备、VFIO/ownership transfer及unmap失败边界（review1011.txt:70–140）。本席直接核实的只有整页权限、ordinary unmap行为、cookie-release钩子和忽略返回值；kTLS/loopback实际数据链、外接设备可达性、VFIO重绑风险均未在本席复现，保持“评审指出的风险/待核边界”。评审提出opt-in domain policy及dense slot等建议（:91–98,219–225），不能当已实现功能。

11. **10/14是明确的进行中更新。** Dragos询问把CoCo/IOMMU优化选择分开、设备级pool及不必全RW（review1014.txt:195–207）。作者回复正在增加不同opt-in/isolation knobs（reply1014.txt:47–49），在做lean per-CPU cache，底层pool持有2MiB，cache持有较小集合；接收侧每设备一个pool供该设备所有pagepools（:55–64）。这是作者更新中的设计内容，不是本邮件已贴diff、已测更小浪费或已合入的证明。

## 对提案事实基底的接受范围

- 接受：大页/持久映射复用与细粒度撤权之间存在应明确的合同差异；多domain追踪、domain销毁、cap/shrinker和MRU均需纳入强对照（上述2–9）。
- 接受为条件问题：如果要求某buffer unmap结束其硬件授权，原RFC的pool fast path不能直接当满足同合同的精确map/unmap替代；如果整个可信池允许长期共享，则不能只因ordinary unmap不撤页就判它违反该合同（上述2–4）。这不替用户选应用合同。
- 不接受为已证事实：正常XDP路径一定混入互不授权对象、存在长尾混装造成重大持有内存、两个NIC共用特定IOMMU瓶颈、候选机制已有完整成本收益、已有真实漏洞、10/14更新已贴代码。此席不做XDP调用链、论文novelty比较或方向裁决。

## 阅读边界及失败收据

完整HTML/txt已保存不等于全文每行审计。阅读区间见manifest；特别是02/04/07只定向核生命周期/分配/域追踪，不把独立patch中后来被覆盖的注释当最终实现。web对08/22 timeout，随后直接HTTP下载成功；原失败和成功均记录。thread-next从10/11先到10/12的两封PMD大小讨论，不能误当10/14作者回复；通过10/14日索引与精确文本搜索定位2319。两封10/12仅浏览作导航，未成为关键结论证据；无本地全文hash。没有新模型、实验、构建、设备操作、生产源码或全局docs修改。
