# MAP-RECYCLE-RECLAIM-1：source cut 定向核查

2026-10-16 JST。Codex-only；请求 gpt-6.1-sol / xhigh，实际独立运行 metadata 为 unknown。ARS 按已选的机制设计／定向来源核查使用，仅核保存的 Linux v6.12 原文和手工事件偏序。无驱动实现、状态／性能模型、测试或设备运行。下文的“可以”均是满足列明条件的设计可达性，不是已测收益或已验证的并发实现。

## 1. 给 root 的结论

**存在不新增每 TX 独立 token 的安全回收窗口：最后 page_pool 引用已归还且页通过普通可回收检查之后，尚未发布到 alloc.cache/ring；另一窗口是原分配 owner 在已有非并发保护下，从自己的 private alloc.cache 真正取下一个 idle 页之后。** 前者能在该页下一次正常归还时撤 B 图；后者还能处理“已在缓存，未来不再自然 return”的冷关联。两者都可保留源 A 图、pp_info 和 source 代际 g，不必让页真正退出 page_pool。[PP:736–773；818–829；864–876；H:245–308；RX:274–302]

本席按 root 最新收窄，建议保留的最小强版本是 **R-cache：pressure 由原 allocation owner 显式服务，仅从已有 private alloc.cache 取下候选，独占页期间撤 B，完成必要页相关读者排空后重新发布**。首版不消费、扫描或重插 ring，不增加每 alloc 的 ACTIVE/IDLE 原子状态；但必须新增真实 owner 服务点／唤醒与扫描工作，不能说现有驱动已经有这条 control pump。普通 R/O3 获得同一接口并完整计费。

R-cache 的具体新增能力是：**不等待候选页再被分配、再产生包、再自然 return 或 source true exit，就能释放一个已缓存页的持久 B 关联，同时保留对其他页继续跨 recycle 复用的选择。** 这只相对旧 V1-N/K 的无独立容量回收合同增加能力；O0 本来就可在逐 TX 的 CQ 后只卸 B 短图，A 页仍留源池，所以“只卸 B、保留 A”本身不是新能力。[XD:673–700；PP:764–773] 有限 K 不再只靠 source 所有权结束轮转，但它只在存在可取下的已关联 cache 页、owner 实际获服务、B 撤图和 metadata 退役完成时推进；ring-only、全部在途、driver bias 未归还、owner 未运行均无关联回收进展保证。满额继续 O0，不能把 K 不进展说成应用不能完成或工作流死锁。128 是此版 private cache 的物理上限，不是 K、ring 容量或整个 pool 的页预算。[PP:422–436；722–733；T:37–55；189–215]

## 2. 冻结范围与证据等级

沿用保存源码的 v6.12 mlx5e cyclic / linear / native-XDP / order0 / 非 XSK，单次 FIB→固定 B 或本机 UDP；一轮一页一包的配置来源沿用前轮 source-opportunity.md §1，未重核其他配置文件。源实例／代际 g、B/config 身份与 T 持续专用池授权条件不变；没有由 dma_unmap 名称新增逐包撤权或隔离要求。S 合同另列，不能拿本 cut 替代其授权证明。

primary evidence 是保存的官方源码字节；本文只证调用位置、引用门和条件偏序。前轮 source-opportunity.md 与 close-and-capacity-contract.md 只用于导航／沿用已冻结接口，不能当运行数据。原始路径、版本、hash 和本轮阅读范围见 source-manifest.json。没有现成工作负载频率、后端 unmap 服务时间、CPU 成本或 K 足够性的证据。

别名均相对保存 raw 根目录：

| 别名 | 原文件 |
| --- | --- |
| PP | net/core/page_pool.c |
| H | include/net/page_pool/helpers.h |
| T | include/net/page_pool/types.h |
| RX | drivers/net/ethernet/mellanox/mlx5/core/en_rx.c |
| XD | drivers/net/ethernet/mellanox/mlx5/core/en/xdp.c |

## 3. 最后 pp 归还到底证明什么

