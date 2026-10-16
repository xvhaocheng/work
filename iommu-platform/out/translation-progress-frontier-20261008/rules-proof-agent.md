# §45 独立关键推演：有限 RR 完成、普通中间 checkpoint 与停止裁决

作者与核查：Codex 研究席亲自推演并核关键源码和两篇 primary；没有委托 DeepSeek，没有调用 Claude，没有修改 RTL，没有 build/run，也没有执行数学脚本。root 的有限图交叉检查另归 root；本文的结论是手工证明和明确受限的状态上界。

导航：[root 模型合同](codex-model-contract.md)、[§44 字段推演](../innovation-frontier-reset-20261008/continuation-critical-agent.md)。下文 §2 给四种 q 的判别，§3 给可实际编码的强普通中间点，§4 给 max-gap 命题的证明与边界，§5 给同预算 PWC，§6 给创新停止和唯一后续合同差异。

## 1. 固定合同与不能免费的状态

固定两个已经存在的有限翻译请求 A/V、一条可切换 PTW、静态正常 Sv39 S × Sv39x4 G、所有路径三层且最终 4KiB、DDT/PDT 已解析。以退休的 PTE 读取数计量，忽略 cycle、DDR 带宽和有限但不固定的 R 延迟。一次读取只推进当前请求；没有跨请求合并读取/扇出、PWC 或其它缓存跳读。

单请求的成功链是：

`G1 G2 G3 S1 | G1 G2 G3 S2 | G1 G2 G3 S3 | G1 G2 G3(final)`。

位置 p 是已完成的有效读取数，不是可免费保存的进度寄存器。原始请求和 resolved context C 双方同计且 immutable，包括原 IOVA、请求属性、S/G roots、context 身份与权限设置；不能把 T、Q、S-leaf 或 p 写入 C 来隐藏 progress。active 执行寄存器、owner/valid、terminal/fault 归属也必须计入总资源，本文 B 只比较额外 suspended frame，不能从它推导 LUT/面积。

两个请求均未完成时，每轮允许执行 1..q 笔，再真实交接。轮到某请求时必须执行选定的正长度 chunk，且每个已发出的正常读取最终响应。只写“最多 q 笔”并不给正服务/保存策略，不构成进度保证。另一个请求完成后，可以连续完成剩下请求。

保存点是 R 已实际退休、PTE 检查与下一阶段状态更新已经确定、下一 AR 尚未 first VALID 的边界。已 first VALID/accepted AR 不可撤销；本文静态步链把这一不可撤尾部退休后才交接。fault/result 已产生时必须先进入有计费且归属明确的 terminal 路径，不能在暂停时丢掉判决。当前 PTW 没有 A/D 写操作，因此本文没有隐含需重放的 atomic side effect。

字段符号：T 是当前 S table 的 guest PPN29；g 是已累积 S global1；Q 是当前 G table 的 physical PPN44；Y 是 G 叶完成后当前 S-PTE 所在 physical page PPN44；L 是 S leaf guest PPN29；a=(w,x,u,g) 四位是成功路径之后最终 IOTLB 内容所需属性。低 12 位和每层索引从 C 的 IOVA、T/L 和 level 推导。Sv39x4 的 GPA 是 41 位而非名称暗示的 39 位。

## 2. q=1,2,3,4：可保证什么

这里的“保证”指列出的确定策略在上述公平正服务和正常响应前提下保证两个有限请求结束，不指所有允许任意早交接的执行都结束。

