# 映射关联与源owner退役：关键近邻能力核查

2026-10-16 JST。结论：**“source owner通知关联设备丢弃/重建缓存映射，以免必须长期pin backing”已有具体dma-buf动态attachment契约；DMA_PMD也已有owner持有domain位图、整页退役时卸所有图的具体版本。** 不能把通知、反向关联或退役时卸图这个总体能力当作新增。另一方面，这些接口不自动构成当前普通page_pool/XDP链中的低成本实现；可复用能力与网络路径的集成后果要分别核账。rIOMMU不是这个方向的持久payload池前作：它主要优化动态map/unmap及ring内失效摊销。

本席只核方法/接口，未设计新阈值、模型或协议，不做root的贡献/收益裁决。请求Sol/high，实际运行模型/强度元数据unknown；Codex-only，未用DeepSeek/Claude。ARS fact-check及PDF阅读纪律沿用已读入口。

## 来源、版本和引用读法

- rIOMMU：Moshe Malka、Nadav Amit、Muli Ben-Yehuda、Dan Tsafrir，*rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers*，ASPLOS 2015，DOI 10.1145/2694344.2694355。[作者PDF](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)。复用已有`out/shared-translation-literature-20261006/papers/riommu2015.pdf/.txt`，原始下载时间unknown；本轮web核作者原文存在但未重下PDF作字节匹配。新生成`riommu-page-NN.txt`按PDF页（1-based）分开，原文hash与阅读区间见manifest。p8图10/11渲染已查看，避免双栏伪代码混读。
- Linux固定**v6.12 commit `adc218676eef25575469234709c2d87185ca223a`**；tag→commit来自上轮已核官方tag收据，本席未重复联网解析tag。只新下载同commit三文件：`include/linux/dma-buf.h`、`drivers/dma-buf/dma-buf.c`、`Documentation/driver-api/dma-buf.rst`，放`raw/`。来源为官方维护者Torvalds GitHub仓库的commit固定raw，URL/HTTP/time/hash/git-blob计算值见manifest与`download-receipts.json`。
- 下文`h:N`指本目录`raw/include/linux/dma-buf.h:N`；`c:N`指`raw/drivers/dma-buf/dma-buf.c:N`；`rst:N`指`raw/Documentation/driver-api/dma-buf.rst:N`。`old-v612/...`与`rfc/...`均复用下述旧run原文，准确绝对路径和hash见manifest，不指生产树。

## 一、rIOMMU：先纠正“持久映射/allocator协同”的范围

1. **持久VM映射不在它的主问题中。** §2.1 PDF p2 `riommu-page-02.txt:82–104`：VM creation/migration类static/persistent映射明确不是重点；关注每DMA前后频繁建立/删除的dynamic/streaming/single-use映射。因此不能把它概括成“长留源池映射以等allocator通知撤销”的机制。
2. **ring本体持久和payload目标短命是两个表。** §4 p6 `riommu-page-06.txt:114–123`接p7 `riommu-page-07.txt:31–40`：每设备ring对应两个rRING，descriptor ring本体初始化时map，设备down才unmap；从descriptor取到的payload IOVA反复map/unmap，仅DMA在途有效。rPTE带物理地址、任意size、方向和valid，提出比页更细的保护。
3. **allocator是IOVA整数分配器，不是物理源allocator的退役合作接口。** §3.2 p5 `riommu-page-05.txt:14–44`的strict+优化IOVA allocation/free；§4 p7 `riommu-page-07.txt:101–120`与图11 p8 `riommu-page-08.txt:50–79`将map关联到device/ring ID，顺序分配flat-table tail、设置地址/size/dir/valid，unmap清valid、降nmapped、同步表更新。已核方法没有源page owner向多个缓存B的retire callback；这个限定只针对本文设计，不是全球不存在。
4. **缓存与撤销依赖ring顺序和新硬件语义。** §4 p6 `riommu-page-06.txt:78–98`及p8 `riommu-page-08.txt:82–119`：每ring至多一个current IOTLB项（另可prefetch next），新translation替换旧translation，驱动DMA完成后批量unmap，最后一项end_of_burst触发显式invalidate。不是“把普通多项IOTLB失效直接省掉”的可部署软件技巧；map/unmap顺序约束与设备可out-of-order使用有效IOVA也要分开。p9 `riommu-page-09.txt:6–22`还按最大在途IOVA配rRING容量，overflow需降供给或回baseline。
5. **不是实际rIOMMU硬件结果。** §5.1 p9 `riommu-page-09.txt:54–90`：真实执行rIOMMU driver代码，但无硬件rIOTLB，失效用2150 cycles忙等模拟，部分硬件翻译差异未计。本席未重跑。与本候选有价值的相邻认识是“将mapping组织绑定正常I/O生命周期并显式处理回收边界”，不是它已经实现跨source-owner的持久alias退役。

