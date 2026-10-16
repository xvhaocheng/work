# stage106：权重准备与本轮内持久 KV 写回来源卡

结论：固定 FlexLLMGen 源码确实给出两项不同用途的完整搬运：下一注意力层的全部权重进入计算设备，前一注意力层生成的完整 prefill K/V 进入下一生成步会读取的 cache home。它们并非同一对象的可选中转。**但只有明确选择 CPU offload 时这两项搬运才必需；源码并未证明该选择由容量或所有权约束强迫，也只有一个原生 GPU。因此本卡可保留为有条件的功能投影合同，当前不凭本卡单独注册多物理请求者的性能 G0。** 如果独立分布式来源席能给出真实 rank、不同对象的强制目的及完整完成，优先选择它；本席未读或裁决该席材料。

来源固定为作者仓库 FMInference/FlexLLMGen 的 `004ffef82b46e8dc8685c55d0cdda650bdaf1269`。以下均为现有原文直接核查，没有新增下载、模型运行或时间估计。`source-evidence.json` 给出本地路径、原始 URL、哈希及摘录。

## 有条件的有限合同

选择源码支持的 `opt-125m`，h=768、12 heads、head_dim=64、FP16；[opt_config.py:65](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-fpga-fit-20261012/source/sources/opt_config.py:65) 与 [opt_config.py:31](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-fpga-fit-20261012/source/sources/opt_config.py:31)。B=4、prompt P=512、generation G=32、一个 GPU batch，沿用源码这四个参数的默认值，[flex_opt.py:1283](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:1283)；型号并非 CLI 默认型号，默认型号是 OPT6.7b。本卡主动选择已固定来源的 OPT125m，不下载该模型。

明确策略为 `percent=[0,100,0,100,100,0]`、`sep_layer=true`、`overlap=true`、`pin_weight=true`、dense attention、CPU attention compute=false、压缩=false、stop=None、cut_gen_len=None。这使所有权重 home 为 CPU，cache home 为 CPU，激活与计算为 GPU；百分比是应用策略输入，**不是已验硬容量约束**。[flex_opt.py:92](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:92)、[flex_opt.py:321](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:321)、[flex_opt.py:1197](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:1197)。不把这个条件写成无条件的原生部署需求。

取单 batch 正式重叠循环的 `(i=0,j=2,k=0)`：层表为 j0=InputEmbed、j1=SA0、j2=MLP0、j3=SA1，由 [flex_opt.py:596](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:596) 确定。

| 有限任务 | 完整对象和源目的 | 本轮全部有效字节 | 独立用途 |
|---|---|---:|---|
| W：`load_weight(0,3,0)` | SA1 的 q/k/v/out 四个 768×768 FP16 权重和对应四个 bias，加 LN weight/bias；CPU `weight_home[3]` → GPU `weight_read_buf[3]` | 4,727,808 | `compute_layer(0,3,0)` 消费全部 10 个张量 |
| C：`store_cache(0,1,0)` | SA0 已生成的 K、V 各 `(512,48,64)` FP16；GPU `cache_write_buf[1][0]` → CPU `cache_home[1][0]` 的 `[0:512,:,:]` | 6,291,456 | 下一生成步 SA0 需要这个完整前缀；W 不能提供这些数值 |
| 合计 | 10 个完整权重张量＋2 个完整 prefill 输出张量 | **11,019,264** | 两项都完成与排空，不以只完成 W 为成功 |

W 的规格、完整 smart_copy 与消费：[flex_opt.py:281](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:281)、[flex_opt.py:309](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:309)、[flex_opt.py:434](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:434)。W 四个矩阵各 1,179,648B，六个向量各 1,536B；总量 `2*(4*h*h+6*h)`，不省略 bias 或 LN。

C 的 prefill 产生与存入待写缓冲：[flex_opt.py:444](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:444)。原文 `mha` 输出 K/V 为 `(s,b*n_head,head_dim)`，未压缩路径返回原 dtype 的 TorchTensor，[pytorch_backend.py:308](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:308)、[pytorch_backend.py:354](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:354)。每个输出 `512*48*64*2=3,145,728B`；[flex_opt.py:413](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:413) 将**整段 K 和 V**分别写回，非挑选小片段。不同层、不同 tensor、不同后续消费者，二者不存在 W→C 或 C→W 数据依赖。

## 完整 cache 边界与生命周期

每个 CPU cache home 的真实容量是 `(P+G-1,B*n_head,h/n_head)=(543,48,64)`，FP16，**3,336,192B/个，K+V 共 6,672,384B**。[pytorch_backend.py:287](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:287)。不是 P+G=544 行，也不是浮点32的 CPU attention workspace；本卡不走 CPU attention 路径。[pytorch_backend.py:196](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:196) 才是单独 FP32 workspace。

