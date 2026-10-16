## 摘要导航
交付 **1 条具体规则草案 A**：叶级返回防火墙（Leaf-Return Firewall, LRF）。它只做一件事：N 快命中仍可探测，但当单 PTW 正处于**最后一级叶 PTE 服务窗口**且候选是**多拍数据读**时，不让该候选走 FAST accept，而是退回 REJECT/原慢路径等待。第二层“owner-drain 联合规则”经检查退化为角色选择 N + owner 优先 + R→AW coupler + 全缓冲证书，**不交付为创新候选**。裁定：A 是新提出规则文本但**新颖性未证**，很可能仍属“PTW 感知数据准入 + burst 长度”组合，不能包装首创；仅作为可试工程改进草案。

**关键证据定位**：N/REJECT/OUT 在 `rv_iommu_nonblock_hit.sv:100-103,157-169,172`；单 PTW 相位在 `rv_iommu_ptw.sv:100,108,385-390,393-417`；翻译返回后发物理 AR/AW 在 `rv_iommu_trans_ctl_wrap.sv:329-360,693-703,716-726`；n_window 在 `rv_iommu_tl_wrap.sv:303-306,317-329`；现有 owner 读准入在 `rv_iommu_banked_ctl.sv:246-278`；跨 bank AW 门控在 `axi_demux_simple.sv:175-177`；前作边界见 `composition-history-isolation-20261007.md:86-99,133,145,165-170,193-201` 与 `innovation-deeper-20261006.md:52-57,99-104`。

---

## 候选 A：叶级返回防火墙（LRF）
### 1. 一句大白话
N 可以提前算出翻译，但当单 PTW 正在等服务最后一级 PTE，而另一个 bank 的快命中是一条长数据读时，先不让这条读走 FAST accept；等叶级 PTE 离开或原慢路径可服务时再放行。不是拒绝 N，也不是全局限速。

### 2. 精确伪代码 / 条件 / 状态转移 / invariant
新增两个信号：

```systemverilog
// 1) trans_req_data_t 增加 8-bit len；由 bank 在打包翻译请求时从 AR/AW 复制。
// rv_iommu_trans_ctl_wrap.sv:631-637, 649-663 附近
trans_req_data_n.len = is_write ? aw_data.len : ar_data.len;

// 2) rv_iommu_ptw.sv 输出叶级服务窗口；只用于 S1-only normal walk。
// 现有 active_o 不能表示“下一项必为叶 PTE”；这里显式加 main_lvl_q==LVL3。
assign ptw_leaf_service_o =
    (state_q == MEM_ACCESS || state_q == PROC_PTE) &&
    (ptw_stage_q == STAGE_1) &&
    (main_lvl_q == LVL3);
```

N 侧只改 accept 条件，不新增状态：

```systemverilog
// rv_iommu_nonblock_hit.sv:100-103
assign leaf_hold =
    ptw_leaf_service_i &&
    is_read &&
    (candidate_i.len != 8'd0);   // 多拍读；PTE 读本身是单拍 len=0

assign accept_o =
    state_q == LEAF && enable_i &&
    candidate_valid_i && candidate_i == req_q &&
    !invalidate_i && ddtp_i == ddtp_q && dc_ok &&
    ddtc_dc_i == dc_q && leaf_ok &&
    !leaf_hold;

// LEAF：accept_o 为真 -> OUT；否则 -> REJECT（现有 :157-169）
// REJECT：等 normal_fire_i 或 candidate_valid_i 撤销（现有 :172）
```

**状态转移**：  
IDLE→DC→LEAF 不变。若 `leaf_hold=1`，LEAF 不产生 OUT，进入 REJECT；REJECT 保持 candidate 不变，等原 slow path `normal_fire_i`。已接受的 OUT 响应完全保留，不撤销 VALID。

