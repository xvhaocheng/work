# Leone等，Physically-Aware Preemptive Virtual Channels for Deadlock-Free AXI Networks-on-Chip，arXiv2607.01430v1〔正式题名待整理〕

稳定ID：`arxiv-2607.01430` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/abs/2607.01430)
- [来源2](https://arxiv.org/pdf/2607.01430)
- [来源3](https://arxiv.org/html/2607.01430v1)
- [来源4](https://arxiv.org/abs/2607.01430v1)
- [来源5](https://arxiv.org/pdf/2607.01430v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/arxiv-2607.01430.pdf) · canonical：[原路径](../../iommu-platform/out/fragment-drain-boundary-20261008/map-agent-sources/release-prior/preemptive-vc2026.pdf) · SHA-256 `6a3188b56b64c8e31095219940cb97e762f7496ccbbd560c7dd7ae802c373ea0`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/composition-history-isolation-20261007.md:172](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/composition-history-isolation-20261007.md:212](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/composition-history-isolation-20261007.md:419](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/composition-history-isolation-20261007.md:518](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/dependency-contract-innovation-20261008.md:20](../../iommu-platform/docs/dependency-contract-innovation-20261008.md)
- [iommu-platform/docs/fragment-drain-cost-boundary-20261008.md:47](../../iommu-platform/docs/fragment-drain-cost-boundary-20261008.md)
- [iommu-platform/docs/natural-write-order-boundary-20261008.md:91](../../iommu-platform/docs/natural-write-order-boundary-20261008.md)
- [iommu-platform/docs/original-window-progress-boundary-20261008.md:62](../../iommu-platform/docs/original-window-progress-boundary-20261008.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/composition-history-isolation-20261007.md:419](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

定向核[CTC2009 §3–4](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)、[Arm DMA350 §4.3.4](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)、[AXI-REALM](https://arxiv.org/pdf/2501.10161)、[2026 Preemptive VCs §III](https://arxiv.org/html/2607.01430v1)、[旧IOMMU专用VC专利](https://patents.google.com/patent/US7480784B2/en)和MASK §5.4。共享、接收预约、避免DMA循环均有前作；2026 VC等此前§12已核，不称全部新文献。增量微架构论文不要求每个原语首创，但必须有可推广的实现边界、可信成本性能证据，不能只换位置/名字。

[iommu-platform/docs/composition-history-isolation-20261007.md:172](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

协议边界：[Arm AXI规范](https://documentation-service.arm.com/static/67ab5a186dbc975ccea91734)规定AXI4不支持W数据交错，合并写事务后必须保持对应地址顺序。这不等于协议强制整个系统只能有一个在途事务；当前demux门控是防特定依赖的实现选择，替代设计需要保留合法路由与进度。现成coupler参考[官方iDMA项目](https://github.com/pulp-platform/iDMA)，以本地实际版本为准。另发现[2026年AXI NoC流量类别分离预印本](https://arxiv.org/abs/2607.01430)，仅核作者摘要：讨论端点读写协议依赖与多平面/VC资源代价；未全文核机制，不据此声称它解决当前banked IOMMU问题或建议重构NoC。

[iommu-platform/docs/posttl-independent-verdict-20261008.md:47](../../iommu-platform/docs/posttl-independent-verdict-20261008.md)：

原文链接：[AXI-REALM](https://arxiv.org/html/2501.10161v1)、[Preemptive VC](https://arxiv.org/html/2607.01430v1)。[Arm AXI规范](https://community.arm.com/cfs-file/__key/communityserver-discussions-components-files/476/IHI0022H_5F00_amba_5F00_axi_5F00_protocol_5F00_spec.pdf) A3.4/A4.3.1允许本例大于16拍、非exclusive的Non-modifiable INCR拆分；当前原父256拍、子128/256拍，属性没有伪改成可拆。

[iommu-platform/docs/research-novelty-assessment-20261015.md:51](../../iommu-platform/docs/research-novelty-assessment-20261015.md)：

- **生命周期、同步与读写耦合本身都有先例。** Linux dma-buf明确区分设备完成和CPU缓存可见性；dmaplane预印本讨论映射、所有权、信用及完成；2026年NoC预印本研究DMA读写耦合循环和有限VC代价。这些有助于把合同做对，不能作为宽泛新概念。[Linux文档](https://docs.kernel.org/driver-api/dma-buf.html)；[dmaplane §3–5](https://arxiv.org/html/2603.10030v1)；[AXI NoC §I/III](https://arxiv.org/html/2607.01430v1)。两篇预印本的身份不等同已同行评审论文。

[返回总目录](../README.md)
