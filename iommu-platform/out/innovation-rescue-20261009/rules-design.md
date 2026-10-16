# §49 正向候选：权限检查后的共享 R，成员可按既有 W 债务退组并独立补读

2026-10-09。Codex 本席独立提出共享输入 R 融合及 commit 前 W 小组塑形；root Codex提出中途成员脱离、保留 logical parent 后补读；本席补充 first-VALID 退出边界、原文强对照、成本和反例。仅关键源阅读/纸面推演，无 RTL 改动、构建、DUT、BIT 或 Claude/DeepSeek 调用。

## 1. 本轮要做的实际动作

对两个正常独立 copy job，**分别**完成源 READ 和目的 WRITE 的权限检查。如果已保存结果表明两个源片段指向同一物理地址、访问属性相容，且该输入在注册的只读 epoch 中保持不变，则将原本两笔物理数据 R 合成一笔，按每成员自己的 ID、parent 进度和 RLAST返还。目的 W 仍是两条独立流。

在物理共享 R 首次 VALID 前，先根据两 DMA 真实可预约私有停车容量，把接下来的**虚拟**目的 AW 在其首次 VALID/owner-push 前塑形成小组，按 V/A 顺序提交。这样广播 R不会因慢成员 FIFO 满而把快成员卡在整长 W burst 的中间。

当 A 预算关闭，保持物理 R 的 ID/LEN/RLAST不变。A 仅继续接收清偿已有 W 债务和已 offer 返 R所需的前缀，然后从后续 fanout mask退出；V继续接收原长 R。A 的原 logical AR/parent仍在其私有窗口中，恢复时从准确已交付 offset补读。暂停 A 不要求完成整 shared R，也不取消 A 的 job。

这是保留的正向微架构候选。目标同时是：无暂停时少一半重复源读；有独立暂停时，源长读不把另一成员的继续工作量扩大为整个 quantum。不是把 refcount/history/frontier重新命名。

## 2. 合法输入与平台边界

一个具体两 job 工作负载：

| job | DID/context | source IOVA | source PA | destination PA | length |
| --- | --- | --- | --- | --- | --- |
| V | DID10、context V | 0 | `0x80000000` | `0x80010000` | 2KiB |
| A | DID11、context A | `0x20000` | `0x80000000` | `0x80020000` | 2KiB |

destination IOVA 可各为对应 context 的正常输出区域。两个源虚拟地址/context 可以不同；它们的权限必须分别检查，得到 PA 相同后才融合。两目的与源物理区间互不重叠，输入由驱动/负载保证本 epoch 内无 writer，输出各自独立。实际 copy API 的完成/错误归属仍分别保留。

只比较 PA 不足以允许融合：还需相同正常 memory 类型/相关 AXI访问属性、无 exclusives/原子/MMIO副作用、源内容稳定及一致的可用 mapping epoch。本轮限制普通静态 mapping、正常成功、8B对齐的 DEFAULT copy。这个只读 epoch是**额外明确合同**，不是由“mapping static”自动推出；若没有它，后面的补读可能观察不同内容，必须取消此优化而走普通 copy。

producer已有 parent/returned元数据，可以保存部分 logical AR进度；但当前实现没有 R fanout、目的 mapping recipe或 W塑形反馈，不能声称已装有候选。

### 2.1 24B/flow 的确切来源

§43 的 [实际 build argv](/home/xvhaocheng/work/iommu-platform/out/dual-write-prefix-20261008/build-argv.json:1) 明确是 `-GDmaBufferDepth=3`；top 的 DataWidth为64，两个 DMA均传此参数。真实 [idma_buffer](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_buffer.sv:33) 对每个 byte lane建深3 FIFO：8 lanes×3byte=24B/flow，总48B。transport [实例连接](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:468) 把该 BufferDepth传入。

这仅是该小 FIFO诊断配置。生产 wrapper默认/实际大 BufferDepth的容量不能被写成通用24B。

