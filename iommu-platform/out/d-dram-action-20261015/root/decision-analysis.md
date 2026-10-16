# D完整准备判别：当前前史对不进入P/PN完整选策实验

2026-10-15，D-DRAM-ACTION-CONTRACT-1。ARS-Codex experiment-agent的有界来源投影与确定性正确性验证。研究继续Codex-only；源码核查请求Sol/high，实现和独立核账请求Sol/medium，实际运行的model/effort元数据未知，主会话未声称原位切换。

**四个完整COPY准备案例与独立核账通过；同一模式内，两条前史经过所选准备后，有效DRAM服务状态相同。因此停止这版“准备前行状态差异→准备后新增观测→选择P/PN”的扩测，不让这对前史进入完整P/PN选策比较。** 准备期间轨迹确实不同；本轮没有构成同观测、不同动作偏好的状态对，也没有证明整个D方向无价值。稳定判定见[接受记录](root-result-acceptance.json)。

## 决定的问题与来源

上一轮已经证明：在固定Ramulator2实现中，普通读前史留下的开行/cap可以穿过严格排空和idle，但会被冲突访问、cap关行或刷新改写。那只回答“来源状态能否存在”，没有回答“真实任务准备后还能否保留”。[上一轮裁决](../../d-dram-boundary-20261015/root/decision-analysis.md)保留，不把几次读warm替代完整COPY准备。

原M0的准备程序是：建立物理镜像，RTL复位，读DONE基线，V完成256次4KiB COPY、A按实际完成反馈重供给，drain剩余A，核对数据与批状态，配置A/V两批，最后才取t0并启动64/64测量。P或PN从复位起生效。准备段没有独立IOTLB clear，也没有warm后完整check_quiet；原计时排除了配置、warm和尾drain。旧CSV从t0后开始，不能恢复warm请求的实际顺序。[源码/旧记录核查](../preparation/findings.md)逐项列出当前C++及旧日志行号、字节绑定和未知项。

因此本轮新建有界投影：复用历史Verilated功能核的精确archive/header/runtime，用当前原始COPY反馈程序的私有C++副本连接固定DRAM模型。保留64/64批配置、gap0、A8、warm256和原guard，在测量t0之前退出。没有调用旧包装器，也没有重跑旧M0测量。

## 事前冻结的模型与完成域

[冻结合同](preparation-projection-contract.json)先于实现与四次运行，SHA256为`bc1bdea49b7f6d983dc8582cb70e77b006eeb4dd0e295a8dc5b66c48c72468c7`。固定Ramulator2 v2.0a commit `be93be78055d922aa1d4d33e15bcc8f2b0c61a9d`，DDR4_8Gb_x8/DDR4_2400R、1channel/2ranks、FRFCFS、AllBank、cap4、RoBaRaCoCh；直接复用上一轮已建库。

- 两条普通读前史分别触及row0或row0后row1；统一DRAM tick256启动功能RTL复位，DRAM不复位。这个保留边界是本轮声明的来源投影，不能说成原生接口已执行的动作。
- 每个parent触及的每条64B线各有独立child，不跨parent合并。读child实际callback后才提供8B AXI R。实际写须完整接收8拍全strobe数据，才能供给对应child；只接收8B INCR、对齐完整写线、非atomic输入，越域直接拒绝。
- 库的真实WR issue不会回调或保留写恢复token。本轮B采用私有推导数据末：每个自有child实际WR后`nCWL12+nBL4=16`tick成熟才可B；继续保留至`WR+34`的恢复债务。它不是库已有写完成回调，也不是MIG/AXI或板端完成保证。
- 末端须全部自有R/B已消费、WR服务与恢复成熟，五个provider容器/future全空、无Refreshing、下一刷新在未来，功能队列空，RTL in_flight0/job_quiet3/CPUquiet。每个harness tick推进一次DRAM，是声明的模型时钟关系，未校准硬件。

参见[写完成源级卡](../completion/completion-card.md)、[桥接合同](../completion/bridge-contract.json)、[root实现审查](implementation-review.json)和[私有实现/构建记录](../implementation/README.md)。历史功能核只核定产物身份，没有证明其等价于当前生产RTL；生产Linux/RTL/ABI均未修改。

## 四例结果

| mode/前史 | V/A COPY warm完成 | warm ticks | strong DRAM tick | 额外strong等待 |
| --- | --- | ---: | ---: | ---: |
| P / row0 | 256/257 | 1100597 | 1100970 | 0 |
| P / row1 | 256/257 | 1100597 | 1100970 | 0 |
| PN / row0 | 256/257 | 1113567 | 1113940 | 0 |
| PN / row1 | 256/257 | 1113567 | 1113940 | 0 |

