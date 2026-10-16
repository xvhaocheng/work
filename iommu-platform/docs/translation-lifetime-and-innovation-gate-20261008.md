# §40：翻译寿命边界与创新门槛正式报告

日期：2026-10-08。状态：正式报告，非草案。本阶段 Codex 已执行只读源码命令并写入证据/草案文件；没有 DUT build/run/BIT/board，没有改 production RTL/Linux/test，没有写 SD。范围：汇总 §40 结论、证据边界与下一最小判别规格；本轮未确立独立新机制，不作论文分区判断。

## 结论

1. 因为当前平台没有接入“普通译址结果必须等 DMA 数据完成才释放，并以该释放反馈阻塞 IOFENCE”的可达路径，源码可达性门槛否决当前 LTI-style 寿命反馈；Arm LTI 完成寿命规则不能直接当作本 RISC-V IOMMU 已实现约束。
2. 这不表示命令队列永不等待：CQ 命令取回仍通过 DS/内存通路，可受互连竞争影响；也不表示当前实现完整符合所有 IOFENCE 语义。TL ERROR 等 fault_ready 是故障记录接收，之后以 error 响应返回；未实现“等待软件修页后重试”的通用协议。当前未验 fault 排空。
3. 普通 fault/error+合法 discard 必须作为进度强对照：是否能返回错误、准确报告 copy 失败并安全排空，仍需验证。保持整个 copy 成功是性能额外合同，不是标准失效安全的必要条件；不得混入安全论证或成功工作量。
4. PR/PW 硬件 TODO 是工程边界，不是本轮创新或已证实漏洞。PR、PW 是专用字段而非 reserved 位；合法命令检查不会因 PR/PW=1 拒绝；源码也没有实现排空等待。真实 FPGA InclATS=0；无 ATS 分支清零 invalidation_inflight 等；§39 静态 6 PASS 不验证 live invalidation。
5. 两套微架构规则——A 细粒度预算关闭只准入排空闭包，B 提交前使多个 W 序列一致——因相同关键决定已被内部/普通方法覆盖，不能称独立创新。A 的排空债务、已承诺写读豁免、成对 slot 预约和 byte 前缀已有邻近；B 可由普通每 bank 原序 FIFO＋prepared 位/序号复现。
6. 两个 native Codex 研究席均已完成：第一席 `innovation_rules_40` artifact 为 `innovation-rules-agent.md`；第二席 map artifact 为 `innovation-map-agent.md`。ShareMMU §V-C7 只按根审定写为适用性问题，不称已发现生产死锁。
7. 下一单一判别规格以 `codex-next-decision.md` 为准：固定映射，long copy 内可控让路，小缓冲下最小合法前沿/分片成本界。temporary translation lock 只属 ShareMMU 适用性反例，不是优先给生产新加锁，也不是当前 IOTINVAL 语义。
8. 主报告没有排除强普通的 new rule，因此不能认定独立机制。可保留待证明的锐界作为分析候选。因为强普通对照尚未被排除，所以当前只写适用性边界与成本界候选；如果普通方案在同信息、同资源、同完成语义下达到同保证同成本，就停止新机制推荐。

## 核心表

| 方向 | 强普通前作/对照 | 当前处置 |
|---|---|---|
| LTI-style 寿命反馈；ShareMMU §V-C7 stalled miss | Arm LTI §6.2.1 独立完成义务；§6.2.2 多事务寿命；普通成对预译址、先停整 job/quiesce、sequence fence | LTI 源码可达性门槛否决；ShareMMU 只写适用性问题，不称生产死锁 |
| PR/PW TODO 与软件默认 | PR/PW 专用字段；合法检查不拒；Linux 默认 IOFENCE 设 opcode/func、dword1=0、PR/PW=0 | 工程边界，非独立创新/漏洞 |
| W 次序一致、预算关闭债务 | 普通每 bank 原序 FIFO＋prepared 位/序号；旧债务保留、已承诺写读豁免、成对 slot、byte 前缀 | 不能称独立创新；不升级生产方案 |
| 错位 R/W 分段排空范围 | 普通共同分段、descriptor-derived 边界、完整 R 尾部预约、每子片独立译址/ready 队列 | 保留窄分析/成本界；普通同保证同成本则停止 |

## 证据与边界