还存在 TC 的 W FIFO与 spill：[TC W FIFO](/home/xvhaocheng/work/iommu-platform/out/natural-write-order-20261008/trans_ctl_write_limit.sv:225)。它们需要成本计入。但是在 input axi_mux当前 W owner仍为V时，A 的 W不能进入这条下游路径，故不能把 TC所有W storage当成A可用停车容量。需要 root核当前队列/路由后，给每成员的可达 capacity证书，而非只看总 byte数。

## 3. 直接广播为什么会失败，以及候选改变哪一步

在正常两 copy各有2KiB源 parent、V先 offer目的2KiB AW、A随后 offer目的2KiB AW的状态中，virtual/physical W order均为V→A。不存在AW倒置。

假设两源结果都成功，把一笔2KiB物理 R用普通 fork送给两个 DMA。V的 W可流式排空，A尚未成为 input W owner，其private FIFO只能停24B。fork若等待每beat对两个sink都交付后再前进，到第4beat前后，A满且暂不能交W。V还远未收到256beats，不能交其virtual WLAST。A须等V WLAST才成为W owner，而V须等A继续接收广播 R，形成环：

`A R停车满 → shared R停 → V数据不足/WLAST未到 → A不能交W → A停车仍满`。

若 fork多一个 R skid，环点只是后移一个beat，不解决2KiB与24B的差距。对 root 当前 native更多旁路/寄存的精确停滞周期未运行；这是所定义 fork在该合法状态上的纸面反例，所有 memory路径仍正常服务。

**候选动作是改上游目的 AW 工作粒度，而非只添一个 fork。** 取最保守小组 `g=3beats=24B`，V组3beat、A组3beat交替。每组当尾beat进入上游W管线，virtual WLAST就真的到达，owner可以切换。A保存的3beat成为其下一组完整payload，从而清出空间继续广播。

只在下游 physical AW再分成3beat不够：上游原虚拟V AW若仍为2KiB，input mux仍等原V virtual WLAST，A仍无法交W。因此feedback位置必须在 [legalizer目的推进](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_legalizer.sv:254) 及原virtual AW首次offer之前，或采用完全保持等效virtual owner分组语义的新job adapter；不能已offer后改AWLEN。

跨bank也不能要求所有成员的physical AW预先接受：原inputdemux有bank W锁。需要的是**先保存各自WRITE翻译recipe**，再按顺序发短AW；不是凭空绕过bank锁。原source prepared-prefix producer只在physicalAW已接受时供data，其门不直接支持本候选的A私有停车，需要新的容量/供给规则。

## 4. 规则输入、状态、更新与输出

### 4.1 输入

- 注册 job `J_f={job_epoch,source,destination,length,context_ref,immutable_input_epoch}`；信息在producer/驱动已有或新增sideband，不是普通TL自动知道。
- 每 source parent 的 live/generation/full key、实际 R交付offset、已保存READ result；两个相同IOVA不等于有相同PA/授权。
- 每 destination 当前正常 WRITE result/页覆盖区间；需新增可保存recipe，不以IOTLB pin取代private结果。
- 上游 AW首次offer及其对应job-relative end，全部 held/accepted AW、virtual/physical W队列义务、已交W进度；firstoffer即不可撤，不能只数physicalAW。
- 各 DMA已预约的**可达**private free capacity，以及相关metadata/B队列可用credit；瞬时RREADY/WREADY不是未来容量保证。
- pause/resume，当前物理R、每成员返R的firstVALID/handshake。

### 4.2 必需状态

- 一个sealed cohort：members mask、每成员original parent slot/generation、logical delivery offset；不在物理R开始后加入新成员。
- 一个immutable物理R header及 quantum length `L`，全局R beat index；原AR可采用reserved fused ID，R要以该ID正确路由。
- 每held返Rbeat的未交付成员mask；若无payload skid，外部原R必须一直held直到mask清零。若增加skid，其全部data/response/last/ID位计费。
- 每成员exit-request/exit-cut、W debt frontier、已offer返R末端，以及private parking占用。
- READ和WRITE recipe及epoch/full key；原helper已存的READ result可以复用，但bridge端口和producer可见性要计。WRITE recipe当前没有可直接免费复用的长期字段。
- W组生成进度、每已offer AW的immutable header/子完成归属，以及源physicalAR与两个logicalAR/RLAST的重组状态。

