import json,pathlib,hashlib,datetime
O=pathlib.Path(__file__).resolve().parent
M=json.loads((O/'source-manifest.json').read_text())
D={r['upstream_path']:r for r in M['files'] if 'upstream_path' in r}
# Each quote is a verbatim, deliberately short substring within the indicated source span.
E=[]
def ev(i,p,a,b,q,n):
 r=D[p];s=pathlib.Path(r['path']).read_text().splitlines();assert q in '\n'.join(s[a-1:b]),(i,p,a,b,q)
 E.append({'id':i,'file':r.get('original_path',r['path']),'snapshot':r['path'],'upstream_path':p,'line_start':a,'line_end':b,'sha256':r['sha256'],'quote':q,'meaning':n,'url':r.get('url'),'evidence_type':'source_structure'})
ev('E01','deepspeed/runtime/swap_tensor/utils.py',186,189,"device='cpu'",'池中是CPU tensor；同处调用accelerator.pin_memory。')
ev('E02','deepspeed/runtime/swap_tensor/utils.py',217,226,'self.free_buffer_index.append(self.used_buffer_index[b_id])','逻辑归还池槽，非释放backing/解除设备IOVA。')
ev('E03','csrc/aio/py_lib/deepspeed_cpu_op.cpp',33,40,'_buffer.is_cpu()','CPU且pinned/managed跳过bounce；然后调用contiguous。')
ev('E04','csrc/aio/py_lib/deepspeed_cpu_op.cpp',40,40,'_cpu_buffer.contiguous()','非contiguous输入可能额外创建数据存储。')
ev('E05','csrc/aio/py_lib/deepspeed_cpu_op.cpp',45,65,'if (_buffer.is_cuda())','GPU目标的finish copy是条件支路，不是CPU池的必经支路。')
ev('E06','csrc/aio/common/deepspeed_aio_utils.cpp',112,129,'mlock(ptr, size)','managed bounce按CPU系统页对齐、mlock，未调用NVMe DMA API。')
ev('E07','csrc/aio/py_lib/deepspeed_pin_tensor.cpp',14,20,'munlock(iter->first, iter->second);','managed locked tensor析构解除CPU页锁并free。')
ev('E08','csrc/aio/common/deepspeed_aio_common.cpp',267,283,'O_DIRECT','普通构建的read/write open包含O_DIRECT。')
ev('E09','csrc/aio/common/deepspeed_aio_common.cpp',270,273,'#if defined(__ENABLE_CANN__)','CANN编译条件会移除O_DIRECT。')
ev('E10','csrc/aio/common/deepspeed_aio_utils.cpp',41,61,'io_prep_pread','按block_size、host ptr和文件offset构造IOCB。')
ev('E11','csrc/aio/common/deepspeed_aio_utils.cpp',77,101,'io_prep_pwrite','overlap generator继续按每块构造IOCB。')
ev('E12','csrc/aio/py_lib/deepspeed_aio_op_desc.cpp',18,26,'buffer.nbytes()','每线程分配的是同一tensor字节切片。')
ev('E13','csrc/aio/common/deepspeed_aio_types.cpp',60,76,'io_queue_init(queue_depth, &_io_ctxt);','context在handle/worker构造；析构queue release。')
ev('E14','csrc/aio/common/deepspeed_aio_types.cpp',71,76,'io_queue_release(_io_ctxt);','IOCB对象和native AIO context生命周期。')
ev('E15','csrc/aio/py_lib/deepspeed_py_io_handle.cpp',176,195,'completed_op->finish();','wait等所有线程完成后finish再close fd。')
ev('E16','csrc/aio/py_lib/deepspeed_py_io_handle.cpp',188,188,'close(completed_op->_fd);','正常wait逐tensor操作close文件。')
ev('E17','csrc/aio/py_lib/deepspeed_py_io_handle.cpp',164,174,'assert(0 == _num_pending_ops);','handle析构stop/join要求pending先已清零，并不自动drain。')
ev('E18','csrc/aio/py_lib/deepspeed_py_io_handle.cpp',299,310,'false, true','async入口设置validate=false、async=true。')
ev('E19','csrc/aio/common/deepspeed_aio_common.cpp',112,126,'assert(n_completes >= min_completes);','只核事件数量，不读event.res/res2。')
ev('E20','csrc/aio/common/deepspeed_aio_common.cpp',71,110,'assert(submit_ret > 0);','submit只要求正值，不要求==本批n_iocbs。')
ev('E21','fs/aio.c',1459,1468,'iocb->ki_res.res = res;','kernel向事件写入实际结果，native wrapper未核该字段。')
ev('E22','fs/aio.c',1534,1598,'call_read_iter','文件AIO先调用filesystem read_iter/write_iter，非直接指定NVMe device。')
ev('E23','lib/iov_iter.c',1743,1773,'pin_user_pages_fast(addr, maxpages, gup_flags, *pages);','user-backed direct IO额外按请求页提取取得pins。')
ev('E24','fs/direct-io.c',167,174,'iov_iter_extract_pages','经典DIO路径提取请求所需页。')
ev('E25','fs/iomap/direct-io.c',151,181,'bio_release_pages(bio, false);','iomap direct IO完成释放本请求页；read dirty路径可能延后release。')
ev('E26','block/bio.c',1317,1354,'bio_set_flag(bio, BIO_PAGE_PINNED);','bio封装每次请求的页pin，不是池全生涯映射。')
ev('E27','block/blk.h',444,448,'unpin_user_page(page);','BIO_PAGE_PINNED按完成回收页pin。')
ev('E28','block/blk-map.c',630,664,'copy = true;','request用户映射接口在bounce/alignment等情况下可复制；不据此声称选定filesystem走该接口。')
ev('E29','drivers/nvme/host/pci.c',730,802,'dma_map_bvec','简单request data在提交前map。')
ev('E30','drivers/nvme/host/pci.c',792,818,'dma_map_sgtable','一般request收集SG、map、编码PRP/SGL，失败可回收。')
ev('E31','drivers/nvme/host/pci.c',540,562,'dma_unmap_sgtable','payload请求结束unmap且回收PRP/SGL和SG资源。')
ev('E32','drivers/nvme/host/pci.c',961,980,'nvme_pci_unmap_rq(req);','completion先unmap data再向上完成request。')
ev('E33','drivers/nvme/host/pci.c',2936,2936,'get_device(&pdev->dev);','所有read/write queue使用controller PCI device DMA主体。')
ev('E34','drivers/nvme/host/pci.c',1512,1513,'dma_alloc_coherent','队列descriptor的coherent allocation生命周期与payload mapping不同。')
ev('E35','drivers/iommu/riscv/iommu.c',1303,1309,'pci_dev_id(to_pci_dev(dev));','本地RISC-V DID取自PCI function或firmware ID，非handle/thread/queue id。')
ev('E36','drivers/iommu/dma-iommu.c',730,753,'iommu_dma_alloc_iova','translated DMA API在map时分配IOVA并建立页表映射。')
ev('E37','drivers/iommu/dma-iommu.c',1318,1328,'iommu_map_sg','一般SG走一段连续IOVA，并由物理片段和页尺寸控制mapping。')
ev('E38','drivers/iommu/dma-iommu.c',707,727,'if (!iotlb_gather.queued)','unmap必须移除映射；strict立即sync，FQ可能排队。')
ev('E39','drivers/iommu/dma-iommu.c',148,152,'flush_iotlb_all','FQ最终可按批flush，不等于每次unmap单独全清。')
ev('E40','drivers/iommu/riscv/iommu.c',1175,1186,'return false;','当前capable没有DEFERRED_FLUSH case。')
ev('E41','drivers/iommu/dma-iommu.c',617,620,'domain->type = IOMMU_DOMAIN_DMA;','不支持deferred能力时FQ退为strict DMA域。')
ev('E42','drivers/iommu/riscv/iommu.c',1730,1784,'// riscv_iommu_cmd_inval_set_pscid(&cmd, domain->id);','本地driver当前普通VMA没有设置PSCID，逐地址代码也注释。')
ev('E43','drivers/iommu/riscv/iommu.c',1768,1770,'riscv_iommu_post(domain->iommu, &cmd);','范围函数实际post一条未设置地址的命令，随后IOFENCE；硬件行为/成功未知。')
ev('E44','drivers/iommu/riscv/iommu.c',1799,1809,'riscv_iommu_flush_iotlb_range','map成功sync回调也调用同范围函数。')
ev('E45','drivers/iommu/iommu.c',2415,2417,'ops->iotlb_sync_map(domain, iova, size);','core单范围成功map调用该回调。')
ev('E46','drivers/iommu/iommu.c',2558,2559,'ops->iotlb_sync_map(domain, iova, mapped);','core SG成功map调用该回调。')
ev('E47','kernel/dma/mapping.c',116,126,'if (likely(!ops))','无DMA ops可能直接DMA；bypass另受config、mask、bus limit条件控制。')
ev('E48','drivers/iommu/iommu.c',2693,2699,'IOMMU_DOMAIN_IDENTITY','iommu.passthrough可请求identity域，是否成功须核运行。')
ev('E49','drivers/iommu/dma-iommu.c',1616,1636,'if (iommu_is_dma_domain(domain))','只有DMA domain成功初始化才安装iommu_dma_ops。')
ev('E50','Documentation/core-api/dma-api-howto.rst',615,615,'dma_sync_single_for_cpu(dev, dma_handle, size, direction);','合法长期streaming映射允许CPU/device sync。')
ev('E51','Documentation/core-api/dma-api-howto.rst',627,627,'dma_sync_single_for_device(dev, dma_handle, size, direction);','CPU改过数据后再次交设备需sync和正确direction。')
ev('E52','Documentation/core-api/dma-api-howto.rst',642,645,'After the last DMA transfer','合法映射复用最后才unmap，不能把持久映射当非法对照。')
ev('E53','drivers/iommu/iommu.c',2296,2311,'(iova ^ paddr)','大IOMMU leaf需要IOVA/物理地址相对alignment和足够长度，CPU hugepage不自动满足。')
ev('E54','drivers/iommu/iommu.c',2326,2333,'iommu_pgsize','实际DMA leaf尺寸由domain支持、物理/IOVA对齐及长度选择。')
ev('E55','deepspeed/runtime/swap_tensor/pipelined_optimizer_swapper.py',34,37,'self.aio_handle.wait() == self.num_ops','源层的完成检查按tensor操作数。')
ev('E56','deepspeed/runtime/swap_tensor/pipelined_optimizer_swapper.py',160,165,'self.swap_buffer_manager.free','wait之后归还逻辑池槽。')
ev('E57','deepspeed/runtime/swap_tensor/optimizer_utils.py',126,127,"os.path.join(base_folder, 'optimizer'",'交换文件位于同base_folder/rank路径，不自带硬件分流。')
lookup={x['id']:x for x in E}
claims=[
 ('C01','固定CPU pinned池不等于持续NVMe DMA/IOVA映射','source_structure + conditional_deduction','池的backing持有到allocator/tensor引用生命周期结束；free只是槽位可重用。NVMe payload的映射所有者与单位则是kernel request，并在completion unmap，因此两种生命周期不同。既不能把池常驻视为NVMe持久映射，也不能从每request unmap推出物理页在每tile被重新分配。',['E01','E02','E29','E30','E31','E32']),
 ('C02','CPU页锁和bounce必须分别计价','source_structure','selected CPU contiguous pinned/managed tensor可以直接作为AIO数据区；非CPU或未pinned输入可能先分配CPU bounce、write前copy、read finish时copy回并free。contiguous()对非contiguous输入还可能形成额外存储。managed page-aligned alloc只调用posix_memalign+mlock，析构munlock+free；是否另有GPU注册由runtime allocator决定，源码不足以推成NVMe映射。',['E03','E04','E05','E06','E07']),
 ('C03','O_DIRECT是条件源码结构，不是DMA注册或持久化许可','source_structure + conditional_deduction','通常read用O_RDONLY|O_DIRECT，write用O_WRONLY|O_CREAT|O_DIRECT；CANN条件去掉O_DIRECT。该open没有O_SYNC/O_DSYNC，选定async路径未见fsync/fdatasync。因此direct数据传输、device DMA映射和断电持久化是不同合同；write completion/close不能单独证明crash durability。实际filesystem、编译宏、direct-IO支持和文件alignment未知。',['E08','E09','E16']),
 ('C04','IOCB、CPU线程和native AIO context是软件提交资源','source_structure','一个tensor分给p个线程，线程按block_size生成host-ptr/fd/offset的IOCB；每context按q分配IOCB/event、初始化native AIO context并在析构release。read/write handles及其线程不携带device DID；它们可改变供给、排队、提交重叠，不能增加物理requester。',['E10','E11','E12','E13','E14','E33','E35']),
 ('C05','wait、close和pool归还具有确定的正常路径次序','source_structure','正常wait等每个worker的完成队列，调用finish，然后close fd、减少pending；optimizer swap完成先wait再free池槽。handle析构先stop/join，stop断言pending==0，调用者必须先完成清理，不能假定析构自动等待未完成I/O。Torch/accelerator池最终backing释放或allocator cache何时退还OS不在本次冻结DeepSpeed源码内。',['E15','E16','E17','E55','E56','E02']),
 ('C06','完成操作数不能代替完整字节成功','source_structure','native io_pgetevents只核返回事件数量，未逐项检查event.res/res2；kernel完成事件会提供真实res。submit只断言返回正值，批量提交未核submit_ret==n_iocbs；async wrappers的validate=false。因此本轮只能确认源层期待全部操作完成，不能据wait数目声称每块成功、无短读/错误或数据正确。这是接受证据缺项，本关口不修代码。',['E19','E20','E21','E18','E55']),
 ('C07','file AIO到NVMe之间仍有实际filesystem/block层','source_structure + conditional_deduction','kernel AIO调用对应file read_iter/write_iter；经典DIO和iomap是可见的两种直接依赖路径，不声称本机nvme_path必走某一个。用户页的request pins由iov_iter提取，bio完成或dirty-work回收；这是AIO传输生命周期，不是app整池长期注册。blk-map存在bounce/copy规则，但没有实际filesystem证据时不能强行把blk_rq_map_user_iov插入普通file-AIO调用链。',['E22','E23','E24','E25','E26','E27','E28']),
 ('C08','NVMe payload映射单位是每个driver request','source_structure','nvme_prep_rq依据本request物理segments调用nvme_map_data；简单data用dma_map_bvec，一般用request SG的dma_map_sgtable并编码PRP/SGL。完成路径先unmap再nvme_complete_rq；错误支路也回收。软件tensor操作/IOCB可以经filesystem及block层拆分或合并，不能固定为一个tensor一次mapping，也不能直接按AIO block数量宣称实际IOMMU map/walk数量。SQ/coherent descriptor可有较长生命周期，与payload不同。',['E29','E30','E31','E32','E34']),
 ('C09','translated DMA路径确有IOVA分配和映射，但实际选路未知','source_structure + conditional_deduction','在iommu_dma_ops的map_page/map_sg路径，代码按domain granule分配IOVA、建立映射并处理sync；SG可合成连续IOVA，SWIOTLB及P2PDMA另有条件分支。CPU ptr/pin不固定返回的IOVA，回收后再次取得相同IOVA也不代表映射从未解除。实际nvme_path、controller、domain/ops、coherency和bounce条件需要运行配置证据，本轮没有测量。',['E36','E37','E38','E47','E49']),
 ('C10','不能泛称每个unmap必定清除全部硬件TLB','source_structure + conditional_deduction','通用dma-iommu解除范围映射后，strict立即iotlb_sync，DMA-FQ可将IOVA和失效延后/合批，最终调用driver flush。失效的地址、domain、ASID范围和硬件行为依driver与IOMMU而定；解除映射不等于所有其他映射的硬件缓存必全失。没有运行配置、有效命令轨迹、hit/miss/walk及完成量测，loss保持unknown。',['E38','E39']),
 ('C11','本地RISC-V驱动有宽失效源码事实，但不能迁移为native NVMe实测事实','source_structure + conditional_deduction','本地flush_iotlb_range的普通VMA PSCID及逐地址代码被注释，实际post未设置地址的IOTINVAL并调用IOFENCE；map成功回调也到同函数。当前capable不返回DEFERRED_FLUSH能力，通用初始化会把DMA_FQ退为strict DMA。若所选NVMe真实使用这个driver、有效translated domain且提交成功，就存在宽失效命令路径；不能断言真实GPU/NVMe已使用它、每命令成功、硬件全缓存清空或已有性能损失。该驱动范围/接口问题不在本关口修复，也不是新研究机制的收益证据。',['E40','E41','E42','E43','E44','E45','E46']),
 ('C12','IOMMU bypass/identity可能令翻译缺口根本不存在','source_structure + conditional_deduction','DMA API无ops、允许的dma_ops_bypass+足够mask/bus limit、平台direct分支及identity/passthrough都是相邻反条件；iommu_setup_dma_ops只为成功初始化的DMA domain安装iommu DMA ops。当前源码不能确定实际机器的NVMe是否经过host IOMMU；不能因为打开O_DIRECT或调用dma_map API就称它已经过地址翻译。',['E47','E48','E49']),
 ('C13','同一PCI NVMe controller上的read/write不是两个DID','source_structure + conditional_deduction','NVMe queue最终关联同nvme_dev，DMA API device为该controller的pdev->dev；本地RISC-V PCI DID由PCI function取得，非read/write handle、software thread、queue id。若所有swap文件真实落在同PCI NVMe controller，读写可重叠并竞争但只有这个physical requester。base_folder不能证明后端只一设备，也不能证明两个设备：RAID、device-mapper、NVMe-oF等配置未知，不另选这些工作流。',['E33','E35','E57']),
 ('C14','所选CPU swap缓冲区不强制GPU同时通过主机IOMMU','source_structure + conditional_deduction','CPU池路径与cpu_op条件足以否决“NVMe read/write+GPU天然同时作为两个请求器”的推断。GPU目标的finish copy只在对应tensor条件分支中；GPU allocator pin/registration本身也不等于有并行GPU DMA。GPU参与CPU optimizer inner-loop、GPU requester/ATC/IOMMU拓扑及同时发生的DMA均unknown，计算与完整数值合同由root独占。',['E01','E03','E05','E33']),
 ('C15','合法普通持续映射必须保留为强对照','source_structure + conditional_deduction','Linux DMA API本就允许streaming区域map一次，多次DMA间按CPU/device所有权sync，最后unmap。方向必须覆盖NVMe read写主存与NVMe write读主存，CPU更新不得在device所有权期间访问；需稳定页生命周期、权限和错误/撤销/停止后回收。可行驱动/注册接口的普通持续映射不是非法对照。现有DeepSpeed file AIO只传fd、host ptr、byte length、offset，未提供保留/复用NVMe DMA mapping的用户接口；仅app pin_memory/mlock/O_DIRECT不能改变stock pci.c完成时unmap。实现需实际driver/API或有界stack改变及全部成本，不在本关口实施。',['E50','E51','E52','E10','E29','E31','E32']),
 ('C16','保留CPU residency、batch/layout和大页普通控制的合法空间','source_structure + conditional_deduction','按真实H与同完成/持久化合同评估CPU状态驻留；若工作集可合法保留，应计其少读/少写及最终输出成本。batch、block_size、队列深度、线程数、文件布局和alignment可改变普通I/O供给/拆分；CPU hugepage并不自动等于大IOMMU leaf，大leaf还需domain支持、物理连续/IOVA对齐、请求范围足够。不能人为冻结这些普通控制来制造新机制收益，也不能宣称在未知H/硬件上它们已胜出。',['E10','E11','E36','E53','E54'])]
