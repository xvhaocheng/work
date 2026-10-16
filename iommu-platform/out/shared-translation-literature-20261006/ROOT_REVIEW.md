# 根审阅记录：共享地址翻译专题

> 后续增补：当前目录为123项；本文件保留首轮72项的审定记录。新12项关键核查、CLAP补全文与SoftWalker引用修正见[增补审定](expansion-01/ROOT_REVIEW.md)。

截至 2026-10-06。DeepSeek 阅读分组全文并提出证据卡；Codex 检索原始来源、直接核关键段落和元数据，作以下裁定。阅读和目录校验不是论文复现，也不是对全部文字的逐句审计。

## 关键原文核查

| 工作 | 直接检查位置 | 接受的窄结论 |
|---|---|---|
| MGvm | papers/mgvm2022.txt，dHSL-balance 与 per-kernel HSL 配置段 | 粗粒度映射有局部性收益，也可能使热点拥塞在一个 TLB slice；已有运行时细化映射，因此“局部性+负载均衡联合考虑”不是空白。 |
| Trans-FW | papers/transfw2023.txt，When to borrow 与 latency sensitivity 段 | 已利用 host PW-queue 压力决定远端转发，也处理双路结果；远端翻译不总比本地排队快。不可称完全未考虑排队。 |
| Barre Chord | papers/barre2024.txt，Support for dynamically allocated data、page migration 与 large page 段 | 特定合并策略不支持 kernel 内动态分配的数据；不能扩大成整个系统不支持任何动态分配或迁移。 |
| HDPAT | papers/hdpat2026.txt，architecture assumptions 与 performance/ablation 段 | 明确排除 GPU-to-GPU page migration，未评估 shootdown 影响；中心 IOMMU 在一些不规则负载中仍占主导。大会页旧摘要 13 项/57.99% 与本地最终稿 14 项/1.57×不同，结果按各版本分别引用。 |
| WaferTrans | papers/wafertrans2026.txt，PTE Presence Consistency、Distributed PTE-PC Mapping、Network Pressure | presence 信息与完整 PTE 内容/权限一致性不同；作者报告迁移不频繁条件下的维护流量，不能外推频繁重映射性能。全文是 2026-09 预印本，模板页眉不当期刊录用证明。 |
| Fast & Safe IO Memory Protection | papers/fastsafe2024.txt，strict protection、selectively preserving PTcache、tail latency | 通过保留仍有效的中间页表缓存降低 miss 代价，不能简化为延后撤销叶映射或取消严格隔离；论文含尾延迟数据，不可声称该领域从未测尾延迟。 |
| Bermuda | papers/bermuda2025.txt，abstract、methodology 与 SMMUv2 micro/macro-TLB | 1.79×延迟是特定平台和小传输条件下报告值，不是通用 IOMMU 界，也不是该工作已提供缓解保证。 |
| ShareMMU | papers/sharemmu2026.txt，shared TLB permission checks 与 shootdown/back-invalidation 段 | 复用要满足权限校验；仍等待 dirty write-back、私有 TLB 清理及确认。不能把共享映射当成免费撤销或数据一致性。 |
| IDYLL | papers/idyll2023.txt，in-PTE directory 与 IRMB 段 | 已有定向失效、合并和允许保守误报的设计；不是“GPU 映射更新从未被处理”。 |
| cuPTW | papers/cuptw2026.txt，第4—5节及资源配置/瓶颈分析 | 以低 PTW 并发、MSHR 饱和和空闲计算资源为重要动机，计算资源充裕与否是边界。主性能评估是模拟，不能用标题推断已有商用实现。 |
| GRIT | papers/grit2024.txt，abstract 与 page placement/migration 动机 | 多GPU细粒度动态页放置已有强前作，应与翻译路径研究区分又同时考虑。 |
| NoCstar | papers/nocstar2018.txt，TLB miss storm 与 interconnect/request queue 段 | 已考虑部分争用与压力情形；其 CPU 核间模型不自动等价于异构设备端到端 QoS。 |

以上定位以章节和短语为主；PDF 双栏提取和 form-feed 会导致不同工具的行计数略有差别。worker 行号用于导航，不视作独立验证。

## 元数据与统计纠正

- S0 对旧地图计数的“矛盾”判断不采纳：旧 manifest 的 43 项可以由 38 全文版本、4 官方文档和 1 未取得来源组成；36 独立工作与38版本也可以同时成立。
- 不采用 S0 混合标准状态、全文可得性和实物评价的单一“成熟度”等级。出版状态、阅读深度、评价类型分别保存。
- GPU 数据 publish/subscribe 不直接等于翻译结果共享；GPS 等外围论文未为凑数纳入本专题。
- 题头缺会议字段，不等于尚未发表。已以作者/机构/大会或出版社来源补核 SLL TLB、HDPAT、Revelator、STAR、ObservUVM、DATE SVA、FPT migration 等。具体覆盖字段及来源保留在 records/root-overrides.json；原始 worker 答复不改写。
- Orchestrated Scheduling 的作者主页与 DAC 2023 官方节目一致，但下载稿包含2024引文；本地稿具体更新时间保留未知，不用该异常否认论文存在。
- SSR 作者列表用 Accelerator，下载稿题名用 GPU；记录题名差异，不将两个标题计两篇。
- STAR 的 arXiv 稿与 MICRO 2024 稿归为同一工作，保留两份版本及各自证据卡；目录首选正式稿，不宣称两版内容完全一致。
- SPARTA 仅核到当前预印本，保留 preprint。ShareMMU、NB-Walker、CATO 属截至本次日期的 MICRO 2026 接收/待开会工作；有 camera-ready 不自动算大会已召开。
- 只有题录/摘要的 ShadowUpdate、CLAP、LATPC、Clover 等可以约束“无人研究”的说法，不能据此声称全面核验其协议、评估或假设。

## 科学结论的限制

“基本共享思想历史长”有证据；“整个领域已做完”没有证据。本轮提出的是值得检验的瓶颈关系，不是已证实的创新空白。平均加速、公平性统计、尾延迟观测和最坏情况保证分开。实机动机测量、模拟机制收益、RTL面积估算也分开。

本轮未复现论文实验，未更改 RTL/Linux，未编译、上板或操作 SD。既有172项实验不为本次文献判断提供自动的新颖性或因果证明。
