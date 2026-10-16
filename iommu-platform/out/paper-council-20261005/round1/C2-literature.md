# C2 本轮原始文献证据（2026-10-05）

用途：六席位可复用的定向证据；不是穷尽检索。下列行号是本次网页解析器行号，不是本地文件行号；PDF 同时给页码，页码从 1 起。只说已核对关键段落，不说通读所有论文。条目未验证的部分明确标注。

## L1：Fast & Safe IO Memory Protection（SOSP 2024）

- 作者原文：https://www.qizhecai.com/papers/fands.pdf
- 直接 PDF 可读，15 页。题目/作者/会议：PDF 第 1 页，解析 L0–32。
- 核对：第 2 页 L129–145；第 6 页 L714–746；第 4 页 L353–368。其方案利用 IOVA 局部性和页表缓存失效管理降低翻译成本，并已讨论共跑尾延迟；因此“失效影响别的流”和“严格保护下优化性能”不是空白。
- 限制：未复现作者结果；没有据此断言该论文证明或否定本提案的双时限契约。

## L2：Utilizing the IOMMU Scalably（USENIX ATC 2015）

- 作者原文：https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf
- 直接 PDF 可读，14 页。作者 Peleg、Morrison、Serebrin、Tsafrir。
- 核对：第 1 页 L10–31、第 9 页 §5 L608–612。已有 scalable IOVA assignment 与 deferred IOTLB invalidation；独立范围的失效不必共同争用一把全局 flush-queue 锁。
- 限制：该原文片段的 scalable invalidation 是 deferred 路线，不能直接说已经给出严格撤销完成时限；也不能把现成 per-domain/per-core batching 当全新贡献。

## L3：ShareMMU（MICRO 2026）——本轮从摘要证据升级为关键正文证据

- 作者机构页面：https://research.nvidia.com/publication/2026-09_sharemmu-supporting-secure-address-translation-sharing-among-untrusted
- 该页列出版日期 2026-09-15、MICRO，并提供 camera-ready PDF。
- 可读原文：https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf?VersionId=5BjALNe5CpmAmE5xIkXa6I31Ml7zg1ZO
- 18 页；核对第 2 页 L132–165、第 6 页 L500–536、第 7 页 L610–622、第 8–9 页 L730–790/Fig.11。
- 已覆盖：Permission Tracker 安全复用预翻译；targeted back-invalidation；权限变更的瞬态状态、in-flight drain 及避免循环等待的说明。因此新提案不能把权限版本、回失效或排空本身当创新。
- 仍未确立：有没有与本候选相同的独立流服务 deadline＋撤销 deadline 的联合定量规则。一次检索无命中不等于全文没有；应定向核对定量保证、评估与相关工作后才能判定差异。
- 注意：`find("revocation")` 无命中，但 `invalidation` 找到了实际撤销协议。**不能用关键词无命中宣称没有做过撤销。**

## L4：RISC-V IOMMU 命令与失效语义（规范，非算法论文）

- 固定版本原文：https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html
- 核对 §Page-Table cache invalidation L63–94 与 §Command-queue Fence L99–113。
- 精确范围可受 PSCID/GSCID/地址约束；简单实现可做更大范围失效。命令 CQ fetch 保序，执行可乱序；IOFENCE.C 的完成含义不能由局部优化削弱。PR/PW 与尚未被 IOMMU 处理的请求有不同处理边界。
- 推论（C2，不是规范原文结论）：局部失效速度不等于安全复用完成时间；新方法必须计入真实排空与提交前等待。规范语义本身并不提供所提双时限机制。

## L5：Beyond the Bermuda Triangle of Contention（RTCSA 2025）

- 作者原文：https://arxiv.org/html/2508.19670v1
- 核对 §V-A L524–526、§V-C L531–534、§V-D L535–538。
- 共享 IOMMU 干扰已表征；QoS 调节、page coloring/分区与预取等已讨论，缓解方案完整实测明确不在其范围。不能把“从普通 DMA 转到 IOMMU 上做限流”视为没有先例。
- 不把该论文讨论 SMMUv3 的文字冒充在本平台或其他未测体系上的实验证明。

## L6：HyperTRIO（ISCA 2020）——仍保留访问限制

- 尝试 https://par.nsf.gov/servlets/purl/10213947 与 https://parallel.princeton.edu/papers/ISCA20-hypertrio.pdf ，本轮两者直开 Internal Error。
- 继承本地已有有限证据：`docs/research-literature-review-20261004.md:26` 与 `docs/paper-route-five-round-review-20261004.md:92`。
- 不升级为全文已读；不能引用未核对页码。缓存分区/多在途翻译是强先例线索，精确与控制面方案重叠仍待全文核查。

## 待核对而非已证结论

- PREM/可预测执行模型与实时 DMA 预留是否已经覆盖 C2-T 的完整描述符准备—执行—撤销契约。
- CryptoMMU、Border Control 与 Intel Host Permission Table 的原始论文/规范；本轮从 ShareMMU 获取的是最近邻线索，未独立核对各自全文。
- 是否已有命令队列/撤销服务的联合实时准入规则；当前定向检索未确立缺口，不承诺无人做过。

本文件是 Codex C2 直接浏览和判断；未调用 DeepSeek 或 Claude，未进行论文实验复现。
