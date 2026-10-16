# 总表：限速/准入实验完成度核清（只读，基于所给文件，不执行/不设计新试验）

**总结论**：你上一轮若只依据 composition-172 就断言"没有后台限速曲线、没有反馈控制"，确有遗漏。项目已经完成两类实验：(a) P4M 固定 gap 限速（short 4 例 → pilot-21 → formal60 部分）；(b) method88 逐任务"自适应准入"7 方法（fixedgap + p1..p6，88/88 上板）。formal60 未完成：只有 24 在线 + 1 离线追认，剩 35 为 `prepared_not_run`。以下带 file:line。

---

## (1) P4M 完成度（short / pilot / formal60）

| 阶段 | 实际完成 | 状态定性 | 证据 |
|---|---|---|---|
| P4M short | 4 例功能通过：solo / a1-gap0 / a8-gap0 / a8-gap1000 | **已完成**；仅功能验收，n=1 不证收益 | `docs/p4m-short-results-review.md:3,9-12,27,46` |
| short 内的 P4M-2 显式对照 | legacy `--batch` vs `--p4m` gap0 **未做** | 缺口（当时） | `docs/p4m-short-results-review.md:28,39` |
| pilot-21 计划 | 当时**只确定计划，未启动预试** | 该文档时点未跑 | `docs/p4m-short-results-review.md:48-68`（`:68` 明写未启动） |
| P4M pilot-21 | **已完成**，21/21 功能通过，服务器已复核 | 完成（n=3 工程重复） | `docs/p4m-pilot-results-review.md:3,7` |
| pilot 内 P4M-2 对照 | **已执行**，但受启动相位/预窗口历史混淆，**结论未定** | 执行完成、结论 inconclusive | `docs/p4m-pilot-results-review.md:3,27-33` |
| formal60 原始在线 | 旧 runner **24 在线通过**；第 25 次 formal-b09-g2000 因 `startup_line_count` 36->36 检查 exit5；**后 35 未跑** | 未完成 | `docs/p4m-formal60-recovery.md:6` |
| formal60 第 25 次 | **离线复核追认**保留（不伪称 25 在线通过） | 1 离线追认 | `docs/p4m-formal60-recovery.md:7` |
| formal60 resume35 | 修复 + 主机验证已完成；35 次**尚未上板**，状态 `prepared_not_run`；Dell 未接板 | **未完成** | `docs/p4m-formal60-recovery.md:8,45,55,90` |

**明确回答**：formal60 现状 = **24 online 通过 + 1 离线追认 + 35 待跑（prepared_not_run）**，与你质疑一致。采集修复/主机验证是"已就绪"，不等于板端已跑；`docs/p4m-formal60-recovery.md:55` 亦明说"脚本 35 完成不等于论文 60 正式已审结"。**不得把 prepared 外推为已完成。**

版本/口径（按要求区分）：short 为 raw BOOT Linux 00088（`p4m-short-results-review.md:34`）；formal60 使用 kernel 00089 / P4M v7（`p4m-formal60-recovery.md:44`）；method88 使用 kernel 00093（见下）。pilot 所用内核在本批文本中未给出 → **缺证据，不臆断**。

---

## (2) method88：fixedgap / p1..p6 控制依据、是否自适应、是否真实上板

方法清单（`docs/research-method-next-20261004.md:26`）：

| 方法 | 控制依据（按命名/文件） | 是否"自适应准入" | 依据 |
|---|---|---|---|
| `fixedgap` | 固定 gap，完成驱动节奏（完成一项再等固定周期） | 否，属固定限速基线/参照 | `docs/research-method-next-20261004.md:26,44`；`docs/platform-feasibility-20261006.md:12,24` |
| `p1` | **按字节** | 是（准入反馈） | `docs/research-method-next-20261004.md:26` |
| `p2` | **按请求** | 是 | 同上 |
| `p3` | **按 miss** | 是 | 同上 |
| `p4` | **事件加权** | 是 | 同上 |
| `p5` | **实占**（`ctl_did==DID_A` 且 `REQ_TRANS`/`AWAIT_RESP` 周期计费） | 是（近似"按成本"） | `docs/research-method-next-20261004.md:26`；`out/method88-execution-20261004/RESULTS.md:78` |
| `p6` | **仅 V 反馈** | 是 | `docs/research-method-next-20261004.md:26` |

