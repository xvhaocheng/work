# B1-STRIPE-COST-1：结果存储、端口与 owner 生命周期卡

2026-10-16；storage-cost 独占席，Codex-only。建议/请求 `gpt-6.1-sol` / `xhigh`；工具未提供本席独立运行元数据，实际模型/强度均记 `unknown`。使用 ARS-Codex 的有界问题、同条件比较与原始证据核查方法，不启动完整研究 pipeline。AI 辅助分析；源码是证据，布局与公式是本席推导，不是已实现或实测硬件。

**结论：当前能给出有限、可核算的组织，但没有从本条带证据推出 B1 的存储优势。** 最紧凑的可信一次性版本可以直接选 O-PAQ-D：共享一份 span/context/owner 表头，连续页以 ordinal 隐式索引，逐页保存最终 PPN，允许故障后回收成功字段。放到 endpoint、TC 旁或 IOMMU 内时，相同信息与使用语义可以采用相同压缩；若一个“local ATC”也只服务这个有限 span，它就是同类结果表。O-PIN 可省去新增 PPN 副本，但占用现有 IOTLB way，并需真实 pin、登记、失效和最后用途桥接；当前没有这些接口。现有 debug 及正常 TC 都不能免费提供 4/5 页未来结果的保持与采纳。

本卡只判断存储与部署理由，不重新裁决应用窗口、普通预热后的残余或全局方向。root 提供的正常有用页集合紧上界为 14；那是条件容量论证，**不证明当前 PLRU 实际不驱逐**。不读取其他席本轮产物，不把 HERO 源码映射成已在 Dell 原生执行的 SVA 工作负载。[C0；root 定向消息，待 root 在其产物中接受]

## 1. 固定范围与计量口径

- 输入：默认 future span 16,384 B；假设页粒度 4,096 B，起始页内偏移 `o`，则 `N=ceil((o+16384)/4096)`，`o=0` 时 4 页，否则 5 页。实际映射、对齐、是否大页、物理连续性均 unknown。所有控制使用相同投影；如果合法大页覆盖 span，N 可减至 1；若有物理连续性证据，可共用基址/差分压缩，不能只优惠候选。[C0，C1]
- B 是 host→device 源读取。本卡的紧凑布局绑定**同一 source span、context、访问类型与 privilege**；不把一次 R 翻译成功当成任意 W/RX 的授权。HERO 当前对象是 32-bit job ID，未来 span 尚无已分配的多页结果描述符。[H1:72–74；H2:58–85]
- 所有数字是**声明字段的逻辑存储位**。不含组合比较器、MUX、布线、端口电路、跨时钟桥、LUT/FF/BRAM 映射与 Fmax；这些未综合/布线，均 unknown。当前两级 IOTLB 的字段很多，不说明一个固定读取结果也必须复制两级完整 PTE。
- 区分新增位、现有共享资源占用、实现接口成本。不能把占用已有 way 计作新增 172 bit，也不能说它免费。未核实的空闲 TC/iDMA 字段不抵扣新增存储。
- `44N` 是未压缩任意PPN时的参考表示量，不是净新增硬件位或面积下界。若借用TC当前不用的MRIF等字段，须证明它们与现有live owner不冲突、normal整份response写入不会覆盖future数据，并收费保持MUX、读写口和控制；O-PAQ-D/紧凑ATC同样允许此复用。实际物理布局、常量消除与可回收容量未知。

## 2. 直接核到的当前配置与宽度

