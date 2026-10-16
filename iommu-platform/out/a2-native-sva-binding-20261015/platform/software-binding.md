# A2 当前平台软件绑定与最小重设计

2026-10-15；独占范围：Linux map/driver/app/consumer/unmap。仅源码审查与设计，未改生产代码、未构建/执行 kernel、未操作板、未回放旧实验。DATE 原始软件的取得与论文定义由 root/DATE 席单独裁决。

## 结论与能力分层

**现有平台 iDMA 用户接口不是进程 SVA，也没有完整的用户零拷贝任务。** 它用 DMA API 分配内核缓冲区，在设备当前域中另外建立 IOVA 别名，向 DMA 提交该别名；应用只接收实验统计。`raw_user` 是原始观察结果的用户输出指针，不是 DMA 源/目的地址。当前没有可声称完成“真实 SVA 应用→必要 consumer→释放”的现成 CLI。

两种重设计都可从当前源码的实际能力出发，但都**尚未实现/运行**：

| 层次 | 地址与页表 | 最小能力 | 边界 |
|---|---|---|---|
| 当前实验接口 | kernel buffer；DMA API IOVA + 手工 IOVA alias；独立设备页表 | 内核合成 memcpy/核验 | 无用户 buffer、无用户 mmap、无进程绑定、无应用 consumer |
| C1：专用 unmanaged 域的 pinned 同址镜像 | DMA 地址数值等于选定 UVA；单独 I/O 页表映射到 pinned/driver-owned PA | 固定地址、有限窗口零拷贝；可作具名简单对照；是否达到论文的较弱 SVA 定义交 root 裁决 | 不共享 `mm->pgd`，不称 generic/process-page-table SVA |
| C2：专用 DID 的 non-PASID 单 mm 绑定 | `CONF.pv=0`；DC.FSC 直接指同一 `mm->pgd`；DMA 直接收到 UVA | 真正共享 CPU 进程页表的静态 pinned 用户任务 | 新增私有内核绑定接口；不是现有 `iommu_sva_bind_device()` 已可运行；无 demand paging/ATS/多 mm 复用 |

C2 是建议保留的较强重设计；C1 是必要的普通可执行对照。**如果新增 helper 能实际确认页表/设备上下文失效与排空，并通过下列有限任务生命周期验收，C2 才能称已在本平台实现的 pinned 单 mm SVA。** 当前仅源码支持其静态地址翻译机制，不支持“完整软件链已经闭合”或完整时间收益。

## 当前实际调用链

以下路径都以 `iommu-platform/` 为根；完整 SHA-256 和短原文见同名 JSON。

| 步骤 | 原文位置 | 实际行为与边界 |
|---|---|---|
| probe / 身份 | `linux/drivers/misc/idma.c:2934-2981`；`configs/platform.dts:132-174` | platform misc 设备；DT DID10/11/12/13；64-bit DMA mask；不是 PCI 设备。软件实例 ID 按 probe 分配，必须按 CAPS 验 DID，不能仅凭设备名推身份。 |
| open / close | `idma.c:215-225` | open 只把 `fp->private_data` 指向全局 `idma_device`；release 空，无 per-open `mm`/pin/session。 |
| fops | `idma.c:2927-2932` | 只有 open/release/unlocked_ioctl；无 mmap/read/write/poll。 |
| 应用 | `bench/idma_experiment.c:648-675`；`bench/idma_research.c:1515-1560`；`bench/idma_composition.c:489-499` | 打开 misc 设备，传标量实验配置；`research.raw_user` 仅承接观察数据。没有传用户数据 buffer。 |
| 准备/并发 | `idma.c:153-154,2389-2456` | 全实验 mutex 包括 mapping 与执行；v/a 最多两个内核 stream，共同准备后执行。该 mutex 不能跨用户返回长期持有，新增 session 必须用 owner 状态拒绝旧 ioctl。 |
| 分配与映射 | `idma.c:354-399` | `iommu_get_domain_for_dev()`，拒绝 identity；`dma_alloc_attrs(FORCE_CONTIGUOUS)` 分 src/dst，CPU 填合成 pattern，再对连续 PA 每页 `iommu_map()`。 |
| IOVA 选址 | `idma.c:229-299` | 扫域 aperture 高端空闲 PTE，2MiB 步长，选两个别名；不是 `dma_map_*` 用户 buffer，未与 default-domain IOVA allocator 保留该额外区间，不能原样扩成任意 UVA 注册接口。 |
| 提交 | `idma.c:411-467` | 写 src_iova/dst_iova/chunk，CONF=0，再读 NEXT 建立 transfer_id；无 current->mm/PASID/SVA API。NEXT=0 会 quarantine。 |
| 完成/排空 | `idma.c:668-687,751-769,2613-2639` | DONE 匹配 transfer_id；停止 refill，双方 drain；失败 retain 全部映射/缓冲至重启。DONE 本身不等于传输数据正确，硬件 error 可被屏蔽，需 owner 席的正常响应与字节合同。 |
| CPU 取得/consumer | `idma.c:2643-2652,772-818` | 双方 drain 后 `local_flush_icache_all()` 再内核逐字节核验；不是应用实际消费输出，也没有当前 Linux 的逐前缀交付 ABI。 |
| 清理 | `idma.c:333-347,2737-2744` | 先手工 unmap alias，再 dma_free_attrs 原 DMA allocation；retain 时全跳过。close 不能代替这些步骤。 |
| 非阻塞 helper | `bench/idma_nonblock_read.c:3-16,136-166,300-303` | `/dev/mem` mmap 仅访问已冻结 observer，选择 bank 并读原始观察值；不能证明用户数据 mmap/SVA。 |

