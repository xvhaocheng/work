# §41 Codex：budget-off之后的有限继续、最小义务与物理释放

2026-10-08，`innovation_rules_40`。Codex独立推演，直接核§39原型与实际AXI/TC关键源，并重读AXI-REALM primary。未调用DeepSeek/Claude，未执行枚举、DUT run/build或板卡动作，未改生产代码。下述预算关闭是待研究的控制事件，**不声称生产已经装有AXI-REALM或本控制器**。

## 结论与导航

- [§1](#1-释放目标和受限状态)明确物理quiet及允许保留的parent；[§2](#2-必须纳入的初始义务与继续规则)列首次VALID、两W序、payload、TL依赖。
- [§3](#3-单flow固定端点的精确最小前缀)给受限最小前缀固定点与32KiB手推；[§4](#4-闭包不等于可执行的最小反例)给闭包假充分、整事务假环和过度预约反例。
- [§5](#5-实际可证明到哪里)给必要条件及有限继续的受限充分条件；[§6](#6-下一强普通对照及退出判据)给唯一值得核的具体增量与否决规格。

**因为逐拍依赖、前缀守恒、已承诺义务排空及同信息的普通nonuniform splitter能重现这些决策，本稿不提出独立新机制。** 当前可以提出的工程增量只有：相对“预算一关便禁止所有新AR/AW”的错误控制，继续允许清退已承诺W义务所必需的同job source child/raw-parent，以及当前不可停车R所必需的destination AW/TL，同时冻结无关工作。相同debt allowlist、准确W前缀及suffix parking的普通控制器可以实现这个增量；下一强普通若同成本复现，就退出硬件主机制。

## 1. 释放目标和受限状态

### 1.1 先选目标通路，不能用一个busy位代替

固定正常映射、有限线性copy，不含fault、invalidation、ATS、PRI、abort或IOFENCE寿命依赖。每flow用copy offset表示有效payload；不把物理地址差、masked beat或PTE字节当copy有效字节。当前两DMA是主证据，不外推4DMA已验证。

令Γ为希望让给victim的共享通路及其背压传播边界，至少包含所研究的physical source R route、共享W选择器及其前后的必要AW/W pipeline。若B response、TL response/header tracker会阻止victim继续使用Γ，也必须纳入；若它们已在独立通路且没有该背压，才可单独留待退休。这是释放目标的明确定义，不是把所有协议完成无条件绑在一起。

区分两种quiet：

1. **瞬时头部空闲**：现在Γ上没有aggressor token；队列后面仍有aggressor已承诺AW。victim服务到该行时，aggressor重新成为W head；若预算仍关而其source被禁，victim仍会被阻住。
2. **预算关闭后的物理释放**：aggressor在Γ上所有不可撤销的offer/route/W义务已经清退；预算关闭期间不再生成未选进继续集合的新义务。独立私有payload、原parent header、原RLAST恢复计数可以保留。这个目标允许victim继续推进，未要求原job、原parent或所有私有metadata退休。

所以不能只排当前aggressor W head而忽略两序列后面的已承诺aggressor行；也不能为证明quiet而人为要求所有raw parents完成。

### 1.2 snapshot最少需要的字段

预算关闭事件t0的输入状态至少为：

| 类别 | 必要字段及含义 |
| --- | --- |
| Source | 每个已首次VALID/被各层接收但尚未物理R完成的child及其copy offset、有效长度、ID、剩余beats、所在层；共享route/held R归属；accepted raw parent的剩余子片另外记为metadata |
| Destination | 虚拟、物理两W序列每行的flow/copy/seq、有效区间、剩余virtual/physical W；AW锁、AW spill以及首次VALID但未物理握手的AW另记；不能只记owner一位 |
| Payload | 每flow source已交付prefix、virtual W已接收prefix、physical W已交付prefix；各private FIFO有效byte/mask；共享TC W FIFO/spill内payload位置、归属、占用 |
| Translation | 已first-offer/accepted TL及其响应义务；目的/source所需permission/context/PA是否已备；缺少项的PTE返回使用哪个通路、是否依赖Γ释放 |
| Capacity | 可独立停车的有效B、现有占用、独占预约；实际stream transit容量；parent/header/响应slot与各flow隔离边界 |
| Admission | 哪些新raw-parent、child、AW、TL可以作为同job继续；首次VALID之前的门；已offer保持规则；参与队列中可插入的foreign前驱范围 |

Source parent被wrapper接收，不自动意味着其整个原长度已不可撤销地送到DDR。必须逐层区分：透明splitter可以保存原parent，只让已经首VALID的physical child成为物理R义务；反之，普通未分片AR一旦送入不能暂停的共享TC/输出链，就不能假装它只是可私有暂停的metadata。

### 1.3 三个payload前沿

在aligned、无重排的受限copy中令：p_dma为交给DMA私有数据路径的source prefix，q_v为交给virtual W链的prefix，q_p为真正交给physical W的prefix。通常 `q_p <= q_v <= p_dma`；realignment/mask时应改为有效byte库存，不能直接用beat乘8。

`p_dma-q_v`是候选私有suffix；`q_v-q_p`仍在TC/shared transport中，不能当作独立B。最终物理释放若清退全部Γ中W transport，才有 `q_v=q_p=q`，并要求 `p-q<=B`。仅virtual WLAST或source child RLAST不能单独给出这个结论。

## 2. 必须纳入的初始义务与继续规则

### 2.1 初始seed

初始集合S0包含：aggressor所有在Γ或其不可暂停供给链上首VALID/accepted且未完成的physical source child；虚拟W序中所有已首次offer的aggressor AW行；physical accepted/first-VALID AW对应的aggressor行；已存在的transport payload；与这些动作关联且已first-offer/accepted的TL/PTE/response。

如果已有raw AR锁在共享上游mux，冻结其下游接收会保持该mux不能换victim，则该offer及释放接收slot的动作也属于S0。若raw父请求已经被独立每flow metadata捕获，并且不占Γ，则未offer余部不是S0，只有服务已有W债务所需的部分才被加入。

§39原型对raw AR确有额外隔离能力：top `610–618` 按perflow credit直接挑raw AR，`619–622`把source AR从共享`axi_mux`移走。`ParentDepth(4)`不是生产通用mux的性质。实际vendor `axi_mux:413`有AR `LockIn=1`，`428–439`有AR spill；若部署控制器换到生产接口，必须重新检查首VALID的接收债务，不能借原型的私有捕获来证明生产quiet。

### 2.2 含选择的闭包，而非固定的全重叠图

从S0扩展直到无新增义务，但应先确定合法继续端点、parking分配、队列提交顺序。闭包包含下面规则：

1. **已承诺source必须到自己的physical RLAST。** 剩余有效payload可以流向合法W，也可以存进已独占预约的private空位；不能缩短当前burst或依赖尚未确定能释放Γ的全局槽。
2. **已承诺AW必须有足够payload到真实WLAST。** 加入其尚缺的source有效区间、必要raw-parent接收、合法physical child、翻译及返回。下一source child可能从一个新raw parent取得；“禁止所有新AR”会把已有W债务永远留在head。
3. **不可停车的R尾部必须有destination许可。** 若现有W无法消费且private剩余空间不足，则加入最短合法destination continuation AW及其TL。不是所有与R相交的未来AW都必须加入：能独立park的suffix允许对应未来AW留到下次预算。
4. **W前驱同时闭包两序列。** 为完成某row，必须服务virtual与physical两序列中不能越过的前驱；某前驱flow的缺失source/translation也成为必要继续。counterpart未提交时，要沿真实允许的AW提交顺序使其提交，不能把两序列视作可任意交换的集合。共享spill中的旧row也必须排空。
5. **只补必需翻译，但不能撤销已有offer。** 待完成TL/PTE包含在继续集合；尚未offer且与所选释放prefix无关的background preparation被冻结。已有`prep_offer_q`或`slow_q`不能靠关掉新candidate来取消。
6. **容量不可复用预约。** 多flow共享停车槽时先分配B_i并确保总额及当前占用合法；最终 `p_i-q_i<=B_i`。两flow分别声称同一个B会给假证书。stream FIFO还要逐拍满足容量，最终suffix约束不能替代中间容量约束。

所得闭包可能有多个互不包含的合法选择：例如“park尾部”和“多写一个目的burst”均能清退当前R，费用不同；AW提交顺序也影响foreign前驱。因此没有提前选择这些分支时，不能声称存在唯一的‘最小事务集合’，也不能直接用一次普通reachability求出最小buffer/work。

闭包内foreign前驱的必要服务量应单列；它是aggressor释放的依赖成本，不等于这些bytes属于aggressor。未来foreign注入若可无限插到必要新AW之前、译址/DDR可饿死，则有限aggressor集合不能自动推出有限释放。

## 3. 单flow固定端点的精确最小前缀

暂时去掉其他flow及两W序失配，固定可用source physical结束点集合R、destination结束点集合D，均含0/N；已有first-VALID burst不能新增内部端点。B为独占的**最终**private suffix容量，保证stream可以沿所选R/W正常服务，所需翻译也有独立进度。

令L_R为source已经交付以及已不可撤销physical offers结束位置的最大值；L_W为physical/virtual已交付及所有待排空immutable AW有效结束位置的最大值。这里不把未offer raw-parent结束点加入L_R。定义 `next_X(z)=min{x∈X:x>=z}`，越过N为无解。

从 `q_0=next_D(L_W)` 开始迭代：

```
p_k     = next_R(max(L_R, q_k))
q_{k+1} = next_D(max(L_W, q_k, p_k-B))
```

固定点 `(p*,q*)` 满足 `p*∈R, q*∈D, p*>=max(L_R,q*), q*>=L_W, p*-q*<=B`。

**单调、终止、逐分量最小证明：** q不下降，R/D有限，所以不越过N便在有限步骤终止。设任何可行目标为 `(P,Q)`。初始q0≤Q；若qk≤Q，因为P≥L_R且P≥Q，有pk≤P；又Q≥L_W、Q≥qk且Q≥P−B≥pk−B，故qk+1≤Q。归纳得到所有迭代的p≤P、q≤Q，固定点因而逐分量最小。终止点的q等式直接给容量条件。该证明依赖固定端点、独占B及prefix；动态新cut、共享B选择和两W序扩展要先确定，再重算。

这个固定点是守恒与端点向上取整，不是新调度算法。它给最小**最终有效prefix**，不证明在途配置可执行，也不自动最小化source与AW总笔数、TL请求或周期。

### 3.1 代表例：32KiB错位，不能新增source端点

手推状态：`N=32768`，`R={0,2048,...,32768}`，`D={0,1024,3072,...,31744,32768}`，`L_R=2048`，`L_W=1024`或0，`B=24`。这里24B是受限假设，未用48B总配置直接宣称每flow真实可停车。

L_W=1024时：q0=1024，p0=2048；因为2048−24>1024，q1=3072；p1=4096，q2=5120；随后q为 `3072,5120,...,31744,32768`，p为 `4096,6144,...,32768`。直到q=N才可固定。L_W=0时第一轮同样推到q=3072。**即使每笔source只有2KiB，最小最终释放prefix仍到整个32KiB job**；并未人为把parent/job退休作为目标。

若允许在尚未offer的source后续范围新增端点3072，则 q1=p1=3072，新增cut一刀足够停止闭包；当前immutable `[0,2048)` 没被缩短。软件首subjob3KiB，或普通内部nonuniform child `[2048,3072)`，都能达到这一几何结果。

若t0前尚无payload完成，目标p=q=3KiB最多仍需要读3KiB有效数据、写3KiB有效数据，合计6KiB，另有header/TL/PTE/response和foreign前驱；不能把它写成总工作≤3KiB或H=4KiB总工作保障。若已有部分完成，则扣除已交付部分。

本例只在目的消费链ready或有独立TL进度的前提下成立；B是最终parking，不意味着2KiB source无需短stream transit buffer。§39已有1KiB children；这些端点与本D相容，不能把这个禁止新增cut的2KiB构造当作§39成功配置的新发现或DUT实测。

## 4. 闭包不等于可执行的最小反例

### 4.1 整事务依赖图出现假环

单flow、2拍source、2拍W、private FIFO1拍、对应AW已ready。若画“整R完成需要整W消费”及“整W完成需要整R完成”，图有环。实际序列 `R0→W0→R1→W1` 可以完成。对流式DMA，正确依赖是每拍/可消费prefix和空槽token，不是整burst完成。用普通event/prefix依赖图即可修正；这不产生新机制。

### 4.2 所有重叠AW都强行排空会过度预约

source child4拍已经immutable，destination第一AW2拍已准备，下一AW2拍尚未offer，private独立parking2拍。完成source4、W前2，park尾2即可物理释放；没有义务必须现在offer第二AW。若闭包无条件纳入所有与R区间重叠的W，便错误地要求更多AW/TL/服务。

完整R预约4拍也可释放，但它不是最低payload需求：准备好2拍W前缀加2拍停车便足够。**这只反驳‘只能完整R buffer’的弱普通；普通streaming＋准确W前缀＋tail reservation同样达到2拍，不提供候选独占能力。**

### 4.3 Strict stop会使已承诺W或R无法结束

source immutable4拍，已prepared W前2拍，private parking仅1拍；后2拍目的AW尚未offer。若后AW/TL可以独立完成，继续它便可排空；若预算关闭禁止所有新AW，则至少一拍无法存/写，R无法到RLAST。

对偶情形：AW4拍已不可撤销，source仅已offer前2拍，其余源数据尚未到来。禁止所有新source child/raw AR会使W无法到WLAST。旧‘排空既有义务’若只按已接受AR/AW行列清单，而不追踪所需payload和未offer counterpart，也是不完整控制。普通必要依赖debt可以修正。

### 4.4 几何固定点存在，但没有合法有限schedule

把§3.1当前source `[0,2048)`设为已经held共享R；目的W仅 `[0,1024)`已准备，B24。为消费剩余source，下一目的AW `[1024,3072)`还缺译址；其PTE R只能排在当前不可交错的held R之后。source剩余1KiB不能park，W前缀不足，PTE不能返回，三者无法进展。

几何新增未来cut3072不修复当前immutable AR的1KiB尾部，完全允许必要新AW也没有用。需要在source首VALID之前准备足够W链/先切到可排空范围，或另加足够parking/独立PTE通路。这个现场是受限composition反例，**未声称在§39现有准入规则下可达**：§39 `compact_pretl_owner_fragment:87–123`在source offer前计算匹配且已物理接受的W连续prefix，并将child限制在该capacity内，已排除上述不足准备的child。

### 4.5 双W序、spill与TL剩余工作

只有两W队头owner一致不够：同flow两个不同seq或payload区间也可能不匹配。若确有某现场需要服务某row，两序列前驱以及它们的source供给都不能略过；相反，不应凭抽象失配图声称当前同bank trace出现死锁。

vendor `axi_mux:301–309`在首次AW可见就push W-owner row，尚未等待下游AW握手。只收集accepted AW会遗漏真实锁。若启用SpillW，`365`可在WLAST进入spill时pop，上游空队列仍可能有held physical W；**当前mux默认SpillW=0**（`51`），§39输入mux未override，不能虚构那里有该spill。即便当前配置，TC `225–239`的W FIFO仍使virtual W已完成与physical W已完成不同。

背景TL也不能靠关candidate忽略：helper `151–157`继续输出已有prep_offer，`188–209`保存accepted slow请求，`211–214`锁住准备offer，`216–233`收响应。budget-off应冻结新的无关candidate，而已有offer/slow及其PTW返回仍需完成。把保留parent本身全部算继续TL需求则又过度；它未offer的prepared miss不是不可撤销义务。

## 5. 实际可证明到哪里

### 5.1 必要条件

任意正常无abort释放schedule都要完成S0中占Γ的不可撤销动作，满足两W序、未缩短current physical burst、有效payload守恒、最终独立parking及所有中间buffer容量。单flow受限情况下最终目标不可能小于§3的固定点。多flow对每个实际W前驱提高L_W后再闭包；共享B的合法分配、所有必要TL的返回路径是额外必要条件。

任何只含‘集合已闭合’或‘所有debt得到预算豁免’的规则，都缺少实际进度论证；集合闭合可以同时是等待环。总N有限与outstanding项数有限也不能单独打破环。

### 5.2 准确充要条件与可实施的充分条件

对完整snapshot、合法continue动作集合和真实RTL转移规则，准确的充要条件是：**存在一个有限、满足所有协议/资源约束的执行，把state带到§1定义的释放目标，且所有新动作均在继续集合中。** 这是普通有限状态可达性/有资源的调度；直接重述不能称创新或低成本certificate。

一种可证明的受限充分构造为：固定端点与B_i；所有相关两W前驱具有兼容的可服务顺序；必要翻译在held data期间已有结果或有独立进度；物理source首次offer只落在可消费prepared W前缀加独占parking范围；保持所有首VALID；逐拍为stream FIFO槽分配明确的produce/consume顺序；允许必需counterpart、禁止无关新义务；共享fabric对这些持续可服务offers/响应有进度保证。

若把以上具体合法选择展开成完整beat/prefix事件和slot释放token，且已记录的全部AND依赖可拓扑服务，则事件有限，可归纳逐个完成，最终释放。可选择park/write的OR分支必须先决定。**不能把抽象whole-burst DAG无环当充分条件，也不能从AXI握手协议本身推出公平/有限周期。** 完整事件表可能很大，此稿未声称存在低成本在线RTL检查器。

工作量可以对被选集合数剩余有效source/W beats、新header、TL/PTE及foreign前驱；只有外部每一步的等待有上界时才能转成周期界。H只限制静态目的检查点距离，没有首次offer范围栏杆就不能约束现场debt或这些工作。允许所有有限同job继续可给N尺度上界，但这不是小H释放保证。

### 5.3 现有普通已经区分tail与parent

§39 `compact_pretl_owner_fragment:265–268`仅在原parent最后beat时done++；`270`却在每physical child RLAST清busy。`134–141`恢复原parent RLAST。因此“保留原parent metadata而提前释放physical child”是现有普通透明分片能力，不能作为新贡献。top `549–575`的direct/normal source是单个global child，其route归属保持至child完成。

parent quiet只有在真实共享header/response槽被它占满并使victim受阻时才成为进一步目标。§39每flow raw credit与专用捕获提供局部保留的边界；生产不同接入点要核其真实槽及锁，不应统一强迫parent退休，也不应统一假定parent留存免费。

## 6. 下一强普通对照及退出判据

### 6.1 可否决的唯一具体工程增量

在相同§39 prepared-source旁路基础上，增加待核的budget-off控制时，仅测试这一规格：

**输入**：§1.2 snapshot、budget事件、所选source/destination release prefix及B、prepared响应。

**状态**：冻结位；t0既有AW/physical child/TL义务；两W queue的row/seq与progress；必要source/destination有效prefix上界；独立parking预约；已first-offer的稳定payload。原parent关联沿用现有结构，不新增‘必须全部done’语义。

**输出**：预算关后只允许清退既有不可撤销动作，以及上述固定点/两W前驱所必需的同job child、raw-parent接收、destination AW/TL；无关future children、AW、background prep不再首次offer；所有已有VALID继续保持；quiet只有在Γ的真实stage clear时才报告。

**事件**：t0 snapshot；首次offer增加debt；TL结果准备；各R/W prefix进展；两序列出队与pipeline排空；child RLAST释放；parking占用/释放；最终quiet。不以original parent done替代child release。

§39 child已经受完整prepared W-prefix保护，所以**单独完成当前child不需要新destination AW**。主要继续量来自t0更远已承诺AW：必须供给其payload，可能需要下一child甚至尚未接收raw parent。若关预算后只禁止新physical child，哪怕当前child已quiet，也可能在未来aggressor W head停住。这个对照应测试真实两W序剩余债务，而不只测busy下降。

### 6.2 Strongest ordinary，不能只拿strict stop或整R预约比

对照普通实现取相同source/destination copy关联、prepared/cache状态、W序/seq、B及continue allowlist：普通nonuniform splitter保留原parent/RLAST；首offer范围栏杆；prepared W-prefix数据供给；普通stream FIFO＋suffix独占预约；已承诺debt排空及必要counterpart豁免；相同background prep冻结与stage-clear监测。它可以逐事件产生候选相同的新child/AW/TL和相同quiet时刻。当前没有发现它缺少的信息或动作，也没有证明候选state更小。

完整R预约＋ordinary AXI isolate是另一个有用控制，但它若buffer更大不能单独证明创新；应同时保留上述小buffer streaming普通对照。软件/descriptor tiling也必须保留：本32KiB错位例首tile3KiB及随后4KiB等tile可以让backend_idle成为更强边界；供给、TL、macrocopy身份/完成合并费用要实计，不能以未要求单硬件job为由拒绝这条实际普通方案。

**否决条件**：相同有效payload容量、parent/header容量、W tracking、TL资源与接口语义下，普通控制能重现必要continue及finite release，新增控制便只有正确集成/验证价值；退出独立机制主贡献。若为候选另开PTW通路或增payload/metadata槽，必须给普通同等资源，不能归因给闭包。

**只有如果**找到本平台真实可达的snapshot，使最强普通在同信息、同可用动作和同资源下无法给同保证，或者候选对相同精确保证提供实际可验证的更小state/更短组合路径，才出现未被本等效构造覆盖的增量。此稿没有这样的见证，明确退出将‘义务闭包/释放证书’单独命名立新。

## 7. 已直接核的导航

关键源码均在当前工程，行号为本稿核查版本：

- [§39 source/W-prefix与parent分片](/home/xvhaocheng/work/iommu-platform/out/prepared-source-bypass-20261008/compact_pretl_owner_fragment.sv:87)：87–123已prepared匹配前缀；210–239两W ring；241–254child firstoffer/接受；265–270parent与child分别结束。
- [§39 raw-parent隔离](/home/xvhaocheng/work/iommu-platform/out/prepared-source-bypass-20261008/multi_actual_dma_top.sv:610)：610–622绕过共享AR mux；549–575单global child route；872–894实际观测接口。
- [§39普通TL准备](/home/xvhaocheng/work/iommu-platform/out/prepared-source-bypass-20261008/hit_bypass_parent_translation.sv:144)：144–158candidate/offer；188–233在途请求及回复。
- [实际AXI mux](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/vendor/axi/src/axi_mux.sv:283)：283–347首次AW/W-owner push及AW spill；360–380 W pop与可选spill；413–439 AR锁与spill；50–55默认SpillW/R关闭、AW/AR开启。
- [实际TC W流式状态](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:174)：174–179物理AW/W计数；222–239 W FIFO；332–368physical header/data。
- [真实byte FIFO](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_buffer.sv:33)：33–50逐byte stream FIFO，不能只用总配置B抹掉mask/占用。

[AXI-REALM作者primary](https://arxiv.org/pdf/2501.10161)，§III-A1/A2、§III-B：已有透明AR/AW/W分片、B合并、原RLAST恢复；完整fragment W在buffer后才forward AW/W；另假设互连进度与RR。本文核的是本平台R→W/PTE供给与budget关闭如何满足这些前提，并不证明AXI-REALM论文承诺了本平台该组合，也不声称本平台部署了它。

本轮只完成理论/源码核查。root已报告§41静态几何oracle 11390分类、greedy/brute mismatch0；该oracle不检查这里的TL依赖、两W序、budget-off继续或physical quiet，本席未把它作为本稿机制验收。
