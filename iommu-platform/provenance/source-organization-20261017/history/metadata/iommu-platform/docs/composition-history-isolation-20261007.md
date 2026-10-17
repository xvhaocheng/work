# 共享替换历史隔离：实板结果与下一步（2026-10-07）

**隔离后，P/PN 的额外未命中次数差消失，双方绝对耗时降低约21%；但 PN 仍比 P 慢12.67%～12.79%。** 22项实板实验全部通过；剩余退化的根因与论文创新尚未确定。

最新小对照见§9：同一BIT/启动的18项板测通过，gap0短段中PN让V耗时增加12.85%、A有效速度下降11.39%；供给间隔改变了退化幅度，尚未证明板端具体等待链。

## 1. 生产改动与范围

生产 RTL 已有改动，集中在 IOTLB 与 TL 包装层：新增分区局部的 PLRU touch，固定 ways0/1 与 ways2..15；touch 归属取常驻命中路，不用 `policy_mask_i`。全掩码 S/N 逻辑不变。Linux/SD 未更新。

启动与加载：Linux 从 SD 启动；JTAG 只加载 BIT；以太网用于 SSH 控制与身份识别，不承担网络启动。新启动的 boot_id 为 `d61e7517-2b15-4702-86f2-3f338ddd5345`，内核仍为 cpn3-clean 同版本，本轮重载产生了新的启动标识。

定义：P 为容量分区，N 为命中旁路，合称 PN；W 为 V 的测量窗口周期数，时钟 50 MHz。

来源：[production.patch](../out/composition-isolated-20261007/production.patch)、[board-acceptance.json](../out/composition-isolated-20261007/board-acceptance.json)。

## 2. 构建、测试与板级验收

原生构建 WNS -0.219 ns，一项端点未过；沿用已有的一次布线后优化后，最终 WNS +0.022 ns、WHS +0.051 ns，建立、保持和脉宽时序失败以及布线错误均为 0。资源 LUT 156186、FF 127714、Slice 50556/50950。

原 IOTLB 回归的 25834 次求值检查通过，TL 回归通过；新增隔离测试的 9232 次求值检查通过。这些数字是检查次数，不是板测用例数。板级 22/22 通过，其中 4 个短例、18 个性能例，failed=0；22 个用例由原 runner 执行内置校验，Codex 回传后按 `composition_plan.validate_case`（含 meta）逐例执行并验收。原始记录汇总见 [summary.json](../out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/summary.json)。

## 3. A8 gap0 对比（旧 clean 对新 isolated）

核心结果如下。旧侧来自旧 clean，新侧来自 isolated 实际原始记录。W 以 50 MHz 计，新 P 的 W 约 703 万周期，约 140.7 ms；新 PN 的 W 约 793 万周期，约 158.6 ms，额外约 18 ms。

| 组 | 旧 A 未命中次数 | 新 A 未命中次数 | 旧 W 范围 | 新 W 范围 | PN 相对 P 放慢幅度 |
|---|---:|---:|---:|---:|---:|
| P | 5117 | 2048 | 8960484–8994909 | 7032355–7039655 | — |
| PN | 6582 | 2048 | 10001202–10006893 | 7931144–7931945 | 旧 +11.2200%–+11.6355%；新 +12.6744%–+12.7922% |

旧 A 未命中次数差为 +1465，新双方均为 2048，差为 0。新 P 的 V 普通请求 16384、V 快速请求 0、V 未命中 0；新 PN 的 V 普通请求 12288、V 快速请求 4096、V 未命中 0，总 V 请求均为 16384。需要明确：A 未命中次数相同只表示数量相同，不表示访问页相同、请求顺序相同、PTW 开销相同或请求时间线相同。新 A 的普通查找数为 16385～16386，部分 P/PN 配对相差 1；A 窗口完成数均为 4096，但窗口长度不同，因此不能写成吞吐相同或速度不变。

样本量 n=3，仅作描述性对比；跨 BIT、跨启动的前后比较仍有混杂，S/N 对照变化在 0.28% 内。

来源：[comparison.json](../out/composition-isolated-20261007/comparison.json)；原始记录例：[b1 a8 mp record](../out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mp/record.json)、[b1 a8 mpn record](../out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mpn/record.json)。

## 4. 解释边界

隔离使 PN 相对 P 的额外 A 未命中次数差从 +1465 降到 0，并改善双方绝对 W：P 下降约 21.44% 至 21.82%，PN 下降约 20.69% 至 20.74%。剩余 PN 放慢不能只用未命中次数差解释。这里不做显著性主张，也不写“隔离失败”或“回归已修复”。更早的 PTW 修复版有独立历史记录；本表基线使用最新 clean 版，未混入旧版数据。

整板 I/O/CDC 完整签核尚未完成。剩余 PN 放慢的因果归因仍未完成，等未命中次数下约 0.90M 周期差额来源尚未拆开。

## 5. 下一步最小任务

取合法实际 DMA/TL 事件序列，解释等未命中次数下新增约 0.90M 周期：区分本实现侧额外气泡、仲裁与响应缓冲，与真实服务顺序、不同未命中请求、PTW 开销、数据路径竞争。先定位这次差额，同时核对真实工作流和强普通基线。只有明确了现有方法缺少的信息或未处理的约束，才提出小机制并验证收益；剩余耗时差本身不构成创新。论文主线仍是保护关键 DMA 的完成时间，同时保留后台吞吐；固定 A8 的 p5 方案已有负结果，不据此重新立项，PLRU 隔离也不单独作为论文贡献。

创新审查见 [novelty-gate-20261006.md](novelty-gate-20261006.md)；脚本引用见 [composition_plan.py](../scripts/composition_plan.py)。

## 6. 来源与历史口径

旧侧数据来自旧 clean 同一次启动完成的 172 条记录，按相同参数筛出其中 18 例，保留原 ID 与顺序；新侧数据来自 isolated 实际原始记录，位于 `out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/`。comparison、acceptance 与 summary 均从这些原始记录汇总，生产补丁单独列出。板级验收状态为 passed、count=22、failed=0，检查项为 `composition_plan.validate_case`（含 meta）逐例通过，详见 [board-acceptance.json](../out/composition-isolated-20261007/board-acceptance.json)。

作者与边界：DeepSeek 起草提案与文档；Codex 复核关键 RTL、应用改动、执行测试、完成构建与上板验收。

## 7. PN 算法源码审核与定向时序检查

2026-10-07 用户明确要求 DeepSeek、Codex、Claude 联合审核 PN 是否有算法或实现错误。审核针对当前 isolated BIT；10份关键生产 RTL 与其构建副本逐字一致。此轮没有修改生产 RTL/Linux，没有 FPGA 编译、写卡、重启或新增板测。完整模型回复及证据保存在 [审核目录](../out/pn-algorithm-audit-20261007/)；最终状态以该目录的 `audit-summary.json` 为准。

**当前没有确认能解释 PN 比 P 多约 896482 周期的具体 bug；这也不是算法无缺陷的证明。** P 的填充分区归属、隔离后的 PLRU touch、N 的稳定请求/上下文检查和实际响应握手尚未发现能直接解释差额的错误。三个需要纠正的怀疑如下：

- `NonblockEnable` 是编译参数，composition 中四个运行模式都为1。P/PN共有主请求门控，不能把它当作 PN 独有的取消预取；运行差异在 `n_intercept`。
- 现有 TL 测试已检查快速响应持续反压。实际 bank 接受翻译请求后在 `AWAIT_RESP` 无条件 ready，测试台任意拉低 ready 不是已经证明可达的生产等待。
- Claude 的条件性 owner 过时反例要求旧 bank 的 W 尚未结束便接受另一 bank 的 AW。实际 `axi_demux_simple.sv:175–177` 禁止该顺序，输入 `SpillAw=0`；所用 vendor 与 BIT 构建副本一致。该具体反例不成立，不据此修改 owner 逻辑。

Codex 复用已有、输入文件未变更的 Verilated TL 模型，仅编译一个审计用 C++ 驱动，检查 hot/cold V、13个到达相位、P/PN共52个局部场景。固定 A 最后一层 PTE 返回时刻，A 的响应相对 P 最多晚1周期；hot V 在实际快速接受时提前4–8周期，其他相位相同；cold V 仅一个相位晚1周期。未发现漏接收、重复响应或重复填充，填充分区检查通过。[测试代码](../out/pn-algorithm-audit-20261007/late_probe.cpp)／[结果](../out/pn-algorithm-audit-20261007/late-probe-summary.json)。这只证明所枚举的单对 TL 场景，不是整个 DMA/DDR 路径的性能上界；不接受把粗略局部拍数预算写成板级形式证明。

复算已有三组板测记录得到以下计数差，未新增采样：[窗口计数归约](../out/pn-algorithm-audit-20261007/window-decomposition.json)。

| 指标（周期，三次平均） | P | PN | PN−P |
|---|---:|---:|---:|
| V 窗口 W | 7035178 | 7931659.67 | +896481.67 |
| V 硬件执行阶段 | 7031083 | 7927564.67 | +896481.67 |
| V 任务补充阶段 | 4095 | 4095 | 0 |
| V 软件轮询阶段 | 0 | 0 | 0 |
| V、A 同时 pending | 7018795 | 7915276.67 | +896481.67 |
| IOMMU 入口 AW valid 且不 ready | 6796732 | 7660220 | +863488 |

这些计数相互重叠，不能相加，也不能把 AW 等待增加直接认定为根因。它们将差额定位在已有硬件执行窗口内，支持优先核查翻译提前返回之后的实际 DMA 读写服务顺序、PTE 读与数据突发的竞争、AW/W 队列持有时间。完整系统轨迹尚缺，不能排除其他 TL 行为。当前 BIT 的 DS 固定输入/输出延迟各50周期，后续复现必须保留这一条件。

下一最小步骤是复用 `tests/nonblock_actual_dma*` 的真实 iDMA、banked controller 和 TL 联调入口，核对固定2/14分区及运行模式的接入后，仅对小段 P/PN 实际事件序列做归因；现有 CLI 仅开放 s/n、默认关闭 policy，不能声称旧联调已覆盖这次 P/PN。先找额外等待的合法来源，再决定是否改 RTL，不重跑全量板测矩阵。DeepSeek提供首轮证据及定向修订，Claude提供独立意见和交叉审定，Codex负责关键源码复核、否证候选和实际仿真执行。


## 8. 真实 DMA 小段复现与 DDR 因果对照

2026-10-07，本节承接§7。**仿真已找到一条合法机制：N 提前放行 V 的数据读，使 A 的页表读在共享 DDR 返回队列里等待更久。板上约12.74%的差额尚未完成归因。** 未修改生产 RTL/Linux，未重编 FPGA、上板或写卡；10份关键源仍与 isolated BIT 构建副本一致。

复用现有 `nonblock_actual_dma`：真实两 iDMA、banked controller、TL、DS_if、固定50/50 delayer与共享xbar；增加 p/pn运行选项，policy编译模型只构建一次，后续复用。A8、V gap0、V64、warmup256，DDR模型额外读延迟31，合成CPU读参与竞争；这不是 MIG/CVA6/Linux 的完整时序模型。窗口以实际 V 第一次 issue 和第64次 done 划定，统计 `(open,close]`，不包含软件轮询延长或预热/排空。

| DDR 返回策略（仅模型） | P：W周期 | PN：W周期 | PN相对P |
|---|---:|---:|---:|
| 原有 FIFO | 91878 | 93480 | 慢1.744% |
| 窗口内 DS 读优先 | 79847 | 79484 | 快0.455% |

四组数据检查、排空与独立完成计数均通过：各64 V、64 A；普通未命中32、A填充31，窗口末仍可能有正在进行的walk；PN快速路径分别62/14次，P均0。DS优先干预只改变C++ DDR的跨ID返回顺序，保留同ID顺序及被反压时的VALID数据，不改变RTL或预热。它用于检验因果，未作为生产修复或创新方案。

基线的93次DDR DS读，其地址与顺序逐项一致。P→PN服务延迟总和10206→15711，增量5505；PTW active也增加5505（20726→26231）。入口AW等待89298→90900，全为跨bank前笔W尚未结束，差1602与W差相同。不同计数重叠，不能相加。DS优先时，两种模式的93次DS读延迟总和均2976、每次32周期，PTW active均13489，PN退化消失。

直接事件例（周期相对窗口开点）：PN的V快响应在2758/2764；V数据读2763进入DDR（ID32、0x80000000、256拍），A叶子PTE读2766进入（ID48、0x70010、1拍），PTE到3100才返回，耗334周期，3156填充。P中该PTE在2763进入、2808返回，耗45周期、2864填充，V数据读到2876才进入。说明更早取得翻译并不保证组合后完成更早，数据服务可能推迟正在进行的翻译。

证据：[四组原始归约](../out/pn-data-path-20261007/comparison.json)、[完整地址序列与延迟](../out/pn-data-path-20261007/trace-evidence.json)、[P事件](../out/pn-data-path-20261007/p-full-events.csv)、[PN事件](../out/pn-data-path-20261007/pn-full-events.csv)、[验收](../out/pn-data-path-20261007/acceptance.json)、[关键源一致性](../out/pn-data-path-20261007/source-provenance.json)。完整模型回复、日志与接口patch在同目录。早先16k周期截取轨迹已补为完整窗口，两次基线W保持一致。

边界：仅证明此模型与工作负载可达；板上PN慢12.74%，本模型慢1.744%，尚不能解释全部差额。DDR优先干预也改变了合成CPU实际读次数，不能写成所有CPU因素已隔离，或已发现确定算法bug。下一步按板上实际返回路径核对PTE/数据服务顺序，选择小反事实对照；不自动增加占资源的诊断硬件或重跑172项。论文仍需强普通基线和明确增量，当前发现不直接算创新。

复用命令：首次 `python3 tests/test_nonblock_actual_dma.py --system --mode p --a-pages 8 --v-gap 0 --transfers 64 --warmup 256 --ptw-latency 31`；随后直接用 `out/nonblock-actual-dma/obj-policy-system/Vnonblock_actual_dma_system_top +mode=pn +a-pages=8 +v-gap=0 +transfers=64 +warmup=256 +ptw-latency=31 +a-target=1048576 +system=1`。增加 `+ds-priority=1` 仅作模型干预，`+event-trace=路径` 导出窗口CSV。

DeepSeek提出运行接口patch和证据报告；Codex修订硬件窗口、选择测试观测、实现DDR因果干预，复核关键源并实际执行构建/仿真与验收。此阶段未追加Claude调用。

## 9. 同一 BIT 与启动下的 V 供给间隔板测

2026-10-07承接§8，复用当前isolated BIT与既有Mac SSH runner。**18/18全部通过；gap0短段复现PN退化，V和A都变慢。增加V供给间隔后相对差距缩小，但这不是性能修复，也没有直接证明板上的PTE等待原因。**

运行03:52:54–03:55:34 UTC，约2分40秒，run_id `c7bdaec923b3815c`；内核仍为`6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty`，boot_id始终为`d61e7517-2b15-4702-86f2-3f338ddd5345`。原计划按既定顺序筛选A8、P/PN、V gap0/1000/4000、三个block，只把测量V拷贝数4096改为64；每次4KiB、warmup256、A gap0、原超时与校验不变。新包装器沿用原`case_argv`、`validate_case`和`case_metrics`，没有改生产RTL/Linux。

全部18条原始record按现成`composition_plan.validate_case(case, record, meta)`验收，exit0、problems为空、V/A数据错误0、restore_status0、taint前后0、启动不变；阶段dmesg新增72条正常观测/start日志，未见新内核告警。没有FPGA构建、JTAG加载、SD写入、重启或打开UART。完整原始目录已回传，Linux保持运行。

W为V的测量窗口**周期数**，50MHz，包含刻意增加的gap。每样本A有效速度为`A窗口完成数 × 50,000,000 / W`，然后取三次均值；百分比为同gap的组均值之比，不是配对百分比的均值。V完成数各64。

| V gap（周期） | P平均W（周期） | PN平均W（周期） | PN的V耗时变化 | P的A平均速度（拷贝/s） | PN的A平均速度（拷贝/s） | PN的A速度变化 | PN快速翻译请求数（范围） |
|---|---:|---:|---:|---:|---:|---:|---:|
| 0 | 109839.3 | 123956.7 | +12.85% | 29133.55 | 25815.51 | −11.39% | 64 |
| 1000 | 171739.7 | 178143.7 | +3.73% | 36974.57 | 35551.88 | −3.85% | 110–111 |
| 4000 | 327404.7 | 327658.3 | +0.08% | 48207.52 | 48220.97 | +0.03% | 5–8 |

同block配对的V耗时变化范围分别为+12.54%～+13.19%、+3.13%～+4.06%、−0.27%～+0.32%。第三组没有一致变化方向；n=3仅作描述性结果，不作显著性主张。

gap0的P/PN各完成64次A拷贝，A miss和fill均32，V miss均0；因此PN的V变慢并没有换来更高A吞吐。gap1000时PN快速翻译次数增加，V仍比P慢3.73%，A速度仍低3.85%。gap4000中N仅触发5–8次，不能声称N保持同强度生效而退化被消除。跨gap的A窗口完成数从64增到126–127、315–316，A miss从32增到67、162；供给间隔同时改变了A工作量、翻译时序及下游竞争。

增加gap后，V绝对W从约11万增到17万、33万周期；缩小P/PN相对差距不能当作加速。实际额外V gap累计为0、63000、252000周期；不能用W减去gap冒充独立服务延迟。IOMMU入口AW反压均值PN−P分别为13589、6510.3、253.7周期，仅是定位线索。这些正规计数来自同一个`(open,close]`窗口，条件/事件可重叠，不能当作互斥耗时相加。

证据边界：当前V9没有PTW占有或页表AR/R独立时序计数；`sharedAWwait`只统计tr入口`valid && !ready`。板测支持退化对V供给节奏敏感，与§8的下游竞争假设相容，不能单独证明MIG返回FIFO、PTE被数据突发阻塞，也不能定量解释此前4096拷贝的约12.74%差额。同miss数不代表同地址、顺序或服务代价。尚无确定算法bug或新论文机制结论。

下一步不重复gap矩阵：在既有真实DMA仿真中，判别“保留N提前翻译，单独约束V物理数据读进入共享服务的时机”。先试A的PTW活跃时暂缓新V数据读，保持合法AXI握手和已发请求的进度，检查数据正确、排空、N确实仍触发，以及gap0下P/PN的V完成时间与A速度。这个试验只用来决定是否值得做最小RTL改动；普通页表优先/准入已有前史，不能直接包装为创新。尚未实施该候选或修改生产RTL。

