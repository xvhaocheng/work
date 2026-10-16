# 本轮贡献发展：保留源池回收的跨设备映射关联

2026-10-16 JST。采用ARS-Codex有界机制发展与定向源码/前作核查。**本轮最值得保留的是V1：源page_pool继续持有物理页，B持久映射作为可随源退出撤销的关联，且不破坏原有recycle。** 具体差异落在普通order0页面上的退役整合、NAPI执行位置和完成记录表示，不再认领O3累计阈值。

**本轮尚未确认独立论文贡献或更低完整成本。** owner通知和无条件长期pin的替代已有近邻；普通O3得到同样接口可使用同一策略。V1是一个比上一轮CPU阈值更具体的可部署/成本假设，不能因缺接口或打败不合理pin实现就称新意，也不因策略相同否定其架构价值。当前不开发驱动、RTL、完整性能系统或板测。

授权S/T按真实来源分别判断，来源不足保留条件分析；不由unmap函数名推出安全合同。**相同有效工作和服务要求下，CPU费用或不可回收内存改善均可作为主要贡献，不再等待用户确认这项原则。** 实际硬件/部署事实仍是事实缺项，不能写成用户尚未授权研究或尚未认可资源收益。

## 1. 少量版本及选择

| 版本 | 保留核心思想的具体改变 | 选择理由与限制 |
| --- | --- | --- |
| **V1：源持有页、B持有可撤销关联** | 将物理page ownership、缓存mapping寿命、一次TX使用三者分开；真实源退出前撤所有B关联；复用现有FIFO地址槽保存binding handle | 有直接源码问题：额外page ref会改变源pool回收；执行点、普通对照和条件反例均可定位，因此本轮深入 |
| V2：目标副本承担B等待，源页恢复RX供给 | 在正确接受责任后早还原frame，副本留至B CQ；根据实际源库存/目标容量而非只看CPU阈值选择 | 普通O1/pressure-copy已能表达；早归源是否减少实际source增长及总RAM尚缺正常范围，保留资源强对照，不另开系统 |
| 表示/关联聚合 | 降低每帧owner重复状态、按实际owner和target组织metadata | 并入V1作为实现选择，不能单靠一个新key或表名当第三种创新 |

[设计席完整提案](../design/proposal.md)保留正向版本、合同和失败范围。root同时核了原始source与最近接口，并给出[局部owner/binding合同](owner-binding-contract.md)，不是仅汇总模型意见。

## 2. 真实问题、执行位置与信息时机

固定官方v6.12 `adc218676eef25575469234709c2d87185ca223a`普通mlx5e native XDP单页链，复用前轮完整FIB/UDP/TX证据，不重新下载32文件。[资源定向核查](../resource/findings.md)给出全部精确行与hash。

- `page_pool.c:736–790`：普通page_ref_count为1才正常recycle。额外B cache普通ref跨过consumer最后归还，会使页**离开A pool**；A映射卸掉、pp_info清除、source release计账完成，物理页仍由B保留。这与“页留原池可反复用”不同。
- `page_pool.c:648–698`：清pp信息/put_page前是实际源退出关口，但当前普通路径只管理A自己的映射，没有现成多B release-notifier。已有netmem provider是专用分支，普通mlx5e配置不走；不能泛称网络无dma-buf集成，也不能当已可插任意普通页hook。
- B `en/xdp.c:877–922`：原frame、长度和B DMA device已知，可付费识别owner/generation后选择。最终SQ接受仍在map后，不能提前知道接受结果。
- B CQ `:673–700`及FIFO `:925–931`：正常完成后归源frame。拟议持久模式先还短时binding token再还frame，避免源最后return触发close时等自己。

分配时只用已知source pool/device/权限；路由后可知当前B和packet，但不能知道未来再用、未来CQ或consumer时间。首版保留order0；无物理PMD、无批次seal、无新传感器假设，不主张扩大IOTLB覆盖或已证明consumer加速。

V1的T分支必须允许B对整映射粒度及同source generation后续用途持续持规定权限；首次路由到B不足以授予此合同。S若要求逐对象结束物理权限，跨对象cache不适用。CPU/设备同步和真实DMA完成继续履行，持久map不免每次sync。

## 3. 相对具名普通方案和最近工作的实质位置