**PTW 专项**：无独立"按 PTW"方法；`p3` 是 miss 维度，`p5` 的计费区间**覆盖** DDT/PDT/PTW 翻译响应（`out/method88-execution-20261004/RESULTS.md:78`）。故"按 PTW 的自适应准入"在本批中并非独立项，勿混称。

**是否都真实上板**：是。同一 role2 BIT 两次启动（每次 44 项，共 88 项）真实板端运行，`/usr/bin/idma_research`（`docs/mac-method88.md:12,120-125,189-191`）。功能结果 **88/88 host_exit0，失败0/缺失0**（`out/method88-execution-20261004/RESULTS.md:3,17`；`out/method88-analysis-20261004-01/report.zh.txt:12-14`）。boot1 复用既有启动（含既往 462 项活动），boot2 为新启动，**分别报告不合并**（`report.zh.txt:17-19`）。

**强对照阴性结论四格**（`out/method88-execution-20261004/RESULTS.md:29-34`；对应 `report.zh.txt:22-27,30-37,38-45,46-53`）：

| 启动 | 方向 | 强基线 | p5 相对增益 | 保护匹配 | 通过 |
|---|---|---|---:|---|---|
| boot 1 | forward | p4 | −5.68952% | true | false |
| boot 1 | reverse | p4 | −8.23689% | true | false |
| boot 2 | forward | p1 | −5.15142% | true | false |
| boot 2 | reverse | p2 | −4.43680% | true | false |

方向 0/1 两启动均 False，**跨方向 False**（`report.zh.txt:56-58`；`out/method88-execution-20261004/RESULTS.md:38`）。补充参考：p5 vs fixedgap 有正增益但两格保护未匹配（`RESULTS.md:44-47`）；p5 vs p6 反向两启动复现（+34.68%/+30.99%），但**不构成实占计费独有收益证明**（`RESULTS.md:21,49,51,57`）。据预定条件**结束固定 A8 实占主创新路线**，p5 转为已测候选/基线（`RESULTS.md:20,100`）。

---

## (3) 已做的限速/反馈实验 vs 新候选 A（页工作集感知）

**已做（足够说明）**：
- 固定 gap 限速曲线：P4M short gap0/1000（`docs/p4m-short-results-review.md:12`）；pilot gap0/500/1000/2000 + legacy batch（`docs/p4m-pilot-results-review.md:31`；计划见 `p4m-short-results-review.md:52`）；formal60 gap0/1000/2000 各 20（`docs/p4m-formal60-recovery.md:5,61-65`）。
- 逐任务自适应准入：method88 的 p1..p6 已上板 88/88（`docs/research-method-next-20261004.md:26`；`out/method88-execution-20261004/RESULTS.md:17`），其中 p5 实占计费为最强候选。

**composition-172 的真实边界**：该批**A gap 全为零**，**没有 A gap 后台限速曲线 / 页感知控制 / 阶段适应**——这是"本批"的结论，**不能外推为全项目**（`docs/platform-feasibility-20261006.md:49`）。这正是上一轮的遗漏点：172 只是 A gap=0 的边界实验，不是全项目限速证据。

**新候选 A（页工作集感知）与旧 M（p5 实占等）必须明示的差异**（防止换名重做）：
- 旧 M（p5 等）：**运行中**、**逐任务**、基于翻译占用/事件/miss 的**反馈准入**（`docs/research-method-next-20261004.md:26`；`out/method88-execution-20261004/RESULTS.md:78`）；**不使用页工作集信息**，V/A 工作集在该批固定（`docs/platform-feasibility-20261006.md:49`）。
- 新 A：**批次/页工作集感知**，驱动已知源/目的 IOVA 与页数，**预先选本批提交节奏**，维护明确成本与特征定义；是**粗粒度原型，尚非逐任务反馈**（`docs/platform-feasibility-20261006.md:5,13,71`）。
- 硬边界：**静态已知页数不能冒充在线推断**；页身份/访问顺序/阶段切换需新供给接口，当前静态顺序环不覆盖（`docs/platform-feasibility-20261006.md:70,72`）。
- 故 A 的新颖性只能在"页工作集感知的批次选择"这一差异上成立，**不能在 p5/逐任务反馈已被 88/88 四格证否后换名重报**。

