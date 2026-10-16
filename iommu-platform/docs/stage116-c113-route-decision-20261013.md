# stage116：C113 采证路径与平台适配裁决

**决定：C113 条件封存；唯一下一任务为一个未关闭工作流的来源预筛。** 当前材料没有具体可接受的原生运行包入口，也没有已接受、保留原研究因果的 FPGA CPU-Adam 投影。未知不构成科学阴性，不宣布多 DMA 共享翻译主问题结束。本轮未执行后继任务。

本轮依据用户确认的两项选择执行：先做路线裁决；分开来源与准入前普通基线判别。旧 stage115 的完整取包目标仍 paused、未完成，原目标字串和记录不改。本轮 stage116 是独立的已授权有界交付，**未登记为实际 goal**：实际接口不允许覆盖未完成目标，先前拒绝已有真实记录，本轮不重复失败调用，也不把路线交付标作旧 goal complete。

## 一、修订后的执行关口

| 关口 | 必须成立 | 本关口应取得的交付 |
|---|---|---|
| 来源／接口 | 一个真实任务的owner、producer/consumer、依赖、字节；实际共享翻译路径和自然有用peer；合法接口及有依据的预算；可执行普通对照和全部成本项目 | 可核需求／接口卡、具体待判别缺口、未知和适配清单。数值结果／全部耗时／普通残余不再要求先装在一个外部包里 |
| 准入前有界普通基线判别 | 来源／接口全部通过，另有明确的执行合同 | 数值正确性、成功字节、完成／排空／释放、实际资源和全部成本；判断具体损失及普通方法是否覆盖 |
| 正式G0／G1 | 普通判别支持具体可达、可避免翻译残余后才登记G0；G0支持后才进入G1 | 失效原因、控制时机、相邻反例、准确前作和净成本支持的新动作 |

这项修订适用于后续任务。**stage115 原完整证据包标准未降低，也不被改写成已完成。** 实际版本、路径、自然peer或接口未知时仍不准入基线；缺成本实测不是性能收益，不把未知填零。[完整关口合同](../out/stage116-c113-route-decision-20261013/gate-contract.json)。

## 二、C113 原生证据路线

源码支持 CPU-Adam 对p/m/v真实更新后回写，及条件性的PCI NVMe请求map/unmap；这不证明所运行文件路径经过启用翻译的NVMe，也不证明另一设备自然同时穿过同一单元。已有三个入口仍分别是未执行模板、CPU offload日志、参数NVMe／optimizer CPU的用户错误报告，没有新的同次原生运行包身份或共享路径。

每项事实均分为已有原始证据、后续需测量、当前授权输入中无法取得。共17项，包括实际版本/启动/config、FS/block/PCI/domain/commonunit/peer、N/B/p/H/pages/layout、数值/完整完成和普通成本。未来测量必须先有来源和接口，不是允许部署补证。[证据缺项表](../out/stage116-c113-route-decision-20261013/evidence-route-matrix.json)。本轮未广搜issue或读取另一个工作流。

## 三、当前已验 FPGA 接口的能力与边界

直接核 frozen RTL 的四个iDMA→mux→IOMMU路径，以及已验有限模块的所有权、NEXT/DONE、quiet和核字节逻辑：

- 四个 DMA 共享硬件路径有源证据，但不自动产生 C113 的自然多请求者需求。
- 有限模块在已验 repair snapshot 中。批次触发和退休均检查所有引擎静止，phase按顺序调用批次；其消费者对比seed字节，不是CPU-Adam数值更新。
- 当前 tracked Linux `idma.c` 有pair／composition实验入口。**不据有限模块推断所有接口或硬件都不能并发**；这些实验入口仍不是已接受的C113逐槽消费与生命周期合同。tracked树没有有限header的那两个猜测路径，已纠正证据位置，没有把旧模块恢复进源码。
- host CPUAdam有Torch/PyBind及SIMD依赖；源码算术不证明已有可在CVA6执行并已验数值的运行包。这里不宣称全仓不存在某种实现，也未重新验板端身份。

[20组源证据](../out/stage116-c113-route-decision-20261013/critical/root-source-review.json)／[实际路径与版本观察](../out/stage116-c113-route-decision-20261013/critical/interface-location-observation.json)。

## 四、最小适配需求与因果限制

至少还需：真实CPU-Adam消费者及数值参考；可在其他合法DMA进行时安全交接CPU/设备所有权的逐槽合同；有来源的自然有用peer及明确保留下来的映射机制。端口、准备、同步、发布、验证、排空、释放和适配成本均未测，保留null。

更重要的是，把NVMe文件请求改为固定DDR池复制，会删除FS/block/media及按请求映射生命周期；现FPGA允许普通持久映射，不能为制造差异再强制每copy remap。两个AIO句柄也不能人为改成两个必须隔离的DID。真实CPU计算本身不制造第二个DMA peer。这些缺项即使软件能移植也不自动消失，所以当前不批准投影、适配或基线实验。[投影裁决](../out/stage116-c113-route-decision-20261013/projection-feasibility.json)。

## 五、唯一下一任务

冻结为 **stage117：一个未关闭工作流的来源／接口预筛**，未登记、未执行，不自动启动。由执行者负责定向选源，GPU／AI优先，先冻一个名称/版本/原始出处与新增事实；随后只核这一工作流。首先回答谁搬什么、实际共享路径、真实消费依赖及现平台合法接口，不要求用户先交出新事实，也不要求外部来源预先包含本研究所有成本实验。

允许结束为推进、条件保留或本轮无候选；没有具体来源或合法接口就不给基线准入，不用第二个候选凑门槛。不恢复R107/C112/row/HERO/coldweight/sharedcopy/fixedA8，不造压力、隔离、冷却或期限。本轮没有为stage117选择具体工作流。[唯一下一合同](../out/stage116-c113-route-decision-20261013/next-goal-contract.json)。

## 六、执行与验收

Codex-only，root按Sol／ultra、原生上下文默认要求执行；实际runtime model/effort/window不可独立读取，不以旧元数据证明本席。无新agent／DeepSeek／Claude／Astra、生产ABI／driver／RTL改动、build／board／GPU／NVMe部署、实验或已验校验器重跑。路径猜测失败已入账并纠正；没有把它当实验失败。

[稳定裁决](../out/stage116-c113-route-decision-20261013/root-result-acceptance.json)／[实际账本](../out/stage116-c113-route-decision-20261013/execution-ledger.json)／[交付核验](../out/stage116-c113-route-decision-20261013/validation/delivery-check.json)。本轮验证限于新合同／证据／记录一致性，不证明新原生包、性能收益或论文完成。
