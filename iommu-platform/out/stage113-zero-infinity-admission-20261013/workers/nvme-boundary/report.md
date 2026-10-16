# stage113：已选 ZeRO-Infinity NVMe AIO → Linux DMA/IOMMU 边界

裁决：保留原生 tiled CPU-Adam swap 的来源结构和 request 级 DMA 生命周期；拒绝从 pinned 池推出持续 NVMe IOVA、从两个 AIO handle 推出两个 DID、从 unmap 推出每次全部 TLB 必失效或已测 loss。实际物理后端、翻译域、自然并行 peer、参数/H与运行完成证据缺失。本报告不准入 G0/G1，也不修驱动、不改生产、不实验。

## 范围与版本

- DeepSpeed v0.16.4 / `e2dc3eeb1923073e32739596a4fd051417d4ff92`，tree `bf322fe527f2d78b76232ea6cb5ad3e5757a1ef1`；19个已用源码blob与官方固定tree逐一匹配。
- 本地Linux `c0f5e13ea81fa30d78f3bb1d6389e2397321e040`，Makefile为6.5.0-rc2；18个审读源文件均与HEAD一致。旧dirty文件原样保留，见source-manifest.json。该源码副本不是所选NVMe工作负载实际运行Linux的证明。
- root独占stage3 optimizer计算/数值、完整byte contract、FPGA投影及最终接受；本席只处理AIO/内核/硬件身份边界。未调用DeepSeek、Claude或其它子席。
- 原始404 GET保存在source/parent-manifest.json；本席web open失败和一次不同方法定向恢复见failed-fetches.json。只增本worker目录内证据/报告；无生产改动、构建、测试、板操作。

## 核心结论与原始证据

### C01 固定CPU pinned池不等于持续NVMe DMA/IOVA映射

判定类别：`source_structure + conditional_deduction`。池的backing持有到allocator/tensor引用生命周期结束；free只是槽位可重用。NVMe payload的映射所有者与单位则是kernel request，并在completion unmap，因此两种生命周期不同。既不能把池常驻视为NVMe持久映射，也不能从每request unmap推出物理页在每tile被重新分配。

- E01: [deepspeed/runtime/swap_tensor/utils.py:186–189](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/utils.py:186)；SHA-256 `e035d79b036bf2a8d16df65185eea38a2021b02d47027dd193ecf1ae4c868cb3`；原文：`device='cpu'`。
- E02: [deepspeed/runtime/swap_tensor/utils.py:217–226](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/utils.py:217)；SHA-256 `e035d79b036bf2a8d16df65185eea38a2021b02d47027dd193ecf1ae4c868cb3`；原文：`self.free_buffer_index.append(self.used_buffer_index[b_id])`。
- E29: [drivers/nvme/host/pci.c:730–802](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:730)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`dma_map_bvec`。
- E30: [drivers/nvme/host/pci.c:792–818](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:792)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`dma_map_sgtable`。
- E31: [drivers/nvme/host/pci.c:540–562](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:540)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`dma_unmap_sgtable`。
- E32: [drivers/nvme/host/pci.c:961–980](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:961)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`nvme_pci_unmap_rq(req);`。

### C02 CPU页锁和bounce必须分别计价

判定类别：`source_structure`。selected CPU contiguous pinned/managed tensor可以直接作为AIO数据区；非CPU或未pinned输入可能先分配CPU bounce、write前copy、read finish时copy回并free。contiguous()对非contiguous输入还可能形成额外存储。managed page-aligned alloc只调用posix_memalign+mlock，析构munlock+free；是否另有GPU注册由runtime allocator决定，源码不足以推成NVMe映射。

- E03: [csrc/aio/py_lib/deepspeed_cpu_op.cpp:33–40](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_cpu_op.cpp:33)；SHA-256 `da4d3dfe5e92c01d7da6b902cd8caa4281b0eea582037b9e4614383cc9abcf2c`；原文：`_buffer.is_cpu()`。
- E04: [csrc/aio/py_lib/deepspeed_cpu_op.cpp:40–40](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_cpu_op.cpp:40)；SHA-256 `da4d3dfe5e92c01d7da6b902cd8caa4281b0eea582037b9e4614383cc9abcf2c`；原文：`_cpu_buffer.contiguous()`。
- E05: [csrc/aio/py_lib/deepspeed_cpu_op.cpp:45–65](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_cpu_op.cpp:45)；SHA-256 `da4d3dfe5e92c01d7da6b902cd8caa4281b0eea582037b9e4614383cc9abcf2c`；原文：`if (_buffer.is_cuda())`。
- E06: [csrc/aio/common/deepspeed_aio_utils.cpp:112–129](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/common/deepspeed_aio_utils.cpp:112)；SHA-256 `e399c475c416b4c4ca5e8fcb382b8495c8c02658f282dea303eed96b4419620b`；原文：`mlock(ptr, size)`。
- E07: [csrc/aio/py_lib/deepspeed_pin_tensor.cpp:14–20](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_pin_tensor.cpp:14)；SHA-256 `3296264457df3e07e472323fe2d8f0fa79c41629b2d70814c4517e1c2365e4ae`；原文：`munlock(iter->first, iter->second);`。

