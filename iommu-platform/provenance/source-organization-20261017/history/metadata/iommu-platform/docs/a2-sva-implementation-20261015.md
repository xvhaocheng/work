# A2真实SVA适配：代码、构建与功能边界

2026年10月15日。当前平台的最小适配已落到Linux驱动、私有ABI、任务程序和rootfs，并成功生成内核Image。接受的是代码实现、软件构建和分层功能检查；尚未运行新内核或真实DMA，不能宣称完整native任务、缓存交接或性能已经通过。原MIG暂停，未启动FPGA构建或板测。

本轮保留A“控制位置与完整任务预测”。最小文件任务允许原buffer直接writev，普通方案已具备相同交付能力，因此不把该功能本身作为A的独有价值，也不为它寻找压力或相位胜例。下一版需绑定确实要求DMA结果位置或前缀消费的真实合同。

## 1. 补丁与版本

- [Linux增量补丁](../out/a2-sva-implementation-20261015/patches/linux-a2.patch)：新任务驱动、domain/context生命周期、借用mm根页表、CQ完成证据及稀疏页表释放修补。
- [平台增量补丁](../out/a2-sva-implementation-20261015/patches/platform-a2.patch)：任务程序、1675B固定输入、主机检查、构建及rootfs接入。
- [构建锁](../out/a2-sva-implementation-20261015/kernel/build-lock.json)：Linux `c0f5e13ea81fa30d78f3bb1d6389e2397321e040`，6.5.0-rc2 dirty；GCC 11.4.0；完整.config、全部已跟踪差异与未跟踪源码副本、最终输出SHA。该版本是当前平台，未冒充DATE的Linux6.9。
- [逐文件前后SHA](../out/a2-sva-implementation-20261015/patches/source-manifest.json)区分原有脏改动与本轮增量；两份补丁均通过只读反向适用检查。应用基线是本轮as-found状态，不能直接当成干净上游HEAD补丁。
- 最终Image、vmlinux、rootfs、任务二进制及DTB已另存[kernel/artifacts](../out/a2-sva-implementation-20261015/kernel/artifacts)。保留先前成功构建和最终公平性修订后的独立记录；早期哈希在pre-final，不冒充最终输出。

当前.config保持SMP/NR_CPUS64及IOMMU_SVA/RISCV_IOMMU，运行门限要求实际只有一颗online hart；不因通用SVA配置开启就声称实现了通用PASID SVA。源码默认mode0、四DMA各24B、单TC；本轮未改生产RTL或服务参数，未证当前已加载硬件身份。

## 2. 当前平台实际实现

入口是[任务驱动](../linux/drivers/misc/idma_sva_task.h)、[ABI](../linux/include/uapi/linux/idma_sva.h)与[任务程序](../bench/idma_sva_task.c)。ABI1提供96B CAPS、232B同步RUN，私有ioctl为`'i'/0x12、0x13`。运行须显式接受平台前提后启用`iommu.research=1`、`idma.sva_enable=1`；二者默认关闭。编译通过不是启用依据。

限定CAP_SYS_RAWIO、单进程/单线程/独占mm、普通Sv39、4KiB私有匿名RW页、8个固定且互异的长期pinned页、DID10/11、PV0/noPASID/noATS。拒绝huge/special/shared和borrowed模式不支持的PTE；不修改CPU的A/D位，也不提供缺页恢复、IOPF或任意异步共享页表。

最终审查补上CPU PTE老化条件：`FOLL_PIN+mmap_read_lock`仍不能阻止普通rmap清A，而当前PTW对A=0直接fault。C2必须先对同8页mlock，再初始化写触碰建立A/D，并保留到安全返回；driver要求VM_LOCKED，拒绝启用LRU_GEN、IDLE_PAGE_TRACKING、THP、KSM、NUMA_BALANCING、DAMON的构建。当前.config均未开启；普通rmap在锁定base-page处提前返回。外部/proc/PID/clear_refs、ptrace等mm修改同样不属于允许profile。这不是“mmap锁冻结所有PTE”的通用保证。

三个DMA模式使用相同任务、8页arena、四span、NEXT/DONE规则和两lane：

| 模式 | 地址与页表 | 付出的资源 |
| --- | --- | --- |
| C1 普通IOVA | 从0x10000000开始的独立I/O页表 | 同8pin，映射7个数据页，独立页表/域及全部交接成本 |
| C1s 同址shadow | IOVA数值等于用户VA，仍是独立I/O页表 | 同上，不因CPU PTE.A清零就被削弱 |
| C2 pinned-mm | DC.FSC借真实进程pgd，设备直接使用用户VA | 同8pin，额外mlock/munlock、mm引用、持续mmap保护和域状态；新增I/O页表页为0，原CPU页表并非免费 |

