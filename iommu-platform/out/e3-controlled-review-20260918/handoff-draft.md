<!-- handoff-section.md -->
**状态**：E3 v6 受控运行已完成上板并通过服务器复核。复核脚本 `server_review_v6.py` 在原始数据上只读运行，结果未改；本轮没有重新编译、没有改 RTL / 驱动 / CLI。此前的失败草稿只是分析脚本的解析与错误汇总缺陷（不是 FPGA 失败），缺陷已修正后再跑一次即通过；mutation 检查共 8 项（1 项正常对照 + 7 项故障）全部符合预期。英文草稿已定稿为两份中文文档：`iommu-platform/docs/e3-controlled-results-review.md`、`iommu-platform/docs/e3-next-method-plan.md`。

**结果**：复核 `status: passed`、exit 0，9 项 checks 全部 `true`，`structural_errors` / `warnings` / `anomalies` 全空；11 对 / 22 轮、418 条唯一 UART 观测行、114 条完全相同重复去重、11 份完整 CLI JSON 与 UART 对象逐字段一致、输入哈希未变。机制结论只限于当前实现 / 配置 / 到达方式，互补样本仅 4 对；cold 上观察到的 `e70−e41=9`、`e42−e41=2`、V 自身 4 周期只是本轮观察值，不是普适常数。原始数据与全部不利结果保留。

**下一步**：按 P4M 实验卡推进——先做“共同 V 硬件窗口的 A 完成量 + 边界在途计数 + A 完整 copy 之后硬件固定间隔”的基线实现卡，接着 gap0 自对照验证、Solo / A1 / A8 短验收，再预试选定少量 gap 点与重复数、冻结正式取舍曲线；gap 范围与重复数尚未确定，V / A 阈值未指定前只画曲线。主证据：原始 run `iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/`；服务器复核 `iommu-platform/out/e3-controlled-review-20260918/server-review.json`、`iommu-platform/out/e3-controlled-review-20260918/review-summary.json`；复核脚本 `iommu-platform/out/e3-controlled-review-20260918/server_review_v6.py`；实验卡 `iommu-platform/out/e3-controlled-review-20260918/e3-next-method-plan.md`。
