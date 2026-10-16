# v6.12 dma-iommu / SWIOTLB：来源核查

结论：**IOMMU 与 bounce 可以同时存在，但不能把当前 B 的“整页持久 map”直接认作 bounce。** 固定实现仅在 `dev_use_swiotlb(dev,size,dir)` 且原物理起点或长度不按 IOVA granule 对齐时进入 bounce。若本切面整页起点、整页长度均按该 granule 对齐，map 不发生 SWIOTLB payload copy；后续包范围 sync 也不会凭空创建 bounce。若映射确实 bounce，首次 map 拷贝整个映射请求的 `mapping_size`，以后的 `DMA_TO_DEVICE` range sync 拷贝所同步范围 `size`。数据改变却仅复用旧 DMA handle，不能替代该次同步。

本报告只核固定 commit `adc218676eef25575469234709c2d87185ca223a` 的 backend。请求配置 `gpt-6.1-sol/high`；独立运行 model/effort metadata 未取得，实际记为 `unknown`。Codex 读源码、取原始文件并撰写本报告；未使用 DeepSeek，未测任何 native 配置或成本。

## 证据入口与范围

- `I` = `/home/xvhaocheng/work/iommu-platform/out/mapping-admission-20261016/v612-source/raw/drivers/iommu/dma-iommu.c`，复用原文件、旧 HTTP 收据和 hash。
- `M` = 同一 `raw/kernel/dma/mapping.c`；`D` = 同一 `raw/include/linux/dma-mapping.h`，只核后端分派、need_sync 和 range 转发的小段；DMA API 文字契约及 mlx5 全链由 API 席独占。
- `S` = 本目录 `raw/kernel/dma/swiotlb.c`；`H` = 本目录 `raw/include/linux/swiotlb.h`。后者是唯一额外 header：关键 inline 的 pool-membership 检查决定无条件 swiotlb 调用实际上 copy 还是 no-op，不能只凭函数名推断。
- 官方固定原文：[dma-iommu.c](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/drivers/iommu/dma-iommu.c)、[swiotlb.c](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/kernel/dma/swiotlb.c)、[swiotlb.h](https://github.com/torvalds/linux/blob/adc218676eef25575469234709c2d87185ca223a/include/linux/swiotlb.h)。本次只补两个固定 URL，无版本／论文／全仓检索。

## 源码已核：bounce 的触发和映射对象

1. `I:601–612`：untrusted 定义是 PCI 设备的 `untrusted` 位；`dev_use_swiotlb` = `CONFIG_SWIOTLB && (untrusted || dma_kmalloc_needs_bounce(dev,size,dir))`。**不是所有不对齐 map 都 bounce**；还要满足前一谓词。`dma_kmalloc_needs_bounce` 的 inline 定义不在已取文件内，本轮不扩张其架构、方向和配置细节，保持未展开谓词。
2. `I:684–685,714`：IOVA granule 选 `domain->pgsize_bitmap` 最小页。`I:1175–1189`：第二条件为 `iova_offset(iovad, phys | size)`。因此应精确写“按该 IOVA granule 对齐”；由 CPU `PAGE_SIZE` 直接推相同 granule，仍需具体 domain 条件。整页对齐不是用“可信设备”替代安全合同：即使 untrusted，也可因第二条件为零而不 bounce。
3. 满足条件而 SWIOTLB 不 active 则 mapping error，pool 分配失败也 mapping error（`I:1181–1192`，`S:1382–1409`）；不能把失败请求记为成功完成。
4. `I:1188–1189` 将 `phys` 改成 bounce PA；`I:1217` 再交 `__iommu_dma_map`。后者 `I:848–876` 分配 IOVA 并 `iommu_map(domain,iova,phys-ioff,...)`。所以该合法路径是 **IOVA → bounce PA → CPU 源 buffer**，不是 IOMMU 与 SWIOTLB 二选一。
5. `S:184–205,354–369` 解析／保存 `force`，`H:172–177` 提供 `is_swiotlb_force_bounce`。但已核的 **dma-iommu** 触发函数 `I:606–612` 与 `map_page` 并不读取 force 位；本轮不把 `swiotlb=force` 当作本 IOMMU 路径“整页必 bounce”的证明。direct DMA 的 force 判定不在独占范围，未核。
6. `I:742–759`：`DMA_TO_DEVICE` 为 `IOMMU_READ`；`DMA_FROM_DEVICE` 为 `IOMMU_WRITE`；bidirectional 两者都有。这里是设备访问映射的方向，不是完整部署信任／授权合同。

## 源码已核：每类事件的真实动作

以下只计实际 copy 调用的逻辑字节数，**不是 CPU 时间、DRAM 流量或性能模型**。`m` 是成功 map 请求的实际 size；`l` 是合法范围 sync 的 size。所有范围须落在原成功映射内、满足 DMA API 的范围／缓存对齐要求；同步前后的 owner 与完成条件由 root/API 合并。

| 事件 | 不 bounce 的 dma-iommu 映射 | 确实 bounce 的映射 | 精确原文 |
|---|---|---|---|
| 成功 map，任意方向 | 无 SWIOTLB payload copy；非 coherent 且非 SKIP 时 arch-for-device | **原 buffer → bounce，m 字节**，然后非 coherent 且非 SKIP 时对 bounce arch-for-device，随后建 IOMMU 映射 | `I:1188–1219`; `S:1364–1367,1401–1439,859–921` |
| map 时 untrusted padding | 没有 bounce padding 动作 | bounce 前 padding 和后 padding分别 `memset(0)`；payload 是 copy，不能把它也称作 zero | `I:1194–1210` |
| sync-for-device，TO_DEVICE / BIDIR | coherent 可提前 return；否则 arch-for-device；SWIOTLB wrapper 对非 pool PA 无 copy | **原 buffer → bounce，l 字节**，然后如非 coherent 对 bounce arch-for-device | `I:1117–1129`; `H:261–267`; `S:1552–1559` |
| sync-for-device，FROM_DEVICE | 同上缓存维护分支 | 不做 payload bounce copy；必要时 arch-for-device | `S:1556–1559`; `I:1128–1129` |
| sync-for-CPU，FROM_DEVICE / BIDIR | coherent 可提前 return；否则 arch-for-CPU；wrapper 无 copy | 先如非 coherent 对 bounce arch-for-CPU，后 **bounce → 原 buffer，l 字节** | `I:1102–1114`; `H:273–279`; `S:1562–1569` |
| sync-for-CPU，TO_DEVICE | 同上缓存维护分支 | 无 payload bounce copy；仍可有缓存维护／地址翻译等调用 | `I:1107–1114`; `S:1566–1569` |
| unmap，TO_DEVICE | 如非 coherent 且非 SKIP 做 arch-for-CPU；撤销映射、IOVA 回收相关动作 | 同左，再释放 bounce slots；**无 bounce → 原 buffer copy** | `I:1223–1238`; `S:1535–1549`; `H:248–255` |
| unmap，FROM_DEVICE / BIDIR | 如上 | 非 SKIP 时再 **bounce → 原 buffer，m 字节**，释放 slots | `I:1223–1238`; `S:1542–1549` |

`S:1401–1403` 为分配而上取整的 size，不等于 payload copy 长度；payload map copy 明确传入 `mapping_size` (`S:1439`)。sync 的包范围由 `D:398–409` 把 `dma_handle + offset` 与 `size` 原样转交，`I:1125–1126` 先做 IOVA→PA；`S:862–886` 找到对应 slot / 原地址偏移。对有效范围，copy 是传入 `l`；越界会警告和截短 (`S:888–893`)，这不是合法策略或费用优化。

`H:142–169,248–279` 明确：仅 PA 属该设备 bounce pool 时调用底层 copy/unmap；因此一次非 bounce 的整页 map，随后对小包调用 range sync，即使当前 `dev_use_swiotlb(dev,l,dir)` 为真，也只会查 pool 后 no-op，不会新分配 bounce 或拷贝该包。

`I:1107–1108,1122–1123` 的 coherent 提前 return 还依赖**当前 sync 的 size 和 dir** 重新计算 `dev_use_swiotlb`。本报告的 bounce 反例固定 untrusted，因此该条件对所有包范围稳定为真；不把未展开 `dma_kmalloc_needs_bounce` 的尺寸边界推成所有 backend 的普适规律。

`arch_sync_dma_*` 的实际 cache 指令、cacheline 覆盖长度、是否在特定方向 no-op、耗时都依赖架构／部署，未取得。仅看到调用不能赋 `m` 或 `l` 物理带宽成本，也不能把 cache 维护、payload copy、padding zero、IOVA/PTE 操作混账。pool 初始化 `S:269–299` 的批量清零属于池生命周期，不是每个 streaming map 都清零整池。

## 源码已核：SKIP 与 dma_need_sync 的边界

**SKIP 不会删除 map 时的 payload 初始化 copy。** `S:1412–1439` 无论 dir、attrs 都调用 `swiotlb_bounce(...,mapping_size,DMA_TO_DEVICE,...)`；它同时覆盖 FROM_DEVICE 的初始副本，以保持设备部分写时未写区语义（`S:1430–1437`）。`DMA_ATTR_SKIP_CPU_SYNC` 在 `I:1214–1215,1233–1234` 跳过对应 arch sync，在 `S:1542–1545` 跳过 unmap 时的 copyback；显式 sync 函数本身没有 attrs 参数。合法使用条件由 API 席引用官方契约；本席不为减账自行添加该 flag。

**`dma_need_sync` 足以表达 API 的“是否需要调用同步”，不足以分类“本 mapping copy/zero/cache 各多少”。**

- `D:295–312,329–331`：`dma_dev_need_sync` 为 `!dev->dma_skip_sync || CONFIG_DMA_API_DEBUG`，再决定是否进后端；`M:430–441` 只有 direct 路径做地址粒度细查，**IOMMU 分支返回 true**。
- `M:449–455`：direct/IOMMU 设备在 setup 时以 coherence 初始化 skip。`S:1412–1416` 首次成功分配 bounce 后调用 `dma_reset_need_sync(dev)`；此 inline 定义未另补取，但调用点注释明确意图“always sync SWIOTLB buffers”。不能把首次 bounce 前观测到的 false 永久缓存为设备分类。
- true 可以是 noncoherent、debug、或该设备曾分配 bounce 后仍保持需 sync；同设备当前另一个 granule 对齐 mapping 不必 bounce。false 与 true 都不是 bounce-copy 字节计量接口；不能据此独断是否发生 memcopy、是否全页 copy、是否同步有零 CPU 成本。
- `D:333–358` 的无 DMA/无 DMA_NEED_SYNC 编译分支返回 false 且 sync 为空；本轮没有具体运行 config，不虚构其适用性。

## 条件部署反例：改变 payload 后读到旧 bounce

此为源码支持的**条件反例**，不是当前整页 B 映射已触发 bounce 的测量或主张。

冻结条件：采用 dma-iommu backend、`CONFIG_SWIOTLB=y`、PCI untrusted B、有 active/可成功分配的 pool；map 起点或长度不按 IOVA granule 对齐；`DMA_TO_DEVICE`、不加 SKIP；buffer/handle/B 配置在合法生命周期内，包范围合法、串行取得所有权。令成功 map 长度为 `m`，包范围为 `R=[o,o+l)`。冻结 map 的 bounce PA Q 和源 PA P。

1. t0：P 的 R 含 P0；`dma_map_page(...,m,DMA_TO_DEVICE)` 将 P0 所在整个 m 复制至 Q，并映射 IOVA d→Q。首包 B 正常完成。
2. t1：先等 B 完成，再合法交回 CPU（需要时 sync-for-CPU）；该方向的 swiotlb CPU sync 不 copy。下一包由 CPU 或另设备 A 在完整前置同步／完成后生产为 P1，且 `P1 != P0`。不允许此时 B 仍读同范围。
3. 错误路径：仅因持有旧 d，跳过 `dma_sync_single_range_for_device(B,d,o,l,DMA_TO_DEVICE)` 就让 B 再读 d+o。IOMMU 仍指 Q；Q 的 R 仍是 P0，B 可读到旧包。coherence 不能使两个不同 PA 的副本自动一致。
4. 普通合法持久对照：提交 B 前对 R 调用上述 device sync。`I:1125–1129` + `H:261–267` + `S:1552–1557` 复制 P1 的 l 字节到 Q，然后必要 cache 维护；B 读取 P1，等完成后再允许 CPU/A 复用。普通策略同样可用。
5. 普通 per-packet map 对照：先生产 P1，再合法 fresh map 包范围／所需映射范围；如果同样触发 bounce，map 将该请求范围初始化为 P1，B 完成后 unmap。它承担每次 mapping/slot/zero/arch 等必要动作；无凭空零成本的持久或复制独占能力。

**强普通对照 O0-T 必须另保留**：如果和持久候选共享同一整 granule 授权 T，整页实际独占且 CPU/A/B 交接允许，O0-T 每包都 fresh map granule 对齐的整页，descriptor 只读包范围，完成后按整页 map 参数 unmap。`I:1175–1180` 对普通 fresh map 同样成立，它也可避免 bounce；“整页比非对齐包 map 少 bounce”不属于持久映射独有收益。O0-T 仍承担每次 IOVA/PTE 和必要的整页 cache 维护等成本，与持久方案的包范围同步成本应分别计，不能仅凭无 bounce 断言总成本相同。若 granule 不兼容该页对齐、没有整页授权，或整页 CPU 所有权／缓存对齐交接无法成立，则把 O0-T 保留为对应条件分析，不偷加隔离门槛或默认其可达。

冻结的**整页、IOVA-granule对齐**切面则不同：此 dma-iommu 路径的 Q 不存在，d 映射 P；map 与包范围 sync 的 SWIOTLB payload copy 都为 0。noncoherent 的缓存同步和设备所有权／完成顺序仍须保持。不能用上面的相邻非对齐 bounce 反例，否定该整页候选的可见性；也不能由本切面无 bounce 断言任何 backend 都无隐藏复制。

## 限制与交回 root 的问题

- `source`：以上固定函数体、顺序、copy/zero 分离、granule 对齐门槛、need_sync 的非分类性质，已逐段直接读。
- `conditional_deployment`：具体域 granule、B DMA backend/coherence/trust、active pool、可成功映射、范围对齐、CPU/A/B 合法所有权与完成顺序，须 root 与 API/source 证据合并；源码中的可达路径不是本机／mlx5 当前实测配置。
- `unknown`：`dma_kmalloc_needs_bounce` 内部架构条件、direct DMA force 路径、具体 arch cache动作和成本、运行配置、真实copy/bandwidth/CPU数字。本报告没有新 driver/RTL/ABI、collector、模型、测试、hostprobe、板测或 canonical/Git 操作。
- 建议 root 直接核 `I:1175–1219` 与 `S:1412–1439,1552–1569`，防止把“整页 map”写成“整页必 copy”；需要参数化时以精确 `iova_offset` 条件冻结，而不是泛称“有 IOMMU / coherent 就没 bounce”。
- 这是 AI 辅助的固定源码核查，不是新颖性／独占收益裁决。只用一个固定 Linux 版本来源，不伪称存在第二份独立实现证据；未核项保留为未核。
