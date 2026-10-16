| HPM RTL 与既有仿真 | `cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_hpm.sv`；`tests/hpm_test_top.sv`、`tests/hpm_test.cpp` |
| 四 DMA/DID/PID/地址窗口 | `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv` |
| IOMMU 容量、DS、FPGA 顶层 | `cva6/corev_apu/tb/ariane_soc_pkg.sv`、`cva6/corev_apu/fpga/src/ariane_xilinx.sv` |
| iDMA 与 active cycles | `cva6/corev_apu/iDMA/`，由 CVA6 跟踪，没有独立 Git |

## 已确认的实验范围

后续讨论已修正目标：**争取方法论文，因果表征作为方法设计的前半段，不提前把终点限定为表征论文。** 本次重审 E0～E4/P0～P5 与 v1，文档补入 P4M 方法原型/直接基线/双方代价/冻结规则后验证，并明确 E2 阶段执行依赖、P2 分层校准；具体方法尚未选定。用户随后要求限制软件增量；预热/映射记录、A 间隔、UT_REQ 分母、运行记录器及新执行模式均为按需候选，不是自动待办。

[复审](iommu-platform/docs/experiment-v1-review.md)发现两项可复现问题：① 旧模式仅将 DID 混入高位，小 chunk（1/2/4 字节）短试次漏检错设备源；已改为每字节带唯一 DMA 实例编号。新增 96 个正常/错源组合在旧代码检出 36 项失败，修复后全部通过，原 12 个调度场景也通过；驱动目标交叉编译通过。smoke 每路增加 8 页中前 4 页各拷贝一次、校验其余未触及页，无新接口。② HPM 对全局 valid 取沿，连续 UT_REQ→DDTW 可丢后者；生产 HPM 单元复现得 0/预期 1，有低 valid 间隔的对照正常。RTL 尚未修，当前六槽未选择 DDTW，不能据此说六槽全部失效。S1/S2 与 IOTLB miss 同处产生，不是独立 PTW 访存量或占用时间。原缺陷证据 [out/review-v1-20260909](iommu-platform/out/review-v1-20260909/README.md)仍对应修复前版本；修复验证见[验证记录](iommu-platform/docs/validation.md)。没有新板上数据；交付镜像已于 2026-09-15 更新。

用户在独立审查任务中确认优先“尽快形成一篇扎实投稿”。[本轮审查](iommu-platform/docs/research-review-2026-09-09.md)建议首篇只打通一条能改变设计选择的因果链，不先实现所有机制/措施；保留 E0～E4 既定判据和 FPGA 主线。新增明确点：W 同时改变物理数据工作集与闭环到达率，事后匹配 A 吞吐可评措施代价但不自动识别纯翻译效应；需落实 A 预热相位和独立试次统计。补查 TPDS 2017 的软件管理 IOTLB 非阻塞先例，进一步限制泛称 Hit-under-Miss 创新；其机制并非当前硬件 PTW 的等价实现。上述均是审查建议，无新颖性确认、源码修改或新实验数据，用户尚未选定具体新主张。

用户将本任务作为投稿导向的并行思考支线，推演问题、緩和措置和シナリオ，并要求实质创新。最新[论文逻辑](iommu-platform/docs/paper-logic.md)收敛为“缓存保护何时足够，何时还需干预共享服务”，沿用 E0～E4，优先区分替换、翻译中等待与翻译后出口等待，再选择最小必要措施。性能反转和等待预算仅保留为可选分支，不是必须出现的结果或已选实现。当前**没有已确认的创新点**：RTCSA 2025 已测 DMA/IOMMU 干扰，MASK/DWS 已有翻译感知反馈与组合控制，本次补查 STFM/ATLAS 也排除了“改为时间计费即创新”。下一步收敛少量因果实验卡及措施适用条件，在相近 A 进度下评价 V；仅更新研究文档和交接，未改代码、构建或产生实验数据。

已补充微基准定位：机制与限定范围的预测可主要用微基准；同点重复、冻结规则后测试未见参数、真实应用外推是三层证据。真实 workload 不是当前研究前置条件；声明应用收益时再补对应证据。额度选择须先预测后验证，穷举最佳点仅作离线参照。默认 V 两个映射页/纯循环可能只呈容量阶跃，q 分区与额外时序控制尚未实现。

