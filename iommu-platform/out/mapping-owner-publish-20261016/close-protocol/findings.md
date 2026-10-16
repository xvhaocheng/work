# MAP-OWNER-RECALL-PUBLISH-1：请求、关闭与退休合同

2026-10-16 JST。Codex-only；沿本席请求 Sol/xhigh，实际 runtime metadata unknown，未声明原位切换。继续已选 ARS 机制设计／定向原文核查；只读保存源码并写手工条件偏序，没有并发实现、状态／性能模型或设备运行。

## 1. 可交给 root 评估的具体方案

**保留单 source-pool / 固定 B/config 的有界控制 mailbox：miss 只合并当前 pressure 代际；owner 每次取一个请求 snapshot、最多关闭一个实际 private-cache 页关联；ACK 报“已尝试及结果”，UNMAP_DONE 报实际撤图阶段，RETIRED 才返 K。** 关闭 owner 的 gate 与 NAPI 唤醒共用短锁；关闭后 ctx 可继续处理晚 frame／record 退休，但不再触碰旧 channel/NAPI。NO_ELIGIBLE 终结本代，不自唤醒重试；首版只接受明确的控制／库存变化 REARM。

这不增加逐 TX 或逐 alloc 独立 use-token，但增加 miss/control 门、mailbox/epoch、owner action、退休节点和 ctx 寿命管理。既有 TX 查找／ready/DMA地址读取与同步费用仍在。普通 R/O3/slot 得到同样接口和全部费用。条件协议不是已证明正确的实现，也没有时限或净收益结论。

## 2. 已核原文及其实际边界

本席 primary source 为已保存官方 Linux v6.12（commit adc218676eef25575469234709c2d87185ca223a）；精确字节/hash/阅读范围见 source-manifest.json。别名 M=en_main.c、PP=net/core/page_pool.c、H=include/net/page_pool/helpers.h、T=include/net/page_pool/types.h，M 的完整路径为 drivers/net/ethernet/mellanox/mlx5/core/en_main.c。

| 原文位置 | 能支持的事实 | 不能直接推出 |
| --- | --- | --- |
| M:924–950 | 普通 RQ 创建 order0 page_pool，pool.napi 指向 rq.cq.napi，并注册 PAGE_POOL 内存模型 | 裸 pool/NAPI 指针有独立后台寿命 |
| M:1371–1384 | RQ disable 位后 synchronize_net；close_rq 取消已有 dim/recover work，再 destroy_rq、free_rx_descs、free_rq | 新增 reclaim worker/ctx 自动包含在已有 cancel_work_sync 内；CPU同步等于 DMA drain |
| M:1017–1030；1190–1193 | free_rq 调 page_pool_destroy；destroy_rq 转到 mlx5_core_destroy_rq | 已读到核心/FW destroy 的完成、错误和 DMA 停止语义 |
| M:2748–2776 | deactivate_channel 最后 napi_disable；close_channel 关队列、del NAPI、free channel | napi_disable 自动排空新增 ctx/metadata callback，或代表设备 DMA 已完成 |
| M:2587–2605；2077–2086；1760–1763 | close_queues 先关闭 XDP SQ 后 RQ；close_xdpsq 在 CPU同步后 destroy_sq 再清残余 desc | destroy_sq 下游真实 stop 完成已在本席证明；清 desc 等于正常成功 CQ |
| M:3197–3216；3403–3418 | 设备 close 先禁新 TX/XDP、deactivate channels，再 close channels | 固定 B 映射的新 gate 已存在；可以关共享 source A 来代替 B 控制关闭 |
| PP:607–623；673–698 | pool inflight 是 hold−release；source page release 可能使 pool 不再可引用 | ctx ref 能替代 pool pin；release后还能回读 pool |
| PP:1021–1058；1115–1134 | destroy 假定 allocation 不并发，清 cache/ring；inflight 未零则延后 retry | page_pool_destroy 返回代表所有 frame、pool、metadata 已销毁 |
| PP:1005–1018；1061–1084 | 最终 disconnect/uninit/kfree 与 delayed retry 是 pool 自身机制 | 外部 ctx/worker 自动被这个机制管理 |
| PP:1087–1112；H:446–449；T:225–229 | pool.user_cnt 有自己的用途；直接回收 unlink 要求 NAPI 已 disabled；最终可清 pool.napi | user_cnt 是给任意后台 worker 的通用 pin API；NAPI link 可以不经寿命协议长期缓存 |

