# E6-PN：统一基座上的翻译与整体性能

- Origin Skill/Mode：ARS-Codex / experiment-agent / plan；2026-10-18 JST。
- Status：准备中，尚无 E6-PN 新 RTL、BIT 或板测结果。历史结果仅为动机。
- 主分支 `ispass/e6-pn`，独立工作树 `/home/xvhaocheng/work-e6-pn`；Root 在 `ispass/root-a` 审查、保存验收与排板。工具席独占 `ispass/e6-pn-tools`；gap 席独占 `ispass/e6-pn-gap` 的原前端小批，经 Root 接受后集成，不并改组件树。

要回答三个问题：原始 DS0 下 PN 是否仍比 P 慢；V 与 A 是否同时变慢；差额是否随 V 供给间隔变化。历史 DS50 研究 BIT 的约 12.7% 退化不能替代本次验证，也不能据仿真直接认定真实 MIG 的根因。

固定基座为 [统一原版清单](../../board/baseline-source.json)：platform `885ed0a7ea90ed42dae2b7a2dadfdd0acba15273`、CVA6 `8aac3fcd345af7686a55af1867783fd6b91a0069`、IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`、Linux `45bf76b15e36cf608dff95013d91f8bedb7b1b8e`，原 pathv3/BTC1/OBS3，50MHz。一个 BIT 运行时选 S/N/P/PN；仅移植 P 的 2/14 容量分区及分区局部 PLRU touch、已有 N 命中旁路及其必要依赖和控制。保留 S/P 的原控制器和原 3-beat backend；N/PN 的历史 banked/full-buffer 支路单独隔离。实际接口与有限验证由子席方案交 Root 审查，不能把旧研究树 mode0 称为原版。

用户已批准最小 V batch cooldown 以落实 gap1000/4000，并于本轮明确批准约 256 个计数触发器加少量标志的被动 A/双方共同硬件时长捕获。该观测不控制数据通路，不携旧拥塞诊断 observer、事件 FIFO 或新增页表 AR→R 逻辑；现有 observer 无法提供的页表读延迟累计跳过。资源数只是设计估计，须用真实综合验收。

| 阶段 | 配置与每格重复 | 独立试次 | 接受条件 |
| --- | --- | ---: | --- |
| DS0 S 回归 | Solo/A1/A8，各 5；gap0 | 15 | 原 kernel、原 CLI、reset S，不写 MODE；逐格硬件时间中位数相对原 DS0 偏差 ≤0.5% |
| DS0 主矩阵 | S/N/P/PN × A1/A8/A64，各 10；gap0 | 120 | S15 全过后；配置内按完整区组轮转 |
| DS0 供给补充 | P/PN × A8 × V gap0/1000/4000，各 5 | 30 | 独立 phase，gap0 不从主矩阵借点 |
| DS50 历史对照 | S/N/P/PN × A8，各 10；gap0 | 40 | 仅实际 DS0 S15 通过后才允许同源 DS50 构建 |

共预登记 205 个独立试次，各 phase 分开计数。V=DMA0、A=DMA1，V 为 4KiB×4096，软件预热256。保留原基线 A 连续批量供给、V 完成后 STOP/drain 的合同；A 最终工作量按实测报告，不改成双方各固定4096自然结束的 G 协议。S15 的原 DS0 参考中位数为 Solo 56.53868ms、A1 107.09009ms、A8 118.22441ms。

先 DS0，50MHz、WNS≥0、setup/hold/pulse 失败端点均0、routing errors0。只允许一次历史 post-route 物理优化流程；仍不通过即停，不改约束或继续尝试。S15 任一格超出0.5%或内置错误门失败即停，不进入正式测量或 DS50 构建。构建在 E1b 之后、尚未启动的 E3-lite 之前；不打断本轮已经在跑的完整 E3 构建及板任务。Root 是唯一硬件操作者。

正式输出保留实际 BIT、kernel、源码和命令身份、原始证据，并产统一17列长表 CSV 与分析脚本。指标包括 V/A 硬件时间、各自吞吐、双方完整工作共同吞吐、双方 miss/PTW、真实 N accept、V 分段等待、入口 AW 等待；HPM 软件采样区间、V 窗口、A 自身窗口和共同窗口分别标明，不能互换分母。零完整段为 N/A。比较先逐同 boot/block 配对计算，再报告 n、中位数、四分位与范围；不预设 PN 结果或因果结论。

通过检查的中途论文数据可提交推送；二进制留本机，失败和未完成证据保留但不冒称完整论文样本。Root 不合并 main，最终合并步骤与重叠点维护于 [合并说明](../../board/root-a-merge.md)。

当前已审设计入口：[最小移植与硬门](port-plan.md)、[固定来源调用链](source-map.md)、[测量合同](measurement-contract.md)、[原软件与采集计划](software-plan.md)、[Root设计接受](root-validation/plan01.json)。A1只实施P IOTLB/TL首小批；A2并行原前端gap小批；A3并行纯主机S15门，均须真实源码与有限验证接受后再集成。