### 4.3 建立和正常执行

1. **准备：**READ/WRITE各自检查权限，private保存结果。错误成员独立进入普通错误/回退路径；不把一个成员的成功替另一个成员授权。
2. **选择：**若两个当前source PA区间有同一prefix、属性/epoch相容，构造候选。`L`限制在已保存source/destination覆盖范围、两个logical parent剩余、AXI4KiB边界和明确的不可撤work预算内。否则普通独立copy。
3. **塑形：**每 destination新W group的length不超过该组已预约可达停车量、覆盖页边界和L剩余。仅修改尚未首次offer的虚拟AW。
4. **封闭：**物理 R首次VALID前封闭members和L，并保证必要未来W组可由已保存recipe产生、所需metadata/Bcredit能持续推进。不要预先制造整个L的两条W债务；group按有限窗口逐次提交。
5. **执行：**一笔source physicalR，分别返原logical ID；维护独立returned offsets和原parent RLAST。W按确定小组序发送，所有firstVALID保持。
6. **结束：**physicalRLAST退休释放共享R状态，各job只按其自身logical parent/R和W/B的原完成合同完成。

目的WRITE recipe让后续3beat groups不必重新walk每个group；这是private状态换请求的明确成本。有限IOTLB可以逐次替换正常cache条目，已承诺供给依靠private结果。普通ATC/translate-first也可以保留相同recipe，必须作为强对照。

### 4.4 成员暂停与精确脱离

记所有offset均为job-relative、8B beat边界：

- `d_A`：A全部不可撤AW offer覆盖的payload end；含held及入owner queue但尚未external accept者；
- `r_A`：已向A完成返R handshake的prefix end；
- `h_A`：已向A首次VALID但尚未handshake的当前返Rbeat end，没有则不约束。

退出点必须满足 `c_A >= max(d_A,r_A,h_A)`，且不超过已选共享quantum实际覆盖范围。pause到达后不创建新的无关A AW group，但preserve全部held offer。若held AW在后续真正进入队列，仍是旧义务，不能漏掉其end。

到c_A之前仍给A必要的返R；对应W可继续排空。c_A之后不再创建A返R VALID，将A从future fanout mask移除。**已有的A返R VALID不能靠mask清除撤回**：须完成其handshake后才脱离。正在外部held的同一物理Rbeat可以已给V、尚未给A；只在两者当前交付义务都消失后，原physicalR才可advance。

允许私有parking时，可选择把额外数据交给A并停车，至多其已预约B；这样c_A可以在W债务end之后，但这额外R工作和parking都计入暂停合同。最简单实现先只选不可撤最小cut。

V继续原physicalR；ARLEN/ID/RLAST不变。A的logical parent不退休，保存实际returned offset；恢复后先消费已停在privateFIFO中、尚未W的payload，再以**实际已返R offset c_A**生成补读。不能错误地用W offset作补读起点并重复返R。

补读可以从剩余READ recipe旁路，但只有recipe仍属合法epoch时；否则重新译址。由于source只读epoch，重复read观察同一数据。补读可分普通children，原logical RLAST只在真正完成原parent最后beat时产生。

## 5. 返 R 保持的最小普通实现与审定边界

无需假设双sink每cycle同时ready。对每个外部heldRbeat设 `pending` 初值为current members：

```
sink[f].valid = source_r.valid && pending[f]
source_r.ready = AND_f (!pending[f] || sink[f].ready)
发生sink[f] handshake: clear pending[f]
发生source_r handshake: pending <- next-beat members
```

当前source_r在ready低时按AXI保持全部payload。已收过该beat的sink不再收到同beat。退组只更新next-beat members；对已firstVALID的当前成员仍按其handshake清pending。

这是普通fork的具体保持逻辑，不单列创新。若多个physicalR IDs并行，必须逐route持有相应mask/header，成本不止两个bits。本稿为一个active fusedR quantum。是否可以完全无skid、避免组合ready环，需根对具体AXI连接审定；本稿没有实现或timing结论。

### 暂停后的“quiet”必须准确

A退出意味着它不再对当前shared R产生接收反压，其旧W/B/held接口按明确合同已排空，未完成logical parent和有限payload可以留在private区。

