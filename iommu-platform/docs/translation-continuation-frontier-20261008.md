# §44：三席创新边界与两阶段续走成本

日期：2026-10-08。范围：只读源码与 primary 文献核查、纸面推演。

## 一、现状与总体结论

本轮采用 root Codex＋两席 native，分别负责机制/平台、具体前作、强普通反例。root 直接核关键源码与原文后裁定：下面四项规则都有具体前作或同效普通构造，停止独立机制推荐，不启动新 DUT；项目主线仍是多 DMA 共享 IOMMU 地址翻译的干扰与性能。以下每条给出最强前作、真实场景、未解约束、同信息同资源普通对照与停止条件。

关键的接口更正：结构性的 ready 阻塞路径（TL 在 COMPLETE 等返回）在正常 TC 不会成立为可达瓶颈。root 已直接核 `rv_iommu_trans_ctl_wrap.sv:675–702`：请求被接受后 TC 进入 AWAIT_RESP，`trans_resp_ready_o` 恒为 1；结果写入 TC 保存状态后才进 COMPLETE 等物理 AR/AW。故不得表述为 worker 诊断出已发生瓶颈；AW/W 背压在此正常请求链中钉不住 TL 的共享结果。

## 二、条目

### 2.1 共享 walk 结果驻留 / 慢 subscriber

