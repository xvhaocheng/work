# C113 已发表运行与硬件边界：stage114 独占证据报告

结论：取得了同一 ZeRO-Infinity NVMe 分块 CPU optimizer 工作流的已发表实验配置，足以加强“真实模型规模造成 NVMe 需求”的来源事实；仍未取得可将该运行闭合到文件系统、PCI requester、启用翻译的 IOMMU/domain、自然同单元 peer 和普通完整成本残余的原生记录。缺项是部署证据未知，不能据此判定无共享、无翻译或无贡献。本席没有运行实验、测试、构建、板命令或生产改动，没有调用 DeepSeek／Claude，也没有登记 G0／G1 或新 goal。

## 1. 单一既有 published run：只按同一表／图收字段

选定 **原论文 Table 1 / Figure 5c 的单节点、0.5T 标称模型**，不把其他工作流、另一个图的 batch 或后续代码配置混入。

| 字段 | 已发表值 | 证据及限制 |
|---|---|---|
| 模型 | GPT-like Transformer，标称 0.5T | txt:570–577，PDF9–10；没有完整模型 JSON、词表、精确参数总数 |
| hidden / layers / sequence | 18K=18432 / 124 / 1024 | Table1 的 K=1024；不是 Figure2a 中 0.50T 的 hidden20K/layers100 |
| 节点／GPU参与数 | 1个 DGX-2 节点，16 GPU | Table1 与§8.4 txt:599–600；GPU 为 V100 SXM3 32GB，§8.1 txt:555–558 |
| MP | 1 | Table1 txt:577；正文该图明确无 model parallelism |
| batch/GPU | 8 | Table1 txt:577；native gradient_accumulation_steps 未给 |
| FP16 parameter storage | NVMe | Table1 txt:577 |
| optimizer state storage | NVMe | Table1 txt:577 |
| optimizer compute | 设计要求取 chunk 到 CPU 更新并回写 | §5.2.2 txt:427–438，PDF7；未公开这一运行的 CPUAdam 类／内核版本／命令 |
| rank | 16个GPU参与；若一GPU一进程且无其他划分，可推16个DP参与者 | 这是条件推论。native world_size、rank ID、CPU/NUMA/文件/设备分配未知，不能声称取得精确 launcher |
| 硬件名义容量 | GPU0.5TB，CPU1.5TB，NVMe28TB／节点 | Figure2b PDF4原图已视觉亲核；不转换成实际 H 或可用文件字节 |
| 实际可用 H／pinned pool | unknown | §6.3仅说 reused tens of GB，不是该运行预算、B 或 Lmax |

该表还公开 50B／100B（hidden8192、layers62／125、batch26／24、FP16 CPU、OptStateNVMe）和1T（hidden25600、layers128、batch7、两者NVMe）单节点实验，但本结论的容量判别只用0.5T这一行。完整字段逐行保存在 [summary.json](/home/xvhaocheng/work/iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/summary.json)。§8.1说明实验最多32节点／512GPU和800Gbps节点间带宽；不把这个最大集群当成该单节点运行的配置。

0.5T是论文标称规模。依据C113已固定的三项FP32 p/m/v状态边界，**标称0.5T × 12B ≈ 6TB**，已经超过Figure2b的单节点CPU1.5TB名义物理容量。因而这份来源不能再以“全部 optimizer 状态都可CPU常驻”抹去NVMe容量需求。此处6TB只是标称规模的容量推论，**不是 exact P、实际 H、padding 后 N_i、文件／wire／DDR／DMA字节或v0.16.4测量**。它也不排除 CPU 内合法驻留的当前 tile、稳定 scratch 和有限 persistent pinned pool；NVMe必要性本身不强制每步发生TLB损失。

## 2. 原文公开了什么，没有公开什么

原论文的实验实际为§8（PDF9–11），§7是实现；Appendix A在PDF14。§5.2.2把NVMe optimizer更新明确写成NVMe→CPU分块、CPU更新、CPU→NVMe回写；§5.2.2及§6.3写出异步read/write、explicit synchronization、NVMe读与回写和CPU optimizer计算重叠、pinning、buffer复用（critical-quotes P02–P05）。这些是同一工作流的设计及已发表运行背景，不是固定sha运行的动态证明。

