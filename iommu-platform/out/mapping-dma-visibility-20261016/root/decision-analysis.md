# 持久B映射的数据可见性与完整费用裁决

2026-10-16 JST。**本轮保留“合法交接后复用整页B映射”的路线，并收窄其低成本主张：旧DMA句柄不代替新包同步；通用保守完成路径保留第三个DMA值，存B整页mapping base，先交回CPU侧再处理frame。** 不增加独立binding token，不代表可以普遍省掉完成所需地址值或同步调用。

另一个决定性结果是：**在固定dma-iommu实现中，物理起点及size均按该domain的IOVA granule对齐时，不走此处SWIOTLB bounce；普通逐次整页映射O0-T同样受益。** 反弹副本读旧包的反例仅适用于确实bounce的映射，不能拿来否定本整页对齐分支，也不能把“不bounce”认作持久缓存独占收益。

本轮是来源绑定的部署／费用条件核查，未开发、未运行模型或原生实验。继续最值得验证的是：**在满足CPU/A/B交接的具体后端上，省掉重复IOVA/PTE建撤图能否覆盖lookup、sync、控制／关闭及回收费用。** 不是新DMA同步方法或独占策略，未选论文主线。

## 1. 新判别题、固定切面和证据

上轮V-C固定值表解决拟议record读取／复用，不证明DMA backing中的内容已更新。本轮由“同p跨正常recycle装入新包，B旧映射是否仍看见新内容”这一具体合同触发，不重做metadata／NAPI协议、旧阈值账或数据入口搜索。

冻结官方Linux v6.12 commit `adc218676eef25575469234709c2d87185ca223a`，mlx5e cyclic／linear native-XDP、order0、单buffer非XSK，固定B/config；T允许完整实际granule范围及后续合法用途持续授权，S独立。具体domain granule、coherence、backend、trust、运行config及native成本仍未知。不是本机已有双mlx5或共享IOMMU的实测。

[API合同核查](../api-contract/findings.md)独占streaming／range／direction和真实driver放置；[backend核查](../backend/findings.md)独占dma-iommu/SWIOTLB。只补取同commit两份DMA文档、swiotlb.c和一个决定pool-membership的header；原有driver/DMA源复用。后文D=`include/linux/dma-mapping.h`、M=`kernel/dma/mapping.c`、I=`drivers/iommu/dma-iommu.c`、RX=`en_rx.c`、XD=`en/xdp.c`、PP=`net/core/page_pool.c`；S/H是本轮swiotlb实现／header。准确字节、hash、读取范围见[source-manifest.json](source-manifest.json)。

## 2. 映射范围、同步范围和实际分配范围不同

| 已核事实 | 对候选和普通对照的含义 |
| --- | --- |
| `en/params.c:256–281,693–707`、`en_main.c:604–640`：本XDP/linear切面stride=PAGE_SIZE，一页一次allocation一packet，wi.offset=0 | 不用同页其他在途RX片段制造当前反例；driver bias仍照留。single-buffer本身不构成这个证明 |
| RX:1680–1684：A for_cpu的size为`MLX5_SKB_FRAG_SZ(rx_headroom+cqe_bcnt)` | stride不是sync长度；这个表达式不是固定PAGE_SIZE，也不只是裸payload。宏未补取／展开，不填每包数值，更不能由stride推出整页A已交回CPU |
| PP:439–480,740–767：A页级map保留到离池，显式for_device属于A | A有自己的BIDIRECTIONAL映射；A sync不会遍历或更新B的映射／副本 |
| D:398–409、官方dma-api.rst:449–465：single sync可局部，range helper转发base+offset,size | B初始PAGE_SIZE map不会因此变成小范围map；后续可以同步合法packet范围。最终unmap仍配原map的整页尺寸、方向和attrs |
| XD:877–885、673–700：stock B映射data/len，完成时unmap再return | 删除逐次map/unmap也删除了其对应的交接动作，必须补正确sync／no-op合同；V-C只给地址和状态，不负责这些动作 |

