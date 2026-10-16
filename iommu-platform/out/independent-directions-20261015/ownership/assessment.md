# 有限资源、翻译准备与所有权：独立方向判断

2026-10-15；本席为独占机制判断席，Codex-only。结论不预设主会话应选哪个其他方向。使用academic-research-suite的deep-research问题收敛、方法比较与原件核查指引；没有启动完整pipeline、问卷、实验、模型、构建、板操作或goal。只写本目录。

**本思想族最值得保留的是B1：给非ATS的片上DMA提供有限、可撤销、与payload提交分开的翻译服务，并证明一种有用且可部署的成本点。它现在适合作为有明确首关的实现型候选，不适合直接押注两周实现。** 合法先行窗口有真实程序依据；但“提前翻译”“保存翻译以后再用”“结果失效握手”“等buffer释放再搬”分别已有原作或具体普通实现。当前缺的不是再写一套owner词汇，而是自然任务中确有尚未消除的翻译关键路径，以及候选相对可执行预热/pin/PA队列具有哪项实际能力、部署或全成本改进。

这不是要求所有构件首次出现或必须胜过自己的等价实现。O-PAQ可以是B1的实现；若做成一项有意义的片上接口/集成贡献，应以准确的新接口、实现与完整证据立论，不能把同一控制程序换名后说成新调度算法。**本轮没有找到足以建议优先研发的独有新动作，也没有证明本思想族整体无价值。**

## 1. 覆盖了哪些历史方向，关闭恰好到哪里

下表复用已接受成果，只重新核对决定当前判断的原件。没有重跑旧检查。

| 历史方向 | 已知决定性证据 | 应停止的主张 | 仍保留的空间/资产 |
| --- | --- | --- | --- |
| 父内复用、当前可见父提前准备、前置/后置位置 | `docs/parent-translation-reuse-boundary-20261008.md:7–13`：lookup98→66，实际改善约0.3%，旧后置可见6父；`docs/prepared-source-bypass-boundary-20261008.md:9–26`：正常翻译后的物理缓存旁路消掉97.45%旧差距 | 不能把不等可见范围或绕过正常TC等待所产生的差距都称独有调度新意；旧23.95%差距不是该规则净收益 | 已完成的真实DMA/值/原件，0.7958%局部残差的精确条件；不扩大成全系统等价 |
| LTI-style寿命反馈、预算关闭债务、原序准备 | `docs/translation-lifetime-and-innovation-gate-20261008.md:7–14`；本席亲核TC `transaction_controller/rv_iommu_trans_ctl_wrap.sv:693–702` | 当前正常TC等待响应恒ready，不能凭AW/W背压虚构“共享TL结果等数据完成才释放”的瓶颈；PR/PW未实现不是创新 | 准确翻译结果/数据owner寿命区分；其他实际协议必须另核 |
| 共享walk驻留、慢subscriber | `docs/translation-continuation-frontier-20261008.md:13–19`及TC上述源码 | 当前正常接受端不存在假定的拒收瓶颈；deferred relookup、普通结果槽/位图已是强对照 | 有拒收端点的新接口可以研究，但它不是当前已证需求 |
| phase-union、partial G restart、固定15步进度前沿 | `docs/translation-progress-frontier-20261008.md:5–26`：固定五族受限模型的普通checkpoint达到30 PTE下界；续走文档29–35、53–69 | 固定链调度、删进度字段、提前yield本身停止作为独立机制；不能将B位frame当总面积或任意编码下界 | 未知路径/持续到达/同预算PWC的真实边界未被定理覆盖；若研究它们须先有新问题和锐界，不能仅以“更难”立新 |
| 无关失效与fault记录容量 | `docs/translation-continuation-frontier-20261008.md:21–43` | 范围比较/精确抑制、普通FIFO/credit/VOQ已经能表达；不能把静态fault0结果解释成故障瓶颈 | 精确协议实现与自然故障需求仍可形成工程工作，当前没有新方法收益 |
| R2原位头复用、正常WRITE许可提前准备 | 本席直接读 `out/r2-ownership-cost-20261013/root-result-acceptance.json:7–20,46`：O-B5S同状态/端口/事件；具名管理状态各1329bit加相同公共项 | 关闭本版原位union/原序许可的独有动作与逻辑成本优势；不能再用双份prepared头作唯一弱对照 | 已接受入口AW之后的精确生命周期、正常fault清债、合法字段回收；不是发表覆盖，也不是所有物理实现相等 |
| CO1完成依赖与翻译最后用途 | `docs/completion-obligation-design-20261014.md:13–27`：OC1-min用同stage反馈/位图/release-path/defer精确实现 | 本版阶段引用保护、释放链排序与最后用途的独有规则/必要逻辑成本 | 翻译用途退休早于数据/任务退休的接口事实、实现素材；新的架构能力另判 |
| RCS1完整翻译集合 | 本席直接读 `out/rcs1-completion-set-20261014/root-result-acceptance.json:5–33`：18-key有真实不同选择；同一个SJF在四条件完成时间均不差 | 仅关闭冻结“最大完整剩余页集合数带来完整性能优势”；不能再用集合数当完成指标 | E_q精简规则与动态/依赖条件未被全族普通等价证明；原64B fullbuffer条件模型错误必须保留，不能说native收益 |
| BR1阻塞传播替换 | `docs/blocking-replacement-20261014.md:11–27`：固定单节点DAG由StaticCP精确化简；19copy图还有12个outside-H victim | 本轮固定条件的独有完整收益入口关闭；“考虑关键性/停顿”宽泛动机已有前史 | victim会改变后续服务/顺序/residency的动态族未被该式覆盖，但当前无正例，不宜换名立即重开 |
| SDH1同bank必要源read hit | `docs/source-drain-hit-20261014.md:9–27`：O-NBO同source/tag/OUT路径复现；冷偏序仍未经native | 本冻结source-drain资格规则的独有动作与成本；不能只放宽bank号而漏结果owner | 精确full-source路由、W生产者证书、实际值原型；片上非阻塞实现的实际成本未证明相同 |
| LR1源读完成后局部撤销/归还 | 本席直接读 `out/local-read-reclaim-20261015/root-result-acceptance.json:5–20`：普通phased/targeted同事件；Intel有相关排空但不是DMA整页receipt | 关闭本冻结规则独有策略/已证明状态节省；不能据此说完整协议已发表或所有native同成本 | accepted-parent frontier、PA alias/full reader闭合、Rdone先于W/B源码偏序、功能值参考；ACK实际完成时窗口未知 |
| B0跨payload边界提前翻译 | 下节ICCD2018原文 | 停止“地址先知道、SPM还忙时纯翻译”“进度限定lookahead”本身的新意 | B1的片上服务接口/部署/成本问题；不能仅因生产没接好就称创新 |

