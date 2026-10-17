# E5 延迟分布能力入口

实际 baseline observer v3 只导出逐阶段 count/sum/max 与边界库存，不能获得逐请求样本、CDF、p50 或 p99。历史 v6 的有限目标记录也不能替代完整分布。

原始源码证据、寄存器容量、可附加到其他运行的现有 mean/max 采集口径，以及尚未实施的最小被动直方图请求，见[完整能力与变更请求](capability-and-change-request.md)。该 RTL 请求等待用户决定。

已从历史统一100次整理出的论文可用阶段数据见[摘要](baseline100/summary.md)、[逐试次阶段 CSV](baseline100/trial-stages.csv)和[完整统计及输入身份](baseline100/summary.json)：100试次、1600阶段行、160组。它们是已有板测数据的离线整理，没有新增 E5 板测。零完成计数的均值和最大值为 N/A；不生成请求分位数。Root 的重算回执见[验证](root-integration-validation.json)。
