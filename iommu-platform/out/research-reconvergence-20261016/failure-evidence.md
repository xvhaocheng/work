# 历史负结果：原始证据与机制边界核查

2026-10-16。独占任务：重新立题的历史根因核查；不提出新机制、不修改共享状态、不恢复任何旧实验或硬件。工作区基准 `main c981e01`（本席读取时 HEAD 一致）。建议／请求配置 Sol/high；可取得的实际运行模型与强度元数据均为 **unknown**，不以请求值代证。Codex-only；未调用 DeepSeek、Claude 或其他子席。

使用 ARS-Codex deep-research/fact-check 来源核查纪律。本文的“已核”指本席实际读到的文件／字段和必要只读算术，不是全部历史重新认证。历史材料自身的模型归因原样尊重；本轮全部核查由 Codex 完成。没有联网、模型执行、仿真、构建、板测或旧结果覆盖。First-PTE 固定程序已经关闭，本席未重读并重算其运行，仅遵守暂停边界。

## 1. 判定口径与结论地图

证据级别按本任务区分：**S** 源码／接口事实；**B** 具名版本实板原始记录；**R** 实际 DMA/翻译 RTL＋声明行为环境；**C** 声明假设的合成事件模型／费用账；**D** 从这些事实推出的有界逻辑；**U** 未证或未知。B/R/C不是同一性能总体，不能沿时间顺序拼成一条原生因果证明。

| 分支 | 真正被否定／关闭的主张 | 已证原因 | 未证而不能补成原因 |
| --- | --- | --- | --- |
| p5 | 固定 A8/target170 的实际占用计费，相比保护匹配的最强 p1–p4，具有稳定额外 A 进度 | B：两启动×两方向四格 A 组均值增益均负；S：真实计费和准入规则可核 | 为什么输：信息冗余、反馈迟、粒度粗、误计费或参数欠佳中哪项主导，均未识别 |
| P/PN isolated | 加命中旁路应在这份实板工作集上直接改善 P | B：等 A miss 数仍慢约12.67%–12.79%；S：隔离替换历史的确切范围可核 | 约0.90M板级周期差的完整根因仍未完成；不能由M0倒填 |
| M0 | 必须新增逐COPY门控才能收回本合同 PN–P 完成差额 | R：普通模型 DS 返回优先已使两路完整完成与收尾相同 | 相同优先在MIG的可实施性、全硬件成本、旧板级差额是否同源 |
| D-CS1 | 该版 Z 摘要及2单位读／决策费用，能改善冻结菜单的完整目标成本 | C/D：主配置免费信息上界0；八配置付费信息成本下界均输可执行固定动作；特定H1反号由等时间等待复现 | 真实SPDK设备中该持续状态／分布／费用存在性；不是D方向整体否决 |
| mapping阈值／池化 | 累计超额接纳是独立新控制器；27 CPU小计已构成完整净赢／天然节省payload RAM | S/C/D：规则就是O3；未测差项可能吃完2或3单位差；stock也无idle B pin | 原生再用频率、完整CPU、内存净差与完成服务，均unknown/null；没有实测性能负例 |
| R107 | 当前all-ready有限KV恢复卡在合法普通大页／驻留／批量之后仍有具名可避免翻译残余 | B：恢复1728 lookup全hit、0 walk，完整字节／层依赖／回收通过 | 原生GPU多任务需求、hit固定延迟、其他动态部署的残余；没有证明所有AI翻译需求消失 |

## 2. p5：输给保护匹配的简单计费；失败机制尚未识别

### 2.1 直接数值与原始记录

正式结果入口：`iommu-platform/out/method88-execution-20261004/RESULTS.md:17–38`。分析输出：`iommu-platform/out/method88-analysis-20261004-01/report.zh.txt:21–58`。本席进一步读取原始 `metrics.json`，对每格3轮的 A MiB/s取组均值，计算 `mean(A_p5)/mean(A_control)-1`，重得如下数值；这里只读算术，未执行旧分析器或任何板端程序。

原始前缀 **P5RAW** = `iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/runs/`。每个文件 `metrics.json:4` 为 A MiB/s；`v_mib_s`为同文件尾部键。每格所有3个文件均实际读取：

