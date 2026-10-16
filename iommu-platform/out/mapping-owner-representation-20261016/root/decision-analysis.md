# 映射关联表示裁决：值快照可以替代逐记录宽限期，但不是免费缓存

2026-10-16 JST。**本轮最值得保留的具体版本，是固定存储、只返回值的稀疏表：写者串行更新，命中读者验证一个完整快照，随后只借原frame寿命使用复制出的DMA地址。** 稳定命中可不取索引锁；正常cache-owner回收也不必仅为新增record读者等一次RCU grace。发生版本冲突则进入短锁慢路重新查找，不宣称wait-free。

这是源码支持的**条件表示设计**，没有实现、并发模型或性能数据。它比“每hit必须加锁才能免RCU”的版本更强，但不是新的seqcount算法；普通R/O3可以完整采用。**本轮推进的是表示与成本认识：逐record RCU等待、每hit共享锁、按全PFN范围建密表都不是该整合思想的必付下界。** 未证独占机制、完整CPU优势或论文主贡献；低成本整合假设保留。

## 1. 具体问题和本轮新输入

保留前轮已选范围：v6.12 mlx5e cyclic／linear、order0、page_pool、单buffer非XSK，固定B/config，private alloc.cache-only owner回收。T为专用池完整映射范围持续授权的条件部署，S分开。当前帧在miss时走共同合法回退，不等待回收；不改变上一轮发布／关闭协议，不增加独立逐TX／逐alloc use token。

原版表示允许hash、短锁、RCU或等价保护，但未固定所有reader。若把“必要metadata退休”直接落实为每个record都等一次全局RCU，有限关联库存可能被已完成unmap的旧metadata占用；若改成全锁读，则每hit付锁与争用。本轮由这项**具体实现差项**触发，不重搜数据、不重复NAPI发布协议或累计阈值账。

新核两份官方固定commit原文：`include/linux/seqlock.h`、`Documentation/locking/seqlock.rst`。其他frame、page_pool、XDP和前作比较均复用已保存证据，固定commit为`adc218676eef25575469234709c2d87185ca223a`。全部定位用本地原始字节行号，web解析器行号不作本地锚点。[frame来源核查](../frame-source/findings.md)／[表示比较](../representation/findings.md)／[seq来源收据](seq-source/http-receipts.json)。本轮没有新文献覆盖性检索或新原生运行事实。

## 2. 四种具体表示及选中版本

| 表示 | 读取及回收 | 真实交换 |
| --- | --- | --- |
| RCU稀疏链表索引 | reader可跨节点走next；删节点后延迟复用，表/节点寿命靠RCU等机制 | 命中不取共享索引锁；延迟回收占metadata库存。允许批量callback、额外预留退休节点，不能强迫每节点一个callback或把映射配额与node库存混成一项 |
| 短锁稀疏索引 | 固定pair的irq-safe index/state锁保护全部遍历、值复制、free list和控制再查；frame读者解锁后不碰record | 条件性免逐record grace，但每hit有锁RMW／争用。采用单pair锁是首版易审查选择，不是最优锁划分或其他实现必付下界 |
| PFN密表／frame cookie | 算位置或读取已传cookie，再读地址与状态 | 密表按覆盖范围付存储，普通order0池不承诺物理连续。cookie须实际写入／传递并保寿命；现有`mem.id`不是免费B handle，不能占用A已有dma_addr等字段 |
| **固定位置、只读值的稀疏表** | H个固定cell，无可变next指针；一个表级seqcount覆盖完整lookup。稳定快照返回值；odd／冲突转同一writer锁内完整relookup | **本轮保留的最强版本。** 不要求每hit锁，也不靠逐record grace保cell内存；付H容量／探测／版本读和屏障、写侧失效及慢路、epoch/整体关闭成本 |

前轮[整合分析第3节](../../mapping-integration-delta-20261016/owner/integration-analysis.md)已经允许bucket/record锁加RCU或等价保护，第5节已经允许付费PFN直表或cookie。因此前三种不是本轮首次提出。第四种把“等价保护”具体化为标准值快照表，也不据此认领新的并发原语。