### C03 O_DIRECT是条件源码结构，不是DMA注册或持久化许可

判定类别：`source_structure + conditional_deduction`。通常read用O_RDONLY|O_DIRECT，write用O_WRONLY|O_CREAT|O_DIRECT；CANN条件去掉O_DIRECT。该open没有O_SYNC/O_DSYNC，选定async路径未见fsync/fdatasync。因此direct数据传输、device DMA映射和断电持久化是不同合同；write completion/close不能单独证明crash durability。实际filesystem、编译宏、direct-IO支持和文件alignment未知。

- E08: [csrc/aio/common/deepspeed_aio_common.cpp:267–283](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/csrc/aio/common/deepspeed_aio_common.cpp:267)；SHA-256 `e2234f5c6c23a47be83e2c6e9e231f0139a3301b2aff84b27b2b56432d190195`；原文：`O_DIRECT`。
- E09: [csrc/aio/common/deepspeed_aio_common.cpp:270–273](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/csrc/aio/common/deepspeed_aio_common.cpp:270)；SHA-256 `e2234f5c6c23a47be83e2c6e9e231f0139a3301b2aff84b27b2b56432d190195`；原文：`#if defined(__ENABLE_CANN__)`。
- E16: [csrc/aio/py_lib/deepspeed_py_io_handle.cpp:188–188](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_py_io_handle.cpp:188)；SHA-256 `7f613fdec611324b7038e922aee7c45df20409adf28c7ace697873b4ed32b5a1`；原文：`close(completed_op->_fd);`。

### C04 IOCB、CPU线程和native AIO context是软件提交资源

判定类别：`source_structure`。一个tensor分给p个线程，线程按block_size生成host-ptr/fd/offset的IOCB；每context按q分配IOCB/event、初始化native AIO context并在析构release。read/write handles及其线程不携带device DID；它们可改变供给、排队、提交重叠，不能增加物理requester。

- E10: [csrc/aio/common/deepspeed_aio_utils.cpp:41–61](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/common/deepspeed_aio_utils.cpp:41)；SHA-256 `e399c475c416b4c4ca5e8fcb382b8495c8c02658f282dea303eed96b4419620b`；原文：`io_prep_pread`。
- E11: [csrc/aio/common/deepspeed_aio_utils.cpp:77–101](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/common/deepspeed_aio_utils.cpp:77)；SHA-256 `e399c475c416b4c4ca5e8fcb382b8495c8c02658f282dea303eed96b4419620b`；原文：`io_prep_pwrite`。
- E12: [csrc/aio/py_lib/deepspeed_aio_op_desc.cpp:18–26](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/csrc/aio/py_lib/deepspeed_aio_op_desc.cpp:18)；SHA-256 `86ef19163ff113b739bcd8938a15f01af0adec3ed02400c7c37e7efee1677ce2`；原文：`buffer.nbytes()`。
- E13: [csrc/aio/common/deepspeed_aio_types.cpp:60–76](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/common/deepspeed_aio_types.cpp:60)；SHA-256 `beb47c510f442bef04807054c9020c80f2f135216dc0d51055ea0296081f3e74`；原文：`io_queue_init(queue_depth, &_io_ctxt);`。
- E14: [csrc/aio/common/deepspeed_aio_types.cpp:71–76](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/common/deepspeed_aio_types.cpp:71)；SHA-256 `beb47c510f442bef04807054c9020c80f2f135216dc0d51055ea0296081f3e74`；原文：`io_queue_release(_io_ctxt);`。
- E33: [drivers/nvme/host/pci.c:2936–2936](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:2936)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`get_device(&pdev->dev);`。
- E35: [drivers/iommu/riscv/iommu.c:1303–1309](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/riscv/iommu.c:1303)；SHA-256 `948e0c37d4885868ca3793201096c9fee61a6ce47d9e2be79364de1c24edad10`；原文：`pci_dev_id(to_pci_dev(dev));`。

