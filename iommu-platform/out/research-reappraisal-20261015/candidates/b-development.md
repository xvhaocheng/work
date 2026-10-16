# B：跨payload所有权边界的有限翻译准备

2026-10-15；本席独占来源核查与机制设计，交root裁决。Codex-only。0新模型、实验、RTL、driver、ABI、构建、板操作或旧goal/旧席恢复。

**决定：保留B的核心，但重设计其论文主张；本轮不准入模型。** 真实应用中确有“未来外存地址可先求出，而对应SPM载荷槽仍被计算占用”的自然窗口。可是仅在这个窗口提前walk、以进度限定预取距离，ICCD2018已经做到。最强合理B应改为明确区分翻译准备与载荷提交的有限生命周期合同，再证明一项实际部署或全成本增量。当前没有证明普通预取、有限pin、prepared-PA队列及同信息ready/EDF留下这种净差；这不是全领域架构已发表或整个思想族无价值的结论。

本文件的B是本轮“跨payload边界准备”，**不是**2026-10-14 FLOW表中B的主动push/组播命名，避免同字母误继承贡献。

## 1. 一个查得到的自然所有权窗口

最强现有入口是固定HERO官方应用 `openmp-examples/mm-large/mm-large.c`：commit `c8481a930a15a385d4f58f519bed83d2e75e2a73`，SHA256 `8b16b51169bb31b7eb290eb5f309ddfbba25fa94e3fae3b327baadf491360186`。[固定源码](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/mm-large/mm-large.c)，[本地原件](/home/xvhaocheng/work/iommu-platform/out/workflow-stream-contract-20261012/source-evidence/hero/openmp-examples/mm-large/mm-large.c:164)。本轮直接读源码，不只引用旧摘要。

| 原件位置 | 直接事实 | 可推导而未运行的事实 |
| --- | --- | --- |
| 44–70 | `a,b,c`和尺寸在调用时给定；A/B/C各两个固定SPM槽 | 对固定迭代号，未来host地址和目标槽可在当前槽释放前计算 |
| 164–202 | 每轮切换B槽，提前搬下一条带；下轮host源是`b+(t+1)*stripe_size_b`，并等当前条带DMA完成 | 对`t+2<n_stripes`，在t轮就能求出`t+2`的源地址，无须先触碰那个payload |
| 208–228 | barrier后，当前计算读`b_ptrs[!b_idx]`并写当前C；`omp for`未加`nowait` | 当前B槽在计算结束前不可被`t+2`覆盖；正常worksharing-loop结束有隐式barrier，下一轮复用才安全 |
| 143–161、214–236、249–254 | C要先算出，再由DMA从对应C槽写回已知host条带 | C的外存WRITE翻译可先准备，但DMA不能提前读尚未生产完的C payload |
| 284–305 | 默认height256，源码规则得stripe_height16、16条带 | 每条带16KiB、六槽96KiB是源码尺寸，不是后来为B安排的参数 |