| 策略 | q1 | q2 | q3 | q4 | 固定链的原因 |
|---|---|---|---|---|---|
| 每笔均保存完整 continuation | 完成 | 完成 | 完成 | 完成 | 每次正读取使有效 p 增加；每请求最多 15 笔 |
| 每次真实交接 whole restart，无其它保留 | 不完成 | 不完成 | 不完成 | 不完成 | 首个请求完成前，两者每轮都无法穿过 15 笔间距 |
| 只保存 S-semantic parent/leaf，交接时丢整个当前 G 子 walk | 不完成 | 不完成 | 不完成 | 选用 G+S chunk 即完成 | 下一 S 前缀进展需要 3G+1S 的连续四笔；final 需要三笔 |
| §3 的 G-leaf/S 两段普通保存 | 不适用 | 需增加其它点 | 完成 | 完成 | 把 4 笔块拆成 3+1，读完 S 后主动让路 |
| §3 的 G-root/G-leaf 普通中间保存 | 不适用 | 完成 | 完成 | 完成 | 可保存点的间距最多两笔 |

完整 continuation 对 q1..4 都是每请求 15 笔、两请求合计 30 笔；使用完整 quota 时，分别最多需要 15/8/5/4 个自身轮次。30 仅是这个“每笔只推进当前请求、无跳读/合并”的固定链下界。

纯 partial G restart 的最小可达非进展环可从初始 `I(j=1,G-root)` 出发：

* q1：A 读 G1、交接并丢 Q；V 读 G1、同样丢 Q；两者回到初态。
* q2：A/V 各读 G1,G2、交接并丢 Q；两者回到初态。
* q3：A/V 各读 G1,G2,G3，得到 guest S-PTE 的 physical 映射；若规则只保留 S-semantic target 而丢映射，交接后仍回到初态，下一轮再做同一个 G 子 walk，没有任何 S PTE 退休。

q3 的第三种环尤其说明：“三个 G 读取已经完成”不等于 S 前缀推进。A 一次，V 一次，再 A/V 各一次，就已经是两次完整重复打断；无限重复仍是 PTE 执行公平，但两个有限请求都不结束。

q4 只有采用 `G1,G2,G3,S` 这个不超过四笔的语义 chunk，S1/S2/S3 的 parent/leaf才依次推进，再用三笔 final G 完成。自身轮次读取数为 4,4,4,3，合计仍 15。若一个“max q4”的任意规则每轮只读 G1 就交接，仍可 livelock；max-gap 是存在可完成策略的判别，并非所有公平执行的证明。

## 3. 最强具体普通中间规则：q3 不需要只在 36/82 位间选

### 3.1 q3：完成 G，下一轮单读 S 后主动交接

每个 intermediate G 子 walk 一轮完成三笔，保存 `(Y,T,g,j,phase)`。恢复后直接由 Y 和原 IOVA 的 S 索引读 S PTE；完成 S 的检查/parent 更新后主动交接，即使本轮还剩两笔 quota。不要为了填满 q 而进入下一 G 后再丢进度。

单请求的交接位置可以取：

`0 → 3 → 4 → 7 → 8 → 11 → 12 → 15`。

自身各轮读取数 `3,1,3,1,3,1,3`，每请求 15 笔，两请求 30 笔，没有重读。最终 G 的三笔在一轮内完成，不需要保存它的中间状态。

对应固定三层成功链的六个内部记录状态：`S1, I2-root, S2, I3-root, S3, F-root`。三位 tag 足够区分六种 frame；初态由 C/valid 表示，终态走双方同计的 terminal。payload 是：

| 点 | 保存字段 | data 上界 |
|---|---|---:|
| p3，已完成首个 G，待 S1 | Y；T=原 S-root、g=0 可从 C/初态推导 | 44 |
| p4/p8，已读 S nonleaf，下一 G-root | T29+g1 | 30 |
| p7/p11，已完成下一 G，待 S2/S3 | Y44+T29+g1 | 74 |
| p12，S leaf 已检查，final G-root | L29+a4 | 33 |

于是统一 suspended frame 是 **74+3=77 bit 的实现上界**，另加真实 C、valid、token、owner、terminal、active 等共同成本。这不是 77 位必要性定理，也不是综合面积。若推广为任意 S leaf level 并允许 intermediate G 在不同层提前结束，可以使用九种 I/S/F×j 状态、四位 tag、完整 Y44，得到 **78 bit 的普通上界**；实际大页控制与 terminal 路径仍须另行核对，不能将此上界冒充本文已验证的全部 fault/动态路径实现。

