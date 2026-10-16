# 文献缺口审计与补存（2026-10-06）

针对“相关文献是否还有遗漏”的有界审计。20项经公开原始来源核实的优先条目，其中16项补存全文、4项正式论文全文待取。原123项主目录与19项缺全文状态保持；本批尚未全部生成全文阅读卡，不把下载等同于精读。

- [审定说明](../../../docs/shared-translation-gaps-20261006.md) · [结构化清单](verified-gaps.json) · [审计](audit.json) · [根审阅](ROOT_REVIEW.md)

- DeepSeek：[原任务](gap-input.json)、[原答复](gap.md)、[修订任务](revision-input.json)、[三篇证据与草稿](revision.md)。保留初稿错误供追溯，最终结论以根审阅及审定说明为准。

- 复用上一轮下载器；[批1记录](records/download-batch1.json)和[批2记录](records/download-batch2.json)含失败/替代来源及哈希。MIND作者原链接404后改用已核实arXiv版本；无盲重试。

| 文献 | 出版/版本 | 优先级 | 来源及本地全文 | 阅读深度 |
|---|---|---|---|---|
| Coz: Finding Code that Counts with Causal Profiling | SOSP 2015 | P0 | [来源](https://arxiv.org/pdf/1608.03676) · [PDF](papers/coz2015.pdf) · [文本](papers/coz2015.txt) | root_key_sections_checked |
| Bottleneck Identification and Scheduling in Multithreaded Applications | ASPLOS 2012 | P0 | [来源](https://people.inf.ethz.ch/omutlu/pub/bottleneck-identification-and-scheduling_asplos12.pdf) · [PDF](papers/bis2012.pdf) · [文本](papers/bis2012.txt) | root_key_sections_checked |
| The Application Slowdown Model: Quantifying and Controlling the Impact of Inter-Application Interference at Shared Caches and Main Memory | MICRO 2015 | P0 | [来源](https://users.ece.cmu.edu/~omutlu/pub/application-slowdown-model_micro15.pdf) · [PDF](papers/asm2015.pdf) · [文本](papers/asm2015.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| MISE: Providing Performance Predictability and Improving Fairness in Shared Main Memory Systems | HPCA 2013 | P0 | [来源](https://www.istc-cc.cmu.edu/publications/papers/2013/mise-hpca13.pdf) · [PDF](papers/mise2013.pdf) · [文本](papers/mise2013.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| Parallelism-Aware Batch Scheduling: Enhancing both Performance and Fairness of Shared DRAM Systems | ISCA 2008 | P1 | [来源](https://people.inf.ethz.ch/omutlu/pub/parbs_isca08.pdf) · [PDF](papers/parbs2008.pdf) · [文本](papers/parbs2008.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| Redundant Memory Mappings for Fast Access to Large Memories | ISCA 2015 | P0 | [来源](https://pages.cs.wisc.edu/~swift/papers/isca15-rmm.pdf) · [PDF](papers/rmm2015.pdf) · [文本](papers/rmm2015.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| Twizzler: a Data-Centric OS for Non-Volatile Memory | USENIX ATC 2020 | P1 | [来源](https://par.nsf.gov/servlets/purl/10222952) · [PDF](papers/twizzler2020.pdf) · [文本](papers/twizzler2020.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| MIND: In-Network Memory Management for Disaggregated Data Centers | SOSP 2021 | P1 | [来源](https://arxiv.org/abs/2107.00164) · [PDF](papers/mind2021.pdf) · [文本](papers/mind2021.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| TinyLFU: A Highly Efficient Cache Admission Policy | 2015 author preprint; formal version to verify | P1 | [来源](https://arxiv.org/pdf/1512.00727) · [PDF](papers/tinylfu2015.pdf) · [文本](papers/tinylfu2015.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| Leases: An Efficient Fault-Tolerant Mechanism for Distributed File Cache Consistency | SOSP 1989 | P1 | [来源](https://www.cs.wm.edu/~liqun/teaching/cs780_17s/papers/leases.pdf) · [PDF](papers/leases1989.pdf) · [文本](papers/leases1989.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| Rethinking Unified Memory for NPU-PIM Systems: Dual-View Memory for Dynamic Inference of LLM | MICRO 2026 accepted_upcoming; arXiv 2608.06989 | P0 | [来源](https://arxiv.org/pdf/2608.06989) · [PDF](papers/pfm2026.pdf) · [文本](papers/pfm2026.txt) | root_key_sections_checked |
| Dead on Arrival: Characterizing and Protecting Against Dead-Entry TLB Misses in GPU Microarchitectures | 2026 preprint; submitted is not accepted | P0 | [来源](https://arxiv.org/pdf/2606.00486) · [PDF](papers/deadentry2026.pdf) · [文本](papers/deadentry2026.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| Virtuoso: Enabling Fast and Accurate Virtual Memory Research via an Imitation-based Operating System Simulation Methodology | ASPLOS 2025 | P1 | [来源](https://arxiv.org/pdf/2403.04635) · [PDF](papers/virtuoso2025.pdf) · [文本](papers/virtuoso2025.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| CounterPoint: Using Hardware Event Counters to Refute and Refine Microarchitectural Assumptions | ASPLOS 2026 | P0 | [来源](https://cs.stanford.edu/~trippel/pubs/lindsay_ASPLOS26.pdf) · [PDF](papers/counterpoint2026.pdf) · [文本](papers/counterpoint2026.txt) | root_key_sections_checked |
| Criticality Stacks: Identifying Critical Threads in Parallel Programs using Synchronization Behavior | ISCA 2013 | P0 | [来源](https://users.elis.ugent.be/~leeckhou/papers/isca13.pdf) · [PDF](papers/criticality2013.pdf) · [文本](papers/criticality2013.txt) | root_key_sections_checked |
| ATLAS: A Scalable and High-Performance Scheduling Algorithm for Multiple Memory Controllers | HPCA 2010 | P1 | [来源](https://www.cs.cmu.edu/~dongsuh/paper/HPCA16_ATLAS.pdf) · [PDF](papers/atlas2010.pdf) · [文本](papers/atlas2010.txt) | primary_metadata_or_abstract_checked_full_review_pending |
| Criticality-Aware Dynamic Task Scheduling for Heterogeneous Architectures | ICS 2015 | P0 | [来源](https://www.bsc.es/mateo-valero/71-conferences) | metadata_plus_related_author_talk |
| Trail: Effective, Low-Cost and Scalable Temporal TLB Prefetching via Page-Table-Embedded Deltas | MICRO 2026 accepted_upcoming | P1 | [来源](https://www.microarch.org/micro59/program/) | metadata_only |
| SPMC: Speculative Multicast for GPU Address Translation Sharing | MICRO 2026 accepted_upcoming | P0 | [来源](https://www.microarch.org/micro59/program/) | metadata_only |
| Make Every Batch Count: Fault Entry Merging for Efficient Batching in Unified Virtual Memory | MICRO 2026 accepted_upcoming | P0 | [来源](https://www.microarch.org/micro59/program/) | metadata_only |

P0表示需要优先检查的新颖性威胁，不等于已证明完全覆盖候选。来源确认、原文关键段核验、完整阅读与实验复现是不同状态。本轮未启动仿真、改RTL/Linux、构建或上板。