1. H:227–238 定义 pp_ref 是以后通过 page_pool API 归还的 outstanding 引用；driver 可先设置 bias，再一次性归还未用部分。RX:281–289 每次 page_pool alloc 后仍执行 page_pool_fragment_page(page, PAGE_SIZE / 64)。**一页一包没有删除这个 bias。**
2. 成功 redirect 后 RX:1838–1841 给已交出的 frame 计一个 frags；本机线性 skb 也在 RX:1701–1708 标记回收并计 frags。driver 后续在 RX:298–302 归还 bias − frags；只有该 unref 返回逻辑零，才调用 put_unrefed。RX:321–335、374–380 还要求相应 fragment/WQE 真的能释放。正常 refill 先 free 本小批旧 WQE，再 alloc 新 WQE；不能在旧 WQE 尚未完成时任意挪页。[RX:457–478；893–911；1855–1856]
3. B 普通 CQ 按实际 WQE／packet 回收：XD:783–803 调用 desc 回收并 flush frame bulk，XD:673–700 先执行 stock 短图 unmap，再归还 frame。**单个 CQE、doorbell、redirect 返回成功或 desc 已入 FIFO 均不是最终 pp return。** 对候选持久模式，这条链需改成持久图不逐 CQ 卸图、只按原 frame 协议归还；本轮没有改代码或声称它已存在。
4. PP bulk 在 PP:869–873 先要求 page_pool_is_last_ref；driver bias 先归零的另一顺序从 RX:301–302 直接进 put_unrefed。两者共同汇合于 PP:750–773 的回收判定，不能只在 bulk 中加 hook 而漏 driver 最后 bias 路径。[H:305–323；PP:818–838]
5. 最后 pp 门是事件结论，不是“计数字段等于零”。H:250–285 特意把完成后的字段保留／恢复为 1。**pp_ref==1 与普通 page_ref==1 都不能让外部扫描者断言页在 idle cache。** 活跃单个 consumer 也可能剩逻辑一个 pp 引用；而 driver 下一轮会重新设 bias。[H:245–308；RX:281–289]
6. 对普通 page-backed 页，还要满足 page_ref_count==1 且非 pfmemalloc，才走正常 recycle；否则进入 true exit。last-pp 门不等于任意非 page_pool CPU 引用也已经全部消失。[PP:736–790]

手工偏序 B-bias（令 b=PAGE_SIZE/64，固定本轮只有一个交付 consumer）：

| 合法顺序 | pp 引用含义 | 可否按本 cut 撤 B |
| --- | --- | --- |
| B frame 完成并真正 return，driver bias 尚在 | b→b−1；driver 仍持未用 bias | 此刻不行 |
| 随后 driver 归还 b−1 | 最后 pp 用户结束，字段按 H 恢复为 1 | 通过 recycle 检查后、发布前可以 |
| 反过来，driver 先归还 b−1 | 留一个 consumer；字段可为 1 | 此刻不行 |
| 随后 B 真正 return | 最后 pp 用户结束；字段仍可为 1 | 通过检查后、发布前可以 |

这是符号引用账，不是状态模型执行。错误停止清理的设备排空仍须原驱动合同；XD:819–840 的 NULL-CQE 清残余不能被当成正常 CQ 成功或完整硬件停止证明。

## 4. 三个版本的最强边界

| 版本 | 有效 cut 与可执行动作 | 必须新增／计费 | 能释放什么；主要限制 |
| --- | --- | --- | --- |
| R-return：只在最后 return 看 pressure | 共用 put_unrefed/put_page 汇合点；可回收检查后、A sync 和 cache/ring 发布前，唯一 claim B record、同步撤 B，再按原路径发布页 | pressure 发布／读取、关联查找、唯一 closer、B unmap、CPU／metadata 退役；不可持索引锁等待后端 | 下一次实际经过这个门的已关联页；对已经冷存 cache/ring 且不再 return 的页没有保证 |
| **R-cache：保留版本** | 原 allocation owner 在已有非并发保护下扫描 private cache，真正摘下已关联页；页在私人保管期间撤 B；完成必要页读者排空后按 owner 保护重新发布 | 显式 owner 服务／唤醒、最多128项 cache 检查、索引查找、摘下／恢复、唯一 closer、unmap与退役；不是免费现有 hook | 已在 private cache 的冷关联，无需再次 return；不覆盖 ring-only、在途或未 drain bias 的页，无 wall-time 保证 |
| R-external：外部线程扫 idle 候选 | 仅凭 pp_ref、page_ref、LIVE 或“曾最后 return”都不够；必须向 owner 交接并实际取得页，或新增与每次 alloc 共同仲裁的 IDLE/ACTIVE/EVICTING 协议 | 若直接外扫：每 alloc 查找／原子转移、return 发布、CAS失败处理、页暂存／交接、ABA／record寿命、关闭费用；若委托 owner，就退化为 R-cache 的服务接口 | 可以另行设计更宽范围；现有原语不能直接证明安全；本轮不选、不实施 |

