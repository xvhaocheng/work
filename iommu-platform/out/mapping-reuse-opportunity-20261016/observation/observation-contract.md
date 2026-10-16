# MAP-REUSE-OPPORTUNITY-1：现成事件的观测与配对合同

2026-10-16。请求 Sol/high，实际模型/强度独立元数据 unknown；Codex-only，未用 DeepSeek/Claude。沿用 ARS-Codex 已定有界来源核查，不扩完整 pipeline。本轮只读取源码、继承执行账，补取固定 v6.12 的三个 trace 定义；未开启 tracing、采集、驱动、构建、模型或设备操作。

## 结论

**现成事件具备“完整源 pool 寿命区间中，同物理页反复出现 B DMA map”的部分基础，但不能仅靠它们恢复逐帧接纳、CQ 和最后引用链，也不能测完整 map/unmap CPU。** source 身份、目标设备/config 和无丢事件等前提齐全时，可以先计“重复映射出现”；它不是已完成包的重复使用次数，更不是已证性能收益。

本席检查的资产范围为本任务合同、mapping-admission 的源码/README、mapping-lifetime 与 mapping-integration 的执行账，以及本轮三个 trace 定义。这里没有匹配此工作流的原生逐事件采集；两个前轮账明确原生运行数为 0，admission README 同样记录 0 原生运行。**这是已列资产范围不足，不是全仓不存在记录的结论。** E3/M0、PMD说明数字不作为这里的同工作流轨迹。trace 定义存在不等于内核运行配置支持、事件已开启或记录已采到。

## 固定源码合同与本机只读身份分开

root提供的 `../root/host-readonly-identity.json`（UTC 2026-10-16T02:28:03.749461+00:00）记录本机运行 `6.8.0-60-generic`、x86_64；已见物理netdev驱动为 e1000e/cdc_ncm，mlx5物理netdev计数为0。两处tracefs的available_events读取均EACCES(errno13)。这证明本机已读身份不匹配冻结的v6.12 mlx5e工作流；不证明内核没有这些trace功能，也不把e1000e的IOMMU group 12当作translated domain、共享IOMMU服务或信任合同证据。本席只读取这份root收据，没有再探测host。

因此下文事件字段/触发位置仅对固定v6.12源码作确证；不能直接外推6.8事件格式，也不能从本机身份推出该冻结工作流已有原生记录。来源绑定的正常工作流条件分析仍可继续，数据/部署缺项不改写成等待一般授权或要求用户先保证收益。

## 来源与引用

固定 commit `adc218676eef25575469234709c2d87185ca223a`（官方 Linux v6.12，继承上轮 tag 解析）。`pph`、`dmah`、`xdph` 指本目录 `raw/include/trace/events/{page_pool,dma,xdp}.h`；本轮分别 HTTP 200，逐文件 URL/time/hash 见 download-receipts.json。`pp`、`dm`、`xdp`、`devmap`、`filter`、`helpers`、`dmahdr` 分别指旧 `/home/xvhaocheng/work/iommu-platform/out/mapping-admission-20261016/v612-source/raw/` 下 `net/core/page_pool.c`、`kernel/dma/mapping.c`、`net/core/xdp.c`、`kernel/bpf/devmap.c`、`net/core/filter.c`、`include/net/page_pool/helpers.h`、`include/linux/dma-mapping.h`。`mlx` 指同旧目录 `drivers/net/ethernet/mellanox/mlx5/core/en/xdp.c`。这些旧文件本轮复用，未重复下载 mapping.c。精确绝对路径、hash、阅读范围与继承收据在 source-manifest.json。

源码是已核 API/事件字段和触发位置的一手证据，不是性能测量；医学 I–VII 证据等级、期刊/掠夺出版与作者 COI 项不适用于这些内核代码。未提出全球文献/代码不存在主张。

## 现有事件与实际触发位置

