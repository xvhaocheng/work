# 多 DMA 共享 IOMMU 的机制设计与下游交接（round03）

**stage**: 90
**date**: 2026-10-09
**范围**: 只做机制设计与可执行交接；不含实现、复现、RTL、Linux、构建、上板。所有数字都是手算、服务单位或字段子集，不是实测。
**作者/责任**: 机制与创新判断、验收与保存由 Codex root 亲自完成；一个必要独立关键设计席参与；源码证据整理与中文稿起草由 DeepSeek 完成；最终核查保存 Codex root。
**权威入口**: [root-result-acceptance.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round03/root-result-acceptance.json)、[root-frontier-design.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round03/root-frontier-design.json)、[root-source-review.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round03/root-source-review.json)、[root-primary-review.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round03/root-primary-review.json)。本稿接续 [round02](/home/xvhaocheng/work/iommu-platform/docs/mechanism-design-handoff-20261009-round02.md)，完整规则与推导见上述材料。

## 一、继承 round02 之后，本轮新增了什么

Round02 的 L 是“在既有 burst_req_t 首次 backend VALID 之前，用当前翻译局部性选一个目的缓冲”。Round03 将其推进为 **L-F**，增加以下可检验的合同：

1. **新的不可变 pool-parent 类型 + 内部 unbound write 阶段**。Round02 改的是已有 parent 的选择时机，目的字段仍要在 first VALID 前固定；L-F 明确要求一个新 parent 类型，公开承诺“从某注册池里选一个输出页”，而不是固定目的 IOVA，并在内部保留一个尚未绑定目的的写阶段。
2. **抽象 free-slot 容量预留**。Round02 只能在 launch 那一刻挑已经 FREE 的 slot；L-F 先预留一个 FREE 数量的抽象 credit，源阶段期间真实释放出来的合格 slot 稍后仍可被选。
3. **事件维护的 resident/free 索引**。Round02 每次 COPY 最多做 q 次普通端口 hint 查询；L-F 改为在 IOTLB fill/replacement 与 ownership reserve/release 事件上维护一个有界反向索引，选择时仍照常发普通 WRITE 翻译和权限请求。索引只是 advisory：不存 PA，不存可复用权限，真实 source READ 走未改动的 IOMMU，不需要 source PA voucher、权限重放或额外 walk。
4. **有界备选 L-I**。把同一套事件索引放回已有的 pre-firstVALID 选择点，从而避开新符号后端合同；它是 L 的实现变体，不是第三条独立研究方向。若 L-F 的完整成本输给普通 source prepare，就退回 L-I 或停止该收益主张。
5. **继承与降级**。R 仍是 round02 的关系压缩 backup，本轮不新增 R 收益；T 因原生 PTE 续步可下一拍发起，保持低优先级。
6. **未提升项**。joint matching、裸 hot-first、current-victim prediction、slot credit、软件 generation relocation 都不是独立算法主张。

## 二、必须写进正文的关键纠偏

- **ownership 正例不能写成 A 覆盖 Z**。正确顺序是：初始驻留 Y 与 Z；A 先拥有 Z 并完成真实 Y->Z COPY；完成后 Z 属于消费者；只有消费者**实际访问/同步/释放**之后 Z 才 FREE，而且这个过程要落在 V 必要的源 S 翻译/读取阶段内。A 与 V 还需要一个合法的共同 translation-cache context/key，默认不同 PROCESS_ID 不构成证明。
- **61/97 只是 agent draft，不是已接受定量正例**。该构造没有排完整的资源冲突排程，只能当示意。定量门槛统一改用 max(s,r) 形式。
- **成本口径**。当前追踪 parent 选项 O=98；L-F 已命名字段是 3355bit/420B；旧设计在同信任内核合同下调整后是 3587bit/449B；两者只差 232bit=29B，不是完整实现面积，也不能声称 1473->420 是独特贡献。全部 256 槽的软件 generation 原始字段合计至少 1024B；旧方案采用同一信任合同也能省去这部分硬件副本。
- **不可改已有 VALID**。普通 COPY 不能修改既有 burst_req_t 的 VALID 语义；现有 decouple_aw 只是延迟一个已经固定的 AW，并不能重新绑定目的。
- **强对照必须保留**。strong source prepare 可以匹配甚至更快，必须保留它、允许 source-prepare 与 WRITE 合法重叠、允许合法 PA 续用；不得人为给它加重复 walk 或禁止 PA 续用。
- **索引不是新算法**。同一合同下的普通 hardware hot-first 与这套 resident index 完全匹配；不能把索引本身单独称为新算法。