R-return 的合适位置必须早于 PP:769–773，因为成功 direct return 在 PP:770 直接结束；远端／非 direct 则要早于 PP:826 的 ring publication 或 PP:883–892 的 bulk producer。不能等发布完再假定仍拥有页，也不能只钩真正退出 PP:673–698。[PP:700–733；764–773；818–829；864–902]

## 5. R-cache 的最小协议与不变式

以下是可审查的接口条件，不是已验证实现：

**请求。** K 满或其他已定压力条件只发布 reclaim-needed／epoch 请求并让当前 B 需求走合法 O0；一次 miss 不能倒填成已接纳。请求保留到实际 retire 完成或按明示规则撤销。需要把工作交到拥有这个 pool alloc.cache 的 owner；没有 owner 调度时，状态位本身没有进展。[T:189–215；PP:427]

root 另行直接核到现成 NAPI schedule 入口与 poll 的候选插入位置：en_main.c:2646–2651、932；en_txrx.c:124–198、254。此为 root 的定向原文交接，本席未扩读这些文件。它支持投影到 positive-budget poll 的 post_wqes 之前，budget==0 的提前跳出不算已服务；**仍须新增 pending、生命周期、失唤醒和 reclaim hook 协议**，不是已有完整 reclaim pump，也不构成有界调度证明。该 poll 在 RCU 读侧内运行，hook 不能同步等待包含自身的 CPU grace；必要 metadata 退休／返 K 应在离开读侧后的合法机制完成。

**取得页。** owner 在正常 allocation 非并发保护中读取并扫描自己的 cache，找到确实仍在数组内且有 B 关联的页后，真正移出容器，建立单一私人保管责任。PP:730–731 的缓存不变式给出普通 page_ref==1；在不滥用归还 API 的合同中，该容器成员资格和取得动作证明没有合法 source frame／driver bias 留在外面。远端 CPU 可是 return producer，但不能有第二个并发 allocation/cache 扫描者。[PP:722–733；795–815；T:189–215]

当前单 pool 的 alloc.cache 是内嵌的一份 private cache，并非每个 CPU 都有一份可直接扫描的空闲数组。[T:189–215] owner 的保护身份也不能简化成永久固定 CPU：PP:809–815 还检查当前 NAPI list_owner。真正远端／不满足 local 判定的 return 走 ring；不能给本地 reclaimer 免费算作已掌握“其他 CPU 的缓存”。需要跨 owner 的工作必须经各自服务／交接，不能直接并发读改 cache。

**关图。** 在不持 source/B 索引锁的情况下取得唯一 closer 责任，关闭这个 record 的新使用门，按正确的持久映射参数撤 B；A 的 dma_addr、pp_info、pp_ref 不能被当作 B storage 覆盖。页仍属于同一 source g，不执行 PP:687 的 source release 计账，也不 clear pp_info/put_page。B/config 稳定且后端能在该调用上下文同步完成，是选定同步版本的必要条件；本轮没有测其时限。若做不到，必须另明确私人页暂存与完成后交回 owner 的责任；不能先把页重新放回供 alloc。[PP:648–698；旧 close-and-capacity-contract.md]

**CPU／metadata。** 无 frame 只排除了合法 frame 数据路径的新使用，不自动排除 control reader、旧索引读者或正在关闭 B 的线程。TX 必须在持原 frame 期间完成 DMA 地址读取，并在转交／最终归还之后不再解引用关联；CQ 持久 mode 只用原 frame 责任，归还后不再读 record。control reader 若仍能使用旧地址发起 DMA，或 dereference source page，则必须先关闭入口并实际排空；单独 RCU 标记或写一个 CLOSED 不能取消已加载的指针。唯一 closer 与 B关闭竞争沿用旧合同，不能在 softirq/读侧中等待一个正反向等自己的 closer。

