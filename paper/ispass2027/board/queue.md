# Root A：FPGA 板测队列

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: UNVERIFIED（下表区分构建、准备与实际采集）
- Version Label: root-a-board-queue-v1

本轮板卡唯一调度与执行者为 Root A。子 agent 只准备代码、构建和分析；任何时刻只有一个 JTAG/UART/板测任务。各工具继续使用既有 UART 独占锁。此文件仅在 `ispass/root-a` worktree 维护，不修改主目录或共享结果表。

## 当前资源与顺序

DS25/DS100 构建已完成并验收：两档WNS均+0.177ns，CPU 50MHz，setup/hold/pulse失败端点及routing errors均0。独立会话“构建并采集 DS 延迟曲线”在12:53:13Z已启动同身份DS25→DS100，共100次；当前DS25正在启动旧pathv3。Root本地12:56:37Z启动被UART锁在触硬件前拒绝，失败目录原样保留，不再启动替代任务。用户已明确授权联系该会话并统一排程，协调消息已送达：原campaign继续两档100次，随后保持DS100交还板卡，不自行写SD或加测。

唯一活动campaign为 `/home/xvhaocheng/work/paper/ispass2027/board/runs/ds-sweep-20261017-01/`，launch为同级`ds-sweep-20261017-01-launch/`；Root只读其稳定records冻结本任务快照。Root被锁拒绝的记录在本worktree `runs/ds-sweep-campaign-launch01/`及`runs/ds-sweep-20261017-01/`，不计为已加载或性能试次。

| 原优先级 | 任务 | 状态 | 采集规模 | 预计板时 / 条件 |
| --- | --- | --- | --- | --- |
| 1 | E3 DS0 小子集 | 用户已批准仅 P0 最小 driver/RTL、构建与45点；A2 分批实施 | 3 个 V 大小 × Solo/A1/A8 × 5 = 45 次 | 约45分钟采集，另需新构建；packed-ring，不扩三类完整形态 |
| 2 | E1b DS25 | 验收通过；现有外部campaign在Root排程下运行 | Solo/A1/A8/A16/A64 各 10 次 = 50 次 | 约 20–30 分钟，须 50MHz、WNS≥0、违例端点和 routing errors 全零 |
| 3 | E1b DS100 | 验收通过；等待DS25全50点完成后自动切换 | 同上 50 次 | 约 20–30 分钟，同一档集中完成；两档仅换 BIT，复用已写 pathv3 |
| 4 | E2 DS0 设备数 | 四设备 HPM 软件批已审核通过；multi driver 分批接入 | 1/2/3 个干扰设备、每设备 1/8 页，加同总足迹6页对照，共9条件各10次 | 共90次，约25–40分钟，不改既有DS0 BIT |
| 5 | E3 DS0 论文子集 | A2 设计，运行包待能力核查 | 六种大小及三类流量组合的论文子集 | 用户上限 6 小时；以实际软件采集总时长估算 |
| 6 | E4 DS0 页大小 | 用户已授权独立分支修改/编译；A3完成E2采集器后准备 | 4KiB 对 2MiB，各点 10 次，未测 | 需显式软件建图与实际leaf证据；先审设计再实施，不混当前稳定软件 |
| 7 | E6 DS0 角色互换 | BIT/driver 动态角色已核实；collector 16项主机检查通过 | V=DMA1、A=DMA0；Solo/A1/A8 各 10 次 | 约20分钟，旧payload集中测完后再切新软件 |
| 附加 | E5 延迟分布 | baseline v3 无导出样本/直方图；用户已授权，A1完成backend检查后准备最小被动histogram | 已整理历史100试次的1600阶段行/160组；新运行继续复用mean/max，p99/CDF未测 | Root重算通过；不从累计值推造分位数，不改RTL |
| 最后准备 | E6 P/N/PN/P4M 缓解 | 按现有论文清单独立分支准备，尚未分配实施 | 尚无 | 先审统一基座方案，再验证编译；不恢复新机制探索 |

板时暂以既有 DS0 50 次约 785 秒（约 15.7 秒/点）加配置切换与日志开销粗估，不是硬件运行时间或保证。构建耗时不计为占板。能力受阻的任务保持未测，可推进其余合法任务。

E3 P0 已获用户明确回复“批准仅P0最小改动、构建和45点板测”。在新代码/BIT 就绪前继续 E1b；E1b 保持实际旧 pathv3 payload，不提前换成 P0 内核。新 P0 使用紧密页环与新独立长度操作，旧 ABI1/4KiB 行为保留。全三形态、E4/E5 RTL 扩展及缓解构建不包含在此授权内。

核实 E2/P0 都需新软件后，按“同一Linux配置集中测完”的规则调整实际排程：**旧pathv3上的DS25全50点→DS100全50点→旧DS0上的E6角色互换30点→一次更新新共同payload→原DS0 BIT的E2 90点→P0 DS0新BIT的E3 45点**。角色互换因此在更新软件前完成，保留与历史baseline完全相同的Linux，避免后续回写旧镜像。E3完整三形态、E4大页及E5直方图保持未实施请求，不顺带扩围。

## 完成记录

历史 G1 和统一 100 次已完成，保留原始包和身份，不为本队列重复测量。本轮新增采集尚无。

## Git 与交接

Root 分支 `ispass/root-a`，worktree `/home/xvhaocheng/work-root-a`。A1/A2/A3 各自使用独立 `ispass/a1-ds-sweep`、`ispass/a2-workloads`、`ispass/a3-capabilities` 分支。Root 审核并整合到任务分支；最终推送供合并，不自行合入 main。共享 `data/board_results.csv`、`README.md`、`CURRENT_STATUS.md` 的拟合并内容写在任务结果和状态里。

## 后续修改授权更新（2026-10-17 13:03Z）

用户明确补充：“未来还有实验需要修改rtl 和linux. 都可以加载agent进行, 先在单独branch里面修改然后编译. 然后轮流上班测试收集数据”。据此，现有论文实验清单中的必要RTL/Linux改动无需再因原只读/测试程序限制逐项等待：A1在backend证据后准备E5被动直方图，A3在E2采集器后准备E4显式2MiB映射，A2在当前共同软件后准备E3其余负载形态；E6历史缓解移至最后准备。各批先由Root审设计与实际代码，固定源码后验证/编译，验收通过才加入唯一板队列，不混改正在构建的源树。当前P0/Multi软件固定Linux `acece3a3ab7d72ba1f936c1984b82a4a55f28d46`、平台`52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0`，一次共同payload已开始编译。此授权不恢复新的机制探索，也不允许子agent自行争用板卡。