| 事件 | 实际保存字段 | 真正含义与源位置 | 不能据此认定 |
|---|---|---|---|
| page_pool_state_hold | pool 指针、netmem、PFN、pool全局 u32 hold | 新页实际加入 pool，order路径 pp:498–513，order0批路径 pp:538–559；在 set_pp_info、A map成功后；批路径可先加入cache，尚未交任何RX。定义 pph:68–92 | 不是每次 RX alloc；不是每次正常 recycle；hold不是per-page代际 |
| page_pool_state_release | pool 指针、netmem、PFN、pool全局 u32 release | pp:673–693：普通页在 A unmap后，atomic release计数后、clear_pp_info/put_page前触发。定义 pph:42–66 | 是离开pool，不保证该put为物理最后引用；不是每次 frame/CQ完成；不证明B所有alias已卸 |
| page_pool_release | pool、inflight、hold、release、destroy_cnt | pp:607–623 仅 strict inflight读取分支；pph:14–40 | 名称不是逐页释放；inflight不是在途包/TX数；不是每次destroy的唯一epoch |
| page_pool_update_nid | pool、旧nid、新nid | pph:94–114；仅节点更新事件定义 | 不是alloc/recycle或稳定pool identity |
| dma_map_page | dev_name字符串、phys_addr（page物理地址+offset）、dma_addr、size、dir、attrs | dm:155–179：backend map之后、kmsan之后、debug_dma_map_page之前；dmah:36–71 | 无page指针/pool/frame/SQ/config/call-id；DMA地址不必是IOVA，可能direct；不是entry事件 |
| dma_unmap_page | dev_name、dma_addr、size、dir、attrs | dm:183–198：backend unmap之后、debug之前；dmah:78–110 | 无phys/page/pool/frame、无返回状态；不是配套map函数的exit计时事件；无DMA drain证明 |
| dma_map_sg | dev_name、输入phys数组、输出DMA地址/长度数组、dir、attrs | dm:220–223 仅 ents>0 才触发；dmah:178–222，数组长度反映nents/ents但不保存显式call-id | 不记录失败map；输入phys数组不含每输入段length；不保存sg_table/SG指针或pool |
| dma_unmap_sg | dev_name、输入SG phys数组、dir、attrs | dm:301–316 在backend unmap之前；dmah:224–254（数组字段名addrs实际赋sg_phys） | 不是DMA地址数组，不含size/原输出DMA列表；不能和page unmap一样当post事件 |
| dma_sync_single_for_cpu/device | dev_name、DMAaddr、size、dir | dm:361–393 在backend sync之后；dmah:256–290；dmahdr:284–345显示配置/need-sync可跳过 | 无attrs/phys/call-id；无事件不等于丢失，也可能合法noop；单个post事件无完整CPU计时 |
| xdp_redirect / xdp_redirect_err | prog_id、action、源ifindex、err、to_ifindex、map_id、map_index | xdph:90–169；native filter:4377–4445 在enqueue结果之后；xdp_do_redirect调用该逐frame路径见filter:4448–4472 | 参数和记录均无frame/page；成功只是redirect入队，不是B SQ接受或CQ；广播to_ifindex可为0 |
| xdp_devmap_xmit | from_ifindex、to_ifindex、action、sent、drops、err | xdph:268–302；devmap:376–416 在ndo_xdp_xmit返回、未发送frame归还之后，每bulk一次 | 无逐frame身份/顺序列表、无CQ；drops=原cnt-sent还可含egress BPF结果；不能把sent当已完成 |
| xdp_exception / xdp_bulk_tx | 前者prog_id/action/ifindex；后者ifindex/action/sent/drops/err | xdph:29–81定义；bulk_tx触发位置未在本轮冻结mlx5e链找到，不将定义当已触发 | 不是一般逐frame寿命事件 |
| mem_connect / mem_disconnect | allocator/xa指针、mem_id、mem_type；connect另含rxq指针和ifindex | xdph:321–380；xdp:357–362注册后connect，xdp:86 disconnect位置已定向查到 | 可辅助定位allocator，不是每page join/lastref；mem_id/指针没有永不复用epoch保证 |
| mem_return_failed | page指针、mem_id、mem_type | xdph:382–406定义；本轮检查的net/core/xdp.c没有该事件调用 | 不是正常frame return/last-ref事件；不拿仅有定义代替实际触发 |

