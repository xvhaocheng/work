# 10月15日增量审阅：A、局部回收及独立研究方向

## 范围与证据层级

独占完整阅读 reading-chunks.json 的零基下标9—15、35—37、43—44、47、49，共14块、11个来源、74175字符；逐块状态见 daily-a-ledger.json。没有读取其他块。另通过现有dell SSH（BatchMode/StrictHostKeyChecking=yes/UpdateHostKeys=no）全文读取A2实施、A5、独立六族比较的最终验收JSON和六族目录README。没有读原始总线/事件轨迹、构建锁或PDF全文，没有运行测试/模型/编译/板测、没有修改远端。本文实验数字属于留存报告，选定最终验收交叉核对，不等于本轮重验。

模型配置没有切换或修改；本席实际model/effort未通过独立内省确认，不宣称已采用新AGENTS建议。研究产物中历史Sol/ultra配置仅作为其自记执行身份，不代替本席配置证据。

## 重要时间关系：不能继续把A作为默认全局主线

research-reappraisal-20261015.md先按旧计划推进A、B1备选；research-content-map-20261015.md继续A主线且观察到A2工作树比状态页超前。research-novelty-assessment-20261015.md后续接受用户纠正：之前是沿旧A优先在A内部发展，不是平等跨方向比较，撤回“A最有希望”排名。independent-research-directions-20261015.md真正比较六族，临时投入建议为D有限观测指导完整控制选择第一、I失效粒度/活跃DMA/回收共同成本第二、A源合同与位置分析第三，B1/Q/G随后。A4/A5自身也明确不恢复默认排名。

所以“目前全局已确定A最优”“A5意味着重新排第一”均不成立；六族建议也没有自动授权D模型或换一个永久默认。各方向独立论文贡献均未验收。A仍可按用户授权做有界发展，且其资产可辅助D/I合法性分析。

## A的具体演变与实际完成程度

### 重评pilot与A1控制位置方法

重评把标准DAG/max-plus/carry-in作为工具，目标是从实际接口、first-offer/accept、owner和后继请求生成抽取合法控制，再预测双方/consumer/释放。私有RTL＋行为内存完整2×2copy pilot一次构建、RR和current-VALID DS-first/F4各一次：V=7009、A=6497、quiet=7019条件周期完全相同；28次offer没有双头竞争或held-late-DS。行为FIFO有487/940拍前序payload债务，但该选择点无新机会；不是MIG债务/收益正例，不继续调相位压力。

control-position-method报告固定旧HERO mm-large.c，6个2048B槽、8笔搬运、4个8192MAC计算，提出发布前选择与已发布前缀边界。四案×两个程序共8次完整源语义episode，1412事件、完整字节与1024元素consumer通过；B保留结构案删去两个重复job。这个无时序完成序不是CPU/缓存/AXI/RTL/MIG模型，全部时延UNKNOWN，H1/H2是结构留出而非时间或独立应用留出。检查器做结构/ready/owner/consumer，通用协议/缓存/有限资源时序后端未实现。

### A2来源绑定：弥补旧关闭SVM来源缺口，不等于复现论文

找到DATE关联hero-tools 47339e0→SDK fb56489→LLVM d8cffb8，Linux作者e4b0725/6.9公开enabled实现谱系。AXPY do_map→pin_user_pages/独立iommu_map→IOVA标量offload→Snitch publisher→DMA→DONE→host；IOVA为0x80000000+(UVA mod 0x40000000)，不是同UVA/mm->pgd。普通config仍6.1.22、IOMMU分支可变，无论文实际命令/commit绑定，所以不接受准确论文实验快照已复现。

三个源码缺项实际影响完整任务：末输出无真实DMA wait前置DONE保证；do_map路径y_dev_virt=y_host使恢复自拷/原地reference比较同数组；map记录list注释/release空而应用free，缺逐buffer unmap/失效/unpin。仅针对取得版本，不否定论文其他版本。

当前平台默认源码mode0/四DMA各24B/单TC，与旧两4KiB banked夹具不同，当前加载bit未证。每lane normal仅一个未退job，FIFO8不等八个独立任务；2048B无fullbuffer许可，64B HOLD/4KiB batch不泛化。受限non-PASID pinned-mm SVA是新适配，不能称通用Linux PASID/IOPF。

### A2软件实施：确有构建和代码，native尚未执行

当前Linux6.5.0-rc2 dirty基线已实现驱动、私有ABI1、任务程序/rootfs，生成Image等产物；patch针对as-found脏树，不是clean upstream HEAD。C1普通IOVA、C1s同VA shadow、C2借真实pgd；同8页/四span/1639B payload、1675B文件输出。C2额外付mm引用、全任务mmap锁、mlock/munlock、失去用户态prefix重叠，普通独立表不被强加CPU PTE A/D约束或mlock。新IO表实际页数未知，7数据页不等7页表。

