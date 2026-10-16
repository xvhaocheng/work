# 当前状态（stage 88 · round01 · 2026-10-09）

## 最新结论
- 决定状态：稳定设计 / 条件式路线选择；**不是已成立独立贡献，也不承诺二区**。
- 候选 A（区间补集自耗尽跨 SID 合同）为条件主线；候选 C（动态 split/merge + 有费用驻留）为条件备选；候选 B 已被精确普通 MSHR walker 覆盖，**停止为独立路线，不建模型**。
- A 主手例（归一化服务时间，非 cycle/非实测）：candidate H+24、global-prefix H+30、range-correct whole-source H+25、普通同合同 cursor H+24；完整时刻 candidate 10/14/24 对 whole-source 8/14/25，**旧 V 反而慢 2，必须同时报告**。
- A 无独有系统收益证据（普通 cursor 复现 24）；次强 whole-source 只差 1 单位；CPU 生产/暂存假设未实测。
- B 正例被 C1 完全复现、M2 时 C2 更快 → 无独有系统收益。
- C 静态 7→6 miss/260→240 被普通目的分组精确复现；动态候选**无独有正例**；score 单位纠正为 dimensionless q × hold_cycles。

## 实际执行
- 模型 0；RTL 改动 0；Linux 改动 0；构建 0；上板 0；Claude 调用 0。
- 活动：定向关键源码阅读；经典/近期原始文献与作者代码核查；Codex 机制、证明、手算、成本与条件裁决；DeepSeek 源码证据整理 + 普通文档起草。
- 旧 §76–78 的 6 构建步骤与 29 次实际 COPY PASS **仅引用，不重跑，不算本轮验证**。

## 下一门槛
- A：先固定与对照一致的信任/权限合同（可信 DMA 不重读时先比最简进度方案）；对比普通静态端点分段 cursor、rIOMMU、逐页源完成 + 预装 gate、双缓冲，在字段/端口/软件交互账上找可检验差异。
- C：先给动态覆盖/替换事件下普通 dynamic legalizer + 同信息 pinning 无法复现的手算点，否则 C 模型不启动。
- 过门才跑 M1/M2（A）与 M8/M9（C）；M3–M7 为 A 邻反例/退化/错误场景。

## 关键入口
- 最终权威：`/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-result-acceptance.json`、`root-range-complement-design.json`、`root-cost-and-gates.json`
- 细节卡：`root-primary-review.json`、`root-source-review.json`、`walk-critical.json`、`endpoint-design.json`
- 未采用手推：`lifecycle-redesign.json`
- Proposal 目标：`/home/xvhaocheng/work/iommu-platform/docs/higher-model-mechanism-proposal-20261009.md`

## 归档
- stage87：`/home/xvhaocheng/work/iommu-platform/docs/status-stage87-20261009.md`
