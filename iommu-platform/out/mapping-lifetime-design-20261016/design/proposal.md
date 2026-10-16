# 映射寿命与源分配器：两个具体版本，深入 V1

2026-10-16。本轮按 `root/task-contract.md` 独占做正向机制发展。沿用 ARS 的有界机制设计与实质前作比较；不启动完整 pipeline。请求沿用 Astra／xhigh，实际运行模型与强度不可独立核实，记为 unknown；没有声称原位切换。全程 Codex-only，未启子席。

## 结论

**推荐深入 V1：在已有 order-0 page_pool 上，让源分配器继续掌握物理页，把 B 的持久映射作为随源退出而撤销的关联；完成记录复用已有 frame 与 FIFO 槽，不给映射缓存额外的普通 page ref。** 它保留“组织映射寿命以降低费用”的核心思想，但不再要求 PMD 连续分配、批次 seal、搬走 CPU 分支或预测未来路由。

这不是新的接纳算法，也不能认领一般“弱关联／owner 通知退役”协议：root 已核 dma-buf 动态附件有相近能力；DMA_PMD 原 RFC 也已有源块回收前撤各域映射和域退出协调。现在值得裁决的是：**在实际 page_pool 回收和 XDP 完成边界上，能否用更低的表示／同步费用提供同样可组合的退役，同时避免映射缓存改变源页回收行为。** 若普通 O3＋同样 release 接口或 dma-buf adapter 以同等成本提供它，就把 V1 收为普通部署方案；不能扩大成整个映射组织思想无价值。

CPU 或不可回收内存改善均可作为研究价值，不再等待一般性确认。本席只在明确合同条件下推导，不把来源不足写成用户未授权，也不把较宽的授权当免费条件。

## 1. 两个版本与选择理由

| 版本 | 具体新增组织 | 当时可知信息与执行点 | 相对普通方法的准确位置 |
| --- | --- | --- | --- |
| **V1：源持有、B 关联可撤销** | B cache 不增普通 page ref；源真正离池前关闭并撤销所有 B 关联；cache 自身淘汰只撤 B，不退源。持久模式的完成 FIFO 用关联 handle 替换每帧 DMA-address 槽。 | B `dma_map_single` 前已有 frame、目标 DMA device 和长度；源 `page_pool_return_page` 是归普通分配器前的门。当前路由与历史可用，未来再用次数不可用。 | O3 的策略可完全相同。候选是 page_pool 本地的接口／表示／退役整合，须与 O3＋同接口、dma-buf adapter、RFC 粒度投影比完整成本。一般通知和映射缓存已有。 |
| **V2：目标紧凑副本承担 TX 等待，源页恢复供给** | 在 B 接受之前私有准备紧凑副本；仅在实际接受原输入责任后归还原 frame，副本等 B 完成才归目标池。根据真实源池供给和目标容量限制选择，而不是只看 copy 与 map 的 CPU 小计。 | B map 前有长度和当前供给／占用；SQ 接受在后面才知道。源回收与后续 RX 分配必须按策略实际改变。 | O1 紧凑复制、DAMN／shadowing 的池化与普通 pressure-copy 已覆盖动作。多看一个水位不是新策略；该版本主要作为完整资源对照和来源问题保留。 |

选 V1 的理由：已核源码给出一个直接机制冲突——普通额外 page ref 会使 page_pool 放弃 recycle，而映射缓存通常只需要关联有效性；V1 的改变能直接作用于此冲突。V2 则必须先发现“早归源使实际源增长或供给阻塞减少”的正常范围，不能只凭原 frame 提前 return 推出总内存收益。当前不把 V2 另开为系统开发或性能模型。

## 2. 最小来源与合同

本席直接核官方 v6.12 保存原文，commit 为 `adc218676eef25575469234709c2d87185ca223a`，文件位于上一轮 `v612-source/raw/`，不是生产树。必要边界如下。

