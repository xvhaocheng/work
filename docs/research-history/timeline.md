> 2026-10-17解释优先：历史E3无条件因果表述受时序未闭合限定；详见本轮证据更正及文末增量。旧正文原样保留。

# 2026年7月至10月14日研究时间线

本时间线按原始提交时间、正文实验日期、阶段及运行身份排列，不按mtime。各节保留设计、实现、实验、失败、否定和转向。标题单元仅是阅读分配，跨月追加以正文为准；重复同步记录不计独立实验。当前状态以README及本次来源版本为准。原始CSV/UART未全读，抽核范围随章节注明。

## 阶段导航

| 时段 | 问题与转向 |
|---|---|
| 7—8月 | 多DMA软件入口、继承AW修复；八月逐日证据缺口 |
| 9月8—15日 | 平台迁移、测量审计、校验/HPM与SC.W/缓存功能修复 |
| 9月16日 | E1软件慢化→E4零AXI重叠→自主batch硬件慢化→DS回切/路径分段 |
| 9月17—20日 | E3失败配对→v6局部因果→P4M双方取舍与采集恢复 |
| 10月1—4日 | P4M补齐、M假设、fullstats/E2、462→88强对照关闭、N功能/资源失败 |
| 10月5—7日 | DS/PTW功能bug、观测裁剪、clean/isolated板测、模型服务反馈、共享池/准入强对照 |
| 10月8—9日 | 错位写/译后分片、普通准备追平、共享读/在途加入、合同/成本推导与路线反省 |
| 10月10—12日 | M0完整任务、普通DS/期限/准备、大页AI、R107有限实板恢复及工程上游支线 |
| 10月13—14日 | 多个来源/适配关、stage117六案、R1/R2/PTE/AW、A—H、CO1→RCS1→BR1 |

以下是分阶段审阅细节，可按标题直接定位；原始作者与历史执行归因保留，不把此次阅读写成新实验。


---

# 2026年7—8月：多DMA扩展与日期证据缺口

本单元由主席直接只读查看现存Linux、CVA6和IOMMU仓库的作者提交列表、选定原始补丁；另阅读全文早期Git证据文件、归档恢复说明及工具准备说明。未以10月回顾替代七月原始提交。Git读取只用log/show，没有checkout、fetch、构建或实验。源码历史不是完整日记，不能据无提交宣称没有工作。

## 原问题与扩展

原研究关注Accelerator-side IOMMU的Remote Page Walk自身代价：用FPGA可控页表延迟区分页表读取与载荷搬运，Pinned与Bounce Buffer/CPU copy提供端到端参照。此背景不等于真实多Chiplet、P2P安全或完整一致性系统验收。后来多DMA扩展增加了设备间翻译缓存替换和共享服务等待问题；它不是突然从10月CO1开始的研究。

## 七月可直接核对的原始事实

| 日期（提交原始作者/提交时间，JST） | 实现 | 原始证据与范围 |
|---|---|---|
| 7月9日20:49 | Linux增加idma1、DID11；新增按长度单次搬运IDMA_IOC_RUN_ONCE及bytes/cycles/miss/PTW/active/errors返回 | Linux `0cb98744633bcb038ed691f9fd0cd69d23e289a2`。本轮直接读设备树及UAPI补丁；驱动119行变化只看stat，未全文代码审计 |
| 7月11日01:28 | CVA6更新IOMMU子模块指针6cc8ed30→7fbb2b99，引用大传输AW优先死锁修复 | CVA6 `3e409badcca73d786b510319ef09444fbdf3a731`。本轮直接读gitlink补丁；修复自身提交日期为6月29日，不能说7月11日重新设计了修复 |
| 7月26日18:03 | Linux加入idma2/3、DID12/13以及Pair/Multi ioctl | Linux `48185ad5dc62d32bd4429bf63b955ba1daceb47d`。题名“3个idma”与实际四路DTS不同，本轮直接读完整DTS/UAPI补丁；软件描述不证明当时四路RTL/板测已完成 |

仓库路径分别为 `/home/xvhaocheng/work/iommu-platform/linux`、`/home/xvhaocheng/work/iommu-platform/cva6`。补充直接取证文本保存在本整理目录的 `evidence/july-august-git.txt`、`evidence/july-selected-patches.txt`。这是原Git内容的审阅副本，不修改或补写用户历史。

原基础rootfs由7月11日镜像继承，归档 `platform-archives/restore-check/assets/README.md` 明确它并非全部用户空间源码重建。后续可复现范围是由该固定镜像打包新内核/benchmark，不是整个镜像来源已完备。

## 八月的具体缺口

本轮对已知Linux、CVA6、IOMMU仓库运行只读 `log --all --author=xvhaocheng`，限定7月1日至9月1日；取得上述3个七月作者提交，未取得八月作者提交。该筛选不覆盖其他作者、未提交工作或缺失ref，因此不能推出八月没有研究。

目前工作区中没有找到可供本轮阅读全文、明确属于八月的逐日日志/周报。九月8日迁移把四DMA RTL、HPM edge、Independent/drain/quarantine与计时修改保存为版本；这些修改的最初开发日期仍不确定，不能把保存日期填为八月某日。早期Barrier/Lockstep表格仍缺命令、版本和完整原始运行身份，不能用后来汇总替代它们。

## 平台之外相关记录

本轮完整阅读 `platform-archives/restore-check/README.md`、`docs/{patches,validation,build}.md`、`assets/README.md` 五份归档说明，明确迁移/离线恢复、HPM定向测试、FPGA构建和仍未上板的历史边界。它们是9月8日快照，不作为最新板端状态。

另阅读全文 `tools/claude-advisor/README.md` 与 `preparation/{draft,revised}/README.md`，核对工具草稿→授权限定修订→离线测试准备的支线；没有读取任何密钥或实际配置、没有调用Claude。工具准备不是科研性能结果；历史默认分工与后期Codex-only要求分别保留。

## 对旧材料的纠正

- 不能写七月已验证四DMA共享性能：七月原始证据主要是软件入口与继承修复，早期性能表原运行身份仍缺。
- 不能把“实现了3个idma”提交题名当硬件路数，DTS实际有idma0—3。
- 不能凭9月迁移的保存提交日期倒填八月开发时间。
- 当前缺的是八月原始日记/周报和早期运行证据，不是用新实验填补历史。若以后发现原始记录，应按正文日期、commit、命令及运行身份补入时间线。

本单元没有读取原学位论文全文、全部旧工程源码、七月驱动完整实现或全部早期原始实验日志。上述限制同时记入coverage与首次daily记录。


---

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


---

# 10月1—4日单元审阅

## 实际覆盖

逐块模型阅读全文144/144块、25来源、分配快照共607KB左右。覆盖HANDOFF本单元48块，validation对应35块，status-history对应27块，其余21份阶段规格/计划/文献/交接/报告全文。具体源路径及行段见ledger-oct_01_04.json；此处不是声称HANDOFF整体或validation整体已全文读完，那由根汇总其他单元。第一次11—27批次工具包装截断涉及chunk21，已单独重读完整；其余批次限约14500字符，无依赖截断。

重复关系：HANDOFF、validation、status-history大量复述同一事件，不能计为独立证据/独立实验。research-n-prototype-spec、nonblock-pilot-plan、nonblock-v5-qualified-read-plan保存历史状态并追加新结果，已读后续修订，不能只引用顶部“构建中/未上板”。composition-v1-mac与software-update虽文件名10/4，但正文追加10/5—7结果；此单元已读全文，10/5以后细节交由相邻单元整合。paper-logic顶部10/4“尚无N实现”仅对应当天较早状态，与当日后续v5完成并不矛盾，应以事件链解释。

额外只读SSH定向抽核：E2 accepted-summary、462 accepted-evidence、method88 results JSON相关键及部分比较、v5-board-analysis-acceptance；原始E2 W8 r1的input/research2、N-v5 A8 r1 helper words/diagnostic，以及E2/462/method88各一份record的success/errno/键结构。P4M RESULTS前6000字符、fullstats RESULTS前6000字符、method88 RESULTS前6000字符为抽读，未计全文。原始raw4096未逐字全核、未重新算结果或重跑实验；递归record数量128/924/176含board与根副本，绝不是128/924/176次试验。

## 阶段叙述与转向

### 10/1 UTC晚—10/2 JST：完成P4M基线，再设计M

根HANDOFF:1902—1921、2472—2578，validation:1504—1519，paper-logic“并行计划/五轮最终裁决”。原P4M60补缺并非只新跑35：原25＋原保留6＋新boot29，53在线＋7离线追认，原缺失exit不补写0。正式报告生成2026-10-01T16:34:18Z，即JST10/2。每gap20；V中位108.813/150.603/171.389 MiB/s，A同窗108.813/75.301/57.088。固定gap收益与A代价成立，仅这三个设定，无最优/所有限速无效结论。唯一b15-g0整轮V miss/S1=2/2，却gap0最快；相位未知，不能称冷miss/替换/加速。

两轮预审后正式五轮：七项候选收敛C1+D4为M；重排缺合法trace、合并接口/前提不足、安全通道可见性/先行工作不足退出主线，D1仅诊断。N/P/R常规路线不再自动实现，但未被M击败。M是下一完整4KiB copy合法ADMIT边界准入，A owner且REQ_TRANS/AWAIT_RESP每拍扣有符号余额，旧b补给后更新下窗b、负债保留、首copy也准入、已有VALID不撤、错误只停新放行并排空。COMPLETE另列，不是把所有干扰时间计费。

H-M0：相同字节/req/miss及训练固定类型权重之外是否还有有用成本信息；H-M1：相同外层反馈与预算、匹配V完成及短窗目标下能否比强对照保留更多A。两者必须分开，不能由“信息更多”推控制更优。时间计费/信用/反馈已见SafeSU、AXI-REALM、MISE及直接IOMMU技术披露，本身不是创新。

84诊断包实际实施，E2最终用4KiB而非早期64B草案：6short+60E2+18cost。Linux00090、observer v8。记录RAM多字写综合失败→宽记录单端口；布局不足→仅研究profile关闭旧E3详细追踪。初板第4项Solo EINVAL，继续既定非Solo后80成功/2失败/2未运行。失败发生公共validator拒零A页，在DMA/映射前；E2 W0不受影响。分析21个E2时点警告和14个关窗lane_active警告属于分析口径错误，修正重算原数据，不删警告或伪改84全过。

00091最小Solo适配修复只改占位页为1、NO_AGGRESSOR与research a_pages0不改，无A执行；独立11项2short+9main新boot通过，不能新Solo拼旧nonSolo。Solo283.485、A8g0 108.857、g2000 V171.488/A57.115；全部主V硬件window hit16384/miss0仍共跑慢，证明这组损失无需额外V miss，但不分摊翻译/数据根因。

### 10/2—3：排除抽样偏相位，完成E2并投入M强对照

固定每16次抽样在gap1000只抽到miss0，漏1536/2047两miss copy，不能外推总体。fullstats六组汇总窗口内完整copy，carry-in/out除外；21=3short+18main，00092新BIT和新软件同包，10/3板上及离线接受。req4/miss2全量均值约1408/1101/841周期仍不同，支持简单req/miss不能精确表示成本，未否定丰富类型计数，未支持H-M1。A1 main仅20周期仍明显减速是反例边界；A16前两次V miss2保留，taint0不抹hrtimer警告。全量sum/count是均值，不能写总体中位或p99。

部署偏好从强求同包改为减少SD更换：共用Linux00093＋role1 E2/role2方法两BIT，Mac/JTAG切换后各新boot，非运行态热切。两BIT构建中net声明、组合反馈、ARM长路径和DDR布线问题均修正后最终时序E2+.177ns、method+.048ns；原负时序BIT不交付。完整IO/clock/CDC签核仍未完成，不能“全平台已签核”。

E2新64=4short+30boundary+30prefix，00093/role1单boot64d4328b。W7正反各5次保留两target；W8正反各5替换，有限前缀N1保留、N8/N16替换。完整链：V初在→DID11实际更新覆盖→deadline遍历不在→A排空后对应首miss，探测自身替换独立归属。128target中62替换首miss/66resident hit不是128独立试验。W7首次4KiB696周期；W8 1483/1486，D88全部5，D89 20 vs822/821；不是纯排队差，更不是稳态吞吐或普适W8阈值。采集tar-z失败只重取第1记录，不重跑。

### 10/4凌晨：462得到弱对照收益但强反例；88关闭本版p5

462=9short+12ref+89calibration+192formal+40boundary+120reverse holdout，单boot93406a68、00093，结束00:50:01JST。全462技术接受不等创新。正式p5对fixedgap t150/t170 +15.289/+24.211%，但t170对p1/p2 −6.464/−6.110%。p5 vs p6反向+40.672%正式−3.791%，方向与阶段混杂；反向fixedgap不保护匹配；A1/A16 p5未达150。0xE25只许可关闭非stall；E2E含IDLE stale DID非实际cost分母；p5已包含DDT/PDT/PTW响应等待，不存在“漏DDT因此改全段”的解释。

