# 生命周期与更新干扰：独立方向审查

2026-10-15。独占范围：翻译生命周期、共享CPU页表、失效与完整交付；补核旧D3-P2/A6-P1。Codex只读审查，未用DeepSeek/Claude、未启动子席、goal、实验、编译或板操作。只新增本目录来源和本报告；不改变A的候选地位或旧结果。

**结论：本族最值得继续判别的是“真实动态I/O的失效粒度和命令服务，如何影响仍在运行的DMA，以及隔离这种影响要付多少回收/命令/重walk成本”。它有独立于A2 borrowed-mm的现实来源，但还没有已核正例、独有机制或完整论文贡献。纯SVA接入、补notifier、三阶段完成/回收、保守失效目录均不能承担新意。D3的CQ前缀耦合应保留为独立的完成报告问题，不能直接拿来解释不发CQ命令的静态设备的数据时延。**

本次按academic-research-suite的bounded literature/synthesis方式工作；复用已取得原件，定向核方法和契约，不宣称系统综述或查新穷尽。没有设置或模拟ARS全pipeline验收。

## 1. 已否决什么，未否决什么

| 旧方向或版本 | 有决定力的证据 | 这次保留的准确范围 |
|---|---|---|
| LTI式“译址要活到整个DMA结束，继而阻塞IOFENCE” | `docs/translation-lifetime-and-innovation-gate-20261008.md:7–14,28–31`，普通TL在响应接受后退休，TC在物理AR/AW握手后退休，CQ没有R/W完成输入 | 当前实现的该条路径不可达；不能把Arm LTI规则套入RISC-V源码。不是证明所有命令均无等待、全部平台均无寿命问题 |
| D6保守订阅目录的新不变量 | IDYLL §6.2原件`out/research-landscape-20261005/papers/idyll2023.txt:458–505`允许hash误报 | 该差异被准确前作覆盖；不否决目录的性能价值或新的费用表示问题 |
| CO1“最后翻译用途先退休，再保留数据/consumer所有权”的独有动作 | `docs/completion-obligation-design-20261014.md:13–17,23–39`，具体OC1-min逐事件复现同动作/费用类别 | 关闭CO1该版独有准入与必要逻辑成本主张；不能推成整个生命周期族无研究价值 |
| 公共Hero源中末DMA wait、map记录/rollback、unmap后unpin等修补 | `docs/a2-native-sva-binding-20261015.md:24–30`；`docs/a2-sva-implementation-20261015.md:86–92` | 合法的功能基线/工程维护；没有证据说明“补缺项”构成新协议 |
| CPU pgd借根、同数值UVA、零新增I/O页表 | A2实现`docs/a2-sva-implementation-20261015.md:21–33,47–49,96–104` | 与普通IOVA、sameVA shadow公平比较。借根付mm锁、mlock/A位稳定及失去异步prefix重叠；这些是本实现约束，不是SVA的普遍不可能性 |
| “通知/失效完成”“数据完成”“物理页可复用”三者分离 | 官方Linux dma-buf现文已要求fence退休和unmap两个后继等待，HMM要求设备更新在invalidate callback返回前完成（下方S4/S5） | 生命周期完整合同重要，但这些区分本身不是新认识；不能靠重命名receipt立题 |
| D3-P2 CQ前缀耦合、A6-P1他人失效干扰 | `docs/innovation-council18-20261006.md:49,59,69`；原`round2/C3.md:73–97`、`round2/C6.md:17–34` | 仍是独立条件问题，本次未发现足以全面覆盖其实际跨域尾时延/费用前沿的证据，也未获得肯定性能证据 |

## 2. 最强现实入口：少发命令和少误伤live翻译之间已有真实争论

