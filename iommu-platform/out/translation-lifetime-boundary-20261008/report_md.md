# §40：翻译寿命边界与创新门槛正式报告（起草提案）

日期：2026-10-08。状态：起草提案，未执行 shell，未改文件，未运行新的 DUT/build/BIT/board。范围：按根 Codex `codex-platform-gate` 审定、第一席 `innovation_rules_40` 推演、DeepSeek 证据整理，汇总 §40 的正式结论、证据边界和下一最小判别规格；不作新机制设计，不作论文等级或分区预期判断。

## 结论

1. 因为当前平台没有接入“普通译址结果必须等 DMA 数据完成才释放，并以该释放反馈阻塞 IOFENCE”的路径，源码可达性门槛否决当前 LTI-style 寿命反馈；不能用该依赖建立新准入机制。Arm LTI 的完成寿命规则不能直接当作本 RISC-V IOMMU 已实现的约束。
2. 这不表示命令队列永不等待：CQ 的命令取回仍通过 DS／内存通路，可受互连竞争影响；也不表示当前实现完整符合所有 IOFENCE 语义。TL ERROR 等 fault_ready 是故障记录接收，之后以 error 响应返回；本次没有实现“等待软件修页后重试”的通用协议。若未来研究 live remap/revocation，必须先定义实际支持的完成确认、页表更新次序、旧映射何时允许回收及哪些请求仍获准使用旧权限。
3. PR/PW 的硬件 TODO 是工程边界，不是本轮可宣称的研究创新或已证实漏洞。PR、PW 是专用字段而非 reserved 位；当前合法命令检查本身不会因 PR/PW=1 拒绝命令，源码也没有实现它们的排空等待。Linux 默认 IOFENCE 只设置 opcode/func，dword1=0，普通同步调用 PR/PW=0；真实 FPGA 配置 InclATS=0，无 ATS 分支会把 invalidation_inflight 等清零；CQH 在命令最后一拍取回时推进，随后才 DECODE，CQH 早推进不等于 generic completion；§39 静态 6 PASS 不验证 live invalidation。本轮不改生产，不把补齐规范功能包装成独立论文机制。
4. 两套具体微架构规则——A 细粒度预算关闭只准入排空闭包，B 提交前使多个 W 序列一致——因为相同关键决定已被内部/普通方法覆盖，不能称独立创新。A 的排空债务、已承诺写读豁免、成对 slot 预约和 byte 前缀在 `composition-history-isolation-20261007.md` 已有邻近；B 可由普通每 bank 原序 FIFO＋prepared 位／序号复现。
5. 两个独立研究席已按最新要求真实创建：第一席 `innovation_rules_40` 的 artifact 为 `../out/translation-lifetime-boundary-20261008/innovation-rules-agent.md`，已完成；第二席 map agent 仍在收敛。其 ShareMMU 反例按根审定只写为适用性问题，artifact 链接可用但最终审定再合并，当前不当作已完成证据。根将在最终审定修正状态。
6. 保留一个窄问题：错位 R/W burst 的尾 buffer／最小分片界；另保留 ShareMMU §V-C7 的 stalled miss 假设适用性问题。下一最小判别规格是提案，没有 run；先限定在 out 抽象模型/规格，不重跑 §39、不磨既有小差距。只有证明强普通方案在相同信息、总资源和合法完成语义下不能获得同样保证，并核清已有工作没有同式结论，才能讨论独立贡献；当前门槛直接否决的是假定存在的寿命反馈。

## 核心表

