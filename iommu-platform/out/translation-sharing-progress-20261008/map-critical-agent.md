# §46 primary 关键审查：共享翻译结果与独立续走保护

2026-10-08。Codex 本席亲自推演、实读关键原文及固定版本代码，与 root/rules 交叉质疑。最终收敛到本合同的3项主要 primary：Barr、US7415575B1、NB-Walker。最初复核的 gem5 旧证据按 root 新发现的共享 hit/局部历史问题换成直接 QoS 原文；旧副本保留，不继续扩图。无 DeepSeek/Claude 调用，无数学模型/DUT/build，无 production RTL/Linux 修改。本稿不以作者代码推断尚未取得的 NB-Walker 论文全部保证。

**结论：共享结果与独立续走状态不必绑定为同一槽，也不必放进同一替换域。因为已有具体的“共享 fetch＋每请求信息＋共享有限容量上的 PTW 预约”实现，把这三项重新组合或给共享项加 pin 不能作为独立机制。** 共享与保护仍可能争用总字节、端口或 MSHR，但那是需要实际核对的资源，而非语义上必然的整槽冲突。本文没有发现一个强普通方法达不到的新成本点或进度命题。

对“所有 shared cache 都天然提供暂停保证”的泛化也应停止：cache 可重取记录、精确 checkpoint、已接受 PTE 的返回路由是不同对象。以下给出具体普通构造及其成本，不靠泛称 cache 可等价否决。

## 1. 三项直接 primary 与可导航证据

本地根目录：`/home/xvhaocheng/work/iommu-platform/out/translation-sharing-progress-20261008/primary/`。`sources.json` 固定每个文件 URL、commit、bytes、SHA256 和取得方式。代码行号为保存的原始文件行号；Barr txt 为 `pdftotext -layout`，PDF 页从1开始。

### A. Barr、Cox、Rixner，ISCA 2010

**Translation Caching: Skip, Don’t Walk (the Page Table)**。[作者原文 PDF](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)，本地 `translation-cache2010.pdf/.txt`，本轮只复核必要段。

- §3.2/§3.3，PDF p4，txt `260–296`：partial translation 以虚拟前缀作 key；最长前缀命中跳过上层；一项 path 可保存多个中间 PPN。
- §3.1.2，p4，`254–263`；§4.2，p5，`298–357`：split cache 隔开不同层；unified cache 动态共享容量；replacement 可考虑 reload 成本和层级，含 modified LRU/variable insertion point。
- 表1/2，p5，`362–385`：tag/data 分开计费，partial data40bit、tag9/18/27bit，path data120bit。它们不是本平台含 fault/上下文的完整帧成本。

**覆盖：** 复用某层/某前缀的进度、分层或统一容量、按重取代价选择。**不直接覆盖：** 多地址空间/version key 的完整当前架构语义、pin 住每个未完成请求的最低续走状态、每设备持续进度或同 bit 预算最优 frontier。因为本文是 cache 组织研究，不能把 LRU 的平均复用当 lossless checkpoint 保护。

### B. US7415575B1：共享 hit 与 requester-specific history 是明确前史

