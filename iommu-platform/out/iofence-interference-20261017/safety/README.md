# strict unmap 与选择性 payload fence 的安全范围

2026-10-17。ARS-Codex `deep-research / fact-check` 的有界原文核查；Codex-only。本文只回答安全性与当前模型边界，不提出机制、合同或后继实验。未修改生产 RTL/Linux、FPGA、论文或既有实验；未构建、仿真或板测。主会话已通知第三门前作不通过，机制工作停止；本文不重新裁决第三门。

**结论：按 domain 等待可以有条件安全；按单设备等待没有无条件安全性。** 标准 RISC-V `IOFENCE.C(PR/PW)` 本身没有 domain/device 选择字段，承诺覆盖所有已处理的先前对应请求。因此“特定帧可以安全回收”和“可以把标准全局 fence 提前报告完成”是两个不同命题。现有 RTL 的 PR/PW TODO 是真实功能缺项；旧测试 overlay 只快照 `comp` 已接受请求，存在漏掉已完成翻译、仍停在 IOMMU 内部的请求的源码级风险。没有证据表明历史28配置实际触发了该窗口，不改写旧结果。

## 1. 来源及版本

规范使用已保存的 Ratified v1.0.1（2026-02-22），dated tag `20260222`，commit `054005f4ffe30838a4ffe144a01ea3933ffea6f2`；关键章节为 **§4.1.2，印刷页50–51** 和 **§7.3，印刷页89–93**。本次重新从官方仓库 HTTPS 取源，并在线查得 main 为 `97e23d078a50822bb247c564af31c2645b4c36ca`（2026-10-14）；main 的 queues、software guidelines 两文件与上述 ratified 版本字节完全相同。不能据此推论整份 main 规范没有变化。

Linux 固定 `58161ead29e50d70a317ea26b472e0c7508e69da`；RTL 是历史 `dead-translation-20261017/frozen` 内的修复基座 IOMMU `85b157ebe71a623e326e51fff7b7126fe77e8eb9`，不是活动研究 checkout。`runner/rtl-overlay` 是历史独立测试输入，不是生产实现。完整 URL、hash、取回时间、源码行号见 [来源导航](sources.md) 和 [source.json](source.json)。

## 2. 安全回收究竟要等什么

以下把“撤销旧映射”“把物理帧交给新 owner”“把 IOVA 交给新映射”分开。只释放一个仍共享帧的引用，不等于允许立即改作其他用途。

1. **软件完成与停止再发。** 正常 Linux streaming DMA API 要求 DMA 活动完成后才调用 unmap；固定版本官方 DMA 文档 `601–602,666` 明确如此。驱动必须依据具体设备的完成合同停止对该映射的新工作。一次 COPY 的 DONE 只证明这次 COPY 的检查条件；不能作为任意设备、其他队列、其他 PASID 或其他设备全部静默的证明。
2. **清除访问权限并发布页表写。** 软件先修改 PTE 撤权，在修改对 IOMMU 全局可见后发匹配的失效命令；必要的 CPU cache 维护与写顺序须满足平台要求。规范软件指南 `123–129`、queues `386–397` 给出该顺序。
3. **正确完成 translation invalidation。** 按页表层级与地址空间发 `IOTINVAL.VMA/GVMA`，有 DDT/PDT 变化还要相应目录失效；若设备有 DevATC，先失效 IOATC，再向所有相关 DevATC 发 ATS invalidation，等待成功完成。缓存失效不撤销已保存在事务中的 PA。
4. **把已经处理的旧 payload 排到全局顺序点。** 规范 queues `368–397`：原先可读的内存至少要排序先前读；原先可写时要排序先前读与写。旧写不能在新 owner 写入后再次覆盖该帧；旧读不能在帧存入秘密后才取数。Linux helper 当前统一设置 `PR=PW=1`，不是按单次 DMA 方向删掉某一位。
5. **处理尚未被 IOMMU 处理的设备在途请求。** PR/PW 不保证这部分；规范 queues `399–406` 要求在需要时使用互连特定 fence。若设备/驱动完成合同已保证不存在这类旧请求，可由该合同排除；若没有，不能仅靠 IOFENCE 推断排空。尤其 IOVA 再用后，一个旧的、尚未处理的 IOVA 请求可能命中新映射，不能靠清旧 PTE 修复这种生命周期错误。
6. **成功完成后再复用。** 必须观察真正的完成，并处理 timeout/fault；不能把“已提交”或普通命令 CQH 前进当完成。RISC-V fence 的 CQH 越过 fence 是标准完成证据。解除 allocator 保留或释放 page-table freelist，须在所需同步完成后进行。

