# docs/mechanism-design-handoff-20261009-round02.md

**stage**: 89  
**date**: 2026-10-09  
**范围**: 设计、手算、机制卡与下游交接；不包含实现、复现、RTL、Linux、构建、上板。  
**作者/责任**: 设计作者与验收 Codex root；两个必要独立关键设计席（translation-action、fragment-contract）；接口证据与中文稿起草 DeepSeek；最终审核保存 Codex。  

## 0. 一句话推荐
推荐 **L：在首次 backend VALID 前，用当前翻译局部性在可互换目的缓冲中选择目的地**。它要改变的是“实际需要翻译的目的端点/冷 walk 工作”，而不是只把请求排队、限流或延后。备选 **R：COPY 关系压缩 IOTLB** 主要节省表示成本，字段子集 876 vs 1312 bit，但 generic 任意关系字典可精确复现，当前不能据此推出独立压缩算法。**T** 在本平台因原生 PTE 续步可下一拍发起，低优先级。  

必须同时说明：**COPY_TO_POOL 接口本身不是新贡献**。io_uring provided buffers 已经支持从注册组选择输出缓冲并返回 ID；DAMN 已覆盖持久 DMA 池、chunk/bump 分配和 magazine；IOTLB wall 工作已讨论页局部性、hugepage-aware 分配和 recycled-buffer shuffling。L 的可证伪假设只是：在软件在付费的晚决策点看不到驻留变化时，有界硬件选择点能否以更低总成本避免真实翻译工作，并胜过最佳软件/准备/容量对照。  

## 1. 权威与纠正
以 /home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round02/root-result-acceptance.json 为权威；/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round02/root-source-review.json 纠正早期 worker。关键纠正：目的必须在 **first VALID** 前稳定，不是 first READY/handshake；next_id.re 是读事件，不是写 NEXT_ID；frontend burst_req_t 是父 COPY 描述符，完成不等于只有一个 AXI burst；实际实例 TouchEnable(NonblockEnable)，不能拿 IOTLB 模块默认参数证明生产总在 hint 时改 PLRU；IOTLB content 含 permission 字段，但 tag hit 本身不是 access grant；wrapper 在 hit 上消费 content_1S.w/r/x/u 和 stage2 字段。  

本地源码证据（绝对路径）：  
- /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:240-269,323-340,472-570：NEXT_ID 读触发、受控 HOLD、COUNT 为首次公开 backend VALID 并冻结启动合同下的 ownership、WAIT 等实际 COPY 完成；当前没有池分配/所选 slot 返回。  
- /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv:35-138,208-336,384-424：单查询口、context/stage/page-size 匹配；TouchEnable=true 且 touch_i=false 才能 no-touch；无效/替换独立。  
- /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:658-686,1330-1388：实际实例参数与 normal wrapper permission check。  
- /home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:384-445,507-554：PROC_PTE 可设 state_n=MEM_ACCESS 并更新 next pointer，没有天然长固定 gap。  

## 2. L 卡摘要
**动作**：COPY_TO_POOL(src_iova,pool_id,length,tag)。至多 4 个 eligible free 候选/选择；一个 parent/原生 frontend；不宣称四个 pending translation 或四个 walker。  
**工作负载前提**：应用接受已注册输出池中任意正确拥有的 slot，完成返回实际 slot 和 length；源/目的不重叠；消费者在实际 COPY 完成和 cache 同步后释放该 slot。排除固定指针 memcpy、MMIO、应用在 selection 前观察目的、消费者必须固定地址且不记额外 COPY。  
**伪代码/合同**：  
1. 首次 backend VALID 前锁存不可变 source/length/options/tag，验证受信 pool、长度、非重叠。  
2. 预留 completion capacity，然后 round-robin 扫描至多 q=4 个 FREE slot。查询只是 hint：用同 PSCID/GSCID/stage 匹配，抑制 replacement touch；每个串行查询都支付真实 lookup 机会；从未选中的 hint 绝不触发 page walk。  
3. 优先 resident eligible slot，否则普通 round-robin。原子 FREE->RESERVED 处理并发分配。重验可用性；若变化，有界重试一次，否则最老普通候选；不做无界 hunt。  
4. 首次 backend VALID 前冻结所选目的和所有 parent 字段；VALID&&!READY 或 backend/AXI 发布后不得改变。  
5. READ/WRITE 都走未改动的正常 IOMMU 权限检查。Advisory 可 stale；stale 只导致 miss/正常 fault，不绕过权限。正确性不要求 pinning，invalidation 总是赢。  
6. 只有成功实际 READ/WRITE/B 完成才 RESERVED->DELIVERED 并发布 tag,slot,length,status。输出 ownership 保持到消费者以匹配 generation 释放。完整 completion storage 阻止新 launch，但不得阻塞已发出 B。  
7. 访问错误隔离目的，并在合法排空后显式报错；原生 NO_ERROR_HANDLING 不提供该合同，错误恢复是下游设计义务。  
8. 池耗尽公平等待且不产生 payload 请求；probe budget 与普通需求对账；年龄 fallback 用 cold free slot 而不是饿死；计数 wrap 需 drain 后复用。  

