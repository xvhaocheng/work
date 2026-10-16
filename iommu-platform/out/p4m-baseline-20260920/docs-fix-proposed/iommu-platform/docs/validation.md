# 验证记录（2026-09-08）

## 2026-09-20 P4M-1 主机验证完成，软件构建通过，FPGA 构建进行中

本轮实际状态：P4M-1 主机实现与主机验证完成。`validation.json` 记录 14 组主机/RTL 检查 exit 0，包括 production frontend+observer 集成；`board_tested` 仍为 false。完整软件构建通过 exit 0；FPGA 构建当前 running，最终结果用 {{FPGA_RESULT}} 占位；交付 README 待定（[delivery README](../out/p4m-baseline-20260920/delivery/README.md)），用 {{DELIVERY_RESULT}} 占位。证据：[validation.json](../out/p4m-baseline-20260920/validation.json)、[build-status.json](../out/p4m-baseline-20260920/build-status.json)。版本：内核 `6.5.0-rc2-00088-g9f8da9e7a6a0`，CVA6 `e822bc9d8adf246a5d098585fb229744bafd9cc1`，Linux `9f8da9e7a6a08fb19134146241018eaff0eee124`，IOMMU 未改 `137f5e5fb1f2cc65a1346359226b16247c3f8445`；软件构建根 `1cdeb61`，FPGA 构建 `2fbf7ae`，两者差异仅为集成测试，不涉及综合源码。最新实际板上版本仍为 E3 v6/Linux00087；P4M profile `migration-4dma-p4m`/observer v7 未上板，Dell 无板。下一项是板上短验收 `platform-p4m short /tmp/p4m-v7-01`：Solo/A1gap0/A8gap0/A8gap1000，64 jobs、warmup 8、4 KiB；gap1000 仅工程点。之后预试同版本 gap0 并选定正式小 gap 集；暂不做全矩阵或复杂硬件方法。主机模型不等于板卡验收，等待 FPGA 结果不等于板卡通过。本次文档修订未重跑测试/构建，未读取成功日志；以上以实际 validation.json/build-status.json 为准。

## 2026-09-18 E3 v6 实板回传与服务器复核通过

本轮 3 短 + 8 正式对共 22 轮，板端数据错误、quarantine、taint 均为 0。服务器 9 组检查通过、exit 0；11 份 CLI JSON 与 UART 全字段匹配，418 条唯一观测、114 条相同重复去重，输入哈希未变。正常对照及 7 类故障注入均按预期通过/拒绝。[复核 JSON](../out/e3-controlled-review-20260918/server-review.json)、[汇总和测试](../out/e3-controlled-review-20260918/review-summary.json)。初稿分析程序错误已修正，不是 FPGA 失败。

4 对 cold 的 A 剩余服务和 V 剩余等待增量同为 127/129/129/130 周期，V 自身翻译 4 周期。支持当前实现/配置下的 A-miss 引发 V-hit 阻塞；有限样本与前置时间差保留，不将其等同于完整 E3/E4 验收或方法收益。[结果分析](e3-controlled-results-review.md)、[下一步 P4M](e3-next-method-plan.md)。本次未改 RTL/驱动/CLI、未重新构建或上板。下载哈希/SD 回读与原时钟/CDC/IO 边界保持；旧构建状态是历史快照。

## 2026-09-18 E3 v6 / Linux00087 实板完成并回传

Mac 已按固定 3 短 + 8 正式对完成 22 轮，板端及离线执行/字段/守恒验收均通过；22 个 sequence、418 条完整观测，data_errors/quarantine/taint 均为 0。新 BIT migration-4dma-e3-controlled，实读 observer magic 0x49444d414f425306、launcher 0x49444d414c4e4301、STAT0x501079fc；Linux 6.5.0-rc2-00087-g2de370cd344e。无重试/补采/扩矩阵/重编，已退出 root 并关闭串口，printk 未改。

完整 16 文件已回传并逐文件远端回读比对：/home/xvhaocheng/work/iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/。含原始 short/main JSON、全部 UART、验收/摘要、版本 delivery/run、host-validation、E3_CONTROLLED_RESULTS.md 及验收脚本/测试。归档 run/summary 保留上传前状态，回传完成以本节和 Mac upload-receipt.json 为准。主机回归 111 项、25 子测试通过；实时 printk 有破碎字符，完整 dmesg 回显完好，按阶段 seq 取 114/304 条观测，原始字节与所有样本保留。

正式 cold 12/12 触发、hot 0/4，V miss/S1 PTW 全为 0；cold A 每轮 miss/S1 PTW 均为 2。4 对 cold 互补的 V 剩余等待 D128−D0 为 +127/+129/+129/+130 周期，自身翻译均为 4 周期；正式 2 对 D0/D0 的 V 剩余等待差为 −2/0。跨轮目标/PTE/前史计数/插入身份一致且无截断，但触发前等待/服务仍有波动，仅互补对6两项前置时间完全相同。有限证据支持延迟传播，不凭接近128即宣布纯D因果已证明；64B诊断不计算吞吐、不与旧A8混算。下一步服务器独立复核身份/前缀及原始证据，不自动扩实验或改RTL。

按用户要求跳过 Mac 下载哈希和 SD 回读；交付 SHA 是来源声明，非 Mac 实测。时序收敛/CDC/外部IO审查边界保留。DeepSeek 起草验收、收集脚本及测试修订；Codex 审查、应用、运行验证/上板/回传并完成原始证据解释。每轮完整结果默认回传 Dell 的持续规则继续有效。

## 2026-09-18 E3 v6 受控有限实验（构建交付完成）

[v5 结果复核](e3-queue-results-review.md)：服务器 9472 检查/0 失败，原始文件哈希不变；执行/覆盖有效，因果配对未成立。新 [E3_CONTROLLED](../E3_CONTROLLED.md) 保持 DS50/50，用固定映射、单次 64 B 请求和 A 读选中后硬件释放 V，隔离 CPU 启动间隔与无限 A 供给前史。

