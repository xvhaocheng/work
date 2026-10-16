# stage111：有限接口与受检资源释放建议

本席为 Codex Sol/ultra。这里只保存只读源码核查及计划；没有实现、编译、设备操作或重跑旧验证。根席负责低层实现、最终接受和实际执行。

## 推荐决定

新增一个有限 run ioctl，内部一次完成 prepare → prior swapout → restore → verify → checked release。不要新增跨 ioctl 的 prepare/run/release 会话：本轮只有一个有限实例，所有池和映射在整个 run 内稳定驻留即可。现有 open/release 仅存 device 指针，没有会话资源生命周期（linux/drivers/misc/idma.c:215-226）；旧全局 mutex 已串行化准备、映射和执行（:153-154）。保留旧 ABI 和旧路径。

首 pilot 只允许 DMA0/DID10 在 S0 中访问同可信 owner 的全部四池。四真实 DID 来自 linux/arch/riscv/boot/dts/cva6/cva6-ariane-minimal.dts:141-174；授权 single-DMA 来自 out/r107-bounded-decision-20261012/instance/finite-contract.json:72-82。DID10/11 并行能力可保留在接口设计中，本轮不执行第二种场景。

请求应只携带 ABI/size、冻结合同身份、零 reserved、有限 timeout、四池数据输入、固定有限 manifest 和有界结果缓冲。禁止用户 PA/IOVA 或任意 DMA 地址。一次复制完整 manifest 到 kernel-owned 内存并验证精确 worker/layer/component/slot 覆盖后才 issue；没有用户指针在 DMA 中重新解释的路径。恢复是 144 原 copy、96 合法合并 logical runs、1,769,472B；不是 GPU native trace（finite-contract.json:85-104）。

每个 run 保存分配记录、DMA handle、size、attrs、actual leaf extent、原始 PTE/PA、device/domain/DID/root 身份、NEXT/DONE 基线、每段完成 ID、各阶段原始时间、guard/源核查和失败资源状态。结果只能在 mutex 内从 kernel-owned snapshot 复制。

## 池和唯一 DMA-API 映射

选择 DMA API 的唯一返回映射，不调用旧 idma_find_iova / idma_iommu_map_range 追加 alias。优先四次完整 4MiB dma_alloc_attrs(GFP_KERNEL, FORCE_CONTIGUOUS)：每个逻辑池 2,359,296B，剩余完整 backing 作为 guard，总 16MiB。合法失败回退是八次 2MiB FORCE_CONTIGUOUS，各逻辑池用两个已返回 extent 的 offset resolver，禁止为伪造连续 IOVA 再手工 map。

源码路径：linux/drivers/iommu/dma-iommu.c:1493-1509 在 FORCE_CONTIGUOUS 时走连续分配并且只做一个 __iommu_dma_map；:1446-1477 先 CMA 再 alloc_pages(get_order(size))。已恢复 embedded config 禁用 CMA（out/r107-platform-identity-recovery-20261012/kernel/Image-embedded.config:680），所以保持该配置时普通页路径分别是 order10/order9；新 kernel 仍须核配置。若以后启用 CMA，其 alignment 会受 CONFIG_CMA_ALIGNMENT 限制（linux/kernel/dma/contiguous.c:282-286），不能只凭 FORCE_CONTIGUOUS 名称保证 2MiB 对齐。

IOVA 使用 API 的 allocator reservation：dma-iommu.c:658-687、730-753；alloc_iova_fast 的常规路径要求 size alignment（linux/drivers/iommu/iova.c:439-459）。叶型由实际 PA、IOVA、size 和 bitmap 决定（linux/drivers/iommu/iommu.c:2264-2284），必须读 actual PTE，不能把 order/attrs 当板端叶证据。四个 4MiB extent 若实际双端均对齐且支持 2MiB，可形成八个 2MiB leaf。

