# §44 Native Codex：Sv39两阶段续走表示、普通同字段构造与停止裁决

2026-10-08，`innovation_rules_40`。本席直接读PTW/TL/package及Rocket 2022关键原文，亲自推演phase字段、恢复公式和条件下界。root独立推得同union，并补出partial restart与持续进度约束；map提供固定版本primary及错误路径质疑。本稿只做手工源码/理论审定，未修改RTL、未build/run、未执行数学oracle，未调用DeepSeek/Claude。

## 结论

**82bit是一个不含原请求/context、valid/token及终态队列的walk续走帧上界，不是普适最小状态，也不是面积结果。它的具体表示可由普通nested tracking逐字段实现；Rocket 2022已经有父PPN/叶PPN复用及从原请求重建索引的主体。因此停止把此表示作为独立机制。**

更强普通partial restart仅需36bit同类帧，单次handoff仍可只排空当前一笔PTE，再恢复时重读最多3笔G-stage PTE。但反复暂停可能不推进S-stage；持续进度须允许一次G+S语义quantum最多4笔PTE，或限制暂停次数/保证足够连续服务。不能同时免费宣称36bit、一笔PTE的统一让路界、无限请求下的有界完成。

保留未解问题是：在同架构输出、同持续进度/到达合同、允许普通PWC和合法cache优化时，多设备的状态容量、额外PTE工作、让路粒度是否有可证明完整frontier。当前没有完成该证明，下面几个点不能称新理论。

## 1. 范围、退休点及双方共同的C

这里的名称“Sv39×Sv39”指S-stage Sv39和G-stage Sv39x4，GPA实际41bit。当前package：VLEN39=39、GPLEN39=41、GPPNW39=29、PPN_t44bit；soc PAddrWidth56、AXI地址64。[package](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/packages/rv_iommu_pkg.sv:73)、[soc配置](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/tb/ariane_soc_pkg.sv:126)。

限定正常双stage请求、静态mapping、无硬件A/D写、无另一条生成payload/side effect的路径。DDT/PDT已解析，下面不是其implicit-only walk、MSI/MRIF或fault队列本身的完整checkpoint表示。不过续走时后来的PTE仍可能非法或返回AXI error，不能以“已走过的前缀正常”删除未来fault所需target。

合法walk暂停点为：当前PTE的R真实接受且全部有效性/属性检查已完成，当前响应归属已明确，下一PTE AR尚未首次VALID。如果pause时当前AR已firstVALID或已accept，先退休该笔，不能撤VALID或误丢R。此处只比较walk执行槽/DS尾部释放；原译址请求仍有最终reply义务。

共同不可变信息记为C，必须有真实存储或受保护引用，双方同计：

- 原请求：IOVA、DID/PID/pid_valid、ttype、priv、debug、bank/response归属。当前top原struct是64+24+20+1+6+1+1+1=118bit；按合法固定模式压缩可另做，但双方用同条件。[request struct](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv:135)。
- 解析后的S/G根、PSCID/GSCID、mode/enables及权限策略、SUM、DTF、MSI判别等需要的DC/PC内容。若cache way会被其他请求替换，C不能仅留一个未保护way引用。
- 请求身份、context slot/generation及已accept请求/最终reply关系。下面不将它们藏入“免费的token”；若有n个可选择C槽，引用位数至少按实际槽数计，通常为ceil(log2 n)，valid和调度状态另计。

PTW输入由TL live寄存器和req_data提供：`iova_i=req_data.iova`、roots、PSCID/GSCID、store/RX、MSI、implicit信息；IOTLB更新直接读取这些输入。[TL连接759–805](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv:759)。因此切换执行context必须连同C选择；只恢复PTW pointer而覆盖live request会错填cache tag/错报fault。

## 2. 符号与三个phase

S-stage下一/叶level为j∈{1,2,3}；当前G-stage level为k∈{1,2,3}。索引

`s_j(V) = V[38:30], V[29:21], V[20:12]`，分别9bit；

G-root索引是target[40:30]，11bit；后两级G索引是target[29:21]/[20:12]，9bit。令`m(1)=18, m(2)=9, m(3)=0`表示superpage置换的低PPN位数。

- **I(j,k)**：正为第j个S PTE地址做G-stage翻译，下一操作读G PTE。
- **S(j,k)**：G leaf刚正常退休，下一操作读第j个S PTE；k保留刚退休G leaf的level，用于压缩其物理映射。
- **F(j,k)**：S leaf已正常退休，正为最终GPA做G-stage翻译，下一操作读G PTE。

I/F的k为下一G PTE level；S的k为已退休G leaf level。它们不同含义，由phase区分，不把S阶段的物理pointer level误当Slevel。