**重新发布与返槽分开。** 至少满足 B unmap／所需同步完成、旧 record 的页相关读者已排空、旧 record 此后不再解引用该 page，才允许 owner 把页重新发布到自己的 cache。纯 metadata reader 可以在已证明不再触页／发起 DMA 的退休态继续结束；但 K slot 仍要等所有必须的 CPU reader／metadata 退役后才返回可复用库存，避免 record 重用／generation ABA。若旧读者仍可触页，必须连页一起继续私人保管。不存在“unmap 一返回就一定立即免费一个 K slot”的无条件结论。

**再次接纳。** 日后同一 g 再次送 B，可用空 slot 建新关联，也可因 K 满走 O0；这是同 source g 的新 mapping episode。不能把原 record 已关闭的地址继续命中。若 owner 扫描／摘下／重新发布改变实际出栈顺序或暂停了 refill，源页序列、替代分配和后续 B 身份需按该策略导出；不抄 O0 的固定 PFN 轨迹。

这套证明不需要每 TX token，也不要求每 alloc 增一项 ACTIVE/IDLE 原子状态，因为所有撤图者先从原 owner 取得了容器中实际 idle 的页。但它仍有每个持久 TX 的关联查找／ready/DMA读取与必要 sync，有 pressure 时的 owner 控制成本，有关闭与 metadata 状态；不能称零 metadata、零同步或现成无锁实现。R/O3 在同信息／接口上也可执行这一组织。

## 6. 来源绑定的决定性正例与相邻反例

**正例 C-cache：R-return 没有事件，R-cache 可以回收。** 取 K 至少一个已占 slot，某已关联页 p 正常完成 B frame 和 driver bias，普通回收条件成立；在没有 pressure 时经 direct 分支进入 source alloc.cache。p 上面还有足够的其他正常供给页，或者已有 RX WQE 正在处理后续输入；不把页强制立即再分配。随后另一已经合法取得 source frame 的 q 实际请求固定 B，遇 K 满，O0 完成并置 pressure。原 owner 的新服务点在下一次正常受保护执行中扫描 private cache，**此时 p 仍是实际 cache 成员**，故可摘下 p、同步撤 B，并按第5节完成退休／重新发布。p 不必再次自然 return，source g 也未结束；一个 K slot 在必要 metadata 退役后可供后续 B 需求接纳。[PP:722–773；422–436；RX:457–478]

这只要求相应容器状态与事件偏序，未设人为流量 gap、未来再用次数或后端时间。可选择的新服务位置包括原 owner 执行 refill、旧 WQE free 之后而新 alloc 之前的 RX:471→472 边界；**该服务点是拟增接口，stock 没有调用它。** 当前 q 已回退，不能倒算其收益；后续是否有值得接纳的需求仍须真实记录。

**反例 C-no-owner：同一个冷 cache 页，无实际 owner 服务。** pressure 已置，p 始终在 cache 较低处；正常小批返回／分配只使用其上方足够的热页，或 RX owner 暂时根本不再执行。R-return 从未再看到 p；R-cache 的服务状态位也没有执行者。K 仍满。cache 的栈式取页 PP:430 和先 free 小批再 alloc 的 RX:471–472 允许这种条件序列；不支持为任一真实实例填频率或时长。即使自然 alloc 最终可能触及 p，也不能把“可能”当进展界。

**反例 C-ring-only：source 已 idle，但首版看不到。** K 个已关联页全部经远端／非 direct return 合法进入 ring，private cache 没有已关联候选。RX 以其他热页 direct-return→cache→alloc 维持供给，cache 在需要取页时一直非空，PP:432–433 的 ring refill 不被调用。R-cache 可实际运行很多次仍无可回收候选；R-return 也没有这些冷页的第二次 return。源池有 idle 页并不等于所选 private-cache 接口可触达它们。[PP:370–419；422–436；795–815；883–902]

这里只证明正常不进展可能性。首版不为解决它而私自消费／重插 ring：那会新增 sole-consumer 扩展、扫描与发布同步，且可能因并发 producer 填满 ring 导致源页退出或额外暂存；它们需另立完整服务／资源合同。128 cache 上限不消除 ring-only 反例。

**反例 C-active/local：没有安全 idle 页。** 已关联 p 的这一轮改走本机 UDP，但 skb 仍由正常 consumer 持有；或者 p 已送 B 而尚无真正 CQ/frame flush；或者 B 已 return 但 driver bias 未 drain。混合目的不让 p 的旧 B 图自动变得可关闭。last-pp 门尚未完成，p 不在合法 cache 中；owner 只能略过并使新需求继续 O0，直到真实 consumer／bias 结束。不能强迫完成、把 CPU dequeue 时刻冒充最后页归还、或用 page_ref==1 旁路这个门。[RX:1701–1708；274–302；XD:673–700；783–803；H:245–308]

