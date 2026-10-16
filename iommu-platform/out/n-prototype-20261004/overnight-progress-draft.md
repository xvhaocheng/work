下面是我从授权证据里提取、按你要求起草的完整中文 MD（建议落盘 `docs/research-overnight-progress-20261004.md`），以及聊天框用简明表格。**本稿为草案；实际构建/板测运行与最终审阅归属 Codex。**

---

# N 原型线 昨夜进展报告（草案快照）

- 快照日期：2026-10-04，只到所提供 JSON/MD 字段为止
- 来源：`CURRENT_STATUS.md`、`out/n-prototype-20261004/handoff-for-overnight-report.md`、各阶段 `summary.json`/`boot.json`/`*-build-acceptance.json`/`v*-work-state.json`/`v5-host-acceptance.json`、`analysis-v4-pilot/RESULTS_REVIEW.md`
- 时间口径：原文多为 UTC；本表同时给出日本时间 JST = UTC + 9
- **用户最新停止要求**：完成当前 v5 两套板测与结果分析、必要交接更新后停止；不自动启动下一轮实验、代码或编译（`CURRENT_STATUS.md:3`、`v5-build-state.json:37`）

## 一句话结论

昨夜到现在的 N 原型线已推进到 **v5**：原版 15 项基线、v3 S/N 各 15、v4 S/N 各 15 都已在板上跑完并通过数据/协议验收；但 **v3、v4 的 fast 候选/接收计数始终是 0**，“快通路真正被走到”仍未观察到。v5 主机侧资格逻辑与控制器清理测试通过，S 版已恢复出可接受构建并**刚启动**板上 15 项，**v5 目前没有板测结果**，N 版仍在编译。

## 阶段表（实际做了什么 / 完成多少 / 结果 / 意义）

| 阶段 | 实际动作 | 已完成数量 | 结果 | 意义 |
|---|---|---|---|---|
| original-03（基线） | 原版 BIT SSH/JTAG 启动，跑冻结 15 项 | 15/15 completed、exit0、problems 空、taint0 | `nonblock-pilot-20261004-original-03/summary.json:214-229`，boot `:1-19` | 本线可复用的原版对照；不重跑 |
| 说明 | 原版为**复用已有 flash 启动**，仅一次人工 Ctrl-C 解除启动冻结，非重烧/重跑 | 0 重跑 | `original-03/boot.json:5-15`、`summary.json:222-223` | 记录工程受限点，避免当作干净新启动 |
| v0 | 首次上板尝试 | 串行版仅短 Solo/A1 exit0 且数据正确，短 A8 未正常返回，剩余 12 未开始；接收计数全 0 | `v1-work-state.json:4`、`CURRENT_STATUS.md:1002`、`:1004` | 功能未通过；根因是计数信号作用域（源码 931-933 位于 gen_dma 内，1381-1382 产生 implicit wire） |
| v1（读守卫版） | 主机模型 + 两版 BIT 编译 | 编译/时序 exit0；banked 轮询交错重叠模型 8/12 失败（legacy 12/12 通过） | `v1-work-state.json:2`、`:8-13`、`:25-31` | **功能拒绝，未上板**；模型暴露 V 第二段 R 排在共享返回队首、cfifo 满、AW 未接收的有限等待环 |
| v2（整份 4KiB buffer 版） | 主机模型 + 两版 BIT 编译 | banked/legacy depth512 各 12/12 通过 | `v2-work-state.json:3-6` | 模型通过但 **FPGA 布局超限**：combined LUT S207069/N207226 > 203800，Place 失败，两版 exit1（`v2-work-state.json:28-39`） |
| v3（局部 RAM 版） | 复用既有 AsyncDpRam 存储分支，主机验证 + 两版 BIT + 板测 | 板测 S 15/15、N 15/15 完成，exit0、problems 空、taint0 | 构建 `serial-v3-build-acceptance.json:1-27`、`hit-v3-build-acceptance.json:1-25`；板测 `serial-v3-01/summary.json:469-482`、`hit-v3-01/summary.json:469-483` | 数据/协议通过；**但 N 版 fast V/A 接受计数全 0**，未验到快通路被服务（`v3-work-state.json:25`） |
| v4（flat MSI 兼容 + 被动诊断） | 只读证据确认 DID10/11 为 flat MSI mode1；放宽普通 4KiB 输入/缓存物理地址判定；新增被动诊断 bank；两版 BIT + 板测 | S 15/15、N 15/15，exit0、problems 空、taint0；诊断 bank 有效 | `serial-v4-build-acceptance.json:1-25`、`hit-v4-build-acceptance.json:1-25`；板测 `serial-v4-01/summary.json:469-482`、`hit-v4-01/summary.json:469-483` | 消除一处已确认静态兼容限制；**candidate/probe/DC/LEAF/fast 仍全 0**（`analysis-v4-pilot/RESULTS_REVIEW.md:134-153`） |
| v5（限定 4KiB job 资格 + 仅 DID10/11 读入口放行） | 新增 `idma_fullbuffer_certificate`；主机单项/联合/控制器清理测试；两版 FPGA 编译 | 主机：certificate unit exit0、clean 重建 11/11、qualified/guarded 各 12/12（`v5-host-acceptance.json:1-34`）；S 构建 recovery exit0，N 构建中 | `v5-build-state.json:3`、`:38-39`；`serial-v5-build-acceptance.json:1-27` | 资格模型 A8 观测到 fire78，但模型总周期**未见改善**（`v5-build-state.json:11-15`）；S 板测**刚启动、无结果** |