## 3. 原文支持的边界

### 3.1 frame保映射，值快照保metadata读取

已保存`en/xdp.c:877–940`在frame仍有效的prepare段建立DMA地址并向SQ提交；`:673–700`在unmap后归还frame，正常CQ与残余清理共用这条责任。原生FIFO保存frame和DMA地址值，**源码不强制新增B-record指针跨到CQ**。拟议持久mode可把record读取结束在prepare，CQ只按原责任归还frame；原生目前仍是O0，没有实现该mode。

page_pool的实际cache成员经最后pp引用和driver bias门；owner摘下成员并阻止再分配，才可撤B。`pp_ref==1`不是idle证书。源真实退出须先关B再做原release/clear/put，target真正销毁须另禁新DMA、CPU静默和实际DMA完成／stop。**一个seqcount快照只说明值彼此一致，不保证映射持续有效；合法frame及目标配置寿命是另一个必要前提。** 完整原文见[frame核查](../frame-source/findings.md)。

### 3.2 seqcount能支持什么

官方[固定v6.12文档](https://raw.githubusercontent.com/torvalds/linux/adc218676eef25575469234709c2d87185ca223a/Documentation/locking/seqlock.rst)本地14–50行要求复制完整数据、检查版本，并由外部机制串行写者和避免写者被相应reader上下文打断；不能靠这种版本检查保护已失效的跟随指针。

本地[seqlock.h](seq-source/raw/include/linux/seqlock.h)209–212行以acquire读取序号；267–300行的`read_seqcount_begin`／`raw_read_seqcount_begin`都会等待odd稳定，**名字里有raw也不等于不等待**。303–320行的`raw_read_seqcount`不替caller检查odd；368–388行的`read_seqcount_retry`带最终读取屏障；405–430行显示写侧odd／even及写屏障。本文选“读取raw序号、odd立即转锁慢路、复制值、正常retry检查”，不偷换为无限等待begin或无屏障retry。

这只核准可用原语与禁止事项，未实现精确READ_ONCE布局、字段宽度、锁序、版本回绕处理或LKMM证明。plain seqcount配外部irq-safe writer锁，写侧非抢占／合适IRQ约束必须落实；NMI等未覆盖caller不得加入。没有按API名称推导性能或所有模式正确。

## 4. 选中版本的最小表示与执行

### 4.1 永久位置、有限责任

每固定pair有H个cell，数组地址、H及寻址规则从attach保持到ctx整体销毁。cell只向快路径公开数值字段：完整source/config键、record代际、状态、DMA base及必要值。没有可跟随的record/page/next指针；不得从尚未验证的字段派生越界数组位置、解引用或提交DMA。输入寻址来自当前合法frame和固定表参数，仍需真实source身份及旧alias退出合同，版本号不能代替它。

写者用一个pair级irq-safe锁L串行索引、状态和配额。K继续约束该版已接纳／creating／closing及尚未结束operation责任，H另表实际表示容量。CREATING/LIVE/CLOSING占用自己的cell；唯一creator/closer可在锁外持operation责任，但其结束前cell和K信用不得复用。map/unmap、等待和callback不在odd版本区或L中执行；只有短字段更新进入odd→更新→even。

控制面只取值ticket，后续在L内按完整key重新查找并校验代际；不把slot号当永久record指针。普通bucket优化若按旧bucket锁直接读一个已转移到别的bucket的slot，会发生无保护访问；可安全relookup或另有明确slot锁，本轮首版直接用pair锁，不把“地址固定＋generation检查”当同步替代。

### 4.2 命中与慢路

- 合法frame先建立本次source/config身份并保其责任；ctx/table及B配置另有合法attachment／gate保护。
- 乐观读先取序号，odd立即转慢路；在至多H次固定位置探测中复制完整key、LIVE、DMA base等值，最后做正常retry检查。**任何冲突都丢弃整个结果**，不能把部分命中值用于DMA。
- 成功命中后只使用复制的值和frame局部参数，完成必要DMA sync／prepare；不留下record pointer、slot回调或CQ record访问。frame最后归还前结束所有依赖该映射的使用。
- odd／冲突进入L内重新完整查找。若当前LIVE，复制既有地址；若确认ABSENT／配额拒绝，才按共同已接受回退合同处理。**不能把版本冲突直接当ABSENT另map同一页**，那会引入本轮未核的重叠映射／同步合同。CREATING/CLOSING沿原有合法竞争／回退边界，不以新版本检查授权任意双图或丢弃有效工作。

一次乐观尝试后转锁是首版选择；没有等待时限保证。全表同序号意味着一个不相关key的写也会使本次读失败；热点写入／重建可能使大多数hit落慢路。普通R/O3可选择相同或更合适的分片，不能强留其弱实现。

### 4.3 删除和复用

open addressing使用EMPTY／TOMBSTONE及完整key规则：查找不能跨未处理的EMPTY遗漏同键，插入在确认无重复键前不能贪用第一个墓碑；碰撞及清理最坏H探测计费。H可以等于或大于K，但高装载／墓碑会增加成本，不能假定常数一次命中。重建／压缩若做，须L与版本覆盖、固定容量、时间和临时空间计费；本轮未实现。

owner只取真正idle且已独占隔离的p，在L下标CLOSING断新入口，锁外按同后端完成B unmap／必要同步。唯一closer及其他实际operation责任全部结束；之后L内提交删除／墓碑、完成该表示的RETIRED条件并返K。新key可复用固定cell；尚在复制旧数值的乐观reader仍只读永久内存，最终版本检查必须拒绝被改动的快照。**无需先等所有这种无指针读者退出，仍须防止旧writer和旧地址被实际使用。**

UNMAP_DONE和RETIRED仍分层：在没有额外CPU/worker责任时可以同一动作先后完成，不能写成unmap天然包含退休。如果后端异步撤图、worker还用record或控制读者外逃指针，就继续占K／保页；标准seqcount不会消除这些义务。

序号完整回绕、source/config代际或cell代际复用也必须处理。本文以一次读期间不发生完整版本循环为显式条件；若不能支持，需在达到复用边界前停止新更新、关闭相应入口并做整体reader静默／换代后再启用，或者另选表示。没有运行频率或时限事实，不声称已验证该条件或重置开销为零。

## 5. 决定性预测与相邻失败

**P1，结构性条件预测，非性能结果。** p实际idle并被owner隔离；B撤图已完成，唯一operation结束；另一个CPU仍处于覆盖旧索引的RCU读侧，或其callback尚未执行。RCU节点复用版必须保旧node；全锁版和固定值表在各自合同满足时可完成RETIRED并周转cell。值表的下一次无冲突LIVE命中不取L。预测只比较metadata可复用与操作集合；不把它写成已经省CPU、提高吞吐或减少payload RAM。

**对强普通对照的限制：** 允许RCU分离映射配额与退休node库存、预留额外node、批量退休，使用者也可直接改用本值表。额外库存够用时上述等待不必阻塞新接纳。只有固定实际metadata预算、实际表示大小及回收/服务序列后，才能判断这一结构差异是否形成资源优势；“同K”不自动等于同RAM预算。原生数值均null。

| 失败条件 | 为什么破坏推导 |
| --- | --- |
| 给可变链表加序号，reader先跟随已free的next，最后才发现版本变了 | 检查发生太晚，UAF已经发生；必须固定可访问存储／无指针值表或保原指针寿命 |
| 序号一致、DMA base也一致，但caller已归还frame，或B/config已换代 | 值一致不保DMA寿命；原frame与目标gate/drain仍须成立 |
| map/unmap长操作置于odd区，或写者被本CPUreader打断后reader自旋 | 可长等／自等待；短字段出版、外部writer同步、odd转安全慢路，不宣称无等待 |
| 删除无墓碑、插入见空位就写，或版本冲突被当成未命中 | 可能漏同key、重复图或错误使用；完整lookup／插入规则及锁慢路必须核实 |
| cell标FREE时旧creator/closer还在锁外写，或控制面按旧slot裸指针访问 | 固定存储不等逻辑复用安全；operation责任、完整key relookup及代际校验不可省 |
| 改动期间版本完整回绕，或query身份跨source代际却只比PFN | 可接受陈旧快照；必须处理回绕、真实source/config身份和旧alias退出 |
| 写频率高／H装载高，大多数hit读失败并争L | 价值失败：探测、重读与锁慢路可能比全锁或RCU更贵，不能靠“无锁”名称推净赢 |

## 6. 成本和物理资源分别结账

值表稳定hit仍付寻址、key/状态/地址读取、版本及屏障、offset与DMA同步；冲突付重复扫描和L等待。写者付序号更新和cacheline失效、同步、墓碑/清理；source回收、pressure/pump、unmap、关闭CPU同步、B/config及ctx整体寿命、未来重建和供给反馈照付。相对全锁版只是稳定hit不必RMW索引锁，不等没有CPU同步。

新增record不使用逐record RCU，不代表系统无RCU。原生`xdp_return_frame_bulk`的`bq->xa`在`net/core/xdp.c:480–492`保留allocator指针，`:455–464`后续使用，`:84–89`删除仍call_rcu；table/ctx/config整体销毁也必须保护全部reader。不能删除这些现存机制来制造低成本。

固定表H、每cell实际字段和对齐、K operation节点、bucket头／RCU额外node、控制存储分别核字节；尚未编译，不给编造的layout或cycle值。H按接纳预算预留而非按全PFN跨度覆盖，是与密表的具体空间区别；RCU稀疏同样不是全PFN密表。

固定预留K/H的表示即使少了退休等待，也不会自动释放已分配metadata RAM；只是可用槽位周转可能更早。RCU强对照允许record-only读者，在B撤图及页不再被旧reader触及后同样可先重发布p，不必为等待node grace扣住源页；不能据弱pin构造制造payload RAM差。

source本来持有的S仍在。仅回收metadata信用不意味着payload下降；真实B延后P、复制新增C、退出/异步保管Q按每策略实际序列与S取物理并集，PTE/IOVA另记。同步正常idle分支可无新增per-record CPU退休队列；backend/Q、config关闭和旧writer责任不因此归零。控制用时/重发布次序可改变后续分配，不强套同一O0 PFN轨迹。

## 7. 对具名普通方案及贡献的裁决

stock O0仍逐次map/unmap；候选仍有机会跨正常recycle复用B映射、在K压力时回收冷关联。DMA_PMD已有idle隔离／allocator协同；DAMN、DMA shadowing及dma-buf边界沿用既核原文，本轮没有新证据更改其覆盖结论。**R-OB4K-T-N、O3和合法slot适配都获得本值表、同同步条件和优化机会。** 普通R照用不证明全部整合思想无价值，但排除了把这种表示包装成候选独有能力或算法。

可核查的新增认识是：page_pool原frame寿命可以承担映射使用保护，固定无指针表示和版本验证可以另外承担metadata读取保护。两项义务分开后，逐record grace和逐hit锁都不是必需；整体ctx/config/allocator寿命依然存在。这具体化了前轮“RCU或等价保护”，没有找到超出普通seqcount适用条件的新并发机制，也没有取得完整成本数字。因此**本轮通过表示可行性的条件判别，未通过独立方法／净收益贡献门槛**；停止“锁版有必付每hit锁下界”和“每record一定等grace”的过宽成本前提，保留可实现的普通整合路线。

下一最小动作仍须以会改变论文价值判断的具体证据为输入：为一个具名部署取得可归属的完整map/unmap及lookup/同步/冲突/退休成本和实际reuse／关闭机会，或提出明确的新部署能力。已经结束的同批数据入口搜索不重开；没有这些新输入时不自动开发、采集、建模或继续拆相同协议。若只有本普通表示的条件推导，就按工程设计资产保留，不补同例数字强造新意。

B1/I/D/A5/A2、并发独立事项、MIG暂停及10月15—28窗口保持；未选论文主线。本轮全部Codex-only，执行与模型请求／实际元数据分开登记。
