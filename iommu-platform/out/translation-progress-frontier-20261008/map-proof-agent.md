# §45 独立 primary 审核：持续服务、选择性续走与重计算

2026-10-08。Codex 亲自读取原文、推演与 root/rules 交叉质疑；本席只读取3项 primary，未复读 Rocket 字段、未调用 DeepSeek/Claude、未运行数学模型/DUT/build、未改 RTL。root 的有限图运行与结果由 root 独立记录，本稿不将纸面分析或下载原文称为实验通过。

**结论：停止把已知固定链的 checkpoint 容量—重读—quantum 曲线立为新理论。** root 的固定两请求模型中，选择 checkpoint 使相邻间距不超过 q 就可持续推进；普通提前 yield 不必 restart。下面原文已有部分路径保留、跨层共享槽、按重载代价保留以及 nested G 子 walk 结果复用，不能只与 whole restart/full snapshot 两端比较。

真实未知路径、变长阶段与多个 context 的 online 边界未被本轮证明。**不能因为这3项原文没给所需完整 frontier，就断言其他文献没有，也不能因为推广到 online 更难就认定创新。** 本轮没有提出一个经过强普通对照后仍成立的新规则。

## 1. 实读的3项 primary 与精确范围

所有本地文件位于 `/home/xvhaocheng/work/iommu-platform/out/translation-progress-frontier-20261008/primary/`；`sources.json`记录真实取得的 URL、bytes、SHA256。`*-raw.txt`按PDF对象顺序提取；不带raw的txt采用layout，**两者行号不可混用**。PDF页码均指从1开始的文件页码，部分论文另有出版页码。

### A. Barr、Cox、Rixner，ISCA 2010

**Translation Caching: Skip, Don’t Walk (the Page Table)**。[官方作者PDF](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)。本地 `translation-cache2010.pdf/.txt`，沿用已有原文副本；共12页。

- §3.2/§3.3，PDF p4，layout txt `260–296`：以虚拟前缀标记部分翻译；最长匹配跳过上层；path entry可保留多个中间PPN。
- §4.2 Partitioning，PDF p5，`298–357`：统一槽可由不同层共享；按重新加载代价保留更有用的层，给modified-LRU/可变插入点。
- 表1/2，PDF p5，`362–385`：明确区分tag与data成本；示例partial translation data40bit，tag9/18/27bit，path data120bit。
- §5.2，PDF p6，`416–427`：模拟器只保存tag，不保存实际PA，用于该论文的cache行为比较。

**覆盖：** 不同深度共享容量、选择性保留进度、按重取成本保留、减少重新读取。**没有直接给出：** 所有nested权限/fault状态的bit成本、被持续暂停的多设备请求完成保证或三轴最优曲线。其抽象成本表不能直接当当前IOMMU的完整token宽度。

### B. Bhargava、Serebrin、Spadini、Manne，ASPLOS 2008

