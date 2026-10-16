# 2026-10-10至12日审阅单元

## 实际覆盖与证据边界

94个分块、34个来源、370,378 UTF-8字节，全部按27批向审阅模型展示并实际读完，无文档分批输出截断。逐块 source/start_line/end_line/full_read/sha256 见 `ledger-oct_10_12.json`。其中 HANDOFF 是457–780与3300–3309行的32块；validation是451–660与2363–2395行的30块；这两个大文件只声明本单元所分配部分读完，整文件由root合并其他单元。`research-execution-plan-20261011.md`只有7–17行；其文件名不代表仍是10月11日状态，当前块已是较后BR1规则，不能据名字排序。

34来源中，AGENTS、README、研究方法和本单元多数专篇是整个文件（来源快照全文）；HANDOFF、validation和execution-plan是部分范围。两份route-audit正文内容重复且全景报告明确指出byte-identical，不能计两次独立研究。validation和HANDOFF的多数同阶段条目同文重复，仍逐块读以确保后继勘误/否定未漏。status文件是历史快照：文件名stage95有一份正文stage96，必须用内容阶段而非名字或mtime理解。

额外通过已有dell SSH只读核了10份out JSON的存在及部分/全段内容，见ledger。原始M0 results只投影十配置标量及首记录事件数组；R107 receipt只投影实际标量、HPM、composition计数、observer前10字段和首尾batch，不宣称逐字完整复审全部144批。首次合并JSON工具输出有截断，关键数值用小投影补核；不算原始CSV、UART和binary池全读。未运行任何实验、校验器、构建或板端动作；未改远端文件。

以下绝对路径共同前缀：`/home/xvhaocheng/work/iommu-platform/`。报告叙述引用的是实际留存实验，文档与回执核读不等于本轮重验实验。

## 10月10日：路线反省、方法流程与真正完整任务测量

阶段93–96的主题不只是换一个算法名字。先前机制探索从固定目的COPY不断延伸到分段、共享源、晚选目的、在途加入；真实需求与成本没有同步固定。用户允许推翻主线、不绑定Chiplet，以有价值的独立贡献为核心。原Remote PTW代价→多DMA共享翻译干扰的扩展仍合理；但候选功能正确、普通部件组合、PN退化都不能直接成为贡献。

`研究方法.md`保存用户七步方法：核核心原文、整理约五篇文献、从具体失败切入、独立质疑、修订后核前作、小实验先行、计工程成本。`research-convergence-plan-20261010.md`选择MASK、Walk Scheduling、MMU-aware DMA、KPart、Bermuda五篇；地图是邻近机制和强对照入口，不是已证明开放问题清单。不要求候选击败携带候选完整规则的“普通可编程实现”，等价实现与独立强对照要分别判断。

实际执行前问题：旧模型first V issue至last V done并STOP A，隐藏初次准入与A尾段；策略源码已有p1–p5 V完成反馈、p6 gap反馈，不能写成纯静态；ADMIT只能控制下一未承诺copy，不能修当前copy。PolicyEnable分区参数不等于旧反馈门控已接入。stage96把G0改为先判普通具体失效，G1再选动作，M0先解决共同就绪与双方自然完成。

stage97 M0实际实现：共同t0在首次NEXT_ID前，A/V各64×4KiB，预热256后排空，CPU关闭、静态S1/S2off、gap0。十配置、2560事件时间戳核对；首发128周期、双方先结束顺序、watchdog、14损坏记录/9参数拒绝。不是模型设置计划，已留实际运行与验收。

A8 FIFO P/PN：V 86594/90283，A86082/89771；普通DS优先后两配置同V76738/A76164/Treturn76745。walk返回93笔，AR至返回累计5974/9663差3689，优先后均2976。A1四模式相同73539/72965，测量段无walk。关闭“这项差额需要新增逐copy门控”的理由。DS优先只在行为DDR服务选择实现，无综合/面积/Fmax/板测，不能解释旧PN板端12.67–12.79%全部差额，也不能写15%硬件收益。N/S工作量不同未由这项干预解释。

依据：HANDOFF705–750；docs/workflow-g0-m0-results-20261010.md§判别结果/测量验收/研究边界；合同 `out/workflow-g0-m0-20261010/workflow-failure-card.md`、`baseline-register.json`；实际 `results.json`（本轮十配置标量直接抽核）、`results-table.csv`与各CSV（未逐字读）；验收 `m0-acceptance.json`（本轮全文）与`root-result-acceptance.json`（通过文档引用）。

## 10月11日发起、12日收束：行预取不是Sobel实测，普通P已满足期限

stage94先将DASH行预取写成需求合同：640×480、640B行、四槽、30fps；前3行初始化、477稳态期限。槽释放限制的是实际载荷覆盖，不禁止合法提前翻译/预读。后台8MiB/2048×4KiB、DID11是研究追加，非DASH原生应用。最早contract.py有效一周期数据只是检查器夹具，没有DMA时序含义。

