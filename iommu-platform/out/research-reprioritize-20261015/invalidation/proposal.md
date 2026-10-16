# I 正向设计：用一个有界稀疏页集命令，同时保留精确范围和较低提交量

任务日期 2026-10-15，交付日期 2026-10-16（Asia/Tokyo）；RESEARCH-REPRIORITIZE-1 / I 独占产物。**保留一个明确的架构投影，但本轮不建议进入实现：正常 strict unmap 的可合并稀疏输入、同域 live 余量和前端服务瓶颈仍未得到来源支持。I 保持 not_ready。** 不作跨方向排名，不以 D 降优先级提高 I，保留 A5/A2 成果与状态，不恢复旧模型或 MIG。

下文是 Codex 生成的候选设计与未运行预测。请求配置为 gpt-6.1-sol / xhigh；工具没有返回独立实际模型/推理元数据，两项实际字段均为 unknown。研究 Codex-only，没有 DeepSeek/Claude、下级 agent、模型执行、构建、设备或板操作。使用 ARS-Codex 的有界 scoping 方法，不启动完整 pipeline。父任务要求的机制卡、前作界线和最小实验规格属于本次明确交付，不代表已经越过任何研究验收。

## 1. 先固定问题，而不是把三个成本词拼成机制

**研究问题：当一个正常撤销批确实包含“同一 context、近邻但不连续的待撤销页”，且这些页之间仍有需要保留缓存翻译的合法 live 页时，能否用有界目标集表示降低提交/执行费用，同时维持精确失效与原本的 strict 返回、IOVA 再用合同？**

三个子问题共享这一范围：①正常调用何时真的提供这种批；②单命令目标集在实际流水线中是否比普通精确命令串更便宜；③减少的费用能否改变完整任务或 IOVA 回收结果。初始范围为单 stage、非 global、4KiB leaf、无 ATS 的主机 DMA；不讨论迁移故障、设备超时或跨 VM 回收。

候选不新增“安全撤销”能力：正确的逐页/范围失效本来就能安全撤销。拟增加的是**在同一有限命令里表达非连续精确集合，并对未选 leaf 给出明确不误清保证**。这改变的是表示与处理费用，不是发明 bitmap、合批、预算调度或另一套完成语义。

## 2. 正常来源、真正的等待、以及不能偷换的寿命

| 已亲核原文 | 能接受的事实 | 不能据此推出的事 |
| --- | --- | --- |
| SRC01，ixgbe_main.c:8317–8329、8369–8370 | 正常 TX 分别 map skb header 和 fragments，并把返回 DMA 地址交给 descriptor | 物理 fragments 离散不等于 IOVA 稀疏；也不等于现成稀疏 gather |
| SRC01:1133–1144、1161–1187 | cleanup 先看末 descriptor 的 DD，再 unmap header/fragment；同 ring 按 cleanup 前缀推进 | 不能令第一个 strict unmap 提前返回，等待后面的 unmap 来凑一批 |
| SRC01:1155–1165 | 源码在 unmap 之前调用 xdp_return_frame/napi_consume_skb | 不能把整个 skb payload 必然保留到 IOTLB 完成；调用也不证明实际最后一次物理 free 恰好在此发生 |
| SRC02，dma-iommu.c:707–727 | 非 FQ 在 iommu_unmap_fast 后同步，之后 free IOVA | 真正阻塞/再用点有源码依据；时长、命令数与成功运行仍未知 |
| SRC02:127–152、174–219；SRC03，iommu.h:203–218 | FQ 按完成计数释放 IOVA/freelist；freelist 是可由驱动加入的页表页，不是 payload 页的同义词 | 不能把通用 freelist 能力当当前 RISC-V 实际每次都产生页表回收债务 |
| SRC05，riscv/io_pgtable.c:180–208 | 本地正常 unmap 清 leaf PTE，逐页登记 gather；这段未加入页表 freelist | 当前链上页表回收字节应按实际记录，不能固定乘页数虚造 |
| SRC02:1318–1332、1359–1391 | 普通 map_sg 一次分配连续 IOVA；unmap_sg 对这一连续 allocation 调一次 __iommu_dma_unmap | 不能用同一 skb 的 scatter/gather 物理离散来证明需要稀疏硬件命令 |