普通独立页表实际分配页数尚未实测，返回U32_MAX表示未知，不能把7个数据页映射写成7张页表。CQ有每IOMMU一张独立4KiB协议marker页，未计入payload arena；三种DMA模式共同支付，非额外数据buffer。任务程序只保留输入文件和8页arena，逐字节推导oracle，不另分配完整expected arena。普通方案不被迫mlock；C2特有的页表稳定成本单独计入，mlock失败不回退或跳过检查。

### 发布、完成与释放

成功顺序是：输入恢复/初始化→pin及map/借根→取得两个设备owner→attach两个context→完成失效/缓存交接→串行四次配置及NEXT→各自真实DONE→CPU缓存获取及payload比较→恢复原domain→再次完成失效→unmap/释放域→最后缓存交接→dirty-unpin/mm解锁→RUN返回SAFE→独立全arena校验→三包文件writev/close→munmap。

源码调用、首次offer、接受、返回数据、最后W/B、DONE和CPU可消费分别记账。RUN不使用内部翻译完成反馈，不把FIFO8变成8个normal job，不增加2048B或64B许可，也不提前覆盖live寄存器。本轮只使用公开NEXT/DONE串行，尚未实现前缀并行消费或DS-first控制。

四span合同固定为`(src,dst,bytes,lane)`：`(32,16396,60,0)`、`(4128,20492,65,1)`、`(8224,20557,503,1)`、`(12320,24588,1011,0)`。三包payload为60/568/1011B，目的记录头12B；第8页只作守卫。权威输入沿用[native-task-input.json](../out/a2-native-sva-binding-20261015/design/native-task-input.json)，不采用旧文字中曾出现的另一套pre-guard偏移。

数据正确有两层：内核在完成后比较四段source/destination；用户态从不可变输入独立检查整个32768B arena（数据、头部及未写区域）后交给文件consumer。consumer合同为writev完整返回1675B及close成功，不声称持久化存储fsync完成。重复运行重新读输入并恢复arena，输出路径/inode不能与输入或先前输出别名。

### mm、异常与上下文

采用已审定同步版本：从pin到所有DMA排空、domain恢复、失效完成和unpin持续持有mmap_read_lock及mm引用。进程退出只能在RUN安全返回后进行；不把缺少MMU notifier的异步版本冒充已实现。本实现付出整个任务阻塞及损失用户态消费重叠的代价。

超时只停止后续发布，当前job仍须等真实DONE后才允许清理；NEXT回执不确定、失效失败、默认域恢复失败、short-unmap或域释放未认证时保留pins/mm/锁/owner并阻塞，不因超时free。没有硬件abort，因此故障返回时间无上界，需要平台恢复。动态unbind/hotplug不支持；remove中的局部early-return不能阻止driver core的devm释放，不能据此宣称安全热拔插。

IOMMU修改在一次domain转移中串行化old/new/endpoint锁，成功发出失效命令后才更改链表，失败恢复DC；BLOCKED使DC无效。借用pgd不进入普通io-pgtable释放。私有PSCID采用上半区并在有恢复证据后回收，真实PASID限制为下半区，因此公开max_pasids上限减至2^19；GSCID独立16位。普通曾attach但没有恢复证据的ID仍保留lease，未宣称修好所有通用域泄漏或原驱动的全部SVA限制。

页表释放修补同时处理两处实际泄漏：L0表不能被递归基例跳过；已分配的子表不能因第0项无效而漏free。仅释放页表页，不释放映射的数据页；根页由domain单独释放一次。

## 3. CQ与缓存：实现了什么、证明到哪里

[公开research helper](../linux/include/linux/riscv_iommu_research.h)要求调用者先证明所有DMA/TL/PTW/result/spill及R/W/B已quiet、无ATS/S2、没有其他publisher。helper本身不建立该前提，也不提供PR/PW drain。

实现精确五命令：DID10/11的IODIR.DDT DV1、host VMA000、带fresh token的AV marker、后继noAV sentinel。证据同时要求完整64位marker匹配、modulo CQH到后继fetch终点、CQ状态/基址/tail/生产者记录及错误generation一致；排他保护涵盖所有CQ发布与IRQ清错。发布后失败会poison队列并保留marker，不重试、重置或清错掩盖失败。

缓存回调实际调用当前Linux的local_flush_icache_all，经现CVA6 FENCE.I整I/D路径。只复用stage117已核的硬件能力，没有重跑旧cache实验或搬用258周期常数；新Linux执行、全缓存可见性与费用仍需对应硬件验证。每次查询marker的同步回调也属于完整费用。