## 二、dma-buf：明确已有的可组合owner/importer能力

### 对象/关联/映射是分开的

- exporter拥有buffer分配、backing选择与迁移责任，多个importer经每设备attachment使用同一对象（`rst:39–63`；`h:491–520`）。`dma_buf_export`允许把allocator私有对象及ops包入dma_buf（`c:606–619`）。它不是要求每个用户都独立拥有一份payload。
- attach检查device DMA约束，必要时可迁移，否则可拒绝；map可集合所有attachments需求来选择backing（`h:46–72,124–160`）。这已是owner与外来设备约束的协同，而非调用方随便对任意PFN维持缓存。
- logical dma_buf本身引用计数；map/pin是另一个层次。最后put才调用exporter release（`h:197–203,290–300`；`c:757–773`），正常使用顺序要求先清map/detach，再put所持引用（`c:575–603`）。所以“无需长期pin backing”不等于“无需持有dma_buf对象，或owner可以无视活跃引用free”。

### 动态通知及不用长期pin的确切条件

- `dma_buf_attach_ops.move_notify`说明：提供回调可避免mapping存在时把backing一直pin；reservation锁由callback和map共同持有，以排除move过程中并发建立mapping（`h:470–488`）。`dma_buf_dynamic_attach`实际要求importer_ops带move_notify，并把attachment挂到owner对象列表（`c:903–934`）。
- `dma_buf_is_dynamic`由exporter是否有pin ops判定，`dma_buf_attachment_is_dynamic`由importer_ops是否存在判定（`h:573–598`）；不能只看用了dynamic_attach函数名就断言双方dynamic。
- core `dma_buf_move_notify`在reservation锁下遍历所有importer_ops并调用通知（`c:1264–1282`）。这是**实际已有owner→importer通知/反向关联接口**。但它没有直接执行IOMMU unmap、没有强行终止DMA、也没有自行等待所有hardware完成；header明确“已有mapping仍有效、callback本身不直接影响它”，importer须尽快销毁/重建，新map在callback返回后可得到新backing（`h:481–486`）。不能把callback返回写成硬件权限已经同步撤光。
- dynamic importer必须在device访问前遵守write fences，并为callback不能立即disable的访问设置fences（`h:410–427`）。这些fences、各exporter迁移实现与importer回调才组成在途DMA的履约；本席没有追加读取具体DRM/其他exporter，因此不声称某个设备的完整move/free序列已验证，也不给有限完成时间保证。
- `CONFIG_DMABUF_MOVE_NOTIFY`也是实际代码门：dynamic exporter在该配置关闭时map会pin、unmap会unpin（`c:1137–1151,1231–1235`）。双方dynamic且配置允许通知的路径才不在core做这项map-duration pin；实际部署配置、exporter策略未知。不能从接口愿景推出本机/当前网络链已经无pin。

### 两种map cache不能混在一起

