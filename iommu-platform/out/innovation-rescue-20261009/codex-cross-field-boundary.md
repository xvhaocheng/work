# §49 跨领域前作与实际可实现边界

本文件为 Codex 根级设计/原文判断；不是 DeepSeek 摘要。2026-10-09 再核近邻及跨领域原文。

## 不能作为新增贡献的部件

- [DMA-Latte v3](https://arxiv.org/html/2511.06605v3) VI-A（HTML line308）已经在批量独立 copy 中根据 source/size 推断 broadcast。因此透明发现同源作业、只读一次本身不能作新点；V-B 的带宽饱和讨论也要求真实收益，不能从 read beats 减半直接推出速度翻倍。
- [Multicast AXI XBAR](https://arxiv.org/html/2502.19215v1) II-A line121–124 已给出 finite-buffer W 顺序死锁和全目标 acquire/commit 控制。因此 finite FIFO + multicast 会死锁、按不可撤销请求保证进度本身也有直接前史。
- [Torrent](https://arxiv.org/html/2512.17589v1) III-A/C line129–142：cfg/grant/data/finish，端点分流。Endpoint fork、无整段暂存均不是本工作发明。
- [RFC2887](https://www.rfc-editor.org/rfc/rfc2887.html) §5 已讨论 slowest receiver 限速、不同速率分组/退出；§4 讨论补传。因此“慢者退出，之后补传”不能当新的通用算法。
- [SC24 Network-Offloaded Broadcast/Allgather](https://arxiv.org/html/2408.13356v2) III-B/C/D：ring staging、收到块 bitmap、缺块零拷贝 fetch；barrier/预投 receive 避免 RNR。该文实际机制说明 multicast + 有限 staging + repair 是已有研究，也列出其存储代价。本工作不能以跨域名词不同掩盖同一原理。

## 留下的具体待证贡献

不是通用 fork/replay，而是：在一条仍未退休的长 physical AXI R 内，让 logical A 在 **physical W first-VALID、已冻结的 logical RVALID** 义务结清之后退出，同时让 logical V 沿原 AR 继续；每个 flow 只有 q 宽写债务窗，无整 job/缺块 bitmap 的 payload 暂存；A 以后在精确 contiguous cursor 补读。必须给真实接口控制、完整状态代价及强 tile/closed multicast 的比较。

上述论文的已核章节没有直接给出这组控制合同，这是有限范围的机制比较，不能由全文关键词未命中推断世界上不存在同样实现。若后续原文已有同样合同/组织，须缩小或放弃这项机制贡献；若真实 RTL 在同缓冲与完整 metadata 成本下保住长 R 而对照须更多 physical children，且该差异有系统意义，就可形成微架构贡献。

## 实现位置不能模糊

生产 iDMA frontend 的 deburst 同时约束读与写；因此直接只改 CONF 不实现本合同。可以在 iDMA 输出加有状态的 W burst splitter：接受并保持原 logical AW parent，外部只 first-offer q 大小 children，W 逐段转发并汇合 B；pause 保留 logical parent，停止 future physical AW。coalescer 位于完成权限/属性核验之后，只合并允许共享的 physical source R；raw logical AR/AW/R/B 协议始终保留。

physical quiet 不等于原 logical parent/job retired。已有 logical long AW 不能撤回，但留在每流 splitter 内的余量不会立即占据共享 physical W owner；所有已 first-offer 的 physical children 仍须排完。必须计 raw-parent metadata、源/目的 PA recipes、children/B 汇合、skid、returned cursor、mask、held headers 和 iDMA FIFO，不能只报新增计数器。

若 adapter 置于译址之后，q 个 PA children 不自动代表 q 次 PTW；若置于之前，实际 TL/lookup/PTW 都要计，不能混用对照。可预先保存稳定的 WR recipe，但其寄存器/映射生命周期不是免费。初版排除 live invalidation 与 mutable source；跨页按实际准备好的 permission/mapping span 切段。

## 仍未关闭的性能风险

两流合计仍写 2N beat，因此共享 R 不改写带宽下界。q 写段可能放大 AW/B 或译址开销。一个长 physical R 被 backpressure 时会占用该 read child；第三方公平性必须另测。较大默认 FIFO 能消除小 FIFO 的结构循环，故 B3 只能说明有界缓冲场景，不能伪装成 production 默认故障。

下一 RTL 实验先证明协议、数据与 bounded debt；再测同成本 tile 对照的 AR 开销/长 R 代价。不能将事件模型 step 差解释成 FPGA cycle 或把论文分区作为验收值。
