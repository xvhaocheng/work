# 导航（先读这段）

- **Q1（依赖/可排空语义的最强前作）** → §2 证据卡，逐条 file:line + 完整段落。最强直接前作：**socdma2018（MMU-Aware DMA, ICCD 2018）**，其次 **ctc2009**。iDMA2024 与 AXI-REALM 只有地址/ID/length/priority/retry 元数据。
- **Q2（四机制能否做普通部分后缀预约）** → §3，逐机制给「能否 / 信息接口 / 缺证据」，并显式区分「可编程表达」与「已有策略证据」。
- **Q3（近邻清单）** → §4，四个子命题分别定位，标注是否有本轮全文。
- **同合同最强普通 C 算法草案** → §5。
- **待查论文清单** → §6（题名/年份/URL/有无本轮全文）。
- **不确定性 + 建议下一文件** → §7。

标签约定：**[P]**=所附论文/文档原文事实；**[L]**=文档作者对文献的记述（非我逐字核验）；**[S]**=源码/平台规格事实（引自文档中的源码行号，我未执行）；**[I]**=我的推断；**[U]**=未实现规格（候选，无实现/无测）。

---

# 1. 重要更正如何落入本答复

- **无时间四 DMA 模型不能否定性能空间**：§3 不把「四 DMA 共享」当作否证；只按单 copy 的接口合同判断。
- **MASK token ≠ completed-head occupancy**：§3-Q3(c) 明确 MASK 是 TLB fill token，不是完成头占用额度。
- **合法 R 反压 ≠ 释放路径**：§5 C 算法不把 `RREADY=0` 当排空前缀通道；前缀写走独立写路径，且要求写服务不以该读 RLAST/PTW 为前提。
- **accepted AR 已有顺序是当前信息**：§5 用同 ID accepted AR 的发出顺序推断返回顺序，作为 C 的准入与预约输入。
- **p 减小必须增加预约**：§5 把 p 设为「只在不减少的前提下保守」，任何 p 下降必须原子取得新增空间，否则维持旧保证。
- **WREADY 当前值不是保证**：§5 不以瞬时 WREADY 建立 p；p 只能来自「写资源对该前缀有独立、不转赠的服务合同」。

---

# 2. 四篇机制对照表（iDMA2024 / CTC2009 / socdma2018 / AXI-REALM）

| 维度 | iDMA2024 [P] | CTC2009 [P] | socdma2018（MMU-Aware DMA）[P] | AXI-REALM [P] |
|---|---|---|---|---|
| 携带的信息接口 | 1D descriptor: src/dst addr, transfer length, protocol, backend options（`idma2024.txt:151-153,234-236`）；transfer_id/status（`:219-227`） | P_REQ{连接请求, 总消息大小, 可作优先级用信息}；P_ACK（credit 初始化）；单 credit counter + 单 data input queue（`ctc2009.txt:22-24,382-402`） | 每 in-flight burst 元数据：cluster-external/-internal addr, length, AXI ID, DMA transfer ID, R/W（`socdma2018.txt:404-407`）；状态 free/in-flight/failed/peeked/reissuable，head/tail（`:362-370`） | 交易 tID、address、state、latency、timeout（`:496-497`）；6 写/4 读 stage 计数（`:505-529`）；region 预算/周期 |
| copy/job 级 **源→目的数据依赖** | 无。只有 transfer 级 descriptor 与 error 级 replay（`:277-279`） | 有 **消息级依赖链/偏序 ≺**，但是 message-flow，不是 payload 内容依赖（`ctc2009.txt:166-169,265-268`） | **有**：burst 按 page frame number 分组；失败 burst 与同 PFN burst 一起 peeked/reissuable（`:409-417`） | 无数据依赖；只有 AW↔W 绑定（写 buffer 仅在 W 数据齐备时放行 AW/W）（`:340-354`） |
| **可排空 / drain** 语义 | 无显式；error handler continue/abort/replay（`:277-279`）；数据流解耦 R/W（`:337-341`） | 有：credit 耗尽时「以最后一个 credit 作为 packet tail 结束注入」，路径释放给其他 packet；连接直到全部 flit 交付才终止（`:481-487`） | **有**：一旦有 failed burst，控制单元停止发新 burst，等所有 in-flight 完成，再按原始请求序 reissue（`:396-401`） | 无 drain；有 burst splitter + write buffer 保持「AW/W 顺序」与 B coalescing、R.last gating（`:289-302`） |
| 目的侧**分段/翻译**感知 | 无翻译；legalizer 按 page boundary/最大 burst/用户 burst 上限切分源与目的（`:294-297`） | 无目的分段；有接收端 queue 容量与 K（每 P_ACK credit 数）（`:466-494`） | **有翻译感知**：burst 不跨 page，每 burst 需在到达时刻有一个 TLB entry；连发 burst 只需短窗口内多个 entry（`:396-401`）；整传输可跨至多 17 个 4KiB 页（`:386-388`） | 无翻译；有 region/subordinate 地址译码与预算（`:417-444`） |
| 读 burst 长度控制 | legalizer 按协议最大/用户上限/页边界切（`:294-297`；AXI4 = 256 beats 或 4kB 先到，`:313-315`） | 不控制 read burst；控制 flit 注入（credit） | 粗粒度≤64KiB 拆成≤2KiB burst（`:240-255`） | burst splitter 可 1–256 beats 运行期配置；粒度大需 write buffer 足够大（`:289-305`） |
| 单小池做**部分后缀预约** | 可**表达**：decoupling FIFO 或整传输 SRAM buffer（`:329-332`），但在文本中无任何按目的覆盖的预约策略 [I] | 可**表达**：单 credit counter + 单 data queue（`:22-24`），但无「目的翻译就绪前缀 p」概念 [I] | 结构最接近：每 burst 元数据 + PFN 分组 + 有序 reissue，但机制是 **drop+retry**（`:88-89,386-392`），在不丢/不重试合同下需改名 [I] | 不能：**无读负载缓冲**，R 只做 pass-through（R.last gating）（`:301-302`）；write buffer 只 2 AW + 1 碎片写 burst（`:350-354`）→ 无法替读 burst 保后缀 [I] |

