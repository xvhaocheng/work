# 2026年9月研究历史审阅

本单元实际阅读全文：182/182分配块、52个来源、341,296字符（块含来源/行号头，约610KB UTF-8）。这不是52文件全部全文：HANDOFF、validation、status-history、研究计划为分配章节，其余来源完整性须与总manifest合并。逐块源路径、行范围、阅读标记见 ledger-september.json。所有内容来自Dell快照；本轮只读文档和定向SSH证据，没有运行构建、实验或验证程序。

阅读分46批，首次合并输出在0065中段截断，随后单独完整补读0065。不能把程序读取等同模型阅读。本单元的全部182块都已实际输出阅读。注意按日期分配存在少数跨月文档：mac-method88、mac-research2和experiment-instrumentation的10月2日追加段、最新研究计划分配给本单元，已照样阅读，不能据单元名将其当成9月事实。

## 1. 9月的实质演进

### 9月8日：从旧工程与继承测量，转为可追溯平台和独立V窗口

根《研究总览_2026-09-08.md》全文记录原学位论文是Accelerator-side IOMMU/Remote Page Walk评估环境；新课题扩展到多DMA共享翻译资源。原论文IOTLB8、文稿Kernel6.15与新工程IOTLB16、源码6.5.0不能混用。远端延迟FPGA模型不是已搭真实多Chiplet/PCIe P2P。

《平台重建方案.md》全文显示迁移动机是旧顶层URL、组件origin和本地提交对象不一致、未跟踪benchmark及未提交修改导致无法保证clone复现；保存22仓库历史/diff/未跟踪资产，建立iommu-platform并固定21组件、gitlink/lock、脱离旧绝对路径。保留AW优先修复7fbb2b9、接口6cc8ed3、四DMA地址译码、HPM edge、OpenSBI timer25MHz。迁移完成记录在validation.md §历史：平台迁移已完成（1849–1863），含2332源码blob审计与离线恢复，不等于板测或完整时序签核。

benchmark-audit.md全文确认旧Pair independent已存在，四路Multi仍逐chunk barrier；两路结果公共cycles/HPM重复写入，不能当Victim独立完成时间。旧单路0x01已失配，Pair suite忽略data errors仍exit0，固定数据0xa5漏地址/重复漏搬运，random无seed。历史总吞吐约91.9%–96.3%仅研究动机，不能称V slowdown或HOL证据。

新v1退役旧0x01–0x05，使用查询0x06/运行0x07，一核管理两路，V固定工作完成即结束，A持续补交后安全排空。per-DID HPM独占会话保存恢复，异常保留映射/quarantine。CLI65、调度12、HPM25、rootfs4与软件构建通过；主机桩不验证真实DMA/MMIO。计时仍含CPU补交和完成发现，不是纯硬件时间。

设计起始已明确E0校准、E1性能、E2排空后状态、E3请求阻塞、E4供给/数据对照；并非事后看到slowdown才发明判据。FPGA为系统实验主线，仿真按缺口辅助。见experiment-design-v1.md、experiment-implementation-plan.md、experiment-instrumentation.md、experiment-v1.md全文。

### 9月9–15日：控制软件增量，修缺陷，方法目标与创新门槛明确

research-review-2026-09-09.md与experiment-v1-review.md全文：用户优先扎实投稿，并随后明确方法论文目标，表征用于方法落点。不能把最初审查的“首篇表征”建议当最终决定。W同时改变翻译和物理数据工作集及闭环到达率；同吞吐比较可评代价，不能自动识别纯翻译效应。统计单位是完整试次，4096描述符不是4096独立样本。

确定缺陷：小chunk DID只在高字节混入，1/2/4B错源漏检，96组合旧36失败→修后全过；仅修每字节设备身份与多页部分覆盖smoke，不自动加全套v1.1。HPM连续valid UT_REQ→DDTW漏后者，实际生产HPM定向仿真复现；六槽未选DDTW，不可声称全HPM失效。S1/S2与miss相关源，不是独立页表访存数/占用。

