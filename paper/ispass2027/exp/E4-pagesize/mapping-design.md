# E4：有界4KiB/2MiB映射设计，待Root审查

2026-10-17。用户已授权后续论文实验的独立软件/RTL分支准备与构建，Root要求先审本设计再修改低层mapping。本文件仅设计；未改低层源码、未构建新payload、未上板。

顶层独立worktree `/home/xvhaocheng/work-a3-pagesize`、分支 `ispass/a3-pagesize` 从main `f1917a2`创建；仅取回已接受4746290的E4报告/证据。Linux独立worktree `/home/xvhaocheng/work-a3-pagesize-linux`、同名分支，从共同软件稳定提交 `acece3a3ab7d72ba1f936c1984b82a4a55f28d46`开始。冻结E2/P0工作树和镜像不追加E4。复用实际DS0 BIT；原硬件能力证据在 [source-evidence.json](source-evidence.json)，本设计的当前软件原文在 [mapping-source-evidence.json](mapping-source-evidence.json)。

## 固定比较与实际工作

两条件只改变测量IOVA的IOMMU leaf：4096B或2097152B。V DMA0、A DMA1，各有源2MiB、目的2MiB，总DMA buffer footprint恒为8MiB。两边都用同一 `dma_alloc_attrs(...DMA_ATTR_FORCE_CONTIGUOUS)` 方法申请四个2MiB物理连续分配，并要求PA/测量IOVA均2MiB对齐。4K条件不能改成零散分配，2M条件不能减少buffer。既有DMA API分配自身的映射与缓存属性保持两条件相同；DMA实际使用独立测量IOVA。

两条件V/A的batch配置、4KiB COPY、4KiB步长、512页环遍历、V4096次和warmup256完全相同，V测量遍历完整buffer八次。A使用相同自主长目标、在V之前启动，在V结束后STOP并完整排空。两leaf条件的A最终完成数可以不同，按实际记录；它包含startup/drain，不称相同实际A完成量或精确V窗吞吐。A的连续运行、V的精确硬件窗口以及全部原有数据/排空/observer检查保留。先准备2条件×10=20点DS0轮转；本设计不默认加其他footprint、Solo或混合leaf组合。

## 最小新接口与范围

保留ABI1、P0 ABI2、multi ABI3的参数、布局和默认4K映射。新增独立 `idma_pages.h`/ABI4，建议CAPS `0x0d`、RUN `0x0e`（Root确认后固定），仅接受上述V0/A1、两种leaf和固定工作。输入显式区分 `buffer_bytes=2MiB`、`mapping_page_bytes={4KiB,2MiB}`、`chunk_bytes=4096`；不复用旧CLI的pages含义。新独立CLI/collector，不向旧JSON塞新语义。

请求包含旧pair结果、两lane生命周期mask/状态和四个buffer的映射证据。每buffer输出IOVA、base PA、bytes、requested map size、实际leaf shift min/max、已验证4K子页数、first/last leaf raw PTE及leaf IOVA base、map/unmap完成bytes、解除映射后的空闲验证状态。完整布局/静态断言在Root接受本设计后单批交付。错误时也输出所有已经形成的证据和原errno；不以成功flag隐藏部分准备或清理结果。

低层预期修改限：`drivers/misc/idma.c`、新UAPI/header；RISC-V driver `iommu.c`/`io_pgtable.c`与内部header增加只读leaf查询，以及新的窄公共查询header。不改DMA allocator、CMA、DTS、内存/地址约束、BIT或IOMMU map/unmap算法。保持此前缓存交接与STOP/drain/quarantine机制。

## 真实leaf证据

现有 `io_pgtable.c:107–128`的 `riscv_iommu_pt_walk_fetch`已经在实际叶处返回 `leaf_shift`；`209–225`只读地址转换使用它拼接正确offset。新增窄接口拟为：

```c
struct riscv_iommu_leaf_info {
    u64 raw_pte;
    phys_addr_t phys;
    unsigned long leaf_iova_base;
    u32 leaf_shift;
};
int riscv_iommu_query_leaf(struct iommu_domain *domain, unsigned long iova,
                          struct riscv_iommu_leaf_info *out);
```

`iommu.c`先验证domain非NULL且 `domain->ops == &riscv_iommu_domain_ops`，之后才能container_of；拒绝identity/blocked/SVA、未初始化root、nested/G-stage等本轮不支持的上下文。DMA/DMA_FQ/UNMANAGED须按实际domain模式记录；本轮要求单stage已初始化分页。内部helper复用现有walk，读取实际PTE，检查present、R/W叶及shift，只读返回raw/PA/leaf base；缺映射返回ENOENT，类型不符返回EOPNOTSUPP。CONFIG不可达有失败stub，不输出伪造leaf。

