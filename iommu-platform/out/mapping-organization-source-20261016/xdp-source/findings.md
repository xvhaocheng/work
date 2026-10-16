# 正常 XDP 跨设备 buffer 生命周期：固定本地 Linux 源级切片

结论：本地同一 Linux 快照具有真实硬件驱动链 `mlx5e A RX page_pool → ingress XDP_REDIRECT → devmap bulk → mlx5e B ndo_xdp_xmit → B TX CQE → B dma_unmap → xdp_return_frame_bulk → A page_pool`。这是源码可达性与合同核对，未证明本机有两张 mlx5 网卡、已配置 XDP/IOMMU、实际运行这条路径，也不是 DMA_PMD 移植或性能结果。最重要的反向边界：这一版本 mlx5e 明确不让 XDP 的多个包共用一个物理 page，不能拿它证明同 4KiB 页同时混有 CPU-only PASS 包和 B-TX 包。

## 版本和选定合同

- 源码根 `/home/xvhaocheng/work/iommu-platform/linux`；Git HEAD `c0f5e13ea81fa30d78f3bb1d6389e2397321e040`，Makefile 为 6.5.0-rc2，但该 HEAD 含本地研究提交，不能称纯官方 v6.5-rc2。平台 HEAD `0a570deda89636631462bb188d39c058409df364`，`sources.lock.json:125–128` 的 Linux commit 与实际 HEAD 相同。
- 实际工作树有已存在修改；完整状态与每个引用文件 hash 见 `source-manifest.json`。本切片所引用文件在开始、结束核对时均与 HEAD 对应内容相同。本席没有修改 Linux、驱动、配置、生产文档或其他席位产物。
- 本文源路径均相对上述 Linux 根，行号针对工作树实际文件；manifest 给出绝对路径、SHA256、HEAD blob 和行范围。只用此一快照，不从其他树补链。
- 冻结为 native XDP、非 AF_XDP、非 broadcast、cyclic/linear RX、单 page 包、包大小满足 MTU/头尾空间、packet merge 关闭、B 支持并启用 ndo_xdp_xmit、CPU 对应 B channel 存在。A/B 指两个 mlx5e netdev；若要独立 DMA 授权，再要求其实际 DMA device/domain 隔离。以上配置为条件，不是本机观测。
- 建议/请求配置 Sol/high；实际独立模型与推理强度的运行元数据 unknown。共享 IOMMU Codex-only；未用 DeepSeek/Claude，未启动子席。

## 1. 分配时和路由确定时知道什么

A 在建 RX queue 时已知源 DMA device、NUMA、页大小/order0、池大小、XDP 程序存在和方向；`en_main.c:747–753,833–858` 设置 `DMA_BIDIRECTIONAL`（无 XDP 则 `DMA_FROM_DEVICE`）、`PP_FLAG_DMA_MAP | PP_FLAG_DMA_SYNC_DEV | PP_FLAG_PAGE_FRAG`、`order=0, max_len=PAGE_SIZE, dev=rq->pdev`，并注册 MEM_TYPE_PAGE_POOL。它还不知道某个尚未收到的包最终 PASS、DROP、TX、REDIRECT 到 B 或其他目标。配置的 map 可以已包含 B，但“可能目的集合已知”不等于“每个分配对象的实际用途已知”。这不是目的必然不可预知的证明：若程序固定全重定向到 B、map 不变，部署方可以预先知道目的。当前驱动分配接口本身不携带每包最终action/目标，本文也没有运行含PASS/B混合的classifier或证明其动作分布。

`en_rx.c:276–292` 从 pool 取页并设置 fragment bias；`:343–363` 取得 pool DMA 地址并写 A 的 RX WQE。慢分配 `net/core/page_pool.c:397–450` 可批量取页并预映射，因此映射甚至可先于单个包到达。

RX CQE 后 `en_rx.c:1653–1688` 对收到范围 sync-for-CPU，取 `rq->xdp_prog` 并执行 `mlx5e_xdp_handle`。`en/xdp.c:271–288` 此时才判 action。BPF helper `include/linux/filter.h:1506–1542` 在执行时 lookup map，保存目标/索引/类型，返回 REDIRECT；`net/core/filter.c:4282–4357` 把这个 frame 放进相应 devmap。到此实际 B 已知，但 B 的 DMA map、SQ 接受和硬件完成仍未发生。

