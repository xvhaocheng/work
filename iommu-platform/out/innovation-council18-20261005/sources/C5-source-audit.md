# C5 新增来源审计（2026-10-06）

独立 R1 定向查新记录。这里只保留来源与实际阅读边界，不复制网页全文；没有执行代码或验证作者实验。四篇指定原文的完整阅读记录见 round1/C5.md §2。

1. SUV，MICRO 2024，https://www.csa.iisc.ac.in/~arkapravab/papers/MICRO24_SUV.pdf 。原文 PDF 文本浏览了摘要/引言的静态密度、线程/块工作集、阶段插入迁移与不规则指针访问的选择性运行时管理，以及编译器实现的少量段落；未全文。需要补查策略详细成本及开源代码。不能认领“静态语义+动态 UVM”为新点。

2. Forest: Access-aware GPU UVM Management，ISCA 2025，https://guilhermecox.github.io/dw/lin-isca25.pdf 。浏览摘要/引言、§3.3、§4.1–4.2、§4.3部分、§4.5–4.7、§5前部；未全文。阅读涉及 Access Time Tracker、对象/页访问时间、周期读取、已识别模式停止监测、SpecForest 的模式复用；未审作者代码。

3. Towards Fully-fledged GPU Multitasking via Proactive Memory Scheduling（MSched），https://arxiv.org/abs/2512.24637 与 https://arxiv.org/html/2512.24637v2 。v1为2025-12-31，v2为2026-01-02。实际读摘要/引言、§4–6相关机制和§7开头；未全文。内核参数模板、离线剖析、调度时间线、按预测顺序提前迁移均为已有思想。其“几乎全覆盖”是特定基准报告，不自动覆盖全部数据依赖图应用。会议身份及实现可用性未核。

4. Elastic KV Cache，https://arxiv.org/abs/2608.23658 与 https://arxiv.org/html/2608.23658v1 。2026-08-24预印本。读摘要/引言、部分控制实现、实验和限制相关段；未全文。讨论同虚拟地址下交换物理内存用途，并报告简单调整 prefill 分块的强结果。只在作者报告配置成立；不据此宣称所有动态 KV 机制都没有价值。

5. VAMP: Dynamic HBM Repartitioning for Multi-Turn MoE Serving，https://arxiv.org/abs/2609.13537 与 https://arxiv.org/html/2609.13537v1 。2026-09-11预印本。读摘要/引言和部分评估；未全文。涉及专家参数、KV和前缀的空间成本权衡；作者同时报告不同延迟、完成率等取舍。仅作动态重分区的重叠提醒。

6. Can a GPU Runtime Place Memory Well With Only Page Faults?，https://eunomia.dev/research/gpu-memory-placement-evidence/ 。读“what is missing”、证据记录、年龄/置信度、生命周期、成本与实验提案相关段。公开研究笔记，非经评审论文，页面内容未做历史版本核验。它可以证明作者已公开提出相关研究抽象，不能证明该抽象已实现或取得性能收益。

7. Asymmetric Virtual Memory Paging for Hybrid Mamba-Transformer Inference，https://arxiv.org/abs/2605.22416 。仅搜索返回的题名与摘要片段，未打开全文，不引用其具体机制或结果。

待补优先级：ObservUVM 作者代码及事件生命周期 > SUV/Forest/MSched 全文和可运行强基线 > ObservUVM 引用的 HELM、Ariadne、DeepUM 等邻近机制。未做系统穷举查新，不将本轮来源列表当创新证明。