**结论（文献事实层面）**：四篇里，只有 **socdma2018** 显式维护「burst 级 src→dst 记录 + 按翻译页分组 + 有序放行」；**ctc2009** 显式维护「消息级依赖/credit 与 tail 结束」。iDMA2024 与 AXI-REALM 处在「地址/length/ID/priority/retry/监控」层，无 copy 级源→目的 payload 依赖。

---

# 3. Q1 逐项原始证据（file:line + 必要完整段落）

## 3.1 显式传递 **copy/job 级源→目的依赖或可排空语义** 的最强前作

### (A) socdma2018 = MMU-Aware DMA Engine（ICCD 2018）— **最强直接前作**
`out/paper-driven-innovation-20261008/papers/socdma2018.txt`

> `:404-407`：An entry in the buffer contains all metadata required to uniquely identify and reissue a burst: cluster-external and -internal address, length, AXI ID, DMA transfer ID, and whether it was a read or a write.

> `:412-417`：The retirement buffer must keep the order in which bursts were issued (because AXI bursts with the same ID are ordered) and must be able to complete bursts with different AXI IDs in any order. For these reasons, the retirement buffer can not be a simple FIFO queue.

> `:396-401`：To the DMA control unit, the retirement buffer reports the current number of in-flight and failed bursts and provides the metadata of the next reissuable burst. When at least one burst has failed, the control unit stops issuing new bursts from its queue and waits for all in-flight bursts to complete. Once there are no more in-flight bursts, the control unit reissues bursts as soon as they are reissuable until there are no more failed (and in-flight) bursts.

> `:409-417`：Upon such a read, the retirement buffer marks all 'failed' bursts with the same page frame number as 'peeked' (so that the same page is not reported twice). Meanwhile, the PE determines the missing physical address and adds it to the TLB. When it is done, it writes the handled virtual address to the same DMA register, upon which the retirement buffer marks all 'failed' or 'peeked' bursts with the same page frame number as 'reissuable'.

> `:396-397`：An AXI burst may not cross a page, so each burst requires exactly one TLB entry at the instant its request arrives at the IOMMU.

> `:386-392`：To guarantee data integrity, all TLB entries required for the completion of a transfer (which can touch up to 17 4 KiB pages) had to be locked before the transfer could be programmed... As the TLB is shared by multiple clusters, this limited the number of DMA transfers that could be enqueued at a given time and substantially reduced the effective data transfer bandwidth.

**[L]**（`docs/paper-driven-innovation-20261008.md:20`）：「MMU-aware DMA，ICCD 2018 ... 记录 burst 元数据，TLB miss 后排空在途，再按序重试；降低完整数据缓冲需要 ... 是直接、强先行工作。区别只能建立在明确的『无故障重试/不中断有效传输』合同及成本上」。