88冻结A8 target170、7method×2方向×6轮＋4short，沿用462选择不重新调参。原计划两fresh boot被部署修订为第一复用既有boot含462历史、第二新boot236b6316。每boot/方向n3，强基线在p1—p4中满足V保护匹配且A均值最高者选；四格−5.68952%/−8.23689%/−5.15142%/−4.43680%，按预定规则结束固定A8实占主创新路线，不为p5继续搜有利条件。反向p5胜p6两boot复现约+34.68/+30.99%，保留但不能当独有收益。无显著性/全条件无效/等价证明。

### 10/4白天：N从接口正确性失败走到触发，未得稳定收益

same-DID复用A context草案被否决；两银行完整头/同ID归属、自持DC/权限、单响应仲裁，普通hit-under-miss基线，非新方法。v0短Solo/A1字节正确、A8停滞；RO接收全0由generate作用域implicit wire已证，但非A8唯一根因。AW跨银行重排候选小仿真排除，不能因此排除全部读写循环。

v1读守卫36模型场景过、BIT时序过，新增rr两burst模型banked8/12 fail/legacy12 pass，故功能拒绝未上板。v2 DMA0/1各4KiB、净增8144B、meta11→520，有限模型过但combined LUT207069/207226>203800布局失败。v3局部RAM解决布局，真实FIFO有效队列等价9/9（454278检查），S/N各15板测过但fast0；只有fast字段，不能说candidate/probe0。v4修实际flat MSI mode1兼容＋diag，S/N30过candidate/probe/fast0，是冻结入口未行使，不是无缓存hit。v5唯一对齐4KiB job fullbuffer资格只放DID10/11读入口，scope/source-list/ENV失败保留，恢复两BIT过；S/N各15板测真实触发，N A8每run fast6144/6146/6146、A16 8193。N/S中位A8+4.15%、A16−2.69%，N相原版−14.52/−22.09%，A8三轮104.263/92.987/93.048全部保留。不同BIT/boot、n3、原版3拍而S/N512拍限制因果。105完整技术项目不加v0部分/v1v2模型，也不把method88+N=193当重复次数。15:55:56/16:10:06JST S/N完成，07:22:10UTC验收，按用户暂停。

当晚解除暂停后五轮复审，composition-v1同BIT四模式S/N/P/PN、16项P2/14，Vgap0/1000/4000，172=16short+12S/S+144explore。保护/绕过/组合本身非创新，PN须超过强单项且条目存活/真实fast可分；非安全隔离。新ioctl0x11、raw128/generation/quiet切换已实现、Linuxcpn1过，10/4BIT布线中断原因未知；后来10/5恢复失败是相邻阶段，不把10/4running当现行状态。

并行软件基础设施：10/4低RISC网驱动/SPI/SD原分区更新器与Mac/JTAG重启工具软件65项及构建过，当时真实板闭环未验。不要把后来net2/net3实板成功提前写到10/4；普通reboot无CPU复位设备。Claude顾问仅用户点名且本地隐藏输入准备是工具支线，不是论文成果，也未读其凭据。

## 关键实验可追溯路径（全为Dell绝对路径）

根前缀 /home/xvhaocheng/work/iommu-platform/ 。下列并非此轮重验。

|实验|合同|实际运行|验收/分析|本轮raw覆盖|
|---|---|---|---|---|
|P4M60|/home/xvhaocheng/work/iommu-platform/docs/e3-next-method-plan.md（本单元未读全文，历史合同由九月单元）|/home/xvhaocheng/work/iommu-platform/out/p4m-formal60-recovery-20260920/evidence/p4m-formal60-215406；/home/xvhaocheng/work/iommu-platform/out/p4m-complete-review-20261002/retained6；/home/xvhaocheng/work/iommu-platform/out/mac-p4m-remaining29-20261002/collection/unpacked|/home/xvhaocheng/work/iommu-platform/out/p4m-complete-review-20261002/RESULTS.md、summary.json、trials.csv|此轮只报告摘要抽读，未核60raw|
|84诊断/Solo修复|/home/xvhaocheng/work/iommu-platform/docs/experiment-plan.md末节；/home/xvhaocheng/work/iommu-platform/docs/mac-followup-20261002.md|/home/xvhaocheng/work/iommu-platform/out/mac-research-v1-20261002；/home/xvhaocheng/work/iommu-platform/out/mac-research-cont-20261002；/home/xvhaocheng/work/iommu-platform/out/mac-research-followup-20261002|/home/xvhaocheng/work/iommu-platform/out/research-board-review-20261002/cross-segment-summary.json；/home/xvhaocheng/work/iommu-platform/out/research-followup-review-20261002/acceptance.json|未核raw，仅完整阶段记录|
|fullstats21|/home/xvhaocheng/work/iommu-platform/docs/research-fullstats-v1.md|/home/xvhaocheng/work/iommu-platform/out/mac-research-fullstats-20261003|/home/xvhaocheng/work/iommu-platform/out/research-fullstats-review-20261003/RESULTS.md、acceptance.json、direct-check.json|正式报告前6000字符抽读，无21raw复算|
|E2新64|/home/xvhaocheng/work/iommu-platform/docs/research-two-bit-v2.md|/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/e2/runs/|/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/accepted-summary.json、RESULTS.md、audit-final/board-audit.json|抽核W8正向r1 input/research2及一W0 success，非raw4096事件重放|
|method462|/home/xvhaocheng/work/iommu-platform/docs/research-two-bit-v2.md|/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/method/|/home/xvhaocheng/work/iommu-platform/out/method-results-review-20261004/accepted-evidence.json、RESULTS.md|抽核一boundary raw记录success/errno，验收相关键|
|method88|/home/xvhaocheng/work/iommu-platform/docs/research-method-next-20261004.md|/home/xvhaocheng/work/iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/|/home/xvhaocheng/work/iommu-platform/out/method88-analysis-20261004-01/results.json、report.zh.txt；/home/xvhaocheng/work/iommu-platform/out/method88-execution-20261004/RESULTS.md|抽核一fixedgap record success/errno，分析summary/boot/部分比较；未88raw复算|
|N v0—v5|/home/xvhaocheng/work/iommu-platform/docs/nonblock-pilot-plan-20261004.md、nonblock-v5-qualified-read-plan-20261004.md|/home/xvhaocheng/work/iommu-platform/out/n-prototype-20261004/nonblock-pilot-20261004-{original-03,serial-v3-01,hit-v3-01,serial-v4-01,hit-v4-01,serial-v5-01,hit-v5-01}/|/home/xvhaocheng/work/iommu-platform/out/n-prototype-20261004/v5-board-analysis-acceptance.json、analysis-v5-pilot/RESULTS_REVIEW.md|A8 r1 helper words/diagnostic core_fast6144及一A1record success/errno|
|composition-v1|/home/xvhaocheng/work/iommu-platform/docs/composition-v1-spec-20261004.md|10/4无板run；失败构建out/paper-route-review-20261004|同目录software-build-state.json、recovery-v1/recovery-state.json（后者10/5失败见相邻单元）|未核raw，10/4不是已测性能|

表内路径已展开为Dell绝对路径；不把Mac本地链接冒充远端文件。

## 会议草稿遗漏/纠正

1. 必须加入平台之外的部署/测量能力进展：硬件batch共同窗、Solo适配故障与分段、全量统计去除固定抽样偏相位、两BIT共软件减少SD更换、N读写依赖和RAM资源失败链；不能仅“完成更多数据”。
2. 九月“P4M25可用35待补”改为60完整，但53在线7离线、25/6/29分段；删“V全部0miss”。
3. “E2未证/84未上板/M待实现”在10/2日本语稿为历史；10/19改为新64局部替换、462/88实际评价、p5本版结束。
4. “实占比miss计数更精确所以更好”改为H-M0简单计数信息差异，H-M1强对照未显示增量；胜fixedgap不够。
5. “N已加速/完全无死锁/两个时序过即可板测合格”删去。完整路径真实触发成立，收益不稳/相原版更低，模型/物理实现/实板分别写。
6. “组合容量保护与命中绕行创新成立”改为10/4启动有停止门禁可行性筛选，后续必须纳入相邻单元的PN负结果/普通覆盖。
7. 文献纠正Bermuda SMMUv2实测、SMMUv3只是discussion；ECRTS2023不是RTSS2022；时间计费/hit-under-miss/分区已有，专利披露非效果实验。
8. 不把五轮两模型模拟当6位独立同行审稿，不把193功能清单项目当193机制重复。

本单元未访问旧私人wiki/会议草稿原文，以上是依据历史稿与记录的具体纠正项，非逐句红线。全文阅览不等于重新证明文档记录；没有改Dell、运行构建/仿真/板实验、读取凭据或外部发信。


---

# 10月5—9日机制单元A：最终全文审阅简报

审阅2026-10-14本地只读快照；远端源均在`/home/xvhaocheng/work/`。**本单元182/182分配块全部真实完整展示阅读，57批全部完成，未读0、截断0；涉及42个来源文件，其中本单元HANDOFF分配59/59块全部读完。** 不宣称读遍所有源文件：部分长文按分配切块，另一B单元负责互补范围；完整逐块路径和起止行见`ledger-oct_05_09_a.json`。

本轮没有直接打开原始out回执/CSV/LOG或实验源码，以下实测值来自已读当时HANDOFF、专题报告和validation记载，并非重新验证或原始证据复算。70/87/107/120等数字及“剩余/下一批”仅保留在文末分批过程记录，均是历史，不是最终覆盖状态。本单元没有远端文件修改、实验、构建、仿真或板测；统一Dell归档由根任务负责。

## 10月5日：P/N/PN工程实现与路线纠偏

P为固定2/14缓存份额，N为PTW忙时另一bank合格hit旁路，四模式同一IOMMU/同一BIT运行时切换；四模式不代表四个IOMMU。既有512拍DMA0/1缓冲非本轮新加，四DMA中只有前两路有完整copy保证/大缓冲。

composition-v1恢复构建最后31 node overlaps、WNS−3.268ns，拒收，未上板；不能把route_design命令成功或DCP存在当合法布线。原critical path为IOTLB tags_q到旧research observer event_ram。实验观测持续叠加，四同级观测/组合控制综合LUT33390、FF25217，不等于全部可删或方法硬件开销。composition profile裁剪旧research事件记录与旧N诊断，保留通用/path/P4M及双快照、128字段、方法。10联合scenario及4入口检查过（非实际AXI/SoC），新base工具exit0但WNS−0.380拒收；单次route-opt 03:06 JST WNS+0.058/WHS+0.052/WPWS+0.062、273893/273893布通、routing errors0，accepted BIT。最终routed LUT163673、Slice99.45%，全IO/CDC仍未签核。根HANDOFF3029—3102；专题`iommu-platform/docs/composition-handoff-20261005.md`、`composition-routing-cost-20261005.md`、`composition-trim-20261005.md`已全文读。

网络工程从Mac百兆0bda:8152/en11、旧驱动只支持1000，改Linux驱动100/1000并增量net2/net3。net3真实SD写入/同步/JTAG/开机联网/SSH身份通过，科学实验0；BIT和Linux发布不可混版本。`/home/xvhaocheng/work/iommu-platform/out/ethernet-workflow-20261005/ethernet-workflow-20261005-03/result.json`。

冻结172最初52通过/1失败/119未运行，N板端卡死；模型N4096/持续A、并发warm和CPU写AMO皆过，缺MIG/板级转换，未复现。只读USER4快照v1/v2对CPU/COMP/DS/DDR/Wowner做观测。v2正常三帧后N4096再现；卡死帧5/6/7显示Wowner FIFO count4/headDS、只DS ready但DS WVALID0，外部DDR/atomic可接受，队头等DS数据。不能仅从地址断言FQ。Codex源核：AWID CQ0/FQ1/MRIF2/MSI3/PQ4，而原W数组PQ0/CQ1/FQ2/MRIF3/MSI4错位且重复case3，63f790a接PQ引入。唯一DS RTL纠正数组及空满保护。旧源码scenario1 AW1/W0/B0 watchdog；新四场景均通过（FQ4拍、并发9burst21拍、提前CQ4拍、满队列12单拍）。该工程bug和后来的PN性能退化是不同问题。证据目录`/home/xvhaocheng/work/iommu-platform/out/composition-dsfix-20261005/targeted-test-acceptance.json`、`test-before-final.log`、`test-after-final.log`；v2原始现场`out/composition-hangdiag2-20261005/mac-records/diagnostic-board.tar`。根HANDOFF3116—3206已读；修复BIT后续正式172完成由B单元后续块合并。