| boot/方向 | p5轮次A向量 | 强对照轮次A向量 | 重算相对增益 |
| --- | --- | --- | ---: |
| 1/f | r1/r2/r3：68.1088142923,73.1682729361,71.9107897205 | p4：69.4252155603,79.4872911936,77.1364729165 | −5.6895203598% |
| 1/r | r1/r2/r3：74.2331353151,77.0844152435,68.7963348789 | p4：82.6877517170,82.5672627746,74.6168510216 | −8.2368893214% |
| 2/f | r4/r5/r6：78.3491363145,71.2213041415,75.4765130268 | p1：83.5770484641,74.1513209558,79.5413530192 | −5.1514238019% |
| 2/r | r4/r5/r6：74.8422411669,71.4758305270,69.4267440294 | p2：70.3090554785,81.6644067669,73.7879463213 | −4.4368047254% |

文件路径规则例如 `P5RAW/m88-b1-r1-p5-f/metrics.json:4` 与 `P5RAW/m88-b1-r1-p4-f/metrics.json:4`；其余按上表boot、轮、策略、方向逐字替换。四格p5／对照的V中位分别为173.46678/172.65082、172.81761/173.52258、173.87615/170.75544、172.21471/170.08184。仅这些中位不足以自行证明全部保护匹配；完整缺额条件采用保存分析报告与冻结规则，不伪称本席重新验完88个raw record。

强基线并非任挑弱对照：`platform-archives/retired-code-20261005/iommu-platform/scripts/research_method88.py:710–728`只在完整3配对、protection_matched 的 p1–p4 中选A均值最高者；`:1073–1080`保存同轮、V中位5%、双方≥170、E23/E20缺额组均值差≤10百分点、每run完整窗口>0的规则。实际88/88、84 long/4 short、第一boot有既往462项历史及第二boot为新启动，见分析报告`:7–19`；不是两个全新启动。n=3工程重复，不作显著性推断。

### 2.2 已直接核查的控制规则

生产路径 `iommu-platform/cva6/corev_apu/fpga/src/dma_research_policy.sv`：

- `:79–84,318–330`：p1按A issue字节、p2按select、p3按miss、p4按hit/miss/PTW权重、p5按实际REQ_TRANS/AWAIT_RESP周期计费。p1–p4不是全部“固定gap”；p6才是V-only反馈gap。
- `:284–294`：p5以A DID及REQ_TRANS/AWAIT_RESP状态限定，COMPLETE不计；A-owner无状态过滤的E2E是另一事件。
- `:333–345,352–360`：在运行段计费，固定窗口补b，正余额封顶、债务不清零；`:381–405`用V完成字节的窗口反馈调A budget；`:596–610`以计费后的余额决定新准入许可。这个控制是**准入与后续窗口调整**，不是已接受事务的撤销。
- `iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:674–701`：REQ_TRANS发请求，AWAIT_RESP一直等翻译响应。故不能把“p5未计DDT/PDT/PTW内部响应等待”写成已证原因。
- 原始 `P5RAW/m88-b1-r1-p5-f/metrics.json:18,24`的 `a_req_await_cycles=charged_cycles=1455152`；`:16`的owner cycles为14004369，是不同口径；`:33`permission_closed也不是有前端wait证实的ADMIT阻塞。源码`:46–49`明确无a_wait输入。

### 2.3 根因范围

**已证**：细粒度实际占用信号＋此反馈／准入器，在这份A8/target170合同中不足以转换为相对保护匹配简单计费的额外A进度。**未证**：究竟是信号对最优决定无额外信息、观测发生太迟、4KiB动作太粗、预算／窗口参数不佳，还是多因素。源码揭示可能的控制边界，不能单独证明哪一边界导致四格负收益。COMPLETE占比仅0.380313%–0.851637%、每COPY均值4cycle的既有辅助提取见正式报告`:61–79`，本席未重提取全部fullstats；不能凭该小比例宣布它是失败根因。

准确不足类别：**结果增量不足已证；信息／有效窗口不足的具体因果未证；接口／面积成本未作本轮定量裁决。** 反向p5对p6约+34.68%/+30.99%可成立，却不足以建立“实际占用计费独有价值”；强p1–p4仍胜，见正式报告`:40–59,98–102`。不否定全部反馈控制或所有实际计费。

## 3. P/PN与M0：缓存计数、服务顺序、硬件承诺是三层问题

### 3.1 PLRU隔离后的实板事实