因此，**channel/NAPI 可以已删除，pool 却因旧 frame/inflight 继续存活**；ctx 的 owner 指针寿命和 source pool 寿命必须分开。PP:684–685 的最后 release 后禁止回读 pool 尤其不能被 metadata callback 绕过。

NAPI 核心 schedule/complete/disable 原文由另一席独占核查。本席不重读 dev.c/header；第4节锁内 schedule 与 hook 后 repoll 是 root 提供的接口前提，最终以 root 接受的 NAPI 原文裁决，不能把本席文字当核心语义证据。

## 3. 最小状态与四层完成

每个 pair ctx 预留固定 control mailbox，记录 source_epoch、B_config_epoch、pressure_generation、req_seq、pending snapshot、active snapshot、带序号的终结结果。至多一个 active 和一个 pending 合并批次；不排队保存 frame/packet，也不为每个 miss 分配请求节点。请求序号／代际不得在仍可能被观察时回绕或复用。K 个记录连同其 retire 节点继续共计 K。

| 层级 | 何时可发布 | 明确不代表什么 |
| --- | --- | --- |
| REQUEST_ACCEPTED(s,g) | gate/epochs 合法，pending 已受保护地记入并完成合法 kick；同代 miss 可返回 COALESCED_WITH(s) | owner 已运行、页存在或 slot 已空 |
| OWNER_ACK(s,result) | owner 对 snapshot s 做了最多一个动作，或给出 NO_ELIGIBLE、SOURCE_CLOSING、B_CLOSING/RETIRE_ALREADY_PENDING 等具名结果 | B unmap 一定完成、后台 metadata 已退役、实际 DMA 全局排空 |
| UNMAP_DONE(record_epoch) | 唯一 closer 已按配对参数完成 DMA API/backend 所要求的撤图与同步，并移除旧 live 使用入口 | CPU metadata 无旧读者、K 可复用；也不自动证明其他 B TX 已完成 |
| RETIRED(record_epoch) | 必要 CPU reader、metadata、索引／配置引用及异步责任全部退役，旧 generation 不再可访问／误用 | payload 物理内存减少、请求触发的那一帧有收益 |

同步正常分支中，ACK 可以携带 UNMAP_DONE=true；仍保留独立字段和 record generation，不能用单个 done 位同时代表四层。ACK 保留空间也必须有界；旧结果被覆盖后查询只能报告 STALE_RESULT，不能拿更大 ack_seq 猜某旧请求已成功返槽。请求 miss 不等待 ACK，不持 frame 等 slot，照常 O0。

## 4. 发布、snapshot 与显式 rearm

使用 ctx 的 irq-safe 短锁 L 统一 owner OPEN/epoch、pair B 控制门、pressure_generation、pending/active。只在短锁内改控制状态和执行经源码核准的非 inline-poll napi_schedule；不在锁内 scan、DMA unmap、等待 CPU grace 或执行可能阻塞的回调。这里采用 root 指定的直接 schedule 语义，不能不经核查换成可能带其他同步行为的包装器。

**miss 发布：** 调用者先有 ctx 的合法寿命来源；在 L 内核对 source/B epochs 和 OPEN。当前代若已 pending/active/终结，则只合并／返回已有结果。只有已显式 REARM 的新代才能占用空 mailbox：先发布 pending，再在同一 L 临界区合法 schedule，最后解锁。关闭者也用 L 关门，因此不能在已解绑 NAPI 后出现早先 publisher 的迟到 kick。当前 miss 不因请求成功而改变 O0 决定。

**owner 执行：** 在真实合格 owner 位置取得 L，只取当前 pending snapshot s 并清除此旧 pending，将 s 标 active；解锁后检查合法 gate、实际 private cache 与唯一 closer，最多取下一页。不等竞争 closer／自身 grace。结束时重新取 L，只发布 s 的结果并清 active=s，**不得清理不同 req_seq 的新 pending**。epoch 已变／关闭时，未开始的动作终结为相应 closing；已经取得页／开始撤图的动作必须走自己的完成／交接，不能把它当取消后遗忘。

