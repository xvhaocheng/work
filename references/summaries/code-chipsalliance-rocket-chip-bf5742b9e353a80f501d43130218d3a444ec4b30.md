# chipsalliance/rocket-chip/bf5742b9e353a80f501d43130218d3a444ec4b30〔正式题名待整理〕

稳定ID：`code-chipsalliance-rocket-chip-bf5742b9e353a80f501d43130218d3a444ec4b30` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/chipsalliance/rocket-chip/blob/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala)
- [来源2](https://raw.githubusercontent.com/chipsalliance/rocket-chip/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/translation-continuation-frontier-20261008.md:31](../../iommu-platform/docs/translation-continuation-frontier-20261008.md)
- [iommu-platform/out/innovation-frontier-reset-20261008/codex-critical-design.md:59](../../iommu-platform/out/innovation-frontier-reset-20261008/codex-critical-design.md)
- [iommu-platform/out/innovation-frontier-reset-20261008/deepseek-report-draft.md:34](../../iommu-platform/out/innovation-frontier-reset-20261008/deepseek-report-draft.md)
- [iommu-platform/out/innovation-frontier-reset-20261008/continuation-critical-agent.md:115](../../iommu-platform/out/innovation-frontier-reset-20261008/continuation-critical-agent.md)
- [iommu-platform/out/innovation-frontier-reset-20261008/continuation-prior-agent.md:13](../../iommu-platform/out/innovation-frontier-reset-20261008/continuation-prior-agent.md)
- [iommu-platform/out/translation-progress-frontier-20261008/rules-proof-agent.md:120](../../iommu-platform/out/translation-progress-frontier-20261008/rules-proof-agent.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/translation-continuation-frontier-20261008.md:31](../../iommu-platform/docs/translation-continuation-frontier-20261008.md)：

- **最强前作**：[US8140781B2](https://patents.google.com/patent/US8140781B2/en)（2009 公开）Fig3–6、step305–345、claims 2–3：tracking queue、completion token、合成下一层 packet 并重新仲裁。root 另直接核官方 Rocket 2022 pinned 源码 commit `bf5742b9e353a80f501d43130218d3a444ec4b30`，[PTW.scala](https://github.com/chipsalliance/rocket-chip/blob/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala) :158–171（`aux_pte/r_pte/count` 等）、:196–214（由页号＋request 索引恢复 PTE 地址）、:524–565（父/leaf 复用与 G 返回恢复 S）。具体表示主体已被覆盖；这不是“笼统 ordinary 可抄任何算法”式的不新。

[iommu-platform/out/innovation-frontier-reset-20261008/continuation-critical-agent.md:115](../../iommu-platform/out/innovation-frontier-reset-20261008/continuation-critical-agent.md)：

它与候选逐字段同宽、同恢复公式、同PTE尾部释放及零重读保证。这里不是泛泛模仿：Rocket 2022固定版本已经使用`r_pte`保存当前物理PPN，`aux_pte`在S nonleaf父PPN与S leaf之间复用，target/index从`r_req`与aux恢复，count/aux_count表示两层level。[官方固定源码](https://raw.githubusercontent.com/chipsalliance/rocket-chip/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala)、本地核读158–214、559–565。

[iommu-platform/out/innovation-frontier-reset-20261008/deepseek-report-draft.md:34](../../iommu-platform/out/innovation-frontier-reset-20261008/deepseek-report-draft.md)：

- **最强前作**：[US8140781B2](https://patents.google.com/patent/US8140781B2/en)（2009 公开）Fig3–6、step305–345、claims 2–3：tracking queue、completion token、合成下一层 packet 并重新仲裁。**新增已给定事实**：root 直接核官方 Rocket 2022 pinned 源码 commit `bf5742b9e353a80f501d43130218d3a444ec4b30`，[PTW.scala](https://github.com/chipsalliance/rocket-chip/blob/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala) :158–171（`aux_pte/r_pte/count` 等）、:196–214（由页号＋request 索引恢复 PTE 地址）、:524–565（父/leaf 复用与 G 返回恢复 S）。具体表示主体已被覆盖；这不是“笼统 ordinary 可抄任何算法”式的不新。

[iommu-platform/out/translation-progress-frontier-20261008/rules-proof-agent.md:120](../../iommu-platform/out/translation-progress-frontier-20261008/rules-proof-agent.md)：

Rocket 2022 的 `aux_pte` 已在 S parent 与 S leaf 间复用，`r_pte` 提供物理 table PPN，地址offset由原 request/levels重建。因此 q3/q2 普通 tracking token 可以直接按这些旧字段加上述保存规则实现，而非仅凭“ordinary能照抄任何新算法”否决。[固定源码](https://raw.githubusercontent.com/chipsalliance/rocket-chip/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala)，关键 `:158–214,524–530,559–565` 已在§44直接核读。原源码保留 full PTE 和 CPU 用字段，不能声称它已经公布77/81/82位IOMMU checkpoint。

[返回总目录](../README.md)
