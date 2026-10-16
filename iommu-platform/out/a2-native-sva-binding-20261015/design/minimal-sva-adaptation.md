# 当前平台最小真实SVA任务适配

2026年10月15日，root提出的可执行重设计。当前没有新启用的SVA任务运行。本方案给出实现接口、修改范围、能力及停止关口；不是已完成的driver或可直接上板的补丁。

## 选择的能力和工作负载

优先建立**单进程、固定pinned用户页、非PASID、设备直接使用同一mm页表**的真实SVA功能链。CPU指针数值直接进入DMA SRC/DST，DC.FSC使用该mm的Sv39根。只允许DID10/11、CONF=0/PV=0、一次session一个mm，不支持换页/迁移/跨进程并发/ATS/IOPF或动态PASID。硬件的plain Sv39、U/R/W/A/D要求见[静态权限绑定](../interface/publisher-owner.md)。这比简单IOVA零拷贝多提供相同地址和同页表，不能冒称标准`iommu_sva_bind_device`已经可用。

普通对照C1是DATE式显式pin+独立IOVA映射；更简单的同VA shadow映射另列C1s。两者都具有真实零拷贝能力，但不共享mm页表。需要分别计页表建立、pin、flush、结果表、同步、CPU/consumer和解除映射。不能因为C2更像“完整SVA”就假定性能更好。

**唯一的最小native功能任务**选用stage117有源码依据的三包、四span交付形状，接真实文件consumer；具体地址和ABI见[软件方案](../platform/software-binding.md)。三包payload为60/568/1011B，第二包由65+503B构成，总payload1639B、输出含三份12B header共1675B。8页arena=32768B，其中4源页、3目的页、1页CPU metadata；DID10/11分别对应lane0/1，localAXIID=0、mux完整ID=0/4；每lane最多一笔normal job，software pending=0。4次descriptor的长度为60/65/503/1011，不调用64B HOLD或4KiB许可。第一功能episode按d0→d1→d2→d3的公开DONE全局串行策略，正常长度和当前24B缓冲保持；双lane并发是后继条件版本，先查正常进展，不为避免阻塞私改banked/512拍。CPU完整读取/校验并写出1675B framed文件，消费收据之后才归还槽，终末再drain/context恢复/unpin/free。该布局是显式新用户地址适配，未复跑stage117或继承其物理地址/周期；未对齐R补齐及实际burst数由真实legalizer重新生成，不沿用旧6burst统计。

这项任务复用当前idma真实copy入口及stage117完整consumer/owner规则，是尚待实现的新Linux功能接入任务，不是论文Snitch AXPY，也不是A的时间正例。将完整frame交给输出接口是任务合同；CPU直接writev原source片段、保留原buffer、普通CPU组包都须获得合法机会，不能为了DMA优势额外要求它们复制或整cache清空。当前平台没有Snitch和TCDM，不能把CPU计算加DDR staging的AXPY冒充论文加速器任务；原buffer直接CPU AXPY可以删除这种staging，因此不把该构造作为A收益正例。

后续性能应用优先使用新取得HeroSDK的真实AXPY/双缓冲来源，先修完整完成与独立consumer。若移植到当前平台，必须提出确实需要DMA搬运的consumer/设备接口；否则选择真实copy服务的完成目标，并允许CPU及原buffer普通方案，不能人为拖延buffer制造需求。

## 最小软件接口和状态

在独立研究ABI原型中新增 `BEGIN, REGISTER, SUBMIT, WAIT, CONSUMED, END`；先放out review，不修改生产IDMA旧ABI或复用旧experiment ioctls伪装现成能力。