| 对象 | 当前源码字段/配置 | 可核算逻辑位 | 来源 |
|---|---|---:|---|
| 地址 | AXI IOVA 64、PA 56；最终 4 KiB PPN 44 | 64 / 56 / 44 | CFG:127–134；PKG:73–86；TOP:125–126 |
| 输入 context/access | DID 24、PID-valid 1、PID 20、ttype 6、priv 1 | 52 | PKG:79–86,528–542；TOP:135–145 |
| 解析后 context key | PSCID 20、GSCID 16、stage-enable 2 | 38 | PKG:85–86；TLB:59–68 |
| IOTLB tag | VPN 29（11+9+9）、PSCID 20、GSCID 16、stage 2、valid 1 | 68/way | TLB:59–68 |
| IOTLB data | 每 stage：PPN 44、is_1G/is_2M 2、g/u/x/w/r/v 6 | 52/stage；104/way | PKG:315–330 |
| IOTLB 常驻数组 | 16 × (68+104) + 15 PLRU | **2,767** | CFG:115；TLB:339,463–472 |
| 正常 TL 请求寄存器类型 | 64 IOVA +52 context/access +is_debug 1+bank 1 | 118 | TOP:135–145 |
| 正常 TL 结果寄存器类型 | bank/error/ignore/is_mrif 4、spaddr 56、MRIF 102、range 3、bypass 1、fault_code 12、x/w/r/g/u/sum/t2gpa 7 | **185** | TOP:148–165；PKG:354–358,488 |
| TC 当前事务状态 | 单份 request 118 +response 185 +FSM 3 | **306**，不含各 FIFO | TC:96–108,735–745 |
| debug 返回 | PPN 44、fault 1、S 1、PBMT 2 | **48**，一份覆盖式结果 | TOP:167–173；DBG:74–86 |
| 故障剩余 payload | cause 12、bad GPA 41、guest/implicit 2 | **55**；原 IOVA/context 可由保持的表头+ordinal 重建 | TOP:175–187；TL:436–445 |
| 现有缓冲配置 | AR FIFO 8、AW FIFO 8；W FIFO 32；fault FIFO 8 | FIFO payload 总位未在本卡完整展开，不计为可借用空间 | CFG:111–114；TC:121–164,222–239 |

2,767 只是 IOTLB 主 tag/data/PLRU 状态；未把 research observer 输出当成额外缓存位，也未把组合 next-state 再重复计一遍。4/5 个 IOTLB way 的现有容量占用是 **688 / 860 bit**，即 16-way 容量的 **25% / 31.25%**；这不是新增面积。全有用集合的去重和跨条带用途由 root 卡负责。

当前 FPGA 工作树 `IOMMUNonblockMode=0`，因此此生产默认路径是 serial TC；nonzero 模式另有 banked prototype，不假设其资源能免费复用。[FPGA0:169–172；FPGA1:1394–1408；TOP:358–425] 这是源码配置，未核运行 bitstream。

## 3. 当前接口究竟能做到什么

**debug 已实现纯翻译与单份结果读取，没有 future-result adopt。** DBG 把 IOVA/DID/PID/RW/priv 送进 TL；FSM 在一次返回前保持 ONGOING，response-ready 恒 1，返回覆盖 MMIO 结果。TL 给 normal 入口优先，共享翻译路径。[DBG:47–105；TL:168–213,1544–1567] debug 返回没有 context tag、epoch、owner/handle、完整权限集合或精确多级粒度。PBMT 当前明确接常量 0；S 只有“存在 superpage”的一位，不能装作完整 granule。对于本卡的逐 4 KiB 归一化结果，44-bit PPN 足够表达最终物理页；成功的指定 R 检查可作为当时成功证据，仍需要未来采纳时的有效期与 context 绑定。[TL:450–454,1330–1365,1374–1412]

**正常 TC 已有已译 PA owner，owner 只对应已实际接收的原生 Ax 事务。** 入口 AR/AW FIFO 保存原事务，TC 在 `AWAIT_RESP` 恒 ready，把返回存入 `trans_resp_data_q`；在 COMPLETE 才用该 PA 驱动物理 AR/AW，内部 `axi_comp_resp.*_ready` 后释放原队头并回 IDLE。该内部交接可进入 demux/spill，不能一律称最终 DRAM ARREADY。下游继续拥有 R/W/B 等义务。[TC:121–164,329–368,692–725,735–745] 一份 185-bit TC 结果可以复用成**该已提交 burst**的物理地址寄存器；它没有让未提交的 future stripe 在 SPM release 前持有 N 个结果的入口。让 future payload 在此长期 held VALID 还会占住 TC 队头，并已经承诺该 Ax，不能当纯准备或任意撤销。

**不能把 debug PPN 写回 DMA 地址就称为现成绕过。** 当前 iDMA backend 在两组 Ax 设置 `mmu_atst=0, mmu_valid=0`；TC 据 AT 字段选 untranslated/translated 类型。TL 的 translated 路径还检查 device context `EN_ATS`，T2GPA 配置决定返回地址按 GPA 还是 SPA 使用。[DMA:380–407；TC:630–666；TL:1109–1147] 当前 `InclATS=0` 禁用 ATS 前门，不能推成“RTL 不含任何 translated 语义”，也不能推成“endpoint 已有 ATC/ATS 许可”。ATS 的既有概念复用先前原规范核读；本席没有联网重读或 PCI-SIG 全文验证。[ATS:1–14]