新增CQ本体RTL功能检查使用生产rv_iommu_cq_handler原件和测试边界的AXI responder/CSR flops，源码/生产参数不变。正常、环形回绕、marker B SLVERR、仅head的合法无AV变体共4项通过。正常和回绕均看到token先于B可见但head尚未越过marker；直到B成功并读取后继末拍才接受联合证据。仅head变体明确缺少exact-bundle前提，只反证单看head不充分。

这不是完整IOMMU/DMA仿真，未验证失效脉冲之后的缓存消费者、CPU缓存、原生Linux或全平台排空；迹中的索引只说明功能顺序，不用于时间预测或AXI/MIG债务。

## 4. 实际检查与失败记录

| 层级 | 实际结果 | 不包含 |
| --- | --- | --- |
| 用户任务主机检查 | 最终16个方法通过（含原13回归及锁页/错误检查）；首批5模式各2次的10份1675B输出及SHA保留；主机与静态RV64 Werror编译通过 | 三种DMA模式在此使用显式host-model memcpy，非DMA/SVA |
| 实际任务驱动C控制流 | 20案通过：3模式正常、partial-pin/map、claim/attach、两次barrier、NEXT0、deadline后真实DONE、restore/unmap/release、A清零及无mlock时普通成功/borrowed拒绝 | Linux/MMIO/IOMMU为边界替身，retain由scheduler处longjmp观测，非恢复实现 |
| 实际io_pgtable源码析构 | 7案47项ASan/UBSan检查通过；原版同检查9项失败 | 原生Linux页分配与并发 |
| 当前Linux | 分批3对象、vmlinux、rootfs、Image及DTB均成功；最终公平性修订后再次编译idma.o/Image成功 | 内核启动、真实DMA、FPGA、板测 |
| CQ生产本体RTL | 新4案通过 | DMA、cache消费者、整平台、时间性能 |
| Hero关联软件 | Linux6.9驱动对象及固定Hero LLVM完整AXPY offload编译链接通过；CPU consumer9案、真实driver C边界12案通过 | 论文准确实验配置、平台完成gate及native运行 |

原始入口：[最终用户检查](../out/a2-sva-implementation-20261015/task/locked-pte-batch/app/results.json)、[最终驱动检查](../out/a2-sva-implementation-20261015/task/locked-pte-batch/lifecycle/results.json)、[析构原版对照](../out/a2-sva-implementation-20261015/iommu/host-test/result.json)、[全部当前构建](../out/a2-sva-implementation-20261015/kernel/build-lock.json)、[CQ迹与检查](../out/a2-sva-implementation-20261015/model/cq-check-results.json)。各层计数不相加成一个native通过数。

驱动首14案及旧SHA完整封存；随后发现普通IOVA/shadow不应受CPU PTE.A/D约束，只改borrowed检查范围，追加3项并重编新驱动；再因CPU PTE老化问题补mlock合同并针对变化检查。用户任务仅在其源码因mlock改变后重测，旧13项/10文件和驱动17项均封存；CQ和析构源码未变，没有重跑。CQ首次响应器没有覆盖AW/W stall，保留该记录后只修测试边界，未改生产RTL或扫参数寻性能收益。Hero首次编译/工具/sysroot问题及一轮ASan运行时异常均保留在hero/logs与evidence，不把异常替换成“所有检查一次通过”。

Hero一轮ASan执行出现反复DEADLYSIGNAL输出，已停止并保留完整原日志；同一仍带ASan插桩的二进制使用handle_segv=0，以及独立UBSan执行均通过。没有证明该ASan运行时异常的根因已解决，不能报告为所有sanitizer无异常。

## 5. HeroSDK补丁的独立范围

固定hero-tools `47339e040c43ab4d44d9a870bc307251084a1ba4`、SDK `fb56489c97b41471b6b9d707f0a38b3bc789a76e`、LLVM `d8cffb8d7294483b45732283719e250116fbe6c0`和作者Linux6.9 `e4b0725a5089f6cc0fc5afec06f3091d98dffa68`。完整来源不重搜；仅构建本任务需要的实际单位及AXPY链。

补丁增加最后DMA wait/fence及barrier、独立reference和每轮恢复、per-open owner及逐buffer页/映射记录、partial-pin/map回滚、unmap/失效完成后dirty-unpin/free、close不确定时保留。以真实固定运行时头编译设备主体，修正原inline asm约束并保留FP32融合运算语义。

