# §78 后续创新门槛（Codex 实质设计）

## 新结果排除了什么

14项实际完整copy均通过。五项真在途加入（含E63、E256）读取257拍；普通full-ready startup读取256拍，并在五组实际对照中更快。run01对故意solo启动的run02平均耗时低17.1607%，不能把这个改善当独立贡献。prepare-order0两版run13/14均未加入，终点相同；其CSV/原始指标是否逐字一致由最终脚本另核。

因此，许可在第一条source AR之前已齐的工作量已经有更简单答案。下一场景必须让A任务或许可在V已启动后真实到达，控制器不能提前知道将来的成功结果；不能为了成功故意停V、强制内存按某地址选择或修改alreadyVALID响应。

## J1 仍有一个更强的普通反证

源码中skid_data_q只有reset及实际physicalR捕获写入。skid清空只清valid，数据不立即擦除。因此V已交付第一拍、尚未捕获第二拍时，原8B响应寄存器还保存第0拍。若A真实late到达并获own READ/WRITE成功、PA/normal属性一致，且无A已firstVALID物理child，就可能把该保留数据重新作为A logical beat0交付，再让A加入旧V的未来R。

这只是一条源码推导的普通对照，尚未实现/运行。实施时必须核以下边界：数据确属同一job/epoch/PA的第0拍，immutable source；注入沿不同时捕获新的physicalR；已经held的physicalR保持payload/VALID；禁止同沿新建A prefix AR；独立logical LAST及原W债仍成立。寄存器物理存在不等于这些合同已证明。

若这条普通retained-response控制能在同资源下读取N256且保持V早启动，J1真实late场景也不足以支持新算法。不能宣称“晚到就只有补读方案”，也不能默认保存这8B需要额外payload容量；控制/验证成本仍须计费。

## 可推进的真实差异

优先在明确的真实late任务、热/冷IOTLB、连续作业和不同目的WRITE授权前沿下检验。当前fixture每次reset且只支持一对2KiB作业；不能把现有hot/continuous行为当已完成。连续作业需要在真实quiet后更新parent/sequence/epoch，保留IOMMU缓存但不能偷用过期recipe；这是下一批架构工作，尚未实现。

同授权信息/总资源的普通等待窗口、读分片、保留响应/有限history、pending-read merge和广播都应获得对照机会。如果它们复现全部决策和收益，就不能称独立规则。如果新义务/许可反馈在不可撤协议和有限缓冲下带来额外可证明能力，且真实RTL超过这些对照，才可以形成机制贡献。

本轮不会因为一组17%或“在途加入实现成功”就降低此门槛。模型轮询修复是实验基础，不是论文新机制；当前也未测FPGA面积/时序/上板速度。
