
## 2026-10-04 Dell远程N工程验收更新
原版15/15已完成并回Dell。N串行新BIT仅短Solo/A1数据正确，但接收计数全0，因此机制验收不合格；短A8未正常返回，剩余12未执行，命中开启版未上板。旧输出全部保留，不自动重跑。计数接线作用域错误已在Dell工作树修复，尚未重编；A8停滞原因仍诊断中。
已停止对应host runner并JTAG切回已知可用原版BIT；实际UART确认Linux00093，boot956fa8d0-0bf3-4104-a8b1-2af736624a01，taint0。恢复记录results/nonblock-pilot-20261004-restored-status，UART已释放。无需更新SD，后续由Dell远程继续测试/编译/JTAG；勿把本次坏BIT用于正式实验。


## 2026-10-04 N v1 修复构建中（Dell 自动推进）

原版对照15/15完成；v0短Solo/A1计数全零、短A8停滞，未通过验收，失败记录保留。已恢复原版BIT，Linux00093，boot956fa8d0-0bf3-4104-a8b1-2af736624a01，taint0，UART释放。

Dell已修观察器作用域接线，并给双银行控制器增加读请求接纳约束；有限共享返回模型36场景通过，但不等于真实DMA/DDR覆盖，也未最终证明板端A8根因。串行/命中绕过两版v1 BIT正在完整编译。结束后经JTAG依次上板，各15项；原版15项不重跑，技术错误立即停止。当前不需要重编Linux或人工SD更新。命中绕过是否激活、是否改善性能、创新性均尚待证据。

DeepSeek起草仿真/工具/文档，Codex审查关键RTL并实际执行验证、编译与板测。


### N v1两版BIT编译和最终时序通过，未加载

新版文件在 `tools/nonblock-pilot-20261004/banked-serial-v1.bit` 与 `banked-hit-v1.bit`。两版WNS均+0.177ns，建立/保持/脉宽失败0；旧IO全面审查仍未完成。当前FPGA仍为恢复的原版Linux00093，尚未重新加载这些新BIT。

Dell新增双突发重叠模型的固定来源优先模式banked12/12、legacy12/12通过，正在补两路请求轮询交错顺序，随后才决定是否上板。无SD更新要求。DeepSeek起草用例，Codex审查并实际执行。


### N v1功能拒绝；Dell准备v2整份缓冲方案

追加轮询交错重叠模型：v1 banked8/12失败、4个Solo通过，legacy12/12通过。v1 BIT虽时序通过但功能未接受，尚未加载。当前FPGA保持原版Linux00093。有限模型V第二段R填满3beat缓冲，但第二段AW未进入，挡住当前W owner A所需后方R/PTW；不能当作旧板端A8最终根因。

v2决定只对DMA0/1采用BufferDepth512（每路4KiB）；S/N同配置，DMA2/3与原版仍3。当前冻结batch硬件要求num_bytes4096且源/目的页对齐，前端整份完成后才发下一份，支持这份整缓冲的工作量边界。现有Linux/reader/ABI可沿用，仅BIT更新；尚在仿真准备，尚未开始v2构建或板测。额外数据存储8144字节与随动metadata资源要如实报告，此方案已有前人工作，不称创新。


### N v2有限模型通过，两版完整BIT构建已启动

512深度+双突发重叠+轮询顺序的banked与legacy有限模型各12/12通过；不是完整实际DMA/DDR仿真。两v2 BIT正在Dell完整构建，最终时序/资源待测，尚未传输或加载v2。当前仍为恢复后的原版Linux；SD无需更新。v1已整体归档 rr-rejected-v1-builds，Mac v1文件未加载且功能未接受。S/N最终同512深度，原版15项保持历史3深度作为布局+缓冲合并开销参考，不重跑。构建正常日志不巡查，结束后必要时序验收，再按冻结计划JTAG各15项。


## Dell续作：N v2布局失败，v3局部RAM修复中

v2 S/N两版FPGA编译因LUT/控制集资源超限失败，未生成接受BIT、未上板。v3保留每参与DMA4KiB容量，改用已有FPGA RAM存储分支，待实际FIFO验证及两版重新编译。Mac仍保持恢复原版00093；已传v1 BIT功能不接受，不要加载。无需更新Linux或SD，后续由Dell经SSH/JTAG继续。


## N v3两BIT已完成，开始S板测

实际FIFO9/9验证通过；资源超限修复，两版最终约17.4万LUT，内部时序失败0。S原构建BIT生成后Vivado报告打开崩溃，复用产物补齐MCS，不重新综合。当前banked-serial-v3.bit及banked-hit-v3.bit已传到tools/nonblock-pilot-20261004，原Linux00093/SD不改。S/N按冻结各15项运行，技术首错停、不自动重跑，原版既有15项不重跑。


## S v3 15/15完成，开始N v3

