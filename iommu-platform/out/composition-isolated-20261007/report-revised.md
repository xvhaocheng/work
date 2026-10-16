# Composition history isolation：2026-10-07 对比与验收

日期：2026-10-07。范围：整理 `out/composition-isolated-20261007/` 的对比、验收、原始记录、汇总与生产补丁，并对照 `docs/novelty-gate-20261006.md` 研究门。本文只整理已给出的结果与边界。

## 1. 生产改动与范围

生产 RTL 已有改动，集中在 IOTLB 与 TL 包装层：新增分区局部的 PLRU touch，固定 ways0/1 与 ways2..15；touch 归属取常驻命中路，不用 `policy_mask_i`。全掩码 S/N 逻辑不变。Linux/SD 未更新。

启动与加载：Linux 从 SD 启动；JTAG 只加载 BIT；以太网用于 SSH 控制与身份识别，不承担网络启动。新启动的 boot_id 为 `d61e7517-2b15-4702-86f2-3f338ddd5345`，内核仍为 cpn3-clean 同版本，启动过程有变化。

定义：P 为容量分区，N 为命中旁路，合称 PN；W 为 V 的测量窗口周期数，时钟 50 MHz。

来源：[production.patch](../out/composition-isolated-20261007/production.patch)、[board-acceptance.json](../out/composition-isolated-20261007/board-acceptance.json)。

## 2. 构建、测试与板级验收

原生构建 WNS -0.219 ns，一项端点未过；沿用已有的一次布线后优化后，最终 WNS +0.022 ns、WHS +0.051 ns，所有门级检查失败数为 0。资源 LUT 156186、FF 127714、Slice 50556。

原有测试 25834 项通过，TL 相关测试通过；新增隔离测试 9232 项通过。板级 22/22 通过，其中 4 个短例、18 个性能例，failed=0；板级验收由 Codex 按 `composition_plan.validate_case`（含 meta）逐例执行并验收。原始记录汇总见 [summary.json](../out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/summary.json)。

## 3. A8 gap0 对比（旧 clean 对新 isolated）

核心结果如下。旧侧来自旧 clean，新侧来自 isolated 实际原始记录。W 以 50 MHz 计，新 P 的 W 约 703 万周期，约 140.7 ms；新 PN 的 W 约 793 万周期，约 158.6 ms，额外约 18 ms。

| 组 | 旧 A 缺失数 | 新 A 缺失数 | 旧 W 范围 | 新 W 范围 | PN 相对 P 放慢幅度 |
|---|---:|---:|---:|---:|---:|
| P | 5117 | 2048 | 8960484–8994909 | 7032355–7039655 | — |
| PN | 6582 | 2048 | 10001202–10006893 | 7931144–7931945 | 旧 +11.2200%–+11.6355%；新 +12.6744%–+12.7922% |

旧 A 缺失数差为 +1465，新双方均为 2048，差为 0。新 P 的 V 普通请求 16384、V 快速请求 0、V 缺失 0；新 PN 的 V 普通请求 12288、V 快速请求 4096、V 缺失 0，总 V 请求均为 16384。需要明确：A 缺失数相同只表示数量相同，不表示访问页相同、请求顺序相同、PTW 开销相同或请求时间线相同。新 A 的普通查找在 16385/16386 相差 1 或 2；A 窗口完成数均为 4096，但窗口长度不同，因此不能写成吞吐相同或速度不变。

样本量 n=3，仅作描述性对比；跨 BIT、跨启动的前后比较仍有混杂，S/N 对照变化在 0.28% 内。

来源：[comparison.json](../out/composition-isolated-20261007/comparison.json)；原始记录例：[b1 a8 mp record](../out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mp/record.json)、[b1 a8 mpn record](../out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/cases/explore-b1-g0-a8-mpn/record.json)。

## 4. 解释边界

隔离使 PN 相对 P 的额外 A 缺失数差从 +1465 降到 0，并改善双方绝对 W：P 下降约 21.44% 至 21.82%，PN 下降约 20.69% 至 20.74%。剩余 PN 放慢不能只用缺失数差解释。这里不做显著性主张，也不写“隔离失败”或“回归已修复”。历史 PTW 修正在对比中仅见 `[11.58, 12.22]`，不作因果解读。

未完成项：整板 IOC/DC 完整签核尚未完成；这里只作为覆盖范围限制说明，不作为异常告警。剩余 PN 放慢的因果归因仍未完成，等缺失数下约 0.90M 周期差额来源尚未拆开。

## 5. 下一步最小任务

取合法实际 DMA/TL 事件序列，解释等缺失数下新增约 0.90M 周期：区分本实现侧额外气泡、仲裁与响应缓冲，与真实服务顺序、不同缺失、PTW 开销、数据路径竞争。先有精确原因，再讨论机制、工作流或新意；没有先验一般机制或新意主张，也不复活 p5 固定 A8。主论文方向仍是关键 DMA QoS 与后台吞吐，不是自动安全结论，也不是 PLRU 贡献论文。

研究门见 [novelty-gate-20261006.md](novelty-gate-20261006.md)；脚本引用见 [composition_plan.py](../scripts/composition_plan.py)。

## 6. 来源与历史口径

旧侧数据来自旧 clean 172 次启动中按相同参数筛出的 18 例，保留原 ID 与顺序；新侧数据来自 isolated 实际原始记录，位于 `out/composition-isolated-20261007/mac-records/composition-isolated-20261007-formal01/`。comparison、acceptance 与 summary 均从这些原始记录汇总，生产补丁单独列出。板级验收状态为 passed、count=22、failed=0，检查项为 `composition_plan.validate_case`（含 meta）逐例通过，详见 [board-acceptance.json](../out/composition-isolated-20261007/board-acceptance.json)。

作者与边界：DeepSeek 起草提案与文档；Codex 复核关键 RTL、应用改动、执行测试、完成构建与上板验收。
