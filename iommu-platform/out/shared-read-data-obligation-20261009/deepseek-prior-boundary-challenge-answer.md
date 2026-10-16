# §56 机制的最接近已发表覆盖、新颖性风险矩阵与有界下一步实验

> 证据边界声明：以下仅依据你提供的 5 份一次文献分线摘录 + §49/§54/§55/§56 根级设计/验收文本，**未联网、未重跑任何实验、未读取全文**。因此所有“未命中”都只表示“在这 5 份摘录及已引文献行内未见”，**不表示世界上不存在同类实现**；也不得把“普通控制器可任意编码”当作已有发表。§56 尚无运行结果（`codex-design-before-run.md` 仅为运行前设计）。RFC2887、SC24/arXiv:2408.13356v2 仅在 §49 内被二手引用（`codex-cross-field-boundary.md:10-11`），本次无其一次原文，故不作为一次证据。

---

## 0. 证据来源清单（可导航）

| 代号 | 文件 | 依赖行 |
|---|---|---|
| DMA-Latte | `out/shared-read-data-obligation-20261009/primary-literature-web-evidence.json` (2511.06605v3) | L76, L163-170, L180-182, L189-192, L308 |
| Torrent | 同上 (2512.17589v1) | L123-124, L129-134, L137, L140-143 |
| XBAR | 同上 (2502.19215v1) | L105-107, L119-124, L134-135 |
| SoCDMA | `out/shared-translation-literature-20261006/papers/socdma2018.txt` | L20-24, L88, L344-350, L393-401, L402-418, L553-569 |
| ODP | `out/shared-translation-literature-20261006/papers/rdmaodp2021.txt` | L176-186, L263-269, L349-361, L408-427, L628-640 |
| §49 边界 | `out/innovation-rescue-20261009/codex-cross-field-boundary.md` | L7-11, L15, L17, L21, L23, L25, L29 |
| §54 验收 | `out/shared-read-permission-frontier-20261009/codex-result-acceptance.md` | L5, L11, L14, L16, L18, L20 |
| §55 验收 | `out/shared-read-frontier-closure-20261009/codex-result-acceptance.md` | L5, L10, L12, L14, L16, L18 |
| §56 设计 | `out/shared-read-data-obligation-20261009/codex-design-before-run.md` | L3, L5, L7, L9, L11 |

---

## 1. 逐项机制的“最接近已发表覆盖”匹配

### M1 共享物理源 AR + 独立 READ 上下文
- **最接近：DMA-Latte bcst。** `primary-literature-web-evidence.json:167` “Improves link utilization? Yes (**1 read / 2 writes**)”；L169 “Lowers memory traffic? Yes (**Source read once**)”；L181 “by reading the source once for both destinations… reusing a read also lowers memory traffic”；L180 “specifying a **single source and two destination** memory locations”。
- **分布式版本最接近：Torrent。** L141 “the Torrent attached to the source memory is in **read mode**”；L142 “the data switch **duplicates the incoming data** from ④ into two copies… **on-the-fly data duplication with no temporary storage**”。
- **译址依赖最接近：SoCDMA。** L396-399 “An AXI burst may not cross a page, so each burst requires **exactly one TLB entry** at the instant its request arrives… Requests of consecutive bursts can arrive **back-to-back**… multiple TLB entries need to be present only for a **short time interval** for an entire transfer spanning multiple pages”。
- **判定**：**“源读一次、扇出多份”这一元件已被 DMA-Latte/Torrent 覆盖**；但都是“一条命令固定 2 目标/一条固定链”，**没有“一条长物理 AR 被多个独立 logical 读上下文/权限成员共享，且成员可独立退出”**。

