# BR1：准确前作、普通覆盖与候选等价实现

2026-10-14。定向原文核查，不做全面查新。仅新打开root指定的两个primary PDF；MeDiC/MASK/ISCAwalks/DAC/DEPOT继承RCS1 root原文裁决，没有全面重查。B7是本席给出的固定图普通构造及数学证明，不声称已找到一篇完整发表的BR1原作。

## 1. 两个新定向primary证据

| 原作和精确入口 | 原文决定证据 | BR1边界 |
| --- | --- | --- |
| [Qureshi et al., A Case for MLP-Aware Cache Replacement, ISCA2006](https://users.ece.cmu.edu/~omutlu/pub/qureshi_isca06.pdf)，PDF p3 §3.1 Algorithm1，文本lines233–255；pp5–6 §5／§5.1，lines566–573、621–650 | 每周期按同时outstanding demand misses数N给每个miss累计 `miss.mlp cost += (1/N)`；完成后把成本存到block tag，LIN victim最小化 `R(i) + λ · costq(i)`。 | miss数量不等于stall收益、历史暴露成本用于replacement已是明确前作；BR1不能把这个动机当首次贡献。所核program是历史MLP成本加recency，不是当前真实DMA owners至下一release的victim-sensitive服务重排投影。MLP与criticality不同亦在p3 lines215–225明确区分。这个有限program比较不构成穷尽不存在结论。 |
| [Mirbagher-Ajorpaz et al., CHiRP: Control-Flow History Reuse Prediction, MICRO2020](https://microarch.org/micro53/papers/738300a131.pdf)，PDF pp5–7 §IV A–E，Fig5 Algorithm，lines435–470、477–523、528–592；作者由p1 lines1–19核实 | 历史signature预测TLB entry dead/live；victim程序lines513–517：`if entry.isDead = true then return entry`，否则 `return LRUentry()`。 | TLB历史复用预测、dead entry优先和紧时序的metadata更新不是BR1新能力。所核program以复用history/signature为信息，未表达当前owned R/W/B、release DAG或下一释放max/sum/PTE。把next-use普通对照称CHiRP逐程序复现也不准确；BR1必须承担自己的登记和同步成本。 |

原文quoted片段每source低于25词；额外保留精确页、节、算法行及web文本位置，不复制完整文章。上述比较属于从原program推导的机制边界。PDF第一页有名称可直接校验；以链接原作支持，不把secondary摘要当原文。

## 2. 继承证据，只改其与本问题的关系

继承入口：[RCS1 prior/ordinary/closure boundaries](../../rcs1-completion-set-20261014/root/prior-and-closed-boundaries.md:5)。该文件给出原作URL、页／节及已核程序：

- MeDiC PACT2015：warp group进度、hit fraction分类、insertion priority和bypass已存在。BR1不能把“顾及group完成”当新贡献；本轮fractionInsert是当前U信息的新普通adaptation，不是原MeDiC采样程序。
- Scheduling Page Table Walks ISCA2018：同一SIMD instruction等待其walk集合，按instruction成批／估计walk work排序已存在。对本轮SJF-horizon动机有准确覆盖，walk scheduling和normal-fill victim是不同动作。
- MASK ASPLOS2018：基于miss信息的TLB fill资格和单独bypass cache已存在。本轮不能免费借32-entry bypass cache或直达通道。
- DAC2023 orchestration：miss rate指导任务放置，task-block标识用于partition/sharing，completion更新已有。不能把task-aware allocation/feedback本身当BR1首创。
- DEPOT 2026 primary preprint：最近evicted VPN及reinstallation保护、all-protected普通fallback已有。历史复用保护与当前release因果投影有program区别；preprint身份及本平台未证收益继承。

这些继承原文没有在本席重新打开或重新运行；不新增“已发表准确BR1”的断言。当前未检索新的criticality/slack论文，因为具体coverage已由固定程序和条件证明完成，不需要假借一个未核作者题名。

## 3. 三种裁决必须分开

1. **准确前作覆盖：** 如果原文明确给出当前owned-release horizon、相同信息、相同victim-dependent共享服务/依赖投影和目标程序，则可否决这项机制的首次提出。上述两个PDF没有给出这个完整程序；不能因为它们已做cost/reuse就作更广否决。
2. **具体普通coverage：** B7在固定单节点DAG族以一次forward/reverse slack计算精确给出所有owner F，与BR1全victim目标同序；B8在完整release H可保留及后续fill不丢H的条件下给出零影响victim，另外需要固定服务／单调score条件和完整tie条件才精确覆盖BR1最优动作。它们是完整固定程序及条件证明；若root新场景全部在相应族，就不能把那项收益称BR1独有，即使未找到完整发表原作。新增miss在动态sharedFIFO中可能通过流量整形改善其他owner，不能从Hpin baseline推全域单调性。
3. **候选等价实现：** EQ-BR1逐victim运行同一个候选projector，是BR1的实现方式。单纯“普通控制也可以照抄这个投影”既不是准确前作也不是独立普通覆盖；应在成本账中允许该合法实现／化简，不能用它制造无穷对照否决。

B7未覆盖的一般族只有条件性描述：victim改变合法PTE/data到达及服务顺序、多个受影响节点、反复lookup/cache state变化。它不是已经建立的BR1优势。要进入模型，必须有root接受的真实cold event chain、同信息program差异、完整成本及headroom；本席不授权实验。

## 4. 范围和未解项

原RCS1四条件性能关闭、CO1具体普通覆盖、R2/R107/C112及其他关闭分支保持。没有复用旧51copy作新条件、复制旧cache snapshot、增pressure、warmup、phase或人为delay。

当前未解：H是否真覆盖到下一release的全部shared用途；正式key数量和legalmask是否保证free/outside-H；projection对后续fill究竟用何program；服务/d估计公开端点及误差；真实snapshot/owner反馈是否可用；最强普通程序与BR1实际decision/CPU/时钟成本。这些归root／witness独占任务，本席不核生产源码和native count。

**root新增的source证据纠正继承边界：** `src/idma_fullbuffer_certificate.sv:16–18` 只在CapacityBytes至少4096、normal、长度4096、source/destination均页对齐时给eligible；`dma_core_wrap:173–183` 经实际backend握手／job_rsp关联，`fpga:1466` 至TC。RCS1旧64B fullbuffer许可属于conditionalengine外加能力，不能引用其“native”措辞作为BR1普通或候选的免费真实接口。旧模型数据和性能关闭仍保留，不重跑；生产原guard与真实4096B证书约束适用于本轮全部控制。原文核查归root，此处准确继承新证据，没有第二次扫描。
