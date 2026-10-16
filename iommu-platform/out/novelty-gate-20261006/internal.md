# 创新审查：项目内证据提取（只读）

只读范围＝给定 6 份文档 + 唯一逐行源码 `dma_research_policy.sv`。凡引用 `idma.c` / `idma_reg64_frontend.sv` / 顶层 / TL wrapper 的行号，均来自 `platform-feasibility-20261006.md` 的转述，**本次未读到这些文件原文**，已逐条标注。

## 0. 可导航摘要（6 条结论）

1. **旧策略块只识别 DID，不识别页。** `dma_research_policy.sv:96-124` 全部输入中与“身份”有关的只有 `ctl_did_i/select_did_i/lookup_did_i` 三个 DID；无 IOVA、无页号、无 PTE、无工作集、无未来信息。窗口是**固定周期计数**（`window_q`，默认 16384，`:706`），仅在“全 V-active 窗口”上做反馈与直方图（`:305-307`）。→ 候选 A 要的“页工作集/复用”**在现实现中确实不存在**，属“项目未做”。
2. **p4 含 PTW 事件，且用 `ctl_did_i` 限定。** `cost_ev(mode4)=whit·hit + wmiss·miss + wptw·ptw`（`:325-327`），`ev_ptw_a = ptw_r_fire_i && (ctl_did_i==DID_A)`（`:288`），是**PTW 返回**事件（`:57`），不是 PTW 发起。命中/未命中用 `lookup_did_i`，PTW 与 p5 用 `ctl_did_i`——限定符不同（`:285-288`）。
3. **p5 计费＝按周期计，不是按事件。** `ev_actual_a = (ctl_did_i==DID_A) && (state∈{REQ_TRANS,AWAIT_RESP}) && (state!=COMPLETE)`（`:290-293`），每周期计 1（`:328`）；准入＝`bal_next>=0`（`:600`）；窗口末 `refill_en = win_last & mode∉{0,6}`（`:344`）即回填 `b_q`，正侧受 `credit_cap` 截断、**债务不豁免**（`:358`）。误写禁区见 §2。
4. **p6 不计费，只做 gap 节奏 + V 反馈。** `cost_ev=0`（`:329` 默认枝）；准入＝`m6_cd_next==0`（`:599`）；每次 A done 用 `b_q` 重载倒计时（`:422-424`）；反馈方向与计费模式相反（V 慢→gap 加长，`:389-391`）。**fixedgap 不在本策略块内**，是驱动侧寄存器 gap（`platform-feasibility:17,29`）。
5. **method88 阴性只锁死“固定 A8 实占主路线”，不锁死所有反馈。** 四格对最强匹配简单对照全负（`RESULTS:31-34`），p5→已测基线（`:100`）；但同一报告明写“不宣称实际占用计费普遍无用”（`:102`），且反向 p5-vs-p6 复现（`:49,51`）不能升格为跨方向结论（`:57-58`）。
6. **立题硬缺口：公平目标仅“原则级”已定义、新候选阈值未冻结；真实场景/地址路径证据缺失；相比强普通规则的可核差异尚无正例。** 见 §6 未满足清单。

---

## 1. 方法逐项核对（fixedgap、p1..p6；p7 附注）

共同基础设施（所有模式）：`win_run = armed & ~frozen`（`:303`）；`win_last = win_cnt == window-1`（`:304`）；`win_complete = win_last & act_all_now`（`:307`，`act_all_now = win_allact_q & v_active_i`，`:305`）；反馈/直方图/短窗统计**只**在 `win_complete` 生效（`:463-469`、`:770`），回填在**所有** `win_last`（`:344`）。