固定 Linux 证据：`dma-iommu.c:823–843` 清 PTE，strict 路径同步后归还 IOVA；RISC-V `iommu.c:1005–1112` 按 PSCID 失效所有相关 IOMMU，`517–535` 提交 IOFENCE 并等待，`iommu-bits.h:766–771` 设置 PR/PW。`iommu.c:1268–1274` 在同步之后释放拆除的页表页。这里证明成功路径的源级顺序，不证明驱动错误路径已可靠阻止回收；其 sync 是 void、错误仅日志，历史来源核查已经披露。

**页表页需要单列。** payload fence 不自动证明所有 implicit page-table reads、walk cache 与迟到 fill 已结束。当前夹具只改预先存在的4KiB leaf，不拆表；不能把其 payload 安全结论扩展到 page-table freelist 回收。FQ 保留的 IOVA、拆表页与 payload RAM 也不是同一资源。

## 3. 单设备范围的最小失败执行

这是接口安全性反例，不声称当前两 iDMA 测试实际执行过此序列，也不把违反正常 DMA API 生命周期的行为伪装成原生工作负载。

设 A、B 都属于 D，共用 `x→F` 映射，软件准备撤销此映射并回收 F：

| 顺序 | 事件 |
| --- | --- |
| 1 | B 的旧写 W 已通过 D 的合法翻译取得 PA=F，仍在 IOMMU/下游队列，尚未到全局顺序点。 |
| 2 | 软件停止相关新工作，清 D 的 `x` PTE，正确完成该映射失效；A 的旧请求已排完。 |
| 3 | 错误的回收协议只等待 A 的 device-scope fence，忽略 B 的 W，随后把 F 交给新 owner。 |
| 4 | 下游继续 W，在新 owner 数据之后覆盖 F。 |

失效 PTE 不能改变 W 已持有的 PA。把 W 改为旧读，可得到回收后读到新 owner 数据的相应反例。若系统的实际 DMA 完成合同已在步骤2前保证 W 全局完成，此特定执行被合同排除；这正是**附加条件**，不能证明单设备接口无条件安全。若 scope 包含 D 所有旧设备请求，步骤3必须等 W，该反例消失。

反过来，若 A 独占 F，B 不能通过任何 alias、ATS、guest 地址空间或旧 binding 访问 F，那么 B 的不相关 payload 并非 F 回收的必要前驱。这个事实允许条件式域/设备范围分析，不支持把所有设备范围一概判为不安全。

## 4. domain 范围的充分条件与证明

令 F 为待改作新用途的 payload 物理帧集合，D 为其旧授权范围，T 为对 D 的撤权与失效真正完成的界线。以下是**充分条件**，不是当前平台已验证具备的通用能力：

| 条件 | 必须包含的内容 |
| --- | --- |
| 授权闭包 | 任意能按旧权限访问 F 的 requester 都在 D 的 fence 集合中，或已经以独立可靠完成证明排空。D 中所有设备、所有相关 IOMMU、全部有权限的 PASID/SSID 均覆盖。 |
| alias 与共享帧 | 已撤销 F 所有须撤销的 IOVA/PA alias。若另一 domain 仍合法映射 F，则 F 仍是共享对象，不能单凭 D 的 fence 交给不相关 owner；需要其他 owner 撤权/引用生命周期，或有独立静默证明。 |
| 正确失效 | PTE 发布、IOTINVAL、需要的 IODIR、ATS/ATC 等均成功完成；不存在仍会把旧授权重新引入的在途 walk/late fill。全局 PTE 不能误用排除 global mappings 的 PSCID 失效。 |
| 稳定身份 | 请求在取得旧授权时绑定 D 及其 incarnation；设备/domain/PASID/PSCID/GSCID 重绑和 ID 再用不能把旧请求重新归类到新 D。可用先彻底排空再重绑满足；不能只查完成时的当前 DID→domain 表。 |
| 请求覆盖 | 已处理旧请求在 fence 集合中，即使它仍停在 TC、spill、demux 或下游队列。未处理旧请求已由设备/互连完成合同排除或排空；新请求不能在撤权后继续取得旧授权。 |
| 完成含义 | 等待的是对应请求到足够的全局顺序点，读/写类型符合原权限；确认的是集合完成，不是可能被年轻请求凑足的无身份总计数。 |
| 回收对象 | 本证明针对 payload F。回收页表页、DDT/PDT 或 MRIF 等结构须另外满足隐式访问及对应协议完成条件。 |