当前分配实际还有 DMA API 自己的 IOVA 映射：`dma-iommu.c:1483-1515`。额外 alias 不替代该分配句柄；清理必须分别释放。当前代码未 `pin_user_pages*`、未把 CPU 进程页表安装到 DC、未 `iommu_sva_bind_device()`。

## 现有 RISC-V SVA 代码为何不能直接接当前 iDMA

`linux/drivers/iommu/riscv/Kconfig:4-17` 选择 IOMMU_SVA/MMU_NOTIFIER/PCI PASID，说明功能被编入，不说明该 platform 设备实际启用。

1. `riscv/iommu.c:1037-1097` 对非 PCI 设备直接返回；只有完整 PCI ATS/PRI/PASID 路径设 `ep->pasid_enabled=true`。platform endpoint 由 `:1295-1324` 零初始化并走这一路。
2. `:1102-1117` 启用 SVA 必须有 `pq_work` 且 `pasid_enabled`；`:1630-1633` 的 set_dev_pasid 同样拒绝未启用端点。
3. generic `linux/drivers/iommu/iommu-sva.c:49-105` 明确是先启用 feature，再为 mm 分 PASID 并 `iommu_attach_device_pasid`；它不是无 PASID 的普通 `iommu_attach_device()`。
4. 原 SVA set_dev_pasid 确会把 `mm->pgd` 放进 PC.FSC（`riscv/iommu.c:1639-1676`），但当前 iDMA 的 pv1 只带每 lane 固定20+i PID，不能冒充已支持动态 PASID。pv0 可直接走 DC.FSC，故 C2 不需要新增动态 PID 寄存器。
5. 当前 SVA mm.release 还是 TODO（`:614-621`）；set_dev_pasid 的单页 PD8 表与 PASID 范围、注册失败清理等也未作为本任务运行前提验收。不应通过强行设置 pasid_enabled、复用 PCI helper 或改固定 PID 绕过它们。

硬件同席核查支持 C2 静态权限：PROT0→user；S1 要求用户 PTE.U=1，store 要求 W1；PTW 要求 A/R，store 再要求 D，**无硬件 A/D 自动更新**。Pv0/PDTV0 可直取 DC.FSC，实际 Sv39。精确 RTL 原文与 hash 由 `publisher-owner` 产物负责；本文件不冒充已做 Linux/RTL 联合执行。

## 两条路线共用的真实 Linux API 边界

### 专用域与 DMA owner 是必要修改

先停止并 drain 两设备的现有任务；在 driver 全局锁下建立 per-session owner，所有旧实验 ioctl 发现 owner 后返回 EBUSY。用 `iommu_device_claim_dma_owner(dev, ctx)`（core `iommu.c:3162-3191`），不用文档已禁止新调用的 group 级接口（`:3124-3130`）。每个 singleton group、DID、IOMMU 实例均要验证；两个设备用同一个 owner。