| 方法 | 真正“输入/事件” | 计费量 | 反馈量 | 准入条件（何时 allow） | 窗口/预测是否用页身份或未来信息 |
|---|---|---|---|---|---|
| fixedgap | 驱动 gap 寄存器（32 位，`idma_reg64_frontend.sv:187` 解码 0xd0；驱动 `idma.c:2547` 写入）；完成后等待再发下一项 | 不在本策略块；由前端 COUNT→WAIT→GAP 节奏实现（`platform-feasibility:29`，引 `idma_reg64_frontend.sv:519`） | 无（开环固定值） | 无策略 gate；节奏由 gap 寄存器决定 | 否 |
| p1 字节（mode1） | `a_issue_i`（`:322`） | 每 A issue 事件 4096 B / `BYTES_DONE`（`:132,322`） | V 完成字节 vs target±deadband（`:381-396`） | 默认枝：`bal_next>=0`（`:600`） | 否；窗口=周期计数 |
| p2 请求（mode2） | `select_fire_i && select_did_i==DID_A`（`ev_sel_a`，`:284`） | 每 A select 事件 =1（`:323`） | 同上 | 同上 | 否 |
| p3 miss（mode3） | `lookup_i && lookup_did_i==DID_A && !lookup_hit_i`（`:285-287`） | 每 A 所属 lookup miss =1（`:324`） | 同上 | 同上 | 否 |
| p4 事件加权（mode4） | hit=`ev_lut_hit_a`、miss=`ev_lut_miss_a`、ptw=`ev_ptw_a`（`:288,325-327`） | `whit·hit + wmiss·miss + wptw·ptw`；默认 `whit=1,wmiss=100,wptw=100`（`:224-226,714-716`）；ARM 拒绝全零权重（`:512-513`） | 同上 | 同上 | 否 |
| p5 实占（mode5） | `ctl_state_i`、`ctl_did_i`（`:290-293`） | **每周期** 1，条件是 `ctl_did==DID_A` 且 state∈{REQ_TRANS(1),AWAIT_RESP(2)}；COMPLETE(4) 永不计（`:91-93,289,328`） | 同上（仍用 V 完成字节，不是实占反馈） | `bal_next>=0`（`:600`）；窗口末回填 `b_q`（`:344-345`） | 否 |
| p6 仅 V 反馈（mode6） | `v_done_i`（反馈）、`a_done_i`（gap 重载）、`v_active_i`（窗口） | **0**（`:329`） | V 完成字节 vs target±deadband，但方向相反：V 慢→`fb_inc`→gap 增大（`:389-391`） | `mode6: allow_core = (m6_cd_next==0)`（`:599`）；`a_done_i` 时 `m6_cd_next=b_q`（`:422-424`） | 否 |
| p7 滞后（附注，不在本轮核 range） | 同 p5 的 `ev_actual_a` | 上一完整窗口的 `prev_main_q` 在下一个窗口末一次性计入（`:333-338,762-767`） | 同 p5 | 同计费模式默认枝 | 否 |

关键单元语义（供核）：

- **反馈量**：`obs_s = $signed(vdone_bytes_now)`，`vdone_bytes_now = win_vdone_q(+v_done)·4096`（`:306,310,381`）；对计费模式 `fb_inc = obs > tgt+db`、`fb_dec = obs+db < tgt`（`:393-394`）；只有 `win_complete && mode!=0 && (fb_inc||fb_dec)` 才写 `b_q` 并更新 bmin/bmax（`:770-773`）；`b_next` 受 `minbud/maxbud` 夹取（`:399-405`）。
- **准入合流**：`policy_allow_o = enable_core & ~abort_eff & ~block_all & allow_core`（`:610`），`enable_core = Enabled & armed & ~frozen`（`:586`），`block_all = err_any_reg | err_any_now`（`:594`）。ARM 还需 `quiet_i`、未 armed/frozen、`supported_i`、非 `cfg_bad`（`:529-537,503-513`）。`policy_abort_o` 只用**寄存器化**错误/abort（`:616`），同周期计数溢出只关门不放 abort（`:611-615`）。
- **前端握手落点**：ADMIT 不发 VALID、收到 allow 且 backend idle 才进 COUNT；COUNT 发 VALID 后必须保持到握手（`platform-feasibility:31`，引 `idma_reg64_frontend.sv:588`）。**当前配置 `research_role=0`，旧 policy 未实例化**（同条，引顶层 `:1099,:1280`）。

---

## 2. 特别核对：p4 的 PTW、p5 的计费/停止、误写禁区

**p4 是否含 PTW 事件：是。** 三段串联，缺一不可：
- `ev_ptw_a = ptw_r_fire_i && (ctl_did_i == DID_A)`（`:288`），是**返回/回填完成**事件；
- 计数器注释同义：`E2D A-owned PTW return events (ctl_did==DID_A and ptw_r_fire)`（`:57`）；
- 权重项 `+ (ev_ptw_a ? {32'd0, wptw_q} : 64'd0)`（`:327`）。
- 边界：限定符是 `ctl_did_i`，**不是** `lookup_did_i`（`:285-288`）。`platform-feasibility` 把 stale-DID 警告明确挂在 `0xE2E`（A-owner cycles）而非 p4/p5 计费上（`RESULTS:77` 引 `dma_research_policy.sv:58,199,294,451`）。**p4 的 ptw 项是否继承同一 stale-DID 风险，本轮材料未裁定**——属需根确认的架构问题（见 §7 不确定性）。