### M2 每接收方“成功 WRITE 前缀”
- **最接近（分离完成）：SoCDMA。** L404-411 每条 in-flight burst 记录 metadata 并“report the **success or failure** of the burst with the responded AXI ID”；L412-415 “must keep the **order** in which bursts were issued (because AXI bursts with the same ID are ordered) and must be able to **complete bursts with different AXI IDs in any order**”。
- **最接近（按引擎独立同步）：DMA-Latte。** L181 “each DMA **independently synchronizes** with the CPU on completion”。
- **反例（显式 JOIN，非 per-recipient 前缀）：XBAR。** L119 “a B handshake is propagated **only after receiving a response from every slave**”。
- **判定**：**per-burst / per-request 的分离完成是已前作**；**“共享读组内每接收方的成功写前缀”在摘录中未见**。

### M3 A 到未对齐 frontier1，而 V 已 firstVALID qW2 领先 A qW1
- **最接近（跨接收方的 W 顺序危险）：XBAR。** L122 “slave 0 receives AW0 before AW1… must thus receive **all W0x transactions before any W1x**… slave 1 expects W transactions in the **opposite order**… we must **stall**”；L123 “This condition leads to a **deadlock**”。
- **最接近（可产生跨 ID 乱序的基元）：SoCDMA。** L414-415（同 ID 保序、异 ID 可任意乱序完成）。
- **最接近（共享译址 + 多独立 READ 的时序病态）：ODP。** L263 “Packet damming… **caused by READ operations**”；L419-427 “**READ operations from multiple QPs** cause simultaneous page faults followed by **update failure of page statuses**”。
- **判定**：**“跨接收方 W 顺序约束 + 有限缓冲导致 stall/deadlock”已有直接前史（XBAR）**；**“未对齐的 1-beat frontier1 与已被跨越的 qW2 并存”这一具体构造在摘录中未见**。

### M4 仅 FUTURE A logicalR membership 退出（旧 W/B 保留），且条件为 firstVALID A-W 数据已进入真实 DMA buffer
- **最接近（completion ⊥ retirement 的基元）：SoCDMA。** L402-403 “a retirement buffer that keeps track of in-flight bursts”，L404-407 条目含 ext/int address、length、AXI ID、DMA transfer ID、R/W 与状态 Free/**In Flight**/Failed/Peeked/Reissuable；L383-384 指出原引擎 “reported a transfer as **complete as soon as** it had received the final read or write response… **regardless of whether all bursts were successful or not**”。
- **语义相反的直接前作：XBAR。** L119 B-join all-or-nothing；L124 “force a master to **‘acquire’ all slaves at once**, breaking Coffman’s **wait-for** condition”，`aw.commit`（L121）。
- **“不等对方状态”最近的分布式控制：Torrent。** L132-134 Grant/Finish 反向传播；L133 “stores and forwards every received data frame… **as soon as it receives it**”；但 Finish 是 **join**（尾节点反向回传）。
- **最接近的“不等待本地状态结束”：ODP。** L177-186 “the client raised a page fault for a response and **discarded the response**… retransmitted a request… **regardless of the resolution of the page fault**. That is, the client does **not wait locally** for the page fault to get resolved”；L181-186 接收端 “**do not have to store any dropped packets**… reliability guarantees to leave them on the **sender side**”。
- **判定**：**“完成 ≠ 成功”“同 ID 保序 + 异 ID 乱序”的退休基元已被 SoCDMA 覆盖**；**“把读组成员资格从 B 退休中解耦、以 firstVALID 写数据驻留为门控”在摘录中未见**。这是 §56 目前**最强的残余候选**。

### M5 全数据已在私有 FIFO 的 A 新短 W 可在读退出期继续排空、推进下一个 realWRITE 父
- **最接近：DMA-Latte b2b。** L189 “loads of a **subsequent copy command can be issued without waiting for stores of a prior copy command** to finish, as long as both commands have unique source and destination buffers (**no data-dependence hazards**)”；L192 讨论并行度 vs b2b 的权衡。
- **流水前送版本：Torrent。** L133 立即转发。
- **§55 的同类约束：** `codex-result-acceptance.md:5` “权限前沿末拍必须已有 **global Wcoverage**”。
- **判定**：**“后续负载不等先前存储”原则已被 b2b 覆盖**；**“在读成员退出窗口内、由译址前沿门控地排空私有数据”是组合式新点**。

