## Material Passport

2026-10-15；ARS-Codex experiment-agent：固定输入的原理与正确性核查。来源模型MQSim固定commit `51f0f2d3fed92d88ef4a0fa61a38024b07bf9d16`。本文件接受的是特定缺失完成的源码/事件根因，不是论文创新或物理器件测量。

## 已确认的因果链

原2048个请求中，主机第1777次生成请求（arrival=177700000模型ns、LBA2912、16 sectors）被绑定为设备native ID1776、该生命周期CID53、LPA182、child UID2841，Source=USERIO、Type=WRITE。

| 顺序 | 原始日志行 | 动作 |
| --- | --- | --- |
| 1 | 10788 | host UID1777生成 |
| 2 | 66940 / 66948 | 同一host对象绑定SQE，再绑定设备请求，CID53 |
| 3 | 66955 / 66956 | child2841以NO_PPA建立，进入LPA回收屏障 |
| 4 | 67100 / 67102 / 67104 | 屏障释放→AMU局部handler→直接删除child |
| 5 | 完整78290条记录核查 | 无该child的TSU提交、PHY完成、cache owner完成、CQE发送或host完成 |

实际原始事件与SHA/摘录：[缺失请求身份](missing-request-events.json)、[root接受](identity-acceptance.json)、[完整日志](../implementation/run-2048/stderr.log)。所有2048个host生成UID均唯一绑定SQE/设备请求；2047个唯一host完成。3438次child创建中，1个Source=MAPPING、owner0的t0 helper不属于待完成用户请求，另1个是上述被屏障删除的用户child；其余3436个走PHY完成/删除。没有identity_error。

原源码 `Address_Mapping_Unit_Page_Level.cpp:1797–1821` 的release将等待child传入本类handler后delete；该handler在`1665–1670`遇到Source!=MAPPING直接return。真正用户完成路径是PHY广播→Advanced cache从owner列表移除原child→正常request serviced→CQE。此删除路径没有走这些步骤，同时owner列表仍保留已经失效的原ptr。因此当前缺失完成已由动态身份链与实际源码共同解释；仅补成功通知会掩盖未执行的用户写入。

## 记录方式的有效性

只新增被动观察，不修改模型字段或事件调度；observer外部map使用专用32MiB BSS arena，不消耗模型通用heap的节点。原因是原源码有按指针排序的等待集合，额外heap分配可能改变顺序。根本保证仍来自本次实际对照：过滤新增identity事件后，与旧23891 callbacks相对应的全部原有记录与上轮逐项相同；输出XML也逐字节相同。请求/child采用独立递增UID，指针或循环CID只作关联字段，不充当唯一身份。

本轮初始身份重放仅1次，未改seed/cache/到达时机/页数/结束条件；启动前源码、二进制、合同和输入hash均留存。[运行manifest](../implementation/run-2048/pre-run-manifest.json)、[结果](../implementation/run-2048/status.json)、[观察审查](observer-review.json)。这是对新具体失败的诊断重放，不恢复旧M0、CS1或硬件实验。

## 修复决定

进入最小私有语义变体：只从指定LPA的两张屏障等待表摘出原child，解锁后以局部借用指针list调用正常Translate路径，保留owner列表、数据/bitmap/timestamp及真实分配、program和完成因果。不delete/rebuild child，不补造CQE，不改GC旧block所有权或绕过overfull保护。实现范围及四输入验收见[正式修复合同](repair-contract.json)，边界推演见[设计说明](../design/repair-contract.md)。

本轮仅在整页USERIO write-only/cache-off、既有单stream/单资源配置上验证；同LPA并发覆写保留，沿用源模型按allocation更新映射的规则。混合read/write、partial、cache writeback、多chip以及真实payload字节不因计数相等自动获得正确性证明。旧结果原样保留，后继须明确标记为私有语义变体。