## 三、L-F 合同：新 parent 不可变

**操作**: COPY_TO_POOL(src, pool, length=64, tag)。父请求公开承诺“从 pool 中选一个输出页”，不承诺固定目的 IOVA。

**不可变公开字段**: 在**新的 pool-parent VALID 之前**固定 source、length、pool、context/options、epoch 与 tag。

**当前首版限制**:

- 对齐 64B COPY，源与目的各自落在同一个 4KiB 页内；首版固定目的偏移 256。
- 固定、稳定的单级 4KiB 映射；每个参与 frontend 只有一个受信 pool/context；已注册池与匹配 context 的虚地址范围互不重叠。
- 每个活动 pool COPY 的源位于该池之外；池注册与正常历史预热可以先于 FREE 报价。
- 应用接受任选 slot，只有在 READ/WRITE/B 成功并完成相应 cache 同步后才拿到真实身份。
- 当前只有最前两个 DMA 实例在 N 模式有完整 buffer provisioning；四池状态只是容量设计，不是四个原生 walker 或四个 pending source translation 的证据。
- 首版每个 frontend 至多一个活动 parent；普通对照保留全部 8 个原生 AX head、合法 burst 流水与既有 buffer，不对对照强加一子限制。
- 受信内核串行化且恰好一次地校验 release；软件维护每槽 generation 与 lease。同一信任合同必须同样施加到所有对照。不受信直接释放与动态重映射需要单独计价的扩展。

手算中的单个 AR/AW 要求配置允许一次传完 8 beats；若 burst 限制更短，必须计入额外 AR/AW/B，READ 回调聚合所有子请求后才算成功。索引遇到同页重复条目时，reserve/release 更新全部匹配行，实际 ownership 原子校验保证不会重复分配。

**发布边界**:

- 既有固定目的 burst_req_t 自首次 VALID 起不可变，不能靠改 dst 实现 L-F。
- 新 pool-parent 字段在 VALID 期间与接受后保持不可变；每个源 AXI AR 自首次 VALID 起稳定。
- 所选目的、WRITE options 与 write child 在它们首次 AW VALID 之前冻结；目的未绑定时不得出现 AW。
- 使用经过验证的 parent READ 完成回调，覆盖全部真实 R beat 与读状态，而不是 IOTLB tag hit、单独首个 R、未被接受的 VALID 或快照抽头。

## 四、credit 不变量与状态机

**不变量**: 0 <= unbound_count <= free_count <= N；绑定原子地消耗一个 credit 和一个 FREE slot；新准入要求 free_count > unbound_count。作用是：未绑定的源工作不可能耗尽它承诺的输出容量，同时绑定可以选一个源阶段内新释放的热 slot，而不是当初可用的冷 slot。义务：该池所有分配路径都遵守同一不变量；release 不绕过 lease 校验；重配置要等 credit 与 ownership 全部排空；不对流氓分配/释放路径声明正确性保证。

**状态机**:

- **ADMIT**: 检查 pool/context、64B 几何、源不重叠；预完成容量与完整 64B payload 容量；原子要求 free_count > unbound_count 后 unbound_count 加一；此时不预留也不报告任何具体页。
- **SOURCE**: 发普通源 READ 与正常权限检查；把 8 个真实 R beat 收进已计价的 native payload，保持源请求字段稳定；回调返回真实 parent 身份与读状态；不额外发准备 READ，也不发合成权限。
- **BIND**: 源真实成功完成后，对该池查看至多 E 条 resident 元数据行；按普通有界 RR 选一个 FREE 的已索引 slot，否则按普通池 RR 选 FREE slot；原子做 FREE->COPY，同时 free_count 与 unbound_count 各减一；stale hint 不能选中非 FREE slot；若 invalidation/config 与选择冲突，抑制 hint 或重试一次，然后按普通方式选 FREE 槽回退，计入相应开销。
- **DESTINATION**: 由所选页与固定偏移生成并锁存普通 write child；走未改的 WRITE 翻译/context/权限；stale hint 可以 miss 或 fault；只有目的已绑定才发布稳定 AW；合法排空全部真实 W 与 B。
- **COMPLETE**: 只有最终 B 与 COPY 成功才发布 tag、pool、slot、epoch、length、status 并标记消费者 ownership；完成存储已在 SOURCE 之前预留，因此不会阻塞已经发出的响应。
- **RELEASE**: 内核校验软件 lease/generation 与 cache 同步，然后发有序受信 release；原子置 FREE、free_count 加一、更新匹配 resident 元数据；重配置要求所有 unbound、COPY 与消费者 lease 排空后才能改 pool epoch。
- **ERROR**: 源 fault 且未 BIND 时，合法排空/取消尚未发布的工作并释放抽象 credit，不产生输出；目的 fault 后隔离该 slot，等 outstanding 事务排空后报错。读写错误聚合与原生响应排空要单独设计，当前 NO_ERROR_HANDLING 不提供该合同。

