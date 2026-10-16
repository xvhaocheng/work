# SPDK v24.09：真实 DMA 映射链与普通对照

2026-10-15。**SPDK `perf` 的普通起点是固定 task 缓冲的完成驱动复用；4KiB IO 不意味着每笔新增4KiB映射，也不意味着IOMMU用4KiB leaf。** 因而先给合法大页、常驻pool、完成驱动补发和普通队深/提交控制完整机会。源码没有证明这些控制在某个具体部署中已足够，也没有证明它们失败。保留来源约束下的小模型空间；真实部署、actual leaf、共享IOTLB/PTW及性能效果全部未取得。

本席独占映射与普通对照；不复核旧M0/六旧日志、不承担事件采集或root创新裁决。ARS-Codex / deep-research / fact-check，限定官方源码证据核查。请求配置 `gpt-6.1-sol / high`；实际模型/强度的独立运行元数据本席未取得，字段为null，由root核对。Codex-only，无DeepSeek/Claude；0应用运行、模型运行、构建、设备探测、板操作、生产源码或配置修改、goal操作。完整原件、hash、读取范围和获取失败见[sources.json](sources.json)。

## 1. 固定版本与可证明范围

- SPDK v24.09固定 `19524ad45373e35d5fdc06f4e081444d87ceed80`，复用既有 `perf.c` 与 `system_configuration.md` 原件及来源卡S1。
- 官方gitlink定位到 `spdk/dpdk` 的 `8d8db71763ccc7f092e3d66dbc8b32deaaf69fb4`，不是随意套一个DPDK最新版；见[gitlink原件](sources/spdk_dpdk_gitlink.json)。
- Linux `v6.12` 两个原件只用来展示一个明确、可核查的VFIO type1 → IOMMU实现链，**不是该SPDK版本声明的唯一支持内核，也不是选定机器的已部署内核**。不把这段参考代码反推成Dell、NIC或SSD实况。[VFIO参考源码](https://github.com/torvalds/linux/blob/v6.12/drivers/vfio/vfio_iommu_type1.c)、[IOMMU参考源码](https://github.com/torvalds/linux/blob/v6.12/drivers/iommu/iommu.c)。

## 2. 从 task 到设备 DMA 地址

### 2.1 task池与复用

`perf.c:1603–1630,1737–1758`：每个worker的每个namespace context初始分配 `g_queue_depth` 个task并提交。`804–844` 的 `nvme_setup_payload` 按 `max_io_size_bytes = g_io_size_bytes + g_max_io_md_size*g_max_io_size_blocks` 分配payload；`max_io_md_size`非零时还分配独立metadata buffer。`1471–1532,1535–1576`：完成减少队深，非draining时用**同一个task及其iovs**调用 `submit_single_io(task)`，只有draining/失败路径释放。随机模式在 `1480–1495` 选择namespace偏移，未重新分配payload。[固定perf原文](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/app/spdk_nvme_perf/perf.c#L804)。

因此正常无错、无drain的这条PCIe路径中，连续完成/补发不是每笔pin/map/unmap。应用task释放与DPDK底层memseg解除映射也不是同一粒度；memseg释放事件才走下面的unregister/unmap链。跨进程重启、pool扩缩、hotplug或异常恢复的地址稳定性不由这条复用事实保证。

`Q*payload_bytes`只能作为每context的payload请求字节预算，另加metadata、allocator填充、task/队列/tracker和实际memseg。它不是整个进程的映射字节，也不是IOTLB条目数。多个4KiB task可落在同一个2MiB backing中；task布局也可能跨多个memseg。未取得运行地址，故actual footprint=null。

### 2.2 allocator与软件VA→IOVA

`env.c:45–61,81–97,111–115`：`spdk_dma_zmalloc_socket → spdk_zmalloc → rte_zmalloc_socket`，指定NUMA失败还可能回退任意socket；释放进入`rte_free`。这段allocator自身没有逐IO VFIO ioctl。[固定env源码](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/lib/env_dpdk/env.c#L45)。

`memory.c:687–747`：初始化遍历DPDK memseg并注册；内存ALLOC/FREE事件分别触发`spdk_mem_register/unregister`。`332–390`的软件注册表要求2MiB对齐与2MiB整倍数；`947–963`从`rte_memseg.iova`取设备地址，不应被局部变量名`paddr`误导。`1046–1177`区分DPDK管理与外部内存：**DPDK分支只填SPDK软件translation表**，外部分支才按IOVA=VA/PA等条件调用SPDK自己的VFIO map。`1512–1538`的`spdk_vtophys`查询软件表再加偏移。[固定memory源码](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/lib/env_dpdk/memory.c#L687)。

重要边界：这里2MiB是SPDK软件注册/查询组织，不是IOMMU硬件leaf承诺。`spdk_vtophys()`也是软件查表，不是给硬件IOTLB发一条预取。`1516–1518`在无hugepage模式直接返回错误，所以不能把`perf`通用usage里的`--no-huge`无条件当本条PCIe DMA路径的合法4KiB对照。

### 2.3 DPDK PCI/VFIO映射与Linux pin

固定DPDK `eal_vfio.c:802–918`：设备所属VFIO group加入container，primary process在首个active group选择IOMMU type；default container的既有memseg由`dma_map_func`映射，之后注册内存事件回调。`1399–1477`的type1链遍历memseg，提交包含`vaddr/iova/len`及READ/WRITE权限的`VFIO_IOMMU_MAP_DMA`；`606–657`在后续ALLOC/FREE按memseg粒度map/unmap。IOVA=VA时同样需要映射，不表示绕过翻译。No-IOMMU是代码中的另一个类型，不能因使用VFIO就自动认定translated。[固定DPDK源码](https://github.com/spdk/dpdk/blob/8d8db71763ccc7f092e3d66dbc8b32deaaf69fb4/lib/eal/linux/eal_vfio.c#L802)。

SPDK `memory.c:772–840,1379–1422`另有外部内存映射记录、refcount与首个SPDK设备加入时的补映射；这不能替换上述DPDK管理payload的分支。原文`system_configuration.md:90–135`描述首设备attach后的内存映射及memlock限制；Linux v6.12的更具体实现为`vaddr_get_pfns:550–569`调用`pin_user_pages_remote(...FOLL_LONGTERM...)`，`vfio_pin_pages_remote:597–689`检查连续PFN并做RLIMIT_MEMLOCK记账，`vfio_pin_map_dma:1441–1489`循环pin连续段并映射。应写“文档称mlock约束；参考实现实际是long-term pin/locked_vm记账”，不能声称已观察到部署调用了mlock系统调用。

### 2.4 PRP/SGL与IOMMU leaf分开

`nvme_pcie_common.c:28–36`：真实PCIe路径用`spdk_vtophys`取得设备地址。`1195–1289`按controller的`page_size`组织PRP并写入命令；`1320–1357`的SGL按软件查询返回的连续段组织。`139–231`还显示SQ/CQ/tracker/PRP列表也有设备地址和DMA相关内存。故不能只数payload翻译而漏控制队列/descriptor；具体设备fetch和cache行为仍需实际事件。[固定PCIe common源码](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/lib/nvme/nvme_pcie_common.c#L1195)。

IO大小、PRP页粒度、CPU backing页、SPDK软件注册单位、VFIO map区间、IOMMU leaf是六个不同对象。即使PRP以4KiB片段组织，设备地址仍可被一个较大IOMMU leaf覆盖。

## 3. 何时4KiB，何时普通大页/常驻pool可能足够

Linux v6.12 `vfio_iommu_type1.c:1414–1474`把连续PFN段传入`iommu_map`；`iommu.c:2388–2498`的选择受domain `pgsize_bitmap`、IOVA与PA共同对齐、剩余长度限制，再选允许的最大页交给driver `map_pages`。这给出**条件**，不给部署的actual leaf证明。

| 条件 | 可以作的判断 | 不能偷换的判断 |
| --- | --- | --- |
| 2MiB backing、IOVA/PA对齐、足够连续长度、driver/domain支持2MiB并实际安装大leaf，权限/撤销允许 | 合法大页是必须执行的普通对照 | CPU hugepage配置本身不证明上述条件全满足 |
| domain只支持4KiB，或连续段/对齐/映射边界限制到4KiB；或确有同语义的细粒度权限/撤销要求 | 4KiB是可说明的部署/研究条件 | 不能仅因IO=4KiB、PRP=4KiB或研究旧模型=4KiB而宣称原部署必须4KiB |
| pool稳定且用实际leaf覆盖后的工作集可驻留；没有持续冲突/失效驱逐 | 常驻pool+正常补发可能已足够；须实际验证 | pin/map常驻不保证IOTLB或walk cache永不被其他流驱逐 |
| pool常驻但多个流共享有限缓存/队列/带宽，冲突或失效持续 | 仍可能存在共享翻译问题 | 不能从“可能”填入实际干扰/反号/收益 |

参考VFIO还有`disable_hugepages`开关（L52–56、L480–491、L681–682）可使连续pin合批受限。这是参考实现的可检查因素，未获授权修改或探测，更不是本次已采用的deployment配置。不同内核、driver、IOMMU、iommufd或外部env实现需重新绑定实现证据。

## 4. 强普通对照先行：动作、可用性与完整代价

| 普通动作 | 原部署可用性/已核接口 | 必须计入的成本和混淆 |
| --- | --- | --- |
| 原有pool复用、按完成补发 | stock perf当前路径已有；无需研究新接口 | allocation/zero-fill、memseg扩展、map/pin、内存占用/NUMA、完成poll、错误、最终drain |
| 调整队深/qpairs、completion-per-poll/core mask | stock `-q/-P/-C/-c`；`perf.c:1882–1933` | QD改变初始task/pool footprint和排队；减少QD可能牺牲SSD吞吐并延长完整结束；qpairs增加ring/tracker和CPU；poll限额会延后CQ处理 |
| 保持pool不变只限制inflight/提交时刻 | 可实现的普通应用控制，但本perf片段没有现成“固定pool+可变admission”开关 | 需要明确、计费的应用修改/调度器；不能假装是stock已部署，也不能把研究动作独占该能力 |
| pacing/延后补发 | 本perf给定PCIe路径没有已核原生rate/pacing参数；`task_complete`默认立即补发 | 必须声明新增普通软件实现，计timer/poll/CPU/等待/完整finish；不能把减少有效工作量当等价 |
| 预分配、预注册、准备PRP/SGL和安全复用 | allocator/map/已有命令准备是普通能力；本perf初始task分配发生在worker计时/同步之后（L1737–1758） | 若移到任务前面须登记改动与可见ready边界，保留准备时间、内存及pin权限成本；不能按原报表窗口把它抹掉 |
| 使用合法大页/优化pool布局 | CPU hugepage与IOMMU page条件分开验；IOVA模式、buffer alignment、huge memory是已有参数 | hugepage保留/碎片、pin内存、NUMA/分配约束、页表与必要同步；不能扩大DMA权限或放松撤销语义 |
| warmup/提前触碰 | perf有`-a`；内存初始化有memset | warmup是额外工作/时间，IOTLB可能再被驱逐；不能把warmup当免费永久translation准备 |

`perf.c:1285–1297`还明确queue-size可能不足并在driver排队。普通对照必须拥有全部真实可用队列/在途能力，不人为限制为单child。比较QD效果时，若目标是分离“调供给时序”与“减小翻译工作集”，应分别报告stock QD变更和固定pool的普通admission变体，不能混成同映射条件。

本席不重读网络手册，故不新增网络pacing参数断言；沿用既有demand-card的来源及其权限/成本边界。

## 5. 证明物理共享翻译结构至少还需要什么

以下是待收集证据合同，**不是此次已发现的设备事实**：

1. 选定机器/OS/driver/firmware、具体NIC和NVMe BDF/RID；两端实际translated mode、domain/root/页表、IOVA范围与权限，不是仅有vfio-pci绑定。NIC内核DMA与SPDK的VFIO可能是不同软件路径，须各自确认。
2. 把两个Requester落实到实际remapping unit：例如对应平台固件表/设备到IOMMU实例的绑定及driver/domain attach记录。共同group、container或domain都不能单独证明某级IOTLB/PTW的物理共享方式；不同group/domain也不能单独否定共享硬件缓存。
3. 对欲主张的具体资源分别给结构证据：IOTLB的sharing/tagging/partition或replacement；walk cache、PTW并发槽/队列与服务仲裁；invalidation/command queue；如果有ATS/设备ATC，说明其启用状态和相应失效路径。共同remapping unit也可能内部有分区/不同端口。
4. 实际leaf/IOVA覆盖、buffer复用/权限/撤销、提交/成功完成及两流重叠窗口。若做性能因果归因，还需能排除CPU供给、CQ poll、PCIe/DDR带宽、SSD媒体/控制器服务，并以实际translation事件/计数或可定位的结构实验支撑；单个吞吐下降不定位PTW/HOL。

`system_configuration.md:49–87`仅解释group为VFIO权限/隔离单位。DPDK `eal_vfio.c:802–858`解释group/container软件管理；Linux VFIO `1414–1439`甚至显式遍历domain列表，故container名称也不是单一物理translation cache的证明。

## 6. 原部署动作与RISC-V研究投影的边界

原部署可确认的能力是上述stock参数、pool复用、VFIO映射和普通软件可扩展控制。**SPDK未原生暴露本研究P/PN，不构成禁止任何来源绑定模型的理由。** 若研究对象明确为“SPDK式完成驱动pool供给投影到现RISC-V共享IOMMU”，可以把有限task/buffer、IOVA页覆盖、submit/completion/reuse、外部服务与poll延迟显式映射为模型输入，并标注新增控制的位置、能力及费用；P/PN若由研究硬件提供，应写“研究平台动作”，不写成SPDK部署已有功能。

但CPUoff固定gap的两路copy不能自动代表PCIe端到端command完成。数据方向也须明确：NVMe read主要是设备→host payload写入；host内存COPY加入了源读/目标写两侧，本来就多出翻译与带宽。须声明阶段近似与额外流量，或者选择忠实的单侧DMA模型。SQ/CQ/PRP、poll、外部服务和最终排空缺失时，不报端到端收益。

**合理模型空间是条件成立后的动态供给与共享翻译相遇问题**：常驻池在竞争下仍可能被驱逐，真实命令完成会改变下一笔payload的时间；这两点值得结合来源证据建模。是否存在可跨quiet边界预测的状态、何种观测增量和P/PN完整效果由root另行裁决。本席无实际leaf、地址段或服务trace，故不冻结自然正反状态，不给预测收益。若合法大页/复用/普通admission已满足完整目标，应停止被其覆盖的增量主张；若它们不足且残余能定位到共享结构，才有进一步机制研究的明确缺口。

## 7. 最小交接与接受条件

下一步无需广搜或全clone：优先获取一个具体实例的buffer/IOVA/memseg/actual leaf与Requester→共享资源绑定，以及成功完成驱动的有限任务记录；或者root明确接受较弱范围的研究投影并列出假设。部署信息缺失不妨碍假设明确的小模型，但使“真实机器必有此问题”“4KiB是天然场景”“普通方法已失败”三种陈述均无证据。

接受的直接结论：固定SPDK/DPDK映射实现链、常驻复用、leaf选择条件、普通对照成本和所需拓扑证据。未接受且保持null：实际部署/leaf/page footprint、NIC映射生命周期、物理共享IOTLB/PTW、普通对照性能成败、P/PN可部署性与完整差额。没有新实验结果；模型间一致也不替代这些原始事实。