stage98实际私有C++链接已有SYSTEM RTL，不修改生产RTL/Linux。固定P、CPUoff、无DS优先、无新门控；4KiB/2MiB各一次确定性模型，双方480+2048自然完成，AR/AW/W/B/quiet和逐行数据核。4KiB本来就0/477迟到、最慢3192周期、最小余量280；2MiB2175/1297。共同窗后台938/2048笔；A最终3309615/1401779。大页最低授权容量432KiB→6MiB，额外5,849,088B，并非免费。2MiB后台早结束后的低延迟不能冒充同竞争强度曲线。

因此归档“新增保护才能满足本期限”假设，不能说全局最优、所有期限可保证或实际Sobel/FPS已验。B流处理当时缺kernel/槽释放，后续才补。

依据：HANDOFF689–704；docs/workflow-row-g0-results-20261011.md全文；合同 `out/research-execution-20261011/row-baseline-register.json`（本轮部分读取）与`docs/workflow-row-prefilter-20261011.md`；实际 `out/research-execution-20261011/row-p-4k.json`/`.csv`、`row-p-2m.json`/`.csv`（未完整原始核）；验收 `row-p-4k-independent-verification.json`（本轮全文，0迟到/938后台/480+2048/真实quiet已核）及2m同类文件（文档引用）。

## 10月12日：B双缓冲来源、同钟R2与失败修复

stage99取得HERO固定mm-large.c，不是原2018 SP代码。256² uint32，A/B/C各双16KiB槽，96KiB；A16+B256+C16=288搬运，256tile、16,777,216乘加。实际host malloc/同步memcpy和保留8线程功能执行一次，全65536输出正确、1390事件/槽代次/三负例拒绝。新增512屏障且host_ns仅观察顺序，不是异步DMA或CVA6/FPGA计时。早文档写C=A×B，后stage100明确实际索引为A×Bᵀ；默认B=2I不能用数值区分转置，后来非对称稠密微迹才核通用算术。

stage100新增同clock私有R2：单未修改CVA6乘法器、实际96KiB单总读写端口、响应1周期、真实iDMA/IOMMU与行为DDR。不是原HERO8PE、CVA6指令执行或现成FPGA速度。首轮普通TC可超配置四AW至八、3位W计数归零死锁；只在私有副本按相同预算修AW准入，四对照共用。工程失败不能作科学阴性或候选收益。

四模型：solo4k V100813737；shared4k V100819743/BG3132054；startup4k V100813799/BG3130863；shared2m V100806133/BG1288361。普通首块A0/B0完成后再发后台恢复5944/6006=98.97%，余差62，后台最终还早1191。共同ready没有后移，后台首服务实际延后13549周期，但原合同无首笔期限，不能事后添加期限救候选。2m无匹配solo，不用它归因全部共享损失。

stage101进一步定位：255后续tile都是一周期交接，无后续输入/输出槽气泡；6006=5988首命令+18累计tile差，startup62=45+17；尾C同5023。后台在q7计算中已完，不覆盖后续条带输出。原SP图5确有16KiB/four pages，更正stage98“无块字节数”遗漏，但kernel和固定时序仍未取得。HERO8RV32/16bank128KiB不能按比例缩放R2。原E3某旧时序报告−6.470ns未绑定已验boardimage，不能因此撤销E3局部因果，也不能给新B50MHz背书。

stage102再核HERO README/testset：device0/1是同一加速器，A/B/C同结果；独立后台来源仍未决。没有deadline不构成否定，研究追加有限背景合法，但不冒称原生应用。固定R2新增门控理由归档，无G1；原8PE和整个流处理领域没有被否定。

依据：HANDOFF635–688；docs/workflow-stream-contract/g0-results/platform-review/background-demand-20261012.md全文；合同 `out/workflow-stream-g0-20261012/contract.json`（本轮全文）；实际 `core-comparison.json`（本轮全文、四案数值核）及case CSV/二进制输出（未全文核）；验收 `validator/full-acceptance-summary.json`（本轮全文）、`validator/integrity-capacity-fixed-acceptance.json`、`protocol-raw-acceptance.json`（后两文档定位）。SYSTEM DDR B反压、全应用逐RAM独立回放、板资源/时序未验。

## 10月12日：真实需求筛选与平台纠偏

stage103来源预筛网络存储（SPDKv24.09/iperf3.17.1）和周期采集（官方SDI/YAVTA/DTS）。真实程序/生命周期有源，缺同运行实例共享翻译单元、到达/成功完成、映射/成本。随机LBA不代表新IOVA；SPDK完成计数含错误，计时在排空前；capture采10帧只存3，不是只有3次DMA。默认DTS SMMUdisabled不能推所有capture配置。两卡未达G0是需求未决，不是性能失败。