首次整页B map还覆盖headroom、frame header等数据，不仅descriptor payload。T须确实授权该完整范围；没有该授权时改布局、复制或更小范围组织，费用另计，不凭存在IOMMU假定许可。初始整页cache交接也须覆盖所有相关缓存线；现A局部for_cpu不自动给出这个保证。

## 3. 一条保守的充分交接合同

官方[DMA HOWTO](https://raw.githubusercontent.com/torvalds/linux/adc218676eef25575469234709c2d87185ca223a/Documentation/core-api/dma-api-howto.rst)本地627–665给持久streaming的CPU/device反复交接；[DMA API](https://raw.githubusercontent.com/torvalds/linux/adc218676eef25575469234709c2d87185ca223a/Documentation/core-api/dma-api.rst)本地300–319、449–465说明方向和局部同步。下面是充分设计合同，**不是所有平台必须执行非零cache操作的下界**，也不是当前driver已实现。

1. **A完成→CPU准备。** 等A实际完成；在CPU/XDP触碰之前，对A映射完成其所需for_cpu。若将整页交给B，须补足整页安全CPU/A交接；保守可把相应RX入口改为整页A for_cpu，但这会影响该池的PASS等包，也要计费。不能先让CPU写dirty frame metadata，再盲加整页invalidate；合法的局部扩展优化需另证范围/缓存线和无并发条件。
2. **首次B接纳。** CPU/XDP及frame转换后建立`dma_map_page(B,p,0,PAGE_SIZE,TO_DEVICE)`并检查失败。默认map已对整个请求范围做对应准备。最终B handoff之前必须结束相关CPU frame／inline准备：可以预存所需值／重排准备阶段，或先B整页for_cpu→完成CPU准备→B合法range for_device再提交。新prepare/publication切面、失败回滚与额外调用均是实际整合工作，不能当免费接口。
3. **每次B提交。** CPU或A已更新内容后，在B读取前以B device、原B base、合法off/len、TO_DEVICE执行所需for_device；这不是重新建图。若首次map已经是最后CPU修改之后的最终handoff且无中间需交接访问，可不再重复同一准备。所有实际读取范围／缓存线条件须满足；完整frame责任保持到B真实停止。
4. **B完成→归还。** 验证B已停止读。保守版先B for_cpu覆盖原整页，再读取/复用需要CPU所有权的frame metadata并return。TO_DEVICE后端可能没有cache/copy工作，wrapper也可能跳过；CQ本身不是DMA API。范围状态机或特定可省调用须有证据，不把非零成本强加给所有实现。
5. **下一A循环和最终退出。** page_pool完成A for_device后才允许A覆写新包；A completion／A for_cpu／CPU修改后，再走B准备。最后一次B结束且禁新后按整页原参数unmap，source/target真正关闭、唯一closer、ctx/config寿命仍按既有合同。

原XD:593–608可以在xmit里复制inline payload；stock map只覆盖payload，不能据新增整页map推它错误。本轮只是指出：**不能无核查地把现有map行替成整页lookup，再宣称其全部CPU访问与最终handoff已正确放置。** XD:932实际读取预存局部`xdptxd->has_frags`，不是再次解引用frame；引用必须按真实字段，不能放大缺口。精确prepare/doorbell改造尚未实现或验证。

## 4. 完成路径：第三值有用，不是独立token

保守B整页for_cpu需要`sq->pdev`、B原mapping base、PAGE_SIZE和TO_DEVICE。CPU frame/PFN不能无lookup转换成任意IOMMU IOVA；为了先同步再解引用整页内的frame header，本轮选：**mode＋frame指针值＋B整页base三个逻辑值**。队列上下文给device，另两参数是冻结常量。CQ先pop base并for_cpu，再处理frame；不查V-C，也不新增binding ref／token。

stock原本就有第三个dma值；拟议producer/consumer一致把持久mode中的意义设为整页base，descriptor仍用base+off。实际entry/union/cacheline字节未编译，为null。若第三值保存packet DMA而希望读frame求off后反推base，须先证明frame header的CPU访问合法；不能用“先读header才能同步header”的循环来完成通用证明。特定no-sync配置、额外source-side交接、传递offset或lookup等合法替代仍允许且计费。

因此，**停止“两逻辑完成项是所有持久整合的普遍收益”这一扩大主张**；原来的两项窄版保持条件身份，未实施或改写历史结果。取消独立binding token仍可成立，与是否保留同步地址值是不同问题。普通R/O3/slot获得同一三值或已证明更简表示。

## 5. dma-iommu与反弹副本：只按真实条件下结论

I:601–612定义`dev_use_swiotlb`；I:1175–1189还要求`iova_offset(iovad,phys|size)!=0`才分配bounce。IOVA granule来自domain支持页大小，不借用旧FPGA IOTLB参数，也不自动等于CPU PAGE_SIZE。**p起点和PAGE_SIZE均按此granule对齐时，该B整页map不走这里的bounce，即使untrusted位为真。** 若granule／授权范围不匹配，分支条件保留，不强行称为无copy。

真正bounce时，I把PA换为S提供的bounce PA后再建IOVA：IOMMU与bounce并不互斥。S:1412–1439在map初始化时按`mapping_size`从原buffer复制；`DMA_ATTR_SKIP_CPU_SYNC`不删这份初始化copy。I:1194–1210还可清零untrusted前后padding，copy与zero分账。S:1552–1569表明后续TO_DEVICE/BIDIR for_device复制所请求的合法size；TO_DEVICE for_cpu不做bounce→原buffer的payload回拷。unmap另有slots释放及方向／attrs控制的回拷。

| 路径 | 已核逻辑动作 | 不能推导 |
| --- | --- | --- |
| granule对齐、非bounce的B整页map | 无SWIOTLB payload copy；仍有IOVA/PTE／可能arch cache准备 | 不是所有架构0同步CPU，也不证明整页授权已满足 |
| 非bounce后做小range sync | 不会新建bounce；H:248–279先检查PA属于pool才调用底层 | 看到swiotlb函数名不等发生copy；coherent但粗need_sync为真仍可能走查询路径 |
| 确实bounce的首次map | copy实际map请求m字节，可能padding zero，保留bounce slots | 上取整分配size不等payload copy量，逻辑copy量不等DRAM字节或CPU时间 |
| 确实bounce的TO_DEVICE range sync | 原→bounce复制合法l字节，必要cache维护 | 持久映射不消除每包copy；sync范围不反向缩小初始map或保留空间 |

`dma_need_sync`的官方合同只回答是否可省sync调用。D:295–331、M:430–464显示：IOMMU设备初始以coherence设skip；粗路径需要sync后，IOMMU的`__dma_need_sync`返回true，并不为当前地址细判bounce；S:1412–1416的首次bounce可重置skip，debug也影响调用。因此true不是copy分类器，旧false也不能永久当设备分类。优先保留正确API调用，让实际wrapper／backend执行no-op；不自造flags或从A状态推B。

本轮未展开`dma_kmalloc_needs_bounce`的架构内部条件、direct-DMA force路径或实际arch cache指令。固定dma-iommu触发处不读force位，故不能用`swiotlb=force`证明此整页分支必bounce。这些都是限定边界，不冒称已经全平台验证。

## 6. 决定性预测、相邻反例和强普通对照

**P-VIS（手工条件预测，非运行结果）。** 固定合法frame、全范围T、稳定B/config、有效dma-iommu且p起点／PAGE_SIZE按IOVA granule对齐，完成上述交接后，p装入两个不同包时，B可继续用同一成功映射并读取新数据；此分支SWIOTLB payload copy为0。它省去第二次建撤图的机会，仍付数据同步、lookup及全部源/目标职责，不给CPU／吞吐净赢。

**F-VIS，真实bounce相邻反例。** 仅改为实际满足bounce触发的非对齐成功映射d→Q；第一包P0在map时复制到Q。B结束、CPU/A合法生产不同的P1后，仅复用d且省掉B for_device，则Q仍可含P0；B读旧包。补合法range for_device可复制P1解决。该反例不能移植为“当前对齐整页一定读旧数据”的证据；coherent也不会令两个不同PA副本自动相等。

**必须保留O0-T：** 在同一完整granule授权、整页实际独占及CPU/A/B交接条件下，普通每包fresh map对齐整页、descriptor只读包范围，完成后整页unmap，同样避开此bounce。O0-T不需要持久关联／K/V-C；付每次IOVA/PTE、相应cache准备、CPU访问放置与退出成本。其优化接口候选同享；不能以弱payload-map baseline制造“避免bounce”的独占创新，也不能由此说二者总成本已相等。

| 相邻失败 | 对具体主张的限制 |
| --- | --- |
| 只见stride=PAGE_SIZE就省首次整页交接；或CPU写metadata后再盲加A整页invalidate | 初始CPU所有权／cacheline条件未闭合；普通整页map与候选均须修正并计费 |
| 把A sync、B CQ、旧IOVA或V-C稳定版本当本次B同步 | 各自证明不同义务；错误bounce分支可旧数据，非coherent分支也不能凭句柄跳cache合同 |
| 保守完成只存mode+frame，又不准lookup／无额外传值 | 无通用B IOVA来源；采用三值base或证明更窄no-sync／其他付费路径 |
| 最后一个包长用于撤销原整页mapping | map/unmap参数不配对；按原整页参数退出 |
| 有效工作大多一次B使用、额外交接／回退多、sync/query与控制费超过少做的建撤图 | 价值失败，需限制该部署收益；不因缺频率就自动开发或宣布整体思想失败 |

## 7. 完整成本、资源与贡献裁决

stock O0包含每包payload map/unmap；O0-T有每包整页map/unmap；持久普通R/O3/V-C有首次／最终map/unmap、每次必要sync、lookup、K/H、控制及关闭。不能一方按完整streaming、一方只数PTE调用。首次整页同步、可能新增的全页A RX交接及PASS前缀费用、TX prepare／inline/publication适配、三值CQ取值／API、动态need_sync/debug/backend查询、失败rollback、metadata退休及cold重建都计。

CPU、payload copy逻辑字节、padding zero、cache维护、IOVA/PTE、metadata读取分别报告；逻辑copy不是固定DDR流量。实际物理按各策略S/P/C/Q并集去重。bounce slots是后端另外分配／保留的副本，与源S不同，须单列backend bounce实占及pool预留／暂不可用份额后再入物理并集；不能漏掉，也不能把预留整pool都算每map新增。本轮将后端副本另记C_sw，物理并集扩写为S∪P∪C_app∪C_sw∪Q_retire，并按A/B实际backing分别核账；B不bounce不证明A也不bounce。手工copy方案的目标C_app、B延后P、真实退出Q_retire另计；对齐非bounce分支不凭函数名增加一份副本。控制与回收可改变后续分配，不套同一O0 PFN轨迹。

**裁决：通过“后端/数据可见性可以形成明确条件合同”的判别；保留V-C＋标准streaming交接＋三值Bbase的保守整合版本。** 本轮停止泛化的两完成项收益、IOMMU必无copy、整页必bounce、need_sync能分类copy、A sync代B sync等前提；不否定已有frame免独立token或固定值表条件证明。源码核查不是原生正确性／性能实证。

最接近的普通持久映射／DMA_PMD及DMA API已经提供复用和sync能力，DMA shadowing／DAMN已有copy与持久组织前史；本轮没有新论文全文，不扩大覆盖结论。O0-T、普通R/O3同享aligned mapping和同步优化，故**本轮新增的是实际整合合同与费用纠偏，不是独占新机制或新的同步算法**。只有在具名部署中，同有效工作与服务要求下取得可归属CPU／不可回收内存或部署能力的实质差异，并给完整成本和适用边界，才可发展对应贡献。当前所有native数值仍null。

下一最小动作是[对齐streaming复用证据合同](next-minimal-contract.json)：只有新、匹配backend／granule／sync/no-op合同的完整费用／生命周期输入或具体新能力差项，才推进有界贡献判断；不重复同批旧数据入口、旧阈值账或相同协议，不由未知自动建driver／collector／模型或板测。不等一般授权、CPU/内存价值确认或用户保证痛点。B1/I/D/A5/A2、独立事项、MIG暂停及10月15—28窗口保持，未选论文主线。
