# 项目参考文献与资料库

2026-10-16整理。这里集中项目**已经引用或核查过**的论文、规范、RFC/公开补丁、专利披露和技术资料，供新agent复用。共 **445 条资料记录、138 个本地PDF索引**；这不是同样数量的独立论文，也不是一次新的系统查新。

[逐条摘要总表](summaries/README.md) · [机器目录](catalog.json) · [本地原件](papers/README.md) · [未读与缺原件](reading-gaps.md) · [覆盖与维护规则](COVERAGE.md) · [验证结果](verification.json)

## 从当前问题开始

- **2026-10-16重新立题核读增量**：[GPUMMU/Neighborhood、服务分段与回收近邻的原文位置](research-reconvergence-20261016.md)。明确新增核读/既有复用、覆盖与未核边界；不以文献库收录或模型一致代替新意证据。
- **M0首PTE票据／接口实现成本**：先读 [MASK](summaries/mask2018.md) → [AXI-REALM](summaries/axi_realm2025.md) → [iDMA](summaries/idma_tc2024.md)。MASK已联合考虑PTE/data与配额，REALM已有合法分段/预算；这些前史限定当前增量。当前模型和接口成本问题见[当前状态](../CURRENT_STATUS.md)，不是文献库选择了论文主线。 本轮[源码/前作成对比较](../iommu-platform/docs/first-pte-interface-cost-20261016.md#5-论文贡献判断)定向核到MASK §5.4、AXI-REALM §III-A/IV-B、iDMA §II-C；保留原阅读等级，不升级为全面查新。
- **映射组织、持续授权、回收／CPU与内存成本**：[DAMN](summaries/damn2018.md) → [rIOMMU](summaries/riommu2015.md) → [Utilizing the IOMMU Scalably](summaries/iommu_scalable2015.md) → [IOTLB Wall](summaries/iotlb_wall2023.md) → [F&S](summaries/fastsafe2024.md) → [DMA_PMD RFC](summaries/dma_pmd_rfc2026.md)。明确区分来源已实现能力、公开补丁和普通适配。
- **共享翻译干扰与强前作**：[Bermuda](summaries/bermuda2025.md)、[DWS](summaries/walk_stealing2021.md)、[Scheduling PTW](summaries/walk_scheduling2018.md)、[Orchestrated](summaries/orchestrated2023.md)、[ShareMMU](summaries/sharemmu2026.md)、[DATE2025 RISC-V SVA](summaries/riscvsva2025.md)。
- **分析方法边界**：[RTAS2026 DPU](summaries/aromolo_rtas2026.md) → [TC2023完整job界](summaries/bounding_multiax2023.md) → [AXI-ICRT](summaries/axi_icrt2023.md)。FIFO外调度、合法分块、预算和一般响应分析均有前史。
- **补基础与广域近邻**：从[经典翻译](topics/foundations.md)、[多GPU](topics/multigpu.md)、[数据搬运/动态内存](topics/data-lifecycle.md)选读，不必先读完全部库。

## 按主题查找

| 主题 | 条目数 |
|---|---:|
| [异构加速器翻译](topics/accelerators.md) | 19 |
| [AXI与实时分析](topics/axi-realtime.md) | 4 |
| [数据搬运／动态内存／缓存分配](topics/data-lifecycle.md) | 16 |
| [DMA／数据流](topics/dma-dataflow.md) | 3 |
| [一般模型与形式化分析](topics/formal-analysis.md) | 1 |
| [经典翻译与覆盖](topics/foundations.md) | 29 |
| [GPU翻译与并发](topics/gpu.md) | 19 |
| [I/O与IOMMU](topics/io.md) | 14 |
| [映射更新与生命周期](topics/lifecycle.md) | 31 |
| [持久映射与回收](topics/mapping-lifecycle.md) | 10 |
| [多GPU／分布式翻译](topics/multigpu.md) | 11 |
| [专利技术披露](topics/patents.md) | 24 |
| [资源管理与普通强对照](topics/resource-management.md) | 5 |
| [安全近邻](topics/security.md) | 1 |
| [源码与artifact](topics/software-artifacts.md) | 58 |
| [标准与官方接口](topics/standards-software.md) | 60 |
| [补充引用，题录与摘要待整理](topics/supplementary-research.md) | 138 |
| [研究工具／方法资料](topics/workflow-tools.md) | 2 |

## 分清资料类型与阅读深度

| 类型 | 记录数 |
|---|---:|
| 论文／作者稿 | 259 |
| 已标明预印本 | 6 |
| 规范与官方技术文档 | 66 |
| 学位论文 | 1 |
| RFC | 1 |
| 公开补丁／邮件讨论 | 9 |
| 公开专利披露 | 24 |
| 软件源码／固定版本 | 51 |
| artifact元数据 | 7 |
| 题录／机构记录，身份待整理 | 21 |

| 阅读证据 | 记录数 |
|---|---:|
| 既有worker全文提取；非root逐篇精读 | 80 |
| 局部已核（保留原有核读范围） | 57 |
| 仅题录/摘要/提及或核读范围待整理 | 308 |

`worker_fulltext_record`表示历史worker对已存全文作了证据提取；不表示root或用户逐篇全文精读。`local_partial`保留具体章节/源码核查范围。`metadata_or_mention`不作全文机制结论。每条的`legacy_reading_status`、`legacy_root_spotchecked`保留旧123专题状态，后来的核读单独列`root_read_evidence`，不让旧false抹去后来证据。

共有 **310 条**题录字段仍显式不完整（`unknown`或仅项目标签），可直接追到已有来源，不能伪装成规范的最终参考文献表。已知题名、作者与年份来自既有记录；没有根据标题补机制。历史引用、当前采用、原文作者结果和本项目复现分开。

## 覆盖到哪里

受控扫描 **1581 份项目叙述文档**，规范化 **697 个外部URL**；其中 575 个已归入条目，122 个保留导航/操作排除标记，详见[url-inventory.json](url-inventory.json)。原共享翻译 **123项全部保留**，STAR不同版本仍归一项；后来AXI/RTAS、映射/RFC、缓存/数据流、KV/LLM和形式化近邻另行补入。

仅按精确DOI、arXiv编号、已确认镜像/工作身份归并；未完成身份核对的题录记录保留为待整理，**条目数不能用于独立工作计数**。没有穷尽历史源树、PDF参考文献表、原始HTTP日志或未带链接的每个名称；这些潜在遗漏明确留在[COVERAGE.md](COVERAGE.md)。本轮没有新联网检索、论文复现实验、RTL/Linux修改、构建、硬件操作或研究状态恢复。

本轮由Codex整理并核验文件链接/原件哈希；历史目录中的DeepSeek全文证据提取归属继续保留，不将其改写成Codex本轮重读。
