# stage113：ZeRO-Infinity 来源合同条件保留，现 FPGA 实验不准入

2026年10月13日 JST。整体仍在阶段一立题。实际 goal 已登记，无额外 token 预算；本轮只完成来源、需求与接口裁决。Codex-only，root 与一个独占 Sol／ultra 子席；没有 DeepSeek、Claude、Astra、G0/G1、新实现、构建或设备实验。完整交付及状态同步后，实际 goal 已由工具标 complete，见[真实回执](../out/stage113-zero-infinity-admission-20261013/goal-completion.json)为准。

**决定：条件保留 C113 的原生来源合同，当前 FPGA 投影不准入实验。** 新源码事实是 CPU Adam 更新产生真实回写依赖，以及固定 pinned 缓冲仍走 Linux NVMe 按请求映射／解除映射。没有实际配置、共享翻译拓扑、自然并行的另一物理搬运者和完整普通控制后的损失记录，因此不能把这些事实接受为共享翻译性能缺口。这个结果不是科学阴性，也没有恢复 R107、C112 或其他关闭卡。

[机器裁决](../out/stage113-zero-infinity-admission-20261013/root-result-acceptance.json)／[完整证据导航](../out/stage113-zero-infinity-admission-20261013/README.md)／[root关键原文](../out/stage113-zero-infinity-admission-20261013/critical/root-source-review.json)。

## 单一来源与新增事实

本轮只选 **DeepSpeed ZeRO-Infinity 的 NVMe 分块 CPU-Adam 优化器更新**，冻结 `v0.16.4`、提交 `e2dc3eeb1923073e32739596a4fd051417d4ff92`。官方 tag 解析元数据、完整相关源码、失败下载和 SHA256 均保留。[冻结版本](../out/stage113-zero-infinity-admission-20261013/source/version.json)／[源码清单](../out/stage113-zero-infinity-admission-20261013/source/manifest.json)／[直接依赖](../out/stage113-zero-infinity-admission-20261013/source/root-direct-dependencies.json)。采用这个固定版本，不把当前网页新增接口或默认值混进旧源码。

