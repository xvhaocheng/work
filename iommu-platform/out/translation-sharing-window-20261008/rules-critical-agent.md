# §48：真实 parent 窗口的译址空洞与普通连续前沿构造

2026-10-08，Codex 独立关键源码/机制推演。仅阅读最新 `out/dual-write-prefix-20261008/` producer、helper 及真实 iDMA 的必要分段/握手依赖；没有改 RTL、构建、运行 DUT 或调用 Claude/DeepSeek。本稿不是运行结果。root 负责本轮主报告与状态文件。

## 1. 审定结论与导航

1. **一般正常、8B 对齐 job 的成功译址序列允许空洞，且不需要 fault。** 最小有用见证是源 `3072`、目的 `0x10000`、长度 `4096`、`CONF=1`：首父请求只有 1KiB，被 helper 的 `len==255` 资格过滤；第二父请求为 2KiB，仍能先成功准备。此时源第一页仍未译址。精确握手见 §3。
2. **该见证没有证明原页对齐 fixture 已出现空洞。** 对原 `src=0`、长度 32KiB、无 fault/flush、所有父请求均为 2KiB、严格 INCR 且 eligibility 为地址前缀的限定，helper 单 slow 请求和 flow 内 oldest-first 能维持首次成功准备前缀。快 hit 本身不能推翻这个限定，见 §4。
3. **`done_q` 是父请求的 R 数据退休序号，不是译址完成序号。** `prepared_q` 是尝试已经接受，也不是成功结果。真实结果证书是仍匹配 parent generation/key 的 `cache_valid`，见 §2。
4. **无需另建 hole bitmap，普通实现可以扫描既有 Depth4 窗口求连续证书前沿。** 但原 helper 对短父请求根本不产生证书，若要求这些父请求也在 R 退休前推进，必须补明确的 TC 成功事件或扩大既有四槽结果资格；不能把它假设成现有 IOMMU 免费信号，见 §5。
5. **“注册 job 的剩余用途”若只是 interval suffix 取交集再 OR/refcount，与同信息普通 job-aware pin/release 算法等价。** 下一有意义判别是 materialization 证书的接口来源、完整成本及共享容量收益，见 §6–7。

## 2. 字段与事件：三个不同的 valid

以 flow `f` 为单位，原始父窗口的真实 live 序号是

`L_f = [done_q[f], pa_q[f])`，`0 <= pa_q[f]-done_q[f] <= 4`。

`seq -> slot(seq)=seq%4`。top 以 `done + ((slot+4-done%4)%4)` 重建该物理槽当前 generation；不能用槽号直接当 job 内序号。

| 状态/事件 | 实际含义 | 可以证明什么 |
| --- | --- | --- |
| producer raw `s_ar_valid && s_ar_ready` | 原始 AR 存入 `parent_q`，`pa_q++` | 该父请求元数据已私有接收；不证明有 TL 请求或成功译址 |
| top `reuse_parent_valid` | 真实 live **且** len255/size3/INCR/无 lock、页内等 helper 资格 | 允许 helper 查询/缓存；其 0 不等于该 raw 槽不在途 |
| `prep_offer_q` | 已经构造一个后台 TL offer | 后续门变化不能撤回该 offer；不证明接受或成功 |
| TL request `m_valid && m_ready`，随后 `prepared_q=1` | 一次准备/first-source 翻译尝试已接受 | 可能尚在途，也可能最后失败；不是 materialization |
| TL response `m_resp_valid && m_resp_ready` 且 helper fill 条件满足 | 结果写入 `valid_q/value_q/seq_q` | 当前仍存活、同 generation/full key 的私有译址结果已保存 |
| `cache_valid` | 上述结果仍符合 enable/flush/live/seq/正常可读合同 | 可用作此受限静态模型中的成功 materialization 证书 |
| 子 AR `m_ar_valid && m_ar_ready` | child 被下游接受，`busy_q=1` | 非 direct 路径可仅进入 TC AR FIFO；不能当外部物理 AR 或 TL 完成 |
| parent 最后一拍 R `m_r_valid && m_r_ready && s_r_o.last` | `returned_q` 推进，`done_q++` | 父数据已完整返回；当前正常 R 合同中也蕴含之前成功译址，但时间更晚 |