最终层级：用户方法16、真实driver C边界20、析构7案47项（原版9失败）、CQ生产本体4案、Hero consumer9和driver C边界12；各层不相加为native通过数。Image成功不是内核已启动，host memcpy替身不是DMA，CQ本体RTL不是整平台失效消费者/CPU缓存/排空验证。最终验收明确新内核启动0、native DMA0、整平台DMA RTL0、FPGA构建/板测0。

实际修正包括：FOLL_PIN+mmap锁不防rmap清A，C2需锁8页及严格禁用aging profile；普通shadow不受此限制；io_pgtable L0与entry0空导致的页表释放泄漏；旧LLVM relocation61/标准57不兼容，首个make成功ELF仍被否决，经限定EH-only副本转换生成静态接受ELF。ASan handler异常原件保留，未定位根因，不称所有sanitizer无异常。

Hero-specific quiesce/失效完成/cache三hook仍缺代码，默认pin前EOPNOTSUPP。不能写“Hero已实现只待上板”，当前DID10/11 PV0 CQ合同不能直接移给Hero DID0/PV1/PID0。完整时间四案仍未冻结；原buffer直接writev已满足该文件功能，所以停止该任务独有价值主张，保留软件功能基线。

### A3→A4→A5：由粗wait-all到具体普通完成ID与完成域

A3沿真实AXPY：64tiles、四2048B槽/core8 DMA publisher/核0—7 L1 FMA。runtime本有transaction ID wait，应用丢弃ID用wait-all；最后tile可只等最后两个输入而保留尾部排空，中间tile需Y复用guard。执行8项符号检查；同信息标准依赖模型结果相同，无周期/native/实际AXPY episode。

A4补固定FIFO→reshaper→AW→最后B→累计completed-ID条件链，O-T1收紧到只保存后发RY一个32位ID，前缀覆盖RX；一般O-G双ID加Y复用guard。完成不检查R/B错误，不等host可见或可free；runtime cfg bit2被RTL截断，实际serialize=0，B有序不能当job全串行。变体未应用/编译/运行。

A5新核地址腿：输入host AR/R过外IOMMU，L1 AW/W/B本地；输出L1 AR/R本地、host AW/W/B过外IOMMU，同SID/SSID。远端R结束不等本地输入已完成。固定同descriptor前缀：RX63=b+188、RY63=b+189、WY62=b+190已接受。正切面completed=b+190可放行K63但WY62仍live；反切面completed=b+189时RY远端R已完但本地写/B未完，必须继续等。两切面只在手工DAG上为合法下闭集，不证默认RTL容量/时序可达。

A5一次离线依赖检查覆盖cold/hot两offload共384job（不是新应用运行），同信息标准DAG得到相同判断；完整时间UNKNOWN，四时间案例未冻结。Snitch固定单DMA/Carfield仅FPCluster过IOMMU，真实第二publisher/共享竞争未证；CPU FMA仍合法，源无CPU预算排除。停止仅“该动作必须新增早接口”版本，保留A完成域/等待范围证书思想，非新求解器或论文主贡献验收。

## LR1局部读归还：有值原型，具体规则被普通覆盖

LR1两4096B copy、原每源512beatbuffer，source全R成功后允许归还source PA，目的从保留oldbytes继续W/B。seal从frontend/JobFIFO接受parent算，未VALID第二burst仍属旧授权必须drain；需完整PA alias/readers/context/epoch及旧walk/OUT/heldAR/acceptedR关账，再PTE/cache/最终失效/排序/ACK。Rdone→Wpending有源码偏序，但ACK完成时是否仍有窗口未知。

私有Python功能原型用显式抽象cache/PTW/invalid/order primitive，8组122断言、完整值/旧新代/错误隔离通过；独立73,728snapshot/221,184calls是策略检查，不是native/时序。ordinary phased/targeted同选定付费接口执行相同完整链，无独有动作或已证明状态费用优势；0性能条件/native代码/FPGA。Intel page-selective drain有明确前史，但RC drained不同DMA全页receipt，不能混。

特别保留报告顶部勘误：同选定executor/primitive的等价仅关闭该版规则及已证明状态节省，不能证明完整协议已发表、所有native adapter等成本或整个思想族无价值。

## 六族独立比较与新意边界

D拟从O0（含可得复用/扇出/突发与噪声）相近而P→PN完整净后果相反的同合同状态对，研究额外跨owner PTW/payload联合时序信息能否及时、低成本改进未见任务决策。旧PN实板负例与另一M0合同不能拼成状态对。CounterPoint/HELM/ObservUVM/MISE/ASM/Coz已有观测/干预/选择；DOA2026已非常接近“相似miss、结构不同、保护收益不同/组合退化”。因此D1状态对、D2信号和费用、D3冻结留出决策收益、D4完整闭环都尚待证，不能宣称新思想已成立。

