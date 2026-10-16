# §47 Codex独立推演：两条共享trace、合并扣费与job未来用途边界

本席亲自设计trace、推演必要信息与强普通对照，并定向核TLfast/single PTW、TC FIFO及private prepared-parent关键源。没有调用DeepSeek/Claude，没有运行trace、编译、改production，也没有重跑§45。root的DeepSeek证据稿仅用于定位MMU-aware DMA原文，本席另亲读原论文相关段。导航：§1真实接口；§2/§3两条可直接转bench的小场景；§4本席提出并淘汰的合并规则；§5root job-hint关键合同；§6结论。

## 1. 当前真实接口不能被替换成一个假定的并发coalescer

直接核：`rv_iommu_tl_wrap.sv:170–215` main request spill保留一份请求至reply；`:375,759–805`只有一个main PTW，以该main原请求/context驱动。`:303–308` fast window限定S1-only、另一TC route bank等条件。`rv_iommu_nonblock_hit.sv:85–107,142–172`用实际请求DID的DC/PSCID查4KiB IOTLB，接受成功命中；miss→REJECT，等待normal_fire，不将miss加入active walk或MSHR。

因此现在可执行的能力是“先fill、跨DID后来命中”，以及有条件的hit-under-main-walk。本文并发PTE合并场景是**允许设计的未来扩展**，需加admitted private Info/MSHR等真实容量；candidate和ordinary都拥有并计同样这些资源。不能把这份新coalescer的组织缺陷称为当前single walker旧瓶颈。

bank不是DID也不是direction。`tests/nonblock_completion_banked_ctl.sv:17–20,254–255,378–418`按原AXI ID bit选bank、拷贝请求后写bank、按bank回response。§39 actualtop使用`BankIdBit(SameBankDmasEnable ? 3 : 2)`；same-bank模式下A/V的R/W都可以进入bank0。fast需要不同route bank的例子要显式换路由，不冒充§39两个DMA同bank。

## 2. 小场景一：同context两copy的共享source及partial prefix

这是**TL request trace**，不是完整DMA payload仿真。给两份32KiB copy的合法地址区域，并先取p=0的四请求作最小实例。DID10=V、DID11=A；两DC共同PSCID=0x77、同Sv39 iosatp root、G-stage Bare，pid_valid=0、priv=0、debug=0、MSI模式0，权限正常。两者共享source读数据、destination互不重叠：

| job | source | destination | length |
|---|---:|---:|---:|
| V | 0x00000000 | 0x00010000 | 0x8000 |
| A | 0x00000000 | 0x00020000 | 0x8000 |

可给TL memory dictionary：root R=0x90000000，R[0]指向Q=0x90001000，Q[0]指向leaf table L=0x90002000。L的VPN0..7映射到0x80000000+p×4096，VPN16..23到0x80010000+p×4096，VPN32..39到0x80020000+p×4096。所有leaf v/r/w/u/a/d=1、x/g=0；nonleaf设置合法表指针。以上只是给bench可初始化的字典，不声称当前Linux已经设置该页表。

DDTC通过真实配置/warmup保持两DC有效，IOTLB初始冷；统计单独记PTE，DDT配置读取另计。若需要触达现有fast，V route bank0、A route bank1，按以下**握手事件**驱动，不用臆造固定cycle：VALID在READY前一直保持。

| 步骤 | 发送请求及触发条件 | 正确可观察结果 |
|---|---|---|
| 1 | V R(src0=0)，等正常reply | 读R[0]、Q[0]、L[0]；source0入IOTLB |
| 2 | V W(dst0=0x10000)，进入main PTW | 请求相同R/Q prefix，leaf=L[16] |
| 3 | V main walk active且fast窗口合法时，A R(src0=0) | 同PSCID/root缓存命中；实际A权限独立检查，0笔新增PTE |
| 4 | 等normal通路可accept，A W(dst0=0x20000) | prefix相同，独立leaf=L[32] |