4KiB backing 回退可使用非 FORCE_CONTIGUOUS 的同一 DMA API remap allocation；DMA_ATTR_ALLOC_SINGLE_PAGES 在 dma-iommu.c:844-849 限制其物理页分配粒度。该 API 保留连续 IOVA；其 actual leaf 仍须读取，不能从 order0 推定所有 PTE 都是 4KiB。若需要保证逐页 4KiB extent，也可分别申请 PAGE_SIZE 的 API 对象并用 extent resolver，但其映射/对象成本必须完整计入；本轮不为此扩测试矩阵。

DMA coherent allocation 的 IOMMU 权限由 BIDIRECTIONAL 给出（dma-iommu.c:1486-1487，:638-652），当前 RISC-V PTE 写权限路径同时允许读（linux/drivers/iommu/riscv/io_pgtable.c:151-153）。因此 CPU_HOME 不宣称硬件 RO；逻辑不可变由独占所有权和全字节源保持验证保证。四池各自 PA backing 必须不重叠。额外软件创建 DMA alias=0；CPU virtual remap 不是另一个 DMA IOVA alias。

## ownership 和缓存交接

driver 只使用 kernel-owned pool，完整 run 内不 mmap 给用户。copy_from_user 后初始化实际四池；真实执行 prior swapout，再恢复，期间不释放或重映射。prior swapout 另计 1,769,472B，不能用 CPU memcpy 冒充该 DMA 成本。prior direction、源完整性、restore direction及 guard各自核验。

普通 DMA API 的 coherent 名字不能当真实 snoop。已有板路径明确使用 CVA6 WT D-cache 的 FENCE.I workaround：linux/drivers/misc/idma.c:2491-2495 在 handoff 前执行，:2643-2645 在两路 drain 后重新获得 CPU visibility；linux/arch/riscv/include/asm/cacheflush.h:11-14 实际发出 fence.i。新 run 复用该平台交接，按实际 producer / DMA / CPU consumer 的阶段计时，不做人工每层 flush。原 embedded config 开了 SMP（:320）；因此必须验实际单 hart，或者使用覆盖全部相关 hart 的交接，不能把 local FENCE.I 说成多核同步。

先完成初始化和 PTE 写入可见性，再做现有 checked S1 flush，成功后 issue；结束时所有 DONE+quiet 后再 FENCE.I 才读池。swapout→restore 之间若仅 DMA producer/consumer，可在完整 prior DONE/quiet 后交接；若插入 CPU byte check，其缓存交接和全部读取成本单列。每层记录真实 layer_dma_complete；完整 run 后进行 CPU layer checks 时另记 layer_proxy_ready，不能将延迟代理核查冒充原生 attention 开始。依赖入口：finite-contract.json:115-128。

idma_experiment_lock 不是 device/domain binding 生命周期锁。HPM snapshot 不拿 device/domain reference，要求 caller 保持 bound（linux/drivers/iommu/riscv/iommu-hpm.c:165-180；iommu.h:163-173）。新 owner必须稳定绑定、阻止 unbind/reattach，并按既有 lock 顺序核 deadlock；普通 get_device 只保对象引用不能替代此要求。

## 4KiB descriptor 与生产 W-credit 边界

首 pilot 选单 DMA0、4KiB 双端对齐、DEFAULT/CONF=0；按冻结寄存器前端自身的 COUNT→WAIT→trans_complete 顺序执行，不额外制造软件 single-inflight 限制。可保留原硬件 batch，对合法连续24KiB/12KiB run分别设置6/3个4KiB task，减少CPU供给次数。恢复仍为 96 logical runs，但需 432 physical descriptors（1,769,472/4096）；页表可能仍为 2MiB，不得称其为 4KiB PTE 对照。两方向各 432 段，共 864 physical descriptors和 3,538,944B。冻结参数和窗口可达性补充见本目录 frozen-dma-burst-window-boundary.md。

