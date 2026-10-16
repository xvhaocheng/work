# composition 裁剪修复交接（2026-10-05）

**当前状态：优化版已通过现有约束下的构建、时序与布线验收，尚未上板。**

本文件记录本次 composition profile 观测裁剪修复、主机测试、完整 FPGA 编译、一次 route 后物理优化及最终验收状态；不重开全项目分析，不改实验计划。

## 1. 旧完整 FPGA 编译失败事实
- 失败表现：31 route node overlap，WNS -3.268ns。
- 实际 critical path：IOTLB `tags_q` → 旧 `dma_research_observer` `event_ram`。
- 整体 LUT 占用 89.8%。
- 结论限定：以上为失败实际 critical path；布线拥塞的完整归因不在本文件重开。详见 `docs/composition-routing-cost-20261005.md`。

## 2. 本次裁剪范围（仅 composition profile）
- 仅当 `IOMMUNonblockMode=3`（composition profile）禁用旧 `dma_research_observer` 与旧 `dma_nonblock_observer`。
- 两旧观测 bank 读出 0，不假造 capability，`research_reverse=0`。
- 保留：PathEnable/P4MEnable/ResearchEnable、通用 `dma_observer`/path 统计、composition 128 rawwords、开/关两缓存快照、ARM/OPEN/CLOSE 与所有 IOTLB/P/N 方法逻辑。
- 新 RTL 裁剪由 Codex 负责。
- 接口冻结依据：`docs/composition-v1-spec-20261004.md`。

## 3. 主机测试草案与已运行结果（已通过，最终优化时未重跑）
- DeepSeek 提出独立输出目录选项、启动包装、10 scenario 实际 observer+composition 联合测试草案；Codex 已应用并实际执行。
- 已运行：`../out/composition-trim-20261005/integration-result.log` —— PASS 10 composition observer scenarios。
- builder 边界：`../out/composition-trim-20261005/build-interface-checks.json` —— PASS 4 边界：拒绝 out 外路径、out 根目录、已存在目录，以及在非 FPGA 操作使用该选项；另有源码边界确认。
- 范围限制：实际 AXI 数据传输、真实 SoC、板上效果不在这两项测试范围内；脚本 PASS 不等于硬件验收。

## 4. 脚本平台变更
- 新增 `--fpga-stage`：仅允许 `ROOT/out` 子目录且目标路径必须不存在，不改变原默认行为。
- 工作区已归档 8 个 method88 文件，两个测试入口的默认生成目录改到 `out/`。

## 5. 完整编译与旧 base 结果（已完成，未接受）
- 完整构建 02:49 JST exit0 并生成 BIT/MCS，但最终 WNS −0.380ns、setup 失败 30，不接受原 BIT。全 273890 线布通/error0，hold +0.052ns 与 pulse +0.062ns 均失败 0。
- 旧 original 裁剪 base 工具 exit0，保存 [base-acceptance.json](../out/composition-trim-20261005/base-acceptance.json) 为 false；即使 bit 非空也不得加载。
- 证据：`../out/composition-trim-20261005/build-state.json`、`build.log`、`fpga-src`。报告与 routed DCP 已移到 stage 的 `work-fpga`，不再使用已不存在的 `ariane.runs` 路径。
- 源 DCP 原路径收尾搬入 `work-fpga`；历史文件移走由编译自身 Makefile 完成，不是被误删除。

## 6. 最终 route-opt 验收（已通过）
- 从 routed DCP 启动一次布线后物理优化：PID 3230656，执行 02:59:41→03:06:49 JST，7 分 8 秒，exit0。
- 优化：单次 post-route `phys_opt_design -directive Explore`；无 RTL 修改、无 constraints/频率变更、未重跑综合或 Linux/SD，未加生产硬件。
- 最终结果：WNS +0.058ns、WHS +0.052ns、WPWS +0.062ns；setup/hold/pulse 失败端点均 0；routing errors 0；fully routed 273893/273893；combinational loops 0；latch loops 0；native bitstream DRC errors 0（warnings 227，继承问题未全部签核 IO/CDC）。
- 资源：routed LUT 163673/203800=80.31%；Register 133604；occupied slices 50670/50950=99.45%。仍非常密集，不能声称面积问题全解。
- 产物：`../out/composition-trim-20261005/route-opt/ariane_xilinx.bit`（11,443,722 B）、`ariane_xilinx.mcs`（31,472,748 B）、`ariane_xilinx_routed_opt.dcp`（133,778,206 B）。
- 最终验收引用：[acceptance.json](../out/composition-trim-20261005/acceptance.json)（accepted=true；scope 限于现有约束下 FPGA build 与 timing/routing acceptance，不等于板测或完整 IO/CDC 签核）；执行状态 [execution-state.json](../out/composition-trim-20261005/route-opt/execution-state.json)，门槛结果 [result.json](../out/composition-trim-20261005/route-opt/result.json)。

## 7. 变更范围与不声称事项
- 无 Linux 源 / ABI / CLI / 计划改动；本次 RTL 修复无需重做 SD。
- 另一对话负责 Linux 加载/网络验收；暂不启动 172。本任务未触碰 UART/JTAG/SD，不自动实验。
- 不声称板测成功、创新成立、IO/CDC 完全签核或已经上传 Mac；本文件也不新增批准流程。

## 8. 记录分工与证据边界
- DeepSeek 负责证据整理、一次性 out 目录 Tcl 与文档草稿；Codex 负责关键 RTL、诊断、门槛审查、实际执行与最终验收。
- RTL 记录仅提供必要的硬件瞬时事实；Linux 负责配置/读取/数据校验；Mac/Dell 保存 JSON、UART 日志、统计与报告。
- 保留原失败构建目录与旧 base 未接受记录。

## 9. 验收规则（本轮已按此完成）
- 编译结束后一次性核查：合法布线、最终 timing、非空 BIT/MCS，之后才验收。
- 禁止循环读取正常日志。
- 开始编译不等于完成验收；本轮 base 未接受，route-opt 已按 `acceptance.json` 接受。
- Explore 支持引用沿用 AMD 2023.1 文档链接一次：[AMD 2023.1 Explore 支持](https://docs.amd.com/r/2023.1-English/ug904-vivado-implementation/Directives-Used-by-phys_opt_design-and-route_design-in-Implementation-Strategies)。