本树 sample `samples/bpf/xdp_redirect_map.bpf.c:38–68` 提供实际 devmap program：检查包、可能 PASS、改 MAC，调用 bpf_redirect_map；`:71–87` 的 devmap egress program 也会读/改包并返回 PASS。后者 PASS 的意思是“允许继续发往既选 B”，与 ingress PASS 上送协议栈不同，不能混淆。

## 2. ingress PASS 与 REDIRECT 是两个分支

- ingress PASS：`en/xdp.c:273–274` 返回 false，继续 `en_rx.c:1685–1698` 的 `mlx5e_build_linear_skb`。`:1623–1640` 用 `napi_build_skb(va,...)` 在原存储上建立 skb，没有整包复制；随后标记 recycle、增加该页 fragment 使用数，`:1841` 进入 GRO/协议栈。它不是 PASS 后自动进入 devmap。这里“CPU用途”只表示该分支的协议栈消费，不能承诺以后不会经普通网络转发再交设备。
- REDIRECT：`en/xdp.c:280–288` 成功后设置 XDP_XMIT/REDIRECT 标志；`en_rx.c:1826–1830` 在不生成 skb 时增加该页 frags，保留异步所有权。若 redirect 在入队前报错，转 drop/abort，不设置成功所有权标志，后续 RX WQE 回收处理该页。
- 对 PASS skb，最终 `net/core/skbuff.c:910–919` 的共享 dataref 门决定能否真正释放 data；head 的 recycle 路径为 `:882–886,897–904` → `page_pool_return_skb_page`（`net/core/page_pool.c:919–954`）→ 同一个 pool fragment/引用门。CPU 软件消费者最后放手与 B 硬件 TX 完成是不同分支上的不同事件。

## 3. 跨 B 发送：同一 frame、额外 DMA 映射、异步完成

`include/net/xdp.h:292–309` 对 page_pool 在原 buffer 头部存 xdp_frame，复制 mem-info，返回原内存；本合同没有整包复制。`kernel/bpf/devmap.c:468–486` 检查目标能力和可转发性后 enqueue；`:443–465` 的 per-CPU bulk 满了可先 `bq_xmit_all(...,0)`，poll 尾 `__dev_flush:409–419` 以 `XDP_XMIT_FLUSH` 发。`:385–398` 调目标 ndo，并由 devmap 负责归还未接受帧。

B 的 ndo 在 `en_main.c:4923` 注册为 `mlx5e_xdp_xmit`。`en/xdp.c:779–817` 查设备状态、flags、当前 CPU 与 channel，把 frame->data/len 对 `sq->pdev` 另做 `dma_map_single(... DMA_TO_DEVICE)`；成功接受后 `:857–872` 记录 frame 和 B DMA 地址到 FIFO。普通 WQE `:545–583` 可以复制少量 inline header，但剩余 data segment 使用 DMA 地址，不是整包 CPU 复制发送。MPWQE 也是 WQE 组织，不是大页或 PMD 组织。

软件 flush 仅在 `en/xdp.c:875–880` 完成 MPWQE/敲 doorbell，`en/xdp.h:151–156` 是 notify_hw。`net/core/filter.c:4175–4193` 所说 completing redirect 是 bulk 提交/RCU 边界，不是 B 读完 buffer、更不是撤销或最后引用证书。

正常完成路径由 `en_txrx.c:168` poll B XDPSQ CQ，`en/xdp.c:696–723` 必须取到 CQE，依据其 wqe_counter 回收已完成 WQE；`:629–646` 先对 B 的 data/fragment 调 `dma_unmap_single(... DMA_TO_DEVICE)`，再 `xdp_return_frame_bulk`；`:735` flush return bulk。这是驱动依赖硬件 CQE 的正常完成门，未测量硬件轨迹或宣称线缆对端已收到包。CQ 中出现错误 opcode也会回收并告警（`:725–732`）；`free_xdpsq_descs:751–776` 的关闭清理是另一条需依赖队列停机合同的路径，不能当正常成功 TX CQE 证据。