p=0共三个不同leaf、两个共同prefix；只用现有完整IOTLB而没有PWC且每冷leaf正常三层walk时是9笔PTE。一个普通2-record partial/PWC能将唯一PTE footprint降到5，不能把candidate coalescer对9的差异直接叫独有收益。p=0..7的成对循环有24个不同leaf、两个共同prefix，理想footprint为26；这是静态请求集合的footprint，不是已运行当前copy的TL计数。

还可作真正合并测试：冷cache同时注册V R(page0)和A R(page1)，两个请求共享R[0]/Q[0]，不同leaf L[0]/L[1]。未来coalescer可读共同prefix各一次并保留各自IOVA/permission/return Info；普通PWC或共享fetch+private Info同样只需四个唯一PTE。若同时请求同page0，可以合并整个成功result，仍保留两个返回身份/权限检查。比较须固定C/private Info、缓存key/版本、记录数、lookup/response端口与同样的提前准备机会。

这条trace针对V/A整体性能有实际意义：A的source翻译复用V结果，两个destination的独立翻译仍争walker；合并不能免除下游copy数据流量。单凭PTE费用分配也不能保证shared DS/DDR的数据带宽隔离。

## 3. 小场景二：持续合并到达不能靠“最后一个waiter消失”保证公平

未来扩展固定两entry：shared ready result S、独立V result Z；每entry最多K=2个waiters，response端口每次一次握手。所有memory读取都正常有限响应，所有endpoint ready，没有永关budget，也没有制造不公平DDR。A持续发同key S，V有一个已接受、已经获得Z结果的有限请求。

可执行到达序列：

| 事件 | 动作 |
|---|---|
| e0 | A0/A1同key S进入cohort C0；S的PTE first-offer，正常返回 |
| e1 | V0独立key Z进入另一entry，正常PTE返回，V0等待response |
| e2 | 回复A0；同一次服务机会后A2到达，加入仍未空的C0 |
| e3 | 回复A1；A3到达补进C0 |
| e(n≥4) | 回复当前最旧A；下一个A马上加入C0 |

坏规则“当前entry drain到refcount=0再换entry”永远见不到C0空，V0不结束。capK仍允许旧移除、新加入，所以K本身不是持续公平证明。这里共享slot/response组织是未来设计的选择，不能称当前生产故障。

最强普通能在相同两entry/K/Info预算击败它：S返回时seal C0（也可first-offer时seal，late join走下一cohort），现有A0/A1照常处理；后续A请求未获credit就READY=0，必须在上游保持VALID，或进入已计费下一cohort/cache-hit队列。共享PTE缓存S仍可复用，不强制重读。对ready response按实际DID/class RR，V0最多等有限旧cohort响应就被选中。两个class的RR只需一个轮转状态，而不要求免费无限age timestamp。

如果要求晚到V urgent加入已经queued的A PTE，普通entry可按live waiters更新priority；如果AR已经first VALID/accepted，当前AXI payload不能改变，urgent只能继承**下一可仲裁服务机会**，并承担这一笔正常不可撤尾部。没有有限DDR最大响应时间，就只有PTE服务数意义的等待，不能宣称统一cycle bound。

## 4. 本席尝试的新规则及具体淘汰理由

候选规则：每个shared PTE entry维护受益DID集合/每waiter age；其可仲裁priority取当前live受益者最大priority或累计等待，physical费用按受益class分摊，结果fanout逐waiter记一次logical completion。cohort固定后扣费，后来加入不得改已经结算的归属；late arrivals可以加入下一cohort并命中保存结果。

明确输入是实际waiter DID/class、同key/context/version、请求到达/age、first-offer、R-retire、response握手；state包括private原请求/剩余target/权限及return token、entry key/data、waiter/ref/cohort mask、priority aggregate、物理debit和logical counters。输出是下一PTE仲裁选择、accept/ready、响应顺序、扣费。origin/class不能猜main fillmask或resident way；所有新增fields两方同计。

最小扣费矛盾只有两个waiters：一个physical PTE由A/V共同使用。如果要求物理守恒`charge_A+charge_V=1`，又要求每个受益者都扣一整笔`charge_A=charge_V=1`，两者不能同时成立。这个矛盾来自两种费用单位，不说明谁的调度新。

