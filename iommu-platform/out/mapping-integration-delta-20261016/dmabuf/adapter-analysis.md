# dynamic dma-buf 适配：字段、调用、锁和退出责任差项

2026-10-16。任务 OWNER-BINDING-INTEGRATION-DELTA-1 的独占子项；仅条件设计与原文核查，未写驱动、模型或测试。请求 gpt-6.1-sol/high；实际独立模型和强度元数据 unknown。共享研究 Codex-only，未用 DeepSeek/Claude。ARS-Codex 沿用已定的有界来源核查；source_verification_agent.md 只取证据、来源与不确定性纪律，不扩成新论文调研或完整 pipeline。

## 结论及接受边界

1. **合理 dma-buf 适配不必每包 export/attach，不必使空闲 cache 长期持有 page/pp 引用。** exporter 可预建 K 个可复用 PAGE_SIZE logical slot、按需懒建页对象，也可导出固定大小 span；importer 自有缓存、range 状态和控制面准备均允许。logical dma_buf 的 file 引用保持对象/私有状态活着，core 不会因此自动给 backing 增加 page 引用。双方 dynamic、MOVE_NOTIFY 打开、exporter 不另加长期 pin，才适用这一结论；“对象未销毁”不能替代“backing 已经安全撤图”。[h:200–203,290–324,470–488,559–598；c:638–680,767–772,940–960,1137–1151]
2. **标准 core 的确增加对象包装和控制面维护，但不能从原语数直接推出 V1 净赢。** 每 export 包含 anon inode/file 与 dma_buf 分配；每 attach 包含 GFP_KERNEL attachment、resv 锁下列表连接。对象可分组，reservation 可外供/共享；不要求用户 fd、每包 file 引用、独立分配每个私有字段或第四个 FIFO 槽。[c:543–568,624–693,714–727,903–934；h:513–545]
3. **NAPI 直接调用标准 map/unmap 不能成立；热命中绕开这些入口则未被该事实排除。** `might_sleep()` 在 map 的缓存快返之前，trylock 不能消除入口的睡眠合同；动态 importer 自有 cache 可在它自己的不睡 gate 下取 token。首次 miss 可走普通短 map/unmap 当前帧，再由 worker 准备持久项；或先 copy/defer。必须记 fallback 当前工作、worker、准备失败及 Q 成本。完整 fence 发布/查询也要满足 NAPI 合同；三份 core 源码不足以证明具体实现已经做到。[c:1112–1155,1189–1201,1217–1235；h:131–133,173–177,193–195,410–427]
4. **局部退出有三种强度，不能混为 core 免费支持或强制全池重映射。** DB-PAGE 可用对象通知精确退出一页；DB-SPAN-STD 用标准整对象 map 时可保守撤整份 SG，影响局限在选定 span；DB-SPAN-RANGE 允许补私有 range map/invalidate 和状态，只对指定页建/撤图。第三种是本轮允许的普通适配构造，不能称为现成标准 dma-buf partial API，也不能假称已在某驱动测得。[h:124–191,302–308,470–522；c:1264–1282]
5. **已证实的是可复用组织能力及条件差项，未证实 V1 独有部署能力或更低完整成本。** 同 source 退出付费接口、同 FIFO handle 和信息下，普通 O3 等价仅否定策略增量；不能据此否定接口。若 V1 只减少上列 shell/prepare/锁或 fence 成本，必须给同服务下具体布局与实际数据；也允许该差项最终为零。

## 来源定位与等级

`h:N` = `/home/xvhaocheng/work/iommu-platform/out/mapping-lifetime-design-20261016/prior/raw/include/linux/dma-buf.h:N`；`c:N` = 同目录 `drivers/dma-buf/dma-buf.c:N`；`rst:N` = 同目录 `Documentation/driver-api/dma-buf.rst:N`。均复用官方 v6.12 固定 commit `adc218676eef25575469234709c2d87185ca223a` 原文；本轮未重新下载、未声称新核远端字节。hash、读取区间与继承来源收据见 [source-manifest.json](source-manifest.json)。源码是接口行为的直接证据，非经验性能证据；医学 I–VII 研究等级不套用于内核源码。未涉及期刊、DOI、掠夺期刊或作者 COI；这些项目不适用，不伪造通过证明。