已按用户要求将面向教授的简短讲稿改为[日语 Wiki 条目](iommu-platform/docs/experiment-v1-report-ja.md)，涵盖实验计划、v1 设计与实现状态。Notion 连续返回授权重试，未能读取或写入用户所指当前页面；页面同步仍待完成。本次仅更新文档风格，未改源码、重建或产生板上数据。

用户希望组会能讲清 access pattern 和实验逻辑。已增加上表讲解稿并定向核对当前驱动：V 固定页对、A 顺序循环；每路一个软件未确认完成描述符；预热按 V 次数，切换时仅 V 回到偏移 0，A 相位继续。该次补充仅改说明，没有新构建/板上数据；实际地址与完整翻译上下文目前不在 JSON 中，不能宣称跨试次物理轨迹已固定。

完整 E0 校准、E1 性能、E2 翻译状态、E3 阻塞、E4 路径/软件对照已提前规划；不是等 v1 结果出来再临时设计机制实验。当前仅完成 E0 准备/E1 的最小软件入口，不代表整套实验或 P2 板上校准已完成。

新版只支持 4 KiB 基础页、顺序循环、Solo/双路；V 完成固定工作量就结束自身测量窗口，A 在预热和测量期间由同一 CPU 持续轮询补交，之后停止并排空。默认 V 源/目标各 1 页，A 源/目标各 8 页，chunk 4096，V transfers 4096、warmup 256。这些是试跑默认值，不是正式采样矩阵。

- 主时间为首个 V 被测提交前到最后完成被软件观察到，含 CPU/MMIO/调度开销，不含 A 尾部排空。
- `dma_active_cycles` 是 backend 非 idle 累加，不是 IOMMU 专属等待。
- HPM V/A 各三槽，精确 DID 过滤、独占会话、保存恢复；`hpm_configured` 只表示配置回读成功，板上事件归属尚未校准。快照非原子，边界外 A 事件可能计入。
- 补交间隙是软件观察完成到下一次提交触发读完成，含提交 MMIO；不等于硬件空闲时间。软件 in-flight 不证明请求重叠。
- 失败会停止补交、排空；无法确认安全时保留映射/缓冲并隔离设备至重启，禁止继续实验。最终数据校验不证明每个重复描述符都实际正确搬运。
- 新 ABI 查询/运行是 0x06/0x07；旧 0x01～0x05 返回 ENOTTY。旧 Solo/Pair/Multi/Once 当前均已删除，不能继续写“旧 independent 已实现所以可直接使用”。
- 新程序没有 random、大页、BARE、四路独立流量、硬件请求重叠/阻塞观测或自动 slowdown。E2/E3 所需控制与额外观测未实现；Page Batching、IOTLB 预留、Hit-under-Miss 等仍是候选缓解方法。

旧论文是 Accelerator-side IOMMU 的 Remote Page Walk 性能评估；英文稿在 `/home/xvhaocheng/thesis/2026_thesis_eng2/`。FPGA 模拟远端访问代价，不代表真实多 Chiplet/PCIe P2P 系统已搭好。

## 当前版本与本轮验证

下表为最小软件修复后的本地提交；Linux、顶层 gitlink 与 lock 一致，CVA6/IOMMU 未改。已有日语报告、研究草案及复审文档保留，部分文档尚未提交；交付镜像已按下表源码于 2026-09-15 更新，构建清单如实记录顶层文档 dirty 状态。

| 仓库 | 分支 | 提交 |
|---|---|---|
| 顶层 | `codex/idma-experiment-v1` | `0bb9851` |
| Linux | `codex/idma-experiment-v1` | `031e91dbadc115dafac957067135002b74035a10` |
| CVA6 | `platform` | `7f64251542f9eaf2e80dced55d87c9d6e6a2ab79` |
| IOMMU | `platform` | `da8a8011f2c52fb1e0c5fd8ae9b9757a5f0ffbb5` |
| OpenSBI | `platform` | `e8d21bfdc2dd753314627968e496c3a1188f6dfb` |