已通过前端 HOLD/GO/cancel、launcher、两路真实前端联测、finite observer、旧 batch/observer/E3 回归、HPM session、checked S1 flush、CLI 和 rootfs 主机检查，以及相关内核对象交叉编译。驱动错误路径 19 场景模型与板端脚本已通过；完整软件和 FPGA 构建均退出 0、manifest passed（内核 00087）。BIT/MCS 非空，清单产物哈希、35 个验证源码哈希、关键硬件导出源码及实际 rootfs 内程序均已核对。FPGA 结束 UTC 06:24:37。[18 项验证记录](../out/e3-controlled-20260918/validation.json)、[构建状态](../out/e3-controlled-20260918/build-status.json)、[完整交付](../out/e3-controlled-20260918/delivery/README.md)。新 v6 尚未上板；主机模型不证明真实缓存一致性或完整 SoC 行为。成功日志只保存，时序报告未读；原时钟/CDC/IO 审查边界保留。

## 2026-09-18 E3 v5 / Linux00086 实板完成并回传

Mac 已完成 3 对短验收 + 10 对正式实验，26 子轮全部通过，26 个 sequence、494 条去重观测，data_errors/quarantine/taint 均为 0。新 BIT 为 migration-4dma-e3-queue，实读 observer magic 0x49444d414f425305，STAT0x501079fc，Linux 6.5.0-rc2-00086-gd2cd32d78918。未补采或重编；printk 已恢复，串口已关闭。

完整证据：/home/xvhaocheng/work/iommu-platform/out/mac-e3-queue-20260918-130545-run-d01lc9wp/，含原始 short/main JSON、验收/摘要、完整 console.raw.log、版本 run.json、host-validation.json 及 E3_QUEUE_RESULTS.md。归档 run/summary 保留上传前状态快照，回传完成以本节及 Mac upload-receipt.json 为准。正式 A8 16/16 触发、A1 0/4；A8 目标 V 首次 miss 16/16、插入记录截断 9/16、实际触发时旧队头为 A 9/16。中位吞吐 A8 109.29 MiB/s、A1 149.51 MiB/s；前史及插入差异仍在，不证明纯 D128 效应或 HOL。所有不可比样本保留，不扩大矩阵。

用户持续授权：以后每轮结果必须默认回传 Dell 并更新交接，不以仅 Mac 本地保存视为交付完成。此授权不扩大硬件改动或重跑范围。DeepSeek 起草辅助脚本/文档，Codex 审查、修正语义并执行测试、上板及回传。

## 2026-09-18 E3 v5 等待识别与带诊断配对（构建交付完成）

00085 D0/D0 回传已独立复核：9对/18子轮/342条观测与UART一致，原数据哈希不变；正式A8八轮触发但前史仍不同，旧约8ms差未稳定复现。详见 [结果](e3-diag-results-review.md)。

已实施控制器真实V接收未选中计数，覆盖A读占AR头时的后续V等待；v5/bankv2新增门槛、旧头、触发前摘要和两条插入请求。Linux新增带原软件诊断的互补D配对，旧v4在运行前拒绝；旧接口兼容。[新协议](../E3_QUEUE_PAIR.md)定义有限3短+10正式和字段口径，尚未新上板。

主机验证全部通过：真实controller/FIFO/demux/queue/delay开关各15场景，E3延迟/写/观测/控制，旧observer/path，驱动76控制+3缓存+96小chunk，E3 CLI49/旧CLI69/rootfs5；只证明相应模型/接口，不是完整SoC或板卡验收。证据 `out/e3-followup-20260918/validation.json`，真实控制器联测单独有 `controller-test/result.json`（源哈希已核对）。新78..95摘要/队头仍需外部语义自洽检查，不能将syscall success写成全部新增字段已独立核验。

构建起点顶层0119909，CVA6 147c4d4a、IOMMU 137f5e5、Linux d2cd32d78918，lock/gitlink一致。软件退出0/manifest passed，内核00086，所有软件产物哈希与manifest一致。FPGA首次因许可证连接失败退出2，失败树/log已归档；完整地址及允许网络环境下从锁定源码重启构建成功，退出0/manifest passed，BIT/MCS非空，结束UTC 00:56:45。所有清单产物哈希与关键导出源码一致，MCS未列入标准manifest，其非空检查及哈希另记delivery.json。成功日志不读，只查实际错误；原时钟/CDC/IO边界不变。构建最终状态见 `out/e3-followup-20260918/build-status.json`；[完整交付](../out/e3-followup-20260918/delivery/README.md)。BIT SHA256 `8f90865ef1fb4f747e7a1f16c3a4992adf4499c56356e0d8d1679b8962d7342e`；payload `13dc664895f570925b6ae59f11cedbfefcb3de331c0d9d6518557b7ce4a7ea36`。

DeepSeek提取证据并提出CLI/桩/CLI测试修改；Codex核对原始记录与关键代码、负责RTL/驱动、应用审查提案及执行验证/构建。旧数据/产物与既有脏文档保留。

## 2026-09-18 00085 D0/D0 实板诊断完成（Mac回传）

复用原E3 BIT，STAT0x501079fc，Linux00085启动。3短+6正式配对共18子轮执行/数据/观测检查通过，data_errors/quarantine/taint0，每轮256字段及19条观测完整；固定V0/A1，正式顺序A8,A8,A1,A8,A8,A1，没有补采。证据在 iommu-platform/out/mac-e3-diag-short-20260918-081411-run-1z6a407q/ 和 iommu-platform/out/mac-e3-diag-main-20260918-081411-run-1z6a407q/（相对work），各有records/assessment/summary.json，summary包含完整UART。用户明确批准后已回传。

本轮正式12轮V预热256/256，A提交/完成/排空256/255/1；A8末页均7、下一页0。A8预热miss/S1 PTW首轮512，第二轮312（第1对318），工作量/末页相同但前史状态不同。A8八轮均触发、正式V miss均2，A miss多数5142（第1对次轮5133）；A1四轮无触发且正式miss0。第3对A1第二轮软件A返回到V开始间隔16.9236ms，不能当精确硬件发射差或指认具体中断。硬件MiB/s中位A8两轮109.10/109.07，A1两轮149.36/149.56。

旧约8ms轮次差未在本诊断组稳定复现，但诊断有扰动，不称修复或D128/HOL因果完成。先独立复核预热HPM/软件启动区间，再决定最小状态/触发覆盖改动；本轮无RTL/驱动改动、重编或扩矩阵。已恢复printk、退出root、关闭串口。原哈希/SD回读跳过及时钟/CDC/IO边界保留。Mac详细报告E3_DIAG_RESULTS.md；DeepSeek整理证据表，Codex核对原始代码/JSON并执行测试与板卡操作。

