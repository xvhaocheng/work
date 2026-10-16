# Nonblock v5 限定读接纳运行计划（2026-10-04，草案，未执行）

> 本稿为 worker 起草的文档 proposal；**未运行、未构建、未修改任何文件**。除所提供 JSON/MD 已有字段外，不声称任何通过或失败，也不声称 v5 编译已开始或全部验证完成。归属：Codex 关键 RTL、apply 与实际测试；本稿只做文档。
> 原版旧基线**不重跑**，不改 Linux/SD（`out/n-prototype-20261004/v5-build-state.json:8-9`）。本原型是前人已知有限 hit-under-miss 基线，**不主张 novelty**；full-transfer-buffer 亦已有工作（`docs/research-n-prototype-spec-20261004.md:137`）。

---

## 1. 目的与范围

v5 只回答一个问题：在**当前前端与冻结工作量**下，加入一项受限的“整 4 KiB 任务资格”读入口放行后，是否会首次出现**真实、合格的 fast 候选**（后续 probe / DC / LEAF / accept 是否被真正服务）。

- 这是**因果工程测试**，不是新方法声明。
- 安全论证只覆盖**当前有限前端 + 冻结 workload**（整副本、对齐、串行一次一个 job），**不是**通用 AXI 或任意多 job 的无死锁证明。
- 若仍无候选，**读写生成器耦合**是保留下来的另一条替代假设，不得据本轮反推“无机会”（`out/n-prototype-20261004/analysis-v4-pilot/RESULTS_REVIEW.md:174-176`）。

---

## 2. 读接纳目的（老师式说明）

问题：写通道当前“W owner”在收到 LAST 之前占住输入 demux 的 W 归属；此时**另一路源读数据**若先回来，会把该 DMA 的 R→W 耦合 FIFO 填满，而这一路的 AW 又进不去 demux，结果是**共享内存 R 通道无法再返回当前 owner 的源数据或页表读**，形成**循环等待**（`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:246-256`）。

做法：**延迟“选择”这类读，而不是撤回任何 AXI/TL VALID**（同上 `:250-251`）。所有权与优先级保持原样：

- 未收到 LAST 前保持 W ownership，用有符号 `w_unfilled_q` 计 AW 减 LAST（最小 −1 表示已预留的头，避免无符号下溢）（`:253-257`、`:274-275`）。
- `restrict_reads = (w_unfilled_q > 0) || aw_valid`，`read_owner` 取当前 owner 或本拍 AW 的 bank（`:259-260`）。
- 新 AW 在同一拍进入某 bank 时，它的 FIFO 头**下一拍才可见**；因此用 `!(aw_accept && aw_sel)` 阻止当拍选旧 AR（`:261-268`）。
- AW FIFO 满（无 AW handshake）时，允许当前 owner 继续读、排空已备好的写（`:263-264`）。

AW 优先于 AR、AW/W 关联、stable held VALID 一律保留（`cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:623-624`、`:628`、`:645-666`）。

---

## 3. v5 证书（模块级，整任务资格）

`cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv`：

- 用途：只对**一笔**完整、对齐的 4 KiB 普通复制授予“读入口放行”；只采样**已接纳的前端 job** 与**已退役的后端 job**，**从不改动任一手握**；寄存器前端串行化 job；**意外重叠/退役则撤销该许可直到复位**（`:2-5`）。
- 资格：`CapacityBytes >= 4096 && normal_copy_i && length_i == 4096 && src_i[11:0]==0 && dst_i[11:0]==0`（`:16-17`）。
- 放行：`permit_o = pending_q && eligible_q && !fault_q && normal_copy_i`——即需要**当前仍为 normal copy 的活体门**（`:18`）。
- 生命周期 FSM（`:26-42`）：
  - `{fire,done}=2'b10`：若已 pending 则置 `fault_q`（意外重叠）+ 置 pending/eligible（`:27-31`）。
  - `2'b01`：若未 pending 则置 `fault_q`（意外 done）+ 清空（`:32-36`）。
  - `2'b11`：替换一笔已完成 job；无前驱的即完成 job 不需读许可（`:37-42`）。
  - `fault_q` 只在复位清（`:24`）——**sticky**。

集成（`cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv`）：