“预设始终成功”的更弱合同还可以在 S 等待读取时丢旧 T，只留 Y44+g1；固定链为 **45+3=48 bit** 上界，一般 I/S/F×j 为 **49 bit** 上界。它是合法成功链控制，不满足未来故障输出相同的合同。若把 77 的 fault/缓存内容合同拿来限制一个 success-only 普通控制，就是不公平比较。

### 3.2 为什么真实 fault 合同不能随意丢 T

直接源码：`rv_iommu_ptw.sv:608–615` 的 S PPN 超过29位将 stage 改为 `STAGE_2_INTERMED`，`ERROR:639–644` 在该 stage 用 `gpa_x_q` 输出 bad GPA。对于 S **leaf** 的高 PPN 错误，leaf 成功路径没有更新原 intermediate gpa_x；对于 S **nonleaf**，`:525/:558` 已由返回 PTE 更新 gpa_x，不能将两者统一称为旧 target。

固定 C 下，两个不同 guest S table T 可以被 G 映到同一个 physical Y。刚完成 G 后，如果只保留 Y/g，两个 walk 的恢复状态相同；下一笔相同的高 PPN S leaf 错误却需要输出不同 guest-PTE GPA。因此，在要求当前 fault 输出完全保留且不重读/外存 target 的合同下，S 待读 frame 必须保留可区分 T 的信息。`Y44+T29+g1` 是一个直接的普通充分构造，不由此推出各宽度都对实际 DDR 地址域独立必要。

F-root 的 L/a 则可以完全采用 §44 的普通 aux-parent/leaf union。当前 IOTLB 内容同时保留 S1 w/x/u/global 与 S1 PPN，不能只凭本次访问的一个 allow/deny 位复现其后续 cache 内容；如果允许改变填充或跳过 fill，普通控制的压缩空间会变化，必须同合同比较。

### 3.3 q2：保留 G-root 的 returned next-page pointer

一个零重读普通策略不是每轮机械地读两笔，而是第一次只读 G1 后让路，保存 returned next-table Q；下一自身轮读 G2+G3，保存 Y；以后每次读 S+下一 G1，保存更新后的 parent/leaf 与 Q。

单请求路径：

`0 → 1 → 3 → 5 → 7 → 9 → 11 → 13 → 15`。

各轮 `1,2,2,2,2,2,2,2`，仍总计 15。I 的 root-return frame 保存 T29+g1+Q44（第一 S root 的 T/g 可从 C 恢复）；G-leaf frame保存 Y/T/g；final 的 root-return 保存 L29+a4+Q44=77。以固定链任意当前 p 的四位 tag 直接编码，**77+4=81 bit** 是简单普通实现上界。专用这条路径只保存 p∈{1,3,5,7,9,11,13} 七个内部记录状态，三位tag足够，因此还有 **77+3=80 bit** 的具体普通上界；初态/terminal/valid仍另行双方同计。两者都不是最优编码，本稿不以省一位作为贡献，也不为这一额外普通primitive扩大root已准备的有限图运行。

只保留 G1-return Q、却在 G2/G3完成后仍丢 Y，不能保证 q2 跨到 S：恢复后两笔额度刚好重新完成 G2/G3，然后又丢映射。需要保存 G-leaf，或让恢复后有足够额度接着读 S，或使用有计费且保护的 PWC。这个反例正好杀掉“有一个 partial checkpoint 就一定进展”的泛化。

## 4. max-gap 命题：接受受限版本，拒绝普遍下界

