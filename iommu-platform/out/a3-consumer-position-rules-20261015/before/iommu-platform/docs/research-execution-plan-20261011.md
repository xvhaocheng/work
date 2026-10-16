# A控制位置方法的局部执行计划

2026年10月15日。A2最小当前平台软件已实现、构建并通过分层检查；真实DMA/新内核运行和完整任务时间仍未验。

1. 继承A1四案八次/1412事件、七篇核心文献及最近五天成果，不回放旧实验或重新全面调研。
2. 当前Linux6.5私有同步ABI、固定pin/Sv39/noPASID借用CPU页表及C1 IOVA/C1s同址shadow已写入代码。C2要求同8页mlock并限制PTE老化能力；普通方案不承担该额外约束。domain/owner/异常保留和map/unmap/unpin全路径已实现，native仍需验。
3. 用户16项、驱动C边界20项+配置1项、析构47项、新CQ本体4项通过，内核Image/rootfs及RV64程序构建通过；各层不是完整native运行。新接口默认关闭，无FPGA/板操作。
4. Hero精确LLVM d8完整host/device链及Linux6.9驱动对象编译通过，首错误ELF已否决；透明EH兼容后静态验收通过。三个硬件gate仍需实际实现，不能把helper桩当闭合链。
5. 原buffer writev已满足本最小文件功能，停止该独有收益主张；保留A，绑定真实device/consumer位置或CPU预算，再比较独立shadow prefix、同步borrowed和付费内核consumer。当前同步实现损失用户态重叠，费用必须计入。
6. 将来安排硬件时按已冻结native功能合同验身份/cache/RWB/CQ/进程与资源生命周期；真正有用选择、普通不足及有限服务闭合后才冻结一正一反两时间留出。不造时延/压力/相位，不自动恢复MIG或新建FPGA任务。

[实施报告](a2-sva-implementation-20261015.md)／[交付入口](../out/a2-sva-implementation-20261015/README.md)／[稳定接受](../out/a2-sva-implementation-20261015/root-result-acceptance.json)／[native合同](../out/a2-sva-implementation-20261015/validation/full-native-contract.json)／[后继决定](../out/a2-sva-implementation-20261015/next-decision-contract.json)。两周10月15—28日和历史用时保留，论文未完成，Codex-only；原MIG1792023314 paused及失败/0采集事实不变。
