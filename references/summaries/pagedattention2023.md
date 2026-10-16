# PagedAttention〔正式题名待整理〕

稳定ID：`pagedattention2023` · 类型：`research_paper` · 主题：`data-lifecycle`

作者：unknown；年份：2023；出处：SOSP 2023。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：既有广域地图的逐篇证据卡，问题范围以该记录为准。

机制／已存认识：把 KV cache 分成块，逻辑块→物理块表；块表翻译、按需分配、copy-on-write、引用计数、并行采样/beam search 共享：`:111-133, 327-409`。

支持边界：既有已存全文与逐篇笔记；本轮不重审评价。

限制：未复现论文实验；仅存全文不等于全文精读。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。

原始来源：

- [来源1](https://arxiv.org/pdf/2309.06180)
- [来源2](https://arxiv.org/pdf/2309.06180v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/pagedattention2023.pdf) · canonical：[原路径](../../iommu-platform/out/research-landscape-20261005/papers/pagedattention2023.pdf) · SHA-256 `55b3b324d779a67c59dac2519445e3b07c14e6ff5c656fadb47a3d7b5997469e`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/research-landscape-20261005/source-manifest.json:393](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/ai-platform-constraints-20261012.md:30](../../iommu-platform/docs/ai-platform-constraints-20261012.md)
- [iommu-platform/docs/r107-bounded-decision-20261012.md:9](../../iommu-platform/docs/r107-bounded-decision-20261012.md)
- [iommu-platform/docs/research-landscape-20261005.md:114](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/ai-platform-constraints-20261012/ai-block-source/constraint-card.md:9](../../iommu-platform/out/ai-platform-constraints-20261012/ai-block-source/constraint-card.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:112](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/innovation-council18-20261005/round1/A5.md:87](../../iommu-platform/out/innovation-council18-20261005/round1/A5.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/A5.md:87](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/A5.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/r107-bounded-decision-20261012.md:9](../../iommu-platform/docs/r107-bounded-decision-20261012.md)：

来源为固定 [vLLM v0.1.0 commit](https://github.com/vllm-project/vllm/tree/67d96c29fba9b72cb4c4edbc26211c208a00ebdd)、[PagedAttention v1 原论文](https://arxiv.org/pdf/2309.06180v1)，以及唯一新增的 [官方 OPT-125m 固定配置](https://huggingface.co/facebook/opt-125m/resolve/27dcfa74d334bc871f3234de431e71c6eeba5dd6/config.json)。模型配置给出12层、hidden768、12 heads、fp16；D=64 和 TP2 后每 worker 6 heads按固定源码计算。TP2、各端8槽、两个序列的 token ID、checkpoint 和字节标签均为明确研究选择，未称原生请求 trace。

[iommu-platform/docs/ai-platform-constraints-20261012.md:30](../../iommu-platform/docs/ai-platform-constraints-20261012.md)：

新材料限于作者 [PagedAttention固定v1论文](https://arxiv.org/pdf/2309.06180v1) 和 [vLLM官方v0.1.0固定commit](https://github.com/vllm-project/vllm/tree/67d96c29fba9b72cb4c4edbc26211c208a00ebdd)。源码核两条必要链，共十四个提取文件；不把历史源码说成当前API。论文讨论的GPU KV块管理与主机IOMMU页必须区分。

[iommu-platform/docs/research-landscape-20261005.md:114](../../iommu-platform/docs/research-landscape-20261005.md)：

| [PagedAttention](https://arxiv.org/pdf/2309.06180) | SOSP 2023 | KV cache 分块、块表、共享与 CoW | A100 实测；2–4× 不适用所有 GPU 负载，DNN 训练可能退化 |

[iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:112](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)：

| [PagedAttention](https://arxiv.org/pdf/2309.06180) | SOSP 2023 | KV cache 分块、块表、共享与 CoW | A100 实测；2–4× 不适用所有 GPU 负载，DNN 训练可能退化 |

[返回总目录](../README.md)