- `net/core/page_pool.c:736–790`：普通 `page_ref_count==1` 才走 page recycle；引用升高时走退出 pool 路径。因此“给 B map cache 加一个长期 `get_page`”不是零副作用的实现。它可能使原页离开 A pool、把引用留给缓存，并改变下次 RX 的分配。
- 同文件 `:648–698`：退出先处理源映射，再增加 release 计数，然后清 pp_info、put_page；注释明确最后一页可能释放 pool。新 owner 通知必须在这些失去源身份／生命周期保证的动作之前有正确位置，不能事后查已清的 pp_info 或引用可能已释放的 pool。
- 同文件 `:1049–1058,1115–1134`：pool 销毁包含 scrub、inflight 检查和延后重试；单包回收、页离池与 pool 对象释放不是同一事件。
- `en/xdp.c:877–922`：map 发生在最终 SQ 接受之前，失败会卸图；不能提前知道接受结果。
- 同文件 `:673–700,925–931`，`en/xdp.h:75–101`：当前 FRAME 路径保存 mode、frame 指针、DMA 地址，完成时卸图再归 frame。可以提出“持久模式用 handle 代地址”的表示方案，但现有代码没有这种新模式，也没有测 sizeof、路径开销或实现正确性。

V1 的首版保持 stock order-0、一页一 frame 的布局，不把多个 order-0 PFN 假装成连续 PMD。只主张减少重复 map/unmap 的可能 CPU 费用，不主张更大 IOTLB reach、更少 miss 或更快受保护 consumer。

**T 分支必须写实到页：** B 规定权限须允许覆盖被接纳源页的全部映射字节及其在本次源所有权期间的后续用途，包括后续 PASS／CPU 路径、headroom、元数据与空洞。仅“本包去 B”不能授予这个合同；若原合同不允许 B 观察后续 CPU 路径数据，本页不得留 B 映射，可选精确短 map 或合法目标 copy。A 的长期权限不替 B 作决定。

**S 分支：** 若每个对象有固定物理撤权边界，V1 的跨对象持久关联不适用；仍可复用退役接口的工程部分，但每次实际解除授权必须满足 S。不能把 Linux strict unmap 的同步设置直接当作逐包 S 要求。

首版部署投影另需明确翻译 DMA、无 bounce、授权域／DMA device 的真实关系及一致性条件。非 coherent 下每次 sync 仍需做；缓存映射不能跳过 CPU／设备可见性。基于 domain 等价共享关联前还须满足 DMA mask、ops、属性及设备生命周期，不能用两个 netdev 或相同裸 domain 指针直接代替这些检查。

## 3. V1 的三个寿命与状态

### 3.1 不再让一个引用同时承担三件事

1. **源存储所有权**：由 A page_pool 及正常 frame／consumer 引用链维护；决定页是否可再次 RX、是否可离池归一般分配器。
2. **B 映射关联**：记录源身份、目标 DMA device／配置代际、方向／权限、实际 IOVA、映射范围、状态和元数据寿命。关联存在本身不取得长期普通 page ref，也不因其缓存热度阻止源退出。
3. **一次已准备／已提交的使用**：短时关联 token 防止该次 map 地址被 cache 淘汰或域退出撤走；实际 DMA 完成及 frame 责任仍按现有 TX 链处理。token 不是 DMA 完成证书，也不替代原 frame 的存储保活责任。

映射 key 至少含 `(source owner identity, source generation, page/span, target DMA configuration identity, direction/rights/attrs)`。source generation 只在源所有权结束后重建；正常 return 到同一个 pool、PASS、下一个 packet 都不自行换代。PFN 与 pool 指针可能复用，不能单独当唯一身份。

源状态为 `OWNED → EXITING → RELEASED`；单个目标关联为 `ABSENT → CREATING → LIVE → CLOSING → DEAD`。这些是语义状态，不是已实现 C 结构、锁协议或形式化证明。状态表大小、锁粒度和读者回收成本均待计费。