claim 会先切 blocking domain（core `:3103-3120`）。C1 再用 `iommu_domain_alloc(dev->bus)` 建 unmanaged domain（`:1942-1945`），同一新域 attach 两 singleton groups（`:2001-2025`）。C2 通过新增私有 helper 建 borrowed-mm domain，并由 IOMMU core 一致管理 group/domain，禁止只改 DC、让 `iommu_get_domain_for_dev()` 仍显示旧域。

**现有 domain 切换不能直接当成可靠前提。** core `:2117-2143` 在切换时只调用 attach；RISC-V attach `riscv/iommu.c:1611-1613` 直接改 ep->domain 并 list_add_tail，未从旧域链表摘除。list_del 只见于 detach_endpoint `:1335-1342`，其调用在 release/free。必须先修有序 detach/attach/rollback 的 endpoint 链表与锁规则，再有限验一次 default→blocking→task→blocking→default；否则新域 free/恢复有旧链表残留风险。不是本轮已执行的修复。

PSCID 也须记账：当前 `:1409-1419,1522` 通过临时 mm 申请 domain ID，而 `:1461-1476` 不释放对应 ID。新 helper 必须用可回收的合法 PSCID allocator，或单次限定并如实保留该资源；不能称反复创建/释放已无泄漏。

### 缓存不能靠 DMA 地址名称糊过去

`dma-iommu.c:950-979` 的 dma_sync_single_for_* 把传入句柄交给 **default DMA domain** 查 PA，core `iommu.c:2078-2081` 也明确返回 default_domain。因此 C1/C2 的 UVA、手工 alias 或 borrowed-mm 地址不得冒充 dma_handle。C1 pinned 路线由保存的 page/PA 做平台 cache hook；C2 也按 pinned page 清单处理，或使用经证明可用的 CVA6 全 cache 交接。

本 DTS 仅声明 `rv64imafdch`，未宣告 Zicbom/Svpbmt（`platform.dts:21-22`）。虽然已有构建 `.config` 打开两项，`ALT_CMO_OP` 默认路径为 NOP（`arch/riscv/include/asm/errata_list.h:120-139`），`ALT_SVPBMT` 默认返回0（`:53-63`）。`dma_alloc_coherent`/`dma_mmap_attrs` 的名称不能证明这颗 CVA6 的缓存一致性或用户页真正不可缓存。

现 driver 的 native acquire 是 `fence.i`（`arch/riscv/include/asm/cacheflush.h:11-14`），stage117 已验证该 CVA6 WT cache 控制的端口级语义与成本，但未验证本新 Linux ABI。新增 release 必须保证 CPU 源写/metadata 写已退休后才 NEXT；acquire 必须在本包所有 span 正常完成后执行、返回后才发布 CPU ownership。先限定实际单 hart，并核在线 CPU/缓存配置；若有多 hart，需要覆盖全部可能驻留副本。全 cache 交接仍需完整收费，不能只算 payload。

**root提出的更小功能bring-up选项：从M-mode最早启动起保持Dcache disabled，贯穿本次完整任务，不在active DMA中途切换。** root已定向核 `cva6/core/csr_regfile.sv:95,466,1096` 的DCACHE enable CSR、`core/cva6.sv:666,769` 的WT连接，以及WT controller禁用后读NC/不取hit（`core/cache_subsystem/wt_dcache_ctrl.sv:88,145`）、write buffer写NC（`wt_dcache_wbuffer.sv:58,144`）。这些是root提供的源码入口，本席不重复其RTL审查。需要新增且尚未实现的OpenSBI启动hook、实际CSR读回/固定profile manifest、写缓冲排空/内存排序证明；所有普通对照用同一profile，完整计CPU、页表访问及consumer变慢。该profile仅用于受限功能SVA闭链，不能替代cached应用合同或用于宣称cached应用收益；正常cache profile仍需独立接受。

### 上下文/翻译失效的完成证据

现 `riscv_iommu_post_sync()` 只等 CQH（`:480-533`）；现文件自己说明 CQH 在 decode 前推进，当前 checked helper 只适用于 quiesced/no-ATS/no-S2 的特定 VMA 顺序（`:2148-2162`）。这不是一般 bind/unbind 或有活动 PTW 时的执行完成证明。