SDH1、BR1、CO1表项是有限历史范围复核，本席未重新复判其全部程序/证明；R2/RCS1/LR1稳定接受与B核心原件为本轮直接核查对象。历史文件中的旧“停止”按2026-10-15新规则作上述窄解释。

## 2. 真实问题：两个owner不是同一个权限对象

固定HERO应用 `out/workflow-stream-contract-20261012/source-evidence/hero/openmp-examples/mm-large/mm-large.c:44–70,164–228` 给出实际双缓冲：当前计算读B槽，下一条带已有DMA，下一次复用同一B槽的host源地址可由`t+2`和固定stride预先算出。`omp for`没有nowait，当前计算结束前不能覆盖该槽。该事实不需要人为加延迟或后台流量；它只证明**合法先行窗口**，没有证明当前Genesys2的残余stall或加速。

容易混淆的是：未来host源页的mapping权限与当前SPM槽的payload占用是不同对象。准备B(t+2)的host源翻译并没有把当前SPM写权转给别人；应用仍须等计算释放SPM槽再搬运。因此“跨payload owner”本身不产生跨保护域的新授权能力。只要普通纯翻译预取不写SPM，就可合法进入同一窗口。

stage117/DPDK的已发布RX buffer已经给后端写权，不能当成“仍被旧consumer占有而不能写”的正例。已有stage117完整应用结果也不能换压力再造差距；旧平台复核的255次交接没有额外槽气泡，不能把源码窗口写成旧运行已观察到的损失（`out/research-reappraisal-20261015/candidates/b-development.md:22–36`）。

## 3. 最近原作的实质覆盖与未覆盖

