# 单一需求卡：两 GPU pipeline rank 的冷权重准备阶段

结论：来源支持两个 rank 各自准备不同、完整、有限的 MLP 权重包；直接搬运仍需得到两个独立输出。Root 已选择此方向作条件 G0 入口。**这里明确提取并提前了准备排程，不声称原生同时加载、不声称源 GPU 共用 IOMMU、不声称 OPT-125m 容量逼迫 offload，也没有推理、仿真或板测结果。**完整合同见 [demand-card.json](/home/xvhaocheng/work/iommu-platform/out/ai-mandatory-work-g0-20261012/distributed-source/demand-card.json)，固定源字节、哈希和必要行段见 [source-manifest.json](/home/xvhaocheng/work/iommu-platform/out/ai-mandatory-work-g0-20261012/distributed-source/source-manifest.json)。

来源是作者 FlexLLMGen 固定提交 `004ffef82b46e8dc8685c55d0cdda650bdaf1269`。[dist_flex_opt.py:50](/home/xvhaocheng/work/iommu-platform/out/ai-mandatory-work-g0-20261012/distributed-source/sources/dist_flex_opt.py:50) 按连续完整隐藏层划分 pipeline rank；[opt_config.py:65](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-fpga-fit-20261012/source/sources/opt_config.py:65) 给出 OPT-125m 的 12 层、h=768、FFN=3072，[同文件:31](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-fpga-fit-20261012/source/sources/opt_config.py:31) 给出 FP16。`world_size=2` 得到 rank0 层0–5、rank1 层6–11；取两者首个本地 MLP：全局层0与6，`sep_layer=True` 时分别是本地 `j=2` 与 `j=1`。[原生设备绑定:550](/home/xvhaocheng/work/iommu-platform/out/ai-mandatory-work-g0-20261012/distributed-source/sources/dist_flex_opt.py:550) 为各进程选择 `cuda:local_rank`，global rank/world_size 进入划分。原生 CLI 只有设置 head-IP/port 才保留分布式参数，否则 [672行](/home/xvhaocheng/work/iommu-platform/out/ai-mandatory-work-g0-20261012/distributed-source/sources/dist_flex_opt.py:672) 归为单 rank；这里没有运行 CLI。

固定选定策略为 `percent=[0,100,100,0,100,0]`、`pin_weight=True`、未压缩权重/缓存、`sep_layer=True`、`overlap=False`、GPU batch size4、一个 GPU batch、两个 inner iterations、CPU cache compute关闭；通信取GPU，异步通信关闭。后面的 batch/通信参数只是可支持的原生配置，不参与该准备阶段的计算计时。权重放置由 [flex_opt.py:92](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:92) 决定；CPU 分配/pin由 [pytorch_backend.py:184](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:184) 决定，pinned CPU→CUDA 使用 [851行](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:851) 普通直接 copy。

每个 rank 的完整对象如下；文件身份分别为 `decoder.layers.0.<tensor>` 与 `decoder.layers.6.<tensor>`，不能把同一层重复复制冒充两个 rank 的独立工作。形状、文件命名和全部六个 `smart_copy` 来自 [flex_opt.py:477](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:477)。

| 张量 | FP16形状 | 每rank有效字节 | 每rank张量内≤4KiB job |
|---|---|---:|---:|
| fc1.weight | 3072×768 | 4,718,592 | 1,152 |
| fc1.bias | 3072 | 6,144 | 2 |
| fc2.weight | 768×3072 | 4,718,592 | 1,152 |
| fc2.bias | 768 | 1,536 | 1 |
| final_layer_norm.weight | 768 | 1,536 | 1 |
| final_layer_norm.bias | 768 | 1,536 | 1 |
| 合计 | 六个完整张量 | 9,447,936 | 2,309 |