A257是反馈供给的实际结果，没有强行写死供给数。每例在t0及64/64 measurement launch之前退出；表中只代表该私有投影的准备消耗。不能与旧内存模型的周期混作性能比较，也不能据P/PN准备时间差推出完整任务偏好。

在各模式的pre_t0与strong drain，32bank的row/state/cap、controller write_mode、有效命令时序、公开时钟和下一刷新全等。仅bank[0,0,0,1]的3个历史项和6个ready期限存在raw差异，全部已过期；ACT长度4窗口也按实际`nFAW26`成熟。完整warm仍留下开行，并非把一切清成cold。[终态比较](effective-state-comparison.json)、[phase摘录](phase-excerpts.json)、[独立历史检查](../verification/history-and-source.json)给出原始定位。

准备过程的六类规范化序列均不同：物理请求、DRAM命令、cfg、硬件完成、实际R和B。两mode均可在最早V L1 PTE读0x40400看到row1命中：RD2035，对比row0的PRE2035/ACT2051/RD2067；首个O_phys差异是同地址0x50000读的tick2200对2168。它们不是同O_phys重放，也不能称O_host同观测对。[首差与raw哈希](../verification/evidence-bindings.json)保留完整见证。

本轮未捕获全部RTL/IOTLB内部条目，终态等价仅限所核DRAM服务域；不推断整个系统世界等价。即使内部有其他差异，也不能用它挽救这版“保留的DRAM状态”论证。

## 验证与实际执行账

独立checker流式核查8,663,148行CSV，四例合计2,052个COPY warm、每方向8,404,992B载荷、131,328个实际写child、4,104次B。逐parent/child核W覆盖与数据、接受与所有权、RD→callback→R、WR→数据末→B→恢复、唯一返回、配置反馈、刷新、终态排空。原功能A数据校验保留，并新增完整V源/目的512word校验。真实DRAM命令与官方trace逐条一致；服务数目另由小trace核闭合。

每个P案例961次、每个PN案例955次B已发生但恢复尚未成熟，说明B不能单独充当最终恢复边界。峰值AR4/AW4/line credits192，低于冻结16/16/1024。5种独立负变异各在四例实际小状态上被同checker拒绝，共20个变异案例：重复callback、B提前到WR issue、缺WR却B、缺W却B、恢复未成熟即retire。AR/AW size/burst由运行guard约束，未在raw独立记录，不能扩大为独立观测证明。

[正式核账](../verification/README.md)列出程序、report、实际receipt和原文件hash。checker由独立子席实际执行；root直接读关键原始与变更源码、完整checker，核phase/早期命令和状态比较后接受，没有重复整轮模型。checker开发时修正REF end的−1及DONE基线；root探索记录误用nFAW20后更正26，[初稿与更正](analysis-corrections.json)保留。均未触发模型重跑。

实际4次新prepare-only run，均退出0、无case重试；4次私有host构建尝试中3次成功链接、1次因多余线程runtime失败。0次新DRAM库构建，0次RTL生成/构建，0次旧实验重跑，0次完整64/64测量，0次硬件操作。[执行账本](execution-ledger.json)绑定64个运行前输入及四例原始输出；完整输出留在`implementation/runs/`，不覆盖历史结果。

## 局部停止与下一决定问题

本轮排除了**选定两前史在固定完整准备后仍有不同有效DRAM状态**这一版本。它没有给出免费新信息价值、传感器价值、P/PN完整目标收益，也没有新的论文创新主张。前作阅读不重复；普通公开history/layout预测器和已有配置/复用/扇出/突发信息仍是后续必须同等供给的强对照。

下一项限定为[D-BOUNDARY-SOURCE-2](next-minimal-contract.json)，尚未执行：从真实驱动与持久任务/缓冲区来源中，确认一个正常task/phase切换边界究竟允许什么动作、保留什么状态、每次必须付什么准备成本。先分清一次性试验初始化与每任务准备，明确quiet/config/IOTLB clear、生命周期、最早可取得信息；比较同信息公共history/layout预测与一个具体可达新关系。Linux可请求warm次数，不意味着可以降低固定M0 warm256来追正例。

只有真实边界与未被普通信息解释的缺口得到来源支持，才事前冻结正常输入、两种合法动作、全部完成/guard/时序及同成本wait/replay对照，再谈完整动作实验。找不到时给出明确D优先级裁决，保留证据，不继续无限扫状态；不自动转I或A。新观测采集/传输/存储/CPU/切换费和应用完整生命周期偏好仍未知。

D仍是当前主线，I维持轻量not_ready，A5/A2及独立工程成果保留；10月15—28日窗口不重算。MIG goal1792023314保持paused，WNS−0.391ns、598个setup失败、0次新load，实际债务未知；本轮没有恢复原生、FPGA、板测、collector或MIG。
