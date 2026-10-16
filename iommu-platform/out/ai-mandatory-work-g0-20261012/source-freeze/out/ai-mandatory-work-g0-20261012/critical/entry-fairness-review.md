# stage106 G0入口与公平对照：关键接口审查

结论：根提出的“两份分属原生分布式rank的冷MLP包、各直接源→独立目的；整张量FIFO；4KiB/2MiB各soloV、soloA、concurrent，再各一个single-engine coordinator”八案是足够小且有意义的**受控准备阶段G0入口**，条件是来源席确认两个对象的独立消费用途，根接受共同释放和平台投影边界，并先验收私有适配。无需商用GPU、无需猜计算时间；现有stage105二进制不能直接执行这八案。本审查没有构建或运行，不验收性能、普通失效、新颖性或G1。

## 入口合同

- 来源选择由根给定：rank0/layer0和rank1/layer6，各完整OPT125m MLP六个FP16张量。每角色9,447,936B，两个角色共18,895,872B，不能把其中一个省略或只测另一个的窗口。来源席负责核原作者代码、rank/layer归属和消费用途；本席不重复其来源调查。
- `ready=0`表示受控边界：CPU驻留来源图像已准备、两份独立目的缓冲为空且可用、整个有限包同时释放。它继承来源的数据对象和放置任务，**不声称原程序原生同时提交**，不声称FPGA内存容量强迫offload，不加入推理compute delay、token-rate或deadline。
- 并行与隔离案两个实际iDMA为物理执行角色：DMA0/DID10和DMA1/DID11。single-engine案由DMA0/DID10可信协调器顺序准备两个目的，DID11禁用；两个来源rank的独立用途不证明两个物理copy engine不可合并。源码rank用于对象归属；它不证明CUDA流等于DID或原应用共享当前RISC-V IOMMU。两路原始寄存器前端真实存在，配置选择器只支持两路；其余两路不参与本投影。[实际实例](../../../tests/nonblock_actual_dma_top.sv:352)
- 源和目的为不同固定所有权对象：每角色读取自己的源，写自己的目的；两个目的分别表示不同rank的GPU目标放置。共享DDR只是投影底层，不等于原生GPU内存。若另选共享不可变源或零拷贝消费接口，必须明确它是否仍完成相同来源放置合同；不能靠随意禁止合法省工作制造收益，也不能用“FPGA可以读所有DDR”自动取消继承的目标对象合同。
- 合同保留两份完整输出、原张量内部顺序、独立就绪、真实完成、全部排空。性能解释只针对这份受控IOMMU搬运合同；不能推出GPU推理、SSD吞吐、原生分布式应用调度或板上性能。

## 最小适配与实际合法动作

1. 新私有合同按`role × tensor`列唯一任务ID、src/dst IOVA、字节范围和不变release。每角色六个整张量任务，共12笔，最长4,718,592B。旧加载器的`bytes<=4096`、固定中转、硬编码计数和resident-direct仅channel1均须去掉，改成按合同逐字节/范围守恒。[旧限制](../../gpu-ai-fpga-fit-20261012/private/transfer_driver.cpp:220)
2. 整张量是合法普通选项：NUM_BYTES直接形成请求length；wrap的TFLenWidth=AxiAddrWidth且HardwareLegalizer开启；legalizer负责拆合法AXI burst/边界。长job不是一条跨页AXI burst，更不意味着任意长job有4KiB fullbuffer快路径许可。[NUM_BYTES](../../../cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:320)／[后端](../../../cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:306)／[拆分](../../../cva6/corev_apu/iDMA/src/idma_legalizer.sv:232)
3. 调度器须允许两路各一笔活动：先真实轮询双方REG_DONE，与各自NEXT_ID精确匹配，退休后再补本路下一张量；不能旧版全局一笔等完后才配置另一角色。[旧串行循环](../../gpu-ai-fpga-fit-20261012/private/transfer_driver.cpp:377) 当前前端COUNT→WAIT的单活动job是实际实现边界；JobFifoDepth=8不等于八个job并行，AXI在途参数8仍保留。[前端](../../../cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:519)
4. 控制器只用合同、软件记录和真实寄存器结果；`dbg_dma_issue/done`、TL、DDR等仅观测/验收。逻辑role必须成为独立字段：single-engine的role1任务仍由channel0完成，不能以channel推断rank、目的所有权或张量归属。每一笔cfg读写都走原cfg_xact并推进模型，完成另一角色可能发生于这些tick，observer必须始终采样。首个NEXT之前只能配置/延迟尚未提交任务；一旦请求VALID已发布，普通软件不能撤回或中途改变它。[配置接口](../../gpu-ai-fpga-fit-20261012/private/retained_tb.cpp:1490)／[VALID承诺](../../../cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:519)
5. 保留原DDR协议及容量，不新增人工受限槽、不关掉可用AXI在途、不采用必须经过的无用中转。旧Observer按AXI port和bank归属当前活动job，单活动前提下可复用；需把Tensor[6]/channel0填充就绪改为两角色独立12对象验收。[归属](../../gpu-ai-fpga-fit-20261012/private/transfer_driver.cpp:106)／[硬件完成守恒](../../gpu-ai-fpga-fit-20261012/private/transfer_driver.cpp:189)