报告 `iommu-platform/docs/composition-history-isolation-20261007.md:23–42` 给出P/PN A miss均2048、PN仍慢12.6744%–12.7922%。本席实际读取两个原始记录：

- `iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mp/record.json:148–176`：raw[8]=7032355、raw[17]=2048；V普通请求16384。
- 同目录 `explore-b1-g0-a8-mpn/record.json:148–176`：raw[8]=7931945、raw[17]=2048；V普通12288，raw[21]快速4096。两者V总请求相同且V miss为0，但不能推出A请求页／时序／全部PTW费用相同。

对应原始生产patch `iommu-platform/out/composition-isolated-20261007/production.patch:19–40,45–61,67–72` 实际为固定16项、ways0/1对2..15的partition-local PLRU touch，touch归属来自resident way；不是“关闭N”或任意mask通用隔离。隔离去掉的是共享替换历史造成的额外miss数差，**没有消除实际服务竞争**。

剩余约896481.67周期被定位在V硬件执行、同时pending窗口，补充／软件轮询差为0；AW not-ready增量863488与这些窗口重叠，不能相加或当根因，报告`:68–79`。该报告`:60–66`明确未发现能解释此差的具体bug，局部TL相位测试不构成整DMA/DDR上界。

### 3.2 M0真正测到什么

`iommu-platform/out/workflow-g0-m0-20261010/results-table.csv:7,9–11`与 `results.json:2418,3376,3855,4334`直接核得：

| A8模式 | T_V | T_A | T_return | walk活跃周期 |
| --- | ---: | ---: | ---: | ---: |
| P FIFO |86594|86082|86601|16483|
| PN FIFO |90283|89771|90292|20172|
| P DS优先 |76738|76164|76745|13485|
| PN DS优先 |76738|76164|76745|13485|

原FIFO的PN–P为3689周期（4.26%），完整双方自然完成，不截A。每路64次4KiB，真实读写各262144B、128AW/B、数据／quiet通过；CPU背景关闭、warm256、DS50/50、行为DDR startup31。这与旧板测V窗口不是同一合同（`iommu-platform/docs/workflow-g0-m0-results-20261010.md:7,23–39`）。

独立事件匹配统计 `iommu-platform/out/workflow-g0-m0-20261010/trace-statistics.json:426–437,712–723,814–825,916–927`：每案93个DS读，AR→R累计5974、9663、2976、2976；P/PN原差同为3689。DS优先后每个32。原始表A1四模式全同、walk0见`results-table.csv:2–5`，是同规则的相邻无walk控制。有限模型内支持一个因果机制：**早放V payload改变跨ID返回队列竞争，拖长关键walk；普通优先顺序能收回差额。**

### 3.3 合法窗口与外推限制

`iommu-platform/tests/nonblock_actual_dma_test.cpp:679–698`直接表明：只要`!ddr_r_held`，在t0至V最终done窗口，把最老DS request旋到C++ `mem_q`前；保留同ID顺序、请求latency/beat状态及held VALID。它是在**模型已接受AR的跨ID返回队列**上作选择；可执行模型干预不自动等于真实MIG在相同阶段有这种选择能力。`:699–719`随后对队首扣startup或输出R。

因此：M0否定的是“**这份模型差额使新增逐COPY门控成为必要**”，不是旧板差额已经解释／消除，也不是DS优先完整硬件成本为0。已证不足为**普通服务规则覆盖了候选赖以立题的剩余**；实际平台是否缺可用控制窗口／接口及其费用是U。当前源码之后的扩展选项不是历史运行启用事实，原运行仍以保存合同／日志为准。

## 4. D-CS1：信息精度不是价值；等待本身改变服务状态

### 4.1 模型身份与控制规则

`iommu-platform/out/d-service-model-20261015/root/service-contract.json:13–24,75–120,122–154`明确是**合成条件模型**：两owner／有限buffer、mode-blind FCFS设备、假设可跨host quiet保留的h债务、每时间单位冷却1、完成加热16、service=`base_d+floor(h/8)`；coupled/split是新假设，不继承旧M0或实SPDK服务事实。

