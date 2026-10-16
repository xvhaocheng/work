# B双缓冲应用合同与目标计时关口 stage99

日期：2026-10-12。全部由Codex完成；两个新增席指定gpt-6.1-sol/ultra，独立核验复用同配置Codex席。没有DeepSeek或Claude调用。

**实际进展：已补齐一份来源明确、执行过且独立验收的双缓冲应用合同。B仍缺与DMA同一时钟的计算／存储接口，不能进入性能结论或新规则设计。** 本轮完成的是应用采证和具体缺口登记；方法论文总体目标没有完成。stage98的A阴性及stage97旧PN分支保持原结论。

本轮先稳定[根裁决](../out/workflow-stream-contract-20261012/root-result-acceptance.json)，再同步计划与当前状态。完整源、私有功能程序、实际结果和计时／成本核查均留在独立产物目录，没有修改原测试、生产RTL、Linux或ABI。

## 1. 新来源实际说明什么

采用PULP官方HERO仓库固定提交`c8481a930a15a385d4f58f519bed83d2e75e2a73`的[mm-large.c](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/mm-large/mm-large.c)。原函数44—276行、默认参数284—305行、初始化320—326行及参考335—346行已直接核查。许可证和22份定向源码原样保留，[来源清单](../out/workflow-stream-contract-20261012/source-evidence/source-manifest.json)给出网址、提交和哈希。

这是相关团队官方应用的新来源适配，**不是原2018论文SP复现**。本轮有界搜索没有取得原SP代码。固定版本实际启用copy-based device1；SVM device0位于`if(0)`内。因此这里取得的是计算与缓冲依赖，不是一个现成共享IOMMU性能实验。原论文的强度扫描仍不能拼成同次应用的动态阶段。

| 合同项 | 已核事实 |
|---|---|
| 固定输入与运算 | 256×256 uint32，`C[i,j]=sum_k A[i,k]*B[j,k]`，模2³²；原默认A从0递增，B=2I |
| 条带 | 16行×256元素=16KiB；外部连续条带stride16KiB，局部行pitch1024B，无halo |
| 缓冲 | A/B/C各双缓冲：六个16KiB槽，共96KiB；不是旧泛化模板的四槽，也不是iDMA私有载荷FIFO |
| 计算 | 16×16=256个tile；每tile16×16×256次乘加，全程16,777,216次；不因B稀疏删原内积 |
| 搬运 | A16次、B256次、C16次，共288次×16KiB=4,718,592B；不包括OpenMP卸载映射等运行时成本 |
| 并行来源 | 原函数请求8线程，其中0/1/2管理A/B/C；不是当前平台已有8个计算器或3个独立V iDMA的证据 |
| 别名与目标 | 外部A/B/C独立、restrict；没有实际deadline；后续应评价完整输出完成及固定A后台进度／尾段 |

A条带s保持至它的16个tile全部读完；B条带每tile装入并在旧tile读完后复用；C条带经过16个tile写满才可搬出，旧输出真正完成后才可覆写。计算还要等待相应A/B代次搬入及C槽可写。原屏障给出保守释放关系，没有真实最后读事件时不提前宣布槽位可回收。

## 2. 实际执行与独立验收

私有[功能程序](../out/workflow-stream-contract-20261012/proposed/hero-mm-functional.c)保留原函数全部算术、控制语句和8线程结构，只插入两个观察块。根审查和独立核验均把这两个观察块删除后与原44—276行逐字比较，完全相等。不可用的host L1 allocator替换为实际malloc；API调用执行真实同步CPU memcpy，并记录独立job与槽位代次。原主机库的async名字不表示真实异步，L1 malloc实际上返回NULL，故没有冒称直接运行完整HERO卸载环境。

根实际编译一次、默认程序执行一次，零编译告警，输出全部65,536个uint32。独立验收另执行一次完整数学参考与事件重建，不重跑驱动：

- 全部65,536个值正确，交叉`2*linear_index`，总和4,294,901,760。
- 1,390事件；288组issue/copy_done/wait、256组compute起止、4,718,592B及六槽完整释放通过。
- A/C最终代次7，B最终代次127；A跨16tile复用、B每tile重载、C旧输出完成再覆写的顺序通过。
- 改一输出值、删除一个copy_done并重排ID、提前复用B槽且保持ID／host_ns单调，三个内存损坏例均拒绝。原产物未改。

[独立验收](../out/workflow-stream-contract-20261012/validator/acceptance.json)／[实际执行](../out/workflow-stream-contract-20261012/validator/execution.json)／[可重跑接口](../out/workflow-stream-contract-20261012/proposed/functional-build-argv.json)／[编译及产物来源](../out/workflow-stream-contract-20261012/functional-execution/run-provenance.json)。

**边界：** 观察块增加512个屏障；搬运为同步CPU拷贝，host_ns仅标观察顺序。本结果不是异步DMA、AXI B完成、IOMMU干扰、目标周期、FPGA吞吐或G0性能验收。默认B=2I本身不能数值区分转置，故同时固定实际原索引。乘加次数由保留循环确定，不称硬件退休计数；此次只验证原默认数据。