### C05 wait、close和pool归还具有确定的正常路径次序

判定类别：`source_structure`。正常wait等每个worker的完成队列，调用finish，然后close fd、减少pending；optimizer swap完成先wait再free池槽。handle析构先stop/join，stop断言pending==0，调用者必须先完成清理，不能假定析构自动等待未完成I/O。Torch/accelerator池最终backing释放或allocator cache何时退还OS不在本次冻结DeepSpeed源码内。

- E15: [csrc/aio/py_lib/deepspeed_py_io_handle.cpp:176–195](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_py_io_handle.cpp:176)；SHA-256 `7f613fdec611324b7038e922aee7c45df20409adf28c7ace697873b4ed32b5a1`；原文：`completed_op->finish();`。
- E16: [csrc/aio/py_lib/deepspeed_py_io_handle.cpp:188–188](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_py_io_handle.cpp:188)；SHA-256 `7f613fdec611324b7038e922aee7c45df20409adf28c7ace697873b4ed32b5a1`；原文：`close(completed_op->_fd);`。
- E17: [csrc/aio/py_lib/deepspeed_py_io_handle.cpp:164–174](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_py_io_handle.cpp:164)；SHA-256 `7f613fdec611324b7038e922aee7c45df20409adf28c7ace697873b4ed32b5a1`；原文：`assert(0 == _num_pending_ops);`。
- E55: [deepspeed/runtime/swap_tensor/pipelined_optimizer_swapper.py:34–37](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/pipelined_optimizer_swapper.py:34)；SHA-256 `c59b97681629dded65adb361889ad63421a3a4be0d4778a5730d1f5e1ec6dd32`；原文：`self.aio_handle.wait() == self.num_ops`。
- E56: [deepspeed/runtime/swap_tensor/pipelined_optimizer_swapper.py:160–165](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/pipelined_optimizer_swapper.py:160)；SHA-256 `c59b97681629dded65adb361889ad63421a3a4be0d4778a5730d1f5e1ec6dd32`；原文：`self.swap_buffer_manager.free`。
- E02: [deepspeed/runtime/swap_tensor/utils.py:217–226](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/utils.py:217)；SHA-256 `e035d79b036bf2a8d16df65185eea38a2021b02d47027dd193ecf1ae4c868cb3`；原文：`self.free_buffer_index.append(self.used_buffer_index[b_id])`。

### C06 完成操作数不能代替完整字节成功

判定类别：`source_structure`。native io_pgetevents只核返回事件数量，未逐项检查event.res/res2；kernel完成事件会提供真实res。submit只断言返回正值，批量提交未核submit_ret==n_iocbs；async wrappers的validate=false。因此本轮只能确认源层期待全部操作完成，不能据wait数目声称每块成功、无短读/错误或数据正确。这是接受证据缺项，本关口不修代码。

- E19: [csrc/aio/common/deepspeed_aio_common.cpp:112–126](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/csrc/aio/common/deepspeed_aio_common.cpp:112)；SHA-256 `e2234f5c6c23a47be83e2c6e9e231f0139a3301b2aff84b27b2b56432d190195`；原文：`assert(n_completes >= min_completes);`。
- E20: [csrc/aio/common/deepspeed_aio_common.cpp:71–110](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/csrc/aio/common/deepspeed_aio_common.cpp:71)；SHA-256 `e2234f5c6c23a47be83e2c6e9e231f0139a3301b2aff84b27b2b56432d190195`；原文：`assert(submit_ret > 0);`。
- E21: [fs/aio.c:1459–1468](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/fs/aio.c:1459)；SHA-256 `da69a6b08d6d1d2516d49092abb72d77fd30ee7bf928877fbd242c892f654a35`；原文：`iocb->ki_res.res = res;`。
- E18: [csrc/aio/py_lib/deepspeed_py_io_handle.cpp:299–310](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_py_io_handle.cpp:299)；SHA-256 `7f613fdec611324b7038e922aee7c45df20409adf28c7ace697873b4ed32b5a1`；原文：`false, true`。
- E55: [deepspeed/runtime/swap_tensor/pipelined_optimizer_swapper.py:34–37](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/pipelined_optimizer_swapper.py:34)；SHA-256 `c59b97681629dded65adb361889ad63421a3a4be0d4778a5730d1f5e1ec6dd32`；原文：`self.aio_handle.wait() == self.num_ops`。

### C07 file AIO到NVMe之间仍有实际filesystem/block层