六席五轮30答复只交研究计划，初接口首选随后用户质疑转向，根10/5纠正继续性能主线，接口降备选。模型投票非创新证据。开放搜索零散动态映射后又按用户请求先研究地图（36独立论文38全文版本/4官方文档），候选暂存，不自动转安全或实现。根HANDOFF1712—1749。不能让早期“接口优先”“动态零散内存优先”覆盖后来的纠正。

## 10月6日：文献、首篇缩题与预审负结果

专题共享翻译72独立工作、59全文/60版本、13题录摘要，Codex核12关键原文、DeepSeek整理卡；全文取得≠通读全部或复现实验。补VIO/BaM/GPUrdma/GPUDirect，GPUDirect1:1直通限制意味着NIC-GPU存在不能证明共享IOMMU翻译竞争。六瓶颈地图是综合，不是领域空白证明。

用户博一不要求顶刊，首篇A页工作集感知提交、B配置边界选择一度优先，但最新预审两者均未放行实现；必须复用已有P4M/method88，不能“从零建立限速基线”或挑有利条件救p5。新A要区别字节、请求、miss、加权事件、实际占用、V反馈；gap为完成后闭环等待，不是在途深度，512拍buffer不等descriptor队列。首篇文档119行已读，后续是否有续篇由B单元合并。`first-paper-options-20261006.md`顶部最新修订与下方历史优先级不能混用。

C1共享/存储已有ShareMMU等前史；实际ShareMMU artifact替换max age−5shareCounter而非论文有限LRU公式，接口至少一端scratchpad非双外存。普通逐读预约O+R≤B能在服务/排空条件下解除原C2返回环；AW-before-AR已有。新U→B→W→M→U环只是拟议条件、iDMA真实可达和合法更新范围未核，不存在假定global revoke lock。C2宽泛依赖准入未具体增量，不入RTL。A1局部性消费者等待暂缓，B1 PN组合边界仅低成本归因候选。根HANDOFF1593—1703。

clean BIT完整构建base−0.131ns拒收，单次post-route+0.169ns/+0.038ns、三失败0，LUT156215。首次更新旧内核wc在shmem读取软锁死，未调用新BIT JTAG，不能归罪新BIT，根因仍开放；恢复旧ptwfix后第二次34m34s更新成功，新cpn3-clean boot身份，JTAG本轮2/SD1/实验0，新BIT不能继承旧172。根HANDOFF1548—1559。

## 10月7日：历史隔离、板端供给干预和强普通准入

PLRU fixed2/14按resident way更新所属树，S/N fullmask不变；生产IOTLB/TL两文件改，原25834检查、新9232和TL回归过（合成初态不等DMA可达）。旧clean PN退化、A miss5117→6582只是线索；isolated结果由B单元补。

PN算法审计没有确认解释12.74%板退化的bug，不能说无bug或必然架构效应。Claude提出跨bank AW老owner例与demux175—177禁止条件冲突撤回；DeepSeek误把编译NonblockEnable作P/PN差异撤回。单TL52相位均过不等完整DMA/DDR。旧窗口AW反压+863488与总W+896481.67重叠定位不能相加归因。

同isolated BIT/boot18项gap64板测03:52:54—03:55:34UTC，A8/P2/PN3/gap0/1000/4000/block1—3、warm256、V64，18/18/no taint。gap0 P/PN W109839.3/123956.7（V+12.85%、A−11.39%）；gap1000 +3.73%/−3.85%；gap4000 +0.08%/+0.03%，FAST降5—8且A完成315—316，不能说N同强度修复退化。gap增V绝对W也增，不能扣gap冒充独立服务延迟。硬件无PTW独立计数，支持供给敏感性，未证MIG FIFO或全板因果。合同`out/pn-board-path-20261007/reviewed-plan.json`；run`mac-records/pn-gap64-20261007-formal01/`；验收`board-acceptance.json`、`reduction.json`。

逐PTE B额度PoC相对PN−1.203%，仍慢P0.531%，普通周期/wholewalk更好，普通在途同效，DS优先B只再省5周期；这些专题fragment327—420已读，同条件默认S/N/P/PN105588/152042/91878/93480补N数据，未实板。C源热目的冷5短仿真一般源先行成立但非同job FAST严格预测。PN中A FAST提前DDR使V根PTE服务32→577，优先返回干预回32，只是行为DDR队列干预，非可实施MIG仲裁/旧板12.7%全差。C未实现，不能说C测差。

普通owner8/8屏蔽FAST虽少退化但新增本bank源重walk；ordinary wholewalk256保FAST64、冷PN91727<P92334（0.657%），所以旧C首选停止。此是准入位置改变touch/PLRU的顺序效应，非跨bank共享历史旧bug。合同/运行/验收在`out/owner-baseline-20261007/codex-test-spec.md`、`runs.json`、`acceptance.json`；本单元未打开raw。

完成预约Mode4单槽/5普通两槽/6精确下一W流式/7无POOL，无池Mode7热P/PN92109/92374、冷92549/92814覆盖共享池主价值。代理fast_pending被后续翻译覆盖，新每bank8 READtag修；29执行只有21最终矩阵＝原13+修标签8，旧计数unreliable，周期/bytes仍可保留。48B仅payload，不含TC/FIFO元数据或面积。`out/completion-reserve-late-20261007/{acceptance.json,runs.json,runs-all-executions.json,comparison.json,codex-review.md}`。

同bank无池48B真实2DMA warm卡住，TC/AR头A，外部AR头V，WheadA无数据，sharedR/PTW空；三watchdog非生产板bug。双bank48B、同bank8192B、普通源prepared写门48B皆过但A完成量不同不能净排名。候选完成AR头暂存+bypass尚未实现、(K−1)L非定理。`out/write-order-boundary-20261007/{stage-evidence.json,codex-review.md}`。

payload-frontier复杂扫描与普通准确virtual-W/preparedAW两对CSV逐字同，PN约6.25%改善不是独立规则，仍慢P0.6009%。virtual WLAST不等DDR/B完成。tests副本有8PASS，生产hash同；scan降普通工程。`out/payload-frontier-poc-20261007/{runs.json,stage-evidence.json,codex-review.md}`。

## 10月8日：从写依赖到译后分片，收益被强普通逐步消解

读服务串行/接受后重叠4仿真均过，P/PN92109/92374→85339/85984，PN差仍在；计数empty/setup/beat/backpressure互斥账本不等模块因果。四源36读交错九策略324抽象分析34680累计states非四DMA RTL；普通W-bank预留2头已无停滞。

CONF0源目的共同边界，旧PN合同不能用CONF1源2048/目的1024+1024错位解释。RAWCouplingAvail编译参数与conf不同。新同bank自然CONF1长任务发现C0配置4项/计数3bit第8AW回绕关闭W，out修capacity门但production未修；修后R尾/Wowner/PTW排队环仍停，ordinary8192B buffer解。13run6PASS7watchdog，不丢失败，非旧PN根因。`out/natural-write-order-20261008/{runs.json,stage-evidence.json,README.md}`。

译后parent/owner固定max128 vs动态最长prepared W前缀：6PASS，动态物理AR64→55但延迟31反而末W+553，延迟100平均仍晚31，不主推动态。FIFO48B/5962声明位未综合，非四DMA/MIG。前/后置8PASS约27.09%差伴入口/metadata不同，N四项FAST0因为同bank不触发，撤回有效N对照。原父翻译复用随后降低TLlookup但并非PTE减，不能把全部速差归32hit。

原窗口配平16run10PASS6watchdog，单父window2/4卡、ordinary多父槽解；同window1后置残差6.706%，不是加槽新机制。普通multi-parent准备18PASS最强均24751 vs post18824（23.95%），但仍源片进繁忙TC。真ordinary prepared物理AR bypass6PASS均18975，原gap5927→151，97.45%被消解，剩0.7958%，故不能以位置/旧23.95%创新。声明4598 vs4602非等总面积；静态6PASS不验证失效。专题历史685—775已读；证据`out/{original-window-20261008,multi-parent-preparation-20261008,prepared-source-bypass-20261008}/`各stage-evidence和root audit。

LTI-style译址寿命反馈当前平台无普通译址结果等DMA完成释放并阻IOFENCE路径，PR/PW TODO、Linux普通PR/PW=0/InclATS0，CQH早推进不等generic completion；不能为创新添加不存在锁。fault+合法discard为强进度控制，成功copy为额外性能合同，当前fault排空未验。

有限排空§42 1build4PASS，普通到达字节下界：V先至少15360R/15368W，A先7168各，7new child无new AW。4115/5436周期不同起点不可比较因果。§43双bank34/34虚offer到物理AW全序一致、max(vr−pr)=1；headlag旧payload退休而非AW倒置，停止冲突分支。只静态CONF1/DEFAULT单job，不是生产一般AW定理或硬实时界。`out/committed-write-drain-20261008/{stage-evidence.json,codex-result-acceptance.md}`；`out/dual-write-prefix-20261008/{order-evidence.json,codex-result-acceptance.md,stage-evidence.json}`。

shared translation可达性手例同PSCID/root共享上级而叶不同，旧§43实际PSCID10/11/Bare不能称nested/shared实测。N只是hit不是第二miss coalescer，IOTLB/PTW独立无统一槽争用。requester history专利已有，alias纸面victim变化不等周期/PN归因。remaining-use有限cache9组36普通records，ordinary E8/gap4达24 compulsory，停止纯remaining算法命名；noPWC miss×3为Sv39数学模型非DUT。

## 10月9日：共享读真实完成→普通覆盖→新合同设计

§50独立src READ/dst WRITE recipe接真实iDMA/TL/sharedR+qW，27run最后12接受、早7PASS、6失败、2参数不适用不当lat0。lat31模型V/A5657/5650 vs q2 8579/8572约34.1%主要少AR请求模型代价，非MIG/PTW独立干预；pause V只低1—4%。single-reset一次pause/resume、静态许可不能推广连续job/全部暂停；raw留`out/shared-read-rtl-20261009/{execution-ledger.json,codex-result-acceptance.md,README.md}`。

后继§52/53早恢复、lookahead普通同轨迹，§57 28PASS接力V慢A快、PTE1000更差，§59 27run26PASS+F255末拍停滞后private末拍修，只特定fullWR/q2，logical RLAST不能代W/B。§62三Q37PASS与§63ordinary end≤peerW+Q15PASS（11CSV同）覆盖大Q“peer算法”。§70—74共170执行152PASS/8旧watchdog/10strict reject：§70 bandwidth模型整体19—39%但V部分慢，multicast解释；§72 A suffix错依赖已完成V READ修ownA；§73 independent V代价长budget慢53.4%；§74早A AR只12—14cycles~0.13%，普通pipeline。各编号目录见HANDOFF1007—1079与3297—3299，全为out RTL模型，非板测。

§76—78共29完整copyPASS（6成功build），修模型RR从idle探测转实际R握手提交，旧interval64零窗/公平服务结论收窄。真实在途join5次E64/63/256、R257 vs弱320，约17.16%优势被强ordinary许可已齐起点共享R256、V19402/A19411比join19462/19465更快。晚许可无join两CSV同。连续epoch/fault/remap/held物理R绑定未测。`out/shared-read-inflight-join-20261009/{codex-result-acceptance.md,execution-ledger.json,codex-next-novelty-gate.md}`。

§80三创新版本被完整普通覆盖：shared-ring同信息/端口/授权/容量O-ring同，Whead guard复现B且旧DUT该例不可达，C120比较每scene/profile具体ordinary同ACK/walk/makespan（不是一个全域在线万能selector）。12 tests/120Python比较非RTL/FPGA，DARTS核源码不是StarPU实跑，LRU/globalremaining脚本修正保留。`out/parallel-innovation-model-20261009/{root-result-acceptance.json,c-comparison-verdict.json,accepted-source-manifest.json}`。

§82结构成本A complete-subblock/PTE two-slot已有，166b普通vs168observer、nested172，停止独有存储贡献不是综合逻辑等价；B18有限Python模型全部事先oracle一致，candidate常态少一AW，但conversion ordinary A同/B早2且无null少流量，成本/前作未闭不送RTL。monotone suffix后继未实现。`out/innovation-recourse-cost-20261009/{root-result-acceptance.json,execution-ledger.json,all-traces.json,a-full-structure-cost.json}`。

§84 N−Ci≤Qi允许所有consumer仅部分destWR整包unsplit-read条件规则，Python6图190states、Ui>Qi唯一deadlock，非sharedRTL；coupler当前关闭，新启firstR只一AW credit不能默认两AW。§86强two-child普通27/33胜单child H3 29/33，物理prefix history同读次数，所选性能点关闭；统一载荷池6槽vs9仅省192bit payload，端口metadata可能抵消非面积。`out/innovation-permission-frontier-20261009/`、`innovation-native-cost-20261009/`根accept/design。

