# 共享 IOMMU 历史实验总账

这里回答“做过什么、实际做到哪一步、结果支持什么、后来如何修正、原始证据在哪里”。当前状态仍由 [CURRENT_STATUS](../../CURRENT_STATUS.md) 管理；本账不调度实验。整理日期为 **2026-10-18 JST**，没有重跑实验、构建、板测或科学归约。

## 新 agent 怎么查

1. 先在[逐轮总账](index.md)按问题或旧名称搜索。ID、stage、原目录名、历史身份 ID 和别名同时保存在 [catalog.json](catalog.json)，也可查 [CSV](experiments.csv)。
2. 打开对应详细卡片，先读结论、范围和后续关系，再沿原报告、原验收和数据入口取证。**原报告的“下一步”“未上板”“running”只代表当时；有后继时不能直接当现状。**
3. 查看[补充与修正关系](corrections.md)。补充、实际执行、功能修正、证据等级修正和同轮资料分别标注；同轮报告不增加重复次数。
4. 找不到时查 [coverage.csv](coverage.csv)，确认目录是否归到另一轮、属于构建／计划／恢复副本，或确实没有结果证据。查具体 run、失败或 manifest 用 [evidence-files.csv](evidence-files.csv)。这是文件定位表，不是独立试验表。
5. 涉及源码版本时查 [identity-links.csv](identity-links.csv) → [既有历史身份记录](../../iommu-platform/provenance/source-organization-20261017/history/README.md#历史版本身份补填)及实际 source/build/run manifest。当前 checkout、执行时输入和后来的归档提交分别使用。

在称“没做过”“没有结果”或安排复现前，必须检查相关轮次、后继关系和覆盖清单。没有找到时写明搜索范围及缺项；不要把“当前论文表未选入”写成“历史未做”。

## 主要实验链

| 问题 | 入口与演变 |
| --- | --- |
| 原论文和早期双 DMA | [早期记录](periods/07-09.md#early-remote-ptw)：Remote PTW、Pinned/Bounce Buffer、工作集和 Barrier 表；早期运行身份缺失。七月源码事实另列，不倒填八月日期。 |
| 软件窗口与真正硬件重叠 | [E1](periods/07-09.md#h-e1-results-review) → E4完成发现／供给 → batch自主批次 → DS0／DS50回切 → path。软件慢化、硬件窗口和附加DS敏感性分开。 |
| miss 到 hit 的局部等待 | [E3 v4](periods/07-09.md#h-e3-results-review) → D0/D0 → v5 → v6 → [时序证据更正](periods/10-15-18.md#correction-e3-timing)。保留观察，不无条件称时序验证后的定量因果。 |
| 固定限速和反馈计费 | [P4M](periods/10-01-04.md#h-p4m-complete-review)及短测／pilot／启动控制／25＋6＋29恢复 → 84项失败与Solo11 → fullstats21 → method462 → method88。信息价值与强匹配控制收益分开。 |
| 容量替换与命中旁路 | [E2新版64](periods/10-01-04.md#h-e2-results-review)、[N v0–v5](periods/10-01-04.md#n-v0)、composition失败 → DS/PTW修复fresh172 → clean172 → 分区历史隔离22 → gap18 → 真实DMA模型 → M0双方完整工作。 |
| 共享缓冲、译后分段与共享读 | [10月5—9日](periods/10-05-09.md)：pool／park／W顺序／原父窗口／提前准备／PA旁路／有限几何、共享读各子轮与真正late join。局部节省、强普通覆盖及失败版本均保留。 |
| 应用投影与消费者 | [10月10—14日](periods/10-10-14.md)：行流／HERO／冷权重／R107普通板测／C113 NVMe来源／stage117逐包协议、真实缓存和六案G0。native、主机、条件模型和FPGA分开。 |
| 生命周期、控制位置与映射组织 | [10月15—18日](periods/10-15-18.md)：A2–A5、D-CS1／MQSim／DRAM、B1、mapping各轮；CO1/RCS1/BR1与SDH1/LR1的范围及端点勘误链接在各原轮。 |
| 首PTE与PLRU后继 | 条件模型 → 接口成本 → [局部成对RTL](periods/10-15-18.md#h-first-pte-paired-local) → DISCOVERY-REUSE-1 → [PLRU直接victim与LRU消融](periods/10-15-18.md#h-plru-ablation)。旧“未直接采victim”不能覆盖后继消融。 |
| 失效与回收 | [宽／细粒度失效28配置](periods/10-15-18.md#h-invalidation-interference) → [主线strict/FQ/ORACLE与死条目](periods/10-15-18.md#h-dead-translation) → [IOFENCE归因与模型边界修正](periods/10-15-18.md#h-iofence-interference)。26组IOFENCE计划没有执行。 |
| 投稿复现与统一基线 | [G1九次](periods/10-15-18.md#ispass-g1)、[统一100次](periods/10-15-18.md#ispass-baseline100)、DS25/DS100新增100、既有100次离线分析，以及新负载E3因接口边界停止。历史E3与负载E3不是同一轮。 |

## 文件与口径

| 文件 | 作用 |
| --- | --- |
| [index.md](index.md)／[experiments.csv](experiments.csv) | 逐轮检索：问题、报告结果、结论、证据层、状态和后续关联。 |
| `periods/` | 五个时段的详细卡片，含原文行段、摘录、来源SHA、原始产物和身份入口。按来源正文日期排序；仅路径日期会显式标记，不使用mtime。 |
| [catalog.json](catalog.json) | 可维护的主数据。包含原报告视图、源核查、未执行计划、失败和恢复；条目数不是独立实验数。 |
| [corrections.md](corrections.md) | 有方向、有理由的后续关系；“同轮资料”避免重复累计。 |
| [coverage.csv](coverage.csv) | 本次635个入口单元的明确归属或辅助分类，包括平台374个一级输出目录、平台协议／报告、板测操作目录、历史计划、归档和CARRV写作材料。 |
| [evidence-files.csv](evidence-files.csv) | 运行／构建／验收元数据定位与本次字节哈希；不读取或推定每个文件内的科学判据，不把fixture、launch或多个副本算作试次。 |
| [identity-links.csv](identity-links.csv) | 复用原79条身份索引及60条补充身份；配置、子条件和轮次不是同一种计数单位。新投稿运行从配套包进入。 |
| [verification.json](verification.json) | 静态覆盖、ID／关系完整性、精确原文摘录与哈希检查；不出具科学复验或安全证明。 |

## 覆盖与缺口

本次完成的是**上述仓库范围的逐轮导航与入口归属**。原报告保留各自的执行和验收状态；来源摘录、旧阅读账本和必要关键段用于连接结论，程序读取／列目录不计为模型全文核读。未宣称全部UART、CSV事件、源码、论文或二进制被重新审阅。数据表的标量行、case、target、parent和boot均不互换计数。

- 原学位论文与图表在仓库外 `/home/xvhaocheng/thesis`；本账登记仓库内已有引文入口及结果概括，没有搬迁或声称逐项恢复其全部实验。
- 早期Barrier／Lockstep表仍缺完整命令、确切日期、实际版本和原始运行链；八月逐日记录缺失。没有用后来的mtime、分支或新实验补写。
- 历史dirty输入、实际加载校验、旧内核完整重现等未知项沿既有manifest保留。现在对文件取SHA，只证明本次所见字节，不证明历史实际加载身份。
- 原始输出里的构建树、frozen源码、before快照、对象目录和归档恢复副本不重复作为新的实验。元数据文件表按选定文件名定位，并非全文件枚举；完整数据沿实验原目录进入。
- 历史审阅目录中的“首次全历史基线仍未完成”描述的是该轮核读与原始证据覆盖，不因本账635个入口已归属而被改写成“全部科学结论独立复验完成”。

## 怎样维护

新增轮次或发现遗漏时，在 `catalog.json` 增补具名条目，记录实际执行层、结果、结论边界、来源行段与产物；新目录／报告必须在coverage有归属或明确辅助原因。为旧条目新增后继关系，原数据和旧裁决保持。更新当前主题摘要和状态时，采用已确认的最新限定，不把整个旧报告复制进入口。

在仓库根目录运行以下**文档静态工具**：

```sh
python3 tools/build_experiment_ledger.py
python3 tools/build_experiment_ledger.py --check
```

渲染只写本总账的派生文档；检查不写文件，拒绝未归属的新入口、失效ID／关系、被改动的来源摘录和已登记元数据字节。确有新运行资料时，在补完归属后显式运行 `--index-evidence` 更新文件定位表；不得为通过检查而把未知实验标成已完成。该工具不导入实验runner、不调用历史validator、不触板。