本席直接核 `model/simulator.py:16–27,43–66,184–193,219–224`：每个动作按自己的等待／PN配置费用冷却h并重新运行完整target，包含allocation/map/clear/warm、callback/free及cleanup；不是固定到达日历或事后从收益减费用。PN多1单位配置与普通P额外wait1能产生相同target输入状态。`analysis/evaluate.py:12–17,85–98,168–213`按完整O0分组，Z额外细分，用exact Fraction计算每个信息组的最佳均值；付费菜单限制wait≥2，2单位读／决策已经在各自轨迹中。

信息边界见 `root/information-contract.md:5–12`：公开时间戳不得删；Z只能前一次invocation摘要，不给未来；普通控制同享Z与等待。O0可识别状态不等于可低成本实现算法，Z的廉价摘要价值仍需另测；本版最强的关闭理由是输给**可执行固定动作**，并不依赖免费O0 oracle。

### 4.2 直接原始世界／事件抽核

`model/worlds.json:1`为单行完整原始世界JSON，使用`config_id`和`prehistory.{k,r}`作精确记录定位。本席读取如下目标成本向量（mode顺序P wait0/1/2/3，再PN wait0/1/2/3）：

- H1,(4,0),h0=51，Z=[6,25,49]：**307,306,307,308；306,307,308,309**。
- H1,(5,12),h0=48，Z=[7,26,50]：**305,306,307,308；306,307,308,309**。

前者P0的boundary4/target188，Pwait1与PN0都boundary5/target186；后者P0为boundary4/target186。完整J还含reference，故不能把target188与186直接当两单位净收益。

本席还解压只读三份已有原始事件JSON，各文件压缩前为单行，定位文件即精确源入口：

| `model/traces/`文件 | response_start且path=fast | device_start work前缀 | target结束t |
| --- | ---: | --- | ---: |
| `73c400523815d81263c7.json.gz`（前者P0） |0|3,4,6,7,8,9,9,10…|188|
| `3ed2a9c848fa7d683fc5.json.gz`（前者Pwait1／后者P0） |0|2,4,6,7,8,9,10,10…|186|
| `1b038d90fd72484b1c4a.json.gz`（前者PN0） |0|2,4,6,7,8,9,10,10…|186|

PN0虽有3次probe_start，却无fast response；不是N成功绕过walk导致该处改善。事件定义`simulator.py:125–154`区分probe与fast response，不能数一个并不存在的事件名。各结尾successful/submitted=[8,8]、live0/free4。Pwait1与PN0的device work向量完全相同；PN探测事件本身不必逐事件相同。

既有root更广抽样 `root/causal-check.json:4–20,182–268,454–541`含6世界18动作，并核四个H1反号世界fast0。本席不声称重新解压了所有1578轨迹；当前抽核与该接受相容。原始生成没有当时完整源码hash，后增checker/current hash不能补历史绑定，见`model/README.md:13–15`、`root/causal-check.json:559–561`。

### 4.3 价值上界与关闭范围

精确结果 `analysis/findings.md:5–22`：4主配置免费Z信息价值全0；H1留出仅1/35，其余留出0。付费Z离线最优成本（可实现成本的下界）减去最佳固定登记动作成本，8配置依次H1=46/35、H2/H3/H4=2、M_coupled_W1=1、W4=3、M_split_rw_W1=1、W4=27/35，全严格正。

**已证**：冻结模型／权重／菜单中，即便给付费Z离线最佳选择，也不足以支付其时间成本；特定H1反号仅需普通等时间冷却便复现，不是N bypass因果。**未证**：真实provider有这样的持久状态、35等权前史或2单位实际费用；不证明所有新信息没用、所有摘要无法更便宜，或D整体不可行。有效不足分两类：主配置**信息对动作选择的额外价值0**；H1**小价值被观测／决策费用吃尽且改善含等待混淆**。

## 5. mapping阈值／池化：规则等价、实际机会和全成本是不同关

### 5.1 阈值并未形成第二个控制器

本席直接读 `iommu-platform/out/mapping-admission-20261016/ledger/fee_ledger.py:13–14,44–107`：key为extent/generation/B/READ；O3用当前冻结F/R与历史a，在`a+R>=F`时建持久图，否则付便宜copy／short map并累计R；只有offline分支`:47–52,82–83`读未来次数。PASS不改变计数／关联，retire才清除。该累计超额候选就是普通O3，不是“与O3实验接近”。`ledger/README.md:15–17`明确本轮未定义另一个独立controller。