- BEGIN：检查CPU/实际profile、同mm、DID独占、无旧receipt；取得每device DMA owner并以升序锁lane，保存默认domain/context。claim前必须先修/核正常domain transition的旧endpoint摘链和rollback；现源码直接`list_add_tail`不能当已验安全路径。
- REGISTER：页对齐、长度有界、普通用户RW页；prefault并按实际PTE核U/A/R/W/D及reserved位；pin真实页并保存全部返回页与引用，partial-pin失败逐项回滚。显式拒绝不支持的格式、hugetlb/PBMT/COW变化和地址溢出，不凭FOLL_WRITE假定D=1，也不凭pin假定CPU aging永不清A。保存每段的原始mm/UVA/page/权限/epoch。
- SVA bind：在RISC-V IOMMU私有研究接口新增borrowed-mm-root模式，使用真实物理PGD根、唯一/稳定PSCID、PDTV=0、SXL=0、S2 Bare。可以由独占unmanaged研究domain承载该私有模式，但map/unmap操作须拒绝borrowed-root，不能伪造generic SVA feature。finalize/atp/free分别识别borrowed root，free不得释放`mm->pgd`；持有mm引用并实现notifier/退出路径。不能调用现有PASID helper后忽略`-ENODEV`。
- SUBMIT：验证本mm、两地址均属REGISTER、权限和generation匹配、lane无未退休receipt；写CONF=0及SRC/DST/LEN，再读NEXT。返回真实functional ID；0/未知提交状态进入drain/隔离处理。无内部翻译完成通知、无软件pending；若后来增加pending，另算容量、锁、receipt映射和槽持有。
- WAIT：只读公开DONE/必要状态，匹配本lane receipt；不以STATUS=0代表绝对quiet。正常成功另要求可用错误证据和实际字节。no-error-handling路径缺少公开错误码时，不能给通用成功API保证；功能任务仅在全部字节/guard和fault快照一致的范围通过，不能掩盖同值错误。
- CONSUMED：先完成平台cache acquire，再读全部目的数据、做真实consumer，输出收据；此后才释放应用slot。超时不构成取消，不能交回仍被硬件引用的页。
- END：禁止新publication，等全部真实DMA receipts及必要source/target R/W/B义务排空；恢复/撤销DC、完成失效，之后才能释放pin/mm/domain/device owner和用户页。错误不能证明排空时quarantine，保留页与context，不使用固定延时后free。

mmu-notifier的`invalidate_range_start`/release必须在CPU撤页前停止publisher并完成安全drain/撤context；pin只阻止物理页回收，不能阻止UVA被munmap/mremap或页表变更。非blockable通知不能等待时返回可重试/拒绝或走已证明的停止协议，不能悄悄成功。固定研究任务禁止fork/exec/同mm其他线程改映射，kernel仍须保证异常close/退出不UAF。方案可先限制单调用持有读侧映射锁的有界同步episode，但不得让fault/drain依赖被自己长期持有的写侧锁。锁顺序、notifier与阻塞等待是实现前独立验收点。

## 缓存与command完成的具体选择

当前DTS没有声明Zicbom/Svpbmt能力；打开内核CONFIG或调用名为coherent的allocator不能证明可见性。DMA sync查默认DMA domain，也不能把C1s/C2的UVA当作`dma_handle`使用。物理页来源、CPU属性、真实cache操作必须逐一绑定。

**优先复用现有CVA6的整cache交接**：Linux `local_flush_icache_all()`发出FENCE.I；`commit_stage.sv:204–212`先等store无pending，`cva6.sv:596,732`接到controller，`controller.sv:100–112`在本平台实际请求I/D失效并等待确认，`wt_dcache_missunit.sv:398–405,477–490`先排空WB/MSHR再完整清空。stage117已经验证同一控制/缓存路径；新Linux ABI仍须核单hart、编译/加载身份、前后ownership和真实执行收据，但不重复旧cache micro。source/page-table release和每个完整包acquire均计全cache及重填、metadata/消费者费用，不能套258常数。这是当前实现的额外行为，不是RISC-V FENCE.I标准对所有平台的Dcache保证。

只有上述Linux到cache绑定失败，才考虑更重的备用bring-up profile：从M-mode最早启动保持`CSR_DCACHE=0x701`为0，新增启动hook/读回并核boot链/WT排空，所有普通对照用同一禁cache配置。该方案代价是整个Linux、页表访问和consumer变慢，不是当前首选，也未实现；它不替代正常缓存应用的性能结果。不在active DMA中途toggle，不编造flush时间。