## 五、事件维护的 resident/free 索引

- **行格式**: 每个 IOTLB way 一行 valid1、pool2、slot6、eligible_free1，F=4、N=64 时 10 bit/way，16 行共 160 bit。
- **fill**: 在普通 4KiB 非 global fill 胜出时，把更新 VPN/context/stage 与至多 F 个注册范围比较，用减法得到 slot，记录该 way 的 pool 与 slot 及当前 FREE 状态；普通 cache tag/content 仍独立更新，索引不得阻塞普通翻译。
- **replace**: 真实替换时覆盖旧行；不保留脱离的 residency，也不把被替换的数据算作有效。
- **ownership event**: 一个串行 reserve/release 事件把 (pool, slot, new_FREE) 广播给 E 个小引用；只更新匹配行；并发 fill 使用已解析的 ownership next-state，或保守抑制 hint。
- **invalidate**: 任何 IOTINVAL 或 policy clear 都保守地使全部索引行失效，并在那一拍压过新 hint；正确性仍走正常权限检查。不假设后台 free 扫描；后续 fill 重建该行。选择性失效后丢掉仍然有效的热 hint 是明确的负例。
- **selection**: 对 E 个 eligible bit 做 pool 过滤与 RR 归约，绑定前做一次原子真实 ownership 测试；不发普通 IOTLB hint 请求，也不为未选页做投机 PTW。
- **timing**: 索引自带 compare/reduce/mux 与 fanout 成本；同拍版本可能伤 Fmax，流水维护要计状态与 stale event 处理，或保守丢弃 hint；它不是免费的额外 IOTLB 端口。
- **permissions**: 精确 context 范围匹配是保守的；首版跳过 global 与非 4KiB 项；无论索引是否 eligible，正常 READ 与 WRITE 各自都要过自己的访问检查。
- **普通对照**: 同一合同下的普通 hardware hot-first 与这套索引完全匹配；软件 fresh-state 选择在完整成本 <=3 时也匹配；便宜 LIFO/页局部策略残 miss p_b 在 20*p_b <= 3+额外成本时胜出。因此这套索引本身不能单独称为新算法。

## 六、完整已计成本与待计成本

**已计命名字段（O=98 时）**: pool 4*(64+7+24+20+16+2+32+6+7+7+4)=756 bit；ownership 4*64*2=512 bit（00 消费者，01 COPY，10 FREE，11 隔离）；resident_index 16*10=160 bit；parent 4*(64+32+32+2+32+6+64+4+2+1+O)=956+4O bit；completion 4*(32+2+6+32+4+1+32)=436 bit；callback_skid 69 bit；arbiter 74 bit。合计 2963+4O=3355 bit，向上取整 420 B，且**只是命名字段**。

**旧口径与本轮纠偏**: round02 用真实 O=98 时是 11387+4*98=11779 bit，即 1473 B，而不是 O=32 示意的 1440 B；把每槽 generation 也移到同一受信内核合同后，旧设计变成 3195+4O=3587 bit，即 449 B。因此 3355 与 3587 只差 232 bit=29 B，**不是完整实现面积**；布局还在 callback/completion 合同上不同，两者都不是净实现面积。不能声称 1473->420 是独特架构节省。

