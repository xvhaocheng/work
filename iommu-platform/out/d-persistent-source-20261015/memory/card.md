# 候选卡：Ramulator2 DDR4 行／刷新状态

**本席接受范围：持续状态来源成立；给定完整 O_phys 可重建。真实 O_host 是否足够、新增信息优势、收费接口与实际硬件收益均未核。**

| 字段 | 核查结果 |
| --- | --- |
| 固定来源 | CMU-SAFARI/ramulator2 v2.0a，`be93be78055d922aa1d4d33e15bcc8f2b0c61a9d`；论文 arXiv:2308.11030v2，核心 §2.1/2.2/3.1。 |
| 真实问题 | 用户队列排空、翻译 quiet 或 IOTLB clear，不自动等于下游内存冷状态；warm 也会改变内存。这里只对具体模拟实现核实。 |
| 实现规则 | 示例 DDR4_8Gb_x8 / DDR4_2400R，1 channel、2 ranks；FRFCFS、active/priority 仲裁、读写水位 .2/.8、AllBank refresh、ClosedRowPolicy cap=4；open row 与 cap 可跨 idle。 |
| 最强正向来源 | 普通前史可留下开行、ready deadline、刷新 future action；不同 PTE-read／payload-write 地址和时机可能走不同服务路径。它们必须由正常命令／warm／周期刷新产生。 |
| 不得制造的隐态 | 刷新从公开 tick0 每 nREFI 发起，没有独立随机 phase；不能在相同完整 O_phys 下私改行、counter、seed、epoch。不同物理前史是否具有相同真实 O_host 须另证。idle 后 write-mode bit 并非独立永久债务。 |
| 强普通对照 | 给定 O_phys 的确定回放仅作条件基准，O_host 下能否实现及费用未核；现有 FRFCFS／水位；OpenRow／cap4／cap1（含 PRE 成本）；等时间普通等待／合法 prepare／布局，按实际可编程能力纳入。 |
| 来源费用 | 模型 timing 有原表／公式。MMIO／传感器采样传输、状态存储、CPU回放／决策／切换总费用 unknown；不能用 C++变量读取或模拟速度作生产接口费用。 |
| 完成成本 | 读写队列空仍须查 active、priority、pending 读和刷新 future action；WR 无完成 callback，不能映射成 AXI B 或真实写入完成。 |
| 条件正例 | 若另证真实可得完整 O_host 不能及时识别仍改变动作排序的状态，且付费新接口比 O_host 可实现的普通预测／回放、等待／已有 row/write 控制省完整完成成本，才继续信息贡献。当前源码未核 O_host→O_phys 映射／获取成本，既不证明 O_host 足够，也不证明有缺口。 |
| 相邻反例／否证 | 已知 reset＋O_phys 可重建只约束物理入口，不单独否定 O_host 信息贡献；若 O_host 可低成本确定该迹则否定相应信息缺口；全rank刷新／warm抹去差异；等时等待复现；原生读写独立；观测费用吃掉收益。 |
| 当前实验状态 | 0 模拟运行／构建／新模型／旧实验恢复／设备操作；MIG1792023314 paused不变；M0读写独立旧结论保留。 |
| 下一最小项 | root 可决定是否只验“普通前史→严格排空→短idle保留→刷新洗去→O_phys 条件重建”的固定源码命令迹；先补真实可见性／费用，不能直接进入Z选策搜索。本席未实施。 |

入口：[详细核查](findings.md)／[源字节、Git blob、行号和读取范围](sources.json)／[机器可读裁决](assessment.json)。源码事实、确定性推论、实际部署未知已分别标记。请求 Sol/high；实际模型与强度运行元数据未知。

O_phys＝controller accepted 的完整物理地址／读写种类／精确tick／同tick顺序＋epoch／配置；O_host＝项目实际可见的SPDK主机submit／CQ等完整历史。O_host→O_phys 映射和成本未核。文字修正未新增来源；旧 artifact-hashes.json 保留首次快照。
