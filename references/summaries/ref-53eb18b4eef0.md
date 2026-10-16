# RISC-V CBQRI v1.0 的 IOMMU QoS 扩展〔正式题名待整理〕

稳定ID：`ref-53eb18b4eef0` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://docs.riscv.org/reference/cbqri/v1.0/qos_iommu.html)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/paper-scenarios-mitigation.md:173](../../iommu-platform/docs/paper-scenarios-mitigation.md)
- [iommu-platform/out/innovation-council18-20261005/round1/C2.md:22](../../iommu-platform/out/innovation-council18-20261005/round1/C2.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C2.md:22](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C2.md)
- [iommu-platform/out/innovation-council18-20261005/round2/C2.md:31](../../iommu-platform/out/innovation-council18-20261005/round2/C2.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/paper-scenarios-mitigation.md:173](../../iommu-platform/docs/paper-scenarios-mitigation.md)：

规范层面，[RISC-V CBQRI v1.0 的 IOMMU QoS 扩展](https://docs.riscv.org/reference/cbqri/v1.0/qos_iommu.html)已定义设备/IOMMU 请求的 QoS 身份关联，并描述可选 IOATC 容量分配与监测。因此增加身份、配额或配置接口也不能直接当新算法。规范提供接口不等于解决了本课题的内部服务等待，更不表示当前锁定 RTL 已支持这些能力。

[iommu-platform/out/innovation-council18-20261005/round1/C2.md:22](../../iommu-platform/out/innovation-council18-20261005/round1/C2.md)：

| [RISC-V CBQRI v1.0 IOMMU QoS ID](https://docs.riscv.org/reference/cbqri/v1.0/qos_iommu.html) | 官方 §4.1 全节 | `DC.ta` 的 RCID/MCID 已关联页表访问和翻译后的设备请求；另有全局 IOMMU 请求 ID，并允许 IOATC 容量控制。简单增加设备标签或将 PTE 流量归属到设备不能算新接口。 |

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C2.md:22](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C2.md)：

| [RISC-V CBQRI v1.0 IOMMU QoS ID](https://docs.riscv.org/reference/cbqri/v1.0/qos_iommu.html) | 官方 §4.1 全节 | `DC.ta` 的 RCID/MCID 已关联页表访问和翻译后的设备请求；另有全局 IOMMU 请求 ID，并允许 IOATC 容量控制。简单增加设备标签或将 PTE 流量归属到设备不能算新接口。 |

[iommu-platform/out/innovation-council18-20261005/round2/C2.md:31](../../iommu-platform/out/innovation-council18-20261005/round2/C2.md)：

| S8 | [CBQRI v1.0 IOMMU QoS ID](https://docs.riscv.org/reference/cbqri/v1.0/qos_iommu.html)：R1 核官方 §4.1 全节；标签、可选 IOATC 容量控制已有，不等于任意 PWC 前缀驻留保证。 |

[返回总目录](../README.md)