27个组合可用5bit联合tag：I编码0–8，S编码9–17，F编码18–26；每组以(j−1)×3+(k−1)定位。I(1,1)是初始门，可不作为R退休产生的checkpoint；删除它仍需5bit。这里是固定tag编码上界，不是与最大payload独立相加的最优信息下界。

## 3. 最有希望的表示与精确恢复

### 3.1 I：S语义target与当前G表根分开

令T为当前S页表page的guest PPN，29bit；`X=(T<<12)|(s_j(V)<<3)`是该S PTE地址的GPA。j=1时T直接由C的S根低29bit得出；j>1时T是以前正常S nonleaf返回的PPN，不能从原IOVA/初始根不读内存地推导。

G k=1从C的hgatp根及X[40:30]构造请求，不存当前G table；k>1存最近G nonleaf的物理table PPN Q，44bit，构造`(Q<<12)|(g_k(X)<<3)`。不保存完整56bitcurrent pointer，也不同时存41bitX。

S父nonleaf累计global记gS，1bit。j=1尚未读任何S祖先，gS固定0；j>1须保留。最近G PTE不能恢复此前S ancestor的gS。

### 3.2 S：旧target不能消失，G superpage映射可因式分解

刚退休G leaf的正常检查已完成，其权限无需在后续S读重复检查。保留其物理PPN高`44−m(k)`bit，低m(k)由G superpage合法对齐已知为0。S PTE所在的物理page

`Y = concat(Gleaf_PPN_high, T_low_m(k))`，

下一物理S PTE地址为`(Y<<12)|(s_j(V)<<3)`。故存T和Gleaf高部即可，不存完整mapped physical pointer。若不用superpage压缩，则保存Y44也正确，只会使某些phase payload更宽；整个帧最大值仍由F决定。

**为什么读S PTE时还需旧T？** 当前[608–615](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:608)把高S PPN非法的异常改成STAGE_2_INTERMED，ERROR用gpa_x报告bad GPA（[641–644](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:641)）。若返回的是高PPN **leaf**，gpa_x未更新，需旧X；若高PPN **nonleaf**，525/558先计算新gpa_x_n，随后错误状态保留它，可能报新truncated X。不能统称所有高PPN fault都报旧X。

最小不可省见证：同原请求/C，两组合法上游页表使第j>1个S表位于不同guest T，但G映射alias到相同物理Y；在S checkpoint只留Y、j、相同gS，会得到相同保存状态。未来收到同一个高PPN非法S leaf，需报告不同旧X。若不允许额外重读或外部保存T，就无法正确区分。因此当前fault合同下，mapped physical page不能代替guest target；这不是新机制，只是压缩的正确性边界。

### 3.3 F：用S leaf推导最终GPA，不同时保存gpaddr与leaf64

S leaf正常退休后，raw S leaf PPN高`29−m(j)`bit是独立值，低m(j)已由superpage合法对齐为0。原IOVA提供其置换位及低12bit，可重建final GPA；不再保留旧S PTE target或gpa_x。

为了重建当前源码完整IOTLB content_1S，保留aS=(w,x,u,g)，4bit，g已合并S ancestor global。当前PTW正常leaf检查保证v=1、r=1、A=1；store时D也已通过检查，当前IOTLB更新不存A/D。RSW/reserved无后续用途，非法值应已走terminal分支，而不是被压缩时丢掉。[PTW leaf checks](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:490)、[IOTLB输出字段](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:218)。

G k=1的current table仍从C得出；k>1存Q44。最终GPA的29bit page部分由S leaf与V推导，G-stage错误报告的低12bit也由V得出。这里的v/r常量优化依赖当前PTW成功路径，不能外推所有CPU execute-only leaf或另一个A/D实现。

## 4. phase位表：payload＋固定5bit tag

表中仅列额外continuation payload，不含C、valid/token、terminal结果、调度/世代状态。k列在S phase指退休G leaf level。未做综合；较小的phase payload不会自动减少一个统一宽度slot的面积。

| phase、j | k=1 data | k=2 data | k=3 data | 必要来源 |
|---|---:|---:|---:|---|
| I，j=1 | 0 | 44 | 44 | T/root、gS从C/常量；非root Q44 |
| I，j=2或3 | 30 | 74 | 74 | T29+gS1，非root再Q44 |
| S，j=1 | 26 | 35 | 44 | T从C；G leaf高44−mG |
| S，j=2或3 | 56 | 65 | 74 | T29+gS1+G leaf高44−mG |
| F，j=1 | 15 | 59 | 59 | Sleaf高11+aS4，非root再Q44 |
| F，j=2 | 24 | 68 | 68 | Sleaf高20+aS4，非root再Q44 |
| F，j=3 | 33 | 77 | 77 | Sleaf29+aS4，非root再Q44 |

