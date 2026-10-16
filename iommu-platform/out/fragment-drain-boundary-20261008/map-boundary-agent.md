# §41 前作排空合同与固定映射 copy 的成本边界审核

作者与执行：Codex 独立关键来源审核和机制分析。没有调用 DeepSeek 或 Claude；没有 build、运行 DUT、修改 production。2026-10-08。

## 判定先行

本轮没有找到让“根据静态 source/destination burst 边界选择最少 cuts”超出普通分段的真实平台约束。现有描述符、两侧地址偏移和 legalizer 限长已经给出所需信息；同信息的普通非均匀分段可以生成同一组合法 cuts。因为已有 iDMA 本身还提供共同 R/W 分段，不能把共同切分、按 burst 让路、完整 R 空间预约或完整 W 后提交作为新机制。

有用的剩余问题是更严格的**组合条件**：预算关闭后，到底哪个共享资源能被回收；已经接收的 R、已经承诺的 AW/W，以及还未发出但为排空 R 必须发出的 destination 请求，能否在一份有限工作量证书内完成。AXI-REALM 的单个 AXI 隔离状态机与这个 DMA 内部依赖不是同一份合同。如果能给出前作没有的必要／充分组合条件，或在同总 payload、metadata 和同暂停工作量下得到普通整段 R/W 预约、统一小 burst、非均匀共同切分均达不到的成本点，这才支持新的分析贡献或机制贡献；目前没有证明这些条件成立。

## 可导航的原始来源