stage104曾只读核Dell T400/NVMe/NIC同dmar1、DMA-FQ，并拟OPT6.7B disk100/CPU100功能种子；用户马上指出服务器不是实验平台。该主线选择被撤销，零安装/下载/AI运行。既有Genesys2/CVA6+四iDMA+RISC-V IOMMU/Linux保持，GPU/AI只是来源优先。报告开头纠偏比下方保留历史“进入准备”更当前，不能漏读。

stage105 FlexLLMGen OPT125m一层MLP六FP16张量9,447,936B、2309分段；两跳4618任务/驻留一跳2309任务RTL功能正确，但原第一跳CPUcopy，驻留DDR可普通一跳完成，强制中转没有第二独立用途。关闭当前可选中转收益前提，不是否定AI。

stage106换成两rank层0/6不同必需MLP包，均ready0（研究提取，非原GPU天然并发），八案普通G0。driver DONE reset1误当0以及C8 overflow满当空私有两行修复后八案完整通过。4k并发5,365,690→2m2,377,027周期，−55.6995%；双包18,895,872B的单R口乐观下界2,361,984，仅剩0.63285%总完成潜在改善。普通singleDMA首包1,188,896、全2,377,298只比并发晚271，代价是另一包首时未进展。关闭all-ready冷准备新门控前提；实际大页分配/cache/供给/板时钟成本当时未知。

stage107复核不能用10MiB/order12 helper限制否定所有大页：普通20×2MiB extent可保十二完整张量，未实际分配。KV整池预分配，logical free/refcount不撤映射；已有layer event、同stream、共享去重承担安全。KV块管理不等于IOMMU页生命周期。同一期不双向swap，多序列才默认swap；保守容量启发式不证明强迫。由此形成R107有限KV恢复卡。

依据：docs/real-dma-scenario-screen、gpu-ai-deployment-gate、gpu-ai-fpga-fit、ai-mandatory-work-g0-results、ai-platform-constraints-20261012.md全文。AI实际与验收 `out/ai-mandatory-work-g0-20261012/credit-fixed-core-results.csv`（未全读）、`validator/credit-fixed-suite-acceptance.json`（本轮八案投影逐项accepted/bytes/timing核，部分首案正文）、`credit-fixed-g0-register.json`（文档定位）。

## 10月12日：R107从有限源合同到真实板端普通覆盖

stage108固定vLLMv0.1.0、PagedAttentionv1、OPT125m配置：12层/12head/TP2每6、B16/D64/fp16，双序列共享完整前缀+不同尾槽，3唯一live/ref2/1/1/COW0，8槽容纳live3及保守预留5。144完整恢复/1,769,472B，之前swapout另同量；CPU代理标记663,552B不是native attention必读量，Sequence长度不证明KV有效。CPU AST源函数/形状回放一次核六池，未执行GPU。四池逻辑9MiB，普通4×4MiB或8×2MiB backing16MiB/8目标叶。合法连续run可96项，可信owner singleDID合并允许。IOTLB16>8不证明零miss，实际leaf/cache/fence/身份未知。

stage109登录当前Linux，优化f9 bit内部时序+0.022/+0.051但IO/CDC等保留；JTAG读IDCODE1 Unknown_Device不擅推损坏。stage110查明CPU DMI TAP误认FPGA，B接口xc7k325t/0x43651093，用户批准易失载入。第一次回读匹配但SD CRC失败；第二次同bit不并行回读成功boot，物理因果未证明。旧内核81122源冻结/2078÷2079链接对象同字节，仅version.o横幅32B不同，完整Image/ELF未重现，停止追旧横幅转新冻结内核。

stage111普通adapter实现、构建、备份/dryrun后曾等待新SD批准甚至goal blocked；这些是历史状态，不是最终结论。批准后原RUN配置idle误当backend quiet导致NEXT前EBUSY，未发DMA，不作科学阴性。r1只在独立idma_finite.h改精确backend/batch idle检查，保留unknown NEXT隔离。最终唯一实际hardware pilot通过：priorout/restore各432硬件4k task、48+96whole batches、1,769,472B，四池逐字节、24worker-layer/12joint代理检查、release0/quarantine0。实际DMA0/DID10/S0、优化f9，不包含独立57eb562新修复，未新构建FPGA。

原receipt抽核：ioctl run_calls1/rc0，144批、8叶、16MiB backing，data_errors0；composition prior1728lookup/1720hit/8miss，restore1728/1728/0，HPMwalk增量8/0，observer issued=completed432。普通prepare/priorout/residency/batch/singleDMA已经覆盖本合同翻译miss，R107有限卡归档，无G0/G1。零walk不代表所有hit固定延迟消失，不是原生多GPU吞吐或attention/token结果。