原文校正：c:1104–1107 的一般 map-duration pin 描述不能单独覆盖 dynamic 特例。h:470–486 明确允许动态通知避免 backing pin；c:1137–1151 的实际 pin 分支依赖 MOVE_NOTIFY；c:940–960 表明单方 dynamic 还会退化。须同时读这些边界。

## 三种具名适配及具体布局

以下 DB-* 是本分析明确定义的可审查构造名，不是已发表驱动名。没有下载或审计一个新 exporter；因此“可执行步骤”是满足 API 的调用/责任方案，非编译、运行或完整死锁证明。

| 适配 | export 单元与 backing | attachment / SG | reservation 与私有状态 | 可避开的收费 |
|---|---|---|---|---|
| DB-PAGE / DB-SLOT-K | K 个预 export/attach 的 PAGE_SIZE logical slots，K 不必等于源页总数 P；或仅实际需求页 worker lazy export。slot 绑定当前 owner/generation，旧 backing 正确 notify、drain、撤图后可在控制面重绑新页；未绑定/未准备 map 可拒绝 | 每选定 slot×B 一 attachment；每个已接纳 mapping 有 importer 所有的 SG。固定 B 时 K 个关系可反复使用，不按新帧或 generation 重建 wrapper；新 backing 仍须实际 map 准备 | exporter priv 是可重绑 slot；source→slot 索引和代际负责防 stale 命中；importer_priv 是 binding。resv 可独立或外供共享；保留 owner/config generation、状态、SG/DMA、worker/Q。active token 仅在合同需要时存在 | 不强制 P 个对象、不强制新 generation 再 export、不强制每包 file/page/pp 引用；slot 用尽可合法 fallback并计费 |
| DB-SPAN-STD | 一固定大小逻辑 span，含已加入的一批源页；span 大小在对象生命内不变，物理页可经动态协议更换 | 每 span×B 一个 attachment，标准 map 返回完整 span backing SG；缓存整份表。page→span/offset 索引由 adapter 保存 | 每 span 一 resv 或外供共享 resv；每页仍有 generation、退出/active 状态，以免只持整对象状态就混淆页的用途 | 相比 DB-PAGE 减少 shell/attach/resv 数；不强制聚合整个 pool，也不要求连续 PFN |
| DB-SPAN-RANGE | 固定大小 shell 表示受管 span；私有 slot/range 表登记当前 backing、generation 和洞；空 slot 不作为可访问已映射 backing | 每 span×B 预建 dynamic attachment；私有 `range_acquire/map/revoke` 管理逐页/连续段 DMA handles 和 range SG。标准 map 仅在确有完整 backing 时返回完整 SG，或明确拒绝；不能把带洞表假装标准完整 buffer map | resv 保护 shell/attachment/整对象移动；range gate、索引、每范围 handles、pending invalidations 由私有状态维护。局部源退出走双方私有 range 付费接口；整对象 move_notify 仍使该对象所有缓存失效 | 避免为了移除一页而必全池 map/unmap；代价是实现该 range 协议、映射配对和同步，而非 core 凭空提供 |

exporter `pin/unpin` 回调成对提供，才被 core 判作 dynamic；正常双方 dynamic、MOVE_NOTIFY 开的 cache 路径不调用它们。若显式 pin 必须阻止真实 backing 退役，不能把 pin callback 写成无条件成功的空操作来获得无 pin 标签。[h:86–122,581–598；c:638–643,1137–1151]

聚合边界：标准 `size` 不变，所以 pool 扩缩不能直接改 dma_buf.size；可固定 slot 容量、分 span 加新对象、或在完整整对象移动后更换 backing。标准整表 map 不接受 offset/length；不能依赖调用期间偷偷改变 `attachment.priv` 选择子页并将返回的单页表称作完整 span 映射。若选择私有 range 接口，要按 DB-SPAN-RANGE 明示新增函数和成本。core 私有指针本身允许这些状态，不证明所有扩展都已实现。[h:179–191,302–308,499–545]

## 事件调用表：控制面、NAPI、退出与关闭

### 1. source join / prepare（可睡上下文）