## 根提出的40MiB映射与48MiB模型预留

四个10MiB槽，按2MiB对齐，依次为V-source、V-dest、A-source、A-dest。IOVA从0x40000000连续安排，PA从0x84000000连续安排；槽起点偏移为0、10、20、30MiB，最后结束40MiB。并行/隔离案两个root各只映射自己的两个槽，源R、目的RW，禁止跨角色目的写入。single-engine案只有DID10有效，映射全部四槽为源R/目的RW；这份任务没有来源要求的安全隔离，可信协调器允许两份输出。各root的地址均落在同一个1GiB虚拟区间；大页PA低9个PPN位须为零。源A位、目的A/D位显式设置，不用页错误或隐含硬件更新制造成本。[PTW对齐及A/D](../../../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:487)

| 项目 | 4KiB | 2MiB |
|---|---:|---:|
| 每root映射长度 | 20MiB | 20MiB |
| 叶PTE总数（两root） | 10,240 | 20 |
| 页表页数（两root，含root/L1） | 24 | 4 |
| 页表字节 | 98,304 | 16,384 |
| DDT另预留 | 4,096 | 4,096 |
| 显式元数据预留总字节 | 102,400 | 20,480 |

4KiB预算为每root一个root、一个L1、十个L0；2MiB为每root一个root和一个L1。预算不包含CPU描述符、payload文件副本、进程内存、OS、运行时和实际板端分配碎片。2MiB在PTW/IOTLB源码中有处理，仍须用实际DUT数据/完成验证，不把支持位图当正确性。[大页缓存匹配](../../../cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:117)

现有dense storage是PA[0x84000000,0x87000000)连续48MiB，内部三片各16MiB只是存储实现。因此四槽可以跨内部片界，不用扩内存；应同时报告“映射槽40MiB、实际模型backing预留48MiB”，另核未映射8MiB与张量洞字节未被改写。[实际连续寻址](../../gpu-ai-fpga-fit-20261012/private/retained_tb.cpp:188) 原driver逐`transfer_dense[n]`初始化、旧to_pa与全区比较都带源/中转/目的语义，须改按槽地址而非重命名旧数组。[旧地址归属](../../gpu-ai-fpga-fit-20261012/private/transfer_driver.cpp:56)

## 八案合同：六个matched对照与两个强普通替代

固定同一调度器、六整张量job/role、源图像、槽/权限、设备上下文、冷reset、CPU-off、PTW/DDR设置、观测代码和统计边界。唯一实验因素是mapping或启用角色掩码：4KiB下soloV、soloA、concurrent，2MiB下相同三案。隔离案保留被动角色的源/目的/页表初始化，不发该角色任务；它只校准被测角色，不声称完成18,895,872B的pair。每个concurrent案必须双方六笔/六张量完整完成、硬件issue/done各6、REG_DONE匹配、全部数据和排空通过。每个solo案按活动角色期望6笔和9,447,936B，未活动角色必须零issue/done且数据未变。

主普通策略就是工作就绪、自由通道即补下一笔的每路FIFO与实际DONE事件反馈。对本次“共同释放、无互相依赖、固定每路单job容量”合同，这种反馈与并发FIFO是同一控制能力，不应为维持对照名称重复运行一个“新反馈”案。另外在4KiB和2MiB各做一个single-engine普通强对照：DMA0把rank0的六张量全部准备后再准备rank1的六张量，二者release仍为0，总12笔和18,895,872B。它完成相同两个独立目的，计入rank1全部排队等待；它是平台投影中的合法合并执行，不能宣称原生GPU具有一个跨GPU copy engine。单root映射40MiB，4KiB共22页＝90,112B，2MiB共2页＝8,192B，另加DDT4KiB。根登记实际权限/PTE/DC初始化成本与DID11无效。它不是matched隔离因果对照，因为物理身份、授权范围和页表预算有变化；它是相同完整服务合同的普通执行替代。两份必需输出不等于两copyengine必需。八案中不再额外追加两个engine串行；不做所有job长度、所有映射与所有策略的笛卡尔矩阵。

