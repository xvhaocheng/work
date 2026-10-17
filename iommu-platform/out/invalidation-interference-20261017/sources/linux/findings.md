# Linux 失效、域与 IOVA 来源核查

PSCID 是一阶段地址空间标识；GSCID 是客体二阶段地址空间标识；AV/PSCV/GV 分别表示命令按地址、按 PSCID、按客体上下文限定范围；FQ 是延迟刷新队列；CQ 是命令队列。本文只讨论正常 host 一阶段 DMA API，SVA、ATS、attach/teardown 和 guest nesting 另行标注。

**最新主线已按 PSCID 限定普通失效，并经 generic_pt 支持 FQ；开发分支和本地平台仍发不限定 PSCID/地址的 host 一阶段宽失效。两个平台设备默认各有独立域，也不能挡住后一种宽失效。**这是源码结论，尚无本轮运行数值。

## 查询版本

| 版本 | 固定身份 | 查询/本地状态 |
| --- | --- | --- |
| A，torvalds/linux 默认 `master` | `58161ead29e50d70a317ea26b472e0c7508e69da` | 官方 API 于 2026-10-17 06:41:50 UTC／15:41:50 JST 返回的 ref；默认分支另经仓库 API 确认 |
| B，malejo97/linux `wip/riscv-iommu` | `d371fb0f95d5bae20af8135f976ffd8a06bfaa23` | 同时查询官方 API；查询原响应、HTTP headers 和 UTC/JST 起止时间均保留 |
| C，平台实际 Linux | `f215540ad19dfe88c91b43b82c09cf40c514f323` | 读取时 `codex/organized-current-20261017`，dirty=false；选定源文件均另存 SHA256。干净工作树包含既有本地扩展，不等于上游原样版本 |

原始查询在 [queries/](queries/)，三个版本分别在 [A-mainline/](A-mainline/)、[B-development/](B-development/)、[C-platform/](C-platform/)。初次 `git ls-remote` 超时，没有取得有效返回；raw.githubusercontent.com 多项返回429，随后通过官方 GitHub Contents API 取得固定 commit 的原字节。失败记录保留，没有把失败请求写成成功。API 原始 `Date` headers 与客户端查询时刻分别记录；这里固定的是该次 API 响应返回的 HEAD，不持续追随随后提交。

## 横向比较

| 普通 DMA 路径 | A 最新主线 | B 开发分支 | C 本地平台 |
| --- | --- | --- | --- |
| 4KiB leaf unmap | `IOTINVAL.VMA`，PSCV=1、GV=0；可按地址 AV=1 | `IOTINVAL.VMA`，AV=PSCV=GV=0 | 同 B |
| 地址失效退回整体 | 没有层级信息、计数为0或单地址命令数>512等情况下，退回该 PSCID 的 AV=0；非leaf变化且硬件无 NL 也退回。支持 S 时优先使用适当的 NAPOT 范围命令，不能把512说成无条件字节阈值 | PSCID/地址调用都被注释，全部范围直接宽失效；没有有效的按范围大小切换阈值 | 同 B |
| 延迟刷新能力 | 没有 `.capable`，但 generic_pt 域被视为支持 FQ；非strict翻译默认可启用，分配失败仍回退 strict | capable 不返回 DEFERRED_FLUSH，DMA_FQ 初始化回退 DMA | 同 B |
| 默认设备分组 | platform 每设备一组；PCI 根据 alias/ACS/拓扑分组 | 同 A 的基本规则 | 同 A 的基本规则 |
| 普通一阶段域标识 | 每分页域分配 PSCID，范围1…2²⁰−1 | 每域经匿名 PASID 分配器取得 `domain->id`，写入 PSCID | 每域 PSCID 独立分配，范围2¹⁹…2²⁰−1，与进程 PASID 区间分开 |

A 的地址模式还受实际 gather 的 leaf/table 层级和硬件 S/NL capability 影响；表中不是承诺所有4KiB unmap均只发一个按地址命令。启动参数/编译配置决定 strict、lazy 或 passthrough，不能仅凭仓库源码断言一台实际机器正在用哪种域。位组合的规范范围由本轮规范核查单独给出。

## 精确源码入口