## “实际板测” 与 “主机/仿真/编译” 必须分开看

- **实际板测**：original-03、v3 S/N、v4 S/N 各 15 项，共 **75 项**在板上完成并通过数据/协议验收。原版 15 项是**复用基线**，在三 label 分析中不重复累计（`analysis-v4-pilot/RESULTS_REVIEW.md:27-40`）。v0 部分、v1、v2 未上板；v5 刚启动。
- **主机 RTL/模型验证**：v1 轮询交错重叠 36 场景、v2 depth512 轮询 12/12、v3 FIFO 9/9、v4 MSI 7/10/8/8 + 21/21 + 12/12、v5 certificate unit + clean 重建 11/11 + proxy 12/12/12 均属此层，不能当板测（`v1-work-state.json`、`v2-work-state.json:3-6`、`fifo-ram-v3-acceptance.json:1-13`、`v4-host-acceptance.json:1-26`、`v5-host-acceptance.json:1-34`）。
- **FPGA 构建/时序**：v1 两版 exit0；v2 两版 exit1（LUT 超限）；v3 S 初版 exit1（BIT 完成后 Vivado 段错误，复用产物补 MCS，recovery exit0），N exit0；v4 两版 exit0；v5 S recovery exit0、N 仍在构建。构建通过只证明“能烧”，不等于板测通过或加速。

## 白话解释

- **A 干扰路 / V 重点观察路**：A 是故意制造的背景流量（干扰方），V 是真正关心完成时间的被观测方。
- **缓存命中**：翻译结果已在 IOTLB，不需走页表遍历（PTW）。命中本应快，但共享翻译控制器一次只处理一请求时，V 的命中也可能被 A 的 miss 压住。
- **共享阻塞**：A 与 V 共用翻译控制器/返回路径；A 占住处理状态时 V 只能等。
- **快通路**：非阻塞设计里“命中可越过 miss”的路径。v3/v4 接受计数全 0 = 本冻结工作量下这条路从未真正被触发。
- **4KiB buffer**：每路 DMA 存一份完整 4KiB 拷贝的缓冲，避免读写交错打断。这是已有工程做法（`research-literature-review-20261004.md:89`），本线不当作新东西。
- **验证通过 ≠ 加速**：模型通过、编译 exit0、时序 WNS 为正、板测 15/15 exit0/taint0，这些只说明“功能/数据正确”或“能烧进板”；v3/v4 fast 全 0、v5 模型总周期未见改善，就是“通过但没加速证据”的直接例子。

## 尚未证明

- **N 是否带来任何性能收益**：没有；v3/v4 板测 fast 全 0，v5 模型对旧守卫 94003/127222 vs 76451/109751 未见改善（`v5-build-state.json:11-15`）。
- **fast 全 0 的根因**：诊断为“本冻结工作量未提供合格候选”（科学未行使），**不是技术失败**；不能据此说“无机会”，也不能说守卫是唯一原因（`analysis-v4-pilot/RESULTS_REVIEW.md:142-153`、`:169-177`）。
- **v5 是否改变该结论**：未知；v5 板测刚启动，无结果。
- **S 与 N 差异能否归因 enable 本身**：不能；两版是不同 BIT、不同布局/面积/时序，只作条件性描述比较（`analysis-v4-pilot/RESULTS_REVIEW.md:37`）。
- **创新性**：未成立；本线是前人已知 hit-under-miss / full-transfer-buffer 的工程对照，不主张 novelty、不主张百分比节省（`nonblock-v5-qualified-read-plan-20261004.md:4`、`research-literature-review-20261004.md:42`）。

## 时间与边界

可核查时间戳（UTC → JST）：

- original-03 完成 2026-10-03T21:07:14Z → 06:07:14 JST（`original-03/summary.json:216`）
- serial-01 停止 21:40:28Z → 06:40:28 JST（`serial-01/summary.json:5`）
- serial-02 停止 21:44:25Z → 06:44:25 JST（`serial-02/summary.json:5`）
- serial-03 boot 2026-10-03T21:52:26Z → 06:52:26 JST（`serial-03/boot.json:4`；**无 summary.json**，不补造完整次数）
- v3 S 完成 2026-10-04T02:30:08Z → 11:30:08 JST（`serial-v3-01/summary.json:471`）
- v3 N 完成 02:42:37Z → 11:42:37 JST（`hit-v3-01/summary.json:471`）
- v4 S 完成 04:32:27Z → 13:32:27 JST（`serial-v4-01/summary.json:471`）
- v4 N 完成 04:45:01Z → 13:45:01 JST（`hit-v4-01/summary.json:471`）
- v5 S 构建验收 2026-10-04T06:46:23.720709+00:00 → 15:46:23 JST（`serial-v5-build-acceptance.json:22`）

