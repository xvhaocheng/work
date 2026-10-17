# Root A 板测与代码线：2026-10-17 JST

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: run
- Verification Status: PARTIAL（E1b、E3 P0、E6角色已完成；E2/E4/E5/完整E3/G按队列继续）
- Updated: 2026-10-17 14:40 UTC

当前唯一板任务是原DS0 BIT加载`board/runs/e2-ds0-load01/`，14:38:06Z启动；继续使用已写入的共同内核`6.5.0-rc2-ispass2027-p0-multi-v1`。启动身份通过后由Root采E2 90点。子agent只做独立分支代码、主机验证和编译，不接触JTAG/UART/SD。

## 已完成的论文数据

| 实验 | 新试次 | 完成与关键结果 | 论文入口 |
| --- | --- | --- | --- |
| E1b DS曲线 | 100 | DS25/DS100各50、五条件各10；两档WNS均+0.177ns，失败端点/routing errors均0。含历史DS0/DS50后最终200试次。 | [完整数据与9个图](../exp/E1b-ds-sweep/final-results.md) |
| E3 P0小子集 | 45 | DS0，64B/1KiB/4KiB×Solo/A1/A8×5，45/45全部通过；14:25:47Z–14:37:05Z，无失败或替换。 | [最终45点与统一CSV](../exp/E3-workloads/checkpoints/p0-final01/README.md) |
| E6角色互换 | 30 | 原DS0/旧pathv3，V1/A0，Solo/A1/A8各10；同boot/block A8/A1时间比中位数1.10434084。 | [角色结果](../exp/E6-mitigation/role-swap-results.md) |

E3关键口径是同boot、同区组、同V大小的A8/A1配对：

| V descriptor | n | 额外时间中位数 | Q1–Q3 | 吞吐损失中位数 |
| --- | --- | --- | --- | --- |
| 64B | 5 | 14.2461% | 14.1921%–14.2633% | 12.4697% |
| 1KiB | 5 | 13.8811% | 13.8760%–13.9157% | 12.1892% |
| 4KiB | 5 | 10.4000% | 10.3991%–10.4506% | 9.4203% |

**一句话判断：DS0上小传输更敏感，A8/A1增幅约14%，4KiB约10.4%，增加约3–4个百分点。** 进一步同区组跨大小配对的时间增幅差中位数为64B 3.8469、1KiB 3.5157个百分点，各5对。逐block数值、公式和case定位见最终包`size-effects.json`。这些是描述性统计，IQR不是置信区间，不作统计显著性或纯翻译归因。每点固定4096个V descriptor，三大小有效工作分别256KiB/4MiB/16MiB，只在同大小内算慢化；A1已含带宽/仲裁共跑代价。HPM是非原子软件区间，A完整工作含启动/排空；DMA2/3无PATH时不能补造逐设备内部等待。

DS口径始终是IOMMU数据结构访存的附加延迟，DS0为主结果，DS25/50/100只作敏感性分析。E1b同boot/block A8/A1、A16/A1、A64/A1时间增幅中位数：DS0为10.3962%/13.7155%/16.3512%，DS100为76.0753%/88.7220%/98.1181%。E6角色与历史V0各条件中位数的绝对描述差异<0.06%，不作跨boot配对。

所有阶段快照均为同一campaign的累计视图。E1b早期16/50/80新增点与最终200包、E3首9点与最终45包都不得叠计；以实际boot+observer sequence及原命令hash识别重叠。独立DS会话可能已将同一批100新点并入main，后续汇总必须去重。

## 在准备和等待的任务

| 任务 | 实际状态 | 下一步 |
| --- | --- | --- |
| E2 设备数 | 实际Linux acece3已在板；原DS0正在启动。90点collector、完整出口、不可变prefix出口已审；Root实跑prefix44检查通过。 | 同Linux完成90点；A3补六项同block配对离线报告。 |
| E4 页大小+E5分布 | 共同软件首次完整构建passed，Linux da6b69f/platform a68603f；release e4-e5-v1，payload29,363,656B。E5 BIT于14:06:29Z passed。 | E2后更新一次软件、加载E5 BIT，共采20点；E5附加采集，不另跑50点。 |
| 完整E3 | CVA6 1d587/platform dfebcc，Depth64的唯一DS0构建14:10:50Z开始，仍running；ABI5 helper682a2ca已审，adapter46df97c正交验证。 | 最终源码审查、CLI/collector和完整软件构建；自身时序/资源验收后再排≤6小时子集。 |
| E6 P4M G | 从E5 6c5基座独立准备；frontend2c7c68f已审，39情境/207332周期，与真实旧P0默认关闭/启用gap0逐拍比较。 | 接被动whole/window观测与小接口，逐批审查后编译；G固定V/A各4096×4KiB自然完成。 |
| E6 P/N/PN M | 完成历史来源/容量/接口设计与负结果追溯，未启动移植。 | 维持最后优先级，不恢复机制探索。 |

E4/E5冻结包在`board/packages/e4-e5-20261017/`：小型元数据原字节复制，BIT/Image/payload/SPI模块仅本机硬链接，不进Git。E5最终BIT SHA d934846ecb7f2fd80bcb7d2f18889ae0fd8c9bbd28a0f5fef8e949614139221c，50MHz，WNS+0.126/WHS+0.047/WPWS+0.062ns，失败端点与routing errors均0，290520/290520 routable nets全完成。Slice50581/50950（99.28%）；LUT166334、regs142751、BRAM50、DSP27。完整E3和G必须各自验收，不能沿用E5通过结论。

## 执行边界与异常

Root worktree `/home/xvhaocheng/work-root-a`，分支`ispass/root-a`。不在主目录切分支，不修改共享README/CURRENT_STATUS/data表，不自行合main、不强推或重置。已接受代码/验证/论文数据持续推送任务分支。正式最终汇总应选择各任务最新完整包，补共享索引和结果表；原始数据、失败与旧结论保留。

用户已授权现有论文清单必要RTL/Linux改动在独立分支实现、编译、验收后串行上板，并授权途中同步论文可用真实数据。当前无需用户决定；时序失败会停止该版本并报告，不自行改约束或搜索实现策略。现阶段均为Codex工作，未调用DeepSeek；历史报告中的模型归属保留原文。

已保留的实际异常：Root重复DS启动在触硬件前被UART锁拒绝；E5首次输出误放共享out后取消独占进程树（非timing失败）；初P0软件最后SPI link缺module.lds，原失败保留、仅模块恢复后正式delivery通过；合法长度backend通过，零长度backpressure负例仍failed且CLI/driver/RTL三层拒零；普通PIE ASan启动失败未冒称通过，non-PIE有限检查另存。上述不作为性能试次或被替换点。

P0结束前的纯主机续接session27823以143终止，原因未建立，未启动E2硬件；独立P0 collector仍45/45成功。Root确认原result=0后才手动dispatch唯一E2加载，未重复任何试次，记录见`board/runs/e3-to-e2-continuation01.json`。下一步E2 collector由Root在boot验证后启动，不再依赖该已结束等待。