C1/C2 开始任何 DMA 前，必须确认 DC 更新、IODIR、相关 IOTINVAL 与必要 fence 已在实际硬件完成；终末恢复也一样。只读到新内存中的 DC 或 CQH 不能代替。可选的实现必须由 owner 席界定真实公开执行回执/充分的协议序列；不得 peek 内部 TL complete、随便加等待周期或假设现 API 已有此回执。本项未闭合时，整条 native 路线仍为设计，禁止性能运行。

## C1：独占 unmanaged + pinned UVA 镜像

任意 malloc 支持需要 `pin_user_pages*`、区间验证、溢出/重叠检查和资源上限。先限制 private anonymous、普通4KiB、无 huge/IO/PFNMAP/COW共享，写预触碰；长期 session 用 FOLL_LONGTERM|FOLL_WRITE（源也 write-pin，避免把共享零页/未破 COW 当稳定所有权），保存确切 page 列表与当前 mm。建立同一 unmanaged 域的 `UVA→page_to_phys(page)`，按页 iommu_map；不靠连续物理页假设。只在新域内选/验证 UVA，绝不把旧 default DMA allocator 的空洞探测当 reserve。

该路线的 I/O 页表独立于 CPU，所以 CPU munmap/remap 不会把正在 DMA 的 PA 偷换：pin 与 I/O 映射保持到 drain。MM/VMA 失效应撤销 publisher、拒绝新 submit、标记任务 revoked；当前任务不能再发布“同VA consumer成功”，但已在途操作可在原 pinned 页安全排空。全撤销后 unmap→完成失效→unpin dirty destination。fd 从另一 mm 使用必须拒绝；用户退出不能令在途页提前释放。无正常 drain 时保留 pin/domain/device owner 到真正 reset/drain，不能用 close 返回替代安全证明。

**更小且建议先作为 C1 对照的版本是 driver-owned coherent allocation + mmap：**

1. 在 default 域仍有效、旧实验已静止时，以 dev0 `dma_alloc_attrs(FORCE_CONTIGUOUS)` 取得固定 arena，并保存原 cpu_addr/dma_handle/size/attrs；用 `dma_get_sgtable_attrs()` 取得 backing pages（`dma-iommu.c:1549-1573`），不能对 remapped VA 盲用 virt_to_page，也不能把 dma_handle 当 PA。
2. `dma_mmap_attrs()` 建 MAP_SHARED 用户映射，源码 `dma-iommu.c:1518-1546` 给出实际页映射；记录原 VMA base，与 arena offsets 合成 UVA。它通常是 PFNMAP/专用 VMA，不能再用普通 GUP 假装 pin；driver 的 allocation 引用负责物理页寿命。分配先于 claim，避免在新 unmanaged 域调用 DMA allocator混淆域。
3. vm_ops open/close 引用计数；VM_DONTCOPY/DONTDUMP/DONTEXPAND；禁止 mremap/分裂/权限变化或明确 revoked，检查 mm 唯一；core vm_ops 支持接口见 `include/linux/mm.h:547-563`。VMA.close 持 mmap_lock，可睡眠，但不在其中反取 mmap_lock 等待工作线程。close 只撤销发布和排 drain，不提前 dma_free。
4. claim/attach 后，新域以该 UVA 映射 arena backing pages；原 default IOVA allocation 保留但不发给 DMA。正常 FINISH detach/恢复确认后再 unmap VMA、释放最后 VMA引用，最后 dma_free_attrs。API 本身要求直到用户映射释放前都不能 free（`kernel/dma/mapping.c:449-459`）。
5. 该版本不接任意 malloc、不接用户页表、不接受指针图跨 arena；它是受限同VA零拷贝，不称完整/generic SVA。即使“coherent”分配也沿用上述显式 WT cache 合同。

C1的owner mm身份以 `mmgrab`/`mmdrop` 保住 `mm_struct`（不以此假装保住用户页表）；实际物理页由driver allocation或FOLL_PIN保住。VMA退出可继续，由revoked状态禁止新供给，独立domain/页保留到drain；正常收尾必须同时满足“无DMA引用、无用户VMA引用、无未完consumer”才free。这样无需把C2的整棵mm页表寿命问题带入C1。

