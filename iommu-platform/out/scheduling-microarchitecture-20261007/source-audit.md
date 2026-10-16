# 原源定位（Codex 定向核查，2026-10-07）

定位使用本轮网页解析器的 PDF 0-based 页号／行号，正文页码另注明；不把未获全文项列作精读。全部为转述，不抄录长段。

| 原源 | 本轮核到的内容与定位 | 对候选的限制 |
|---|---|---|
| [Improving Memory Scheduling via Processor-Side Load Criticality Information, ISCA 2013](https://users.ece.cmu.edu/~saugatag/papers/13isca.pdf) | PDF p2，解析行213–222、289–294；相关工作行1019–1048：criticality 与 CAS/RAS 顺序、关键性标志；已有阻塞消费者等判据。 | release-aware 大概念不能单独立题。 |
| [Staged Memory Scheduling, ISCA 2012](https://people.inf.ethz.ch/omutlu/pub/staged-memory-scheduling_isca12.pdf) | PDF p3–5，解析行451–520、571–608：source FIFO、批形成、SJF/RR、轻载旁路、同 source 顺序。 | 准备队列/分来源/避 HOL 均需具体增量。 |
| [DASH: Deadline-Aware High-Performance Memory Scheduler for Heterogeneous Systems with Hardware Accelerators](https://people.inf.ethz.ch/omutlu/pub/dash_deadline-aware-heterogeneous-memory-scheduler_taco16.pdf) | PDF p5/p8–9，解析行248–257、355–415：完成内存请求进度、期望进度、短期限高优先、应用强度。全文页眉为 December 2015；作者网页也按2016收录。 | 完成进度反馈不是新颖性依据。 |
| [Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engines, ICCD 2018](https://arxiv.org/pdf/1808.09751) | §IV-C，PDF p5，解析行586–718：整传输锁TLB的限制、burst retirement metadata、ID顺序、失败排空与重发。 | 地址/job/读写元数据和按段准备本身已很接近前作。 |
| [AXI-REALM, 2025稿](https://arxiv.org/pdf/2501.10161) | §III-A，PDF p3–4，解析行480–558；p5行641–684：碎片化/响应合并、完整写payload再发AW/W、周期budget、分阶段跟踪。 | D须面对可配置短burst与完整写缓冲；不能弱化对照。 |
| [Arm DMA-350 TRM, Issue 04, 2023-01-18](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2) | §5.8.1，正文87页，PDF index86，解析行3327–3337：读请求有完整返回空间，写请求有完整burst数据。 | 接收空间/写数据齐备不是本项目首创。 |
| [Adaptive Burst-Writes, TODAES 2015, DOI 10.1145/2753757，作者机构记录](https://researchportal.hkust.edu.hk/en/publications/adaptive-burst-writes-abw-memory-requests-scheduling-to-reduce-wr/) | 仅机构摘要，解析行14–26：按写row-hit与读到达率改变一次write batch数量。未获/未核正文。 | 只限制宽泛自适应写批次主张，不等同AXI单burst切分。 |
| [NB-Walker MICRO 2026 artifact](https://zenodo.org/records/21488827) | 正式题录/公开artifact；此前已定向检查AE树，本轮未重克隆/执行，仍无论文正文。 | 单walker非阻塞等宽泛主张仍有前沿全文缺口。 |

自适应 AXI burst＋翻译/成对准备状态的精确同式，尚未排除。检索无命中不是新颖性证明；本轮未搜遍全部专利或文献，也未复现以上论文。
