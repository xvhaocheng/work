# MAP-OWNER-REPRESENTATION-COST-1：表示寿命与完整成本

2026-10-16 JST。独占表示设计／读者分类／完整成本差项；不重做TX完整调用链。请求`gpt-6.1-sol/xhigh`，本席没有独立运行元数据，actual model／effort均为`unknown`。Codex-only，未调用DeepSeek／Claude；沿用已定ARS有界机制设计阶段。本文件是条件设计，**不是实现、并发验证、状态／性能模型或原生测量**。

## 1. 结论

**“UNMAP_DONE后还要等一次逐record RCU grace才能返K”不是跨表示不可避免的费用。** 如果索引的全部链遍历、状态／地址读取和无frame控制查询都在同一正确的短锁保护下结束，锁外只留下已复制的值；唯一creator／closer的operation责任又阻止记录被提前复用，那么冷页已被owner实际隔离且unmap及所有旧记录责任结束后，可以不等逐record grace就返K。原frame／全部fragment责任保护锁外的DMA使用，不能反过来声称它保护任意控制面裸指针。

首版最易验收的具体实现是**每(source pool,固定B/config)一个irq-safe index/state锁**，统一保护稀疏bucket链、K free list和状态；它付每hit共享锁及争用费。真正的多bucket版本可以减少争用，但须按完整key重新查找，不能拿旧slot号／旧bucket直接访问已迁移复用的记录。本文把该优化单列，不把复杂版本当已验证首版。

还有更强的普通相邻实现：**ctx寿命内固定地址、无next链、只存值的open-address cells，加短writer锁和覆盖整次lookup的版本快照**。稳定hit可能同时免reader锁及逐record grace；冲突／odd立即进入同一writer锁完整relookup。它仍要付固定cell空间、探测／墓碑、版本读写及冲突慢路等待。因此“每hit必付锁费”也不是所有可行表示的下界。root负责核对应v6.12 seqcount原文和具体API；本文不以伪代码当内核内存序证明。

这轮**具体化了前轮已明确允许的锁／等价保护和付费直表**，增加可检验的K周转费用差项；没有发现新锁算法、安全原理或独占策略。普通R／O3／dma-buf slot适配同享任何表示。不要求击败它们照抄同一实现才承认部署／工程价值，也不能把它们能照抄写成完整成本已实测相同。

## 2. 前轮到底覆盖到哪里

以下定位均为本轮实际读取的旧产物，不是新文献发现。

| 旧原文 | 已覆盖 | 本轮细化，不能反称首次提出 |
| --- | --- | --- |
| `mapping-integration-delta-20261016/owner/integration-analysis.md:38–42` | lazy sparse K记录；bucket/record spinlock；CPU reader用RCU**或等价保护**；控制reader须有真实metadata引用或全局互斥；首次CREATING可枚举、唯一creator/closer | 所有链遍历／控制查询统一锁内结束，逐record不再需要RCU引用；operation责任如何结束后返槽 |
| 同文`:64,94–98` | 不持锁等DMA／grace；creator/closer交接；Q节点预留；当前worker自身用完字段以前不可复用记录 | 不是所有Q都来自CPU grace；锁版只删其真实不再需要的record-reader退休责任，backend／页退出Q照留 |
| 同文`:79,112,118–122` | 付费PFN直索引／frame cookie；直表覆盖空间或传递链费用；允许后续具体表示差项；各资源分报 | dense/direct的具体覆盖范围与读取寿命；同K、同RAM条件下是否可用 |
| `primitive-table.json:53–105,137–160,283–297,313–320,389–429` | hash/xarray/direct；每hit仍有CPU reader寿命；预留退休和延迟复用；双方同表示 | “grace或等价”不是“每个record必等RCU”的证明；本轮给具体等价保护 |
| `mapping-owner-publish-20261016/root/decision-analysis.md:31–53,65–69,99–109`及`hypothesis-card.json` | K含在退记录；只在**必要**CPU／索引／异步责任实际结束后返K；ctx/config/NAPI寿命另算；未找到同接口独占原语 | 逐record责任可以在短锁／唯一操作结束时已全部完成；四层完成仍分开，不要求UNMAP_DONE与RETIRED之间必须有正时间间隔 |
| `mapping-recycle-reclaim-20261016/root/decision-analysis.md:25–38,44–52,58–74` | owner实际摘private cache页；不能只读pp_ref判idle；hook在RCU读侧不能等自己的grace；能力与净收益分开 | 不等逐record grace是换表示，不是消除所有CPU同步，更不替代设备DMA完成 |

