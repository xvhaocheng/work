# stage115 新原始来源补查：参数换入错误，未取得所需原生包

2026年10月13日JST。按用户继续指令，完整 stage115 目标保留；实际工具仍为 blocked，没有重新登记或标 complete。本次定向检索后只核一个新入口：[DeepSpeed Discussion 7447](https://github.com/deepspeedai/DeepSpeed/discussions/7447)。没有恢复旧席或关闭分支，也没有实验、部署或源码修改。

作者贴出的配置把 parameter 放在 NVMe、optimizer 放在 CPU；二者都有路径字段，不代表 optimizer 已采用 NVMe。其描述指向 0.17.2 源分支，缺实际版本/commit、启动命令及解析后的运行配置；不能接成冻结 v0.16.4 的 C113 NVMe CPU-Adam 记录。

新原始报告涉及参数换入：主配置 B=200、每缓冲4,194,304元素，作者报197个swap文件，最大参数155,582,464元素超出单缓冲；另一个配置报5个缓冲不足以换入197项。这是作者报告的准备阶段错误，文件清单和完整日志未给出。按其 FP16 描述，主配置缓冲名义总容量1,677,721,600字节；并非成功分配、pinned占用或成功搬运字节。两种配置不拼成同次运行。

这份来源只增加参数尺寸/数量边界；拒绝作为目标原生证据包。没有实际存储到PCI/IOMMU的翻译路径、共同单元和自然peer，也没有数值校验、成功bytes/DONE/quiet/释放、普通强对照完整成本及翻译残余。作者声称的大参数驻留修改未在本机应用，也不作性能证据。

C113条件保留、当前FPGA不准入G0/G1，未知不作科学阴性；未新增GRPO或其他研究工作流。完整获取目标与论文均未完成。本入口已收束，不继续扫issue或重读已核来源。

[原HTML及hash](source-receipt.json)／[派生正文](raw/discussion-7447-markdown-bodies.txt)／[root六组亲核原文](root-critical-review.json)／[稳定入口裁决](entry-result.json)／[有界检索账本](lookup-ledger.json)／[完整目标的实际状态](goal-before-source-review.json)／[记录核验](delivery-check.json)。
