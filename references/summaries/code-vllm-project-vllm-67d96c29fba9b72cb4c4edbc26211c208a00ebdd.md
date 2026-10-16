# vllm-project/vllm/67d96c29fba9b72cb4c4edbc26211c208a00ebdd〔正式题名待整理〕

稳定ID：`code-vllm-project-vllm-67d96c29fba9b72cb4c4edbc26211c208a00ebdd` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/block_manager.py#L34-L46)
- [来源2](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/block_manager.py#L158-L166)
- [来源3](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/scheduler.py#L31-L43)
- [来源4](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/scheduler.py#L343-L365)
- [来源5](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/model_executor/layers/attention.py#L130-L160)
- [来源6](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/worker/cache_engine.py#L66-L100)
- [来源7](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/worker/worker.py#L243-L285)
- [来源8](https://github.com/vllm-project/vllm/tree/67d96c29fba9b72cb4c4edbc26211c208a00ebdd)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/ai-platform-constraints-20261012.md:32](../../iommu-platform/docs/ai-platform-constraints-20261012.md)
- [iommu-platform/docs/ai-platform-constraints-20261012.md:36](../../iommu-platform/docs/ai-platform-constraints-20261012.md)
- [iommu-platform/docs/ai-platform-constraints-20261012.md:34](../../iommu-platform/docs/ai-platform-constraints-20261012.md)
- [iommu-platform/docs/ai-platform-constraints-20261012.md:30](../../iommu-platform/docs/ai-platform-constraints-20261012.md)
- [iommu-platform/docs/r107-bounded-decision-20261012.md:9](../../iommu-platform/docs/r107-bounded-decision-20261012.md)
- [iommu-platform/out/ai-platform-constraints-20261012/ai-block-source/constraint-card.md:10](../../iommu-platform/out/ai-platform-constraints-20261012/ai-block-source/constraint-card.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/ai-platform-constraints-20261012.md:36](../../iommu-platform/docs/ai-platform-constraints-20261012.md)：

三项边界改变后续合同：同一期scheduler禁止同时swap-in和swap-out；单序列默认重算、多序列才默认swap；`can_append_slot`及`can_swap_in`保守预留每sequence一块，发生swap不自动证明物理容量不可避免。多GPU head分片可以是不同有用数据，但CUDA stream不是DID，来源没有证明两个实际请求器共享本平台翻译路径。[方向约束](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/scheduler.py#L31-L43)、[抢占选择](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/scheduler.py#L343-L365)、[预留判断](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/block_manager.py#L158-L166)。

[iommu-platform/docs/r107-bounded-decision-20261012.md:9](../../iommu-platform/docs/r107-bounded-decision-20261012.md)：

来源为固定 [vLLM v0.1.0 commit](https://github.com/vllm-project/vllm/tree/67d96c29fba9b72cb4c4edbc26211c208a00ebdd)、[PagedAttention v1 原论文](https://arxiv.org/pdf/2309.06180v1)，以及唯一新增的 [官方 OPT-125m 固定配置](https://huggingface.co/facebook/opt-125m/resolve/27dcfa74d334bc871f3234de431e71c6eeba5dd6/config.json)。模型配置给出12层、hidden768、12 heads、fp16；D=64 和 TP2 后每 worker 6 heads按固定源码计算。TP2、各端8槽、两个序列的 token ID、checkpoint 和字节标签均为明确研究选择，未称原生请求 trace。

[iommu-platform/docs/ai-platform-constraints-20261012.md:34](../../iommu-platform/docs/ai-platform-constraints-20261012.md)：

多序列group在所选换出分支后有完整恢复义务：搬全部唯一KV槽，共享槽只搬一次；每层event在覆盖／使用前等待。同stream按层拷贝和事件已经实现安全依赖。精确`src_to_dst`在当前copy issue前可见，整池映射可以更早准备。安全能力、性能缺口和新颖性分别判断，不能把已有layer event改名为新方法。[发出链](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/worker/worker.py#L243-L285)、[使用屏障](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/model_executor/layers/attention.py#L130-L160)。

[iommu-platform/docs/ai-platform-constraints-20261012.md:32](../../iommu-platform/docs/ai-platform-constraints-20261012.md)：

固定源码一次分配每层K/V的GPU与pinned CPU整池。逻辑free只是编号和引用计数回收；没有每块unpin或IOMMU撤权。一个合法大页可以容纳多个逻辑槽及不同生命周期。因此“KV块回收频繁”不能推出“普通稳定映射／大页不适用”。[池分配](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/worker/cache_engine.py#L66-L100)、[逻辑回收](https://github.com/vllm-project/vllm/blob/67d96c29fba9b72cb4c4edbc26211c208a00ebdd/vllm/core/block_manager.py#L34-L46)。

[返回总目录](../README.md)
