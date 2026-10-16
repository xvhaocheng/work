# C4 补充来源与访问边界

日期：2026-10-06。C4 独立检索；未读取其他席提案。以下为阅读定位记录，不是论文全文替代品。浏览器取得了列明的原始网页/PDF；本地命令下载三个 PDF 因网络权限失败，未得到本地 PDF，未为此请求提升权限。本文为 UTF-8 文本，不含 NUL。

## 已查原始来源

1. **Optimizing Indirect Memory References with milk**, PACT 2016。
   - 原始 PDF：https://commit.csail.mit.edu/papers/2016/kiriansky16pact-milk.pdf
   - 作者项目索引：https://milk-lang.github.io/publications.html
   - 本轮实际检查摘要、§1、§3.1–3.2、§5.2.1–5.2.2、§7.1、§8 的相关段落；不是本轮完整逐页复审。
   - §3.1：collection/distribution/delivery 可协程融合，临时空间耗尽可提前 delivery；不能说软件 inspector-executor 必须完整物化所有中间结果或一定要多轮才能摊销。
   - §5.2：临时容器受 TLB reach、cache 容量约束；DRAM bank 也是排布因素；默认 LIFO、需要串行语义时 FIFO。因此“按页分组＋跨线程重排＋返回次序契约”本身不足以创新。
   - §7.1：实测平台/工作集条件下，大页和缓存中的 PTE 已降低翻译成本。不能从其结果推出小页场景必慢，也不能说它完全忽略翻译。

2. **Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine**, arXiv:1808.09751，2018。
   - 原始 PDF：https://arxiv.org/pdf/1808.09751
   - 原始摘要：https://arxiv.org/abs/1808.09751
   - 本轮检查摘要、§I/II、§IV-C、§V-A/相关结果、§VI；不是完整全文复审。
   - §IV-C：DMA 保存 burst 的地址、长度、ID、状态等回放元数据，重试未成功 burst；按虚拟页合并故障报告。发生失败后该 DMA 停止发新 burst，待在途结束再重放。
   - §VI：避免整个 DMA 因 TLB miss 停止并保持顺序保证，被列为未来工作。这只说明该 2018 版本未完成，不能证明 2026 仍无人解决。
   - §V-A：Zynq-7045 平台，不是本项目 CVA6 平台。

3. **Asynchronous Memory Access Chaining**, PVLDB 9(4)，2015 年卷期 / VLDB 2016。
   - 原始 PDF：https://www.vldb.org/pvldb/vol9/p252-kocberber.pdf
   - 本轮检查摘要、§1–§3.1。明确按 lookup 保存独立状态，在可变链长、早停和读写依赖下保持链间并行。不能用同步逐条 chase 或固定锁步 batch 作为唯一 CPU 对手。

4. **AMReX 官方 GPU 文档**，本轮读到页面版本 26.11-dev。
   - https://amrex-codes.github.io/amrex/docs_html/GPU.html
   - https://github.com/AMReX-Codes/AMReX-Codes.github.io/blob/main/amrex/docs_html/GPU.html
   - 实际检查 Memory Arenas、Elixir、Async Arena、最佳实践相关段落。文档有内存池、长驻数据、stream-ordered allocator；不能从 regrid/短命 C++ 对象推出持续 OS 页映射 churn 或物理碎片。
   - 当前网页会变化，后续实验应锁定实际应用提交与配置；本轮未克隆或运行 AMReX。

5. **CXL-ANNS: Software-Hardware Collaborative Memory Disaggregation and Computation for Billion-Scale Approximate Nearest Neighbor Search**, USENIX ATC 2023。
   - 原始作者/会议摘要：https://www.usenix.org/conference/atc23/presentation/jang
   - 本轮仅查官方摘要：包含邻居缓存、预取、放宽查询依赖与并行搜索。没有全文审查，不能据此断言覆盖所有动态 ANN 场景；足以阻止把“近内存图搜索＋推测”本身当全新方向。

## 只找到或未能取得全文的待核项

- **Terminus: A Programmable Accelerator for Read and Update Operations on Sparse Data Structures**, Hyun Ryong Lee、Daniel Sanchez，MICRO 2024。作者列表确认：https://people.csail.mit.edu/sanchez/ 。本轮尝试的推测 PDF 路径不可访问，作者页该项没有 PDF 链接；没有读到全文。DX100 §7/参考文献 65 直接讨论其递归结构的 fine-grained update。应优先取得全文以检验任何“支持更新/指针结构”的新意。
- **ANSMET: Approximate Nearest Neighbor Search with Near-Memory Processing and Hybrid Early Termination**, ISCA 2025。作者 PDF：https://nas.iiis.tsinghua.edu.cn/~gaomy/pubs/ansmet.isca25.pdf 。本轮检索得到原 PDF 索引段落，打开超时；不声称全文已读。
- 其他精准关键词：`translation-aware memory access reordering accelerator`、`TLB-aware radix partitioning`、`consumer completion sparse gather scheduling`、`coflow scheduling shared data reuse`、`Phloem fine-grain pipeline parallelism`、`MAPLE tiny but mighty latency tolerance`。本轮检索不构成穷尽查新。

## 本地四篇必读原文的重要核查

- DAE：`out/research-landscape-20261005/papers/dae1982.txt`，§2/§3/§8；已有访存与计算解耦、store/load 依赖和队列死锁讨论。
- IMPICA：`out/research-landscape-20261005/papers/impica2016.txt`，§4/§5.2/§5.3；region 分配、OS 维护页表/TLB一致性；其评估不包含 CPU 同时修改被 offload 的结构。
- DX100：`out/research-landscape-20261005/papers/dx1002025.txt`，§3.5/§3.6/§4/§6/§7；大页、256-entry TLB、应用生存期一次预装 PTE；内部逐元素 finish bit 与 CPU 侧 tile-ready 等待须分开。
- Tiara：`out/research-landscape-20261005/papers/tiara2026.txt`，§4.1，363–378 行；主要 Tiara latency 为 Verilator 模型，throughput 由 latency/并发数推导，主要非 Tiara 对手为 analytical model；BF-2 图为实测，FPGA 单 MP 有 post-route timing。不能把整组主要性能结果写为 FPGA 实板实测。
