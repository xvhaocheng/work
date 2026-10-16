# 新一轮正向设计与投入裁决

2026-10-15启动，2026-10-16（JST）交付；RESEARCH-REPRIORITIZE-1。ARS-Codex有界问题发展、前作实质比较与证据规划。不是全pipeline，不声称完成系统综述或论文。共享研究Codex-only。

**本轮不选替代论文主线。下一笔有限证据投入选B1的一条未来条带残余/存储卡；这不是认定B1最有论文前景，也不是恢复旧B1实验。** 选择理由是它已有具名正常地址/依赖窗口，能把“需要新接口”收敛为一个可判别问题；I还缺合法稀疏批与live预算，D-live这次复用轨迹未给当前PTE保护正例，A/G若现在开工则会先扩大抽取/工作流工程。现有资料不足以给这些方向的论文成功率或净收益排名。

本轮确实做了三项不同的正向版本：B1-D一次性翻译结果交接、I-SET32精确稀疏页集失效、D-live固定模式下的合法补交时机控制。另认真展开Q-S，回看A/G与C2-P1；不是照旧A或D叙事继续。各版都有新增能力、具名普通对照、条件正例、相邻反例和完整成本。子席意见不代替root原文和原始轨迹核查。

## 1. 三个版本，分别新增什么

| 版本 | 真实来源与正向设计 | 最接近工作/普通强对照 | 本轮关键新事实与裁决 |
| --- | --- | --- | --- |
| B1-D：准备结果晚交接 | HERO mm-large计算当前B条带时可算更远地址，但SPM仍由当前计算占用。把future span的合法翻译结果保存在有限一次性资源中，等真实release及接收能力成立再交给准确owner | ICCD2018进度限定纯预取、ATS/ATC；现有debug预热、PTE cache、只pin尚有用途的页、普通有限PA结果池O-PAQ-D；同样允许字段回收 | 真实对象只有32-bit job ID，t+2尚未提交，没有免费结果表。默认条带16KiB，在假定4KiB映射时需4/5页；真实粒度/对齐未知。保留接口/部署成本问题；只先核一条残余/存储卡 |
| I-SET32：有界稀疏失效 | 正常TX完成后撤销映射可能需要同时压低命令费用和live collateral。拟议一个base+mask命令准确清选中leaf，额外约定本命令不清不相交leaf | IDYLL稀疏offset合并、z-READ批量bitmap；正确精确命令同批发布、可行range/DP、同面积combining、SG/arena/持久池、合法分domain与FQ | 物理离散不代表IOVA离散；map_sg已可连续分配。串行strict调用不能免费攒成批。正常同context稀疏批、live余量与命令前端瓶颈仍缺，I维持not_ready |
| D-live：固定mode下决定下一代补交 | SPDK成功callback后重用task/buffer；在新一代尚未submit前，立即补交或进入有界ready队列。避免旧版joint quiet/config/clear造成的跨界问题 | MASK及已核干预/反馈工作；Arcus流量整形；普通QD/pacing、公开完成历史优先、轮转、同费用试选 | 延后适配器是新增普通软件，动作本身不算创新。已有两条COPY投影均未显示新A实际访问时旧V PTE仍在途，不据此启动collector/延后矩阵或恢复D主线 |

完整提案：[B1/Q](../prepare-resource/proposal.md)、[I](../invalidation/proposal.md)、[root A/G及D-live](consumer-and-refill-design.md)。这些是设计与可推翻预测，未实现或运行新机制，不能写成性能结论。

## 2. B1：保留最强合理版本，但撤回免费存储理由

直接源码支持future地址先于SPM覆写许可可知；两者是不同对象。它**不证明**普通预热之后还剩完整任务关键路径上的翻译等待。`hero_dma_job_t`仅有`uint32_t id`，异步helper写寄存器后读ID启动，不能把可计算地址当成已存在的描述符/多页PPN存储。[源定位与hash](root-source-manifest.json)绑定mm-large:164–228/284–310、helper header:72–74和实现:58–88。