证据：[数值归约](../out/pn-board-path-20261007/reduction.json)、[逐条验收](../out/pn-board-path-20261007/board-acceptance.json)、[冻结计划](../out/pn-board-path-20261007/reviewed-plan.json)、[完整原始记录](../out/pn-board-path-20261007/mac-records/pn-gap64-20261007-formal01/)、[运行日志](../out/pn-board-path-20261007/mac-run.log)。包装器为[mac_pn_gap_probe.py](../out/pn-board-path-20261007/mac_pn_gap_probe.py)，Mac入口在`tools/composition-isolated-20261007/`。

DeepSeek负责接口证据、包装器和归约/报告提案；Codex直接核关键计数及调用链，应用并执行板测、回传和最终验收。一次定向修订引入W单位及A百分比错误，未采纳；最终表按原始记录核算，审定保存在`reduction.json.review`。完整模型回复保留在同目录，此阶段未追加Claude调用。

## 10. PN组合收益与下游干扰的相关研究

2026-10-07用户追问：PN应比P/N领先，现有仿真接近持平是否有相关研究？定向复用现有文献地图，不重做全领域检索。**已有直接研究翻译/数据请求干扰及优化组合收益有限的前作；尚未确认其中哪篇覆盖本平台固定2/14分区＋PTW忙时跨bank命中旁路＋共享AW/W约束的完整组合。** 没检到相同组合不代表无人做过。