**软件侧同信任对照**: 每槽 generation 32 bit、4*64 槽至少 8192 bit=1024 B 原始 RAM，单独计费。也就是说，软件用至少 1024B 的 generation 同信任对照同样能省掉这部分硬件语义。

**待计 / 未解决**: fill 上至多 F 次 context/range compare 与 slot 算术、每 reserve/release 的 E 次 pool/slot compare、pool filter、16 路选择与地址重建；串行 ownership 命令与四 frontend arbiter；register bus、CDC、status、counter、error、callback routing、write-child 再生成、事件流水/版本状态、packing/ECC 与 fanout。每个活动 pool parent 至少 64B 保留源 payload；最前两个 N 实例已有 512 word buffer 与对照公平共享，legacy 三 word 实例需要额外容量或显式分块。既有 8 AX head、32 个 translated write-data word 与 spill/流水状态是公共资源，未经证明不能算消除或重分配；四个索引 context 不等于四个翻译生命周期。池内存仍是 4*64*4096=1MiB DDR。内核 generation/lease 表、pinning、映射、cache 同步、消费者访问、release MMIO 与 completion CPU 流量都是额外系统成本。等预算的额外普通 IOTLB 条目、端口、lookahead 与原生并发仍是强对照；Powers-of-two PLRU 改尺寸与 comparator/timing 成本要计入，不能由 bit 除法推出。

## 七、真实正例 hypothesis 与强对照

**ownership 正例（本轮主推的真实动作假设）**: 初态驻留 Y、Z；V 池中有 FREE 冷 X 与当前由 A 持有、用于其 pending 热 COPY Y->Z 的 Z。A 真实完成 COPY 后 Z 归消费者；只有消费者实际访问/同步/释放之后 Z 才 FREE，并要落在 V 必要源 S 翻译/读取阶段内。A 与 V 需要合法共同 translation-cache context/key。推进条件：V 的 unbound credit 允许立刻启动 S；Z 真正 FREE 且熬过 source fill 之后 V 绑定 Z。现有 early fixed-dst COPY 只能选冷 X，或等到 Z 释放后才启动源。

**max(s,r) 门槛**: 令 r 为真实 release 时刻、s 为真实 source 阶段、w 为源后剩余 copy 工作、delta_L 为晚绑定机制 delta、delta_P 为 source-prepare continuation delta，则 early-wait = r+s+w+delta_wait；L = max(s,r)+w+delta_L（在所述重叠成立前提下）；最强 prepare = max(s,r)+w+delta_P，并且可能有更好的 payload 重叠。L 只有在**完整增量成本/面积或端到端延迟**低于该对照时才赢。要有用，需要重叠 min(s,r) 超过相对 wait 的开销，并且要有相对 prepare 的实测完整成本差，而不只是有 min(s,r)。若 release 在源阶段之后、source walk 很短/完全隐藏、Z 在绑定前被驱逐、消费者必须固定地址、source-prepare continuation 很便宜、或安全 buffer 空间为零，收益会消失甚至反转。

**索引构造**: N=64、h=8、q=4、M=20 时，普通 RR 是 V41.5/A85.5；q4 probe 是 V41.56134/A85.56134；若全部索引维护/fanout 都能塞进已计价实现且不新增串行 stall，delta_index=3 给 V27/A71、1 次总冷 walk，相对 RR 增益 14.5，之后每一项维护/staging/消费者成本都从 14.5 里扣。这**只说明去掉了 q 次查询观测瓶颈，不是新的 hot-first 算法**。h=0、所有 FREE 页都驻留、stale hint、或便宜最优策略 p_b=0.1，都会让这个额外选择失去用处。

**source 自驱逐小例**: 原生两个受保护 way 可先装 X、Y，随后源 S miss 并填它当前的 victim；仅凭早先 X/Y hit flag 无法区分谁存活。精确 current-victim/source-miss prediction 与临时目的 pin 就能复现这个小例，所以付费 post-source selection 在这个例子上并不独立更优。该例不是决定性独立收益例。

## 八、相邻反例

- release 早于到达；
- release 在 source 阶段之后；
- 从不 release，但正好有 FREE X 可回退；
- 全热/全冷；
- source 已经 hit；
- 只有一个 free slot；
- 正常 invalidation；
- 便宜 hugepage/按页分组的布局；
- 消费者必须固定地址且要额外一次 COPY；
- h=0、所有 FREE 都驻留、stale hint、p_b=0.1；
- 假设的 source walk 独占唯一 service server，使 A 的完成与消费者 release 无法落在源关键路径内。