2026年7月SMMUv3 RIL补丁讨论有比假设“系统突然remap”更扎实的入口：作者提出每个gather尽量用一个range命令，减少失效提交时延；reviewer担心扩大范围会影响live DMA的摄像头/显示；作者回应等时流量应已容忍偶发完整walk，新的RIL扩张通常轻于旧的full invalidation，并把formal isochronous支持及替代RIL算法留作有条件后继。**两方都必须保留：这是现实权衡，不是已经证明的deadline问题。** [原讨论](https://lists.infradead.org/pipermail/linux-arm-kernel/2026-July/1146330.html)、[作者回应](https://lists.infradead.org/pipermail/linux-arm-kernel/2026-July/1146543.html)；本地`lifecycle/sources/smmu-range-concern.txt:16–66,70–79`、`smmu-range-response.txt:16–40`。

正常映射流量也有实际来源，不必虚构迁移：当前固定Linux的ixgbe TX从`dma_map_single`取得skb映射，在DD表明发送完成后清理并`dma_unmap_single/page`。直接入口为`linux/drivers/net/ethernet/intel/ixgbe/ixgbe_main.c:1133–1165,1181–1187,8317–8329,8369–8370`。这只证明实际驱动确有这种map/unmap生命周期；**不证明每包一条fence、不证明当前Genesys2有这张NIC、不证明摄像/显示与它共享同一个IOMMU或domain。** page池、持久映射和驱动合批必须按实际配置核，不能人为排除。

因此可调查两种不同的真实部署：同一domain中活跃范围受到过宽失效波及；不同domain中仍因命令取回、更新端口或共享walker/内存服务受扰。前者要确认实际失效footprint，后者不能用“地址不相交”便宣称无缓存效应。若背景所在domain完全独立，range扩大本身不会凭空清除另一domain的精确tag；跨域收益只能来自已核共享服务或实际扩大到全局的实现。

## 3. 最接近方法与剩余差异

以下核心三篇定向读了方法，不凭摘要作覆盖结论；SMMUv3讨论是现行工程原始证据，不能代替已发表机制。

| 准确前作 | 已做什么、信息/执行能力、成本与边界 | 本候选如要推进，具体需增加什么 |
|---|---|---|
| **IDYLL，MICRO 2023**，§5.2、§6.2–6.3 | 已直接解释失效与demand walk争队列/PWC/walker；PTE目录允许保守hash，IRMB合并并延后本地PTE写；TLB仍立即失效，IRMB命中转far fault并依MSHR/replay防旧映射。增加目录/IRMB、查找及驱逐工作。 | 不能认领失效争资源、保守目录、惰性/批处理本身。可区分的目标是具名I/O撤销语义下的第三方服务与回收共同费用；需超出其方法能完成的同合同结果。原件`idyll2023.txt:399–419,464–505,557–585,597–643`；[作者原文](https://yananguo.com/files/micro23_2.pdf) |
| **Utilizing the IOMMU Scalably，USENIX ATC 2015**，§3.2、§5 | 已指出不同IOVA范围没有固有flush依赖；per-core软件flush队列降低全局锁，等待硬件完成时放开CQ锁，同时保留IOVA/页表直到同步完成。采用延迟失效保护窗口，不承诺严格撤销。 | 不能认领软件队列解耦、锁外等待或普通合批。需在相同严格撤销/允许延期合同中，解释跨域服务损害与资源债务，且击败同费用per-core/分组基线。`iommu_scalable2015.txt:264–304,546–603`；[作者原文](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf) |
| **Beyond the Bermuda Triangle of Contention，2025预印本**，§III–IV | 用ZCU104/Arm SMMUv2、FPGA DMA与FPD-DMA并发读来区分TBU/TCU数据侧竞争；静态小事务微基准与硬件时延/PMU。原文背景也已提出frequent invalidations。 | 不能认领共享IOMMU干扰或小DMA脆弱性。已核评估没有将正常map/unmap命令流与victim分开操纵并计回收代价；这不是声称整领域无人做过。`bermuda2025.txt:393–445,563–588`；[完整HTML](https://arxiv.org/html/2508.19670v1) |

必须保留的其他强邻近：

- **rIOMMU，ASPLOS 2015，§4**：按ring顺序用flat表和每ring至多一个IOTLB当前项，使后继替换隐式取消旧项，仅burst末显式失效。这是实际可降低命令率的强设计，但依赖ring访问/撤销顺序，不能无价移植到通用页表；也不能忽略它而把减少末端失效命令当新。原件`riommu2015.txt:390–414,426–450`；[作者原文](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)。
- **ShareMMU，MICRO 2026 camera-ready，§V-B/§V-C7–8**：Permission Tracker验证physical请求，反向失效后等待dirty writeback/in-flight访问和flush ack，再改权限；其进展理由依“stalled miss不占内部执行资源、hit有界完成”。原件`sharemmu2026.txt:363–430,500–582`。这限制了普通三阶段撤销的新意；没有据其条件宣称当前iDMA存在相同死锁。它不是本候选的命令服务隔离方案。
- FPT/Griffin按页在途跟踪与选择性drain的直接近邻，本次沿用旧C3核查入口`round2/C3.md:61–65,143–145`，**本席未重读其完整论文**；本候选不以“新增per-page safe-to-reuse查询”为新意。

上述方法各有平台/语义条件，不能拼成一个零成本万能baseline。没有复现任何论文，也没有把作者讨论视为性能实证。

## 4. D3-P2：规范支持的只是完成报告依赖

RISC-V规范§4.1.2规定IOFENCE完成覆盖此前CQ命令；失效的PSCID/地址限定不会缩小这个前缀。于是若A命令尚未完成且处在B的IOFENCE前，B不能合法报告完成。PR/PW另针对IOMMU已处理请求，不包含全部未到达设备请求。规范也允许实现把定向失效扩大为全局。原件`sources/riscv-queues.txt:280–282,302–355`；[官方队列规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)。

**这不等于A阻止了B的数据交易。** B若始终只使用既定映射且不等待任何CQ完成，就不能仅用此规则解释它的尾时延；必须再有CQ共享内存/端口/缓存/准入的真实边。反之，B若正等待映射撤销、reclaim或context转换完成，完成报告耦合本身可能延长资源持有。

Intel官方VT-d Rev5已提供相邻wait之间的最低完成组保证，FN=0允许后继描述符在前wait完成前执行；其他descriptor类型仍有先后要求，Device-TLB timeout会禁止所有pending wait完成。不能主张更细完成分组首次出现，也不能从“may”推断具体Intel硬件必然按context独立返回。原件`sources/intel-vtd-rev5.txt:7496–7527,7628–7653`；[官方Rev5](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf)。

只用标准RISC-V接口时，普通软件可在入CQ前限流、优先排急项、切开长批并添加正常fence；它无法从已经提交的前缀删除慢A。若研究需要让B跳过已提交A，必须明确是另一个完成契约/硬件设计，并先比较实际VT-d类能力。**本席不建议现在新增这种接口。** 先拿正常合法命令完成分布；只能靠设备故障或故意ATS超时出现时，应另裁故障隔离，不借它证明正常性能创新。

## 5. 当前平台的两项决定性源码事实

1. **先排除可修的扩大范围。** 当前`linux/drivers/iommu/riscv/iommu.c:1799–1817`的host VMA分支把`set_pscid`注释，range ADDR循环也注释，实际发全host-space失效。相对地硬件`rv_iommu_iotlb.sv:217–244`实现PSCID/VA匹配。若此配置观察到静态设备被清翻译，先比较正确PSCID/range普通基线；这能证明部署问题，不能自动证明需要新架构。
2. **不具备正常长ATS前缀的原生正例。** `rv_iommu_cq_handler.sv:310–339`直接发IOTINVAL脉冲；`:348–392`的PR/PW为TODO、长等待来自ATS；`rv_iommu_top.sv:788–803`在无ATS分支把inflight置零。硬件`rv_iommu_iotlb.sv:217–318`在inval周期优先于fill，说明有候选共享更新边，**不证明会丢失、重试或延迟特定fill，也不证明有可观性能影响**，要核完整握手再裁。

文件绝对根为`/home/xvhaocheng/work/iommu-platform/`，RTL上述文件位于`cva6/corev_apu/riscv-iommu/rtl/software_interface/`、`rtl/translation_logic/`与`rtl/`。生产树本来dirty；本席未修改它。平台HEAD=`0a570deda89636631462bb188d39c058409df364`，Linux HEAD=`c0f5e13ea81fa30d78f3bb1d6389e2397321e040`，HEAD不能代表完整脏源码身份。

## 6. 最强可发展候选卡

**研究问题。** 在正常、必须完成的动态I/O撤销下，能否同时控制live DMA的翻译时延与映射资源回收时延？何时选择一个扩大范围的命令、多个精确命令或受限批组，才能以最少命令/CPU/缓冲费用满足两者？如果命令完成前缀而非数据服务是主因，改成资源回收命题，不能混写。

**拟议新增认识/方法。** 对每个实际gather建立合法命令集合及真实失效footprint，同时计三种费用：命令执行/取回/完成服务；被额外清除live翻译导致的实际重walk与victim任务延迟；撤销排队导致的IOVA/页表/pin/buffer保留。用真实应用的translation与reclaim预算选择可实现命令序列。该认识若能解释现行one-RIL与exact策略的不同适用条件，并预测未参与设计的正常任务，是有意义的研究结果；不能仅展示一张参数折中曲线就宣称贡献。

**候选机制，仅作有条件发展。** 不改标准撤销语义，软件在提交前选择合法精确/范围/整context命令和有界批次；需要服务隔离时加入按上下文的有限未提交队列/额度，完成仍用原生IOFENCE。必要信息是实际待撤销范围、允许保留预算、明确登记的live敏感范围/设备合同、已核命令footprint和服务特征。可选集合还须服从硬件errata；上述作者回应报告CONT相关erratum要求RIL，不能把任意single命令序列当合法对照，具体目标仍需查对应勘误。这不是已成立的新算法；最小覆盖/区间DP、静态阈值与普通队列调度都必须作为可执行对照。若同信息普通组合做到同合同同成本，就停止该机制新意，保留问题解释和适用界。

**全部费用。** 实际命令字节数（RISC-V CQ每项16B）及取回/等待/marker流量，CPU/锁/轮询，额外精确命令数，range/gather元数据与选择时间，公平性/背景吞吐，增加的walk/PTE内存流量，延迟回收的IOVA/页表页/真实payload buffer与pin驻留，cache交接、错误保留和timeout归因；若需探针/敏感范围注释/跟踪硬件，其接口、面积、时延也收费。不能把新增保留内存或加大可撤销窗口记为免费吞吐优化。不存在硬件abort/有界服务证据时，只能给条件保证，不能给任意fault下有限回收时间。

**具名普通baseline。** 目标版本实际采用的one-RIL/full/singles规则（上述7月补丁是具体近邻，本席未核合入状态，不据讨论宣布已合入）；硬件支持下的精确PSCID/范围失效；固定阈值、gather合批、每提交者队列/优先级/批量上限；相同语义下的持久映射/page池；可合法延期时的per-core deferred flush；可适用ring顺序时的rIOMMU。后两者若改变撤销保护窗口/硬件数据结构，分开标合同与代价。对D3另比较真实wait分组，不把它拼入RISC-V免费能力。

**决定性正例要求。** 必须有正常、具名应用的实际撤销需求及共享拓扑；在正确匹配范围、合理page池/合批/暖PTE缓存之后，仍有可因果归属的命令或更新服务损害，并使真实live任务/回收预算不满足；在相同成本下，候选合法序列能同时满足。当前没有这条正例，不能预填gain、deadline或p99数值。

**最小合法判别例（条件见证，不是已有workload或性能正例）。** 选实际支持RIL的SMMUv3 profile，4KiB普通leaf、无CONT及相关erratum限制；同一合法domain内两个已完成DMA的buffer位于VPN10和VPN14，需要撤销，VPN12的另一DMA仍活跃且映射保持不变。只清VPN10/14的PTE；两条精确失效和一条覆盖VPN10–14的合法RIL都满足撤销语义，后者可额外清掉VPN12翻译。若实测表明其下一次访问真的因此重walk并消耗live任务预算，而多一条精确命令仍能满足撤销/CPU预算，那么exact优于one-RIL。反过来若VPN12无后继访问、已不在cache或其full-walk余量充足，one-RIL可能更合适。**这对只证明选择有现实后果；exact基线本来就会赢第一例，所以不能用它证明新策略独有。** 真正方法正例需一个预先固定、可执行的联合选择器在未参与设计的混合输入上兼顾两类费用，并胜过具名同信息普通选择规则。

必须补的物理事实包括：实际domain共享/别名和命令footprint，VPN12驻留与后继访问，range操作是否真的清掉该翻译，额外命令和新增walk占用哪些共享服务，live任务预算能容忍多少full walk，真正的unmap/回收时点。若将VPN12设备放到不同domain且硬件精确匹配，跨范围误伤应消失；这个**分domain相邻反例/普通baseline**保留，其context、页表、IOVA/设备分组成本与平台是否允许独立domain也要计入。不能用同domain的最小例直接证明跨domain干扰。

**相邻反例。** live设备可容忍实际full-walk预算；range精确/普通RIL已足够；正常map/unmap速率很低或持久映射消除压力；两个设备没有共享阻塞服务；背景回收预算不允许任何整形；全部损害都来自驱动全局误清或一般DDR竞争。命中这些情况就收窄/停止该应用主张，不扩命令率找赢家。

**可核查novelty论证。** IDYLL已优化失效对demand walk的竞争；ATC2015已拆软件flush队列/减少锁；Bermuda已测共享翻译的数据侧干扰；现行驱动讨论也已明确扩大失效与live DMA的权衡。因此不能把这些现象或构件宣称新。若真实正常I/O证据表明，现有合法强基线仍不能同时满足live任务和撤销回收预算，而以明确可得信息/完整代价设计的选择法给出新可达结果或可验证费用界，就形成了有意义的新增贡献。当前支持的是真实问题来源和规范差异；“强基线仍不足”“新选择法有效”“界有预测能力”均待验证。

## 7. 第一项决定证据与短期风险

**第一项应取得的证据不是新RTL或大实验矩阵。** 选一条实际允许的正常I/O链，取得有限map/unmap→gather→命令序列→完成/释放轨迹，同时写清live设备的真实预算与共享domain/硬件拓扑；只定向核能形成阻塞的服务边。按实际硬件比较one-range、exact与普通合批，明确差异来自全局扩大、服务争用还是完成报告。当前直接源码已经给出“旧驱动扩大范围”的弱基线危险，下一步须先消除此解释。

若目标仍是当前无ATS平台，优先判A6的精确失效/更新服务边；D3长ATS前缀不在这里虚拟成原生结果。若选SMMUv3 RIL/等时部署，需相应可取得平台/原始trace；不能拿当前FPGA假扮现成摄像头/显示合同。研究范围不应被现平台绑死，但平台更换成本须显式进入比较。

短期风险为**中高**：问题来源比“补SVA功能”扎实，前作差异可明确，但真实同实例共置/预算和余量仍缺，现行作者给出强反解释，当前FPGA配置对D3又缺能力。因此本族可作为独立第2候选参加root比较；“第2”只是导航编号，本席不作跨方向排名，也不因A2成熟度或当前缺板轨迹给它默认低排名。下一步应先做一次便宜来源/轨迹判别，不先承诺新协议/硬件。若只能得到普通策略适用条件和工程修补，诚实按该范围交付，不能借工程工作量充当论文创新。

## 8. 亲核导航与阅读覆盖

- **最需root直接读的新原件**：`sources/smmu-range-concern.txt:16–66`与`sources/smmu-range-response.txt:28–40`；`sources/intel-vtd-rev5.txt:7628–7653`；实际`linux/drivers/iommu/riscv/iommu.c:1799–1817`及`rv_iommu_iotlb.sv:217–244`。
- 官方当前文档本地原始HTML、可导航txt及SHA/时间：`sources/retrieval.json`；SMMUv3双方讨论`sources/smmu-retrieval.json`。Intel官方PDF SHA256=`a136448f2a14d49aa524106f58a139a5add90f78eef8a6abaabcc917ebc753b6`，本地`sources/intel-vtd-rev5.pdf`与`sources/intel-vtd-rev5.txt`。以章节为定位，不宣称整本规范审计或依PDF分页给出未核页号。
- **S4 Linux dma-buf**：[官方文档](https://docs.kernel.org/driver-api/dma-buf.html)，本地`sources/linux-dmabuf.txt:570–610`；只核revoke、同步与CPU访问段，不宣称全文所有驱动已符合契约。
- **S5 Linux HMM**：[官方文档](https://docs.kernel.org/mm/hmm.html)，本地`sources/linux-hmm.txt:175–186`及相邻填充/锁段；本次不据它声称当前平台HMM已实现。
- **S6 Linux SVA/ENQCMD**：[官方文档](https://docs.kernel.org/arch/x86/sva.html)，本次核Background、Process Address Space Tagging/PASID lifecycle，notifier+ATS/PRI已有；不把x86方案当当前无ATS iDMA能力。
- IDYLL、ShareMMU、rIOMMU、Scalably、Bermuda的原PDF/txt继续复用`out/research-landscape-20261005/papers/`，本报告所列段落为本席实际定向阅读范围，未冒充全文复现实验/实现核验。
- 没有为旧模型结果重跑检查；保存来源是文献处理，不是新平台实验。新编译/模型/RTL/板/原生应用运行均为0。