1. **framework `cache_sgt_mapping`**：第一份每attachment map可缓存（`h:37–44`）；map有缓存就复用并检查DMA方向兼容，unmap遇到缓存sgt直接return（`c:1125–1135,1153–1156,1228–1229`），detach才调用exporter unmap并释放attachment（`c:1015–1036`）。
2. **dynamic importer自己的缓存**：header明确缓存由DMA-BUF core（非dynamic importer）或importer承担，exporter不应返回共享的同一sgt给多次map（`h:173–177`）。dynamic exporter的pin/unpin与framework cache flag互斥，`dma_buf_export`直接拒绝组合（`c:638–643`）。双方dynamic缓存应由importer配合move_notify/fences管理，不能把static framework的noop-unmap缓存和dynamic迁移能力无条件拼成一个免费模式。
3. **dynamic/static不匹配的退化**：`c:936–961`在一侧不能dynamic时预建BIDIRECTIONAL缓存；若exporter dynamic同时pin，直到detach卸图/unpin。这是持久化与可迁移性之间已有的明确退化，不是所有dma-buf attachment都具有无长期pin优势。
4. map/unmap本身允许sleep，实际map入口有`might_sleep`且要求reservation锁（`h:131–133`；`c:1118–1123,1221–1226`）。`dma_buf_pin`也只允许有限scanout等用途，不准用户无界pin（`c:1040–1049`）。这些接口不是现成NAPI热路径的无锁常数动作；适配成本/控制上下文必须实查，不能凭接口存在就令其零成本。

## 三、与page_pool / DMA_PMD及本候选的精确差异

| 已有对象 | owner握有哪些关联 | 什么时候处理 | 已解决什么 | 本轮不能顺带声称什么 |
|---|---|---|---|---|
| 普通v6.12 page-backed page_pool | 一个pool配置的device/direction和每页DMAaddr | 页留pool期间保留图；离pool卸本pool图后清metadata/put_page | A自身持久映射跟随owner退出 | 会自动发现新加的B alias并替B卸掉 |
| DMA_PMD RFC | 每PMD的domains_mapped位图＋domain registry | 整页所有blocks idle后退役；domain销毁也清追踪 | 同owner PMD在多个domain首次建图并在源页退役统一unmap | 首次引入source-owned reverse map/retire unmap；或已实现所有driver解绑/trust变化同步 |
| dma-buf双方dynamic | logical buffer对象的attachment列表，importer可缓存映射 | exporter发move_notify；锁/fences协调旧访问/新backing；最终detach/put另行处理 | 通知外部importer、保持logical对象同时允许backing迁移、避免无条件长期pin | core回调本身已停DMA/撤权限；或直接覆盖page_pool recycle的每种generation含义 |
| rIOMMU | device/ring对应的动态rPTE/IOVA | 每DMA map/unmap；burst尾显式invalidate | 用ring语义降低动态map及失效成本 | 是持久源extent缓存的allocator退役通知系统 |

page_pool直接原文复核：`old-v612/net/core/page_pool.c:461–489`在A `pool->p.dev` map并保留至leaving pool；`:648–698`退出只卸该device的保存DMAaddr、clear pp info、put_page；`:736–790`page_ref_count==1才能正常recycle，抬额外page ref会改变回收分支。参数`old-v612/include/net/page_pool/types.h:58–93`包含device/direction及init_callback，但这个callback明确是test-only的初始化callback，不能冒充通用退役通知。

**避免过度概括：v6.12 page_pool.c:679–682本来就有`mp_dmabuf_devmem_release_page`的provider分支。** 本轮冻结普通page-backed路径走else；本席没有扩扫该provider，故不说“网络没有dma-buf集成”，也不声称该provider已支持当前mlx5e native-XDP跨B缓存目的。

RFC复用/点核：`rfc/mapping08.txt:378–402`按domain首次map并置位；`rfc/domain07.txt:202–234,375–381`在源PMD release调用unmap_all遍历仍活domain；`:424–451`保留active blocks/pool直至最后block释放。其unmap返回值未检查、硬件backend同步完成未由本席重核，保留上轮边界；不能把常规调用顺序冒充失败安全证明。