**新接口**：池注册/释放；advisory IOTLB probe arbitration 或显式计价的 shadow tags；chosen-destination completion register/queue；锁存 parent 和所选 slot 状态。已有研究快照不是免费功能接口，缺完整授权；宽 tag copy/端口必须计费。  

## 3. L 定量正反例（手算，不是实测 cycles）
归一化单服务服务器构造：两个任务 V 后 A 同 H 后一起释放；均为 64B COPY；V 有两个可互换 free 输出槽，恰一个匹配驻留；互斥驻留状态等权；V/A source hit，A dest miss。成本 D=24（2 normal translation lookup/check + AR1 + R8 + AW1 + W8 + B1 + frontend3；64bit payload，8 R beats、8 W beats），M=20（每冷端点额外串行服务单位，含 PTE 请求/返回；是归一化总量，不是原生测得延迟），delta=4（2 串行 hint probe + 1 reserve/commit + 1 chosen-slot report/read）。  
- early fixed 恰好 hit：V=24,A=68。  
- early fixed 恰好 miss：V=44,A=88。  
- balanced 固定选择均值：V=34,A=78。  
- bounded late choose delta=4：V=28,A=72。  
- 软件同 fresh 信息 late choose：V=24+delta_sw,A=68+delta_sw。  
结论：信息时机示例使均值 34/78->28/72，但早期幸运热固定 24/68 更优；q2,N2,h1,M20 给 6 个期望单位；没有建立相对 fresh-information 软件优势。  
随机池过滤：gain(q)=M*((1-h/N)-C(N-h,q)/C(N,q))-delta(q)。N=64,h=8,q=4,M=20,delta=6 -> -0.06134；N=256,h=16 -> -2.67517。这否决“随机大池正收益”的预设；有用域需要足够集中 eligible choice 和相对最佳 LIFO/page-local 软件策略不可预测的残余驻留，或更大且不被隐藏 M。  
成本阈值：参考 F=4 pool/frontend,P=64 slot/pool,q=4；pool_bits=696，ownership_bits=8704（state2+slot_generation32），parent_bits=980+4O，candidate_bits=624，completion_bits=308，arbiter_bits=75；合计 11387+4O bits；O=32 示例如 11515 bits，向上取整 1440B。未计：packing/ECC/pipeline buffers、额外 full tag/PA shadow、真实下游队列、所有 ID/parent completion metadata、CDC、register 接口、extra pipeline；端口最多 4 cycles 及对普通流量干扰；并行替代要 4*16 comparisons/routing/context checks；software 1MiB DDR（4*64*4KiB）及 OS pinning、映射、cache 同步、free bookkeeping、CPU register traffic、指针 indirection。1440B 可买可观普通 IOTLB 容量；若同预算普通容量扩展以更低端到端成本解决工作集，则在该范围停止该架构。  

## 4. R 备选卡摘要
机制假设：保留分别验证的 source/destination translation，在稀疏 COPY 对之间共享学习的 VPN delta 和 PPN delta。表示子集：V=29,P=44,A=8；uncompressed mapping V+P+A+1=82bit，16 映射 1312bit；pair_row V+P+2A+2=91bit，8 行 728bit；group dv+dp+valid 74bit，2 组 148bit；pair_total_subset=876，ordinary_total_subset=1312，节省 436bit，33.2317%。若 identity 用 PSCID20+GSCID16+stage2=38bit/group，双方各加 76，则 952 vs 1388，仍省 436bit（31.4121%）。  
但 **generic arbitrary relation dictionary exactly matches compressed fields**：同 91-bit row 和 74-bit group 可精确复现全部字段节省。因此当前正例只是精确表示大小构造，不是净 RTL 面积、更少真实 miss、更快 copy 或已建立 novelty。必须先比较完整 discovery/encoding/lookup/invalidation/fill/ports/critical path/transient/proof state 成本。  
强反例：每目的独立 remap 使 G=N，八 singleton 时 1320 vs 1312，节省消失；只 source 复用会保留 destination attributes/validity 并浪费容量；全相邻仿射时 CoLT/range/clustered/hugepage 可覆盖多于两个映射；每对只用一次或普通 IOTLB 已装工作集时冷验证不变；order-1 pair 分配失败/migration/bank locality 成本可吃掉收益。  