- `capacity = BufferDepth * (AxiDataWidth/8)`（`:172`）；默认 `BufferDepth=3`（`:27`）。
- `job_fire_i = be_valid_d && be_ready_d`（job FIFO 接纳，`:175`）；`job_done_i = idma_rsp_valid && idma_rsp_ready`（backend done，`:176`；后一信号恒 `1'b1`，`:205`；`be_trans_complete = idma_rsp_valid`，`:204`）。
- `normal_copy_i = (ttype == idma_pkg::DEFAULT)`（`:177`）；`permit_o → fullbuffer_job_o`（`:181`）。
- 透传：`dma_core_wrap_intf.BUFFER_DEPTH`（`:364`）→ `dma_core_wrap.BufferDepth`（`:427`）。

即：**证书把“前端 job 接纳→后端 done”这一段串行边界**作为其许可的时间窗。

---

## 4. 前端串行边界与限定条件

- 前端只启动**正好 4096 B、源/目的 12 位对齐**的 job：`num_bytes==4096`（`cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:148`）、`src_addr[11:0]==0 && dst_addr[11:0]==0`（`:150`）。
- `COUNT` 发出 VALID，地址在 ready 前保持稳定（`:512-521`）；**只有 ready 才进 `WAIT`**（`:521-522`）；`WAIT` 等 `trans_complete_i`（`:537-539`）。
- job 事件：`batch_task_issue = COUNT && ready_i`（`:151`）；`batch_task_done = WAIT && trans_complete_i`（`:152`）。
- 读入口放行**只对 DMA0/1 的 DID10/11**：`FullBufferDID = 24'(10 + b)`，b∈{0,1}（`rv_iommu_banked_ctl.sv:319`），且 `ReadAdmissionEnable=1'b1`（`:318`）。
- 放行例外只挂在读入口：`(!ReadAdmissionEnable || read_admit_i || (fullbuffer_job_i && ar_data.stream_id == FullBufferDID))`（`rv_iommu_trans_ctl_wrap.sv:646-647`）。**其它 DID 与所有普通 AW/W 所有权、AW 优先、stable held VALID 一律保留**（同上 `:628`、`:645-666`；`rv_iommu_banked_ctl.sv:246-278`）。

**范围安全论证**：以上限定依赖“当前前端只会发起整 4 KiB、对齐、一次一个 job”的有限形状，因此不是通用 AXI / 多 job 无死锁证明（`rv_iommu_banked_ctl.sv:246-256` 的循环等待是模型层解释）。

---

## 5. 当前实际状态（只引用已有字段）

`out/n-prototype-20261004/v5-build-state.json`：

- `revision = v5-qualified-read-admission`（`:2`）。
- `stage = host_validation_in_progress`（`:3`）——**编译尚未开始，不得写成 build started**。
- **证书单元 PASS**：`certificate_unit_exit = 0`（`:4`）。
- **受限限定 + 受护 proxy RR12/depth512 PASS**：`coupled_qualified_RR12_exit = 0`、`coupled_guarded_RR12_exit = 0`（`:5-6`）。
- **独立 banked 测试 FAIL**：`banked_ctl_test_exit = 1`（`:7`）——**待首次错误定位**。
- `linux_changed = false`、`sd_update_required = false`（`:8-9`）。

因此**不能**写“全部验证完成”，也**不能**写 v5 已开始构建。

---

## 6. 冻结 S/N 对照

- **冻结 S/N**：相同 qual/source/容量（DMA0/1 各 4 KiB），但**不同编译 BIT**，故物理布局/面积/时序不同 → 属**多次新 boot**，非同一 BIT 上运行时切换（`RESULTS_REVIEW.md:29-37`；`docs/research-n-prototype-spec-20261004.md:105`、`:119`）。
- 计划：**S、N 各按原 15 case 计划**（15 default sequence，配 `--diagnostics`）；**旧样本不重跑**；可选诊断沿用 **old ABI / Linux 00093 / reader 不变**，**不改 SD**（`docs/research-n-prototype-spec-20261004.md:172`；`docs/nonblock-pilot-plan-20261004.md:115`）。
- **原版旧基线不重放**；`original-03` 复用已有旧基线（`RESULTS_REVIEW.md:31`、`:38`）。
- **原版→S 混淆**：同时含结构 + buffer 差异（3 拍 vs 512 拍），**不得作纯布局因果**（`RESULTS_REVIEW.md:40`、`:116`）。
- **S/N 只作条件性描述比较**，同容量 + 同 default plan，但不同 BIT；**不做统计显著性**（n=3 只给描述性中位/min/max）（`RESULTS_REVIEW.md:37`、`:39`、`:117`）。

---

## 7. 预期结果分支