1. 源 owner 先建立稳定 owner-entry，并使真正 release 钩子能找到它；此时 backing 由 source 正常拥有，不能先向 B 发布一条无生命周期保护的 PFN。
2. DB-SLOT-K 在控制面预先对 K 个固定 PAGE_SIZE logical slots 用 `dma_buf_export(exp_info)`，后续重绑不再 export；DB-PAGE 也可仅按实际需求 lazy export，聚合方案在 span 创建时做一次。`ops` 至少 map/unmap/release，另提供合法 pin/unpin；`priv` 指向 owner-entry/span。保持 logical file 引用；只内核使用不调用 dma_buf_fd，不建用户 fd。export 自带 anon inode/file、dma_buf、内部或外供 reservation、配置相关统计和调试工作。[c:632–691]
3. 对已知 B 调 `dma_buf_dynamic_attach(dmabuf,B.dev,&importer_ops,binding)`；调用时不持 resv。attach 验 DMA 约束，建立 attachment；失败则不得发布 cache。目标引用、配置 lifetime 和 prepare 取消必须另由 adapter 担任。[c:824–848,903–934；h:46–72]
4. 预留 SG/索引/worker item/fence 所需元数据；选择现在预 map 或后续 worker。预 map 需 resv、处理 producer/move write fences、调用 map 并取得 SG，再以 range gate 发布 LIVE cache。SG 所有权在 importer，销毁时必须归还对应 unmap；不将同一 SG 多次 map 返回。[h:150–177；c:1112–1155]
5. 可只对实际 target/实际接纳范围准备，不假设所有 pool×target 已预 map。K 槽方案预留 source→slot 索引，重绑先关旧 gate、notify/drain、归还旧 SG、清旧索引，再以新 owner/generation 实际 map 并发布；未准备或无空槽当前帧走合法 fallback，不能让 K 槽等价于免费预 map 全 P 页。DB-PAGE 对象和 attachment 也可按需由 worker 懒建：代价是首次帧走 fallback，而非强迫所有页预付。

### 2. 首次 miss（NAPI）

- 查 owner/config generation 与 cache gate。若无 LIVE binding，不能在 NAPI 直接调用 export、attach、标准 map/unmap；它们的睡眠/分配合同未获 atomic 保证。
- 允许普通短 map 当前源页、sync、TX，CQ 对应短 unmap；其可用性以冻结 O0 原路径/实际 DMA backend 为前提，本席没有重新验证 DMA backend。用同三槽 FIFO handle 表示 SHORT_MODE，不能让持久 worker 误卸这次短图。
- 同时仅投递一个有界 prepare 请求。worker 在可睡上下文检查 owner 和 target 尚存、代际未变，建对象/attachment/map，然后再发布 cache。worker 排队期间 source 退出：若未取得源生命周期保护就取消，不可重用裸 PFN；若已取得受控保护则转 Q，所有延迟页/索引均计费。
- copy/defer 也是合法回退，但需计目标池 C、复制带宽、额外等待与有效完成服务；“worker 最终准备好”不能代替当前帧已服务。若服务不允许这些回退，则该未准备 miss 不满足此部署合同。
- 标准 map 即使内部可缓存快返也先执行 might_sleep；先试 resv 锁再调用它仍不改变这项事实。[c:1118–1135]

### 3. hot hit / TX 接受、失败和 CQ

一般版本中，适配器可在自有短 spin/try gate 下验证 owner generation、target/config generation 和 LIVE 状态，取得 active token；它随后使用已缓存的合法 DMA address、执行每次必要 sync、接受 TX，用相同 FIFO binding handle 归还 token。TX 未接受时回滚 token；CQ 完成后先归 token，再把 frame 还源。没有新增 payload pin 或第 4 槽的必要。

共同窄版允许固定 B、K 个已准备槽、无 idle eviction，并直接用既有 frame/pp-ref 保护正在 TX 的源 backing：若已证明持有 frame 时真实 source release 不可达、target close 能统一停新请求并 drain、worker 不在活 frame 间重绑，则 hot/CQ 可没有新增 per-TX token、计数或额外锁；slot adapter 同样享有该优化。源退出发生时已无这类活 frame，可按 owner→slot 索引撤图。这个结论依赖具体调用链与共同合同，不能泛化到独立 idle eviction、活帧 migration 或外部 writer；不是 core 自带自动保护。仅 slot 查找/代际、必要 sync、FIFO handle 和控制面退出责任仍存在。

