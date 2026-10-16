# LR1：读阶段归还与局部翻译撤销，2026-10-15

**结论：交付了源页分阶段归还的私有功能原型；独立普通分阶段程序及定向排空程序复现其完整事件链，本版没有独有动作或完整可实现成本优势，不进入性能条件。** 这关闭的是本冻结LR1方法主张；原生接口能力、准确所有权边界及真实值参考保留，论文未完成。

actualgoal1791990424已登记，无额外token预算，两个工作日上限；root和两新独占席实际6.1-sol/ultra、fork none、有效原生上下文475000，未修改覆盖配置。五页同步及必要新交付验证之后才actual complete；回执由[完整导航](../out/local-read-reclaim-20261015/README.md)进入。日期按用户Asia/Tokyo 2026-10-15，原立题窗口累计。

## 具体规则与新增能力

正常source read全部完成后，source PA页可以由新owner写入新内容，而旧copy目的仍从已保留oldbytes继续W/B。对象是(sourceID,job,完整地址空间,物理page,mapping generation,全部reader/alias集)，不能只比较IOVA。固定DMA0/1 ID0/4、各原512beatbuffer、4096B对齐DEFAULT/deburst=0、只读源授权、source PA!=destination PA、单PTW/cache16及原队列；不扩大64B fullbuffer许可。

规则是：关闭旧代新parent/未登记范围，join全部已授权读字节和old raw/walk/N OUT/normal result/heldAR/acceptedR义务，撤销旧PTE、完成必要cache/PTE可见性、最终精确失效并防oldwalk晚fill，取得target normal-memory排序证明，再ACK释放source页。任何firstVALID保持到实际接受；目的W/B/consumer继续。新ACK不代表globalIOFENCE完成。

**seal frontier必须从frontend→原JobFIFO接受parent算起。** 已接受4096B parent的尚未VALID第二burst仍是既有授权/debt，继续生成并正常drain；禁止它会使半页任务永远无法取得整页收据。JobFIFO depth8不提供八个免费未来任务。来源及规范化接口见[机制卡](../out/local-read-reclaim-20261015/root/mechanism-card.md)。

## 原生链：能证明什么，不能证明什么

[root亲核11files/22原文](../out/local-read-reclaim-20261015/root/critical-source-evidence.json)，结合[source席35files/118原文](../out/local-read-reclaim-20261015/source/source-evidence.json)及[原生链](../out/local-read-reclaim-20261015/source/native-chain.md)，形成以下稳定事实：

- 原byte FIFO不fall-through，最终R进入后，其最终W以及B仍有因果后继。无需加延迟/压力即可给出Rdone→Wpending的源码偏序。**新增ACK执行完时是否还W/Bpending未知**，其seal/PTE/cache/inval/order开销可能耗尽窗口；未执行native链。
- 每个4096B DEFAULT是两段256beat，需全512次实际R handshake、两个RLAST及每次RRESP；RDP credit也保留。DEFAULT done跟最后B，NO_ERROR_HANDLING error固定0，不是读阶段成功收据。
- N只在LEAF接受时核invalidate；已接受OUT继续返回不是自动bug，而是尚有owner。正常结果、heldphysicalAR也不因cache失效消失。最终失效必须在旧owner/可能晚fill关账之后。
- 当前launcher仅支持64B/CONF0，不能免费封4096B任务。当前Linux src/dst都映射READ|WRITE；只读源、PA alias/全reader登记和page/job/epoch流水线关联均缺接口。
- 当前CQ PR/PW仍TODO，CQH在REGISTER先推进再DECODE；现全局AR-RLAST/in_flight计数不能作局部ACK，也没有传给CQ构成已验drain。Linux first-stage精确PSCID/ADDR路径被注释，补标准基线功能不算新方法。
- 本CVA6无DMA snoop，已有FENCE.I/cache workaround并不提供Rdone时的源页归还同步。`target_ordering_proven`必须是明确付费证书，R/数据进入保护存储不等于全局IOFENCE。

## 准确前作与强普通

[RISC-V IOMMU规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)提供地址/context失效及CQ/order合同；PR/PW针对已processed访问的全局排序，尚未processed旧请求需额外封闭。已有target read/owner/visibility证明时，普通程序允许采用合法PR=PW=0的命令完成同步；不能凭当前native flags0自动认为DMA已排空。

[Intel VT-d Rev5.0官方完整原文](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf) §6.5.4确有domain/page相关排空；page保证涉及指定domain内untranslated地址与range重叠的non-committed主存请求。读drained是Root-Complex已从主存取完返回数据，不是DMA全页R receipt；raw/未processed与translated ATS还要另处理，实现也可能更宽排空。不能将page-selective drain说成没有前史。[原件/hash/编号导航](../out/local-read-reclaim-20261015/controls/critical-sources.json)／[root关键原文复核](../out/local-read-reclaim-20261015/root/primary-review.json)。

[ordinary_policy.py](../out/local-read-reclaim-20261015/controls/ordinary_policy.py)独立实现、不导候选/oracle，三个程序为：whole_copy等实际B后撤销；phased在source全读/old义务闭合后撤销；targeted_drain是同付费target证明上的普通定向程序。后者**不是在RISC-V上复现VT-d**，也没有将Intel RC drained冒充DMA receipt。

普通phased用自己的源阶段join、unmap/cache/preciseinvalidate/目标排序/ACK流程，能够在目标W/B和无关R尚pending时执行相同归还。资格、primitive、状态采样、firstVALID及error-retirement全部相同，不强迫普通globalquiet或禁止其字段复用。准确发表覆盖与本轮具名普通构造分别见[prior.md](../out/local-read-reclaim-20261015/controls/prior.md)、[完整普通程序](../out/local-read-reclaim-20261015/controls/普通程序.md)。

