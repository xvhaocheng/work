# §48 已知共享范围与 producer 步调：三项关键前作及强普通对照

2026-10-08；本席 Codex 亲自读原文、推演机制边界及反例。没有调用 Claude/DeepSeek，没有另开 agent，没有 DUT/build/数学运行或生产修改。复用已有 primary 的精确副本，新增下载一项作者论文；全文和 hash 导航见 [primary/sources.json](primary/sources.json)。

## 结论

**可以留下具体、可比较的 descriptor-driven 调度候选，但不能把 translation-aware pacing、已知共享范围聚集任务、共享条目保留命名为新原则。** 三项原作分别覆盖改变 producer 的运行集合/组成、按翻译压力放置任务、用软件范围与共享语义协调任务和缓存。

**所读的具体规则没有给出两个正在运行的 DMA copy 按连续 translation-materialization 水位维持显式相位差界。** 这只是精确覆盖边界，不是检索没找到就证明独立创新。普通逐页 tile barrier、有限领先窗口、shared-source ref 保护和 last-destination-page records 可以实现相近目的，必须允许这些同信息对照。

如果新规则与普通 range/refcount 保留产生相同事件及完整成本，就不能称新算法；如果具体反馈/准入组织在真实 producer 合同下，以同信息/总资源保留较大合法并行窗口，并取得整体 copy 性能或完整实现成本增量，可以继续论证 modest 微架构贡献。本次没有该实验或最终成本结论。

## 1. 三项 primary：明确到实际动作

### Pichai/Hsu/Bhattacharjee，ASPLOS 2014

**Architectural Support for Address Translation on GPUs: Designing Memory Management Units for CPU/GPUs with Unified Address Spaces**。

- 精确保存版本：[PDF](primary/pichai2014.pdf)，15 PDF 页；首页有 ASPLOS'14 版权/DOI，CreationDate 为 2014-01-10。原取得 URL 为 [Illinois 大学所存原始论文](https://courses.grainger.illinois.edu/ece598ms/fa2021/papers/paper233.pdf)。这是原作副本，不是该课程的综述。SHA256 `c958b785f98aedfb834fea8381a0147163599d4584cd1a0428b7b7a8cb5a8e3f`。
- **此固定版本调度是 §7.2，不是 §6.2**：[raw:881](primary/pichai2014.raw.txt:881)，PDF pp9–10；TA-CCWS 在 [raw:915](primary/pichai2014.raw.txt:915)，TCWS 在 [raw:925](primary/pichai2014.raw.txt:925)。§6.2 在本版本讨论 CPU-style translation。不能把另一稿/browser 摘取的章节号混过来。
- §7.2：TA-CCWS 增加 TLB miss 对 lost-locality score 的权重；TCWS 用 TLB victim-tag arrays，VTA hit 更新 per-warp score 并选择较高 score 的 warp。它还在 TLB hit 按 LRU depth 调整 score，[raw:1360](primary/pichai2014.raw.txt:1360)，不是仅发生 miss 才反应。
- §8.2，PDF pp12–13，[raw:1687](primary/pichai2014.raw.txt:1687)：Common Page Matrix 记录 warp-pair 过去共享 PTE 的程度；动态 thread compaction 偏向组合这些来源，TLB hit 的 warp history 更新矩阵。

**覆盖**：TLB 信息主动改变 producer 集合与组成，促进翻译局部性；接近 eviction 的 hit 能提前触发调度。**没有从这几个具体规则得到**：已登记 source/destination range 的精确 future-use、水位差上限、两个独立 DMA 的 copy-tail 或 first-VALID 义务合同。不能把“从 warp 改成 DMA”单独当创新；若特定 descriptor feedback/algorithm 有增量，须单独比较。

### Li/Wang/Tang，DAC 2023 作者稿

**Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs**。