## C2：non-PASID 单 mm 直接进程页表

在 C1 owner/会话与有限任务 ABI 上替换绑定位置。新增私有 `riscv_iommu_bind_mm_did()`/unbind 一对 helper；不是强行调用现 PASID helper。两DID独占同一 mm；CONF.pv=0、PDTV=0、ATS/PRI/S2关闭；同一合法 PSCID 标识共享地址空间，DC.FSC `{Sv39, phys(mm->pgd)>>12}`，DC.TA 按该 PSCID，user PROT。软件提出的静态地址等价与当前 SVA PC.FSC 实现 `riscv/iommu.c:1650-1655`、普通 DC.FSC 写入 `:1588-1594` 一致；实际联合执行仍待验。

新增 domain 必须明确 `pgd_borrowed=true`、不建 io-pgtable ops、不允许 iommu_map/unmap，也不得沿用普通 `domain_free` 去 free `mm->pgd`。`mmget` 保 mm_users/页表活着，pinned registered pages 保实际 I/O backing 活着；两者不能互相替代。每次描述符只允许注册区间，其他进程/同 FD 不同 mm 拒绝。借完整 mm 意味着 DMA 原则上拥有该进程全部 user 页权限，注册区间是可信 driver 的命令约束，不是更细 IOMMU 隔离。

固定 private-anon 4KiB 页、MAP_POPULATE/逐页写预触碰；用 `pin_user_pages*`（API自身设置FOLL_PIN）并传 FOLL_WRITE|FOLL_LONGTERM。查实际 PTE R/W/U/A/D，特别目的页 D 必须已经为1。源只承诺无修改的固定任务，不支持并发 fork/COW、mprotect/munmap/exec、migration/reclaim 重绑定或缺页重试。pin 本身不冻结 UVA→PA，mmap_read_lock 也不是覆盖全部外部 invalidation 的安全替代。预触碰只建立当时的A/D；CPU aging仍可能清A（RISC-V `pgtable.h:542-573`），不是永久置位许可。新helper需证明该有限静态窗口内的A/D稳定，或将其后翻译故障作为失败、不发布consumer；不能把一次prefault当通用缺页SVA完成证据。

**真正必须补的 MMU notifier/退出合同：**

- 注册先于 enable/publish，持合适 mm 引用；invalidate 开始即原子关闭新供给、使 session revoked。若区间/页表改变时仍可能被硬件读取，需要真实停止/排空或失效屏障，不能只把软件状态改成 invalid。当前 `.release` TODO 与忽略失效返回值不能复用。
- pinned backing 保留；共享页表的 secondary TLB/在途 PTW 必须在 Linux 允许释放页表页之前完成真实失效同步。`mmu_notifier.h:189-201` 支持 shared-PT 的 invalidate_range 接口，但要求正确同步语义；它不是可忽略 CQ 错误的许可。
- 若选 invalidate_range_start 撤销完整任务，非 blockable 情况不能睡眠，需按 `mmu_notifier.h:177-182` 返回 EAGAIN；不能同时实现与可失败 start 不兼容的 end 回调。blockable 回调不能在 timeout 后宣告成功并让页表继续拆除；无法 drain 的情况须停留安全阻塞/设备隔离状态，不能把 quarantine 的数据页引用当作保住全部页表页。
- 最小正常应用承诺无映射修改，FINISH 先两 lane 排空、cache acquire/consumer闭包、关闭绑定并确认失效，之后解除 notifier/mm 引用与 pin。异常退出由内核清理路径停止供给、真实 drain 后再拆。缺少可靠隔离/abort 时，不承诺失败退出有界；本接口在该故障接受项未过前不能称生产可用。

这是一组有界必需修改，不是要求先实现通用可分页 SVA。若 root 决定只先验静态成功路径，产物必须明确标 `static-pinned-success-path`，保留异常排空/生命周期未验状态。

## 固定有限 native 任务与用户 ABI 草案

选择一个实际有输出的有限任务：**framed 文件的两 DMA scatter→packet ring→CPU顺序消费→输出 framed 文件**。这是新平台适配与功能接受用例，不冒充 DATE 原应用、不证明工作流性能价值。复用 stage117 的3包、4span、1639B payload/1675B record 形状与缓存所有权合同，不回放其旧测试。