原文已有“RCU或等价保护”，所以本轮是**原来允许选项的具体化与费用边界修正**。若把上轮例子“ACK时K尚未返”扩大成所有表示都必须延后一轮RCU，则这个扩大解读被锁版条件例否定；上轮“必要退休完成才返K”的安全规则保持成立。

## 3. 三种主表示与一个最邻近增强

`K`是既定持久关联记录／在退责任预算，不是pool总页数。`H`为索引bucket或固定cell数量；`U`为直表实际覆盖的可能页索引集合大小。没有给H/K比例、结构字节或时间常数赋数值。

| 表示 | 具体结构和正常hit | 记录复用条件／完整代价 |
| --- | --- | --- |
| **R-S：RCU sparse，嵌入node的K-record** | 固定ctx表＋稀疏bucket，record内嵌next和source/target/retire节点；writer短锁；reader在RCU保护下遍历、读取有效状态／dma_base，锁外DMA仍由原frame保护 | erase后旧reader可能仍沿旧node/next遍历，必须等真实grace且所有creator/closer/callback责任结束才可重写node／返K。固定arena地址与generation校验本身不保护旧next链。付RCU发表／回调、在退K库存和因此发生的fallback；reader可能已在外层RCU中，不能强收一次额外entry/exit费 |
| **L-S：短锁 sparse＋原frame借用** | 稀疏key→record；首版同pair一个L保护所有bucket链、free list、状态、查询和控制再查。hit在L内复制所需IOVA／状态值，解锁后不得保存record/page/config裸指针；原frame责任继续至真实完成 | 删去索引后没有越锁reader，唯一操作与异步责任结束即可返K，无逐record RCU。付每hit L获取／释放、cacheline转移／串行化／irq mask，以及miss/close锁费；整体ctx/table/config/NAPI销毁同步照付 |
| **D-D：dense/direct** | (a) ctx寿命内固定PFN覆盖side table，每个可覆盖项保留状态／DMA值；(b) K槽直接cookie由source合法取得并随frame传递。状态出版和控制查询仍需正确协议；LIVE期间原frame可保护稳定值的使用 | (a)付U覆盖空间和初始化、cache/TLB访问，普通page_pool没有自动提供紧凑PFN集合；radix目录另付节点／lookup，不叫零成本直表。(b)付cookie字段及来源、每次alloc/prepare初始化／传递／失败／完成覆盖、代际与重用；现有A dma_addr和XDP mem.id不是免费B cookie。两者都不自动需要PFN仿射IOVA窗口，也不自动继承PMD leaf |
| **V-C：固定value cells，相邻增强** | 固定H个无next值cell；单writer锁＋版本快照。完整lookup读到稳定key/LIVE/dma_base才能hit；odd/冲突进入L完整relookup。稳定hit可能无reader锁 | 不重用cell内存，只更新其值，不用逐record grace；仍须无旧操作／DMA／page责任后才释放K。付H-cell空间、探测、墓碑、版本和冲突慢路；它可能比L-S更强，也可能比简单锁更贵，未测 |

R-S是本轮的**具体嵌入node强对照**，不是“所有RCU结构都必卡K”的定理。若把不可变key/state/dma值完整复制到独立RCU node，可先释放另一层association slot，再退休旧node；这会新增／转移有界node预算、回调和缓存足迹。若node及其退休仍计入原K，容量约束未凭名称消失；若另给node池J，则J和耗尽回退同样付费。允许这种普通改进，但不能拿额外空间与本轮固定K/metadata预算做免费比较。

## 4. 读者分类：谁保护什么

stock v6.12没有本文新增的B缓存record。下表是将拟议访问点绑定到真实已有frame／源池／控制职责的分类；完整TX调用链和目标关闭原始核查由本轮source席／root负责。本文直接重读的原始范围为`page_pool.c:422–436,717–790`和`helpers.h:225–287`，其余准确原文定位复用上述已接受材料，不伪称本席重新全链核验。

