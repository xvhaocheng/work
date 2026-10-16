# stage107：vLLM KV 生命周期条件卡

**决定：保留一个有来源的条件工作流，不注册新 G0，不进入 G1。** CPU 换出、恢复与共享块存活有真实数据依赖；固定源码却没有“逻辑块回收必须改变 IOMMU 映射／权限”的约束。普通有序拷贝、每层事件、refcount 和稳定池已完成现有安全动作。是否仍有翻译相关性能缺口，需要实际平台、容量与完整完成证据，不能由 PagedAttention 的名称推出。

主实验平台继续为 Genesys2／CVA6＋四 iDMA＋RISC-V IOMMU＋Linux。这里仅核作者工作流；没有 GPU 推理、板卡、DUT、构建或 Linux adapter。

## 固定来源与边界

- P1：作者 [PagedAttention 论文 arXiv:2309.06180v1](https://arxiv.org/pdf/2309.06180v1)，2023-09-12，主要核 §4.2–4.6、§5.1、§7.3。
- P2：[vLLM 官方 v0.1.0 固定 commit](https://github.com/vllm-project/vllm/tree/67d96c29fba9b72cb4c4edbc26211c208a00ebdd)。仅闭合 allocation→worker→swap→layer/use，以及 scheduler→block manager→free/reuse。每文件 URL、SHA256、核查行号、提取／浏览次数见 [source-manifest.json](source-manifest.json)。
- FlexLLMGen 旧 KV 卡只作已知参考，没有重新调查。新增 primary 材料为两项：一篇固定版本作者论文和一个固定官方代码快照；浏览同一快照的多个必要文件不增加独立作品数，所有文件逐项登记。

## 真实需求与有用作业

调度器在没有足够空槽继续运行请求时抢占低优先级 sequence group（`scheduler.py:114–166`）。固定版本对单序列默认重算，对多序列 group（例如 beam search）默认换出，因为这个实现不支持多序列重算（`343–365`）。因此，**“发生 swap”只在所选执行分支成立，不是所有 AI 请求必做复制**；单序列重算、降低并发或更早完成已有请求仍是合法普通对照。

CPU 与 GPU 各有 block allocator。换出必须为全部未完成序列的唯一物理 KV 槽建立 GPU→CPU 映射；共享前缀只搬一次、维持引用计数。恢复建立 CPU→GPU 映射，并在每层写入新 KV、读取历史 KV 前等该层拷贝事件。该任务完成目标是保留并最终重新消费整个 group 的 KV，不能只搬一个前缀、一个 child 或若干早完成块就宣布 useful job 完成。来源：`block_manager.py:146–224`、`cache_engine.py:102–125`、`attention.py:130–160`；论文 §4.5 的 all-or-nothing 与 gang scheduling。

对任一真实 scheduler dictionary，令 `N=len(src_to_dst)` 为去重后的槽数，`L` 为本 worker 层数、`H` 为本 worker attention heads、`D` 为 head size、`B` 为 token/block、`e` 为 dtype 字节数：

- 一方向完整移动字节：`N × 2 × L × B × H × D × e`；源码发出 `2 × L × N` 个 `cudaMemcpyAsync`。来源：`get_cache_block_size`（`cache_engine.py:133–147`）与 `cache_kernels.cu:29–45`。
- 尾块也搬整个槽，未填满 token 的字节属于搬运／容量成本，不能全部记作语义 useful KV。语义有效量由每个唯一槽内实际存活 token 数决定；共享槽不得按 beam 数重复计数。
- 换出再恢复至少两倍该字节量，另计 GPU copy-on-write、后续 append、元数据、初始化与同步。没有下载或运行真实请求 trace，故本卡不填写捏造的 `N`、吞吐、ready 周期或必需交换次数。

## 分配、准备、存活与可控制位置

`CacheEngine.__init__` 一次分配全部 GPU 与 CPU cache；每层 K、V 各一张整池 tensor（`cache_engine.py:39–47,66–100`）。CPU allocation 使用 `pin_memory=True`。GPU／CPU 池之间复制，原生目的区不同于源区；但这不证明将它们映射到同一 FPGA DDR 后仍不可 alias，adapter 必须分别给出实际数据生产者、消费者和非 alias 义务。

`BlockAllocator.allocate/free` 只 pop、增减 refcount、回收编号（`block_manager.py:34–46`）。fork 共享槽；COW 分配新槽后复制；swap/free 回收的是槽元数据。整张 pinned tensor 仍存活，源码没有每 block 的 `munmap`、unpin 或 IOMMU 撤权。底层 swap 用 `data_ptr + block_number × block_bytes`（`cache_kernels.cu:29–45`），不是在 Linux 页表中 remap。**token block、torch tensor、GPU block table 与 CPU 物理页／IOMMU 映射不能互换概念。** pinned API 也不提供 CPU 实际物理页尺寸、碎片、THP／hugetlb 成败或 NVIDIA driver 的 DMA map 时点。

准备分为两种。backing 池、其覆盖的映射和容量在 cache 初始化后可提前准备，成本必须计入；本轮源码不能禁这种准备。精确 `src_to_dst` 对则取决于本期 free list、共享状态和 scheduler 选择，最晚在 `scheduler.schedule()` 返回、`Worker.execute_model()` 发出拷贝前已经可见（`llm_engine.py:219–230`、`worker.py:243–260`）。因而源级 policy 可在**当前 copy 发出前**选择合法批量、次序或准备；不能等当前 copy 的完成事件再声称加速当前 copy。

拷贝在同一个 cache stream 中按层、K/V 排队；每层拷完记录事件（`cache_engine.py:108–119`）。OPT 把该层事件交给 attention；attention 可先进行 prompt attention，再 `event.wait()`，之后才覆盖 KV 槽或做 decode attention（`opt.py:214–221`、`attention.py:119–160`）。这是按层的 use 屏障，不是必须等全部层恢复后才能开始任何计算。在有模型输出的正常 decode 路径，下一次 scheduler 是上一轮 worker 返回、CPU 更新输出之后；sampler 把 GPU 结果转为 Python 值（`sampler.py:280–300,314–357`）。这些调用顺序支持生命周期依赖，不提供具体 overlap 周期、copy engine 个数或截止时间。

管理器可在真实拷贝发出前把旧 GPU／CPU 槽编号加入 free list；同 cache stream 的次序以及 layer event 保护随后读写。不能把这个逻辑 free 误写成已完成物理传输、CPU unpin 或新的 DMA 授权。

## 容量与独立请求器

GPU block 数来自 `(total_gpu_memory × utilization - profiled_peak) // block_bytes`，CPU 数来自 `cpu_swap_space // block_bytes`（`worker.py:108–115`）；实际初始化采用各 worker 的最小可用数（`llm_engine.py:111–134`）。来源支持一个有限资源执行分支，**没有证明特定请求集在所有合法 residency／admission／调度策略下必须 swap**。`can_append_slot` 为每个 running sequence 保守要求一个空块（`block_manager.py:105–110`），即使尾槽尚可 append；`can_swap_in` 也额外预留每个 swapped sequence 一块（`158–166`）。仅达到这个启发式门槛不是物理容量不可避免性的证据。

单 worker 对应单 GPU（`worker.py:17–23`）。论文 §4.6 和官方配置支持多个真实 GPU tensor-parallel worker，每 worker 存不同 heads 的 KV 分片；同一 centralized manager 广播共同 block ID。这证明分片可以是不同有用数据，不证明某两者共享同一主机 IOMMU、具有不同 DID 或当前平台已配置对应真实 requester。CUDA streams 不是 DID。

**同一 scheduler output 明确不能同时 swap-in 与 swap-out**（`scheduler.py:42–43,145–166`）。因此不得在本版本创建“一 GPU 换入与换出原生同期竞争”的 G0。若条件卡将来选 tensor-parallel shard，则两真实 DMA 可以是同方向的不同 shard 搬运；这一 FPGA adapter、物理 DID、完成与消费者接口仍待单独验收，不是已经发生的原生 GPU 运行。

## 普通方法与分别裁决

| 项目 | 来源已完成／允许的动作 | 尚缺证据 |
| --- | --- | --- |
| 安全与存活 | refcount、共享前缀、COW；同 stream 顺序；每层 event 在 overwrite／use 前等待 | 没有 source 指定的 block 级 IOMMU 撤权、新安全缺口或权限代际问题 |
| 提前准备 | 稳定 backing 池可提前分配；映射准备与合法 residency 必须进入强对照 | 实际 allocator、pin／map、启动时间、物理页尺寸与 resident budget 未测 |
| 普通批量／合并 | 历史 host↔GPU backend 每 block memcpy；两端同时连续的 runs 可合并，scatter/batchedcopy 可作为完整普通对照；不可假设任意非连续 pair 都可零成本合并 | 本卡未实现改进 backend，也没有完整有限 trace 的控制／带宽损失；GPU COW fused copy 已是作者既有机制，不能冒充新 IOMMU 动作 |
| 大页 | 一个合法 2MiB backing 页可容纳多个逻辑槽；槽的各自 refcount／生命周期在 tensor metadata 中管理 | 没有证明原生 PyTorch pinned allocator 实际用了 2MiB；也没有证明必须 4KiB。大页路径是否可用及代价由实际平台证据裁决 |
| 新动作／性能 | source pair mapping 在当前 issue 前可见，具备插入源级控制的位置；真实 consumer layer/use 信息存在 | 无 adapter 接口、无普通失效、无剩余 PTW／HOL 归因、无新增机制接受、无净收益或 G1 |

## 正向保留的可检验前提与相邻反例

一句话条件主张：**如果**一个真实有限 KV 恢复执行中，两同方向、不同 head 分片由两个真实 iDMA／DID 搬运；相等总预算下 CPU/GPU 存活容量与已有消费者依赖确实限制 payload 提前搬运；允许稳定池映射准备、合法 2MiB、批量／合并和 residency 后仍有可核的翻译相关等待；而 `src_to_dst` 或 layer-use 信息能在当前 iDMA 首次 issue 前到达可执行控制点，**才保留“按本次 KV 恢复依赖改造翻译／发出控制”的重设计问题**。这是一个有条件的前提，不是当前已成立的创新或待运行矩阵。

source 支持的正例分支是：多序列 group 在 source 选定的抢占策略下被换出，后续必须完整恢复它的唯一共享 KV，并在该层 use 前完成；真实生成／释放时点比旧冷权重共同 ready0 更有依据。source 未提供上述两 DID、不可避免容量实例、物理页／映射和剩余翻译等待，故本轮止于条件卡。

相邻反例：单序列采用默认重算，根本没有 swap；可用驻留预算容纳全部 live KV，或普通 admission／整 group 串行完成已满足目标；稳定池被合法大页覆盖且映射可提前准备；普通 layer event／copy 次序已解决复用依赖且没有剩余翻译等待；或新信号只在本次 copy 完成后出现。任一反例都足以否定其对应的“必须复制／不能提前准备／新增动作能加速当前作业”前提，不能用人为禁用普通方法保留收益。

完整成本至少包含模型与 activation 留存、GPU／CPU 整池 backing、pin／map 与大页准备／失败回退、额外 staging、metadata／descriptor、共享/COW 复制、同步、cache ownership、两方向全部恢复 bytes、全部 group 完成、排空，以及新增控制本身。当前只有源码结构与字节公式，工程代价未测。

**本席给 root 的接受问题：**需求与每层安全依赖有来源；源级当前 preissue 控制位置有来源；共享 IOMMU 的独立 requester、普通方法实际失效、新信息产生何种新增可执行翻译动作及净收益均缺证。因为具体普通安全机制已覆盖现有 release/use，不将 GPU paged KV 内存管理能力作为共享 IOMMU 创新；也不据此否定主研究问题。root 直接核固定源后做最终裁决。