| 研究与原文 | 已核实的相关发现 | 对当前PN的关系与边界 |
|---|---|---|
| [MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency，ASPLOS 2018](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) | §5.4明确区分数据干扰翻译与应用数据相互干扰；采用Golden/Silver/Normal三队列，让页表请求优先，同时按翻译阻塞情况调度数据。§7.2分解TLB、cache、DRAM机制。 | 下游服务顺序是直接近邻，页表优先已有强前史。MASK的TLB-fill token和L2 cache bypass不是本平台N的跨bank命中提前响应，不能等同。 |
| [Improving GPU Multi-tenancy with Page Walk Stealing，HPCA 2021（DWS）](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) | §VII-D/Fig.11评估MASK+DWS，明确组合可能不比单用增加显著收益；讨论有限walker、page-walk cache和不同TLB容量使MASK收益比原文小。§VII-B说明walker份额也影响TLB占用。 | 直接支持“作用于不同位置的优化不保证叠加”。不是P+N组合变慢的复现，也不能用它量化本平台13%的退化。 |
| [Scheduling Page Table Walks for Irregular GPU Applications，ISCA 2018](https://www.csa.iisc.ac.in/~arkapravab/papers/GPU_page_walk_scheduler_ISCA_18.pdf) | 引言与调度分析表明，walk服务顺序影响完成进度；相对FCFS，所提调度平均加速30%，朴素随机调度可使应用变慢26%。 | 说明同样的翻译工作量也会因顺序不同而有不同完成时间；对象为GPU指令/warp的翻译批次，不是DMA数据与页表的同一队列实验。 |
| [Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs，DAC 2023](https://libingyao.github.io/assets/files/DAC2023.pdf) | §V/Fig.10：简单TLB分区让平均执行时间增加14.3%；协调调度和动态set sharing等完整方案平均减少12.5%。nw中命中率提高未转为性能提高，作者解释为计算瓶颈及延迟隐藏。 | 分区＋调度已有直接前作，改善翻译指标不保证整体加速也有实例；其分区是GPU TBid映射，负结果原因是容量减少，不能套作本平台miss相同的解释。 |
| [Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems，RTCSA 2025](https://arxiv.org/pdf/2508.19670) | §III在ZCU104/Arm MMU-500 SMMUv2上实测多DMA共享IOTLB干扰，部分小事务延迟最高1.79倍。§VI-C明确未完整实验评估缓解方法。 | 真实DMA/IOMMU平台近邻，可参考实验与时延口径；未证明我们的共享AW/W等待链，亦未测试本平台P/N/PN。 |

原文核对：MASK PDF第8页§5.4、第11页§7.2；DWS本地作者PDF第11页§VII-D、[文本831–864行](../out/shared-translation-literature-20261006/papers/walk_stealing2021.txt)（本次网页全文读取超时，但本地原始论文可核）；Scheduling PTW PDF第1页及调度分析；DAC作者PDF第5页§V；Bermuda作者PDF第3页平台与第9页缓解讨论。数字全部是相应论文自身评估，不能与我们的板测或仿真百分比直接比较。

最新近邻：[NB-Walker: Non-blocking Page Table Walker to Enhance Address Translation in NUMA GPUs](https://www.microarch.org/micro59/program/)已列入MICRO 2026正式议程，本次另核到[作者公开artifact](https://github.com/redbird-arch/micro2026-nbwalker-artifact)。本地此前只有题录；这次也未获得论文全文，README不足以判定其与N的机制关系或性能。不能根据“non-blocking”题名声称我们的N相同或不同。另有同场次contention-aware translation offloading题目，仅作待核线索，未纳入已读机制对照。

对本平台的研究判断：翻译与数据联合调度、页表优先、分区与调度组合都已有研究。接下来仍按§9做最小因果判别；若寻求论文增量，应对照这些普通基线，证明DMA读写依赖和共享事务约束具体留下了什么问题，以及小改动能否同时改善V完成时间和A吞吐。当前没有据此确认创新，也没有补齐同条件N仿真，不能说PN已与N持平。

DeepSeek从已有笔记筛选6篇并明确原文待核问题；Codex浏览作者/出版原始资料，直接核DWS关键组合段落及其他论文对应机制，审定上述对照。[完整回复和任务](../out/pn-related-work-20261007/)保留；网络受限首次调用无答复，一次授权网络恢复完整返回87282 provider tokens。无Claude调用、生产改动、编译、仿真或板卡操作。

## 11. 第三项瓶颈的现成解法与 PN 创新验收条件

用户指出前两项可以改变工作集，追问第三项解法及“PN胜P/N是否就能算创新”。这里前两项是让P确实减少V未命中、让N遇到可绕过的翻译等待；第三项是翻译提前后仍被数据服务及AW/W事务依赖阻挡。工作集/到达节奏可以形成两项功能同时有用的条件，但应保留真实场景依据或预先确定的参数范围，不能仅挑对PN有利的点。当前尚未改变工作集、未补N同条件仿真，也没有新的性能结论。

| 解法 | 要减少的等待 | 当前平台状态 |
|---|---|---|
| 页表优先／数据读准入调度 | 长数据请求让页表读排队 | MASK已有；本平台仅做过模型DDR返回优先，真实MIG方案未实现。已进入FIFO的长突发不能仅靠入口优先级立即抢占。 |
| 缩短数据burst | 每次连续占用共享R/W的时间 | 当前前端仅有完全deburst到单拍的配置，不开放任意32/64拍；现成composition路径使用CONF0并限制参数。增加可控burst需要适配接口，须保总字节、页布局并统计额外请求。 |
| R→AW耦合，延后写地址直到数据开始到达 | 写事务早占owner、源翻译／读数据却尚未完成 | iDMA已有`idma_channel_coupler`，首拍R提供AW放行信用；但实际`RAW_COUPLING_AVAIL=0`，软件不能启用未综合的模块。与当前读准入的组合需先核合法时序，首拍不等于所有源依赖就绪。 |
| 分离缓冲／返回空间预留／有序owner队列 | 一个bank的依赖堵住另一个bank | 普通缓冲和信用办法已有前史；当前只有单owner标记及signed未完成计数，不是跨bank owner FIFO。若做多owner排队需要同时保证W路由、顺序与进度；不能只删除现有guard。 |

关键源码由Codex直接核：`cva6/corev_apu/iDMA/src/idma_channel_coupler.sv:105,140–179`首拍R与AW信用；`idma_backend.sv:811–858`编译条件/旁路；`frontends/register_64bit/idma_reg64_frontend.sv:366–373`耦合与单拍deburst；`fpga/src/ariane_peripherals_xilinx.sv:1269–1272`缓冲512拍、8个在途、RAWCoupling关闭；`riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:246–278`单owner与读准入；`cva6/vendor/pulp-platform/axi/src/axi_demux_simple.sv:175–177`跨bank AW门控。当前控制器本就AW优先（`rv_iommu_trans_ctl_wrap.sv:623–647`），不把再加AW优先当新改法。

协议边界：[Arm AXI规范](https://documentation-service.arm.com/static/67ab5a186dbc975ccea91734)规定AXI4不支持W数据交错，合并写事务后必须保持对应地址顺序。这不等于协议强制整个系统只能有一个在途事务；当前demux门控是防特定依赖的实现选择，替代设计需要保留合法路由与进度。现成coupler参考[官方iDMA项目](https://github.com/pulp-platform/iDMA)，以本地实际版本为准。另发现[2026年AXI NoC流量类别分离预印本](https://arxiv.org/abs/2607.01430)，仅核作者摘要：讨论端点读写协议依赖与多平面/VC资源代价；未全文核机制，不据此声称它解决当前banked IOMMU问题或建议重构NoC。

创新判断由Codex负责：**PN比P和N快是性能证据，不是单独的新颖性证明。** 合理增量论文可以采用已有构件，但应至少证明一个具体增量：新的协调机制；前作未覆盖且真实可达的约束；或可复用、有边界的因果结论。单纯同时开启原P/N、换到更有利工作集，贡献较弱。反过来，不要求所有构件原创，也不要求每个场景都赢。

对本题的可审查贡献应是：解释什么时候分区与命中旁路能互补、什么时候下游依赖抵消收益；若提出轻量协调，说明它怎样保留翻译提前收益，同时减轻写owner或页表/数据排队，并在同资源/同工作量下给出V完成时间与A吞吐代价。除了S/P/N/PN消融，纳入适用的普通页表优先、短burst或现成耦合基线；不把全部不相关的撤销/重放机制强行列成必测清单。资源和参数改动必须记账，功能错误不能当性能收益。实际是否超出最强近邻仍待定向前作核验和实验，不据此承诺发表。

DeepSeek提供现成模块/配置证据和答复草案，完整50280 provider tokens；Codex核关键源及Arm/论文原始资料并裁定边界。worker将当前owner计数当作owner队列及混淆gap4000耗时/速度百分比的部分未采纳；软件deburst的建议限定为当前前端单拍能力，未当作现成任意burst选项。[完整提案及审定](../out/pn-third-bottleneck-20261007/)保留。本轮仅讨论与证据核查，未改生产RTL/Linux、未构建或运行新仿真/板测；§9的下一小判别仍未实施。

## 12. 其他解法筛选：请求作用、完成依赖与页表访问削减

用户要求继续探索其他解决方法。筛选结果是四条不同的优化入口，**优先保留B作为研究候选、A作为需先证可达的备选；C/D作为工程对照。没有任何一条已通过创新或性能验收。** 把同一种数据准入移到另一个模块、改变阈值，不能自动成为新贡献。当前§9最小仿真尚未实施，本节补充它的候选与停止条件，不代表已选择生产实现。

| 方向 | 大白话与实际目标 | 平台可行性／最近前作 | 判断 |
|---|---|---|---|
| A：服务能释放当前占用者的请求 | 若V被A未结束的写事务挡住，先帮助A完成必要的源翻译／读／写排空，让V尽早进入；优先级随真实依赖改变 | bank标签、owner计数已有，但输入owner不等于下游owner，缺搬运依赖关联；单PTW不能中途抢占。BIS与Scheduling PTW已研究进度/关键性调度 | 工程跨度中至高；先找当前规则没覆盖的合法选择机会，无机会即停止 |
| B：按读写作用协调N与数据启动 | 写hit可能帮助W开路；读hit可能提前送入长数据突发。依据请求角色与完成依赖决定何时旁路、何时启动数据，而非一律越早越好 | N已有读写角色与bank；生产没有已验收的owner反馈策略或翻译完成／数据启动分离机制。MASK已有翻译/数据服务协调 | 首选小判别；潜在增量在具体依赖信息及低成本规则，尚非确认创新 |
| C：显式进度信息驱动动态burst | 存在翻译争用时减小下一笔尚未发出的数据burst，无争用时保留较大burst | legalizer有长度控制基础，前端只开放单拍deburst；需要接口与配对/信用审查。短burst、adaptive flow control属于普通近邻 | 工程跨度中；作为固定短burst的扩展对照，不能仅凭动态二字立题 |
| D：减少页表读本身 | 用小nonleaf/PTE缓存避免反复读上级页表，或另核独立片上页表通路 | 当前轨迹有大量重复上级项；缓存须处理失效，BRAM通路/软件页表放置未验。RISC-V规范已允许nonleaf缓存 | 普通基线；小缓存比重构NoC更值得先评估，但单独加缓存不算新颖 |

### 12.1 新增可核证据与接口边界

Codex只对已有轨迹作地址频数归约，没有重跑实验：P/PN的93个DDR DS读均只有6个不同PTE字地址，`0x30000`与`0x60800`各31次，四个叶PTE为8/8/8/7次。`tests/nonblock_actual_dma_test.cpp:407–414`明确前两者是A的两级非叶项；**62/93约66.7%是重复上级访问，但不是可承诺的周期节省比例**。冷启动、缓存命中代价、并发流与失效都需计入；这个静态小工作集不能外推真实应用。[归约与审定](../out/pn-alternatives-20261007/review.json)。

关键源码已由Codex直接核：

- `rv_iommu_banked_ctl.sv:246–278`的单owner读准入；`:372–408`的bank标签、翻译RR与响应路由。当前规则已偏向owner，仅再加owner优先未必有新机会；源／目的依赖还要真实关联，不能用“后台A”标签代替。
- `rv_iommu_trans_ctl_wrap.sv:625–647`本就AW优先，且`fullbuffer_job_i`与DID匹配可豁免普通读准入；`dma_core_wrap.sv:173–183`用真实job生成证书，`idma_fullbuffer_certificate.sv:16–18`限定单个对齐4KiB、足够缓冲与normal copy。**不能把当前所有等待归为读guard，也不能将已有完整缓冲证书作为新方案。** AW跨bank隔离仍由`axi_demux_simple.sv:175–177`保持。
- `rv_iommu_nonblock_hit.sv:94–107,143–172`已有读写权限判定、accept与touch；若只在accept加门控，拒绝会进入REJECT并等待原慢路径，不能称一位开关已足够。B应先定义尚未接收候选的选择与退回规则；已接受的OUT响应须完整保留。`rv_iommu_tl_wrap.sv:303–306`仅定义合法窗口，不提供收益判断。
- `rv_iommu_trans_ctl_wrap.sv:329–357,714–725`在翻译返回后发物理AR/AW，并等待下游握手才结束当前状态。保留提前翻译、暂缓物理数据需要明确准备／发出状态；若还想准备下一笔，需要额外元数据存储与失效处理，并非已有独立流水。
- `idma_backend.sv:612–625`是iDMA虚拟AR路径，不能当翻译完成后的物理AR出口；ready背压不能用来随意撤销已呈现的AXI VALID。`rv_iommu_ptw.sv:385–417`在PTE返回后判叶，现成active/ARvalid不能证明“下一拍必发叶项”。
- 改AXI burst长度、保留原4KiB job不会仅因分块就取消证书；若把frontend job改为2KiB则不再满足长度条件。读写分块、数据重组、last标记与信用必须一同核查，不能声称只需几bit或天然保序。

### 12.2 强前作对照及新颖性缺口

[Scheduling PTW，ISCA2018](https://www.csa.iisc.ac.in/~arkapravab/papers/GPU_page_walk_scheduler_ISCA_18.pdf)已依据指令完成所需工作量调度并批处理翻译；[BIS，ASPLOS2012](https://users.ece.cmu.edu/~omutlu/pub/bottleneck-identification-and-scheduling_asplos12.pdf)依据其他线程等待识别关键瓶颈。因此A的宽泛“帮助阻挡别人的工作”并不新。本轮BIS网页全文超时，原文检索及既有本地全文可核；没有把网页超时写成已取得新全文。

[MASK §5.4](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf)已有页表优先，并依据活跃walk与受阻warp安排应用数据服务，限制了B/C的宽泛“翻译感知”主张。可能值得检验的差别是**有限双端DMA中，命中请求的读写作用、W事务完成依赖与页表服务相结合**，是否比普通页表优先、固定短burst、现成R→AW耦合带来额外有效进度。现在只给出问题与接口，尚无新的完整规则、反例收益或全领域新颖性证明。

D的缓存构件已有：[RISC-V IOMMU规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html)明确提到缓存非叶页表项；[MMU-aware DMA，ICCD2018](https://arxiv.org/pdf/1808.09751)已有预取、受控TLB管理及burst失败重放，不能把普通提前准备重新包装为新机制。

本轮补核[2026年Preemptive VCs原文§III](https://arxiv.org/pdf/2607.01430)：在封装NoC流类别中允许受阻流让出共享物理链路，研究读写端点依赖与缓冲/布线代价；它不是直接在外部AXI口撤销VALID或抢占已接受的W序列。当前平台是共享xbar/DDR，论文的NoC成本结果不能迁移到这块FPGA。全文缺口由此前“仅摘要”升级为关键机制已核；不建议为本题先替换整个互连。

### 12.3 最小推进顺序与停止条件

先复用真实DMA模型，在测试侧补齐N请求的读写角色、是否关联当前未完成W及后续物理请求的事件对应；旧CSV没有完整角色/owner关联，不能凭相邻周期硬配对。**只有发现当前固定规则没有覆盖的合法选择机会，才值得实现A/B小策略。** 保留§9普通PTW活跃时数据准入作为对照，不直接启动FPGA构建。

B的最小对照为当前P、原PN、普通PTW感知数据准入PN、角色/依赖协调PN；按需要加一个能解释差异的普通基线，不一开始铺全矩阵。小缓存先估重复项，再决定是否值得实现；不能用免费理想缓存的收益直接代替同资源生产结果。若最终要声称PN胜N，仍必须补同条件N。参数保持同工作量、缓冲容量、页布局与供给范围，记录增加的翻译/数据请求。

判定看实际数据正确、排空与合法握手，以及V完成时间、A有效速度、资源/请求代价。fast数可以因walk缩短而下降，**计数变零不自动等于关N，也不能单靠fast非零证明收益**；应检查仍有可提前服务的机会与实际响应/搬运进度。普通办法同时改善P/PN是有效工程结果，不因相对差不变就否认其价值；论文是否成立另看新增机制或可复用结论。

停止条件：没有新增可达选择点；角色/依赖信息不比固定策略提供有效选择；收益完全由普通基线覆盖且没有新的资源/适用边界结论；或只靠增加延迟使N失去机会、双方总搬运时间更长。不会为这些候选自动扩大RTL、增加历史诊断连线或重跑172。

DeepSeek两次提案/定向修订均完整返回（deepseek-flash，thinking/high，默认最大393216；合计312813 provider tokens）。Codex核关键源码与原论文、剔除重复/混杂候选并执行既有轨迹频数归约。worker将虚拟AR称完成读、首拍R时延当翻译留痕、单纯换地址当分流等部分未采纳；严格“PN-P必须缩窄才算工程有效”的判据亦未采纳。[完整答复、任务与最终审定](../out/pn-alternatives-20261007/)保留。本轮无生产RTL/Linux变更、编译、新仿真/板测或板卡操作，创新尚未确认。

## 13. 用户指出均有前作：撤回创新路线优先推荐

**用户的判断成立：§12给出的是工程优化入口及待核问题，没有交付可立项的具体创新点。** 先前称B为“首选”、A为“有探索价值”，容易让人理解为已经通过创新预审，推荐强度过高。现在撤回该优先推荐；A/B保留为问题记录，C/D保留为普通基线，不据§12启动论文机制实现。没有证明某一前作与本平台逐项相同，也不能据此获得新颖性。

| §12建议 | 已有覆盖与尚缺的增量 |
|---|---|
| A：帮助阻挡其他工作者的请求完成 | [BIS](https://users.ece.cmu.edu/~omutlu/pub/bottleneck-identification-and-scheduling_asplos12.pdf)依据等待识别瓶颈；[Scheduling PTW](https://www.csa.iisc.ac.in/~arkapravab/papers/GPU_page_walk_scheduler_ISCA_18.pdf)依据完成所需工作量和批处理安排翻译。尚未给出它们的普通移植/当前owner规则不能覆盖的合法状态。 |
| B：协调翻译旁路与数据启动 | [MASK §5.4](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf)已有页表优先与依据walk/受阻warp的数据服务。增加DMA读写角色、改变控制位置只是可能差异，尚未证明它改变了普通基线无法有效作出的选择。 |
| C：动态burst | 长度控制、分块与流控是已有基础；当前还没有相对固定短burst和适用普通流控的新规则/有效差异。不能用动态二字认定创新。 |
| D：小页表缓存 | [RISC-V IOMMU规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html)明确允许非叶缓存，62次重复访问只是本工作集的工程动机。 |

“两个优化组合可能没有额外收益”也不足以当新发现：[DWS §VII-D/Fig.11](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)已讨论MASK+DWS可能无显著叠加收益。本地原文`papers/walk_stealing2021.txt:850–864`已直接核查；它不是当前P/N，也没有复现这里的AXI链条，因此仍允许寻找具体新增结论，但泛泛组合不单调的表述贡献不足。

后续研究先交付一个具体反例或约束：真实DMA场景中，在相同可用信息、资源与合法协议下，适用的最强旧方案在哪里必须付出额外代价；新的信息/规则具体改变哪一步，并给可证伪预测。先核前作和接口，最低成本概念验证用于验证已经明确的差异，不要求立项前有完整实板收益。找不到差异就停止该候选，也允许扩大到PN之外的问题。当前尚未交付这个反例，不把进一步排查或未获全文等同创新空间。

本轮Codex直接作关键创新审定，复核MASK、Scheduling PTW、RISC-V规范及本地BIS/DWS原文；没有新增DeepSeek/Claude或子agent调用、源码改动、测试、编译或板卡操作。既有PN退化/仿真因果证据保留，尚不能承诺论文贡献。

## 14. Codex与DeepSeek分别提出具体规则：尚未通过创新预审

用户要求“创建前人没有的新规则”，并明确提出方案由Codex、DeepSeek都探索。本轮双方分别写出了条件/状态/停止办法；Codex不是仅审核。**目前交付两份可审查规则草案，没有交付已确立的创新机制。** 不把新写的规则等同世界首创；不因§13否决而用新名字恢复旧推荐。

完整材料：[Codex提案](../out/new-rule-20261007/codex-proposal.md)、[DeepSeek提案](../out/new-rule-20261007/deepseek-answer.md)、[关键审定及纠正](../out/new-rule-20261007/codex-review.md)。两份原稿保留，含未采纳的过强表述，使用时以审定为准。

| 提案者 | 规则与具体决定 | 当前判断 |
|---|---|---|
| DeepSeek | 主PTW处于S1最后一级、MEM_ACCESS/PROC_PTE时，另一bank的缓存命中若为多拍读（AxLEN!=0），不接受FAST，进入现有REJECT等待普通路径；上级阶段、写、单拍读维持当前行为。 | 改动范围相对小，可作工程对照；PTW阶段感知准入与长度控制的已有覆盖很强，创新未证。 |
| Codex | 先准备命中的物理译址；当前PTE未越过共同共享AR入口前，不产生新的旁路物理读offer。放行后，每个实际PTE阶段只允许预留Q个读beat；读返回减少在途债务，不恢复本阶段额度，实际PTE进展才重新计量，旧债务保留。已承诺写所需的排空读另行豁免。 | 针对优先级生效边界前的状态及进展记账，具体差异可查；需要新接口/状态及排空证明，尚不能立项或直接改生产RTL。 |

### 14.1 从既有轨迹出发，但不虚构普通方案的失败

§8 PN轨迹的数据AR在2763接收、PTE AR在2766接收、数据首R在2813、PTE R在3100。**AR被接收不等于整条数据读不可抢占**；原模型`tests/nonblock_actual_dma_test.cpp:519–524`能在无held R时调整跨ID返回顺序，已有DS优先干预已移除模型PN退化。Codex原稿拿这段暗示普通页表优先已经无能为力，过强，审定已纠正。不能把FIFO默认策略的局限当成AXI协议或MASK的必然局限。

Codex候选的两个不同决定是：同缓存命中/相同队列占用下，PTE还在共同入口之前，暂缓新数据offer；PTE已在该入口握手且有阶段预算，可以offer。这只是具体设计差异，不是最强前作不能作同样决定的证明。新阶段开始时旧VALID/排队/在途读仍可能影响当前PTE，必须保留债务；无条件全时在途量<=Q的原稿说法也已撤回。MIG跨ID重排下，入口先后不推出服务顺序或硬延迟保障。

DeepSeek方案的“暂缓”实际上是放弃这次N接受、退回普通路径；窗口结束不会自动重试N。它拦所有多拍读，2拍也拦，不能称仅拦长读而允许普通短读。LVL3是最后一级请求，尚未验证合法叶；LVL1/2也可能是大页叶。保留owner guard不构成新增规则无死锁的证明，两份方案都需要核已承诺写的源数据排空关系。

### 14.2 与前作逐项比较

- [MASK ASPLOS2018](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf) §5.3已有walk-depth标记，§5.4已有PTE优先、依据并发walk/受阻warp分配的数据配额。PTW相位、页表优先、token本身不能立为新点；本文两条精确规则未据此证明同式先例，也没有排除其他覆盖。
- 新核[AuRORA MICRO2023](https://people.eecs.berkeley.edu/~ysshao/assets/papers/aurora-micro2023.pdf) §3.4.3：按系统干扰和任务要求动态调整加速器memory request rate。它限制“动态准入/减少数据发送就是创新”的表述；不是本平台逐PTE进展规则的同式证明。
- Scheduling PTW、MMU-aware DMA、DWS以及普通credit/预约/依赖调度的既有覆盖仍成立。NB-Walker全文/精确协议仍缺，不能宣布已排除最新前作。控制放在IOMMU而不是DDR只是实现差别，单独不足以支撑创新。

### 14.3 下一项仅作有条件判别，不启动长构建

若继续此候选，先复用既有实际DMA模型的一个gap0条件，对比原PN、普通PTE优先、普通固定credit及两条规则，分离Codex方案的入口确认和按进展补许可。保持job/burst、工作集、P策略、CPU生成方式及窗口定义一致；记录V耗时、A有效速度、数据错误/排空、合法快命中机会、实际读提交与PTE阶段关系。最终判断还必须补同条件N；当前不能宣称PN超过P/N。

**停止条件：** 普通方法作同样选择/取得同样结果；收益只是关闭N或全局少发；仅在人工强化延迟/严格FIFO而真实平台无适用接口时成立；或者新增状态/线路不适合当前几乎满Slice的FPGA。任一成立就撤回相应创新候选，不靠扩大矩阵维持路线。发现有效差异后才设计生产接口与板端确认，不直接跑172或完整BIT构建。

本轮DeepSeek-flash思考/high提出一条规则并否决owner组合，共218227 provider tokens，默认最大预算393216、未截断。Codex保存另一份独立提案后读取DeepSeek答复，直接核关键RTL、原DDR驱动/轨迹及原论文，纠正两份草案；双方共享事实/候选轴，不称完全盲审。没有Claude/新Codex子agent、生产RTL/Linux修改、编译、新仿真/板测或板卡操作。

## 15. 两条规则的创新查验：一条工程对照、一条未验证差异

用户要求“检查是否是创新”。**当前两条都不能作为已确立的论文新方法：DeepSeek的最后一级多拍读门控仅保留工程对照；Codex的入口确认＋逐PTE额度保留一个待验证差异，不直接进入生产RTL/BIT。** 这是有限范围的研究审定，不是证明全世界已有逐项相同机制，也没有否定整个项目。

完整证据：[Codex原源审定](../out/rule-novelty-check-20261007/codex-novelty-review.md)、[DeepSeek修订答复](../out/rule-novelty-check-20261007/deepseek-revised-answer.md)。初版与修订诊断完整保留；初版的同式/旧实验论证未采纳。

### 15.1 原论文对照与剩余差异

| 规则 | 最接近的已核前作 | 当前尚存的精确不同 | 结论 |
|---|---|---|---|
| A：最后一级PTW期间拒绝多拍N读 | [MASK §5.3–5.4](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf)已有walk-depth与PTE/data协同；Scheduling PTW已有翻译工作量调度。 | 把主walk最后一级、AxLEN!=0与N接受结合。没有在所核段落找到完全同式，但只是细化条件/控制点，尚无独立效果或成本优势证据。 | 创新理由弱，工程对照；不能说某前作已逐项做过A，也不建议作为主贡献投入。 |
| B：PTE入口确认＋按阶段补额 | [AXI-REALM](https://arxiv.org/pdf/2501.10161)的AXI入口budget/fragmentation/write-data-ready，与[ABU ECRTS2019](https://drops.dagstuhl.de/entities/document/10.4230/LIPIcs.ECRTS.2019.24)的周期预算是很贴近平台的近邻；MASK/AuRORA另有PTE优先及动态供给。 | 以PTE共同入口握手限制新offer；预算按实际PTE阶段进展重新计量，而非按时钟period。已核原文没有这一同式规则；旧债务保留。 | 有具体未验证差异，但没有证明适用普通方法无法取得同等效果/成本，故创新未通过；暂留研究候选。 |

新增核查[AXI-REALM 2025完整公开稿](https://arxiv.org/pdf/2501.10161) §III-A-1/2/3：可配置burst拆分、完整写数据就绪才转发AW/W、逐beat扣预算和周期补预算；其2023前身已被[DATE24接收](https://arxiv.org/abs/2311.09662)。ABU原文§3的预算B与周期P同样表明AXI代理/counter门控已有前史。原PDF和文本已保存于本轮`papers/`；不将其资源/保证数字外推到本FPGA。

**B确实有不同决定，但尚非新贡献验收：** 从策略状态看，当前阶段used=Q、旧数据已返回、PTE尚未进展，周期预算到期或普通outstanding cap腾出空间时可能准入下一条读；B仍阻止。差异直接来自规则，不代表现有256-beat测试已到达这个状态，也不证明B总体更快、更公平或有硬延迟上界。进一步价值可能是效果、成本或适用约束的小增量，不要求必须发明全新硬件原语。

### 15.2 否决审查中的错误已纠正

1. 旧p4按PTW返回事件**收费**，固定窗口末**补额**。直接源码`dma_research_policy.sv:325–327,340–345,353–354`已核；它不等于B的PTE进展补额。撤回DeepSeek初版“p4已知同式”的论证。
2. MASK的walk-depth原用于按层cache命中率绕L2，不等于A的数据FAST准入。Scheduling PTW的job长度是翻译所需访存工作量，不是DMA AxLEN。只能用作相近思想，不能偷换成逐项覆盖。
3. 能写成FSM/counter、旧构件组合或小逻辑，不证明不能创新；不要求信息无法由任何旧状态组合得出、必须有硬实时保证或预审前完成MIG实板收益。需要的是明确新增决定/约束与公平的效果或成本证据。
4. 共享PLRU隔离已完成，旧额外miss差已消失；原初版把旧板测与固定delayer模型混写、要求重新排除PLRU，未采用。§3/8/9仍是最新事实边界：模型DDR因果链已证，板端对应链未证。

**另外，DeepSeek修订表中的“Q>beats进展缺失”比较号颠倒，最终应检查`beats>Q`时的处理与进展。** 未据这个笔误新增模型调用，原文保留、Codex在最终审定明确纠正。两条策略的排空/异常/失效保障仍待设计，但缺实现本身不是新颖性否决证明。

### 15.3 研究决策与检查范围

只保留B的具体事件差异作为下一项可核问题：强普通PTE优先、周期预算/在途上限，以及按walk单一额度，能否在同等合法接口与资源约束下实现同等效果；按阶段计量是否带来独立成本/性能/约束增量。如果只有关闭N、少发数据、严格FIFO或不适用接口有效，就停止该方法推荐。当前不重跑历史矩阵、不修改生产RTL或构建BIT；若后续做概念验证，应先做一个短条件，并补最终P/N/PN同条件判断所缺的N数据。

本轮补核NB-Walker作者Zenodo v2 artifact、AE README/目录/只读setup脚本，公开AE树24项为工具/配置，未得到论文正文或核完整MMU机制；不能宣称排除最新重合。CATO全文缺失保持。新增原论文与近邻核查不是对世界全部文献的穷尽证明。

DeepSeek两次完整思考/high（初版97565、定向修订133564，共231129 provider tokens），最大预算393216、stop/未截断；它未执行或写文件。Codex直接核原论文、旧策略源码与原DDR排序逻辑，纠正证据并最终审定；无Claude/新Codex子agent、生产改动、编译、新仿真/板测或板卡操作。

## 16. B最小差异验证：普通对照仍更强

**最小验证完成，当前单条件不支持继续把B作为论文主方法推荐。** 本轮修改测试文件、构建一次Verilator模型并执行21次短仿真，21/21通过；未修改生产RTL/Linux，未构建BIT或操作板卡/SD。

固定V拷贝64次、A工作集8页、V间隔0、预热256次、模型内存延迟31周期；各组窗口内A也均完成64次。A有效吞吐为`64×4096/W`字节/周期，V耗时与A吞吐同时比较。下面均经过同容量的单请求缓冲：

| 方案 | V耗时（周期） | A吞吐（字节/周期） |
|---|---:|---:|
| P | 91874 | 2.853 |
| N | 151947 | 1.725 |
| PN | 93487 | 2.804 |
| PN＋仅确认页表请求入口 | 92917 | 2.821 |
| PN＋逐PTE补额（B） | 92362 | 2.838 |
| PN＋固定周期补额（1024周期） | 92227 | 2.842 |
| PN＋每次完整页表遍历给一次额度 | 91091 | 2.878 |
| PN＋普通在途额度上限 | 92362 | 2.838 |

B相对PN的V耗时降低1.203%、A吞吐增加1.218%，但V仍比P慢0.531%。固定周期和整次遍历额度更好，普通在途额度与B耗时相同。加入普通页表优先后，PN为79491周期、B为79486，仅多省5周期（0.0063%）；没有体现足以支持该规则独立价值的优势。这不是否定所有场景下的逐PTE方法，成本也尚未综合。

默认不加缓冲的S/N/P/PN为105588/152042/91878/93480周期；P/PN及页表优先结果精确复现旧值，补到了此前缺少的N同条件数据。不能把这个A8结果外推所有工作集。

测试代理放在TC交出物理读请求之后、共享互连之前，所有缓冲策略容量相同；B以真实PTE返回补额，预算256拍，请求首次声明VALID前预留，返回数据只减债务。FAST读标签与当前TC事务绑定，已发请求保持稳定。它仍不是生产TC中完整的待发状态协议，缓冲本身可能引入队头等待。

数据及终点排空检查通过，运行期AXI稳定性检查未报错；事件计数与窗口一致。B的两组分别检查155个完整阶段，其中38/14个阶段有实际收费请求，均未超过256拍。所有组排空豁免次数为0，未覆盖复杂豁免；没有通用活性、失效或异常证明。仅一轮确定性DDR模型、模拟CPU流量随耗时改变，不能声称实板/MIG收益。

**下一步性能主线保持，但先核具体完成依赖与普通页表优先的失效条件。** 先找真实请求路径及强普通方法无法覆盖的反例，再提出机制；不自动进入下一矩阵或BIT构建。

证据：[原始测量](../out/pte-progress-poc-20261007/measurements.json)、[同条件比较](../out/pte-progress-poc-20261007/comparison.json)、[验收](../out/pte-progress-poc-20261007/acceptance.json)、[关键审定](../out/pte-progress-poc-20261007/codex-review.md)、[归约脚本](../out/pte-progress-poc-20261007/summarize.py)。初稿、实际诊断及修订全部保留。

DeepSeek四次完整思考/high交付测试草案、定向修订、汇总与报告，441009 provider tokens；Codex负责架构及关键源码审查、修补、应用、实际编译/执行和研究验收。无Claude或新Codex子agent。

## 17. 调度与微架构继续探索：Codex 独立设计，候选仍待判别

**Codex 已亲自提出两条规则并作关键分析；目前没有确认创新或新性能结果。** 用户明确要求创新设计、机制分析不能全交给 DeepSeek。已在根 AGENTS.md 记录；本轮 Codex 先写独立规格，再读取两份已派发的 DeepSeek 补充答案，随后核原源并审定。

| Codex 候选 | 具体决定与硬件支持 | 最强普通对照／尚缺证据 |
|---|---|---|
| C：目的准备状态指导源数据提前量 | 源热目的冷时，有独立返回空间才允许一个小早读窗口；两端准备好且目的可执行后扩张；目的被另一 job 的写所有权阻塞时不再扩张。需绑定 job/字节段的准备表、翻译准备接口、容量账本。 | 两端先翻译＋普通在途额度、逐读空间预留、页表优先、固定短段。可能等价；源热目的冷状态在当前 AW-first/单TC流程中的真实可达性未证。 |
| D：可执行等待者触发下一段缩短 | 另一 job 已准备、只等 W 路径时，缩短尚未声明 VALID 的下一读写字节段；另一 job 还缺翻译时保留长段。需真实等待原因与 legalizer 的逐段上限接口。 | 普通下游压力自适应本来可能同样区分。已从主候选降为配套机制／普通对照，不把动态 burst 本身立题。 |

**当前优先核 C 的具体差异，不直接实施。** 研究问题是“怎样保留有用的提前数据重叠，同时限制尚不能执行的数据占用”；不是增加固定额度或再次做软件限速。若两端准备＋普通额度以相同资源达到同样结果，就停止 C 主贡献主张。支持小而清楚的增量论文，不要求全部构件首次发明。

实际接口限制：TC 已有 AW-first，翻译完成后直接 COMPLETE/物理 VALID；待发槽需纳入在途。输入虚拟 AW/W owner 与 completion mux 的物理 owner 不相同。iDMA 已有共同读写字节边界，但 buffer 为逐 byte FIFO、usage 未连接；段长增加的请求/翻译数量要记账。translation-only 不能用普通 AW 假冒，否则会提前保留 W owner。第一判别限定静态映射和正常 DMA；IOFENCE PR/PW 仍为 TODO，不能称完整更新/故障协议。

补核强前作：[Criticality ISCA2013](https://users.ece.cmu.edu/~saugatag/papers/13isca.pdf)、[SMS ISCA2012](https://people.inf.ethz.ch/omutlu/pub/staged-memory-scheduling_isca12.pdf)、[DASH](https://people.inf.ethz.ch/omutlu/pub/dash_deadline-aware-heterogeneous-memory-scheduler_taco16.pdf)、[MMU-aware DMA ICCD2018 §IV-C](https://arxiv.org/pdf/1808.09751)、[AXI-REALM §III-A](https://arxiv.org/pdf/2501.10161)、[Arm DMA-350 §5.8.1](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)。它们已覆盖关键性/进度、分来源队列、DMA burst身份和重试、拆分/写数据齐备、完整返回空间等构件。另核[ABW机构摘要](https://researchportal.hkust.edu.hk/en/publications/adaptive-burst-writes-abw-memory-requests-scheduling-to-reduce-wr/)的自适应 DRAM 写批次，不等于 AXI 单 burst；其正文及 NB-Walker 精确机制仍有缺口，未排除全部前作。

DeepSeek 五个接口提案未采纳为实现：最老不等于关键；新请求等待自己的返回可能成环；按一次PTE返回区分DATA/PTE不成立；用PTE返回抵completion-data债务不守恒；DID/AXI ID不等于job字节段。分队列消除测试代理的HOL也不能当生产系统新增贡献。完整草案保留，使用时以 Codex 审定为准。

下一只做定向的可达性／等价性判别，满足后再用现有真实 DMA 模型作一次短概念验证，同时看 V 耗时、A 完成字节/吞吐及额外事务、翻译与资源开销；不自动扩大矩阵或进入 FPGA 长构建。本轮仅设计、检索、源码阅读和文档记录，未改生产RTL/Linux、未构建或执行新仿真、未操作板卡/SD。

证据：[Codex 独立稿](../out/scheduling-microarchitecture-20261007/codex-proposal.md)、[最终审定](../out/scheduling-microarchitecture-20261007/codex-review.md)、[原源定位](../out/scheduling-microarchitecture-20261007/source-audit.md)、[完整材料导航](../out/scheduling-microarchitecture-20261007/README.md)。DeepSeek两次完整high/393216，267076＋146387＝413463 provider tokens，均在用户最新分工要求前派发；提供补充提案/证据，未执行或写文件。Codex独立设计、直接源码/原文核查和最终判断；无Claude或新Codex子agent。

## 18. C可达性小判别：一般源先行成立，独立创新仍未成立

**5/5短仿真通过，但没有实现C。** Codex预先定义“预热不变、测量换到新目的页”的单文件夹具和否证条件，亲自检查源码、实际事件与创新结论。复用已生成SV模型，只重编C++/链接一次；生产RTL/Linux、BIT及板卡/SD均未操作。完整证据：[Codex审定](../out/c-readiness-check-20261007/codex-review.md)、[验收](../out/c-readiness-check-20261007/acceptance.json)、[运行命令/JSON](../out/c-readiness-check-20261007/runs.json)。

新目的IOVA0x10002000→PA0x80002000有合法PTE，warm仍访问旧目的；不是OS缺页。固定A8、V64个4KiB、gap0、warm256、模型延迟31及原DS路径。旧热PN精确复现93487周期；下面是单次路径测试，非统计性能结论，各窗口V/A均完成64次：

| 新冷目的条件 | P的V窗口周期 | PN的V窗口周期 | PN相对P的V耗时 | A完成吞吐 P / PN（B/cycle） |
|---|---:|---:|---:|---:|
| FIFO DDR模型 | 92334 | 94427 | +2.267% | 2.8391 / 2.7762 |
| DS读优先返回干预 | 80252 | 79911 | −0.425% | 3.2665 / 3.2804 |

FIFO中A吞吐下降2.217%；优先干预中提高0.427%。A完成吞吐按64×4096/W计，包含窗口首job carry-in；CPU轮询流量随执行反馈改变。没有新冷页S/N组，不称PN领先P/N。

**可达性分两层：** 首V源DDR AR在16/26周期，确实早于新目的叶PTE读及填充（P叶读1554/fill1657，PN叶读2107/fill2216）。四组均只有一次V fill，证明一般“源已缓存、目的冷”源先行状态；但首V源capture FAST位都是0，而且P也有此状态，未达到预稿“同一作业FAST源先于自己的冷目的”严格预测。旧热日志中源早于目的AW不能证明目的冷；AW可能被另一bank输入W owner阻挡。

**更清楚的等待链在另一作业：** V查冷目的页表期间，PN中A的两条FAST源读于1226/1232进入DDR，先于V根PTE1263；A目的AW到2728/2735才进入，之前被V的输入W所有权阻挡。FIFO P的V根PTE1259→1291，服务延迟32；PN为1263→1840，延迟577。DS优先干预仍允许A早读，但PN根PTE延迟回到32，V PTWactive从FIFO1010降至435，P优先也为435。该模型支持提前数据延长关键翻译等待，不能把重叠差值相加、解释完整2093周期差，或直接归因旧板端约12.7%退化。

DS优先在C++已接收DDR读队列中重排不同ID的返回，是因果干预/理想参考；不是DUT AR仲裁，更未证明板上MIG能作同样选择。后续须有可实现的入口强对照。

**Codex判断：C不升级论文主方法。** 自己的目的是否译好不足以判断提前读是否有用，还需看它能否排出以及是否妨碍占W路径一方的页表请求。两bank的稳定PTW片段里，FAST候选与主bank相反；owner关系已能区分某些“帮助排空/只能囤数据”的情况，普通owner额度可能不需要新增prepare表就作同样决定。此为有条件机制推演，尚非所有动态状态的等价证明。

下一仅核一个精确差异：同owner/PTW关系、同返回空间/在途约束下，C的额外目的prepare信息是否仍能产生不同且更有用的合法选择；普通owner排空优先＋非owner有界额度、两端预翻译/普通预约均为强对照。如果找不到真实反例或普通方法等效且更省，就停止C主贡献推荐。性能主线保持，不能把owner限流本身重新宣布创新，也不自动恢复B推荐或进入BIT/172。

重新核[MMU-aware DMA ICCD2018 §IV-C](https://arxiv.org/pdf/1808.09751)的burst身份/失败排空重发，以及[rIOMMU ASPLOS2015](https://dants.github.io/papers/riommu-asplos-2015.pdf)的ring下一项预取；未把相近构件说成本候选精确同式。前述强前作及NB-Walker全文缺口仍在。DeepSeek三次完整high/393216草案与修订，共492799 provider tokens，未执行；Codex独立规格、关键源码/原文/机制分析、补丁修正、实际运行与验收。无Claude或新Codex子agent。

## 19. 普通准入强对照：8/8通过，停止旧C首选推荐

**本轮没有实现C，不能说“C没人做过”或“C实测效果不佳”。** 实际测的是两个普通方法：复用原owner读guard的保守配置，以及已有翻译后整walk额度。Codex亲自设计准入位置/时序和判别条件，核源码/实际事件；DeepSeek提供接口证据和单文件C++草案。完整[审定](../out/owner-baseline-20261007/codex-review.md)、[8/8验收](../out/owner-baseline-20261007/acceptance.json)、[命令/结果](../out/owner-baseline-20261007/runs.json)。

仅3个tests文件新增owner-only许可掩码与阶段开关；实际4KiB缓冲、生产RTL/Linux不变。预热沿原buffer，所有新方法的warm摘要逐字匹配同mode旧结果。默认关闭精确复现93487/94427。一次SV模型构建通过（首次不支持input默认值的elaboration失败保留并修正），追加普通walk公平对照只重编C++；无BIT/板卡/SD操作。

| 条件/普通方法 | P完成窗口周期 | PN完成窗口周期 | PN FAST | V fill P/PN |
|---|---:|---:|---:|---:|
| 原热页buffer | 91874（既有） | 93487 | 62 | 0/0 |
| 原热页owner保守准入 | 92898 | 92857 | 0 | 0/0 |
| 新冷目的buffer | 92334（既有） | 94427 | 64 | 1/1 |
| 新冷目的owner保守准入 | 93778 | 93797 | 0 | 2/2 |
| 新冷目的翻译后整walk额度256 | 92334 | 91727 | 64 | 1/1 |

全部窗口A/V各完成64个4KiB；owner方法使PN比原PN快约0.67%，但FAST全部消失，且仍比原P慢。已有整walk方法则保留64个FAST，冷PN比原PN快2.859%、比同条件P快0.657%，A有效吞吐比P提高0.662%（2.8391→2.8579 B/cycle）。P普通walk精确复现92334。单次模型结果，不是统计或板端收益；没有新S/N，CPU轮询数随完成反馈改变。

**新的明确机制：准入位置也会改变本bank的缓存访问顺序。** 原冷PN源AR16/26先行，V只遍历新目的。owner cold PN先读目的叶0x50010于992、fill1096，随后又读源叶0x50000于1473、fill1576，再发源AR1585/1592；P也出现同样两次V walk。IOTLB命中touch影响PLRU，V固定2位置、warm同序、失效输入为0、只有新目的这一项更新发生在源miss之前；这些源码约束支持新目的填充替换原源项。与已隔离的跨bank共享历史旧缺陷不同，不是重新发现该bug。整walk普通法把额度放在翻译后的AR入口，没有新增源walk，V fill仍1。

这既不能由owner保守配置直接否定C，也不能把“保留翻译、推迟部分数据”重新当C独有贡献：普通整walk已经在本case实现这一效果。**当前停止旧C作为首选创新方案的推荐。** 尚无新增目的prepare信息超出普通walk/在途额度、两端普通准备的收益或成本证据；不是世界前作穷尽结论或全状态等价证明，也不是C算法实测失败。

性能主线保持，91727作为冷页新强普通基线。下一候选进入实现前，须先给出这些普通方法在同资源/真实约束下无法兼顾的具体合法状态对，或同效果的可验证成本减少；当前仍缺这个反例。不以PN略胜P直接确认创新、不自动扩矩阵或BIT。再核[MMU-aware DMA §IV-C](https://arxiv.org/pdf/1808.09751)的锁TLB代价/重发元数据与[AXI-REALM §V-B](https://arxiv.org/pdf/2501.10161)的调节/缓存及过度限流代价，未将相近构件说成精确同式。

DeepSeek三次完整high/393216共270546 provider tokens，stop/未截断，未执行或写文件。Codex独立规格、关键SV/准入位置与机制判断、原源码/文献核查、草案应用、实际构建/仿真和最终验收；无Claude或新Codex子agent。所有失败入口和原始证据保留。

## 20. 继续探索：按可排空读段预约，保留成本预研候选

**本轮收窄出E2成本候选，仍未确立创新，也未实现或测量E2。** Codex独立提出共享接收区E1、同bank队头反例，并进一步提出E2：能为当前写通路排空提供数据的读段，用小FIFO流式处理；其他提前读须在共享AR仲裁选择之前预约完整burst返回空间。准确判断需具体DMA及read/write burst匹配，不能只看bank、同DID AW pending或PTW阶段。[Codex设计/反例](../out/completion-reserve-exploration-20261007/codex-independent-design.md)／[关键源码与原文审定](../out/completion-reserve-exploration-20261007/codex-review.md)。

两项边界核实：普通walk额度只限制指定阶段新FAST读，不是全部数据或PTE等待上界；旧冷页轨迹中的256beat数据在root返回后持续1327→1582，下一层PTE1408→1653等待245cycles（root32），CPU读也有影响，不能全差归因这一读。生产4DMA共2bank，10/12为bank0、11/13为bank1，整copy容量保证只覆盖10/11；2DMA既有PASS不足以证明4DMA全状态进度。此轮未复现任何新的板端bug。

初稿E1“共享一个整页slot”与普通pool预约重叠，停止以共享/预约本身作主创新。E2研究容量应随**不能排空的已承诺读burst**配置，而不是每DMA配整job。当前ARLEN255、64bit，一段2048B；共享2048B加两个3beat FIFO共2096B，相对现8192B理论payload少74.41%，但只是待检验配置，非面积/吞吐节省、最小容量定理或无死锁保证。共享区满时，准确排空者可通过小FIFO供数而普通全响应空间规则拒绝；普通pool若加相同依赖门则可能等价，必须作强对照。相同DID第二段源读也可能不是当前W头所需，不能粗放豁免。

定向核[CTC2009 §3–4](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)、[Arm DMA350 §4.3.4](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)、[AXI-REALM](https://arxiv.org/pdf/2501.10161)、[2026 Preemptive VCs §III](https://arxiv.org/html/2607.01430v1)、[旧IOMMU专用VC专利](https://patents.google.com/patent/US7480784B2/en)和MASK §5.4。共享、接收预约、避免DMA循环均有前作；2026 VC等此前§12已核，不称全部新文献。增量微架构论文不要求每个原语首创，但必须有可推广的实现边界、可信成本性能证据，不能只换位置/名字。

DeepSeek给证据/独立质疑，Codex直接审定并纠正其“同job AW/AR重叠需多个job”“选择前预约等同输出VALID后反压”“per-ID事务计数等同字节空间”“现私有证书就是动态pool”等过度推论，完整原回复保留。两次high/max393216共509490 provider tokens，stop/未截断；DeepSeek未执行/改文件。仅研究文档/证据，生产RTL/Linux/tests/编译/BIT/板卡均无动作，最新实际验收仍§19的8/8。

**下一主线：用更小缓冲保留非阻塞翻译收益，E2仅作预研候选。** 先核当前write burst匹配与返回重放的接口成本；通过才做2DMA小PoC，共比私有8KiB、普通pool全响应预约、普通pool＋准确owner门及现walk/在途强方法，同时看V/A吞吐与小模块面积/时序。若普通依赖pool同式同成本，或新增控制消耗抵消缓冲收益且无性能价值，停止E2主贡献。不自动扩4DMA矩阵/完整BIT；冷页91727保留为已有普通强基线。

## 21. 共享返回缓冲原型13项通过：容量下降，并行机会被当前入口限制压住

**当前实现没有性能收益，未进入生产/BIT。** §20 候选 E2 已从接口讨论推进到测试专用真实 RTL；此处候选 E2 与早期“攻击者停止后的替换残留”E2 实验不同。新增 `tests/nonblock_completion_pool.sv`，两个真实 iDMA 各3拍私有 FIFO，完整返回存入 RTL 共享区，按 DMA 私有 R 端口回放。按源/目的段偏移、实际 AW 长度与两级 W 队首匹配；pool 许可只覆盖已预约的当前读，真实 fullbuffer 证书保持0。DDR C++ 模型没有替其存池数据。

修正原型接线后13个唯一完整场景通过，V/A各64次4KiB，源/目的/未用页验证、原系统/新池排空与收发守恒均完成。原 PN 热页93487、FAST62精确复现；普通两槽共享区4144B，热 P97774/PN103348、FAST0/31；选择性流式加一槽2096B，热 P112451/PN112448、FAST0/0；冷 P112931/PN112948、FAST0/0。2096对8192减少74.41%仅是数据容量差，排除元数据/控制，不是LUT或面积结果。

Codex再试“当前W属主的后续段不抢池”，相同2096B，池/流grant从960/324变640/644（含预热）；热 P125680/PN125677，冷 P126180/PN126177，FAST仍0。这个结果否定仅改该分配规则可恢复并行的预测，不将它推荐为改进。此前普通大私有缓冲＋walk强基线 P92334/PN91727 单独保留，未计入13项。

**关键实施约束：预约在翻译入口，且每DMA只能有一个未消费完的AR。** 原始首段V事件：原版第二DDR AR+26先于首Rlast+334；选择性版第二AR+1234晚于首Rlast+330；改分配版+1238/+334。结合关键源，支持返回存储与翻译入队绑定压住了请求流水；不是已完成全部损失的因果分解。两槽普通实现也有一读限制，不能当最强pool上界或同在途深度公平比较。

下一主线：元数据请求先排队，在**每bank完成AR进入锁定的共享完成mux之前**预约真实返回空间，保留私有回放。需按具体请求/段匹配许可，防止整job容量位给未预约AR放行，守住AW/W顺序、响应ID/长度/last及VALID稳定；先看FAST和正确进度，再同队列/同容量比较普通精确依赖池与普通额度。此候选可能仍与通用credit/pool等价，创新未成立；当前负结果不否定全部共享返回方案。

仅新测试模块、3个测试源和原启动器的一行依赖加载改动；76个编译生产SV哈希不变，未改Linux/生产RTL/BIT或动板卡。初次lint关键字/层级问题、首测AW首次选择事件与寄存器后payload错配（fault260），均是Codex新原型错误，已修正并保留失败证据；13/13非首次运行全过。限2DMA、正常对齐4KiB、每DMA一个执行job、合成CPU/行为DDR，未综合面积、任意背压/错误/取消/4DMA/MIG或上板验证。

[Codex设计与审定](../out/e2-interface-poc-20261007/codex-review.md)／[原始9项](../out/e2-interface-poc-20261007/initial-runs.json)／[后续4项](../out/e2-interface-poc-20261007/reserved-runs.json)／[首V事件](../out/e2-interface-poc-20261007/first-v-read-trace.json)／[测试补丁](../out/e2-interface-poc-20261007/test-source.patch)。DeepSeek负责接口证据、C++及结果验收草案；Codex独立设计、源代码审查、应用修补、构建/仿真执行和最终判断。


## 22. 完成阶段准入与强普通对照：共享池主贡献停止

**新21项最终矩阵通过；最强结果来自普通无池写前缀准入，创新尚未成立。** 元数据提前排队恢复TL FAST，但Mode4单槽热PN123804仍慢；紧邻下一段流式Mode6降到99432，普通两槽Mode5为102746。再补普通准确写前缀、不用POOL的Mode7，热P92109/PN92374、冷P92549/PN92814，V/A各64完整4KiB。热PN较本轮原大私有缓冲93487耗时少1.19%，仍比同法P慢约0.29%。共享池在当前fixture无独立价值，不再推荐为主贡献。

新规则核源/微架构由Codex亲自设计：AR元数据入队与实际返回容量解耦，在每bank完成AR进入锁定mux前预约；当前与紧邻下一W段精确匹配源/目的偏移、len/size，追踪两个AW首次选择/WLAST写顺序，下一物理AW接受后才放行。同readID0旧STREAM先完成，或旧POOL已全驻留，才能承诺future STREAM；寄存admit不撤VALID。限定两DMA、对齐正常4KiB、单执行job、行为DDR与独立公平W，不是一般无死锁证明。

活跃DMA payload：原8192B、两槽4144B、单槽2096B、无池48B（两3拍FIFO）。此数字排除共同IOMMU W FIFO、元数据、响应字段和控制，未综合面积/时序。iDMA已有小FIFO流式及可替换整缓冲，[原论文§II-C](https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf)；Arm DMA350完整响应容量和CTC消息依赖/接收信用均是强前作，不能凭变小/换位置宣布创新。[完整设计、受限推演、表及原源](../out/completion-reserve-late-20261007/codex-review.md)。

Codex发现旧testproxy单FAST标签被多条提前翻译覆盖，补每bank8个成功READ标签FIFO后，只重编/重跑Mode7 buffer/walk八项。修后read FAST62/64、标签fault0/queued0、buffer周期不变；此前13条buffer数据/cycles/TL FAST仍有效，其31/32读分类撤销。walk修后无fast_gated_offers/budget_stall，结果相同意味着本fixture无额外额度动作。实际29次完整数据执行保留，最终21配置＝原13＋修后8；旧8不计最终分类验收。六次SV模型构建成功＋一次lint，核对exit0、76生产SV哈希不变，无生产/Linux/BIT/SD/板卡操作。

下一主线：先确认两个W顺序不同是否在本平台合法可达，以及普通前缀准入的安全/适用边界，再决定最小回退或顺序约束。若普通机制同资源可等价，就作为工程优化；需要具体前作不能同样处理的差异才转论文方法，不自动上BIT。[验收](../out/completion-reserve-late-20261007/acceptance.json)／[最终21项](../out/completion-reserve-late-20261007/runs.json)／[全部执行](../out/completion-reserve-late-20261007/runs-all-executions.json)／[比较](../out/completion-reserve-late-20261007/comparison.json)／[源码补丁](../out/completion-reserve-late-20261007/test-source.patch)。DeepSeek验收与独立质疑草案，Codex直接设计、关键诊断、执行验收和创新审定。


## 23. 二区目标推进：共享bank的真实停滞边界与下一机制规格

**创新尚未成立，但已得到具体合法反例；简单加FIFO或源优先修复不能单独算贡献。** Codex核输入demux的W锁定及TC内部两个AR spill，排除未证的AW翻转和“没有完成头缓冲”假设。原Mode7双bank在第二AW延后64/4096cycle均完成，4096使future=0，原覆盖检查误报已修。随后用两个真实DMA共享一个bank、相同有限64cycle源AR背压，复现完成AR队头V0/V1挡住A0：bank0 spill双满、queue_ids0x400、TC COMPLETE、写头A且四AW已准备，sharedR空、PTW/TL空闲，独立V watchdog触发。不是带宽饱和，亦不是自己的AW待译。

| 同一小判别 | DMA活跃payload | 结果 | 窗口V/A完成 | V窗口cycles |
|---|---:|---|---:|---:|
| 两独立bank＋普通完成前缀门 | 48B | PASS | 64/63 | 129548 |
| 两DMA同bank＋相同门 | 48B | 预热停滞 | 0/0 | 无窗口 |
| 同bank＋普通完整私有缓冲 | 8192B | PASS | 64/63 | 127659 |
| 同bank＋普通源端准确写准备门 | 48B | PASS | 64/64 | 131464 |

均S、warmup2、V64个aligned4KiB；成功项完整字节/排空检查。容量排除TC FIFO/metadata/control，非面积；各法改变请求时序/暖场A数量/窗内工作量，不能计算公平速度收益，不代表PN。生产四DMA/两bank的共享拓扑已有，但本次是两DMA测试别名到同bank的最小形态，非生产BIT或板上bug。源准备门属普通强对照，可低成本修复，未宣称创新。

下一候选由Codex独立提出：已翻译但当前不可排空的AR只暂存请求头；当前写所需的源读可跨不同ID越过这些头，暂存满时仍保留直接通路。同ID保持顺序、按实际handshake收费/释放，READ分类随头携带。先核两头共享暂存＋直接通路与普通每DMA两槽的差异，比较H1/H2并证明容量条件；K个DMA/bank、L先行读的(K−1)L只是待证界，不是已得定理。候选未实现。若普通共享可挑选队列/旁路同资源等价，仍按工程实现处理；论文价值需来自新进度/容量结论和同条件成本/性能，不因命名或反例即确认二区。

新补核[Arm LTI官方概述](https://www.arm.com/architecture/system-architectures/amba/amba-5)已支持翻译/数据事务分离；MMU-aware DMA、DMA350、CTC及2026 AXI VC前作仍必须比较。完整LTI流控细节本轮未取得，不称排除所有前作。主线仍共享IOMMU性能/成本，不转安全。后续再扩四真实DMA两bank和PN；先小PoC，不自动完整BIT。

本轮10成功SV构建、2次C++重编，13次调用中7 PASS、3独立watchdog复现、1覆盖检查错、1新配置标签错、1CLI参数拒绝，全部原始保留，不能叫13/13通过。源门首测标签错出自Codex新same-bank测试配置仍按ID2 pop，修成实际BankIdBit后PASS；停滞在comp AR=0阶段，与该错无关，最新清洁诊断再次复现。仅5个测试源改变，76生产SV哈希不变，无Linux/BIT/SD/板卡操作。[Codex审定/规格/前作/限定](../out/write-order-boundary-20261007/codex-review.md)／[阶段清点](../out/write-order-boundary-20261007/stage-evidence.json)／[执行](../out/write-order-boundary-20261007/runs.json)／[补丁](../out/write-order-boundary-20261007/test-source.patch)。DeepSeek两次证据/验收草案300918 provider tokens，Codex亲自设计、关键源核查、诊断、实现执行与创新审定。

## 24. 请求头暂存原型已实现：解除本例停滞，独立机制创新未成立

本轮接§23实现测试专用的共享已翻译AR头暂存及当前W前缀源读直接通路。两真实iDMA，每路同时至多一个活动job，连续执行普通对齐4KiB任务，每任务两个2048B读burst，原始读ID0/4。未覆盖真实四DMA、MIG或板卡；同bank映射与有限首AR延迟为受控合法反例，不能称板端自然bug。

| 同bank S，warmup2、V64 | 结果 | 窗口cycles | 关键状态 |
|---|---|---:|---|
| 共享1槽＋直通 | 预期watchdog失败 | 无 | 1头已存，V1仍挡住A0；派发0 |
| 共享2槽、无直通（Mode1） | 预期watchdog失败 | 无 | 2头已存，A0可见但无空槽；派发0 |
| 共享2槽＋直通 | PASS | 127092 | 暂存262＋直通2＝派发264；满槽直通2；终止排空 |
| 普通每DMA2槽，共4槽 | PASS | 127147 | 暂存/派发264，峰值3，终止排空 |
| 普通共享3槽、无直通 | PASS | 127147 | 与普通分队列窗口指标相同，峰值3 |

三个通过例窗口均V64/A63；终止计数含warm及窗口外排空。所有方法的首AR夹具放在公共路径、每job首次VALID前有限延迟64 cycles。55周期不作净性能收益结论。强普通容量对照应是共享3槽，而不是只用4槽分队列作比较；本例只有一个新增请求头槽的差异，未测LUT/FF/总存储成本。

另核正常分bank、无夹具、warmup256、V/A各64：共享2槽＋直通与普通分队列均P92109/PN92374 cycles，PN快速翻译/快速数据读62/62，与此前无池Mode7相同。保留提前翻译机会，没有新PN收益。

完整头按源ID最早序号选择；同ID有更早暂存头时禁止直通。下游首次VALID后锁住选择，直到真实AR握手释放。放行复用既有准确W前缀谓词。新加入同ID顺序、VALID载荷稳定、终止排空及捕获/派发守恒检查。FAST标签改为按源FIFO：Codex核TC成功TL响应时原AR头尚未弹出，能正确采样源ID；无需TL响应与完成在同周期。

7次SV构建成功；9次实际运行中7PASS＋2预期watchdog失败，不称全部通过。失败例在终止检查前退出，没有READ-TAG-TERMINAL；七个成功例标签fault/queued均0，字节/排空/守恒检查通过。6测试源变化，76生产SV哈希不变，无综合/Linux/BIT/SD/板卡动作。48B仅活动DMA小数据FIFO，不含公共TC、元数据、控制及新增头序号/选择状态；默认mode0保留旧代码路径，本轮未另跑默认路径等价验证。

**机制创新判定：该实现可由普通共享选择队列＋同样的旁路表达，停止把它作为独立新规则推荐。** 本例H1/H2失败不是通用最小容量或进度证明，(K−1)L仍未证；工程原型与容量假设保留。下一主线先在真实四DMA/两bank约束下，找普通规则同信息/同资源不能给出保证的具体边界，明确新增状态和非平凡进度/成本结论；简单推广普通组件不足以支撑二区。

[完整报告](../out/header-park-poc-20261007/README.md)／[9次原始结果](../out/header-park-poc-20261007/runs.json)／[阶段清点](../out/header-park-poc-20261007/stage-evidence.json)／[源差分](../out/header-park-poc-20261007/test-source.patch)。DeepSeek提出按源FAST标签补丁、证据/文档草案；Codex亲自设计实现核心并发机制、核源、修订应用、执行验收。DeepSeek初文档模式号及失败例终止标签表述已纠正，原答复和修订均保留。三次调用205006 provider tokens，均high/393216，完整返回；未调用Claude或新Codex子agent。

## 25. 物理写尾未排完时提前放行源读：改善与普通规则相同

2026-10-08，用户继续推进创新；沿用10月7日artifact目录。[完整报告](../out/payload-frontier-poc-20261007/README.md)／[Codex关键审定](../out/payload-frontier-poc-20261007/codex-review.md)／[运行与行号](../out/payload-frontier-poc-20261007/runs.json)。

Codex亲自提出：目标匹配physical AW已准备、其物理前序写payload已通过virtual WLAST进入TC时，当前virtual W头的源读不必继续等physical W头完全一致。先实现完整前缀扫描，再同更简单普通virtual-W-head＋matching prepared-AW比较。普通版扫描只作fault16独立见证，不进入准入决定；旧准确当前/下一段门、VALID锁、字节及fault检查保留。virtual WLAST不是DDR完成或最后B。

两早期无夹具PN校准严格/扫描均92374，FAST62、额外状态0。随后共同DDR有限夹具：每completion 256beat写接受248beat后暂停WREADY64cycles。最后三模型各P/PN一次：

| 方法 | P cycles | PN cycles | PN额外窗口grant |
|---|---:|---:|---:|
| 原严格虚拟/物理写队头门 | 101731 | 102686 | 0 |
| 完整物理前缀payload检查 | 95692 | 96267 | 96 |
| 普通虚拟写队头＋目的AW准备 | 95692 | 96267 | 96 |

V/A每窗各64次4KiB，PN FAST62。PN相对严格耗时少6.2511%、固定V/A吞吐高6.6679%；P耗时少5.9362%。最强普通PN仍慢P575cycles/0.6009%，不支持PN领先。每窗写stall16384/写拍65536相同；CPU polling请求数量随耗时改变，不能说全部DDR请求完全一致。完整扫描与普通P、PN两对已记录CSV各逐字相同，[轨迹比较](../out/payload-frontier-poc-20261007/trace-equivalence.json)；仅此矩阵等价，不是所有输入或四DMA的定理。

因果样本：同一PN窗口A第二写在653开始64cycle暂停；扫描/普通版654发V源读、657到DDR，严格版725才发；暂停717释放，V首数据707返回。展示可与已完整供给的前序写尾重叠，未证明MIG/原板上PN退化原因。输入demux的正常W所有权锁限制跨bank AW前进，源读准备门限制payload供给；这帮助解释额外扫描为何在此模型不提供独立决定。详细正常job/last-B/epoch限制见审定，不把frontend job_fire误当backend开始。

8次实际仿真全部PASS（2早期校准＋6最后模型）；5SV构建成功＋1夹具日志函数名笔误失败后修正。四tests源变，76实际编译生产SV前后hash同；无Linux/BIT/SD/板卡操作、无综合或默认路径重复回归。READ-TAG fault/queued0，E2 pending/occupancy/metaqueued0，read proxy capture=issued1284、empty1/debt0，received=replayed328704；PASS包含逐tick旧fault及fault16监视，不是形式证明。48B仅两活跃DMA小FIFO，不含共同TC/元数据/控制。wait窗口值为每cycle符合额外条件源数之和，含grant那拍；raw含warm/排空，posedge后窗口快照修正pre-edge差分偏一。

**独立完整扫描创新未成立，降为工程对照。** 已核[AXI-REALM DATE2024官方全文](https://past.date-conference.com/proceedings-archive/2024/DATA/493_pdf_upload.pdf)III-A写数据准备门，以及[2026 AXI VC预印本](https://arxiv.org/html/2607.01430v1)中的DMA R→W依赖；通用ready/消除等待环不能当首创，前作也未证明本平台全部谓词/异常行为。

下一仍性能/成本主线：将真实四DMA IDs0/4/8/12、两bank（0/2、1/3）、copy epoch与burst、共享TC头/写顺序置于同一规格，先找普通准确门、共享选择＋旁路、每DMA队列、完整返回预约在同信息/容量下无法兼顾提前翻译和进度的合法状态或非平凡成本结论。此轮未实现四DMA；简单扩普通队列不作为新贡献，无创新/二区验收，不自动BIT/172。

[阶段清点](../out/payload-frontier-poc-20261007/stage-evidence.json)／[差分](../out/payload-frontier-poc-20261007/test-source.patch)／[DeepSeek完整指标稿](../out/payload-frontier-poc-20261007/evidence-answer.md)。DeepSeek观测接口/C++统计草案、指标与记录草案；Codex独立设计新门与强对照、修改关键源/夹具、纠正测量及job复用推断、核原源/原论文、执行编译仿真和最终验收。三次high/393216完整返回共378225 provider tokens；无Claude/新Codex子agent。

## 26. 提前翻译后仍等待写路径：读延迟干预与四DMA容量边界

2026-10-08接§25继续。**复杂扫描与普通方法轨迹相同，说明当前输入下放行决定等价；不能单凭同效就认定还有瓶颈。** PN已提前翻译却没有总耗时收益，则需要沿后续请求路径定位。[完整审定](../out/read-service-boundary-20261008/codex-review.md)／[四次运行及行号](../out/read-service-boundary-20261008/runs.json)／[阶段清点](../out/read-service-boundary-20261008/stage-evidence.json)。

复用普通virtual-W模型，无写尾夹具，两真实DMA两bank、A8/gap0/V64/warmup256、三拍私有FIFO。Codex只改测试C++：读服务四类互斥计数，以及仅measurement_phase生效的准备延迟重叠选项；从AR接受时计时，孤立请求首次R仍32cycle，保持R顺序、VALID和数据口速率。两模式预热行逐字一致，0次SV重建、1次C++重链接、4次仿真PASS：

| 读准备延迟模型 | P cycles | PN cycles | PN−P |
|---|---:|---:|---:|
| 原队头串行等待 | 92109 | 92374 | +265（0.288%） |
| 从请求接受起等待，可重叠 | 85339 | 85984 | +645（0.756%） |

四项V/A均64，PN FAST62。串行模型明显影响绝对耗时，但去掉这一限制仍无PN领先，因此不是PN不足的唯一解释。互斥服务账本严格加到窗口耗时：serial PN−P为读队列空+241、准备等待−31、读拍−8、R反压+63；overlap为+443/+128/+64/+10。差额主要表现为空闲，不等于具体模块因果或板上内存带宽饱和。DMA数据读256/DS读93相同，CPU轮询请求数随窗口改变。

[轨迹解析结果](../out/read-service-boundary-20261008/trace-analysis.json)按源ID/地址/len完整匹配P、PN各V128/A128条capture→arfire→DDR请求，捕获到DDR均5cycles；FIFO初始pending0重建空区间总量4351/4592精确匹配新计数。PN的62 FAST逐一配到V源读：前/后burst各31次，翻译返回到proxy捕获1051/1052cycle，直到DDR1056/1057。这是提前翻译后的等待，不能当成可全部消除或可累加的收益。

首FAST在2483cycle；A页表fill2933、第二段最后R3518，V AW等待3524解除，V源读3534捕获。入口flags17的bit0直接表示已有W尚未排空、另一bank新AW不能进；普通virtual门还要求当前virtual W head＋准确matching prepared-AW。这里地址已译好，但写路径/目的准备还没轮到V，追加扫描不能解除同一限制。**尚未通过仅改变该写条件的安全干预，量化它对265cycle总差的独立因果贡献；当前定位不等于修复/创新证明。**

同时完成四DMA接口/受限抽象边界：[审定](../out/four-dma-boundary-20261008/codex-review.md)／[结果](../out/four-dma-boundary-20261008/results.json)／[状态见证](../out/four-dma-boundary-20261008/traces.json)。生产四源ID0/4/8/12分入两bank，但完整copy保证/大FIFO仅前两路；现test仅两个真实iDMA。模型限定四源各一个两burst任务、固定写顺序、36种同源保序读交错、派发原子R→W/假定AW可备；九策略324抽象分析、34680累计状态，未跑四真实DMA RTL。盲目共享2/3/4槽各30/36组合可达停滞，普通当前W-bank预留2头即可0/36，普通每bank2头也是0/36。没有包含原TC额外头槽、FAST/PTW/周期/异常epoch，不是通用最小容量或新定理。纠正DeepSeek把bank当唯一身份及same-bank必fault12的推断，未采用。

**下一仍性能/成本主线：停止扩复杂扫描，核跨bank写锁定和源读供给的最小安全变化，并与普通顺序队列/预留/准备门同信息同资源比较。** 提前翻译、共享槽、预留本身均不能当创新；先证明强普通方法遗漏的限制，才值得加机制。当前未确认独立创新或二区水准，不自动BIT/172。

仅`tests/nonblock_actual_dma_test.cpp`改变、76编译生产SV哈希前后相同；无Linux/BIT/SD/板卡操作。旧§25二进制冻结为legacy-binary，旧日志/CSV保持，同obj现已C++重链接。DeepSeek三次接口证据与抽象/轨迹脚本提案385750 provider tokens完整high/max393216返回；Codex独立设计合同/干预、原源诊断、审核脚本并执行验收。无Claude/新Codex子agent。

## 27. 原论文驱动创新筛选：分段错配与部分返回预约

2026-10-08接§26，按用户“在阅读论文的情况下探索创新”。[原文/源码完整审定与机制规格](paper-driven-innovation-20261008.md)／[完整资料](../out/paper-driven-innovation-20261008/README.md)。本轮对照14篇机制段落，新增iDMA2024、CTC2009、AlphaServer1996原PDF与TXT；不是新增14篇或通读14全文。经典/前沿共同约束：翻译预取、credit/软隔离、关键性调度、共享返回流控和小FIFO直通已有，不能改名当创新。

Codex独立设计并筛选：A预翻译后承诺写顺序须移到源AW首次VALID之前；B阻塞链调度没有区别普通owner优先的合法性能反例；C完成头寿命记credit与普通队列预留重叠。三者均不升级独立贡献，暂不实施。保留D窄候选：源一次R2048，目的W1024+1024、首段翻译/独立排空保证成立时，前1024直通、后1024预约共享池，等物理RLAST释放返回服务，再回放尾部；原逻辑RLAST仍按真实全部数据交付。仅条件性容量/微架构规格，非仿真、最低容量或创新证明。

实际source/frontend/Linux链路确认：64-bit/256beats合法化边界2048B；CONF bit0→decouple_rw；Linux/test用CONF=0共同分段，原2KiB错位copy在此配置R/W均1024+1024，需bit0=1才R2048/W1024+1024。因此D不是旧PN退化解释，不把存在配置能力当板端使用事实。与RAWCouplingAvail编译参数区分。

纠正DeepSeek：无时间模型只看进度不排除性能差；已接收AR顺序可观察；MASK token非context occupancy；允许R反压不表示后续PTE能越过；p承诺不能无资源补充地缩水；瞬时WREADY不保证独立排空。比较须用准确per-segment ordinary cut-through而非弱基线；其若同样预约后缀，D机制就等价。未找到同款论文不足以宣称首创，可考虑的论文形态是明确的容量—长burst—翻译服务边界与更低成本实现，但尚未成立。

下一只做上述独立分段的真实iDMA最小判别，比较整预约F、共同分段短读S、准确普通直通池C、D；同字节/在途/翻译状态/公共FIFO。C同决定同成本或S同吞吐更简单就停主贡献推荐。没有执行此判别，不先Linux/BIT全构建或扩172实验。

本轮仅阅读、规格/文档与公开PDF下载；生产/test/Linux修改、新测试/仿真/构建/BIT/SD/板卡动作0，实际最新仍§26。DeepSeek三轮证据/质疑448607 provider tokens完整high/max393216；Codex亲自设计、阅读关键原文/实际源码、裁决与纠错并执行资料保存。无Claude/新Codex子agent。

## 28. 继续创新筛选：依赖信息、翻译寿命与写承诺

2026-10-08接§27。用户继续探索创新；[本轮原文/接口审定与候选规格](dependency-contract-innovation-20261008.md)／[任务和完整答案](../out/dependency-contract-innovation-20261008/README.md)。新增MetaSys2022、TAPA2023、XDMA2025三论文PDF/TXT及LTI2020、Intel DSA June2025两官方规范，均核关键段落，不称通读所有全文；旧Walk Scheduling ISCA2018 batching/SJF重新核。

Codex独立E/F草案后确认：现测试从iDMA内部job/src/dst/len取得额外copy语义；引擎内部确有依赖，AXI消息没有定义这套关联合同，USER仍可扩展。LTI允许ordering满足前译址、SPEC仅预取；LC不依赖其它Stall response，多事务使用须在全球可观察后LC；现TC地址ready后的COMPLETE不能当同协议安全释放点。NoStall/静态映射限定与故障扩展分开。

D准确byte credit可表达同预约、E通用metadata与TAPA显式流控/XDMA协作重叠、F朴素成对slot预约与ready队列已有强普通对照。能力可表达不等于已有发表同款，不据此禁止增量组合创新；目前确实没有建立候选比强简单基线更好的决策/成本。有限pin槽互占仅拟议结构条件反例，不是生产RTL新bug。

下一窄问题是可丢弃准备与真实W承诺之间的译址结果寿命：准备期不长期pin；真实AW首次VALID前复核src READ/dst WRITE和具体进度资源；真实提交后保留映射至合法结束点，不撤回VALID/重试已接受burst。候选未实现；旧草案地址握手后退handle不作为LTI/失效正确方案。主线仍共享IOMMU性能/成本，无二区认证。

先复用CONF0真实iDMA作冷热一端＋准备后替换两最小形状，与当前W-head、普通联合准备ready队列、普通可丢弃预取复核、W完整后AW同合同/总资源比較。普通方法同效同成本就停复杂规则；重做吞噬收益就停该实现；没有执行判别/扩矩阵/BIT。D独立分段判别亦未执行，本轮优先更贴近现PN动机的问题。

DeepSeek2次成功high/thinking/max393216、stop/未截断共366895 provider tokens，另一次模型前路径验证错误无usage。Codex纠正worker的PFN/copy分类、prefix连续性、credit双归还/泄漏/守恒、LTI误读及资源单位混淆，原答复保留。本轮仅研究文档/资料保存；新生产/test/Linux代码、测试/仿真/构建/BIT/JTAG/SD/板卡动作0，实际最新仍§26，未有四真实DMA或新性能/面积证据。无Claude/新Codex子agent。

## 29. 翻译返回对照与新准入规则反证

2026-10-08接§28，用户强调需要创新。**专用翻译通道/VC、页表优先、朴素动态burst不是新贡献；Codex新G/H草案亦未认证创新。** [原文/源码与完整审定](translation-lane-and-burst-rule-20261008.md)／[两项新运行与全答复](../out/translation-lane-check-20261008/README.md)／[清点](../out/translation-lane-check-20261008/stage-evidence.json)。

生产DS与comp已有独立发起端口，经单DDR目标/clock converter/MIG共享返回服务；root补核peripherals四DMA mux与DS固定50/50延迟器。共享R端口不意味着MIG必须FIFO，不是DS与payload共用同一byte-credit池。test只有两个真iDMA，debug闲置，DMAcfg直连；不称四真DMA实验。

复用§26旧binary，无重编；只启用现成窗口内DS-priority。同单口一拍/周期、共享16队列、准备31不变，新增P/PN两项exit0/PASS：P88989、PN91547cycles（旧FIFO92109/92374），V/A各64、DMA AR256、DS完成93、FAST0/62。DS从DDR AR到首R均值/最大两者都32、A PTW-active都13489，但PN仍慢2558（2.875%），不能认定全过程翻译/准备已相同。暖场各自旧对照逐字相同；字节与终止排空通过。PN−P账本empty/setup/transfer/backpressure为+1312/+992/+256/−2；合成CPU读221/253随闭环服务改变，不是Linux轮询，未做独立因果干预，不认定带宽饱和或W锁定解释全差。

新增VDC2011、Arm DMA350官方两PDF/TXT，另网页核TaPaSCo/H2RC2019 §IV-B（下载代理503），重核MASK/AMD来源。VDC已有页/FIFO动态burst，TaPaSCo已有多引擎R/W互堵和完整预约，DMA350明确完整R空间/W数据再提交，均为强普通对照。TI原专利可提前结束其合同下burst，不能照搬已定ARLEN的AXI；2026 adaptive-ECC/CN专利正文缺口保持。

Codex先提出G：在虚拟AR首次VALID前由已备且独立可消费目的前缀p决定源读L，长R仅预约不可排空后缀。greedy最长可行在credit0或p≥Lmax同决定；有credit时G更保守，但未证收益。p0允许安全预约回退，不能一律拒绝。随后H只对具体跨copy进展依赖保留最小资源并切短；当前owner若有p>0，普通切短＋直通可能已不需byte池，非零保护量和独有决定未证。未实现G/H。

DeepSeek反例人为共享PTE返回/payload信用，与当前接口不同；Codex不采平台可达/创新结论，不按worker建议加该池或先实现。下一先给同信息同资源下候选比普通动态分段＋准确尾池＋owner直通、整R预约＋W齐备门、固定预留与DS优先的真实决定/成本差异；无见证停止G/H主贡献，不先生产RTL/BIT。旧CONF0对齐输入不产生新分段错配，不拿候选解释旧PN。

DeepSeek两次high/thinking/max393216完整stop共340752 provider tokens，提供连接证据、原文抽取/反证；Codex独立设计、直接核关键原文/实际代码、执行两实验、拒绝无效反例并审定。源码/编译/Linux/BIT/JTAG/SD/板卡动作0，无Claude/新Codex子agent；既有实板172结论保持。

## 30. 译址后分段候选与单 DMA 几何反证

Codex 独立提出 I：保留原逻辑源读，在源翻译完成之后、内存物理 AR 首次 VALID 之前，根据对应目的的有效译址和具体写序选择片边界，恢复原 ID/拍数/RLAST。此机制未实现，透明分片和动态粒度不能算新贡献。[完整审定](post-translation-fragment-witness-20261008.md)。

复用旧 Mode0、512拍大缓冲模型，只重编 C++，两种 CONF 各执行一次均 PASS。源从页首读2048 B，目的从页偏移3072写2048 B：CONF0读1024+1024，CONF1读2048，目的均写1024+1024。完整数据/未写区域/排空均正确。第二目的 physical AW=1456，首源 AR=1899，首R=1931，说明目的先准备好了，不能把该场景用作 I 的收益证据。两个普通模式末R差31周期，亦不是新机制加速。

新核 AXI-REALM 透明拆分/RLAST机制、2026 Preemptive VCs 的 DMA依赖死锁与轻量抢占机制、CN114780466B 的源目的页首地址前置。下一步先核不依赖强制逆读/交错的自然多 DMA 写序，必须观察部分可推进和整段可推进两类状态，再比较共同边界、精确尾部预约、固定译址后分片及同信息的普通动态选片，计入全部元数据。公式能被简单硬件表达不等于已有发表，也不等于已建立新贡献。

[两次真实运行与模型限制](../out/adaptive-fragment-witness-20261008/README.md)。DeepSeek取证及驱动草案，Codex独立机制分析、直接核源与原文、执行与判断。0SV构建、1C++编译、2仿真；旧模型诊断参数限制保留。当前生产/tests/Linux/BIT/板卡均未改；创新和论文等级未成立。

## 31. 自然同 bank 写序：工程计数回绕与读尾等待环

2026-10-08接§30，[完整审定与I修订规格](natural-write-order-boundary-20261008.md)／[实际命令、日志、CSV和清点](../out/natural-write-order-20261008/README.md)。新out副本两个真实iDMA同bank、普通S，无强制逆读/AW交错/源读门/owner门/DS优先，删除TEST_COMPLETION_GATE；单序列行为DDR并非MIG。源页首、目的偏移3072，2KiB和32KiB，V先/A先。

Codex运行前独立指出C0：每bank配置4项、外部写计数3bit，原AW只依据overflow禁止；第8项使q回绕0/overflow1，WREADY/WVALID被关闭。两长任务均实测成立，第8项是bank接受，DDR只观察7AW。out复制TC提前按配置容量禁止AW后，最大4/无overflow，原两短任务的CSV逐字相同；生产源码尚未应用修复。此为工程缺陷及修正原型，不是算法创新。

排除C0后，32KiB/CONF1仍在自然写序停滞。V先例source AR5657为80002800/2KiB，对应目的81003400..3bff，前1KiB可接到A之前的V写段、后1KiB属A之后的V段。后V目的AW5649早已准备，关键是实际W顺序空洞而非缺目的译址。6098 V前写WLAST后Whead=A，6100起V R尾反压；软件队列快照确认A根页表30000读取在其后，A首写got0。[可复核见证](../out/natural-write-order-20261008/dependency-witness.json)。1024B前缀须扣前序V数据义务，不当免费当拍排空保证；快照不单独指定该A页表访问归属哪项逻辑事务。

普通CONF0共同分段两顺序仍停在来自错误owner的完整短R；普通512拍缓冲两顺序都完成，各DMA源/目的4096拍、总32AR/34AW，完整数据/未写头尾和终止排空通过。两路iDMA FIFO总量48/8192B，不含公共TC/AXI cut/元数据，未综合面积；最后DDR W24411不是新机制性能。短读单独不足，不等于所有普通owner调度/预约方法都无效。

Codex修订I为译址后按虚拟/物理W义务选择child边界＋跨ID parent/owner服务，恢复原ID/拍数/唯一最终RLAST，不撤回VALID、不改已收burst。只切短但占唯一parent槽可能继续挡A；2parent槽不是充分性定理，同owner先填满、上游队头、元数据取得和异常/epoch仍需解决。本轮未实现I。AXI-REALM透明拆分、2026 VC读写依赖、TI EDMA3源目的分段/TR流水均为前史，不把拆分和owner优先称新贡献；TI仅核索引原文段落，PDF直连未获。

下一同资源比较固定后译址1KiB＋相同owner调度、普通共同分段＋准确W-owner门、精确尾部预约/per-ID选择旁路、大缓冲和页表优先/可交错返回。普通方法同效同成本则停动态规则主贡献；有独有净收益才推广真实四DMA与综合。创新及论文等级未成立，不先BIT/172。

3次SV+C++构建＋2次C++重链接成功，13仿真6PASS/7watchdog；失败completed/verified=false，其中1次队列快照与先前长V例CSV逐字相同。76生产SV哈希不变，无生产/tests/Linux/BIT/SD/板卡改变。测试两port同bank映射非生产四路原样、缓冲3/3或512/512非生产512/3；长错位CONF1不解释旧PN。DeepSeek驱动/只读观测草案和聚焦修订，两成功high/thinking/max393216完整返回249657 provider tokens；Codex独立设计、原源/前作核查、诊断修正与实际执行验收。无本轮Claude/新增Codex子agent。

## 32. 普通准确源读门完成：小缓冲可推进，尚无新机制贡献

2026-10-08接§31，[完整结果及下一门槛](write-owner-baseline-20261008.md)／[命令与清点](../out/write-owner-baseline-20261008/README.md)。Codex新out普通门仅门控各DMA源AR，在virtual/physical Whead同owner、相同完成序、对应AW prepared及源/目的offset/len/size严格匹配时首次offer，运行held保持VALID至握手。采用真实descriptor、8槽可回收AW ring与宽序号，未硬编码PA；生产/tests均不改。

旧SourceReadGate/Frontier只进Mode>=4 latepool，固定4KiB aligned/Depth8/len255，不能用于新32KiB错位。本轮Mode0的新out接口实际接入独立门，无TEST_COMPLETION_GATE，生产banked_ctl和§31写容量修正副本保持，无强制逆读/交错/CPU/预热/batch/DS优先。

1次新SV+C++构建＋1次旧大缓冲C++重链接成功，4运行4PASS。均CONF0/两流各32KiB/src0/dst3072，BUF3＋门/BUF512无门分别V先/A先。四项各路32源AR/32目的AW/4096拍R-W，总64AR/64AW；完整bytes、源和未写区、NEXT-DONE、quiet通过，payload R反压0、other/DS208，无计数溢出。两gate CSV128次virtualAR独立核当前W owner/offset/len127/got0匹配。[审计](../out/write-owner-baseline-20261008/gate-admission-audit.json)。

两路iDMA dataFIFO48/8192B，只计payload，不计门descriptor/AW ring/计数、共同FIFO/cuts，未综合面积。最后DDR W owner27894/full25146，两个启动顺序相同，差2748；首virtualAR1032/68，首DDR源AR2880/1934。均为reset-relative轨迹时刻，非独立copy完成窗/平均V-A吞吐/纯译址成本。源门通过正常反馈也改变自然AW出现顺序。累计gate waits55046可超过周期，是被挡流数逐拍和。

**普通门已解除这一CONF0等待环，因此不再把解除停滞本身作为候选贡献。** 下一保留“提前逻辑译址与后端服务粒度/总成本”的问题，先固定后译址1KiB＋相同owner调度，再与动态I、普通源门、准确尾池/per-ID旁路和大缓冲公平比较。原parent同ID最老顺序、最终RLAST、实际AW部分消费、上游队头/容量及可分片AR属性要核；2槽不是充分性定理。固定同效同成本则停动态主贡献。候选I与固定post-TL均未实现、创新与二区未成立。

复用§31 CONF1大缓冲几何仅提示虚拟Ax66对本CONF0的128，不能推PTE/miss/净性能。重核[AXI-REALM III-A1](https://arxiv.org/html/2501.10161v1)透明分片/RLAST与支持属性，位置/粒度不称首创。Codex还纠正worker：旧can_drain确有offset/len/size；旧release327–328赋值在reset分支，不能当有效runtime sticky，新门hold在else。未修改旧test门、未推旧验收全部无效。

76生产SVhash同，无生产/tests/Linux/BIT/SD/JTAG/板卡动作；仍两真实constant-ID正常静态job的同bank重分类、串行FIFO DDR，非生产四路混合512/3/MIG/旧PN解释。DeepSeek两轮接口/普通脚本草案与修订343532 provider tokens完整high/max393216；Codex独立设计关键RTL、直接核源/前作、执行编译运行和裁定。无本轮Claude/新增Codex子agent。原始全答与失败推断保留，当前最终以审定为准。

## 33. 固定与动态后译址分片已判别：当前独立主创新不立项

用户要求推进到能判断独立创新。Codex亲自实现两个可切换规则，接在译址后completion AR和R返回之间，保留原逻辑父、按每ID最老处理，未完成父可暂停服务另一ID。真实descriptor/source握手关联offset；虚拟AW与已接受物理AW独立owner/段序环、实际W拍进度，共同连续已准备前缀才安全发源读。固定max1KiB，动态按最长前缀，本例1/2KiB。两流各8父槽、一次一child，无新增R数据缓存，父ID/RLAST保持，AR全结构属性保持并核合法拆分条件。无通用故障/失效生命周期或最小容量证明。

1 SV+C++构建成功，同binary六运行6PASS（31cycle固定/动态各V/A先，100cycle仅V先一对）。CONF1两路各32KiB/src0/dst3072；均32逻辑源AR＋34目的AW，固定64物理源AR、动态55，9父合并，payload两FIFO48B。每路4096R/W拍、bytes/源/guards/NEXT对应DONE/quiet；验收516项绿，包括有序父子覆盖与日志片选择对应实际DDR AR。[结果](posttl-independent-verdict-20261008.md)／[验收](../out/posttl-independent-20261008/analysis.json)。

31参数全体末W20213→20766晚553，V先V末W17530→18005/A20213→20766；A先镜像。100末W31805→31737早68，但V27604→27734晚130/A31805→31737早68，两个末W平均晚31。firstVAR全68，这些是共同reset数据时刻／源请求至写末拍观察窗，不是DONE/稳态吞吐。参数名ptw-latency影响全部行为DDR读，不能当仅PTW干预。请求减14.0625%不是miss/字节带宽或独有翻译减少。

声明模块非统计5962bits＋观测160bits，不含公共TC/cuts、含部分范围检查状态，未经综合。固定独立实现可裁剪多项扫描；不能宣称动态同总面积/更省。固定也保留原父逻辑请求并解除本例停滞，因此当前不把动态最长前缀作为独立主贡献。不是已有论文完全同式的证明：AXI-REALM已有透明分片/RLAST、DMA350已有整读/整写条件、Preemptive VC已有DMA RW环，所核段落未证明完整联合覆盖，少量性能差也不逻辑否定新颖性。立项不通过是因强普通覆盖主要收益，剩余差异不稳定且无成本/通用进度证据。

保留固定后译址分片作强基线。下一贡献须给总资源约束下译址/服务/进度的必要条件或强固定达不到的具体机制，先独立差异见证，不继续包装最长前缀，不自动BIT。76生产SV哈希同，无原tests/生产/Linux/BIT/SD/板卡动作，沿用§31 out C0修正、production C0未改；非四流/MIG/旧PN根因/二区验收。DeepSeek取证与普通验收稿/修订，Codex设计关键RTL、核源/原文、实际执行与审定；三次high/max393216、stop/非truncated，总658044 provider tokens。PDF换页符编号差被核清，Codex误报引用错误已撤回；脚本首次SyntaxError来自Codex误截围栏，完整提取后字段/分组修订验收通过。[清点](../out/posttl-independent-20261008/stage-evidence.json)／[关键审定](../out/posttl-independent-20261008/codex-review.md)。

## 34. 固定方法父槽1／2／4与8同效：继续收紧独立创新门槛

用户“继续”。Codex独立参数化已译址父保存槽，virtual源关联和两侧AW环保留8、iDMA和TC深度保留。3次SV+C++构建exit0，1/2/4槽各V先/A先共6新仿真全部PASS；2份8槽旧§33基准明确reused，不重编重跑。480汇总条件全过，六新CSV逐字节等同对应旧8槽，均逻辑源AR32/目的AW34/物理子AR64、每流4096R/W、完整bytes/源/guard/NEXT对应DONE/quiet。V先末DDR W17530/20213，A先镜像，全体20213；reset-relative数据端点不是DONE耗时/稳态吞吐。

当前1槽无可观测损失，不能反推旧8槽内部高水位1或一般单槽进度。物理父每项171bit、两流各删7项，声明state5962→3568少2394，仅metadata，观测160另计，无综合/公共FIFO/cuts/整板面积结论。natural强制S、CONF1、两真DMA同bank、各32KiB/src0/dst3072/48B iDMA数据FIFO，无CPU/预热/反序/AW交错；不是P/N/PN、四DMA/MIG/旧PN解释。

Codex核[AMD2009 US7480784B2 Fig10](https://patents.google.com/patent/US7480784B2/en)：已有IOMMU write-miss/PTW/P2P依赖环和专用VC。未证明同式双W-prefix覆盖，但不能声称首次发现IOMMU环/专线本身创新。固定基线比原型估计更省，动态最长前缀当前独立主贡献仍不成立。

下一受控规格：同CONF1和AW几何/owner信息/总成本比较前置固定分片与后置固定分片；有差异再加普通提前翻译准备强基线，不能以缓存命中的重复TL请求直接当多次PTW成本。此三项未实现/运行，独立机制和二区未确立。[判别报告](parent-capacity-boundary-20261008.md)／[实际raw/退出码/复用分类](../out/parent-capacity-20261008/README.md)／[关键审定](../out/parent-capacity-20261008/codex-review.md)。

76生产SV哈希同，原tests/生产/Linux/BIT/SD/板卡无操作。DeepSeek3次证据/普通汇总草案修订，high/max393216、stop/非truncated、实际usage793120 provider tokens；Codex个人设计、核源/前作、执行审定与补同ID真实打印序检查。初稿argv格式与修订Markdown层级错误后修好，没有额外DUT复跑。[阶段清点](../out/parent-capacity-20261008/stage-evidence.json)。

## 35. 固定分片位置正结果：下一用普通父翻译复用判别创新

2026-10-08接§34，用户“继续”。Codex独立实现preTL原父保存和固定128拍分片，复用§34每流1父postTL基线与同双W owner/seq服务、原ID/256拍/final RLAST恢复，一child在途，无新R payload。pre接在两DMA共享AR仲裁之前；post保留8项virtual关联/原TC队列，逻辑入口容量有别，不能称等总资源或单变量。

1新SV+C++构建、3仅C++重链接均exit0；8新仿真pre/post×S/N×V/A先全PASS。CONF1、两真DMA同bank、每流32KiB/src0/dst3072、总48B iDMA FIFO，无诊断反序/交错/CPU/预热/DS优先。均32原父、64physical AR、34AW，各流4096R/W，bytes/源/guards/NEXT→DONE/quiet通过；最终902raw条件过。post/S两CSV等§34；N四CSV等对应S。

V先真实硬件issue→done前24162/27436后17471/20149，均值25799→18810，低27.0902%；A先为确定性镜像，不是随机重复统计。末DDR W前24221/27500后17530/20213，和DONE分别报告。TL98→66/hit64→32，但miss34/实际PTW102/DDTW2相同。少的是重复命中请求，不是walk或byte流量；整个周期差不能全归32hit，还改变early translation/AW反馈与入口保存。

Codex原先预计用现成N作有效快路径强对照，运行FAST0后直接核rv_iommu_tl_wrap.sv:303–306，window要求跨bank，本例同bank不触发；已撤回“该对照检验有效快路径”的解释。下一普通父请求首次翻译后供同页剩余child复用，必要时提前准备，与后置按全部metadata/TC容量配成本。若普通同效同成本，当前位置规则不能称独立创新；若新约束下独有决定/进度/成本且核清前作，才可建立贡献。§33动态负结论保持，不承诺二区。

模块声明前2502/post3568bits、观测160另计，不含公共TC/cuts，未综合；新增pre_parent_ar UNOPTFLAT例示路径检查不等全lint/时序无环验收。静态有效页内正常任务，不含fault/epoch/失效寿命/通用四DMA/MIG/旧PN根因。

76生产SVhash同，无原tests/生产RTL/Linux/BIT/SD/板卡动作。DeepSeek3次high/max393216完整stop/nontruncated，观测/汇总草案修订，usage332487 provider tokens；汇总初稿24条件失败来自总数/单流与other解析，聚焦修正仅重解析raw无DUT重复。Codex设计关键RTL、核原文/源、实际构建运行及验收，另核八CSV实际child AR地址/ID/len顺序。[完整判断](fragment-placement-boundary-20261008.md)／[八次原始证据](../out/fragment-placement-20261008/README.md)／[审定](../out/fragment-placement-20261008/codex-review.md)／[阶段清点](../out/fragment-placement-20261008/stage-evidence.json)。

## 36. 普通父翻译复用与当前父提前准备：主要差距未消除，可见范围必须配平

用户“持续推进”。Codex实现TC↔TL普通memoization，真实父头/seq输入、完整TL context匹配、成功完整响应保存、spaddr+页内offset复用，不改原前置分片/双W服务/物理AR/RLAST或payload。普通模式首次source child照常译址，第二child复用；early模式在TC无新VALID且idle时译址已经接受的原父，offer寄存保持，背景响应自己接收，正式两child复用。

3新SV+C++构建全exit0、10新运行全exit0/PASS；首四SVplusargs未生效都enabled1，明确excluded，不算关闭对照。Codex初context归因不准确已撤回，生成代码直接enable1，确切消除原因未追；显式C++输入修正4bound0/1和2early2为6有效新对照。4旧§35pre/post只引用；原日志/首次源码/旧模型保留，不因整理删。

关闭两CSV等旧pre；V先真实V/A delta关闭24162/27436、复用24084/27340、提前24086/27342，均值25799/25712/25714，改善0.337/0.329%；旧post17471/20149均18810，剩6902/6904，A先镜像非随机重复。真实TL98→66、hit64→32，均miss34/102PTW/2DDTW/64物理源AR/34AW/每流4096R-W/完整数据与quiet。early prepared32/filled32/reused64，末slow/hit/valid/offer0。少32重复TL处理在该路径不足以解释27%差距，不说普通ahead普遍无效。

旧post firstphysicalR2404之前，源原VAR在68/69/70/71/2367/2374先接收六个2KiB父，且没有payload可完成它们。是整条路径原父在途至少6的实际下界，不是某slot内部高水位；pre每流1raw父，当前父完毕才接受下个，early只当前已接受父，未配相同未来可见范围。

模块声明state普通3514/提前3518/post3568bit、观察另计，无综合/总面积/同logicalcapacity；原数据FIFO48B。同bank两真DMA/CONF1/src0/dst3072/各32KiB/全部行为DDR读31，未fault/epoch/失效/通用进度/四DMA/MIG/PN根因验收。背景fault可见性待生产处理，不因加flush线说安全回退已完成。

本轮重核Arm官方LTI IHI0089A §6.2.2同页同context单译址共享和完成寿命、§1.1排序条件前译址；§4.2SPEC不是普通数据使用许可。所以普通父复用/提前/单加队列不能直接立新规则。下一普通同原父可见范围/总成本，复用header/TC队列或稳定VALID下一父的无副作用准备；该版未实现。如果普通同效同成本，当前位置规则不能称独立创新；新约束下独有进度/成本且核清前作才可建立贡献。§33动态负结论保持，不先BIT/172。

DeepSeek接口取证与汇总草案/offset和字段revision，3high/max393216完整stop/nontruncated共259312 provider tokens；Codex关键设计、直接核源/前作、执行与数学审定。首次14解析条件失败修正只读raw；Codex另核出跨启动取两个第一发起者的均值错误，直接修为固定V先同run V/A，最终778条件过，数学另从原log重算。76生产SVhash同，无原tests/production/Linux/BIT/SD/板卡动作。[报告](parent-translation-reuse-boundary-20261008.md)／[raw分类与清点](../out/parent-translation-reuse-20261008/README.md)／[6原父见证](../out/parent-translation-reuse-20261008/lookahead-witness.json)／[关键数学与state](../out/parent-translation-reuse-20261008/codex-metric-audit.json)。


## §37（2026-10-08）：原请求窗口配平与父结果保留进度边界

Codex设计out源AR到原父RLAST的perflow credit，和physical parent深度分开。4新SV+C++build成功、16新run：10完整拷贝PASS、6看门狗停滞；容量组363条件、时机组48条件正确采集。没有重复旧DUT；失败不算完成平均值，不把collection_valid称全DUT通过。

同原窗口1后置V/A22282/25697均23989.5，普通提前25714，残差低6.706%；新普通可消费owner资格只把均值改25713，仍差1723.5（6.703%）。窗口8／单父槽18810，CSV等旧post。窗口2/4单父槽两序停滞，window2/depth2恢复20151，window4/depth2仍停、depth4恢复18824。现场共享TC卡A下一AR，A旧父槽要等W而双W头等V下一父，12采样冻结；这是Codex原型组合进度缺口，非生产源码或旧PN解释。普通扩槽解本例，不能把加槽、credit或发现此等待环本身当独立创新。

成功均TL66/hit32/miss34/PTW102/DDTW2/AR64/AW34/每流4096R-W/完整数据与quiet；FIFO48B无新payload。含credit声明post3576/3918/4602bits，对普通3518；未综合/非总预算等同，virtual/AW环仍8。新普通owner准备32/填入32/缓存64，pre入口VAR64为child而原父32。76生产SV一致，无原tests/production/Linux/BIT/SD/FPGA动作。

下一普通pre多原父header准备，让它同样看到未来父，并对窗口4/parent4后置配总metadata预算。Codex核normal响应83bits，predepth4+8*(83+valid)=4500声明bits，对后置4602仅余102bits给控制保持；这是预算边界未实现，不省完整有效字段、VALID稳定、原父寿命和失效/错误退路来凑数。如果普通同效同预算就不能立位置/联合规则独立创新；独有进度/成本且强前作未覆盖该约束才可立贡献。PULP xbar、2026PreemptiveVC、LTI、2025MMIO预装前史都限制宽泛首创表述。

DeepSeek3次完整汇总/修订，Codex关键设计/核源/前作、执行及修正跨条件平均和误把不同方法当相等的指标检查。实际数学另从raw核，不因模型共识接受创新。[完整判断](original-window-progress-boundary-20261008.md)／[raw分类](../out/original-window-20261008/README.md)／[执行账本](../out/original-window-20261008/stage-evidence.json)。下一不重扫或重跑本轮，不先BIT。

## §38（2026-10-08）多父准备边界：普通缓存命中直通对照与条件性独立创新判据

- 接 §37。Codex 亲自设计／检查 RTL 与新颖性论证、执行构建／仿真并最终验收；DeepSeek 起草／修订文档。5 新构建、18 新运行、18 PASS；8 次普通模式、4 次读优先、2 次观察运行、4 次命中直通；`analysis.md` 只覆盖最初 8 项，根审计覆盖全部 18 项；新增 10 项是 DUT 运行，不是另加根审计；仅 out 副本，不改 production/tests/Linux/BIT/SD/board。
- 已完成最强普通缓存命中直通 AW 优先 V 先启动 23380/26122 mean24751；旧 post window4 V 先启动 17485/20163 mean18824，均值降低 23.946507%，绝对 5927。它不是普通 TC 旁路，源子片仍进共享 TC。
- prepare0/1/2 均值均 25869.5，但 prepare0/2 CSV+prepseq 相同、prepare1 不同；不能称 3 个独立性能点。读优先非统一最好：串行 lazy 25630、串行 prep 25847.5、缓存命中直通 read 24992，后者比 24751 差。声明功能位 pre3316+helper1281=4597 与 post4602 未综合，不是等面积；原窗口高水位 4/4，未来父 30/32。
- 因为 Arm LTI 已定义 translation-before-ordering／lookaside（1.1／1.2.2）、同 page+context share 与 completion deadline 6.2.2，仅缓存读绕过 TC 或背景未命中不能称为独立创新。如果普通完整 TC 旁路同进度+成本，则位置／组合规则不能称为独立创新；如果联合依赖前沿准入／保留能在同预算下处理该旁路并在故障／失效／生命周期约束下展示独特进度／成本，则能称为独立机制。下一最强普通对照（已准备源片物理 AR 在 TC 写未命中期间通过，full permission／lifetime／BC／error／routing，same original window／saved metadata／payload）未实现；下一优先检验普通物理AR旁路。
- 因果见证：源 V0 缓存填充 1019；写 REQUEST 2421→写 RESPONSE 2861 同一 pending 持续 440 周期等待写；源命中接受 2868 在写完成之后，不能称 2868 时 TC 仍忙写。计数为 reset／drain 重叠，不是延迟分解，也不与 flow completion 求和；整个模型不只 IOMMU。
- 证据与详表见 [multi-parent-preparation-boundary-20261008.md](./multi-parent-preparation-boundary-20261008.md)。

## §39（2026-10-08）普通物理源读旁路消除97.45%原差距

- Codex out副本实现真正绕过繁忙TC的已准备源AR，而非§38只绕后台TL等待。单全局sourcechild固定route至Rlast，保留完整AR属性；源ID0/4加保留位1/5、返回还原。1新build／6新run／6完整copy PASS，旧post只读，关闭两CSV与§38逐字节同。
- 同原window4/父槽4/FIFO48B/CONF1/各32KiB/读延迟31；V先off均24751，cache17704/20246均18975，wait18146/20984均19565，旧post均18824。原gap5927→151，缩小97.4523%；post对最好普通只再低0.7958%（分母18975）。cache A先20279/17737均19008，交换后各+33，不是镜像复制。
- cache62direct/2fallback、写AWAIT_RESP期间directAR25/16；wait64/0、24/24。六项TL66/hit32/miss34/PTW102/DDTW2/FAST0、物理AR64/AW34/各4096R-W，parent32/child64，bytes/guard/ID/len/hold/quiet通过；TC入口var=fallback。原窗口高水位4/4终态0/0。
- 首sourceAR/R普通早897，随后相对差有正负；V父2/5两次offer未准备好回退TC，不断言它们解释全部151或固定流水／带宽瓶颈。声明state4598vs4602未综合、非等总面积；76productionSVhash不变，无Linux/BIT/SD/板卡动作。
- 因为普通旁路有Arm LTI1.1/1.2.2/1.2.3前史且几乎追平，不能用位置或旧23.95%立独立机制。下一先核平台真实结果释放／目的W／失效软件可达依赖，再用普通子片译址／预约／重译强对照。若普通同保证同成本，就不能称独立创新；具体真实约束下独有且前作未覆盖的进度／成本才立题，不能人为加约束求胜。
- [完整报告](prepared-source-bypass-boundary-20261008.md)／[根审计](../out/prepared-source-bypass-20261008/codex-raw-audit.json)／[执行账本](../out/prepared-source-bypass-20261008/stage-evidence.json)／[下一门槛](../out/prepared-source-bypass-20261008/codex-next-boundary.md)。DeepSeek汇总和报告草案，Codex架构、关键源码／官方前作、执行和最终审定。

## §40 翻译寿命边界与创新门槛（2026-10-08）

- 新增正式文档：`translation-lifetime-and-innovation-gate-20261008.md`，整理根 Codex §40 审定、第一席 `innovation_rules_40`、第二席 map、DeepSeek 证据与 source-evidence.json。完整 worker responses 保留在 `../out/translation-lifetime-boundary-20261008/`。
- 结论：因为当前平台没有“普通译址结果必须等 DMA 数据完成才释放、并以该释放反馈阻塞 IOFENCE”的可达路径，源码可达性门槛否决当前 LTI-style 寿命反馈。Arm LTI §6.2.1 独立完成义务与 §6.2.2 多事务寿命分清。
- PR/PW 硬件 TODO 不是创新；Linux 默认 IOFENCE 设置 opcode/func 与 dword1=0，普通同步调用 PR/PW=0；真实 InclATS=0；CQH 早推进不等 generic completion；§39 静态 6 PASS 不验证 live invalidation。
- 普通 fault/error+discard 列为强进度控制；保持 copy 成功是性能额外合同，不混安全或成功工作量；当前 fault 排空未验。
- 本轮的常规规则、PT 着色、共享计账、walker 续表转存具体提案均未排除强前作／普通方法，停止将这些提案作为独立机制；ShareMMU §V-C7 仅作适用性问题。NB-Walker 新作者 artifact 已取得代码但论文全文仍未得、未复现实验。
- 下一最小判别以 codex-next-decision 为准：固定映射、long copy 内可控让路、小缓冲下最小合法前沿/分片成本界；temporary translation lock 只属 ShareMMU 适用性反例，不是生产新加锁，也不是当前 IOTINVAL 语义。当前未 run。
- 主报告没有排除强普通 new rule，因此不能认定独立机制；待证明锐界只作分析候选，不假造实验/算法/证明。
- 本轮新增 DUT build=0/run=0/BIT=0/board=0；没有改 production RTL/Linux/test，没有写 SD。详见 [translation-lifetime-and-innovation-gate-20261008.md](translation-lifetime-and-innovation-gate-20261008.md)。
- 归属：DeepSeek 整理证据和起草文档；Codex 根与 2 研究席作创新推演、关键源/原文核查、最终审定；未使用 Claude。

## §41（2026-10-08）

- 完成一次静态数学运行：程序 exit0，7585+3805=11390 有限分类，部分重复覆盖，greedy/brute mismatch0，非 DUT 试验。
- 接受两个纠正：B>0 一个 cut 支持多个停止点（真实 1 对旧 2）；H 只是安全目的点间距，不是 budget-off 工作/周期界。
- 静态 32KiB 表与软件 tiling 9 subjob/source24/AW17/8cuts 达下界，但未运行、不称同 TL/cost/interface；B24 仅数学假设。
- 根核 NO_ERROR_HANDLING、fullbuffer certificate observer、C0 仅 out；真实义务含首次 offer、mux owner、held、W 前缀，须 beat/prefix 里程碑。
- §39 87–123 two-layer matched physical-accepted W prefix 门覆盖当前 child 准入，删去“未来 AW 未准备”新 bug 反例；实际剩余改为待验证较远 W-owner 债务需更多 source child/raw parent。top610–622 限 out 原型。
- `codex-next-decision.md` 已存在；两名研究席的两轮审核均已完成，全文已保存，根已直接核关键原文、实际源码及两份最终分析；这不等于硬件验收。
- 新 2026 主源、RTAS2025、AMD PG313 仅定向补证，未并入 123，不声称全域 novelty 或论文 gap。

归属：DeepSeek整理证据、提出脚本与文档草案；Codex设计、核源、实际执行并审定；未调用Claude。

[本阶段报告](fragment-drain-cost-boundary-20261008.md) · [下一判别](../out/fragment-drain-boundary-20261008/codex-next-decision.md)

## §42（2026-10-08）：已承诺写请求的有限排空

| 日期 | 阶段 | 动作 | 结果 |
|---|---|---|---|
| 2026-10-08 | §42 | root 1 build + 4 run | exit 0，4/4 PASS，root 接受 |
| 2026-10-08 | §42 | 普通强对照 V 先/A 先 | 4115/5436 request→quiet，pause-end V W 32768/19456 |
| 2026-10-08 | §42 | 严格停 V 先/A 先 | 窗口内无 quiet，pause-end V W 11264/0 |
| 2026-10-08 | §42 | 字节下界 | V 先 ≥15360 R/≥15368 W；A 先 7168/7168 |
| 2026-10-08 | §42 | 成本与下一步 | 功能 ≥65 bit；next 同 codex-next-decision；单有序队列守恒则停本分支 |

[报告](committed-write-drain-boundary-20261008.md)／[账本](../out/committed-write-drain-20261008/stage-evidence.json)／[下一创新判别](../out/committed-write-drain-20261008/codex-next-decision.md)。DeepSeek提供驱动、证据和文档草案；Codex设计RTL、核关键源、实际执行并审定；未调用Claude。

## §43（2026-10-08）：双bank写序倒置不可达，停止机制分支

- 2026-10-08：§42后进入双bank写序分支；root设计观测并执行新build/run；native纸面审计未执行；DeepSeek整理证据/草稿；Claude未用。
- 前版E7/§2.6勘误：AWVALID由mgmt_valid_aw与egress_aw_valid join；mgmt token只由ingress_w_last压入、AW accept消费；external AW前W burst已供给；childburst>WBufferDepth属上游容量/进度问题。
- E6/§2.5勘误：isolate_i在Normal同拍默认仍连通，除非容量命中；状态边沿转换；Hold保留已offer VALID；Drain抑制未来AR/AW。
- 扣费措辞更正：budget on handshake（counter边沿寄存），非after handshake；budget_left只驱动isolation；2023 optional throttle不证明冻结revision等同评估revision。
- 决定：停止双写序冲突枝；root又构造普通owner-prefix记账，纯字节负担准入也归为普通对照。下一仅核真实翻译相关约束/锐界，不先编译阈值扫描。

本轮1新build＋1新run；两32KiBcopy PASS；34/34AW全序匹配，head lag64/owner16/max1；A request→quiet3006，AW4→4、child3→7、parent5→5。范围仅out观测；[正式报告](dual-write-order-boundary-20261008.md)／[根级判断](../out/dual-write-prefix-20261008/codex-result-acceptance.md)／[下一判别](../out/dual-write-prefix-20261008/codex-next-decision.md)。

## §44（2026-10-08）：三席前作/强对照核查，转为续走成本分析判别

root＋两个复用Codex研究席完成独立方案和交叉质疑；DeepSeek补接口证据及报告。正常TC已accept请求恒ready，阻塞W不钉TL返回；scope-aware fast取消有具体范围比较器/2022前作；每PTE continuation有2009 token/tracking前作，phase union的父/leaf复用和索引恢复又被Rocket2022具体source覆盖；FQ queue/credit/去重同效。因此本轮四种规则停止作为独立机制，不先编译新DUT。

手工构造ordinary retained frame最大82bit、partial G restart最大36bit，均另计C/token/active/在途/terminal，非最小位数/面积。一次partial pause最多重复3次G读，但反复读root再restart可以都无S进展；持续前进需G+S≤4笔语义quantum或有限暂停/连续服务。把这一持续进度约束加入下一frontier合同，不能仅比一次让路快。

下一只问相同到达/持续进度/完整资源下，nested多设备的状态—重复工作—让路完整边界是否有新的锐界/达到界规则；当前未证明。如果只是已有checkpoint/restart/PWC点选择，就不能称独立创新。本轮DUT/数学模型build/run/生产RTL/Linux/BIT/JTAG/board/SD均0，最新实际copy仍§43；[报告](translation-continuation-frontier-20261008.md)／[账本](../out/innovation-frontier-reset-20261008/stage-evidence.json)／[下一判别](../out/innovation-frontier-reset-20261008/codex-next-decision.md)。DeepSeek两成功调用118259 provider tokens/high/max393216/未截断，全文保留；Codex亲自设计/核源/执行保存/审定，Claude未用。

## §45（2026-10-08）：普通早让路达到固定链工作下界

root＋两席亲自证明/质疑并核primary，DeepSeek起草与修订数学脚本/报告；root执行一次数学图，6450配置、mismatch0、exit0、0.7781777381896973s。所有可完成共30读，限定两请求各15步、no PWC/shared/coalesced reads。q为连续PTE服务上界，允许提前yield；B仅单停车帧、C/active/身份/terminal另计。max-gap iff是列明codec下存在完成策略，非任意公平策略或全资源最优。

强普通q3/B77按3/1/3/1/3/1/3零重读；q4/B36按4/4/4/3同效；q3/B36无解。q2专用80bit普通构造只纸面，未进五族图；五族不是完整全编码frontier。Barr2010 partial/path缓存与Bhargava2008 NTLB支持具体保留语义，未给这些bit/暂停数；Hong–Kung一般前史不直接等于完整nested frontier。因为普通达到本合同最低工作，固定链checkpoint/codec调度不能称独立创新，不编译该枝。

下一只查真实共享结果复用与进度保护是否竞争同资源，必须同总状态/语义/退休信息允许dynamic checkpoint/cache/pin/普通分槽；若能消差异就停。主线保持共享IOMMU性能干扰，不转安全。无DUT/build/run/production RTL/Linux/tests/BIT/JTAG/board/SD动作，最新actualcopy仍§43；[报告](translation-progress-frontier-20261008.md)／[根验收](../out/translation-progress-frontier-20261008/codex-result-acceptance.md)／[下一判别](../out/translation-progress-frontier-20261008/codex-next-decision.md)。三DeepSeek成功调用共106589 provider tokens/high/max393216/未截断；Claude未调用，原始材料保留。

## §46（2026-10-08）：合法共享命中下两种历史合同的区别

root＋两席完成关键设计/质疑，DeepSeek补证据与文稿。现有IOTLB与PTW progress独立，无所假设的统一slot压力；最新A/V不同PSCID/root、G Bare，旧结果非共享/nested实测。纸面同PSCID/root构造可共享两笔上层PTE，leaf与每请求状态分别保留。NB-Walker作者固定源码已有shared fetch/private Info/有限MSHR/PTW预约；因为强普通覆盖，组合不能称独立创新。

新增明确边界：resident-way的local PLRU隔离不等于requester历史隔离。A合法alias命中V/P0可改变下一V cold fill的victim；旧S3明确允许A-mask命中V-way并touch protected节点，不是旧合同bug。main owner fillmask不能区别main A alias和main A忙时fast V hit；需要实际accepted lookup来源。该普通接口/filter又有2008 US7415575B1的shared-hit/client-specific replacement直接前作，因此只作强对照，不立独立机制，不归因旧PN变慢。

下一保持V/A整体性能目标，不自动引入硬requester不干扰要求；同合法共享空间/总状态/有效信息比较普通true-requester filter、resident-way、共享容量/准入策略，先找普通不可达点。必要定向例复用TL bench放out，尚未实现/运行；不重跑固定链模型，不启动该已有机制的BIT。最新实际完整copy仍§43；本轮math/DUT/build/production RTL/Linux/tests/BIT/JTAG/board/SD均0。[报告](translation-sharing-progress-20261008.md)／[历史证明](../out/translation-sharing-progress-20261008/codex-requester-history-boundary.md)／[账本](../out/translation-sharing-progress-20261008/stage-evidence.json)／[下一判别](../out/translation-sharing-progress-20261008/codex-next-decision.md)。两DeepSeek调用263080 provider tokens/high/max393216/未截断，原始与补正保留；Codex亲自设计、核关键来源并审定，无Claude。

## §47（2026-10-08）：任务剩余用途反馈，先过强普通与场景门

Codex根亲自设计/核源/核原文，两复用研究席完成质疑，DeepSeek补证据与文稿。停止将shared history filter、晚合并priority/扣费、descriptor输送/prefetch/pin当独立原则；已有具体前作。保留有限registered-copy remaining-translation-use反馈＋具体shared-cache管理候选。native TC每direction FIFO允许条件性保守page frontier；最新§43 helper prepared≠valid、快慢返回并存，未证producer连续前缀，需要watermark/计费holes。bank非DID/dir，大页看完整范围。

局部232/464/128bit是载荷手算，非完整成本/面积；两槽见证ordinary same-info同miss，非实测创新。下一场景改用两DMA共享不可变source、独立destination、错开进度；独立单调copy可能已被last-page/shared-entry pointer完全覆盖。先证额外非compulsory PTW，再允许同信息同总资源的range/bitmap/pointer/ref保护、任务感知准入与动态容量对照。如果可靠新反馈组织或具体规则有实际性能/成本增量，就可以论证独立微架构贡献；如果ordinary同成本同收益，就不能称新算法。当前未实现或运行。

本轮新math/DUT/build/production RTL/Linux/tests/BIT/JTAG/board/SD均0，最新actualcopy仍§43；两DeepSeek成功返回88876 provider tokens/high/max393216/未截断，无Claude。6原作SHA一致和11关键源hash只作证据保存。[报告](translation-sharing-qos-20261008.md)／[根设计](../out/translation-sharing-qos-20261008/codex-candidate-design.md)／[账本](../out/translation-sharing-qos-20261008/stage-evidence.json)／[下一判别](../out/translation-sharing-qos-20261008/codex-next-decision.md)。

## §48（2026-10-08研究，2026-10-09交接）：逻辑缓存工作点与普通覆盖

root亲自设计/核源码和三primary，DeepSeek起草模型、schema/证据/稿件，root唯一执行并接受。模型9trace/E组×4普通=36records，N8共享source/context独立aligned dest；每trace64请求/24keys、逻辑gap8/4/1。E8/gap4 LRU30→ordinary remaining-use24，E4配对LRU24，E16全24；DP两小case6一致。只是cache/miss模型，非DUT或copy速度，PTE=3×miss限无PWC。首次运行前修trace重复key，未重跑。

真raw live比helper full-parent资格宽；src3072短首父可被skip、后父先成功，max成功地址不能当前沿；原aligned/full/prefixeligible/no-fault条件可前缀。普通Depth4扫描已有valid/seq/window不用另存holes，短父及时证书及IOMMU跨模块接口仍需计费。因为ordinary interval/refcount同need/决策，remaining-use原则停止独立算法命名；下一只核materialization前沿控超前、data overlap与committed W必要继续，对照含同信息ordinary tile/finite-window/translate-first/有限lookahead，具体增量成立才做最小out copy。

1math run exit0（0.01556497160345316s）；DUT/build/production RTL/Linux/tests/BIT/JTAG/board/SD全0，latestactualcopy§43。四DeepSeek成功high/max393216共193410 provider tokens，未截断；Codex根与两席亲自设计/关键核源，root应用/执行/接受，无Claude。[报告](translation-sharing-window-20261008.md)／[验收](../out/translation-sharing-window-20261008/codex-result-acceptance.md)／[数据](../out/translation-sharing-window-20261008/cache-model-result.json)／[账本](../out/translation-sharing-window-20261008/stage-evidence.json)／[下一判别](../out/translation-sharing-window-20261008/codex-next-decision.md)。

## §49 共享长读取与独立暂停恢复（2026-10-09，root接受事件模型）

从复现的finite-buffer共享问题推进新具体控制：W first-VALID前限制q债务组、existing W coverage才冻结R mask、旧义务drain后A退出、V继续原longR、A以contiguous cursor恢复solo child；earlyresume遇已展开VlongW先drain/EXIT，不能重新接短common组。已有sharedread/auto grouping/fork/slow退组+repair前作明确保留，具体AXI控制与完整成本是待证贡献。

run03 310场景/7540checks及run04 4/104均exit0，合计314/7644受限事件模型；run01 598failed、run02 143failed、两版code与raw全部保留，root另执行一个earlyresume trace诊断。N16/B3/q2/P3/resume200候选/强tile同R28/AW6/214steps、AR2/4；no-pause同48steps、AR1/8；不得当平台速度。permanent pause open/tile的V完成，idle stalled只是未恢复A；小FIFO naive循环非默认4KiB/flow故障。payload48B+skid8B=56B，metadata另计；<=2q是受限工作量推导非cycles/逐例形式证明。permission/AxCACHE/delayedB未实际执行。

根亲自核实际生成reg与Arm：旧CONF1 cache0不直接合读；未来候选与强tile同CONF129/cache0010，核实际许可/属性，nonmodifiable/device回退。现成splitter只singlebeat，q>1要明确写侧children/Bjoin、读侧保long；限page/256beatspan。一个out-only iDMA/IOMMU原型尚未编写/构建，下一测完整copy真实成本；本阶段productionRTL/Linux/tests/BIT/JTAG/board/SD及DUTbuild/run均0，latestactualcopy仍§43。DeepSeek证据/模型/修订/文档提案，Codex根与两席设计/关键核源，root应用关键resume修补/实际执行/接受；5flash/high/max393216成功未截断，共527959 provider tokens，无Claude。

[报告](shared-read-independent-scheduling-20261009.md)／[根接受](../out/innovation-rescue-20261009/codex-result-acceptance.md)／[data](../out/innovation-rescue-20261009/accepted-model-summary.json)／[执行ledger](../out/innovation-rescue-20261009/execution-ledger.json)／[RTL合同](../out/innovation-rescue-20261009/codex-rtl-admission-contract.md)／[前作边界](../out/innovation-rescue-20261009/codex-cross-field-boundary.md)。