| 访问者／实际职责 | 可合法留到锁外的东西 | L-S的硬性边界 |
| --- | --- | --- |
| B prepare／正常hit，尚持原frame及全部相关片段责任 | 已复制dma_base/offset及必要值；有效target配置来自独立gate/config合同 | 从链头到next遍历、key/state/地址读取都在L内；不保record指针。每个frame自己的责任不是给任意后台reader的保护 |
| 首次map creator | 一个唯一、不可被别人回收的operation责任；其frame；已稳定的map参数/config责任 | L内先占K并使CREATING可枚举，L外map，L内复核open/epoch后publish。关闭只能标取消／登记责任，不能在map未返回时偷换record；失败由一个责任人卸私有图 |
| CQ／合法设备drain完成 | 原frame、mode和完成所需既有值 | 窄版完成不需record handle／查询；归还frame后不继续读record/page。若实现仍新增这类尾访问，就必须重新计其保护 |
| source allocation owner冷回收 | 从private cache实际摘下的页的源所有权；一个唯一closer责任 | 只能在原非并发cache合同下摘页；`PP:427,721–731`有直接依据。cache里实际合格且全部fragment/bias结束，不用pp_ref数值1猜idle |
| source真正release hook | 尚未执行最终put的页退出责任；或显式转给worker的同一责任 | L内定位／claim；锁外unmap／终结段。release_cnt以后不回读pool；异步worker仍有责任时不得返K；不能在NAPI等反过来等自己的closer |
| target枚举／关闭者，没有本页frame | 锁内取得的唯一operation责任、其独立config/ctx引用；或纯值快照 | target gate先关新准入并按既定合同排CPU发布者／真实DMA；L不是DMA drain。纯查询不许拿page*/record*/链cursor跨锁后继续解引用 |
| mailbox／ACK／遥测／控制查询 | ticket、完整key、slot id、record epoch、统计值等**值** | 后续必须正确锁内relookup＋验证。旧值不是读取旧slot内存的许可；过期报STALE，不写复用记录。要一致全表snapshot须另付锁时长／复制空间，不能假称分段扫描天然原子 |
| unmap/retire worker、排队回调 | 显式operation/worker责任及其稳定参数 | 唯一操作指针不属于任意reader，但其存活必阻止该record reuse；需要异步责任时仍占K。lock版不许可“删去RCU但保旧裸callback” |
| pool attachment／ctx／config registry／NAPI获取者 | 由已有合法anchor/受保护登记取得的引用 | 必须先有合法ctx/table寿命才可拿其中L。固定bucket/cell/table与lock本体的销毁仍要关入口并排其真实访问者；不能拿record不等grace推ctx也不必同步 |

直接原文还显示：`helpers.h:227–238`定义pp_ref对应page_pool-aware未归还责任；`:250–285`最后引用可恢复为1。因此“pp_ref==1”既不是独立idle判断，也不是控制指针寿命证明。原frame借用只在真实frame仍有效、全部fragment/bias以及退出合同都闭合的范围成立。

## 5. L-S可行首版及复用偏序

### 5.1 首版一个pair锁

固定ctx内预留K个record及其嵌入节点、bucket表、free list和operation状态。key由当前source ctx epoch及真实page身份/PFN组成，B/config在ctx内固定；控制ticket另含record实例代际。相同ctx中PFN离池再加入时也须先完成旧source真正release的unmap/erase，不能仅靠PFN相等继承旧图。

L内只做有界字段／链操作、唯一责任转移与复制；map/unmap、等待、NAPI disable、DMA drain、可能释放对象的终结动作在锁外。K上限给出了扫描工作数量上限，**没有把O(K)扫描称为低延迟短临界区**；关闭／状态查询应分次锁内取得一个有唯一责任的项，或按明确费用使用快照。首版实际锁类、IRQ/BH模式、锁序与PREEMPT_RT等运行条件仍须实现核验。

普通hit取得LIVE并复制地址后，允许锁外sync／prepare／SQ提交，是因为当前原frame使source最后release和cache-only owner隔离都不可达；target关闭另外先关gate并排已进入CPU发布者、真实DMA，再撤图。**锁只保护CPU记录访问，不能独自保护解锁后的DMA。**

CREATING时只有creator可完成map；并发close只标取消或接收明确交接。creator在L外得到map结果后，L内若仍允许则publish LIVE，否则自己或明确接管者在L外唯一unmap。CLOSING期间其他查询只复制状态／报busy，不持裸指针等待；既定fallback／失败责任不能被本轮扩成未核验的同page重叠另map。

### 5.2 冷页的record复用偏序

