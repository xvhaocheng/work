# IOFENCE三道门：前作门未通过，停止机制

术语：drain＝使已处理的设备访存到达全局排序点；CQ＝命令队列；processed＝本模型选定的已处理请求前沿。

**结论：按用户“任何一项不通过就停止”的要求，停止新增按域／按地址drain机制。** Intel VT-d已经明确规定这些语义，第三道门先不通过；计划中的26组流量实验在构建前停止，**本轮没有新仿真数字**。误上传的原文及附属六文件已删除，原始附件与`d177c8a`实验记录保留。

| 问题 | 本次结果 | 依据／边界 |
| --- | --- | --- |
| B流量增大会不会放大A的PR/PW等待？ | **未测，不能下性能结论** | 第三门失败后未构建、未运行；仅保留[未验证提案与状态](analysis/status.json)，无scoped机制 |
| 按设备／按域缩小是否安全？ | 设备范围无条件不安全；域范围需要条件 | 同域另一设备可持有同一已解除映射的旧PA请求；域范围须覆盖全部相关设备、正确的请求前沿、上下文代际、在途翻译／ATS及共享帧生命周期，见[安全分析](safety/README.md) |
| 前人是否已做过？ | **不通过：已有域／页范围payload drain** | Intel VT-d Rev5.0 §6.5.4，印刷6-45—6-46；descriptor见§6.5.2.3，完成点见§6.5.2.9；[原文核查与Arm／RISC-V对照](prior/README.md) |

Intel原文不是只谈TLB缓存：它把已处理但尚未提交内存的设备请求定义为non-committed requests。域选择IOTLB失效配合DR/DW，仅保证目标域这些读写被排空；页选择还限定重叠的untranslated地址范围。队列路径须在下一`inv_wait_dsc`完成之前满足drain。其“only guarantees”是最低完成保证，**不能据此声称所有实现都不等待其他域，或已有隔离性能数字**。root已直接阅读并视觉核对[官方PDF第137页／印刷6-46](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf#page=137)。

**旧数据的归因保持更正。** ＋37,854拍来自旧PR=PW=0、两fence路径，主要增加在CQ命令取数（＋38,503拍），不是已测得的主线PR/PW drain。单跑差0、共跑差19,512拍的“100%”仅为该mode×共跑差分交互项。见[原始诊断](../dead-translation-20261017/diagnosis/results.md)。

**模型还存在一个必须披露的覆盖边界。** 旧PR/PW测试只快照comp AR/AW已接受请求；TC已收到翻译结果、却尚未经过COMPLETE／demux背压的请求可能未被纳入。源码允许这一状态，但没有证明旧28组实际触发，因而不回写或否定旧原始测量；也不能将其称为完整规范PR/PW实现。成功TL→TC响应握手是更早、可审查的候选前沿，本轮未接入、未验证。原版PR/PW仍为TODO，IOTINVAL与late-fill还须分别处理；未自动修生产RTL、提交上游PR或启动FPGA。详见[安全分析](safety/README.md)。

论文agent已收到已核数字、短于FQ超时的限制、错误归因更正、模型覆盖边界与第三门停止结论；它仅负责`paper/carrv/`。本任务仅写本目录，未修改生产代码、版本锁、旧实验或论文。本轮由Codex进行原文核读和模型审查，DeepSeek／Claude未调用；[未验证runner](runner/README.md)与来源哈希均保留，不能当已运行成果。
