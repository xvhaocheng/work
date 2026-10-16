下面是**拟提交的报告草案**（未修改任何文件、未运行任何命令；本回复仅为供 root Codex 审定的提案）。建议路径：`docs/translation-continuation-frontier-20261008.md`。

```markdown
# 共享 IOMMU 地址翻译干扰：续走前沿报告草案

日期：2026-10-08。范围：只读源码与 primary 文献核查、纸面推演。

## 一、现状与总体结论

root Codex 与两席 native 已完成只读源码/primary 探索。三席一致：当前各项候选均被最强普通方法覆盖，不建议为其启动新 DUT；项目主线仍是多 DMA 共享 IOMMU 地址翻译的干扰与性能。以下每条给出最强前作、真实场景、未解约束、同信息同资源普通对照与停止条件。

关键的接口更正：结构性的 ready 阻塞路径（TL 在 COMPLETE 等返回）在正常 TC 不会成立为可达瓶颈。root 已直接核 `rv_iommu_trans_ctl_wrap.sv:675–702`：请求被接受后 TC 进入 AWAIT_RESP，`trans_resp_ready_o` 恒为 1；结果写入 TC 保存状态后才进 COMPLETE 等物理 AR/AW。故不得表述为 worker 诊断出已发生瓶颈；AW/W 背压在此正常请求链中钉不住 TL 的共享结果。

## 二、条目

### 2.1 共享 walk 结果驻留 / 慢 subscriber

- **最强前作**：Cong 等, HPCA 2017, DOI 10.1109/HPCA.2017.19（[作者 PDF](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf)）：不同步加速器的翻译复用、共享 TLB 过滤重复 miss、hit-under-miss、结果路由；官方 gem5 coalescer 固定 revision `392c1ced53827198652f5eda58e1874246b024f4`（committer 日期 2020-09-10），[tlb_coalescer.cc](https://gem5.googlesource.com/public/gem5/+/392c1ced53827198652f5eda58e1874246b024f4/src/gpu-compute/tlb_coalescer.cc) cc 95–124 / 140–211 / 211–222。
- **真实场景**：两 DMA 同 key 被合并后，一接收端 ready=0。合法 ready/valid 序列，但该步骤无法由当前 TC 的 AW/W 迟滞推出。
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

- **最强前作**：[US8140781B2](https://patents.google.com/patent/US8140781B2/en)（2009 公开）Fig3–6、step305–345、claims 2–3：tracking queue、completion token、合成下一层 packet 并重新仲裁。**新增已给定事实**：root 直接核官方 Rocket 2022 pinned 源码 commit `bf5742b9e353a80f501d43130218d3a444ec4b30`，[PTW.scala](https://github.com/chipsalliance/rocket-chip/blob/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala) :158–171（`aux_pte/r_pte/count` 等）、:196–214（由页号＋request 索引恢复 PTE 地址）、:524–565（父/leaf 复用与 G 返回恢复 S）。具体表示主体已被覆盖；这不是“笼统 ordinary 可抄任何算法”式的不新。
- **真实场景**：Sv39＋Sv39x4、两边三层、4 KiB leaf、无页表缓存的全冷路径最多 15 笔 PTE 读（`rv_iommu_ptw.sv:316–398,418–478`）。该路径未在 §43 运行中出现，也未由本席运行。
- **未解约束**：phase union 的最终位表尚由 root 与 native 核对；jointtag 准确位数未知（**不写作已验 82 bits，不宣称面积降幅**）。TL request/DC/PC/权限/context 仍须保持或保存，`update_o.vpn/pscid/gscid` 引用输入；已 offer 的 AR、已 accept 未返回的 R、最终结果/错误不可丢。
- **同信息同资源普通对照**：O1 原请求 token＋G/S target/最近返回 PTE＋等级/权限的 continuation tracking＋同样优先/RR；O2 安全边界 restart；O3 PWC/partial translation＋O1/O2。候选若仅做 offset 推导、phase union、删除不再使用字段，O1 必须允许这些同样具体优化；总存储需配平。
- **停止条件**：O1 同字段同保证追平 → 停止压缩机制推荐；O2＋同预算 PWC 追平工作成本 → 停止；纯 15 PTE 计数/字段删减不自动立新理论。

### 2.4 故障记录的短期容量背压

- **最强前作**：RISC-V IOMMU FQ 语义（[in-memory queues](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)）：满/内存错误置 OF/MF 后丢触发与后续记录，多个相同 fault 只报告 subset。
- **真实场景**：FQ 一条 record 需 AW→4W→B，WRITE 期间不接新 record（`rv_iommu_fq_handler.sv:183–279`）；`FaultFifoDepth=8`（`ariane_soc_pkg.sv:114`）；正常 DMA 故障全走单一 trans_fault port（`rv_iommu_sw_if_wrap.sv:245–302`）。TL ERROR 确会持续 fault_valid 等 fault_ready（`rv_iommu_tl_wrap.sv:1515–1540`）。
- **未解约束**：未执行自然可达实验；当前仅 §43 fault0 负载，不能宣称它是 §39 性能残差或 §43 让路时间的原因。
- **同信息同资源普通对照**：普通 worst-case credit 预约、per-DID VOQ、异步 record FIFO/reply 槽、精确 key 去重，均给相同合同；已有 FIFO 脱耦不能忽略。
- **停止条件**：ordinary 同容量同日志合同复现收益，或真实 iDMA 不能供出该 fault 序列 → 停止此枝。

### 2.5 方向性前作（用于边界，不替代上述构造）

HPCA21 DWS（[PDF](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)）p1/p4：owned walker 经 idle stealing 使 owner 新请求至多等一个 foreign walk，并未把整个 nested walk 等同一笔 PTE。ISCA18 Scheduling Page Table Walks（[PDF](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)）：按整体剩余 work 与同批调度。二者支持“续走/剩余工作调度已有前史”，不替代具体普通 checkpoint/restart/PWC 构造。

## 三、条件性未来分析目标（尚未证明）

在有限续走 state 容量下，nested 多设备的让路粒度、重复 PTE 工作与存储的 frontier，是否可给出新的可证明界，或达到该界的具体规则。当前未证明；必须与普通 checkpoint/restart/PWC 在同合同、同资源下比较。纯 15 PTE 计数或字段删减本身不自动构成新理论。

## 四、执行范围

本阶段仅只读源码/primary 核查与纸面推演。新 DUT build/run、生产 RTL/Linux 改动、板卡 BIT/JTAG/SD 动作均为 0；最新完整实测仍为 §43（静态 DEFAULT copy，TL66、miss34、PTW102、fault0）。源码见证不是 experiment。本报告不宣称新延迟/面积收益、已证明定理或论文分区。

## 五、事实清单（已核 / 已给定）

1. root 核 `rv_iommu_trans_ctl_wrap.sv:675–702`：AWAIT_RESP 中 `trans_resp_ready_o=1`，结果写入 TC 后才进 COMPLETE 等 AR/AW（`:718–728`）。
2. `rv_iommu_tl_wrap.sv:1545–1567`：正常结果 slow_resp 握手即回 IDLE，不等数据 RLAST/WLAST/B。
3. TL 单 FSM、单响应端口；`banked_ctl` 仅按 `.bank` 路由；无 dedup/coalesce/waiter 元数据。
4. IOTINVAL/IODIR 被 OR 成 fast invalidate（`rv_iommu_tl_wrap.sv:320–321`）；非阻塞 probe 可进 REJECT（`rv_iommu_nonblock_hit.sv:149–172`）；CQ 无 TL/PTW busy 输入。
5. Sv39＋Sv39x4 全冷路径 15 笔 PTE（`rv_iommu_ptw.sv:316–398,418–478`）；当前 PTW 无 A/D 原子写回（`:264,490–497`）。
6. Rocket pinned commit `bf5742b9e353a80f501d43130218d3a444ec4b30` 的 `PTW.scala` :158–171 / :196–214 / :524–565 内容如上。
7. FQ record AW→4W→B、WRITE 期间不接新（`rv_iommu_fq_handler.sv:183–279`）；FaultFifoDepth=8；单一 trans_fault port。
8. §43 实测为静态 DEFAULT copy，TL66、miss34、PTW102、fault0。
9. 本轮 2026-10-08 build/run/生产 RTL/Linux/BIT/JTAG/SD 均为 0。

## 六、待证清单（未解决）

1. phase union 的最终位表；jointtag 准确位数（不得预设 82 bits，不得据此推面积）。
2. 是否存在某表示在同 permission/context/error 合同、同总存储下，给出超越 O1/O2/O3 的“释放粒度—重复工作”组合，并附可达反例与下界。
3. 有限续走容量下 nested 多设备让路粒度 / 重复 PTE 工作 / 存储 frontier 是否可证明新界。
4. IOTINVAL 无关失效序列在真实 CQ 时序下是否自然出现（需动态命令实验，本轮未做）。
5. 故障记录短期容量背压在真实 iDMA fault 序列下是否自然可达（本轮未做）。
6. Rocket 2022 具体表示与 RISC-V nested 语义逐字段对应关系（仅记“主体覆盖”，非逐字段等价证明）。
7. 任何延迟/面积收益：本轮无综合、无实测，不得宣称。
```