**条件证明。** 反设 T 后将 F 改作新用途，仍有一个旧权限访问 q 破坏新数据或读取新秘密。若 q 在撤权前已取得旧授权，授权闭包与稳定身份使 q 属于 D；请求覆盖使其已被排空或属于 fence 集合，完成条件使其读数/写入已在复用前被全局排序，矛盾。若 q 尚未取得旧授权，正确失效及未处理请求条件使其在 T 后不能取得旧权限；若它因 IOVA 再用得到新授权，那属于被请求覆盖/驱动生命周期条件禁止的旧请求再发。域外 q 要么无权访问 F，要么被授权闭包中的独立完成/共享 owner 条件处理，也不能构成反例。因此，在这些条件下无需等待无权访问 F 的其他域 payload，便可安全回收 F。

**缺条件的直接反例。** 另一域 E 仍持有 `y→F`，D 完全排空后 E 再写 F，说明 domain fence 不替代所有权管理；这也不是选择性 fence 独有的失败，全局 fence 同样不会撤销 E 的 live 映射。旧 D 请求持有 PA=F，设备被重绑到 E，排空时按“当前域”只计 E，说明缺少旧身份或排空后重绑会漏旧请求。一个设备有多个 PASID，或多个 guest 一阶段空间经共同二阶段映射 F，只凭单个 DID/PSCID 筛选同样不自动形成上述闭包。

Linux protection domain、硬件 PSCID、device_id 与 PASID 不能直接视为同一标识。host first stage 可按 PSCID；guest 需要相应 GSCID/PSCID 组合，二阶段 GVMA 的地址是 guest physical address，且可能影响多个一阶段空间。规范 §§4.1.1、7.3.1–7.3.7 给出这些区别。

## 5. 旧 `comp accepted` cohort 的具体漏洞及可接受边界

历史 C++ `invalidation_dma.cpp:388–398` 在 fence DECODE 快照 `comp_reads/comp_writes`，含当拍 comp 握手；`408–409` 只在 comp AR/AW 握手时增计数，`470–472` 用 DDR RLAST/B 更新完成。该快照准确表示 **comp 已接受的请求集合**，未证明等于规范的已处理集合。

直接源码链：

- TL `rv_iommu_tl_wrap.sv:1351–1373` 在 COMPLETE 发送翻译响应。
- TC `rv_iommu_trans_ctl_wrap.sv:645–654` 在 AWAIT_RESP 取响应，把 PA 等内容寄存后进入 COMPLETE；`286–311` 从已寄存 PA 构造 comp 地址。
- TC `669–678` 只有下游地址 ready 才离开 COMPLETE；因此可已经有旧 PA、却尚无 comp accepted 计数。TC `97` 的 `in_flight` 也不是 payload 到全局点计数。
- CQ `241–260` 取命令仅依赖 enable 与 head/tail；没有等待 TC 的接口。它在 `310–339` 直接发 IOTINVAL 脉冲，故上述请求阻塞不强制阻止 fence DECODE。

可达风险执行是：成功 TL→TC 响应 → comp 背压 → fence 快照没有此请求 → 快照已有集合完成后 fence 返回 → 该旧 PA 请求随后才被接受。它不需要伪造内部状态，是 RTL 分离的控制通路允许的交错。**本项没有运行该反例，也没有统计历史28配置在每个 fence 是否出现差值**，因此只认定覆盖风险，不宣称已观察到旧实验错误或安全破坏。

主会话讨论的修正边界为：以**正常 payload、非 error/ignore/MRIF 的成功 TL→TC response handshake**作为模型的 processed 事件，关联原始设备/域身份，第一次 fence DECODE 按读写及 lane 快照且包含同拍；再确认这些具体请求到模型全局点。当前 TC 在 AWAIT_RESP 恒 ready，TL normal COMPLETE 才 valid，因此这个边界能覆盖已转交 TC 以及 comp 背压/后续 demux 中的 payload。**这是模型的明确处理前沿，不宣称规范规定所有实现必须使用这个信号；本轮未构建或运行这一修正。**

