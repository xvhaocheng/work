# C6 新增来源与阅读边界

检索/获取日期：2026-10-06。独立 R1 阶段，仅查原始论文、作者页面和官方文档。下列“待全文”不代表没有价值，也不构成不存在其他先行工作的证明。

1. **GPU Coroutines for Flexible Splitting and Scheduling of Rendering Tasks**，Zheng et al.，ACM TOG 43(6), article 281，SIGGRAPH Asia 2024，DOI 10.1145/3687766。
   - 作者全文：https://sites.cs.ucsb.edu/~lingqi/publications/paper_siga24gpucoroutines.pdf
   - 本地全文：`C6-gpu-coroutines2024.txt`。PDF 经作者站获取并提取为 UTF-8 文本，已去除 NUL；原 PDF 在 `/tmp/C6-GPUCoroutines.pdf`，不作为持久交付。
   - 本席重点直接读 §2.4、§3、§5、§6、§7 和 §8；没有逐条验证附录编译器算法，也没有执行其代码。
   - 核实内容：已有 continuation/状态帧、活跃状态分析、wavefront 和 persistent-thread 调度；寄存器、共享内存、全局访存和调度开销之间的取舍并非空白；自动选择挂起点也已列作后续方向。
   - 作者代码：https://github.com/LuisaGroup/LuisaCompute-coroutine （仅页面核对，未读实现）。

2. **Linux Buffer Sharing and Synchronization (dma-buf)**，官方文档。
   - https://docs.kernel.org/driver-api/dma-buf.html
   - 本地正文：`C6-linux-dmabuf-20261006.txt`，读取并保存当日 latest 页，未固定 Linux release。
   - 本席直接核对 “Indefinite DMA Fences” 和 “Recoverable Hardware Page Faults Implications”。相关本地位置 2917–3016 行。
   - 用途：证明内存分配、隐式 fence、设备执行资源的进展依赖及预留/抢占/分离方案早已讨论。它不证明 NVIDIA WIC 实现使用同一 fence 路径或存在死锁。

3. **Don’t Get Stuck in Traffic: A Case for Contention-Aware Translation Offloading / CATO**。
   - 作者页索引：https://skku-compasslab.github.io/compasslab/publications/260710-micro-cato/
   - 本次搜索返回作者页正文，标注 MICRO 2026；直接 open 同 URL 返回 404。**仅搜索索引的作者页面内容，未获论文全文，也未独立确认题录。**
   - 索引内容涉及：按内存层拥塞选择翻译路径、CXL 中末级 PTE 哈希副本、DRAM 权威页表回退。仅用于提示“CXL 翻译卸载”存在非常接近的先行风险，不采信其性能数字作本报告论据。
   - 下一步：确认作者页恢复/正式 MICRO 2026 论文目录及可用 PDF，阅读一致性和更新成本。

4. **ODRP: On-Demand Remote Paging with Programmable RDMA**，NSDI 2025。
   - 官方页：https://www.usenix.org/conference/nsdi25/presentation/wang-zixuan
   - 官方全文已通过网页工具打开：https://www.usenix.org/system/files/nsdi25-wang-zixuan.pdf
   - 本席只读摘要与简介层面的机制说明，**未做全文审查**。已足以否定“RNIC 动态远端内存管理无人讨论”的概括；不据此裁决更窄机制的新颖性。

5. **NP-RDMA: Using Commodity RDMA without Pinning Memory**。
   - 原始预印本页：https://arxiv.org/abs/2310.11062
   - 本席读摘要，未读全文/最终发表版本。摘要直接涉及 MMU-notifier、IOMMU/SMMU 和软件缺页处理，因此“RDMA 免 pin + IOMMU”不是可以直接认领的空白。

6. **MGG: Accelerating Graph Neural Networks with Fine-Grained Intra-Kernel Communication-Computation Pipelining on Multi-GPU Platforms**，OSDI 2023。
   - 官方页：https://www.usenix.org/conference/osdi23/presentation/wang-yuke
   - 全文入口：https://www.usenix.org/system/files/osdi23-wang-yuke.pdf
   - 本席核摘要，未读全文。证明多 GPU GNN 的细粒度通信计算流水有应用与强先行；不能证明本候选需要的“真实缺页与未就绪数据同时出现”普遍成立。

7. **T3: Transparent Tracking & Triggering for Fine-grained Overlap of Compute & Collectives**，2024。
   - 原始预印本：https://arxiv.org/abs/2401.16677
   - 本席核摘要，未读全文。已有生产进度跟踪、细粒度触发和计算/通信竞争缓解，不能把这些概念本身当创新。

8. **CUDA C++ Programming Guide 12.6**，官方归档。
   - https://docs.nvidia.com/cuda/archive/12.6.0/cuda-c-programming-guide/index.html
   - 本次搜索核到 hardware multithreading / resident-block 资源限制段落。它支持“可执行 warp、驻留块、寄存器是不同维度”，不直接说明任何 WIC 驱动版本在 fault 期间是否触发整个 context 的换出。

检索发现未纳入正式主张的其他条目包括 M5、Mitosis、TPage 和 DetShare；没有阅读它们的全文，也没有拿它们作完成新颖性审查的依据。