统一slot可以使用77bit data union+5bit tag=82bit。S父T区在F phase变为S leaf/属性区；Q区保存当前G nonleaf或退休G leaf的必要PPN部。无需额外原IOVA64、X41、final GPA41、leaf64或完整next pointer56。

**82不是最优编码证明。** 最大77bit data与27种tag的值域不一定独立，联合密编码可能进一步合并tag信息；尚未证明最小record width，也未实现这种编解码或allocator。固定宽度slot不能将“此刻只用15bit”冒充容量可供别的context复用。

## 5. 条件下界与它不能证明的事情

固定F(j=3,k>1)、固定原请求/C，若允许任意可合法访问的56bit physical PTE table page，当前G table Q44、S leaf PPN L29可以独立变化；同当前普通load/SUM等可使w/x/u/global四属性也独立。立即发出的G AR区分Q；后续G-stage fault的bad GPA区分L；保持完整IOTLB更新内容的合同区分aS。故这一**固定phase、无重读、无外部保存、完整原IOTLB fill内容**合同至少需要77bit数据。

这个计数针对支持的地址空间/页表族，不能从PAddrWidth56推出真实FPGA DDR有2^44个可放表的page；若物理分配范围已限定，地址熵相应变小，普通也能用同限制。不能把77再加tag5当完整信息下界。

更关键，完整IOTLB fill内容不是一个必须人为附加的架构要求。允许普通skip-fill/direct response、提前形成S permission verdict、合法调整cache布局等时，aS可能不必完整保存；例如本请求的S-deny verdict可更小，但须核最终response权限及fault优先级，不能直接宣称一个1bit verdict支持未来不同访问。当前未证明architecture-only最小位数。

因此77只是检查丢字段是否正确的条件性下界，不是通过“强制cache逐bit相同”建立新颖性的工具。真正比较应让双方都使用合法cache优化与等架构输出。

## 6. 普通同字段构造：具体实现，不用“任何规则都能抄”作理由

### 6.1 最小tracking token＋原请求＋最近PTE

普通tracking entry保存C引用、5bit phase/levels及上述77bit union；最近PTE只提取已通过检查的PPN/必要属性，不机械保留raw64bit。

- 退休S nonleaf：把其guest PPN写T，累计gS，设I(next j,1)，G根从C重建。
- 退休G nonleaf：PPN写Q，k加1；T/Sleaf区不动，从target及k重建下一index。
- 退休G intermediate leaf：设S(j,k)，Q区存该leaf必要PPN高部，T/gS保持；从T与G leaf重建S物理读。
- 退休S leaf：T区改存其PPN高部与aS，设F(j,1)；从leaf和原IOVA重建final target。
- 退休最终G leaf/error：先产生/保存对应结果并完成该terminal交接，不能把这个已发生的verdict作为“restart未完成工作”丢弃。

