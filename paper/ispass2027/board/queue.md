# Root A：FPGA 板测队列

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Origin Date: 2026-10-17 JST
- Verification Status: UNVERIFIED（下表区分构建、准备与实际采集）
- Version Label: root-a-board-queue-v1

本轮板卡唯一调度与执行者为 Root A。子 agent 只准备代码、构建和分析；任何时刻只有一个 JTAG/UART/板测任务。各工具继续使用既有 UART 独占锁。此文件仅在 `ispass/root-a` worktree 维护，不修改主目录或共享结果表。

## 当前资源与顺序

板卡已通过本轮 UART 身份命令确认仍为 DS50 + `6.5.0-rc2-ispass2027-pathv3`，boot `b1eaa85f-01d7-4976-9dac-b57cc8af49b7`、taint=0，四个 DMA 节点齐全；原始回执见 `runs/root-a-current-identity01/`。最后有效 load 为主工作区 `board/runs/baseline100-20261017-01/ds50-last-load/load.json`；未切换 BIT。两个已有 DS 构建均由原调度进程继续运行，Root A 接续，不重复启动。

| 顺序 | 任务 | 状态 | 采集规模 | 预计板时 / 条件 |
| --- | --- | --- | --- | --- |
| 1 | E3 DS0 小子集 | 用户已批准仅 P0 最小 driver/RTL、构建与45点；A2 分批实施 | 3 个 V 大小 × Solo/A1/A8 × 5 = 45 次 | 约45分钟采集，另需新构建；packed-ring，不扩三类完整形态 |
| 2 | E1b DS25 | 构建运行中，A1 接续验收 | Solo/A1/A8/A16/A64 各 10 次 = 50 次 | 约 20–30 分钟，须 50MHz、WNS≥0、违例端点和 routing errors 全零 |
| 3 | E1b DS100 | 构建运行中，A1 接续验收 | 同上 50 次 | 约 20–30 分钟，同一档集中完成；两档仅换 BIT，复用已写 pathv3 |
| 4 | E2 DS0 设备数 | 当前 driver/UAPI 只有一名干扰者；A3 整理最小变更请求 | 1/2/3 个干扰设备、每设备 1/8 页，加同总足迹对照，各 10 次 | 不以多进程绕过全局实验锁；保持未测 |
| 5 | E3 DS0 论文子集 | A2 设计，运行包待能力核查 | 六种大小及三类流量组合的论文子集 | 用户上限 6 小时；以实际软件采集总时长估算 |
| 6 | E4 DS0 页大小 | A3 只读能力核查 | 4KiB 对 2MiB，各点 10 次 | 若需内核/driver 改动则提交 change-request 等决定 |
| 7 | E6 DS0 角色互换 | BIT/driver 动态角色已核实；A3 准备独立 collector | V=DMA1、A=DMA0；Solo/A1/A8 各 10 次 | 约 20 分钟，保留动态角色的 16 条 PATH 与排空检查 |
| 附加 | E5 延迟分布 | baseline v3 无导出样本/直方图；准备最小被动 histogram 请求 | 复用现有 PATH mean/max；p99/CDF 未测 | 不从累计值推造分位数，不改 RTL |
| 不执行 | E6 P/N/PN/P4M 缓解 | 只写方案/change-request | 无 | 等用户决定，不启动 RTL 构建 |

板时暂以既有 DS0 50 次约 785 秒（约 15.7 秒/点）加配置切换与日志开销粗估，不是硬件运行时间或保证。构建耗时不计为占板。能力受阻的任务保持未测，可推进其余合法任务。

E3 P0 已获用户明确回复“批准仅P0最小改动、构建和45点板测”。在新代码/BIT 就绪前继续 E1b；E1b 保持实际旧 pathv3 payload，不提前换成 P0 内核。新 P0 使用紧密页环与新独立长度操作，旧 ABI1/4KiB 行为保留。全三形态、E4/E5 RTL 扩展及缓解构建不包含在此授权内。

## 完成记录

历史 G1 和统一 100 次已完成，保留原始包和身份，不为本队列重复测量。本轮新增采集尚无。

## Git 与交接

Root 分支 `ispass/root-a`，worktree `/home/xvhaocheng/work-root-a`。A1/A2/A3 各自使用独立 `ispass/a1-ds-sweep`、`ispass/a2-workloads`、`ispass/a3-capabilities` 分支。Root 审核并整合到任务分支；最终推送供合并，不自行合入 main。共享 `data/board_results.csv`、`README.md`、`CURRENT_STATUS.md` 的拟合并内容写在任务结果和状态里。