完整ioctl名义21.960371360s，恢复阶段名义45.192160ms，Dell条件TIME界40.556956–49.914060ms；raw counter、kernel nominal、Dell CLOCK_MONOTONIC和条件界分别报告。计时校准同bootCPU兼容45.267–55.711MHz、TIME22.634–27.856，依赖固定率假设，不是精确SI频率；TIME由SBI读CLINT、2:1不是独立振荡器证据。研究全池核/原始交付/guard成本全部披露，但不能当原生GPU生产必需开销。

合同：`/home/xvhaocheng/work/iommu-platform/out/r107-bounded-decision-20261012/instance/finite-contract.md`（文档完整说明，本轮未全文原件）。实际：`/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/live/finite-pilot-result/receipt.json`（本轮上述关键raw抽核，144全部batch/池/UART未完整独立核）。验收：同目录外`repair-1/validation/actual-finite-pilot-validation.json`（本轮全文），限制明确无独立per-beat或per-task NEXT接受追踪，层ready为CPU代理。全成本`repair-1/costs-and-boundaries.json`、失败`failure-root-cause.json`、inactive后继`next-goal-contract.json`通过docs/r107-ordinary-pilot-20261012.md末§入口定位，未全文读取。报告、execution-gate、platform-identity-recovery、bounded-decision四篇全文已读。

## 同日独立工程与上游贡献，不混入方法收益

AXI 4KiB边界checker：FIXED页尾off-by-one、INCR/WRAP size-1范围、WRAP窄指数回绕；IOMMU57eb562修复，旧816064检查112514失败、初修152失败、最终全过，非全AXI协议/DMA/FPGA验收。独立DS/PTW原用户修复移植上游mc/dti-ats：DS旧AW1/W0/B0失败→4/4，PTW合法4k cause13/15→7/7；已推fork，connector403不能建PR，后用户网页PR51/PTW、52/DS open，未合并。

对PR42/43/45–49只读裁决：47/49排序缺口作可靠性待办，但当前quiesced/无PRPW实验不据此否定；46/48真正MRIF前修，当前Linux基本MSI不触发；45当前FIFO需定向回归，不套旧竞态整补丁；42不是必破Linux ABI，43无当前需求。工程范围与实际liveBIT不能混。

Linux IOVA叶偏移与HPM宏括号分别8e3a740/778059独立单文件，目标malejo97/linux:wip/riscv-iommu/d371fb0f，已推fork；旧16项12失败→新全过、宏32项14失败→全过，PR创建403未产生编号。排除CVA6 cache workaround与实验API，不称新Linux主线bug或上游已合并。

依据：HANDOFF519–550、3300–3309；validation507–522与2390–2395。原证据 `out/iommu-axi-bc-fix-20261012/acceptance.json`、`out/upstream-iommu-fixes-20261012/submission.json`、`platform-pr-acceptance.json`、`out/upstream-linux-fixes-20261012/submission.md`本轮只通过已全文阶段记录定位，未完整原diff/运行重核。

## 给会议草稿的具体补充及改写

未提供私人wiki/草稿原文，本单元不声称逐句比对。若旧稿仅六月至九月Slack，必须新增上述stage97–111全链，尤其：

1. 不能把“阶段测量仍未适配/只V窗口”当当前状态：M0双方各64自然完成已实际验。
2. 不写“PN组合有效或12.7%待新门控回收”：实板反例未完整归因，模型该分支普通DS已覆盖。
3. 不写“行预取有deadline所以需新规则”：普通4k P已0/477，非Sobel/FPS或硬实时验。
4. 不把B“缺kernel一直未跑”当当前：HERO host功能→R2同钟模型→普通首块98.97%覆盖→真实后台未决完整链已做；原SP16KiB遗漏已更正，HERO不是其复现。
5. 不把Dell GPU/NVMe作为新的主实验平台或OPT6.7B已经部署：选择当日被用户撤销且0运行。
6. 不把cold weight大页模型改善或KV块频繁free包装方法：源存在合法驻留/大页/合并；logical free不撤IOMMU映射。
7. 不保留“R107待板测”或“R107新方法验证成功”：已实板普通恢复，0walk，归档的是该有限卡，方法论文仍阶段一。
8. 工程bug、失败修复、来源身份、上游PR准备另列可靠性与可复用资产；不称性能创新、全协议证明、Linux主线已合并。
9. 当前未解是有真实新增事实的任务/owner/lifetime/bytes合同、普通方法不可覆盖的残余、有用完成与完整成本；不能用扩压力/换deadline/恢复旧卡代替新事实。

本单元保留结论：普通服务与准备在多个有限合同足够，具体性能主张关闭；E2/E3、测量/平台资产仍有价值。没有推出所有应用/共享翻译方法无空间，更没有选择大学报备用新题。
