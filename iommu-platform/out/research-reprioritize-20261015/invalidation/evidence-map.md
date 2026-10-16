# I 证据地图与核读范围

任务日期 2026-10-15；交付 2026-10-16（Asia/Tokyo）。这是对 proposal 的可导航证据入口，不是查新或性能证书。所有哈希覆盖实际文件完整字节，行号为 1-based inclusive；真实核读范围与完整 SHA256 在 [source-manifest.json](source-manifest.json)。原论文 PDF 仅作为既有文本的原件关联做哈希，本席没有重新渲染/完整精读 PDF。

## root 应先亲核的五处

1. **真正正常源与等待：** [ixgbe_main.c](/home/xvhaocheng/work/iommu-platform/linux/drivers/net/ethernet/intel/ixgbe/ixgbe_main.c:1133) 的 DD、cleanup 顺序与每 fragment unmap；[dma-iommu.c](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/dma-iommu.c:707) 在 strict sync 后释放 IOVA。没有实际稀疏合批来源就不能接受 SET32 正例。
2. **最便宜的新强对照：** [dma-iommu.c:1318](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/dma-iommu.c:1318)、同文件 :1359–1391：map_sg 使用连续 IOVA allocation，unmap_sg 调一次连续区间 __iommu_dma_unmap。物理 fragment 离散不能作新硬件必要性证据。
3. **回收寿命不同：** [ixgbe_main.c:1155](/home/xvhaocheng/work/iommu-platform/linux/drivers/net/ethernet/intel/ixgbe/ixgbe_main.c:1155) 先调用 skb 消费，再 unmap；[io_pgtable.c:180](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/io_pgtable.c:180) 的本地 unmap 清 PTE/gather，所读段不加 freelist。不能把 IOVA、页表和 payload/pin 统一乘一个延后时间。
4. **最接近压缩方法：** [IDYLL §6.3](/home/xvhaocheng/work/iommu-platform/out/research-landscape-20261005/papers/idyll2023.txt:514) 已有 base+多个 offsets 的有界稀疏合并，:571–573 明确 TLB 立即失效。候选新增付费位置/合同需要结果支持，不能只换名或换位置。
5. **规范与实际执行边界：** [RISC-V 保存原文](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/riscv-queues.txt:280) 允许扩大失效；:302–351 规定 CQ 前缀与 memory ordering。当前 [CQ RTL](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:276) 在取回后推进 head，:362 仍 PR/PW TODO；不得接受 raw head 是完整撤销安全证明。

## 命题—证据—判断

| 命题 | 原始证据 | 本轮判断与不能外推的范围 |
| --- | --- | --- |
| 正常动态 I/O 确有 map/DD/unmap | SRC01 | source-only 成立；本机部署、co-location 和 sparse batch 均未知 |
| strict 回收与 FQ 不同 | SRC02:617–620、707–727，SRC04:1198–1209 | 条件 strict 接口存在；FQ 当前回退；不据能力缺项断言全部普通合批失败 |
| payload 必须随 IOVA 保留到 fence | SRC01:1155–1165，SRC02:690–727 | 此统一推断不成立；实际最后 physical free 仍须观察，不能反向宣称调用立即完成释放 |
| 当前本地每 unmap 都产生页表页回收债务 | SRC03:203–218，SRC05:180–208 | 通用支持不等于本地实际加入 freelist；当前所读路径没有此动作 |
| skb fragments 要靠新 sparse opcode 才能合并失效 | SRC02:1318–1332、1359–1391 | 这个必要性说法被普通连续 SG 对照排除；不等于所有跨对象动态稀疏模式都不存在 |
| 现有 gather 已保存完整稀疏集合 | SRC03:591–627 | 不成立：min/max 表示且 disjoint 可能触发 sync；新 gather 是付费改动 |
| bitmap 命令一定不会清 live 页 | SRC08:280–282；SRC06:217–244 | 只有新增明确保留合同并正确实现才可说；现行规范默认没有此性能保证 |
| 精确命令只需把注释恢复就完整正确 | SRC04:1777–1846，SRC06，SRC07，SRC08 | 不接受：须核 identity、page/global/nonleaf/alias、旧 fill 与完成。修复过宽 baseline 不是候选创新 |
| 稀疏合并/压缩未被前作做过 | SRC11:514–588 | 不成立；IDYLL 明确已有。候选与它的正常命令费用差异仅是假设 |
| 没有 CQ 等待的静态 live 数据尾时延可由 CQ prefix 规则单独解释 | SRC08:302–351 | 不成立；需真实共享服务或误清因果边 |
| 八目标示例已证性能正例 | proposal §6 | 不成立；是未运行、有明确前提的表示能力见证。32B/144B 只是编码下界 |
| 目前应实现 I | 全部原文与缺项 | 不建议。本轮仅保留投影和一次输入问题；I not_ready，跨方向选择交 root |