### 3.2 从当前 frame 到最后退役的实际动作

| 边界 | 拟议动作 | 必须维护的责任 |
| --- | --- | --- |
| B map 前 `en/xdp.c:877–882` | 原 frame 仍有效时识别源 owner／generation，取得 LIVE 关联的临时 token；若需首建图，创建与源退出、B 退出串行化。copy／短 map／持久 admission 仍可由普通 O3 决定。 | 不能从裸 PFN 建立无 source 生命周期保证的 cache；source 已 EXITING 或目标配置已关闭则拒绝新关联。状态／元数据分配失败须走合法 fallback。 |
| 准备后、SQ 接受前 `:909–922` | 进行实际 per-use DMA sync；尚未承诺 B 会接受。 | SQ／map 失败先撤本次短时 token 和临时资源，原 frame 仍由 devmap 未接受后缀路径处理。首次建立后未被接受的闲关联保留还是撤回须计费，不能免费抹掉尝试。 |
| 实际接受后 `:925–931` | 以新 mode 保存原 frame 和关联 handle，使用关联 IOVA 加合法 offset。 | 原 frame 的正常存储责任保留到真实完成；不会因为有持久 mapping 就提前归 RX。这里没有旧整页 seal。 |
| B 完成 `:673–700` | 持久模式先释放本次 token，再正常 `xdp_return_frame_bulk`；短 map 模式保持原 unmap；copy 模式按自己的目标 owner 归还。 | release 顺序使源最终 return 看不到一个本应已经结束的 token。每帧归还一次，prefix／失败／关闭均须遵守。 |
| cache 主动淘汰 | 标记该关联 CLOSING，禁止新 token；等真实已有使用结束，卸该 B 映射并回收关联。 | **不要求源页离 pool，不延后别的页 RX，也不改 source generation。** 同页后续需要 B 时可重新 map，费用照计。 |
| 源页真正离池 `page_pool_return_page:673–692` 前 | owner 从 OWNED 改 EXITING，禁止注册／lookup 成功；关闭所有目标关联，完成实际撤销、同步及必要读者退出，然后才执行原本失去 pp_info／source owner 的步骤。 | 普通源引用应已保护正在 TX 的 frame，但仍必须核 token／完成不变量，不能只收到 notify 就 free。若发现仍活跃或撤销需要延后，该页隔离保留，不能回一般分配器。 |
| 目标设备／配置退出 | 停新关联和新 token，走真实 driver DMA drain，再与源退出协调唯一 unmapper；映射的 device／domain 元数据在此期间保持有效。 | source EXITING 与 target CLOSING 同时发生时，不双 unmap、不漏 unmap、不访问已释放页表。driver drain 仍须源码证明，不能由关联 refcount 代替。 |

页离池路径可能在网络回收上下文运行。首版应先核能否在所选真实 DMA 路径以可接受次数同步撤销，而不是假定可拿任意睡眠锁。若需要 worker／grace period，则显式保留页、源控制块与目标所需元数据，并计入队列、等待、内存和退役尾延迟；不能在 worker 完成前执行原 `put_page`。该 deferred 路径的设计还未闭合，不作为免费可用能力。

映射的 fan-out 必须有限，或至少有实际可控上界；例如一个 source generation 的目标关联达到部署给定上限后，后续目标退回普通短 map／copy。该容量限制不是新策略，也不能把普通对照的关联上限压低来制造胜利。

一个重要反例是释放顺序倒置：B 完成若先 `xdp_return_frame_bulk`，最后 source return 可能同步进入 owner-close；owner-close 又等待这个尚未释放的 B token，就形成自等待。**先还 B token，再还 source frame** 是必须验证的顺序，不是性能选择。token／cache 对关联元数据的 refcount 与普通 page ref 是两种引用。

### 3.3 具体低成本表示：复用完成记录，而不是加一套 owner

