# MAP-RECYCLE-RECLAIM-1：最接近的回收能力核查

2026-10-16。独占范围为已有 DMA_PMD safe reclaim boundary 与普通 owner/dma-buf 适配是否覆盖本候选；root负责选机制、核原生cut与最终接受。本席请求 `gpt-6.1-sol/high`；实际独立model/effort metadata均为unknown，不能以请求替代实际证明。Codex-only；采用ARS deep-research fact-check来源纪律。仅读既存来源并写本目录三份交付物，未联网、下载、执行artifact/模型/测试或改驱动。

## 结论

**“所有frame归还后才退役映射、回收者先排除再分配、因此不用独立每TX映射token”的原理，与DMA_PMD已给出的分配器归还/全idle回收原理重合。** 不能把这三个条件本身作为新增机制。DMA_PMD不只是泛泛建议shrinker：02的最后block归还代码将全idle PMD摘出或纳入idle表，05的shrinker在同一pool锁下摘idle页，04的alloc也在该锁下取idle页；07再把实际`dma_pmd_unmap_all()`接进退役路径。这里核实的是公开RFC补丁中的具体实现文本，不是合入主线、完整正确性或性能验收。

**本候选相对该RFC的具体能力差项是“源页仍可留在原page_pool，A映射与source身份不变，仅回收B映射，以有限K的B关联容量重新接纳其他页”。** RFC所核路径退役整个PMD、卸其全部domain图，最后回buddy；它不是上述按普通source页单独周转B的现成实现。这一区别可带来更细的映射资源管理能力，但普通R-OB4K-T适配只要得到相同的source最后归还/再分配cut和关闭能力，也能增加同一B-only状态转移。不能把“普通R也可构造”升级为已有论文已实现此接口、完整成本相同或整个架构无价值；也不能把尚无网络hook反过来作为创新证明。

root本轮具体选定的是 **owner-assisted private `alloc.cache` 显式回收**：NAPI/source allocation owner串行取下idle页、同步撤B、按安全出版顺序重入cache；首轮不处理ring。不要求独立per-TX或per-alloc新状态，但支付owner pump、scan+lookup、同步unmap与metadata退役费用。以下一般回收义务只在这个具体cut内实例化；普通R获同一cut，不能强迫它采用更重的任意时刻异步evict版本。

## 逐项原文与核查范围

下列短名均相对 `/home/xvhaocheng/work/iommu-platform/out/`；行号是已存邮件diff/分析文件行号，不是已应用内核源文件行号。完整路径、hash、所读范围和原URL在[source-manifest.json](source-manifest.json)。

| 证据 | 支持的具体事实 | 不支持的扩大结论 |
|---|---|---|
| `mapping-organization-source-20261016/dma-pmd/lifecycle02.txt:25–39,143–158` | pool归还摊销map/unmap/IOTLB；全idle先摘分配器，再process-context卸图，再RCU后buddy释放 | 单独02此处只是计划性说明；不能把其207占位注释当实际unmap |
| `lifecycle02.txt:351–362,411–445` | block ref归零进入return；在pool锁下更新free/dirty，`avail==nr`才全idle；超水位/销毁就摘出排worker，否则idle | “任意一个TX CQ就可卸整页”不成立；注释周期不是实测 |
| `lifecycle02.txt:738–743` | hook实际接进`__free_pages_prepare()`，返回false阻止普通buddy释放 | 不是现成page_pool正常recycle通知 |
| `cap05.txt:181–208,220–249` | shrinker只计全idle页；pool锁下从idle摘页，锁外`dma_pmd_release_page()` | 没有在仍partial PMD中按空闲子页单卸B的逻辑 |
| `allocation04.txt:341–365` | alloc与shrinker/return使用同一pool锁；从partial优先、idle次之取页再分配 | 不能把“观察到idle”与“已经阻止重分配”混为一事 |
| `domain07.txt:351–378` | 后续hunk删除02占位，实际接入`dma_pmd_unmap_all()`；明确process context | 未核该函数所有内部/后端；不宣称单独搬用可并发重map安全 |
| `cap05.txt:25–39,138–148,273–282` | idle watermark、全局PMD页cap和内存压力shrinker已有；cap只挡增长，不同步逐出旧映射 | 不是K个B关联的精确容量上限，也不保某次新接纳立刻成功 |
| `mapping08.txt:83–91,300–319,336–338,378–405` | pooled每buffer unmap为空；整PMD RW/coherent lazy映射，位图+仿射IOVA，首次建图锁保护 | 不免费继承order0、规定单向权限、非coherent支持或PTE/IOVA成本 |

