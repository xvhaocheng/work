# 当前状态（stage 88 · round01 · 2026-10-09）

## 结论

- 稳定设计 / 条件式路线选择；不是已成立独立贡献，不承诺二区。
- A 条件主线；C 条件备选；B 停止为独立路线，只作普通 MSHR walker 强对照，不建模型。
- A 手算（归一化服务时间，扣共同 H）：candidate 10/14/24、weak global-prefix 8/14/30、whole-source 保 A 为 8/14/26、先 CPU 为 8/15/25、ordinary 同合同 cursor 10/14/24；旧 V 8→10 变慢已报告。
- A 普通同合同 cursor 复现 24；whole-source 保 A 差 2、先 CPU 差 1 且 A 晚 1；CPU 生产/暂存假设未实测。
- B C0 36/46、候选与 C1 46/26、C2 36/17；邻反例变差。
- C 静态 260→240 被普通目的页分组精确复现；动态候选无独有正例；score 单位 dimensionless q × hold_cycles。

## 实际执行

- 模型 0；RTL 改动 0；Linux 改动 0；构建 0；上板 0；Claude 调用 0。
- 执行：定向关键源码阅读；经典/近期原始文献与作者代码核查；机制、证明、手算、成本与条件裁决；源码证据整理与文档起草。
- 旧 §76–78 的 6 构建步骤与 29 次 COPY PASS 仅引用，不重跑，不算本轮验证。

## 下一门槛

- A：先核同信任/权限合同，对可信 single-use DMA 仅进度、静态 reader 端点分段 cursor、具体可执行 whole-source、逐页源完成+预装门/定向撤销、rIOMMU 多 ring、额外源页双缓冲/bounce-copy 做完整账。
- C：先给普通 dynamic legalizer+相同反馈 bounded pinning、全许可预准备/WRITE 前瞻一页、同 window 软件预切/SG 生成、同资源真实在途/容量无法复现的手算点，否则不启动 C 模型。
- B：不再建模。

## 证据

- 权威：[root-result-acceptance.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-result-acceptance.json)、[root-range-complement-design.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-range-complement-design.json)、[root-cost-and-gates.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-cost-and-gates.json)
- 细节：[walk-critical.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/walk-critical.json)、[endpoint-design.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/endpoint-design.json)
- 未采用手推：[lifecycle-redesign.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/lifecycle-redesign.json)
- Proposal 目标：[higher-model-mechanism-proposal-20261009.md](/home/xvhaocheng/work/iommu-platform/docs/higher-model-mechanism-proposal-20261009.md)
- stage87：[status-stage87-20261009.md](/home/xvhaocheng/work/iommu-platform/docs/status-stage87-20261009.md)
