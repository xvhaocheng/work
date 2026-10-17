# Linux 主线 DMA-IOMMU strict 与 FQ 源码核对

2026-10-17。在线查询区间为 07:24:16.610066–07:24:16.832515 UTC（16:24:16 JST），`torvalds/linux` 默认分支 `master` 的 HEAD 为 **`58161ead29e50d70a317ea26b472e0c7508e69da`**。07:26:47 UTC 另经仓库 API 确认默认分支。查询原响应、时间和 HTTP headers 见 [master-ref-query.json](queries/master-ref-query.json)、[master-ref.json](queries/master-ref.json)、[repository-query.json](queries/repository-query.json)。官方入口：[默认分支 API](https://api.github.com/repos/torvalds/linux/git/ref/heads/master)、[固定提交](https://github.com/torvalds/linux/commit/58161ead29e50d70a317ea26b472e0c7508e69da)。

本次 HEAD 与上一轮相同，复用上一轮 A-mainline 的 18 个源码文件原字节，补取 7 个配置／页表生命周期文件。25 个文件的 SHA256、Git blob SHA1、API base64 与本地字节均核对一致，详见 [manifest.json](manifest.json)。复用文件的原查询时间和原目录分别保留，未伪装为重新下载。浏览器接口的访问失败及成功 HTTPS API 替代见 [查询说明](queries/browser-tool-notes.json)。这是 ARS `deep-research / fact-check` 的源码事实核查，由 Codex 直接核读；未调用 DeepSeek／Claude。

## 1. 主线模式的编译与运行条件

必须分开三件事：

- **策略选项默认值**：`drivers/iommu/Kconfig:96–100` 对 X86／S390 以外的架构默认 `IOMMU_DEFAULT_DMA_STRICT`；strict 与 lazy 的 help 分别等价于 `iommu.passthrough=0 iommu.strict=1/0`。见 [Kconfig:96–134](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/Kconfig#L96-L134)。
- **DMA-IOMMU 是否编入**：同文件 `153–159` 的 `IOMMU_DMA` 是隐藏 `def_bool ARM64 || X86 || S390`；RISC-V 驱动的 `Kconfig:4–12` 只选择 IOMMU_API、GENERIC_PT、IOMMU_PT 和 RISCV64 页表，没有选择 IOMMU_DMA；本次核读的 `arch/riscv/Kconfig`、`arch/riscv/configs/defconfig` 也无 IOMMU_DMA 条目。若最终 `CONFIG_IOMMU_DMA=n`，core 明确将默认域强制为 IDENTITY。见 [IOMMU_DMA 定义](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/Kconfig#L152-L159)、[RISC-V 选择](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/riscv/Kconfig#L4-L12)、[core:1915–1925](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/iommu.c#L1915-L1925)。本次未生成完整 `.config`，不宣称已经验证整个 defconfig 的最终解值。
- **启用 DMA-IOMMU 后的选择**：`iommu.c:47,808–816` 初始化 strict bool 并注册通用 `iommu.strict` early 参数；`228–256` 在 translated 且非 strict 时选 DMA_FQ。RISC-V ops 本身没有强制默认域类型的 callback（`1506–1515`）。因此条件模型需明确 `CONFIG_IOMMU_DMA=y`、translated 域及 strict／lazy 选择，不能称为已运行的原样 RISC-V 默认内核轨迹。文档 `kernel-parameters.txt:2680–2693` 的架构标签仍未列 RISC-V，通用 parser 源码本身没有这个架构 guard。见 [初始化与选择](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/iommu.c#L228-L256)、[启动参数 parser](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/iommu.c#L808-L823)。

**主线 RISC-V 页表通路具备 FQ 能力**：驱动 `iommu.c:1335–1394` 使用 generic_pt，后者 `iommu_pt.h:1232–1243` 设置 `domain->is_iommupt=true`；`dma-iommu.c:652–658` 对 iommupt 域直接返回支持 FQ，不需要 RISC-V `.capable` 声明 DEFERRED_FLUSH。FQ 初始化分配失败时仍回退 strict（`717–721`）。见 [FQ 能力判断](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/dma-iommu.c#L652-L658)、[失败回退](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/dma-iommu.c#L715-L721)。本地旧驱动缺少能力位的结论不能套到本版本。

## 2. strict unmap 的精确命令

普通物理页 DMA 映射的 `__iommu_dma_unmap` 先向 IOVA 粒度对齐，清页表，再在非 queued 情况立即同步，最后归还 IOVA：`dma-iommu.c:823–843`。generic_pt `iommu_pt.h:1066–1104` 清 leaf；仅当 unmap 范围完整覆盖某个下级表时才拆该表并把它加入 freelist。逐页 unmap 并不保证每次都释放一个下级表。core `iommu.c:2780–2799` 实际调用 generic_pt `unmap_range`，`iommu_pt.h:1109–1129` 将改动送入 gather。见 [DMA unmap](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/dma-iommu.c#L823-L843)、[清 PTE 与拆表](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/generic_pt/iommu_pt.h#L1066-L1129)。

RISC-V `iommu.c:960–1048` 的计算与优先级如下；`end` 为 inclusive：

1. `combined = leaf_levels_bitmap | table_levels_bitmap`；`non_leaf = table_levels_bitmap != 0`。没有层级信息时标记宽失效。
2. 计算最小覆盖 `[start,end]` 的自然对齐幂次范围：`range_sz_lg2=fls64(start ^ end)`，`range_addr=start & ~(2^range_sz_lg2-1)`；跨度达到 64 位时改宽失效。
3. 最低改动层决定 `stride_lg2=12+9*__ffs(combined)`（`__ffs` 是最低置位 bit 的零基索引）；`num=(end-start+1)>>stride_lg2`。**`num==0` 或 `num>512`** 标记逐地址路径使用宽失效。它不是“unmap API 次数超过 512”，也不固定以 4KiB 页计；最低层为 level0 时才是 4KiB stride。
4. 命令基底均为 `IOTINVAL.VMA`，`PSCV=1`、本域 PSCID，初始 `AV=0`。若改过 non-leaf 且无 CAP_NL，直接发这个域内宽失效。
5. 其余情况若 CAP_S 且 `range_sz_lg2>=13`，优先发 **一条** `AV=1,S=1` 的 NAPOT 范围命令；必要时设 NL。**这条路径优先于 `num>512` 标记**，所以支持 S 的硬件可在大于 512 个 stride 时仍用范围命令。
6. 没走 S 路径时：`single.use_global` 为真则发 `AV=0`；否则逐 stride 发 `num` 条 `AV=1,S=0`，必要时设 NL。单个 4KiB leaf、没有拆表时通常是一条按地址命令。

这里源码变量的 `global` 仍带 PSCV／PSCID，并非跨所有 PSCID。官方：[计算与命令分支](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/riscv/iommu.c#L960-L1048)、[命令字段构造](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/riscv/iommu-bits.h#L716-L756)。

驱动向绑定此域的每个不同 IOMMU 发命令，随后逐 IOMMU 发 `IOFENCE.C(PR=1,PW=1)` 并等待 CQ consumer 越过该命令；成功同步后释放 gather 中被拆掉的页表页。见 `riscv/iommu.c:1051–1112,1255–1274,517–535,367–382` 与 `iommu-bits.h:766–771`：[域广播及同步](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/riscv/iommu.c#L1051-L1112)、[IOFENCE helper](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/riscv/iommu-bits.h#L766-L771)。`RISCV_IOMMU_IOTINVAL_TIMEOUT=90000000us` 是异常等待上限，不能当正常刷新延迟；sync 的错误路径只记录错误且为 void 返回，下面的时序模型限于命令成功完成。

## 3. map 是否刷新

向合法空槽建立普通 4KiB 映射时，无单独的 IOTLB 刷新：`__iommu_dma_map` 调 `iommu_map`；core 的 `iommu_sync_map` 在没有 `.iotlb_sync_map` callback 时直接成功；RISC-V paging ops 没有这个 callback。见 `dma-iommu.c:846–874`、`iommu.c:2667–2674,2708–2721`、`riscv/iommu.c:1335–1341`：[通用 map 同步](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/iommu.c#L2667-L2721)。

有一个必须保留的例外：generic_pt map 用大 leaf 替换空的下级页表时，会把旧表挂入 gather，并在 map 返回前 `iommu_iotlb_sync`，刷新可能引用旧表的 walk cache。见 `iommu_pt.h:519–564,995–1000`：[替换空表](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/generic_pt/iommu_pt.h#L519-L564)、[map 的条件同步](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/generic_pt/iommu_pt.h#L993-L1000)。页表写入的非一致性 CPU cache 维护另见 `24–40,630`，与 IOTLB invalidation 分开。

## 4. FQ 容量、触发与时间

| 项目 | 本 commit 的精确行为 |
| --- | --- |
| 通常配置 | 每 CPU 每 DMA cookie 一条 256 槽 ring；保留一槽区分满／空，最多 **255 条**待回收 entry |
| 特殊配置 | `dev->iommu->shadow_on_flush=true` 才选单条 32768 槽 ring，最多 32767 entry，timeout 1000ms；RISC-V 驱动本身没有设置该标志 |
| 默认 timeout | 10ms，经 `jiffies + msecs_to_jiffies(timeout)` 安排；受 HZ 和定时器调度影响，不保证恰好 10ms 执行 |
| entry 单位 | 一个 IOVA range，含 `pfn`、`pages`、被拆页表的 freelist、flush counter；一条可以含多页 |
| 首次入队 | 若 cookie 的 timer 没在运行，则开启它；后续入队不反复把期限向后延 |
| 满触发 | 入队前先回收已经覆盖的旧 entry；仍 full 才做一次 `flush_iotlb_all`、更新完成计数、回收本 ring，再放入当前 entry |
| timer 触发 | 清 timer_on，刷新该域一次，再扫 single ring 或所有 possible CPU ring 回收 |
| 刷新命令 | `flush_iotlb_all` 人工构造完整地址、leaf/table bitmap；RISC-V 最终每个关联 IOMMU 发一条本 PSCID、AV=0 的 VMA，加 IOFENCE.C 并等待 |

原文：`dma-iommu.c:98–128,142–247,637–648` 的 [FQ 常数与环](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/dma-iommu.c#L98-L247)、[选项初始化](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/dma-iommu.c#L637-L648)；`riscv/iommu.c:1255–1265` 的 [flush-all gather](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/riscv/iommu.c#L1255-L1265)。不要将这里的 IOVA flush queue 与 RISC-V 硬件 fault queue 的 `RISCV_IOMMU_DEF_FQ_COUNT=4096` 混用。

单 CPU、没有其他刷新／timer 插入的条件下，前 255 次入队占满 ring，**第 256 次** unmap 的入队动作先刷新前 255 条，再把第 256 条作为新 entry 留下。当前 unmap 的 PTE 已在这次 full flush 前清除，但它入队时记录新的 start counter，因此仍会保守等后续一次覆盖其 counter 的 flush 才回收；不能把本次 entry 也直接归还。其他 CPU 的 domain flush 可使本 ring 的旧 entry 提前具备回收条件，而实际执行回收仍等下一次本 ring enqueue 或 timer 扫描。

## 5. 从 dma_unmap 到 IOVA 再用的生命周期

1. `__iommu_dma_unmap` 读取 `cookie->fq_domain` 决定 `gather.queued`，先清 PTE。generic_pt 在 queued 时**跳过中途 IOTLB sync 和 range gather**，只保留修改层 bitmap 并把被拆页表挂到 freelist；这是真正的延迟失效。见 `dma-iommu.c:830–843`、`generic_pt/iommu_pt.h:68–110`：[queued 处理](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/generic_pt/iommu_pt.h#L68-L110)。
2. `queue_iova` 的 `smp_mb` 在页表更新后，entry 记录 `fq_flush_start_cnt`。每次刷新严格按 `start_cnt++ → flush_iotlb_all callback → finish_cnt++` 顺序；仅 **`entry.counter < finish_cnt`** 可以回收。相等时不能回收。见 `dma-iommu.c:142–178,206–239`。
3. 回收时先 `iommu_put_pages_list`，再 `free_iova_fast`。前者最终 `folio_put` 归还页表页；后者先尝试把 IOVA 放入 rcache，失败再从红黑树释放。FQ 持有期间 IOVA 不在可复用 rcache 中，并保持其分配占用。见 `dma-iommu.c:149–161`、`iommu-pages.c:90–130`、`iova.c:416–430`：[回收顺序](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/dma-iommu.c#L142-L161)、[IOVA 归还](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/iova.c#L416-L430)。
4. 进入 rcache 后即可供 `alloc_iova_fast` 再分配，即使红黑树节点暂未移除；无需等整个 rcache 排空。同 CPU、同大小／limit 且无竞争时 magazine 的 push／pop 可马上复用刚归还的 IOVA。真实顺序受 CPU、尺寸、limit、在途分配及 FQ 回收批次影响。见 `iova.c:377–412,654–674,766–816`：[先查 rcache 的分配](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/iova.c#L377-L412)、[magazine 的末端再用](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/iova.c#L654-L674)。普通分配失败会冲刷 rcache 后重试，本通路没有通过分配失败来主动完成 FQ 的步骤（同处 `396–409`）。

**资源要分账**：entry 的 freelist 是 IOMMU 页表页，FQ held IOVA 不是 payload RAM。`iommu_dma_free` 在可能只入 FQ 的 `__iommu_dma_unmap` 后立即调用数据页释放；见 `dma-iommu.c:1594–1618`：[数据页生命周期](https://github.com/torvalds/linux/blob/58161ead29e50d70a317ea26b472e0c7508e69da/drivers/iommu/dma-iommu.c#L1594-L1618)。不能把待刷新 IOVA 范围直接乘页大小当物理内存保留。

可实现的正常单 CPU 主线路径为：strict 每次清 PTE 后完成按上述能力／层级选择的失效，再归还 IOVA；FQ 每次清 PTE 后保留 IOVA 与拆表页，按 255 条实际容量或 10ms timer 触发域刷新，完成 counter 条件后再归还。刷新和回收完成前，普通 map 不会重新取得这个 FQ-held IOVA。IOTLB 中是否仍驻留该死翻译还取决于硬件替换，不能把 FQ entry 存在等同于缓存必命中。

本页只证明固定版本源码行为。没有生成完整内核配置、构建／启动 Linux、测 CQ 时延或运行板测；实际设备 group、PSCID、S／NL capability、CPU 数和 HZ 未观测。没有改生产源码、共享状态文档或旧研究结果。
