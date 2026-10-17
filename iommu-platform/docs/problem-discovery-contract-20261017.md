# 问题发现实验合同：后台目的页再用与完整COPY等待

## Material Passport

- ARS-Codex `experiment-agent / plan`；2026-10-17 JST；`DISCOVERY-REUSE-1`；输入`main@4c83ac30385059b19a8830fea7720596724abb44`。
- **状态：具体提案已准备，实验UNVERIFIED；未授权实施/运行。**用户粘贴材料明确：“这里给的是下一轮任务建议，不是已经获准执行的新实验”。本轮只做源码核查、实验设计和工作规则修正。
- 目标仍是方法/机制论文。本实验先取得能改变设计选择的新事实，不以证明某个候选胜出为目的，不将表征本身自动认领为论文贡献。
- 新性能模型、RTL仿真、编译/综合、生产RTL/Linux修改、旧实验重跑、加载/板测均0。Codex-only；两个独占源码/覆盖席请求Sol/high，实际独立运行元数据unknown。

## 1. 唯一研究问题及竞争解释

**当V只复用固定两页、A源固定而目的页再用距离从分区内扩大到分区外时，已实现的P/PN＋承诺前DS-first/F4之后，完整双方COPY的主要等待落在哪里？其中是否还有能被合法普通控制改变的依赖？**

本轮只改变**A目的地址再用距离**，不再提出三个机制，不先要求创新成立。

| 竞争解释 | 应看到什么 | 什么会推翻/限制它 |
| --- | --- | --- |
| H1：A反复目的miss形成必要翻译链，跨bank串行或PTE等待暴露到完整任务 | 环回后仍有带身份的A目的miss/walk；等待中的任务确实需要这些PTE/翻译响应；改变合法映射组织后对应依赖和完整完成随动 | 只有首次冷填；V实际未可供给；或去掉该链后完整结果不变。单纯PTW active增多不够 |
| H2：主要残余是payload服务/已经承诺的债务或返回回压 | 关键parent的必需请求在明确端点等待，前序payload尚未退休；用VALID/READY区分真实回压、startup及排队 | 只看到握手空档、时间重叠或下游隐藏状态；这些仍记unknown，不能冒充因果 |
| H3：供给/写腿/完成发现决定结果，翻译精度或早意图并非当前要付费的能力 | hardware DONE后至下一配置/issue的空洞，或源已全部返回后AW/W/B的必要前驱，解释较长parent；完成口径改变不了实际依赖 | 后台进度使前台一直busy不等于没有供给空洞；现有观察不足时不得强行选H3 |

三者可能共同存在；不要求将所有周期硬分给一个原因。成功是排除至少一种解释或确认/否定一个实际约束，**不是候选加速达到某个百分比**。若观察仍不能区分，准确交付unknown与缺失的具体事件，不自动扩大实验。

## 2. 工作流、输入和普通基座

### 正常外部程序与单一行为轴

两真实iDMA各完成**64个正常、4KiB对齐、4096B、batch-OFF COPY**。每lane只保留一个未完成parent；固定源内容，V目的恒定，A第j个parent选择目的`j % R`。全部映射在reset/开始前建立，运行中不改PTE、映射、IOTLB或源数据，不逐parent清缓存/poison。

供给顺序固定：初始quiet后取`t0`，配置A、V，再触发A、V；其后每轮V poll→若exact DONE则配置/触发下一V→A同样处理→正常tick。某lane达到64后停止补发，继续服务另一lane至64；最后排空。没有“V完成即截A”、自主batch、额外启动delay、内部状态强置或按观测挑地址。单线程MMIO的自然空洞必须测出来；不承诺两lane持续100%重叠。

同一程序在不同策略下完成时刻、补发时刻和最终请求序列时间可以不同，这是闭环结果，**不强制相同外部到达日历**。每lane的地址/parent顺序不随策略改变。