## 2026-09-18 E3 D0/D0 软件诊断交付

新增独立 ioctl0x0a/能力位和 `--diagnostic 1`，记录两轮预热提交/完成/排空、末偏移、预热 HPM、ARM/启动软件时间区间；原 D0/D128 ABI/CLI 保持。Linux `1b268b89200c`，构建顶层 `c5c4c43`，lock/gitlink一致；新内核 `6.5.0-rc2-00085-g1b268b89200c`。构建后仅去重测试列表，不改变编译源码。

真实驱动主机模型64控制场景（新增11诊断）+3缓存+96小chunk、E3 CLI39、原CLI69、rootfs5通过；覆盖D0/D0、两轮清零、预热HPM差分、不等速A、零预热、Solo、旧能力拒绝、非法配置、失败及隔离。软件完整构建退出0、manifest passed、产物哈希核对通过。日志仅保存，无成功日志审查。证据 `out/e3-diag-20260918/{validation,build-status}.json`；[交付](../out/e3-diag-20260918/delivery/README.md)。

沿用既有E3 BIT `a99ce956…`，没有RTL修改/新FPGA构建。payload SHA256 `1e4a49b08bfcbcbeb1e86eb17fb0836d373d5950d61b274b885d443f7c458f62`。新版尚未上板；主机模型不证明FPGA行为。下一步按 [诊断卡](../E3_DIAGNOSTIC.md) 更新SD软件，3短+6诊断对，不扩矩阵。软件时间不是硬件握手；诊断本身有扰动。AR队头触发覆盖限制及原时钟/CDC/IO边界仍在，待回传后安排硬件修订。

## 2026-09-17 E3结果服务器独立复核

15配对JSON、30子轮、570条观测与summary内UART逐项一致，退出/taint/数据/排空/AXI/内部子段/写路径守恒、实际D0/128延迟检查通过。原数据未改，证据 `out/mac-e3-main-20260917-222837-run-p0lbvbv7/server-review.{py,json}`；[分析](e3-results-review.md)。

正式首轮A1→A8：V读/写排队18/4→596.78/263.62周期，选中后1/4/2不变；写后段增量在WLAST前，之后约18周期不变。正式A8双触发0/10；第8/9对无触发仍第二轮快约8ms，不能归因D128。触发只看AR头的范围限制已由代码确认，但尚未确认首轮无触发的逐门槛原因。跨配对5个V首查hit记录的剩余等待中位差126周期仅作探索线索，不能替代计划内对照。

本轮仅分析/文档，无代码修改、构建或新上板。待双轮状态/启动及触发覆盖修正后再做有限因果配对；不因当前失败扩样本。原下载哈希/SD回读跳过、时钟/CDC/IO与短测hrtimer边界保留。

## 2026-09-17 E3实板采样完成：执行通过，配对因果条件未满足

新migration-4dma-e3 BIT/Linux00084启动，STAT0x501079fc。按交付完成3短+12正式配对（30子轮），每轮256字段及19条观测齐全；数据错误/quarantine/taint0，任务/AXI/path/E3守恒和触发延迟精度检查通过。没有补采、扩矩阵或修改RTL。

关键限制：正式10对A8第一轮触发0/10、第二轮8/10，两轮均触发0/10；首轮D0/128已交替。第一轮全V miss0/A miss6160；第二轮8次V2、2次V0，A miss5133/5136。硬件中位第一/二轮155.245/146.567ms（103.06/109.16MiB/s），不可当128周期因果效果。2对A1四轮无触发。短A8虽双触发但身份/前史/首次命中不同且有插入，亦非干净对照。先独立复核双轮状态及触发门槛，不以加样本替代问题解释。主机检查通过只说明测量正确性，不证明HOL。

证据：iommu-platform/out/mac-e3-short-20260917-222837-run-p0lbvbv7/与iommu-platform/out/mac-e3-main-20260917-222837-run-p0lbvbv7/，各含records/assessment/summary.json（summary含完整UART，路径相对work）；Mac报告E3_STAGE_RESULTS.md。短测一次hrtimer延迟13439920ns保留。printk已恢复、退出root/关闭串口。原哈希/SD回读跳过及时钟/CDC/IO边界保留。

## 2026-09-16 E3单次干预/配对会话实现与完整构建

[E3_SERVICE](../E3_SERVICE.md)已实现v4内部四段、首次IOTLB查询、有界A/V记录、COMP AW/WLAST/B配对，以及单窗口一笔A PTW响应D0/128。同一ioctl映射生命周期内分别预热/排空/验证两轮，保留批量轮询及DS50/50；异常停止、quarantine规则不变。无触发/首次V缺失/插入请求保留，不等同因果通过。

- 真实RTL定向验证通过：精确D0/128、一次触发、重复ARM、背压及禁用保留响应；四lane/每lane同ID四笔排队、身份/溢出/冻结、V首次hit/miss及中间插入A；COMP写AW/W先后、同时发生、环绕、跨ID B重排、carry/结束在途/错误；v4 MMIO范围、模式、错误传播。
- 旧observer28+6、v3路径17及缩位溢出、双frontend联测4通过。真实驱动主机模型53个控制流场景+3缓存场景+96小chunk组合通过，其中9个配对新增场景覆盖一次分配两轮、正反顺序、Solo、旧BIT拒绝、无触发/触发、首轮/次轮失败及超时隔离；E3 CLI23组、旧CLI及rootfs4项通过。上述是事件/MMIO模型，不是完整IOMMU压力仿真。
- 软件和FPGA均退出0/manifest passed，BIT/MCS非空，FPGA结束UTC 09:27:41；内核00084。构建起点顶层bea8f57，CVA6 44068498、IOMMU1519510、Linux94a519eebf07，lock/gitlink一致；构建期间仅文档/测试提交e4d6161，不改变锁定组件。原研究文档dirty状态保留并记录在manifest。
- 交付[README](../out/e3-service-20260916/delivery/README.md)，证据`out/e3-service-20260916/{validation,build-status}.json`。BIT SHA256 `a99ce9565b82449ca5eb0a8afc18c8b79cdf9e23a5a4e9197aafae02b5df98d4`，payload `c9b1ac46d6c9ef623f2a814477fb92d7ab62441eff97278cd3376f254d621a33`。

