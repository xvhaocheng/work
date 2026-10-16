# E/F：独立命中通路与 membership sketch 的关键判别

日期：2026-10-14。作者/源码审查：Codex Sol 6.1，约定 ultra。范围：仅关键源码、必要前作和纸面机制分析；无 DeepSeek/Claude、新模型、实验、构建、测试或生产修改。该文只写入独占产物，root 负责最终综合及共享状态文档。

## 先给裁决

1. **E 有比现有 N 更强的可描述能力，但它必须实际解除请求生命周期、查找窗口或结果保留的限制。** 现有 N 只在异 bank 的 S1 页表走访窗口探测一个合法命中；E 若支持同 bank 多个可区分事务、常驻独立命中查找/上下文、命中保留槽，以及不会依赖 miss 队列释放的结果去路，就新增了合法事件。只把 N 称为 fast plane，或只加一个命中结果 VC，不能说明新增能力。
2. **现合同下“共用翻译 response 出口”不是已证明的 HOL 根因。** 每 bank 唯一等待翻译的 TC 在 AWAIT_RESP 恒 ready=1；N 的响应在请求真实握手后的下一拍进入 OUT。合法回应的目标 TC 能接收，因此独立 hit-response VC 的当前可达收益未成立。将来多 tag/结果 FIFO 设计若产生真实目标背压，需重新给出该场景，不能从共享端口直接推收益。
3. **F 能安全地产生“确定不在缓存”和“可能在缓存”两类，不能独立产生“权限合法的命中”。** 若请求被提前接受为可能命中，却在真实查找时 miss，必须有已保留的 miss/replay 去路，或在接受前不承诺。零 false negative 只保证不把实际在缓存的项误分成不在；不保证 false positive 能完成，也不保证权限/上下文正确。
4. **当前 16-entry cache 的最强简单对照是直接精确探测/精确标签副本 + 同预算命中保留槽、miss 上限、轮询服务和 replay。** F 只有在同样资源、查找/更新端口、时序及完整任务下，节省的访问或等待超过 hash/更新/replay 费用，才有净收益。没有必要先搭新大模型。
5. **E/F 不能直接替代完整 DMA 安全准入。** 命中完成之后仍有 AR/AW/W/R/B 的容量、W 所有权与跨 bank 的 read-admission。E 若让被现有 guard 禁止的 AR 提前发出，就必须证明完整的数据与返回债务有安全容量；hit、sketch positive、独立 translation reply 均不提供这种容量证明。

## 原始源码与版本边界

源码根 `cva6/corev_apu/riscv-iommu/`。该仓库 HEAD `57eb5621802ef9ed8d77c684b9a03205c6630391`；读取的是实际脏工作树，不能把这些 N/banked 研究扩展归到该提交：`rv_iommu_top.sv`、`rv_iommu_trans_ctl_wrap.sv`、`rv_iommu_iotlb.sv`、`rv_iommu_tl_wrap.sv` 修改中；`rv_iommu_banked_ctl.sv`、`rv_iommu_nonblock_hit.sv` 为未跟踪文件。本文没有修改它们。

轻量 `rg --files` 找到本模块顶层是 `rtl/rv_iommu_top.sv`，另有 `rtl/rv_iommu_wrap.sv`，不是一个同目录下的 `rv_iommu.sv`。本文没有据 wrapper 默认参数宣布当前 FPGA 打开 N；生产使能/完整原 workload 快照仍须由 root 当前接口证据确认。

