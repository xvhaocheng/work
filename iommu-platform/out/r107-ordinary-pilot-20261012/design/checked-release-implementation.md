# stage111：actual leaf与checked release实现交接

作者：Codex Sol/ultra。本组件已写入新隔离source，尚未由root接受或编译。没有修改production、旧freeze、idma.c或UAPI；没有设备操作/新模型执行。可应用小补丁为本目录`checked-release.patch`，完整实际静态命令、hash与结果在`checked-release-implementation.json`。

## 实际接口和实现入口

- `source/linux/include/linux/iommu-finite.h:12`：root指定的leaf/allocation类型；`:24`起记录全部caller contract。iova/pa均为actual leaf base；tables[]为root到leaf所在表的真实PA，unused/reserved清零。CONFIG_RISCV_IOMMU=y才启用helper；disabled/module构建用EOPNOTSUPP stubs，避免built-in dma-iommu引用module-only provider造成未定义符号。
- `source/linux/drivers/iommu/riscv/io_pgtable.c:233`：`riscv_iommu_finite_leaf`。先用已有typed endpoint入口核dev，要求caller HPM session存活；按attach既有顺序取domain.lock→ep.lock，核live domain/DC/root、hostSv39、nonS2/nonATS/nonPASID、同一domain恰好一个endpoint和当前实际page-table ops。PGDIR_SHIFT=30、PAGE_SHIFT=12是本平台明确支持范围，不从请求猜页型。沿原树逐表READ_ONCE rawPTE，用_PAGE_PFN_MASK/_PAGE_PFN_SHIFT得到下一表PA和actual leaf PA；只有真实zero PTE返回ENOENT，非法reserved/权限/超页PA对齐/循环表/域变化分别返回其他errno。
- `source/linux/drivers/iommu/dma-iommu.c:1632`：`iommu_dma_free_finite_checked`。nr限定1..8，process context、正常iommu_dma_ops、无bypass/coherentoverride、DMA域/IOVA cookie、无FQ、页对齐/范围不溢出。预审全部记录的CPU/IOVA范围不重叠、vmalloc对象为exact VM_DMA_COHERENT区域/size、IOVA reservation恰好覆盖本extent；再核每actual leaf全部落在extent内、V/R/W且无X、CPU每页PA等于实际leaf PA、全batch backing无重复页。allocator padding或foreign extent明确拒绝。

## 实際释放顺序

1. 全部预审完成后再开始撤PTE；每次重读actual leaf与预审snapshot逐字节相同，只按actual leaf完整span调用iommu_unmap_fast，要求exact返回。
2. 按每个API extent的全部4KiB位置再次walk，只允许ENOENT；EOPNOTSUPP/ESTALE/EPROTO不能混成已撤。
3. `dma-iommu.c:1804`才调用checked_sync。caller callback必须先发布PTE stores，再执行既有checked global host-S1 invalidation/fence；不能改变binding或提前释放HPM。
4. callback成功后核domain/ops/FQ仍不变；`:1816`开始归还IOVA reservation，再做正确signature的debug_dma_free_coherent和__iommu_dma_free CPU remap/backing释放。不调用dma_free_attrs或__iommu_dma_unmap，不二次撤PTE。

所有非零路径均不归还IOVA、不释放backing、也不改items；PTE可处于部分或全部已撤状态。root须持久保留owner/items、标quarantine，不能重试helper或重map。成功也不改原items，root把它们只作released receipt并更新owner状态，不能再调用。

root已明确接受GFP_KERNEL provenance由独占内核finite owner保证；这些items只能来自其exact成功dma_alloc_attrs记录。helper无法通用只读识别atomic pool，header明确不作该保证，禁止foreign/user-supplied kernel pointers及NO_KERNEL_MAPPING。HPM mutex当前task拥有、device/domain稳定和owned extent的其他map/unmap exclusion仍为caller必要前提；mutex_is_locked加lockdep断言不能冒称代替caller生命周期管理，domain.lock也不阻旧map回调。

## 关键原始来源

- unchanged `design/dma-iommu.before-finite.c:707-727`显示旧__iommu_dma_unmap在void sync后马上归还IOVA；`:1439-1443`显示旧.free马上释放backing。本新增路径不修改它们。
- `source/linux/drivers/iommu/riscv/iommu.c:2047-2063`的endpoint入口先核fwspec ops/type/DID；`:2108-2143`给当前checked hostS1/DC/root限制；`:2148-2162`保留当前quiesced no-ATS/no-S2 fence证据范围；`:2164-2295`执行checked错误代际/post/fence核验。
- `design/io_pgtable.before-finite.c:178-206`实际leaf unmap只清一项PTE并按传入粒度前进，没有退页表页freelist；core `source/linux/drivers/iommu/iommu.c:2423-2433`按请求extent选pgsize，所以新增路径必须逐actual leaf，不能直接整extent撤后只信returned bytes。
- `source/linux/kernel/dma/debug.h:31-32`和`:88-90`亲核signature为(dev,size,void*virt,dma_addr_taddr)，此kernel头在kernel/dma而非include/linux。CONFIG_DMA_API_DEBUG关闭时为inline stub。
- `source/linux/kernel/dma/remap.c:22-30,38-54,60-69`亲核普通DMA CPU remap区域及free所需exact VM_DMA_COHERENT flag；新增路径预审一致。

## 已执行静态检查及边界

12项实际静态源码断言通过：8个旧关键函数body逐字节不变、旧iommu_dma_ops表不变、revocation→callback→IOVA/debug/backing释放次序、没有旧free/第二次unmap调用、items未改写。Linux checkpatch针对本补丁0错误/0警告/0checks；raw patch没有commit metadata，因此只禁用了signoff检查并忽略COMMIT_MESSAGE，没有伪造签名。`patch --dry-run -R`对这三个新隔离文件通过。

这些是静态检查，不是编译或功能验收。root需直接核实际代码、接入callback/owner失败处理，再构建及执行必要检查。Allocation在DMA API返回NULL之前的内部失败回滚不由本release接口承诺留存；源码原路径未改，这个已登记未知仍保留。
