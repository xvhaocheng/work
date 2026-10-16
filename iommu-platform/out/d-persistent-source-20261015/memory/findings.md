# Ramulator2 持续服务状态：有来源，信息优势与部署费用仍缺

2026-10-15；ARS-Codex fact-check / source-verification；Codex-only。请求 Sol/high，实际模型／强度运行元数据不可取得，记 unknown。本席只核一个官方实现族；没有执行模拟器、编译、建新模型、恢复实验或改全局文档。

**结论：Ramulator2 的开行、关行计数、未到期时序约束和刷新进度确实可以跨“用户请求已排空”继续存在；给定完整物理入口信息 O_phys 时，选定实现可确定性重放。这里的 O_phys 不等于真实主机可见的完整 O_host。** O_phys 包含公开冷启动时刻、精确 memory tick、完整 controller accepted 物理请求（包括 PTE 与 payload）、同 tick 顺序和配置。普通源码回放在这些条件下能重建状态；不能据此推出 SPDK 主机 submit／CQ 历史 O_host 已足够，或 O_host 下没有隐藏信息。O_host→O_phys 的实际映射、获取成本与在线推断成本均未核。实际 MIG 的状态、读写耦合、可见接口与费用均未知；既有 M0 的读写独立负结果与 MIG paused 均保持。

## 固定来源及阅读范围