判定类别：`source_structure + conditional_deduction`。kernel AIO调用对应file read_iter/write_iter；经典DIO和iomap是可见的两种直接依赖路径，不声称本机nvme_path必走某一个。用户页的request pins由iov_iter提取，bio完成或dirty-work回收；这是AIO传输生命周期，不是app整池长期注册。blk-map存在bounce/copy规则，但没有实际filesystem证据时不能强行把blk_rq_map_user_iov插入普通file-AIO调用链。

- E22: [fs/aio.c:1534–1598](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/fs/aio.c:1534)；SHA-256 `da69a6b08d6d1d2516d49092abb72d77fd30ee7bf928877fbd242c892f654a35`；原文：`call_read_iter`。
- E23: [lib/iov_iter.c:1743–1773](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/lib/iov_iter.c:1743)；SHA-256 `b9c0a1d4485873ca3a600e3c3e8d7ec15c0d6387f7b20167fef8315b501776aa`；原文：`pin_user_pages_fast(addr, maxpages, gup_flags, *pages);`。
- E24: [fs/direct-io.c:167–174](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/fs/direct-io.c:167)；SHA-256 `3cefe29a73f880fb052bc2cfc83ec97db4e04630c71ae1b4ad92b4c2135691f9`；原文：`iov_iter_extract_pages`。
- E25: [fs/iomap/direct-io.c:151–181](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/fs/iomap/direct-io.c:151)；SHA-256 `33a972350b35b252294371225cb41e435a055d06e4a32a6f88c837c57379444f`；原文：`bio_release_pages(bio, false);`。
- E26: [block/bio.c:1317–1354](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/block/bio.c:1317)；SHA-256 `552d48fa02c5059a18eaaceb1077864645cdf1e763de30f37ff309e705b33a3e`；原文：`bio_set_flag(bio, BIO_PAGE_PINNED);`。
- E27: [block/blk.h:444–448](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/block/blk.h:444)；SHA-256 `40ea9781e7c8c0ad8678e0e2ac923a288cb00c0f8f2b4ed256bef99f5e1101db`；原文：`unpin_user_page(page);`。
- E28: [block/blk-map.c:630–664](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/block/blk-map.c:630)；SHA-256 `7217b3b24942c8345e0ba0e1ce0ea9a7a2cc200b10447ff4313f5e3ca503fac6`；原文：`copy = true;`。

### C08 NVMe payload映射单位是每个driver request

判定类别：`source_structure`。nvme_prep_rq依据本request物理segments调用nvme_map_data；简单data用dma_map_bvec，一般用request SG的dma_map_sgtable并编码PRP/SGL。完成路径先unmap再nvme_complete_rq；错误支路也回收。软件tensor操作/IOCB可以经filesystem及block层拆分或合并，不能固定为一个tensor一次mapping，也不能直接按AIO block数量宣称实际IOMMU map/walk数量。SQ/coherent descriptor可有较长生命周期，与payload不同。

- E29: [drivers/nvme/host/pci.c:730–802](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:730)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`dma_map_bvec`。
- E30: [drivers/nvme/host/pci.c:792–818](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:792)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`dma_map_sgtable`。
- E31: [drivers/nvme/host/pci.c:540–562](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:540)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`dma_unmap_sgtable`。
- E32: [drivers/nvme/host/pci.c:961–980](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:961)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`nvme_pci_unmap_rq(req);`。
- E34: [drivers/nvme/host/pci.c:1512–1513](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:1512)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`dma_alloc_coherent`。

### C09 translated DMA路径确有IOVA分配和映射，但实际选路未知

判定类别：`source_structure + conditional_deduction`。在iommu_dma_ops的map_page/map_sg路径，代码按domain granule分配IOVA、建立映射并处理sync；SG可合成连续IOVA，SWIOTLB及P2PDMA另有条件分支。CPU ptr/pin不固定返回的IOVA，回收后再次取得相同IOVA也不代表映射从未解除。实际nvme_path、controller、domain/ops、coherency和bounce条件需要运行配置证据，本轮没有测量。

- E36: [drivers/iommu/dma-iommu.c:730–753](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/dma-iommu.c:730)；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`；原文：`iommu_dma_alloc_iova`。
- E37: [drivers/iommu/dma-iommu.c:1318–1328](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/dma-iommu.c:1318)；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`；原文：`iommu_map_sg`。
- E38: [drivers/iommu/dma-iommu.c:707–727](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/dma-iommu.c:707)；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`；原文：`if (!iotlb_gather.queued)`。
- E47: [kernel/dma/mapping.c:116–126](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/kernel/dma/mapping.c:116)；SHA-256 `698bf2e08a5429319dd278219ac437d5d5198537907c7c2e0d31d9610192c559`；原文：`if (likely(!ops))`。
- E49: [drivers/iommu/dma-iommu.c:1616–1636](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/dma-iommu.c:1616)；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`；原文：`if (iommu_is_dma_domain(domain))`。