quotes={'scope':'stage113 sole DeepSpeed NVMe AIO/Linux DMA-IOMMU boundary','frozen_versions':{'deepspeed':M['deepspeed'],'linux_commit':M['linux']['commit']},'quotation_policy':'Short exact substrings; surrounding full original source retained at cited snapshot paths. Evidence spans are inclusive 1-based source line numbers.','entries':E}
(O/'critical-quotes.json').write_text(json.dumps(quotes,ensure_ascii=False,indent=2)+'\n')
md=['# stage113：已选 ZeRO-Infinity NVMe AIO → Linux DMA/IOMMU 边界','', '裁决：保留原生 tiled CPU-Adam swap 的来源结构和 request 级 DMA 生命周期；拒绝从 pinned 池推出持续 NVMe IOVA、从两个 AIO handle 推出两个 DID、从 unmap 推出每次全部 TLB 必失效或已测 loss。实际物理后端、翻译域、自然并行 peer、参数/H与运行完成证据缺失。本报告不准入 G0/G1，也不修驱动、不改生产、不实验。','', '## 范围与版本','',f"- DeepSpeed v0.16.4 / `{M['deepspeed']['commit']}`，tree `{M['deepspeed']['tree']}`；19个已用源码blob与官方固定tree逐一匹配。",f"- 本地Linux `{M['linux']['commit']}`，Makefile为6.5.0-rc2；18个审读源文件均与HEAD一致。旧dirty文件原样保留，见source-manifest.json。该源码副本不是所选NVMe工作负载实际运行Linux的证明。", '- root独占stage3 optimizer计算/数值、完整byte contract、FPGA投影及最终接受；本席只处理AIO/内核/硬件身份边界。未调用DeepSeek、Claude或其它子席。', '- 原始404 GET保存在source/parent-manifest.json；本席web open失败和一次不同方法定向恢复见failed-fetches.json。只增本worker目录内证据/报告；无生产改动、构建、测试、板操作。','', '## 核心结论与原始证据','']
for i,title,kind,body,ids in claims:
 md += [f'### {i} {title}','',f'判定类别：`{kind}`。{body}','']
 for k in ids:
  e=lookup[k];md += [f"- {k}: [{e['upstream_path']}:{e['line_start']}–{e['line_end']}]({e['snapshot']}:{e['line_start']})；SHA-256 `{e['sha256']}`；原文：`{e['quote']}`。"]
 md+=['']