本轮起点顶层 `151c9a7ef2f2aea707c8f587b9b1906691a65430`、Linux `2ef3f4b82afd3d2b039d6c67f214e50f27f10198`，均干净。完整软件从顶层 `ffe55a40175937122b3d6be92a6c005099f16dfd` 构建，起止干净；结束提交仅更新文档，不改变被测源码。所有提交仅保存在本地，没有推送。

已通过：

- CLI 解析/ABI/错误传播 65 组。
- 动态抽取真实 DMA 调度函数的 12 场景，含独立结束、循环补交、预热隔离、超时/中断排空和 quarantine；三类临时注入回归被检出。
- 真实 HPM 控制逻辑 25 组，含六槽配置故障、状态恢复、互斥和进位；检出并修复两个索引宏参数未括住导致 Aggressor PTW 读错寄存器的问题。
- rootfs 白名单/去重/设备节点 4 项；内核目标交叉编译、相关样式和 diff 检查。
- `make check`：21 个组件干净，所有指针/lock 一致，无 alternates。
- `make software JOBS=12`：新用户程序、rootfs、Linux Image、DTB、lloader、OpenSBI payload 全流程通过。DTC 原有 interrupt provider 缺少 `#address-cells` 警告仍在。
- 交付目录全部 SHA256 校验通过；原 BIT/MCS 哈希未变。

证据：`out/manifests/software-migration-4dma.json`、`out-experiment-software.log`、`out/experiment-v1/{delivery.json,SHA256SUMS}`。主机 harness 不验证真实 DMA 映射/MMIO 时序、RTL 事件准确性或板上压力。

## 产物和历史验证

`out/experiment-v1/` 集中保存新版 `fw_payload.bin`、Image、platform.dtb、rootfs.cpio、lloader、实验程序、smoke、lock、manifest、日志和 SHA256。它是软件产物集合，**不是已确认布局的 SD 整盘镜像**；不能假设所有文件都要拷入卡。

覆盖软件前已备份到 `out/pre-experiment-v1-20260908T124529Z/`（含 archive.json）；旧 out/bench 可执行文件移入其中 legacy-benchmarks。旧 `out/migration-4dma/delivery.json` 是历史软硬件组合，其软件哈希不对应新版；当前组合看新 delivery.json。未删除 FPGA 报告/产物或旧源码历史。

硬件为 Genesys 2/XC7K325T、RV64 CVA6 50 MHz、timer 25 MHz；4 DMA，DID 10～13、PID 20～23，IOTLB 16，DDTC/PDTC 各 8，HPM 31。

- `migration-4dma`：DS 50/50，BIT/MCS 已构建，沿用作明确标注的校准候选。
- `baseline-4dma`：DS 0/0，配置已准备，未构建/上板验证。正式样本不能混用两个配置。
- 构建器只支持四路拓扑，改 DMA 数量不能只改 JSON。
- 历史 HPM RTL 测试覆盖 pulse、held-valid、idle、DID filter、inhibit；相同测试检出旧 RTL 10/预期 1 的错误。这不是完整 IOMMU 系统仿真。
- FPGA WNS 0.177 ns、WHS 0.038 ns、WPWS 0.062 ns，总负余量 0；仅表示现有约束满足。仍有 6 项 methodology Critical Warning（TIMING-4/6/7/27）及外部 IO delay 缺失，详见验证记录。
- FPGA 实际导出源码 2,332 个跟踪 blob 与锁定提交一致，见 `out/migration-4dma/staged-source-audit.json`。旧 manifest 的顶层结束状态已单独说明，不据此否认实际源码审计。
- BIT/MCS 和 routed timing/methodology 报告在 `out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/`。MCS 步骤会重建 Vivado 项目，别去已消失的 `ariane.runs/impl_1` 找报告。

## 构建、备份与工作视图

常用：`make check`、`make software JOBS=12`、`make hpm-test`、`make fpga PROFILE=migration-4dma`、`make bundle`。不要为熟悉仓库重跑完整构建；`make check` 会写 out/manifests，并非严格只读。