round01 A条件主线/C备选/B普通walker覆盖；A与同合同cursor均10/14/24，correct whole-source8/14/26或8/15/25，无独有系统收益；旧CPU overlap8/14/25非法作废。round03 L-F预留输出、真实正常源READ再按resident/free绑定目的；F1 ownership-change无合法正例不进收益，无模型/RTL/Linux/build。O98与3355bit vs3587bit只差29B字段非净面积。10/9§93从原毕业论文路线审查：原问题合理，测量/供给修正必要；后期贡献不稳而合同持续扩展需纠偏，暂停具体候选，普通复现单例不等全路线无价；等价实现不自动发表前作覆盖。`docs/mechanism-design-handoff-20261009-round03.md`、`out/higher-model-mechanism-20261009/round03/root-result-acceptance.json`；`docs/research-route-audit-20261009.md`和`out/route-audit-20261009/root-route-verdict.json`仅通过本单元HANDOFF引用，专题未读。

## 给私人wiki/会议稿的明确纠正

1. 不能只讲PLRU/PN和CO1两端：10/5真实DS写归属bug修复、诊断现场、观测资源与route-opt、Linux网络更新是独立工程链；方法成本和仪器成本须分开。
2. 10/5原17252/1/119与后来clean/isolated验收区别版本、boot。同一个“172/172”不能迁移新BIT。
3. 10/7板gap收窄退化不是加速或N同强度修好；无板端PTW计数，模型DS优先因果不能改写成MIG根因。
4. C没有实作/测差，普通wholewalk已覆盖单case目的准备效果。shared pool约74%理论payload、后置27%/23.95%、sharedread34%/join17%均须列强普通覆盖或模型请求成本边界，不能当论文贡献。
5. 两真实DMA/四源抽象分析与四DMA生产、RTL行为DDR与实板区别；CONF1错位新合同不解释旧CONF0 PN。
6. 10/6A/B首篇建议与10/9新合同设计都是历史条件候选，后续否定修订优先；不能给“已有方法论文/明确创新”。
7. 当前研究可保留平台、干扰问题、因果局部证据，负结果收窄具体主张而非领域无价值。成功copy与安全fault/discard不是同合同；可达性和完整费用缺口不能靠额外弱化普通对照补。

## 分批过程记录：前期尚未读范围（已在后续全部完成）

本单元第20—56批共112块未展示读取，包含first-paper后续/高阶机制专题、innovation深审、round02—04、literature/full native design、status85—91与validation重复记录等，全部ledger full_read=false。其他阅读单元负责互补片段，本文件不代称全文。所有原始CSV/LOG、源码、完整论文及out回执本单元未抽核，不算原始证据已读；仅已读报告给出可追踪绝对目录。没有运行任何实验、构建、板测或改远端文件。

## 续读补充（第20—34批，2026-10-14）

累计真实阅读全文87/182分配块，next_batch=35；上方70/182及剩112是前一轮历史快照，以ledger/continuation最新数字为准。新增17块无截断。包括高阶机制round01两片、innovation-deeper/followup全文、round02一片/round03一片/round04全文、natural-write全文、novelty-gate前136行、paper-driven全文、parent reuse全文、platform feasibility全文、posttl verdict全文、landscape两片、permission release全文、日语讲稿尾部。其余专题仍未读完，未新增raw抽核。

新增关键纠正与精细证据：

- 10/9 round01 A源回收区间补集：未来F=[I,end)、公开读义务U，首次publicVALID前从F移U，只有完整成功真实R才删U；回收必须无未来/未退休读、无未知别名并有真实CPU/cache所有权。原18 obligation字段2985bit≈374B是字段子集而非净面积，软件可能O(P)，不能宣称全系统O(F+K)。若trusted descriptor已保证不重读，普通真实源进度可能足够，不能新增威胁模型制造性能贡献。证书FIFO满只丢通知/sticky overflow不丢权限账、不阻R，软件未确认页不得复用；page query冻结ARM/PUBLISH但已公开AXI继续。错误冻结证书合法排空，NO_ERROR_HANDLING未验恢复。
- 同文C静态bridge-last7→6 miss/260→240被普通目的分组精确覆盖，e4早但e1—3晚44，非Pareto；K2不是整机2entry。动态split/merge+costed residency无独有正例；九组M1—M9未来计划全部未执行。普通同合同cursor复现A24不自动宣布已发表前作覆盖整个新合同，但不能再声称独有调度收益。
- 10/6 innovation-deeper明确actualRAWcoupler关闭允许物理AW先源翻译，CONF0共同推进不排除下游先后；只属于源码可允许前缀非仿真/死锁。ordinary O+R≤B加AW前全部所需源读已发/已有数据可在公平服务、U只等物理请求等限定条件排除环；不必严格同时先取得两端翻译，Ts+max(Td,Lr)+Lw为普通重叠手算非实测。更新等整个descriptor、fault、额外反压环不适用。若完整差异只靠不存在global lock或不给对照流水，停止C2。
- 10/9 round02 R generic relation dictionary精确复现编码不意味着整体完整成本一致；CoLT4.1—4.2定向原文已核而HPCA2014全文未得。T native一拍续步无长空窗，不为不存在gap建模型。所有closed-form三概率case非workload仿真。
- round03 F1合法原始warm/ownership轨迹必须A真COPY完成→consumer/cache sync→Z真实release在V源READ阶段；source自驱逐X/Y小例被精确victim/pin覆盖，非独有点。F1/F5/F6先行，F2—4按差异，6组首批S1—6只是计划。与ordinary source-prepare+晚allocate比较不准迫使其重复walk。完整索引和新合同复制到ordinary allocator是候选等价实现，非已发表覆盖证据；独立已有准备/pin/布局/容量对照完整复现收益才关闭该收益。
- round04 L-M匹配容量r(H)与Hall是标准数学，{X,Y}/{Y}驱X使两个local h≥1但rank2→1，小例证明scalar不足而非独有速度。guard319bit/若新resident160则479，不能319+479再相加；都是字段子集。BIND需全部合法FREE图可完成后偏热，绑定未译WRITE义务到响应进稳定物理头才退，不能FREE→COPY就清。no-fill必须完整普通RW/A-D/stage/fault检查不免费许可。L-W公共前缀晚叶选择30+δ vs40手算被cancel+PWC匹配PTE次数，取消仅组件；0模型/RTL/Linux/build/board。最新stage91候选并不构成已通过后来的stage92/93路线暂停。
- parent reuse10新run中首4plusargs开关未生效排除受控比较，只6有效新对照。关闭pre CSV与旧同；复用/提前25712/25714 vs25799仅0.337%/0.329%，PTW102未减；后置在首R前已接6raw父，而pre每流只1，physical父槽各1不等同可见范围。字段3514/3518/3568仅声明非等总面积。背景失效输入0、fault可见性生产未处理。
- platform feasibility10/6仍写P4M60未发现后35续跑，此为当日边界；不能用它覆盖后来的实际formal60完整状态。ptwfix172是同boot三block不是三启动，A长batch2^32在V后STOP，不能当固定4096A完整job；PN/N A8 gap4000 V−3.21%～−2.93%同时A rate−31.09%～−30.90%，不能只报V好。quiet/clear是实验接口非规范必须全局静默。
- permission release同池6 vs独立9字收益来自重复基线；candidate和ordinary均6，3×64bit仅192bit字段。弹性前缀11reads vs13、coalition3数据 vs B/D2均手推，被samepool/阶段完成优先或occupancy+partner continuation覆盖；授权本身不释放数据、必须真实W退休，现接口2许可生命周期，四prepare须队列费用。
- 日语10/7既有汇报可核出处尾284—353：旧84=80成功/2失败/2未执行，cost11同boot补充不是续84；E2 64项中的128为页延次数非独立128试验；method88两启动第一复用/n3非显著性；clean172只功能，旧10/5P/PN非clean性能；N v5 S/N各15，A8约+4.2%、A16约−2.7%非普适。这个稿具体遗漏后续isolated板gap、共享池/分片/旁路/shared-read普通覆盖、stage91/92—93停止链，应更新10/19材料。

尚余95块（第35—56批）；所有远端source保持只读，未写Dell docs/research-history，统一落库由根负责。

## 续读补充2（第35—42批）

累计107/182真实全文块，next43，余75。新增中文讲稿1—285（旧计划不能当10/14现状）、shared-read独立调度35行全文、deepening47—114、literature1—94、status through84分配20块。新增报告/历史重复以ledger为准，未抽核out raw。

重点新增：
- §49 event模型314场景/7644check非RTL，run01 598/run02 143fail来自Flow.resumed遗漏等worker实现、保留原稿。B3二FIFO48B+skid8B=56payload非完整面积；model B在WLAST同事件退无delayedB。N16/q2/P3/resume200候选与ordinary tile都214eventsteps，candidate少AR非实测速；independent35steps不满足同指定pause合同不能弱比较。有限tail≤2q需一个已first-offer Wgroup、B≥q、有限公平服务，非固定walltime。
- §51 two-pause RTL15新执行14PASS/run09旧第二quiet失败后修；ordinary mode2/q256与longmode1四对CSV SHA完全同（nopause5657/5650、once5161/19848、twice5161/29793、twice+B64背压13277/28956），同cost同效直接否定以§50 q2弱基线34%为新算法。义务保持即pause不能挡已firstVALID短W需要R，singletonAR要整段Wcoverage。
- §64 ordinary ordered-prefix15PASS末metrics同63，仅5CSV AW早非性能；§65—69合156真实RTL执行148PASS6旧死锁2actual拒绝。Max2 issued-group相等改≤，普通two-B pipeline解释~19%压力改善；minimum-J约1—4%未确立创新。parallel E160 sharedR416vsindependent512少18.75%，V4550vs3883慢17.2%；压力V9824vs7800慢25.9%、A9833同，不能少流量=更快。
- §75 short-prefix29exec27PASS+2route-not-bound拒绝，E=N修独立R512；18short成功strictJ<E窗口0只否定该窄门，不能否定后宽join。源1functionalFF/134observer/0payload非面积。
- §79 C closure4小例ACK31.25→29.75但ordinary hotpage追平，15desc到t28 candidate3ACKvsordinary1仅早期纸面差异，被§80后来完整ordinary覆盖。B兑现被same-slot nextWowner lookahead复现。§81 A cached evidence candidate10reads被PATH-MRU10/leaf+bypass9覆盖；B nulltail104/7被原起点ordinary adaptiveprefix104/6少null，next只cost/前作；C BBMpartialcache复现19而39弱rootrestart。
- §7/8/9 timeline新补具体完整仿真因果：two真实iDMA/DS50-50 A8 V64，FIFO P91878/PN93480（+1.744%），DS返回优先P79847/PN79484（−.455%），93DS地址序同，额外等5505=PTWactive增，VFAST数据先Aleaf；只模型局部因果，不全解释板12.74。
- isolated22/22真实板：最终WNS+.022/WHS+.051/LUT156186，一次JTAG无Linux/SD更新，bootd61e...；A8/gap0P/PN A miss均2048（旧5117/6582），绝对P时间−21.44~21.82%、PN−20.69~20.74%，新PN仍+12.67~12.79%；S/N跨版≤.28%。跨BIT/boot描述性不能把全剩余归因。这个后续结果使10/7中文/日文稿“尚未隔离”应更新为已做隔离、额外miss消除仍性能退化。
- clean功能172最终10/6 23:38JST同boot27m41s172PASS，不是10/6构建启动时已有172，也不是新研究样本或性能验证。module14420→5561LUT与整板165535→156215不能混。
- 文献库最新123独立104全文/105版本19metadata，72是初轮而非10/6最终总数；NB artifact10/8源码已得但论文未得。literature六脉络/问题关系非领域空白证明，不能重复将全文下载计全文本次阅读/复现实验。

## 续读补充3（第43—44批）

实际120/182，next45，余62。新status85—88/90—91全文与through84旧史7块。stage85绝对WRITE连续前沿Fi、privateQi的e≤min(Fi+Qi)为条件安全最大段，不等最快；真实prepResp入owned slot才许用，READY0的TL VALID非权限。四手推controls长21/23、strongsplit20/22、onesponsor22/24、allfull23/25，均R8/W16/3AWB每flow/ownprep6，未实测；ARcost3 long47split48、cost1 long45split44是解释目标非性能。stage86 WRITE记录可回收READ字段不等总存储。stage90成本3355 vs3587差29B另计至少1024B软件generation及1MiBDDR池，不是总优势。stage91 no-fill返回是翻译响应不是payload。root落库前以ledger120为实际统计，不将旧段107/87/70合加。

