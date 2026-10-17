# ISPASS 2027 投稿工作区

目标：多个DMA共享RISC-V IOMMU的**FPGA实测刻画论文**。摘要截止 **2026-12-11 AoE**；全文截止 **2026-12-18 AoE**。正文9页（图表及附录计入，参考文献不计）、IEEE双栏、双盲。[官方要求](https://ispass.org/2027/submission/)。

当前阶段：实施投稿模式切换、历史数据归集、前作精读、历史G1复现、独立修复基线构建及100次板测。研究保持Codex-only；不开发新机制，不恢复旧判别门或停止审查。

## 交付入口

| 交付 | 文件 | 当前状态 |
| --- | --- | --- |
| 实板总表与口径 | [board_results.csv](data/board_results.csv)、[数据说明](data/README.md) | 历史、G1与正式100次分别追源；失效仿真另表 |
| 前作对比 | [prior-work.md](related/prior-work.md) | 两篇全文与有界检索完成，未核项明列 |
| G1及统一基线 | [G1配套包](board/packages/g1-20261017/README.md)、[baseline100配套包](board/packages/baseline100-20261017/README.md)、[运行说明](board/g1-reproduce.md) | G1九次通过；统一100次全部完成，三个实际启动 |
| E1b DS四点曲线 | [四档五条件统计/IQR](data/ds-sweep-summary.md)、[执行报告](board/ds-sweep.md)、[实验包](board/packages/ds-sweep-20261017/README.md) | DS25/DS100新增100次通过，复用旧DS0/DS50；200次形成四点曲线 |
| 统一100次离线补充分析 | [三个问题与表格](analysis/baseline100-extra.md)、[复现脚本](../../analysis/scripts/baseline100_extra.py) | 合计吞吐及跨窗界限、DS0读写四阶段、分boot与合并离散度/10000次bootstrap CI；原始数据只读 |
| 大纲与图表清单 | [outline.md](outline.md) | 已完成十图清单及行映射 |
| 可复用上板流程 | [skill](board/skill/genesys2-iommu-board/SKILL.md) | 实测流程已固化，通过仓库级发现链接启用；仅显式调用 |
| E3负载形态准备 | [能力与变更请求](exp/E3-workloads/capability.md)、[矩阵/预算](exp/E3-workloads/run-package.md)、[未执行说明](exp/E3-workloads/subset-results.md) | pathv3/DS0 batch固定4KiB且V/A共享chunk；依用户修改范围停止，新板测0次，三类硬件流量尚未实现 |

实板长表共27544个标量行（历史4096＋G1 828＋统一100次12480＋E1b新增100次10140），失效仿真另表980行。
新基线的十组中位数及关键稳定ID见[结果摘要](data/baseline100-summary.md)，
全部来源与单位见[数据说明](data/README.md)。

## 执行合同

- 历史G1：batch DS50/50 BIT＋00082 payload，V0/A1，4KiB×4096、预热256，Solo/A1/A8轮转各3次。硬件时间中位数相对56.5/107/155ms偏差不超过2%；新配置不套用此阈值。
- 统一100次：原版加必要修复、明示批量供给和被动path观测；DS0原始直连、DS50额外延迟诊断。各DS下Solo/A1/A8/A16/A64各10次；DS50五区组→DS0十区组→DS50五区组，每区组五条件轮转。
- E1b：用户单独授权DS25/DS100补点；同一固定源码、同一已写SD的pathv3，仅切BIT，DS25全部50次→DS100全部50次已完成。图2的DS曲线采用同区组Ak/A1比值的中位数与IQR，旧DS0/DS50不重测。
- 最新执行偏好：以后同配置全部测完再切另一套，100次默认DS50十区组→DS0十区组，两种DS共用Linux，不再默认回切。本轮提出调整时已完成前25次并进入DS0，保留既定实际顺序补完余点，不重新测一轮。
- Codex负责构建、JTAG加载、UART控制及板测，沿已验证的恢复/返回流程直接更新SD、启动与采集；不格式化或重分区，不覆盖历史输出，失败原样保留。
- 设备数2→3→4、三类负载及传输大小、4KiB/2MiB、延迟分布、统一基线缓解复测仅列待补，本次不自动执行。
- 运行前复用manifest记录实际源码、overlay、BIT、内核及命令身份。未知项写unknown；不以现存文件hash倒填历史实际加载验证。

[切换前状态原文](provenance/current-status-before-submission-mode.md)保留历史。动态执行状态见根[CURRENT_STATUS](../../CURRENT_STATUS.md)。

用户最新执行偏好：直接传输、更新、启动与采集，不增加重复哈希、SD回读或独立确认。实验结果、源码commit、image和BIT由[配套实验包](board/packages/README.md)的manifest关联；本机`artifacts/`保存二进制，Git保存元数据、结果和源码恢复材料。G1历史构建含dirty树，完整实际执行树的单一commit保持`unknown`，不拿当前HEAD补齐；新基线的固定commit及两BIT/同一payload配对见baseline100包。