Peter C. Tong、Colyn S. Case，NVIDIA，**Shared cache with client-specific replacement policy**。申请/priority 2005-12-08，公开授权文本 2008-08-19。[原始公开全文](https://patents.google.com/patent/US7415575B1/en)、[原始 PDF](https://patentimages.storage.googleapis.com/04/36/70/d3ec7e828f841f/US7415575.pdf)。本地 `US7415575B1.html/.pdf/.txt`；16页。这里只作技术 prior 判断，不讨论专利法律状态。

- 任意 client 可命中任意 TLB entry：PDF p13，txt `695–700`；原公开 HTML 正文对应 lookup 叙述。限制的是 replacement subset，不是全部 hit 可见范围。
- Fig7：PDF p7 的 sheet6图；步骤702/706/708 收到实际 client ID，依 requesting client 决定是否更新 LRU-r；正文 p14，txt `744–771`（接 p14右栏 `705–709`），Google正文行 `505–509`。本席直接查看 p7 图，已保存 `US7415575B1-fig7.png`。
- 多个 restricted client 分别有 subset 与 LRU-r1/r2，subset 可重叠/不重叠；不限制 strict LRU：PDF p15，txt `816–837`，Google正文 `527–530`。
- Claim6 的 hit 更新条件和 claim8 的 translation 用途：p16，txt `859–875`。

**覆盖：** 同一 translation 可跨 client 命中，同时把局部替换历史绑定实际 requester，区别于 resident entry 的分区。**未直接给出：** 本平台树形 PLRU、所有对称分区的完整不干扰定理、fast/main 同拍 provenance 或 nested 续走 bit 成本。它还保留全 client 更新的 global history，不能把该全文误写成所有状态都已隔离。但 requester 控制局部 history 这一规则已有精确前史，普通将它接到真实 hit 事件不构成新机制。

### C. NB-Walker，MICRO 2026 作者 artifact v2

作者公开 artifact 标题 **NB-Walker: Non-blocking Page Table Walker to Enhance Address Translation in NUMA GPUs**；[官方 Zenodo v2，2026-07-22](https://zenodo.org/records/21488827)，DOI `10.5281/zenodo.21488827`。原文件已在 §40 取得。setup 指定 NB 分支 commit `8837da7dda5591837877d20d4ad51b042c988aad`。[作者仓库](https://github.com/redbird-arch/micro2026-nbwalker-artifact)，本次副本用该既有作者 repository 的只读 `git show` 取得，没有执行 setup/build/run。网页 raw 在本轮返回 cache miss，不能说本轮重新下载成功；本地精确文件和 SHA 可直接核。

固定原始链接前缀为 `https://raw.githubusercontent.com/redbird-arch/micro2026-nbwalker-artifact/8837da7dda5591837877d20d4ad51b042c988aad/`；本地前缀为 `primary/nbwalker/`，两者后面均接下表路径。

| 原始 source path | 行号 | 精确实现边界 |
|---|---:|---|
| `simulator/mem/vm/mmu/nbwalker/NBWalkerMMU.go` | `31–42, 95–115, 191–217` | transaction 含原地址/PID/当前PPN/level；可从 PWC 返回的部分进度继续；primary 等待时推进 secondary。 |
| 同上 | `314–340` | 每请求 block 作为 write.Info 发往 L1V；发送成功才清本地 secondary。没有免费删除状态。 |
| `simulator/mem/vm/nbwalker.go` | `7–17` | block 含 PID、Address、Level、PPNWithOffset/PPN、MsgID；源码将部分字段注明 CAM/debug，这不是本平台全部位数证明。 |
| `simulator/mgpusim/timing/caches/nbwalker/walkerStage.go` | `118–140, 194–210, 229–270` | 按 PID＋PTEBlockID 找共享 MSHR；相同子块 fetch 合并，其他 offset 单独发；entry 的 Requests 最多8。 |
| `simulator/mem/cache/walkermshr.go` | `17–59, 63–78, 106–120` | data 与 PTW 项共用同 entries 容量；查询还分 PTW 类别；IsPartialFull 对总 entry 数比较阈值。 |
| `simulator/mem/cache/mshr.go` | `12–28` | entry 的 Requests/OffsetBits 与数据、key 分开存在；waiter 不是零成本。 |
| `simulator/mgpusim/timing/caches/nbwalker/directory.go` | `173–179, 260–266` | 非 instruction 的普通 data miss 在剩余容量达到 numReservedPTWEntry 时停止准入。 |
| `simulator/mgpusim/timing/caches/nbwalker/cache.go` | `54–59` | 接收命令更新 PTW 预约量；未核其生成策略，不声称已证明最优动态分配。 |
| `simulator/mgpusim/timing/caches/nbwalker/bottomparser.go` | `118–147` | 同一返回数据按每个 waiter 的 PTE offset 提取；该子块全部返回后移除共享 MSHR，完成 waiter 仍在独立 transaction。 |
| `simulator/mgpusim/timing/caches/nbwalker/respondstage.go` | `47–61` | 各请求响应携带其原 write.Info，成功发送才移除独立 transaction。 |
| `simulator/mem/vm/mmu/nbwalker/NBWalkerMMU.go` | `558–580` | 返回队列满可 backpressure；用该请求 Address/PID/level 与新 PTE 数据恢复 continuation。 |

**强直接覆盖：** shared PTE fetch/response 与 private continuation 同时存在；可以把 continuation 移到 message/MSHR/response queue；共享容量可按 progress 类别预约。**真实未覆盖合同：** 这是固定4级 GPU 模型，所读路径没有当前 Sv39×Sv39 的 permission/fault/版本完整状态，也没有 per-DMA 的持续服务上界或 byte 精确面积分析。MSHR reservation 针对 PTW 与 data 两类，不能直接当各 DMA 都有公平性。没有取得论文全文，因此这些是具体实现证据，不是论文已给某定理的推断。

## 2. Codex 亲自构造的合法字段差异

稳定 mapping、无 A/D 写、共同 G-stage context/root，两个 DMA 分别继续自己的 S-stage walk。已读 S nonleaf 给出 guest table GPPN `T_A != T_V`；令其高11bit相同、低18bit不同。本平台 GPA41 的 G-root index 取这高11bit，故两个 walk 下一笔 G-root PTE 地址相同，并得到相同下级 G-table PPN `Q`。这是一份合法可共享的 partial result。

但是下一 G2/G3 索引由各自 `T` 的低18bit决定。`T` 来自已读的 S nonleaf，不是原 IOVA 的函数；不能把它归入免费 immutable C。两请求还可能有不同 S-level、Sglobal；F 阶段有各自 S-leaf权限。future fault 的 bad GPA 也需要原 target。**因此共享 Q 不足以合并整个续走状态。** 这不是新现象：NB-Walker 的 one fetch＋各自 Info 已直接展示同类结构；nested 只是要补足真实字段。

一种普通同效布局是：共享项 `P={G context/generation, top11 key, Q, valid, ref/protection}`；每请求项 `D_i={C/token, phase/levels, T_i suffix18, Sglobal/必要属性, P reference}`。如果借 P 的 key 重建完整 T，P 在这些 live D 引用期间必须受保护；如果允许 P 被驱逐，就把完整 T/必要 Q 复制回 D，或按相同重读合同 restart。不同请求的 suffix、fault 信息与返回身份仍单独计费。**不声称这是最小 codec；它只是完整可执行的普通构造。**

完整 same-key、same-context、same-generation 的请求可以进一步共用当前目标与 PTE 数据；各 DMA 的返回 token 和访问类型仍保留。不同 read/write 可共享原始 PTE 字节，再各自检查权限；不能复用第一个请求的最终成功 verdict 代替第二个检查。跨 context 的物理 PTE 合并要另证语义 key 与更新规则，本轮不把缺省 VPN equality 当许可。

## 3. 必须分清的三种保护义务

1. **可重取 cache 记录：** 静态映射下驱逐可以只增加之后的 PTE 读取，不必丢失已接受请求；若保证某工作上界，驱逐是否容许由该合同决定。
2. **已处理 checkpoint：** 要精确继续且不重读，就保护所有 live 字段，可分布在 D/P 中；若合同允许子walk restart，丢一部分进度合法，但要计重读，并给足持续推进的 quantum/服务。§45 的 root-repeat 反例不能靠 cycle fairness 自动消失。
3. **已接受且未退休 PTE：** 必须保留其 response→request 路由/身份或等价可证明的关联，不能把不能撤销的响应当 cache miss 静默丢失。能合法接收后再 restart 不等于可提前抹去 route。此处必须按实际 ID/tag 路径核，不人为制造 result-ready 背压。

因此“某 cache 项被 pin”可以保护共享数据，却不自动保护各请求剩余目标。反过来，完整 private D 允许 cache 全部变为 best effort；缓存没有必然进入每次暂停的 correctness 环。NB-Walker 将返回关联保存在每请求 Info/transaction，正是这类拆分的直接实例。

## 4. 强普通控制与真实成本

最强比较至少允许以下具体控制，不只固定均分 cache：

- **独立 frame＋共享 best-effort partial cache：** 每个接受请求保留合法 checkpoint，PWC 任意驱逐只改变性能；有限请求、有限 memory service 与满足 checkpoint gap 的持续调度下可完成。这个条件并非任意无限到达的 latency 保证。
- **one shared record＋per-request suffix/route：** 上述 P/D 布局；有 live reference 时普通 ref/pin 保护，最后引用释放后成为可驱逐 cache。可按 total byte 分配而非强制每逻辑对象整占一槽。
- **有限共池＋progress reservation：** 与 NB-Walker 一样先保护必须保留的类别，剩余容量给 optional reuse；加入每 DMA 准入和已接受请求的持续调度时，其费用也计入。本文不把加入普通公平规则说成既有作者已给定理。
- **允许合同内 restart/relookup：** 若候选可以丢缓存并重走，控制同样可以；若禁止重读，两者都要保存必要状态。不能一边用 cache refill，一边要求普通永久保留整个 walk。

成本必须含所有存放位置：active/parking D、共享 P 的 data/key/context/version、ref/waiter与位图、MSHR route、消息/返回队列、终端结果/fault、lookup端口与传递时间。NB-Walker 把状态发出去不是消灭状态；Go pointer、slice header 与源码的 CAM 备注也不能直接当 bit/LUT 面积。固定 mapping 允许简化 generation，但两方要同时简化；允许同 key 请求共享 C 中某些不可变值也应同时允许。

共享 P 与 private D 可能物理共池：它们争总字节但各自保护不同字段；也可能分槽/不同 SRAM。**只与一个被固定切分且不能借容量的普通实现比较，无法证明新机制。** 若候选的唯一动作是以 ref/pin 在 shared P 与 separate D 间安排字段，以上具体普通布局已有同一字段与合法恢复方式；必须再给独有决策或新必要/充分条件。

## 5. 本轮可审定命题与停止条件

可以成立的分析事实：partial result 的共享 degree 不等于完整 continuation 的共享 degree；依赖该 shared record 恢复的独立请求需要保存其 nonshared suffix/attributes/route，并保护共享依赖或承担合法 restart。这个结构事实直接由合法字段例和作者代码支持，**不足以作为新理论**。

如果 root 核出的真实同一容量/端口约束，使一个候选在同 key、same version、same fault/结果语义、同到达与持续服务、同总资源下，实现以上 split / dynamic P-D / reservation / restart 控制都达不到的成本点，或得到它们不能直接推出的完整组合必要/充分条件，则可以继续研究该差异。当前没有这个见证。三项 primary 没给完整 nested frontier 不是未解决性的证明。

如果 ordinary P-D alias/ref＋每请求 Info 复现候选全部保存、恢复和共享，停止立机制；如果差异只有 cache quota、pin 位、metadata 不计费或新加统一槽限制，停止。如果实际平台 cache 与 walker state 是分离资源，且没有共享 continuation admission 瓶颈，就不能把纸面整槽竞争宣称为当前生产可达。扩充并发能力可以另作合理设计，但不能先选一个阻塞组织，再用普通分槽修复形成新贡献。

**建议：本合同保留为正确的强对照规格，不开展 RTL 或性能扫描。** 先由 root 的真实接口证据确定竞争对象；若没有超过上述普通构造的差异，§46 应明确停止，不为正结果制造约束。

## 6. root 新发现的合法跨 DID alias hit：真实修复边界

root 直接核到：IOTLB tag 无 DID，fillmask 控制 allocation；PartitionLocalHistory 的 touch 由 resident way 归属选择。DID10/11 若合法共用 PSCID/root/VPN，A 能命中 V 的 protected resident way，并修改 V local PLRU。不同 PSCID/root 的旧 fixture 不覆盖这个事件；共享 SCID 的合法性由 root 已核官方规范，本席不重复查询或声称跑过此例。

这是具体可达的实现隔离缺口，但修复规则已有上节明确前史：把**真实 hit requester class**传到 history 更新入口，仅更新该 class 可分配且本次命中所在的 local partition；跨区 hit 可以返回结果，同时不 touch 外方局部历史。对称 disjoint 分区可由 requester gate 加 resident eligibility 组合实现，不需要把 DID 放进 translation tag 从而破坏共享。owner/fillclass 处理另有事件，不能用 main fill owner 冒充同拍 fast-hit requester。

若 local history 的更新只由本 class 的合法 hit/fill 触发，则固定该 class 的 resident occupancy 与自身事件序列时，插入外方 alias hits 不会改变此 history；这是普通条件不变量，不是全 IOMMU 时间隔离，也不覆盖会改变 occupancy 的外方 fill/invalidation、lookup 端口或 PTW/memory 干扰。强对照必须包含这个 gate，因此在同样 gate 后才比较候选的新增效果。

**因为原文已允许共享 TLB hit 并按 requester 分离 local replacement 信息，不能把 requester-qualified PLRU touch 称独立创新。** 它可以作为必要工程修复及新的同 context alias fixture；若修复后候选优势消失，停止该创新枝。没有新的完整组合边界或同成本不可复制的收益，不扩大论文主贡献。