在本有限 prefill 写回结束时，只有 `[0:512)` 是有效产生的缓存，剩余31行是未来容量，不得凭分配形状宣称它们已就绪或把它们计作有效传输。后续 decode i=1…30 每次生成并持久写回一行，每对 K/V 是12,288B，位置 `[P+i-1:P+i)`；完整 G32 生命周期内实际持久写入542行，合计6,660,096B。i=31 是最后生成步，源码明确不写回；[flex_opt.py:405](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:405)、[flex_opt.py:700](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:700)。因此，不可把最后步的写回做成 mandatory companion。

下一个 decode i=1 的 dense GPU path 从 CPU home smart_copy `[0:P+i)`，即513行；原文下一步 `mha_gen` 将最后一行覆盖为新的 K/V，再做注意力。[flex_opt.py:359](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:359)、[pytorch_backend.py:401](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:401)。故本次512行是真实后续必用前缀；不能把额外复制的待覆盖行当已有有效历史数据。

这里“持久”仅指**本次 generate 内跨生成步存活**。generate 前清空/初始化 cache，结束后删除，[flex_opt.py:861](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:861)、[flex_opt.py:903](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:903)。没有跨请求持久 KV、掉电持久化或 fsync 合同。

## 就绪、同步和普通调度

W 源 home 已由构造中的 `init_all_weights()` 准备，早于 generate；[flex_opt.py:636](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:636)、[flex_opt.py:797](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:797)。C 在上一迭代 j1 的 `SelfAttention.forward` 产生，上一迭代末的 `sync()` 完成该 GPU 工作。在 j2 入口二者可以同时可用，但 W **更早**已可准备；不能假造共同释放事件以禁止提前准备。

正式循环顺序为 `load_weight(i,j+1)`、`load_cache(i,j+1)`、`load_hidden(i,j)`、`compute_layer(i,j)`、`store_cache(i,j-1)`、`store_hidden(i,j)`、`sync()`，[flex_opt.py:1011](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:1011)。W 走 load-weight CUDA stream；C 的 store 入口使用 store-cache stream，但本卡非 pinned CPU cache 的 prefill 大对象实际上派给 copy queue/worker stream。[flex_opt.py:653](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:653)、[flex_opt.py:716](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:716)、[pytorch_backend.py:837](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:837)、[pytorch_backend.py:878](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:878)。代码允许两项在途重叠，**不证明实际延迟、并发比例或哪个硬件瓶颈**。本席早期消息误将调试同步套入正式循环，已在后续消息纠正；本卡使用直接重读后的正式循环。

`general_copy` 文档要求 disk queue 与 CUDA 两种同步，[pytorch_backend.py:791](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:791)；`self.sync()` 依次 join copy queue、CUDA synchronize，[flex_opt.py:793](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:793)、[pytorch_backend.py:676](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:676)。Python 的 `cache_write_buf.store`、copy 入队或 `pop()` 都不是完成证据。源顺序没有 store-cache 前单独 sync；上一层的 KV 已由上一迭代末 sync 就绪。

原生意义下，W 的最近消费者是后一个 SA 层，C 的历史前缀消费者是后一个 token 的 SA0；这是 DAG 先后关系，不是自造时间 deadline，也不自动证明 W 必须被赋予特殊权限。有限搬运服务的成功条件为 W 的10个完整张量与 C 的2个完整前缀在目标可见，所有被许可的工作恰好完成一次，真实硬件完成与最终排空。若声称应用使用，还需证据证明相应下游 compute/load 真消费；本卡没有运行应用。

普通强对照至少包括：