**判读**：这是把「源 burst、目的 burst、翻译页、顺序、可放行」放在**同一 job 级结构**里的直接前作。**但它基于 drop+retry（`:88-89` 混合 IOMMU signal miss 并 drop 交易）**，与「不丢已接受请求 / 无 TLB miss 重试」合同不同。

### (B) ctc2009 — **消息级依赖 + tail 结束 + connection 持久**
`out/paper-driven-innovation-20261008/papers/ctc2009.txt`

> `:166-169`：Indeed, the causality relations among pairs of messages can be modeled as a partial order relation ≺ over the set of all possible messages that are transferred in the network.

> `:382-402`：When a producer NIs needs to initiate a connection towards a consumer peer NId, it first sends a request packet called P_REQ... The P_REQ packet also indicates the total size of the message to be delivered and some additional information that can be used by the NI (i.e. for priority decisions)... The difference is that the first P_ACK sent by NId actually initializes the output credit counter of NIs so that it can generate and send a specific amount of data.

> `:481-487`：To avoid loss of performance and reduce contentions, a producer node that runs out of credits terminates the injection of the packet by tagging the flit using the last credit as packet tail. In this way the path along the network is made available to other possibly blocked packets. Note that the CTC connection is not terminated until the delivery of all the flits of the message is completed.

> `:22-24`：a single credit counter and a single input data queue are shared among all possible communications.

**[L]**（`docs/paper-driven-innovation-20261008.md:16`）：「按消费归还 credit；credit 耗尽可结束当前 NoC packet、释放路径，逻辑连接继续 ... 共享 credit/解除返回路径占有本身不新；其 packet 可以切断，外部 AXI 已接受 burst 不能直接照搬」。

**判读**：CTC 有「消息总大小 → 连接 → credit → tail 结束 → 逻辑连接继续」的完整 job 级合同，是「按消费释放返回路径」的最强经典。**没有**源→目的 payload 依赖，也没有目的翻译就绪概念。

## 3.2 只有 **地址 / ID / length / priority / 重试元数据** 的（不构成 copy 级依赖语义）

### (C) iDMA2024
> `:151-153`：the back-end accepts a 1D transfer descriptor specifying a source address, a destination address, transfer length, protocol, and back-end options

> `:234-236`：The descriptors consist of a src_address, dst_address, transfer_length, and a run-time backend_configuration

> `:277-279`：Our current error handler can either continue, abort, or replay erroneous transfers. Replaying erroneous transfers allows complex ND transfers to continue in case of errors in single back-end iterations without the need to abort and restart the entire transfer.

> `:294-297`：Otherwise, splitting happens at page boundaries, the maximum burst length supported by the protocol, or user-specified burst length limitations. The source and destination protocols' requirements are considered to guarantee only legal transfers are emitted.

> `:329-332`：Fully buffered operation may be required depending on the system and the memory endpoints; in this case, the small FIFO buffer in the dataflow element may be replaced with an SRAM-based buffer, allowing entire transfers to be stored.

**判读**：descriptor + error replay + legalizer，**仅有地址/length/协议/选项/错误重试**。legalizer 知道源与目的两端的页边界 [I]，但原文**没有**任何「按目的覆盖做后缀预约」的策略或依赖接口。摘要把「小 FIFO / 大 SRAM 取舍」明确讨论（`docs/paper-driven-innovation-20261008.md:23`）——即**普通整传输缓冲**是对照，非本候选。

### (D) AXI-REALM 2025
> `:289-302`：the burst splitter accepts incoming burst transactions and splits them to a runtime-configurable granularity, from one to 256 beats... Write responses of the fragmented bursts are coalesced transparently. Read responses are passed through, except for the R.last signal, which is gated according to the length of the original transaction.

> `:340-354`：We prevent this behavior by storing the fragmented write burst in a write buffer, Figure 2b. The buffer forwards the AW request and the W burst only if the write data is fully contained within the buffer. The transaction buffer is configured to hold two AWs and one fragmented write burst.

> `:496-497`：The LD table stores metadata such as tID, address, state, latency, and timeout, allowing detailed tracking of each outstanding transaction.

**判读**：tID/address/state/latency/timeout/stage 计数 + AW-W 绑定 + B coalescing + R.last gating。**无 payload 依赖、无读缓冲、无翻译感知**。