此选择有两个独立理由：DMA0 当前 buffer 是 512×8=4KiB，DMA2/3 只有三 beat，不能扩用同一完整-buffer论证（cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1262-1270）。AXI legalizer 每 burst 至多 256 beat，64bit 下最大 2KiB（cva6/corev_apu/iDMA/src/idma_legalizer.sv:12-14、75-85）；双端 4KiB 对齐、CONF=0 的 4KiB 段是两条写 burst，读写保持 coupled（register_64bit/idma_reg64_frontend.sv:341-373）。从零 outstanding 出发、全平台无其他 translated write、一次只发这一个段时，此段不能产生八条未消费 AW，因而有界规避已知 C=8 空/满别名条件。这个推导须由根直接核当前冻结源和 actual 设置，不能扩大成任意 workload/参数正确性。

旧证据只是 4k-solo0 分段模型、2,309 descriptor、六张量功能通过（out/ai-mandatory-work-g0-20261012/validator/segmented-minimal-acceptance.json:1-19；docs/ai-mandatory-work-g0-results-20261012.md:25-27），没有当前 FPGA 的 R107 验收，更没有 DID10/11 并行或 DMA2/3 验收。私有 W 修复不是 production merge（out/ai-mandatory-work-g0-20261012/credit-boundary-root-acceptance.json:3-9、27-31）。

## DONE、quiet 和失败隔离

NEXT_ID 读取本身触发；返回零不是未执行的证明，旧驱动因此 quarantine（idma.c:453-462）。保存 run 前真实 DONE 基线和每次返回的 NEXT_ID；不能硬套 reset DONE=0，源实际 reset NEXT=2/DONE=1（idma_transfer_id_gen.sv:69-75）。每个物理段必须只凭其 own DONE ID 退休（idma.c:668-685），六完整槽的所有段完成才记 worker/layer完成。

最后 DMA DONE 之后仍核 composition word5 quiet。当前集成 quiet覆盖所有四 frontend/backend、issue、launcher、DMA读写计数和 IOMMU quiet（ariane_peripherals_xilinx.sv:1028-1035；dma_observer.sv:105-108；idma_reg64_frontend.sv:148-150；dma_composition_control.sv:466-473）。只读 quiet，不调用会 clear/切配置的 composition_configure。全局 quiet无法建立时，四池和所有相关 requester/domain都保留并隔离；STOP 不取消 AXI（idma.c:821-843），用户 signal 不能中断资源安全 drain。

全字节验证包含 copied padding、每个源保持、未活动/空槽 guard以及完整16MiB backing slack，不能只看 hash或 sampled words；原核查模式明确 source/untouched独立验证（idma.c:776-785），有限原合同 byte/guard入口为finite-contract.md:19、27。数据错误在安全 drain成立后可正常撤映射；无法证明安全 drain不准CPU读取不确定写入区或free。

## 最小而正确的 checked release 内部 helper

在新隔离 kernel 的 linux/drivers/iommu/dma-iommu.c 增加仅供 finite owner调用的 batch checked-free helper；不改 iommu_dma_ops.free，也不改已有 dma_free_attrs 路径。所有分配记录一起进入 helper，支持4/8 extent及有界4KiB回退。callback由 owner提供，只做当前平台 CPU PTE可见性与已存在的受检全局 S1 invalidation。

必要性：旧 __iommu_dma_unmap 在 :722 撤 PTE，:725-727 调 void sync后立即归还IOVA；iommu_dma_free 在 :1442-1443随即释放backing。RISC-V普通 sync在iommu.c:1768-1770不检查post/fence返回。因此旧 free之前flush不能证明PTE撤销之后fence成立；先free再checkedflush也无法保留资源。

建议严格顺序：

