# FlexLLMGen 来源到 FPGA 的有限权重准备合同

结论：可以忠实提取一个 **完整 MLP 权重包的有限准备阶段**，以板上两个真实 iDMA 执行 `DRAM文件镜像 → 中转缓冲 → DRAM目标` 的功能投影。所选包完整包含源 OPT-125m 单层 MLP 的六个张量，共 **9,447,936 字节**。该任务不需要计算耗时、deadline 或后台长流。它支持完成、数据守恒、缓冲所有权和共享 IOMMU 搬运机制的验证；它不提供真实 NVMe/GPU 端点，也不提供模型推理或 token 性能证据。root 已冻结此包作为 static/functional projection。

机器可读合同见 [source-task-contract.json](source-task-contract.json)。本报告与 JSON 均为 Codex 直接核源产物；未调用 DeepSeek/Claude，未修改生产代码或测试，未执行性能实验。

## 1. 原始任务与准确来源

论文是 ICML 2023 的 FlexGen；实现是固定提交 `004ffef82b46e8dc8685c55d0cdda650bdaf1269` 的 FlexLLMGen。两者身份分开，不声称 2024 源码就是论文原始评测工件。旧材料和 SHA 见 `../../gpu-ai-deployment-gate-20261012/ai_workflow/source-manifest.json`；本轮增加的三份完整官方源文件、URL、提交、SHA 和获取时间见 [supplemental-source-manifest.json](supplemental-source-manifest.json)。没有模型数值权重或 tokenizer 工件。

以下 `flex_opt.py`、`pytorch_backend.py`、`sheng23a.txt` 行号指向旧目录 `../../gpu-ai-deployment-gate-20261012/ai_workflow/sources/`；`opt_config.py`、`utils.py`、`timer.py` 指向本目录 `sources/`。

论文 Algorithm 1 在每个批次后同步全部逻辑工作，前面的权重预取、缓存/激活搬入搬出和当前计算具有明确跨批次依赖（`sheng23a.txt:329–370`，论文物理第 5–6 页）。源码 `generation_loop_overlap_multi_batch` 同样每次内循环 `self.sync()`（`flex_opt.py:1034–1054`）；`sync` 先等磁盘拷贝队列，再全 CUDA 同步（`793–795`）。因此源有有限屏障，不能加入无来源的后台长流或假设无限计算空档。

选取 `MLP.load_weight(k=0)`：源把一个 MLP 的六个完整张量搬到 `weight_read_buf`，之后跨多个微批次复用，直到最后批次才捐出/释放（`flex_opt.py:477–505,519–533`）。源全磁盘权重放置可由 `w_gpu_percent=0,w_cpu_percent=0` 指定；`init_weight_list` 将各张量放到 disk，`pin_memory` 参数不把 disk 张量变成 GPU/CPU 驻留（`92–117`）。默认 CLI 是 OPT-6.7B、全 GPU、一个微批次（`1276–1305`），默认本身并不触发这个磁盘准备阶段。

`OptConfig` 默认 FP16（`opt_config.py:31`）；受支持 OPT-125m 明确 `h=input_dim=768,n_head=12`（`65–69`）。本轮选择这个真实支持的小模型配置的一个完整准备包，不改 hidden size。`dataclasses.replace` 确实允许 kwargs 改配置（`125`），但不会取得匹配数值权重；任意缩 h 后不能声称仍是原 OPT-125m。

| 张量 | 源形状 | FP16 字节 |
| --- | --- | ---: |
| `fc1.weight` | `(4h,h)` | 4,718,592 |
| `fc1.bias` | `(4h,)` | 6,144 |
| `fc2.weight` | `(h,4h)` | 4,718,592 |
| `fc2.bias` | `(h,)` | 1,536 |
| `final_layer_norm.weight` | `(h,)` | 1,536 |
| `final_layer_norm.bias` | `(h,)` | 1,536 |
| 完整包 | `2*(8h²+7h)` | **9,447,936** |

形状和加载顺序直接来自 `flex_opt.py:477–505`。完整包不等于整个 transformer 层，更不等于完整模型；完整包包含所选 MLP 的全部六份权重，没有以 slice 冒称整层。

## 2. 原实现的两跳、ready/done 与寿命

源磁盘路径不是直接提交 NVMe 队列。`general_copy` 向 Python worker 队列提交任务（`pytorch_backend.py:831–836`）；worker 把文件 `open_memmap` 映为 CPU 张量（`866–869`），执行两次 copy（`878–906`）：

```text
完整文件权重已存在 / OS已能提供所触及的页面
       ↓（真正存储读取可能在此，也可能是文件缓存命中）
映射的CPU源数据 → CPU拷贝填入pinned relay → CUDA拷贝到目标
                                                   ↓
                           磁盘队列join + CUDA全同步 → 权重可供消费
```

第一跳 `tmp_cpu_buf.copy_(src_data)` 是 CPU 对文件映射的拷贝（`901`）；第二跳 `dst_data.copy_(tmp_cpu_buf)` 是 relay 到 CUDA 目标的拷贝（`902`）。源数据由 OS 页缓存/缺页供给，源码没有暴露每次 NVMe I/O 的 submitted/ready/done。`open_memmap` 返回或 `queue.put_nowait` 都不代表数据准备完成。`queue.task_done()`（`906`）也不能单独冒充 CUDA DMA 完成；原合同由 queue join 后的 CUDA 全同步收口。

