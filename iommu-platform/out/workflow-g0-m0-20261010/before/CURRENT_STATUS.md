# 当前状态 stage 95 2026年10月10日全景审查后

- [全景审查报告](iommu-platform/docs/research-panorama-audit-20261010.md)：原方向及多DMA扩展合理；后期逐渐围绕机制延续而变更问题，最新计划只完成部分纠偏。
- 用户本次明确：以有价值的独立研究贡献为核心，允许推翻当前主线，不要求绑定原始Chiplet场景。建议保留问题域的一次有限立题机会，不以已有投入作为继续理由。
- 原计划入口仍为[研究计划.md](研究计划.md)，方法见[研究方法.md](研究方法.md)。本次未修改计划原文；下列为审查建议，尚未启动新性能实验。
- 下一判别题：有动机的工作流中，具体可执行普通控制是否仍留下有用、可归因且可操作的损失。G0允许有界基线先判别是建议，不预设必有失败或必需新机制。
- 与copy关联的状态准入只是候选，不再作为审查建议中的默认主解法；须说明状态对象、时效及它控制哪笔尚未承诺的请求。
- 拟定固定COPY各64次仍是微基准。共同就绪计时、V/A各64笔真实读写及B完成仍待最小适配，不能原样使用旧first V issue至final V done后STOP A的窗口。
- E2/E3、P4M、method88及PN按原范围复用：P4M60份为53在线加7离线、跨启动；method88阴性只否定固定A8/target170下该版p5的额外收益。
- PN板端慢12.67%–12.79%不等于候选收益空间；行为DDR模型不替代实板归因。旧反馈已存在，接口适配、大页、提前准备及模式切换成本不得遗漏。
- 固定A8 p5重试、共享载荷、在途加入、晚选目的及裸walker等旧候选不自动恢复；Sobel保留补充候选，性能缺口未测。原10工作日为管理建议，不是本次启动计时。
- 本次完成关键原文/源码审查、既有记录数值抽核与报告保存；新性能实验、RTL/Linux修改、构建、上板、Claude及新增子agent均0。
- [根裁决](iommu-platform/out/research-panorama-audit-20261010/root-result-acceptance.json)／[原始抽核](iommu-platform/out/research-panorama-audit-20261010/raw-spotcheck.json)／[调用账本](iommu-platform/out/research-panorama-audit-20261010/worker-ledger.json)。
- [stage95审查前原样归档](iommu-platform/docs/status-stage95-before-panorama-audit-20261010.md)。