producer 对 raw AR 的检查允许 len127 等短父请求；它只限制 job 范围、8B 对齐、size3、INCR、页内和特定小非缓存请求。`done_q` 仅在最后一拍 R 推进。

helper 的 `prepared_q` 在 TL request 接受时置位；成功 `valid_q` 则在 TL response 退休且 parent/key/seq 未变、无 error/ignore/MRIF、PA 页内偏移吻合时置位。后台 response 自行 ready；它不需要等 source R。

关键源码：

- [producer live/候选头部](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/compact_pretl_owner_fragment.sv:88)
- [producer job/raw 接受与短父请求检查](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/compact_pretl_owner_fragment.sv:203)
- [producer child 接受与 R-last 退休](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/compact_pretl_owner_fragment.sv:281)
- [top generation、helper 资格、prepare mask](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/multi_actual_dma_top.sv:1013)
- [helper cache valid/failed 与 source-first/reuse](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/hit_bypass_parent_translation.sv:101)
- [helper request 接受、offer 与成功 fill](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/hit_bypass_parent_translation.sv:190)

## 3. 无 fault 的具体可达空洞

### 3.1 真正 legalizer 给出的父序列

真实 wrapper 使用硬件 legalizer、64bit AXI、`NumAxInFlight=8`。AXI 的最大 burst 为 256 beat，所以 legalizer 的 `PageSize=min(4096,256*8)=2048`，默认 source virtual boundary 是 2KiB。

`CONF=1` 的 bit0 是 `decouple_rw=1`，bit1 `deburst=0`；source 不按目的边界共同截短。legalizer 以 `r_num_bytes_to_pb` 分段，推进地址加本段字节数。前端给 INCR、无 lock、data access，job 的普通合法性要求只到 8B 对齐。

选择一个正常单 job：

`A: src=3072, dst=0x10000, len=4096, CONF=1`。目的 offset 仍为 0，完整 source/destination 区间互不重叠；源分段和下面全部握手条件不变。

| seq | raw AR start | 字节数 | AXI len | helper `parent_valid` |
| --- | ---: | ---: | ---: | --- |
| 0 | 3072 | 1024 | 127 | 0：不满足 len255 |
| 1 | 4096 | 2048 | 255 | 1：start 页内 offset0，其他正常条件满足 |
| 2 | 6144 | 1024 | 127 | 0：job 尾段 |

所有段都满足 producer 的 source 范围、8B 对齐及不跨 4KiB 检查。seq0 的 `len127` 也不触发 `!cache[1] && len<16` 拒绝。

源码：

- [64bit/真实 DMA 参数](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/multi_actual_dma_top.sv:244)
- [AXI 最大 256 beats](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_pkg.sv:107)
- [legalizer PageSize](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:74)
- [legalizer source boundary](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:139)
- [decoupled byte数与源地址推进](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:208)
- [CONF bit0/bit1](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv:196)
- [前端 INCR/decouple/deburst 配置](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:354)

### 3.2 握手序列与必要驻留条件

设 V 没有活跃 job，`reuse_enable=1`、`reuse_prepare=1`、`physical_bypass=2`、`pause_a=0`；只在下面这个**有限见证前缀**中将 `pause_reads_a=1`，成功 fill 后可撤销。其用途是阻止 A 创建任何 source child，而不是让内存不服务或永久停 budget。映射正常，TL/目的 AW/内存正常有限服务。

1. **J：**普通 A job 接受；`active[A]=1, pa[A]=done[A]=0`。前端/后端正常收到上面的 immutable job。
2. **R0：**raw AR seq0 `(3072,len127)` 向 producer 握手。槽0 存父元数据，`pa=1, done=0, returned[0]=0`。top 的 helper 资格槽0为0。
3. **R1：**raw AR seq1 `(4096,len255)` 握手。槽1 存元数据，`pa=2, done=0`。top 的 helper 资格槽1为1。
4. **O1：**当 helper 没有 slow/hit/offer、TC 没有 source TL valid 时，背景选择器从 `head_seq=done=0` 扫 rank0、rank1：rank0 不满足 helper `parent_valid`，**不会终止扫描**；rank1满足条件，锁存 seq1 的 prepare offer。若目的 AW 翻译仍占用 normal TL，则等待它正常完成即可；无 source payload/R 依赖阻止该 prepare。
5. **T1：**该 prepare 的 `m_valid && m_ready`。`slow_q=1, slow_src=1, slow_prepare=1, slow_seq=1, slow_index=1`，`prepared[1]=1`。
6. **P1：**seq1 的正常可读 TL response 退休，完整 key/seq/live 检查通过，helper `valid[1]=1`，之后 `cache_valid[1]=1`。`done=0`，seq0 仍在 producer，`returned[0]=0`。

