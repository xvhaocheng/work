# §47 合并服务归属与共享替换历史：关键前作审核

2026-10-08；本席 Codex 亲自核原文、提出反例与强对照。未调用 DeepSeek/Claude，未执行 DUT、数学实验、编译或生产修改。这里只保存研究分析及公开 primary 副本；结论不是仿真或板端结果。

## 判定先行

1. **不能把晚加入的合并 miss 受益者提高共有服务紧迫性立为原则创新**：Li 2015 已给后续 merger 发更新消息、改变 DRAM 排队请求 score，以及所有 waiter 的累计年龄。不把它夸成 per-DMA deadline/fair-work 定理。
2. **不能把替换历史与实际 resident 分开、把复用收益归属来源、或用使用区间纠正近期历史立为原则创新**：UCP、SHiP、Hawkeye 分别有明确的字段和更新规则。三者的“来源”不同，不能混称 requester history，也不能因它们没有 DID 字段就判定新。
3. **descriptor/job 暴露未来用途可以成为新的可用信息，但信息输送和 pending-consumer 保护已有直接 IOMMU 前作**。值得低成本判别的是：能否用真实 translation materialization 进度形成正确、紧凑的多 DMA 剩余用途信息，并在相同信息/总状态下改进实际共享替换。仅改成 job ID、软件预装或锁住到消费，不构成独立机制。

本席精读以下 5 项原作的关键段；不扩大检索。root 补充的两项直接 IOMMU 专利另列为交叉证据，未冒称本席另读全文。

## 可导航的 5 个主源

所有路径均相对本目录；绝对目录为 `/home/xvhaocheng/work/iommu-platform/out/translation-sharing-qos-20261008/`。PDF 页码从 1 开始；txt 行号用 Unix newline 计，不能把 form-feed 当额外行。下载 URL、SHA256、版本事实边界见 [primary/sources.json](primary/sources.json)。

