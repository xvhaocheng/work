# 共享翻译结果、私有续走与 requester 历史：进度说明（2026-10-08）

**主结论：当前平台不存在所假设的“统一 slot 争用”；共享翻译结果 ＋ 各请求私有续走状态已有具体 NB 前作（NB-Walker 固定 commit 源码），因为普通方法已经实现所列能力，把二者简单组合不能称独立创新。**

## 0 范围与实测边界

本轮做了源码、原文和纸面构造核查，没有新增数学运行、DUT 构建或运行、生产 RTL/Linux/tests 修改、BIT/JTAG/板卡/SD 操作。最新实际完整拷贝仍为 §43；下文共享场景未运行，不能据此解释旧 PN 变慢或给出性能收益。

## 1 当前平台不是统一存储池

IOTLB 与 PTW 是 `rv_iommu_tl_wrap.sv:657-685`／`:759-804` 的独立实例；IOTLB 的 tag/PLRU（`rv_iommu_iotlb.sv:58-68,464-473`）与 PTW 的 walk 状态（`rv_iommu_ptw.sv:661-696`）分属不同 `always_ff`，PTW 仅在结束时经 `update_o` 写 IOTLB。lookup 端口、TL 入口与外部内存可以争用，但没有“同一缓存槽／同一续走帧池”的证据。

## 2 旧 latest A/V fixture 不能当 nested 共享实测

`tests/composition_tl_test.cpp:342-371`、`tests/nonblock_actual_dma_test.cpp:453-503`（及 `out/dual-write-prefix-20261008/multi_test.cpp:913-948`）中，V/A 的 PSCID 为 10/11、S-root 为 0x20000/0x30000、IOVA 起点为 0x10000000/0x20000000，且都是 Sv39 stage1-only、G-stage Bare。它们没有验证跨 DID 共享或双阶段 nested 共享；其中 N 的命中场景来自 V 自身上下文。旧实验的完整拷贝与原有性能结果仍保留，不能改称共享场景的结果。

## 3 一个合法共享构造（纸面，未 run）

让 DID11 的 DC 也引用 PSCID10 与相同 S-root 0x20000、G-stage Bare；DID10 读 IOVA 0x10000000（P0），DID11 读 0x10001000（P1）。两笔共享的是上层 PTE：root PTE 地址 0x20000（0x10001→0x40000）与中间 PTE 地址 0x40400（0x14001→0x50000），地址与解出字段相同，可一次读取共享；但相邻的两条 4KiB leaf PTE 地址不同（0x50000=0x200000DF、0x50008=0x200004DF），最终翻译不能合并。这里共享的是每次三级翻译中的前两次 PTE 读取，第三次读取不同 leaf。后文的第三个页面 P3 是另一笔请求，不能与“第三级 PTE 读取”混为一谈。共享前提是静态、同一合法地址空间与相同解释语义；PSCID/GSCID 标识地址空间以支持复用，不是物理地址相同即可共享。

## 4 工程边界：resident-way 隔离 ≠ requester 隔离

IOTLB tag 无 DID，lookup 不检查 fill mask；`PartitionLocalHistory` 按被命中的 resident way 更新 PLRU（`rv_iommu_iotlb.sv:384-401`）。因此 A 可合法命中 V 的 protected way 并改 V 的 local PLRU：V 已有 P0/P1 两条 resident 时，A 命中 way0/P0 会把下一个 V cold fill 的 victim 从 way0 改成 way1（纸面构造；P3 的 leaf 为 0x50018，值 0x200008DF，IOVA 0x10003000→SPA 0x80002000）。这只改变未来 victim，不改条目内容，也推不出平均速度或“PN 变慢”。旧合同是“不同 resident 分区的 PLRU 节点分开”，不是“A 的任何命中都不能改 V 历史”：`tests/composition_iotlb_isolated_test.cpp:570-576` 的 S3 明确要求 V-way 在 A-mask 下命中并写入 protected 节点 7 —— 那是旧合同的有意期待，故不能称旧测试通过无效、也不能称原 RTL 有 bug。

## 5 不能用 main fill mask 过滤 touch：正确 source class sideband 是普通工程方案（尚未实现）

