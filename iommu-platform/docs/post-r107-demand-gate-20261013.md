# stage112：逐层归并需求核证；当前 FPGA 投影归档

2026年10月13日JST。整体仍处于阶段一立题。本轮实际登记并由工具完成 goal，无额外 token 预算，Codex-only；完成一次有界来源与接口裁决，没有 G0/G1、实现、构建、板测或旧回放。

**裁决：归档 C112“以逐层分片归并支持新的 FPGA 共享翻译缺口”这项当前投影。** 两分片的原生归并义务有源码支持；它的物理请求器、源值、完成接口及共享翻译路径没有在现有 FPGA 上成立。新增核证还表明归并使用持久缓冲，不能推出需要逐层重新映射。没有证据证明合法普通控制之外存在可达、可避免的翻译损失，因此不进入 G0。这里结束的是这一投影的进入尝试，不是否定原生 collective 的用途或整个共享 IOMMU 主问题。

R107 原有限恢复卡继续归档。其原始实板结果与全成本复用为资产，不重跑、不把当前消费合同补进历史结果。[stage111 原裁决](../out/r107-ordinary-pilot-20261012/root-result-acceptance.json)、[原始接受](../out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json)。

## 1. 本轮问题与界限

问题是：保留的 GPU/AI 原文中，一个新增任务或生命周期事实能否使普通大页、整池预映射、驻留、批量及授权单 DMA 合并仍留下具体翻译损失？进入条件是新增原始需求事实；结束条件为至多一张来源卡、可执行对照的接口条件、完整成本和稳定裁决。没有成立的 FPGA 接口时，交付具体缺口，而不先造实现。[登记回执](../out/post-r107-demand-gate-20261013/goal-registration.json)、[原执行合同](../out/post-r107-demand-gate-20261013/goal-contract-input.json)。

只读取既有 vLLM v0.1.0 固定提交 `67d96c29fba9b72cb4c4edbc26211c208a00ebdd` 的直接调用依赖；从已保存的官方原 archive 提取四个文件，没有新来源系统、网络搜寻或新增场景。原 archive SHA256 `1b41ae17ef7bf56a37d4793bbaab9b994d93d341033c2313084dcb7846551fe6`。[提取与官方路径](../out/post-r107-demand-gate-20261013/source/new-primary-source-manifest.json)。

## 2. C112 来源合同：恢复以后确有逐层消费和归并

本轮新增的是对 RowParallelLinear 和 GraphAllReduce 实现的直接核证；不是新增应用运行，也不是把已知 layer event 改称新事实。

- OPT `out_proj` 与 `fc2` 都是 RowParallelLinear。TP>1 时，局部矩阵乘积写入 launcher 的缓冲切片，然后调用归并 launcher；12层对应24次这两种归并出现，每个rank各24次调用。仅统计这两条调用链，不声称覆盖 embedding、sampler、初始化及模型全部通信。
- GraphAllReduce 在初始化时分配 `[M, hidden_size]` CUDA 缓冲；同一缓冲的前缀供后续调用复用。默认还为8到M、步长8的尺寸建立 CUDA graphs，每种包含 warmup、capture 及同步。`launch` 返回 Tensor 对象不是本轮观测到的 GPU 完成事件；CPU API 返回、GPU执行完成与网络数据完成不能混为一谈。
- cache stream 依层提交 K/V 拷贝并记录 event。OPT 的第 i 层取得第 i 个 event，attention 在写新 KV、读取旧 KV 前等待它；下一层收到本层 hidden states。安全依赖原文已知；实际 GPU 重叠、到达与计算时长仍未测。
- worker 使用 NCCL backend。CUDA/NCCL 的实际算法、NVLink/PCIe/RDMA选择、物理 requester、主机 IOMMU 经过范围及映射行为没有从调用名得到证明。

直接入口：[OPT 调用](../out/ai-platform-constraints-20261012/ai-block-source/vllm-source/vllm/model_executor/models/opt.py:75)、[MLP调用](../out/ai-platform-constraints-20261012/ai-block-source/vllm-source/vllm/model_executor/models/opt.py:117)、[RowParallel 实现](../out/post-r107-demand-gate-20261013/source/tp-layers.py:415)、[持久缓冲与 graph](../out/post-r107-demand-gate-20261013/source/parallel_state.py:520)、[原生层事件](../out/ai-platform-constraints-20261012/ai-block-source/vllm-source/vllm/worker/cache_engine.py:102)、[真正消费](../out/ai-platform-constraints-20261012/ai-block-source/vllm-source/vllm/model_executor/layers/attention.py:130)、[backend](../out/ai-platform-constraints-20261012/ai-block-source/vllm-source/vllm/worker/worker.py:289)。完整精确引文与哈希保存在 [root亲核记录](../out/post-r107-demand-gate-20261013/critical/root-source-review.json)。

