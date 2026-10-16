# 已核来源到最小因果合同

2026-10-16；source席独占交付。请求 `gpt-6.1-sol/high`，实际独立model/effort运行元数据unknown。Codex-only；按ARS deep-research的来源核查原则作有界事实整理，不启动完整pipeline。本席没有新模型、实验、trace重解析、网络检索、构建或硬件动作。父任务合同优先于旧文档的后继门槛。

**结论：可以开展一个来源绑定的条件小模型。** 已有来源同时支持三项必要结构：真正walk启动早于本地及聚合PTE AR可见；生成MIG路径没有accepted-read type-priority；正常buffer/owner合同允许独立A提前源读与V目的翻译/写owner形成完成依赖。三者在一个native短窗口内共现及完整净收益仍是未知，模型须把共现时间公开冻结，不能把未知当成禁止思考的理由。

## 1. 已核X及最少来源范围

| 事实 | 最少源码/既有证明 | 本轮范围 |
|---|---|---|
| TL发ptw_init不等于PTW实际新启动 | 历史绑定TL `:373–375`；PTW `:315–365`、edge `:151–157` | 直接核PTW起始与合法S1分支，复用TL init原摘录 |
| IDLE新启动当拍PTW active仍0，本地ARVALID仍0；MEM_ACCESS才1 | PTW `:90–100,275–283,384–389` | 旧source-evidence含完整原文，另定向核active/start |
| 本地PTE需过7输入DS仲裁、非旁路AR spill | `rv_iommu_ds_if.sv:77–109` | 复用旧逐字source-evidence；不能将聚合DS全部认成目标PTE |
| selector第一次输出VALID已作保持承诺 | vendor选中 `axi_mux.sv:331–364`；`rr_arb_tree.sv:145–184` | 复用controls/evidence及ordinary-program，不能用晚到intent撤回held |
| 生成MIG单active/head读展开、顺序数据/metadata返回；ARQOS只影响读写切换 | 已核 `ip-semantics/primary-sources.md:25–73`；原manifest中S02 `:138–225`、S03 `:107–118`、S05 `:125–131,159–215`、S06 `:189–326`、S08 `:989–1111`、S10 `:395–442,844–933` | 复用原完整证明和hash；本轮不重读所有MIG源、不重新证明当前加载身份 |

由此得到最小X：在本owner尚无held时，**实际start已发生/正在合法发生、目标P还未成为current-VALID共享头**的窗口里，普通current-VALID DS-first只能看见其他合法L。若其首次offer L，此后不可用晚到P替换；若L随后先被本生成MIG接受，P接受时L还欠k>0个真实R握手，则P首次返回要等L最后R握手。k=0、P先接受或L未进入这个MIG时不作该推论。不能把这点写成所有AXI/C++模型整burst不可抢占。

旧M0 `docs/workflow-g0-m0-results-20261010.md:9–35` 的普通DS优先操作的是**行为DDR已接受队列**，本来能跨ID rotate，held R除外；其P/PN相同完成结果完全保留。C++ `tests/nonblock_actual_dma_test.cpp:679–719` 的原文摘录见旧critical-source-evidence的model_ds_priority项。这个服务能力与MIG入口规则是两个控制接口。

## 2. 先固定数据、权限和owner，才能使用短序列

已有可复用事实：

- `out/c-readiness-check-20261007/codex-review.md:23–30`：首V源AR16/26早于新目的叶PTE/fill，且source capture的FAST=0。它证明一般normal“源已缓存、目的冷”的源先行，**不证明同一作业FAST源先于自己的冷目的**。`acceptance.json:168–258`保留该区别。
- 同review `:39–45`、`first-walk-interference.json:46–96`：另一job A的FAST源读1226/1232先于V根PTE1263；V根返回1840，A目的AW2728/2735，输入W此前由V占有。优先返回干预把同根PTE返回变为1295，同时仍允许这两条A源读。数字来自旧行为模型，不是MIG测量，旧DS固定50/50条件也不能偷换成新模型的native管线时长。
- 输入owner事实是历史绑定 `rv_iommu_banked_ctl.sv:235–278`：bank由原ID位选择，W未完成计数与AW可见性约束新读选择。TC `:625–647` 的AW-first只覆盖它当前可见头；normal aligned4096 fullbuffer证书与匹配DID可绕普通read admission。证书 `idma_fullbuffer_certificate.sv:14–18,26–35`依真实job而非虚构64B例外。
- `ariane_peripherals_xilinx.sv:1262–1274`：前两DMA在该Nonblock profile下buffer512、其他3、inflight8。64bit下前两是每DMA4096B原有容量，不新增pool或整job资源；SOURCE READY不能由buffer尺寸单独推出，仍需合法成功翻译及真实请求。

