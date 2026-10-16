# §49 根级前作核查与可检验增量

2026-10-09根Codex通过web直接读取原文关键段。浏览页号只是web提取行号；论文精确版本写在URL。未声称完成全球新颖性证明。

## 1. AXI XBAR的有限缓冲问题已有人解决

[A Multicast-Capable AXI Crossbar for Many-core Machine Learning Accelerators v1](https://arxiv.org/html/2502.19215v1)，II-A（web提取105–124）：multicast/unicast相互排空；多个slave所见AW顺序不同且无法缓存全部W，会产生等待环。其新增aw.is_mcast/aw.commit与一致选择保证全部目的同时acquire。

不能把‘广播会死锁’、‘全部目的ready后commit’重新命名。当前拟复现的环是一个有序Whead与两个独立DMA R-input小buffer，拓扑不同，但容量/循环保守原则并不新。增量要落到独立任务成员的pause/partial parent replay和具体低存储实现。

## 2. DMA endpoint复制/少读内存不是新原理

[Torrent: A Distributed DMA for Efficient and Flexible Point-to-Multipoint Data Movement v1](https://arxiv.org/html/2512.17589v1)，III-A/C（129–143）：独立配置链、反向Grant、Data与Finish；中间节点store/forward，data switch on-the-fly复制而不额外存payload。已有与AXI兼容的endpoint multicast，不能把read-once、DMA自带复制或不改NoC当独占贡献。

该文的公开合同将多个目的放在一次Chainwrite任务里。它未在这些段落描述已partial-return的独立logical AR如何退出后补读；这只能确定与本文显式合同的区别，不证明作者硬件不能扩展或领域没人做。

## 3. 最新DMA-Latte已透明识别多copy广播

[DMA-Latte: Expanding the Reach of DMA Offloads to Latency-bound ML Communication v3](https://arxiv.org/html/2511.06605v3)，2026-09-14版本。旧v1题名DMA Collectives，§49不能只对旧版本。

IV-B（179–182）：MI300X single-source/two-destination broadcast，少command、engine、sync及source读取。V-B5/7（235–244）：bandwidth saturation时broadcast未必额外快，b2b在小尺寸可比broadcast更好，大小范围对比不可遗漏。VI-A（308）：runtime可从hipMemcpyBatchAsync中的相同source/size自动选择bcst，自动合并独立copy本身已覆盖。

公开作者代码入口：[AMDResearch/AMD-DMA-Latte](https://github.com/AMDResearch/AMD-DMA-Latte)。本轮只看README/论文原文，不声称复现其MI300X、阅读全部代码或用CVA6实测超过MICRO2026结果。

## 4. 检索也发现跨领域preemption，不能泛称无人做

官方[Microsoft GPU preemption](https://learn.microsoft.com/en-us/windows-hardware/drivers/display/gpu-preemption)已有split/partial DMA command处理和多adapter broadcast内容约束，但DMA packet在该上下文是GPU命令buffer，不能直接等同AXI payload parent R。

检索也出现NoC/network multicast preemption、重传、有限缓冲流控。这些需要按数据保留/receiver恢复语义核，不能以检索结果标题证明本机制已存在；同样不能因几个精确关键词没找到就宣布首次。

## 5. 本轮正向贡献合同

已确知的背景方法组合，接着解决一个具体接口约束：已offer长physical R不改ID/LEN/RLAST，成员A按firstVALID保护与W debt先清偿再停接，V继续，A partial logicalparent原cursor保留并恢复后补读。首次AW前short-W shaping解决tiny private buffers，退出后未offerW恢复long以减少额外headers。

可检验结果是‘共享读取时保留独立暂停能力’、相对于同private容量closed multicast的等待消除、相对于可退出tile multicast的物理请求次数/短W数量取舍。源读少一半只是无pause背景能力，必须与公开DMA broadcast对照而非作为新原理。

如果源/输出合同真实有用、此联合动作产生同资源收益且没有直接前作给出相同组合与保证，就可以论证微架构独立贡献。如果直接preemptible multicast已做同partial-response/W debt控制，或strong具体实现同保证同完整成本，则应转向新的差异；不能靠‘普通也能写同样代码’无证据判定已有。