## 4. A/B DMA 授权与 strict 的范围

`en_main.c:650,1337,2459` 串起 rq/sq->pdev = channel->pdev = mlx5_core_dma_dev(mdev)。因此两个 netdev 名称、两个端口不证明两个 DMA device 或两个 IOMMU domain。实际拓扑、group、domain、dma_ops、passthrough、SWIOTLB、coherency、本机配置均未核。

在条件“实际使用本树 iommu_dma_ops，且没有 bounce”下：`include/linux/dma-mapping.h:328–337` 把 B dma_map_single 变成 page+offset；`drivers/iommu/dma-iommu.c:638–655` 规定 A 的 BIDIRECTIONAL 为 IOMMU_READ|WRITE，B 的 TO_DEVICE 为 IOMMU_READ；`:730–753,1015–1025` 以相应 dev 的 `iommu_get_dma_domain` 分配 IOVA/map。权限按 IOVA/domain 定义，不能说 map 自动逐对象字节隔离；`:736–749` 对 granule 向外对齐。实际 page size/最小 granule未知，不默认写成 4KiB。

`kernel/dma/mapping.c:147–167` 也允许 direct DMA，不能只因驱动调用 dma_map 就断言有 IOMMU。如果 A/B 真属隔离 domain，B 的本次 unmap 不删除 A pool 的常驻映射；如果 A/B 共 domain，B 即便删去自己这次 IOVA，A 常驻映射等 alias 仍可能授予相同 domain 访问该页，不能等同撤掉“B 对这个物理对象的一切权能”。

源 pool 映射保持到页离开 pool：`net/core/page_pool.c:335–343` 明说 lifetime，`:495–520` 才真正 dma_unmap 并清 pool info。正常 recycle `:591–603` 只 sync-for-device 和回缓存/ring，不 unmap；故 B 完成归还后 A 映射可继续用于下一包。无 XDP 的 FROM_DEVICE 只写权限与本切片 XDP BIDIR不可混用。

必须把两种合同分开：Linux 现有 pool 允许上述常驻映射和受控复用；应用若额外要求“每包完成即撤销设备对该对象的全部访问”，这是更强的逐对象撤销合同，需要明确撤哪些domain/alias、何时撤、等待谁。不能据 A 常驻映射宣布 Linux page_pool 违反 strict。IOMMU 的 strict/unmap 时同步与“何时由上层选择调用 unmap”也是不同问题：`drivers/iommu/dma-iommu.c:707–727` 在非 queued 时于实际 unmap 内同步 IOTLB；queued 时延迟处理。strict 不自动迫使 pool 每包 unmap，且软件 bulk flush、CQE、dma_unmap、IOTLB 同步、最后引用五个门不能互相代替。

## 5. return/fragment/引用/最终释放不是一个门

1. `net/core/xdp.c:472–511` 把完成 frame 的 data/frag 地址收进按 mem.id 组织的 return bulk；`:458–467` 才调用源 `page_pool_put_page_bulk`。此刻 B 本次 DMA mapping已卸，但 bulk 入队还不等于完成 pool return。
2. `net/core/page_pool.c:645–652` 先判 last-frag；`include/net/page_pool.h:286–342` 明确 `pp_frag_count` 统计能更新数据/有 DMA 的 fragment用户，不能只看普通 page refcnt。mlx5 的 bias 初值是 PAGE_SIZE/64；`en_rx.c:295–302` 在 RX 侧退还未使用 bias，异步消费者 return 退还自己一份。两方先后不限；只有最后fragment归还后才可全页 recycle/unmap/sync。`en_rx.c:331–335,374–380` 是 RX WQE 最后页/未 skip 的回收入口。
3. 满足 last-frag 后 `net/core/page_pool.c:591–621` 才看 `page_ref_count==1` 且非 pfmemalloc：满足则 sync并尝试缓存/ring，A map继续；不满足则释放 pool映射/状态并 put_page，普通最后 page ref 仍可能在其他持有者手上。
4. ring 满 `:626–634,677–681` 或 pool销毁 scrub `:764–830` 才走 `page_pool_return_page:525–529`，先 release/unmap，再 put_page。这里最后普通 page引用决定物理页何时回通用分配器，不能把“回pool”等同“释放物理内存”。
5. pool 对象自己的最后 user/inflight 是另一门；`page_pool_destroy:881–900` 可以延期重试，`:823–832` 等 inflight=0才 free pool。B CQE不是 pool对象完成证书。