stock 单 frame 的 FIFO 已存 mode、`xdpf` 和 `dma_addr`（`xdp.c:925–931`）。持久模式完成时无需用每帧地址做 unmap，可提出把该 DMA-address 槽换成关联 handle：原 `xdpf` 仍给出正确的 source return；handle 给出短时使用与关联状态。

这是一个可以逐字段审查的表示差异：不为每帧再包一份 source-owner 对象，不需要从 IOVA 反向猜该返回哪个 allocator。映射 key、map base、目标对象和生命周期集中到实际活跃关联中。**它不证明零新增字节**：模式字段、union 最大成员、alignment、关联表、free-list、读者同步及 error 状态都必须核，尚未编译。普通 O3 和等价 dma-buf adapter 同样可以复用这个完成槽；不能把它们固定为低效 wrapper 后认领优势。

### 3.4 更窄的部署合同：原生上下文中的首次需求接纳

root/prior 已指出 generic dma-buf mapping 入口带 `might_sleep` 和 reservation 锁义务，其原接口不能未经适配就放进 NAPI 首次 map miss；root 已另行直接核 `dma-buf.c:1118` 及 v6.12 `dma-iommu.c:872` 的 `GFP_ATOMIC` 路径，精确版本与 source manifest 由 root 接入。本席据此提出一个**实现仍待核实的具体部署版本**：控制面预先注册 source owner 及 release 钩子，预分配有界关联元数据；B 首次实际需求到达时，沿当前 native TX 已有的 DMA_API 路径尝试建图，接纳过程不新增睡眠接口。容量不足、map 失败或 owner 关闭时走普通 fallback，失败代价照计。

这不要求每个 source page×B 在控制面全部预映射，也不需在数据路径导出 fd／构造通用分享对象；但预分配容量、预注册、表查找、锁／原子操作和退役服务均有成本。只有真实调用链确认新路径不睡眠后，才能说达到该部署合同，当前没有实现证明。

强 dma-buf 对照允许控制面提前 export／attach／map，热路径只查 importer cache；首次缺失可以 defer/workqueue 或 copy fallback，并计算其首次服务、预建图资源和生命周期费用。不能说它“每包都睡眠”或“不支持网络”。O0 本来可沿原路径处理首次 map；O3＋同 owner 接口也可支持本候选的首次需求行为。因此这个条件只把接口成本比较变具体，不形成策略创新或已证明速度优势。

## 4. 最接近工作与具名强对照

| 对照／前作 | 已有或应允许的能力 | 本轮可以比较、不能偷换的部分 |
| --- | --- | --- |
| O0：stock 逐次 B map/unmap | 现有源 pool、正常 IOVA 分配缓存、正确批处理和同步 | baseline 未强制新增 source PMD，也未每包全域 flush。V1 必须扣除 lookup、token、同步和最终退役费用。 |
| O1：目标紧凑池复制 | 目标永久映射、正确 completion owner、同信息供给规则与 pool 回收 | 它是 V2 的直接强对照，不作为新方法。源早归不等于少物理内存。 |
| O3＋owner release 接口 | 同一 copy／短 map／持久策略，同源信息、同新增付费接口、同表示优化 | **策略可与 V1 完全相同。** 相对它只有具体实现／接口成本差，不可再次声称累计阈值或 history key 新。 |
| 普通 pin-cache | 为持久关联增加 page ref 并在 eviction 才 put | 用于解释真实 recycle 冲突；不是唯一、更不是最强 baseline。应展示其反馈，不能凭击败它就认领贡献。 |
| dma-buf 动态附件等价 adapter | root 已核 owner move-notify、动态映射生命周期、方向缓存与 reservation 协调；core notify 不自动完成 importer 的撤图与 DMA 等待 | 不认领“可撤销弱关联”。允许 adapter 复用对象、按 pool／窗口组织、只在需要处锁定；不能强制每 packet 导出 fd／构造完整对象。须比较同合同下真实可执行的 adapter。详细源位置由 prior 席／root 接入。 |
| DMA_PMD RFC 的普通粒度投影 | 02/22 已有 pool release、异步撤图及 RCU；07/22 已有跨 domain 撤图和 domain teardown；08/22 已有首次 map／后续复用 | 本席直接核原文：`lifecycle02.txt:25–39`、`domain07.txt:25–44,150–200`、`mapping08.txt:25–40,302–317`。不能把“由 owner 撤图”认领。order-0／READ-only 等改写是普通可比较适配，适配成本不能免费，原 PMD RW 也不能冒充同合同。 |
| DAMN、shadowing、fbuf、rIOMMU | 已核池／复制／映射重用／路径或 ownership 等机制继续作为近邻 | 本席复用前轮报告；rIOMMU 与动态附件本轮由 prior 席定向核。未在本席独立精读的结果不冒充本席证据，也不以没读到全文断言全球缺项。 |