本机工具：Vivado 2023.1（`/tools/Xilinx/Vivado/2023.1/bin/vivado`）、riscv64-linux-gnu GCC 11.4、riscv64-unknown-elf GCC 10.2、Verilator 4.038、Python 3.10。RISCV 默认 `/home/xvhaocheng/RISCV/riscv_tools`。许可证完整地址 `2100@opus4.apal.cs.waseda.ac.jp` 曾直连验证；短主机名 opus4 曾解析失败。没有新增 SSH 转发或 systemd 服务。

组件修改须先提交，再依次更新父 gitlink 和 `sources.lock.json` 并提交；IOMMU 的链路是 IOMMU → CVA6 → 顶层。构建检查拒绝脏组件或不匹配指针。FPGA 导出目录已存在时拒绝覆盖，需按构建说明保留旧结果。

来源：CVA6 `zero-day-labs/cva6 feat/iommu-refact` 锚点 `1f992d80`；IOMMU `zero-day-labs/riscv-iommu mc/dti-ats` 锚点 `c80bba7`，不是当前平台版本。保留接口修复、AW 优先大传输修复、HPM edge 修复、四 DMA 地址译码及 OpenSBI timer 修复；不要切回锚点覆盖个人更改。

没有个人远端，直接 clone --recursive 不保证取到本地提交。`out/source-bundles/` 和 `platform-archives/restore-check` 已验证的是迁移阶段版本，**尚不含本轮实验修改**；要迁移新版先导出新 bundle。本轮没有重复完整离线恢复或覆盖旧 bundles。

历史备份 `platform-archives/legacy-20260908` 保留 22 个旧仓库历史/diff/未跟踪文件/产物。`platform-archives/migration-tools` 是一次性脚本。均未删除。

工作视图由外层 `.vscode/settings.json` 和 `iommu-platform.code-workspace` 控制，关闭自动展开并限制扫描，隐藏 OpenSBI/小依赖/恢复副本。尚未收到用户 Reload Window 后界面确认。组件裁剪和“缩减成 5 个 Git”只讨论过，用户澄清为界面需求后没有执行，也不是待办。apb_timer 在实际硬件中、tb 内有 SoC 包、vendor/modified 有在用 RTL；不能整目录删除。RV64 未实例化的两个宽度转换 IP 属于 Debug Module，不是 iDMA。

## 后续接手

软件交付已于 2026-09-15 更新，包含小 chunk 修复及新增 smoke。设备准备后使用 `out/experiment-v1/`，准备启动和日志通道，跑 platform-smoke（四路无预热/有预热 Solo、多页部分覆盖，加 DMA0/1 双路角色互换），随后做 E0：设备/HPM 归属、窗口与预热敏感性、真实压力校准。warmup=0 不自动等于 cold，命令成功不等于机制证据。独立时钟约束审查仍未完成。

E2/E3 已有设计但无状态/请求控制实现。IOTLB 按上下文/页号等匹配，不能由 DID 不同断言条目独立；AW/AR FIFO 等待不能仅凭入口 stall，PTW active 未接父层；DS delayer 汇合多类数据结构请求，不是 PTW 专属干预。上游 vsim/类式 VIP 可按需参考，当前只验证 HPM Verilator 流程。

需要历史决策原文时定向查源任务 **整理论文工作文件夹**：任务 ID `01a0806d-e7d2-7092-964c-0edd0919d17e`，host ID `remote-ssh-discovered:dell`。不要默认重读完整聊天。本轮未创建新对话。

## 2026-09-16 SC 修复构建完成（最新结果）

新 BIT/MCS 构建已通过，退出 0；完成 UTC：2026-09-15T15:56:47.509135+00:00。

BIT：`/home/xvhaocheng/work/iommu-platform/out/migration-4dma-scfix/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit`；11443722 bytes；SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`。

正式 routed timing：WNS 0.177 ns、WHS 0.010 ns、WPWS 0.062 ns，TNS/THS/TPWS 均为 0。已设置的时序约束满足，但 6 项时钟 Critical Warning 及 IO/CDC 审查待办保留。

源码与 54 场景回归已通过，旧 BIT 未覆盖。新 BIT 尚未上板；Linux 登录和 DMA smoke/HPM 尚未验收，下一步用新 BIT 重新配置板卡，SD 不改。