**NO_ELIGIBLE：** 在这次 owner snapshot 上没有可取得的已关联 private-cache 页，是该 pressure_generation 的终结结果；ring-only／全部在途／只有竞争 closer 都不能靠永久 pending 等待解决。同代反复 miss 返回既有结果且 O0，不自动递增 generation、不反复 napi_schedule。只有当前代终结ACK已经发布，且发生真正新的控制决定或库存变化，才能通过明确 REARM(reason, observed_generation) 在 L 内产生下一代再试；REARM 自己也计控制费用，active未ACK时只合并／拒绝rearm。

首版**没有自动 last-return/cache 通知**。若以后要加自动 rearm，需补已关联页进入 cache 时的分支／lookup、epoch发布、关闭竞态与额外运行费用；那是可选扩展，不是当前自治进展保证。当前方案也不要求每 alloc 维护 ACTIVE/IDLE 状态。

若ACK后的独立新代在 owner hook 已经过、NAPI complete 尚未结束时到来，应用 pending 必须保留。root交接确认：MISSED触发后继poll只用于enabled NAPI的普通完成分支，不把budget0、DISABLE、netpoll/其他特殊分支并入这个承诺；NAPI原文由该独占席/root提供。本席不能用MISSED替代应用层的pending/snapshot隔离，也不将这个条件分支提升成未来服务时限。budget0或gate关闭不得把pending当作已owner服务。

## 5. ctx、pool、NAPI 与 B/config 的寿命责任

**source anchor。** 拟议 ctx 由 source pool 生命周期显式拥有一个 anchor：从成功 attach 开始，直到 pool 最终 disconnect／释放前的组合回调确认 source 所有权结束。实际接入须保留既有 XDP disconnect 功能，不能偷换唯一回调或仅等驱动 free_rq；当前原文没有给任意新 ctx 自动注册这种 anchor 的实现。源页／原 frame 责任保护 pool 时，miss 可以沿 pool→ctx 的受控关系进入；无 frame 的控制查询／publisher 则须从受保护登记表取控制引用，不能先读裸指针再事后加 ref。

source anchor **不等于持 ctx ref 就 pin pool**。anchor撤销后，存活的记录／metadata callback 只能触 ctx、自己的受保护配置和不再解引用的 opaque 身份，不得再读 pool、channel 或 NAPI。pool 最后 release 后的代码同样遵守 PP:684–685。最终 ctx 释放要求 source anchor 已撤销、owner已解绑、所有请求已终结、控制发布／查询及必要索引读者结束、全部 K记录／retire任务的引用归零。

**owner attachment。** channel存活时，ctx 的 owner attachment 在 L 下保护 napi 指针与 source_epoch。新 kick 只允许 OWNER_OPEN；SOURCE_STOPPING 后禁止 kick。source gate关闭且已入场 hook 完成检查／必要 CPU同步之后，L 内改为 OWNER_DETACHED、清 owner/napi 指针。ctx 可以因此活过 channel；不能再靠 pool.p.napi 找回旧 owner。迟到 retire 通知只更新 ctx/结果，不 schedule 已关闭 owner。

**control/action 引用。** 活跃 snapshot、已分离页动作和异步 retire 都有明确 ctx/record责任；这些是有界 miss/control/record 状态，不是逐 TX use-token。引用只能从仍有效的 anchor 或受保护 lookup取得。源/B关闭在可阻塞控制上下文排空这些责任；不在 L、NAPI读侧或相互依赖的 closer 中等待。

**B/config anchor。** 持久映射及已 claim 的 closer 必须保证对应 device/domain/config 在最后合法 unmap前仍有效；get_device、RCU或ctx ref 单独不保证 DMA配置寿命。固定单pair内可用同一L线性化B控制门；若真实B关闭覆盖多个pool ctx，必须有全B配置gate及已入场publisher/closer的登记／排空，不能把单pair锁证明扩大成跨pool自动正确。

**最小正常读者约束。** 为避免把任意 control reader 变成额外页保留，首版可以规定：control/retire读者只读 record字段及opaque source键，不解引用page、不凭旧DMA地址发起工作；page仅由持原frame者或真正取下页的owner访问。正常同步撤图、从live索引撤下并断绝旧record触页后，可将页重新发布，metadata grace异步完成后返K。若实际 reader 仍可触页，就必须连页一起私人保管至其结束；不能只写CLOSED后立即放回。owner已关闭时，这种私人页只能转交安全source退出责任，不能由worker并发写alloc.cache。

## 6. source 关闭顺序