| 闭合字段 | 本关口资料支持程度 |
|---|---|
| nvme_path / mount / 文件系统和 mount options | unknown；CPU/NVMe storage label不提供路径 |
| RAID／partition／block device／真实NVMe数量 | unknown；28TB容量不等于设备数或映射 |
| NVMe PCI BDF／requester ID／CPU root／PCI switch | unknown |
| IOMMU enabled / translated 或 passthrough／domain／共享翻译单元 | unknown；同node或PCIe路径不证明启用翻译或同单元 |
| 物理 peer 的 requester／domain／同时 DMA／结果 owner | unknown |
| GPU数量及算法自然 overlap | 已公开；16GPU参与及NVMe/CPU/GPU重叠设计。仅能证明算法层并行需求，不能闭合同翻译单元peer |
| T / N_i / B / p / H | 全部unknown。p是C113合同中的AIO intra_op_parallelism；大页／4KiB、IOVA叶布局另为unknown |
| pin/unpin、premap、map/unmap和IOMMU缓存生命周期 | unknown；buffer reused/pinned不等于DMA映射持久，也不强制内容更新需要撤权 |
| full file/wire/DDR/DMA bytes、原生数值输入输出、step/DONE/quiet/释放 | unknown；论文吞吐结果不是这些轨迹 |
| 普通完整成本后的具体可避免翻译残余 | unknown；论文没有归因到IOMMU的对照 |

多个进程／AIO请求、读写双向或bulk async不能当成多个物理NVMe requester。CPU optimizer线程不是DMA peer。多GPU存在也不能自动当成GPU+NVMe共享enabled IOMMU的证据；实际requester和同时活动窗口需要原生记录。本文未把后来固定代码的普通queue、pool配置倒灌到2021 run。

## 3. Figure2b／附录／官方材料的范围

Figure2b标题称available memory，表体是aggregate hardware容量和每GPU achievable bandwidth；其单节点CPU1.5TB、NVMe28TB属于硬件名义上限。图没有进程其他常驻量、可pin额度、NUMA可达预算、空闲空间、buffer reservation或实时可用H。原文同时明确pinned memory稀缺，过订阅可降低性能或不稳定，并用“小量、tens of GB”复用来承载大量状态（txt:491–503，PDF8）。这不能提供有限数值B*Lmax或H。

附录各表按其对应图独立保留：Table4 Figure6a的1000B行为16GPU、MP4、batch5、total20（txt:799）；Table1 Figure5c的1T则MP1、batch7。它们可能属于不同配置／协议，不能拼成同一run，更不能静默改附录MP。Table8 Figure6e的NVMe行为64GPU、MP1、batch4、total128（txt:830），在普通“一GPU一进程”读取下乘积为256，与列出的128不闭合；原文无launcher或batch定义解释该差别。本席保留原值和不确定性，不替作者纠错。Table5的单层tiling模型和其他8B系统特性实验也不倒灌到0.5T主行。