DAMN复用：按设备/rights/NUMA分层池、chunk refcount与pool长期映射已经确认；其protection是CPU首读复制，zero-copy IP forwarding回DMA API；shrinker是原型未实现的方案描述。本轮不重读全文，不把这些已有事实重新写成新实验。

## 四、给root的有界接受结论

- 如果主张仅为“source owner列出B关联并在backing退役通知B撤cache，以避免额外长期pin”，dma-buf的dynamic attachment已实质提供这类组织接口；DMA_PMD同owner域位图也覆盖具体PMD退役版本。该总体能力不能认领为从无到有。
- 如果当前XDP/page_pool仍缺直接可用钩子，这只说明该冻结路径需要集成，不足以认领新接口/架构。要核差异，应与**具名dma-buf dynamic attachment/importer cache适配**和**DMA_PMD owner-bound bitset适配**比较同样信息、引用/DMA义务、锁与元数据、retire及错误成本；不是只打“给每个B map加page pin”这个弱对照。
- 复用旧组件不排除架构价值。如果存在明确可复现的新部署能力或更低完整实现成本，仍可构成有意义增量；本席未验证该差异、网络适配成本或源区/设备规模，不能提前接受，也不因通知已有而关闭整个组织思想。
- dma-buf能移动logical buffer的backing不等于能随意将同一旧PFN/旧IOVA授权给新generation的数据；“logical对象寿命”“物理backing move/free”“pool内数据复用/授权generation”须对齐。现有h/c并未给本候选的generation/权限变化协议，但补字段本身也不证明新意。

未做具体dma-buf exporter/importer实现、netdev provider、NAPI适配与backend IOMMU失败链全面审计，因而不提供完整安全证明或部署性能结论。没有抓第四个Linux源文件，没有模型/实验/构建/生产改动；所有新产物只在本`prior/`目录。

## 五、V1补核：dma-buf标准接口的对象与范围粒度

参数事实：`map_dma_buf(attachment, direction)`返回该logical buffer backing的已DMA映射`sg_table`，`unmap_dma_buf(attachment, sg_table, direction)`归还这份表；两者没有请求子区间的`offset,length`（`h:124–191`）。`dma_buf.size`在对象寿命内不变（`h:302–308`）；attachment绑定该dma_buf及设备，并可携exporter/importer私有状态（`h:491–521`）。`move_notify(attachment)`同样没有range参数（`h:470–488`），core的`dma_buf_move_notify(dmabuf)`遍历该对象所有dynamic attachments，通知其销毁/重建所有映射（`c:1264–1282`）；core的map/unmap wrapper也只传attachment/direction/表，不替调用者做子区间过滤（`c:1112–1156,1217–1236`）。因此标准边界是**logical buffer/attachment及返回的整份SG映射**，不是core提供的per-page/per-frame/range通知接口；SG可由多个物理段构成，不等于每对象一物理页。

普通adapter不应预先固定为每帧或每page导出：可以比较细粒度的多对象导出，也可以比较把source pool/span聚合成较少对象。前者能让对象级通知对应更细退役单元，但对象/attachment/reservation/引用及export成本随所选粒度变化；后者减少对象数，但标准通知不指明哪段改变、标准map返回的是所导出对象的SG，不能自动给它“只通知/只重映射一页”的免费能力，也不能未经适配证明就收费为每次必全pool硬件重映射。`dma_buf_export_info`可带自定size及allocator私有数据（`h:528–546`），attachment也有私有状态，因此**core没有range参数不证明importer不能自建per-range缓存、附加range协议或局部硬件重映射**；这些应作为同等授权、同等信息的普通adapter能力，实际维护、同步和失效粒度需具体实现后核账。这里仅确认标准API的表达粒度与设计取舍，不裁定per-range实现不可能，不据接口差异认领新意，也未新增adapter代码或实验。