需保留每次快照的 `processed-minus-comp`、请求身份及完成对应关系；结束时 `processed = comp accepted = DDR completed` 是必要守恒，却不能独自证明中途 fence 未提前完成。若实际实现允许完成重排，按 lane 的总数阈值还必须防止年轻请求越过旧请求并替它满足阈值。历史行为 DDR 的 FIFO 特性是当前计数法的条件，不是通用内存系统保证。

## 6. 在途 walk/late fill 是另一条风险

已读源码表明存在独立并发风险：PTW 端口 `35–84` 没有 inval 输入；TL `550,552` 分别把失效和 update 接 IOTLB，`579` 未使用 PTW active；PTW `393–395,478–480` 可接收已发出的 PTE 读并稍后发 fill；IOTLB `204–305` 让失效优先于同拍 fill，但 `305–318` 在后续无失效周期可重新置 valid。

所以，不能仅凭一次 IOTLB 清 valid 就证明旧 walk 不会迟到填回。可是，把**所有尚未完成的 walk**都算作 fence 已处理 payload 也不正确：它们还未成功产生 payload，可能 fault，必须先由 translation invalidation 的并发合同处理。成功 TL response 前沿没有自动修复这个问题。

本轮没有运行 late-fill 反例；没有升级为已复现实装 bug。当前历史工作流 A 每次 COPY DONE 后才 unmap 相应 leaf，可以排除该 A 映射在 unmap 时仍有这次 COPY 的同 key 活跃翻译。B 的其他 key walk 可能并行，但不据此直接否定该 A 回收条件。推广到活跃撤权、页表拆除或更一般设备队列时，必须另证旧 walk 不能在撤权完成后重新提供旧权限。

## 7. DDR RLAST/B 与真正的全局顺序点

规范只要求到达可以被系统其他 harts/IOMMUs 观察的全局顺序点，**没有要求所有实现等待 DRAM 读响应全部到达设备**。在某个具体互连中，读值已被确定且已建立所需全局次序时，可能早于最终 RLAST；写进入保证可见和顺序的 coherence/ordering 结构，也可能早于 B 返回。是否成立由该实现的协议及内存模型证明，不能从接口命名推断。

当前行为 DDR 在模型 `write64` 时写数据，以相应 RLAST/B 握手作为完成记账点（`cpp:432–471`）；这是对该模型的保守完成选点，便于完整字节核验。模型没有执行真实 CPU cache/coherence 系统，不能用它证明实机全局点，也不能把模型等待拍数外推成规范不可避免延迟。主存保证与 I/O memory 也要分开：规范 `408–411` 将后者留给实现及 I/O 协议。

## 8. RTL PR/PW 与 CQH 完成缺项应如何表述

冻结生产 CQ handler `362` 明写 PR/PW TODO，合法 IOFENCE 分支没有 payload drain 输入/等待；`276–283` 在命令读取完成、尚未 DECODE 时就推进 CQH。两者与标准有可判定的差项：

- **PR/PW：** 若先前已处理 payload 尚未到全局顺序点而该 fence 被报告完成，违反 queues `368–376` 的请求；因此这是功能正确性缺口，不只是缺少性能优化。若某个具体执行此前已全部排空，则不会在该执行发生数据破坏，仍不能宣称通用 PR/PW 功能已实现。
- **CQH：** 标准允许普通命令在读取后推进 CQH，但对 IOFENCE，越过 fence 具有完成含义。若 CQH 越过后仍等待先前 ATS invalidation，或 AV=1 的 marker 尚未完成/最终会 fault，软件会过早观察完成；规范 `348–356,414–420,528–536` 不允许这种情形。当前没有 AV/ATS 等待且所有前驱已完成的简单执行，不会仅因实现早一拍推进就必然构成可观察错误。
- **历史 overlay：** 它仅对 PR/PW fence 延迟 CQH，并加入 WAIT_PRPW；AV+PRPW 与 ATS 被明确排除（overlay `291–298,414–426,532–539`）。所以它是窄测试修补，不能当通用 PR/PW/AV/ATS conformance 修复；第5节的集合覆盖风险仍成立。

当前已有基准主要验证拷贝和失效干扰，没有在回收后的帧写入新 owner 数据并检查迟到请求。准确说法是“源码存在规范功能缺项，历史特定完成轨迹的安全影响未测”，不能写成“已经导致 Linux 数据损坏”，也不能因为没有回收场景而把缺项改称完全符合标准。

本文为 AI 辅助原文核查；来源事实、条件证明及未运行风险分别标注。原始在线取回记录和本地源 hash 已保留；没有调用 DeepSeek/Claude，没有新增实验。
