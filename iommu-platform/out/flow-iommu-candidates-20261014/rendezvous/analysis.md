# A / TRT 候选卡：在途上层 PTE rendezvous

日期：2026-10-14。Sol / Codex 独占机制与关键原文审查；没有 DeepSeek、Claude、模型、仿真、构建或生产改动。下面算术为手推假设，不是测量。

## 1. 裁决与真实新增动作

**宽泛 TRT 应重设计：不同最终页的待请求共享尚未完成的上层 PTE 读取，并保存续走位置，这个动作已被 Neighborhood-aware Translation（MICRO 2018）的 IOMMU 机制覆盖；它不是只有事后 PWC。** 当前平台确实缺少 miss join，但补上已有动作是实现贡献，不能单凭“跨设备”或新表名作为独立机制创新。

真实问题是：两个合法上下文正在请求不同 4KiB 页，其 walk 路径存在共同上层 PTE；若先各自占槽并重复发读，会增加服务量和占用。TRT 新动作应精确定义为：在 PTE 返回前，接受并关联第二请求；返回后，为每个已接受 waiter 保存它自己的有效续走状态，各自完成剩余路径、权限和响应。不同最终页通常仍需各读自己的 leaf；共享页表页不等于共享一个 PTE 值。

若要继续作为研究候选，须提出前作没有提供的具体能力，例如有明确失败场景的跨隔离上下文部分路径共享、失效期间的可证明有限占槽和撤销协议；还须说明普通 per-level PWC、同 PTE MSHR merge 和 NA-PTW 为什么在同语义与预算下不能完成。仅增加 context tag / epoch，是正确实现的必要成本，本身没有建立独立贡献。

## 2. 最接近原文：动作边界而非题名比较