## 120 字导航摘要

> 导航：三席只读核查确认共享 walk 驻留、IOTINVAL 取消、两阶段 phase union、故障记录背压四项均被最强普通方法覆盖，不建议新 DUT；主线仍为多 DMA 共享 IOMMU 翻译干扰与性能。ready 结构路径在正常 TC 不成立，jointtag 位数未核。

## 事实 / 待证清单（独立摘录）

**事实（已核或已给定）**
- F1 TC `675–702`：AWAIT_RESP 无条件 ready，结果先存 TC，COMPLETE 才等 AR/AW。
- F2 TL `1545–1567`：正常结果握手即回 IDLE，不等数据 B/RLAST。
- F3 TL 单 FSM、单响应口、无 dedup/waiter 元数据；`banked_ctl` 仅按 `.bank` 路由。
- F4 IOTINVAL/IODIR OR 成 fast invalidate；probe 可 REJECT；CQ 无 TL/PTW busy 输入。
- F5 两阶段全冷 15 笔 PTE；当前 PTW 无 A/D 写回。
- F6 Rocket pinned commit `bf5742b9e353a80f501d43130218d3a444ec4b30` `PTW.scala` :158–171/:196–214/:524–565。
- F7 FQ record AW→4W→B、WRITE 期间不接新；FaultFifoDepth=8；单 trans_fault port。
- F8 §43 实测静态 DEFAULT copy，TL66/miss34/PTW102/fault0。
- F9 本轮 build/run/RTL/Linux/BIT/JTAG/SD = 0。