当前 strict 后端源码可达，FQ 因缺 DEFERRED_FLUSH 回退 strict（SRC02:617–620，SRC04:1198–1209）。已有 .config 未启用 ixgbe，DTS 没有该 NIC 的接入实例（SRC15/SRC16，前轮后端门）。**正常来源是实际 Linux 驱动源码，研究投影不是本机部署事实。**

本轮所读代码不提供“多个 CPU/多个 TX ring 的 strict unmap 在各自返回前，经同一个聚合器形成目标集”的现成实现。cq_lock 只串行发布描述符，锁外 polling 不是稀疏批构造器（SRC04:492–544）。多 CPU 可能并发、多个命令可能在途，不足以证明某个未发布的完整集合可被硬件一次获取。

因此允许的两条后续来源路线仅为：

1. 取得正常并发 strict 调用的实际重叠切片；在每个调用仍等待、没有提前返回的前提下，采用有界提交前聚合。聚合等待、锁、每调用完成点都计费，普通命令串获得同一聚合器。
2. 明确新增 bulk-unmap 调用接口，由驱动只提交已完成、可一同撤销的对象。新接口、资源延后归还和调用点修改是候选费用；不能说当前 ixgbe 已经具有该接口。

两条都尚未成立为本实例证据，不能先偷偷改 API 再把收益归给 bitmap。

## 3. 两个具体思路，保留一个作有条件能力投影

### 思路一：map 时整理 IOVA，使撤销天然连续

按 live/临时用途或 queue 为 IOVA 分 arena，同一次完整退休的 SG 放入连续 allocation。这样一次普通 RIL/区间命令就不必跨 live 页。它可以在 map 阶段改写事后区间 DP 无法改变的几何条件，而且不需要复制 payload。

**这是强对照，当前不作为所选创新。** 普通 per-queue arena、现有 map_sg、保持合法复用 pool 已能表达大量这类动作；rIOMMU §4 更已经改变 per-ring 地址/表结构来降低失效。Scalably §3.1/§4 也直接讨论分配密度、缓存、页表膨胀。不能仅把分配用途改名为“回收 cohort”就认领新机制。

若以后找到严格的动态寿命/IOVA 容量约束，使便宜的静态 arena 仍无法保持同费用下的撤销连续性，它可以再独立发展；本轮没有这个输入。普通 arena 的预留空间、碎片、每 ring 元数据、分配失败和页表驻留必须实际计入，但不能人为给它很小容量来制造优势。

### 思路二（保留）：一个命令表示一个窗口里的精确稀疏页集

暂称 `IOTINVAL.SET32`，是**拟议扩展，当前 ISA/驱动/RTL 均未实现**。输入为 `(PSCID, base_VPN, mask32)`；base 对齐 32 个 4KiB 页，bit k 选择 base+k。目标集合恰为置位页。软件只合并同一 context 的目标，不需知道未来访问、live 缓存驻留或一个硬件订阅目录。

初始设计给非选中、非重叠的 4KiB leaf translation 一个明确保留合同：该命令本身不得使它失效。普通容量替换仍可能发生，不提供永驻或任意 DMA deadline 保证。RISC-V 标准允许实现扩大 IOTINVAL（SRC08:280–282），所以这条保留语义是需要实现、验收和收费的额外合同；不是从新编码自动得到的保证。

位预算的一种**概念新编码**为：opcode 7 + subop 3 + flags 2 + PSCID 20 + base_VPN 52 + mask 32 + reserved 12 = 128 bits。52 位 VPN 足以表达 64 位地址的 4KiB 页号；flags 用于固定的 leaf-only/保留能力合同。此编码不携带 GSCID，不支持 nested/guest；如果增加 GSCID 或更多语义，必须重新编码或增加命令字节，不能免费扩展。现有字段宽度依据 SRC14:425–440，但这不是占用现有 reserved 位的合法实现补丁，也未分配正式 opcode。

