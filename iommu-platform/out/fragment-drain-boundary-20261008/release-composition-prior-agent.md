# §41 延续：成功 copy 的释放组合合同前作审定

2026-10-08，Codex 亲自核全文并推演适用性。只查3份主源，另核1段 AMD 官方端点依赖说明；没有调用 DeepSeek／Claude，没有 run、build、production edit。§41 的11390个几何分类 greedy/brute 一致是根提供的已执行结果，本席没有重跑或把它当作新算法证明。

## 结论

**端点的 R→W 依赖、选择性停 source、通道分离、关闭新地址后排空、依赖图闭包，以及完整 R 响应预约，都已有直接覆盖或可由普通控制实现，不能作为新机制。** 针对本轮更窄的“允许已接收 source AR 的返回量超过 private B，成功 copy 暂停时要为排空继续哪些 destination，跨多层 W-owner 的最小 continuation 工作量是多少”，这3份全文没有给出同一精确命题。但没有找到该命题，不等于证明不存在前作，也不排除同信息普通分片／预约同效。

因而保留的只能是一份待证明的分析命题：**明确实际要释放的共享资源后，给出合法成功暂停的必要／充分组合条件和紧的 continuation 工作量界。** 如果最后只是可达图闭包、完整 R 预约，或普通 descriptor 分片的等价改写，就不能把它立为独立贡献。如果界包含之前分析省略的 held VALID、W-owner 前缀、parent/child 层级和 private parking，且在同总 payload/metadata 下证明一个普通强控制达不到的成本点，才有新增分析或机制贡献的根据。

## 版本与完整原文：可直接给根复核

全部本次下载文件位于 `out/fragment-drain-boundary-20261008/map-agent-sources/release-prior/`，`sources.json` 留存 URL、实下载 bytes、SHA256 与一次机构仓储404。每个成功 PDF 均已转成完整 `pdftotext -layout` 文本；没有执行其附带代码。

