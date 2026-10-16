# §40：真实结果释放／失效等待边界审定

日期：2026-10-08。Codex 根 agent 直接核关键源码并作判断；DeepSeek 整理 CQ/Linux 证据。此项是源码可达性门槛，不是新 DUT、板测或通用正确性证明。

## 判断

因为当前平台没有接入“正常译址结果必须等 DMA 数据完成才能释放，并以该释放反馈阻塞 IOFENCE”的路径，不能用该依赖来建立新准入机制。Arm LTI 的完成寿命规则不能直接当成当前 RISC-V IOMMU 已实现的约束。

这不表示命令队列永远不会等待：CQ 的命令读取仍通过 DS／内存通路，可受互连竞争影响。也不表示当前实现完整符合所有 IOFENCE 语义。PR/PW 的硬件 TODO 和 CQH 提前推进是工程边界；不是本轮可宣称的研究创新或已证实的安全漏洞。

## 根直接核的关键事实

| 事实 | 原始位置 | 判别含义 |
|---|---|---|
| 普通 TL 的 COMPLETE 在 slow_resp_ready 后回 IDLE | `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:1561–1565` | 不等源 RLAST／目的 W 完成。 |
| TC AWAIT_RESP 接收完整结果，COMPLETE 在物理 AR/AW 握手后回 IDLE | `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:693–725` | 后续 AXI 事务仍可在途，但没有把其完成反馈给 TL 或 CQ 的该种寿命锁。 |
| CQ handler 没有 TC/R/W 完成输入；IOFENCE PR/PW 是显式 TODO | `cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:36–102,348–395`，尤其362 | 不能把软件写 PR/PW 说成已经获得硬件排空能力。 |
| PR、PW 是专用字段，并非 reserved 位 | `cva6/corev_apu/riscv-iommu/packages/rv_iommu_pkg.sv:415–426` | 当前合法命令检查本身不会因 PR/PW=1 拒绝命令；源码也没有实现它们的排空等待。 |
| CQH 在命令最后一拍取回时推进，随后才 DECODE | CQ handler `:276–299` | CQH 不可笼统当执行／数据完成证明。 |
| 真实 FPGA 配置来自 ariane_soc::RVIOMMUCfg，nonblock_cfg 仅改 MSITrans | `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1394–1406` | 不能用 NullCfg 代替板端真实配置。 |
| 实际 InclATS=0；gen 无 ATS 分支将 invalidation_inflight 等信号清零 | `cva6/corev_apu/tb/ariane_soc_pkg.sv:110–137`，尤其122；`rv_iommu_top.sv:788–803` | CQ 的 ATS 等待分支不能解释当前配置的等待。 |
| Linux 默认 IOFENCE 只设置 opcode/func，dword1=0 | `linux/drivers/iommu/riscv/iommu.c:359–364`；PR/PW 位在 `iommu-bits.h:448–449` | 当前普通同步调用 PR/PW=0；AV helper 也不设置 PR/PW。 |
| Linux post_sync 发布 CQT 后轮询 CQH | `linux/drivers/iommu/riscv/iommu.c:480–534` | 软件观察的是命令队列位置，不是独立的 DMA R/W 完成通知。 |
| 研究 flush helper 要求 DMA 已 quiesced，并明确不是 ATS/PR/PW/general barrier；注释解释 CQH 早推进 | `linux/drivers/iommu/riscv/iommu.c:2148–2162` | 既有受限实验保证不等于支持活跃 copy 的失效与内存回收。 |
| §39 DUT 把 CQ 请求、IOTINVAL、IODIRINVAL 固定为零 | `out/prepared-source-bypass-20261008/multi_system_top.sv:454`；`multi_banked_top.sv:603–608` | §39 的6项 PASS 不检验 live invalidation／上述寿命协议。 |

故障路径也不等价于 LTI Stall：TL ERROR 等 fault_ready 是故障记录接收，之后以 error 响应返回；本次没有实现“等待软件修页后重试”的通用协议。

## 官方规范与工程限制

[RISC-V IOMMU 规范：内存队列／IOFENCE.C](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html) §IOFENCE.C 明确区分命令取回与执行；PR/PW 约束已经处理的读／写到全局排序点；尚未被 IOMMU 处理的请求还需要互连特定的 fence。它们也不是“整个 DMA job 已完成”的同义词。AV 完成写回可提供另一种命令完成观察手段。

当前硬件没有完整实现上述数据排空部分，CQH 的早推进也已有 Linux 本地注释记录。本轮不改生产、不把补齐规范功能包装成独立论文机制。若未来研究 live remap/revocation，必须先定义实际支持的完成确认、页表更新次序、旧映射何时允许回收及哪些请求仍获准使用旧权限。

## Codex 对新增研究切口的判断

1. 两侧 W 次序一致：普通每 bank 的原顺序 FIFO＋prepared 位／序号即可同信息同状态复现，不能作为新的调度机制。
2. 预算关闭后保留排空债务：与既有 PTE 入口确认＋旧债务保留＋已承诺写的读豁免重合。给排空义务新名称不能建立独立性。
3. 错位 R/W 分段的排空范围：若要求已发 R 和 W 都结束后释放共享通路，小尾部缓冲可能让有限 burst 导出全 job 排空。需要明确停车位置、共享 R route／W owner、payload B 的真实边界；原型的全局 source-child route 到 RLAST 是可检查的约束。§39 的普通1KiB child 已避开代理例，普通共同分段也可同效。可继续查成本／响应量下界，不以该反例救回位置创新。
4. ShareMMU §V-C7 的无死锁论证假设 stalled miss 不占用 accelerator 执行资源，已有 hit 在有限时间内排空。小缓冲 copy 中已译址源读可能依赖尚未译址目的写，是该假设的适用性问题。当前平台未实现同式 live revocation，不能声称已发现生产死锁。Arm LTI §6.2.1 已对不得依赖软件动作或后续 Stall 译址的完成义务设约束；§6.2.2 另规定共享译址结果的完成寿命。普通配对预译址／先停整 job／sequence fence 都必须进入强对照。

若要继续第四项，Codex 优先研究“最小需要一起准备／排空的 byte 前沿”，而不是扩大旧 epoch 的权限豁免。失效后的新写请求没有自动继承旧源读权限；只有在协议明确定义并且完成确认之前维持旧映射时才可讨论旧工作。否则方案可能只是在推迟失效完成，或不合法地访问已撤销映射。

只有证明强普通方案在相同信息、总资源和合法完成语义下不能获得同样保证，并核清已有工作没有同式结论，才能建立独立贡献。当前本门槛直接否决的是假定存在的寿命反馈；没有作论文等级或录用判断。

## 验证边界

本项新增 DUT build=0、run=0、BIT build=0、board run=0。未改生产 RTL、Linux 或原测试，未写 SD／加载 FPGA；前阶段数值只引用。关键源码原文及哈希见 `source-evidence.json`；两席独立研究详见对应 agent 文件。没有把文件哈希当成硬件正确性测试。
