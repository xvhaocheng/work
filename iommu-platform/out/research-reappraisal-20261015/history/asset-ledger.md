# 历史贡献与可复用资产账本

2026-10-15；Codex独占历史席。只核已存证据，不重跑实验、validator、构建或板操作，不修改生产或全局文档。完整结构化条目、原始字段抽核和root核查入口见[asset-ledger.json](/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/history/asset-ledger.json)。

保留的核心资产是E2有限替换因果、E3局部等待因果、真实batch供给及守恒、M0双方完整完成、P4M/method88强基线、R107全成本普通恢复，以及R2所有权/成本设计。旧关闭只到对应版本与合同；同付费接口策略等价不证明接口已发表或架构无价值。8月逐日日志缺口明确保留。

## 先锁住不可混淆的口径

| 对象 | 可复用事实 | 禁止合并的推断 |
|---|---|---|
| E4 → batch | 前者逐项poll/refill时40共跑零重叠；后者自主batch约99.7%在途重叠 | 不能把E1/E4软件慢化与batch硬件慢化当同一协议下连续归因 |
| E2 / E3 | 有限替换后首次miss / 64B人工延迟的局部等待传播 | 不能合成全部自然4KiB任务损失或缓解贡献 |
| PN / M0 | 旧实板约12.7%退化 / 完整full64模型约4.26%差额被条件DS规则消除 | 不能拿模型代替实板归因或把15%写成FPGA收益 |
| 三种DS控制 | 模型已接受返回服务优先 / 原生入口current-VALID选择 / 新付费意图暂缓首次offer | 不同承诺位置；不能把mem_q.rotate当MIG能力 |
| RCS1 64B | 原engine给额外fullbuffer豁免，负结果只限该engine | 原生证书只认正常页对齐4096B；修正native性能未知 |
| MIG观测 | 第6次最终时序失败，0加载/0采集，goal实际paused | 债务未知，不能记0或科学阴性 |

## 条目与复用方向

表中A指控制位置与完整服务方法，B指有限准备／发布协议；这是复用入口，不是选择或批准候选。