---

## 2. 已发表新颖性风险矩阵

| # | 机制部件 | 最近前作（文件:行） | 前作覆盖**类型** | 残余风险等级 | §56 相对增量 |
|---|---|---|---|---|---|
| M1 | 共享物理源 AR 扇出多份 | DMA-Latte L167/L169/L180-181；Torrent L141-142；SoCDMA L396-399 | **通用广播 / 源读一次 / 无暂存** | **高**（元件已覆盖） | 仅“多独立读上下文共享一条长 AR + 权限成员” |
| M2 | 每接收方成功写前缀 | SoCDMA L404-415；DMA-Latte L181；XBAR L119（反例） | **Per-burst 分离完成 + 跨 ID 乱序** | **中高** | “共享读组内 per-recipient 前缀” |
| M3 | 未对齐 frontier1 vs 越界 qW2 | XBAR L122-123；SoCDMA L414-415；ODP L263/L419-427 | **普通 R/W 排序危险 / 跨接收方 W 顺序 stall** | **中** | 具体未对齐构造 + 已被跨越的 qW2 |
| M4 | future-R 成员退出 ⊥ 旧 W/B 退休 | SoCDMA L383-415；XBAR L119/L124；Torrent L132-134；ODP L177-186 | **completion≠success / join / 不进本地等待** | **中低（最强候选）** | 成员资格门控 = firstVALID 写数据驻留 |
| M5 | 全数据私有的 A 短 W 继续排空 | DMA-Latte L189/L192；Torrent L133；§55 L5 | **后续不等先前存储（b2b）** | **中高** | 译址前沿门控下的私有 payload 排空 |

**矩阵整体读法**：M1/M5 是强前史（若单独包装会被 DMA-Latte b2b/bcst 直接覆盖）；M2 已被 SoCDMA 的分离完成覆盖掉“引擎/请求”粒度，只剩“读组成员”粒度；**唯一在 5 份摘录中反复找不到同构语义的是 M4**——即“读组成员资格退出”与“B 响应退休”解耦、且门控量是 firstVALID 写数据的**实际驻留**而非 B。这是本任务应聚焦的残余点。

---

## 3. 必须严格区分的四类“前作”

**(1) 通用 DMA 广播 / 接收端丢弃 / 后补修复 —— 已前作，不能作新点**
- DMA-Latte bcst 透明推断：L308 “runtime can safely infer if a broadcast is being attempted”，§49:7 已判定“透明发现同源作业、只读一次本身不能作新点”。
- ODP 丢弃+重传：L181-186；L581-585 “To the best of our knowledge, our work is the **first to experimentally analyze the retransmission and timeout** of packets derived from page faults”。
- Torrent Chainwrite cfg/grant/data/finish + 端点分流 + 无暂存：L129-142，§49:9 已判定 “Endpoint fork、无整段暂存均不是本工作发明”。
- SC24 ring staging/bitmap/repair 与 RFC2887 慢者退出+补传：§49:10-11（**本任务无一次原文，仅二手引用，不能强化为一次证据**）。

**(2) 普通共同 R/W tiling —— 是控制，不是算法新颖**
- §54:11 “mode2 普通共同按成功页截断”；§54:14 “mode0 更强普通共享首段→V 单独后段→A 补段”；§55:10；§56:11 “普通共同 pagecut 对照可把两路 W quantum **同时对齐最小成功 WRITE end**…它**可能增加 V 一个 W 子请求**”。
- **要求**：§56 的对照**必须**给出这个“共同 W 边界”强普通版本（而不是固定 q2 对照），否则等于用自造自锁做对照（§56:11 明确警告）。