I有SMMUv3 RIL讨论的真实命令效率/live DMA权衡，但未证期限失败或补丁最终状态；需真实map/unmap→gather→command→完成回收与live/domain预算轨迹。IDYLL/Scalably/rIOMMU为强对照，现RISC-V驱动PSCID/ADDR narrowing注释须先做正确精确基线，修缺陷不是新机制。

B1未来地址早于payload槽可复用的窗口有来源，但ICCD2018预取/ATS已有，当前debug纯翻译前门存在且应作计费强对照；无ordinary prewarm后的真实完整残余。Q需具体超出MASK/DWS/普通分区/限流/DS/SJF；G保留真实gather/访问更新/多GPU共享问题，缺任务和部分原文不作终局否定。

发表方法、同信息同预算普通实现、同合同标准分析应分三组比较。RTAS2026已有FIFO外调度/合法chunk/完整响应界；TC2023允许job内任意到达但有读写独立/pipeline不满前提。普通ID、事件图、最长路、owner细节正确不自动构成独立贡献；A需可复用抽取在未见真实任务给出有用部署判断及完整结果，不能只手工给一个实例补边。

## 本次应修正的历史导航/会议稿

1. “A优先”只对重评/内容地图当时快照，后经用户纠正撤回；六族比较是D/I/A临时建议，A5没有恢复全局排名。
2. 内容地图记录A2“无最终验收、13用户/17driver”是查档时点；最终实施报告及实读JSON为16/20、软件接受native pending，保留旧版本不相加。
3. 旧A1只取得关闭SVM SDK缺口已由A2 DATE enabled谱系补齐；准确论文实验commit/config仍未闭合。
4. A2不是“适配完全没做”，也不是“新内核已运行/SVA性能通过”；Hero gate仍实质未实现。
5. A3两个输入ID已经A4/A5收紧为一个后发RY；不能继续说物理ID语义完全未核，也不能扩大到成功/host可见/free保证。
6. 两offload384job、四案八次source semantics、离线DAG切面、CQ4例、host tests、RTL pilot是不同层级，任何一项都不是完整时间四案通过。
7. 普通覆盖严格限定版本/付费接口/执行器；不等发表覆盖或架构族无价值。MIG工程暂停非科学阴性：build6 WNS−0.391ns/598 setup失败，0新采集、真实债务unknown。

## 准确绝对路径与本轮核查范围

已直接全文读的最终验收：

- `/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/root-result-acceptance.json`
- `/home/xvhaocheng/work/iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json`
- `/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/root/root-result-acceptance.json`
- 导航：`/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/README.md`

六族验收不在目录根；实际路径多一层`root/`，经存在性和README核正。

以下已从全文报告定位，未在本单元打开原始out文件（不得计raw已核）：

- A1合同 `/home/xvhaocheng/work/iommu-platform/out/control-position-method-20261015/cases/frozen-contract.json`；运行 `validation/semantic-runs/summary.json`、验收 `validation/source-event-verification.json`（同目录）。
- A2构建 `/home/xvhaocheng/work/iommu-platform/out/a2-sva-implementation-20261015/kernel/build-lock.json`；最终测试 `task/locked-pte-batch/app/results.json`、`task/locked-pte-batch/lifecycle/results.json`、`model/cq-check-results.json`（同目录）。
- A3 `/home/xvhaocheng/work/iommu-platform/out/a3-consumer-position-rules-20261015/validation/rule-probes.json`；A4 `/home/xvhaocheng/work/iommu-platform/out/a4-axpy-control-decision-20261015/source/receipt.md`。
- A5合同 `/home/xvhaocheng/work/iommu-platform/out/a5-wait-scope-contract-20261015/case-contract.json`；运行派生 `/home/xvhaocheng/work/iommu-platform/out/a5-wait-scope-contract-20261015/validation/dependency-results.json`。
- LR1合同 `/home/xvhaocheng/work/iommu-platform/out/local-read-reclaim-20261015/root/mechanism-card.md`；功能 `/home/xvhaocheng/work/iommu-platform/out/local-read-reclaim-20261015/prototype/functional-results.json`；范围勘误 `/home/xvhaocheng/work/iommu-platform/out/shared-service-control-boundary-20261015/errata/corrections.md`。
- 重评pilot `/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/a-pilot-results.md`；时间预测 `/home/xvhaocheng/work/iommu-platform/out/research-reappraisal-20261015/analysis/prospective-validation.json`。

本单元未选择新方法、未执行D/I建议、未改远端或恢复MIG；由父任务统一维护research-history。