### (E) 地图里的翻译层调度/共享（只到 token/priority/批处理层）
- MASK ASPLOS 2018：warp token 控 TLB fill；**token ≠ 完成头实际占用额度**（`docs/paper-driven-innovation-20261008.md:18,99`）。
- Scheduling PTW ISCA 2018：同指令 walk 批处理 + PWC 估计（`:19`）。
- DWS HPCA 2021 / Orchestrated DAC 2023：walker 分配/借用、SM miss-rate 调度（`:21-22`）。
- BIS ASPLOS 2012：按线程等待周期识别瓶颈（`:17`）。
- 这些均**不携带 DMA copy 级源→目的 payload 依赖**。

### (F) 真串行地址依赖 ≠ DMA copy 依赖（避免混淆）
`docs/research-landscape-20261005.md:49`：IMPICA（pointer chasing）、DX100（不支持 pointer chasing）、Tiara（应用间接表非硬件 OS 页表）、NDS、GPS（publish-subscribe、弱内存合并写）、WIC（warp 级中断替代轮询）。这些是**访存层/同步层依赖**，不是 DMA job 的源→目的分段依赖。

---

# 4. Q2：四机制能否在给定合同下用同一小池做「普通部分后缀预约」

**合同（三不）**：① 不改变外部合法 AXI；② 不丢已接受请求 / 无 TLB miss 重试；③ 不缩短已接受读 burst。目标：对 R/W 异分段，用**一个小共享池**，把**不会等待本读返回之后才解锁的连续前缀 p** 直通排空，后缀 L−p 独占预约。

## 4.1 iDMA2024
- **能否**：**【可编程表达，未见策略】**。iDMA 有解耦 R/W 的 dataflow element（`:337-341`）、可按页/协议/用户上限切分的 legalizer（`:294-297`）、以及可替换为 SRAM 的整传输缓冲（`:329-332`）。因此「池 + 直通前缀 + 后缀预约」在硬件上**可表达**。
- **信息接口**：1D descriptor（src, dst, len, protocol, options, transfer_id）→ legalizer 产出合法 src burst 与 dst write；transport 层按字节流对齐/移位（`:342-351`）。
- **缺证据**：
  - 原文**没有**任何把「目的分段覆盖」传到 transport 层做 `p` 判定的接口或策略；legalizer 只保证合法性，不保证「目的页已就绪」。
  - 原文无翻译/TLB 概念，`p` 的来源（目的 PTW 就绪）不在 iDMA 接口内。
  - 无「按目的覆盖的 byte-credit / per-AR 预约」描述；只有 FIFO 或整传输 SRAM。
- **结论 [I]**：iDMA2024 提供的是**最强的普通整传输/小 FIFO 直通对照**，不是已实现的「部分后缀预约」；不能把 legalizer 能算边界当作已有策略。

## 4.2 CTC2009
- **能否**：**【可表达 tail 结束 + 逻辑连接持久，但无目的翻译 p】**。CTC 明确允许「credit 耗尽 → 以最后 credit 作为 packet tail 结束注入、释放路径、连接不终止」（`:481-487`），且单 credit counter + 单 data input queue 共享（`:22-24`）——这**正是「一个共享池 + 部分交付 + 路径释放」的最强经典形状**。
- **信息接口**：P_REQ{总消息大小, 优先级信息} → P_ACK 初始化 producer 的 credit counter；K = 每 P_ACK 的 credit 数；消费 K 个 slot 回一个 P_ACK（`:466-494`）。
- **缺证据**：
  - CTC 的「前缀/后缀」是 NoC flit 级的**消费 credit**，**不是**「目的翻译就绪」意义上的可靠前缀；它不知道目的侧哪个写段可排空。
  - **合同冲突点**：CTC 允许把 packet **在原长度前结束**（tail 标记提前）——这在 NoC packet 内合法，但如果映射到「已接受的 AXI read burst」上，就意味着缩短已接受 burst，违反本合同③。因此直接套用需**证明 CTC 的 tail 结束不缩短物理 AXI burst** [I]。
  - 无 per-segment 目的覆盖、无 PTW 就绪信息。
- **结论 [I]**：CTC 是「共享 credit + 尾结束 + 逻辑连接持久」的强对照；在本合同下不能直接充当「按目的翻译就绪的后缀预约」。

