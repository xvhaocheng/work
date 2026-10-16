# LR1证据与结论范围更正

LR1的实际goal1791990424保持complete。冻结参考原型的功能结果、原始数据和本版性能入口关闭保留。本更正把共同接口上的控制等价，与完整接口架构、发表覆盖和原生成本分别说明，不恢复候选。

## 共同接口上的控制等价

R2的O-B5S反驳了必须另存prepared头、保持rank或把raw cell留到B的成本主张。CO1的OC1-min是在阅读候选卡后构造的等价精化，证明TTL与结构rank不是本规则所必需。SDH1的O-NBO和LR1的phased/targeted均为独立代码，但共用候选新增的付费事实与执行接口。

准确结论是：**同一选定接口和执行器下，具名程序复现冻结规则，逻辑配对差为零；没有已证明的候选专属实现成本优势。** 这不证明完整协议已经发表、现有原生系统已有全部接口，或所有实现物理成本相同。完整接口的实际需求、准确前作、信息产生、集成成本和净收益仍要分别裁决。

[分类原件](../root/candidate-scope-review.json)及[关键原文](../root/critical-source-excerpts.md)记录具体来源。成本目录完整，不等于实现总成本已经数值闭合。

## Intel摘录导航

LR1的旧`root/primary-review.json`嵌入文本与所引用的物理行号不一致：三段正文涉及PASID-cache、HPT及optional invalidation，不能作为声称drain段落的摘录证据。旧文件的精确副本保存在[primary-review-original.json](primary-review-original.json)，旧pack不改写。

正确来源仍是原下载的Intel VT-d Rev5.0正文：保存文本的LF物理行8129–8142定义主存drain及RC取完读响应，8169–8186规定global/domain/page范围。[具体诊断](line-index-diagnosis.json)确认原文本SHA未变，三段旧摘录精确匹配`str.splitlines()`的位置而不匹配LF物理位置；该文本有354个form feed，逻辑行号与物理行号因此不同。新的[物理行摘录](../root/critical-source-excerpts.md)明确以LF划分行并保留form feed，与`nl -ba`一致。这些正确原文继续支持“domain/page相关drain已有前史；RC fetch完成不同于DMA整页R收据”。

## TC交接与后继义务

旧`source/native-chain.md:27`把TC COMPLETE/raw pop描述为真正外部物理AR握手。实际COMPLETE按内部`axi_comp_resp.ar_ready`交接，并且后继demux启用SpillAr/SpillR。

应写为：**内部完整地址移交允许raw cell退休；后继spill、held header及已接受R继续拥有不可丢弃的访问义务。** 内部交接、外部首次VALID、外部真正接受和R退休各是不同事件。cache失效和raw pop都不能直接充当页面ACK。

[native-chain-original.md](native-chain-original.md)保留旧说明；新[源码摘录](../root/critical-source-excerpts.md)给出两个端点及spills。

## 多别名成功范围

LR1原型对相同源PA但不同context/IOVA的登记项直接拒绝，成功共享reader例在支持的相同范围内。因此应写为“已支持范围内成功；未闭合跨context/IOVA别名拒绝”，不能称已实现一般多别名成功撤权。

源整页R收据、accepted-parent封口和通知主要属于DMA/系统接口。旧walk晚fill、翻译结果关账、正式context范围及真正失效/排序完成可能涉及IOMMU增量，但LR1没有建立其相对普通drain后invalidate的完整成本或净收益优势。补齐标准CQ能力本身也不算方法创新。

[机器可读更正](corrections.json)列明每项原件、修正证据和有限影响。未运行旧模型、功能检查或性能实验。
