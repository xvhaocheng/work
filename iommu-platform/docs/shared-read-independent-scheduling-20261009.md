# 共享长读取与各自暂停/恢复：2026-10-09 当前增量与强对照

后续§50已完成真实RTL完整copy；本文件保留§49事件模型的历史结果，当前进展见[真实RTL报告](shared-read-rtl-progress-20261009.md)。不能把下文event steps与新cycles合并。

本文件整理 Codex 根级已接受的事件模型结果与对照边界。范围是事件模型，不是 RTL、不是平台 cycle、不是 BIT。全文 t/events 一律称 event steps，不称 cycles。Codex 根与两个研究席亲自提出设计、质疑和核查关键源码/前作；DeepSeek 作者模型提案、修订、证据整理与本文档草稿；Codex 根完成代码审查、关键 resume 分支修正、执行和最终接受。未调用 Claude。

## 1. 当前具体增量与结果

- 实际接受：run03 为 310 个受限场景、7540 checks、exit 0；run04 为 4 个额外定向真实 Sim 场景、104 checks、exit 0。合计 314 场景、7644 checks（314=310+4，7644=7540+104）。不是 314 个 DUT/FPGA 实验。
- 失败与旧版本全部保留：run01 的 598 个失败、run02 的 143 个失败及两版代码均未删除。run01 首败根因是作者实现未设置 Flow.resumed，补偿 AR 未发出；root 读实际源码先定位，再送定向修订。属 worker 实现错，不是生产 CVA6/IOMMU 同 bug 的证据。
- 机制实际动作：一条 shared physical long R 在 A 清完已 first-offer 的短 W 与已冻结 RVALID 后，只继续给 V；A 保存 contiguous returned cursor，在原 physical R/skid 退休后另发 solo child，完整 payload、index、logical RLAST、AW-WLAST-B 平衡均检查。run04 中两种策略均 V long AW first-offer t4、resume 请求 t6、A 旧债务 drain EXIT t10，t6 未重返错误 common band；held AWREADY/RREADY 注入进入模型 Sim 真实控制路径，不是 toy 变量自证。
- 有限工作界：common phase 每流至多一个 unretired physical W group、长度 <= q，非 head 私有容量 B >= q；每个冻结 required mask 的 beat 必须落在该流已 first-offer 的 W coverage 内。收到有效 pause 后停止 future A first-offer，故旧 A 额外 R <= q、旧 A W <= q，合计 payload work <= 2q。该界是公平有限响应、单个 common group、固定已授权内存下的工作量推导，不是固定 event 期限；314 场景未逐条输出 pause-tail-q 专项数值证明，也无全输入或生产无死锁的形式证明。若允许预排多个 group，界须累加所有已 first-offer/上游不可撤销的物理义务，不能仍报一个 q。
- 暂存成本：B3 为每流 24B FIFO，二流合计 48B；共享 skid 另 8B，模型 payload 暂存合计 56B。这不代表完整面积：raw parents、PA recipes、held headers、B join、cursor/mask 须另计。Python history/out arrays 是验证 oracle，不是拟实现硬件缓存。default production FIFO=512 beat，即每流 4KiB，足以停放本例 2KiB；小 FIFO 的 naive 负例只说明有界缓冲场景，不是默认生产 bug。
- 模型中 B 在 WLAST 同事件退休，未模拟真实 delayed-B；memory permissions 与属性是前提，静态 admission predicate 不能当作 IOMMU 权限测试。5 次成功 DeepSeek flash/high/default最大预算调用共 527959 provider tokens，全部未截断；原响应完整保存。

## 2. 强对照表（同参数 N16/B3/q2/P3/resume200）

| 策略 | physical R / AR | AW | 最终 event steps | 能否在指定 P3 实现有限 pause | 备注 |
|---|---|---|---|---|---|
| open long read（候选） | 28 beats / 2 | 6 | 214 | 是 | 与强 tile 同为 214，不能声称 makespan 领先 |
| closed short long read | 16 / 1 | 16 | 236 | V 需等 A 恢复才完成 | 读取较少、AW 较多，V 未在 A resume 前完成 |
| ordinary tile exit（强对照） | 28 / 4 | 6 | 214 | 是 | 与候选同为 214；AR 更多 |
| independent long | 32 / 2 | 2 | 35 | 否 | 已一次承诺 long A W，不能在指定 P3 处实现同样有限暂停；35 event steps 不构成同保证的暂停性能对照 |

