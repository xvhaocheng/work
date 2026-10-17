# E6-PN：统一基座上的翻译与整体性能

- Origin Skill/Mode：ARS-Codex / experiment-agent / plan；2026-10-18 JST。
- Status：准备中，P/N/history、原前端gap、S15离线门、只读helper、被动时长/tap、独立MODE控制、真实双backend与DS保护已通过Root有限审查。尚无完整 PN BIT 或板测结果。历史结果仅为动机。
- 主分支 `ispass/e6-pn`，独立工作树 `/home/xvhaocheng/work-e6-pn`；Root 在 `ispass/root-a` 审查、保存验收与排板。工具席独占 `ispass/e6-pn-tools`；gap 席独占 `ispass/e6-pn-gap` 的原前端小批，经 Root 接受后集成，不并改组件树。

要回答三个问题：原始 DS0 下 PN 是否仍比 P 慢；V 与 A 是否同时变慢；差额是否随 V 供给间隔变化。历史 DS50 研究 BIT 的约 12.7% 退化不能替代本次验证，也不能据仿真直接认定真实 MIG 的根因。

固定基座为 [统一原版清单](../../board/baseline-source.json)：platform `885ed0a7ea90ed42dae2b7a2dadfdd0acba15273`、CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`、Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`，原 pathv3/BTC1/OBS3，50MHz。一个BIT运行时选S/N/P隔离/PN/P共享历史（raw0/1/2/3/6）；仅移植 P 的 2/14 容量分区及分区局部 PLRU touch、已有 N 命中旁路及其必要依赖和控制。保留 S/P 的原控制器和原 3-beat backend；N/PN 的历史 banked/full-buffer 支路单独隔离。实际接口与有限验证由子席方案交 Root 审查，不能把旧研究树 mode0 称为原版。

用户已批准最小 V batch cooldown 以落实 gap1000/4000，并于本轮明确批准约 256 个计数触发器加少量标志的被动 A/双方共同硬件时长捕获。该观测不控制数据通路，不携旧拥塞诊断 observer、事件 FIFO 或新增页表 AR→R 逻辑；现有 observer 无法提供的页表读延迟累计跳过。资源数只是设计估计，须用真实综合验收。

| 阶段 | 配置与每格重复 | 独立试次 | 接受条件 |
| --- | --- | ---: | --- |
| DS0 S 回归 | Solo/A1/A8，各 5；gap0 | 15 | 原 kernel、原 CLI、reset S，不写 MODE；逐格硬件时间中位数相对原 DS0 偏差 ≤0.5% |
| DS0 主矩阵 | S/N/P（隔离）/PN/P（共享历史） × A1/A8/A64，各 10；gap0 | 150（含新增30） | S15 全过后；配置内按完整区组轮转 |
| DS0 供给补充 | P/PN × A8 × V gap0/1000/4000，各 5 | 30 | 独立 phase，gap0 不从主矩阵借点 |
| DS50 历史对照 | S/N/P/PN × A8，各 10；gap0 | 40 | 仅实际 DS0 S15 通过后才允许同源 DS50 构建 |

含历史开关时共预登记235个独立试次，各phase分开计数；[逐点顺序与条件](plans/README.md)已保存235/205互斥计划，尚无运行数据。用户追加的 P（共享历史）保持 2/14 容量分区，仅恢复整棵共享 PLRU 的 touch；P 与 PN 默认仍隔离。先在独立小批审查控制位扇出和实际展开；若显著增加时序风险，则完整构建前关闭该功能，回退原隔离版本的 205 点计划，并明确标记新增30点未运行及原因。不为该开关反复完整构建或改约束。DS50 仍只有原四模式40点，供给间隔补充仍只有隔离 P 与 PN。V=DMA0、A=DMA1，V 为 4KiB×4096，软件预热256。保留原基线 A 连续批量供给、V 完成后 STOP/drain 的合同；A 最终工作量按实测报告，不改成双方各固定4096自然结束的 G 协议。S15 的原 DS0 参考中位数为 Solo 56.53868ms、A1 107.09009ms、A8 118.22441ms。