逻辑release始终0，容量free另记；禁止把配置开始、首issue或上一job完成重新当作原逻辑release。每job至少记`release, config_start, NEXT_ID返回, hw_issue, hw_done, DONE观测`。每role记录共同t0到最终hw_done、最终DONE观测，另记录两路最终排空。拆出release→issue供给/排队、issue→hw_done硬件服务、hw_done→DONE观测延迟；并发A的启动配置被V先占用造成的偏移不能叫翻译阻塞。[原计时字段](../../gpu-ai-fpga-fit-20261012/private/transfer_driver.cpp:28)

slowdown应在同一mapping内用`T_role_concurrent / T_role_solo`报告，并分别给硬件/寄存器完成版本；再比较不同mapping的绝对时长、slowdown、整包makespan、TL及DDR证据。不拿4KiB并发除以2MiB隔离，不只报告最快角色或V窗口。缺真实应用deadline和计算时长时，不宣称“满足原应用SLO”或仅因为并发比独占慢就裁普通方法失效。

## 运行前关键风险及停止条件

- **长任务watchdog**：4,718,592B至少589,824个64bit读载荷拍，旧每job绝对200,000cycle限制必然可能误杀。按本角色的真实payload/翻译进度做无进展判定，另有基于固定job长度/模型参数的有限总预算；不能让另一角色活动刷新其watchdog。预算不是应用deadline。[旧绝对限制](../../gpu-ai-fpga-fit-20261012/private/transfer_driver.cpp:394)
- **模型容量/顺序**：DDR入口16项读队列，返回按单ordered R队列；B亦按模型队列。该合法但受限的行为会制造/加剧HOL，不能默认为MIG行为。保持八案相同模型配置并如实报告。[队列](../../gpu-ai-fpga-fit-20261012/private/retained_tb.cpp:214)／[容量](../../gpu-ai-fpga-fit-20261012/private/retained_tb.cpp:696)
- **名字不等于独立PTW延迟**：`ptw_latency`也初始化所有DDR读请求的延迟，包括payload；改变它并非单独干预PTW。八案固定值，不据该参数扫点做纯PTW因果结论。[实际赋值](../../gpu-ai-fpga-fit-20261012/private/retained_tb.cpp:1330)
- **CPU-off不等于零软件成本**：本测试cfg绕过CPU crossbar/dma_cfg，未模拟CPU、cache、实际Linux分配/ownership，也未测生成CPU-pinned来源和GPU目标的成本。CFG模型tick在阶段服务指标中计入；源初始化/PTE写入发生在受控release前，要列明其字节/操作数与未计时成本，不能叫端到端净收益。[明确边界](../../../tests/nonblock_actual_dma_system_top.sv:46)
- **完整性优先**：任何数据、权限/范围、NEXT/DONE、AW/W/B、AR/R、TL守恒、VALID稳定或最终quiet失败都先修适配，不能进入性能裁决。两路独立payload签名有利于拒绝跨角色别名/串包；须保留独立输出及原始events。
- **因果结论**：六个matched案可证明这份合同在该RTL模型里的并发影响及对页粒度的敏感性；2MiB同时改变walk数量和元数据DDR流量，不能仅凭时间下降叫PTW唯一原因。至少保留关键TL等待、翻译响应、DDR队列/返回的原始路径证据，明确容量、HOL、互连、供给仍可能并存。
- **停止/推进**：若来源席不能支持两个独立对象/固定消费目的，或原依赖明确禁止共ready而根仍欲称原生并发，则入口不成立，记录该工作流证据缺口。若整张量＋合法大页或合法单engine普通执行已覆盖受控需求且无具体可改残余失效，则关闭此分支G1；这不是否定共享IOMMU主问题。若残余只来自额外复制、配置供给或模型DDR饱和，不把它包装为新翻译门控需求。只有具体未被普通策略覆盖、对双方完整工作有可检验差异的残余问题才进入G1。

执行声明：本席由Codex直接核关键源与接口，没有DeepSeek/Claude、外部来源调用、构建、DUT运行、生产代码或共享文档修改。stage105构建manifest只证明此前冻结的二进制；新私有适配必须另建冻结记录，不能复用其hash冒充未改。[旧冻结记录](../../gpu-ai-fpga-fit-20261012/build-transfer-manifest.json:575)