context失效同样需要真实完成：现有CQH在取指末拍就增加，不能作通用IOFENCE执行确认；PR/PW支持也未完整实现。root已核并接受一个**静态有条件**协议：真实全链quiescent/noATS，独占CQ及错误恢复，连续提交“IODIR.DDT(DV1,DID10)、IODIR.DDT(DV1,DID11)、host VMA(GV/AV/PSCV全0)、AV marker、后继无AV fence”。marker独占8B对齐物理cell，写入zero_extend(新32bit token)；bundle长5且小于ring容量，整段禁止其他poster追加/清错/重启。等全64bit新值可见，并以模ring算术等CQH到后继fetch完成的索引，证明前marker B=OKAY及先前匹配cache valid已同步清除；不证明后继fence自身decode。不能用DV0全局IODIR替代精确范围，PDTC实现未覆盖该全局分支。root直接核CQ/缓存提交，接口席给出[6个反例和精确证据](../interface/publisher-owner.md#7-root新增-quiescent-context-barrier有界反例核验)。

该序列尚未实现/运行，不承担DMA排空或一般IOFENCE PR/PW语义；全链quiescence必须先独立证明，包含旧TL/PTW/result/spill不会再回填。FQ晚到错误另核；CPU stores发布和token读取cache可见性另计。它用已有公开CQ接口，比新增内部翻译完成反馈更小，但不是新的论文贡献或现已可用的完整driver。

## 修改位置和费用

| 批次 | 主要修改位置 | 必须验收的行为和费用 |
| --- | --- | --- |
| P0 完整来源 | 新HeroSDK固定ref中的AXPY main/kernel、map library/driver、runtime manifest | 正确独立reference；最后输出真实wait；每map记录与unmap/unpin/partial rollback；确切kernel/compiler/platform ref。仅修这项不算A新意 |
| P1 功能SVA | `linux/drivers/iommu/riscv/iommu.c`及私有头；`linux/drivers/misc/idma.c`独立session/ABI；新小user task | domain transition、borrowed root/mm、notifier、两DID owner、固定页pin、真实NEXT/DONE、所有错误/drain/恢复；新增每页引用与元数据/两lane收据，不增加数据buffer |
| P2 可见性 | 现有Linux FENCE.I到CVA6整cache路径的新ABI绑定；必要CQ marker helper | 优先复用stage117缓存能力，计WB/MSHR/清空/重填和consumer；仅绑定失败才考虑备用启动禁cache及其更高成本 |
| P3 有用应用 | 有来源的真实consumer/数据位置合同，适配后的publisher | 当前用途/未来用途实际可选；原buffer、CPU、公开完成串行及同信息策略都合法比较；没有新增任务需求就不搭时间后端 |
| P4 最小时间后端 | 精确锁定编译source/profile和服务的独立backend | 有限读写/响应队列、AXI稳定和AW/W/B owner、真实host/供给/cache/compute/consumer成本；未满足不执行时间四案 |

费用账至少包含页/SG引用、mm/notifier、独占锁、domain/context和command marker、每job三次地址/长度写+NEXT及DONE polling、错误/清理、cache profile造成的CPU与PTE开销、consumer和全部slot持有/损失重叠。当前没有这些费用的周期/面积数据，均不填零。

## 实现验收顺序与停止范围

先按P0完成source闭包；P1/P2只做一次单lane成功/guard/释放、双lane完整consumer、partial-pin/map失败和close/drain最小功能验收。mm失效、返回背压和错误不能导致UAF/错归属；不为这些检查恢复旧矩阵或MIG观测。当前board身份未知，部署前需单独核加载与时序；功能模型和FPGA结果分开。

P3若没有真实可避免任务损失或普通方案消除了候选差额，记录该具体版本的结果，保留SVA/合同资产，不靠延迟、压力、phase、扩大buffer/walker或删consumer造正例。只有P3及服务费用闭合，才冻结时间正例、相邻反例、两个时间留出。它们不是原八次source语义的改名。