定义 S_B 为某个已固定 ordinary primitive、在预算 B 内能**完整独立保存当前状态**的 checkpoint 集合，加可由 immutable C 重建的0与terminal15。primitive 必须实际编码 phase/level，不能把数学模型的 p 本身当免费状态。允许 semantic fallback 时，它也只能回到同一 primitive 可完整表示的点。真实交接后，除了该 token 和 C，前一请求的所有未编码执行信息丢失。

令连续可保存点 `s_0=0<...<s_m=15`，`G(B)=max_i(s_{i+1}-s_i)`。

**受限命题：两个有限请求存在单调、不重读、按 RR 交接的完成策略，当且仅当 q≥G(B)。存在任何完成策略也满足同一必要条件。**

充分性：每轮只从 s_i 执行到 s_(i+1)，该段长度≤q；保存此点并交接。每次给该请求的正服务严格推进一个保存点，有限个点后 terminal。两个请求共读 30 笔，restart 无需使用。

必要性：设相邻保存点 a<b 且 b-a>q。在首个请求完成前，两个请求都会被强制交接。归纳假定某请求的已保存有效位置≤a；一次服务最多到 a+q<b，交接时无法保存任何 (a,b) 内的状态，只能回退到≤a。另一请求同理。因此两个请求都不可能第一次形成≥b的保存位置或越过该区间到terminal，首个完成不会发生。回退/重读不能生成不存在的可保存点，故不能救过大 gap。

这一证明的范围很窄：

* 它证明 root 五族模型中**存在完成路径**，不证明所有任意早停执行完成。确定策略仍须选上述单调 chunk。
* 它假定除了恢复 token 没有“留在 active 寄存器里的旧 flow 一半进度”。若另一请求可同时执行而旧状态仍留下，那就是额外共享存储/执行结构，必须计费并进模型。
* 它不涵盖两个 token 的联合编码、共享页表 prefix、protected PWC、coalesced PTE reads、合法跳 fill、request-only verdict、稀疏物理地址域或其它压缩。这些可能增加可保存点、减少字段或增加跳读边。
* S_B 从列明表示得来，不是从“任意 B bit 一定只能表达这些状态”得来。当前 77/81/82 都是具体上界。没有输出可观察性和信息下界证明，就不能把 max-gap 图结果扩成总资源最优 frontier。
* 一方已经完成后取消强制交接不破坏必要性：过大 gap 阻止的是**首个**请求完成，所以无法先借第二阶段的连续执行越过它。

据此接受 root `codex-model-contract.md` §4 的受限 max-gap iff；它是已知有限链上普通 checkpoint 可达性，而不是 nested IOMMU 的独立新理论。

## 5. 同预算 PWC 与 primary 对照

