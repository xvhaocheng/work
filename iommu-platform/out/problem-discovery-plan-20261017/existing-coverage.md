# 已有输入覆盖与可观测性核对

## Material Passport

- 2026-10-17；问题发现规划的只读证据子任务；输入HEAD由root指定为`4c83ac3`。
- ARS-Codex `experiment-agent / plan`阶段证据附件，不是新实验运行计划或执行授权。
- 本次仅定向阅读合同、既有报告、runner和CSV片段；未运行模型、实验、构建、重解析验收或板测。
- Codex-only；请求Sol/high；实际独立model/effort metadata unknown。
- 独占输出本文件；不改共享状态、代码或历史结果；最终设计由root/用户决定。

## 结论

在本次明确核查的E2、E3-v6、M0、旧70及First-PTE六case中，**没有找到与“batch-OFF重复aligned4096 COPY、源固定、目的固定映射页环轮换、固定供给、双方完整排空”完全相同的地址序列＋供给＋起止＋基线结果**。
这不是全历史不存在证明；新程序的环序列、次数和供给接口尚未冻结，也不能先把相邻旧结果称为完全同参。
可直接复用的是既有结果本身及测量/守恒组件。若最终合同恰好退回First-PTE原单pair、同历史、同策略，则对应旧case必须复用，不能因新名字重跑。
四个地址再用条件×两个基线只是上限；此核查不要求凑满八配置，不安排M开关或相位搜索。

## 已核合同与不等价点

| 既有输入 | 可复用 | 与目的环候选的关键差异；原文入口 |
|---|---|---|
| E2：64项、W0/1/7/8/16、正反顺序、N0/1/8/16/112 | V驻留→A覆盖→V条目消失→quiet后首次miss的实板证据；16项容量/W7-W8有限布局边界；首探口径 | V warm2后A单独执行，A排空再V probe1；A同时轮换源/目的，非固定源；5,000,000周期A阶段和V单探时间均非双方整任务t0。原合同`iommu-platform/docs/research-two-bit-v2.md:13–19,49`；结果`iommu-platform/out/e2-results-review-20261003/RESULTS.md:25–39,47–73,86–98`。 |
| E3-v6：11对/22轮、64B、D0/D128 | A PTW延长传给等待中V hit的局部实板因果；四对增量127/129/129/130保留 | formal每方只一份64B，硬件launcher在A被选后释放V；prime/probe及配对失效/历史有专用合同；不是4096B多copy目的环，不是普通CPU供给。原合同`iommu-platform/E3_CONTROLLED.md:8–10,20–75`；结果`iommu-platform/docs/e3-controlled-results-review.md:35–46,59–78`。 |
| M0：A1/A8，S/P/N/PN及A8 P/PN＋DS-return-priority，共10配置 | 各64 COPY自然完成、262144B/路、128AW/B/路、quiet；原PN/P差额及DS优先消除结果；full-era计数框架 | A源和目的一起转环；自主batch=ON、warm256；t0在双方配置后NEXT前；与batch-OFF每次软件改目的不同。合同`iommu-platform/out/workflow-g0-m0-20261010/baseline-register.json:4–21,22–85`；结果`iommu-platform/docs/workflow-g0-m0-results-20261010.md:7–35`。 |
| 旧70：5固定条件×14固定程序 | 条件suffix模型、相邻失败和同ticket逐事件等价；守恒与完整drain思想 | 直接给定V源512beat/A源256beat已保留、V写owner等suffix状态，前缀768是显式假设非真实MMIO轨迹；R/W/setup共用串行资源；无真实目的环程序。`iommu-platform/out/mechanism-construction-20261016/model/frozen-inputs.json:31–48,84–105`及`model/README.md:5–35`。 |
| First-PTE六case：base/M/prepared × cold/hot | 同一pair的4481/4481/4519及1749/1749/1794；base/M逐已记录事件相等、M零mask；实际分段和backend/selector观测资产 | 每方仅一份batch-OFF COPY，a_pages=1；hot历史只是同策略前一pair自然完成；PN及共同DS-first/F4固定，未覆盖多copy目的环或P/PN比较。原合同`iommu-platform/out/first-pte-paired-local-20261016/root/input-contract.json:49–77`；结果`iommu-platform/docs/first-pte-paired-local-20261016.md:20–26,42–57`。 |

