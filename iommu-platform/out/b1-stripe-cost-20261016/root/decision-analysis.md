# B1单条带成本卡：本轮接受与下一步

2026-10-16 JST；B1-STRIPE-COST-1。ARS-Codex有界源码核查、协议与成本推导；Codex辅助完成，不是完整pipeline、系统综述、原生应用结果或硬件测量。

**所选单条带卡已完成；当前B1-D实例转为待具体残余/成本证据，不进入新模型或RTL实现。暂不选论文主线。** 这次没有证明普通预热已经足够，也没有证明B1思想族无价值；实际得到的是更准确的来源、一个容量边界和公平的存储比较。下一步不重复写同一张卡或自动再做六族重排，只有能改变该判断的具体输入/实现差异才进入后继。

## 1. 这次新增了什么证据

| 决定问题 | 已核结果 | 不能推出什么 |
| --- | --- | --- |
| 哪个槽被占用、何时可搬B2 | B0/B2用槽0；t0计算隐式barrier释放；t1提交B2，之后计算B1以遮蔽B2搬运 | 地址晚知、准备等于payload许可、一次wait差额就是整任务收益 |
| 来源是否现成SVA | 实际启用copy-based device1；SVM device0为if(0)。外部地址b与host malloc的关系由未核runtime决定 | 当前Linux原VA/SID绑定、现成共享IOMMU实际工作负载 |
| 本窗口有多少普通外部来源 | A0/B0已wait；A1/B1可能仍在途；s0没有C DMA。4KiB投影A1∪B1∪B2至多14页，16项全共享IOTLB能容纳这个集合 | 所有页必须同时驻留、PLRU必保留，或所有源场景都不缺容量 |
| 结果池实际占什么 | 当前job ID仅32bit；一次性表可共享context/span并按连续页索引，有限pending/ready/fault都收费 | 免费未来descriptor、通用ATC不能用相同压缩、逻辑位数就是面积 |
| 能否直接用debug的PPN | debug有单份48bit返回；当前translated-context的iDMA仍发untranslated类型。复用准备结果须有合法采纳/权限/有效性路径 | 改DMA地址已绕过翻译，或因缺接口就证明创新 |

root直接核了应用、类型、TC、TL、debug、IOTLB与iDMA backend关键原文。详见[单条带卡](../stripe-source/findings.md)、[root残余与容量推导](residual-and-capacity.md)、[存储/端口卡](../storage-cost/cost-card.md)及[来源清单](root-source-manifest.json)。子席意见一致不算证明。

## 2. 成本现在能算到哪里

默认条带16KiB，若4KiB且页对齐，N=4；否则N=5。真实映射/对齐未知，可行大页和物理连续性同样给普通方案。参考表头`H=157+E+W+G`：包括64bit原span地址、52bit输入context/access、38bit解析key、3bit控制，以及epoch E、路由W和generation G。这个编码可压缩、不是全局最小值，双方共享同样的回收机会。

在“单span、固定4KiB、同一访问类型、首故障终止准备且排空已承诺工作”的共同参考合同下：

| 组织 | N=4的主表示位 | N=5的主表示位 | 还需计入 |
| --- | ---: | ---: | --- |
| B1 / 普通O-PAQ-D / 同scope紧凑local ATC | 343+E+W+G | 390+E+W+G | last-use证明、epoch管理、路由/队列/保持、接口与物理实现X项 |
| O-PIN参考编码 | 238+E+W+G | 241+E+W+G | 同类X项；还占4/5个已有IOTLB way，即688/860逻辑位的共享资源 |

这些数字是可重算的参考表示，**不证明完整协议已验证或某方案面积/净收益较优**。O-PIN的引用管理、替换排除、失效与回收端口尚未实现；同scope ATC可以与B1采用同样表头和ordinal，不能拿一个通用ATC逐页复制所有字段来制造B1优势。原32bit job ID可以作handle，仍不能容纳表体；若借TC无用字段，须证明原owner、整体response写入和读写口不冲突。