允许design冻结的**条件状态S**：两个独立正常aligned4096B copy V/A使用前两原buffer；V源内容已合法返回并保存在其buffer，V已经持输入W owner，V目的映射合法但本次翻译cold；A源读L已经成功翻译、完整AR字段/身份稳定并可在其正常fullbuffer证书下到selector，A目的AW因V owner而等。L不生成V的数据，不是V排空所需的source，所有V所需读及已有AW/W/R/B义务继续排空。

其依赖图为：

`V目标PTE返回 -> V目的翻译结束 -> V合法AW/W排空 -> 释放输入W owner -> A合法AW/W排空 -> 双方B/错误/consumer与quiet完成`

A的独立L只与P争共享读服务；先读L未必立即推进A写。在S下暂缓**尚未由selector首次offer**的L不阻断V数据生产，因此不会凭这个门引入“等P而P又等L”的循环。已有source准入限制及完整buffer资格保持。若V源仍缺数据、L正供给当前owner排空、或A只持3beat buffer，则本S不适用；不得靠修改原DMA能力修补。

这是一个合理、来源约束的条件形状，不是从真实reset逐拍证明完整S。旧C路径本身也不证明本次start与L首次可选的精确共现相位。

## 3. 实际early intent能够从哪里来

最早可考虑的信号是PTW当前`state_q==IDLE && init_ptw_i && !edge_trigger_q`且确认合法S1路径的**实际启动条件**；不是把`ptw_init`电平、`active_o`或未经握手的输入VALID叫作已经接受的walk。S1 `2'b10`计算已锁请求的root PTE指针。双阶段路径另可能立即fault，本小模型只选正常S1、无隐式翻译。

TL `:177–214` 的非旁路req spill保存req_data；`:416`从该锁定请求解码is_store；`:769–800`把稳定IOVA、上下文、方向和stage送PTW。故初始intent可以绑定**当前已锁TL请求**的bank、读写类型及本次walker occurrence，而不是猜未来job。这里“源”若指producer，含义是此锁定TL/PTW来源；若指DMA payload source，其成功翻译ready是另一事实，不能相互代替。

- 初始P本地AR最早在start状态更新后出现，DS AR非旁路spill还至少需要一次寄存更新。到共享selector的精确最短时延、route/spill/固定延迟是否适用以及intent传播/注册延迟没有在本轮冻结为实测数字。要改变选择，必须有`intent到达 <= L首次offer前可决策时刻 < P共享头可见`。
- 这是拟议新增付费信息通路。现共享selector只见source0/1/2/3合法AR头、原READY和自己的history，没有现成PTW实际start输入。已知固定工作流或预先冻结的普通等待规则可能推断或保守覆盖start，不能声称所有public-history程序都无力预测。本轮也没有完整native不可区分前缀证明；只能在明示观察合同的小模型中安排同一公开history、不同隐藏start。
- 目标标识须区分PTW AR与其他DS：原DS `:235–272`用local ID0路由普通PTW，其他ID给DDTW/PDTW/CQ/MSIPTW等。两端fullID/occurrence、stage及reset域必须一致；单正常PTW、无旧DS的模型可简化成一个有epoch的pending occurrence。**不能因某个DS头出现就默认目标P已经兑现**，除非模型显式无其他DS。
- intent只推迟新payload选择，不授予翻译结果、数据访问或owner权限；不取消已有AR或R、不改AW/W owner、不挪同ID次序。lease超时应解除新抑制，P即便晚到仍正常current-VALID DS-first；反之首次offer后的L必须完整保持。不能因intent撤销而清已接受义务。
- 下一层PTE的地址/访问是否存在取决于前PTE返回和叶/错判定（PTW `:392–417`），不能从initial start宣称未来每层必发，也不能把等上级返回期间已承诺的L重新算作后来leaf intent的可避免债务。后续阶段intent是另一个接口合同，本席不默认扩展。

## 4. 交给design的小模型冻结表

