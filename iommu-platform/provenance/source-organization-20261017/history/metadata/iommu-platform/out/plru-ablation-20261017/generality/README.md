# 替换偏置能推广到哪里

2026-10-17。这里的 mask 是**允许分配的 IOTLB 条目编号掩码**；它不是 IOVA/VPN 的位掩码，也不是 tag 的 valid 位。本检查只读源码和已有 Linux 记录，没有新 Linux、FPGA 或板端实验。主实验的因果结论另见上级目录。

| 问题 | 结论 | 直接依据与边界 |
|---|---|---|
| 本地 CVA6 的 TLB 是否有同样的 mask 条件？ | **不会由这项 mask 条件触发：所查三种 TLB 没有 way eligibility mask。** | Sv32、Sv39、Sv39x4 均以完整二叉树解码 victim；tag valid 只参与查找/失效，不裁剪替换树。普通 tree-PLRU 自身仍可能受访问次序影响，不能据此承诺精确 LRU 的驻留性质。 |
| zero-day-labs 上游当前默认主分支是否有同样的 mask 条件？ | **不会由这项 mask 条件触发：该版本没有这项分区扩展。** | 官方 GitHub API 实时返回默认分支 `main`、提交 `18ad30a7d2fd8fc0c0f16b99e39b0fd276437445`；IOTLB 完整树解码不检查 way mask。该设计注明改编自 CVA6 TLB，两个来源不是独立硬件家族的复现。 |
| 本地分区 IOTLB 是否具备不等 eligible 子树？ | **会，源码条件存在；实际 39 次 walk 的归因交给主实验。** | `0xfffc` 允许 way 2–15：根的两个子树分别有 6/8 个可分配叶子，下一层有 2/4 的划分。只判断子树是否非空，再按一个 PLRU bit 选择；不按可分配叶子数重构平衡树。 |
| Linux streaming allocator 会不会产出相邻、重复或循环的页号？ | **会，源码允许；不会仅因页号奇偶触发 way-mask 偏置。** | 冷路径从允许上限向下找对齐空洞；小分配可从每 CPU magazine 复用旧范围。真正相关的是映射寿命、命中/填入次序、热源再用和替换状态。 |
| 每次 I/O 都 map/unmap 的真实 Linux 路径是否已证明重现 R8？ | **不确定；现有记录不能作此证明。** | 当前 strict 后端在 unmap 时同步失效，本地 RISC-V 驱动还把范围失效扩大为整个一阶段上下文集合；它可能先破坏热源与目的驻留。找到的既有 iDMA IOVA 记录来自专用固定映射，没有通用 streaming allocator 的逐 map/unmap 地址与访问联合轨迹。 |

## 1. 本地和上游源码定位

以下路径相对工作区根。完整同字节副本、SHA-256、行号片段在 [source-manifest.json](source-manifest.json) 和 [source-excerpts.md](source-excerpts.md)。本地 CVA6 HEAD 为 `a1b08934391833aed21e6ad63b545634040808ac`，三份 TLB 在本轮检查时无工作树改动；本地 IOMMU HEAD 为 `57eb5621802ef9ed8d77c684b9a03205c6630391`。文件 hash 优先于 HEAD 标识实际读到的字节。

| 文件 | 精确行号 | 核到的逻辑 |
|---|---|---|
| `iommu-platform/cva6/core/mmu_sv32/cva6_tlb_sv32.sv` | 124、141–215 | refill 用 `replace_en[i]`；hit 更新路径，完整树按 way 编号路径解码，无允许分配 mask 输入。 |
| `iommu-platform/cva6/core/mmu_sv39/tlb.sv` | 134、152–227 | 同上；183 行命中更新，226 行 `replace_en[i] = en`。 |
| `iommu-platform/cva6/core/mmu_sv39x4/cva6_tlb_sv39x4.sv` | 247、272–347 | 同上；303 行命中更新，346 行 `replace_en[i] = en`。88–90 行 `mask_pn2` 是地址匹配临时变量，与 way eligibility 无关。 |
| 上游 `rtl/translation_logic/rv_iommu_iotlb_sv39x4.sv` | 14–20、24–68、354、382–455 | 声明 CVA6 血缘；接口无 policy mask；411–419 行命中更新，437–454 行直接遍历完整树。 |
| 本地 `iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv` | 38–40、353–372、384–400、405–422 | 本地 `policy_mask_i` 与 partition-local history 扩展。eligible 通过 OR 得到，仅编码非空；416–418 行选择非空子树，422 行最终与 allocation mask 相与。 |