关键区别是**授权/分配器粒度与回收对象**，不是“它有shrinker、我们有return hook”这样的名称区别。RFC的最后block门决定整个PMD已经无使用者；普通page_pool需依据实际全部片段归还与独占再分配门确定某源页没有prepare或DMA使用者。若这两个门均成立，两者共享同一种安全依据。page_pool的实际计数/bias/可达cut由source-cut席核，本席不以逻辑计数值必须为0补造条件。

## 候选相对三类具名对照

| 对照及证据等级 | 已有或普通可构造的能力 | 本次候选差项及判断 |
|---|---|---|
| DMA_PMD公开RFC 02/04/05/07/08；原始补丁文本 | allocator-owned extent、最后block全idle、同锁摘出/重分配、watermark/shrinker、全domain卸图再回buddy；map命中无独立每TX映射token | candidate保留源页/A，仅拆B；更细source-page与B-only容量粒度，不是所核RFC现成能力。两者不能按相同内存/PTE布局免费对换 |
| R-OB4K-T-N；`mapping-integration-delta-20261016/owner/integration-analysis.md:35–44,66–79,100–112`定义的普通构造，非既发表项目 | 普通order0 source owner稀疏K关联；固定B；frame保护在途；正常无idle pin；现有构造只在source真exit/target close卸图 | 在同付费source-idle cut加一次`LIVE→CLOSING→ABSENT`、禁止重分配直到卸图责任结束，并仅卸B即可覆盖本候选。此转移是本轮普通适配推导，不是假称旧N合同早已允许idle eviction |
| DB-SLOT-K；`mapping-integration-delta-20261016/dmabuf/adapter-analysis.md:7–11,23–31,35–41,55–68`定义的普通适配，非core现成page_pool驱动 | K个logical shell/attachment可重绑，旧backing在notify/drain/unmap后可换新；窄版frame保护可省每TXtoken | 同idle cut让callback/撤图仅在无活frame且不再分配时发生，普通slot可卸旧B并让source保有页。标准map/unmap有睡眠/resv合同，可能需worker和源页隔离；NAPI首次热/冷路径与fence未核，不宣称同成本可部署 |

因此，**限定“只在正常return压力触发回收”是缓存逐出时机/策略约束，不单独构成新原理；将这个约束与source空闲cut结合，确实可以避开通用任意时刻逐出所需的独立每TX计数。** 该低成本能力必须同时给普通R和符合合同的slot adapter。它不同于“读到active==0就无锁卸图”：必须先禁止新的alloc/prepare使用这个映射；已有frame引用只证明当前使用者已结束，不会替回收者自动挡住未来重分配。

## 同信息接口下的完整义务

以下是可审查对照合同，不是本席替root选定的实现：

1. 正常最后归还边界若保留图，页可以进入源pool；一旦决定退役B，必须先关该binding新使用门，并把页从可重新分配状态排除，或在alloc路径出版给driver之前同步完成卸图。不能用上一次return的idle观察替代当前独占权。
2. 卸B完成且相应CPU读者/旧record使用结束后，K容量才能算可再用。若异步worker持旧record，则其尚未退还的record/IOVA/PTE都应占预算；不能先free slot而留下无账卸图债务。
3. 源页保留在pool时不应新增永久page/pp引用。异步隔离使其暂时不能再给RX用，仍可能导致替代source页分配或fallback；这属于实际延期/分配反馈，不能因“source仍拥有”隐藏，也不能直接把整个page_pool存量算作B额外payload。
4. 固定B配置、全部frame责任与target gate继续成立；已隔离且合法frame使用均已结束的idle页，不另要求全B排空。真正target关闭时仍须禁新和真实DMA drain；卸图期间若允许新prepare或目标换域，必须加等价保护。首次map的CREATING/关闭握手、metadata寿命和必要sync不能因没有每TXtoken消失。此处由root复核澄清normal idle回收与target关闭的范围。
5. K全部busy、没有页达到可回收cut，或后端unmap尚未完成时，当前包需合法O0/fallback；“有限K可轮转”是有合格idle事件并完成退役时可取得新容量，不是bounded-latency接纳承诺。若只在未来return触发，也不保证压力到来时早已在pool里的idle图立即释放；alloc侧回收须实际取得该页独占控制，不能在任意pool扫描中默认拿到。

