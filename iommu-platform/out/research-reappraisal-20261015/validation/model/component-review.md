# research_ar_selector 独占普通组件交付

初稿只新增同目录 `research_ar_selector.sv` 和本文，初稿阶段未编译／运行。随后 root 已编译完整私有模型；本席另受命完成下文组件wrapper适配及四项协议检查。组件和适配由 Codex 编写，未调用 DeepSeek 或 Claude；本席没有改生产源码、旧实验、helper现版或已成功完整模型，也没有新增DMA调用。

已直接核读 `../../../shared-service-control-boundary-20261015/controls/ordinary-program.md:30–125` 的完整8bit逻辑及 `cva6/vendor/pulp-platform/common_cells/src/rr_arb_tree.sv:151–244,260–310` 的 LockIn／RR／AXI-ready 语义。主替换接口对应 `cva6/vendor/pulp-platform/axi/src/axi_mux.sv:333–350`，完整 ID prepend 位于同文件 `:135–181`。

## 固定合同及具体差异

- 同一 binary 仅在 initial 中读 `+ar-ds-first=0/1`；缺省0。其他整数直接 `$fatal`。复位释放后监控拒绝mode改变。只有活动分支收到原下游 `gnt_i`，非活动分支请求掩成0。
- 模式0原样实例化 `rr_arb_tree`，NumIn=4、AxiVldRdy=1、LockIn=1、flush=0、rr=0，ExtPrio/FairArb保留原默认。没有重写其 tie-break 或锁mask机制。
- 模式1逐项对应原普通8bit设计：source3聚合DS；nonDS游标只在nonDS接受前进；有nonDS pending的DS输入接受才累计streak；无nonDS竞争清零，nonDS接受清零，饱和4；下一新选择逃逸；已首次offer的头锁index至接受，整个AR仍由上游保持。额外 `idx_o` 只暴露已选index。
- 原RR锁整个当拍请求mask，DS只锁所选index；两者均依赖合法producer保持尚未接受的头，均不新增数据存储。原设计已明确其非DS tie-break不必逐周期等于原四源RR。
- 原 `AxiVldRdy=1` 的空闲READY可非零，故非法grant检查限定在 `req_o=1` 的有效offer/accept；空闲READY没有请求token。否则忠实RR模式在正常空闲期会被错误判失败。

## 只读观测

每个owner首次采样offer及每次accept输出一行 `AR-OWNER-JSON {...}`，phase分别 `firstoffer`、`accept`。本地cycle从复位释放后第一个上升沿记1；sequence从1起，每次新offer递增，held后accept沿用该序号。同周期offer并accept输出两行。`held=1`表示本拍之前已经有未接受offer；`mask`为当前有效源四位mask的十进制数；`addr`与`packet`为带0x前缀的字符串，`id`是完整ID十进制数，`len`为ARLEN值，非beats。

最终自动输出 `AR-OWNER-SUMMARY-JSON`，包括offer/accept、首次双头offer计数及首个cycle、每源accept计数、held非DS遇DS计数及其中DS在该初offer时尚不存在的late计数。`first_dual_head_cycle=0`表示从未发生；运行终止时`held=1`表示还有未接受offer，组件不自行断言任务应已全部完成。

独立算术参考监控原接受史产生的F4计数与nonDS游标，并核对新选择优先、逃逸及轮转。全packet held稳定、source有效、onehot、grant/ready、fullID高两位与source、所选data及offer/accept守恒均用直接 `if/$fatal`，不依赖ASSERTSOFF宏或Verilator SVA使能。监控数据副本、64bit计数、日志与参考逻辑只供私有模型，不计入物理硬件费用；8bit只是DS控制编码，整个双模式binary更不是面积测量。

## 初稿交接时的验证清单（实际补验见下）

1. 编译实际ID-prepended AR struct，确认包路径、DataType字段、SV cast、final与打印在选中仿真器可用。尚未编译，不能称语法已过。
2. 模式0与未替换的原RR在相同合法输入流下逐周期输出对齐，包括req_o=0的ready行为和multi-cycle stall。模式0应没有人为DS动作。
3. 模式1覆盖4次连续有竞争DS accept后的nonDS逃逸；其间下游stall不得当作accept；nonDS撤尽后streak清零；DS-free nonDS RR；三个nonDS持续竞争的0→1→2轮转。四源覆盖不能仅由旧source2/3任务推断。
4. 先offer nonDS并保持、DS晚到时保持原完整AR到accept；改变任何held字段或撤回VALID应触发direct fatal，ASSERTSOFF下亦然。另测新offer和accept同拍，确保sequence守恒。
5. 确认只有指定DDR四输入AR处接入本组件；原AR spill、AW/W/B/R、行为DDR FIFO law和全部任务完成检查均不变。AR accept是spill输入接受，并非MIG接受或R完成；只凭这里的差异不能声明完整任务性能改善。
6. 对非法plusarg运行一次负例。完整任务结束时root须检查最终quiet、offer=accept、held=0及其他R/W/B和数据结果；组件没有新增quiet硬件口。

## 后续组件适配与实际结果

安装版本为Verilator 4.038。root原组件attempt1因不支持 `--binary` 失败，未产生仿真；`component-build-attempt1.log` 和 `component-results-attempt1.json` 保留原字节，保存清单记录SHA。无延时端口wrapper改由 `component_test.cpp` 驱动，使用 `--cc --exe --build` 和 `-CFLAGS -std=c++17`。本席attempt2到达链接后因缺少旧runtime的 `sc_time_stamp()` 失败，未调用组件；保留attempt2日志／结果及驱动版本。唯一一次针对该诊断的恢复补入时间hook，attempt3成功构建。

随后恰好执行四次组件调用，core dump limit为0：RR-held返回0，2个offer／2个accept；DS-F4-held返回0，接受源顺序为 `3,3,3,3,0,1,2,0,2,3`，10个offer／10个accept；两者最终held=0且各记录一次held非DS遇到late DS。DS中的前五次新选择实际同时有DS及nonDS pending，第四个DS accept之后逃逸source0。

changed-held负例在source2首次held后只改变qos，进程实际返回−6，原始日志明确报 `AR owner changed a previously offered held full packet`；非法mode=3实际返回−6，日志明确报参数仅允许0/1。正例调用 `top->final()` 后输出summary。所有四个预期结果接受，见 `component-results.json`，命令、退出码、尝试次数、保留原件及SHA见 `component-execution-ledger.json`。

此组件域覆盖F4连续有竞争接受、三个nonDS循环顺序、首次offer后DS晚到仍保持source2、held数据违规拒绝和非法mode拒绝；没有把这组合成输入当作冻结DMA任务出现双头竞争的证据。没有追加完整模型构建或DMA运行。持续竞争下各源长期公平、更复杂的交错stall/reset及所有实际任务的完整R/W/B后果仍只由对应实际证据支持；四项小检查不扩大为穷尽证明、综合时序或性能收益。