9月15日软件交付更新，旧包保留。主张“换到RISC-V、多DMA、一般限速、hit-under-miss、按时间计费就是创新”均受已有RTCSA/MASK/DWS/STFM/ATLAS/TPDS工作约束；9月末又核HPCA2017和非阻塞缓存MSHR隔离。这是局部文献核查，不是完成全部新颖性搜索。

### 9月15–16日：两次独立功能故障先修，不能把实验异常当研究效果

SC_WORD_FIX.md全文：Mac Linux proc/dentry/Oops无法登录，实板高半SC.W失败误报成功1000/1000；wt_axi_adapter等待B时清掉amo_off，修为保持。真实生产RTL54场景旧6失败/新0；独立scfix BIT随后登录/proc正常。此修复与后续DMA数据错误不同。

DMA_SMOKE_FIX.md/DMA_CACHE_SYNC.md全文：BusyBox timeout缺失exit127先未发DMA；兼容脚本第二例EILSEQ，warmup和调用次序混杂，不直接归因预热。诊断目标offset912初始化eb经FENCE.I变期望14、源始终14支持CPU旧缓存视图。当前Linux默认coherent，CVA6 WT无外部DMA失效；平台FENCE.I同时清Dcache，非通用RISC-V保证。前提交与安全排空后各加同步，测量窗外；保留真实错误语义。另修软件iova_to_phys错误把IOVA页号拼进PA，旧异常大*_pte_pa不是硬件实际翻译。修后两启动29功能、单DID32/双DID12归属，18探索单列。

### 9月16日E1→E4：第一次大慢化首先暴露软件供给问题

E1 144项=16有限HPM检查+48敏感性+80主样本，双角色Solo/A1/A8/A32各10。V软件Solo584–587ms，A1约1.728倍、A8/A32约1.79倍；V miss全0，A1也0，V backend active基本270万周期。不能把79%全当翻译。E1复核未读Mac完整UART，完整E0未签核。

随后E4被动H/P/R和AXI观测，保留轮询与供给。105项=9短+36开关+60主，86观测组。H约56ms基本不变，新增窗口时间99.08%–99.92%落在P（任务完成→DONE读取）；60主样本40共跑both均0，无硬件请求重叠。P含CPU/控制MMIO，不能全称CPU，也不能说IOMMU绝无干扰。开关中位最大0.5051%低于原1%门槛，n3不能泛化。转向理由是真实压力不足、完成发现路径主导；没有改中断，改为硬件自主有限batch并整批CPU轮询。

### 9月16日batch→DS回切→分段：终于获得硬件慢化及位置证据

batch每路仍最多一个执行任务，4KiB完成后自主接下一项，4096项gap共4095周期；先A后V、安全STOP/排空。14短+60双角色主+30固定角色A16/32/64扩展，共104。重叠99.69%–99.81%；固定V0硬件Solo56.56ms/282.91MiB/s、A1 107.10ms/149.40、A8 154.99ms/103.24。A8对A1耗时+44.7%，A1双方无miss已慢，故全部损失不是翻译；90性能样本88 Vmiss0，另2 Vmiss2全部保留。A16/32/64扩展未同期重采Solo/A1，不可称六条件同轮随机实验。

DS0固定角色六条件各10，A8变118.19ms/135.38MiB/s，A8–A64吞吐较DS50+31%–36%，Solo/A1基本不变；但Vmiss全部2、A8miss6160→5141、跨启动映射未固定。回切DS50三条件各10，A8恢复154.88ms/103.31，主要miss模式恢复。支持DS敏感性，DS汇合多类数据结构请求且改变闭环节奏，不能把36.7ms称纯PTW成本。

v3 path33项（3短+30正式）将A8对A1 V内部读25→601.04、写11→268.68周期；入口恒1、出口地址反压0，写后段1130.37→1455.66。正式Vmiss全0。位置指向共享翻译服务，但stage1仍含排队/翻译/出队，sum不可相加算总损失比例。v4再拆内部，读排队18→596.78、写4→263.62，选中后1/4/2不变；写AW→WLAST1111.31→1439.79，WLAST→B约18不变。