原始结果 `ledger/result.json:4–9,41–45,478–482,915–919,1352–1356,1789–1793`：d2/x3/s4/F3/R1，O0/O1/O2/O3/offline小计40/30/29/27/25。脚本`:108–125,182–193`保留K、每次查询h、source布局／保留／集成G、关联／IOVA/PTE／同步／回调L等未知项；不是漏掉后当零。

`ledger/README.md:9–13`：3个对齐PMD，源区1/1/8再用、全部TX接受、前对象返还后才下一RX都是条件fixture；stock order0不保证PMD物理布局。权限READ构造还不同于原RFC整PMD RW。**数值账是CPU工作量说明，非cycle模型、原生CPU或真实流量。**

净CPU条件 `ledger/README.md:29–31`：O3胜O2须`K+10h+ΔG+ΔL<2`，胜O1须`10h+ΔG+ΔL+ΔK<3`。小计差仅是可供残余成本消耗的预算，未测差项足以正反两向改变结论；不能称已经性能失败，也不能称27已净赢。

### 5.2 pool生命周期确有接口工作，不能靠普通化简宣布架构已发表

直接源码：`iommu-platform/out/mapping-admission-20261016/v612-source/raw/drivers/net/ethernet/mellanox/mlx5/core/en/xdp.c:877–885,909–922`在最终SQ接受前做B map，失败unmap；`:673–700` CQ路径unmap后return frame。`v612-source/raw/net/core/page_pool.c:659–698`是真正source退出卸A图、release计账、clear信息和put；`:866–873`只有最后pp引用才进回收。普通completion与source真正退出是不同关口。

因此跨recycle持久B图需要真实source/target关闭联动及对应费用；缺这个接口有具体工程内容，但不自动成立独立研究价值。具名普通R-OB4K-T-N是本项目构造，**不是已发表同名系统或未经改造RFC**。`iommu-platform/out/mapping-integration-delta-20261016/owner/integration-analysis.md:7–11,100–124`给普通R同字段／表示／frame引用及关闭能力后，覆盖目前V1原语；这关闭当前**独有动作／必然较少源侧原语**主张，不证明全成本实测相同或所有组织已发表。

库存界与资源后果：`iommu-platform/out/mapping-integration-delta-20261016/root/close-and-capacity-contract.md:23–33`，K界CREATING/LIVE/CLOSING/Q库存，slot不足走O0；无worker/backend服务界不能推排空时限。`owner/integration-analysis.md:114–122`按每策略真实`S∪P∪C∪Q`物理并集去重，不冻结O0的PFN再用轨迹；IOVA另计。stock O0也无idle B pin，因此候选相对stock**无天然payload RAM节省**；去掉copy可能少C，也可能因Q／替代分配增加其他部分，需实际数据。

准确不足：**策略独立性不足已证；真实再用率／完整成本与净收益未证；接口关闭协议仍条件设计。** `iommu-platform/out/mapping-reuse-opportunity-20261016/README.md:3–9`记录native统计null、源码正常再用可达但频率未知。资料无匹配输入不等再用0，更不等整个映射组织族失败。

## 6. R107：普通驻留已清空本有限卡的miss残余，准备和资源仍付费

本席直接读 `iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/live/finite-pilot-result/receipt.json:1`（完整JSON单行，精确字段定位如下）：

- `ioctl`：run_calls1/return0；`run.status=release_status=quarantined=data_errors=0`、stage11。
- `run.restore_bytes=swapout_bytes=1769472`，各432 tasks；swapout48、restore96 logical runs；batch_count144，worker_layers24/joint_layers12。
- `run.composition[0][12:15]=[1728,1720,8]`；`run.composition[1][12:15]=[1728,1728,0]`；hpm_before/after恢复walk与fill增量0。
- `run.leaves`确为8个2MiB叶、4池合16MiB；backing_bytes16777216，非凭“4KiB硬件task”推小页映射。4个pool各2359296B的`memcmp_equal=true`。

报告 `iommu-platform/docs/r107-ordinary-pilot-20261012.md:7–28,49–53`给来源、硬件身份、原始字段含义及全成本：普通大页＋premap＋驻留＋batch＋授权singleDMA完成全部当前工作。冷准备是在priorout支付，两phase间只cache交接、不flush/remap，不是免费warm初态；普通方案合法2MiB页不能人为删掉。CPU恢复阶段、完整ioctl、研究核验、backing/metadata成本分列，不从0walk推出一切成本为0。