CPU须分别计idle检测/分支、K/record管理、索引/状态锁、B-unmap和失效、worker调度/CPU宽限、再map、fallback及正常sync；payload按各策略真实S/P/C/Q并集去重，另列metadata/PTE与IOVA。候选、普通R、slot方案都能利用既有frame寿命，不强迫后两者新增per-TX原子计数、per-packet export或长期idle pin。dma-buf标准shell/attachment/resv与worker是其实际条件费用，但能否构成决定性差额本轮未知。

## 两个决定性区分例及相邻失败

- **对未改DMA_PMD的能力区分例（条件推导）**：同一PMD中子块x已全部归还，子块y仍合法在途。RFC不能把整个PMD判idle，不能经已核shrinker退掉其B映射。普通basepage candidate若x的实际全部fragment已归还，并能独占其再分配门，就可只卸x的B图、保留A/source页并周转该slot。该例只说明RFC granule/owner机制的条件差异，**不是当前v6.12 order0原生物理布局，也不是相同source pool实验**；不证明已实现、净收益或胜过同样basepage的R。细粒度同时失去整PMD大leaf的翻译覆盖强项，basepage可能增加PTE、mapping数量和IOTLB压力，不能只计回收便利而忽略这些费用。
- **对旧V1-N/K的能力区分例（条件推导）**：K条B图所绑定页仍属source池、都已idle而不会真正离pool；新页z需要B。旧N/K合同只能O0，candidate若取得一个idle cut、卸B并归还K容量即可后续接纳z。普通R加相同cut亦可；该例不能独立支持candidate相对R的新机制。
- **相邻失败**：x刚被重新alloc给RX/prepare，回收者仍因旧idle标记卸图；或只归还一个frame而同页另一fragment在途。两者都破坏无token依据。另一失败是只返还K记账但未完成旧unmap/metadata责任，制造无界退役队列。没有合格cut/可进展后端时仍需fallback，不将这一失败扩大为整个思想族无价值。

## 可支持的新意表述与停止范围

可核查的表述是：“最接近的DMA_PMD已用allocator所有权、全部block归还和独占摘出实现持久映射安全回收，并有watermark/shrinker。这里要推进的是普通page_pool中源保留与特定B映射退役的分离，在同一真实空闲边界周转有限关联预算；同边界普通owner适配也能做到。若新的接口/表示确实以更低完整CPU或更少不可回收资源在相同服务下完成该能力，或明确获得普通强对照未能覆盖的可执行能力，贡献可落在该接口/实现组织及其结果；目前来源只支持能力差项与普通适配覆盖，不支持成本优势、‘首次safe reclaim’或独有无token机制。”

因为DMA_PMD的全idle/同锁摘出已经给出同类safe boundary，停止“最后归还安全回收本身、无需每TXtoken本身”这两项独立原理新意主张。因为普通R同享该cut可做相同B-only退役，停止把这一步策略转移认作R不能覆盖的机制增量。保留源保留/B图退役分离的接口与实现成本问题，不把普通可构造性写成已发表或同成本证明。

本轮无需重读DAMN5.15：最接近的DMA_PMD原始回收代码已足以裁决上述重合；没有因此声称DAMN不存在相同能力或完成跨全部文献的首创性排查。该核查不选论文主线、不恢复实验，亦不重复旧阈值/数据入口账。