OpenMP循环末隐式barrier依据[官方OpenMP 5.2 §11.5](https://www.openmp.org/spec-html/5.2/openmpse66.html)；这里使用默认`omp for`语义。不能把当前8核执行时间按核数直接移植到Genesys2。

这个窗口没有人为插入延迟，也没有增加background流量。具体地，在t轮计算占有B槽时，B(t+2)的host源翻译可先进行，**B(t+2)的数据不能先写进当前计算槽**。SPM槽的payload所有权和host源页的翻译权限是两件事：B原始矩阵已初始化（320–325），而源级推导允许求未来地址，不代表原应用已经发出了纯prepare调用、注册了release事件，或当前硬件已有该接口。

必须保留的限制：

- 地址可推导不等于映射已被授权、页已驻留或可绕过正式DID/PID/stage检查；SPM本身是物理存储，准备的是host源READ或host目的WRITE的翻译。
- 上述是**源码级合法先行窗口**，没有观测当前CVA6实现中其实际持续时间、对应IOTLB miss、可避免stall或净完成收益。
- [已验收R2工作流复核](/home/xvhaocheng/work/iommu-platform/docs/workflow-stream-platform-review-20261012.md:26)记录全部255次tile交接均无额外输入/输出槽气泡，后台提前结束；它不支持把这次窗口包装成已存在的残余性能损失。
- 原ICCD2018 SP也有真实双缓冲，PDF7页§V-B/TXT437–450、PDF8页Fig.5明确16KiB/4页；本轮直接看了既存的[原页图](/home/xvhaocheng/work/iommu-platform/out/workflow-stream-platform-review-20261012/mmu-aware-page8.png)。但该原SP具体kernel/逐块迹未取得；HERO mm-large是另一份真实官方程序，不能混称同一应用复现。

## 2. stage117能与不能作为B需求的部分

DPDK v25.11固定源码 `drivers/net/virtio/virtio_rxtx.c`：285–321把接收mbuf的addr/len/WRITE/cookie发布到可用ring；173–200按used ID取回原cookie。它证明“未来目的地址可以先已知”和真实接收所有权，但**已发布RX buffer已交给后端写入**，不证明“地址已知但仍被旧consumer占有且不能写”。不能把两个状态混为同一正例。[本地原件](/home/xvhaocheng/work/iommu-platform/out/stage117-r3-astra-20261013/source/dpdk/drivers/net/virtio/virtio_rxtx.c:285)，SHA256 `e810f63638f791c147167008ff0cb6ea6e48c418b0d1453a6dbc8ae6c85e356e`。

[stage117交付G0](/home/xvhaocheng/work/iommu-platform/docs/stage117-delivery-g0-20261013.md:15)使用同一16KiB物理池、驻留2MiB leaf，A归还只作逻辑事件、物理源未复用成下一包、映射一直驻留；12次lookup中11hit。它没有观测B所需的重复占槽→释放→新payload使用序列。六案控制已经是有效普通结果，不能重新冷却映射、延长consumer、加包或压力来制造B窗口。其真实cache交接、逐包完成与完整成本资产可复用，已有阴性结果保持。

## 3. 最强合理B合同：先准备，再按真实释放事件提交

只提出小合同，不把预取、TFID、deadline、uTLB/push、全新多walker合成大架构。

**输入。** 可信应用/前端登记有限未来span：真实地址空间身份、IOVA范围、READ/WRITE及实际属性、任务/槽身份、该槽的payload-owner代、已具来源的释放事件。映射身份须由正式上下文解析得到；数字IOVA、TFID或软件自报代不能代替权限。未来地址只能来自稳定参数、正式descriptor或已完成的数据依赖，不能使用未来oracle。

**PREPARE。** 在持有K个有限翻译准备credit之一时，发纯翻译请求；它不发布payload AR/AW/W，不接受/复制payload，不借用当前计算槽。请求被接受后，保留完整pending身份直到响应/合法终止，响应恰一次。单正常PTW配置仍只有一份active walk，准备请求也占其时间和PTE流量。prepared结果可用两种明确付费实现：IOTLB entry有限pin+引用，或独立PA/permission结果槽。两种都要同强对照比较，不规定对照必须复制完整raw/prepared header。

**READY。** 保存一个只对真实mapping有效的结果和关联。准备完成只表示翻译可用，不能当作buffer可写/可读，也不能发布used、B完成或释放旧owner。准备失败给真实fault；如果采用无副作用hint失败转demand，必须把该行为写进双方合同，不能隐去fault或把失败当零成本。

**COMMIT。** 只有真实release事件发生、owner代匹配、正式映射仍有效且原事务次序允许时，才把准备结果移交给即将执行的DMA请求。以目标/结果接收握手为所有权交接点；不可在下游尚未接收时释放最后一份PA/权限。旧owner的计算、R/W/B债务和cache交接继续存在，不能靠prepare完成消掉。首次payload VALID后保持稳定，不在held VALID上换地址或改策略。

**回退与退出。** 准备过晚/槽满/映射变化/取消时保留合法demand路径，有界老化和demand保底；已启动walk若不可取消就等真实退休，不能中途抹账。没有新应用事实时，先限定稳定映射及有界成功任务；若未来确实包含异步remap，则必须把prepared结果纳入与普通翻译同样的失效/ack/在途排空，不能只加generation或定时lease声称安全。本轮不把未来未出现的remap用作立题动机。

**本合同能正向保证的范围。** 源码条件允许在不扩大payload存储、不提前改变buffer所有权的前提下把翻译服务放进自然计算窗口；K使准备存储/在途状态有限。此处是协议草案和条件推理，不是RTL/形式安全验收；K本身不保证及时完成、公平或任务加速。

## 4. 与R2/O-B5S的确切边界

[R2完整裁决](/home/xvhaocheng/work/iommu-platform/docs/r2-ownership-cost-20261014.md:19)已经让**真实接受的virtual AW**进入同一raw/prepared union cell；NormalTL响应后复用字段，完整内部移交后提前释放原cell；O-B5S以同样状态、端口、事件做到写请求管理1329bit加相同公共项。不能再次以原位union、early release、无需第二份prepared头、保持原序为B新意。

B现在讨论的是：未来任务已登记而**对应payload DMA尚不能发起**，因此尚没有那笔自然生成的virtual AW/AR时，允许做纯翻译准备。与旧R2承诺时点确实不同。但这个前门、task/span记录、结果容器和接收接口都要收费；它们不是当前免费能力，也不因旧R2没有前门就自动新颖。若人为提早接受payload AW再延迟W，只是改变债务/资源，不等同无payload承诺的prepare。

本轮直接核当前 `idma_reg64_frontend.sv`325–327、490–524、588–610：地址/length存在，COUNT首次VALID后稳定，ADMIT/HOLD无backend VALID；它没有该未来span登记+纯prepare+结果采纳协议。[原件](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:325)，SHA256 `b1800c757d28f9c21b0a4a484d888313145c0805194dd7c7766e337f88c57df7`。这里地址可见与权限有效仍分开。

翻译wrapper 174–213请求仲裁/spill、282–299的两bank生命周期/公共返回和quiet，说明不能把一条纯prepare请求免费塞进“8 outstanding”的空口号。新的结果owner/tag/出口若增加，就明确计价。[原件](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:282)，SHA256 `d3ab23374604d16d4cccbfafb012c65fbd5e55c738d94672b7b515a9a1fa0aae`。这些是当前源码事实，未绑定当前已加载bitstream。

## 5. 最近前作与具名强对照

三篇完整证据卡及PDF/TXT哈希见[文献卡](../literature/b-cards.md)；准确差异不能被下表省略成名称比较。

| 对照 | 来源类别、已具动作 | 对B的约束与必须收费处 |
| --- | --- | --- |
| O-HTP：有限进度纯翻译预取 | **发表前作**ICCD2018 PDF4–5页§IV-A、TXT272–347：由WT进度限定d/D，hit不访问payload | 覆盖提前walk及有限lookahead。helper PE、progress通信、队列、TLB容量和PTE流量全计；不能强迫它等payload释放才prefetch |
| O-PIN：同信息有限pin | **具名普通构造**；短期/整DMA pin有ICCD2018 PDF6页§IV-C和PDF8页§V-C前史；本轮具体“future span+release event”调度不是据此声称原作已实现 | 给同future地址、release事件、K和正式权限；预取后pin，真实移交/最后使用解除。pin元数据、victim受限和被延迟demand计入。不能把普通锁死整笔DMA作为唯一实现 |
| O-PAQ：有限prepared-PA队列 | **具名普通实现构造**；不是本轮找到的“已发表整协议” | K项保存真实PA/权限、task/owner引用及pending身份；payload释放后发起合法请求，按握手移交；允许union字段、ring指针、已消费字段回收和同样失效处理。若它与B逐状态/端口等价，只排除B的相应策略/逻辑成本独有性，不证明该接口已发表或整体架构无价值 |
| O-READY / O-EDF | **普通调度构造**；只使用已知真实ready/release和来源支持的期限 | 两者得到与B同样信息、同K和同准备前门；没有应用deadline时只能用真实ready/依赖/有界FIFO，不额外发明期限。若B只是按release先后选队列，不能独称新算法 |
| O-PWC/LLC | **发表前作**HPCA2017 PDF8–9页§IV-D；DATE2025 PDF4–5页§IV-B/C | 缓存及时更新，不人为禁掉页表复用；记录PTE访问/bytes与未命中后真实残余。DATE平台的低残余百分比不能直接挪到本平台，也不能忽略其强替代意义 |
| O-layout/residency/large-page、普通数据提前搬运 | 既有平台可用普通能力；stage117已真实使用2MiB驻留leaf | 对合同允许的布局、驻留、大页、额外payload存储都计全成本后开放。若一项合法普通方法已满足目标，不能为B排除它；若增加payload槽，则给出真实面积/持有/带宽预算 |

同新增前门的O-PAQ可能是B的另一个实现，而非独立前作；具体实现须按来源和成本比较。既不要求候选击败自己，也不能因换名实现同动作就宣称独立策略增量。

## 6. 完整成本、决定性正例与相邻反例

最小账本是下列全部项；未实现项为unknown，不能记0。设准备结果项数K，登记span数N。

- **登记/软件。** N个future span的上下文、范围、任务/槽、owner代、release关联；枚举跨页和大页覆盖、不可伪造身份、读/写权限、注册/取消、helper执行或CPU供给及通信。预测release时点若用profiling也收费。沿用已有descriptor字段时要核真实可消费时刻和端口。
- **翻译债务。** 单PTW实际服务时间、DDT/PDTC/IOTLB查找、PTE层数/bytes、cache端口和替换、speculative浪费、pending-key去重及准备抢占demand的额外等待。K个结果槽不等于K个walker。
- **保存/移交。** K项真实key/PA/permission/页粒度、pending/ready/fault状态、owner引用/指针；lookup比较器、read/write端口、response mux与背压、commit联结与原生请求的合法采纳。pin占据公共cache或另加PA队列二选一均计，不伪装免费两份资源。
- **生命周期。** 旧payload owner计算与R/W/B、cache交接、异常、取消、失效/ack/排空；真实用完及最终mapping释放。buffer owner代和mapping有效性不能混成一个代。稳定mapping下可省去没有需要的代，但双方同样可省。
- **完整任务。** 共同逻辑ready到所有要求的consumer完成/最终释放；每任务完成向量与真实后台工作；原始数据读写字节、hold bytecycles、CPU准备/供给/cache/consumer开销。准备walk早结束不等于完成改善。
- **物理实现。** FF/LUT/BRAM、组合路径/Fmax、端口和连线、能耗/软件/板端时序；本轮全部未测。不能以header字段算术代替总面积。

纸面正例只证明动作有用的可能条件：某条future host span地址在`a`时刻已可求出，相应SPM槽要到真实计算释放`r>a`才允许payload DMA；翻译服务L，且在可用窗口内能完成、结果在commit仍有效。若其后payload及消费者确实等这个翻译，早准备可将release后的翻译剩余从L减到`max(0,L-(r-a))`，理想减少至多`min(L,r-a)`；需要扣实际供给、lookup/commit、争用及cache交接。这是手推时间关系，**不是新运行或对O-PIN/O-PAQ的胜例**。它们同样可以取得此上限。

真正可判别B增量的正例必须额外给出：在HERO自然映射/地址/计算条件下，缓存预取会因真实活跃页竞争丢失有用结果，而同预算pin/PAQ有一项真实状态/端口/移交或完成成本不能覆盖B；也可以是同完成目标下B严格降低可实现总成本。当前没有该原始事实或精确差异，不能先加干扰、限制对照或人为拖release再造见证。

相邻反例至少包括：翻译本来hit或LLC/PWC足够快；自然计算完全隐藏普通demand；普通PHT及时且未被驱逐；K项pin/PAQ同样覆盖；映射在准备后变化使结果作废；预取抢走PTW/cache/内存资源造成其他真任务更慢；准备信息生成费用大于藏掉的剩余；payload/cache/B债务才是最终关键路径；未来地址依赖尚未完成的payload读取。这些反例不否定源码窗口，只改变净收益和适用条件。

## 7. 可核查novelty论证与当前推进条件

ICCD2018已在共享TLB中利用worker执行进度，通过有限距离的纯翻译请求在payload使用前准备映射（PDF4–5页§IV-A/TXT272–347），并用短burst退休减少长期锁页（PDF6页§IV-C/TXT371–412）；HPCA2017已提供分级TLB、跨accelerator同页复用和host-PWC/cache支持（PDF6–9页§IV-B/C/D），DATE2025进一步在RISC-V scratchpad平台实测LLC加payload bypass可显著降低翻译残余（PDF4–5页§IV-B/C）。本B拟把**翻译准备许可、结果所有权和payload-slot释放**明确分成有限的prepare/commit生命周期，研究不增加payload槽时的可部署成本和完成边界。HERO源码支持地址与槽释放解耦这一问题存在；但当前尚没有原始证据证明该合同在既有普通预取、pin或PAQ之外带来新能力或净成本差，也未核全领域同类协议。因此这里只保留可检验架构问题，不将该合同写成已成立独立方法，更不宣称首次。

一句条件主张：**如果自然任务中future翻译可先行、残余翻译确实阻挡有用完成，且有限prepare/commit的真实状态/端口/交接成本相对同信息pin/PAQ存在可核净差，B就可能构成有意义的架构或方法增量；如果具名普通方案在同完整预算下逐状态覆盖，则不能把对应调度/逻辑成本作为B独有贡献。**

当前决定分三层：

1. **停止B0的宽泛新意主张。** 因ICCD2018已有决定性方法原文，不再主张“地址已知而payload不ready时提前walk”“进度感知有限预取”本身独立。
2. **保留B1合同作重设计材料。** 源码自然窗口成立；与O-B5S的承诺时点不同，不能将旧R2结论直接覆盖过去。当前没有可准入模型的独立动作或全成本差，不新增原型/性能序列。
3. **下一关口是定向证据，不是扩矩阵。** 仅当新原始事实给出HERO或另一已授权真实程序的自然release/lookup/完成链，并指出上述具名普通方法的具体不足或可实现成本差，再由root决定是否冻结一次最小模型。没有新事实时不恢复旧R2/stage117实验，也不把未知物理成本当差异。
