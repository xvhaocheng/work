# OWNER-BINDING-INTEGRATION-DELTA-1：V1与普通RFC式基础页适配

2026-10-16 JST。独占范围：source owner、基础页映射关联、源释放与其共同原语；target关闭原始驱动核查由root负责。本文件是可执行设计合同与来源核查，**没有实现、运行模型、内核/设备测量或完整并发证明**。请求配置 `gpt-6.1-sol/xhigh`；本席未取得provider运行元数据，实际model/effort均为`unknown`。Codex-only；使用ARS-Codex已定有界机制与定向源码核查纪律，未启动pipeline、网络或其他模型。

## 1. 可交回root的结论

**最强普通适配可以覆盖目前V1的这组能力。** 具名对照 **R-OB4K-T** 是“DMA_PMD owner-bound位图思想的普通order0、规定权限、现有DMA API适配”，是本轮设计的普通构造，**不是已发表/已实现的同名项目，也不是未经改造的RFC**。它和V1同样增加真实source-release联动、仅已接纳页的稀疏关联记录、每target映射状态、非睡眠首miss、正常frame完成及真实target drain。给两者相同字段布局和执行位置后，本席没有找到V1必须少做、而R-OB4K-T必须多做的源侧原语；这不是完整成本已测相同的结论。基础页/权限适配的工作量不能自动成为新意。

**固定小B集合、无独立idle eviction的窄版V1-N及R-OB4K-T-N都可以取消每次TX的独立binding token。** 原frame/全部page_pool片段引用持续覆盖准备期及在途DMA；映射只在真实source退出或target已禁新、CPU访问排空、设备DMA真实完成/合法排空后撤销。这会去掉候选通用版的token RMW、CQ token_put及完成FIFO中的binding handle需要，不能只给V1。固定B可去掉一般复合key哈希和逐包target代际比较，**不能同时宣称完全不用读取映射地址/就绪状态**：普通DMA API返回的IOVA仍需保存在源页侧记录并读取。用PFN仿射窗口代替地址表是另一种有代价的后端，不能免费继承RFC窗口。

取消token的条件证明只涉及映射的DMA使用寿命。它不消除首次建图并发、CPU metadata出版/回收、控制面target配置寿命、非睡眠unmap条件或退役Q。root仍须接受对应target关闭/出版合同。

## 2. 证据边界与可导航原文