### C10 不能泛称每个unmap必定清除全部硬件TLB

判定类别：`source_structure + conditional_deduction`。通用dma-iommu解除范围映射后，strict立即iotlb_sync，DMA-FQ可将IOVA和失效延后/合批，最终调用driver flush。失效的地址、domain、ASID范围和硬件行为依driver与IOMMU而定；解除映射不等于所有其他映射的硬件缓存必全失。没有运行配置、有效命令轨迹、hit/miss/walk及完成量测，loss保持unknown。

- E38: [drivers/iommu/dma-iommu.c:707–727](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/dma-iommu.c:707)；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`；原文：`if (!iotlb_gather.queued)`。
- E39: [drivers/iommu/dma-iommu.c:148–152](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/dma-iommu.c:148)；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`；原文：`flush_iotlb_all`。

### C11 本地RISC-V驱动有宽失效源码事实，但不能迁移为native NVMe实测事实

判定类别：`source_structure + conditional_deduction`。本地flush_iotlb_range的普通VMA PSCID及逐地址代码被注释，实际post未设置地址的IOTINVAL并调用IOFENCE；map成功回调也到同函数。当前capable不返回DEFERRED_FLUSH能力，通用初始化会把DMA_FQ退为strict DMA。若所选NVMe真实使用这个driver、有效translated domain且提交成功，就存在宽失效命令路径；不能断言真实GPU/NVMe已使用它、每命令成功、硬件全缓存清空或已有性能损失。该驱动范围/接口问题不在本关口修复，也不是新研究机制的收益证据。

- E40: [drivers/iommu/riscv/iommu.c:1175–1186](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/riscv/iommu.c:1175)；SHA-256 `948e0c37d4885868ca3793201096c9fee61a6ce47d9e2be79364de1c24edad10`；原文：`return false;`。
- E41: [drivers/iommu/dma-iommu.c:617–620](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/dma-iommu.c:617)；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`；原文：`domain->type = IOMMU_DOMAIN_DMA;`。
- E42: [drivers/iommu/riscv/iommu.c:1730–1784](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/riscv/iommu.c:1730)；SHA-256 `948e0c37d4885868ca3793201096c9fee61a6ce47d9e2be79364de1c24edad10`；原文：`// riscv_iommu_cmd_inval_set_pscid(&cmd, domain->id);`。
- E43: [drivers/iommu/riscv/iommu.c:1768–1770](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/riscv/iommu.c:1768)；SHA-256 `948e0c37d4885868ca3793201096c9fee61a6ce47d9e2be79364de1c24edad10`；原文：`riscv_iommu_post(domain->iommu, &cmd);`。
- E44: [drivers/iommu/riscv/iommu.c:1799–1809](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/riscv/iommu.c:1799)；SHA-256 `948e0c37d4885868ca3793201096c9fee61a6ce47d9e2be79364de1c24edad10`；原文：`riscv_iommu_flush_iotlb_range`。
- E45: [drivers/iommu/iommu.c:2415–2417](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/iommu.c:2415)；SHA-256 `f04a493ebd8df77137f269a6aa1d66f894be50ffe77b423680d6759d8596e6eb`；原文：`ops->iotlb_sync_map(domain, iova, size);`。
- E46: [drivers/iommu/iommu.c:2558–2559](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/iommu.c:2558)；SHA-256 `f04a493ebd8df77137f269a6aa1d66f894be50ffe77b423680d6759d8596e6eb`；原文：`ops->iotlb_sync_map(domain, iova, mapped);`。

### C12 IOMMU bypass/identity可能令翻译缺口根本不存在

判定类别：`source_structure + conditional_deduction`。DMA API无ops、允许的dma_ops_bypass+足够mask/bus limit、平台direct分支及identity/passthrough都是相邻反条件；iommu_setup_dma_ops只为成功初始化的DMA domain安装iommu DMA ops。当前源码不能确定实际机器的NVMe是否经过host IOMMU；不能因为打开O_DIRECT或调用dma_map API就称它已经过地址翻译。