| 原作/规范 | 本轮亲核决定方法的位置 | 已做什么；代价和边界 | 对B的准确影响 |
| --- | --- | --- | --- |
| Kurth等，ICCD2018，*Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine*，[作者稿](https://arxiv.org/abs/1808.09751v1) | `out/shared-translation-literature-20261006/papers/socdma2018.txt:272–298,336–347,371–412`，PDF4–6，§IV-A/IV-C；本席读实际文本 | compiler DDG+worker进度限制d/D；prefetch hit不下发payload，miss走软件handler；短burst retirement替代整DMA长期锁页。helper PE/进度通信/软件队列/锁/TLB/PTE服务要计；hybrid miss/error/drop/retry与当前硬件normal wait不同 | 直接覆盖B0宽泛动作。没有核其完整实现，不能说原作已经实现本候选的每个owner标签或现RISC-V适配 |
| Cong等，HPCA2017，*Supporting Address Translation for Accelerator-Centric Architectures*，[作者稿](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf) | `accelerator_translation2017.txt:393–436,528–588`，PDF6–9，§IV-B/C/D；本席定向读方法 | private/shared TLB、hit-under-miss、host PWC/cacheline与warmup；增加port/queue/demux并有shootdown要求 | 不能把非阻塞、跨accelerator驻留复用、PWC作为B新意。原作没提供SPM-release应用票据，不代表这种票据就有贡献 |
| Koenig等，DATE2025，*Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs*，[作者稿](https://arxiv.org/abs/2502.17398v1) | `riscvsva2025.txt:182–298`，PDF4–5，§IV-B/C，Listing1；本席定向读方法与评价口径 | host/PTW共用LLC、payload绕LLC、映射前flush/之后host self-invalidation；该实现中LLC显著压低翻译剩余。kernel计时排除host offload/sync | 必须比较PTE缓存及合法映射预热，不能只用无缓存高延迟walk。其小开销数据不能移植为本机结论，也没覆盖所有多DMA干扰 |
| ATS正式协议，RISC-V IOMMU v20260222 | [Data Structures](https://docs.riscv.org/reference/iommu/v20260222/iommu_data_structures.html)，Translation control/PCIe ATS translation request handling；[Queue](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)，IOMMU PCIe ATS commands；详见本目录 `primary-protocol-notes.md` | translation completion与后续translated访问分离；保存权限/粒度，设备ATC与失效响应有正式语义 | 因此B1“先保存结果、以后用、失效ack”的通用接口主张也不能轻率保留为首次。ATS没有掌握应用SPM owner，也不能证明小型片上非ATS适配等成本 |

论文精确书目、旧原件hash和已核/未核边界见`out/research-reappraisal-20261015/literature/b-cards.md`；本席不宣称重新全文精读所有旧论文。补核ATS1.1只取得官方公开metadata，没有读其完整PCIe正文。所以上述标准覆盖限定于实际读到的RISC-V正式行为，未宣称逐条协议等价或全领域查新。

三作并无实际矛盾：ICCD2018追求预取与可重试DMA，HPCA2017追求层级缓存与host walk，DATE2025在特定RISC-V平台证实PTE缓存可显著削弱问题。方法/平台与计时合同不同，不能把一个的百分比当作另一个的收益上限。这里的跨作判断是条件差异，仍待作者接受，不是穷尽文献冲突清单。

## 4. 新亲核：本平台已经有一个纯翻译前门

这补充并收窄旧B文档的“当前没有future-span登记+纯prepare+采纳协议”：完整B协议确实没有，但不应省略已有debug translation服务。

- `cva6/corev_apu/tb/ariane_soc_pkg.sv:110–129`：IOTLB16、InclDbg=1、InclATS=0、56bit PA、64bit AXI。
- `.../rtl/software_interface/rv_iommu_sw_if_wrap.sv:548–575`：debug寄存器接`rv_iommu_dbg_if_ctl`，独立TL请求/响应端。
- `.../rtl/software_interface/rv_iommu_dbg_if_ctl.sv:58–70`：使用IOVA/DID/PID/PV/priv及R/W请求；75–86返回fault/PBMT/superpage/PPN且ready恒1；93–104为一次请求生命周期。
- `.../rtl/translation_logic/rv_iommu_tl_wrap.sv:173–213,1329–1335,1544–1567`：debug共享normal翻译服务；正常walk后IOTLB hit方可完成；debug结果直接回寄存器，不经TC payload AR/AW。它会与normal请求争服务，normal优先；不能当免费并行PTW。

因此**O-DBG：受信CPU用debug寄存器提前翻译同真实DID/PID地址，再按原native DMA访问**是当前源码允许、值得先核的软件预热强对照。这里还没有实写/实测该程序；须核MMIO访问权、触发/完成语义、并发寄存器互斥、全部fault与供给开销，不能声称已部署。它没有未来span登记、pin、自动失效的独立PA结果容器或DMA安全采纳，不能把debug返回PA直接当可绕过正式权限的通行证。

本事实使首项判别更便宜：先比较合法纯翻译预热是否已满足自然任务，而不必先给生产RTL添加一个新prepare ABI。已有源级MMIO接口存在不是已测性能结论。

## 5. 最强B1合同及具名完整对照

研究问题限定为：**在payload槽不能扩容、future地址实际可得的片上DMA中，最少多少额外可实现服务状态/端口与软件费用，能使翻译结果在真实payload-ready时可用，并保持正式权限与完整数据owner语义？** 这可以产生有意义的接口/实现结果；若要升级为新算法，必须指出普通规则之外的准确新选择和证据。

最小新接口草案：`prepare(context, span, R/W, application-token)`只取得有限K个pending/ready credit之一；返回完整许可和范围或正式失败。`adopt(result-token, native-request)`只能在原请求正式允许且实际接收握手时移交，原owner直到最后一份身份/权限已被新owner接受才能退。应用payload-ready仍由真实计算/consumer依赖保证，不要求IOMMU复制整应用DAG。失效使结果不再可用；已承诺VALID、accepted R/W/B与旧owner仍按正式规则退休。稳定mapping可先是明确合同，不能无故加异步remap来制造需求。

这份合同避免把整个应用owner账本塞进IOMMU，仅在需要硬件强制SPM保护时才另立新合同；当前没有这种需求证据。K限制状态，不保证deadline或净完成收益。

| 具名比较 | 必须允许的能力/成本 |
| --- | --- |
| O-DBG，现源码debug预热 | 同future地址/上下文/应用ready；提前walk后仍走正常native lookup；计全部MMIO/CPU、串行争用、cache污染、重walk及error处理 |
| O-HTP，ICCD2018式有限进度纯预取 | 同样lookahead与已知依赖；可以在payload尚不ready时运行；计helper/通信/PTE/缓存机会成本；不能强迫它等待release才预取 |
| O-PIN，有限pin | 用同一新增前门和K约束，准确首次/最后翻译用途；允许早解除，保留正式失效。计占用cache、受限victim、metadata及阻挡其他demand；不强迫整任务长pin |
| O-PAQ，有限prepared-PA队列 | 同真实权限/粒度/context/owner关联与adopt；允许ring、union、阶段字段回收、相同失效。它可以是B1实施方法，不能拿它的换名等价单独否定接口架构价值 |
| O-ATS/local ATC | 以正式translation/translated/失效语义为基线，比较片上适配真实成本；不能要求本候选搬上PCIe PHY/完整端点而把其全部成本罚给普通 |
| O-READY/已有依赖队列 | 与候选同信息/K/前门/结果容器；实际没有deadline就不用假EDF期限。若只是按release-ready排序，不能独称新算法 |
| 普通正常译址后物理缓存、PWC/LLC、驻留/大页及多payload槽 | 开放合同允许的配置并计真实完整成本；额外payload槽也可作面积/持有/流量替代，但不能默认为免费 |

B1与R2的实质差别只在承诺时点：R2已有真实入口AW，由cell持有整头；B1要服务尚未允许payload DMA发起的future span，因此新增前门/请求身份/返回容器必须付费。R2未覆盖这个时点，不足以否定B1；ATS/预取又说明该分离本身不是未见过的能力。片上具体实现若能用更小真实状态和端口达到一项有用目标，仍可形成系统贡献。

## 6. 成立条件、邻反、全费用与第一项廉价判别

先用符号说明有用窗口，不冒充性能：a为future地址/正式context可得时刻，r为payload首次合法提交时刻，普通demand翻译服务L。若能在自然区间[a,r]完成并保存有效翻译，且原本后继确实等它，理想release后残余可由L降至`max(0,L-(r-a))`。这只是局部上限，O-HTP/O-PIN/O-PAQ可同样做到，不能当B1独有胜例。

**决定性正例必须同时有四个事实。**

1. 同一真实程序/映射/资源配置中，future地址可得但payload槽自然不可复用；记录实际release/lookup/consumer chain，不能把source code偏序当测得cycle。
2. 正常预映射/PWC/LLC/允许大页和普通提前预热后，仍有翻译残余阻挡完整有用完成；不能只看walk数、最后一个prepare或一个owner提前。
3. B1给一项准确新能力，或一个同完成目标的实际部署/完整成本更佳点。若只是O-PAQ的一种实现，应诚实把贡献定为接口与实现，不伪造策略独有性。
4. 正例的所有payload/CPU供给/cache交接、错误/最终释放、全部额外服务均入账；一个固定可执行强对照贯穿正反条件，不拼事后赢家。

相邻反例应只改变一个实际条件：未来地址依赖尚未完成的数据，因此a≥r；普通debug/HTP已经隐藏walk；结果在r前被自然驱逐/失效；新增prepare争走单PTW导致另一个真实任务更慢；payload或最终consumer才是关键路径；CPU/lookup/adopt费用超过藏掉的剩余。无async remap的任务不必特造remap反例作为主性能场景，但安全合同不可隐去其适用范围。

完整成本至少包括：span/context/权限登记；K个pending/ready/fault owner和result内容；命中/替换/pin及比较器/读写口/仲裁；单PTW/PTE/cache/内存服务和废预取；adopt与heldVALID；正式失效/取消/error/结果退休；应用旧新payload owner和所有R/W/B债务；MMIO/helper/CPU/cache维护/通知/最终consumer；payload全部字节和占用；物理FF/LUT/BRAM/Fmax/能耗。字段上界不是面积，未知项不得记0。

**第一项廉价判别不是新模型：** 从固定HERO循环与既有运行/合法配置中，仅构造一张B(t+2)的“地址可得→当前槽最后读→下一payload→consumer”偏序和翻译key/使用次数账本，明确O-DBG/O-HTP提前可做的服务。随后查现有trace是否已含r之后真实翻译等待；没有trace就明确缺证，不以手填L替代。如果现有纯预热在相同资源下已经覆盖，保留其工程接口，不投入新prepare/commit微架构。若确有自然残余，再明确候选与PAQ/pin的接口/状态差后立一次最小验证合同。

本轮已完成该首关的**源级前半部分**：合法窗口成立、普通纯翻译前门存在、发表的进度预取可进入同窗口。后半的实际残余/净成本证据未取得；已有workflow-stream复核没有额外槽气泡，stage117则没有观察到所需重复占槽/释放/新payload序列。不能把“下一次测量可能有收益”写成正向立题证据。

## 7. 两周窗口中的价值与取舍

B1比重新调RCS集合分数、BR1固定图、R2字段回收或LR1脚本更值得保留，因为它有一个独立的真实接口问题：非ATS片上DMA如何合法消费提前取得的翻译，而不提前占用payload。它也能复用当前TL/debug、普通物理旁路、full-source/错误owner资产；无需从0重建所有部件。

但当前两周内的主要风险是两项相连证据都缺：HERO自然窗口尚未与当前四DMA共享平台的实际翻译关键路径绑定，且可用的预热/缓存/PAQ/pin已经相当强。实现B1还需有界结果owner、native采纳和正式失效关联；这些是低层协议与真实成本工作。只完成原型可证明能做，不能自动回答为什么论文需要这个实现。固定链理论方向又缺自然外生pause需求，不能因为已有6,450点结果就优先。

所以，本席在族内保留B1，建议主会话把它列为**证据触发的后备候选**：若能够先拿到自然残余和一项具体部署/成本差，便可以转为两周内的有界实现题；若首关仍只有源码合法性与普通方法同效，就不投入两周追逐性能。此判断只关本思想族，不隐含A或任何其他方向较优。

可核查novelty段落：ICCD2018已利用worker进度在payload访问前做纯翻译，ATS正式协议已区分翻译结果与后续数据访问，HPCA2017/DATE2025又给出分级缓存和PTE缓存的强替代。当前HERO源码显示future host地址可得而SPM仍忙，当前IOMMU也已有debug翻译前门。我们可研究的新增部分因此应是一个面向非ATS片上DMA、有限且正式可采纳的服务接口及其有意义的实现成本点，而不是提前walk、owner票据名称或ready排序本身。**如果该实现相对准确普通控制给出新的可部署能力或同完成目标下更佳完整成本，就有独立系统贡献；如果只是同协议同状态的PAQ再命名，就不能将其作为新算法主贡献。** 当前证据支持问题结构和接口缺口，尚未支持后一项正向贡献；不是全领域首创/覆盖的最终结论。

## 8. 原件入口与未核问题

- 本席直接关键原件摘录与hash：本目录`critical-source-excerpts.md`、`source-manifest.json`，共11个原件；原始位置均保留。
- 发表前作完整旧卡：`out/research-reappraisal-20261015/literature/b-cards.md`；本席所读方法精确位置见上表。
- ATS本轮官方核查范围和失败留存说明：`primary-protocol-notes.md`；未假装取得完整ATS1.1正文。
- 未核：debug软件程序与MMIO供给实测；HERO在当前硬件/映射的真正时间trace、cache/PTE命中与consumer关键路径；PAQ/pin/onchip ATS的完整可实现状态和端口；异步remap/取消安全全路径；物理时序/资源/能耗；新接口在真实多publisher工作负载中的部署价值。
- 0新模型/实验/RTL/driver/ABI/构建/native/FPGA/板操作，0外部模型，0旧席/goal恢复。只做源核、原作方法比较和条件设计。
