# WO2010117360A1〔正式题名待整理〕

稳定ID：`ref-dba6c2ba49d0` · 类型：`patent` · 主题：`patents`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：patents资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://patents.google.com/patent/WO2010117360A1/en)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/paper-logic.md:337](../../iommu-platform/docs/paper-logic.md)
- [iommu-platform/out/route-cross-review-20261002/codex-round2-input.md:35](../../iommu-platform/out/route-cross-review-20261002/codex-round2-input.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/paper-logic.md:337](../../iommu-platform/docs/paper-logic.md)：

| [WO2010117360A1](https://patents.google.com/patent/WO2010117360A1/en) | 摘要与[0012]–[0013]、[0024]–[0028]：按设备组织pending请求，避免其他设备待处理请求造成阻塞 | “按设备分队列避免阻塞”已有直接IOMMU技术披露；不作完整覆盖或法律判断 |

[iommu-platform/out/route-cross-review-20261002/codex-round2-input.md:35](../../iommu-platform/out/route-cross-review-20261002/codex-round2-input.md)：

6. 更直接的技术先行披露 WO2010117360A1（2010）Iommu with translation request management and methods for managing translation requests。原文摘要及[0003][0007][0008]在原始页面索引可读：某设备不可服务/未命中请求会阻塞其他设备，按设备组织reorder buffer保存pending请求以保持各设备顺序。https://patents.google.com/patent/WO2010117360A1/en ——直接削弱“按设备分队列避免跨设备阻塞”作创新。网页直接open失败，仅核对上述摘要/段落，不声称已读全部权利要求、所有实现或有法律判断。

[返回总目录](../README.md)
