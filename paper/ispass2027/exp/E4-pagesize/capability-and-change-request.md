# E4：2MiB映射能力核查与最小变更请求

## Material Passport

- 2026-10-17；状态：仅源码核查/变更请求，**4KiB/2MiB对照未测**。
- 固定基座：baseline100 Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`、CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`。
- 未改内核/driver/RTL；本项需要软件变更，由用户/Root决定是否继续；不因其他获准批次顺带实施。

前缀沿用 [E2定义](../E2-device-scaling/capability-and-plan.md)，原文见 [source-evidence.json](source-evidence.json)。

实际RTL有2MiB叶项支持：`HW/src/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:191–212`记录is_2M，`492–493`校验superpage物理对齐；这仅证明源码路径存在，不是本轮2MiB板测。Linux generic IOMMU的`SW/src/linux/drivers/iommu/riscv/iommu.c:2023`宣告SZ_4K|SZ_2M|SZ_512M；`io_pgtable.c:29–40,90–104,131–175`按pgsize在相应层建立叶项。

**现有idma实验接口不能选择2MiB。** `SW/src/linux/drivers/misc/idma.c:295`把pages定义为4KiB分配页数；`316–322`把显式`PAGE_SIZE`传给每次iommu_map；`270–272`也按PAGE_SIZE解映射，`648–649`要求内核PAGE_SIZE为4KiB。`185`的2MiB IOVA对齐、`DMA_ATTR_FORCE_CONTIGUOUS`或`--victim-pages 512`都不能改变这些4KiB叶项。CPU hugepage/虚拟地址步长也不是IOMMU叶级证据。

最低软件请求为新独立实验参数/ABI选择**IOMMU map page size**（4096或2097152），保持COPY chunk=4096及相同bytes/访问序列，保留旧ABI默认4KiB。需要：

1. 分离`buffer_bytes`、`mapping_page_bytes`和`copy_chunk_bytes`，源/目的每段长度均为map页大小整数倍；4KiB/2MiB比较使用完全相同buffer_bytes、工作量与地址序列，不能把1×4KiB和1×2MiB直接称同足迹。最终大小由Root按内存预算选定，每点DS0×10次。
2. IOVA与实际PA都校验2MiB对齐且映射范围物理连续；分配不满足则在DMA启动前报错，不默默退回4KiB。当前内核`linux-build/.config:680`没有CMA，不能凭分配接口名称保证任意大缓冲成功。仍使用平台允许的DMA缓存交接，不用用户态/dev/mem绕开生命周期。
3. 以所选pgsize做map、失败回滚与unmap；将实际IOVA/PA、bytes、请求/实际叶级输出到原manifest/JSON。generic `iommu_iova_to_phys()`返回地址不足以证明叶级，需要只读页表walk获取leaf shift（已有内部`riscv_iommu_pt_walk_fetch:107–128`可回传leaf_shift，当前不是实验可调用的公开接口）。逐段验证4KiB leaf shift12或2MiB leaf shift21，不修改运行期页表。
4. 验证对齐拒绝、叶级/地址翻译一致、跨大页边界及末尾、失败/排空后的映射清理、原4KiB ABI回归；完成后才能构建新Linux并按用户决定上板。现有BIT可复用，是否需要RTL修改要以实际验证为准，不预设已通过。

延迟分布能力与页大小独立，见 [E5](../E5-latency/capability-and-change-request.md)。新大页图可以先做吞吐/既有stage mean/max；p50/p99目前仍缺采集能力。没有发出本项硬件或内核实施命令。