1. source owner合法服务，实际摘下合格private-cache页p；原frame／全部fragment及bias责任确已结束，不再有合法旧B DMA使用；同owner分配不能同时取p。
2. L内找到当前key／实例，将LIVE置CLOSING并指定唯一closer。记录继续可枚举；其他控制者只见状态或转交既定责任。
3. L外执行同backend合法B unmap及必要同步；不在当前RCU读侧等待覆盖自己的grace。
4. L内断去所有可达索引，旧链遍历／状态读者已在锁序中结束；closer仍独占旧record，故此时不能让别的creator抢写它。把后续ACK／页重发布所需字段复制为值。
5. 完成旧record还承担的operation/worker/callback和必要config职责。旧record不再用于任何page解引用、DMA提交或终结段；p可按原源所有权重发布。若backend要求worker或source finalizer尚未完成，本步骤就尚未结束。
6. L内确认旧实例已没有可访问它的责任，最后清理并返free list/K；解锁后本closer也不再访问旧record字段。后续ACK只用复制ticket/结果，不回写旧实例。**此时RETIRED成立，可以与UNMAP_DONE处于同一次服务，不需另等逐record RCU。**

source真正退出和target真正销毁各自仍须满足原有更强关口；这不是允许owner对任意在途页作无token独立eviction。ctx/config/NAPI/table整体退休不混进这个“逐record可返K”的结论。

### 5.3 多bucket优化的安全边界

如果改为每bucket锁，reader仍须持该bucket锁遍历到结束。control ticket只含`(完整key,source epoch,slot id,record epoch)`值；再次访问时，**按完整key计算bucket，在该锁内从链头重新找现存项，找到后才比较slot/epoch**。未找到或代际不同即STALE；不把slot id变成数组指针后在旧bucket锁下读其新字段。否则record已在另一bucket复用，旧锁不保护它，generation比较本身也是竞态。

跨锁record指针只授予唯一creator/closer，且其operation责任阻止reuse。free-reserve锁与bucket锁不嵌套：先独占reserve，再进bucket复核，竞争失败退出bucket后返reserve；关闭从bucket摘下后仍由唯一closer拥有，全部旧责任结束才单独返reserve。target扫描按bucket逐项claim，不保存未claim链cursor。这个版本增加锁序／预留／失败交接费用，尚无实现证明；root可选择首版pair锁而不靠该优化维持可行性。

## 6. V-C：固定值cell能否进一步免hit锁

本节仅给root提出的最邻近普通表示一个有界接口，不扩大为新系统设计。H个cell内存固定到ctx整体销毁；存数值key、记录代际、EMPTY/TOMBSTONE/CREATING/LIVE/CLOSING状态及dma_base等值，不存reader要沿用的page*/record*/next。K credit独立限制可接纳关联及未结operation，H/装载留量另计metadata。固定B/config通过外层合法配置寿命取得。

writer仍拿同pair L；只在短值更新段置版本odd，结束后even；**不能把map/unmap、等待、排队worker或DMA drain放在odd段中**。reader对完整hash探测进行一次版本快照，精确比较当前source/key与LIVE后复制dma_base；版本changed/odd时不接受命中。root核到`read_seqcount_begin`／`raw_read_seqcount_begin`会等odd；名字相近的`raw_seqcount_begin`则不等待，会mask LSB使对应retry失败。本首版选择`raw_read_seqcount`＋显式odd判定后转L慢路，再以配套retry和正确值拷贝／内存序接受稳定快照。root保存的同commit header本地`:267–348,368–388`支持这一区别；本文不据此声称exact READ_ONCE／内存序／LKMM实现已验证或已编译。

**odd／冲突→L内正常完整relookup**，不是无条件O0。锁内若LIVE，复制已存在的映射；只有确认ABSENT／接纳被预算拒绝且既有合法O0合同适用时才沿原fallback。CREATING/CLOSING遵守共同的唯一操作／失败交接，不能因为快照失败又对同page另建未知合法性的重叠DMA图。故稳定hit可以免锁，冲突慢路仍等L；没有无等待／有界墙钟保证。

删除保留TOMBSTONE，不能写EMPTY使后续碰撞key被漏掉；插入须先完成同key查找才能复用先遇到的墓碑，writer串行防重复。table全为非EMPTY时miss最多探测H个cell后结束；墓碑可被重用但最坏lookup仍为H。不假定便宜load factor、不隐去查满表／删除／墓碑费用；首版不搬动仍被creator/closer独占的cell，也不免费增加重建器。