普通 dma_map_single→dma_map_page 见 dmahdr:380–389，unmap_single→unmap_page 见 :392–395；冻结 mlx5e→B 的主帧和frags分别走 mlx:881–896，因而上述 page事件相关。这里的 dev 是 `sq->pdev`，不能把 event 的 dev_name 直接当 B netdev/ifindex；一个设备还可能承载多netdev/queue，实际DMA backend、domain、shared关系和权限仍需配置证据。

失败覆盖：dma_map_page 的backend失败可在dma_addr中留下 DMA_MAPPING_ERROR，dmahdr:75,95–101 给判据；但 dm:164–165 的无dma_mask提前失败不发事件，map_single的vmalloc拒绝也在进入map_page之前返回（dmahdr:383–388）。实际dma_addr_t位宽/事件格式须随build保存，不能固定把某个32/64位值一概判成功。SG map只记录成功；unmap本身void，事件无失败/实际硬件失效确认字段。不能以事件数当全部attempt数。

## alloc、recycle、完成与最后引用的缺口

pp:423–436、577–590缓存alloc快返没有state_hold；pp:764–773直接recycle/候选recycle没有state_release；bulk pp:866–876先做is_last_ref，pp:882–902入ring或溢出时真退出。因此 hold→release 区间可跨很多次alloc/recycle，也可含从未实际使用的预分配页；仅区间长度不能还原再用频率。

原生 CQ 路径在 mlx:748–803 按CQE推进多个WQE，再由 mlx:669–700 逐wi->num_pkts取FIFO、unmap、xdp_return_frame_bulk；异常关闭释放也调用同一free helper但传NULL CQ/CQE（mlx:819–840）。因此 dma_unmap_page事件不携带“正常CQ / SQ失败清理 / teardown”分类；发unmap也不单独证明这是一次有效完成。TX失败但map成功会在mlx:909–922马上unmap，必须和CQ unmap分开。

mlx:940 累加nxmit、:947–950可发doorbell并返回接受数；devmap事件只保存聚合sent。xdp:455–508还会把frame/frag归还批处理，真正pp最后引用判断在helpers:245–287,305–323以及pp:869–873，没有对应正常last-fragment事件。这些源码位置的信息存在，不等于被现成事件记录。不能把flush、doorbell或CPU grace当DMA完成。

## 分级配对合同

**L0——源寿命内B映射出现。** 需先有完整pool epoch及page ownership episode；以观测到的hold建立episode，匹配相同pool/netmem/PFN的release结束。hold/release计数只是pool全局序数，32位可回绕；pool指针、PFN、mem_id都不可独立作永久identity。对普通order0页，可按实际PAGE_SHIFT把phys_addr范围归入该episode，保留offset/size；map跨页则按每页交集处理，不把一次API调用同时算成多个packet。只有确定事件dev_name确属所选B/config、并排除其他同device映射，才能计B map；source的A pool map在hold之前，勿混作B首次使用。

**L1——真正接受的逐frame B工作。** 需额外逐frame/packet序号与page/offset/frags、目标B/config、SQ/producer/slot或可配对事务ID，并记录driver接受/拒绝及失败清理。现成redirect无frame，devmap_xmit只有批sent，不能按时间就近猜哪几条map是成功发送；需允许当前正常批处理和egress BPF改变列表。

**L2——匹配完成、最后片段与source真正退出。** 需将accepted frame绑定SQ/WQE及其epoch，记录正常CQ、错误/teardown、最后frame/fragment引用归还。源release marker可结束pool episode，但不能补齐每个frame完成身份。最后物理free另是更强事件，本合同不从pool release推出。

map↔unmap的弱关联键只能是配置已识别的device lifetime＋DMA地址＋size＋dir＋attrs（结合该map携phys），并维护每个键的活跃实例集合。IOVA/总线地址可以复用，direct下同一页并发相同map可得到相同DMA地址；同键多活实例时无call-id，必须标ambiguous，不能擅自FIFO/LIFO。即使零丢事件，post-unmap也有并发ABA：CPU0的backend已归还IOVA、尚未发trace_unmap，CPU1便可能复用同IOVA并先发新trace_map；此后CPU0旧unmap事件不能按时间戳活跃表卸掉CPU1新map。必须有无歧义配置身份与调用配对，或entry/return调用ID；存在这一可能且无法排除时隔离ambiguous，不强行推生命周期。它不妨碍按每条map自身phys字段和完整源episode计映射出现，却阻止把该次数升级成accept/CQ链。SG unmap缺DMA列表/length，只靠phys数组也可能同键并发重复，需独立配对ID或更强被证顺序。map成功与unmap的间隔是**映射驻留区间**，不是map/unmap函数entry/exit CPU区间。