source seq0 尚未译址：child 创建被 `pause_reads_a` 阻止；没有 source child AR，就没有该地址的 source TC 翻译。后台 seq0 又被 helper 资格过滤。seq1 此时已有实际保存的成功结果，故这是 **首次 materialization 的真空洞**，不是两次缓存命中顺序不同。

raw 前两笔同时驻留不要求第一笔 R 已返回：producer 有四槽容量，后端独立 read-meta FIFO 有八槽；`decouple_rw` 允许 read legalizer 独立推进，AR 路径为 fall-through 后直接接协议 AR。这些元数据槽在空 payload 下足以接受至少前两笔。该见证允许目的 AW 正常接受并等待 W，未制造 W-before-AW 或双 W order inversion。

关键握手证据：

- [raw producer 选择与正确返回 ready](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/multi_actual_dma_top.sv:708)
- [raw/prepare 的 pause gate 区别](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/multi_actual_dma_top.sv:646)
- [child 创建唯一 pause_reads gate](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/compact_pretl_owner_fragment.sv:281)
- [helper rank 扫描不在无资格头部终止](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/hit_bypass_parent_translation.sv:139)
- [read-meta FIFO 和 AR fall-through](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:554)
- [decouple legalizer 流控](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:452)
- [transport AR 独立直通](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:200)

如果 watermark 直接取“最大成功 start page”，这里会取 page1，并声称 page0 已越过。job 的 page0 源区间 `[3072,4096)` 实际尚未 materialize，所以会错误取消其 V/A remaining-use 保护。保守保留当前 q 页也救不了 q 之前的这个 hole。

**限定：**上述是直接源码约束下的握手可达构造，未在 DUT 执行。它改变了页对齐源地址 fixture，不能据此宣称既有 §43 run 发生了 hole。

## 4. 原全 full-parent fixture 的前缀条件为何更强

以下条件同时成立时，flow 内首次有效准备保持前缀：

- 单活跃 job，strict INCR raw 顺序，所有 live 父请求均通过 helper 资格；
- enable/prepare 不变、无 flush/失效，正常成功且 parent/key/generation 不被取消；
- prepare eligibility 为地址递增的前缀；
- 旧 held offer 保留，单 `slow_q` 未退休时不创建下一条后台 offer。

归纳理由：新背景准备选该 flow 最老的尚未 valid 且未尝试父请求；在上述正常成功限定中，较老 `prepared && !valid` 只能属于当前未退休 slow 请求，而该请求使新后台准备无法发出。较老槽的成功 fill 退休后，后一个才可进入后台准备。普通 source child 也只属于 `done` 头父请求；复用 hit 要求该父行**已经** valid，不能第一次 materialize 一个尚未准备的更后父请求。

prepare mode 0/1/2 改变 flow 之间偏好，不改变每 flow rank 从旧到新的扫描。两个 flow 全局成功事件可以交织，这不等于同一个 job/direction 的缺口。runtime eligibility 仅控制**新**准备，原 offer/slow 必须完成；一个后来变得不 eligible 的 held offer仍可 fill，也不能单独证明它前面的正常父请求未完成。

因此，不能用“有 fast/后台路径”笼统推翻原页对齐无 fault fixture 的前缀。§3 真正打破的是第一项：producer 的 live 集合严格大于 helper 资格集合。另有 fault/cancel 后 prepared-invalid 被扫描跳过的扩展行为，但本稿不拿它反驳无 fault 原合同。

以上是受限手工归纳；不是所有控制组合、动态映射、多 job、fault 或任意外部 parent 接口的通用证明。

## 5. 最强普通同信息实现：扫描原窗口，不另存 holes

### 5.1 先用现成证书，得到保守连续前沿

