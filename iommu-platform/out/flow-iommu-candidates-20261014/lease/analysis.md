# B/C/G：B translation multicast/push、C translation lease、G TFID 审查

日期：2026-10-14。执行者：Codex（Sol 席，继承既定 ultra）。范围：source-only 关键机制审查；未调用 DeepSeek/Claude，未建模型、运行实验、构建或修改生产源码。本文只交 root 接受，不写共享状态。

## 1. 可接受结论与优先级

**可以保留 flow lease 作为任务生命周期接口，但连续 base+offset、分段映射、owner set、generation 或 multicast 这些字段单独不构成独立机制贡献。** 这些动作分别必须面对 Direct Segments/CoLT、分段翻译、权限上下文与 ATS 的具体已有能力。只有把任务完成信息变成已有控制没有的、正确且省成本的撤销/保留动作，并在强对照下成立，才有独立候选。

| 项目 | 当前判定 | 优先级与适配成本 |
|---|---|---|
| C 连续 lease | base/limit/offset 翻译能力已有准确前作覆盖。不能把底层仍为 4 KiB PTE 当成排除 CoLT 的条件 | P0 先做纸面强对照：同预算 ordinary range/coalescing；当前实验已强制连续物理分配，直接相关。暂无性能结论 |
| C segmented lease | K 个连续区段需要 K 个区间映射；其翻译能力可由 segment/range TLB 表达。lease 的潜在差异在生命周期，不能靠改名成立 | P1 有界重设计：允许完成后局部撤销、保留未完成子段，比较 whole-range flush 与独立 segment entries；必须计所有元数据 |
| G TFID | 可作工作流关联、完成匹配、预热路由标签；不能直接作授权 key | P0 作为辅助接口候选。若只传回 opaque tag，规范已有接口；若跨设备传播，需要可信标签来源及生命周期管理 |
| B multicast-to-device-uTLB | ATS 的 request/completion 和批量连续 translation response 是强对照。root 另核到主动 write-push device-ATC 的专利原文，因此简单 push 本身也有直接前史。通用 unsolicited 跨设备翻译安装不在已核 RISC-V ATS 接口列表里 | P2；当前配置 InclATS=0，实际 iDMA 没有已证明 device-uTLB/安装通道。须改端点、缓存、分发、确认、失效协议，成本最高 |

这里的优先级是下一次 source-only 判别顺序，不是实现授权或论文贡献接受。

## 2. 关键原文与精确定位

本地论文目录：`iommu-platform/out/shared-translation-literature-20261006/papers/`。下列页码是 PDF 文件从 1 起的页序；本地行号按 `nl -ba` 的换行口径，未把 form-feed 当新文本行。

