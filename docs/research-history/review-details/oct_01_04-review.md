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