## 4.3 socdma2018（MMU-Aware DMA）
- **能否**：**【结构最接近，但机制本身是 drop+retry，违反合同②】**。retirement buffer 已保存每 burst 的 src/dst/len/ID/RW（`:404-407`），按 PFN 分组（`:409-417`），并「停止发新 burst、等 in-flight 完成、再按原序 reissue」（`:396-401`）。
- **信息接口**：DMA 控制单元 ↔ retirement buffer（增/成功/failed）；retirement buffer ↔ PE 控制接口（读出首个失败外部地址，PE 补 TLB 后写回，标 reissuable）；retirement buffer ↔ AXI transfer unit。
- **缺证据 / 合同冲突**：
  - `:88-89` 原文：「Hybrid IOMMUs... signal the TLB miss back to the master and **drop the transaction**」——这是**丢请求 + 重试**，与本合同②直接冲突。
  - `:386-392` 原 SoA 是「先把整传输所需 TLB entry 全部 lock 再编程」——即**整传输翻译预留**，带宽受限。
  - 原文**无**「后缀独占预约 / byte-credit / 单池容量账本」；它只做「in-flight 排空 + 有序 reissue」。
- **结论 [I]**：socdma2018 提供了最完整的「目的翻译页分组 + 有序放行」接口前作；但它在合同②外。若把「drop+retry」改成「hold until ready（不丢）」，其 retirement buffer 结构可作**C 的元数据与顺序接口**——但这属于**我方改写，非原文已有策略 [U]**。

## 4.4 AXI-REALM 2025
- **能否**：**【不能**替读 burst 保后缀**}**。读响应只 pass-through，仅 `R.last` 被 gate（`:301-302`）；**没有读负载缓冲**。写侧 write buffer 只存 **2 AW + 1 个碎片写 burst**（`:350-354`）。
- **信息接口**：irealm（burst splitter + write buffer + M&R）与 erealm（DOTQ：ID HT 表、LD 表、R/W 表；stage 计数）——都是**监控/整形**元数据（`:484-529`）。
- **缺证据**：
  - 无池容量用于 payload；write buffer 尺寸是「够装一个碎片写 burst」，**不是后缀预约池**。
  - 无翻译感知；`p` 概念不存在。
- **结论 [I]**：AXI-REALM 对 (a)「先就绪再提交 W 顺序」有价值（AW/W 绑定 + burst splitter + write buffer），但对 (d)「读 burst 长度 + 目的翻译进度联动」**无负载侧机制**。其 burst splitter 明确把「粒度大 → write buffer 必须大」（`:297-305`）写成**权衡事实**，是 (d) 的重要对照。

### Q2 汇总判定
- **没有一篇在给定三不合同下已实现「同一小池 + 按目的翻译就绪的部分后缀预约」**。
- 可**表达**（不构成策略证据）：iDMA2024、CTC2009。
- **结构最近但合同冲突**：socdma2018（drop+retry）。
- **不能**：AXI-REALM（无读缓冲）。
- **最强普通对照**：socdma2018 retirement buffer 语义 + iDMA 整传输 SRAM/小 FIFO 对照 + CTC 共享 credit（三者取最强面组合，见 §5）。

---

# 5. 同合同最强普通 C 算法草案（不削弱对照）

**C 定义**：普通「准确 per-segment 直通池 + byte-credit + 目标写顺序感知 + 可排空条件」策略。**不**依赖任何新 token/新 VC；只用已可获得的信息（目的每段覆盖、accepted AR 的发出顺序、目的写段就绪）。

## 5.1 信息接口（C 需要的全部输入）
```
Descriptor/Frontend 已提供:
  src_addr, dst_addr, transfer_length, transfer_id          [iDMA2024:151-153,234-236]
Legalizer/前端计算 (源与目的协议边界均已知) [iDMA2024:294-297]:
  src_segs[]   = [(s_off, s_len, src_page)]                 顺序
  dst_segs[]   = [(d_off, d_len, dst_page)]                 顺序
  cover(dst_addr+d_off, n) -> 覆盖 [d_off, d_off+n) 的 dst_segs 子集
翻译层提供 (平台侧, 非 iDMA 自有):
  tready[dst_page] : 该目的页已有有效翻译(PTE返回)           [socdma2018:396-397 类比]
  write_commit_order[] : 目的写提交/消费顺序                  [AXI-REALM:340-354 类比]
accepted AR 表 (DMA 自有):
  ar_seq : accepted AR 的发出序号（当前信息）                [socdma2018:412-417 类比]
```
**注**：C **不**把瞬时 `WREADY` 当作排空证书（更正⑤）；**不**把合法 `RREADY=0` 当作返回路径已释放（更正③）；**不**把 MASK 式 token 当作完成头占用额度（更正②）。