当前IOTLB主tag/data/PLRU声明共2767逻辑位；当前TC request/response/FSM为306逻辑位，不含FIFO，综合可能常量消除。它们不等于可自由分配的空闲空间。准备返回写、采纳读、release/free、invalidation可以串行共用bank，但会增加仲裁等待；直接接ready恒1的debug必须可靠捕获返回。未综合、未布线、没有周期/功耗/总面积数。[逐字段、端口和故障合同](../storage-cost/cost-card.md)。

翻译表可在本次B2最后一次需要某页结果的物理请求被真实接收后释放；无需强迫普通pin留到B2本地计算或整个矩阵结束。原TC只知道当前Ax，不知道future parent剩余字节；把最后用途信息从原生DMA owner可靠传到释放点，仍是明确的未知成本。表放endpoint可能便宜，但要核实际接法；这是可发展的集成问题，不是本轮已证优势。

## 3. 最接近工作与可保留的新意

- Kurth等，*Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine*，ICCD2018：§IV-A已有worker进度限定纯翻译预取，§IV-C已有逐burst失败跟踪/重放与缩短整DMA锁定限制。不能把“SPM忙时先译”“按最后用途归还”宽称新。[原文定位](../../research-reprioritize-20261015/prepare-resource/evidence-map.md)。
- Koenig等，*Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs*，DATE2025：PTE LLC是需要允许的普通设计，论文计时不含host offload/sync；其结果不直接代表当前完整生命周期。[原文](../../shared-translation-literature-20261006/papers/riscvsva2025.txt)。
- RISC-V IOMMU v20260222的ATS语义已有翻译响应、权限/粒度、ATC保存及后续translated访问；本轮重核保存的正式原文局部，不声称精读PCI-SIG全文或当前端点已实现ATS。[原规范保存件](../../independent-directions-20261015/ownership/sources/riscv-20260222-data-structures.txt)。

因此保留的贡献命题是：**若**一条来源明确、假设冻结的正常span在允许普通预热/PTE cache/合法pin后仍有完整目标上的残余，且一种结果持有/交接组织用明确的字段、端口和有效性接口改变部署能力或完整成本，便可形成接口/集成贡献。最简实现可以就是O-PAQ-D；不要求所有构件首次出现。当前没有这个残余或成本点，不能用新命名、缺接口或免费存储代替证据。

同接口普通构造只说明这套动作可如此实现，不证明该架构已发表或整个B1被覆盖。本轮也没有全面核查更一般的ATC压缩文献，因此不作全领域新颖性断言。

## 4. root裁决与精确再进入条件

本次**来源卡、普通残余卡、存储/端口/生命周期卡均接受为有边界的研究产物**。不接受“已证驱逐/关键等待”“B1节省真实硬件”“普通方案已满足全部目标”“已形成论文主贡献”。本单条B2实例尚不足以支持追加新模型、adapter或RTL建设；把这次证据投入结题，不为维持方向继续扩矩阵。

后继只保留一个触发合同：[B1-REENTRY-WITNESS-1](next-minimal-contract.json)。它接受测量或明确的source-bound投影，不强制native：固定目标侧地址/leaf/context、合法请求与预热/消费顺序、普通方案后的残余及完整后果；**或者**给同scope组织的具体owner/端口/失效/最后用途接法与可核费用差异。未知输入不能填造成测量；不能任意添压力流、缩减基线容量、限制普通回收来制造差异。这是输入/设计触发条件，不是新增许可流程；既有用户授权保持。

I仍not_ready，D原下一任务版降优先级、新补交版未升位，A5/A2与Q/G/C2资产保留，暂不选论文主线。不会自动回退A/I/D，也不自动开启新一轮六族总评。10月15—28日立题窗口与MIG暂停不变。

本轮只有定向读源、表示与几何算术核对、研究产物和入口同步；0新机制/服务模型/源程序实验，0旧实验重跑、构建、生产Linux/RTL/ABI编辑、联网与硬件操作。请求配置：源卡Sol/high、成本推导Sol/xhigh、文档Sol/medium；root不声称原位切换，实际独立模型/强度元数据unknown，未用DeepSeek/Claude。

稳定接受与后继边界见[root接受](root-result-acceptance.json)、[源验证](source-verification.json)和[最终交付核验](delivery-verification.json)。
