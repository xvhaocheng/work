# 有界下一步与 M 决策门（不构成本分析中的构建建议；执行由 Codex 决定）

## 1. 当前裁决
M（actual main occupancy based charging）值得继续，但只值得有界诊断与公平对照，不值得直接进入完整方法工程。理由：fullstats21 显示 same req=4,miss=2 的 mean_main 在全量聚合中随 gap 条件存活并同向出现；byte/request/miss/固定类型权重在这一子组内近似常数，因此存在额外信息的可能性。但本段没有 M 实现、没有控制实验、没有独立测试，不能裁定控制价值或新颖性。

## 2. 有界下一步
1) L0：只做接口/可观测性/小范围真实 RTL 合同检查。确认 owner/state/完整成功搬运/排空/CDC/只读快照口径；不把不可抢占边界、身份工程或 token bucket 骨架当创新。
2) L1：自然 4KiB 成本诊断。保持相同字节与任务数，比较单页高复用、A8、低复用；保留同版 Solo/无节流与固定 gap 参考。记录译前占用、译后/数据压力、MRIF、COMPLETE、队列压力、相位与真实映射；人工 128 周期只作探针，不作自然代价。
3) 事件类型计数加厚：在现有 req/miss 之外，加入 COMPLETE、MRIF、数据通路/后完成、FIFO 占用、PTW active、等待阶段、owner/state 生命周期，以及可归因的逐请求或累计量。当前 only req=4,m=0/1/2 覆盖不足。
4) 简单 V feedback 对照：V-only feedback 基线独立实现，与 occupancy charging 共用同一 outer controller、同一目标单位、同一调参预算、同一死区/限幅规则；不要让 occupancy charging 额外获得优先级或更宽松预算。

## 3. 公平基线（必须同 outer controller）
- byte 计费。
- 翻译请求计费。
- miss 计费。
- 按训练期固定类型成本加权的请求计费。
- actual main occupancy 计费。
- V-only feedback。
- 固定 gap 重新调参版；旧 P4M60 只作历史参考，不混入新方法独立测试。

所有基线同 BIT、同仪器、同时钟、同工作集、同完整 copy 作动点、同 calibration 预算；分别校准各自单位，不强制同数值额度。

## 4. 决策门
Gate M0（H-M0 诊断）：在自然 4KiB 条件下，actual main occupancy 是否在 byte/request/miss/固定类型权重与 phase/gap 之外仍有控制相关残余？若 richer event type counts 能完全解释，则停止 M 新方法主张或保持未决。若存在稳定残余，进入 M1。

Gate M1（H-M1 控制价值）：只在 M0 过后进行，且必须独立测试。要求在匹配 V 完成时间/短时未达目标分布容差下，occupancy charging 比 byte/request/miss/固定类型权重/V-only 强基线保留更多 A 同 V 硬件窗口有效完成速率，并达到事先冻结的工程意义门限。

Gate fairness：同一 outer feedback、同一 lookup/仲裁/容量口径；P 不偷加优先级；不同容量或成本必须单列，不伪装同成本。

Gate observability：owner/state/complete/drain/CDC/结果配对未验证时，不进入性能主张；没有下游时间上界，不声明硬实时或无条件 A 最低进度。

## 5. 停止/未决条件
- 若 same req/miss 剩余占用被简单计数或 phase 完全解释，停止 M 新方法主张。
- 若 M1 无实用增量，停止或撤回新方法主张；不把阴性解释成等价已证。
- 若证据不足，保持未决，不强行进入完整工程。
- 不因 V hit 占多数就推断纯 IOMMU 因果；V hit 只说明 lookup 命中，不排除共享服务、出口等待或软件供给影响。

## 6. 需 Codex 验证的直接项
重算 main_condition_summary；核对 group_counts/sum_req/sum_miss/req=4*count/miss=g*count；核对 sampled_same_group 覆盖缺口；核对 V hit/miss 例外；核对 UART/taint/provenance 警告；执行最终 198 文件哈希验证。