因此当前 novelty 命题必须收窄为：**已有工作给出持久池和 owner 协调的一般机制；本候选尝试在 page_pool 保有物理页的既有快路径上，提供适配网络回收上下文的低成本多目标关联退役，并复用完成记录避免重复 owner 表示。如果同合同 adapter／普通适配以相同费用做到，就不具备该版接口／实现增量；若一个可核的同步、表示或 source-supply 差异仍在，才有系统贡献的空间。** 目前差异是拟议、未测，不是新意成立声明。

## 5. CPU、物理内存和供给反馈分别归因

### V1

源 pool 本来拥有的物理页，不因 B 多一个映射就多出一份 payload。活跃 B DMA 对原 frame 的正常持有，也不全归咎于 mapping cache。可能新增的物理保留发生在：源本应离池释放，但 B 关联关闭、同步或 worker 等待使它暂留；以及错误 pin 改变源回收后的真实后果。IOVA、PTE 和关联表是另一类资源，须单独计物理支撑内存与虚拟地址占用。

CPU 账至少包括首建图、所有实际撤图、每次查询／token／状态分支、原本需要的 DMA sync、源退出 callback、目标退出串行化、元数据申请／回收、grace period／worker、失败与 cache eviction 重建。相对 O0 可能减少重复 map/unmap；相对 O3＋同接口，这些策略事件可完全相同，只能比较具体差项。

不能使用“所有策略固定同 PFN 再用轨迹”的完整闭环结论。若 pin-cache 导致原页离 pool，下次 RX 就可能慢分配另一页；若 V1 retirement 排队耗尽可用供给，也会改变后续源页。应固定外部到达、路由与完成义务，分别生成实际 recycle／allocation／retire 过程，记录 source-generation 改变；源码绑定的开放条件分析可以先做，但不能把 policy 后果固定掉。

### V2

原 frame 早归 A pool 后可能只是留在 cache，未实际释放；目标副本又占一份存储。只有它避免了本来必须发生的源池扩张、缩小了可兑现的源保留容量，或在相同总内存下解决真实供给阻塞，才有相应收益。目标紧凑分配确实可能以小于一页的容量容纳一个 packet，但要算 chunk rounding、空洞、未满 cache、每对象元数据与最后引用；“小 packet”不自动带来实际紧凑物理占用。

V2 顺序必须是：准备私有 copy → 实际成功接受输入 frame 责任 → 原 frame 归 A → B 完成后 copy 归自己的目标池。未接受时销毁私有副本，原 frame 留给原 owner 路径；拒绝后缀、map/SQ 失败都收费。不能为早归源在接受前双返源 frame，也不能让复制对象携带源 mem.id 被误归 A。

若 A 是固定常驻容量且不会缩小，copy 新增目标池通常不能直接称总物理内存减少；仍可比较供给／吞吐条件，但所需总内存预算不能暗增。这个分支暂作为 V1 的资源强对照，不为证明 V2 另建模型。

## 6. V1 的决定性预测与相邻失败

