# Torrent: A Distributed DMA for Efficient and Flexible Point-to-Multipoint Data Movement，2025〔正式题名待整理〕

稳定ID：`arxiv-2512.17589` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/html/2512.17589v1)
- [来源2](https://arxiv.org/html/2512.17589v1#S3)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-morning-20261009.md:31](../../iommu-platform/docs/research-morning-20261009.md)
- [iommu-platform/out/innovation-rescue-20261009/codex-cross-field-boundary.md:9](../../iommu-platform/out/innovation-rescue-20261009/codex-cross-field-boundary.md)
- [iommu-platform/out/innovation-rescue-20261009/codex-primary-review.md:13](../../iommu-platform/out/innovation-rescue-20261009/codex-primary-review.md)
- [iommu-platform/out/innovation-rescue-20261009/rules-design.md:171](../../iommu-platform/out/innovation-rescue-20261009/rules-design.md)
- [iommu-platform/out/innovation-rescue-20261009/rules-design.md:177](../../iommu-platform/out/innovation-rescue-20261009/rules-design.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-morning-20261009.md:31](../../iommu-platform/docs/research-morning-20261009.md)：

[Torrent](https://arxiv.org/html/2512.17589v1)有grant/data/finish阶段；[AXI multicast crossbar](https://arxiv.org/html/2502.19215v1)处理AW/W/B、多播前排空相关单播。已读正文没有给出本原型完整的独立权限、长READ、logical LAST、旧W/B义务切换合同；这是核查边界，不是证明所有已有研究都没有。

[iommu-platform/out/innovation-rescue-20261009/rules-design.md:171](../../iommu-platform/out/innovation-rescue-20261009/rules-design.md)：

| [Torrent: A Distributed DMA for Efficient and Flexible Point-to-Multipoint Data Movement，2025](https://arxiv.org/html/2512.17589v1#S3) | III-A/III-C，L129–143：configuration、后向grant、transfer、finish；endpoint数据switch复制/转发，以链式P2P实现P2MP。 | 同source少一次read、显式P2MP及无大临时payloadbuffer的streaming是强前作。不能把endpoint广播、ready grant或source复用当新增点。应允许同信息ordinary chain/broadcast，计其route/endpoint/stream接口。 |

[iommu-platform/out/innovation-rescue-20261009/codex-primary-review.md:13](../../iommu-platform/out/innovation-rescue-20261009/codex-primary-review.md)：

[Torrent: A Distributed DMA for Efficient and Flexible Point-to-Multipoint Data Movement v1](https://arxiv.org/html/2512.17589v1)，III-A/C（129–143）：独立配置链、反向Grant、Data与Finish；中间节点store/forward，data switch on-the-fly复制而不额外存payload。已有与AXI兼容的endpoint multicast，不能把read-once、DMA自带复制或不改NoC当独占贡献。

[iommu-platform/out/innovation-rescue-20261009/codex-cross-field-boundary.md:9](../../iommu-platform/out/innovation-rescue-20261009/codex-cross-field-boundary.md)：

- [Torrent](https://arxiv.org/html/2512.17589v1) III-A/C line129–142：cfg/grant/data/finish，端点分流。Endpoint fork、无整段暂存均不是本工作发明。

[返回总目录](../README.md)