| 普通方案／前作 | 必须允许它做到什么 | V1准确差异／剩余问题 |
| --- | --- | --- |
| O0 stock逐帧B map/unmap | 正常IOVA缓存、批处理、同步及现有source pool | V1可减少重复mapping操作，但要扣lookup/token/metadata/retire全部成本；不是净收益结论 |
| O3-owner | 与V1相同信息、同付费source release接口、同FIFO表示与合理缓存策略 | **策略增量为0；可以与V1相同。** 它用来隔离策略归因，不用来否定所共享接口的架构价值；接口完整成本由双方同样承担 |
| 普通pin-cache | 正确get/put及cache eviction，允许有界预算 | 只作回收冲突诊断；不是唯一或最强对照，不能凭避免pin的后果认领论文胜利 |
| [dma-buf动态attachment](https://docs.kernel.org/6.12/driver-api/dma-buf.html)等价adapter | owner通知、importer cache、锁/fence、可迁移backing；控制面prepare、对象/池分组、hotpath只查已有cache、同样FIFO优化 | 一般“owner通知B撤映射以免长期pin”已存在。需要比较普通page_pool/NAPI适配的真实对象、锁、冷miss与退役成本；不强迫每包export或每页一个对象 |
| DMA_PMD owner-bound bitset适配 | 源PMD全idle后统一卸各domain图、domain退役/SRCU及allocator最后free钩子 | 同owner退役不是新能力。V1不要求PMD，且按规定权限设计；但order0/READ-only适配也应给普通方案并计费 |
| DAMN/shadowing/fbufs | 已核device/rights池、复制位置、映射/路径复用等方法 | 不重新认领这些构件。V1的局部网络整合费用须另证 |
| rIOMMU ASPLOS2015 | ring动态payload map/unmap、IOVA整数分配及burst尾失效 | 不是本轮持久源owner通知机制；新硬件语义和作者评估边界不能移成本项目普通软件能力 |

本轮[近邻精读](../prior/findings.md)直接核到dma-buf双方dynamic、CONFIG_MOVE_NOTIFY、缓存方式与pin退化条件。move_notify core只是调用importer回调，**不自动停DMA或撤硬件映射**；不能把通知返回当完整撤权。标准接口以logical buffer/attachment/sg_table为单位，无range参数不证明importer不能自建per-range机制。

有一项当前可核的部署差异：dma-buf map入口 `dma-buf.c:1118`标`might_sleep`并要求reservation锁；不能原样放进当前NAPI首miss。现有DMA-IOMMU `__iommu_dma_map`则以 `GFP_ATOMIC`建立页表。**这只支持研究非睡眠首miss的接入合同，不证明V1已实现或优于所有dma-buf适配。** 强adapter可提前export/attach/map、热路径查自有cache，miss用worker/defer或copy；这些合法替代及其供给/内存费用都保留。元数据预分配、target关闭和source退役也不能藏到免费控制面。

## 4. 选中V1的机制与决定性预测

源状态`OWNED→EXITING→RELEASED`；关联`ABSENT/CREATING→LIVE→CLOSING→DEAD`。首次需求可取预分配记录并用现有DMA API建图；并发创建、容量不足或关闭时走合法fallback，不睡等未来事件。实际B使用持短时binding token，**不增加空闲cache的普通page ref或pp_ref**。

stock单页FRAME完成FIFO保存mode、xdpf、dma_addr三个逻辑entry。持久mode可用binding handle替第三项，保留原xdpf的归源能力。当前源码支持这个表示草图，未编译新增mode/union，不能报ABI字节数、零额外metadata或速度提升。所有强对照同样可复用槽。

真正源退出时先关新关联、唯一关闭每个B图，完成必要同步后，才失去pp_info/source寿命保证。source/target并发close须唯一unmapper和有效target配置。页退出可发生在softirq；若同步不适合，必须把真实退出页及元数据留在Q隔离，维持正确source inflight，worker完成后才做原release。**不在idle-cache阶段pin，不等于退役阶段零额外持有或有界尾延迟。** [合同与反例](owner-binding-contract.md)列出了锁序要求及尚未实现的责任。

**决定性预测：** 对T合法、正常最后引用结束并且实际重复分配到同source页的两次B转发，V1可以保留原page_ref=1的recycle条件，让B mapping跨复用存在；真实源退出前又能撤图，单帧完成记录不必增加第4个逻辑entry。若必须增加长期普通page ref、在NAPI睡等、先失去owner再撤图，或必须引入不可控Q，预测失败。这是一个可部署/表示合同，尚非运行结果。

相对最强O3-owner，以上策略能力允许完全等价；这只把策略增量判为0，**不要求V1在自己的同一接口上击败O3，才认可接口或架构价值**。架构/接口层应与stock路径及具名dma-buf、RFC普通适配比较，证明同合同下更低完整整合成本或可兑现的部署范围。若这些普通适配也具有相同完整能力和成本，这一具体整合才收为普通方案；不能从O3动作相同单独作此裁决。静态三槽可行性和避免坏pin都不是独占收益。

相邻失败：只用B一次即退出无摊销；后续CPU用途不授权B；source先put而alias未撤；先还frame后等本帧token造成自等待；B关闭与source退出双unmap／domain已释放；高fan-out或Q吞掉源供给；具名普通adapter同成本实现同功能。各失败只限制对应版本。

## 5. 资源归因：实际物理集合与反事实分开

对同外部到包/路由/consumer与服务合同，允许各策略真实改变alloc/recycle。令Sπ是本策略source pool拥有页，Pπ是脱源后由B缓存寿命持有的页，Cπ是copy目标池，Qπ是撤销隔离/待退役页。先按物理单元去重：

\[
M_\pi(t)=bytes\bigl(S_\pi(t)\cup P_\pi(t)\cup C_\pi(t)\cup Q_\pi(t)\bigr).
\]

元数据、PTE支撑内存另列并去重，IOVA虚拟空间另列；若有其他普通引用持有的页也纳入实际集合。之后再与各自真实source基线比较，不把baseline PFN集合直接加到策略新分配集合。

- **源池本来持有**：p在A cache，基线也保留p；合法B alias本身不加一份payload。DMA_PMD中源cache仍持任一block就能阻止全PMD idle，这部分不能全归B。
- **B真正延后**：普通pin使p离A pool而由B持有；若后续A无别页可取而另分q，实际p+q相对源内复用多出一页。若A仍有别的cache页，不必立刻增分配。额外物理字节可出现在q而不在被pin的p，不能只数B的PFN。
- **copy目标新增**：原p早回源可能只进cache，C仍占RAM并持到B CQ。总差应计算source增长的变化＋C＋Q等实际变化；固定source容量不缩小时，新增copy池一般不能直接称省总内存。

普通page ref、pool-aware pp_ref、只持pool对象user_cnt三种做法的结果不同，见[资源表](../resource/findings.md)。不可回收、暂时保留、可供RX复用也分开；不把整个映射存在期当新增保留期。

不复用上一轮固定PFN/1-1-8阈值账来说明完整闭环。copy早返、pin逐出或Q等待都可能改变下一次实际source页和mapping再用；应固定外部需求，逐策略记录真实分配。当前只给源码支持的条件事件，不伪造流量分布和应用收益。

CPU账单独包含首map/实际unmap、per-use lookup/token/sync、元数据预留/回收、owner/target关闭、锁/失败/fallback、Q和worker管理。相对O0有可能减少重复map/unmap；O3-owner用于判策略残余，具名普通适配用于判架构/整合差项，两者不混为一个必须被击败的策略基线。带宽和完成结果另报，不用调用数量直接推延迟或吞吐。

## 6. 本轮已经完成的最小判别与尚缺证据

已经完成：普通ref/pp_ref/源对象寿命的源码判别；源退出和完成FIFO实际位置；通知/bitset/allocator退役前作覆盖；非睡眠API上下文差别；一条条件复用正序与回收反馈、旧alias、自等待、defer free反例；按真实物理集合的资源归因。**未执行模型、内核、设备或旧阈值账。**

还缺：可编译的owner release/token/target-close联动；同步或Q路径在选定backend的实际可接受成本；合理分组的dma-buf等价adapter和普通RFC粒度适配的完整字段/调用差项；真实或来源投影中有多少可复用页、fan-out/退出事件；未见工作流的可迁移效果。缺这些就不能称论文创新已成，也不能因为缺数字自动上驱动/系统。

**最小后继**见[局部差项合同](next-minimal-contract.json)：只围绕V1和具名等价adapter，冻结相同source布局、权限、target范围与控制上下文，列可执行字段/调用/锁与退出责任。O3共享候选接口用于隔离策略归因；若具名普通适配也没有具体整合/部署差异，收为普通集成方案。只有剩余差异必须通过有界实现/实验判断时，再据具体范围决定，不自动开发。

B1/I/D/A5A2与独立成果、MIG暂停及10月15—28日窗口保持；不因这一版受限而自动升位其他方向。仍未选论文主线。