md += ['## 完整生命周期与应计成本','', '| 阶段 | 可确认的结构/必须计入 | 未确认的量 | 证据 |','|---|---|---|---|',
'| 池准备 | CPU tensor分配/zero、pin；pool backing与逻辑slot分别持有 | 真实H、allocator锁页/注册/缓存生命周期和次数 | C01/C02 |',
'| 一次tensor AIO操作 | 文件stat/read size、open O_DIRECT、descriptor tensor引用、p线程工作入队 | 实际文件/FS路径、操作数、open/metadata耗时 | C03–C05 |',
'| 可选bounce/copy | 未pinned或非CPU输入的CPU bounce、write前copy、read后copyback；contiguous可能复制 | selected pinned contiguous切片可跳过；allocator特殊条件未知 | C02 |',
'| native AIO contexts | 每handle有base context和p个worker contexts、每context q个IOCB/event；初始化/释放 | libaio内存、q/p实际配置，提交/收割成本 | C04 |',
'| 文件到block | filesystem extent/metadata、用户页request pin、bio装配、拆分/合并、可选bounce | actual FS、block设备链、request数、页数 | C07 |',
'| NVMe准备 | SG/mempool、data DMA map、IOVA/PTE、PRP/SGL列表、CPU/device sync；SQ/CQ等长期descriptor资源 | coherency、P2P/bounce、mapping leaf、实际map个数与成本 | C08/C09 |',
'| 硬件完成 | NVMe请求完成后先payload unmap并回收SG/PRP，才向上完成 | controller DMA bytes、media/queue服务、共享IOMMU的miss/walk/失效 | C08–C13 |',
'| 上层reap | event返回res；worker完成队列；native wait后finish/close；pipeline按操作数检查 | 短IO/error核验、所有字节正确与最后输出完成 | C05/C06 |',
'| tile回收 | wait后逻辑slot归还；CPU pinned backing通常仍由池持有 | terminal allocator退还OS/解除注册及其成本 | C01/C05 |',
'| 终止 | pending先清零，stop/join，IOCB/context release，managed locked tensors munlock/free | 错误/取消流程成功、线程/所有权quiet、SSD缓存持久化 | C03/C05 |',
'| 普通持续映射对照 | 稳定页/IOVA注册、双方向权限、CPU/device sync、最终停止并unmap | 必须给实际driver接口与全部setup/teardown、驻留/IOVA资源 | C15 |','',
'## 正条件、相邻反条件和最小缺项','', '| 待接受能力 | 有证据的正条件 | 相邻反条件 | 最小缺项 |','|---|---|---|---|',
'| 原生file-AIO触达翻译 | swap文件实际落在PCI NVMe，经成功安装的translated DMA ops；有效request map/done/unmap | identity/bypass、无IOMMU；FS fallback/非NVMe后端；只见O_DIRECT/CPU pin | 实际配置、FS/block链、PCI function/DID、domain/ops与完成证据 |',
'| 自然多requester共享翻译 | 来源工作负载本身存在独立物理DMA peer，实际同时经同IOMMU并有可识别DID/事务 | 同controller的read/write queues仍一个requester；CPU计算不是DMA peer；GPU copy无证据 | 在既定source内的自然peer及共享路径/重叠完成证据；不能另造peer或第二workflow |',
'| 有翻译性能缺口 | 真实完成/正确字节合同成立，实际归因揭示翻译等待/失效/容量对同预算合法普通控制仍有限制 | SSD媒体/队列/FS/CPU供给/bounce主导；普通residency/layout/largeleaf/合法persistent mapping复现 | 真实H/参数、可执行普通对照和分量证据；本关口均未实验 |',
'| 持久映射为真实普通控制 | driver/API保留数据区DMA地址且有合法CPU/device sync及终止释放 | app只pin/mlock，不改变stock NVMe请求完成时unmap；非法CPU与DMA同时写 | 具体注册接口、权限/方向、pin寿命、request复用/错误/取消/最终回收合同 |',
'| 投影到现FPGA | root证明任务byte/compute/所有权合同、自然实际requester/共享路径及普通控制外新事实 | 为模拟native人为让原本合法FPGA整池映射逐tile remap；把一个NVMe拆成多个假DID | root独占；本席不扩FPGA压力、不恢复R107/C112、不自动开始G0/G1 |','',
'## unknown清单','', '- 实际job配置、tile尺寸/数目、read/write pipeline开关、p/q/block_size、H、rank数、保存/持久化合同。完整byte和计算部分交root。', '- 实际nvme_path、mount/filesystem、block backend、NVMe controller PCI function、DID、request队列路径和kernel版本。', '- 实际IOMMU instance/domain/ops、bypass/identity/translated、IOVA地址/leaf、coherency、SWIOTLB/P2P、失效命令成功与范围。', '- 自然第二physical DMA requester、GPU是否实际出现在此CPU optimizer inner-loop并有host-IOMMU共享与同时DMA。', '- 完整request/IOCB byte成功、terminal outputs、quiet与最终释放；仅源wait数量不是证据。', '- IOATC hit/miss/walk、translation loss、SSD/FS/CPU/复制/注册/映射分量和收益。未实测，保持unknown。','',
'## 导航','', '- `critical-quotes.json`：E01–E57原始短引文、精确行号、完整hash、原文件与冻结snapshot路径。', '- `source-manifest.json`：40个source/metadata入口，Linux HEAD/dirty状态及固定DeepSpeed tree/blob一致性。', '- `source/parent-manifest.json`、`source/parent-version.json`、`failed-fetches.json`：初始失败GET、固定版本与本席定向恢复证据。', '- 本目录report生成/来源核验属于产物完整性检查；没有运行workload/test/build或测板。','']
(O/'report.md').write_text('\n'.join(md))
S={'stage':'stage113','owner':'/root/s113_nvme_dma_boundary','status':'source_interface_adjudication_complete','verdict':'retain_native_source_contract_conditionally; physical/native-IOMMU execution and multi-requester admission unknown; no G0/G1 admission by this worker','accepted':['Pinned CPU swap pool lifetime differs from request-granular NVMe streaming DMA mapping.','PCI controller device is DMA requester; software handle/thread/queue does not allocate a DID.','Ordinary legal persistent streaming mapping remains admissible if an actual driver/API, sync, direction and complete lifetime cost exist.'],'rejected_inferences':['two handles imply two NVMe devices/DIDs','GPU DMA necessarily overlaps selected CPU optimizer loop','every unmap empties all TLBs','source wait count proves full successful bytes','native IOMMU loss or FPGA admission has been measured'],'report':str(O/'report.md'),'quotes':str(O/'critical-quotes.json'),'manifest':str(O/'source-manifest.json'),'minimal_missing':['actual nvme_path/FS/block/PCI requester/domain topology','natural independent DMA peer and shared translated path in selected source','actual parameters/H and legal ordinary controls','full byte/completion/ownership/terminal release evidence'],'runtime_measurements':None,'production_changes':[],'builds':[],'tests':[],'board_operations':[],'model_calls':['Codex gpt-6.1-sol/ultra only'],'generated_utc':datetime.datetime.now(datetime.timezone.utc).isoformat()}
(O/'summary.json').write_text(json.dumps(S,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'report':str(O/'report.md'),'report_lines':len((O/'report.md').read_text().splitlines()),'claims':len(claims),'evidence_count':len(E)},ensure_ascii=False,indent=2))