这里“只改DMA地址仍走native翻译”限于当前translated context与现有AT字段。另一合法Bare DID/可信物理提交口可以是部署替代；但独立授权/绑定、切换、映射pin/撤销和混合源目的腿费用都需计入。本卡未核完整Bare通路，不将其免费存在或绝对不存在当证据。

## 4. 共享表头与按状态复用：所有控制同等允许

选一个可审查的紧凑布局作为成本参考，**不声称编码全局最小**。这是一条 span 的普通有限表 O-PAQ-D；同一结果位置可部署在 endpoint、IOMMU 旁或 TC 结果口旁。固定一个 span pool `K=N`，页 ordinal 是固定槽号；只有一个准备页在共享 TL/PTW 上推进，其余 pending 同样占预约额度。

| 字段 | 存放方式 | 位数/表达式 | 回收或压缩条件 |
|---|---|---:|---|
| 原 source span | 一份完整输入地址（VPN52+offset12），16 KiB 长度是本卡常量 | 64 | 准确源范围不能在最后 identity/fault 用途前释放；若长度可变须加 `ceil(log2(Lmax+1))`，本卡的 16 KiB 上限是15 bit |
| 请求 context/权限类型 | DID/PID-valid/PID/ttype/priv 共享表头 | 52 | 固定 R/priv、固定 endpoint 可消除相应常量；各控制同样消除 |
| 解析 key | PSCID/GSCID/stage 共享表头 | 38 | 若对所有相关命令保守清整表，且完整 context 变化均同步到该清除协议，可消除；不能只删字段而不补失效路由 |
| epoch snapshot | 表头 | `E` | E 未定；必须证明不在旧 owner/return 存活时绕回，或按 drain-before-reuse 规则运行 |
| source/result/fault sink | 表头 route | `W=ceil(log2(M))` | 当前四 DMA 的投影可取 W=2；HERO 到当前 endpoint 的实际绑定 unknown。若由 DID 唯一导出，须保留真实路由实现费用而可省重复字段 |
| job generation/handle | 表头或受管 slot 的 generation | `G` | `hero_dma_job_t.id` 的32位可在旧 wait 完成后改作 handle；表体仍需计费，且不得与尚活旧 ID 同时复用。无 ABA 的停等合同可省 generation，常规并发值 unknown |
| stripe 控制 | 2-bit FREE/ACTIVE/FAULT/STALE，加1-bit application release | 3 | release 是应用依赖，不等于 Ax 握手或 consumer 完成 |
| 页状态 | 每槽2-bit，区分 free/reserved-or-pending/ready/terminal-use | `2N` | 故障可上移到 stripe 状态；不因压缩 bitmap 而少算 K 中的 pending/fault owner |
| 当前页 ordinal | 单个串行准备/故障 ordinal 寄存器 | `u=ceil(log2(N))` | 完成后可重用作 fault ordinal；不再增加一份故障 page tag |
| 成功结果 | 4 KiB 归一化最终 PPN，隐式连续输入 VPN | `44N` | 只绑定表头中的同一 access；不必逐页重复 VPN/context。物理页不连续，不能默认差分 |
| 成功许可 | READY 只代表同一 context/access 的检查已成功；epoch 管有效期 | 包含在状态语义 | 若要允许任意访问复用，另收费权限/属性；不能从 PPN 或非fault推断W许可 |
| 故障 | 与成功 bank 互斥，详见下文 | `F` 或全bank union | 不能把已发 PTE、故障排队或 sink 背压丢掉 |
| last-use | page-state 记录是否仍有用途；真实最后用途证明由上游原生 owner 提供 | 额外 `X_last` | 尚无 prepare/adopt ABI，故所需 watermark/refcount/parent 剩余量状态未定，不能写0冒充已有桥 |

表头已列字段合计 `H=64+52+38+3+E+W+G=157+E+W+G`。示例不依赖未核的39-bit地址窄化；若先完成Sv39 canonical校验并可重构高位，VPN52可缩至27，各控制同享25位节省。当前IOTLB内部29-bit key不代表任意64-bit入口地址可直接丢掉高位。

**故障采用两个明确合同，而不是夸大固定快照。**