稳定快照只给**一致的值拷贝**。锁外map寿命仍靠原frame／owner隔离及target gate/drain；无frame控制查询不能拿dma_base提交DMA。cell reuse前仍要旧map/unmap和operation责任完成；固定cell地址仅免逐cell内存释放，不等于允许复用旧map。版本计数的回绕及卡住reader必须有明确假设，或停用快路后整体quiesce／换ctx才能重新启用；不能把未量测的时间界当无ABA证明。writer可能被同CPU会拿L的reader中断的模式也须排除／正确屏蔽，禁止靠odd自旋或慢路拿锁制造自死锁。

**接受边界：** V-C若通过真实读者、内存序、writer context、回绕和完整lookup核验，可能替代L-S成为更强默认表示。因此本轮不能把“短锁每hit费用”当候选必付下界；两者的工程成本差仍需实际实现／测量。固定表、seqcount和锁选择本身都不是新算法。

## 7. 一个决定性条件例及最邻近反例

以下为来源绑定的手工偏序，未执行模型／测试。所有方案固定相同外部工作、路由、consumer和有效完成／服务合同，分别导出实际分配／回收；不强套同一PFN未来轨迹。

**P-REP：** K已用满，其中p仍有B关联且确实留在source private cache。合法owner请求已服务并实际隔离p；p没有任何合法在途frame/fragment/DMA，unmap及必要同步可在选定上下文完成。还有一个unlink以前已开始的真实RCU读侧区间r未结束；它可以是原有较宽的poll读侧，不要求它持有p的frame。它会使R-S嵌入node的退休回调尚未完成。L-S相同外层RCU区间并不保留该record指针，所有本record读取已按L结束，唯一closer也已结束。

在r真正结束以前，如果出现一个可合法接纳且对当前可用K有需求的后续B工作q，则：R-S的这个K槽仍被在退node占用，q按既有合同走O0／其他已允许路径；L-S已返该K槽，q可被普通同信息规则接纳。**判别结果是“相同关联预算下更早可用的K及更少被CPU退休占住的库存”，不是已经更低CPU／RAM／延迟。** V-C若上述接口被接受，也可取得这个结果；同接口R/O3/slot复制L-S/V-C同样通过。该条件例不要求p自然return、source退出或B全局停机，也不声称r/后续q在真实工作负载中的频率已知。

**最近价值反例：** 只把“q到达前grace仍未完成”改为“grace已完成，或期间根本没有需槽的q”，则R-S也及时返K；L-S仍为每hit付L。若同pair并发／写cacheline传递较重或多数hit热而回收稀少，新增锁费可能高于少付的callback/fallback；这直接限制L-S在该部署的CPU价值，不否定owner回收能力。

**最近安全反例：** 只允许一个telemetry/target枚举者把record*或next留到解锁后，或旧callback稍后写slot，步骤6即不能返K。必须重新加受控引用／RCU退休或改为只复制值；页frame仍在、record arena地址固定、slot generation校验都不能自动修复。若未结束creator/worker或target真实DMA仍在，也同样不能返槽。

**V-C相邻反例：** 写者更新频繁、墓碑／碰撞导致长探测、快照反复冲突，则常转L慢路并多付版本／探测，可能劣于简单L-S；若writer odd段能被同CPU慢路reader打断则有自死锁风险，不能接受这个配置。若为稀疏page_pool预留U全PFN值表才得到直读，还须计其完整空间，不能用D-D/V-C名称掩盖新预算。

## 8. 完整成本交换

