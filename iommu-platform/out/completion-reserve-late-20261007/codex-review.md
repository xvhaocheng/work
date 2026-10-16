# 完成阶段准入：共享池被更简单的写前缀对照替代

本轮最强结果来自**不使用共享返回池**的普通准确写前缀准入。Mode7 热 P/PN 为92109/92374 cycles，冷为92549/92814；PN仍比同方法P慢约0.29%。热PN较本轮原大私有缓冲PN93487少1.19%耗时。V/A均完成64次4KiB，完整字节检查及排空通过。此处E2是共享缓冲候选名，不是旧的替换残留实验编号。

## 实际推进与资源口径

Codex先实现每DMA8项元数据队列，在每bank完成AR进入共享完成mux前预约空间。单槽Mode4恢复TL FAST，但数据仍被串行：热P119646、PN123804。普通Mode5保持同元数据队列、每DMA一个完整返回槽，热P96772、PN102746。再实现Mode6精确下一写段流式放行：热P98784、PN99432。池方案改善PN但P更慢。

最后补普通Mode7：保留同样精确写前缀门，完全禁止POOL grant。它比Mode6 PN快7.10%，且不需要2KiB返回池。`storage[1][256]`在Mode7只是未使用数组维度，pool状态始终0；未预约、读写或占用此数组。**活跃DMA payload**是两个3拍、每拍8B私有FIFO，共48B；Mode6为2096B、Mode5为4144B、原Mode0为8192B。这排除共同存在的IOMMU W FIFO、元数据、响应字段和新增控制，未综合，不是总片上存储或面积/LUT减少证明。

|同A8/64次4KiB条件|热P|热PN|冷P|冷PN|活跃DMA payload|
|---|---:|---:|---:|---:|---:|
|原大私有缓冲，本轮校准|历史91874*|93487|未重跑|未重跑|8192B|
|Mode4完成阶段一槽|119646|123804|120146|124304|2096B|
|Mode5普通两槽|96772|102746|97212|103366|4144B|
|Mode6紧邻下一段＋一槽|98784|99432|99244|99892|2096B|
|Mode7普通准确写前缀，无池|92109|92374|92549|92814|48B|
|Mode7另加普通walk额度256|92109|92374|92549|92814|48B|

*旧热P91874、旧大缓冲冷walk P92334/PN91727均为历史记录，不算本轮新矩阵。上述cycles是V首发到V最后完成窗口，较低较好；所有新配置窗口内A也完成64次，因此同时保留两设备吞吐口径。P/PN是相同已编译模型的运行时模式。

## Codex独立规则与受限进度分析

元数据先入队，真实返回空间预约与翻译解耦；不伪造fullbuffer证书。`tests/nonblock_completion_banked_ctl.sv`是生产banked controller测试副本，只在明确宏/参数下关闭旧入口guard并在各bank完成AR前加寄存admit。AW/W/B/R不变，默认仍使用原生产模块；宏下Gate0校准精确93487。

当前读STREAM须源/目的相对job偏移、len/size匹配，以及两端W队头、物理AW接受和每DMAW序号匹配。下一读提前STREAM还须：两端**紧邻第二项**都是该DMA；下一AW已实际接受；恰有一个旧读；旧读已STREAM发射并保持同readID0顺序，或旧POOL响应已全部到达。两个8项写顺序账本在真实AW首次选择时push，在真实WLAST时pop，并逐拍核其队首与mux实际队首一致。不能以“后面还有同bank AW”替代立即后继。

在限定正常两burst夹具中，旧STREAM的RLAST同ID先于新R；或旧POOL全部驻留，可以独立回放。旧W所需字节不再依赖新R，公平独立W服务下先完成旧段，紧邻下一段即可排空新R。future标签允许写头从target−1推进到target，不能要求一直停在target−1。新R等待旧POOL回放时保持背压，私有端只回放旧包；VALID不因临时证书变化撤回，AR armed保持到握手，响应id/len/last、终端守恒与fault均检查。