- [作者原始 PDF URL](https://libingyao.github.io/assets/files/DAC2023.pdf)，本地 [PDF](primary/orchestrated2023.pdf)，6 PDF 页；复用 map 的固定副本，SHA256 `65f29701646a6e8e1b7b9507855d0e10047e3286439e199b955391475664e46b`。本稿分析绑定该副本，不冒称当前远程 URL 一定字节相同。map 对作者年份引文有版本不确定性；本地 CreationDate 是 2023-04-13，不由此断言未修改 camera-ready。
- §IV-A，PDF p4，[raw:636](primary/orchestrated2023.raw.txt:636)：每 SM 的 `<TLBhits,TLBtotal>` 形成即时 miss rate，新 TB 优先放到较少 thrashing 的 SM；不限制可并发 TB 数，不能把它描述为在线 throttling。
- §IV-B/C，PDF pp4–5，[raw:690](primary/orchestrated2023.raw.txt:690)、[raw:764](primary/orchestrated2023.raw.txt:764)：TBid 分 set；满 set 可借相邻空 set，并通过 sharing flag 扩大查找；TB 完成时重置相关 flag，[raw:796](primary/orchestrated2023.raw.txt:796)。

**覆盖**：translation-aware task placement 与 partial sharing/容量干扰控制；没有改变 TLB 容量。**具体区别**：其决定是新 TB 放哪、查哪些 sets；不是在两个 live streams 中暂停较快页进度，等待同页较慢消费者。不能用它“不节流”说明整个领域没有 pacing；Pichai 已给 producer scheduling。

### Vijaykumar et al.，ISCA 2018

**The Locality Descriptor: A Holistic Cross-Layer Abstraction to Express Data Locality in GPUs**。

- [作者 Onur Mutlu 原始 PDF](https://people.inf.ethz.ch/omutlu/pub/LocalityDescriptor-Cross-Layer-GPU-Data-Locality-Abstraction_isca18.pdf)，本地 [PDF](primary/locality-descriptor2018.pdf)，14 PDF 页；本轮实下载，SHA256 见 manifest。
- §3.3/§4.3，PDF pp5–8，[raw:581](primary/locality-descriptor2018.raw.txt:581)、[raw:855](primary/locality-descriptor2018.raw.txt:855)：已知地址范围、C-Tile/D-Tile 关系、共享语义和 priority 指导 CTA clustering；共享数据的 CTAs 放同 SM，Algorithm 1 还协调 locality 与并行度。driver 在 launch 前定策略，[raw:931](primary/locality-descriptor2018.raw.txt:931)。
- §4.4，PDF p8，[raw:939](primary/locality-descriptor2018.raw.txt:939)：共享数据用 soft pin priority；文中目标是留到其他 threads 访问，但具体 soft pin 明确没有控制 thrashing 的策略。hard pin 的固定 victim way 和周期 timer 也不是精确 per-consumer last-use release。
- §4.5，[raw:982](primary/locality-descriptor2018.raw.txt:982)：regular coaccessed streams 的 prefetch lead 受 cache capacity、active tiles、tile width 和 stride 限制。

**覆盖**：软件暴露共享范围、协调任务和保留/预取，用容量限制领先距离。**具体区别**：这里的“领先”是预取距离；规定动作是 task clustering/placement 与 cache priority，不是测两个已运行 consumer 的完成水位并暂停快 consumer。它是数据 cache 原作，不能冒称已有 IOMMU 多 DMA 保证；但把数据换成 translation 的范围/共享描述本身不足以立新。

## 2. 是否主动调整两个 stream 的相位

| 原作 | 实际主动动作 | 原文是否给两个 live consumer 的 bounded phase rule |
|---|---|---|
| Pichai 2014 | 改运行 warp 集合；按过去 shared-PTE 关系重组动态 warp | 所读规则没有精确两个水位/范围；已明确改变 producer 调度，不能只称 cache 优化 |
| Orchestrated 2023 | 放置新 TB；动态相邻 sets sharing | 没有这项规则，且明确当前方案不 throttle TB 并行度 |
| Locality Descriptor 2018 | 按已知共享范围组任务；pin/bypass/prefetch 联动 | 聚集 task，容量约束 prefetch lead；没有按实际两消费者进度暂停快流的规定动作 |

这些“没有”限于已读、已定位的具体机制，不构成全领域不存在的声明。

## 3. 亲自给出的同信息强普通规则

固定映射、无 fault 的初始判别中，两已登记 linear copy jobs 读取同一只读输入、写不相交输出；输入必须共享**同一有效 translation key**，只有物理数据相同不够。令 source 页 p 的 consumer 集为两个登记 jobs。

### O1：逐页配对 tile barrier

按 source 页边界将两 jobs 分为对应的 copy tiles。先完成两份 tile p，再提交 tile p+1；本页 source translation 在两 consumer 翻译需求结束前保留。各 DMA 用普通 last-destination-page record 保住输出跨 tile 的部分页复用；仅在可证明无后续使用时 no-allocate/bypass 输出翻译。

在顺序 tile、正常译址、适当 request 切分的纸面合同下，一条 shared-source 当前页记录和两个当前 destination 页记录即可避免这里的额外容量重读：新 source 页出现前，两 consumer 已不再需要旧 source 页；每条输出流保留自己的相邻部分页。**“3 条 key”只数最终翻译记录**，不是完整系统 bit/面积：descriptor/context/epoch、正常 TC/private parent metadata、valid/permissions、更新接口都另计。它不保证任意原生 prefetch/FIFO 调度达到同样结果，也不包含 DC/PC/PTW 的全部首次工作。

此控制在软件可做，也可作硬件 inline tiler。必须计 subjob 供给、共同分割 source/destination burst 造成的额外 AR/AW、barrier 扩大数据等待及失去 lookahead 的成本。不能拿不切 descriptor 的候选与“免费的软件 tile”比；也不能故意只允许普通 full-copy barrier。

**直接否决条件**：如果原候选只保护 interval/refcount，再配相同 tile 顺序，而源/输出 miss、完整 metadata 和 request 代价相同，就不能称新算法。O1 能消容量重读的纸面事实本身不是当前 FPGA 的性能结论。

### O2：有限领先窗口，比 full-page barrier 更强

维护 `U`：已预留/已 materialize 给某 consumer、仍会被另一个已登记 consumer 需要的 shared-source 页集合。只在 `|U|<C_s` 时允许宣布一个新的 shared-source 页；同页复用不额外占新 lease。完成所有本页必要 translation consumers 才释放对应 lease，不能以一个 fragment 返回冒充整页完成。候选 producer 用同信息也可以运行这一普通规则，不预设等待整条数据尾部。

这是一项具体 finite-window/refcount admission 构造；容量保证来自先 reservation 再 first-VALID，而不是从 `max(successful_seq)` 猜一个连续前缀。root/rules 已发现最新 helper 允许 seq0 未完成、seq1 准备成功，因此需要原 parent window 的 live/generation/certificate 扫描或真实补充事件，全部计费。watermark 若在 producer，不会自动免费出现在 IOMMU。

**进度限定**：新 AR 关闭后，已经 first-VALID/accepted 的 AW/W、parent lookahead 和队列义务仍要完成。普通两流水位差并不证明 streaming DMA 能安全停车；既有目的 burst 跨过 source 页边界可能需要更多 source。可从新 tile 的入口控制，或让 admission 同时覆盖配对 source/destination 义务，但不能临时撤回已 offered AXI 请求。O2 能否保留原 burst 形状并满足该合同，是具体实现/成本问题。

## 4. 剩余可比较的增量及反证

区别值得测试的位置是**调度单位与反馈**：新控制如果只按两个 DMA 的 data-retirement 游标同步，将不必要地拖住可独立推进的 translation；如果只看 translation 最大成功序号，又会在真实 holes 下漏掉未完成页。一个有效候选需要可靠连续 materialization 与 in-flight reservation，并允许 hit/private prepared 状态中的合法数据尾部继续。

但把 control event 换成 translation materialization，或把 bitmap 写成 interval suffix，本身是普通语义和状态表示，不自动新。Pichai 已用 TLB hit/eviction 压力主动控制 producer；Locality Descriptor 已用知道的 shared range 协调 scheduling/cache。具体候选须与 O2 的相同反馈、相同 pool 成本比较，不能只对比两个互不知晓任务的 RR streams。

**一个合法失败事件序列**：A 先 materialize source p+1；V 还需要 p；A 的早发 destination burst 已接受，尾部需要 source p+2。若 controller 仅因 A 比 V 领先就关闭 p+2，并且 V 的 W 被这个旧 AW 队首挡住，cache lag cap 可变成进度环。这个例子只说明规则需核已有承诺的义务；本席未证明该顺序在当前特定 out wrapper 中可达，不把它称生产死锁。完整 tile barrier 的强普通控制不会先提交 p+2 的 AW，因此合同不同、代价也不同。

## 5. 最小判别与停止建议

首先确认在当前 cache/masks、真实 key 和任务长度下，确实存在额外非compulsory source PTW。共享输入 32KiB 的页数可能小于某些配置的实际可用容量；不能仅因进度不同就假设 thrashing。分开数 input reuse miss、一次性 output fill/pollution、DC/PC work 与数据带宽。

同登记任务和总资源只比较：原 RR/现有 producer；共享 source last-page/last-destination 普通保护；O1 tile barrier；O2 finite window；候选可靠反馈+具体策略。看完整 copy makespan、两 DMA 完成时间、PTW/source重读、AR/AW/metadata/供给代价。必须有正确完整 copy；降低 miss 不等于降低时间。

如果 last-page 或 O1/O2 已在相同完整成本消除重读，停止纯替换/phase-sync 机制命名；如果 candidate 的改善依赖对照拿不到相同登记任务/窗口事件或免费多存 producer metadata，也停止。若 candidate 在真实可达、同信息/资源下保留更多合法数据并行度，或用更低完整接口成本达到相同性能，就可以继续论证具体微架构增量，不要求证明所有未来算法不可能。当前没有这样的验证点，没有新定理或期刊分区承诺。

## 6. root 本轮有限模型结果与最终收口

根席通知已执行一次模型，exit0：9 个 trace/E 组 × 4 个普通策略，共 36 records，固定 N8、aligned、D/S/D/S 事件。E8/gap4 的 miss 数为 LRU30、remaining-use24、private-dest26、oracle24；E4/paired 都为24；E16各策略都为24。导航：[原始模型输出](cache-model-result.json)、[根席接受记录](codex-result-acceptance.md)。这是 **root 执行的有限逻辑模型**，本席未执行/重跑；这些数字不是 DUT 性能、周期或速度，也不证明任意策略/容量的全域最优。

因为该具体普通控制达到 remaining-use 的工作点，remaining-use 原则停止独立算法命名。下一只审查 materialization-frontier 控超前时保留数据重叠、held/committed W debt 能 finite continuation 的具体合同；O1/O2、translate-first、lookahead 都须得到相同有效信息和完整计费。已发表原作没有写出这个精确边界，不能单独作为新颖性证据。三项原文核查完成，根席已亲读关键段；本席不继续扩搜或运行。