| 资源／完成 | 必须保留的共同账 | 表示改变的真实差项与限制 |
| --- | --- | --- |
| CPU正常hit | key／地址读取、规定sync、原frame/SQ处理、合法target配置／gate | R-S的链遍历/RCU出版纪律；L-S每hit L及争用；D-D直接entry/cookie来源读取；V-C版本/探测、冲突后完整锁内relookup。固定B可以简key，不能省掉读取本身 |
| CPU miss／回收 | K预留、unique creator/closer、map/unmap、失败清理、owner请求/调度/ACK、扫描／查找 | R-S callback/grace推进与容量回退；L-S reserve/lookup/close锁序；D-D覆盖初始化/传递更新；V-C墓碑/查满/版本更新。冷项误驱逐及未来重建均计 |
| CPU关闭 | 真实CPU publisher排空、DMA stop/drain、source终结段、ctx/config/NAPI/table生命周期 | L-S/V-C只免逐record reader grace，不免整体关闭同步；backend worker／队列服务、op引用仍计 |
| 关联库存 | K含CREATING/LIVE/CLOSING及尚有责任的退记录 | L-S/V-C可缩短CPU-reader部分的占槽时间；不保证DMA或worker服务。固定预分配K的RAM字节**不会因为空槽增多自动减少**，只是可用容量增加 |
| metadata RAM | ctx/mailbox、K record/operation、索引、必要worker/Q资源 | R-S嵌入node与在退callback；L-S锁/bucket/cacheline布局；D-D按U或合法cookie字段覆盖；V-C H值cell与墓碑。只有另改实际分配/预算、同服务验证后，才能称metadata RAM减少；无编译字节数据 |
| payload物理RAM | 实际`S∪P∪C∪Q_payload`按页去重；源本来持有与B实际延后分开，源/替代分配反馈各自导出 | 冷回收p仍属于源S；若R-S旧reader只读record，p在unmap后也可重发布，不应人为pin到grace。故免grace不自动减少payload Q，更不减少S。backend要求留页时L-S/V-C也留；不新增复制池C |
| PTE／IOVA | 普通DMA API整实际granule的页表支撑、IOVA库存／失效成本 | 索引dense并不等于大leaf或仿射窗口；提前接纳q可能多保留其PTE/IOVA、少做O0 map/unmap，均按真实时序记 |
| 带宽／cache | 真实payload copy/设备流量与metadata cache流量分开 | 不凭少callback推DMA带宽收益；共享锁cacheline和H/U表访问可能增加CPU内存流量 |
| 有效完成／延迟 | O0仍可完成相同有效工作；接纳、拒绝、owner服务、unmap、retired分别报告 | K更早可用不等吞吐／tail latency必优；L-S/V-C慢路L等待、owner控制占时、后续allocator反馈可改变完成时间；全部原生量null |

若要主张L-S相对R-S的CPU收益，需在各自真实序列中证明，**少付的callback／容量fallback／相关重建费用，大于新增的每hit锁／争用／临界区及其余变化费用**；V-C还须计版本／探测和冲突慢路。这里不给任意时间、阈值或“省几次映射就回本”的旧账。CPU、metadata RAM、payload RAM、PTE/IOVA、带宽和有效完成各自下结论，不能未经价值取舍把它们加成一个净收益分数。

## 9. 本轮能／不能改变的研究判断

能改变：把“逐record RCU退休会压住K”从必然成本改为**R-S嵌入链node的一种实现成本**；给出L-S及更强V-C可消掉该等待的具体条件，并把每hit同步、固定表空间／探测、控制再查和全局生命周期费用摆到同一张账上。这是触发本轮的具体表示／成本差项，不是继续拆上一轮相同mailbox协议。

不能改变：既有持续授权T仍为部署条件，S另列；owner必须实际隔离合格cache页；source最后引用不代替target配置寿命或设备DMA完成；K不足O0不是应用死锁；取消独立TX token／B-only撤图／同信息策略并未因此变独占新机制。前轮明确已允许锁／等价保护和直表，本轮没有新的closest文献检索，不能给“首次”或未查全文的novelty宣称。

可核查的贡献表达是：前轮已有持久关联、owner cache-only隔离和普通锁/RCU/直表选项；剩余新增认识是**在保留普通page_pool与原frame责任时，真实record reader集合允许把索引回收从全局grace转为锁内完成，或以固定值表避开可复用链节点，从而用不同热路径／空间成本交换有限K周转**。这个条件能力由本文偏序支持，实际净收益、最佳表示和部署频率未验证。若后续在同有效工作和服务下，具名实现相对stock／最接近合理适配取得可归属CPU或不可回收内存改善，且费用／适用边界完整，就可以论证相应整合／部署贡献；同接口普通R/O3能采用它不单独否定该工程价值。普通锁／RCU／seqcount选择本身和本轮文字合同不能充当独立新算法或论文主线证据。

交付`representation-card.json`与`evidence-manifest.json`。未改driver／RTL／ABI／collector、canonical、Git；未运行状态／性能模型、native、测试、构建、旧实验或板测；未打开新网络／全文文献／数据入口扫描。source席／root仍须对真实新增访问点、seqcount准确语义与target关闭原始证据作接受裁决。