1. **Advisory prepare fault**：纯准备失败只使该 stripe 不能采纳；实际合法 native submit 时重译/报错，收费相应重走。ready/fault 状态与归属仍占 K。若使用当前 debug，既有 TL fault 路由/队列可能先发生，不能假定纯翻译无 fault sink 费用。[TL:1510–1541] 这不是“原生故障语义已由新表完整替代”。
2. **保留首故障**：一个条带遇首故障即终止该次准备，取消尚未 first-offer 的页，排空已承诺页，向有归属的 sink 保存/交接 `F=55` 剩余 fault payload；表头和 ordinal 重建输入身份。之前的成功结果不再用于采纳，其 PPN bank 可合法转存首故障。故 `D_one=max(44N,55)`；N=4/5时仍为44N。若要求逐页独立失败、继续准备并同时保留全部故障，不能用这项优化，应取每槽 `max(44,55)` 或单独 K×55 fault pool。O-PIN 与 local ATC 获准使用相同首故障合同、相同回收和同一补译策略。

上述55位不自动证明精确 future-fault 协议已实现：fault queue 的报送时机、重复报错、动态映射重试以及 revoke 后的终态要随实现冻结。本卡不按一个“fault=1”就豁免这些义务。

## 5. 可核算成本对照

下表是单条带固定 pool 的**参考编码**；均另加必要但接口未冻结的 `X_last + X_epoch + X_route + X_impl`。其中 `X_epoch` 至少包括管理端当前epoch的 E 位及 wrap/drain 协议状态；`X_route` 是路由/接收队列；`X_impl` 是实际allocator/接口寄存器等。固定单pool无需free-list；若允许多个stripe/动态槽复用，要加owner指针/分配器，不能把它们隐藏。E/W/G已在H中收费，X不重复收同一字段。所有这些量是必要项的符号接口，不是已测面积。

| 组织 | 新增逻辑存储公式 | N=4 | N=5 | 现有资源占用与条件 |
|---|---|---:|---:|---|
| **O-PAQ-D / B1 最佳已给出普通组织** | `H+u+2N+max(44N,55)` | `343+E+W+G` | `390+E+W+G` | 首故障整条带终止；结果 bank 真正在某一位置分配；再加X项 |
| 独立逐页故障参考版 | `H+u+N*(2+max(44,55))` | `387+E+W+G` | `445+E+W+G` | 不能拿这一较宽合同单独惩罚ordinary ATC/PIN |
| **O-PIN，存way ordinal** | `H+u+2N+16+max(4N,55)` | `238+E+W+G` | `241+E+W+G` | 16-bit该stripe pin mask；ready页存4-bit way；IOTLB另占688/860现有位，侵占4/5 ways；首故障后先合法解除本owner，再复用map bank |
| O-PIN，advisory故障简化 | `H+u+2N+16+4N` | `199+E+W+G` | `206+E+W+G` | 错误实际消费时重走；若以复用IOTLB lookup来解除而不留way map，可再省4N位，增加该口仲裁/服务，不免费多端口 |
| endpoint 固定span local ATC | 与 O-PAQ-D 相同字段、相同故障/last-use合同时，采用相同式子 | 同上 | 同上 | 新的结果返回/失效/translated发起或可信adopt接法；并非当前已有 |

数字相加可复核，例如 `343=157+2+8+176`，`390=157+3+10+220`，`238=157+2+8+16+55`，`241=157+3+10+16+55`。它们不构成“B1总资源只有343bit”或“PIN一定面积更小”的结论；X项、way机会成本、关键口与时序尚未实现。保守全局flush可省解析key38位但付出额外重新准备，双方相同；若仅advisory失败且不保留55位，one-shot N=4/5主成功bank已足够，pin可能更小。

一般的可复用 ATC 需要逐 entry 做 tag/context/权限检查：一个未共享的示例是 `VPN52 + (DID24+PV1+PID20+PSCID20+GSCID16+stage2)=135` tag/context 位，2-bit状态，以及 `max(PPN44+perm7+range3+PBMT2, fault55)=56` 数据位，即 **`193+E+owner/ref` bit/entry**，再加miss/返回/失效队列及命中检查器。这里perm7取当前normal response中的x/w/r/g/u/sum/t2gpa；是示例信息格式，不是正式ATS协议包宽或已验证的完整权限checker。它允许复用任意访问，语义强于固定R一次性表，不能用193×N和紧凑B1直接相减宣称收益：ATC可共享context、归一化4KiB，或采用完全相同固定span布局。反过来B1若要相同通用权限/重用，也要收费。