成功日志仅保存，未审查时序报告；原时钟/CDC/IO边界保留。尚未上板，无新性能/因果结论。下一步固定角色3项配对短验收（Solo/A1/A8，各含两轮），通过后10对A8+2对A1交替D顺序，错误停止，无触发不补采。

## 2026-09-16 v3分段路径结果独立复核

3短+30正式试次的原JSON及同seq三条batch、16条path共627条观测与回传UART一致，退出/taint/数据/排空/身份/窗口/AXI及路径守恒通过。证据 `out/mac-path-main-20260916-164633-run-vvguros8/server-review.{py,json}`；[结果分析](path-results-review.md)。原数据未改。

硬件中位56.474/107.160/154.920ms，正式V miss全0；A8-A1内部读+576.04、写+257.68周期，入口/出口地址反压不变，写后段+325.29周期。分段定位目标已完成，不能将累计周期相加作整批代价或宣称HOL。下一步[E3因果计划](e3-shared-service-plan.md)仍待实现；本轮无RTL/软件修改、构建或新上板。软件窗口及hrtimer日志单列，原HPM/下载哈希/SD回读/时钟CDC边界保留。

## 2026-09-16 v3分段路径实板验收与30次正式试验

Mac已加载配套v3 BIT/内核00083，固定V0/A1：Solo8两次+A8 64一次（warm0）通过后，Solo/A1/A8各10次4096项/warm256，全部通过。每次保存3 batch+16 path，同seq、valid/status、任务/AXI/各段和邻段守恒、V边界与area均通过，数据错误/quarantine/taint0。硬件中位吞吐283.32/149.31/103.28 MiB/s；V miss/S1 PTW正式全0，A8 A miss/PTW全6160。原50/50慢化保持；新增stage1及写stage3等待不能直接称纯PTW/HOL。

证据：[短验收](../out/mac-path-short-20260916-164633-run-vvguros8/assessment.json)、[正式验收](../out/mac-path-main-20260916-164633-run-vvguros8/assessment.json)；相邻records.json保存全部JSON/19行结构化记录，summary.json含完整UART与中位/min/max。短测hrtimer中断延迟16886920ns原样保存，硬件/软件窗口分报。未重编、未审核成功构建时序，下载哈希/SD回读按用户要求跳过。

## 2026-09-16 v3分段路径观测实现及完整构建

实现[E4_PATH](../E4_PATH.md)：DS50/50、整批轮询保持，按源ID/读写/同ID顺序匹配四段边界；carry/start/end与full分别记录，错误及溢出置无效。软件批量默认要求v3，每次在排空/验证后输出原3条batch+16条path。

- 新真实RTL17场景通过：四端口、读写、重复ARM、同ID四笔在途、双源交错、反压、零周期、窗口首尾、冻结及ID/容量/非空闲错误；缩位事件数/area/sum溢出和清除通过。
- 旧observer28+6、frontend联测4、实际驱动47场景与96个小chunk组合通过。驱动覆盖缺v3、路径计数/守恒/area错误和合法carry。证据 `out/e4-path-20260916/validation.json`；这些是事件/MMIO模型，未验证板上真实时序。
- 软件和FPGA完整构建均退出0、manifest passed，BIT/MCS非空；源码顶层a2e8475、CVA6 a1572926、Linux45bf76b15e36（00083），lock/gitlink同步。profile `migration-4dma-path`。成功日志仅保存未审查，未新增时序签核结论。
- BIT SHA256 `62f822d19851da0309b1a7e085197661caa72f6366fccf34d721fa418b941c29`；payload `031025c1fb1551ab0e82610027f76106e7b2efe3052b45cda62ea39dc0173c6a`。交付[README](../out/e4-path-20260916/delivery/README.md)，构建状态 `out/e4-path-20260916/build-status.json`。

尚未上板，无新机制/性能结论。固定V0/A1，下一步8项Solo两次与64项A8一次无预热；通过后Solo/A1/A8各10次4096项/warm256。原时钟/CDC/IO待办与旧实验产物保留。

## 2026-09-16 后续机制计划（仅文档）

新增 [后续执行计划](next-mechanism-plan.md)，定向核对现有observer、IOMMU入口/译后接口及事务控制器ID传递。仅确定实施边界和验收条件；没有新增RTL/软件、测试运行、构建或上板结果。此前DS50回切复核结论保持。

## 2026-09-16 DS50回切服务器独立复核

30条JSON/observer与UART逐项核对通过，原数据未改，历史对照源文件哈希未变；退出/taint/数据/排空/身份/任务/H+gap/AXI守恒通过。证据 `out/mac-batch-ds50-20260916-135544-run-eawziww2/server-review.{py,json}`。A8硬件时间154.99→118.19→154.88ms，主要miss模式恢复；第14/16项V miss2/A miss5141保留。复现目标完成，机制未定位。详见 [回切复测复核](ds50-retest-results-review.md)；本轮无构建或新实板操作。

## 2026-09-16 50/50 BIT回切复测完成（最新）

固定V=DMA0/A=DMA1，Solo/A1/A8各10次，30/30通过；STAT0x501079fc、Linux00082。硬件中位283.33/149.51/103.31MiB/s，V miss/S1 PTW Solo/A1中位0，A8中位0（少数样本可2），A miss/PTW中位6160；与DS0/0的A8 135.38MiB/s相比恢复至约103MiB/s。data_errors/quarantine/taint0，observer计数/AXI守恒通过。证据out/mac-batch-ds50-20260916-135544-run-eawziww2/，完整UART已随回传保存；未重新编译或刷卡，哈希/回读按用户要求跳过。

## 2026-09-16 DS0结果服务器独立复核

60条原JSON/observer与回传UART逐项一致，退出/taint/数据/排空/身份/任务/窗口/AXI守恒通过；历史对照原文件哈希未变。复核结果 `out/mac-batch-ds0-20260916-132332-run-ebgxhirr/server-review.json`，计算过程为同目录server-review.py。A8相对A1额外硬件时间44.71%→10.50%，支持DS敏感性；V miss及A miss模式同时变化，且未同期回切50/50，不能定量称纯翻译损失。详见 [DS0结果复核](ds0-batch-results-review.md)。本次无新构建或实板运行。

## 2026-09-16 DS0/0实板正式对照完成（最新）

