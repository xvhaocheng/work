# 两篇原作者来源的精确覆盖边界

两篇正文覆盖多个候选组件，但本席没有在指定段落找到本轮“两个合法 bank 头、共享服务两顺序预测、全 owned 完成 max/sum 排名”的完整发表机制。**组件已有不等于整体已覆盖；两篇未出现也不证明创新。** 判断仅限已读原文，未开展全面检索。

## MASK，ASPLOS 2018

[原作者 PDF](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)；[本地原件](/home/xvhaocheng/work/iommu-platform/out/tl-memory-feedback-20261014/source-prior/mask-asplos2018.pdf)。题名为 *MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency*，Rachata Ausavarungnirun 等。指定 §5.4 **实际在 PDF 第8页，非第7页**；第7页含图10与 §5.2–5.3。

§5.4 在 DRAM 控制器分三队：译址请求的 Golden FIFO 优先，随后 Silver，再 Normal；后两队用 FR-FCFS。Silver 轮额按各应用并发 walk 与 stalled warp 指标分配。它覆盖译址感知内存优先、应用反馈与公平；动作位置是下游 DRAM 排队。该段没有在上游单 PTW 启动前评估两个整 walk 的 AB/BA 联合执行、预测全部数据/R/B/fault退休或按两任务 max/sum 完成排序。因此不能以 MASK 单篇作为本轮整体精确覆盖证据；也不能把“译址请求优先”作为独立新动作。

## Scheduling Page Table Walks for Irregular GPU Applications，ISCA 2018

[原作者 PDF](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)；[本地原件](/home/xvhaocheng/work/iommu-platform/out/tl-memory-feedback-20261014/source-prior/ptw-scheduling-isca2018.pdf)。Seunghee Shin、Guilherme Cox 等；§III 在 PDF3–5，§IV 在 PDF6并延至7；§VI 在 PDF10并延至11。

§III/IV 按 SIMD instruction 聚批，优先预计较少 memory accesses 的组；PWC 查询估计 walk 工作量，walker 就绪时扫描 pending 队，平分取老者，加年龄防饥饿。闲 walker 遇新请求直接启动，正文并计入扫描延迟。它覆盖 ready walker 前选择、同指令全部 walk 完成、工作量估计/SJF/聚批/aging。§VI 跨 PDF10–11 的末句是：

> we leave such explorations for future work.

其指向更好的调度器协调。该处没有实现共享 DRAM 状态反馈的双顺序全完成预测；不排除其他前作。把译址排序本身、短组优先或批内优先当新贡献会被该文覆盖。

## 本轮只读裁决

| 判断 | 结果 | 原因 |
|---|---|---|
| 两篇是否整体精确覆盖本轮动作 | 没有获得该证据 | 已读正文动作位置、输入和目标均不同；通用可组合组件不是整体发表证明 |
| 是否因此可宣布独立创新 | 否 | 还缺具体同信息/全成本强对照差异、顺序反馈因果证据和研究范围内必要前作核验 |
| 是否因此立即封闭共享 memory 分支 | 否 | 这两篇不提供该整体覆盖证明；旧 §79B 的固定 W/序无关成本限制不能自动外推 |
| 是否需要在本席继续广搜 | 否 | 先由 root 裁决真实来源/控制差异门；没有新关键疑点时不扩检索面 |

PDF SHA、原 URL、下载字节数见 [pdf-provenance.json](/home/xvhaocheng/work/iommu-platform/out/tl-memory-feedback-20261014/source-prior/pdf-provenance.json)；带页边界的完整提取文本与原 PDF 同目录。原件用于审查，不把摘要当全文证据。