下文 `PP`=`out/mapping-admission-20261016/v612-source/raw/net/core/page_pool.c`；`H`=`…/include/net/page_pool/helpers.h`；`T`=`…/include/net/page_pool/types.h`；`XD`/`XH`=`…/drivers/net/ethernet/mellanox/mlx5/core/en/{xdp.c,xdp.h}`；`RX`=`…/mlx5/core/en_rx.c`；`XC`=`…/net/core/xdp.c`。均为已存官方Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`。`L02`/`D07`/`M08`=`out/mapping-organization-source-20261016/dma-pmd/{lifecycle02,domain07,mapping08}.txt`，行号为保存邮件diff的行号，并非已应用源文件行号。完整绝对路径、hash、范围及旧manifest见[source-manifest.json](source-manifest.json)。

| 原始证据 | 支持的实际事实 | 不支持什么 |
| --- | --- | --- |
| PP:626–645；T:89–91 | join设置pp身份并初始化片段计数；现有init callback注释为private/test-only | 不是现成release-notifier；不能据此说已接入V1 |
| PP:673–698 | 真正离pool会卸A图、增加release_cnt、clear_pp_info及put_page；release计数后不能再用pool | 普通路径没有B关联关闭协议；只延迟put而提前release_cnt不保pool寿命 |
| PP:736–790；818–829；857–902 | 普通page_ref=1允许recycle；最后pp引用才进入bulk回收；ring满可直接真正退出 | 不保证某页下次一定被重新分配；空闲alias不天然增加payload |
| H:227–238；245–308；311–323；RX:274–302 | pp_ref表示page_pool-aware未归还用户；driver用bias批量设置/归还；最后引用测试可把字段恢复1 | 单buffer不等于一页一包；不能拿单帧CQ认定整页已idle；不强制计数字段物理值为0 |
| XD:877–922；925–950；673–700；XH:75–102 | target当前map在SQ接受前；成功单buffer FRAME FIFO为mode/xdpf/dma_addr；完成先unmap后return frame | 不证明新增mode已编译，也不证明FIFO减少一个逻辑entry必定省结构体/缓存行字节 |
| XD:819–840；XH:138–150 | 已有残余descriptor回收与网络reader同步入口 | synchronize_net/RCU不是设备DMA完成证书；FW stop/drain含义由root另核 |
| 已存`include/linux/dma-mapping.h`:380–396；XD:683–684；PP:659–665 | `dma_map_single_attrs`转为map_page_attrs，`dma_unmap_single_attrs`直接转为unmap_page_attrs；已有B CQ和source return都使用此前端 | 支持同一有效backend、对齐实际granule且无新增等待竞争时的同步分支；不证明所有backend/设备、时延或并发关闭都无需Q |
| PP:906–934；990–1056；1141–1150 | 片段drain、ring/cache清空、NUMA迁移也走真正return | 不能只在pool最终disconnect时卸B；释放关口应统一覆盖所有callsite |
| L02:143–215；348–449；738–743；D07:351–378 | RFC已有最后block free、全idle退役、worker卸各域图、RCU后buddy释放；07补入真实unmap_all | 不可把02占位当已unmap；RCU只保护CPU读者；原机制绑定PMD allocator |
| L02:492–563；D07:149–176；202–233；269–346 | RFC有per-PMD位图/map_lock、domain registry、SRCU防域释放及索引复用 | 位图本身不是拒新map门；D07:211先清bit、212释放锁、231后unmap，单独搬函数会有重建竞争 |
| M08:290–410；83–91 | RFC用物理地址仿射IOVA、整PMD `CACHE|READ|WRITE`、仅coherent设备；首次图用GFP_ATOMIC；per-buffer unmap为no-op | 不能免费推order0 READ-only/非coherent支持；不能把源码复杂度/周期注释作为本轮测量 |

## 3. 冻结共同合同与最小表示

只比较当前普通page-backed、order0、单buffer XDP链。包可占页的一部分；同页的其他片段、driver未耗尽bias都保留。T是条件：B在完整实际映射粒度及source generation内后续合法用途拥有规定权限；每次实际CPU/A/B同步照做。S逐对象撤权分支不适用跨对象持续图，不能凭空给B额外权限。改变target DMA domain、mask、attrs或权限必须走相应关闭后重新配置，不能默认为原图可继续用。

**共同表示（设计投影）**：

- `pool_ctx`：源pool关联、控制面owner epoch、page-record索引、有限record预算、退出记录数及独立控制面kref；固定target配置表含设备/domain配置寿命和每槽规定方向/attrs。source关联、record/worker控制引用分别退还；ctx可晚于原pool释放，但那时不得再解引用旧pool。不能只持pool user_cnt便当每页都安全。
- **最强主对照采用lazy sparse**：source join无需每页wrapper。pool创建时装统一release hook与pool_ctx；首次B使用才从K个预留record取得一个，记录`pool cookie/page/target config/direction`及规定attrs。正常无关联页source release查索引为空即继续原return。K满则本次走O0，未来容量释放后仍可再尝试接纳；不把“某次未预建record”写成永久禁止接纳。
- 关联record兼任本页已缓存关联的owner记录：pool_ctx、PFN、source epoch/唯一record实例、`OWNED/EXITING/DEAD`、短状态锁、`slot[B]`（固定A→B可就是一槽）、retire-node/worker状态、控制面引用。slot最少有`ABSENT/CREATING/LIVE/CLOSING`和`dma_base`；通用可evict版另有active token数。首次创建时frame仍持真实pp-ref，因此普通source-release最后片段门不能并发越过；target关闭竞争仍需单独握手。
- 稀疏索引可用K个预分配record的固定bucket哈希，source/target链节点和Q节点嵌入record，避免NAPI首次接纳另外分配节点；变更用bucket/record spinlock，CPU reader用RCU或等价保护。可选pool私有xarray/PFN side table也须预留其节点或处理非阻塞插入失败。若要密集PFN直索引，需为被覆盖的物理范围另付metadata内存；不把RFC稀疏per-PMD数组的常数占用迁移为per-4K零成本。不得占用已有A的page_pool dma_addr/pp_magic/pp_ref字段。
- 每个已建图slot至少保存一次backend返回的DMA base及状态。固定B和统一权限可把target/rights/generation从每slot key提升到pool配置；指向page_owner的lookup仍要发生。没有跨关闭存活的外部handle时，source generation可由record唯一实例加延迟回收保证，无须每包读一个大复合key。control worker必须持真实metadata引用或在全局互斥范围内访问。
- pool hook/ctx在页向driver公开前就位；lazy record在首次map时先以CREATING同时进入source索引及target可枚举集合，完成map和复核后才出版LIVE。只有原frame仍持有效page_pool片段责任的reader可借用，不增加idle page/pp引用。erase先于source最终put；CPU读者回收另遵守RCU或等价握手，不以“页还在”自动代替所有控制面reader寿命。若采用eager join-record，它只是双方都可选择的实现，不能强迫普通R每个源页都预建record。

R-OB4K-T使用**同一表示**。它复用RFC的owner→mapped-target集合、唯一卸图、domain关闭索引保护原语；不要求保留原PMD allocation、2M leaf、物理全区IOVA window、RW权限和全RAM元数据。O3-owner也同样得到此接口与表示，故其策略增量为0；该事实只限策略，架构比较如下逐项裁决。

## 4. 可执行事件表：V1-G与R-OB4K-T-G共同通用版

`G`允许独立cache eviction，因此保留每次binding token。以下函数名是拟议接口名，当前源码没有实现。每个事件同时给双方；不将一方规范写得更多就当其成本更高。

| 事件 | 字段、调用、分配/索引 | 锁、责任与失败闭合 |
| --- | --- | --- |
| source join | pool创建先登记pool cookie/hook/target配置及K-record预算；普通页仍由PP set_pp_info建立源身份，主对照不为每页建wrapper；可选eager模式双方同享 | pool控制面分配与新pool epoch计费；stock init_callback不是现成production release接口。pool对象销毁前hook/ctx随inflight责任存活；单纯user_cnt不代逐页release |
| 正常recycle | 不改owner/generation，不新建record，不给idle cache加get_page或pp_ref | 仍按PP:736–790与last-fragment判断；映射关联留存不阻止A回收。不能把每个packet变化当source换代 |
| target map miss | frame仍有效；稀疏查表，缺record才取K预留，锁内唯一插入CREATING并保持target/source都可发现；锁外同一DMA API整granule映射；复核owner OWNED及target仍open再出版LIVE；返回IOVA+当前offset并持token | K满或CREATING竞争者不睡等，走合法O0；未取到record不永久禁止以后接纳。有限预留、索引和失败清理都计费。close竞争只允许一个creator/closer接管已建图；失败不得丢frame。页引用不保护target配置，creator需要target读侧/控制面寿命 |
| target hit | 合法reader读record/slot；锁/CAS在LIVE下token_get；执行规定B sync与提交准备 | LIVE→CLOSING后不再给新token；不能把lookup、token、sync省为零。映射为整个granule但sync范围/隔离要求按实际backend冻结 |
| SQ accept | 成功前缀由target承担frame责任，FIFO记persistent-mode/xdpf/handle（可替原dma_addr逻辑槽） | 只在XD实际ret成功后接管；record与target配置保持至token结束；不要求第4槽，结构大小未编译 |
| map/SQ fail | 首map失败回O0或保持原未接受后缀；SQ拒绝则token_put；若当前是O0则照常卸短图 | 已建持久图可在LIVE缓存；private map未出版则由creator卸。原frame仍交未接受后缀原责任人，不提前return或双return |
| CQ/合法drain | persistent token_put先于原`xdp_return_frame_bulk`；O0仍unmap然后return | 源最后return可立即触发source close，token顺序防自等待。正常CQ与残余descriptor回收用同样mode分派；真实DMA完成由driver合同保证 |
| cache evict | 锁内LIVE→CLOSING拒新token；保留source索引可发现性；active=0后唯一closer实际unmap；完成后ABSENT | 不拿spinlock等CQ或做未知可睡backend调用。不能先erase使source退出漏图；evict不释放source page、不改source generation；并发evict/source/target close共用一次性关闭责任 |
| source真正release | 在PP:679之前的统一hook把owner置EXITING；枚举包括CLOSING的所有slot，禁止新token/map；清完所有图后才调用拆出的原return终结段 | 原终结段只执行一次：A unmap→release_cnt→clear_pp_info→put_page。正常frame合同应使此时无活token；非零是合同违例或需受控defer，不能在同NAPI等自己。release_cnt以后不得再访问pool |
| source pool close | 禁新alloc（现有PP:1028–1030要求）；cache/ring页经同一release hook；在途片段依原引用晚归还 | 不能pool destroy时一次unmap_all便释放仍在B DMA的页。记录与source pool保持到该页真正release计账；沿用PP inflight延迟destroy，不能先清账另存裸pool指针 |
| target close | target gate禁新，等待已进入creator/submit reader，真实停止/排空DMA，再唯一关闭该target所有source slot；最后退配置/索引 | 与source close竞争通过slot close状态与control引用串行；不持source spinlock等待target控制锁。target仅device引用不等于domain配置不会变，必须有实际配置寿命协议；root核完整驱动序列 |

**锁的最小规则**：control target/config互斥在外；page_owner锁只守短状态转移；不得拿page锁等待target gate、DMA完成或worker。target关闭先关gate、等map/submit CPU reader退出，随后才claim待关闭record并逐个锁page；source关闭先在page锁下关门并取得待关闭slot的控制引用，释放page锁后调用卸图。首map的CREATING记录始终可枚举。若source hook位于RCU/NAPI读侧，不能等待target已经claim但仍在等该读者的closer：这会形成`source等binding→target等RCU→source`环。选择先reader drain再claim，或source安全Q defer离开读侧；不能spin等。CPU出版/读者宽限、唯一closer及target配置寿命必须各自闭合；表不是完成的并发证明。

## 5. 更强低成本窄版：V1-N与R-OB4K-T-N

额外冻结：B集合及每槽rights/config在一个open epoch固定；不独立按LRU/压力撤某个LIVE关联；仅页真正退出或target关闭才unmap；budget在首次建图前接纳/拒绝，已有LIVE图不能靠未经排空的eviction临时挪空间。固定A→B、K个预留binding槽是最窄实例，lazy按需首建图；非eligible或容量不足本次用O0，不丢有效包，也不强制为所有source页预map/预建wrapper。

**源侧token消除证明（条件推导）**：

1. 每个进入B prepare的frame仍拥有page_pool片段责任；接受后保留到CQ或真实DMA drain，失败后由原未接受后缀归还。H:227–238定义其意义，PP:866–873在last-ref之前不进入实际return；RX:274–302的bias drain也必须包含在“全部引用”的条件中。
2. 一个仍活的B frame存在，就不能合法通过该page的最后片段门进入统一source-release hook；若页已进入cache/ring，也不同时有该页的合法未归还fragment。故source是真正release时，不会有准备/在途B DMA依赖它；无需额外binding.active在这个门重复表达已有frame责任。
3. source pool destroy不推翻步骤2：它只清闲cache/ring，在途片段沿PP inflight保住source对象直至真正归还。source主动revoke、权限撤销、memory migration等若允许提前撤map，就不属于此N合同，必须退回G或先实现等价drain。
4. 唯一另一个卸图点是target关闭。它必须先禁新并等所有已进入map/submit的CPU reader，再真实drain全部相关SQ/DMA，且处理全部CQs/残余frame责任后卸图。页面pp_ref不能证明target配置安全；这一步不能省。root若不能接受目标排空保证，则N版本部署条件未闭合，不能靠RCU补齐。

由此N版本无per-use token RMW、无CQ token_put、persistent完成无需查binding或保存其handle；可设计`mode+xdpf`两逻辑entry，或保留第三entry作为兼容/直接IOVA冗余。它仍需要每次map/hit时取得slot的有效DMA地址、完成per-use sync以及正常SQ/frame管理。三槽/两槽仅逻辑描述，**没有编译得到union字节/缓存行收益**。固定B不要求单页单frame，不论同页有几个fragment，按最后引用门才退出。

独立metadata hash lookup的消除需另付表示：例如source预分配PFN直索引side table可合并owner定位与slot读取；或者给frame加已知cookie传递。前者增加metadata覆盖空间，后者改frame表示与传递链。现有page_pool的DMA地址槽已经属于A，XDP mem.id是allocator id，不能据此称已有一个免费B指针。**消掉通用哈希不等于消掉地址读取，消掉token不等于免CPU metadata读侧保护。** 这些表示选项同样给R-OB4K-T-N。

### 最邻近的token反例

- 允许idle eviction但只读到`active=0`或只因某一frame CQ就撤图：第二个同页fragment仍在另一B SQ DMA，或者prepare尚未入SQ；pp_ref不会替独立evict阻止unmap。需G token、已有同等active语义或显式排空。
- 固定B但开放期间换domain/rights且未drain：source页仍在，所以pp_ref保护物理页，却不能保旧target图。必须关闭旧epoch再更新。
- 持久模式先return frame、随后继续读binding或对DMA作尾处理：最后frame可能触发源卸图/erase，产生UAF。N完成路径不得return后再用该record；G需token先放再归frame。
- 仅取消token同时遗漏join/真正release hook：源页离pool、被新owner复用后B alias仍在；generation校验不能撤掉硬件旧权限。

## 6. 同步退出与Q：双方相同的必要费用

**同步分支**：只有选定backend的DMA unmap/必要失效同步在真实return上下文允许且服务成本可接受，release hook才可原地逐slot撤图，再调用原终结段。N由于last-fragment门已无该页合法DMA，不需要等待该页CQ；但unmap/IOTLB/backend调用本身仍可能不适合softirq，不能从GFP_ATOMIC首map反推同步unmap可用。RFC D07:370–372把其unmap慢路径放process context，故不能把RFC worker换成软中断同步并宣称已经可用。

同步分支已有更直接的条件依据：已存v6.12 `include/linux/dma-mapping.h:380–396`中single前端转page前端；B CQ已有`dma_unmap_single`（XD:683–684），source真正return已有`dma_unmap_page_attrs`（PP:659–665）。因此**同一有效DMA backend、实际basepage granule对齐、没有新增等待锁或关闭竞争**时，source hook同步撤B图不是全新的API上下文要求，可以作为双方的条件可执行分支。该依据不证明所有backend、硬件失效完成时延或target关闭竞争，Q也不能据此全部删去。

**Q分支**：hook标EXITING并摘出可分配索引，不执行PP release_cnt/clear_pp_info/put_page；把page_owner内预留retire-node排给worker。source页真实所有权和inflight仍在；A map也可保留至原终结段。worker拥有页退出责任与target配置引用，完成每个slot唯一卸图及所需同步，擦除索引、按规定读者宽限回收metadata，最后执行一次原return终结段。不能只延后普通put而先计release_cnt，也不能递归再次进同一hook。

ctx/record控制引用必须与原pool内存分开：最终return前保存终结段所需参数，并保持当前record/worker控制引用；执行release_cnt之后不再访问原pool。终结段返回后只可操作独立ctx/record的安全引用，完成record回收及ctx控制引用退还。记录提前从索引摘下不等于可在当前终结段仍要使用其字段时复用record；CPU grace也不替代该worker自身引用。

Q节点、worker可运行资源及target配置引用应在首次持久图接纳前预留。若最多接纳N个source-page记录，最坏同时退出需要覆盖N页而非凭一个“小Q队列”保证无限已建图可退。可限制首次映射的记录数并用O0 fallback；已经产生的撤图义务不能因Q满而强行free。已有mapping解除后记录预算才返还。Q页数可以被接纳上限约束，**排空时间没有driver/backend有界服务事实就仍是未知**。异步Q是L02:143–215已有模式的普通基础页投影，双方同价计入，不是新增能力本身。

## 7. RFC迁移需要支付的具体差项

| 原RFC条件/能力 | R-OB4K-T需做的真实适配 | V1对应义务/裁决 |
| --- | --- | --- |
| PMD allocator持整个extent，最后block free钩子决定全idle | source身份改为page_pool的实际join/每页return；不能等普通buddy最后put才认source授权结束；引入page-record与hook/Q | V1同样缺该hook/record/Q；不是R独有附加费用 |
| per-PMD稀疏数组、free/dirty bitmap及pool三链表 | order0沿stock page_pool库存；不引入PMD分配/清零/碎片；每page元数据按真实数量计 | V1不能再用“R需要2M”制造物理胜例；两者page_pool原库存相同条件起点 |
| `iova=domain_base+phys`及大物理窗口预留 | 主对照改用现有DMA API的按页IOVA，slot存dma_base与状态，按页/target unmap；要仿射则另计窗口范围、mask/aperture和4K PTE开销 | V1也用同slot读取和IOVA分配；R不保留“bit+add且无需IOVA树”免费快路径 |
| 一bit/domain隐含PMD整范围RW/coherent | 固定rights class/target config；整页规定方向/attrs，后端实际能执行的权限及coherency/sync限制同V1；不同rights不能共用一个bit | V1不能仅因有rights字段认新增能力，也不能把DMA_TO_DEVICE名称当未经核实的所有backend硬件READ-only证明 |
| domain registry/SRCU保护unmapper与domain teardown | 保留配置life/draining/无旧索引复用；source/target close串行且唯一unmapper | 双方同需；SRCU不能代DMA drain。可固定表代一般registry，优化同样给双方 |
| 全idle PMD已摘allocator，无合法map caller | 基础页EXITING门在clear/put之前，记录仍可枚举；首map出版与close握手 | 双方同需；单搬unmap_all清bit再卸图不闭合 |
| per-buffer unmap no-op仍调用DMA API入口 | N可在driver persistent mode直接归frame，不再CQ走无意义unmap；G记录handle并token_put | 普通R也可用相同FIFO mode，不强留其额外CQ调用作为人为成本 |

这些是可复核的适配清单；不存在“既然RFC有难以直接搬来的部分，V1就已经更便宜”的推论。双方完全可采用同一个普通适配实现。若后续出现具体表示/后端/部署差异，需具名修改一方并带真实费用重新比较，不能按方案名称打分。

## 8. 完整资源账与决定性预测

按外部到包、路由、consumer及有效完成/服务合同固定输入；策略各自导出真实source取页/回收，**不冻结同一PFN再用轨迹**。本轮无实测CPU/带宽/延迟数值。

- CPU：join/index插入、record预留、每次owner/slot读取及sync、首次map/失败清理、SQ/frame、G token、独立evict、source/target关闭遍历、worker/RCU等分别记。N消token是双方的共同优化，不能直接折算周期或吞吐。
- Metadata：pool/target表、每页record、每slot地址/状态、索引节点及覆盖范围、Q节点/worker资源和CPU读侧回收分别计。未编译则结构字节为unknown，不沿用RFC每PMD的256B等常数为每4K结果。
- 页表支撑内存与leaf数量单独记；order0普通图不继承PMD leaf覆盖效果。IOVA为另一个虚拟空间预算，按页分配与仿射窗口全范围预留不混算为RAM。
- 物理payload按实际`Sπ ∪ Pπ ∪ Cπ ∪ Qπ`去重：S源拥有；P脱源而由B/cache保留；C复制目标；Q退役隔离。无idle pin的候选/普通R均可P=0，但Q不必0；S与Q重叠仍只算一页。源pool本来持有与B延后退役/引发替代页分配的因果归因分开。copy早return不等于source物理释放；本文主设计无copy新增，但O0/copy fallback若使用必须计C与copy带宽。
- 完成结果、拒绝/失败、排队/Q等待与服务合同另报；容量不足回O0的mapping CPU不能藏掉。metadata、PTE内存也不可混入payload集合后又重复加。

**决定性预测P1（尚未执行）**：在合法T、固定B/rights、无独立eviction、所有frame/fragment持至真实CQ/drain、选定backend退出闭合的条件下，对实际再次分配到同页同source generation的两次B转发，V1-N可让B图跨正常recycle留存，idle阶段不增加page/pp引用，TX不增加binding token，最终source退出先撤B图再release，完成记录不需第四逻辑entry。**R-OB4K-T-N凭同样接口、同一表示和原语可覆盖P1**；因此P1即使成功，也只是接口/部署可行性预测，不给出V1独有机制或低成本证据。

**相邻失败F1**：只增加“任意idle map压力淘汰”而保持上述无token实现，同页另一个fragment仍prepare/DMA时即可卸图；P1不再成立。双方都须加token/等价在途机制或先drain；若重新加G功能后的完整费用大于省下map费用，只限制这个版本。另一个邻近失败是同步unmap不允许且Q资源/服务不满足合同，此时窄版需限定后端/接纳或重新设计，不能先物理free。

**接受/否决理由**：接受V1核心思想的强低成本N版及其条件合同；接受普通R以相同能力/字段实现的强对照；否决“per-TX token对所有V1不可缺”“固定B就零metadata读取”“普通R必须永久pin/2M/RW/额外FIFO槽”的比较前提。源侧独占整合差项集合目前为空。没有据此关闭整个思想族，也未以O3等价一条证据否定接口架构；只是这一具体普通基础页构造已覆盖该组能力，现有证据不支持额外的新机制或更低成本主张。

## 9. 交接与未解项

交付[primitive-table.json](primitive-table.json)逐事件/差项给出结构化接口，及[source-manifest.json](source-manifest.json)原文hash/范围。root需合并的协议未解项是：实际target gate/CPU reader/drain/config寿命合同是否可接受；指定backend的真实softirq退出或Q成本；若要主张更少元数据读取需固定一种真实side table/传递表示并与普通R同等计费。它们是设计接受条件，**不建议为这些未知再开一轮相同字段表或无限协议审计**。

**仅一项可能改变研究价值判断的后继建议（不自动执行）**：先冻结一份**真实O0 mapping费用与源再用/退出的机会证据合同**。输入应来自一个具名、同服务要求的工作负载/后端，包含真实source页/epoch的join、实际alloc/recycle/exit、B映射及接受/CQ/target-close事件，和该O0 backend的map/unmap实际CPU费用；已有可用原始证据优先，缺失就标明缺失，不制造固定PFN轨迹。若能识别同source generation中真实重复B图及可归属的CPU费用，且有限K、退出/Q频率留下有意义的资源余量，就推进普通整合的价值评估，并只针对还会改变净收益判断的未知选有界验证；**这不直接证明V1独有或论文新意**。若实际页大多一次B使用就真正退出、可省mapping费用很小，或接纳/退役使服务需求失守，就限制该具名部署/版本的收益假设。单纯缺数据不启动驱动、采集系统、模型或板测；root决定是否及如何取得输入。

本席未改canonical状态、计划、HANDOFF或任何源码。