## 5.2 核心算法（单池，per-AR 后缀独占预约）
```
状态:
  free : 池剩余字节, 初始 N
  per AR r: {r.L, r.p, r.tail, r.reserved, r.hold_seg[], r.state}

函数 reliable_prefix(r, dst_segs, tready, write_commit_order) -> p:
  # 只把「不会等待本读返回之后的翻译/数据来解锁」的连续目的前缀计入
  p = 0
  for seg in dst_segs (按目的偏移升序, 覆盖 r 的逻辑区间):
      if tready[seg.dst_page] AND can_commit_before(seg, write_commit_order)
            AND not depends_on_this_RLAST(seg):          # 更正③④⑤
          p += seg.len
      else:
          break                                           # 必须连续前缀
  return p

准入 (在发新物理 AR 前, 更正⑥: 空间不够不发):
  p    = reliable_prefix(...)
  tail = r.L - p
  assert tail >= 0
  if free < tail:      return WAIT_FOR_POOL           # 不发 AR, 不制造伪成功
  free -= tail ; r.reserved = tail ; r.hold_seg = 后缀段
  r.p = p ; 发出 AR (长度 = r.L, 不缩短未接受的未来 burst; 合同③只约束已接受 burst)

响应路径 (R beat, 偏移 o, 字节 b):
  if o < r.p:
      write_path(b)                       # 直通；可对 R 合法反压, 但不假设路径释放
  else:
      pool_store(r, o, b)                 # 该区间已由 r.reserved 独占, 不再分配
  if RLAST(r):
      r.physicalDone = true               # 可释放返回服务路径 (socdma2018:396-401 类比)

消费/退休 (某目的写段 k 被消费, 且 dst 翻译已就绪):
  flush stored tail 段 k 到 write_path (按 write_commit_order)
  free += consumed_bytes_of_k             # 逐消费释放, 单池账本

r 结束: free += r.reserved - r.unconsumed_tail
```

## 5.3 p 变化规则（更正⑥）
```
降 p 请求 (例如目的翻译被 invalidate 或写顺序改变):
  delta = p_old - p_new            # >0
  need  = delta                    # tail 变长, 需要新增预约
  if free >= need:  free -= need ; r.reserved += need ; p = p_new    # 原子
  else:             维持 p_old 直到实际消费完成 (不缩减旧保证)
```
**不许**「只保守缩小 p 却不补资源」。

## 5.4 多 AR 账本与不变量
```
inv1: Σ_r r.reserved <= N                                    # 预约总和 ≤ 池
inv2: 一次前缀保证不得跨 AR 复用
inv3: AR 已承诺后, p 非增 必须伴随原子新增预约
inv4: 前缀写服务不以 r 的 RLAST / 后续 PTW 为前提（否则 p=0）  # 更正③⑤
inv5: accepted AR 的发出顺序 ar_seq 是已有信息, 用于判断同 ID 返回顺序
inv6: job/source/ID/权限/失效 epoch 属于接口合同, 不入池但必须随 r 保存
```

## 5.5 C 的平凡/退化点（必须报告，避免伪收益）
- 若每个目的翻译都已就绪或 `p = 0` → C 退化为直通/整预约。
- 若源 AR 与目的分段同步缩短已解决且额外请求可被流水隐藏 → C 可能只有缓冲代价。
- 若 C 在同准入/同容量/同成本下与 D 同决定 → 应停止 D 的机制创新推荐（`docs/paper-driven-innovation-20261008.md:115-117`）。

**[U]**：本节是**候选规格草案**，未实现、未综合、未测。

---

# 6. Q3 近邻清单（四个子命题）

## (a) translation-only 预准备后再承诺 W 顺序
| 来源 | 可追溯引用 | 本轮全文? | 覆盖点 |
|---|---|---|---|
| socdma2018 | `out/paper-driven-innovation-20261008/papers/socdma2018.txt:404-417,396-401` | **有** | burst 元数据 + PFN 分组 + 有序 reissue；先解决翻译再放行 [P] |
| AXI-REALM 2025 | `out/rule-novelty-check-20261007/papers/axi-realm2025.txt:340-354` | **有** | write buffer：W 数据齐备才放行 AW/W（W 顺序承诺）[P] |
| C&F (Restuccia & Kastner, TCAD 2022) | AXI-REALM 引 [29]，`:880-882`；URL 见 `:1141-1143` | **无全文** | 「hold back transactions until they can certainly be issued」= 就绪后提交 [L] |
| MMU-aware DMA 条目 | `docs/paper-driven-innovation-20261008.md:20` | 即 socdma2018 全文 | 直接强前作 [L] |
| RISC-V IOMMU in-memory queues | `docs/research-landscape-20261005.md:134` | 标准（非论文） | IOTINVAL/IODIR/IOFENCE；PR/PW 只覆盖已处理请求，in-flight 可能需互连 fence [L] |
| Preemptive VC, arXiv 2026 | `docs/paper-driven-innovation-20261008.md:28` | 作者 arXiv HTML（文中称已读） | DMA 内部 R→W 与共享 NoC 链路的等待环 [L] |