## 最终续读与覆盖验收（第45—56批全部完成）

**最终182/182完整阅读，42个来源文件的分配范围，728822 UTF-8字节／408364 Unicode字符，57批全部，未读0／截断0。** 这是分配块全文覆盖；与B单元互补后才是长来源全文覆盖。无本轮raw/源码直接核验。逐块清单字段source/start_line/end_line/chunk_id/full_read保持完整。

末批补充此前简报未展开的独立结果及边界：

1. 固定nested链§45：唯一数学graph6450配置（5族×86B预算×15q），mismatch0，可完成总work30。q=PTE最多连续服务数、B=单suspended frame bit，非cycle/面积/全状态；q3/B77普通semantic_chunks路径3/1/3/1/3/1/3、q4/B36普通semanticrestart4/4/4/3均零重读，q3/B36无解。30仅两个各15步/noPWC/no跨请求共享，普通checkpoint+主动让路已经达下界，停止固定链调度创新。存在完成策略不等任意公平策略皆完成，q2/80bit额外纸构未进枚举/非全架构最小编码。合同`/home/xvhaocheng/work/iommu-platform/out/translation-progress-frontier-20261008/codex-model-contract.md`，运行`frontier_report.json`与`execution-ledger.json`，验收`codex-result-acceptance.md`；报告`docs/translation-progress-frontier-20261008.md`已全文读。
2. §41 geometry math11390分类（7585+3805有重复）greedy/brute0非11390DUT，B>0一cut可支持多个stop点，H只safe目标点距不是预算关后的工作/时限。软件3KiB+7×4KiB+1KiB/9subjob/source24/AW17/8cut仅静态未跑。`out/fragment-drain-boundary-20261008/{geometry_oracle.py,geometry-oracle-result.json,stage-evidence.json}`。
3. §83反向history追加纸账：ordinary A107/B110、candidate A110/B14，共3AWB/sourceAR2/realRW15/null0，candidate A慢3/peer早96是条件取舍非实验cycle/全ordinary最优；原两N12/Q3 normal/gap被futureblind adaptive普通同时匹配。wholechild动态q上界≤Q与wholejob累加、null≤Q(Q+1)/2、AW≤1+Q均工作界非walltime。ordinary pretranslation+nativecoupler+oneprimer逐动作等价，停止两scene调度试验。接口nonclaiming prepare/真实physPA-route/全source目的覆盖是显式新费用，不能fake LAST闭合。`out/innovation-mechanism-focus-20261009/{root-result-acceptance.json,root-capability-and-interface-contract.json,b-positive-design.json,c-mainline-positive-design.json}`。
4. §60—61两个阶段各10completePASS、每流RW256，peer5596/5604被普通resident5596/5596，F128只0.27%差；旧barrier8.7%不能peer独有。`out/shared-read-resident-admission-20261009/codex-result-acceptance.md`。
5. §34 physicalparent1/2/4六RTL对照6PASS且CSV同旧8槽，各末W20213，virtual/AW关联仍8；少2394声明bit非综合/全队列缩减/通用进度。`out/parent-capacity-20261008/{summary.json,stage-evidence.json}`。
6. §32 ordinary准确源gate48B vs8192B四completePASS，CONF0各源AR32/AW32（总128Ax）而旧CONF1总66；gate放行需virtual/physical Wowner/seq同、completionAW成功、偏移长度size真实匹配，并firstVALID held。W末27894vs25146差2748非DONEwindow/纯译址/速度因果。计挡55046是每cycle被挡流数量和可超run长度，不是实际等待。`docs/write-owner-baseline-20261008.md`已全文；`out/write-owner-baseline-20261008/{runs.json,stage-evidence.json,gate-admission-audit.json}`。后继强fixedpost结果由§33记，解除环本身普通已做到。
7. 10/7早期共享pool候选E2（不是旧容量替换E2）13唯一完整scene通过；payload8192→2096B仅配置，singlepool热PN112448较93487慢20.28%且FAST62→0、改分配125677更慢；ordinary2slot103348/FAST31。5SV成功+1CPP诊断，初lint/genmux/AW前后观察fault260为原型错误已修。`out/e2-interface-poc-20261007/{initial-runs.json,reserved-runs.json,acceptance.json,comparison.json,codex-review.md}`。
8. 四翻译长期候选比较`docs/translation-comparison-20261006.md`1—109中MASK一行旧误venue ISCA与后续novelty-gate已更正ASPLOS2018矛盾，按后续关键原文修订处理，不将旧候选排序当前。闭环方法要求固定程序输入而非固定每请求发出时刻，miss/供给/吞吐为可变中介；依赖理想启发式非数学oracle；额外观测净决策收益非分类准确，global最小观测消融未证。

最终应引用的关键链：工程卡死有实板现场与DS接口oldfail/newpass；PLRU局部合成因果与isolated实板miss反事实分开；PN数据返回竞争在行为RTL模型有因果干预、实板尚未全归因；小buffer/conf1写依赖为扩展合同有RTL停滞与普通解；post位置和shared-read弱对照优势随后被强ordinary大幅/精确覆盖；stage79—91仅手推/小Python的多数独有收益关闭或未证，stage93路线暂停保留研究问题不宣称领域无价。所有原始日志/CSV无需逐字读目标已明确，本单元并未抽核这些原始证据，汇总须保留此边界。


---

# 10月5—9日 b 单元审阅（已完成）

证据读法：ledger 中 full_read 指该分配块实际完整进入模型并读完；不表示全文source已完整读完（a单元拥有交错块）。未运行实验或修改远端。每个裸 out 路径须冠 /home/xvhaocheng/work/iommu-platform/。后续直接SSH抽核范围见本节末尾；原始CSV/UART未全核。

## 10月5日：先排工程功能故障，再谈组合性能

HANDOFF 1750—1758、3125—3257详细记录：首次新composition冻结172于第53项停止，52通过/1失败/119未运行；失联没有JSON/boot.after，不能凭boot changed None断言重启。USER4硬件快照显示COMP W阻塞、DDR AW阻塞、PTW空闲，只定位不是根因。真实DDR原子层模型N4096仍通过，不能说实板修复。随后两处历史IOMMU源码问题定位与修复：DS写源排列（63f790a，2025-04-02），PTW合法叶误置ERROR（cc0a4844，2025-01-20，S1 LVL1独立if/else错、S2合法LVL3拒绝）。DS-only N4096成功但172 PN第53条F15，仍52/1/119；合法4K旧错误13/15→新7场景全过，TL过。PTW修复BIT新boot后fresh172全过，不合并旧52项；报告 /home/xvhaocheng/work/iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json。功能通过不等于PN收益。

同日工作流工程：联网更新32MiB SD第一分区、JTAG重载闭环。method88代码归档4,796行，非删除原数据；原有83? 此处仅明确manifest而不编造组件数。综合172284→152609 LUT比较仅Synthesized同阶段；base WNS−0.380需一次post-route优化，不混placed报告。

## 10月6日：清理观测成本、局部PLRU因果与广泛立题

HANDOFF 1538—1711：历史USER4挂起诊断和composition path observer裁剪，V9兼容Linux先部署。模块OOC before14420/9199→after5561/3222，少8859 LUT/5977FF，不是整板差，未含USER4。clean新BIT/cpn3-clean同boot172全部通过、原日志回传，不能用旧ptwfix172代表新BIT。

B1四例为正常fill+单way mask合成驻留、actual RTL vs局部分区PLRU shadow：V touch/Afill victim8/2，touch0 2/2，同拍用旧tree2/2，改后继页收益反向；474次求值+25834默认回归。证明共享替换历史跨容量分区影响victim，尚非合法DMA可达或板上退化归因。

研究立题扩大：18席六方向R1/R2共36正式答复/26原候选，根没验收任何创新；123独立文献、104全文/105版本、19摘要，但根关键审核24不等于104精读。纯metadata replay、生命周期、双视图、动态供给不能因新命名成立；C1历史vs收益、C2缓冲DMA暂停/排空保留假设，ShareMMU锁范围源码取得失败。既有method88 p5强对照4组A增益均负，用户纠正“限速未做”错误后明确不重做。

## 10月7日：隔离修复、模型因果链与共享池尝试

composition-history-isolation分配块1—126、206—326、421—515、607—684、776—814已读（其他块在a）。生产分区localPLRU修改、22板测通过；旧A miss5117/6582→新2048/2048，双方W绝对低约21%，PN仍慢12.67%—12.79%（P约7.035M/PN7.932M）。cross BIT/boot n3描述性，不把miss数量相同当地址/时序/PTW成本相同。报告 /home/xvhaocheng/work/iommu-platform/docs/composition-history-isolation-20261007.md §1—9；验收 out/composition-isolated-20261007/board-acceptance.json。18同boot供给间隔短段板测：gap0 PN V+12.85%/A速度−11.39%；gap1000 +3.73%；gap4000 +0.08%无一致方向，不是修复。

局部TL52场景fixed A叶PTE返回时刻，A最多晚1拍、hot V快4—8拍，无漏重复，不能把局部拍数当板级上界。真实2iDMA＋behavior DDR＋synthetic CPU四模型P/PN91878/93480(+1.744%)；仅跨ID DS优先79847/79484(−0.455%)。PN V数据2763先于A PTE2766，PTE334拍vsP45拍；93 DS地址顺序同，服务差5505等PTWactive差。证明本模型可达服务反馈链，不定量解释板12.74%、非MIG部署新方法。报告同§8；out/pn-data-path-20261007/{comparison.json,trace-evidence.json,acceptance.json,p-full-events.csv,pn-full-events.csv}。

Codex逐PTE进展额度与DeepSeek最后级多拍FAST拒绝提案原误判已纠正：AR接受不意味着所有跨ID R不可抢占；p4按事件收费不是按进展补额。21短仿真同条件P91874/N151947/PN93487/逐PTE92362/普通在途92362/整walk91091；普通更强，停止B主方法推荐（非领域全否）。out/pte-progress-poc-20261007/{acceptance.json,comparison.json,codex-review.md}。

冷目的5/5：P92334/PN94427，DS优先80252/79911；一般热源冷目的先行成立，但FAST同job路径未证，不升级C。共享响应池13完整场景：8192B private→2096B payload少74.41%，PN112448更慢、FAST0；调整125677更慢。预约太早/每DMA一未消费AR压流水。完成阶段预约修后FAST恢复但普通无POOL W-prefix48B P92109/PN92374最强，停止共享池贡献；payload排除公共TC/metadata，无面积。最终21矩阵=13旧+修后8，实际29运行，旧FAST标签覆盖错误纠正分类。out/e2-interface-poc-20261007/{acceptance.json,initial-runs.json,reserved-runs.json}，out/completion-reserve-late-20261007/{acceptance.json,runs.json,runs-all-executions.json}。

同bank合法AR延迟64小例：V0/V1挡A0、TC两个AR spill满、PTW闲、WheadA，真实RTLwatchdog；不是板bug或饱和。48B普通准确源准备门也解除，8192B完整FIFO成功。请求头park两槽＋直通127092 vs普通shared3 127147，只一头差55拍不报性能；正常PN同92374。普通选择队列+旁路表达等效，停止创新；(K−1)L未证。out/write-order-boundary-20261007/{runs.json,stage-evidence.json,codex-review.md}；out/header-park-poc-20261007/{runs.json,stage-evidence.json,README.md}。

## 10月8日：自然错位反例与普通准备逐步追平

共享DDR读延迟overlap干预P92109/PN92374→85339/85984，仍慢265/645；fast→source capture2/3拍，关键受AW/W关系；并非全部PN差额因果。4DMA这里只BFS九策略×36=324抽象组合，盲共享H2—4 30/36停滞，普通每bank2槽或当前W-bank预约2槽0/36；非实4DMA性能/定理。out/read-service-boundary-20261008/{runs.json,trace-analysis.json}、out/four-dma-boundary-20261008/{results.json,traces.json,codex-review.md}。

natural32KiB/src0/dst3072真实AW错位使一个2KiB R跨A前后V W段，V尾R堵A PTE；生产C0写容量错误仅out修正、未应用production。普通准确源门CONF0/BUF3两启动全通过vs512，不把解除环作创新。postTL固定1KiB vs动态最长前缀6PASS，动态physical AR64→55但31延迟末W晚553，100延迟整体早68、均值晚31，未稳定独有收益；固定同效解除，关闭动态主贡献。固定父槽1/2/4 vs8 CSV逐字同，减少2394字段bit但非面积。前/后置位置8PASS真实issue→DONE均值25799→18810低27.09%，但TLmiss34/PTW102同；可见容量不等。普通父复用/当前父early只改善0.33%，post首R前已接受六原父，不能据此否定普通lookahead。普通已准备source PA旁路6PASS后V/A均值18975 vspost18824仅0.7958%，原23.95%差缩97.45%；无完整成本、不能说所有151拍原因已归因。合同/运行/验收入口分别out/write-owner-baseline-20261008、posttl-independent-20261008、parent-capacity-20261008、fragment-placement-20261008、parent-translation-reuse-20261008、prepared-source-bypass-20261008的README/stage-evidence/codex-review（旁路为codex-raw-audit.json/analysis.json）。