**(3) Per-burst retry + 分离完成 —— 已前作**
- SoCDMA L402-418（retirement buffer、free/in-flight/failed/peeked/reissuable、同 ID 保序、异 ID 乱序）；DMA-Latte L181；ODP L177-186。**“退出重试 + 完成与成功分离”不能立为独立新颖点。**

**(4) 译址相关 AXI 依赖 / 接口贡献 —— 最近的“接口级”前作，也是唯一可对标的贡献形态**
- SoCDMA L350 “Our implementation uses **one bit in the AXI 4 user field**”；L393-394 “the DMA engine can keep track of bursts that missed in the TLB and **restart them** after the miss has been handled”；L396-397 “An AXI burst **may not cross a page**”。
- XBAR L121/L124 `aw.is_mcast` / `aw.commit`（全目标 acquire 语义）；L134-135 面积/时序：13.1 kGE / 45.4 kGE（9 % / 12 %），16-to-16 仅 6 % 降频。
- **判定**：**已有“译址依赖的 AXI 接口扩展”（SoCDMA 的 AXI user bit + retirement buffer、XBAR 的 is_mcast/commit）**。§56 若要走接口贡献路线，**必须落在与 XBAR `L119` join 语义相反/互补的具体握手**（per-member retire 独立于 B），而不是复述“DMA 能感知 TLB miss”。

---

## 4. 什么才算“中等会议级别的独立架构贡献”

鉴于 §54:16 强普通对照已把优势压到 **19 cycles**，§55:14 压到 **33 cycles / 一个 AR**，且 §55:12 记录 common pagecut “0 rotation”，**单靠“成功前沿暂退补读”或“读/写前沿分离”已不足以命名独立算法**（§54:16 已明确）。要构成独立的、可在 moderate venue（workshop / DATE-FPL short / IEEE Micro letter / AXI 接口提案类）站住的架构贡献，需同时交付以下 5 项中的**至少 3 项，且第 1 项必选**：

1. **（必选）形式化合同 + AXI4 接口扩展**：AW/R 上的 membership 标注、per-member `retire` 握手、把 B 从 XBAR:119 的“全 slave 齐备才回”改为 **per-member B 或 join+early-retire**；并给出不变式：成员退出后禁止任何未来 R 路由到该成员；全部 firstVALID 写数据须已驻留在成员真实 DMA buffer；已发 AW/W/B 保持原 ID/顺序（对齐 SoCDMA:414-415 的保序约束）。
2. **一般性反例 + 最小修复**：把 §55:16 的 F1（V 旧 q2 W 在 A 旧 1-beat W 之前，V 需下一 R1 才 WLAST，A 的 B 因此等 V，当前 member 含 A 却不许 R1 超过 A 权限，退组又等 A 的 B）作为**窄合约的合法反例**，证明新规则在一般 q/W 边界下消除依赖环，**且不增加 W 事务数**。这是“counterexample + minimal fix”级贡献；若新规则会多出一个 V W 子请求（§56:11 的隐患），则降级为工程修正。
3. **成本可信**：面积/时序 vs XBAR 13.1/45.4 kGE、9/12 %（XBAR:134-135）；状态 vs SoCDMA 64 B/16 KiB（SoCDMA:560-569）。§49:23 已要求计 raw-parent metadata、PA recipes、children/B 汇合、skid、held headers 与 iDMA FIFO，**不能只报新增计数器**。
4. **证明最强普通控制达不到**：给“共同 W 边界对齐最小成功 WRITE end”的普通 pagecut（§56:11）在同信息、同资源、同 W 事务数下**不能**实现同等退出语义；否则必须放弃独立算法命名。
5. **长 R 保持的微架构证据**：真实 RTL 在**同缓冲、完整 metadata 成本**下保住长物理 R，而对照需更多 physical children（§49:17）。

**风险提示（须 Codex 裁决）**：§49:25 要求明确 adapter 位于译址前/后并据此计 PTW；§49:29 指出两流合计仍写 2N beat、q 写段可能放大 AW/B 或译址开销、第三方公平性未测、大默认 FIFO 会消除结构循环（故 B3 只证明有界缓冲场景）。这些都属架构判断，超出本 worker 范围。

