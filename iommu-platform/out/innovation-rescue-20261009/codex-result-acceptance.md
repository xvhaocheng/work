# §49 根级接受：共享长读取、各自暂停与恢复

2026-10-09。Codex 根亲自提出机制、核关键源码/原文、逐批审代码与实际执行。DeepSeek 作者实现/局部修订；root 加了关键 resume else 分支并执行真正 deferred-resume 交互，不将 worker 静态推演当验证。

## 实际接受什么

run03：310 个受限事件场景、7540 checks、exit0；run04：4 个额外定向真实 Sim 场景、104 checks、exit0。合计 314 场景、7644 checks；包含预期停顿的负例断言，全部 `ok=false` 均 fatal。不是 314 个 DUT/FPGA 实验。run01 的598失败、run02 的143失败及两版代码未删除。

一条 shared physical long R 可在 A 清完已 first-offer 的短 W 和已冻结 RVALID 后，只继续给 V；A 保存 contiguous returned cursor，在原 physical R/skid 退休后另发 solo child，完整 payload / index / logical RLAST / AW-WLAST-B 平衡均检查。实际 held AWREADY/RREADY 注入进入模型 Sim，非 toy 变量自证。run04 两策略均 V long AW first-offer t4，resume 请求 t6，A 旧债务 drain EXIT t10；没有在 t6 重返错误 common band。

在 N16/B3/q2、P3、resume200 的同参数模型见证：open 物理 read28 beat/AR2，总 AW6；tile 同 read28/AW6、AR4；双方最终214 event steps，不能声称 makespan 领先。closed read16/AR1却让 V 等 A 恢复，AW16。independent read32/AR2/AW2，但已一次承诺 long A W，不能在指定 P3 处实现同样有限暂停；它完成35步不能作为满足同保证的暂停性能对照。

no-pause N16：open/closed/tile 都 read16，AW16；open/closed AR1、tile AR8；三者全部48 event steps；independent read32/AW2在35步完成。明确保留新机制写分片开销，AR少不是已实测更快。N256的 early P2 源读取510 vs independent512，节约很小；不能将某个最佳停点的共享量推广所有暂停场景。

## 根级控制不变量与有限工作界

在 common phase，每流最多一个 unretired physical W group、长度 <=q、非 head 私有容量 B>=q；每个冻结 required mask 中的 beat 必须落在该 flow 已 first-offer 的 W coverage 内。收到有效 pause 后停止 future A first-offer，因此旧 A 额外 R <=q、旧 A W <=q，合计 payload work <=2q。保存中间 RVALID mask，不能将它撤销作为退出；早 resume 若 V 长 W 已 first-offer，先 drain/EXIT 再 solo，不重接 common。

这是在公平有限响应、单个 common group、固定已授权内存下的工作量推导，非固定 cycle 期限。314 场景没有逐一输出 pause-tail-q 专项数值证明，也没有形式证明全输入或生产无死锁。若允许预排多个 group，界要累加所有已 first-offer/上游不可撤销的物理义务，不能仍报一个q。

## 创新与代价接受门

共享读、自动同源 grouping、动态 fork、slow receiver退出和补传都有前作；根读的来源见 cross-field-boundary / primary-review。留下的设计点是：AXI first-VALID义务下，长 physical R 与独立 logical pause 之间的具体准入/退出/恢复组织。不以“ordinary也能加全部提案”为否决，也不以没有检索到一篇论文为首创证据。

如果同完整硬件成本的实际 RTL 保住长 R，又比强 tile 的多个 children 带来实际延迟、翻译控制或资源增量，且前作没有同样控制，则可形成独立微架构贡献。当前模型证明受限可行性与 AR 数差异；没有证明 PTW 减少、综合面积或平台速度收益。

B3=每流24B FIFO，二流共48B；共享 skid另8B，故模型 payload暂存合计56B。Python history/out arrays是验证 oracle，不是拟实现硬件缓存。需要另计 raw parents、PA recipes、held headers、B join、cursor/mask。default production FIFO=512beat即每流4KiB，足以停放本例2KiB；小 FIFO 的 naive 负例不是默认生产 bug。

## 下一项授权工作

仅一个 out-only RTL adapter 原型与强控制，先连接真实 iDMA/IOMMU、同源不同 context 的独立许可、实际 shared source PA。physical W splitter 保留 raw long parent，仅发 q children；只在准备好的 permission/page span 内保 long R。先一次构建加少量完整 copy 的有限 pause/early resume，比 AR/AW/B、R/W、TL/PTW、V/A时间、完整状态成本；不先编 BIT，也不重跑旧阶段。实现合同见 codex-cross-field-boundary.md。