**正向条件序列：** 源页 p 在 A pool 内正常分配并保持普通 page ref 为原状态；B 真正需要时首建 READ 关联并发一帧；B CQE 后先还 token、再正常归 source pool；p 可再次 RX，期间 T 明确允许 B 对该页所有后续用途的规定权限；下一次 B 使用命中同 generation 的关联。之后源真正决定离池，先关闭所有 B 关联并完成实际撤销，再清 source 身份和归普通分配器。整个序列不需要物理 PMD、不等待未知 packet 拼批、不增加长期普通 page ref。

可推翻预测是：**在这个来源合同中，V1 能保留 stock 的 recycle 条件，并把多个 B 使用期间的映射操作摊到一个 source generation；如果第 3.4 节接口条件成立，还能在原生收发上下文首次需求时接纳，不依赖全量提前映射。完整 CPU 节省仍只有在减少的 map/unmap 大于全部查询、同步、表示和退役差项时成立。** 相对最强同接口 O3／adapter，只有这些差项确实不同才有独立实现价值。不能从 source 行号直接断言该不等式为真。

相邻失败应至少覆盖：

1. **同一页只去 B 一次便离池。** 无重复使用可摊销，新关联维护可能比 O0 更贵；不靠未来预测逃避它。
2. **同一页后续转为不允许 B 读的 CPU 用途。** T 范围不成立，持久关联必须撤销或起初拒绝；首次 B route 不能扩大授权。
3. **源退出先清 pp_info／put_page，B 撤图仍在排队。** 同 PFN 被新 owner 使用后旧 B alias 仍活，协议失败；generation 只能拒绝新查表，不能消灭已存在的硬件权限。
4. **source EXITING 与 B device/domain teardown 并发。** 若重复 unmap、访问已释放 domain、或仅 notify 未等实际 DMA，就失败。每条关闭路径必须拥有一次性清理责任。
5. **高关联 fan-out 或回收上下文不能承受撤销工作。** deferred quarantine 的实际内存／延迟吃掉收益；不能把必要 worker 设为零费用。
6. **普通 O3＋release 接口或 dma-buf adapter 复用同 FIFO／状态后，差项相同。** 此时 V1 是普通正确集成，停止该实现新意主张；不据此关闭 V2 或更一般组织问题。

## 7. 本席接受范围和下一份最小判别

本轮接受的是一个具名、源绑定、可反驳的 V1 设计，及 V2 资源对照。当前没有实现或性能结论。下一步应限于：

1. 把 source release、target drain 和 map-cache reader 的实际上下文／锁顺序接起来；精确注明哪些责任由正常 frame 引用承担，哪些还缺接口。特别核 `pages_state_release_cnt` 之前的 owner 元数据生命期，以及延后释放时如何保持其正确计数。
2. 为 V1、O3＋同接口、dma-buf adapter、RFC 普通粒度适配列同一份字段／调用表。允许全部合理对象复用和 FIFO 槽复用；若无具体差异，给出“普通集成”结论。
3. 用一个当前可达的 recycle→reuse→source exit 序列和一个 source／target 并发退出反例审查责任。只在新的具体差异需要测量时，再决定一个有界实现或实验，不自动建系统。
4. CPU、physical bytes、IOVA/PTE、带宽、完成与 source supply 分别记账；根据改变的分配行为更新闭环，不再补上一轮累计阈值的示例数字。

root/prior 的动态附件与 rIOMMU 核查结果是最终接受依据的一部分，本文不提前宣布其未覆盖。一般授权与 CPU／内存价值已经允许条件分析；只有出现具体互斥部署取舍才需要新的用户决定。

## 证据与执行

本席直接阅读的关键源、hash 和范围见 `source-manifest.json`；前轮报告与本轮 root 传入的近邻信息明确区分。本席没有联网、实验、性能／功能模型、构建、生产代码／ABI／RTL 修改、硬件操作或 canonical 文档写入。只有本目录三份设计文件。由 Codex 辅助完成有界设计，尚未验证实际成本或证明论文贡献。