| 决定事实 | 源码入口与行号 | 意义 |
|---|---|---|
| N 只借用现有缓存端口 | `rtl/translation_logic/rv_iommu_nonblock_hit.sv:2–4`；`rv_iommu_tl_wrap.sv:249–277` | DDTC/IOTLB lookup 输入 mux 探测请求；normal hit 被探测期间屏蔽。不是同时两个独立 cache ports |
| N 的打开窗口 | `rv_iommu_tl_wrap.sv:303–306` | normal TL 必须在 PT、walker active、仅 S1、非隐式 DDT/PDT、异 bank，无 ATS/debug 请求 |
| normal 与 N 的真实请求许可 | `rv_iommu_tl_wrap.sv:282–286` | probe 不握手；只有 `n_accept` 或正常引擎真实 ready 才接受 |
| 探测上下文来源 | `rv_iommu_nonblock_hit.sv:51–55,84–103` | 先按请求 DID 查真实 DDTC，取真实 PSCID，随后检查 IOTLB；不是借当前 walker 的上下文 |
| 探测不能冒充权利 | `rv_iommu_nonblock_hit.sv:84–103` | DDTC valid、S1 Sv39/S2 Bare、无 PID/priv/debug/ATS/PRI、canonical、MSI 范围排除；leaf valid/U/read或write权限、4KiB页；同请求、同 DDTP、同 DC、无 invalidate 才接受 |
| accepted hit 有保留结果 | `rv_iommu_nonblock_hit.sv:158–172` | 响应存 `resp_q`，OUT 维持到 ready；不是无缓冲 combinational success |
| 共用翻译回应出口 | `rv_iommu_tl_wrap.sv:346–352`；`rv_iommu_banked_ctl.sv:395–408` | rr fast/slow 合流、按 bank 路由。共用但不能直接认定会阻塞 |
| 每 bank 单生命周期 | `rv_iommu_banked_ctl.sv:43–46`；`rv_iommu_trans_ctl_wrap.sv:625–725` | 只有 IDLE 选 AW/AR，REQ_TRANS→AWAIT_RESP→COMPLETE；保留 FIFO 头直到 completion 地址握手。samebank 次请求不出 translation valid |
| 唯一 pending translation 的接收者恒 ready | `rv_iommu_trans_ctl_wrap.sv:693–701` | AWAIT_RESP `trans_resp_ready_o=1`；当前合法 fast/slow 响应没有目标翻译回应阻塞证据 |
| AW-first 与 AR 准入 | `rv_iommu_trans_ctl_wrap.sv:623–668` | AW 优先；AR 受 read_admit 限制；低延迟查找也不能替代这个选择规则 |
| 完整数据路径依赖 | `rv_iommu_banked_ctl.sv:246–268` | 非 owner AR 可填 DMA R→W FIFO、等待 AW 进入 input demux，继而卡共享内存 R 与 owner source/PTW，形成环；guard 在选择前拦住它 |
| shared AXI 完成出口 | `rv_iommu_banked_ctl.sv:427–448,480–520` | 两 bank 合流到同一个 AXI completion master，R/B返回也共享；translation reply VC不是独立数据 R/B 网络 |
| 精确缓存 key 与匹配 | `rv_iommu_iotlb.sv:58–68,101–136` | VPN29+PSCID20+GSCID16+stage2+valid；global leaf忽略PSCID，superpage忽略低VPN，分类key不能漏这些合法别名 |
| invalidate 优先于 fill | `rv_iommu_iotlb.sv:198–332` | F 的 insert/delete/rebuild 必须与精确缓存相同更新顺序保持过近似 |

正常路径还支持两级翻译、process context、MSI、不同 privilege/RX/SUM 等，见 `rv_iommu_tl_wrap.sv:1068–1278,1329–1403`。E 若从有限 N 扩展成通用命中平面，必须计入真实 DC/PC 取得、stage组合、global/superpage、权限及 fault handling。不能只复制 VPN tag 并返 PPN。

## E 的候选卡

**真实问题。** 有足够合法缓存命中可服务时，前序 miss/单生命周期使下一笔请求无法发出，或独立的受保护设备请求被共享查找/结果保留资源占满。现 N 只解除其中一个异 bank walker 窗口。

**新增动作。** 在 miss 域忙时，拥有自身生命周期 tag 和结果槽的合法 hit 仍可完成精确上下文及 leaf 查找；成功以后保留完整结果并独立交付。这里的 credit 应覆盖请求持有、精确结果及必要 replay 资源，不是 MASK 的 fill token，也不是“存在一个计数器”就等于资源已保留。

**最小有界实现接口。**