## 3. 当前平台的确切缺口与成本

[计时／接口核查](../out/workflow-stream-contract-20261012/compute-interface-review.json)给出实际源码行号、工具检查和两种途径。根直接核当前SYSTEM声明、地址路由、dummy端点、CVA6计数与乘法流水边界：SYSTEM是合成CPU＋真实DMA/IOMMU＋行为DDR，0..4GiB走DDR，第二端点是`axi_err_slv`，没有应用计算器或96KiB本地暂存区。历史core-only二进制缺当前源码／配置溯源，版本探测不算kernel测量；标准APU可执行资产也未取得。

| 途径 | 所需最小落实 | 能说明什么 |
|---|---|---|
| 忠实原八PE程序 | 对应PULP配置／运行时、存储与DMA预算及实际周期／事件；当前未有这份同配置资产 | 可以提供原平台证据；不能直接当现CVA6／FPGA时延 |
| 单CVA6平台投影 | 当前core源码构建、bare-metal加载／同步、六槽实际存储、同clock DMA/DDR及可见最终写入 | 可以测选定CVA6平台；更换计算资源须明确，独跑常数不能替代共享路径 |
| 有限端口计算模型 | 六槽96KiB、固定有限端口／队列、一个明确乘法／累加器、私有top和工作流driver；计控制及服务争用 | 是合法可做的定义模型；周期来自实际实现，不是原八PE或现成FPGA实测 |

第三种是后续可选的小模型，不因组件不同就否定其研究用途。但它至少要增加计算与RAM端点、控制／仲裁、私有展开和独立数据／协议验收，不能免费把C++算完的tile塞进旧DDR模型。六个DDR地址也不能冒称本地SPM。当前工作量卡没有自动选择新计算架构；本批按计划的小适配关口登记这笔额外投入。

[私有桥预算](../out/workflow-stream-contract-20261012/private-bridge-budget.json)列出四个SV/CPP单元：必要算术package、合并SPM/计算单元、私有SYSTEM top、工作流driver，另计runner、源码依赖、仲裁及独立协议／数据验收。至少需一次新私有展开；现有archive的第二端点没有这些端口，不能只重链接C++增加计算器。其单总读写许可／拍、一个乘法器、同步读latency1是待冻结架构，工作量下限不是原应用实测周期。

DMA还需冻结实际V通道数、软件供给、完整映射、A有限工作量和双方排空。普通N=false、batchoff的16KiB COPY在源码接口上可达：length取num_bytes，hardware legalizer拆burst，FIFO容量不是COPY长度上限。既有batch限定4KiB、program_single硬写4096，私有driver需参数化长度并映射整个16KiB。接口可达与实际执行是两回事，本轮没有16KiB目标测试。源程序的三个请求流和8 stream ID不等于8笔backend事务一直在途；N的4KiB全缓冲安全证明不能外推到16KiB。

## 4. 后续判别怎样进入，怎样结束

本轮剩余缺口已逐项记入[计时关口登记](../out/workflow-stream-contract-20261012/timing-gap-register.json)：同clock计算与末次C写入可见、实际六槽有限存储、DMA／供给预算及普通策略的真实损失。首先落实其中一个明确平台合同；不得调一个恰好制造气泡的compute delay，不为维持候选建设完整PMCA。

合同通过后才登记一次有界G0：固定内核／输入／六槽／映射／双方工作量，比较匹配独跑与共享强普通策略；若有气泡，再仅作必要的低翻译压力或已核可执行服务干预。合法大页、提前准备、B/A普通复用、直接利用程序就绪／槽释放、普通服务规则以及相关既有反馈均公平允许并计成本。不能把普通方法尚未接入写成普通方法失效。

**候选进入条件：** 可达的翻译等待确实传播到有用输出，普通具体策略仍有损失，候选改变可执行决定且完整成本不过收益。当前没有这些数据，因此不进G1、留出或方法原型。

**本批终点：** 实际应用来源／功能／依赖关完成，计时与架构成本关待落实；B保留条件备选，不是科学阴性。若后续证据成本超过立题投入，就结束本选题轮次并保留缺证卡，不扩大工作流搜索凑创新。A已有的登记期限阴性不调整，旧PN门控不恢复，方法论文不自动改成表征论文。

## 5. 执行与保留

实际新增：主机功能编译1次、功能运行1次、独立验收1次及3个内存负例；另有2次RISC-V编译接口检查和历史可执行版本探测，均非应用目标计时。无新Verilator展开、DMA性能模型进程、FPGA构建或板测。作者与实际执行分别记录，不把源码席的提案描述成它已运行程序。

[执行账本](../out/workflow-stream-contract-20261012/execution-ledger.json)分列根执行和席位记录。[源码保护](../out/workflow-stream-contract-20261012/source-guard.json)136项哈希检查覆盖130个不同路径，零不符，包括22份下载源及旧模型／产物；不是全仓库完整验收。旧状态、计划和本次改动文档原样快照在`out/workflow-stream-contract-20261012/before/`。没有删除历史、清理脏工作树或修改生产接口。