nested continuation只有分析：15PTE冷nested、77条件数据下界/82普通帧上界不是面积全编码最优。固定链一次数学6450配置/mismatch0，可完成全30read；q≥可保存相邻最大gap iff存在策略，普通checkpoint达下界，停止此独立贡献。7585/3805/11390片几何分类也是数学，不DUT。remaining-use共享cache模型36records E8 gap4 LRU30→ordinary24，但same-info interval/refcount同规则，停止独立原则，非copy速度。

## 10月9日：共享读原型与不断加强普通对照

stage49事件模型初run01/02失败598/143，最终314场景7644checks接受，N16候选/强tile同R28/AW6/214steps，仅AR2/4；非RTLcycles，CONF129 cache属性前提要同对照。

stage51真实adapter两build15run14PASS一quiet失败；普通q256与longread四对CSV同，关闭长读34%贡献。stage58普通WRITE lookahead46PASS，多数F63以上比接力好；stage60/61普通resident准入追平peer8.7%。stage65—69合计156run148PASS/6旧真实死锁/2要求拒绝；Max2 B流水线解释约19%改善；并行补读E160少18.75%读但V慢17.2%，压力慢25.9%，不能拿省流量当性能创新。stage75短补读29run27PASS/2旧route拒绝，严格加入窗口0不否定更宽合同。实际latest stage76—78六build29copyPASS（由相关HANDOFF引用，此b单元尚未直接read对应a块），五强普通许可齐起点共享更快，true late join功能成立仍无创新。

stage79—92主要手推/原文/接口筛选，无新模型/RTL/Linux/build/board。等待链优先、deadline next-owner、closed density grouping、stage形状shared tags、leaf BBM、按许可coalition、longread最大safe-frontier均被具体ordinary覆盖或缺完整可达性/成本。共享行172bit、319/479bit、1228? 1228是10月14别处，不在本单元。L-M overlapping {X,Y}/{Y}最大匹配容量秩，但ordinary合法early-bind+pin复现；L-W集合叶选择3PTE被checkpoint取消+partial cache复现。stage92 bounded judgment后暂停（不全领域否定）；out/higher-model-mechanism-20261009/round05/root-result-acceptance.json；docs/mechanism-design-handoff-20261009-round05.md（待专题块全文）。

## 对旧会议草稿必须纠正

不能还写“只有平台、没有机制证据”；但也不能把clean172、PLRU隔离、功能PoC当论文创新。必须记录10月5两历史功能bug、10月7PLRU工程隔离与剩余退化；模型1.744%与板12.74%分开、DS优先为模型干预。必须加入失败/关闭链：p5 method88、逐PTEB、共享pool、park、动态fragment、固定链checkpoint、remaining-use、共享读省流量与earlybind普通覆盖；不能用不同时期结果合并共同分母。7—9月草稿遗漏这整个强基线与否定演进，单CO1结论也不足概括10月。

## 最终覆盖与后续专题补充

全部分配186块全文实际读完，共401128字符，39个source（ledger实际计算）；没有截断块、未读块0。source全文是否读完必须与a单元联合清单判断，不能以本单元块读完宣布整个对应source读完。HANDOFF63块，MAC1，专题/status61，validation61。validation与status历史重复记录仍逐块读，未据摘要跳过；后续否定/修订保留。

专题原件新增清楚边界：
- innovation-focus-20261006.md顶部明确后来innovation-followup已取得之前503的ShareMMU必要源码，artifact替换计分与论文伪码不同、原读环可由普通返回空间预留解除。因此本报告此前“锁范围源码未取得”只适用于当时阶段，不是最新总判断；请与a对应followup合并。
- research-progress-presentation-ja-20261007.md分配1—283与zh286—356已读。旧老师稿第7页使用10月5旧composition11.58%—12.22%；第10页仍计划隔离PLRU。用于10月19材料必须改为后来isolated22/22、额外miss差消失/剩12.67—12.79%、18gap小板测和2DMA模型反馈。不能继续说“隔离未实施”。
- §37原请求窗口公平对照后27.09%缩至6.706%；窗口2/4配depth1存在6watchdog，普通加slots解除，窗口8原轨迹1slot同效不是通用保证。§38普通future-parent缓存hit直通仍进共享TC，只有§39物理旁路真绕忙TC，不能混称。
- §70—74是170run（152完整PASS、8旧watchdog、10实际cutover拒绝），非整夜总数。§72独立A suffix错误依赖已退休V授权导致8死锁修复；原型故障不论文贡献。§76—78 RR指针旧idle前进藏窗口，修按真实R握手进，5truejoin；强普通startup五组更快，17%对弱法不能独有宣称。
- §80Python120完整调度比较，每候选完整ACK曲线至少被一个具体普通control复现，但这不是一套在线万能selector；§82Python18模型事件仅h0静态已准备，不证明IOMMU因果；§84Python6图190state，Ui>Qi一故意死锁不DUT，Ui≤Qi为普通安全容量。
- status-stage93-20261010.md正文标题/日期为10月9stage93，10月10附补充。指出问题由原远端翻译自身成本扩为多DMA干扰合理；后期贡献目标没固定、合同不断扩，不能把每个新toy反例当自然应用。建议有期限固定贡献、2—3可执行对照和正反例而未启动；不是自动选新方法。
- validation.md 1341—1346仍写第一次paper-council“优先接口”，同一paper-council-decision.md已因用户异议修订“性能继续、接口备选”。属未同步的历史摘要；采用最新决定但保留原分歧，不写全体原本一致。

## 本轮直接SSH证据核对（无重跑）

已实际SSH确认并全文读取两份board acceptance：
1. `/home/xvhaocheng/work/iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json`：172 planned/attempted/completed、failed/unrun0、boot2695...、sameboot PN诊断true、source DS/PTW matchtrue、IO/CDC false、performance claimfalse。**dmesg_prefix_preserved=false**，虽new_fault_or_kernel_alerts为空，不能报告前缀保存检查已通过；这是验收原JSON中的边界。
2. `/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/board-acceptance.json`：22/0failed、bootd61...、cross_BIT_boot=true、WNS0.022/WHS0.051、kernel新警告空、end_starts_with_before=true。完整JSON的18 crossBIT百分比已读。

另实际确认存在并抽读顶层关键数值（不是全文JSON或原CSV）：
- `/home/xvhaocheng/work/iommu-platform/out/prepared-source-bypass-20261008/codex-raw-audit.json`：1build/6run/6PASS、均18975/18824、151残差、97.45233676%、cache镜像false/A先19008、未综合。
- `/home/xvhaocheng/work/iommu-platform/out/original-window-20261008/stage-evidence.json`：4build/16run/10PASS/6watchdog、all_dut_runs_completed=false，capacity14/timing2。
- `/home/xvhaocheng/work/iommu-platform/out/shared-read-inflight-join-20261009/execution-ledger.json`：stage78、boardfalse、14运行exit0、5truejoin run01/04/06/10/11；disabled与latepermission CSV字节同。其run/build完整条目未展开。

原始record/log/CSV此次仅沿引用定位，未逐项核验所有原始数值或source/binary hash。此前文档的“raw审计通过”是历史root记录，不是本轮重验；无实验、构建、仿真或板动作。


---

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


---

# 2026-10-13—14 历史审阅单元

本单元119个分块、37个来源均已逐块显示并阅读全文。覆盖根CURRENT_STATUS、HANDOFF中分配的10/13—14完整段落、validation相应段落、33份docs来源（含具名来源/机制/实验报告与计划片段）。覆盖依据是 `ledger-oct_13_14.json`；HANDOFF、validation与计划的其余部分由其他单元负责，本单元不声称它们单文件全文。重复关系：HANDOFF与validation大段同步重复，报告给出独立设计/执行细节；保留重复记录有助于核修订，不应把119块当119项研究。

另外通过已授权 `dell` SSH阅读全文6个out文件：RCS根裁决、32行结果表，BR1根裁决与后继合同，AW run-result和六案G0 README。默认沙箱网络拒绝后，经只读授权通道成功；未改网络/SSH/文件配置，未运行科学测试。该新增读取是派生数值表及执行回执抽核，未独立复算事件或原始字节，不代表重新验证实验。

## 10月13日：应用来源的实质筛选，不是只有CO1

stage111的普通R107恢复已留实板验收（本单元读HANDOFF/validation当日条目）：实际boot370a9dd6…/S0，4×4MiB backing，8真实2MiB leaf；priorout和restore各1769472B/432tasks，总864硬件task、144batch。恢复1728hit/0miss/0walk，冷换出8miss/8walk。24worker-layer/12joint是全恢复后CPU字节核验记账，不是原生attention/collective执行。原pre-NEXT EBUSY为软件idle判定错误，最小修复及一次实板pilot完成，不能解释为硬件平台不可并发。

stage112核vLLM TP2层依赖与24次RowParallel归并，活动前缀每rank12288B。GraphAllReduce持久buffer与旧backing slack可容纳两前缀，未增加leaf。归档的是当前FPGA共享翻译缺口投影：partial输出/算术consumer/真实requester/共享路径没有成立，且源事实未排除稳定映射。不是collective科学阴性。

stage113—115选DeepSpeed v0.16.4 ZeRO-Infinity NVMe tiled CPU-Adam：区别真实p/m/v更新、app pinned池与NVMe按request map/unmap。两AIO句柄不等于两PCI requester，pin不等于持续IOVA。源码推导稳定期AIO `6ΣL_i+8ΣQ_i`，完整成功bytes和拓扑仍缺。stage114原论文0.5T单节点需求约p/m/v 6TB>名义CPU1.5TB，排除该规模全CPU驻留；不能排除固定tile/pool、大页、persistent map、单设备。固定官方小测试只得到256MiB预期AIO合同，测试源码未运行；sub_group_size100并不拆weight成100元素tile。stage115官方NVMe模板无关联run，SC22原日志实际CPU offload，Discussion7447是参数NVMe/optimizer CPU，不能拼成NVMe CPU-Adam实跑。取包goal先active→blocked→观察paused，stage116条件封存；状态改变不计研究贡献。

stage116把来源/接口与成本数值获取拆开，避免要求外部包先含所有本研究实验；仍不准把NVMe改固定DDR copy后宣称保留原按request mapping因果。

stage117第一来源Holoscan 2.7.0双目RoCE：持续MR、GPU数值consumer成立，actual translated共同路径未证。RDMA输入原无host源读，而DEFAULT DDR copy增加翻译源读；现WRITE只有64bit寄存器字，无图像stream payload接口。stage118给needs_named_adaptation，图像投影条件归档，未实施stage119。

stage117-r2 DPDK dmafwd：独占可写RX mbuf可原地改MAC并TX，未证明必须独立copy，静态拒绝基线，不是测得零copy更快。stage117-r3改选同源vm2vm：B预发布buffer/cookie，返回A指针不满足B原地址消费，所以必要搬运合同成立；CPU直接A→B和同owner单DMA是合法强对照，不强制host staging两次copy。

其后同一个三包60/65/1514B、载荷1639B/含header1675B、四span60/65/503/1011B逐关推进：

1. 三结果：packet/descriptor/lifetime软件适配；Dell i9-14900K主机CPU一次生命周期29830ns（交付5128、observer24344、间隙358）；单DMA RTL配置至quiet1227周期，12lookup/11hit。主机ns不能与RTL周期算速度比。旧“功能满足所以性能关闭”后来明确勘误。
2. 用户修关口：一次有界普通性能比较本身就是G0，无需预先证明残余再重复。四案single/parallel排空1161/1601模型周期，真实重叠1333；single完成向量更早且少一引擎。440差额混合冷DDT、供给、数据与翻译，未归因。
3. 逐包软件glue静态关：fence.i等待CPU写buffer/MSHR，不等待全部DMA，不能由全cache flush推全DMA必须静止；跨group16B缓存行闭包无交集，同包header/payload共享行仍需绑定。公开接口缺packet join/acquire/publish/consumer/return。
4. 协议适配：17定向协议测试、同binary single/parallel两例；peer-active消费有见证，但MODEL_UNCACHED_ASSUMPTION，不能称真实cache安全。
5. 真实WT cache端口：3微测试＋2集成例；缺fence会返旧字、正确fence返新字，MSHR/写退休闭合。包1取得与used发布时peer活跃，实际consumer时peer已完成；当时未观察消费重叠，不是全域不可能。
6. 六案G0：真实缓存端口CPU/单DMA/双DMA×即时/整批，均一个新binary各一次。used向量CPU即时[114,392,2051]、单DMA整批[1455,1455,1455]；双DMA即时相较单DMA即时首包早71，后两包晚109，metadata工作与多context流量不同。普通即时控制有真实consumer/peer重叠。完整CPU指令/ALU/软件/应用/FPGA成本null，没有共享翻译损失完整因果证据，未进G1。六案不是原生CVA6 memcpy或原生DPDK实测。