- 入口先取得可唯一标识的 TC 槽（至少区分 bank 内两个 pending 请求）；保留必要 AW/AR metadata、AXI ID/sequence/权限类型、上下文epoch；新 tag 不能只用 `bank`。
- 独立 exact hit access：新 lookup port、合规复制缓存、或明确保证上界的查找仲裁。若仍等 normal engine 从 DDT/PDT/MSI/COMPLETE 回到可借窗口，就没有从全状态解除 miss 等待。
- hit result reserve：在接受之前，得到该设备可用的结果保留槽；普通 miss 不得耗尽这份最低保留。miss-specific 状态、PTW/FQ、fault backpressure不能占它。
- 所有不能完全证明的请求继续 normal path；不以 raw cached tag 命中跳过 DC/PID/permission/MSI/invalidation 检查。
- 为 FP/eviction race/上下文失效设独立 replay 票据或避免提前接受；后续真 miss仍守 normal credit 和顺序。
- data permit 另行取得：地址发布、W所有权和RWB容量不能仅由 translation success 放行。若业务本身有可靠完整输入缓冲，可给出该配置的证明；当前通用 read→write DMA没有这个默认承诺。

**比 N 更强的可达正例（纸面可行，当前未测）。** B bank 正在为请求 b0 等待长期 miss；同 bank 后继 b1 是另一 AXI ID、DC与leaf均已缓存、在普通 AXI排序规则下可合法越过 b0，b1 是延迟敏感任务的唯一剩余关键请求，completion fabric 有容量、RWB guard允许。现 TC不会发 b1；E 的第二生命周期/exact-hit端口可让 b1完成。必须对 b0/b1依赖、相同ID顺序、DMA源是否真的同时给出b1逐项确认，不能人为新增后继。

**较小正例。** A 的 DDT/PDT或 slow COMPLETE 因非 hit 原因占normal状态；B 合法hit已在另一bank TC REQ_TRANS，N窗口关闭，而独立DC+leaf查找可以服务B。要确认A对应状态实际可长期等待，且B任务完成不依赖A的数据通道。当前单 result ready 合同不支持凭空制造 response-only 等待。

**相邻反例。**

- b1与b0同AXI ID且发布/返回必须保持顺序：第二tag可提前翻译，却不能直接更早完成b1。
- 所有B请求本来已落在N允许的异bank PT窗口：E若没有更高请求吞吐需求，行为被N覆盖。
- B hit后的source R塞满其DMA R→W缓冲，而AW受其他bankW所有权阻挡：独立hit plane加速进入已有死锁环；response VC不修复数据R/W环。
- 完整任务最后仍等A的同一慢 miss，提前完成B hit也不缩短共同退休；task-level completion应同时检查关键路径与完整工作守恒。
- B 的DC miss、权限拒绝、MSI/2S/PID或者invalidate期间：Bloom positive不改变合法处理路径。

**最强同预算普通对照。** 在相同全生命周期/tag、缓存读端口、结果保留槽、FIFO总字节、miss outstanding上限及公平带宽下，给普通nonblocking exact lookup一个每设备最低保留/剩余共享规则与独立可选择结果槽。结果被blocked时可以选另一个ready destination；不用固定FIFO头制造HOL。read-admission仍按同完整RWB规则执行。这个对照实际可以执行E的大部分动作；是否有独立创新应比较E剩下的特殊信息/动作和具体不同结果，而非否认对照可使用相同已有模块。

**机制新颖性判据。** 多tag、常驻hit查找、结果credit、分离虚拟通道各自是普通nonblocking缓存/流控构件。因为已有N、SMMU microTLB及NoC reply-reservation已有相关能力，不能把“hit不等miss”“reply使用独立VC”本身作为独立创新。若E提出能同时约束translation与DMA数据债务的新许可，并展示普通精确hit+同预算保留基线无法由其现有信息作出同动作，则那项新增信息/许可规则可形成独立候选；该判断留给root footprint设计。

**最低资源合同（构件下界，不是综合成本）。** 同bank前序miss与后一hit同时存在，至少需要两个可唯一识别的生命周期以及后一请求的完整header保存；原来的一个bank bit不能区分它们。一次常驻hit验证至少要有对正确DC和正确leaf的查询服务以及结果保留。物理上可付第二DDTC/IOTLB读端口，或付带coherent更新的副本，或付保证时隙与最大等待的仲裁；不能把“借共享端口但无保证”算独立。