执行可用一个 128-bit pending register。对 E 个 leaf tag，先匹配 context 与窗口高位，再以页号低 5 位选 mask bit；一次遍历或分 bank 遍历清选中项。并行匹配的逻辑/布线、串行遍历的占用时间必须比较；不预设它与普通单地址比较同周期同面积。无需先取一张外部目标表，因此也没有隐含免费表 fetch。

## 4. 正确执行和回收接口必须一起说清

1. 只有正常 DMA 完成、调用者合法开始撤销后才登记目标。clear PTE 与命令发布之间沿用目标平台所需的可见性/屏障；mask 不代替 PTE 更新。
2. 稀疏 gather 有固定容量 K，保留准确目标，不把 min/max 当准确集合。当前 gather 遇 disjoint/pgsize 变化会同步，见 SRC03:603–627；修改这一处是实质接口工作。容量满、跨窗口、不同 context 或无法证明的页型，回退普通精确序列，不能漏页或无界等待。
3. 同一窗口只用一个 bounded mask；不等未知未来到达。可合并窗口/等待上限在真正来源确定后冻结，并让普通精确串、RIL/DP使用完全相同的等待与已知集合。
4. 命令完成前，必须处理已接受旧 walk/fill：要么阻止选中页旧 fill 再插入，要么在发布前排清相应 walk、失效并确保后继读观察更新。不能只清现有 tag 然后立即 ack。当前 inval 优先于 update 的 if/else（SRC06:215–244、315–331）不单独证明完整 race 已闭合。
5. 所有目标完成后，才允许普通 IOFENCE 报告该命令完成；IOFENCE 仍覆盖此前完整 CQ 前缀，不新增绕过前缀的 per-context 完成。实际 head 取回不等于执行完成（SRC07:276–283；SRC08:302–308）。
6. 每调用原有 strict 返回点、IOVA 可再分配点、实际页表 freelist 释放点和实际 payload owner 归还点分别记账。错误/timeout 不能产生成功再用收据；现有 void sync/轮询的错误处理缺口不是本候选贡献。本轮不修复也不接受它作为已通过后端。
7. 初始仅 leaf PTE 改动且上层页表稳定。superpage、global、非 leaf 变更、alias、S2、ATS 都回退原有适当完整序列。bitmap 选中一个 4KiB 页不能保留覆盖它的旧 huge translation。PR/PW 与未到达 IOMMU 的设备请求另有协议要求（SRC08:319–351）；当前硬件 PR/PW TODO（SRC07:362）不凭候选编码消失。

## 5. 同信息、可执行的具名强对照

| 对照 | 给足的动作/信息 | 剩余比较点 |
| --- | --- | --- |
| Exact-PSCID 串 | 正确 identity、leaf/alias覆盖；同一目标集合；合法 CQ 批发布；一组正常 fence | 单页/精确连续段完全正确，比较 descriptor/decoder/匹配费用；不能保留旧全 host 弱基线 |
| Legal-RIL-DP | 在硬件真实可编码 range 上做最小覆盖/费用 DP，允许多个精确 interval、full/context 和同一 live 信息 | 某两个目标间有必须保留的 live leaf 时，区间表达可能须拆；DP 本身不是能力不足或新算法 |
| 同批普通列表 | 普通精确命令数组，一次 burst 写 CQ/少 doorbell/同 fence；相同聚合等待 | 候选必须击败它的实际服务费用，不能只数软件调用次数 |
| 现有命令的硬件 combining | 有界预读普通精确命令，再内部合并成 mask/offset set；同面积/存储/批边界 | 可成为等价架构实现。若同费用完全复现，则停止“必须新 opcode”主张；不据此说新增能力已发表 |
| SG + queue arena + RIL | 完整对象 map_sg、per-queue 地址安排、合法持久 pool；可重新选 IOVA | 若同空间/CPU/页表成本就消除稀疏，当前实例不需要本机制；不能把原 IOVA 布局人为冻结 |
| 分 domain/context | 合法 group/attach 条件下分离临时/敏感对象 | 可消除跨 context 直接误清；仍共享的服务费用须实核。同一 endpoint 多用途是否可拆不能想当然 |
| FQ/per-core deferred | 在应用明确允许的延期保护合同下使用真实能力与相同资源预算 | 与 strict 合同区分；当前 RISC-V FQ 回退只是部署事实，不能借缺能力宣布普通合批失败 |