baseline-4dma-batch加载成功，STAT0x501079fc、Linux00082；固定V=DMA0/A=DMA1，六条件Solo/A1/A8/A16/A32/A64各10次，4096项/warmup256，60/60通过。按用户要求不短校准、不交换角色。数据错误/quarantine/taint0，observer计数/覆盖/窗口/AXI守恒通过。硬件中位MiB/s 283.17/149.59/135.38/131.61/129.98/128.64。

较此前同角色DS50/50，Solo/A1基本不变，A8～A64提升约31%～36%；旧数据为历史对照，非同期交错采样。新版A8～A64共40项V miss/S1 PTW均2，Solo/A1均0；A miss/PTW中位0/5141/6940/7501/8331，S2全0。不能直接把提升折算单次PTW代价或证明HOL。原样保留全部样本，无补采。

证据out/mac-batch-ds0-20260916-132332-run-ebgxhirr/，含原JSON/observer、统计及完整UART；Mac报告DS0_BATCH_RESULTS.md。未修改RTL/Linux或重编；原哈希/SD回读跳过、HPM/时钟/CDC/IO签核边界保留。已恢复printk并退出root，当前保持DS0/0配置。

## 2026-09-16 batch结果服务器独立复核

104条（短14+主60+扩展30）原JSON/observer与summary内UART逐项匹配，退出/taint/正确性/计数/窗口及AXI守恒通过；原文件不改。证据 `out/mac-batch-main-20260916-112954-run-9sa69rhc/server-review.{py,json}`。自主供给/持续重叠/硬件慢化的阶段目标达成；A8较A1的V硬件中位耗时增加44.7%，具体机制未定，两个非零V miss样本保留。范围和统计见 [batch结果复核](batch-results-review.md)。本轮无构建、新上板或E2/E3实施。

## 2026-09-16 A16/A32/A64固定角色实板扩展完成（最新）

用户明确不短校准、不交换角色；固定V=DMA0/A=DMA1，4096项/warmup256，各10次轮换条件，30/30通过。数据错误/quarantine/taint0，30组observer窗口/计数/覆盖/AXI守恒通过。硬件MiB/s中位98.27/95.95/94.84，A miss/S1 PTW中位7246/7743/7976；第28项A16的V miss/PTW2，其余29项V0，完整保留未补采。S2全0。

证据out/mac-batch-ext-20260916-114036-run-8pjv_djc/，包含原始JSON、观测、统计、完整UART；Mac报告BATCH_EXT_RESULTS.md。未重编或刷卡，旧Solo/A1/A8未重采，仅历史对照；不能单凭趋势归因为HOL/纯翻译。新增页数未独立短校准、未反向角色验收，原HPM/时钟/CDC/IO待办保留。

## 2026-09-16 batch实板正式矩阵完成（最新）

Mac新BIT/Linux00082完成14短校准+60正式样本，74/74通过；正式4096项/warmup256、Solo/A1/A8、双角色每条件10次轮换顺序。数据错误/quarantine/taint均0，observer计数/覆盖/H+gap/AXI守恒通过。硬件中位吞吐约283/149/103 MiB/s，双路在途重叠99.69%～99.79%。第42项DMA1+A8的V miss/PTW2，其余59项V0；A8 miss/PTW中位6160，第42项5141，完整保留无补采。不能归为纯翻译或HOL；硬件窗口与软件/HPM窗口分开统计。

原始JSON、观测、统计和完整UART已回传 out/mac-batch-main-20260916-112954-run-9sa69rhc/；短校准 out/mac-batch-short-20260916-111626-run-n8mg7wsz/。Mac报告BATCH_MAIN_RESULTS.md，RTL/Linux未改、未重编，下载哈希/SD回读按用户要求跳过。原时钟/CDC/IO与完整HPM语义待办保留，不自动启动E2/E3。

## 2026-09-16 有限硬件批量版（构建交付完成）

用户批准RTL/驱动批量模式，并要求编译日志只保存、有错误才检查。实现/边界见 [E4_BATCH.md](../E4_BATCH.md)。frontend批量11、旧模式对照12、observer28+2溢出、双frontend/observer合成backend联测4、驱动41场景及96小chunk组合、CLI回归通过。软件及最终FPGA构建均退出0，manifest为passed，BIT/MCS非空；Linux00082。软件构建顶层e47b48e，重复批次计数修正后的FPGA构建顶层3198fef，Linux b7979a1a06ac、CVA6 7ed43840cab2。证据 `out/e4-batch-20260916/`，交付 [README](../out/e4-batch-20260916/delivery/README.md)。只确认构建状态及产物，未审查成功日志/时序报告；无新版实板结果，不宣称时序/CDC签核。下一步新BIT及SD软件一起更新，先8/64项短批次、重复相同配置和双角色重叠校准。

## 2026-09-16 E4服务器独立复核

105条原JSON及86组观测逐项检查通过，复核脚本与结果位于 `out/mac-e4-stage-20260916-094001-run-kzi_s0zc/server-review.{py,json}`，含原文件哈希，未改回传数据。按同角色均值差，共跑窗口增量99.08%～99.92%在P；60主样本含40共跑均无AXI重叠，V的H基本不变、miss0。详见 [E4结果复核与下一步](e4-results-review.md)。仅复核/文档更新，未改代码、构建或上板；建议的独立批量供给尚未实施。UART未在服务器重解析，Mac下载哈希/SD回读未做的版本边界保留。

## 2026-09-16 E4新版实板验收完成（最新）

Mac以新observe BIT和Linux00081完成105/105：9短测、36开关开销、60主样本。86组time/axi_v/axi_a检查通过，data_errors/quarantine=0、taint0，开关配对中位变化最大0.506%，低于预定1%门槛。软硬件未再修改。

主样本每条件10次：V H约56ms基本不变，P从Solo约346–349ms增至A1约767–769ms/A8约804–805ms，R约185–191ms；60主样本AXI both均0。支持优先核查供给/完成读取路径，不把P全部当CPU、不把H全部当IOMMU，不宣称HOL。下一步供给方式改动需另行明确，不自动实施E2/E3。