| 形式 | 至少保存/保留 | 必须另计 |
|---|---|---|
| E 一笔受保护hit | 一份新请求生命周期/header、一个完整翻译结果槽、一份该设备最低hit credit；同bank并发需新tag/序列映射 | DC/PC/leaf查询端口或副本、仲裁、更新广播、invalidation/epoch、AXI ID顺序、target地址/W/R/B路径资源 |
| F passive probe | hash/filter状态、probe所需请求快照；只有exact success与结果容量就绪才accept | hash时序、fill/invalidate/rebuild更新端口；原N已具备probe快照和一个结果槽，重复使用不得当新增免费的槽 |
| F early accept + replay | 每个已接受候选保存完整header/顺序身份，hit结果槽；至少有不依赖它们释放的可达replay容量/资格 | miss释放时的公平仲裁、FP移交原子性、rollback/epoch重查；若共用一个escape槽，必须防多个FP同时承诺超过escape容量 |
| 独立reply VC | 明确独立的队列/credit、目的端可接收结果的保留容量 | 同物理链路公平服务及sink不等别的request资源；只加类型bit没有独立buffer，不能提供保证 |

设新增pending数为`Q`，request全宽为`W_req`、原AW/AR header必要宽为`W_hdr`、result为`W_rsp`、replay深度`Q_rep`，则最少元数据费用只能写为实际保存字段的并集，不得把它们的重叠重复相加或直接省略；保守上界组件式为`Q×(W_req+W_hdr+W_rsp+tag/seq/epoch)+Q_rep×replay_header`，另加credit计数器、端口/控制及缓存副本。计数器只记余额而不储存事务，不能代替这些槽。尚未固定接口故完整数字费用为未定。

## F 的严格正确性合同

令精确cache中、按实际tag/global/superpage/context匹配规则可能命中的集合为 `C(t)`；sketch认可的查询集合为 `S(t)`。**零FN所需不变量是 `C(t) ⊆ S(t)`，并必须在每个实际准入/分类瞬间成立。** `S` 可以过大，不能缺少真实成员。该集合只是存在性的过近似，权限、上下文、可达lookup及结果容量是独立条件。

| 操作 | 可靠方式 | 必须支付/处理的成本与失败边界 |
|---|---|---|
| insert/fill | 所有hash位/计数在cache新valid对查询可见以前或同原子边沿发布 | fill与lookup并发需顺序、forward或禁probe；漏掉一条fill即可能FN |
| eviction/invalidate | 普通Bloom不清单成员的共享bits；允许陈旧positives | bit a与b共享，删除a清bit会将仍驻留b变FN。保留bits安全但FPR随历史增长，可到全1 |
| exact counting | 每bit精确计数、仅对真实插入/真实删除更新 | 重复插入/重复删除、同hash重复位置、同拍insert/delete必须计量一致；计数器位宽覆盖最大真实贡献 |
| saturating counting | 已饱和bit不得按普通decrement推回零；保留sticky或溢出计数/重建 | 例如容量3但真实计数4，饱和3后删除3项会得到0而仍有1项，造成FN；仅“用counting Bloom”不够 |
| rebuild | 静默重建、双epoch加更新journal、或双filter union并安全切换 | 不可清空filter同时留下cache有效；snapshot之后新fill必须追到new版本；老读者/epoch需退休；union切换不能漏并发fill |
| global别名 | global条目投影到不含PSCID的global过滤集合；查询同时查local/global | 仅hash(PSCID,VPN)会漏另一个PSCID的合法global命中；产生FN |
| superpage | 分页大小独立filter或覆盖匹配前缀 | 仅hash完整4KiB VPN漏同2MiB/1GiB页的邻页；展开放入全部4KiB页又不划算；可以限定F为4KiB候选 |
| DC/PC变更/上下文复用 | exact context检查、相关invalidate与epoch次序一致 | DID不是永久PSCID；stale positive不是许可。保守stale阳性可replay；用旧DC/PPN返成功是安全错误 |
| policy-clear/重分区 | sketch与实际cache更新规则同步；cache先变无效可保留阳性 | 重分区若允许cache新项而filter未就绪，可FN；不可假定P只影响分配就不用同步fill |

**FP 与没credit的关系要区分停顿和死锁。** FP请求拿到hit槽，精确lookup显示miss，miss队列满：它先出现backpressure，但不自动死锁。若旧miss还能独立完成、释放slot，等待是有限的。若它占了旧miss完成所需的共享TC/result/输入/返回资源，形成“FP等miss槽→旧miss等FP持有资源→FP”，才是死锁。必须画实际资源依赖并证明有逃生，不用“FP存在”直接宣称必死锁。