- E47: [kernel/dma/mapping.c:116–126](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/kernel/dma/mapping.c:116)；SHA-256 `698bf2e08a5429319dd278219ac437d5d5198537907c7c2e0d31d9610192c559`；原文：`if (likely(!ops))`。
- E48: [drivers/iommu/iommu.c:2693–2699](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/iommu.c:2693)；SHA-256 `f04a493ebd8df77137f269a6aa1d66f894be50ffe77b423680d6759d8596e6eb`；原文：`IOMMU_DOMAIN_IDENTITY`。
- E49: [drivers/iommu/dma-iommu.c:1616–1636](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/dma-iommu.c:1616)；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`；原文：`if (iommu_is_dma_domain(domain))`。

### C13 同一PCI NVMe controller上的read/write不是两个DID

判定类别：`source_structure + conditional_deduction`。NVMe queue最终关联同nvme_dev，DMA API device为该controller的pdev->dev；本地RISC-V PCI DID由PCI function取得，非read/write handle、software thread、queue id。若所有swap文件真实落在同PCI NVMe controller，读写可重叠并竞争但只有这个physical requester。base_folder不能证明后端只一设备，也不能证明两个设备：RAID、device-mapper、NVMe-oF等配置未知，不另选这些工作流。

- E33: [drivers/nvme/host/pci.c:2936–2936](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:2936)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`get_device(&pdev->dev);`。
- E35: [drivers/iommu/riscv/iommu.c:1303–1309](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/riscv/iommu.c:1303)；SHA-256 `948e0c37d4885868ca3793201096c9fee61a6ce47d9e2be79364de1c24edad10`；原文：`pci_dev_id(to_pci_dev(dev));`。
- E57: [deepspeed/runtime/swap_tensor/optimizer_utils.py:126–127](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/optimizer_utils.py:126)；SHA-256 `965afa1336e2e823b95142be8ceeacaba8723013e4bc7c34c5a2e459f3f0aba8`；原文：`os.path.join(base_folder, 'optimizer'`。

### C14 所选CPU swap缓冲区不强制GPU同时通过主机IOMMU

判定类别：`source_structure + conditional_deduction`。CPU池路径与cpu_op条件足以否决“NVMe read/write+GPU天然同时作为两个请求器”的推断。GPU目标的finish copy只在对应tensor条件分支中；GPU allocator pin/registration本身也不等于有并行GPU DMA。GPU参与CPU optimizer inner-loop、GPU requester/ATC/IOMMU拓扑及同时发生的DMA均unknown，计算与完整数值合同由root独占。

- E01: [deepspeed/runtime/swap_tensor/utils.py:186–189](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/source/DeepSpeed/deepspeed/runtime/swap_tensor/utils.py:186)；SHA-256 `e035d79b036bf2a8d16df65185eea38a2021b02d47027dd193ecf1ae4c868cb3`；原文：`device='cpu'`。
- E03: [csrc/aio/py_lib/deepspeed_cpu_op.cpp:33–40](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_cpu_op.cpp:33)；SHA-256 `da4d3dfe5e92c01d7da6b902cd8caa4281b0eea582037b9e4614383cc9abcf2c`；原文：`_buffer.is_cpu()`。
- E05: [csrc/aio/py_lib/deepspeed_cpu_op.cpp:45–65](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/py_lib/deepspeed_cpu_op.cpp:45)；SHA-256 `da4d3dfe5e92c01d7da6b902cd8caa4281b0eea582037b9e4614383cc9abcf2c`；原文：`if (_buffer.is_cuda())`。
- E33: [drivers/nvme/host/pci.c:2936–2936](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:2936)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`get_device(&pdev->dev);`。

### C15 合法普通持续映射必须保留为强对照

判定类别：`source_structure + conditional_deduction`。Linux DMA API本就允许streaming区域map一次，多次DMA间按CPU/device所有权sync，最后unmap。方向必须覆盖NVMe read写主存与NVMe write读主存，CPU更新不得在device所有权期间访问；需稳定页生命周期、权限和错误/撤销/停止后回收。可行驱动/注册接口的普通持续映射不是非法对照。现有DeepSpeed file AIO只传fd、host ptr、byte length、offset，未提供保留/复用NVMe DMA mapping的用户接口；仅app pin_memory/mlock/O_DIRECT不能改变stock pci.c完成时unmap。实现需实际driver/API或有界stack改变及全部成本，不在本关口实施。

- E50: [Documentation/core-api/dma-api-howto.rst:615–615](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/Documentation/core-api/dma-api-howto.rst:615)；SHA-256 `d2655c797b797a8b563b9877331e00076573428086844a4c54d765793de62e92`；原文：`dma_sync_single_for_cpu(dev, dma_handle, size, direction);`。
- E51: [Documentation/core-api/dma-api-howto.rst:627–627](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/Documentation/core-api/dma-api-howto.rst:627)；SHA-256 `d2655c797b797a8b563b9877331e00076573428086844a4c54d765793de62e92`；原文：`dma_sync_single_for_device(dev, dma_handle, size, direction);`。
- E52: [Documentation/core-api/dma-api-howto.rst:642–645](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/Documentation/core-api/dma-api-howto.rst:642)；SHA-256 `d2655c797b797a8b563b9877331e00076573428086844a4c54d765793de62e92`；原文：`After the last DMA transfer`。
- E10: [csrc/aio/common/deepspeed_aio_utils.cpp:41–61](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/common/deepspeed_aio_utils.cpp:41)；SHA-256 `e399c475c416b4c4ca5e8fcb382b8495c8c02658f282dea303eed96b4419620b`；原文：`io_prep_pread`。
- E29: [drivers/nvme/host/pci.c:730–802](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:730)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`dma_map_bvec`。
- E31: [drivers/nvme/host/pci.c:540–562](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:540)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`dma_unmap_sgtable`。
- E32: [drivers/nvme/host/pci.c:961–980](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/nvme/host/pci.c:961)；SHA-256 `43072ccd7fcfeb6ef6689a4ea7eacb6e16e61505b01440802c8fe7b6ff0ec7b3`；原文：`nvme_pci_unmap_rq(req);`。

### C16 保留CPU residency、batch/layout和大页普通控制的合法空间

判定类别：`source_structure + conditional_deduction`。按真实H与同完成/持久化合同评估CPU状态驻留；若工作集可合法保留，应计其少读/少写及最终输出成本。batch、block_size、队列深度、线程数、文件布局和alignment可改变普通I/O供给/拆分；CPU hugepage并不自动等于大IOMMU leaf，大leaf还需domain支持、物理连续/IOVA对齐、请求范围足够。不能人为冻结这些普通控制来制造新机制收益，也不能宣称在未知H/硬件上它们已胜出。

- E10: [csrc/aio/common/deepspeed_aio_utils.cpp:41–61](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/common/deepspeed_aio_utils.cpp:41)；SHA-256 `e399c475c416b4c4ca5e8fcb382b8495c8c02658f282dea303eed96b4419620b`；原文：`io_prep_pread`。
- E11: [csrc/aio/common/deepspeed_aio_utils.cpp:77–101](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/DeepSpeed/csrc/aio/common/deepspeed_aio_utils.cpp:77)；SHA-256 `e399c475c416b4c4ca5e8fcb382b8495c8c02658f282dea303eed96b4419620b`；原文：`io_prep_pwrite`。
- E36: [drivers/iommu/dma-iommu.c:730–753](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/dma-iommu.c:730)；SHA-256 `68862ded252075324f542f2a66db4aceaa6179480cbdbd24f5400394ff2ea176`；原文：`iommu_dma_alloc_iova`。
- E53: [drivers/iommu/iommu.c:2296–2311](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/iommu.c:2296)；SHA-256 `f04a493ebd8df77137f269a6aa1d66f894be50ffe77b423680d6759d8596e6eb`；原文：`(iova ^ paddr)`。
- E54: [drivers/iommu/iommu.c:2326–2333](/home/xvhaocheng/work/iommu-platform/out/stage113-zero-infinity-admission-20261013/workers/nvme-boundary/source/Linux/drivers/iommu/iommu.c:2326)；SHA-256 `f04a493ebd8df77137f269a6aa1d66f894be50ffe77b423680d6759d8596e6eb`；原文：`iommu_pgsize`。

## 完整生命周期与应计成本

| 阶段 | 可确认的结构/必须计入 | 未确认的量 | 证据 |
|---|---|---|---|
| 池准备 | CPU tensor分配/zero、pin；pool backing与逻辑slot分别持有 | 真实H、allocator锁页/注册/缓存生命周期和次数 | C01/C02 |
| 一次tensor AIO操作 | 文件stat/read size、open O_DIRECT、descriptor tensor引用、p线程工作入队 | 实际文件/FS路径、操作数、open/metadata耗时 | C03–C05 |
| 可选bounce/copy | 未pinned或非CPU输入的CPU bounce、write前copy、read后copyback；contiguous可能复制 | selected pinned contiguous切片可跳过；allocator特殊条件未知 | C02 |
| native AIO contexts | 每handle有base context和p个worker contexts、每context q个IOCB/event；初始化/释放 | libaio内存、q/p实际配置，提交/收割成本 | C04 |
| 文件到block | filesystem extent/metadata、用户页request pin、bio装配、拆分/合并、可选bounce | actual FS、block设备链、request数、页数 | C07 |
| NVMe准备 | SG/mempool、data DMA map、IOVA/PTE、PRP/SGL列表、CPU/device sync；SQ/CQ等长期descriptor资源 | coherency、P2P/bounce、mapping leaf、实际map个数与成本 | C08/C09 |
| 硬件完成 | NVMe请求完成后先payload unmap并回收SG/PRP，才向上完成 | controller DMA bytes、media/queue服务、共享IOMMU的miss/walk/失效 | C08–C13 |
| 上层reap | event返回res；worker完成队列；native wait后finish/close；pipeline按操作数检查 | 短IO/error核验、所有字节正确与最后输出完成 | C05/C06 |
| tile回收 | wait后逻辑slot归还；CPU pinned backing通常仍由池持有 | terminal allocator退还OS/解除注册及其成本 | C01/C05 |
| 终止 | pending先清零，stop/join，IOCB/context release，managed locked tensors munlock/free | 错误/取消流程成功、线程/所有权quiet、SSD缓存持久化 | C03/C05 |
| 普通持续映射对照 | 稳定页/IOVA注册、双方向权限、CPU/device sync、最终停止并unmap | 必须给实际driver接口与全部setup/teardown、驻留/IOVA资源 | C15 |

## 正条件、相邻反条件和最小缺项

| 待接受能力 | 有证据的正条件 | 相邻反条件 | 最小缺项 |
|---|---|---|---|
| 原生file-AIO触达翻译 | swap文件实际落在PCI NVMe，经成功安装的translated DMA ops；有效request map/done/unmap | identity/bypass、无IOMMU；FS fallback/非NVMe后端；只见O_DIRECT/CPU pin | 实际配置、FS/block链、PCI function/DID、domain/ops与完成证据 |
| 自然多requester共享翻译 | 来源工作负载本身存在独立物理DMA peer，实际同时经同IOMMU并有可识别DID/事务 | 同controller的read/write queues仍一个requester；CPU计算不是DMA peer；GPU copy无证据 | 在既定source内的自然peer及共享路径/重叠完成证据；不能另造peer或第二workflow |
| 有翻译性能缺口 | 真实完成/正确字节合同成立，实际归因揭示翻译等待/失效/容量对同预算合法普通控制仍有限制 | SSD媒体/队列/FS/CPU供给/bounce主导；普通residency/layout/largeleaf/合法persistent mapping复现 | 真实H/参数、可执行普通对照和分量证据；本关口均未实验 |
| 持久映射为真实普通控制 | driver/API保留数据区DMA地址且有合法CPU/device sync及终止释放 | app只pin/mlock，不改变stock NVMe请求完成时unmap；非法CPU与DMA同时写 | 具体注册接口、权限/方向、pin寿命、request复用/错误/取消/最终回收合同 |
| 投影到现FPGA | root证明任务byte/compute/所有权合同、自然实际requester/共享路径及普通控制外新事实 | 为模拟native人为让原本合法FPGA整池映射逐tile remap；把一个NVMe拆成多个假DID | root独占；本席不扩FPGA压力、不恢复R107/C112、不自动开始G0/G1 |

## unknown清单

- 实际job配置、tile尺寸/数目、read/write pipeline开关、p/q/block_size、H、rank数、保存/持久化合同。完整byte和计算部分交root。
- 实际nvme_path、mount/filesystem、block backend、NVMe controller PCI function、DID、request队列路径和kernel版本。
- 实际IOMMU instance/domain/ops、bypass/identity/translated、IOVA地址/leaf、coherency、SWIOTLB/P2P、失效命令成功与范围。
- 自然第二physical DMA requester、GPU是否实际出现在此CPU optimizer inner-loop并有host-IOMMU共享与同时DMA。
- 完整request/IOCB byte成功、terminal outputs、quiet与最终释放；仅源wait数量不是证据。
- IOATC hit/miss/walk、translation loss、SSD/FS/CPU/复制/注册/映射分量和收益。未实测，保持unknown。

## 导航

- `critical-quotes.json`：E01–E57原始短引文、精确行号、完整hash、原文件与冻结snapshot路径。
- `source-manifest.json`：40个source/metadata入口，Linux HEAD/dirty状态及固定DeepSpeed tree/blob一致性。
- `source/parent-manifest.json`、`source/parent-version.json`、`failed-fetches.json`：初始失败GET、固定版本与本席定向恢复证据。
- 本目录report生成/来源核验属于产物完整性检查；没有运行workload/test/build或测板。