| 来源 | 本轮直接核的部分 | 本地原文与精确定位 | 官方链接 |
| --- | --- | --- | --- |
| AXI-REALM 2025 扩展论文，14 页 | 1–256 beat 运行时拆分；W 整片缓存；R 直通但重构父 RLAST；预算与 progress/RR 条件 | `out/rule-novelty-check-20261007/papers/axi-realm2025.pdf`、`.txt`；§III-A1/A2 页4，txt287–304、306–351、355–356；§III-A3/III-B 页5，txt378–391、425–439 | [作者 arXiv PDF](https://arxiv.org/pdf/2501.10161)，[全文 HTML](https://arxiv.org/html/2501.10161v1) |
| 官方 AXI-RT RTL | head/tail isolation、地址握手扣预算、W/R 排空、父子响应 | 本目录 `map-agent-sources/`；下节列文件和行号 | [axi_rt 固定版本](https://github.com/pulp-platform/axi_rt/tree/50153a346b753dc2bc7723c446656a43db35d02d/src) |
| MMU-aware DMA，ICCD 2018 | 本地 SPM 合同；miss/error 返回后停发、排空、顺序重试；metadata 成本含义 | `out/shared-translation-literature-20261006/papers/socdma2018.pdf`、`.txt`；§III 页4 txt234–254；§IV-C 页6 txt374–412；§V-D 页9 txt537–561 | [作者 arXiv PDF](https://arxiv.org/pdf/1808.09751) |
| Arm DMA-350 TRM Issue04，2023 | 完整 R 空间预约、完整 W 后发、每 burst 仲裁；stop 与可恢复 pause 区分 | `out/translation-lane-check-20261008/papers/arm-dma350-2023.pdf`、`.txt`；§4.3.4 页22 txt992–1000；§5.8.1 页87 txt4538–4551；§5.8.2 页88 txt4580–4587；§4.8.2 页31–32 txt1439–1474；§5.7 页82 txt4230–4245；§6.2 页104 txt5483–5502 | [Arm 官方 PDF](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2) |

论文 txt 是既有 `pdftotext -layout` 版本，行内可能混合两个栏。上表页码是 PDF 的 1 起始页码；Arm 的印刷页码与 PDF 页码一致。不要只复制一行而忽略其所在栏。原文保留完整，可直接定位核查。

## AXI-REALM：实际关闭了什么，允许排空什么

官方仓库本轮固定为 `50153a346b753dc2bc7723c446656a43db35d02d`，提交时间 2025-03-27。锁文件中的 `axi` 依赖为 `fccffb5953ec8564218ba05e20adbedec845e014`，version 0.39.1。下载的是官方当前公开代码，并非已证明与论文实验完全相同的 RTL revision；本轮没有仿真验证。

下载目录：`out/fragment-drain-boundary-20261008/map-agent-sources/`。`sources.json` 保存每个文件的精确 raw URL、SHA256、大小；`axi-rt-main-commit.json` 保存官方提交元数据。没有执行这些下载源码。

| 源文件 | 行号与直接事实 |
| --- | --- |
| `axi_rt_unit.sv` | 197–219：head `axi_isolate` 设置 `TerminateTransaction=0`，任一 R/W region 预算耗尽触发 global isolate。226–272：splitter 后跟 write buffer。278–282、352–385：以尾部 AR/AW 握手的整个 burst bytes 扣预算。393–415：tail isolation 同样不返回错误；`isolated_o = tail_isolated \| isolated`。 |
| `axi_rt_unit_counter.sv` | 53–57、75–90：周期归零重新装入预算；预算为0或underflow触发 spent。停止的是后续地址接收，已提交事务仍有排空义务。 |
| `axi_isolate.sv` | 216–243：AW/B、W/WLAST、AR/RLAST 在途计数。254–255 默认连接所有通道。275–301、340–365：已出现但未握手 VALID 进入 Hold，不能撤回；Drain 阻止未来 AW/AR。318–323：W 只在已承诺 AW 对应的 pending W 存在时继续；B/R 在 Drain 中仍直通，RREADY 仍来自上游。303–314、368–378：对应计数归零后才切断响应。 |
| `axi_gran_burst_splitter_ax_chan.sv` | 91–125：保存父 burst 地址/剩余长度后接收父请求，生成限长 child；129–150：后续 child 被 tail 停住时父 metadata 仍保存。 |
| `axi_gran_burst_splitter.sv` | 316–374：R payload 直通，按父剩余长度重构 RLAST；不为父响应预留完整 payload 存储。 |
| `axi_write_buffer.sv` | 58–65：AR/R 完全旁路，RREADY 直通；94–123、148–176：见到一个完整 fragment 的 WLAST 后才允许该 AW/W 出口，即完整 W 后提交。 |

依赖 isolation 官方固定源码：[axi_isolate.sv](https://github.com/pulp-platform/axi/blob/fccffb5953ec8564218ba05e20adbedec845e014/src/axi_isolate.sv)。其它源文件使用上表固定 axi_rt commit；本地文件名与官方 `src/` 相同。

必须区分两个 quiet：

1. tail 的物理 child 都完成，没有共享输出在途事务；
2. head 的原始 parent 及其 response/route slots 都完成。

tail 隔离可以在剩余 child 尚未发送时达到第1项，保留 splitter 内父 metadata；`isolated_o` 使用 OR，不证明第2项。如果拟议释放证书只要求共享尾部让路，保留父槽可能完全合法，普通透明 splitter 就是强控制；如果要求回收父槽或 IOMMU 前的 route 状态，就必须逐项证明，不可引用这个 isolated 位代替。

论文明确 R 是直通而 W 有整片 buffer，并以互连 progress 和 RR 为前提。其每周期预算可用保证还以所有预算总量不超过互连／subordinate 每周期可服务量为条件。论文没有给出一个任意 streaming DMA 的“关闭全部新地址后，完整父事务必在不续预算的固定工作量内清空”的定理。上述差别是适用性边界，不证明论文评估错误。周期续额能继续服务，不能把临时不能完成 isolation 说成永久死锁。

一个只用于组合合同审核的小反例：一个 source 物理 AR 已接收，尚待返回 L 字节；DMA 可独立停车的空间为 B<L；为腾出空间所需的 destination AW 尚未在隔离点首次 VALID。现在同时关闭未来 AR/AW。RREADY 在 B 填满后降低；隔离允许旧 R 返回，却禁止那个未来 AW，W 也没有对应已接收 AW 可用于排空。正常、固定、合法映射并不消除这个环；destination 译址在稍后正常完成也不能穿过已经关闭的 AW gate。这个例子描述的是一种合法 master 与普通全局 gate 的组合，不声称现 production 已装有 AXI-REALM 或已经测得该轨迹。选择完整 R 预约、较小物理 R child，或在 gate 之前预先接收足够 destination 义务，都可以普通地打断它。

当前官方 splitter 的支持判断还需保留一个不确定项：`txn_supported` 的104–106以 `len >= len_limit` 直接返回 supported，而地址拆分状态机95–105以 `len <= len_limit` 判无需拆分，和前者的“未触碰事务”注释不一致。本轮不运行、不诊断该 guard 是否造成具体功能错误，因此不能用此 RTL 未验分支证明 non-modifiable burst 的新成本界。论文关于不支持 fragment 的请求报错的论述也不等于成功 copy 合同。

## 2018 与 DMA350：最强普通对照为什么不能省略

2018 的 miss 路径会返回失败响应、丢弃该访问，并让 DMA 保存 burst metadata；至少一片失败后停新片，等其余片响应，再由软件补 TLB 后按原请求顺序重试。其 DMA 一端是低延迟 local SPM，不是两个共享外部端点间仅靠小 FIFO 的 streaming copy。§V-D 的64B对16KiB比较是 miss metadata 与 IOMMU 内吸收8个失败 W 的数据缓存；它没有把256KiB local SPM 或可重读原始 payload 从系统合同中取消。因此它覆盖“metadata、排空与重试”思路，却不直接覆盖本轮的无错误、不中止 copy 内 pause 的全部数据保留成本。

这不构成新颖性的正证据。固定映射场景本来就可以普通地分片、保留读到的数据、在安全边界等待，再恢复原 copy。用人工 fault/error 来达到早 quiet 会改变成功合同；普通 fault/error completion 应作为合同分支列明，不可偷偷从成功 copy 对照中删掉，也不可与成功 copy 混算。

DMA350 给出了更直接的普通反证：AR 只有在 FIFO 能容纳**完整 R 响应**时才发，AW 只有在完整 W burst 已在 FIFO 内时才发；§5.8.1 还明确把这个规则与避免 R/W 环依赖关联。§5.8.2 每个 burst 后重仲裁。因而同一份完整 R 预约不能只在一个 burst 上重复使用、让多个在途 R 超额共享；控制必须按尚未返回的总 payload 预约。配合有限 outstanding，数据排空不必等待尚未发出的未来 destination AW。

DMA350 的 stop 会等待已在途 R/W 响应并取消命令剩余部分；pause 保留命令/trigger 状态并可以恢复。TRM 没有在 pause 的文字说明里单独承诺所有已在途总线事务已排净，所以不要把“立即 pause”当作本轮想要的完整释放证书。它的完整 R/W 发射条件与短 burst 则仍是可实现该证书的强普通资源设计。

## 只保留一个真实平台约束，及其否决力

唯一保留的约束是**本平台 iDMA 使用按 beat 前进的小 streaming FIFO，没有上述完整 R 空间预约**：

- `cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:254–257`：RREADY 由当前 mask 下的 byte FIFO 可写空间决定。
- 同文件 `343–360`：W 数据可发送后逐 beat 消耗这些 FIFO；已有数据量不必覆盖整个 W burst。
- `cva6/corev_apu/iDMA/src/idma_buffer.sv:33–50`：存储是每 byte lane 的有界 stream FIFO。
- `cva6/corev_apu/iDMA/src/idma_legalizer.sv:199–214`：`decouple_rw=0` 已按 source/destination 中更近的边界共同切 R/W；`decouple_rw=1` 才各走各的边界。

这个约束解释了为何当前 raw streaming copy 会形成跨 burst 的排空义务；它**没有**把最少 cuts 问题变成独有新机制，因为边界仍是静态已知，普通 coupled legalizer、额外 source cut 或完整 R 预约均能处理。若额外信息只是一份 descriptor，普通控制也应得到同一份信息。若候选只是将相同 cut 集合在不同模块生成，则决定、保证和成本一致，必须淘汰机制新颖性。

当前 source FIFO 的资源按有效 byte lane 和 masks 计，不能把 B 当作任意可交换的通用 payload pool；全局小模型必须保留 lane 和已接收片的实际占用。非整 copy 释放也必须明确是 physical child、translation route、parent response slot、还是 frontend busy；frontend idle 不是内部 pause 的替代观测。

## 下一份可审的最小判别规格

只做固定映射、正常 copy 内暂停的规格；不加 TLB 锁，不靠 fault/error 消掉 payload，不更改 production。根负责推导 H/B/cuts 成本界，本席的审核要求是把证书附在**已承诺工作**上：首次 VALID 已不可撤回的地址、已接收 R、已经接收 AW 的 W 数据／B、以及为完成这些义务必须生成的 future destination 都应计入。最大 safe-prefix 间隔 H 不能单独当作暂停之后的实际工作量；outstanding lookahead 与 AW 顺序义务可能跨过这个 prefix。

最小输入仍可采用源0/2048/4096…、目的0/1024/3072…边界，B<1024。先以纸面／小独立模型判四个强控制：

| 控制 | 必须提供相同的什么 | 应计成本 |
| --- | --- | --- |
| 普通共同／非均匀切分，按片暂停 | 同 descriptor、同映射、同暂停工作量、同有效 parking B | 两侧额外物理 burst、response metadata、额外 cuts；不是只比统一固定1KB方案 |
| DMA350 式完整 R 预约＋完整 W 后提交 | 同总 payload 和最大 outstanding；FIFO free-space 的预约不能重复 | 预约计数/标签；必要时将 R/W burst 缩小到资源可承受范围；多出来的 payload 全算 |
| AXI-REALM 式透明 child splitter＋完整 W buffer | 同样明确 tail quiet 还是 parent quiet | splitter 内父 metadata、完整 W fragment buffer、原 parent 占槽；R 通路缺的停车空间也必须算 |
| 仅停新 source、允许普通 destination drain | 同工作量目标和同资源 | 为排空 source 必須继续的 destination 链、已承诺后续 R/AW；不得把整 job 结束排除在观测外 |

如果任何普通控制在同总 payload/metadata 下复现同工作量证书与 cuts 成本，则不能立新的调度／分段机制。如果只是 full-R 大 buffer 用更多存储达同效果，仍须与在同 B 下缩小 burst 的普通控制比；只击败大 buffer 不足够。

可能保留的分析命题是：给出 DMA 与 regulator 组合的必要／充分释放条件，并确定这些条件的最小资源成本。这里必须证明“必要 future 请求能越过 gate”或“所有返回 payload 已有独立停车空间”，同时覆盖 held VALID、同 ID 响应、W 全局承诺顺序和 parent/child 层级。若可直接归约为常见完整响应预约／静态共同分段条件，就把它当作平台解释与实验控制，退出独立贡献路线。

停止条件：普通非均匀分段同效同成本；小物理 child 的完整 R 预约在同 B 下同效；只靠不续预算或人为 TLB 锁出现区别；pause 实际只在整 job idle 可用；真实 response/route slot 保留但被错误计作释放；或者收益来自已被§39普通物理旁路消除的通路差异。当前没有新 DUT 运行结果，也没有证明新的成本点。