精确旧LLVM使用`R_RISCV_32_PCREL=61`，与当前GNU标准编号57不兼容；root在“make退出0”的首个ELF中发现.rela.eh_frame仍报unsupported，否决该产物。直接使用同版本lld又遇到Ubuntu CRT的R_RISCV_ALIGN不支持，失败原件均保留。最终使用公开的有界host链接适配器：只把固定d8 RV64对象副本中.eh_frame的两个RELA类型61改为等义57，保留原对象、全部EH数据/offset/addend/symbol及逐字节证据；非EH出现61直接拒绝。最终重链无unsupported，动态重定位仅COPY/JUMP_SLOT，8个FDE均在执行段，固定LLVM unwind解析无错误，且移除了错误的构建机RUNPATH。 此构建明确使用Ubuntu GCC11/glibc2.35交叉sysroot，不冒充原Buildroot运行环境；板端依赖及运行身份仍未验。

**Hero平台quiesce/失效完成/cache gate的具体实现仍缺。** 默认gate在pin之前返回EOPNOTSUPP，host替身只验证生命周期，不使真实映射链自动可运行。该缺口需要Hero硬件版本专用后端；当前平台的DID10/11、PV0 CQ合同不能直接移植为Hero DID0/PV1/PID0协议。因此不能把Hero一栏写成“只剩上板验收”，也不能把本轮软件组合等同论文确切实验构建。

## 6. 方法增量与下一决定

本轮把A2输入具体化为可执行的profile/地址/context/owner/receipt/生命周期合同，尤其区分：

1. NEXT调用与接受、DONE与CPU可消费、发出失效与失效完成、pin持有与CPU页表稳定。
2. 普通独立页表能容忍CPU页表变化，而borrowed-root额外要求mm稳定；同信息对照不应被人为加上后者的限制。
3. 更早发布数据或提前释放slot不能自动改善完整consumer完成；同步安全实现还会损失用户态消费重叠。

这些是方法实现与可验证边界，尚非独立算法贡献。沿用[七篇前作矩阵与AXI核查](control-position-method-20261015.md)：MASK已协同翻译/数据服务，DWS已有walker交错/公平控制，近邻AXI分析已处理有界服务；不重新把这些能力、标准DAG或共享页表本身主张为新意。A仍需在真实可选动作与相同资源普通对照下，预测未参与设计的完整任务，并证明控制位置带来的结果或部署成本增量。

**本版决定：保留代码为功能基线；停止“三包文件输出本身证明A独有收益”的主张，时间四案仍未准入。** 原buffer直接writev已合法完成相同1675B输出；主机结果不推导原生性能优劣。当前同步C2既无并发fresh-head选择，也不支持stage117式用户态prefix/peer并行链，不能强造时间正例。

最强合理后继是“真实consumer合同下的前缀交付与最小控制位置”：先证实真实任务要求设备位置、设备consumer或CPU预算；再给C1/C1s当前用途优先、公开DONE节流/串行、原buffer保留及适用DS-first完整机会。如果确有prefix消费需求，具体比较独立I/O页表+固定pin的异步prefix方案、当前同步borrowed方案，以及在mmap保护内付费执行的内核consumer。异步borrowed版必须另有可证明的停止/排空与mm失效协议；没有硬件abort时不能只加notifier就放行。

后继只需补决定性任务事实和该接口所需最小实现，资源仍为同8页/原lane，不增加许可或人为延迟。满足实际启用链、缓存/失效/异常验收及普通方案确有不足后，才冻结一正一邻反两留出、完整输入/服务界/预期区间与失败规则。完整时间从共同任务就绪到双方、必要consumer及资源释放，含准备、供给、锁/slot持有、cache/CQ、排空及损失重叠；没有测得的参数保持UNKNOWN。

## 7. 尚需的验收合同

1. 软件运行前核对应板/bit源码身份、mode0/24B/单TC、单hart WT/cache、noATS/no外部publisher、所有源quiet。该步骤尚未做；新opt-in维持关闭。
2. 未来经用户安排的对应硬件验收，先运行C1、C1s、C2各两轮三包任务；核完整1675B文件与全arena、四NEXT/DONE、R/W/B退休、cache可见性、context恢复/marker、pin/mm/ID归还。覆盖实际map失败回滚、进程退出与异常保留；永久等待不当成成功恢复。
3. 若全链通过仍无普通方案之外的有用选择，止于功能基线，按上节真实consumer问题重设计，不自动扩实验矩阵。Hero专用gate是额外实施缺口，须先写代码并验证再谈其native链。
4. 本轮真实DMA/新内核运行/FPGA构建/板端次数均0，完整时间/收益/Fmax均null。原MIG goal1792023314继续paused，build6 WNS−0.391ns/598setup失败、0新加载采集与未知债务不变；旧实验及四案八次源语义不重跑。

Codex-only；root承担实质机制、当前任务驱动、关键源审查、执行和最终裁决，Sol独占席提供IOMMU补丁、任务程序/分层夹具及Hero适配。未调用DeepSeek或Claude。两周10月15—28日及历史用时不重置，论文未完成。