**待证**
- T1 phase union 最终位表；jointtag 准确位数（不预设 82 bits，不推面积）。
- T2 是否存在同合同同存储下超越 O1/O2/O3 的“释放粒度—重复工作”点，并给反例与下界。
- T3 有限续走容量下 nested 多设备让路粒度/重复 PTE 工作/存储 frontier 的新界。
- T4 IOTINVAL 序列在真实 CQ 时序下是否自然出现。
- T5 故障记录短期容量背压是否在真实 iDMA fault 序列下自然可达。
- T6 Rocket 表示与 RISC-V nested 语义的逐字段对应关系。
- T7 延迟/面积收益（无综合、无实测，不得宣称）。

## 假设与未决（供 root 审定）

- 假设 root 已核的 TC/接口更正为最终版本；报告全文回避“worker 诊断出已发生瓶颈”的表述。
- 假设 IOTINVAL、nested per-PTE、FQ credit 三项的普通覆盖裁决维持“停止独立机制推荐、归工程基线”。
- phase union 位表与 jointtag 位宽未定，报告已显式置为待证，未给任何数值。
- 未包含编号引用表；primary 链接均就近置于主张处。
- 本 worker 未改文件、未运行、未建议全面构建，也未承诺论文分区；如需落地文件，请由 root 执行写入并复核链接可访问性。