持久映射只在没有真实撤权要求时替代；原buffer reuse保存oldbytes但不撤销旧授权；scratch/CPU也需满足相同撤权与全完成合同并计存储/复制/cache/CPU成本，均未作为被故意限制的弱对照。

## 私有功能原型与新验证

[reclaim_protocol.py](../out/local-read-reclaim-20261015/prototype/reclaim_protocol.py)执行两4096B物理copy的实际值，保持original4096B每源buffer，不新增payload池。IOVA与PA分别保存，physical alias按PA+正式context/IOVA辨识；source旧代与CPU新代区分。PTE/cache/invalid-commit/normal-memory ordering是显式抽象primitive；old-owner ticket退休是外部owner关账观察，**不是真实cache/PTW/CQ/CPU-cache仿真或native执行**。其Python无界字段、日志和dictionary不证明有限硬件面积。

[八组/122项新功能断言及八条完整trace](../out/local-read-reclaim-20261015/prototype/functional-results.json)实际通过：

| 条件 | 结果 |
| --- | --- |
| source全部R，W/B未完成，无关R仍在途 | phaseACK可发；CPU实际覆盖源page4096B后，旧目的完整oldbytes不变，无关任务完成 |
| 独立phased/targeted | 正例全部event/值/流量相同；phased在身份交换、错误终结也完全复现 |
| whole_copy | 合法等目标B再release；没有强迫等无关R，不能用其保守等待证明创新 |
| heldVALID、旧raw/walk/OUT/normal/ingress | 保留header/VALID，相关旧owner非0时不得撤销/ACK |
| 已授权但未发第二burst | 计旧debt，seal后继续正常drain；不能假装源读完成或造成半页死等 |
| 不完整reader、PA alias、writer、登记/epoch缺项 | 不成功ACK；两个正式共享reader均完成后才可归还 |
| 撤销/同步/失效/order次序 | 缺阶段不得直接ACK；cache失效不代表排序完成 |
| 真实read error | drain后失败收据+源page隔离，旧W/B和consumer以error状态退休；禁止成功复用 |

正常trace的data R/W各8192B、1024拍，4AR/4AW/4B/2consumer，CPU源覆写4096B另计。错误trace R/Wtransport仍各8192B，真正WSTRB有效写4096B，失败源的4096B由共同zeroWSTRB sink清债；B在此read-fault案例实际都OKAY，sticky R错误决定failed consumer，不伪造B错误。隔离页保持明确owner和原4096B占用，后续原生恢复/再次回收未实现，不称生产无泄漏fault接口。

独立席的新策略检查为73,728 snapshots／221,184 calls通过；root未重跑这些检查。root只执行本轮值/所有权集成：第一次在ticket `kind`字段与日志参数冲突时报错；修正并补独立IOVA/PA alias表示后122项通过。没有旧模型/实验/validator/trace/build重跑。

## 完整成本与门裁决

[root完整账本](../out/local-read-reclaim-20261015/root/full-cost.json)与[独立账本](../out/local-read-reclaim-20261015/controls/cost.json)包含trusted registration/全alias/readers、seal/frontier、读计数/RRESP、所有owner关联和端口、PTE/cache/inval/order、成功/error/隔离/epoch、CPU通知与控制，以及全部数据和consumer退休。新每reader计数如用8B单位至少10bit表示0..512，如用byte至少13bit表示0..4096；合法复用需证明，没有宣称这是总位数。

在这套**共同选定primitive和控制实现**中，候选与普通phased/targeted操作相同，构造性逻辑差0；没有候选独有的更窄可实现表示。不是所有native adapter等成本、最小面积定理或实测Fmax。wholecopy可以从已证job完整退休推导部分条件，不能强迫它重复新增phase传感器。

PTE/CQ/cacheline元数据流量、CPU通知/cache维护、源ACK实际窗口、总新增位数、面积/Fmax和应用时间未测，保留null；这些工作不能因为模型primitive是一个event而计零。CPU覆盖的实际值工作已经参考执行，但不当实测CPU/DDR流量与延迟。提前归还收据不等于整体任务收益。

因此本LR1独立方法性能入口未通过：具体强普通已经同动作覆盖，没有完整成本优势或有符号净收益。**0新性能条件、0新native RTL/driver/ABI/FPGA/build**，不开始GPU/AI应用移植；不是全领域或所有生命周期方法否定。

## 交付与后继边界

正向保留阶段归还接口、完整值/旧新代验证、source accepted-parent frontier、准确主存排空与DMA receipt区分、真正Rdone→Wpending偏序、标准基线与原生接口缺项。SDH1、BR1、RCS1、R2、R107/C112等原收益关闭不恢复，页交换仍仅DMA协作卡。

继续IOMMU方法需要先提出一个普通phase＋selective drain确实不满足的具体需求，以及新的执行动作或完整可实现成本差；不把相同readreceipt/seal/invalidate脚本再换名登记source-only goal，不以native未补PR/PW/readonly/精确失效证明普通弱。仅完整收益存活后一个GPU/AI真正共享路径及FPGA关键验证；不自动转页交换或表征论文。

[稳定根裁决](../out/local-read-reclaim-20261015/root-result-acceptance.json)／[下一合同](../out/local-read-reclaim-20261015/next-goal-contract.json)／[本轮执行账本](../out/local-read-reclaim-20261015/root/execution-ledger.json)。8/9月资产、E2/E3边界、method88/M0/stage117完成方法继续保留；原立题窗口累计，goal结束不等于论文完成。