证据：`../out/mac-e4-stage-20260916-094001-run-kzi_s0zc/` 内105条原JSON、86观测组、逐项判定及版本边界。完整UART在Mac results/serial/20260916-094001-console-x8xsc_f0/console.raw.log，详细说明E4_STAGE_RESULTS.md。首轮直接printk缺字，从内核dmesg恢复完整观测；后续临时loglevel4并逐次dmesg读回，结尾恢复7 4 1 7、observe=N、taint0，串口已退出。用户要求未做下载哈希/SD回读，期望哈希不能称Mac实测。旧E1与本轮不合并，原时钟/CDC/IO待办保留。

## 2026-09-16 E4被动观测实现及构建交付

用户确认保留轮询后已实现最小硬件/驱动观测，默认关闭，原热循环/ABI保持。接口、哈希及边界见 [E4_OBSERVER.md](../E4_OBSERVER.md)，交付 `out/e4-observer-20260916/delivery/`。24+2个生产观测RTL场景、12个真实frontend前后逐周期对照任务、驱动原19+新增11场景及96小chunk组合通过；驱动目标编译和固定21组件检查通过。CVA6 `540544ce`、Linux `853e60b234c3`，构建顶层 `228aed2`，独立profile `migration-4dma-observe`。软件和FPGA完整构建退出0，内核00081；新BIT SHA256 `41fa23d0aee476b603111df192c612f3d40e844067211564d2b8b175a60b9b90`。

布线后 WNS=0.177ns、WHS=0.027ns、WPWS=0.062ns，TNS/THS/TPWS均0，内部未约束端点0；LUT67.08%、Slice96.55%。观测2272个寄存器同属50MHz。原6项时钟方法Critical Warning及21/23个缺少输入/输出delay、3个部分input delay保留，不能把正余量称为完整约束/CDC签核。报告见 `out/e4-observer-20260916/reports-routed/`。初始许可证访问失败与缺RISCV手动重试均保留日志，成功构建沿用同一导出源码，最终manifest记录续建来源。

新版本尚无板上数据；下一步Mac更新BIT及SD软件，先关闭/开启观测短试次与计数校准，再36条开关对照及60条E4主样本。旧E1及产物保留，不重命名为纯硬件干扰证据。

## 2026-09-16 E4 观测实施前方案

已定向核对DMA frontend的任务ID/完成/DONE读取、8项FIFO与WAIT控制、平台mux两侧AXI及Mac读出约束。具体 [E4实验卡](e4-supply-timing-plan.md)采用Linux可读被动汇总候选，定义H/P/R和AXI重叠口径、校准、36条观测开销与60条主样本计划。仅完成方案，尚无新RTL/软件、仿真、构建或上板结果；E1实测状态不变。

## 2026-09-16 E1 服务器数据复核

已独立读取144条JSON，核对成功状态、V完成数、两路data_errors/quarantine；80条主样本逐条匹配assessment条件/时间。主样本V miss全0，A1 miss0仍慢约73%，A8/32 miss8190且比A1额外慢约3.5%～3.9%；V backend active中位数各组约270万周期。具体统计、来源哈希和边界见 [E1复核](e1-results-review.md)及回传目录server-review.json。建议下一步优先E4供给/计时核查，随后按证据安排E2/E3；没有认定硬件请求重叠、纯翻译慢化或HOL。本轮仅数据复核/定向源码阅读/文档，无新构建或板上运行。

## 2026-09-16 Mac 当前双路 E1 已完成（最新）

缓存同步内核00080与SC.W修复BIT已在Mac实板通过两次新启动29组功能试次、单DID32组和双DID12组归属检查；其后18个探索性能样本独立保留。本次用户确认仅做当前双路E1，不含E2/E3。

本轮144/144通过：16项有限HPM/冷热/进位回绕检查，48个预热/窗口敏感性样本，80个主样本（双角色，Solo/A1/A8/A32各10次）。data_errors/quarantine=0、taint0，脚本退出0。A1页软件观测耗时约Solo的1.73倍，A8/32页约1.79倍；敏感性归一化变化最大0.427%。未重建或刷卡，Mac串口已退出。

证据：`../out/mac-e1-stage-20260916-032650-run-rbo67pvd/` 的records.json、assessment.json、summary.json（内含采样前协议）。Mac完整UART在results/serial/20260916-032650-console-jz4u_u_b/console.raw.log，说明E1_STAGE_RESULTS.md/HPM_REMAINING.md。

范围严格限定软件计时E1、DS50/50。完整E0/P2仍未签核：S2正向、溢出IRQ、空窗口开销和硬件请求重叠缺接口，DDTW相邻事件漏计及时钟/CDC/IO待办保留；PTW字段不是独立页表访存量。无纯硬件慢化/HOL结论。下载哈希/SD回读按用户明确要求未做，不将期望哈希写成实测值。


## 2026-09-16 首错回传及 DMA 缓存同步修正版（最新）

诊断版已上板：新启动 warmup32 通过，随后完整 smoke 首组 warmup0 失败、余13组未运行。offset912 的 dst=eb 经 FENCE.I 变为期望14，源始终14，支持旧缓存视图；Linux正常、taint0。两条JSON和首错已到服务器，完整串口仍在Mac。

修正及边界见 [DMA 缓存同步](../DMA_CACHE_SYNC.md)：DMA前与安全排空后各同步一次，均在测量窗外；另修正软件 IOVA→PA 查询的页内偏移，不将旧诊断大地址误作硬件翻译。Linux `3b47ab1645e4`，构建顶层 `76de9ee73656cc90bdae5331be7020b5f905e205`，内核 `6.5.0-rc2-00080-g3b47ab1645e4`。19个调度/诊断/缓存场景、96小chunk组合、16软件查询场景通过；删除任一同步能被回归检出，旧软件查询12/16失败、修正后0/16失败。内核目标、checkpatch、21组件版本检查、完整软件构建和交付内容/哈希核对通过。保留原DTS警告及用户文档脏改动。

交付 `out/dma-cache-sync-20260916/delivery/`，payload SHA256 `6f52ad85c4d2fc87e97301d98f1394216b843cd8be601b2965eca429caf74053`；沿用SC.W修复BIT，未重建FPGA。**本轮缓存同步版尚未上板，数据故障未宣布解决。** 下一步更新SD软件，按交付说明覆盖两种启动/调用顺序的完整smoke；再做HPM校准，不能混用修复前后性能数据。以下诊断和上板准备状态为历史记录。

## 2026-09-16 实板 DMA smoke 失败与诊断