ICCD2018 §IV-A已有按worker进度纯翻译预取；§IV-C已有逐burst重放与缩短锁定期。DATE2025已在其平台展示PTE LLC的重要性，且未计host offload/sync；不把论文数字移植成本机残余。[已核原文定位](../prepare-resource/evidence-map.md)。ATS把翻译和后续访问分开，结果持有/失效也不是空白。

因此可发展的贡献是：在一条有来源的future span上，证明允许的预热/PTE cache后仍存在有意义的残余，再以具体片上结果组织取得更好的完整成本或部署能力。**不要求每个构件首次出现，也不要求候选击败在同一新增接口上实现它的普通程序。** O-PAQ-D如果就是最简实现，应诚实作为候选实现；必须收费，不能再宣称新调度算法或省去必要结果存储。与现有debug/PTE cache/local ATC的具体差值，才决定是否值得建设新接口。

本轮没有证明残余、面积、时钟、完整任务净值或一般异步撤销协议。下一卡必须允许“普通方法已足够”“输入未知而无法决定”，不能靠添加无来源压力流、任意缩小cache或冻结不利映射制造正例。[下一最小合同](next-minimal-contract.json)。

## 3. I：比“稀疏”这个名称更具体的能力与费用

I-SET32是概念扩展，不是现行ISA能力。仅host single-stage、non-global 4KiB leaf、稳定上层、无ATS；mask清选中页，非选中保留是额外付费合同。RISC-V允许扩大失效，格式不会自动提供保留保证。八个不连续页的16B命令算术可从144B降到32B，但这不是实际fetch、执行延迟或收益。[设计与边界](../invalidation/candidate-card.json)。

root重核ixgbe调用consume API在unmap之前、strict的sync→free_iova顺序，以及map_sg连续IOVA。consume API不证明此刻最后物理free；也不能把所有payload都收费到fence结束。该RISC-V leaf unmap读取范围只清PTE并gather，没有逐次回收页表页的证据。strict批来源必须在首个返回之前已经合法可用；不能偷偷延后返回收集后续串行调用。

