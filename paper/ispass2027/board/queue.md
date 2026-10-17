# Root A：FPGA 板测队列

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: UNVERIFIED（下表区分构建、准备与实际采集）
- Version Label: root-a-board-queue-v1

本轮板卡唯一调度与执行者为 Root A。子 agent 只准备代码、构建和分析；任何时刻只有一个 JTAG/UART/板测任务。各工具继续使用既有 UART 独占锁。此文件仅在 `ispass/root-a` worktree 维护，不修改主目录或共享结果表。

## 当前资源与顺序

DS25/DS100 构建已完成并验收：两档WNS均+0.177ns，CPU 50MHz，setup/hold/pulse失败端点及routing errors均0。独立会话“构建并采集 DS 延迟曲线”的两档100次已全部完成；Root只读复核并冻结最终200试次包（含历史100）。用户授权协调后已统一排程。Root本地12:56:37Z重复启动被UART锁在触硬件前拒绝，失败原样保留，不计试次。

已完成DS campaign为 `/home/xvhaocheng/work/paper/ispass2027/board/runs/ds-sweep-20261017-01/`，launch回执returncode=0。原DS0+旧pathv3加载和角色互换30次均已完成通过，结果为`exp/E6-mitigation/paper/e6-role-swap-ds0/`。共同软件SD更新于14:17:50Z成功，P0新DS0 BIT及`6.5.0-rc2-ispass2027-p0-multi-v1`于14:25:44Z验证启动，taint0。当前唯一板任务为`runs/p0-ds0-subset-20261017-01/`，已冻结首区组9/45、各cell n=1的真实论文数据。整轮45点通过后再换原DS0 BIT采90点E2。Root被锁拒绝的记录在本worktree `runs/ds-sweep-campaign-launch01/`及`runs/ds-sweep-20261017-01/`，不计为已加载或性能试次。

| 原优先级 | 任务 | 状态 | 采集规模 | 预计板时 / 条件 |
| --- | --- | --- | --- | --- |
| 1 | E3 DS0 小子集 | 采集中；首区组9/45已冻结，所有cell n=1 | 3 个 V 大小 × Solo/A1/A8 × 5 = 45 次 | packed-ring；本轮结束才下最终判断 |
| 2 | E1b DS25 | 完成50/50，五条件各10，全部接受；阶段CSV已冻结 | Solo/A1/A8/A16/A64 各 10 次 = 50 次 | 约 20–30 分钟，须 50MHz、WNS≥0、违例端点和 routing errors 全零 |
| 3 | E1b DS100 | 完成50/50，五条件各10，全部接受；最终四档200试次包已冻结 | 同上 50 次 | 已完成；两档仅换 BIT，复用已写 pathv3 |
| 4 | E2 DS0 设备数 | 多设备驱动/CLI/90点collector已审核，共用软件delivery已通过 | 1/2/3 个干扰设备、每设备 1/8 页，加同总足迹6页对照，共9条件各10次 | 共90次，约25–40分钟，不改既有DS0 BIT |
| 5 | E3 DS0 论文子集 | BTC3/Depth64已审，14:10:50Z启动独立DS0构建；ABI5驱动分批审查 | 六种大小及三类流量组合；新协议等16MiB有效字节 | 用户上限6小时；候选150新点按实际用时定稿，不与P045混池 |
| 6 | E4 DS0 页大小 | E4/E5共同软件首次完整构建通过；20点collector已审并通过Root38+14+11主机检查 | 4KiB 对2MiB，各点10次，未测 | 不混当前稳定软件；与已审E5 Linux后续共同编译 |
| 7 | E6 DS0 角色互换 | 完成30/30，Solo/A1/A8各10；原DS0 BIT及旧pathv3 | V=DMA1、A=DMA0；Solo/A1/A8各10次 | 已完成；同boot/区组A8/A1中位数1.10434084，旧payload不再加测 |
| 附加 | E5 延迟分布 | 被动直方图RTL/Linux已审，独立DS0 BIT最终通过，等待与E4共同采集 | 16阶段×32桶；当前尚无新直方图板测 | 保留carry/overflow/最终有效性，量化分位所在桶区间 |
| 最后准备 | E6 P/N/PN/P4M 缓解 | 原始来源和设计已审，先分配P4M G前端小批；M保持设计 | G候选40点，未测 | G采用两lane各固定4096个4KiB任务；接口/验证后再编译入队 |

板时暂以既有 DS0 50 次约 785 秒（约 15.7 秒/点）加配置切换与日志开销粗估，不是硬件运行时间或保证。构建耗时不计为占板。能力受阻的任务保持未测，可推进其余合法任务。

E3 P0 已获用户明确回复“批准仅P0最小改动、构建和45点板测”。在新代码/BIT 就绪前继续 E1b；E1b 保持实际旧 pathv3 payload，不提前换成 P0 内核。新 P0 使用紧密页环与新独立长度操作，旧 ABI1/4KiB 行为保留。全三形态、E4/E5 RTL 扩展及缓解构建不包含在此授权内。

核实 E2/P0 都需新软件后，按“同一Linux配置集中测完”的规则调整实际排程：**旧pathv3上的DS25全50点→DS100全50点→旧DS0上的E6角色互换30点→一次更新新共同payload→P0 DS0新BIT的E3 45点→原DS0 BIT的E2 90点**。角色互换因此在更新软件前完成，保留与历史baseline完全相同的Linux，避免后续回写旧镜像。E3完整三形态、E4大页及E5直方图按后续明确授权并行准备，验收后单独入队。