口语的“昨夜1点”无精确证据锚定，本报告不补造该时间；以上是**有原始 JSON 可核查**的时间点。快照仅到所提供字段为止，v5 板测及更晚状态由父级拿到实际结果后补。

## 已写的 md 与当前项

- 相关已写/已更新 md：
  - `docs/research-n-prototype-spec-20261004.md`
  - `docs/nonblock-pilot-plan-20261004.md`
  - `docs/nonblock-dependency-diagnosis-20261004.md`
  - `docs/nonblock-v5-qualified-read-plan-20261004.md`
  - `out/n-prototype-20261004/analysis-v4-pilot/RESULTS.md`、`RESULTS_REVIEW.md`
  - 本文件 `docs/research-overnight-progress-20261004.md`
- **当前进行项**：v5 S 板测刚经 SSH/JTAG 启动、暂无 v5 结果；v5 N FPGA 构建仍在运行（`v5-build-state.json:3`）。按用户停止要求，完成 v5 两套板测与结果分析后停止，不启动下一轮（`CURRENT_STATUS.md:3`、`v5-build-state.json:37`）。

*归属：本报告为草案；阶段实现、RTL 与测试/构建的执行归属见各 `*-build-acceptance.json`/`v*-work-state.json`/`v5-host-acceptance.json` 的 attribution 字段（DeepSeek 起草，Codex 关键 RTL 与执行审阅）。*

---

## 聊天框简明表格答复

| 问题 | 答复 |
|---|---|
| 写的 md 在哪 | 建议 `docs/research-overnight-progress-20261004.md`（本草案） |
| **实际板测**（可用原始 JSON 复核） | original-03 15/15；v3 S 15/15、v3 N 15/15；v4 S 15/15、v4 N 15/15 → 共 **75 项**通过数据/协议验收 |
| 原版是否反复算 | 否；original-03 是**复用基线**，三 label 分析里不重复累计（`analysis-v4-pilot/RESULTS_REVIEW.md:27-40`） |
| v0–v2 | v0 观测接线错误未通过；v1 RR 耦合死锁**模型**被拒、**未上板**；v2 整 4KiB buffer **模型**通过但 **FPGA LUT 超限**（S207069/N207226 > 203800，两版 exit1） |
| v3 | 局部 RAM 版，两版板测 15/15，但 **N fast 计数全 0** |
| v4 | flat MSI 兼容 + 被动诊断，两版板测 15/15，**candidate/probe/DC/LEAF/fast 仍全 0** |
| v5 主机 | certificate unit exit0；clean 重建 11/11；qualified/guarded RR 各 12/12；A8 模型 fire78，**总周期未见改善** |
| v5 构建 | 初次 scope 错误、模块清单遗漏已修；**S recovery exit0，WNS 0.177 / WHS 0.035 / LUT 174413 / FF 138384**；**N 仍在构建** |
| v5 板测 | **S 刚经 SSH 启动，暂未拿到结果**；不把“正在执行”当完成 |
| 失败/未运行 | serial-01 0/15、serial-02 0/15、serial-03 仅 boot 无 summary（短 Solo/A1 通过、短 A8 未正常返回，余 12 未开始）——**保留，不补造次数** |
| 尚未证明 | N 性能收益；fast 全 0 根因；v5 是否改变结论；S/N 纯 enable 因果；创新性 |
| 最新要求 | 完成 v5 两套板测 + 分析文档后**停**，不启动下轮代码/构建 |

**时间换算说明**：口语“昨夜1点”无法精确锚定，不补造；表中列出的可核查 UTC 时间已换算为 JST（UTC+9），并明确 serial-01/02/03 与 v3/v4 各段的完成/停止时间。

**建议验收检查**（供 Codex）：
1. `summary.json:469-482` / `:469-483` 各 label `completed=15, failed=0, status=complete`；
2. `boot.json` 的 `uname_r` 均为 `6.5.0-rc2-00093-gc0f5e13ea81f`、`taint=0`；
3. `serial-03/` 目录无 `summary.json`，不得写成完成；
4. v5 `serial-v5-build-acceptance.json:22` 的 `actual_acceptance_utc` 与 `recovery_exit=0` 一致；
5. `v5-build-state.json:3` 的 stage 仍为 `S_build_accepted_N_build_running`，即 v5 无板测结果。

**未做事**：无新研究方案、无新模型测试、无普通检查、无重新跑、无新编译触发；未改变 v1–v5 既有失败/未运行记录。