| 内容 | 地位 | 最小可用冻结 |
|---|---|---|
| 请求/容量/顺序 | 来源事实加有界抽象 | 使用前两原4096B buffer、原单PTW、每方向原能力；只展示一个合法payload burst L和一个8B PTE P，完整4096 parent其余工作不能删除，可共同前后缀计入 |
| 时间/时钟 | 模型假设 | 单抽象时钟；intent传播d、P到selector间隔δ、burst服务b、每新AR开销a、write tail w均公开；不冒充真实UI/platform跨域周期 |
| 服务律 | 来源支持的特定抽象 | 对所核MIG路径采用accepted-read顺序输出、held稳定；共同旧债务D两方同付；不借FIFO深度制造等待 |
| 源ready/owner依赖 | 来源形状＋条件共现 | S中的V源已保留、A独立L已合法ready、V目的cold、V持owner；不作同job FAST断言 |
| 局部选择窗口 | 待验证假设 | 无旧selector held、目标P未共享可见、L仍未首次offer、intent及时到达、普通fairness未强制nonDS逃逸 |
| 完成定义 | 必须保留 | 同有效字节、许可、全部R/AW/W/B/error与quiet，必要consumer/cache交接；仅P返回提前不足以接受完整收益 |
| CPU/存储/线路 | 待实现未知 | 模型可以计事件/字节/已定义状态，实际门数、Fmax、传线与同步、CPU费用不得填0冒充已验 |
| 其他程序 | 固定具名普通对照 | current-VALID DS-first；固定等待lease；预先准备/序列化；原合法deburst；同一新增intent接口的简单门控，最后一项归等价实现而非发表前作 |

最小邻近反例：①L先于intent到达已held/下游接受；②P已current-VALID，DS-first本就先选它；③lease到期P仍未可见，新门恢复L且失去窗口；④P接受时k=0；⑤L为当前owner所需数据而粗门阻断进度；⑥L/P顺序改变但V写尾/其他共同债务覆盖全部关键路径；⑦公开固定等待正好覆盖δ，动作相同但需计无walk时的额外等待；⑧同新增intent接口简单普通程序动作完全相同，这排除复杂调度策略增量，不自动排除接口价值。

## 5. 当前接受范围与原始入口

可以接受：上述最小X、条件S、实际start生产点以及小模型事实/假设分离。不能接受：native窗口已经出现、MIG实际收益、零成本意图、硬实时上界、所有普通方法不可复制、独立新算法或论文主线已定。缺自然整段trace/未实现接口并不使条件推导本身无效。

逐项hash、原manifest、定向读取范围和事实/条件/未知见同目录 `evidence.json`。核心复用manifest：

- `out/shared-service-mechanism-admission-20261015/root/source-evidence.json`
- `out/shared-service-control-boundary-20261015/controls/evidence.json`
- `out/shared-service-control-boundary-20261015/root/critical-source-evidence.json`
- `out/ddr-service-boundary-20261014/ip-semantics/source-snapshot.json`
- `out/c-readiness-check-20261007/acceptance.json`及`codex-review.md:23–57`

旧档案中DeepSeek的历史贡献保持原归属；本席无DeepSeek新调用，仅Codex复用/核查。

## 6. design请求的最小补充：非DDR尾与合法单PTE条件

仅调换串行内存同等工作量的次序不保证makespan改善；要算完成变化，必须保留真实依赖尾。**存在不可零化的翻译→物理AW状态边界**：TL `:1329–1331`在IOTLB hit后请求下一COMPLETE，`:1544–1566`才给正常slow_resp_valid；TC `:693–702`在AWAIT_RESP握手后锁响应并请求COMPLETE，`:716–725`在COMPLETE才发布物理请求。因此仅“翻译响应→本TC物理AW”就要跨一次该时钟的状态寄存更新；PTE最终R到该点还包括fill/lookup等，精确拍数未核为固定数值。design可公开参数化τ>0，检验候选是否用L服务覆盖τ及后续write tail，不能把τ省略后声称总工作收益。抽象read/write是否共用串行server也须明确，不能从MIG单读顺序推出读写完全串行。

若需要一个PTE代表完整walk，存在**另选合法映射**的来源支撑，而不是把原4KiB页三层walk删成一层：PTW `:416–437`允许S1根LVL1叶并取IOVA低30位，`:478–503`在!en2时update，须满足根叶PPN低18位零、V/R/A及write所需D等权限；TL `:1443–1447`同样合成1GiB地址。正常4096B aligned copy可以位于一个合法1GiB映射中，fullbuffer资格由job字节/对齐决定而非页表叶大小。此为条件输入，不是旧cold-dst夹具的既有1GiB测量。更贴近旧4KiB页时须保留三个按数据依赖串行出生的PTE及processing gaps，不能靠初始start假装后续每层已是current-VALID。