### 9月17–18日E3失败迭代：不是一开始就拿到干净因果

v4/00084 3短+12正式15对30子轮，执行/570观测通过，但A8首轮0/10触发、次轮8/10、双触发0/10。两对均未触发仍次轮快约8ms，不能归D128。代码触发只看AR头V，A读服务期间占头，漏掉后方已接收V。探索跨配对5 Vhit残余中位差126仅线索。

00085 D0/D0 3短+6正式9对18子轮：旧8ms未稳定复现但诊断扰动，A预热提交/完成/排空256/255/1、末页一致，miss512→312/318仍异；A8双触发8/8，第一对身份/首查不同，不能恢复只差D因果。

v5/00086增加真实V接收未选中计数、候选门槛/前缀/两插入身份，13对26子轮9472检查通过。A8 16/16触发、旧头A9/16表明覆盖修复有效，但V首查16/16 miss、插入9/16截断；8 A8正式对7身份不同，唯一第5目标相同仍预热异且插入截断。第5 ΔA128/ΔV134是探索，不是合格因果；八对全部不能作为只差D。

v6/00087转独立64B有限任务、每轮S1失效/hot-cold prep/Vprime-probe、同一映射内两轮；launcher在A读实际选中后的下拍释放V，隔离CPU发射间隔与长批自然碰撞。11对22轮、418唯一观测/114重复、9验收通过。4 cold互补ΔA剩余=ΔV剩余127/129/129/130，V自身4周期；14cold A响应→V选中9。D0/D0 −2/0。仅main case6（pair9）两项干预前时间全相同，其余3对时序不同保留。结合串行控制器支持本实现/配置/到达方式 A-miss→后来V-hit等待，非AXI规范强制、非所有IOMMU、非解释全部47.76ms、非新方法。

### 9月20日P4M：把单侧加速转为共同窗口双方取舍

硬件在完整copy后插gap，不卡在途AXI；共同V窗口A完整copy done与首尾在途/carry，不能拿整轮A或HPM除V窗。4短功能后pilot21（三block七条件），gap1000/2000 V改善同时A下降，gap500三个block反而慢，保留非单调反例。第三P4M gap0开窗A ID1448 vs4/4，预窗口已多跑1444copy；不是全局ID或尾poll假象，原因未确认为中断或容量。

启动控制00089仅软件短非睡眠双trigger临界区，之后恢复IRQ/抢占，完成仍polling。12项=4短+8 BPPBBPPB，4对P/B硬件比0.999413682/0.999699263/0.999995512/0.999888800，描述性接近不等价证明；A→V163–165µs，开窗A ID均6。

formal60冻结20交错block gap0/1000/2000各20。在线24通过，第25 CLI正确/taint0却runner exit5，因为dmesg环形淘汰旧seq1总行数36→36，非DMA失败。离线25/25追认，但原在线24不改写；resume35准备，9月30仍35未跑。首24 V中位108.8275/150.6503/171.4546，A同窗108.8275/75.3252/57.0901，显示V/A节流取舍非最优gap/显著性/新颖性。10月后续完成应由10月单元更新，不能让这篇恢复报告冻结当前。

### 9月30日：汇报资产和N/P设计筛选，创新仍未成立

8页PPT/PDF/讲稿、问答、证据图准备，numeric-verification与materials-validation记录，不新增实验。用户上次只确认讲到A/V共跑慢，实际日期未知；本次准备E3/P4M但未确认实际讲完，不能擅自当会议已完成。

N为合理有限非阻塞，P为同底座同各类容量静态预留，仅资源分配，P不是新算法；必要保护位置/容量候选R未形成。结果槽分配时机不能人为设置长miss早占以造弱N；V空闲要独立固定窗、准入前等待计入。若合理N已解决则停止额外保护。动态借用暂不立项，槽耗尽未实测不能写新发现。见HANDOFF 2448–2471及status-history 759–765。

