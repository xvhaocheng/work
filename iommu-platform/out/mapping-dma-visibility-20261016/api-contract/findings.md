# DMA streaming前端合同：持久B整页映射跨包复用

2026-10-16。任务MAP-DMA-VISIBILITY-CONTRACT-1的API独占席。请求配置gpt-6.1-sol/high；实际独立model/effort元数据unknown。Codex-only，未用DeepSeek，未修改driver/ABI/测试/模型，未探测host/设备；actual deployment和native费用均null。本报告是固定源码与官方合同核查及条件推导，不是已经实现的B cache。

## 结论与边界

**B映射句柄存在，不保证后来由A或CPU改写的数据已经适合B读取。** 对每一轮新包，先确认上一轮B停止访问，再完成合法的所有权交接；在A RX完成并完成A→CPU同步、CPU/XDP最后修改之后，以B的device/dma_addr/direction对本轮B会读取的范围执行for_device。这个步骤不能由旧map或A的sync代替。TO_DEVICE只约束B不写内存；A另有自己的BIDIRECTIONAL映射，不能因此说A也只能读。

**保守、可移植的持久streaming事件模板应保留B的for_cpu交接。** B_CQ只证明这次B操作完成；它没有调用DMA API，也不自动完成该映射从device侧回到CPU侧的同步。B为TO_DEVICE时，for_cpu通常没有“取回B写入数据”的需求，具体后端可以不做cache/copy工作；不能把“后端此方向无工作”偷换成“CQ是一种通用cache ownership API”。要在特定配置删掉调用或合并交接，必须有该配置的API/backend证明，不能仅凭B只读、A已sync或硬件coherent。以下模板是充分做法，不主张所有部署都必须支付非零的B for_cpu费用。

**整页map与每包range-sync是两个范围。** 拟议首次`dma_map_page(B, page, 0, PAGE_SIZE, DMA_TO_DEVICE)`作用于整页；以后`dma_sync_single_range_for_device(B, b_dma, off_p, len_p, DMA_TO_DEVICE)`可只同步本次packet。前者没有因后者存在而变成“只为payload做首次同步”。首次整页map所涉及的整页缓存线、CPU/A所有权和无在途访问条件须成立；现mlx5 linear路径仅做A局部for_cpu，不能单凭那一行宣称已建立整页交接合同。可保守增加A整页for_cpu和B整页for_cpu作为合同构造，但其成本必须计入，也不能说现驱动已有这些新增动作。

## 原文入口与源码证据

以下本地相对路径均相对于`/home/xvhaocheng/work/`；行号按保存的raw原始文件，一基计数。`S/`简写为`iommu-platform/out/mapping-admission-20261016/v612-source/raw/`；`D/`简写为`iommu-platform/out/mapping-dma-visibility-20261016/api-contract/raw/Documentation/core-api/`。完整绝对入口及hash见source-manifest.json。

| 判别点 | 直接证据 | 能保证／不能推出 |
|---|---|---|
| 持久streaming可重复使用 | `D/dma-api-howto.rst:627–665` | 正式提供反复DMA与中间CPU触碰的sync模式，最终unmap；不能推出任意map存活期间都无需同步。 |
| TO_DEVICE方向 | `D/dma-api.rst:300–305`；`D/dma-api-howto.rst:471–498` | 软件最后修改后、设备接收前同步；被映射的该设备只读。方向不是“整个物理页所有设备都不能写”。 |
| BIDIRECTIONAL双向交接 | `D/dma-api.rst:312–319` | device前和device后CPU访问前均须相应sync；A映射在native XDP为BIDIR。 |
| 局部sync合法 | `D/dma-api.rst:449–465`；`S/include/linux/dma-mapping.h:398–409` | sync_single允许handle/size不同于原map以同步子范围；range helper实际就是原dma地址加offset后调用single。不是新映射，不改变原map尺寸或权限。 |
| map/unmap尺寸与缓存线 | `D/dma-api.rst:287–298,327–346` | map区域边界/缓存线共享须正确；unmap参数与原map对应；整页B映射最终应以原整页尺寸与TO_DEVICE解除，不能以最后一个packet的长度unmap。 |
| 不跨device广播 | `S/kernel/dma/mapping.c:362–394` | 根据传入dev选择direct/iommu/ops并将同一addr/size/dir交给后端；没有遍历其他设备映射的逻辑。A sync不等于B sync。 |
| API可无后端工作 | `S/include/linux/dma-mapping.h:295–356`；`S/kernel/dma/mapping.c:430–464` | wrapper可依据device能力/配置跳过后端，dma_need_sync可判定特定address；coherent初始化后仍可能因SWIOTLB映射重置skip状态。不能无条件“coherent所以所有B sync永远可删”。 |
| SKIP_CPU_SYNC的前提 | `S/include/linux/dma-mapping.h:35–40` | 其注释假定buffer已转device domain，允许平台跳cache sync。不是新包数据已存在于B私有映射/bounce缓冲的证明；是否仍需copy由backend席核定。 |