1. **B/C 当前宽失效。**B [riscv/iommu.c:1722–1788](B-development/drivers/iommu/riscv/iommu.c#L1722-L1788)，C [1777–1845](C-platform/drivers/iommu/riscv/iommu.c#L1777-L1845)。C 1803构造VMA，1804的 `set_pscid` 被注释，1807–1813的地址循环也被注释，1815只提交一次。C [333–369](C-platform/drivers/iommu/riscv/iommu.c#L333-L369) 显示构造器重置命令，只有 setter 才置 AV/PSCV/GV，因此普通 host 路径三位皆0。g_stage 分支发的是 `IOTINVAL.GVMA`＋GSCID，不是普通一阶段VMA。
2. **A 的阈值不能简化成“超过512页必全局”。**[A iommu.c:961–1002](A-mainline/drivers/iommu/riscv/iommu.c#L961-L1002) 用最低变更层计算 `stride_lg2=9*__ffs(combined)+12` 与命令数量，`!num || num>512` 设置单地址路径回退；[1005–1048](A-mainline/drivers/iommu/riscv/iommu.c#L1005-L1048) 先设置 PSCID，再处理无NL的非leaf退回、S支持的NAPOT范围、逐地址路径。这里代码命名的 `global` **仍保留PSCV=1**，不是跨所有PSCID。无S、仅4KiB leaf、对齐且连续时，512个4KiB单位对应2MiB，超过才退回；这个特例不能替代完整条件。[iommu-bits.h:716–763](A-mainline/drivers/iommu/riscv/iommu-bits.h#L716-L763) 给出AV/PSCV/GV、S/NL的具体构造。
3. **A 支持等价的 FQ 能力。**[A dma-iommu.c:652–658](A-mainline/drivers/iommu/dma-iommu.c#L652-L658) 对 `iommupt_from_domain(domain)` 直接返回true。[generic_pt/iommu_pt.h:1232–1244](A-mainline/drivers/iommu/generic_pt/iommu_pt.h#L1232-L1244) 置 `domain->is_iommupt=true`；[generic_pt/iommu.h:69–74](A-mainline/include/linux/generic_pt/iommu.h#L69-L74) 识别该域。RISC-V [iommu.c:1343–1396](A-mainline/drivers/iommu/riscv/iommu.c#L1343-L1396) 使用这套初始化，且 [1506–1516](A-mainline/drivers/iommu/riscv/iommu.c#L1506-L1516) 没有capable callback。不能因没有DEFERRED_FLUSH字样就判A不支持。失败回退见 [dma-iommu.c:717–721](A-mainline/drivers/iommu/dma-iommu.c#L717-L721)。
4. **B/C 则明确回退 strict。**B [capable:1167–1180](B-development/drivers/iommu/riscv/iommu.c#L1167-L1180)，C [1198–1211](C-platform/drivers/iommu/riscv/iommu.c#L1198-L1211) 仅返回缓存一致性/启动保护。B/C [dma-iommu.c:617–620](C-platform/drivers/iommu/dma-iommu.c#L617-L620) 在DEFERRED_FLUSH不足时把DMA_FQ改成DMA。C [708–727](C-platform/drivers/iommu/dma-iommu.c#L708-L727) 显示非queued unmap立即同步；[riscv/io_pgtable.c:180–209](C-platform/drivers/iommu/riscv/io_pgtable.c#L180-L209) 先清PTE并加入gather。
5. **每组默认一个域，并非同IOMMU就同域。**C [generic iommu.c:1521–1595](C-platform/drivers/iommu/iommu.c#L1521-L1595) 的platform callback每设备建组，PCI callback处理DMA alias、ACS和不能隔离的functions。C [2832–2898](C-platform/drivers/iommu/iommu.c#L2832-L2898) 为group分配并安装默认域；A同类入口 [1592–1685](A-mainline/drivers/iommu/iommu.c#L1592-L1685)、[3148–3208](A-mainline/drivers/iommu/iommu.c#L3148-L3208)。因此独立platform DMA或可隔离PCI设备的默认翻译域通常不同；共享PCI group则同域，同一domain被明确绑定给多个设备也是合法情形，不能假设所有不同设备一律不同域。
6. **域到标识的实际映射。**A [iommu.c:1382–1389](A-mainline/drivers/iommu/riscv/iommu.c#L1382-L1389) 分配PSCID，[1320–1330](A-mainline/drivers/iommu/riscv/iommu.c#L1320-L1330) 写到上下文。B [1401–1412](B-development/drivers/iommu/riscv/iommu.c#L1401-L1412)、[1507–1515](B-development/drivers/iommu/riscv/iommu.c#L1507-L1515)、[1571–1587](B-development/drivers/iommu/riscv/iommu.c#L1571-L1587) 分配并写 `domain->id`。C [82–86](C-platform/drivers/iommu/riscv/iommu.c#L82-L86)、[1533–1541](C-platform/drivers/iommu/riscv/iommu.c#L1533-L1541)、[1614–1622](C-platform/drivers/iommu/riscv/iommu.c#L1614-L1622) 将host的 `iohgatp=0`，并写TA.PSCID。**普通host一阶段不能被描述为“两个不同GSCID”**；GSCID用于明确启用guest二阶段的场景。B/C 的 `g_stage` 在域分配kzalloc后默认false，显式nesting才置true（B1710–1719；C1765–1774）。不同设备ID/DID也不能替代PSCID域身份。

## IOVA 不是必然“每个 I/O 都换新页”

三版都保留了：冷rbtree分配从 DMA/aperture 上界向下搜索，但 fast path 先取 per-CPU range cache；free又把IOVA放回cache。C [iova.c:303–325](C-platform/drivers/iommu/iova.c#L303-L325)、[429–489](C-platform/drivers/iommu/iova.c#L429-L489)、[697–718](C-platform/drivers/iommu/iova.c#L697-L718)；A对应 [247–269](A-mainline/drivers/iommu/iova.c#L247-L269)、[367–430](A-mainline/drivers/iommu/iova.c#L367-L430)、[654–674](A-mainline/drivers/iommu/iova.c#L654-L674)。C [dma-iommu.c:657–702](C-platform/drivers/iommu/dma-iommu.c#L657-L702) 还按设备DMA上限/aperture截断，PCI通常先试32位窗口。

**源码推导：**同CPU、相同4KiB大小和limit、无其他分配打断时，刚释放的页位于magazine末尾，下次pop可立即复用同一IOVA；两个buffer也可从这组页反复复用。不同CPU、批量在途、其他分配或flush queue持有未退役IOVA会改变序列。因此“逐次map/unmap”不等于“逐次全新IOVA”；单调递减的新页序列只能作为明确选择的工作负载，不能称默认fast path精确复现。

真实FQ也不是固定每16或64次flush：B/C [dma-iommu.c:87–90](C-platform/drivers/iommu/dma-iommu.c#L87-L90) 是256-entry ring和10ms timer；[148–220](C-platform/drivers/iommu/dma-iommu.c#L148-L220) 在满队列或计时到达时flush，并在完成计数满足之后才将IOVA归还rcache。A默认值仍256/10ms，但增加single/per-CPU选项（[99–103](A-mainline/drivers/iommu/dma-iommu.c#L99-L103)、[630–648](A-mainline/drivers/iommu/dma-iommu.c#L630-L648)）。N=16/64是有界批量近似，不能写成这三个版本的实际Linux默认批量。

## C 的 map/unmap 命令账

**成功map也会失效。**C [dma-iommu.c:730–753](C-platform/drivers/iommu/dma-iommu.c#L730-L753) 调 `iommu_map`；[generic iommu.c:2402–2419](C-platform/drivers/iommu/iommu.c#L2402-L2419) 对一次成功的region调用一次 `.iotlb_sync_map`；C [riscv/iommu.c:1848–1858](C-platform/drivers/iommu/riscv/iommu.c#L1848-L1858) 进入相同宽flush。这个callback按本次region调用，不是按内部PTE逐项回调；4KiB正常region没有多页gather额外同步。

在普通host-S1、ATS/PASID关闭、无其他lifecycle/故障且CQ提交成功的条件下：

| 操作 | IOTINVAL.VMA | IOFENCE.C | 同步helper调用 |
| --- | ---: | ---: | ---: |
| 一次独立4KiB map成功 | 1 | 2 | 2 |
| 一次对应4KiB strict unmap | 1 | 2 | 2 |
| 一次COPY有两个独立map＋两个unmap | **4** | **8** | **8** |

来源：C [1777–1834](C-platform/drivers/iommu/riscv/iommu.c#L1777-L1834) 在1815提交一次IOTINVAL，1817第一次同步，1822–1823跳过无PASID endpoint，但1833的第二次同步仍无条件执行。因此最后一行是**12条预期CQ命令**。fence构造 [372–376](C-platform/drivers/iommu/riscv/iommu.c#L372-L376) 设置AV=0、PR=PW=0；[571–575](C-platform/drivers/iommu/riscv/iommu.c#L571-L575) 调 `post_sync(true)`，实际同步在 [493–544](C-platform/drivers/iommu/riscv/iommu.c#L493-L544) 轮询CQH。这里没有新测量CQ接受数、时延或完成语义；满队列/提交失败、ATS、SG合并、跨页或错误回滚会改变命令账，不能沿用12。

本轮来源核查没有验证已有benchmark是否每COPY执行这四次API调用，也没有声称上述CQH轮询已证明所有硬件完成义务。当前仿真若只比较unmap失效而控制map侧不发命令，应明确是隔离unmap侧的测量；真实C全API行为另包含这里的map侧与两次fence开销。

## 保留的不确定项

- 未启动/识别真实内核或板上IOMMU，没有实际设备group、PSCID或IOVA逐笔轨迹；只能给源码默认规则与条件场景。
- A的S/NL capability实际组合、启动strict/lazy配置和gather层级决定精确命令；没有把2MiB/512特例外推全部范围。
- 规范对global PTE、非leaf cache与IOFENCE完成的精确义务由本轮规范核查给出；此文件不把Linux源码注释当规范替代品。
- C保留既有research/SVA helper，它们不是普通DMA API默认路径；C clean只证明读取时工作树身份，不表示本轮启用或运行它们。

完整文件SHA、原始查询及本文件引用的源行片段见 [manifest.json](manifest.json)。本次为Codex-only只读核查，无生产改动、编译、实验或板测。