1. **先在L关闭source的请求／新持久准入／rearm gate。** 后续miss不再kick；B仍合法时可用O0。已有frame按原引用协议结束；不能因source owner停止就宣布它们已完成或提前卸在途B图。已有LIVE关联的读取仍受frame和既有关闭协议约束，不能在本轮偷偷增加逐TX独立ref。
2. **阻断新增owner动作并完成已有动作。** 保持channel/NAPI attachment有效直到同L内可能的kick已结束。再按驱动停RQ／NAPI流程和NAPI席核准的CPU同步／hook完成检查静默实际cache owner。已有代码有M:1373–1374、2765；napi_disable本身不等于设备DMA drain、poll所有尾部／后台callback或新ctx都已排空。不得持L等待owner。
3. **终结未服务请求并解绑owner。** 未开始snapshot给SOURCE_CLOSING，已开始动作完成撤图／退役交接后才算无主私人页。清napi指针，进入OWNER_DETACHED；关闭后的ACK/retire不再触channel。此时可沿原驱动路径关队列、free_rx_descs及page_pool_destroy。[M:1377–1384；2587–2605；2768–2776]
4. **pool正常延迟结束。** page_pool_destroy只开始／推进本身的release机制；旧frame/source页可能晚归还，逐页true-exit仍须完成其B关联关闭责任，再做原source release。PP:1028–1030的“allocation不能并发”必须覆盖新增cache owner；最终disconnect前仍保留ctx source anchor。对需要等待的真实退出，不先增release/clear_pp_info/put再留下裸页工作。[PP:673–698；1021–1058；1115–1134]
5. **最后释放ctx。** source anchor、全部record/metadata与控制引用都结束后再销毁ctx。不得把driver channel的kvfree或一次napi_disable当这一步。

这个source关闭合同不替代原RQ硬件停机要求；M:1190–1193只显示向core destroy转发，本席没有读到其设备/FW完成保证。

## 7. B 控制关闭与真实 B/config 销毁分别处理

**仅关闭持久映射组织：** 关闭该B/config的pressure/rearm和新持久准入门；待已入场control publishers/owner closer结束；未服务请求给B_CLOSING。共享source A继续工作，B设备仍合法时O0可以继续服务。活跃持久关联依原frame最后引用／逐页退出或其他已证明安全cut退役；不能因为gate关闭就卸仍被合法B帧使用的图。这个操作不声称B/config已可销毁。

**真实销毁／替换B/config：** 在上一步之外，还必须通过B自己的合法入口禁新DMA（包括O0），排完已入场CPU/NAPI发布者，完成B实际DMA完成或合法stop契约，然后枚举并关闭其余映射；配置寿命覆盖全部唯一unmap及必要metadata使用。M:3215–3216、2077–2086提供现有关闭调用次序；M:1760–1763仅转发core destroy，**真实设备stop/FW语义仍缺直接原文**。不能用napi_disable、synchronize_net、清desc或ctx计数代替该事实。

**并发 closer：** cache owner normal recall只取真正idle且成功claim的record；若B关闭已claim，owner直接skip/终结结果，不在NAPI等待B closer。B控制关闭则在合法控制上下文等先入场owner完成，之后处理剩余记录；record在未unmap前不能从全部可枚举路径消失。source true-exit与B关闭只允许一个unmapper，必要时把source页及pool inflight留给有界retire责任，退出读侧后推进，禁止source读者等待一个反过来等自己grace的B closer。

**普通cache idle回收不需要B全局排空。** 它的无在途证明来自实际cache成员资格和owner独占取得，沿前轮真实frame/bias协议；只需自己的映射、配置与CPU-reader责任。将B全局stop当每次idle回收前提会不必要地扩大合同与费用。

## 8. 手工正例与必要反例

**P1，有效条件序列。** source/B epochs均OPEN，p是实际private-cache已关联页。q在K满时走O0；持合法ctx寿命，在L内发布请求(s,g)后schedule。owner在合格位置以L取snapshot并清旧pending，解锁后独占摘p、成功claim、同步完成其B unmap和live索引删除；按record-only读者合同将p重新发布，ACK(s,UNMAP_DONE,retire_pending=1)。poll离开相关读侧后，异步metadata退休实际完成，发布RETIRED并返一个K槽。下一B需求是否使用该槽按实际准入顺序判；首帧q不倒算收益。期间source A没有停止，也不需要B全局排空。