**反例 C-external-race：外扫的历史 idle 记录失效。** t0 最后 return 将 p 发布 cache；t1 外部扫描者读到历史 IDLE／pp_ref==1；t2 原 owner 从 PP:430 取 p 并在 RX:285 重设 bias，将下一 frame 合法送 B；t3 外部扫描者按旧观察卸 B 图。t3 可以与新 B 使用冲突。record 仍 LIVE、普通 page_ref==1、曾经完成一次 CQ都不能阻止这一序列。

若真的允许外部直接回收，最小补救须让 return 的 IDLE 发布、每次 alloc 的 IDLE→ACTIVE 与 reclaimer 的 IDLE→EVICTING 进行同一个原子仲裁；reclaimer 获胜时，alloc 不得把该页交 driver／重设可使用状态，必须暂存或交回 owner，且解决记录重用和失败路径。这个新增费用按每 source allocation round 发生；在固定一页一包切面，频率可接近逐包。它不是每 TX token 的同名物，但也不能把它换名后说免费。本轮不选择该扩展。

## 7. 有限 K 到底能保证什么

| 命题 | 本轮判定 |
| --- | --- |
| K 对 CREATING/LIVE/CLOSING/退休记录总数形成上界 | 可作为预留库存设计约束；必须所有状态共计 K，未退役不能返槽 |
| private cache 只要有一个可取得的已关联页，就无须等它未来再 return | R-cache 支持；前提是 owner 真正执行、唯一 close 成功及读者退役 |
| pressure 之后必然在有限 wall-time 释放 slot | 不成立；owner 调度、backend unmap、CPU grace 都无本轮有界事实 |
| 源池存在 idle 页就必能回收 | 不成立；页可能只在 ring，首版不可触达 |
| 所有 K 关联都在途／仅有 driver bias，仍能安全轮转 | 不成立；须等待真正 last-ref 或另付更强 use/close 协议 |
| 一个后续 B miss 必定得到刚释放的 slot | 不成立；需要实际退役先于该需求，且无其他合法准入先占；首个触发 miss 已 O0 |
| 回收 B 图自然减少 payload RAM | 不成立；p 原本就在 S 中被源池持有，R-cache 可只减少 B 的 PTE/IOVA/关联占用；暂存／refill反馈还可能增加其他供给需求 |

可承诺的条件服务步进展是：若 pressure 保留，某服务时刻 private cache 确有可摘的已关联页，owner 获排他服务，唯一撤图和必要退休都终止，则至少一个占用 slot 能退役；无需该页再次 return。若要求“及时”到某具体下一次 B 接纳之前，必须额外有这些服务／退役先于该接纳的真实顺序事实，不能从 cache 大小或 K 推出时间。

## 8. 能保留的研究主张和必须停止的版本

可保留的具体能力：正常 source 所有权未结束时，利用 owner 已持有的 idle 容器责任，在有限预留 K 下关闭冷的持久 B 关联，保留无需每 TX 另计 token、其他关联可继续跨 recycle 复用的窄整合，并付控制／重建成本。它补上旧 V1-N/K 仅 source true-exit 的容量死角，不是重述旧退出 hook；也不把 O0 已有的只卸 B、保留 A 当新能力。

必须停止的版本：仅最后 return 看 pressure 就保证 K 及时轮转；外部靠 pp_ref==1／历史 idle 无同步撤图；只要一页一包即可忽略 bias；unmap 后立即复用 K record而不排 CPU 读者；128 等于 pool 或 K 预算；B 映射回收本身就是 payload RAM 节省。

上述同一 cut、owner pump 和 K/退休合同也能给普通 R/O3。**本席未证明独占新机制、低完整 CPU 成本、净收益、最接近前作覆盖或论文主贡献。** 它们由 root 与独立 closest-reclaim 席按各自范围裁决；普通构造能做同一动作只约束这个动作的排他增量，不能推出接口无价值或整族问题已解决。未知成本不自动触发驱动开发。

AI 参与：本文由 Codex 对保存原文作定向阅读和条件分析，未使用 DeepSeek／Claude。人工序列不是实验；未使用模型一致性代替原文或执行证据。