选官方 [v2.0a / be93be78055d922aa1d4d33e15bcc8f2b0c61a9d](https://github.com/CMU-SAFARI/ramulator2/tree/be93be78055d922aa1d4d33e15bcc8f2b0c61a9d)，而非漂移的 main（查询时已是 2.1）。官方 [论文 arXiv v2](https://arxiv.org/pdf/2308.11030v2)，Luo 等，2023-11-29 修订，已读摘要、§1、§2.1／2.1.1、§2.2、§3.1；§3.2读过但不用于部署费用。没有把 §3.3 RowHammer 评估当本问题证据，也没有宣称全文精读。PDF 两个 URL 的字节一致，SHA-256 `407e16c90d25286dbb9380e22b0c850568339ac3657c5b582094fb547c46e135`。

论文 pp.2–3 的接口、请求→调度→命令→状态更新时间流程与源码对应；p.4 §3.1 是作者报告的 Micron Verilog 命令合法性验证，不是本次运行、真实系统吞吐校准或 MIG 等价性证明。论文与仓库同一作者群，不能算两项独立外部验证。源码／论文均为第一手材料；硬套医学证据等级只能记 VI（单项描述／实现研究），不由此降低源码对“自身行为”的直接证据效力。未见本次读范围内的掠夺出版信号；未单独核 CAL 出版元数据／撤稿数据库。作者自评自身工具有 intellectual COI；资金／财务 COI 未核。

全部下载原字节、固定 Git blob、SHA-256、读取区间、官方链接见 [sources.json](sources.json)。聚焦的八个核心实现文件为 controller、scheduler、refresh、rowpolicy、DDR4、node、action、preq；补读配置、地址映射与接口／初始化／录迹文件以免臆测可见性和复位路径。没有 clone 仓库。

## 源码核实

以下路径均位于本目录 `source/`，行号保留原文件。

| 问题 | 源码事实与限制 |
| --- | --- |
| 实际示例配置 | `example_config.yaml:13–38`：GenericDRAM、1 channel／2 ranks、DDR4_8Gb_x8、DDR4_2400R、Generic controller、FRFCFS、AllBank、ClosedRowPolicy cap=4、RoBaRaCoCh。示例 frontend 是 SimpleO3 + RandomTranslation（1–10），不是 IOMMU／PTW；后续若按物理内存入口核对，不得声称运行了原示例 frontend。 |
| 队空后开行仍在 | `action.h:17–26` ACT 建开行，PRE 清开行；`DDR4.cpp:496–521` 普通 read/write 映射 RD/WR，并不自动关行。`basic_rowpolicies.cpp:72–117` 无已选命令就 return，不因 idle 关行；列访问累加到 cap 才向优先队列放 PRE。故 cap=4 下 1–3 次列访问后排空，可保留行及计数；OpenRowPolicy:10–21 完全不主动关行。 |
| 队空不等于冷状态 | `node.h:45–71,134–188,209–232` 保存每层命令 ready clocks／有限命令历史；命令发出后按约束更新时间，后续 ready 判定与当前 tick 比较。队空没有清除这些字段；旧 deadline 到期后不再限制服务，原始历史字段仍在不等于仍有性能影响。 |
| 刷新持续发生 | `all_bank_refresh.cpp:12–55` 从 tick 0 起每 nREFI 向每 rank 放一次刷新请求，next+=nREFI，不抽随机 phase；controller:171–204 每 tick 都调 refresh。`preq.h:45–69` 若有开行先 PREA；`action.h:74–110` PREA 清行，REFab 进入 Refreshing，结束回 Closed；`DDR4.cpp:183–243,467–471` 保留 future action 及 nRFC 阻塞。因此 host 请求队列空仍可能处于刷新或等下一次刷新。 |
| 读 PTE 与写 payload 的服务差异 | `DDR4.cpp:112–115,442–489` 不认识 PTE 语义，只认识 read/write；同 rank／bankgroup 有 RD↔WR 转向与 bank 开／关行约束。`generic_dram_controller.cpp:325–399` 先 active，再 priority，再按水位选 read/write 队列；priority 头未 ready 时会阻止普通队列。故该实现能支持有地址和时间条件的读写互扰；不能据此推翻项目原生 M0 或证明 MIG 的通道拓扑。 |
| write-mode 是否永久债务 | controller:18–20,55–56,325–335：默认低/高水位 .2/.8；完全空闲时 mode 可变 true，但新 read 到达且写队列为 0 时同次选择改回 read。它不是空队列后仍独立拖慢读的证据；上次 WR 留下的 DRAM ready deadline 才可能暂时影响下次 RD。 |
| 严格排空边界 | controller:9–14,206–220,298–317：须含 read/write、active、priority、pending read；用户完成或普通读写队列空不够。WR 发出 final command 后移除，无 write callback；不能把 send(true) 或写队列空当实际 AXI B／DDR 写入完成。future refresh action 也可能仍在。 |

## clear、warm、idle 与 reset 的区别

**已核事实：** `memory_system.h:24–72`、controller.h:31–51 和 DDR4 的所核接口没有 IOTLB clear/warm/reset 操作；新实例的 `clocked.h:21`、refresh:12–37、node:54–71、rowpolicy:50–67 建立初态，`DDR4.cpp:196–208,608–613` 创建新节点，`factory.cpp:65–111,152–160` 经构造器创建组件。**新建完整实例再 connect/setup** 才是本族可追到的冷启动路径；不是重新调用现有对象 init()，也不是清请求队列。

**推论：** 外部 IOTLB clear 若只改翻译缓存，就不会直接清这个内存对象；但 clear 的耗时会推进 tick，可能跨刷新。IOTLB warm 若发生实际 PTE 读取，它本身会 ACT/PRE、推进 cap、耗掉转向约束并改变刷新相位，所以“warm 后保持原状态”必须逐条算，不可当不扰动准备。短 idle 可保留未关行；跨一个已完成的全 rank 刷新会关掉旧行并清列计数，再等既有 timing constraints 到期后，旧请求相关的有效服务记忆可消退。周期刷新相位仍在，且在公开启动 epoch 下可计算。不会声称任意固定 idle 长度无条件洗掉一切。

## O_phys 可重建，不推出 O_host 已足够

**源规则推论（未执行验证）：** 对固定上述配置、无插件随机源、已知冷启动与确定同 tick 调用顺序，物理入口的 accepted 请求序列（完整地址、读写种类、入队 tick／顺序）决定队列、FRFCFS tie、命令、行状态、计数、ready clocks 和刷新事件。`linear_mappers.cpp:23–49,74–92` 给确定地址映射；scheduler:21–54 按 ready 再 arrive 比较；内存 tick 先 dram 再 controller（generic_DRAM_system.cpp:75–80）。将这些充分输入统称 O_phys（含公开配置／epoch），则相同 O_phys 不能仅靠私改内部行／phase 生成不同世界。这是 O_phys 的确定性结论，不是 O_host 的信息充分性结论。所有选定配置、epoch、种子／随机来源（本控制器无；若保留 RandomTranslation 则其映射与种子也须公开并另核）必须对所有策略相同。

按示例 64-bit channel width、8n prefetch 与所核映射计算，64B 为交易粒度；物理地址的 column 为 bits 6–12、rank 为 bit 13、bankgroup 为 bits 14–15、bank 为 bits 16–17、row 为 bits 18–33（1 channel 无 channel bits）。这是由源码配置推导、未运行验证的位映射；不能移植到 MIG。它使给定完整物理地址时可以辨认本模型的同 bank／同 row；但主机 O_host 是否能取得全部 PTE／payload 的实际入口地址与时序未核，不能据此判断项目是否需要新增接口。

项目真实可见的完整 O_host 指 SPDK 主机 submit／CQ 等历史，必须与 controller accepted 物理入口轨迹 O_phys 分开。中间缓存、翻译、互连重排及可能的其他请求使 O_host→O_phys 并非已证等同；本次没有核实其实际映射、获取方式与成本。因此现有证据既不能认定 O_host 已足够，也不能认定必有会改变动作选择的信息缺口。若另证 O_host 能确定全部 memory arrival，才可把 O_phys 的重建推论传回 O_host；否则须明确缺的是何级事件。不能删去 O_host 原有信息或私造随机相位，回放成本也不能默认免费。

## 可见接口、费用与强普通对照

- 真正外部库接口为 send 的接受／拒绝、read callback、tick 和 get_tCK（memory_system.h:44–72；controller:113–160,298–317）；内部 `IDRAM::check_ready/check_rowbuffer_hit` 是模拟器 C++ 组件接口（dram.h:83–124），不是硬件传感器或主机可读寄存器。
- 官方 TraceRecorder 可输出选中命令／地址／tick（trace_recorder.cpp:16–58）；默认示例 plugins 为空。将其用于校验内部真值可以，将其当生产系统免费观测不可以。论文模拟速度不能作读取 Z、MMIO、CPU 预测器或模式切换费用。
- 可明确使用的服务费用仅是该模型的 timing 规则，例如 tRFC=360ns／tREFI=7800ns 的表项及按 tCK 转 cycle（DDR4.cpp:390–435）；这不是项目 DDR 实测。新观测的采样、传输、同步、CPU、状态存储、陈旧与决策时延；取得 O_phys 及普通回放的维护成本；准备/等待/模式切换的真实成本均 unknown，不填合成常数。
- 强普通基线至少包括已实现的 FRFCFS／active 优先／维护优先／读写水位，以及 OpenRowPolicy、cap=4 和 cap=1 的关行控制。cap=1 是每次列访问后排 PRE，不等价零成本瞬时关行。改变 row policy／watermark 是否可在真实 MIG 配置中合法做到，仍未知；不能默认给任一方额外可编程能力。
- 普通等待须逐步推进同一刷新与时序状态，并计完整完成时间；“等到约束成熟／公开刷新结束”是可执行候选，不能仅把等待当无效成本。给定 O_phys 的回放目前只能作有条件的可重建基准，不能直接当作 O_host 下已经可部署的普通策略；普通物理布局／bank 分离和合法提前准备也必须纳入对应权能的对照，不宣称已经比较过。
- 版本细节不能改写为理想语义：`request.h:47–65` max_size 默认32，但 enqueue 的 `<=` 会允许第33项；水位仍乘32。无修复／运行。本问题无需靠撑满队列制造正例。

## 最强可保留版本与决定性否证条件

**可保留的来源版本：** “翻译层 quiet/clear 并不必然让下游内存冷却；普通 warm 会改变行状态。某些 PTE read 与 payload write 因映射、开行和读写转向／刷新时机而成本不同。”其意义是给 D 的服务来源和实验排空语义补实据，不是新状态接口已有独有收益。

如果能另取一项真实来源，证明真实可得的完整 O_host 无法及时推断某个仍影响动作排序的状态，并给出低成本可实现接口及对 FRFCFS、实际可由 O_host 实现的普通预测／回放、合法 row/write 控制和等时间普通等待的完整收益，才有继续发展信息贡献的条件。当前来源只证明已知 cold reset＋完整 O_phys 时确定，并未检验 O_host 的信息充分性，也未证明 O_host 下的正／零信息价值。

具体否证：①实际由完整 O_host 可实现且计入费用的普通预测／回放已精确区分状态（单有 O_phys 离线回放不满足此项）；②warm／排空到合法控制点已消除相关行差异；③收益只来自模式切换耗时导致转向／刷新冷却，等时间普通等待复现；④所谓持久 debt 实为尚未排空的 active/priority/pending 或 refresh future action；⑤换成 cap=1／普通水位或布局即可解释；⑥新增观测费用超过可得收益；⑦实际硬件读写本就独立或映射不同。每项只否定其覆盖的版本。

**下一最小检查建议（未授权实施，本席未做）：** 若 root 需要验收这条来源，可在固定原实现的物理请求入口做一个小型命令迹检查：普通读／写前史后严格排空，短 idle，检查保留行与 cap 计数；跨已完成 all-rank refresh 后核关行；再让只读完整 O_phys 的回放重建同一迹；这只是条件重建检查，不能当成 O_host 普通策略已可部署的证据。所有配置／epoch／请求／种子相同且事前保存源 hash。它验证来源与可重建边界，不应先扩大成新的 Z 选策／参数搜索。若当前目标必须先证明新增信息与部署费用，则应先补实际入口可见性与接口资料，不能以此检查充当该证明。

文字范围修正（2026-10-15）：明确区分 O_phys 与 O_host；未新增来源或执行。`artifact-hashes.json` 保留首次产物快照的 hash，不代表本次修正后三份正文的当前 hash。
