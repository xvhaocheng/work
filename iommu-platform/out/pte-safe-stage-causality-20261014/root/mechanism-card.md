# 两安全队头的PTE阶段等待：本轮机制卡

可推翻主张：在同一自然fresh、两个原生合法且真正安全的NormalTL队头上，先完成一个会释放不同的已拥有payload；若payload改变后继PTE的相对生成／准入／返回等待，使用当前义务及已完成历史的两顺序预测可能改善全部任务完成。

动作固定为merged NormalTL首次VALID前选择A→B或B→A；单头立即native处理，不等第二头；已offer保持，完整walk、NativeN、IoTLB16/1PTW、fullID、同ID、W/R/B/fault与原生容量不变。缺有效历史RR，一次越过后强制服务等待队头。future PTE内容／PA、未接受请求及隐藏DDR状态不输入。

必须按同source/job/context/stage/level的PTE测前驱consume→birth→内部DS接受→delayer/xbar接受→DDR接受→firstRVALID／真实握手→PTW消费的**相对间隔**。仅排第二导致整个walk开始更晚，或payload利用生成间隙，都不足以称PTE等待反馈。

强对照保留九个完整冻结NormalTL规则、全任务quiet范围DS返回优先，以及一个O-DS-AR：当前VALID DS优先，原有firstVALID／已接受spill不能撤销，4次DS公平逃逸、fullID、W/R/B/fault完整。各所需observer/history/ports/选择延时同样计费；一个规则覆盖可选RR位移不等于全F1复制或发表覆盖。普通O-two-stage仅在其固定独立两阶段前提成立时才适用。

本轮源码给出双READ同步条件但未提供真实固定供给时序。root只批准一次被动native前缀探针，没有ABBA动作或预测器。新2×4KiB/512运行无任何双头VALID，12个PTE内部→DDR56、DDR→consume88，存在payload在途但无AR竞争，故本固定前缀源门未过。原生frame全8192B目的核对及完整R/W/B/quiet成功，不是候选收益。

正例／相邻反例／身份交换／另一合法RW四条件未准入。没有相位扫描、人工hold、压力/容量扩张、旧实验/validator重跑，所有失败新build保留。新增预测器数值成本和面积/Fmax/应用null。仅关闭本前缀作为因果正例；F1全族和其他合法输入初态保持open/unknown，论文未完成。
