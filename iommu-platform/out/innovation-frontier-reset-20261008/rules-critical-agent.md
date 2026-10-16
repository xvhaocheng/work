# §44 Native Codex：真实翻译约束、两个候选及强普通反驳

2026-10-08，`innovation_rules_40`。本席亲自设计、推演并直接核关键RTL及primary来源。只读源码和浏览原文；未改RTL、未build/run、未重核§43同一证明、未调用DeepSeek/Claude。root负责最终阶段审定。

## 结论与导航

本席找到两项**真实接口限制**，但没有找到能超出同信息、同资源强普通方法的独立规则。因此不建议为下面两个候选启动新DUT：

1. [两阶段walk的有限释放与checkpoint成本](#候选一两阶段walk的有限释放与checkpoint成本)：整walk份额的时间粒度确实过粗；Sv39两阶段全冷正常路径最多15笔PTE读。可是PTE返回后保留tracking状态、重新仲裁下一层已有明确前作；普通checkpoint或restart可给同一释放保证。
2. [故障记录的短期容量背压](#候选二故障记录的短期容量背压)：当前TL ERROR确会等待fault_ready，内部记录存储满而FQ正写内存时可延迟无关普通译址。不过已有FIFO脱耦，标准允许满/错误时丢记录及相同fault报告合并。普通信用预约、去重、结果/日志分开存储均可覆盖提出的规则。

这里的增量是将下一判别从“通用walk公平/返回义务”收紧到两个可核查合同：**以PTE事务为单位的释放粒度和完整续走状态成本；故障日志究竟保留多少信息**。这不是机制新颖性或论文主贡献认定。

已与map交换：map核正常TC AWAIT_RESP始终ready，停止慢subscriber钉住共享翻译结果的候选；本席不把新coalescer自造的背压当当前平台真实问题。root另查CQ/invalidation，本席不重复。

## 0. 先排除没有实现的约束

### 0.1 当前PTW没有A/D原子写回

[PTW默认输出](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:264)把AWvalid/Wvalid置0，整个正常PTW FSM没有置1的写阶段；[490–497](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:490)在A=0或store且D=0时报page fault。

官方规范的SADE/GADE能力允许另一种实现进行原子更新，但并不证明当前RTL有这种不可撤写义务。[RISC-V IOMMU数据结构、AD更新能力](https://docs.riscv.org/reference/iommu/v20260222/iommu_data_structures.html)。因此不能用“PTE的A/D写尚未退休”拯救旧预算闭包或提出当前平台新调度。增加该feature是工程扩展，不能将新增feature自身制造的依赖当原问题。

### 0.2 已accepted正常译址结果没有慢consumer回压

map及root直接核TC `675–702`：正常TL请求握手后，TC AWAIT_RESP无条件assert译址response ready；结果被存入TC，AW/W的阻塞位于此后COMPLETE。当前data retirement不反馈TL result-ready。故“一个停流subscriber阻止共享结果释放”需要不同接口才成立，不能用普通AW/W迟滞代替此条件。

### 0.3 本轮从静态正常数据路径转向新配置，而非声称已有运行验证

§43实际运行是静态DEFAULT copy，TL66、miss34、PTW102、fault0。下面两阶段和fault情形有RTL路径、协议/软件意义，**没有在§43运行中出现，也未由本席运行**。两阶段的真实用途是guest自行约束设备IOVA且host再约束GPA；fault用途是错误descriptor/越权地址隔离。没有证据表明当前Linux已跑这些负载，不能把合法可配置路径说成已上板场景。

## 候选一：两阶段walk的有限释放与checkpoint成本

### 1.1 我提出的具体规则

欲将一个正在服务A的长walk从“整walk结束后可换flow”改为“当前已offer的PTE读退休后可换flow”。输入为：已accept译址请求身份/context、当前PTW stage/level、当前PTE AR首次VALID/accept/Raccept、预算暂停或V的新需求、可用续走context槽。

状态至少区分：未offer下一PTE；已firstVALID未ARaccept；已ARaccept待R；R已处理并有下一PTE指针；最终译址/fault产生但reply未交付。输出为下一PTE request及其身份、保存/恢复续走context、正常或fault response；不得撤已offer VALID，不得把A请求当作已完成丢弃。

在R真正返回并处理后、下一AR首次VALID之前，可以保存A续走状态并服务V。若pause发生在某AR已firstVALID之后，先完成那一笔，停发下一笔。A稍后恢复，不以永久停预算结束任务。

这条规则可以仅释放共享DS requester/串行walker执行槽，同时让A的请求身份与续走metadata留在私有或共享context存储。不能人为要求A parent必须退休才算释放，也不能只看PTW state=IDLE而不看held AR/R与reply。

### 1.2 为什么问题真实：最小长尾见证

生产[PTW `316–398`](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:316)支持STAGE_2_INTERMED、STAGE_1、STAGE_2_FINAL；MEM_ACCESS发一笔AR后进入PROC_PTE，收到R才生成下一地址。[418–478](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:418)明确intermediate G-stage leaf后读S-stage PTE，S-stage leaf后再做final G-stage。

假设device/process context已命中、两阶段均三层、全4KiB leaf、无页表缓存命中/错误/MSI。A的一次冷译址必须：

- 三个S-stage PTE，各自先做最多三个G-stage PTE读，再读该S-stage PTE，共3×(3+1)=12次。
- 最终GPA再做三次G-stage PTE读，共15次。

V是另一flow的一次正常单阶段冷译址，仅需3次。令每笔PTE事务在同样正常内存服务下耗时T，无人刻意饿死或阻塞内存。A开始第一笔后V到达；整walk不可抢占让V的起步等待可接近15T，每PTE checkpoint则至多等当前一笔剩余T，再服务V。常数FSM/仲裁开销另算，不把步数直接声称实测周期。

它反驳的只是“限制foreign walk数量=限制相同大小时间片”。HPCA21 DWS说owned walker通过idle stealing使owner新请求至多等一个foreign walk，并没有将一整个nested walk等同一笔PTE或给任意memory固定周期。因此这是细化适用条件，不是说DWS自己的保证错误。[DWS正文p1/p4](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)。

### 1.3 不能遗漏的metadata与不可撤义务

[PTW `90–158`](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:90)有main/s1 level、stage、IOVA、intermediate/final GPA、S-stage leaf、PTE物理指针、global/implicit/fault状态。TL还有request DID/PID/context/permissions及最终IOTLB update/fault需要的字段。PTW的`update_o.vpn/pscid/gscid`还引用输入信息；仅保存一个PTE指针然后覆盖TL live request不正确。

不要求每checkpoint保存所有寄存器：可在选定phase保存其必要子集，并从仍保留的原请求/DC/PC恢复其他字段。但被复用的现有存储、剩余请求身份、pending fault/reply都要计入同总资源；没有实现/综合，不能宣布固定bit数或免费checkpoint。

当前PTW AR ID固定0，单笔AR后等R。若直接覆盖执行context，在旧R回来时会将旧PTE误按新stage解释。安全普通实现有两条路：在旧R完成后切换；或另外保存响应归属/容量并接收旧R。后一条需要真实路由状态，不能把“保存context”当作撤销memory事务。

### 1.4 最强普通方法及等效构造：击杀本规则

**普通O1：PTE continuation tracking＋同样的优先/RR仲裁。** 给一份active与一份suspended context；R处理后将下一PTE continuation重新排入仲裁；保持held request及最终reply。采用候选相同的必要字段/phase压缩时，它占同信息、同存储容量，并给同一笔不可撤PTE尾部界。若候选只选择少数phase暂停，O1可使用相同phase，不能靠命名“阶段识别”逃过对照。

这不是为否决临时臆造的ordinary。US8140781B2在2009年公开的设计已有I/O device page-walk tracking queue、completion token、合成下一level packet再重新仲裁，以及按VC分开的tracking/completion队列。其文中step305–345和Fig3–6直接覆盖“保存请求身份、PTE返回、下一层重新仲裁”的主体；不声称该前作已经完整实现本RTL的nested权限/错误字段。[原专利正文及claims2–3](https://patents.google.com/patent/US8140781B2/en)。

**普通O2：安全边界restart。** 保留原请求身份，先排空当前已offer PTE，再丢弃尚未对外完成的partial进展；服务V后重走A。静态mapping下可正确恢复，牺牲已做的PTE工作，省continuation字段；原请求存储和reply路由仍要计入，不能称整个方案零metadata。候选若宣称低存储，必须同时击败这个普通重走点，而不是只比全寄存器快照。

**普通O3：PWC/partial translation cache＋O1/O2。** 当前无PWC不是禁止ordinary拥有PWC的理论依据；比较要配平总存储。已有page-table-prefix缓存可减少restart工作，不能忽略它后称checkpoint工作量必然独有。

### 1.5 可证明性质与它的边界

在单PTW执行槽、至少有足够请求身份与一份续走context、正常无A/D写且memory每笔可退休前提下，O1可在当前PTE退休后使A无新DS offer；PTE read work尾部最多当前一笔8B。请求logical completion仍未发生，这是允许保存metadata的释放点。

如果已firstVALID的当前PTE必须经该共享接口接受并返回，且没有合法取消，那么**物理tail quiet不能早于该PTE退休**。这是AXI不可撤事务的下界，不是新翻译定理。若想同时服务V，额外归属/返回槽能重叠等待，但不会让旧A物理事务消失。

时间界必须外加该PTE接受/返回的服务界T，以及内部处理/仲裁界δ。仅假设eventual fair progress可证有限完成，不能推出uniform T。15是上述特定两阶段路径的PTE次数，不含DDT/PDT额外读、日志、MSI或A/D更新实现。

### 1.6 创新条件、最小下一验证与停止条件

因为每PTE continuation/重新仲裁有明确前作，且同字段O1能给同释放保证，**不能把这条规则作为独立机制**。因为O2有低状态重走点，不能仅宣称比全寄存器快照省状态就立成本贡献。

如果提出一种不同表示，使同permission/context/error合同下，在等总存储中达到普通O1/O2/PWC组合都做不到的“释放粒度＋不重走工作量”点，并给可达反例及下界，才有独立成本机制的候选。当前没有此表示；通用状态压缩或简单寄存器liveness列表不足以满足条件。

**下一最小验证只做一件事**：先列上述15-PTE代表路径每checkpoint实际live字段，构造一个O1与一个O2状态表，检查所谓新表示是否真的省过普通必要字段或只复用相同原请求存储。这是未执行的规格，不要求先编译。若O1同容量同保证，或O2＋同预算PWC追平工作成本，立即停止此枝。

## 候选二：故障记录的短期容量背压

### 2.1 我提出的具体规则及接口

规则尝试在接受译址请求前预约其可能需要的fault record槽；正常译址释放预约，故障时把完整record写入所约定存储并立即让TL给错误reply。对重复fault仅保留已承诺代表record，将后续相同fault的reply从log生产脱耦。

输入：请求DID/PID/priv/access/context、fault verdict、fault_ready、FQ writer状态、内部空槽与已记录key。状态：已accept请求对应的保留credit；record的未offer、已firstAW、4W进度、B待返回；明确generation的已报告key。输出：TL request admit、fault record enqueue/skip、正常或错误reply。

不能把不同权限subscriber一个结果广播完事：同一translation的read可以成功、write可以permission fault。若做合并，要分别判定consumer权限/错误，并保留必要记录字段。这个逐consumer判断是正确性要求，不自带新颖性。

### 2.2 真可达的背压点，及错误的强约束

[TL `1515–1540`](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:1515)：正常请求ERROR持续fault_valid，等fault_ready后才COMPLETE。生产N-hit窗口限定PT active、1S且非2S（[303–308](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:303)），不会在ERROR状态直接绕出一个无关普通hit。

但是已有[fault arbiter `42–108`](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_fault_arbiter.sv:42)：每fault来源port一个spill，RR后共享FaultFifoDepth FIFO。正常DMA故障全走一个trans_fault port（[SW wrapper `245–302`](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_sw_if_wrap.sv:245)），port不是DID隔离。soc配置FaultFifoDepth8（[ariane_soc_pkg `114`](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv:114)），不能说“每次fault都等日志B才TL响应”。

[FQ `183–279`](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_fq_handler.sv:183)的一条record需AW→4W→B；WRITE期间不接新record。若内部FIFO/spill已经满，下一fault到TL ERROR会等当前写腾槽。这种有限服务下的短期容量背压确实存在状态路径，尚未执行自然可达实验。

FQ full/MF是另一种情况：`fq_ready=(IDLE|ERROR)`（[129–131](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_fq_handler.sv:129)），ERROR消费后续record而不写；disabled在IDLE同样不产生写。因此“软件不腾FQ就永久阻所有normal译址”不是这条路径的合同。

官方FQ语义允许：满/内存错误置OF/MF后丢触发与后续记录；多个相同fault只报告subset，甚至一个。[RISC-V IOMMU FQ章](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)。没有核到“日志写期间不允许有任何有限背压”的要求，不能据此将短期ERROR等待判为规范违规。

### 2.3 最小见证与强普通等效

最小抽象见证：一个record正在正常WRITE且B尚未返回；内部可接收record槽全满；A下一已accept译址产生fault，TL ERROR待credit；V正常译址已在等待。B在有限时间内正常返回，随后槽释放、A完成error reply、V得以服务。不需要人为不公平memory、不需要永久关预算、不需要TL/invalidation锁。

它仅证明**有限record存储能耦合串行TL**，没有证明新rule改善：

- 预约credit就是ordinary worst-case response reservation。候选每accepted translation保留一槽时，普通同样保留一槽，可做相同决策；在healthy请求占多数时会降低并发，必须实计。
- 提前发送error reply必须保存未交fault record，ordinary异步record FIFO和reply槽实现相同合同；已有FIFO不能被忽略。perDID credit/VOQ是普通隔离，不因record由翻译产生就变新。
- 重复fault过滤可按完全相同的DID/PID/priv/access/IOVA/cause/implicit-GPA/generation key实施，普通精确key cache可给同样判定。标准允许subset报告，不能伪造每subscriber必落盘的硬约束。
- 没有日志保留要求时，标准允许的DTF/disabled/OF-MF discard是更强的工程控制；但若候选要求保留代表fault，必须给ordinary同样保留合同，不能用全丢记录代替等效对照。
- 若非fault V为hit，可扩ordinary权限完整lookaside在ERROR期间返回，配同result容量；若V为cold，需要能暂存A日志并接纳V请求的普通多context/queue。仅放宽当前N-hit适用窗口是feature扩展。

### 2.4 分清日志quiet、TL quiet与device处理结束

fault_ready只表示record交给内部存储或drop路径，既不是FQ record B退休，也不是device error响应完成。已firstAW的日志事务仍不可撤；若日志字段仍引用即将覆盖的TL request，则必须先copy完整record。TL ERROR离开本身不能作为fault-write physical quiet。

有限log服务可以给有限背压，但队列满到清空的周期界取决于实际writer/DS服务。FQ soft-head长时间不推进最终触发OF并drop，不能为了证明环而禁止标准drop。lossless无限distinct faults＋有限record存储＋有限消费速率本来不能同时保证无背压；这是一般生产消费容量限制，不立新定理。

### 2.5 创新条件、下一验证与停止条件

因为已有脱耦FIFO、普通credit/VOQ及标准允许的相同fault报告减少覆盖上述决策，**不能把预约或detach-FQ作为独立新机制**。因为当前仅正常fault0负载，不能宣称它是§39性能残差或§43让路时间的原因。

如果能在明确保留的诊断信息、同record存储、同内存服务下，给出ordinary精确去重/credit/VOQ无法达到的healthy-flow延迟或日志保证，并证明不会把不同permission/error混成一类，才有新候选。当前没有这种反例；若差异只来自ordinary未实现某队列或窗口，归工程支持。

下一最小验证应先纸面检查一个WRITE在途＋内部满的快照与实际TL request顺序，确认正常DMA源不会先被自己的error接口停止而无法产生预想fault串；再将同容量ordinary record credit/dedup接入模型比较。不能只人为把fault_ready拉低做“可达性”证据。若ordinary同容量同日志合同复现收益，或真实iDMA不能供出该fault序列，停止此枝。

## 3. 这轮保留什么、停止什么

保留两项具体研究边界：nested walk的事务数/续走live状态；正常fault日志的有限容量及信息合同。两者都源自真实RTL，不借旧prepared-prefix/W序证明。

停止新增机制名称、停止将每PTE暂停/record预约包装为创新，停止因为缺少feature而先做DUT。若后续没有实质超越普通同合同同成本的表示或保证，本席建议退出这两枝，把它们作为论文必要强基线与接口限制说明。

本席未提出已成立的新定理、未得到新性能/面积结果、未承诺论文分区。root可以继续独立的CQ/invalidation或map的新假设，但不能把三席的共同否决改写为模型共识证明了创新。

## 4. Primary导航及覆盖范围

- [RISC-V IOMMU FQ语义](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)：FQ末尾OF/MF discard及identical fault subset；不据此声称任何logging暂态背压违法。
- [RISC-V IOMMU SADE/GADE](https://docs.riscv.org/reference/iommu/v20260222/iommu_data_structures.html)：AD更新有能力条件；当前生产PTW没有写。
- [HPCA21 DWS](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf)：p1/p4 soft-owned walker/idle stealing、至多一foreign walk；不是每PTE抢占。
- [US8140781B2](https://patents.google.com/patent/US8140781B2/en)：2009公开；Fig3–6、step305–345、claims2–3 tracking/completion/token与next-level re-arbitration。明确击杀continuation主体，不推断已实现所有RISC-V nested语义。
- [ISCA18 Scheduling Page Table Walks](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)：按SIMD instruction整体剩余work和同instruction批次调度；支持“翻译需求的后续收益/剩余工作调度已有前史”，不替代上述具体普通checkpoint构造。