这不意味着shared R header/port全部quiet：V还拥有原R长事务，共享cohort仍须保留V状态到physicalRLAST。不能把A的退组标志当整IOMMU/DDR返回路由已释放，也不能假装其private parent完成。若系统要求pause必须使整个sharedphysicalR静默，本候选就须把L也限制在该系统要求的work界内。

## 6. 可执行正向 trace、work目标与最强反例

### 6.1 无暂停时的正向动作账本

对 §2 的2KiB同输入，两份独立READ权限成功后选择L=256beats，g=3beats：

- ordinary独立copy：两笔source物理R共512beats，两个输出W共512beats；
- 候选：一笔source物理R共256beats，两个输出W仍共512beats；
- 两logical AR各收到256beats、各自最后一拍RLAST，无额外R/W、不混ID。

32KiB同输入完全同步时，相同关系为sourceR64KiB→32KiB，destW仍64KiB。**这只是事务工作量目标，不是速度实测，也不是泛广播的新颖性。** 减少payload R有望减少共享DDR上与PTW的竞争，是否抵消短W header代价待判。

### 6.2 中途暂停的正向 trace

原物理R L=256beats正在流动。在offset6beats处A pause，snapshot含A已offerW至9beats，并有一拍A返RfirstVALID到7beats。则最小退出cut `c_A=9beats`：继续交到9，排空A已offerW，A保留rawparent returned9而非done；V继续完成256。A恢复后另读剩余247beats，完成其原parent。

总source物理R为256+247=503beats，小于独立copy512，暂停没有迫使A收满256。若暂停在128且旧债务cut为129，则总383beats；无暂停为256。可选privateparking能多保存至B范围，但需明计工作。

在“被A未收的后缀没有另存payload、没有免费数据cache/旁路”合同中，A恢复至少须再获得L-c_A beats，候选补读恰达到。该简单守恒只作为效果正确性基准，不包装新理论。

有限请求持续进度还需fair的候选/普通路径选择、目的AW/W/B在R held时可以继续服务、recipe范围完整，以及group必要metadatacredit不被无关futurejob占死。封closed cohort避免无限arrival延长当前事务，但不自动证明所有tenant延迟界。

### 6.3 第一项强反例：只读epoch缺失

V已收source offset100的旧值，A退出后CPU或其它DMA把该源字节改成新值。A补读得到新值，不能再解释为两个copy从同一只读input epoch取得同数据。静态页表不阻止此合法writer。

解决是此优化仅用于明确不可变输入epoch；其它job普通独立copy。不能以“原benchmark没有writer”宣称一般API等价。

### 6.4 第二项强反例：短 W 的代价可能直接杀掉性能

L=256beats、g=3需要每flow86个AW，共172个；两个长W仅需2个AW，多170个header。W beats没有减少。

若W/header或normalTL确认开销高、两个独立copy本来已把W带宽打满，候选少256个Rbeats也可能更慢。尤其不能把R也切24B来隐藏fanout进度，否则新增AR latency会更糟。强ordinary允许自己的source保持长R、目的同样短W；不能假定ordinary tiling一定生成同样多短AR。

可保留性能选项是：成员退组后，在尚未offer的新W上扩回普通长burst；正常cohort按可达free容量和明确work预算取最大group，而不恒定3beat。这个选择需真实容量证书；不靠瞬时ready赌未来。

### 6.5 第三项强反例：整共享 R release合同

若pauseA被定义成必须让整个共享物理R在3beat工作内结束，原L256一旦offer就不能满足。成员脱离只减A-specific债务，不缩ARLEN。必须在firstVALID前限制L或拒绝融合。此限制可能使候选退化成普通小块broadcast，源长burst/header优势消失。

## 7. 三个最新直接primary：它们覆盖什么、怎样作强对照

以下均已由本席浏览原文机制段；v1最初阅读后按root提醒改用DMA-Latte **v3当前版本**。未下载/运行作者artifact，不以全文没出现某词证明没有同机制。

