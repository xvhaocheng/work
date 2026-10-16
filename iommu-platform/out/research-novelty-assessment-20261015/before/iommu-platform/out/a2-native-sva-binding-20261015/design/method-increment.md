# A2 从源码身份和发布状态推导控制合同

2026年10月15日，root设计。继承A1的K与算法，不将原四案八次源语义升级为时间结果。本轮增加的是合同抽取规则及可反驳的绑定，不是另造DAG或max-plus算法。

## 输入增加什么

每个任务的数据引用增加 `application_buffer, owning_mm, CPU_VA, device_address, physical_pages, context_root, DID, PV/PID, permissions, map_generation`。每个值必须有来源；数值未知可保留关系。例如DATE公开新驱动给 `IOVA=0x80000000+(UVA mod 0x40000000)`，再建立IOVA/PA及LLC bypass别名，这不等于UVA与IOVA相同，更不等于设备直接使用进程页表。映射epoch、槽generation和functional job ID也不能相互替代。

配置输入增加 `source_commit+dirty_hash, elaborated_profile, actual_vendor_source_list, build_identity, run_identity`。当前源码默认、已有条件夹具、旧板加载分别有值；不能取各自最有利一部分拼成一个平台。实际来源表见[接口证据](../interface/publisher-owner.json)及[DATE软件证据](../sources/date-software.json)。

为每个边界记录 `observation, offered_packet, current_owner, capacity_unit, release_predicate, public_receipt`。capacity_unit区分job、header、数据拍、页和软件pending。FIFO深度8只有在前端能产生8个未完成job时才成为可达任务并行度。

## 发布转换与所有权规则

当前平台normal lane的合同是：

```text
软件ready / 可选择
  -> 独占lane并写SRC,DST,LEN,CONF
  -> NEXT_ID读取导致IDLE -> COUNT并分配functional ID
  -> COUNT首次完整offer -> job FIFO接受 -> frontend WAIT
  -> backend分段生成AR/AW/W等真实义务
  -> 最后burst的B引发backend response -> frontend retire
  -> 公开DONE观察 -> cache acquire -> consumer -> 槽/映射归还
```

NEXT的MMIO响应、COUNT offer、FIFO接受和backend接受是不同事件；精确先后由总线/前端状态决定，不能把一项收据填成所有项。一次NEXT读在lane非IDLE时不构成另一个排队请求，可能返回0。因此source async API有两个合法适配：阻塞到该lane可发布；或付费增加有限software pending和虚拟receipt→真实ID关联。原调用返回的语义若变了，重新展开ready关系和所有槽持有成本。

规则R1：尚未NEXT发布且应用依赖允许时，可选择/延迟新job。若COUNT或更下级held头已经offer，本owner须保持完整包至接受；仍要分别检查其他端点是否有合法选择。

规则R2：一个未退休receipt占用lane；JobFIFO8不能放大为8个normal在途job。正常2KiB对齐copy不需要额外许可，但没有4KiB FullBuffer例外。64B HOLD和4KiB batch ADMIT不用于这个任务。

规则R3：同固定AXIID的多个burst，按 `lane+functional_job+direction+burst_ordinal+fullAXIID`关联。AW在上游mux向spill首次offer即建立W-owner次序；不能等外部AW接受再分配W归属。raw WLAST、target WLAST、B接受及consumer各自释放对应资源。

规则R4：DONE只解除已退休job的义务；当前NO_ERROR_HANDLING不能由DONE证明成功。cache acquire、全部目标字节/响应检查及consumer仍必须发生。超时不是取消；没有安全drain证据时保留映射和buffer，不运行清理后续。

## 推导输出和时间准入

首先从软件依赖和上述转换求实际可发布集合，再沿真实profile的前端/TC/TL/PTW/target/返回链展开。source允许两种调用次序，不自动证明DDR选择器出现两个fresh heads；若实际前端串行化，方法应在这里拒绝该次晚位置二选一。

完整完成输出仍为 `(T_A,T_B,T_consumer,T_release,T_quiet)`。本轮新的具体可检查输出为：

- 普通源端NEXT前控制对未发布job合法；公开DONE节流可执行，损失重叠待计。
- “同lane两个async立即返回就有两个硬件任务”的适配不成立，须明确阻塞或软件pending费用。
- 当前mode0每DMA24B/单TC与旧512拍banked夹具不相同，后者时间不能赋给当前应用。
- DATE新公开链的target DONE缺尾DMA闭合、host consumer别名、mapping释放缺失；现有源不能满足完整完成谓词。

只有数据/身份、publisher、生命周期、实际服务和完整成本全部闭合，才准入时间计算。现有行为DDR的读FIFO服务是具名条件服务；写/B使用无显式容量deque、真实host/runtime/cache/consumer服务未绑定，不能直接充当本任务的有限资源后端。事件序号不转周期；返回拍数不转MIG时间。

## 与前作的可核增量

MASK在可修改MC内联合PTE/data优先（PDF7–8 §§5.2–5.4），DWS提供walker软分区和非抢占steal局部交错性质（PDF5／印刷630 §V）；它们提供可用策略和局部边界。TC2023、ICRT和AXI-REALM等已处理AXI服务/预算/分段/阻塞；A1的[准确原文比较](../../control-position-method-20261015/literature/analysis-delta.md)应与本文件一起阅读。标准状态图、残余阻塞和有限资源分析不是A的新算法。

A2将上述分析的输入追溯到真实应用地址、软件receipt、前端可达并行度、当前编译profile、跨AW/W/B的owner和最后consumer/解除映射。它已经识别出几种会改变部署判断的具体不合法绑定；这些是方法资产，尚不是独立论文贡献的充分证据。若该抽取能在未参与设计的真实完整任务上预测当前位置普通控制是否足够、何时更早控制有净价值，且优于具名普通分析/实现的费用或能力，才接受这一版方法贡献。只发现源码缺陷、补齐生命周期或把来源表写全，不单列novelty。