1. owner已停止提交，所有owned ID DONE且全局quiet成立；保持idma mutex、稳定device/domain binding、HPM session。先保存完整数据和final原始snapshot。
2. helper检查所有对象确实来自正常iommu_dma_ops allocation，device/domain/cookie与owner acquire snapshot相同；cookie为IOVA而非MSI，fq_domain=NULL，非direct/per-device-coherent/atomic-pool旁路；不能仅根据get_dma_ops猜分配来源（kernel/dma/mapping.c:513-522的coherent旁路必须排除）。对象完整size/attrs/CPU remap及allocator reservation都留在owner ledger。
3. 在受约束的map/unmap锁下读取actual leaf extents，确认各leaf完整属于本API allocation、PA和权限仍与setup snapshot相同。逐actual leaf调用iommu_unmap_fast，要求exact leaf返回；所有allocation都只撤PTE，不调用__iommu_dma_unmap、dma_free_attrs、queue_iova或free_iova。
4. 撤后再walk所有covered4KiB位置，必须无present PTE；保留撤前/后snapshot。然后callback先执行当前CVA6 PTE-store可见性FENCE.I，再调用riscv_iommu_hpm_flush_s1。现有入口iommu-hpm.c:231-237最终进入riscv_iommu_flush_s1_checked（iommu.c:2164-2295）；后者核live root/domain、CQ enabled/errors/代际、post结果及fence结果。当前quiesced no-ATS/no-S2边界见iommu.c:2148-2162，不声称通用ATS/PR/PW同步证明。
5. 只有全部撤销和callback都成功后，helper内部使用iommu_dma_free_iova(cookie,aligned_handle,aligned_size,NULL)归还各API reservation，再做debug_dma_free_coherent记账，最后__iommu_dma_free(dev,size,cpu_addr)释放CPU remap和backing（dma-iommu.c:1408-1436）。严禁再调用旧.free，避免二次unmap。同步gather的freelist必须为空，或按受检同步后规则释放；当前RISC-V leaf unmap只清PTE，没有把页表页加入freelist（io_pgtable.c:178-206）。不要将含未释放表页的gather放栈后遗失。

actual leaf尺寸不是可省检查：core iommu.c:2423-2433选择unmap pgsize依据IOVA/请求size，而不是PTE真实leaf；RISC-V unmap包装只转发（:1828-1838），io_pgtable.c:191-203每次仅清实际找到的一项，却按传入page_size前进/记账。大extent内实际4KiB leaf若按2MiB请求整体撤，不能单凭returned bytes证明全PTE清空。逐真实leaf和撤后全extent walk是本helper必要正确性条件；这是源码推导，根须直接复核，尚未新测试。

失败状态必须明确：

- 预审拒绝且尚未撤PTE：映射/IOVA/backing全部保留，owner决定在安全条件下按同helper清理。
- 撤PTE部分完成/撤后发现残留/checked flush失败：部分或全部PTE已撤，但全部原IOVA reservation、CPU ptr、backing、domain/device refs、撤前后leaf records及gather均保留。状态为REVOKED_OR_PARTIAL_QUARANTINED，禁止重发/重新map，不自动重试free。
- NEXT=0、DONE超时或quiet失败：尚未撤PTE，全部映射及16MiB backing保留直到reboot；隔离整个共享IOMMU后续研究ioctl，不能只标DMA0而允许其他requester继续复用可能残余状态。现有idma_quarantine/cleanup skip提供保留惯例（idma.c:333-346、402-408、751-762），新ownerledger必须真正持久保存引用/状态。
- 成功release以后copy_to_user失败：硬件任务及资源释放实际已完成，返回交付错误并保存kernel摘要；不能重跑restore或声称DMA未发生。

普通dma_alloc_attrs内部mapping失败的回滚发生在它返回NULL之前，driver无法保留未返回的对象（dma-iommu.c:1508-1512及:749-751）。首pilot如遇该技术错误立即停止，不能将其当翻译科学结论；如果新kernel要宣称这种内部失败也全部retain，需要同一有限owner分配路径增设对应内部钩子，而不能仅由release helper承诺。此项属于明确未闭合边界。

## 接受前仍未知道的事实

当前板端actual allocation、leaf snapshot、全部cache/fence成本、计时校准、完整prior+restore NEXT/DONE/quiet和16MiB验证都未执行。本建议不把API返回、旧模型PASS或源码推导当新板端验收。checked helper的准确锁顺序、map/unmap互斥和分配旁路拒绝由根低层设计并验证；旧freeze和production不修改。