| 原作 / 原文入口 | 本轮直接核查位置 | 对本候选的实际约束 |
|---|---|---|
| [Direct Segments, ISCA 2013](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) | `direct_segments2013.txt:128–148`（PDF p2），`:309–341`（p5），§3.1 | 用 base/limit/offset 把连续 VA 区间直接转成连续 PA；边界可按 4 KiB，对内部必须具有相同权限。普通任意长度 range 能覆盖“没有大页 PTE 但物理连续”的能力 |
| [CoLT, MICRO 2012](https://users.ece.cmu.edu/~omutlu/pub/MICRO2012-proceedings.pdf) | `colt2012.txt:270–280`（PDF p4），`:287–313`（p5），§4.1.4–4.2 | 连续 base-page 翻译合并，shared attributes，保留基础 PTE；SA 从一条 PTE cache line 找最多 8 项，FA 长度字段可覆盖 32 页并扫描已有项。§4.1.5 whole-coalesced invalidation，局部 uncoalescing 明列 future work |
| [SLL TLB, HPCA 2011](https://mrmgroup.cs.princeton.edu/papers/abhattac-hpca2011.pdf) | `slltlb2011.txt:168–189`（PDF p3），§3 | SLL 是 Shared Last-Level，不是 segment TLB。一次 miss 填 shared 与请求方 L1，后续 sharer L1 miss 从 shared 取；mostly-inclusive 时失效两层都必须查。不能把共享 L2/私有 L1 这一组织本身当新意 |
| [rIOMMU, ASPLOS 2015](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf) | `riommu2015.txt:382–420`（PDF p6），`:444–472`（p7），`:523–551`（p8–9），§4 | 有 buffer 尺寸与方向保护、bdf+ring 身份、current/next translation；利用 map/unmap 顺序使新 entry 替换隐式撤销旧 entry，显式失效留到 burst 末。不能把“任务/描述符生命周期减少 invalidation”泛称新能力；有效 DMA 访问不必 FIFO，顺序前提主要在 map/unmap，不能误称只允许顺序 DMA |
| [Accelerator-Centric Translation, HPCA 2017](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf) | `accelerator_translation2017.txt:402–413`（PDF p6），`:481–523`（p7），`:599–616`（p8），§IV-B/C/D | 私有 TLB、共享 L2、跨加速器相同 page 翻译复用、hit-under-miss、两层 shootdown 已有。不是自动安装到所有未来 consumer；但足以否定“device-uTLB + shared translation”宽泛新意 |

连续性与共享是两条轴：CoLT/Direct Segments解决一个上下文里的覆盖；SLL/HPCA17解决多个请求方的共享。把两者并列装进 lease 不自动产生新的可执行动作。

**规范核查（官方主源）**：

- [RISC-V IOMMU v20260222：Introduction](https://docs.riscv.org/reference/iommu/v20260222/iommu_intro.html)，Placement/data flow；网页行169–180、188–201、214：设备/进程身份、请求意图是翻译输入；opaque tag 可往返关联；ATS 接口列 request/completion、invalidation/completion、PRI。身份真实性由 IO bridge/root port 保证。
- [RISC-V IOMMU v20260222：Data Structures](https://docs.riscv.org/reference/iommu/v20260222/iommu_data_structures.html)，`ta`、Process context、Process to translate IOVA；网页行174、382–415：PSCID/GSCID描述地址空间；相同 PSCID 而页表不相同会导致选择不可预测；同 TFID 不证明两个上下文映射相等。
- [RISC-V IOMMU v20260222：In-memory queue interface](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)，Page-Table invalidation、Command-queue Fence、ATS commands；网页行101、107–113、135–138：IOTINVAL、IOFENCE、device-ATC acknowledgement 各有义务；PR/PW 对已 processed 请求排序，未到 IOMMU 请求另需互连 fence。不能用 completion tag 或 expiry 省略它们。
- [Intel VT-d specification 官方 PDF](https://cdrdv2-public.intel.com/831418/vt-directed-io-spec.pdf)，§4.1.1–4.1.2，4-1附近：ATS 请求 Length 可要求连续等粒度多笔 translation，8 bytes/translation。官方搜索返回了此节原文，但 web open 因 PDF 超过抓取上限失败；因此这里据已返回的原始段落确认批量能力，不声称逐页核完最新版 ATS Base Specification。

本轮未取得 PCI-SIG Base Specification 全文。不得把“已核接口不包含任意 unsolicited multicast install”扩大成“所有 PCIe/CXL 版本都禁止所有形式的 proactive delivery”。任何厂商扩展、设备特有预取/安装接口仍是未知项。

## 3. G：TFID 可以是什么，不能是什么

建议接口语义：`TFID = trusted flow association`，对应 owner 集、任务/区段、已认证的 DID/PASID 关系和完成账本。它可把同一工作流的多个描述符关联起来、选择预热目标、汇总 completion；它既不证明映射相等，也不授予读写权限。

**放行条件必须仍包含**：请求方真实身份 → 当前 DC/PC/页表根及 stage → range 覆盖 → read/write/privilege/其他属性 → 映射仍有效 → 当前 owner 已被允许参与。可以共享 raw mapping，但每个不同保护上下文的权限与映射等价性须独立证实；TFID 不应替代这些检查。generation 只有与发布/撤销规则和不可绕过的检查位置结合才有效。

实际生产 RTL：`cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:45–68,106–132`，key 含 VPN、PSCID、GSCID、stage、valid；没有 DID。global PTE 时按既定规则绕过 PSCID 比较。由此，同一真正共享 domain 的不同 DID 本来就可复用翻译；“按 TFID 去除重复 DID”不是必然新增能力。

实际 Linux：`linux/drivers/iommu/riscv/iommu.c:1574–1595`，普通 S-stage attach 将 `domain->id` 写 PSCID、同 domain 根写 DC.fsc。`:1650–1655` 是 SVA PASID 到 PC/PSCID/page-root 路径，不能与普通 DMA attach 混用。

具体 shared/split 影响：shared domain 具有同 authority 与根，允许现 IOTLB 普通共享；split domain 具有独立 authority，即使 PA 有重叠，也不能直接共用 permission-bearing entry。当前源码证明两种配置的语义，**未读取本次启动 DT/日志，所以未判四个实际设备当前共享还是拆分**。TFID 不得制造假共享。

## 4. C：连续与分段 lease 的强普通对照

### 连续 4 KiB

普通超页仅在对齐、大小、属性和实际页表条件满足时适用，不应假定任意连续范围都能直接用一个超页。反过来，不能因此排除 ordinary range/CoLT：它们保留 4 KiB PTE，缓存连续映射的 base+offset，覆盖不必满足超页大小。预先注册 lease 如果省掉填充前读取/验证所有基础映射，省下的其实是 authority contract 改变；须计 pinning、注册、更新、拒绝/回退，并给同样注册能力的 ordinary range 对照。

生产 Linux 的 `linux/drivers/misc/idma.c:270–279,365–375,389–395`：实验取得已有 device domain，`DMA_ATTR_FORCE_CONTIGUOUS` 分配连续源/目的，按 PAGE_SIZE 对 `phys + off` 逐页建映射，读写权限相同。这是最应先面对 range/coalescing 的现成条件。不能声称实际 job 已有非连续 segments，不能据此推出真实应用也应全部连续。

### segmented lease

若 flow 映射为 K 个 `(IOVA_start, PA_start, len, attrs)` 连续子段，那么检索与存储至少需维护这些段；K 接近页数时压缩收益消失。普通 segment/range TLB 用 K entries 可给相同 translation coverage；同预算需要包含所有 ranges、有效位、owner/member 关联、各 owner 权限、generation、invalidations、完成/未完成账本、安装/退回通道与 buffer。

若只靠 TFID 或 descriptor 中的 segment index 快速选段，要确保来源可信、index 和真实访问 range 一致。降低查找端口/比较器是潜在实现收益，但必须比较同样可信索引的 ordinary segment table，而不能强制普通方案 CAM 全查。

## 5. 过期与撤销的最小正确性义务

generation/expiration 只能防止 **未来读取该 lease** 使用旧状态。已经翻译后送下游的 PA、已接收 AW/AR、endpoint buffer 中尚未到 IOMMU 的请求不会自动带回该检查点。任何过期方案至少需闭合四项：

1. 可信位置停止该 owner 的未来发起/接受，发布权限或映射撤销并阻止 stale fill；
2. 失效所有可能使用旧翻译的 shared IOATC 和私有 device ATC，获得所需协议确认，处理 timeout/reset；
3. 确认旧请求读写已到规定全局排序点；对未到 IOMMU 的在途请求使用互连/端点排空机制；
4. 然后才允许 physical memory reuse 或权限换给新 owner。逻辑映射撤销与物理回收可以分离，但页仍必须被保留到正确完成点。

所谓“任务完成”须精确到支持此义务的终点：发完 descriptor、source AR/AW 接收、最后 raw R/W beat、destination B、device completion、全局可见并不是同一个事件。禁止把读完源 buffer 当成目的写入已完成，禁止把 generation bump 当成排空已接收 DMA。

Linux 已有具体失效次序的源码入口：`linux/drivers/iommu/riscv/iommu.c:583–611`，先 page-table cache 失效与 fence，再向每 endpoint 发 ATS.INVAL，再 fence；该段是 SVA notifier，不证明当前无 ATS 的 iDMA 已有新 lease completion/reclaim 合同。

## 6. B：multicast 安装的实际边界

普通强对照必须包含：每 consumer 主动 ATS prefetch，单请求多连续 translation 返回、共享 L2 请求归并、私有 uTLB miss 经共享 L2、通知各 consumer 预热，以及已核 author-delivery 类方案（由 root 做 HDPAT 等集中比较）。不能只比每 consumer 独立 PTW。

若一种自定义 SoC 协议把已验证翻译主动安装到 N 个 device uTLB，确实可能消除第二至第 N 个 consumer 的冷启动请求/等待；这是 **与标准 pull 路径的动作差异**，不等于本轮已证明独立新颖性。root 亲核到 [US20250335370A1](https://patents.google.com/patent/US20250335370A1/en) claims21/25/26、Fig3D/3E 的主动 write-push device ATC（2025-10-30公开，parent于2024公开），及 [WO2022133841A1](https://patents.google.com/patent/WO2022133841A1/en) claims1/6/7 的 job descriptor pretranslation；本席没有重复核全文，不把“push”或“descriptor提前翻译”作为剩余创新。owner graph上的选择分发/生命周期协议仍需另判。须携带每 consumer 的匹配身份、range、权限、generation；端点可拒绝、已失效时拒绝迟到 install；明确 backpressure、重试、ack、所有者退出和失效重入。N 份缓存项、网络扇出和 invalidation 都计成本。

当前配置直接证据：`cva6/corev_apu/riscv-iommu/packages/rv_iommu_cfg_pkg.sv:71,129` 有 InclATS，默认 0。`rv_iommu_top.sv:100–106,233–236` 有 ATS 接口与 invalidation 内部信号，但有接口定义不等于当前 production instantiated endpoint 拥有 uTLB 或安装协议。没有核到本次生产 top 覆盖该默认，也未证明 iDMA 私有 ATC。本项进入性能模型前必须拿到它们。

决定性正例：已注册的共同映射，N 个 consumer 相继首次访问，producer 已拥有翻译且目标消费确定；主动安装总费用小于各 consumer 合法 pull 的等待。相邻反例：预测 consumer 不用该页、endpoint 启动已经提前 ATS prefetch、或在 install 到达前映射失效；前者污染/扇出可超收益，后者普通能重现，第三必须安全拒绝旧项。

## 7. 一条值得保留的条件性能力：完成驱动的局部拆分/保留

建议 C 重设计成明确动作，而非继续扩大 lease 字段：**任务完成后，仅撤销完成 owner/子段，保持同一压缩映射内其他仍有效子段，无需使其重新 walk；映射创建和拆分成本有界。** TFID 用于把可信 completion 关联到具体 owner/子段；数据授权仍由正式上下文给出。

这是可用条件性差异：CoLT 已核版本在一项 invalidation 时清整个 coalesced entry，局部 uncoalescing是未实现展望；rIOMMU 利用 ring map/unmap序列，并不是任意 flow 多 owner 的局部生命周期合同。**如果**新的实现能在 owner/子段异步退出时完成安全局部撤销，而 ordinary 独立 segment entries 或已有部分 uncoalescing 在相同完整预算下无法获得同样覆盖与生命周期费用，那么该新增动作可成为有界机制候选。**如果**独立 segment/range entries 足以无额外 walk 保留所有活子段，或 rIOMMU 当前/next 工作集匹配该 flow，那么不能把这一场景收益算作本机制独有。

强对照必须包括三者：CoLT whole-entry flush 原版、ordinary K 段独立缓存/独立失效、等费用 partial-uncoalescing range 控制（后一项是简单机制对照，不宣称准确已有论文）。不能只击败第一项就称创新。跨保护域权限共享与 ShareMMU 的近邻审查交 root；本席不把该主题重复包成新的 owner table。

### root 必须亲核的 ShareMMU 原文入口

[ShareMMU MICRO2026 camera-ready](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf)，本地 `papers/sharemmu2026.txt`：

- PDF p6 §V-B，行361–409：PPN-tagged Secure Vector 中 per-accelerator权限、VPN aliases、Access Mask；targeted back-invalidation，拒不配合端点后续访问由中心Permission Tracker拦截。它已经覆盖“owner集合/权限表/定向失效”的宽泛能力。
- PDF p7 §V-C1，行413–430附近：沿已有shootdown路径执行动态权限变化，不应宣称前作没有permission lifecycle。
- PDF p8 §V-C7，行500–521，接 PDF p9 Fig11 与行552–567：先锁shared TLB并通知端点；dirty writeback与in-flight drain，flush ack后才更新/回收PT状态。排空论证明确假定stalled miss不占端点执行资源，active hits有界完成。新flow若包含已接受源读→目的翻译依赖，不能自动套此假定；也不能仅凭抽象依赖声称实际当前生产存在死锁。

以上为本席直接原文定位；root接受前应自己读这些关键行。ShareMMU不把generation替代这些步骤。

决定性正例：多 DMA 异步处理同 buffer 的不同 segments，一些可回收子段先完成，其他子段长期仍活；连续缓存整体失效会反复损失仍活 coverage，独立 entries 在同容量下装不下全部当前活段。需证明真实工作流会发生、completion足够精确、split数据结构足够小，并计实时拆分端口与被滞留页。

相邻反例：全部 owner 同时完成、映射长期 pin 不需中途撤销、短 flow 中 registration 大于节省、最大 fragmentation 已退化为逐页 entries、owner complete 早于已接收 DMA 全局完成。最后一种是安全失败，不能用平均收益盖过。

## 8. 未知项与下一步的退出条件

- 无实际这轮 flow 原始 trace、段数分布、首次使用/退出先后或 completion端点，所以收益与 netgain均未建立。
- 无 actual shared/split domain启动证据；Linux语义与现 key 已核，不能替代本次配置。
- 无 endpoint uTLB、可信 TFID 来源、multicast installation/ack 当前接口证明；InclATS生产 override 待核。
- 本轮没有证明局部拆分的数据结构有优于所有 simple segment controls 的成本；没有论文层面的 exhaustive novelty 判定。
- ATS 批量能力由 Intel官方原始段落核到；PCI-SIG全文/设备特有主动预热能力未穷尽。
- 只有在给出一个具体真实 flow、完整 authority/完成点、strong-control成本和决定性场景后，才值得建立小模型。若连续 ordinary range 或独立 segment控制已经完整重现，结束该性能分支，保留接口/安全合同，不否定全部工作流方向。

