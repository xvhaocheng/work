# 四候选比较：本轮证据与审定

日期：2026-10-06。交付：[前作—场景—约束—验证矩阵](../../docs/translation-comparison-20261006.md)。本轮为文献比较与实验设计，没有新实验、构建、板卡操作或论文复现。

## 模型分工与原始记录

DeepSeek 提取给定原文、草拟比较与反证，不能执行命令或自主浏览。Codex 执行工具、核关键来源，负责研究取舍、实验设计与最终审阅。本轮未调用 Claude 或新建 Codex 子 agent。

| 组 | 范围 | 完整答复／原始返回／输入 | provider total tokens |
|---|---|---|---:|
| A | ①依赖调度、③观测与动作 | [答复](A_dependency_observation.md) · [JSON](A_dependency_observation.json) · [输入](A_dependency_observation-input.json) | 324176 |
| B | ②映射生命周期、④翻译表示 | [答复](B_lifecycle_representation.md) · [JSON](B_lifecycle_representation.json) · [输入](B_lifecycle_representation-input.json) | 391321 |

两次均为 deepseek-flash、thinking enabled/high、requested/effective budget 393216、finish_reason stop、truncated false，CLI exit 0；合计 715497 provider tokens。这是 API 返回用量，不是价格或节约量。机器元数据见 [audit.json](audit.json)。

## Codex 审阅与未采用推论

- A 将“依赖信息不能由运行时廉价取得”列为必要条件，过强。运行时廉价提供信息完全可能是方案的一部分；应问普通运行时调度或简单优先级用同样信息是否已取得同效果。
- B 要求“相同维护成本”，不适合比较生命周期策略。固定正确性语义和总资源预算，维护消息、元数据、确认、重填是待测结果。
- 不采用 Hydra+IDYLL+ShareMMU 直接拼装基线：它们的复制对象、层级和接口不同。先选定层级，再实现适用思想及其实际成本。
- 不要求证明现有 ATS 接口缺少某功能才准研究；先核实际接口。新增接口只是设计及成本，不自动构成创新。
- A 未收到 VIO 全文，B 未收到 VBI/DIPTA 等全文，均不等于工作区未收集。上述资料已存在，不重复列作下载缺口。
- 原始答复若给了精确行号，不整体认证。例如 IDYLL 立即失效的关键段实际在本地文本 569–573、646–650；CLAP+migration 在 1417–1443；ShareMMU 等待确认在 420–430 和 550 附近。最终文档以已核章节及全文链接为准。
- ObservUVM 实测卡为 RTX 3090，原文注明 24 GB GDDR；最终文档使用“GPU 显存”，不把论文泛称的 HBM 当成该设备物理内存类型。
- SoC DMA 是 FPGA 实现用作硬件仿真平台，负载是由应用阶段提取的可调基准；不能称已获得完整 NIC–GPU 跨设备应用。CLAP 的跨 kernel 迁移扩展是构造示例，不能证明真实工作负载普遍频繁变化。
- 依赖完整的启发式不自动是最优 oracle；固定时间戳 trace 不能证明带反馈的端到端效果。③的动作后悔值必须定义可行动作和状态演化，不能由分类准确率代替。
- ④按总字节/带宽/内存预算比较，不强行固定相同页大小和条目数；若数据复制在应用中允许，应作为带成本基线。EMT 是框架，不能冒充真实目标应用。

## 本轮直接核读范围

| 本地原文 | 关键位置 | 直接用于审定的事实 |
|---|---|---|
| `../shared-translation-literature-20261006/papers/socdma2018.txt` | §V-A/B，419–487 附近 | Zynq/PMCA/共享内存路径；FPGA emulator；两类应用阶段基准。 |
| `../shared-translation-literature-20261006/papers/hydra2024.txt` | §3.2、§3.5，365–395、538–578 | 惰性部分复制；按页表 sharer 缩小 shootdown，非每设备 TLB 精确驻留。 |
| `../shared-translation-literature-20261006/papers/idyll2023.txt` | 569–573、646–650 | 延迟 PTE 失效，但收到失效时 TLB 仍立即处理。 |
| `../shared-translation-literature-20261006/papers/sharemmu2026.txt` | §V-C，420–455、535–568 | back-invalidation、writeback/flush ack、在途请求完成；不能省略确认成本。 |
| `../shared-translation-literature-20261006/papers/clap2025.txt` | Cooperation with Page Migrations，1417–1443 | CLAP+migration 已存在；跨 kernel 变化是特设示例。 |
| `../shared-translation-literature-20261006/papers/emt2025.txt` | Limitations，476–490 | 当前聚焦 CPU VA→PA，不面向 DMA IOMMU，但明确可潜在扩展。 |
| `../shared-translation-literature-20261006/papers/observuvm2026.txt` | §VII，650–682 | RTX 3090/GDDR、14 应用、反馈驱动策略。 |
| `../shared-translation-literature-20261006/gap-audit-02/papers/counterpoint2026.txt` | 735–759 | MMU 模型约束与真实采样；不等于在线最优控制。 |

PFM、2018 PTW 调度、BIS/Criticality/Coz、VIO 和 GPUDirect 的关键事实沿用此前根审阅并与本次草稿对照，未声称本轮重新逐字精读全部论文。不是所有 worker 证据都经逐条原文复核。

## 补证、失败与导航检查

[web-1](web-1.txt)、[web-2](web-2.txt)、[web-3](web-3.txt)：ShadowUpdate 官方机构摘要/题录、两位作者页面，出版社直达返回 418；未取得全文。摘要已明确失效传播新映射与迁移 tracker，足以限制宽泛主张，不能替代完整协议。

[web-4](web-4.txt)、[web-5](web-5.txt)：SPMC 官方程序、CATS 正式线索/相关讲稿；宽泛 HELM 查询出现同名无关结果，未采纳，后续改用完整题名。

[web-6](web-6.txt)、[web-7](web-7.txt)：HELM 出版社页面/搜索索引补出 MIM 与作者 artifact DOI；正文直达 403，Zenodo 网页工具未能访问，未下载或运行 artifact。CATS 正式全文仍未取得，不把 UPC 讲稿当正式论文。

本轮没有新增 PDF，主 123 项及两批增补统计不改。关键全文不可得记录为未排除覆盖风险，不写成研究空白。文档链接与两次返回元数据的机械检查见 [delivery-checks.json](delivery-checks.json)，检查不等于研究验证。