| 原始来源 | 已直接核到的机制与判别 |
|---|---|
| [Shin et al., Neighborhood-aware address translation, MICRO 2018, pp.352–363](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf)，§IV pp.356–358，Fig.8 在 pp.358–359 | IOMMU pending buffer 增加 PA/PL/CL；PWST 记录正在 walk 的 VA 和 level。若待请求与 ongoing 上层访问可合并，CL 阻止另起重复 walk。读返回后按对应 PTE 写各自下一层 PA/PL，之后从更深层开始。**包含同上层 entry、不同 leaf 的 TRT 场景**。本地全文 [neighborhood2018.txt](../../shared-translation-literature-20261006/papers/neighborhood2018.txt):413–453、500–559；本次线上 PDF fetch 超时，原文核读使用既存本地 PDF/text。 |
| [Barr et al., Translation Caching: Skip, Don't Walk, ISCA 2010](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)，§3.2–3.3（PDF 第3–4页） | STC/UTC 用 VPN prefix 索引 partial translation，选择最长匹配前缀跳过已完成层；TPC 保存路径。它建立 PWC/partial translation 强对照，不能把可复用上层结果仅归为完整 walk 完成后的结果。本文没有在这几节建立 active waiter joining 协议；不把它直接当 TRT 前作。原文 [translation_cache2010.txt](../../shared-translation-literature-20261006/papers/translation_cache2010.txt):261–296。 |
| [NB-Walker 作者 artifact v2](https://zenodo.org/records/21488827)，指定 commit `8837da7dda5591837877d20d4ad51b042c988aad` | 原始 `NBWalkerMMU.go`:77–78 双 context；191–218 primary 等内存时执行 secondary；293–340 将 PID/Address/PPNWithOffset/Level/MsgID 发往 L1V 后释放本地 secondary；557–583 从返回重建 continuation，每个非最终层即 fillPageWalkCache。证明非阻塞/续表槽释放已有具体实现；**所读函数没有证明不同最终页共同上层 PTE rendezvous，不能由标题推断覆盖**。本地 [源码](../../translation-lifetime-boundary-20261008/map-agent-sources/nbwalker-source/simulator/mem/vm/mmu/nbwalker/NBWalkerMMU.go)。[作者主页](https://maxkev1n.github.io/publications/micro-2026/)和[官方程序](https://www.microarch.org/micro59/program/)仅核题录；没有取得论文正文，也未运行 artifact。 |
| [ShareMMU camera-ready, MICRO 2026](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf)，§V-B/V-C（PDF 第5页） | PPN 标识 Secure Vector；逐加速器权限、Alias Table、Access Mask 和 inclusive back-invalidation 支撑已完成翻译的安全跨设备复用。原文 [sharemmu2026.txt](../../shared-translation-literature-20261006/papers/sharemmu2026.txt):364–410。这些段没有在途上层 waiter 协议，不能据此直接否决 TRT；却说明“跨设备复用必须各自授权”并非空白。 |

NA-PTW 的合并单位是 **64B cacheline 的 neighborhood**，涵盖 8 个不同 index 的 PTE；给每个 waiter 提供其对应条目，并非把 owner 的 PTE 广播给所有 waiter。same-entry TRT 是它的子集。其示例 Req0/Req1 共上层路径但 leaf 不同；Req2 在某上层 index 不同、PTE 同 cacheline，也取得它自己的下一层地址。本平台 `rv_iommu_ptw.sv`:277–280 是 8B、len=0，不能直接使用 NA 的额外七个 leaf PTE 收益。

普通 walk coalescing 要区分：同最终 VPN 的 miss 合并、物理同 PTE 读请求的 MSHR 合并、NA 的不同 VPN 部分路径合并。[Scheduling Page Table Walks, ISCA 2018](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)的核心是请求调度（本地 `walk_scheduling2018.txt`:19–29、464–465），不能拿其“coalescer”泛称替代 NA 的机制级证据。这里没有声称完成全域文献/专利搜索。

## 3. 当前平台：有排队容量，不等于有第二 walk

关键源码均在 `cva6/corev_apu/riscv-iommu/rtl/`，按行可核：

- `rv_iommu_top.sv`:358–415 选择 banked controller；481–565 / 565后是 MSI 配置互斥分支，各只有一个 `rv_iommu_tl_wrap`。`transaction_controller/rv_iommu_banked_ctl.sv`:367–391 仲裁两 bank 到一个翻译请求端口；400–408 通过 `.bank` 路由一个响应端口。bank 数不等于 PTW 数。
- `translation_logic/rv_iommu_tl_wrap.sv`:174–176 的 head request 仅在最终响应握手后释放；201–213 是 input spill。**spill 不是单槽**：`vendor/common_cells/src/spill_register_flushable.sv`:40–101 有 A/B 两寄存器。`NonblockEnable=0` 时第二请求可能已经在 spill 握手排队，但队尾不进入第二 DDT/PT context，不能 join 当前 walk。
- `NonblockEnable=1` 时 `tl_wrap.sv`:284–286 的 normal gate 要求 IDLE 且无 head；303–329 的侧门限另一 bank。`rv_iommu_nonblock_hit.sv`:94–104 明确要求 DDTC 安全 + **IOTLB hit** 才 accept；miss 不握手、不拥有结果。未握手的第二 VALID 可以被观察，但不是已获授权的 waiter。
- `rv_iommu_ptw.sv`:38–84 只有一个 init 和一份事务/context 输入；90–115 只有一个 state/level；324–331 仅 IDLE 启动；386–397 先一次 AR 再等待一次 R，无独立 join、waiter、resume 或 owner-tag 接口。`tl_wrap.sv`:759–792 只例化一个 PTW。`rv_iommu_iotlb.sv`:106–109 的 PSCID/GSCID 匹配是 leaf cache 语义，不能当作跨上下文 partial-path 合法证明。

因此当前代码不可能直接形成两个 active miss waiter 的 TRT。最小改动涉及：第二请求的正式 admission / context 验证；head/队尾可见的匹配；PTW 的 PTE 地址/level/返回广播；waiter continuation；按 waiter 的结果/错误回传与退休。若只观察未握手 VALID，则必须持续核 stable candidate，在真正 `valid && ready` 接受前不能完成或消耗该请求。扩成超过两 bank waiter 还改变 controller admission、tag 容量和生命周期；不能只计 TRT 表。

## 4. 同预算强对照与有界算术

合同：两请求 A/B 已合法就绪，三层 Sv39/4KiB，前两层为相同 PTE，leaf 不同；页表不变、无 fault/A-D写；单 PTW/单存储服务位置，每个 PTE 的服务及必要状态开销为固定 L>0；B 在首层返回前到达 a∈[0,L)。独立响应握手均 ready。下面只算翻译子图，不算 DMA 全任务。

| 实现 | PTE读服务数 | A完成 / B完成 | B自到达的等待+服务 |
|---|---:|---|---|
| 原无上层缓存，串行 A 后 B | 6 | 3L / 6L | 6L−a |
| TRT/NA式 pending continuation，先完成 A再 B leaf | 4 | 3L / 4L | 4L−a |
| 公平普通 PWC：每个 nonleaf 返回立即更新、B dequeue重查最长prefix | 4 | 3L / 4L | 4L−a |

这是原基线中的明确正例：少两次重复上层读，B少2L；**同时是独有收益的反例**：公平 PWC 同样4次读、同完成时刻。允许 PWC 回收旧项、同预算分配两项，不允许人为规定只在整个 walk 完成后填充或用 arrival 时陈旧 lookup 冻结 B 的路径。NB-Walker 源码173–185、575–583也实际逐层 fill。

若 TRT 在2L后优先服务B leaf，则 B=3L、A=4L；同 context 与调度预算的普通 cached-prefix continuation 也须允许相同次序。由换优先级带来的单设备改善不自动是共享收益。若两个独立 walker 同时发同PTE读，强对照必须含同物理PTE的在途 MSHR merge；如果 merge 已把两次读合一，不能再次把省读归 TRT。NA 的 wait-before-issue 控制亦必须作为直接机制对照。

相邻反例：仅第一层共用时服务6→5、公平PWC也5；无共同entry时两者6、TRT匹配/排队更慢；B在A结束后才到达时没有in-flight窗口、PWC仍受益；同页表页但 index 不同且只返回8B时不能合并为一次可复用值；域/epoch不同必须拒join或独立合法化；TRT满表、owner长期受阻或失效重放会使等待增大。服务/互连/响应任一无上界就不能推出有限等待；本合同仅给4L−a，不给真实板端期限。未来有界服务Lmax、N个已接受独立请求、保持不饥饿且无重放时，三层最坏读量≤3N只是保守子图界；全部admission/response/context成本必须另加。

## 5. key、权限与全部成本

**两个候选 key 层次不能混用。** Conservative logical join key 至少关联已验证页表根/模式、stage、level、相关 VPN prefix、PSCID/GSCID/上下文身份及有效代；first-stage 经过 second-stage 的节点还需相关 guest/host provenance。Physical-read merge key 用实际8B PTE物理地址（node PPN + index*8），但每个 waiter 必须先独立到达合法 fetch 状态，并兼容读取/解释属性。相同 node PPN 不能确定 index；相同 IOVA 不能确定 root；相同 raw PTE 不能自动复用另一域的最终PA/permissions。

每个 waiter 留自己的 IOVA剩余index/offset、访问种类、特权/context、累积权限/全局属性、stage状态、bank/owner及fault关联；nonleaf返回可给同 entry 的 waiter 同一个raw值，随后独立验证和推进。leaf只有匹配该 waiter 的条目才可解码；最终结果按自己的权限、context和tag回填/回应。失效/上下文更新必须遵守现有命令顺序，选择排空或取消重放；若用epoch，防旧owner返回把已失效continuation重新变有效，并计epoch回绕/退休成本。epoch不是无成本证明。

完整新增成本类别：owner表与PTE key/CAM匹配；waiter表或链/bitmap、续走状态、满表回压；第二DC/PC验证及cache端口；PTW暂停/恢复mux与仲裁；共享R错误展开与逐waiter权限/fault；IOTLB回填带宽；单response端口的队列、held-valid、bank/tag/退休；owner退出后waiter存活；invalidation/epoch/drain/replay；合法共享识别的软件/页表约束；额外输入/输出端口、比较器关键路径、时钟/面积/能耗。若改变PTE读取8B→64B，另计总线/line buffer/bandwidth与失效粒度。当前无完整bit/area/Fmax/实际收益数字，不以一个小表的bit数代替总成本。

## 6. 推进条件

保持为**重设计候选**，不进RTL、不跑大模型。若找到真实多device原工作，在已验证共享路径且second miss可以正式admit的窗口中，普通逐层PWC、物理same-PTE merge、NA pending continuation均不能满足同一资源/撤销/等待合同，而新动作可满足，并计全成本，则可进入一个有界判别模型。若只需原平台加PWC就把上述6L变4L，应按已有优化实现看待，结束独有性能分支。NB-Walker正文缺口单独保留，不用metadata否决；NA原文已经足以裁决宽泛“在途上层共享”主张。