## 5. T 低优先级卡
候选是“下一次翻译访问最早到达承诺 + 安全数据片段”：26/33 vs conservative 26/34 仅在 V<=26；但 unrestricted longburst 31/22 的 joint sum 和 makespan 更好，同证书 max-fit 完全复现候选。原生 PTW 续步仅一拍，因此当前不把 T 作为优先实现。只允许一次定向可达性检查；若 <=1 useful unit 或普通 fine-grained arbitration 已足够，停止。不得人为给 walker 加延迟制造成正例。  

## 6. 主要前作与准确范围
- io_uring_prep_provide_buffers manual：https://man7.org/linux/man-pages/man3/io_uring_prep_provide_buffers.3.html，read description lines27-70；只支持从注册组选择输出缓冲并返回 ID；manual 未描述用当前 IOTLB 驻留选择。  
- DAMN 2018：DOI 10.1145/3173162.3173175，https://dants.github.io/papers/damn-asplos-2018.pdf，read sections5.1,5.3,5.4 pp5-7；持久 per-device/access-right DMA pool、chunk/bump 和 magazine；裸持久注册/池复用不能声明新。  
- IOTLB wall 2023：DOI 10.7717/peerj-cs.1385，https://dejankostic.com/documents/publications/iotlb-peerj23.pdf，read pp7,18-22；页局部性、hugepage-aware allocator、recycled-buffer shuffling 已明确讨论，必须作控制。  
- AXI-REALM 2025：https://arxiv.org/pdf/2501.10161，直接读 III-A 及 III-C；ingress burst split/write buffer/budget、egress outstanding、stage latency tracking、runtime granularity/dynamic budget；未覆盖未公开下一 PTE 的最早到达不可提前承诺。  
- AXI-REALM 2023 preprint：https://arxiv.org/abs/2311.09662；已含基本分片与 period budget，不能把 2025 当首次。  
- MASK ASPLOS 2018：DOI 10.1145/3173162.3173169，https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf，read 5.4 lines889-947；Golden Queue 优先级/应用干扰管理；仅给 PTE 更高优先级不足以新。  
- Scheduling Page Table Walks for Irregular GPU Applications ISCA 2018：DOI 10.1109/ISCA.2018.00025，https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf；机制细节沿用 round01 原文核验，本轮不声称重新完整审查。  
- CoLT MICRO 2012：https://www.cs.yale.edu/homes/abhishek/binhpham-micro12.pdf；author-hosted primary PDF text returned in search，direct open currently 404，因此**没有 fresh full-text review**。  
- Increasing TLB Reach HPCA 2014：DOI 10.1109/HPCA.2014.6835964，https://www.cs.yale.edu/homes/abhishek/binhpham-hpca14.pdf；direct retrieval failed，只能作 nearest-prior follow-up，**不能声称已全文核**。  
- Coalesced TLB 2019：https://arxiv.org/pdf/1908.08774，read primary arXiv v2 abstract、definitions、3.1/3.2；variable span/compression 已覆盖，不证明任意 sparse COPY-pair 压缩。  
- WO2025226345A1：https://patents.google.com/patent/WO2025226345A1/en，disclosure lines134-145,186-237；shared PA/attribute bit factoring 不是新，不提供法律可专利意见。  
- WO2022133841A1：https://patents.google.com/patent/WO2022133841A1/en；地址预翻译/job descriptor 已公开，携带 prepared address 本身不能作为新贡献。  
- US7480784B2：https://patents.google.com/patent/US7480784B2/en；IOMMU 专用 virtual channel 打破 pagewalk/data 死锁；不是性能对照。  