双方共12个完整张量、18,895,872B有效搬运、4,618个可选4KiB job。原生是12次完整张量 copy；切成4KiB是现有诊断运输接口的投影，普通对照可用完整张量 direct copy。源与目的各自按张量4KiB对齐，四组缓冲共37,830,656B；在已登记48MiB载荷预算中余12,500,992B，仍须计页表、保护区、描述符、CPU/cache交接和Linux实际分配。若实际支持2MiB对齐连续映射，四组各10MiB，共40MiB；不能据算术认为已在Linux分配成功。

提取调用是 `layers[j].load_weight(weight_home[j], weight_read_buf[j], k=0)`。[MLP.load_weight:497](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:497) 只读取六个 weight_home、k、目标设备和输出 holder，**没有 activation、Task、cache、attention mask 前置依赖**；[MLP构造:462](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:462) 设置目标设备也不需要激活。CPU源权重必须已经初始化，输出 holder 为空；原生 `smart_copy` 过程中分配新的CUDA张量，投影提前预留DDR目的范围属于显式适配，成本计入。`k>0`不执行加载，不能替代这次冷准备。之后 [MLP.forward:519](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:519) 才同时需要激活和六张量。

原生依赖与投影释放必须分开：所有 rank-local homes 在 [DistOptLM构造:79](/home/xvhaocheng/work/iommu-platform/out/ai-mandatory-work-g0-20261012/distributed-source/sources/dist_flex_opt.py:79) 初始化，但 [normal loop:418](/home/xvhaocheng/work/iommu-platform/out/ai-mandatory-work-g0-20261012/distributed-source/sources/dist_flex_opt.py:418) 在层加载前收发跨rank激活；原生rank1首次计算依赖rank0输出，所选MLP也依赖各自先前attention计算。这里改成 consumer 执行前的共同释放准备：两组源已初始化/可见、两组目的不驻留且独立保留、映射已允许、两个实际设备空闲。两组 load 本身无互相数据边；rank0/rank1 后续计算依赖仍在准备阶段之外。所有控制器均允许提前准备；allocation/map/prewarm/CPU交接须从较早起点计入完整成本。

在规定冷初始状态和CPU-home策略下，两套目标权重都必须准备；direct 一跳不能删掉另一套输出。若已在目标设备，[smart_copy:142](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:142) 可以不复制，这是改变初始驻留状态或先前已经付了准备成本。合法保留目标驻留、一次完整提前准备、稳定mapping、正常预热、whole-tensor direct、2MiB页均是强普通对照。若接口允许，一个iDMA依次完成两套对象也应作为合法调度对照；来源证明两个独立消费者，不证明两个复制引擎都不可省。

FPGA绑定仅定义为：两个已有物理iDMA分别直接复制一套对象，经过现有共用RISC-V IOMMU，源/目的均为DDR内独立范围；DID与实际路径由root/平台审查确认。源 [README:113](/home/xvhaocheng/work/iommu-platform/out/ai-mandatory-work-g0-20261012/distributed-source/sources/README.md:113) 支持两GPU pipeline且建议不同机器，原论文 [文本:554](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/sheng23a.txt:554) 测四台机器各一GPU；它们没有提供原生共享IOMMU证据。这里只保留对象身份、有限工作、翻译与完成语义，不投影GPU/NCCL带宽、计算间隔或吞吐。

完成条件为每个job真实完成且无错误、字节区间恰好覆盖每个完整张量、必要可见性/cache交接完成，才可置tensor_ready；各六张量全就绪才可置rank_ready；双方全就绪且outstanding=0才算joint_ready。源保持不可变，目的保持到真实消费者结束或固定fixture核查结束；发出描述符不等于完成。目前未取真实权重版本，独立rank的确定性fixture只能验证对象身份/完整搬运，不能证明AI数值正确。

入口裁决：如果root接受上述排程提取、实际物理身份、内存/API/cache成本和完整强普通对照，可以进入一次有界G0；如果需要靠源自然同时加载、OPT125m容量强迫offload、源GPU共用IOMMU、强制两复制引擎或屏蔽prepare/大页来产生收益，就拒绝这种主张。这里没有普通失败、新机制或净性能收益证据，没有生成第三场景。
