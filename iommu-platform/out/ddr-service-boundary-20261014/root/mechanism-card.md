# 实际DDR服务边界机制卡（2026-10-14）

本轮登记实际goal1791941230，无额外token预算。接受历史Genesys2参考的外部服务边界及条件返回顺序因果；**两合法NormalTL队头的独有选择收益仍unknown，停止本轮进入双头来源搜索/A-B/预测器**。这一停止不关闭F1全族或论文主问题。已接受读不能被AR优先提升，不等于新机制已成立。

## 具体问题与证据边界

原候选F1只在NormalTL合流首次VALID前比较A→B/B→A，预测已拥有数据义务与历史估计的完整walk共同服务，按max完成、sum完成、年龄/RR选择；缺历史回退RR，连续一次越过后保留对方下一次许可。一个PTW、IoTLB16、原有容量、完整walk、命中旁路、同ID顺序及全部退休条件不变。

上轮有限S1排除依赖私有模型的普通DS优先和完整DS读服务。本轮核物理集成参考里该优先实际能做什么。来源是**一个已有构建目录**，不新选板、不恢复R107研究。构建清单与24个overlay输入、顶层profile纯内存变换、build-inputs及bit哈希一致；生成XCI选择MIG4.2，构建脚本read_ip和同目录funcsim的RD_PRI_REG/AR实例提供历史实例旁证。生成IP字节没有在build_start单独封存，未独立提取bit内部完整语义，当前加载板身份unknown。以下服务结论对钉住的生成参考成立，不冒充当前上板实测。

原路径：四DMA在IOMMU前合流；IOMMU完成source2与DS source3进入同一DDR目的端。crossbar是4个源端、12个目的端，localID4bit/fullID6bit，payload fullID32/36/40/44，PTW48、DDTW49。共享路径含原atomics wrapper、AXI64异步clock converter、MIG AXI64及其内部64→256 upsizer。Genesys2DDR分支无外部64→512转换。完整ID不降宽，QOS传递本身不提供队首提升。

MIG实例：physicalDDR32bit、nCK4、ECCoff、BL8、ORDERING=NORM、RD_PRI_REG。shim在4:1下C_MC_BURST_LEN=1，因此native data FIFO32条256bit单元；metadata FIFO30条native command信息加2级skid；upsizer32条计划记录不是32条可调度AXI读。不得把这些数当全burst容量或PTE墙钟延迟上界。profile CPU50MHz/DS50+50不是本轮时钟验收，不混算system与UI时域。

## 三个承诺阶段

| 阶段 | 普通外部AR优先实际能力 | 必须保留的限制 |
|---|---|---|
| payload未接受，且尚未在该仲裁点首次VALID承诺 | 该点同时拥有合法DS/payload时可以先offer DS；这是已有普通动作的可实现位置，具体集成/公平/选择逻辑仍计费 | 上游spill/CDC已接受或已held的payload不能称为“可自由选择”；尚无DS VALID时不能预知未来PTE。 |
| payload已被MIG AR接受，R未完整返回 | 只能选择之后未承诺的请求，不能撤销active读、改已存QOS或提升后来的PTE | active burst逐native command展开，下一active读须等本burst最后command；RD_PRI_REG只选读/写，QOS只作非零提示，无read-vs-read排序。 |
| MIG RVALID已held，RREADY=0 | 消费当前拍、释放空间；不能把当前拍改为PTE | RID/DATA/RESP/LAST稳定到握手。上游CDC/R spill held与MIG held分别核，不能把上游未消费误当MIG仍未返回。 |