普通具体控制即可实现一致合同：两个class用半PTE为credit单位，单class服务扣2、两class共同服务各扣1，总扣2；多同DID waiters的logical完成单独计。也可将physical费用全记initiator，其他受益者只记logical service；如果这些额外受益不额外消耗PTE且response仲裁公平，免费复用本身不会剥夺V的physical share。必须先声明按physical work还是logical demand公平，不能事后选更好看的指标。

半单位只是两class的直接例子，不宣传为任意N免费精确分数器；规模扩大须计实际counter/近似误差/溢出。priority可从private Info扫描或保存aggregate，candidate同样需要；closed cohort与response RR解决上面的fairness trace，无需新增独有状态。

这里还有具体前作，而非仅说ordinary能抄任意算法：Dongdong Li 2015 UBC thesis §4.1将后来merger的MSHR信息以dummy request更新DRAM中已经排队的请求；§4.3将entry score设为各waiter age之和并按merge length更新，§4.4给MIB metadata。本席直接核本轮`primary/intercore-thesis.txt:1438–1505,1567–1617`与[原论文](https://central.bac-lac.gc.ca/.item?app=Library&id=TC-BVAU-52650&oclc_number=1033019633&op=pdf)。它不是tenant公平扣费定理，但直接覆盖“晚加入waiter改变共享服务优先级/累计等待score”的核心。

因此**淘汰该coalescing扣费/priority规则作为独立机制**：账本单位澄清是普通per-waiter bookkeeping，late inheritance有具体原作，cohort/准入/response公平有同信息同资源控制。没有发现同资源强普通做不到的V/A性能点。它可以作为未来基线规格，不能凭移到IOMMU就立新。

## 5. root转向job信息：一个可行信息增量与三项反例边界

### 5.1 新信息能区分旧接口不能区分的已注册未来

缓存含X/Y，A当前miss Z，过去lookup/history与当前IOMMU输入完全相同。情况α：已注册V job只剩X的translation用途；情况β：只剩Y。上游iDMA知道descriptor，但IOMMU只见已经到达的burst；旧接口无法区分α/β。新增`J=(epoch,src,dst,len,context-ref)`及可靠materialization/frontier能区分。只靠history无法在这对场景都保留正确known-use条目。

这说明job sideband是**真实额外可用信息**，不是以“普通也能实现同算法”否定所有increment。可能的贡献应明确落在这个新接口、低状态实现的安全remaining-use/demotion规则，以及真实wholecopy收益；允许同job信息的ordinary interval/admission/pin/release是必要强对照。

但registered范围仅是有界已声明用途，不是全未来trace。未知下一job可马上重复被释放的X；不能许诺它无miss。src/dst两个未完成方向之间的下一translation顺序还取决于已准备metadata/W义务和调度，范围membership本身不是精确next-use时间。应先用保守用途，而非将所有remaining页永久硬锁；容量/准入仍需使已accepted jobs有进度。

### 5.2 materialized不等于legalizer已推进，也不等于data退休

原AXI translation response只带一次请求的结果，TL request没有burstlen。一次first-page结果不能自动证明整页所有parent/children已经不再需要全局cache。`resolved(start,len,dir,epoch)`若来自private prepared parent，必须有其可覆盖extent/identity证明；若用physical AR/AWaccept，是更保守的已用译址证据。R/B数据退休不需要作为IOTLB last-use强屏障，因为地址/permission可已在私有TC/parent中。

max address不是contiguous frontier：若后页Y先被materialize而前页X仍未译，取max=Y会错误宣告X用途结束。必须有严格INCR及成功返回顺序，或窗口hole/contiguous watermark。legalizer地址先前进只是产生/排队burst，不说明下面都已译。

MMU-aware DMA ICCD2018的直接原文也支持早用途边界：整transfer≤17页预锁到完成是旧DMA不能处理failed burst的限制；新设计指出TLB项只在burst请求到IOMMU时必须存在，并记录失败burst metadata作重试。不能把整job锁强迫给ordinary。[原论文](https://arxiv.org/pdf/1808.09751)，本席亲读PDFp5 §IV-C。因该文有retry合同，当前无fault/retry多job共享规则还需单独比较，不把未写该新合同等同于新颖性证明。

### 5.3 root要求的保守last-success-start-page：窄接受与未证范围

直接核native TC `rv_iommu_trans_ctl_wrap.sv:121–164,620–727`：AR/AW各FIFO，IDLE只选head且AW优先；单FSM/单`trans_req_data_q`，REQ→AWAIT one pending、translation response ready=1；COMPLETE在**内部axi_comp握手**后才pop对应FIFO。它证明同bank每direction的native TL成功结果按该FIFO到达顺序。内部TC完成可早于外部physical AW/ARaccept，不应再混事件。

因此，在明确限定“单job每direction实际查询严格INCR、固定bank、正常成功、epoch内”的合同下，保守cursor q=该direction最后成功materialized的start-page可用：只将**完整entry覆盖末页<q**的用途判为已越过，当前q页一直保留到下一页或可靠job-end。4KiB entry可简化entrypage<q；2M/1G不能只比较start-page。AR/AW两个方向分别记q并OR用途，不能由某一direction先进推导另一direction完成。

bank不等direction：current same-bank模式两个DMA全部R/W可能共bank0，different-bank模式按DMA原AXI ID选择，故cursor应按实际registered job owner/epoch/方向/context绑定，不能按bank粗略合并；同DID也可能有不同job。跨bank条带、未来out-of-order coalescer不满足上述顺序，必须窗口hole或生产者提供contiguous watermark。

§39 source private helper又不在native one-pending证明内：`hit_bypass_parent_translation.sv:139–158,188–236`只有一个slow_q，但prepared-parent reuse hit可以在background slow_prepare未结束时返回；prepare按每flow oldest-rank扫描，状态是ParentDepth4的valid/seq窗口。`prepared_q`在offer/accept时置位，不是materialized；`valid_q`只在成功response与matching parent/seq时置位。`multi_actual_dma_top.sv:914–922`从这些live parent与seq提供prepare metadata。

若cursor取helper cache-valid填充事件，仍需证明每job raw parent严格INCR、准备没有eligible/pause跳过、generation/slot回收正确。本文没有完成latest带mask路径的无洞证明，**保留已有pending/valid/seq窗口，不先压成一cursor**；可用它产生contiguous watermark。可靠job-end也不能只看legalizerlen=0；需最后translated parent marker与pending状态，或保守用实际backenddone。root缩窄的native合同可接受，不能自动外推全部prepared-source原型。

状态成本也不由O(D)自动变小：D=4个DMA、E=16个cache entry、W=29位page号时，两direction cursor仅payload已经`2DW=232 bit`；逐entry/每DMA/每direction use bitmap是`2DE=128 bit`。两方还要公平计descriptor/context/epoch/valid、pending/window、末页marker及更新/比较端口，cursor的动态区间判断也不是零面积。本文不声称完整元数据更小；下一成本门是从现有private状态取得真实contiguous watermark的来源及全部状态账本，而非先实现232位cursor就宣称节约。

## 6. 这轮留下什么

两条小trace已给出输入、页表字典、握手到达与可观察结果；没有运行，不能称性能PASS。合并扣费/priority支线因为具体普通账本、closed cohort及Li2015覆盖而停止立机制，不靠“不可能所有ordinary复现”作为泛否决。

job sideband保留为root当前单一候选的**信息/接口增量**：精确到已registered用途与可靠materialization，未知未来正常arrival；同信息ordinary控制必须自由选择软保留/驱逐/准入与合法重取。若实际规则只变成descriptor预装/硬锁或rIOMMU顺序last-use，按已有直接前作淘汰；若有同容量跨DMA共享、保守低状态cost与真实wholecopy收益，root再审其明确increment。本席不扩大实现/实验，未证明helper无洞或连续arrival全系统latency。