先 DS0，50MHz、WNS≥0、setup/hold/pulse 失败端点均0、routing errors0。只允许一次历史 post-route 物理优化流程；仍不通过即停，不改约束或继续尝试。S15 任一格超出0.5%或内置错误门失败即停，不进入正式测量或 DS50 构建。构建在 E1b 之后、尚未启动的 E3-lite 之前；完整 E3 版本已因布线/时序失败停止，保留失败原件，不再重试。当前板恢复与原软件准备串行完成，不提前进行 PN 测量。Root 是唯一硬件操作者。

正式输出保留实际 BIT、kernel、源码和命令身份、原始证据，并产统一17列长表 CSV 与分析脚本。指标包括 V/A 硬件时间、各自吞吐、双方完整工作共同吞吐、双方 miss/PTW、真实 N accept、V 分段等待、入口 AW 等待；HPM 软件采样区间、V 窗口、A 自身窗口和共同窗口分别标明，不能互换分母。零完整段为 N/A。比较先逐同 boot/block 配对计算，再报告 n、中位数、四分位与范围；不预设 PN 结果或因果结论。

通过检查的中途论文数据可提交推送；二进制留本机，失败和未完成证据保留但不冒称完整论文样本。Root 不合并 main，最终合并步骤与重叠点维护于 [合并说明](../../board/root-a-merge.md)。

当前已接受小批与复核入口：

- P/N/history：[P原件](root-validation/p-only01.json)、[N原件](root-validation/n-core01.json)、[历史开关27938项与首构风险决定](root-validation/history01.json)；历史开关未增加IOTLB状态，物理时序尚未测。
- 原前端gap：[事件及兼容性](root-validation/gap-accepted01.json)。S15离线门：[Root实际51项](root-validation/s15-accepted01.json)；真实S15仍0点。
- 被动模块与原事实：[capture](root-validation/observer01.json)、[tap](root-validation/taps-accepted01.json)、[独立MODE控制](root-validation/control-accepted01.json)；[新bank与旧observer整合a324](root-validation/embedding-accepted01.json)已有限接受，Root126项实际源/raw绑定通过；外部STOP/N/quiet仍为测试接口。
- 只读工具：[P共享历史适配244项/71情境](root-validation/pn-control-shared-accepted01.json)，原S15/source-map不变，无MODE/GAP写操作或板执行。
- 双backend：[接受回执](root-validation/backend-accepted01.json)、[Root109726项真实原raw复核](root-validation/backend-raw01.json)。N实际4096B容量、自然退休与证书撤销通过；不是整栈或物理RAM/时序证明。
- DS：[只读保护与真实MSIPTW尾拍](ds-validation/README.md)、[Root168906项复核](root-validation/ds-accepted01.json)。只新增17FF transport guard，不加论文诊断观测。

Root已接受[旧observer内bank/capture接线方案](observer/integration-plan/bank-embedding-v1.md)及SW/program独立quiet提案；[TC/vendor6082/95f](root-validation/tc-accepted01.json)、[SW/program0dbd](root-validation/sw-accepted01.json)及[typed透传30bc](root-validation/corewrap-accepted01.json)均已Root有限接受。SW补既有DDTP pending及下一沿MSI来源，只读无新FF；全域仍待实际聚合。当前A1实施TL/MSIPTW小批，A2新增独立pn-config writer，A3补实际_intf透传（[调用链纠正](interface-callchain-correction01.md)），之后才外围接线。后续仍需全栈接线/真实有限gold、受限writer和采集脚本、源锁与DS0构建；这些小批回执不代表整栈或时序已过。E3-lite有限接受，综合排在PN首构之后。原pathv3/原DS0已于18:39:20Z启动核验成功，板空闲，等候合格PN BIT。