**已证**：对这份all-ready有限卡，普通强控制之后没有被展示的可避免miss/walk残余，因此没有以该残余为由进入G0/G1的依据。**未证**：hit延迟全消失、换出/恢复硬件2844周期差全由8walk造成、原生GPU attention/token性能或多DMA竞争；报告`:26`明确当场无独立背景请求。不能用它否定动态到达、内存预算变化或其他AI需求。

第一次NEXT前EBUSY是把composition配置idle误认DMA quiet导致的工程错误、没有触发DMA（报告`:12–14`），经一次有界修复成功；它不是科学阴性，也不参与本卡失败根因。真正的立题不足是**任务当前允许的普通准备／驻留把目标残余清空**，而非方案因实现错误输掉。

## 7. 跨分支四个机制性共同点（均限证据覆盖）

1. **额外可见信息必须在相同服务要求下改变一个有用决定。** p5的细计费未兑现额外进度，但原因未知；D-CS1能精确证明主配置免费Z对完整菜单无价值。二者只共同证明“信号更细”不自动推出结果更好，不能用CS1的信息冗余替p5诊断。（§2、§4）
2. **局部提前完成可以把竞争推向下游关键资源，而不是消灭工作。** PN命中早放在M0改变payload/PTE返回竞争，普通DS优先收回差额；实板等miss数仍慢说明只看miss计数不够。该因果已证于M0声明服务模型，实板同因仍U。（§3）
3. **必须让普通方案使用真实合法的准备、等待和复用机会。** CS1等时间P等待复现某反号；R107大页与驻留清除当前miss残余；mapping O3可在相同付费接口做相同接纳，普通R共享frame引用简化。它们分别关的是特定策略增量、需求残余或逻辑原语主张，不能合并成“接口没有价值”。（§4–6）
4. **释放／quiet／库存／虚拟地址不是同一种资源证明。** CS1 host quiet可保留假设provider债务；mapping源page离pool、B关联撤图、CPU读者和真实DMA drain各有义务，K库存不等时限或净RAM；R107冷准备与backing成本必须保留。剩余费用与任务可行性未核时，局部CPU小计或0walk都不构成全系统净赢。（§4–6）

这些共同点不是新的机制设计或全族淘汰标准；它们只约束下一轮如何解释已有资产。最关键的未知仍是 **p5负收益的具体因果、isolated板级P/PN约0.90M差额的完整链、真实系统中可行动窗口及其完整接口费用**。本席未用新实验填这些缺口。

## 8. 审阅边界与交接

主要可点击证据入口（下文各相对路径均以工作区 `/home/xvhaocheng/work` 为根）：

- [p5四格保存分析](/home/xvhaocheng/work/iommu-platform/out/method88-analysis-20261004-01/report.zh.txt:21)、[原始p5第一轮metrics](/home/xvhaocheng/work/iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/method/runs/m88-b1-r1-p5-f/metrics.json:4)、[计费RTL](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/fpga/src/dma_research_policy.sv:284)。
- [P/PN原始板测P](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mp/record.json:148)、[PN](/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mpn/record.json:148)。
- [M0完整完成原表](/home/xvhaocheng/work/iommu-platform/out/workflow-g0-m0-20261010/results-table.csv:7)、[DS模型控制规则](/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp:679)。
- [D-CS1原始世界](/home/xvhaocheng/work/iommu-platform/out/d-service-model-20261015/model/worlds.json:1)、[provider与冷却源码](/home/xvhaocheng/work/iommu-platform/out/d-service-model-20261015/model/simulator.py:16)、[精确值诊断](/home/xvhaocheng/work/iommu-platform/out/d-service-model-20261015/analysis/findings.md:5)。
- [mapping普通O3代码](/home/xvhaocheng/work/iommu-platform/out/mapping-admission-20261016/ledger/fee_ledger.py:71)、[原始全费用边界](/home/xvhaocheng/work/iommu-platform/out/mapping-admission-20261016/ledger/result.json:2260)。
- [R107原始receipt](/home/xvhaocheng/work/iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/live/finite-pilot-result/receipt.json:1)、[报告及解释边界](/home/xvhaocheng/work/iommu-platform/docs/r107-ordinary-pilot-20261012.md:14)。

