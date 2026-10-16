# R107 有限恢复合同（stage108，核验前冻结）

结论：固定 vLLM v0.1.0 足以给出有限槽、所有权、字节和层依赖合同；这里落实的是**完整 OPT-125m KV 形状的有限研究回放**，不是原生 GPU 推理、真实请求 trace 或已运行 FPGA 适配。是否存在普通方法未覆盖的翻译等待，由 root 裁决。

唯一新增来源是官方 [`facebook/opt-125m` 配置](https://huggingface.co/facebook/opt-125m/resolve/27dcfa74d334bc871f3234de431e71c6eeba5dd6/config.json)：12 层、hidden 768、12 heads、float16（本地 `source/opt-125m-config.json:14,19,20,23`）。采用完整 12 层；不是从模型删层。vLLM `config.py:71-81` 给出 D=768/12=64、TP2 时每 worker 6 heads；`opt.py:65-79,290-302` 与论文 §4.6 支持两个 worker 存不同 head 子集。TP2 是研究选择，不主张 OPT-125m 需要两 GPU。

回放参数逐项登记在 `finite-contract.json:parameter_provenance`。B=16 复用论文明确默认（原文 txt:949-952）。CPU/GPU 各 8 槽/worker 是研究容量选择；实际 live 仅 3 槽，恢复后仍有 5 空槽，满足源码 3+2 的保守预留，不人为制造容量压力。只截取一个已选择 swap 分支的 group 恢复；没有构造让 `_schedule()` 自动触发 preemption 的负载，不能声称交换不可避免。

两个序列共享完整 16-token prefix，各有一个不同 1-token 尾槽，长度各 17；有效唯一缓存位置为 18，不能把共享 prefix 按两序列重复算成 34。序列 ID、token ID 和 KV 字节是研究代理生成，未由 tokenizer/LLM 产生。这是合成、静止的 cached-prefix checkpoint；源函数验证元数据转换，不证明某次原生应用执行到过该 checkpoint。

槽分配按源码 LIFO 和引用计数冻结：原 GPU 表 A=[7,6]、B=[7,5]；swapout 去重为 GPU7→CPU7、6→6、5→5；swapin 去重为 CPU7→GPU5、6→7、5→6。共享 prefix 引用数2，尾槽各1。完整 prefix 后 append 进入新块分支，所以本实例 COW=0；没有删掉隐藏 COW 成本。`block_manager.py:89-144,146-224`、`sequence.py:75-125` 支持这些状态转换。

每 worker 每层 K=[6,8,16,8]、V=[6,64,16]，每 component 槽12,288B。全层恢复为 `2 workers ×12 layers ×2 K/V ×3 unique slots`=144 完整 copy，1,769,472B；其中本研究 checkpoint 的代理有效标记字节663,552B，尾槽 padding1,105,920B。先前完整 swapout 另计1,769,472B，所以两方向至少3,538,944B。有限全字节回放不能用单层、单头、单 child 或 early prefix 代替整组完成。槽字节来自 `cache_engine.py:49-64,133-147` 与 `cache_kernels.cu:29-45`，尾部按完整槽搬运。

**native 有效字节未验收：**`cached_valid_tokens=17` 是 CPU 代理选定的静止 checkpoint 值，`Sequence.get_len()` 只证明逻辑 token 数。`worker.py:190-210` 在源 decode 路径把最后一个 token 送入本次 forward、位置为 `get_len()-1`；`attention.py:130-147` 先等恢复 event，再通过 `reshape_and_cache` 覆盖该 token 的 KV，然后 `149-160` 才做 decode attention。因此实际 scheduler-entry 的缓存有效长度可能小于逻辑长度，恢复尾槽的部分数据可能先被覆盖；663,552B **不是 native attention 必需读取字节**。本次全槽复制数是既有 backend 的物理工作量和代理合同，不证明语义最小必要搬运量。真实 phase 边界、valid 标记和覆盖规则仍需 native 证据；不能从代理的全字节消费者要求推出翻译残余损失。

拟 FPGA 适配有四个互不 alias 的 DDR 池：CPU_HOME_0、GPU_RESIDENT_0、CPU_HOME_1、GPU_RESIDENT_1。每池完整 backing 2,359,296B，四池9,437,184B；layer/组件按冻结偏移打包，是适配布局而非原生 torch 物理布局。地址写成真实分配后 IOVA_BASE+offset，目前没有捏造 PA/IOVA。DID10/11 分别拟搬两分片，由平台席确认真实 requester 与映射。一个可信进程拥有全部池，可以授权单 DMA 合并两分片，不制造租户隔离。大页、预映射、普通批量和合法驻留全部保留；5→6、6→7可合成连续24,576B run，7→5独立12,288B，普通 coalescing 可把144项变为96项，全部字节不变。

CPU 生产者代理写有限非零 fp16 标签；不同 worker 的符号位不同，因此分片是不同数据。K/V 分别遵循来源布局；有效位置与 padding 分开。所有 full-slot padding 也复制、核验，但不记作语义 useful KV。CPU 消费代理逐字节核整个恢复集合、padding、源保持和未分配槽 guard，不执行 attention/all-reduce。自然终止是有限集合全部恢复/核验且排空，没有造算时、到达、deadline 或 EOS。初始化、验证读取、此前 swapout 和潜在缓存交接全部单列成本；不把 CPU 字节检查时间冒充 DMA 或模型时延。

来源安全动作已经包含共享去重/refcount、同 cache stream 的完整 copy 顺序、按层 event；`attention.py:130-160` 在覆盖 KV/decode use 前等待，`opt.py:214-221` 按层传递事件。每分片每层6个完整copy后消费；两分片共同层就绪仅表示数据齐，不等于 attention 开始或 token 完成。元数据 `free` 可能早于物理 copy (`block_manager.py:185-186,216-217`)；不能把它当 DMA 完成、unpin、撤映射。当前有限研究检查没有任何硬件完成事件。

整池布局/容量在 setup 可见，精确恢复字典在 `llm_engine.py:219-230`、`worker.py:243-260` 发出当前copy之前可见。现有 GPU 事件保护的是 use 安全，不提供 FPGA adapter 或残余翻译损失。冻结后只做一次短的源函数/CPU字节核验，保留实际命令、完整原文/哈希、展开job清单和验收记录；不跑旧八案，不写 Linux/RTL adapter，不构建。

需要 root/平台席确认的接口为实际非 alias backing/叶、DID与共享翻译路径、issue前字典接入、缓存所有权、真实NEXT/DONE与drain及完整准备成本。来源不支持 capacity-forced swap、同一期双向swap或跨GPU共同主机IOMMU的结论；`can_append_slot`/`can_swap_in` 是保守启发式，不是物理不可能性证明。合同本身不注册 G0、不选择调度创新。

冻结后唯一核验已经通过，实际命令为 `python3 out/r107-bounded-decision-20261012/instance/check_finite_contract.py`（工作目录为平台）。原源码类的槽表/refcount、K/V shape 和 CPU full-slot 字节检查通过；144 jobs、24 worker-layer、source 保留与所有空槽 guard 完整核验。结果见 `acceptance.json`、展开清单 `expanded-restore-jobs.json`、6份原始完整池数据 `raw/` 和 `raw-manifest.json`。只复用 stage106 的 `Rejection/require/sha`，旧 shape/event 函数没有执行；没有硬件完成或时延验收。