| 主源 | 官方版本、日期和 URL | 实际本地全文与页数 | 精确原段 |
| --- | --- | --- | --- |
| **Physically-Aware Preemptive Virtual Channels for Deadlock-Free AXI Networks-on-Chip**；Lorenzo Leone、Luca Colagrande、Luca Benini | arXiv **2607.01430v1**，2026-07-01 19:47:42 UTC；[官方版本页](https://arxiv.org/abs/2607.01430v1)，[固定 v1 PDF](https://arxiv.org/pdf/2607.01430v1) | `preemptive-vc2026.pdf`、`.txt`；实下载370487B，PDF **5页**；txt24有v1/date盖戳。arXiv摘要comments写4页，保留差异，不用其页数替代实下载页码 | §I、Fig1，PDF页1 txt65–84：DMA返回R逐beat送本地W、外来W占路径形成端点环；同页txt49–64：R/W分离、下游有空间才占link；§II页2 txt156–182：静态消息类分离与先行关系 |
| **Scalable AXI4 Transaction Monitoring for Mixed-Criticality SoCs: From Phase-Level Precision to ID-Level Efficiency**；Chaoqun Liang等 | arXiv **2608.30435v1**，2026-08-31 08:26:08 UTC；[官方版本页](https://arxiv.org/abs/2608.30435v1)，[固定 v1 PDF](https://arxiv.org/pdf/2608.30435v1) | `axi-monitor2026.pdf`、`.txt`；实下载968443B，PDF **14页**；txt12有v1/date盖戳 | §III-A Fault Management，PDF页6 txt485–500、518–527：fabricated error、接受后回收；§IV-D **PDF页9** txt796–857：固定drain集、Eq7与接受速率条件，ILT的额外RLAST合同 |
| **A Design Flow to Securely Isolate FPGA Bus Transactions in Heterogeneous SoCs**；Niko Salamini等，RTAS2025 | [作者官方 PDF](https://retis.sssup.it/~a.biondi/papers/RTAS25.pdf)；PDF元数据创建日2025-04-03；不是arXiv稿 | `rtas-isolation2025.pdf`、`.txt`；实下载9652076B，PDF **13页** | §V-A页5 txt271–310：固定属性与静态ID/translation regimes；§V-A2页6 txt352–373、页7 txt374–406：AIM的FIFO与ID映射行为 |

两个 arXiv 的标题、作者、v1/date 既从官方版本页核，又从实际下载 PDF 的标题／盖戳核。上表页码只按本次实际 PDF formfeed计数，不混用网页渲染页索引。监控论文的 Fault Management 在本次 PDF 第6页；此前即时消息曾估成第5页，已更正。

SHA256：

- `preemptive-vc2026.pdf`：`6a3188b56b64c8e31095219940cb97e762f7496ccbbd560c7dd7ae802c373ea0`。
- `axi-monitor2026.pdf`：`4d4cf0d52a823f6f24d6238783b3c2702b2fb676d787d89bab65f09d8f2adec9`。
- RTAS25的SHA见同目录 `sources.json`；先尝试的 EHU 仓储 live404，改用已核作者URL成功，不把缓存命中说成实际下载成功。

## 主源1：端点依赖环被直接覆盖，未覆盖成功暂停的 continuation 优化

NoC2026的 Fig1 就是 DMA 返回 R 逐beat继续 W，外来 W 占共享路径后使 R 无法继续的反例。它把协议／端点依赖与路由无环区分开，并用读写类分离、每flit下游空间条件解除共享link阻塞。其贡献在宽link VC物理成本，不在首次发现 R→W 依赖。本轮不能把类似环、多层 owner 图或增加escape通道重新命名后立新意。[原文§I](https://arxiv.org/pdf/2607.01430v1)

它没有定义预算关闭后的私有parking B、正常copy继续哪些future AW、或最小继续工作量。这个范围差别不构成正结论：如果普通VC／独立物理路径已让目标共享资源回收，就必须按其实际payload/metadata成本比较；反过来，通道分离不能替一个明确禁止future AW的gate发出必要AW。两份合同应分别审。

AMD 的 [PG313 NoC Deadlock Avoidance](https://docs.amd.com/r/en-US/pg313-network-on-chip/NoC-Deadlock-Avoidance) 更直接把“端点不存在外部信号完成依赖”列为NoC无死锁适用条件；其双输入组合器例子给出小FIFO填满导致另一流堵塞，建议分离route/VC，或 near-full反馈只停填满FIFO的source、保持另一source让它排空。这是普通选择性停source的直接反证。这里不扩展成新文献线。

## 主源2：固定集合的 cut-and-drain 界已有，错误完成与成功copy合同不同

监控2026的cut冻结新AR/AW，drain合成SLVERR。§IV-D令切断时在途写／读数为Nw/Nr、最大burst beat数为Lmax，给出 `Tdrain ≤ max(Lmax+Nw, Nr·Lmax)`；原文明确每通道每cycle一个 fabricated beat **被manager接受**。PLT/CLT仍补满读burst，ILT单beat RLAST另要求manager按RLAST而不是ARLEN计数。[原文§III-A／IV-D](https://arxiv.org/pdf/2608.30435v1)

这直接覆盖“关闭新地址、固定在途集、按容量和burst长度给drain界”，不能拿来包装新分析。它靠错误完成替代真实payload；接受速率是原文条件，不能把界改读成任意小FIFO streaming manager的无条件成功排空界。如果系统允许copy失败并能独立丢弃／接受错误R，普通故障完成是强控制；如果本轮要求暂停后原copy仍成功，就必须保留真实数据与未来W义务，不能仅因去掉了错误路径便宣称新颖。

本轮未复现其监控实验，也没有核RTL的manager-error接收实现；特别不将单beat提前RLAST移植到当前iDMA后声称协议兼容。

## 主源3：RTAS AXI隔离本身已有；其隔离对象与本题不同

RTAS25 §V-A把隔离实现为固定AxPROT/QoS/CACHE、以AxUSER区分HA并分配独占AXI ID池，让IOMMU选各HA的translation regime。AIM具有独立请求／数据／响应FIFO，缓存并恢复原ID。没有预算停发后的成功copy释放合同，也没有source已接收R到future destination的最小工作量命题。[作者原文§V](https://retis.sssup.it/~a.biondi/papers/RTAS25.pdf)

不能因为标题含isolate便断言它已证明本题，也不能把AXI ID空间隔离、受保护属性、IOMMU配合或FIFO组合当作本题的新方法。这里是空间／访问隔离与动态资源排空对象不同，不是本题突破了RTAS隔离机制。

## Codex 对精确命题的独立边界判断

目标应限定为固定合法映射的成功copy内让路。允许private parent metadata继续占用，要求集合Q中的共享W/R或route状态释放。若只是tail child quiet，就不得要求私有parent同时结束；若parent的W-owner token位于共享队列，token仍要释放，即使其描述符metadata可以私有保留。

在暂停触发状态s，至少分别记录：

1. 隔离点前已经首次VALID、不可撤回的AR/AW；以及隔离点后真正已接收的请求。二者不能用同一个“在途”计数混合。
2. 各DMA有效byte/lane occupancy、尚未返回的R及其范围。B只包括可以不依靠Q继续独立收完的停车容量，不能把仍持有Q路由的响应spill算进private B。
3. 每层W-owner FIFO的已承诺顺序、每个AW所需的W范围与WLAST/B完成义务；一个destination在上层未承诺，不代表下层没有前置owner阻塞。
4. 可继续的必要future请求集合与禁止的新普通source工作，映射和权限仍合法；每层parent/child最后响应与token的对应关系。

精确问题是：在这些真实状态下，是否存在不撤回VALID、不丢payload、不违反W/同ID顺序的有限合法序列，使Q释放；若存在，最少新增AR/AW、R/W bytes或物理bursts是多少。写清三种工作单位，不能把时间、bytes、bursts相互替换。

普通闭包只表达“若要完成x，需要y”的必要前置；它本身既不证明容量可行，也不证明队列顺序下存在可执行schedule，更不证明最小成本。例如“park R”与“继续W”是可选动作，是否可park由实际lane空间决定；放行某DMA必要W，可能先要求完成另一DMA的既有owner前缀。若最终用有限状态搜索求一条路径，那仍是普通reachability方法。只有把这些条件压成有新内容、紧而可审的组合界，才可能成为新增分析；“我们第一次画了图”不能。

这个问题与2018 local-SPM/miss-retry、DMA350 full-R/full-W和AXI-REALM的适用差别已经记录在 [map-boundary-agent.md](map-boundary-agent.md)。不再重复其全文读取；这些差别不能自己证明新颖性。

## 强控制与退出规则

| 必须比较的普通控制 | 同合同要求 | 否决候选的结果 |
| --- | --- | --- |
| 同B的小burst＋完整R响应预约，完整W后发 | outstanding R的预约不可重复；按lane/有效bytes计全部payload；成功copy可恢复；同Q | 在同总payload/metadata和相同释放工作量下同效，就不能把continuation许可作为独有机制 |
| software tiling／普通非均匀descriptor分片 | 给相同src/dst边界、页信息、同B和暂停需求；计descriptor供给与两侧物理bursts | 若同cuts与同成本实现相同证书，几何／软件分片等价否决新算法 |
| AXI-REALM式透明child splitter／普通动态限长 | 允许保留私有parent；真正共享parent token仍计Q；计全部W buffer与metadata | 若child quiet已让Q可服务其他DMA，不得强加parent全退来制造优势 |
| 普通VC／独立返回通路或独立payload parking | 相同Q、同总存储与owner/order信息 | 若已解除Q占有，不能把复述endpoint依赖或多层图当新机制 |
| fault/error completion＋丢弃／排空 | 单列copy可失败的合同，不与成功copy数据保持混算 | 若应用允许失败，错误路径直接解决问题；成功copy额外要求应公开计价 |

值得保留的最小可判命题：**在给定s、Q、B和固定owner顺序时，强普通方案能否用同资源实现最小成功continuation；或者，是否存在一个严格低于“全部已在途R都完整预约”的必要停车／继续工作量边界，同时保证多层W-owner真正释放。** 本轮没有证明这种严格新成本点，没有找到相同精确命题的这3篇覆盖，也没有完成足以排除所有相关前作的全域新颖性证明。

因此停止继续扩检索。下一审定应先把一个真实状态s、Q和强普通方案列出来；如果证明归约为普通分段／响应预约／图闭包，保留为平台解释与对照，退出独立贡献路线。若只有人为长期关闭本可恢复的budget才出现反例，或Q定义把合法私有metadata误算为共享阻塞，也应退出。绝不因这3篇没有该公式就宣称已经有论文gap。