调用者必须持有映射生命周期并排除该范围的map/unmap；这是实验期间的只读查询，不宣称任意并发修改安全。idma全局experiment mutex、已持device/domain引用和独占测量IOVA保证本次范围稳定。既有domain mutex没有覆盖所有map/unmap（`iommu.c:1804–1843`直接分派），不能把它虚称页表保护锁，也不为本项引入跨模块锁协议。

在任何DMA启动之前，逐buffer遍历512个4K子页；每点查询实际leaf，要求shift精确12或21、PA精确base+offset、leaf base对应所选粒度，并另查最后字节确保末尾offset正确。4K条件必须512个4K叶，2M条件必须同一2M叶覆盖全段；任意混合/回退/missing/不连续立即拒绝。记录全部检查计数和first/last原始PTE，不以 `iommu_iova_to_phys` 地址正确代替leaf证据。在所有lane完整排空、解除映射前重复相同验证；该检查在计时窗外。硬件PTW已有2M支持仍仅是源码能力，最终要由真实运行的DMA正确性和证据验收。

## 分配、map/unmap与失败闭合

实际 `dma-iommu.c:1446–1517`在FORCE_CONTIGUOUS时走连续页分配；`include/linux/dma-map-ops.h:159–169`在CMA关闭时返回NULL后由 `alloc_pages_node(get_order(2MiB))`分配，并按相同order释放。既有内核CONFIG_CMA=n，不修改它。逐4K CPU页核对PFN连续及PA2M对齐，覆盖remap返回CPU地址情况。任一分配或约束不满足就保留失败；不缩buffer、不改成4K leaf、不给额外board回验或重试。

新E4路径使用独立checked mapping状态（每方向累计mapped bytes、所选pgsize、是否E4），旧 `idma_stream_prepare/cleanup` wrapper保持旧行为。沿用已存在接受pgsize参数的map/unmap方法，但新路径不可依赖旧void unmap忽略返回的做法：

1. 分配并初始化四个buffer，取同一既有高端2M对齐IOVA搜索；对每方向按所选pgsize调用 `iommu_map`，每次成功才推进mapped bytes。第一次失败保留原errno和已成功prefix，交统一清理，不丢失部分映射的状态。
2. 准备失败且DMA未启动，可直接按已成功prefix清理。已启动/中途失败首先对所有已启动lane STOP并逐lane drain；无法证明quiet的lane按原机制retain/quarantine，其全部buffer和映射不解除、不释放。
3. quiet lane对每个已映射粒度调用 `iommu_unmap`并核对实际返回bytes；即使一处失败也尝试其余已知range，记录已解除bytes。再逐4K检查整个曾占用范围不存在映射。短unmap或残留映射使lane retain/quarantine，不释放对应DMA分配，原运行错误优先。
4. 仅在该lane确实quiet且全部手工测量映射已解除后，以原 `dma_free_attrs`释放两个原始分配及其DMA API映射。无映射的半初始化分配可以安全释放。所有device引用、context和HPM所有权按原路径闭合；retain的DMA内存不能因释放context而被误释放。

这会使用小的共用prepare控制helper与旧wrapper，避免复制/弱化低层完成处理。若实现发现需要改变旧ABI的安全行为，先给Root精确diff，不顺带重构。

## 有限验收与排队

先做实际只读walk函数的4K/2M/missing/末尾/类型拒绝有限页表模型，必须检查真实PTE和shift；再实际driver函数桩测试两种粒度、所有分配/map位置失败与prefix、PA/IOVA不齐、leaf不符、短unmap/残留、zero-ID/retain、每lane drain失败、原错误优先、全部旧38 pair/P0和multi57场景回归。验证应证明各生命周期状态，不用“新函数返回预期常数”代替真实函数。

随后RV64对象编译（idma/iommu/io_pgtable）、UAPI静态断言、CLI静态编译、kernel栈大小检查；Root直接审关键diff/原始回执后再冻结新的E4软件包。普通对象编译可以自主进行，正式payload按共同构建流程独立身份。Root按现有DS0排程运行20点；连续内存不足产生的原始失败保留，不能作为重采理由。E2/P0冻结包和此前数据保持各自身份。