S版15项exit0/problems空/taint0，原始结果已回传Dell；Linux00093，boot1c4d6878-a1ea-41a5-b472-e8fdcfbad136。原先短A8停滞现已完成，长A8/A16也完成；这仅是当前试验技术验收，不是通用无死锁或创新证明。随后通过JTAG换banked-hit-v3.bit，固定同一15项流程，遇错停，不重跑。


## N v3 15/15完成：快速接收未触发，Dell分析诊断中

N同15项exit0、检查问题空、taint0，原始结果已回传Dell。累计原版/S/N45项均完整；N窗口fastV/A全0、normal/accepted非零，不能评价绕过收益或直接断言坏了。Dell正在审查机会窗和实际上下文/权限门控，下一步先诊断再决定功能修改。当前N v3 BIT和Linux00093保持运行，串口已释放；无需SD改动，不自动重跑。


## Dell 2026-10-04：v4编译中（无需SD）

v3三版45项通过，但N快速接收全部0，尚未测试到绕过收益。DID10/11实际MSI模式1与v3仅接受模式0的守卫不相容；DDTP模式2本身受支持。Dell已实现普通地址兼容修正及独立被动诊断bank，关键仿真通过，正在编译两份v4 BIT。计划用原Linux00093经JTAG跑S/N v4各15新样本并开启诊断。当前N v3仍运行，请勿自行重跑或更新SD；Dell继续负责后续执行。


## Dell自动推进：v4完成、v5验证中（2026-10-04）

v4 S/N各15项均通过且已回传Dell，无数据错误、taint0；N全部候选/探测/快速接收为0，不能评价加速收益。当前N v4 Linux00093运行、UART已释放。Dell正在验证v5受限整任务缓冲读接纳，尚未开始FPGA编译；之后将继续SSH/JTAG板测，不更新Linux/SD。原始目录serial-v4-01和hit-v4-01均保留，不重跑。


### v5主机验收通过，BIT两版编译中

资格单元、控制器11项（干净构建）和旧守卫/资格开启联合模型各12项实际通过。资格模型A8两组各出现78次实际快速接收，但模型整体耗时未改善，尚无v5板测结果。Dell已启动S/N两版完整FPGA构建，完成并验收后将直接上传新版BIT，使用现有v4脚本/reader与Linux00093继续各15项新试验；无人工SD操作。


### 用户最新停止要求

完成当前v5 S/N两套板测、分析结果并更新文档后暂时停止，不自动启动下一轮实验、代码或编译。当前Dell两套恢复构建仍在等待返回；无v5板测结果。此要求覆盖此前循环至SD的要求。


### 当前v5：S完成、N执行中

S v5 completed15/failed0/runnerexit0/无新增内核警告，run c76ccc3fac1ff68a，boot d103c6f7-b74a-487b-adac-20b46e0ebbd2/Linux00093/boottaint0，整目录已回传Dell。N v5 BIT已上传并启动同样15项，当前UART归N runner使用；不另开串口。昨夜进展说明已在Dell docs/research-overnight-progress-20261004.md落盘。N结束后分析两套本轮结果、同步记录并按用户要求停止，不进入下一轮。


## 2026-10-04 v5两版完成并分析：按用户要求暂停

S/N各15/15、runner退出0，原始目录均已回传Dell；离线汇总exit0/completeTrue/quality_issues0，无新增数据错误/内核警告、taint0。N run e7497b8802cb9e58，finished_at07:10:06Z（JST16:10:06），boot c2bd10b5-26e7-4de6-ac4f-d3a04c14498c，Linux00093；N runner已退出，UART释放，Linux/SD不变。

N快路径A8每轮实际接收6144/6146/6146请求，A16每轮8193；Solo/A1及S为0。长组每条件3次：N/S V速率A8+4.15%、A16−2.69%；N对原版A8−14.52%、A16−22.09%。A8三轮104.263/92.987/93.048 MiB/s全保留。路径已实际工作，但不能宣布改善成功；不同BIT/新启动与缓冲差异限制因果归因。旧HPM/miss/PTW不用作新结构指标，fast不是命中率。

凌晨method88公平对照88/88完成，相对最强匹配简单方案四格均负，该固定A8实占主方案路线结束。后续N线original15+v3 30+v4 30+v5 30=105完整通过，原版复用不重复计，早期失败/部分/未跑与模型另存。

总进展说明复制到docs/research-overnight-progress-20261004.md；v5汇总与复核报告在results/nonblock-pilot-20261004-analysis-v5/。DeepSeek起草说明/结果报告，Codex直接审查关键证据、执行实际试验与离线分析。当前任务已经按用户“这轮实验完成后,分析结果后暂时停止”要求暂停；不自动重跑、新增实验、改代码或编译。Linux继续保持，UART可供后续用户操作。