### runner直接核读

- M0当前夹具`iommu-platform/tests/nonblock_actual_dma_test.cpp:2023–2036`明确写`REG_B_CTL=1`。
- 同文件`:2100–2114` warm A为`src=A_BASE+p*4096`、`dst=A_BASE+(a_pages+p)*4096`；`:2327–2341`为A环和V单页配置，full t0在两配置之后。
- 同文件`:2837–2870,2909–2914`证明完整batch自然终结、无STOP，最后quiet；不能把这个供给合同换成batch-OFF而原样搬时间。
- First-PTE `iommu-platform/out/first-pte-paired-local-20261016/runner/paired_dma.cpp:2391–2413`：按A/V配置、A/V触发，轮询双方DONE再quiet；`:2425–2435`要求每路恰好1issue/1done并验证batch关闭。
- 同文件`:2459–2487`冻结a_pages=1、1transfer、DS-return-priority关闭、startup31、PN；hot只自然执行一次同程序至quiet后poison目的。
- 旧70完整工作量虽然守恒，`model/README.md:25–31`也明确其accepted FIFO与无真实背压；不能替代新真实runner可达性。

## 当前paired CSV究竟记录什么

以下简称`P=iommu-platform/out/first-pte-paired-local-20261016`；代码行号以实际组合副本为准。

1. `P/combined/paired_dma.cpp:943–998`在full-era记录issue/done数组、TL请求/响应总数、lookup/hit总数、PTW active总拍及PTE握手等。
2. 同文件`:1005–1053`写`full_dma_issue/done`、所有DDR AR/AW/W/B/R握手、PTW start/end/AR/R、fill、fast响应。
3. `P/root/instrument.py:28–60`追加实际walk start、P1 local/selector first、selector接受/held稳定；`:67–96`追加真正backend接受、本地AR first/accept、本地RLAST、parent done。
4. 真实debug绑定：`P/combined/overlay/tests/nonblock_actual_dma_system_top.sv:1490–1506`，backend为`be_valid&&be_ready`；source drain为读legalizer不busy且无读metadata输出。drain还与C++跟踪的backend-active合用，不能将job FIFO里的任务当源已完成。
5. **不能把legacy可见字段误称paired已记录字段。** `P/combined/paired_dma.cpp:496–504`的`trace_event`只在`win_open && !full_era`落盘；paired在`:2286–2324`打开full-era，因此旧`aw_wait_change`、`ddr_r_first/last`不会由这条路径写CSV。
6. `P/results/base-cold/events.csv:1–10`实际有配置40拍、A issue42/backend44等；`:33–57`有walk与PTE服务链，符合上述两套实际输出，并没有凭注释推测字段。

## 三类原因的可判别边界

| 原因 | 已有CSV能直接回答 | 不能直接回答／所需边界 |
|---|---|---|
| ①IOTLB重读／翻译服务等待 | 全任务lookup/hit/PTE工作量；actual PTW开始/结束；DDR全ID/地址的PTE AR→R；与payload已承诺请求的时间关系 | 没有逐次IOVA＋bank＋hit/miss关联，也无逐lookup的victim/替换身份；不能仅由PTE增多认定“目的环挤掉哪条”或将active拍全部称纯PTW。需同一次有身份的lookup/填充关联，或只报告工作量及时间段，保留原因unknown。 |
| ②已承诺payload债务／回压 | DDR AR的ID/地址/len及每beat R握手可按该夹具FIFO/同ID顺序配对；可数某PTE接受时前序payload还有多少beat；selector first/accept可区分尚未承诺和已承诺 | 只见R握手空档不能分startup、R VALID却不READY、上游空；当前paired不逐拍输出DDR R VALID/READY或mem_q服务分类。full_R事件没有数据值，最终内存校验另证数据；不能把握手空档自动叫回压，更不能把重叠债务全部算关键路径。 |
| ③前端供给／写owner／consumer | cfg/NEXT、issue、backend_accept、本地AR/RLAST、parent DONE、最终B与quiet；可将软件可供给→issue→backend→源返回→完成分段 | `ready[j]=t0或上次done`是重建逻辑就绪，不是真实hardware ready；NEXT返回不是backend接受。无CPU消费者真实动作/释放；没有完整逐拍AW owner/W_SELECT/全部阻塞条件。backend或源结束到DONE的间隔不能独归写owner或consumer。 |