固定DeepSpeed sha下下载的官方 [SC21-ZeRO-Infinity.pdf](https://raw.githubusercontent.com/deepspeedai/DeepSpeed/e2dc3eeb1923073e32739596a4fd051417d4ff92/docs/assets/files/SC21-ZeRO-Infinity.pdf) 是23页报告slides。第3页图再次显示单节点0.5／1.5／28TB，第11页是overlap、DeepNVMe和pinned层示意，第14–18页是模型规模／吞吐／scaling／特性图。它没有native run JSON、launcher、路径、rank map、IOMMU配置或原始完成日志。文件元数据creation为2023-10-19 UTC；这只说明归档PDF的导出元数据，既不能推出SC21实验发生在2023，也不能证明运行代码版本。固定asset所在版本不等于实验执行版本。

仅沿原论文ref[40]打开的 [NVIDIA 2019 DGX SuperPOD原始文档](https://developer.nvidia.com/blog/dgx-superpod-world-record-supercomputing-enterprise/) 在原始正文列通用DGX-2H存储：DDR4 1.5TB、内部NVMe30TB／节点（本地txt:119–126），并说明GPU到网络端口的PCIe switch路径（txt:86–87）。它是通用平台材料，不是原论文单节点配置清单；30TB与论文28TB分别保留。不能用该文中Spectrum Scale／NFS训练数据层代替实际optimizer nvme_path，更不能推NVMe/IOMMU域。文章现网页包含现代AI summary，本席只用原始正文，没有采用summary。

作者 [Microsoft Research publication页](https://www.microsoft.com/en-us/research/publication/zero-infinity-breaking-the-gpu-memory-wall-for-extreme-scale-deep-learning/) 的返回内容为April2021/arXiv metadata和摘要，未公开原生实验附件。其Publication短链访问失败，direct HTML403，均记账，未继续搜索其他platform资料。

## 4. 翻译残余与普通办法的正反条件

文中§8.2将10T→20T吞吐下降归因于CPU内存容纳activation checkpoint而压低batch，明确写“不是NVMe bandwidth”（txt:607–613）。这说明存在应用／容量原因，不能把offload慢自动改称translation慢；也不能据此否定其他未测场景的翻译影响。§8.6的prefetch/overlap对照、activation offload成本是应用系统优化事实，没有计入完整premap/map、page layout或IOMMU对照。§8.5的2GB预碎片化是刻意控制GPU连续分配的实验（txt:662–668），不是自然IOTLB压力，不可作为本关口natural pressure。

普通CPU驻留（能合法fit的部分）、4KiB／大页、布局／批量、提前准备、预映射／持久映射、稳定scratch、合法已有pool与队列都保留。论文pinning和复用既不提供它们的完整成本，也不证明这些方案失败；本席不据此挑选G1动作。

如果一份**既有同workflow原生记录**同时给出实际NVMe物理requester和enabled translation path、自然同时活动peer及同共享翻译单元，并以完整byte/数值/完成/quiet和普通全成本对照留下具体可避免翻译残余，就可支撑进入后续最小关口。若具体部署显示passthrough／无启用翻译、没有自然同单元peer，或合法ordinary已消除残余，该部署不满足相应条件。当前材料未给这类正反部署事实；因此缺证据不作为科学阴性。

## 5. 最小缺项与交付导航

下一步仍只需要这一来源的一份既有native run bundle：精确软件／模型／optimizer／launch与rank配置；对应saved nvme_path→mount/FS/block/PCI/IOMMU/domain/common unit和自然peer活动；实际T/N_i/B/p/H、buffer与页布局；完整数值、byte、完成/quiet及ordinary成本证据。继续读通用DGX或算法图不能补这个缺口，本席据此停止有界查源。

- [summary.json](/home/xvhaocheng/work/iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/summary.json)：选定run及其他公开字段、unknown矩阵、接受／禁止推论和最小缺项。
- [critical-quotes.json](/home/xvhaocheng/work/iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/critical-quotes.json)：原论文逐组原文／行号／PDF页、Figure2b视觉原表完整数值、官方资料边界。
- [source-manifest.json](/home/xvhaocheng/work/iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/source-manifest.json)：完整PDF／txt、官方硬件HTML／txt、关键页面图的SHA256和PDF metadata。
- [bounded-search-ledger.json](/home/xvhaocheng/work/iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/bounded-search-ledger.json)：所有定向访问、获取失败及停止理由；零general／issue search。

关键原始PDF hash：arxiv v1 `6b291a9bf12fe52d6eafda2ec69d65cbab59db046cf27d8ecb4912331dc402c1`；固定sha SC21 asset `2abea0e5fbb72ac73c530b82015c77d4aeb3584684bbae8ebcf5085f31de5615`。完整原始材料位于本目录source/；MSR raw HTML受403阻断，未声称完整留存该页。