安全设计选项：

1. **无提前接受**：sketch positive仅启动无承诺probe；真正exact-hit与结果容量都确认才握手，失败回normal。最接近现N；不需要替已接受FP保存全事务，但不能宣称它让更多未知请求进入系统。
2. **replay保留**：accept时每个候选都有不依赖其命中槽/结果槽释放的replay位置与公平最终normal资格；精确miss可原子移交并释放hit资源。独立replay FIFO本身还需要被其后normal仲裁接受；容量与不会互等必须证明。
3. **双域credit**：每个提前接受候选同时持有miss逃生credit。最简单保守，但强miss限额时会限制真hit一起进入，可能消掉F的动机；这不是免费的解决办法。

**短哈希/指纹不等于精确tag。** 有碰撞时只能判“可能”；不得省掉最终exact tag和权限。若用fingerprint array代替Bloom，insert驱逐指纹可能让仍在真实cache的tag消失，成为FN，除非其容量/索引与真实cache一一覆盖、相应cache也被同时驱逐或采用保守overflow。

## F 的成本与最强精确基线

实际IOTLB完整tag为 `29 VPN + 20 PSCID + 16 GSCID + 2 stage + 1 valid = 68 bits/entry`，16项共1088bits；global信息在leaf content。内容结构两stage，每stage `44 PPN + 2 page-size + 6 g/u/x/w/r/v = 52 bits`，两stage 104bits/entry，16项1664bits；68+104合计2752bits。来源 `packages/rv_iommu_pkg.sv:76–86,315–330`、`rv_iommu_iotlb.sv:58–68`。这是存储字段小计，不含PLRU、DDTC/PC、端口、mux、组合比较、pending元数据、队列、时序、CDC，不是FPGA面积数字。

有限S1/4KiB的membership目录可只保留 `VPN29+PSCID20+valid1+global1=51bits/entry`（816bits/16项），但要明确其它stage完全不进入F覆盖，且仍需DC验证。若只服务预先固定且epoch安全的一个context，每项还能减字段；减字段必须由真实绑定合同保证，不能默认DID恒等PSCID。当前exact cache为组合16路比较；新增sketch并不是省掉一个已有多拍大SRAM lookup，可能多一道hash和更新控制。

纸面Bloom示例：16驻留项、128bits、4hash，均匀独立hash下理论 `p_fp≈(1−exp(−4×16/128))^4≈2.4%`。这是估计而非实测，并假设filter描述当前驻留集合；不删除时n是历史插入集合大小，64个历史不同项时同公式接近56%。hash相关性/恶意输入也可恶化。正确counting若每项对同bit只贡献一次，16驻留项每bit计数至少能表示0…16，即5bits/bit：128×5=640bits，另加hash、更新及重建元数据；若按每个hash位置累加，则最坏每bit64贡献、需7bits即896bits或处理重复hash。约128-bit的bit filter小于tag目录，并不能推出完整实现更便宜。

必须公平比较两条具体baseline：

- **Exact-first N extension**：借相同lookup端口/结果槽作真实DDTC+IOTLB probe，hit成功才握手；失败回normal，不消耗misscredit。比较F省下的无效probe能否超过新增hash/update路径。
- **Exact replica/microTLB**：相同总预算给精确key目录（或少量含PPN/permission的本地microTLB）、合法的更新broadcast及epoch，按同设备保留credit服务；目录只作分类时仍最后读内容。不把baseline强制只看FIFO头，不强制无global/superpage处理。新增端口/上下文复制费用计入两者。

F的正例可以是miss风暴持续占用稀缺exact lookup bandwidth，sketch可靠negative让无命中候选直接走普通miss资格，省下的lookup让命中任务更早完成；这需要有真正稀缺的查找服务时间和多个候选，而当前16路组合IOTLB+两bank单tag不能直接提供该正例。相邻反例是filter历史饱和、全部positive、exact lookup仍跑每项；F只添加hash及replay费用。另一反例是miss域没有可用credit时大量FP占满hit/replay保留，使本来可命中请求在队头等；没有可挑选队列和逃生时F反而恶化延迟。