---

## (4) `docs/platform-feasibility-20261006.md` 必要的有限文本纠正

**保留为真实结论（不改）**：
- 172/172 完成、data_errors/taint 为零（`:35`）。
- 本批 composition/explore **A gap 全为零，无 A gap 扫描**（`:49`）。
- 现 profile `research_role=0`，**旧 policy 被禁用**；ADMIT 钩子存在但当前 BIT 未启用（`:14,26` 第 5 点）。
- 资源/时序：Slice LUT 81.22%、占用 Slice 99.60%、WNS +0.031ns，不能承诺"还剩约 19% LUT"（`:57-63`）。

**建议的有限文本纠正（纠正"全项目从零起步"暗示）**：
1. `:5` 判语"平台足以**启动**范围收敛的机制研究"与 `:12-14` 表格易读成从零起步。建议加一句限定：**固定 gap 限速（P4M short/pilot/formal60）与逐任务自适应准入（method88 p1..p6，88/88 上板）已在本平台完成**，A 是**新增批次级页工作集维度**，不是首次反馈控制。依据：`docs/p4m-short-results-review.md:3,9-12`、`docs/p4m-pilot-results-review.md:3,7`、`docs/p4m-formal60-recovery.md:5`、`docs/research-method-next-20261004.md:26`、`out/method88-execution-20261004/RESULTS.md:17`。
2. `:49` "没有后台限速曲线"应**限定作用域**为"本轮复算的 composition-ptwfix/explore 样本（A gap=0）范围内"，并交叉引用 P4M/formal60 gap 曲线；否则与 `p4m-pilot-results-review.md:31`、`p4m-formal60-recovery.md:5` 冲突。
3. `:13` "尚非逐任务反馈控制"应更正为：**逐任务反馈准入（p1..p6）已存在并上板**；本文件所指 A 的新增点在于**页工作集感知的批次级选择**（`docs/research-method-next-20261004.md:26`；`docs/platform-feasibility-20261006.md:70`）。
4. `:14` "逐任务自适应提交 A … 需重新验证"可保留，但需注明其与已测 p1..p6 的关系：新 profile 接入的是**页工作集驱动**的准入，而非重跑旧反馈。
5. `:49` "页感知控制"确实缺失——**保留**该否定，作为 A 的落点依据。

---

## 证据位置与缺口（便于 Codex 核验）

- 最强证据：`out/method88-execution-20261004/RESULTS.md:29-34`（四格）、`:17`（88/88）、`:78`（p5 计费口径）；`out/method88-analysis-20261004-01/report.zh.txt:22-58`（逐格增益与跨方向）。
- P4M：`docs/p4m-short-results-review.md:3,9-12,28,68`；`docs/p4m-pilot-results-review.md:3,7,27-33`；`docs/p4m-formal60-recovery.md:6-8,45,55,90`。
- **区分口径**：运行重复（method88 每 boot/方向 n=3；explore 同 boot 三 block，`platform-feasibility:37`）；boot（method88 boot1 复用 + boot2 新启动，`report.zh.txt:17-19`；P4M 单 run/boot）；版本（short 00088、formal60 00089、method88 00093；pilot 内核未给出）。
- **缺口/不确定性**：formal60 resume35 板端证据**尚不存在**（`prepared_not_run`）；method88 aux 目录无逐行锚点（`RESULTS.md:63` 自述）；pilot 内核版本与 P4M short 与 formal60 是否同 boot/同版本未在给定文件中闭合。

（说明：以上仅为对所给文件的只读整理，未执行任何命令或测试；未声称任何未由文件确证的通过结论。）