已读入口：根AGENTS、docs/agent-policy、平台AGENTS、CURRENT_STATUS、研究导航、研究计划、docs/research-results与docs/research-history/README；按本报告定位定向核原始报告／代码／JSON，不扫描Linux/RTL全树。p5读取24个指定metrics作四格算术，未重验88归档；P/PN抽核2个原始record；M0核原始结果与已保存匹配统计，未逐beat重建全部trace；CS1抽核2世界及3原始轨迹，未回放全部1578；mapping读算术代码与保存结果，未运行账本；R107读原始receipt与报告，未重新验硬件身份、raw pool字节或UAPI缓冲。

没有外部论文的新核读或前作全覆盖宣称；mapping中涉及DAMN／RFC的历史认识仅作为既有报告边界，本文独立结论依据是已读规则／源码与数据。未修改生产源码、共享状态、计划、HANDOFF或他人产物；本席唯一新增文件即本报告，由root纳入本轮统一验收与同步。

## 9. 追加：E2替换链与E3等待传播的决定性原始字段

本节按root追加任务，仅从两份正式报告定向核代表原始输入；未重验全部64项、全部UART、归档hash或重跑分析器／实验。证据类型是 **B：既有实板有限诊断**；下面算术是本席对保存字段的只读重算。

### 9.1 E2：一个原始record闭合驻留→覆盖→探测前消失→first miss

正式入口 `iommu-platform/out/e2-results-review-20261003/RESULTS.md:25,86–98`。决定性原始来源是 [W8正向N112第1轮record](/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/e2/runs/e2-boundary-w8-forward-n112-r1/record.json:1)；原文件为单行JSON，以下以raw字偏移和字段路径精确定位，不虚构多行锚点。

运行身份由同轮 [metadata](/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/e2/runs/e2-boundary-w8-forward-n112-r1/metadata.json:22)直接核读：role1 BIT记录sha `319b415e0a717565fa11df7bf23c1c9cc6ac8e283cdbaea791e795c9b2924d40`（`:22–25`），boot `64d4328b-ba1c-4e35-9044-7409fd158027`（`:26`），kernel `6.5.0-rc2-00093-gc0f5e13ea81f`（`:32`），exit/framed_exit0，taint前后0。参数为A8／112次、5,000,000cycle、warm2、Vprobe1、正向（`:3–20,27`）。这些是保存运行身份，不是本席新验BIT字节。

raw含义定向核 `iommu-platform/scripts/research_results.py:111–116,202–225,306–374,970–985,994–1026`：warm/deadline/freeze基址0x100/0x120/0x140，16项每项2字；事件基址0x200、每项6字；tag的VPN/PSCID/valid解码及target身份匹配。不是靠摘要分类反推raw。

| 链条位置 | 本席从该record直接读得 |
| --- | --- |
| V两个target | raw[0x10:0x14]：src VPN67108352／dst VPN67108353，PSCID1、GSCID0、S1、valid |
| warm驻留 | raw[0x100..0x11f]：src在slot10，dst在slot2 |
| A覆盖dst | event16，raw[0x260..0x265]：epoch40520、phase2、DID11、slot2；旧VPN67108353/PSCID1有效，改为VPN67108367/PSCID2有效；target1_removed=1 |
| A覆盖src | event17，raw[0x266..0x26b]：epoch40961、phase2、DID11、slot10；旧VPN67108352/PSCID1有效，改为VPN67108359/PSCID2有效；target0_removed=1 |
| 探测前消失 | deadline快照raw[0x120..0x13f]：两个target均无有效匹配项；snapshot_valid raw[0x21]=7 |
| A已完成／后发probe | raw[0x25]=raw[0x26]=112；A最后done raw[0x28]=114329；deadline raw[0x1e]=5022641，probestart raw[0x1f]=5023975。phase_stats[1]的in_flight_at_end=0 |
| 首次再用 | dst raw[0x18]=5028528，src raw[0x14]=5028939；对应fp_live的hit bit32均0、self bitmap均0。顺序是dst先、src后；src的other bitmap32来自dst已回填，不能误说A后dst还驻留 |
| 探测工作与结果 | phase_stats[2] completed1／4096B、miss2／S1PTW2、data_errors0；raw[D87]=1487、D88=5、D89=823、D8C=2hit、D8D=2miss；freeze快照已回填src slot13／dst slot5 |