生成源码的mc_app_hi_pri固定0；没有accepted读队列查询、取消、改优先或返回重排外部端口。native bank可重排物理命令，但UI恢复读顺序；不同ID也没有本实例的PTE返回越过能力。厂商对应说明：[AMD UG586 4.2 Reordering](https://docs.amd.com/r/en-US/ug586_7Series_MIS/Reordering)。RD_PRI_REG的精确行为取实际生成源码；对应厂商正文提取失败，不用旧4.1PDF代证。

## 一条条件因果链及相邻边界

只取合法已拥有的payload读L及同路径PTE读P。令MIG侧握手序列满足AR(L)先于AR(P)，P被接受时L尚余k>0个AXI R拍未握手。k来自L已接受ARLEN及MIG侧已发生的RID/R握手；不读隐藏row/bank/queue，不由任意满FIFO初态生成。

1. L的AR头被接受并稳定保存，native command逐项下发；在最后command前，后继P不能成为第二个可选active读。输入register/upsizer可能已暂存P，暂存不等于可重排。
2. metadata记录L在前、P在后。即使物理DDR先完成P，UI恢复顺序，返回FIFO/解包计划仍按L→P。
3. 在MIG AXI侧，P首次可见R必须晚于L最后R握手：`t(P.firstR) > t(L.RLAST_handshake)`。从AR(P)后到P.firstR前至少还要完成k个L拍握手，每拍最多一次。这是**顺序/服务机会下界**，不是k个system周期、总时延差或平均DDR延迟。
4. 当前DS AR优先、RR/年龄、QoS及held-R消费不能撤销这笔已接受读债务。读写仲裁能改变native读命令的服务机会，不能改变这两个读的外部返回次序。

相邻边界是k=0，或P先于L被接受：该L对P的上述正前序债务不成立。没有改变任何容量/负载/返回策略；这是合同条件的逻辑反例，**不是新运行的正反条件**。若reset/错误改变退休边界，必须另核完整错误终结，不能把此正常读命题扩展成所有故障的周期定理。

**缺口：这条链只证明服务边界，不证明在实际共同就绪的两合法队头中，A/B选择能改变P前面的k及双方完整完成。** 在选择时已接受的公共L债务，对两个顺序都不可撤销；不能把同一公共债务改记成候选收益。选择之后可能释放的读/写义务、下一PTE出现及全任务依赖，需要正常来源及已有所有权关联证据。本轮没有这种源证，也没有独有选择差异。

## 冻结普通强对照与覆盖边界

九项完整在线合同沿用[已有baseline-contract](../../tl-memory-feedback-20261014/ordinary-cost/baseline-contract.json)：O-RR、O-FCFS-age、O-owner-block、O-short-TL、O-debt-min、O-debt-max、O-release-density、O-turn、O-two-stage。持锁/公平/缺历史回退优先，所有规则可获得相同**已付费**可观察信息，保留现有命中旁路。每场景事后取最佳普通策略只属离线上界。

- 外部DS优先实际版本是承诺前AR选择，不是accepted队列提升；它在上述L已接受状态无动作可撤债，但未接受时可避免L先行。没有双头反事实，不能排除该普通提前选择、owner/欠账规则覆盖全部候选选择。
- O-debt-min/max按既有已拥有R/W/B关联计数，不能仅看bank名或把未来payload算免费债务；本轮没有同一来源上候选与其选择不同的证据，也不宣称它们完整覆盖F1。
- O-turn保持原在线经验规则；旧统一串行turn定理不适用于当前实际DDR，不能继承其数学收益。O-two-stage只在独立固定两阶段域成立，共享DDR域未证明，不借Johnson作全面否决。
- MASK/ISCA2018的PTE优先、工作量排序/分组覆盖边界沿用既有原文核查，本轮不复核/重跑；它们未直接证明本F1全族已发表覆盖。本轮无新增前作创新接受。

## 所有权与完整成本

NormalTL完整118bit请求及185bit响应、active-walk owner、native source/target/sameID、W输入和输出owner、raw/physical AR/AW、Rspill、B、错误路由全部继承；不新增payload槽、walker或credit。选择必须在首次NormalTL VALID前提交，held grant/header不能随历史更新。R-last、physical/error W-last、Bcapture、sourceB及最终quiet不同，不能只测TL结束。

沿用[37分类成本接口](../../tl-memory-feedback-20261014/ordinary-cost/cost-interface-card.md)，新增必须出现的项目为：两头age/历史class与valid/overflow；per-head真实obligation关联、AR/R/W/B计数及timestamp；PTE完成计数和service反馈；跨clock事件CDC与一致snapshot；反馈读写端口、多事件更新/匹配；两顺序虚拟状态与max/sum算术、选择mux、fairness、选择延迟和关键路径；历史获取的实际字节/时间及可能CPU/ABI。

公用事件观测可共享但只扣真实仍有生命周期的字段。native RR不强加unused历史；复杂预测不免费拿host trace或MIG内部队列。MIG侧k精确监视需要按fullID/接受次序跟踪和R退休反馈；聚合bank欠账不能替代这个顺序。系统侧只有AR接受而不知道CDC中的PTE/数据次序，也不能推出MIG侧k。

如要额外实现accepted读重排，须新增请求/返回tag、选择器、有限credit和缓冲、same-ID/依赖、heldR稳定、公平/写服务/错误/reset，并改变MIG集成；这不是现有QOS的一根免费线，也不属本轮原候选授权实现。没有实现总bits/ports/latency、面积、Fmax、CPU或应用测量，**全部null，不记0；性能成本等价未接受**。

## 接受、停止与唯一后继

接受：已有构建的来源/bit字节绑定；钉住生成参考的三阶段外部优先边界；L在先且k>0时的条件顺序下界。

不接受：当前live FPGA等价、自然双头可达、same-PTE等待净差、候选独有选择、双方完整收益、数字完整成本差、已发表新颖性或FPGA/应用收益。停止理由是**缺少“合法队头选择改变前序债务并超出一个冻结普通规则”的完整反事实**，不是假设所有普通规则都能复制候选，也不是根问题不可做。

下一输入限定为一个现存正常来源的原始事实/调用链，指出在两合法队头共同就绪时，哪项已经拥有的不同义务会随首选次序进入下一PTE之前，并给一个冻结普通规则不能复现的完整选择差异。证据出现后才核该一个安全双头来源；两关过后才一对A/B；完整收益及费用存活后才冻结≤4条件，随后GPU/AI实际共享路径与RTL/FPGA。没有新原始事实不自动扫相位、造压力或继续搭预测器。