但是 **只检查地址 cache 不够**：dynamic importer 必须遵守 write fences；h:419–421 用 should 要求为 move_notify 不能立即禁用的访问设置 fences。若共同窄版证明 callback 只在无活 frame 时可达、能立即使旧 cache 不可再用，则不能据该条强收固定 per-TX fence 发布；此时须保留来源绑定的无并发/完成证明。[h:415–427] 可选择：(a) 预留且经核实不分配的 reservation/fence 发布路径，NAPI 只 trylock，冲突即 fallback；(b) producer-ready/代际 gate 加预安装的有界 in-flight fence/cohort，由 exporter 与 importer 的同一私有合同限制写入者和 backing move。后一选择必须核 fence 的使用类型、信号条件、源再次写入的合法顺序，不能让一个“cache 存在即永不信号”的 fence 阻塞正常 recycle；不得假设任意外部 writer 没有新 write fence。**本轮没有 dma-resv 实现或具体 exporter/网络适配的原文，故 a/b 的完整非睡实现与成本均 unknown。** 它们是明确待验接口，不是现成已通过路径；trylock 失败也不能无条件 short-map：仍须遵守对应 producer/close gate，遇真实撤销只能取消/defer/copy到允许的目标。

### 4. source 真退出 / notify / 局部退出

1. source 付费 release 钩子先在共同 gate 标 RETIRING，禁止新 token 和 stale prepare 发布；确保短路径和持久路径的已接受 TX 都有完成责任。正常 recycle 只改变数据/use generation，若规定权限和 source 所有权不变可继续 cache；不能把正常 recycle 误当 backing free。
2. 可睡 exporter worker 取得 resv，DB-PAGE 调 dma_buf_move_notify；DB-SPAN-STD 只在所选 span 中通知所有 attachments；core 会遍历 dynamic attachment，callback 在 resv 持有状态调用。callback 可短暂取得私有 gate 关 cache，标旧映射失效；不要在持 gate 时等 CQ。[h:476–486；c:1272–1280]
3. 若在途访问不能立刻 disable，callback 需有有效 fence/完成责任。不要把 move_notify 的 void 返回当作 drain 或 IOMMU 撤图完成。worker 应释放 resv 后等待需要 resv 才能前进的任务；CQ/token 归还不能依赖当前 worker 持有的睡锁。精确锁序须被具体适配固定。
4. 在 token=0（窄版由既有 frame 保护证明无活用）/设备 drain 且 producer/move fence 满足后，由每 binding 唯一 UNMAPPING winner 在可睡上下文拿 resv 调 dma_buf_unmap_attachment；detach 不能代替 importer 自有 cache 的显式 unmap，因为该表不在 core attach->sgt。[h:173–177；c:1015–1036,1217–1235]
5. DB-SPAN-STD 整份 SG 要按原 map 配对归还，再以合法完整 backing 建新表；不能从 core SG unmap 入口推断有按页 unmap。DB-SPAN-RANGE 在相同 source 范围付费接口下只关闭受影响 slot，等待这些 token，按私有 range map 的准确配对方式撤其图、清索引；其余 ranges 可继续，但整对象 move_notify 仍保守关闭全部 ranges。若底层只提供整表 unmap，则分成独立可配对 handles，或退回整表，不能对一个 dma_map_sg_table 结果任意局部 dma_unmap_page。
6. backend 所需 invalidation 完成/隔离债务处理后，source 才能清 backing、真正归还页；logical shell 可保留为已关闭/无 backing 元数据，未来标准 map 需报错或提供新的合法 backing。DB-PAGE 也可此时 detach/put 撤销该 object，pool/span 若仍留 shell 不能保留过期 page 指针。

NAPI 源退出不能睡时，此序列要求 **source 保留该退出页于 Q，由 worker 完成撤图后继续原释放责任**；不是“先 free 再补 unmap”。Q 的页、附加引用、pool/control 引用和排队时间按真实物理集合计；有任何 page ref/pp_ref 若改变正常 recycle 就单独归因。同步无需 Q 的版本也要证明确切调用链和 backend；本轮不证明。

### 5. cache 淘汰与 target close