**Invariant / 保证范围**：
- 只在 `n_window` 已限定的 S1-only、非 MSI、非 debug、另一 bank、主 PTW active 条件下生效。
- 写请求、单拍读、非叶级相位完全保持当前 N 行为。
- 不改变 AW 优先、W owner 读准入、跨 bank AW 门控、已发物理请求。
- **不是硬保证**：不声称 DDR/MIG 有服务界；若 PTW 相位判断不到叶级，或真实服务无界，只退化为经验性准入。

### 3. 两个可观测状态下的不同选择
| 状态 | 缓存/队列 | PTW 相位 | 候选 | 选择 |
|---|---|---|---|---|
| X | IOTLB hit，bank 队列深度 1 | `main_lvl_q=LVL2`，`state_q=MEM_ACCESS`，正在读上级 PTE | V 读 `len=255` | 允许 FAST accept；不认为会打断叶级返回 |
| Y | IOTLB hit，bank 队列深度 1 | `main_lvl_q=LVL3`，`state_q=PROC_PTE`，等待叶 PTE | 同一 V 读 `len=255` | 禁止 FAST accept；进 REJECT 等原慢路径 |
| Y’ | 同上 | 同上 | V 写 `len=0` 或短读 | 允许，避免误伤写排水和短请求 |

这直接区分“当前 waiting 另一个 PTE 但没有即将进入的读”与“即将/正在服务叶 PTE”，而不是只看 `active`。

### 4. 合法短时序例（非实测）
- T0：A 的 PTW 在 `PROC_PTE`，`main_lvl_q=LVL3`，叶 PTE 读未返回；bank0 出现 V 读快命中，`len=255`。
- T0：`leaf_hold=1`，`accept_o=0`，N 进 REJECT；V 的 AR VALID 保持，不握手。
- T1–T4：A 叶 PTE 返回，PTW 回 IDLE；V 仍在 REJECT。
- T5：`normal_gate` 允许原 slow path；V 被原 arbiter 接收，随后完成翻译并发出物理 AR。
- 结果：A 叶 PTE 不被 V 长数据读插队。对比基线：V 在 T0 快接受、T1 发物理 AR，A 叶 PTE 被推后。
合法点：不撤销 AR VALID；REJECT 等待原慢路径；OUT 未被消费。

### 5. 与强前作具体相同 / 不同 / 未知
- **相同**：MASK §5.4 已做页表优先、并依据 active walk/受阻 warp 安排数据服务；`composition-history-isolation-20261007.md:145` 已列为直接近邻。`innovation-deeper-20261006.md:99` 已有 R→AW coupler。普通 deadline/slack/TDMA/token 也会按阶段/时间片/信用限制数据启动。
- **不同**：MASK 是 Golden/Silver/Normal 队列与 token；本规则不改 DDR 队列，只在 RISC-V banked IOMMU 的 N fast accept 上，用“PTW 最后一级相位 + 候选 burst 长度”做一次性准入。它不重排已发请求，不新增信用窗口。
- **未知**：是否已有工作精确覆盖“nonblocking hit + 单 PTW + 叶级相位 + 多拍读长度”的组合；本轮不能证明没有。若再加超时上限，就明显退化为经验性 slack/deadline 控制。

### 6. 当前源码 hook 与额外成本量级
- `rv_iommu_nonblock_hit.sv:100-103`：加 `!leaf_hold`；`:157-169` 已有 LEAF→REJECT 分支；`:172` 已有 REJECT 退出。
- `rv_iommu_ptw.sv:100,108,385-390,393-417`：新增 `ptw_leaf_service_o`；不改变 `active_o` 语义。
- `rv_iommu_tl_wrap.sv:303-306,317-329`：把相位信号送进 `rv_iommu_nonblock_hit`。
- `rv_iommu_trans_ctl_wrap.sv:631-637,649-663`：在 `trans_req_data_n` 填 `len`；`:693-703` 的 AWAIT_RESP、`:716-726` COMPLETE 不变。
- `rv_iommu_banked_ctl.sv:246-278` 与 `axi_demux_simple.sv:175-177` 不动。
- **成本量级**：`trans_req_data_t` 增加 1 个 8-bit 字段；PTW 增加 1-bit 输出；TL 增加 1 条 wire；nonblock_hit 增加 1 个比较器/少量组合逻辑。不报 LUT 数量，需实现后由 Codex 实际综合。