## (b) 以 copy 依赖作为准入信息
| 来源 | 引用 | 全文? | 覆盖点 |
|---|---|---|---|
| CTC 2009 | `ctc2009.txt:382-407,466-494` | **有** | P_REQ 携总消息大小 → 连接初始化 credit；按接收容量准入 [P] |
| socdma2018 | `socdma2018.txt:404-407,396-401` | **有** | 以 in-flight burst 数上限准入 [P] |
| C&F | AXI-REALM `:880-882` | **无** | 就绪前 hold back = 就绪语义准入 [L] |
| ABU / ABE | AXI-REALM `:867-879` | **无** | 预算/周期、名义 burst 上限 + max outstanding 准入 [L] |
| iDMA2024 | `idma2024.txt:219-227,277-279` | **有** | transfer_id/status + error replay（仅 transfer 级准入）[P] |

## (c) 决定 PTW 与 payload 服务对象
| 来源 | 引用 | 全文? | 覆盖点 |
|---|---|---|---|
| MASK ASPLOS 2018 | `docs/paper-driven-innovation-20261008.md:18,99` | 地图多为模拟；见 `research-landscape-20261005.md:81` | warp token 控 TLB fill（**token ≠ 完成头占用额度**）[L] |
| Scheduling PTW ISCA 2018 | `docs/paper-driven-innovation-20261008.md:19` | 地图含 | 同指令 walk 批 + SJF + 防饥饿 [L] |
| BIS ASPLOS 2012 | `docs/paper-driven-innovation-20261008.md:17` | 地图含 | 按线程等待周期优先瓶颈 [L] |
| DWS HPCA 2021 / Orchestrated DAC 2023 | `:21-22` | 地图含 | walker 借用/租户分配；SM miss-rate 调度 [L] |
| cuPTW 2026 | `docs/shared-translation-deepening-20261006.md:27`；`docs/paper-driven-innovation-20261008.md:27` | 全文（POMACS/SIGMETRICS 2026） | idle CU 执行 walk；MSHR 饱和时 L2 TLB 转发 [L] |
| SoftWalker MICRO 2025 | `shared-translation-deepening-20261006.md:27` | 全文（机制部分 Accel-sim） | PW warp + In-TLB MSHR，最高调度优先级 [L] |
| socdma2018 | `socdma2018.txt:88-89,396-417` | **有** | TLB miss 时对 in-flight 排空 + 有序 retry（PTW 与 payload 服务次序）[P] |

## (d) 联合改变读 burst 长度 + 目的翻译准备进度
| 来源 | 引用 | 全文? | 覆盖点 |
|---|---|---|---|
| **socdma2018** | `socdma2018.txt:396-397` + `:240-255` | **有** | 「burst 不跨页 ⇒ 每 burst 需恰一个 TLB entry」= **显式把 burst 长度与翻译 entry 数联动**，最强直接前作 [P] |
| AXI-REALM | `axi-realm2025.txt:289-305` | **有** | burst splitter 1–256 beats；粒度大 → write buffer 必须大（burst 长度↔缓冲代价）[P] |
| iDMA2024 | `idma2024.txt:294-297,313-315` | **有** | legalizer 按页边界/协议最大/用户上限切源与目的；AXI4 = 256 beats 或 4kB [P] |
| Vogel TECS 2017 / TPDS 2017（socdma 之 SoA） | socdma2018 ref [7],[8] `:538-542` | **无全文** | 先设并 lock TLB entry 再编程 DMA [L] |
| NB-Walker / CATO MICRO 2026 | `shared-translation-deepening-20261006.md:106-107` | **无全文**（题录） | 非阻塞 NUMA GPU PTW / 争用感知翻译卸载 [L] |

**未找到 ≠ 新颖**：以上仅是本轮材料覆盖情况；`docs/shared-translation-deepening-20261006.md:9` 明示「本批没看到只表示本批证据未覆盖」。

---

# 7. 待查论文清单（题名 / 年份 / URL / 本轮全文）