## 2. 关键实验证据链与本轮核查层级

以下全部根在 `/home/xvhaocheng/work/iommu-platform/`，裸out不可改为work/out。合同/报告已阅读全文；列出的验收JSON均用SSH确认存在并抽核键；E1/E4/E3v4/诊断/v6/P4M另抽核实值。运行raw全文未读、不执行分析程序。

| 阶段 | 合同/报告 | 实际运行原始目录 | 验收/分析 | 本轮raw核查 |
|---|---|---|---|---|
| E1 | docs/experiment-design-v1.md、docs/e1-results-review.md | out/mac-e1-stage-20260916-032650-run-rbo67pvd/{records,assessment,summary}.json | 同目录server-review.json | 验收144/80、complete_e0=false；未读全部records/UART |
| E4 | E4_OBSERVER.md、docs/e4-supply-timing-plan.md、docs/e4-results-review.md | out/mac-e4-stage-20260916-094001-run-kzi_s0zc/ | 同目录server-review.json | 验收105/86、40共跑零重叠；未读全部raw |
| batch | E4_BATCH.md、docs/batch-results-review.md | out/mac-batch-main-20260916-112954-run-9sa69rhc/，短out/mac-batch-short-20260916-111626-run-n8mg7wsz/，扩out/mac-batch-ext-20260916-114036-run-8pjv_djc/ | main/server-review.json | 验收存在/结构；raw未抽核 |
| DS0/回切 | docs/ds0-batch-results-review.md、docs/ds50-retest-results-review.md | out/mac-batch-ds0-20260916-132332-run-ebgxhirr/；out/mac-batch-ds50-20260916-135544-run-eawziww2/ | 各server-review.json | 存在/结构；raw未抽核 |
| path v3 | E4_PATH.md、docs/path-results-review.md | out/mac-path-main-20260916-164633-run-vvguros8/，short同前缀 | main/server-review.json | 存在/结构；raw未抽核 |
| E3 v4 | E3_SERVICE.md、docs/e3-shared-service-plan.md、docs/e3-results-review.md | out/mac-e3-main-20260917-222837-run-p0lbvbv7/，short同前缀 | main/server-review.json | measurement=true、causal=false、双触发0；raw未抽核 |
| D0/D0 | E3_DIAGNOSTIC.md、docs/e3-diag-results-review.md | out/mac-e3-diag-main-20260918-081411-run-1z6a407q/，short同前缀 | main/server-review.json | 9对18轮、measurement=true、causal=false；raw未抽核 |
| E3 v5 | E3_QUEUE_PAIR.md、docs/e3-queue-results-review.md | out/mac-e3-queue-20260918-130545-run-d01lc9wp/ | server-review-v5.json；out/e3-controlled-20260918/v5-reviewed-facts.json | 验收部分checks抽读；原raw未核 |
| E3 v6 | E3_CONTROLLED.md、docs/e3-controlled-results-review.md | out/mac-e3-controlled-20260918-164635-run-rk48bhte/all-original-pairs.json、console.raw.log | out/e3-controlled-review-20260918/server-review.json | raw pair9 seq17/18 e4/e41/e69/e70/e71/e73/prefix/insert/firsthit实际读取，见下文；其余10对未逐项复核、UART未核 |
| P4M | P4M_BASELINE.md、P4M_START_CONTROL.md、docs/p4m-{short,pilot}-results-review.md、docs/p4m-formal60-recovery.md | out/mac-p4m-short-20260920-180713-run-l7cin11d/；out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/；out/mac-p4m-start-20260920-211940-run-46nvmwgi/；out/mac-p4m-formal60-20260920-215406-run-cicggil4/ | out/p4m-short-review-20260920/review.json；out/p4m-pilot-review-20260920/focused-evidence.json；out/p4m-formal60-recovery-20260920/{audit,statistics,validation-final}.json | audit实际抽核offline25/25、no rerun、原online24/attempt25/unrun35；raw未抽核 |