## 拟议的付费观察接口（本轮未开发）

| 需要补的关系 | 最小字段/观察点 | 与现成事件的关系 |
|---|---|---|
| pool与source ownership episode | pool注册/销毁epoch，source join ID，普通页匿名稳定ID或PFN+代际；真实退出原因/阶段 | pph只有pointer/PFN/全局计数，无完整epoch/原因；可用可靠控制面快照补部分 |
| 每次供给/回收 | alloc/fragment的offset/length、use ID、cache/ring/slow来源、每次return及last-pp-ref结果 | 需在已定位alloc/recycle/pp-ref源点观察；不能说hold/release免费提供 |
| B调用、真正接受与完成 | frame/use ID＋主页/frags，B/netdev/SQ/config epoch，map call ID/result，accepted/rejected，WQE/CQE和错误/teardown类型 | DMA事件有地址，XDP事件有路由/批计数；二者缺稳定联结，须新增关联或经验证函数探针 |
| 完整map/unmap CPU | 各函数entry/exit、同调用ID、执行CPU/上下文、嵌套/抢占信息、失败早退、批处理边界 | 本席仅提供dm/dmahdr/driver准确源点；实际计时方法和完整CPU归因由root裁决；不把新增探针称免费 |
| 物理/关联及Q变化 | source/ref/Q等状态与其归因ID，control/worker责任、最终释放及重分配 | 现有事件不能推候选反事实RAM或Q；候选影响recycle后需逐策略轨迹 |

固定工作流的实际运行内核commit/config、HAS_DMA/PAGE_POOL、trace事件启用/过滤、缓冲/丢失统计、CPU与时钟、原始二进制format、DMAaddr宽度/PAGE_SIZE、目标netdev与PCI/dev/config/domain对应、RX页/片段模式均 unknown。本机release/架构和网络driver仅按上节root收据已知；tracefs读取EACCES保留为可读性缺口。不得从源码文件存在推当前运行kernel支持；本席不额外探测运行配置。

## 丢失、并发和删失规则

- 仅有release没有hold：left-censored；仅有hold没有release：right-censored。可报告已观测的映射数，不把尾部当正常真退出，不把边界page归零或无限寿命。新记录中仍活的映射也属right-censored，不能补造unmap。
- 丢事件、过滤、跨CPU顺序不确定、clock不一致或计数异常时，按受影响pool/设备/时间范围打不完整标记；可给明确观测下界，但无丢失上界时不编有限上界。u32 hold/release计数有序性只能在已证epoch/无丢失/回绕规则下使用；release并发原子分配序号不保证事件按序抵达。
- 地址、pool/netmem/frame/SQ/WQE counter和mem_id可能复用或回绕；新epoch不得和旧同值合并。时间相邻不等于同frame，不能单凭PFN、pool指针或dev_name匹配。
- 多CPU、多SQ、批量map/CQ/return必须保留批成员及其接受子集；当前events没有的成员不能按平均分摊伪造。order0不自动等于一页一包，需保留片段最后引用。
- 重复map计数分为尝试/成功/接受/有效完成四层；只有已证字段支持相应层。map成功而TX拒绝/后续drop不是有效完成，但其CPU可能真实发生，由root另归因。
- O0记录可作机会界和来源条件；候选改变回收/Q/分配后不能强套同PFN顺序当闭环实测。所有CPU、物理集合、带宽与有效完成分开报告。

## 最小未核项

1. 是否已有**匹配此原生工作流**的可用原始记录及构建/设备配置：本轮已列资产未给出，不扩仓扫描、不启动采集。
2. 只有L0字段齐全时，先做来源绑定的重复map出现；若要提升到L1/L2，需上表付费关联字段，不能补故事。
3. 完整CPU计时、batch/失败归因及机会上下界由root裁决；本席没有CPU数字、性能收益或新意裁决。