## mlx5/page_pool实际放置

1. **A配置确实是order0整页persistent page_pool。** `S/drivers/net/ethernet/mellanox/mlx5/core/en_main.c:838–843`在有XDP程序时选`DMA_BIDIRECTIONAL`；`923–935`非XSK分支创建order=0、`PP_FLAG_DMA_MAP | PP_FLAG_DMA_SYNC_DEV`、dev=`rq->pdev`、max_len=`PAGE_SIZE`的pool。`S/net/core/page_pool.c:216–238`限制pool方向为FROM_DEVICE或BIDIR并启用sync标志。这是本候选固定场景，不推广所有mlx5模式。再复用并直接核`S/drivers/net/ethernet/mellanox/mlx5/core/en/params.c:256–281,296–317,693–707`：linear大小受一页容纳约束，XDP stride提升至PAGE_SIZE，num_frags=1；`en_main.c:604–634`按stride分区，冻结场景的wi.offset=0且每allocation只一packet。因此不把同页其他在途RX fragment当本候选反例。
2. **A首次map与回收for_device。** `S/net/core/page_pool.c:461–480`以offset0、`PAGE_SIZE << order`、pool设备和方向map并使用`SKIP_CPU_SYNC | WEAK_ORDERING`，随后调用pool for_device；`439–458`按pool offset及`min(dma_sync_size,max_len)`同步；`740–767`在可回收页进入cache/ring前执行此操作。SKIP未取消后续明确for_device；此处全部属于A。
3. **A RX→CPU/XDP。** `S/drivers/net/ethernet/mellanox/mlx5/core/en_rx.c:1662–1706`的linear函数在`1682–1684`对`wi->offset, frag_size`做A for_cpu，之后才prefetch、准备xdp_buff、调用XDP处理；这里传入的不是固定整页常量，也不是裸payload长度：`1680`是`frag_size = MLX5_SKB_FRAG_SZ(rx_headroom + cqe_bcnt)`，与PAR:698的MTU/byte_count字段frag_size及PAR:699的PAGE_SIZE字段frag_stride各自不同。宏定义所处en.h未在本批保存raw内，未额外抓取，所以本报告保留确切表达式，不凭猜测写出它对每包的数值；stride=PAGE_SIZE不能证明该sync调用实参恒为PAGE_SIZE。`S/include/net/page_pool/helpers.h:424–443`提供相同意义的CPU sync shorthand，独立于是否设置pool for_device。CPU/XDP需要触碰的frame header/headroom/元数据区域可能大于B最终读取的payload；两者范围不能强行画等号。
4. **同设备XDP_TX有现成复用，但不是候选B。** `S/drivers/net/ethernet/mellanox/mlx5/core/en/xdp.c:119–126`复用page_pool地址，对TX长度执行BIDIR for_device；此处属于原RX页的本地XDP_TX。不能拿它作为跨另一device的B持久TO_DEVICE映射已经存在的证据。
5. **跨目标的现路径每包map/unmap。** 同文件`847–885`的`mlx5e_xdp_xmit()`按每个frame data/len给`sq->pdev`做`dma_map_single(...TO_DEVICE)`；`659–684,700`的FRAME完成释放路径先`dma_unmap_single(...xdpf->len,TO_DEVICE)`，再return frame。`914–921`提交失败时同样unmap。本候选把这一B packet映射替换成持久整页关联是拟议修改；现代码未提供这个cache。该map/unmap也履行对应数据同步，删除它们时不能只核IOVA/PTE寿命。