- **最强前作**：Cong 等, HPCA 2017, DOI 10.1109/HPCA.2017.19（[作者 PDF](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf)）：不同步加速器的翻译复用、共享 TLB 过滤重复 miss、hit-under-miss、结果路由；官方 gem5 coalescer 固定 revision `392c1ced53827198652f5eda58e1874246b024f4`（committer 日期 2020-09-10），[tlb_coalescer.cc](https://gem5.googlesource.com/public/gem5/+/392c1ced53827198652f5eda58e1874246b024f4/src/gpu-compute/tlb_coalescer.cc) cc 95–124 / 140–211 / 211–222。
- **真实场景**：当前共享翻译的正常请求，结果先由 TC 接收保存。假设另一种接口中的慢 subscriber 拒收结果，不能作为本平台已存在的场景。
- **未解约束**：非接收端点下未交付结果的驻留合同与隔离上界；gem5 该路径未实现拒收结果的停放/重试合同（cc 214–222, 369–374）。当前 TC 是接受端点。
- **同信息同资源普通对照**：现有共享 IOTLB 的 deferred relookup（`rv_iommu_tl_wrap.sv:271–280,1280–1331`）；逐请求完成槽 / 共享结果 entry＋consumer bitmap；允许 replay 时也必须计入对照。
- **停止条件**：AW/W stall 无法产生一个已接受正常请求的 ready=0 → 停完成背压假设（当前已满足）；deferred relookup 已给同 walk 数 → 不立 walk-sharing 机制；普通槽/位图/replay 在同总预算下复现全部事件与成本 → 停候选，不改名。

### 2.2 无关失效命令不放大热译址等待

- **最强前作**：[US20220414016A1](https://patents.google.com/patent/US20220414016A1/en)（2022 公开）明确辨认匹配的在途 walk，并标记其结果不进入翻译缓存。
- **真实场景**：A 的主 walk 在 PT，V 为另一 bank 单阶段热请求、fast probe 处于 DC/LEAF，CQ 发仅针对其他地址空间/页的命令。`rv_iommu_tl_wrap.sv:320–321` 将任意 IOTINVAL/IODIR OR 成 fast invalidate；`rv_iommu_nonblock_hit.sv:149–172` 可使 probe 进 REJECT。CQ 命令输出端无普通 TL/PTW busy 输入（`rv_iommu_cq_handler.sv:32–100,300–362`），故不能假定命令必等 A 结束。
- **未解约束**：这是条件性源码事件序列，未执行动态命令实验；不能据此认定违规、已发生新 bug，或解释旧静态 PN 差距。
- **同信息同资源普通对照**：普通范围比较器＋同版本/结果槽可实现完全相同的取消/重试规则。
- **停止条件**：已有具体同效普通实现与范围匹配前作 → 停止独立机制推荐，归工程基线；不为它先做大矩阵或 BIT。

### 2.3 两阶段 walk 的 phase union 续走

- **最强前作**：[US8140781B2](https://patents.google.com/patent/US8140781B2/en)（2009 公开）Fig3–6、step305–345、claims 2–3：tracking queue、completion token、合成下一层 packet 并重新仲裁。root 另直接核官方 Rocket 2022 pinned 源码 commit `bf5742b9e353a80f501d43130218d3a444ec4b30`，[PTW.scala](https://github.com/chipsalliance/rocket-chip/blob/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala) :158–171（`aux_pte/r_pte/count` 等）、:196–214（由页号＋request 索引恢复 PTE 地址）、:524–565（父/leaf 复用与 G 返回恢复 S）。具体表示主体已被覆盖；这不是“笼统 ordinary 可抄任何算法”式的不新。
- **真实场景**：Sv39＋Sv39x4、两边三层、4 KiB leaf、无页表缓存的全冷路径最多 15 笔 PTE 读（`rv_iommu_ptw.sv:316–398,418–478`）。该路径未在 §43 运行中出现，也未由本席运行。
- **未解约束**：两席逐字段构造了 79/82bit 的普通续走帧（见下节），只是特定合同的逻辑字段计数，不是 RTL/面积验收。TL request/DC/PC/权限/context 仍须保持或保存，`update_o.vpn/pscid/gscid` 引用输入；已 offer 的 AR、已 accept 未返回的 R、最终结果/错误不可丢。
- **同信息同资源普通对照**：O1 原请求 token＋G/S target/最近返回 PTE＋等级/权限的 continuation tracking＋同样优先/RR；O2 安全边界 restart，包括仅重走当前 G 子 walk；O3 PWC/partial translation＋O1/O2。候选若仅做 offset 推导、phase union、删除不再使用字段，O1 必须允许这些同样具体优化；总存储需配平。
- **停止条件**：O1 同字段同保证追平 → 停止压缩机制推荐；O2＋同预算 PWC 追平工作成本 → 停止；纯 15 PTE 计数/字段删减不自动立新理论。

### 2.4 故障记录的短期容量背压

- **最强前作**：RISC-V IOMMU FQ 语义（[in-memory queues](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)）：满/内存错误置 OF/MF 后丢触发与后续记录，多个相同 fault 只报告 subset。
- **真实场景**：FQ 一条 record 需 AW→4W→B，WRITE 期间不接新 record（`rv_iommu_fq_handler.sv:183–279`）；`FaultFifoDepth=8`（`ariane_soc_pkg.sv:114`）；正常 DMA 故障全走单一 trans_fault port（`rv_iommu_sw_if_wrap.sv:245–302`）。TL ERROR 确会持续 fault_valid 等 fault_ready（`rv_iommu_tl_wrap.sv:1515–1540`）。
- **未解约束**：未执行自然可达实验；当前仅 §43 fault0 负载，不能宣称它是 §39 性能残差或 §43 让路时间的原因。
- **同信息同资源普通对照**：普通 worst-case credit 预约、per-DID VOQ、异步 record FIFO/reply 槽、精确 key 去重，均给相同合同；已有 FIFO 脱耦不能忽略。
- **停止条件**：ordinary 同容量同日志合同复现收益，或真实 iDMA 不能供出该 fault 序列 → 停止此枝。

### 2.5 方向性前作（用于边界，不替代上述构造）

HPCA21 DWS（[PDF](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)）p1/p4：owned walker 经 idle stealing 使 owner 新请求至多等一个 foreign walk，并未把整个 nested walk 等同一笔 PTE。ISCA18 Scheduling Page Table Walks（[PDF](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)）：按整体剩余 work 与同批调度。二者支持“续走/剩余工作调度已有前史”，不替代具体普通 checkpoint/restart/PWC 构造。

## 三、具体成本点与下一单一判别

root 独立提出 phase union 和 partial G restart，两席逐字段与 Rocket/CVA6/香山公开表示对照。完整交叉稿见[续走表示](../out/innovation-frontier-reset-20261008/continuation-prior-agent.md)、[root 设计与裁决](../out/innovation-frontier-reset-20261008/codex-critical-design.md)。

在 PAddr56、GPA41、正常静态 mapping、当前无 A/D 写、下一 PTE 尚未首次 offer 的合同下：

| 具体普通方案 | 暂停时保留的进度字段 | 代价与保证 |
|---|---|---|
| 逐 PTE 续走 | 父/叶 guest 页号、当前物理页表页号、权限、phase/level；普通特化最大82bit | 不重读已完成 PTE；当前已 offer 的 PTE 仍需退休。 |
| 仅重走当前 G 小段 | 父/叶 guest 页号与等级/属性；最大36bit | 一次暂停最多重做3次 G-PTE；已完成 S 前缀保留。 |
| 从原请求全部重走 | 保留原请求及其 context | 保存进度较少，但重复工作可能包含已做 S 前缀。 |

所有数字均为纸面构造。原请求/DC/PC/根/身份/权限 `C`、valid/owner/token、在途归属、active 状态及 fault/result 存储另计；不能宣称82→36就是总面积降幅，不能把大页逻辑省位当可并发使用的物理槽。

**新增判别：持续进度必须和让路一起比较。** A、V 若每读一个 G-root PTE 就暂停、丢 G 进度、交给另一方，双方虽公平获得内存服务，却可以一直重读 root，无法到达 G-leaf/S-PTE。甚至做完 G 后、读 S-PTE 前再丢映射也可能不推进。此反例属于提出的 partial-restart 策略，不是生产平台已发生卡死。

因此低状态普通方案需要完整 G＋S 的语义运行片段（本特定冷路径最多4笔PTE；final最多3笔），或明确的暂停次数/连续服务条件。一次快速交接不能自动证明不断交接时仍能完成。时间界还要依赖真实内存服务界，不能把15笔读直接换成固定周期。

下一只查一个分析问题：**固定请求到达与持续进度合同后，有限续走状态容量下，让路等待、重复 PTE 读取和状态存储的完整成本边界是什么？** 对照须同时包含逐 PTE continuation、whole walk、whole restart、partial G restart、同预算 PWC/partial translation。

如果能给出前作未建立的新锐界、或达到它且超出这些具体对照的规则，就可以形成独立分析/机制贡献；如果只是选择已有构造、删字段、观察15次读取或复述重启可能不前进，就不能称独立创新。**本轮没有完整界、没有新最优算法或新的性能收益。** 先过纸面可达性、强对照与必要性判别，才做最小模型；不先编译阈值扫描。

## 四、执行范围

本阶段仅只读源码/primary 核查与纸面推演。新 DUT build/run、生产 RTL/Linux 改动、板卡 BIT/JTAG/SD 动作均为 0；最新完整实测仍为 §43（静态 DEFAULT copy，TL66、miss34、PTW102、fault0）。源码见证不是 experiment。本报告不宣称新延迟/面积收益、已证明定理或论文分区。

[执行账本](../out/innovation-frontier-reset-20261008/stage-evidence.json)／[下一单一判别](../out/innovation-frontier-reset-20261008/codex-next-decision.md)。



本轮两个成功 DeepSeek 调用均为 deepseek-flash、thinking/high、默认最大预算、未截断：接口证据 86347、报告草案 31912，共118259 provider tokens。DeepSeek 整理接口证据与文档草稿；root Codex 和两席亲自设计/质疑、核关键源，root 执行资料保存并最终审定。未调用 Claude。

完整资料：[第一席](../out/innovation-frontier-reset-20261008/map-critical-agent.md) · [第二席](../out/innovation-frontier-reset-20261008/rules-critical-agent.md) · [逐字段续走核查](../out/innovation-frontier-reset-20261008/continuation-critical-agent.md) · [前作交叉审核](../out/innovation-frontier-reset-20261008/continuation-prior-agent.md) · [DeepSeek 原始证据](../out/innovation-frontier-reset-20261008/deepseek-interface-evidence-response.json) · [原始报告草案](../out/innovation-frontier-reset-20261008/deepseek-report-draft-response.json)。