上游查询时间为 `2026-10-17T05:23:54.583354+00:00`。固定版本的[官方源文件](https://github.com/zero-day-labs/riscv-iommu/blob/18ad30a7d2fd8fc0c0f16b99e39b0fd276437445/rtl/translation_logic/rv_iommu_iotlb_sv39x4.sv#L382-L455)、[默认分支仓库页](https://github.com/zero-day-labs/riscv-iommu)、[官方 API](https://api.github.com/repos/zero-day-labs/riscv-iommu)均已查询；下载原件和元数据位于 [raw/upstream](raw/upstream/)，回执为 [upstream-source-receipt.json](upstream-source-receipt.json)。这是该时刻的固定版本结论，不覆盖其他分支、未来提交或未检查的开源设计。

因此，本次检查**没有找到第二个 stock 实现具备同样的 mask 不等子树条件**。普通 tree-PLRU 不是具有精确 LRU 栈性质的替换策略：频繁命中一个热源可把树的祖先指针反复拨向另一侧，这类访问次序偏置可以共有；本检查没有对所核 stock 版本运行该序列。不能把“普通 PLRU 源码同源”写成“普遍存在 masked PLRU 缺陷”；也不能把本地分区改动的问题归给上游默认实现。

## 2. Linux streaming map/unmap 与页号序列

所读平台 Linux HEAD 为 `c0f5e13ea81fa30d78f3bb1d6389e2397321e040`；`dma-iommu.c` 和 `iova.c` 本轮无工作树改动。当前 `riscv/iommu.c` 和研究 `idma.c` 有既有改动，均以本目录快照 hash 标识，不能由 HEAD 推断运行版本。此前 [IOVA 预算核查](../../mapping-iova-budget-20261016/allocator/findings.md)使用外部 Linux v6.12；这里重新定向读取**平台已有 Linux 源码**，没有混用其行号。

1. **映射和分配。** `linux/drivers/iommu/dma-iommu.c:1015–1067` 的 `iommu_dma_map_page` 进入 `__iommu_dma_map`；`:730–753` 对物理偏移和覆盖长度按 IOVA granule 对齐，分配新 IOVA、建立页表，再返回 `iova + offset`。`:658–687` 以 DMA mask、bus limit、aperture 为上限调用 `alloc_iova_fast`；这里的 DMA mask 是地址可达上限，与 RTL 的 way mask 无关。
2. **冷路径能给相邻页号。** `linux/drivers/iommu/iova.c:178–225` 向下搜索空洞，关键是 `new_pfn = (high_pfn - size) & align_mask`（205 行）。对连续的 size=1 分配、空缓存和无其他占用的上端空洞，可得到 `L, L-1, L-2, ...`，同时包含奇偶页。大于一 granule 的分配按大小对齐；它不保证所有正常工作负载都给某一个固定步长。
3. **热路径会复用页号。** `iova.c:439–459` 先取 rcache，小请求先向上取二次幂；`:487–492` 释放优先放回缓存；`:697–717` magazine 从最后一个符合地址上限的项弹出，`:839–887` 选择本 CPU 的 loaded/prev/depot。因此正常同尺寸循环可重复使用近期 IOVA，也可因释放顺序、CPU、其他映射和地址限制变成其他序列。不能只凭源码声称原生驱动一定单调、随机或交替。
4. **页号奇偶不是机制。** 本地 IOTLB 是按 VPN/context 全相联匹配（本地 `rv_iommu_iotlb.sv:103–136`），victim 的树路径索引是 **way 编号** `i`（405–422 行），不取 VPN 的低位。对保持页大小、别名关系和 context 相同的访问序列作一一页号重命名，命中/替换次序不会因奇偶标签本身改变。实验里“偶数页反复 miss”只能是该填入与访问次序下的页标签现象。
5. **unmap 改变驻留条件。** `dma-iommu.c:707–727` 在 strict 情况先 `iommu_unmap_fast`，再同步 IOTLB，最后回收 IOVA；FQ 则另有退休寿命。当前平台 `dma-iommu.c:618–620` 因 RISC-V 驱动未声明 DEFERRED_FLUSH 能力而把 DMA_FQ 降为 DMA（能力函数 `riscv/iommu.c:1198–1209`）。`riscv/iommu.c:1803–1817` 的 PSCID 和逐地址设置仍被注释，一阶段执行宽失效及 IOFENCE。对串行“每个 I/O 完成就 unmap”流，下一次 miss 可能首先来自主动撤销，不能认成九个持续映射页之间的容量替换。并发、多映射保持 live 的时间窗仍可能出现相关访问次序，但是否足够长、是否带本地分区 mask、效应多大，源码不能定量回答。

## 3. 已有真实 Linux IOVA 记录

按研究导航定向检查了 mapping-iova-budget、mapping-dma-visibility、d-source-gate 的来源报告，再沿既有验证记录检查 DMA 缓存诊断与 composition-isolated 目录；没有扫描整个 Linux 或全部 out。

找到的原始 [mismatch.txt](raw/local/iommu-platform/out/dma-smoke-debug-20260916/mac-20260916-020422-run-_hjma4td/mismatch.txt)（1 行）记录 Linux `6.5.0-rc2-00079-g53d3113dee74` 的 DID10：`src_iova=0x3fffe00390`、`dst_iova=0x3fffe01390`、偏移 `0x390`。对应 4KiB 页基址是 `0x3fffe00000` 和 `0x3fffe01000`，VPN 分别 `0x3fffe00`（偶）和 `0x3fffe01`（奇）。该次是既有**数据校验失败**诊断，不能作为替换性能样本；旧 `*_pte_pa` 查询还有另行确认的软件错误，不取它们推断硬件 IOTLB 内容。

更关键的是，冻结的历史 `out/dma-cache-sync-20260916/idma-before.c:157–178` 用 `idma_find_iova` 手选 2MiB 对齐基址，再把目的紧接在源后；`:296–305` 用直接 `iommu_map` 建立研究映射。当前 `linux/drivers/misc/idma.c:242–264,386–395` 保持同一性质。因而这对真实地址**不验证 `alloc_iova_fast` 的自然分配次序**，也不是每个 COPY 都 map/unmap 的流式应用轨迹，且没有关联 IOTLB way/victim，不能确认替换因果。抽核的 composition-isolated `short-mp-a8/record.raw.json` 没有具名 IOVA/map/unmap 序列字段；未把其 raw 数组当成未经解码的页号证据。

有限结论是：**Linux 能分配足够普通的页号标签，源码没有奇偶禁区；是否实际触发本地 masked tree-PLRU 偏置仍取决于访问/驻留与分区条件，现有 Linux 记录没有完成该联合验证。** 无新实验，也不据此声称正常网络/存储应用已重现主实验现象。

## 文件

- [source-manifest.json](source-manifest.json)：10 份本地原件的路径、hash、读取范围和仓库 HEAD。
- [source-excerpts.md](source-excerpts.md)：带原始行号的关键片段；[local-hash-check.json](local-hash-check.json) 核实快照字节一致。
- [upstream-source-receipt.json](upstream-source-receipt.json)：上游默认分支、固定 commit、下载 URL/hash；[raw/upstream](raw/upstream/) 保存 API 与原始源码。
- [collect_local_evidence.py](collect_local_evidence.py)：本地材料收集方法；重新运行会重新采当前工作树，历史验证应读已冻结的快照。
- [existing-record-inspection.json](existing-record-inspection.json)：一份既有板端 JSON 的抽核范围与 hash；[verification.json](verification.json) 记录快照、JSON、链接和原文 hash 检查。

本子任务由 Codex 直接查官方来源、读源码并整理；未调用 DeepSeek，也没有修改生产源文件。