输入每包 `[le64 cookie, le32 len, payload]`；len固定60/568/1011，cookie固定1/2/3；功能 fixture 第 p 包第 q 字节为 `(17*p+q)&255`，p从0计。正式 CLI 接受同格式实际文件，格式/长度错误在 DMA 前拒绝。packet1中间拆65+503；无需任意延迟、压力、相位或新增 2048/64B 许可。

arena 共8个4KiB页：4源span页（每页payload始于+32），3个独占目的包页（record始于+16，12B header 后接payload，即payload始于+28），1页仅CPU metadata；源payload与目的完整record前后各16B guards，不用cacheline padding冒充额外容量。C1映射7个数据页（metadata不映给DMA），C2借用同一进程页表但driver仍只接受这7页的白名单。

| descriptor | lane / DID | 源 | 目的 | bytes / 完成所属 |
|---|---|---|---|---|
| d0 | 0 / CAPS确认10 | arena page0+32 | page4+28 | 60 / packet0 |
| d1 | 1 / CAPS确认11 | page1+32 | page5+28 | 65 / packet1 的第一span |
| d2 | 1 / 同上 | page2+32 | page5+93 | 503 / packet1 的第二span |
| d3 | 0 / 同上 | page3+32 | page6+28 | 1011 / packet2 |

只用公开 NEXT/DONE，一lane最多一job；d2等d1完成，d3等d0完成，其他许可依实际published/owner状态。每包所有span完成后才 cache acquire并发布 packet-complete；CPU consumer 严格0→1→2，逐包校验 header/cookie/guard，计算payload摘要，`writev`输出 header+payload，再 ACK归还本包槽。必要consumer是输出文件本身；单有驱动 verify/统计 PASS 不算交付。只有已到达CPU owner的完整包能消费；部分完成spans不授予整包ownership。

拟新增 UAPI（版本化，固定 struct_size、reserved0、无内部TL观察）：

- `CAPS`：实际 DID/生产配置/地址模式/owner、可用lane、公开完成/错误可见能力，缺项拒绝run。
- `CREATE`：设置 peer，C1 mmap arena或C2注册实际 user arena；先把全部 descriptors/config拷入内核，验证所有offset/len/溢出/别名/权限与page closure。
- `BIND`：取得独占owner、映射/借页表与完成屏障，返回 arena identity/generation；此时仍未触发DMA。
- `PUBLISH`：交付已写退休source及目标空槽的cookie/generation；在白名单内提交有限四job，禁止自动无限aggressor。
- `WAIT_PREFIX`：返回公开 DONE 验证且cache取得的单调完整包前缀；需要 DATA_OK（故障观察/内容均确认），不能只有 DONE。
- `ACK`：消费者已完成包的cookie，允许槽复用；本固定任务无需复用来制造收益。
- `FINISH`：停止供给，双方完整drain、核字节/错误/guards、确认失效/恢复、释放mapping/pin；FD close/进程退出调用同一cleanup状态机，失败保留全部必要资源。

拟 CLI（**尚不存在，不得拿此命令直接执行**）：

```text
idma_packet_task --device /dev/idma0 --peer 1 --address-mode pinned-mm \
  --input three-frames.bin --output delivered-frames.bin --policy two-lane
idma_packet_task --device /dev/idma0 --peer 1 --address-mode sameva-shadow \
  --input three-frames.bin --output delivered-frames.bin --policy two-lane
```

普通强对照具名为 `sameva-shadow/two-lane`，使用同信息、同source-ready、同四descriptor/八页/公开完成与cache合同；`pinned-mm/serial` 与CPU直接packet copy也应保留合法路径。CPU对照无需承担DMA专用全cache acquire；不得用人为相同清空成本或单child限制压弱它。C1/C2共享task调度器才能把绑定位置差异与供给策略差异分开；全部时间先输出 UNKNOWN。

## 资源与完整费用（设计账，非测量）

固定任务有4次合法DMA提交、3次完整packet交接、3次consumer写出与ACK、双方最终drain；有效DMA payload1639B，输出1675B。不用64笔参数或任意额外回放。