| Primary | 直接机制证据 | 对本候选的影响 |
| --- | --- | --- |
| [A Multicast-Capable AXI Crossbar for Many-core Machine Learning Accelerators，2025](https://arxiv.org/html/2502.19215v1#S2) | II-A，web L105–124：多目的W的B join；有限Wbuffer下不同slave相反AW顺序可死锁；以一致选择/all-slave acquire和aw.commit保证。 | “finite buffer + multicast + commit避免死锁”已有直接先例，不能泛立新。其机制是写multi-slave acquisition；本平台不是制造相反AW顺序，目标是串行owner下两独立返R和成员退出。仍允许ordinary采用同样acquire/commit能力。 |
| [Torrent: A Distributed DMA for Efficient and Flexible Point-to-Multipoint Data Movement，2025](https://arxiv.org/html/2512.17589v1#S3) | III-A/III-C，L129–143：configuration、后向grant、transfer、finish；endpoint数据switch复制/转发，以链式P2P实现P2MP。 | 同source少一次read、显式P2MP及无大临时payloadbuffer的streaming是强前作。不能把endpoint广播、ready grant或source复用当新增点。应允许同信息ordinary chain/broadcast，计其route/endpoint/stream接口。 |
| [DMA-Latte: Expanding the Reach of DMA Offloads to Latency-bound ML Communication，v3 2026-09-14](https://arxiv.org/html/2511.06605v3#S6) | IV-B L179–182：MI300X bcst一source/two destinations、source只读一次；VI L308：runtime检查hipMemcpyBatchAsync的source/size透明推断broadcast；V-B5–8实际比较bcst/b2b/pcpy/size范围。 | 独立copy自动归broadcast以及减少sourceR均已覆盖；不能靠显式广播命令较弱或v1缺少runtime段立差异。候选必须比较成员暂停、已有firstVALID保持、logical parent精确补读和work界；性能对照允许size-based broadcast/b2b/pcpy选优。 |

对应固定HTML原文：

- https://arxiv.org/html/2502.19215v1 ，II-A；官方作者实现 https://github.com/colluca/axi/tree/multicast 。
- https://arxiv.org/html/2512.17589v1 ，III-A、III-C；本文还给出chain setup有成本，不能假装普通链式广播零开销。
- https://arxiv.org/html/2511.06605v3 ，IV-B、V-B5–8、VI；作者artifact https://github.com/AMDResearch/AMD-DMA-Latte 。

## 8. 强普通控制与具体增量判据

所有控制获得相同job/context、READ/WRITE result、pause、firstoffer debt、private容量和logical returned字段，且保留实际AR/AW/R/W不可撤规则。

1. **普通copy调度：**tile barrier、finite outstanding window、remaining-use/ref protection、translate-first/lookahead、source长R/目的短W、普通return VOQ/owner gate。它们本身没有把重复payload R变成一笔；数据动作能力缺口是实际2L→L，不是cache history修复。
2. **普通broadcast/read-coalescer：**允许closed cohort、共享fetch/private Info、动态priority、有限buffer、普通AW裁切。如果它把logical请求视为不可部分返回的整broadcast成员，则pause必须等整个cohort或保存全后缀；本候选保留partial logicalparent而补读，实现成员级退出。不能人为禁止普通保留cursor。
3. **最强普通动态fork/drop+native partial replay：**它可以采用与§4–5完全相同的mask、cut和cursor，对未交成员尾部补读。如果这个**具体实现**在相同成本下满足成员W debt、firstVALID、logical RLAST和持续进度合同，那就是候选的直接等效实现；不要再宣称独占的新算法。需要比其现有published/API/RTL合同与代价，而不是说“任何普通算法都能编程出来”就中止。

保留的创新假设明确为：**权限分别检查后的共享数据R，在长physicalR已offer时允许成员按自己的真实非撤W义务退出；通过尚未offer的virtualAW塑形使有限privateFIFO可持续复制；余下logicalparent用准确cursor恢复。** 三项动作在同一bounded-storage接口中协同，改变copy数据事务和独立pause语义，而不是给普通coalescing添加新名称。

如果上述合同能在同payload/完整metadata成本下，比atomic broadcast或强ordinary动态fork/replay提供明确更短A continuation、保持V长R或更少header/重读的实际点，就可以论证独立微架构增量。因为最新primary已经覆盖广播、自发现以及commit防死锁，不能把那些单项或source读减半单独作为创新。

## 9. 最小后续判别：先保留正向机制，再检验最危险点

本轮不实现。建议root下一步只选两项明确合同见证：

- **正常正向：**2KiB同PA的双正常copy，原source parents/IDs/RLAST，24B/flow，W组交替；事务账本一笔physicalR、两个logical R完整、两输出完整且可终止。比较same短W ordinary长R独立copy与明确broadcast强控制，不只compare弱LRU。
- **独立暂停：**sharedR中段、A已有一条held返R VALID且AW队列仍有未physicalaccept offer；A继续到准确cut然后退出，V原physicalR继续，A恢复仅补剩余；检查每beat唯一、原parent最后beat/RLAST、heldVALID保持及A-specific quiet。不是要求全sharedR在同一时点quiet。

first gate是根对新的R/W接口可达性和原virtualW粒度的审定。性能 gate是新增AW数量能否被sourceR减少抵消；若无正向吞吐点，可缩小cohort适用范围或停止该实现，不能只给理论读减半当速度。

这一候选目前有具体动作和可证伪合同，尚无DUT/综合/性能证据。paper创新需要精确最强ordinary/primary比较，不能凭“文中没写退出”自动宣称世界首创。

## 10. 对 root 首版事件模型合同的关键审核

直接阅读 [root设计](/home/xvhaocheng/work/iommu-platform/out/innovation-rescue-20261009/codex-open-copy-design.md:17) 和 [模型合同](/home/xvhaocheng/work/iommu-platform/out/innovation-rescue-20261009/model-implementation-contract.md:7)。本席不运行/代写routine脚本。

首版选择每flow只有一个未退休短AW、`q<=B`，上一band各成员W/B都清完才提交下一band；A的返RfirstVALID必须落在其已offerW覆盖范围内。这比本稿允许额外privateparking的通用变体更保守，但可以把退出cut缩到旧短W末端，并简化work界。root探针q2/B3；本稿q3/B3是容量上限手推，不是主实验参数。

**必须补足的 ingress 条件：**模型把required mask在physicalR进入single-beat skid时冻结。若下一band尚未offer A AW，却提前以mask3吸入其第一个beat，之后A pause，则A因无offeredW coverage不能获得RVALID；该mask又被冻结，A不能从此held beat脱离，V会被一个从未向AfirstVALID的内部phantom obligation阻塞。

一个最小状态反例是上一band完成、A `returned=q`、没有未退休AW，skid已提前装了index q/mask3但A outputVALID因coverage为0；此时pauseA。不能通过永远等待一个被暂停的新AW修复它。

推荐首版只在以下条件下从physical R接收入skid：未paused且将被列为member的每flow，均已offer覆盖这一beat的W；已paused且旧W末端之前的flow仍保留；已paused且无旧义务的flow在**入skid前**从新mask排除。尚未满足条件时可保持外部physicalRVALID、拉低READY；不撤原物理R。另一合法设计是仅first-sinkVALID才锁该成员，但其state合同比首版更复杂。

**work界单位：**gate保证snapshot `returned_A<=debt_end_A`；严格连续W且最多一条q短AW时，pause后新增A返R至多q beats，待排W也至多q beats。故`≤q`是新增R计数界，R+W transferred work最多`2q`，AW/B/heldheader以及公平等待时间另计，不能变成周期界。

**恢复：**A必须从自己实际returned补读，不能一resume就重新加入尚在途且全局index已越过其cursor的原fusedR；那会跳过中间数据。closed member集合、独立补读child和原parent最后RLAST需保留。

**可达范围：**两个源结果/目的recipe均预备成功，原logical source parents已接收，新的legalizer可在首次virtualAW之前生成q片，且已有W/skid/routes正常服务，足以支持受限事件模型的正确性合同。其值来自真正可执行事件状态才算模型验收；该抽象不能自动证明当前RTL bank gate、AXI连接、PTW或cycle收益。root负责之后的critical-source/DUT判别。
