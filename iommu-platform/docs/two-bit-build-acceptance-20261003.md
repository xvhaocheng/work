# 两 BIT / 共用 Linux 验收记录（2026-10-03）

本次是实现、主机验证和构建交付；尚未在 Mac/FPGA 执行，不能据此声称方法收益或论文创新成立。

## 实现与版本

- 共用 Linux `6.5.0-rc2-00093-gc0f5e13ea81f`，软件构建平台 `481bd47fd9d62de212ed7eb4619a0554c05bc12e`。
- CVA6 初版 `6f968ec980e663046b89383b00d3dc85ce3e176f`；E2 role1 使用 Vivado 输入 net 声明修复 `a0497a8dac660f8feb19d0f486d4862777276ed3`，E2 构建平台 `10fd189ee705c2b14983c513c295e381fc997e2d`。
- 方法 role2 base（当前构建退出 0，base 时序为负，不作为交付）：CVA6 `a1b08934391833aed21e6ad63b545634040808ac`，构建平台 `7430772309dbed9453b505eb97f43c2a3f54f141`。base WNS -0.150 ns、TNS -0.179 ns、2 setup 失败、WHS +0.028 ns、WPWS +0.062 ns、0 hold/pulse 失败、0 组合环；base 产物与 manifest 原样保留。当前方法保留 `policy_abort` 寄存 sticky 错误 + 显式 abort（仍立即关闭准入），并在 ARM 命令接受分支内直接 pulse research arm；未增加普通计费路径延迟，普通计费规则不变。物理优化已退出0，最终WNS +0.048ns、WHS +0.028ns、WPWS +0.062ns，建立/保持/脉宽失败端点及组合回路均为0；使用优化产物交付。历史方法 role2（未作为交付）：CVA6 `475f3e195039ff2ce691598ac86f43a073ec4de6`，平台 `9a172d2a24842b316e6a7975a24fe69848ff44fe`，详见下文历史构建记录。
- 三个锁文件仅 CVA6 提交不同；保留原软件构建及锁文件，不重复编译 Linux。
- 新接口保留旧 ABI；两 profile 的软件、时钟、设备映射相同，通过能力区区别 role1 E2 / role2 方法。

## 主机验证（已实际执行）

所有证据在平台 `out/two-bit-20261003/`，完整 DeepSeek 回复与失败修订也保留。

| 范围 | 结果 | 主要证据文件 |
|---|---|---|
| E2 计时 | 116 + 窄位宽 2 检查通过 | test_dma_research_e2_timing.py.revision.log |
| 研究观察器 | 312 + 窄位宽 2 检查通过 | test_research_observer.final.log |
| 控制器账目、边沿及错误 | 3744 + 窄位宽 618 + 关闭模式 5 检查通过；组合反馈修复后通过 | test_research_policy.registered-abort.final.log |
| frontend 准入/反向与控制器连接 | frontend 6 场景、真实frontend连接的10仿真场景通过；A flags Launch1 Observer0 | test_idma_research_frontend.py.log；test_idma_research_policy.registered-abort.final.log |
| 版本能力连接 | role1/2 各 24、role0 3 检查通过 | test_research2_caps.py.revision2.log |
| CLI 与旧 ABI | 新 61、旧 77 检查通过 | test_research2_cli.revision.log；test_research_cli.regression.log |
| 驱动 | 实际目标文件编译；配置、能力、未结束策略拒绝、反向覆盖检查通过 | test_research2_config_driver.final.log；test_research2_reverse_driver.py.revision.log |
| 调度器旧流程 | 3 个测试方法包含既有场景矩阵，通过；MMIO 为宿主模拟 | test_idma_scheduler.final.log |
| 冻结计划 | 32 测试通过，含 526 项 CLI 参数和 stub 往返；不是板测 | test_research2_plan.final.log |
| 离线分析器 | 29 测试通过；包含真实记录格式、窗口直方图和缺选择文件时的提前停止 | test_research2_results.final.log |
| Mac 工具 | 36 测试通过，串口/JTAG 用模拟对象；shell 框架用实际 /bin/sh | test_mac_research2.final.log |
| ARM 同拍脉冲修复 | P4M 35、窄位宽 2、研究模式 35 场景；旧观察器 28＋6 通过 | test_dma_p4m_observer.direct-arm.final.log；test_dma_observer.direct-arm.log |
| 打包时序门槛 | 20 测试通过；真实 E2 报告接受、旧方法负时序报告拒绝 | test_package_research2_timing_gate.final.log；timing-gate-actual-reports.json |
| 优化产物溯源 | 12测试通过，包含从优化记录到打包读取及篡改拒绝；真实优化manifest已生成并复核 | test_finalize_research2_postroute.final.log；method-direct-arm-postroute/manifest.json |