| 方向 | 根审定门槛/证据 | 普通强对照 | 当前处置 |
|---|---|---|---|
| 两侧 W 次序一致 | `innovation-rules-agent.md` §3：ready-first 弱策略反例不能否定普通每 bank 原序 FIFO＋prepared 位／序号；同信息、同 header、同 seq 存储、同输出即可复现 | 普通 prepared FIFO；order-preserving queue；跨 bank 仍并行 | 不能称独立创新；如完全等普通 prepared FIFO，只确认工程基线 |
| 预算关闭后保留排空债务 | `innovation-rules-agent.md` §2.1：规则主体已被旧债务保留、已承诺写读豁免、成对 slot 预约、byte 前缀/尾部保留覆盖；`dma_research_policy` 已有反馈和计费 | 普通成对预约；普通 budget/debt/refund；CONF0 共同分段；完整 R 尾部预约 | 不能仅凭新计费位置立独立机制；不升级为生产方案 |
| 错位 R/W 分段排空范围 | `innovation-rules-agent.md` §2.2–2.3：受限模型可让关闭后排空随 job 长度增长；但 §39 普通物理源 child 已切 1KiB，恰好排除该前提；需核真实停车位置、共享 R route/W owner、payload B 边界 | 静态普通 splitter 按相同 q 输出；descriptor-derived 边界切分；每子片独立译址＋ready 队列；额外 tail storage；共同分段 | 保留为窄分析/最优化问题；若普通同信息达到同保证同成本，停止新机制推荐 |
| ShareMMU §V-C7 无死锁假设 | 根审定：该论证假设 stalled miss 不占用 accelerator 执行资源、已命中有限时间排空；小缓冲 copy 中已译址源读可能依赖尚未译址目的写，是适用性问题；本平台未实现同式 live revocation | 普通配对预译址；先停整 job/quiesce；sequence fence；Arm LTI §6.2.1/§6.2.2 强对照 | 只写适用性问题，不称已发现生产死锁；第二席仍在收敛，最终审定合并 |

## 证据与边界

- 根审定与完整证据链接：根审定 [../out/translation-lifetime-boundary-20261008/codex-platform-gate.md](../out/translation-lifetime-boundary-20261008/codex-platform-gate.md)；第一席 [innovation-rules-agent.md](../out/translation-lifetime-boundary-20261008/innovation-rules-agent.md)；源码证据 JSON [source-evidence.json](../out/translation-lifetime-boundary-20261008/source-evidence.json)；DeepSeek 证据整理 [deepseek-source-evidence.md](../out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md)。没有把文件哈希当成硬件正确性测试。
- 根直接核的关键源码：普通 TL 的 COMPLETE 在 `slow_resp_ready` 后回 IDLE（`cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:1561–1565`）；TC AWAIT_RESP 接收完整结果，COMPLETE 在物理 AR/AW 握手后回 IDLE（`.../transaction_controller/rv_iommu_trans_ctl_wrap.sv:693–725`）；CQ handler 端口无 TC/R/W 完成输入（`.../software_interface/rv_iommu_cq_handler.sv:36–102`）；IOFENCE PR/PW 是显式 TODO（`:348–395`，尤其 `:362`）；PR/PW 是专用字段（`packages/rv_iommu_pkg.sv:415–426`）；CQH 在取回末拍推进（`:276–299`）。
- 配置与软件：真实 FPGA 配置来自 `ariane_soc::RVIOMMUCfg`，nonblock_cfg 只改 MSITrans（`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1394–1406`）；实际 InclATS=0，gen 无 ATS 分支清零 invalidation_inflight 等（`cva6/corev_apu/tb/ariane_soc_pkg.sv:110–137`，尤其 `:122`；`rv_iommu_top.sv:788–803`）；Linux 默认 IOFENCE 只设置 opcode/func、dword1=0（`linux/drivers/iommu/riscv/iommu.c:359–364`；PR/PW 位见 `iommu-bits.h:448–449`）；`post_sync` 发布 CQT 后轮询 CQH（`iommu.c:480–534`）；研究 flush helper 要求 DMA 已 quiesced，并明确不是 ATS/PR/PW/general barrier，注释解释 CQH 早推进（`iommu.c:2148–2162`）。
- §39 静态边界：DUT 把 CQ 请求、IOTINVAL、IODIRINVAL 固定为零（`../out/prepared-source-bypass-20261008/multi_system_top.sv:454`；`multi_banked_top.sv:603–608`），因此其 6 项 PASS 不检验 live invalidation，也不检验上述寿命协议。
- 其他接口证据：`rv_iommu_banked_ctl.sv:246–277` 的 W owner/AW 计数限制 source AR 选择，存在真实 R→W→共享返回依赖；`rv_iommu_ds_if.sv:77–108` 的七路 AR 经仲裁和 spill 发往 DS，页表读取与命令读取有共同 DS 入口，但数据 comp 是另一发起端口；`dma_research_policy.sv:284–345` 已有译址次数、miss、PTW beat、TC 周期预算和 cycle 末无条件 refill；`idma_legalizer.sv:137–221,450–461` 的 src/dst 边界可独立或共同推进；`multi_actual_dma_top.sv:549–575,867–943` 显示普通可缓存准备结果、物理源 AR 真绕 TC、单全局 child route 固定至 RLAST。
- 官方规范：RISC-V IOMMU 规范 [In-memory queues / IOFENCE.C](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html) 明确区分命令取回与执行；PR/PW 约束已处理的读／写到全局排序点，未处理请求仍需互连特定 fence，不是“整个 DMA job 已完成”的同义词；AV 完成写回可提供另一种命令完成观察手段。Arm LTI §6.2.1 对不得依赖软件动作或后续 Stall 译址的完成义务设约束；§6.2.2 另规定共享译址结果的完成寿命。
- 前史强对照：AXI-REALM 的 period budget／逐 beat 扣费／budget 耗尽后 reduce outstanding 和隔离；Arm CCI-500 TRM 的读写合并 OT 限额保留防死锁 tracker；TLB Prefetching＋MMU-Aware DMA 的逐 burst metadata 退休/重试和同页 walk 合并；GPU page walk scheduler 的按整体翻译工作量调度；MASK 的 TLB fill token、data quota 和 stage-budget；AXI NoC 的 endpoint R/W 依赖环与轻量 VC。上述不能把“发现 R/W 环或加独立 VC”本身称创新。
- 第二席 map agent 仍在收敛：其 ShareMMU artifact 链接见 `../out/shared-translation-literature-20261006/papers/sharemmu2026.txt`；本报告只按根审定记录 §V-C7 的 stalled miss 适用性问题，不声称该席已完成，不声称已发现生产死锁，最终审定再合并其收敛结果。