只有这些邻例下都有界、普通的回退可解释，收益域才算说得清。

## 九、6 项下游任务：推进与停止

先并行完成 F1（可达轨迹）、F5（合同）与 F6（完整成本）；有自洽正例后进入 F2 的小模型，再按差异补 F3/F4。不先搭大矩阵，也不要求进入最小模型前证明普遍优越。首批固定 6 组：S1 两个热 FREE 候选加冷源、无并发变化；S2 源阶段内真实释放 Z；S3 同例延迟释放；S4 释放在到达前且源已命中；S5 所有候选冷；S6 正常竞争 fill 改变目的驻留。每组保留完整 READ/WRITE/B 与必要许可，按实际差异决定是否补邻例。

| ID | 任务 | 推进标准 | 停止标准 |
|---|---|---|---|
| F1 | 原起点合法 warm/ownership trace：source Y 与输出 Z 已缓存，A 为 pending COPY 拥有 Z，V 有 FREE 冷 X，V 源 S 冷；A 真实完成 -> 消费者/cache sync -> 在 V bind 前 release Z。验证共同 PSCID/stage、原生 masked PLRU（含 post-fill source hit）、全部 R/W/B 与真实 callback/buffer 路由。不建快照注入器。 | 一个合法 N-overlap/ownership-change 例，且 Z 在绑定时真正 free 且有用。 | 默认 context 不能合法共享所声称条目、必经 source/inflight fill 总会移除 Z、或计费后的 release 无法重叠。 |
| F2 | 小事件模型：用完整原生容量与同一原始 trace，比较 L-F、early/页局部/LIFO、launch 前等热、source-prefetch/permission-prepare+late allocate、精确可用 victim prediction、合法临时 pin、full permissions ahead+retention 与额外普通 IOTLB 条目。不迫使 prepare 做重复 walk；包含高效 ticket 备选与真实 W/B。 | 相对最强可执行对照出现有用的 V/A 端到端或实现成本 frontier；单个正区域即可继续，不要求普遍证明。 | 普通 prepare/同合同 late allocator 在同/更低完整成本下匹配，或真实其他瓶颈掩盖全部节省。 |
| F3 | 同一工作负载消融：只晚绑定 + 普通 allocator；只在 early fixed parent 用 resident index；完整 L-F；同点 q1/q4 probe selection。计价 fill/release 维护、source buffering、普通端口仲裁与 CPU 侧。 | 分清哪个动作去掉真实 walk/延迟，哪个只改元数据。 | 只有在弱随机分配或未计价索引/端口下才有增益。 |
| F4 | 相邻例：release 早于到达、release 在 source 阶段后、never-release 但有 free X 回退、全热/全冷、source 已 hit、一个 free slot、正常 invalidation、便宜 hugepage/按页分组布局、固定地址消费者要额外 COPY。 | 有界普通回退与可解释收益域。 | credit/payload 无界死锁、无界等热、或必需的收尾 COPY 直接击败该例。 |
| F5 | 精确新接口合同 worksheet：不可变新 parent 与每个 AXI VALID；一次真实普通 READ；normal WRITE 检查；同拍 pool/fill/ownership 变化；credit free_count 不变量；完成预留；source/dest fault；有序受信 lease release；未完整排空前不得重配置。 | 一个自洽受限 64B 首版，无需 free source voucher 或额外 walker。 | 需要可变既有 VALID、绕过权限、未计价 payload/route/continuation，或响应排空无法满足。 |
| F6 | L-F 与普通 source-prepare/同索引对照的完整状态/端口/逻辑 worksheet：把 O 换成 98，计价 write-child 再生成/callback、source payload 保留、CPU generation 表、MMIO/cache sync、索引 fanout/流水与等预算普通 cache 扩展。 | 相对具体更简单架构仍有一个可行成本点存活。 | 已知的 29B 节省被抹掉且没有查询/延迟优势，或同预算容量/布局更便宜地解决工作负载。 |

## 十、条件式独立创新