## 7. 新颖性边界
- L：provided buffers、persistent DMA pools、locality-aware allocators 都已存在，裸 API/局部性不能作为贡献。只有当前翻译状态在付费晚决策点给出真实工作负载上的有用、不可复现 full-cost 能力，且 exact nearest work 不覆盖，集成交互才可能成为独立机制。  
- R：显式 generic relation dictionary 精确复现编码，所以字段节省不推出独立压缩算法。若完整低成本机制的 discovery/lookup/invalidation/cost point 与最近工作留下差异，才可能贡献。  
- T：AXI-REALM 和 same-certificate max-fit 已覆盖调度，只有新的有用 arrival contract 才可能贡献；当前 native timing 不支持前提。  

## 8. 下一批 7 项：L1-L4 / R1-R2 / T0
| ID | 输入 | 最小验证 | 推进标准 | 停止标准 |  
| L1 | 从原始开始、合法完整 COPY/release 历史；native PLRU/partition masks；source/dest checks；真实 trace，不是注入两个快照 | 生成相同软件 allocator 观察但不同有用目的驻留 | 至少一个有用 hidden-state mismatch 能存活 cheap LIFO/page-local prediction，且所选 slot 真 free/eligible | 无可达 mismatch，或普通 source lookup 总破坏 prospective hit |  
| L2 | 使用全部 native 资源容量的小事件模型；fixed、software late+probe、hardware late、full prepare+retention、extra ordinary entries 同总预算；M/delta 用实测或 sweep；完整 V/A 完成和 CPU 路径 | 比较 full-cost V/A 或性能/成本 frontier | 存在 nontrivial region；不要求证明全局普遍优越 | 即使理想 fresh-state choice 也输最佳真实对照，或节省翻译工作完全被隐藏 |  
| L3 | N=2/4/16/64，q=0/1/2/4，hot fraction 和 consumer return order；allhot/allcold/singlefree，hugepage/layout controls，fixed-address consumer extra-copy | 局部性/可用性 sweep | 收益与 residual-miss*unhidden-M > full overhead 对齐，fallback 限制负邻居 | 只有手工制造初态或削弱 allocator 才得收益 |  
| L4 | 原子 reserve、immutable firstVALID、完整 completion storage、stale advisory probe、permission fault、consumer cache sync、generation wrap 的规格和手推 | 无 free permission、无过早释放、无死锁；所有字段/端口计账 | 正确性不需要未计价 shadow/cache/parallelism | 需要未预算 shadow/cache/parallelism 或不兼容 native semantics |  
| R1 | 等 fixed-4KiB/context factoring 的完整成本表；singleton escape；COPY hints 给普通 relation dictionary；实际 paired mapping lifetime 和 allocator cost | 超越字段算术的容量/延迟/面积点，或精确最近公开机制区别 | 保留 useful representable point，或识别 exact nearest distinction | 同信息普通实现 discovery/encoding/lookup/invalidation/full cost 完全相同，且主张是独立算法 |  
| R2 | 若 R1 通过：最小 repeated sparse COPY 模型，neighbor G=N，source-only reuse，half invalidation/remap；16 first cold permissions、8 real copies/round、full buffers | 全成本系统优势或同服务更低完整实现成本 | 有优势或更低成本 | 普通 compression/layout 或额外 decode/proof state 下节省消失 |  
| T0 | 读现有 final shared service resource 和结构延迟边界；除非真实长 non-occupying gap 和 bounded service 已识别，不建新模型 | 真实 g>fixed fragment overhead，且不是人为加 walker delay | 存在真实可达空窗 | native 一拍续步/fine-grained service/无有限 bound |  

执行语义：L2 只要求 nontrivial region，不要求预先证明普遍优越；R1 的停止条件是同信息普通实现完全一致且主张为独立算法；不要用“预先普遍优越证明”阻止最小模型。  

## 9. 执行和声明
本轮 model_runs=0，rtl_changes=0，linux_changes=0，builds=0，board_runs=0，claude_calls=0。closed_form 为手推加 3 个概率 case 的直接求值，不是工作负载模拟器。所有数字都是服务单位/字段子集/手算或闭式，不是实测 cycles；不得四舍五入成实测 cycles。不宣称首创、不宣称已达二区、不宣称模型通过、不宣称 CoLT/HPCA2014 已全文核。下游实现/复现由用户指定下游完成；本稿只做机制设计和可执行交接。