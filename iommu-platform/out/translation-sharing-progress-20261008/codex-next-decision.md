# §46 根级接受与下一判别

2026-10-08；root Codex。源码/原文/纸面构造审定完成，没有新数学或 DUT 运行。DeepSeek整理证据及文档草案；Codex亲自设计、核关键来源、保存并审定。两席已完成关键质疑，无 Claude。

## 已停止的分支

1. 现有 IOTLB 与 PTW 状态分开，没有假设的统一保存槽竞争。不能把另建统一池后的压力叫作既有板端瓶颈。
2. 共享 PTE 结果与每请求 private Info、有限 MSHR、PTW reservation 有 NB-Walker 固定作者源码的具体普通实现；Barr 的 partial/path cache 也须作为对照。把它们简单组合不能称独立创新。
3. 合法 cross-DID alias 确实区分 resident-way 历史与 requester 历史。它不是旧 test 或 RTL 违背旧合同：既有 S3 明确允许 A-mask 下的 V-way touch。
4. 真实 hit 来源 sideband 与 requester-local 历史过滤是普通工程方案；2008 US7415575B1 已给共享 hit/client-specific replacement。它不能单独作为论文新机制。main fill mask 不能代替实际 lookup 来源。

## 下一单一判别

保持多 DMA 共享 IOMMU 的整体性能主线，不自动增加请求来源绝对不干扰的硬合同。先按同一合法共享地址空间、相同总状态和相同有效信息，比较：

- 当前 resident-way local history；
- 普通 true-requester history filtering（normal 与 fast 的实际接受来源）；
- 普通共享替换及允许的容量/准入控制。

要回答：在合法跨 DMA 复用中，是否存在这些强普通方法达不到、而候选规则能达到的 V/A 整体吞吐或明确延迟—成本点？硬 requester 隔离只是一种可选合同，不能代替性能贡献，也不能把对照的无条件过滤当作天生最优。

若需要定向执行，复用 composition TL bench，新增例只在 out 工作副本中：同 PSCID/同 root；先覆盖 main A alias 和 main A 忙时 fast V 的实际 owner，检查完整返回、hit 与后续 victim。保留旧物理 way 测试预期。新例是工程基线，不是现成新性能结论。实现/运行尚未开始；生产 RTL/Linux/BIT 无本轮改动。

## 创新接受门与停止条件

因为普通方法有具体源码/前作覆盖，本轮不能把 slot-sharing、private continuation、true-requester gate 命名为独立创新。如果下一候选只改阈值、换 owner 来源或在弱对照上占优，就停止该机制枝。如果能在相同资源、真实可达场景下证明新的普通不可达点，并给出不依赖错误身份/旧工程缺陷的机制与实测，才可以称独立贡献。当前没有这一点，也没有期刊分区承诺。

不重跑 §45 固定链图或 §43 老 DUT；不为这项已有机制启动 FPGA 编译。最新真实完整 copy 仍为 §43。本轮纸面例、成本式及接口不可区分证明均不是板卡/仿真通过。