| 编号 | 资产 | 分类 | 研究复用入口 |
|---|---|---|---|
| [H01](#h01) | 多DMA原始扩展与8月日期缺口 | 已证工程事实；日期未证 | 复用四DMA平台入口及追溯方法；A/B均从已有工程继续，不重复搭平台。 |
| [H02](#h02) | E4：逐项软件供给下零AXI重叠 | 已证有界观察；简单硬件归因负证据 | A：复用硬件窗口/供给窗口/完成发现窗口分离，以及先确认真实竞争的判据。 |
| [H03](#h03) | 硬件batch：真实重叠与硬件慢化 | 已证硬件观察 | A：复用自主有限供给、末项真实done冻结、字节/排空/AXI守恒；将无miss数据竞争作为必须保留的对照。 |
| [H04](#h04) | DS三阶段回切与路径分段 | 已证敏感性；全归因未证 | A：以共享服务位置和任务关键路径组织问题；保留数据后段与内部等待两个通道。 |
| [H05](#h05) | E3 v6：A-miss到后来V-hit的局部等待传播 | 已证局部因果 | A：复用明确目标请求身份、触发资格、prefix、插入/截断和局部延迟传播判据。 |
| [H06](#h06) | P4M：同窗双方进度的普通节流基线 | 已证基线取舍 | A：复用后台共同窗口完成字节及carry计量；与双方固定工作量完整完成明确区分。 |
| [H07](#h07) | E2：有限容量替换到首次再用miss | 已证局部因果 | A：复用容量/替换效应与服务等待的因果区分，不以命中率替代完整任务效果。 |
| [H08](#h08) | method88：固定A8实占计费p5无独有额外收益 | 负结果 | A：复用同V保护程度下A同窗进度、控制成本和跨方向验证规则。 |
| [H09](#h09) | P/N/PN实板：组合退化反例 | 负结果；根因未证 | A：保留翻译提前放行可能加剧共享数据/PTE服务竞争的真实反例。 |
| [H10](#h10) | M0：双方完整任务完成及DS模型反证 | 已证评价资产；有限模型负结果 | A：优先复用共同就绪、完整固定工作量、多阶段交付/quiet、时间戳与真实字节事务守恒；无须强套64这个数。 |
| [H11](#h11) | R107：合法普通准备后有限恢复全hit | 已证普通控制成功；有限需求归档 | A：复用合法大页、premap/residency、batch、单DMA强普通控制及实际prepare/priorout/teardown成本。 |
| [H12](#h12) | stage117：cache/consumer/前缀交付六案普通基线 | 已证有限普通适配；特定性能扩展关闭 | A：复用used发布、真实consumer、源/目的owner归还、quiet和完整生命周期分离。 |
| [H13](#h13) | R2：生命周期与具名同预算普通构造 | 已证有条件逻辑等价；该版动作/逻辑成本优势关闭 | B：复用raw/prepare/ready/held/physicalAW/WLAST/B各所有权交接和阶段字段复用。 |
| [H14](#h14) | RCS1：完整集合选择差异保留，原engine性能负结果 | 条件模型负结果；native映射勘误 | B：复用E_q化简、完整剩余lookup集合与数据owner退休分离。 |
| [H15](#h15) | 控制位置/早期意图：结构盲窗成立，净价值未证 | 已证源码结构；真实需求/净价值未证；同接口策略增量未立 | A：复用真实控制owner、firstVALID锁、downstream旧债务和公平计数边界；模型返回重排需换成native合法入口。 |
| [H16](#h16) | MIG观测：时序失败后暂停，债务未知 | 工程失败；观测目标未完成；暂停保留 | A：保留firsteligiblePTE、同沿Rfire先扣、known-empty/reset、fullID/occurrence/job-generation归属的观测合同。 |

<a id="h01"></a>

## H01 多DMA原始扩展与8月日期缺口

**2026-07至09-08｜已证工程事实；日期未证｜保存的Git原文及补丁；无早期性能验收**

7月9日软件增加第二DMA和RUN_ONCE；7月26日DTS已有idma0—3/DID10—13及Pair/Multi。9月8日版本保存四DMA RTL、观测和排空修改，不能把保存时间倒填为8月开发日期。

范围：保留原始提交身份；没有本轮重读Linux或.git。八月逐日日志与早期Barrier/Lockstep运行身份仍缺。

可用于：

- 复用四DMA平台入口及追溯方法；A/B均从已有工程继续，不重复搭平台。
- 在论文历史段区分提出问题、实现入口和实板性能验收。

不可推出：

- 不能称7月已验证四DMA共享性能。
- 缺少8月作者提交不等于8月无研究。
- 不能将旧Barrier总吞吐91.9%—96.3%当V独立完成或HOL因果。

原始资产：

- `docs/research-history/evidence/july-august-git.txt`
- `docs/research-history/evidence/july-selected-patches.txt`

正式验收：无早期实验正式验收；只保留Git实现身份。

本轮证据导航：

- [docs/research-history/evidence/july-august-git.txt:1–19](/home/xvhaocheng/work/docs/research-history/evidence/july-august-git.txt:1)；本轮定向读取
- [docs/research-history/evidence/july-selected-patches.txt:1–150](/home/xvhaocheng/work/docs/research-history/evidence/july-selected-patches.txt:1)；本轮未重读全部补丁，仅核保存入口
- [docs/research-history/review-details/july-august-review.md:13–38](/home/xvhaocheng/work/docs/research-history/review-details/july-august-review.md:13)；本轮定向读取
- [iommu-platform/out/research-progress-from-july-20261014/evidence.json:33–60](/home/xvhaocheng/work/iommu-platform/out/research-progress-from-july-20261014/evidence.json:33)；本轮定向读取

未证/未重验：8月实际开发日期和逐日工作记录未恢复。；早期性能表的命令、版本、原始运行身份不完整。

<a id="h02"></a>

## H02 E4：逐项软件供给下零AXI重叠

**2026-09-16｜已证有界观察；简单硬件归因负证据｜实板回传及服务器正式验收**

105条JSON/86组观测通过；60主样本、40共跑中非零重叠样本为0。V硬件H约56ms基本不变，软件窗口增量99.08%—99.92%落在实际完成到DONE读取段P。

范围：逐项poll/refill供给；V4096×4KiB、预热256，Solo/A1/A8双角色各10次。P含CPU、调度和控制MMIO等。

可用于：

- A：复用硬件窗口/供给窗口/完成发现窗口分离，以及先确认真实竞争的判据。
- B：发布协议改变供给后必须重新定义观察窗口，不能复用不适配的H/P/R合同。

不可推出：

- 零重叠不证明IOMMU绝无缓存历史干扰。
- 不能把P全部称CPU因果。
- 不能把E1约1.73—1.79倍软件慢化直接写成共享硬件HOL。

原始资产：

- `iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/{records,assessment,summary}.json`

正式验收：`iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/server-review.json`

本轮证据导航：

- [iommu-platform/docs/e4-results-review.md:7–32](/home/xvhaocheng/work/iommu-platform/docs/e4-results-review.md:7)；本轮定向读取
- [iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/server-review.json:8–12](/home/xvhaocheng/work/iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/server-review.json:8)；本轮定向读取
- [iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/server-review.json:148–192](/home/xvhaocheng/work/iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/server-review.json:148)；本轮定向读取
- [iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/server-review.json:263–264](/home/xvhaocheng/work/iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/server-review.json:263)；本轮定向读取
- [iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/records.json:1](/home/xvhaocheng/work/iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/records.json:1)；本轮只读结构，未逐条复核；字段：`main/main_observers`

未证/未重验：完整UART本轮未重新解析。；下载哈希、SD回读和完整时钟/CDC/IO签核不在此验收。

<a id="h03"></a>

## H03 硬件batch：真实重叠与硬件慢化

**2026-09-16｜已证硬件观察｜实板有限自主批量供给**

14短+60主+30扩展=104条；主矩阵共跑在途重叠约99.69%—99.81%。V0 Solo/A1/A8硬件中位56.56/107.10/154.99ms，吞吐282.91/149.40/103.24MiB/s。A1双方0miss仍约1.89倍慢化。

范围：每路最多1个执行任务；4096个4KiB copy硬件接续，总gap4095周期。固定角色A16/32/64扩展未同期重采基线。

可用于：

- A：复用自主有限供给、末项真实done冻结、字节/排空/AXI守恒；将无miss数据竞争作为必须保留的对照。
- B：发布后的实际读写压力应由真实事务证明，不能只看软件并行或miss计数。

不可推出：

- 在途重叠不等于同时服务，也不单独证明HOL。
- H覆盖全DMA路径，非IOMMU专属时间。
- Solo到A8总差不能全部归PTW；多数V命中也不能直接证明HOL。

原始资产：

- `iommu-platform/out/mac-batch-short-20260916-111626-run-n8mg7wsz/`
- `iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/`
- `iommu-platform/out/mac-batch-ext-20260916-114036-run-8pjv_djc/`

正式验收：`iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/server-review.json`

本轮证据导航：

- [iommu-platform/docs/batch-results-review.md:7–36](/home/xvhaocheng/work/iommu-platform/docs/batch-results-review.md:7)；本轮定向读取
- [iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/server-review.json:117–145](/home/xvhaocheng/work/iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/server-review.json:117)；本轮定向读取
- [iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/server-review.json:179–188](/home/xvhaocheng/work/iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/server-review.json:179)；本轮定向读取
- [iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/records.json:1](/home/xvhaocheng/work/iommu-platform/out/mac-batch-main-20260916-112954-run-9sa69rhc/records.json:1)；本轮只读60条结构，未全量复核

未证/未重验：主/扩展90性能样本中的2条V miss2保留；本轮不重算分布。；软件包围HPM非原子，A整轮任务数含启动和尾排空。

<a id="h04"></a>

## H04 DS三阶段回切与路径分段

**2026-09-16｜已证敏感性；全归因未证｜实板先后配置及位置观测**

DS50→0→50时V0 A8硬件中位154.99→118.19→154.88ms；Solo/A1基本恢复。v3 A1→A8内部读/写25/11→601.04/268.68周期，写后段也增大。

范围：仅一次三阶段切换，跨启动映射/缓存前史不完全固定；DS是PTW/DDTW/PDTW/CQ等聚合服务。分段sum及相互重叠事务不可相加。

可用于：

- A：以共享服务位置和任务关键路径组织问题；保留数据后段与内部等待两个通道。
- B：准备/发布改变到达节奏后，需计入翻译和载荷之间的反馈。

不可推出：

- 36.7ms不能当纯PTW成本。
- DS延迟敏感性不单独证明HOL。
- 不能把分段sum相加计算总损失百分比或把DS端口等同目标PTE。

原始资产：

- `iommu-platform/out/mac-batch-ds0-20260916-132332-run-ebgxhirr/`
- `iommu-platform/out/mac-batch-ds50-20260916-135544-run-eawziww2/`
- `iommu-platform/out/mac-path-main-20260916-164633-run-vvguros8/`

正式验收：`iommu-platform/out/mac-batch-ds0-20260916-132332-run-ebgxhirr/server-review.json`；`iommu-platform/out/mac-batch-ds50-20260916-135544-run-eawziww2/server-review.json`；`iommu-platform/out/mac-path-main-20260916-164633-run-vvguros8/server-review.json`

本轮证据导航：

- [iommu-platform/docs/ds0-batch-results-review.md:15–31](/home/xvhaocheng/work/iommu-platform/docs/ds0-batch-results-review.md:15)；本轮定向读取
- [iommu-platform/docs/ds50-retest-results-review.md:13–27](/home/xvhaocheng/work/iommu-platform/docs/ds50-retest-results-review.md:13)；本轮定向读取
- [iommu-platform/docs/path-results-review.md:17–41](/home/xvhaocheng/work/iommu-platform/docs/path-results-review.md:17)；本轮定向读取

未证/未重验：本轮读正式报告并抽核三份原验收元数据，未重审三目录全部raw。；完整自然工作负载的因果分解未完成。

补充正式验收抽核：[DS0:6](/home/xvhaocheng/work/iommu-platform/out/mac-batch-ds0-20260916-132332-run-ebgxhirr/server-review.json:6)、[DS50:6](/home/xvhaocheng/work/iommu-platform/out/mac-batch-ds50-20260916-135544-run-eawziww2/server-review.json:6)、[path:5](/home/xvhaocheng/work/iommu-platform/out/mac-path-main-20260916-164633-run-vvguros8/server-review.json:5)。本轮已直接抽核其passed/样本数/原输入hash元数据；未重审全部raw。

<a id="h05"></a>

## H05 E3 v6：A-miss到后来V-hit的局部等待传播

**2026-09-18｜已证局部因果｜受控64B人工延迟实板诊断**

11对/22轮、418唯一UART行，9项checks通过。4个cold互补对A剩余服务与V剩余等待增量同为127/129/129/130周期。原raw pair9 seq17/18给出A剩余272→401、V剩余281→410，V自身4→4。

范围：单次启动、64B有限诊断；D128人工响应延长。只有pair9触发前两项时间均匹配，其他3对差异保留；hot为未触发对照，没有实际+128暴露。

可用于：

- A：复用明确目标请求身份、触发资格、prefix、插入/截断和局部延迟传播判据。
- B：翻译服务与结果/数据owner分开，真实首查hit不能提前写成排队时已知hit。

不可推出：

- 不能解释全部自然4KiB A8慢化。
- 不是所有IOMMU或AXI规范强制HOL；不是方法收益或新颖性。
- 原v4/v5测量通过不等于只差D因果通过；分析脚本草稿失败不等于板测失败。

原始资产：

- `iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/all-original-pairs.json`
- `iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/console.raw.log`

正式验收：`iommu-platform/out/e3-controlled-review-20260918/server-review.json`

本轮证据导航：

- [iommu-platform/docs/e3-controlled-results-review.md:33–78](/home/xvhaocheng/work/iommu-platform/docs/e3-controlled-results-review.md:33)；本轮定向读取
- [iommu-platform/out/e3-controlled-review-20260918/server-review.json:38–46](/home/xvhaocheng/work/iommu-platform/out/e3-controlled-review-20260918/server-review.json:38)；本轮定向读取
- [iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/all-original-pairs.json:1](/home/xvhaocheng/work/iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/all-original-pairs.json:1)；本轮直接抽读pair9 seq17/18及e3_words；字段：`[8].rounds[0:2].e3_words[4,39,41,69,70,71,73]`
- [docs/research-history/review-details/september-review.md:51–57](/home/xvhaocheng/work/docs/research-history/review-details/september-review.md:51)；本轮定向读取

未证/未重验：本轮未逐对复核其余10对或重新解析UART；保留正式原验收。；时序/CDC/IO及下载哈希边界沿用原报告。

<a id="h06"></a>

## H06 P4M：同窗双方进度的普通节流基线

**2026-09-20至10-02｜已证基线取舍｜跨采集段实板60份描述性结果**

60份补齐：gap0/1000/2000各20，53在线通过、7离线追认；来源25+6+29。V中位108.813/150.603/171.389MiB/s，A同窗108.813/75.301/57.088MiB/s，改善V要付A进度成本。

范围：前31与新启动29分层；统计单位是run。旧9月“待35”已被10月完成取代；在线/离线身份保留，不改写退出码。

可用于：

- A：复用后台共同窗口完成字节及carry计量；与双方固定工作量完整完成明确区分。
- B：许可/发布延期的机会成本必须包含后台进度，而非只展示前台变快。

不可推出：

- 不是同一次启动60次在线通过。
- 固定gap不证明最优、统计显著性、新颖性或普适因果。
- 整轮A/HPM不能除V窗冒充A同窗速率。

原始资产：

- `iommu-platform/out/p4m-formal60-recovery-20260920/evidence/p4m-formal60-215406/`
- `iommu-platform/out/p4m-complete-review-20261002/retained6/`
- `iommu-platform/out/mac-p4m-remaining29-20261002/collection/unpacked/`

正式验收：`iommu-platform/out/p4m-complete-review-20261002/summary.json`

本轮证据导航：

- [iommu-platform/out/p4m-complete-review-20261002/RESULTS.md:10–33](/home/xvhaocheng/work/iommu-platform/out/p4m-complete-review-20261002/RESULTS.md:10)；本轮定向读取
- [iommu-platform/out/p4m-complete-review-20261002/summary.json:1930–1960](/home/xvhaocheng/work/iommu-platform/out/p4m-complete-review-20261002/summary.json:1930)；本轮直接抽核sample_counts及输入目录
- [docs/research-history/review-details/september-review.md:61–65](/home/xvhaocheng/work/docs/research-history/review-details/september-review.md:61)；本轮定向读取

未证/未重验：本轮未重验缺失的原始exit或全60原JSON；沿用既有审计身份。；gap500 pilot非单调反例须保留，不以最终三gap覆盖全部策略。

<a id="h07"></a>

## H07 E2：有限容量替换到首次再用miss

**2026-10-03｜已证局部因果｜实板64项及完整历史重放验收**

accepted-summary为64 clean、0失败/缺失；16项IOTLB中V2项，A W7=14项时保留，W8=16项时替换。W7/W8正向各5条总探测中位696/1483周期，probe miss0/2；warm驻留→A更新替换→探前消失→首次再用miss链条闭合。

范围：A已完成/排空后才V一次4KiB探测，容量效应与正在共跑排队隔离。固定布局/访问方式，同boot工程重复，128 target不是128独立试验。

可用于：

- A：复用容量/替换效应与服务等待的因果区分，不以命中率替代完整任务效果。
- B：准备翻译集合及其寿命应有真实驻留/替换证据；速率限制不自动等于驻留保护。

不可推出：

- 不能推出稳态吞吐全归因、所有替换策略或缓解收益。
- 不能把A固定100ms阶段当替换延迟，或将D89当纯PTW计算。
- 单凭E2现象不能主张方法创新。

原始资产：

- `iommu-platform/out/mac-research-two-bit-20261003/e2/runs/`

正式验收：`iommu-platform/out/e2-results-review-20261003/accepted-summary.json`

本轮证据导航：

- [iommu-platform/out/e2-results-review-20261003/RESULTS.md:7–25](/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/RESULTS.md:7)；本轮定向读取
- [iommu-platform/out/e2-results-review-20261003/RESULTS.md:86–98](/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/RESULTS.md:86)；本轮定向读取
- [iommu-platform/out/e2-results-review-20261003/accepted-summary.json:2–48](/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/accepted-summary.json:2)；本轮定向读取
- [iommu-platform/out/e2-results-review-20261003/accepted-summary.json:259–310](/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/accepted-summary.json:259)；本轮定向读取
- [iommu-platform/out/e2-results-review-20261003/accepted-summary.json:467–519](/home/xvhaocheng/work/iommu-platform/out/e2-results-review-20261003/accepted-summary.json:467)；本轮定向读取
- [iommu-platform/out/mac-research-two-bit-20261003/e2/runs/e2-boundary-w8-forward-n112-r1/record.json:1](/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/e2/runs/e2-boundary-w8-forward-n112-r1/record.json:1)；本轮直接抽核input/phase_stats/status；字段：`input; phase_stats[2]: completed1,4096B,miss2,S1PTW2,quarantined0`

未证/未重验：本轮只抽核代表record与正式验收，不重跑1226文件哈希、重放器或board验证。

<a id="h08"></a>

## H08 method88：固定A8实占计费p5无独有额外收益

**2026-10-04｜负结果｜冻结强基线实板比较**

88/88完成，84long+4short；两启动分报。四启动×方向格p5相对保护匹配强基线增益为−5.68952%、−8.23689%、−5.15142%、−4.43680%；关闭该版固定A8/target170独有额外收益主张。

范围：每格n=3工程重复；boot1已有462历史，boot2新启动。强对照按冻结规则从p1–p4中选保护匹配A进度最高者，是逐格包络，不是已部署在线全能程序。

可用于：

- A：复用同V保护程度下A同窗进度、控制成本和跨方向验证规则。
- B：相同接口新增复杂策略应与明确普通程序比较，不能只胜弱fixedgap或单个p6。

不可推出：

- 不证明所有占用/翻译感知控制无用。
- p5反向胜p6不能替代对强基线独有收益，也不证明跨方向普遍收益。
- E2E含IDLE stale DID不能当真实服务分母，E25不是ADMIT stall。

原始资产：

- `iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/`

正式验收：`iommu-platform/out/method88-analysis-20261004-01/results.json`

本轮证据导航：

- [iommu-platform/out/method88-execution-20261004/RESULTS.md:17–38](/home/xvhaocheng/work/iommu-platform/out/method88-execution-20261004/RESULTS.md:17)；本轮定向读取
- [iommu-platform/out/method88-analysis-20261004-01/report.zh.txt:7–58](/home/xvhaocheng/work/iommu-platform/out/method88-analysis-20261004-01/report.zh.txt:7)；本轮定向读取
- [iommu-platform/out/method88-analysis-20261004-01/report.zh.txt:156–163](/home/xvhaocheng/work/iommu-platform/out/method88-analysis-20261004-01/report.zh.txt:156)；本轮定向读取
- [iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/runs/m88-b1-r1-p5-f/record.json:1](/home/xvhaocheng/work/iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/runs/m88-b1-r1-p5-f/record.json:1)；本轮直接抽核状态/input/rates；字段：`success=true;mode=cost;A8,warm256,V4096;V173.707163,A_common68.108814MiB/s`

未证/未重验：本轮未独立重解包88归档、重算分组或复验raw哈希。；物理实现费用及一般在线最优不由逐格包络证明。

<a id="h09"></a>

## H09 P/N/PN实板：组合退化反例

**2026-10-07｜负结果；根因未证｜隔离替换历史后实板22项**

隔离后P/PN的A miss差消失，均2048；三对A8/gap0的PN仍比P慢12.6744%—12.7922%。22/22板验收通过；对应优化后实现WNS+0.022ns。

范围：P容量分区，N命中旁路，PN组合。n=3；旧/新BIT及boot前后比较仍有混杂，不能将绝对改善全归隔离。剩余约0.90M周期未完整板端归因。

可用于：

- A：保留翻译提前放行可能加剧共享数据/PTE服务竞争的真实反例。
- B：准备成功、返回更早与有用任务更快分别裁决；需跟随AW/W/R/B到真实退休。

不可推出：

- 不能把12.7%当新门控可收回的固定收益。
- 同miss数不等于同页面/顺序/时间线。
- 行为模型中的机制链不是旧实板完整归因；本结果不得与M0的4.26%混算。

原始资产：

- `iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/`

正式验收：`iommu-platform/out/composition-isolated-20261007/board-acceptance.json`

本轮证据导航：

- [iommu-platform/docs/composition-history-isolation-20261007.md:3–42](/home/xvhaocheng/work/iommu-platform/docs/composition-history-isolation-20261007.md:3)；本轮定向读取
- [iommu-platform/docs/composition-history-isolation-20261007.md:86–99](/home/xvhaocheng/work/iommu-platform/docs/composition-history-isolation-20261007.md:86)；本轮定向读取
- [iommu-platform/out/composition-isolated-20261007/board-acceptance.json:2–39](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/board-acceptance.json:2)；本轮定向读取
- [iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mp/record.json:20–35](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mp/record.json:20)；本轮抽核合同和成功状态
- [iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mpn/record.json:20–35](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mpn/record.json:20)；本轮抽核合同和成功状态

未证/未重验：完整板端服务因果及IO/CDC签核未闭合；当前板身份不由历史Mac结果推得。

<a id="h10"></a>

## H10 M0：双方完整任务完成及DS模型反证

**2026-10-10｜已证评价资产；有限模型负结果｜真实iDMA RTL联调+行为DDR服务模型**

共同就绪起点、V/A各64次4KiB自然完成，首发等待计费；字节、AW/B、数据、quiet和watchdog验收通过。原FIFO P/PN V=86594/90283，差3689即4.2601%；同DS优先均V76738/A76164/T_return76745。

范围：固定静态S1、S2关、warm256、gap0、CPU关、PTW模型延迟31，每配置确定性单次。DS优先仅为C++已接受读返回服务选择，非native MIG可用能力。

可用于：

- A：优先复用共同就绪、完整固定工作量、多阶段交付/quiet、时间戳与真实字节事务守恒；无须强套64这个数。
- A：服务顺序可能改变关键路径，平均载荷请求更慢也可任务更快。
- B：完整prepare/issue/response及两方完成成本评价模板。

不可推出：

- 不解释旧实板12.7%全部差额。
- 不能宣称15% FPGA收益、DS规则已有综合/面积/Fmax或MIG可返回重排。
- 关闭的是据该P/PN模型差额新增逐copy门控的理由，不是所有应用或整个主问题。

原始资产：

- `iommu-platform/out/workflow-g0-m0-20261010/a8-{p,pn,p-ds,pn-ds}.{log,csv}`
- `iommu-platform/out/workflow-g0-m0-20261010/results.json`

正式验收：`iommu-platform/out/workflow-g0-m0-20261010/m0-acceptance.json`；`iommu-platform/out/workflow-g0-m0-20261010/root-result-acceptance.json`

本轮证据导航：

- [iommu-platform/docs/workflow-g0-m0-results-20261010.md:7–39](/home/xvhaocheng/work/iommu-platform/docs/workflow-g0-m0-results-20261010.md:7)；本轮定向读取
- [iommu-platform/out/workflow-g0-m0-20261010/root-result-acceptance.json:4–59](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/root-result-acceptance.json:4)；本轮定向读取
- [iommu-platform/out/workflow-g0-m0-20261010/m0-acceptance.json:2–49](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/m0-acceptance.json:2)；本轮定向读取
- [iommu-platform/out/workflow-g0-m0-20261010/results-table.csv:7–11](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/results-table.csv:7)；本轮定向读取
- [iommu-platform/out/workflow-g0-m0-20261010/a8-p-ds.log:19–22](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/a8-p-ds.log:19)；本轮直接抽核FULL-BATCH-JSON；字段：`T_V76738,T_A76164;each64issue/done,262144 read/writeB,AW/B128,quiet1`

未证/未重验：CPU-inclusive应用值、物理成本、FPGA收益未测；N/S工作量不同未被该干预解决。

<a id="h11"></a>

## H11 R107：合法普通准备后有限恢复全hit

**2026-10-12｜已证普通控制成功；有限需求归档｜实际f9 FPGA单DMA+全成本收据**

必要r1修复后唯一成功pilot：priorout/restore每phase1,769,472B/432硬件task；48+96=144whole batch全部完成。实际8个2MiB叶、16MiB backing；priorout1728lookup/8walk，驻留restore1728lookup全hit/0walk；全池字节核验与release0/quarantined0。

范围：OPT125m L12/H12/TP2来源投影，三个live槽，固定all-ready、单DMA/DID10；不是原生GPU/vLLM/attention。冷成本在真实priorout支付；同映射驻留不被免费清零。

可用于：

- A：复用合法大页、premap/residency、batch、单DMA强普通控制及实际prepare/priorout/teardown成本。
- B：owner/lifetime、layer-ready代理、全字节/guard、真实释放收据和条件时钟换算资产。

不可推出：

- 不能据此否定所有AI或共享IOMMU残余。
- 零miss不证明hit-path/internal latency全部为0；AR not-ready0也不证明无内部等待。
- 48/96布局不同，2844硬件周期差不是8walk因果。
- 首次EBUSY发生在NEXT前，0 DMA触发，不是科学阴性。

原始资产：

- `iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/live/finite-pilot-result/{receipt.json,uapi-run.bin,uapi-batches.bin,uapi-leaves.bin,pool*.bin}`

正式验收：`iommu-platform/out/r107-ordinary-pilot-20261012/root-result-acceptance.json`

本轮证据导航：

- [iommu-platform/docs/r107-ordinary-pilot-20261012.md:12–28](/home/xvhaocheng/work/iommu-platform/docs/r107-ordinary-pilot-20261012.md:12)；本轮定向读取
- [iommu-platform/docs/r107-ordinary-pilot-20261012.md:45–55](/home/xvhaocheng/work/iommu-platform/docs/r107-ordinary-pilot-20261012.md:45)；本轮定向读取
- [iommu-platform/out/r107-ordinary-pilot-20261012/root-result-acceptance.json:22–72](/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012/root-result-acceptance.json:22)；本轮定向读取
- [iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json:2–36](/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json:2)；本轮定向读取
- [iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/live/finite-pilot-result/receipt.json:1](/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/live/finite-pilot-result/receipt.json:1)；本轮解析run及composition/hpm和四pool摘要；字段：`run.composition[0:2][12:15]=[1728,1720,8]/[1728,1728,0];release0;poolmemcmp=true`

未证/未重验：本轮不重新逐字节读四bin或跑原validator；保留实际验收SHA与receipt。；层ready是CPU代理；原生计算/到达/期限、精确peak RAM和严格SI时钟未证。

<a id="h12"></a>

## H12 stage117：cache/consumer/前缀交付六案普通基线

**2026-10-13｜已证有限普通适配；特定性能扩展关闭｜真实cache端口及共享backing受控模型**

CPU/单DMA/双DMA×即时/整批六案功能、所有权、字节、metadata、consumer和成本绑定接受。即时双DMA首包比即时单DMA早71周期、后两包各晚109周期且多一activeDMA；未确证普通控制外可达可避免翻译损失，不进G1。

范围：三帧60/65/1514B、payload1639B、同owner/队列、单驻留2MiB leaf。CPU是乐观确定性端口前端，非原生CVA6 memcpy；模型周期非FPGA/app时延。

可用于：

- A：复用used发布、真实consumer、源/目的owner归还、quiet和完整生命周期分离。
- B：复用正常TL/CPUcache交接、前缀metadata/fence/供给成本，不将ready当消费或free。

不可推出：

- 自然消费重叠/普通适配不是独立创新。
- used向量覆盖不等于全部资源成本支配。
- 不能把71周期全归翻译；不得称穷尽CPU/DMA混合策略。

原始资产：

- `iommu-platform/out/stage117-delivery-g0-20261013/implementation/results/{cpu,single,parallel}_{eager,batch}/run.json`；CPU/cache/memory总线csv、事件和binary snapshot同目录；[原导航:11](/home/xvhaocheng/work/iommu-platform/out/stage117-delivery-g0-20261013/README.md:11)，本轮未重读六案raw

正式验收：`iommu-platform/out/stage117-delivery-g0-20261013/root-result-acceptance.json`

本轮证据导航：

- [iommu-platform/docs/stage117-delivery-g0-20261013.md:15–48](/home/xvhaocheng/work/iommu-platform/docs/stage117-delivery-g0-20261013.md:15)；本轮定向读取
- [iommu-platform/docs/stage117-delivery-g0-20261013.md:52–62](/home/xvhaocheng/work/iommu-platform/docs/stage117-delivery-g0-20261013.md:52)；本轮定向读取
- [iommu-platform/out/stage117-delivery-g0-20261013/root-result-acceptance.json:74–88](/home/xvhaocheng/work/iommu-platform/out/stage117-delivery-g0-20261013/root-result-acceptance.json:74)；本轮抽核G1/causality/cost边界

未证/未重验：本轮只核正式验收/报告，不重新验证16356 AR。；完整nativeCPU/软件/应用/FPGA成本仍null。

<a id="h13"></a>

## H13 R2：生命周期与具名同预算普通构造

**2026-10-13至14｜已证有条件逻辑等价；该版动作/逻辑成本优势关闭｜源码约束的人工协议及成本比较；未执行prototype**

R2提前准备正常WRITE许可、原序提交，可由具体O-B5S以同状态/端口/调度完成。配对均1329bit请求管理状态+C_common，完整37成本类别图相同，逻辑差0；原位union复用与cell提前释放双方均允许。

范围：有限公平合法INCR64/non-ATOP，ID0/4/8/12、每bank最多1个READ或WRITE owner、正常TL成功/fault、真实供齐W/fair sinks；不含reset/remap/cancel/ATOP/MRIF一般安全。

可用于：

- B：复用raw/prepare/ready/held/physicalAW/WLAST/B各所有权交接和阶段字段复用。
- B：具名O-B5S作为完整同预算普通构造，费用包括存储/读写端口/锁/错误sink/软件公共项。
- A/B：首次VALID承诺、内部spill移交、外部接受必须分端点记录。

不可推出：

- 1329bit不是总模块面积、最小下界或净增量；未知物理费用不等于0。
- 同付费接口逻辑等价不是发表前作覆盖或所有prepare/publish协议无价值。
- 内部AW接受不等于外部physicalAW接受；前轮过强推断已纠正。
- 不能宣称原生四DMA故障完整任务活性或RTL形式证明。

原始资产：

- `iommu-platform/out/r2-ownership-cost-20261013/root/source-evidence.json`
- `iommu-platform/out/r2-ownership-cost-20261013/control/source-manifest.json`
- `iommu-platform/out/r2-ownership-cost-20261013/root/lifecycle-design.json`
- `iommu-platform/out/r2-ownership-cost-20261013/root/full-cost-comparison.json`

正式验收：`iommu-platform/out/r2-ownership-cost-20261013/root-result-acceptance.json`

本轮证据导航：

- [iommu-platform/docs/r2-ownership-cost-20261014.md:3–45](/home/xvhaocheng/work/iommu-platform/docs/r2-ownership-cost-20261014.md:3)；本轮定向读取
- [iommu-platform/docs/r2-ownership-cost-20261014.md:49–82](/home/xvhaocheng/work/iommu-platform/docs/r2-ownership-cost-20261014.md:49)；本轮定向读取
- [iommu-platform/out/r2-ownership-cost-20261013/root-result-acceptance.json:7–27](/home/xvhaocheng/work/iommu-platform/out/r2-ownership-cost-20261013/root-result-acceptance.json:7)；本轮定向读取
- [iommu-platform/out/r2-ownership-cost-20261013/root/full-cost-comparison.json:17–44](/home/xvhaocheng/work/iommu-platform/out/r2-ownership-cost-20261013/root/full-cost-comparison.json:17)；本轮定向读取
- [iommu-platform/out/r2-ownership-cost-20261013/root/lifecycle-design.json:1–125](/home/xvhaocheng/work/iommu-platform/out/r2-ownership-cost-20261013/root/lifecycle-design.json:1)；本轮核结构及报告所列关键交接，非全文逐字段复证

未证/未重验：没有性能序列、RTL/FPGA/物理成本或应用净值；本轮未重新逐条核24/27源摘录。

<a id="h14"></a>

## H14 RCS1：完整集合选择差异保留，原engine性能负结果

**2026-10-14｜条件模型负结果；native映射勘误｜18-key/4-task单lookup单walker条件模型**

E_q精简评分保留，与普通pin/minrefs/SJF确有选择差异；4条件×8规则=32episode中固定同一SJF全等或更优。RCS/SJF最后完成921/920、923/923、921/920、907/900。关闭原engine完整性能优势。

范围：每episode51×64B copy，真实模型数据读写各3264B、51AR/AW/B、408R/W、4consumer/owner释放及quiet。原engine给64B fullbuffer豁免是额外条件能力；生产证书只认正常页对齐4096B，64B没有native permit。

可用于：

- B：复用E_q化简、完整剩余lookup集合与数据owner退休分离。
- A/B：完整完成向量、下一真实用途与早期活路径的反例可用来筛选。
- 复用事件/守恒分析结构，不能把旧32值换名为修正native结果。

不可推出：

- 不能因保全更多完整集合就称任务更快。
- 原负结果不证明全族普通逐动作覆盖或完成感知cache无价值。
- 没有修正后native性能值；不能将条件豁免写成原生能力。
- 总状态/CPU/面积/Fmax/FPGA净收益null，字段组件不构成全成本优势。

原始资产：

- `iommu-platform/out/rcs1-completion-set-20261014/model/*.events.jsonl`
- `iommu-platform/out/rcs1-completion-set-20261014/model/*.result.json`
- `iommu-platform/out/rcs1-completion-set-20261014/model/conditional_engine.py`

正式验收：`iommu-platform/out/rcs1-completion-set-20261014/root-result-acceptance.json`

本轮证据导航：

- [iommu-platform/docs/rcs1-completion-set-20261014.md:1](/home/xvhaocheng/work/iommu-platform/docs/rcs1-completion-set-20261014.md:1)；本轮定向读取
- [iommu-platform/docs/rcs1-completion-set-20261014.md:13–52](/home/xvhaocheng/work/iommu-platform/docs/rcs1-completion-set-20261014.md:13)；本轮定向读取
- [iommu-platform/out/rcs1-completion-set-20261014/root-result-acceptance.json:3–33](/home/xvhaocheng/work/iommu-platform/out/rcs1-completion-set-20261014/root-result-acceptance.json:3)；本轮定向读取
- [iommu-platform/out/rcs1-completion-set-20261014/root-result-acceptance.json:101–208](/home/xvhaocheng/work/iommu-platform/out/rcs1-completion-set-20261014/root-result-acceptance.json:101)；本轮定向读取
- [iommu-platform/out/blocking-replacement-20261014/root/fullbuffer-platform-correction.md:5–11](/home/xvhaocheng/work/iommu-platform/out/blocking-replacement-20261014/root/fullbuffer-platform-correction.md:5)；本轮定向读取

未证/未重验：本轮未重新读116750事件或执行engine/checker。；准确E_q已发表覆盖为null，原局部文献核查不穷尽。；修正native guard后的性能未测，不自动恢复RCS1。

<a id="h15"></a>

## H15 控制位置/早期意图：结构盲窗成立，净价值未证

**2026-10-14至15｜已证源码结构；真实需求/净价值未证；同接口策略增量未立｜历史生成参考及未集成普通程序**

合法新PTW启动先于本地PTE ARVALID，再经DS仲裁/spill才可见；busy可晚于最早启动。普通current-VALID DS-first/F4只能在本owner首次offer前选当前合法头；同新增付费意图接口上的简单门控能表达所述动作。

范围：参考为10月7日composition及生成MIG，非当前板身份。普通设计未集成/编译/时序/板测。F4按四次有竞争的selector/spill输入接受给非DS逃逸，非4cycle/MIG/R完成。

可用于：

- A：复用真实控制owner、firstVALID锁、downstream旧债务和公平计数边界；模型返回重排需换成native合法入口。
- B：围绕合法意图何时产生、传播、绑定owner并赶在firstoffer之前定义有界prepare/publish协议。
- 保存同付费接口普通门控作为策略对照，同时单独审查接口架构和完整费用。

不可推出：

- 结构窗口不等于独立L在窗口首次可选，亦不等于正MIG债务或有用任务损失。
- 同接口策略等价不能证明准确接口已发表或架构无价值；接口缺项也不证明创新。
- 8bit绝对selector编码不是净增成本；DS聚合端口不能全称PTE。
- mem_q.rotate/已接受返回提升不是native MIG能力或已证硬件上界。

原始资产：

- `iommu-platform/out/shared-service-mechanism-admission-20261015/root/source-evidence.json`
- `iommu-platform/out/shared-service-control-boundary-20261015/controls/evidence.json`
- `iommu-platform/out/ddr-service-boundary-20261014/ip-semantics/primary-sources.md`

正式验收：`iommu-platform/out/shared-service-mechanism-admission-20261015/root-result-acceptance.json`

本轮证据导航：

- [iommu-platform/docs/shared-service-mechanism-admission-20261015.md:9–55](/home/xvhaocheng/work/iommu-platform/docs/shared-service-mechanism-admission-20261015.md:9)；本轮定向读取
- [iommu-platform/docs/shared-service-mechanism-admission-20261015.md:59–79](/home/xvhaocheng/work/iommu-platform/docs/shared-service-mechanism-admission-20261015.md:59)；本轮定向读取
- [iommu-platform/out/shared-service-mechanism-admission-20261015/root-result-acceptance.json:28–44](/home/xvhaocheng/work/iommu-platform/out/shared-service-mechanism-admission-20261015/root-result-acceptance.json:28)；本轮定向读取
- [iommu-platform/out/shared-service-mechanism-admission-20261015/root-result-acceptance.json:78–116](/home/xvhaocheng/work/iommu-platform/out/shared-service-mechanism-admission-20261015/root-result-acceptance.json:78)；本轮定向读取
- [iommu-platform/out/shared-service-control-boundary-20261015/controls/ordinary-program.md:13–34](/home/xvhaocheng/work/iommu-platform/out/shared-service-control-boundary-20261015/controls/ordinary-program.md:13)；本轮定向读取
- [iommu-platform/out/shared-service-control-boundary-20261015/controls/ordinary-program.md:115–143](/home/xvhaocheng/work/iommu-platform/out/shared-service-control-boundary-20261015/controls/ordinary-program.md:115)；本轮定向读取

未证/未重验：本历史席未重新审生成MIG全源码；root需按独占源核接受当前参考结论。；合法共现前缀、传播延迟、完整任务R/W/B/consumer/quiet后果和具名普通完整费用仍缺。

<a id="h16"></a>

## H16 MIG观测：时序失败后暂停，债务未知

**2026-10-15｜工程失败；观测目标未完成；暂停保留｜第6次构建工具原报告及goal回执**

第6次Make退出0/生成BIT，但原routed timing WNS−0.391ns、TNS−54.142ns、598setup端点；0新BIT加载、0工作负载、0真实MIG记录。payload_debt_beats=null，goal1792023314实际paused。

范围：50MHz与200MHz均有setup失败；OOC功能/综合通过不能替代全平台时序。本轮不恢复goal或采集器、不第7次构建。

可用于：

- A：保留firsteligiblePTE、同沿Rfire先扣、known-empty/reset、fullID/occurrence/job-generation归属的观测合同。
- B：观测成本目录可供检查协议额外跨域/关联费用，120+29+5组功能资产可复用但不是板证据。

不可推出：

- 没有有效记录不能记债务0、自然竞争不存在、科学阴性或方法失败。
- 正债务一条也不自动证明ordinary不足或完整收益。
- 失败实现资源不是通过时序硬件面积；Make0不等于时序接受。

原始资产：

- `iommu-platform/out/mig-pte-debt-observation-20261015/final-build6/reports/ariane_xilinx_timing_summary_routed.rpt`
- `iommu-platform/out/mig-pte-debt-observation-20261015/root/build-attempt-6.log`
- `iommu-platform/out/mig-pte-debt-observation-20261015/root/final-build6-manifest.json`

正式验收：`iommu-platform/out/mig-pte-debt-observation-20261015/root-result-acceptance.json`

本轮证据导航：

- [iommu-platform/docs/mig-pte-debt-observation-20261015.md:3–38](/home/xvhaocheng/work/iommu-platform/docs/mig-pte-debt-observation-20261015.md:3)；本轮定向读取
- [iommu-platform/out/mig-pte-debt-observation-20261015/root-result-acceptance.json:4–37](/home/xvhaocheng/work/iommu-platform/out/mig-pte-debt-observation-20261015/root-result-acceptance.json:4)；本轮定向读取
- [iommu-platform/out/mig-pte-debt-observation-20261015/final-build6/reports/ariane_xilinx_timing_summary_routed.rpt:153–162](/home/xvhaocheng/work/iommu-platform/out/mig-pte-debt-observation-20261015/final-build6/reports/ariane_xilinx_timing_summary_routed.rpt:153)；本轮直接抽读timing summary
- [iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json:2–9](/home/xvhaocheng/work/iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json:2)；本轮定向读取

未证/未重验：真实MIG债务值、实际新BIT身份/原生功能、完整任务后果均未获得。；后续工程估计0.5—1工作日不是已获授权执行或下次成功保证。

## 给root的重点交接

- **R-A1 选择A控制位置/完整服务方法时，核历史模型服务律与native端点能力不等价**（H10, H15）：root按现有源核席核首次offer/held/spill/CDC/MIG；勿将M0返回重排直接实现为MIG接口。
- **R-B1 选择B有界prepare/publish时，排除已关闭R2同预算重命名并保留接口架构问题**（H13, H15）：root核O-B5S完整字段/端口与新协议实际新增能力；等价只限具名合同，不自动关闭所有协议。
- **R-B2 小copy合法read准入**（H14）：root亲读idma_fullbuffer_certificate.sv与调用链；64B不能继承4096B permit。
- **R-C1 历史正证据不越界**（H05, H07, H09, H10, H11）：可直接导航正式验收和本轮raw抽核；仅对新主张必需的原字段再核，不重跑旧矩阵。
- **R-C2 暂停边界**（H16）：保留实际pause回执；重审授权不自动恢复MIG观测。

本轮只重新组织并抽核历史证据，不声称全量重验；旧DeepSeek/Claude历史作者归因沿用原报告，不追溯改写为本轮Codex。当前两周立题、论文未完成及原暂停/关闭身份保持。