**p5 计费/停止条件（不可误写）：**
- 计费区间＝`REQ_TRANS|AWAIT_RESP` 的**每个周期**，非事件计数（`:290-293,328,455,661`）。
- 唯一被排除的非 IDLE 状态是 COMPLETE（`:289,93`）；`RESULTS:78` 据此判“不存在漏 DDT/PDT/PTW 计费主张”：`REQ_TRANS` 发出翻译请求、`AWAIT_RESP` 等 `trans_resp_valid_i`，内部响应全程落在计费区间内。
- 停止条件＝`bal_next < 0` 关闭准入，直到下一次 `win_last` 回填（`:344-345,600`）；正侧封顶、**债务永不豁免**（`:358`）；`refill_en` 覆盖**部分 V-active / V 不活跃**的窗口，以避免 A 因空预算死锁（`:340-344`）。
- COMPLETE 的 busy cycle 可另行观测（`cost_comp_q[1]/cur_a_comp`，`RESULTS:79` 引 `dma_research_observer.sv:1528-1535,1041-1060`），**不应混入 E2E**。
- 误写禁区：`0xE2E` 含 IDLE stale DID，**不能作真实服务/占用分母**（`RESULTS:86` 引 `:58,199,294,451`）；`0xE25` 是 `permission_closed_cycles`（由**寄存器化** error/abort 或 balance deny 构成，`:46-49,604-607`），**不是 ADMIT-blocked cycles**，也不得作分母（`RESULTS:86`）。
- 附加不变量：`main_inc`（mode7）只累计 `ev_actual_a`，**不是**全部 A-owner 周期（`:336-338`）。

---

## 3. 候选 A（页面工作集感知 DMA 提交控制）能力盘点

**已经有的能力（项目内可核）**
- 驱动侧**已知源/目的 IOVA 与页数**、batch 自主循环（`platform-feasibility:18`）。
- 静态 A 页数可选 `{1,8,16}`、4 KiB chunk、V gap `{0,1000,4000}`、A gap=0（composition 校验 `:2775`，`platform-feasibility:27`）；底层通用页数检查 1..4096、前端 batch 支持同环长（`:936`，同条）。
- 每 lane 32 位 **gap 寄存器**已贯通，完成驱动节奏（`platform-feasibility:17,29`；驱动 `:2547`）——**本项目已做过固定 gap 限速**（`platform-feasibility:5,7`）。
- **ADMIT/allow 握手源码存在**（前端 `:588`），策略块可组合式按周期开关；`audit` 点：`policy_allow_o`（`:610`）。
- 反馈式准入已有落地：p1..p6 的负反馈 + 单控制变量 `b_q`（`:377-405`）。
- 文献侧已研究“页局部性↔翻译压力”：IOTLB Wall 2023 的每 MiB miss、缓冲区局部性/回收、大页感知分配器（`first-paper-options:35`）。

**确实未出现的“输入/动作”（＝项目未做）**
- 策略块**无任何页/地址输入**：输入清单仅 DID 三路 + 事件/状态/配置（`:96-124`）。无 IOVA、页号、PTE、页集合、复用计数。
- 无 **在线翻译压力推断**：所有计费都是事件计数或状态周期，反馈都是“窗口 V 完成字节 vs 固定 target”。
- 无**逐任务端到端耗时/尾延迟**采集接口（`platform-feasibility:33`；`first-paper-options:50` 明说“没有逐作业时间戳时只报告现有完成窗口”）。
- 当前 composition **强制 A gap=0**，研究入口/BIT 尚未落地（`platform-feasibility:17`：最小新增工作＝有版本边界的 CLI/UAPI/校验入口，且“尚未实际验证新软件”）。
- 无**无停顿**运行中模式切换；S/N/P/PN 切换要求 quiet + 清 IOTLB（`platform-feasibility:20`）。
- 无任意描述符重排 / 可变多任务在途 / 并行 walker（`platform-feasibility:21`）。