| 项目 | 原生来源合同 | 现有 FPGA 实际证据与界限 |
|---|---|---|
| 任务 | 各rank恢复K/V；各层attention与MLP产生局部输出，归并后继续 | 只执行完整priorout/restore；没有attention、局部输出或归并执行 |
| 所有权 | worker关联一个GPU，拥有KV与归并buffer；同一TP group共同消费结果 | 有一个可信owner/DID10授权访问四池；来源没有要求把这些逻辑角色强制分成隔离DID |
| 生命周期 | 整池与launcher buffer预分配，forward复用切片 | stage111按owner保持映射到最后checked release；不能说逻辑阶段结束就unpin或撤权 |
| 依赖 | 该层cache完成→KV写/读→局部输出→collective结果→后续层 | `joint_layers++`是CPU全字节校验后的记账；不是归并完成，更不是token完成 |
| 自然结束 | 本次层输出已经被后续算子消费；重用buffer必须在消费后合法发生 | 原有限硬件任务已DONE、排空、校验及释放；新消费的真实完成点未提供 |
| 数据与时间 | 模型结构可推导形状；实际输出值、计算、到达、期限未知 | 不用模式种子冒充计算结果，不用旧板周期填入新的生产或期限 |

旧板源码 `[idma_finite_phase(...,1)] → idma_finite_verify(...,true)` 明确是**全部恢复以后**CPU逐字节核验，再填24个worker-layer及12个joint字段；`idma_finite_byte`读取CPU地址。CPU的AXI口直接接系统互连，不能当作另一条iDMA翻译请求。[实际消费代理](../out/r107-ordinary-pilot-20261012/repair-1/source/linux/drivers/misc/idma_finite.h:475)、[调用次序](../out/r107-ordinary-pilot-20261012/repair-1/source/linux/drivers/misc/idma_finite.h:708)、[CPU连接](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/fpga/src/ariane_xilinx.sv:709)。

## 3. 有限字节合同：形状成立，执行数据与wire合同未成立

只沿用原有限来源卡的两条生成序列、OPT125m、TP2与FP16，不加序列、槽或模型。若从该checkpoint继续一次decode，源码将2个输入token补到8行。两个RowParallel位置都输出hidden=768的行。[padding原文](../out/ai-platform-constraints-20261012/ai-block-source/vllm-source/vllm/worker/worker.py:212)。这是一份源码推导的后续消费形状，不是新native trace或已执行研究回放。

| 数量 | 精确值或未知 |
|---|---:|
| 每rank、每次归并的活动buffer前缀 | 8×768×2＝12,288B |
| 每前缀有效2行／padding6行 | 3,072B／9,216B |
| 这两条位置的group归并出现数 | 12层×2＝24 |
| rank API调用数 | 2×24＝48 |
| 全24次rank输入逻辑操作数字节 | 589,824B |
| 全24次rank结果逻辑操作数字节 | 589,824B；原地复用，不能再算一份同量storage |
| 有效rank局部输入总量 | 147,456B |
| 有效归并结果去重后总量 | 73,728B；两个rank消费同一逻辑归并结果 |
| 一次两个活动前缀的存储 | 24,576B，不因24次调用乘成常驻存储 |
| 每rank完整持久buffer容量 | M×768×2B；旧有限KV合同没有给M，未知 |
| 实际网络字节、DMA字节、NCCL算法、输出值、时序 | 未验实 |

调用计数不包含本链之外的通信。逻辑输入／输出、网络载荷、物理DDR流量、KV恢复字节和存储容量分别计算；后四者不能从前两者相加推得。[机器可读字节合同](../out/post-r107-demand-gate-20261013/source/finite-consumer-byte-contract.json)。

## 4. 普通强对照与合法控制点

保留原完整ordinary恢复：四池、实际大页及合法4KiB回退、整池预映射、驻留、批量、连续范围合并、同owner授权单DMA。真实归并是算术操作；当前iDMA只搬运，不能把两个partial拷到同一地址说成已经all-reduce。

如果未来明确采用共同DDR的有限研究投影，普通对照必须允许：已有CPU或真实消费者产生partial；两个partial真实ready后，以合法单DMA或多DMA收集到owner授权位置；CPU/已有reduce单元处理完整数值并交付结果；按真实完成反馈提交后续任务。同样计准备、cache交接、软件供给、算术、广播、排空、撤销与释放。不能人为禁止CPU归并、稳定scratch或授权合并，也不能把这份**尚未实现的新ordinary glue**算成普通方法失效。原生GPU位级数值、舍入及backend完成接口在此前提中仍须单独定义。