## 下一最小判别规格（提案，未 run）

- 受限模型：线性 byte-preserving copy；source 与 destination 按各自 2KiB 边界分段；无错误/失效；目的流保持每 DMA 顺序；预算关闭后要求已不可撤销的物理 R/W 最终完成；每 DMA 可停放 payload B 小于 1KiB；不允许修改已 offer 的 ARLEN/AWLEN；不假定 MIG 按 AR 次序返回，不引用 LTI。
- 保留命题：令 q 为目的完整 burst 的候选停止点，`r(q)` 是覆盖 q 的最后一个已完整取得的物理源 burst 末端；在只有单 DMA、只在目的 burst 结束停车、没有额外 outstanding 越过停止点的受限实现中，`safe_stop(q) => r(q) - q <= B`。若 B=0，停止点必须同时是 source 和 destination 物理 burst 的结束点。升级为充分条件仍须分别证明协议 route、upstream logical RLAST、错误和各共享返回通路的停车可行性；仅 byte 算式不是通用进度证明。
- 可检查的受限最优化：引入最大隔离响应量 H，把目的停止点作为节点；相邻停止点差≤H 则有边；现有 source 分段尾部≤B 成本 0，否则在该目的点新增 source physical cut 成本 1；求最小成本到 job 末端，得到“为满足 H 至少需要多少额外源 fragment”。全部输入可由 descriptor 和普通 legalizer 提前得到；如果静态普通 splitter 按相同 q 输出、用相同 parent/RLAST 恢复 metadata，就可以逐事件复现同一保证与请求数，因此不能把最优 cuts 放在后译址位置后再称独有机制。
- 建议检查：扫作业长度、src/dst offset、读写 legalizer decouple、物理 child 最大长度、FIFO B 和预算关闭时刻，同时保留正常 physical bypass；检查关闭后的实际 source/dest 字节与译址服务数、源/目的/预算守恒、真实 quiet、错误处理、所有 held VALID 稳定。当前未 run。
- 停止条件：若未分片且 B<1KiB 时，在上述错位几何里关闭后的排空字节保持常数而不随 job 长度增长，应寻找哪个真实停车通路打断了模型，命题必须收窄或淘汰；若普通共同分段/边界切分达到同样保证和 metadata 成本，停止新机制推荐。
- 实现边界：这是跨模块时序与进度问题，至少涉及 frontend descriptor 关联、legalizer/source 父恢复、TC 准入、两侧 W 序、budget observer；不能让普通实现 worker 自主改所有模块后再验收；不能用请求量界声称硬时延界，还须存在 DDR/互连/PTW 有限服务界及饥饿防止假设。

## 归属

DeepSeek evidence/doc drafting, Codex source/innovation design/agent/acceptance.