## 新近邻的精确状态

[Crossref 原始元数据](sources/zread2024-crossref.json) 核实 DOI [10.1109/ACCESS.2024.3502688](https://doi.org/10.1109/ACCESS.2024.3502688) 的题名、作者和 IEEE 原始入口。[获取收据](sources/retrieval.json) 记录 IEEE 两入口 HTTP 418、CORE thesis 镜像 HTTP 403；没有成功取得或保存正文。检索中出现 bitmap-based ranged TLB flushing，只用于定位强近邻。**全文方法、准确章节与覆盖判定均未核；不作覆盖或 novelty 已通过的结论。** 本轮不继续泛搜或绕访问限制。

## 来源目录

下表所有“当前源码”都只表示实际工作树字节；HEAD 不代表 dirty 文件或运行硬件。来源层级不按论文好坏打分，而区分原文、声明、既有分析与线索。

| ID | 本地入口 | 实际核读范围 | 来源级别/用途 |
| --- | --- | --- | --- |
| SRC01 | [ixgbe_main.c](/home/xvhaocheng/work/iommu-platform/linux/drivers/net/ethernet/intel/ixgbe/ixgbe_main.c:1108) | 1108–1209, 8298–8383 | primary_source_current_worktree；normal TX lifecycle; driver source only, not installed NIC |
| SRC02 | [dma-iommu.c](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/dma-iommu.c:127) | 127–219, 610–623, 637–754, 1070–1087, 1224–1239, 1260–1393, 1616–1637 | primary_source_current_worktree；FQ/strict, IOVA allocation, SG allocation and unmap; runtime backend unknown |
| SRC03 | [iommu.h](/home/xvhaocheng/work/iommu-platform/linux/include/linux/iommu.h:203) | 203–226, 548–629 | primary_source_current_worktree；gather stores min/max and may sync disjoint pages; freelist not payload ownership |
| SRC04 | [iommu.c](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/iommu.c:333) | 333–364, 485–608, 1193–1212, 1352–1357, 1449–1471, 1777–1846, 1855–1893 | primary_source_current_dirty_worktree；post/poll/flush, current overbroad host operands, capable fallback; not correctness run |
| SRC05 | [io_pgtable.c](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/io_pgtable.c:168) | 168–209, 230–256 | primary_source_current_worktree；actual local leaf unmap and gather; no freelist addition in read path |
| SRC06 | [rv_iommu_iotlb.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:194) | 194–247, 310–335 | primary_rtl_current_worktree；PSCID/address matching and inval priority; not full handshake/fill-race proof |
| SRC07 | [rv_iommu_cq_handler.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:255) | 255–402 | primary_rtl_current_worktree；head fetch/decode pulse and PR/PW TODO; no performance inference |
| SRC08 | [riscv-queues.txt](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/riscv-queues.txt:145) | 145–285, 302–355 | primary_official_spec_saved_text；leaf semantics, permitted expansion, IOFENCE prefix and memory ordering |
| SRC09 | [smmu-range-concern.txt](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-concern.txt:16) | 16–85 | primary_maintainer_discussion_saved_text；one-RIL and live-DMA concern; not deployed deadline result |
| SRC10 | [smmu-range-response.txt](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/smmu-range-response.txt:16) | 16–45 | primary_maintainer_discussion_saved_text；full-walk-budget counterexplanation and CONT erratum warning |
| SRC11 | [idyll2023.txt](/home/xvhaocheng/work/iommu-platform/out/research-landscape-20261005/papers/idyll2023.txt:395) | 395–419, 458–510, 510–597, 626–643 | primary_paper_saved_text；MICRO2023 section5.2/6.2/6.3: compressed sparse offsets, immediate TLB invalidation, lazy PTE maintenance |
| SRC12 | [iommu_scalable2015.txt](/home/xvhaocheng/work/iommu-platform/out/research-landscape-20261005/papers/iommu_scalable2015.txt:240) | 240–305, 530–603 | primary_paper_saved_text；ATC2015 allocator density/cache/cost and per-core flush; original deferred contract |
| SRC13 | [riommu2015.txt](/home/xvhaocheng/work/iommu-platform/out/research-landscape-20261005/papers/riommu2015.txt:390) | 390–453 | primary_paper_saved_text；ASPLOS2015 section4 ring/flat structure and one current entry; not implemented here |
| SRC14 | [iommu-bits.h](/home/xvhaocheng/work/iommu-platform/linux/drivers/iommu/riscv/iommu-bits.h:414) | 414–450 | primary_source_current_worktree；128bit current command; opcode7/subop3/PSCID20/GSCID16; proposed encoding separate |
| SRC15 | [.config](/home/xvhaocheng/work/iommu-platform/out/linux/.config:1796) | 1796–1801, 3924–3932 | saved_build_configuration_not_runtime；IXGBE off; strict configured; no running image identity |
| SRC16 | [platform.dts](/home/xvhaocheng/work/iommu-platform/configs/platform.dts:108) | 108–115, 132–177 | tracked_topology_declaration_not_runtime；four idma declarations same instance; lowrisc eth no iommus; native ixgbe absence deferred to prior full-DTS gate |
| SRC17a | [assessment.md](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/assessment.md:1) | 1–102 | prior_analysis_not_primary_proof；previous bounded I review; reused navigation, no global ranking |
| SRC17b | [prescreen.md](/home/xvhaocheng/work/iommu-platform/out/research-plan-implementation-20261015/i/prescreen.md:1) | 1–79 | prior_analysis_not_primary_proof；not_ready baseline and trace requirements |
| SRC17c | [source-evidence.md](/home/xvhaocheng/work/iommu-platform/out/research-plan-implementation-20261015/i/source-evidence.md:1) | 1–185 | prior_source_navigation；navigated source ranges; actual critical files read directly |
| SRC17d | [findings.md](/home/xvhaocheng/work/iommu-platform/out/d-source-gate-20261015/i-backend/findings.md:1) | 1–79 | prior_bounded_source_gate；conditional DMA backend and no current ixgbe instance; not new audit |
| SRC17e | [independent-research-directions-20261015.md](/home/xvhaocheng/work/iommu-platform/docs/independent-research-directions-20261015.md:68) | 68–85 | prior_analysis_not_primary_proof；I task source; old rank not adopted |
| SRC18 | [zread2024-crossref.json](/home/xvhaocheng/work/iommu-platform/out/research-reprioritize-20261015/invalidation/sources/zread2024-crossref.json) | JSON 指定字段 | publisher_deposited_bibliographic_metadata_only；DOI/title/authors/resource verified; full methods NOT CHECKED |
| SRC19 | [retrieval.json](/home/xvhaocheng/work/iommu-platform/out/research-reprioritize-20261015/invalidation/sources/retrieval.json:1) | 1–27 | retrieval_receipt_not_paper_content；two IEEE HTTP418 and CORE thesis403; no recovered manuscript bytes |
| RAW08 | [riscv-queues.html](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/lifecycle/sources/riscv-queues.html) | 未核读正文；只关联哈希 | raw_primary_counterpart_hashed_only；raw counterpart provenance only; this subtask read saved text, not rendered PDF/html |
| RAW11 | [idyll2023.pdf](/home/xvhaocheng/work/iommu-platform/out/research-landscape-20261005/papers/idyll2023.pdf) | 未核读正文；只关联哈希 | raw_primary_counterpart_hashed_only；raw counterpart provenance only; this subtask read saved text, not rendered PDF/html |
| RAW12 | [iommu_scalable2015.pdf](/home/xvhaocheng/work/iommu-platform/out/research-landscape-20261005/papers/iommu_scalable2015.pdf) | 未核读正文；只关联哈希 | raw_primary_counterpart_hashed_only；raw counterpart provenance only; this subtask read saved text, not rendered PDF/html |
| RAW13 | [riommu2015.pdf](/home/xvhaocheng/work/iommu-platform/out/research-landscape-20261005/papers/riommu2015.pdf) | 未核读正文；只关联哈希 | raw_primary_counterpart_hashed_only；raw counterpart provenance only; this subtask read saved text, not rendered PDF/html |

## 交付与实际执行

- [proposal.md](proposal.md)：两个动作思路、单个保留投影、128-bit 预算、完整语义/费用、同信息对照、未运行正反例和一次最小来源输入。
- [candidate-card.json](candidate-card.json)：机器可读范围、缺项、条件预测与停止规则。
- [source-manifest.json](source-manifest.json)：原文绝对路径/准确核读范围/SHA256/来源级别；实际模型字段 unknown。
- 0 模型/实验/构建/旧结果重跑；0 生产源码修改；0 设备/板操作；0 外部模型。进行有界 primary 检索和元数据/原文获取尝试。JSON/哈希/路径的机械检查不计作机制模型或实验。
- 所有新文件都在本 invalidation 目录；root 负责 canonical 状态与最终研究裁决。本席不改 CURRENT_STATUS/HANDOFF，避免越过独占写入范围。