## 6. 端口与首次承诺，不能由位数替代

| 位置/组织 | 实际或最低付费访问 | 可串行化的办法及代价 | 当前实现状态 |
|---|---|---|---|
| 当前debug | 一个未完成debug；normal优先的共享请求入口；一份48-bit覆盖返回 | CPU逐页写MMIO、轮询/读回；若保留结果另放CPU/设备表并传handle | 已实现入口；无多页私有保持/adopt |
| 当前IOTLB | 1个逻辑lookup输入、1个update输入，另有全阵列失效逻辑；PLRU控制 | pin claim/release与normal查找可仲裁，claim必须与fill/way选择原子绑定 | lookup/fill/inval已实现；pin lease未实现，policy_mask只是实验replacement分配接口 |
| O-PAQ-D | 准备返回写、native采纳读、release/free状态改写；epoch/inval及fault sink | 最小1RW bank可每周期只选一类动作，加入仲裁和等待；TL ready可控则保持owner，若直接接ready恒1的debug需至少可靠捕获/接收寄存器，不能吞返回 | 全部为拟议；不能直接借用48-bit debug寄存器视为N-entry bank |
| O-PIN | normal已有lookup；prepare fill/way pin原子化；最后用途unpin；正式inval优先清有效性 | 保留4-bit way map减少解除时lookup；或省map并复用lookup口，付出服务占用。无空闲way时登记拒绝/等待，不接受无归属walk | 拟议pin/ref、release接法；不把PolicyEnable等同已实现pin |
| endpoint ATC | local hit lookup、prepare/miss completion写、translated Ax注入、invalidation接收与ack | 同一bank可串行；多DMA或多lane同时消费必须显式仲裁/复制/增加端口 | 当前endpoint不具备该ATC/发起接口 |
| TC结果旁PA表 | 与O-PAQ-D相同；在当前normal response接收点写入，对后续请求匹配并取回 | 可串行共享normal结果口，但future纯翻译请求仍需新增合法入口和owner；不能先伪造payload Ax | 拟议位置替代，不预设必须修改IOMMU核心 |

一条prepare串行也不是多页返回容量免费：登记时K=N，包括队列中尚未发的pending、在译pending、ready及fault；先登记后慢慢寻找返回空间会破坏此合同。单端口允许用吞吐交换面积，但具体延迟/可否赶上release均 unknown。

## 7. owner、最后用途与可回收字段

| 阶段 | 归属/不可丢义务 | 可合法回收 |
|---|---|---|
| 登记接受前 | 应用仍持有future计算出的地址；无硬件结果承诺 | 无已接受walk，可拒绝；32-bit旧job ID只在旧wait归还后复用 |
| 登记接受后 | 一份source/context/generation、N个额度；pending明确有owner | 连续VPN无需逐槽复制；一个串行cursor足够索引当前页；不额外假设N个walker |
| PTE first VALID/握手后 | PTW当前地址/返回owner、错误路径必须保留；发布后不能因release/epoch改变换请求 | 下一尚未first-offer的页可取消；当前PTE需退休。[PTW:274–280,384–405,618–644] |
| ready等待SPM release | PPN、权限请求绑定、epoch有效性、owner保持；只允许按未来native真正范围采纳 | 原walk中间字段可回收；完整结果不能因“还没payload”而消失 |
| 采纳前 | 验证span/page、DID/PID/priv/access、generation/epoch、真实接收能力，确认release；判错/重译发生在physical first VALID之前 | 采纳接口/匹配逻辑拟议；无论表放哪里都收费 |
| physical Ax在TC/demux中被接受 | 该child的PA/权限已交给具体下游owner；其R/W/B obligation继续 | 若这是该页**最后一次翻译用途**，表项可立即释放/取消pin，无需等payload R/B或矩阵consumer完成；同页还有未发burst不得提前释放 |
| fault/stale/cancel | 已承诺PTE和fault sink仍要排空；共享header在最后身份/错误用途前存活 | 首故障整条带放弃后可复用旧PPN bank；fault被sink接收、所有旧return/引用清零后重用handle |
| 数据/consumer/buffer | 原DMA跟踪ID、R/W/B、cache handoff、consumer和SPM占用是分别的owner | 释放翻译表/解除pin不等于归还SPM或完成矩阵 |

