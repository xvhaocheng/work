# R4 协调者直接核查与待裁决项

本文件是根 Codex 对关键证据的点验和审稿问题，不是新增席位或新一轮答案。保留原回复，纠正在后续轮显式完成。

## 已直接核对的本地边界

- `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_nonblock_hit.sv:84–104`：accept 必须通过 DDTC/IOTLB hit 与权限门。`cva6/corev_apu/fpga/src/dma_composition_control.sv:352` 只在 fast 且 accept 时计数。D2 R3 再写“含 fast 的服务 miss 未测”不能推翻此定义；被拒 probe 历史是另一问题。
- `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1264–1278`：DMA generate 内各实例自己的 BUFFER_DEPTH；`iDMA/src/idma_legalizer.sv:75–82` 给出 burst 分段容量，支持 C1 的源码结论。
- `cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:362` 的 PR/PW TODO 确实存在。未检查完整系统可达性，不据此发布漏洞结论。

## 规范点验和 R4 必答

根 Codex 直接读取了 [固定版 RISC-V 队列规范](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html) 的失效及 fence 条款（网页 L92–114）。PR/PW 的义务针对已处理访问的全局排序点；还未到达 IOMMU 的请求有额外平台同步边界。

1. A1 R3 以 AXI B 响应晚于 AV 直接判非法，缺少 B 与全局可见性关系证明。请改为抽象全局可见事件，或明确并验证平台映射；不要用未证实的时间戳替代规范事件。
2. 同一 IOFENCE 不能因撤销只涉及 A 就自行缩小对其他先前访问的规范义务。选择性安全复用证书若另设接口，须计入完整协议和比较成本。
3. C2 R3 新核查表明官方 ref 已有 PR/PW 同步回调。请读该轮文献原始证据，不得继续把 ref 描述为完全不知排空，也不得只与故意弱化的逐条相等 oracle 比较。
4. D2 R3 再把 `v_fast>0 ⇒ a_miss>0` 叫必要条件：开窗前开始的 walk 可以进窗，聚合窗口不能支持这个无条件蕴含。样本相关与机制必然性须分开。
5. D3 R3 表里“窗口 A 不是 goodput”与正文冲突。`a_completed_jobs_window / v_window_cycles` 是该共同窗口下的完成率；A 裸计数不是速率，whole-round 可变尾段也不能直接比较。
6. 各候选要挑战信息、语义、资源与成本公平的强对手。额外硬件不是一概禁止，但须量化代价；不得为了凑主线保留被普通 scoreboard、RR、信用调度或参考模型完整覆盖的提案。

R4 应允许“目前无候选已达到立项证据”作为结论，并给低成本解决剩余不确定性的步骤。用户要求想象力与发表导向，不要求模型投票证明创新。
