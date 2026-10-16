# 共享IOMMU重新立题：三个具体机制构造、前作交叉核查与投入裁决

2026-10-16 JST。基准与本轮输入HEAD均为`c981e014d42368409e8bb64b4259570803ca492d`，分支`main`；原有已跟踪工作树差异仅为无关应用日志两行，不纳入研究。旧构建目录/二进制的未跟踪文件不触碰。

**本轮没有找到足以立项的论文机制，因此不强选论文主线，也不启动下一项机制性能实验。**下面三个设计在控制位置、所改决策及资源责任上不同；但“可设计出三个结构”并不等于“已有三个创新”。续行walker的核心结构已有直接发表前作，译后分段的关键动作已有前作，范围回收则尚无普通方法之后必须解决的源绑定实例。它们均不应以“再补一个成本数字”转入开发。

这不是对共享IOMMU问题域的否定，也不是声称所有合理新机制都已被穷尽。本轮具体覆盖walker执行粒度、物理服务承诺和撤回完成范围；未对Chiplet放置、真实多加速器不规则访问或全部缓存组织作新的全面查新。

方法为ARS-Codex的有界问题发展、最近工作核读和反例推导。用户明确要求主动生成候选；不运行完整pipeline、例行复审或外部模型服务。Codex-only：历史核查请求Sol/high，服务设计请求Sol/xhigh，开放walker设计请求Astra/xhigh；实际独立运行元数据unknown，请求配置不作运行证明。没有新模型、仿真、综合、板测或生产RTL/Linux修改。

## 1. 现状判断：真实问题成立，缺的是哪一类贡献

### 已确认的科学事实