## 10月13—14日：从应用优先改为机制先行的具体失败链

用户随后允许机制→有界合成microbenchmark→GPU/AI真实共享路径→FPGA。不再把实际工作流包作为所有机制探索统一前提，也未恢复旧关闭收益。R1事件证书当前安全动作被Mode7 O(1)和普通current头+准确目的AW表达；R2原序写许可提前准备先保留，再核全生命周期与37成本类别，具名WRITE成功/TL fault范围与普通O-B5S配对1329bit+C_common，独有逻辑动作/成本关闭。1329是字段合计，不是总面积。重要修正：TC内部AW handoff不等外部AW接受，W可在firstVALID锁后、READY前前进；长burst保证应以256>16+2=18和firstVALID锁表述。

F1是NormalTL firstVALID前的两顺序反馈预测。原118bit头缺fullID/LEN/SIZE/RB debt，observer/history/selector/CDC皆付费。对称R/W tau例既无真实独占server/source证据，也被固定O-turn覆盖，停止该例；不关闭F1全族。R/PTE返回cut在生产小buffer源码条件下AB/BA都不能完整结束，不能把另bank先TL称收益；实际四源DEFAULT前缀可达性unknown。

一次PTE-safe被动RTL新前缀4151row、两sole4KiB：双NormalTL VALID、双fresh安全READ、comp/DS coVALID均0。12PTE内部→DDR56、DDR→firstR32、DDR→consume88，后继birth1；208held均DDTW，payload/PTW无held。没有合法ABBA，未扫相位。源码残余关进一步只在空复位/两private512buffer/正常S1/完整DSpriority条件模型域排四因果：queue≤6<16，spill恢复≤8<准备52。这不是真实MIG theorem。

MIG4.2绑定显示已accepted payload读不能让后来PTE读越过，QoS/RD_PRI_REG只读写方向优先；私有DS优先不能当MIG免费接口。short-mn-a1软件只启动两个512buffer源，未启动3buffer源；V固定16但A target2^32在软件观察后STOP，候选可改变A工作量，所以原窗口不能直接证明固定双方收益。firstVALID合流早于TL gate，后来的coVALID不是重选机会。

PTE准备gate只能暂停尚未发downstream firstVALID的payload。局部等待/填空档被earlyPause及a≤g覆盖；R-only依赖式min(a,h)≤g未闭合W/B。完整RWB手算ADMIT/WAIT方向会反转，但具体O-envelope-derived闭式max/add统一覆盖窄域；并非完整F1已发表覆盖。literal current-full-AW两job入口被原W bank排他排除；4bit reservation证书只证明顺序，非AW接受。后续AW源码纠正：upstream默认两槽AW spill，coupled第二AR依赖源侧AW0，不依赖后一级input AW0或另一raw WLAST。

另一次AW自然入口新被动记录确实得到两源accepted coupled/nondeburst、source第二AR提前及B2/A0前驱；B2/A0持续1695拍，A读P活429拍，B物理载荷头L全无效。因此固定冷前缀停止、不补预热/重采，future-AW全族仍开放。完成3190/4145模型周期是原基线，不是候选收益。初次误把super_last当final burst分析已纠正，没有重采。

## A—H、CO1、RCS1、BR1的最新snapshot

A在途多层会合已有Neighborhood-aware直接前作；B/D简单ATC push、descriptor预翻译有披露；C连续segment已有DirectSegments/CoLT，保留完成驱动局部撤销；E共享reply口本身未证HOL，当前已有N；F sketch过近似不是权限；G TFID不替正式domain。H基础new-page credit应作普通强基线。保留的是任务后继/真实buffer释放义务的H/D/G和C局部撤销卡，尚无性能/整体FLOW论文贡献接受。

CO1区别最后翻译用途退休与真实task/buffer退休，可信登记+用途保护+release-path准入；普通OC1-min逐事件表达同动作/escape，配对1228+16E+C_common无独有必要逻辑资源。4任务64B/七key是未运行纸面图；公平token错误、非法相邻DAG均修正并留原稿。关闭仅本版，无性能运行。

RCS1随后是不同规则：完整walk后最多保全剩余完整翻译集合，化简E_q过滤refs；18key可产生RCS换p3、minrefs/pin换a、SJF换b的真实条件模型选择差异。4条件×8固定规则32episode，RCS max/sum=921/3491、923/3484、921/3491、907/3411；同一SJF=920/3478、923/3484、920/3478、900/3408。P局部提前不代表最后任务/双方收益。每episode51copy，R/W各3264B、408R/W、51B、4consumer/release、quiet闭合；这是条件服务模型，不是RTL/MIG/板测。派生32行CSV本轮已实际读，未重算116750原始事件。

BR1是当前最新：normal fill victim按owned义务至下一真实release的传播代价选择。固定DAG/单受影响节点、资源顺序和后续residency不变时StaticCP `F0+max(0,d-slack)`精确覆盖；动态service/order族unknown。新19copy/17key图只有4个相关resident、12outside-H，未有后续fill且同延迟时普通Hpin零影响tie；未给优选反转和完整净gain，0性能条件/模型。名义R/W各1216B、51PTE/408B和128B冷DC是合同算术，非完成实测。

BR1最新直接源码勘误非常重要：生产fullbuffer证书仅normal、页对齐、正好4096B且容量≥4096；64B没有permit。旧RCS1 engine给64B的例外为额外条件能力，不能再称保留native guard。旧数据/报告/engine保留，32结果仅适用该旧条件engine，未修后重跑或取得native性能；旧性能关闭不恢复。

## 关键实验与合同/运行/验收导航

以下统一基址为Dell绝对路径 `/home/xvhaocheng/work/iommu-platform/out/`。每项列出的文件是报告明确导航；除上方6个额外read记录，其余只定位而未在本单元直接读取out原件。父单元如核存在性/全文，应另记。

| 阶段 | 合同/进入 | 实际运行/原始证据 | 验收/分析 |
|---|---|---|---|
| stage113—114来源 | stage113-zero-infinity-admission-20261013/byte-and-resource-contract.json；stage114-c113-native-evidence-20261013/source/config-derived-byte-contract.json | 对本研究无新工作流运行，source/原作者PDF仅来源 | 各目录root-result-acceptance.json |
| stage117三个结果 | stage117-three-results-20261013/common/fixture.json | cpu/run.json；dma/results/run.json；dma/results/delivery.json（同目录） | root/critical-review.json；dma/results/artifact-verification.json |
| stage117四案G0 | stage117-plan-revision-g0-20261013/root/gate-acceptance.json；revised-gates.json | model/results/comparison.json，逐事件/snapshot由README导航 | root-result-acceptance.json |
| stage117协议适配 | stage117-packet-handoff-gate-20261013/adaptation-scope-card.json | stage117-packet-protocol-adapter-20261013/implementation/complete-execution-ledger.json（报告给完整运行导航） | root-result-acceptance.json（同protocol目录） |
| stage117真实cache | stage117-real-cache-handoff-20261013/next-goal-contract.json仅后继；实际进入报告章节 | implementation/costs-and-overlap.json；三运行原件见README | checker/delivery.json；root-result-acceptance.json |
| stage117六案 | stage117-delivery-g0-20261013/comparison-contract.json | implementation/results/{cpu,single,parallel}_{eager,batch}/run.json及CPU/cache/memory CSV；interface_probe/ | checker/summary.json；root-result-acceptance.json；root/goal-complete-receipt.json |
| PTE-safe前缀 | pte-safe-stage-causality-20261014/README.md（报告内冻结供给/服务合同） | 新4151row、12PTE表及失败构建由README定位，未在此猜CSV文件名 | root-result-acceptance.json；root/delivery-verification.json |
| AW被动 | aw-natural-entry-observation-20261014/README.md（原供给/反向还原合同） | model/prefix.csv；model/run-result.json（回执全文read） | root-result-acceptance.json |
| CO1 | completion-obligation-design-20261014/root/full-cost-comparison.json；next-goal-contract.json为后继 | 未性能运行；root设计、control/min-rule.md为分析 | root-result-acceptance.json |
| RCS1 | rcs1-completion-set-20261014/README.md（冻结条件导航） | model/results-table.csv（全文read，派生表） | root/event-verification.json；root-result-acceptance.json（全文read） |
| BR1最新 | blocking-replacement-20261014/root/projection-contract.md；root/mechanism-card.md | 未运行；witness/legal-chain.md为合同；root/fullbuffer-platform-correction.md为源码勘误 | root-result-acceptance.json、next-goal-contract.json（全文read） |

## 给旧私人wiki/会议稿的具体修订点

- 不应以“最新CO1”截止；补RCS32条件模型负结果和BR1入口停止及64B勘误。
- 不应把R107的24layer/12joint写为原生计算/collective执行；它是CPU字节代理。
- 不应把NVMe模板、CPUoffload日志、原论文规模及不同版本测试拼成一次关联原生运行。
- 不应说“平台只能串行/全cache flush必须全DMA停”；有限repair接口、一般tracked并发接口、WT fence.i三者不同。
- stage117有多轮来源转向、适配实现与修订后的G0，不是从AI失败直接跳CO1。早期功能满足不构成并发性能否定；后续四案/六案已测模型结果分别支持有限裁决。
- 六案普通即时发布确有消费重叠，不再沿前一真实cache关“未观察消费重叠”概括所有阶段；71周期局部差额不能全归翻译。
- R2应关闭本版完整逻辑成本扩展，不能沿旧“成本未核”继续描述；1329等组件数不是面积。
- F1/PTE/AW的源码条件、数学手推、一次被动RTL记录与真正候选收益实验分开。MIG accepted read不支持免费DS读重排；private模型能力不能当真实硬件。
- 当前待办是新的明确合法动作/完整资源寿命或有符号动态service差异，不是自动重跑旧图、补预热、扩大压力；首篇方法论文未完成。大学报备案题目本单元未代选。

未读边界：未阅读全文各out source/原作PDF、全部事件CSV、二进制构建输入、原生GPU/NVMe日志和现板身份；不声称独立复验安全/性能。没有读取私人会话、凭据或无关文件，没有远端写入。


---

# 2026-10-15每日增量（保留前文10月14日快照）

本次按正文阶段和具名合同先后梳理，不按mtime；当日早先“未准入/旧计数/A优先”不覆盖后来的接受与用户纠正。原始数据未重验。

| 阶段 | 实际进展 | 证据边界 |
|---|---|---|
| 10月14日基线之后SDH1 | 私有非时序协议原型、32768选择配对、普通O-NBO同动作与构造成本配对；条件两页swap值/流量参考 | 无cache/PTW/DDR/native性能；该版策略关闭，不否定付费接口全部价值 |
| LR1源页归还 | 8功能组/122检查及完整旧义务/owner/失效/排序参考；普通phased/targeted复现 | 付费primitive私有功能模型；ACK后真实净窗口未知 |
| 共享服务控制边界 | currentVALID DS-first/F4只改首次offer；已接受MIG债务不可撤。启动意图到共享PTE可见存在结构窗口 | 共现、正债务、完整任务净收益未证；普通设计未部署/时序接受 |
| MIG观测 | 第6构建产物生成但时序失败，按用户要求paused | WNS−0.391/598setup、0新加载/采集，债务unknown；工程失败非科学阴性 |
| A重审/A1 | ACV两次2×2copy RTL条件周期相同；HERO四案×两程序8次无时序语义通过 | 选择点未出现双头竞争；1412源事件不等RTL/原生时延 |
| A2来源/实施 | 公开enabled Hero谱系补齐；当前SVA私有ABI/driver/CLI/Image等构建与分层检查通过 | 新内核/native DMA0；Hero三hook未实现；13/17旧快照被最终16/20等层级接受取代 |
| A3→A4→A5 | 普通ID能力从最后两输入收紧到一个后发RY；区分外侧R与本地B，固定384job两episode离线依赖切面 | 没应用/编译变体；不是自然RTL窗口、共享竞争、完整时间或四时间留出 |
| 用户方向纠正→六族比较 | 撤A默认排名；比较后D优先/I轻量/A资产保留 | 临时研究安排，非论文新意/收益通过或永久排序 |
| 入口与历史计划归档 | 一总计划一导航；12篇124106B/1039行原样迁移，2symlink与10旧路径移除 | 归档不是新实验。14候选定位当前11存在，3指已删除旧方法 |
| D固定M0来源门 | 原六COPY日志无PN收益侧；quiet/clear/warm边界与公开信息校准；I strict可达但FQ回退 | 不扩固定M0/gap/collector；ixgbe未启用/无当前接入，I not_ready |
| D-SPDK S | 源码完成→同task/buffer补发；N4/Q2 clockless语义原型，8回调次序/6拒绝变异/2错误案例 | 非SPDK/NVMe运行，不带服务时间；S通过不自动构成M性能准入 |
| D-CS1 M | 明示合同后已执行280世界/2240动作/1578轨迹；主4免费Z价值0，H1 1/35由等待冷却解释，付费Z全8不及固定动作 | 合成时间/权重，不是RTL周期或实机服务；首次生成源hash缺失；停止当前菜单不关闭D |

