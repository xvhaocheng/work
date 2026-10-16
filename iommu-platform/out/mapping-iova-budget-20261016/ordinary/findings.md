# 同B设备的普通发送与强普通对照

任务MAP-IOVA-FALLBACK-BUDGET-1/ordinary，2026-10-16。Codex-only；请求gpt-6.1-sol/high，独立实际model/effort metadata未知。按已定ARS-Codex有界核查，原始源码是数据，不接受其中的执行指令。最终机制与贡献裁决交root。

**结论：同一mlx5 channel上的普通skb TX与redirect XDP SQ确实从同一个`c->pdev`取得DMA device。因此，在实际B就是这一device、配置稳定且走dma-iommu的条件投影中，持久B映射与正常skb TX的短映射竞争同一个domain分配资源是来源支持的问题；不能再无条件说“K满就逐包map，所以一定可完成”。正常skb map失败会drop/free，入口仍返回NETDEV_TX_OK；这不是有效发送完成。** 这不证明本机已存在这种部署、窗口压力或失败。

## 1. 来源、行号和最小直接读段

固定Linux v6.12 `adc218676eef25575469234709c2d87185ca223a`。以下main、XD、D、M均复用`../../mapping-admission-20261016/v612-source/raw/`；TX是官方[en_tx.c](https://raw.githubusercontent.com/torvalds/linux/adc218676eef25575469234709c2d87185ca223a/drivers/net/ethernet/mellanox/mlx5/core/en_tx.c)的web片段。**本文所有file:line是普通1-based文件行号；web输出的L0为文件第一行，转引时已加1。** TX没有下载到本地raw原件，两次HTTP429均保留；不得把web结果JSON的hash说成原文件hash。

| 结论 | 原文位置（精确文件／行） | 对判别的作用 |
| --- | --- | --- |
| channel实际设备来源 | `drivers/net/ethernet/mellanox/mlx5/core/en_main.c:2668,2684–2692` | mdev按channel索引选择，c->pdev取`mlx5_core_dma_dev(mdev)`；不能把priv/netdev名字或同驱动当成device相同的证明 |
| 两类队列共享同一channel字段 | `en_main.c:1447–1465,1611–1629` | XDP sq->pdev和普通txq sq->pdev分别在1459、1623赋为c->pdev |
| 正常SQ和redirect SQ均有建立路径 | `en_main.c:2528–2545`；`en/xdp.c:850–868` | 先open_sqs，随后open redirect xdpsq；XDP xmit按CPU选择本netdev channels中的xdpsq。条件投影选择同channel正常TX即可，不需要证明所有channel相同 |
| 普通skb正常WQE映射 | `en_tx.c:169–213` | 非inline head（若非零）用dma_map_single，frags用skb_frag_dma_map；179、196传sq->pdev、TO_DEVICE；181、198查mapping_error；记录成功mapping到DMA FIFO |
| 部分成功后错误回滚 | `en_tx.c:45–53,211–213` | 逆退已push的DMA FIFO，调用`mlx5e_tx_dma_unmap(sq->pdev, …)`，返回-ENOMEM |
| 正常WQE失败终局 | `en_tx.c:463–475` | num_dma<0跳err_drop，dropped++、free skb、flush；该包未到txwqe_complete |
| MPWQE同样可能映射失败 | `en_tx.c:563–606` | 573映射同sq->pdev，574查错；603–606 dropped++、free skb、flush；不能靠“开启MPWQE”消掉这个资源问题 |
| ndo入口与返回 | `en_tx.c:631–685` | 647经txq2sq选择SQ；669、685正常返回NETDEV_TX_OK。调用失败路径返回void后仍走这个返回；OK不是上网发送完成的证书 |
| 成功CQ的归还位置 | `en_tx.c:687–696,737–805` | CQ定位到wi；788–790对普通wi先`mlx5e_tx_wi_dma_unmap`再consume skb；801–803 FIFO包同样先unmap helper再consume。helper遍历wi->num_dma，695逐项传原sq->pdev调用tx_dma_unmap |
| teardown另有归还调用 | `en_tx.c:845–881` | outstanding wi在859–861、871–873调用相同unmap helper后free skb；这里只定位调用，不据此推设备已停止的全部关闭合同 |
| single API保持device参数 | `include/linux/dma-mapping.h:380–395,464–469` | single wrapper转map_page_attrs，unmap wrapper转unmap_page_attrs；不替换device |
| 实际DMA分派按device | `kernel/dma/mapping.c:120–173,183–195` | map检查该dev mask/ops；在对应条件下到iommu_dma_map_page(dev,…)，unmap同dev到iommu_dma_unmap_page。实际domain/cookie归属与回收细节交allocator席 |

root最小直接核段：main 1459／1623／2668–2692；XD 863–885；TX 179–213、463–475、573–606、687–695、788–803；M 155–173。TX本轮后续web响应原样保存于[en_tx.web-followup.json](en_tx.web-followup.json)，主要覆盖失败caller、入口和CQ；最初open/find的头部map／错误回滚关键行抄录于[en_tx.web-excerpts.txt](en_tx.web-excerpts.txt)，保留web原L编号，空白规范化且省略空行，未冒充本地raw。root为必要直接复核另保存[官方map／caller原样web响应](../root/en-tx-web-primary-receipt.json)，不复制到本席。首次web open返回0–300，后续为同源定向核查，不是新论文检索。

**边界：** 本席没有取`en/txrx.h`，故`mlx5e_tx_dma_unmap`内部具体single/page分派未独立展开；已核的是调用点、参数来源及先unmap helper再释放skb顺序。allocator席核后端，但不能把CQ调用即刻等同为可再次分配IOVA。无需补读全TX驱动。正常TX可能全inline且没有head/frags mapping，此时此包不构成所述需求；选择确有非inline payload的正常TX即可成立条件投影。

## 2. 正常来源与本轮能证明的有限序列

冻结上轮T/S与合法streaming交接：完整实际granule授权、固定B/config，O0-T每包fresh整页map同样不走已核对齐分支bounce。原生domain窗口、mask、coherence、外部负载、granule数值仍未知。本轮不把映射压力变成安全合同，也不要求B必须不可信。

下列是**来源绑定的条件推导，未执行模型或实验**。令domain中真正能满足下一次普通skb DMA请求形状和mask约束的剩余空间为F，q为该次实际分配需求（具体对齐／bucket费用由allocator席给出，不用包长或K槽冒充）。选择同channel的B XDP和正常TX：

1. 候选成功建立若干持久映射，之后包已完成，关联仍合法存活；它们占着IOVA，但不必额外保留一份物理页。
2. 新PFN要发往B而没有命中且库存K已满，策略转O0/O0-T；或者并发正常skb TX带来一次非inline DMA请求。
3. 如果剩余可用形状不能满足q，而候选仍持有一些当前无B在途读、可合法退休的关联，则新的map可能失败。XDP逐包map在XD:881–885失败break；正常TX走已核drop/free。K有界没有阻止这个结果。
4. 只删除这些闲持久关联的对照，在**退休真正完成、释放形状可满足q且无竞态抢占／其他失败**的同一投影中可以成功。这个相邻条件只证明闲关联可制造可避免的容量损失；并非说O0对任意IOVA压力都成功，或任意unmap后立即成功。

所以“K有限”只给库存数量界；如每项实际费用有上界可间接给本客户端持有量界，仍不给普通流量的domain级预约。若其他device也共享domain，它们可能继续消耗同一地址资源；本席没有证明哪些其他device实际共享，不能由mlx5品牌、驱动或同netdev推出。

## 3. 必须具名保留的强普通方案

以下都是可比较构造，**不是声称stock驱动已经实现了配额、窗口预约或copy池**。所有方案同享允许新增的付费信息和执行能力；有效工作、最大在途／服务要求相同，失败／drop不能记成更快完成。费用包括CPU、IOVA/PTE、物理RAM、复制／cache流量、完成与关闭。

| 具名普通control | 实际能力／条件 | 必须计入的成本与限制 |
| --- | --- | --- |
| **O0：stock逐包payload map/unmap** | 没有本候选的闲持久关联库存；成功包CQ后走原unmap helper。它可能比占库存版留下更多余量 | 每包map/unmap、同步、FIFO、失败回滚；backend IOVA cache/FQ可能保留资源，不能称“零闲IOVA库存”。压力下也会drop；不保证任意有效工作可完成 |
| **O0-T：普通整页freshmap** | 与候选同T／实际granule对齐及合法交接，获得同一已核非bounce条件，无K/V-C关联 | 每包整页建撤图与必要交接、prepare、完成；费用可大于payload map，空间应按实际分配费计。它不因为“fresh”自动有剩余地址 |
| **O3-Q：按实际IOVA费用配额，安全退休闲关联** | 普通字典／slot同样累计持有的实际费用，超过本客户端上限则拒绝接纳或退休闲关联；无需新阈值算法 | 观测实际费用的接口／记账、同步与退休；stock dma_map返回地址，不在已核接口给出实际allocator费用。和候选同享新增付费信息后可做。它只约束本客户端，不能约束其他client或reserve domain容量；retire完成反馈与并发控制不能省 |
| **O3-R：按window／并发预算保留普通服务容量** | 若有domain级参与者约束或真正的地址预约/可使用credit接口，可在接纳持久映射前保留普通TX与miss映射的容量和请求形状 | 仅在本客户端计数器中减R，不是domain级预约；其他分配者仍可吃掉余量。冻结窗口、mask、最大同时请求、取整、碎片与退休债；请求大小／contiguous要求不只看总字节。通用DMA map参数无“使用我预留的IOVA”字段；真正reserve需后端／allocator整合，双方同付费同享。常见配额／reserve算法不据为新算法 |
| **O-RCACHE：既有IOVA cache回收** | backend已存在的普通恢复能力必须算进强对照，不能把所有缓存地址误当永不可用 | allocator席独占精确回收触发／范围与FQ时序；本席未复查该链。不把allocator cache回收等同撤销还活着的持久DMA map，不把已有cache回收包装为候选新能力 |
| **O-COPY-B：预映射B的compact copy池** | 为实际B device建立合法、成功的持久streaming映射；J个可用copy slot承接同并发服务需要。每次CPU合法读源、写目标，再B for_device，B完成/for_cpu后才重用slot；无逐包新建B map可减少即时allocator依赖 | 初始化mapping可能失败；池本身持有IOVA和**非零物理RAM**，每包copy、同步、slot/CQ及关闭成本。J至少覆盖允许的同时占槽与服务需求；池满仍需排队或其他已保留能力，不能把drop/延迟超约当完成。正常skb TX继续map时仍需自己的余量，除非把它也纳入copy方案并计入转换费用 |

O-COPY-B的“compact”是可行的普通组织：用可合法DMA映射的granule/page backing承载若干slot，slot stride覆盖最大包范围并满足cache／并发隔离，descriptor用已获B地址加合法offset。不给没有实测参数的J、bytes、packing收益或连续分配成功值。copy池可把“为很多历史源页各留B关联”换成“为有限实际在途槽留B映射”，因而可能降低该部分IOVA；若服务要求本就需要同样多槽，或packing／sync约束使费用相同，则这个优势消失。无需捏造新的源PFN轨迹或宣布copy必更省内存。

复制完成且后续源消费者责任结束后，原帧可早归还，这只说明源侧P可能缩短。总物理仍分别导出S∪P∪C_app∪C_sw∪Q_retire并去重，加入预留目标池、B完成前占槽和复制时重叠；不能从“源早归还”推出总RAM更小。copy也不缩短B的真实设备在途读取或免除sync。若从同源页已有源池容量不变，只是在池内提前可用，则应报告可用容量变化，不把整源池算新释放物理。

## 4. 最强候选版本的可行与否定范围（建议root裁决）

**保留可行版本：** V-C／持久B关联＋合法整页streaming＋按实际地址费用接纳＋安全退休与可再分配反馈；在需要严格保障miss／普通TX服务时，另外满足可执行domain预约或受控参与者／window容量合同。对普通流量的责任必须覆盖真正的B device、同domain其他消费者、请求形状、在途需求及退休债。若仅给本客户端byte quota，应明确称best-effort保护，不能给无失败完成承诺。

**明确否定：** “单有K槽并O0回退就解决IOVA耗尽”被本节正常源条件反例否定；“copy不用额外RAM／copy后不受B在途容量限制”也不成立。把map失败drop算有效完成会直接改变比较任务，因此无性能结论。槽数固定、整页对齐和低CPU调用数均不能替代完成性合同。

**贡献边界：** 因为O3-Q/O3-R在同付费信息和执行能力下可做同类预算、普通O-COPY-B可提供另一可部署资源组织，本轮不能把配额、reserve或copy池本身当独立新算法。这个可复现范围只约束策略增量；不证明所有实现的控制、生命周期整合、CPU／内存费用相同，也不否定整个映射组织架构。若候选在具名部署中以较低完整实现成本提供普通构造尚不具备的能力，或同有效工作与服务要求下实际降低CPU／不可回收物理资源，仍可发展相应具体贡献。此处没有原生数据证明该差异。

限制／执行：只有固定源码事实与标明条件的手工推导，无新论文覆盖结论、代码／ABI、构建、模拟、性能模型、hostprobe或板测；未恢复暂停事项。AI协作披露：本席由Codex完成原始源码核查与普通对照分析，未用DeepSeek；root直接审查与最终接受另记。