**尚未具体定义的概念**
- “近期页集合/近似计数”的**键与粒度**：文档只要求按实际地址空间/翻译上下文与页粒度定义、合理去重，并明确“不能认定 DID 总是 IOTLB tag”（`first-paper-options:59`）——**具体键/容量/淘汰规则未定义**。
- 代理**预测什么**：`first-paper-options:29,58` 只给“页信息预测翻译压力”的目标陈述，未给可核定义；并已预告 PWC/预取/容量/页大小/其它设备会使其失准（`:58`）。
- **控制成本口径**未量化：页集合维护、CPU 周期、元数据、提交延迟、批处理摊销、阶段切换、后台积压（`first-paper-options:61`）。
- “新 A 相对 p1..p6 新增的信息/动作/场景”**是要求而非已写内容**（`platform-feasibility:74` 原文“先写清新 A 相对 p1..p6 所新增的信息、动作和场景”）。

**区分：项目未做 vs 领域没人做（后者证据不足，须按已核前作设限）**
- 页局部性→翻译压力：**领域已有**（IOTLB Wall 2023，`:35`）。
- 翻译感知准入/调度：**已有**（MASK ISCA 2018，`:38`）。
- 利用描述符顺序/批量：**已有**（rIOMMU 2015，`:37`；`performance-council:96`）。
- 多 DMA 干扰 + per-stream QoS 方向：**已有**（Bermuda §V-C，`:36`；该稿缓解机制实验未完成）。
- 平均吞吐反馈→延迟 CDF 反馈：**已有**（Memory Latency Distribution-Driven Regulation, ECRTS2023；`research-method-next:12`）。
- PTW 调度/批量合并：**已有**（Scheduling Page Table Walks ISCA18、Neighborhood-Aware Translation MICRO18；`performance-council:101`）。
- 因此“按页限速/两个预算/首次反馈准入”均**不构成**差异；`platform-feasibility:54` 原文禁止把“第一次反馈准入”当差异。

---

## 4. 候选 B（共享 IOMMU 配置适用边界与选择规则）能力盘点

**已经有的能力**
- 同一 BIT 支持 S/N/P/PN（`platform-feasibility:16`）。
- 观测：窗口、lookup/hit/miss、fast accept、填充、缓存前后快照（`:16`）；驱动 generation 校验并试验后恢复 S（`:32`，引 `dma_composition_control.sv:153,182`、顶层 `:1042`、驱动 `:2278`）。
- 功能验收 172/172（short16/noise12/explore144，`:40`）；explore＝4 mode × 4 A 页数(含 solo) × 3 V gap = 48 条件、单 boot 内三 block（`:42`）。
- 已复算的条件差异（PN vs N）：A8 g0 → W +16.06%~+16.33%、r_A −14.04%~−13.84%；A16 g0 → W −1.28%~−1.16%、r_A +1.17%~+1.29%；A8 g4000 → W −3.21%~−2.93%、r_A −31.09%~−30.90%（`:46-51`，来源 `metrics-spotcheck.json`）。
- 计划中的对照口径：同一 BIT、同一仪器、同一工作集、同一完整 copy 作动点比较 S/N/P，报 V 端到端、A 同窗有效完成字节、LUT/FF/BRAM/Fmax（`RESULTS:110`）。

**确实未出现的“输入/动作”**
- **因果归因未完成**：`first-paper-options:68` 原文“已有条件差异尚未完成因果归因”。
- **无未见留出集**：172 条均已看过、单 boot 三 block，不能当新验证集（`:68`；`performance-council:71`）。
- 无**选择规则/预测器**：尚需“在新工作集/释放节奏/实现配置上预先预测再验证”（`first-paper-options:70`）。
- 无**“可部署选择器”与“oracle 标签”的拆分**：事后知道四模式结果再选最佳＝oracle，选择信息必须事先可得、试探与切换成本计入（`:77`）。
- 运行中切换不成立：需 quiet + 清 IOTLB（`:20`）。
- 无经验证的在线一致采样接口、无完整逐 job 时间序列；旧 HPM 标为 raw_only，不能替代已定义窗口指标（`:33`）。

**尚未具体定义的概念**
- “对配置选择有用且已研究未充分解释的具体条件”未定（`first-paper-options:70`）。
- “简洁选择规则”未定；目标图（`局部性/提交节奏 → 最佳配置`）只是图轴建议，不是规则（`:72`）。
- 选择代价口径未定：相对固定选择/简单 miss 阈值的**错误选择损失、测量成本、不适用范围**（`:72`）。