## D的合同—实际运行—验收定位

均以Dell `/home/xvhaocheng/work/iommu-platform/out/`为根：

- `d-service-model-20261015/root/service-contract.json`、`root/information-contract.md`：冻结资源、模式、普通等待、完整O0与付费Z、全生命周期；本轮全文读。
- `d-service-model-20261015/model/execution-receipts.json`、`verification.json`、`replay-verification.json`：生成/增强回放的留存回执，本轮全文读，未执行其中命令。
- `d-service-model-20261015/root/root-result-acceptance.json`、`root/decision-analysis.md`与`analysis/findings.md`：稳定停止范围及精确结果，本轮全文读。`analysis/results.json`仅顶层键，全部世界/事件未读。
- `d-spdk-contract-20261015/root/semantic-prototype-contract.json`、`model/run-receipt.json`、`model/verification.json`：源语义合同和实际回执，本轮全文读；实际/联合部署trace各0。
- `d-source-gate-20261015/i-backend/findings.md`：有条件strict后端、DEFERRED_FLUSH缺项导致FQ回退、IXGBE/config/DTS边界，本轮全文读。
- `mig-pte-debt-observation-20261015/root/goal-pause.json`、`root/final-build6-manifest.json`、目录根`root-result-acceptance.json`：暂停与失败产物身份，本轮全文读，未重新验时序或加载位流。

H1“48/35”与“46/35”分别比较付费Z对完整O0离线下界、对可执行最佳固定动作，不是同一基线的矛盾。1/35仅有限等权信息价值，不是自然分布或旁路收益。

## 后续方法与证据规则

当前按任务选模型强度，撤旧全树Sol/ultra默认；请求配置不冒充实际运行元数据。用户要求优先发展idea、精读最近前作、区分发表覆盖/普通构造/等价实现/离线上界。旧个人研究方法已删除，ARS-Codex按需导航；规范调整不恢复暂停实验。

本轮独占阅读细节见 `evidence/daily-2026-10-15/daily-a-review.md` 和 `daily-archive-review.md`。私人wiki原稿仍未取得；10月19材料应加入上述完整进展，不能沿用“目前BR1”“A默认第一”“D未准入”“A2仅缺上板”或把MIG失败写成债务为0。


---

# 2026-10-16核查续作／截至2026-10-17新增（内容日期顺序）

本节补入10月15日保存后产生的记录；10月16日读取中断、17日恢复，没有把采集日或mtime当研究日期。来源层级与实际阅读范围见本轮reading-ledger；文档审阅不是实验重验。

## 10月15日后续：D从来源存在，转到准备与真实控制边界

问题是上一任务观测的状态能否跨quiet/config/clear/warm用于下一任务P/PN选择。MQSim先发现缺完成，唯一身份链定位mapping-only回调丢弃事务，私有修复后四冻结输入完整完成，但完整排空边界活动GC均0；这不是整个SSD持续状态假设被否定。DRAM九run支持读前史排空后保留开行、warm可保留/改写/洗去；来源事实不等信息价值。

随后四个prepare-only投影（P/PN×两前史）各完成V256/A257；同模式两前史准备后有效DRAM状态、clock及next refresh相同。只结束这对前史的残余观察版本，不声称所有准备清除所有状态。其B桥接为私有WR+16/34条件，不是原生AXI/MIG写完成保证；没有64/64完整动作测量。

真实来源核查又区分：composition每ioctl重新alloc/init/map/warm并最终unmap/free；SPDK callback持续同task/buffer补发，但terminal drain释放资源，不是现成保池quiet→config→resume。故旧D下一任务选择版降优先级，不自动新增adapter/sensor/扫状态；未证明整个D无价值，也未自动提高I/A。

来源：`iommu-platform/out/{d-mqsim-identity,d-dram-boundary,d-dram-action,d-boundary-source}-20261015/root/root-result-acceptance.json`。证据为具名模拟器/准备投影与源码；本轮只读接受及历史新增段，未复跑。

## 10月15日启动、16日交付：投入重排与B1单条带成本

旧两轨迹的合法补交分析没有给当前旧V PTE保护正例：最早软件返回可剩60模型tick，但下一A物理请求前已归零。B1单条带进一步核普通预热机会和一次性翻译结果表示；页集合紧上界14≤16不保证实际替换保留，但来源未给预热后强制驱逐残余。343/390等逻辑位是条件编码，非净面积。结束此证据卡、保留具体残余/成本触发；不认定普通预热已充分或B1全族无效。

来源：`out/research-reprioritize-20261015/root/root-result-acceptance.json`、`out/b1-stripe-cost-20261016/root/root-result-acceptance.json`（本节及下文out均在`/home/xvhaocheng/work/iommu-platform/`下）。源码/旧轨迹分析与纸面算术，不是新性能实验。

## 10月16日：映射组织从接纳阈值到owner、同步与地址预算

从DMA_PMD＋XDP正常来源提出整页转交与copy/短map/持久接纳。累计超额版本按定义即普通O3，有限CPU小计40/30/29/27/25为说明单位，不能写原生净赢。此后V1把源物理page_pool owner、B可撤映射关联和一次TX使用分开；真实frame/全部片段寿命可条件替代额外逐TX token，但普通R/slot/O3同享接口，整合成本仍未知。

后续明确：真正source退出、target关闭、请求接受/owner ACK/unmap/metadata退休分层；cache-only owner回收不保证返槽时限，也不等payload RAM下降。seqcount稳定值表可条件免reader锁，冲突须同writer锁重查；普通方案也可采用，未实现验证。合法streaming交接仍需B整页for_cpu等责任，持久映射句柄/A同步不能替代B可见性。IOVA live、rcache、FQ债务分账；K关联槽不保证普通回退地址供给或界多代FQ债务，IOVA不等物理RAM。

取得的公开参考输入不匹配当前source/B map/CQ/fullCPU合同；匹配输入0不等再用或收益0。各轮均保留条件协议/来源/成本资产，未选论文主线，native CPU/RAM/有效完成收益null；原始素材下载和仓库整理不能计性能贡献。来源：本轮ledger中`out/mapping-*-20261016/root/root-result-acceptance.json`具名字段及HANDOFF新段；未逐篇重核全部Linux源码/前作PDF。

## 10月16日：First-PTE条件模型→成对源码设计→局部RTL裁决

原问题是已接受payload不能被后来PTE追越时，付费早PTW意图能否在首次承诺前保护一次顺序。一次冻结Python条件模型70模拟/82检查：cold完整quiet2066→2060，短gap反慢1、longgap快9、late无效；普通零暴露分段2058更快。同接口if/else等价仅排除策略独占，不证明接口已发表。单位是抽象模型时间。

源码设计保留M-ID同钟f1关联及held/READY/escape责任；普通O-tail-prepared复用legalizer/metadata/TC保持，新增1sticky而非重型新FIFO。随后实际局部RTL六case（base/M/prepared×cold/hot）完成：base=M4481/1749，prepared4519/1794；M arm3514晚于A末源RLAST3507，mask0。故M比主动分段快不等于M有增量，结束这份正常COPY程序收益主张，不调相位救正例。

O同锥局部综合增80SliceLUT/1FF/14CARRY4，M helper9FF/28原语LUT范围不同，不作全成本排名；20ns仅综合后分析，无place/route或板测。共同DS-first/F4不是stock免费能力，行为DDR非MIG。来源：`docs/first-pte-{interface-cost,paired-local}-20261016.md`、`out/first-pte-paired-local-20261016/root/{input-contract,root-result-acceptance,paired-analysis}.json`与`out/mechanism-construction-20261016/model/execution-receipt.json`。

## 10月16日：RECONVERGENCE-1三个具体构造均不立项

借TC状态续行PTW的核心共享FSM/部分walk被GPUMMU2014与Neighborhood2018限制；118bit避免副本不等相对强barrel净省。译后分段/返回预约允许保留一次原翻译，但REALM式fragmenter/credit已表达当前关键动作，未留下独立成本差项。范围旧代回收须先给精确失效、设备完成、分domain计数及VT-d定向drain机会；未取得匹配正常残余，不以driver扩大失效或PR/PW TODO制造需求。

这轮仅源级设计、原文关键章节与旧结果算术，没有三次新机制实验；不强选主线、不启动无辨别力实验或两周开发，也不否定整个领域。p5失败具体根因、实板PN完整根因仍unknown；M0不能倒填它们。原报告的E3“受控因果”措辞同时受本次时序更正限制，原报告未改。来源：`docs/research-reconvergence-20261016.md`、同out根`root-result-acceptance.json`、`references/research-reconvergence-20261016.md`。

## 10月17日：DISCOVERY-REUSE-1取得两项问题事实与普通映射干预

准备合同先未授权，后用户“继续”再实际执行；不是拿旧计划的未运行状态覆盖后来结果。问题发现只需有价值未知、可区分解释和有界取证，不要求先有新机制或保证胜例。一次局部编译、六基础＋两触发诊断，各一个确定性条件；两真实iDMA各64个normal4096 COPY，共1024parent，不是1024次独立实验。P/PN共同承诺前DS-first/F4，原FIFO行为DDR startup31/DS50，M/O/CPU关闭，完整配置、补发、B/DONE与quiet保留。

| A映射／目的环R | P完整T_return | PN完整T_return | 两模式walk | 研究事实 |
|---|---:|---:|---:|---|
| 4KiB／1 | 76331 | 76869 | 4 | PN长约0.70% |
| 4KiB／8 | 90290 | 106570 | 39 | A9页仍重复walk；PN长约18.03% |
| 4KiB／16 | 101334 | 130104 | 67 | 同walk仍PN长约28.39% |
| A专用池2MiB／16 | 75797 | 75797 | 3 | 普通映射本arena已足够；N接受0 |

单位是局部RTL仿真clock steps，非实测FPGA/CPU时间。R8的masked tree-PLRU热源/不等子树解释精确吻合偶数目的各8walk、奇数各1walk；内部victim未直接采样，具体way仍源码推导，invalid-first/LRU未实测。

R16 PN中A j16第二PTE(ID48/occ129)在35006接受、35468才到FIFO头、35499返回；其前V j16两payload ID32/occ127/128在34790/34796已承诺，最后R至35467。队列等待461步有具体前驱，DS-first不能撤已承诺读；不把该局部等待扩为全部可回收净收益。P/PN闭环到达日历不同，供给/读腿/写腿分报。

普通A专用池2MiB诊断保持COPY/物理内容/供给、V4KiB，均3次初始walk与75797完整步；parent CSV字节同，PN仅多两个未接受probe。A私有arena原本均预留2MiB，未测OS分配/不可回收RAM/CPU收益；PN大页还改变N资格，差额不能全叫纯walk节省。诊断前有六条件计数及直接原始链检查，但当时触发JSON原字节/hash未保存，当前trigger在八条件后重生成，此来源限制保留。

裁决：新事实成功，当前归普通组织可解/替换局限，尚无新论文机制。普通替换校准仅为未执行建议，不能称修复已通过。来源：`docs/problem-discovery-{contract,results}-20261017.md`；`out/problem-discovery-run-20261017/{acceptance,diagnostic-acceptance}.json`、八份`runs/*/attempt01/{receipt,result}.json`、`analysis/{replacement-source.md,committed-debt-example.json}`。本轮全部读这些小记录；8MB summary仅顶层比较抽读，没有重读全部parent/events、运行归约器或独立复跑。

## 覆盖与持续边界

本轮不宣称全历史/全仓完整覆盖；原先分配正文完成统计不是原始证据全审。八月日志、早期Barrier原运行身份、私人wiki及大量原始CSV/PDF/源码仍有缺口；按coverage逐项区分全文/增量/抽读/仅索引。仓库同步、文档重构只记工程背景。MIG暂停、I not_ready、A5/A2保留、D与mapping/p5/CS1的具体停止范围保持，原10月15—28日窗口不重启。整理没有修改生产或实验源、恢复goal或授权新硬件动作。
