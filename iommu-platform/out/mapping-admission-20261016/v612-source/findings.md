# 官方 Linux v6.12：mlx5e RX/FIB→跨设备 REDIRECT 或本地 UDP

结论：官方 v6.12 内这两条普通链可以在明确配置条件下连通。A 的 RX pool 映射可以跨包保留；跨 B 的 native REDIRECT 仍在 B `ndo_xdp_xmit` 逐帧执行额外 `DMA_TO_DEVICE` map，正常 TX CQE 后卸该映射并归还源 frame。另一分支 ingress PASS 构造原 RX 存储上的 recycle skb；本地 UDP 普通 recvmsg 将 payload 复制到用户 buffer 后消费自己的 skb 引用。**E5 与 E6 是不同包/不同动作的分支，不能串成同一包先后经过。** 本报告不证明部署、流量分布、DMA_PMD 适配或收益，不提出整页 seal。

## 固定版本与方法

- 直接向官方 [Linus kernel.org Git](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git) 执行只读 `ls-remote`：`refs/tags/v6.12` 的 annotated tag 对象为 `06090c9b622a7e1f797e775db4c035e0d779b76e`，解引用 commit 为 **`adc218676eef25575469234709c2d87185ca223a`**。[官方维护者 GitHub 镜像 v6.12](https://github.com/torvalds/linux/releases/tag/v6.12) 对应同 commit；`raw/Makefile:1–6` 为 6.12.0、无 extra version。
- 所有代码均按这个完整 commit 从 `raw.githubusercontent.com/torvalds/linux/<commit>/<path>` 定向获取，保留原目录结构和原始字节，未 checkout、fetch 或改动生产 Git 树。每文件 HTTP 状态、URL、字节数、SHA256、行数、核读范围见 `source-manifest.json`。下载不等于全文精读；未用文件明确标注。
- 本文 `路径:行` 都相对本 run 的 `v612-source/raw/`，**不指生产树同名路径**。关键源码原文入口：[FIB sample](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/samples/bpf/xdp_fwd_kern.c#L43)、[B TX/map/CQ](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/drivers/net/ethernet/mellanox/mlx5/core/en/xdp.c#L847)、[UDP recv](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/net/ipv4/udp.c#L1816)、[pool map/return](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/net/core/page_pool.c#L461)。
- 按 ARS `deep-research/fact-check` 的 source-verification 纪律做定向源码核验；不运行完整研究 pipeline、论文评分或外部模型。实现源码是本合同的一级直接证据，不把它伪装成运行测量、实验论文或 RCT 等级。
- 请求/建议 Sol/high；实际独立模型、推理强度元数据 unknown，不声称已原位切换。Codex-only；0 DeepSeek/Claude调用，0新子席。

## 冻结条件与不能默认为真的部分

普通 NIC mlx5e profile、A 为 cyclic linear single-buffer RX、非 AF_XDP、XDP native、非 generic skb mode；一个包连同必要 head/tailroom 装进一个 CPU page，非 striding/nonlinear/multibuffer，packet-merge/LRO/HW-GRO 关闭。源码接口允许此配置：`en/params.c:296–316` 判 linear，`en_rx.c:2673–2720` 选择 cyclic linear handler；`en_rx.c:75–79` 与 `en_main.c:5885` 串普通 NIC handler。

进一步冻结 IPv4、无 IP options、未分片、校验正确、非 multicast/broadcast 的普通包；A 装 `samples/bpf/xdp_fwd_kern.c:xdp_fwd_prog`，不是 `xdp_fwd_direct_prog`。转发分支要求 FIB 成功、转发启用、neighbour有效、MTU允许、devmap配置的实际出口为 B，B 是另一个正常 mlx5e netdev且支持/启用 native ndo_xdp_xmit，当前CPU对应 B channel存在；不附带 devmap egress BPF程序。软件/设备配置均未实测。

本地 UDP 分支再要求目的地址属于本机、匹配普通 IPv4 UDP socket、无 UDP encap/UDPLite/UDP-GRO聚合、非MSG_PEEK/MSG_ERRQUEUE、无输入MSG_TRUNC、用户buffer足够且拷贝成功；无额外 skb clone/引用持有者、packet tap/AF_PACKET/raw listener、重定向/tee/窃取数据的netfilter/tc/BPF等消费者。GRO路径冻结为 `GRO_NORMAL` 的普通skb传递，可以正常批量上送，但不混成聚合数据包。源码没有替我们验证这些排除条件；若不满足，仍能说本次 recvmsg 放掉自己的引用，不能保证它就是最后一份。

## E0–E7：按用户标签逐项核验

| 事件 | 这一步真正完成什么 | 直接源码位置与边界 |
|---|---|---|
| E0 RX pool建立/取页 | A pool的order0、DMA_MAP/SYNC、XDP BIDIR、device/NUMA/NAPI已定；给RX取页 | `en_main.c:838–844,924–950`；`en_rx.c:274–292`。v6.12只设置DMA_MAP与DMA_SYNC_DEV，**没有旧6.5的PP_FLAG_PAGE_FRAG**。 |
| E1 A慢分配map并保存 | 新页以A device映射，DMAaddr保存；缓存复用不重建此图，直至离pool | `net/core/page_pool.c:461–489,523–571`。`:468`明确kept for lifetime until leaving pool。RX WQE使用保存地址见`en_rx.c:343–363`。 |
| E2 A RX CQE/CPU sync/XDP/FIB | 已收包的CPU sync；根据包头与当前FIB/neighbor/map选择动作/出口 | `en_rx.c:1816–1845,1665–1699`；`en/xdp.c:318–345`；sample `:43–143`。路由与目标接受不是同一个门。 |
| E3 devmap bulk/flush/ndo | 将原存储的xdp_frame enqueue，满批可发，尾flush敲提交；ndo返回接受前缀数 | `include/net/xdp.h:291–308`；`net/core/filter.c:4377–4458`；`kernel/bpf/devmap.c:376–432,456–500`。未接受后缀由devmap return。 |
| E4 B map及接受TX | B自己的DMA device上逐帧TO_DEVICE map；SQ接受才计入前缀，FIFO保存frame与B地址 | `en/xdp.c:847–950`。map本身先于SQ接受；失败/拒绝清理与被接受帧责任不同。 |
| E5 B TX CQ回收 | 正常CQE指示完成的WQE回收；B unmap→xdp_return_frame_bulk→flush return→源pool | `en_txrx.c:168`；`en/xdp.c:748–803,673–700`；`net/core/xdp.c:455–508`。不是线缆对端已收包证明。 |
| E6 ingress PASS/本地UDP消费 | 原RX存储建recycle skb；本地协议栈/UDP排队；nonPEEK dequeue、payload copy后consume自己的skb | `en_rx.c:1635–1652,1701–1708,1853`；`udp.c:1715–1765,1816–1909,1616–1629`。最后用户还须穿过skb/dataref/pool ref门，详下。 |
| E7 离pool与物理释放 | 退出pool时A unmap、clear pool info、put_page；最后普通page引用结束才允许物理回分配器 | `page_pool.c:648–698,736–790,818–829,857–902`。正常归pool可只sync并保留A map。pool对象自身还等user/inflight门`:1049–1058,1115–1134`。 |

说明：E0/E1 可先批量映射多页，然后尚未收包；E3满bulk时可立即调用B，故不能强制把整个NAPI里的所有E2排在所有E4前。E5/E6的consumer return与A RX侧bias drain的先后也不固定。这里是因果/所有权边界，不是全系统串行时间表。

## E2：实际出口什么时候已知

分配时已知A、pool、NUMA、page大小、XDP程序存在；可知道程序/map/FIB的可能目的集合，或根据固定部署知道某些流必走B。但驱动的分配接口本身不携带这个尚未到达包的最终action/出口；不应宣称出口在所有部署中必然不可预知，也不能把未来再用次数当在线输入。

sample `xdp_fwd_prog:146–149` 调 `xdp_fwd_flags(...,0)`。IPv4 sample `:61–78` 从包头填FIB参数，`:102–104` 用ingress ifindex调用helper。helper `net/core/filter.c:5894–5976` 用当前网络命名空间/路由规则 lookup并选path；`:5992–5995` 将**实际egress netdev ifindex**写回；`:6016–6025` 要求有效neighbour并填MAC。sample `:121–140` 再验证devmap含此出口、减TTL/改MAC、redirect_map。

本机目的：helper `:5972–5973` 对`res.type != RTN_UNICAST`返回NOT_FWDED，涵盖RTN_LOCAL；sample `:143`返回PASS。FIB之前检查forwarding，若关闭也可PASS（`:5910–5913`）；因此PASS不是“必定本机消费”的证书，也可能是TTL、无neighbour、未配置出口等回退，随后普通栈仍可转发。本文E6只选真正本机目的子集。

redirect helper `include/linux/filter.h:1705–1740` 对devmap作第二次lookup并保存目标；条目变化或lookup失败可影响最终动作（低flags为0时失败返回XDP_ABORTED），不能把sample先前`bpf_map_lookup_elem`当永久接受证书。E3知道传给哪个B ndo；E4又检查B设备状态、CPU/channel、map与SQ。进入B ndo时已知本次frame、len、B DMA device和SQ上下文；这不等于提前知道最终accept。`en/xdp.c:881–885`先map；`:909–910`再传`check_result=0`调用xmit，regular路径`:569–585`才调用`:511–520`的SQ stop-room check，MPWQE路径`:488–491`才调用`:428–443`的空间check。故stock路径可能已经付map费用才因队满拒绝，再卸map；不能给policy一个免费的“最终可接受”oracle。成功接受后才有有效映射/descriptor责任转移；未来再用次数或最后引用时间仍未知。

## E3–E5：跨设备路径与错误所有权

page_pool的buff→frame转换是在原headroom写frame并复制mem-info (`include/net/xdp.h:291–308`)，无整包复制。devmap `:488–497` 只验目标xmit能力、SG能力与转发长度，不代表B已map/接受。原链无map失败后自动copy重试保证。

B `en_main.c:5160` 注册ndo，`en/xdp.c:856–868` 在map前可返回ENETDOWN/EINVAL/ENXIO；`:877–885` 对原frame做`dma_map_single(sq->pdev,...,DMA_TO_DEVICE)`，`:909–922` SQ拒绝时把成功的单buffer映射卸掉；`:925–940` 成功才把frame/地址放FIFO并nxmit++。`en/xdp.c:595–613` 最多复制必要inline header，剩余data segment仍用DMA地址；不能把硬件inline当O1整包复制。单buffer冻结路径不扩展为可选SG错误路径已验证。

`kernel/bpf/devmap.c:399–412` 按返回数解释：负数表示一个都没接受；非负k表示接受前缀 `[0,k)`，devmap归还 `[k,n)`。入队前错误由A保持原RX回收责任（`en/xdp.c:327–345; en_rx.c:1838–1842,331–335,374–380`）；入队成功后的B拒绝由devmap return；已接受帧由B完成回收。不要让两方重复归还，也不要把拒绝后缀混成成功前缀。

E3 flush是提交：`en/xdp.c:943–950` 完成MPWQE，FLUSH时doorbell；`en/xdp.h:158–164` 是notify_hw。source poll尾调用 `en_rx.c:2530–2531`→`en/xdp.c:953–965`→`xdp_do_flush`；`net/core/filter.c:4256–4274` 的“completing redirect”指NAPI/RCU下bulk提交，**不是B DMA完成、IOTLB同步或最后引用**。v6.12进入发送循环后的out路径会完成MPWQE，doorbell仍取决FLUSH；map前的设备/flags/channel错误可直接返回，不把这些早退冒充提交。

正常E5必须取得B CQE：`en/xdp.c:764–790` 按wqe_counter回收，`:683–700` B unmap后return_frame_bulk，`:803` flush。错误CQE也有回收/告警，`:793–799`；关闭清理`:819–845`是另一个依赖停机合同的路径，不冒充正常成功CQE。完成只说明驱动可以回收本次DMA读buffer，不等于对端收到，也不等于A map消失。

## XDP_TX必须分开计费

`en/xdp.c:322–326` 的XDP_TX走A自己的`mlx5e_xmit_xdp_buff`；page_pool分支`:119–126` 直接取A已有DMAaddr并sync-for-device，利用BIDIRECTIONAL，不额外做跨B的map。它记录PAGE模式，完成时`:703–720`直接pool recycle。它仍有同步、descriptor/doorbell、硬件TX、completion/ref门，并非零成本。**跨设备REDIRECT走B ndo的FRAME模式，不能拿XDP_TX的“复用A映射”替它免掉B mapping成本。** sample `xdp_fwd_prog`本身不返回XDP_TX。

## E6：本地UDP的完整正常调用链与最后引用

1. ingress PASS使`en/xdp.c:320–321`返回false。`en_rx.c:1635–1652`用`napi_build_skb(va,frag_size)`包装原RX数据；`:1706–1708`标记recycle并记consumer；`net/core/skbuff.c:533–568`确定新skb包装同data、head_frag=1。此处没有整包复制。
2. `en_rx.c:1853`进`napi_gro_receive`。冻结GRO_NORMAL：`gro.c:475–476,565–568,599–638`→`include/net/gro.h:511–528`普通batch上送→`dev.c:5874–5904,5702–5718,5722–5812`的list接收与IPv4 packet handler。`af_inet.c:1896–1900`注册ip_rcv/ip_list_rcv。普通batch不是多包合并。
3. IPv4 list入口 `ip_input.c:642–672`→`:633–638`→`:594–630`调用`ip_rcv_finish_core`，`:364–368`作输入路由，`:575–580`走dst_input。单skb入口`:560–571,435–450`同样走路由和dst_input。RTN_LOCAL在`route.c:2301–2306,2334–2351`得到RTCF_LOCAL，`:1628–1629`把dst.input设ip_local_deliver。`ip_input.c:242–256`（冻结未分片）→`:227–234`→`:187–206`协议分发。这里可出现raw listener额外消费，正是需要排除而不能假装源码默认无clone的原因。
4. `af_inet.c:1945–1950`注册udp_rcv；`udp.c:2630–2633`→`__udp4_lib_rcv:2387–2449`找socket→`udp_unicast_rcv_skb:2365–2380`→`udp_queue_rcv_skb:2207–2213`（冻结无GSO）→`udp_queue_rcv_one_skb:2096–2195`（冻结非encap/filter改写路径）→`__udp_queue_rcv_skb:2052–2064`→`__udp_enqueue_schedule_skb:1514–1574`排入receive_queue。排队保留原RX数据；用户尚未读取时不能回pool。enqueue也可能内存压力drop/condense，非本冻结成功链的收益证据。
5. `udp_recvmsg:1831–1834`取skb；`__skb_recv_udp:1735–1765`调用`__skb_try_recv_from_queue`，`net/core/datagram.c:183–199`中nonPEEK从队列unlink，PEEK则额外加skb users。`include/net/sock.h:677–684`确认nonPEEK off=0。足够len令`udp.c:1836–1841`复制完整payload；`:1857–1866`走copy_linear_skb或copy/checksum函数。`include/net/udp.h:381–385`明确copy_to_iter_full；`datagram.c:521–540,833–865`也是拷贝而非把RX页借给用户。拷贝失败或checksum错误分别走错误归还/retry，不能把它们算成功完整消费。
6. 正常拷贝及控制消息处理后`udp.c:1908`调用`skb_consume_udp`。`:1621–1629`先要求`skb_unref`为最后skb users；否则仅放自己的引用。`include/linux/skbuff.h:1221–1253`分别定义skb users与clone共享dataref两个门。然后`skbuff.c:1448–1452`→`skb_release_data:1102–1125`判dataref→`skb_free_head:1089–1098`→`skb_pp_recycle:1043–1047`→`napi_pp_put_page:1022–1038`→`page_pool_put_full_netmem`。另有clone时consumer syscall返回也不等于最后data释放。
7. pool last-user是第三个门：`include/net/page_pool/helpers.h:227–308,311–345`用`pp_ref_count`记pool API的引用，最后一次unref才调用put_unrefed。注意v6.12逻辑最后用户后可把计数重置1，不能坚持要求字段物理值变0。A的RX bias由`en_rx.c:274–302`设置/退回未用份数；consumer return与RX drain两者都到位才可recycle，先后不固定。第四个门是普通page refcount与recycle容量，见E7。

因此，在上述无额外持有者、复制成功且RX bias已drain的条件下，本地UDP consume能成为原RX page最后pool消费者并触发归pool。若bias较晚drain，syscall可先返回而最终归pool稍后发生。**用户收到的是自己的payload副本，后续用户计算不继续持有原RX page**；应用计算完成时间不能代替RX页最后引用时间。

## E7：归pool、离pool、物理free与pool对象free

`page_pool.c:736–773`在pool last-ref后看普通`page_ref_count==1`与非pfmemalloc，合格就sync-for-device并回cache/ring，继续保留A映射；`:776–790`普通引用升高时退出pool，`:818–829,857–902`缓存/ring满时也可退出。退出走`:648–698`：A dma_unmap、清DMAaddr、清pool metadata、put_page。其他普通ref若尚在，put_page不是立即物理释放。物理free的精确分配器内部指令未向mm扩扫；这里严格只证明归还普通page引用的调用，不以put_page这个名字冒充“已物理free”。pool对象另由`:1049–1058,1115–1134`等用户数/inflight，和单包最后引用不同。

## 路由后的mapping admission/copy：真实钩子与现有代码的缺项

这是新候选的**有界集成推断**，不是v6.12已经存在的功能，也不是实现方案已经验证。FIB路由/BPF动作可以复用现成sample，不能让BPF当前的redirect helper自动拥有内核pool分配、持久DMA map或新frame所有权协议。

| 可定位边界 | 当时已有信息 | 能做/还缺什么 |
|---|---|---|
| E2 helper/sample路由成功后 | packet len、FIB egress ifindex、当前包内容/可付费历史 | 适合判别“这包将去B”，但此处B的DMA device/domain和成功mapping/accept尚未建立；原sample无source-region→domain admission API。 |
| E3 `devmap.c:393–399`最终调用目标ndo前 | 最终B netdev、这一bulk的frames及可选egress程序执行后的结果 | 可作为公共policy插入边界，但要提供源区ID、DMA device/domain对应、分配/映射和错误归属；不能只修改BPF返回值完成持久pool机制。 |
| E4 **B `en/xdp.c:877–881`，在dma_map_single前** | 具体frame数据/len/mem-info、B `sq->pdev`、SQ上下文；可以付费识别原page/pool/源区历史 | 是最直接的驱动内选择短map/保留源区map/复制的落点。当前代码无条件逐帧map；source-region表、跨包B映射缓存、目标紧凑TX pool及其retire钩子均不是现成该链功能。 |
| E5 `:673–700`完成回收 | 已接受frame、完成CQE、保存的B地址/mode | 当前FRAME模式每次unmap；持久源区map或目标pool需要让descriptor元数据区分“短map待卸”与“持久映射引用释放”，并按正确pool归还。只删unmap会使E7和撤销/退役失配。 |

T合同若明确准许专用范围对A/B以规定权限持续映射至退役，可以保留B map并保持正常buffer引用；但必须覆盖span内headroom/frame元数据/tailroom/空洞的授权、在途DMA与trust变化，以及pool退役时B alias回收。源码E7只清A pool映射，不会替一个新加的B cache自动卸图。持久B map不能保留到原page已回通用分配器后仍可访问；追加ref/pin若改变page_refcount，又会触发当前pool的非recycle路径，须计入集成与保留成本。这里不要求逐包撤A映射，也不设计整页seal。

O1 copy的对象/错误归属必须清楚：新目标池buffer是**第二份实际物理对象**，需要自己正确的mem allocator/type/id、DMAaddr、head/tail布局和completion return；不能memcpy源frame后保留源mem.id让copy在完成时误归A pool。源frame在CPU读拷完成且不再有使用者时可以归A，但它仍需遵守RX bias/pool引用门。当前驱动没有这套目标TX pool和持久模式。

若选择E4内部copy、保持现有“接受前缀/未接受后缀”接口，则接受前尚未发生的错误必须保留原frame给devmap归还，并回收私下分配的copy/短map；当成功接受一个输入frame责任后，驱动可以把其原frame归源，并将copy持到B CQ后归目标pool。这是责任不变式，不是假定现有ndo API允许任意替换调用方数组。若改在devmap层替换frame，必须明确交接新对象、原对象只归还一次、失败尾部到底持有什么；不能同时用旧指针让devmap再返原对象。还要保持合法前缀返回或显式修改API，不能跳过一个失败输入后继续接受任意后项而仍返回简单count。

O1早归源pool不自动减少总内存：目标池copy仍活到B完成，源池也可能保持其容量；初始化/保留目标页、CPU/内存带宽复制、同步、descriptor元数据、退役全部收费。O2给同物理源区添加B alias不是第二份payload物理内存，但有IOVA/PTE/表元数据/映射保留和退出成本。原池+短map O0、目标copy O1、按domain首次建图保留的O2与同信息普通累计费用选择O3，都可以在上述合法钩子拿到相同当时信息；“路由后才按需map”本身不超出O2，也不能把真实未来`r_g`给候选而不给普通规则。没有真实费用/源区复用轨迹，源码链不裁决这些规则谁胜，也不裁决论文新意。O3的短map分支在未来加入DMA_PMD后，还须有合法bypass进入普通map/unmap；不能由任意size参数自行假设能绕过大页fastpath。该RFC接口由root另核，本文不从stock v6.12虚构这项能力。

## DMA/domain/PMD/strict与当前证据边界

`en_main.c:741,1459,2691`将rq/sq DMA device指向`mlx5_core_dma_dev(mdev)`；两个netdev不保证两个DMA device/IOMMU group/domain。`kernel/dma/mapping.c:155–179`可走direct、iommu_dma或其他ops，故driver dma_map不自动证明翻译路径。若实际使用翻译DMA且A/B不同domain、同共享IOMMU服务，`dma-iommu.c:742–759`给A BIDIR读写/B TO_DEVICE读；`:848–876,1163–1173`按各device domain建图并按granule对齐。实际page size、IOMMU granule、SID/domain、TLB容量/共享/替换与invalidate代价均unknown，**没有导入本平台16项IOTLB参数**。

B `dma_map_single`在`include/linux/dma-mapping.h:380–395`下沉page+offset；`dma-iommu.c:1175–1219`在条件SWIOTLB可bounce。因此原frame转换同存储得到源码支持，但实际A/B都对同原物理页DMA还需no-bounce条件。非coherent下`:1214–1215,1233–1234`同步也不能漏账。A/B若共domain或有其他alias，卸B本次IOVA不保证撤该domain对物理页的一切访问。

strict是实际unmap的IOTLB同步选择：`dma-iommu.c:825–845`非queued时同步，queued时延后；不强迫上层每包调用unmap。Linux RX pool常驻映射是普通合法合同；用户T允许持久range至退役。S逐对象隔离/物理授权结束只是另一个需求分支，目前没有实际mlx5e部署需求证据，不能拿strict等同S或宣布page_pool违约。

`en/params.c:278–281,693–707`仍明确XDP不支持一page多个包，`en_main.c:927`仍order0。所以可支持“同一源页跨不同包生命周期被复用、逐包FIB结果可能不同”的来源；不同packet的CPU/B用途并不在同一物理page上同时并存。源码也没保证多个order0页构成物理连续/PMD对齐的专用span；不能把逻辑pool或MPWQE直接当PMD。源区组织、DMA_PMD物理适配和按domain首次图建立由后继假设/实现另行给出，当前不做实现、不跑模型。

## 交付验收与未解项

本报告已在同一官方commit内核 E0–E7、XDP_TX区别、FIB本地/转发分支、UDP最后引用门以及路由后具体集成边界；raw文件可离线定位，HTTP/hash/范围可重复机械核验。没有实际网络设备、驱动加载、BPF配置、用户UDP程序、DMA轨迹、物理页布局、性能测量或新系统实现。因此“source链完整且有条件”不等于“本机native可运行/条件已成立”。

尚需外部/后继输入：部署实际T或S与设备信任/授权范围；A/B DMA topology/domain/translation/bounce/coherency；RX分配和路由下真实或明确冻结的源区→目标再用关系；完整短map/首建图/保留/复制/同步/退出费用；PMD span取得/保留及适配；目标pool初始化/退役与错误/最后引用接口。没有这些，不给mapping admission/copy新意或性能正结论。完整日志/来源都留本run，未写canonical状态/计划文档。