- 根审定：[codex-platform-gate.md](../out/translation-lifetime-boundary-20261008/codex-platform-gate.md)；第一席：[innovation-rules-agent.md](../out/translation-lifetime-boundary-20261008/innovation-rules-agent.md)；第二席：[innovation-map-agent.md](../out/translation-lifetime-boundary-20261008/innovation-map-agent.md)；下一判别：[codex-next-decision.md](../out/translation-lifetime-boundary-20261008/codex-next-decision.md)；源码证据：[source-evidence.json](../out/translation-lifetime-boundary-20261008/source-evidence.json)；DeepSeek 整理：[deepseek-source-evidence.md](../out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md)。DeepSeek完整调用结果分别保存在 `deepseek-source-evidence-response.txt`、`deepseek-doc-draft-response.txt`、`deepseek-doc-revision-response.txt`；最终审定稿及调用用量另见本阶段账本。
- 根直接核的关键源码：普通 TL COMPLETE 在 `slow_resp_ready` 后回 IDLE（`rv_iommu_tl_wrap.sv:1561–1565`）；TC AWAIT_RESP 接收译址结果，COMPLETE 在物理 AR/AW 握手后回 IDLE（`rv_iommu_trans_ctl_wrap.sv:693–725`）；CQ handler 端口无 TC/R/W 完成输入，IOFENCE PR/PW 是显式 TODO（`rv_iommu_cq_handler.sv:36–102,348–395`）；CQH 在取回末拍推进（`:276–299`）；PR/PW 是专用字段（`rv_iommu_pkg.sv:415–426`）。
- 配置与软件：真实 FPGA 配置来自 `ariane_soc::RVIOMMUCfg`，nonblock_cfg 仅改 MSITrans（`ariane_peripherals_xilinx.sv:1394–1406`）；InclATS=0（`ariane_soc_pkg.sv:110–137`，尤其 `:122`；`rv_iommu_top.sv:788–803`）；Linux 默认 IOFENCE 设置 opcode/func 与 dword1=0，普通同步调用 PR/PW=0（`iommu.c:359–364`；`iommu-bits.h:448–449`）；`post_sync` 发布 CQT 后轮询 CQH（`iommu.c:480–534`）；研究 flush helper 要求 DMA 已 quiesced，并明确不是 ATS/PR/PW/general barrier（`iommu.c:2148–2162`）。
- §39 静态边界：DUT 把 CQ 请求、IOTINVAL、IODIRINVAL 固定为零（`../out/prepared-source-bypass-20261008/multi_system_top.sv:454`；`multi_banked_top.sv:603–608`），其 6 项 PASS 不检验 live invalidation。
- 官方规范与 Arm LTI：[RISC-V IOMMU 官方规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)区分命令取回与执行；PR/PW 约束已处理读写到全局排序点，未处理请求仍需互连特定 fence，不等同整个 DMA job 完成。Arm LTI §6.2.1 约束独立完成义务，§6.2.2 另规定共享译址结果的多事务寿命，二者必须分清。
- 前史强对照：常规规则、PT 着色、共享计账、nonblocking walker 已有强前作覆盖，详见第二席。NB-Walker 新作者 artifact 已取得代码并只读保存，但论文全文仍未得、未复现实验；不从代码反推全部论文结论。没有把文件哈希当硬件正确性测试。

## 下一最小判别规格（提案，未 run）

- 受限模型：线性 byte-preserving copy；source/destination 按各自边界分段；无错误/失效；目的流保持每 DMA 顺序；预算关闭后要求已不可撤销的物理 R/W 最终完成；每 DMA 可停放 payload B 小于 1KiB；不允许修改已 offer 的 ARLEN/AWLEN；不假定 MIG 按 AR 次序返回；不引用 LTI。
- 保留命题：令 q 为目的完整 burst 的候选停止点，`r(q)` 是覆盖 q 的最后一个已完整取得物理源 burst 末端；在单 DMA、只在目的 burst 结束停车、无额外 outstanding 越过停止点的受限实现中，`safe_stop(q) => r(q)-q <= B`。若 B=0，停止点必须同时是 source 和 destination 物理 burst 的结束点。升级为充分条件仍必须分别证明协议 route、upstream logical RLAST、错误与共享返回通路停车可行性；仅 byte 算式不是通用进度证明。
- 成本界：令 H 为允许的最大两个可释放停止点之间 byte 距离，是工作量界，不是时钟时间界。目的停止点作节点；相邻停止点差≤H 则有边；现有 source 分段尾部≤B 成本 0，否则新增 source physical cut 成本 1；求最小成本到 job 末端，得到“为满足 H 至少需要多少额外源 fragment”。全部输入可由 descriptor 和普通 legalizer 提前得到。
- 普通强对照与停止条件：比较普通共同分段、固定 child、descriptor-derived 非均匀分段、成对准备＋完整 tail 预约、按片暂停、sequence fence、精确页/上下文失效＋正常 quiesce。若普通静态 splitter 按相同 q 输出、用相同 parent/RLAST 恢复 metadata，则可逐事件复现同一保证与请求数，必须停止新机制推荐。
- 下一检查严格按 Codex 规格：先清点真实边界和可独立停车存储，比较理想前沿与同信息普通按片暂停；只有成本界有明确差异才写一个 out 诊断副本，试一个错位输入和一个共同分段强控制。必要检查为字节／预算守恒、held VALID、R身份／RLAST、W归属及最终 quiet；当前均未运行新DUT，fault排空也未验。
- 退出条件：若未分片且 B<1KiB 时，在上述错位几何里关闭后排空字节保持常数而不随 job 长度增长，应寻找真实停车通路打断模型，命题必须收窄或淘汰；若普通共同分段/边界切分达到同样保证和 metadata 成本，停止新机制推荐；若只有人为 TLB 锁产生差异，不能立新机制。
- 实现边界：这是跨模块时序与进度问题，至少涉及 frontend descriptor 关联、legalizer/source 父恢复、TC 准入、两侧 W 序、budget observer；不能让普通实现 worker 自主改所有模块后再验收。不能用请求量界声称硬时延界；还须存在 DDR/互连/PTW 有限服务界及饥饿防止假设。当前未实现、未运行。

## 归属

归属：DeepSeek 整理证据和起草文档；Codex 根与 2 研究席作创新推演、关键源/原文核查、最终审定；未使用 Claude。