用户提供的后续记录确认 SC.W 修复 BIT 已启动到 Linux，登录/proc 检查正常、taint=0；DMA 功能验收仍失败。原 smoke 缺 BusyBox timeout，在提交前退出127；临时兼容脚本只跑两例，DMA0 单页8次 warmup0通过、warmup32数据校验EILSEQ，其余12例未执行。收到的是摘要，原始串口和JSON尚未传到本服务器。

定向核查、最小首错/缓存探针和后续诊断交付见 [DMA smoke 修正记录](../DMA_SMOKE_FIX.md)。源码查到缓存一致性契约缺口，但还没有把它确认为此次失败的唯一根因；未将主机测试、诊断探针或此前SC.W修复解释为DMA数据问题已解决。

最小诊断 Linux `53d3113dee74`、顶层构建 `38774213f7cb8f3ea4240e6e127e363ae5f6be02` 已完成软件构建；16 个调度/诊断场景、96 小 chunk 组合、9 项 watchdog、5 项 rootfs 检查通过。交付 `out/dma-smoke-debug-20260916/delivery/`，内容/架构/SHA256 核查通过；旧交付保留，新版尚未上板。源码组件全部固定，已有顶层文档 dirty 状态保留并记入 manifest。

## 2026-09-15：更新软件交付

- `make software JOBS=12` 退出 0；已更新 `out/experiment-v1/` 的 Image、rootfs、OpenSBI payload、用户程序、smoke、manifest 和版本/SHA256 清单，纳入小 chunk 修复及多页部分覆盖 smoke。
- 源码：顶层 `0bb9851bd12fc054bc40d217756d6f0b3662badb`，Linux `031e91dbadc115dafac957067135002b74035a10`。顶层已有研究文档改动，dirty 状态保存在构建清单中。
- 旧交付完整保留在 `out/experiment-v1-before-20260915T140327Z/`。本次日志 `out-experiment-software-20260915.log` 仅保存，按用户要求未逐项审查；既有主机测试未重跑，未重建 FPGA 或上板。

## 2026-09-09：最小软件修复

- 按用户要求仅修改数据模式，使用唯一 DMA 实例编号标记每个字节，修复小 chunk 短试次漏检另一设备源的问题。未扩展 ABI、CLI、调度或 HPM；RTL 未改。
- `python3 tests/test_idma_scheduler.py`：原 12 个调度场景和新增 96 个正常/错源组合全部通过。组合为 chunk=1/2/4/8 × DID 10～13 的 12 个不同有序配对 × 正常/错源；未修代码先出现 36 项预期失败。日志：`/tmp/idma-small-chunk-regression-{red,green}.log`。
- 现有 `board-smoke.sh` 每路只加一条多页命令：8 页中前 4 页各拷贝一次，余下 4 页保持未触及并接受校验。`sh -n tests/board-smoke.sh` 与 diff 检查通过；板上尚未执行。
- 驱动目标交叉编译通过：`make -C linux ARCH=riscv O=/home/xvhaocheng/work/iommu-platform/out/linux CROSS_COMPILE=riscv64-linux-gnu- -j4 drivers/misc/idma.o`。日志：`/tmp/idma-minimal-fix-build.log`。
- Linux 修复提交 `031e91dbadc115dafac957067135002b74035a10`；顶层测试/gitlink/lock 提交 `0bb9851`，均仅保存在本地。原研究文档继续保留。
- **当日仅完成目标编译；完整软件交付已于 2026-09-15 更新，见上一节。** 以上验证不代表真实 DMA/MMIO、板上功能或机制证据。HPM 相邻事件的 RTL 问题仍待修。

## 2026-09-09：实验计划与 v1 实现复审

- CLI 65 组、生产调度 12 场景、HPM 会话 25 组主机检查重跑通过。
- 新增生产函数主机注入：无预热、4 字节短试次交换两路 DMA 模拟源缓冲，实际校验仍返回成功；8 字节对照检出 EILSEQ。说明小 chunk 的模式归属校验有盲区，不否定默认完整 4 KiB 对该错源的检测。
- 新增实际 HPM RTL 定向仿真：UT_REQ→DDTW 连续 valid，DDTW 得 0/预期 1；加低 valid 间隔对照得 1。事件序列经翻译 FSM 静态核查，未进行完整翻译逻辑集成仿真。当前 v1 六槽不含 DDTW，不能推断它们全部计错。
- 审查阶段两项问题未修生产代码（后续软件修复见上一节）；复现程序的退出 0 表示成功复现缺陷/对照，不是生产正确性通过。源文件、结果、版本与 SHA256 见 [审查证据](../out/review-v1-20260909/README.md)，详细影响和建议见 [实现复审](experiment-v1-review.md)。
- 文档补入方法工作包、明确 E2/P2 依赖和 HPM 字段语义。Linux/CVA6/IOMMU HEAD 未变且与 lock 一致、工作树干净。未执行完整软件/FPGA 构建或上板实验。

## 2026-09-09：研究计划独立审查

完成[投稿导向审查](research-review-2026-09-09.md)，仅新增研究记录并更新交接。定向核对 DMA 分配/调度/预热、IOTLB 与事务控制状态机；顶层 HEAD 仍为 `25e452a7861f2208056180fb1faa7b9c0f5c1606`，Linux/CVA6/IOMMU HEAD 与 lock 一致、组件工作树干净。保留已有未跟踪研究文档。未改源码、运行构建/仿真或采集板上数据；以下既有验证边界与待办不变。

## 本轮：双路实验入口 v1

已用版本化 `idma_experiment` 替换四个旧 benchmark，修改 Linux 的测量调度与 per-DID HPM 会话；CVA6/IOMMU RTL 未修改。实现和指标解释见 [实验入口 v1](experiment-v1.md)。