**F1，应用pending失唤醒。** owner完成s的动作并发布终结ACK；独立控制者随后显式rearm，s+1在L内发布pending并kick；旧hook结束阶段再盲写pending=false，丢掉s+1。即使enabled普通完成分支的kernel MISSED确实带来下一poll，应用层也已无工作。修复是claim时只取／清旧snapshot，ACK只覆盖s；新pending只能由它自己的claim或关闭终结。

**F2，自grace循环。** hook位于RCU读侧，持p/record调用同步CPU grace，grace等待当前hook退出；hook等待grace返回。或B closer等包含source退出者的grace，而source读侧等B closer。修复是读侧不等；claim失败skip；必要retire异步且拥有ctx/record/私人页责任。UNMAP_DONE与RETIRED分开，不能在hook里为返K而强等。

**F3，裸pool/NAPI。** publisher保存pool.napi后暂停；source关闭、del NAPI、free channel；pool因为旧frame仍活着，publisher稍后从旧pool/napi指针schedule。或metadata callback在最后source release后读取pool来找ctx。pool仍活不保channel、ctx仍活不保pool；两条都不安全。修复是同L gate+kick线性化、关闭前静默owner、解绑后禁止NAPI，以及source anchor/合法ctx引用；release后不回读pool。

**F4，过早K复用。** ownerunmap后把slot立即交q复用；旧metadata reader/callback仍按旧generation写状态或删source/B索引，结果误作用新record。即便p已安全回cache，slot仍不能复用。修复是全部必需reader/metadata退休后才返K，并校验record/source/B epochs；序号不是实际退休的替代。

**F5，NO_ELIGIBLE自旋。** ring-only或全在途时，owner每次ACK无候选却留下同一pending，反复schedule等待slot；在没有新可触达事实时可无限烧控制CPU。修复是终结当前pressure_generation、同代miss合并/O0、明确REARM。没有自动rearm不保证后续缓存新候选会自行被看见；这是首版功能边界。

**F6，只有persistent gate却销毁B。** B关闭新持久准入，但O0仍向同B提交；控制面据napi_disable/CPU同步销毁domain，随后O0或旧accepted帧仍需DMA。修复是区分“持久组织关闭”和“真实B/config销毁”；后者必须有B自身所有新DMA禁入与真实完成/stop事实，且不应停共享source A。

## 9. 有界性、成本与仍缺合同

状态／存储可有界：每pair固定mailbox（至多一个active和一个pending合并批次）、一页owner动作batch、K个CREATING/LIVE/CLOSING/retire共预算记录和预留节点；没有frame等待队列。K与source正常已持页分别计，关联私有保管页≤K不推出总RAM净差≤K。若允许每个请求独立订阅／结果存档，必须另设有界容量／拒绝规则，不能偷偷增长控制队列。

有条件进展：合法新代已发布、owner实际获得合格服务、private cache有可取页、唯一撤图和metadata退休都终止，则该次请求可经ACK/UNMAP_DONE/RETIRED推进。没有wall-time保证；缺owner、ring-only、全在途或退休未完成可不返K，O0仍可完成有效工作。CPU须包括miss合并、短锁/schedule、显式rearm、扫描/lookup、unmap/retire、重建与供给反馈；没有成本就保持条件结果，不自动开发系统。

本席仍缺、不可伪称已取得的实际合同是：

- root最终接受的NAPI schedule/MISSED/complete/disable/RCU语义，以及新增hook的精确位置与完成屏障；本席不重复另一席阅读。
- proposed ctx source-anchor与现有XDP disconnect的实际组合、控制lookup/ref与最终释放实现；page_pool现成销毁不自动接管它。
- B/config anchor、跨pool时B全局control gate与已入场publisher/closer登记的具体实现；device普通ref不能替代domain/config寿命。
- 后端unmap在所选owner上下文所需的合法同步合同；真实B/RQ设备停止的core/FW完成证据。普通idle回收本身不因此额外要求全B drain。
- 全部record CPU读者的实际集合及“record-only/no-page/no-submit”是否成立；否则须保留私人页至相应读者结束，并重新计费。

这些缺项明确了未来接受实现所需证据，不是要求用户再给一般授权或保证痛点，也不授权本轮开发／实验。**本轮条件控制协议在此交付；后继以集成完整成本或具名代表性可运行证据为门槛，不自动继续拆协议、补代码或追core/FW。** 本文由Codex完成，未使用DeepSeek／Claude；没有以模型一致或文字协议代替原文与实际验证。