这份原始链支持：**在此有限访问布局和实现中，A提交更新确实移除V驻留条目，引发quiet A之后V首次再用miss及额外翻译工作。** 该例4KiB probe，不是E3的64B；5,000,000cycle是A固定阶段窗口，不是替换耗时。W7与W8的14+2=16／16+2=18边界及64项重复总体仅据正式报告`:7–9,43–56`保留，本席没有再扫全部raw。不得外推所有布局、全部共跑稳态吞吐或将D87差全叫纯PTW。

### 9.2 E3：四组原始配对按D128−D0重算剩余等待增量

正式入口 `iommu-platform/docs/e3-controlled-results-review.md:59–78`。决定性原始来源为 [all-original-pairs.json](/home/xvhaocheng/work/iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/all-original-pairs.json:1)，也是单行JSON；只读零基数组索引4/6/8/10（报告pair5/7/9/11），各两轮 `rounds[].e3_words`。字段规则直接核 `iommu-platform/out/e3-controlled-review-20260918/server_review_v6.py:47–59,670–686`：**Aremaining=e41−e4，Vremaining=e70−e4**；e41是A翻译响应，e70是V被选择，e4是被选PTE响应在延迟器收到时刻，不是数据返回时间。

| 原始pair／main case | D0的(e4,e41,e70) | D128的(e4,e41,e70) | Aremaining D0→D128 | Vremaining D0→D128 | 两项增量 |
| --- | --- | --- | --- | --- | --- |
| pair5／case2，seq9/10 | (6117,6389,6398) | (6118,6517,6526) |272→399|281→408|+127／+127|
| pair7／case4，seq14/13 | (53820,54091,54100) | (6196,6596,6605) |271→400|280→409|+129／+129|
| pair9／case6，seq17/18 | (6089,6361,6370) | (6113,6514,6523) |272→401|281→410|+129／+129|
| pair11／case8，seq22/21 | (6214,6485,6494) | (6088,6489,6498) |271→401|280→410|+130／+130|

pair7/11原运行先D128后D0，表格按delay归一后才取D128−D0；不把“第二轮减第一轮”的−129/−130误报为反向机制。八轮raw trigger=1，actual_d e7与配置D0/D128一致；target A身份e32..e36=[seq1,ID4,DID11,IOVA274875809792,event127]，V e64..e68=[seq1,ID0,DID10,相同数值IOVA,event127]，V first_hit e76=1。上下文DID不同，相同IOVA不是相同翻译条目。四份input均chunk_bytes64、transfers1、warmup0、cold/complementary。

运行身份／未验证项直接核 [run.json](/home/xvhaocheng/work/iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/run.json:3)：label `e3-v6-00087-controlled`，observer magic `0x49444D414F425306`，实读kernel `6.5.0-rc2-00087-g2de370cd344e`（`:32–41`），DS50/50（`:11–14`），单次启动有限受控诊断。另直接读原始 `console.raw.log:99,277`的Linux启动／uname行，版本相符。BIT仅expected hash `b55f5afbba667a7540ed003dd724d078e0e6e4bff5d30fe7e796abba5b7f2ecc`，`:7–9`明确sha256 null／hash_verified false；软件payload sha及SD回读亦未证（`:17,39`），不能写成已完整镜像绑定验收。

人工延迟不是自然工作负载事实：本席直接核 `iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_e3_delay.sv:20–31,40–60`，每ARM捕获一次eligible单beat响应，saved寄存器保持，selected且delay128时remaining=128再倒数。正式报告`:39–46`限定S1 PTW、A DID11、V已等待；其旧行号对应当时源码，不把今天代码路径存在升级为旧bit字节匹配证明。

因此可说：**本实现／本到达方式的A-miss PTW人为延长，在四个有限64B配对中同步延长后来V-hit的剩余等待，增量127/129/129/130，支持共享串行控制／AR队列等待传播。** 完整排队与硬件完成增量不是同一分母；四对中仅case6触发前时间匹配，其他三对保留、单boot/N4，不作统计推广、通用AXI强制HOL或自然4KiB总慢化归因（正式报告`:68–82`）。hot对未触发，不能冒充受同等128cycle暴露的阴性。