### 7. 最小可证伪实验与停止条件
- 复用 `tests/nonblock_actual_dma*` 的真实 iDMA、banked controller、TL、固定 50/50 delayer；P/PN、A8、V gap0、transfers 64、warmup256、ptw-latency31，条件同 §8。
- 三组：P、原 PN、PN+LRF。记录 V 完成 W、A 有效速度、A miss/fill、fast 次数、V 数据 AR 与 A 叶 PTE 的先后、PTW active、DS 读延迟。
- 预测：若 A 的等待链成立，PN+LRF 的 V W 收窄，A 速度恢复；A miss/fill 不变；写和短读仍可有 N。
- **停止/证伪**：若 V W 不改善或 V/A 都更慢；若改善只靠人为增大总延迟；若 fast 数下降但检查事件后发现已无合法快命中机会；若写/短读也被误伤；若叶级相位根本没有快命中候选。`fast=0` 本身不算自动关闭 N，必须检查实际机会。

### 8. 失败 / 死锁 / 饥饿 / 无效可能性
- 候选在 REJECT 等待原慢路径；若 PTW 卡死或 fault，原路径最终完成/报错，不会因本规则形成新环，但 V 延迟会增加。
- 若叶级窗口占比过高，长读可能反复进 REJECT，V 饥饿；写和短读不受影响。
- `ptw_leaf_service` 若误把 S2/MSI/implicit walk 也判为叶级，会过度禁止；必须复用 `n_window` 的 S1-only 约束。
- 无真实服务界；不能把该规则写成硬实时保证。若后续加“超时后放行”，只是经验控制。
- 本规则不持有已接受翻译，故不新增 invalidation stale 风险；若改成“接受后暂缓物理 AR”，则必须补失效/中止语义，当前不予采纳。

---

## 第二层尝试：owner-drain / 角色联合规则
尝试过“W owner 正在占 W 通道时，只放行能解锁该 owner 的源读快命中”。它需要知道某条读是否属于未完成写。当前 `rv_iommu_banked_ctl.sv:246-278` 的 `w_unfilled_q` 只数是输入 AW→WLAST，不是下游所有权；input owner 不是下游 owner。要补真实关联需 iDMA 侧带 `src_for_owner` 元数据，而现有 `idma_channel_coupler.sv:105,140-179` 已做首拍 R→AW；`idma_fullbuffer_certificate.sv:16-18` 已有 4KiB 证书旁路；`innovation-deeper-20261006.md:52-57` 已有 `O+R≤B` 返回空间信用。该方向退化为 **角色选择 N + owner 优先 + 现成 coupler + 全缓冲证书 + 普通信用**，故不交付为创新候选。

---

## 严厉裁定
1. **候选 A（LRF）**：是新提出的具体规则文本，但**新颖性未证**。它与“PTW 感知数据准入”高度接近，且很可能被 MASK §5.4 的页表优先/数据服务协调覆盖；加入超时后更像普通 slack/deadline。不得宣称世界首创或已通过创新立项。  
2. **第二层 owner-drain 联合规则**：判定为**已有规则组合**，不交付为候选。  
3. 若 Codex 进一步原论文检索也判定 A 普通，则按用户要求，把 A 作为**一个具体可试工程改进草案**：先加 PTW 叶级相位 + `trans_req_data_t.len`，在现有真实 DMA 模型中只做最小对照，不直接开 FPGA 构建，不包装首创。  
4. 必要新颖性缺口由 Codex 继续原始检索：MASK 是否已用最后一级相位 + burst 长度；nonblocking TLB/hit 与单 PTW 相位组合；RISC-V banked IOMMU + 输入 W owner demux 下的具体准入规则。未见不等于无人做过。