- 主机测试通过：CLI 65 组、生产调度 12 场景、生产 HPM 控制 25 组、rootfs 4 项。
- 定向内核交叉编译通过；生产驱动/新 HPM 文件样式检查及 diff 检查通过。
- HPM 主机测试检出并验证修复两个索引宏的运算优先级错误，避免 Aggressor PTW 读取错误寄存器。
- `make check` 和 `make software JOBS=12` 通过。构建起止均为干净顶层提交 `ffe55a40175937122b3d6be92a6c005099f16dfd`，Linux 为 `affbfbf6adda41d5b33f7b36880279d8ee8f48f7`；21 个组件、gitlink 和 lock 一致。
- 新程序交叉编译、rootfs 内容/去重检查、Linux Image、DTB、lloader 与 OpenSBI payload 全部构建成功。DTC 保留原有 interrupt provider 缺少 `#address-cells` 的警告，本轮未修改 DTS。
- [软件交付目录说明](../out/experiment-v1/README.md) 与 [版本和 SHA256 清单](../out/experiment-v1/delivery.json) 已生成；目录内 `sha256sum -c SHA256SUMS` 全部通过。新软件 manifest 为 `out/manifests/software-migration-4dma.json`，日志为 `out-experiment-software.log`。
- 此前镜像和旧可执行文件保存在 `out/pre-experiment-v1-20260908T124529Z/`，带 SHA256 清单。原 BIT/MCS 哈希与历史清单一致，本轮没有重新构建 FPGA。
- 用户确认当前主机未连接 FPGA，也尚未准备 SD 卡。新版 smoke、per-DID 端到端校准和正式实验均未上板执行；继续使用原来的 SD 启动方向，实际介质布局待准备时确定。

以下为本轮替换前的平台迁移验证记录。旧软件 manifest、旧 `out/migration-4dma/delivery.json` 不代表新版软件；其中 FPGA 报告及 BIT/MCS 仍保留。

## 历史：平台迁移已完成

- 原工程 22 个已初始化仓库（包括旧 demo 顶层）的 Git bundle 已验证，另存 staged/working diff、未跟踪文件、关键旧构建产物及 SHA256。
- 新平台固定 21 个组件；所有组件工作树干净，各级 gitlink 与 lock 对应；没有 Git alternates 或指向旧目录的 origin。
- 当时四个用户态程序已交叉编译：idma_bench、idma_pair_bench、idma_multi_bench、idma_once（本轮已退役）。
- rootfs 打包成功并校验新程序内容，保留原有设备节点，无需 root 解包。
- Linux Image、DTB、lloader、OpenSBI fw_payload.bin 已从新目录完整构建成功。
- 实际 HPM RTL 的定向测试通过：pulse、held-valid、idle、DID filter、inhibit；独立 cycles 正常增长。
- 同一个 HPM 测试应用于旧 RTL，单脉冲后空闲检查失败：得到 10，预期 1。证明该测试能够识别旧问题。
- 新平台的 bundle 已在 `../platform-archives/restore-check` 完整离线恢复，并通过全部 21 个组件、各级 gitlink、工作树及 Git alternates 检查。
- Genesys 2 板卡定义已随源码保存，来源提交及文件哈希见 `configs/boards/provenance.json`。
- 仓库内的板卡定义已用 Vivado 创建内存项目验证，正确识别 `digilentinc.com:genesys2:part0:1.1`。
- FPGA 的 8 个 IP、主体综合、布局和布线成功，新 BIT（约 11 MiB）和 MCS（约 31 MiB）已生成，完整 `make fpga` 返回 0。
- 本次 FPGA 构建树的 2,332 个 Git 跟踪源码 blob 与锁定提交逐项一致，记录在 `out/migration-4dma/staged-source-audit.json`。

## FPGA 报告

`migration-4dma`，Genesys 2 / xc7k325tffg900-2，Vivado 2023.1。布线后的正式 timing summary：

| 项目 | 结果 |
|---|---:|
| WNS / TNS | 0.177 ns / 0 |
| WHS / THS | 0.038 ns / 0 |
| WPWS / TPWS | 0.062 ns / 0 |
| 未约束的内部端点 | 0 |
| LUT（布局后） | 132,584 / 203,800，65.06% |
| Slice（布局后） | 47,974 / 50,950，94.16% |

Vivado 判定当前已设置的时序约束全部满足。这不等于约束完整性或全部 CDC 检查通过：

- check_timing 仍报告 21 个输入缺少 input delay、23 个输出缺少 output delay、3 个部分 input delay。
- methodology 报告 6 项 Critical Warning：TIMING-4 ×1、TIMING-6 ×2、TIMING-7 ×2、TIMING-27 ×1，涉及内部时钟重定义、相关时钟没有共同主时钟／共同节点。需单独审查原有时钟约束，不能仅凭正 WNS 宣布完整时序签核。
- 综合时 `clk_out2_xlnx_clk_gen` 查询为空的两条警告，在完整网表中对应时钟及异步分组已存在；见另存的 `out/migration-4dma/reports-synthesis/ariane.clock_interaction.rpt`。
- 上游清单还生成了当前 RV64 配置未实例化的两个 Debug Module 宽度转换 IP（`xlnx_axi_dwidth_converter_dm_master/slave`，仅在 `XLEN == 32` 时使用，不是 iDMA）；其 XDC 没有作用于任何实例的警告已保留，未通过降级警告隐藏。

详细布线报告位于 `out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/`，与 BIT/MCS 一起保存。上游 MCS 步骤重新创建 Vivado 项目，原来的 `ariane.runs/impl_1` 不作为归档位置。

## 待完成

- 原有时钟定义、CDC 与外部端口约束审查，具体项目见上述 FPGA 报告。
- bitstream 上板、四路 Solo、多设备并发、DID-filtered HPM 端到端校准：尚未执行。新版入口先运行新的 `platform-smoke`；大描述符回归暂不由这个只支持页内 chunk 的入口覆盖。
- 无人为延迟 PROFILE 的 FPGA 构建与性能实验：尚未执行。

完整许可证地址 `2100@opus4.apal.cs.waseda.ac.jp` 已验证可直接使用。

编译成功不证明硬件功能通过，也不替代约束完整性审查。完成上述审查并保存新 bitstream 对应的板上测试日志后，才能给平台标记为 validated baseline。

详细输出位于 out/manifests、out-hpm.log、out-software.log、out-fpga.log。软件已在干净的顶层提交 `de942152a8e7` 上重复增量构建通过；以实际 manifest 完整提交号为准。本次 FPGA 启动于平台整理期间，旧版入口的 manifest 中顶层状态反映结束时状态；固定组件与实际导出源码另有逐项校验记录。启动时使用本机板卡定义，其内容与随后保存到 configs/boards 的文件相同；新版入口显式使用仓库中的板卡定义并记录启动状态。主体逻辑未在构建中修改。