## 6. 同 page/同 PMD 的来源边界

`en/params.c:266–269` 明确 XDP不支持多个包每page；cyclic linear `:746–760` 的 frag_stride至少 PAGE_SIZE，pool order0 (`en_main.c:836`)。因此选定链有“包的用途在 allocation之后决定”的源码来源，但没有“同一page同时有不同包的CPU/B用途”的来源。单包先被CPU读改再交B是时间顺序共享，不是同时两个独立对象。非XDP时的 page fragment支持不能移植成该XDP合同。

同一order0 page可先后承载PASS包和REDIRECT包（pool复用允许，实际轨迹未测）；不同order0页可能有不同action。源码没有把这些页组成物理连续、PMD对齐、同权限生命周期的span，也没有本切片的 DMA_PMD 调用/适配。已定向检查所引用 mlx5/page_pool/DMA接口文件中的 DMA_PMD/dma_pmd，未出现；这只描述选定路径，不声称整个仓库无该字符串。若未来人为把多页放同PMD映射，CPU/B混用途和不同完成时刻可作为冻结假设的投影；物理邻接、映射粒度、原生可达率和收益都不能写成已获Linux证据。当前没有新大页模型或适配实验。

## 7. copy fallback 与错误归属

- 正常 page_pool→单目标devmap没有转换整包copy；缺 headroom、目标不支持、MTU、DMA map失败、SQ不足均可报错/drop，没有通用“失败就自动复制重试”保证。入队失败由A保持RX回收责任 (`en/xdp.c:282–297; en_rx.c:1826–1830,374–380`)；入队后 B返回负数/接受前缀，未接受帧由devmap归还 (`kernel/bpf/devmap.c:385–398`)。
- B单页 map失败：未获得有效B map，break；单页 map成功但 SQ拒绝：B先unmap (`en/xdp.c:841–854`) 再让devmap归还。已接受frame由B CQ完成卸映射/归还，不再由A重复释放。
- 本树另有显式复制路径，不能偷换为本合同：AF_XDP转换 `net/core/xdp.c:544–579` 分配新order0、memcpy，成功后xsk_buff_free原UMEM，之后新frame独立释放；broadcast克隆 `kernel/bpf/devmap.c:552–564` / `net/core/xdp.c:671–696` 也复制并改成MEM_TYPE_PAGE_ORDER0，非同一物理对象共享。
- DMA层可能bounce：`drivers/iommu/dma-iommu.c:1027–1067` 对特定SWIOTLB/非对齐条件分配bounce；`kernel/dma/swiotlb.c:801–817` 拷入、`:872–883` 按方向回拷/归还。此时 B实际DMA页可能是bounce页，不能无条件声称A/B DMA相同原page；本合同主链的同物理存储命题以no-bounce为条件。失败时驱动/上层仍按dma_mapping_error和所有权前缀处理。
- 有界源码疑点，未修复、未扩大为本单页链结论：可选multi-buffer分支 `en/xdp.c:824–837` 的fragment map失败会卸已成功fragment后goto out；从该函数控制流看未经过`:846–847` 的head unmap，可能遗留head mapping。单页冻结路径不走此分支；不得把本报告当multi-buffer所有失败路径已通过验证，后继若纳入SG需单独核修。这里是源码观察，不是已运行复现或上游缺陷归因。

## 仍未知与最小后继输入

未检查真实网卡、BPF加载、IOMMU配置和运行日志；未运行任何设备/源码程序、新模型、构建、性能实验或网络访问。若后继要把该链变成映射组织收益正例，需要提供：选定两实际DMA device/domain及alias边界；实际PAGE_SIZE/IOMMU granule；明确的逐对象撤销新合同或现有pool合同选择；经验证的连续/对齐物理span分配与DMA_PMD适配；正常动作/完成/引用轨迹；相同权限与复用规则的普通4KiB/常驻pool/复制或bounce对照和全部成本。不能靠本链的晚绑定路由单独推出大页组织收益。