给出一个仅限活动前缀的合法布局见证：每rank逻辑GPU池为2,359,296B，实际已有4,194,304B backing。若新合同授权使用池后slack，可在每个GPU池偏移 `0x240000..0x242fff` 保留一个12,288B活动前缀；它不覆盖旧逻辑池，完整落在旧映射的第二个2MiB叶内。两buffer共24,576B，**无需新增叶**。这不是新的物理分配或当前仍有效的IOVA，stage111已正常释放。旧R107 guard要求保持原样；本轮没有写这些slack，未来合同修改guard范围必须明确。完整M、weights、NCCL内部资源未被这项几何见证覆盖。

若使用相同授权DID、相同驻留映射且没有新增撤销或其他替换，单是这两个活动前缀不引入新翻译页身份。先前板上8叶完整恢复1728hit、0miss、0walk可证明原ordinary能力，但不能证明尚未执行的collective净性能。因为源码没有要求逐层remap／独立安全domain／新的映射working set，这个事实不支持用强制4KiB、多DID或扩大池来制造新缺口。

合法控制点是任务的真实ready、未发布NEXT之前、完整DONE和quiet之后，以及真实消费者完成后可重用的buffer；已握手事务不可撤回。`event.record`、CUDA API返回和CPU记账均不等价于这些FPGA硬件握手。

## 5. 成本与边界

| 成本 | 证据或当前状态 |
|---|---|
| 原ordinary backing／映射／准备 | 已实测4×4MiB、8真实2MiB叶、16MiB backing；旧成本原样保留 |
| 归并buffer | 原生完整每rank M×1536B，M未知；有限活动前缀两rank共24,576B |
| 原生初始化 | NCCL group初始化与warmup；每8行尺寸graph warmup/capture/synchronize；时长、graph和通信资源未知，不能免费略去 |
| partial生产与消费 | attention、两处linear、归并、后续算子与最终token链；数值及时间未知，不能用任意sleep或synthetic arrival填充 |
| native collective | backend/source确定；算法、wire、physical requester、registration、page布局与IOMMU经过范围未知 |
| FPGA必要ordinary glue | partial导入／生产、稳定scratch权限、真实ready/done、数值与缓存交接、完整结果消费；接口和运行成本未成立，未实现 |
| 新控制／实现 | 没有接受任何方法规则；metadata、存储、端口、控制延迟、切换与再热若将来实现必须全计 |
| 完整清理 | buffer不得在消费或在途读写前重用／释放；native completion与销毁未验，原有限FPGA checked释放已通过 |
| 本轮成本 | 源码／原始数据读核、四文件提取与合同整理；没有新设备实验，没有实测GPU性能 |

原stage111的kernel whole名义21.960371360s、restore名义45.192160ms／条件Dell40.556956–49.914060ms继续只是原有限恢复成本，不作为C112 latency；原始周期、名义时间、独立Dell条件界分别保留。seed、全字节验证与输出属于实验检查，不能冒充原生计算。[旧完整成本](../out/r107-ordinary-pilot-20261012/repair-1/costs-and-boundaries.json)。当前SI精度、精确peak RAM与新任务运行成本未知。

E3保留一个原始case6序列17/18：受控delay0/128，A剩余服务及pending V-hit剩余等待各增加129周期，前置时间131/135相同。这证明旧串行路径的局部传播能力，**不能给C112提供自然cold requester、到达或deadline**。冻结TC在IDLE才选择新请求、AWAIT_RESP等待当前翻译，源码解释与原始数据相符；本轮没有复测。[原始选段与字段](../out/post-r107-demand-gate-20261013/critical/existing-e3-boundary.json)、[旧接受复核](../out/e3-controlled-review-20260918/review-summary.json)、[冻结TC](../out/composition-isolated-20261007/fpga-src/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:625)。

## 6. 正反条件、归档理由与路线

可检验的正条件是：两个有独立用途的真实搬运任务经过当前共享翻译路径；完整owner、source→consumer和自然映射／就绪事件已确定；在合法大页、提前准备、驻留、批量、单DMA与完整准备/执行/释放成本下，仍能出现消费者关键路径上的翻译等待，并且存在当前接口可实施的避免动作。必须给原始记录或来源支持，不能只引用E3存在HOL。