| 原作 / 实际使用版本 | 原文位置与本地导航 | 精确 primary URL |
|---|---|---|
| M. K. Qureshi, Y. N. Patt, **Utility-Based Cache Partitioning**，TR-HPS-2006-009，2006-09-25 | §3.1 PDF p6，[txt:249](primary/ucp-tr2006.txt:249)；§3.2 p7，[txt:324](primary/ucp-tr2006.txt:324)；§3.5/3.6 p10，[txt:452](primary/ucp-tr2006.txt:452)、[txt:469](primary/ucp-tr2006.txt:469) | [作者 HPS 原始 TR](https://hps.ece.utexas.edu/pub/TR-HPS-2006-009.pdf)；本地 [PDF](primary/ucp-tr2006.pdf)。这是 24 页 TR，未冒用 MICRO 版页码。 |
| C.-J. Wu et al., **SHiP: Signature-Based Hit Predictor for High Performance Caching**，MICRO 2011 | §3.1 PDF p3，[txt:144](primary/ship-micro2011.txt:144)、[txt:151](primary/ship-micro2011.txt:151)、[txt:163](primary/ship-micro2011.txt:163)；§3.2/4.1 p4，[txt:186](primary/ship-micro2011.txt:186) | [ISTC 原始论文](https://istc-cc.cmu.edu/publications/papers/2011/SHiP.pdf)；本地 [PDF](primary/ship-micro2011.pdf)。 |
| A. Jain, C. Lin, **Back to the Future: Leveraging Belady’s Algorithm for Improved Cache Replacement**，ISCA 2016 | §3.1 PDF p4，[txt:239](primary/hawkeye-isca2016.txt:239)；§3.2 p5，[txt:351](primary/hawkeye-isca2016.txt:351)；§3.3/3.4 p6，[txt:464](primary/hawkeye-isca2016.txt:464)；Scheduling Effects p11，[txt:1247](primary/hawkeye-isca2016.txt:1247) | [作者 Calvin Lin 原始论文](https://www.cs.utexas.edu/~lin/papers/isca16.pdf)；本地 [PDF](primary/hawkeye-isca2016.pdf)。 |
| Dongdong Li, **Inter-core Locality Aware Memory Access Scheduling**，UBC MASc thesis，2015-04 | §4.1 印刷 pp22–24/PDF pp35–37，[txt:1395](primary/intercore-thesis.txt:1395)、[txt:1460](primary/intercore-thesis.txt:1460)；§4.3 印刷 pp26–27/PDF pp39–40，[txt:1567](primary/intercore-thesis.txt:1567)、[txt:1604](primary/intercore-thesis.txt:1604)；§4.4/4.5 印刷 pp27–30，[txt:1647](primary/intercore-thesis.txt:1647)、[txt:1738](primary/intercore-thesis.txt:1738)、[txt:1772](primary/intercore-thesis.txt:1772) | [加拿大国家图书馆保存的作者原始 thesis](https://central.bac-lac.gc.ca/.item?app=Library&id=TC-BVAU-52650&oclc_number=1033019633&op=pdf)；本地 [PDF](primary/intercore-thesis.pdf)。不是四页 CAL 2016 论文；官方作者 [publication list](https://people.ece.ubc.ca/aamodt/publications/) 链接 [CAL DOI 10.1109/LCA.2015.2435709](https://ieeexplore.ieee.org/document/7110523/)，未取得该四页全文。 |
| AMD, **US20250190351A1 Address translation preloading**，公开日 2025-06-12，申请日 2023-12-12 | 原公开段落 [0036]–[0038]、[0041]、[0047]、[0054]–[0056]；对应 [逐段 HTML-derived txt:36](primary/US20250190351A1-description.txt:36)、[txt:38](primary/US20250190351A1-description.txt:38)、[txt:41](primary/US20250190351A1-description.txt:41)、[txt:54](primary/US20250190351A1-description.txt:54) | [原始公开专利正文](https://patents.google.com/patent/US20250190351A1/en)、[原始 PDF](https://patentimages.storage.googleapis.com/b5/29/6c/a0c78c17b0a125/US20250190351A1.pdf)；本地 [HTML](primary/US20250190351A1.html)、[PDF](primary/US20250190351A1.pdf)。PDF 为 13 页扫描图，pdftotext 没有文字；txt 来自 HTML，不冒称 PDF OCR。 |

Library thesis 下载返回 818 字节历史 HTTP 头加 PDF；[原始响应载荷](primary/intercore-thesis.http-payload)保留，正常 PDF 仅去掉 `%PDF-` 之前的封装。首页明确 April 2015；不用搜索引擎的抓取时间作论文日期。

## 字段与更新规则：究竟覆盖什么

### 合并服务不是第一次请求的静态属性

Li §4.1：首 miss 分配 MSHR、发送真实 DRAM 请求；后 miss 合并，同时发送 dummy request 更新已排队的请求。§4.3 使用 `AgeReq=t−creation_time`、`AgeMSHR=Σ AgeReq`；共有请求的年龄每周期增加 merge length。§4.4 的 MIB 存 block address、merge length、age、valid；更新消息与正常 fetch 分开。§4.5 还给 4 位 merge length、15 位饱和 age 和 valid 的成本。

**直接覆盖**：晚加入者改变共享请求优先级；共有服务按累计等待者紧迫性排序；合并规模、年龄与控制消息共同计费。**没有从这些段得到的结论**：按不同租户而非 merge 数收费、每 DMA 最低服务份额、最早 deadline 保证、饱和后无限到达下无饥饿的数学界。论文提出 anti-starvation 目的及实验，不能把 15 位饱和 counter 当硬保证。

### 同一 hit 的三个“来源”不能混用

- **UCP**：每 core 的 ATD/UMON 独立观察该 core 的访问，估计不同容量下的 miss savings；实际 cache tag 保存 installing core，并按 quota 选择 owner 内/外 victim。这里的 counterfactual utility、resident allocation owner 是两层状态。不能由 quota 自动作出“别人命中该 entry 就该/不该更新 owner 历史”的唯一最优判断。
- **SHiP**：entry 保存 insertion signature 与 outcome；任何后续 hit 增加该 entry **保存签名**的 SHCT。signature 可取 PC/地址区域/指令序列；hit promotion 仍用 RRIP 的普通规则。它有明确的“后续复用给填入来源记收益”，不是当前 hit requester 独立隔离；也没有精确 remaining-use count。
- **Hawkeye**：sampler 保存地址、最近 PC 与时间，OPTgen 用过去使用区间和容量重叠判断应不应该缓存；训练最后访问 X 的 PC，而当前访问 PC 产生预测并更新 RRIP。它已直接使用 liveness interval。Scheduling Effects 又明确过去最优受未来非确定 interleaving 限制。

上述核心字段可分别换成 DMA class 或 lookup source 作**普通适配基线**，但要显式定义语义：DID、共有 translation key、fill owner、hit requester、job epoch 并不相同。也不能把 UCP 在 SPEC multiprogrammed workloads 上的结果冒称 same-SCID 多 DMA 共享翻译实测或性能保证。

### 已有软件未来信息不只 prefetch

AMD [0036]/Fig5 保存 VA/PA、DID/PASID、权限/enable 等；[0038] 已允许去重、覆盖优先级及禁止覆盖特定条目；[0041] 描述短暂 DMA 的 map→DMA→unmap；[0047] 软件选择近未来预装时机。因此“软件知道即将访问页、送 IOMMU、保护条目”原则已被覆盖。

这些具体段/字段没有实现一个 `job(epoch,src,dst,len)` 的剩余翻译用途集合，也没有 translation-materialization/frontier 消费反馈。**这只说明本源的精确机制边界；不以缺字段或未搜到作新颖性证明。**

## root 补充的更直接 IOMMU 反例

根席已亲核并保存 [root-primary-browser-evidence.json](root-primary-browser-evidence.json)。本席仅交叉读其中指定原文片段，未另取/读两个 PDF 全文：

- [US20230401160A1，Address translation prefetching for input/output devices](https://patents.google.com/patent/US20230401160A1/en)：`setup_find` 的原网页 L352–356、683–685，DMA stream setup 可以同时发送给设备/IOMMU，甚至广播同一 setup command。因此 descriptor/setup 送 IOMMU 本身已有直接前作。
- [US20070067505A1，Method and an apparatus to prevent over subscription and thrashing of TLB entries in I/O virtualization hardware](https://patents.google.com/patent/US20070067505A1/en)：`pending_window_and_setup_open` L153–169、218–226；per-port allocation/deallocation windows，walk 完成后 lock-down，top-of-queue 消费条件下转回 LRU。普通 later hit 不一定释放。它已经区分 page-walk 完成与 pending 消费，并含多个 root ports 的资源管理。

因此禁止把“保护到第一次需要它的翻译消费者用完”“descriptor sideband”“每 port 一窗口”作为新原则。JP2025156909A 只有根席搜索片段，没有成功全文，本稿不据它确认算法。

## 亲自推演：合法共享 hit 的性能冲突

考虑两个 V 可分配 ways，存共有翻译 x/y；x 比 y 旧。A/V 的 DID 不同，但该 key 的 PSCID/root/VPN 合法相同。A 可以命中 x，之后 V miss z。这里 lookup 能命中共有 entry；分配 mask 仅约束 z 的 victim，不能把 A alias hit 当非法。以下按两 way LRU 给最小纸面例；生产 PLRU 的 mask/树位需要 bench 另核，未冒称已测 victim。

| 完全相同的历史前缀 | 下一次真实用途 | resident-way touch | requester-only filtering |
|---|---|---|---|
| x 旧、y 新；A hit x；V miss z | V 随后只用 y，A 不再用 x | A 把 x 提为新，z 驱逐 y，V 多 miss | x 未提升，z 驱逐 x，V hit y |
| 同上 | A 随后只用 x，V 不再用 y | 驱逐 y，A hit x | 驱逐 x，A 多 miss |

这证明**无条件 requester gate 与无条件 resident promotion 不能因身份语义自动成为整体性能最优**，不是证明任意普通算法不可达到好点，更不证明前作错误。UCP 的 utility、SHiP 的来源预测、Hawkeye 的过去区间均是应加入的动态普通对照。如果两个未来只在尚未提交的工作中不同，硬件现有信息甚至不能区分它们；人为喂候选未来、不给对照，会制造收益。

该冲突只需串行 translation lookups 和实际替换，本平台单 PTW 也能到达。它不依赖多 walker、慢 response subscriber 或新的合并器；§44 已核正常 TC 接受请求后 naturally ready，本稿没有制造其 ready=0。

## 亲自推演：合并的物理与逻辑账不能强行同一个标量

假设可配置的新 coalescer 将 A/V 的同一 PTE miss 合并：A 先到，V 后到；物理 PTE 只读一次。将一次物理工作全扣首 requester，使晚加入者未付费；给每 waiter 全扣一次，则逻辑收费总和超过实际物理 work。这是两个计费合同，不能仅由等式不同判断哪个错误。

强普通构造用两个账本：physical ledger 精确记录一次 fetch；per-DMA logical ledger 记录该 DMA 的参与/收益或独享等价 work，并在队列选择时使用该账本。共有 fetch 取 max priority/min deadline/累计 age 时，仍计 waiter membership、首次/最后时刻及 frozen cohort；“一个 DMA 重复加入 n 次”是否增加 tenant entitlement 必须明确。普通 pairwise 分摊、独享等价收费、每租户一次记账、有限 sealed cohort 都要允许，不能只对比 first-owner 全扣。

这没有提出新公平算法，也没有声称 Li 已实现以上账本。它给的是具体 ordinary construction：若候选仅换 charge denominator 或 subscriber priority 聚合，不应据此独立立新。若要做硬 SLA，需明确到达限制、shared key/version、不可撤 PTE tail、in-flight 加入规则及响应容量，不能从 throughput 图推导 deadline。

**平台门槛**：当前单 active PTW 没有多个 live walk 可供 continuation 重新排序；NB-Walker 的共享 PTE/Info 构造是前作/新原型对照，不是本平台生产已有 coalescing。更改已接受 AXI AR 的 priority 也不等于能改其已承诺服务。当前这条支线先停，不为抽象 inheritance 立 RTL 项目。

## 唯一值得留作低成本判别的信息合同

root 的窄候选是已注册 DMA job 暴露**剩余 translation lookup 用途**，而非笼统未来或数据完成。可定义 `L(e,d)`：device d 的已接受 job 中，仍可能需要翻译 key e 的、尚未 materialize 的片段是否存在；entry 保留信号取所有 d 的 union。某 d 的最后一次 lookup 结束并不代表其他 d 已不需要 e；物理数据尚在 R/W 途中也不自动代表仍需要 IOTLB entry。

本席提出的审查规则是：只根据能证明的 `L(e,d)` 区分 dead-for-this-consumer、shared-live 和 no-known-remaining-consumer；最后一种只作为 eviction hint，不能撤销映射或保证未来未注册 job 不 miss。该规则具有可执行语义，却是普通 remaining-use/refcount 的具体化，**单独不能立原则创新**。2007 专利已区分 walk 完成与 translation consumer 使用，所以 materialization 比 data retirement 早这个区别本身也不是新原则；不能将其窗口直接夸成任意多个 job 的精确剩余用途集合。

如果 root 能提出新的紧凑 materialization 接口/状态表示，准确处理 src/dst 切片、乱序 holes、accepted-but-not-materialized 请求与跨 DMA shared-live，再用其信息制定实际替换规则并获得明确整体收益，可以作为 modest 微架构候选继续；贡献应落在具体反馈和决策，而不是“DMA 也能提供未来信息”。本次原文审核没有证明这项独立贡献已经成立。

### 必须同信息、同资源的强普通对照

1. 现有 resident history、实际 requester filtering、共享 RRIP/普通替换，并保留合理 capacity/admission 方法。
2. 按 requester 的独立 ATD/utility；SHiP 式 stored-fill-signature training；Hawkeye 式历史使用区间。缩放到本平台可用总状态，不能免费送候选信息或把 MB-cache 的原始 overhead 硬搬成唯一对照。
3. 拥有相同注册 job 信息的普通 page/use refcount、interval/range intersection、last-translation-use release，以及 per-consumer pin/protection union；2007 allocation/deallocation-window 方案须作为消费保护基线。
4. software tiling/descriptor ordering 和普通 pending-request 信号。若候选需要额外 epoch、每 entry mask、每 job range 或 holes bitmap，全部计入总 metadata；consumer/waiter Info 在 MSHR 或 response queue 里仍须计费。

未知 DMA 间交错不能免费变成 total next-use 顺序。用绝对未来次序最远的 entry 作 victim 是普通 next-use/Bélády 对照；如果只知道每 DMA 单调顺序而不知道合并顺序，应说明算法的保守性及实际近用/远用误判。

### 最小决定性规格与退出条件

先复用合法同 PSCID/root 的串行 lookup，构造上述 x/y/z 两个未来分支；增加已注册 job 的最后 translation 与长数据尾部错位，以及 A dead/V live、相同 job 的 src/dst 再访、materialized holes。观察完整正确 response、IOTLB miss/victim、V/A 共同完成时间和 total metadata。只做替换时不需要引入 coalescer；本席未运行。

若相同 job 信息下普通 interval/refcount/last-use 释放产生相同保留与 victim 序列，停止机制命名；若收益只来自更大状态、候选独占 future 信息、把数据尾部错误当翻译未消费或弱 history baseline，同样停止。若候选区别是新的可达、紧凑反馈接口以及具体非等同替换决策，就按这些差异审查面积/延迟/收益，允许 modest 算法贡献，不追加“所有可想象普通算法数学不可能”的要求。

## 本轮接受边界

没有找到一个可据此认定的新 coalescing QoS 原理或 history-attribution 原理。因为具体强前作与普通账本覆盖泛称机制，这两支停止；不是因为“论文标题像”。descriptor-materialization 合同仅保留为根席待判规格；未来用途接口、pending-consumer protection 和通用 last-use 释放已被强普通对照覆盖，不能直接写成新机制。未作论文分区承诺。