**区分：项目未做 vs 领域没人做**
- 容量分配 × 带宽争用联合考虑：**已有**（KPart，`performance-council:69,106`；`first-paper-options:106`）。
- 观测/决策主张受 HELM/CounterPoint/Coz 约束（`:106`）。
- 最强对照族：IOTLB Bottleneck 2010、Bermuda、MASK、KPart（`first-paper-options:74`；`performance-council:69`）。
- “组合反噬/miss 少未必快/统计后选配置”**均非新发现**（`first-paper-options:111`）。

---

## 5. method88 阴性对提案的硬约束（可核）

来源：`RESULTS.md`、`research-method-next-20261004.md`、`platform-feasibility-20261006.md`。

1. **四格全负是“固定 A8 实占主路线”的终止依据**：boot1 fwd vs p4 −5.68952%、boot1 rev vs p4 −8.23689%、boot2 fwd vs p1 −5.15142%、boot2 rev vs p2 −4.43680%（`RESULTS:31-34,36`）；跨方向 False（`:38`）；p5 转为已测候选/基线（`:100`；`research-method-next:60`）。
2. **不得为 p5 重挑有利条件**（`RESULTS:100,107`；`research-method-next:21,60`）。
3. **不得把“首次反馈准入”当新意**（`platform-feasibility:54`）。
4. **新 A 必须写明相对 p1..p6 新增的信息/动作/场景**，并在**可比的 A 有效完成率/双方曲线**下比较字节、请求、miss、事件加权、实占、仅 V 反馈；计入软件与控制开销；**静态已知页数不能冒充在线推断**；大页/复用等适用强基线必须一并评价；只有重述旧机制或为 p5 挑条件→停止该提案（`platform-feasibility:75`）。
5. **不得把不同 BIT/内核数据拼成同条件比较**（`:74`）。
6. **反向 p5-vs-p6 复现不可升格**：两启动 reverse 均匹配且 p5 更高（+34.68150% / +30.99451%，`RESULTS:49,51`），但 forward 一格低于 5% 阈值、一格保护未匹配（`:48,50`），因此“不存在跨方向普遍结论”，且不构成 p5 独有收益证明（`:57-58,101`）。fixedgap 仅作参照，两格未匹配一律保留披露、不作收益声明（`:44-47,59`）。
7. **不得反向外推成“所有反馈都无效”**：`RESULTS:102` 明写“不宣称实际占用计费普遍无用，也不宣称已证明新机制”。这正是本次任务要求的边界。
8. **统计口径约束**：5% 仅工程参考、非统计保证；每启动/方向 n=3 只是工程重复；两启动非两次全新启动（boot1 含既往 462 项活动）（`:83-85`，`research-method-next:37,47`）。
9. **读数禁区**：`0xE2E` 含 IDLE stale DID 不能作分母；`0xE25` 不是 ADMIT stall 且不得作分母（`RESULTS:86`）。
10. **E2/E3 只支撑有限布局/实现范围**，不能推持续吞吐归因；method88 不重做、不扩展 E2/E3（`:94-95`）。
11. **参数来源约束**：method88 复用既有 selection 参数、不改阈值不重校准（`research-method-next:27`）；主张与筛选规则须**提前冻结**，且 88 是看过旧数据的复核、非盲测（`:47`）。

---

## 6. 立题检查：公平目标 / 真实场景-地址路径 / 强普通规则可核差异

| 检查项 | 状态 | 证据 |
|---|---|---|
| **明确公平目标** | **原则级已定，新候选阈值未冻结（部分满足）** | 已定：x＝后台有效字节/秒、y＝敏感任务端到端耗时，禁止“敏感延迟÷后台字节”，禁止只挑很差的限速参数（`first-paper-options:50,54`）；固定逻辑任务/输入/权限语义/合理资源预算（`:52`）；新规则参数与资源不得享不公平优势、不靠人为禁用保护候选（`:47,60`）。旧实验已有可核判据：V 两侧中位速率 ≥ target 170、`|medianL−medianR|/mean ≤5%`、E23/E20 不足比例差 ≤10pp、每 run 有效完整窗口 >0、A 增益 5% 仅工程参考（`research-method-next:37`；`RESULTS:19`）。**缺**：新候选 A/B 的具体 target、对照参数空间、资源预算与阈值尚未出现。 |
| **真实场景/地址路径证据** | **未满足** | `performance-council:15`：有限 descriptor、搬运/计算交替、ping-pong 复用“**目前只是结构化合成假设，没有本项目实际应用轨迹支持**”；`platform-feasibility:81`：当前多 iDMA 微基准、Mac 控制网口不能冒充真实 NIC/GPU 共享翻译应用；`first-paper-options:86`：应选可核实数据搬运工作流，参数化 workload 必须称“工作流模型”，应用级主张需真实程序或可信轨迹。**缺**：任何真实地址路径轨迹/实际程序入口。 |
| **相比强普通规则的可核差异** | **未满足（仅有要求，无正例）** | 要求：`first-paper-options:110,115,117`；`platform-feasibility:75`。否定判据已写：`first-paper-options:117`“**尚无新证据证明上述实例存在**”，并要求“普通控制判断错误的正例 + 相近但普通控制有效的阴性对照”，比较须含字节限速、页/局部性启发式、固定队列/批量、适用大页/复用。**缺**：正例、阴性对照、参数量化后的强基线收益空间。 |