旧 gap、batch、受控启动及 capture/Solo 回归也通过，具体日志在同目录。没有把建议测试或模型说明当作测试结果。

关键复核修正：策略直方图总和等于完整活动窗口数 E20，不等于 V 完成次数 E21；Mac 目录切换失败时跳过整条含分号命令；未产生 selection 时仍可分析提前停止的数据；固定 gap 在正式阶段按各目标记录缺额。

真实frontend＋policy小范围Vivado综合回路0：`out/two-bit-20261003/registered-abort-synth/loops.rpt`。

## 构建状态

- 共用软件：完整构建退出 0，manifest passed；payload SHA256 `af9f703b6d00420598282a983201361983b17ed731bc245284b67689dcd9e748`。
- E2 role1：完整 BIT 构建退出 0（平台 `10fd189ee705c2b14983c513c295e381fc997e2d`，CVA6 `a0497a8dac660f8feb19d0f486d4862777276ed3`）；WNS +0.177 ns、WHS +0.028 ns、WPWS +0.062 ns，失败端点 0、组合环 0；旧 IO/clock methodology warnings 仍在。
- 旧方法 role2（历史）：在平台 `10fd189ee705c2b14983c513c295e381fc997e2d` 构建，WNS -0.322 ns、598 个 setup 失败、3 个组合环；未作为交付。其目录保留在 `out/migration-4dma-research-method-v2/fpga-src-before-registered-abort`，旧 manifest 复制到 `out/two-bit-20261003/method-before-registered-abort.manifest.json`；旧 post-route optimization 因无法修复功能反馈在 143 停止，不是交付。
- 旧方法 role2（历史）：在平台 `9a172d2a24842b316e6a7975a24fe69848ff44fe` 构建，CVA6 `475f3e195039ff2ce691598ac86f43a073ec4de6`，构建退出 0 但最终 WNS -3.979 ns、TNS -17075.918 ns、9454 setup 失败、0 hold/pulse 失败、0 组合环；未作为交付。保留目录 `out/migration-4dma-research-method-v2/fpga-src-before-direct-arm`，manifest `out/two-bit-20261003/method-registered-abort-negative-timing.manifest.json`。
- 当前方法 role2（base）：源码已改为在 ARM 命令接受分支内直接 pulse research arm（移除 readback->poll_id->phase_n->research_arm 长路径），平台 `7430772309dbed9453b505eb97f43c2a3f54f141`，CVA6 `a1b08934391833aed21e6ad63b545634040808ac`；完整构建退出 0，但最终 base routed timing 为负：WNS -0.150 ns、TNS -0.179 ns、2 个 setup 失败端点、WHS +0.028 ns、WPWS +0.062 ns、0 个 hold/pulse 失败、0 组合环。base 时序被判负，base 产物和 manifest 原样保留，不作为交付依据。
- 当前方法 role2（最终物理优化）：`out/two-bit-20261003/method-direct-arm-postroute/` 中的优化退出0；最终WNS +0.048ns、WHS +0.028ns、WPWS +0.062ns，TNS/THS/TPWS及三类失败端点均0、组合回路0。实际输入检查点/原manifest/Tcl/执行结果/源锁/输出哈希已复核，独立 `manifest.json` 已生成。最终BIT SHA256 `31e18ee96e6edd47d444ed3cf9f4279cfa1ba494b53497d37183a3d7d0c33651`，11,443,722字节；MCS非空，保留在Dell。原负时序产物和manifest未覆盖。
- 打包脚本现在在创建输出目录前强制读取并校验 role1 routed timing summary；role2 必须使用 authenticated post-route optimization manifest 和优化后的 BIT/MCS/timing，base 负时序只作为 provenance 保留，不阻塞已验证通过的优化。
- 正常编译日志仅保存，没有巡查；失败后查看错误，完成后检查必要时序/资源报告。方法版最终LUT176978/203800（86.84%）、寄存器145056/407600（35.59%）、Slice50908/50950（99.92%）、BRAM50/445、DSP27/840；E2放置报告LUT169689（83.26%）、寄存器140271（34.41%）、Slice50850（99.80%）。不同角色含不同观测逻辑，这不是方法本体开销的公平拆分。

## 上板与解释边界

E2 64 项、方法 462 项按合同冻结；参数选择只用校准数据，边界及反向验证不再调参。主机验证不替代真实 DMA、缓存可见性、板上时序和 JTAG/启动验证。E2 构建仍有旧 IO/clock methodology warnings；历史时钟/CDC/IO 约束审查仍未全面完成。旧 clock/IO/CDC methodology 仍未签署；不要将 stale methodology TIMING-16 warnings 与实际 Design Timing Summary 混淆，打包验收只读取 routed timing summary 的数值行和显式组合环计数。

DeepSeek 起草实现、测试、脚本和说明；Codex 决策、审查关键源码，应用修改并实际执行验证与构建。