默认四个 worker（`pytorch_backend.py:624,641–646`），每个 relay 是 `(1*GB,)` 个 FP16 元素（`882`）；`utils.py:15–17` 的 `GB=2^30`，所以 **每 worker 2 GiB，四 worker 共 8 GiB**。这个实际源内存成本不能藏起来，也不能原样投射 FPGA DDR。对本轮已知有限包，源码只访问 `cpu_buf[:size]`（`899–900`），可以明确适配为按任务最大张量或整包预算分配；这是容量适配，不是未改源程序的直接运行。任何减少 worker 数的并行度变化也必须计入。

源 weight home 从初始化留到删除；目标权重在所有微批次消费后释放。本轮没有 compute 消费，目标寿命截取为“最后搬运实际完成后进行全字节校验，再 release”。relay 至少留到第二跳实际完成；不能在 B 提交时抢先复用。原 worker 同一 copy stream 中维持操作顺序；板端跨两个物理 DMA 必须由真实完成与平台要求的可见性屏障显式建立这个关系。

## 3. FPGA 可执行投影与普通基线

DMA_A 和 DMA_B 必须绑定现有平台两个真实 requester；具体 DID 由 root 平台审查绑定。它们是研究中的角色投影。原 source 是同一个 GPU 的 CUDA 软件操作，不会因为有两条 stream 就产生两个物理 DID；本合同禁止那种映射。

对每个张量 `t`，合同是：

```text
home_ready → A_t（home→relay）→ A_t实际done/可见性
                                      ↓
                               B_t（relay→target）→ B_t实际done
六个B_t全部done → package_ready → 全字节校验 → release
```

每个完整张量可按实际 DMA 描述符限制分成连续范围；每块保留 `A_done → B_start`，所有块覆盖原张量且互不遗漏/重复，整张量在全部块完成后 ready。源码 MLP 加载时没有切块，切块是明确适配，并增加描述符、提交、等待和 fence 成本。尺寸首先取上表；无硬件限制无需强切 4 KiB，也不能靠人为小 slot 制造阻塞。

统一 relay 预算采用 **整包 9,447,936 B**，可按独占 offset 安排全部张量/块。home、relay、target 合计 **28,343,808 B**，另加页表、描述符、缓存/一致性维护、校验、Linux 和应用占用。真实可分配预算及 cache 规则仍由平台审查验。所有对照可合法分区和回收同样的 relay，不限制为固定两槽。

可执行普通对照包括：

- **简单串行**：每张量先 A、等完成，再 B、等完成；最大临时 relay 只需 4,718,592 B，剩余同预算空间仍可使用。
- **完整阶段提前准备**：A 将六个张量全部搬入整包 relay，等所有 A 完成，再让 B 搬全部张量。这是合法的完整有限阶段预准备，不需要 deadline。
- **普通共享 relay 流水**：同预算 allocator 和依赖队列，在实际 DMA 前端容量允许时，让 A 写尚未占用的另一段与 B 读已准备段并行。不强制 A/B 整体轮流，不凭新机制独占普通控制也能使用的能力。
- **CPU/DRAM resident 一跳**：在 FPGA 的 DRAM 代理映射中，若 home 可以被目标普通 DMA 直接读取，应允许直接 `home→target`。中转的物理必要性已被代理删除；因此两跳相对这一跳的收益不代表真实 offload 收益。映射持久化、合法翻译预热、整张量搬运同样允许；成本要分别计入。

选用源 `generation_loop_normal` 的 benchmark 数字作为完成基线也不稳妥：该循环没有逐批次显式 disk drain（`flex_opt.py:912–927`），计时器默认无同步（`timer.py:20–39`）。相反 `generation_loop_debug_normal` 在操作边界显式 `self.sync`（`flex_opt.py:962–978`）。板端基线应按实际硬件完成收口，不能只复刻函数调用/计时停止。

## 4. 可主张的边界与决定问题

若两 DMA 的物理身份、共同 IOMMU 路径、内存可见性、完整包字节及实际完成全部验证成立，可以说“现有 FPGA 真实执行了来源驱动的有限两阶段权重准备搬运合同，并检验其翻译和缓冲生命周期”。在这个明确的内存代理合同内，可以研究翻译干扰；但本轮尚无普通方法具体失效，**不能据此登记 G0，更没有新方法性能结论**。

要进一步称“实际 AI 权重准备”，须取得固定模型 revision 下所选层真实六份数值张量并校验；当前只有公式/源码，没有这些数值。确定性 fixture 足够验证 DMA 字节守恒和完成语义，必须标明 fixture。要称“NVMe offload”或“GPU性能”，则须真实对应协议、端点和完整准备/消费成本，板上两个 iDMA 名称不能替代它们。若要恢复计算重叠/应用吞吐主张，须增加实际消费工作及其测量，不能猜 delay。

唯一范围决定已由 root 的冻结指令解决：接受 **有限权重准备阶段的 DRAM 代理与功能/翻译主张**。余下是平台事实验收，不再需要另选 AI 场景或假设计算时间。

本席实际只做来源核查、三份补充源获取、JSON/尺寸/来源哈希校验；未做仿真、板测或性能跑，未改公共状态文档。
