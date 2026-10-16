# §29：翻译返回对照与创新反证

结论与候选取舍见[研究审定](../../docs/translation-lane-and-burst-rule-20261008.md)。新增运行2/PASS2；复用既有二进制，无编译、新源码或板卡动作。G/H未实现，未认证独立创新。

## 本轮真实执行
- [两项执行记录](run-execution.json)，[P命令](priority-p-command.json)、[PN命令](priority-pn-command.json)
- [P日志](priority-p.log)、[PN日志](priority-pn.log)，[比较](comparison.json)
- [P结构化原记录](priority-p-records.json)、[PN结构化原记录](priority-pn-records.json)
- [P原始事件](priority-p-events.csv)、[PN原始事件](priority-pn-events.csv)
- 原FIFO对照是§26已有运行：[P](../read-service-boundary-20261008/serial-p.log)、[PN](../read-service-boundary-20261008/serial-pn.log)，本轮未重复跑。
- 二进制：`../payload-frontier-poc-20261007/obj-ordinary-virtual/Vnonblock_actual_dma_system_top`；SHA256 `963cdc5cc7a23234f9df594d00811a07d16dc684d3cf66022d56082daf06d1c9`。
- [Codex审定与未采信项](codex-review.md)、[阶段清点](stage-evidence.json)、[资料链接/原答复检查](artifact-check.json)。

## 独立设计与外部审查
- [Codex先行草案G](codex-new-rule.md)；后续H定义在[反证任务](challenge-task.md)，均是研究规格，不是已验证算法。
- DeepSeek连接证据：[任务](routing-task.md)、[完整答复](routing-answer.md)、[原始返回](routing-raw.json)，deepseek-flash／thinking/high／max393216，200910 provider tokens，未截断。
- DeepSeek反证：[任务](challenge-task.md)、[完整答复](challenge-answer.md)、[原始返回](challenge-raw.json)，同配置，139842 provider tokens，未截断。
- 两次合计340752；worker没有执行命令、修改源或验证。Codex执行两次仿真、核实际源码/原文、审定和归档；没有Claude或新Codex子agent。
- routing中额外C++ patch未应用，已有ds-priority已足够；challenge的共享PTE/payload信用反例及实现建议未采信。

## 原文
[Codex关键段阅读记录](primary-reading.md)。

[来源/下载结果](papers/sources.json)：VDC2011及Arm DMA350规范两PDF/TXT成功保存；TaPaSCo/H2RC2019下载遇代理503，关键全文段落通过网页PDF阅读。第一次三URL直连被sandbox网络限制，随后获准公开资料下载，两成功/一失败；没有重试代理503。
- [VDC2011全文](papers/vdc2011.pdf)／[提取](papers/vdc2011.txt)，关键TXT1270–1280。
- [Arm DMA350全文](papers/arm-dma350-2023.pdf)／[提取](papers/arm-dma350-2023.txt)，关键TXT4535–4551。
- [TaPaSCo/H2RC2019作者原PDF](https://www.esa.informatik.tu-darmstadt.de/assets/publications/materials/2019/2019_H2RC_MIO.pdf)，§IV-B p4。
- AMD IOMMU与MASK原始来源、TI原专利及未取得正文的检索缺口见研究审定。没有把二手题录当完整机制证据。