定义真正 raw live 条件 `L(seq)`，**不要**用 `reuse_parent_valid` 代替它。定义当前 generation 的成功证书 `T(seq)`：对于已支持的 full-parent，它就是仍匹配的 helper `cache_valid`。对原 helper 没覆盖的父请求先视为“无证书”，即使它可能已经正常翻译过。

组合计算：

```
k = 0
for j = 0..3:
    seq = done + j
    若 j==k 且 seq<pa 且 T(seq): k++
    否则停在第一处 live 未证书槽
r = done + k
```

这使用既有 raw slots、`done/pa`、helper `valid/seq/key`；不需要新增 per-parent hole bitmap。`k` 可为组合 3bit，不是寄存状态。若有成功证书槽1、未证书槽0，输出仍为 `r=done=0`。

正常数据已经退休的序号 `<done` 是历史成功前缀。对当前 live 窗口，扫描只推进连续证书段。static normal job 下行在数据退休前稳定保留；退休后从 live 集合消失并同时推进 done，因此不会把已证明的前缀突然变成洞。

page watermark 可保守地从连续成功段的最后 start page 更新；若连续段为空而 head 存在，用已数据退休前缀得出的 head start page 下界仍是安全的。当前 q 页保持到下一页或可靠 end marker。窗口暂空时不凭空推进；可以保持已发布 watermark，直到可靠 job end。superpage entry 需对其 **end page** 判断，而不是只比较起始页。

不需要额外 hole bits，不等于零成本：扫描器需要槽选择、generation/资格检查、优先编码/加法和地址页比较；已有 window/result 存储及其端口必须对双方同计。一个连续 cursor 是**压缩成功证书的前缀**，不是压缩所有已成功但不连续的点。

### 5.2 短父请求若也要在 R 返回前推进，需要真实额外接口

原 helper 对 seq0 短父请求的 `source_first` 也要求其 `parent_valid`。即使解除 pause 后 seq0 通过普通 TC 成功译址，当前 helper 不会因此给 seq0 写 success-valid。因此仅扫描现有 `cache_valid` 对该槽会保守地等到 `done` 数据退休。

若候选合同明确要求全正常 raw 父请求先于 R 退休发布 materialization，强普通有两种具体办法：

1. **扩大已有四行结果的合法父集合。** 正常 8B 对齐、同 4KiB 内、正常 read/key、可读且非 error/ignore/MRIF 的短父请求也参加 parent-key/prepare/first-source 检查。在当前静态 mapping、同页 raw 的条件下，首地址成功检查给该父请求所在页的可读映射；existing response83/seq32/valid 行容量本来就是每物理槽一行，不必另加 hole bitmap。必须相应审合法长度、direct/reuse 偏移、异常/失效边界；这是提议的普通扩展，**原源码尚未提供**。
2. **显式 TC 成功事件。** 在成功 translation response 被 TC 接受且结果存入其私有上下文时，提供 `materialized(owner,job_epoch,parent_seq,success)`；同页 raw extent/translation key 由 producer 当前槽关联。以每 live parent 的证书位覆盖普通路径；如果既有行 valid 字段可合法复用则不另存，否则两 flow 的四槽只需额外 8 个 success bits，但 tag/路由/epoch 和相关请求 FIFO sideband也必须计费。不能只用 child AR upstream accepted 或 `prepared=1` 代替这条事件。

第二种必须准确带着 original-parent 身份通过 AR FIFO/TC，而不是从 TL 的 118bit 请求凭空取得 job 序号。已有 TC 是 per-bank FIFO+单 FSM，但没有这条 parent/epoch 输出合同。bank 由 AXI id 路由，不能无证明地当成固定 direction 或 DID。

目前已有 producer+helper 的字段可以在二者旁边组合扫描；普通 IOMMU 端并不直接拥有 `parent_q/pa/done/job length`。实用接口可以由 producer 发布少量

`(owner/class, context_ref, job_epoch, direction, valid, contiguous_page_frontier, end_marker)`。

consumer 若寄存每 job 每 direction 的 page cursor，仍需 `2*D*W` 位 payload，再加 valid/end/epoch/context/sideband状态；双方同计。若改由 IOMMU 直接扫描所有父行，完整 window/结果元数据与读取端口则计入其总资源，不能说 IOMMU 免费可见。

### 5.3 不要混淆两种保证