## 一条保守的合法事件序列（设计推导，非既有实现）

冻结条件：整order0页独占本轮单buffer；上一轮B completion准确表示不再读该页；页、A/B device/config及mapping lifetime稳定；所有实际CPU触达/设备触达范围已知并有缓存线安全条件；B只读；新A RX在上一B结束及交接完成后才能覆写；错误/teardown同样先停止访问再回收。只保留B映射不赋予B无限时访问权；T持续授权仍是部署假设，S另行判别。

- **建立B关联的第一轮：** A完成RX → 在CPU/XDP触碰之前完成A for_cpu；若采用A整页for_cpu作为首次B整页map的保守交接条件，必须在此时做，不能在CPU/XDP已经写脏frame/metadata后盲加整页invalidate → CPU/XDP完成修改、frame转换及所有相关frame/inline准备，预拷后续所需局部值并重排必要工作 → `b_dma = dma_map_page(B,page,0,PAGE_SIZE,TO_DEVICE)`并检查错误 → 不再触碰已handoff区域，以`b_dma+off_p`及len_p提交B。默认首次map已同步整map范围，但“map之后直接调用现xmit”不构成candidate全页CPU交接闭合的证明：`en/xdp.c:595–599`可能在xmit中读取payload以memcpy inline字节，`925–940`仍使用has_frags控制信息（确切地是已预存的`xdptxd->has_frags`局部字段，并非该处重新解引用frame）。另一条充分路径是B整页map后先B for_cpu整页 → 完成CPU frame/inline准备 → 对B将读取的合法range做B for_device → submit；最后handoff后避免再次访问该区域。这里列的是保守充分条件，不据此宣称stock TO_DEVICE inline读取错误，也不主张所有平台都必须增加这些调用。
- **B完成后：** 收到并验证B completion → `dma_sync_single_for_cpu(B,b_dma,PAGE_SIZE,TO_DEVICE)`（保守整页，从首次整页handoff收回，或使用经过证明的范围状态机；具体TO_DEVICE后端可无工作）→ 现在才允许需要CPU-side ownership的CPU写/metadata复用及转交A。不能把下一步A for_device算作前一步B for_cpu。
- **新一轮A写：** 完成CPU访问 → A `for_device`按A的BIDIR映射和RX触达范围交给A（现page_pool回收路径承担其已覆盖的范围）→ 发布RX descriptor → A RX写入新包 → A completion → A `for_cpu`覆盖本轮CPU/XDP触达范围 → CPU/XDP处理新包。
- **新一轮B读：** CPU/XDP最后写之后，`dma_sync_single_range_for_device(B,b_dma,off_new,len_new,TO_DEVICE)`，其范围至少覆盖B真正读取的全部字节并满足cacheline边界/并发条件 → 发布B descriptor → B读 → 回到B完成步骤。off/len可变，但必须仍在原PAGE_SIZE映射内；不改direction。
- **最终撤销：** 等待所有B操作结束、阻止新使用 → `dma_unmap_page(B,b_dma,PAGE_SIZE,TO_DEVICE)`；A pool自己的mapping寿命另行结束。不能以“B_CQ来了”替代unmap/sync或页生命周期检查。

该模板把所有权与可见性交接分开写清楚；如果root采用更弱的range所有权状态机，需明确初始整页handoff后哪些范围已收回、CPU/XDP触达范围、A实际写范围及缓存线重叠，不能只把payload len代入所有步骤。模板没有证明DMA API在任意双设备平台自动提供device-to-device共享语义；它将设备写与CPU交接串行化，具体backend/bounce仍需backend席原文核验。

## B完成位置所需数据与原两逻辑项的影响