在 IOTLB 输入边界，main A 的正常 alias hit 与“main A 忙时 fast V 的同 key hit”可以给出完全相同的 lookup/VPN/PSCID/stage/touch/mask（`rv_iommu_tl_wrap.sv:271-277,669-673`），但前者正确 class=A、后者=V，因此按 main mask 过滤会错误压制 fast V。fast 侧来源与接受条件已具体核对：`rv_iommu_nonblock_hit.sv:52` `ddtc_did_o = req_q.did`、`:55` `iotlb_pscid_o = dc_q.ta.pscid`、`:54` `iotlb_vpn_o = req_q.iova[...]`；`:100-103` 接受时要求 `candidate_i == req_q`、`ddtp_i == ddtp_q`、`ddtc_dc_i == dc_q` 与 `leaf_ok`；`:106-107` 仅在 accept 时 touch 两个 cache，且该布尔不含 requester class。正确做法是把**实际 lookup 来源**的 class 与 accepted touch 一起传过该边界；这是局部充分的普通接口方案，本轮未实现、未运行，也不处理准入/填充/失效与端口干扰，不能当端到端保证。

## 6 明确前作与 claim 边界

[US7415575B1](https://patents.google.com/patent/US7415575B1/en)（NVIDIA，priority 2005-12-08，公开 2008-08-19）已公开“任意 client 可命中共享 TLB entry，但只按实际 requesting client 更新其局部替换历史”（Fig.7 步骤 702/706/708 接收 client ID；claim 6）。因此“共享 hit ＋ requester-specific replacement”属既有机制，因为它直接覆盖共享命中与按请求来源更新历史的基本机制，补真实 hit 来源只能作为工程合同补充与强对照，不能称独立创新。[NB-Walker artifact v2](https://zenodo.org/records/21488827)（固定 commit 8837da7d…）的源码给出“共享 PTE fetch ＋ 每请求 Info/transaction ＋ 共享 MSHR 与 PTW reservation”，这是**代码级实现证据**，不是其论文的公平进度定理；引用须落到具体文件行，不用泛标题充当证明。

## 7 下一步与停止条件

主线继续评价 V 与 A 的整体性能，不自动追加“A 的任何命中都不能改变 V 历史”的硬保证。下一步先把普通的真实 requester 过滤纳入强对照，检查共享命中带来的复用收益与替换影响，是否存在它、resident-way 隔离及普通共享策略都做不到的同资源性能点。若需要执行判别，复用既有 TL bench，仅补同 PSCID/shared-root 的定向例，覆盖 main A alias 和 main A 忙时 fast V 两个来源。它是工程基线，不能单独当论文贡献；当前未实施，不为此先编译生产 RTL/BIT，也不重跑固定链模型。

## 8 来源与账本

[共享可达性与 NB 源码对照](../out/translation-sharing-progress-20261008/codex-sharing-reachability.md)／[请求来源与历史的根级证明](../out/translation-sharing-progress-20261008/codex-requester-history-boundary.md)／[前作审核](../out/translation-sharing-progress-20261008/map-critical-agent.md)／[状态与反例审核](../out/translation-sharing-progress-20261008/rules-critical-agent.md)／[原始证据](../out/translation-sharing-progress-20261008/deepseek-source-evidence.md)／[证据补正](../out/translation-sharing-progress-20261008/deepseek-source-corrigendum.md)。

[原文及源码清单](../out/translation-sharing-progress-20261008/primary/sources.json)保留 Barr 2010、US7415575B1、NB-Walker 固定 commit；旧 gem5 副本保留，但不作为本轮三项最终对照。官方规范前提见 [RISC-V IOMMU 简介](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html)及[数据结构](https://docs.riscv.org/reference/iommu/v20260222/iommu_data_structures.html)。[阶段账本](../out/translation-sharing-progress-20261008/stage-evidence.json)／[下一判别](../out/translation-sharing-progress-20261008/codex-next-decision.md)。

**归属**：DeepSeek 负责证据整理/勘误/草稿；Codex root 亲自构造、读关键源码与专利原文、保存并最终审定；两名 native 席做关键质疑；未使用 Claude。