q3 的 Y/T 点已有明确普通 cache 表示。Bhargava 等 ASPLOS 2008 §4.2 的 NTLB 以 guest-PTE GPA 查对应 nested leaf，命中后构造 guest-PTE physical address并跳过整个 nested子walk。这支持保存 mapped guest-PTE page 的普通构造；原文没有给本文77位 frame或q3持续RR保证。[原论文](https://www.cs.columbia.edu/~cdall/candidacy/pdf/Bhargava2008.pdf)；[正式 DOI](https://doi.org/10.1145/1346281.1346286)；本轮局部证据 `primary/nested2008.txt:363–408`。

q2 的 Q 点也有具体 ordinary 表示。Barr 等 ISCA 2010 §3.2/3.3 保存地址 prefix 到 table PPN 的 partial/path translation，§4.2 讨论跨level共享槽及按重载成本保留，表1/2明确区分 tag 与 data。它覆盖 returned next-table pointer 的缓存形式，没有声称本文嵌套 fault合同或每轮两笔操作的完成界。[作者论文](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)；本地证据 `../shared-translation-literature-20261006/papers/translation_cache2010.txt:265–310,326–385`。

Rocket 2022 的 `aux_pte` 已在 S parent 与 S leaf 间复用，`r_pte` 提供物理 table PPN，地址offset由原 request/levels重建。因此 q3/q2 普通 tracking token 可以直接按这些旧字段加上述保存规则实现，而非仅凭“ordinary能照抄任何新算法”否决。[固定源码](https://raw.githubusercontent.com/chipsalliance/rocket-chip/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala)，关键 `:158–214,524–530,559–565` 已在§44直接核读。原源码保留 full PTE 和 CPU 用字段，不能声称它已经公布77/81/82位IOMMU checkpoint。

把 progress 放入 PWC 时，至少需计 valid、context/key、prefix/level、PPN、必要权限/故障语义以及替换/保护 state；context引用也需要稳定存储。共享 PWC 的普通LRU能被另一个请求驱逐，不能假定每次恢复都命中。保护一个请求的进度直到它结束，再服务另一个请求，可以让某些 restart策略的两个有限请求结束，但其 key/PPN/保护成本必须与同预算token比较，不能将cache放在预算外。

当前一个 active engine+一个 suspended frame 的预算不等于“一份全局 PWC 更便宜”；一个protected记录可能正好把原suspended frame放到别处。反过来，两个请求共享真实页表prefix时PWC可能有实质复用收益，也会改变30笔下界。本文没有枚举所有这类普通优化，因此没有完整 frontier。

## 6. 创新停止、后续唯一合同差异与验证范围

因为 Rocket 的 parent/leaf union、Bhargava 的 nested叶映射以及Barr的partial pointer给出具体同字段普通构造，不能把phase-specific字段裁剪、q3的G/S语义切块、q2的G-root checkpoint命名为独立新机制。因为固定known chain的可达性退化为相邻checkpoint的gap判别，不能把其有限图或DP作为独立新理论。相同total state、原请求/context、输出合同及PWC保护都计入后，已有普通实现可复现本稿所有明确保证。

如果后续能证明**超出列明保存格式**的同architecture输出、同总容量、同PTE让路/持续进度合同的锐界，并且普通checkpoint/PWC/partial restart都达不到某点，才有新的理论贡献；当前没有这项证明。只比较36与82或只发现某个最大quota填满策略livelock，就停止创新枝。

唯一值得区别但本轮不扩展的合同是：允许controller选择较早chunk边界，与**外生budget-off可在任意PTE退休时到达、要求一笔不可撤尾部内释放执行机**不同。q3规则只能在选定点直接保存，不能把主动3+1调度的结果免费当作任意时刻释放且不重读。强普通对照仍须包括完整token、带计费保护的partial/PWC和回退重读；外生handoff若可无限早到，也必须明确最低语义服务或中断次数/到达限制，否则任何丢进度规则都可无logical progress。当前生产PTW没有该暂停接口，不能把它虚构为既有平台约束或借此恢复本稿的新颖性。

两个有限请求结束与持续到达的有界响应也不同。对于固定仅两个单 outstanding端点和本稿固定chunk策略，可以由有限轮次数推出有限PTE服务机会界；若允许无限排队、变化的已接纳请求数或没有到达/最小服务合同，单个有限例的完成不能推出所有后续请求的uniform latency。正常DDR仅保证最终响应，也不能把PTE次数界换成cycle界。

最小下一验证仅是 root 已准备的五个ordinary保存primitive有限图：允许1..q早交接、保存和fallback严格计费，对比 Dijkstra完成可达性与max-gap判别，保留q3 zero-rework路径和pure partial的q3非进展环。本文不执行它。若交叉检查一致，结论就是受限普通checkpoint选择模型的数学收尾，不需要DUT/新RTL或扩大实验。

已人工审定：15步正常链、上述普通q3/q2构造、pure partial反例、受限max-gap证明、future高Sleaf fault的旧target必要区分、primary相同字段表示。未验证：RTL checkpoint实现、所有动态fault/大页/权限优先级路径、共享PWC完整空间、任意联合编码下界、持续到达的uniform latency、面积/频率/性能。