**等价实现与前作要分开**：把本提案完整索引与新 parent 合同放进一个普通 hot-first allocator，会得到机制本身的一种等价实现；这不是已有论文覆盖的证据，也不是要求提案击败自身。只晚绑定的普通页局部/LIFO 是消融；独立可执行的 source prepare、victim/pin、布局与容量扩展是强对照。若它们完整复现收益，应明确是哪一部分没有新增价值，不能只凭“另一个系统也能加同一模块”否定整个集成。

单个动作都不是新贡献：hot-first、RR、count reservation、late pool selection 都是普通动作；pooled selection、locality/coloring、translation-size-aware allocation、prefetched translation 也都已有工作覆盖。**创新门槛**：只有当存在合法 ownership-change 区间与真实付费流水，使 L-F 相对 source prepare、current-victim/pin、软件/局部性与等资源容量对照，在 V/A frontier 或完整实现成本上更好，并且 exact nearest published mechanism 不覆盖这种集成时，它才可能构成独立机制贡献。若这些对照在同/更低完整成本下匹配，就停止该收益主张，或退回 L-I。若最近的集成 DMA allocator/resident-index/late-output-selection 机制完全覆盖，则整条主张停止。L-F 是下一步设计/决定性模型假设；R 保持 backup，T 保持低优先级，其 round02 负条件不外推到未测/未推导情形。

## 十一、本轮 primary 定向核查（每项摘要不超过 80 汉字）

- Cache Index-Aware Memory Allocation, ISMM 2011，DOI 10.1145/1993478.1993486，[原文](https://www.cs.tau.ac.il/~mad/publications/ismm2011-CIF.pdf)。摘要：分配器影响 TLB/cache 局部性、LIFO 页局部 free list、按 cache index 做 spacing/coloring 都是旧工作；不能把 locality-aware allocation 当新。该核查只读作者 PDF pp1-4 与指定 browser 行，不证明 L-F 首创。
- Providing Task Isolation via TLB Coloring, RTAS 2015，DOI 10.1109/RTAS.2015.7108391，[原文](https://moss.csc.ncsu.edu/~mueller/ftp/pub/mueller/papers/rtas15.pdf)。摘要：用虚地址分配隔离组相联 TLB、按 color 的 free list 与容量约束都已公开；原生共享 IOTLB 是全相联 + way mask，set coloring 不能直接照搬，它仍是相关分配器/锁控制。
- Elastic Translations: Fast Virtual Memory with Multiple Translation Sizes, MICRO 2024，DOI 10.1109/MICRO61859.2024.00012，[原文](https://site.psomas.xyz/assets/files/elastic-translations-preprint.pdf)。摘要：fault 时按连续性分配、透明中间尺寸翻译、异步提升与 TLB-miss 引导选择已覆盖；不能把匹配翻译几何的分配说成全新，同成本 layout/hugepage/coalescing 对照仍可能消掉 L-F 收益域。
- Latency reduction for direct memory access operations involving address translation，US20140281055A1 / US9460024B2，[原文](https://patents.google.com/patent/US9460024B2/en)。摘要：提前访问 descriptor/buffer 翻译、含 dummy 读写，说明翻译预取与 descriptor 触发准备已存在；L-F 做的是必要真实 source READ 后延迟选输出，不是只为预热 IOTLB 的额外 dummy 操作。该检索非完整专利/权利要求/法律审查。

round02 已接受的 io_uring provided buffers、DAMN、IOTLB wall、CoLT、AXI-REALM 与 MASK 范围继续有效，本轮不重读也不当成新完整核查。仍开放：exact closest integrated DMA allocator/resident-index/late-output-selection 机制、HPCA2014 clustered translation 全文、同合同普通 source preparation 是否同/更低完整成本。

## 十二、本轮实际完成范围

本轮 model_runs=0，rtl_changes=0，linux_changes=0，builds=0，board_runs=0，claude_calls=0；Codex 独立关键设计席 1；DeepSeek 实际成功调用 2 次，分别整理源码证据与起草文档；两次 provider total_tokens 合计 148316。只做设计与交接，不含实现或复现；实现与复现交给下游。所有数字都是手算、服务单位或字段子集，不是实测 cycles，也不是模型通过。不声称首创、不声称达成二区、不声称模型/RTL/Linux/build/board 结果。下游负责实现与复现，本轮完成设计交接。