这是受限设计推演及场景验证，**不是一般AXI无死锁证明**。两端写顺序若不一致、同bank多个DMA/不同ID交织、任意背压、错误/取消、RAWCoupling或SpillW变化均未覆盖。新模块限制对齐正常4KiB、每DMA串行job、readID0，生产TC/iDMA仍实际参与；CPU为合成供给，DDR为行为模型，不是MIG/实板结论。

## 新发现的测试侧标签问题及修正

旧`nonblock_read_admission.sv`每bank只有`fast_pending_q`一位，任意后续翻译响应会覆盖它。完成阶段准入使翻译早于真实AR较多，旧标签不再逐请求匹配。这是Codex新原型与旧测试代理的接口不兼容，**不是生产IOMMU新bug**。原13条buffer模式的完整字节结果、TL FAST和cycles仍有效，但其FAST读31/32计数不可用于分类；“TL支持读写”不能推出本次FAST一半读、一半写。

修正为每bank8项成功READ翻译标签FIFO，按READ响应push、对应完成AR捕获pop，跳过写响应/翻译错误，逐拍检查溢出/空读、终端检查排空。只重编并重跑受影响的Mode7 buffer/walk八项。修正后热/冷FAST读62/64，TL FAST62/64，tags fault0/queued0，buffer cycles精确保持。walk仍相同，但fast_gated_offers、budget_stall均0：该夹具没有触发walk限制，不能称额度产生收益。

最终矩阵21条＝原13条＋修tag八条。原Mode7八条数据校验也通过，但分类/额度解释已撤销，原日志与`runs-pre-tagfix.json`保留；29次实际执行不能包装为29个独立有效配置。六个SV模型构建成功、一次lint；终端记录核对另见实际生成的`acceptance.json`。不重复构建或运行无受影响的0/4/5/6模型。

## 具体轨迹与因果限度

CSV只记录测量窗口，cycle已是V首发相对周期。首V源DDR第一AR/第二AR/第一物理RLAST：原16/26/334，Mode4为17/1223/330，Mode5为17/1210/330，Mode6为17/932/330，Mode7为640/647/945。Mode6旧POOL物理RLAST早于私有回放消费，不能混用。Mode7恢复源两段并行在途，但推迟首V数据直到准确W通路准备。

Mode6热PN页表DS最大读延迟290、behind_comp6789、A PTW占用22297；Mode7为47、0、14357。支持当前准入减少不合适提前数据与页表请求的竞争；CPU请求量也随窗口变化，不能把全部差值归因单一R包或内存带宽。

## 创新判断与下一主线

**停止推荐共享池作为当前主贡献。** 普通准确写前缀对照更小、更快；共享/接收预约本身不能包装创新。保留Mode7为强工程基线和研究入口，不称其已达到论文创新。

[iDMA原论文 §II-C](https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf)已有小FIFO流式数据通路及按系统替换为完整缓冲；[Arm DMA350 §4.3.4](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)使用完整响应空间的读准入；[CTC §3–4](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)已研究接收容量与消息依赖。它们支持相关原语有前史，不证明当前双写队列规则已被精确覆盖或新颖。

下一步先定向判断**翻译前与翻译后写顺序不一致是否在本平台合法可达**，以及普通写前缀准入在多DMA/异步翻译下的适用界限。若不可达或普通机制同资源可等价，按工程优化处理；若可达，先给出具体等待环及最小安全回退/顺序约束，再比较已有DMA耦合/credit方法能否同样处理。没有该差异证据前，不进入生产/BIT或扩大无目的矩阵。

## 分工与证据

Codex独立设计完成阶段准入、紧邻写段规则和无池强对照，亲自核关键源码、修正标签接口并执行构建/仿真。DeepSeek给C++元数据验收草案、设计独立质疑、日志核对与说明草稿；其错误建议（物理AR应等于虚拟AR、禁止W头推进target、FAST应固定读写成对）未采纳。完整任务/原响应均留在本目录。没有调用Claude或新Codex子agent。

`runs.json`/各日志是最终矩阵，`runs-all-executions.json`保留全部29次；`first-window-events.json`、构建command/result/log、`test-source.patch`与`before/after/`供复查。76个编译生产SV字节是否不变以实际`acceptance.json`为准。本轮未改生产RTL/Linux或加载BIT、写SD、操作板卡。