**Accelerating Two-Dimensional Page Walks for Virtualized Systems**。[正式DOI](https://doi.org/10.1145/1346281.1346286)；下载自[原论文的学术镜像](https://www.cs.columbia.edu/~cdall/candidacy/pdf/Bhargava2008.pdf)，不是摘要转述。本地 `nested2008.pdf`、`nested2008.txt`、`nested2008-raw.txt`；10页，出版p26–35。

- §4.2，PDF p5–6，layout `374–408`：NTLB以guest PTE的GPA为key缓存G子walk叶映射；命中后直接得到guest-PTE HPA，跳过完整nested子walk。
- 表2，PDF p6，raw `768–776`：默认24-entry PWC、16-entry NTLB，均fully-associative/LRU、2-cycle access。
- §5，PDF p6，raw `733–753`：single-thread模型，并明确未含multi-processor/multi-guest效应。
- §6.4，PDF p8–9，raw `1114–1130`：PWC/NTLB容量比较及包含末级guest entry的策略变化；不同容量在该实验中采用同访问延迟。

**覆盖：** 完成一个G子walk后保存其结果供后续S访问、nested部分路径与结果复用、具体容量比较。**没有直接给出：** q=3暂停合同、78bit帧或严格lossless fault输出证明；cache采用LRU而非给每个暂停请求锁定一项。不能把平均cache命中当强暂停保证。

### C. Hong、Kung，STOC 1981

**I/O Complexity: The Red-Blue Pebble Game**，DOI `10.1145/800076.802486`。[原论文PDF镜像](https://perso.ens-lyon.fr/loris.marchal/docs-data-aware/hong_kung_red_blue_pebble_game_STOC81.pdf)。本地 `hong-kung1981.pdf/.txt`、`hong-kung1981-raw.txt`；8页，出版p326–333。

- §2，PDF p2，raw `85–143`：固定DAG；red/blue表示fast/slow存储；R1/R2读写、R3计算、R4释放；fast为S words，slow任意大，目标计R1/R2次数。
- Theorem3.1，PDF p3，`202–209`；Lemma3.1，PDF p4，`293–301`：通过2S-partition给出I/O下界，通常写为 `Q ≥ S(P(2S)−1)`；旧扫描OCR有字符损坏，公式核查应看PDF。

**覆盖：** 保存、删除、重取和重计算随容量变化的一般范式，证明“有限状态换额外工作”并非新主题。**不能直接套用：** 它按word而非phase-dependent bit数计容量，没有到达/服务quantum/每请求完成时间；任意slow存储允许写出中间结果；已知DAG中的输入访问也不等于必须先得PTE指针的online walk。因此不以pebbling泛论否决一个尚未给出的nested输出合同定理。

## 2. 与 rules 的中间普通点交叉核对

以下是双方根据当前RTL合同构造的baseline，不是给上述论文伪造硬件字段或暂停保证。双方共同计原请求/DC/PC/根/身份C、valid/token、active与在途状态、terminal错误/结果。PAddr56/GPA41、稳定mapping、当前无A/D写；future S leaf可能fault，不能为all-normal oracle免费删旧guest target。

| 普通点 | 保存与恢复 | 对应primary能力 | 成本/保证边界 |
|---|---|---|---|
| q=3：G完整结束后保存，下一turn读S | 保留guest表target T与G映射Y、Sglobal；不必重读G | Bhargava NTLB保存完成的G映射，命中后读S | 保存位置已有明确语义先例；同fault合同需T。rules广scope静态tag上界78bit，固定三层路径77bit；论文没有这些数字。 |
| q=2：G-root后提前yield | 保存当前G table Q；下turn G2/G3；S读后可再做下个G-root | Barr partial translation以prefix保留某深度table PPN | 可在q前主动yield；ordinary checkpoint placement，不只允许“用完q才切换”。rules上界81bit，不是最小codec。 |
| q=1：逐PTE保留 | 保存父target、当前table进度、phase/level | 部分路径/continuation范式已有前史 | §44广scope上界82bit；固定全三层路径可减少不可达phase，81bit；不能把不同scope当同一面积结果。 |
| 小帧G子walkrestart | 保留S父/叶，丢当前G进度，恢复从G-root重读 | 子任务重发＋选择性保留 | 36bit另计C等，单次多≤3读；要允许完整G+S量子或限制暂停次数才持续完成。 |

这不是说一个LRU cache天然满足同样的q保证；ordinary可以把具体request的已验证结果放入受保护slot，或用同信息的context indexed临时项。**这项保护/选择是构造的一部分，必须计成本，也不能仅因为原文没有pin位就把保护叫新机制。**

q3是最强的直接对应：已完成G结果被保留后，后续S访问不用重走G。q2对应的则是某层table PPN，而不是完整叶映射。这两种状态语义不同，baseline不能只提供最终translation cache然后宣称所有partial progress都被覆盖，也不能排除它们使候选获得人为优势。

## 3. pinned PWC、共享live槽与成本的实际映射

PWC/NTLB必须纳入等总存储比较，但不能免费加入：

- 通用cache需要GPA/前缀key、地址空间/世代、level、data、valid与必要权限。2008/2010表中cache项不是一个免费的44bit Q。
- 若改成仅服务一个已accept request的临时项，key可用C slot ID与phase/level，target由保留的T/IOVA重建；这是ordinary specialization，仍要计slot保护和恢复路由。
- pin已完成G映射可实现q3的续走；pin某层partial translation可减少q2重读。只pin根或只pin完整叶不能同时保存所有阶段状态，必须按真实live data选择。
- 当前active寄存器与suspended存储都计入总容量。一个执行slot与一个停车slot能交换状态；不能将active存储在一方计费、在另一方免费。跨phase字段共享也不能把同一时刻两个不同request的独立PPN装进一个槽。
- cache命中仍有lookup/权限检查/仲裁时间。只按PTE事务计q时，命中可减少外部读，却不是零周期；转换成延迟需要计cache服务和memory服务。
- 若允许把continuation写入DDR，这是另一普通空间—访问点。Hong–Kung允许任意slow存储；本平台当前PTW不写，不代表理论对照永远不能spill。若研究只限定on-chip/read-only walker，必须明示范围，而非声称全架构frontier。

固定mapping使重读在语义上可重复，但future PTE的返回和结果输出合同仍保留：旧T不能因guest-HPA alias或当前访问正常就删除。对应S high-PPN fault的旧/新target差异已经在§44原文审定；本轮不重复逐字段阅读。若允许skip-fill/direct result而非同IOTLB内容，可以进一步改变必要字段，必须用同一输出合同比较。

## 4. 一步后续边界：未知路径可以研究，但不足以立创新

固定链结论不能自动推广成未知PTE内容的最优理论：遇到不同级叶、非法PTE、G映射大小或不同context时，下一状态与可用compact checkpoint由实际R结果决定；每次PTE服务也未必等长。

不过，普通在线策略已经能在当前R退休后观察phase/level和live字段，选择保存当前table、完成G映射或重发子walk；Barr的level/reload-aware保留说明不能只提供静态均分缓存作弱对照。general dynamic checkpoint、variable-size caching、pinned PWC与同预算restart必须进入比较。**不允许把“oracle知道未来，而普通不知道”当机制差异；候选和baseline必须获得相同已退休PTE信息。**

如果要继续，最小新分析合同应固定：合法context集合与到达规则；同fault/cache输出；总存储与active/停车/标签费用；允许的checkpoint/重读/缓存/spill操作；q到底是事务数还是周期；memory/cache服务界；持续进度和让路的指标。然后给ordinary动态选择做不到的可达反例或下界。没有这些条件，所谓完整frontier只会是预选算法类的曲线。

暂停服务的反例依旧重要但不新：轮流退休一笔G-root并restart可以永远不推进S；只给cycle fairness不足以保证request fairness。无重复策略保留合适checkpoint、在q前主动yield就能避免此类重读；在根的known-chain模型中，其可达条件由相邻checkpoint gap决定，普通达到每请求15读、两请求30读下界。本席接受这个纸面停止理由，有限图检查由root另行记录。

## 5. 停止建议

停止fixed-chain frontier、逐phase pin与以q前提前yield为新机制的命名；不增加DUT或阈值扫描。若未知路径的差异只是普通动态checkpoint、变大小cache项或新增槽未实现，就停止该枝作为独立创新。

只在同到达/持续服务、同语义和同总资源下，证明一个上述ordinary family达不到的成本点，或给出忠实的nested在线模型及此前一般定理不能直接推得的必要/充分边界，才有继续立机制或理论的依据。目前没有这个证据。三项source未给完整多DMA三轴frontier，不是新颖性证明。