所有对照都知道候选知道的已完成集合、页型、context、允许回收时限和任何登记 live 范围；都无未来 oracle。离线最优只作上界，不能替代这些实际程序。软件普通列表无法在当前单命令格式里表达任意 mask，这只说明候选增加付费表示能力，不能单独证明 novelty 或净收益。

## 6. 一个未运行正例预测和紧邻反例

**条件正例 P（不是已有 workload/trace）。** 来源若确实给出同 context、同一 32 页窗内八个可同批撤销页 `T={2,4,6,8,10,12,14,16}`，间隔奇数页为仍合法、驻留且马上再访问的 live leaf；所有对象 DMA 都满足各自完成/撤销条件。PTE 只清 T。允许的未提交批等待、资源峰值、保护窗口对所有方案相同。

- SET32：一个目标命令 + 一个 fence，编码最小共 32B；mask 为这些 bit 的集合，不做范围扩张。
- Exact/零额外 live 清除的区间 DP：若每相邻目标间都夹着需要保留的 live 页，至少八个互不跨洞的区间/单页命令 + 一个 fence，16B 格式下为 144B。这个表示下界不含实际取回 cacheline、doorbell 和服务时延，也不是新颖性证明。
- One-RIL：一个 range + 一个 fence，可能编码同为 32B，但会覆盖 live 洞；只有实际触发的额外 walk 超过真实余量时才是问题。持久 pool、arena 或分 domain 若在同预算下把洞移走，则按它们的实际较优结果裁决。

唯一可接受的性能预测写成条件：如果候选的编码+等待+执行+fence 完整时间低于真实回收预算，普通同批精确串高于它；且 range 的实际额外 walk 超过 live 余量，而 bitmap 没有因比较/共享端口造成同等延迟，则候选能改变这一实例的可达结果。上述成本全部未知，不给虚构 p99、deadline 或加速比。

**相邻反例 N：** 保持正常来源/时序/资源预算，只令 live 页不再有后继访问或实际可容忍 full walk，one-RIL 就可能同命令数且硬件更简单；或把 T 改为一个连续 SG allocation，普通精确 range 直接相等；或合法 per-queue arena 以相同成本得到连续退休前缀。还包括每个 strict 调用只有一个连续目标且无可用并发窗口，此时 SET32 增加编码/逻辑而无法摊销。应停止该实例，不增加假 remap、稀疏度或命令率寻找赢家。

## 7. 前作边界与可核查的新意论证

- **IDYLL，MICRO 2023，§5.2/§6.3，原件 SRC11:399–419、514–588（论文 pp.1168–1169 附近，按原件页脚定位）**已认识 invalidation 与 walk 争用，并把 36-bit base 和多达 16 个 9-bit offset 合成 IRMB entry。TLB 在收到请求时仍立即失效，延后的主要是本地 PTE 写入。故不能认领“稀疏地址压缩”“有界合并”或“保护 demand walk”本身。本候选放在 host 命令发布/译码路径，是否因此降低正常 strict 回收费用且不清 live leaf，需要实证；换位置本身不够。
- **Utilizing the IOMMU Scalably，ATC 2015，§3.1–3.2/§4–5，SRC12:249–305、530–603**已给分配密度/页表成本、per-core flush 和锁外硬件等待，且明确 IOVA/页表再用约束。软件聚合和分配器不是空白；候选若只是重做这些优化，就没有新增方法。
- **rIOMMU，ASPLOS 2015，§4，SRC13:390–453**利用 ring 顺序、flat 表与至多一个当前 IOTLB 项，使后继替换旧项。当前 ixgbe cleanup 正好具有 ring 前缀来源，所以必须认真比较其思想条件以及更便宜的普通 arena；不能把它的硬件/页表组织免费搬入当前层级页表，也不能忽略它。
- **SMMUv3 2026-07 工程讨论，SRC09:16–79 与 SRC10:28–45**已提出 one-RIL/live DMA 取舍，作者认为等时设备常已有 full-walk 余量，且 CONT erratum 会约束合法命令。本文只用它证明问题值得问，不据此认定目标有 deadline 失败、补丁已合入或 RISC-V 具有 RIL。
- **新检索线索：z-READ 2024，DOI 10.1109/ACCESS.2024.3502688。** Crossref 已核题名/作者/原发布入口；检索出现 bitmap-based ranged TLB flushing 段落，因此这是必须保留的强近邻。IEEE 原 PDF 两条入口 HTTP 418，2020 作者 thesis 的 CORE 镜像 HTTP 403，本轮没有取得可保存核读的正文，**其方法/覆盖关系标未核**。不以检索摘要定覆盖，也不以未取得全文为“无人做过”证据。原收据在 sources/retrieval.json 与 SRC18。

