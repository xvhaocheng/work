# stage107：实际大页／合并约束与 AI 恢复依赖审查

2026-10-12。root＋两个 Codex 席完成；新席为 gpt-6.1-sol／ultra，没有调用 DeepSeek 或 Claude。

**本轮完成约束审查和重设计卡，条件保留 KV 恢复依赖方向；没有登记新 G0，也没有进入 G1。** 整体仍处于第一阶段“问题与方法立题”，本轮是 stage106 普通 G0 后的约束复核与具体重设计关口，不是方法原型或实板验证阶段。

主实验平台保持 Genesys2／CVA6＋四 iDMA＋RISC-V IOMMU＋Linux。GPU／AI 只作场景优先；执行服务器用于构建、仿真和核源。stage106 的共同就绪连续冷准备分支仍结束，八案原始结果和私有工程修复不变。

## 1. 实际大页不能按旧 helper 的限制直接排除

现存 `out/linux/.config` 关闭 CMA，默认 `MAX_ORDER=10`。当前 helper 对整个槽使用 `DMA_ATTR_FORCE_CONTIGUOUS`；10MiB 需 order12／16MiB，超出该 generic buddy 路线的 order10／4MiB 上限。这个结论限定于现存配置及没有其他有效 coherent pool 的路径，不能替代正在运行的板端配置。源码入口：[分配链](../linux/drivers/iommu/dma-iommu.c:1446)、[order上限](../linux/include/linux/mmzone.h:28)、[拒绝超限](../linux/mm/page_alloc.c:4453)。

普通替代不是分配整块10MiB，而是每槽五个独立2MiB物理 extent，映射成连续且对齐的10MiB IOVA。四槽共二十个 order9 块、40MiB载荷 backing，不要求整槽或全部载荷物理连续。Linux 与实际 RTL 都有2MiB叶路径；PA与IOVA均需2MiB对齐，整叶及填充范围属于相应授权对象。CPU页仍可为4KiB。[Linux选页](../linux/drivers/iommu/iommu.c:2264)、[硬件大叶对齐](../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:487)。

静态见证保留 stage106 原十二个完整张量 descriptor、偏移及18,895,872B有效传输，检查二十叶、四槽、源目标无alias和全字节覆盖。四个大权重对象跨三个非连续物理块，仍可以保留整张量提交；覆盖分段不是软件必须拆 descriptor 的要求。所有PA为**未分配的示例地址**，RAM范围检查不证明Linux中空闲。原48MiB dense模型 backing原样保留；示例PA跨度78MiB含未预留空隙，不是新增78MiB存储预算。[布局与一次校验](../out/ai-platform-constraints-20261012/platform/layout-witness.md)。

`dma_map_sg`成功不保证返回IOVA为2MiB对齐，也不保证用了大叶；真实adapter要核PA、IOVA和叶大小，失败保留合法4KiB回退。二十次order9分配的成功率／碎片／时间没有实测。不能据总内存容量就保证成功，也不能以helper目前只映4KiB否决大页。

## 2. 合并、所有权与硬件预算的边界

- 生产四iDMA的DID为10—13，前端DID固定，没有逐copy换DID接口。单可信DID访问两包在实际所有者授权时是合法普通基线；不同设备的旧IOVA不能直接带到另一个domain。当前helper拒绝同DID配对只是实验接口限制，不是硬件禁止合并。
- 当前helper没有任意应用tensor导入接口。`dma_alloc_attrs`已经有DMA API映射，helper又建手工IOVA别名，源目标均READ|WRITE。旧模型的sourceRO／无额外别名不能当Linux实现或租户隔离证据；真正adapter须明确所有映射、权限和完整生命周期，不能凭空追加隔离要求来制造收益。
- 生产mode0每DMA buffer为3 beats；stage106私有两路为512。源码mode1可以给前两路512、运行NB关闭，但其编译资源仍存在，尚未核本轮实际bit／Image／时序。默认实例与私有模型不能称同资源板端验证。[生产实例](../cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1269)。
- CVA6没有DMA snoop。现helper明确通过FENCE.I完成CPU写排空与缓存失效；map成功和默认coherent配置不代替实际缓存交接。准备在性能窗之前也不等于零成本。[现有交接说明](../linux/drivers/misc/idma.c:2491)。

stage106写credit满8时的私有两处修复尚未合入生产源。后续真实准备若触发同一错误，先作为工程关单独处理；不能把未修实现停滞作为普通方法科学阴性或新方法收益。[普通适配合同](../out/ai-platform-constraints-20261012/critical/ordinary-preparation-contract.md)／[完整平台证据](../out/ai-platform-constraints-20261012/platform/constraint-evidence.md)。

## 3. AI来源增加了真实依赖，没有证明块级映射变化