- arena backing 8页=32768B；C1 driver-owned版本分配API本身先清零全部arena，需计入。C1还保留原default-domain DMA allocation映射/IOVA reserve 32768B，实际hardware只使用新域7页=28672B shadow映射，不能忽略双套映射准备费用。
- C1若 arena7数据页同落一个2MiB窗口，Sv39独立task页表至少3页=12288B（root/中层/leaf）；跨窗口/1GiB边界会增加，不能预填常量当实测。两group blocking域与default域仍有额外root/对象/ID驻留成本；计实际分配量。
- C2没有新task leaf镜像，但保留mm/原CPU页表、至少8个注册页pin引用、notifier/session/两个owner和可回收PSCID；CPU页表首次分配/预缺页/写触碰/A/D建立不能算免费。借用已有表不等于无映射准备费用。
- descriptor至多4条，源/目的/长度/cookie/lane/packet/generation等元数据按实际sizeof记账，不用64B槽假设；CAPS/CREATE/BIND/PUBLISH/WAIT_PREFIX/ACK/FINISH所有syscall与内核拷贝、软件NEXT/DONE轮询/分派、cache全清/重填、HPM若使用、错误检查均计入。
- 文件input读入/解帧/拷到source、header/guard初始化、source持有期、consumer payload读取/摘要与输出文件内核拷贝、ACK后可能的覆写、最后guards/ownership核验、终末失效/恢复/unpin/free全计；setup与steady阶段可分别列，但完整任务不可漏项。
- 所有周期/ns/吞吐/重叠/收益为UNKNOWN；stage117的258等模型周期不搬成本新Linux时间，旧Image也不是新ABI已执行证据。

## 接受步骤与必须保留的未核项

1. 先接受架构/命名：C1受限同址镜像与C2真借进程页表分别记账；软件不得声称已有SVA或DATE应用复现。
2. 最小内核批次只处理owner/domain transition/borrowed root/free与失效错误传播；无应用性能矩阵。先核真实bind/restore回执，再允许DMA输入。不能仅以能编译为接受。
3. 下批次实现8页有限task ABI与consumer，driver只收固定合法描述符；正常任务只运行一次三包功能输入，序列先冻结。独立原始证据需覆盖CPU UVA与PA、C1域/ C2 FSC=mm->pgd、DID/pv/权限/A/D、全部读写响应及byte-enable、公开NEXT/DONE、cache取得、consumer文件字节及ACK、两个lane最终无在途、失效/恢复与最终释放。
4. 生命周期先做必要定向否例：partial pin/map失败、第二设备claim失败、NEXT=0/信号/超时、mmap关闭或invalidating registered range、输出失败/早关FD。没有实证排空时对应路径必须retained/quarantined，不能PASS。C2页表页寿命否例若无可靠硬件取消/隔离不通过，不冒充生产健壮性。
5. 一次C1同信息同资源对照与C2正常功能接受只回答绑定能力/等价语义，不自动构成时间收益/新意。出现具体可达的完整任务差异之后，才由root另行准入时间预测或native/RTL/FPGA性能工作。

当前未知：真实Linux新ABI尚无；生产FPGA当前装载版本/错误响应观察/一般执行fence回执/异常abort尚无本轮证据；C2完整MM失效与异常退出生命周期尚未实现；真实应用需求/前作价值由root继续裁决。原MIG1792023314保持paused。

## 源与构建版本边界

Linux实际HEAD=`c0f5e13ea81fa30d78f3bb1d6389e2397321e040`，与sources.lock一致，但工作树有idma.c/idma_research.h/UAPI及lowrisc网络本地修改。源码报告针对JSON所列hash，不针对HEAD的干净树。

现有 `out/linux/.config` 含RISCV_IOMMU/IOMMU_SVA/MMU_NOTIFIER、SMP/NR_CPUS64、Zicbom/Svpbmt；`out/linux/include/generated/utsrelease.h` 为 `6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty`。现Image大小24131072B，mtime为2026-10-06 20:36:54 +0900；仅定向hash/metadata，未执行，不能证明今天的idma脏源码已入镜像或当前板已加载。`scripts/platform.py:148-186` 才是当前UAPI staging/bench/build/DTS入口，本轮未调用。
