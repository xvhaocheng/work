# 普通 2MiB 分散 backing 与完整准备合同

这是普通基线适配合同，不是创新方法、已实现 Linux adapter 或实板结果。保留 stage106 原任务与原始结果，解决其大页对照在实际平台必须满足什么条件。

1. 当前现存 config CMA=n、默认 MAX_ORDER=10；10MiB FORCE_CONTIGUOUS 需要 order12，不能沿该 generic buddy 路线保证整槽成功。替代为每10MiB槽五个独立2MiB物理 extent，四槽共二十块／40MiB。每块 order9 在当前上限内，但二十次成功率、碎片和分配时间尚未测。48MiB 的旧模型完整 backing 含 guard，不能改报40MiB。
2. 对每槽保留连续且2MiB对齐的IOVA；每extent连续2MiB且PA同样对齐，各写一个合法2MiB叶。整槽及全任务不要求物理连续。整张量copy可以跨IOVA叶，由已存在硬件分段继续；无需人为增加“每个extent一条descriptor”限制。
3. 使用 kernel page allocator＋显式映射时，不得与 DMA API 自动映射重复建立未计成本／权限的别名。若用 SG/DMA API，必须检查实际IOVA对齐、叶大小及生命周期；map返回成功不保证采用2MiB叶。任一对齐／allocation／map失败保留合法4KiB回退并计其全部成本。
4. 完整2MiB页及填充范围必须属于相应已授权对象；实际任务所有者决定是否允许一个可信DID完成两包。硬件DID固定，换设备需对应domain映射。现helper全局锁和每DID私有stream不是多租户安全证据，也不能据helper不能选同DID就禁普通单DMA。
5. CVA6缓存不被DMA snoop；复用现有明确的交接／FENCE.I约定，核实际平台语义及成本。源码默认coherent配置和SG map成功不代替数据、缓存所有权与完成校验。准备在计时窗前并非免费；每阶段独立记录实际开始／结束。
6. 冻结实际Image/DTB/bit/source差异及运行模式；mode0生产buffer为3 beats，stage106私有两路为512。源码mode1可给前两路512、运行NB关闭，但尚无本轮实板身份或时序证明。不能把默认mode0或旧本地Image称为同资源验证。
7. 最小运行验收若将来需要，只做allocation→map/leaf→cache handoff→原完整任务→drain→cache reacquire→unmap/free；记录所有阶段成本和失败回退。不重跑已验矩阵，不先启动FPGA长构建。

静态布局见证只能证明给定示例地址的几何合法性；它不能证明板端分配成功、cache正确、DMA完成、性能或净收益。

来源：[平台约束](../platform/constraint-evidence.md)、[root关键源码](root-source-review.json)。