补充：no-pause N16 下 open/closed/tile 均 read16、AW16，同 48 event steps；open/closed AR1、tile AR8；independent read32/AW2 在 35 steps 完成。明确保留新机制写分片开销，AR 少不等于已实测更快。N256 early P2 下候选源读取 510 vs independent 512，节约很小，不能把某个最佳停点的共享量推广所有暂停场景。永久 pause（N16/B3/q2/P2）：open read16/AR1、V在20步完成、A停在returned2；tile read16/AR2、V在22步完成；closed只read2且V未完成。open/tile最后的 stalled 标志是脚本等待未恢复A触发idle guard，不是V死锁。另有no-pause naive长写分流负例：A FIFO3填满，V等待下一beat，发生结构循环；仅限这组小缓冲。

## 3. 前作边界与创新判断

共享读、自动同源 grouping、动态 fork、slow receiver 退出与补传都有前作：DMA-Latte v3 VI-A（HTML line308）、Multicast AXI XBAR II-A（line121-124）、Torrent III-A/C（line129-142）、RFC 2887 §5/§4、SC24 Network-Offloaded Broadcast/Allgather III-B/C/D。留下的设计点是：在 AXI first-VALID 义务下，一条仍未退休的长 physical AXI R 与独立 logical pause 之间的具体准入/退出/恢复组织——physical W first-VALID、已冻结 logical RVALID 义务结清后 A 退出，V 沿原 AR 继续；每流只有 q 宽写债务窗，无整 job/缺块 bitmap 的 payload 暂存；A 以后在精确 contiguous cursor 补读。如果同完整硬件成本的真实 RTL 保住长 R，又比强 tile 的多个 children 带来实际延迟、翻译控制或资源增量，且前作没有同样控制，则可形成独立微架构贡献；当前模型只证明受限可行性与 AR 数差异，不能独立证明 PTW 减少、综合面积或平台速度收益。所以不能以“ordinary 也能加全部提案”否决，也不能以检索未命中当首创证据。

## 4. 下一 RTL 实验（尚未开始）

仅一个 out-only RTL adapter 原型与强控制，现未开始构建，无 board/BIT。先连接真实 iDMA/IOMMU、同源不同 context 的独立许可、实际 shared source PA。根级接口核查新增一项必要条件：旧 benchmark CONF1 的 cache 字段为0，不能直接做合并读取。新候选与强对照需同设普通非缓存、可修改事务的 cache=0010（当前生成寄存器 CONF129），并核下游属性；不可修改/device 请求回退独立路径。[Arm 官方属性说明](https://support.arm.com/documentation/102202/0200/Channel-signals)支持该限制。现成 axi_burst_splitter 只拆成单beat，不会自动实现可调q；需明确实现写侧children/B汇合、保持读侧长burst。physical W splitter 保留 raw long parent，只发 q children；只在准备好的 permission/page span 内保 long R。先一次构建加少量完整 copy 的有限 pause/early resume，比较 AR/AW/B、R/W、TL/PTW、V/A 时间与完整状态成本；不先编 BIT，不重跑旧阶段。实现合同见 [RTL准入与复用合同](../out/innovation-rescue-20261009/codex-rtl-admission-contract.md)。

证据：[根级接受](../out/innovation-rescue-20261009/codex-result-acceptance.md)、[原始结果摘要](../out/innovation-rescue-20261009/accepted-model-summary.json)、[执行账本](../out/innovation-rescue-20261009/execution-ledger.json)、[跨领域前作边界](../out/innovation-rescue-20261009/codex-cross-field-boundary.md)、[下一实现合同](../out/innovation-rescue-20261009/codex-rtl-admission-contract.md)。
