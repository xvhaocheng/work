# E6 历史缓解的有界移植设计

本批只有设计与原始来源证据，尚无生产修改、编译、新BIT或板测。
分支 `ispass/e6-mitigation` 从main `7123bbfe79cca298c36a880206ab85a2d68e6f67` 创建，
本工作树只写本目录。Root审定实际范围后才进入后续组件小批；E4、完整E3优先。

- `design.md`：原版锚、固定容量与服务合同、P4M gap及完整工作代价、最小移植/验收/板点上限。
- `source-map.md`：可导航的实际组件commit、文件行号、历史执行输入与新组织commit的区别。
- `evidence/index.json`：34份只读Git源码/原报告的路径、blob/SHA256、来源角色。
- `evidence/isolated-source-comparison.json`：7份IOMMU关键源码与历史isolated实际构建副本逐字节相同。
- `evidence/historical-identity-rows.json`：原历史身份表的三行完整选摘，保留dirty/overlay与未知项。

历史P/N/PN的负结果、P4M启动混杂/采集失败与跨boot追认均保留。
本批不恢复旧机制探索、普通对照判别门或停止审查，不将任何新设计写成已验证功能。
研究与本批设计由Codex完成；证据中旧报告的DeepSeek等归属仅指当年的历史工作。