**有条件 novelty 论证：** 最近原文已经做到按页精确撤销、软件合批、稀疏请求压缩及 ring 适配。若正常 I/O 仍存在既无法以同成本整理成连续范围、又不能靠普通同批精确串满足 live 与再用合同的稀疏批，而且以新增的有界目标集表示/保留语义在完整接口、逻辑和服务成本下改变这一结果，则新增贡献可以落在这项成本受限的架构能力及其适用界。若进一步精读发现已有工作已有同问题、同命令能力和同完成合同，则停止该版新意主张；若普通 SG/arena/combining 便宜地复现，则相应停止该实例或“新 opcode 必要”主张。当前材料只支持一个可检验设计空间，**不支持机制新颖性、独有收益或论文主贡献已通过**。

## 8. 全部代价、一次最小判别和投入建议

必须计入：稀疏目标 gather 的构造/去重/窗口切分 CPU；每调用同步/聚合锁和最长等待；准确元数据及溢出回退；CQ 存储、实际 cacheline fetch/doorbell/完成 marker；比较器/布线/寄存器/遍历 bank 与时钟影响；选中旧 walk/fill 的阻止/排空；所有必要 PWC、alias、global/huge/S2/ATS 回退；额外 miss/walk/PTE/内存争用；各类 IOVA、实际页表、payload/pin 的峰值与驻留积分；错误保留与公平性。候选不能凭少七个 descriptor 直接宣称 CPU、带宽、尾延迟或回收都下降。普通布局对照也计 arena 碎片、预留空间、分配失败和 map_sg 改造，但不重复算物理 payload 复制（它未必需要复制）。

**本轮最小后继不是硬件，也不是全面 trace 工程。** 只接受一份现成或可从一个明确 normal 调用切片取得的输入包：

1. 一组同 context 的真实 normal map/unmap 与 strict 返回顺序，证明多个近邻非连续目标在第一个返回前已可合法共存；若要新 bulk 接口，标其确切调用点和新增资源保留，而非伪装现状。
2. 同一包内的已有 map_sg/per-queue arena/持久 pool/分 domain 可用条件，及 live 页确有后继访问和实际 full-walk/完成预算。无需设备数据内容。
3. 一个真实可得的前端服务量度或原始事件切面，能区分多 descriptor 取回/译码与真正 IOTLB/内存服务；只看软件包率不够。

**一次可改变判断的结果：** 如果这一有限包显示自然稀疏批且普通布局在同资源预算下不能消除洞，可由 root 决定冻结一个纯软件事件级规格，比较同目标同批的精确串/合法 RIL-DP/SET32，并闭合旧 fill、strict 返回、错误和 IOVA 再用不变量；来源支持的模型不要求先有 native FPGA，本轮未授权或执行该后继。若这个包只给出逐调用连续 unmap，或 SG/arena 已得到相同结果，就停止本 SET32 实例的投入。缺的是上述一组有因果联系的输入，不是泛泛再采更多数据。

在此之前，保留 proposal 与对照，**不写新 opcode、不加 capability、不改驱动/RTL、不开设备、不重新跑 A2/CQ/MIG**。当前 I 的轻量 not_ready 和所有旧分支边界保持；由 root 直接核关键原文后决定是否把这条来源问题纳入下一轮。