真正最后一次翻译用途通常早于最终R/B；也可能晚于首个同页burst。当前TC只知道其队头Ax，不知道未来parent还有哪些未发字节。若采用原生job source进度、page-last标注或计数回调形成这条证明，需要新的可靠桥接；`X_last`只在接口/实现确定后才能数值化。把结果移到已知parent的endpoint，可能减少跨模块信息搬运，**这是可以真实区分部署成本的问题**；当前没有该部署的代码/端口/时间证据，不能把这种可能写成已有优势。

相邻条带同key共享边界页时，可共享一份PPN/IOTLB data，但必须保留每个合法用途的lease/ref或可证明的顺序释放。多个span可用每header位图或per-entry refcount表达；前者多收各owner位图，后者多收 `ceil(log2(Rmax+1))` 和更新仲裁。单个owner先完成不允许清除其他owner的pin。没有理由强迫ordinary一直pin到整DMA/consumer结束。

## 8. 正常物理缓存与endpoint替代位置

**先译地址、以后发payload的PA结果表**可以靠近正常TC结果接收点或endpoint。若它仍要从未来IOVA识别结果、检查context/epoch并在release后发原生事务，它的信息与O-PAQ-D相同，不能靠“physical cache”名称省掉source tag、permission或owner。把表放在IOMMU之外并不消除撤销/失效握手；相反可能增加路由和跨边界ack，但也可能让parent/last-use信息更便宜。这两类费用都要实际冻结接口才能比较。

**真正按物理地址存payload的数据cache**通常在normal translation后查找；它可以减少数据读取服务，仍需要先得到正确PA，不能自动保持future IOVA翻译。若为绕过忙SPM而提前读完整16KiB条带至另一个buffer，在允许host数据稳定与合法读取的合同下，payload容量至少 **131,072 bit**，还需cache tags、valid/dirty/coherence、返回/路由与搬入SPM成本；这是一个不同面积/行为点。没有要求它一次存完整条带，分块版本可更小，但只能按其实际保持前缀计效用。SPM owner仍不能被提前覆写。

**已存在下游physical Ax寄存器**可以承担已交接child的地址，现有费用作为双方共同基线；它不能在没有已提交事务/真实槽的情况下兼作未来4/5页免费cache。若提前正常提交到一个新staging endpoint，该endpoint的存储/回压/完成/最后归还就是付费新owner，不能把新地址绕行写成零改动。

## 9. 本卡裁决、缺口与交接

1. **可保留的具体问题**：同一个未来source span，用IOMMU旁结果表、endpoint固定span ATC、O-PIN或TC旁PA表时，最后用途信息在哪里已有、需跨哪条边界传递、哪个口与demand争用、如何接正式inval，可能形成部署成本差异。需要一份真实接法与字段/端口布局，不能先假定IOMMU是唯一落点。
2. **本版不成立的成本理由**：32-bit现有job ID能容纳整条带结果；TC已有PA故免费有N个future槽；通用ATC逐项复制所有context而B1共享所以必省；K只数ready；pin必须留到consumer；同key共享页可省掉owner；1RW天然等于无时延。这些都被本卡明确排除。
3. **普通构造的结论范围**：O-PAQ-D在同一付费登记/采纳接口上可表达这套有限动作，说明重新命名队列不提供独立存储/策略增量。它不证明该接口架构已发表、不否定有证据的片上集成贡献，也不表示整个B思想族关闭。
4. **仍是unknown**：实际HERO映射/地址空间，普通debug/PTE-cache后是否有真实残余，SPAN晚到是否阻挡完整consumer，当前PLRU是否发生相关eviction，adopt/last-use/inval桥的X费用，部署端口吞吐、LUT/BRAM/时序/功耗/净性能。没有用缺失接口来推导novelty，也没有因为容量≤16就推导无需任何留存。

本席只写本目录三份文件；0新模型/旧实验/构建/硬件/联网/DeepSeek/Claude；不修改生产RTL/软件ABI或全局状态文档。实际操作为定向读源、字段与算术核对、生成本卡及hash manifest。root须回到关键原文接受本卡结论，后续是否继续由root在完整单条带卡中裁决。

来源ID、精确文件、hash及读取范围见 [source-manifest.json](source-manifest.json)；结构化字段与可重算公式见 [cost-card.json](cost-card.json)。