AW flags的真实方程在`P/combined/overlay/tests/nonblock_actual_dma_system_top.sv:1422–1438`：
- bit0为未锁定、已有W open且选路不同；bit1为ID计数或W-open容量条件；bit2为同ID已占其他选路；bit3为demux下游AW不ready。
- 它们是局部布尔条件，可同时成立；legacy代码按低bit优先归类仅是计数惯例。并非全局互斥根因，更非consumer完成证书；当前paired CSV未输出它们。

### 合法已有控制究竟隔离什么

- **E2 quiet后探测**已把驻留影响与并发payload/供给排队分开，但只识别该独立首探，不直接解释新双方整任务。
- **E3 D0/D128**已做同类单PTW响应延长的局部干预；它隔离被延长的翻译服务等待传播，不是新普通COPY环的现成基线，也不授权复活板测。
- **M0 DS-return-priority**保留同ID及held R，在行为DDR模型中可隔离返回选择贡献；结果原文`:23–25`有walk数相同、DS延迟差消失及完整完成同步改善。这项已有因果结论可复用。
- 但M0的返回提升不能作为固定真实MIG合同的“合法已有控制”：`iommu-platform/docs/ddr-service-boundary-20261014.md:7–17`明确MIG已接受读不能被后来PTE外部提升，held R不可替换。不能悄悄作为新目的环第二基线。
- **共同承诺前DS-first/F4**是First-PTE已经实现的合法selector规则，只能选择尚未held/接受的请求；六case都用它，没有stock与该规则的既有配对结果。已承诺债务不受其事后改变。
- **P固定而N开/关（P vs PN）**是现有普通配置，足以给两条执行路径一个明确对照；会改变hit旁路、到达与排队，不单独隔离①/②/③。M0的P/PN结果不能迁移到batch-OFF目的环；若选为第二基线，应将机制归因限制为路径干预，而非宣称纯IOTLB实验。
- **普通O-prepared**确已运行，但同时改变read burst、提前翻译/TC占用和返回相位；可作强普通完整对照，不是只改变owner或payload债务的干净干预。参见`iommu-platform/docs/first-pte-paired-local-20261016.md:34–38,55–57`。
- 因此，当前六case trace不足以用一个现成普通开关同时裁定三类根因。可先报告各阶段/工作量的可观测变化；只有一个预登记控制确实改变指定依赖且完整完成随动，才能升级为对应局部因果。单纯时间重叠仍记相关。

## 未核范围与交接

- 未枚举所有旧out、归档和板测输入；未逐成员重验E2/E3 tar、SHA或运行身份；沿正式验收入口保留既有证据，不重新做全仓hash审计。
- 未重新消费全部六case CSV；只核输出逻辑、关键原始片段与正式逐行配对结果。若最终需要新统计，应登记具体分析及独立输出，不称本次已算。
- 未检查目的环新映射布局、页表级别/大页合并、TL逐条事件接口或新runner补丁；当前固定source/destination环不是已存在的可执行新合同。
- 未核真实应用consumer、物理内存回收或CPU成本；本夹具没有这些消费者语义，不增加虚构收益链。
- 最终冻结前需明确每路copy数/总字节、目的索引函数、是否每copy重配、下一copy何时供给、总起点与quiet终点及两个baseline。只有这些全相同才可直接复用旧时间。
- 可先接受“在所核资产中尚无完全相同结果”，不将它升级为必须运行八配置、已授权新实验或已有新机制证据。