### 未满足项清单（供根作立题裁决）

1. **页身份输入不存在**：需新接口/新状态，当前策略块无任何页/地址端口（`:96-124`）。是否值得扩接口＝架构问题，归根。
2. **真实场景/地址路径证据缺失**（`performance-council:15`、`platform-feasibility:81`）。
3. **相对 p1..p6/大页/复用/固定队列的定量差异缺失**；新 A 的“增量信息/动作/场景”尚未成文（`platform-feasibility:74,75`）。
4. **新候选的公平判据与停止条件未冻结**（只有旧 method88 的 t170 规则可复用，但那是另一 profile：`platform-feasibility:74` 禁止跨 BIT/内核拼接）。
5. **B 的“有用条件 + 选择规则”尚无定义**，且 172/explore 已看过、非留出（`first-paper-options:68`；`performance-council:71`）。
6. **必须保留“项目未做 ≠ 领域空白”的区分**：A/B 的核心近邻已存在（IOTLB Wall、MASK、rIOMMU、Bermuda、KPart、记忆延迟 CDF 调节、PTW 调度/合并），文档明确不写“首次/发表概率/最坏情况保证”（`first-paper-options:117`）。
7. **p5 结论的适用范围**：只锁死“固定 A8 实占主路线”，不锁死所有反馈/限速方案（`RESULTS:100,102`）。

---

## 7. 不确定性、假设与依赖（供根复核，勿直接当结论）

- **架构级待裁（超出本次授权）**
  1. p4 的 `ev_ptw_a` 用 `ctl_did_i`（`:288`），与 `0xE2E` 同源；`RESULTS:77` 的 stale-DID 警告只针对 `0xE2E`。p4 的 PTW 项是否应继承同一归因不确定性——**未裁定**。
  2. p2 的文档标签“请求”与其真实计费事件 `select_fire`（`:284,323`）是否同义——代码与 RTL 头注释一致（`:80`），但与中文标签存在表达差；需根确认是否影响旧结果解释。
- **未读到的文件/依赖**：`idma.c`、`idma_reg64_frontend.sv`、`dma_composition_control.sv`、`ariane_peripherals_xilinx.sv`、`dma_core_wrap.sv`、`rv_iommu_tl_wrap.sv`、`rv_iommu_trans_ctl_wrap.sv`、`dma_research_observer.sv`、`rv_iommu_iotlb.sv` 本次**未提供原文**；相应行号均为 `platform-feasibility-20261006.md`、`RESULTS.md`、`performance-council-decision-20261005.md` 的转述，未二次核验。
- **未执行**：未编译、未仿真、未上板、未读 raw/hash、未重测；本文件不含任何“测试通过”主张。
- **假设**：mode 编号与 p 编号的对应（p1↔mode1…p6↔mode6、fixedgap 在策略块外）取自 `research-method-next:26` 与 `dma_research_policy.sv:79-84` 交叉一致；若存在别处 mode 重编号，本表需重核。

### 建议的验收检查（供根执行，非本次执行）
- 用 `dma_research_policy.sv:318-331` 单测向量逐个模式断言 `cost_ev`，尤其 mode4 三分量与 mode5 状态限定。
- 断言 `refill_en`（`:344`）在 mode∈{1..5,7} 的**部分/不活跃**窗口仍回填，验证“不会死锁”陈述。
- 断言 `0xE2E`（`:451`）与 p5 计费（`:455`）独立性，确认 0xE2E 不可作分母。
- 若根要立题 A/B：先产出 ①相对 p1..p6 的增量信息/动作/场景文字 ②新公平判据冻结 ③真实或可信工作流/地址路径入口 ④强普通规则的正例/阴性对照设计，再谈实现。