1. **全驻留/部分驻留**：源码默认 `[100,0,100,0,100,0]`；GPU home 的 `smart_copy` 直接返回原 tensor，不产生 W 的复制，GPU-resident cache 不需要 CPU offload。[flex_opt.py:1291](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:1291)、[pytorch_backend.py:142](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:142)。`TorchDevice` 接收 mem_capacity，但 `allocate` 没检查该值，[pytorch_backend.py:162](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:162)、[pytorch_backend.py:184](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:184)。源码不能证明驻留非法。两项体积相加约11MiB，不足以靠体积本身证明本局部服务必须 offload；全模型其他对象的占用要单独证明，不能偷渡。
2. **resident direct**：在既有 DDR 功能投影中，W 和 C 各自直接从实际源到实际目标；去掉可选 pinned relay，但保留确实必要的两个目标。若 target 可直接引用/继续保有 producer 数据，普通别名/驻留必须获准；不得以禁止合法别名制造搬运必需。
3. **提前 prepare**：W 的源已早于 KV 就绪，可以先完成 W 再写 C；也可以在已就绪的 KV 上先写 C。完整比较从相同最早源就绪开始，计入提前传输、额外驻留及配置，不能把 prepare 移出计时窗口后只给新方法记录。整对象与页内分段都应允许，不人为只准一个4KiB槽。
4. **原始 overlap＋事件反馈**：按正式层循环提交；在合法发布完成后，普通反馈控制可先提交近期消费者 W、先排空已就绪 C，或按固定轮转/串行处理。W、C 都必须完成，最终 drain、软件供给、读写 bytes 全计入。页表提前准备、合法大页和 pinned cache-home 减少中转也不能先验排除；更改 pinned 策略须计实际内存成本。

## 平台投影边界与完整成本

原生只创建一个 `TorchDevice("cuda:0")`，[flex_opt.py:1192](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/flex_opt.py:1192)。三条 CUDA streams、copy worker 数、张量层编号或微批编号都不是不同物理 DMA 请求者或 RISC-V IOMMU DID。源码没有本卡两个原生物理 DMA 身份、ATS/PRI/PASID 或其 IOMMU 干扰证据。

若投影到现有 FPGA，可以明确定义一个真实 iDMA 负责 W CPU-domain 镜像→operand 区，一个负责 C producer-domain 镜像→cache-home 区，并通过真实两个 DID 访问已有 IOMMU。这是**现有 FPGA 的两个物理请求者承载源任务角色**，不是原生 GPU 多设备干扰。两个源/目标若同在 DDR，域名不能代替真的不可别名、权限和预算约束。所有域映射、完成校验与 CPU/cache 交接尚需 root 接受后落实。

至少需登记：完整12个 tensor 的源/目标占用；C home 的未来容量；页表/DDT/页内尾段；每channel合法在途额度和cfg提交；实际CPU供给与cache ownership；初始化、prepare、总读写、错误回收与最终drain。4KiB tensor内分段时 W 为1,158笔（四矩阵各288、六向量各1）；C 为1,536笔（K/V各768）；合计2,694笔，**不强套stage97的64笔**。不要求该分段一定优于完整对象或大页。

原生 prefill K/V 由 permute 后创建 Tensor，是逻辑形状，不应未经核对就视为物理连续数组；[pytorch_backend.py:323](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:323)、[pytorch_backend.py:354](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:354)。密集 DDR 镜像可验证**逻辑数据合同**，但若声称承载实际源 tensor，要加上真实 strides/packing 与CPU交接成本。copy worker 用 pinned relay 做 GPU→CPU 和 CPU→CPU 两次复制，[pytorch_backend.py:898](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:898)。worker每个 `torch.empty((1*GB,), dtype=float16)` 是2GiB，默认4个worker合计8GiB，不是4GiB；[pytorch_backend.py:624](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:624)、[pytorch_backend.py:882](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/sources/pytorch_backend.py:882)、[utils.py:17](/home/xvhaocheng/work/iommu-platform/out/gpu-ai-fpga-fit-20261012/source/sources/utils.py:17)。它不能免费加入现有48MiB载荷预留；更轻的原生实现是另一个需要完整计成本的对照。

## 进入/停止裁决

如果明确证明同预算下 W 不能先驻留、C 必须进入独立 home 且不能留在 producer；给出真实两个 iDMA 的身份/权限；保留W已有的提前准备权；并把完整成本和两项完成写进合同，那么可把本卡用于**AI来源的有限搬运 G0 投影**。这仍不等于已证明原生GPU多设备干扰、普通方法失效、新机制或净收益。

因为当前源码只证明 offload 政策可配置，默认驻留合法，现有DDR未建立不可别名义务，且原生只有一个GPU，所以本席**不建议现在单凭它注册性能G0或搭新模型**。如果本轮分布式来源也缺上述必要证据，记录本輪来源需求缺口；不要用最后步多写KV、固定等待、期限或禁止驻留来补洞，也不扩大搜索范围。

Root 本轮选择独立分布式来源的按 rank 不同冷权重准备，作为本轮唯一 G0 来源；本卡保留为后续条件资产。这是本轮来源选择，不是 GPU/AI 应用的负面实验结论。本席停止进一步搜源。

本席仅由Codex直接核源码与生成产物；未用DeepSeek或Claude，未修改生产代码/共享文档，未运行模型、RTL、完整构建或板测。