IDYLL §6.3已有base+多个offset合并，但收到请求即TLB失效，延后的是PTE维护。root另取得z-READ原论文的§IV-B.2/Algorithm3局部正文：它已按批32页、base及bitmap组织CPU TLB shootdown，接收端对选中VA做处理；所以“bitmap+32页合批”不能认领新意。它不等同一个硬件IOTINVAL命令及其完成/保留合同，不能据此宣称整版撞车。[论文正文载体](https://www.researchgate.net/publication/386005926_Design_and_Implementation_of_Efficient_and_Transparent_Zero_Copy_Read)。

worker当时全文入口失败、方法未核的记录保留；上述是root后取得的**局部方法核读**，未下载完整PDF，也没有全论文覆盖/hash保证。[取得范围及失败收据](sources/zread2024-root-retrieval.json)。SET32若要继续，须有同批普通精确串/combining与可行IOVA布局仍达不到的真实费用点；当前没有这一输入，不因D退位提升I。

## 4. D-live：合法动作成立，不等于信息有价值

stock SPDK的成功callback确有新一代submit前的位置。bounded ready队列、延后与最终释放是新增适配，不是stock特性；保持固定mode可绕开全owner quiet与IOTLB clear。普通pacing本来就允许，创新只能落在同信息强对照之后仍有及时关系信息改变完整决策，且覆盖全部费用。

针对这个新版本，定向核读Arcus §4.1–4.3：它已把软件提交与实际DMA供给解耦，以per-flow硬件token bucket整形，再用profiling和runtime反馈调参。因此供给解耦、token bucket或反馈整形不能泛称新意；本候选尚待证明owner-bound PTE—payload关系的增量。该局部核读不证明Arcus覆盖所有共享IOMMU问题。[Arcus v1原文](https://arxiv.org/html/2410.17577v1)、[本地范围/hash](sources/arcus-retrieval.json)。

本轮真正新增的证据是对已有P、PN各一条COPY warm轨迹各做一次完整只读提取：共4,331,570事件行，各256次合法A补交，排除各一个末尾drain。cfg读取日志之后还需一tick函数才返回；不能用更早hardware DONE作软件已知。源码V先poll/refill，A后poll/refill，未改变旧guard或gap。[提取程序、实际执行与结果](../refill-window/brief.md)。

| 检查点 | P | PN |
| --- | ---: | ---: |
| 合法补交决定时V busy | 256/256 | 256/256 |
| 决定时任意已接受V PTE尚未回调/实际R消费 | 1/256 | 1/256 |
| 下一A首物理访问时任意V PTE pending | 0/256 | 0/256 |
| 下一A首payload时任意V PTE pending | 0/256 | 0/256 |

唯一旧PTE在软件最早返回时还剩60模型tick，到下一A metadata开始前已结束253tick；到首payload前，P/PN分别已结束1061/2158tick。root核提取代码、cfg返回点和两条原始UID见证；PN首payload是8353tick，不能沿用P的7256。[机器结果](../refill-window/analysis.json)。

这只说明“延后新A来保护决定时那笔旧PTE”在这两条来源轨迹未取得有效正例，足以降低**这个实施版本**的投入；不是反事实延后实验，更不是控制无效证明。busy不等于walk pending，检查点没有PTE不等于整个walk结束；后续V代际身份未快照，仍unknown。该COPY投影也不等于NVMe native。没有新信号接口、适配器、延后模型、信息价值或完整控制收益。

## 5. 六族其余版本与下一步

Q-S被发展为在当前不可撤销PTE退休后保存有限frame、让demand先服务并保证prepare进展；DWS是whole-walk借用，不能说它直接给PTE边界。但具名O-CKPT已表达所提动作，未找到新成本点。root直接核TC AWAIT_RESP恒ready，排除了payload背压钉住共享TL响应这个假来源。只停止“把该pause称新机制”的版本，不关闭动态Q族。

A可发展为跨端点完成域的可靠抽取/部署方法；当前A5手工384jobs与标准DAG一致，没有未见抽取结果及实际部署后果。G可发展动态消费者前缀释放；仍缺具名正常动态输入。C2-P1可问有限返回资源下的有用完整服务界；不能把任意到达包装成超出TC2023，也没有正常满返回阻塞见证与可复用分析增量。均保留已有结果，不因现有资产多而优先，不因一次负结果否定全族。

下一合同为**B1-STRIPE-COST-1**：仅一条正常future stripe，固定源事件、输入假设、允许普通方案、结果归属和物理存储/端口，回答是否有值得实现的残余与具体成本点。先用现成源码/已核材料；缺项明示，不先建大模型、不新采硬件、不恢复旧实验。若普通方法足够或无法形成有来源的成本问题，就结束该次证据投入；后继由该结果决定，不自动回到D/I/A或再开六族总评。

**这是下一证据工作的选择，不是B1的论文最优性结论。** I须由合法稀疏批/live预算/前端服务包触发；D-live须由新且及时的具体关系/控制窗口触发。10月15—28日窗口、A5/A2、所有历史正负证据与MIG暂停保持。

## 6. 执行、责任与适用范围

新机制/源程序/服务模型运行0，构建0，旧实验重跑0，生产Linux/RTL/ABI修改0，设备/FPGA/MIG操作0。本轮有只读旧日志分析和两项定向前作取得；不能沿用上轮“0联网、0分析”的总计。原始CSV未修改，方法脚本与收据均在独立输出目录。

请求配置：开放B1/Q设计Astra/xhigh，明确I机制设计Sol/xhigh，普通日志提取与文档Sol/medium；第三设计席两次因thread limit未创建，root接手A/G与D-live；实际模型/强度独立元数据均unknown，不用请求值冒充运行证明，不重跑完成席。未使用DeepSeek/Claude。子席中的全局授权推断不作为本轮裁决；“未进入后继”不等于用户未授权，既有授权和暂停边界保持。

稳定机器接受、源核查、下一合同和最终文档保护回执见[接受](root-result-acceptance.json)、[root源清单](root-source-manifest.json)、[下一合同](next-minimal-contract.json)与[交付验证](delivery-verification.json)。冻结旧六族报告作为历史输入，本报告是本次新事实之后的判断。
