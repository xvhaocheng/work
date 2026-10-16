# F1真实R/PTE返回封口：机制卡

**问题**：选择先翻译的合法队头会先释放不同的数据返回；若这些返回占满真实源缓冲并封住共享R，下一PTE可能尚未生成／准入。只在首次NormalTL VALID前比较AB／BA，能否改善所有已拥有任务的完整完成？

**新增能力**：F1仅新增同一当前义务快照的两顺序服务预测和首个NormalTL选择；R/PTE共队、真实容量、W独立服务、完整walk保持。过去完成PTE数量／服务记录可以估计，未收到PTE的内容、未接受future child、隐藏MIG状态不能输入。无有效历史走RR；一次越过另一等待队头后强制服务它。选择后完整ID/header held至accept。

**普通强对照**：前轮九个冻结在线合同全部保留，含RR/年龄、NativeN/DSpriority、owner、短TL、欠账、release-density和普通两阶段/turn规则。DSpriority本轮合同覆盖共同t0至所有任务/R/W/B/faultquiet。新增一个明确普通 `O-envelope-select`：真实完整source/return容量/WRITE供给与W租约证书的唯一安全头先于已证危险READ；缺证、双安全、双危险按age/RR，held/fair优先。它只判断当前有限返回义务，不做两个预测rollout。准确WRITE准备、whole-envelope/预约/one-primer/coupler各从合法共同origin独立比较、完整计费。

**完整所有权与成本**：当前merged NormalTL没有full源ID/LEN/SIZE和R/W/B债务；必须付真实head关联、原生buffer/slot占用、W owner/lease、各级PTE接受及R首次VALID/held/握手反馈、历史状态/端口、预测与选择延迟。虚拟地址接受、内部spill接受和物理DDR承诺分别记账。许可不释放W owner；W owner在真实WLAST握手释放，任务还需每个B、R末拍、fault与最终quiet。原生/ordinary共有对象只算一次，另一个源的槽或inactive数组不是免费容量。数值宽度/逻辑/延迟未冻结，面积/Fmax/应用均null。

**本轮三个决定范围**：K0 private单个对齐4KiB/每源512拍，不能以自身payload容量溢出制造heldR。K1生产小源同bank阻塞只是一条条件接口前缀；在bank0 `[危险READ2, 解锁owner0所需READ0]` 和FIFO completion-R服务下，两bank顺序都不能完成全部义务。K2“唯一安全头先”动作被单个O-envelope-select表达，但不证明两安全头的服务预测、实际时延、数字成本或发表整体覆盖。

**正反例门**：当前无一个既实际fresh可达、又对普通有完整差异的正例被接受；因此没有制造邻反例／身份交换／RW留出、没有搭私有原型。K0的512对齐容量条件是数学排除边界，不能把减小缓冲当正例。K1的未知DEFAULT前缀也不能手工初始化队列伪装成实际可达。若以后有两安全头真实服务窗口差异，需新原始事实和同一在线完整对照重新准入，不自动继承本轮假设。

**裁决**：本轮具名held-return实例不准入。F1全族仍开放；没有独有收益/论文主贡献接受，不修改论文定位。原立题窗口累计，论文未完成。