| 事实 | 直接证据及本轮核查 | 能说到哪里 |
| --- | --- | --- |
| 跨设备IOTLB容量替换 | E2 W8原始[record](../out/mac-research-two-bit-20261003/e2/runs/e2-boundary-w8-forward-n112-r1/record.json#L1)：V两项warm驻留；A事件16/17覆盖；probe前均消失；首再用2miss/2PTW。字段解码见[研究结果解析器](../scripts/research_results.py#L306) | **实板局部因果**。W7/W8的16/18项是该布局边界，不是任意策略、稳态或应用吞吐结论 |
| A miss延长后来V hit的等待 | E3四组[原始配对](../out/mac-e3-controlled-20260918-164635-run-rk48bhte/all-original-pairs.json#L1)按D128−D0归一，A/V剩余时间均同增127/129/129/130；V first_hit=1 | **实板受控因果**，64B、一次启动、人工延迟及特定到达方式。旧镜像记录仍有hash_verified=false，不能升级为完整镜像重认证 |
| 翻译与payload最终共享服务，可出现优化反作用 | isolated实板[P](../out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mp/record.json#L148)/[PN](../out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mpn/record.json#L148)的A miss均2048，PN仍慢；M0[原表](../out/workflow-g0-m0-20261010/results-table.csv#L7)在另一合同中出现并消除差额 | **板测现象＋模型内因果分开**。不能用M0倒填实板全部根因；DS还汇合CQ/DDT/PDT等，不是纯PTE |
| 软件供给/发现、设备完成和消费者完成不同 | E4[正式报告](e4-results-review.md)把旧软件慢化的大部分落点定位在硬件done后至读取DONE；40个旧共跑主样本AXI重叠0。R107和M0后来采用不同完整完成合同 | **已有板测/协议资产**，不是软件延迟全部由CPU某条指令造成；不同计时分母不能拼接 |
| 新接口可工作却没有控制机会 | First-PTE[原接受](../out/first-pte-paired-local-20261016/root/root-result-acceptance.json#L33)：A末源RLAST3507，M arm3514，实际mask0；base/M cold4481、hot1749且记录事件相同 | **局部RTL＋行为DDR**。结束固定COPY版本；不再调相位、特殊冷态或扩M，不重跑六案/旧70条 |

原始身份、字段和本轮抽核范围见[历史证据§9](../out/research-reconvergence-20261016/failure-evidence.md#L187)。当前事实不支持“尚无真实干扰”；也不支持“既然有干扰，任何更复杂控制都值得做”。

目前缺少的不是另一份控制器，而是一个**具体的增量链**：正常任务确实需要某项服务；具名合理方案在给定成本下仍有不足；新结构用当时可得的信息改变合法行为或实现成本；完整结果受益。机制可以利用既有构件，成本贡献也可以成立；不要求任何普通程序都无法复现候选。

## 2. 失败机制归纳：不能给所有负结果套同一个原因

| 过去候选 | 核实的失败/不足 | 不能补写的解释 |
| --- | --- | --- |
| p5实际占用计费 | 24份原始metrics重算，四格相对保护匹配最强p1–p4的A增益为−5.69%、−8.24%、−5.15%、−4.44%。[计费RTL](../cva6/corev_apu/fpga/src/dma_research_policy.sv#L284)确实计REQ_TRANS/AWAIT_RESP；控制的是新COPY准入和窗口预算 | **为什么输的具体根因仍未知**。信息冗余、反馈迟、动作粗或参数不佳是待区分解释；不能说已证明漏计DDT/PTW，亦不能从owner总时间反推外部损害 |
| P/N/PN直接组合 | 分区解决驻留，hit旁路改变payload到达，二者作用于不同资源。isolated实板慢化12.67%–12.79%仍在；两项局部优化不保证完整任务相加 | “PN反向”不是新方法；实板剩余约0.90M周期未完全归因，不能把AW not-ready等重叠窗口相加当根因 |
| 从M0差额推新门控 | M0 FIFO中P/PN的V时间86594/90283；DS优先后均76738，双方和收尾也相同。[实际规则](../tests/nonblock_actual_dma_test.cpp#L679)在未held时旋转C++已接收的跨ID返回队列 | **普通模型服务规则清除了该合同的差额**，并非证明MIG免费提供这种重排或旧实板已解决 |
| First-PTE M-ID | 当前正常COPY从未触发真正mask；意图身份/协议能工作，但可改变的A请求已读完 | **缺实际控制机会已证**；不是少几个bit、缺新tag或需要更强推理。不会重开这份固定程序 |
| 映射接纳/复用 | 累计超额阈值就是O3；持久池/首次使用建图已有强前史。27对29的CPU小计仍要求`K+10h+ΔG+ΔL<2`；stock也没有idle B pin | **策略等价已证，完整成本相等未证**。不能以未知成本维持独立算法主张，也不能推出整个接口组织或所有原生实现无价值 |
| D-CS1付费观测 | 主配置免费Z信息价值0；八配置付费下界均输可执行固定动作。H1一个反号中PN无fast response，普通P等时间wait1复现provider工作及target186 | **信息无额外决策价值/小价值被费用吃尽是模型内结果**；不能由此诊断p5，或称真实SPDK已有同一债务/分布 |
| R107恢复翻译 | [原receipt](../out/r107-ordinary-pilot-20261012/repair-1/live/finite-pilot-result/receipt.json#L1)的恢复1728lookup全部hit、0walk；8个2MiB叶，冷准备和驻留有费用 | **该有限all-ready卡没有展示可避免walk残余**；不是所有AI任务都不需要翻译优化，也不是准备/内存成本为0 |

细节与原始路径见[失败证据§2–6](../out/research-reconvergence-20261016/failure-evidence.md#L20)。本轮没有重跑上述实验。

由这些结果能抽出的共同认识是：

1. **观测精度、可控动作和有用结果是三个环节。** D-CS1能证明某些信息不改变最优动作；p5只能证明该控制器没有兑现额外进度，不能越级诊断原因。
2. **局部提速可能把工作推向下游。** PN在声明的M0模型中属于这种情形；缓存命中数、PTW占用或首PTE时间单独都不代表净收益。
3. **普通方案的准备、等待、合法大页和原有缓冲是实际能力。** 删除它们可以制造候选正例，却不形成贡献。反过来，把候选新增接口也交给普通策略，只能说明策略不独占，接口价值仍需另判。
4. **缺口各不相同。** First-PTE缺机会；D-CS1缺付费信息价值；R107缺该目标残余；mapping缺独立策略及完整净收益；p5和实板PN仍有未识别根因。不存在一个已证的“所有候选都只是信息不够”总解释。

## 3. 三个不同机制候选：先构造最强版本，再作有界裁决

### 候选一：借用TC保持状态的多上下文续行PTW

**A. Problem。** 一个共享walker等待A的一次PTE读时，V的miss即使已在另一个bank登记，也可能不能开始。N仅帮助合格hit，无法让第二个miss在等待期间推进。现有PTW的`IDLE/MEM_ACCESS/PROC_PTE`和单套pointer/level状态可直接核于[PTW](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv#L90)。E3支持串行等待问题，但并未测本候选的新收益。

**B. Root Cause。** 长时存储等待和短时PTE解释共用一次完整walk所有权：`发PTE(A) → 等R(A) → 解释/发下一层(A)`，V不能借解释器/存储发射口。瓶颈是等待期间上下文不能独立保存及返回匹配，不是未来PTE内容未知。

**C. Existing Solutions。** 除当前单walker＋N/P，还必须比较：①两个完整walker；②**GPUMMU2014的共享执行器＋多个page-walk buffer**；③同上下文数、可复用TC的普通barrel walker；④DWS/ISCA2018 walk scheduling和普通PWC。不能只胜复制两个完整walker。

**D. 最强具体设计。** 每bank保留一个续行槽，复用TC已持有的原请求身份/IOVA；S1初版保存next-PTE pointer、level、global累计位及实际不可重建的权限/context状态。首版全局仅一笔outstanding PTE，在消耗其R、尚未offer下一AR的边界换入另一个就绪slot；它不能隐藏当前这笔未返回PTE的等待。若要在等A的R时另发V，必须再付返回owner/ID、状态及实际端口能力。真实PTE握手后保存下一状态，已发布VALID不撤回；错误返回其原owner，取消/失效时保留已承诺R的接收与丢弃责任。新增按bank读TC的接口/mux和寿命约束，不能免费借另一bank尚未暴露的header；TL解析的context也不能假装全部在TC。

**E. Expected Benefit。** 在正常并发miss且有可并行存储服务时，缩短V等待或提高吞吐，避免复制完整执行逻辑；同时计完整双方COPY/B/quiet、上下文/返回存储和选择路径。底层单一非抢占队列、全hit或PWC已消掉长等待时，收益可以为零。

**F. Falsification。** 最小判别必须让同资源普通barrel也保存两个续行、同样借用TC，再比较实际存储/端口或可达完成。若状态和可执行调度配对相同，不能做一轮“都比单walker快”的实验来支持新意。**本轮已核到这种核心结构配对，故不建议进入该实验。**

**G. Novelty Risk与裁决。** [Power/Hill/Wood，HPCA2014，§4.4、Fig9、PDF p7](https://research.cs.wisc.edu/multifacet/papers/hpca14_gpummu_appendix.pdf)已经明确保存outstanding address/state并共享walk状态机；本文还说明可扩展到其他硬件遍历ISA。不能把“GPU变IOMMU”当增量。TC保存118bit请求不等于相对强普通净省118bit，GPUMMU并未要求再复制完整总线header。邻域/前缀折叠又有MICRO2018 Neighborhood-aware PTW。**本版不立独立机制或成本主张**；这是发表结构＋尚无增量见证的适配，不是“所有PTW组织无价值”。完整设计、源码及正文定位见[候选一证据](../out/research-reconvergence-20261016/translation-candidate.md)。

### 候选二：译后物理读分段与有返回空间保证的服务承诺

**A. Problem。** PTE出现前，长payload可能已经取得无法撤回的物理服务；接收端回压还可能延长顺序返回。模型DS优先依赖服务者具有的选择权限，不能靠一个外侧优先bit复制。

**B. Root Cause。** TC把已翻译AR送往xbar；[仲裁器LockIn与AR spill](../cva6/vendor/pulp-platform/axi/src/axi_mux.sv#L333)一旦承诺，后来的PTE无权换头；顺序MIG返回也不能让PTE穿越旧burst。`RVALID&&!RREADY`仅证当前拍回压，不能证明后面的PTE已在内部ready。

**C. Existing Solutions。** 当前legalizer读写独立cap/decouple、prepared分段、current-VALID DS优先和fullbuffer；发表的MASK、AXI-REALM、RTAS2026队外控制与credit/VC；简单替代为**在TC后用普通透明fragmenter**，而非强迫每child重新翻译。

**D. 最强具体设计。** `IOMMU_COMP→xbar`读支路接adapter，对合法memory INCR把原256beat分为64beat children（示例、非已测最优）；原虚拟事务只翻译一次。发布前预约足量返回空间，至多一个未完child；真实child RLAST后才可发布下一child。current-VALID DS-first/F4在首次offer前选择，held不抢。保持原ID出现次序、原parent的单RLAST、RRESP、全部AW/W/B和错误责任；新增原header/beat计数、child credit及不可重复使用的空间预约。原512beat buffer仅在寿命/空间可证明时复用，不能把满额空间同时许诺两次。

**E. Expected Benefit。** PTE若在第一个child服务期间正常出现，可先于其余未承诺children取得服务，可能改善完整任务。代价是额外AR、startup/row locality、MLP下降与adapter控制；不把旧40步当每AR固有费用。需报告双方完成、全部工作、physical bytes、AR/B和quiet。PTE过迟、payload无回压或其他阶段掩盖时关闭。

**F. Falsification。** 强对照同享译后位置、返回空间、child限额和DS规则。如果它逐动作复现，就不能以比“只允许legalizer”的弱对照更快认新意。**这个普通构造在设计层已经完整表达当前动作，本轮无独立成本差项，因此不排等价性能实验。**

**G. Novelty Risk与裁决。** [AXI-REALM §III-A1/A2，PDF pp3–4](https://arxiv.org/html/2501.10161v2)已有1..256粒度、meta保持、R.last重构；[MASK §5.4，PDF p8](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)已有翻译/data联合调度；[CTC §3–4](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)已有端到端接收credit。它们不等于同一SoC已实现，但候选没有说明在同预算下新增何种能力。**将本版转为强普通对照，暂不投入。**完整正文核读、MIG端点和held-R变体见[候选二证据](../out/research-reconvergence-20261016/service-candidate.md)。

### 候选三：只等待被撤回范围旧代请求的安全回收

**A. Problem。** 精确IOTINVAL可以不逐出其他domain，但IOFENCE的完成前缀/PR/PW等待范围未必跟着缩小；回收Q可能等待不相干长请求R。正常map/unmap及迁移有可信来源，但**尚无当前部署的这种残余实证**。

**B. Root Cause。** 缓存有效性、已翻译且已处理的DMA、尚未到达IOMMU的请求分属不同责任。消掉TLB项不等于旧DMA结束；全局前缀又可能过宽。当前CQ的[PR/PW TODO](../cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv#L348)不是可用的正确排空基线，Linux[省略PSCID/ADDR](../linux/drivers/iommu/riscv/iommu.c#L1777)也只能先作普通修正。

**C. Existing Solutions。** 正确精确失效＋充分设备完成证据；per-domain/queue在途计数；合法批处理/持久池；Intel VT-d的page/PASID选择失效及相关读写drain。IDYLL和HATRIC已有缩小失效目标/一致性开销的方法，不能以目录、hash保守误报或代次命名立新。

**D. 最强具体设计。** 有限K个`{context,range,generation}`scope，在物理请求首次承诺前登记scope旧代义务。撤回时关闭该scope准入、精确失效、取消未承诺walk并接完已承诺R；只有旧代请求到实际可见退休点且不会再发布旧翻译，才ACK。其他scope不停。需要范围匹配表、计数、每在途索引/代次、跨页处理、更新可见性与wrap/fallback；设备停发/互连栅栏仍按真实合同完成，ACK前不复用IOVA。不能把普通B无条件当全局可见。

**E. Expected Benefit。** 若被回收范围已无旧请求，而无关范围仍在普通等待前缀中，可能缩短合法回收及不可回收内存驻留；CPU、元数据、payload物理并集和背景进度分别报告。映射存在本身不表示多保留一份物理页。

**F. Falsification。** 在同授权、同完成要求下，若正常设备完成证据已允许不发该PR/PW，或普通per-domain/queue计数/持久池足够，则停止。只有来源绑定实例还存在“同domain不同范围”的额外等待，且正确普通替代仍更贵，才有scope级硬件必要性。本轮没有这样的输入，**不自动启动采集器或实现来维持候选**。

**G. Novelty Risk与裁决。** [RISC-V队列规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)区分精确失效、IOFENCE及未抵达请求；[Intel VT-d Rev5 §6.5.2.4/5、6.5.4](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf)已有定向失效/drain。不能声称Intel必定等待全部无关请求，原文没给本轮所需的内部细节/成本。[IDYLL §6.2–6.3](https://par.nsf.gov/servlets/purl/10492257)保持即时TLB失效、用IRMB延后PTE写回；[HATRIC §4.1–4.2](https://arxiv.org/abs/1701.07517)用co-tag和缓存一致性缩小目标。这些不完全覆盖scope退休，但大幅压缩宽主张。**不立项：缺的是必须使用该能力的正常残余和相对具名近邻的差异，不能把未核当空白。**详见[候选三设计与来源](../out/research-reconvergence-20261016/lifecycle-candidate.md)。

## 4. 批判性比较与排序

下表不使用虚构发表概率或数值评分。“高/中/低”是本轮基于所列证据的研究判断，不是测量。

| 维度 | TC借用续行PTW | 译后分段/返回预约 | 范围旧代回收 |
| --- | --- | --- | --- |
| 问题真实性 | 串行等待有本地因果；并发miss需求合理 | 共享服务成立，当前正常COPY未给增量窗口 | 规范允许问题，当前正常残余实例未取得 |
| 机制具体性 | 高，slot/返回/TC寿命明确 | 高，位置/parent/credit明确 | 中，协议明确但全局可见点依部署 |
| 普通覆盖风险 | **极高**：已有threaded PTW，可借TC | **极高**：REALM式适配＋credit | 高：完成证据、分domain、持久池、已有drain |
| 最近发表工作 | GPUMMU2014、Neighborhood-aware2018、DWS | REALM、MASK、RTAS2026、CTC | IDYLL、HATRIC；VT-d规范能力 |
| 平台复用 | 高；仍需TL/context接口 | 高；纯读adapter并非已接入 | 低；PR/PW尚缺，真实实例/路径也未绑定 |
| 最小实验的辨别力 | 现定义与强barrel核心配对，性能试验不能证明新意 | 现定义与强fragmenter动作配对，性能试验不能证明新意 | 输入不足，不能用人工长尾代替现实需求 |
| 实现风险 | 返回关联、fault/inval、端口和长期槽寿命 | VALID/RLAST、credit、错误和吞吐损失 | 最大，撤回/可见性/迟到/复用语义 |
| 当前论文潜力 | 当前版不足；不以未知成本悬挂 | 当前版不足；保留为普通能力 | 问题保留，当前不足立项 |

**投入裁决：三个均不达到立项条件，因而本轮没有优先候选，也没有获准排队的备选。**候选二最明确转为普通基线；候选一结束本版结构新意/已证成本主张；候选三保留问题但不投入。不能为了交付“第一/第二/第三名”把不成立的主张升级为研究主线。这适用用户明确给出的“三者不足时不强选”分支。

## 5. 推荐主线与可证伪核心假说

**本轮不推荐一条已经达到论文立项条件的主线。**这不是“先选续行PTW，以后再解释新意”，也不是自动退回A/D/First-PTE或转投表征论文。

最强剩余的**问题假说**是：某些正常共享DMA工作在合理准备、驻留、分段和完成接口之后，仍因不可分割的共享资源义务出现可避免的完整工作损失。但它还缺具体`任务Y/条件X/动作M/成本C/结果Z`，因此**不是本轮交付的论文贡献假说**。不能把这句话包装成新机制或以它直接申请两周生产开发。

本轮确切否定的是三个已展开版本的投入依据：候选一已有原文覆盖核心组织且未找到净成本差；候选二普通组件能够完整表达动作且无独立资源优势；候选三既有完成/定向drain机会未排除。没有任何一项否定允许推广成整个共享IOMMU研究无价值。

## 6. 下一项决定性实验：本轮不发布无辨别力的实验

**新实验执行为0，下一项机制实验不启动。**对候选一再跑“两miss比串行快”、对候选二再跑“短burst让PTE早到”，都只能重验前作机制；不能区分本候选和具名强普通。对候选三注入任意无关长请求，又会把未证明的部署残余做成胜例。故这三项都不应被登记为“决定性论文实验”。

各候选的支持/推翻/普通足够条件已经在§3F列明。若未来有**具体新差项**，最小实验才应冻结：正常来源与实际可得信息、候选/最近方法/简单替代、相同状态与缓冲预算、实际承诺端点，以及完整`T_V/T_A/T_all/T_quiet`或CPU/不可回收内存。通过必须同时有来源中的可达机会、相对强普通的有用差异和合理实现费用；失败则结束该具体差项，不续接同一缺口的更多协议轮次。

这些是实验资格，不是一份要求用户提供痛点的清单，也不是对后续持续研究授权的疑问。本轮已经完成源级可判定的比较；没有用“还未综合”当作自动开发理由。

## 7. 后续两周：实施排期暂不启动

**本轮第一项实验没有一个有辨别力的候选可进入，因此不制造两周实施进度。**原10月15—28日立题窗口不重新起算，MIG与其他分支边界不变。

只有今后另一个具体差项已取得可区分的最小实验通过记录，才采用下面这个两周执行框架；它不是本轮自动任务：

| 相对通过日 | 可展开工作 | 结束/退回条件 |
| --- | --- | --- |
| 第1–3天 | 冻结单个机制、最近具名实现与简单替代；把通过例和最近失败条件接入现有局部夹具 | 发现强普通能力遗漏或输入来自不可部署的状态，停止该例 |
| 第4–7天 | 一次最小局部RTL，实现必要接口/状态；验证完整双方工作、返回与退出，测相同锥成本 | 同信息同资源无完整收益/成本差，结束该版本 |
| 第8–10天 | 一个未见正常任务/历史、一个相邻反例；必要时局部时序/资源评估 | 优势依赖调相位、少做工作、免费接口或新弱基线，撤回主张 |
| 第11–14天 | 汇总因果消融、实际费用和前作差异，形成论文贡献论证及是否扩大验证的决定 | 不把功能正确或一张条件图写成论文完成；无增量则收尾，不自动全板构建 |

这个框架不能被拿来恢复暂停实验。生产RTL/Linux、大规模矩阵、全板综合/布线/加载仍需后续具体授权；B1/I/D/A5/A2、mapping/p5/D-CS1和MIG维持既有结果与暂停边界。

## 原始证据、阅读深度与本轮资产

- [历史负结果与E2/E3原始字段](../out/research-reconvergence-20261016/failure-evidence.md)：不是全历史重新认证；p5抽核24个metrics、P/PN两原record、CS1两world/三轨迹，旧结果不覆盖。
- [续行PTW候选](../out/research-reconvergence-20261016/translation-candidate.md)、[共享服务候选](../out/research-reconvergence-20261016/service-candidate.md)、[回收候选](../out/research-reconvergence-20261016/lifecycle-candidate.md)：各自给出源码、正文页码、必要费用和接受/否决理由。
- 原文核读是关键章节，不是论文库全部全文。root直接读GPUMMU §4.4、REALM §III-A、IDYLL §6.2–6.3、HATRIC §4.1–4.2及队列/VT-d相关规范；其他具名前作的新增/复用范围分别列在独占报告。部分PDF结构预检UNAVAILABLE（缺pypdf），Poppler文本读取成功，不把它写成结构PASS。
- `references`已收录不等于已核读；本轮新阅读补记见[文献增量](../../references/research-reconvergence-20261016.md)。没有按“未搜到同名方案”认新意，没有窃取同接口等价实现来冒充独立旧方法。
- root直接检查计费、返回优先、PTW状态、DS仲裁、xbar锁定、splitter RLAST、CQ和精确失效等关键源码；独占席不是用模型一致投票判新意。
- [本轮接受与输入身份](../out/research-reconvergence-20261016/root-result-acceptance.json)记录范围、实际执行和保留事项。全程不改变旧First-PTE裁决，不重跑任何历史实验，不修改生产模块。