| 题名 | 年 | URL（来自所附文档） | 本轮全文? |
|---|---|---|---|
| Cut and Forward: Safe and Secure Communication for FPGA System on Chips (Restuccia & Kastner) | 2022 | AXI-REALM 引 [29]，`axi-realm2025.txt:1141-1143` | **无** |
| AXI-ICRT: Towards a Real-Time AXI-Interconnect for Highly Integrated SoCs | 2023 | `axi-realm2025.txt:1154-1157` | **无** |
| A Bandwidth Reservation Mechanism for AXI-based Hardware Accelerators on FPGAs (ABU) | 2019 | `axi-realm2025.txt:1150-1153` | **无** |
| Is your bus arbiter really fair? (ABE) | 2019 | `axi-realm2025.txt:1101-1103` | **无** |
| AXI HyperConnect: A Predictable, Hypervisor-level Interconnect for Hardware Accelerators in FPGA SoCs | 2020 | `axi-realm2025.txt:1164-1167` | **无** |
| Lightweight virtual memory support... pointer-rich data structures (Vogel et al., TPDS) | 2017 | socdma2018 ref [7]，`socdma2018.txt:538` | **无** |
| Efficient virtual memory sharing via on-accelerator page table walking (Vogel et al., TECS) | 2017 | socdma2018 ref [8]，`socdma2018.txt:541-542` | **无** |
| Preemptive Virtual Channels / waiting-cycle avoidance in DMA | 2026 | `https://arxiv.org/html/2607.01430v1`（`paper-driven-innovation-20261008.md:28`） | 作者 arXiv HTML（文中称已读），本地无转换文本 |
| ShadowUpdate（迁移失效传播新映射 + 在途请求） | 2026 | `shared-translation-deepening-20261006.md:103`（机构摘要） | **无**（摘要级） |
| LATPC（TLB prefetch / MSHR 压缩 / 批量 PTW） | 2025 | `shared-translation-deepening-20261006.md:104` | **无**（摘要级） |
| Clover（晶圆级页表副本存储/访问距离） | 2026 | `shared-translation-deepening-20261006.md:105` | **无**（摘要级） |
| NB-Walker / CATO（非阻塞 NUMA GPU PTW / 争用感知卸载） | 2026 | `shared-translation-deepening-20261006.md:106-107` | **无**（题录） |
| WaferTrans（分布式 PTE 存在性定位远端翻译） | 2026 | `shared-translation-literature-20261006.md:52` | **有全文**，未核正式录用 |
| RISC-V IOMMU in-memory queues（标准） | 2026 | `https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html` | 官方文档 |
| Linux HMM / SVA（标准） | — | `docs/research-landscape-20261005.md:136-137` | 官方文档 |

---

# 8. 不确定性与建议下一文件

**不确定性**
1. **[P] 出处的本地行号**属于「具体转换版本」（`docs/paper-driven-innovation-20261008.md:30`）。若 Codex 用其他 PDF 转换，行号需重映射。
2. socdma2018 是否为当前平台 iDMA legalizer 的直接前作，我只据「burst 不跨页」与「retirement buffer」做结构类比 [I]；需 Codex 核 CVA6 iDMA legalizer 是否同样 burst 不跨页。
3. q2 的「可编程表达」判定全是 [I]；没有一篇在合同②下已实现后缀预约。
4. C 算法中的 `can_commit_before` / `depends_on_this_RLAST` 需要真实的 AW/W/DDR 依赖证据；否则按更正⑤ `p=0`。
5. `docs/paper-driven-innovation-20261008.md:120` 明示本阶段「没有实现 D、没有测试/仿真/构建」；我没有执行任何命令，本答复**不含任何测试通过声明**。

**建议下一文件（Codex 优先）**
- `cva6/corev_apu/iDMA/src/idma_legalizer.sv`（尤其 `:82` 与 `:202`，确认「源/目的分段上限 2048B」与共同分段开关）与 `idma_backend.sv:811`（`RAWCouplingAvail`）——用于把 §2 表中 iDMA 的「可编程表达」落实为源码事实。
- Linux `drivers/misc/idma.c:419` 与 `tests/nonblock_actual_dma_test.cpp:1225,1715`——确认 CONF=0 现状与实验入口（`docs/paper-driven-innovation-20261008.md:47-51`）。
- 若要补 §6 待查项：C&F、AXI-ICRT、ABU、ABE、Vogel TPDS/TECS 2017、ShadowUpdate/LATPC/Clover/NB-Walker/CATO 全文——这些是本轮唯一能改变 [L]→[P] 判定的材料。