v6 raw pair9：D0 seq17 e4=6089/e41=6361/e69=5958/e70=6370/e71=6371/e73=6375；D128 seq18 e4=6113/e41=6514/e69=5982/e70=6523/e71=6524/e73=6528。两轮prefix相同、insert1/truncate0/firsthit1。人工差分 A剩余272→401、V剩余281→410均+129，V自身4→4，触发前V131→131。这是本轮有限原始字段抽核，不是重跑验收，不代表所有11对复核。

## 3. 对旧wiki/私人会议稿应补的具体历史内容

未取得原稿逐句文本，以下是对照其若仅凭六月至九月Slack摘要的修订清单，不冒称逐句比对：

1. 不能只写“7–9月搭平台，10月找方法”；9月8日迁移与benchmark审计、9月9日校验/HPM缺陷、9月15–16日SC.W和DMA一致性修复是可信测量前提，且功能故障不等于IOMMU干扰。
2. 删除把旧Pair公共cycles/active slowdown称V独立时间的表述。V独立软件窗是9月8日v1新工作，硬件完成窗是9月16日batch另一协议。
3. E1約79%慢化须接E4“零重叠/H不变/P增量”否定简单硬件归因，再接batch自主供给“99.7%重叠/硬件慢化”。少掉这步会错误叙述为同一数据逐步证明HOL。
4. DS50→0→50是一次三阶段恢复，物理映射/miss模式不全同；不要把差值称纯PTW或完整因果。数据后段无miss共跑竞争也有证据。
5. E3不能只展示成功v6：至少用一段交代v4无双触发、D0诊断前史不同、v5覆盖修好但身份和截断仍不合格，转为有限64B硬件时序。这是研究方法的重要纠错过程。
6. v6是4互补对局部等待传播，V后来首次查hit，不是排队当时已查完hit；hot没有实际+128暴露；唯一pair9前置时间全匹配，其他3保留。不能推广全部旧A8或普适架构。
7. P4M固定gap是基线、A下降是成本；保留gap500反例、pilot开窗历史和formal第25采集故障。9月恢复状态35未跑与10月完成须按时间分层。
8. 9月末N/P是可否决设计卡无原型无新颖性；P静态对照并非算法。会议资料准备不等实际讲完；日语v1 wiki仍写无实板数据，是9月8日历史稿需更新。

## 4. 矛盾、证据边界与当前衔接

README.md仍9月18日v5未上板，repository-map同；E3_QUEUE_PAIR和P4M_START_CONTROL顶部仍保留构建时未上板，结果/HANDOFF/validation后续段明确已跑。这些是历史快照陈旧，不能据其否定后续运行。patches.md“双路Independent已实现”是迁移前旧程序，v1后来已退役；不是当前可直接用接口。E3_CONTROLLED §10仍称418是预期/未测，顶部和结果明确已实测，须注明协议原文保留。

本单元最新研究计划7–38行已出现RCS1/BR1，比此前只读CO1更晚：RCS1原四engine条件固定SJF不差→原版性能主张关闭；BR1固定图族被StaticCP覆盖，动态共享服务改变族未决/性能入口未过，唯一下一关要求超StaticCP/Hpin/localcost/owner/next-use合法动作或完整成本见证。这应由最新机制单元核实完整新报告，本单元仅将所读计划作当前衔接，不新选方法。

未重新核原学位论文、旧工程所有源码、全部UART/CSV、板卡当前状态、所有时钟/CDC/IO；全文读完本单元文档不等于重验实验。HPM完整E0/S2/IRQ/DDTW和下载哈希/SD回读缺口按当次范围保留，不能自动全写通过或所有板测无效。证据优先级：原运行参数/记录与直接事件>验收/分析>当前汇总>历史摘要/计划；科学结论还取决于合同可比性，exit0不取代因果门。