来源是已有[driver提交/环推进](../linux/drivers/misc/idma.c#L440)、[poll/refill](../linux/drivers/misc/idma.c#L690)和[drain](../linux/drivers/misc/idma.c#L751)，以及现有RTL夹具的[普通MMIO配置/NEXT](../out/first-pte-paired-local-20261016/runner/paired_dma.cpp#L1503)。**固定源、只转目的和双方64次自然收尾是新的受控投影，不是已运行的Linux应用。**模拟真实DMA/翻译模块与行为DDR，不包含真实CPU、Linux、消费者计算、free或FPGA时序。

### 固定普通配置，不能混称stock

- 使用First-PTE已验证的actual-DMA system源列表及独立测试副本；`PolicyEnable=1`、`PairPolicy=0`，只选runtime **P=(N0,P1)** 和 **PN=(N1,P1)**。同一个elaboration，模式只在reset/quiet边界设置，全程不切换，不声称免费在线换模式。
- 两者共同使用已实现的**DRAM AR承诺前DS-first/F4**；它不是stock，也不是M。四次DS grant后的公平escape保留，held VALID不撤回。
- 原行为DDR FIFO、startup31、DS50/50、R/W独立推进不改。C++跨ID R-return priority关闭、queued startup overlap关闭；不假设MIG可提升已接受读。
- M、O-tail/local/prepared/job128全部关闭；原读/写256beat分段、两路512beat数据buffer与normal4096 guard保持。CPU背景、额外延迟、clear/warm、专用owner/门控模式关闭。
- P的16项IOTLB分配为V两项、A十四项；因此选R跨越**A源1＋目的R**的足迹边界，而不是事后寻找获胜点。[实际mask与lookup端口](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv#L660)。

### 六个基础条件，最多两个诊断条件

| 条件 | A目的环R | A有效页足迹 | 普通模式 | 用途 |
| --- | ---: | ---: | --- | --- |
| B1/B2 | 1 | 2 | P / PN | 极短再用；首填之后的低walk控制 |
| B3/B4 | 8 | 9 | P / PN | 足迹小于A十四项；与长再用区分容量/首次填充 |
| B5/B6 | 16 | 17 | P / PN | 足迹超过A十四项；至少四次完整环回 |

这不是预测miss数，也不是统计样本量。所有条件都从reset后的正常冷历史开始，64次全部纳入主结果；可标注首次触及/环回，但不删除前缀或挑“稳态最好一段”。确定性环境首批每配置一次，不作显著性或自然应用分布推断。

**预留D1/D2：仅在R16的4KiB轨迹确认有重复目的walk及完整任务中的相关必要等待时，使用P/PN各一次“A专用池改为普通2MiB叶”的诊断。**V仍是原4KiB映射；A/V有效COPY、物理源/目的、供给程序、buffer和全部字节不变。若没有该事实，或下述大页合法条件不成立，不做这两个配置，也不改填别的点。总配置上限8，不追加cold/hot、solo、相位或分段矩阵。

大页对照是合法普通方案的机会，不能为保留问题而排除。它目前**仅是已实现Sv39能力的测试接线提案，尚未运行此程序**；不是声称本runner已有大页命令。P模式的4K/2M对照避免把N资格变化误认纯walk因果；PN模式A大页会改变其N资格，必须连同路径变化报告。因此诊断首先检验映射组织的整体作用，不把完整差额全部叫纯PTE等待节省。

### 地址和授权：为大页普通对照预先保持可达性

| 对象 | IOVA | PA |
| --- | --- | --- |
| V源/目的 | `0x10000000 / 0x10001000` | `0x80000000 / 0x80001000` |
| A固定源 | `0x20000000` | `0x90000000` |
| A目的p | **`0x20100000 + p*4096`** | `0x90100000 + p*4096` |

root采用上表，**未采用可执行性附件中`0x20001000`起始的A目的IOVA建议**：采用1MiB偏移，使IOVA/PA一致落入同一对齐2MiB线性专用池，避免人为排除普通大页。L0目的索引为256..271、源为0；A L1索引256。表根仍为DDT `A0010000`、V root/L1/L0 `A0020000/A0040000/A0050000`、A `A0030000/A0060000/A0070000`。页表、CPU scratch及V/A载荷不重叠，均在真实DRAM decode `[80000000,c0000000)`。

所有条件均预留A的`[90000000,90200000)` **同一个2MiB私有测试arena**，包含源、目的及未使用空闲区；不在该arena放页表、其他设备私有对象或元数据。本微基准明确允许A持续访问整个专用arena，不能把大页授权范围偷偷扩大到别人的内存。基础配置只装4KiB叶；诊断改A L1一个2MiB叶，PTE A/D预置，reset前完成，运行中无promote/remap。PTW已有[superpage合法性判断](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv#L487)。

需分别记录：预留arena2MiB、实际有效payload页`R+3`、实际写入字节、页表及其他元数据。C++稀疏memory字典不是OS物理分配测量；不由“预留一样”认领RAM节省。以上是合成测试授权合同，不声称真实部署已采用它。

## 3. 能区分解释的最小观察，不再只堆计数

### 逐parent守恒与完成合同

每parent记录`lane,j,src,dst,exact NEXT_ID,cfg/NEXT/poll timestamps,HW issue/backend accept/HW DONE`；必须保持单active及原身份。每lane读/写各262144B、32768个64bit R beat、128AW/128B、64issue/64DONE、错误0、最后B不晚于相应parent DONE。未分段payload AR也应守恒，不能只固定总数而不关联parent。

源反复复用会让最终目的内存掩盖中间漏写/坏写。故新增测试检查：每个W握手按pending AW地址、beat和strobe核对固定源数据；逐parent R/W/B守恒，最终再验所有触及目的及源不变。**这些检查尚未实现**，不得以最终memcmp代替。FAIL/超时/不匹配全部保留，不记成性能阴性。

主结果为`t0→最终HW DONE`的`T_V,T_A,T_data=max(T_V,T_A)`及`t0→final quiet`的`T_return`，包括所有配置、NEXT、poll和补发空洞；单位为行为DDR下**RTL仿真时钟步数**。初始化/reset/策略设置成本单列，不宣称Linux完整生命周期或应用CPU费用。输入准备和内存核验不藏成应用收益。

### 必须实际落盘的事件与归属

1. **供给链**：logical-ready（j0=t0，其余为本lane前一HW DONE）、cfg/NEXT开始/结束、真实issue、backend accept。logical-ready是软件工作流定义，不是硬件READY。原paired只一pair，需新补发器，不能直接换命令行参数。
2. **翻译链**：每个有效normal请求的bank/DID/类型/IOVA、首次lookup hit/miss；N probe另记，不以probe次数当已接受miss；真实normal/N接受和响应。probe与main lookup按[实际mux](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv#L271)归属，不能一律绑定main_bank。重复驻留周期不当成独立请求。
3. **PTW链**：真实walk开始/结束、owner及层级、各PTE在local→DS spill→shared selector→DDR的offer/accept/return；同ID复用以endpoint/occurrence区分。当前单PTW单笔PTE可用顺序队列配对，不能只用ID永久指代首PTE。
4. **payload承诺/服务链**：本地AR、共享selector、DDR接受、每笔剩余R、source512beat收齐，AW/W/B和parent DONE。首次offer和接受分别记录；已held/已承诺不列成尚可选择的请求。
5. **真实等待而非握手空档**：RVALID/RREADY、相关AW等待flags、TL主owner状态及选路，只记录状态变化/握手并保留起止；行为DDR当前服务的request ID/occurrence、startup、beat和held状态作为**模型真值**另列。它不是实际部署新增可得信息，也不交给控制器。

现有CSV没有完整这些字段。尤其legacy `trace_event`在full-era关闭，所以源码中的`aw_wait_change`**并未写进此前paired CSV**。[具体缺口与源位置](../out/problem-discovery-plan-20261017/existing-coverage.md#L36)。需要只读测试wrapper/tap及C++输出增强，不驱动DUT，不改服务规则或生产RTL。

### 分析与局部因果边界

- 全部parent保留；为呈现依赖，固定展示每配置最后完成parent，以及V延迟最长parent（同值取最小j，重复则合并）。同时公开全表，不能只挑有利窗口。
- parent可按`ready→issue→backend→首R→全部source R→最后B→DONE`分段；跨lane或重叠布尔条件不相加成`T_return`损失。AW flags可同时成立，不能按低bit优先惯例宣称唯一根因。
- 一个等待要称“存在控制机会”，须有可见的必要前驱、阻塞义务owner、实际承诺端点，以及一个可合法改变该前驱/义务的动作；没在本轮实施该动作时只记观察/推断。不能从PTE与payload重叠就认领可回收收益。
- P/PN是合法路径干预，会改变后续到达与竞争，不单独隔离PTW/供给/owner三者。D1/D2若触发，可检验大页组织是否足够；不等于精确隔离每一周期根因。没有干净干预的剩余保留unknown，不添加oracle返回重排或新候选控制器。

## 4. 执行前最小改动、资源上限和停止规则

**本轮没有生成可执行新命令或补丁。**旧paired main只接受cold/hot/policy/trace，build又限定旧输出根；直接对它加ring参数会失败。批准后的最小工作仅在新目录`out/problem-discovery-run-20261017/`建立测试副本：

1. 复用旧源列表/normal DMA/行为DDR；新增固定源目的环初始化、P/PN runtime参数、六条件调度及有条件D1/D2。不调用会覆盖历史结果的包装脚本。
2. 替换一次pair断言，增加逐parent数据/守恒检查和少量只读身份/timing tap；M/O使能静态关闭，删去观察器中first-PTE/O专属单次断言，不能把它们误移作新程序守卫。
3. 只做新测试的必要编译/结构检查，再逐配置执行；同一基座不为P/PN重复elaborate。硬件/周期结果由真正执行记录提供，当前只有源码可行性。

拟定投入上限：一次基座准备/编译，编译硬超时15min；单配置仿真上限1,000,000时钟步数及120s墙钟时间，先到即终止；基础6配置，最多2诊断配置。每lane独立无进展watchdog，不能让A进度掩盖V停滞。所有实际调用、退出、失败和原始事件写新目录；修复仅针对明确实现错误，不自动扫参或重跑整个旧矩阵。触及总1小时执行预算仍不能取得合格轨迹时，交付阻塞事实，不迁移到FPGA或恢复MIG。

该上限是投入/监控约定，不是统计保证或真实时延预算。设置这些上限没有授权本轮执行。若基座错误，需要有界修复，失败日志仍保留；改变程序/服务/参数后不能把前后结果混成同一冻结条件。

## 5. 三种研究交付与退出

| 取得的事实 | 本轮发现阶段的裁决 | 后续含义 |
| --- | --- | --- |
| 合格持续供给后，普通控制没有明显可改变的主要残余；或N/P差只在首填且被完整后续覆盖 | 缩小/结束该工作负载范围 | 不设计控制器，不因全负而补点；保留边界事实 |
| 有等待，但来自明确缺陷或普通大页/已有机制已足够 | 修复/普通优化类 | 不包装论文创新，不自动开发完整优化系统 |
| 有完整残余，能指出普通方法缺少的信息、动作或资源组织，并有合法干预支持其作用 | 进入**一项**机制推导 | 先发展这项差异，不再生成三个宽泛候选；仍需新意/完整成本/独立条件才能谈论文主贡献 |

若只有相关性而因果未决，单列为未决，不能假装已经落入第三类。一次问题发现有信息增量可以成功，而论文机制仍未成立；两者分别记录。

旧First-PTE三policy六case、旧70、E2/E3/M0/p5/D-CS1均不重跑；前三候选停止主张不撤销。B1/I/D/A5/A2及MIG暂停、WNS−0.391ns/598setup/0newloads/debtunknown保持；不重启原10月15—28日窗口。未来新结果即使发现早意图窗口，也不自动运行M为旧版翻案。

## 已完成的准备与直接证据

- [夹具可行性](../out/problem-discovery-plan-20261017/fixture-feasibility.md)：真实normal配置/NEXT/DONE、guard、地址及只读tap；建议地址已由本合同上表覆盖，不把建议当执行事实。
- [已有覆盖](../out/problem-discovery-plan-20261017/existing-coverage.md)：所核E2/E3/M0/旧70/First-PTE均非本合同同输入；没有声称全仓历史穷尽。
- root直接核[一次pair入口](../out/first-pte-paired-local-20261016/runner/paired_dma.cpp#L2391)、[旧映射初始化](../out/first-pte-paired-local-20261016/runner/paired_dma.cpp#L533)、[TL/N生命周期](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv#L284)、lookup mux/mask、superpage检查及driver poll/refill。以当前字节为准，不改旧合同/hash。
- 本轮允许的实际执行仅为文件读取、地址/预算算术和文档检查。没有实验数据，不用“源码上可做”冒充“已经形成持续竞争”。批准后的实现与运行仍是下一步。

### 准备检查（非实验验收）

相对输入`4c83ac3`，地址对齐/不重叠、A的2MiB线性映射与L0索引、64次工作量及最多8配置算术通过；本轮新增/修改的33个本地文档链接可定位。HANDOFF与validation的旧正文仍为完整字节后缀，CURRENT分支/独立工程段未变，已跟踪生产源码与历史实验输出未修改；本轮文档差异格式检查通过。以上不证明新供给器、只读tap、数据检查或仿真行为正确，它们均尚未实现/运行。