按“先看 actual candidate/reject/accept，再谈 cause/benefit”的顺序（`docs/research-n-prototype-spec-20261004.md:172`）：

- **分支 A（首次出现候选）**：诊断 bank bit2 `candidate` 出现 → 检查 probe/DC/LEAF/accept 计数，并核对**同一窗口**内 `diag fast == core fast sum`（`docs/research-n-prototype-spec-20261004.md:167`）。若真实 accept，则再评估是否被真正服务。
- **分支 B（仍 fast0）**：`candidate` 仍 0 → **科学未行使（scientific nonactivation），不是技术失败**（`RESULTS_REVIEW.md:150`）。此时**读写生成器耦合**是保留的另一条替代假设（`RESULTS_REVIEW.md:174-176`）；不得写成“无机会”，也不得把上游守卫写成已证唯一 cause（`RESULTS_REVIEW.md:175-176`）。
- **分支 C（证书 fault）**：`fault_q` 被置（意外重叠/迟到 done）→ 许可被撤销直到复位，属**技术性诊断点**，应停下核对证书时间窗与前端 COUNT→WAIT 串行边界（`idma_fullbuffer_certificate.sv:5`、`:27-36`；`idma_reg64_frontend.sv:151-152`）。
- **分支 D（协议/数据错误）**：任何首次技术性错误即停止。

---

## 8. 停止 / 错误 / 不自动重跑规则

- **技术性错误首次出现即停止**；`fast0` 是科学负结果，**不是技术性停止条件**（`docs/research-n-prototype-spec-20261004.md:172`；`docs/nonblock-pilot-plan-20261004.md:115`）。
- 不自动重跑：失败/已通过的样本与旧原版 15 项一律不重跑；`original` 旧基线不复放（`docs/research-n-prototype-spec-20261004.md:162`、`:172`）。
- 不推测量子未知的 build resource/timing，不新增 novelty（同上 `:173`）。
- 当前 `banked_ctl_test_exit = 1`：**先定位首次错误**，不进入构建（`v5-build-state.json:7`）。

---

## 9. 验收门（gating，按顺序）

1. **真实且有限（bounded）的测试通过**：证书单元、受限限定 + 受护 proxy、**以及当前 FAIL 的独立 banked 测试**须通过；未通过前**不 gate 构建**。
2. **直接 critical review**。
3. **最终时序检查**。
4. **板级数据/协议**（15 case 计划）**并且**必须落到**同窗口**的 actual candidate / probe / accept 与 core fast（`docs/research-n-prototype-spec-20261004.md:167`）。
5. **`fast0` 视为科学未行使，不记技术失败**（`RESULTS_REVIEW.md:150`）。

在上述 1–3 全部成立之前，**不得**声称验证完成或编译/板测开始（`v5-build-state.json:3`）。

---

## 10. 保留声明（明确不做）

- 不重跑旧原版 15 项；不复放旧基线；不改 Linux/SD（`v5-build-state.json:8-9`）。
- 不宣称 novelty；不宣称 v5 已有性能收益（`docs/research-n-prototype-spec-20261004.md:137`）。
- 不把上游读守卫写成已证唯一 cause；本轮从未进入 probe（`RESULTS_REVIEW.md:175-176`）。
- 不把原版→S 差值当纯布局因果（`RESULTS_REVIEW.md:40`、`:116`）。
- 不把 S/N 当统计显著性结论（`RESULTS_REVIEW.md:39`）。
- 不新增与 v5 无关的未来设想。

---

## 11. 证据路径

- 当前修订状态：`out/n-prototype-20261004/v5-build-state.json`（`:2-9`）。
- v4 结果复核（candidate=0 / enable bit / 守卫非唯一 cause）：`out/n-prototype-20261004/analysis-v4-pilot/RESULTS_REVIEW.md`（`:29-40`、`:134-153`、`:169-177`）。
- v4 运行计划与诊断约束：`docs/nonblock-pilot-plan-20261004.md:109-124`；`docs/research-n-prototype-spec-20261004.md:158-173`。
- v3 归档映射：`v3-accepted-builds/RELOCATION.json`（`docs/research-n-prototype-spec-20261004.md:171`）。
- v5 源码：
  - `cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv:2-42`
  - `cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:171-182`、`:204-205`、`:364`、`:427`
  - `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:148-152`、`:512-563`
  - `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:97`、`:246-278`、`:314-358`
  - `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:23-25`、`:58-63`、`:623-669`