它与候选逐字段同宽、同恢复公式、同PTE尾部释放及零重读保证。这里不是泛泛模仿：Rocket 2022固定版本已经使用`r_pte`保存当前物理PPN，`aux_pte`在S nonleaf父PPN与S leaf之间复用，target/index从`r_req`与aux恢复，count/aux_count表示两层level。[官方固定源码](https://raw.githubusercontent.com/chipsalliance/rocket-chip/bf5742b9e353a80f501d43130218d3a444ec4b30/src/main/scala/rocket/PTW.scala)、[本地核读158–214](/home/xvhaocheng/work/iommu-platform/out/innovation-frontier-reset-20261008/continuation-primary/rocket-2022.scala:158)、[559–565](/home/xvhaocheng/work/iommu-platform/out/innovation-frontier-reset-20261008/continuation-primary/rocket-2022.scala:559)。

Rocket自身仍有full PTE和CPU其他fault/response字段，不能说该版本发表了82bit checkpoint。上述82bit是对其已明确的PPN+target+levels表示，按当前IOMMU成功/继续路径做字段精简；5bit联合tag较分开2+2+2编码省1bit，superpage低位从输入恢复也是普通地址构造。没有改变保存的信息、可继续的工作或调度保证。

### 6.2 同预算PWC

普通PWC可以保存已读出的G partial mapping/table PPN，并在restart时跳过该部分；entry还需target prefix、context tag、level及valid，必须按真实格式计入同预算。不能声称一个任意标准PWC entry在82bit内自动支持所有phase或保证无重读。

若PWC是专属于暂停context且有C token，保存上述parent/leaf与G partial记录，它可直接采用同union，成为同82bit普通tracking表示；若面向多个context共享，它可能因prefix共享更划算，也可能替换而失去保证。比较必须允许这一控制，不拿full raw PTE snapshot当唯一普通成本。

### 6.3 更强partial restart：36bit，不丢已完成S语义进展

root补出的具体普通点：暂停时只保存S语义状态，丢正在/刚完成但未被S读消费的G子walk进展。I/S都映射到I(j)：存T29+gS1，j=1时由C恢复；F(j)存S leaf高部+aS4。六个I_j/F_j状态可用3bit joint tag，最大payload33，故36bit+C/valid/token。

恢复从C hgatp root重建该G target并重走本子walk；此前正常退休S nonleaf/leaf无需重读。正常静态无A/D写时，丢弃未对外完成的G partial进展不会丢side effect。一次暂停最多重新读取3笔G PTE，包含在G leaf刚退休但尚未读S PTE时丢掉的整个3级映射。它比“整个walk restart最多重读14笔已完成PTE”强。

这也有具体普通依据：Rocket `do_switch`及root构造已按aux parent/leaf+原请求启动G子walk，见上述559–565及593–599。不把此低状态点称新算法。

## 7. 一次handoff与持续进度必须分开

82bit普通tracking保留每个PTE进展：在正常静态路径与公平获得执行机会下，按一笔PTE轮询可以单调推进，最多15笔自身PTE工作完成双stage walk。其真实周期界仍需要每笔PTE服务界和terminal/log/reply合同。

36bit partial restart的一笔PTE尾部界仅说明一次handoff：当前已offer PTE退后保存语义parent/leaf即可让路。它**不**证明每次resume很快再暂停时仍有logical progress。

有限两次打断反例：A正为某S PTE做三层G walk。

1. A读G-root，正常返回nonleaf；在下一G PTE首次VALID之前暂停1，丢Q；V1正常完成。
2. A恢复，又读同G-root；在下一G PTE前暂停2，仍保存完全相同I(j)；V2正常完成。
3. 此时A两次得到执行且memory均正常，但同一个S PTE完全没有前进。只要后来允许连续服务，它会完成；将这种到达重复任意多次可造成任意大重读，不依赖不公平memory。

更强版本是在G leaf完成后、S读尚未offer时每次暂停；每次连读3笔G也仍不能推进S，因为刚获得的映射被丢。故“允许G子walk最多3笔连续服务”还不够：I需G≤3＋S读1的语义quantum≤4；F需G≤3到terminal结果交接，或采用暂停次数上限/足够连续服务合同。

若选择4-PTE保护quantum，下一victim的最坏非抢占等待也要按最多4笔服务计，而不是继续宣传统一一笔。若限制总暂停M次，在上述静态模型额外G读可上界为3M，但若无限新到达仍不断截断，没有固定M，不能套这个界。

## 8. 当前裁决与唯一下一最小判别

| 普通点 | 额外walk帧上界 | 单次handoff尾部 | 恢复额外PTE工作 | 持续进度条件 |
|---|---:|---|---|---|
| 完整因式tracking | 82bit | 当前一笔PTE | 0 | 保留每PTE进展＋公平执行；terminal另计 |
| S语义partial restart | 36bit | 当前一笔PTE | 每pause≤3笔G读 | G+S quantum≤4、或有限暂停/连续服务 |
| whole-walk restart | 不存PTW进展；C/身份仍需 | 当前一笔PTE | 重读已完成前缀，单walk最多14笔 | 有最终足够连续服务或有限暂停 |

上表是手工构造的三个可比较点，不是完整Pareto frontier。不同持续进度条件不能混成同一性能/释放合同，frame宽度也不是总面积或完整context容量。

**因为普通Rocket-style tracking具体保存相同parent/leaf/物理PPN并从原请求推导offset，82bit表示的主体及其保证已被覆盖；它剩余的字段修剪和joint tag精简不能作为独立创新。因此这一表示枝停止。**

若今后研究frontier，唯一最小判别先固定：架构可见结果/故障、cache优化自由度、N个设备及C槽成本、有限或持续到达、重复暂停允许次数、每PTE服务合同、所要求handoff与request progress。然后用同预算普通因式tracking、partial restart和共享PWC构造可达状态/工作点，尝试证明一个缺口；尚未做此完整模型。

如果新点仅通过丢cache属性、隐藏C/terminal queue成本、重复暂停却忽略livelock、对比full64bit PTE而不给普通字段精简得到，应立即否决。现有强primary证明表示原理与实现方式已存在，未证明它们已发表这个完整frontier；这项未知不能代替新的下界/独有结果，也不为此先build。