- **安全：**未被连续证书覆盖的较早页不被误判越过。现有窗口保守扫描可以保证该静态正常范围的安全。
- **及时：**所有普通父请求成功译址后立即更新前沿，不等数据。原过滤短父请求的 helper 没有覆盖这一保证；需要上面的事件/合法扩展。

若仍要求较晚已 materialize 的 hole 后条目立刻解除其保留，单 cursor有意丢掉这部分信息；可直接对四个现有槽判每 entry 的剩余用途，无需先强行变成一个 cursor。其比较器/端口及消费者可用信息仍须公平计费。

## 6. “剩余用途”与普通 interval/refcount 的具体等价构造

对注册 job `J=(epoch,context,src,dst,len)`、每 direction 的连续 frontier 与可靠结束状态，构造 `Remaining(J,dir)`：

- 已证明连续 materialize 的前缀可以取消对应缓存用途；
- 仍 live 且未证书的 raw 段、尚未 offered 的 job 后缀保留用途；
- 若只发布保守 page frontier，则直接取该 direction 的保守地址后缀；当前 frontier 所在页仍保留。

对共享缓存 entry `e`，普通实现计算

`need(e,J,dir) = compatible_context(e,J) && overlap(entry_page_interval(e), Remaining(J,dir))`。

然后 `V_need(e)=OR_J,dir [J为V && need(e,J,dir)]`，或者对相同 boolean 做 refcount。A 可以命中/借用 shared entry；其 replacement 是否可选取由 `V_need`/普通 capacity admission/pin policy 决定。job 接受建立区间，成功 materialization/连续 frontier移动更新区间，end marker删除区间。这个实现用同一 J、证书、frontier 信息逐项复现“剩余用途保留/释放”。

这是算法等价，不声称已经给出当前平台成本最佳布局。逐 entry bitmap、每 job cursor、直接比较器或缓存 refcount 各有不同状态/端口/更新代价：例如 D4/E16/W29 时，仅双方向 cursor payload 为232bit，已经高于 E×D×2=128bit 的方向用途 bitmap；job区间/context/epoch/队列等还没计入。若要主张更少总资源必须列完整布局。

同 IOVA 只有相同有效 translation context/generation/权限合同才可共享结果；共享 PTE/partial fetch 不自动共享 whole continuation。未知未来 job 正常到达，当前 Remaining 只给注册 job 的有限未来信息，不能保证所有未知未来访问均最优。pinning 也不能因持续加入 waiter/job 而免费保证公平；closed cohort/准入/动态 priority 等强普通规则必须允许并计其状态。

因为该 boolean 与上述普通 interval/refcount完全相同，**不能把“注册 job 剩余用途取交集/OR”本身作为独立机制创新**。如果真实跨模块 materialization 接口能以明确更少总状态达到相同安全/及时/公平合同，或共享容量下有普通同信息同成本布局达不到的具体保证，则那个明确差异才是可验证增量；本稿没有证明这一差异。

## 7. 下一最小判别与停止条件

下一步只需先完成一个 source-contract 判别，不必扩 performance 矩阵：

1. 对 §3 的短父请求 witness，确定拟用真实 materialization 事件是否在 seq1 fill 时把 seq0 当 unresolved、并阻止 page0错误越过；解除有限 pause 后，确定 seq0 普通 TC成功事件能否在 R-last之前补证书。若只能看到 `prepared`/最大的成功地址/`done`，就达不到该及时且安全合同。
2. 对支持集合完整的页对齐 fixture，比较候选 cursor 和普通 Depth4组合连续扫描，两者必须使用完全相同 cert/job/context/epoch 信息。先列接口、状态和组合/端口成本，再决定是否需要实现。
3. 若两者相同输入给出相同 need/release/admission，且候选没有明确成本/保证增量，停止把该算法作为创新，保留为工程接口。不能通过禁止普通读现有四槽 valid 或给普通加入数据退休等待来制造优势。

**审定状态：**§2字段及 §3分段/握手条件为直接源码手工审定；§3整体 witness未运行；§4为明确假设下的归纳，未做形式验证；§5–6是未来普通构造而非已实现 RTL。没有宣称生产 IOMMU 已具备 job/frontier 反馈，或原 §43 fixture 已实测空洞。