相邻反条件是：同owner稳定映射、相同驻留leaf、两个活动prefix不增加翻译身份，或所谓消费者只是CPU检查／GPU内部算术且没有新iDMA流量。此时不能凭“层有依赖”“跨rank有collective”登记翻译G0。本轮命中这一进入边界；尚未执行的完整C112普通程序没有被测为失败或成功。

安全语义：来源已实现层event与collective调用，不能把它们改名为新安全能力。性能增量：没有接受的ordinary外残余或净收益。机制新颖性／论文贡献：没有形成新规则，不作新颖性正面或全领域否定结论，因而不启动无对象的前作搜寻或G1设计。

本次归档的确定理由是**当前FPGA接口缺失＋源事实未排除普通稳定映射**；未知的是native实际物理通信、完整buffer预算、执行时序及剩余翻译损失的数值。不能把“未知”改写为0，也不把接口缺失当科学阴性。

分支结束后回整体阶段一的来源选择：下一轮需一个明确来源下的自然独立搬运/映射需求，先补任务、物理请求路径、合法普通对照失效的新事实。GPU/AI仍优先、现FPGA仍主平台；不把扩大这两条序列、增加DID、改小页或人为冷却作为路线。当前没有已获准自动实施的后继场景，下一合同为inactive的进入要求；没有新输入时不重复读同一调用链或搭collective模拟器。[下一关口合同](../out/post-r107-demand-gate-20261013/next-goal-contract.json)。

## 7. 实际执行与交付

本轮主会话直接核11组精确源入口、原R107 receipt及一个E3原始对；四个新增提取文件来自原官方archive，19个关键旧输入保护。创建一个exclusive Sol/ultra席失败，工具原因是`agent thread limit reached`，因此没有子席执行/结果；root完成该核查。不调用DeepSeek、Claude或Astra。

没有修改生产源码、模型配置、缓存、数据库、历史运行或原实验数据；没有板命令、SD/bit操作、旧stage106/108/E3回放、旧校验器重跑、新构建、G0/G1。直接依赖文件名和引文范围的查找/编译错误及修正写入账本，未伪装成科学结果。[账本](../out/post-r107-demand-gate-20261013/execution-ledger.json)、[稳定裁决](../out/post-r107-demand-gate-20261013/root-result-acceptance.json)、[核验与保护](../out/post-r107-demand-gate-20261013/validation/delivery-check.json)。[实际goal结束回执](../out/post-r107-demand-gate-20261013/goal-completion.json)。本轮goal结束不代表方法论文完成。

## 8. 用户补充的旧席处理顺序：已核任务与产物，关闭能力缺失

主会话只读核本线程原始运行记录中的三份最后FINAL_ANSWER，随后没有这些席的实际新任务dispatch；三席历史任务均已完成，本轮不再需要。当前`list_agents`仍显示pending_init，不能改写为运行状态completed或closed，也不能据此认定占用原因。

| 旧席 | 历史任务最后完成UTC | 当前列表状态 | 本轮需要 | 产物／线程处理 |
|---|---|---|---|---|
| `/root/r109_artifact_identity` | 2026-10-12 09:07:32.051 | pending_init | 否 | 4个关键入口保持原路径并核hash；未关闭 |
| `/root/r111_build_staging` | 2026-10-12 12:13:23.254 | pending_init | 否 | 4个关键入口含真实payload保持原路径并核hash；未关闭 |
| `/root/r111_ordinary_interface_plan` | 2026-10-12 12:10:57.108 | pending_init | 否 | 4个关键入口含最终审查／patch保持原路径并核hash；未关闭 |

[官方管理说明](https://learn.chatgpt.com/docs/agent-configuration/subagents)区分管理、停止与关闭线程；官方支持描述不等于当前暴露接口必有相应能力。当前协作工具只有spawn/send/followup/interrupt/list/wait，没有close。App额外工具只说明archive/unarchive，未承诺关闭子席或释放其名额，因此没有用归档代替关闭，也没有中断旧席。App线程列表一次只读查询未返回完成结果，结束等待后没有用它作状态证据。

没有释放名额的实证，所以没有按“已释放”重试创建；不降模型/强度、不改配置、数据库或历史。当前关键任务已经由root完成，没有必要仅为补一个独立席换主会话。若后续确有独占关键问题需要席位，使用保存的研究交接与本项限制进入新主会话，再明确指定Sol/ultra。当前不自动换线程、复制历史或开始新研究goal。[完整生命周期核验](../out/post-r107-demand-gate-20261013/validation/agent-lifecycle-review.json)、[原始FINAL入口](../out/post-r107-demand-gate-20261013/validation/old-seat-runtime-lookup.json)。