新材料限于作者 [PagedAttention固定v1论文](https://arxiv.org/pdf/2309.06180v1) 和 [vLLM官方v0.1.0固定commit](https://github.com/vllm-project/vllm/tree/67d96c29fba9b72cb4c4edbc26211c208a00ebdd)。源码核两条必要链，共十四个提取文件；不把历史源码说成当前API。论文讨论的GPU KV块管理与主机IOMMU页必须区分。

固定源码一次分配每层K/V的GPU与pinned CPU整池。逻辑free只是编号和引用计数回收；没有每块unpin或IOMMU撤权。一个合法大页可以容纳多个逻辑槽及不同生命周期。因此“KV块回收频繁”不能推出“普通稳定映射／大页不适用”。[池分配](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/worker/cache_engine.py#L66-L100)、[逻辑回收](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/block_manager.py#L34-L46)。

多序列group在所选换出分支后有完整恢复义务：搬全部唯一KV槽，共享槽只搬一次；每层event在覆盖／使用前等待。同stream按层拷贝和事件已经实现安全依赖。精确`src_to_dst`在当前copy issue前可见，整池映射可以更早准备。安全能力、性能缺口和新颖性分别判断，不能把已有layer event改名为新方法。[发出链](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/worker/worker.py#L243-L285)、[使用屏障](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/model_executor/layers/attention.py#L130-L160)。

三项边界改变后续合同：同一期scheduler禁止同时swap-in和swap-out；单序列默认重算、多序列才默认swap；`can_append_slot`及`can_swap_in`保守预留每sequence一块，发生swap不自动证明物理容量不可避免。多GPU head分片可以是不同有用数据，但CUDA stream不是DID，来源没有证明两个实际请求器共享本平台翻译路径。[方向约束](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/scheduler.py#L31-L43)、[抢占选择](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/scheduler.py#L343-L365)、[预留判断](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/block_manager.py#L158-L166)。

## 4. 完整成本与接受范围

| 项目 | 本轮已核／推导 | 未测或必须验收 |
| --- | --- | --- |
| 旧四槽普通大页路线 | 20×2MiB＝40MiB；order9；20显式map，或经核验的4份SG映射 | 实际allocator／碎片／对齐／叶、失败回退与CPU时间 |
| 旧模型资源 | 原48MiB backing与八案保留；mode资源差异已核 | 正在运行的bit／Image／DTB／时钟／时序及实际资源 |
| KV完整工作量 | 去重N、L层、H heads、D head size、B tokens/block、e字节；一方向`2LNBHDe`B，原生`2LN`次memcpy | 实际有限N、语义有效token／尾槽、共享去重、两方向与COW成本 |
| 准备／映射 | 整池可以准备；PA/IOVA条件与缓存交接明确 | allocate/init/pin/PT/map/IOTINVAL/IOFENCE/FENCE.I及摊销寿命 |
| 发出／消费／清理 | 当前preissue信息和layer use依赖有源 | 软件供给、真实消费者、完整双方完成、drain/cache回收/unmap/free |
| 新控制 | 尚无规则、接口或资源接受 | buffer／metadata／端口／控制延迟／切换；全部计入净收益 |

尾槽未填满部分属于搬运和容量成本，不全记语义有效KV。没有实际计算、到达或期限就不填写这些周期；层双方搬运完成只表示数据就绪，不冒称attention或token完成。所有运行成本数值保持未测。[成本账表](../out/ai-platform-constraints-20261012/critical/cost-and-control-inventory.json)。

## 5. 重设计裁决与下一步

保留 [R107重设计卡](../out/ai-platform-constraints-20261012/critical/redesign-card.md)：真实有限KV恢复、同方向不同分片、两实际DMA、实际存活／消费依赖与当前issue前控制。**如果**允许合法大页、预映射、批量、驻留与授权合并并计完整成本后，仍存在可达、可避免的翻译相关有效路径等待，才登记最小G0；G0支持缺口后才设计最多两条规则。具体动作形成后核准确前作，不用想象的增强程序代替覆盖证据。

下一步只将这一卡落实为一份有限任务／所有权／生命周期合同和普通准备路径：明确原生来源与研究回放、真实资源与完整集合、消费者及自然结束、各阶段成本可测位置。有具体合同和合法控制点才做必要适配；缺失就归档，不加第三场景、不猜计算、不扩大压力，也不先实现新RTL。[下一goal合同](../out/ai-platform-constraints-20261012/next-goal-contract.json)。

本轮验收：平台27项源／配置hash和90段精确引文、AI十四文件／41处行号、root直接核24项、静态布局一次通过、49项旧输入未改。AI静态检查一次语法错误纠正后通过；root行号记录1—30超过14行的错误改为实际1—14，一次恢复通过，均记账。没有新DUT、编译、GPU推理、板连接、物理页分配或生产修改。这是源码／几何／合同验收，不是普通失效或方法收益验收。

[稳定root裁决](../out/ai-platform-constraints-20261012/root-result-acceptance.json)／[执行账本](../out/ai-platform-constraints-20261012/execution-ledger.json)／[直接核源](../out/ai-platform-constraints-20261012/critical/root-source-review.json)／[证据验收](../out/ai-platform-constraints-20261012/critical/root-evidence-acceptance.json)／[执行前五文档](../out/ai-platform-constraints-20261012/before/)。