## 完成记录

历史 G1 和统一 100 次已完成，保留原始包和身份，不为本队列重复测量。本轮已冻结DS25前16个实际成功试次，见 `exp/E1b-ds-sweep/checkpoints/ds25-progress01/`；该快照含16个新增试次及显式引用的100个历史试次，状态partial，各条件n=3或4，无填补、无部分曲线图。活动campaign继续采集，快照不可与后续累计包重复追加。

## Git 与交接

Root 分支 `ispass/root-a`，worktree `/home/xvhaocheng/work-root-a`。A1/A2/A3 各自使用独立 `ispass/a1-ds-sweep`、`ispass/a2-workloads`、`ispass/a3-capabilities` 分支。Root 审核并整合到任务分支；最终推送供合并，不自行合入 main。共享 `data/board_results.csv`、`README.md`、`CURRENT_STATUS.md` 的拟合并内容写在任务结果和状态里。

## 后续修改授权更新（2026-10-17 13:03Z）

用户明确补充：“未来还有实验需要修改rtl 和linux. 都可以加载agent进行, 先在单独branch里面修改然后编译. 然后轮流上班测试收集数据”。据此，现有论文实验清单中的必要RTL/Linux改动无需再因原只读/测试程序限制逐项等待：A1在backend证据后准备E5被动直方图，A3在E2采集器后准备E4显式2MiB映射，A2在当前共同软件后准备E3其余负载形态；E6历史缓解移至最后准备。各批先由Root审设计与实际代码，固定源码后验证/编译，验收通过才加入唯一板队列，不混改正在构建的源树。当前P0/Multi软件固定Linux `acece3a3ab7d72ba1f936c1984b82a4a55f28d46`、平台`52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0`，一次共同payload已开始编译。此授权不恢复新的机制探索，也不允许子agent自行争用板卡。

DS25全50点已完成且无采集失败，最新累积论文包为`exp/E1b-ds-sweep/checkpoints/ds25-stage-50/`（含DS25新50+历史100，共150试次）。每条件n=10；四档矩阵仍partial，等待DS100。旧`ds25-progress01`仅为历史快照，不与本包重复追加。P0新DS0 BIT最终验收已通过：50MHz，WNS+0.177ns、WHS+0.035ns、WPWS+0.062ns，三类失败端点与routing errors均0。E2/P0硬件与共用软件已全部就绪，按旧payload集中顺序等待上板。

DS100中途快照为`exp/E1b-ds-sweep/checkpoints/ds100-progress01/`：DS25 50+DS100 30共80新增试次，含历史100后总180；DS100各条件n=6，整档未完。该累计快照替代此前阶段包作当前查看，合并时只选最终包，不叠加计数。

E5 hist构建首次输出误放共享main/out，已在早期停止该独占20进程树，旧session退出143，非timing失败；原manifest/log与取消/无遗留进程证据保存，原输出未移动。合规新构建于13:28:41Z在A1独立worktree `exp/E5-latency/artifacts/e5-ds0-build01/`启动，固定SHA/RTL/约束不变；后台编译不占板。

14:01Z排程更新：共同软件写入后直接加载P0新DS0 BIT先跑45点，再换原DS0 BIT跑E2 90；仍是同一Linux、两次必要BIT加载，切换数不增加，提前回答小传输干扰量级。E4/E5共同Linux已冻结`da6b69fc007ad5e671dc288dd8038d02e3ebe43d`，平台`a68603f0fc668771bc0c9e3a86fcefe289bfa17b`；唯一新软件构建已passed，release `6.5.0-rc2-ispass2027-e4-e5-v1`，payload29,363,656B。E5 BIT已进入routing，尚未最终验收。该后续软件不替换当前正在传输的acece正式包。

E5最终BIT于14:06:29Z通过：WNS+0.126ns/WHS+0.047ns/WPWS+0.062ns，全部失败端点0，290520/290520可布线net全路由、routing errors0，DS0直接wire、CPU20ns。正式完整manifest和原报告在`exp/E5-latency/build/completed02/`；BIT SHA `d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c`。完整E3的1d587容量批和固定dfebcc builder已审，允许独立构建；尚未记为该版本时序通过。

full-E3 DS0唯一构建已于14:10:50Z启动，`exp/E3-workloads/artifacts/full-e3-ds0-build01/`（A2独立worktree），当前running；CVA6 1d587、平台dfebcc、显式Depth64，Linux da6只作reference。构建不占板，当前板卡仍仅SD update。

14:22Z更新：SD updater成功返回0后，Root有限续接已在14:17:52Z启动P0 BIT加载；另一个有限续接只在boot身份成功后启动45点collector，无第二上板任务。E4/E5只读运行包已复制元数据并硬链接冻结产物到`board/packages/e4-e5-20261017/`，没有启动硬件或重复hash。E2 immutable-prefix两个纯host出口已交A3实施，完整90点出口仍严格；E6只先准备G(P4M)最小批，P/N/PN的M移植未启动。

首区组论文数据见`exp/E3-workloads/checkpoints/p0-progress01/`：V64/1024/4096B的同区组A8/A1额外慢化分别14.2461%/13.8811%/10.3991%，各n=1、running/partial；不将其当完整45点结果，不与最终包相加。Root有限续接session27823只在P0正式passed且45/45后才加载原DS0、验证同内核并启动E2；任一步失败即停，无自动替代试次。