原论文已经讨论 NVMe 读、NVMe 写与 CPU 优化器计算重叠；不能把这条流水线本身称作本研究创新。[原论文 v1](https://arxiv.org/abs/2104.07857v1)、[保存的原文与哈希](../out/stage113-zero-infinity-admission-20261013/source/paper-manifest.json)；对应文本 `source/ZeRO-Infinity-2104.07857v1.txt:383`。本轮没有提出新机制，无需把无对象的前作搜寻当作创新验收。

相对关闭卡的实质新增不是换模型或增加压力：R107 搬回既有 KV 数据；这里 CPU 算术真实改变参数、动量和方差后才能回写。C112 的消费者和物理搬运链未成立；这里的 CPU 计算及文件 AIO 调用链有具体实现，但部署设备和共享路径仍没有原始运行证据。

## C113 需求／接口卡

| 字段 | 原生源码合同与证据 | 当前接受边界 |
|---|---|---|
| 有界任务 | 一个 rank 的一次成功、非 overflow、稳定阶段优化器更新；含此前梯度存盘与更新后状态回写完成 | 不代表整个训练迭代；初始化、上游 backward／归约和下游参数消费另列成本 |
| owner | 训练 rank 的 optimizer、参数／状态张量和 `optimizer/rankN` 文件；一个配置 `nvme_path` | 逻辑 rank、read/write handle、线程和 queue 不等于独立安全 owner 或 DID；文件路径的物理后端未知 |
| producer | backward／归约后的梯度；CPU unscale／clip 和 Adam 产生新参数、m、v | 没有原生数值输入、输出或 trace，不用模式种子冒充它们 |
| consumer | Adam 消费当前块参数、梯度、m、v；FP32 参数发布为低精度分片；更新状态供下一次使用 | 下游训练完成、GPU完成和磁盘持久化不是同一个完成点；未测时不合并 |
| 依赖 | flush 梯度写→当前块读完成→unscale／clip→CPU Adam→低精度参数发布→状态写；下一块读可预取，上一块写可延后等待 | 可并行的是不同块；当前块的写不能早于真实更新，buffer不能早于I/O完成重用 |
| 生命周期 | pinned池在构造时分配、清零并锁页；slot `free`只归还索引，不解除锁页；每个文件操作单独open、wait后close | 应用池寿命、文件fd寿命、block request的DMA映射寿命分别记录 |
| 完成 | Python要求wait返回操作数；最后可交换块强制同步写；C++收取完成事件后finish／close | 所核AIO收取点未检查每个event的`res/res2`，async入口关闭字节验证；计数不是完整成功字节证据。没有观测到损坏或短I/O |
| 映射／物理搬运 | PCI NVMe驱动对每request调用DMA map，完成时unmap；使用该PCI函数的`struct device` | 这只证明所核驱动结构。实际`nvme_path`是否直连该NVMe、domain、IOMMU bypass／ATS／桥和同时经过的peer均未知 |
| 目标 | 保证真实更新与回写完成，在完整成本下减少其关键路径上可避免的翻译等待，同时保留其他真实工作的进度 | 来源没有提供本轮到达、期限或翻译损失，不能指定任意sleep、冷却或人为deadline |

关键代码：[step次序](../out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/zero/stage3.py:2091)、[CPU计算](../out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/ops/adam/cpu_adam.py:136)、[真实算术](../out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/csrc/adam/cpu_adam_impl.cpp:45)、[池复用](../out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/utils.py:184)、[同步及预取](../out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/pipelined_optimizer_swapper.py:103)。

## 完整字节与资源合同

这里只交付符号合同，不造一个较小数值实例。每rank实际可交换块数、模型／梯度形状、DP padding、运行配置、CPU预算 H 和原始文件长度均未知。[完整机器合同](../out/stage113-zero-infinity-admission-20261013/byte-and-resource-contract.json)。

令 `N_i` 为第i块分配的FP32元素数，含partition padding，有用元素 `n_i≤N_i`；AIO线程并行数为p，元素对齐 `a=256p`，每个state文件期望长度 `L_i=4a⌈N_i/a⌉`。令 `Q_i` 为达到交换阈值、实际写入文件的对齐梯度段元素总数。小段和拆出的尾部可驻留CPU，不强制交换。阈值是 `max(1MiB, block_size)`，不是任意小tile。

稳定阶段Adam有三个状态文件：FP32参数、m、v。`step`是CPU标量，不额外算成一个张量文件。包含此前梯度存盘的这份有界任务，其期望AIO字节是：

| 类别 | 期望请求字节 |
|---|---:|
| 状态读 | `3ΣL_i` |
| 状态写 | `3ΣL_i` |
| 前置梯度写 | `4ΣQ_i` |
| 更新前梯度读 | `4ΣQ_i` |
| 合计 | `6ΣL_i+8ΣQ_i` |

仅在所有梯度都交换、`Q_i=N_i`、且state无需额外对齐时，合计才简化为 `32ΣN_i`，读／写各 `16ΣN_i`。这是源码请求合同，不是实测NVMe、wire或DDR字节。读入口按文件stat长度操作；真实文件必须与期望形状逐项核对。首次m/v懒初始化、overflow跳过、残留小梯度、参数另行NVMe发布和异常清理须分别计，不能套稳定阶段公式。

CPU逻辑操作数也单列：unscale／clip的梯度读写约 `8ΣN_i`；Adam读取p/g/m/v并写p/m/v共 `28ΣN_i`；若执行FP32→FP16发布，读写共 `6ΣN_i`。它们不是额外NVMe载荷，也不等于物理DDR流量。GPU D2H/H2D字节、缓存命中、DMA读写事务拆分和协议开销没有记录，不从逻辑操作数硬推。

常驻pinned swap池为 `B×L_max`。初始化清零／锁页有成本。成熟块通常占4个slot（p/m/v/g）；两个块的read-ahead至少需8个，若上一块write保留4个allocation，而当前块与下一块各保留4个，双流水内部阶段可需12个。这里是源码顺序的条件容量界，不是已分配／已运行结果。v0.16.4默认B=4且pipeline_read/write=false，不能宣称默认就有双流水，不能为寻找收益修改配置加压。全CPU驻留普通对照需参数／m／v至少 `12ΣN_i`、梯度最高 `4ΣN_i`，另加低精度参数、模型、runtime、队列和元数据；H未知，既不能免费假设装得下，也不能断言自然装不下。

## 实际共享路径与FPGA接口

root亲核Linux `c0f5e13ea81fa30d78f3bb1d6389e2397321e040` 的NVMe请求链及关键DMA文档；子席完整边界见[独占报告](../out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/report.md)。本地旧脏改动保留，源码事实不冒充部署配置。

`pci.c:730–818`按request建立DMA映射，`:961–980`在完成前unmap，`:2936`关联PCI设备。两个read/write AIO句柄使用同一NVMe后端时，仍属于同一PCI requester；一个NVMe的多个queue可以并行，但不是多个物理DMA设备。选中inner loop使用CPU Adam，不能凭GPU训练应用名就声称GPU copy同时穿过同一个主机IOMMU。实际存储path解析、设备ID、domain/翻译模式及自然同时活动peer都缺运行记录。

普通持久streaming映射有Linux合法接口：一次map、重复CPU/device sync、最后unmap；DeepSpeed这份文件AIO入口没有保留NVMe IOVA的用户接口。将来若增加驱动注册，必须把权限、方向、缓存交接、错误处理和最终撤权成本全部计入；不能把“接口需要实现”称作普通方法在科学上失效。[原接口说明](/home/xvhaocheng/work/iommu-platform/linux/Documentation/core-api/dma-api-howto.rst:607)。

本地RISC-V驱动的range失效未设置地址／PSCID，map同步也调用同路径；通用DMA-FQ可延后失效，本地驱动不宣告DEFERRED_FLUSH能力。只有所选设备真的使用相应翻译domain时这些代码才关联该任务；不能据此宣称每次unmap必清全硬件TLB或已经造成loss，也不能把普通驱动失效范围修正直接归为新方法贡献。本轮不修该驱动。

当前实际FPGA冻结源码是 `fpga/src/ariane_peripherals_xilinx.sv`：四iDMA经mux接同IOMMU TR口（`:870`、`:1218–1282`、`:1451`），不是通用tb单DMA wrapper。原已验有限接口能整池映射、NEXT发布、匹配DONE、quiet与checked释放；原程序要求all-engine idle并顺序执行，消费者是CPU字节检查，**不是CPU Adam或NVMe文件I/O**。旧结果只证明这些原能力，本轮未读板端新身份或执行设备命令。

一个未来合法有限投影可用真实CPU算术和DDR中明确授权的数据搬运，保留相同owner池的持久映射、大页和单DMA合并；它必须明确说明删去了哪些GPU、NVMe和块设备成本。现在没有接受这样的任务清单、真实输出或并行消费者接口，未实现。尤其不能为了复制原生驱动临时映射而禁止FPGA普通整池映射，也不能把一次SSD读改成额外DDR copy后声称自然多DMA任务已经成立。

## 普通强对照与完整成本

| 合法对照／成本 | 必须保留或计入的内容 | 本轮数值状态 |
|---|---|---|
| CPU驻留／预算 | H允许时省掉NVMe交换；梯度、状态、低精度副本、runtime均占用真实资源 | H、总模型／状态未知；不设人为不足 |
| 大页与4KiB回退 | 物理连续、IOVA对齐、驱动支持时允许大页；pin／O_DIRECT不保证大页 | 真实page/layout/叶数未知 |
| 预映射／持久映射 | 既有合法驱动路径、固定pool、CPU/device sync；现FPGA整池驻留必须允许 | native注册接口未提供；FPGA旧能力已验，新任务成本未知 |
| 批量／布局／供给 | 合法合并连续范围，保留队列深度、预取、普通调度和write-back；不可越过更新依赖 | block拆分、file/extent、线程/供给成本未知 |
| 同owner单DMA | 允许一个设备／一个授权DMA处理同owner读写；多DID不是应用语义义务 | 不强制隔离；吞吐／进度未知 |
| 稳定scratch与CPU路径 | 真实CPU Adam、合法复用、梯度小尾部驻留与低精度发布 | 已有源码，CVA6数值／软件适配与时间未验 |
| 准备 | rank初始化、状态lazy init、pinned池清零、文件/目录/fd、AIO context/thread、PRP/SGL和映射 | 结构已核，时间、实际peak和分配成功未知 |
| 执行／消费者 | 梯度生产及归约、存盘、读入、clip、Adam、格式转换、参数发布、状态回写与下游消费 | 所有新运行时间／数值未知 |
| 验证 | 成功byte数、短／错误I/O、完整输出数值、实际任务DONE与进度 | source wait数量不够；未执行验收，不声称失败运行 |
| 排空／释放 | 所有pending I/O完成后close、pool/handle清理、CPU/device最后sync、unmap/失效／fence、错误路径所有权保留 | 源码边界分列，完整异常销毁及实际成本未验 |
| 迁移／新控制 | 任何driver注册、消费者接口、元数据、策略资源、端口、延迟和切换／重热 | 本轮没有实现或新规则，未来不能填零 |
| 本轮研究执行 | 官方源码下载、源核、一个独占席、合同与文件交付 | 实际调用／耗时／token以账本和goal工具为准；不宣称节省 |

## 正条件、反条件与下一步

**正条件：** 若同一冻结工作流的原始配置和trace证明两个有用、自然并行的物理搬运者共享实际翻译服务，给出真实预算、完整字节、数值、mapping/ready/DONE生命周期，并在合法驻留、大页、批量、布局、持久映射及其完整实现成本之后仍留下消费者关键路径上的可避免翻译损失，那么这张卡可以进入最小G0。是否独立创新还取决于具体新增动作、准确前作覆盖与净成本；源生命周期事实不等于创新。

**相邻反条件：** 两句柄只是同一NVMe设备；CPU计算是唯一consumer且没有同时活动peer；合法CPU驻留已移除交换；或合法固定pool/persistent映射没有反复翻译身份变化。此时这项来源不能支持所要求的多DMA缺口。这里只给条件，未测完整任务损失不写为零。

**下一最小缺项：** 同一来源的一份既有原生运行配置＋对应记录，首先确认实际path→设备／domain／共享单元和自然peer，再给N_i、B、p、H、page/layout、真实数值与各阶段bytes/completion；比较合法ordinary后的残余和完整成本。没有这些资料，不启动FPGA adapter、NVMe driver改造或G0。下一合同[inactive](../out/stage113-zero-infinity-admission-20261013/next-goal-contract.json)，本轮不自动执行它、不追加第二个工作流。

来源任务和映射寿命作为可导航资产保留；当前FPGA实验入口不接受。本轮完成只表示stage113关口交付完成，方法论文未完成。