保守完整mapping的B for_cpu需要四类值：`sq->pdev`（既有队列上下文）、B原整页mapping DMA base（运行时值）、`PAGE_SIZE`（冻结常量）和`DMA_TO_DEVICE`（冻结常量）。它不需要frame generation/binding token，也不需要重走V-C lookup；前提是completion payload保留这个DMA base。

原拟只传mode+frame两逻辑项，一般不能从CPU页/frame指针无lookup求出B的任意IOMMU IOVA。不能把CPU PFN当B DMA地址。**允许保留stock第三个dma payload后，可继续无lookup完成保守交接：拟议producer将第三值定义为B整页map base；CQ先pop mode、frame指针值和该base，再`dma_sync_single_for_cpu(sq->pdev,base,PAGE_SIZE,TO_DEVICE)`，随后才解引用frame/header并return。** B提交descriptor里的地址仍是base+off_p；第三payload的意义在此设计中改为base，而非stock per-packet map返回值。producer/consumer须一并一致修改；producer还须满足首轮模板的最终CPU交接条件，第三个base值只解决CQ无需lookup且无需提前读frame的问题，不单独证明现xmit可在整页map之后原样调用。根目标采用这三个逻辑值的保守方案，不能把原mode+frame两项作为普遍收益；这里未写代码、未改ABI、未编译，实际联合体/queue字节量null。保留第三逻辑值属于同步所需operation数据，不等于额外binding引用。

若坚持第三值仍存packet DMA，可数学上以`base=packet_dma-off_p`复原；但从`xdpf->data`读取off_p之前必须已经有frame header的CPU所有权。首次B整页map包括header，不能假定CQ可先读header才完成整页for_cpu。可另行预先收回header范围/保存offset，代价需明算；本轮采用第三值直接保留base即可避免此依赖。源代码stock FRAME路径`en/xdp.c:678–684`先获取frame指针/dma payload并读取`xdpf->len`来unmap，其原map范围只是frame data/len（881–882），不能直接套用到新增整页mapping。

## 谁可以skip以及未决项

- 第一次B map若发生在本轮最后修改之后、默认map同步已覆盖B读范围，提交前重复B for_device可省；映射较早、数据后来变化则不能用旧map当新数据同步。
- API wrapper或`dma_need_sync(B,b_dma)==false`提供该映射同步无须后端操作的依据；该信息不能从A推B，也不能从旧地址/旧配置继承。`dma_dev_need_sync`是设备级粗判，`dma_need_sync`还可能按地址细判。是否手工移除调用需保持DMA-API-debug等语义，通常保留API调用即可得到正确no-op优化。
- B TO_DEVICE的for_cpu是所有权模板的一部分；是否其实现为无cache/copy工作，由backend路径确定。不是每个B_CQ都带一个非零cache操作成本。若完全删掉该API调用，需要明确支撑被省略交接的配置合同。
- `DMA_ATTR_SKIP_CPU_SYNC`不是“任何多设备映射都能省”的旗标。本轮只读头文件注释，不扩展抓取dma-attributes文档；不对它在所有平台/device-domain间的用法作更强普遍断言。新包若由A重新写入，B后端是否须更新私有bounce副本，以及initial full-page map与后续packet range copy尺寸，由backend席裁决。
- 本席不评价backend实际CPU、带宽、bounce物理保留或native部署。API证明不能给出这些实测数。普通O0/持久/O3/复制方案同样享有合法map/sync/no-op优化；正确调用序列本身不是候选独占创新。

## 检查与交接

两个官方同commit文档均HTTP200，完整raw保存并附URL、bytes、SHA256收据；2次web official raw URL open仅用于必要官方浏览，未搜索论文/其他版本。复用的9份源码不重新下载，source-manifest逐个计算当前hash并对照既有receipt（helpers/types是否实际作为主证据见记录）。仅provenance与定向源码核查，无native/实验/性能测试。root建议优先直读：dma-api.rst 300–319/449–465，howto 627–665，dma-mapping.h 295–312/398–409，mapping.c 362–394，en_rx.c 1662–1706，page_pool.c 439–480/740–767，en/xdp.c 881–885/683–700。