因为普通Bloom过滤器和精确lookup后fallback的构件本身不能提供DMA完整准入，不能把“零FN分类”叫作完整任务安全新能力。如果F结合root候选产生一种新的受资源约束前瞻信息，并通过真实动作差异/完整任务和同预算精确baseline，则该组合中的新许可关系可以保留为独立候选；否则F仅作为实现优化备选。

## 必要前作与准确覆盖

以下只用primary paper/source。浏览读取于2026-10-14；不将搜索摘要当作机制证明，也不引用无关误中条目。

| 前作 | 原文入口 | 对E/F的实际覆盖与不覆盖 |
|---|---|---|
| MASK, ASPLOS 2018 | [论文PDF](https://people.inf.ethz.ch/omutlu/pub/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)，§5.2，PDF第7页，§5.3，第7–8页 | 所有warp都可探shared L2 TLB，token控制fill资格；无token结果进入32项TLB bypass cache；两者并行tag probe；PTE修改flush两缓存。它覆盖fill-token/bypass缓存/翻译缓存干扰治理，token不是每device request/response保证，不能据此声称它已给出当前DMA端到端安全hit-credit规则 |
| Arm CoreLink MMU-500 TRM DDI0517B | [Arm官方PDF](https://documentation-service.arm.com/static/5e908aac8259fe2368e2b389)，§2.4.1，2-16页 | TBU有fully associative可配置depth microTLB，缓存TCU返回PTW结果，先查microTLB再TCU。已存在本地命中/中心miss分层，因此把本地fast plane本身当独立创新不成立。本文读取到官方搜索索引该节内容；直接open PDF遇Internal Error，未据此延伸FIFO/credit实现细节 |
| Beyond the Bermuda Triangle of Contention, 2025 | [作者论文arXiv](https://arxiv.org/html/2508.19670v1)，§III，HTML 217–230；§V-C/D，531–537 | 实测SMMU共享干扰并明确microhit/macrohit/PTW三级；提出QoS/partition等缓解方向但未全面评估。支持真实问题与现有分层，不证明某具体资源隔离协议已完成，也不能把其平台数值直接用于本平台 |
| Chaudhuri & Heinrich, TPDS 2004, Exploring Virtual Network Selection Algorithms in DSM Cache Coherence Protocols | [论文PDF](https://www.openu.ac.il/home/wiseman/sds/mainak.pdf)，PDF第2页，文字85–96 | 两lane协议请求执行前需回复队列有空间，回复是可服务sink；分离虚拟网络不能代替endpoint资源保证。覆盖reply资源先留/依赖图的原理，不是IOMMU/DMA专用协议 |
| NB-Walker, MICRO 2026 | [作者页面](https://maxkev1n.github.io/publications/micro-2026/)；[作者artifact仓库](https://github.com/redbird-arch/micro2026-nbwalker-artifact) README；[Zenodo作者artifact](https://zenodo.org/records/21488605) | 作者/代码发布入口能确认题目与NUMA GPU nonblocking PTW议题；公开作者页面无正文/PDF链接，repo可见README/config/script不足以核完整核心机制。因此只列关键未闭合前作，不凭名称宣布它覆盖E/F，也不伪造论文内容。需要root若推进walker类论文主贡献时定向取得正文/核心代码；本轮不扩大下载/复现 |

## root下一判别题与不准入条件

优先判：**完整任务中是否有一笔N不能接、普通同预算exact-hit/credit基线也不能凭既有信息选择，而E或F+新完整准入能安全选择并缩短实际退休的请求？** 先固定合法供应序列、cache/DC/epoch、bank/ID、完整RWB、原后继任务和全部资源，给一个正例及相邻反例。

若只有“另一bank命中绕过walker”，由N直接覆盖；若只有“reply VC”，当前接收者恒ready的源码已否定该具体HOL解释；若只有“Bloom negative准确”，不提供许可/资源新动作。这些原因只结束对应主张，不否定多DMA干扰主问题或root的新数据债务准入方向。

未闭合：生产使能与当前workload快照；同bank后继是否真实并发/可乱序；exact端口占用是否是关键等待；DDTC/PC复制/invalidation/epoch；FP逃生资源依赖；完整RWB安全；共同任务退休因果；NB-Walker正文覆盖；总字节/端口/hash/update/面积/Fmax/能耗/CPU供给成本。所有性能数字、总工程成本及净收益均未测，不能标passed。