---

## 5. 一个有界的下一步实验（超出“小 AR setup saving”）

**命名**：`F1-membership-retire-vs-B-retire`（在现有 §56 out-only fixture 内增量，不改接口层）。

**输入（沿用 §56:3 的既有参数，不新造 payload）**：
- A dst offset **4088**、2 KiB copy；**actual A 第一 WRITE parent 仅 1 beat**，第二 255 beats；V 原父 256 beats，q2 W；全部权限成功，V 为全长 WRITE sponsor（§56:3）。
- 触发 F1：V firstqW2 位于 A firstqW1 之前；V 需下一 R1 才能 WLAST（§55:16）。
- 对照组（**必须全给**）：(a) 旧规则（预期复现 F1 依赖环/无进展）；(b) 新规则（firstVALID A-W 数据全部驻留真实 DMA buffer 后清 A future-R membership）；(c) **强普通 common pagecut 且共同 W 边界对齐最小成功 WRITE end**（§56:11）；(d) 普通串行独立读（§56:5）；(e) prefix/suffix（§54:14）。

**观测（逐拍，必须区分计划值与实际值）**：
- A membership 状态；A firstVALID W 数据的真实 DMA buffer 驻留量；A 首 rawB 退休时刻；V 的 W/B 与 RLAST。
- **return 计数必须是“实际被 DMA 接受的 logical R”**，`raw_written` 是“job 全局已交 W”；**不得以计划 R 或 internal offer 充当已有 payload**（§56:9）。
- source R beats / AR 数 / 每目标实际 W256 beats（对齐 §54:3、§55:3 的根核方式）。
- `promised_end`、`next_child_end` 由**现有** qwriter parent 地址/offset/count 计算，**无新 payload**（§56:9）。

**验收（全部满足才算通过）**：
1. 新规则下 V WLAST 完成、A 首 rawB 在 V RLAST 之前退休；
2. 新规则总 W 事务数 **≤** 对照 (c)（若等于或更多，则记录“无独立算法”并回退）；
3. 无任何“需要未来 A R”的未承诺 W 被接受；
4. 全部 firstVALID held 稳定、原父 ID/RLAST/B 不变、完整 target + 未写区 + actual 拍数一致（§56:9）；
5. 记录“全数据已在私有 FIFO 的 A 短 W”是否真实排空并推进下一个 realWRITE 父（M5），验证 `next_child_end` 是否前移。

**边界（遵 §55:18 安全回退）**：本实验**保持单 child**；扩到两 child 需要先解决 held 物理 R 与 V 旧 W 队首的两层环，**不得声称 RR 保证重入**；安全 fallback 是 V 继续到 RLAST。故障/动态失效/多作业/all-member pause 不在合同（§56:11）。

**为什么这算“超出小 AR saving”**：它把测量目标从“省 AR 数”改为**“成员资格退休是否真的先于 B 退休、且不多写 W”**，直接检验 M4 这一唯一残余候选；若对照 (c) 同等或更优，则 M4 也应被判定为“已被强普通控制吸收”，据此收缩或放弃接口贡献。

---

## 6. 未决与需 Codex 裁决

- 本 worker 无法核实任何运行；§56 未运行，F1 结论目前只是 §55:16 的审核推理，不是实测异常。
- SoCDMA/XBAR 的 read-path 细节（AR/R 的 demux/mux 与 join）**未在摘录中出现**（XBAR:L105 只明确 AW/W/B），故“读方向的 join 语义前史”证据不足，需 Codex 补原文后再判。
- RFC2887、arXiv:2408.13356v2 仅二手引用，不能作为一次证据（§49:10-11）。
- 是否把 §56 定位为“算法”还是“AXI 接口合同”，属架构判断，须 Codex 领导决断；本 worker 只提供 M4 残余候选与上述有界实验。