# §44 nested continuation 表示与成本：独立 prior 审核

2026-10-08，Codex 直接设计推演、读取关键 RTL 与 primary 源码；未用 DeepSeek/Claude，未修改 RTL、运行模型、build 或 DUT。与 rules 席交换其具体字段表，root 已直接复核下述 Rocket 固定版本。所有新下载仅为本目录 `continuation-primary/` 下的只读证据；完整 URL、commit、date、bytes、SHA256 在 `continuation-primary/sources.json`。

**结论：三阶段切分、S 父/G 子分解、父 PPN 与叶 PPN 复用、原请求索引重建，以及 superpage 低位重建均有具体 primary 表示，不能据此立新的 continuation 机制。** 这是逐字段对照的结论，不是“普通方案可以照抄任何新算法”。旧实现没有自称 82bit 暂停帧；下面区分其明确存在的表示、同语义特化后的普通构造，以及尚需证明的成本命题。

## 1. 强 source 与可直接导航的位置

### A. Rocket 2022：完整父/子表示与转换，最关键的覆盖证据

官方 commit **bf5742b9e353a80f501d43130218d3a444ec4b30**，committer 日期 **2022-02-01T20:42:43Z**，通过 GitHub 官方 API 核对。

- [官方固定源文件](https://raw.githubusercontent.com/chipsalliance/rocket-chip/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala)
- 本地：`out/innovation-frontier-reset-20261008/continuation-primary/rocket-2022.scala`，24973 bytes。
- `156–171`：保留 request/return destination，`r_pte` 与 `aux_pte`、当前 level 与 `aux_count`，`stage2/stage2_final`。G 子 walk 的目标索引来自 `aux_pte.ppn`，S 索引来自原请求。
- `184–204`：检查无效/超宽 PPN 与 superpage；next-PTE 地址由当前 table PPN 和目标 VPN 的 level index 组合，不需要保存完整物理字节指针。
- `211–214`：S-stage 的 PTE GPA 由 `aux_pte.ppn` 和原请求的 S-level VPN index 重建。
- `217–218, 489–494`：superpage 低 PPN 位由原请求或父 target 重建，并用父属性形成结果。
- `524–530`：G 子 walk 返回后恢复 S `aux_count`；S 叶完成后转 final G。
- **`559–565`：同一 `aux_pte` 在 S nonleaf 时保存下一 S 表 PTE，在 S leaf 时保存叶 PPN/属性并合成地址。** 这明确实现父表/最终叶的阶段复用。
- `163, 396–403`：仍保存 fault GPA offset，返回标记是否属于 PTE 访问；不能忽略 fault 合同后宣称旧寄存器全部无用。

这些位置直接支持 `T=父表或叶PPN`、`Q=当前物理table PPN`、`j/k=父/子level`、地址索引重建。它是通用 CPU PTW，不是多 DMA 抢占设计；仍有 full-PTE 寄存器、CPU 权限/PMP、faultoffset、响应字段。因此**不宣称旧源码的总成本就是 82bit，不宣称它已实现任意 PTE 边界的暂停**。

### B. 香山 2026-09-28：S/G 独立模块与返回合同

官方 kunminghu-v3 固定 commit **09453ff8f44e9d2b526e6149c60e1dc3f08c7192**，committer 日期 **2026-09-28T17:35:48Z**。

- [官方 PageTableWalker.scala](https://raw.githubusercontent.com/OpenXiangShan/XiangShan/09453ff8f44e9d2b526e6149c60e1dc3f08c7192/src/main/scala/xiangshan/cache/mmu/PageTableWalker.scala)
- [官方 MMUBundle.scala](https://raw.githubusercontent.com/OpenXiangShan/XiangShan/09453ff8f44e9d2b526e6149c60e1dc3f08c7192/src/main/scala/xiangshan/cache/mmu/MMUBundle.scala)
- 本地前者 `continuation-primary/xiangshan-PageTableWalker.scala`，后者 `continuation-primary/xiangshan-MMUBundle.scala`。
- 前者 `104–195`：S PTW 保留 level、PPN、VPN、请求源和 S/G 执行状态；`208–233`重建中间/最终 GPA。
- `329–332, 434–460`：父 PTW 用 GPA VPN/id/source 请求 HPTW，并处理独立 G 结果；`1387`起有独立 HPTW 执行模块。分解 S/G FSM 与父状态等待子结果已有实际代码。
- 后者 `1206–1224`：HptwResp 包含 PPN、level、权限和 guest/access fault；`1227–1232`按 G 叶 level 以 target GPA 的低 VPN 位重建 superpage PPN。
- [官方 PTW 文档](https://docs.xiangshan.cc/projects/design/en/kunminghu-v3/memblock/MMU/L2TLB/PTW/) 的 allStage/onlyStage2 状态流程与 hptw 接口；[官方 HPTW 文档](https://docs.xiangshan.cc/projects/design/en/kunminghu-v3/memblock/MMU/L2TLB/HPTW/)的 G 叶/异常返回与逐层访问。

**覆盖：** 父 S continuation 与 G 子请求/返回的语义分解、地址重建和 G superpage 信息。**不推断：** 整个香山 walker 总状态很小、任何外部预算暂停都可直接停 HPTW，或其多设备隔离保证与本平台相同。

### C. 本平台直接祖先与已有每 PTE continuation

当前 IOMMU PTW `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:17–20` 明确说明改自 CVA6 Sv39/Sv39x4 PTW。同树祖先 `cva6/core/mmu_sv39x4/cva6_ptw_sv39x4.sv:105–135`已有三个阶段、S/G level、目标/物理指针；`389–415`保存 S 叶、G 完成后恢复 S。祖先文件头标注 Bruno Sá，2022-08-14；本轮未把这项文件头日期当成已验证的公开版本发布日期，官方 raw 路径查询未取得文件，主要公开表示证据使用上面的 Rocket 固定版本。

[US8140781B2](https://patents.google.com/patent/US8140781B2/en) 的 tracking/token、PTE 返回后的 next-level 再仲裁已由 root 前轮直接核原文，见 Fig3–6、step305–345、claims2–3；**它覆盖逐 PTE continuation 主体，不单独证明完整 nested 权限/异常帧已存在**。不能因为它未列 RISC-V nested 全字段，就据缺 feature 认定新颖。

## 2. 与 rules 的精确表示逐字段对照

限定 PAddrWidth56/GPA41（soc `ariane_soc_pkg.sv:127`；IOMMU pkg `rv_iommu_pkg.sv:74–82`），稳定合法上下文，无 A/D 写回，暂停在已完成 PTE 检查且下一 PTE 尚未 first-offer 的安全边界。`C` 是双方同样计费的原请求、DC/PC、根与身份/权限上下文；不能把它在 baseline 中删除、在候选中免费保留。

rules 的表示为：I=intermediate G；S=准备读取 S PTE；F=final G。`T` 在 I/S 表示 S 表 GPPN，在 F 复用为 S leaf PPN；`Q` 表示当前 G table SPA PPN，S 时存映射后的 S-PTE physical page；`j/k` 为 S/G level；S global 或最终 leaf 属性另保留。

| 提案字段/重建 | 最强 prior 同一表示 | 真实剩余差值 |
|---|---|---|
| I/S 的 T29，F 时复用为 S leaf PPN | Rocket `aux_pte`，尤其 `559–565` 的 nonleaf/leaf 分支 | 通用 PTE 裁成当前 IOMMU 语义的 live bits；复用原理已存在。 |
| G 当前 table PPN Q44，按 target index 构造 next PTE | Rocket `r_pte.ppn`＋`196–204` | 不必存完整 next-HPA，已有构造；stage root 可由 C 恢复。 |
| `x={T,IOVA_j,000}` | Rocket `211–214`，`r_req.addr`＋`aux_count` | 相同原请求索引重建；不是新增压缩。 |
| G 返回恢复 S level | Rocket `524–530` | 直接使用已有父 level。 |
| final GPA 由 leaf high PPN＋IOVA 低位构成 | Rocket `217–218,559–565`；香山 `208–233` | 大页派生低位不用独立保存，但必须保留 leaf level。 |
| S 时只存 Gleaf 高 `44−mG`，以 T 低 `mG` 还原 physical page | Rocket `489–494`；香山 `genPPNS2:1227–1232` | 同一 superpage 拼接表示；可变位宽的容量收益还须真实 packing。 |
| 21/27 可达 phase/level 合并为5bit tag | 旧代码分别存 level/stage；合法状态集合可枚举 | 旧2+2+2独立编码与联合编码至多1bit差；这项 enum 编码本身没有新嵌套规则。 |

不是声称旧源码可以原样装入82bit，而是已经找到**所有大块 payload 的同一语义因子和重建步骤**。将该 source 的成功路径特化为本 IOMMU 合同，会得到具体普通帧：I/S 最坏 `T29+Q44+global1+tag5=79bit`；F 最坏 `Sleaf29+Q44+{w,x,u,global}4+tag5=82bit`。C、valid、slot/return token、暂停选择、在途归属、error/result 保存和 active-context 仍另计。这是 Codex 的源级构造，不是旧论文的面积数字；本轮没有实现/综合。

Sleaf 的 v/r 在本 PTW 已成功通过时可固定1：`rv_iommu_ptw.sv:406–409,491–504`检查有效性、r/a等；PPN 高15位在两阶段合法成功路径固定0，见 `607–615`。CPU Rocket 的一般权限更宽，不能拿 CPU full-PTE 寄存器宽度作为 IOMMU ordinary 的必要成本。

## 3. 两个容易遗漏的语义/成本边界

**异常地址不是只按最终 GPA 重建。** 当前 PTW 的高 S PPN 错误会强制 intermediate-G 标记（`609–615`），ERROR 用 `gpa_x_q` 报 bad GPA（`629–645`）。对于 S leaf，旧 `gpa_x` 没有在本拍被更新；对 j1/j2 的 S nonleaf，`525/558`已经计算新 truncated target，之后 ERROR 覆盖 state，所以报告值可以是更新后的 target。必须保留或正确重建 S 前目标 T，不能笼统说所有高 PPN fault 都报旧 x；也不能先判 leaf/高位成功后再暂停，遗漏同拍优先覆盖的错误语义。

**逻辑可变字段数不是自动可借用的物理容量。** j/k=1 可从 C 根恢复，superpage 有9/18位派生低位，确实可以减少 token 的 live information；若每个槽仍固定分配82bit，这些空位没有增加可并发保存的 context 数。若要宣称新容量点，必须计实际 packed pool、索引/碎片、分配与恢复带宽。普通固定槽、按phase分开的池与允许 restart 的点都要配平总容量。

最小充分状态也取决于输出合同。若要求同最终两阶段 IOTLB entry，必须保留 S leaf 的后续访问权限；若允许 skip-fill/direct result，当前请求的 S 权限可以压为一个 verdict并延后至G错误优先处理后使用。后者不能保留与前者同样的未来读/写命中语义，因此79/82或其他数值不能被无条件叫作全系统信息下界。层级计数、fault优先、superpage与根配置必须一起固定。

## 4. 语义边界暂停的实际保证与接受条件

当前 PTW 单笔 PTE AR，ID固定0（`266–277`），MEM_ACCESS first VALID 后不可撤；AR握手转PROC_PTE并等R（`384–398`）。安全暂停必须在该R已归属/退休并完成 PTE 错误和下一步检查后，阻止下一次 AR first offer；或额外保留旧返回归属与容量。保存执行变量本身不能撤销已offer事务。正常稳定映射、完整 C 与 continuation 帧能避免重复 PTE read，但这仍是已有 continuation tracking 加已有 nested 表示的组合，未得到新的服务规则。

Sv39 S 与 Sv39x4 G 的3层4KiB冷路径为3次S-PTE读＋4个3步G子walk，合计15次PTE读，不含DDT/PDT、fault record、MSI与未实现A/D写回。whole-walk 与每PTE释放粒度不同；DWS的一个foreign whole walk边界不会自动变成一笔PTE周期上界，但指出这个范围也不是新保证。时间上界还需要memory接受/返回的真实服务界，不能由eventual progress推出固定周期T。

调度 prior 的准确范围：[Improving GPU Multi-tenancy with Page Walk Stealing，HPCA 2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)，本地 `out/shared-translation-literature-20261006/papers/walk_stealing2021.txt:109–117,369–381`，soft-owned walker/idle stealing 与一个 foreign walk；[Scheduling Page Table Walks for Irregular GPU Applications，ISCA 2018](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)，本地 `walk_scheduling2018.txt:23–35,75–94`，按同 SIMD instruction 的翻译总工作与 batch 调度。两者不能直接替代 nested 暂停状态证明，也不能通过把 instruction 换成 DMA job 产生新调度机制。

## 5. 交叉质疑后的更强普通点：只 restart 当前 G 子 walk

root提出、rules与本席逐项核对的 O2 不做 whole-walk restart：每次当前PTE R已退休并完成检查后，保留 S 父或 S 叶语义，丢当前 G 进度；恢复时由 C 的 hgatp 从当前 G 子 walk 的根重起，已完成 S 前缀不重读。

- I/S 保留 T29＋Sglobal1；F 保留合法 Sleaf PPN29＋w/x/u/global4。
- 用 `I_j/F_j` 六种状态编码 phase＋Slevel，tag3；最大 `33+3=36bit`，C/valid/owner/slot token、active-state、在途归属、terminal错误/结果仍另计。
- root重起的具体 prior 构造是 Rocket `593–599` 的 `makeHypervisorRootPTE(hgatp,target,...)`，它本来就被 `500,503,559–565` 的子 walk 切换使用；香山的父 GPA 请求和 G root 地址构造在 `PageTableWalker.scala:329–332,1425–1438`。这是对子任务已有输入合同的重发，不是任意抄候选的82bit压缩。
- I 中 target 由 T 和原IOVA_j重建；F 由 Sleaf、Slevel和原IOVA重建；S 已得到 G 映射但未读下一 S PTE时，O2丢掉该映射，重新完成 G，再读该 S PTE。

在稳定映射、正常 PTE 无写侧效、已退休的子 walk进度可丢而未发布terminal结果的条件下，**一次暂停至多浪费当前 G 子 walk 的3次已做读**；对于只有一次handoff且之后持续恢复服务的3层冷路径，可额外至多3次G读。若总共有有限P次这种暂停，`15+3P`是保守读数上界，不能由这个式子自动推出P有限或延迟有界。异常已判定或 final G 已触发 IOTLB/result commit 时必须完成/保存terminal分支，不能把其状态免费丢掉；当前无A/D写与固定映射是这个同效构造的必要范围。

**关键反例：公平地轮换 PTE 不等于公平地完成请求。** A与V每人读完第1个G PTE就暂停并丢子进度，可以无限轮流重读G根，双方都没有S进展；甚至每人做完3次G后、读S PTE前就丢映射，也仍然不能推进S前缀。保证I阶段的持续进展需要完整 `G≤3＋S1` 的语义quantum（最坏4笔PTE），F阶段需要G≤3到terminal提交，或明确限制暂停次数/保证足够连续服务。

所以O2可以有“一笔不可撤PTE尾部”的单次handoff性质，但**不能免费同时继承每PTE无限抢占下的持续完成保证**。未来frontier必须将固定到达、可持续服务合同、36bit partial restart、82bit无重复continuation与whole restart一起比较；不能只拿full snapshot对whole restart证明候选胜出。

如果新表示能在相同 C/结果/异常/在途合同与同总容量下给出上述普通 source-factorized frame、checkpoint/restart及同预算PWC不能达到的等待—重复读点，并且给出必要性证据，才可立机制或成本分析贡献；因为本轮的大块字段已经逐项被明确 prior 表示覆盖，**不能以82bit或S/G分解立机制，也不能以未综合的省位数字立独有收益**。

仍值得判别的唯一分析问题是：**在固定到达与持续进度合同下，有限walker总状态容量是否允许给出nested多设备等待、重复PTE读与状态容量的完整frontier，而非复述既有checkpoint/restart tradeoff？**

本轮没有这个完整frontier、不能证明 ordinary 最优点更差，也没有发现一个真正不同且生产可达的替代机制；因此不能据这个问题认定新理论。root可用本稿字段/输出合同作下一轮接受边界，无须为证据不足而立即建新队列或跑新DUT。