淘汰只关闭相应 cache gate，等其 token 归零，在 worker 中卸图；source 未真退出时可继续正常拥有页，不把卸图算物理页减少。target close 先标 target/config CLOSING，堵住 hot、SHORT_MODE 和 prepare 新接受；取消/接管 worker，等待已接受 DMA 的设备语义 drain，逐 cache unique-unmapper 撤图，再 detach 每 attachment，最后释放 dmabuf/file 及 target/config 引用。detach 调用不得已持 resv，因为 core 会再取锁。[c:834–848,1015–1036]

source exit 与 target close 相遇时，状态 winner 持唯一 unmap 责任，另一方等待 completion，不第二次 unmap。不能先销毁 attachment/device/config 后让 worker 使用它，也不能把 dma_buf_put 当 source free 的唯一触发器，否则 B 的对象引用可能把 source 退出关口无限推迟。c:1015–1036 的 core detach 只自动处理 attach->sgt 的 core cache；本设计双方 dynamic + importer cache 正常不使用该槽。

## 费用差项：必需与可规避要分开

| 项 | core/合同真正必需 | 普通布局可规避项 | 必须另核的完整成本 |
|---|---|---|---|
| object shell | 使用 dma_buf_export 即有 anon inode/file、dma_buf、有效 resv/ops/priv、引用责任 | 非每包；span 聚合；外供 resv；不装用户 fd；复用已关闭固定大小 shell | 对象个数、配置统计、模块引用、内存 bytes、分配失败/拆除时间 unknown |
| attachment | 每选定对象与 B 关系一份 attachment；resv 列表维护；双方 dynamic 判据 | 非每包；目标懒建/prepare；聚合减少关系数 | fan-out、设备约束验证、config lifetime unknown |
| SG 与 map | 每次成功标准 map 有独占可归还的 SG；与原入口准确配对 unmap | importer cache；预留/arena；不用每帧新表；range 适配可避整表重建 | IOVA/PTE、SG段数、sync、invalidations、map失败和 Q unknown |
| 锁与 fence | 标准 map/unmap/notify 的 resv 合同；在途访问/写依赖责任 | hot 自有 cache/gate；分 span；trylock+合法 fallback；共享/分片 lock | NAPI fence路由、争用、wait/worker wakeup unknown |
| source lifetime | 真释放前旧 B 图及 DMA 必安全结束，owner-entry 不提前消失 | 不强制 idle page pin；可以仅留 logical 元数据 | source release hook、Q/对象延期、pool寿命及分配反馈 unknown |
| FIFO | frame、binding/SHORT_MODE、完成顺序责任；token依合同可省 | 同已付费 handle；不增第四槽 | handle bits/layout、lookup命中、cacheline bytes unknown |

call-cost-table.json 给各阶段符号调用数/影响集合；一般 token 版本和固定 B+K 槽无 idle eviction 的 frame-protected 窄版分别计；不是周期、吞吐或实测。物理页只按 S/P/C/Q 并集计，不因一张额外 SG 或 mapping 再算一份 payload；SG/对象/PTE 元数据、IOVA、CPU 与带宽分别列。

## 给 root 的最小未核点

- 首要：具体 importer 的不睡 fence 发布/查询及 gate 锁序，含 worker 与 CQ 不自等待；不能用 might_sleep 一句结束判别。
- 其次：source 真退出钩子的原上下文能否同步完成，否则 Q 容量/责任；对象 shell 和 pool/private state 何时可独立释放。
- 如果 root 要把 DB-SPAN-RANGE 当可部署普通强对照：只需核所选范围接口如何配对页/SG map/unmap、部分 slot 失效及 device/config drain，无需扩成全内核或新实验。
- 只有上述可执行义务固定后再比较实际对象数、map粒度、元数据 bytes、CPU/不可回收内存；目前数量/周期/sizeof/负载/worker时延均 unknown。本轮没有必要下载具体官方 adapter；若后续需要，只选一个并先协调 root。

这里没有支持“dynamic dma-buf 必须长期 pin backing”“每包必 export”“core 无 range 所以无法局部适配”或“热 hit 必睡”的证据；也没有支持“普通适配已同成本完成全部 V1 合同”的证据。保留这两个边界，避免由功能可组合性跳到性能等价或由缺测跳到架构创新。
